#ifndef _MODBUS_ENC_H
#define _MODBUS_ENC_H

#define MDENC_INFO_SN0 (0*64+7)

#define MDENC_ADDR_SPEC (11*64)

#define MDENC_STATUS_ANGLEH (14*64+0)
#define MDENC_STATUS_CICLEH (14*64+2)
#define MDENC_STATUS_STATUS (14*64+4)
#define MDENC_STATUS_SPEED (14*64+5)

#define MDENC_CTL_RST (15*64+1)
#define MDENC_CTL_LOCK (15*64+2)
#define MDENC_CTL_REGOPT (15*64+4)
#define MDENC_CTL_REGCMD (15*64+5)
#define MDENC_CTL_REGADDR (15*64+6)
#define MDENC_CTL_REGDAT (15*64+7)

#define MDENC_SETUP_ID (16*64+6)
#define MDENC_SETUP_BAUD (16*64+7)

#define MDENC_DEF_SN0 (17*64+4)

#define MDENC_ERR_CRC (1<<0)
#define MDENC_ERR_MTLOW (1<<1)
#define MDENC_ERR_MTHIGH (1<<2)
#define MDENC_ERR_SPEED (1<<3)
#define MDENC_ERR_POWVOL (1<<4)
#define MDENC_ERR_BATVOL (1<<5)
#define MDENC_ERR_TMP (1<<6)
#define MDENC_ERR_CHANG (1<<7)

#define MDENC_OPT_RCMD (1)
#define MDENC_OPT_WCMD (2)
#define MDENC_REG_RCMD (0b00110000)
#define MDENC_REG_WCMD (0b01100000)
#define MDENC_REG_FCMD (0b11000000)
#define MDENC_REG_ZCMD (0b01010000)

#include "Modbus.h"

class ModbusEnc : public Modbus
{
public:
	ModbusEnc();
	int32_t readAngle(uint8_t masId);
	int32_t readCicle(uint8_t masId);
	uint8_t readAngleCicle(uint8_t ID, int32_t *Angle, int32_t *Cicle);
	int16_t readSpeed(uint8_t masId);
	uint16_t readStatus(uint8_t masId);
	uint8_t readMtReg(uint8_t masId, uint16_t regAddr);
	uint8_t writeMtReg(uint8_t masId, uint16_t regAddr, uint16_t regDat);
	uint8_t flushMtReg(uint8_t masId);
	int16_t readABZ(uint8_t masId);
	uint8_t writeABZ(uint8_t masId, uint16_t abzVal);
	uint8_t unLockMem(uint8_t masId, uint16_t wAddr, uint16_t Key);
	uint8_t LockMem(uint8_t masId);

private:
};

#endif