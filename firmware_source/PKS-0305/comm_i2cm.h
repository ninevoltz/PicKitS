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
//    Filename:            comm_i2cm.h
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
#ifndef __COMM_I2CM_H
#define __COMM_I2CM_H


//==========================================================
//--- RESOURCES
//==========================================================
#define  COMM_I2CM_SCK              LATBbits.LATB1
#define  COMM_I2CM_SDA              LATBbits.LATB0
#define  COMM_I2CM_SCK_TRIS         TRISBbits.TRISB1     
#define  COMM_I2CM_SDA_TRIS         TRISBbits.TRISB0     
#define  COMM_I2CM_ADC_SDA_TRIS     TRISAbits.TRISA1
#define  COMM_I2CM_ADC_SCL_TRIS     TRISAbits.TRISA2

#define  I2CM_AUX1_TRIS    TRISCbits.TRISC6
#define  I2CM_AUX2_TRIS    TRISCbits.TRISC7
#define  I2CM_AUX1_PORT    PORTCbits.RC6
#define  I2CM_AUX2_PORT    PORTCbits.RC7

//==========================================================
//--- PROTOTYPES
//==========================================================
TD_BYTE comm_i2cm_init(void);
void comm_i2cm_reset(void);
void comm_i2cm_svc(void);
void comm_i2cm_postproc(void);
void comm_i2cm_adc_bus(void);


//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
#define  COMM_TAG_I2CM_INIT               0x80
#define  COMM_TAG_I2CM_START              0x81
#define  COMM_TAG_I2CM_STOP               0x82
#define  COMM_TAG_I2CM_RESTART            0x83
#define  COMM_TAG_I2CM_WRITE_BYTES        0x84
#define  COMM_TAG_I2CM_READ_BYTES         0x85
#define  COMM_TAG_I2CM_READ_BLOCK         0x86
#define  COMM_TAG_I2CM_BITRATE            0x87
//#define  COMM_TAG_I2CM_RESET              0x88
#define  COMM_TAG_I2CM_READ_BYTES_NLB     0x89
#define  COMM_TAG_I2CM_READ_BLOCK_NLB     0x8A

#define  COMM_TAG_I2CM_AUX1_RST           0x90
#define  COMM_TAG_I2CM_AUX1_SET           0x91
#define  COMM_TAG_I2CM_AUX1_OUT           0x92
#define  COMM_TAG_I2CM_AUX1_IN            0x93
#define  COMM_TAG_I2CM_AUX1_WAIT_0        0x94
#define  COMM_TAG_I2CM_AUX1_WAIT_1        0x95
#define  COMM_TAG_I2CM_AUX2_RST           0x96
#define  COMM_TAG_I2CM_AUX2_SET           0x97
#define  COMM_TAG_I2CM_AUX2_OUT           0x98
#define  COMM_TAG_I2CM_AUX2_IN            0x99
#define  COMM_TAG_I2CM_AUX2_WAIT_0        0x9A
#define  COMM_TAG_I2CM_AUX2_WAIT_1        0x9B
#define  COMM_TAG_I2CM_SPECIAL_1          0x9C



#define  COMM_TAG_I2CM_EVENT_START        0x80
#define  COMM_TAG_I2CM_EVENT_STOP         0x81
#define  COMM_TAG_I2CM_EVENT_RESTART      0x82
#define  COMM_TAG_I2CM_EVENT_ACK_TX       0x83
#define  COMM_TAG_I2CM_EVENT_NACK_TX      0x84
#define  COMM_TAG_I2CM_EVENT_ACK_RX       0x85
#define  COMM_TAG_I2CM_EVENT_NACK_RX      0x86
#define  COMM_TAG_I2CM_EVENT_BYTE_WRITE   0x87
#define  COMM_TAG_I2CM_EVENT_BYTE_READ    0x88
#define  COMM_TAG_I2CM_XACT_ERR           0x89
#define  COMM_TAG_I2CM_STATUS_ERR         0x8A

#define  COMM_I2CM_SSPCON1_DFLT     0x28;
#define  COMM_I2CM_SSPCON1_IDLE     0x00;
//#define  COMM_I2CM_SSPADD_DFLT      0x7f;


