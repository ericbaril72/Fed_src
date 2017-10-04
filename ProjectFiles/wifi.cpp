#include "wifi.h"

#define DEBUG_WIFI
#define Serial SerialUSB
//#define Serial1 SerialUSB

//extern DL_fcts DL_fct[];
extern HttpURL PostDataURL;
extern SRouter wifi;
extern SdFat sd;
extern SdFile logFile;

extern RTC_PCF8523 pcfrtc;
extern RTCZero rtc;
extern bool ledState;

extern display_screen display_screen;
Adafruit_WINC1500Client client;
extern SRouter wifilist[6];
long int totalPostedSize=0;
long int totalPostedFailSize=0;
/*
extern StaticJsonBuffer<256> jsonBufferStatus;
extern JsonObject& JSONstatus;
extern StaticJsonBuffer<128> jsonBufferEvent;
extern JsonObject& JSONevent;
*/

uint8_t databuf[1028];

bool postthisfile(HttpURL PostDataURL,char * filename) {
	SdFile fileToPost;
	char chardirname[40];
	char buffer[80];
	int filecount = 0;
	long int fsize = 0;
	
	if(!fileToPost.open(filename,O_READ)) {
		Serial.print("couldn't open: ");
		Serial.println(filename);
		fileToPost.close();
		return false;
	}
	fsize = fileToPost.fileSize();
	
	if (fsize==0) {
		Serial.println("\t"+ String(filename)+" is empty file, erase !");
		fileToPost.remove();
		return false;
	}
	char postfilelink[] = "/upload";
	bool serverAvailable=true;
	long int inittime = millis();
	int lastclientwrite =0;
	int buffsize=1024;
	byte clientBuf[buffsize];
	int clientCount = 0;
	bool payload=false;
	bool emptyNewLine=false;
	
	int serverTimeout=500;
	String postHeader="";
	String datastring ="";
	String datastringend ="";
		
	// open file to send
	
	
	
	bool received = false;
	RW_spi.rfpacketsend=true;
	char msgbuffer[40];
	sprintf(msgbuffer,"%s(%dkB)",filename,fsize/1024);
	httpJsonPostEvent(PostDataURL,"Uploading",msgbuffer);
	//DL_fct[DL_SERVER].status=FCT_WARNING;
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		//DL_fct[DL_SERVER].status=FCT_ACTIVE;
		
		
		//char filenametopost[40];
		//fileToPost.getName(filename, 40);
		
		 // prep connection HEADERS
		 
		if(1) {
			//Serial.println("\tPosting (" + String(chardirname) + "-" + String(filename) + ":" + String(fsize) + ") to: " + String(PostDataURL.host) + ":" + String(PostDataURL.port) + String( postfilelink));
			//Serial.print("Posting (" + String(filename) + ":" + String(fsize) + ") to: " + String(PostDataURL.host) + ":" + String(PostDataURL.port) + String( postfilelink));
			Serial.print("   Posting (" + String(filename) + ":" + String(fsize) + ")");
			
			// Prepare HTTP POST header
			String CRLN = "\r\n";
			//String boundaryStr = "----WebKitFormBoundaryBaEzMXvGCQnJ3YJy";
			String boundaryStr = "BoundaryStringToIdentifyWhenthereisdisconnect";
			String boundaryStrCRLN = "--" + boundaryStr + CRLN;
			String boundaryStrEND =  "--" + boundaryStr + "--";

			// Building datastring payload to prepare Content-length value
			String Content1 = "Content-Disposition: form-data; name=\"VehicleName\"" + CRLN + CRLN +  String(Vehicle.id) + CRLN;
			
			//String Content2 = "Content-Disposition: form-data; name=\"FilePath\"" + CRLN +  CRLN + String(chardirname) + CRLN;;
			String Content2 = "Content-Disposition: form-data; name=\"FilePath\"" + CRLN +  CRLN + String("") + CRLN;
			
			String Content3 = "Content-Disposition: form-data; name=\"file\"; filename=\"" + String(filename) + "\"" + CRLN;
			String Content4 = "Content-Transfer-Encoding: binary" + CRLN ;
			String Content5 = "Content-Type: \"\"" +  CRLN;

			//datastring = boundaryStrCRLN + Content1 + boundaryStrCRLN + Content2 + boundaryStrCRLN + Content3;
			datastring = boundaryStrCRLN + Content1 + boundaryStrCRLN + Content2 + boundaryStrCRLN + Content3 + Content4+ Content5 + CRLN;
			datastringend = CRLN + boundaryStrEND + CRLN;
			String datastrlen = String(datastring.length() + fsize + datastringend.length());

			String HPOST = "POST " + String(postfilelink) + " HTTP/1.1" + CRLN;
			String HHost = "Host: " + String(PostDataURL.host) + ":" + String(PostDataURL.port) + CRLN;
			String HContentLength = "Content-Length: " + datastrlen + CRLN;
			String HContentType = "Content-Type: multipart/form-data; boundary=" + boundaryStr + CRLN;
			postHeader = HPOST + HHost + HContentLength + HContentType +  CRLN;
		}
		//Serial.print(postHeader);
		//Serial.print(datastring);
		inittime = millis();
		client.print(postHeader);
		client.print(datastring);
		
		int timeout =serverTimeout;
		// Send every bytes of the file to WINC1500
		while (fileToPost.curPosition() < fileToPost.fileSize()  && serverAvailable) {
			uint32_t n =  min(buffsize, fileToPost.fileSize() - fileToPost.curPosition());
			
			fileToPost.read(clientBuf, min(buffsize, n));
			client.write(clientBuf, n);
			//Serial.write(clientBuf, n);
			lastclientwrite = millis()+500;
			//delay(100);
		}
			
		
		
			
		if (clientCount > 0 ) {
			//if(!client.write(clientBuf, clientCount)) {Serial.print();} //
				client.write(clientBuf, clientCount);
				//Serial.write(clientBuf, clientCount);
		}
		
		client.print(datastringend);
		
		//Serial.println("----- ---FINISHED-------------------------------- -----");
		
		timeout = 5000;
		while( timeout>0 && serverAvailable){
			String linebuf = "";
			while (client.available()) {
				char c = client.read();
				timeout = 5000;
				linebuf += c;
								
				//Serial.write(c);
				if (c == 13) {
					client.read();
					//if (payload) Serial.println("\tServerAns:"+String(linebuf));		
					if (linebuf.startsWith("PASS")) received = true;
					
					linebuf = "";
					if (emptyNewLine) payload=true;
					emptyNewLine=true;
				} else emptyNewLine=false;
			}
			if(!client.connected()) serverAvailable=false;
			timeout-=10;
			delay(10);
		}
		//Serial.println("----- ----------------------------------- -----");
		if(!serverAvailable){
			Serial.println(" Server Stopped talking after ( Bytes) :..."+String(fsize));
			client.flush();
			client.stop();
		}
		if(1){
			if (received) {
			
				// file received at server, so rename file to .bak
				memcpy(buffer + 19, ".bak", 4);
				fileToPost.rename(sd.vwd(), buffer);
			
				float deltatime = (millis() - inittime);
				char rate[6];
				char timesecs[6];
				dtostrf(fsize / deltatime, 0, 0, rate);
				dtostrf(deltatime / 1000, 3, 0, timesecs);
				Serial.println("\t@ " + String(rate) + " KB/s in " + String(timesecs) + "(s)");
				httpJsonPostEvent(PostDataURL,"Uploaded",filename);
				totalPostedSize+=fsize;
			}
			else {
				totalPostedFailSize+=fsize; 
				Serial.println("\tNot received ");
				
			}
		}
	}
	else {
		Serial.println("  Server did not respond to Connection request !");
		//DL_fct[DL_SERVER].status=FCT_FAILURE;
	}
	
	if (!client.connected()) client.stop();
	
	
	RW_spi.rfpacketsend=false;
	fileToPost.close();
	//Serial.println("----- ----------------------------------- -----");
	//Serial.println("File uploaded :"+String(received));
			
	return received;
}
bool downloadFile(String  filePath, String filename, String sourceaddress){
	int tempint=0;
	
	Serial.println("--- Downloading file ---"+sourceaddress);
	
	long int inittime=millis();
	char buffer[50];
	char filesize[15];
	long int fsize=0;
	String linebuf;
	char c;
	int jcnt=0;
	SdFile dlfile;
	
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
	//Serial.println("Make DIR:"+String(charFilePath));
	RW_spi.sdpacketwrite=true;
	RW_spi.rfpacketsend=true;
	//sd.mkdir(charFilePath);
	
	
	//memcpy(pathfilename,filePath,sizeof(filePath));
	//Serial.println("\tDownload File to SD:"+String(charpathfilename));
	//Serial.println("\tFilePath:"+String(filePath));
	//Serial.println("\tFilename:"+String(filename));
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
			Serial.print(".-");
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
						//Serial.println(linebuf);
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
					delay(1000);
					Serial.println("No payload Yet");
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
					delay(3);//Serial.println(tempint);
				}
				else {
					client.read();
					Serial.println("Error with JCNT>FSIZE");
				}
				timeout=millis()+10000;
				while(!client.available() && millis()<timeout && jcnt<fsize) {
					Serial.print("\t");
					Serial.print(jcnt/1024);
					Serial.println(" kB rcvd");
					delay(500);
					
					
					
					
				}
			}
			
			
			float deltatime =(millis()-inittime);
			char rate[6];
			char timesecs[6];
			dtostrf(fsize/deltatime,0,0,rate);
			dtostrf(deltatime/1000,5,1,timesecs);
			Serial.println("\t\tTransfer Rate: "+String(rate)+" KB/s: "+String(timesecs)+"(s)");
			// do nothing forevermore:
			
			if(fsize<=45)	Serial.println("File Not available ("+String(fsize)+"B)");
			else if (dlfile.fileSize()!=fsize) 	Serial.println("\tError: transfer timeout after "+String(dlfile.fileSize())+ " Bytes");
			else {
				fileOK=true;
				Serial.println("\tFile OK !");
				
			}
			if(!fileOK) dlfile.remove();
			else Vehicle.lastfilesize=fsize;
			dlfile.close();
			
		}
		else Serial.println("\tServer Timeout !");
		
	}
	
	else Serial.println("\tCould not CONNECT ");
	client.flush();
	client.stop();
	client.flush();
	RW_spi.sdpacketwrite=false;
	RW_spi.rfpacketsend=false;
	return fileOK;
}
void postfromfilelist(bool erase) {
	RW_spi.sdpacketwrite=true;
	SdFile listfilestopost;
	char buffer[80];
	SdFile tempfilepost;
	SdFile fileToPost;
	char chardirname[40],logFilename[40];
	totalPostedSize=0;
	totalPostedFailSize=0;
	long starttime=millis();
	
	tempfilepost.open("temppost.lst", O_RDWR | O_CREAT | O_AT_END);
	//			/2029/15/24/251746ON.bak
	if (listfilestopost.open("filepost.lst",  O_RDWR)) {
		listfilestopost.seekSet(0);
		char c;
		String linebuf = "";
		bool ServerAvail = true;
		
		

		
		while (listfilestopost.available()) {
			c = listfilestopost.read();
			linebuf += c;
			if (c == 13) {
				c = listfilestopost.read();   //remove the extra LF(10) after the CR(13)
				Serial.print("File to post:"+linebuf);
				linebuf.toCharArray(buffer, linebuf.length());
				linebuf.toCharArray(chardirname, 11);
				linebuf = "";
				chardirname[10] = '\0';
				//logFilename[10] = '\0';
				if (ServerAvail) {				// Stop trying to send files if server not responding
					/*Serial.print("-->");
					Serial.print(buffer);
					Serial.println("             strcmp(buffer,\"MDL.bin\") : "+String(strcmp(buffer,"MDL.bin")));
					Serial.print("-->");
					Serial.print(buffer);
					Serial.println("             strcmp(buffer,\"fd.bin\") : "+String(strcmp(buffer,"fd.bin")));
					Serial.print("-->");
					Serial.print(buffer);
					Serial.println("             strcmp(buffer,\"filepost.lst\") : "+String(strcmp(buffer,"filepost.lst")));
					*/
					//Serial.print("strcmp ");
					
					
					if ((strcmp(buffer, "filepost.lst") == 0)
					
					|| (strcmp(buffer,"MDL.bin") ==0 )
					|| (strcmp(buffer,"fd.bin") ==0 )
					|| (strcmp(buffer,"temppost.lst") ==0 )
					//|| (strcmp(buffer,logFilename) >=8)
					)
					
					{	// Dont send the file if it is the current log file
						Serial.println("Not posting this:"+ String(buffer));
						
						
						
					}
					else {
						///Serial.println("Uploading File-->"+String(buffer));
						//Serial.println("strcmp(MDL.bin)-->"+String(strcmp(buffer,"MDL.bin")));
						
						if (!postthisfile(PostDataURL,buffer)) {		// Send the file, Check Server Answer, you may need to keep it in the list
							Serial.println("\tfailed to upload. keeping in the list: " + String(buffer));
							tempfilepost.println(buffer);
							tempfilepost.flush();
							//ServerAvail = false;
						}
						else {
							if(erase){
								fileToPost.open(buffer, O_WRITE);
								//Serial.print("\tErasing: "+String(buffer));
								fileToPost.remove();
							}
							
						}
						
					}
				}
				else {
					Serial.println("Server not avail.  keeping in the list:"+ String(buffer));
					tempfilepost.println(buffer);
					tempfilepost.flush();
				}
			}
		}
		listfilestopost.remove();
		listfilestopost.close();  //??? maybe still needed
		
	}
	
	else Serial.println("Could not open filepost.lst to append new logfile entry");
	//Serial.println("Done with filepost.lst content");

	if (!tempfilepost.rename(sd.vwd(), "filepost.lst")) Serial.println("Could not rename !!!!!");
	tempfilepost.close();
	
	Serial.println("Total Bytes sent: "+String(totalPostedSize));
	char rate[6];
	char timesecs[6];
	long endtime=millis();
	int deltatime =endtime-starttime;
	Serial.println("TOTAL transfer time : "+String((deltatime)/1000)+" seconds");

	dtostrf(totalPostedSize / deltatime, 0, 0, rate);
	dtostrf(deltatime / 1000, 3, 0, timesecs);
	Serial.println("\t@ " + String(rate) + " KB/s in " + String(timesecs) + "(s)");
	Serial.println("Total Bytes UNSENT: "+String(totalPostedSize));
	Serial.println("Total Files size: "+String(totalPostedSize+totalPostedFailSize));
	
	RW_spi.sdpacketwrite=false;
}

