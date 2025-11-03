
const express = require("express");
const fs = require("fs").promises;
const path = require("path");
const bcrypt = require("bcrypt");
const { execFile } = require("child_process");
const morgan = require("morgan");

const app = express();
const PORT = process.env.PORT || 5000;


app.use(express.json());
app.use(morgan("dev"));


app.use((req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");
  if (req.method === "OPTIONS") return res.sendStatus(204);
  next();
});


const guiPath = path.join(__dirname, "../gui");
const dataDir = path.join(__dirname, "../data");
const userFile = path.join(dataDir, "users.json");


(async () => {
  try {
    await fs.mkdir(dataDir, { recursive: true });
    try {
      await fs.access(userFile);
    } catch {
      await fs.writeFile(userFile, JSON.stringify([], null, 2));
      console.log("🆕 Created users.json file");
    }
  } catch (err) {
    console.error("❌ Error initializing data directory:", err);
  }
})();


app.use(express.static(guiPath));


async function readUsers() {
  try {
    const data = await fs.readFile(userFile, "utf8");
    return JSON.parse(data || "[]");
  } catch (err) {
    console.error("❌ Error reading users file:", err);
    return [];
  }
}

async function writeUsers(users) {
  try {
    await fs.writeFile(userFile, JSON.stringify(users, null, 2));
  } catch (err) {
    console.error("❌ Error writing users file:", err);
  }
}


const pages = ["index", "signup", "forgotpassword", "dashboard"];
pages.forEach((page) => {
  app.get(`/${page === "index" ? "" : page}`, (_, res) =>
    res.sendFile(path.join(guiPath, `${page}.html`))
  );
});


function addPostRoute(route, handler) {
  app.post(route, handler);
  const short = route.replace(/^\/api/, "");
  if (short !== route) app.post(short, handler);
}

// SYNC USERS (localStorage → server)
addPostRoute("/api/sync-users", async (req, res) => {
  const { users } = req.body;
  if (!Array.isArray(users))
    return res.status(400).json({ success: false, error: "Invalid users format." });

  await writeUsers(users);
  console.log(`🔁 Synced ${users.length} users from localStorage`);
  res.json({ success: true, message: "Users synced successfully." });
});

// SIGNUP 
addPostRoute("/api/signup", async (req, res) => {
  const { username, password, age } = req.body;

  if (!username?.trim() || !password?.trim() || !age)
    return res.status(400).json({ success: false, error: "All fields required." });

  const users = await readUsers();
  if (users.some((u) => u.username === username))
    return res.status(409).json({ success: false, error: "Username already exists." });

  const hashed = await bcrypt.hash(password, 10);
  const userAge = parseInt(age);

  users.push({
    username,
    password: hashed,
    age: userAge,
    balance: 0,
    transactions: []
  });

  await writeUsers(users);
  console.log(`🟢 User registered: ${username} (Age: ${userAge})`);
  res.json({ success: true, message: "Signup successful." });
});

// LOGIN
addPostRoute("/api/login", async (req, res) => {
  const { username, password } = req.body;
  if (!username?.trim() || !password?.trim())
    return res.status(400).json({ success: false, error: "All fields required." });

  const users = await readUsers();
  const user = users.find((u) => u.username === username);
  if (!user)
    return res.status(401).json({ success: false, error: "Invalid username." });

  const match = await bcrypt.compare(password, user.password);
  if (!match)
    return res.status(401).json({ success: false, error: "Wrong password." });

  console.log(`✅ Login successful: ${username}`);
  res.json({ success: true, username, age: user.age });
});

// FORGOT PASSWORD
addPostRoute("/api/forgot-password", async (req, res) => {
  const { username, newPassword } = req.body;
  const users = await readUsers();
  const idx = users.findIndex((u) => u.username === username);

  if (idx === -1)
    return res.status(404).json({ success: false, error: "User not found." });

  users[idx].password = await bcrypt.hash(newPassword, 10);
  await writeUsers(users);

  console.log(`🔁 Password reset for: ${username}`);
  res.json({ success: true, message: "Password updated successfully." });
});


