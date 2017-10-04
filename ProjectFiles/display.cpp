#include "display.h"
#include "ProjectMain.h"
#include <avr/pgmspace.h>
//#include "bgtest1.binh"

//extern SdFile file;		// change fct to pass pointer
//extern  V_status V_status;
//bool penCalibrated=false;
//extern char logStatus[] ;

extern spi_access RW_spi;
term_lcd LCDterminal;
//extern DL_fcts DL_fct[];
extern SdFat sd;
extern V_status Vehicle;
extern bool ledState;
extern uint32_t refreshlcdtimeout;
extern int refreshlcdtimeoutperiod;

extern RTCZero rtc;
extern Adafruit_WINC1500 WiFi;
extern HttpURL PostDataURL;
extern statusDescriptors StatusList[];

uint32_t statusColor[5]={0x646464,0x00aaff,0xffff7f,0xffaa00,0xff0000};
FT800IMPL_SPI FTImpl(FT_CS_PIN,FT_PDN_PIN,FT_INT_PIN);
menuBtns menuBtns;
display_screen display_screen;
float j=2.5;
bool dir=1;



int predefCos(float j){
	uint pos=10*j;
	int cosTable[]={3220,3214,3194,3162,3117,3061,2993,2914,2826,2728,2622,2510,2391,2268,2141,2012,1882,1753,1625,1500,1379,1264,1155,1054,961,879,806,745,695,658,633,621,622,636,663,703,754,817,
		892,976,1070,1173,1283,1399,1520,1646,1774,1904,2034,2162,2289,2411,2529,2641,2745,2841,2928,3005,3071,3126,3168,3198,3216};
	return cosTable[pos];
	
}
int predefSin(float j){
	uint pos=10*j;
	int sinTable[]={2560,2690,2818,2944,3066,3183,3294,3397,3493,3578,3654,3719,3772,3813,3841,3857,3859,3849,3826,3790,3742,3682,3611,3529,3438,3338,3230,3116,2995,2871,2743,2614,2484,2355,2228,
		2104,1985,1871,1765,1666,1576,1496,1427,1369,1323,1289,1268,1260,1265,1283,1313,1356,1412,1478,1555,1643,1739,1844,1956,2074,2197,2323,2452};
	return sinTable[pos];	
}

