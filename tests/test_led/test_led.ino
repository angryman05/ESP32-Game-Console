#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  17
// SCK = 18, MOSI = 23 -> SPI hardware implicit

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

int bx = 80, by = 64, dx = 2, dy = 2;   // mingea
const int R = 4;

void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);   // vezi depanarea dacă culorile/marginile sunt ciudate
  tft.setRotation(1);          // landscape: 160 x 128

  // test culori
  tft.fillScreen(ST77XX_RED);   delay(400);
  tft.fillScreen(ST77XX_GREEN); delay(400);
  tft.fillScreen(ST77XX_BLUE);  delay(400);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("SYNC 3in1");
  delay(1500);
  tft.fillScreen(ST77XX_BLACK);

  // palete de pong decorative
  tft.fillRect(6, 44, 4, 40, ST77XX_WHITE);
  tft.fillRect(150, 44, 4, 40, ST77XX_WHITE);
}

void loop() {
  tft.fillCircle(bx, by, R, ST77XX_BLACK);      // șterge mingea veche
  bx += dx; by += dy;
  if (bx < 14 + R || bx > 146 - R) dx = -dx;    // ricoșează între palete
  if (by < R || by > 127 - R)      dy = -dy;
  tft.fillCircle(bx, by, R, ST77XX_WHITE);      // desenează mingea nouă
  delay(16);                                     // ~60 fps
}