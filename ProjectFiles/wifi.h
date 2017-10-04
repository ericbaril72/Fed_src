//#include <stdlib.hput>			//defines standard types



#ifndef _WIFI_H_
#define _WIFI_H_

#include <arduino.h>
#include <ProjectMain.h>

struct HttpURL {
	const char* host;
	uint16_t port;
};


struct SRouter {
	const char* ssid;
	const char* pass;
};


int postdata(HttpURL URL,char * somedata);
int postJSON(HttpURL URL,char * somedata, char * posttype);

void postmystatus();
//int postdata(HttpURL URL,char * somedata);
//bool postdata(char * somedata);

bool postthisfile(HttpURL URL,char * filename);
bool connectRouter();
bool checkfornewFW_dl();
bool checkforbootloading_exec();
bool downloadFile(String  filePath, String filename, String sourceaddress);
bool fakedownloadFile(String  filePath, String filename, String sourceaddress);
extern void InitWifi();
void printEncryptionType(int thisType);
void listNetworks();
bool connect_to_ok_ssid();
void printWifiStatus() ;
void SDconfigConnectWifi();
bool WifiAvail(void);
void postDirectory();
bool getdownloadlist(HttpURL PostDataURL);
void serialstatusdump();
bool checkforfiledownloadlist();
int patchfiledownloadcnt(HttpURL URL,int Id, int cnt);
void postfromfilelist(bool erase);
int httpJsonPost(HttpURL URL, char * JSONstring, char * JSONkey);
int httpJsonPostEvent(HttpURL URL, char * key, char * value);void getJSON(char urllink[150], char * outchararray);
void resetWINC();

#endif