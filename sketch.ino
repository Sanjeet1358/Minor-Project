#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>
#include <MPU6050.h>
#include <math.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// ---------------- PIN DEFINITIONS ----------------
#define POT_SPO2 34
#define POT_BPM  35
#define LED_PIN 14
#define BUZZER_PIN 27
#define DS18B20_PIN 13

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- DS18B20 ----------------
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

// ---------------- MPU6050 ----------------
MPU6050 mpu;

// ---------------- WEB SERVER ----------------
WebServer server(80);

// ---------------- WIFI ----------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------------- WHATSAPP ----------------
const char* WHATSAPP_PHONE  = "9779746264051";
const char* WHATSAPP_APIKEY = "bFDFSm2PTaQG";

// ---------------- THINGSPEAK ----------------
const char* THINGSPEAK_API_KEY = "INMKD1321CBLDB2O";
const char* THINGSPEAK_URL = "http://api.thingspeak.com/update";

// ---------------- VARIABLES ----------------
float temperature;
int spo2Value;
int bpmValue;

int16_t rawAx, rawAy, rawAz;
float ax, ay, az;
float accelMag;

unsigned long alertStart = 0;
bool alertActive = false;
unsigned long lastThingSpeakUpdate = 0;

// ---------- SEND WHATSAPP ALERT ----------
void sendWhatsAppAlert(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  message.replace(" ", "%20");
  message.replace("\n", "%0A");

  String url = "/send.php?phone=" + String(WHATSAPP_PHONE) +
               "&text=" + message +
               "&apikey=" + String(WHATSAPP_APIKEY);

  if (!client.connect("api.textmebot.com", 443)) return;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.textmebot.com\r\n" +
               "Connection: close\r\n\r\n");

  client.stop();
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  sensors.begin();
  mpu.initialize();

  lcd.init();
  lcd.backlight();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) delay(300);

  lcd.clear();
  lcd.print("WiFi Connected");
}

// ---------------- LOOP ----------------
void loop() {
  // ---------- READ SENSORS ----------
  spo2Value = map(analogRead(POT_SPO2), 0, 4095, 90, 100);
  bpmValue  = map(analogRead(POT_BPM), 0, 4095, 50, 150);

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  mpu.getAcceleration(&rawAx, &rawAy, &rawAz);
  ax = rawAx / 16384.0;
  ay = rawAy / 16384.0;
  az = rawAz / 16384.0;
  accelMag = sqrt(ax * ax + ay * ay + az * az);

  // ---------- SERIAL PLOTTER OUTPUT WITH CLEAR NAMES ----------
  Serial.print("Temp:"); Serial.print(temperature); Serial.print(",");
  Serial.print("SpO2:"); Serial.print(spo2Value); Serial.print(",");
  Serial.print("BPM:"); Serial.print(bpmValue); Serial.print(",");
  Serial.print("Accel:"); Serial.println(accelMag);

  // ---------- ALERT ----------
  if ((temperature > 38 || bpmValue > 120 || accelMag > 2.5) && !alertActive) {
    alertActive = true;
    alertStart = millis();

    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500);

    String msg = "HEALTH ALERT\n";
    msg += "Temp: " + String(temperature) + "C\n";
    msg += "BPM: " + String(bpmValue) + "\n";
    msg += "SpO2: " + String(spo2Value) + "%\n";
    msg += "Fall Detected";

    sendWhatsAppAlert(msg);
  }

  if (alertActive && millis() - alertStart > 3000) {
    alertActive = false;
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // ---------- LCD DISPLAY ----------
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print(" B:");
  lcd.print(bpmValue);

  lcd.setCursor(0, 1);
  lcd.print("SpO2:");
  lcd.print(spo2Value);
  lcd.print("%   ");

  // ---------- THINGSPEAK UPDATE ----------
  if (millis() - lastThingSpeakUpdate > 15000) { // 15 seconds
    lastThingSpeakUpdate = millis();

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(THINGSPEAK_URL) + "?api_key=" + THINGSPEAK_API_KEY +
                   "&field1=" + String(temperature) +
                   "&field2=" + String(spo2Value) +
                   "&field3=" + String(bpmValue) +
                   "&field4=" + String(accelMag);
      http.begin(url);
      int code = http.GET(); // Send request
      http.end();
    }
  }

  delay(300);
}
