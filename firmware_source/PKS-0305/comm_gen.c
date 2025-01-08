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
//    Filename:            comm_gen.c
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
//--- COMM_GEN
//
// "general" communication facilities
// refers to commnication "mode" to call appropriate 
// initialization/processing/post-processing functions
// otherwise
// processes script commands common to all "modes"
//==========================================================
//--- FUNCTIONS
//
//--- comm_init()
//    general initialization - default to "idle"
//
//--- comm_init_mode()
//    calls mode-dependant initialization
//
//--- comm_reset()
//    perform general "reset" - clear flags & registers
//
//--- comm_svc()
//    comm controller service: call mode-dependent function
//
//--- comm_gen()
//    process "general" comm controller commands
//
//=== UTILITIES
//
//--- comm_post_event()
//    post "EVENT" marker (and TIME if enabled)
//
//--- comm_post_event_1b()
//    post "EVENT" marker w/ 1 "event-dependant" data byte
//
//--- comm_post_event_2b()
//    post "EVENT" marker w/ 2 "event-dependant" data bytes
//
//--- comm_post_event_time()
//    post event: "TIME"
//
//--- comm_post_data()
//    post event: "DATA"
//
//--- comm_post_event_time_get()
//    fetch time stamp, post TIMER_OVF event if necessary
//
//--- comm_data_get()
//    fetch byte from script
//
//--- comm_post_cbuf2()
//    post data byte to CBUF2
//
//--- comm_post_cbuf3()
//    post data byte to CBUF3
//
//--- comm_led_busy_data()
//    indicate data transaction using LED
//
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0003 - 03-01-08
//    -- added 'break;' to case statement for COMM_TAG_GEN_MARKER_HOST
//       was allowing case to fall thru and reset time stamp
//    -- added function comm_clear()
//    -- eliminated comm_sb_clear
//    -- re-organized init function into reset/config/clear
//
//    12-05-07 - ME
//    -- disable vsrc_init() in function comm_init()
//    -- disabled CCP1CON=0 in comm_reset()
//    -- added cbuf1_init(),cbuf2_init(),cbuf3_init() to comm_init()
//
//    0002 - 03-07-07 - ME
//    -- added 'post' functions for cbuf3
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 

//==========================================================
//--- INCLUDES
//==========================================================
#include "project.h"
#include "cbuf.h"
#include "led2.h"
#include "comm_gen.h"
#include "comm_i2cm.h"
#include "comm_spi.h"
#include "comm_usart.h"
#include "comm_i2cs.h"
#include "comm_lin.h"
#include "timer0.h"
#include "vsrc.h"
#include "exec.h"

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
TD_BYTE comm_state;
TD_BYTE comm_data;
TD_BYTE comm_data_2;
TD_BYTE comm_data_3;
TD_BYTE comm_cmnd_count_1;
TD_BYTE comm_cmnd_count_2;
TD_BYTE comm_cmnd_macro_count_1;
TD_BYTE comm_cmnd_macro_count_2;
TD_WORD_W comm_macro_count_1_W;
TD_WORD_W comm_macro_count_2_W;
TD_BYTE_B comm_flags1_B;
TD_BYTE_B comm_flags2_B;
TD_BYTE_B comm_varbs_B[16];
TD_BYTE  comm_data_sim;

//debug
TD_BYTE  comm_temp;

//==========================================================
//--- comm_init()
//    general initialization - default to "idle"
//==========================================================
void comm_init(void)
{
   comm_gen_reset();
   comm_gen_config();
   comm_init_mode(COMM_MODE_IDLE);
   comm_clear();
}

//==========================================================
//--- comm_reset()
//    perform general "reset" - clear flags & registers
//==========================================================
void comm_gen_reset(void)
{

   //-----------------------------------
   //--- CLEAR MODE_DEPENDENT H/W
   //-----------------------------------
   comm_lin_reset();
   //comm_i2c_reset();
   //comm_spi_reset();
   comm_usart_reset();
   
   //--- (CHANGE TO comm_i2c_reset() WHEN POSSIBLE)
   SSPCON1 = 0;      // SSP module
   PIE1bits.SSPIE = 0;
   //CCP1CON = 0;      // CCP control

   //-----------------------------------
   //--- I/O PINS
   //-----------------------------------
   //--- (CHANGE TO AUX MODULE WHEN POSSIBLE)
   COMM_IO_1A_DIR = 1;     // set all I/O pins to INPUT
   COMM_IO_4A_DIR = 1;
   COMM_IO_5A_DIR = 1;
   COMM_IO_6A_DIR = 1;

   COMM_IO_1B_DIR = 1;
   COMM_IO_4B_DIR = 1;
   COMM_IO_5B_DIR = 1;
   COMM_IO_6B_DIR = 1;

}   
   
