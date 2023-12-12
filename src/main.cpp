#include <Arduino.h>
// first include a library for fourier transform
#include <arduinoFFT.h>

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

#define SAMPLES 64              // Must be a power of 2
#define SAMPLING_FREQUENCY 1000 // Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT(); // Create an FFT object

unsigned int sampling_period_us; // Sampling period in microseconds
unsigned long microseconds;      // microseconds since program start
double vReal[SAMPLES];           // Create an array of size SAMPLES to hold real values
double vImag[SAMPLES];           // Create an array of size SAMPLES to hold imaginary values

float modules[3] = {0}; // This is where we're going to store the modules of the fourier transform
int RGB_values[3] = {0}; // This is where we're going to store the RGB values

// Number of samples to average
const int numSamples = 100;

// Circular buffer to store previous microphone readings
int micBuffer[numSamples] = {0};

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
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
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros(); // Overflows after around 70 minutes! (4294967295 us)
    vReal[i] = analogRead(PIN_MIC);
    vImag[i] = 0;
    while (micros() < (microseconds + sampling_period_us))
    {
    } // empty loop
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  for (int i = 0; i < 3; i++)
  {
    modules[i] = 0;
    RGB_values[i] = 0;
  }

  for (int i = 1; i < SAMPLES / 2; i++)
  {
    double a = pow(vReal[i], 2);
    double b = pow(vImag[i], 2);
    if (i <= SAMPLES / 6)
    {
      modules[0] += sqrt(a + b);
    }
    else if (i <= SAMPLES / 3)
    {
      modules[1] += sqrt(a + b);
    }
    else
    {
      modules[2] += sqrt(a + b);
    }
  }

  for (int i = 0; i < 3; i++)
  {
    modules[i] /= SAMPLES / 6;
  }

  // convert modules to RGB values
  RGB_values[0] = modules[0] /= 100;
  RGB_values[1] = modules[1] * 2;
  RGB_values[2] = modules[2] * 2;

  for (int i = 0; i < 3; i++)
  {
    RGB_values[i] = modules[i];
  }

  Serial.print(">Red:");
  Serial.println(RGB_values[0]);
  Serial.print(">Green:");
  Serial.println(RGB_values[1]);
  Serial.print(">Blue:");
  Serial.println(RGB_values[2]);

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

  delay(25);
}
