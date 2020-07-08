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
#include "ra_mhtest.h"
#include "rt_pwr_msg.h"
#include "rt_plc_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static int plog = 1;
ra_mhtest *ra_mhtest::m_mh = 0;

pwr_tStatus ra_mhtest::mh_ack_bc(mh_sAck* mp)
{
  if (m_mh->m_clean)
    return 1;

  if (plog)
    m_mh->m_log->vlog('D', "ack_bc %d", mp->TargetId.Idx);
  sSup *sup = m_mh->find_sup(&mp->TargetId);
  if (!sup) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "ack_cb, unknown eventid");
    return 1;
  }
  sup->status &= ~mh_mEventStatus_NotAck;
  if (sup->status != sup->estatus && sup->prio < SUP_ASIZE) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "ack_bc status differs %d %d", sup->status, sup->estatus);
  }
  if ((sup->estatus & mh_mEventStatus_NotRet) == 0 && 
      (sup->estatus & mh_mEventStatus_NotAck) == 0) {
    sup->active = 0;
    sup->eactive = 0;
    sup->estatus = 0;
    sup->status = 0;
  }
  return 1;
}

pwr_tStatus ra_mhtest::mh_return_bc(mh_sReturn* mp)
{
  unsigned int idx, num, prio;

  if (m_mh->m_clean) {
    mh_OutunitAck(&mp->TargetId);
    return 1;
  }
  
  if (plog)
    m_mh->m_log->vlog('D', "return_bc %d", mp->TargetId.Idx);
  switch (mp->EventText[0]) {
  case 'A':
    prio = 0;
    break;
  case 'B':
    prio = 1;
    break;
  case 'C':
    prio = 2;
    break;
  case 'D':
    prio = 3;
    break;
  case 'O':
    prio = 4;
    break;
  case 'I':
    prio = 5;
    break;
  default:
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong eventtext, %s", mp->EventText);
    return 1;
  }

  num = sscanf(&mp->EventText[1], "%u", &idx);
  if (num != 1 || idx < 1 || idx > 32) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong index, %s", mp->EventText);
    return 1;
  }
  idx--;
  sSup *sup = &m_mh->m_sup[prio][idx];
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, sup->oid)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong object, %s", mp->EventText);
    return 1;
  }
  if (mp->TargetId.Idx != sup->eventid.Idx) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc wrong eventid, %s", mp->EventText);
    return 1;
  }
  sup->status &= ~mh_mEventStatus_NotRet;
  if (sup->status != sup->estatus && prio < SUP_ASIZE) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "return_bc status differs %d %d", sup->status, sup->estatus);
  }
  if ((sup->estatus & mh_mEventStatus_NotRet) == 0 && 
      (sup->estatus & mh_mEventStatus_NotAck) == 0) {
    sup->eactive = 0;
    sup->active = 0;
    sup->estatus = 0;
    sup->status = 0;
  }

  return 1;
}

pwr_tStatus ra_mhtest::mh_alarm_bc(mh_sMessage* mp)
{
  unsigned int idx, num, prio;

  if (m_mh->m_clean) {
    if (mp->Status & mh_mEventStatus_NotAck)
      mh_OutunitAck(&mp->Info.Id);
    return 1;
  }

  if (plog)
    m_mh->m_log->vlog('D', "alarm_bc %d", mp->Info.Id.Idx);
  switch (mp->EventText[0]) {
  case 'A':
    prio = 0;
    break;
  case 'B':
    prio = 1;
    break;
  case 'C':
    prio = 2;
    break;
  case 'D':
    prio = 3;
    break;
  case 'O':
    prio = 4;
    break;
  case 'I':
    prio = 5;
    break;
  default:
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong eventtext, %s", mp->EventText);
    return 1;
  }

  num = sscanf(&mp->EventText[1], "%u", &idx);
  if (num != 1 || idx < 1 || idx > 32) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong index, %s", mp->EventText);
    return 1;
  }
  idx--;
  sSup *sup = &m_mh->m_sup[prio][idx];
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, sup->oid)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc wrong object, %s", mp->EventText);
    return 1;
  }
  sup->active = 1;
  if (!sup->eactive) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc sup not active, %d %s", sup->eventid.Idx, 
	     mp->EventText);
    return 1;
  }
  sup->status = mp->Status & 0x7;
  if (sup->status != sup->estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "alarm_bc status differs %d %d %d", sup->status, sup->estatus,
	     sup->eventid.Idx);
    return 1;
  }
  memcpy(&sup->eventid, &mp->Info.Id, sizeof(mh_sEventId));
  return 1;
}

