//==================================================================== 
//--- COPYRIGHT
//==================================================================== 
// Copyright 2006 Microchip Technology Inc.
// 
// Microchip Technology Inc. ("Microchip") licenses this software to
// you solely for use with Microchip products. The software is owned
// by Microchip and is protected under applicable copyright laws. All
// rights reserved.
// 
// SOFTWARE IS PROVIDED IN AN "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
// WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
// BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
// PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
// BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
// ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
// 
//==================================================================== 
//--- TITLE
//==================================================================== 
//    Filename:            timer3.h
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.xx
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//==================================================================== 
//--- DESCRIPTION
//==================================================================== 
//
// EXAMPLE(S):
// timer3_set((TD_WORD_W) (TIME_MSEC / TIMER3_MSEC));
// where:
//    TIME_MSEC = desired time in MSEC
//
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0001 - 03-18-08 - ME
//    - initial release
//
//==================================================================== 
#ifndef __TIMER3_H
#define __TIMER3_H

//================================================
//--- DEFINITIONS
//================================================

//--- CONFIGURATION (16-bit, PS:8, CLK:INTERNAL)
#define  TIMER3_T3CON_OFF  0xB0
#define  TIMER3_T3CON_ON   0xB1

//--- RESOLUTION
#define  TIMER3_USEC    (1.6)
#define  TIMER3_MSEC    (0.0016)


//================================================
//--- PROTOTYPES
//================================================
void timer3_init(void);
void timer3_set(TD_WORD time);
void timer3_wait(TD_WORD time);

#endif //__TIMER3_H