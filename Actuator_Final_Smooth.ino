#include <Servo.h>
#include <Adafruit_NeoPixel.h>

const int potPin = A0;           // Pin where the potentiometer is connected
const int ledPin = 6;            // Pin where the WS2812B strip is connected
const int numPixels = 20;        // Number of pixels in the strip
const int actuatorPin = 11;      // Pin to control the actuator

const int minValue = 380;        // Minimum potentiometer value
const int maxValue = 780;        // Maximum potentiometer value

Servo myServo;                   // Create a Servo object for the actuator
Servo myActuator;                // Create another Servo object for the actuator
Adafruit_NeoPixel strip(numPixels, ledPin, NEO_GRB + NEO_KHZ800); // Create NeoPixel object

void setup() {
  Serial.begin(9600);            // Start serial communication
  myServo.attach(9);             // Attach the servo to pin 9
  myActuator.attach(actuatorPin); // Attach the actuator to pin 10
  strip.begin();                 // Initialize the strip
  strip.show();                  // Initialize all pixels to 'off'
  setWhite();                    // Set the strip to white
}

void loop() {
  int potValue = analogRead(potPin);  // Read the potentiometer value

  if (potValue >= minValue && potValue <= maxValue) {
    Serial.print("Potentiometer Value: ");
    Serial.println(potValue);

    // Invert the mapping for the actuator
    int actuatorAngle = map(potValue, minValue, maxValue, 180, 0);
    myActuator.write(actuatorAngle);

    // Map the potentiometer value to the servo range (0 to 180 degrees)
    int servoAngle = map(potValue, minValue, maxValue, 10, 165);
    myServo.write(servoAngle);

    // Map the potentiometer value to the color range (0 to 255)
    int colorValue = map(potValue, minValue, maxValue, 0, 255);
    strip.fill(strip.Color(255-colorValue, 0,colorValue)); // Blue to Red gradient
    strip.show();
  } else {
    Serial.println("Potentiometer value out of range");
    myActuator.write(0); // Ensure actuator is off
  }

  delay(100);  // Delay for stability (adjust as needed)
}

// Function to set the LED strip to white
void setWhite() {
  strip.fill(strip.Color(255, 255, 255)); // Set all pixels to white
  strip.show(); // Update the strip
}
