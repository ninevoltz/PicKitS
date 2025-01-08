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
//    Filename:            i2cs.c
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
//    I2C slave

//===================================================================== 
//--- HISTORY
//===================================================================== 
//
//    0001 - 08-01-06 - ME
//    - alpha release
//
//===================================================================== 

//==========================================================
//--- INCLUDES
//==========================================================
#include "project.h"
#include "cbuf.h"
#include "comm_gen.h"
#include "comm_i2cs.h"

//==========================================================
//--- DEFINITIONS / DECLARATIONS
//==========================================================
// none


//==========================================================
//--- comm_i2cs_init()
//
//    initialization
//==========================================================
TD_BYTE comm_i2cs_init(void)
{
   comm_varbs_clear();
   comm_i2cs_reset();
   return(0);
}

//==========================================================
//--- comm_i2cs_reset()
//
//    hardware "reset"
//==========================================================
void comm_i2cs_reset(void)
{
   SSPCON1 = 0;
   PIE1bits.SSPIE = 0;
   COMM_I2CS_FLAGS = 0;
   I2CS_SCK = 1;
   I2CS_SDA = 1;
   I2CS_SCK_TRIS = 1;
   I2CS_SDA_TRIS = 1;
   COMM_IO_4B_DIR = 1;
   COMM_IO_5B_DIR = 1;
   SSPADD = COMM_CB_I2CS_SLAVE_ADDR_REG;
   SSPCON2 = I2CS_SSPCON2_DFLT | (COMM_CB_I2CS_SLAVE_ADDR_MASK_REG & I2CS_SSPCON2_DFLT_MASK) ;
   SSPCON1 = I2CS_SSPCON1_DFLT;
   PIR1bits.SSPIF = 0;
   IPR1bits.SSPIP = 0;
   PIE1bits.SSPIE = 1;
   SSPCON1bits.SSPEN = 1;
   
/*
   //--------------------------------------
   //--- MODE 2 - SLAVE PROFILE
   //--------------------------------------
   cbuf3_put(0x20);  // slave
   cbuf3_put(0x16);
   cbuf3_put(0x00);
   cbuf3_put(0x00);
   
   cbuf3_put(0x40);  // register - read only
   cbuf3_put(0x21);
   cbuf3_put(0x02);  // byte count
   cbuf3_put(0x34);
   cbuf3_put(0x12);
   
   cbuf3_put(0x40);  // register rd/wr
   cbuf3_put(0x22);
   cbuf3_put(0x01);  // byte count
   cbuf3_put(0x56);

   cbuf3_put(0x20);  // slave
   cbuf3_put(0x18);
   cbuf3_put(0x00);
   cbuf3_put(0x00);

   cbuf3_put(0x48);  // register rd/wr, block
   cbuf3_put(0x00);  // addr=0
   cbuf3_put(0x04);  // byte count
   cbuf3_put(0x11);
   cbuf3_put(0x22);
   cbuf3_put(0x33);
   cbuf3_put(0x44);
   
   cbuf3_put(0x80);
*/
/*   
   //--------------------------------------
   //--- SCRIPT COMMAND
   //--------------------------------------
   cbuf1_put(0xC3);
   cbuf1_put(0x16);
   cbuf1_put(0x21);         
*/
/*
   cbuf1_put(0xC1);
   cbuf1_put(0x03);
   cbuf1_put(0x56);
   cbuf1_put(0x78);
   cbuf1_put(0x9A);
*/

}
 
