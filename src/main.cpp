#include <Arduino.h>
// sample the microphone and switch on the LEDs if the weighted average derivative of the signal is above a threshold, then switch them on proportionally to the average derivative

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

// length of the sampling buffer
#define SAMPLES 15
// create a buffer for the sampling values
int samples[SAMPLES];

// create a buffer for the derivative values
int derivatives[SAMPLES - 1];

float weight;
float decay_factor;
float average_derivative = 0;

int intensity;

// create a buffer for the RGB values
int RGB_values[3];

// index of the last sample
int last_sample = 0;

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
  weight = 1.0;
  decay_factor = 0.6;
  // save the microphone value in the buffer at the position last_sample
  samples[last_sample] = analogRead(PIN_MIC);
  // write the current microphone value to the serial port
  Serial.println(">MIC: " + String(samples[last_sample]));

  // calculate each derivative value beginning at the last sample
  for (int i = 0; i < SAMPLES - 1; i++)
  {
    derivatives[i] = samples[(last_sample + i + 1) % SAMPLES] - samples[(last_sample + i) % SAMPLES];
  }
  // calculate the average derivative by using weighted average (the most recent values have more weight, then they decay exponentially)
  for (int i = 0; i < SAMPLES - 1; i++)
  {
    average_derivative += weight * derivatives[i];
    weight *= decay_factor;
  }
  average_derivative /= SAMPLES - 1;

  // print the average derivative
  Serial.println(">DER: " + String(average_derivative));

  intensity = average_derivative > 0.3 ? average_derivative * 20 : 0;

  // convert modules to RGB values
  RGB_values[0] = intensity;
  RGB_values[1] = intensity;
  RGB_values[2] = intensity;

  // limit all the brightnesses between 0 and 255
  for (int i = 0; i < 3; i++)
  {
    if (RGB_values[i] > 255)
    {
      RGB_values[i] = 255;
    }
    else if (RGB_values[i] < 0)
    {
      RGB_values[i] = 0;
    }
  }
  analogWrite(PIN_RED, RGB_values[0]);
  analogWrite(PIN_GREEN, RGB_values[1]);
  analogWrite(PIN_BLUE, RGB_values[2]);

  // increment last_sample
  last_sample++;
  // if last_sample is equal to SAMPLES, reset it to 0
  if (last_sample == SAMPLES)
  {
    last_sample = 0;
  }
  // sleep for 100 microseconds
  delay(1);
  // delayMicroseconds(100);
}