//==========================================================
//--- COMM I2CM CONTROL BLOCK
//==========================================================
#define  COMM_CB_I2CM_EVENT_START_TX_FLAG    exec_control_B[16].bit0
#define  COMM_CB_I2CM_EVENT_STOP_TX_FLAG     exec_control_B[16].bit1
#define  COMM_CB_I2CM_EVENT_RESTART_TX_FLAG  exec_control_B[16].bit2
#define  COMM_CB_I2CM_EVENT_ACK_TX_FLAG      exec_control_B[16].bit3
//#define  COMM_CB_I2CM_EVENT_NACK_TX_FLAG     exec_control_B[16].bit4
#define  COMM_CB_I2CM_EVENT_ACK_RX_FLAG      exec_control_B[16].bit5
//#define  COMM_CB_I2CM_EVENT_NACK_RX_FLAG     exec_control_B[16].bit6
#define  COMM_CB_I2CM_EVENT_BYTE_TX_FLAG     exec_control_B[16].bit7

#define  COMM_CB_I2CM_EVENT_BYTE_RX_FLAG     exec_control_B[17].bit0
#define  COMM_CB_I2CM_EVENT_XACT_ERR_FLAG    exec_control_B[17].bit1
#define  COMM_CB_I2CM_EVENT_STATUS_ERR_FLAG  exec_control_B[17].bit2

#define  COMM_CB_I2CM_AUX1_PORT_FLAG         exec_control_B[21].bit0
#define  COMM_CB_I2CM_AUX2_PORT_FLAG         exec_control_B[21].bit1
#define  COMM_CB_I2CM_AUX1_TRIS_FLAG         exec_control_B[21].bit2
#define  COMM_CB_I2CM_AUX2_TRIS_FLAG         exec_control_B[21].bit3

#define  COMM_CB_I2CM_BITRATE_REG            exec_control_B[23]._byte

//==========================================================
//--- COMM I2CM STATUS BLOCK
//==========================================================
#define  COMM_SB_I2CM_WCOL_FLAG              exec_status_B[12].bit0
#define  COMM_SB_I2CM_SSPOV_FLAG             exec_status_B[12].bit1
#define  COMM_SB_I2CM_NACK_FLAG              exec_status_B[12].bit2
#define  COMM_SB_I2CM_ERR_FLAG               exec_status_B[12].bit7
#define  COMM_SB_I2CM_ERR_REG                exec_status_B[12]._byte
#define  COMM_SB_I2CM_ERR_REG_MASK           (TD_BYTE)0x7F

#define  COMM_SB_I2CM_AUX1_PORT_FLAG         exec_status_B[16].bit0
#define  COMM_SB_I2CM_AUX2_PORT_FLAG         exec_status_B[16].bit1
#define  COMM_SB_I2CM_AUX1_TRIS_FLAG         exec_status_B[16].bit2
#define  COMM_SB_I2CM_AUX2_TRIS_FLAG         exec_status_B[16].bit3

#define  COMM_SB_I2CM_ADC_SDA_REG            exec_status_B[17]._byte
#define  COMM_SB_I2CM_ADC_SCL_REG            exec_status_B[18]._byte
#define  COMM_SB_I2CM_BITRATE_REG            exec_status_B[19]._byte

#define  COMM_SB_I2CM_REG12                  exec_status_B[12]._byte
#define  COMM_SB_I2CM_REG13                  exec_status_B[13]._byte
#define  COMM_SB_I2CM_REG14                  exec_status_B[14]._byte
#define  COMM_SB_I2CM_REG15                  exec_status_B[15]._byte
#define  COMM_SB_I2CM_REG16                  exec_status_B[16]._byte
#define  COMM_SB_I2CM_REG17                  exec_status_B[17]._byte
#define  COMM_SB_I2CM_REG18                  exec_status_B[18]._byte
#define  COMM_SB_I2CM_REG19                  exec_status_B[19]._byte

//------------------------------------------------
//--- ADC:
//------------------------------------------------
#define  COMM_I2CM_ADCON0_SDA    0x07  // CH:1
#define  COMM_I2CM_ADCON0_SCL    0x0B  // CH:2
#define  COMM_I2CM_ADCON2        0x3D  // JUST:L,TAQ:20TAD,TAD:F/16

//--- TIMER
#define  COMM_I2CM_ADC_TIMER        timer0_A1
#define  COMM_I2CM_ADC_INTV_MSEC    100
#define  COMM_I2CM_ADC_INTV_TA      COMM_I2CM_ADC_INTV_MSEC * 1000.0 / TIMER0_A_USEC

#endif // __COMM_I2CM_H
