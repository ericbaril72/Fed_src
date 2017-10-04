/*
 * ProjectMain.cpp
 *
 * Created: 2016-08-08 10:34:50
 *  Author: EricB
 *
 * 
 * Arduino originating Project
 * Trying not to change anything fromthe CORE arduino files ( for now ... )
 *  check out /arduino/core/arduino
 *	startup.c for main system init
 *  and cortex_handlers.c   for most handler ( interupts / system ticks / etc )
 * will refactor later
 *
 */ 
#include "ProjectMain.h"


SdFat sd;
SdFile logFile,tempfile;

RTC_PCF8523 pcfrtc;
RTCZero rtc;

bool ledState = true;
bool calibrationDone = false;

char logFilePath[] = "2016/01/01\0";
char logFilename[] = "2016/01/01/hhmmssoo.log\0";

canMsgBuffer CanBuff;
OneWire ds(d18b20);


bool newFWfile=false;
//bool gpsenabled = false;
char tempbuffer[1024];

StaticJsonBuffer<400> jsonBufferStatus;
JsonObject& JSONstatus = jsonBufferStatus.createObject();
StaticJsonBuffer<128> jsonBufferEvent;
JsonObject& JSONevent = jsonBufferEvent.createObject();
Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);

SRouter wifilist[]={
	{"iPhone de Eric","password123"},
	{"motrecconnect",""},
	{"motrecpriv","mot8761234567"},
	{"HUAWEI-1E4E","20156059"},
	{"MOTREC","mot8761234567"},
	{"HUAWEI-1B76","20154237"},
	{"test","testtest"}
};

HttpURL URLList[4]={
	{"159.203.59.104",8080},
	{"192.168.0.184",8080},
	{"192.168.0.133",8080},
	{"192.168.13.1",8080}
};

statusDescriptors StatusList[]={
	{"uu","undefined"},		// 0
	{"OF","OFF"},					// 1
	{"ON","ON"},					// 2
	{"C1","ChargingC1"},		// 3
	{"C2","ChargingC2"},		// 4
	{"C3","FastCharge"},		// 5
	{"DS","DeepSleep"},		//6
	{"NB","NoBatteries"}		//7
} ;

SRouter wifi = {"iPhone de Eric","password123"};
HttpURL PostDataURL = {"159.203.59.104",8080};

extern display_screen display_screen;

V_status Vehicle;
refreshTimeout refreshTimeout;

spi_access RW_spi;


uint32_t lastsent = 0;					// last msg sent timer
uint32_t lastfilecreated=0;



uint32_t refreshcnt =0;
uint32_t refreshlcdtimeout =1000;		//250 usually
uint32_t serialtimeout =250;
uint32_t gpstimeout= 250;
uint32_t PostTimeout = 1000;
long newlogfilename =0;

int		timer100ms=0;


byte i;
byte present = 0;
byte type_s;
byte data[12];
byte addr[8];
float celsius, fahrenheit;



void setup(){
	initIOs();
	initSerial();
	
	String fwrev = String(F(__DATE__))+String(F(__TIME__));
	fwrev.toCharArray(Vehicle.fwrevCA,fwrev.length());
	
	Serial.println("\f");
	
	Serial.println("Build rev: " + String(F(__DATE__)) + " @ " + String(F(__TIME__)));
	Serial.println(Vehicle.fwrevCA);
	Serial.println("----------------------------------------------");
	Serial.println("---------------- INIT ------------------------");
	Serial.println("----------------------------------------------");
	
	initSD();
	InitRTC();
	
	InitWifi();
	
	
	
	Serial.println("Posting Revision event to server");
	
	#define factorydefault 1
	#define production 0
	
	//#define buildrev factorydefault
	#define buildrev production
	if(0){
		if (buildrev == factorydefault) {
			if (!httpJsonPostEvent(PostDataURL,"VersionNote","FactoryDefault"))	Serial.println("Server has not answered !has the server answered ?");
		}
		else if (buildrev == production) {
			if (!httpJsonPostEvent(PostDataURL,"VersionNote","Production"))	Serial.println("Server has not answered !has the server answered ?");
		}
		
		
		if (!httpJsonPostEvent(PostDataURL,"RebootRev:",Vehicle.fwrevCA))Serial.println("Server has not answered !has the server answered ?");
		httpJsonPostEvent(PostDataURL,"ssid",(char *)wifi.ssid);
		httpJsonPostEvent(PostDataURL,"test","---------test77");
		
		getdownloadlist(PostDataURL);
		#define ERASE 1
		uploadSDtoServer(ERASE);
	}
		
	
	
	createLogFile();
	
	
	//InitCAN();
	//InitLCD();
	
	Serial.println("----------------------------------------------");
	Serial.println("--------------- BEGIN ------------------------");
	Serial.println("----------------------------------------------");
	
	display_screen.looprun = true;
	/*
	scheduler.gpsrefresh.period=10000;
	scheduler.refreshlcd.perios=250;
	scheduler.poststatus.period=10000;
	scheduler.serialdump.period=1000;
	*/
	PostTimeout=millis()+1000;
	lastfilecreated = millis()+10000;
	//gpstimeout=millis()+30000;
	refreshlcdtimeout=0;
	Vehicle.lastIntegralTS=millis();	
	
	
}


