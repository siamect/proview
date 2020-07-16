/*
  Test nettime functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "co_msg.h"
#include "co_dcli.h"
#include "rt_net.h"
#include "ra_nettimetest.h"
#include "co_time_msg.h"
#include "co_cdh_msg.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"


typedef struct {
  net_sTime nettime;
  pwr_tTime result;
} sNetTimeToTime;

void ra_nettimetest::NetTimeToTime(void)
{
  sNetTimeToTime d[] = {
    {{2682419400,500000000}, {2682419400,500000000}},
    {{2145913199,500000000}, {2145913199,500000000}},
    {{2145913200,000000000}, {2145913200,000000000}}
  };
  
  pwr_tTime result;
  net_sTime nettime;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = net_NetTimeToTime(&d[i].nettime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->vlog('E', "NetTimeToTime, result differs idx %d", i);
      return;
    }
    nettime = net_TimeToNetTime(&result);
    if (memcmp(&nettime, &d[i].nettime, sizeof(net_sTime)) != 0) {
      m_log->vlog('E', "NetTimeToTime, result differs idx %d", i);
      return;
    }
  }

  m_log->log('S', "NetTimeToTime", TIME__SUCCESS);
}

typedef struct {
  net_sTime nettime;
  pwr_tDeltaTime result;
} sNetTimeToDeltaTime;

void ra_nettimetest::NetTimeToDeltaTime(void)
{
  sNetTimeToDeltaTime d[] = {
    {{0,1}, {0,1}},
    {{100,1}, {100,1}},
    {{0,1}, {0,1}},
    {{100,0}, {100,0}},
    {{0,500000000}, {0,500000000}},
    {{22222,500000000}, {22222,500000000}}
  };
  
  pwr_tDeltaTime result;
  net_sTime nettime;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = net_NetTimeToDeltaTime(&d[i].nettime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->vlog('E', "NetTimeToDeltaTime, result differs idx %d", i);
      return;
    }
    nettime = net_DeltaTimeToNetTime(&result);
    if (memcmp(&nettime, &d[i].nettime, sizeof(net_sTime)) != 0) {
      m_log->vlog('E', "NetTimeToDeltaTime, result differs idx %d", i);
      return;
    }
  }

  m_log->log('S', "NetTimeToDeltaTime", TIME__SUCCESS);
}

// Constructor
ra_nettimetest::ra_nettimetest()
{
  m_log = new tst_log(&m_sts, "rt-NetTime", "$pwrp_log/nettime.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
ra_nettimetest::~ra_nettimetest()
{
  delete m_log;
}

int main()
{
  ra_nettimetest time;
  pwr_tStatus sts;

  sts = gdh_Init("ra_nettimetest");
  if (EVEN(sts)) {
    time.log()->log('S', "NetTime gdh_Init", sts);  
    exit(0);
  }

  time.NetTimeToTime();
  time.NetTimeToDeltaTime();
}


