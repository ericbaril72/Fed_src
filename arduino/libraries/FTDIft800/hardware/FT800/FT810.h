/*****************************************************************************
* Copyright (c) Future Technology Devices International 2014
* propriety of Future Technology devices International.
*
* Software License Agreement
*
* This code is provided as an example only and is not guaranteed by FTDI. 
* FTDI accept no responsibility for any issues resulting from its use. 
* The developer of the final application incorporating any parts of this 
* sample project is responsible for ensuring its safe and correct operation 
* and for any consequences resulting from its use.
*****************************************************************************/
/**
* @file                           FT800.h
* @brief                          Contains FT800 graphics controller interface for arduino platform.
								  Tested platform version: Arduino 1.0.4 and later
* @version                        0.1.0
* @date                           2014/02/05
*
*/

#ifndef _FT800_H_
#define _FT800_H_

/* Macros for display panels */
#define FT_DISPLAY_VSYNC0_WQVGA 		(0L)
#define FT_DISPLAY_VSYNC1_WQVGA 		(10L)
#define FT_DISPLAY_VOFFSET_WQVGA		(12L)
//#define FT_DISPLAY_VCYCLE_WQVGA 		(292L)  // 286
#define FT_DISPLAY_VCYCLE_WQVGA 		(286L)  // 286
#define FT_DISPLAY_HSYNC0_WQVGA 		(0L)	// 
#define FT_DISPLAY_HSYNC1_WQVGA 		(41L)
#define FT_DISPLAY_HOFFSET_WQVGA 		(43L)	
//#define FT_DISPLAY_HCYCLE_WQVGA 		(548L)	//525
#define FT_DISPLAY_HCYCLE_WQVGA 		(525L)	//525
#define FT_DISPLAY_HSIZE_WQVGA 			(480L)//display width
#define FT_DISPLAY_VSIZE_WQVGA 			(272L)//display height
#define FT_DISPLAY_PCLKPOL_WQVGA 		(1L)
#define FT_DISPLAY_SWIZZLE_WQVGA 		(0L)
#define FT_DISPLAY_PCLK_WQVGA 			(5L)

#define FT_DISPLAY_VSYNC0_WVGA 			(0L)
#define FT_DISPLAY_VSYNC1_WVGA 			(10L)
#define FT_DISPLAY_VOFFSET_WVGA			(12L)
#define FT_DISPLAY_VCYCLE_WVGA 			(500L)
#define FT_DISPLAY_HSYNC0_WVGA 			(0L)
#define FT_DISPLAY_HSYNC1_WVGA 			(41L)
#define FT_DISPLAY_HOFFSET_WVGA 		(43L)
#define FT_DISPLAY_HCYCLE_WVGA 			(900L)
#define FT_DISPLAY_HSIZE_WVGA 			(800L)//display width
#define FT_DISPLAY_VSIZE_WVGA 			(480L)//display height
#define FT_DISPLAY_PCLKPOL_WVGA 		(1L)
#define FT_DISPLAY_SWIZZLE_WVGA 		(0L)
#define FT_DISPLAY_PCLK_WVGA 			(5L)

#define FT_DISPLAY_VSYNC0_QVGA 			(0L)
#define FT_DISPLAY_VSYNC1_QVGA 			(2L)
#define FT_DISPLAY_VOFFSET_QVGA 		(13L)
#define FT_DISPLAY_VCYCLE_QVGA 			(263L)
#define FT_DISPLAY_HSYNC0_QVGA 			(0L)
#define FT_DISPLAY_HSYNC1_QVGA 			(10L)
#define FT_DISPLAY_HOFFSET_QVGA 		(70L)
#define FT_DISPLAY_HCYCLE_QVGA 			(408L)
#define FT_DISPLAY_HSIZE_QVGA 			(320L)//display width
#define FT_DISPLAY_VSIZE_QVGA 			(240L)//display height
#define FT_DISPLAY_PCLKPOL_QVGA 		(0L)
#define FT_DISPLAY_SWIZZLE_QVGA 		(2L)
#define FT_DISPLAY_PCLK_QVGA 			(8L)