pwr_tStatus ra_mhtest::mh_block_bc(mh_sBlock* mp)
{
  return 1;
}

pwr_tStatus ra_mhtest::mh_cancel_bc(mh_sReturn* mp)
{
  return 1;
}

pwr_tStatus ra_mhtest::mh_info_bc(mh_sMessage* mp)
{
  unsigned int idx, num, prio;

  if (m_mh->m_clean) {
    if (mp->Status & mh_mEventStatus_NotAck)
      mh_OutunitAck(&mp->Info.Id);
    return 1;
  }

  if (plog)
    m_mh->m_log->vlog('D', "info_bc %d", mp->Info.Id.Idx);
  switch (mp->EventText[0]) {
  case 'I':
    prio = 5;
    break;
  default:
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc wrong eventtext, %s", mp->EventText);
    return 1;
  }

  num = sscanf(&mp->EventText[1], "%u", &idx);
  if (num != 1 || idx < 1 || idx > 32) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc wrong index, %s", mp->EventText);
    return 1;
  }
  idx--;
  sSup *sup = &m_mh->m_sup[prio][idx];
  if (cdh_ObjidIsNotEqual(mp->Object.Objid, sup->oid)) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc wrong object, %s", mp->EventText);
    return 1;
  }
  sup->active = 1;
  if (!sup->eactive) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc sup not active, %d %s", sup->eventid.Idx, 
	     mp->EventText);
    return 1;
  }
  sup->status = mp->Status & 0x7;
  if (sup->status != sup->estatus) {
    m_mh->m_errcnt++;
    if (plog)
      m_mh->m_log->vlog('X', "info_bc status differs %d %d %d", sup->status, sup->estatus,
	     sup->eventid.Idx);
    return 1;
  }
  memcpy(&sup->eventid, &mp->Info.Id, sizeof(mh_sEventId));
  return 1;
}

pwr_tStatus ra_mhtest::mh_clear_alarmlist_bc(pwr_tNodeIndex nix)
{
  return 1;
}

pwr_tStatus ra_mhtest::mh_clear_blocklist_bc(pwr_tNodeIndex nix)
{
  return 1;
}

pwr_tStatus ra_mhtest::mh_alarmstatus_bc(mh_sAlarmStatus* mp)
{
  sSup *sup;

  if (!m_mh->m_checkstatus)
    return 1;

  // Reset check
  for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
    for (int j = 0; j < 32; j++)
      m_mh->m_sup[i][j].check = 0;
  }

  for (unsigned int i = 0; i < mp->Count; i++) {
    mh_sEventId id;
    id.Idx = mp->Sts[i].Idx;
    sup = m_mh->find_sup(&id);
    if (!sup) {
      m_mh->m_errcnt++;
      if (plog)
	m_mh->m_log->vlog('X', "alarmstatus_bc unknown %d", mp->Sts[i].Idx);
    }
    else {
      sup->check = 1;
      if (!sup->active) {
	m_mh->m_errcnt++;
	if (plog)
	  m_mh->m_log->vlog('X', "alarmstatus_bc not active %d", mp->Sts[i].Idx);
      }
      if (sup->status != (mp->Sts[i].Status & 7)) {
	m_mh->m_errcnt++;
	if (plog)
	  m_mh->m_log->vlog('X', "alarmstatus_bc status differ %d %d", mp->Sts[i].Status,
		 sup->status);	
      }
    }
  }
  for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
    for (int j = 0; j < 32; j++) {
      sup = &m_mh->m_sup[i][j];
      if (sup->active && !sup->check) {
	m_mh->m_errcnt++;
	if (plog)
	  m_mh->m_log->vlog('X', "alarmstatus_bc active not present %d", sup->eventid.Idx);
      }
    }
  }
  m_mh->m_checkstatus = 0;
  return 1;
}

