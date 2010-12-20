/* 
 * Proview   $Id: rs_remote_mq.c,v 1.3 2006-04-24 13:22:23 claes Exp $
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
*		===============
*                P r o v i e w
*               ===============
**************************************************************************
*
* Filename:             rs_remote_wmq.c
*
* Description:		Remote transport process for Websphere Message Queue 
*                       as a client
*			For further information, please refer to Webspherer MQ 
*			documentation.
*
* Change log:		2010-12-08, Robert Karlsson
*			First version introduced in 4.8.0-2
*
*
**************************************************************************
**************************************************************************/

/*_Include files_________________________________________________________*/

#if defined PWRE_CONF_WMQ

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "pwr_class.h"
#include "pwr_systemclasses.h"
#include "rt_gdh.h"
#include "co_time.h"
#include "co_cdh.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_remoteclasses.h"
#include "rt_pwr_msg.h"
#include "rt_aproc.h"
#include "remote.h"
#include "remote_remtrans_utils.h"

// Message Q include files

#include <cmqc.h>

#define TIME_INCR 0.02
#define debug 0

remnode_item rn;
pwr_sClass_RemnodeWMQ *rn_wmq;

char mgr_name[MQ_Q_MGR_NAME_LENGTH];
MQHCONN Hconn;

MQOD   RcvObjDesc = {MQOD_DEFAULT};
MQLONG RcvOpenOptions; // options that control the open-call
MQOD   SndObjDesc = {MQOD_DEFAULT};
MQLONG SndOpenOptions; // options that control the open-call
char   rcv_que_name[MQ_Q_NAME_LENGTH];
char   snd_que_name[MQ_Q_NAME_LENGTH];
MQHOBJ RcvHobj; // object handle
MQHOBJ SndHobj; // object handle


/*************************************************************************
**************************************************************************
*
* RemoteSleep
*
**************************************************************************
**************************************************************************/

void RemoteSleep(float time)
{
  struct timespec rqtp, rmtp;

  rqtp.tv_sec = 0;
  rqtp.tv_nsec = (long int) (time * 1000000000);
  nanosleep(&rqtp, &rmtp);
  return;
}

/*************************************************************************
**************************************************************************
*
* Namn : wmq_receive
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Invoked when a MQ message is received.
*
**************************************************************************
**************************************************************************/

