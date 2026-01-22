#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "DooFamily1";
const char* password = "b90006924";

ESP8266WebServer server(80);

int blinkDelay = 500;      // ms
bool ledState = false;
unsigned long lastBlink = 0;

void handleRoot() {
  if (server.hasArg("delay")) {
    blinkDelay = server.arg("delay").toInt();
  }

  String page = "<html><body>";
  page += "<h2>LED Blink Control</h2>";
  page += "<p>Current delay: " + String(blinkDelay) + " ms</p>";
  page += "<form>";
  page += "<input type='number' name='delay' placeholder='ms'>";
  page += "<input type='submit' value='Set'>";
  page += "</form>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  delay(300);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  if (millis() - lastBlink >= blinkDelay) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);
  }
}
