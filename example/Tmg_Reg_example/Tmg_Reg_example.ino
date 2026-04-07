#include "TmgEnc.h"

TmgEncClass tmgEnc;

#define ABZ_SET (16384)

void setup() {
  //put your setup code here, to run once:
  Serial.begin(115200);//波特率115200，用于结果输出
  //Serial1.begin(2500000, SERIAL_8N1, 18, 17);//esp32-s3
  Serial1.begin(2500000);//多摩川波特率2.5M
  tmgEnc.pSerial = &Serial1;
  delay(1000);
  #ifdef CONF_ABZ
  tmgEnc.tmgRegUnlock(0x60);
  tmgEnc.tmgWriteReg(0x60, 0x07, ((ABZ_SET-1)<<2)>>8);
  tmgEnc.tmgRegUnlock(0x60);
  tmgEnc.tmgWriteReg(0x60, 0x08, (((ABZ_SET-1)<<2)&0xff));
  tmgEnc.tmgRegUnlock(0xC0);
  uint8_t rDat = tmgEnc.tmgReadReg(0xC0, 0x00);
  if(!tmgEnc.getLastError()){
    Serial.print("flush:");
    Serial.println(rDat, HEX);
  }
  #endif
}

void loop() {
  //put your main code here, to run repeatedly: 
  tmgEnc.tmgRegUnlock(0x30);
  uint8_t rDat = tmgEnc.tmgReadReg(0x30, 0x07);
  if(!tmgEnc.getLastError()){
    Serial.print("abz(07):");
    Serial.println(rDat, HEX);
  }
  tmgEnc.tmgRegUnlock(0x30);
  rDat = tmgEnc.tmgReadReg(0x30, 0x08);
  if(!tmgEnc.getLastError()){
    Serial.print("abz(08):");
    Serial.println(rDat, HEX);
  }
  delay(300);
}

