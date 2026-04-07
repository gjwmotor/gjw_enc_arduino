#include "TmgEnc.h"

//CRC校验
uint8_t TmgEncClass::crc8_xor(uint8_t Poly, uint8_t Dat)
{
  uint8_t crc8 = Dat;
  for(uint8_t i=0; i<8; i++){
    if(crc8&0x80){
      crc8 = (crc8<<1)^Poly;
    }else{
      crc8 <<= 1;
    }
  }
  return crc8;
}

//多个字节CRC校验
uint8_t TmgEncClass::crc8_xor_list(uint8_t Poly, uint8_t *pDat, uint8_t nLen)
{
  uint8_t crc8 = 0;
  while(nLen--){
    crc8 = crc8^*pDat++;
    crc8 = crc8_xor(Poly, crc8);
  }
  return crc8;
}

boolean TmgEncClass::tmgRequest_1A()
{
  uint8_t tmgbuf[11];
  pSerial->write(0x1A);
  tmgDecode_Ack(0x1A, tmgbuf, 11, timeOut);
  if(!err){
    SF = tmgbuf[1];//解码SF
    ABS = tmgbuf[2]|(tmgbuf[3]<<8)|(tmgbuf[4]<<16);//解码ABS
    ENID = tmgbuf[5];//解码ENID
    ABM = int16_t(tmgbuf[6]|(tmgbuf[7]<<8));//解码ABM
    ALMC = tmgbuf[9];//解码ALMC
  }
  return (err==0);
}

boolean TmgEncClass::tmgRequest_02()
{
  uint8_t tmgbuf[11];
  pSerial->write(0x02);
  tmgDecode_Ack(0x02, tmgbuf, 6, timeOut);
  if(!err){
    SF = tmgbuf[1];//解码SF
    ABS = tmgbuf[2]|(tmgbuf[3]<<8)|(tmgbuf[4]<<16);//解码ABS
  }
  return (err==0);
}

boolean TmgEncClass::tmgRequest_92()
{
  uint8_t tmgbuf[10];
  pSerial->write(0x92);
  tmgDecode_Ack(0x92, tmgbuf, 10, timeOut);
  if(!err){
    HDIDL = tmgbuf[1]|(tmgbuf[2]<<8);
    HDIDH = tmgbuf[3]|(tmgbuf[4]<<8);
    SFIDL = tmgbuf[5]|(tmgbuf[6]<<8);
    SFIDH = tmgbuf[7]|(tmgbuf[8]<<8);
  }
  return (err==0);
}

boolean TmgEncClass::tmgRequest_4A()
{
  uint8_t tmgbuf[3];
  pSerial->write(0x4A);
  tmgDecode_Ack(0x4A, tmgbuf, 3, timeOut);
  if(!err){
    SF = tmgbuf[1];//解码SF
  }
  return (err==0);
}

boolean TmgEncClass::tmgRequest_62()
{
  uint8_t tmgbuf[6];
  for(uint8_t i=0; i<10; i++){
    pSerial->write(0x62);
    tmgDecode_Ack(0x62, tmgbuf, 6, timeOut);
    if(err){
      break;
    }
  }
  if(!err){
    SF = tmgbuf[1];//解码SF
    ABS = tmgbuf[2]|(tmgbuf[3]<<8)|(tmgbuf[4]<<16);//解码ABS
  }
  return (err==0);
}

boolean TmgEncClass::tmgRequest_C2()
{
  uint8_t tmgbuf[6];
  for(uint8_t i=0; i<10; i++){
    pSerial->write(0xC2);
    tmgDecode_Ack(0xC2, tmgbuf, 6, timeOut);
    if(err){
      break;
    }
  }
  if(!err){
    SF = tmgbuf[1];//解码SF
    ABS = tmgbuf[2]|(tmgbuf[3]<<8)|(tmgbuf[4]<<16);//解码ABS
  }
  return (err==0);
}

boolean TmgEncClass::tmgWriteConf(uint8_t Addr, uint8_t Dat)
{
  uint8_t buf[2];
  buf[0] = Addr;
  buf[1] = ~Addr;
  boolean res1 = tmgWriteEprom(0x40, Addr);
  boolean res2 = tmgWriteEprom(0x41, ~Addr);
  boolean res3 = tmgWriteEprom(0x42, crc8_xor_list(0x07, buf, 2));
  boolean res4 = tmgWriteEprom(Addr, Dat);
  return res1&&res2&&res3&&res4;
}

