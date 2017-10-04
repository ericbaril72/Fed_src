/*
 * oldsnippets.c
 *
 * Created: 2016-11-25 10:53:19
 *  Author: EricB
 */ 
/*
void adc_init(){
	analogRead(pinV_Batt);  // do some pin init  pinPeripheral()
	
	ADC->CTRLA.bit.ENABLE = 0x00;             // Disable ADC
	ADCsync();
	//ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC0_Val; //  2.2297 V Supply VDDANA
	//ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain select as 1X
	ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_DIV2_Val;  // default
	ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC1_Val;
	ADCsync();    //  ref 31.6.16
	ADC->INPUTCTRL.bit.MUXPOS = g_APinDescription[ADCPIN].ulADCChannelNumber;
	ADCsync();
	ADC->AVGCTRL.reg = 0x00 ;       //no averaging
	ADC->SAMPCTRL.reg = 0x00;  ; //sample length in 1/2 CLK_ADC cycles
	ADCsync();
	ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV16 | ADC_CTRLB_FREERUN | ADC_CTRLB_RESSEL_10BIT;
	ADCsync();
	ADC->CTRLA.bit.ENABLE = 0x01;
	ADCsync();
}
*/

// see this example :
// https://github.com/manitou48/ZERO/blob/master/adcdma.ino

/*
void adc_dma(void *rxdata,  size_t hwords) {
	uint32_t temp_CHCTRLB_reg;

	DMAC->CHID.reg = DMAC_CHID_ID(chnl);
	DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << chnl));
	temp_CHCTRLB_reg = DMAC_CHCTRLB_LVL(0) |
	DMAC_CHCTRLB_TRIGSRC(ADC_DMAC_ID_RESRDY) | DMAC_CHCTRLB_TRIGACT_BEAT;
	DMAC->CHCTRLB.reg = temp_CHCTRLB_reg;
	DMAC->CHINTENSET.reg = DMAC_CHINTENSET_MASK ; // enable all 3 interrupts
	dmadone = 0;
	descriptor.descaddr = 0;
	descriptor.srcaddr = (uint32_t) &ADC->RESULT.reg;
	descriptor.btcnt =  hwords;
	descriptor.dstaddr = (uint32_t)rxdata + hwords*2;   // end address
	descriptor.btctrl =  DMAC_BTCTRL_BEATSIZE_HWORD | DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_VALID;
	memcpy(&descriptor_section[chnl],&descriptor, sizeof(dmacdescriptor));

	// start channel
	DMAC->CHID.reg = DMAC_CHID_ID(chnl);
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
}

static void   ADCsync() {
	while (ADC->STATUS.bit.SYNCBUSY == 1); //Just wait till the ADC is free
}

void DMAC_Handler() {
	// interrupts DMAC_CHINTENCLR_TERR DMAC_CHINTENCLR_TCMPL DMAC_CHINTENCLR_SUSP
	uint8_t active_channel;

	// disable irqs ?
	__disable_irq();
	active_channel =  DMAC->INTPEND.reg & DMAC_INTPEND_ID_Msk; // get channel number
	DMAC->CHID.reg = DMAC_CHID_ID(active_channel);
	dmadone = DMAC->CHINTFLAG.reg;
	DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL; // clear
	DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TERR;
	DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_SUSP;
	__enable_irq();
}

//adc_init();
//dma_init();

void dma_init() {
	// probably on by default
	PM->AHBMASK.reg |= PM_AHBMASK_DMAC ;
	PM->APBBMASK.reg |= PM_APBBMASK_DMAC ;
	NVIC_EnableIRQ( DMAC_IRQn ) ;

	DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
	DMAC->WRBADDR.reg = (uint32_t)wrb;
	DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);
}

*/



