# Development log

## 2026-09-25 — Initial breadboard assembly

**Goal:** begin assembling the relay and pump setup for an ESP32 watering system.

**Work recorded:** relay and pump connected on the breadboard.

**Verification:** functional and electrical checks have not yet been documented. The ESP32 model, wiring, GPIO and relay active level still need to be recorded.

**Next step:** configure the software environment and verify ESP32 communication before testing relay control.

## Format for future entries

Copy the following structure for each small stage and fill it in after doing the work:

- Date and phase: 2026/09/25 (Phase1)
- Goal: Check if the ESP32 is working and checking if relay system is working
- Hardware or wiring changes: No changes made 
- Software changes: Initialised the platform.io ide 
- Test method: Run pump for 20 seconds through the relay
- Expected result: Pump ran successfully through the relay
- Observed result: Pump ran for 20 seconds but is way to strong so would try with a smaller battery next time
- Measurements, photographs or screenshots:
- Problems and decisions: To much voltage causing pump to pump to much water at a time
- GitHub commit or release link: 
- Next step: Soil controll
