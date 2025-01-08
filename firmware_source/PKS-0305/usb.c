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
//    Filename:            usb.c
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
//    12-05-07 - ME
//    -- added UCONbits.PKTDIS=0; to EP0,OUT,SETUP per errata
//
//    0001 - 08-01-06 - ME
//    - initial release
//
//==================================================================== 
#include "project.h"
#include "usb.h"
#include "usb_data_constants.h"
#ifdef __DEBUG_TRACE
#include "cbuf.h"
#endif

#pragma udata

//==========================================================
//--- DATA
//==========================================================
TD_BYTE     usb_state;
TD_BYTE     usb_addr;
TD_BYTE_B   usb_flags_B;
TD_BYTE     usb_config;

//--- LOCAL
TD_POINTER usb_src_p;
TD_POINTER usb_dst_p;
TD_BYTE usb_count;
TD_BYTE usb_count_temp;

//--- USB BUFFER DESCRIPTOR TABLE
#pragma udata usb4ram = 0x400
volatile far BDT usb_bdt_ep0_out;    // endpoint: 0, dir: out
volatile far BDT usb_bdt_ep0_in;     // endpoint: 0, dir: in
volatile far BDT usb_bdt_ep1_out;    // endpoint: 1, dir: out
volatile far BDT usb_bdt_ep1_in;     // endpoint: 1, dir: in

//--- USB BUFFERS
#pragma udata usb5ram = 0x500
volatile far USB_BUF_CONTROL usb_buf_ep0_out;
volatile far USB_BUF_CONTROL usb_buf_ep0_in;
volatile far TD_BYTE usb_buf_ep1_out[USB_EP1_SIZE_MAX];
volatile far TD_BYTE usb_buf_ep1_in[USB_EP1_SIZE_MAX];

//----------------------------------------------------------
//--- post "trace" messages to circular buffer: CBUF1
//----------------------------------------------------------

#pragma code _usb_ep0

//==========================================================
//=== UTILITIES
//==========================================================

//================================================
//--- usb_detach_soft()
//
//    overview:
//    execute "soft" detach
//================================================
void usb_detach_soft(void)
{
   usb_module_disable();
}

//================================================
//--- usb_module_disable()
//
//    overview:
//    disable USB control hardware
//================================================
void usb_module_disable(void)
{
   UCON = 0;                  //
   UIE = 0;
   usb_state = USB_STATE_DETACHED;
}

//================================================
//--- usb_init()
//
//    overview:
//    configure/enable USB controller
//    called once from "main"
//================================================
void usb_init(void)
{
   UCONbits.USBEN = 0;        // disable USB
   UCFGbits.UPUEN = 1;        // enable pullups
   UCFGbits.FSEN = 1;         // enable full speed
   UEP0 = _EPHSHK | _EPOUTEN | _EPINEN;
   UEP1 = _EPHSHK | _EPOUTEN | _EPINEN | _EPCONDIS;
   usb_bdt_load();            // load buffer descriptors
   UCONbits.USBEN = 1;        // enable USB
   usb_state = USB_STATE_ATTACHED;
}

//================================================
//--- usb_reset()
//    
//    overview:
//    executed as a result of 
//    USB "RESET" transaction
//================================================
void usb_reset(void)
{
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF5);
#endif
   UIRbits.TRNIF = 0;
   UIRbits.TRNIF = 0;
   UIRbits.TRNIF = 0;
   UIRbits.TRNIF = 0;
   UADDR = 0;
   usb_flags_B._b = 0;
   usb_state = 0;
   usb_bdt_load();
}

//================================================
//--- usb_bdt_load()
//
//    overview:
//    load buffer descriptor table
//    with default values
//================================================
void usb_bdt_load(void)
{
   usb_bdt_ep0_out.status._b  = _BDS_UOWN;
   usb_bdt_ep0_out.count      = USB_EP0_SIZE_MAX;
   usb_bdt_ep0_out.addr       = (TD_BYTE *)&usb_buf_ep0_out;
   usb_bdt_ep0_in.status._b  = 0;
   usb_bdt_ep0_in.count      = USB_EP0_SIZE_MAX;
   usb_bdt_ep0_in.addr       = (TD_BYTE *)&usb_buf_ep0_in;

   usb_bdt_ep1_out.status._b  = _BDS_UOWN;
   usb_bdt_ep1_out.count      = USB_EP1_SIZE_MAX;
   usb_bdt_ep1_out.addr       = usb_buf_ep1_out;
   usb_bdt_ep1_in.status._b  = _BDS_DTS;
   usb_bdt_ep1_in.count      = USB_EP1_SIZE_MAX;
   usb_bdt_ep1_in.addr       = usb_buf_ep1_in;
}

