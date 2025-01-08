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
//    Filename:            cbuf.h
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

#ifndef _CBUF_H
#define _CBUF_H

//----------------------------
//--- TYPEDEF
//----------------------------
typedef struct CBUF
{
   TD_BYTE  write;
   TD_BYTE  read;
   TD_BYTE  used;
   TD_BYTE  unused;
};

//--------------------------------------
//--------------------------------------
//--- CBUF1: START
//--------------------------------------
//--------------------------------------

//----------------------------
//--- DEFINES
//----------------------------
extern struct CBUF cbuf1;
#define CBUF1_LEN 255         // max: 255
extern TD_BYTE cbuf1_data[CBUF1_LEN];

//----------------------------
//--- PROTOTYPES
//----------------------------
void cbuf1_init(void);
TD_BYTE cbuf1_get(void);
void cbuf1_put(TD_BYTE data);

//--------------------------------------
//--------------------------------------
//--- CBUF1: END
//--------------------------------------
//--------------------------------------

//--------------------------------------
//--------------------------------------
//--- CBUF2: START
//--------------------------------------
//--------------------------------------

//----------------------------
//--- DEFINES
//----------------------------
extern struct CBUF cbuf2;
#define CBUF2_LEN 255         // max: 255
extern TD_BYTE cbuf2_data[CBUF2_LEN];

//----------------------------
//--- PROTOTYPES
//----------------------------
void cbuf2_init(void);
TD_BYTE cbuf2_get(void);
void cbuf2_put(TD_BYTE data);

//--------------------------------------
//--------------------------------------
//--- CBUF2: END
//--------------------------------------
//--------------------------------------


//--------------------------------------
//--------------------------------------
//--- CBUF3: START
//--------------------------------------
//--------------------------------------

//----------------------------
//--- DEFINES
//----------------------------
extern struct CBUF cbuf3;
#define CBUF3_LEN 255         // max: 255
extern TD_BYTE cbuf3_data[CBUF3_LEN];

//----------------------------
//--- PROTOTYPES
//----------------------------
void cbuf3_init(void);
TD_BYTE cbuf3_get(void);
void cbuf3_put(TD_BYTE data);

//--------------------------------------
//--------------------------------------
//--- CBUF3: END
//--------------------------------------
//--------------------------------------


#endif // _CBUF_H

