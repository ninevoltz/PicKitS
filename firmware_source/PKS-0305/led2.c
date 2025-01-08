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
//    Filename:            led2.c
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
//    LED2
//    functionality:
//       turn LED ON/OFF immediately
//       flash once           ON-timed    OFF-indefnitely)
//       flash once           OFF-timed   ON-indefinitely)
//       flash indefinitely   ON-timed    OFF-timed
//
//    2 8-bit registers are associated with each LED
//    reg1: configuration
//       7:6   mode
//       5:5   LED state - initial
//       4:0   time (100 msec resolution) 
//
//    reg2: control
//       7:6   n/a
//       5:5   LED state
//       4:0   timer
//
//    led_1_config()
//    led_2_config()
//    led_3_config()
//       loads the configuration register and inits the I/O pin
//
//    led_svc()
//       is called from the main idle loop
//       to process all active LEDs
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
#include "led2.h"

//--- DECLARATIONS
#pragma udata led
TD_BYTE_B   led_1_conf_B, led_1_oper_B;
TD_BYTE_B   led_2_conf_B, led_2_oper_B;
//TD_BYTE_B   led_3_conf_B, led_3_oper_B;

//================================================
//--- led_init()
//================================================
void led_init(void)
{
   LED_1_TRIS = 0;
   LED_2_TRIS = 0;
   //LED_3_TRIS_BIT = 0;
   led_1_config(LED_1_CONFIG_DFLT);
   led_2_config(LED_2_CONFIG_DFLT);
   //led3_config(LED_3_CONFIG_DFLT);
}

//================================================
//--- led_1_config()
//================================================
void led_1_config(TD_BYTE config)
{
   led_1_conf_B._byte = config;
   led_1_oper_B._byte = config;
   if(not (led_1_conf_B._byte & LED_MASK_MODE))
   {
      if(led_1_conf_B.bit5)
         LED_1_PIN = 1;
      else
         LED_1_PIN = 0;
   }
}

//================================================
//--- led_2_config()
//================================================
void led_2_config(TD_BYTE config)
{
   led_2_conf_B._byte = config;
   led_2_oper_B._byte = config;
   if(not (led_2_conf_B._byte & LED_MASK_MODE))
   {
      if(led_2_conf_B.bit5)
         LED_2_PIN = 1;
      else
         LED_2_PIN = 0;
   }
}

//================================================
//--- led_3_config()
//================================================
/*
void led_3_config(TD_BYTE config)
{
   led_3_conf_B._byte = config;
   led_3_oper_B._byte = config;
   if(not (led_3_conf_B._byte & LED_MASK_MODE))
   {
      if(led_3_conf_B.bit5)
         LED_3_PIN = 1;
      else
         LED_3_PIN = 0;
   }
}
*/

//================================================
//--- led_svc()
//================================================
void led_svc(void)
{
   //-----------------------------------
   //--- TIMER
   //-----------------------------------
   if(LED_TIMER)
      return;
   LED_TIMER = LED_TIMER_SET_T;
   
   //-----------------------------------
   //--- LED1
   //-----------------------------------
   if(led_1_oper_B.bit7)
   {
      if(led_1_oper_B._byte & LED_MASK_TIME)
         led_1_oper_B._byte--;
      else
      {
         led_1_oper_B._byte &= LED_MASK_STATE;
         led_1_oper_B._byte ^= LED_MASK_STATE;
         led_1_oper_B._byte |= led_1_conf_B._byte & ~LED_MASK_STATE;
         if(not led_1_oper_B.bit6)
            led_1_oper_B.bit7 = 0;
         if(led_1_oper_B.bit5)
            LED_1_PIN = 1;
         else
            LED_1_PIN = 0;
      }
      if(led_1_oper_B.bit5)
         LED_1_PIN = 1;
      else
         LED_1_PIN = 0;
   }
   
   //-----------------------------------
   //--- LED2
   //-----------------------------------
   if(led_2_oper_B.bit7)
   {
      if(led_2_oper_B._byte & LED_MASK_TIME)
         led_2_oper_B._byte--;
      else
      {
         led_2_oper_B._byte &= LED_MASK_STATE;
         led_2_oper_B._byte ^= LED_MASK_STATE;
         led_2_oper_B._byte |= led_2_conf_B._byte & ~LED_MASK_STATE;
         if(not led_2_oper_B.bit6)
            led_2_oper_B.bit7 = 0;
         if(led_2_oper_B.bit5)
            LED_2_PIN = 1;
         else
            LED_2_PIN = 0;
      }
      if(led_2_oper_B.bit5)
         LED_2_PIN = 1;
      else
         LED_2_PIN = 0;
   }
   
   //-----------------------------------
   //--- LED3
   //-----------------------------------
   /*
   if(led_3_oper_B.bit7)
   {
      if(led_3_oper_B._byte & LED_MASK_TIME)
         led_3_oper_B._byte--;
      else
      {
         led_3_oper_B._byte &= LED_MASK_STATE;
         led_3_oper_B._byte ^= LED_MASK_STATE;
         led_3_oper_B._byte |= led_3_conf_B._byte & ~LED_MASK_STATE;
         if(not led_3_oper_B.bit6)
            led_3_oper_B.bit7 = 0;
         if(led_3_oper_B.bit5)
            LED_3_PIN = 1;
         else
            LED_3_PIN = 0;
      }
      if(led_3_oper_B.bit5)
         LED_3_PIN = 1;
      else
         LED_3_PIN = 0;
   }
   */
}
