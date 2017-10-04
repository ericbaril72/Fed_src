/*
 * ProjectMain.h
 *
 * Created: 2016-08-08 10:32:22
 *  Author: EricB
 */ 


#ifndef _PROJECTMAIN_H_
#define _PROJECTMAIN_H_

#include <avr/dtostrf.h>
#include <stdlib.h>


//#define Serial1 SerialUSB

#include <Arduino.h>
#include <Wire.h>				// for I2C interface
#include "RTClib.h"				// for PCF rtc mngmt
#include <RTCZero.h>
#include <SdFat.h>
#include "display.h"
#include "CANbus.h"
#include <Adafruit_WINC1500.h>
#include <wifi.h>
#include <arduinoJson.h>
#include "OneWire.h"
#include <initSystem.h>
//#include <JsonStreamingParser.h>


#define ENABLED 1
#define DISABLED 0

#define pinV_Batt  0						//	A0
#define pinV_Current_sensor 1				//    A1
#define pinV_Current_sensor 2				//    A2
#define pinV_ksi							// a3
											//	A3  used for display !!!
											// a4 AND a5   FOR DISPLAY
											

#define pinSpeed 6				// mapped on pin 6   or CAN1 ???
#define d18b20	5


//#define pinV_Charger2  3				
#define VBACKUP A7
#define pinV_Backup  9				//	A7		( reserved pin on Feather M0 )
#define Vd50k  (58 / 3.3) * (3.3 / 1024)	//  3.3k / 58k voltage divider on all other Analog inputs
#define Vd21_33k ( 32.6 / 21.6) * (4.95 / 1024)	// Current Sensor
#define Vd12_228k ( 221 / 11.9) * (3.3 / 1024)	// Voltagedivider Vbatt
#define VbackupVD  2 * (3.3 / 1024)		//  50% voltage divider on Vbackup
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC and comment this out

#define SD_CHIP_SELECT 10

		// count of the # of items in DL_fct   and #defines
#define DL_LCD 0
#define DL_SD 1
#define DL_CAN 2
#define DL_RTC 3
#define DL_Wifi 4
#define DL_SERVER 5
#define DL_SPEED 6

#define FCT_DISABLE 0
#define FCT_ACTIVE 1
#define FCT_WARNING 2
#define FCT_ERROR 3
#define FCT_FAILUR

#define V_NOBATT 7
#define V_LOW_BACKUP_BATT	6
#define V_CHARGER1		3
#define V_CHARGER2
#define V_QUICKCHARGE
#define V_OFF 1
#define V_ON 2


typedef struct refreshTimeout {
	long gps;
	long lcd;
	long statuspost;
	long serialdump;
	long checkServer;
	};
	
struct V_status {
	float calibA0=float(0.06085);
	float calibA1=float(Vd12_228k);
	float calibA2=float(Vd21_33k);
	float calibA3=float(0.061624);
	float V_Batt = 0;
	float Current = 0;
	float A_CurrentA =0;
	float V_Backup = 41.0;
	float A_CurrentB =0;
	float V_ksi = 40.3;
	float rpmratio = WheelDiameter / MotorWheelRatio * 0.287267904 ;		// (km/in-hr)
	float speed=0.0;
	float power=0;
	float EnergyIN=0;	// Watts per second
	float EnergyOUT=0;
	float TempS1=0;
	float TempS2=0;
	char type[10]="DConly";
	uint16_t A0=0;
	uint16_t A1=0;
	uint16_t A2=0;
	uint16_t A3=0;
	
	uint16_t A_CurrentA_Avg =0;
	uint16_t A_CurrentB_Avg =0;
	uint8_t oversampling=0;
	uint16_t V_Batt_Avg = 33.3;
	
	uint16_t V_Backup_Avg = 42.1;
	uint16_t V_Charger1_Avg = 20.1;
	uint16_t V_Charger2_Avg = 30.1;
	uint16_t V_ksi_Avg = 50.7;
	long int powerOut=0;
	long int powerIn=0;
	long int lastIntegralTS=0;
	char oldstate = 0;
	char state = 0;
	bool charger_detect=0;
	uint16_t offsetA1=520;
	uint16_t offsetA2=527;
	uint16_t TempS_idx=0;
	uint8_t SOC = 0;
	char Name[40] = "Hybrid01";
	uint32_t lastfilesize=0;
	int WheelDiameter = 16;
	int MotorWheelRatio = 16.99;
	int cancnt=0;
	int rpmcnt = 0;
	int rpm =0;
	byte mac[7]={0};
	char id[13]={0};
	char time[15];
	char timeM0[15];	
	uint32 lastcanpckt=0;
	bool cantrfic=false;
	bool displayOn=true;
	bool lastPost=0;
	char fwrevCA[40];
	int wifiTimeout=0;
	int servernoack=0;
	bool fileupdate=true;
};
struct {
	bool valid=false;
	char GGA[128];
	char VTG[64];
} gps;
extern V_status Vehicle;
extern uint32_t refreshlcdtimeout;
/*
typdef struct schedule_info {
	long nextCall=0;
	int period;
};
*/

typedef struct DL_fcts {
	uint16_t idx;
	char name[10];
	uint16_t active;
	uint16_t status;
	uint16_t options;
	//uint16_t w = 50;
	//uint16_t h = 50;
} DL_fct[7];

//DL_fct[0] =		{ DL_LCD,   "LCD"		,  DISABLED, 0,0 };
//DL_fct[1] =	{ DL_SD,   "SDCard"		,  ENABLED, 0,0 };
//DL_fct[2] =	{ DL_CAN, "CAN"			,  ENABLED, 0,0 };
//DL_fct[3] =	{ DL_RTC,   "RTC"		,  ENABLED, 0,0 };
//DL_fct[4] =	{ DL_Wifi,  "WIFI"		,  ENABLED, 0,0 };
//DL_fct[5] =	{ DL_SERVER,  "Server"	,  ENABLED, 0,0 };
//DL_fct[6] =	{ DL_SPEED,   "Speed"	,  DISABLED, 0,0 };


struct spi_access {
	bool canpacketread=false;
	bool sdpacketwrite=false;
	bool rfpacketsend=false;
	bool lcdspiaccess=false;
	bool serverAvailable=false;
};
extern spi_access RW_spi;


struct statusDescriptors {
	char logextension[3];
	char desc[40];
} ;





//extern statusDescriptors StatusList1[];

void evalspeed(void);

void TC3_Handler();
void printtime();
void wakeISR();
void dingrpm();
void updateVehicleStatus();
void createLogFile();
void checkforupdate();
void prepStatusData();
void updateLogFile(char loopdata[]);
void ISRcan(void);
void updateGPSinfo();

//char *dtostrf(double value, int width, unsigned int precision, char *result);
void printTemp();
void dateTime(uint16_t* date, uint16_t* time);

bool updateCalibrationFactors();
void uploadSDtoServer(bool erase);
void initCalibration();


#endif /* PROJECTMAIN_H_ */