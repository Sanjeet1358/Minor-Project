# IoT-Based Health Monitoring System (Simulation)

## Project Overview
This project is an IoT-based health monitoring system developed using an ESP32 microcontroller and simulated on the Wokwi platform. The system is designed to continuously monitor basic health parameters and provide real-time visualization and alerts. The primary objective of this project is to demonstrate the feasibility of a low-cost, sensor-based patient observation system suitable for hospital observation wards or remote monitoring scenarios.

The current implementation is a simulation-only model intended for academic evaluation and system validation. Hardware prototyping will be performed in later stages.

---

## Key Features
- Real-time monitoring of body temperature
- Simulation of heart rate (BPM) and SpO₂ levels
- Motion and fall detection using an accelerometer
- Visual display of health parameters on an LCD
- Alert mechanism using buzzer and LED for abnormal readings
- Local web server dashboard hosted on ESP32
- Cloud-ready architecture for future IoT integration

---

## Components Used (Simulated)
- ESP32 Microcontroller  
- LCD Display (I2C)  
- Temperature Sensor (DS18B20 or equivalent)  
- MPU6050 Accelerometer  
- Potentiometers (used to simulate BPM and SpO₂ values)  
- Buzzer  
- LEDs  

---

## Simulation Platform
This project is simulated using **Wokwi**, an online electronics simulator that allows embedded system development without physical hardware.

🔗 **Wokwi Simulation Link:**  
https://wokwi.com/projects/449111686354403329

---

## File Description
- `sketch.ino`  
  Contains the complete firmware code written using the Arduino framework for ESP32.

- `diagram.json`  
  Defines the circuit connections and hardware configuration used in the Wokwi simulation.

---

## Use Case
The system is intended for:
- Patient observation wards
- Elderly care monitoring
- Academic demonstrations of IoT-based healthcare systems
- Embedded systems and sensor integration learning

---

## Project Status
- ✔ Simulation completed  
- ✔ Core functionality implemented  
- ⏳ Hardware implementation planned for final phase  

---

## Future Enhancements
- Integration with real biomedical sensors
- Cloud data logging using IoT platforms (e.g., ThingSpeak, Firebase)
- Mobile application for remote monitoring
- Emergency notification system
- Data analytics and trend visualization

---

## Disclaimer
This project is developed for educational purposes only. The simulated health parameters do not represent real medical data and should not be used for clinical decision-making.
