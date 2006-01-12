/* 
 * Proview   $Id: rs_remotehandler.c,v 1.1 2006-01-12 06:39:33 claes Exp $
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

/************************************************************************
*
*                P S S - 9 0 0 0
*               =================
*************************************************************************
*
* Filename:             remotehandler.c
*
*                       Date    Pgm.    Read.   Remark
* Modified              950307	Hans Werner	ELN
*			950309  C Jurstrand	VMS
*			950309  C Jurstrand	VMS
*			950915  CS 		Converted to DECC compiler
*			960215  C Jurstrand	3964R and TCP/IP
*			960927  C Jurstrand	3964R_VNET and name for 1:st process
*			970602	C Jurstrand	MODBUS and ADLP10
*			971016	C Jurstrand	DMQ
*			980610	C Jurstrand	Universal serial prot.
*			980922	C Jurstrand	MOMENTUM
*			000330	C Jurstrand	3.0b and LIMAB and UDP/IP
*			000517	C Jurstrand	Lynx OS
*			001222  C Jurstrand	Varmomstart, QCOM
*			010405  C Jurstrand	3.3a, QCOM på ELN
*			040303  J Nylund	Ändrat till omgivningsvariabel
*                                               i sökvägarna till exe-filerna
*			040422	C Jurstrand	4.0.0
*			
* Description:
*       Start and control of transportprocesses for remote communication
*
**************************************************************************
**************************************************************************/

