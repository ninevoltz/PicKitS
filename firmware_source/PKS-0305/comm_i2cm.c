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
//    Filename:            comm_i2cm.c
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.00
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//==================================================================== 
//--- DESCRIPTION
//==================================================================== 
//    I2C MASTER communication controller
//    uses MSSP to implement I2C Communication
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 

//==========================================================
//--- INCLUDES
//==========================================================
#include "project.h"
#include "comm_gen.h"
#include "comm_i2cm.h"
#include "cbuf.h"
#include "timer0.h"
#include "timer3.h"

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
// none


//==========================================================
//--- comm_i2cm_init()
//
//    initialize communications mode: I2CM
//==========================================================
TD_BYTE comm_i2cm_init(void)
{
   //-----------------------------------
   //--- RESET
   //-----------------------------------
   comm_i2cm_reset();
   
   //-----------------------------------
   //--- SETUP MSSP MODULE
   //-----------------------------------
   SSPCON1bits.SSPEN = 0;
   SSPADD = COMM_CB_I2CM_BITRATE_REG;
   SSPCON2 = 0;
   COMM_SB_I2CM_BITRATE_REG = SSPADD;
   COMM_I2CM_SCK = 1; 
   COMM_I2CM_SDA = 1;
   COMM_I2CM_SCK_TRIS = 1;
   COMM_I2CM_SDA_TRIS = 1;
   SSPCON1 = COMM_I2CM_SSPCON1_DFLT;

   //-----------------------------------
   //--- setup READ/MEASURE
   //-----------------------------------
   COMM_I2CM_ADC_SDA_TRIS = 1;                  // init ADC pins
   COMM_I2CM_ADC_SCL_TRIS = 1;                  //
   if((ADCON1 & 0x0F) > 0x0C)                   // config ADC pins analog
      ADCON1 = 0x0C;
   COMM_I2CM_ADC_TIMER = COMM_I2CM_ADC_INTV_TA; // init timer

   //--------------------------------
   //--- AUX PINS
   //--------------------------------

   //--- AUX1 TRIS/DIR
   if(COMM_CB_I2CM_AUX1_TRIS_FLAG)
      I2CM_AUX1_TRIS = 1;
   else
      I2CM_AUX1_TRIS = 0;

   //--- AUX1 PORT/STATE
   if(COMM_CB_I2CM_AUX1_PORT_FLAG)
      I2CM_AUX1_PORT = 1;
   else
      I2CM_AUX1_PORT = 0;

   //--- AUX2 TRIS/DIR
   if(COMM_CB_I2CM_AUX2_TRIS_FLAG)
      I2CM_AUX2_TRIS = 1;
   else
      I2CM_AUX2_TRIS = 0;

   //--- AUX2 PORT/STATE
   if(COMM_CB_I2CM_AUX2_PORT_FLAG)
      I2CM_AUX2_PORT = 1;
   else
      I2CM_AUX2_PORT = 0;


   return(0);
}

//==========================================================
//--- comm_i2cm_reset()
//==========================================================
void comm_i2cm_reset(void)
{
   COMM_SB_I2CM_REG12 = 0;
   COMM_SB_I2CM_REG13 = 0;
   COMM_SB_I2CM_REG14 = 0;
   COMM_SB_I2CM_REG15 = 0;
   COMM_SB_I2CM_REG16 = 0;
   COMM_SB_I2CM_REG17 = 0;
   COMM_SB_I2CM_REG18 = 0;
   COMM_SB_I2CM_REG19 = 0;
   SSPCON1 = COMM_I2CM_SSPCON1_IDLE;
   COMM_I2CM_SCK_TRIS = 1;
   COMM_I2CM_SDA_TRIS = 1;
}


