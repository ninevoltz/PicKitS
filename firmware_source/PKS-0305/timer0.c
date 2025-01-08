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
//    Filename:            timer0.c
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
//--- SOFT TIMERS
//
//     uses TMR0 to manage 1-byte "soft" timers
//     timers are decremented until they reach zero
//     calling routine loads timer then 
//       monitors that timer for zero 
//
//     class "A" timers
//     are decremented when TMR0 rolls over
//
//     class "B" timers
//     are decremented when timer0_AB rolls over
//     timer0_AB must be configured for the desired resolution
//
//--- function: timer0_init()
//        initializes TMR0
//        configured for desired resolution of "A" timers
//
//--- function: timer0_svc()
//        must be called at regular intervals
//        decrements soft timers on TMR0 rollover
//
//-----------------------------------------------------------
//--- CONFIGURATION
//-----------------------------------------------------------
//        1. declare/allocate timers as needed
//        2. declare allocated timers "extern" in "timer0.h"
//        3. modify "timer0_init()" for desired TMR0 operation
//        4. modify "timer0_svc()" to service all allocated timers
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
#include "timer0.h"

#pragma udata
//================================================
//--- DECLARE/ALLOCATION TIMER(s)
//------------------------------------------------
TD_BYTE timer0_A0;         // 
TD_BYTE timer0_A1;         // i2cm adc timer
TD_BYTE timer0_A2;         // led service interval
TD_BYTE timer0_A3;         // vsrc read interval
TD_BYTE timer0_A4;         // exec cbuf2 flush timer / reboot timer
TD_BYTE timer0_B0;         // 
TD_BYTE timer0_B1;         // 
TD_BYTE timer0_B2;         //
TD_BYTE timer0_B3;         //
TD_BYTE timer0_B4;         //
TD_BYTE timer0_C0;         //
TD_BYTE timer0_C1;         //
TD_WORD_W   timer0_AB0_W;  // script timer
TD_WORD_W   timer0_AB1_W;  // script timer
TD_WORD_W   timer0_AB2_W;  //
TD_WORD_W   timer0_AB3_W;  //

//--- 
TD_BYTE timer0_AB;
TD_BYTE timer0_BC;

//---
TD_BYTE_B timer0_flags_B;
//---
TD_BYTE timer0_reg[3];

//--- resolution of "B" timers in units of "A"
//#define  TIMER0_A_USEC  409
#define  TIMER0_B_MSEC  (TIMER0_A_USEC * 256 / 1000)
#define  TIMER0_C_MSEC  (1000)
//#define  TIMER0_AB_TA   TIMER0_B_MSEC * 1000.0 / TIMER0_A_USEC
#define  TIMER0_BC_TB   (TIMER0_C_MSEC / TIMER0_B_MSEC)

#pragma code
//==========================================================
//--- timer0_init()
//    initialize TMR0 for use w/ soft timers
//==========================================================
void timer0_init(void)
{
   T0CON = 0;                 // rst timer0 control register
   INTCONbits.TMR0IE = 0;     // rst ineterrupt enable
   INTCON2bits.TMR0IP = 0;    // set priority
   INTCONbits.TMR0IF = 0;     // rst interrupt flag
   INTCONbits.TMR0IE = 1;     // set interrupt enable
   T0CON = TIMER0_T0CON_DFLT; // set prescale, enable timer

   //timer0_AB = -TIMER0_AB_TA;
   timer0_BC = -TIMER0_BC_TB;
   
   timer0_reg[0] = 0;
   timer0_reg[1] = 0;
   timer0_reg[2] = 0;
   timer0_flags_B._byte = 0;
}

//==========================================================
//--- timer0_svc() - process timers
//    decrement soft timers until zero
//==========================================================
void timer0_svc(void)
{
	if (INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF = 0;

//--- process time register
      timer0_reg[1]++;
      if(not timer0_reg[1])
      {
         timer0_reg[2]++;
         if(not timer0_reg[2])
            TIMER0_REG_OVF_FLAG = 1;
      }
      
   if(timer0_AB0_W._word)
      timer0_AB0_W._word--;
   if(timer0_AB1_W._word)
      timer0_AB1_W._word--;
   if(timer0_AB2_W._word)
      timer0_AB2_W._word--;
   if(timer0_AB3_W._word)
      timer0_AB3_W._word--;
      
//--- process timers
      if(timer0_A0)
         timer0_A0--;
      if(timer0_A1)
         timer0_A1--;
      if(timer0_A2)
         timer0_A2--;
      if(timer0_A3)
         timer0_A3--;
      if(timer0_A4)
         timer0_A4--;
      if(not ++timer0_AB)
      {
         //timer0_AB = -TIMER0_AB_TA;
         if(timer0_B0)
            timer0_B0--;
         if(timer0_B1)
            timer0_B1--;
         if(timer0_B2)
            timer0_B2--;
         if(timer0_B3)
            timer0_B3--;
         if(timer0_B4)
            timer0_B4--;
         if(not ++timer0_BC)
         {
            timer0_BC = -TIMER0_BC_TB;
            if(timer0_C0)
               timer0_C0--;
            if(timer0_C1)
               timer0_C1--;
         }
      }
	}
}
	

//================================================
//--- timer0_reg_read()
//================================================
void timer0_reg_read(void)
{
   INTCONbits.TMR0IE = 0;
   timer0_reg[0] = TMR0L;
   if(INTCONbits.TMR0IF)
      timer0_reg[0] = 0xFF;
   if(TIMER0_REG_OVF_FLAG)
   {
      TIMER0_REG_OVF_SYNC_FLAG = 1;
      TIMER0_REG_OVF_FLAG = 0;
   }
   INTCONbits.TMR0IE = 1;
}
      
//================================================
//--- timer0_reg_clear()
//================================================
void timer0_reg_clear(void)
{
   INTCONbits.TMR0IE = 0;
   timer0_reg[1] = 0;
   timer0_reg[2] = 0;
   TIMER0_REG_OVF_FLAG = 0;
   TIMER0_REG_OVF_SYNC_FLAG = 0;
   INTCONbits.TMR0IE = 1;
}   