app.get("/api/users", async (_, res) => {
  const users = await readUsers();
  res.json({ success: true, users });
});


const UNDERAGE_LIMIT = 20;
const DEFAULT_LIMIT = 100;

const transactionTracker = {}; 

async function canTransact(username) {
  const users = await readUsers();
  const user = users.find((u) => u.username === username);
  const now = Date.now();
  const oneDay = 24 * 60 * 60 * 1000;

  if (!transactionTracker[username]) transactionTracker[username] = [];
  transactionTracker[username] = transactionTracker[username].filter(
    (ts) => now - ts < oneDay
  );

  const limit = user && user.age < 18 ? UNDERAGE_LIMIT : DEFAULT_LIMIT;

  if (transactionTracker[username].length >= limit) {
    console.warn(`🚫 ${username} reached daily limit (${limit}).`);
    return false;
  }

  transactionTracker[username].push(now);
  console.log(`📊 ${username}: ${transactionTracker[username].length}/${limit} today`);
  return true;
}


const exeName =
  process.platform === "win32"
    ? "BankingTransactionManager.exe"
    : "BankingTransactionManager";
const exePath = path.join(__dirname, exeName);

function runBackendCommand(args, res) {
  fs.access(exePath)
    .then(() => {
      console.log(`▶️ Running backend: ${exeName} ${args.join(" ")}`);
      execFile(exePath, args, { timeout: 20000 }, (error, stdout, stderr) => {
        if (error) {
          console.error("❌ Backend error:", error.message);
          return res.status(500).json({ success: false, error: error.message });
        }
        if (stderr) console.warn("⚠️ Backend stderr:", stderr.trim());
        const output = stdout.trim() || "No output from backend";
        console.log("💬 Output:", output);
        res.json({ success: true, output });
      });
    })
    .catch(() => {
      res.status(404).json({
        success: false,
        error: "Backend executable not found. Please compile BankingTransactionManager.",
      });
    });
}


app.post("/api/deposit", async (req, res) => {
  const { username, amount } = req.body;
  if (!username || isNaN(amount))
    return res.status(400).json({ success: false, error: "Invalid deposit request." });

  if (!(await canTransact(username)))
    return res.status(429).json({
      success: false,
      error: "Daily transaction limit reached. Try again tomorrow.",
    });

  runBackendCommand(["deposit", username, String(amount)], res);
});

app.post("/api/withdraw", async (req, res) => {
  const { username, amount } = req.body;
  if (!username || isNaN(amount))
    return res.status(400).json({ success: false, error: "Invalid withdrawal request." });

  if (!(await canTransact(username)))
    return res.status(429).json({
      success: false,
      error: "Daily transaction limit reached. Try again tomorrow.",
    });

  runBackendCommand(["withdraw", username, String(amount)], res);
});

app.post("/api/transfer", async (req, res) => {
  const { fromUser, toUser, amount } = req.body;
  if (!fromUser || !toUser || isNaN(amount))
    return res.status(400).json({ success: false, error: "Invalid transfer request." });

  if (!(await canTransact(fromUser)))
    return res.status(429).json({
      success: false,
      error: "Daily transaction limit reached. Try again tomorrow.",
    });

  runBackendCommand(["transfer", fromUser, toUser, String(amount)], res);
});

app.post("/api/undo", (_, res) => runBackendCommand(["undo"], res));
app.post("/api/redo", (_, res) => runBackendCommand(["redo"], res));
app.get("/api/mini-statement", (_, res) => runBackendCommand(["mini-statement"], res));


app.use((_, res) =>
  res.status(404).json({ success: false, error: "Endpoint not found." })
);


app.listen(PORT, () => {
  console.log(`🚀 Server running at: http://localhost:${PORT}`);
  console.log(`📂 Serving frontend from: ${guiPath}`);
  console.log(`⚙️ Backend executable path: ${exePath}`);
});
