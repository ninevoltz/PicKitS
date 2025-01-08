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
//    Filename:            comm_usart.c
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
//--- COMM_USART
//
//    communications controller: USART
//
//--- comm_usart_init()
//    initialize usart per CONTROL_BLOCK
//
//--- comm_usart_reset()
//    reset COMM_USART controller to IDLE
//
//--- comm_usart_svc()
//    usart service
//
//--- comm_usart_postproc()
//    post processing
//
//--- comm_usart_rcv()
//    receive USART data
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
#include "comm_usart.h"
#include "cbuf.h"


//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
// none


//==========================================================
//--- comm_usart_init()
//    initialize usart per CONTROL_BLOCK
//==========================================================
TD_BYTE comm_usart_init(void)
{
   //comm_usart_reset();
   comm_usart_config();
   comm_usart_clear();
   return(0);
}

//==========================================================
//--- comm_usart_config()
//    initialize usart per CONTROL_BLOCK
//==========================================================
void comm_usart_config(void)
{

   //--- SET IDLE
   //comm_usart_reset();                 // set IDLE
   
   //--- INIT PER MODE
   switch(COMM_CB_MODE_REG)
   {
      case COMM_MODE_USART_A:
         if(not COMM_CB_USART_CONFIG_RCV_DIS_FLAG)
            RCSTAbits.CREN = 1;
         RCSTAbits.SPEN = 1;
         USART_TX_TRIS = 0;
         USART_RX_TRIS = 1;
         TXSTAbits.TXEN = 1;
         IPR1bits.RCIP = 0;
         PIE1bits.RCIE = 1;
         
         break;
      case COMM_MODE_USART_SM:
         if(COMM_CB_USART_CONFIG_CKP_FLAG)
            BAUDCONbits.SCKP = 1;
         TXSTAbits.CSRC = 1;
         TXSTAbits.SYNC = 1;
         RCSTAbits.SPEN = 1;
         USART_TX_TRIS = 0;
         USART_RX_TRIS = 1;
         break;
      case COMM_MODE_USART_SS:
         TXSTAbits.SYNC = 1;
         RCSTAbits.SPEN = 1;
         USART_TX_TRIS = 1;
         USART_RX_TRIS = 1;
         break;
      default:
         COMM_SB_USART_INIT_ERR_FLAG = 1;
         break;
   }

   //--- IF NO ERROR, FINISH INIT ...
   if(not COMM_SB_USART_INIT_ERR_FLAG)
   {
      SPBRG = COMM_CB_USART_BRG_LO_REG;
      COMM_SB_USART_BRG_LO_REG = SPBRG;
      SPBRGH = COMM_CB_USART_BRG_HI_REG;
      COMM_SB_USART_BRG_HI_REG = SPBRGH;
      BAUDCONbits.BRG16 = 1;
      TXSTAbits.BRGH = 1;
      TXSTAbits.TXEN = 1;

      //--- CONFIGURATION
      COMM_SB_USART_CONFIG_REG = COMM_CB_USART_CONFIG_REG;

      //--------------------------------
      //--- AUX PINS
      //--------------------------------

      //--- AUX1 TRIS/DIR
      if(COMM_CB_USART_AUX1_TRIS_FLAG)
         USART_AUX1_TRIS = 1;
      else
         USART_AUX1_TRIS = 0;

      //--- AUX1 PORT/STATE
      if(COMM_CB_USART_AUX1_PORT_FLAG)
         USART_AUX1_PORT = 1;
      else
         USART_AUX1_PORT = 0;

      //--- AUX2 TRIS/DIR
      if(COMM_CB_USART_AUX2_TRIS_FLAG)
         USART_AUX2_TRIS = 1;
      else
         USART_AUX2_TRIS = 0;

      //--- AUX2 PORT/STATE
      if(COMM_CB_USART_AUX2_PORT_FLAG)
         USART_AUX2_PORT = 1;
      else
         USART_AUX2_PORT = 0;
   }
   
}


