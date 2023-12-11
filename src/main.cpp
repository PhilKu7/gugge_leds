#include <Arduino.h>

// Pin number for the LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 11
#endif

#define MOSFET_E0 10
#define MOSFET_E1 7
#define MOSFET_FAN 9
#define MOSFET_BED 8

#define RED MOSFET_E0
#define GREEN MOSFET_BED
#define BLUE MOSFET_E1

const int ledPin = LED_BUILTIN;

void setup()
{
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

int i = 0;
void loop()
{
  i < 10 ? i++ : i = 0;
  // Set RED pin to 50% PWM
  analogWrite(RED, i); // 50% of 255
  analogWrite(GREEN, i); // 50% of 255
  analogWrite(BLUE, i); // 50% of 255

  // Turn on the LED
  digitalWrite(ledPin, HIGH);
  delay(200); // Wait for 1 second

  // Turn off the LED
  digitalWrite(ledPin, LOW);
  delay(200); // Wait for 1 second
}
