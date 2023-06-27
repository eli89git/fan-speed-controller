#include <Servo.h>

Servo esc;  // create servo object to control the ESC

const int buttonPin = 2; // pin of the button
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button
int speedStep = 0; // motor speed step

// define LED pins
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;

// Define an array that holds the speeds for each step
int speeds[] = {1000, 1500, 1600, 1700};
int numSpeeds = sizeof(speeds) / sizeof(int); // Calculate the number of speeds

void setup() {
  Serial.begin(9600); // start serial communication at 9600 baud
  pinMode(buttonPin, INPUT); // initialize the button pin as input
  pinMode(ledPin1, OUTPUT); // initialize the LED pins as output
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  esc.attach(9);  // attaches the ESC on pin 9
  esc.writeMicroseconds(1000); // send initial signal for ESC starting
  delay(3000); // delay to allow the ESC to recognize the starting signal
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the pushbutton input pin

  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == HIGH) { // if the current state is HIGH then the button went from off to on
      speedStep++; // increment speed step
      if (speedStep >= numSpeeds) { // loop back to zero after reaching the last speed
        speedStep = 0;
      }
      int speed = speeds[speedStep]; // Get the speed from the array
      esc.writeMicroseconds(speed); // set the speed of the motor
      
      // turn LEDs on or off based on speed
      digitalWrite(ledPin1, speedStep > 0 ? HIGH : LOW);
      digitalWrite(ledPin2, speedStep > 1 ? HIGH : LOW);
      digitalWrite(ledPin3, speedStep > 2 ? HIGH : LOW);

      Serial.print("Button state: "); // print button state to serial port
      Serial.println(buttonState);
      Serial.print("Speed: "); // print speed to serial port
      Serial.println(speed);
    }
    delay(50); // debounce delay; increase if the output flickers
  }

  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop
}