//==========================================================
//--- comm_usart_reset()
//    reset COMM_USART controller to IDLE
//==========================================================
void comm_usart_reset(void)
{
   //--- clear regs
   TXSTA = 0;
   RCSTA = 0;
   BAUDCON = 0;
   USART_TX_TRIS = 1;
   USART_RX_TRIS = 1;
   USART_AUX1_TRIS = 1;
   USART_AUX2_TRIS = 1;
}


   
//==========================================================
//--- comm_usart_clear()
//    clear variables - non-destructive
//==========================================================
void comm_usart_clear(void)
{
   //-------------------------
   //--- CLEAR STATUS
   //-------------------------
   COMM_SB_USART_REG12 = 0;
   COMM_SB_USART_REG13 = 0;
   COMM_SB_USART_REG14 = 0;
   COMM_SB_USART_REG15 = 0;
   COMM_SB_USART_REG16 = 0;
   COMM_SB_USART_REG17 = 0;
   COMM_SB_USART_REG18 = 0;
   COMM_SB_USART_REG19 = 0;   
   COMM_USART_FLAGS = 0;
   
   //-------------------------
   //--- RESET H/W ERROR(S)
   //-------------------------
   RCSTAbits.FERR = 0;
   RCSTAbits.OERR = 0;

   //-------------------------
   //--- RE-LOAD STATUS (STATIC)
   //-------------------------
   COMM_SB_USART_CONFIG_REG = COMM_CB_USART_CONFIG_REG;

   //-------------------------
   //--- RE-LOAD STATUS (DYNAMIC)
   //-------------------------
   comm_usart_postproc();
   
}