void getJSON(char urllink[150], char * outchararray){
	
	RW_spi.rfpacketsend=true;
	bool payload=false;
	bool emptyNewLine=false;
	
	//String linebuf="";
	char linebuff[400];
	int timeout;
	char c;
	if (WiFi.status()!=WL_CONNECTED) {
		RW_spi.rfpacketsend=false;
		
	}
	
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		client.print("GET "); client.print(urllink);	client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();
		
		timeout = millis()+2000;
		delay(250);
		while(!client.available() && millis()<timeout) {
			delay(100);
			Serial.print(".");
		}
		while (client.available()) {
			c = client.read();
			if (payload) {
				sprintf(outchararray+strlen(outchararray),"%c",c);
			}
			if (c==13){
				c=client.read();
				if (emptyNewLine) payload=true;
				emptyNewLine=true;
			}
			else emptyNewLine=false;
		}
		
		client.stop();
		client.flush();
	}
	else Serial.println("\tCould not connect");
	//Serial.print("sizeof:");
	//Serial.println(sizeof(linebuff));
	//Serial.println("----");
	RW_spi.rfpacketsend=false;
	//memcpy(outchararray,linebuff,strlen(linebuff)+1);
	
}

int httpJsonPostEvent(HttpURL URL, char * key, char * value) {
	char jsonString[64];
	sprintf(jsonString,"{\"%s\":\"%s\"}",key,value);
	return httpJsonPost(URL,jsonString,"event");
}
int httpJsonPost(HttpURL URL, char * JSONstring, char * JSONkey) {
	RW_spi.rfpacketsend=true;
	Serial.print("--> Send JSON Status : ");
	if (WifiAvail()==false) return false;
	
	char postHeader[200];
	
	int returncode = 0;
	int jsonlength = strlen(JSONstring);
	sprintf(postHeader, "POST /vehicleupdate/%s HTTP/1.1\r\n"
						"Host: %s:%d\r\n"
						"Content-Type: application/json\r\n"
						"Content-Length: %d\r\n"
						,	Vehicle.id, URL.host, URL.port, 5+strlen(JSONkey)+strlen(JSONstring));
	
	//Serial.println("jsonstring="+String(JSONstring));
	//Serial.println("-------------------posting this:-------");
	//Serial.println(postHeader);
	//Serial.print("{\""+String(JSONkey)+"\":");
	//Serial.print(JSONstring);
	//Serial.println("}");
	
	
	
		
	client.stop();	
	if (client.connect(URL.host, URL.port)) {
		Serial.print("Server OK, ");
		client.println(postHeader);
		client.print("{\""+String(JSONkey)+"\":");
		client.print(JSONstring);
		client.println("}");
				
		int timeout = 1000;
		String linebuf="";
		while(client.connected() || timeout>0){
			if (client.available()) {
				char c = client.read();
				timeout = 500;
				linebuf += c;
				if (c == 10) {
					//Serial.print(linebuf);
					if (linebuf.startsWith("200", 9)) {
						returncode = 1;
						Serial.println(" Received OK");
						break;
					}
					linebuf = "";
				}
			}
			else {
				if (!client.connected())break;
				else delay(10);
			}
			timeout-=10;
		}
		//Serial.print("--->ServerAnswer:");
		//Serial.println(linebuf);
	} else {
		Serial.println("\tNo Server");
		delay(1000);
	}	
	client.stop();
	RW_spi.rfpacketsend=false;
	return returncode;
}