/*  older code stuff */
	
				//sprintf(tempbuffer+strlen(tempbuffer) ,",\"Test\":\"123\"",gps.GGA);
					
				//sprintf(tempbuffer+strlen(tempbuffer) ,",\"CanMsg\":{");
					/*
					if (CanBuff.idx>=0){
						for (int l=1;l<=CanBuff.idx;l++){
							if (l>1) sprintf(tempbuffer+strlen(tempbuffer),",");
							sprintf(tempbuffer+strlen(tempbuffer),"\"%x\":\"%02X",CanBuff.id[l].msgId,CanBuff.id[l].msg[0]);
							for (int j=1;j<8;j++) sprintf(tempbuffer +strlen(tempbuffer),";%02X",CanBuff.id[l].msg[j]);
							sprintf(tempbuffer+strlen(tempbuffer),"\"");
						}
					}
				//sprintf(tempbuffer+strlen(tempbuffer),"}");  // end of CAN  entry
						
				sprintf(tempbuffer+strlen(tempbuffer),",\"gps\":{");
					if (gps.valid) {
						sprintf(tempbuffer+strlen(tempbuffer),"\"gga\":\"%s\"",gps.GGA);
						sprintf(tempbuffer+strlen(tempbuffer),",\"vtg\":\"%s\"",gps.VTG);
					}
				sprintf(tempbuffer+strlen(tempbuffer),"}");  // end of gps entry
				
				
				
				char tempval[10];
				sprintf(tempbuffer+strlen(tempbuffer),",\"State\":{}",Vehicle.state);
				
				dtostrf(Vehicle.speed, 2, 1, tempval);
				
				sprintf(tempbuffer+strlen(tempbuffer),",\"Speed\":\"%s\"",tempval);
				
				
				dtostrf(Vehicle.V_Batt, 2, 1, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"VBatt\":\"%s\"",tempval);
					
				dtostrf(Vehicle.A_CurrentA, 4, 1, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"AmpsA\":\"%s\"",tempval);
				dtostrf(Vehicle.A_CurrentB, 4, 1, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"AmpsB\":\"%s\"",tempval);
				dtostrf(Vehicle.power, 4, 0, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"power\":\"%s\"",tempval);
				dtostrf(Vehicle.EnergyIN, 6, 0, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"EnergyIN\":\"%s\"",tempval);
				dtostrf(Vehicle.EnergyOUT, 6, 0, tempval);
				sprintf(tempbuffer+strlen(tempbuffer),",\"EnergyOUT\":\"%s\"",tempval);
					Serial.print("tempval-->");
					Serial.print(tempval);
					Serial.println("<--");
				
				sprintf(tempbuffer+strlen(tempbuffer),"}}"); // end of status and JSON object list
				*/
					
				//Serial.println("Posting this data ("+String(strlen(tempbuffer))+" Bytes) :"+tempbuffer);
				//Serial.println("------");
				//postdata(PostDataURL,tempbuffer);
				
				/*
				
				#define ADCPIN A1
				#define HWORDS 16
				uint16_t adcbuf[HWORDS];

				typedef struct {
					uint16_t btctrl;
					uint16_t btcnt;
					uint32_t srcaddr;
					uint32_t dstaddr;
					uint32_t descaddr;
				} dmacdescriptor ;
				volatile dmacdescriptor wrb[12] __attribute__ ((aligned (16)));
				dmacdescriptor descriptor_section[12] __attribute__ ((aligned (16)));
				dmacdescriptor descriptor __attribute__ ((aligned (16)));
				static uint32_t chnl = 0;  // DMA channel
				volatile uint32_t dmadone;

				while(0){
					digitalWrite(13, ledState=!ledState);
					
					Serial.print("\f");
					//t = micros();
					adc_dma(adcbuf,HWORDS);
					Serial.print("DMA called:");
					
					while(!dmadone);  // await DMA done isr
					//t = micros() - t;
					//Serial.print(t);  Serial.println(" us   ");
					Serial.print("ADC:");
					Serial.println(ADC->RESULT.reg);
					for (int l=0;l<HWORDS;l++){
						Serial.println(String(l)+":"+String(adcbuf[l]));
					}
					//dmadone=1;
					delay(250);
				}*/
				
/*
void poststatusupdate(char * someeventinfo){
	tempbuffer[0]='\0';
	sprintf(tempbuffer ,"?{\"status\":{\"id\":\"%02X%02X%02X%02X%02X%02X\"",
		Vehicle.mac[0],Vehicle.mac[1],Vehicle.mac[2],
		Vehicle.mac[3],Vehicle.mac[4],Vehicle.mac[5]);

		sprintf(tempbuffer+strlen(tempbuffer) ,",\"Event\":{%s}",someeventinfo);
	sprintf(tempbuffer+strlen(tempbuffer),"}}"); // end of status and JSON object list
	Serial.print("Final URL:");
	Serial.print(tempbuffer);
	postdata(PostDataURL,tempbuffer);
	RW_spi.sdpacketwrite=true;
	logFile.println(tempbuffer);
	logFile.flush();
	RW_spi.sdpacketwrite=false;
}*/
/*
void printtime() {
	char tempbuffer[40];
	sprintf(tempbuffer, "M0 Date: 20%02d/%02d/%02d/%02dh%02d:%02ds" ,
	rtc.getYear(),
	rtc.getMonth(),
	rtc.getDay(),
	rtc.getHours(),
	rtc.getMinutes(),
	rtc.getSeconds());
	PrintLCD(tempbuffer);
	//Serial.println(tempbuffer);
}

void poststatusupdate(char * someeventinfo){
	tempbuffer[0]='\0';
	sprintf(tempbuffer ,"?{\"status\":{\"id\":\"%s\"",Vehicle.Name);

		sprintf(tempbuffer+strlen(tempbuffer) ,",\"Event\":{%s}",someeventinfo);
	sprintf(tempbuffer+strlen(tempbuffer),"}}"); // end of status and JSON object list
	Serial.print("Final URL:");
	Serial.print(tempbuffer);
	postdata(PostDataURL,tempbuffer);
	RW_spi.sdpacketwrite=true;
	logFile.println(tempbuffer);
	logFile.flush();
	RW_spi.sdpacketwrite=false;
}
*/

/*
struct {
	schedule_info serialdump;
	schedule_info gpsrefresh;
	schedule_info poststatus;
	schedule_info refreshlcd;
}scheduler;
*/
//0x333,0x217,0x22E,0x1AE,0x200,0x202,0x100,0x150
//int CanIdTxList[20] = {0x333,0x217,0x22E,0x1AE,0x200,0x202,0x100,0x150};
//int CanIdLogList[20]= {0x333,0x217,0x22E,0x1AE,0x200,0x202,0x100,0x150};
//HttpURL PostDataURL = {"192.168.3.116",8080};
//SRouter wifi = {"MOTREC","mot8761234567"};
//SRouter wifi = {"test","testtest"};
//HttpURL PostDataURL = {"192.168.0.128",8080};
//HttpURL PostDataURL = {"192.168.13.1",8080}	;
	
