#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <AnimatedGIF.h>
#include "../test_images/weather2.h"

#define GIF_IMAGE weather2

// Konfiguracja TFT
TFT_eSPI tft = TFT_eSPI();
#define DISPLAY_WIDTH  tft.width()
#define DISPLAY_HEIGHT tft.height()
#define BUFFER_SIZE 256

#define COLORT1 TFT_BLACK
#define COLORT2 TFT_GREEN
#define COLORT3 TFT_RED
#define COLOR_RED TFT_RED
#define COLOR_YELLOW TFT_YELLOW
#define COLOR_WHITE TFT_WHITE

// Definicje pinów
#define LED_RED 27
#define LED_GREEN 25
#define LED_WHITE 33
#define FOTO_RES 34

Adafruit_BME280 bme;

// Bufory dla DMA
#ifdef USE_DMA
uint16_t usTemp[2][BUFFER_SIZE];
#else
uint16_t usTemp[1][BUFFER_SIZE];
#endif
bool dmaBuf = 0;

// Zmienne czasowe
unsigned long previousGIFMillis = 0;
unsigned long previousSensorMillis = 0;
unsigned long previousLEDMillis = 0;
unsigned long previousClientMillis = 0;
const long intervalGIF = 50;
const long intervalSensor = 1000;
const long intervalLED = 1000;

bool ledState = false;

int foto_raw = 0;
float foto_res = 0.0;
float bmpT, bmpP, bmpH;

time_t now;

String sOut = "";
byte autorefresh = 5;
int ile = 0;

// Konfiguracja WiFi
const char* ssid = "realmeGT";
const char* password = "sylwia12345";

WebServer webServer(80);
WebSocketsServer webSocket(81);
AnimatedGIF gif;

void handleRoot() {
    if (SPIFFS.exists("/index.html")) {
        File file = SPIFFS.open("/index.html", "r");
        if (file) {
            webServer.streamFile(file, "text/html");
            file.close();
        } else {
            webServer.send(500, "text/plain", "Internal Server Error: Unable to open file.");
        }
    } else {
        webServer.send(404, "text/plain", "File not found.");
    }
}

void handleNotFound() {
    webServer.send(404, "text/plain", "Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connection from %s\n", num, ip.toString().c_str());
        }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] Text: %s\n", num, payload);
            break;
    }
}

void setup() {
    Serial.begin(115200);

    tft.begin();
#ifdef USE_DMA
    tft.initDMA();
#endif
    tft.setRotation(0);
    Serial.print("TFT Init OK");

    tft.fillScreen(TFT_BLACK);
    gif.begin(BIG_ENDIAN_PIXELS);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);

    delay(1000);

    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    } else {
        Serial.println("BME280 sensor OK");
    }

    if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw)) {
        Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
        tft.startWrite();
    }

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        i = i + 1;
        if (i % 2 == 0) {
            Serial.print(".");
        } else {
            Serial.print("+");
        }
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("Server started");
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
    } else {
        Serial.println("SPIFFS mounted successfully");
    }

    webServer.on("/", handleRoot);
    webServer.onNotFound(handleNotFound);
    webServer.begin();
    Serial.println("HTTP server started");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started");
}