/* Macros for default settings */
#define FT_DISPLAY_VSYNC0 				FT_DISPLAY_VSYNC0_WVGA
#define FT_DISPLAY_VSYNC1 				FT_DISPLAY_VSYNC1_WVGA
#define FT_DISPLAY_VOFFSET				FT_DISPLAY_VOFFSET_WVGA
#define FT_DISPLAY_VCYCLE 				FT_DISPLAY_VCYCLE_WVGA
#define FT_DISPLAY_HSYNC0 				FT_DISPLAY_HSYNC0_WVGA
#define FT_DISPLAY_HSYNC1 				FT_DISPLAY_HSYNC1_WVGA
#define FT_DISPLAY_HOFFSET 				FT_DISPLAY_HOFFSET_WVGA
#define FT_DISPLAY_HCYCLE 				FT_DISPLAY_HCYCLE_WVGA
#define FT_DISPLAY_HSIZE				FT_DISPLAY_HSIZE_WVGA
#define FT_DISPLAY_VSIZE				FT_DISPLAY_VSIZE_WVGA
#define FT_DISPLAY_PCLKPOL 				FT_DISPLAY_PCLKPOL_WVGA
#define FT_DISPLAY_SWIZZLE 				FT_DISPLAY_SWIZZLE_WVGA
#define FT_DISPLAY_PCLK					FT_DISPLAY_PCLK_WVGA

#define FT_GPIO0				0
#define FT_GPIO1				1	//default gpio pin for audio shutdown, 1 - eanble, 0 - disable
#define FT_GPIO7				7	//default gpio pin for display enable, 1 - enable, 0 - disable

/* Coprocessor reset related macros */
#define FT_RESET_HOLD_COPROCESSOR		1
#define FT_RESET_RELEASE_COPROCESSOR	0


/* Macros for general purpose */
#define FT_DISABLE				0
#define FT_ENABLE				1
#define FT_FALSE				0
#define FT_TRUE					1

/* Macros used for graphics commands */
#define FT_NEVER                0
#define FT_LESS                 1
#define FT_LEQUAL               2
#define FT_GREATER              3
#define FT_GEQUAL               4
#define FT_EQUAL                5
#define FT_NOTEQUAL             6
#define FT_ALWAYS               7

/* Bitmap format macros */
#define FT_ARGB1555             0
#define FT_L1                   1
#define FT_L4                   2
#define FT_L8                   3
#define FT_RGB332               4
#define FT_ARGB2                5
#define FT_ARGB4                6
#define FT_RGB565               7
#define FT_PALETTED             8
#define FT_TEXT8X8              9
#define FT_TEXTVGA              10
#define FT_BARGRAPH             11

/* Bitmap filter type macros */
#define FT_NEAREST              0
#define FT_BILINEAR             1

/* Bitmap wrap type macros */
#define FT_BORDER               0
#define FT_REPEAT               1

/* Stencil macros */
#define FT_KEEP                 1
#define FT_REPLACE              2
#define FT_INCR                 3
#define FT_DECR                 4
#define FT_INVERT               5

/* Graphics display list swap macros */
#define FT_DLSWAP_DONE          0
#define FT_DLSWAP_LINE          1
#define FT_DLSWAP_FRAME         2

/* Interrupt bits */
#define FT_INT_SWAP             0x01
#define FT_INT_TOUCH            0x02
#define FT_INT_TAG              0x04
#define FT_INT_SOUND            0x08
#define FT_INT_PLAYBACK         0x10
#define FT_INT_CMDEMPTY         0x20
#define FT_INT_CMDFLAG          0x40
#define FT_INT_CONVCOMPLETE     0x80

/* Touch mode macros */
#define FT_TMODE_OFF        	0
#define FT_TMODE_ONESHOT    	1
#define FT_TMODE_FRAME      	2
#define FT_TMODE_CONTINUOUS 	3

