#include "CANbus.h"

//extern DL_fcts DL_fct[];
extern V_status Vehicle;
extern char tempbuffer[1024];
extern canMsgBuffer CanBuff;


MCP_CAN CAN0(CS_CAN0);

void InitCAN(){
	//if (!DL_fct[DL_CAN].active) return;
	//DL_fct[DL_CAN].status=FCT_WARNING;
	//PrintLCD("----- CAN bus init ---------");
			
	
	Serial.println("\tInitCan()");
	
	while (CAN0.begin(CAN0baud) != CAN_OK) {
		//DL_fct[DL_CAN].status=FCT_ERROR;
		PrintLCD("\tCan0 init fail @ ");
		Serial.println("\t@ "+String(millis()));
		delay(500);
		//DL_fct[DL_CAN].status=FCT_WARNING;
		PrintLCD(" Retry ...");
		
		//DL_fct[DL_CAN].status=FCT_DISABLE;
		//DL_fct[DL_CAN].active=FCT_DISABLE;
		return;
	}
	Serial.println("\tSuccess");
	//CAN0.init_Mask( 0, 0, 0x000 );
	//CAN0.init_Mask( 0, 1, 0x000 );
	//CAN0.init_Mask( 1, 0, 0x000 );
	//CAN0.init_Mask( 1, 1, 0x000 );         // Set the 2 masks as CAN Standard
	
	while (!digitalRead(INT_CAN0)) readCAN();		// Clear any pending msg in the MCP2515
	
	if (CAN0.checkError()>0) Serial.println("CAN ERROR on Init");
	attachInterrupt(INT_CAN0, ISRcan, FALLING);
	//DL_fct[DL_CAN].status=FCT_ACTIVE;
	//sendCANStop();
	PrintLCD("\t CAN init success !");
	
}

void sendCANStop(void){
	//unsigned char SamsungStop[8] =  {0x8e, 0, 0, 0, 0, 0, 0, 0};
	int nodeId=0x100;
	unsigned char SamsungStop[8] =  {0xa5, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		
		Serial.println("\t Sending STOP msg!");
		if (CAN0.checkError()>0) Serial.println("\t-CAN ERROR before !");
		
	Serial.print(CAN0.sendMsgBuf(nodeId, 0, 8, SamsungStop));
	if (Serial.print(CAN0.checkError())>0) Serial.println("\t-CAN ERROR  after");
	
	
	delay(1000);
}

bool readCAN() {
	if (RW_spi.rfpacketsend || RW_spi.sdpacketwrite || RW_spi.lcdspiaccess) return false;
	RW_spi.canpacketread = true;
	bool validpacket=false;
	
	unsigned char len0 = 0;
	unsigned char rxBuf0[8];
	long unsigned int rxId;
	unsigned char SamsungStop[8] =  {0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	//unsigned int spibusy = 0x1C0040;		//PORT_PA06  | PORT_PA18 | PORT_PA19 | PORT_PA20;		// PA06: Wifi_CS		PA18:SD_CS		PA19:CS_CAN1		PA20:CS_CAN0
	//if ((REG_PORT_OUT0  & 0x1C0040)==0x1c0040){	}
	
	digitalWrite(13,HIGH);
	while (!digitalRead(INT_CAN0)) {
		CAN0.readMsgBuf(&len0, rxBuf0);
		rxId = CAN0.getCanId();                    			// Get message ID  , No SPI port access done ...
		//if ((rxId == 0x333) || (rxId == 0x217) || (rxId == 0x22E) || (rxId == 0x1AE) || (rxId == 0x200) || (rxId == 0x202) || ((rxId >=0x100) && (rxId <=0x150)  )){
		if(1){
			validpacket=true;
			
			if (rxId==0x200){
				Vehicle.V_Batt = rxBuf0[0] | rxBuf0[1] << 8;
				Vehicle.Current = rxBuf0[2] | rxBuf0[3] << 8;
				Vehicle.SOC = rxBuf0[5];
				if (rxBuf0[4] & 0x04) Vehicle.charger_detect = 1;
				else Vehicle.charger_detect = 0;
			}
			else if (rxId==0x217) {
				Vehicle.rpmcnt = rxBuf0[1];
			}
			
			// Prepare CanMsg Buffer
			//  --> circular buffer if(20<CanBuff.idx++) CanBuff.idx=1;	
			int k=1;
			while ((k<=CanBuff.idx) && (CanBuff.id[k].msgId!=rxId )) k++;
			
			if (k>28) k=28;
			if (k>CanBuff.idx) CanBuff.idx=k;
			CanBuff.id[k].timestamp=millis();
			CanBuff.id[k].len=len0;
			CanBuff.id[k].msgId=rxId;
			if (rxId==0x100) {
				if (CanBuff.id[k].msg[0]!=rxBuf0[0]) Vehicle.state=0;
			}
			for (int i=0;i<len0;i++) CanBuff.id[k].msg[i]=rxBuf0[i];
		}	
		if (rxId==0x16B) {
			//Serial.println("ISR STOP SAMSUNG");
			CAN0.sendMsgBuf(0x150, 1, 8, SamsungStop);
		}	
		Vehicle.cancnt+=1;
		Vehicle.lastcanpckt=millis();
	}
	digitalWrite(13,LOW);
	RW_spi.canpacketread = false;
	return validpacket;
}


