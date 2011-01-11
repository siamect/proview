/* 
 * Proview   $Id: rt_io_m_motioncontrol_usbio.c,v 1.1 2007-11-22 13:28:59 claes Exp $
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

/* rt_io_m_motioncontrol_usbio.c -- I/O methods for class MotionControl_USBIO. */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "co_time.h"
#include "rt_io_base.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "rt_io_card_write.h"
#include "rt_io_msg.h"

#define D_MAX_SIZE 10
#define AI_MAX_SIZE 4
#define AO_MAX_SIZE 4

typedef struct {
  pwr_tTime ErrTime;
  int fd;
  int DiSize;
  int DoSize;
  int AiSize;
  int AoSize;
  int AiNum;
  int AoNum;
  unsigned char DiMask[D_MAX_SIZE];
  unsigned char DoMask[D_MAX_SIZE];
  unsigned char AiMask[AI_MAX_SIZE];
  unsigned char AoMask[AO_MAX_SIZE];
  int IdCnt;
  int DiPollId;
  int DiPendingPoll;
  int AiIntervalCnt;
  io_sChannel *DChanList[D_MAX_SIZE * 8];
  io_sChannel *AiChanList[AI_MAX_SIZE * 8];
  io_sChannel *AoChanList[AO_MAX_SIZE * 8];
} io_sLocal;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <termios.h>

static FILE *fp;

static void logg( const char *str)
{
  pwr_tTime t;
  char timstr[40];

  time_GetTime( &t);
  time_AtoAscii( 0, time_eFormat_Time, timstr, sizeof(timstr));
  timstr[9] = 0;

  fprintf( fp, "%s%04lld %s\n", timstr, t.tv_nsec/100000, str);
}

typedef enum {
  ard_eMsgType_Write 	= 1,
  ard_eMsgType_DiRead 	= 2,
  ard_eMsgType_AiRead 	= 3,
  ard_eMsgType_Configure = 4,
  ard_eMsgType_Status 	= 5,
  ard_eMsgType_Debug 	= 6
} ard_eMsgType;

#define ARD__SUCCESS 1
#define ARD__DICONFIG 2
#define ARD__DOCONFIG 4
#define ARD__AICONFIG 6
#define ARD__AOCONFIG 8
#define ARD__COMMERROR 10
#define ARD__MSGSIZE 12
#define ARD__NOMSG 14

typedef struct {
  unsigned char size __attribute__ ((aligned(1)));
  unsigned char id __attribute__ ((aligned(1)));
  unsigned char type __attribute__ ((aligned(1)));
  unsigned char data[100] __attribute__ ((aligned(1)));
} ard_sMsg;

static int receive( int fd, int id, ard_sMsg *rmsg, int size)
{
  fd_set rfd;
  struct timeval tv = {1,0};
  int sts;
  int msize;

  FD_ZERO(&rfd);
  FD_SET(fd, &rfd);
  while ( 1) {
    sts = select(fd+1, &rfd, NULL, NULL, &tv);
    if ( sts == 0) return ARD__NOMSG;
    
    msize = 0;
    msize += read( fd, rmsg, 1);

    logg( "Receive read");

    while ( msize < rmsg->size) {
      sts = select(fd+1, &rfd, NULL, NULL, &tv);
      if ( sts == 0) return ARD__NOMSG;

      msize += read( fd, (char *)rmsg + msize, rmsg->size - msize);

      logg( "Receive read ++");

    }

    if ( rmsg->type == ard_eMsgType_Debug) {
      int i;
      printf( "Debug (id %d, size %d, type %d): ", rmsg->id, rmsg->size - 3,
	      rmsg->type);
      for ( i = 0; i < rmsg->size - 3; i++)
	printf( "%u ", rmsg->data[i]);
      printf( "\n");
    }
    else {
      if ( rmsg->size == size + 3 && rmsg->id == id)
	return ARD__SUCCESS;
    }
  }
  return ARD__NOMSG;
}

static int poll_di( ard_sMsg *msg, io_sLocal *local)
{
  int sts;

  msg->size = 3;
  msg->id = local->IdCnt++;
  msg->type = ard_eMsgType_DiRead;

  logg( "Poll Di");
  sts = write( local->fd, msg, msg->size);
  local->DiPollId = msg->id;
  local->DiPendingPoll = 1;
  return sts;
}      


