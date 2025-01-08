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
//    (see comm_lin.c)
//
//==================================================================== 

#ifndef __COMM_LIN_H
#define __COMM_LIN_H
#include "exec.h"


//------------------------------------------------
//--- RESOURCES
//------------------------------------------------
#define  LIN_TX_TRIS       TRISCbits.TRISC6
#define  LIN_RX_TRIS       TRISCbits.TRISC7
#define  LIN_TX_PORT       PORTCbits.RC6
#define  LIN_RX_PORT       PORTCbits.RC7

#define  LIN_CS_TRIS       TRISBbits.TRISB0
#define  LIN_CS_PORT       PORTBbits.RB0
#define  LIN_AFAULT_TRIS   TRISBbits.TRISB1
#define  LIN_AFAULT_PORT   PORTBbits.RB1

#define  LIN_AUX1_TRIS     TRISBbits.TRISB0
#define  LIN_AUX2_TRIS     TRISBbits.TRISB1
#define  LIN_AUX1_PORT     PORTBbits.RB0
#define  LIN_AUX2_PORT     PORTBbits.RB1

//------------------------------------------------
//--- DEFINITIONS / DECLARATIONS
//------------------------------------------------

//--- INCOMING DATA TAG(S)
#define  COMM_TAG_LIN_INIT                   0x80
#define  COMM_TAG_LIN_XMT_DATA               0x81
#define  COMM_TAG_LIN_XMT_BRK                0x82
#define  COMM_TAG_LIN_XMT_BRK_DATA           0x83
#define  COMM_TAG_LIN_XMT_FRAME              0x84
#define  COMM_TAG_LIN_BAUD_SET               0x85
#define  COMM_TAG_LIN_SPROFILE_ID_WRITE      0x86
#define  COMM_TAG_LIN_SPROFILE_ID_READ       0x87

//--- OUTGOING DATA TAG(S)
#define  COMM_TAG_LIN_EVENT_BYTE_RX          0x80
#define  COMM_TAG_LIN_EVENT_BYTE_TX          0x81
#define  COMM_TAG_LIN_EVENT_STAT_ERR         0x82
#define  COMM_TAG_LIN_EVENT_BREAK_RX         0x83
#define  COMM_TAG_LIN_EVENT_BREAK_TX         0x84
#define  COMM_TAG_LIN_EVENT_AUTOBAUD         0x85
#define  COMM_TAG_LIN_EVENT_CHECKSUM_ERR     0x86
#define  COMM_TAG_LIN_EVENT_IDPARITY_ERR     0x87
#define  COMM_TAG_LIN_SPROFILE_ID_DATA       0x88

#define  COMM_LIN_WAIT_BREAK_MSEC   6
#define  COMM_LIN_WAIT_BREAK_T3     (COMM_LIN_WAIT_BREAK_MSEC / TIMER3_MSEC)

#define  COMM_LIN_WAIT_AB_MSEC      16
#define  COMM_LIN_WAIT_AB_T3        (COMM_LIN_WAIT_AB_MSEC / TIMER3_MSEC)

//------------------------------------------------
//--- COMM USART CONTROL BLOCK
//------------------------------------------------
#define  COMM_CB_LIN_CS_FLAG                    exec_control_B[16].bit3
#define  COMM_CB_LIN_CHECKSUM_ENHANCED_FLAG     exec_control_B[16].bit4
#define  COMM_CB_LIN_SLAVE_FLAG                 exec_control_B[16].bit5
#define  COMM_CB_LIN_RCV_ENABLE_FLAG            exec_control_B[16].bit6
#define  COMM_CB_LIN_AUTOBAUD_FLAG              exec_control_B[16].bit7

#define  COMM_CB_LIN_EVENT_EN_DATA_RX_FLAG      exec_control_B[17].bit0
#define  COMM_CB_LIN_EVENT_EN_DATA_TX_FLAG      exec_control_B[17].bit1
#define  COMM_CB_LIN_EVENT_EN_BREAK_TX_FLAG     exec_control_B[17].bit2
#define  COMM_CB_LIN_EVENT_EN_BREAK_RX_FLAG     exec_control_B[17].bit3
#define  COMM_CB_LIN_EVENT_EN_AUTOBAUD_FLAG     exec_control_B[17].bit4
#define  COMM_CB_LIN_EVENT_EN_CHECKSUM_ERR_FLAG exec_control_B[17].bit5
#define  COMM_CB_LIN_EVENT_EN_IDPARITY_ERR_FLAG exec_control_B[17].bit6
#define  COMM_CB_LIN_EVENT_EN_STAT_ERR_FLAG     exec_control_B[17].bit7

#define  COMM_CB_LIN_BRG_LO_REG                 exec_control_B[22]._byte
#define  COMM_CB_LIN_BRG_HI_REG                 exec_control_B[23]._byte