void displayMain(){
	char tempbuffer[60];
	FTImpl.Cmd_BGColor(0x575756);
	FTImpl.ColorRGB(0xFFFFFF);
	FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,24,31, FT_OPT_CENTER , "MOTREC");
	
	
	
	
	//Background colors
	// Menu Bar
	//FTImpl.ColorRGB();
	//FTImpl.Begin(FT_RECTS);
	//FTImpl.Vertex2f(0 * 16, 0 * 16);
	//FTImpl.Vertex2f(480 * 16, 47 * 16);
	
	
	//  ****************
	// Speedometer
	
		// BACKGROUND
		FTImpl.ColorRGB(0xE35104);
		FTImpl.Begin(FT_RECTS);
		FTImpl.Vertex2f(0 * 16, 48 * 16);
		FTImpl.Vertex2f(240 * 16, 272 * 16);
		FTImpl.ColorRGB(0xFFFFFF);
		FTImpl.Cmd_Gauge(120,160,100,20736,10,5,0,20);
		FTImpl.Cmd_Text(120,235,27,FT_OPT_CENTER,"MPH");
		
		/*
		FTImpl.ColorRGB(0xFFFFFF);
		FTImpl.Begin(FT_RECTS);
		FTImpl.Vertex2f(0 * 16, 48 * 16);
		FTImpl.Vertex2f(120 * 16, 272 * 16);
		FTImpl.BitmapHandle(0);
		FTImpl.BitmapSource(0);
		FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER,240, 224);
		FTImpl.BitmapLayout(FT_ARGB1555,480,224);
		FTImpl.Begin(FT_BITMAPS);
		FTImpl.Vertex2ii(0, 48);
	*/
		
		// Display Speed
		FTImpl.ColorRGB(0xFFFFFF);
		sprintf(tempbuffer,"%d",Vehicle.speed);
		//dtostrf(float(millis()%100)/10, 3, 1, tempbuffer);
		dtostrf(Vehicle.speed, 3, 1, tempbuffer);
		FTImpl.Cmd_Text(120,160,31,FT_OPT_CENTER,tempbuffer);
	
		
	
	//  ****************
	// SOC indicator
	//  ****************
		// BACKGROUND
		FTImpl.ColorRGB(0x15AE97);
		FTImpl.Begin(FT_RECTS);
		FTImpl.Vertex2f(240 * 16, 48 * 16);
		FTImpl.Vertex2f(480 * 16, 159 * 16);
		FTImpl.ColorRGB(0xFFFFFF);
		FTImpl.Cmd_Text(400,60,27,0,"Battery");
		FTImpl.Cmd_Text(400,75,27,0,"Level");
	
		// vALUES
		tempbuffer[0]='-';
		tempbuffer[1]='-';
		tempbuffer[2]=0;
		if (Vehicle.cantrfic) dtostrf(Vehicle.SOC, 3, 0, tempbuffer);
		strcat(tempbuffer," %");
		FTImpl.Cmd_Text(300,95,31,0,tempbuffer);
		//sprintf(tempbuffer,"%d",Vehicle.V_Batt_Avg);
		if (Vehicle.cantrfic) dtostrf(Vehicle.V_Batt/100, 4, 1, tempbuffer);
		strcat(tempbuffer," Volts");
		FTImpl.Cmd_Text(400,135,27,0,tempbuffer);
	
	//  ****************
	// Power Meter
	//  ****************
		// bACKGROUND
		FTImpl.ColorRGB(0xFFFFFF);
		FTImpl.Begin(FT_RECTS);
		FTImpl.Vertex2f(240 * 16, 160 * 16);
		FTImpl.Vertex2f(480 * 16, 272 * 16);
		FTImpl.ColorRGB(0x000000);
		FTImpl.Cmd_Text(400,175,27,0,"Power");
		FTImpl.Cmd_Text(400,190,27,0,"Output");
	
		// Values
		tempbuffer[0]='-';
		tempbuffer[1]='-';
		tempbuffer[2]=0;
		Vehicle.V_Batt_Avg=Vehicle.Current*Vehicle.V_Batt/1000;
		if (Vehicle.cantrfic) dtostrf(Vehicle.V_Batt_Avg, 4, 0, tempbuffer);
		strcat(tempbuffer," W");
		FTImpl.Cmd_Text(300,215,31,0,tempbuffer);
		
		if (Vehicle.cantrfic) dtostrf(Vehicle.Current/10, 4, 1, tempbuffer);
		strcat(tempbuffer," Amps");
		FTImpl.Cmd_Text(400,255,27,0,tempbuffer);
		
	
	
	// Vehicle State
	sprintf(tempbuffer,"%s",StatusList[Vehicle.state].desc);
	FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,70,29, FT_OPT_CENTER , tempbuffer);
	
	
	
	
	//sprintf(tempbuffer,"Can packets: %d",Vehicle.cancnt);
	//FTImpl.Cmd_Text(230,30,28,FT_OPT_CENTER,tempbuffer);
	
	// Time  
	sprintf(tempbuffer,"%02d %c %02d : %02d", rtc.getHours(), (rtc.getSeconds()%2>0)? ':' : '  ', rtc.getMinutes(), rtc.getSeconds());
	//sprintf(tempbuffer,"%02d h %02d : %02d", rtc.getHours(),  rtc.getMinutes(), rtc.getSeconds());
	FTImpl.ColorRGB(0xffffff);
	FTImpl.Cmd_Text(375,10,21,FT_OPT_CENTER,Vehicle.time);
	

	FTImpl.ColorRGB(0xFFFF00);
	/*
	if(DL_fct[DL_Wifi].status==FCT_ACTIVE){
		FTImpl.ColorRGB(0x00FF00);
		
	}
	*/
	
	FTImpl.PointSize(100);
	FTImpl.Begin(FT_POINTS);
	FTImpl.Vertex2f(470*16,24*16);
	
	FTImpl.ColorRGB(0xFFFFFF);
	
	//int tx,ty;
	
	/*
	FTImpl.LineWidth(50);	
	for (float jtemp=2.4; jtemp<j ; jtemp+=0.1){
		FTImpl.Begin(FT_LINES);
		FTImpl.Vertex2f(predefCos(jtemp),predefSin(jtemp));
		FTImpl.Vertex2f(predefCos(jtemp+0.1),predefSin(jtemp+0.1));	
	}
	*/
	//j=2.4+float(Vehicle.speed/3);
	// Final point position
	/*
	FTImpl.PointSize(100);
	FTImpl.Begin(FT_POINTS);
	FTImpl.Vertex2ii(predefCos(j)/16,predefSin(j)/16);
	FTImpl.ColorRGB(0x000000);
	FTImpl.PointSize(50);
	FTImpl.Vertex2ii(predefCos(j)/16,predefSin(j)/16);
	
	*/
	
	
	//if (dir) j+=0.2;
	//else j-=0.2;
	//if (dir && j>6) dir=!dir;
	//if (!dir && j<2.4) dir=!dir;
	//FTImpl.Vertex2f(120*16 +1400*cos(millis()/100),	168*16 + 1400*sin(millis()/100));
	
}