void loop(){
	refreshcnt+=1;
	
	if (millis()>refreshlcdtimeout) {
		refreshlcdtimeout=millis()+250;
		evalspeed();
		//RefreshLCD(0);
		
		if (millis()>serialtimeout){
			Serial.print("\f");		
			serialtimeout=millis()+1000;
			
			updateVehicleStatus();
			if (!calibrationDone) {
				initCalibration();
				inittimer();
				initSpeedSensor();
				calibrationDone=true;
			}
			
			//JSONstatus.prettyPrintTo(Serial);
			
			JSONstatus.printTo(tempbuffer,jsonBufferStatus.size());
			Serial.println(tempbuffer);
			if (Vehicle.state==1){
				if (millis()>PostTimeout){
					PostTimeout=millis()+5000;
					updateLogFile(tempbuffer);
				}
				
			}
			else updateLogFile(tempbuffer);
			
			/*
			StaticJsonBuffer<400> jsonBufferCalib;
			JsonObject& JSONcalib = jsonBufferCalib.createObject();
			JSONcalib["CalibA0"]=Vehicle.calibA0;
			JSONcalib["CalibA1"]=Vehicle.calibA1;
			JSONcalib["CalibA2"]=Vehicle.calibA2;
			JSONcalib["CalibA3"]=Vehicle.calibA3;
			JSONcalib["offsetA1"]=Vehicle.offsetA1;
			JSONcalib["offsetA2"]=Vehicle.offsetA2;
			JSONcalib["A0"]=Vehicle.A0;
			JSONcalib["A1"]=Vehicle.A1;
			JSONcalib["A2"]=Vehicle.A2;
			JSONcalib["A3"]=Vehicle.A3;
			JSONcalib["Vbatt"]=Vehicle.V_Batt;
			JSONcalib["CurrentA"]=Vehicle.A_CurrentA;
			JSONcalib["CurrentB"]=Vehicle.A_CurrentB;
			JSONcalib["Vksi"]=Vehicle.V_ksi;
			JSONcalib.printTo(tempbuffer,jsonBufferCalib.size());
			Serial.print(tempbuffer);
			*/
			//Serial.println(Vehicle.A_CurrentA_Avg);
			//Serial.println(Vehicle.A_CurrentB_Avg);
			//if (Vehicle.fileupdate) updateLogFile(tempbuffer);
						
			
			//Serial.print("Current A:");
			//Serial.println(Vehicle.A_CurrentA);
			//Serial.print("Cuurent B:");
			//Serial.println(Vehicle.A_CurrentB);
			Serial.println("log filename:"+String(logFilename)+"    size: "+String(logFile.fileSize()));
			//Serial.println("log filepath:"+String(logFilePath));
			//Serial.println("logfile Size:"+String(logFile.fileSize()));
			//Serial.println("GPS update in : "+ String(1+(gpstimeout-millis())/1000)+" seconds");
			//Serial.println("Checking update in :" + String(1+(lastfilecreated-millis())/1000)+" seconds");
			//Serial.println("Posting status in : "+ String(2+(PostTimeout-millis())/1000)+" seconds");
			//getTemperatures();
			//updateGPSinfo();
			
			//if ((millis()>PostTimeout)){
				if (0){
				
				//if (Vehicle.state==3) postfromfilelist(1);
				//updateCalibrationFactors();
				
				getdownloadlist(PostDataURL);
				if (Vehicle.fileupdate) updateLogFile(tempbuffer);
				
				Vehicle.lastPost = httpJsonPost(PostDataURL,tempbuffer,"status"); 
				//if (Vehicle.fileupdate) updateLogFile(tempbuffer);
				//if (!Vehicle.fileupdate) logFile.close();
				//initCalibration();
				PostTimeout=millis()+5000;
				
				
				//postJSON(PostDataURL,tempbuffer,"event");
				//postJSON(PostDataURL,"{}","gps");
				//postJSON(PostDataURL,"{}","CANmsg");
					
				//Serial.println("FW rev: "+fwrev);
				//Serial.println("Wifi: "+ String(wifi.ssid)+ "\tServer: " + String(PostDataURL.host));
				//Serial.print("CanBuff.idx");Serial.print(CanBuff.idx);
				//Serial.println("gps.GGA: "+String(gps.GGA));
				//Serial.println("gps.VTG: "+String(gps.VTG));
				/*
				if (CanBuff.idx>0){
					tempbuffer[0]='\0';
					for (int i=1;i<=CanBuff.idx;i++){
						sprintf(tempbuffer,"\t%d;CAN00;0x%03X;%d",
						CanBuff.id[i].timestamp, CanBuff.id[i].msgId,	CanBuff.id[i].len);
						for (int j=0;j<8;j++) sprintf(tempbuffer +strlen(tempbuffer),":%02X",CanBuff.id[i].msg[j]);
						Serial.println(tempbuffer);
						updateLogFile(tempbuffer);
					}
					if (!Vehicle.cantrfic) CanBuff.idx=0;
				}
				*/

				
			}			
		}
	}	
}

