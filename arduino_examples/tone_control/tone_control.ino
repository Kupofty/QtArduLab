#define PASSIVE_BUZZER_PIN 8

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(PASSIVE_BUZZER_PIN, OUTPUT); 
}

void loop() 
{
  if (Serial.available() > 0) 
  {
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
    
    // LED CONTROL
    if (command == "TONE")
    {
      double value = valueStr.toDouble();
      
      if (value>=0)
      {
        tone(pin, value); 
        Serial.println("[CMD] Success");
      }
      else if (value == -1)
      {
         noTone(pin);
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
}
