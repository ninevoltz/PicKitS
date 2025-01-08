//==========================================================
//==========================================================
//--- COMM_PROC_I2CM
//==========================================================
//==========================================================
#include "define_common.h"
#include "comm_gen.h"
#include "comm_sps.h"
#include "cbuf.h"

//==========================================================
//--- comm_proc_sps_init()
//==========================================================
uint8 comm_proc_sps_init(void)
{
   return(0);
}

//==========================================================
//--- comm_proc_sps_postproc()
//==========================================================
void comm_proc_sps_postproc(void)
{
   
   //-----------------------------------
   //--- transaction error
   //-----------------------------------
   COMM_4_FLAG = 0;
   if(SSPCON1bits.WCOL)
   {
      COMM_4_FLAG = 1;
      SSPCON1bits.WCOL = 0;
      COMM_SB_SPS_WCOL_FLAG = 1;
      
   }
   if(SSPCON1bits.SSPOV)
   {
      COMM_4_FLAG = 1;
      SSPCON1bits.SSPOV = 0;
      COMM_SB_SPS_SSPOV_FLAG = 1;
   }
   if(COMM_4_FLAG)
   {
      if(COMM_TAG_SPS_EVENT_STATUS_ERR)
      {
         comm_post_event_1b(COMM_TAG_SPS_EVENT_STATUS_ERR, COMM_SB_SPS_ERR_BYTE);
      }
   }
   
   //-----------------------------------
   //--- composite error
   //-----------------------------------
   if(exec_status[12].all & COMM_SB_SPS_ERR_MASK)
   {
      exec_status[04].bit7 = 1;
      exec_status[12].bit7 = 1;
   }

   //-----------------------------------
   //--- change in error status
   //-----------------------------------
   if(COMM_CB_SPS_EVENT_STATUS_ERR_FLAG)
   {
      if(exec_status[12].all != exec_status_save_2)
      {
         exec_status_save_2 = exec_status[12].all;
         comm_post_event_1b(COMM_TAG_SPS_STATUS_ERR, exec_status[12].all);
      }
   }
   

}