//	void prepStatusData()  { 
	
	//WiFi.macAddress(Vehicle.mac);
	//String loopdata="";
	//char buffer[280];
	//char tempbuffer[40];
	// should be called "generate the status string for FILE + cloud"	
	//Serial.print("Posting Status : ");
	
	
	//loopdata+=buffer;
	//loopdata += "STATUS=1.2:"+ String(millis());
	//sprintf(buffer + strlen(buffer),";VehicleName=%c",Vehicle.Name);
	//loopdata+=buffer;
	//loopdata += ";VehicleName=" ;//+ String(Vehicle.Name);
	/*sprintf(buffer ,"?{\"json\":{\"id\":\"%02X%02X%02X%02X%02X%02X\"",
		Vehicle.mac[0],Vehicle.mac[1],Vehicle.mac[2],
		Vehicle.mac[3],Vehicle.mac[4],Vehicle.mac[5]);
		sprintf(Vehicle.Name ,"MAC:%02X%02X%02X%02X%02X%02X",
			Vehicle.mac[0],Vehicle.mac[1],Vehicle.mac[2],
			Vehicle.mac[3],Vehicle.mac[4],Vehicle.mac[5]);
		*/	
	//sprintf(buffer+ strlen(buffer),";STATUS=1.2:%d",millis());
	//loopdata += String(buffer);
	//sprintf(buffer + strlen(buffer),";State=%s",Vehicle.state);
	//loopdata += ";VehicleState=" + Vehicle.state;
	//Vehicle.speed = Vehicle.rpmcnt * 1;	//Vehicle.rpmratio;
	//dtostrf(Vehicle.speed, 3, 1, tempbuffer);
	//loopdata += ";Speed=" + String(buffer);

	//char tempval[6];
	//dtostrf(Vehicle.V_Batt_Avg, 0, 1, tempval);
	
	//dtostrf(Vehicle.V_Batt_Avg, 3, 1, tempval);
	//sprintf(buffer + strlen(buffer),",\"status\":{\"V_Batt\":\"%s\"",tempval);
	//sprintf(buffer + strlen(buffer),",\"fw_size\":\"%d\"",Vehicle.lastfilesize);
	//sprintf(buffer + strlen(buffer),"}}}");
	//loopdata += ";Vbackup=" + String(tempval);
	//dtostrf(Vehicle.V_ksi_Avg, 3, 1, tempval);
	//loopdata += ";Vksi=" + String(tempval);
	//loopdata += ";CANcnt=" + String(Vehicle.cancnt)	+ ";sdaccess=" + String("sdfileaccess");

	
	//Serial.println("ready for wifi!");
	/*
	RW_spi.rfpacketsend = true;
	if (WiFi.status() != WL_CONNECTED) {
		//PrintLCD("Not connected, calling SDCOnfigConnect");
		SDconfigConnectWifi();
	}*/
	//Serial.println("ready to post:");
	
	
	
	//if (1 && (WiFi.status() == WL_CONNECTED)) { 
		//if(1){
		//if Wifi Connected , send vehicleStatus POST
		//loopdata += ";Wifi_Level=" + String(WiFi.RSSI());
		//char charloopdata[loopdata.length() + 1];
		//loopdata.toCharArray(charloopdata, loopdata.length() + 1);
	
	
/*		RW_spi.rfpacketsend = true;
		Serial.println("Posting result:"+String(postdata(PostDataURL,buffer)));
		Serial.println(buffer); 
		RW_spi.rfpacketsend = false;
	*/	
		//char char2loopdata[loopdata.length() + 1];
		//loopdata.toCharArray(char2loopdata, loopdata.length() + 1);
		//loopdata.print("updating log file wityh post results");
		
		
	//}
	
	//if(0) { //else {
		//Serial.println("couldn't post updating results !");
		//loopdata += ";ServerAck=NA;Wifi_Level=Not_Connected";
		//Serial.println("    Not connected");
		//char char2loopdata[loopdata.length() + 1];
		//loopdata.toCharArray(char2loopdata, loopdata.length() + 1);
		//Serial.println("String converted !");
		//
		//updateLogFile(char2loopdata);
		//Serial.println("updated log file of not postted");
		//connectRouter();
		//WifiAvail();
	//}
	//Serial.println(" out ");
