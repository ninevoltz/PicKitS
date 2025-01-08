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
//    Filename:            exec.c
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
//--- EXEC
//
//    "executive" control of device processes
//    provides supervision of USB & COMM controller(s)
//    primary operations:
//    - receive/process incoming USB data blocks
//    --- read/write circular buffers
//    --- execute "immediate" commands
//    --- automatically "flush" comm controller buffer(s)
//        sending data back to HOST via USB
//    - build outgoing USB data blocks
//    --- device status block
//    --- data from comm controller
//
//    init:
//       read CONTROL_BLOCK from EE
//       if EE checksum bad, 
//          load CONTROL_BLOCK hard-coded "default" 
//
//==========================================================
//--- exec_init()
//    initialize exec module
//
//--- exec_svc()
//    exec service - USB I/O
//
//--- exec_dflush_time_set()
//    set cbuf2 flush timer (from control block)
//
//--- exec_cmnd_proc()
//    process "immediate" command
//
//--- exec_cb_load()
//    load CONTROL_BLOCK from EE or DEFAULTs
//
//--- exec_cb_load_dflt()
//    load CONTROL_BLOCK from DEFAULTs
//
//--- exec_cb_ee_read()
//    read CONTROL_BLOCK from EE
//
//--- exec_cb_ee_write()
//    write CONTROL_BLOCK to EE
//
//--- exec_packet_proc()
//    process USB packet
//
//--- exec_data_get()
//    get data byte from USB input buffer
//
//--- exec_data_put()
//    put data byte into USB output buffer
//
//--- exec_status_packet()
//    build status packet in USB output buffer
//
//--- exec_packet_prep()
//    prep varbs for building USB output buffer/packet
//
//--- exec_packet_finish()
//    complete then transmit USB output buffer/packet
//
//--- exec_packet_cbuf2()
//    load USB output packet/buffer with cbuf2 data
//
//==================================================================== 
//--- HISTORY
//==================================================================== 
//
//    0002 - 03-01-08 - ME
//    -- added 'immediate command' EXEC_CMND_COMM_CLEAR
//    -- added EXEC tags to clear/reset CBUF1,2,3
//
//    12-05-07 - ME
//    -- added call to vsrc_init() to exec_init()
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 

#include "project.h"
#include "exec.h"
#include "comm_gen.h"
#include "cbuf.h"
#include "led2.h"
#include "timer0.h"
#include "ee_util.h"
#include "usb.h"
#include "vsrc.h"

//------------------------------------------------
//--- DEFINITIONS / DECLARATIONS
//------------------------------------------------
#pragma udata
TD_BYTE_B exec_flags;
TD_BYTE *exec_data_p;
TD_BYTE exec_data_count, exec_count;
TD_BYTE exec_tag, exec_data;
TD_BYTE_B exec_control_B[24];
TD_BYTE_B exec_status_B[20];
TD_BYTE exec_status_save_1;
TD_BYTE exec_status_save_2;
TD_BYTE exec_status_save_3;
TD_BYTE exec_debug_1;
TD_BYTE exec_debug_2;
TD_BYTE exec_debug_3;
TD_BYTE exec_status_id;

#pragma romdata exec_constants

#ifndef __DEBUG_EXEC_CBLOCK

