// html_template.h
#ifndef HTML_TEMPLATE_H
#define HTML_TEMPLATE_H

const char* root_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WiFi Configuration</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      background-color: #fff;
      padding: 20px;
      box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
      border-radius: 8px;
      width: 90%;
      max-width: 500px;
      box-sizing: border-box;
    }
    h1 {
      font-size: 24px;
      margin-bottom: 20px;
      text-align: center;
      color: #333;
    }
    form {
      display: flex;
      flex-direction: column;
    }
    .input-group {
      position: relative;
      margin-bottom: 15px;
    }
    input[type="text"],
    input[type="password"],
    input[type="number"] {
      padding: 10px;
      width: 100%;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 16px;
      box-sizing: border-box;
    }
    .toggle-password {
      position: absolute;
      right: 10px;
      top: 50%;
      transform: translateY(-50%);
      cursor: pointer;
      font-size: 16px;
      line-height: 16px;
    }
    input[type="submit"] {
      padding: 10px;
      background-color: #4CAF50;
      color: #fff;
      border: none;
      border-radius: 4px;
      font-size: 16px;
      cursor: pointer;
      transition: background-color 0.3s ease;
    }
    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>WiFi Configuration</h1>
    <form action="/submit" method="POST">
      <div class="input-group">
        <input type="text" name="ssid" placeholder="WiFi SSID" value="%s" required>
      </div>
      <div class="input-group">
        <input type="password" name="password" placeholder="WiFi Password" value="%s" required>
        <span class="toggle-password">👁️</span>
      </div>
      <div class="input-group">
        <input type="text" name="apSSID" placeholder="AP SSID" value="%s" required>
      </div>
      <div class="input-group">
        <input type="password" name="apPassword" placeholder="AP Password" value="%s" required>
        <span class="toggle-password">👁️</span>
      </div>
      <div class="input-group">
        <input type="text" name="localIP" placeholder="Local IP" value="%s" required>
      </div>
      <div class="input-group">
        <input type="text" name="subnetMask" placeholder="Subnet Mask" value="%s" required>
      </div>
      <div class="input-group">
        <input type="text" name="gateWay" placeholder="Gate Way" value="%s" required>
      </div>
      <div class="input-group">
        <input type="text" name="tcpPort" placeholder="TCP Server Port" value="%s" required>
      </div>
      <input type="submit" value="Submit">
    </form>
  </div>
  <script>
    document.querySelectorAll('.toggle-password').forEach(item => {
      item.addEventListener('click', event => {
        let input = item.previousElementSibling;
        if (input.type === 'password') {
          input.type = 'text';
          item.textContent = '🙈';
        } else {
          input.type = 'password';
          item.textContent = '👁️';
        }
      });
    });
  </script>
</body>
</html>
)rawliteral";

#endif // HTML_TEMPLATE_H
