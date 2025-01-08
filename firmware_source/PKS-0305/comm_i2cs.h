//===================================================================== 
//--- COPYRIGHT
//===================================================================== 
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
//===================================================================== 
//--- TITLE
//===================================================================== 
//    Filename:            i2cs.h
//    Microcontroller:     PIC18F2550
//    Compiled using:      C18 V3.00
//
//    Author:              Mark Enochson
//    Company:             Microchip Technology Inc.
//
//===================================================================== 
//--- DESCRIPTION
//===================================================================== 
//
//===================================================================== 
//--- HISTORY
//===================================================================== 
//
//    0001 - 08-01-06 - ME
//    - alpha release
//
//===================================================================== 
#ifndef __COMM_I2CS_H
#define __COMM_I2CS_H


//==========================================================
//--- RESOURCES
//==========================================================
#define  I2CS_PULLUP           LATAbits.LATA4    // get rid of this
#define  I2CS_SCK              LATBbits.LATB1
#define  I2CS_SDA              LATBbits.LATB0
#define  I2CS_SCK_TRIS         TRISBbits.TRISB1     
#define  I2CS_SDA_TRIS         TRISBbits.TRISB0     

#define  COMM_IO_1B_PIN    PORTAbits.RA5
#define  COMM_IO_4B_PIN    PORTAbits.RA1
#define  COMM_IO_5B_PIN    PORTAbits.RA2
#define  COMM_IO_6B_PIN    PORTAbits.RA3

#define  COMM_IO_1B_DIR    TRISAbits.TRISA5
#define  COMM_IO_4B_DIR    TRISAbits.TRISA1
#define  COMM_IO_5B_DIR    TRISAbits.TRISA2
#define  COMM_IO_6B_DIR    TRISAbits.TRISA3


//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
#define  COMM_STATE_I2CS_MIN              0xC0

#define  COMM_TAG_I2CS_INIT               0xC0
#define  COMM_TAG_I2CS_SEND_BYTES         0xC1
#define  COMM_TAG_I2CS_REGDATA_LOAD       0xC2
#define  COMM_TAG_I2CS_REGDATA_RETURN     0xC3

#define  COMM_TAG_I2CS_EVENT_ADDR         0xC0
#define  COMM_TAG_I2CS_EVENT_DATA_RX      0xC1
#define  COMM_TAG_I2CS_EVENT_DATA_TX      0xC2
#define  COMM_TAG_I2CS_EVENT_ACK_RX       0xC3
#define  COMM_TAG_I2CS_EVENT_NACK_RX      0xC4
#define  COMM_TAG_I2CS_START              0xC5
#define  COMM_TAG_I2CS_STOP               0xC6
#define  COMM_TAG_I2CS_EVENT_STATUS_ERR   0xC7
#define  COMM_TAG_I2CS_EVENT_DATA_RQ      0xC8
#define  COMM_TAG_I2CS_EVENT_REG_READ     0xC9
#define  COMM_TAG_I2CS_EVENT_REG_WRITE    0xCA
#define  COMM_TAG_I2CS_REGDATA            0xCB

//--- 7-BIT SLAVE, NO INT ON S&P
#define  I2CS_SSPCON1_DFLT    0x16

//--- 7-BIT SLAVE, INT ON S&P
//#define  I2CS_SSPCON1_DFLT    0x0E

#define  I2CS_SSPCON2_DFLT       0x01
#define  I2CS_SSPCON2_DFLT_MASK  0x3E

//------------------------------------------------
//--- COMM I2CM CONTROL BLOCK
//------------------------------------------------
#define  COMM_CB_I2CS_MODE_REG               exec_control_B[16]._b

#define  COMM_CB_I2CS_EVENTEN_ADDR_RX_FLAG   exec_control_B[17].bit0
#define  COMM_CB_I2CS_EVENTEN_DATA_RX_FLAG   exec_control_B[17].bit1
#define  COMM_CB_I2CS_EVENTEN_DATA_TX_FLAG   exec_control_B[17].bit2
#define  COMM_CB_I2CS_EVENTEN_ACK_RX_FLAG    exec_control_B[17].bit3
#define  COMM_CB_I2CS_EVENTEN_NACK_RX_FLAG   exec_control_B[17].bit4
#define  COMM_CB_I2CS_EVENTEN_REGREAD_FLAG   exec_control_B[17].bit5
#define  COMM_CB_I2CS_EVENTEN_REGWRITE_FLAG  exec_control_B[17].bit6
#define  COMM_CB_I2CS_EVENTEN_STATUSERR_FLAG exec_control_B[17].bit7

#define  COMM_CB_I2CS_EVENTEN_START_FLAG     exec_control_B[18].bit0
#define  COMM_CB_I2CS_EVENTEN_STOP_FLAG      exec_control_B[18].bit1
#define  COMM_CB_I2CS_EVENTEN_DATA_RQ_FLAG   exec_control_B[18].bit2

#define  COMM_CB_I2CS_DATA_N_DFLT_REG        exec_control_B[20]._b
#define  COMM_CB_I2CS_DATA_0_DFLT_REG        exec_control_B[21]._b
#define  COMM_CB_I2CS_SLAVE_ADDR_REG         exec_control_B[22]._b
#define  COMM_CB_I2CS_SLAVE_ADDR_MASK_REG    exec_control_B[23]._b

