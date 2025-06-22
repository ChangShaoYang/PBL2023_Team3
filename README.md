# Smart Autonomous Robot for Red Ball Collection  
_International PBL Project_

![Robot Prototype](docs/13836_0.jpg)  
*Figure: Final robot prototype used in testing and demonstration*

---

## 🧠 Project Overview

This project was developed as part of the International PBL program, where our team designed and built a smart autonomous robot capable of:

- Detecting red balls using a Pixy2 camera  
- Navigating autonomously with ultrasonic sensors  
- Avoiding obstacles and correcting its course  
- Collecting and releasing red balls using a custom servo-powered claw  
- Using non-blocking multitasking to improve motion control

The robot combines hardware integration and autonomous behavior logic, with a focus on practical implementation using Arduino-based systems.

---

## 🎯 Features

- 🎥 **Pixy2 Vision System**: Detects and locates red balls via color-based signature tracking.
- 🧠 **Autonomous Behavior**: Detect → Track → Collect → Return to Wall → Dump.
- 🛑 **Collision and Distance Sensors**: Ultrasonic sensors for wall detection, and physical switches for contact sensing.
- 🔁 **Non-blocking Logic**: Uses `millis()`-based loop structure to allow responsive multi-tasking.
- ⚙️ **Mechanical Claw**: Servo-driven claw for red ball pick-up and drop-off.
- 📦 **3D-Printed Modular Components**: Easy-to-swap structures made with low-cost materials.

---

## 🛠️ Hardware Components

| Component                  | Description                        |
|---------------------------|------------------------------------|
| Arduino Mega 2560         | Main controller board              |
| Pixy2 Camera              | Vision-based red ball detection    |
| L298N Motor Driver        | Dual H-Bridge for DC motors        |
| MG996R Servo Motor        | Drives claw movement               |
| Ultrasonic Sensors (x3)   | Front-left, front-right, rear wall sensing |
| Collision Sensor          | Front-mounted bumper detection     |
| Custom Claw + Funnel      | Designed to hold and dump balls    |
| External Power Supply     | Dual-line power for motors & logic |

---

## 📁 Project Structure

├── code/ # Arduino source code

│ └── PBL_Team3.ino

├── docs/

│ ├── robot.jpg # Robot image

│ ├── prototype.jpg # prototype 3D design

├── README.md


---

## 💡 How It Works

1. Robot starts moving forward while scanning for red balls via Pixy2.
2. When a red ball is detected:
   - Robot centers its view and adjusts distance.
   - Once close enough, lowers the claw to collect it.
3. After collecting, robot moves backward until hitting a wall (via collision sensor).
4. Servo lifts to dump balls.
5. Robot resumes exploration.

> The behavior loop is controlled via multiple helper functions (`detect_redball`, `chase_collect_redball`, `collect_redball`) and event triggers.

---

## 🧠 Sample Logic Diagram (3D Visual)

![3D Design Concept](docs/13835_0.jpg)  
*Figure: Simulation of pick-up and dump motion mechanism*

---

## 🧾 Code Snapshot

It includes:

- Pin definitions
- Pixy2 + Servo + Motor control setup
- Multisensor fusion logic
- Task-oriented behavior functions

> _To run this code, connect the required sensors and upload the sketch to Arduino Mega via the Arduino IDE._

---

## 📎 Notes

- This robot was used in an indoor arena with time-limited collection tasks.
- The design prioritizes low-cost materials, extensibility, and robustness under simple sensor configurations.