//================================================
//--- usb_isr()
//
//    overview:
//    called from USB interrupt
//    -or-
//    called from main idle loop
//
//    react to interrupt flags
//================================================
void usb_isr(void)
{
   if(not UIR)    // if no flags, return/exit
      return;

   if((UIRbits.SOFIF)&&(UIEbits.SOFIE))
      UIRbits.SOFIF = 0;
   if((UIRbits.IDLEIF)&&(UIEbits.IDLEIE))
      UIRbits.IDLEIF = 0;
   if((UIRbits.ACTVIF)&&(UIEbits.ACTVIE))
      UIRbits.ACTVIF = 0;
   if((UIRbits.UERRIF)&&(UIEbits.UERRIE))
      UIRbits.UERRIF = 0;
   if((UIRbits.STALLIF)&&(UIEbits.STALLIE))
      UIRbits.STALLIF = 0;
   if((UIRbits.URSTIF)&&(UIEbits.URSTIE))
      UIRbits.URSTIF = 0;
   if((UIRbits.TRNIF)&&(UIEbits.TRNIE))
   {
      UIRbits.TRNIF = 0;
   } 
   if(UEIR)
   {
      // count errors
   } 
}

//================================================
//--- usb_svc()
//
//    overview:
//    called from caller's idle loop
//    reacts to interrupt flags
//================================================
void usb_svc(void)
{
   //--- 
   if(UIRbits.SOFIF)
   {
      UIRbits.SOFIF = 0;
   }

   //--- 
   if(UIRbits.IDLEIF)
   {
      UIRbits.IDLEIF = 0;
   }

   //--- 
   if(UIRbits.ACTVIF)
   {
      UIRbits.ACTVIF = 0;
   }

   //--- 
   if(UIRbits.UERRIF)
   {
      UIRbits.UERRIF = 0;
   }

   //--- 
   if(UIRbits.STALLIF)
   {
      UIRbits.STALLIF = 0;
   }
   
   //--- 
   if(UIRbits.URSTIF)
   {
      usb_reset();
      UIRbits.URSTIF = 0;
   }

   //--- 
   if(UIRbits.TRNIF)
   {
      usb_trn();
      UIRbits.TRNIF = 0;
   } 

   //--- 
   if(UEIR)
   {
      // count errors
   } 
   
   //---
   usb_ep1_check();
}

//==========================================================
//=== TRANSACTION PROCESSING
//==========================================================

//================================================
//--- usb_trn()
//
//    overview:
//    process USB transaction interrupt
//================================================
void usb_trn(void)
{
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF0);
   //cbuf1_put(USTAT);
#endif

   //-------------------------
   //--- EP0, OUT
   //-------------------------
   if(USTAT == (_USTAT_EP00 | _USTAT_OUT))
   {

#ifdef __DEBUG_TRACE
      cbuf1_put(0xF3);
      cbuf1_put(usb_bdt_ep0_out.status._b);
#endif

      //--- EP0, OUT, SETUP
      if(usb_bdt_ep0_out.status.pid == _PID_SETUP)
      {
         UCONbits.PKTDIS = 0;
         usb_trn_setup();
      }

      //--- EP0, OUT
      else
         usb_trn_control_out();
      usb_ep0_out_finish();
   }

   //-------------------------
   //--- EP0, IN
   //-------------------------
   else if(USTAT == (_USTAT_EP00 | _USTAT_IN))
      usb_trn_control_in();
}

//================================================
//--- usb_trn_control_out()
//
//    overview:
//    process control "OUTPUT" transactions
//================================================
void usb_trn_control_out(void)
{
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF2);
   cbuf1_put(usb_bdt_ep0_out.count);
#endif
}

//================================================
//--- usb_trn_control_in()
//
//    overview:
//    process control "INPUT" transactions
//================================================
void usb_trn_control_in(void)
{

#ifdef __DEBUG_TRACE
   cbuf1_put(0xF4);
   cbuf1_put(usb_bdt_ep0_in.count);
#endif

   if(usb_bdt_ep0_in.count == 0)
   {
      if(USB_ADDR_1_FLAG)
      {
         USB_ADDR_1_FLAG = 0;
         UADDR = usb_addr;
         usb_state = USB_STATE_ADDRESS;
      }
      USB_IN_BUSY_FLAG = 0;
   }
   else
   {
      if(USB_IN_BUSY_FLAG && usb_count)
         usb_ep0_load_flash();
   }
}

