/*
  Test mh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_mh_util.h"
#include "rt_mh_appl.h"
#include "ra_mhappltest.h"
#include "rt_pwr_msg.h"
#include "rt_plc_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static int plog = 1;
ra_mhappltest *ra_mhappltest::m_mh = 0;

typedef struct {
  pwr_tOid object;
  unsigned int eventflags;
  pwr_tTime eventtime;
  pwr_tString80 eventname;
  mh_eEvent eventtype;
  pwr_tString80 eventtext;
  mh_eEventPrio eventprio;
  int cancel;
  pwr_tUInt32 id;
  mh_sEventId eventid;
  int active;
  int eactive;
  int status;
  int estatus;
  int check;
} sApplMessage;

sApplMessage d[] = {
  // Alarm, no return
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 1", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 2", mh_eEventPrio_B, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 3", mh_eEventPrio_C, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 4", mh_eEventPrio_D, 0, 0},

  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_MaintenanceAlarm, "mh_appltest 5", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_SystemAlarm, "mh_appltest 6", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_UserAlarm1, "mh_appltest 7", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_UserAlarm2, "mh_appltest 8", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_UserAlarm3, "mh_appltest 9", mh_eEventPrio_A, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_UserAlarm4, "mh_appltest 10", mh_eEventPrio_A, 0, 0},
  // A alarm, return required
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Return, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 11", mh_eEventPrio_B, 0, 0},
  // A alarm, cancel. Doesn't work...
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Return, pwr_cNTime, "mhappltest", mh_eEvent_Alarm, "mh_appltest 12", mh_eEventPrio_B, 0 /* 1 todo */, 0},

  // Info in operator window
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned | mh_mEventFlags_InfoWindow, pwr_cNTime, "mhappltest", mh_eEvent_InfoSuccess, "mh_appltest 13", mh_eEventPrio_, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Ack | mh_mEventFlags_Returned | mh_mEventFlags_InfoWindow, pwr_cNTime, "mhappltest", mh_eEvent_Info, "mh_appltest 14", mh_eEventPrio_, 0, 0},

  // Info, eventlist only
  {pwr_cNOid, mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_InfoSuccess, "mh_appltest 15", mh_eEventPrio_, 0, 0},
  {pwr_cNOid, mh_mEventFlags_Returned, pwr_cNTime, "mhappltest", mh_eEvent_Info, "mh_appltest 16", mh_eEventPrio_, 0, 0}
};

pwr_tStatus ra_mhappltest::mh_ack_bc(mh_sAck* mp)
{
  int i;

  if (m_mh->m_clean)
    return 1;

  if (plog)
    m_mh->m_log->vlog('D', "ack_bc %d", mp->TargetId.Idx);
  i = m_mh->find_sup(&mp->TargetId);
  if (i == -1) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "ack_cb, unknown eventid");
    return 1;
  }
  d[i].status &= ~mh_mEventStatus_NotAck;
  if (d[i].status != d[i].estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "ack_bc status differs %d %d", d[i].status, d[i].estatus);
  }
  if ((d[i].estatus & mh_mEventStatus_NotRet) == 0 && 
      (d[i].estatus & mh_mEventStatus_NotAck) == 0) {
    d[i].active = 0;
    d[i].eactive = 0;
    d[i].estatus = 0;
    d[i].status = 0;
  }
  return 1;
}

pwr_tStatus ra_mhappltest::mh_return_bc(mh_sReturn* mp)
{
  unsigned int num;
  int i;

  if (m_mh->m_clean) {
    mh_OutunitAck(&mp->TargetId);
    return 1;
  }
  
  if (plog)
    m_mh->m_log->vlog('D', "return_bc %d", mp->TargetId.Idx);
  num = sscanf(&mp->EventText[12], "%d", &i);
  if (num != 1 || i < 1 || i > (int)(sizeof(d)/sizeof(d[0]))) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong index, %s", mp->EventText);
    return 1;
  }
  i--;
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, d[i].object)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong object, %s", mp->EventText);
    return 1;
  }
  if (mp->TargetId.Idx != d[i].eventid.Idx) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong eventid, %s", mp->EventText);
    return 1;
  }
  d[i].status &= ~mh_mEventStatus_NotRet;
  if (d[i].status != d[i].estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc status differs %d %d", d[i].status, d[i].estatus);
  }
  if ((d[i].estatus & mh_mEventStatus_NotRet) == 0 && 
      (d[i].estatus & mh_mEventStatus_NotAck) == 0) {
    d[i].eactive = 0;
    d[i].active = 0;
    d[i].estatus = 0;
    d[i].status = 0;
  }

  return 1;
}