//==========================================================
//--- comm_i2cs_pro_get_regdef()
//    search profile for register definition
//    it is assumed that the SLAVE addr has been located
//    so the address of the regdef(s) is known
//    COMM_I2CS_PRO_REG_OK_FLAG = 1 (regdef found)
//    COMM_I2CS_PRO_REG_OK_FLAG = 0 (regdef not found)
//==========================================================
void comm_i2cs_pro_get_regdef(void)
{
   COMM_I2CS_TEMP = COMM_I2CS_PRO_SLAVE_REGS_INDEX;
   COMM_I2CS_PRO_REG_OK_FLAG = 0;
   while(!COMM_SB_I2CS_PROFILE_ERR_FLAG)
   {
      comm_i2cs_pro_get();
      if(COMM_I2CS_PRO_TAG != COMM_I2CS_PRO_TAG_REGDEF)
         break;
      if(COMM_I2CS_PRO_REG_SEQ_FLAG)
      {
         if((COMM_I2CS_CMND >= COMM_I2CS_PRO_REG) && 
            (COMM_I2CS_CMND < (COMM_I2CS_PRO_REG + COMM_I2CS_PRO_REG_BC)))
         {
            COMM_I2CS_PRO_REG = COMM_I2CS_CMND - COMM_I2CS_PRO_REG;
            COMM_I2CS_PRO_REG_BC -= COMM_I2CS_PRO_REG;
            COMM_I2CS_PRO_REG_INDEX += COMM_I2CS_PRO_REG;
            COMM_I2CS_PRO_REG = COMM_I2CS_CMND;
            COMM_I2CS_PRO_REG_OK_FLAG = 1;
            break;
         }
      }
      else
      {
         if(COMM_I2CS_PRO_REG == COMM_I2CS_CMND)
         {
            COMM_I2CS_PRO_REG_OK_FLAG = 1;
            break;
         }
      }
   }
}

//==========================================================
//--- comm_i2cs_pro_get_slave()
//    search profile for slave address: COMM_I2CS_ADDR
//    COMM_I2CS_PRO_OK_FLAG = 1 (slave addr found)
//    COMM_I2CS_PRO_OK_FLAG = 0 (slave addr not found)
//==========================================================
void comm_i2cs_pro_get_slave(void)
{
   COMM_I2CS_TEMP = 1;
   COMM_I2CS_PRO_SLAVE_OK_FLAG = 0;
   COMM_I2CS_PRO_REG_OK_FLAG = 0;
   do
   {
      comm_i2cs_pro_get();
      if(COMM_I2CS_PRO_TAG == COMM_I2CS_PRO_TAG_SLAVE)
      {
         if(COMM_I2CS_PRO_SLAVE_ADDR == COMM_I2CS_ADDR)
         {
            COMM_I2CS_PRO_SLAVE_OK_FLAG = 1;
            break;
         }
      }
   } while((COMM_I2CS_PRO_TAG != COMM_I2CS_PRO_TAG_END)&&(!COMM_SB_I2CS_PROFILE_ERR_FLAG));
}

//==========================================================
//--- comm_i2cs_pro_get() - get next entity in profile
//    scan slave profile, return one TAG at a time
//    (profile assumed to be properly formed)
//==========================================================
void comm_i2cs_pro_get(void)
{
   COMM_I2CS_PRO_TAG = cbuf3_data[COMM_I2CS_TEMP];

   if(COMM_I2CS_PRO_TAG != COMM_I2CS_PRO_TAG_END)
   {
      COMM_I2CS_TEMP++;
      if(COMM_I2CS_PRO_TAG == COMM_I2CS_PRO_TAG_SLAVE)
      {
         COMM_I2CS_PRO_SLAVE_ADDR = cbuf3_data[COMM_I2CS_TEMP];
         COMM_I2CS_TEMP += 2;
         COMM_I2CS_PRO_SLAVE_REG_INDEX = COMM_I2CS_TEMP;
         COMM_I2CS_TEMP ++;
         COMM_I2CS_PRO_SLAVE_REGS_INDEX = COMM_I2CS_TEMP;
      }
      else if(COMM_I2CS_PRO_TAG_REGDEF == (COMM_I2CS_PRO_TAG & 0xF0))
      {
         COMM_I2CS_PRO_REG_FLAGS = COMM_I2CS_PRO_TAG & 0x0F;
         COMM_I2CS_PRO_TAG &= 0xF0;
         COMM_I2CS_PRO_REG = cbuf3_data[COMM_I2CS_TEMP];
         COMM_I2CS_TEMP ++;
         COMM_I2CS_PRO_REG_BC = cbuf3_data[COMM_I2CS_TEMP];
         COMM_I2CS_TEMP ++;
         COMM_I2CS_PRO_REG_INDEX = COMM_I2CS_TEMP;
         COMM_I2CS_TEMP += COMM_I2CS_PRO_REG_BC;
      }
      else
      {
         COMM_SB_I2CS_PROFILE_ERR_FLAG = 1;
      }
   }
}
 
