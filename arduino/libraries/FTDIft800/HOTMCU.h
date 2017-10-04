#ifndef _HOTMCU_H_
#define _HOTMCU_H_

/* Hardware specific include files */

//#define _FT810_ENABLED
#define _FT800_ENABLED

//macros related to arduino plaform SPI
/* SPI clock frequency */
#define FT_SPI_CLK_FREQ_MIN		8*1000000
#define FT_SPI_CLK_FREQ_MAX		21*1000000  //ORIGINAL CALL
//#define FT_SPI_CLK_FREQ_MAX		12*1000000

//macros specific to sdcard
//#define FT_SD_CSPIN 			8

#include "Arduino.h"

#ifdef _FT800_ENABLED
#include <hardware/FT800/FT800.h>
#endif
//#ifdef _FT810_ENABLED
//#include <hardware/FT800/FT810.h>
//#endif


#include <libraries/FT_GC/FT_GC.h>
#include <libraries/FT_GC/FT_Transport_SPI/FT_Transport_SPI.h>
#include <libraries/FT_GC/FT800/FT800Impl.h>

/* Macros related to display dimensions */
#define FT_DISPLAYWIDTH							FT_DISPLAY_HSIZE_WQVGA
#define FT_DISPLAYHEIGHT						 	FT_DISPLAY_VSIZE_WQVGA
#define FT_DISPLAY_RESOLUTION			 	FT_DISPLAY_WQVGA_480x272

//#define FT_DISPLAYWIDTH							FT_DISPLAY_HSIZE_WVGA
//#define FT_DISPLAYHEIGHT						 	FT_DISPLAY_VSIZE_WVGA
//#define FT_DISPLAY_RESOLUTION			 	FT_DISPLAY_WVGA_800x480

typedef FT800Impl<FT_Transport_SPI> FT800IMPL_SPI;

#endif

/*
GD.wr32(REG_VSYNC0, 0);
GD.wr32(REG_VSYNC1, 10);
GD.wr32(REG_VOFFSET, 12);
GD.wr32(REG_VCYCLE, 500);
GD.wr32(REG_HSYNC0, 0);
GD.wr32(REG_HSYNC1, 41);
GD.wr32(REG_HOFFSET, 43);
GD.wr32(REG_HCYCLE, 900);//548
GD.wr32(REG_HSIZE, 800);
GD.wr32(REG_VSIZE, 480);
GD.wr32(REG_PCLK_POL, 1);//1
GD.wr(REG_SWIZZLE, 0);//3 for GD2
GD.wr32(REG_PCLK, 3);//5

GD.wr(REG_ROTATE, 0);
GD.wr32(REG_DITHER, 1);


*/
