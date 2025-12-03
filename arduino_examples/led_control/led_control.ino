void setup() 
{ 
  Serial.begin(9600); 
  pinMode(LED_BUILTIN, OUTPUT); 
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
      Serial.println("invalid format");
      return;
    }
    
    // Split into 3 tokens
    String command  = message.substring(0, firstSpace);
    String pinStr = message.substring(firstSpace + 1, secondSpace);
    String value = message.substring(secondSpace + 1);

    //Check pin validity
    bool valid = true;
    
    // Check if the string contains only digits
    for (int i = 0; i < pinStr.length(); i++) {
        if (!isDigit(pinStr[i])) {
            valid = false;
            break;
        }
    }
   
    if (!valid) {
        Serial.println("invalid pin");
        return;
    }
    
    int pin = pinStr.toInt(); // safe now
    
    // LED CONTROL
    if (command == "LED")
    {
      if (value == "HIGH")
      {
        digitalWrite(pin, HIGH);
        Serial.println("Switching LED on pin " + pinStr + " to HIGH");
      }
      else if (value == "LOW")
      {
        digitalWrite(pin, LOW);
        Serial.println("[CMD] Switching LED on pin " + pinStr + " to LOW");
      }
      else
      {
        Serial.println("invalid value");
      }
    }
    else
    {
      Serial.println("invalid command");
    }
  }
}