bool connectRouter() {
	// attempt to connect to Wifi network:
	int retries = 0;
	char buffer[40];
	bool connected=false;
	Serial.print("\tTrying to reach :  ");
	sprintf(buffer,"\tSSID: %s %s",wifi.ssid,wifi.pass);
	Serial.println(buffer);
	//PrintLCD(buffer);
	//RW_spi.rfpacketsend=true;
	// Try with last connected Router first ...
	while ((WiFi.status() != WL_CONNECTED) && (retries < 3))  {
		//DL_fct[DL_Wifi].status=FCT_WARNING;
		if (retries) {
			Serial.print("\tRetry... ");
			WiFi.disconnect();
			//PrintLCD("Retry ...");
		} 
			
				
		
		//IP Addr: 192.168.0.195
		//		GW_IP: 192.168.0.4
		//IPAddress IPaddr = {192,168,0,195};
		//IPAddress DNSaddr = {1,2,3,4};
		//IPAddress GWaddr = {192,168,0,4};
		//IPAddress NMaddr = {255,255,255,0};
		//WiFi.config(IPaddr,DNSaddr,GWaddr,NMaddr);
		//Serial.println("Wifi status:"+String(WiFi.status()));
		
		digitalWrite(13,HIGH);
		if (strlen(wifi.pass) == 0) WiFi.begin(wifi.ssid);
		else WiFi.begin(wifi.ssid, wifi.pass);
		digitalWrite(13,LOW);
		
		++retries;
		delay(1000);
	}

	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("   Not Reachable !");
		//DL_fct[DL_Wifi].status=FCT_DISABLE;
	}	  
	else if (WiFi.status() == WL_CONNECTED){
		//DL_fct[DL_Wifi].status=FCT_ACTIVE;
		Serial.println("\tCONNECTED !");
		connected= true;
		//
	}
	else {
		Serial.println("\twifi status: "+String(WiFi.status()));
	}
	//RW_spi.rfpacketsend=false;
	return connected;

}
bool WifiAvail (void){
	return false;
	if (WiFi.status()==WL_CONNECTED) {
		Serial.print("\tWifi OK, ");
		if (Vehicle.wifiTimeout<2) return true;
		resetWINC();
		
	}
	
	if (connect_to_ok_ssid()) return true;
	return false;
	
	// next stuff all done in ok_ssid
	Serial.println("connect to ssid failed ... going trough list !");
	//Serial.print("Wifilistlength:");
	//Serial.println(sizeof wifilist );
	//Serial.print("Wifilist[0]length:");
	//Serial.println(sizeof wifilist[0] );
	//Serial.println(sizeof wifilist[1] );
	//Serial.println( wifilistlength );
	//Serial.println(sizeof wifilist/ sizeof wifilist[0] );
	//Serial.println("--List of Visible Networks--");
	//listNetworks();
	/*Serial.println("--List of Registered Networks--");
	for ( int i=0; i<6 ; i++) {
		Serial.print("\t");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(wifilist[i].ssid);
	}
	*/
	//Serial.println("--Reaching OUT (shortlist )--");
	//if (connect_to_ok_ssid()) return true;
	
	/*
	Serial.println("--Reaching OUT (regular full long list even if not visible !!! )--");
	if (connect_to_ok_ssid()) return true;
	for ( int i=0; i<6 ; i++) {
		wifi = wifilist[i];
		
		if (connectRouter()) return true;
		delay(1000);
	}
	*/
	return false;
}









