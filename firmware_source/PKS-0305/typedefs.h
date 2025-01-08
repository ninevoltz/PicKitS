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
//    Filename:            typedefs.h
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

#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

//================================================
typedef unsigned char   byte,  TD_BYTE;   // 8-bit
typedef unsigned int    word,  TD_WORD;   // 16-bit
typedef unsigned long   dword, TD_DWORD;  // 32-bit
//================================================


//================================================
//--- BYTE
//================================================
typedef union _BYTE
{
   TD_BYTE _b;
   TD_BYTE _byte;
   struct
   {
      unsigned bit0:1;
      unsigned bit1:1;
      unsigned bit2:1;
      unsigned bit3:1;
      unsigned bit4:1;
      unsigned bit5:1;
      unsigned bit6:1;
      unsigned bit7:1;
   };
   struct
   {
      unsigned b0:1;
      unsigned b1:1;
      unsigned b2:1;
      unsigned b3:1;
      unsigned b4:1;
      unsigned b5:1;
      unsigned b6:1;
      unsigned b7:1;
   };
} BYTE,  TD_BYTE_B;

//================================================
//--- WORD
//================================================
typedef union _WORD
{
   //TD_WORD  word_w;
   TD_WORD  _w;
   TD_WORD  _word;
   struct
   {
      TD_BYTE     byte0;
      TD_BYTE     byte1;
   };
   struct
   {
      TD_BYTE_B   byte0_B;
      TD_BYTE_B   byte1_B;
   };
   struct
   {
      TD_BYTE     lo;
      TD_BYTE     hi;
   };
   struct
   {
      TD_BYTE     _b0;
      TD_BYTE     _b1;
   };
   struct
   {
      TD_BYTE_B   _B0;
      TD_BYTE_B   _B1;
   };
} WORD, TD_WORD_W;


//================================================
//--- DWORD
//================================================
typedef union _DWORD
{
   TD_DWORD       _d;
   struct
   {
      TD_WORD     word0;
      TD_WORD     word1;
   };
   struct
   {
      TD_WORD_W   word0_W;
      TD_WORD_W   word1_W;
   };
   struct
   {
      TD_WORD     _w0;
      TD_WORD     _w1;
   };
   struct
   {
      TD_WORD_W   _W0;
      TD_WORD_W   _W1;
   };
   struct
   {
      TD_BYTE     byte0;
      TD_BYTE     byte1;
      TD_BYTE     byte2;
      TD_BYTE     byte3;
   };
   struct
   {
      TD_BYTE_B   byte0_B;
      TD_BYTE_B   byte1_B;
      TD_BYTE_B   byte2_B;
      TD_BYTE_B   byte3_B;
   };
   struct
   {
      TD_BYTE     _b0;
      TD_BYTE     _b1;
      TD_BYTE     _b2;
      TD_BYTE     _b3;
   };
   struct
   {
      TD_BYTE     _B0;
      TD_BYTE     _B1;
      TD_BYTE     _B2;
      TD_BYTE     _B3;
   };
} DWORD, TD_DWORD_D;

//--- ORIGINAL
typedef void(*pFunc)(void);


//================================================
//--- POINTER
//================================================
typedef union _POINTER
{
   TD_WORD        word_w;
   struct
   {
      TD_BYTE     _b0;
      TD_BYTE     _b1;
   };
   TD_WORD_W      word_W;
   TD_BYTE        *ram_b;
   TD_WORD        *ram_w;
   rom TD_BYTE    *rom_b;
   rom TD_WORD    *rom_w;
} POINTER, TD_POINTER;


//================================================
//--- OPERATORS
//================================================
#define  not   !
#define  and   &&

//================================================
//--- MISC
//================================================
typedef enum _BOOL { FALSE = 0, TRUE } BOOL;
#define OK      TRUE
#define FAIL    FALSE


#endif // __TYPEDEFS_H
