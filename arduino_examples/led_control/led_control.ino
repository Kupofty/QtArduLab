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
      Serial.println("invalid message format");
      return;
    }
    
    // Split into 3 tokens
    String command  = message.substring(0, firstSpace);
    String pinStr = message.substring(firstSpace + 1, secondSpace);
    String value = message.substring(secondSpace + 1);

    //Check pin validity
    int pin = pinStr.toInt();
    if(pinStr.toInt() < 0)
    {
      Serial.println("invalid pin");
      return;
    }
      
    // LED CONTROL
    if (command == "LED")
    {
      if (value == "HIGH")
      {
        digitalWrite(pin, HIGH);
        Serial.println("LED " + pinStr + " -> HIGH");
      }
      else if (value == "LOW")
      {
        digitalWrite(pin, LOW);
        Serial.println("LED " + pinStr + " -> LOW");
      }
      else
      {
        Serial.println("invalid LED value");
      }
    }
    else
    {
      Serial.println("invalid command");
    }
  }
}
