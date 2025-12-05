#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(RED_LED, OUTPUT); 
  pinMode(GREEN_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT); 
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
  
  if (command == "ANALOG")
  {
    int value = valueStr.toInt(); 
      
    if (value >=0 && value <= 255)
    {
      analogWrite(pin, value);
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
