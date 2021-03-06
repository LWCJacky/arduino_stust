#include <Arduino.h>
#include <EEPROM.h>
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define PWMA 10
#define PWMB 11
#define address 2
#define IR_INPUT_PIN 4

#include "TinyIRReceiver.hpp"


#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif

volatile struct TinyIRReceiverCallbackDataStruct sCallbackData;
unsigned long myTime;
int speed=0;
void setup() {
  myTime = millis();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("START " __FILE__ " from " __DATE__));
  initPCIInterruptForTinyReceiver();
  Serial.println(F("Ready to receive NEC IR signals at pin " STR(IR_INPUT_PIN)));
  speed=EEPROM.read(address);
  analogWrite(PWMA,speed);
  analogWrite(PWMB,speed);
  Serial.println(speed);
}

void loop() {
  if (sCallbackData.justWritten) {
    sCallbackData.justWritten = false;
    Serial.print(F("Address=0x"));
    Serial.print(sCallbackData.Address, HEX);
    Serial.print(F(" Command=0x"));
    Serial.print(sCallbackData.Command, HEX);
    if (sCallbackData.isRepeat) {
      Serial.print(F(" Repeat"));
    }
    Serial.println();
  }
}

void upon() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, HIGH);
  delay(150);
  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, !HIGH);
}
void dome() {

  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, !HIGH);
  delay(150);
  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, !HIGH);
}
void Left_handed() {

  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, HIGH);
  delay(150);
  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, !HIGH);
}
void right_handed() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, !HIGH);
  delay(150);
  digitalWrite(IN1, !HIGH);
  digitalWrite(IN2, !HIGH);
  digitalWrite(IN3, !HIGH);
  digitalWrite(IN4, !HIGH);
}
void accelerate(){
  speed+=10;
  if(speed>=255){
    speed=255;
  }
  EEPROM.write(address,speed);
  analogWrite(PWMA,speed);
  analogWrite(PWMB,speed);
  Serial.println(speed);
}
void slow_down(){
  speed-=10;
  if(speed<=0){
    speed=0;
  }
  EEPROM.write(address,speed);
  analogWrite(PWMA,speed);
  analogWrite(PWMB,speed);
  Serial.println(speed);
}
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) {
  Serial.print(F("A=0x"));
  Serial.print(aAddress, HEX);
  Serial.print(F(" C=0x"));
  Serial.print(aCommand, HEX);
  Serial.print(F(" R="));
  Serial.print(isRepeat);
  Serial.println();
  switch (aCommand) {
    case 0x18:
      upon();
      break;
    case 0x52:
      dome();
      break;
    case 0x8:
      Left_handed();
      break;
    case 0x5A:
      right_handed();
      break;
    case 0x15:  //??????
      accelerate();
      break;
    case 0x7:  //??????
      slow_down();
      break;  
    default:
      break;
  }
}