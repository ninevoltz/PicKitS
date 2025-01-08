//==================================================================== 
//--- COPYRIGHT
//==================================================================== 
// Copyright 2007 Microchip Technology Inc.
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
//    Filename:            comm_lin.c
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
//    0001 - 03-01-07 - ME
//    -- initial release
//
//    0002 - 03-01-08 - ME
//    -- overhauled module
//    -- changed to interrupt-driven TX
//    -- modified rx_isr
//    -- autobaud detect causes interrupt
//       (previously stayed in ISR, can now with simultaneous
//       MASTER and SLAVE)
//    -- created/defined variable using shared RAM
//    -- eliminated byte of bit flags ... moved to shared RAM
//    -- added SLAVE capability
//    -- set TXEN in INIT function, logic toggles TXIE as necessary
//
//
//    0003 - 04-28-08 - ME
//    -- added definition of bit flag in slave profile:
//       COMM_LIN_SP_IDDEF_ENHANCED_CS_FLAG to indicate a selection
//       of checksum 'type' for applicable node register/identifier
//    -- modified checksum calculation to use 
//       COMM_LIN_SP_IDDEF_ENHANCED_CS_FLAG in the slave profile 
//       to select checksum 'type' - was selected by control block
//
//    0004 - 05-12-08 - ME
//    -- modified rx_isr to avoid posting a SYNC byte event (only data)
//       this allows the DLL/GUI to run more efficiently/quickly
//
//==================================================================== 

//==========================================================
//--- INCLUDES
//==========================================================
#include "project.h"
#include "comm_gen.h"
#include "cbuf.h"
#include "timer0.h"
#include "timer3.h"
#include "comm_lin.h"

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
// (none)

#ifdef   __DEBUG
//#define  __DEBUG_LIN
#endif

#ifdef __DEBUG_LIN
//#define  __DEBUG_LIN_SLAVE_ENABLE
//#define  __DEBUG_LIN_AUX1
//#define  __DEBUG_LIN_AUX2
#endif

#define  COMM_LIN_SP_CHECKSUM_BP ((unsigned char *)(&(COMM_LIN_SP_CHECKSUM)))
#define  COMM_LIN_SP_CHECKSUM_WP ((unsigned int *)(&(COMM_LIN_SP_CHECKSUM)))

//==========================================================
//==========================================================
#ifdef __DEBUG_LIN
//==========================================================
//--- comm_lin_test()
//==========================================================
void comm_lin_test(void)
{
   TD_BYTE i, j, PARITY;
   
   Nop();
   
   cbuf3_init();
   cbuf3_put(0x92);
   cbuf3_put(0x10);
   cbuf3_put(0x11);
   cbuf3_put(0x22);
//   cbuf3_put(0xA9);
   cbuf3_put(0x93);
   cbuf3_put(0x24);
   cbuf3_put(0x12);
   cbuf3_put(0x34);
   cbuf3_put(0x56);
   cbuf3_put(0x81);
   cbuf3_put(0x38);
   cbuf3_put(0x12);
   cbuf3_put(0x00);
   
   COMM_LIN_1_FLAGS = 0;
   comm_flags1_B._b = 0;
   
   comm_state = 0x86;
   cbuf1_put(0x19);
   cbuf1_put(0x02);
   cbuf1_put(0x17);
   cbuf1_put(0x18);

   while(1)
   {
      Nop();
      comm_lin_svc();
   }
      
   return;
   
}
#endif
//==========================================================
//==========================================================


//==========================================================
//--- comm_lin_init()
//==========================================================
//    initialize per CONTROL_BLOCK
//==========================================================
TD_BYTE comm_lin_init(void)
{
   comm_lin_reset();          // idles/disables all hardware used by this module
   comm_lin_config();         // configures hardware
   comm_lin_clear();          // clears dynamic variables/hardware

   cbuf3_init();
   cbuf3_data[1] = 0;         // set slave profile to NULL
   
#ifdef __DEBUG_LIN_AUX1
   LIN_AUX1_PORT = 0;
   LIN_AUX1_TRIS = 0;
#endif

#ifdef __DEBUG_LIN_AUX2
   LIN_AUX2_PORT = 0;
   LIN_AUX2_TRIS = 0;
#endif
   
   return(0);
}

//==========================================================
//--- comm_lin_reset()
//==========================================================
//    RETURN ALL HARDWARE TO RESET/BENIGN STATE
//==========================================================
void comm_lin_reset(void)
{
   //--- DISABLE INTERRUPTS
   PIE1bits.RCIE = 0;
   PIE1bits.TXIE = 0;

   //--- CLEAR H/W REGISTERS
   TXSTA = 0;
   RCSTA = 0;
   BAUDCON = 0;

   //--- SET I/O DIRECTION
   LIN_TX_TRIS = 1;
   LIN_RX_TRIS = 1;
   LIN_CS_TRIS = 1;
   LIN_AFAULT_TRIS = 1;
}

