/* 
 * Proview   $Id: rs_remote_modbus.c,v 1.1 2006-01-12 06:39:33 claes Exp $
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
*                       M O D B U S
*                       ===========
**************************************************************************
*
* Filename:             remote_modbus.c
*
*                       Date    Pgm.    Read.   Remark
* Modified              970317	CJu	-	-
*			030908	UL		For Linux
*			040505	UL		Fixed timeout for recieve to 34 chartimes
*
* Description:          Implements remote transport process for MODBUS
*
**************************************************************************
**************************************************************************/

/*_Include files_________________________________________________________*/

#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#if defined OS_LINUX
# include <termio.h>
#endif
#include <sgtty.h>
#include <sys/ioctl.h>

#include "remote_mq.h"
#include "pwr_class.h"
#include "pwr_systemclasses.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_remoteclasses.h"
#include "remote.h"
#include "remote_utils.h"
#include "remote_remtrans_utils.h"

#include "pwr.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_plc_utl.h"
#include "rt_errh.h"

/*_functions_______________________________________________________________*/

static unsigned int remnode_send(remnode_item *remnode,
                                 pwr_sClass_RemTrans *remtrans,
                                 char *buf,
                                 int buffer_size);
				 
void generate_crc(unsigned char *buf, int size, unsigned char *result);

static unsigned int Receive();

/*_variables_______________________________________________________________*/

remnode_item rn;
pwr_sClass_RemnodeModbus *rn_modbus;
int ser_fd;				/* file domininator for serial port */

float time_since_scan;
float time_since_rcv;

void load_timeval(struct timeval *tv, float t)
{
  tv->tv_sec = t;
  tv->tv_usec = (t-(float)tv->tv_sec) * 1000000;
}

/************************************************************************
**************************************************************************
*
* Namn : generate_crc
*
* Typ  : void
*
* Typ           Parameter              IOGF     Beskrivning
*
* Beskrivning : Generates CRC checksum for a buffer of unsigned chars
*
**************************************************************************
**************************************************************************/

