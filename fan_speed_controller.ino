#include <Servo.h>

Servo esc;  // create servo object to control the ESC

const int buttonPin = 2; // pin of the button
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button
int speedStep = 0; // motor speed step
int speed = 1000;

// define LED pins
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;

void setup() {
  Serial.begin(9600); // start serial communication at 9600 baud
  pinMode(buttonPin, INPUT); // initialize the button pin as input
  esc.attach(9);  // attaches the ESC on pin 9
  esc.writeMicroseconds(speed); // send initial signal for ESC starting
  delay(8000); // delay to allow the ESC to recognize the starting signal
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the pushbutton input pin

  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == HIGH) { // if the current state is HIGH then the button went from off to on
      speedStep++; // increment speed step
      if (speedStep > 3) { // loop back to zero after five
        speedStep = 0;
      }
      speed = map(speedStep, 0, 3, 1000, 1600); // map speed step to appropriate ESC range
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
