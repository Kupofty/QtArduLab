// HC-SR04 pins
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Send a 10 µs pulse to trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  String distanceStr = "[HC-SR04] Distance: " + String(distance) + " cm";
  Serial.println(distanceStr);

  delay(500); // wait 0.5 seconds before next measurement
}
