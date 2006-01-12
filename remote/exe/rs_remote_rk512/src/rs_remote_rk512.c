/* 
 * Proview   $Id: rs_remote_rk512.c,v 1.1 2006-01-12 06:39:33 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*************************************************************************
*
*                       R K 5 1 2
*                       =========
**************************************************************************
*
* Filename:             rs_remote_rk512.c
*
*                       Date    Pgm.    Read.   Remark
* Modified              951006  CJu
*                       951124  TAn
*
* Description:          Implements remote transport process RK512
*
**************************************************************************
**************************************************************************/

/*_Include files_________________________________________________________*/

#ifdef  OS_ELN
#include $vaxelnc
#include $exit_utility
#include $function_descriptor
#include $dda_utility
#include $elnmsg
#include $kernelmsg

#include ssdef
#include stdio
#include stdlib
#include string
#include math
#include iodef
#include descrip
#include psldef
#include libdtdef
#include starlet
#include lib$routines
#endif

#ifdef  OS_VMS
#include <ssdef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iodef.h>
#include <descrip.h>
#include <psldef.h>
#include <libdtdef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <pams_c_process.h>
#include <pams_c_type_class.h>
#include <pams_c_return_status.h>
#include <sbs_msg_def.h>
#endif

#include "pwr_class.h"
#include "pwr_systemclasses.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "pwr_ssabclasses.h"
#include "rs_remote_msg.h"
#include "rs_remote.h"
#include "rs_remtrans_utils.h"

/*_defines_________________________________________________________________*/

#define HEADER_SIZE 10
#define FOLLOW_ON_HEADER_SIZE 4
#define MAX_SIZE_DATA_BLOCK 128
#define MAX_SIZE_TELEGRAM 141
#define RESP_MESSAGE_SIZE 6
#define NUMBER_OF_STOP_CHAR 3

#define TRUE 1
#define FALSE 0

#define BYTE_MASK 0xFF
#define CPU_NR_MASK 0xF0

#define NUL 0
#define STX 2
#define ETX 3
#define DLE 16
#define NAK 21


#ifdef  OS_ELN
/*
 * ELN specific variables
 */

PORT       virtual_serial_line_port;
PORT       job_port;
PROCESS    my_proc;

MESSAGE		read_message;
char		*read_message_buf[1024];
MESSAGE		write_message;
char		*write_message_buf[1024];
int		msgsize = 1024;

#else
/*
 * VMS specific variables
 */

unsigned int receive_ef;
unsigned int timer_ef;
unsigned int ef_mask;

#endif

/*
 * Variables common for VMS and ELN
 */

remnode_item remnode_struct;
remnode_item *remnode = &remnode_struct;
LARGE_INTEGER  timeout_cycle;

float max_acknowledge_delay = 2.0;   /* Ack. time out on STX request */
LARGE_INTEGER  timeout_ack;
float max_response_delay = 5.0;      /* Response time out on send telegr. */
LARGE_INTEGER  timeout_resp;
float max_char_delay = 0.5;         /* Time out receive of next character */
LARGE_INTEGER  timeout_char;

unsigned int  telegram_counter = 0;  /* For control of follow on telegrams */


/********** Function prototypes **********************************/

static void             exith(void);
static void             DeclareExitHandler(void);
static unsigned int     InitTimers(void);
static unsigned int     InitNet(void);
static unsigned int     remnode_send(remnode_item *remnode,
                                     pwr_sClass_RemTrans *remtrans,
                                     char *buf,
                                     int buf_size);
static unsigned int     Receive();
static unsigned int     ReceiveHandler();
int                     SendResponseTelegram(unsigned char CODE);

#ifdef OS_VMS
int                     WaitFor_PLC_Change();
#endif

/*************************************************************************
**************************************************************************
*
* Namn : exith
*
* Typ  : void
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Exit handler that is called before process termination
*
**************************************************************************
**************************************************************************/

static void exith(void)
{
#ifdef OS_ELN

  unsigned int sts;

#else

  return;

#endif
}

/*************************************************************************
**************************************************************************
*
* Namn : DeclareExitHandler
*
* Typ  : void
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Declares an exit handler that is called before
*               process termination
*
**************************************************************************
**************************************************************************/

static void DeclareExitHandler(void)
{
#ifdef OS_ELN

  FUNCTION_DESCRIPTOR fn_descriptor;

  eln$declare_exit_handler(ELN$PASS_FUNCTION_DESCRIPTOR(fn_descriptor, exith),
                           NULL);
#else

  atexit(exith);
#endif
}

/*************************************************************************
**************************************************************************
*
* Namn : InitTimers
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Initiates the timers used in this process.
*
**************************************************************************
**************************************************************************/