//================================================
//--- usb_trn_setup()
//
//    overview:
//    process "SETUP" transactions
//================================================
void usb_trn_setup(void)
{
   
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF1);
   cbuf1_put(usb_buf_ep0_out.rq_type);
   cbuf1_put(usb_buf_ep0_out.request);
   cbuf1_put(usb_buf_ep0_out.length_W._b0);
#endif

   if((usb_buf_ep0_out.rq_type & _RQTYPE_TR_MASK) == _RQTYPE_TR_STD_DEV)
      usb_trn_setup_std_dev();
   else if((usb_buf_ep0_out.rq_type & _RQTYPE_TR_MASK) == _RQTYPE_TR_STD_INTF)
      usb_trn_setup_std_intf();
   else if((usb_buf_ep0_out.rq_type & _RQTYPE_TR_MASK) == _RQTYPE_TR_CLASS_INTF)
      usb_trn_setup_class_intf();
   else
      usb_ep0_trn_unsupported();
}

//================================================
//--- usb_trn_setup_std_dev()
//
//    overview:
//    process "SETUP" transactions for STD, DEV
//================================================
void usb_trn_setup_std_dev(void)
{
   switch(usb_buf_ep0_out.request)
   {
      case  _RQ_SET_ADDRESS:
         usb_addr = usb_buf_ep0_out.value_W._b0;
         USB_ADDR_1_FLAG = 1;
         usb_ep0_load_0len();
         usb_state = USB_STATE_ADDRESS_PENDING;
         break;

      case  _RQ_GET_DESCRIPTOR:
         usb_trn_setup_std_dev_getdsc();
         break;

      case  _RQ_SET_CONFIGURATION:
         usb_config = usb_buf_ep0_out.value_W._b0;
         usb_ep0_load_0len();
         usb_state = USB_STATE_CONFIGURED;
         USB_READY_FLAG = 1;
         break;
         
      //--- not supported
      case  _RQ_CLEAR_FEATURE:
      case  _RQ_SET_FEATURE:
      case  _RQ_GET_INTERFACE:
      case  _RQ_SET_INTERFACE:
      case  _RQ_SYNC_FRAME:
      case  _RQ_GET_STATUS:
      case  _RQ_SET_DESCRIPTOR:
      case  _RQ_GET_CONFIGURATION:
      default:
         usb_ep0_trn_unsupported();
         break;
   }
}

//================================================
//--- usb_trn_setup_std_intf()
//
//    overview:
//    process "SETUP" transactions for STD, INTF
//================================================
void usb_trn_setup_std_intf(void)
{
   switch(usb_buf_ep0_out.request)
   {
      case  _RQ_GET_DESCRIPTOR:
         usb_trn_setup_std_intf_getdsc();
         break;
         
      //--- not supported
      default:
         usb_ep0_trn_unsupported();
         break;
   }
}

//================================================
//--- usb_trn_setup_class_intf()
//
//    overview:
//    process "SETUP" transactions for CLASS, INTF
//================================================
void usb_trn_setup_class_intf(void)
{
   switch(usb_buf_ep0_out.request)
   {
      case  _RQ_SET_IDLE:
         usb_ep0_load_0len();
         break;
      case  _RQ_GET_REPORT:
         usb_trn_setup_class_intf_getreport();
         break;
      default:
         usb_ep0_trn_unsupported();
         break;
   }
}

//================================================
//--- usb_trn_setup_class_intf_getreport()
//
//    overview:
//    process "SETUP" transactions for CLASS, INTF
//    GET_REPORT
//================================================
void usb_trn_setup_class_intf_getreport(void)
{
   switch(usb_buf_ep0_out.value_W._b1)
   {
      case 0:
      case 1:
      case 2:
      case 3:
      default:
         usb_bdt_ep0_in.count = 2;
         usb_buf_ep0_in._b0 = 0x01;
         usb_buf_ep0_in._b1 = 0x02;
         break;
   }     
}