void printWifiStatus() {
	// print the SSID of the network you're attached to:
	
	IPAddress ip = WiFi.localIP();
	Serial.print("\n\tIP Addr: ");
	Serial.println(ip);
	
	ip = WiFi.gatewayIP();
	Serial.print("\tGW_IP: ");
	Serial.println(ip);
	// print the received signal strength:
	RW_spi.rfpacketsend=true;
	long rssi = WiFi.RSSI();
	RW_spi.rfpacketsend=false;
	
	Serial.print("\tSignal:");
	Serial.print(rssi);
	Serial.println(" dBm");
}

bool connect_to_ok_ssid(){
	//RW_spi.rfpacketsend=true;
	Serial.print("Scanning for Wifi ...");
	int numSsid = WiFi.scanNetworks();
	Serial.println("trying default SSID");
	if (numSsid == -1) {
		Serial.println("\tNo Wifi Network");
	} else {
		for (int listelement = 0; listelement <6;listelement++ ){
			//Serial.println("Wifilist# "+String(listelement+1)+"-"+String(wifilist[listelement].ssid));
			for (int thisNet = 0; thisNet < numSsid; thisNet++) {
				//Serial.println("\tWifiScan# "+String(listelement+1)+"-"+String(WiFi.SSID(thisNet)));
				if (String(WiFi.SSID(thisNet)).startsWith(wifilist[listelement].ssid)){
						wifi = wifilist[listelement];
						Serial.println("\tWifiScan# "+String(listelement+1)+"-"+String(WiFi.SSID(thisNet)));
						if (connectRouter()) {
							Vehicle.wifiTimeout=0;
							return true;
						}
						
				}
			}
		}
	}
	Serial.println("Could not connect to wifi !");//RW_spi.rfpacketsend=false;
	return false;
}