//==========================================================
//--- comm_lin_config()
//==========================================================
//    one-time configuration
//==========================================================
void comm_lin_config(void)
{
   //--- I/O
   LIN_TX_TRIS = 0;
   LIN_RX_TRIS = 1;
   LIN_CS_TRIS = 0;
   LIN_AFAULT_TRIS = 1;

   //--- USART
   BAUDCONbits.BRG16 = 1;
   TXSTAbits.BRGH = 1;
   RCSTAbits.SPEN = 1;

   //--- INTERRUPTS
   IPR1bits.RCIP = 0;
   IPR1bits.TXIP = 0;
   TXSTAbits.TXEN = 1;
}

//==========================================================
//--- comm_lin_clear()
//==========================================================
//    assumes mode/hardware has been properly initialized
//    clears any status/errors without a cold re-init
//
//    anything that could have been changed during operation
//       is returned to default
//    anything that is initialized that never changed 
//       is left alone/unaltered
//    call post processor to set status block
//==========================================================
void comm_lin_clear(void)
{
   TD_BYTE i;
   
   //-----------------------------------
   //--- DISABLE INTERRUPT(S)
   //-----------------------------------
   PIE1bits.RCIE = 0;
   PIE1bits.TXIE = 0;

   //-----------------------------------
   //--- BAUD RATE
   //-----------------------------------
   SPBRG = COMM_CB_LIN_BRG_LO_REG;
   SPBRGH = COMM_CB_LIN_BRG_HI_REG;

   //-----------------------------------
   //--- AUTOBAUD
   //-----------------------------------
   if(BAUDCONbits.ABDEN)
      BAUDCONbits.ABDEN = 0;
   BAUDCONbits.ABDOVF = 0;
   
   //-----------------------------------
   //--- INTERRUPT ENABLE(S)
   //-----------------------------------
   if(PIR1bits.RCIF)
      i = RCREG;
   PIE1bits.RCIE = 1;

   //-----------------------------------
   //--- RX ENABLE
   //-----------------------------------
   if(RCSTAbits.OERR)
      RCSTAbits.CREN = 0;
      
   if(COMM_CB_LIN_RCV_ENABLE_FLAG)
      RCSTAbits.CREN = 1;
   else
      RCSTAbits.CREN = 0;

   //-----------------------------------
   //--- LIN CHIP SELECT
   //-----------------------------------
#ifndef __DEBUG_LIN_AUX1
   if(COMM_CB_LIN_CS_FLAG)
      LIN_CS_PORT = 1;
   else
      LIN_CS_PORT = 0;
#endif

   //-----------------------------------
   //--- OPERATIONAL FLAGS
   //-----------------------------------
   COMM_LIN_0_FLAGS = 0;
   COMM_LIN_1_FLAGS = 0;
      
   //-----------------------------------
   //--- STATUS BLOCK
   //-----------------------------------
   COMM_SB_LIN_REG12 = 0;
   COMM_SB_LIN_REG13 = 0;
   COMM_SB_LIN_REG14 = 0;
   COMM_SB_LIN_REG15 = 0;
   COMM_SB_LIN_REG16 = 0;
   COMM_SB_LIN_REG17 = 0;
   COMM_SB_LIN_REG18 = 0;
   COMM_SB_LIN_REG19 = 0;
   //--- EXECUTE POST PROCESSOR (SETS STATUS)
   comm_lin_postproc();


#ifdef   __DEBUG_LIN_SLAVE_ENABLE
//      COMM_CB_LIN_SLAVE_FLAG = 0;
      COMM_CB_LIN_SLAVE_FLAG = 1;
#endif

}


//==========================================================
//==========================================================
//--- comm_lin_svc()
//==========================================================
//==========================================================
void comm_lin_svc(void)
{

   //---------------------------------------------
   //--- DISABLE INTERRUPTS
   //--- FLAG SET TO REPRESENT EXISTING STATE
   //---------------------------------------------
   COMM_LIN_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_LIN_GIEL1_FLAG = 1;
   }

   //---------------------------------------------


   //---
#ifndef __DEBUG_LIN_AUX1
   if(COMM_CB_LIN_CS_FLAG)
      LIN_CS_PORT = 1;
   else
      LIN_CS_PORT = 0;