pwr_tStatus ra_mhappltest::mh_alarm_bc(mh_sMessage* mp)
{
  int num;
  int i;

  if (m_mh->m_clean) {
    if (mp->Status & mh_mEventStatus_NotAck)
      mh_OutunitAck(&mp->Info.Id);
    return 1;
  }

  if (plog)
    m_mh->m_log->vlog('D', "alarm_bc %d", mp->Info.Id.Idx);

  num = sscanf(&mp->EventText[12], "%d", &i);
  if (num != 1 || i < 1 || i > (int)(sizeof(d)/sizeof(d[0]))) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong index, %s", mp->EventText);
    return 1;
  }
  i--;
  if (mp->Info.Id.Idx != d[i].id) {
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc old id, %d %s", mp->Info.Id.Idx, mp->EventText);
    return 1;
  }
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, d[i].object)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong object, %s", mp->EventText);
    return 1;
  }
  d[i].active = 1;
  if (!d[i].eactive) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc sup not active, %d %s", mp->Info.Id.Idx, 
	     mp->EventText);
    return 1;
  }

  memcpy(&d[i].eventid, &mp->Info.Id, sizeof(mh_sEventId));
  d[i].status = mp->Status & 0x7;
  if (d[i].status != d[i].estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc status differs %d %d %d", d[i].status, d[i].estatus,
	     mp->Info.Id.Idx);
    return 1;
  }
  return 1;
}

pwr_tStatus ra_mhappltest::mh_block_bc(mh_sBlock* mp)
{
  return 1;
}

pwr_tStatus ra_mhappltest::mh_cancel_bc(mh_sReturn* mp)
{
  unsigned int num;
  int i;

  if (m_mh->m_clean) {
    return 1;
  }
  
  if (plog)
    m_mh->m_log->vlog('D', "cancel_bc %d", mp->TargetId.Idx);
  num = sscanf(&mp->EventText[12], "%d", &i);
  if (num != 1 || i < 1 || i > (int)(sizeof(d)/sizeof(d[0]))) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "cancel_bc wrong index, %s", mp->EventText);
    return 1;
  }
  i--;
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, d[i].object)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "cancel_bc wrong object, %s", mp->EventText);
    return 1;
  }
  if (mp->TargetId.Idx != d[i].eventid.Idx) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "cancel_bc wrong eventid, %s", mp->EventText);
    return 1;
  }
  d[i].status &= ~mh_mEventStatus_NotRet;
  if (d[i].status != d[i].estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "cancel_bc status differs %d %d", d[i].status, d[i].estatus);
  }
  if ((d[i].estatus & mh_mEventStatus_NotRet) == 0 && 
      (d[i].estatus & mh_mEventStatus_NotAck) == 0) {
    d[i].eactive = 0;
    d[i].active = 0;
    d[i].estatus = 0;
    d[i].status = 0;
  }

  return 1;
}