//}
/*

void oldupdateVehicleStatus() {
	// Vehicule Status depens on logger architecture
	//	Curtis DC Lithium - Curtis AC Lithium - Hybrid ( wt Danfoss ) - Curtis DC - Curtis AC  - Sevcom
	
	//Vehicle.V_Batt_Avg 		= Vd12_228k		* analogRead(pinV_Batt);
	//Vehicle.A_Current_Avg	= Vd21_33k		* analogRead(pinV_Current_sensor);
	//Vehicle.V_ksi_Avg 		= Vd50k		* analogRead(pinV_ksi);
	//Vehicle.V_Charger1_Avg	= Vd50k		* analogRead(pinV_Charger1);
	
	
	//Vehicle.V_Charger2_Avg	= Vd50k		* analogRead(pinV_Charger2);
	//Vehicle.V_Backup_Avg	= VbackupVD	* analogRead(pinV_Backup);
	

	if (0) {		// when no VD are installed ... until based on SD Config file
		Vehicle.V_Batt_Avg = 			1 * analogRead(pinV_Batt);
		if (Vehicle.V_Batt_Avg < 100) {
		Vehicle.state = 6;}
		else if (Vehicle.V_Batt_Avg < 200) {
		Vehicle.state = 7;	}
		else if (Vehicle.V_Batt_Avg < 400) {
		Vehicle.state = 3;	}
		else if (Vehicle.V_Batt_Avg < 600) {
		Vehicle.state = 3;	}
		else if (Vehicle.V_Batt_Avg < 800) {
		Vehicle.state = 1;	}
		else {   Vehicle.state = 2; 	}
	}
	if(0) {
		if (Vehicle.V_Batt_Avg < 5) {
			//Serial.println("     No V Batt present ... ");
			if ( Vehicle.V_Backup_Avg < 3.5)
			Vehicle.state = V_LOW_BACKUP_BATT;
			else 	Vehicle.state = V_LOW_BACKUP_BATT;
		}
		// Charging mode
		else if (Vehicle.V_Charger1_Avg < 10 && Vehicle.V_ksi_Avg > 10)
		Vehicle.state = V_CHARGER1;
		else if (Vehicle.V_Charger1_Avg < 10)
		Vehicle.state = V_CHARGER1;
		else if (Vehicle.V_ksi_Avg < 10)
		Vehicle.state = V_OFF;
		else Vehicle.state = V_ON;
	}
	// ON Vehicle state change , do some tasks !!!
	if (Vehicle.oldstate != Vehicle.state) {
		Vehicle.oldstate = Vehicle.state;
		Serial.print("-- STATE CHANGE : " + String(StatusList[Vehicle.state].desc) + " -- ");
		
		//logFile.close();
		createLogFile();
		//CAN0.sendMsgBuf(0x150, 1, 8, SamsungDump);
		
		switch (	Vehicle.state) {
			case 0:		//VStatus   0- unknown
			Serial.println
			/*
			int postJSON1(HttpURL URL, char * somedata, char * posttype) {
				if (WifiAvail()==false) return false;
				RW_spi.rfpacketsend=true;

				//Serial.print(strlen(somedata));
				//Serial.println(" Bytes");
				//Serial.println(somedata);
				//JSONstatus.printTo(Serial);
				//Serial.println();
				char ltempbuffer[300];
				char getparams[50];
				
				
				if (String(posttype).startsWith("status")) {
					sprintf(getparams,"GET /statusupdate/%s?{\"status\":",Vehicle.id);
						//Serial.print("starts with status:");
						//Serial.println(getparams);
						} else if (String(posttype).startsWith("event")) {
						sprintf(getparams,"GET /statusupdate/%s?{\"event\":",Vehicle.id);
							//Serial.print("starts with status");
							//Serial.println(getparams);
							} else {
							sprintf(getparams,"GET /statusupdate/%s?{\"%s\":",Vehicle.id,posttype);
								//Serial.print("starts with status");
								//Serial.println(getparams);
							}
							
							int returncode = 0;
							client.stop();
							if (client.connect(URL.host, URL.port)) {
								client.print(getparams);
								client.print(somedata);
							client.print("}");
							
							
							client.println(" HTTP/1.1");
							client.print("Host: ");
							client.println(URL.host);
							client.println("Connection: close");
							client.println();
							
							int timeout = 100;
							String linebuf="";
							while(client.connected() && timeout>0){
								if (client.available()) {
									char c = client.read();
									timeout =20;
									linebuf += c;
									if (c == 13) {
										//Serial.println(linebuf);
										if (linebuf.startsWith("200", 9)) returncode = 1;
										//else if (linebuf.startsWith("201 CREATED", 9))  returncode = 1;
										//else if (linebuf.startsWith("MCResult:", 0)) {;}
										linebuf = "";
									}
								}
								else delay(10);
								timeout--;
							}
						}
						
						client.stop();
						RW_spi.rfpacketsend=false;
						return returncode;
					}

("Error with VStatus !!!");
			break;
			case 1:		// VStatus   1-OF= OFF,
			Serial.println("ON");
			//postfromfilelist();
			//deepsleep();				// Goto Sleep until next RTC or Wake Signal !!!
			//sleepforXminutes(0,1,0);
			break;
			case 2:			// VStatus   2-On= ON
			Serial.println("ON");
			break;
			case 3:			// VStatus   3-C1 = Charger 1
			Serial.println("Charging 1");
			// statements
			break;
			case 4:			// VStatus   4-C2=Charger 2
			
			Serial.println("Charging 2");
			// statements
			break;
			case 5:			// VStatus   5-C3 = Both Chargers
			Serial.println("Fast Charge");
			break;
			case 6:			// VStatus   7-ds= DeepSleep
			Serial.println("Deep Sleep");
			//postfromfilelist();
			//sendCommand("sleep=1");
			//deepsleep();
			//digitalWrite(WINC_EN, LOW);
			//sleepforXminutes(0,3,0);
			// statements
			break;
			default:
			Serial.println("--Error on Vehicle Status got the default statement");
			// statements
			break;
		}
	}
}
*/