//==========================================================
//--- comm_i2cm_postproc()
//==========================================================
void comm_i2cm_postproc(void)
{

   //-----------------------------------
   //--- transaction error(s) ?
   //-----------------------------------
   COMM_4_FLAG = 0;
   if(SSPCON1bits.WCOL)
   {
      COMM_4_FLAG = 1;              // set temp flag
      SSPCON1bits.WCOL = 0;         // reset error
      COMM_SB_I2CM_WCOL_FLAG = 1;   // set error flag in STATUS_BLOCK
   }
   if(SSPCON1bits.SSPOV)
   {
      COMM_4_FLAG = 1;              // set temp flag
      SSPCON1bits.SSPOV = 0;        // reset error
      COMM_SB_I2CM_SSPOV_FLAG = 1;  // set error flag in STATUS_BLOCK
   }
   if(COMM_4_FLAG)
   {
      if(COMM_CB_I2CM_EVENT_XACT_ERR_FLAG)
      {
         comm_post_event_1b(COMM_TAG_I2CM_XACT_ERR, COMM_SB_I2CM_ERR_REG);
      }
   }
   
   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(COMM_SB_I2CM_ERR_REG & COMM_SB_I2CM_ERR_REG_MASK)
   {
      COMM_SB_GEN_ERR_FLAG = 1;
      COMM_SB_I2CM_ERR_FLAG = 1;
   }

   //-----------------------------------
   //--- change in error status
   //-----------------------------------
   if(COMM_CB_I2CM_EVENT_STATUS_ERR_FLAG)
   {
      if(COMM_SB_I2CM_ERR_REG != exec_status_save_2)
      {
         exec_status_save_2 = COMM_SB_I2CM_ERR_REG;
         comm_post_event_1b(COMM_TAG_I2CM_STATUS_ERR, COMM_SB_I2CM_ERR_REG);
      }
   }

   //-----------------------------------
   //--- measure bus lines
   //-----------------------------------
   if(not COMM_I2CM_ADC_TIMER)
   {
      COMM_I2CM_ADC_TIMER = COMM_I2CM_ADC_INTV_TA;
      comm_i2cm_adc_bus();
   }

   //-----------------------------------
   //--- AUX PIN STATUS
   //-----------------------------------
   COMM_SB_I2CM_AUX1_TRIS_FLAG = 0;
   if(I2CM_AUX1_TRIS)
      COMM_SB_I2CM_AUX1_TRIS_FLAG = 1;

   COMM_SB_I2CM_AUX2_TRIS_FLAG = 0;
   if(I2CM_AUX2_TRIS)
      COMM_SB_I2CM_AUX2_TRIS_FLAG = 1;
   
   COMM_SB_I2CM_AUX1_PORT_FLAG = 0;
   if(I2CM_AUX1_PORT)
      COMM_SB_I2CM_AUX1_PORT_FLAG = 1;
   
   COMM_SB_I2CM_AUX2_PORT_FLAG = 0;
   if(I2CM_AUX2_PORT)
      COMM_SB_I2CM_AUX2_PORT_FLAG = 1;

}

//==========================================================
//--- comm_i2cm_adc_bus()
//    measure level of BUS pins (ADC channels 1 & 2)
//    save results in STATUS_BLOCK
//==========================================================
void comm_i2cm_adc_bus(void)
{
   //----------------------
   //--- RESET TIMER
   //----------------------
   COMM_I2CM_ADC_TIMER = COMM_I2CM_ADC_INTV_TA;
      
   //----------------------
   //--- MEAS SDA
   //----------------------
   ADCON2 = COMM_I2CM_ADCON2;          // configure ADC
   ADCON0 = COMM_I2CM_ADCON0_SDA;      // set channel: 1
   while(ADCON0bits.GO)                // wait for result
   {} 
   COMM_SB_I2CM_ADC_SDA_REG = ADRESH;  // save results

   //----------------------
   //--- MEAS SCL
   //----------------------
   ADCON0 = COMM_I2CM_ADCON0_SCL;      // set channel: 2
   while(ADCON0bits.GO)                // wait for result
   {} 
   COMM_SB_I2CM_ADC_SCL_REG = ADRESH;  // save results
}