void generate_crc(unsigned char *buf, int size, unsigned char *result)
{
  unsigned short int crc;
  unsigned short int gen_polynomial = 0xA001;
  unsigned short int flag_mask = 0x0001;
  unsigned short int flag;
  short int i, j;  

  crc = 0xFFFF;

  for (i=0; i<size; i++)
  {
    crc = crc^buf[i];

    for (j=0; j<8; j++)
    {
      flag = crc & flag_mask;
      crc = crc >> 1;
      if (flag) crc = crc ^ gen_polynomial;
    }
  }

  result[0] = (unsigned char) (crc & 0x00FF);
  result[1] = (unsigned char) ((crc >> 8) & 0x00FF);

  return;
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
* Beskrivning : Sends a RemTrans message to Remote node
*
**************************************************************************
**************************************************************************/

static unsigned int remnode_send(remnode_item *remnode,
                                 pwr_sClass_RemTrans *remtrans,
                                 char *buf,
                                 int buffer_size)
{
  int			sts;
//  int			size_of_telegram;
//  int			nbr_of_bytes_written = 0;
//  unsigned char         ch;
  unsigned char         telegram[516];

  if (buffer_size > sizeof(telegram)-4)
  {
    errh_Info("Remote modbus send failed, telegram to big");
    return(1);
  }

/*************************************************************************/
/**   Fill up telegram with contents of message and calculate checksum  **/
/*************************************************************************/

  telegram[0] = remtrans->Address[0];		/* Slave address */
  telegram[1] = remtrans->Address[1];		/* Function code */

  memcpy(&telegram[2], buf, buffer_size);
  generate_crc(telegram, buffer_size+2, &telegram[buffer_size+2]);  

/*************************************************************************/
/**    Execute the send procedure                                       **/
/*************************************************************************/

  sts=write(ser_fd, telegram, buffer_size+4);  

/*************************************************************************/
/**  Check final status.                                                **/
/*************************************************************************/

  if (sts > 0)
  {
    /* Send OK, receive answer */
    sts = Receive();
  }
  else 
  {
    /* The send procedure has failed */
    errh_Info("Remote modbus send failed, write() failed");
  }
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
* Beskrivning : Waits for message on serial line
*
**************************************************************************
**************************************************************************/

static unsigned int Receive()
{
  int sts, data_size=0;
//  int nbr_of_bytes_read = 0;
  unsigned char telegram[512];
//  unsigned char ch;
  unsigned char crc[2];
//  DDA$_EXTENDED_STATUS_INFO stsblk;
//  int error_logged = 0;
  char search_remtrans = FALSE;
  remtrans_item *remtrans;

  fd_set read_fd;
  struct timeval tv;

/**** set up timeout ****/

  load_timeval(&tv, rn_modbus->ReadTimeout);
/* old
  tv.tv_sec = 0;
  //tv.tv_usec = 10000; //(4 char timeout recieve vid 9600)
  tv.tv_usec = (1/portspeed)*10*34 //10000 (4 char timeout recieve vid 9600)
*/

/**routine**/  

  FD_ZERO(&read_fd);
  FD_SET(ser_fd, &read_fd);
  sts=select(ser_fd+1, &read_fd, NULL, NULL, &tv);
  sts=read(ser_fd, telegram, 1);

  if(sts>0)
  {
    while(sts>0)
    {
      data_size++;

      load_timeval(&tv, rn_modbus->ReadTimeout);
/* old
      tv.tv_sec = 0;
      tv.tv_usec = (1/portspeed)*10*34 //10000 (4 char timeout recieve vid 9600)
*/
      FD_ZERO(&read_fd);
      FD_SET(ser_fd, &read_fd);
      sts=select(ser_fd+1, &read_fd, NULL, NULL, &tv);
      sts=read(ser_fd, telegram+data_size, 1);
    }

    generate_crc(telegram, data_size-2, crc);
    if( crc[0]!=telegram[data_size-2] || crc[1]!=telegram[data_size-1])
    {
        rn_modbus->ErrCount++;
        errh_Info("Modbus remote CRC error, calc CRC:%02x%02x telegram CRC:%02x%02x",crc[0], crc[1],telegram[data_size-1],telegram[data_size]);
        return (FALSE);
    }
    else
    {
      search_remtrans = true;
      remtrans = rn.remtrans;

      while(remtrans && search_remtrans)
      {
        if ( remtrans->objp->Address[0] == telegram[0] &&
             remtrans->objp->Address[1] == telegram[1] &&
             remtrans->objp->Direction == REMTRANS_IN )
           search_remtrans = false;
        if ( search_remtrans ) remtrans = (remtrans_item *) remtrans->next;
      }   /* endwhile */

      if ( !search_remtrans )
      {
        sts = RemTrans_Receive(remtrans, (char *)&telegram[2], data_size-4);     
        if ( EVEN(sts) )
        {
          remtrans->objp->ErrCount++;
          return (FALSE);
        }
      }

      if (search_remtrans)
      {
        /* No corresponding RemTrans object found */
        rn_modbus->ErrCount++;
        errh_Info("Modbus remote error, no corresponding RemTrans address object found");
        return (FALSE);
      }
    }

  }
  return(sts);    
}


/***************************************************
 ******        Main routine        *****************
 ***************************************************/

int main(int argc, char *argv[])
{

  unsigned int sts;			/* Status from function calls etc. */
  char id[32];
  char pname[32];
  remtrans_item *remtrans;
  int i;

  /* Read arg number 2, should be id for this instance */

  if (argc >= 2)
    strcpy(id, argv[1]);
  else
    strcpy(id, "0");
    
  /* Build process name with id */

  sprintf((char *) pname, "rs_remmodbus_%s", id);

  /* Init of errh */

  errh_Init((char *) pname, errh_eAnix_remote);
  errh_SetStatus(PWR__SRVSTARTUP);

  /* Init of gdh */

  sts = gdh_Init((char *) pname);
  if ( EVEN(sts)) {
    errh_Error("gdh_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Arg number 3 should be my remnodes objid in string representation,
     read it, convert to real objid and store in remnode_item */

  sts = 0;
  if (argc >= 3) sts = cdh_StringToObjid(argv[2], &rn.objid);
  if ( EVEN(sts)) {
    errh_Error("cdh_StringToObjid, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Get pointer to RemnodeModbus object and store locally */
  
  sts = gdh_ObjidToPointer(rn.objid, (pwr_tAddress *) &rn_modbus);
  if ( EVEN(sts)) {
    errh_Error("cdh_ObjidToPointer, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Initialize some internal data and make standard remtrans init */

  rn.next = NULL;
  rn.local = NULL;		// We dont use local structure since we only have one remnode
  rn.retransmit_time = 10.0;	// Not used, but initialize anyway
  rn_modbus->ErrCount = 0;

  sts = RemTrans_Init(&rn);

  if ( EVEN(sts)) {
    errh_Error("RemTrans_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  time_since_scan = 0;
  time_since_rcv = 0;

  /* Store remtrans objects objid in remnode_modbus object */
  remtrans = rn.remtrans;
  i = 0;
  while(remtrans) {
    rn_modbus->RemTransObjects[i++] = remtrans->objid;
    if ( i >= (int)(sizeof(rn_modbus->RemTransObjects)/sizeof(rn_modbus->RemTransObjects[0])))
      break;
    remtrans = (remtrans_item *) remtrans->next;
  }

  /* Initialize device */

  ser_fd = RemUtils_InitSerialDev(rn_modbus->DevName, 
  				  rn_modbus->Speed, 
				  rn_modbus->DataBits,
				  rn_modbus->StopBits,
				  rn_modbus->Parity);
				  
  if (!ser_fd) {
    errh_Error("InitDev, %d", ser_fd);
    errh_SetStatus(PWR__SRVTERM);
    exit(0);
  }    
  
  /* Loop forever */

  while (!doomsday)
  {
    if (rn_modbus->Disable == 1) {
      errh_Fatal("Disabled, exiting");
      errh_SetStatus(PWR__SRVTERM);
      exit(0);
    }   
    sts = Receive();
    sts = RemTrans_Cyclic(&rn, &remnode_send);
  }
}
