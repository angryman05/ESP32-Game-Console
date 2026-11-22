# SYNC – Mini consolă 3-in-1 cu ESP32

Proiect PMP (Proiectare cu Microprocesoare), UTCN – CTI, anul 3.

Consolă portabilă inspirată de Nintendo Switch. Rulează 3 jocuri retro pe un ecran TFT SPI de 1.8" și se controlează cu un controller **PS5 DualSense** prin **Bluetooth**. Ca variantă de rezervă are joystick analogic și butoane fizice.

Jocuri propuse (se aleg 3): Snake, Tetris, Pac-Man, Pong, Breakout.

---

## Componente

| Componentă | Rol |
|---|---|
| NodeMCU-32S (ESP32-WROOM-32, 38 pini) | microcontroller, Bluetooth Classic pentru DualSense |
| Modul LCD SPI 128×160 (ST7735) cu slot SD | afișaj |
| Controller PS5 DualSense | control principal (Bluetooth) |
| Modul joystick analogic XY cu buton | control de rezervă |
| Butoane push (6 mm) | A / B / X / Y / Pause-Home |
| Modul buzzer pasiv 3.3–5 V | sunete |
| LED RGB catod comun 5 mm | indicator „SYNC" / stare |
| Rezistori 1/4 W | LED RGB |
| Breadboard MB-102 (830 puncte) | prototip |
| Fire Dupont (M-M, M-F, F-F) | conexiuni |
| Powerbank + cablu micro-USB | alimentare la demonstrație |

---

## Pinout NodeMCU-32S

![Pinout NodeMCU-32S](images/nodemcu32s-pinout.png)

> Sursa: pagina produsului de pe ardushop.ro. Pe placă, pinii sunt marcați `Pxx` (= GPIOxx), `SVP` = GPIO36, `SVN` = GPIO39.

---

## Progres

- [x] **Etapa 1 – Setup ESP32**
  - driver CP2102 (Silicon Labs CP210x Universal Windows Driver), placa apare pe COM7
  - Arduino IDE 2.x + board packages `esp32` (Espressif) și `esp32_bluepad32` (Ricardo Quesada)
  - board selectat: **NodeMCU-32S (esp32_bluepad32)**
  - test blink pe GPIO2 + Serial Monitor (115200) ✔
- [x] **Etapa 1b – Display pe breadboard**
  - biblioteca **Adafruit ST7735 and ST7789** (+ Adafruit GFX, BusIO)
  - test culori, text și minge pong ✔
- [x] **Etapa 2 – Controller PS5**
  - Bluepad32: pairing cu Create + PS, lightbar setat din cod
  - pătrat mișcat pe ecran cu stick-ul stâng / D-pad, Cross schimbă culoarea, Circle face controllerul să vibreze ✔
- [ ] **Etapa 3** – Framebuffer (fără pâlpâire) + primul joc complet: Pong
- [ ] **Etapa 4** – Butoane fizice + joystick ca variantă de rezervă
- [ ] **Etapa 5** – Meniu de selecție + încă 2 jocuri
- [ ] **Etapa 6** – Buzzer (sunete), LED RGB (indicator SYNC), highscore-uri salvate în flash (`Preferences`)
- [ ] **Etapa 7** – Carcasă stil Switch; alimentare din powerbank prin micro-USB (codul e deja încărcat pe placă)

> Cardul SD **nu este folosit**: pinii modulului SD nu sunt lipiți, iar scorurile se salvează în memoria flash internă (NVS, biblioteca `Preferences`).

---

## Montaj pe breadboard

ESP32 e așezat **cu mufa USB în sus (spre rândul 1)**, pe **rândurile 5–23**, cu pinii în **coloanele b și i**. Rămân libere coloanele **a** (stânga) și **j** (dreapta), câte o gaură pentru fiecare pin.

Verificare: **3V3 = i23**, **GND = b23**.

### Harta pinilor pe breadboard

| Rând | Stânga (gaura liberă: **a**) | Dreapta (gaura liberă: **j**) |
|---|---|---|
| 5 | GPIO6 ⛔ | **Vin (5V)** |
| 6 | GPIO7 ⛔ | GPIO11 ⛔ |
| 7 | GPIO8 ⛔ | GPIO10 ⛔ |
| 8 | GPIO15 | GPIO9 ⛔ |
| 9 | GPIO2 (LED albastru onboard) | GPIO13 |
| 10 | GPIO0 (BOOT) | **GND** |
| 11 | GPIO4 | GPIO12 |
| 12 | GPIO16 | GPIO14 |
| 13 | GPIO17 | GPIO27 |
| 14 | GPIO5 | GPIO26 |
| 15 | GPIO18 | GPIO25 |
| 16 | GPIO19 | GPIO33 |
| 17 | **GND** | GPIO32 |
| 18 | GPIO21 | GPIO35 |
| 19 | RX0 (GPIO3) ⛔ | GPIO34 |
| 20 | TX0 (GPIO1) ⛔ | GPIO39 |
| 21 | GPIO22 | GPIO36 |
| 22 | GPIO23 | EN |
| 23 | **GND** | **3.3V** |