/* Alpha blending macros */
#define FT_ZERO                 0
#define FT_ONE                  1
#define FT_SRC_ALPHA            2
#define FT_DST_ALPHA            3
#define FT_ONE_MINUS_SRC_ALPHA  4
#define FT_ONE_MINUS_DST_ALPHA  5

/* Graphics primitives macros */
#define FT_BITMAPS              1
#define FT_POINTS               2
#define FT_LINES                3
#define FT_LINE_STRIP           4
#define FT_EDGE_STRIP_R         5
#define FT_EDGE_STRIP_L         6
#define FT_EDGE_STRIP_A         7
#define FT_EDGE_STRIP_B         8
#define FT_RECTS                9

/* Widget command macros */
#define FT_OPT_MONO             1
#define FT_OPT_NODL             2
#define FT_OPT_FLAT             256
#define FT_OPT_CENTERX          512
#define FT_OPT_CENTERY          1024
#define FT_OPT_CENTER           (FT_OPT_CENTERX | FT_OPT_CENTERY)
#define FT_OPT_NOBACK           4096
#define FT_OPT_NOTICKS          8192
#define FT_OPT_NOHM             16384
#define FT_OPT_NOPOINTER        16384
#define FT_OPT_NOSECS           32768
#define FT_OPT_NOHANDS          49152
#define FT_OPT_RIGHTX           2048
#define FT_OPT_SIGNED           256

/* Macros related to inbuilt font */
#define FT_NUMCHAR_PERFONT 		(128L)		//number of font characters per bitmap handle
#define FT_FONT_TABLE_SIZE 		(148L)		//size of the font table - utilized for loopup by the graphics engine
#define FT_FONT_TABLE_POINTER	(0xFFFFCUL)	//pointer to the inbuilt font tables starting from bitmap handle 16

/* Audio sample type macros */
#define FT_LINEAR_SAMPLES       0	//8bit signed samples
#define FT_ULAW_SAMPLES         1	//8bit ulaw samples
#define FT_ADPCM_SAMPLES        2	//4bit ima adpcm samples

/* Synthesized sound macros */
#define FT_SILENCE              0x00

#define FT_SQUAREWAVE           0x01
#define FT_SINEWAVE             0x02
#define FT_SAWTOOTH             0x03
#define FT_TRIANGLE             0x04

#define FT_BEEPING              0x05
#define FT_ALARM                0x06
#define FT_WARBLE               0x07
#define FT_CAROUSEL             0x08

#define FT_PIPS(n)              (0x0F + (n))

#define FT_HARP                 0x40
#define FT_XYLOPHONE            0x41
#define FT_TUBA                 0x42
#define FT_GLOCKENSPIEL         0x43
#define FT_ORGAN                0x44
#define FT_TRUMPET              0x45
#define FT_PIANO                0x46
#define FT_CHIMES               0x47
#define FT_MUSICBOX             0x48
#define FT_BELL                 0x49

#define FT_CLICK                0x50
#define FT_SWITCH               0x51
#define FT_COWBELL              0x52
#define FT_NOTCH                0x53
#define FT_HIHAT                0x54
#define FT_KICKDRUM             0x55
#define FT_POP                  0x56
#define FT_CLACK                0x57
#define FT_CHACK                0x58

#define FT_MUTE                 0x60
#define FT_UNMUTE               0x61

