const express = require("express");
const cors = require("cors");
const path = require("path");
const fs = require("fs");

const app = express();
const PORT = 3000;

// ✅ Middleware
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// ✅ Serve GUI files
const guiPath = path.join(__dirname, "../gui");
app.use(express.static(guiPath));

// ✅ Login Endpoint
app.post("/api/login", (req, res) => {
  const { username, password } = req.body;
  const userFile = path.join(__dirname, "users.json");

  try {
    if (!fs.existsSync(userFile)) {
      return res.status(404).json({ success: false, message: "User data not found" });
    }

    const users = JSON.parse(fs.readFileSync(userFile, "utf8"));
    const user = users.find(
      (u) => u.username === username && u.password === password
    );

    if (user) {
      return res.json({ success: true, message: "Login successful", user });
    } else {
      return res.status(401).json({ success: false, message: "Invalid credentials" });
    }
  } catch (err) {
    console.error("Login error:", err);
    res.status(500).json({ success: false, message: "Server error" });
  }
});

// ✅ Default route
app.get("/", (req, res) => {
  res.sendFile(path.join(guiPath, "index.html"));
});

// ✅ Start server
app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});
