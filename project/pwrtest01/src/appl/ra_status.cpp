/*
  Print current project status
*/

#include <stdio.h>
#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "ra_status.h"

void ra_status::test_status()
{
  pwr_tStatus sts, system_sts;
  pwr_tOid noid;
  pwr_tOName name;

  m_sts = gdh_GetNodeObject(0, &noid);  
  if (EVEN(m_sts)) {
    m_log->log('E', "gdh_GetNodeObject", m_sts);
    return;
  }
  m_sts = gdh_ObjidToName(noid, name, sizeof(name), cdh_mName_volumeStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "gdh_ObjidToName", m_sts);
    return;
  }
  strcat(name, ".SystemStatus");
  m_sts = gdh_GetObjectInfo(name, &system_sts, sizeof(system_sts));
  if (EVEN(m_sts)) {
    m_log->log('E', "gdh_GetObjectInfo", m_sts);
    return;
  }

  switch (errh_Severity(system_sts)) {
  case errh_eSeverity_Success:
    m_log->log('S', "Success system status", system_sts);
    break;
  case errh_eSeverity_Info:
    m_log->log('I', "Info system status", system_sts);
    break;
  case errh_eSeverity_Warning:
    m_log->log('W', "Warning system status", system_sts);
    break;
  case errh_eSeverity_Error:
    m_log->log('E', "Error system status", system_sts);
    break;
  case errh_eSeverity_Fatal:
    m_log->log('F', "Fatal system status", system_sts);
    break;
  }
}



// Constructor
ra_status::ra_status()
{
  m_log = new tst_log(&m_sts, "rt-Status", "$pwrp_log/tst_status.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");

  m_sts = gdh_Init("ra_status");
  if (EVEN(m_sts)) {
    m_log->log('F', "gdh_Init", m_sts);
    exit(0);
  }  
}

// Destructor
ra_status::~ra_status()
{
  delete m_log;
}

int main()
{
  ra_status s;

  s.test_status();
}
