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
//    Filename:            comm_gen.h
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

#ifndef __COMM_GEN_H
#define __COMM_GEN_H

#include "typedefs.h"
#include "exec.h"

//==========================================================
//--- RESOURCES
//==========================================================
#define  COMM_IO_1A_PIN    PORTCbits.RC6
#define  COMM_IO_4A_PIN    PORTBbits.RB0
#define  COMM_IO_5A_PIN    PORTBbits.RB1
#define  COMM_IO_6A_PIN    PORTCbits.RC7

#define  COMM_IO_1A_DIR    TRISCbits.TRISC6
#define  COMM_IO_4A_DIR    TRISBbits.TRISB0
#define  COMM_IO_5A_DIR    TRISBbits.TRISB1
#define  COMM_IO_6A_DIR    TRISCbits.TRISC7

#define  COMM_IO_1B_PIN    PORTAbits.RA5
#define  COMM_IO_4B_PIN    PORTAbits.RA1
#define  COMM_IO_5B_PIN    PORTAbits.RA2
#define  COMM_IO_6B_PIN    PORTAbits.RA3

#define  COMM_IO_1B_DIR    TRISAbits.TRISA5
#define  COMM_IO_4B_DIR    TRISAbits.TRISA1
#define  COMM_IO_5B_DIR    TRISAbits.TRISA2
#define  COMM_IO_6B_DIR    TRISAbits.TRISA3

#define  COMM_PULLUP_TRIS  TRISAbits.TRISA4
#define  COMM_PULLUP_PIN   LATAbits.LATA4


//==========================================================
//--- PROTOTYPES
//==========================================================
TD_BYTE comm_data_get(void);
void comm_svc(void);
void comm_gen(void);
void comm_post_data(void);
void comm_post_event_macro_loop(void);
void comm_post_cbuf2(TD_BYTE data_byte);
void comm_post_cbuf2_time(TD_BYTE data_byte);
void comm_post_cbuf3(TD_BYTE data_byte);
void comm_post_event(TD_BYTE b0);
void comm_post_event_1b(TD_BYTE b0, TD_BYTE b1);
void comm_post_event_2b(TD_BYTE b0, TD_BYTE b1, TD_BYTE b2);
void comm_post_event_time(void);
void comm_post_event_time_get(void);
void comm_led_busy_data(void);
void comm_init(void);
void comm_init_mode(TD_BYTE mode);
void comm_reset(void);
void comm_varbs_clear(void);
void comm_clear(void);
void comm_gen_reset(void);
void comm_gen_config(void);

#define  COMM_VSRC_TIMER            timer0_A3
#define  COMM_VSRC_MEAS_INTV_MSEC   100
#define  COMM_VSRC_MEAS_INTV_TA     COMM_VSRC_MEAS_INTV_MSEC * 1000.0 / TIMER0_A_USEC

//==========================================================
//--- DECLARATIONS / DEFINITIONS
//==========================================================
extern TD_BYTE comm_state;
extern TD_BYTE comm_data;
extern TD_BYTE comm_data_2;
extern TD_BYTE comm_data_3;
extern TD_BYTE comm_cmnd_count_1;
extern TD_BYTE comm_cmnd_count_2;
extern TD_BYTE comm_cmnd_macro_count_1;
extern TD_BYTE comm_cmnd_macro_count_2;
extern TD_WORD_W comm_macro_count_1_W;
extern TD_WORD_W comm_macro_count_2_W;
extern TD_BYTE_B comm_flags1_B;
extern TD_BYTE_B comm_flags2_B;
extern TD_BYTE_B comm_varbs_B[16];

//------------------------------------------------
//--- COMM CONTROL BLOCK
//------------------------------------------------
#define  COMM_CB_EVENT_FLAG               exec_control_B[9].bit0
#define  COMM_CB_EVENT_TIME_FLAG          exec_control_B[9].bit1
#define  COMM_CB_PULLUP_ENABLE_FLAG       exec_control_B[9].bit4
#define  COMM_CB_VSRC_ENABLE_FLAG         exec_control_B[9].bit5
#define  COMM_CB_VSRC_VAR_FLAG            exec_control_B[9].bit6
#define  COMM_CB_MACRO_ABORT_FLAG         exec_control_B[9].bit7

