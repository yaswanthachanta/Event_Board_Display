# EventBoard - RTC-Driven Message Display System  

## 📌 Overview  
The **EventBoard** is a real-time automated message display system built using the **LPC2148 ARM7 microcontroller**. It displays **predefined scrolling messages** on a 16x2 LCD at specific times using the **on-chip RTC (Real-Time Clock)**.  

The system includes:  
- **Admin mode** with password-protected keypad access  
- **Message scheduling** based on RTC  
- **Room temperature monitoring** via LM35 sensor  
- **LED indicators** for active/idle status  

This project demonstrates a combination of **secure access, RTC-based automation, and environmental monitoring** in an embedded system.  

---

## 🎯 Features  
- ⏰ **RTC-based Scheduling** – Messages appear automatically at set times.  
- 🔑 **Admin Mode** – Secure access using external switch + password.  
- 📋 **Message Management** – Enable/disable predefined messages.  
- 🌡️ **Temperature Display** – Shows real-time room temperature when idle.  
- 💡 **LED Indicators** –  
  - Green: Active scheduled message  
  - Red: Idle mode  
- 🔊 **Buzzer Alerts** for notifications.   

---

## 🖥️ Hardware Requirements  
- **LPC2148 Microcontroller**  
- **16x2 LCD Display**  
- **Keypad**  
- **LEDs (Red/Green)**  
- **LM35 Temperature Sensor**  
- **Buzzer**  

---

## 💾 Software Requirements  
- Embedded C  
- **Keil µVision (C Compiler)**  
- **Flash Magic** (for programming LPC2148)  

---

## 🔄 System Workflow  
1. At startup, all messages are enabled by default.  
2. The RTC checks system time.  
3. If a scheduled message matches current time → display message (scrolling) + Green LED ON.  
4. If no active message → show **RTC time + Room Temperature** + Red LED ON.  
5. Admins can press a switch → enter **password-protected mode** → update RTC time or enable/disable specific messages.  

---
