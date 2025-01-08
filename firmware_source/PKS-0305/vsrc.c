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
//    Filename:            vsrc.c
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
#include "vsrc.h"

TD_BYTE_B   vsrc_flags_B;
TD_BYTE     vsrc_fault_count;

//==========================================================
//--- vsrc_init()
//    configure VSRC resources
//==========================================================
void vsrc_init(void)
{
   //-----------------------------------
   //--- disable PWM
   //-----------------------------------
   vsrc_pwm_off();
   
   //-----------------------------------
   //--- setup READ/MEASURE
   //-----------------------------------
   VSRC_READ_TRIS = 1;
   if((ADCON1 & 0x0F) == 0x0F)
      ADCON1 = 0x0E;

   //-----------------------------------
   VSRC_CONTROL_TRIS = 0;     // set control pin to output
   vsrc_flags_B._b = 0;       // clear FLAGs
   vsrc_fault_count = 0;
}


//==========================================================
//--- vsrc_read()
//    measure VSRC (ADC channel: 0)
//==========================================================
TD_BYTE vsrc_read(TD_BYTE fault_level)
{
   //-------------------------
   //--- MEASURE VSRC
   //-------------------------
   ADCON2 = VSRC_ADCON2;      // configure ADC
   ADCON0 = VSRC_ADCON0;
   while(ADCON0bits.GO)       // wait for result
   {} 

   //-------------------------
   //--- CHECK FAULT CONDITION
   //-------------------------
   if(VSRC_SET_FLAG)
   {
      if(ADRESH < fault_level)
      {
         if(vsrc_fault_count < VSRC_FAULT_COUNT_TARGET)
            vsrc_fault_count++;
         else
         {
            vsrc_set_off();
            VSRC_FAULT_FLAG = 1;
         }
      }
      else
         vsrc_fault_count = 0;
   }

   //-------------------------
   return(ADRESH);            // return measurement
   
}

//==========================================================
//--- vsrc_set_on()
//    set VSRC control "on"
//==========================================================
void vsrc_set_on(void)
{
   vsrc_pwm_off();
   if(not VSRC_FAULT_FLAG)
   {
      VSRC_CONTROL_PIN = 1;
      VSRC_SET_FLAG = 1;
   }
}


//==========================================================
//--- vsrc_set_off()
//    set VSRC control "off"
//==========================================================
void vsrc_set_off(void)
{
   vsrc_pwm_off();
   VSRC_CONTROL_PIN = 0;
   VSRC_SET_FLAG = 0;
}


//==========================================================
//--- vsrc_set_var()
//    set variable VSRC control
//==========================================================
void vsrc_set_var(TD_BYTE setpoint)
{
   if(not VSRC_FAULT_FLAG)
   {
      //-----------------------------------
      //--- config
      //-----------------------------------
      if(not VSRC_PWM_FLAG)
      {
         PR2 = VSRC_PR2;
         T2CON = VSRC_T2CON_ON;
         CCP1CON = VSRC_CCP1CON_ON;
         VSRC_PWM_FLAG = 1;
      }

      //-----------------------------------
      //--- set
      //-----------------------------------
      CCPR1L = setpoint >> 2;
      if(setpoint & 0x02)
         CCP1CONbits.DC1B1 = 1;
      else
         CCP1CONbits.DC1B1 = 0;
      if(setpoint & 0x01)
         CCP1CONbits.DC1B0 = 1;
      else
         CCP1CONbits.DC1B0 = 0;

      VSRC_SET_FLAG = 1;
   }
}


//==========================================================  
//--- vsrc_pwm_off()
//    disable VSRC PWM (if enabled)
//==========================================================  
void vsrc_pwm_off(void)
{
   if(VSRC_PWM_FLAG)
   {
      T2CON = VSRC_T2CON_OFF;
      CCP1CON = VSRC_CCP1CON_OFF;
      VSRC_PWM_FLAG = 0;
   }
}

