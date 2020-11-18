#include "config.h"
#include "drive.h"
#include "sensors.h"

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BPWM, OUTPUT);
  
  pinMode(LIGHT_LEFT, INPUT);
  pinMode(LIGHT_RIGHT, INPUT);
  
  pinMode(WRIST_PIN, OUTPUT);
  pinMode(GRIPPER_PIN, OUTPUT);
  
  Serial.println("Finding Sensor");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Sensor Found!!");
  delay(1000);
}

void loop() {
  //Serial.println(readFrontUS());
  //gripperSetPosition();
  driveInCorridorUntilIntersection();
  stopMotors();
  delay(1000);
  gripperGrasp();
  delay(1000);
  /*unsigned long time = millis();
  while (millis() - time < 3*1000){
    driveInCorridor();
  }
  
  stopMotors();
  delay(1000);
  gripperGrasp();
  delay(10000);*/
}

void driveInCorridorUntilIntersection(){
  double distToLeft = readLeftUS();
  double distToRight = readRightUS();

  while (max(distToLeft, distToRight) < WALL_DIST) {
    float baseSpeed = 20;
    float turnAmount = 50;
    float leftMotorSpeed = (distToRight / (distToRight+distToLeft))*turnAmount + baseSpeed;
    float rightMotorSpeed = (distToLeft / (distToRight+distToLeft))*turnAmount + baseSpeed;
  
    moveL(leftMotorSpeed,FWD);
    moveR(rightMotorSpeed,FWD); 
    
    distToLeft = readLeftUS();
    distToRight = readRightUS();
  }
}