static unsigned int InitTimers(void)
{
  unsigned int  sts;
  unsigned int  time_function_code = LIB$K_DELTA_SECONDS_F;

  /* Konvertera cykeltiden till intern deltatid */
  sts = lib$cvtf_to_internal_time(&time_function_code,
                                  &remnode->objp->CycleTime,
                                  &timeout_cycle);

  if ( ODD(sts) )
  {
    /* Set timeout for response character from receiver */
    sts = lib$cvtf_to_internal_time(&time_function_code,
                                    &max_acknowledge_delay,
                                    &timeout_ack);
  }

  if ( ODD(sts) )
  {
    /* Set timeout for response telegram from receiver */
    sts = lib$cvtf_to_internal_time(&time_function_code,
                                    &max_response_delay,
                                    &timeout_resp);
  }

  if ( ODD(sts) )
  {
    /* Set timeout for max delay between receive of two characters */
    sts = lib$cvtf_to_internal_time(&time_function_code,
                                    &max_char_delay,
                                    &timeout_char);
  }

  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : InitNet
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Assigns and sets up device
*
**************************************************************************
**************************************************************************/

static unsigned int InitNet(void)
{
  unsigned int            i, sts;
  char                    ch = '\0';
  char                    namestring[80];
  char                    portname[100];
  PORT                    DDA_destination_port;
  struct dsc$descriptor_s dataportname;


  for ( i=0 ; ( ch != ':' )&&( i < 80 ) ; i++ )
  {
    namestring[i] = ch = remnode->objp->NodeName[i];
  }
  namestring[--i] = '\0';

  sprintf(portname, "%s$ACCESS", namestring);
  dataportname.dsc$a_pointer = portname;
  dataportname.dsc$w_length  = strlen(portname);
  dataportname.dsc$b_class   = DSC$K_CLASS_S;
  dataportname.dsc$b_dtype   = DSC$K_DTYPE_T;

  /* Associate portnamne with serial line port ID */
  ker$translate_name(&sts, &DDA_destination_port, &dataportname, NAME$LOCAL);
  if ( ODD(sts) )
    /* Create a new port object */
    ker$create_port(&sts, &virtual_serial_line_port, NULL);
  if ( ODD(sts) )
    /* Connect the port object to the destination_port */
    ker$connect_circuit(&sts,
                        &virtual_serial_line_port,
                        &DDA_destination_port,
                        NULL, NULL, NULL, NULL);

  if ( ODD(sts) )
  {
    /* Create messages for tty_read and tty_write operations */
    ker$create_message(&sts, &read_message, &read_message_buf, msgsize+512);
    ker$create_message(&sts, &write_message, &write_message_buf, msgsize+512);
  }

  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : remnode_send
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Sends a message to Remote node
*
**************************************************************************
**************************************************************************/

static unsigned int remnode_send(remnode_item *remnode,
                                 pwr_sClass_RemTrans *remtrans,
                                 char *buf,
                                 int buf_size)
{
  unsigned int          sts, i;
  unsigned int          size_of_telegram, datasize;
  unsigned int          number_of_DLE = 0;
  unsigned int          nbr_of_bytes_written = 0;
  unsigned int          nbr_of_bytes_read = 0;
  unsigned int          delta_pos = 0;
  unsigned int          pos_counter = 0;
  unsigned int          follow_on = FALSE;
  unsigned int          A_telegram = FALSE;
  unsigned char         ch, cpu_number, CPU;
  unsigned char         BCC = DLE ^ ETX;
  unsigned char         datasize_low_byte, datasize_high_byte;
  unsigned char         received_char = '\0';
  unsigned char         response_buffer[RESP_MESSAGE_SIZE];
  unsigned char         *data_packet_ptr;
  unsigned char         *end_of_message_ptr;
  unsigned char         *restore_buf_ptr = buf;
  static unsigned char  sstx[2] = {STX, '\0'};
  static unsigned char  sdle[2] = {DLE, '\0'};
  static unsigned char  snak[2] = {NAK, '\0'};
  DDA$_BREAK_MASK       DLE_mask = DLE;
  DDA$_BREAK_MASK       STX_mask = STX;


  /* Define complete telegrams for sending */

  struct{
    unsigned char telegram_header[HEADER_SIZE];
    unsigned char telegram[MAX_SIZE_DATA_BLOCK*2 + NUMBER_OF_STOP_CHAR];
  }sendbuffer;

  struct{
    unsigned char telegram_header[FOLLOW_ON_HEADER_SIZE];
    unsigned char telegram[MAX_SIZE_DATA_BLOCK*2 + NUMBER_OF_STOP_CHAR];
  }follow_on_sendbuffer;


  do     /* Send 128 byte telegrams until message is finished */
  {
    if ( !follow_on )
    {
      A_telegram = TRUE;
/*************************************************************************/
/**    Send A-telegram.                                                 **/
/*************************************************************************/
/**    Check if follow on telegrams are needed.                         **/
/*************************************************************************/
      if ( buf_size - pos_counter > MAX_SIZE_DATA_BLOCK )
      {
        delta_pos = MAX_SIZE_DATA_BLOCK;
        follow_on = TRUE;
      }
      else
      {
        delta_pos = buf_size - pos_counter;
      }

/*************************************************************************/
/**    Calculate the size of the A-telegram.                            **/
/*************************************************************************/
      /* Count DLE characters */
      for ( i=0 ; i<delta_pos ; i++ )
      {
         if ( *buf++ == DLE )
            number_of_DLE += 1;
      }
      size_of_telegram = HEADER_SIZE+
                         delta_pos+number_of_DLE+NUMBER_OF_STOP_CHAR;

/*************************************************************************/
/**    Fill in the telegram header and calculate BCC.                   **/
/*************************************************************************/
      /* Size have to be expressed in number of 16 bits words. */
      /* If odd number of bytes add one. */

      datasize = buf_size/2 + buf_size%2;
      datasize_low_byte = (unsigned char)(BYTE_MASK & datasize);
      datasize = datasize >> 8;
      datasize_high_byte = (unsigned char)(BYTE_MASK & datasize);
      cpu_number = (unsigned char)remtrans->Address[2];
      CPU = '\xFF';
      CPU = CPU_NR_MASK & cpu_number;

      sendbuffer.telegram_header[0] = '\0';
      sendbuffer.telegram_header[1] = '\0';
      sendbuffer.telegram_header[2] = 'A';
      sendbuffer.telegram_header[3] = 'D';
      sendbuffer.telegram_header[4] = (unsigned char)remtrans->Address[0];
      sendbuffer.telegram_header[5] = (unsigned char)remtrans->Address[1];
      sendbuffer.telegram_header[6] = datasize_high_byte;
      sendbuffer.telegram_header[7] = datasize_low_byte;
      sendbuffer.telegram_header[8] = '\xFF';
      sendbuffer.telegram_header[9] = CPU;

      /* Calculate checksum for the header */
      for ( i=0 ; i<HEADER_SIZE ; i++ )
      {
        BCC ^= sendbuffer.telegram_header[i];
      }
/*************************************************************************/
/**   Fill up A-telegram with contents of message and calculate BCC     **/
/*************************************************************************/
      buf = restore_buf_ptr;
      for ( i=0 ; i<(delta_pos+number_of_DLE) ; i++ )
      {
        ch = sendbuffer.telegram[i] = *buf++;
        BCC ^= ch;
        if ( ch == DLE )
        {
          sendbuffer.telegram[++i] = DLE;
          BCC ^= ch;
        }
      }
      if ( delta_pos%2 )
      {
        /* Ensure that a even number of bytes is treated */
        sendbuffer.telegram[i++] = '\0';
        size_of_telegram += 1;
      }
      sendbuffer.telegram[i++] = DLE;
      sendbuffer.telegram[i++] = ETX;
      sendbuffer.telegram[i]   = BCC;

      pos_counter = delta_pos;
    }
    else  /* follow on telegram */
    {

/*************************************************************************/
/**    Send follow on telegram.                                         **/
/*************************************************************************/
/**    Check if more follow on telegrams are needed.                    **/
/*************************************************************************/
      if ( buf_size - pos_counter > MAX_SIZE_DATA_BLOCK )
      {
        delta_pos = MAX_SIZE_DATA_BLOCK;
        follow_on = TRUE;
      }
      else
      {
        delta_pos = buf_size - pos_counter;
        follow_on = FALSE;
      }

/*************************************************************************/
/**    Calculate the size of the follow on telegram.                    **/
/*************************************************************************/
      /* Count DLE characters */
      restore_buf_ptr = buf;
      number_of_DLE = 0;
      for ( i=0 ; i<delta_pos ; i++ )
      {
         if ( *buf++ == DLE )
            number_of_DLE += 1;
      }
      size_of_telegram = FOLLOW_ON_HEADER_SIZE+
                         delta_pos+number_of_DLE+NUMBER_OF_STOP_CHAR;

/*************************************************************************/
/**    Fill in the follow on telegram header and calculate BCC.         **/
/*************************************************************************/
      follow_on_sendbuffer.telegram_header[0] = '\xFF';
      follow_on_sendbuffer.telegram_header[1] = '\0';
      follow_on_sendbuffer.telegram_header[2] = 'A';
      follow_on_sendbuffer.telegram_header[3] = 'D';

      /* Calculate checksum for the header */
      BCC = DLE ^ ETX;
      for ( i=0 ; i<FOLLOW_ON_HEADER_SIZE ; i++ )
      {
        BCC ^= follow_on_sendbuffer.telegram_header[i];
      }

/*************************************************************************/
/* Fill up follow on telegram with contents of message and calculate BCC */
/*************************************************************************/
      buf = restore_buf_ptr;
      for ( i = 0 ; i < (delta_pos+number_of_DLE) ; i++ )
      {
        ch = follow_on_sendbuffer.telegram[i] = *buf++;
        BCC ^= ch;
        if ( ch == DLE )
        {
          follow_on_sendbuffer.telegram[++i] = DLE;
          BCC ^= ch;
        }
      }
      if ( delta_pos%2 )
      {
        /* Ensure that a even number of bytes is treated */
        follow_on_sendbuffer.telegram[i++] = '\0';
        size_of_telegram += 1;
      }
      follow_on_sendbuffer.telegram[i++] = DLE;
      follow_on_sendbuffer.telegram[i++] = ETX;
      follow_on_sendbuffer.telegram[i]   = BCC;

      pos_counter += delta_pos;

    }

/*************************************************************************/
/**    Execute the send procedure                                       **/
/*************************************************************************/
    /* Send STX and wait for answer */
    eln$tty_write(&sts, &virtual_serial_line_port, 1, &sstx,
                  &nbr_of_bytes_written, &write_message, &write_message_buf);
    if ( ODD(sts) )
    {
      /* wait for break character or timeout */
      eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
                   &nbr_of_bytes_read, (2+4), &DLE_mask, &timeout_ack,
                   NULL, NULL, &read_message, &read_message_buf);
    }
    if ( ODD(sts) && (sts != ELN$_TIMEOUT) )
    {
      if ( received_char == STX )
      {
        /* Both nodes is in sending mode. */
        /* Cancel this send operation and wait for next timeout or receive */
        eln$tty_write(&sts, &virtual_serial_line_port, 1, &snak,
                      &nbr_of_bytes_written, &write_message, &write_message_buf);
        return(FALSE);
      }
      if ( received_char == DLE )
      {
        /* Contact is established. Send telegram */
        if ( A_telegram )
        {
          eln$tty_write(&sts, &virtual_serial_line_port, size_of_telegram,
                        &sendbuffer,
                        &nbr_of_bytes_written, &write_message, &write_message_buf);
          A_telegram = FALSE;
        }
        else
        {
          eln$tty_write(&sts, &virtual_serial_line_port, size_of_telegram,
                        &follow_on_sendbuffer,
                        &nbr_of_bytes_written, &write_message, &write_message_buf);
        }
        if ( ODD(sts) )
        {
          /* wait for break character or timeout */
          eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
                       &nbr_of_bytes_read, (2+4), &DLE_mask, &timeout_ack,
                       NULL, NULL, &read_message, &read_message_buf);

          if ( ODD(sts)&&(sts != ELN$_TIMEOUT)&&(received_char == DLE) )
          {

/*************************************************************************/
/**   The sending was a SUCCESS. Take care of the response message      **/
/*************************************************************************/
            eln$tty_read(&sts, &virtual_serial_line_port, 1,
                         &received_char, &nbr_of_bytes_read, (2+4),
                         &STX_mask, &timeout_resp, NULL, NULL, 
			 &read_message, &read_message_buf);

            if ( ODD(sts)&&(sts != ELN$_TIMEOUT)&&(received_char == STX) )
            {
              /* Send DLE acknowledge and wait for response data */
              eln$tty_write(&sts, &virtual_serial_line_port, 1, &sdle,
                            &nbr_of_bytes_written, &write_message, &write_message_buf);
              if ( ODD(sts) )
              {
                BCC = '\0';
                for (i=0 ;
                     i<RESP_MESSAGE_SIZE&&ODD(sts)&&(sts!=ELN$_TIMEOUT);
                     i++ )
                {
                  eln$tty_read(&sts, &virtual_serial_line_port, 1,
                               &received_char, &nbr_of_bytes_read, 2,
                               NULL, &timeout_char,
                               NULL, NULL, &read_message, &read_message_buf);
                  response_buffer[i] = received_char;
                  BCC ^= received_char;
                }  /* endfor */

                if ( ODD(sts) &&
                     (sts != ELN$_TIMEOUT) &&
                     (response_buffer[2] == '\0') )
                {
                  /* Compare received BCC with calculated */
                  eln$tty_read(&sts, &virtual_serial_line_port,
                               1, &received_char, &nbr_of_bytes_read, 2,
                               NULL, &timeout_char,
                               NULL, NULL, &read_message, &read_message_buf);
                  if ( ODD(sts) &&
                       (sts != ELN$_TIMEOUT) &&
                       ( BCC == received_char ) )
                  {
                    /* Response telegram received OK */
                    eln$tty_write(&sts, &virtual_serial_line_port, 1, &sdle,
                                  &nbr_of_bytes_written, &write_message, &write_message_buf);
                    if ( response_buffer[3] != 0 )
                    {
                      /* This response contains a error code */
                      errh_CErrLog(REM__SIEMENSERROR,
                                   errh_ErrArgL(response_buffer[3]) );
                    }
                  }
                  else
                  {
                    /* Wrong checksum. */
                    sts = FALSE;
                  }
                }
                else
                {
                  /* This is not a response message as expected */
                  eln$tty_write(&sts, &virtual_serial_line_port, 1, &snak,
                                &nbr_of_bytes_written, &write_message, &write_message_buf);
                  sts = FALSE;
                }
              } /* ENDIF. DLE acknowledge failed */
            }
            else
            {
              /* STX character in response message was expected. */
              /* Ensure that error status is returned */
              sts = FALSE;
            }
          }
          else
          {
            /* DLE ack. after sending telegram was expected. */
            /* Ensure that error status is returned */
            sts = FALSE;
          }
        } /* ENDIF. Contact established but tty_write failed */
      }
      else
      {
        /* Failed in making contact. Wrong response character. */
        /* Ensure that error status is returned */
        sts = FALSE;
      }
    }  /* ENDIF. tty_write or tty_read failed */

/*************************************************************************/
/**  Check final status.                                                **/
/*************************************************************************/
    if ( EVEN(sts) || (sts == ELN$_TIMEOUT) )
    {
      /* The send procedure has failed */
      eln$tty_write(&sts, &virtual_serial_line_port, 1, &snak,
                    &nbr_of_bytes_written, &write_message, &write_message_buf);

      follow_on = FALSE;

      /* Ensure that error status is returned */
      sts = FALSE;
    }

  }while( follow_on );

  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : Receive
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Waits for STX on serial line
*
**************************************************************************
**************************************************************************/

static unsigned int Receive()
{
  unsigned int          sts, wait_result;
  unsigned char         received_char = NUL;
  DDA$_BREAK_MASK       DLE_mask = DLE;
  unsigned int          nbr_of_bytes_read = 0;
  unsigned int          nbr_of_bytes_written = 0;
  static unsigned char  snak[2] = {NAK, NUL};


  eln$tty_read(&sts, &virtual_serial_line_port, 1,
               &received_char, &nbr_of_bytes_read, (2+4), &DLE_mask,
               &timeout_cycle, NULL, NULL, &read_message, &read_message_buf);

  if (sts == ELN$_SUCCESS)
  {
    if (received_char == STX)
    {
      sts = ReceiveHandler();

      while ( ODD(sts) && telegram_counter )
      {
        /* Follow on telegrams are expected */
        received_char = NUL;    
        eln$tty_read(&sts, &virtual_serial_line_port, 1,
                     &received_char, &nbr_of_bytes_read, (2+4), &DLE_mask,
                     &timeout_ack, NULL, NULL, &read_message, &read_message_buf);

        if (sts == ELN$_SUCCESS && received_char == STX)
        {
          sts = ReceiveHandler();
        }
        else
        {
          sts = FALSE;
        }
      } /* endwhile */
    }
    else
    {

      /* We don't understand, Send NAK unless NAK is received */
        
      if (received_char != NAK)
        eln$tty_write(&sts, &virtual_serial_line_port, 1, &snak,
                      &nbr_of_bytes_written, &write_message, &write_message_buf);
    }
  }

  if ( EVEN(sts) )
  {
    remnode->objp->ErrTransCount++;
    telegram_counter = 0;
    eln$tty_write(&sts, &virtual_serial_line_port, 1, &snak,
                  &nbr_of_bytes_written, &write_message, &write_message_buf);
  }

  return(1);
}


/*************************************************************************
**************************************************************************
*
* Namn : ReceiveHandler
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Invoked when a telegram is received.
*
**************************************************************************
**************************************************************************/

static unsigned int ReceiveHandler()
{
  unsigned int          sts, i;
  unsigned int          delta_pos = 0;
  unsigned int          nbr_of_bytes_written = 0;
  unsigned int          nbr_of_bytes_read = 0;
  static unsigned int   datasize, pos_counter;
  static remtrans_item  *remtrans;
  char                  search_remtrans = FALSE;
  char                  send_response = FALSE;
  char                  terminate = FALSE;
  unsigned char         CODE = '\0';
  unsigned char         BCC = '\0';
  unsigned char         char_buffer, BCC_checksum;
  unsigned char         received_char = '\0';
  unsigned char         ReceiveBuffer[MAX_SIZE_TELEGRAM];
  unsigned char         *ReceiveBuffer_ptr;
  unsigned char         sstx[2] = {STX, '\0'};
  unsigned char         sdle[2] = {DLE, '\0'};
  unsigned char         snak[2] = {NAK, '\0'};
  static unsigned char  *TelegramBuffer;
  static unsigned char  *TelegramBuffer_ptr;
  DDA$_BREAK_MASK       DLE_mask = DLE;
  DDA$_BREAK_MASK       STX_mask = STX;


/*************************************************************************/
/** We have received STX earlier, send DLE to responde                  **/
/*************************************************************************/

    /* Send acknowledge ready to receive message */
  eln$tty_write(&sts, &virtual_serial_line_port, 1, &sdle,
                &nbr_of_bytes_written, &write_message, &write_message_buf);
  if ( EVEN(sts) )
    return(FALSE);

/*************************************************************************/
/**    Store the received telegram temporary                            **/
/*************************************************************************/
  eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
               &nbr_of_bytes_read, 2, NULL, &timeout_char,
               NULL, NULL, &read_message, &read_message_buf);
  if ( EVEN(sts) || (sts == ELN$_TIMEOUT) )
    return(FALSE);

  ReceiveBuffer[0] = char_buffer = received_char;
  BCC ^= received_char;

  for ( i=1 ; (terminate==FALSE)&&(i<MAX_SIZE_TELEGRAM) ; i++ )
  {
    eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
                 &nbr_of_bytes_read, 2, NULL, &timeout_char,
                 NULL, NULL, &read_message, &read_message_buf);
    if ( EVEN(sts) || (sts == ELN$_TIMEOUT) )
      return(FALSE);

    if ( (char_buffer == DLE) && (received_char == ETX) )
    {
      /* End of message. Read checksum and terminate. */
      ReceiveBuffer[i] = received_char;
      BCC ^= received_char;
      eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
                   &nbr_of_bytes_read, 2, NULL, &timeout_char,
                   NULL, NULL, &read_message, &read_message_buf);
      if ( EVEN(sts) || (sts == ELN$_TIMEOUT) )
        return(FALSE);

      BCC_checksum = received_char;
      terminate = TRUE;
    }
    else
    {
      /* Store one more received character in the buffer */
      BCC ^= received_char;
      if ( (received_char != DLE) ||
           ((received_char == DLE) && (char_buffer != DLE)) )
      {
        ReceiveBuffer[i] = char_buffer = received_char;
      }
      else
      {
        /* This is a duplicated DLE character. Throw away. */
        i--;
        char_buffer = '\0';
      }
    }
  }    /* endfor */

