#include <Arduino.h>

#define MOSFET_E0 10
#define MOSFET_E1 7
#define MOSFET_FAN 9
#define MOSFET_BED 8
#define TEMP_0 13
#define Z_MAX 19

#define PIN_RED MOSFET_E0
#define PIN_GREEN MOSFET_BED
#define PIN_BLUE MOSFET_E1
#define PIN_MIC TEMP_0
#define PIN_MIC_D Z_MAX

// Number of samples to average
const int numSamples = 100;

// Circular buffer to store previous microphone readings
int micBuffer[numSamples] = {0};

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  // Set the LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_MIC, INPUT);
  pinMode(PIN_MIC_D, INPUT);
}

void loop()
{
  int sound_level = 0;          // This is where we're going to store the raw analog value
  int change[numSamples] = {0}; // This is where we're going to store the derivative of the sound level
  float average_change = 0;       // This is where we're going to store the average change of the sound level
  int brightness = 0;           // This is where we're going to store the brightness of the LED

  // Then measure the sound level
  sound_level = analogRead(PIN_MIC); // Read the analog value of the microphone

  // Save the sound value to the circular buffer
  for (int i = numSamples - 1; i > 0; i--)
  {
    micBuffer[i] = micBuffer[i - 1];
  }
  micBuffer[0] = sound_level;

  // Calculate each change of the sound level
  for (int i = 0; i < numSamples - 1; i++)
  {
    change[i] = micBuffer[i] - micBuffer[i + 1];
  }
  // Calculate the average change of the sound level
  for (int i = 0; i < numSamples - 1; i++)
  {
    average_change += change[i];
  }
  average_change /= numSamples;
  if (average_change*200 > 6)
  {
    brightness = average_change*200;
  }
  else
  {
    brightness = 0;
  }
  // Set brightness of the LEDs
  analogWrite(PIN_RED, brightness);
  analogWrite(PIN_GREEN, brightness);
  analogWrite(PIN_BLUE, brightness);

  // Print the sound level to the Serial Monitor
  Serial.print(">Sound:");
  Serial.println(sound_level);

  // Print the brightness to the Serial Monitor
  Serial.print(">Brightness:");
  Serial.println(brightness);
  // wait 100 microseconds before the next loop
  delayMicroseconds(100);
}
