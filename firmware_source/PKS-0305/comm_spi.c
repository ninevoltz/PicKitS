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
//    Filename:            comm_spi.c
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
//--- COMM_SPI
//
//    communications controller: SPI
//    uses MSSP hardware assist
//
//--- comm_spi_init()
//    initialize module per CONTROL_BLOCK data
//
//--- comm_spi_svc()
//    SPI script service
//
//--- comm_spi_reset()
//    "reset" comm control (f/w, h/w) to benign state
//
//--- comm_spi_postproc()
//    spi script service post processing
//
//--- comm_spi_svc()
//    SPI script service
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
//--- INCLUDES
//==========================================================
#include "project.h"
#include "comm_gen.h"
#include "comm_spi.h"
#include "cbuf.h"

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
// none

#ifdef __DEBUG
//#define __DEBUG_SPI_AUX1_ENABLE
#endif

//==========================================================
//--- comm_spi_init()
//    initialize module per CONTROL_BLOCK data
//==========================================================
TD_BYTE comm_spi_init(void)
{
   comm_spi_reset();
   comm_spi_config();
   comm_spi_clear();

   return(0);
}


//---
void comm_spi_config(void)
{
   
#ifdef __DEBUG_SPI_AUX1_ENABLE
   COMM_CB_SPI_AUX1_ENABLE_FLAG = 1;
   COMM_CB_SPI_AUX1_DIR_FLAG = 0;
   COMM_CB_SPI_AUX1_STATE_FLAG = 1;
#endif

   //---------------------------------------------
   //--- if MASTER or SLAVE
   //---------------------------------------------
   if((COMM_CB_MODE_REG == COMM_MODE_SPI_M) || 
      (COMM_CB_MODE_REG == COMM_MODE_SPI_S) ||
      (COMM_CB_MODE_REG == COMM_MODE_MWIRE_M))
   {

      //--- INPUT DATA SAMPLE SETTING
      if(COMM_CB_SPI_CONFIG_SMP_FLAG)
         SSPSTATbits.SMP = 1;
      else
         SSPSTATbits.SMP = 0;

      //--- TX CLOCK EDGE SETTING
      if(COMM_CB_SPI_CONFIG_CKE_FLAG)
         SSPSTATbits.CKE = 1;
      else
         SSPSTATbits.CKE = 0;

      //--- MASTER-specific
      if((COMM_CB_MODE_REG == COMM_MODE_SPI_M) ||
         (COMM_CB_MODE_REG == COMM_MODE_MWIRE_M))
      {
         SSPCON1 = SSPCON1_SPI_MODE_MASTER;
         COMM_SPI_SCK_TRIS = 0;

         //--- CS
         if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            COMM_SB_SPI_AUX1_ENABLE_FLAG = 1;
            
            COMM_SPI_AUX1_PIN = COMM_CB_SPI_AUX1_STATE_FLAG;
            COMM_SB_SPI_AUX1_STATE_FLAG = COMM_CB_SPI_AUX1_STATE_FLAG;

            COMM_SPI_AUX1_TRIS = COMM_CB_SPI_AUX1_DIR_FLAG;
            COMM_SB_SPI_AUX1_DIR_FLAG = COMM_CB_SPI_AUX1_DIR_FLAG;
         }
         else
         {
            if(COMM_CB_SPI_CONFIG_CSPOL_FLAG)
               COMM_SPI_CS_PIN = 1;
            else
               COMM_SPI_CS_PIN = 0;
            COMM_SPI_CS_TRIS = 0;
            COMM_SB_SPI_AUX1_ENABLE_FLAG = 0;
            COMM_SB_SPI_AUX1_STATE_FLAG = 0;
            COMM_SB_SPI_AUX1_DIR_FLAG = 0;
         }
      }

      //--- SLAVE-specific
      else
      {
         SSPCON1 = SSPCON1_SPI_MODE_SLAVE_NOSS;
         COMM_SPI_SCK_TRIS = 1;
      }
   
      //--- CLOCK POLARITY SETTING
      if(COMM_CB_SPI_CONFIG_CKP_FLAG)
         SSPCON1bits.CKP = 1;
      else
         SSPCON1bits.CKP = 0;

      COMM_SB_SPI_CONFIG_REG = COMM_CB_SPI_CONFIG_REG;

      //---
      COMM_SPI_SDO_TRIS = 0;            
      COMM_SPI_SDI_TRIS = 1;        
   
      //--- setup TMR2 for bit rate
      COMM_SB_SPI_BITRATE_PRESCALE_REG = COMM_CB_SPI_BITRATE_PRESCALE_REG & 0x03;
      COMM_SB_SPI_BITRATE_SCALE_REG = COMM_CB_SPI_BITRATE_SCALE_REG;
      T2CON = COMM_SB_SPI_BITRATE_PRESCALE_REG;
      PR2 = COMM_SB_SPI_BITRATE_SCALE_REG;
      T2CONbits.TMR2ON = 1;

      //---
      SSPCON1bits.SSPEN = 1;
   }
}

