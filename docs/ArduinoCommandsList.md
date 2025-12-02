# QtArduLab — Serial Command Manual

This document defines the communication protocol used between **QtArduLab** and the **Arduino**.  
All commands are ASCII strings terminated with a newline (`\n`) and processed line-by-line.

---
## General Format


| Field | Description |
|-------|-------------|
| **COMMAND** | Category of the command (e.g., `LED`) |
| **ID** | Numeric identifier (e.g., pin number) |
| **VALUE** | Action or state (e.g., `HIGH`, `LOW`) |  


**Example:**

| Command | Description |
|---------|-------------|
| `LED 13 HIGH` | Sets pin 13 HIGH |
| `LED 8 LOW`   | Sets pin 8 LOW |

> **Note:** The physical wiring and the device must match the pin numbers and type specified in the command.

------
### Command Feedback

In the provided arduino code examples, when a command is sent from the Qt GUI to the Arduino, the Arduino sends a confirmation message back over serial. This confirmation is automatically displayed in the Qt GUI input panel with a timestamp. 



**Arduino checks:**
1. Correct number of fields  
2. Valid command type  
3. Pin is numeric  
4. State is supported  
   
**Example:**

- Command sent: `LED 13 HIGH`  
 Arduino response: `[yyy-mm-dd hh:mm:ss] LED 1 -> HIGH`  

- Command sent: `LED LOW`  
Arduino response: `[yyy-mm-dd hh:mm:ss] invalid message format`  

> **Note:** The response messages can be customized as needed; currently, their only purpose is to alert the user.

