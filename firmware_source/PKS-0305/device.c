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
//    Filename:            device.c
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

#include "project.h"

//--------------------------------------
//--- DEVICE CONFIGURATION
//--------------------------------------
#pragma config PLLDIV=5          // 96 mhz pll prescaler
#pragma config CPUDIV=OSC1_PLL2  // cpu system clock postscaler
#pragma config USBDIV=2          // full-speed USB clock source selection
#pragma config FOSC=HS           // oscillator selection bits
///#pragma config FCMEM = OFF       // fail-safe clock monitor
#pragma config IESO = OFF        // internal/external switch over
#pragma config PWRT = OFF        // power up timer
#pragma config BOR = ON          // brown out reset
//#pragma config BORV = 43         // brown out voltage
#pragma config VREGEN=ON         // USB voltage regulator enable
#pragma config WDT=OFF           // watchdog timer
#pragma config WDTPS = 32768     // watchdog postscaler
#pragma config LPT1OSC = OFF     // low power timer1 oscillator enable
#pragma config PBADEN = OFF      // portB a/d enable
#pragma config CCP2MX = OFF      // CCP2 mux bit
#pragma config STVREN = ON       // stack overflow reset
#pragma config LVP=OFF           // low voltage ICSP
//#pragma config ICPRT = OFF       // dedicated in-circuit debug/programming enable
#pragma config XINST = OFF       // extended instruction set enable
#pragma config CP0 = OFF         // code protection: block 0
#pragma config CP1 = OFF         // code protection: block 1
#pragma config CP2 = OFF         // code protection: block 2
#pragma config CP3 = OFF         // code protection: block 3
#pragma config CPB = OFF         // boot block code protection
#pragma config CPD = OFF         // eeprom code protectoin
#pragma config WRT0 = OFF        // write protection: block 0
#pragma config WRT1 = OFF        // write protection: block 1
#pragma config WRT2 = OFF        // write protection: block 2
#pragma config WRT3 = OFF        // write protection: block 3
#pragma config WRTB = OFF        // boot block write protection
#pragma config WRTC = OFF        // configuration register write protection
#pragma config WRTD = OFF        // eeprom write protection
#pragma config EBTR0 = OFF       // table read protection: block 0
#pragma config EBTR1 = OFF       // table read protection: block 1
#pragma config EBTR2 = OFF       // table read protection: block 2
#pragma config EBTR3 = OFF       // table read protection: block 3
#pragma config EBTRB = OFF       // boot block table read protection

#ifdef __DEBUG
#pragma config MCLRE=ON          // MCLR enable
#pragma config DEBUG=ON          // background debugger enable
#else
#pragma config MCLRE=OFF         // MCLR enable
#pragma config DEBUG=OFF         // background debugger enable
#endif

//----------------------------
//--- device_init()
//----------------------------
void device_init()
{
   //--- DEFAULT PORT DATA
   LATA = LATA_DFLT;
   LATB = LATB_DFLT;
   LATC = LATC_DFLT;
   
   //--- DEFAULT PORT DIR
   TRISA = TRISA_DFLT;
   TRISB = TRISB_DFLT;
   TRISC = TRISC_DFLT;
   
   //--- SWITCH
   /*
   SW_PIN_DIR = 1;
   INTCON2bits.NOT_RBPU = 0;
   */
}


   