void ra_mhtest::OutunitConnect(void)
{

  mh_UtilWaitForMh();

  m_sts = mh_OutunitConnect(m_user, mh_eOutunitType_Operator, 0, mh_ack_bc,
      mh_alarm_bc, mh_block_bc, mh_cancel_bc, mh_clear_alarmlist_bc,
      mh_clear_blocklist_bc, mh_info_bc, mh_return_bc, mh_alarmstatus_bc);
  if (EVEN(m_sts)) {
    m_log->log('E', "OutunitConnect", m_sts);
    return;
  }

  m_log->log('S', "OutunitConnect", PWR__SUCCESS);
}


void ra_mhtest::OutunitTest(void)
{
  unsigned int mask;
  sSup *sup;

#if 0
  sup = &m_sup[0][0];
  if (!sup->active) {
    *sup->dp = 1;
    sup->eactive = 1;
    sup->estatus = mh_mEventStatus_NotRet | mh_mEventStatus_NotAck;
  }

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  sup = &m_sup[0][0];
  if (sup->active && sup->status & mh_mEventStatus_NotRet) {
    *sup->dp = 0;
    sup->estatus &= ~mh_mEventStatus_NotRet;
  }

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  sup = &m_sup[0][0];
  if (sup->active && sup->status & mh_mEventStatus_NotAck) {
    mh_OutunitAck(&sup->eventid);
    sup->estatus &= ~mh_mEventStatus_NotAck;
  }
#endif

  for (int k = 0; k < SUP_CYCLES; k++) {
    // Activate
    for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
      mask = rand();
      for (int j = 0; j < 32; j++) {
	sup = &m_sup[i][j];
	sup->prio = i;
	if ( mask & (1 << j)) {
	  if (!sup->eactive) {
	    *sup->dp = 1;
	    sup->eactive = 1;
	    if (i < SUP_ASIZE)
	      sup->estatus = mh_mEventStatus_NotRet | mh_mEventStatus_NotAck;
	    else
	      sup->estatus = mh_mEventStatus_NotAck;
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

    // Return
    for (int i = 0; i < SUP_ASIZE; i++) {
      mask = rand();
      for (int j = 0; j < 32; j++) {
	if ( mask & (1 << j)) {
	  sup = &m_sup[i][j];
	  if (sup->active && sup->status & mh_mEventStatus_NotRet) {
	    *sup->dp = 0;
	    sup->estatus &= ~mh_mEventStatus_NotRet;
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
    for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
      mask = rand();
      for (int j = 0; j < 32; j++) {
	if ( mask & (1 << j)) {
	  sup = &m_sup[i][j];
	  if (sup->active && sup->status & mh_mEventStatus_NotAck) {
	    mh_OutunitAck(&sup->eventid);
	    sup->estatus &= ~mh_mEventStatus_NotAck;
	    if (i >= SUP_ASIZE)
	      *sup->dp = 0;
	  }
	}
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
  }


  // Return and ack remaining
  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  // Return
  for (int i = 0; i < SUP_ASIZE; i++) {
    for (int j = 0; j < 32; j++) {
      sup = &m_sup[i][j];
      if (sup->eactive && sup->status & mh_mEventStatus_NotRet) {
	*sup->dp = 0;
	sup->estatus &= ~mh_mEventStatus_NotRet;
      }
    }
  } 

  for (int i = 0; i < 10; i++) {
    receive();
    time_Sleep(0.2);
  }

  // Ack
  for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
    for (int j = 0; j < 32; j++) {
      sup = &m_sup[i][j];
      if (sup->eactive && sup->status & mh_mEventStatus_NotAck) {
	mh_OutunitAck(&sup->eventid);
	sup->estatus &= ~mh_mEventStatus_NotAck;
	if (i >= SUP_ASIZE)
	  *sup->dp = 0;
      }
    }
  }

  for (int i = 0; i < 60; i++) {
    receive();
    time_Sleep(0.2);
  }

  m_checkstatus = 1;
  while (m_checkstatus) {
    receive();
    time_Sleep(0.2);
  }
  
  // clean();
  if (m_errcnt) {
    m_log->vlog('E', "OutunitTest errors detected %d", m_errcnt);
    return;
  }
  m_log->log('S', "OutunitTest", PWR__SUCCESS);
}

// Constructor
ra_mhtest::ra_mhtest() : m_clean(0), m_checkstatus(0), m_errcnt(0)
  
{
  m_mh = this;
  m_clean = 1;
  m_log = new tst_log(&m_sts, "rt-Mh", "$pwrp_log/mh.log");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
  m_sts = gdh_Init("ra_mhtest");
  if (EVEN(m_sts)) {
    m_log->log('F', "mh gdh_Init", m_sts);  
    exit(0);
  }
  m_sts = gdh_NameToObjid("Nodes-PwrTest01c-OpPlaces-ra_mhtest", &m_user);
  if (EVEN(m_sts)) {
    m_log->log('F', "mh gdh_NameToObjid", m_sts);  
    exit(0);
  }

  m_sts = errh_Init("ra_mhtest", errh_eAnix_appl4);
  if (m_sts != 1) {
    m_log->log('E', "mh errh_Init", m_sts);
    return;
  }
  // errh_SetStatus(PWR__APPLSTARTUP);

  // Connect do objects
  memset(&m_sup, 0, sizeof(m_sup));

  pwr_tOName name;
  pwr_tAName aname;
  pwr_tAttrRef aref;
  pwr_tDlid dlid;

  for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
    switch(i) {
    case 0:
      strcpy(name, "Test01c-MhOutunit-A");
      break;
    case 1:
      strcpy(name, "Test01c-MhOutunit-B");
      break;
    case 2:
      strcpy(name, "Test01c-MhOutunit-C");
      break;
    case 3:
      strcpy(name, "Test01c-MhOutunit-D");
      break;
    case 4:
      strcpy(name, "Test01c-MhOutunit-O");
      break;
    case 5:
      strcpy(name, "Test01c-MhOutunit-I");
      break;
    }      

    for (int j = 0; j < 32; j++) {
      sprintf(aname, "%s%d", name, j+1);
      m_sts = gdh_NameToObjid(aname, &m_sup[i][j].oid);
      if (ODD(m_sts)) {
	aref = cdh_ObjidToAref(m_sup[i][j].oid);
	m_sts = gdh_ArefANameToAref(&aref, "ActualValue", &aref);
      }
      if (ODD(m_sts))
	m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&m_sup[i][j].dp,
					   &dlid);
      if (EVEN(m_sts)) {
	m_log->log('E', "mh dsup connect", name, m_sts);
	exit(0);	
      }
      *m_sup[i][j].dp = 0;
    }
  }
  OutunitConnect();
  clean();
}

void ra_mhtest::clean()
{
  m_clean = 1;
  for (int i = 0; i < SUP_ASIZE+SUP_ISIZE; i++) {
    for (int j = 0; j < 32; j++)
      *m_sup[i][j].dp = 0;
  }
  for (int i = 0; i < 50; i++) {
    time_Sleep(0.2);
    receive();
  }
  m_clean = 0;
}

sSup *ra_mhtest::find_sup(mh_sEventId *id)
{
  for (int j = 0; j < SUP_ASIZE+SUP_ISIZE; j++) {
    for (int i = 0; i < 32; i++) {
      if (m_sup[j][i].eventid.Idx == id->Idx)
	return &m_sup[j][i];
    }
  }
  return 0;
}

void ra_mhtest::receive()
{
    m_sts = mh_OutunitReceive();
    while (ODD(m_sts))
      m_sts = mh_OutunitReceive();
}

unsigned int ra_mhtest::rand()
{
  unsigned int r = random();
  r |= (random() & 1) << 31;
  return r;
}

// Destructor
ra_mhtest::~ra_mhtest()
{
  delete m_log;
}

int main()
{
  ra_mhtest mh;

  // mh.OutunitConnect();
  mh.OutunitTest();
}