void loop() {
    unsigned long currentMillis = millis();

    webServer.handleClient();
    webSocket.loop();
    if (currentMillis - previousGIFMillis >= intervalGIF) {
        previousGIFMillis = currentMillis;

        if (!gif.playFrame(true, NULL)) {
              gif.close();
              gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw);
        }
    }

    tft.setTextSize(2);
    tft.setTextColor(COLOR_WHITE);
    tft.setCursor(30, 155, 1);
    tft.print("TWOJA POGODYNKA");
    
    if (currentMillis - previousSensorMillis >= intervalSensor) {
        previousSensorMillis = currentMillis;

        foto_raw = analogRead(FOTO_RES);
        foto_res = foto_raw / 4095.0 * 100.0;

        bmpT = bme.readTemperature();
        bmpH = bme.readHumidity();
        bmpP = bme.readPressure() / 100.0;

        digitalWrite(LED_RED, bmpT >= 29.0 ? HIGH : LOW);
        digitalWrite(LED_GREEN, bmpH >= 60.0 ? HIGH : LOW);
        digitalWrite(LED_WHITE, foto_res < 5.0 ? HIGH : LOW);

        tft.setTextColor(COLOR_WHITE);
        tft.setTextSize(2);
        
        tft.fillRect(0, 180, 240, 16, TFT_BLACK);
        tft.setCursor(0, 180, 1);
        String foto_text = "Fotorezystor " + String(foto_res) + " %";
        tft.println(foto_text);
    
        tft.fillRect(0, 200, 240, 16, TFT_BLACK);
        tft.setCursor(0, 200, 1);
        String temp_text = "Temperatura " + String(bmpT) + " C";
        tft.println(temp_text);
    
        tft.fillRect(0, 220, 240, 16, TFT_BLACK);
        tft.setCursor(0, 220, 1);
        String pres_text = "Cisnienie " + String(bmpP) + " hPa";
        tft.println(pres_text);
    
        tft.fillRect(0, 240, 240, 16, TFT_BLACK);
        tft.setCursor(0, 240, 1);
        String hum_text = "Wilgotnosc " + String(bmpH) + " %";
        tft.println(hum_text);
    
        tft.setTextSize(1);
        tft.fillRect(0, 260, 240, 16, TFT_BLACK);
        tft.setCursor(0, 260, 1);
        //tft.println(getDataTime());
    
        String message = String(foto_res);
        String mess_temp = String(bmpT);
        String mess_hum = String(bmpH);
        String combinedMessage = message + "," + mess_temp + "," + mess_hum;
        webSocket.broadcastTXT(combinedMessage.c_str(), combinedMessage.length());
    }

    
}

// Draw a line of image directly on the LCD
void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s;
  uint16_t *d, *usPalette;
  int x, y, iWidth, iCount;

  // Display bounds check and cropping
  iWidth = pDraw->iWidth;
  if (iWidth + pDraw->iX > DISPLAY_WIDTH)
    iWidth = DISPLAY_WIDTH - pDraw->iX;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line
  if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
    return;

  // Old image disposal
  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) { // restore to background color
    for (x = 0; x < iWidth; x++) {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) { // if transparency used
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth) {
      c = ucTransparent - 1;
      d = &usTemp[0][0];
      while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE) {
        c = *s++;
        if (c == ucTransparent) { // done, stop
          s--; // back up to treat it like transparent
        } else { // opaque
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) { // any opaque pixels?
        // DMA would degrade performance here due to short line segments
        tft.setAddrWindow(pDraw->iX + x, y, iCount, 1);
        tft.pushPixels(usTemp, iCount);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)
          x++;
        else
          s--;
      }
    }
  } else {
    s = pDraw->pPixels;

    // Unroll the first pass to boost DMA performance
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    if (iWidth <= BUFFER_SIZE)
      for (iCount = 0; iCount < iWidth; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];
    else
      for (iCount = 0; iCount < BUFFER_SIZE; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA // 71.6 fps (ST7796 84.5 fps)
    tft.dmaWait();
    tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
    tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
    dmaBuf = !dmaBuf;
#else // 57.0 fps
    tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
    tft.pushPixels(&usTemp[0][0], iCount);
#endif

    iWidth -= iCount;
    // Loop if pixel buffer smaller than width
    while (iWidth > 0) {
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      if (iWidth <= BUFFER_SIZE)
        for (iCount = 0; iCount < iWidth; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];
      else
        for (iCount = 0; iCount < BUFFER_SIZE; iCount++) usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA
      tft.dmaWait();
      tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
      dmaBuf = !dmaBuf;
#else
      tft.pushPixels(&usTemp[0][0], iCount);
#endif
      iWidth -= iCount;
    }
  }
}
