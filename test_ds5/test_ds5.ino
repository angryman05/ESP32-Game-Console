#include <Bluepad32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS  5
#define TFT_DC  16
#define TFT_RST 17
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

ControllerPtr pad = nullptr;

int px = 76, py = 60;            // pozitia patratului
const int S = 8;                 // marimea patratului
uint16_t culoare = ST77XX_GREEN;
bool prevCross = false, prevCircle = false;

void ecranAsteptare() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 50);  tft.print("Tine apasat CREATE + PS");
  tft.setCursor(10, 65);  tft.print("pana clipeste rapid...");
}

void onConnected(ControllerPtr ctl) {
  if (pad == nullptr) {
    pad = ctl;
    Serial.printf("Controller conectat: %s\n", ctl->getModelName().c_str());
    ctl->setColorLED(0, 0, 255);             // lightbar albastru
    tft.fillScreen(ST77XX_BLACK);
    tft.fillRect(px, py, S, S, culoare);
  }
}

void onDisconnected(ControllerPtr ctl) {
  if (pad == ctl) {
    pad = nullptr;
    Serial.println("Controller deconectat");
    ecranAsteptare();
  }
}

void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);                        // 160 x 128
  ecranAsteptare();

  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();                // la fiecare pornire cere pairing nou (ok pt test)
}

void loop() {
  BP32.update();

  if (pad && pad->isConnected() && pad->hasData()) {
    int nx = px, ny = py;

    // stick stanga: valori aprox -512..512, ignoram zona moarta din centru
    int ax = pad->axisX(), ay = pad->axisY();
    if (abs(ax) > 80) nx += ax / 128;
    if (abs(ay) > 80) ny += ay / 128;

    // D-pad: 0x01 sus, 0x02 jos, 0x04 dreapta, 0x08 stanga
    uint8_t d = pad->dpad();
    if (d & 0x01) ny -= 2;
    if (d & 0x02) ny += 2;
    if (d & 0x04) nx += 2;
    if (d & 0x08) nx -= 2;

    nx = constrain(nx, 0, 160 - S);
    ny = constrain(ny, 0, 128 - S);

    // X (Cross) = culoare noua, la apasare
    bool cross = pad->a();
    bool schimbatCuloare = cross && !prevCross;
    if (schimbatCuloare) {
      culoare = tft.color565(random(80, 256), random(80, 256), random(80, 256));
      Serial.println("Cross apasat");
    }
    prevCross = cross;

    // O (Circle) = vibratie scurta
    bool circle = pad->b();
    if (circle && !prevCircle) {
      pad->playDualRumble(0, 150, 0x80, 0x40);
      Serial.println("Circle apasat");
    }
    prevCircle = circle;

    if (nx != px || ny != py || schimbatCuloare) {
      tft.fillRect(px, py, S, S, ST77XX_BLACK);   // sterge vechiul patrat
      px = nx; py = ny;
      tft.fillRect(px, py, S, S, culoare);        // deseneaza noul patrat
    }
  }

  delay(16);   // ~60 actualizari pe secunda
}