void initCalibration(){
	while (!updateCalibrationFactors()) {
		httpJsonPostEvent(PostDataURL,"CalibrationDone","false");
		updateVehicleStatus();
		JSONstatus.printTo(tempbuffer,jsonBufferStatus.size());
		Vehicle.lastPost = httpJsonPost(PostDataURL,tempbuffer,"status");
		updateLogFile("initCalibration:");
		updateLogFile(tempbuffer);
		
		
		StaticJsonBuffer<256> jsonBufferCalib;
		JsonObject& JSONcalib = jsonBufferCalib.createObject();
		JSONcalib["CalibA0"]=Vehicle.calibA0;
		JSONcalib["CalibA1"]=Vehicle.calibA1;
		JSONcalib["CalibA2"]=Vehicle.calibA2;
		JSONcalib["CalibA3"]=Vehicle.calibA3;
		JSONcalib["offsetA1"]=Vehicle.offsetA1;
		JSONcalib["offsetA2"]=Vehicle.offsetA2;
		JSONcalib["A0"]=Vehicle.A0;
		JSONcalib["A1"]=Vehicle.A1;
		JSONcalib["A2"]=Vehicle.A2;
		JSONcalib["A3"]=Vehicle.A3;
		JSONcalib.printTo(tempbuffer,jsonBufferCalib.size());
		Vehicle.lastPost = httpJsonPost(PostDataURL,tempbuffer,"calibration");
		updateLogFile(tempbuffer);
		
		//delay(5000);
	}
	StaticJsonBuffer<256> jsonBufferCalib;
	JsonObject& JSONcalib = jsonBufferCalib.createObject();
	JSONcalib["CalibA0"]=Vehicle.calibA0;
	JSONcalib["CalibA1"]=Vehicle.calibA1;
	JSONcalib["CalibA2"]=Vehicle.calibA2;
	JSONcalib["CalibA3"]=Vehicle.calibA3;
	JSONcalib["offsetA1"]=Vehicle.offsetA1;
	JSONcalib["offsetA2"]=Vehicle.offsetA2;
	JSONcalib["A0"]=Vehicle.A0;
	JSONcalib["A1"]=Vehicle.A1;
	JSONcalib["A2"]=Vehicle.A2;
	JSONcalib["A3"]=Vehicle.A3;
	JSONcalib.printTo(tempbuffer,jsonBufferCalib.size());
	updateLogFile("initCalibration");
	updateLogFile(tempbuffer);
	//Vehicle.lastPost = httpJsonPost(PostDataURL,tempbuffer,"calibration");
	
	
	//httpJsonPostEvent(PostDataURL,"CalibrationDone","true");
}