//==========================================================
//--- comm_gen_config()
//==========================================================
void comm_gen_config(void)
{
   //-----------------------------------
   //--- PULLUPS
   //-----------------------------------
   COMM_PULLUP_PIN = 1;
   COMM_PULLUP_TRIS = 0;

   ADCON1 = 0x0F;
}
   

//==========================================================
//--- comm_gen_clear()
//    calls mode-dependant 'clear' function
//
//    the 'mode' is determined from the STATUS_BLOCK; therfore,
//    this function assumes/requires prior 'initialization'
//==========================================================
void comm_clear(void)
{
   //-----------------------------------
   //--- CIRCULAR BUFFERS
   //-----------------------------------
   cbuf1_init();                          // clear buffer
   cbuf2_init();                          // clear buffer
   //cbuf3_init();                          // clear buffer (allow protocol-dependant clear)

   //-----------------------------------
   //--- OPERATIONAL VARBS
   //-----------------------------------
   comm_flags1_B._byte = 0;               // clear flags
   comm_flags2_B._byte = 0;
   comm_state = COMM_STATE_GEN_RESET;     // init controller
   exec_status_save_1 = 0;
   exec_status_save_2 = 0;
   exec_status_save_3 = 0;
   comm_data_sim = 0;
   
   //-----------------------------------
   //--- clear comm timer(s)
   //-----------------------------------
   timer0_reg_clear();
   
   //-----------------------------------
   //--- LEDs
   //-----------------------------------
   led_1_config(COMM_LED_CONFIG_OFF);
   led_2_config(COMM_LED_CONFIG_OFF);

   //-----------------------------------
   //--- VSRC
   //-----------------------------------
   COMM_VSRC_TIMER = COMM_VSRC_MEAS_INTV_TA;

   //-----------------------------------
   //--- PULLUPS
   //-----------------------------------
   COMM_PULLUP_PIN = 1;
   COMM_PULLUP_TRIS = 0;

   //-----------------------------------
   //--- STATUS BLOCK
   //-----------------------------------
   exec_status_B[4]._byte = 0;
   exec_status_B[5]._byte = 0;
   //exec_status_B[6]._byte = 0;        // SAVE MODE
   exec_status_B[7]._byte = 0;
   exec_status_B[8]._byte = 0;
   exec_status_B[9]._byte = 0;
   exec_status_B[10]._byte = 0;
   exec_status_B[11]._byte = 0;

   exec_status_B[12]._byte = 0;
   exec_status_B[13]._byte = 0;
   exec_status_B[14]._byte = 0;
   exec_status_B[15]._byte = 0;
   exec_status_B[16]._byte = 0;
   exec_status_B[17]._byte = 0;
   exec_status_B[18]._byte = 0;
   exec_status_B[19]._byte = 0;

   //-----------------------------------
   //--- clear active dev comm controller
   //-----------------------------------
   switch(COMM_SB_MODE_REG)
   {

      //--------------------------------
      case COMM_MODE_LIN:
         comm_lin_clear();
         break;

      //--------------------------------
      case COMM_MODE_SPI_M:
      case COMM_MODE_SPI_S:
      case COMM_MODE_MWIRE_M:
         comm_spi_clear();
         break;
                  
      //--------------------------------
      //--------------------------------
      case COMM_MODE_IDLE:
      case COMM_MODE_I2CM:
      case COMM_MODE_USART_A:
      case COMM_MODE_USART_SM:
      case COMM_MODE_USART_SS:
      case COMM_MODE_I2CS:
      default:
         cbuf3_init();                          // clear buffer
         break;
   }

}