#endif

   //---------------------------------------------
   //--- SWITCH CONTROLLER STATE
   //---------------------------------------------
   switch(comm_state)
   {
      Nop();
      
      //====================================================
      case COMM_TAG_LIN_INIT:
            comm_lin_init();
            break;

      //====================================================
      case COMM_TAG_LIN_XMT_BRK:
      case COMM_TAG_LIN_XMT_BRK_DATA:
      case COMM_TAG_LIN_XMT_FRAME:

         //---------------------------------------
         //--- XMIT BREAK (ALL CMNDS REQUIRE IT)
         //---------------------------------------
         if(not COMM_0_FLAG)
         {

#ifdef __DEBUG_LIN_AUX2
            Nop();
            LIN_AUX2_PORT = 1;
#endif

            if(TXSTAbits.TRMT == 0)
               break;
               
            COMM_LIN_ABAUD_DISABLE_FLAG = 1;          // DISABLE AUTOBAUD
            COMM_0_FLAG = 1;                          // SET OP FLAG
            TXSTAbits.SENDB = 1;                      // SET 'BREAK' CONTROL
            PIE1bits.TXIE = 1;                        // ENABLE INT(S)
            TXREG = 0;                                // LOAD TXREG WITH DUMMY ZERO
            COMM_LIN_TX_SVC_FLAG = 1;                 // ISR FLAG TO CALL SERVICE
            comm_led_busy_data();                     // SET LED BUSY
            if(COMM_CB_LIN_EVENT_EN_BREAK_TX_FLAG)
               comm_post_event(COMM_TAG_LIN_EVENT_BREAK_TX);

#ifdef __DEBUG_LIN_AUX2
            LIN_AUX2_PORT = 0;
#endif
            break;
         }

         //---------------------------------------
         //--- IF BREAK ONLY, WAIT UNTIL DONE ...
         //---------------------------------------
         if(comm_state == COMM_TAG_LIN_XMT_BRK) 
         {
            if(PIR1bits.TXIF)
            {
               PIE1bits.TXIE = 0;                  // DISABLE INT(S) - DONE!
               COMM_LIN_TX_SVC_FLAG = 0;           // RST FLAG: TX SERVICE
               comm_state = COMM_STATE_GEN_NEXT;   // ADVANCE THE SCRIPT
               COMM_PROC_LOOP_FLAG = 1;
            }
            break;
         }

         //---------------------------------------
         //--- 'IMPLIED' SYNC CHARACTER
         //---------------------------------------
         if(comm_state == COMM_TAG_LIN_XMT_FRAME)
            COMM_3_FLAG = 1;

         //-------------------------------------------------
         //--- take advantage of state to xmit data byte(s)
         //-------------------------------------------------
         comm_state = COMM_TAG_LIN_XMT_DATA;
         
      //====================================================
      case COMM_TAG_LIN_XMT_DATA:
      
         //-----------------------------
         //--- GET BYTE COUNT
         //-----------------------------
         if(not COMM_1_FLAG)
         {

            if(TXSTAbits.TRMT == 0)
               break;
               
            if(comm_data_get())                 // script byte available ?
            {
               COMM_1_FLAG = 1;                 //
               comm_cmnd_count_2 = comm_data;   // load byte count
            }
            else
               break;
         }

         //-----------------------------
         //--- CHECK BYTE COUNT
         //-----------------------------
         if(COMM_1_FLAG)
         {
            //------------------------------------
            //--- 'IMPLIED' SYNC CHARACTER 
            //    (NOT PART OF SUPPLIED DATA)
            //------------------------------------

            if(COMM_3_FLAG)
            {
               if(PIR1bits.TXIF)
               {
                  COMM_3_FLAG = 0;
                  comm_data = 0x55;
                  comm_lin_svc_tx();
               }
               else
                  break;
            }

            //------------------------------------
            //--- BYTE COUNT == 0 ??
            //------------------------------------
            else if(comm_cmnd_count_2 == 0)
            {
               if(PIR1bits.TXIF)
               {
                  PIE1bits.TXIE = 0;
                  COMM_LIN_TX_SVC_FLAG = 0;
                  comm_state = COMM_STATE_GEN_NEXT;
               }
               break;
            }

            //------------------------------------
            //--- BYTE COUNT != 0 !
            //------------------------------------
            else
            {
               if(PIR1bits.TXIF)
               {
                  if(comm_data_get())
                  {
                     comm_cmnd_count_2--;
                     comm_led_busy_data();    
                     comm_lin_svc_tx();
                  }
               }
            }
         }
         break;

      //====================================================
      case COMM_TAG_LIN_BAUD_SET:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               SPBRG = comm_data;
               COMM_SB_LIN_BRG_LO_REG = SPBRG;
               COMM_0_FLAG = 1;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         else
         {
            if(comm_data_get())
            {
               SPBRGH = comm_data;
               COMM_SB_LIN_BRG_HI_REG = SPBRGH;
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         break;

      //====================================================
      case COMM_TAG_LIN_SPROFILE_ID_WRITE:

         //-----------------------------
         //--- FETCH ID
         //-----------------------------
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               COMM_0_FLAG = 1;
               comm_data_3 = comm_data;
               if(comm_lin_sp_iddef_search(comm_data))
                  COMM_4_FLAG = 1;
            }
         }

         //-----------------------------
         //--- FETCH BYTE COUNT
         //-----------------------------
         if(not COMM_1_FLAG)
         {
            if(comm_data_get())
            {
               COMM_1_FLAG = 1;
               comm_cmnd_count_1 = comm_data;
            }
         }

         //-----------------------------
         //--- FETCH BYTES
         //-----------------------------
         if(COMM_1_FLAG)
         {
            while(1)
            {

               //---------------------------------
               //--- TAG BYTE COUNT ZERO
               //---------------------------------
               if(comm_cmnd_count_1 == 0)
               {
                  comm_state = COMM_STATE_GEN_NEXT;
                  COMM_PROC_LOOP_FLAG = 1;
                  break;
               } //--- END: BYTE COUNT ZERO - EXIT ...
               
               //---------------------------------
               //--- TAG BYTE COUNT NOT ZERO
               //---------------------------------
               else
               {
                  if(comm_data_get())
                  {
                     comm_cmnd_count_1--;
                     if(COMM_4_FLAG)
                     {
                        if(COMM_LIN_SP_NBYTES)
                        {
                           cbuf3_data[COMM_LIN_SP_INDEX_DATA] = comm_data;
                           COMM_LIN_SP_NBYTES--;
                           COMM_LIN_SP_INDEX_DATA++;
                        }
                     }
                  }
                  else
                     break;
               } //--- END: BYTE COUNT NOT ZERO
            } //--- END: HARD LOOP FETCHING/PROCESSING BYTES FROM SCRIPT
         } //--- END: FETCHING BYTES FROM SCRIPT
         break;
               

      //====================================================
      case COMM_TAG_LIN_SPROFILE_ID_READ:
         if(comm_data_get())
         {
            comm_post_cbuf2(COMM_TAG_LIN_SPROFILE_ID_DATA);
            comm_post_cbuf2(comm_data);
            if(comm_lin_sp_iddef_search(comm_data))
            {
               comm_post_cbuf2(COMM_LIN_SP_NBYTES);
               while(COMM_LIN_SP_NBYTES--)
               {
                  comm_post_cbuf2(cbuf3_data[COMM_LIN_SP_INDEX_DATA]);
                  COMM_LIN_SP_INDEX_DATA++;
               }
            }
               
            //--- NO ID FOUND IN PROFILE - SEND ZERO BYTES
            else
            {
               comm_post_cbuf2(0x00);
            }
            comm_state = COMM_STATE_GEN_NEXT;
            COMM_PROC_LOOP_FLAG = 1;
         }
         break;

      //====================================================
      case COMM_STATE_GEN_NEXT:
         break;
         
      //====================================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;

   } // end SWITCH
   
   //---------------------------------------------
   //--- RE-ENABLE INTERRUPT IF NECESSARY
   //---------------------------------------------
   if(COMM_LIN_GIEL1_FLAG)
      INTCONbits.GIEL = 1;
   //---------------------------------------------

}



