const int potPin = A0;            // Rotary potentiometer signal pin
const int feedbackPin = A1;       // Actuator feedback signal pin
const int actuatorPin1 = 3;       // H-bridge IN1 pin
const int actuatorPin2 = 4;       // H-bridge IN2 pin
const int enaPin = 5;             // H-bridge ENA pin

const int potMin = 500;             // Minimum value of rotary potentiometer
const int potMax = 1023;          // Maximum value of rotary potentiometer
const int feedbackMin = 0;        // Minimum value of actuator feedback potentiometer
const int feedbackMax = 1023;     // Maximum value of actuator feedback potentiometer
const float tolerancePercent = 0.10; // 10% tolerance

const int deadband = 10;          // Deadband around the target position
const int speed = 128;            // Speed of actuator (0-255 for PWM, adjust as needed)
const int overshootDelay = 2000; // Delay after overshooting to avoid immediate reverse

bool overshot = false;            // Flag to indicate if the actuator has overshot the target
unsigned long overshootTime = 0;  // Time when the actuator last overshot the target

void setup() {
  Serial.begin(9600);            // Initialize serial communication
  pinMode(actuatorPin1, OUTPUT); // Set actuator control pins as outputs
  pinMode(actuatorPin2, OUTPUT);
  pinMode(enaPin, OUTPUT);       // Set enable pin as an output
  
  digitalWrite(enaPin, HIGH);    // Enable the H-bridge
}

void loop() {
  int potValue = analogRead(potPin);            // Read rotary potentiometer value
  int feedbackValue = analogRead(feedbackPin);  // Read actuator feedback value
  
  // Map potentiometer value to actuator target position
  int targetPosition = map(potValue, potMin, potMax, feedbackMin, feedbackMax);

  // Calculate tolerance range
  int tolerance = targetPosition * tolerancePercent;
  
  // Debugging output
  //Serial.print("Potentiometer value: ");
  //Serial.println(potValue);
  //Serial.print("Mapped target position: ");
  //Serial.println(targetPosition);
  Serial.print("Feedback value: ");
  Serial.println(feedbackValue);
  //Serial.print("Tolerance: ");
  //Serial.println(tolerance);
  //Serial.print("Deadband: ");
  //Serial.println(deadband);

  // Check if enough time has passed since the last overshoot
  if (overshot && (millis() - overshootTime < overshootDelay)) {
    // Prevent movement if within overshoot delay
    digitalWrite(actuatorPin1, LOW);
    digitalWrite(actuatorPin2, LOW);
    return;
  }
  
  // Determine if actuator needs to move
  if (feedbackValue < (targetPosition - tolerance - deadband)) {
    // Move actuator to the right
    analogWrite(actuatorPin1, speed);
    digitalWrite(actuatorPin2, LOW);
    overshot = false;  // Reset overshoot flag
  } else if (feedbackValue > (targetPosition + tolerance + deadband)) {
    // Move actuator to the left
    digitalWrite(actuatorPin1, LOW);
    analogWrite(actuatorPin2, speed);
    overshot = false;  // Reset overshoot flag
  } else {
    // Stop actuator if within range
    digitalWrite(actuatorPin1, LOW);
    digitalWrite(actuatorPin2, LOW);
    // Check if overshoot occurred
    if (feedbackValue < (targetPosition - tolerance - deadband) || feedbackValue > (targetPosition + tolerance + deadband)) {
      overshot = true;
      overshootTime = millis();  // Record the time of overshoot
    }
  }

  delay(50); // Delay for stability
}
