# ESP8266 Low-Power Sensor Data Logger  
(Pressure & Temperature)

This project is a low-power IoT data logger based on the ESP8266.  
The device periodically measures pressure and temperature sensor data and sends the values to a central server for storage and analysis.  
Between measurements, the ESP8266 enters Deep Sleep mode to minimize power consumption, allowing the device to operate on a battery for approximately six months.

---

## Project Goals

- Measure pressure and temperature using external sensors
- Periodically transmit sensor data to a single server
- Minimize power consumption using ESP8266 Deep Sleep mode
- Achieve long-term battery operation (target: ~6 months)
- Provide a simple and scalable architecture for sensor data logging

---

## System Overview

The system operates in a cyclic manner to maximize energy efficiency:

1. ESP8266 wakes up from Deep Sleep
2. Pressure and temperature sensors are initialized
3. Sensor values are measured
4. Wi-Fi connection is established
5. Sensor data is sent to the server
6. The device enters Deep Sleep mode until the next measurement cycle

Most of the power consumption occurs during the boot, sensor reading, and Wi-Fi transmission phases.  
Deep Sleep drastically reduces power usage during idle periods.

---

## Hardware

### Microcontroller
- ESP8266 (e.g., Wemos D1 mini or ESP8266-based module)

### Sensors
- Pressure sensor (model to be defined)
- Temperature sensor (model to be defined)

Depending on the selected sensors, communication may use I2C, SPI, or analog interfaces.  
Pressure sensors may require signal conditioning or external ADCs.

### Power Supply
- Battery powered (AA/AAA batteries or Li-ion battery)
- Low-dropout or ultra-low-power voltage regulator recommended
- Target operational lifetime: approximately 6 months

---

## Firmware Design

### Sampling Strategy
- The ESP8266 wakes up at a fixed interval defined by `SAMPLE_INTERVAL`
- Sensor data is collected and transmitted immediately
- After transmission, the device enters Deep Sleep

### Deep Sleep Mode
- `ESP.deepSleep()` is used to minimize power consumption
- For timed wake-up, `GPIO16 (D0)` must be connected to the `RST` pin

### Error Handling
- If Wi-Fi connection or data transmission fails, the device may:
  - Retry a limited number of times
  - Enter Deep Sleep to conserve power

---

## Communication

Sensor data is transmitted to a central server using one of the following methods:
- HTTP (REST API)
- MQTT (message-based communication)

The choice of protocol depends on server architecture and scalability requirements.

Example data payload:
```json
{
  "temperature": 23.4,
  "pressure": 101.2,
  "timestamp": 1700000000
}