/*************************************************************************/
/**  A complete message is received. Check BCC.                         **/
/*************************************************************************/
  if ( BCC == BCC_checksum )
  {
    eln$tty_write(&sts, &virtual_serial_line_port, 1, &sdle,
                  &nbr_of_bytes_written, &write_message, &write_message_buf);
    if ( EVEN(sts) )
      return(FALSE);
  }
  else
  {
    /* Checksum in this telegram is wrong */
    return(FALSE);
  }


  if ( !telegram_counter && (ReceiveBuffer[0] == 0X00) )
  {
/*************************************************************************/
/**  This is a ordinary A-telegram. Make some error checks and treat it. */
/*************************************************************************/

    /*  Find out wich RemTrans object that is the target.*/
    search_remtrans = true;
    remtrans = remnode->remtrans;
    while(remtrans && search_remtrans)
    {
      if ( (remtrans->objp->Address[0] == ReceiveBuffer[4]) &&
           (remtrans->objp->Direction  == REMTRANS_IN) )
      {
        search_remtrans = false;
      }
      if ( search_remtrans )
      {
        remtrans = (remtrans_item *) remtrans->next;
      }
    }   /* endwhile */

    if (search_remtrans)
    {
      /* No corresponding RemTrans object found */
      remnode->objp->ErrTransCount++;
      CODE = '\x0A';
    }

    if ( !CODE )
    {
      /* Allowed type of telegram ? */
      if ( ReceiveBuffer[2] != 'A' )
        CODE = '\x16';
    }

    if ( !CODE )
    {
      /* Allowed type of data ? */
      if ( ReceiveBuffer[3] != 'D' )
        CODE = '\x10';
    }

    if ( !CODE )
    {
      /* Check size of received message */
      datasize = (unsigned int)ReceiveBuffer[6];
      datasize = datasize << 8;
      datasize = datasize | (unsigned int)ReceiveBuffer[7];
      datasize = datasize * 2;  /* Convert from 16 bit words to bytes */
      if ( datasize <= remtrans->objp->MaxLength)
      {
        if (datasize > 128) telegram_counter = (datasize-1)/128;
      }
      else
      {
        /* Size of received message is to big */
        remtrans->objp->ErrCount++;
        CODE = '\x34';
      }
    }

    if ( telegram_counter && !CODE )
    {
      /* Follow on telegrams are expected, allocate more memory */
      TelegramBuffer = malloc(datasize);
      TelegramBuffer_ptr = TelegramBuffer;

      /* Temporary store this telegram */
      for ( i =  HEADER_SIZE ;
            i < (HEADER_SIZE + MAX_SIZE_DATA_BLOCK) ;
            i++ )
      {
        *TelegramBuffer_ptr++ = ReceiveBuffer[i];
      }
      pos_counter = MAX_SIZE_DATA_BLOCK;
    }
    else
      if ( !CODE )
      {
        /* This message contains only one telegram. Treat it! */
        ReceiveBuffer_ptr = &ReceiveBuffer[HEADER_SIZE];
        sts = RemTrans_Receive(remtrans,
                               ReceiveBuffer_ptr,
                               datasize);
        if ( EVEN(sts) )
        {
          remtrans->objp->ErrCount++;
          return(1);
        }
      }
  }
  else
  {
    if ( !telegram_counter )
      /* Wrong type of telegram received when A-telegram was expected */
      {
      remnode->objp->ErrTransCount++;
      CODE = '\x10';
      }
  }

  if ( telegram_counter && (ReceiveBuffer[0] == 0XFF) && !CODE )
  {
/*************************************************************************/
/**  This is a follow on telegram. Make some error checks and treat it. **/
/*************************************************************************/

    if ( !CODE )
    {
      /* Allowed type of telegram ? */
      if ( ReceiveBuffer[2] != 'A' )
        CODE = '\x16';
    }

    if ( !CODE )
    {
      /* Allowed type of data ? */
      if ( ReceiveBuffer[3] != 'D' )
        CODE = '\x10';
    }

    if ( !CODE )
    {
      if ( datasize - pos_counter > MAX_SIZE_DATA_BLOCK )
      {
        delta_pos = MAX_SIZE_DATA_BLOCK;
      }
      else
      {
        delta_pos = datasize - pos_counter;
      }

      for ( i =  FOLLOW_ON_HEADER_SIZE ;
            i < (FOLLOW_ON_HEADER_SIZE + delta_pos) ;
            i++ )
      {
        *TelegramBuffer_ptr++ = ReceiveBuffer[i];
      }
      telegram_counter -= 1;
      pos_counter += delta_pos;

      if ( !telegram_counter )
      {
        /* This was the last follow on telegram. Treat the message */
        if ( pos_counter != datasize )
        {
          /* Wrong number of characters counted */
          free(TelegramBuffer);
          remtrans->objp->ErrCount++;
          return(1);
        }
        TelegramBuffer_ptr = TelegramBuffer;
        sts = RemTrans_Receive(remtrans,
                               TelegramBuffer_ptr,
                               datasize);
        free(TelegramBuffer);
        if ( EVEN(sts) )
        {
          remtrans->objp->ErrCount++;
          return(1);
        }
      }
    }
  }
  else
  {
    if ( telegram_counter && !CODE )
      /* Wrong type of telegram received when follow on was expected */
    {
      free(TelegramBuffer);
      telegram_counter = 0;
      remtrans->objp->ErrCount++;
      CODE = '\x10';
    }
  }

  sts = SendResponseTelegram(CODE);
  if ( EVEN(sts) || (sts == ELN$_TIMEOUT) )
  {
    /* A fail has occured when sending response telegram */
    return(FALSE);
  }
  else
  {
    return(TRUE);
  }
}

