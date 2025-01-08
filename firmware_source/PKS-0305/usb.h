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
//    Filename:            usb.h
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

#ifndef __USB_H
#define __USB_H

//------------------------------------------------
//--- USB DATA I/O DEFINES FOR CALLER
//------------------------------------------------
#define USB_RCVBUF_READY_FLAG    usb_flags_B.bit6
#define USB_RCVBUF_COUNT_REG     usb_bdt_ep1_out.count
#define USB_RCVBUF_COUNT_MAX     64
#define USB_RCVBUF_SIZE          64
#define USB_RCVBUF_POINTER       usb_buf_ep1_out
#define USB_RCVBUF_RELEASE_FUNC  usb_ep1_out_finish
#define USB_RCVBUF_GET_FUNC      usb_ep1_out_get

#define USB_XMTBUF_READY_FLAG    usb_flags_B.bit7
#define USB_XMTBUF_COUNT_REG     usb_bdt_ep1_in.count
#define USB_XMTBUF_COUNT_MAX     64
#define USB_XMTBUF_SIZE          64
#define USB_XMTBUF_POINTER       usb_buf_ep1_in
#define USB_XMTBUF_RELEASE_FUNC  usb_ep1_in_finish
#define USB_XMTBUF_PUT_FUNC      usb_ep1_in_put

//------------------------------------------------
//---
//------------------------------------------------
#define  USB_EP0_SIZE_MAX     64
#define  USB_EP1_SIZE_MAX     64

//------------------------------------------------
//--- USB H/W REGISTER: UEPx
//------------------------------------------------
#define  _EPSTALL    0x01
#define  _EPINEN     0x02
#define  _EPOUTEN    0x04
#define  _EPCONDIS   0x08
#define  _EPHSHK     0x10

//------------------------------------------------
//--- LOCAL FLAGS
//------------------------------------------------
#define  USB_ADDR_1_FLAG         usb_flags_B.bit0
#define  USB_ADDR_2_FLAG         usb_flags_B.bit1
#define  USB_READY_FLAG          usb_flags_B.bit2
#define  USB_IN_BUSY_FLAG        usb_flags_B.bit3
//#define  USB_RCVBUF_READY_FLAG   usb_flags_B.bit6
//#define  USB_XMTBUF_READY_FLAG   usb_flags_B.bit7

//------------------------------------------------
//--- USB H/W REGISTER: USTAT
//------------------------------------------------
#define  _USTAT_EP00    0x00<<3
#define  _USTAT_EP01    0x01<<3
#define  _USTAT_IN      0x01<<2
#define  _USTAT_OUT     0x00<<2

#define  _PID_SETUP     0x0D
#define  _PID_OUT       0x01
#define  _PID_IN        0x09

//------------------------------------------------
//--- USB INTERFACE: STATE
//------------------------------------------------
#define  USB_STATE_DETACHED            0
#define  USB_STATE_ATTACHED            1
#define  USB_STATE_POWERED             2
#define  USB_STATE_DEFAULT             3
#define  USB_STATE_ADDRESS_PENDING     4
#define  USB_STATE_ADDRESS             5
#define  USB_STATE_CONFIGURED          6

//------------------------------------------------
//--- BUFFER DESCRIPTOR: STATUS: MASKs
//------------------------------------------------
#define  _BDS_BSTALL    0x04     //Buffer Stall enable
#define  _BDS_DTSEN     0x08     //Data Toggle Synch enable
#define  _BDS_INCDIS    0x10     //Address increment disable
#define  _BDS_KEN       0x20     //SIE keeps buff descriptors enable
#define  _BDS_DAT0      0x00     //DATA0 packet expected next
#define  _BDS_DAT1      0x40     //DATA1 packet expected next
#define  _BDS_DTS       0x40
#define  _BDS_USIE      0x80     //SIE owns buffer
#define  _BDS_UOWN      0x80
#define  _BDS_UCPU      0x00     //CPU owns buffer

