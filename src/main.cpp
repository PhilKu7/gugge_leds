#include <Arduino.h>

#define MOSFET_E0 10
#define MOSFET_E1 7
#define MOSFET_FAN 9
#define MOSFET_BED 8
#define TEMP_0 13

#define RED MOSFET_E0
#define GREEN MOSFET_BED
#define BLUE MOSFET_E1
#define MIC TEMP_0

// Number of samples to average
const int numSamples = 10;

// Circular buffer to store previous microphone readings
int micBuffer[numSamples] = {0};

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  // Set the LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(MIC, INPUT);
}

void loop()
{
  int sound = 0;
  int min = 1024;
  int max = 0;
  int current_mic;

  for (int i = 0; i < numSamples; i++)
  {
    current_mic = analogRead(MIC);
    min = min(current_mic, min);
    max = max(current_mic, max);
    delay(2);
  }

  sound = max - min;

  // Update the circular buffer with the current sound value
  for (int i = numSamples - 1; i > 0; i--)
  {
    micBuffer[i] = micBuffer[i - 1];
  }
  micBuffer[0] = sound;

  // Calculate the moving average of the sound values
  int filteredSound = 0;
  for (int i = 0; i < numSamples; i++)
  {
    filteredSound += micBuffer[i];
  }
  filteredSound /= numSamples;

  // Set RED pin to filtered sound value
  analogWrite(RED, filteredSound);   // 50% of 255
  analogWrite(GREEN, filteredSound); // 50% of 255
  analogWrite(BLUE, filteredSound);  // 50% of 255

  // Print filtered sound value to the terminal
  Serial.println(filteredSound);
}
