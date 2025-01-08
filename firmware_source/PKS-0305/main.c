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
//    Filename:            main.c
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
//    0203 - 03-01-08 - ME
//    -- modified 'comm_gen.c' to version 0003
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 

#include "project.h"
#include <stdio.h>
//#include "stimers.h"
#include "cbuf.h"
//#include "ee.h"
#include "led2.h"
#include "timer0.h"
#include "timer3.h"
#include "comm_gen.h"
#include "comm_i2cm.h"
#include "comm_spi.h"
#include "comm_usart.h"
#include "comm_i2cs.h"
#include "comm_lin.h"
#include "exec.h"
#include "vsrc.h"
#include "usb.h"
#include "usb_data_constants.h"

//------------------------------------------------
//--- VECTOR(s)
//------------------------------------------------
#define  VECTOR_RESET                  0x2000
#define  VECTOR_ISR_HI                 0x2008
#define  VECTOR_ISR_LO                 0x2018

//--- PROTOTYPES
void isr_lo(void);
void isr_hi(void);


#pragma udata
TD_BYTE temp_1, temp_2;


/*
;-------------------------------------------------
;-------------------------------------------------
*/

#pragma code

void main(void)
{
	unsigned char i, cdata;
   TD_WORD_W   temp_W;
   TD_WORD     temp;
   
   //=============================================
   //--- TEST GROUNDS - 1 of 2
   //=============================================
   Nop();
   //=============================================
#ifdef __DEBUG
   temp = (2 / TIMER3_MSEC);
   Nop();
   timer3_set(temp);
   Nop();
   timer3_set((TD_WORD)(2 / TIMER3_MSEC));
   Nop();
   timer3_wait(10.5 / TIMER3_MSEC);
   Nop();
#endif
   
   //=============================================
   //--- INITIALIZE
   //=============================================
   usb_init();
   led_init();
   timer0_init();
   exec_init();
   Nop();
   Nop();
   Nop();
   Nop();
      
   //-------------------------
   //--- INTERRUPTS
   //-------------------------
   RCONbits.IPEN = 1;
   INTCONbits.GIEH = 1;
   INTCONbits.GIEL = 1;

   //=============================================
   //--- DEBUG / DIAG
   //=============================================

   //-------------------------
   //--- DEBUG MODE
   //-------------------------
#ifdef __DEBUG_MODE
   Nop();
//   comm_init_mode(COMM_MODE_LIN);
   exec_cmnd_proc(EXEC_CMND_COMM_INIT_MODE);
#endif

   //-------------------------
   //--- EVENT
   //-------------------------
#ifdef __DEBUG_EVENT
   DEBUG_EVENT_TRIS = 0;
   DEBUG_EVENT_PIN = 0;
#endif

   //-------------------------
   //--- CBUF
   //-------------------------
#ifdef __DEBUG_CBUF_CLEAR
   cbuf2_init();
   for(temp_1=0; temp_1<CBUF2_LEN; temp_1++)
      cbuf2_put(0x00);
   cbuf2_init();
   //cbuf3_init();
   for(temp_1=0; temp_1<CBUF3_LEN; temp_1++)
      cbuf3_put(0x00);
   cbuf3_init();
#endif
   
   //=============================================
   //--- TEST GROUNDS - 2 of 2
   //=============================================
#ifdef __DEBUG
   Nop();
   //comm_lin_test();
#endif
   
   //=============================================
   //--- IDLE LOOP
   //=============================================
   while(1)
   {

      usb_svc();

      //timer0_svc();

      led_svc();

      exec_svc();

      comm_svc();

      EXEC_CB_NEW_FLAG = 0;
      
      //debug
      /*
      if(timer0_C0 == 0)
      {
         timer0_C0 = 10;
         cbuf1_put(0xC1);
         cbuf1_put(0x01);
         cbuf1_put(0x56);
      }
      */
         

   };   
}


   //=============================================
   //--- DIAG - UTILITIES
   //=============================================
#ifdef   __DEBUG_EVENT
void debug_event_toggle(void)
{
   if(DEBUG_EVENT_PIN == 1)
      DEBUG_EVENT_PIN = 0;
   else
      DEBUG_EVENT_PIN = 1;
}

void debug_event_set(void)
{
   DEBUG_EVENT_PIN = 1;
}
   
void debug_event_rst(void)
{
   DEBUG_EVENT_PIN = 0;
}
   
#endif


//==========================================================
//--- VECTORS
//==========================================================

//===========================================================
//===========================================================
#ifndef BOOT_PRESENT
//--------------------------------------
//--- ISR VECTOR: HIGH PRIORITY
//--------------------------------------
#pragma code __vector_isr_hi = 0x08
void __vector_isr_hi(void)
{
   {_asm CALL VECTOR_ISR_HI, 1 _endasm}  // bug fix
}
//--------------------------------------
//--- ISR VECTOR: LOW PRIORITY
//--------------------------------------
#pragma code __vector_isr_lo = 0x18
void __vector_isr_lo(void)
{
   {_asm GOTO VECTOR_ISR_LO _endasm}
}
#endif

//===========================================================
//=== VECTOR(s)
//===========================================================


//--------------------------------------
//--- RESET
//--------------------------------------
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _vector_reset = VECTOR_RESET
void _vector_reset (void)
{
    _asm 
    goto _startup 
    _endasm
}

//--------------------------------------
//--- ISR: VECTOR: HIGH PRIORITY
//--------------------------------------
#pragma code _vector_isr_hi = VECTOR_ISR_HI
void vector_isr_hi(void)
{
   _asm 
   POP
   GOTO isr_hi
    _endasm
}
      
//--------------------------------------
//--- ISR: VECTOR: LOW PRIORITY
//--------------------------------------
#pragma code _vector_isr_lo = VECTOR_ISR_LO
void vector_isr_lo(void)
{
   _asm 
   GOTO isr_lo 
   _endasm

}

//===========================================================
//=== ISR
//===========================================================

//--------------------------------------
//--- ISR: HI PRIORITY
//--------------------------------------
#pragma code
#pragma interrupt isr_hi
void isr_hi(void)
{}
   
//--------------------------------------
//--- ISR: LO PRIORITY
//--------------------------------------
#pragma interruptlow isr_lo
void isr_lo(void)
{
   if((PIE1bits.SSPIE)&&(PIR1bits.SSPIF))
   {
      Nop();
      comm_i2cs_isr();
   }

   timer0_svc();

   if((PIE1bits.RCIE)&&(PIR1bits.RCIF))
   {
      if(COMM_SB_MODE_REG == COMM_MODE_LIN)
         comm_lin_rx_isr();
      if(COMM_SB_MODE_REG == COMM_MODE_USART_A)
         comm_usart_rcv_isr();
   }

   if((PIE1bits.TXIE)&&(PIR1bits.TXIF))
   {
      if(COMM_SB_MODE_REG == COMM_MODE_LIN)
         comm_lin_tx_isr();
   }

}

//==========================================================
//--- BOOT MARKER
//==========================================================
#ifdef BOOT_MARKER_ENABLE
#pragma code
#pragma romdata _boot_marker_addr = BOOT_MARKER_ADDR
rom word _boot_marker = BOOT_MARKER;
#endif   