//================================================
//--- usb_trn_setup_std_dev_getdsc()
//
//    overview:
//    process "SETUP" transactions for STD, DEV
//    GET_DESCRIPTOR
//================================================
void usb_trn_setup_std_dev_getdsc(void)
{
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF6);
   cbuf1_put(usb_buf_ep0_out.value_W._b1);
#endif
   switch(usb_buf_ep0_out.value_W._b1)
   {
      case _DESC_TYPE_DEV:
         usb_count = sizeof(usb_dsc_dev); 
         usb_src_p.rom_b = (rom TD_BYTE *)&usb_dsc_dev;
         usb_ep0_load_flash_init();
         break;
         
      case _DESC_TYPE_CFG:
         usb_count = sizeof(usb_dsc_cfg_01); 
         usb_src_p.rom_b = (rom TD_BYTE *)&usb_dsc_cfg_01;
         usb_ep0_load_flash_init();
         break;
         
      case _DESC_TYPE_RPT:
         usb_count = sizeof(usb_dsc_hid_rpt_01); 
         usb_src_p.rom_b = (rom TD_BYTE *)&usb_dsc_hid_rpt_01;
         usb_ep0_load_flash_init();
         break;
         
      case _DESC_TYPE_STR:
         usb_trn_setup_std_dev_getstring();
         break;
         
         
      //--- not supported
      case _DESC_TYPE_INTF:
      case _DESC_TYPE_EP:
      case _DESC_TYPE_DEVQUAL:
      case _DESC_TYPE_OSC:
      default:     
         usb_ep0_trn_unsupported();
         break;  
   }
}
   
//================================================
//--- usb_trn_setup_std_intf_getdsc()
//
//    overview:
//    process "SETUP" transactions for STD, INTF
//    GET_DESCRIPTOR
//================================================
void usb_trn_setup_std_intf_getdsc(void)
{
   switch(usb_buf_ep0_out.value_W._b1)
   {
      case _DESC_TYPE_RPT:
         usb_count = sizeof(usb_dsc_hid_rpt_01); 
         usb_src_p.rom_b = (rom TD_BYTE *)&usb_dsc_hid_rpt_01;
         usb_ep0_load_flash_init();
         break;

      //--- not supported
      default:     
         usb_ep0_trn_unsupported();
         break;  
   }
}
   
//================================================
//--- usb_trn_setup_std_dev_getstring()
//
//    overview:
//    process "SETUP" transactions for STD, DEV
//    GET_DESCRIPTOR
//================================================
void usb_trn_setup_std_dev_getstring(void)
{
   switch(usb_buf_ep0_out.value_W._b0)
   {
      case 0:
         usb_src_p.rom_w = (rom TD_WORD *)&usb_dsc_str_00;
         usb_count = sizeof(usb_dsc_str_00);
         usb_ep0_load_flash_init();
         break;
         
      case 1:
         usb_src_p.rom_w = (rom TD_WORD *)&usb_dsc_str_01;
         usb_count = sizeof(usb_dsc_str_01);
         usb_ep0_load_flash_init();
         break;
         
      case 2:
         usb_src_p.rom_w = (rom TD_WORD *)&usb_dsc_str_02;
         usb_count = sizeof(usb_dsc_str_02);
         usb_ep0_load_flash_init();
         break;
         
      //--- not supported
      default:
         usb_ep0_trn_unsupported();
         break;
   }   
}

//==========================================================
//=== UTILITIES
//==========================================================

//================================================
//--- usb_ep0_load_flash_init()
//
//    overview:
//    setup to xmit FLASH data to EP0,IN buffer
//================================================
void usb_ep0_load_flash_init()
{
   USB_IN_BUSY_FLAG = 1;
   usb_bdt_ep0_in.status._b = _BDS_DTSEN;
   usb_ep0_load_flash();   
}   

//================================================
//--- usb_ep0_load_flash()
//
//    overview:
//    xmit FLASH data to EP0,IN buffer
//================================================
void usb_ep0_load_flash(void)
{
   if(usb_count > usb_buf_ep0_out.length_W._b0)
      usb_count = usb_buf_ep0_out.length_W._b0;

   usb_count_temp = usb_count;
   if(usb_count_temp > USB_EP0_SIZE_MAX)
      usb_count_temp = USB_EP0_SIZE_MAX;
   
   usb_count -= usb_count_temp;
      
   usb_bdt_ep0_in.count = usb_count_temp;

   usb_dst_p.ram_b = (TD_BYTE *)&usb_buf_ep0_in;
   
   while(usb_count_temp--)
      *usb_dst_p.ram_b++ = *usb_src_p.rom_b++;

   usb_ep0_in_finish();
}

   
//================================================
//--- usb_ep0_trn_unsupported()
//
//    overview:
//    EP0 transaction is not supported, set STALL
//================================================
void usb_ep0_trn_unsupported(void)
{
   UEP0bits.EPSTALL = 1;
}