/* Synthesized sound frequencies, midi note macros */
#define FT_MIDI_A0            	21
#define FT_MIDI_A_0           	22
#define FT_MIDI_B0            	23
#define FT_MIDI_C1            	24
#define FT_MIDI_C_1           	25
#define FT_MIDI_D1            	26
#define FT_MIDI_D_1           	27
#define FT_MIDI_E1            	28
#define FT_MIDI_F1            	29
#define FT_MIDI_F_1           	30
#define FT_MIDI_G1            	31
#define FT_MIDI_G_1           	32
#define FT_MIDI_A1            	33
#define FT_MIDI_A_1           	34
#define FT_MIDI_B1            	35
#define FT_MIDI_C2            	36
#define FT_MIDI_C_2           	37
#define FT_MIDI_D2            	38
#define FT_MIDI_D_2           	39
#define FT_MIDI_E2            	40
#define FT_MIDI_F2            	41
#define FT_MIDI_F_2           	42
#define FT_MIDI_G2            	43
#define FT_MIDI_G_2           	44
#define FT_MIDI_A2            	45
#define FT_MIDI_A_2           	46
#define FT_MIDI_B2            	47
#define FT_MIDI_C3            	48
#define FT_MIDI_C_3           	49
#define FT_MIDI_D3            	50
#define FT_MIDI_D_3           	51
#define FT_MIDI_E3            	52
#define FT_MIDI_F3            	53
#define FT_MIDI_F_3           	54
#define FT_MIDI_G3            	55
#define FT_MIDI_G_3           	56
#define FT_MIDI_A3            	57
#define FT_MIDI_A_3           	58
#define FT_MIDI_B3            	59
#define FT_MIDI_C4            	60
#define FT_MIDI_C_4           	61
#define FT_MIDI_D4            	62
#define FT_MIDI_D_4           	63
#define FT_MIDI_E4            	64
#define FT_MIDI_F4            	65
#define FT_MIDI_F_4           	66
#define FT_MIDI_G4            	67
#define FT_MIDI_G_4           	68
#define FT_MIDI_A4            	69
#define FT_MIDI_A_4           	70
#define FT_MIDI_B4            	71
#define FT_MIDI_C5            	72
#define FT_MIDI_C_5           	73
#define FT_MIDI_D5            	74
#define FT_MIDI_D_5           	75
#define FT_MIDI_E5            	76
#define FT_MIDI_F5            	77
#define FT_MIDI_F_5           	78
#define FT_MIDI_G5            	79
#define FT_MIDI_G_5           	80
#define FT_MIDI_A5            	81
#define FT_MIDI_A_5           	82
#define FT_MIDI_B5            	83
#define FT_MIDI_C6            	84
#define FT_MIDI_C_6           	85
#define FT_MIDI_D6            	86
#define FT_MIDI_D_6           	87
#define FT_MIDI_E6            	88
#define FT_MIDI_F6            	89
#define FT_MIDI_F_6           	90
#define FT_MIDI_G6            	91
#define FT_MIDI_G_6           	92
#define FT_MIDI_A6            	93
#define FT_MIDI_A_6           	94
#define FT_MIDI_B6            	95
#define FT_MIDI_C7            	96
#define FT_MIDI_C_7           	97
#define FT_MIDI_D7            	98
#define FT_MIDI_D_7           	99
#define FT_MIDI_E7            	100
#define FT_MIDI_F7            	101
#define FT_MIDI_F_7           	102
#define FT_MIDI_G7            	103
#define FT_MIDI_G_7           	104
#define FT_MIDI_A7            	105
#define FT_MIDI_A_7           	106
#define FT_MIDI_B7            	107
#define FT_MIDI_C8            	108

/* GPIO bit macros */

/* Display rotation */
#define FT_DISPLAY_0			0	//0 degrees rotation
#define FT_DISPLAY_180			1	//180 degrees rotation


/* Host command macros */
#define FT_ACTIVE				0x00			// Place FT800 in active state
#define FT_STANDBY				0x41			// Place FT800 in Standby (clk running)
#define FT_SLEEP				0x42			// Place FT800 in Sleep (clk off)
#define FT_PWRDOWN				0x50			// Place FT800 in Power Down (core off)
#define FT_CLKEXT				0x44			// Select external clock source
#define FT_CLKINT				0x48			// Select internal clock source
#define FT_CLK48M				0x62			// Select 48MHz PLL output
#define FT_CLK36M				0x61			// Select 36MHz PLL output
#define FT_CORERST				0x68			// Reset core - all registers default and processors reset


