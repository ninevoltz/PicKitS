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
//    Filename:            timer3.c
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.ss
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//==================================================================== 
//--- DESCRIPTION
//==================================================================== 
//
//    USED TIMER3 AS A 16-BIT HARD TIMER
//    CONFIGURED FOR 1.6 USEC RESOLUTION
//    (ABOUT 20 USEC OVERHEAD)
//    MAX: 104.9 MSEC  (1.6 USEC * 85535)
// 
//    EXAMPLE(S):
//    
//    SET TIMER FOR 10.5 MSEC
//       timer3_set(10.5 / TIMER3_MSEC);
//
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0001 - 03-18-08 - ME
//    - initial release
//
//==================================================================== 

#include "project.h"
#include "timer3.h"


void timer3_init()
{
   T3CON = TIMER3_T3CON_OFF;
}

void timer3_set(TD_WORD time)
{
   TD_WORD_W   time_W;
   
   time_W._w = time;
   T3CON = TIMER3_T3CON_OFF;
   time_W._w = (0 - time_W._w);
   TMR3H = time_W.hi;
   TMR3L = time_W.lo;
   PIR2bits.TMR3IF = 0;
   T3CONbits.TMR3ON = 1;
}

void timer3_wait(TD_WORD time)
{
   timer3_set(time);
   while(PIR2bits.TMR3IF == 0)
   {}
}

