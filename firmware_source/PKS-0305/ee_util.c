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
//    Filename:            ee_util.c
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

//==========================================================
//=== EEPROM UTILITIES
//==========================================================
#include "project.h"
#include "ee_util.h"

#pragma udata
TD_WORD_W   ee_data_W;

#pragma  code

//--------------------------------------
//--- ee_read()
//--------------------------------------
TD_WORD ee_read16(static TD_BYTE arg_addr)
{
   ee_read(arg_addr+1);
   ee_data_W.hi = ee_data_W.lo;
   ee_read(arg_addr);
}

//--------------------------------------
//--- ee_read()
//--------------------------------------
TD_BYTE ee_read(static TD_BYTE arg_addr)
{
   EEADR = arg_addr;
   EECON1 = 0;
   EECON1bits.RD = 1;
   ee_data_W.lo = EEDATA;
   return(EEDATA);
}
   
//--------------------------------------
//--- ee_write_na()
//--------------------------------------
void ee_write_na(void)
{
   EECON1 = 0;
   do
   {
      INTCONbits.GIE = 0;
   }  
   while( INTCONbits.GIE);
   
   EECON1bits.WREN = 1;
   EECON2 = 0x55;
   EECON2 = 0xAA;
   EECON1bits.WR  = 1;
   INTCONbits.GIE = 1;
   while(EECON1bits.WR){};
   EECON1bits.WREN = 0;
   EEADR++;
}

//--------------------------------------
//--- ee_write()
//--------------------------------------
void ee_write(static TD_BYTE arg_addr, static TD_BYTE arg_data)
{
   EEADR  = arg_addr;
   EEDATA = arg_data;
   ee_write_na();
}