//==========================================================
//--- comm_lin_postproc()
//    post processing
//
//    look for status error(s)
//    set appropriate STATUS_BLOCK flags
//    set composite status bit
//==========================================================
void comm_lin_postproc(void)
{
   TD_BYTE  temp1;
   
   //-----------------------------------
   //--- SET 'ERROR' FLAG(s)
   //-----------------------------------
   if(COMM_LIN_FERR_FLAG)
   {
      COMM_SB_LIN_STAT_FERR_FLAG = 1;
   }
   COMM_LIN_FERR_FLAG = 0;
   
   if(COMM_LIN_OERR_FLAG || RCSTAbits.OERR)
   {
      COMM_SB_LIN_STAT_OERR_FLAG = 1;
      RCSTAbits.CREN = 0;
      comm_data_2 = RCREG;
      RCSTAbits.CREN = 1;
   }
   COMM_LIN_OERR_FLAG = 0;

   if(COMM_LIN_ABERR_FLAG)
      COMM_SB_LIN_STAT_ABAUDERR_FLAG = 1;
   COMM_LIN_ABERR_FLAG = 0;
   
   //-----------------------------------
   //--- SAVE CURRENT BAUD
   //-----------------------------------
   COMM_SB_LIN_BRG_LO_REG = SPBRG;
   COMM_SB_LIN_BRG_HI_REG = SPBRGH;

   //-----------------------------------
   //--- FLASH 'DATA' LED
   //-----------------------------------
   if(COMM_LIN_DATA_FLAG)
   {
      comm_led_busy_data();
      COMM_LIN_DATA_FLAG = 0;
   }
   
   //-----------------------------------
   //--- transaction error
   //-----------------------------------
   //--- FERR & OERR set in data receive module
   if(LIN_AFAULT_PORT==0)
   {
#ifndef __DEBUG_LIN_AUX2
      COMM_SB_LIN_STAT_AFAULT_FLAG = 1;
#endif
   }

   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(COMM_SB_LIN_STAT_ERR_REG & COMM_SB_LIN_STAT_ERR_REG_MASK)
   {
      Nop();
      COMM_SB_LIN_STAT_ERR_FLAG = 1;

   }
   
   //-----------------------------------
   //--- if error status change - post it ...
   //-----------------------------------
   /*
   if(COMM_SB_LIN_STAT_ERR_REG != exec_status_save_2)
   {
      Nop();
      exec_status_save_2 = COMM_SB_LIN_STAT_ERR_REG;
      comm_post_event_1b(COMM_TAG_LIN_EVENT_STAT_ERR, COMM_SB_LIN_STAT_ERR_REG);
   }
   */
   
   //-----------------------------------
   //--- ACTIVE STATUS
   //-----------------------------------

}