void resetWINC(){
	return;
	Serial.println("Wifi.status():"+String(WiFi.status()));
	Serial.println("Resetting WINC: ");
	//digitalWrite(WINC_CS, HIGH);
	digitalWrite(WINC_EN, LOW);
	digitalWrite(WINC_RST, LOW);
	Serial.println("Done");
	delay(500);
	digitalWrite(WINC_EN, HIGH);
	digitalWrite(WINC_RST, HIGH);
	Serial.println("resetted !");
	Serial.println("Wifi.status():"+String(WiFi.status()));
	WiFi.init();
	Serial.println("Init !");
	Serial.println("Wifi.status():"+String(WiFi.status()));
	delay(5000);
}
void InitWifi(){
	
		//DL_fct[DL_Wifi].status=FCT_WARNING;
		Serial.println("--- INIT Wifi ---");
		//PrintLCD("--- Wifi init ---");
		display_screen.spinner=true;
				
		//DL_fct[DL_Wifi].status=FCT_ACTIVE;
		
		if (WifiAvail()) printWifiStatus();
		
		WiFi.macAddress(Vehicle.mac);
		sprintf(Vehicle.id ,"%02X%02X%02X%02X%02X%02X",
					Vehicle.mac[0],Vehicle.mac[1],Vehicle.mac[2],
					Vehicle.mac[3],Vehicle.mac[4],Vehicle.mac[5]);
		
		Serial.print("\tMAC addr: ");
		Serial.println(Vehicle.id);
		if (WifiAvail()) printWifiStatus();
		
		display_screen.spinner=false;
}