//====================================================================
//--- comm_spi_clear() - 'clear' errors and re-init clean status
//
//    (illadvised duplication of init - should re-org when time permits)
//====================================================================
void comm_spi_clear(void)
{
   
   //-------------------------
   //--- CLEAR STATUS BLOCK
   //-------------------------
   COMM_SB_SPI_REG12 = 0;
   COMM_SB_SPI_REG13 = 0;
   COMM_SB_SPI_REG14 = 0;
   COMM_SB_SPI_REG15 = 0;
   COMM_SB_SPI_REG16 = 0;
   COMM_SB_SPI_REG17 = 0;
   COMM_SB_SPI_REG18 = 0;
   COMM_SB_SPI_REG19 = 0;

   //-------------------------
   //--- RE-INIT PIN(S)
   //-------------------------
   //COMM_SPI_SDO_TRIS = 0;            
   //COMM_SPI_SDI_TRIS = 1;        

   //-------------------------
   //--- RESET H/W ERROR(S)
   //-------------------------
   SSPCON1bits.WCOL = 0;
   SSPCON1bits.SSPOV = 0;

   //-------------------------
   //--- RELOAD STATUS (STATIC)
   //-------------------------
   COMM_SB_SPI_BITRATE_PRESCALE_REG = COMM_CB_SPI_BITRATE_PRESCALE_REG & 0x03;
   COMM_SB_SPI_BITRATE_SCALE_REG = COMM_CB_SPI_BITRATE_SCALE_REG;
   COMM_SB_SPI_CONFIG_REG = COMM_CB_SPI_CONFIG_REG;
   if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
   {
      COMM_SB_SPI_AUX1_ENABLE_FLAG = COMM_CB_SPI_AUX1_ENABLE_FLAG;
      COMM_SB_SPI_AUX1_DIR_FLAG = COMM_CB_SPI_AUX1_DIR_FLAG;
   }

   //-------------------------
   //--- RELOAD STATUS (DYNAMIC)
   //-------------------------
   comm_spi_postproc();
}



//==========================================================
//--- comm_spi_reset()
//    "reset" comm control (f/w, h/w) to benign state
//==========================================================
void comm_spi_reset(void)
{
   
   COMM_SPI_SDO_TRIS = 1;            
   COMM_SPI_SDI_TRIS = 1;        
   COMM_SPI_SCK_TRIS = 1;
   COMM_SPI_CS_TRIS = 1;
   
   COMM_SPI_SDI_2_TRIS = 1;
   COMM_SPI_SDO_2_TRIS = 1;
   COMM_SPI_SCK_2_TRIS = 1;
   COMM_SPI_CS_2_TRIS = 1;

   SSPCON1 = SSPCON1_SPI_MODE_IDLE;
   SSPCON2 = 0;
   SSPSTAT = 0;
   //SSPADD = 0;
   T2CONbits.TMR2ON = 0;

}   


