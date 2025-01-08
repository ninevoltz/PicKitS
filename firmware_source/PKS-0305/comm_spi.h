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
//    Filename:            comm_spi.h
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
#ifndef __COMM_SPI_H
#define __COMM_SPI_H


//==========================================================
//--- RESOURCES
//==========================================================
#define  COMM_SPI_SDI_TRIS    TRISBbits.TRISB0     // serial data in - dir
#define  COMM_SPI_SDO_TRIS    TRISCbits.TRISC7     // serial data out - dir
#define  COMM_SPI_SCK_TRIS    TRISBbits.TRISB1     // serial clock - dir
#define  COMM_SPI_CS_TRIS     TRISCbits.TRISC6     // chip select - dir
#define  COMM_SPI_AUX1_TRIS   TRISCbits.TRISC6     // aux1 - dir

#define  COMM_SPI_SDI_PIN     LATBbits.LATB0       // serial data in - pin
#define  COMM_SPI_SDO_PIN     LATCbits.LATC7       // serial data out - pin
#define  COMM_SPI_SCK_PIN     LATBbits.LATB1       // serial clock - pin
#define  COMM_SPI_CS_PIN      LATCbits.LATC6       // chip select - pin
#define  COMM_SPI_AUX1_PIN    PORTCbits.RC6        // aux1 - pin

//---

#define  COMM_SPI_SDI_2_TRIS  TRISAbits.TRISA1     //
#define  COMM_SPI_SDO_2_TRIS  TRISAbits.TRISA3     //
#define  COMM_SPI_SCK_2_TRIS  TRISAbits.TRISA2     //
#define  COMM_SPI_CS_2_TRIS   TRISAbits.TRISA5     //
#define  COMM_SPI_AUX1_2_TRIS TRISAbits.TRISA5     //

#define  COMM_SPI_SDI_2_PIN   PORTAbits.RA1        //
#define  COMM_SPI_SDO_2_PIN   PORTAbits.RA3        //
#define  COMM_SPI_SCK_2_PIN   PORTAbits.RA2        //
#define  COMM_SPI_CS_2_PIN    PORTAbits.RA5        //
#define  COMM_SPI_AUX1_2_PIN  PORTAbits.RA5        //



//==========================================================
//--- PROTOTYPES
//==========================================================
TD_BYTE comm_spi_init(void);     // initialize per CONTROL_BLOCK
void comm_spi_reset(void);       // reset to IDLE state
void comm_spi_svc(void);         // script processing service
void comm_spi_postproc(void);    // post-processing service
void comm_spi_config(void);      //
void comm_spi_clear(void);       //

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================

//--- INCOMING DATA TAG(S)
#define  COMM_TAG_SPI_BITRATE                0x83
#define  COMM_TAG_SPI_DATAIO_IN              0x84
#define  COMM_TAG_SPI_DATAIO_OUT             0x85
#define  COMM_TAG_SPI_DATAIO_INOUT           0x86
#define  COMM_TAG_SPI_SDO_IN                 0x87
#define  COMM_TAG_SPI_SDO_OUT                0x88
#define  COMM_TAG_SPI_INIT                   0x89
//#define  COMM_TAG_SPI_RESET                  0x8A
#define  COMM_TAG_SPI_CS_ON                  0x8B
#define  COMM_TAG_SPI_CS_OFF                 0x8C

#define  COMM_TAG_SPI_AUX1_RST               0x90
#define  COMM_TAG_SPI_AUX1_SET               0x91
#define  COMM_TAG_SPI_AUX1_OUT               0x92
#define  COMM_TAG_SPI_AUX1_IN                0x93
#define  COMM_TAG_SPI_AUX1_WAIT_0            0x94
#define  COMM_TAG_SPI_AUX1_WAIT_1            0x95

//--- PROCESSING "STATES" (in addition to TAGs above)
#define  COMM_STATE_SPI_DATAIO               0xFF

//--- OUTGOING DATA TAG(S)
#define  COMM_TAG_SPI_EVENT_BYTE_TX          0x80
#define  COMM_TAG_SPI_EVENT_BYTE_RX          0x81
#define  COMM_TAG_SPI_EVENT_STATUS_ERR       0x82

//--- SSPCON1 MODE CODES
#define  SSPCON1_SPI_MODE_IDLE               0x00
#define  SSPCON1_SPI_MODE_MASTER             0x23
#define  SSPCON1_SPI_MODE_SLAVE_SS           0x04
#define  SSPCON1_SPI_MODE_SLAVE_NOSS         0x05

