#ifndef _MODBUS_ENC_H
#define _MODBUS_ENC_H

#define MDENC_SIG_ANGLE_H (14*64+0)
#define MDENC_MUL_CICLE_H (14*64+2)
#define MDENC_STATUS (14*64+4)
#define MDENC_SPEED (14*64+5)

#define MDENC_ERR_CRC (1<<0)
#define MDENC_ERR_MTLOW (1<<1)
#define MDENC_ERR_MTHIGH (1<<2)
#define MDENC_ERR_SPEED (1<<3)
#define MDENC_ERR_POWVOL (1<<4)
#define MDENC_ERR_BATVOL (1<<5)
#define MDENC_ERR_TMP (1<<6)
#define MDENC_ERR_CHANG (1<<7)

#include "Modbus.h"

class ModbusEnc : public Modbus
{
public:
	ModbusEnc();
	int32_t readAngle(uint8_t ID);
	int32_t readCicle(uint8_t ID);
	uint8_t readAngleCicle(uint8_t ID, int32_t *Angle, int32_t *Cicle);
	int16_t readSpeed(uint8_t ID);
	uint16_t readStatus(uint8_t ID);
private:
};

#endif