/* Macros for sound play and stop */
#define FT_SOUND_PLAY					1
#define FT_AUDIO_PLAY					1

/* Macros for audio playback parameters*/
#define FT_AUDIO_SAMPLINGFREQ_MIN		8*1000L
#define FT_AUDIO_SAMPLINGFREQ_MAX		48*1000L

//coprocessor error macros
#define FT_COPRO_ERROR					0xfffUL


#ifdef _FT810_ENABLED

#define FT800_CHIPID				0x00010008UL
//#define FT_ROM_CHIPID					0x302000UL
#define FT_ROM_CHIPID				0x0C0000

#define LOW_FREQ_BOUND  58800000L//98% of 60Mhz

#define RAM_G_SIZE			1024*1024L
#define FT81X_VERSION "1.0.4"

/*
#define ADC_DIFFERENTIAL     1UL
#define ADC_SINGLE_ENDED     0UL
#define ADPCM_SAMPLES        2UL
#define ALWAYS               7UL
#define ARGB1555             0UL
#define ARGB2                5UL
#define ARGB4                6UL
#define BARGRAPH             11UL
#define BILINEAR             1UL
#define BITMAPS              1UL
#define BORDER               0UL
#define DECR                 4UL
#define DLSWAP_DONE          0UL
#define DLSWAP_FRAME         2UL
#define DLSWAP_LINE          1UL
#define DST_ALPHA            3UL
#define EDGE_STRIP_A         7UL
#define EDGE_STRIP_B         8UL
#define EDGE_STRIP_L         6UL
#define EDGE_STRIP_R         5UL
#define EQUAL                5UL
#define GEQUAL               4UL
#define GREATER              3UL
#define INCR                 3UL
#define INT_CMDEMPTY         32UL
#define INT_CMDFLAG          64UL
#define INT_CONVCOMPLETE     128UL
#define INT_G8               18UL
#define INT_L8C              12UL
#define INT_PLAYBACK         16UL
#define INT_SOUND            8UL
#define INT_SWAP             1UL
#define INT_TAG              4UL
#define INT_TOUCH            2UL
#define INT_VGA              13UL
#define INVERT               5UL
#define KEEP                 1UL
#define L1                   1UL
#define L2                   17UL
#define L4                   2UL
#define L8                   3UL
#define LEQUAL               2UL
#define LESS                 1UL
#define LINEAR_SAMPLES       0UL
#define LINES                3UL
#define LINE_STRIP           4UL
#define NEAREST              0UL
#define NEVER                0UL
#define NOTEQUAL             6UL
#define ONE                  1UL
#define ONE_MINUS_DST_ALPHA  5UL
#define ONE_MINUS_SRC_ALPHA  4UL
#define RECTS                9UL
*/
//#define OPT_CENTER           1536UL
//#define OPT_CENTERX          512UL
//#define OPT_CENTERY          1024UL
//#define OPT_FLAT             256UL
#define OPT_FULLSCREEN       8UL
#define OPT_MEDIAFIFO        16UL
//#define OPT_MONO             1UL
//#define OPT_NOBACK           4096UL
//#define OPT_NODL             2UL
//#define OPT_NOHANDS          49152UL
//#define OPT_NOHM             16384UL
//#define OPT_NOPOINTER        16384UL
//#define OPT_NOSECS           32768UL
#define OPT_NOTEAR           4UL
//#define OPT_NOTICKS          8192UL
//#define OPT_RIGHTX           2048UL
//#define OPT_SIGNED           256UL
#define OPT_SOUND            32UL
#define PALETTED             8UL
#define PALETTED4444         15UL
#define PALETTED565          14UL
#define PALETTED8            16UL
#define FTPOINTS             2UL