void uploadSDtoServer(bool erase){
	if (WifiAvail()==false) return;
	sd.ls(LS_SIZE);
	tempfile.open("filepost.lst", O_WRITE);
	tempfile.remove();
	
	tempfile.open("filepost.lst", O_WRITE | O_CREAT | O_AT_END);
	sd.ls(&tempfile, LS_R | LS_DTREE);
	tempfile.close();
	
	int data;
	Serial.println("----------------------reading file:------");
	tempfile.open("filepost.lst", O_READ);
	while ((data=tempfile.read())>0) Serial.write(data);
	tempfile.close();
	
	Serial.println("--------------Posting it all ----");
	
	postfromfilelist(erase);
	Serial.println("----------------------reading file for remainings :");
	
	if (erase) {
		tempfile.open("filepost.lst", O_READ);
		while ((data=tempfile.read())>0) Serial.write(data);
		tempfile.close();
		//sd.ls(&Serial, LS_R | LS_DTREE);
		Serial.println("-----erasing filepost.lst-----");
		
		tempfile.open("filepost.lst", O_WRITE);
		tempfile.remove();
		Serial.println("-----final sd.ls-----");
		Serial.println();
		//sd.ls(&Serial, LS_R | LS_DTREE);
	}
	
}

bool updateCalibrationFactors(){
	bool paramsUpdated=false;
	char teststring[400]={};
	if (sd.exists("params.cfg")) {
		SdFile params;
		params.open("params.cfg",O_READ);
		params.fgets(teststring, sizeof(teststring));
		params.close();
		Serial.println("--------------------------file params parsing");
		Serial.println(teststring);
		Serial.println("---");
		StaticJsonBuffer<400> JSONbuffer1;
		JsonObject& root = JSONbuffer1.parseObject(teststring);
		if (!root.success()) {
			Serial.println("parseObject() failed");
			httpJsonPostEvent(PostDataURL,"ParseParamsFile","Fail");
						
		}
		else if (root["num_results"]==1) {
			Serial.println("New Calibration applied");
			Vehicle.calibA0 = root["objects"][0]["calibA0"];
			Vehicle.calibA1 = root["objects"][0]["calibA1"];
			Vehicle.calibA2 = root["objects"][0]["calibA2"];
			Vehicle.calibA3 = root["objects"][0]["calibA3"];
			Vehicle.offsetA1 = root["objects"][0]["offsetA1"];
			Vehicle.offsetA2 = root["objects"][0]["offsetA2"];
			paramsUpdated=true;
			//Serial.println("Vehicle.offsetA1"+String(Vehicle.offsetA1));
			//Serial.println("Vehicle.offsetA1"+String(Vehicle.offsetA1));
			//Serial.println("New calibration : logfile dump");
			//Serial.println("teststring:"+String(teststring));
			//updateLogFile(teststring);
			//root.printTo(Serial);
			
			
		} else {
			Serial.println("------------------params File parsing ODDD !!!!!");
			root.printTo(Serial);
			Serial.println();
			Serial.println("------------------------------------------");
			
		}

	}
	
	return paramsUpdated;
	
	
	
	teststring[0]='\0';
	char urlLink[200];
	sprintf(urlLink,"http://159.203.59.104:8080/api/v1/calibration?q={\"filters\":[{\"name\":\"VehicleName\",\"op\":\"==\",\"val\":\"%s\"}]}",Vehicle.id);
	Serial.println("Web calibration: "+String(urlLink));
	getJSON(urlLink,teststring);
	Serial.println("Server answered: "+String(teststring));
	
	StaticJsonBuffer<400> JSONbuffer2;
	JsonObject& root2 = JSONbuffer2.parseObject(teststring);
	Serial.println("Parse results:");

	if (!root2.success()) {
		Serial.println("Parse TestString failed:");
		Serial.println(teststring);
		Serial.println("-------------");
	}
	else if (root2["num_results"]==1) {
		//Serial.println("New Calibration:");
		Vehicle.calibA0 = root2["objects"][0]["calibA0"];
		Vehicle.calibA1 = root2["objects"][0]["calibA1"];
		Vehicle.calibA2 = root2["objects"][0]["calibA2"];
		Vehicle.calibA3 = root2["objects"][0]["calibA3"];
		Vehicle.offsetA1 = root2["objects"][0]["offsetA1"];
		Vehicle.offsetA2 = root2["objects"][0]["offsetA2"];
		paramsUpdated = root2["objects"][0]["validated"];
		if (root2["objects"][0]["validated"]==1) paramsUpdated=true;
		else paramsUpdated=false;
		//Serial.println("New calibration : logfile dump");
		//Serial.println("teststring:"+String(teststring));
		//updateLogFile(teststring);
		//root.printTo(Serial);
		SdFile params;
		params.open("params.cfg",O_WRITE | O_CREAT);
		root2.printTo(params);
		params.close();
		
		//postthisfile(PostDataURL,"params.cfg");
			
	}
	else {
		Serial.println("Odd parsing from Web server data");
		root2.printTo(Serial);
		
	}
	Serial.println();
	Serial.println("------------------------");
	return paramsUpdated;
	
}

// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
	DateTime now = pcfrtc.now();
	
	// return date using FAT_DATE macro to format fields
	*date = FAT_DATE(now.year(), now.month(), now.day());
	// return time using FAT_TIME macro to format fields
	*time = FAT_TIME(now.hour(), now.minute(), now.second());
}



void serialstatusdump(){

}

void updateGPSinfo(){
	if (millis()<gpstimeout) return;
	gpstimeout=millis()+10000;
	char gpstag[16];
	char c;
	bool GGA=false;
	bool VTG=false;
	int idx=0;
	
	Serial1.begin(4800);
	gpstimeout=millis()+2000;
	while (millis()<gpstimeout){
		if (Serial1.available()) {
			idx++;
			c=Serial1.read();
			if ((c==10) || (c==13)) {
				if (GGA && VTG) break;
				idx=0;
			} 
			else if ( idx <=6) gpstag[idx-1]=c;
			else {
				if (String(gpstag).startsWith("$GPGGA")) {
					gps.GGA[idx-7]=c;
					GGA=true;
				}
				if (String(gpstag).startsWith("$GPVTG")) {
					gps.VTG[idx-7]=c;
					VTG=true;
				}
			}
		}
	}
	if (GGA && VTG) gps.valid =true;
	Serial1.end();
	//Serial1.begin(115200);
	
}