//==========================================================
//--- comm_i2cm_svc()
//==========================================================
void comm_i2cm_svc(void)
{
   //---------------------------------------------
   //--- SWITCH CONTROLLER STATE
   //---------------------------------------------
   switch(comm_state)
   {

      //==========================================
      case COMM_TAG_I2CM_INIT:
         comm_i2cm_init();
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      /*
      //==========================================
      case COMM_TAG_I2CM_RESET:
         comm_i2cm_reset();
         comm_state = COMM_STATE_GEN_NEXT;
         break;
      */
               
      //==========================================
      case COMM_TAG_I2CM_START:
         if(not COMM_0_FLAG)
         {
            SSPCON2bits.SEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_START_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_START);
            comm_led_busy_data();
         }
         else
         {
            if(SSPCON2bits.SEN == 0)
            {
               comm_state = COMM_STATE_GEN_NEXT;
            }
         }
         break;

      //==========================================
      case COMM_TAG_I2CM_STOP:
         if(not COMM_0_FLAG)
         {
            SSPCON2bits.PEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_STOP_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_STOP);
            comm_led_busy_data();
         }
         else
         {
            if(SSPCON2bits.PEN == 0)
               comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //==========================================
      case COMM_TAG_I2CM_RESTART:
         if(not COMM_0_FLAG)
         {
            SSPCON2bits.RSEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_RESTART_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_RESTART);
            comm_led_busy_data();
         }
         else
         {
            if(SSPCON2bits.RSEN == 0)
               comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //==========================================
      case COMM_TAG_I2CM_WRITE_BYTES:

         //--- GET BYTE COUNT
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2 = comm_data;
               COMM_0_FLAG = 1;
            }
         }

         //--- CHECK BYTE COUNT
         else if(not COMM_1_FLAG)
         {
            if(not comm_cmnd_count_2)
            {
               comm_state = COMM_STATE_GEN_NEXT;
               break;
            }
            if(SSPSTATbits.BF == 0)
               COMM_1_FLAG = 1;
         }
         
         //--- OUTPUT BYTE (when/if available)
         else if(not COMM_2_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2--;
               COMM_2_FLAG = 1;
               PIR1bits.SSPIF = 0;
               SSPBUF = comm_data;
               if(COMM_CB_I2CM_EVENT_BYTE_TX_FLAG)
                  comm_post_event_1b(COMM_TAG_I2CM_EVENT_BYTE_WRITE, comm_data);
               comm_led_busy_data();
            }
         }

         //--- WAIT FOR ACK/NACK
         else
         {
            if(PIR1bits.SSPIF==1)
            {
               PIR1bits.SSPIF = 0;
               if(SSPCON2bits.ACKSTAT==1)
               {
                  if(COMM_CB_I2CM_EVENT_ACK_RX_FLAG)
                     comm_post_event(COMM_TAG_I2CM_EVENT_NACK_RX);
                  COMM_SB_I2CM_NACK_FLAG = 1;       
               }
               else
               {
                  if(COMM_CB_I2CM_EVENT_ACK_RX_FLAG)
                     comm_post_event(COMM_TAG_I2CM_EVENT_ACK_RX);
               }
               COMM_1_FLAG = 0;
               COMM_2_FLAG = 0;
               comm_led_busy_data();
            }
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_READ_BYTES:
      case COMM_TAG_I2CM_READ_BYTES_NLB:
         
         //--- GET BYTE COUNT
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2 = comm_data;
               COMM_0_FLAG = 1;
            }
         }

         //--- CHECK BYTE COUNT
         else if(not COMM_1_FLAG)
         {
            if(not comm_cmnd_count_2)
            {
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
               break;
            }
            COMM_1_FLAG = 1;
            SSPCON2bits.RCEN = 1;
         }

         //--- READ BYTE (when/if available)
         else if(not COMM_2_FLAG)
         {
            if(PIR1bits.SSPIF)
            {
               PIR1bits.SSPIF = 0;
               comm_data = SSPBUF;
               if(COMM_CB_I2CM_EVENT_BYTE_RX_FLAG)
                  comm_post_event_1b(COMM_TAG_I2CM_EVENT_BYTE_READ, comm_data);
               comm_post_data();
               comm_cmnd_count_2--;
               COMM_2_FLAG = 1;
               
               SSPCON2bits.ACKDT = 0;
               if((comm_state == COMM_TAG_I2CM_READ_BYTES_NLB) && (not comm_cmnd_count_2))
                  SSPCON2bits.ACKDT = 1;
               SSPCON2bits.ACKEN = 1;
               comm_led_busy_data();
            }
         }
         
         //--- WAIT FOR ACK/NACK EXECUTION
         else
         {
            if(PIR1bits.SSPIF)
            {
               PIR1bits.SSPIF = 0;
               if(COMM_CB_I2CM_EVENT_ACK_TX_FLAG)
               {
                  if(SSPCON2bits.ACKDT)
                     comm_post_event(COMM_TAG_I2CM_EVENT_NACK_TX);
                  else
                     comm_post_event(COMM_TAG_I2CM_EVENT_ACK_TX);
               }

               COMM_1_FLAG = 0;
               COMM_2_FLAG = 0;
               COMM_PROC_LOOP_FLAG = 1;
               comm_led_busy_data();
            }
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_READ_BLOCK:
      case COMM_TAG_I2CM_READ_BLOCK_NLB:

         //--- GET BYTE COUNT
         if(PIR1bits.SSPIF)
         {
            //--- process byte
            PIR1bits.SSPIF = 0;
            comm_data = SSPBUF;
            comm_post_data();
            comm_cmnd_count_2 = comm_data;

            //--- issue ACK
            SSPCON2bits.ACKDT = 0;
            SSPCON2bits.ACKEN = 1;

            //--- prep for "READ_BYTES" state
            COMM_0_FLAG = 1;
            COMM_1_FLAG = 1;
            COMM_2_FLAG = 1;
            comm_state = COMM_TAG_I2CM_READ_BYTES;
            if(comm_state == COMM_TAG_I2CM_READ_BLOCK_NLB)
               comm_state = COMM_TAG_I2CM_READ_BYTES_NLB;
            comm_led_busy_data();
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_BITRATE:
         if(comm_data_get())
         {
            SSPADD = comm_data;
            COMM_SB_I2CM_BITRATE_REG = SSPADD;
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;
         

      //====================================================
      case COMM_TAG_I2CM_AUX1_RST:
         I2CM_AUX1_PORT = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX1_SET:
         I2CM_AUX1_PORT = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX1_OUT:
         I2CM_AUX1_TRIS = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX1_IN:
         I2CM_AUX1_TRIS = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX1_WAIT_0:
         if(not I2CM_AUX1_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX1_WAIT_1:
         if(I2CM_AUX1_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         

      //====================================================
      case COMM_TAG_I2CM_AUX2_RST:
         I2CM_AUX2_PORT = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX2_SET:
         I2CM_AUX2_PORT = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX2_OUT:
         I2CM_AUX2_TRIS = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX2_IN:
         I2CM_AUX2_TRIS = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX2_WAIT_0:
         if(not I2CM_AUX2_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         
      //====================================================
      case COMM_TAG_I2CM_AUX2_WAIT_1:
         if(I2CM_AUX2_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      //====================================================
      case COMM_TAG_I2CM_SPECIAL_1:
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_SB_I2CM_NACK_FLAG = 1;                  // preset error
         timer3_set(100.0 / TIMER3_MSEC);
         comm_data_get();
         comm_data_2 = comm_data;
         comm_data_get();
         comm_data_3 = comm_data;
         
         //-------------------
         //--- START
         //-------------------
         SSPCON2bits.SEN = 1;
         while(SSPCON2bits.SEN == 1)
         {
            if(PIR2bits.TMR3IF)
               break;
         }
         if(PIR2bits.TMR3IF)
            break;

         //-------------------
         //--- WRITE ADDRESS
         //-------------------
         PIR1bits.SSPIF = 0;
         SSPBUF = comm_data_2;
         while(PIR1bits.SSPIF == 0)
         {
            if(PIR2bits.TMR3IF)
               break;
         }
         if(SSPCON2bits.ACKSTAT==1)
            break;

         if(PIR2bits.TMR3IF)
            break;
         PIR1bits.SSPIF = 0;

         //-------------------
         //--- READ DATA
         //-------------------
         comm_cmnd_count_2 = comm_data_3;

         for(; comm_cmnd_count_2; comm_cmnd_count_2--)
         {
            SSPCON2bits.RCEN = 1;
            while(not PIR1bits.SSPIF)
            {
               if(PIR2bits.TMR3IF)
                  break;
            }
            if(PIR2bits.TMR3IF)
               break;
            comm_data = SSPBUF;
            PIR1bits.SSPIF = 0;
            comm_post_data();
               
            //--- ACK/NACK
            SSPCON2bits.ACKDT = 0;
            if(comm_cmnd_count_2 == 1)
               SSPCON2bits.ACKDT = 1;
            SSPCON2bits.ACKEN = 1;

            while(not PIR1bits.SSPIF)
            {
               if(PIR2bits.TMR3IF)
                  break;
            }
            if(PIR2bits.TMR3IF)
               break;
            PIR1bits.SSPIF = 0;
         } //--- END: FOR:
         
         //-------------------
         //--- STOP
         //-------------------
         SSPCON2bits.PEN = 1;
         while(SSPCON2bits.PEN == 1)
         {
            if(PIR2bits.TMR3IF)
               break;
         }
         if(PIR2bits.TMR3IF)
            break;

         //-------------------
         //--- done
         //-------------------
         COMM_SB_I2CM_NACK_FLAG = 0;
         break;

      //==========================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;

   } // end SWITCH
   
}

