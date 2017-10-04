//#include <stdlib.h>			//defines standard types
#include <Arduino.h> //needed for Serial.println
//#include <string.h> //needed for memcpy

//#include "MC_DL.h"
//#include "FT_MC_FT8XX.h"
//#include <SPI.h>





#ifndef _CANBUS_H_
#define  _CANBUS_H_

#include <ProjectMain.h>
#include <mcp_can.h>

#define CS_CAN0 11
#define INT_CAN0 12
#define CAN0baud CAN_1000KBPS
//#define CAN0baud CAN_125KBPS

struct canmsg {
	uint32_t timestamp;
	uint32_t msgId;
	uint8_t len;
	uint8_t msg[8];
	};

struct canMsgBuffer {
	uint8_t	idx=0;
	canmsg id[30];
	} ;
	
struct canmsg0x200 {
	uint16_t packVoltage;
	int16_t packCurrent;
	uint8_t qom:4;
	uint8_t pdetect:1;
	uint8_t chargerDetect :1;
	uint8_t buzzerStatus:1;
	uint8_t relayStatus:1;
	uint8_t SOC;
	int8_t maxTemp;
	int8_t minTemp;
};


union canID0x201{
	uint8_t msg[8];
	struct  {
		uint16_t maxCellVoltage;				// int number = buf[0] | buf[1] << 8;
		uint16_t minCellVoltage;					// return ((byte[0]<<24)|(byte[1]<<16)|(byte[2]<<8)|(byte[3]));
		uint16_t bVoltage;
		uint16_t FWVersion;
	};
};
struct canmsg0x202 {
	byte faultFlag1;
	byte faultFlag2;
	byte warningFlag1;
	byte warningFlag2;
	byte masterError1;
	byte masterError2;
	uint16_t Pvoltage;
};

struct canmsg0x203 {
	uint8_t reserve[7];
	uint8_t cfgSlaveCount;
};



void InitCAN(void);

bool readCAN();
void sendCANStop(void);

#endif // _canbus_h_