void RefreshLCD(int i){
	/*if (!DL_fct[DL_LCD].active) return;
	if (DL_fct[DL_SD].status!=FCT_ACTIVE) return;
	*/
	if (millis()<refreshlcdtimeout) return;
	
	int16_t w=80,h=50,spc=5;		//menu buttons
	uint16_t menuoptions=0;
	uint tempvalue=0;
	
	//uint8_t duty;
	//duty = millis()>>6;
	//FTImpl.DisplayPWM(duty);
	
	//if ((menuBtns.whentoremove<millis()) && (menuBtns.Ypos<270) )menuBtns.Ypos+=2;
	//if ((menuBtns.whentoremove>millis()) && (menuBtns.Ypos>230) )menuBtns.Ypos-=4;
	
	if(RW_spi.rfpacketsend || RW_spi.sdpacketwrite || RW_spi.lcdspiaccess) return;
	
	RW_spi.lcdspiaccess=true;
	sTagXY sTagxy;
	//FTImpl.GetTagXY(sTagxy);
	
	FTImpl.DLStart(); //start the display list. Note DLStart and DLEnd are helper apis, Cmd_DLStart() and Display() can also be utilized.
	
	FTImpl.ClearColorRGB(0xff,0xff,0xff);
	 
	 //FTImpl.Begin(FT_RECTS);
	 //FTImpl.Cmd_LoadIdentity();
	 //int i=(millis()/100)%360;
	 //FTImpl.Cmd_Rotate(i*65535/360);
	 //FTImpl.Cmd_SetMatrix();
	 //FTImpl.LineWidth(10 * 16);
	 //FTImpl.Vertex2f(83 * 16, 13 * 16);
	 //FTImpl.Vertex2f(98 * 16, 200 * 16);
	 //FTImpl.Vertex2f(200 * 16, 200 * 16);
	
	
	if (!display_screen.looprun) {
		displayBackGPic();
		FTImpl.ColorRGB(0xFFFFFF);
		FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,24,31, FT_OPT_CENTER , "Updating Firmware");		
		//displaydebug();
	}
	
	else{
		
		
		
		//displayWifi();
		//displayTouch();
		
		// display menu buttons
		//FTImpl.TagMask(1);
		
		
		//FTImpl.TagMask(0);
		
		//if (display_screen.spinner) FTImpl.Cmd_Spinner((FT_DISPLAYWIDTH/2),(FT_DISPLAYHEIGHT/2),4,0);
		displayMain();
		//FTImpl.Cmd_Spinner(100,100,4,0);
		
	}
	
	
	if (display_screen.spinner) FTImpl.Cmd_Spinner((FT_DISPLAYWIDTH/2),(FT_DISPLAYHEIGHT/2),4,0);
	//displaySOC();
	/*
	//	Display menu buttons !
	for (int i=0;i<DL_FCTS;i++){
		FTImpl.Cmd_BGColor(0x00ff40);
		FTImpl.Tag(DL_fct[i].idx);
		FTImpl.Cmd_FGColor(statusColor[DL_fct[i].status]);
		int a=statusColor[DL_fct[i].status];
		//if (sTagxy.tag==i)  menuoptions= FT_OPT_FLAT;
		//else menuoptions =  DL_fct[i].options;
		FTImpl.Cmd_Button(menuBtns.Xpos+(menuBtns.w+menuBtns.gap)*i,
		menuBtns.Ypos,	menuBtns.w,	menuBtns.h,	menuBtns.font,	menuoptions,DL_fct[i].name			);
	}
	*/
	
	FTImpl.DLEnd();//end the display list
	FTImpl.Finish();//render the display list and wait for the completion of the DL
	//FTImpl.Cmd_Swap();
	RW_spi.lcdspiaccess=false;
	refreshlcdtimeout=millis()+refreshlcdtimeoutperiod;
}

