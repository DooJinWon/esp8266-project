#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

// -------- WiFi credentials --------
const char* ssid = "DooFamily1";
const char* password = "b90006924";

// -------- Static IP configuration --------
IPAddress local_IP(192, 168, 0, 74);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(8, 8, 8, 8);
IPAddress dns2(8, 8, 4, 4);

// -------- Server (PC/Cloud) --------
const char* SERVER_HOST = "192.168.0.26";  // <-- 서버 PC IP로 변경
const uint16_t SERVER_PORT = 8080;
const char* SERVER_PATH = "/api/sensor";

// -------- Server & Sensor --------
ESP8266WebServer server(80);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double ambientTemp = 0.0;
double objectTemp  = 0.0;

void handleRoot() {
  String page = "<html><head>";
  page += "<meta http-equiv='refresh' content='2'>";
  page += "</head><body>";
  page += "<h2>HW-691 Temperature Monitor</h2>";
  page += "<p><b>Ambient Temperature:</b> " + String(ambientTemp, 2) + " &deg;C</p>";
  page += "<p><b>Object Temperature:</b> " + String(objectTemp, 2) + " &deg;C</p>";
  page += "<p><b>IP:</b> " + WiFi.localIP().toString() + "</p>";
  page += "<p>(Device will sleep in 5 seconds)</p>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

bool postToServer() {
  WiFiClient client;
  if (!client.connect(SERVER_HOST, SERVER_PORT)) {
    return false;
  }

  String payload = "{";
  payload += "\"ambient_c\":" + String(ambientTemp, 2) + ",";
  payload += "\"object_c\":"  + String(objectTemp, 2);
  payload += "}";

  client.print(String("POST ") + SERVER_PATH + " HTTP/1.1\r\n");
  client.print(String("Host: ") + SERVER_HOST + "\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print(String("Content-Length: ") + payload.length() + "\r\n");
  client.print("Connection: close\r\n\r\n");
  client.print(payload);

  // 간단 응답 소비 (2초 제한)
  uint32_t t0 = millis();
  while (client.connected() && millis() - t0 < 2000) {
    while (client.available()) client.read();
  }
  client.stop();
  return true;
}

void setup() {
  // -------- I2C --------
  Wire.begin(D2, D1);

  if (!mlx.begin()) {
    // 센서 없으면 그냥 슬립
    ESP.deepSleep(60 * 1000000ULL);
  }

  // -------- Read sensor once --------
  ambientTemp = mlx.readAmbientTempC();
  objectTemp  = mlx.readObjectTempC();

  // -------- WiFi (Static IP) --------
  WiFi.mode(WIFI_STA);
  WiFi.config(local_IP, gateway, subnet, dns1, dns2);
  WiFi.begin(ssid, password);

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(200);
  }

  if (WiFi.status() != WL_CONNECTED) {
    ESP.deepSleep(60 * 1000000ULL);
  }

  // -------- Send to server once --------
  postToServer();

  // -------- Optional: local web page for debugging (awake window) --------
  server.on("/", handleRoot);
  server.begin();

  uint32_t webTime = millis();
  while (millis() - webTime < 5000) {
    server.handleClient();
    delay(1);
  }

  // -------- Deep Sleep --------
  ESP.deepSleep(60 * 1000000ULL);
}

void loop() {}