//==========================================================
//--- comm_varbs_clear()
//==========================================================
void comm_varbs_clear(void)
{
   comm_varbs_B[0]._b = 0;
   comm_varbs_B[1]._b = 0;
   comm_varbs_B[2]._b = 0;
   comm_varbs_B[3]._b = 0;
   comm_varbs_B[4]._b = 0;
   comm_varbs_B[5]._b = 0;
   comm_varbs_B[6]._b = 0;
   comm_varbs_B[7]._b = 0;
   comm_varbs_B[8]._b = 0;
   comm_varbs_B[9]._b = 0;
   comm_varbs_B[10]._b = 0;
   comm_varbs_B[11]._b = 0;
   comm_varbs_B[12]._b = 0;
   comm_varbs_B[13]._b = 0;
   comm_varbs_B[14]._b = 0;
   comm_varbs_B[15]._b = 0;
}

//==========================================================
//==========================================================
//--- comm_init_mode()
//    calls mode-dependant initialization
//
//    if initialization successful
//    set "mode" in STATUS_BLOCK
//==========================================================
//==========================================================
void comm_init_mode(TD_BYTE mode)
{

   //-----------------------------------
   //--- init active dev comm controller
   //-----------------------------------
   switch(mode)
   {
      COMM_SB_INIT_ERR_FLAG = 0;

      //--------------------------------
      case COMM_MODE_IDLE:
         break;

      //--------------------------------
      case COMM_MODE_I2CM:
         if(comm_i2cm_init())
            COMM_SB_INIT_ERR_FLAG = 1;
         break;

      //--------------------------------
      case COMM_MODE_SPI_M:
      case COMM_MODE_SPI_S:
      case COMM_MODE_MWIRE_M:
         Nop();
         if(comm_spi_init())
            COMM_SB_INIT_ERR_FLAG = 1;
         break;

      //--------------------------------
      case COMM_MODE_USART_A:
      case COMM_MODE_USART_SM:
      case COMM_MODE_USART_SS:
         if(comm_usart_init())
            COMM_SB_INIT_ERR_FLAG = 1;
         break;

      //--------------------------------
      case COMM_MODE_I2CS:
         if(comm_i2cs_init())
            COMM_SB_INIT_ERR_FLAG = 1;
         break;

      //--------------------------------
      case COMM_MODE_LIN:
         if(comm_lin_init())
            COMM_SB_INIT_ERR_FLAG = 1;
         break;
         
      //--------------------------------
      default:
         COMM_SB_INIT_ERR_FLAG = 1;
         break;
   }
                       
   if(COMM_SB_INIT_ERR_FLAG == 0)
      COMM_SB_MODE_REG = mode;

}