void updateVehicleStatus() {
	sprintf(Vehicle.timeM0,"%02dh%02d:%02d", rtc.getHours(),  rtc.getMinutes(), rtc.getSeconds());
	DateTime now = pcfrtc.now();
	sprintf(Vehicle.time,"%02dh%02d:%02d", now.hour(),  now.minute(), now.second());
	if (Vehicle.cancnt==0) Vehicle.cantrfic=false;
	else Vehicle.cantrfic=true;
	Vehicle.cancnt=0;
	
	if ((Vehicle.A_CurrentA>50)	&& (Vehicle.A_CurrentA<50) ) Vehicle.Current=Vehicle.A_CurrentB;		
	else Vehicle.Current=Vehicle.A_CurrentA;
	if (Vehicle.charger_detect) {		// Charging		=mode 3 or 5
		if (Vehicle.Current > 210) Vehicle.state=5;
		else Vehicle.state = 3;
	} else if ((Vehicle.V_ksi<10) && 
			((Vehicle.Current>10) &&(Vehicle.Current<10))
			){		// Charging	=mode 3
		Vehicle.state=3;
	} else if (Vehicle.V_ksi<10) Vehicle.state=1;			//  OFF		= mode 1
	else Vehicle.state = 2;									// ON		= mode 2
	
	
	
	JSONstatus["Time"]=Vehicle.time;
	JSONstatus["TimeM0"]=Vehicle.timeM0;
	JSONstatus["Vbatt"]=Vehicle.V_Batt;
	JSONstatus["Vksi"]=Vehicle.V_ksi;
	
	JSONstatus["Speed"]=Vehicle.speed;
	JSONstatus["RPM"]=Vehicle.rpm;
	if (Vehicle.state==3) JSONstatus["state"]="Charging";
	else if (Vehicle.state==1) JSONstatus["state"]="OFF";
	else if (Vehicle.state==2) JSONstatus["state"]="ON";
	else JSONstatus["state"]="ERROR";
	
	JSONstatus["CurrentA"]=Vehicle.A_CurrentA;
	JSONstatus["CurrentB"]=Vehicle.A_CurrentB;
	JSONstatus["Current"]=Vehicle.Current;
	
	//JSONstatus["Power"]=Vehicle.power;
	//JSONstatus["EnergyOUT"]=Vehicle.EnergyOUT;
	//JSONstatus["EnergyIN"]=Vehicle.EnergyIN;
	//JSONstatus["TempS1"]=Vehicle.TempS1;
	//JSONstatus["TempS2"]=Vehicle.TempS2;
	if (Vehicle.oldstate != Vehicle.state) {
		PostTimeout=0;
		String VehicleState = JSONstatus["state"];
		char CharArrayVehicleState[30];
		VehicleState.toCharArray(CharArrayVehicleState,sizeof(CharArrayVehicleState));
		httpJsonPostEvent(PostDataURL,"statechange",CharArrayVehicleState);
		createLogFile();
		if (Vehicle.state==1) {
			uploadSDtoServer(ERASE);
		}
	}
	Vehicle.oldstate=Vehicle.state;
	
	JSONstatus["LastPost"]= Vehicle.lastPost;
}

void deepsleep(){
	Serial.println("going into deep sleep");
	//EIC->WAKEUP.reg = (1<<digitalPinToInterrupt(14));   //EIC_WAKEUP_WAKEUPEN2;
	TcCount16* TC = (TcCount16*) TC3; // get timer struct

	//TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
	//while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	
	//WiFi.setSleepMode(M2M_PS_MANUAL, 1);
	//WiFi.requestSleep(604800000);
		
	
	//SYSCTRL->VREG.bit.RUNSTDBY = 1;
	// Enable the DFLL48M clock in standby mode
   //SYSCTRL->DFLLCTRL.bit.RUNSTDBY = 1;
  
	//pinMode(1,OUTPUT);
	//digitalWrite(1,HIGH);
	//USBDevice.detach();
	//pinMode(14,INPUT);		// ksi for wake-up
	//attachInterrupt(14,wakeISR,HIGH);	// ksi for wake-up
	
	//rtc.setAlarmTime(rtc.getHours() , rtc.getMinutes() , rtc.getSeconds() +10);
	//rtc.enableAlarm(rtc.MATCH_HHMMSS);
	//rtc.attachInterrupt(alarmMatch);
	
	//rtc.standbyMode();
	//SCB->SCR |= 1<<2; 	// Enable deep-sleep mode
	//__WFI();
	
	//delay(500);
	//digitalWrite(13,LOW);
	//USBDevice.attach();
	
	// re-enable TC interrupts
	//TC->CTRLA.reg |= TC_CTRLA_ENABLE;
	//while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	
}

void wakeISR() {
	detachInterrupt(14);
}