pwr_tStatus ra_mhappltest::mh_info_bc(mh_sMessage* mp)
{
  unsigned int num;
  int i;

  if (m_mh->m_clean) {
    if (mp->Status & mh_mEventStatus_NotAck)
      mh_OutunitAck(&mp->Info.Id);
    return 1;
  }

  if (plog)
    m_mh->m_log->vlog('D', "info_bc %d", mp->Info.Id.Idx);

  num = sscanf(&mp->EventText[12], "%d", &i);
  if (num != 1 || i < 1 || i > (int)(sizeof(d)/sizeof(d[0]))) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc wrong index, %s", mp->EventText);
    return 1;
  }
  i--;
  if (mp->Info.Id.Idx != d[i].id) {
    if (plog)
      m_mh->m_log->vlog('X', "info_bc old id, %d %s", mp->Info.Id.Idx, mp->EventText);
    return 1;
  }
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, d[i].object)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc wrong object, %s", mp->EventText);
    return 1;
  }
  d[i].active = 1;
  if (!d[i].eactive) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc sup not active, %d %s", d[i].eventid.Idx, 
	     mp->EventText);
    return 1;
  }
  memcpy(&d[i].eventid, &mp->Info.Id, sizeof(mh_sEventId));
  d[i].status = mp->Status & 0x7;
  if (d[i].status != d[i].estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc status differs %d %d %d", d[i].status, d[i].estatus,
	     d[i].eventid.Idx);
    return 1;
  }
  if ((d[i].estatus & mh_mEventStatus_NotRet) == 0 && 
      (d[i].estatus & mh_mEventStatus_NotAck) == 0) {
    d[i].eactive = 0;
    d[i].active = 0;
    d[i].estatus = 0;
    d[i].status = 0;
  }
  return 1;
}

pwr_tStatus ra_mhappltest::mh_clear_alarmlist_bc(pwr_tNodeIndex nix)
{
  return 1;
}

pwr_tStatus ra_mhappltest::mh_clear_blocklist_bc(pwr_tNodeIndex nix)
{
  return 1;
}

pwr_tStatus ra_mhappltest::mh_alarmstatus_bc(mh_sAlarmStatus* mp)
{
  int i;

  if (!m_mh->m_checkstatus)
    return 1;

  // Reset check
  for (i = 0; i < int(sizeof(d)/sizeof(d[0])); i++)
    d[i].check = 0;

  for (unsigned int j = 0; j < mp->Count; j++) {
    mh_sEventId id;
    id.Idx = mp->Sts[j].Idx;
    i = m_mh->find_sup(&id);
    if (i == -1) {
      m_mh->m_errcnt++;
      if (plog)
	m_mh->m_log->vlog('X', "alarmstatus_bc unknown %d", mp->Sts[j].Idx);
    }
    else {
      d[i].check = 1;
      if (!d[i].active) {
	m_mh->m_errcnt++;
	if (plog)
	  m_mh->m_log->vlog('X', "alarmstatus_bc not active %d", mp->Sts[j].Idx);
      }
      if (d[i].status != (int)(mp->Sts[j].Status & 7)) {
	m_mh->m_errcnt++;
	if (plog)
	  m_mh->m_log->vlog('X', "alarmstatus_bc status differ %d %d", mp->Sts[j].Status,
		 d[i].status);	
      }
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (d[i].active && !d[i].check) {
      m_mh->m_errcnt++;
      if (plog)
	m_mh->m_log->vlog('X', "alarmstatus_bc active not present %d", d[i].eventid.Idx);
    }
  }
  m_mh->m_checkstatus = 0;
  return 1;
}

void ra_mhappltest::outunit_connect(void)
{

  mh_UtilWaitForMh();

  m_sts = mh_OutunitConnect(m_user, mh_eOutunitType_Operator, 0, mh_ack_bc,
      mh_alarm_bc, mh_block_bc, mh_cancel_bc, mh_clear_alarmlist_bc,
      mh_clear_blocklist_bc, mh_info_bc, mh_return_bc, mh_alarmstatus_bc);
  if (EVEN(m_sts)) {
    m_log->log('E', "mh_OutunitConnect", m_sts);
    return;
  }
}


void ra_mhappltest::ApplTest(void)
{
  mh_sApplMessage amsg;
  pwr_tOid oid;

  m_sts = gdh_NameToObjid("Test01c-MhAppl-Dv1", &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "ApplTest, gdh_NameToObjid", m_sts);
    return;
  }

  clean();

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    memcpy(&d[i].object, &oid, sizeof(pwr_tOid));
    d[i].eactive = 1;
    if (d[i].eventtype == mh_eEvent_Info || 
	d[i].eventtype == mh_eEvent_InfoSuccess) {
      if ((d[i].eventflags & mh_mEventFlags_Ack) != 0)
	d[i].estatus = mh_mEventStatus_NotAck;
    }
    else {
      if (d[i].eventflags & mh_mEventFlags_Returned)
	d[i].estatus = mh_mEventStatus_NotAck;
      else
	d[i].estatus = mh_mEventStatus_NotRet | mh_mEventStatus_NotAck;
    }

    memset(&amsg, 0, sizeof(amsg));
    memcpy(&amsg.Object, &d[i].object, sizeof(pwr_tOid));
    amsg.EventFlags = (mh_mEventFlags)d[i].eventflags;
    time_GetTime(&amsg.EventTime);
    strcpy(amsg.EventName, d[i].eventname);
    amsg.EventType = d[i].eventtype;
    strcpy(amsg.EventText, d[i].eventtext);
    amsg.EventPrio = d[i].eventprio;    

    m_sts = mh_ApplMessage(&d[i].id, &amsg);
    if (EVEN(m_sts)) {
      m_log->vlog('E', "ApplTest mh_ApplMessage", d[i].eventtext, m_sts);
      return;
    }

  }

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }
  m_checkstatus = 1;
  while (m_checkstatus) {
    receive();
    time_Sleep(0.2);
  }


  // Return
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if ((d[i].eventflags & mh_mEventFlags_Return) != 0 &&      
	(d[i].eventflags & mh_mEventFlags_Returned) == 0) {      
      d[i].estatus &= ~mh_mEventStatus_NotRet;
      if (!d[i].cancel) {
	m_sts = mh_ApplReturn(d[i].id, &d[i].eventtext);
	if (EVEN(m_sts)) {
	  m_log->vlog('E', "ApplTest mh_ApplReturn", d[i].eventtext, m_sts);
	  return;
	}
      }
      else {
	m_sts = mh_ApplCancel(d[i].id, &d[i].eventtext);
	if (EVEN(m_sts)) {
	  m_log->vlog('E', "ApplTest mh_ApplCancel", d[i].eventtext, m_sts);
	  return;
	}
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }
  m_checkstatus = 1;
  while (m_checkstatus) {
    receive();
    time_Sleep(0.2);
  }

  // Ack
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if ((d[i].eventflags & mh_mEventFlags_Ack) != 0) {      
      d[i].estatus &= ~mh_mEventStatus_NotAck;
      mh_OutunitAck(&d[i].eventid);
    }
  }

  for (int i = 0; i < 100; i++) {
    receive();
    time_Sleep(0.2);
  }

  m_checkstatus = 1;
  while (m_checkstatus) {
    receive();
    time_Sleep(0.2);
  }


  // Return and ack remaining
  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  
  // clean();
  if (m_errcnt) {
    m_log->vlog('E', "ApplTest errors detected %d", m_errcnt);
    return;
  }
  m_log->log('S', "ApplTest", PWR__SUCCESS);
}