/*************************************************************************
**************************************************************************
*
* Namn : SendResponseTelegram
*
* Typ  : unsigned int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Invoked when a response telegram is to be sent.
*
**************************************************************************
**************************************************************************/
int SendResponseTelegram(unsigned char CODE)
{
  unsigned int          sts;
  unsigned int          nbr_of_bytes_written = 0;
  unsigned int          nbr_of_bytes_read = 0;
  unsigned char         received_char = '\0';
  unsigned char         BCC = '\0';
  unsigned char         ResponseTelegram[RESP_MESSAGE_SIZE+1];
  unsigned char         sstx[2] = {STX, '\0'};
  unsigned char         sdle[2] = {DLE, '\0'};
  DDA$_BREAK_MASK       DLE_mask = DLE;
  DDA$_BREAK_MASK       STX_mask = STX;


  BCC = DLE ^ ETX;
  BCC ^= CODE;
  ResponseTelegram[0] = '\0';
  ResponseTelegram[1] = '\0';
  ResponseTelegram[2] = '\0';
  ResponseTelegram[3] = CODE;
  ResponseTelegram[4] = DLE;
  ResponseTelegram[5] = ETX;
  ResponseTelegram[6] = BCC;

  /* Send STX and wait for answer */
  eln$tty_write(&sts, &virtual_serial_line_port, 1,
                &sstx, &nbr_of_bytes_written, &write_message, &write_message_buf);
  if ( ODD(sts) )
  {
    /* wait for break character or timeout */
    eln$tty_read(&sts, &virtual_serial_line_port, 1, &received_char,
                 &nbr_of_bytes_read, (2+4), &DLE_mask, &timeout_ack,
                 NULL, NULL, &read_message, &read_message_buf);

    if ( ODD(sts) && (sts != ELN$_TIMEOUT) && (received_char == DLE) )
    {
      /* Contact is established. Send response telegram */
      eln$tty_write(&sts, &virtual_serial_line_port,
                    sizeof(ResponseTelegram), &ResponseTelegram,
                    &nbr_of_bytes_written, &write_message, &write_message_buf);
      if ( ODD(sts) )
      {
        /* wait for break character or timeout */
        eln$tty_read(&sts, &virtual_serial_line_port, 1,
                     &received_char, &nbr_of_bytes_read, (2+4), &DLE_mask,
                     &timeout_ack, NULL, NULL, &read_message, &read_message_buf);
        if ( EVEN(sts) || (sts == ELN$_TIMEOUT) || (received_char != DLE) )
        {
          /* DLE character in response was expected. */
          sts = FALSE;
        }
      }  /* ENDIF. Contact established but tty_write failed */
    }
    else
    {
      /* Failed in making contact. DLE in response was expected. */
      sts = FALSE;
    }
  }   /* ENDIF. tty_write failed when sending STX */

  return(sts);
}