void InitLCD() {
	//if (!DL_fct[DL_LCD].active) return;
	Serial.print("--- Init LCD ---");
	uint32_t chipid = 0;
	RW_spi.lcdspiaccess=true;
	//DL_fct[DL_CAN].status=FCT_WARNING;
	FTImpl.Init(FT_DISPLAY_RESOLUTION);//configure the display to the WQVGA

	delay(20);//for safer side
	chipid = FTImpl.Read32(FT_ROM_CHIPID);
	
	
	Serial.print("\tFT_ROM_CHIP_ID:");
	Serial.println(FT_ROM_CHIPID,HEX);
	Serial.print("\tCHIP_ID:");
	Serial.println(chipid,HEX);
	
	
	// Identify the chip
	if(FT800_CHIPID != chipid){
		Serial.print("\r\n\tError in chip id read ");
		Serial.println(chipid,HEX);
		Serial.println("\tNo LCD fct will be performed ");
		//DL_fct[DL_SD].status=FCT_DISABLE;
		//DL_fct[DL_LCD].active=DISABLED;
		RW_spi.lcdspiaccess=false;
		return;
	} else Serial.println(" OK !");
	
	
	//Platform pressure sensitivity adjustment
	FTImpl.Write16(REG_TOUCH_RZTHRESH,1200);
	
	// Set the Display & audio pins
	FTImpl.SetDisplayEnablePin(FT_DISPENABLE_PIN);
	//FTImpl.SetAudioEnablePin(FT_AUDIOENABLE_PIN);
	
	FTImpl.DisplayOn();
	
	
	//
	loadBGpic();
	//loadSOC('g','0');
	
	FTImpl.DLStart();
	
	//displayMain();
	//displaySOC();
	displayBackGPic();
	FTImpl.DLEnd();//end the display list
	FTImpl.Finish();//render the display list and wait for the completion of the DL
	
	//DL_fct[DL_LCD].status=FCT_ACTIVE;
	RW_spi.lcdspiaccess=false;
	//Calibrate();
	//RefreshLCD(0);
	
	
}


