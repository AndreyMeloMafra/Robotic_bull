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

//  Led
#define LedUltrasonic1 2
#define LedUltrasonic2 0
#define LedInfra 1

//  Servo pin
#define pinoServo 11

//  Sharp pin
#define pinoAnalogico A2
#define model 20150

//  Global variables 
Servo serv; 
int pos; 

bool senseServo = true;

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
  serv.write(0);

  //Initial state for pins
  pinMode(LedUltrasonic1, OUTPUT);
  pinMode(LedUltrasonic2, OUTPUT);
  pinMode(LedInfra, OUTPUT);

  //Initial values for leds
  digitalWrite(LedUltrasonic1, LOW);
  digitalWrite(LedUltrasonic2, LOW);
  digitalWrite(LedInfra, HIGH);
}

void loop() {
  float cmMsec, cmMsec2;

  time = millis();
  Serial.println(time);

  long microsec = ultrasonic1.timing();
  cmMsec = ultrasonic1.convert(microsec, Ultrasonic::CM);

//  long microsec2 = ultrasonic2.timing();
//  cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM);

  Serial.print("Sensor 01: ");
  Serial.print(cmMsec);
  Serial.println("cm");
//  Serial.print("Sensor 02: " + cmMsec2 + "cm");
  
//  Control Led 1
  digitalWrite(LedUltrasonic1, ledOn(cmMsec));

//  Control LED 2
//  digitalWrite(LedUltrasonic2, ledOn(cmMsec2));

  if(time >= (time + 1000)){
    //  Control Servo
    controlServo(senseServo);
      
    //Control infra
    dis = infraCode();
    Serial.print(dis);
    Serial.println("cm");
    
    //  digitalWrite(LedInfra, ledOn(dis);
  }
  
}

bool ledOn (double dis) {
  bool led = false;
  
  if(dis <= 30){
    led= true;
  }

  return led;
}

void controlServo(bool sense) {
  if(sense) {
      for(pos = 6; pos < 180; pos++){
        if(time >= 15){
          serv.write(pos);
        } 
      }
  } else {
      for(pos = 180; pos >= 6; pos--){ 
        if(time >= 15){
          serv.write(pos);
        } 
      }
  }
}

double infraCode() {
  double valorVolts = analogRead(pinoAnalogico) * 0.0048828125;
  double distancia = 4800/(valorVolts*200 - 20 );

  return distancia;
}