//==========================================================
//--- SLAVE PROFILE UTILITIES
//==========================================================

//==========================================================
//--- comm_lin_sp_iddef_search()
//==========================================================
//    search slave profile for given id definition
//    if found:
//       - IDENTIFIER info loaded in global registers
//       - return "1"
//    if not found:
//       - return "0"
//==========================================================
TD_BYTE comm_lin_sp_iddef_search(TD_BYTE id)
{
//   id &= 0x3F;
   COMM_LIN_SP_INDEX = 1;   
   while(comm_lin_sp_iddef_next())
   {
      if(COMM_LIN_SP_ID == id)
         return(1);
   }
   return(0);
}

//==========================================================
//--- comm_lin_sp_iddef_next()
//==========================================================
//    examine slave profile at the current cbuf3 index
//    if not an ID DEF
//       - return "0"
//    if it is an ID DEF
//       - load the registers with contents of ID DEF
//       - advance the index to next ID DEF
//       - return "1"
//==========================================================
TD_BYTE comm_lin_sp_iddef_next(void)
{
   TD_BYTE i;

   i = cbuf3_data[COMM_LIN_SP_INDEX]; 
   if(i & 0x80)
   {
      COMM_LIN_SP_IDDEF = i; 
      COMM_LIN_SP_NBYTES = i & 0x0F;
      COMM_LIN_SP_INDEX++;
      COMM_LIN_SP_ID = cbuf3_data[COMM_LIN_SP_INDEX];
      COMM_LIN_SP_INDEX_DATA = ++COMM_LIN_SP_INDEX;
      COMM_LIN_SP_INDEX += COMM_LIN_SP_NBYTES;
      return(1);
   }
   return(0);
}