void PrintLCD(const char Display_string[]) {
	Serial.println(Display_string);
	// if FT800 not mounted, only do Serial debug and exit
	//if (DL_fct[DL_LCD].active!=FCT_ACTIVE) return;
	
	

	// Prepare Terminal style buffer list
	strcpy(LCDterminal.LCDtext[ LCDterminal.LCDtextIdx],Display_string);
	LCDterminal.LCDtextIdx+=1;
	
	if (LCDterminal.LCDtextIdx ==9) LCDterminal.LCDtextIdx =0;
	
	if (LCDterminal.LCDtextTermIdx<8) LCDterminal.LCDtextTermIdx+=1;
	else {
		LCDterminal.LCDtextRollIdx+=1;
		if (LCDterminal.LCDtextRollIdx==9) LCDterminal.LCDtextRollIdx= 0;
		for (int i=0;i<9;i++) {
			//if (LCDtextTermIdx+i<9) Termidx[i]= LCDtextTermIdx+i;
			//else Termidx[i] = LCDtextTermIdx+i -9;
			if (LCDterminal.LCDtextIdx+i < 9) LCDterminal.Termidx[i] = LCDterminal.LCDtextIdx+i;
			else LCDterminal.Termidx[i] = LCDterminal.LCDtextIdx+i - 9;
		}
	}
	//RefreshLCD(refreshcnt);
}


void loadSOC(char color,char level){
	SdFile file2;
	int16_t	n;
	uint8_t temp[512];
	char soc[50]="/static/battery/green/b-g-00_RGB565/b-g-00.bin";
	soc[14]=color;
	soc[16]=level;
	
	Serial.println(String(soc));
	Serial.println(String(level));
	if(!file2.open(soc, O_READ)) {
		Serial.println("\t  - File not found, init at random");
		FTImpl.WriteCmd(CMD_MEMSET);
		FTImpl.WriteCmd(0);								// ptr
		FTImpl.WriteCmd(random(150));					// value to fill with
		FTImpl.WriteCmd(5000);			//number of bytes in the memery block
	}
	else     {
		Serial.print("\t  - SOC file is found with size:");
		Serial.println(file2.fileSize());
		FTImpl.Cmd_Inflate(0);		// CMD_Loadimage for JPG
		while (file2.curPosition() < file2.fileSize()){
			n = min(512, file2.fileSize() - file2.curPosition());
			n = (n + 3) & ~3;   // force 32-bit alignment
			file2.read(temp,n);
			FTImpl.WriteCmd(temp, n);//alignment is already taken care by this api
			
		}
	}
		
	file2.close();
	
	
}

void loadBGpic(){
	SdFile file;
	int16_t	n, blklen = 40*1024L;			//20KB reserved size
	uint8_t temp[512];

	Serial.println("\tLoading bg file");
	//if(!file.open("Mp480272.jpg", O_READ)) {
	//if(!file.open("spd01.bin", O_READ)) {
	if(!file.open("mdl-bg1.jpg", O_READ)) {
		Serial.println("\t  - File not found, init at random");
		FTImpl.WriteCmd(CMD_MEMSET);
		FTImpl.WriteCmd(0);								// ptr
		FTImpl.WriteCmd(random(150));					// value to fill with
		FTImpl.WriteCmd(blklen);			//number of bytes in the memery block
	}
	else     {
		Serial.print("\t  - file is found with size:");
		Serial.println(file.fileSize());
		FTImpl.Cmd_LoadImage(0,0);		// CMD_Loadimage for JPG
		while (file.curPosition() < file.fileSize()){
			n = min(512, file.fileSize() - file.curPosition());
			n = (n + 3) & ~3;   // force 32-bit alignment
			file.read(temp,n);
			FTImpl.WriteCmd(temp, n);//alignment is already taken care by this api
		}
	}
	//FTImpl.Cmd_Memset(blklen,random(150),blklen);
	//FTImpl.BitmapHandle(1);
	file.close();
}
void loadSpeedo(){
	
}
void displaySpeedo(){
	FTImpl.BitmapHandle(0);
	FTImpl.BitmapSource(0);
	FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 240, 224);
	FTImpl.BitmapLayout(FT_RGB565,480,224);		//
	FTImpl.Begin(FT_BITMAPS);
	FTImpl.Vertex2ii(0,0,0,0);
	FTImpl.End();
}