//------------------------------------------------
//--- BUFFER DESCRIPTOR: STATUS: TYPEDEF
//------------------------------------------------
typedef union _BD_STAT
{
   TD_BYTE _b;
   struct{
      unsigned bc8:1;
      unsigned bc9:1;
      unsigned bstall:1;    // Buffer Stall Enable
      unsigned dtsen:1;     // Data Toggle Synch Enable
      unsigned incdis:1;    // Address Increment Disable
      unsigned ken:1;       // BD Keep Enable
      unsigned dts:1;       // Data Toggle Synch Value
      unsigned uown:1;      // USB Ownership
   };    
   struct{
      unsigned bc8:1;
      unsigned bc9:1;
      unsigned pid0:1;      // packet ID
      unsigned pid1:1;      // packet ID
      unsigned pid2:1;      // packet ID
      unsigned pid3:1;      // packet ID
      unsigned :1;
      unsigned uown:1;      // ownership
   };
   struct{
      unsigned :2;
      unsigned pid:4;       //Packet Identifier
      unsigned :2;
   };
} TD_BD_STAT;

//------------------------------------------------
//--- BUFFER DESCRIPTOR: TYPEDEF
//------------------------------------------------
typedef union _BDT
{
   struct
   {
      TD_BD_STAT status;    // status
      TD_BYTE count;        // byte count
      TD_BYTE addr_L;       // buffer address - LSB
      TD_BYTE addr_H;       // buffer address - MSB
   };
   struct
   {
      unsigned :8;
      unsigned :8;
      TD_BYTE* addr;        // Buffer Address
   };
} BDT, TD_BDT;

//------------------------------------------------
//--- USB BUFFER: CONTROL: TYPEDEF
//------------------------------------------------
typedef union _USB_BUF_CONTROL
{
   struct
   {
      TD_BYTE   _b[USB_EP0_SIZE_MAX];
   };
   struct
   {
      TD_BYTE    _b0;
      TD_BYTE    _b1;
      TD_BYTE    _b2;
      TD_BYTE    _b3;
      TD_BYTE    _b4;
      TD_BYTE    _b5;
      TD_BYTE    _b6;
      TD_BYTE    _b7;
   };
   struct
   {
      TD_WORD    _w0;
      TD_WORD    _w1;
      TD_WORD    _w2;
      TD_WORD    _w3;
   };
   struct
   {
      TD_BYTE   rq_type;
      TD_BYTE   request;    
      TD_WORD   value_w;
      TD_WORD   index_w;
      TD_WORD   length_w;
   };
   struct
   {
      unsigned  :8;
      unsigned  :8;
      TD_WORD_W value_W;
      TD_WORD_W index_W;
      TD_WORD_W length_W;
   };
} USB_BUF_CONTROL;

//======================================
//--- USB SETUP REQUEST: MASKs
//======================================

//--- REQUEST: TYPE
#define  _RQTYPE_DIR_MASK        1<<7
#define  _RQTYPE_DIR_OUT         0<<7
#define  _RQTYPE_DIR_IN          1<<7

#define  _RQTYPE_TYPE_MASK       3<<5
#define  _RQTYPE_TYPE_STD        0<<5
#define  _RQTYPE_TYPE_CLASS      1<<5
#define  _RQTYPE_TYPE_VENDOR     2<<5

#define  _RQTYPE_RECP_MASK       0x1F
#define  _RQTYPE_RECP_DEV        0
#define  _RQTYPE_RECP_INTF       1
#define  _RQTYPE_RECP_EP         2
#define  _RQTYPE_RECP_OTHER      3

#define  _RQTYPE_TR_MASK         (_RQTYPE_TYPE_MASK | _RQTYPE_RECP_MASK)
#define  _RQTYPE_TR_STD_DEV      (_RQTYPE_TYPE_STD | _RQTYPE_RECP_DEV)
#define  _RQTYPE_TR_STD_INTF     (_RQTYPE_TYPE_STD | _RQTYPE_RECP_INTF)
#define  _RQTYPE_TR_CLASS_INTF   (_RQTYPE_TYPE_CLASS | _RQTYPE_RECP_INTF)

