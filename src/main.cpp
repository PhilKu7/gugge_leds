#include <Arduino.h>

// Pin number for the LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 11
#endif

#define MOSFET_E0 10
#define MOSFET_E1 7
#define MOSFET_FAN 9
#define MOSFET_BED 8
#define TEMP_0 13

#define RED MOSFET_E0
#define GREEN MOSFET_BED
#define BLUE MOSFET_E1
#define MIC TEMP_0

const int ledPin = LED_BUILTIN;

void setup()
{
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(MIC, INPUT);
}

int i = 0;

void loop()
{
  i = (analogRead(MIC) / 4 - 200) < 0 ? 0 : (analogRead(MIC) / 4 - 200);
  // Set RED pin to 50% PWM
  analogWrite(RED, i);   // 50% of 255
  analogWrite(GREEN, i); // 50% of 255
  analogWrite(BLUE, i);  // 50% of 255

  // Turn on the LED
  digitalWrite(ledPin, HIGH);
  delay(20); // Wait for 1 second

  // Turn off the LED
  digitalWrite(ledPin, LOW);
  delay(20); // Wait for 1 second
}
