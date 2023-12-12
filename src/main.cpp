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
  // Initialize serial communication
  Serial.begin(9600);
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(MIC, INPUT);
}

int sound = 0;
int min, max, current_mic;

void loop()
{
  max = 0;
  min = 1024;
  for (int i = 0; i < 10; i++)
  {
    current_mic = analogRead(MIC);
    if (current_mic < min)
    {
      min = current_mic;
    }
    if (current_mic > max)
    {
      max = current_mic;
    }
    delay(1);
  }
  
  sound = max-min;
  // Set RED pin to 50% PWM
  analogWrite(RED, sound);   // 50% of 255
  analogWrite(GREEN, sound); // 50% of 255
  analogWrite(BLUE, sound);  // 50% of 255

  // Print analogRead(MIC) to the terminal
  Serial.println(max-min);
}
