#include "TmgEnc.h"

TmgEncClass tmgEnc;

void setup() {
  //put your setup code here, to run once:
  Serial.begin(115200);//波特率115200，用于结果输出
  //Serial1.begin(2500000, SERIAL_8N1, 18, 17);//esp32-s3
  Serial1.begin(2500000);//多摩川波特率2.5M
  tmgEnc.pSerial = &Serial1;
  delay(1000);
  tmgEnc.tmgWriteConf(67, 0x0);
  tmgEnc.tmgWriteConf(68, 0x0);
}

void loop() {
  //put your main code here, to run repeatedly: 
  uint8_t setTmp = tmgEnc.tmgReadEprom(67);
  if(!tmgEnc.getLastError()){
    Serial.print("Tmp:");
    Serial.println(setTmp);
  }
  uint8_t setBaudr = tmgEnc.tmgReadEprom(68);
  if(!tmgEnc.getLastError()){
    Serial.print("Baud:");
    Serial.println(setBaudr);
  }
  delay(100);
}

