// app2.js — Frontend Logic for Banking Transaction Manager

const BASE_URL = "http://localhost:5000";

// ------------------- LOGIN -------------------
async function loginUser(event) {
  event.preventDefault();

  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value.trim();

  if (!username || !password) {
    alert("⚠️ Please enter both username and password!");
    return;
  }

  try {
    const response = await fetch(`${BASE_URL}/login`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, password }),
    });

    const result = await response.json();

    if (result.success) {
      alert("✅ Login successful!");
      localStorage.setItem("username", result.username);
      window.location.href = "dashboard.html";
    } else {
      alert("❌ " + result.message);
    }
  } catch (error) {
    console.error("Login Error:", error);
    alert("⚠️ Unable to connect to the server.");
  }
}

// ------------------- SIGNUP -------------------
async function signupUser(event) {
  event.preventDefault();

  const username = document.getElementById("username").value.trim();
  const email = document.getElementById("email").value.trim();
  const contact = document.getElementById("contact").value.trim();
  const password = document.getElementById("password").value.trim();
  const confirmPassword = document.getElementById("confirmPassword").value.trim();

  if (!username || !email || !contact || !password || !confirmPassword) {
    alert("⚠️ Please fill in all fields!");
    return;
  }

  try {
    const response = await fetch(`${BASE_URL}/signup`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, email, contact, password, confirmPassword }),
    });

    const result = await response.json();

    if (result.success) {
      alert("✅ Signup successful! You can now log in.");
      window.location.href = "index.html";
    } else {
      alert("❌ " + result.message);
    }
  } catch (error) {
    console.error("Signup Error:", error);
    alert("⚠️ Unable to connect to the server.");
  }
}

// ------------------- FORGOT PASSWORD -------------------
async function resetPassword(event) {
  event.preventDefault();

  const username = document.getElementById("username").value.trim();
  const contact = document.getElementById("contact").value.trim();
  const newPassword = document.getElementById("newPassword").value.trim();

  if (!username || !contact || !newPassword) {
    alert("⚠️ Please fill in all fields!");
    return;
  }

  try {
    const response = await fetch(`${BASE_URL}/forgot-password`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, contact, newPassword }),
    });

    const result = await response.json();

    if (result.success) {
      alert("✅ Password updated successfully!");
      window.location.href = "index.html";
    } else {
      alert("❌ " + result.message);
    }
  } catch (error) {
    console.error("Forgot Password Error:", error);
    alert("⚠️ Unable to connect to the server.");
  }
}

// ------------------- DASHBOARD -------------------
function loadDashboard() {
  const username = localStorage.getItem("username");
  if (!username) {
    alert("⚠️ Please log in first!");
    window.location.href = "index.html";
    return;
  }

  document.getElementById("welcomeUser").textContent = `Welcome, ${username}!`;
}

// Logout
function logoutUser() {
  localStorage.removeItem("username");
  window.location.href = "index.html";
}

// ------------------- PAGE LINKERS -------------------
// Attach functions dynamically based on which page is open
document.addEventListener("DOMContentLoaded", () => {
  const page = window.location.pathname;

  if (page.includes("index.html")) {
    const form = document.getElementById("loginForm");
    if (form) form.addEventListener("submit", loginUser);
  } else if (page.includes("signup.html")) {
    const form = document.getElementById("signupForm");
    if (form) form.addEventListener("submit", signupUser);
  } else if (page.includes("forgotpassword.html")) {
    const form = document.getElementById("forgotForm");
    if (form) form.addEventListener("submit", resetPassword);
  } else if (page.includes("dashboard.html")) {
    loadDashboard();
    const logoutBtn = document.getElementById("logoutBtn");
    if (logoutBtn) logoutBtn.addEventListener("click", logoutUser);
  }
});