//------------------------------------------------
//--- COMM I2CM STATUS BLOCK
//------------------------------------------------
#define  COMM_SB_I2CS_PROFILE_ERR_FLAG       exec_status_B[12].bit0
#define  COMM_SB_I2CS_SSPOV_FLAG             exec_status_B[12].bit5
#define  COMM_SB_I2CS_WCOL_FLAG              exec_status_B[12].bit6
#define  COMM_SB_I2CS_ERR_FLAG               exec_status_B[12].bit7
#define  COMM_SB_I2CS_ERR_REG                exec_status_B[12]._b
#define  COMM_SB_I2CS_ERR_REG_MASK           0x7F

#define  COMM_SB_I2CS_SCK_STATE_FLAG         exec_status_B[13].bit0
#define  COMM_SB_I2CS_SDA_STATE_FLAG         exec_status_B[13].bit1
//#define  COMM_SB_I2CS_SCK_HELD_FLAG          exec_status_B[13].bit2
//#define  COMM_SB_I2CS_DATA_RQ_FLAG           exec_status_B[13].bit7

//------------------------------------------------
//--- VARBS USING SHARED RAM
//------------------------------------------------
#define  COMM_I2CS_FLAGS                     comm_varbs_B[0]._b

#define  COMM_I2CS_ADDR_FLAG                 comm_varbs_B[0].bit0
#define  COMM_I2CS_READ_FLAG                 comm_varbs_B[0].bit1
#define  COMM_I2CS_READ_DATA_RQ_FLAG         comm_varbs_B[0].bit2    // mode: 1
#define  COMM_I2CS_TEMP_2_FLAG               comm_varbs_B[0].bit2    // mode: 2
#define  COMM_I2CS_READ_DATA_RDY_FLAG        comm_varbs_B[0].bit3    // mode: 1
#define  COMM_I2CS_TEMP_3_FLAG               comm_varbs_B[0].bit3    // mode: 2
#define  COMM_I2CS_PRO_SLAVE_OK_FLAG         comm_varbs_B[0].bit4
#define  COMM_I2CS_PRO_REG_OK_FLAG           comm_varbs_B[0].bit5
#define  COMM_I2CS_EVENT_PENDING_FLAG        comm_varbs_B[0].bit6
#define  COMM_I2CS_TEMP_FLAG                 comm_varbs_B[0].bit7

#define  COMM_I2CS_DATA                      comm_varbs_B[1]._b
#define  COMM_I2CS_ADDR                      comm_varbs_B[2]._b
#define  COMM_I2CS_CMND                      comm_varbs_B[3]._b
#define  COMM_I2CS_REG                       comm_varbs_B[3]._b
#define  COMM_I2CS_TEMP                      comm_varbs_B[4]._b
#define  COMM_I2CS_PRO_TAG                   comm_varbs_B[5]._b
#define  COMM_I2CS_PRO_SLAVE_ADDR            comm_varbs_B[6]._b
#define  COMM_I2CS_PRO_SLAVE_REG_INDEX       comm_varbs_B[7]._b
#define  COMM_I2CS_PRO_SLAVE_REGS_INDEX      comm_varbs_B[8]._b
#define  COMM_I2CS_PRO_REG_FLAGS             comm_varbs_B[9]._b
#define  COMM_I2CS_PRO_REG                   comm_varbs_B[10]._b
#define  COMM_I2CS_PRO_REG_BC                comm_varbs_B[11]._b
#define  COMM_I2CS_PRO_REG_INDEX             comm_varbs_B[12]._b

#define  COMM_I2CS_TAGDATA_1                 comm_varbs_B[13]._b
#define  COMM_I2CS_TAGDATA_2                 comm_varbs_B[14]._b

#define  COMM_I2CS_FLAGS2                    comm_varbs_B[15]._b
#define  COMM_I2CS_XSTARTED_FLAG             comm_varbs_B[15].bit0

#define  COMM_I2CS_PRO_TAG_UNKNOWN           0x00
#define  COMM_I2CS_PRO_TAG_SLAVE             0x20
#define  COMM_I2CS_PRO_TAG_REGDEF            0x40
#define  COMM_I2CS_PRO_TAG_END               0x80

#define  COMM_I2CS_PRO_REG_WRDIS_FLAG        comm_varbs_B[9].bit0
#define  COMM_I2CS_PRO_REG_RDDIS_FLAG        comm_varbs_B[9].bit1
#define  COMM_I2CS_PRO_REG_BLKMODE_FLAG      comm_varbs_B[9].bit2
#define  COMM_I2CS_PRO_REG_SEQ_FLAG          comm_varbs_B[9].bit3

#define  COMM_I2CS_DATA_READ_DEFAULT         0xFF

//------------------------------------------------
//--- PROTOTYPES
//------------------------------------------------
TD_BYTE comm_i2cs_init(void);
void comm_i2cs_reset(void);
void comm_i2cs_svc(void);
void comm_i2cs_postproc(void);
void comm_i2cs_isr(void);
void comm_i2cs_data_send(void);
void comm_i2cs_pro_get_init(void);
void comm_i2cs_pro_get(void);
void comm_i2cs_pro_get_slave(void);
void comm_i2cs_pro_get_regdef(void);
void comm_i2cs_dint(void);

#endif // __COMM_I2CS_H