//==========================================================
//==========================================================
//--- comm_svc()
//    comm controller service: call mode-dependent function
//==========================================================
//==========================================================
void comm_svc(void)
{
   //---------------------------------------------
   //--- VSRC svc
   //---------------------------------------------

   //-----------------------------------
   //--- if new CONTROL_BLOCK ...
   //-----------------------------------
   if(EXEC_CB_NEW_FLAG)
   {

      //----------------------
      //--- SET VSRC
      //----------------------
      if(COMM_CB_VSRC_ENABLE_FLAG)
      {
         if(COMM_CB_VSRC_VAR_FLAG)
         {
            vsrc_set_var(COMM_CB_VSRC_SETPOINT_REG);
         }
         else
         {
            vsrc_set_on();
         }
      }
      else
      {
         vsrc_set_off();
      }
      
   }
   
   //---------------------------------------------
   //--- if timer done, measure VSRC
   //---------------------------------------------
   if(not COMM_VSRC_TIMER)
   {
      COMM_SB_VSRC_MEAS_REG = vsrc_read(COMM_CB_VSRC_FAULTLEVEL_REG);
      COMM_VSRC_TIMER = COMM_VSRC_MEAS_INTV_TA;
      if(VSRC_FAULT_FLAG)
         COMM_SB_VSRC_FAULT_FLAG = 1;
      else
         COMM_SB_VSRC_FAULT_FLAG = 0;
   }

   //---------------------------------------------
   //--- SET: PULLUP
   //---------------------------------------------
   if(COMM_CB_PULLUP_ENABLE_FLAG)
      COMM_PULLUP_PIN = 0;
   else
      COMM_PULLUP_PIN = 1;      

         
   //---------------------------------------------
   //---------------------------------------------
   if(COMM_SB_MODE_REG == COMM_MODE_IDLE)
      return;

   //---------------------------------------------
   //--- preproc activity
   //---------------------------------------------
   do 
   {
      COMM_PROC_LOOP_FLAG = 0;
   
      //---------------------------------------------
      //--- process 
      //---------------------------------------------
      Nop();
      if(comm_state > COMM_STATE_GEN_MAX)
      {
         Nop();
         switch(COMM_SB_MODE_REG)
         {
            case COMM_MODE_I2CM:
               comm_i2cm_svc();
               break;
            case COMM_MODE_SPI:
            case COMM_MODE_MWIRE_M:
               comm_spi_svc();
               break;
            case COMM_MODE_USART_A:
            case COMM_MODE_USART_SM:
            case COMM_MODE_USART_SS:
               comm_usart_svc();
               break;
            case COMM_MODE_I2CS:
               comm_i2cs_svc();
               break;
            case COMM_MODE_LIN:
               comm_lin_svc();
               break;
            default:
               {}
         }
      }
      else
      {
         comm_gen();
      }
   
   } while(COMM_PROC_LOOP_FLAG);
   
   //---------------------------------------------
   //--- postproc activity
   //---------------------------------------------
   //-----------------------------------
   //--- check EVENT timer for overflow
   //-----------------------------------
   if(COMM_CB_EVENT_TIME_FLAG)
   {
      comm_post_event_time_get();
   }

   //-----------------------------------
   //--- check process timeout AB0
   //-----------------------------------
   if(COMM_TIMEOUT_AB0_FLAG)
   {
      if(not timer0_AB0_W._word)
      {
         COMM_SB_TIMEOUT_AB0_FLAG = 1;
         if(COMM_CB_EVENT_TIMEOUT_AB0_FLAG)
            comm_post_event(COMM_TAG_GEN_EVENT_TIMEOUT_AB0);
         COMM_TIMEOUT_AB0_FLAG = 0;
      }
   }
   
   //-----------------------------------
   //--- check process timeout AB1
   //-----------------------------------
   if(COMM_TIMEOUT_AB1_FLAG)
   {
      if(not timer0_AB1_W._word)
      {
         COMM_SB_TIMEOUT_AB1_FLAG = 1;
         if(COMM_CB_EVENT_TIMEOUT_AB1_FLAG)
            comm_post_event(COMM_TAG_GEN_EVENT_TIMEOUT_AB1);
         COMM_TIMEOUT_AB1_FLAG = 0;
      }
   }
   
   //-----------------------------------
   //--- device postproc
   //-----------------------------------
//   if(COMM_SB_MODE_REG == COMM_MODE_I2CM)
//      comm_i2cm_postproc();
   switch(COMM_SB_MODE_REG)
   {
      case COMM_MODE_I2CM:
         comm_i2cm_postproc();
         break;
      case COMM_MODE_SPI:
      case COMM_MODE_MWIRE_M:
         comm_spi_postproc();
         break;
      case COMM_MODE_USART_A:
      case COMM_MODE_USART_SM:
      case COMM_MODE_USART_SS:
         comm_usart_postproc();
         break;
      case COMM_MODE_I2CS:
         comm_i2cs_postproc();
         break;
      case COMM_MODE_LIN:
         comm_lin_postproc();
         break;
      default:
         {}
   }

   //-----------------------------------
   //--- set "COMPOSITE" error
   //-----------------------------------
   if(COMM_SB_GEN_ERR_REG & COMM_SB_GEN_ERR_REG_MASK)
   {
      Nop();
      COMM_SB_GEN_ERR_FLAG = 1;
   }

   //-----------------------------------
   //--- change in error status
   //-----------------------------------
   /*
   if(COMM_CB_EVENT_STATUS_ERR_FLAG)
   {
      if(exec_status_B[4]._byte != exec_status_save_1)
      {
         exec_status_save_1 = exec_status_B[4]._byte;
         comm_post_event_1b(COMM_TAG_GEN_STATUS_ERR, exec_status_B[4]._byte);
      }
   }
   */
   
}
      
