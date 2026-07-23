# Automotive HUD — Speed & Tach

A windshield heads-up display for a vehicle. Reads wheel-speed and tachometer
pulses off the vehicle harness and drives five 7-segment LED displays bright
enough to reflect off the windshield in daylight.

Custom 4-layer PCB (90 × 30 mm) + STM32 firmware. Prototype / proof-of-concept.

---

## Features

- 3-digit speed (km/h), 2-digit RPM (displayed in hundreds — `3500 RPM → 35`)
- Optoisolated inputs — no galvanic path to the vehicle harness
- Hardware input capture for period measurement; adaptive sampling + exponential smoothing
- Automatic day/night dimming driven by the headlight signal
- 12 V automotive input with load-dump protection

## Why LED segments

An earlier SSD1306 OLED prototype failed in direct sun against a light-colored
car. The 7-segment LEDs run at roughly 100,000 cd/m² on-axis versus ~150 cd/m²
for the OLED, which clears that failure case with margin. Teleprompter film on
the windshield suppresses the double image.

---

## Hardware

### Core

| Block | Part |
|---|---|
| MCU | STM32F411RET6 (LQFP64) |
| LED drivers | 2× TLC5947, daisy-chained (48 ch, 40 used) |
| Displays | 5× DSM7UA70105 7-segment, common anode |
| Buck (12 V → 5 V) | LMR36015AQRNXRQ1, 400 kHz |
| LDO (5 V → 3.3 V) | AP2112K-class series LDO |
| Optoisolators | TLP291 (headlight), LTV-827S (speed / tach) |

### Power

- **12 V in:** SMCJ36A TVS, EN tied to VIN. Input caps rated 50 V for load dump.
- **Buck passives:** L = Bourns SRR1260-150M (15 µH, 27 mΩ, −40…+125 °C),
  R_FBT = 100 kΩ, R_FBB = 24.9 kΩ, C_VCC = 1 µF, C_BOOT = 0.1 µF.
- **Split rails:** 5 V feeds the display common anodes; 3.3 V feeds the STM32
  *and* the TLC5947 VCC. The TLC5947 must run at 3.3 V — its logic thresholds
  scale with VCC, and at 5 V the STM32's 3.3 V SPI would fall below V_IH.

### Current & thermal

- IREF = 1.96 kΩ → 25 mA/segment. 2.46 kΩ drops it to a more conservative 20 mA.
- TLC5947 dissipates ~1 W with all 24 channels on; PowerPAD must be soldered to
  a ground pour with a stitching via array (RθJA 32.8 °C/W → ~35 °C rise).
- DSM7UA70105 derates 0.30 mA/°C above 25 °C — about 18 mA allowable at 65 °C
  ambient. Grayscale PWM keeps average current under that in a hot car.

### BOM consolidation

Deliberately kept to three capacitor values (**0.1 µF, 2.2 µF, 22 µF**), one
LED resistor value (**270 Ω**, blue on 5 V is the binding constraint at ~48 mcd),
and reuse of 24.9 kΩ for the BOOT0 pulldown.

### Board

- 4 layers: signal / GND plane / split power plane (5 V + 3.3 V) / signal
- Displays and driver ICs on one side, everything else on the other
- Tented (JLCPCB ink-plugged) vias board-wide; 0.3 mm thermal vias under the
  PowerPAD, dedicated exposed test pads for bring-up
- Status LEDs: 5 V rail (blue), 3.3 V rail (red), 2× GPIO (yellow, green)
- BOOT0 button for UART reflash

---

## Firmware

STM32CubeMX / HAL, FreeRTOS (CMSIS_V2), C.

### Clock & peripherals

| | |
|---|---|
| Core | 100 MHz via HSI PLL, APB1 at 50 MHz |
| TIM2 | Input capture, PSC = 99 → 1 µs tick, 32-bit (rollover-safe) |
| SPI2 | 12.5 MHz, TX-only master, PB13 / PB15 |
| TIM3_CH4 | BLANK PWM at 25 kHz (flicker-free dimming) |
| TIM11 | HAL timebase |
| SWO | PB3 |

### Pin map

| Signal | Pin |
|---|---|
| Tachometer in | PA0 (TIM2_CH1) |
| Speed in | PA1 (TIM2_CH2) |
| Headlight in | PC0 (polled) |
| SCLK | PB13 |
| SIN | PB15 |
| XLAT | PB14 |
| BLANK | PB1 (TIM3_CH4) |

### Tasks

Two FreeRTOS tasks communicating over a queue of packed `uint32` frames:

- `inputTask` — period capture, smoothing, unit conversion
- `displayTask` — segment packing, 72-byte SPI frame, XLAT latch

### Display mapping

```
U7  ch 0–23   → LED1–LED3   speed, three digits
U8  ch 24–47  → LED4–LED5   RPM in hundreds
DIGIT_BASE = { 0, 8, 16, 24, 32 }
```

## Status

Prototype. Schematic complete, board laid out, firmware ported from an earlier
Artemis Nano + OLED revision.

### Known issues / future revisions

- The 1 kΩ opto input resistor (RC1206FR-071KL, 0.25 W) sits at ~62 % of rating.
  Split into two 499 Ω parts or move to an ERJ-P08.
- Thermal vias fall outside JLCPCB's free plug process. Accept minor solder
  loss on v1; use paid via-in-pad fill or dog-bone routing later.
- Polarized sunglasses block the s-polarized windshield reflection. A
  quarter-wave retarder film is the fix OEMs use.
