// Include library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>
#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

int pot_pin = A1;

// Initialize the Liquid Crystal library with the numbers of the interface pins
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd1(0x27,16,2);

#define SD_ChipSelectPin 4 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"

long prev_millis = 0;

void setup() {
    // LCD
    lcd1.init();
    lcd1.backlight();
    lcd1.clear();

    pinMode(pot_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long curr_millis = millis();
  int interval = 1000;

  if(curr_millis - prev_millis > interval) {
    int pot_val = analogRead(pot_pin);
    lcd1.setCursor(0,0);
    lcd1.print(String(pot_val));    
  }
   
}