//==========================================================
//--- ISR
//==========================================================
void comm_i2cs_isr(void)
{
   if(PIR1bits.SSPIF == 0)
      return;
   PIR1bits.SSPIF = 0;
   
   //---------------------------------------------
   //--- BUFFER FULL (ADDR/DATA AVAIL)
   //---------------------------------------------
   if(SSPSTATbits.BF)
   {
      //--------------------------------
      //--- DATA RECEIVED
      //--------------------------------
      if(SSPSTATbits.D)
      {
         COMM_I2CS_DATA = SSPBUF;
         if(COMM_CB_I2CS_EVENTEN_DATA_RX_FLAG)
            comm_post_event_1b(COMM_TAG_I2CS_EVENT_DATA_RX, COMM_I2CS_DATA);

         //--- IF 1ST BYTE, SAVE AS "CMND"
         if(COMM_I2CS_ADDR_FLAG)
         {
            COMM_I2CS_CMND = COMM_I2CS_DATA;

            //--- IF MODE 2, CHECK FOR 'REGDEF' PROFILE
            if(COMM_CB_I2CS_MODE_REG == 2)
            {
               if(COMM_I2CS_PRO_SLAVE_OK_FLAG)
               {
                  comm_i2cs_pro_get_regdef();
                  cbuf3_data[COMM_I2CS_PRO_SLAVE_REG_INDEX] = COMM_I2CS_CMND;
               }
            }

            COMM_I2CS_ADDR_FLAG = 0;
         }
         else
         {
   
            //--- IF MODE 2, CHECK FOR 'REGDEF' PROFILE
            if(COMM_CB_I2CS_MODE_REG == 2)
            {
               if(COMM_I2CS_PRO_REG_OK_FLAG)
               {
                  if(COMM_I2CS_PRO_REG_WRDIS_FLAG == 0)
                  {
                     if(COMM_I2CS_PRO_REG_BC)
                     {
                        cbuf3_data[COMM_I2CS_PRO_REG_INDEX] = COMM_I2CS_DATA;
                        COMM_I2CS_PRO_REG_INDEX++;
                        COMM_I2CS_PRO_REG_BC--;
                     }
                  }
                  if(COMM_I2CS_PRO_REG_SEQ_FLAG)
                     cbuf3_data[COMM_I2CS_PRO_SLAVE_REG_INDEX]++;
               }
               //--- 'WRITE REG' EVENT
               if((COMM_I2CS_EVENT_PENDING_FLAG) && (COMM_CB_I2CS_EVENTEN_REGWRITE_FLAG))
               {
                  COMM_I2CS_EVENT_PENDING_FLAG = 0;
                  comm_post_event_2b(COMM_TAG_I2CS_EVENT_REG_WRITE, COMM_I2CS_ADDR, COMM_I2CS_CMND);
               }
            }
         }
         
         SSPCON1bits.CKP = 1;
      }
      
      //--------------------------------
      //--- ADDR RECEIVED
      //--------------------------------
      else
      {
         COMM_I2CS_ADDR = SSPBUF;
         COMM_I2CS_XSTARTED_FLAG = 1;
         if(COMM_CB_I2CS_EVENTEN_ADDR_RX_FLAG)
            comm_post_event_1b(COMM_TAG_I2CS_EVENT_ADDR, COMM_I2CS_ADDR);
         COMM_I2CS_ADDR &= 0xFE;
         COMM_I2CS_ADDR_FLAG = 1;

         //-----------------------------
         //--- IF MODE 2, 
         //    IS SLAVE ADDR IN PROFILE
         //-----------------------------
         if(COMM_CB_I2CS_MODE_REG == 2)
         {
            COMM_I2CS_EVENT_PENDING_FLAG = 1;
            comm_i2cs_pro_get_slave();
         }

         //-----------------------------
         //--- READ ?!
         //-----------------------------
         if(SSPSTATbits.R)
         {
            COMM_I2CS_READ_FLAG = 1;
            if(COMM_CB_I2CS_MODE_REG == 2)
            {
               if(COMM_I2CS_PRO_SLAVE_OK_FLAG)
               {
                  COMM_I2CS_CMND = cbuf3_data[COMM_I2CS_PRO_SLAVE_REG_INDEX];
                  comm_i2cs_pro_get_regdef();
               }
               if(COMM_I2CS_EVENT_PENDING_FLAG)
               {
                  COMM_I2CS_EVENT_PENDING_FLAG = 0;
                  if(COMM_CB_I2CS_EVENTEN_REGREAD_FLAG)
                     comm_post_event_2b(COMM_TAG_I2CS_EVENT_REG_READ, COMM_I2CS_ADDR, COMM_I2CS_CMND);
               }
            }
         }

         //-----------------------------
         //--- WRITE ?!
         //-----------------------------
         else
         {
            COMM_I2CS_READ_FLAG = 0;
            SSPCON1bits.CKP = 1;
         }
      }
   }

   //---------------------------------------------
   //--- READ ACTIVE ?
   //---------------------------------------------
   
   //-----------------------------------   
   //--- IF 'READ' ACTIVE & CLOCK HI ... 
   //    NACK RECEIVED - READ DONE
   //-----------------------------------   
   if(COMM_I2CS_READ_FLAG)
   {
      if(SSPCON1bits.CKP == 0)
      {
         if(COMM_I2CS_ADDR_FLAG == 0)
         {
            if(COMM_CB_I2CS_EVENTEN_ACK_RX_FLAG)
               comm_post_event(COMM_TAG_I2CS_EVENT_ACK_RX);
         }
      }
      else
      {
         COMM_I2CS_READ_FLAG = 0;
         if(COMM_CB_I2CS_EVENTEN_NACK_RX_FLAG)
            comm_post_event(COMM_TAG_I2CS_EVENT_NACK_RX);
         if(COMM_CB_I2CS_MODE_REG == 1)
            cbuf3_init();
      }
   }
   
   //-----------------------------------   
   //--- READ ACTIVE
   //-----------------------------------   
   if(COMM_I2CS_READ_FLAG)
   {
      
      //----------------------
      //--- MODE: 0
      //----------------------
      if(COMM_CB_I2CS_MODE_REG == 0)
      {
         if(COMM_I2CS_ADDR_FLAG)
            COMM_I2CS_DATA = COMM_CB_I2CS_DATA_0_DFLT_REG;
         else
            COMM_I2CS_DATA = COMM_CB_I2CS_DATA_N_DFLT_REG;
         comm_i2cs_data_send();
      }
      
      //----------------------
      //--- MODE: 1
      //----------------------
      else if(COMM_CB_I2CS_MODE_REG == 1)
      {
         if(cbuf3.used)
         {
            COMM_I2CS_DATA = cbuf3_get();
            comm_i2cs_data_send();
         }
         else
         {
            COMM_I2CS_READ_DATA_RQ_FLAG = 1;
            if(COMM_CB_I2CS_EVENTEN_DATA_RQ_FLAG)
               comm_post_event(COMM_TAG_I2CS_EVENT_DATA_RQ);
         }
      }
      
      //----------------------
      //--- MODE: 2
      //----------------------
      else
      {
         COMM_I2CS_TEMP_FLAG = 0;
         if(COMM_I2CS_PRO_REG_OK_FLAG)
         {
            if(COMM_I2CS_PRO_REG_RDDIS_FLAG == 0)
            {
               if(COMM_I2CS_PRO_REG_BC)
               {
                  COMM_I2CS_DATA = cbuf3_data[COMM_I2CS_PRO_REG_INDEX];
                  comm_i2cs_data_send();
                  COMM_I2CS_PRO_REG_INDEX++;
                  COMM_I2CS_PRO_REG_BC--;
                  COMM_I2CS_TEMP_FLAG = 1;
               }
            }
            if(COMM_I2CS_PRO_REG_SEQ_FLAG)
               cbuf3_data[COMM_I2CS_PRO_SLAVE_REG_INDEX]++;
         }
         if(COMM_I2CS_TEMP_FLAG == 0)
         {
            COMM_I2CS_DATA = COMM_I2CS_DATA_READ_DEFAULT;
            comm_i2cs_data_send();
         }
      }

      //--- RST '1ST BYTE' FLAG ...
      COMM_I2CS_ADDR_FLAG = 0;
   }
}