//#define REPEAT               1UL
//#define REPLACE              2UL
//#define RGB332               4UL
//#define RGB565               7UL
#define SRC_ALPHA            2UL
//#define TEXT8X8              9UL
//#define TEXTVGA              10UL
#define TOUCHMODE_CONTINUOUS 3UL
#define TOUCHMODE_FRAME      2UL
#define TOUCHMODE_OFF        0UL
#define TOUCHMODE_ONESHOT    1UL
#define ULAW_SAMPLES         1UL
#define ZERO                 0UL


#define CMDBUF_SIZE          4096UL
#define FT_CMDFIFO_SIZE			CMDBUF_SIZE

#define RAM_G                0UL
#define FT_RAM_CMD              3178496UL		//0x308000
#define FT_RAM_DL               3145728UL		//0x300000
#define RAM_REG              3153920UL			//0x302000
#define RAM_ROMSUB           3186688UL			//0x30A000


#define ROMFONT_TABLEADDRESS 3145724UL

//#define REG_ANALOG           3154284UL
//#define REG_ANA_COMP         3154308UL
//#define REG_BIST_EN          3154292UL
//#define REG_BUSYBITS         3154152UL
//#define REG_CLOCK            3153928UL
//#define REG_CMDB_SPACE       3155316UL
//#define REG_CMDB_WRITE       3155320UL
#define REG_CMD_DL           3154176UL
#define REG_CMD_READ         3154168UL
#define REG_CMD_WRITE        3154172UL		//0x3020FC
#define REG_CPURESET         3153952UL
//#define REG_CRC              3154296UL
//#define REG_CSPREAD          3154024UL
//#define REG_CTOUCH_EXTENDED  3154184UL
//modified below to reflect the same as datasheet
#define REG_CTOUCH_TOUCH0_XY 3154212UL
#define REG_CTOUCH_TOUCH4_X  3154284UL
#define REG_CTOUCH_TOUCH4_Y  3154208UL
#define REG_CTOUCH_TOUCH1_XY 3154204UL
#define REG_CTOUCH_TOUCH2_XY 3154316UL
#define REG_CTOUCH_TOUCH3_XY 3154320UL
#define REG_CYA_TOUCH        3154280UL
#define REG_DATESTAMP        3155300UL
#define REG_DITHER           3154016UL
#define REG_DLSWAP           3154004UL		//  0x302054
#define REG_FRAMES           3153924UL
#define REG_FREQUENCY        3153932UL
#define REG_GPIO             3154068UL
#define REG_GPIOX            3154076UL
#define REG_GPIOX_DIR        3154072UL
#define REG_GPIO_DIR         3154064UL
#define REG_HCYCLE           3153964UL
#define REG_HOFFSET          3153968UL
#define REG_HSIZE            3153972UL
#define REG_HSYNC0           3153976UL
#define REG_HSYNC1           3153980UL
#define REG_ID               3153920UL		// 0x302000
#define REG_INT_EN           3154092UL
#define REG_INT_FLAGS        3154088UL
#define REG_INT_MASK         3154096UL
#define REG_MACRO_0          3154136UL
#define REG_MACRO_1          3154140UL
#define REG_MEDIAFIFO_READ   3182612UL
#define REG_MEDIAFIFO_WRITE  3182616UL
#define REG_OUTBITS          3154012UL
#define REG_PATCHED_ANALOG   3154288UL
#define REG_PATCHED_TOUCH_FAULT 3154284UL
#define REG_PCLK             3154032UL
#define REG_PCLK_POL         3154028UL
#define REG_PLAY             3154060UL
#define REG_PLAYBACK_FORMAT  3154116UL
#define REG_PLAYBACK_FREQ    3154112UL
#define REG_PLAYBACK_LENGTH  3154104UL
#define REG_PLAYBACK_LOOP    3154120UL
#define REG_PLAYBACK_PLAY    3154124UL
#define REG_PLAYBACK_READPTR 3154108UL
#define REG_PLAYBACK_START   3154100UL
#define REG_PWM_DUTY         3154132UL
#define REG_PWM_HZ           3154128UL
#define REG_RENDERMODE       3153936UL
#define REG_ROMSUB_SEL       3154160UL
#define REG_ROTATE           3154008UL
#define REG_SNAPFORMAT       3153948UL
#define REG_SNAPSHOT         3153944UL
#define REG_SNAPY            3153940UL
#define REG_SOUND            3154056UL
#define REG_SPI_EARLY_TX     3154300UL
#define REG_SPI_WIDTH        3154312UL
#define REG_SWIZZLE          3154020UL
#define REG_TAG              3154044UL
#define REG_TAG_X            3154036UL
#define REG_TAG_Y            3154040UL
#define REG_TAP_CRC          3153956UL
#define REG_TAP_MASK         3153960UL
#define REG_TOUCH_ADC_MODE   3154184UL
#define REG_TOUCH_CHARGE     3154188UL
#define REG_TOUCH_DIRECT_XY  3154316UL
#define REG_TOUCH_DIRECT_Z1Z2 3154320UL
#define REG_TOUCH_FAULT      3154288UL
#define REG_TOUCH_MODE       3154180UL
#define REG_TOUCH_OVERSAMPLE 3154196UL
#define REG_TOUCH_RAW_XY     3154204UL
#define REG_TOUCH_RZ         3154208UL
#define REG_TOUCH_RZTHRESH   3154200UL
#define REG_TOUCH_SCREEN_XY  3154212UL
#define REG_TOUCH_SETTLE     3154192UL
#define REG_TOUCH_TAG        3154220UL
#define REG_TOUCH_TAG1       3154228UL
#define REG_TOUCH_TAG1_XY    3154224UL
#define REG_TOUCH_TAG2       3154236UL
#define REG_TOUCH_TAG2_XY    3154232UL
#define REG_TOUCH_TAG3       3154244UL
#define REG_TOUCH_TAG3_XY    3154240UL
#define REG_TOUCH_TAG4       3154252UL
#define REG_TOUCH_TAG4_XY    3154248UL
#define REG_TOUCH_TAG_XY     3154216UL
#define REG_TOUCH_TRANSFORM_A 3154256UL
#define REG_TOUCH_TRANSFORM_B 3154260UL
#define REG_TOUCH_TRANSFORM_C 3154264UL
#define REG_TOUCH_TRANSFORM_D 3154268UL
#define REG_TOUCH_TRANSFORM_E 3154272UL
#define REG_TOUCH_TRANSFORM_F 3154276UL
#define REG_TRACKER          3182592UL
#define REG_TRACKER_1        3182596UL
#define REG_TRACKER_2        3182600UL
#define REG_TRACKER_3        3182604UL
#define REG_TRACKER_4        3182608UL
#define REG_TRIM             3154304UL
#define REG_VCYCLE           3153984UL
#define REG_VOFFSET          3153988UL
#define REG_VOL_PB           3154048UL
#define REG_VOL_SOUND        3154052UL
#define REG_VSIZE            3153992UL
#define REG_VSYNC0           3153996UL
#define REG_VSYNC1           3154000UL



