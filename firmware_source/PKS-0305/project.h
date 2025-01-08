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
//    Filename:            project.h
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
//    FILE: project.h
//
//    NOTE: 0003 04-28-08 ME
//    -- modified firmware version number from 0x0301 to 0x0302
//
//    NOTE: 0002 12-05-07 ME
//    -- modified firmware version number from 0x0201 to 0x0202
//
//    NOTE: 0001 09-01-06 ME
//    -- added firmware version definitions (moved from exec.h)
//    -- modified firmware version from 0x0107 to 0x0108
//
//    NOTE: 0000 08-01-06 ME
//    -- initial release
//
//==================================================================== 
#ifndef __PROJECT_H
#define __PROJECT_H
#include "typedefs.h"
#include "device.h"

//--------------------------------------
//--- DEFAULTS
//--------------------------------------
#ifndef __DEBUG
#define BOOT_PRESENT
#define BOOT_MARKER_ENABLE
#endif

//--------------------------------------
//--- DIAGNOSTIC
//--------------------------------------
//#define __DEBUG                   // DONE IN MPLAB

#ifdef __DEBUG
//#define __DEBUG_TRACE
//#define __DEBUG_MODE                //--- SET MODE ON STARTUP
//#define __DEBUG_CBUF_CLEAR          //--- CLEAR CBUF ON RESET
//#define __DEBUG_EXEC_CBLOCK         //--- USE DEBUG EXEC BLOCK
//#define __DEBUG_EVENT               //--- USE PIN C1 TO SIGNAL EVENT
//#define __DEBUG_SLAVE_RESPx20       //--- ENABLE DEBUG 'SLAVE' RESPONSE TO ID 0x21-0x2F
#endif

//--- 'EVENT' SIGNAL
#ifdef __DEBUG_EVENT
#define  DEBUG_EVENT_PIN   LATCbits.LATC1
#define  DEBUG_EVENT_TRIS  TRISCbits.TRISC1
void debug_event_toggle(void);
void debug_event_set(void);
void debug_event_rst(void);
#endif


//--- BOOTLOADER - MARKER
#define  BOOT_MARKER                0x5555
#define  BOOT_MARKER_ADDR           0x7CFE

//--------------------------------------
//--- FIRMWARE VERSION
//--------------------------------------
#define  EXEC_FIRMWARE_VERSION_MAJOR      0x03
#define  EXEC_FIRMWARE_VERSION_MINOR      0x05


#endif // __PROJECT_H
