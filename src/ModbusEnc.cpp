#include "ModbusEnc.h"

ModbusEnc::ModbusEnc()
{
}

int32_t ModbusEnc::readAngle(uint8_t ID)
{
	uint16_t wDat[2];
	readReg(ID, MDENC_SIG_ANGLE_H, wDat, 2);
	return ((wDat[0]<<16)|wDat[1]);
}

int32_t ModbusEnc::readCicle(uint8_t ID)
{
	uint16_t wDat[2];
	readReg(ID, MDENC_MUL_CICLE_H, wDat, 2);
	return ((wDat[0]<<16)|wDat[1]);
}

uint8_t ModbusEnc::readAngleCicle(uint8_t ID, int32_t *Angle, int32_t *Cicle)
{
	uint16_t wDat[4];
	uint8_t lastErr = readReg(ID, MDENC_SIG_ANGLE_H, wDat, 4);
	*Angle = ((wDat[0]<<16)|wDat[1]);
	*Cicle = ((wDat[2]<<16)|wDat[3]);
	return lastErr;
}

int16_t ModbusEnc::readSpeed(uint8_t ID)
{
	return readReg(ID, MDENC_SPEED);
}

uint16_t ModbusEnc::readStatus(uint8_t ID)
{
	return readReg(ID, MDENC_STATUS);
}
