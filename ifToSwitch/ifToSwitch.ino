#include <IRremote.h>
#define RECEIVER_PIN 12   //設定接收端輸出連接Arduino的腳位
#define A_1A 3
#define A_1B 5
#define B_1A 6
#define B_1B 9
int Speed=150;   //設定前進速度初始值
void setup() {
    pinMode(A_1A,OUTPUT);
    pinMode(A_1B,OUTPUT);
    pinMode(B_1A,OUTPUT);
    pinMode(B_1B,OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(A_1A,LOW);
    digitalWrite(A_1B,LOW);
    digitalWrite(B_1A,LOW);
    digitalWrite(B_1B,LOW);
    digitalWrite(LED_BUILTIN,LOW);
    IrReceiver.begin(RECEIVER_PIN);
}
//車子停止
void stopcar(){
    digitalWrite(A_1A,LOW);
    digitalWrite(A_1B,LOW);
    digitalWrite(B_1A,LOW);
    digitalWrite(B_1B,LOW);
}
//車子向前走
void forward(){
    digitalWrite(A_1A, LOW);
    analogWrite(A_1B, Speed);
    digitalWrite(B_1A, LOW);
    analogWrite(B_1B, Speed);
}
//車子向後走
void backward(){
    digitalWrite(A_1A, Speed);
    analogWrite(A_1B, LOW);
    digitalWrite(B_1A, Speed);
    analogWrite(B_1B, LOW);
}
//車子右轉
void turnRight(){
    digitalWrite(A_1A, Speed);
    analogWrite(A_1B, LOW);
    digitalWrite(B_1A, LOW);
    analogWrite(B_1B, Speed);
}
//車子左轉
void turnLeft(){
    digitalWrite(A_1A, LOW);
    analogWrite(A_1B, Speed);
    digitalWrite(B_1A, Speed);
    analogWrite(B_1B,LOW);
}
void loop() {
    if(IrReceiver.decode()){  
        switch(IrReceiver.decodedIRData.command){
            case 24:
                forward();
                delay(100);
                break;
            case 82:
                backward();
                delay(100);
                break;
            case 8:
                turnLeft();
                delay(100);
                break;
            case 90:
                turnRight();
                delay(100); 
                break; 
            default:
                stopcar();
                delay(100);           
        }
     IrReceiver.resume();
    }
}