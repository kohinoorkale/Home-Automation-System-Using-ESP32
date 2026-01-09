# Home Automation Using ESP-32

A smart IoT-based system that allows users to control home appliances remotely and secure their home using a mobile application and voice commands.

This project integrates the powerful ESP32 microcontroller with cloud platforms to create a seamless home automation experience, offering convenience, energy efficiency, and real-time security alerts.

## 🚀 Features
* 📱 **Remote Control:** Turn appliances (Lights, Fans) ON/OFF from anywhere in the world.
* 🚨 **Security System:** Detects motion/intruders and sends instant alerts to the user.
* 🗣️ **Voice Integration:** Compatible with Google Assistant and Amazon Alexa.
* 🔄 **Real-time Feedback:** View the live status of switches in the mobile app.
* 🌡️ **Sensor Monitoring:** Real-time temperature and humidity monitoring.
* 📅 **Scheduling:** Set timers to automatically turn devices on or off.

## 🛠️ Hardware Components
* **Microcontroller:** ESP32 Development Board (DOIT DEVKIT V1)
* **Switching:** 4-Channel Relay Module (5V)
* **Security Sensors:** PIR Motion Sensor (HC-SR501) or Magnetic Door Switch (Reed Switch)
* **Alarm:** 5V Piezo Buzzer (for security alerts)
* **Environment Sensors:** DHT11 (Temperature/Humidity)
* **Power:** 5V DC Power Supply / USB Adapter
* **Connection:** Jumper Wires & Breadboard/PCB

## 💻 Software & Tools
* **IDE:** Arduino IDE
* **Programming Language:** Embedded C++
* **IoT Platform:** Blynk IoT / Sinric Pro / ESP RainMaker
* **Voice Assistant:** Google Home / Alexa App

## ⚙️ Working Principle
1.  **Input:** The user sends a command via the App, or the **PIR sensor detects motion**.
2.  **Cloud Communication:** The command or sensor data is sent to the IoT Cloud server via Wi-Fi.
3.  **Processing:** The ESP32 processes the instruction.
4.  **Actuation:**
    * **Automation:** Triggers the Relay to turn appliances ON/OFF.
    * **Security:** Triggers the **Buzzer** if motion is detected while "Security Mode" is armed.
5.  **Feedback/Alerts:** The ESP32 sends the new status or an **intruder notification** back to the mobile app.
