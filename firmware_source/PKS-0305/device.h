#ifndef __DEVICE_H
#define __DEVICE_H

//#define  DEVICE_USBDEV1
#define  DEVICE_PKS_1
//#define  DEVICE_PKS_2

#ifdef DEVICE_USBDEV1
#include <p18f4550.h>
#endif

#ifdef DEVICE_PKS_1
#include <p18f2550.h>
#endif

#ifdef DEVICE_PKS_2
#include <p18f2450.h>
#endif

//================================================
//--- DEFINITIONS
//================================================

//--------------------------------------
//--- CLOCK
//--------------------------------------
//#define _FOSC_10MHZ
#define _FOSC_20MHZ

//--------------------------------------
//--- TIMER0
//--------------------------------------
#ifdef   _FOSC_10MHZ
#define  TIMER0_T0CON_DFLT    0b11000001
#define  TIMER0_A_USEC        409.6
#define  _FOSC_MHZ            10.000
#endif

#ifdef   _FOSC_20MHZ
#define  TIMER0_T0CON_DFLT    0b11000010
#define  TIMER0_A_USEC        409.6
#define  _FOSC_MHZ            20.000
#endif

//--------------------------------------
//---
//--------------------------------------
#ifdef   _FOSC_10MHZ
#define  I2CBBM_ENGINE_T1CON_DFLT  0b10010001  // OSC:FOSC/4, PS:2
#endif

#ifdef   _FOSC_20MHZ
#define  I2CBBM_ENGINE_T1CON_DFLT  0b10100001  // OSC:FOSC/4, PS:4
#endif

//--------------------------------------
//--- I/O
//--------------------------------------
#define  LATA_DFLT            0b000000;
#define  LATB_DFLT            0b00000000;
#define  LATC_DFLT            0b00000000;

#define  TRISA_DFLT           0b000000;
#define  TRISB_DFLT           0b00000000;
#define  TRISC_DFLT           0b00000000;

//--------------------------------------
//--- SWITCH
//--------------------------------------
#define  SW_PIN               PORTBbits.RB5
#define  SW_PIN_DIR           TRISBbits.TRISB5

//--------------------------------------
//--- LEDs
//--------------------------------------
#define  LED_1_PIN             LATCbits.LATC0
#define  LED_2_PIN             LATBbits.LATB4
#define  LED_1_TRIS            TRISCbits.TRISC0
#define  LED_2_TRIS            TRISBbits.TRISB4


#endif // __DEFINE_DEVICE_H
