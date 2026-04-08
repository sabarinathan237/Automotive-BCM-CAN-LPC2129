# 🚗 Intelligent Distributed Automotive Body Control System

## Using ARM7 LPC2129 and CAN Protocol

---

## 📌 Abstract

This project presents the design and implementation of an **Intelligent Distributed Automotive Body Control System (BCM)** using the **ARM7 LPC2129 microcontroller** and the **Controller Area Network (CAN) protocol**. The system follows a **multi-node distributed architecture**, where a central BCM communicates with dedicated control nodes to manage vehicle body functions efficiently and reliably.

Key functionalities include **left and right indicator control, hazard warning synchronization, and multi-mode wiper operation (OFF, LOW, HIGH)**. The use of CAN protocol ensures **high-speed, noise-immune communication with reduced wiring complexity**, making the system suitable for modern automotive applications. Additionally, a **basic diagnostic and fault monitoring mechanism** enhances system reliability and maintainability.

---

## 🏗️ System Architecture

The system consists of the following nodes:

* **BCM Master Node** → Handles user input, LCD display, and CAN communication
* **Left Indicator Node** → Controls left-side indicator LEDs
* **Right Indicator Node** → Controls right-side indicator LEDs
* **Wiper Control Node** → Controls wiper motor modes

---

## ⚙️ Key Features

* Distributed multi-node CAN architecture
* Real-time automotive signal control
* Indicator blinking with hazard synchronization
* Multi-mode wiper control (OFF / LOW / HIGH)
* Fault detection and diagnostic communication
* LCD-based system monitoring
* Reduced wiring using CAN bus
* Modular and scalable system design

---

## 🔌 Hardware Requirements

* ARM7 LPC2129 Microcontroller
* CAN Transceiver (MCP2551 / TJA1050)
* 16x2 LCD Display
* LEDs (Indicator Simulation)
* Push Buttons (Input Control)
* Wiper Motor (via relay/driver circuit)
* Regulated Power Supply

---

## 📡 CAN Message Mapping

| CAN ID | Function                |
| ------ | ----------------------- |
| 0x21   | Left Indicator Control  |
| 0x22   | Right Indicator Control |
| 0x23   | Wiper Control           |
| 0x24   | Hazard Mode             |
| 0x40   | Diagnostic Request      |
| 0x41   | Left Node Response      |
| 0x42   | Right Node Response     |
| 0x43   | Wiper Node Response     |
| 0x31   | Left Node Fault         |
| 0x32   | Right Node Fault        |
| 0x33   | Wiper Node Fault        |

---

## 🧠 Wiper Operation Modes

* **OFF** → Motor Disabled
* **LOW** → Low Speed Operation
* **HIGH** → High Speed Operation

---

## 📂 Project Structure

```
automotive-bcm-can-lpc2129/
│
├── BCM_Master/
│   ├── main.c
│   ├── lcd.c
│   ├── lcd.h
│
├── Left_Node/
│   ├── left_indicator.c
│
├── Right_Node/
│   ├── right_indicator.c
│
├── Wiper_Node/
│   ├── wiper_node.c
│
├── docs/
│   ├── block_diagram.png
│
├── images/
│   ├── demo.png
│
└── README.md
```

---

## 🚀 Execution Steps

1. Open each node project in **Keil uVision**
2. Compile and generate `.hex` files
3. Flash `.hex` files into respective LPC2129 controllers
4. Connect CAN nodes via transceivers
5. Power the system and verify functionality using switches

---

## 📸 Demonstration

![Automotive_Body_Control_System_Using_ARM7_LPC2129_and_CAN_Protocol](https://github.com/user-attachments/assets/a7b8611f-3be7-453d-a6fb-99c6eab5103c)


---

## 🎯 Applications

* Automotive Body Control Modules (BCM)
* Distributed Embedded Control Systems
* Smart Vehicle Electronics
* Automotive Prototyping and Research

---

## 🛠️ Tools & Technologies

* Embedded C Programming
* ARM7 LPC2129 Microcontroller
* CAN (Controller Area Network) Protocol
* Keil uVision IDE

---

## 🧪 Advantages

* Reduced wiring complexity
* High reliability and noise immunity
* Real-time distributed control
* Modular and scalable architecture
* Industry-relevant automotive design

---

## 🔮 Future Scope

* Sensor-based automation (rain / steering integration)
* Advanced diagnostics and CAN analytics
* RTOS-based scheduling
* IoT-enabled vehicle monitoring

---

## 📜 License

This project is licensed under the **MIT License**.

---

## 👨‍💻 Author

SABARINATHAN S
Embedded Systems Developer

* 🔗 LinkedIn: https://www.linkedin.com/in/sabarinathans23
* 📧 Email: [sabarinathansja23@gmail.com](mailto:sabarinathansja23@email.com)

---

## ⭐ Acknowledgment

If you find this project useful, please ⭐ star the repository.
