#include <WiFi.h>
#include <HTTPClient.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <SPI.h>
#include <SD.h>

// Displayklasse für 4.26" 800x480 (GD7965)
// #include <GxEPD2_750_T7.h>
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(15, 27, 26, 25));

// WLAN-Zugang
const char* ssid     = "404 network unavailable";
const char* password = "Albert0905!";

// URL zum BMP
const char* imageUrl = "https://res.cloudinary.com/dsmcpyraq/image/upload/latest.bmp";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WLAN verbunden");

  display.init();
  display.setRotation(1);

  fetchAndDisplayBMP();
}

void loop() {
  // Nur einmal anzeigen
}

void fetchAndDisplayBMP()
{
  HTTPClient http;
  http.begin(imageUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    WiFiClient* stream = http.getStreamPtr();

    // BMP Header überspringen
    uint8_t bmpHeader[54];
    stream->readBytes(bmpHeader, 54);
    if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') {
      Serial.println("❌ Kein gültiger BMP Header!");
      return;
    }

    // Bilddaten-Offset (normal 54)
    uint32_t dataOffset = *(uint32_t*)&bmpHeader[10];
    uint32_t width = *(uint32_t*)&bmpHeader[18];
    uint32_t height = *(uint32_t*)&bmpHeader[22];
    uint16_t bpp = *(uint16_t*)&bmpHeader[28];

    if (bpp != 1 || width != 800 || height != 480) {
      Serial.printf("❌ Format nicht unterstützt: %dx%d, %d bpp\n", width, height, bpp);
      return;
    }

    // Rest bis Datenanfang überspringen
    stream->readBytes(nullptr, dataOffset - 54);

    display.setFullWindow();
    display.firstPage();
    do {
      for (int y = height - 1; y >= 0; y--) { // BMP ist von unten nach oben
        for (int x = 0; x < width; x += 8) {
          if (stream->available()) {
            uint8_t byte = stream->read();
            display.drawBitmap(x, y, &byte, 8, 1, GxEPD_BLACK);
          }
        }
      }
    } while (display.nextPage());

    Serial.println("✅ Bildanzeige abgeschlossen");
  } else {
    Serial.printf("❌ HTTP Fehler: %d\n", httpCode);
  }

  http.end();
}