void TC3_Handler() {
	TcCount16* TC = (TcCount16*) TC3; // get timer struct
		digitalWrite(13,HIGH);
		Vehicle.A0=		analogRead(A0);
		Vehicle.A1=		analogRead(A1);
		Vehicle.A2=		analogRead(A2);
		Vehicle.A3=		analogRead(A3);
		
		Vehicle.V_Batt_Avg		+=Vehicle.A0 ;
		Vehicle.A_CurrentA_Avg	+=Vehicle.A1;
		Vehicle.A_CurrentB_Avg	+=Vehicle.A2;
		Vehicle.V_ksi_Avg		+=Vehicle.A3;
		Vehicle.oversampling++;
		
		if (Vehicle.oversampling>=4){
			Vehicle.V_Batt_Avg/=4;
			Vehicle.A_CurrentA_Avg/=4;
			Vehicle.A_CurrentB_Avg/=4;
			Vehicle.V_ksi_Avg/=4;
			
			Vehicle.oversampling=0;
			Vehicle.V_Batt		=Vehicle.V_Batt_Avg							* Vehicle.calibA0;
			Vehicle.V_ksi		=Vehicle.V_ksi_Avg							* Vehicle.calibA3;
			Vehicle.A_CurrentA	=(Vehicle.A_CurrentA_Avg-Vehicle.offsetA1)	* Vehicle.calibA1;
			Vehicle.A_CurrentB	=(Vehicle.A_CurrentB_Avg-Vehicle.offsetA2)	* Vehicle.calibA2;
			
			
			Vehicle.V_Batt_Avg=0;
			Vehicle.A_CurrentA_Avg=0;
			Vehicle.A_CurrentB_Avg=0;
			Vehicle.V_ksi_Avg=0;
		}
		
		
		
		//if (Vehicle.A_CurrentA<60) Vehicle.power = Vehicle.V_Batt * Vehicle.A_CurrentA;
		//else Vehicle.power = Vehicle.V_Batt * Vehicle.A_CurrentB;
		
		//if (Vehicle.power > 0){
		//	Vehicle.EnergyOUT+= Vehicle.power /4;
		//}
		//else if (Vehicle.power < 0){
		//	Vehicle.EnergyIN+= Vehicle.power /4;
		//}
		digitalWrite(13,LOW);
	
	if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
				
		//if (!digitalRead(INT_CAN0) && !RW_spi.canpacketread) {
			//if (DL_fct[DL_CAN].active==FCT_ACTIVE) {
				//if (readCAN(tempbuffer)) Serial.println(tempbuffer); 		// Si le moduke CAN est install
				//readCAN();
			//}
		//}
		TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
	}
		
	//			for tighter    timer handling then the DIV_xyz prescaler/overflow interupt
	//	if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
	// Do your stuff here ..,.
	//	TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag cco int flag
	//}
}
void TC4_Handler() {		// every seconds , check for the periodic timeouts !!!
	TcCount16* TC = (TcCount16*) TC4; // get timer struct
	
	TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
}
void dingrpm() {
	Vehicle.rpmcnt = Vehicle.rpmcnt+1;
}