#ifdef OS_ELN
/*************************************************************************
**************************************************************************
*
* Namn : WaitFor_PLC_Change
*
* Typ  : int
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Subprocess  waits for message from REMOTEHANDLER on job_port,
*               that will kill transport when switch is done.
*
**************************************************************************
**************************************************************************/

int WaitFor_PLC_Change()
{
  pwr_tStatus sts;
  MESSAGE msg_id;
  char *mess;
  int size;

  while (TRUE) {
    ker$wait_any(&sts, NULL, NULL, &job_port);
    ker$receive(&sts, &msg_id, &mess, &size, &job_port, NULL, NULL);

    if (*mess == 1){
      /* Hot switch init. Do nothing but delete the message */
      ker$delete(&sts, msg_id);
    }
    else if (*mess == 2){
      /* Switch is done. Execute harakiri! */
      ker$delete(&sts, msg_id); /* Delete message */
      exith();
      ker$delete(&sts, my_proc);
    }
    else {
      printf("Fel telegram PLC-byte %%x%x\n",*mess);
      ker$delete(&sts, msg_id);
    }
  }
}
#endif


/*
 * Main routine
 */

main(int argc, char *argv[])
{
  unsigned int sts, i;                          /* Status from function calls etc. */
  pwr_tObjid pwr_node;                          /* Own Pwr node */
  pwr_sClass_PlcProcess *pwr_nodep;             /* Ref to own Pwr node */
  pwr_tTime OriginChgTime;                   /* LastChgTime at start */

  pwr_tClassId  remnode_class;                     /* Vår remnod's class */

#ifdef OS_ELN
  PROCESS id_p;
  static struct dsc$descriptor_s name_desc = {0, DSC$K_DTYPE_T,DSC$K_CLASS_S,0};
  NAME name_id;

  /* Get process-id to be able to kill myself.
     Create subprocess to kill or to signal switch. */

  ker$job_port(&sts, &job_port);
  ker$current_process(&sts, &my_proc);

  /* Create a name for my own process (first process in job) */

  name_desc.dsc$a_pointer = argv[3];
  name_desc.dsc$w_length = strlen(argv[3]);
  ker$create_name(&sts, &name_id, &name_desc, my_proc, NULL);

  ker$create_process( &sts, &id_p, WaitFor_PLC_Change,NULL);
#endif

  DeclareExitHandler();

  sts = gdh_Init("rs_remote_rk512");
  if (EVEN(sts)) exit(sts);

  /* Hämta remnodens objid från argumentvektorn */

  if (argc >= 3)
  {
    sts = cdh_StringToObjid( argv[2], &remnode->objid);
    if (EVEN(sts)) exit(sts);
  }
  else
    remnode->objid = pwr_cNObjid;

  /* Kontrollera att objektet verkligen är en remnod */

  sts = gdh_GetObjectClass(remnode->objid, &remnode_class);
  if (EVEN(sts)) exit(sts);

  if (remnode_class != pwr_cClass_RemNode) exit(0);

  /* Hämta en pekare till remnoden */

  sts = gdh_ObjidToPointer(remnode->objid, (pwr_tAddress *) &remnode->objp);
  if (EVEN(sts)) exit(sts);

  /* Kontrollera att det är rätt transport */

  if (remnode->objp->TransportType != REMNODE_TRANSPORT_RK512) exit(0);

  remnode->Time_since_scan = 0;
  remnode->Time_since_poll = 0;
  remnode->Time_since_IO   = 0;
  remnode->Time_since_send = 0;

  /* Initialize remtrans objects */

  sts = RemTrans_Init(remnode, 0);
  if (EVEN(sts)) exit(sts);

  /* Get pointer to $Node-object */

  sts = gdh_GetClassList( pwr_cClass_PlcProcess, &pwr_node);
  sts = gdh_ObjidToPointer(pwr_node, (pwr_tAddress *) &pwr_nodep);

  /* Initialize device */
  sts = InitNet();
  if (EVEN(sts)) exit(sts);

  /* Initialize timers */
  sts = InitTimers();
  if (EVEN(sts)) exit(sts);

  /* Loop forever */

  while (!doomsday)
  {
    sts = Receive();
    if (EVEN(sts)) exit(sts);

    remnode->Time_since_send += remnode->objp->CycleTime;
    sts = RemTrans_Cyclic(remnode, &remnode_send);
  }
}