#define  COMM_CB_EVENT_MACRO_LOOP_FLAG    exec_control_B[10].bit0
#define  COMM_CB_EVENT_MACRO65K_FLAG      exec_control_B[10].bit1
#define  COMM_CB_EVENT_MACRO_DONE_FLAG    exec_control_B[10].bit2
#define  COMM_CB_EVENT_TIMEOUT_AB0_FLAG   exec_control_B[10].bit3
#define  COMM_CB_EVENT_TIMEOUT_AB1_FLAG   exec_control_B[10].bit4
#define  COMM_CB_EVENT_STATUS_ERR_FLAG    exec_control_B[10].bit5

#define  COMM_CB_MODE_REG                 exec_control_B[8]._byte

#define  COMM_CB_VSRC_SETPOINT_REG        exec_control_B[12]._b
#define  COMM_CB_VSRC_FAULTLEVEL_REG      exec_control_B[13]._b

//------------------------------------------------
//--- COMM STATUS BLOCK
//------------------------------------------------
#define  COMM_SB_TIMEOUT_AB0_FLAG         exec_status_B[4].bit0
#define  COMM_SB_TIMEOUT_AB1_FLAG         exec_status_B[4].bit1
#define  COMM_SB_INIT_ERR_FLAG            exec_status_B[4].bit2
#define  COMM_SB_VSRC_FAULT_FLAG          exec_status_B[4].bit3
#define  COMM_SB_BAD_CMND_FLAG            exec_status_B[4].bit4
#define  COMM_SB_CBUF2_OVF_FLAG           exec_status_B[4].bit5
#define  COMM_SB_CBUF3_OVF_FLAG           exec_status_B[4].bit6
#define  COMM_SB_GEN_ERR_FLAG             exec_status_B[4].bit7

#define  COMM_SB_GEN_ERR_REG              exec_status_B[4]._b
#define  COMM_SB_GEN_ERR_REG_MASK         (TD_BYTE) 0x7F

#define  COMM_SB_BUSY_FLAG                exec_status_B[5].bit0
#define  COMM_SB_MACRO_BUSY_FLAG          exec_status_B[5].bit1
#define  COMM_SB_MACRO_CONT_FLAG          exec_status_B[5].bit2
#define  COMM_SB_WAIT_FLAG                exec_status_B[5].bit3
#define  COMM_SB_BUTTON_FLAG              exec_status_B[5].bit6
#define  COMM_SB_END_SCRIPT_FLAG          exec_status_B[5].bit7

#define  COMM_SB_VSRC_MEAS_REG            exec_status_B[7]._b

//--- all STATUS_BLOCK locations used by communications
#define  COMM_SB_REG_FIRST                4
#define  COMM_SB_REG_LAST                 19

#define  COMM_SB_MODE_REG                 exec_status_B[6]._byte

//------------------------------------------------
//--- COMM "FLAGS"
//------------------------------------------------

//--- valid per instruction ("gen" module)
#define  COMM_0_FLAG                comm_flags1_B.bit0
#define  COMM_1_FLAG                comm_flags1_B.bit1
#define  COMM_2_FLAG                comm_flags1_B.bit2
#define  COMM_3_FLAG                comm_flags1_B.bit3
//--- valid per instruction (protocol specific module)
#define  COMM_4_FLAG                comm_flags1_B.bit4
#define  COMM_5_FLAG                comm_flags1_B.bit5
#define  COMM_6_FLAG                comm_flags1_B.bit6
#define  COMM_7_FLAG                comm_flags1_B.bit7

//--- valid across multiple instructions
#define  COMM_PROC_LOOP_FLAG        comm_flags2_B.bit0
#define  COMM_MACRO_FLAG            comm_flags2_B.bit1
#define  COMM_TIMEOUT_AB0_FLAG      comm_flags2_B.bit2
#define  COMM_TIMEOUT_AB1_FLAG      comm_flags2_B.bit3
#define  COMM_FLUSH_FLAG            comm_flags2_B.bit4
#define  COMM_GIEL1_FLAG            comm_flags2_B.bit5
#define  COMM_GIEL2_FLAG            comm_flags2_B.bit6
#define  COMM_GIEL3_FLAG            comm_flags2_B.bit7

//------------------------------------------------
//--- COMM CONTROLLER "STATE"
//------------------------------------------------
#define  COMM_STATE_GEN_RESET             0x00
#define  COMM_STATE_GEN_NEXT              0x00
#define  COMM_STATE_GEN_NEXT_             0x01
#define  COMM_STATE_GEN_MACRO_X_INIT      0x02
#define  COMM_STATE_GEN_STALL             0x0F
#define  COMM_STATE_GEN_MAX               0x7F

