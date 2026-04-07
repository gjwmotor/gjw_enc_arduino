#ifndef TMG_ENC_H
#define TMG_ENC_H

#include "Arduino.h"

class TmgEncClass{
public:
  uint8_t SF;//多摩川SF状态字
  int32_t ABS;//多摩川单圈磁编码值
  uint8_t ENID;//ENID
  int32_t ABM;//多摩川多圈编码值，有效位数16位
  int8_t ALMC;//
  uint16_t HDIDL;
  uint16_t HDIDH;
  uint16_t SFIDL;
  uint16_t SFIDH;
  uint32_t timeOut = 1000;
  HardwareSerial *pSerial;//串口指针
public:
  boolean tmgDecode_Ack(uint8_t cf, uint8_t *ackBuf, uint8_t ackLen, uint32_t ackT);
  boolean tmgRequest_1A();
  boolean tmgRequest_02();
  boolean tmgRequest_92();
  boolean tmgRequest_4A();
  boolean tmgRequest_C2();
  boolean tmgRequest_62();
  boolean tmgWriteConf(uint8_t Addr, uint8_t Dat);
  boolean tmgWriteEprom(uint8_t Addr, uint8_t Dat);
  uint8_t tmgReadEprom(uint8_t Addr);
  boolean tmgRegUnlock(uint8_t Cmd);
  boolean tmgWriteReg(uint8_t Cmd, uint8_t Addr, uint8_t Dat);
  uint8_t tmgReadReg(uint8_t Cmd, uint8_t Addr);
  uint8_t getLastError() {return err;}
private:
  uint8_t crc8_xor(uint8_t Poly, uint8_t Dat);
  uint8_t crc8_xor_list(uint8_t Poly, uint8_t *pDat, uint8_t nLen);
private:
  uint8_t err = 0;
};

#endif
