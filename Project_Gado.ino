//  Libraries import  
#include <SharpIR.h>
#include <Ultrasonic.h>
#include <Servo.h> 

//  Define ports on arduino
//    Ultrasonic sensor pin
//      Ultrasonic 1
#define pino_trigger 5
#define pino_echo 4

//      Ultrasonic 2
#define pino_trigger2 9
#define pino_echo2 8

//    Led
#define LedUltrasonic1 12
#define LedUltrasonic2 11
#define LedInfra 10

//    Servo pin
#define pinoServo 13

//    Sharp pin
#define pinoAnalogico A2
#define model 20150

//  Global variables 
Servo serv; 
int pos; 

bool servoFunc = true;

double dis;

//  Controller time
unsigned long time;

//  Initial state on pins
Ultrasonic ultrasonic1(pino_trigger, pino_echo);
Ultrasonic ultrasonic2(pino_trigger2, pino_echo2);


void setup() {

  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");

  serv.attach(pinoServo); 
  serv.write(6);

  //Initial state for pins
  pinMode(LedUltrasonic1, OUTPUT);
  pinMode(LedUltrasonic2, OUTPUT);
  pinMode(LedInfra, OUTPUT);

  //Initial values for leds
  digitalWrite(LedUltrasonic1, LOW);
  digitalWrite(LedUltrasonic2, LOW);
  digitalWrite(LedInfra, LOW);
}

void loop() {
  float cmMsec, cmMsec2;

  time = millis();

  long microsec = ultrasonic1.timing();
  cmMsec = ultrasonic1.convert(microsec, Ultrasonic::CM);

  long microsec2 = ultrasonic2.timing();
  cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM);

  Serial.println(cmMsec2);
  
//  Control Led 1
  digitalWrite(LedUltrasonic1, ledOn(cmMsec, false));

//  Control LED 2
  digitalWrite(LedUltrasonic2, ledOn(cmMsec2, false));

//  if(time >= 1000){
//  In development
//      Control Servo
//    if(servoFunc){
//      servoFunc = false;
//      controlServo();
//    }

    //Control infra
    dis = infraCode();
    Serial.print(dis);
    Serial.println("cm");

//    Control Led 3
    digitalWrite(LedInfra, ledOn(dis, true));
    time = 0;
//  }
}

bool ledOn (double dis, bool infra) {
  bool led = false;
  
  if(dis <= 50 && !infra){
    led = true;
  } else if(dis <= 20 && infra) {
    led = true;
  }

  return led;
}

double infraCode() {
  double valorVolts = analogRead(pinoAnalogico) * 0.0048828125;
  double distancia = 4800/(valorVolts*200 - 20 );

  return distancia;
}

//In development
//void controlServo() {
//    unsigned long count;
//    count = millis();
//    
//    for(pos = 6; pos < 180; pos++){
//      if(count >= 15){
//        serv.write(pos);
//        delay(15); 
//        count = 0;
//      } 
//    }
//      
//  if(time >= 2000){
//    for(pos = 180; pos >= 6; pos--){ 
//      if(time >= 15){
//        serv.write(6);
//      } 
//    }
//    servoFunc = true;
//  }
//}