//==========================================================
//--- comm_spi_postproc()
//    spi script service post processing
//
//    called by comm_gen after script service is "released"
//    i.e. forced looping off
//    "forced looping" keeps execution in the service loop
//    to reduce delay/overhead between critical script entities
//==========================================================
void comm_spi_postproc(void)
{
   
   //-----------------------------------
   //--- transaction error
   //-----------------------------------
   COMM_7_FLAG = 0;
   if(SSPCON1bits.WCOL)
   {
      COMM_7_FLAG = 1;
      SSPCON1bits.WCOL = 0;
      COMM_SB_SPI_WCOL_FLAG = 1;
      
   }
   if(SSPCON1bits.SSPOV)
   {
      COMM_7_FLAG = 1;
      SSPCON1bits.SSPOV = 0;
      COMM_SB_SPI_SSPOV_FLAG = 1;
   }
   
   if(COMM_7_FLAG)
   {
      if(COMM_CB_SPI_EVENT_STATUS_ERR_FLAG)
      {
         comm_post_event_1b(COMM_TAG_SPI_EVENT_STATUS_ERR, COMM_SB_SPI_ERR_REG);
      }
   }
   
   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(COMM_SB_SPI_ERR_REG & COMM_SB_SPI_ERR_REG_MASK)
   {
      COMM_SB_GEN_ERR_FLAG = 1;
      COMM_SB_SPI_ERR_FLAG = 1;
   }

   //-----------------------------------
   //--- save pin states to status block
   //-----------------------------------
   COMM_SB_SPI_SDI_STATE_FLAG = 0;
   if(COMM_SPI_SDI_2_PIN)
      COMM_SB_SPI_SDI_STATE_FLAG = 1;

   COMM_SB_SPI_SDO_STATE_FLAG = 0;
   if(COMM_SPI_SDI_2_PIN)
      COMM_SB_SPI_SDO_STATE_FLAG = 1;

   COMM_SB_SPI_SCK_STATE_FLAG = 0;
   if(COMM_SPI_SCK_2_PIN)
      COMM_SB_SPI_SCK_STATE_FLAG = 1;

   COMM_SB_SPI_CS_STATE_FLAG = 0;
   if(COMM_SPI_CS_2_PIN)
      COMM_SB_SPI_CS_STATE_FLAG = 1;

   COMM_SB_SPI_AUX1_STATE_FLAG = COMM_SPI_AUX1_PIN;
}



