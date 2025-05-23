const express = require("express");
const cors = require("cors");
const fs = require("fs");
const path = require("path");
const { exec } = require("child_process");

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static(__dirname)); // serve index.html and other static files

app.post("/run", (req, res) => {
  const code = req.body.code;
  const tempFile = path.join(__dirname, "temp.compii");
  const compilerPath = path.join(__dirname, "compii.exe");

  fs.writeFileSync(tempFile, code);

  exec(`"${compilerPath}" "${tempFile}"`, (error, stdout, stderr) => {
    if (error) {
      return res.json({ output: stderr || error.message });
    }
    res.json({ output: stdout });
  });
});

const PORT = 5000;
app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});