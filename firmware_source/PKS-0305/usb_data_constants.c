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
//    Filename:            usb_data_constants.c
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
//    file: usb_data_constants.c
//
//    note: 0001 09-01-06 ME
//    -- modified string usb_dsc_str_02 from 
//       'Communicator' to 'PICkit Serial Analyzer'
//
//    note: 0000 08-01-06 ME
//    -- initial release
//
//==================================================================== 
#include "project.h"
#include "usb.h"
#include "usb_data_constants.h"

#pragma romdata
//--------------------------------------
//--- REPORT
//--------------------------------------
USB_DSC_HID_RPT_01 usb_dsc_hid_rpt_01 = 
{
   0x06, 0x00, 0xFF,          // usage page
   0x09, 0x01,                // usage
   0xA1, 0x01,                // collection
   0x19, 0x01,                // usage (min)
   0x29, 0x40,                // usage (max)
   0x15, 0x00,                // logical (min)
   0x26, 0xFF, 0x00,          // logical (max)
   0x75, 0x08,                // report size (bits)
   0x95, 0x40,                // report count (fields)
   0x81, 0x02,                // input (data, varb, absol)
   0x19, 0x01,                // usage (min)
   0x29, 0x40,                // usage (max)
   0x91, 0x02,                // output (data, varb, absol)
   0xC0
};   

//--------------------------------------
//--- DESCRIPTOR: DEVICE
//--------------------------------------
rom USB_DSC_DEV usb_dsc_dev = 
{
   sizeof(USB_DSC_DEV),       // Size of this descriptor in bytes
   USB_DSC_TYPE_DEV,          // DEVICE descriptor type
   0x0101,                    // USB Spec Release Number in BCD format
   0x00,                      // Class Code
   0x00,                      // Subclass code
   0x00,                      // Protocol code
   USB_EP0_SIZE_MAX,          // Max packet size for EP0, see usbcfg.h
   0x04D8,                    // Vendor ID = 1240
   0x0036,                    // Product ID = 
   0x0100,                    // Device release number in BCD format
   0x01,                      // Manufacturer string index
   0x02,                      // Product string index
   0x00,                      // Device serial number string index
   0x01                       // Number of possible configurations
};

//--------------------------------------
//--- DESCRIPTOR: CONFIGURATION
//--------------------------------------
USB_DSC_CFG_01 usb_dsc_cfg_01 = 
{
   //-------------------------
   //--- CONFIGURATION
   //-------------------------
   sizeof(USB_DSC_CFG),       // Size of this descriptor in bytes
   USB_DSC_TYPE_CFG,          // CONFIGURATION descriptor type
   sizeof(usb_dsc_cfg_01),    // Total length of data for this cfg
   1,                         // Number of interfaces in this cfg
   1,                         // Index value of this configuration
   0,                         // Configuration string index
   0x80,                      // Attributes, see usbdefs_std_dsc.h
   50,                        // Max power consumption (2X mA)

   //-------------------------
   //--- INTERFACE
   //-------------------------
   sizeof(USB_DSC_INTF),      // Size of this descriptor in bytes
   USB_DSC_TYPE_INTF,         // INTERFACE descriptor type
   0,                         // Interface Number
   0,                         // Alternate Setting Number
   2,                         // Number of endpoints in this intf
   3,                         // Class code (3: HID)
   0,                         // Subclass code
   0,                         // Protocol code
   0,                         // Interface string index

   //-------------------------
   //--- HID, class-specific
   //-------------------------
   sizeof(USB_DSC_HID),       // Size of this descriptor in bytes
   USB_DSC_TYPE_HID,          // HID descriptor type
   0x0001,                    // HID Spec Release Number in BCD format
   0x00,                      // Country Code (0x00 for Not supported)
   1,                         // Number of class descriptors
   USB_DSC_TYPE_RPT,          // Report descriptor type
   sizeof(usb_dsc_hid_rpt_01), // Size of the report descriptor

   //-------------------------
   //--- ENDPOINT 1 - IN
   //-------------------------
   sizeof(USB_DSC_EP),        // length
   USB_DSC_TYPE_EP,           // type
   0x01,                      // EP1, IN
   3,                         // INT
   USB_EP1_SIZE_MAX,          // max packet size
   //debug
   //10,                         // polling interval
   1,                         // polling interval

   //-------------------------
   //--- ENDPOINT 1 - OUT
   //-------------------------
   sizeof(USB_DSC_EP),        // length
   USB_DSC_TYPE_EP,           // type
   0x81,                      // EP1, OUT
   3,                         // INT
   USB_EP1_SIZE_MAX,          // max packet size
   1                          // polling interval
};

//======================================
//--- STRINGS
//======================================
USB_DSC_STR_00 usb_dsc_str_00 = {
   sizeof(usb_dsc_str_00), _DESC_TYPE_STR, 0x0409};

USB_DSC_STR_01 usb_dsc_str_01 = {
   sizeof(usb_dsc_str_01), _DESC_TYPE_STR,
   'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'};

USB_DSC_STR_02 usb_dsc_str_02 = {
   sizeof(usb_dsc_str_02), _DESC_TYPE_STR,
   'P','I','C','k','i','t',' ','S','e','r','i','a','l',' ','A','n','a','l','y','z','e','r'};
