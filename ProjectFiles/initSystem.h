/*
 * initSystem.h
 *
 * Created: 2016-12-08 08:50:18
 *  Author: EricB
 */ 



#ifndef INITSYSTEM_H_
#define INITSYSTEM_H_
#include <ProjectMain.h>
void inittimer();
void initSerial();
void initSD();
void initIOs();
void InitRTC();
void initSpeedSensor();
#endif /* INITSYSTEM_H_ */