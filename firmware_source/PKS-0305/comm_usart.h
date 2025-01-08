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
//    Filename:            comm_usart.h
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

#ifndef __COMM_USART_H
#define __COMM_USART_H
#include "exec.h"


//------------------------------------------------
//--- RESOURCES
//------------------------------------------------
#define  USART_TX_TRIS     TRISCbits.TRISC6
#define  USART_RX_TRIS     TRISCbits.TRISC7

#define  USART_AUX1_TRIS   TRISBbits.TRISB0
#define  USART_AUX2_TRIS   TRISBbits.TRISB1
#define  USART_AUX1_PORT   PORTBbits.RB0
#define  USART_AUX2_PORT   PORTBbits.RB1


//------------------------------------------------
//--- DEFINITIONS / DECLARATIONS
//------------------------------------------------

//--- INCOMING DATA TAG(S)
#define  COMM_TAG_USART_INIT                 0x80
#define  COMM_TAG_USART_RESET                0x81
#define  COMM_TAG_USART_DATA_XMT             0x82
#define  COMM_TAG_USART_DATA_SRCV            0x83
#define  COMM_TAG_USART_DATA_ARCV_ENABLE     0x84
#define  COMM_TAG_USART_DATA_ARCV_DISABLE    0x85
#define  COMM_TAG_USART_BREAK_XMT            0x86
#define  COMM_TAG_USART_BREAK_DATA_XMT       0x87
#define  COMM_TAG_USART_BAUD                 0x88
#define  COMM_TAG_USART_CKP_SET              0x89
#define  COMM_TAG_USART_CKP_RST              0x8A
#define  COMM_TAG_USART_9BIT_SET             0x8B
#define  COMM_TAG_USART_9BIT_RST             0x8C

#define  COMM_TAG_USART_AUX1_RST             0x90
#define  COMM_TAG_USART_AUX1_SET             0x91
#define  COMM_TAG_USART_AUX1_OUT             0x92
#define  COMM_TAG_USART_AUX1_IN              0x93
#define  COMM_TAG_USART_AUX1_WAIT_0          0x94
#define  COMM_TAG_USART_AUX1_WAIT_1          0x95
#define  COMM_TAG_USART_AUX2_RST             0x96
#define  COMM_TAG_USART_AUX2_SET             0x97
#define  COMM_TAG_USART_AUX2_OUT             0x98
#define  COMM_TAG_USART_AUX2_IN              0x99
#define  COMM_TAG_USART_AUX2_WAIT_0          0x9A
#define  COMM_TAG_USART_AUX2_WAIT_1          0x9B

//--- OUTGOING DATA TAG(S)
#define  COMM_TAG_USART_EVENT_BYTE_TX        0x80
#define  COMM_TAG_USART_EVENT_BYTE_RX        0x81
#define  COMM_TAG_USART_EVENT_STATUS_ERR     0x82
#define  COMM_TAG_USART_EVENT_BREAK_TX       0x83


//------------------------------------------------
//--- COMM USART CONTROL BLOCK
//------------------------------------------------
#define  COMM_CB_USART_MODE_REG              exec_control_B[16]._byte

#define  COMM_CB_USART_CONFIG_REG            exec_control_B[17]._byte
#define  COMM_CB_USART_CONFIG_CKP_FLAG       exec_control_B[17].bit0
#define  COMM_CB_USART_CONFIG_9BIT_FLAG      exec_control_B[17].bit1
#define  COMM_CB_USART_CONFIG_RCV_DIS_FLAG   exec_control_B[17].bit2

#define  COMM_CB_USART_EVENT_BYTE_RX_FLAG    exec_control_B[18].bit0
#define  COMM_CB_USART_EVENT_BYTE_TX_FLAG    exec_control_B[18].bit1
#define  COMM_CB_USART_EVENT_STATUS_ERR_FLAG exec_control_B[18].bit2
#define  COMM_CB_USART_EVENT_BREAK_TX_FLAG   exec_control_B[18].bit3