//==========================================================
//--- comm_proc_i2cm()
//==========================================================
void comm_proc_i2cm(void)
{

   do
   {
      COMM_PROC_LOOP_FLAG = 0;
      
   //---------------------------------------------
   //--- SWITCH CONTROLLER STATE
   //---------------------------------------------
   switch(comm_state)
   {

      //==========================================
      case COMM_TAG_I2CM_INIT:
         comm_state = COMM_STATE_GEN_NEXT;
         //SSPCON1 = SSPCON1_DFLT;
         //SSPADD = SSPADD_DFLT;
         break;
         
      //==========================================
      case COMM_TAG_I2CM_START:
         if(not COMM_0_FLAG)
         {
            SSPCON2bits.SEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_START_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_START);
         }
         else
         {
            if(SSPCON2bits.SEN == 0)
            {
               comm_state = COMM_STATE_GEN_NEXT;
            }
         }
         break;

      //==========================================
      case COMM_TAG_I2CM_STOP:
         if(not COMM_0_FLAG)
         {
            //debug
            LED3_PIN = 1;
            SSPCON2bits.PEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_STOP_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_STOP);
         }
         else
         {
            if(SSPCON2bits.PEN == 0)
               comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //==========================================
      case COMM_TAG_I2CM_RESTART:
         if(not COMM_0_FLAG)
         {
            SSPCON2bits.RSEN = 1;
            COMM_0_FLAG = 1;
            if(COMM_CB_I2CM_EVENT_RESTART_TX_FLAG)
               comm_post_event(COMM_TAG_I2CM_EVENT_RESTART);
         }
         else
         {
            if(SSPCON2bits.RSEN == 0)
               comm_state = COMM_STATE_GEN_NEXT;
         }
         break;

      //==========================================
      case COMM_TAG_SPS_DATAIO_OUT:
      case COMM_TAG_SPS_DATAIO_IN:
      case COMM_TAG_SPS_DATAIO_INOUT:

         //--- GET BYTE COUNT
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2 = comm_data;
               COMM_0_FLAG = 1;
            }
         }

         //--- CHECK BYTE COUNT
         else if(not COMM_1_FLAG)
         {
            if(not comm_cmnd_count_2)
            {
               comm_state = COMM_STATE_GEN_NEXT;
               break;
            }
            if(SSPSTATbits.BF == 0)
               COMM_1_FLAG = 1;
         }
         
         //--- OUTPUT BYTE (when/if available)
         else
         {
            if(comm_data_get())
            {
               COMM_1_FLAG = 0;
               comm_cmnd_count_2--;
               PIR1bits.SSPIF = 0;
               SSPBUF = comm_data;
               if(COMM_CB_SPS_EVENT_BYTE_TX_FLAG)
                  comm_post_event_1b(COMM_TAG_SPS_EVENT_BYTE_TX, comm_data);
            }
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_READ_BYTES:
      case COMM_TAG_I2CM_READ_BYTES_NLB:
         
         //--- GET BYTE COUNT
         if(not COMM_0_FLAG)
         {
            if(comm_data_get())
            {
               comm_cmnd_count_2 = comm_data;
               COMM_0_FLAG = 1;
            }
         }

         //--- CHECK BYTE COUNT
         else if(not COMM_1_FLAG)
         {
            if(not comm_cmnd_count_2)
            {
               comm_state = COMM_STATE_GEN_NEXT;
               COMM_PROC_LOOP_FLAG = 1;
               break;
            }
            COMM_1_FLAG = 1;
            SSPCON2bits.RCEN = 1;
         }

         //--- READ BYTE (when/if available)
         else if(not COMM_2_FLAG)
         {
            if(PIR1bits.SSPIF)
            {
               PIR1bits.SSPIF = 0;
               comm_data = SSPBUF;
               //comm_post_data();
               comm_post_event_1b(COMM_TAG_I2CM_EVENT_BYTE_READ, comm_data);
               comm_cmnd_count_2--;
               COMM_2_FLAG = 1;
               
               SSPCON2bits.ACKDT = 0;
               if((comm_state == COMM_TAG_I2CM_READ_BYTES_NLB) && (not comm_cmnd_count_2))
                  SSPCON2bits.ACKDT = 1;
               SSPCON2bits.ACKEN = 1;
            }
         }
         
         //--- WAIT FOR ACK/NACK EXECUTION
         else
         {
            if(PIR1bits.SSPIF)
            {
               PIR1bits.SSPIF = 0;
               if(SSPCON2bits.ACKDT)
                  comm_post_event(COMM_TAG_I2CM_EVENT_NACK_TX);
               else
                  comm_post_event(COMM_TAG_I2CM_EVENT_ACK_TX);
               COMM_1_FLAG = 0;
               COMM_2_FLAG = 0;
               COMM_PROC_LOOP_FLAG = 1;
            }
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_READ_BLOCK:
      case COMM_TAG_I2CM_READ_BLOCK_NLB:

         //--- GET BYTE COUNT
         if(PIR1bits.SSPIF)
         {
            //--- process byte
            PIR1bits.SSPIF = 0;
            comm_data = SSPBUF;
            comm_post_data();
            comm_cmnd_count_2 = comm_data;

            //--- issue ACK
            SSPCON2bits.ACKDT = 0;
            SSPCON2bits.ACKEN = 1;

            //--- prep for "READ_BYTES" state
            COMM_0_FLAG = 1;
            COMM_1_FLAG = 1;
            COMM_2_FLAG = 1;
            comm_state = COMM_TAG_I2CM_READ_BYTES;
            if(comm_state == COMM_TAG_I2CM_READ_BLOCK_NLB)
               comm_state = COMM_TAG_I2CM_READ_BYTES_NLB;
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_BITRATE:
         if(comm_data_get())
         {
            SSPADD = comm_data;
            comm_state = COMM_STATE_GEN_NEXT;
         }
         break;
         
      //==========================================
      case COMM_TAG_I2CM_RESET:
         SSPCON1bits.SSPEN = 0;
         SSPCON1bits.SSPEN = 1;
         break;

      //==========================================
      default:
         COMM_SB_BAD_CMND_FLAG = 1;
         break;

   } // end SWITCH
   
   } while(COMM_PROC_LOOP_FLAG);
}