bool getdownloadlist(HttpURL PostDataURL){
	
	char linkstring[300];
	char jsoncontent[512];
	int jsoncontentIdx=0;
	jsoncontent[0]=0;
		
		
	sprintf(linkstring,"api/v1/dlfilelist?q={\"filters\":[{\"name\":\"VehicleName\",\"op\":\"==\",\"val\":\"%s\"},{\"name\":\"Downloaded\",\"op\":\"==\",\"val\":\"0\"}],\"order_by\":[{\"field\":\"ID\",\"direction\":\"desc\"}],\"limit\":1}",
		Vehicle.id);
	//Serial.println("URL: "+String(linkstring));
	
	bool haspendingfiles=false;
	//bool sok=false;
	//bool sov=false;
	//bool soint=false;
	//bool pendingkey = false;
	//int idx=0,jsonID=0;
	bool payload=false;
	bool emptynewline=false;
	//String key,value;
	String linebuf;
	//String jsonbuff="";
	char filesize[15];
	long int fsize=0;
	int jcnt=0;
	
	//char filename[20],path[50],dllink[50];
	int timeout;
	int counter=0;
	char c;
	Serial.print("--> Get JSON DL list : ");
	RW_spi.rfpacketsend=true;
	
	if (WifiAvail()==false) return false;
	
	/*
	if (WiFi.status()!=WL_CONNECTED) {
		RW_spi.rfpacketsend=false;
		return false;
	}
	*/
	//Serial.println("--- Checking for new DownloadList ---      ");
	
	
	RW_spi.rfpacketsend=true;
	if (client.connect(PostDataURL.host, PostDataURL.port)) {
		client.print("GET "); client.print(linkstring);client.println(" HTTP/1.1");
		client.print("Host: "); client.println(PostDataURL.host);
		client.println("Connection: close");
		client.println();
		Serial.print("Server OK: ");
		timeout = 5000;
		delay(100);
		while(!client.available() && timeout>0) {
			delay(250);
			timeout-=250;
			Serial.print(".");
		}
		//if (millis()<timeout) Serial.println();
		while (client.available()) {
			c = client.read();
			
			if (payload){
				if (jcnt<fsize){
					if (c!=0x10) sprintf(jsoncontent+strlen(jsoncontent),"%c",c);
					jcnt++;
				}
			}
			else {
				linebuf+=c;
				jsoncontent[jsoncontentIdx]=c;
				jsoncontentIdx++;
				
				if (c==13){
					c= client.read();
					if(linebuf.startsWith("Content-Length: ",0)) {
						linebuf.substring(15).toCharArray(filesize, linebuf.substring(15).length());
						fsize = atoi(filesize);
						//Serial.println("\JSON Size: "+String(fsize)+" B");
					}
					
					
					
					linebuf="";
					if (emptynewline) payload=true;
					if (payload) jsoncontent[0]=0;
					emptynewline=true;
				}
				else emptynewline=false;
			}
			timeout=2000;
			
			while(!client.available() && timeout>0 && jcnt<fsize) {
				delay(100);
				timeout-=100;
				Serial.println(",");
			} 
		}
		client.stop();
		client.flush();
		client.stop();
	} 
	else {
		Serial.println("No Server Ack");
		Vehicle.servernoack++;
		if (Vehicle.servernoack>=4){
			resetWINC();
		}
		RW_spi.rfpacketsend=false;
		return haspendingfiles;
	}
	
	RW_spi.rfpacketsend=false;
	
	
	c=0;
	sprintf(jsoncontent+strlen(jsoncontent),"%c\r\n",c);
	if (timeout<=0) {
		if (!payload) Serial.print("No Payload received -->  ");
		Vehicle.wifiTimeout++;
		Serial.println("Timed OUT !" +String(Vehicle.wifiTimeout));
		Serial.println("Received buffer length:"+String(strlen(jsoncontent)));
		Serial.print("buffer:");
		Serial.println(jsoncontent);
		Serial.println("--------------------------------");
		
	}
	if (!payload) Serial.println("No Payload received !");
	else{
		//Serial.println("----------------json---content--");
		//Serial.println(jsoncontent);
		//Serial.println("----------------parse--------");
		StaticJsonBuffer<400> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsoncontent);
		//Serial.println("----------------parsed--------");
		if (!root.success()) Serial.println("JSON parsing() failed");
		else if (root["num_results"]>=1){
			
			//Serial.println("Parsing Results :");;
			String DownLoadLink = root["objects"][0]["DownLoadLink"];
			String Filename =     root["objects"][0]["Filename"];
			String DownloadDir =  root["objects"][0]["DownloadDir"];
			uint16_t Downloaded =  root["objects"][0]["Downloaded"];
			uint16_t ID		   =  root["objects"][0]["ID"];
			
			//Serial.println("   ID: "+String(ID));
			//Serial.println("   PAth: "+DownloadDir);
			//Serial.println("   Filename: "+Filename);
			//Serial.println("   Link: "+DownLoadLink);
			//Serial.println("   Downloaded: "+String(Downloaded));
			//Serial.println("------------------------------");
			//Serial.println("Increment download cnt: "+String(patchfiledownloadcnt(PostDataURL,1,Downloaded+1)));
			
			if (Downloaded==0 && ID!=0) {
				httpJsonPostEvent(PostDataURL,"NewFileToDownload","True");
				Serial.println("   Never downloaded");
				
				char __Filename[sizeof(Filename)];
				Filename.toCharArray(__Filename, sizeof(__Filename));
				
				httpJsonPostEvent(PostDataURL,"Downloading",__Filename);
				//Serial.println("Need to download as it never was ...");
				if (downloadFile(DownloadDir, "tempf.par", DownLoadLink)) {
					//Serial.println("Woo hoo !!!!" +String(patchfiledownloadcnt(PostDataURL,1,Downloaded+1)));
					char IDnum[16];
					sprintf(IDnum,"ID:%d",ID);
					httpJsonPostEvent(PostDataURL,"Download_Success",IDnum);
					
					//Serial.println("remove results: " +String());
					
					//sd.remove(__Filename);
					SdFile erase;
					erase.open(__Filename, O_WRITE);
					erase.remove();
					sd.rename("tempf.par",__Filename);
					
					//Serial.println("rename results: " +String());
					//Serial.println("renaming tempf.par to: "+String(__Filename));
					if (Filename.startsWith("reset")) {
						httpJsonPostEvent(PostDataURL,"newFW","RESET");
						logFile.close();
						
						NVIC_SystemReset();
					}
					else if (Filename.startsWith("MDL.bin")) {
						httpJsonPostEvent(PostDataURL,"newFW_MDL","RESET");
						
						char value[16];
						itoa(Vehicle.V_Batt_Avg,value,10);
						//pinMode(14,INPUT);
						//if (digitalRead(14)) httpJsonPostEvent(PostDataURL,"A0_digital","HIGH");
						//else httpJsonPostEvent(PostDataURL,"A0","LOW");
						//httpJsonPostEvent(PostDataURL,"A0",value);
						if (Vehicle.V_Batt_Avg<100) httpJsonPostEvent(PostDataURL,"BootloadingTo","FactoryDefault");
						else httpJsonPostEvent(PostDataURL,"BootloadingTo","LatestMDL");
						logFile.close();
						NVIC_SystemReset();
					}
					else if (Filename.startsWith("fd.bin")) {
						httpJsonPostEvent(PostDataURL,"newFW_FD","RESET");
						char value[16];
						itoa(Vehicle.V_Batt_Avg,value,10);
						//pinMode(14,INPUT);
						//if (digitalRead(14)) httpJsonPostEvent(PostDataURL,"A0_digital","HIGH");
						//else httpJsonPostEvent(PostDataURL,"A0","LOW");
						//httpJsonPostEvent(PostDataURL,"A0",value);
						if (Vehicle.V_Batt_Avg<100) httpJsonPostEvent(PostDataURL,"BootloadingTo","FactoryDefault");
						else httpJsonPostEvent(PostDataURL,"BootloadingTo","LatestMDL");
						logFile.close();
						NVIC_SystemReset();
					}
				} else httpJsonPostEvent(PostDataURL,"Download","Failed");
			}
			else {
				//httpJsonPostEvent(PostDataURL,"NewFileToDownload","False");
				Serial.println("Allready downloaded ...");
				//delay(2000);
			}
		}
		else {
			Serial.println("Nothing to download ...");
			httpJsonPostEvent(PostDataURL,"NewFileToDownload","None");
		}
	}
	
	
	//Serial.println("Done with Download list:"+String(haspendingfiles));
	//Serial.println("pause...");
	
	return haspendingfiles;
}





void printEncryptionType(int thisType) /* to test */ {/*
	// read the encryption type and print out the name:
	switch (thisType) {
		case ENC_TYPE_WEP:
			Serial.println("WEP");
			break;
		case ENC_TYPE_TKIP:
			Serial.println("WPA");
			break;
		case ENC_TYPE_CCMP:
			Serial.println("WPA2");
			break;
		case ENC_TYPE_NONE:
			Serial.println("None");
			break;
		case ENC_TYPE_AUTO:
			Serial.println("Auto");
			break;
	}
*/}

