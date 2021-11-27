// Include library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>
#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

// Initialize with address 0x27, size 16x2 LCD
LiquidCrystal_I2C lcd1(0x27,16,2);

// Create object called "music" to control audio
TMRpcm music; 


/*
 * ===============================================
 * cONSTANTS / GLOBAL VARS
 * ===============================================
 */
#define SD_ChipSelectPin 4 //Chip select is pin number 4
#define pot_pin A1
#define tilt_pin 2
#define ptr_pin A2
#define spk_pin 9

int fail_time = 5000; // Start with 5 seconds
unsigned long prev_millis = 0;
int next_act = 0;
int score = 0;
int prev_pot = -1;


/*
 * ===============================================
 * TILT SWITCH
 * ===============================================
 */
bool ReadTilt() {
  int val = digitalRead(tilt_pin);
  return (val == HIGH);
}

/*
 * ===============================================
 * PHOTO-RESISTOR
 * ===============================================
 */
bool ReadPTR() {
  int ptr_thresh = 100; // Threshold for "covered"
  int val = analogRead(ptr_pin);
  return (val > ptr_thresh); // Might need to flip LT/GT
}

/*
 * ===============================================
 * POTENTIOMETER
 * ===============================================
 */
bool ReadPot(){
  // goes from 88 to 1023
  int val = analogRead(pot_pin);
  int pot_low = 100, pot_high = 950;

  // LOW = 0 -> 1 -> 2 = HIGH
  if (val < pot_low) val = 0;
  else if (val < pot_high) val = 1;
  else val = 2;

  if (val == prev_pot || val == 1) {
    // No change or in the middle
    return false;
  } else {
    // Changed sides
    prev_pot = val;
    return true;
  }
}

/*
 * ===============================================
 * Helper Functions
 * ===============================================
 */
void printScore() {
    lcd1.setCursor(0,1);
    lcd1.print("Score: ");
    lcd1.setCursor(7,1);
    lcd1.print(String(score));
}

/*
 * ===============================================
 * START OF SETUP
 * ===============================================
 */
void setup() {
  // PIN SETUP
  pinMode(pot_pin, INPUT);
  pinMode(ptr_pin, INPUT);
  pinMode(tilt_pin, INPUT);
  pinMode(spk_pin, OUTPUT); // Probably doesn't matter?
  digitalWrite(spk_pin, LOW); // Also doesn't matter?

  // Variables Setup
  score = 0;
  next_act = 0;
  randomSeed(analogRead(0)); // Read noise for random seed
  next_act = random(3); // Random # from 0:2
  ReadPot(); // Initializes prev_pot
  
  // LCD
  lcd1.init();
  lcd1.backlight();
  
  // SD Card
  if (!SD.begin(SD_ChipSelectPin)) {
    // Only ^^ is needed, rest is just to display if error
    for (int i=5;i>0;i--) {
      lcd1.setCursor(0,0);
      lcd1.print("SD FAILED");
      lcd1.setCursor(0,1);
      lcd1.print(String(i));
      delay(1000);
    }
    lcd1.clear();
  }
  
  // Music setup
  music.speakerPin = spk_pin; // Audio out on pin 9
  music.setVolume(4);    // 0 to 7. Set volume level
}

/*
 * ===============================================
 * START OF LOOP
 * ===============================================
 */
void loop() {
  next_act = random(3); // Random # from 0:2
  if (next_act == 0) {
    // TILT SWITCH
    music.play("tilt.wav");
  } else if (next_act == 1) {
    // PHOTO RESISTOR
    music.play("ptr.wav");
  } else {
    // POTENTIOMETER
    music.play("pot.wav");
  }

  unsigned long curr_millis = millis();
  prev_millis = curr_millis;

  bool act_done = false;
  bool act_wrong = false;
  while (curr_millis - prev_millis < fail_time) {
    curr_millis = millis();
    bool tilted = ReadTilt();
    bool covered = ReadPTR();
    bool pot_change = ReadPot();

    if (next_act == 0) {
      // TILT SWITCH
      act_done = tilted;
      act_wrong = covered || pot_change;
    } 
    if (next_act == 1) {
      // PHOTO RESISTOR
      act_done = covered;
      act_wrong = tilted || pot_change;
    } else {
      // POTENTIOMETER
      act_done = pot_change;
      act_wrong = tilted || covered;
    }
    
    if (act_done || act_wrong)
      break;    
  }

  if (act_wrong || !act_done) {
    // Player has LOST (WRONG ACTION)
    
    // Display Score
    lcd1.clear();
    printScore();

    // Stop game / Dead loop
    while(true) {
      // Inform user failed
      lcd1.setCursor(0,0);
      if (act_wrong)
        lcd1.print("WRONG ACTION!   ");
      else
        lcd1.print("OUT OF TIME!    ");
        
      delay(1500);
      
      // Display Reset suggestion
      lcd1.setCursor(0,0);
      lcd1.print("Reset to play   ");
      delay(1500);
    }
  }
    
  // Player correctly acted
  score++;
  fail_time = fail_time * 0.90; // Speed up the game

  // Show on LCD for correct input
  lcd1.setCursor(0,0);
  lcd1.print("--- CORRECT ----");
  for (int i=0; i < 6; i++) {
    lcd1.scrollDisplayLeft();
    delay(100);
  }
  for (int i=0; i < 6; i++) {
    lcd1.scrollDisplayRight();
    delay(100);
  }
    
}
