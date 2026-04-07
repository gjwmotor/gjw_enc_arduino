#include "ModbusEnc.h"

ModbusEnc mdenc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  //Serial1.begin(2500000, SERIAL_8N1, 18, 17);//esp32-s3
  mdenc.begin(&Serial1);
  mdenc.setTimeOut(100);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t Angle;
  int32_t Cicle;
  mdenc.readAngleCicle(1, &Angle, &Cicle);
  if(!mdenc.getLastError()){
    Serial.print("Angle Cicle:");
    Serial.print(Angle);
    Serial.print(',');
    Serial.println(Cicle);
    delay(10);
  }else{
    Serial.print("last error:");
    Serial.println(mdenc.getLastError());
    delay(2000);
  }

  Angle = mdenc.readAngle(1);
  Cicle = mdenc.readCicle(1);
  int32_t Speed = mdenc.readSpeed(1);
  int32_t Status = mdenc.readStatus(1);
  if(!mdenc.getLastError()){
    Serial.print("Angle:");
    Serial.print(Angle);
    Serial.print(" Cicle:");
    Serial.print(Cicle);
    Serial.print(" Speed:");
    Serial.print(Speed);
    Serial.print(" Status:");
    Serial.println(Status);
    delay(10);
  }else{
    Serial.print("last error:");
    Serial.println(mdenc.getLastError());
    delay(2000);
  }
}