//==========================================================
//--- comm_lin_rx_isr()
//==========================================================
//    IN ORDER OF PRIORITY, LOOK FOR:
//    1. BREAK
//    2. AUTOBAUD FINISH
//    3. DATA
//==========================================================
void comm_lin_rx_isr(void)
{
   TD_BYTE i;
   
   //---------------------------------------------
   //--- SAVE ERROR FLAG(s), FETCH BYTE
   //---------------------------------------------
   if(RCSTAbits.FERR == 1)                   // FRAMING ERROR ?
      COMM_LIN_FERR_FLAG = 1;
   if(RCSTAbits.OERR == 1)                   // OVERRUN ERROR ?
      COMM_LIN_OERR_FLAG = 1;
   comm_data_2 = RCREG;                      // RETRIEVE DATA BYTE

#ifdef __DEBUG_LIN_AUX1
   LIN_AUX1_PORT = 1;
#endif

   //=======================================================
   //--- BREAK ?!?
   //=======================================================
   Nop();
   if((comm_data_2==0) && (COMM_LIN_FERR_FLAG))
   {
      COMM_LIN_FERR_FLAG = 0;                // FERR EXPECTED ON BREAK
      
      COMM_LIN_2_FLAG = 1;                   // FLAG INDICATES HARD ABAUD ERROR, ELSE, IGNORED
   

      //--- POST EVENT MARKER
      if(COMM_CB_LIN_EVENT_EN_BREAK_RX_FLAG)
         comm_post_event(COMM_TAG_LIN_EVENT_BREAK_RX);
            
      //--- SETUP OP FLAGS
      COMM_LIN_SEQ_BRK_FLAG = 1;
      COMM_LIN_SEQ_SYN_FLAG = 0;
      COMM_LIN_SEQ_ID_FLAG = 0;
      COMM_LIN_RX_SP_FLAG = 0;
      COMM_LIN_TX_SP_FLAG = 0;
      COMM_LIN_RX_SP_FLAG = 0;
      
      //------------------------------------------
      //--- AUTOBAUD ENABLED ?
      //------------------------------------------
      if((COMM_CB_LIN_AUTOBAUD_FLAG) && (COMM_LIN_ABAUD_DISABLE_FLAG==0))
      {

         //-----------------------------
         //--- WAIT BREAK COMPLETE
         //    (ERR IF TIMEOUT)
         //-----------------------------
         timer3_set(COMM_LIN_WAIT_BREAK_T3);
         COMM_LIN_ABERR_FLAG = 1;               // PRESET ERROR FLAG
         while(PIR2bits.TMR3IF == 0)
         {
            if(LIN_RX_PORT)                     // SYNC COMPLETE ?
            {
               COMM_LIN_ABERR_FLAG = 0;         // SYNC COMPLETE, RESET ERROR FLAG !
               break;
            }
         }
         
         //---------------------------------------
         //--- BREAK TERMINATED W/O TIMEOUT
         //    ENGAGE AUTOBAUD
         //---------------------------------------
         Nop();

         if(COMM_LIN_ABERR_FLAG == 0)
         {     
            
            //-----------------------------
            //--- ENGAGE AUTOBAUD
            //-----------------------------
            BAUDCONbits.ABDEN = 1;

            //-----------------------------
            //--- WAIT ABAUD COMPLETE ...
            //    (ERR IF TIMEOUT)
            //-----------------------------
            COMM_LIN_COUNT = 0;
            COMM_LIN_3_FLAG = 1;
            COMM_LIN_ABERR_FLAG = 1;
            timer3_set(COMM_LIN_WAIT_AB_T3);
            while(PIR2bits.TMR3IF == 0)
            {
               if(COMM_LIN_3_FLAG == 0)
               {
                  if(LIN_RX_PORT == 1)
                  {
                     if(COMM_LIN_COUNT == 5)
                     {
                        Nop();
                        BAUDCONbits.ABDEN = 0;
                        comm_data_2 = RCREG;
                        COMM_LIN_ABERR_FLAG = 0;
                        break;
                     }
                     COMM_LIN_3_FLAG = 1;
                  }
               }
               else
               {
                  if(LIN_RX_PORT == 0)
                  {
                     COMM_LIN_3_FLAG = 0;
                     COMM_LIN_COUNT++;
                  }
               }
            }
            
            //---
            if(BAUDCONbits.ABDEN)
            {
               BAUDCONbits.ABDEN = 0;           // SAFETY NET CLEAN UP ON TIMEOUT
               COMM_LIN_2_FLAG = 1;             // HARD ERROR
            }
                  
            //-----------------------------
            //--- AUTOBAUD OVERFLOW ?
            //-----------------------------
            if(BAUDCONbits.ABDOVF)
            {
               BAUDCONbits.ABDEN = 0;
               BAUDCONbits.ABDOVF = 0;
               COMM_LIN_ABERR_FLAG = 1;
            }

            //-----------------------------------------
            //--- IF STATUS 'OK' SO FAR
            //    CHECK BAUD 'CHANGE' TO BE WITHIN LIMITS
            //    BAUD_CODE LIMITS: +/-12.5%
            //       CORRESPONDS TO BAUD: +14%/-11%
            //    OPTION:
            //    BAUD_CODE LIMITS: +/-25.0%
            //       CORRESPONDS TO BAUD: +33%/-20%
            //-----------------------------------------
            if(COMM_LIN_ABERR_FLAG == 0)
            {
               comm_macro_count_1_W.lo = SPBRG;
               comm_macro_count_1_W.hi = SPBRGH;
               comm_macro_count_2_W.lo = COMM_CB_LIN_BRG_LO_REG;
               comm_macro_count_2_W.hi = COMM_CB_LIN_BRG_HI_REG;
               if(comm_macro_count_1_W._w > comm_macro_count_2_W._w)
                  comm_macro_count_1_W._w = comm_macro_count_1_W._w - comm_macro_count_2_W._w;
               else
                  comm_macro_count_1_W._w = comm_macro_count_2_W._w - comm_macro_count_1_W._w;
               comm_macro_count_2_W._w >>= 3;
               if(comm_macro_count_1_W._w > comm_macro_count_2_W._w)
                  COMM_LIN_ABERR_FLAG = 1;
               COMM_LIN_2_FLAG = 0;
            }
         } //--- END: BREAK OK
         
         //----------------------------------------------------
         //--- IF AUTOBAUD ERROR, RE-LOAD DEFAULT BAUD
         //----------------------------------------------------
         if(COMM_LIN_ABERR_FLAG)
         {
            SPBRG = COMM_CB_LIN_BRG_LO_REG;
            SPBRGH = COMM_CB_LIN_BRG_HI_REG;
            if(COMM_LIN_2_FLAG == 0)
               COMM_LIN_ABERR_FLAG = 0;
         }
         else
         {
            COMM_LIN_SEQ_SYN_FLAG = 1;
         }
         
         if(COMM_CB_LIN_EVENT_EN_AUTOBAUD_FLAG)
            comm_post_event_2b(COMM_TAG_LIN_EVENT_AUTOBAUD, SPBRG, SPBRGH);

      } //--- END AUTOBAUD
   } //--- END BREAK DETECTION
   

   //=======================================================
   //--- DATA
   //=======================================================
   else
   {
      Nop();
      COMM_LIN_ABAUD_DISABLE_FLAG = 0;

      //---
      COMM_LIN_DATA_FLAG = 1;

      //--------------------------------
      //--- EXPECTING 'SYNC' BYTE ?
      //--------------------------------
      if(not COMM_LIN_SEQ_SYN_FLAG)
      {
         //if(comm_data_2 == 0x55)
            COMM_LIN_SEQ_SYN_FLAG = 1;

      } //--- END 'SYNC' BYTE

      //--------------------------------
      //--- EXPECTING 'DATA' BYTE !
      //--------------------------------
      else
      {
         //------------------------------------------         
         //--- POST 'DATA RX' EVENT TAG(S)
         //------------------------------------------         
         comm_post_event_1b(COMM_TAG_GEN_DATA, comm_data_2);
         if(COMM_CB_LIN_EVENT_EN_DATA_RX_FLAG)
            comm_post_event_1b(COMM_TAG_LIN_EVENT_BYTE_RX, comm_data_2);

         //-----------------------------
         //--- DATA: 'FRAME ID' ?
         //-----------------------------
         if(not COMM_LIN_SEQ_ID_FLAG)
         {
            COMM_LIN_SEQ_ID_FLAG = 1;

            //--------------------------
            //--- SLAVE MODE ?
            //--------------------------
            COMM_LIN_TX_SP_FLAG = 0;
            COMM_LIN_RX_SP_FLAG = 0;

            if(COMM_CB_LIN_SLAVE_FLAG)
            {
               if(comm_lin_parity(comm_data_2) != comm_data_2)
               {
                  if(COMM_CB_LIN_EVENT_EN_IDPARITY_ERR_FLAG)
                  {
                     comm_post_event_1b(COMM_TAG_LIN_EVENT_IDPARITY_ERR, \
                        comm_data_2);
                  }
               }
               if(comm_lin_sp_iddef_search(comm_data_2))
               {
                  //--- INIT CHECKSUM
                  *COMM_LIN_SP_CHECKSUM_WP = 0;
                  if((COMM_LIN_SP_IDDEF_ENHANCED_CS_FLAG) && (COMM_LIN_SP_ID<0x3C))
                     *COMM_LIN_SP_CHECKSUM_WP = comm_data_2;                  
                  
                  if(COMM_LIN_SP_IDDEF_PUBLISH_FLAG)
                  {
                     COMM_LIN_TX_SP_FLAG = 1;
                     comm_lin_tx_isr();
                  }
                  else
                  {
                     COMM_LIN_RX_SP_FLAG = 1;
                  }
               } //--- END: SEARCH SLAVE PROFILE FOR GIVEN ID
            } //--- END: SLAVE MODE
         } //--- END: EXPECTING FRAME ID
         
         //--------------------------
         //--- DATA: 
         //--------------------------
         else
         {
            //--------------------------
            //--- SLAVE MODE ?
            //--------------------------
            if(COMM_CB_LIN_SLAVE_FLAG)
            {
               if(COMM_LIN_RX_SP_FLAG)
               {
                  if(COMM_LIN_SP_NBYTES)
                  {
                     *COMM_LIN_SP_CHECKSUM_WP += comm_data_2;
                     if(*COMM_LIN_SP_CHECKSUM_WP > 255)
                        *COMM_LIN_SP_CHECKSUM_WP -= 255;
                     cbuf3_data[COMM_LIN_SP_INDEX_DATA] = comm_data_2;
                     COMM_LIN_SP_INDEX_DATA++;
                     COMM_LIN_SP_NBYTES--;
                  } //--- END: RX_SP ACTIVE, NBYTES!=0
                  else
                  {
                     if(not COMM_LIN_SP_IDDEF_CHKSUMDATA_FLAG)
                     {
                        //--- CHECKSUM GOOD
                        if(comm_data_2 == ~(*COMM_LIN_SP_CHECKSUM_BP))
                        {
                           Nop();
                        }
                        //--- CHECKSUM BAD
                        else
                        {
                           if(COMM_CB_LIN_EVENT_EN_CHECKSUM_ERR_FLAG)
                              comm_post_event_1b(COMM_TAG_LIN_EVENT_CHECKSUM_ERR, COMM_LIN_SP_ID);
                        }
                     } 
                     COMM_LIN_RX_SP_FLAG = 0;
                  } //--- END: RX_SP ACTIVE, NBYTES==0
               } //--- END: RX_SP ACTIVE
            } //--- END: RX DATA AFTER ID IN SLAVE MODE
         } //--- END: RX DATA AFTER FRAME ID
      } //--- END: RX DATA (NOT SYNC BYTE)
   } //--- END: RX DATA (NOT AUTOBAUD or BREAK)

#ifdef __DEBUG_LIN_AUX1
   LIN_AUX1_PORT = 0;
#endif

} //--- END: RX ISR (BREAK, AUTOBAUD or DATA
   

