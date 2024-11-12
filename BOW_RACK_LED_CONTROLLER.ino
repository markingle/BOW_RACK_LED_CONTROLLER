/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>
#include "esp_adc_cal.h"

const int voltagePin = 32;

int voltageValue = 0;
float Voltage = 0.0;

// How many leds in your strip?
#define NUM_LEDS 21

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI

#define DATA_PIN 33
#define CLOCK_PIN 13

#define RED 17
#define GREEN 18
#define BLUE 19
#define WHITE 25
#define CONTROL2 26
#define CONTROL3 27

int x = 0;
int jumperReading;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setColor(int x)
{
   switch (x) {
    case 1: 
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Red;
      }
      break;
    case 2: 
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Green;
      }
      break;
    case 3: 
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Blue;
      }
      break;
    case 4: 
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::White;
      }
      break;
    default:
      break;
  }
}

void checkJumpers()
{
  jumperReading = digitalRead(RED);
  if (jumperReading == LOW)
  {
    x = 1;  
    //Serial.println("RED");
    setColor(x);
  } else {
    jumperReading = digitalRead(GREEN);
    if (jumperReading == LOW)
    {
      x = 2;
      //Serial.println("GREEN");
      setColor(x);
    } else {
      jumperReading = digitalRead(BLUE);
      if (jumperReading == LOW)
      {
        x = 3;
        //Serial.println("BLUE");
        setColor(x);
      } else {
        jumperReading = digitalRead(WHITE);
        if (jumperReading == LOW)
        {
          x = 4;
          //Serial.println("WHITE");
          setColor(x);
        }
      }
    }
  }
}

uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void setup() {
  Serial.begin(115200);
  pinMode(RED, INPUT);
  pinMode(GREEN, INPUT);
  pinMode(BLUE, INPUT);
  pinMode(WHITE, INPUT);
  pinMode(CONTROL2, INPUT);
  pinMode(CONTROL3, INPUT);
  pinMode(32, INPUT); 

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(WHITE, HIGH);
  digitalWrite(CONTROL2, HIGH);
  digitalWrite(CONTROL3, HIGH);
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;  // GRB ordering is typical
  //FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());
  FastLED.setBrightness(128);  // Set global brightness to 50%
  delay(2000);  // If something ever goes wrong this delay will allow upload.
}

void loop() { 
  checkJumpers();
  FastLED.show();
  delay(500);

  voltageValue = analogRead(voltagePin);

  Voltage = readADC_Cal(voltageValue);
  Serial.println(Voltage/1000.0); // Print Voltage (in V)
  //Serial.println(Voltage);      // Print Voltage (in mV)
  delay(500);
  Serial.print("vV");
  Serial.println(voltageValue);
  delay(500);
}
