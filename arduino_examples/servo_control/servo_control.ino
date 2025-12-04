#include <Servo.h>

Servo myservo; 
int pos = 0;

void setup() 
{ 
  Serial.begin(9600); 
}

void loop() 
{
  if (!Serial.available() > 0) 
    return;

  String message = Serial.readStringUntil('\n');
  message.trim();

  // Parse message
  int firstSpace  = message.indexOf(' ');
  int secondSpace = message.indexOf(' ', firstSpace + 1);
  if (firstSpace < 0 || secondSpace < 0) 
  {
    Serial.println("[CMD] Invalid format");
    return;
  }
  
  // Split into 3 tokens
  String command  = message.substring(0, firstSpace);
  String pinStr = message.substring(firstSpace + 1, secondSpace);
  String valueStr = message.substring(secondSpace + 1);

  //Check pin validity (the string contains only digits)
  for (int i = 0; i < pinStr.length(); i++) {
      if (!isDigit(pinStr[i])) {
        Serial.println("[CMD] Invalid pin");
        return;
      }
  }

  int pin = pinStr.toInt(); 
  myservo.attach(pin);
  int value = valueStr.toInt();

  // SERVOMOTOR CONTROL
  if (command == "SERVO")
  {
    if (value>=0 && value <=180)
    {
      myservo.write(value);
      Serial.println("[CMD] Success");
    }
    else
    {
      Serial.println("[CMD] Invalid value");
    }
  }
  else
  {
    Serial.println("[CMD] Invalid command");
  }

}