//==========================================================
//--- comm_lin_tx_isr()
//==========================================================
//    USART TX interrupt service (in LIN mode)
//    TX data has 2 sources (selected by bit flags)
//    SLAVE & SCRIPT SERVICE ... SLAVE has priority
//==========================================================
void comm_lin_tx_isr(void)
{
   TD_BYTE i;
   
   //-----------------------------------
   //--- SERVICE SCRIPT ?
   //-----------------------------------
   if  (COMM_LIN_TX_SVC_FLAG)
   {
      comm_lin_svc();
   }
   
   //-----------------------------------
   //--- SLAVE TX IN PROGRESS ?
   //-----------------------------------
   else if (COMM_LIN_TX_SP_FLAG)
   
   {
      if(COMM_LIN_SP_NBYTES)
      {
         i = cbuf3_data[COMM_LIN_SP_INDEX_DATA];
         *COMM_LIN_SP_CHECKSUM_WP += i;
         if(*COMM_LIN_SP_CHECKSUM_WP > 255)
            *COMM_LIN_SP_CHECKSUM_WP -= 255;
         comm_lin_tx(i);
         COMM_LIN_SP_INDEX_DATA++;
         COMM_LIN_SP_NBYTES--;
      }
      else
      {
         if(not COMM_LIN_SP_IDDEF_CHKSUMDATA_FLAG)
            comm_lin_tx(~(*COMM_LIN_SP_CHECKSUM_BP));
         COMM_LIN_TX_SP_FLAG = 0;
      }
   }
      
   //-----------------------------------
   //--- DONE !
   //-----------------------------------
   else
   {
      PIE1bits.TXIE = 0;
   }
}