//--- REQUEST: (STD)
#define  _RQ_GET_STATUS          0x00
#define  _RQ_CLEAR_FEATURE       0x01
#define  _RQ_SET_FEATURE         0x03
#define  _RQ_SET_ADDRESS         0x05
#define  _RQ_GET_DESCRIPTOR      0x06
#define  _RQ_SET_DESCRIPTOR      0x07
#define  _RQ_GET_CONFIGURATION   0x08
#define  _RQ_SET_CONFIGURATION   0x09
#define  _RQ_GET_INTERFACE       0x0A
#define  _RQ_SET_INTERFACE       0x0B
#define  _RQ_SYNC_FRAME          0x0C

//--- REQUEST: (CLASS)
#define  _RQ_SET_IDLE            0x0A
#define  _RQ_GET_REPORT          0x01
#define  _RQ_SET_REPORT          0x09

//--- REQUEST: GET_DESCRIPTOR:
#define  _DESC_TYPE_DEV          0x01
#define  _DESC_TYPE_CFG          0x02
#define  _DESC_TYPE_STR          0x03
#define  _DESC_TYPE_INTF         0x04
#define  _DESC_TYPE_EP           0x05
#define  _DESC_TYPE_DEVQUAL      0x06
#define  _DESC_TYPE_OSC          0x07
#define  _DESC_TYPE_RPT          0x22


//==========================================================
//--- DATA: EXTERN
//==========================================================

//--- USB buffer descriptor(s)
extern volatile far BDT usb_bdt_ep0_out;    // endpoint: 0, dir: out
extern volatile far BDT usb_bdt_ep0_in;     // endpoint: 0, dir: in
extern volatile far BDT usb_bdt_ep1_out;    // endpoint: 1, dir: out
extern volatile far BDT usb_bdt_ep1_in;     // endpoint: 1, dir: in

//--- USB buffer(s)
extern volatile far USB_BUF_CONTROL usb_buf_ep0_out;
extern volatile far USB_BUF_CONTROL usb_buf_ep0_in;
extern volatile far TD_BYTE usb_buf_ep1_out[USB_EP1_SIZE_MAX];
extern volatile far TD_BYTE usb_buf_ep1_in[USB_EP1_SIZE_MAX];

//--- INTERFACE / MISC
extern TD_BYTE    usb_addr;
extern TD_BYTE    usb_config;
extern TD_BYTE    usb_state;
extern TD_BYTE_B  usb_flags_B;

extern TD_POINTER usb_trn_src_p;
extern TD_POINTER usb_trn_dst_p;
extern TD_BYTE usb_trn_count;


//==========================================================
//--- PROTOTYPES
//==========================================================
void usb_init(void);
void usb_bdt_load(void);
void usb_isr(void);
void usb_svc(void);

void usb_detach_soft(void);
void usb_module_disable(void);

void usb_trn(void);
void usb_trn_setup(void);
void usb_trn_setup_std_dev(void);
void usb_trn_setup_std_dev_getdsc(void);
void usb_trn_setup_std_dev_getstring(void);
void usb_trn_setup_std_intf(void);
void usb_trn_setup_std_intf_getdsc(void);
void usb_trn_setup_class_intf(void);
void usb_trn_setup_class_intf_getreport(void);

void usb_trn_control_out(void);
void usb_trn_control_in(void);

void usb_ep0_load_flash_init(void);
void usb_ep0_load_flash(void);

void usb_ep0_load_0len(void);
void usb_ep0_trn_unsupported(void);
void usb_ep0_out_finish(void);
void usb_ep0_in_finish(void);

void usb_ep1_check(void);
void usb_ep1_in_finish(void);
void usb_ep1_out_finish(void);
void usb_ep1_out_get(TD_BYTE *dst_p);
void usb_ep1_in_put(TD_BYTE *src_p);

#endif // __USB_H