/*
void checkforupdate(){
	if (millis()<lastfilecreated) return;
	lastfilecreated = millis()+30000;
	Serial.println("--- checking for update");
	if (!WifiAvail()) {
		Serial.println("Wifi  Not connected");
		return;
	}
	if (DL_fct[DL_Wifi].active!=FCT_ACTIVE) {
		Serial.println("Wifi  Not connected");
		return;
	}
	
	if (!newFWfile) {
		if(checkfornewFW_dl()) {
			Serial.println("\t->New firmware to DL");
			
			//RefreshLCD(0);
			if (downloadFile("","tempfw.bin","/download/bin/1.0/MDL.bin")){
				SdFile tempfile;
				tempfile.open("tempfw.bin");
				if (tempfile.fileSize()>50000){
					RW_spi.sdpacketwrite=true;
					sd.remove("fw.bin");
					sd.rename("tempfw.bin","fw.bin");
					RW_spi.sdpacketwrite=false;
					Serial.println("    ->Success !\r\n");
					newFWfile=true;	// this will call reset Comment OUT if you want
				}
			}
			display_screen.spinner=false;
			//RefreshLCD(0);
		}
		else Serial.println("  ->No New firmware to DL\r\n");
		lastfilecreated = millis()+30000;
	}
	if(newFWfile && checkforbootloading_exec()) {
		Serial.println("    Ready to update, calling reset in 5 seconds!");
		Serial.println("-----------------------------------------------");
		display_screen.looprun=false;
		display_screen.spinner=true;
		//RefreshLCD(0);
		logFile.close();
		NVIC_SystemReset();
		
	}
	
	//else Serial.println("    New FW loaded, awaiting Server Bootload command");
	
}


*/

/*int postdata1(HttpURL URL,char * somedata) {
if (WifiAvail()==false) return false;
RW_spi.rfpacketsend=true;
Serial.print("Posting: ");
Serial.print(strlen(somedata));
Serial.println(" Bytes");
Serial.println(somedata);

//Serial.println("\tWifi is availale, connecting to server...");
int returncode = 0;
client.stop();
if (client.connect(URL.host, URL.port)) {
//Serial.print("\tConnected, sending: ");
//Serial.print("/m0client?");
//Serial.print(somedata);
//Serial.print("-------");
client.print("GET ");
client.print("/m0client?");
client.print(somedata);
client.println(" HTTP/1.1");
client.print("Host: ");
client.println(URL.host);
client.println("Connection: close");
client.println();
//Serial.print("Waiting for answer");
//digitalWrite(13, HIGH);
int timeout = 100;
String linebuf="";
while(client.connected() && timeout>0){

if (client.available()) {
char c = client.read();
timeout =20;
linebuf += c;
if (c == 13) {
//Serial.println("ServerAnswer: "+String(linebuf));
//Serial.println(linebuf);
if (linebuf.startsWith("201 CREATED", 9))  returncode = 1;
else if (linebuf.startsWith("200", 9)) returncode = 1;
else if (linebuf.startsWith("MCResult:", 0)) {;}

linebuf = "";
}
}
else {
delay(10);
Serial.print(".");
}
timeout--;

}
//Serial.println("ServerAnswer: "+String(linebuf));
//Serial.print("No conecction or timeout:"+String(timeout));
if (!returncode) {
Serial.println("RETURN code:-1");
returncode = -1;
//loopdata += ";ServerAck=OK";
//DL_fct[DL_SERVER].status=FCT_ACTIVE;
}

}
//else {
//loopdata += ";ServerAck=No_connection";
//DL_fct[DL_SERVER].status=FCT_FAILURE;
//}
//Serial.println("Closing Connection");
client.stop();
if (!client.connected()) {
//digitalWrite(13, LOW);
//loopdata += ";ServerAck=Connection_Timeout";
client.stop();
//Serial.println("Closing Connection(if)");
}
//Serial.println("Done with posting");
//digitalWrite(13,LOW);
RW_spi.rfpacketsend=false;
return returncode;
}

*/

