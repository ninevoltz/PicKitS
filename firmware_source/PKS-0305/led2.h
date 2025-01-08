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
//    Filename:            led2.h
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
#ifndef _LED_H
#define _LED_H
#include "timer0.h"

//----------------------------------------------------------
//--- RESOURCES
//    LED1 - RED
//    LED2 - YELLOW
//    LED3 - n/a
//----------------------------------------------------------
/*
#define  LED_1_PIN             LATCbits.LATC0
#define  LED_2_PIN             LATBbits.LATB4
//#define  LED_3_PIN             LATBbits.LATB4
#define  LED_1_TRIS_BIT        TRISCbits.TRISC0
#define  LED_2_TRIS_BIT        TRISBbits.TRISB4
//#define  LED_3_TRIS_BIT        TRISBbits.TRISB4
*/

#define  LED_TIMER            timer0_A2
#define  LED_TIMER_SET_MSEC   50
#define  LED_TIMER_SET_T      LED_TIMER_SET_MSEC * 1000.0 / TIMER0_A_USEC

//----------------------------------------------------------
//--- PROTOTYPES
//----------------------------------------------------------
void led_1_config(TD_BYTE config);
void led_2_config(TD_BYTE config);
void led_3_config(TD_BYTE config);
void led_svc(void);
void led_init(void);

//----------------------------------------------------------
//--- DEFINITIONs/DECLARATIONs
//----------------------------------------------------------
extern TD_BYTE_B led1_conf_B, led_1_oper_B;
extern TD_BYTE_B led1_conf_B, led_1_oper_B;
extern TD_BYTE_B led1_conf_B, led_1_oper_B;
#define LED_MASK_MODE   0xC0
#define LED_MASK_STATE  0x20
#define LED_MASK_TIME   0x1F

#define LED_1_CONFIG_DFLT   0x00
#define LED_2_CONFIG_DFLT   0x00
#define LED_3_CONFIG_DFLT   0x00

#endif //_LED_H