#define  COMM_CB_USART_19_REG                exec_control_B[19]._byte
#define  COMM_CB_USART_20_REG                exec_control_B[20]._byte
//#define  COMM_CB_USART_21_REG                exec_control_B[21]._byte

#define  COMM_CB_USART_AUX1_PORT_FLAG        exec_control_B[21].bit0
#define  COMM_CB_USART_AUX2_PORT_FLAG        exec_control_B[21].bit1
#define  COMM_CB_USART_AUX1_TRIS_FLAG        exec_control_B[21].bit2
#define  COMM_CB_USART_AUX2_TRIS_FLAG        exec_control_B[21].bit3

#define  COMM_CB_USART_BRG_LO_REG            exec_control_B[22]._byte
#define  COMM_CB_USART_BRG_HI_REG            exec_control_B[23]._byte


//------------------------------------------------
//--- COMM USART STATUS BLOCK
//------------------------------------------------
#define  COMM_SB_USART_REG12                 exec_status_B[12]._b
#define  COMM_SB_USART_REG13                 exec_status_B[13]._b
#define  COMM_SB_USART_REG14                 exec_status_B[14]._b
#define  COMM_SB_USART_REG15                 exec_status_B[15]._b
#define  COMM_SB_USART_REG16                 exec_status_B[16]._b
#define  COMM_SB_USART_REG17                 exec_status_B[17]._b
#define  COMM_SB_USART_REG18                 exec_status_B[18]._b
#define  COMM_SB_USART_REG19                 exec_status_B[19]._b

#define  COMM_SB_USART_STATUS_FERR_FLAG      exec_status_B[12].bit0
#define  COMM_SB_USART_STATUS_OERR_FLAG      exec_status_B[12].bit1
#define  COMM_SB_USART_INIT_ERR_FLAG         exec_status_B[12].bit2
#define  COMM_SB_USART_ERR_FLAG              exec_status_B[12].bit7

#define  COMM_SB_USART_ERR_REG               exec_status_B[12]._byte
#define  COMM_SB_USART_ERR_REG_MASK          (TD_BYTE) 0x7F

#define  COMM_SB_USART_MODE_REG              exec_status_B[13]._byte

#define  COMM_SB_USART_CONFIG_REG            exec_status_B[14]._byte
#define  COMM_SB_USART_CONFIG_CKP_FLAG       exec_status_B[14].bit0
#define  COMM_SB_USART_CONFIG_9BIT_FLAG      exec_status_B[14].bit1
#define  COMM_SB_USART_CONFIG_RCV_DIS_FLAG   exec_status_B[14].bit2

#define  COMM_SB_USART_AUX1_PORT_FLAG        exec_status_B[16].bit0
#define  COMM_SB_USART_AUX2_PORT_FLAG        exec_status_B[16].bit1
#define  COMM_SB_USART_AUX1_TRIS_FLAG        exec_status_B[16].bit2
#define  COMM_SB_USART_AUX2_TRIS_FLAG        exec_status_B[16].bit3

#define  COMM_SB_USART_BRG_LO_REG            exec_status_B[18]._byte
#define  COMM_SB_USART_BRG_HI_REG            exec_status_B[19]._byte

//------------------------------------------------
//--- VARBS USING SHARED RAM
//------------------------------------------------
#define  COMM_USART_FLAGS                    comm_varbs_B[0]._b

#define  COMM_USART_RCV_FLAG                 comm_varbs_B[0].bit0

//------------------------------------------------
//--- PROTOTYPES
//------------------------------------------------
TD_BYTE comm_usart_init(void);
void comm_usart_reset(void);
void comm_usart_config(void);
void comm_usart_clear(void);
void comm_usart_svc(void);
void comm_usart_postproc(void);
void comm_usart_mode_set(void);
//void comm_usart_rcv(void);
void comm_usart_rcv_isr(void);

#endif // __COMM_USART_H