//==========================================================
//--- SEND/RETURN DATA TO MASTER
//==========================================================
void comm_i2cs_data_send(void)
{
   SSPBUF = COMM_I2CS_DATA;
   SSPCON1bits.CKP = 1;
   if(COMM_CB_I2CS_EVENTEN_DATA_TX_FLAG)
      comm_post_event_1b(COMM_TAG_I2CS_EVENT_DATA_TX, COMM_I2CS_DATA);
}

//==========================================================
//--- comm_i2cs_postproc()
//
//    post-processing
//    set status error flag
//    set composite error flag
//==========================================================
void comm_i2cs_postproc(void)
{
   
   //---------------------------------------------
   //--- CHECK FOR 'STOP' EVENT
   //---------------------------------------------
   if(COMM_I2CS_XSTARTED_FLAG)
   {
      //--- disable interrupts
      do
      {
         INTCONbits.GIEL = 0; 
      } while(INTCONbits.GIEL);

      //--- STOP bit encountered
      if(SSPSTATbits.P == 1)
      {
         if(COMM_CB_I2CS_EVENTEN_STOP_FLAG)
            comm_post_event(COMM_TAG_I2CS_STOP);
         COMM_I2CS_XSTARTED_FLAG = 0;
      }
   }
   INTCONbits.GIEL = 1;
   
   //---------------------------------------------
   //--- CHECK COMM I/O PINS/STATE
   //---------------------------------------------
   if(COMM_IO_4B_PIN)
      COMM_SB_I2CS_SDA_STATE_FLAG = 1;
   else
      COMM_SB_I2CS_SDA_STATE_FLAG = 0;

   if(COMM_IO_5B_PIN)
      COMM_SB_I2CS_SCK_STATE_FLAG = 1;
   else
      COMM_SB_I2CS_SCK_STATE_FLAG = 0;
   
   /*
   if(SSPCON1bits.CKP)
      COMM_SB_I2CS_SCK_HELD_FLAG = 0;
   else
      COMM_SB_I2CS_SCK_HELD_FLAG = 1;
   */
      
   //---------------------------------------------
   //--- IF READING & DATA AVAIL ... SEED OUTPUT
   //---------------------------------------------
   
   if(COMM_CB_I2CS_MODE_REG == 1)
   {
      if(COMM_I2CS_READ_DATA_RQ_FLAG)
      {
         if(cbuf3.used)
         {
            COMM_I2CS_READ_DATA_RQ_FLAG = 0;
            COMM_I2CS_DATA = cbuf3_get();
            comm_i2cs_data_send();
         }
      }
   }      
   

   //-----------------------------------
   //--- transaction error
   //-----------------------------------
   COMM_4_FLAG = 0;
   if(SSPCON1bits.WCOL)
   {
      COMM_4_FLAG = 1;
      SSPCON1bits.WCOL = 0;
      COMM_SB_I2CS_WCOL_FLAG = 1;
   }
   if(SSPCON1bits.SSPOV)
   {
      COMM_4_FLAG = 1;
      SSPCON1bits.SSPOV = 0;
      COMM_SB_I2CS_SSPOV_FLAG = 1;
   }
/*
   if(COMM_4_FLAG)
   {
      if(COMM_CB_I2CS_EVENT_XACT_ERR_FLAG)
      {
         comm_post_event_1b(COMM_TAG_I2CS_XACT_ERR, COMM_SB_I2CS_ERR_REG);
      }
   }
*/
   
   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(COMM_SB_I2CS_ERR_REG & COMM_SB_I2CS_ERR_REG_MASK)
   {
      COMM_SB_GEN_ERR_FLAG = 1;
      COMM_SB_I2CS_ERR_FLAG = 1;
   }

   //-----------------------------------
   //--- change in error status
   //-----------------------------------
   if(COMM_CB_I2CS_EVENTEN_STATUSERR_FLAG)
   {
      if(COMM_SB_I2CS_ERR_REG != exec_status_save_3)
      {
         exec_status_save_3 = COMM_SB_I2CS_ERR_REG;
         comm_post_event_1b(COMM_TAG_I2CS_EVENT_STATUS_ERR, COMM_SB_I2CS_ERR_REG);
      }
   }
}


