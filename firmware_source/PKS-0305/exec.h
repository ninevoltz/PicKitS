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
//    Filename:            exec.h
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
//    FILE: exec.h
//    NOTE: 0001 09-01-06 ME
//    -- removed firmware verion definition (moved to project.h)
//
//    NOTE: 0000 08-01-06 ME
//    -- initial release
//
//==================================================================== 
#ifndef __EXEC_H
#define __EXEC_H

//==========================================================
//--- RESOURCES
//==========================================================

//--- SWITCH
#define  SW_PIN               PORTBbits.RB5
#define  SW_PIN_DIR           TRISBbits.TRISB5


//==========================================================
//--- PROTOTYPES
//==========================================================
void exec_svc(void);
void exec_init(void);
void exec_dflush_time_set(void);
TD_BYTE exec_data_get(void);
void exec_data_put(TD_BYTE b0);
void exec_cmnd_proc(TD_BYTE cmnd);
void exec_packet_proc(void);
void exec_packet_prep(void);
void exec_packet_status(TD_BYTE id);
void exec_packet_finish(void);
void exec_packet_cbuf2(void);
void exec_cb_ee_write(void);
TD_BYTE exec_cb_ee_read(void);
void exec_cb_load(void);
void exec_cb_load_dflt(void);
void exec_cbuf1_put(void);

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
#define  EXEC_CB_LENGTH             24
#define  EXEC_SB_LENGTH             20
extern TD_BYTE_B exec_flags;
extern TD_BYTE *exec_data_p;
extern TD_BYTE exec_data_count;
extern TD_BYTE exec_tag;
extern TD_BYTE_B exec_control_B[EXEC_CB_LENGTH];
extern TD_BYTE_B exec_status_B[EXEC_SB_LENGTH];
extern TD_BYTE exec_status_save_1;
extern TD_BYTE exec_status_save_2;
extern TD_BYTE exec_status_save_3;
extern TD_BYTE exec_status_id;

//--------------------------------------
//--- DATA TAGs
//--------------------------------------
#define  EXEC_TAG_EOD               0x00
#define  EXEC_TAG_COMMAND           0x01
#define  EXEC_TAG_CB_WRITE          0x02
#define  EXEC_TAG_CBUF1_WRITE       0x03
#define  EXEC_TAG_CBUF2_WRITE       0x04
#define  EXEC_TAG_CBUF3_WRITE       0x05
#define  EXEC_TAG_LED1_CONFIG       0x06
#define  EXEC_TAG_LED2_CONFIG       0x07
#define  EXEC_TAG_CBUF1_RESET       0x08
#define  EXEC_TAG_CBUF2_RESET       0x09
#define  EXEC_TAG_CBUF3_RESET       0x0A
#define  EXEC_TAG_EXEC_STATUS_RQ    0x0B
#define  EXEC_TAG_RESET             0x0C

#define  EXEC_TAG_BOOT_B            0x42
#define  EXEC_TAG_BOOT_b            0x62

#define  EXEC_TAG_EOD_OUT           0x80
#define  EXEC_TAG_FIRMWARE_VERSION  0x81
#define  EXEC_TAG_CB_DATA           0x82
#define  EXEC_TAG_SB_DATA           0x83
#define  EXEC_TAG_CBUF_STATUS       0x84
#define  EXEC_TAG_CBUF1_DATA        0x85
#define  EXEC_TAG_CBUF2_DATA        0x86
#define  EXEC_TAG_CBUF3_DATA        0x87
#define  EXEC_TAG_PACKET_ID         0x88

//--------------------------------------
//--- CONTROL_BLOCK
//--------------------------------------
#define  EXEC_CB_LED2_DISABLE_DFLT_FLAG   exec_control_B[0].bit4
#define  EXEC_CB_LED1_DISABLE_DFLT_FLAG   exec_control_B[0].bit5
#define  EXEC_CB_DFLUSH_THRESHOLD_FLAG    exec_control_B[0].bit6
#define  EXEC_CB_DFLUSH_TIME_FLAG         exec_control_B[0].bit7
#define  EXEC_CB_REG_FIRST                0
#define  EXEC_CB_REG_LAST                 7

