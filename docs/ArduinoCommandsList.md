# QtArduLab — Serial Command Manual

This document defines the serial protocol between **QtArduLab** and an **Arduino** device.  
All communication uses ASCII strings terminated with a newline (`\n`).

---
## 1. Commands (Qt → Arduino)

### 1.1 Format
Commands sent from Qt to the Arduino follow this structure: 

| Field 1  | Field 2 | Field 3 | 
|----------|---------|---------|
| COMMAND  | ID      | VALUE   |


### 1.2 Available Commands

| COMMAND  | Description                        | Format                   | ID                | Value                    | Example           |
|----------|------------------------------------|--------------------------|-------------------|--------------------------|-------------------|
| `DIGITAL`| Control a digital output           | `DIGITAL <PIN> <STATE>`  | Any digital pin   | `HIGH`, `LOW`            | `DIGITAL 13 HIGH` |
| `SERVO`  | Control a servo motor angle        | `SERVO <PIN> <ANGLE>`    | PWM-capable pin   | `0-180`                  | `SERVO 9 120`     |
| `TONE`   | Control a speaker / passive buzzer | `TONE <PIN> <FREQUENCY>` | Any digital pin   | `0-∞ = TONE`,`-1 = STOP` | `TONE 9 1000`     |

> **Note:** The physical wiring and the device must match the pin numbers and type specified in the command.
> **Note:** New commands will be added in the future


------
## 2. Arduino Responses (Arduino → Qt)
#### 2.1 Responses To Commands

Arduino must use the prefix `[CMD]` for messages that are responses to commands sent by QtArduLab.
QtArduLab extracts and displays only the payload (text after the bracket) in the **Commands** text panel .

**Examples:**
`[CMD] Success`
`[CMD] Switching LED 13 to HIGH`


#### 2.2 Non-Command Messages

Any Arduino message not starting with `[CMD]` is considered:
- Sensor data  
- Logging information  
- Debug output  
- Free-form serial messages  

**Examples:**
`[SENSOR] Temperature is 25°C`
`[SENSOR] Servomotor angle is 180°`

These messages are displayed in the **Sensors / Logs** text panel along with a timestamp of the received time.
Custom prefixes (e.g., `[SENSOR], [LOGS], [WARNING]`,...) can be added to give more info in the QtArduLab text panel.

------
## 3. Arduino Validation Rules

The Arduino sketch needs to validate incoming commands according to the following rules:

1. The incoming message must be stripped  in three tokens.  
2. The command keywords must be supported.  
3. The pin string must contain digits only  .  
4. The value must match a known action.  
5. After the action is executed, the Arduino should send a confirmation response using the correct prefix.

> **Note:** It is the responsibility of the user to correctly implement this validation logic in the Arduino sketch.  
Ready-to-use examples demonstrating a proper implementation are available in the **`arduino_examples/`** folder.
