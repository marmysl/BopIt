// Include library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>
#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

// Initialize the Liquid Crystal library with the numbers of the interface pins
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd1(0x27,16,2);

#define SD_ChipSelectPin 4 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"

// variables
int pot_pin = A1;
int sensorVal = 0;
long prev_millis = 0;

void TiltSwitchTest() {
  int digitalVal = digitalRead(2);
  if (digitalVal == 0) {
    lcd1.print("Tilted");
  } else {
    lcd1.print("Not tilted");
  }
}

void PhotoResistorTest() {
  // read from photoresistor
  sensorVal = analogRead(A2);
  lcd1.setCursor(1,0);
  lcd1.print(sensorVal);
}

void setup() {
    // LCD
    lcd1.init();
    lcd1.backlight();
    lcd1.clear();
    lcd1.setCursor(0,0);

    pinMode(pot_pin, INPUT);

    music.speakerPin = 9; //Auido out on pin 9
    SD.begin(SD_ChipSelectPin);
    music.setVolume(7);    //   0 to 7. Set volume level
}

void PotTest(){
  unsigned long curr_millis = millis();
  int interval = 1000;

  if(curr_millis - prev_millis > interval) {
    int pot_val = analogRead(pot_pin);
    lcd1.setCursor(0,0);
    lcd1.print("read");
    lcd1.setCursor(0,1);
    lcd1.print(String(pot_val)); 
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  lcd1.print("Hello World");
  //music.play("1.wav");
  //TiltSwitchTest();
  //delay(100);
  //PhotoResistorTest();
  //PotTest();
  //delay(250);
}
