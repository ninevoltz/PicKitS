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
//    Filename:            timer0.h
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.00
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//==================================================================== 
//--- DESCRIPTION
//==================================================================== 
//
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 
#ifndef __TIMER0_H
#define __TIMER0_H

extern TD_BYTE timer0_A0;
extern TD_BYTE timer0_A1;
extern TD_BYTE timer0_A2;
extern TD_BYTE timer0_A3;
extern TD_BYTE timer0_A4;
extern TD_BYTE timer0_B0;
extern TD_BYTE timer0_B1;
extern TD_BYTE timer0_B2;
extern TD_BYTE timer0_B3;
extern TD_BYTE timer0_B4;
extern TD_BYTE timer0_C0;
extern TD_BYTE timer0_C1;
extern TD_WORD_W timer0_AB0_W;
extern TD_WORD_W timer0_AB1_W;
extern TD_WORD_W timer0_AB2_W;
extern TD_WORD_W timer0_AB3_W;

extern TD_BYTE    timer0_reg[3];
extern TD_BYTE_B  timer0_flags_B;

#define  TIMER0_REG_OVF_FLAG        timer0_flags_B.bit0
#define  TIMER0_REG_OVF_SYNC_FLAG   timer0_flags_B.bit1

//--- PROTOTYPES
void timer0_init(void);
void timer0_svc(void);
void timer0_reg_read(void);
void timer0_reg_clear(void);

#endif // __TIMER0_H