//================================================
//--- usb_ep0_load_0len
//
//    overview:
//    setup/enable ZERO-LENGTH INPUT transaction
//================================================
void usb_ep0_load_0len()
{
   usb_bdt_ep0_in.count = 0;
   usb_bdt_ep0_in.status._b = _BDS_DTSEN;
   usb_ep0_in_finish();
}

//================================================
//--- usb_ep0_in_finish()
//
//    overview:
//    enable USB, EP0 INPUT transaction
//================================================
void usb_ep0_in_finish(void)
{
   usb_bdt_ep0_in.status._b = \
      ((usb_bdt_ep0_in.status._b & _BDS_DTS) ^ _BDS_DTS) | _BDS_UOWN | _BDS_DTSEN; 
#ifdef __DEBUG_TRACE
   cbuf1_put(0xF7);
   cbuf1_put(usb_bdt_ep0_in.count);
#endif  
}   

//================================================
//--- usb_ep0_out_finish
//
//    overview:
//    reset buffer descriptor for ep0 out
//================================================
void usb_ep0_out_finish(void)
{
   UCONbits.PKTDIS = 0;
   usb_bdt_ep0_out.count = USB_EP0_SIZE_MAX;
   usb_bdt_ep0_out.status._b = _BDS_UOWN;
}

//==========================================================
//=== EP1 (DATA I/O) UTILS
//==========================================================
#pragma code _usb_ep1

//================================================
//--- usb_ep1_check
//
//    overview:
//    prepare flags for calling routine(s)
//
//    the calling routine execute usb_ep1_check()"
//    before relying on buffer ready/not-ready flags
//================================================
void usb_ep1_check(void)
{
   if(USB_READY_FLAG)
   {
      USB_RCVBUF_READY_FLAG = not usb_bdt_ep1_out.status.uown;
      USB_XMTBUF_READY_FLAG = not usb_bdt_ep1_in.status.uown;
   }
   else
   {
      USB_RCVBUF_READY_FLAG = 0;
      USB_XMTBUF_READY_FLAG = 0;
   }
}

//================================================
//--- usb_ep1_in_finish
//
//    overview:
//    reset buffer descriptor: ep1, in
//    (data is ready, make available to the host)
//================================================
void usb_ep1_in_finish(void)
{
   usb_bdt_ep1_in.status._b = \
      ((usb_bdt_ep1_in.status._b & _BDS_DTS) ^ _BDS_DTS) | _BDS_UOWN;
   usb_ep1_check();
}
   

//================================================
//--- usb_ep1_out_finish
//
//    overview:
//    reset buffer descriptor: ep1, out
//    (data rcv'd from host has been processed - rls buffer)
//================================================
void usb_ep1_out_finish(void)
{
   usb_bdt_ep1_out.status._b = \
      ((usb_bdt_ep1_out.status._b & _BDS_DTS) ^ _BDS_DTS) | _BDS_UOWN;
   usb_ep1_check();
}

//================================================
//--- usb_ep1_out_get
//
//    overview:
//    block waiting for rcv buffer
//    move receive buffer to destination
//    release receive buffer
//================================================
void usb_ep1_out_get(TD_BYTE *dst_p)
{
   usb_src_p.ram_b = USB_RCVBUF_POINTER;
   usb_count = USB_RCVBUF_SIZE;
   while(usb_count--)
      *dst_p++ = *usb_src_p.ram_b++;
   USB_RCVBUF_RELEASE_FUNC();
}

//================================================
//--- usb_ep1_in_put
//
//    overview:
//    block waiting for xmt buffer avail
//    move caller data to xmt buffer
//    release/send xmt buffer
//================================================
void usb_ep1_in_put(TD_BYTE *src_p)
{
   usb_dst_p.ram_b = USB_XMTBUF_POINTER;
   usb_count = USB_XMTBUF_SIZE;
   while(usb_count--)
      *usb_dst_p.ram_b++ = *src_p++;
   USB_XMTBUF_RELEASE_FUNC();
}

      
