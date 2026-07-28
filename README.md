# Automotive HUD — Speed & Tach

A second-gen windshield heads-up display for a vehicle. Reads wheel-speed and tachometer
pulses off the vehicle harness and drives five 7-segment LED displays bright
enough to reflect off the windshield in daylight.

Custom 4-layer PCB (95 × 35 mm) + STM32 firmware.

## Features

- 3-digit speed (km/h), 2-digit RPM
- Optoisolated inputs — no galvanic path to the vehicle harness
- Hardware input capture for period measurement; adaptive sampling + exponential smoothing of data
- Automatic day/night dimming driven by the vehicle's illumination signal
- 12 V automotive input with load-dump protection

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
  and the TLC5947 VCC.

### Current & thermal

- IREF = 1.96 kΩ → 25 mA/segment.
- TLC5947 dissipates ~1 W with all 24 channels on; PowerPAD must be soldered to
  a ground pour with a stitching via array (RθJA 32.8 °C/W → ~35 °C rise).
- DSM7UA70105 derates 0.30 mA/°C above 25 °C — about 18 mA allowable at 65 °C
  ambient. Hopefully doesn't melt in the summer heat of a car.

### Board

- 4 layers: signal / GND plane / split power plane (5 V + 3.3 V) / signal
- Displays and driver ICs on one side, everything else on the other
- Tented (JLCPCB ink-plugged) vias board-wide
- Status LEDs: 5 V rail (blue), 3.3 V rail (red), 2× GPIO (yellow, green)
- RST button for MCU

## Firmware

STM32CubeIDE / HAL, FreeRTOS (CMSIS_V2), C.

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
### Physical Assembly

- 3D-printed case in black PETG, two halves that clamp shut on the PCB, which snaps into the bottom case using snap-hooks; designed in SolidWorks
- Both case halves screw in together using SolidWorks mounting boss features
- Teleprompter reflective film on windshield to remove double image from second reflection on outer side of windshield.


### Anticipated issues / future revisions

- Polarized sunglasses block the polarized windshield reflection.
- Add proper collimation in the optical path using plano-convex lenses and fold mirrors
