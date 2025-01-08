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
//    Filename:            usb_constants.h
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
//    FILE: usb_data_constants.h
//    NOTE: 0001 09-01-06 ME
//    -- modified descr. string length of USB_DSC_STR_02 from 12 to 22
//
//    NOTE: 0000 08-01-06 ME
//    -- initial release
//
//==================================================================== 
#ifndef __USB_DATA_CONSTANTS_H
#define __USB_DATA_CONSTANTS_H

//======================================
//--- DESCRIPTOR TYPES
//======================================
#define  USB_DSC_TYPE_DEV     0x01
#define  USB_DSC_TYPE_CFG     0x02
#define  USB_DSC_TYPE_STR     0x03
#define  USB_DSC_TYPE_INTF    0x04
#define  USB_DSC_TYPE_EP      0x05

//======================================
//--- DESCRIPTOR STRUCT: DEVICE
//======================================
typedef struct _USB_DSC_DEV
{
   TD_BYTE  length;
   TD_BYTE  dsc_type;
   TD_WORD  usb_bcd;
   TD_BYTE  class;
   TD_BYTE  subclass;
   TD_BYTE  protocol;
   TD_BYTE  packet_size_0;
   TD_WORD  vendor_id_w;
   TD_WORD  product_id_w;
   TD_WORD  device_bcd_w;
   TD_BYTE  manuf;
   TD_BYTE  product;
   TD_BYTE  serialno;
   TD_BYTE  configno;
} USB_DSC_DEV;

//======================================
//--- DESCRIPTOR STRUCT: CONFIGURATION
//======================================
typedef struct _USB_DSC_CFG
{
   TD_BYTE  length;
   TD_BYTE  type;
   TD_WORD  total_length_w;
   TD_BYTE  intf_num;
   TD_BYTE  cfg_value;
   TD_BYTE  icfg;
   TD_BYTE  attrib;
   TD_BYTE  max_power;
} USB_DSC_CFG;

//======================================
//--- DESCRIPTOR STRUCT: INTERFACE
//======================================
typedef struct _USB_DSC_INTF
{
    TD_BYTE length;
    TD_BYTE type;
    TD_BYTE intf_num;
    TD_BYTE alt_setting;
    TD_BYTE num_eps;
    TD_BYTE class;
    TD_BYTE subclass;
    TD_BYTE protocol;
    TD_BYTE intf;
} USB_DSC_INTF;

//======================================
//--- DESCRIPTOR STRUCT: ENDPOIINT
//======================================
typedef struct _USB_DSC_EP
{
   TD_BYTE  length;
   TD_BYTE  type;
   TD_BYTE  ep_addr;
   TD_BYTE  attrib;
   TD_WORD  max_packet_size_w;
   TD_BYTE  interval;
} USB_DSC_EP;

//======================================
//--- DESCRIPTOR STRUCT: CONFIG SET
//======================================
#define USB_DSC_CFG_01 rom struct \
{ \
   USB_DSC_CFG    cfg;        \
   USB_DSC_INTF   intf;       \
   USB_DSC_HID    hid;        \
   USB_DSC_EP     ep01_in;    \
   USB_DSC_EP     ep01_out;   \
}

//======================================
//--- DESCRIPTOR STRUCT: HID
//======================================
#define  USB_DSC_TYPE_HID     0x21
#define  USB_DSC_TYPE_RPT     0x22

typedef struct _USB_DSC_HID
{
   TD_BYTE  length;
   TD_BYTE  type;
   TD_WORD  class;
   TD_BYTE  country;
   TD_BYTE  class_dsc;
   TD_BYTE  type_report;
   TD_WORD  report_size_w;
} USB_DSC_HID;

#define USB_DSC_HID_RPT_01 rom struct{TD_BYTE report[29];}

#define USB_DSC_STR_00 rom struct {TD_BYTE length; TD_BYTE type; TD_WORD string[1];}
#define USB_DSC_STR_01 rom struct {TD_BYTE length; TD_BYTE type; TD_WORD string[25];}
#define USB_DSC_STR_02 rom struct {TD_BYTE length; TD_BYTE type; TD_WORD string[22];}
   
extern rom USB_DSC_DEV usb_dsc_dev;
extern USB_DSC_CFG_01 usb_dsc_cfg_01;
extern USB_DSC_HID_RPT_01 usb_dsc_hid_rpt_01;
extern USB_DSC_STR_00 usb_dsc_str_00;
extern USB_DSC_STR_01 usb_dsc_str_01;
extern USB_DSC_STR_02 usb_dsc_str_02;

#endif // __USB_DATA_CONSTANTS_H