#define  EXEC_CB_SWITCH_TEST_FLAG         exec_control_B[1].bit0

#define  EXEC_CB_DFLUSH_COUNT_REG         exec_control_B[3]._b
#define  EXEC_CB_DFLUSH_TIME_REG          exec_control_B[4]._b

//--------------------------------------
//--- STATUS_BLOCK
//--------------------------------------
#define  EXEC_SB_CBUF1_OVF_FLAG           exec_status_B[0].bit1
#define  EXEC_SB_CBUF2_OVF_FLAG           exec_status_B[0].bit2
#define  EXEC_SB_CBUF3_OVF_FLAG           exec_status_B[0].bit3
#define  EXEC_SB_DATA_ERR_FLAG            exec_status_B[0].bit4
#define  EXEC_SB_CB_DFLT_FLAG             exec_status_B[0].bit5

#define  EXEC_SB_SWITCH_TEST_FLAG         exec_status_B[1].bit0
#define  EXEC_SB_SWITCH_FLAG              exec_status_B[1].bit1

#define  EXEC_SB_REG_FIRST                 0
#define  EXEC_SB_REG_LAST                  3
#define  EXEC_SB_REG_ALL_FIRST            0
#define  EXEC_SB_REG_ALL_LAST             19

//--------------------------------------
//--- FLAGS
//--------------------------------------
#define  EXEC_PROC_LOOP_FLAG              exec_flags.bit0
#define  EXEC_DFLUSH_FLAG                 exec_flags.bit1
#define  EXEC_RQ_STATUS_PACKET_FLAG       exec_flags.bit2
#define  EXEC_SWTEST_FLAG                 exec_flags.bit3
//#define  EXEC_SWTEST2_FLAG                exec_flags.bit4
#define  EXEC_RQ_STATUS_PACKET_ID_FLAG    exec_flags.bit4
#define  EXEC_GIEL_FLAG                   exec_flags.bit5
#define  EXEC_CB_NEW_FLAG                 exec_flags.bit6
#define  EXEC_PACKET_IN_FLAG              exec_flags.bit7


//--------------------------------------
//--- MISC
//--------------------------------------
#define  EXEC_STATUS_PACKET_ID            0x00
#define  EXEC_DATA_LENGTH_MAX             64
#define  EXEC_PACKET_LENGTH_MAX           64
#define  EXEC_CB_CHKSUM_SEED              0x12

//--------------------------------------
//--- SW TEST: LED CONFIG: OFF
//--------------------------------------
#define  EXEC_LED_CONFIG_SWTEST_OFF       0x00

//--------------------------------------
//--- SW TEST: LED CONFIG: BLINK FAST
//--------------------------------------
#define  EXEC_LED_CONFIG_SWTEST_ON        0xC0

#define  EXEC_REBOOT_DLY_MSEC    100
#define  EXEC_REBOOT_DLY_TA      EXEC_REBOOT_DLY_MSEC * 1000 / TIMER0_A_USEC

//--------------------------------------
//--- EXEC CMND(s)
//--------------------------------------
#define  EXEC_CMND_INIT                   0x00
#define  EXEC_CMND_COMM_INIT_MODE         0x01
#define  EXEC_CMND_RQ_STATUS_PACKET       0x02
#define  EXEC_CMND_CB_EE_WRITE            0x03
#define  EXEC_CMND_CB_EE_READ             0x04
#define  EXEC_CMND_CBUF2_FLUSH            0x05
#define  EXEC_CMND_COMM_INIT              0x06
#define  EXEC_CMND_COMM_CLEAR             0x07

#endif // __EXEC_H
