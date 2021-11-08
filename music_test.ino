#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 4
TMRpcm music;

void setup() {
  // put your setup code here, to run once:
  music.speakerPin = 9;
  music.setVolume(10);
  music.quality(0);
  pinMode(8, OUTPUT);
  if(!SD.begin(SD_ChipSelectPin)) {
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  music.play("1.wav");
  delay(100000);
}