//==========================================================
//--- comm_gen()
//    process "general" comm controller commands
//==========================================================
void comm_gen(void)
{
   
   //-----------------------------------
   //--- STALL PROCESSING ?
   //-----------------------------------
   if(COMM_SB_BAD_CMND_FLAG)
   {
      comm_state = COMM_STATE_GEN_STALL;
   }
   
   //=======================================================
   //--- SWITCH: COMM CONTROLLER SCRIPT STATE/COMMAND
   //=======================================================
   switch (comm_state)
   {
      //====================================================
      case COMM_STATE_GEN_NEXT:
         comm_flags1_B._byte = 0;
         comm_state = COMM_STATE_GEN_NEXT_;
         //break;

      //====================================================
      case COMM_STATE_GEN_NEXT_:

         //--- MACRO
         if(COMM_MACRO_FLAG)
         {
            if(not comm_cmnd_macro_count_1)
            {
               comm_state = COMM_STATE_GEN_MACRO_X_INIT;
               COMM_PROC_LOOP_FLAG = 1;
               break;
            }
            COMM_SB_MACRO_BUSY_FLAG = 1;
         }
         else
         {
            COMM_SB_MACRO_BUSY_FLAG = 0;
            COMM_SB_MACRO_CONT_FLAG = 0;
         }

         //--- FETCH SCRIPT CMND FROM BUFFER
         if(comm_data_get())
         {
            comm_state = comm_data;
            COMM_PROC_LOOP_FLAG = 1;
            if(comm_state < COMM_TAG_MIN)
            {
               COMM_SB_BAD_CMND_FLAG = 1;
            }
            else
            {
               COMM_SB_BUSY_FLAG = 1;
               if(not EXEC_CB_LED1_DISABLE_DFLT_FLAG)
               {
                  led_1_config(COMM_LED_CONFIG_BUSY_SCRIPT_STR);
               }
            }
         }
         else
         {
            if(COMM_SB_BUSY_FLAG)
            {
               if(not EXEC_CB_LED1_DISABLE_DFLT_FLAG)
                  led_1_config(COMM_LED_CONFIG_BUSY_SCRIPT_END);
               COMM_SB_BUSY_FLAG = 0;
            }
         }
         break;
      
      //====================================================
      case COMM_STATE_GEN_MACRO_X_INIT:

         //---------------------------------------
         //--- CHECK MACRO ABORT
         //---------------------------------------
         if(COMM_CB_MACRO_ABORT_FLAG)
         {
            comm_post_event_2b(\
               COMM_TAG_GEN_EVENT_MACRO_ABORT, \
               comm_macro_count_1_W.lo, \
               comm_macro_count_1_W.hi);
            COMM_MACRO_FLAG = 0;
         }

         //---------------------------------------
         //--- CHECK MACRO LOOP COUNT
         //---------------------------------------
         else if((not COMM_SB_MACRO_CONT_FLAG) && \
            (comm_macro_count_1_W._word == comm_macro_count_2_W._word))
         {
            //--- EVENT MESSAGE
            if(COMM_CB_EVENT_MACRO_DONE_FLAG)
            {
               comm_post_event_2b(\
                  COMM_TAG_GEN_EVENT_MACRO_DONE, \
                  comm_macro_count_1_W.lo, \
                  comm_macro_count_1_W.hi);
            }
            COMM_MACRO_FLAG = 0;
         }
         
         //---------------------------------------
         //--- RE-INIT FOR NEXT MACRO EXEC
         //---------------------------------------
         else
         {
            //--- EVENT MESSAGE
            if(COMM_CB_EVENT_MACRO_LOOP_FLAG)
            {
               comm_post_event_2b( \
                  COMM_TAG_GEN_EVENT_MACRO_LOOP, \
                  comm_macro_count_1_W.lo, \
                  comm_macro_count_1_W.hi);
            }
            comm_macro_count_1_W._word++;
            comm_cmnd_macro_count_1 = cbuf3.used;

            //--- EVENT MESSAGE
            if((COMM_CB_EVENT_MACRO65K_FLAG)&&(comm_macro_count_1_W._word==0))
            {
               comm_post_event(COMM_TAG_GEN_EVENT_MACRO65K);
            }

            COMM_PROC_LOOP_FLAG = 1;
         }
         
         comm_state = COMM_STATE_GEN_NEXT;
         break;
            
      //====================================================
      case COMM_STATE_GEN_STALL:
         COMM_SB_BUSY_FLAG = 0;
         break;

      //====================================================
      case COMM_TAG_GEN_LED1:
         if(comm_data_get())
         {
            led_1_config(comm_data);
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //====================================================
      case COMM_TAG_GEN_LED2:
         if(comm_data_get())
         {
            led_2_config(comm_data);
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //====================================================
      case COMM_TAG_GEN_WAIT_1:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               timer0_AB2_W.lo = comm_data;
               COMM_0_FLAG = 1;
            }
         }
         else if(not COMM_1_FLAG)
         {
            if(comm_data_get())
            {
               timer0_AB2_W.hi = comm_data;
               COMM_1_FLAG = 1;
            }
         }
         else
         {
            if(timer0_AB2_W._word)
            {
               COMM_SB_WAIT_FLAG = 1;
            }
            else
            {
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_SB_WAIT_FLAG = 0;
            }
         }
         break;

      //====================================================
      case COMM_TAG_GEN_TIMEOUT_AB0_SET:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               timer0_AB0_W.lo = comm_data;
               COMM_0_FLAG = 1;
            }
         }
         else
         {
            if(comm_data_get())
            {
               timer0_AB0_W.hi = comm_data;
               COMM_TIMEOUT_AB0_FLAG = 1;
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         break;

      //====================================================
      case COMM_TAG_GEN_TIMEOUT_AB1_SET:
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               timer0_AB1_W.lo = comm_data;
               COMM_0_FLAG = 1;
            }
         }
         else
         {
            if(comm_data_get())
            {
               timer0_AB1_W.hi = comm_data;
               COMM_TIMEOUT_AB1_FLAG = 1;
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         break;

      //====================================================
      case COMM_TAG_GEN_TIMEOUT_AB0_RST:
         COMM_TIMEOUT_AB0_FLAG = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_GEN_TIMEOUT_AB1_RST:
         COMM_TIMEOUT_AB1_FLAG = 0;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_GEN_MACRO_CLEAR:
         cbuf3_init();
         comm_state = COMM_STATE_GEN_NEXT;
         COMM_MACRO_FLAG = 0;                 // insurance
         break;
                     
      //====================================================
      case COMM_TAG_GEN_MACRO_DATA:
         if(not COMM_0_FLAG)
         {
            if(cbuf1.used)
            {
               comm_cmnd_count_1 = cbuf1_get();
               COMM_0_FLAG = 1;
            }
         }
         else
         {
            while(cbuf1.used)
            {
               if(comm_cmnd_count_1--)
                  comm_post_cbuf3(cbuf1_get());
               else
               {
                  comm_state = COMM_STATE_GEN_NEXT;
                  break;
               }
            }
         }
         break;

      //====================================================
      case COMM_TAG_GEN_MACRO_DATA_START:
         if(cbuf1.used)
         {
            comm_data = cbuf1_get();
            if(comm_data == COMM_TAG_GEN_MACRO_DATA_END)
               comm_state = COMM_STATE_GEN_NEXT;
            else
               comm_post_cbuf3(comm_data);
         }
         break;

      //====================================================
      case COMM_TAG_GEN_MACRO_RUN:
         if(not COMM_0_FLAG)
         {
            if(cbuf1.used)
            {
               comm_macro_count_1_W.lo = 0;
               comm_macro_count_2_W.lo = cbuf1_get();
               COMM_0_FLAG = 1;
            }
         }
         else
         {
            if(cbuf1.used)
            {
               comm_macro_count_1_W.hi = 0;
               comm_macro_count_2_W.hi = cbuf1_get();
               comm_state = COMM_STATE_GEN_MACRO_X_INIT;
               if(comm_macro_count_2_W._word == 0)
                  COMM_SB_MACRO_CONT_FLAG = 1;
               else
                  COMM_SB_MACRO_CONT_FLAG = 0;
               COMM_MACRO_FLAG = 1;
            }
         }
         break;

      //====================================================
      case COMM_TAG_GEN_END_SCRIPT:
         COMM_SB_END_SCRIPT_FLAG = 1;                    // set status bit
         comm_post_event(COMM_TAG_GEN_EVENT_END_SCRIPT); // post "event"
         COMM_FLUSH_FLAG = 1;                            // force flush of cbuf2
         comm_state = COMM_STATE_GEN_NEXT;               // 
         break;

      //====================================================
      case COMM_TAG_GEN_MARKER_HOST:
         if(comm_data_get())
         {
            comm_post_event_1b(COMM_TAG_GEN_MARKER_DEV, comm_data);
            comm_state = COMM_STATE_GEN_NEXT;
//debug      
            if(comm_data == 0x77)
               COMM_FLUSH_FLAG = 1;                            // force flush of cbuf2
         }
         break;

      //====================================================
      case COMM_TAG_GEN_TIMER_RESET:
         timer0_reg_clear();
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_GEN_FLUSH_CBUF2:
         COMM_FLUSH_FLAG = 1;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      case COMM_TAG_GEN_WAIT_SW_0:
         if(not SW_PIN)
         {
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //====================================================
      case COMM_TAG_GEN_WAIT_SW_1:
         if(SW_PIN)
         {
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //====================================================
      case COMM_TAG_GEN_SIM_SPECIAL_1:
         comm_data = 0x81;
         comm_post_data();        
         comm_data = 0x02;
         comm_post_data();        
         comm_data = 0x03;
         comm_post_data();        
         comm_data = 0x04;
         comm_post_data();        
         comm_data = comm_data_sim & 0x7F;
         comm_post_data();        
         comm_data_sim++;
         comm_state = COMM_STATE_GEN_NEXT;
         break;

      //====================================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;
         
   }

}
                     
//==========================================================
//==========================================================
//--- UTILITIES
//==========================================================
//==========================================================

//==========================================================
//--- comm_data_get()
//    fetch byte from script
//
//    retrieve data from circular buffer(s)
//    if MACRO active, fetch byte from CBUF3
//    if MACRO not active, fetch byte from CBUF1
//    return:
//    0: no data available
//    1: data available in global variable: "comm_data"
//==========================================================
TD_BYTE comm_data_get(void)
{
   //--- DISABLE INTERRUPTS
   COMM_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL1_FLAG = 1;
   }

   if(COMM_MACRO_FLAG)
   {
      if(cbuf3.used)
      {
         comm_data = cbuf3_get();
         cbuf3_put(comm_data);  
         comm_cmnd_macro_count_1--;
         if(COMM_GIEL1_FLAG)
            INTCONbits.GIEL = 1;
         return 1;
      }
   }
   else
   {
      if(cbuf1.used)
      {
         comm_data = cbuf1_get();
         if(COMM_GIEL1_FLAG)
            INTCONbits.GIEL = 1;
         return 1;
      }
   }

   if(COMM_GIEL1_FLAG)
      INTCONbits.GIEL = 1;
   return 0;
   

}

//==========================================================
//--- comm_post_cbuf2()
//    post data byte to CBUF2
//
//    if CBUF2 full, set OVF ERROR in STATUS_BLOCK
//==========================================================
void comm_post_cbuf2(TD_BYTE data_byte)
{
//   if(COMM_CB_EVENT_FLAG)
//   {
      if(cbuf2.unused)
      {
         cbuf2_put(data_byte);
      }
      else
      {
         COMM_SB_CBUF2_OVF_FLAG = 1;
      }
//   }
}
      
//==========================================================
//--- comm_post_cbuf2_time()
//    post data byte to CBUF2 & time stamp
//    if CBUF2 full, set OVF ERROR in STATUS_BLOCK
//==========================================================
void comm_post_cbuf2_time(TD_BYTE data_byte)
{
   //--- POST BYTE
   comm_post_cbuf2(data_byte);

   //--- POST TIME
   if(COMM_CB_EVENT_TIME_FLAG)
      comm_post_event_time();
}
      
//==========================================================
//--- comm_post_cbuf3()
//    post data byte to CBUF3
//
//    if CBUF3 full, set OVF ERROR in STATUS_BLOCK
//==========================================================
void comm_post_cbuf3(TD_BYTE data_byte)
{
   if(cbuf3.unused)
   {
      cbuf3_put(data_byte);
   }
   else
   {
      COMM_SB_CBUF3_OVF_FLAG = 1;
   }
}


//==========================================================
//--- comm_led_busy_data()
//    indicate data transaction using LED
//==========================================================
void comm_led_busy_data(void)
{
   if(not EXEC_CB_LED2_DISABLE_DFLT_FLAG)
   {
      led_2_config(COMM_LED_CONFIG_BUSY_DATA);
   }
}


//==========================================================
//==========================================================


//==========================================================
//--- comm_post_event()
//    post "EVENT" marker (and TIME if enabled)
//==========================================================
void comm_post_event(TD_BYTE b0)
{
         
   //--- DISABLE INTERRUPTS
   COMM_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL1_FLAG = 1;
   }

   //--- POST
   if(COMM_CB_EVENT_FLAG)
   {
      comm_post_cbuf2_time(b0);
   }

   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL1_FLAG)
   {
      INTCONbits.GIEL = 1;
   }
}
      