//==========================================================
//--- comm_i2cs_svc()
//==========================================================
void comm_i2cs_svc(void)
{

   //=======================================================
   //--- SWITCH CONTROLLER STATE
   //=======================================================
   switch(comm_state)
   {

      //====================================================
      case COMM_TAG_I2CS_INIT:
         comm_i2cs_reset();
         comm_state = COMM_STATE_GEN_NEXT;
         break;
         
      //====================================================
      case COMM_TAG_I2CS_SEND_BYTES:
         
         //--- GET BYTE COUNT
         if(not COMM_1_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_1 = comm_data;
               COMM_1_FLAG = 1;
               cbuf3_init();
            }
         }
         
         //--- PULL 'READ' DATA
         else
         {
            while(comm_cmnd_count_1)
            {
               if(comm_data_get())
               {
                  cbuf3_put(comm_data);
                  comm_cmnd_count_1--;
               }
               else
               {
                  break;
               }
            }
            if(comm_cmnd_count_1 == 0)
            {
               COMM_I2CS_READ_DATA_RDY_FLAG = 1;
               comm_state = COMM_STATE_GEN_NEXT;
            }
         }
         break;
         
      //====================================================
      case COMM_TAG_I2CS_REGDATA_LOAD:
         
         //-----------------------------
         //--- IF TAG DATA AVAIL., 
         //    DISABLE INTERRUPTS
         //-----------------------------
         if(not COMM_1_FLAG)
         {
            if(cbuf1.used >= 3)
            {
               comm_i2cs_dint();
               if(COMM_1_FLAG)
                  COMM_PROC_LOOP_FLAG = 1;
            }
         }

         //-----------------------------
         //--- FETCH 1st 3 BYTES OF 'TAG DATA'
         //-----------------------------
         else if(not COMM_2_FLAG)
         {
            comm_data_get();
            COMM_I2CS_ADDR = comm_data;         // SLAVE ADDR
            comm_data_get();
            COMM_I2CS_CMND = comm_data;         // REG ID
            comm_data_get();
            COMM_I2CS_TAGDATA_1 = comm_data;    // BYTE COUNT

            //--------------------------
            //--- SEARCH PROFILE FOR SLAVE/REG
            //--------------------------
            COMM_I2CS_TEMP_2_FLAG = 0;
            comm_i2cs_pro_get_slave();
            if(COMM_I2CS_PRO_SLAVE_OK_FLAG)
            {
               comm_i2cs_pro_get_regdef();
               if(COMM_I2CS_PRO_REG_OK_FLAG)
                  COMM_I2CS_TEMP_2_FLAG = 1;
            }
            COMM_2_FLAG = 1;
            COMM_PROC_LOOP_FLAG = 1;
         }

         //-----------------------------
         //--- PROCESS TAG DATA
         //-----------------------------
         else
         {
            //--- FOR ALL 'TAG DATA'
            while(COMM_I2CS_TAGDATA_1)
            {
               if(comm_data_get())
               {
                  //--- IF SLAVE AND REG FOUND
                  if(COMM_I2CS_TEMP_2_FLAG)
                  {
                     //--- IF REG BYTE COUNT != 0
                     if(COMM_I2CS_PRO_REG_BC)
                        cbuf3_data[COMM_I2CS_PRO_REG_INDEX] = comm_data;
                  }
                  COMM_I2CS_PRO_REG_INDEX++;
                  COMM_I2CS_TAGDATA_1--;
                  COMM_I2CS_PRO_REG_BC--;
               }
               else
               {
                  break;
               }
            }

            INTCONbits.GIEL = 1;
            comm_state = COMM_STATE_GEN_NEXT;
         }            
         break;

      //====================================================
      case COMM_TAG_I2CS_REGDATA_RETURN:
         
         //-----------------------------
         //--- IF TAG DATA AVAIL., 
         //    DISABLE INTERRUPTS
         //-----------------------------
         if(not COMM_1_FLAG)
         {
            if(cbuf1.used >= 2)
            {
               comm_i2cs_dint();
               if(COMM_1_FLAG)
                  COMM_PROC_LOOP_FLAG = 1;
            }
         }

         //-----------------------------
         //--- FETCH 'TAG DATA' (2 bytes)
         //-----------------------------
         else if(not COMM_2_FLAG && (cbuf2.unused>=4))
         {
            comm_data_get();
            COMM_I2CS_ADDR = comm_data;         // SLAVE ADDR
            comm_data_get();
            COMM_I2CS_CMND = comm_data;         // REG ID

            //--------------------------
            //--- SEARCH PROFILE FOR SLAVE/REG
            //--------------------------
            COMM_I2CS_TEMP_2_FLAG = 0;
            comm_i2cs_pro_get_slave();
            if(COMM_I2CS_PRO_SLAVE_OK_FLAG)
            {
               comm_i2cs_pro_get_regdef();
               if(COMM_I2CS_PRO_REG_OK_FLAG)
               {
                  COMM_I2CS_TEMP_2_FLAG = 1;
                  cbuf2_put(COMM_TAG_I2CS_REGDATA);
                  cbuf2_put(COMM_I2CS_ADDR);
                  cbuf2_put(COMM_I2CS_CMND);
                  cbuf2_put(COMM_I2CS_PRO_REG_BC);
               }
            }
            COMM_2_FLAG = 1;
            COMM_PROC_LOOP_FLAG = 1;
         }

         //-----------------------------
         //--- RETURN REG DATA
         //-----------------------------
         else
         {
            //--- IF SLAVE/REG FOUND
            if(COMM_I2CS_TEMP_2_FLAG)
            {

               //--- FOR ALL REG DATA
               while(COMM_I2CS_PRO_REG_BC)
               {
                  if(cbuf2.unused)
                  {
                     comm_data = cbuf3_data[COMM_I2CS_PRO_REG_INDEX];
                     cbuf2_put(comm_data);
                     COMM_I2CS_PRO_REG_INDEX++;
                     COMM_I2CS_PRO_REG_BC--;
                  }
                  else
                     break;
               }
            }

            INTCONbits.GIEL = 1;
            comm_state = COMM_STATE_GEN_NEXT;
         }            
         break;
                  
      //====================================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;

   } // end SWITCH
   
}

//==========================================================
//--- comm_i2cs_dint()
//
//    disable interrupts if between I2C transactions
//    COMM_1_FLAG == 1  interrupts successfully disabled
//    COMM_1_FLAG == 0  action failed, 
//                      I2C transaction active, ints enabled
//==========================================================
void comm_i2cs_dint(void)
{
   if((SSPSTATbits.P) | ((!SSPSTATbits.P) && (!SSPSTATbits.S)))
   {
      while(INTCONbits.GIEL)
          INTCONbits.GIEL = 0;
      if((SSPSTATbits.P) | ((!SSPSTATbits.P) && (!SSPSTATbits.S)))        
         COMM_1_FLAG = 1;
      else
         INTCONbits.GIEL = 1;
   }
}