//==========================================================         
//--- comm_lin_svc_tx()
//==========================================================         
//    transmit 'comm_data' byte
//==========================================================         
void comm_lin_svc_tx(void)
{
   COMM_LIN_TX_SVC_FLAG = 1;
   comm_lin_tx(comm_data);
}


//==========================================================
//--- comm_lin_tx()
//==========================================================
//    send/transmit data byte
//    enable TX
//    enable TX interrupt
//==========================================================
void comm_lin_tx(TD_BYTE data)
{
   TXREG = data;
   PIE1bits.TXIE = 1;
   if(COMM_CB_LIN_EVENT_EN_DATA_TX_FLAG)
      comm_post_event_1b(COMM_TAG_LIN_EVENT_BYTE_TX, comm_data);
}

//==========================================================
//--- comm_lin_parity()
//==========================================================
//    computes 2 parity bits associated with LIN FRAME ID
//    parity is computed on calling argument - 'data'
//    the 'result' is returned as the function value
//    where:
//    result[7:7] = 1 ^ (data[5] ^ data[4] ^ data[3] ^ data[1])
//    result[6:6] = 0 ^ (data[4] ^ data[2] ^ data[1] ^ data[0])
//    result[5:0] = data[5:0]
//==========================================================
TD_BYTE comm_lin_parity(TD_BYTE data)
{
   TD_BYTE mask;

   data &= 0x3F;
   
   mask = data & 0x17;
   if((mask==0x01) || (mask==0x02) || (mask==0x04) || (mask==0x07) || \
      (mask==0x10) || (mask==0x13) || (mask==0x15) || (mask==0x16))
      data |= 0x40;
   mask = data & 0x3A;
   if((mask==0x00) || (mask==0x0A) || (mask==0x12) || (mask==0x18) || \
      (mask==0x22) || (mask==0x28) || (mask==0x30) || (mask==0x3A))
      data |= 0x80;
      
   return(data);
}

