#include <WiFi.h>
#include <HTTPClient.h>
#include <GxEPD2_BW.h>
#include "GxEPD2_display_selection_new_style.h"
#include <SPI.h>

// ✅ WLAN-Zugangsdaten
const char* ssid     = "404 network unavailable";
const char* password = "Albert0905!";

// ✅ Feste Cloudinary-URL deines .bmp-Bildes (ohne Cache!)
const char* imageUrl = "https://bilder-uploader-five.vercel.app/latest.bmp";

// ⏱️ Zeitsteuerung: 1x pro Stunde abrufen
unsigned long lastFetch = 0;
const unsigned long fetchInterval = 60UL * 60UL * 1000UL; // 1 Stunde

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WLAN verbunden.");

  display.init();
  display.setRotation(1); // Querformat

  fetchAndDisplayBMP();
}

void loop()
{
  if (millis() - lastFetch > fetchInterval) {
    fetchAndDisplayBMP();
  }
  delay(1000);
}

void fetchAndDisplayBMP()
{
  lastFetch = millis();

  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(imageUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    WiFiClient* stream = http.getStreamPtr();

    // BMP Header überspringen
    byte header[54];
    stream->readBytes(header, 54);

    // Breite/Höhe auslesen (nur zur Kontrolle)
    int width  = header[18] + (header[19] << 8);
    int height = header[22] + (header[23] << 8);
    Serial.printf("🖼️ Bildgröße: %dx%d\n", width, height);

    display.setFullWindow();
    display.firstPage();
    do {
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 8; x++) {
          if (stream->available()) {
            uint8_t b = stream->read();
            display.drawBitmap(x * 8, height - 1 - y, &b, 8, 1, GxEPD_BLACK); // Umgedreht, da BMP von unten nach oben
          } else {
            Serial.printf("❌ Zu wenig Daten bei Zeile %d\n", y);
            break;
          }
        }
      }
    } while (display.nextPage());

    Serial.println("✅ Bildanzeige abgeschlossen.");
  } else {
    Serial.print("❌ HTTP Fehler: ");
    Serial.println(httpCode);
  }

  http.end();
}