//==========================================================
//--- comm_post_event_1b()
//    post "EVENT" marker w/ 1 "event-dependant" data byte
//==========================================================
void comm_post_event_1b(TD_BYTE b0, TD_BYTE b1)
{

   //--- DISABLE INTERRUPTS
   COMM_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL1_FLAG = 1;
   }


   //--- POST
   if(COMM_CB_EVENT_FLAG)
   {
      comm_post_cbuf2(b0);
      comm_post_cbuf2_time(b1);
   }
   
   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL1_FLAG)
   {
      INTCONbits.GIEL = 1;
   }
}
      
//==========================================================
//--- comm_post_event_2b()
//    post "EVENT" marker w/ 2 "event-dependant" data bytes
//==========================================================
void comm_post_event_2b(TD_BYTE b0, TD_BYTE b1, TD_BYTE b2)
{
   //--- DISABLE INTERRUPTS
   COMM_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL1_FLAG = 1;
   }

   //--- POST
   if(COMM_CB_EVENT_FLAG)
   {
      comm_post_cbuf2(b0);
      comm_post_cbuf2(b1);
      comm_post_cbuf2_time(b2);
   }

   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL1_FLAG)
   {
      INTCONbits.GIEL = 1;
   }
}
      
//==========================================================
//--- comm_post_data()
//    post event: "DATA"
//==========================================================
void comm_post_data(void)
{
   //--- DISABLE INTERRUPTS
   COMM_GIEL1_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL1_FLAG = 1;
   }

   //--- POST
   comm_post_cbuf2(COMM_TAG_GEN_DATA);
   comm_post_cbuf2_time(comm_data);

   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL1_FLAG)
   {
      INTCONbits.GIEL = 1;
   }
}