#define CMD_APPEND           4294967070UL
#define CMD_BGCOLOR          4294967049UL
#define CMD_BITMAP_TRANSFORM 4294967073UL
#define CMD_BUTTON           4294967053UL
#define CMD_CALIBRATE        4294967061UL
#define CMD_CLOCK            4294967060UL
#define CMD_COLDSTART        4294967090UL
#define CMD_CRC              4294967043UL
#define CMD_CSKETCH 		 4294967093UL
#define CMD_DIAL             4294967085UL
#define CMD_DLSTART          4294967040UL  // 0xFFFFFF00
#define CMD_EXECUTE          4294967047UL
#define CMD_FGCOLOR          4294967050UL
#define CMD_GAUGE            4294967059UL
#define CMD_GETMATRIX        4294967091UL
#define CMD_GETPOINT         4294967048UL
#define CMD_GETPROPS         4294967077UL
#define CMD_GETPTR           4294967075UL
#define CMD_GRADCOLOR        4294967092UL
#define CMD_GRADIENT         4294967051UL
#define CMD_HAMMERAUX        4294967044UL
#define CMD_IDCT_DELETED     4294967046UL
#define CMD_INFLATE          4294967074UL
#define CMD_INTERRUPT        4294967042UL
#define CMD_INT_RAMSHARED    4294967101UL
#define CMD_INT_SWLOADIMAGE  4294967102UL
#define CMD_KEYS             4294967054UL
#define CMD_LOADIDENTITY     4294967078UL
#define CMD_LOADIMAGE        4294967076UL
#define CMD_LOGO             4294967089UL
#define CMD_MARCH            4294967045UL
#define CMD_MEDIAFIFO        4294967097UL
#define CMD_MEMCPY           4294967069UL
#define CMD_MEMCRC           4294967064UL
#define CMD_MEMSET           4294967067UL
#define CMD_MEMWRITE         4294967066UL
#define CMD_MEMZERO          4294967068UL
#define CMD_NUMBER           4294967086UL
#define CMD_PLAYVIDEO        4294967098UL
#define CMD_PROGRESS         4294967055UL
#define CMD_REGREAD          4294967065UL
#define CMD_ROMFONT          4294967103UL
#define CMD_ROTATE           4294967081UL
#define CMD_SCALE            4294967080UL
#define CMD_SCREENSAVER      4294967087UL
#define CMD_SCROLLBAR        4294967057UL
#define CMD_SETBASE          4294967096UL
#define CMD_SETBITMAP        4294967107UL
#define CMD_SETFONT          4294967083UL
#define CMD_SETFONT2         4294967099UL
#define CMD_SETMATRIX        4294967082UL
#define CMD_SETROTATE        4294967094UL
#define CMD_SETSCRATCH       4294967100UL
#define CMD_SKETCH           4294967088UL
#define CMD_SLIDER           4294967056UL
#define CMD_SNAPSHOT         4294967071UL
#define CMD_SNAPSHOT2        4294967095UL
#define CMD_SPINNER          4294967062UL
#define CMD_STOP             4294967063UL
#define CMD_SWAP             4294967041UL
#define CMD_SYNC             4294967106UL
#define CMD_TEXT             4294967052UL
#define CMD_TOGGLE           4294967058UL
#define CMD_TOUCH_TRANSFORM  4294967072UL
#define CMD_TRACK            4294967084UL
#define CMD_TRANSLATE        4294967079UL
#define CMD_VIDEOFRAME       4294967105UL
#define CMD_VIDEOSTART       4294967104UL

