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
  - Green: Active scheduled message.  
  - Red: Idle mode .   

---

## 🖥️ Hardware Requirements  
- **LPC2148 Microcontroller**  
- **16x2 LCD Display**  
- **Keypad**  
- **LEDs (Red/Green)**  
- **LM35 Temperature Sensor**   

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

# EventBoard - RTC Driven Message Display System

This project is based on the **LPC2148 ARM7 Microcontroller** and demonstrates the use of **RTC (Real Time Clock), ADC, Keypad, LCD, LEDs, and Switch inputs** for building a real-time event/message display system with admin control.

---

## 🖼️ Block Diagram
![Block Diagram](EventBoard%20-%20RTC-Driven%20Message%20Display%20System.jpg)

---

## 📌 Components Used
1. **LPC2148 Microcontroller**
   - ARM7TDMI-S based 32-bit MCU.
   - Handles real-time operations, ADC, external interrupts, and peripheral interfacing.

2. **RTC (Real Time Clock)**
   - Maintains the system time and date.
   - Provides time-based scheduling for event display.

3. **4x4 Matrix Keypad**
   - User input for setting events/messages.
   - Used for entering passwords or selecting menu options.

4. **Switch (Admin Mode)**
   - Dedicated hardware switch for entering **Admin Mode**.
   - Provides security and prevents unauthorized access.

5. **LM35 (Temperature Sensor)**
   - Provides analog temperature readings.
   - Connected to LPC2148’s **ADC channel**.

6. **LCD Display**
   - Shows event messages, temperature data, and system status.

7. **Green LED**
   - Indicates normal system operation.

8. **Red LED**
   - Indicates error, alert, or restricted access.

---

## ⚙️ Features
- Real-time clock based event display.
- Admin authentication using switch & keypad.
- Temperature monitoring using LM35.
- Visual indication of status using LEDs.
- LCD interface for real-time information display.

---

## 🚀 Applications
- Notice board systems in colleges/offices.
- Event scheduling and monitoring.
- Security-enabled information boards.
- Temperature-based alerts and monitoring.

---

## 🛠️ Tools Required
- **Keil µVision / MDK-ARM** (for coding & debugging)
- **Flash Magic / LPC2148 Programmer** (for flashing code)
- **Proteus / Multisim** (for simulation, optional)
- **ARM GCC Toolchain** (alternative development environment)

---
