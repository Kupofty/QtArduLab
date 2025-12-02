// Turn ON/OFF the built-in LED when receiving specific serial command from Qt GUI

void setup() {
  Serial.begin(9600);           // Initialize Serial
  pinMode(LED_BUILTIN, OUTPUT); // Initialize LED pin
}

void loop() 
{
  // Check if data is available on Serial
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n'); // Read incoming line
    message.trim();                                // Remove \r, spaces, etc.
    Serial.println("Received: " + message);

    if (message == "HIGH") {
      digitalWrite(LED_BUILTIN, HIGH);
    } 
    else if (message == "LOW") {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
