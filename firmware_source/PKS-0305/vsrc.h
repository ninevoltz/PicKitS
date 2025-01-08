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
//    Filename:            vsrc.h
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
#ifndef __VSRC_H
#define __VSRC_H

//---
#include "exec.h"

//----------------------------------------------------------
//--- RESOURCES
//----------------------------------------------------------
#define  VSRC_CONTROL_TRIS    TRISCbits.TRISC2
#define  VSRC_CONTROL_PIN     PORTCbits.RC2

#define  VSRC_READ_TRIS       TRISAbits.TRISA0
#define  VSRC_READ_PIN        PORTAbits.RA0;


//----------------------------------------------------------
//--- PROTOTYPES
//----------------------------------------------------------
void vsrc_init(void);
void vsrc_set_on(void);
void vsrc_set_off(void);
void vsrc_pwm_off(void);
void vsrc_set_var(TD_BYTE setpoint);
TD_BYTE vsrc_read(TD_BYTE fault_level);


//----------------------------------------------------------
//--- DEFINITIONS / DECLARATIONS
//----------------------------------------------------------
extern TD_BYTE_B  vsrc_flags_B;
extern TD_BYTE    vsrc_fault_count;

//--- FLAGS
#define  VSRC_SET_FLAG        vsrc_flags_B.bit2
#define  VSRC_FAULT_FLAG      vsrc_flags_B.bit1
#define  VSRC_PWM_FLAG        vsrc_flags_B.bit0

//--- CONFIG REGS
#define VSRC_PR2           115      // PR2 register
#define VSRC_T2CON_ON      0x04     // TMR2:ON (PS:1)
#define VSRC_T2CON_OFF     0x00     // TMR2:OFF (PS:1)
#define VSRC_CCP1CON_ON    0x0C     // PWM on
#define VSRC_CCP1CON_OFF   0x00     // PWM off

#define VSRC_FAULT_COUNT_TARGET  3

//------------------------------------------------
//--- ADCON0: CH:0
//------------------------------------------------
#define  VSRC_ADCON0    0x03

//------------------------------------------------
//--- ADCON2: JUST:L, TAQ:20TAD, TAD:FOSC/16
//------------------------------------------------
#define  VSRC_ADCON2    0x3D

#endif // __VSRC_H