void ra_mhappltest::ApplGetMsgInfo(void)
{
  mh_sApplMessage amsg, rmsg;
  pwr_tOid oid;

  m_sts = gdh_NameToObjid("Test01c-MhAppl-Dv1", &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "ApplGetMsgInfo, gdh_NameToObjid", m_sts);
    return;
  }

  clean();

  for (unsigned int i = 0; i < 3; i++) {
    memcpy(&d[i].object, &oid, sizeof(pwr_tOid));
    d[i].eactive = 1;
    if (d[i].eventtype == mh_eEvent_Info || 
	d[i].eventtype == mh_eEvent_InfoSuccess) {
      if ((d[i].eventflags & mh_mEventFlags_Ack) != 0)
	d[i].estatus = mh_mEventStatus_NotAck;
    }
    else {
      if (d[i].eventflags & mh_mEventFlags_Returned)
	d[i].estatus = mh_mEventStatus_NotAck;
      else
	d[i].estatus = mh_mEventStatus_NotRet | mh_mEventStatus_NotAck;
    }

    memset(&amsg, 0, sizeof(amsg));
    memcpy(&amsg.Object, &d[i].object, sizeof(pwr_tOid));
    amsg.EventFlags = (mh_mEventFlags)d[i].eventflags;
    time_GetTime(&amsg.EventTime);
    strcpy(amsg.EventName, d[i].eventname);
    amsg.EventType = d[i].eventtype;
    strcpy(amsg.EventText, d[i].eventtext);
    amsg.EventPrio = d[i].eventprio;    

    m_sts = mh_ApplMessage(&d[i].id, &amsg);
    if (EVEN(m_sts)) {
      m_log->vlog('E', "ApplGetMsgInfo mh_ApplMessage", d[i].eventtext, m_sts);
      return;
    }

    m_sts = mh_ApplGetMsgInfo(d[i].id, &rmsg);
    rmsg.Id = amsg.Id;
    rmsg.EventStatus = amsg.EventStatus;
    if (memcmp(&amsg, &rmsg, sizeof(mh_sApplMessage)) != 0) {
      m_log->vlog('E', "ApplGetMsgInfo content differs", d[i].eventtext, m_sts);
      return;
    }
  }

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  // Ack
  for (unsigned int i = 0; i < 3; i++) {
    if ((d[i].eventflags & mh_mEventFlags_Ack) != 0) {      
      d[i].estatus &= ~mh_mEventStatus_NotAck;
      mh_OutunitAck(&d[i].eventid);
    }
  }

  for (int i = 0; i < 100; i++) {
    receive();
    time_Sleep(0.2);
  }

  m_log->log('S', "ApplGetMsgInfo", PWR__SUCCESS);
}