/*
bool fileDownload1(String  filePath, String filename, String sourceaddress){
	int tempint=0;
	#ifdef DEBUG_WIFI
	Serial.println("--- Downloading file ---"+sourceaddress);
	#endif
	
	long int inittime=millis();
	char buffer[50];
	char filesize[15];
	String linebuf;
	char c;
	int jcnt=0;
	SdFile dlfile;
	long int fsize=0;
	int timeout;
	//filePath="files";
	char charFilePath[filePath.length()+1];
	filePath.toCharArray(charFilePath,filePath.length()+1);
	char charpathfilename[filePath.length()+filename.length()+2];
	bool filepayload=false;
	bool fileOK=false;
	
	sprintf(charpathfilename,"%s/%s",filePath.c_str(),filename.c_str());
	//Serial.print("\t");
	Serial.print(charpathfilename);
	
	// create destination directory
	Serial.println("Make DIR:"+String(charFilePath));
	RW_spi.sdpacketwrite=true;
	RW_spi.rfpacketsend=true;
	Serial.println("Make dir: "+String(sd.mkdir(charFilePath)));
	
	
	//memcpy(pathfilename,filePath,sizeof(filePath));
	//Serial.println("\tDownload File to SD:"+String(charpathfilename));
	Serial.println("\tFilePath:"+String(filePath));
	Serial.println("\tFilename:"+String(filename));
	//Serial.println("\tSource URL:"+String(sourceaddress));
	
	
	//Serial.println("\nStarting connection to server:8080");
	
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		//Serial.println("\tConnected to: " + String(sourceaddress));
		//Serial.println("\tFilepathname: "+String(charpathfilename));
		
		// Make a HTTP request:
		client.print("GET ");
		client.print(sourceaddress);
		client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();
		//Serial.print("\tRequest Sent, awaiting RX  ");
		delay(250);
		timeout=millis()+5000;
		while(!client.available() && millis()<timeout) {
			delay(1000);
			Serial.print(".");
		}
		if (client.available()){
			//Serial.println("\nServer is responding !");
			//Serial.print("\nDownloading File :"+String(filename));
			//dlfile.open(charpathfilename, O_RDWR | O_CREAT);
			//dlfile.remove();
			//dlfile.close();
			
			//Serial.print("\nRemoved pre-existing File !");
			//dlfile.open("/binaries/fw1.bin", O_RDWR | O_CREAT );
			dlfile.open(charpathfilename, O_RDWR | O_CREAT);
			
			//dlfile.remove();
			//dlfile.close();
			//dlfile.open(charpathfilename, O_RDWR | O_CREAT);
			//dlfile.open("/binaries/fw1.bin", O_RDWR | O_CREAT );
			//Serial.println("\nCreating new file");
			
			
			//bool postheader=true;

			// Strip the HTTP answer<s HEADER Info and find file size
			while (client.available() && !filepayload) {
				c = client.read();
				linebuf+=c;

				if (!filepayload){
					if (c==13){
						client.read();
						if(linebuf.startsWith("Content-Length: ",0)) {
							linebuf.substring(15).toCharArray(filesize, linebuf.substring(15).length());
							fsize = atoi(filesize);
							Serial.print("\tFile Size: "+String(fsize)+" kB");
						}
						if ((fsize>0) && (linebuf.length()==1)) filepayload=true;
						linebuf="";
					}
					
				}
				timeout=millis()+5000;
				while(!client.available() && millis()<timeout) {
					delay(100);
					Serial.println(".");
				}
			}
			
			// Get the file Payload as binary
			while (client.available()) {
				// Direct PAYLOAD to put in file
				if (jcnt<fsize) {
					tempint=min(client.available(),1024);
					client.read(databuf,tempint);
					jcnt+=tempint;
					dlfile.write(databuf,tempint);
				}
				else client.read();
				timeout=millis()+5000;
				while(!client.available() && millis()<timeout && jcnt<fsize) {
					delay(250);
					#ifdef DEBUG_WIFI
					Serial.print("\t");
					Serial.print(jcnt/1024);
					Serial.println(" kB rcvd");
					#endif
				}
			}
			
			//dlfile.flush();
			
			float deltatime =(millis()-inittime);
			char rate[6];
			char timesecs[6];
			dtostrf(fsize/deltatime,0,0,rate);
			dtostrf(deltatime/1000,5,1,timesecs);
			Serial.println("\t\tTransfer Rate: "+String(rate)+" KB/s: "+String(timesecs)+"(s)");
			// do nothing forevermore:
			
			if(fsize<=233)	Serial.println("File Not available ("+String(fsize)+"B)");
			else if (dlfile.fileSize()!=fsize) 	Serial.println("\tError: transfer timeout");
			else {
				fileOK=true;
				Serial.println("\tFile OK !");
				//sd.remove("");
				//sd.rename("tempdl.par","fw.bin");
			}
			if(!fileOK) dlfile.remove();
			else Vehicle.lastfilesize=fsize;
			dlfile.close();
			
		}
		else Serial.println("\tServer Timeout !");
		client.flush();
		client.stop();
		client.flush();
	}
	
	else Serial.println("\tCould not CONNECT ");
	
	RW_spi.sdpacketwrite=false;
	RW_spi.rfpacketsend=false;
	return fileOK;
}

*/


