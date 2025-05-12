// Define pin numbers
const int potPin = A0;    // Analog pin connected to the potentiometer
const int pwmPin = 9;     // PWM pin connected to the LAC control board

// Define the range of values for mapping
const int potMin = 0;     // Minimum potentiometer value
const int potMax = 1023;  // Maximum potentiometer value (for 10-bit ADC)
const int pwmMin = 1000;  // Minimum PWM value (adjust based on actuator specs)
const int pwmMax = 2000;  // Maximum PWM value (adjust based on actuator specs)

void setup() {
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(potPin);
  
  // Map potentiometer value to PWM range
  int pwmValue = map(potValue, potMin, potMax, pwmMin, pwmMax);
  
  // Write the PWM value to the LAC control board
  analogWrite(pwmPin, pwmValue);
  
  // Print values for debugging
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" PWM Value: ");
  Serial.println(pwmValue);
  
  delay(50);  // Small delay for stability
}