static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  io_sLocal *local;
  pwr_sClass_Arduino_Uno *op = (pwr_sClass_Arduino_Uno *)cp->op;
  unsigned int number, number_byte, number_bit;
  pwr_tStatus sts;
  int i;
  unsigned char wdg;
  struct termios tty_attributes;

  fp = fopen( "/home/claes/ard.log", "w"); // Test

  local = (io_sLocal *) calloc( 1, sizeof(io_sLocal));
  cp->Local = local;
  local->IdCnt = 1;

  for ( i = 0; i < cp->ChanListSize; i++) {
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
      number = ((pwr_sClass_ChanDi *)cp->chanlist[i].cop)->Number;
      number_byte = number / 8;
      number_bit = number % 8;
      if ( number_byte >= D_MAX_SIZE) {
	errh_Error( "IO Init Card '%s', max number exceeded", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_DiConfigureError;
	return IO__INITFAIL;
      }
      else if ( local->DChanList[number]) {
	errh_Error( "IO Init Card '%s', number already configured", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_DiConfigureError;
	return IO__INITFAIL;
      }
      else {
	if ( local->DiSize < number_byte + 1)
	  local->DiSize = number_byte + 1;
	local->DiMask[number_byte] |= 1 << number_bit;
	local->DChanList[number] = &cp->chanlist[i];
      }
    }
    else if ( cp->chanlist[i].cop && 
	      cp->chanlist[i].sop && 
	      cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
      number = ((pwr_sClass_ChanDo *)cp->chanlist[i].cop)->Number;
      number_byte = number / 8;
      number_bit = number % 8;
      if ( number_byte >= D_MAX_SIZE) {
	errh_Error( "IO Init Card '%s', max number exceeded", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_DoConfigureError;
	return IO__INITFAIL;
      }
      else if ( local->DChanList[number]) {
	errh_Error( "IO Init Card '%s', number already configured", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_DoConfigureError;
	return IO__INITFAIL;
      }
      else {
	if ( local->DoSize < number_byte + 1)
	  local->DoSize = number_byte + 1;
	local->DoMask[number_byte] |= 1 << number_bit;
	local->DChanList[number] = &cp->chanlist[i];
      }
    }
    else if ( cp->chanlist[i].cop && 
	      cp->chanlist[i].sop && 
	      cp->chanlist[i].ChanClass == pwr_cClass_ChanAi) {
      number = ((pwr_sClass_ChanAi *)cp->chanlist[i].cop)->Number;
      number_byte = number / 8;
      number_bit = number % 8;
      if ( number_byte >= AI_MAX_SIZE) {
	errh_Error( "IO Init Card '%s', max number exceeded", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_AiConfigureError;
	return IO__INITFAIL;
      }
      else if ( local->AiChanList[number]) {
	errh_Error( "IO Init Card '%s', number already configured", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_AiConfigureError;
	return IO__INITFAIL;
      }
      else {
	if ( local->AiSize < number_byte + 1)
	  local->AiSize = number_byte + 1;
	local->AiMask[number_byte] |= 1 << number_bit;
	local->AiNum++;
	local->AiChanList[number] = &cp->chanlist[i];

	// Calculate conversion coefficients
	io_AiRangeToCoef( &cp->chanlist[i]);
      }
    }
    else if ( cp->chanlist[i].cop && 
	      cp->chanlist[i].sop && 
	      cp->chanlist[i].ChanClass == pwr_cClass_ChanAo) {
      number = ((pwr_sClass_ChanAo *)cp->chanlist[i].cop)->Number;
      number_byte = number / 8;
      number_bit = number % 8;
      if ( number_byte >= AO_MAX_SIZE) {
	errh_Error( "IO Init Card '%s', max number exceeded", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_AoConfigureError;
	return IO__INITFAIL;
      }
      else if ( local->AoChanList[number]) {
	errh_Error( "IO Init Card '%s', number already configured", cp->Name);
	op->Status = pwr_eArduino_StatusEnum_AoConfigureError;
	return IO__INITFAIL;
      }
      else {
	if ( local->AoSize < number_byte + 1)
	  local->AoSize = number_byte + 1;
	local->AoMask[number_byte] |= 1 << number_bit;
	local->AoChanList[number] = &cp->chanlist[i];
	local->AoNum++;

	// Calculate conversion coefficients
	io_AoRangeToCoef( &cp->chanlist[i]);
      }
    }
  }

  // Configure Watchdog
  if ( op->WatchdogTime == 0)
    wdg = 0;
  else if ( op->WatchdogTime > 255)
    wdg = 255;
  else {
    wdg = op->WatchdogTime * 10;
    if ( wdg < 1)
      wdg = 1;    
  }  

  // Open device
  local->fd = open( op->Device, O_RDWR | O_NDELAY | O_NOCTTY);
  if ( local->fd == -1) {
    errh_Error( "IO Init Card '%s', unable to open device %s", cp->Name, op->Device);
    op->Status = pwr_eArduino_StatusEnum_NoSuchDevice;
    return IO__INITFAIL;
  }

  tcgetattr( local->fd, &tty_attributes); 
#if defined OS_LINUX
  tty_attributes.c_cflag &= ~CBAUD;  //maska bort all hastighet
#endif
  switch( op->BaudRate) {
    case 300:
      tty_attributes.c_cflag |= B300;
      break;
    case 1200:
      tty_attributes.c_cflag |= B1200;
      break;
    case 2400:
      tty_attributes.c_cflag |= B2400;
      break;
    case 4800:
      tty_attributes.c_cflag |= B4800;
      break;
    case 9600:
      tty_attributes.c_cflag |= B9600;
      break;
    case 19200:
      tty_attributes.c_cflag |= B19200;
      break;
    case 38400:
      tty_attributes.c_cflag |= B38400;
      break;
    case 57600:
      tty_attributes.c_cflag |= B57600;
      break;
    case 115200:
      tty_attributes.c_cflag |= B115200;
      break;
    default:
      tty_attributes.c_cflag |= B9600;
      break;
  }

  sts = tcsetattr( local->fd, TCSANOW, &tty_attributes);
  if ( sts < 0) {
    errh_Error( "IO Init Card '%s', unable to set baud rate on device %s", cp->Name, op->Device);
    op->Status = pwr_eArduino_StatusEnum_DeviceSetupError;
    return IO__INITFAIL;
  }

  tcflush( local->fd, TCIOFLUSH);

  // Send config message
  ard_sMsg msg, rmsg;
  int offs = 0;

  msg.size = 3;
  msg.id = local->IdCnt++;
  msg.type = ard_eMsgType_Configure;

  msg.data[offs++] = wdg;

  msg.data[offs++] = local->DiSize;
  for ( i = 0; i < local->DiSize; i++)
    msg.data[offs++] = local->DiMask[i];

  msg.data[offs++] = local->DoSize;
  for ( i = 0; i < local->DoSize; i++)
    msg.data[offs++] = local->DoMask[i];

  msg.data[offs++] = local->AiSize;
  for ( i = 0; i < local->AiSize; i++)
    msg.data[offs++] = local->AiMask[i];

  msg.data[offs++] = local->AoSize;
  for ( i = 0; i < local->AoSize; i++)
    msg.data[offs++] = local->AoMask[i];

  msg.size += offs;

  printf( "Config (size=%d, id= %d): ", offs, msg.id);
  for ( i = 0; i < offs; i++)
    printf( "%u ", msg.data[i]);
  printf( "\n");

  sleep(5);

  sts = write( local->fd, &msg, msg.size);
      
  sts = receive( local->fd, msg.id, &rmsg, 1);
  if ( sts & 1) {
    op->Status = rmsg.data[0];    
  }
  else {
    errh_Error( "IO Init Card '%s', config error: %d", cp->Name, sts);
    op->Status = sts;
    return IO__INITFAIL;
    printf( "Config read error %d\n", sts);
  }

  errh_Info( "Init of Arduino card '%s'", cp->Name);

  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp,
			        io_sCard *cp)
{
  io_sLocal *local = cp->Local;

  close( local->fd);
  free( cp->Local);

  fclose(fp); //Test

  return IO__SUCCESS;
}

static pwr_tStatus IoCardRead( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocal *local = cp->Local;
  pwr_sClass_Arduino_Uno *op = (pwr_sClass_Arduino_Uno *)cp->op;
  int i, j;
  unsigned int m;
  pwr_tUInt32 error_count = op->ErrorCount;
  int sts;

  if ( local->DiSize) {
    ard_sMsg msg, rmsg;
    int sts;
    int i, j;
    unsigned char m;


    if ( !local->DiPendingPoll)
      poll_di( &msg, local);

#if 0
    msg.size = 3;
    msg.id = local->IdCnt++;
    msg.type = ard_eMsgType_DiRead;

    logg( "Poll Di");
    sts = write( local->fd, &msg, msg.size);
#endif
      
    local->DiPendingPoll = 0;

    sts = receive( local->fd, local->DiPollId, &rmsg, local->DiSize);
    op->Status = sts;
    if ( EVEN(sts)) {
      op->ErrorCount++;
    }
    else {
      // printf( "Read: %u %u  (%d)\n", rmsg.data[0], rmsg.data[1], sts);

      for ( i = 0; i < local->DiSize; i++) {
	for ( j = 0; j < 8; j++) {
	  m = 1 << j;
	  if ( local->DiMask[i] & m)
	    *(pwr_tBoolean *)local->DChanList[i*8+j]->vbp = ((rmsg.data[i] & m) != 0);
	}
      }
    }
  }

  if ( local->AiSize) {
    int skip_ai = 0;

    if ( op->AiScanInterval > 1) {
      skip_ai = local->AiIntervalCnt;

      local->AiIntervalCnt++;
      if ( local->AiIntervalCnt >= op->AiScanInterval)
	local->AiIntervalCnt = 0;
    }

    if ( !skip_ai) {
      ard_sMsg msg, rmsg;
      pwr_tInt32 ivalue;
      pwr_tFloat32 actvalue;

      msg.size = 3;
      msg.id = local->IdCnt++;
      msg.type = ard_eMsgType_AiRead;

      sts = write( local->fd, &msg, msg.size);
      
      sts = receive( local->fd, msg.id, &rmsg, local->AiNum * 2);
      if ( EVEN(sts)) {
      }
      else {
	for ( i = 0; i < local->AiSize; i++) {
	  for ( j = 0; j < 8; j++) {
	    m = 1 << j;
	    if ( local->AiMask[i] & m) {
	      io_sChannel *chanp = local->AiChanList[i*8+j];
	      pwr_sClass_ChanAi *cop = (pwr_sClass_ChanAi *)chanp->cop;
	      pwr_sClass_Ai *sop = (pwr_sClass_Ai *)chanp->sop;
	      
	      if ( cop->CalculateNewCoef)
		// Request to calculate new coefficients
		io_AiRangeToCoef( chanp);
	      
	      ivalue = rmsg.data[(i*8+j)*2] * 256 + rmsg.data[(i*8+j)*2+1];
	      io_ConvertAi( cop, ivalue, &actvalue);

	      // Filter
	      if ( sop->FilterType == 1 &&
		   sop->FilterAttribute[0] > 0 &&
		   sop->FilterAttribute[0] > ctx->ScanTime) {
		actvalue = *(pwr_tFloat32 *)chanp->vbp + ctx->ScanTime / sop->FilterAttribute[0] *
		  (actvalue - *(pwr_tFloat32 *)chanp->vbp);
	      }
	      
	      *(pwr_tFloat32 *)chanp->vbp = actvalue;
	      sop->SigValue = cop->SigValPolyCoef1 * ivalue + cop->SigValPolyCoef0;
	      sop->RawValue = ivalue;
	    }	    
	  }
	}
      }
    }
  }

  if ( op->ErrorCount >= op->ErrorSoftLimit && 
       error_count < op->ErrorSoftLimit) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
  }
  if ( op->ErrorCount >= op->ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }

  return IO__SUCCESS;
}

static pwr_tStatus IoCardWrite( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocal *local = cp->Local;
  pwr_sClass_Arduino_Uno *op = (pwr_sClass_Arduino_Uno *)cp->op;
  int i, j;
  unsigned char m;
  pwr_tUInt32 error_count = op->ErrorCount;
  ard_sMsg msg;
  int sts;

  if ( local->DoSize) {

    memset( &msg, 0, sizeof(msg));
    msg.size = local->DoSize + 3;
    msg.id = local->IdCnt++;
    msg.type = ard_eMsgType_Write;

    for ( i = 0; i < local->DoSize; i++) {
      for ( j = 0; j < 8; j++) {
	m = 1 << j;
	if ( local->DoMask[i] & m) {
	  if ( *(pwr_tBoolean *)local->DChanList[i*8+j]->vbp)
	    msg.data[i] |= m;
	}
      }
    }


    logg( "Write Do");
    sts = write( local->fd, &msg, msg.size);
  }
 
  if ( op->ErrorCount >= op->ErrorSoftLimit && 
       error_count < op->ErrorSoftLimit) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
  }    
  if ( op->ErrorCount >= op->ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  if ( local->DiSize && 
       op->Options & pwr_mArduino_OptionsMask_OptimizedDiPoll) {
    ard_sMsg msg;

    poll_di( &msg, local);
  }

  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(Arduino_Uno) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