void updateLogFile(char loopdata[]) {
	// Write in Log File
	digitalWrite(13,HIGH);
	RW_spi.sdpacketwrite = true;
	logFile.println(loopdata);
	logFile.flush();
	RW_spi.sdpacketwrite = false;
	delay(250);
	digitalWrite(13,LOW);
}
void evalspeed(){
	const prog_uchar mileshrTable[] PROGMEM=  {
			0,2,3,5,7,8,10,12,13,15,17,18,20,22,24,25,27,29,30,32,34,35,37,39,40,42,44,45,47,49,50,52,54,55,57,59,61,62,64,66,67,69,71,72,74,76,77,79,81,82,84,86,87,89,91,92,94,96,97,99,101,103,104,106
	};
	const prog_uchar kmhrTable[] PROGMEM=	{
			0,3,5,8,11,14,16,19,22,24,27,30,32,35,38,41,43,46,49,51,54,57,60,62,65,68,70,73,76,78,81,84,87,89,92,95,97,100,103,106,108,111,114,116,119,122,124,127,130,133,135,138,141,143,146,149,151,154,157,160,162,165,168,170,173,176,179,181,184,187,189,192,195,197,200,203,206,208,211,214,216,219,222,225,227,230,233,235,238,241,243,246,249,252,254
	};
	//Vehicle.speed = mileshrTable[Vehicle.rpmcnt];	//Vehicle.rpmratio;
	uint8_t rpmcnt = min(Vehicle.rpmcnt,63);
	Vehicle.rpm = rpmcnt;
	//rpmcnt=44;
	//if (rpmcnt>0 ) Vehicle.speed = (float)mileshrTable[rpmcnt]/10;	//Vehicle.rpmratio;
	Vehicle.speed = (float)mileshrTable[rpmcnt]/10;	//Vehicle.rpmratio;
	Vehicle.rpmcnt=0;		//needed if rpmcnt calculated by interupts
	
	//dtostrf(V_status.speed, 3, 1, speedval);
	//Vehicle.rpmcnt = 0;
	//  revolutions / 16.99 * 127,6743 cm				* 3600 ( hrs )    /  100 000 (km)		0,036
	//  rps   / motorwheelRatio * wheeldiameteer * 3.1416 *2.54   *
	//  7.5146748 (127,6743cm/tour) * 3600 ( sec / heure )                 * ( tour / sec ) *  ( .000 01 km / cm )  = 0,270528240 km / heure
	//   		 (128/17) 7.529411
	//  *0.270528= x km/hr				0.271058796				0.621371192 miles / km
	//  *0,16809845504 = X  miles / hr
	//    6 tours / seconds = 1 mi /hr
	//	  12 tr / second = 
	// 3.6964					3.6892
	// 0.2305
	//@ 5km-hr
	//1000rpm = 4,5088km /hr								16.6rps			16/17			4.328vs4.599
	//					= 4.5176																					4.33vs4.608
	//
	//  rpmcnt * 0,016908 * wheel diameter in in
}
void createLogFile() {
	
	SdFile listfilestopost;
	// Set New Log file PATH    yyyy/mm/dd
		
	// create new /directory strucutre in SD based on date
	sprintf(logFilePath, "20%02d/%02d/%02d" , rtc.getYear(), rtc.getMonth(), rtc.getDay());
	Serial.println("-- >Create new directory path: " + String(logFilePath));
	
	// prepare the New LogFile name based on :         yyyy/mm /dd/hhmmss__.log
	sprintf(logFilename, "20%02d/%02d/%02d/%02d%02d%02d%s.log" , rtc.getYear(), rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), StatusList[Vehicle.state].logextension);
	Serial.println("-- >New Log File filename: " + String(logFilename));

	// Create the new log file with a first line entry with vehicle name AND filename
	
	RW_spi.sdpacketwrite=true;
	logFile.close();
	sd.mkdir(logFilePath);
	if (logFile.open(logFilename, O_CREAT | O_WRITE)) {
		logFile.print(Vehicle.id);
		logFile.print("/");
		logFile.println(logFilename);
		
		StaticJsonBuffer<256> jsonBufferCalib;
		JsonObject& JSONcalib = jsonBufferCalib.createObject();
		JSONcalib["CalibA0"]=Vehicle.calibA0;
		JSONcalib["CalibA1"]=Vehicle.calibA1;
		JSONcalib["CalibA2"]=Vehicle.calibA2;
		JSONcalib["CalibA3"]=Vehicle.calibA3;
		JSONcalib["offsetA1"]=Vehicle.offsetA1;
		JSONcalib["offsetA2"]=Vehicle.offsetA2;
		JSONcalib["A0"]=Vehicle.A0;
		JSONcalib["A1"]=Vehicle.A1;
		JSONcalib["A2"]=Vehicle.A2;
		JSONcalib["A3"]=Vehicle.A3;
		JSONcalib.printTo(tempbuffer,jsonBufferCalib.size());
		logFile.println("logfilecreate");
		logFile.println(tempbuffer);
		logFile.println("-------------");
		
		logFile.flush();
	}
	else (Serial.println("ERROR : New log file could not be created"));
	logFile.close();
	
	// Add this file entry into the list of files to post
	if (listfilestopost.open("/logfiles.lst",  O_RDWR | O_CREAT | O_AT_END)) {
		listfilestopost.seekSet(listfilestopost.fileSize());
		listfilestopost.println(String(logFilename));
		//listfilestopost.seekSet(0);
		//cout <<F("\tList of every Files to be posted\n");
		//while (listfilestopost.available()) Serial.write(listfilestopost.read());
		Serial.println("Added to the post list");
	} else {
		Serial.println("Could not open logfiles.lst to append new logfile entry");
	}
	listfilestopost.close();
	
	
	logFile.open(logFilename, O_RDWR);
	logFile.seekSet(logFile.fileSize());
	httpJsonPostEvent(PostDataURL,"NewLogfilename",logFilename);
	RW_spi.sdpacketwrite=false;
	Serial.println("Opened in append mode");
	
	
}

void ISRcan() {
	if (!RW_spi.canpacketread) {
		readCAN();
	}
}