#define DISPLAY() ((0UL<<24))
#define VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define TAG(s) ((3UL<<24)|(((s)&255UL)<<0))
#define COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&2047UL)<<11)|(((y)&2047UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&4095UL)<<12)|(((height)&4095UL)<<0))
#define CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define END() ((33UL<<24))
#define SAVE_CONTEXT() ((34UL<<24))
#define RESTORE_CONTEXT() ((35UL<<24))
#define RETURN() ((36UL<<24))
#define MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
#define CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))

// FT81X reserved commands ...
#define VERTEX_FORMAT(frac) ((39UL<<24)|(((frac)&7UL)<<0))
#define BITMAP_LAYOUT_H(linestride,height) ((40UL<<24)|(((linestride)&3UL)<<2)|(((height)&3UL)<<0))
#define BITMAP_SIZE_H(width,height) ((41UL<<24)|(((width)&3UL)<<2)|(((height)&3UL)<<0))
#define PALETTE_SOURCE(addr) ((42UL<<24)|(((addr)&4194303UL)<<0))
#define VERTEX_TRANSLATE_X(x) ((43UL<<24)|(((x)&131071UL)<<0))
#define VERTEX_TRANSLATE_Y(y) ((44UL<<24)|(((y)&131071UL)<<0))
#define NOP() ((45UL<<24))







#endif /* _FT810_ENABLED */



#endif   /* _FT81`0_H_ */

