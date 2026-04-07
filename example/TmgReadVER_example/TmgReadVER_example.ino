#include "TmgEnc.h"

TmgEncClass tmgEnc;

void setup() {
  //put your setup code here, to run once:
  Serial.begin(115200);//波特率115200，用于结果输出
  //Serial1.begin(2500000, SERIAL_8N1, 18, 17);//esp32-s3
  Serial1.begin(2500000);//多摩川波特率2.5M
  tmgEnc.pSerial = &Serial1;
}

void loop() {
  //put your main code here, to run repeatedly: 
  if(tmgEnc.tmgRequest_92()){
     //输出解码结果
     Serial.print(tmgEnc.HDIDL);
     Serial.print(',');
     Serial.print(tmgEnc.HDIDH);
     Serial.print(',');
     Serial.print(tmgEnc.SFIDL);
     Serial.print(',');
     Serial.println(tmgEnc.SFIDH);
  }
  //延时100ms
  delay(100);
}