boolean TmgEncClass::tmgRegUnlock(uint8_t Cmd)
{
  boolean res1 = tmgWriteEprom(0x3D, Cmd);
  boolean res2 = tmgWriteEprom(0x3E, ~Cmd);
  boolean res3 = tmgWriteEprom(0x3F, (Cmd<<4)|(Cmd>>4));
  return res1&&res2&&res3;
}

boolean TmgEncClass::tmgWriteReg(uint8_t Cmd, uint8_t Addr, uint8_t Dat)
{
  uint8_t txBuf[5];
  txBuf[0] = 0x2A;
  txBuf[1] = Cmd;
  txBuf[2] = Addr;
  txBuf[3] = Dat;
  txBuf[4] = crc8_xor_list(0x01, txBuf, 4);
  pSerial->write(txBuf[0]);
  pSerial->flush();
  delayMicroseconds(10);
  pSerial->write(txBuf+1, 4);
  return tmgDecode_Ack(0x2A, NULL, 5, timeOut);
}

uint8_t TmgEncClass::tmgReadReg(uint8_t Cmd, uint8_t Addr)
{
  uint8_t txBuf[4];
  uint8_t rxBuf[5];
  txBuf[0] = 0xA2;
  txBuf[1] = Cmd;
  txBuf[2] = Addr;
  txBuf[3] = crc8_xor_list(0x01, txBuf, 3);
  pSerial->write(txBuf[0]);
  pSerial->flush();
  delayMicroseconds(10);
  pSerial->write(txBuf+1, 3);
  tmgDecode_Ack(0xA2, rxBuf, 5, timeOut);
  if(rxBuf[1]!=Cmd && rxBuf[2]!=Addr){
    err = 1;
  }
  return rxBuf[3];
}

boolean TmgEncClass::tmgWriteEprom(uint8_t Addr, uint8_t Dat)
{
  uint8_t txBuf[4];
  txBuf[0] = 0x32;
  txBuf[1] = Addr;
  txBuf[2] = Dat;
  txBuf[3] = crc8_xor_list(0x01, txBuf, 3);
  pSerial->write(txBuf[0]);
  pSerial->flush();
  delayMicroseconds(10);
  pSerial->write(txBuf+1, 3);
  return tmgDecode_Ack(0x32, NULL, 4, 10000);
}

uint8_t TmgEncClass::tmgReadEprom(uint8_t Addr)
{
  uint8_t txBuf[3];
  uint8_t rxBuf[4];
  txBuf[0] = 0xEA;
  txBuf[1] = Addr;
  txBuf[2] = crc8_xor_list(0x01, txBuf, 2);
  pSerial->write(txBuf[0]);
  pSerial->flush();
  delayMicroseconds(10);
  pSerial->write(txBuf+1, 2);
  tmgDecode_Ack(0xEA, rxBuf, 4, timeOut);
  if(rxBuf[1]!=Addr){
    err = 1;
  }
  return rxBuf[2];
}

boolean TmgEncClass::tmgDecode_Ack(uint8_t cf, uint8_t *ackBuf, uint8_t ackLen, uint32_t ackT)
{
  uint32_t t_beg = micros();
  uint8_t tmgn = 0;
  while(1){
    //超时处理，超出100us没有接收完整数据，接收超时退出解码
    uint32_t t_usr = micros() - t_beg;
    if(t_usr>ackT){
	  err = 1;
      return false;
    }
    while(pSerial->available()){
      t_beg = micros();
	  uint8_t _Dat = pSerial->read();
	  //Serial.print(_Dat, HEX);
	  if(ackBuf){
	    ackBuf[tmgn] = _Dat;
	  }
      //过滤无用数据
      if((_Dat!=cf) && (tmgn==0)){
        continue;
      }
      //Serial.print(_Dat, HEX);
      tmgn++;
      if(tmgn==ackLen){//读ackLen字节开始解码
        //Serial.println();
		if(ackBuf){
          if(ackBuf[ackLen-1]==crc8_xor_list(0x01, ackBuf, ackLen-1)){//CRC校验数据完整性
		    err = 0;
		  }else{
		    err = 1;
		  }
		}else{
		  err = 0;
		}
		return true;
      }
    }
  }
}
