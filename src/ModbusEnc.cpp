#include "ModbusEnc.h"

ModbusEnc::ModbusEnc()
{
}

int32_t ModbusEnc::readAngle(uint8_t masId)
{
	uint16_t wDat[2];
	readReg(masId, MDENC_STATUS_ANGLEH, wDat, 2);
	return ((wDat[0]<<16)|wDat[1]);
}

int32_t ModbusEnc::readCicle(uint8_t masId)
{
	uint16_t wDat[2];
	readReg(masId, MDENC_STATUS_CICLEH, wDat, 2);
	return ((wDat[0]<<16)|wDat[1]);
}

uint8_t ModbusEnc::readAngleCicle(uint8_t masId, int32_t *Angle, int32_t *Cicle)
{
	uint16_t wDat[4];
	uint8_t lastErr = readReg(masId, MDENC_STATUS_ANGLEH, wDat, 4);
	*Angle = ((wDat[0]<<16)|wDat[1]);
	*Cicle = ((wDat[2]<<16)|wDat[3]);
	return lastErr;
}

int16_t ModbusEnc::readSpeed(uint8_t masId)
{
	return readReg(masId, MDENC_STATUS_SPEED);
}

uint16_t ModbusEnc::readStatus(uint8_t masId)
{
	return readReg(masId, MDENC_STATUS_STATUS);
}

uint8_t ModbusEnc::unLockMem(uint8_t masId, uint16_t wAddr, uint16_t Key)
{
	uint16_t lkDat[2];
	lkDat[0] = wAddr/64;
	lkDat[1] = Key;
	return writeReg(masId, MDENC_CTL_LOCK, lkDat, 2);
}

uint8_t ModbusEnc::LockMem(uint8_t masId)
{
	uint16_t lkDat[2];
	lkDat[0] = 0;
	lkDat[1] = 0;
	return writeReg(ID, MDENC_CTL_LOCK, lkDat, 2);
}

uint8_t ModbusEnc::readMtReg(uint8_t masId, uint16_t regAddr)
{
	writeReg(masId, MDENC_CTL_REGOPT, MDENC_OPT_RCMD);
	writeReg(masId, MDENC_CTL_REGCMD, MDENC_REG_RCMD);
	writeReg(masId, MDENC_CTL_REGADDR, regAddr);
	unLockMem(masId, MDENC_CTL_RST, 0);
	return readReg(masId, MDENC_CTL_REGDAT);
}

uint8_t ModbusEnc::writeMtReg(uint8_t masId, uint16_t regAddr, uint16_t regDat)
{
	writeReg(masId, MDENC_CTL_REGOPT, MDENC_OPT_WCMD);
	writeReg(masId, MDENC_CTL_REGCMD, MDENC_REG_WCMD);
	writeReg(masId, MDENC_CTL_REGADDR, regAddr);
	writeReg(masId, MDENC_CTL_REGDAT, regDat);
	return unLockMem(masId, MDENC_CTL_RST, 0);	
}

uint8_t ModbusEnc::flushMtReg(uint8_t masId)
{
	writeReg(masId, MDENC_CTL_REGOPT, MDENC_OPT_RCMD);
	writeReg(masId, MDENC_CTL_REGCMD, MDENC_REG_FCMD);
	writeReg(masId, MDENC_CTL_REGADDR, 0x00);
	unLockMem(masId, MDENC_CTL_RST, 0);
	return readReg(masId, MDENC_CTL_REGDAT);
}

int16_t ModbusEnc::readABZ(uint8_t masId)
{
	uint8_t rDatH = readMtReg(masId, 0x07);	
	uint8_t rDatL = readMtReg(masId, 0x08);
	//Serial.println(rDatH, HEX);
	//Serial.println(rDatL, HEX);
	return (((rDatH<<8)|rDatL)>>2)+1;
}

uint8_t ModbusEnc::writeABZ(uint8_t masId, uint16_t abzVal)
{
	writeMtReg(masId, 0x07, abzVal>>8);
	writeMtReg(masId, 0x08, (abzVal&0xff));
	return (flushMtReg(masId)==0x55);
}
