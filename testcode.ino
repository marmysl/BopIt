#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 4 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"



// variables
int sensorVal = 0;

void setup() {
  music.speakerPin = 9; //Auido out on pin 9
  SD.begin(SD_ChipSelectPin);
  music.setVolume(4);    //   0 to 7. Set volume level
  // Tilt switch
//  pinMode(2, INPUT);
//  digitalWrite(2, HIGH);
//  // Tilt switch output LED for testing
//  pinMode(13, OUTPUT);
}


void TiltSwitchTest() {
  int digitalVal = digitalRead(2);
  digitalWrite(13, digitalVal);
}

void PhotoResistorTest() {
  // read from photoresistor
  sensorVal = analogRead(A0);
  // LED gets brighter the darker it is at the sensor
  // invert the reading from 0-1023 to 1023-0
  sensorVal = 1023 - sensorVal;
  // write to LED at PWM pin 
  analogWrite(11, map(sensorVal, 0, 1023, 0, 255));
  delay(100);
}

void loop() {
  TiltSwitchTest();
  PhotoResistorTest();
  music.play("JonSnow.wav",2);
}