//==========================================================
//--- comm_usart_svc()
//    usart service
//==========================================================
void comm_usart_svc(void)
{

   //---------------------------------------------
   //--- SWITCH CONTROLLER STATE
   //---------------------------------------------
   switch(comm_state)
   {

      //====================================================
      case COMM_TAG_USART_INIT:
            comm_usart_init();
            break;

      //====================================================
      case COMM_TAG_USART_DATA_XMT:
      
         //-----------------------------
         //--- GET BYTE COUNT
         //-----------------------------
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               COMM_0_FLAG = 1;
               comm_cmnd_count_2 = comm_data;   // load byte count
               COMM_PROC_LOOP_FLAG = 1;
            }
         }

         //-----------------------------
         //--- CHECK BYTE COUNT
         //-----------------------------
         else if(not COMM_1_FLAG)
         {
            if(comm_cmnd_count_2 == 0)
            {
               if(TXSTAbits.TRMT)
               {
                  comm_state = COMM_STATE_GEN_NEXT;
               }
               break;
            }
            else
            {
               if(PIR1bits.TXIF)
                  COMM_1_FLAG = 1;
            }
            COMM_PROC_LOOP_FLAG = 1;
         }
         
         //-----------------------------
         //--- DATA I/O
         //-----------------------------
         else
         {
            if(not comm_data_get())
              break;

            TXREG = comm_data;
            if(COMM_CB_USART_EVENT_BYTE_TX_FLAG)
               comm_post_event_1b(COMM_TAG_USART_EVENT_BYTE_TX, comm_data);
            COMM_1_FLAG = 0;
            comm_cmnd_count_2--;
            comm_led_busy_data();            
         }
         break;
         
      //====================================================
      case COMM_TAG_USART_DATA_SRCV:
      
         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_SM)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         
         //-----------------------------
         //--- GET BYTE COUNT
         //-----------------------------
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               COMM_0_FLAG = 1;
               comm_cmnd_count_2 = comm_data;
               RCSTAbits.CREN = 0;
               RCSTAbits.SREN = 0;
               //USART_RX_TRIS = 1;
            }
         }

         //-----------------------------
         //--- CHECK BYTE COUNT
         //-----------------------------
         else if(not COMM_1_FLAG)
         {
            if(comm_cmnd_count_2 == 0)
            {
               RCSTAbits.SREN = 0;
               //USART_RX_TRIS = 0;
               comm_state = COMM_STATE_GEN_NEXT;
               break;
            }
            else
            {
               COMM_1_FLAG = 1;
               RCSTAbits.SREN = 1;
            }
         }
         
         //-----------------------------
         //--- DATA I/O
         //-----------------------------
         else
         {
            if(RCSTAbits.SREN == 0)
            {
               comm_data = RCREG;
               if(COMM_CB_USART_EVENT_BYTE_RX_FLAG)
                  comm_post_event_1b(COMM_TAG_USART_EVENT_BYTE_RX, comm_data);
               comm_post_data();
               COMM_1_FLAG = 0;
               comm_cmnd_count_2--;
               comm_led_busy_data();
            }
         }
         break;
         

      //====================================================
      case COMM_TAG_USART_DATA_ARCV_ENABLE:
      
         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_A)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         
         COMM_SB_USART_CONFIG_RCV_DIS_FLAG = 0;
         RCSTAbits.CREN = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_USART_DATA_ARCV_DISABLE:

         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_A)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         
         COMM_SB_USART_CONFIG_RCV_DIS_FLAG = 1;
         RCSTAbits.CREN = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_USART_BREAK_XMT:
      case COMM_TAG_USART_BREAK_DATA_XMT:

         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_SM)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         

         //--- SETUP XMIT
         if(not COMM_0_FLAG)
         {
            TXSTAbits.TXEN = 1;
            TXSTAbits.SENDB = 1;
            TXREG = 0;
            COMM_0_FLAG = 1;
            COMM_PROC_LOOP_FLAG = 1;
         }

         //--- WAIT for TXREG MT, LOAD W/ DATA IF APPLICABLE
         else if(not COMM_1_FLAG)
         {
            if(PIR1bits.TXIF)
            {
               if(comm_state == COMM_TAG_USART_BREAK_DATA_XMT)
               {
                  if(comm_data_get())
                  {
                     TXREG = comm_data;
                     COMM_1_FLAG = 1;
                  }
               }
               else
               {
                  COMM_1_FLAG = 1;
               }
               comm_led_busy_data();
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         else
         {
            if(PIR1bits.TXIF)
            {
               if(not TXSTAbits.SENDB)
               {
                  if(TXSTAbits.TRMT)
                  {
                     comm_state = COMM_STATE_GEN_NEXT;
                     COMM_PROC_LOOP_FLAG = 1;
                  }
                  comm_led_busy_data();
               }
            }
         }
         break;

      //====================================================
      case COMM_TAG_USART_BAUD:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               SPBRG = comm_data;
               COMM_SB_USART_BRG_LO_REG = SPBRG;
               COMM_0_FLAG = 1;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         else
         {
            if(comm_data_get())
            {
               SPBRGH = comm_data;
               COMM_SB_USART_BRG_HI_REG = SPBRGH;
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         break;

      //====================================================
      case COMM_TAG_USART_CKP_SET:

         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_SM)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         
         BAUDCONbits.SCKP = 1;
         COMM_SB_USART_CONFIG_CKP_FLAG = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_USART_CKP_RST:

         //-----------------------------
         //--- CHECK MODE
         //-----------------------------
         if(not COMM_MODE_USART_SM)
         {
            COMM_SB_BAD_CMND_FLAG = 1;
            break;
         }
         
         BAUDCONbits.SCKP = 0;
         COMM_SB_USART_CONFIG_CKP_FLAG = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         break;


      //====================================================
      case COMM_TAG_USART_AUX1_RST:
         USART_AUX1_PORT = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX1_SET:
         USART_AUX1_PORT = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX1_OUT:
         USART_AUX1_TRIS = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX1_IN:
         USART_AUX1_TRIS = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX1_WAIT_0:
         if(not USART_AUX1_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX1_WAIT_1:
         if(USART_AUX1_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         

      //====================================================
      case COMM_TAG_USART_AUX2_RST:
         USART_AUX2_PORT = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX2_SET:
         USART_AUX2_PORT = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX2_OUT:
         USART_AUX2_TRIS = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX2_IN:
         USART_AUX2_TRIS = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_PROC_LOOP_FLAG = 1;
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX2_WAIT_0:
         if(not USART_AUX2_PORT)
         {
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;
         
      //====================================================
      case COMM_TAG_USART_AUX2_WAIT_1:
         if(USART_AUX2_PORT)
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
   
   //-----------------------------------
   //--- CHECK RCV BUFFER
   //-----------------------------------
//   comm_usart_rcv();
}


//==========================================================
//--- comm_usart_rcv()
//    receive USART data (ASYNC mode)
//==========================================================
void comm_usart_rcv_isr(void)
{
   //-------------------------------------------------------
   //--- RECEIVE ENABLE ?
   //-------------------------------------------------------
   if(COMM_MODE_USART_A)
   {
      if(PIR1bits.RCIF)
      {
         comm_data = RCREG;
         comm_post_data();
         if(COMM_CB_USART_EVENT_BYTE_RX_FLAG)
            comm_post_event_1b(COMM_TAG_USART_EVENT_BYTE_RX, comm_data);
         COMM_USART_RCV_FLAG = 1;
      }
   }
}
   
//==========================================================
//--- comm_usart_postproc()
//    post processing
//
//    look for status error(s)
//    set appropriate STATUS_BLOCK flags
//    set composite status bit
//==========================================================
void comm_usart_postproc(void)
{
   
   //-----------------------------------
   //--- CHECK RCV
   //-----------------------------------
   if(COMM_USART_RCV_FLAG)
   {
      COMM_USART_RCV_FLAG = 0;
      comm_led_busy_data();
   }
   
//   comm_usart_rcv();

   //-----------------------------------
   //--- AUX PIN STATUS
   //-----------------------------------
   COMM_SB_USART_AUX1_TRIS_FLAG = 0;
   if(USART_AUX1_TRIS)
      COMM_SB_USART_AUX1_TRIS_FLAG = 1;

   COMM_SB_USART_AUX2_TRIS_FLAG = 0;
   if(USART_AUX2_TRIS)
      COMM_SB_USART_AUX2_TRIS_FLAG = 1;
   
   COMM_SB_USART_AUX1_PORT_FLAG = 0;
   if(USART_AUX1_PORT)
      COMM_SB_USART_AUX1_PORT_FLAG = 1;
   
   COMM_SB_USART_AUX2_PORT_FLAG = 0;
   if(USART_AUX2_PORT)
      COMM_SB_USART_AUX2_PORT_FLAG = 1;

   
   //-----------------------------------
   //--- transaction error
   //-----------------------------------
   COMM_4_FLAG = 0;
   if(RCSTAbits.FERR)
   {
      COMM_4_FLAG = 1;
      RCSTAbits.FERR = 0;
      COMM_SB_USART_STATUS_FERR_FLAG = 1;
   }
   if(RCSTAbits.OERR)
   {
      COMM_4_FLAG = 1;
      RCSTAbits.OERR = 0;
      COMM_SB_USART_STATUS_OERR_FLAG = 1;
   }
   
   if(COMM_4_FLAG)
   {
      if(COMM_CB_USART_EVENT_STATUS_ERR_FLAG)
      {
         comm_post_event_1b(COMM_TAG_USART_EVENT_STATUS_ERR, COMM_SB_USART_ERR_REG);
      }
   }
   
   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(COMM_SB_USART_ERR_REG & COMM_SB_USART_ERR_REG_MASK)
   {
      COMM_SB_GEN_ERR_FLAG = 1;
      COMM_SB_USART_ERR_FLAG = 1;
   }

   //-----------------------------------
   //--- REFRESH BAUD RATE STATUS
   //-----------------------------------
   COMM_SB_USART_BRG_LO_REG = SPBRG;
   COMM_SB_USART_BRG_HI_REG = SPBRGH;
}

