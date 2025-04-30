#include <WiFi.h>
#include <HTTPClient.h>
#include <GxEPD2_BW.h>
#include <SPI.h>

// ✅ GxEPD2-Klasse für dein 4.26" Display mit GD7965 (800x480)
#include <GxEPD2_BW.h>
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));

// ✅ WLAN-Zugangsdaten
const char* ssid     = "404 network unavailable";
const char* password = "Albert0905!";

// ✅ Cloudinary-Link zur 800x480 BMP-Datei
const char* imageUrl = "https://res.cloudinary.com/dsmcpyraq/image/upload/latest.bmp";

// ✅ Zeitsteuerung (jede Stunde)
unsigned long lastFetch = 0;
const unsigned long fetchInterval = 60 * 1000; //* 60 * 1000; // 1 Stunde

// Bildgröße (in Pixeln)
#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 480

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WLAN verbunden.");

  display.init();
  display.setRotation(1);

  fetchAndDisplayImage(); // erstes Bild direkt laden
}

void loop()
{
  if (millis() - lastFetch > fetchInterval) {
    fetchAndDisplayImage();
  }

  delay(1000);
}

void fetchAndDisplayImage()
{
  lastFetch = millis();

  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  Serial.println("📥 Lade Bild von Cloudinary...");
  http.begin(imageUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    WiFiClient* stream = http.getStreamPtr();

    display.setFullWindow();
    display.firstPage();
    do {
      uint8_t lineBuffer[IMAGE_WIDTH / 8]; // 800px / 8 = 100 Bytes pro Zeile

      for (int y = 0; y < IMAGE_HEIGHT; y++) {
        size_t bytesRead = stream->readBytes(lineBuffer, sizeof(lineBuffer));
        if (bytesRead == sizeof(lineBuffer)) {
          display.drawBitmap(0, y, lineBuffer, IMAGE_WIDTH, 1, GxEPD_BLACK);
        } else {
          Serial.printf("❌ Zu wenig Daten (%d Bytes) in Zeile %d\n", bytesRead, y);
          break;
        }
      }
    } while (display.nextPage());

    Serial.println("🖼️ Bildanzeige abgeschlossen.");
  } else {
    Serial.print("❌ HTTP Fehler: ");
    Serial.println(httpCode);
  }

  http.end();
}