unsigned int wmq_receive()
{
  MQLONG CompCode;
  MQLONG Reason;

  MQMD MsgDesc = {MQMD_DEFAULT};

  MQLONG DataLength;
  MQCHAR Buffer[16384];
  MQGMO GetMsgOpts = {MQGMO_DEFAULT};

  MQLONG BufferLength = sizeof(Buffer);

  unsigned int sts;

  char search_remtrans;
  remtrans_item *remtrans;

  /* Set options */

  GetMsgOpts.Options = MQGMO_NO_WAIT + MQGMO_ACCEPT_TRUNCATED_MSG;

  MsgDesc.Encoding       = MQENC_NATIVE;
  MsgDesc.CodedCharSetId = MQCCSI_Q_MGR;

  /* Get message */

  MQGET(Hconn, RcvHobj, &MsgDesc, &GetMsgOpts, BufferLength, Buffer, &DataLength, &CompCode, &Reason);

  if (CompCode != MQCC_FAILED) {
  
    if (debug) printf("Received message %d\n", (int) DataLength);
  
    search_remtrans = true;

    remtrans = rn.remtrans;
    while(remtrans && search_remtrans) {
      if ((strncmp(remtrans->objp->TransName, (char *) MsgDesc.MsgId, MQ_MSG_ID_LENGTH) == 0) &&
	  (remtrans->objp->Direction == REMTRANS_IN)) {
        search_remtrans = false;
        sts = RemTrans_Receive(remtrans, (char *) &Buffer, DataLength);
	if (sts != STATUS_OK && sts != STATUS_BUFF) 
	  errh_Error("Error from RemTrans_Receive, status %d", sts, 0);
        break;
      }
      remtrans = (remtrans_item *) remtrans->next;
    }
    if (search_remtrans) {
      rn_wmq->ErrCount++;
      errh_Info("No remtrans for received message, msgid %s", MsgDesc.MsgId, 0);
    }
  }
  else if (Reason != MQRC_NO_MSG_AVAILABLE) {
    rn_wmq->ErrCount++;
    errh_Error("Receive failed, reason %d", Reason, 0);
  }

  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : wmq_send
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Sends a MQ message to Remote node
*
**************************************************************************
**************************************************************************/

unsigned int wmq_send(remnode_item *remnode,
			  pwr_sClass_RemTrans *remtrans,
			  char *buf,
			  int buf_size)

{
  MQLONG   CompCode;
  MQLONG   Reason;
  MQPMO    pmo = {MQPMO_DEFAULT};   /* put message options           */
  //  MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
  MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
  MQLONG   messlen;                /* message length                */

  pmo.Options = MQPMO_NO_SYNCPOINT | MQPMO_FAIL_IF_QUIESCING;

  //  pmo.Options |= MQPMO_NEW_MSG_ID;
  //  pmo.Options |= MQPMO_NEW_CORREL_ID;

  strncpy((char *) md.MsgId, remtrans->TransName, MQ_MSG_ID_LENGTH) ;
  memcpy(md.CorrelId, MQCI_NONE, sizeof(md.CorrelId));

  if ((remtrans->Address[0] <= MQPER_PERSISTENCE_AS_Q_DEF) &&
      (remtrans->Address[0] >= MQPER_NOT_PERSISTENT))
    md.Persistence = remtrans->Address[0];
  else 
    md.Persistence = MQPER_NOT_PERSISTENT; // | MQPRE_NOT_PERSISTENT
  
  messlen = buf_size;

  MQPUT(Hconn,                /* connection handle               */
	SndHobj,             /* object handle                   */
	&md,                 /* message descriptor              */
	&pmo,                /* default options (datagram)      */
	messlen,             /* message length                  */
	buf,                 /* message buffer                  */
	&CompCode,           /* completion code                 */
	&Reason);            /* reason code                     */

  /* report reason, if any */
  if (Reason != MQRC_NONE) {
    remtrans->ErrCount++;
    errh_Error("Send failed, msgid %s, Reason %d", md.MsgId, Reason, 0);
    printf("MQPUT ended with reason code %d\n", (int) Reason);
  }


  //  if (debug) printf("Sent message %d\n", (int) mq_sts);
			
  return( STATUS_OK );
}

/*************************************************************************
**************************************************************************
*
* Main
*
**************************************************************************
**************************************************************************/

int main(int argc, char *argv[])
{
  remtrans_item *remtrans;
  unsigned char id[32];
  unsigned char pname[32];
  
  pwr_tStatus sts;
  int i;
  float time_since_scan = 0.0;
  MQLONG CompCode;
  MQLONG Reason;
  
  
  
  /* Read arg number 2, should be id for this instance and id is our queue number */
  
  if (argc >= 2)
    strcpy((char *)id, argv[1]);
  else
    strcpy((char *)id, "0");
  
  /* Build process name with id */
  
  sprintf((char *) pname, "rs_remwmq_%s", id);
  
  /* Init of errh */
  
  errh_Init((char *) pname, errh_eAnix_remote);
  errh_SetStatus(PWR__SRVSTARTUP);
  
  /* Init of gdh */
  if (debug) printf("Before gdh_init\n");
  sts = gdh_Init((char *) pname);
  if ( EVEN(sts)) {
    errh_Fatal("gdh_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  /* Arg number 3 should be my remnodes objid in string representation,
     read it, convert to real objid and store in remnode_item */
  
  sts = 0;
  if (argc >= 3) sts = cdh_StringToObjid(argv[2], &rn.objid);
  if ( EVEN(sts)) {
    errh_Fatal("cdh_StringToObjid, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  /* Get pointer to RemnodeWMQ object and store locally */
  
  sts = gdh_ObjidToPointer(rn.objid, (pwr_tAddress *) &rn_wmq);
  if ( EVEN(sts)) {
    errh_Fatal("cdh_ObjidToPointer, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  /* Initialize some internal data and make standard remtrans init */
  
  rn.next = NULL;
  rn.local = NULL;		// We dont use local structure since we only have one remnode
  rn_wmq->ErrCount = 0;
  
  if (debug) printf("Before remtrans_init\n");
  
  sts = RemTrans_Init(&rn);
  
  if ( EVEN(sts)) {
    errh_Fatal("RemTrans_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  /* Store remtrans objects objid in remnode_mq object */
  remtrans = rn.remtrans;
  i = 0;
  while(remtrans) {
    rn_wmq->RemTransObjects[i++] = remtrans->objid;
    if ( i >= (int)(sizeof(rn_wmq->RemTransObjects) / sizeof(rn_wmq->RemTransObjects[0])))
      break;
    remtrans = (remtrans_item *) remtrans->next;
  }
  
  /* Variables for MQ calls */
  
  strncpy(mgr_name, rn_wmq->QueueManager, MQ_Q_MGR_NAME_LENGTH);
  //  strncpy(mgr_name, "hejsanqqq", sizeof(MQ_Q_MGR_NAME_LENGTH));
  
  
  /* Connect to specified queue manager */
  
  MQCONN(mgr_name, &Hconn, &CompCode, &Reason);
  
  if ((CompCode != MQCC_OK) | (Reason != MQRC_NONE)) {
    errh_Fatal("MQCONN failed, queue mgr: %s, Code: %d, Reason: %d", mgr_name, CompCode, Reason);
    errh_SetStatus(PWR__SRVTERM);
    exit(0);
  } 

  /* Open queue for receiving messages */
  
  strncpy(rcv_que_name, rn_wmq->RcvQueue, MQ_Q_NAME_LENGTH);
  //  strncpy(rcv_que_name, "hejsanqqq", sizeof(MQ_Q_NAME_LENGTH));

  /* Initialize object descriptor control block */

  strncpy(RcvObjDesc.ObjectName, rcv_que_name, MQ_Q_NAME_LENGTH);

  /* open queue for input but not if MQM stopping */

  RcvOpenOptions =  MQOO_INPUT_AS_Q_DEF | MQOO_FAIL_IF_QUIESCING;

  /* Open queue */

  MQOPEN(Hconn, &RcvObjDesc, RcvOpenOptions, &RcvHobj, &CompCode, &Reason);
  
  if ((CompCode != MQCC_OK) | (Reason != MQRC_NONE)) {
    errh_Fatal("MQOPEN failed, queue: %s, Code: %d, Reason: %d", rcv_que_name, CompCode, Reason);
    errh_SetStatus(PWR__SRVTERM);
    exit(0);
  }

  /* Open queue for sending messages */
  
  strncpy(snd_que_name, rn_wmq->SndQueue, MQ_Q_NAME_LENGTH);
  //  strncpy(snd_que_name, "hejsanqqq", sizeof(MQ_Q_NAME_LENGTH));

  /* Initialize object descriptor control block */

  strncpy(SndObjDesc.ObjectName, snd_que_name, MQ_Q_NAME_LENGTH);

  /* open queue for output but not if MQM stopping */

  SndOpenOptions =  MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING;

  MQOPEN(Hconn, &SndObjDesc, SndOpenOptions, &SndHobj, &CompCode, &Reason);

  if ((CompCode != MQCC_OK) | (Reason != MQRC_NONE)) {
    errh_Fatal("MQOPEN failed, queue: %s, Code: %d, Reason: %d", snd_que_name, CompCode, Reason);
    errh_SetStatus(PWR__SRVTERM);
    exit(0);
  }
  
  errh_SetStatus(PWR__SRUN);
  
  /* Set (re)start time in remnode object */
  
  time_GetTime(&rn_wmq->RestartTime);
  
  /* Loop forever */

  while (!doomsday) {
    if (rn_wmq->Disable == 1) {
      errh_Fatal("Disabled, exiting");
      errh_SetStatus(PWR__SRVTERM);
      exit(0);
    }   
    aproc_TimeStamp(TIME_INCR, 5);
    RemoteSleep(TIME_INCR);

    time_since_scan += TIME_INCR;

    sts = wmq_receive();

    if (time_since_scan >= rn_wmq->ScanTime) {
      sts = RemTrans_Cyclic(&rn, &wmq_send);
      time_since_scan = 0.0;
    }

  }
}

#else
#include <stdio.h>
int main()
{
  printf( "Remote WMQ not built for this release\n");
  return 0;
}
#endif
