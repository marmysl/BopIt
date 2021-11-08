// variables
int sensorVal = 0;

void setup() {
  // Tilt switch
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  // Tilt switch output LED for testing
  pinMode(13, OUTPUT);
}

void TiltSwitchTest() {
  int digitalVal = digitalRead(2);//Read the value of pin2
  if( LOW == digitalVal)//if tilt switch is not breakover
  {
    digitalWrite(13,LOW);//turn the led off
  }
  else //if tilt switch breakover
  {
    digitalWrite(13,HIGH);//turn the led on 
  }
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
}