//==========================================================
//--- comm_spi_svc()
//    SPI script service
//
//    script commands/data are taken from the buffer queue
//    flag "COMM_PROC_LOOP_FLAG" is used to keep program
//    execution in script processing between critical
//    entries in the script
//==========================================================
void comm_spi_svc(void)
{

   //---------------------------------------------
   //--- SWITCH CONTROLLER STATE
   //---------------------------------------------
   switch(comm_state)
   {

      //====================================================
      /*
      case COMM_TAG_SPI_RESET:
         comm_spi_reset();
         comm_state = COMM_STATE_GEN_NEXT;
         break;
      */
      
      //====================================================
      case COMM_TAG_SPI_INIT:
         comm_spi_init();
         comm_state = COMM_STATE_GEN_NEXT;
         break;
         
      //====================================================
      case COMM_TAG_SPI_DATAIO_OUT:
         COMM_4_FLAG = 0;
         COMM_5_FLAG = 1;
         comm_state = COMM_STATE_SPI_DATAIO;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_DATAIO_IN:
         COMM_4_FLAG = 1;
         COMM_5_FLAG = 0;
         comm_state = COMM_STATE_SPI_DATAIO;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_DATAIO_INOUT:
         COMM_4_FLAG = 1;
         COMM_5_FLAG = 1;
         comm_state = COMM_STATE_SPI_DATAIO;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_STATE_SPI_DATAIO:
      //====================================================
      // state used by DATAIO_IN, DATAIO_OUT & DATAIO_INOUT
      // COMM_4_FLAG: rd enabled
      // COMM_5_FLAG: wr enabled
      //====================================================
      //debug
      #ifdef __DEBUG
      if(not SW_PIN)
      {
         Nop();
         Nop();
      }
      #endif
         //-----------------------------
         //--- GET BYTE COUNT (0=>256)
         //-----------------------------
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2 = comm_data;   // load byte count
               COMM_0_FLAG = 1;
               PIR1bits.SSPIF = 0;              // pre-clear interrupt
               
               if(not COMM_CB_SPI_CONFIG_DAOD_FLAG)
               {
                  if(COMM_5_FLAG)
                     COMM_SPI_SDO_TRIS = 0;
                  else
                     COMM_SPI_SDO_TRIS = 1;
               }
            }
         }

         //-------------------------------
         //--- WRITE
         //-------------------------------
         else if(not COMM_1_FLAG)
         {
            PIR1bits.SSPIF = 0;
            if(COMM_5_FLAG)                  // WRITE enabled ?
            {
               if(not comm_data_get())       // get HOST data byte
                  break;
               T2CONbits.TMR2ON = 0;
               TMR2 = 0;
               SSPBUF = comm_data;
               T2CONbits.TMR2ON = 1;   
               if(COMM_CB_SPI_EVENT_BYTE_TX_FLAG)
                  comm_post_event_1b(COMM_TAG_SPI_EVENT_BYTE_TX, comm_data);
            }
            else
            {
               T2CONbits.TMR2ON = 0;
               TMR2 = 0;
               SSPBUF = 0;                   // DUMMY data
               T2CONbits.TMR2ON = 1;   

            }
            COMM_1_FLAG = 1;
            comm_led_busy_data();
         }

         //-------------------------------
         //--- WAIT BYTE COMPLETE
         //-------------------------------
         else
         {
         
            //-------------------------------
            //--- WAIT FOR INT
            //-------------------------------
            if(not PIR1bits.SSPIF)
               break;
                  
            //-------------------------------
            //--- READ BYTE
            //-------------------------------
            Nop();
            comm_data = SSPBUF;              // read/save serial data
            PIR1bits.SSPIF = 0;
            if(COMM_4_FLAG)                  // if READ enabled, post data
            { 
               if(COMM_CB_SPI_EVENT_BYTE_RX_FLAG)
                  comm_post_event_1b(COMM_TAG_SPI_EVENT_BYTE_RX, comm_data);
               comm_post_data();
               comm_led_busy_data();
            }

            //-----------------------------
            //--- CHECK BYTE COUNT
            //-----------------------------
            comm_cmnd_count_2--;
            if(comm_cmnd_count_2)
            {
               COMM_1_FLAG = 0;
               COMM_PROC_LOOP_FLAG = 1;
            }
            else
            {
               comm_state = COMM_STATE_GEN_NEXT;
               break;
            }
         }
         break;
         
      //====================================================
      case COMM_TAG_SPI_BITRATE:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               COMM_SB_SPI_BITRATE_SCALE_REG = comm_data;
               COMM_0_FLAG = 1;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         else
         {
            if(comm_data_get())
            {
               COMM_SB_SPI_BITRATE_PRESCALE_REG = comm_data & 0x03;
               T2CON = COMM_SB_SPI_BITRATE_PRESCALE_REG;
               PR2 = COMM_SB_SPI_BITRATE_SCALE_REG;
               T2CONbits.TMR2ON = 1;
               comm_state = COMM_STATE_GEN_NEXT;
            }
         }
         break;

      //====================================================
      case COMM_TAG_SPI_SDO_IN:
         COMM_SPI_SDO_TRIS = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_SPI_SDO_OUT:
         COMM_SPI_SDO_TRIS = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_CS_ON:
         if(not COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            if(COMM_CB_SPI_CONFIG_CSPOL_FLAG)
               COMM_SPI_CS_PIN = 1;
            else
               COMM_SPI_CS_PIN = 0;
            COMM_SPI_CS_TRIS = 0;
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         else
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      case COMM_TAG_SPI_CS_OFF:
         if(not COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            if(COMM_CB_SPI_CONFIG_CSPOL_FLAG)
               COMM_SPI_CS_PIN = 0;
            else
               COMM_SPI_CS_PIN = 1;
            COMM_SPI_CS_TRIS = 0;
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         else
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      case COMM_TAG_SPI_AUX1_RST:
         if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            COMM_SPI_AUX1_PIN = 0;
         }
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_SPI_AUX1_SET:
         if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            COMM_SPI_AUX1_PIN = 1;
         }
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_AUX1_OUT:
         if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            COMM_SPI_AUX1_TRIS = 0;
         }
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_AUX1_IN:
         if(COMM_CB_SPI_AUX1_ENABLE_FLAG)
         {
            COMM_SPI_AUX1_TRIS = 1;
         }
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;

      //====================================================
      case COMM_TAG_SPI_AUX1_WAIT_0:
         if((not COMM_SPI_AUX1_PIN)||(not COMM_CB_SPI_AUX1_ENABLE_FLAG))
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      case COMM_TAG_SPI_AUX1_WAIT_1:
         if((COMM_SPI_AUX1_PIN)||(not COMM_CB_SPI_AUX1_ENABLE_FLAG))
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;

   } // end SWITCH
   
}

