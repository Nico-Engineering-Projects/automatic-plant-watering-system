# Automatic Plant Watering System

An engineering project by Nico Gerber to develop an ESP32-based system that measures soil moisture and controls a water pump.

**Status:** In progress — breadboard assembly and development planning.

## Goal

Develop the hardware and firmware step by step, verify each stage, and document the design decisions, measurements and lessons learned.

## Current progress

- Relay and pump connected on the breadboard.
- ESP32 board selection, GPIO assignment, relay behaviour and electrical connections still need to be recorded and verified.
- Firmware development and functional testing are next.
- No watering performance or reliability results have been established yet.

## Planned development phases

- [ ] Verify ESP32 upload and Serial Monitor communication.
- [ ] Verify relay control and startup state with the pump disconnected.
- [ ] Verify a deliberately triggered short pump run with automatic shutoff.
- [ ] Measure and calibrate the soil-moisture sensor.
- [ ] Implement and test automatic watering with a soak period.
- [ ] Evaluate the results and document improvements.

## Planned software tools

- Visual Studio Code
- PlatformIO IDE
- Arduino framework for ESP32
- Git and GitHub for version history

The exact ESP32 board and GPIO configuration will be documented before the first firmware is added.

## Documentation

- [Development log](docs/development-log.md)
- Circuit diagrams, component list and test results will be added as the project progresses.
- The Word project report will be kept in `docs/`; a readable PDF can be included with major milestones.

## Project structure as the work grows

| Location | Contents |
| --- | --- |
| `README.md` | Current overview, status and setup instructions |
| `src/` | Firmware source, added when the PlatformIO project is created |
| `platformio.ini` | Verified board, framework and build configuration |
| `hardware/` | Wiring diagrams, schematics and parts list |
| `docs/` | Development log and project report |
| `results/` | Measurements and test observations |
| `images/` | Photographs of the actual project |

## Development history

Small changes are recorded as commits. Tags and releases will mark verified milestones. Each log entry should record what was actually tested and link to the relevant commit.