/*
void printTemp(){
	
	while (ds.search(addr)) {
		//Serial.println("No more addresses.");
		//Serial.println(millis());
		//Serial.println(Vehicle.speed);
		//Serial.println(Vehicle.rpmcnt);
		
		//delay(250);
	
		Vehicle.TempS_idx++;
		Serial.print("ROM =");
		for( i = 0; i < 8; i++) {
			Serial.write(' ');
			Serial.print(addr[i], HEX);
		}
		if (OneWire::crc8(addr, 7) != addr[7]) {
			Serial.println("CRC is not valid!");
			return;
		}
		
		//Serial.print("Data =");
		for ( i = 0; i < 9; i++) {           // we need 9 bytes
			data[i] = ds.read();
			//Serial.print(data[i], HEX);
			//Serial.print(" ");
		}
		present = ds.reset();
		ds.select(addr);
		ds.write(0xBE);         // Read Scratchpad

		//Serial.print("  Data = ");
		//Serial.print(present, HEX);
		//Serial.print(" ");
		for ( i = 0; i < 9; i++) {           // we need 9 bytes
			data[i] = ds.read();
			//Serial.print(data[i], HEX);
			//Serial.print(" ");
		}
		//Serial.print(" CRC=");
		//Serial.print(OneWire::crc8(data, 8), HEX);
		//Serial.println();

		// Convert the data to actual temperature
		// because the result is a 16 bit signed integer, it should
		// be stored to an "int16_t" type, which is always 16 bits
		// even when compiled on a 32 bit processor.
		int16_t raw = (data[1] << 8) | data[0];
		if (type_s) {
			raw = raw << 3; // 9 bit resolution default
			if (data[7] == 0x10) {
				// "count remain" gives full 12 bit resolution
				raw = (raw & 0xFFF0) + 12 - data[6];
			}
			} else {
			byte cfg = (data[4] & 0x60);
			// at lower res, the low bits are undefined, so let's zero them
			if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
			else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
			else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
			//// default is 12 bit resolution, 750 ms conversion time
		}
	
	celsius = (float)raw / 16.0;
	Serial.print(celsius);
	Serial.println(" Celsius, ");
	
	if (Vehicle.TempS_idx==1) Vehicle.TempS1 = celsius;
	else if (Vehicle.TempS_idx==2) Vehicle.TempS2 = celsius;
	
	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);        // start conversion, with parasite power on at the end
	}
	
	ds.reset_search();
	Vehicle.TempS_idx=0;
	return;
	
}

*/
/*
int patchfiledownloadcnt1(HttpURL URL,int Id, int cnt) {
	RW_spi.rfpacketsend=true;
	String patchstring = "PATCH /api/v1/dlfilelist/"+String(Id)+" HTTP/1.1\r\n";
	patchstring += "Host: "+String(URL.host)+":"+String(URL.port)+"\r\n";
	patchstring += "Content-Type: application/json\r\n";
	patchstring += "\r\n";
	patchstring += "{\"Downloaded\":"+String(cnt)+"}\r\n";
	//if (WifiAvail()==false) return false;
	//Serial.println("     - Wifi is availale");
	
	//Serial.print("Trying to Patch: ");
	int returncode = 0;
	//Serial.println(patchstring);
	//Serial.println("------------------------");
	if (client.connect(URL.host, URL.port)) {
		client.print(patchstring);
		
		//digitalWrite(13, HIGH);
		int timeout = 100;
		while(client.connected() && timeout>0){
			String linebuf="";
			if (client.available()) {
				char c = client.read();
				//Serial.print(c);
				timeout =20;
				linebuf += c;
				if (c == 13) {
					
					linebuf = "";
				}
			}
			timeout--;
			delay(1);
		}
		if (!returncode) {
			returncode = -1;
		}
		
	}
	
	if (!client.connected()) {
		client.stop();
	}
	Serial.println();
	//digitalWrite(13,LOW);
	RW_spi.rfpacketsend=false;
	return returncode;
}
*/
void SDconfigConnectWifi1()/* to rewrite */ { /*
	if (DL_fct[DL_Wifi].active==FCT_DISABLE) return;


	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Searching Config file...");
		digitalWrite(WINC_EN, HIGH);
		PrintLCD("Searching config file");
		if (!configFile.open("/params.conf", O_READ)) {
			sd.errorHalt("opening config failed");
		}

		ifstream sdin("/params.conf");
		//Serial.println("---------------------------------------------------");
		Serial.println("Wifi Connection:");

		const int line_buffer_size = 50;
		char buffer[line_buffer_size];
		bool networkinfo = false;
		PrintLCD("Loading server list");
		while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
			if (WiFi.status() == WL_CONNECTED) break;
			char* separator = strchr(buffer, '=');
			if (separator != 0) {
				*separator = 0;
				++separator;
				if (String(buffer).substring(0) == "network") {
					networkinfo = true;
				}
				else if (String(buffer).substring(0) == "ssid") {
					//strcpy(fssid,separator);
					for (uint i = 1; i+1 < strlen(separator); ++i) {
						ssid[i - 1] = separator[i];
						ssid[i] = '\0';
					}
				}
				else if (String(buffer).substring(0) == "pass") {
					//strcpy(fpass,separator);
					pass[0] ='\0';
					for (uint i = 1; i < strlen(separator) - 1; ++i) {
						pass[i - 1] = separator[i];
						pass[i] ='\0';
					}
				}
				else {
					//Serial.print("    New Unparsed variable-->");
					//Serial.print(buffer);
					if (strcmp(buffer, "VehicleName") == 0) {
						strncpy(V_status.VehicleName, separator + 1, strlen(separator) - 2);
						V_status.VehicleName[strlen(separator) - 2] = '\0';
						sprintf(buffer,"Vehiclename: %s",V_status.VehicleName);
						PrintLCD(buffer);
					}
					if (strcmp(buffer, "ServerName") == 0) {
						strncpy(server, separator + 1, strlen(separator) - 2);
						server[strlen(separator) - 2] = '\0';
						sprintf(buffer,"Server Name: %s",server);
						PrintLCD(buffer);
					}
					//Serial.println("<-");
				}
			}

			if ((buffer[0] == '}') && networkinfo) {
			// Closing '}' of the Network parameters
			// if not connected !
			if (WiFi.status() != WL_CONNECTED) connectRouter();
			else break;
		}
	}
	configFile.close();
	*/
}
/*
bool checkfornewFW_dl1(){
	String linebuf="";
	bool needDL = false;
	int timeout;
	RW_spi.rfpacketsend=true;

	if (client.connect(PostDataURL.host, PostDataURL.port)){
		client.print("GET "); client.print("/update/");client.print(Vehicle.Name);client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();

		delay(500);
		timeout = millis()+5000;
		while(!client.available() && millis()<timeout) {
			delay(1000);
			Serial.print(".");
		}
		//if (millis()<timeout) Serial.println();
		
		while (client.available()) {
			char c = client.read();
			linebuf+=c;
			if (c==13){
				c=client.read();
				if (linebuf.startsWith("PASS",0)) needDL=true;
				//Serial.println(linebuf);
				linebuf="";
			}
		}
		client.flush();
		client.stop();
		client.flush();
	}
	else  Serial.println("\tCould not connect");
	
	RW_spi.rfpacketsend=false;
	return needDL;
}
bool checkforbootloading_exec1(){
	RW_spi.rfpacketsend=true;
	bool needbootloading=false;
	String linebuf="";
	int timeout;
	char c;
	if (WiFi.status()!=WL_CONNECTED) return false;

	Serial.println("--- Checking for new FW availability ---      ");
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		client.print("GET "); client.print("/callupdate/");client.print(Vehicle.Name);	client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();
		//Serial.print("\tConnection opened, awaiting RX");
		timeout = millis()+5000;
		delay(500);
		while(!client.available() && millis()<timeout) {
			delay(1000);
			Serial.print(".");
		}
		//if (millis()<timeout) Serial.println();
		while (client.available()) {
			c = client.read();
			linebuf+=c;
			
			if (c==13){
				c= client.read();
				if (linebuf.startsWith("PASS",0))
				needbootloading=true;
				//Serial.println(linebuf);
				linebuf="";
			}
		}
		client.flush();
		client.stop();
		client.flush();
		
		} else {
		Serial.println("\tCould not connect");
	}
	
	RW_spi.rfpacketsend=false;
	return needbootloading;
}
bool checkforfiledownloadlist1(){
	char urllink[150]="/api/v1/dlfilelist?q={\"filters\":[{\"name\":\"VehicleName\",\"op\":\"==\",\"val\":\"F8F005F191D0\"}]}";
	// 159.203.59.104:8080/api/v1/dlfilelist?q={"filters":[{"name":"VehicleName","op":"==","val":"F8F005F191D0"}]}
	RW_spi.rfpacketsend=true;
	bool needbootloading=false;
	String linebuf="";
	int timeout;
	char c;
	if (WiFi.status()!=WL_CONNECTED) return false;

	Serial.println("--- Checking for new FW availability ---      ");
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		client.print("GET "); client.print(urllink);	client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();
		//Serial.print("\tConnection opened, awaiting RX");
		timeout = millis()+5000;
		delay(500);
		while(!client.available() && millis()<timeout) {
			delay(1000);
			Serial.print(".");
		}
		//if (millis()<timeout) Serial.println();
		while (client.available()) {
			c = client.read();
			linebuf+=c;
			//if (c!=10) Serial.print(c);
			//else Serial.println();
			if (c==13){
				//Serial.println();
				c= client.read();
				if (linebuf.startsWith("PASS",0))
				//needbootloading=true;
				//Serial.println(linebuf);
				linebuf="";
			}
		}
		client.flush();
		client.stop();
		client.flush();
		
		} else {
		Serial.println("\tCould not connect");
	}
	
	RW_spi.rfpacketsend=false;
	return needbootloading;
}
*/
/*
void listNetworks() {
	// scan for nearby networks:
	RW_spi.rfpacketsend=true;
	int numSsid = WiFi.scanNetworks();
	char buffer[80];
	Serial.println("--- Visible Routers ---");
	if (numSsid == -1) {
		Serial.println("\tCouldn't get a wifi connection");
		} else {
		for (int thisNet = 0; thisNet < numSsid; thisNet++) {
			sprintf(buffer,"\t%d - %s \t%d",thisNet,WiFi.SSID(thisNet),WiFi.RSSI(thisNet));
			PrintLCD(buffer);
			Serial.print("\t");
			Serial.print(thisNet);
			Serial.print(": ");
			bool isallowed=false;
			for (int listelement = 0; listelement <6;listelement++ ){
				if (String(WiFi.SSID(thisNet)).startsWith(wifilist[listelement].ssid)) isallowed=true;
			}
			if (isallowed) Serial.print(" MATCH ");
			Serial.print(WiFi.SSID(thisNet));
			Serial.print("\t= ");
			Serial.println(WiFi.RSSI(thisNet));
			//Serial.flush();
			
		}
	}
	RW_spi.rfpacketsend=false;
}

*/
/*
void postDirectory(){
	FatFile cff,fileToPost;

	sd.vwd()->rewind();
	int filecount = 0;
	int fsizep = 24;
	char filename[40] = "12345678.123";
	
	while (cff.openNext(sd.vwd(), O_READ)) {
		if (!cff.isDir()) {
			cff.getName(filename,fsizep);
			//Serial.print(" is file : ");
			//Serial.println(filename);
			if (!postthisfile(PostDataURL,filename)) Serial.println("\tFailed sending");
			//Serial.println();
			filecount++;
		}
		cff.close();
	}
}
*/