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
  int ID = mdenc.Ping(0);
  if(!mdenc.getLastError()){
    Serial.print("slave ID:");
    Serial.println(ID, DEC);
    delay(10);
  }else{
    Serial.print("last error:");
    Serial.println(mdenc.getLastError());
    delay(2000);
  }
}
