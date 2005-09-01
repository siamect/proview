/* 
 * Proview   $Id: rt_io_utl.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_utl.c 
   Routines to synchronize with the IO job.  */

#if defined OS_ELN
#include $vaxelnc
#include $kernelmsg
#include $kerneldef
#endif

#include descrip
#include ssdef

#include "pwr.h"
#include "pwr_class.h"
#include "rt_io_msg.h"

typedef struct s_Info
{
  pwr_tBoolean IsStopped;
} t_Info;

#if defined OS_ELN
static AREA lIOUtlSynchArea;
#endif
static t_Info *lInfo;

static t_Info *Map()
{
  int sts;
  static $DESCRIPTOR(name, "IO_UTL_SYNCH_AREA");
  static pwr_tBoolean IsMapped = FALSE;

  /* Map IO_UTL_SYNCH_AREA */

  if (IsMapped) return;

#if defined OS_ELN
  ker$create_area_event(&sts, &lIOUtlSynchArea,
	&lInfo, sizeof(*lInfo), &name, EVENT$CLEARED, NULL);
  if (sts != KER$_AREA_EXISTS)
	memset(lInfo, 0, sizeof(lInfo));
#elif defined OS_VMS
  lInfo = (t_Info*) calloc(1, sizeof(*lInfo));
  lInfo->IsStopped = FALSE;
#endif
  IsMapped = TRUE;
  return( lInfo );
}

/*
 * pwr_tUInt32 io_WaitForStop()
 *
 * Description:
 *  This routine will make the caller wait for the IO-job to stop.
 */ 

pwr_tUInt32 io_WaitForStop()
{
#if defined OS_ELN
  Map();
  ker$wait_any(NULL, NULL, NULL, lIOUtlSynchArea);
#endif
  return(SS$_NORMAL);
}

/*
 * pwr_tBoolean io_IsStopped()
 *
 * Description:
 *  This routine will tell if the IO-job has stopped
 */ 

pwr_tBoolean io_IsStopped()
{
  t_Info *lInfo;
  lInfo = Map();
  return(lInfo->IsStopped);
}

/*
 * int io_SignalStopped()
 *
 * Description:
 *  This routine will wake everybody waiting for the IO to stop.
 *
 */ 
pwr_tUInt32 io_SignalStopped()
{
  t_Info *lInfo;
  lInfo = Map();

  lInfo->IsStopped = TRUE;
#if defined OS_ELN
  ker$signal(NULL, lIOUtlSynchArea);
#endif
  return(SS$_NORMAL);
}

pwr_tStatus io_StopIO(
  int Areaindex,
  PORT *IOjob_portp
)
{
  int sts, sts1;
  MESSAGE	TerminateMsg;
  char		*Msg;
  t_Info *lInfo;

  lInfo = Map();
  /* Create a message and send it */
  ker$create_message( &sts, &TerminateMsg, &Msg, sizeof(char));
  if (EVEN(sts))
    errh_CErrLog(IO__ERRCREMSG,errh_ErrArgMsg(sts), NULL);

  /* Send the dummy message */
  ker$send( &sts, TerminateMsg, sizeof( char ), IOjob_portp, NULL, FALSE);
  if (EVEN(sts))
  {
    errh_CErrLog(IO__ERRSNDMSG,errh_ErrArgMsg(sts), NULL);
    ker$delete( &sts1, TerminateMsg );
  }
  else
    io_WaitForStop();

  return( sts );
}

pwr_tStatus io_TerminateIO(
  int Areaindex,
  PORT *IOjob_portp
)
{
  int sts, sts1;
  MESSAGE	TerminateMsg;
  char		*Msg;
  t_Info *lInfo;

  lInfo = Map();
  /* Create a message and send it */
  ker$create_message( &sts, &TerminateMsg, &Msg, sizeof(char));
  if (EVEN(sts))
    errh_CErrLog(IO__ERRCREMSG,errh_ErrArgMsg(sts), NULL);

  /* Send the dummy message */
  ker$send( &sts, TerminateMsg, sizeof( char ), IOjob_portp, NULL, FALSE);
  if (EVEN(sts))
  {
    errh_CErrLog(IO__ERRSNDMSG,errh_ErrArgMsg(sts), NULL);
    ker$delete( &sts1, TerminateMsg );
  }

  return( sts );
}

pwr_tStatus
io_StartIoComm(
  PORT *IOjob_port
)
{
 pwr_tStatus	sts;
 PORT		IOport;
 $DESCRIPTOR	(IOportname,"IO_COMM_PORT"); 
 $DESCRIPTOR	(IOjobname,"RT_IO_COMM"); 

 /* Test if the analogue communication job IO_COMM is running.
    If not we must create it. */

 ker$translate_name(&sts, &IOport, &IOportname, NAME$LOCAL);		
 if (EVEN(sts) &&  sts != KER$_NO_SUCH_NAME)
     return(sts);

 /* Is IO_COMMs port created */
 if (sts == KER$_NO_SUCH_NAME)
 {
   /* IO_COMM was not active, create it */

   ker$create_job(&sts, &IOjob_port, &IOjobname, NULL);
   if (EVEN(sts)) return(sts);
 }

 return IO__SUCCESS;
}


