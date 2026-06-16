#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <time.h>

// ===== WIFI =====
const char* ssid     = "DNA-WIFI-2564";
const char* password = "password";

// ===== NTP =====
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600*3;   // adjust to your timezone
const int   daylightOffset_sec = 0;

// ===== WEB =====
WebServer server(80);

// Format time
String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Time not ready";
  }
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

// Root page
void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 Local Time</title>
    <script>
    setTimeout(() => location.reload(), 1000);
  </script>
</head>
<body>
  <h2>ESP32 Local Time</h2>
  <p>)rawliteral" + String(analogRead(39))+ getTimeString() + R"rawliteral(</p>

  <h2>Se toimii</h2>

  <p>Sain viimeinkin tehtyä web-serverin Esp32:lla. Täytyi vain löytää oikea malli. Oikea malli oli YouTube-video "ESP32 Web Server Step by Step | Time, Temperature & Modern UI".</p>
  <p>Boardiksi ESP32S3 Dev Module. Arduino IDE 2.37. Selaimeen auki http://myhome.local/</p>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);        // 12-bit: 0–4095
  analogSetAttenuation(ADC_11db);  // full range: 0–3.3V
    ledcAttach(17, 5000, 8);  // any free GPIO

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // mDNS name -> myhome.local
  if (!MDNS.begin("myhome")) {
    Serial.println("mDNS failed");
  } else {
    Serial.println("mDNS started: http://myhome.local/");
  }

  // Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Web server
  server.on("/", handleRoot);
  server.begin();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  //Serial.print("Random number: "+String(random(0,100)));

  ledcWrite(17, 128);        // ~1.65V average (0–255 range)
  Serial.println("Voltage 32: "+String(analogRead(32)*1000));
  Serial.println("Voltage 33: "+String(analogRead(33)*1000));
  Serial.println("Voltage 34: "+String(analogRead(34)*1000));
  Serial.println("Voltage 35: "+String(analogRead(35)*1000));
  Serial.println("Voltage 36: "+String(analogRead(36)*1000));
  Serial.println("Voltage 39: "+String(analogRead(39)*1000));
  delay(500);
}