/********* Include files *************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "co_dcli.h"

#include "pwr.h"
#include "pwr_class.h"
#include "pwrs_c_node.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_pwr_msg.h"
#include "rt_ini_event.h"
#include "pwr_baseclasses.h"
#include "pwr_remoteclasses.h"
#include "remote.h"
#include "rt_plc_utl.h"
#include "rt_pcm.h"
#include "rt_aproc.h"

typedef struct {
  char path[64];
  pwr_tObjid objid;
  pwr_tAddress objref;
  pwr_tClassId classid;
  pwr_tBoolean *disable;
  pwr_tUInt32 *restart_limit;
  pwr_tUInt32 *restarts;
  pwr_tBoolean first;
  pid_t cpid;
  int id;
} Transport;

Transport tp[25];
int tpcount;
int tpmax = 25;

pwr_tObjid			remcfg_objid;
pwr_sClass_RemoteConfig		*remcfgp;

static void AddTransports();

static int StartTransport(unsigned int idx);

/************************************************************************
*
* AddTransport
*
************************************************************************/
static void AddTransports()
{   
  pwr_tObjid objid;
  pwr_tAddress objref;
  pwr_tStatus sts;
  
  /* Init transport (process) counter */
  tpcount = 0;
    
  /* Initialize transport vector with 0's */
  memset(&tp, 0, sizeof(tp));
  
  /* Get and configure all MQ remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_RemnodeMQ, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_mq"); 
    tp[tpcount].id = ((pwr_sClass_RemnodeMQ *) objref)->MyQueue;
    tp[tpcount].disable = &((pwr_sClass_RemnodeMQ *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeMQ *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeMQ *) objref)->RestartCount;
    ((pwr_sClass_RemnodeMQ *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeMQ;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }

  /* Get and configure all UDP remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_RemnodeUDP, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_udpip"); 
    tp[tpcount].id = ((pwr_sClass_RemnodeUDP *) objref)->LocalPort;
    tp[tpcount].disable = &((pwr_sClass_RemnodeUDP *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeUDP *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeUDP *) objref)->RestartCount;
    ((pwr_sClass_RemnodeUDP *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeUDP;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }
  
  /* Get and configure all TCP remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_RemnodeTCP, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_tcpip"); 
    tp[tpcount].id = ((pwr_sClass_RemnodeTCP *) objref)->LocalPort;
    tp[tpcount].disable = &((pwr_sClass_RemnodeTCP *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeTCP *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeTCP *) objref)->RestartCount;
    ((pwr_sClass_RemnodeTCP *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeTCP;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }

  /* Check if there is at least one ALCM remnode. If so, make an entry for rs_remote_alcm in
     the transport table. rs_remote_alcm handles all instances of remnodes in one process */

  sts = gdh_GetClassList (pwr_cClass_RemnodeALCM, &objid);
  if ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_alcm"); 
    tp[tpcount].id = 0;
    tp[tpcount].disable = &((pwr_sClass_RemnodeALCM *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeALCM *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeALCM *) objref)->RestartCount;
    ((pwr_sClass_RemnodeALCM *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeALCM;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
  }

  /* Get and configure all Serial remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_RemnodeSerial, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_serial"); 
    tp[tpcount].id = 0;
    tp[tpcount].disable = &((pwr_sClass_RemnodeSerial *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeSerial *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeSerial *) objref)->RestartCount;
    ((pwr_sClass_RemnodeSerial *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeSerial;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }

  /* Get and configure all Modbus remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_RemnodeModbus, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_modbus"); 
    tp[tpcount].id = 0;
    tp[tpcount].disable = &((pwr_sClass_RemnodeModbus *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_RemnodeModbus *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_RemnodeModbus *) objref)->RestartCount;
    ((pwr_sClass_RemnodeModbus *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_RemnodeModbus;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }

  /* Get and configure all 3964R remnodes, one process for each remnode */

  sts = gdh_GetClassList (pwr_cClass_Remnode3964R, &objid);
  while ( ODD(sts)) 
  {
    sts = gdh_ObjidToPointer(objid, &objref);
    sprintf(tp[tpcount].path, "rs_remote_3964r"); 
    tp[tpcount].id = 0;
    tp[tpcount].disable = &((pwr_sClass_Remnode3964R *) objref)->Disable;
    tp[tpcount].restart_limit = &((pwr_sClass_Remnode3964R *) objref)->RestartLimit;
    tp[tpcount].restarts = &((pwr_sClass_Remnode3964R *) objref)->RestartCount;
    ((pwr_sClass_Remnode3964R *) objref)->RestartCount = 0;
    tp[tpcount].objid = objid;
    tp[tpcount].objref = objref;
    tp[tpcount].classid = pwr_cClass_Remnode3964R;
    tp[tpcount].cpid = -1;
    tp[tpcount].first = true;
      
    remcfgp->RemNodeObjects[tpcount] = objid;

    tpcount++;
    sts = gdh_GetNextObject (objid, &objid);
  }


  return;
}

/************************************************************************
*
* StartTransport
*
************************************************************************/
static int StartTransport(unsigned int idx)
{
  char arg1[40];
  char arg2[40];
  char arg3[40];
  int res;
  
  /* Check index */
  
  if (idx >= tpcount || idx < 0) return -1;
  
  if (*tp[idx].disable || (*tp[idx].restarts >= *tp[idx].restart_limit)) {
    tp[idx].cpid = -1;
    return -1;
  }
  
  if (!tp[idx].first) (*tp[idx].restarts)++;
  tp[idx].first = false;

  memset(arg1, 0, sizeof(arg1));
  memset(arg2, 0, sizeof(arg2));
  memset(arg3, 0, sizeof(arg3));

  sprintf(arg1,"%s", tp[idx].path);
  sprintf(arg2,"%d", tp[idx].id);
  sprintf(arg3,"%s", cdh_ObjidToString(NULL, tp[idx].objid, 0));

  if (((tp[idx].cpid) = fork())) {
  }
  else {
    if (execlp(tp[idx].path, arg1, arg2, arg3, (char *) 0) < 0) {
        errh_Warning("Can't start transport %s", tp[idx].path);
	res = -1;
        _exit(0);
    }
    else {
      res = 1;
    }
  }

  return(res);
}

/************************************************************************
*
* main
*
************************************************************************/
int main()
{
  int				i;
  pwr_tStatus			sts;

  pid_t cpid;
  int stat_loc;

  pwr_tStatus status;
  qcom_sQattr qattr;
  qcom_sQid qini;
  qcom_sQid qid = qcom_cNQid;

  qcom_sGet get;
  char mp[2000];

  char hotswap;
  char new_plc;

  errh_Init("rs_remhdl", errh_eAnix_remote);
  errh_SetStatus(PWR__SRVSTARTUP);

  /* Qcom init */

  if (!qcom_Init(&status, 0, "rs_remhdl")) {
    errh_Error("qcom_Init, %m", status);
    errh_SetStatus(PWR__SRVTERM);
    exit(status);
  }

  qattr.type = qcom_eQtype_private;
  qattr.quota = 100;
  if (!qcom_CreateQ(&status, &qid, &qattr, "Restart")) {
    errh_Error("qcom_CreateQ, %m", status);
    errh_SetStatus(PWR__SRVTERM);
    exit(status);
  }

  qini = qcom_cQini;
  if (!qcom_Bind(&status, &qid, &qini)) {
    errh_Error("qcom_CreateQ, %m", status);
    errh_SetStatus(PWR__SRVTERM);
    exit(-1);
  }

  /* GDH init */

  sts = gdh_Init("rs_remhdl");
  if ( EVEN(sts)) {
    errh_Error("gdh_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  hotswap = 0;

  while (true) 
  {
    /* Get RemoteConfig object */
    sts = gdh_GetClassList(pwr_cClass_RemoteConfig, &remcfg_objid);
    if (ODD(sts)) {
      sts = gdh_ObjidToPointer(remcfg_objid, (pwr_tAddress *) &remcfgp);
      if ( EVEN(sts)) {
        errh_Error("gdh_ObjidToPointer, %m", sts);
        errh_SetStatus(PWR__SRVTERM);
        exit(sts);
      }
      aproc_RegisterObject( remcfg_objid);
      if (remcfgp->Disable) {
        errh_Info("Remote server disabled in RemoteConfig, rs_remotehandler will not run");
        errh_SetStatus(0);
        exit(1);    
      }
      for (i=0; i<tpmax; i++) remcfgp->RemNodeObjects[i] = pwr_cNObjid;
    }
    else {
      errh_Info("No RemoteConfig object found, rs_remotehandler will not run");
      errh_SetStatus(0);
      exit(1);    
    }

    // Add all remote transports

    AddTransports();
/*
    sumsts = 1;
    for (i=0; i<tpcount; i++) 
    {
      sts = StartTransport(i);
      if (sts < 0) sumsts = sts;
      errh_Info("Transport %s started by remotehandler", tp[i].path);
    }
    
    if (sumsts < 0) 
      errh_Warning("All transports started, at least one failed");
    else
      errh_Info("All transports started");
*/
    new_plc = 0;

    do {
      if (!hotswap) errh_SetStatus(PWR__SRUN);
      aproc_TimeStamp();
      get.maxSize = sizeof(mp);
      get.data = mp;
      qcom_Get(&status, &qid, &get, 100);  // TMO == 100 ms
      if (status == QCOM__TMO || status == QCOM__QEMPTY) {

        if (!hotswap) {
          cpid = waitpid(-1, &stat_loc, WNOHANG);
          for (i=0; i<tpcount; i++) {
            if (tp[i].cpid == -1 || cpid == tp[i].cpid) {
              sts = StartTransport(i);
//              errh_Warning("Transport %s terminated, restarted by remotehandler", tp[i].path);
            }
          }
        }
      }
      else {
        ini_mEvent new_event;
        qcom_sEvent *ep = (qcom_sEvent *) get.data;

        new_event.m = ep->mask;

        if (new_event.b.swapInit && !hotswap) {

          hotswap = 1;
	  errh_SetStatus(PWR__SRVRESTART);
          for (i=0; i<tpcount; i++) {
            if (tp[i].cpid > 0) {
              kill(tp[i].cpid, 9);
              cpid = waitpid(tp[i].cpid, &stat_loc, 0);
	      if (cpid != tp[i].cpid) {
                errh_Error("Error in waitpid, exiting"); 
                errh_SetStatus(PWR__SRVTERM);
	        exit(1);
              }
              tp[i].cpid = -1;
            }
          }
        }

        if (new_event.b.swapDone && hotswap) {
          hotswap = 0;
          new_plc = 1;
        }
	
        if (new_event.b.terminate) {
          for (i=0; i<tpcount; i++) {
            if (tp[i].cpid > 0) {
              kill(tp[i].cpid, 9);
              cpid = waitpid(tp[i].cpid, &stat_loc, 0);
	      if (cpid != tp[i].cpid) {
                errh_Error("Error in waitpid, exiting"); 
      	        errh_SetStatus(PWR__SRVTERM);
	        exit(1);
              }
              tp[i].cpid = -1;
            }
          }
	  errh_SetStatus(PWR__SRVTERM);
          exit(0);
        }
	qcom_Free(&sts, get.data);
      }
    } while (!new_plc);
  }

}