void displaySOC(){
	FTImpl.BitmapHandle(0);
	FTImpl.BitmapSource(0);
	FTImpl.BitmapLayout(FT_RGB565,480,112);		//
	FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 240, 112);
	FTImpl.Begin(FT_BITMAPS);
	FTImpl.Vertex2ii(240,47,0,0);
	FTImpl.End();
}
void displayBackGPic(){
	uint16_t		w=480,h=272;
	//FTImpl.ColorRGB(0xFFFF0A);
	//FTImpl.Clear(1,1,1);  
	
	//FTImpl.Cmd_BGColor(0xFFFFFF);
	//FTImpl.Cmd_Gradient(0, 0, 0x0000ff, 160, 0,	0xff0000);
	FTImpl.BitmapHandle(0);	
	FTImpl.BitmapSource(0);			
    FTImpl.BitmapLayout(FT_RGB565,w*2,h);		//
	FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, w, h);	
	FTImpl.Begin(FT_BITMAPS);
	FTImpl.Vertex2ii(0,0,0,0);	
	FTImpl.End();
	
	
			
}



void displaydebug(){
	int YPos = -22;
	FTImpl.ColorRGB(0xFF,0xFF,0xFF);//set the color of the string to white color
	for (int i=0;i<9;i++){
		FTImpl.Cmd_Text(0,YPos+=22, 26, 0, LCDterminal.LCDtext[LCDterminal.Termidx[i]]);
	}
}

void displayWifi(){
	int fonttype=22;
	int fontheight = 22;
	int lineidx = -fontheight;
	int  xpos = 260;
	int font_options=0;
	lineidx=-30;
	char tempbuffer[40];
	//byte mac[7]={0};
	WiFi.macAddress(Vehicle.mac);
	sprintf(tempbuffer,"SN: %02X:%02X:%02X:%02X:%02X:%02X",
				Vehicle.mac[0],Vehicle.mac[1],Vehicle.mac[2],
				Vehicle.mac[3],Vehicle.mac[4],Vehicle.mac[5]);
	
	FTImpl.Cmd_Text(xpos,lineidx+=30, fonttype, 0, tempbuffer);
	sprintf(tempbuffer,"Name: %s",Vehicle.Name);
	FTImpl.Cmd_Text(xpos,lineidx+=30, fonttype, 0, tempbuffer);
	//if (WiFi.status() != WL_CONNECTED) {
		
	//if (DL_fct[DL_Wifi].active) {
		
			//FTImpl.Cmd_FGColor(statusColor[DL_fct[DL_Wifi].status]);
			
			FTImpl.ColorRGB(0xFF,0x22,0x22);
			sprintf(tempbuffer,"WIFI");
			FTImpl.Cmd_Text(xpos,lineidx+=fontheight, fonttype, 0, tempbuffer);
			FTImpl.ColorRGB(0xFF,0xFF,0xFF);
			//listNetworks();
		
	//}
	
	char a = (char)(WiFi.localIP() & 0xff);
	char b = (char)((WiFi.localIP() >> 8) & 0xff);
	char c = (char)((WiFi.localIP() >> 16) & 0xff);
	char d = (char)((WiFi.localIP() >> 24) & 0xff);
	sprintf(tempbuffer,"IP addr: %d.%d.%d.%d",a,b,c,d);
	FTImpl.Cmd_Text(xpos,lineidx+=fontheight, fonttype, 0, tempbuffer);
	sprintf(tempbuffer,"Server: %s",PostDataURL.host);
	FTImpl.Cmd_Text(xpos,lineidx+=fontheight, fonttype, 0, tempbuffer);
	
	
}

