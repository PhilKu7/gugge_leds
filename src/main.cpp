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

// FIR filter coefficients
const int filterLength = 5;
const float filterCoefficients[filterLength] = {0.2, 0.2, 0.2, 0.2, 0.2};

// Circular buffers to store previous microphone readings
float maxBuffer[filterLength] = {0};
float minBuffer[filterLength] = {0};
float micBuffer[filterLength] = {0}; // Add this line to declare and initialize the micBuffer array

// Variables to store filtered max and min values
float filteredMax = 0;
float filteredMin = 1024;

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
    delay(2);
  }

  // Apply filtering to max value
  for (int i = filterLength - 1; i > 0; i--)
  {
    maxBuffer[i] = maxBuffer[i - 1];
  }
  maxBuffer[0] = max;

  filteredMax = 0;
  for (int i = 0; i < filterLength; i++)
  {
    filteredMax += filterCoefficients[i] * maxBuffer[i];
  }

  // Apply filtering to min value
  for (int i = filterLength - 1; i > 0; i--)
  {
    minBuffer[i] = minBuffer[i - 1];
  }
  minBuffer[0] = min;

  filteredMin = 1024;
  for (int i = 0; i < filterLength; i++)
  {
    filteredMin += filterCoefficients[i] * minBuffer[i];
  }

  sound = filteredMax - filteredMin;

  // Update the circular buffer with the current sound value
  for (int i = filterLength - 1; i > 0; i--)
  {
    micBuffer[i] = micBuffer[i - 1];
  }
  micBuffer[0] = sound;

  // Apply the FIR filter
  float filteredSound = 0;
  for (int i = 0; i < filterLength; i++)
  {
    filteredSound += filterCoefficients[i] * micBuffer[i];
  }

  // Set RED pin to filtered sound value
  analogWrite(RED, filteredSound);   // 50% of 255
  analogWrite(GREEN, filteredSound); // 50% of 255
  analogWrite(BLUE, filteredSound);  // 50% of 255

  // Print filtered sound value to the terminal
  Serial.println(filteredSound);
}
