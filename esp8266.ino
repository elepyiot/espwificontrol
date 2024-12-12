#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// تعریف پین خروجی
const int outputPin = 12; // GPIO12 
const int outputPin1 = 13; // GPIO13 
// تنظیمات شبکه هات‌اسپات
const char* ssid = "GadgetMasters";      
const char* password = "12345678";   

// ایجاد سرور وب
ESP8266WebServer server(80);

// صفحه HTML
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Hotspot</title>
</head>
<body style="text-align:center;">
  <h3>Gadget Masters</h3>
  <button onclick="sendRequest('/on')">out on</button>
  <button onclick="sendRequest('/off')">out off</button>
  <script>
    function sendRequest(path) {
      fetch(path).then(response => {
        console.log("Request sent to: " + path);
      });
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  // تنظیمات پایه
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);
   pinMode(outputPin1, OUTPUT);
  digitalWrite(outputPin1, HIGH);

  // آغاز ارتباط سریال
  Serial.begin(115200);
  Serial.println();

  // تنظیم ماژول به حالت Access Point
  WiFi.softAP(ssid, password);
  Serial.print("Access Point started. IP: ");
  Serial.println(WiFi.softAPIP()); // نمایش IP ماژول

  // تعریف مسیرهای HTTP
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/on", []() {
    digitalWrite(outputPin, HIGH);
    digitalWrite(outputPin1, LOW);
    server.send(200, "text/plain", "Device is ON");
  });

  server.on("/off", []() {
    digitalWrite(outputPin, LOW);
    digitalWrite(outputPin1, HIGH);
    server.send(200, "text/plain", "Device is OFF");
  });

  // آغاز سرور
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