#define  COMM_TAG_MIN                     0x10
#define  COMM_TAG_GEN_WAIT_1              0x10
#define  COMM_TAG_GEN_LED1                0x12
#define  COMM_TAG_GEN_LED2                0x13
//#define  COMM_TAG_GEN_LED3                0x14
#define  COMM_TAG_GEN_TIMEOUT_AB0_SET     0x15
#define  COMM_TAG_GEN_TIMEOUT_AB0_RST     0x16
#define  COMM_TAG_GEN_TIMEOUT_AB1_SET     0x17
#define  COMM_TAG_GEN_TIMEOUT_AB1_RST     0x18
#define  COMM_TAG_GEN_MACRO_CLEAR         0x19
#define  COMM_TAG_GEN_MACRO_DATA          0x1A
#define  COMM_TAG_GEN_MACRO_RUN           0x1B
#define  COMM_TAG_GEN_END_SCRIPT          0x1C
#define  COMM_TAG_GEN_MACRO_DATA_START    0x1D
#define  COMM_TAG_GEN_MACRO_DATA_END      0x1E
#define  COMM_TAG_GEN_MARKER_HOST         0x1F
#define  COMM_TAG_GEN_TIMER_RESET         0x20
#define  COMM_TAG_GEN_FLUSH_CBUF2         0x21

#define  COMM_TAG_GEN_WAIT_SW_0           0x22
#define  COMM_TAG_GEN_WAIT_SW_1           0x23
#define  COMM_TAG_GEN_SIM_SPECIAL_1       0x24


#define  COMM_TAG_GEN_DATA                0x10
#define  COMM_TAG_GEN_DATA_BYTES          0x11
#define  COMM_TAG_GEN_EVENT_MACRO_LOOP    0x12
#define  COMM_TAG_GEN_EVENT_TIME          0x13
#define  COMM_TAG_GEN_EVENT_TIME_ROLLOVER 0x14
#define  COMM_TAG_GEN_EVENT_MACRO_DONE    0x15
#define  COMM_TAG_GEN_EVENT_MACRO65K      0x16
#define  COMM_TAG_GEN_EVENT_MACRO_ABORT   0x17
#define  COMM_TAG_GEN_EVENT_TIMEOUT_AB0   0x18
#define  COMM_TAG_GEN_EVENT_TIMEOUT_AB1   0x19
#define  COMM_TAG_GEN_STATUS_ERR          0x1A
#define  COMM_TAG_GEN_EVENT_END_SCRIPT    0x1B
#define  COMM_TAG_GEN_MARKER_DEV          0x1C

//------------------------------------------------
//--- COMM "MODES"
//------------------------------------------------
#define  COMM_MODE_IDLE                   0x00
#define  COMM_MODE_I2CM                   0x01
#define  COMM_MODE_SPI                    0x02
#define  COMM_MODE_SPI_M                  0x02
#define  COMM_MODE_SPI_S                  0x03
#define  COMM_MODE_USART_A                0x04
#define  COMM_MODE_USART_SM               0x05
#define  COMM_MODE_USART_SS               0x06
#define  COMM_MODE_I2CS                   0x07
#define  COMM_MODE_I2CBBM                 0x08
#define  COMM_MODE_I2CSBBM                0x09
#define  COMM_MODE_LIN                    0x0A
#define  COMM_MODE_MWIRE_M                0x0B

//------------------------------------------------
//--- LED CONFIG: DATA TRANSACTION BUSY
//    mode: BLINK ONCE
//    time: 300ms
//------------------------------------------------
#define  COMM_LED_CONFIG_BUSY_DATA        0xA5

//------------------------------------------------
//--- LED CONFIG: SCRIPT BUSY: START
//    mode: ON
//------------------------------------------------
#define  COMM_LED_CONFIG_BUSY_SCRIPT_STR  0x20
#define  COMM_LED_CONFIG_OFF              0x00

//------------------------------------------------
//--- LED CONFIG: SCRIPT BUSY: END
//    mode: BLINK ONCE
//    time: 250ms
//------------------------------------------------
#define  COMM_LED_CONFIG_BUSY_SCRIPT_END  0xA4

#endif // __COMM_GEN_H