//------------------------------------------------
//--- COMM SPI CONTROL BLOCK
//------------------------------------------------
#define  COMM_CB_SPI_EVENT_BYTE_RX_FLAG      exec_control_B[16].bit0
#define  COMM_CB_SPI_EVENT_BYTE_TX_FLAG      exec_control_B[16].bit1
#define  COMM_CB_SPI_EVENT_STATUS_ERR_FLAG   exec_control_B[16].bit2

#define  COMM_CB_SPI_CONFIG_REG              exec_control_B[17]._byte
#define  COMM_CB_SPI_CONFIG_SMP_FLAG         exec_control_B[17].bit0
#define  COMM_CB_SPI_CONFIG_CKE_FLAG         exec_control_B[17].bit1
#define  COMM_CB_SPI_CONFIG_CKP_FLAG         exec_control_B[17].bit2
#define  COMM_CB_SPI_CONFIG_DAOD_FLAG        exec_control_B[17].bit3
#define  COMM_CB_SPI_CONFIG_CSPOL_FLAG       exec_control_B[17].bit7

#define  COMM_CB_SPI_AUX1_REG                exec_control_B[21]._byte
#define  COMM_CB_SPI_AUX1_ENABLE_FLAG        exec_control_B[21].bit0
#define  COMM_CB_SPI_AUX1_STATE_FLAG         exec_control_B[21].bit1
#define  COMM_CB_SPI_AUX1_DIR_FLAG           exec_control_B[21].bit2

#define  COMM_CB_SPI_BITRATE_PRESCALE_REG    exec_control_B[22]._byte
#define  COMM_CB_SPI_BITRATE_SCALE_REG       exec_control_B[23]._byte

//------------------------------------------------
//--- COMM SPI STATUS BLOCK
//------------------------------------------------
#define  COMM_SB_SPI_WCOL_FLAG               exec_status_B[12].bit0
#define  COMM_SB_SPI_SSPOV_FLAG              exec_status_B[12].bit1
#define  COMM_SB_SPI_ERR_FLAG                exec_status_B[12].bit7

#define  COMM_SB_SPI_ERR_REG                 exec_status_B[12]._byte
#define  COMM_SB_SPI_ERR_REG_MASK            (TD_BYTE)0x7F

#define  COMM_SB_SPI_CONFIG_REG              exec_status_B[13]._byte
#define  COMM_SB_SPI_CONFIG_SMP_FLAG         exec_status_B[13].bit0
#define  COMM_SB_SPI_CONFIG_CKE_FLAG         exec_status_B[13].bit1
#define  COMM_SB_SPI_CONFIG_CKP_FLAG         exec_status_B[13].bit2
#define  COMM_SB_SPI_CONFIG_AOD_FLAG         exec_status_B[13].bit3

#define  COMM_SB_SPI_SDI_STATE_FLAG          exec_status_B[14].bit0
#define  COMM_SB_SPI_SDO_STATE_FLAG          exec_status_B[14].bit1
#define  COMM_SB_SPI_SCK_STATE_FLAG          exec_status_B[14].bit2
#define  COMM_SB_SPI_CS_STATE_FLAG           exec_status_B[14].bit3

#define  COMM_SB_SPI_AUX1_REG                exec_status_B[16]._byte
#define  COMM_SB_SPI_AUX1_ENABLE_FLAG        exec_status_B[16].bit0
#define  COMM_SB_SPI_AUX1_STATE_FLAG         exec_status_B[16].bit1
#define  COMM_SB_SPI_AUX1_DIR_FLAG           exec_status_B[16].bit2

#define  COMM_SB_SPI_REG12                   exec_status_B[12]._byte
#define  COMM_SB_SPI_REG13                   exec_status_B[13]._byte
#define  COMM_SB_SPI_REG14                   exec_status_B[14]._byte
#define  COMM_SB_SPI_REG15                   exec_status_B[15]._byte
#define  COMM_SB_SPI_REG16                   exec_status_B[16]._byte
#define  COMM_SB_SPI_REG17                   exec_status_B[17]._byte
#define  COMM_SB_SPI_REG18                   exec_status_B[18]._byte
#define  COMM_SB_SPI_REG19                   exec_status_B[19]._byte

#define  COMM_SB_SPI_BITRATE_PRESCALE_REG    exec_status_B[18]._byte
#define  COMM_SB_SPI_BITRATE_SCALE_REG       exec_status_B[19]._byte



#endif // __COMM_SPI_H
