# Rocketry Project

**Competition:** World Space Olympiad (WSO) - Rocket Competition  
**Team Phase 1:** Galacticos | **Team Phase 2:** NIS Copernicus  
**Role:** Team Captain (both phases)  
**Year:** 2024

---

## Overview

This project was developed across two phases of the World Space Olympiad rocket competition. The objective was to design, simulate, and build a model rocket with an integrated onboard flight computer capable of real-time data acquisition during flight.

Between phases, the hardware stack was significantly upgraded based on lessons learned from Phase 1 - replacing the Arduino Nano with an Arduino Uno, adding a barometric pressure sensor (BMP280), integrating wireless telemetry (NRF24L01), and redesigning the 3D model in Fusion 360 for improved structural accuracy. Due to time constraints, the Phase 2 hardware was launched with Phase 1 firmware; full sensor integration was a planned next iteration.

---

## Phase Comparison

| Parameter | Phase 1 - Galacticos | Phase 2 - NIS Copernicus |
|---|---|---|
| Microcontroller | Arduino Nano | Arduino Uno |
| Storage | SD card module | Data Logging Shield (with RTC) |
| IMU | MPU-6050 | MPU-6050 |
| Barometric sensor | - | BMP280 |
| Wireless telemetry | - | NRF24L01 |
| Ground station | - | Arduino Uno + NRF24L01 |
| Camera | Raspberry Pi Camera V2 | Action camera (on parachute) |
| 3D modeling tool | Tinkercad | Fusion 360 |
| Simulated apogee | 312 m | 415 m |
| Stability margin | 1.3 cal | 2.24 cal |
| Landing speed | - | 3.74 m/s |

---

## Rocket Design

The rocket consists of three primary sections:

**Nosecone** - houses the onboard flight computer payload. At apogee the nosecone is ejected to deploy the parachute recovery system.

**Body tube** - contains the parachute (85 cm diameter, nylon ripstop), wadding, and avionics bay. Centering rings align the internal structure and maintain airframe rigidity.

**Motor section** - contains the propellant motor (RD1-100-7M-7), ejection charge, and trapezoidal fins for passive aerodynamic stabilization.

In Phase 2 the 3D model was redesigned in Fusion 360 with improved internal component layout and material specifications: PLA nosecone (50% infill), Watman paper body tube, plywood motor mount rings (laser cut), and bookbinding board fins.

---

## Avionics

### Phase 1 - Galacticos
- **Arduino Nano** - central flight computer
- **MPU-6050** - 6-axis IMU via I2C for acceleration and attitude data
- **Micro SD card module** - SPI-based data logging
- **Raspberry Pi Camera V2** - aerial imaging

### Phase 2 - NIS Copernicus
- **Arduino Uno** - central flight computer (upgraded for additional I/O)
- **MPU-6050** - 6-axis IMU via I2C
- **Data Logging Shield** - SD storage with onboard RTC for timestamped data
- **BMP280** - barometric pressure and altitude sensor
- **NRF24L01** - 2.4 GHz wireless telemetry to ground station
- **Buzzer** - post-landing acoustic locator
- **Action camera** - mounted on parachute for aerial footage

### Ground Station (Phase 2)
- **Arduino Uno** — receives and displays telemetry
- **NRF24L01** — matched receiver module for rocket-to-ground link

---

## Recovery System

Both phases used an ejection-charge parachute recovery system. At apogee the ejection charge fires, the nosecone separates, and the parachute deploys to bring the rocket to a safe landing speed. In Phase 2 the action camera was suspended below the parachute for aerial footage during descent.

Parachute specs: 85 cm diameter, nylon ripstop material.

---

## Firmware

Two firmware modules were written in C++ for the Arduino Nano (Phase 1). The same firmware was used in Phase 2 due to time constraints - the rocket launched with Phase 1 code running on Phase 2 hardware, logging MPU-6050 data only. BMP280 and NRF24L01 integration was not completed within the competition timeline.

### `sd_logger.ino`
Reads X/Y/Z acceleration from the MPU-6050 and writes raw values to `data.txt` on the SD card at 1 Hz. Data is mirrored to Serial at 9600 baud for ground debugging.

**Known limitation:** File opens and closes on every loop iteration to prevent data loss on power-off, at the cost of write throughput. A production version would batch writes and use interrupt-driven closure at apogee. No timestamps in Phase 1 - this was addressed in Phase 2 hardware via the RTC on the Data Logging Shield, pending firmware update.

### `speed_calculator.ino`
Estimates vertical velocity by numerically integrating calibrated Z-axis acceleration from the MPU-6050. A 100-sample calibration routine runs at startup to establish the static bias offset.