//==========================================================
//--- comm_post_event_time()
//    post event: "TIME"
//==========================================================
void comm_post_event_time(void)
{
   //--- DISABLE INTERRUPTS
   COMM_GIEL3_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL3_FLAG = 1;
   }

   comm_post_event_time_get();
   comm_post_cbuf2(COMM_TAG_GEN_EVENT_TIME);
   //comm_post_cbuf2(timer0_reg[0]);
   comm_post_cbuf2(timer0_reg[1]);
   comm_post_cbuf2(timer0_reg[2]);

   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL3_FLAG)
      INTCONbits.GIEL = 1;
}

//==========================================================
//--- comm_post_event_time_get()
//    fetch time stamp, post TIMER_OVF event if necessary
//==========================================================
void comm_post_event_time_get(void)
{
   //--- DISABLE INTERRUPTS
   COMM_GIEL2_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      COMM_GIEL2_FLAG = 1;
   }

   //--- TIMER READ - CHECK OVERFLOW
   timer0_reg_read();
   if(TIMER0_REG_OVF_SYNC_FLAG)
   {
      comm_post_cbuf2(COMM_TAG_GEN_EVENT_TIME_ROLLOVER);
      TIMER0_REG_OVF_SYNC_FLAG = 0;
   }

   //--- RE-ENABLE INTERRUPTS
   if(COMM_GIEL2_FLAG)
      INTCONbits.GIEL = 1;
}     
   