⛔ = nu se folosește (GPIO6–11 sunt legați la flash, GPIO1/3 la serialul USB).

### Alimentare (șina din dreapta)

Pe partea dreaptă a breadboardului, **+ (roșu) e coloana exterioară**, iar **− (albastru) e coloana de lângă j**.

| De la | La |
|---|---|
| j23 (3.3V) | șina **+** dreapta |
| j10 (GND) | șina **−** dreapta |

> Pe șine se pune **doar 3.3V**. Vin (5V) nu se leagă la șina +.
> Componentele se alimentează din zona rândurilor 10–25 (șinele pot fi întrerupte la mijlocul breadboardului).

---

## Conexiuni realizate

### Display ST7735 (128×160, SPI)

| Pin display | Conectat la | GPIO |
|---|---|---|
| VCC | șina + dreapta | 3.3V |
| GND | șina − dreapta | GND |
| CS | a14 | GPIO5 |
| RESET | a13 | GPIO17 |
| A0 (DC) | a12 | GPIO16 |
| SDA (MOSI) | a22 | GPIO23 |
| SCK | a15 | GPIO18 |
| LED (backlight) | șina + dreapta | 3.3V |

Inițializare în cod: `tft.initR(INITR_BLACKTAB); tft.setRotation(1);` → 160×128, landscape.

### Controller PS5

Fără fire. Conectare prin Bluetooth Classic (Bluepad32): **Create + PS** până clipește rapid.

Maparea butoanelor în Bluepad32 (după poziție, ca la Xbox):

| Funcție Bluepad32 | Buton DualSense |
|---|---|
| `a()` | Cross (X) |
| `b()` | Circle (O) |
| `x()` | Square (□) |
| `y()` | Triangle (△) |
| `axisX()`, `axisY()` | stick stâng (~ −512…512) |
| `dpad()` | 0x01 sus, 0x02 jos, 0x04 dreapta, 0x08 stânga |

---

## Conexiuni planificate

| Componentă | GPIO | Gaura | Observații |
|---|---|---|---|
| Joystick VRx | GPIO34 | j19 | ADC1 (ADC2 nu merge cu Bluetooth activ); alimentat la **3.3V** |
| Joystick VRy | GPIO35 | j18 | ADC1 |
| Joystick SW | GPIO32 | j17 | `INPUT_PULLUP` |
| Buton A | GPIO13 | j9 | `INPUT_PULLUP`, buton la GND |
| Buton B | GPIO14 | j12 | `INPUT_PULLUP`, buton la GND |
| Buton X | GPIO27 | j13 | `INPUT_PULLUP`, buton la GND |
| Buton Y | GPIO26 | j14 | `INPUT_PULLUP`, buton la GND |
| Pause / Home | GPIO33 | j16 | `INPUT_PULLUP`, buton la GND |
| Buzzer pasiv | GPIO25 | j15 | PWM (LEDC) |
| LED RGB – R | GPIO21 | a18 | rezistor 220–330 Ω |
| LED RGB – G | GPIO22 | a21 | rezistor 220–330 Ω |
| LED RGB – B | GPIO2 | a9 | rezistor 220–330 Ω (comun cu LED-ul onboard) |
| LED RGB – catod | GND | șina − | |

Reguli de reținut:
- GPIO34–39 sunt **doar intrări** și nu au pull-up intern, deci nu se folosesc pentru butoane.
- Pentru semnale analogice se folosește **doar ADC1** (GPIO32–39) cât timp Bluetooth-ul e activ.
- GPIO0, 2, 12, 15 sunt pini de strapping, deci se evită pentru butoane.

---

## Software

| Ce | Versiune / sursă |
|---|---|
| Arduino IDE | 2.x |
| Board package | `esp32_bluepad32` 4.1.0 (include Bluepad32) |
| Board | NodeMCU-32S (esp32_bluepad32) |
| Bibliotecă display | Adafruit ST7735 and ST7789 + Adafruit GFX |
| Salvare scoruri | `Preferences` (inclusă în core) |

URL-uri pentru Boards Manager:
```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json
```

---

## Depanare (lecții învățate)

- **Upload blocat la `Connecting...`**: se ține apăsat butonul BOOT (IO0).
- **Caractere ciudate în Serial Monitor la reset**: e mesajul de boot la 74880 baud, normal.
- **Ecran negru (fără backlight)**: problema e la alimentare. Se verifică dacă 3.3V ajunge în șină și dacă componenta e pe aceeași jumătate de șină.
- **Ecran alb**: display-ul primește curent, dar nu e inițializat. Se verifică cele 5 fire de semnal (rânduri numărate greșit, fire slăbite).
- **Controllerul nu se conectează**: consola PS5 trebuie să fie oprită, altfel DualSense se reconectează la ea.
