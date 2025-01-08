//===================================================================== 
//--- COPYRIGHT
//===================================================================== 
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
//===================================================================== 
//--- TITLE
//===================================================================== 
//    Filename:            cbuf.c
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.00
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//===================================================================== 
//--- DESCRIPTION
//===================================================================== 
//
//--- CIRCULAR BUFFER
//    
//    get & put byte from a circular buffer
//    maximum buffer size = 255 (0xFF)
//    buffer definition structure (CBUF) defines:
//       read     read pointer
//       write    write pointer
//       used     # bytes used
//       unused   # bytes unused
//    caller must check data/buffer availability
//    prior to calling "get"/"put"
//
//    multiple buffers can be defined by
//    duplicating "CBUF1" sections in cbuf.c & cbuf.h
//       change duplicate: "cbuf1" -> "cbuf2"
//       change duplicate: "CBUF1" -> "CBUF2"
//
//===================================================================== 
//--- HISTORY
//===================================================================== 
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//===================================================================== 
#include "project.h"
#include "cbuf.h"

//--------------------------------------
//--------------------------------------
//--- CBUF1: START
//--------------------------------------
//--------------------------------------
#pragma udata cbuf1_def
struct CBUF cbuf1;

#pragma udata cbuf1_buf
TD_BYTE cbuf1_data[CBUF1_LEN];

#pragma code cbuf1_code
//----------------------------
//--- cbuf1_init()
//----------------------------
void cbuf1_init()
{
   cbuf1.write = 0;
   cbuf1.read = 0;
   cbuf1.used = 0;
   cbuf1.unused = CBUF1_LEN;
}

//----------------------------
//--- cbuf1_get()
//----------------------------
TD_BYTE cbuf1_get()
{
   if(cbuf1.used)
   {
      cbuf1.read += 1;
      if(cbuf1.read == CBUF1_LEN)
         cbuf1.read = 0;
      cbuf1.used--;
      cbuf1.unused++;
   }
   return cbuf1_data[cbuf1.read];
}

//----------------------------
//--- cbuf1_put()
//----------------------------
void cbuf1_put(TD_BYTE data)
{
   if(cbuf1.unused)
   {
      cbuf1.write++;
      if(cbuf1.write == CBUF1_LEN)
         cbuf1.write = 0;
      cbuf1_data[cbuf1.write] = data;
      cbuf1.used++;
      cbuf1.unused--;
   }
   return ;
}
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
#pragma udata cbuf2_def
struct CBUF cbuf2;

#pragma udata cbuf2_buf
TD_BYTE cbuf2_data[CBUF2_LEN];

#pragma code cbuf2_code
//----------------------------
//--- cbuf2_init()
//----------------------------
void cbuf2_init()
{
   cbuf2.write = 0;
   cbuf2.read = 0;
   cbuf2.used = 0;
   cbuf2.unused = CBUF2_LEN;
}

//----------------------------
//--- cbuf2_get()
//----------------------------
TD_BYTE cbuf2_get()
{
   if(cbuf2.used)
   {
      cbuf2.read += 1;
      if(cbuf2.read == CBUF2_LEN)
         cbuf2.read = 0;
      cbuf2.used--;
      cbuf2.unused++;
   }
   return cbuf2_data[cbuf2.read];
}

//----------------------------
//--- cbuf2_put()
//----------------------------
void cbuf2_put(TD_BYTE data)
{
   if(cbuf2.unused)
   {
      cbuf2.write++;
      if(cbuf2.write == CBUF2_LEN)
         cbuf2.write = 0;
      cbuf2_data[cbuf2.write] = data;
      cbuf2.used++;
      cbuf2.unused--;
   }
   return ;
}
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
#pragma udata cbuf3_def
struct CBUF cbuf3;

#pragma udata cbuf3_buf
TD_BYTE cbuf3_data[CBUF3_LEN];

#pragma code cbuf3_code
//----------------------------
//--- cbuf3_init()
//----------------------------
void cbuf3_init()
{
   cbuf3.write = 0;
   cbuf3.read = 0;
   cbuf3.used = 0;
   cbuf3.unused = CBUF3_LEN;
}

//----------------------------
//--- cbuf3_get()
//----------------------------
TD_BYTE cbuf3_get()
{
   if(cbuf3.used)
   {
      cbuf3.read += 1;
      if(cbuf3.read == CBUF3_LEN)
         cbuf3.read = 0;
      cbuf3.used--;
      cbuf3.unused++;
   }
   return cbuf3_data[cbuf3.read];
}

//----------------------------
//--- cbuf3_put()
//----------------------------
void cbuf3_put(TD_BYTE data)
{
   if(cbuf3.unused)
   {
      cbuf3.write++;
      if(cbuf3.write == CBUF3_LEN)
         cbuf3.write = 0;
      cbuf3_data[cbuf3.write] = data;
      cbuf3.used++;
      cbuf3.unused--;
   }
   return ;
}
//--------------------------------------
//--------------------------------------
//--- CBUF3: END
//--------------------------------------
//--------------------------------------
