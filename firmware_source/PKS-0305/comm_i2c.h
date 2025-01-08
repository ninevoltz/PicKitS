#ifndef _COMM_I2C_H
#define _COMM_I2C_H

#define  COMM_STATE_I2C_M_INIT         0xFF
#define  COMM_STATE_I2C_M_START        0xFE
#define  COMM_STATE_I2C_M_STOP         0xFD
#define  COMM_STATE_I2C_M_RESTART      0xFC
#define  COMM_STATE_I2C_M_WRITE_BYTES  0xFB
#define  COMM_STATE_I2C_M_READ_BYTES   0xFA
#define  COMM_STATE_I2C_M_READ_BLOCK   0xF9
#define  COMM_STATE_I2C_M_BITRATE      0xF8

#endif // _COMM_I2C_H