//------------------------------------------------
//--- COMM USART STATUS BLOCK
//------------------------------------------------
#define  COMM_SB_LIN_REG12                   exec_status_B[12]._b
#define  COMM_SB_LIN_REG13                   exec_status_B[13]._b
#define  COMM_SB_LIN_REG14                   exec_status_B[14]._b
#define  COMM_SB_LIN_REG15                   exec_status_B[15]._b
#define  COMM_SB_LIN_REG16                   exec_status_B[16]._b
#define  COMM_SB_LIN_REG17                   exec_status_B[17]._b
#define  COMM_SB_LIN_REG18                   exec_status_B[18]._b
#define  COMM_SB_LIN_REG19                   exec_status_B[19]._b

#define  COMM_SB_LIN_STAT_FERR_FLAG          exec_status_B[12].bit0
#define  COMM_SB_LIN_STAT_OERR_FLAG          exec_status_B[12].bit1
#define  COMM_SB_LIN_STAT_AFAULT_FLAG        exec_status_B[12].bit2
#define  COMM_SB_LIN_STAT_ABAUDERR_FLAG      exec_status_B[12].bit3
#define  COMM_SB_LIN_STAT_INITERR_FLAG       exec_status_B[12].bit6
#define  COMM_SB_LIN_STAT_ERR_FLAG           exec_status_B[12].bit7

#define  COMM_SB_LIN_STAT_ERR_REG            exec_status_B[12]._byte
#define  COMM_SB_LIN_STAT_ERR_REG_MASK       (TD_BYTE) 0x7F

#define  COMM_SB_LIN_AFAULT_STATE_FLAG       exec_status_B[13].bit0
#define  COMM_SB_LIN_CS_STATE_FLAG           exec_status_B[13].bit1

#define  COMM_SB_LIN_BRG_LO_REG              exec_status_B[18]._byte
#define  COMM_SB_LIN_BRG_HI_REG              exec_status_B[19]._byte

//------------------------------------------------
//--- VARBS USING SHARED RAM
//------------------------------------------------
#define  COMM_LIN_0_FLAGS                    comm_varbs_B[0]._b
#define  COMM_LIN_GIEL1_FLAG                 comm_varbs_B[0].bit0
#define  COMM_LIN_TX_SVC_FLAG                comm_varbs_B[0].bit1
#define  COMM_LIN_TX_SP_FLAG                 comm_varbs_B[0].bit2
#define  COMM_LIN_RX_SP_FLAG                 comm_varbs_B[0].bit3
#define  COMM_LIN_SEQ_BRK_FLAG               comm_varbs_B[0].bit4
#define  COMM_LIN_SEQ_SYN_FLAG               comm_varbs_B[0].bit5
#define  COMM_LIN_SEQ_ID_FLAG                comm_varbs_B[0].bit7

#define  COMM_LIN_1_FLAGS                    comm_varbs_B[1]._b
#define  COMM_LIN_FERR_FLAG                  comm_varbs_B[1].bit0
#define  COMM_LIN_OERR_FLAG                  comm_varbs_B[1].bit1
#define  COMM_LIN_ABERR_FLAG                 comm_varbs_B[1].bit2
#define  COMM_LIN_DATA_FLAG                  comm_varbs_B[1].bit3
#define  COMM_LIN_ABAUD_DISABLE_FLAG         comm_varbs_B[1].bit4
#define  COMM_LIN_3_FLAG                     comm_varbs_B[1].bit5
#define  COMM_LIN_2_FLAG                     comm_varbs_B[1].bit6
#define  COMM_LIN_1_FLAG                     comm_varbs_B[1].bit7

#define  COMM_LIN_SP_INDEX                   comm_varbs_B[2]._b
#define  COMM_LIN_SP_ID                      comm_varbs_B[3]._b
#define  COMM_LIN_SP_IDDEF                   comm_varbs_B[4]._b
#define  COMM_LIN_SP_NBYTES                  comm_varbs_B[5]._b
#define  COMM_LIN_SP_INDEX_DATA              comm_varbs_B[6]._b
#define  COMM_LIN_SP_CHECKSUM                comm_varbs_B[7]._b
#define  COMM_LIN_COUNT                      comm_varbs_B[9]._b

#define  COMM_LIN_SP_IDDEF_PUBLISH_FLAG      comm_varbs_B[4].bit4
#define  COMM_LIN_SP_IDDEF_ENHANCED_CS_FLAG  comm_varbs_B[4].bit5
#define  COMM_LIN_SP_IDDEF_CHKSUMDATA_FLAG   comm_varbs_B[4].bit6

//------------------------------------------------
//--- PROTOTYPES
//------------------------------------------------
TD_BYTE comm_lin_init(void);
void comm_lin_reset(void);
void comm_lin_svc(void);
void comm_lin_postproc(void);
void comm_lin_rx_isr(void);

void comm_lin_test(void);
TD_BYTE comm_lin_sp_iddef_search(TD_BYTE id);
TD_BYTE comm_lin_sp_iddef_next(void);
void comm_lin_tx_isr(void);
void comm_lin_sp_tx(void);
void comm_lin_tx(TD_BYTE data);
void comm_lin_svc_tx(void);
TD_BYTE comm_lin_parity(TD_BYTE data);
void comm_lin_clear(void);
void comm_lin_config(void);

#endif // __COMM_LIN_H