void displayTouch(){
	sTagXY sTagxy;
	FTImpl.GetTagXY(sTagxy);
	
	//if (!penCalibrated)  Calibrate();
	if(0) {;}
	else {
		if ((sTagxy.tag==0) && (display_screen.previousTag !=0)){
			//if(DL_fct[display_screen.previousTag].status!=0) DL_fct[display_screen.previousTag].status;
			//else DL_fct[display_screen.previousTag].status=FCT_ACTIVE;
		}
		if (sTagxy.tag!=0) {
			menuBtns.whentoremove=millis()/1000+menuBtns.timeout;
			 menuBtns.Xpos=menuBtns.initXpos;
		}
			display_screen.previousTag=sTagxy.tag;
	}	
}

// API to display touch registers on the screen 
void Calibrate() {  
	// ************************************************************************* /
	// Below code demonstrates the usage of calibrate function. Calibrate    
	// function will wait untill user presses all the three dots. Only way to
	// come out of this api is to reset the coprocessor bit.                 
	//  ************************************************************************* /

	// Construct the display list with grey as background color, informative string "Please Tap on the dot" followed by inbuilt calibration command 
	FTImpl.DLStart();
	FTImpl.ClearColorRGB(64,64,64);
	FTImpl.Clear(1,1,1);    
	FTImpl.ColorRGB(0xff, 0xff, 0xff);
	FTImpl.Cmd_Text((FT_DISPLAYWIDTH/2), (FT_DISPLAYHEIGHT/2), 27, FT_OPT_CENTER, "Please Tap on the dot");
	FTImpl.Cmd_Calibrate(0);
	
	// Wait for the completion of calibration - either finish can be used for flush and check can be used 
	FTImpl.Finish();
	//penCalibrated=true;
	
}



/*

Ft_App_WrCoCmd_Buffer(phost,BEGIN(LINES));
Ft_App_WrCoCmd_Buffer(phost,LINE_WIDTH(10));
th = ((ft_uint16_t)val - 4500L) * 32768L / 36000L;
for (o = -5; o < 6; o++)
{
	polar(170, th + (o << 5));
	polar(235, th);
}
Ft_App_WrCoCmd_Buffer(phost,SCISSOR_XY(ox+dt,y));
Ft_App_WrCoCmd_Buffer(phost,SCISSOR_SIZE(w,(ft_uint16_t)(FT_DispHeight*0.36)));
F
*/

/*	
		//FTImpl.Cmd_Inflate(0);		// CMD_Inflate for Binaries	
		FTImpl.WriteCmd(CMD_INFLATE);
		//FTImpl.WriteCmd(CMD_LOADIMAGE);  // CMD_Loadimage for JPG
		//FTImpl.WriteCmd(0);
		//FTImpl.WriteCmd(0);		// options = 0
		FTImpl.WriteCmd(0);
		int a[17];
		n = sizeof(a)/sizeof(a[0]);		// filesize for writecmdfrom flash
		FTImpl.WriteCmdfromflash(&MotrecLogoInFlashMemory[0],41869);
		*/


//FTImpl.BitmapLayout(FT_ARGB1555,w*2,h);
//FTImpl.Cmd_LoadIdentity();
//FTImpl.Cmd_Scale(60000,60000);
//FTImpl.Cmd_SetMatrix();
//FTImpl.BitmapHandle(1);
//FTImpl.BitmapSource(20*1024L);
//FTImpl.BitmapLayout(FT_RGB565,w*2,h);
//FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, w, h);
//FTImpl.Cmd_LoadIdentity();
//FTImpl.Cmd_Scale(60000,60000);
//FTImpl.Cmd_SetMatrix();
//FTImpl.Begin(FT_BITMAPS);
//FTImpl.Vertex2ii(50,50,0,0);
//FTImpl.End();