**Known limitation:** Euler forward integration accumulates drift over time. Without BMP280 fusion or a complementary filter, velocity estimates degrade past approximately 5–8 seconds. Suitable for the competition flight duration; sensor fusion was planned for the Phase 2 firmware update.

Both modules were written and tested during Phase 1 and reflect the constraints of a competition timeline. Known limitations were identified in post-competition review and represent the next engineering iteration.

---

## Simulation

Flight trajectory and stability were simulated in OpenRocket across both phases. Key outputs used to finalize each design: CP/CG separation for stability margin verification, altitude and velocity profiles, and motor selection based on apogee target.

Phase 2 achieved a significantly higher simulated apogee (415 m vs 312 m) and better stability margin (2.24 cal vs 1.3 cal) through structural redesign and component repositioning.

---

## Repository Structure

```
rocketry/
├── README.md
├── firmware/
│   ├── sd_logger/
│   │   └── sd_logger.ino
│   └── speed_calculator/
│       └── speed_calculator.ino
├── simulation/
│   ├── phase1-openrocket.png
│   ├── phase1-flight-simulation.png
│   └── phase2-openrocket.png
├── hardware/
│   ├── rocket-construction.png
│   ├── avionics-phase1.png
│   ├── avionics-phase2.png
│   ├── ground-station.png
│   └── sensors.png
├── cad/
│   ├── phase1-tinkercad.png
│   └── phase2-fusion360.png
└── recovery/
    └── parachute-system.png
```

---

## Tools Used

- **OpenRocket** - flight simulation and stability analysis
- **Tinkercad** - Phase 1 3D modeling
- **Fusion 360** - Phase 2 3D modeling
- **Arduino IDE** - firmware development (C++)
- **MPU6050 library** - I2C IMU communication
- **SD library** - SPI file system operations

---

## References

1. Sutton, G. P., & Biblarz, O. — *Rocket Propulsion Elements*
2. [Apogee Rockets](https://www.apogeerockets.com) — design guides and motor specifications
3. [OpenRocket](https://openrocket.info) — simulation software
4. [Instructables](https://www.instructables.com) — fabrication reference


---

## Gallery

### Rocket Construction

![Phase 1 rocket construction](hardware/rocket-construction-phase1.png)
*Phase 1 structural layout: nosecone (payload ejection at apogee), body tube (parachute + avionics), motor section (propellant + fins)*

![Phase 2 rocket construction](hardware/rocket-construction-phase2.png)
*Phase 2 detailed material breakdown: PLA nosecone (50% infill), Watman paper body tube, plywood motor rings (laser cut), bookbinding board fins, epoxy adhesive*

### Avionics

![Phase 1 avionics](hardware/avionics-phase1.png)
*Phase 1 flight computer: Arduino Nano, MPU-6050 IMU, micro SD card module, Raspberry Pi Camera V2*

![Phase 2 avionics](hardware/avionics-phase2.png)
*Phase 2 flight computer assembly: Arduino Uno, Data Logging Shield with RTC, NRF24L01 telemetry, BMP280 barometer, MPU-6050, buzzer*

![Phase 2 wiring diagram](hardware/wiring-diagram-phase2.png)
*Phase 2 wiring diagram: full connection layout between Arduino Uno, Data Logging Shield, NRF24L01, MPU-6050, BMP280, and buzzer*

![Ground station](hardware/ground-station.png)
*Ground station: Arduino Uno + NRF24L01 receiver for real-time telemetry during flight*

![Sensors](hardware/sensors.png)
*Onboard sensors: MPU-6050 IMU, altimeter, RF receiver*

### Simulation

![Phase 1 OpenRocket design](simulation/phase1-openrocket.png)
*Phase 1 OpenRocket design: stability margin 1.3 cal, simulated apogee 312 m*

![Phase 1 flight simulation](simulation/phase1-flight-simulation.png)
*Phase 1 flight simulation: altitude, vertical acceleration, and vertical velocity over time*

![Phase 2 OpenRocket design](simulation/phase2-openrocket.png)
*Phase 2 OpenRocket design: stability margin 2.24 cal, simulated apogee 415 m*

![Phase 2 flight simulation](simulation/phase2-flight-simulation.png)
*Phase 2 flight simulation results: apogee 415 m, max speed 110 m/s, landing speed 3.74 m/s*

### 3D Models

![Phase 1 Tinkercad model](cad/phase1-tinkercad.png)
*Phase 1 3D model designed in Tinkercad: height 93 cm, diameter 8 cm*

![Phase 2 Fusion 360 model](cad/phase2-fusion360.png)
*Phase 2 3D model designed in Fusion 360: full cross-section view with internal component layout*

### Recovery System

![Parachute recovery system](recovery/parachute-system.png)
*Parachute recovery system: 85 cm diameter nylon ripstop parachute, ejected at apogee via ejection charge*