//--- DEFAULT CONFIGURATION
const rom TD_BYTE exec_cb_dflt[EXEC_CB_LENGTH] = {\
   0xC0, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, \
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//--- DEBUG CONFIGURATION
#else
   const rom TD_BYTE exec_cb_dflt[EXEC_CB_LENGTH] = { \
   0xC0, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, \
   0x04, 0x21, 0xFF, 0x00, 0x80, 0x50, 0x00, 0x00, \
   0x02, 0xFF, 0x03, 0x00, 0x22, 0x11, 0x08, 0x02 };
#endif

//==========================================================

#pragma code

//==========================================================
//--- exec_init()
//    initialize exec module
//==========================================================
void exec_init(void)
{
   exec_flags._b = 0;   // clear op flags
   cbuf1_init();        // clear buffers
   cbuf2_init();
   cbuf3_init();
   comm_init();         // init comm controller
   vsrc_init();

   //-----------------------------------
   //--- clear status block
   //-----------------------------------
   for(exec_count = EXEC_SB_REG_ALL_FIRST; exec_count <= EXEC_SB_REG_ALL_LAST; exec_count++)
      exec_status_B[exec_count]._byte = 0;
   
   //-----------------------------------
   //--- load control block
   //-----------------------------------
   //debug
   //exec_cb_load();
   exec_cb_load_dflt();
   EXEC_CB_NEW_FLAG = 1;

   //-----------------------------------
   //--- enable pullup on switch
   //-----------------------------------
   INTCON2bits.NOT_RBPU = 1;
   SW_PIN_DIR = 1;
}

//==========================================================
//--- exec_svc()
//    exec service - USB I/O
//
//    check/process incoming USB data
//    check cbuf2 flush criteria
//    send cbuf2 data via USB (if flush criteria met)
//    send status packet via USB (if requested)
//==========================================================
void exec_svc(void)
{

//debug
//   if(USB_XMTBUF_READY_FLAG)
//   if(USB_RCVBUF_READY_FLAG)
//      debug_event_set();
//   else
//      debug_event_rst();

   //-----------------------------------
   //--- SWITCH STATE
   //-----------------------------------
   EXEC_SB_SWITCH_FLAG = SW_PIN;
         
   //-----------------------------------
   //--- EXEC SWITCH TEST
   //    use LEDs to reflect switch position
   //-----------------------------------
   if(EXEC_CB_SWITCH_TEST_FLAG)
   {
      if(SW_PIN)
      {
         if(not EXEC_SWTEST_FLAG)
         {
            led_1_config(EXEC_LED_CONFIG_SWTEST_OFF);
            led_2_config(EXEC_LED_CONFIG_SWTEST_ON);
            EXEC_SWTEST_FLAG = 1;
         }
      }
      else
      {
         if(EXEC_SWTEST_FLAG)
         {
            led_1_config(EXEC_LED_CONFIG_SWTEST_ON);
            led_2_config(EXEC_LED_CONFIG_SWTEST_OFF);
            EXEC_SWTEST_FLAG = 0;
         }
      }
      EXEC_SB_SWITCH_TEST_FLAG = 1;
   }
   else
   {
      if(EXEC_SB_SWITCH_TEST_FLAG)
      {
         led_1_config(EXEC_LED_CONFIG_SWTEST_OFF);
         led_2_config(EXEC_LED_CONFIG_SWTEST_OFF);
         EXEC_SB_SWITCH_TEST_FLAG = 0;
      }
   }

   //-----------------------------------
   //--- USB buffer full/ready ?
   //    process incoming USB data
   //-----------------------------------
   if(USB_RCVBUF_READY_FLAG)
   {
      exec_data_count = USB_RCVBUF_COUNT_REG;
      exec_data_p = USB_RCVBUF_POINTER;
      exec_packet_proc();
      USB_RCVBUF_RELEASE_FUNC();
   }

   //-----------------------------------
   //--- CBUF2 flush: comm on demand
   //-----------------------------------
   if(COMM_FLUSH_FLAG)
   {
      COMM_FLUSH_FLAG = 0;
      if(cbuf2.used)
         EXEC_DFLUSH_FLAG = 1;
   }
   
   //-----------------------------------
   //--- CBUF2 flush: data threshold ?
   //-----------------------------------
   if(EXEC_CB_DFLUSH_THRESHOLD_FLAG)
   {
      if(cbuf2.used > exec_control_B[3]._byte)
         EXEC_DFLUSH_FLAG = 1;
   }
   
   //-----------------------------------
   //--- CBUF2 flush: time ?
   //-----------------------------------
   if(EXEC_CB_DFLUSH_TIME_FLAG)
   {
      if(not timer0_A4)
      {
         exec_dflush_time_set();
         if(cbuf2.used)
            EXEC_DFLUSH_FLAG = 1;
      }
   }
   
//debug
//   if(EXEC_RQ_STATUS_PACKET_FLAG && EXEC_DFLUSH_FLAG)
//      debug_event_set();
//   else
//      debug_event_rst();

   //-----------------------------------
   //--- send: status packet
   //    don't send until new CB
   //    has been recognized/processed
   //-----------------------------------
   if(not EXEC_CB_NEW_FLAG)
   {
      if((EXEC_RQ_STATUS_PACKET_FLAG) || (EXEC_RQ_STATUS_PACKET_ID_FLAG))
      {
         if(USB_XMTBUF_READY_FLAG)
         {
            exec_packet_prep();

            if(EXEC_RQ_STATUS_PACKET_FLAG)
            {
               EXEC_RQ_STATUS_PACKET_FLAG = 0;
               exec_packet_status(EXEC_STATUS_PACKET_ID);
            }
            else
            {
               EXEC_RQ_STATUS_PACKET_ID_FLAG = 0;
               exec_packet_status(exec_status_id);
            }
            exec_packet_finish();
         }
      }
   }
            
   //-----------------------------------
   //--- send: CBUF2 data
   //-----------------------------------
   if(EXEC_DFLUSH_FLAG)
   {
      if(USB_XMTBUF_READY_FLAG)
      {
//debug
//         exec_dflush_time_set();

         EXEC_DFLUSH_FLAG = 0;
         if(cbuf2.used)
         {
            exec_packet_prep();
            exec_packet_cbuf2();
            exec_packet_finish();
         }
      }
   }

//debug
//   if(USB_XMTBUF_READY_FLAG)
//   if(USB_RCVBUF_READY_FLAG)
//      debug_event_set();
//   else
//      debug_event_rst();

}


//==========================================================
//--- exec_dflush_time_set()
//    set cbuf2 flush timer (from control block)
//    catch value==0, substitute max time i.e. 255
//==========================================================
void exec_dflush_time_set(void)
{
   timer0_A4 = EXEC_CB_DFLUSH_TIME_REG;
   if(timer0_A4 == 0)
      timer0_A4 = 255;
}



//==========================================================
//--- exec_cmnd_proc()
//    process "immediate" command
//==========================================================
void exec_cmnd_proc(TD_BYTE cmnd)
{
   switch(cmnd)
   {

      //==========================================
      case EXEC_CMND_INIT:
         exec_init();
         break;

      //==========================================
      case EXEC_CMND_COMM_INIT_MODE:
         comm_init_mode(exec_control_B[8]._byte);
         break;
         
      //==========================================
      case EXEC_CMND_RQ_STATUS_PACKET:
         EXEC_RQ_STATUS_PACKET_FLAG = 1;
         break;

      //==========================================
      case EXEC_CMND_CB_EE_WRITE:
         exec_cb_ee_write();
         break;

      //==========================================
      case EXEC_CMND_CB_EE_READ:
         exec_cb_load();
         break;

      //==========================================
      case EXEC_CMND_CBUF2_FLUSH:
         EXEC_DFLUSH_FLAG = 1;
         break;

      //==========================================
      case EXEC_CMND_COMM_INIT:
         comm_init();
         break;

      //==========================================
      case EXEC_CMND_COMM_CLEAR:
         comm_clear();
         break;

      //==========================================
      case 9:
         Nop();
         comm_init_mode(exec_control_B[8]._byte);
//         comm_init();
         break;
   }
}

//==========================================================
//--- exec_cb_load()
//    load CONTROL_BLOCK from EE or DEFAULTs
//==========================================================
void exec_cb_load(void)
{
   if(exec_cb_ee_read())
   {
      exec_cb_load_dflt();
      EXEC_SB_CB_DFLT_FLAG = 1;
   }
   else
      EXEC_SB_CB_DFLT_FLAG = 0;
}

//==========================================================
//--- exec_cb_load_dflt()
//    load CONTROL_BLOCK from DEFAULTs
//==========================================================
void exec_cb_load_dflt(void)
{
   exec_count = 0;
   while(exec_count < EXEC_CB_LENGTH)
   {
      exec_control_B[exec_count]._byte = exec_cb_dflt[exec_count];
      exec_count++;
   }
}

//==========================================================
//--- exec_cb_ee_write()
//    write CONTROL_BLOCK to EE
//    checksum value = 2's complement of calc checksum
//==========================================================
void exec_cb_ee_write(void)
{
   exec_count = 0;
   exec_data = EXEC_CB_CHKSUM_SEED;
   EEADR = EE_ADDR_CB_START;
   while(exec_count < EXEC_CB_LENGTH)
   {
      EEDATA = exec_control_B[exec_count++]._byte;
      exec_data += EEDATA;
      ee_write_na();
   }
   EEDATA = ~exec_data + 1;
   ee_write_na();
}

//==========================================================
//--- exec_cb_ee_read()
//    read CONTROL_BLOCK from EE
//    return:
//       ==0:  checksum OK
//       <>0:  checksum bad
//==========================================================
TD_BYTE exec_cb_ee_read(void)
{
   exec_data = EXEC_CB_CHKSUM_SEED;
   exec_count = 0;
   while(exec_count < EXEC_CB_LENGTH)
   {
      exec_control_B[exec_count]._byte = \
         ee_read(EE_ADDR_CB_START + exec_count);
      exec_data += EEDATA;
      exec_count++;
   }
   exec_data += ee_read(EE_ADDR_CB_START + exec_count);
   return(exec_data);
}

//==========================================================
//--- exec_packet_proc()
//    process USB packet
//==========================================================
void exec_packet_proc(void)
{
   EXEC_SB_DATA_ERR_FLAG = 0;
   EXEC_PROC_LOOP_FLAG = 1;

   while(EXEC_PROC_LOOP_FLAG && (not EXEC_SB_DATA_ERR_FLAG))
   {
      
      //------------------------------------------
      //--- fetch TAG
      //------------------------------------------
      if(not exec_data_get())
      {
         EXEC_PROC_LOOP_FLAG = 0;
         break;
      }
      
      //------------------------------------------
      //--- process TAG
      //------------------------------------------
      switch(exec_data)
      {

         //=======================================
         //--- EOD TAG
         //=======================================
         case EXEC_TAG_EOD:
            EXEC_PROC_LOOP_FLAG = 0;
            break;
            
         //=======================================
         //--- Immediate Command
         //=======================================
         case EXEC_TAG_COMMAND:
            if(exec_data_get())
            {
               exec_cmnd_proc(exec_data);
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }
            break;
            
         //=======================================
         //--- write CONTROL_BLOCK
         //=======================================
         case EXEC_TAG_CB_WRITE:
            EXEC_CB_NEW_FLAG = 1;
            exec_count = 0;
            while(exec_count < EXEC_CB_LENGTH)
            {
               if(exec_data_get())
               {
                  exec_control_B[exec_count]._byte = exec_data;
                  exec_count++;
               }
               else
               {
                  EXEC_SB_DATA_ERR_FLAG = 1;
                  EXEC_CB_NEW_FLAG = 0;
                  break;
               }
            }
            break;
            
         //=======================================
         //--- write: CBUF1
         //=======================================
         case EXEC_TAG_CBUF1_WRITE:
            if(exec_data_get())
            {
               exec_count = exec_data;

               //debug
               exec_debug_1 = exec_count;
               exec_debug_2 = cbuf1.unused;

               while(exec_count--)
               {
                  if(exec_data_get())
                  {
                     exec_cbuf1_put();
                  }
                  else
                  {
                     EXEC_SB_DATA_ERR_FLAG = 1;
                  }  // end data byte
               }  // end while
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }  // end data get - byte count
            break;

         //=======================================
         //--- write: CBUF2
         //=======================================
         case EXEC_TAG_CBUF2_WRITE:
            if(exec_data_get())
            {
               exec_count = exec_data;
               while(exec_count--)
               {
                  if(exec_data_get())
                  {
                     if(cbuf2.unused)
                     {
                        cbuf2_put(exec_data);
                     }
                     else
                     {
                        EXEC_SB_CBUF2_OVF_FLAG = 1;
                        EXEC_SB_DATA_ERR_FLAG = 1;
                     }
                  }
                  else
                  {
                     EXEC_SB_DATA_ERR_FLAG = 1;
                  }  // end data byte
               }  // end while
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }  // end data get - byte count
            break;

         //=======================================
         //--- write: CBUF3
         //=======================================
         case EXEC_TAG_CBUF3_WRITE:
            if(exec_data_get())
            {
               exec_count = exec_data;
               while(exec_count--)
               {
                  if(exec_data_get())
                  {
                     if(cbuf3.unused)
                     {
                        cbuf3_put(exec_data);
                     }
                     else
                     {
                        EXEC_SB_CBUF3_OVF_FLAG = 1;
                        EXEC_SB_DATA_ERR_FLAG = 1;
                     }
                  }
                  else
                  {
                     EXEC_SB_DATA_ERR_FLAG = 1;
                  }  // end data byte
               }  // end while
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }  // end data get - byte count
            break;

         //=======================================
         //--- write: LED1 configuration
         //=======================================
         case EXEC_TAG_LED1_CONFIG:
            if(exec_data_get())
            {
               led_1_config(exec_data);
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }
            break;

         //=======================================
         //--- write: LED2 configuration
         //=======================================
         case EXEC_TAG_LED2_CONFIG:
            if(exec_data_get())
            {
               led_2_config(exec_data);
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }
            break;

         //=======================================
         //--- CBUF1 RESET
         //=======================================
         case EXEC_TAG_CBUF1_RESET:
            cbuf1_init();
            break;

         //=======================================
         //--- CBUF2 RESET
         //=======================================
         case EXEC_TAG_CBUF2_RESET:
            cbuf2_init();
            break;

         //=======================================
         //--- CBUF3 RESET
         //=======================================
         case EXEC_TAG_CBUF3_RESET:
            cbuf3_init();
            break;

         //=======================================
         //--- boot
         //=======================================
         case EXEC_TAG_BOOT_B:
         case EXEC_TAG_BOOT_b:
            usb_detach_soft();
            EECON1 = 0;
            EECON1bits.EEPGD = 1;
            EECON1bits.WREN = 1;
            *((rom far char *) BOOT_MARKER_ADDR) = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            timer0_A4 = EXEC_REBOOT_DLY_TA;
            while(timer0_A4)
            {
               timer0_svc();
            }
            Reset();
            break;
            
         //=======================================
         //--- write: STATUS PACKET RQ
         //=======================================
         case EXEC_TAG_EXEC_STATUS_RQ:
            if(exec_data_get())
            {
               exec_status_id = exec_data;
               EXEC_RQ_STATUS_PACKET_ID_FLAG = 1;
            }
            else
            {
               EXEC_SB_DATA_ERR_FLAG = 1;
            }
            break;

         //=======================================
         //--- write: RESET
         //=======================================
         case EXEC_TAG_RESET:
            usb_detach_soft();
            Reset();
            break;
            
         //=======================================
         //--- ERROR
         //=======================================
         default:
            EXEC_SB_DATA_ERR_FLAG = 1;
            break;
      }
   }      
}


//==========================================================
//--- exec_packet_prep()
//    prep varbs for building USB output buffer/packet
//==========================================================
void exec_packet_prep(void)
{
   exec_data_p = USB_XMTBUF_POINTER;
   exec_data_count = 0;
}


//==========================================================
//--- exec_packet_prep()
//    prep varbs for building USB output buffer/packet
//--- exec_packet_finish()
//    complete then transmit USB output buffer/packet
//--- exec_packet_cbuf2()
//    load USB output packet/buffer with cbuf2 data
//==========================================================
void exec_packet_finish(void)
{
   if(exec_data_count < USB_XMTBUF_COUNT_MAX )
      exec_data_put(EXEC_TAG_EOD_OUT);
   USB_XMTBUF_RELEASE_FUNC();
}


//==========================================================
//--- exec_packet_cbuf2()
//    load USB output packet/buffer with cbuf2 data
//==========================================================
void exec_packet_cbuf2(void)
{
   exec_count = EXEC_DATA_LENGTH_MAX - exec_data_count - 2;
   if(exec_count > cbuf2.used)
      exec_count = cbuf2.used;
   exec_data_put(EXEC_TAG_CBUF2_DATA);
   exec_data_put(exec_count);
   while(exec_count--)
   {
      exec_data_put(cbuf2_get());
   }
}


//==========================================================
//--- exec_status_packet()
//    build status packet in USB output buffer
//==========================================================
void exec_packet_status(TD_BYTE id)
{
   exec_data_put(EXEC_TAG_PACKET_ID);
   exec_data_put(id);
   exec_data_put(EXEC_TAG_FIRMWARE_VERSION);
   exec_data_put(EXEC_FIRMWARE_VERSION_MINOR);
   exec_data_put(EXEC_FIRMWARE_VERSION_MAJOR);
   exec_data_put(EXEC_TAG_CB_DATA);
   exec_count = 0;
   while(exec_count < EXEC_CB_LENGTH)
   {
      exec_data_put(exec_control_B[exec_count]._byte);
      exec_count++;
   }
   exec_data_put(EXEC_TAG_SB_DATA);
   exec_count = 0;
   while(exec_count < EXEC_SB_LENGTH)
   {
      exec_data_put(exec_status_B[exec_count]._byte);
      exec_count++;
   }
   exec_data_put(EXEC_TAG_CBUF_STATUS);

   EXEC_GIEL_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      EXEC_GIEL_FLAG = 1;
   }

   exec_data_put(cbuf1.used);
   exec_data_put(cbuf1.unused);
   exec_data_put(cbuf2.used);
   exec_data_put(cbuf2.unused);
   exec_data_put(cbuf3.used);
   exec_data_put(cbuf3.unused);
   
   //debug
   exec_debug_3 = cbuf1.unused;

   if(EXEC_GIEL_FLAG)
      INTCONbits.GIEL = 1;
}
   

//==========================================================
//--- exec_data_get()
//    get data byte from USB input buffer
//==========================================================
TD_BYTE exec_data_get(void)
{
   if(exec_data_count--)
   {
      exec_data = *exec_data_p++;
      return 1;
   }
   return 0;      
}      


//==========================================================
//--- exec_data_put()
//    put data byte into USB output buffer
//==========================================================
void exec_data_put(TD_BYTE b0)
{
   exec_data_p[exec_data_count++] = b0;
}      


//==========================================================
//--- exec_data_put()
//    put data byte into USB output buffer
//==========================================================
void exec_cbuf1_put(void)
{
   EXEC_GIEL_FLAG = 0;
   if(INTCONbits.GIEL)
   {
      while(INTCONbits.GIEL)
      {
         INTCONbits.GIEL = 0;
      }
      EXEC_GIEL_FLAG = 1;
   }

   if(cbuf1.unused)
   {
      cbuf1_put(exec_data);
   }
   else
   {
      EXEC_SB_CBUF1_OVF_FLAG = 1;
      EXEC_SB_DATA_ERR_FLAG = 1;
   }

   if(EXEC_GIEL_FLAG)
      INTCONbits.GIEL = 1;
}