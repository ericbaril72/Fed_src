
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define FT_CS_PIN 				19		//A5
#define FT_PDN_PIN				18		// A4
#define FT_INT_PIN 				17		// A3
#define FT_DISPENABLE_PIN		7		// Internal FT800 mapped PIN
#define FT_AUDIOENABLE_PIN		1		// Internal FT800 mapped PIN

#include <HOTMCU.h>
#include <ProjectMain.h>

//#include <SdFat.h>
//#define DL_FCTS (sizeof(DL_fct)	/ sizeof DL_fct[0])
#define DL_FCTS 7


struct menuBtns {
	uint16_t initXpos =5;
	uint16_t	Xpos= 5;
	uint16_t Ypos= 222;
	uint16_t gap=5;
	uint16_t w=75;
	uint16_t h=40;
	uint16_t font=26;
	uint16_t timeout=3;
	uint16_t whentoremove=1000;
	uint16_t speed=50;
};

struct display_screen {
	bool spinner=true;
	bool looprun=false;
	bool loadimage=true;
	uint16_t previousTag=0;
};

struct term_lcd {
	char LCDtext[10][40]={"","","","","","","","","",""};
	byte LCDtextIdx = 0;
	byte LCDtextTermIdx =0;
	byte LCDtextRollIdx =0;
	byte Termidx[9]={0,1,2,3,4,5,6,7,8};
};





void PrintLCD(const char Display_string[]);
void displayBackGPic();
void displayMain();
void InitLCD();
void RefreshLCD(int i);
void loadBGpic();
void Calibrate();
void displayMain();
void displayTouch();
void displayWifi();
void displaydebug();
void displaySOC();
void loadSOC(char color,char level);

#endif /*  _MC_DL_H_ */