void ra_mhappltest::ApplConnect(void)
{
  pwr_tOid oid;
  pwr_tUInt32 active;

  m_sts = gdh_NameToObjid("Nodes-PwrTest01c-Applications-ra_mhappltest", &oid);
  if (EVEN(m_sts)) {
    m_log->vlog('E', "ApplConnect gdh_NameToObjid", m_sts);
    return;
  }
  
  m_sts = mh_ApplConnect(oid, mh_mApplFlags_NoAbortMsg, "", mh_eEvent_Alarm, 
			 mh_eEventPrio_A, 
			 (mh_mEventFlags)(mh_mEventFlags_Return|mh_mEventFlags_Ack|mh_mEventFlags_Bell), 
			 "", &active);
  if (EVEN(m_sts)) {
    m_log->vlog('E', "ApplConnect", m_sts);
    return;
  }
  m_log->log('S', "ApplConnect", PWR__SUCCESS);
}

void ra_mhappltest::ApplDisconnect(void)
{
  m_sts = mh_ApplDisconnect();
  if (EVEN(m_sts)) {
    m_log->vlog('E', "ApplDisconnect", m_sts);
    return;
  }
  m_log->log('S', "ApplDisconnect", PWR__SUCCESS);
}

// Constructor
ra_mhappltest::ra_mhappltest() : m_clean(0), m_checkstatus(0), m_errcnt(0)
  
{
  m_mh = this;
  m_clean = 1;
  m_log = new tst_log(&m_sts, "rt-MhAppl", "$pwrp_log/mhappl.log");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
  m_sts = gdh_Init("ra_mhappltest");
  if (EVEN(m_sts)) {
    m_log->log('F', "init gdh_Init", m_sts);  
    exit(0);
  }
  m_sts = gdh_NameToObjid("Nodes-PwrTest01c-OpPlaces-ra_mhappltest", &m_user);
  if (EVEN(m_sts)) {
    m_log->log('F', "init gdh_NameToObjid", m_sts);  
    exit(0);
  }

  m_sts = errh_Init("ra_mhappltest", errh_eAnix_appl5);
  if (m_sts != 1) {
    m_log->log('E', "init errh_Init", m_sts);
    return;
  }

  // errh_SetStatus(PWR__APPLSTARTUP);

  outunit_connect();
}

void ra_mhappltest::clean()
{
  m_clean = 1;
  for (int i = 0; i < 50; i++) {
    time_Sleep(0.2);
    receive();
  }
  m_clean = 0;
}

int ra_mhappltest::find_sup(mh_sEventId *id)
{
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (d[i].eventid.Idx == id->Idx)
      return i;
  }
  return -1;
}

void ra_mhappltest::receive()
{
    m_sts = mh_OutunitReceive();
    while (ODD(m_sts))
      m_sts = mh_OutunitReceive();
}

// Destructor
ra_mhappltest::~ra_mhappltest()
{
  delete m_log;
}

int main()
{
  ra_mhappltest mh;

  mh.ApplConnect();
  mh.ApplTest();
  mh.ApplGetMsgInfo();
  mh.ApplDisconnect();
}


