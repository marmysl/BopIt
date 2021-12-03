#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27,16,2);

/*
 * ===============================================
 * CONSTANTS / GLOBAL VARS
 * ===============================================
 */
#define pot_pin A1
#define tilt_pin 2
#define ptr_pin A2
#define spk_pin 9

int fail_time = 5000; // Start with 5 seconds
unsigned long prev_millis = 0;
int next_act = 0;
int score = 0;
bool prev_pot = false;

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

void wait(int duration) {
  unsigned long cmil = millis();
  unsigned long pmil = cmil;
  while (cmil - pmil < duration) {
    cmil = millis();
  }
}

/*
 * ===============================================
 * TILT SWITCH
 * ===============================================
 */
bool ReadTilt() {
  int val = digitalRead(tilt_pin);
  return (val == LOW);
}

/*
 * ===============================================
 * PHOTO-RESISTOR
 * ===============================================
 */
bool ReadPTR() {
  int ptr_thresh = 300; // Threshold for "covered"
  int val = analogRead(ptr_pin);
  return (val < ptr_thresh);
}

/*
 * ===============================================
 * POTENTIOMETER
 * ===============================================
 */
bool ReadPot(){
  // goes from 88 to 1023
  bool val = analogRead(pot_pin) < 470;
  
  if (val == prev_pot) {
    // No change
    return false;
  } else {
    // Changed sides
    return true;
  }
}

/*
 * ===============================================
 * START OF SETUP
 * ===============================================
 */
void setup() {
  // put your setup code here, to run once:

  pinMode(pot_pin, INPUT);
  pinMode(ptr_pin, INPUT);
  pinMode(tilt_pin, INPUT);
  pinMode(spk_pin, OUTPUT); // Probably doesn't matter?
  //digitalWrite(spk_pin, LOW); // Also doesn't matter?

  score = 0;
  next_act = 0;
  prev_pot = analogRead(pot_pin) < 470;
  randomSeed(analogRead(A0)); // Read noise for random seed
  next_act = random(3); // Random # from 0:2
  //next_act = 1;
  
  lcd1.init();
  lcd1.backlight();
}

/*
 * ===============================================
 * START OF LOOP
 * ===============================================
 */
void loop() {
  next_act = random(3);
  wait(750);
  if (next_act == 0) {
    // TILT SWITCH
    lcd1.setCursor(0,0);
    lcd1.print("TILT");
    tone(spk_pin,440,300);
  } else if (next_act == 1) {
    // PHOTO RESISTOR
    lcd1.setCursor(0,0);
    lcd1.print("BONK");
    tone(spk_pin,880,300);
  } else {
    // POTENTIOMETER
    lcd1.setCursor(0,0);
    lcd1.print("OPEN");
    tone(spk_pin,220,300);
  }

  /*
   * ===============================================
   * MAIN GAME
   * ===============================================
   */
  unsigned long curr_millis = millis();
  prev_millis = curr_millis;

  bool act_done = false;
  bool act_wrong = false;
  prev_pot = analogRead(pot_pin) < 470;
  while (curr_millis - prev_millis < fail_time) {
    curr_millis = millis();
    bool tilted = ReadTilt();
    bool covered = ReadPTR();
    bool pot_change = ReadPot();

    if (next_act == 0) {
      // TILT SWITCH
      act_done = tilted;
      act_wrong = pot_change;
    } else if (next_act == 1) {
      // PHOTO RESISTOR
      act_done = covered;
      act_wrong = pot_change;
    } else {
      // POTENTIOMETER
      act_done = pot_change;
      //act_wrong = tilted;
    }

    if (act_done || act_wrong)
      break;    
  }

  /*
   * ===============================================
   * Player Failed
   * ===============================================
   */
  if (act_wrong || !act_done) {
    // Player has LOST (WRONG ACTION)
    
    // Display Score
    lcd1.clear();
    printScore();

    bool first_time = true;
    // Stop game / Dead loop
    while(true) {
      // Inform user failed
      lcd1.setCursor(0,0);
      if (act_wrong)
        lcd1.print("WRONG ACTION!   ");
      else
        lcd1.print("OUT OF TIME!    ");

      if (first_time) {
        tone(spk_pin,294,250);
        wait(500);
        tone(spk_pin,278,250);
        wait(500);
        tone(spk_pin,262,250);
        wait(500);
        tone(spk_pin,247,750);

        first_time = false;
      } else {
        wait(1500);
      }
      // Display Reset suggestion
      lcd1.setCursor(0,0);
      lcd1.print("Push to play    ");
      wait(1500);
    }
  }
    
  /*
   * ===============================================
   * Player Success
   * ===============================================
   */
  score++;
  fail_time = fail_time * 0.90; // Speed up the game

  // Congratulate winner!!
  if (score >= 99) {
    lcd1.clear();
    while (true) {
      lcd1.setCursor(0,0);
      lcd1.print("--- YOU  WIN ---");
      for (int i=0; i < 6; i++) {
        lcd1.scrollDisplayLeft();
        wait(100);
      }
      for (int i=0; i < 6; i++) {
        lcd1.scrollDisplayRight();
        wait(100);
      }
      lcd1.setCursor(0,1);
      lcd1.print("Reset to play!");
    }
  }
  
  // Show on LCD for correct input
  lcd1.setCursor(0,0);
  lcd1.print("--- CORRECT ----");
  for (int i=0; i < 6; i++) {
    lcd1.scrollDisplayLeft();
    wait(100);
  }
  for (int i=0; i < 6; i++) {
    lcd1.scrollDisplayRight();
    wait(100);
  }
  lcd1.clear();
}
