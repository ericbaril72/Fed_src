/*
 * initSystem.cpp
 *
 * Created: 2016-11-25 14:16:03
 *  Author: EricB
 */ 

#include <initSystem.h>

extern bool ledState;
extern SdFat sd;
extern RTC_PCF8523 pcfrtc;
extern RTCZero rtc;
void inittimer() {
	Serial.println("--- Init Timers ---");
	// TC3 for 1ms interupts
	// TC4 for 100ms timeouts , high level tasks
	
	// -------------------------------------------------
	// TC3 initialized for ~1ms interupts to manage SPI bus access ( MCP2515 / FT8XX LCD / SDCard / etc )
	// Enable clock for GLCK0 for TC3
	REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID ( GCM_TCC2_TC3 ) ) ;
	while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

	// The type cast must fit with the selected timer mode
	TcCount16* TC = (TcCount16*) TC3; // get timer struct

	TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ; // Set TC as normal Normal Frq
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256;   // Set perscaler
	//TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;   // Set perscaler
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	// TC->PER.reg = 0xFF;   // Set counter Top using the PER register but the 16/32 bit timer counts allway to max
	// while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	TC->CC[0].reg = 0xFFF;
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	// Interrupts
	TC->INTENSET.reg = 0;              // disable all interrupts
	TC->INTENSET.bit.OVF = 1;          // enable overfollow
	//TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0
	//									make sure you add proper interupt type validation in the handler
	

	// Enable InterruptVector
	NVIC_EnableIRQ(TC3_IRQn);

	// Enable TC
	TC->CTRLA.reg |= TC_CTRLA_ENABLE;
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	
	
	
	
	// -------------------------------------------------
	// TC4 initialisation ~100ms interupts to manage main loop periodic executions
	// Enable clock for GLCK2 for TC4
	
	REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_ID ( GCM_TC4_TC5 ) ) ;
	while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

	// The type cast must fit with the selected timer mode
	TC = (TcCount16*) TC4; // get timer struct

	TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ; // Set TC as normal Normal Frq
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	//TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256;   // Set perscaler
	TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;   // Set perscaler
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	// TC->PER.reg = 0xFF;   // Set counter Top using the PER register but the 16/32 bit timer counts allway to max
	// while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	TC->CC[0].reg = 0xFFF;
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

	// Interrupts
	TC->INTENSET.reg = 0;              // disable all interrupts
	TC->INTENSET.bit.OVF = 1;          // enable overfollow
	//TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0
	//									make sure you add proper interupt type validation in the handler
	

	// Enable InterruptVector
	NVIC_EnableIRQ(TC4_IRQn);

	// Enable TC
	TC->CTRLA.reg |= TC_CTRLA_ENABLE;
	while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
	
	
}
void initSerial(){
	Serial1.begin(115200);
	int timeout = 2;
	
	while ((!Serial) && timeout != 0){
		timeout--;
		digitalWrite(13, ledState=!ledState);
		Serial.print("-- ");
		delay(200);
		digitalWrite(13,LOW);
	}
}
void initSD(){
	SdFile::dateTimeCallback(dateTime);
	if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) {
		Serial.println("SD card init issue ...  check wires or sd card presence");
		//DL_fct[DL_SD].status=FCT_ERROR;
		sd.initErrorHalt();
	} //else DL_fct[DL_SD].status=FCT_ACTIVE;
	
}
void initIOs(){
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	
	digitalWrite(SD_CHIP_SELECT, HIGH);
	pinMode(SD_CHIP_SELECT,OUTPUT);
	
	digitalWrite(FT_CS_PIN, HIGH);
	pinMode(FT_CS_PIN,OUTPUT);
	
	pinMode(WINC_EN, OUTPUT);
	digitalWrite(WINC_EN, HIGH);
	
	pinMode(WINC_EN, OUTPUT);
	digitalWrite(WINC_RST, HIGH);
	
	digitalWrite(WINC_CS, HIGH);
	pinMode(WINC_CS,OUTPUT);
	
	digitalWrite(CS_CAN0,HIGH);
	pinMode(CS_CAN0,OUTPUT);
	
	pinMode(INT_CAN0, INPUT_PULLUP);
	digitalWrite(13, ledState=LOW);

}
void InitRTC(){
	char tempbuffer[70];
	Serial.println("--- RTC init ---");
	//PrintLCD("   PCFrtc init");
	pcfrtc.begin();  // Init PCF8523 on the feather Adalogger board
	//PrintLCD("   M0rtc Init");
	rtc.begin();		// Init M0 onboard RTC;
	DateTime now = pcfrtc.now();
	sprintf(tempbuffer, "\tRTC Date: %02d/%02d/%02d/%02dh%02d:%02ds" , now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
	Serial.println(tempbuffer);
	//pcfrtc.adjust(DateTime(F(__DATE__), F(__TIME__)));	// uncomment if time is lost
	if (!sd.exists("params.cfg")) {
		Serial.println("\tTime set to last  Compile time");
		
		//pcfrtc.adjust(DateTime(F(__DATE__), F(__TIME__)));	// uncomment if time is lost
		//SdFile temp;
		//temp.open("params.cfg", O_CREAT | O_WRITE);
		//temp.println(tempbuffer);
		//temp.flush();
		//temp.close();
	}
	
	sprintf(tempbuffer, "\tRTC Date: %02d/%02d/%02d/%02dh%02d:%02ds" , now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
	
	rtc.setTime(now.hour(), now.minute(), now.second());		// if M0 board is not battery operated
	rtc.setDate(now.day(), now.month(), now.year() - 16);			// if Battery : need to write Comparaison Code + Warning log file Stamping
	// Print Current M0 Time
	sprintf(tempbuffer, "\tM0  Date: 20%02d/%02d/%02d/%02dh%02d:%02ds" , rtc.getYear(), rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
	Serial.println(tempbuffer);
	//DL_fct[DL_RTC].status=FCT_ACTIVE;
	//PrintLCD(tempbuffer);
	
	
	
	
	
}
void initSpeedSensor() {
	Serial.println("--- Init Speed sensor ( pin A2)");
	pinMode(pinSpeed, INPUT_PULLUP);			//pinspeed = 16
	attachInterrupt(pinSpeed, dingrpm, RISING);
}

