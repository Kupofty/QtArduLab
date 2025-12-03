# QtArduLab — Qt Serial Communication Tool for Arduino

---
## Project
QtArduLab is a lightweight Qt-based application designed for **sending commands to an Arduino** and **monitoring serial data** from an Arduino or any device using UART communication.  

Its main goal is to provide a clean and efficient GUI for **debugging, testing, and developing embedded systems**.

---
## Requirements
- Qt 6 (Widgets, SerialPort)
- C++17 or higher
- Arduino or compatible serial device

---
### Command Format
Commands sent to the Arduino from the GUI follow a custom format.  
The Arduino must use and decode this specific format to execute the correct actions.  

For a full list of commands and details, refer to the `ArduinoCommandsList.md` manual in the `docs` folder.
Examples are provided in the `arduino_examples` folder

---
## Copyright and licensing
This software is Copyright (c) Kupofty 2025.  
This project is licensed under the Apache 2.0 License.  
You are free to use, modify, and distribute this software, provided you include proper attribution and comply with the terms of the license.  
See the LICENSE file for more details.  