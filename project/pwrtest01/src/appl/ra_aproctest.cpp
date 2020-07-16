/*
  Test aproc functions.
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
#include "ra_aproctest.h"
#include "rt_pwr_msg.h"
#include "rt_plc_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static float aproctest_wait=0.01;

void ra_aproctest::RegisterObject(void)
{
  pwr_tOid oid;

  m_sts = gdh_NameToObjid("Nodes-PwrTest01c-Applications-ra_aproctest", &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "RegisterObject gdh_NameToObjid", m_sts);
    return;
  }

  m_sts = aproc_RegisterObject(oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "RegisterObject", m_sts);
    return;
  }

  time_Sleep(aproctest_wait);
  if (cdh_ObjidIsNotEqual(m_nodep->ProcObject[m_anix-1], oid)) {
    m_log->log('E', "RegisterObject, objid doesn't match");
    return;
  }

  m_log->log('S', "RegisterObject", PWR__SUCCESS);
}

void ra_aproctest::TimeStamp(void)
{
  errh_SetStatus(PWR__ARUN);

  for (int i = 0; i < 10; i++) {
    aproc_TimeStamp(0.005, 0.010);

    time_Sleep(.005);
    if (m_nodep->ProcStatus[m_anix-1] != PWR__ARUN) {
      m_log->log('E', "RegisterObject, process status doesn't match");
      return;
    }
  }
  time_Sleep(1.0);
  if (m_nodep->ProcStatus[m_anix-1] != PWR__PTIMEOUT) {
    m_log->log('E', "RegisterObject, no timeout occurred");
    return;
  }

  errh_SetStatus(0);

  m_log->log('S', "RegisterObject", PWR__SUCCESS);
}


// Constructor
ra_aproctest::ra_aproctest()
{
  pwr_tOid oid;

  m_anix = errh_eAnix_appl3;
  m_log = new tst_log(&m_sts, "rt-Aproc", "$pwrp_log/aproc.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
  m_sts = gdh_Init("ra_aproctest");
  if (EVEN(m_sts)) {
    m_log->log('F', "aproc gdh_Init", m_sts);  
    exit(0);
  }
  m_sts = gdh_GetNodeObject(pwr_cNNid, &oid);
  if (EVEN(m_sts)) {
    m_log->log('F', "aproc gdh_GetNodeObject", m_sts);  
    exit(0);
  }
  m_sts = gdh_ObjidToPointer(oid, (void **)&m_nodep);
  if (EVEN(m_sts)) {
    m_log->log('F', "aproc gdh_ObjidToPointer", m_sts);  
    exit(0);
  }

  m_sts = errh_Init("ra_aproctest", m_anix);
  if (m_sts != 1) {
    m_log->log('E', "aproc errh_Init", m_sts);
    return;
  }
  errh_SetStatus(PWR__APPLSTARTUP);
}

// Destructor
ra_aproctest::~ra_aproctest()
{
  delete m_log;
}

int main()
{
  ra_aproctest aproc;

  aproc.RegisterObject();
  aproc.TimeStamp();
}


