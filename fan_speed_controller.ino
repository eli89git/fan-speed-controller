#include <Servo.h>

Servo esc;  // create servo object to control the ESC

const int buttonPin = 2; // pin of the button
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button
int speedStep = 0; // motor speed step
int currentSpeed = 1000; // start with the motor stopped

// define LED pins
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;

// Define an array that holds the speeds for each step
int speeds[] = {1000, 1550, 1600, 1650};
int numSpeeds = sizeof(speeds) / sizeof(int); // Calculate the number of speeds

// Define the time it should take to transition to the next speed (in milliseconds)
int deltaT = 1000;
// Define the time it should take to transition to the stopping speed (in milliseconds)
int stoppingDeltaT = 2000;

void setup() {
  Serial.begin(9600); // start serial communication at 9600 baud
  pinMode(buttonPin, INPUT); // initialize the button pin as input
  pinMode(ledPin1, OUTPUT); // initialize the LED pins as output
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  esc.attach(9);  // attaches the ESC on pin 9
  esc.writeMicroseconds(currentSpeed); // send initial signal for ESC starting
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);
  digitalWrite(ledPin3,HIGH);
  delay(4000); // delay to allow the ESC to recognize the starting signal
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPin3,LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the pushbutton input pin

  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == HIGH) { // if the current state is HIGH then the button went from off to on
      speedStep++; // increment speed step
      if (speedStep >= numSpeeds) { // loop back to zero after reaching the last speed
        speedStep = 0;
      }

      int targetSpeed = speeds[speedStep]; // Get the target speed from the array

      // Determine the transition time based on whether the motor is stopping or accelerating
      int transitionTime = speedStep == 0 ? stoppingDeltaT : deltaT;
      float increment = float((targetSpeed - currentSpeed)) / float((transitionTime / 10)); // Calculate the increment for each step
      
      Serial.print("Increment: "); // print button state to serial port
      Serial.println(increment);
      Serial.print("currentSpeed: "); // print button state to serial port
      Serial.println(currentSpeed);
      Serial.print("targetSpeed: "); // print button state to serial port
      Serial.println(targetSpeed);
      
      // Gradually change the speed to the target speed
      for(float i = currentSpeed; i != targetSpeed; i += increment) {
        esc.writeMicroseconds(i); // set the speed of the motor
        delay(10); // delay 10ms for each step to achieve deltaT or stoppingDeltaT in total
        Serial.print("i"); // print button state to serial port
        Serial.println(i);
      }
      
      currentSpeed = targetSpeed; // update current speed

      // turn LEDs on or off based on speed
      digitalWrite(ledPin1, speedStep > 0 ? HIGH : LOW);
      digitalWrite(ledPin2, speedStep > 1 ? HIGH : LOW);
      digitalWrite(ledPin3, speedStep > 2 ? HIGH : LOW);

      Serial.print("Button state: "); // print button state to serial port
      Serial.println(buttonState);
      Serial.print("Speed: "); // print speed to serial port
      Serial.println(currentSpeed);
    }
    delay(50); // debounce delay; increase if the output flickers
  }

  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop
}
