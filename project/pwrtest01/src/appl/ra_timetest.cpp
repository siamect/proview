/*
  Test gdh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "ra_timetest.h"
#include "co_time_msg.h"
#include "co_cdh_msg.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"


typedef struct {
  pwr_tTime time;
  int sts;
} sIsNull;

void ra_timetest::IsNull(void)
{
  sIsNull d[] = {
    {pwr_cNTime, 1},
    {{0,1}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = time_IsNull(&d[i].time);
    if (sts != d[i].sts) {
      m_log->log('E', "IsNull");
      return;
    }
  }

  m_log->log('S', "IsNull", TIME__SUCCESS);
}

typedef struct {
  pwr_tTime time;
  pwr_tDeltaTime dtime;
  pwr_tTime result;
} sAadd;

void ra_timetest::Aadd(void)
{
  sAadd d[] = {
    {pwr_cNTime, {0,1}, {0,1}},
    {{2682419400,500000000}, {0,499999999}, {2682419400,999999999}},
    {{2145913199,500000000}, {0,500000000}, {2145913200,0}},
    {{2145913200,000000000}, {0,500000000}, {2145913200,500000000}}
  };
  
  pwr_tTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Aadd(&result, &d[i].time, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Aadd", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tTime));
    time_Aadd(0, &result, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Aadd", i);
      return;
    }
  }

  m_log->log('S', "Aadd", TIME__SUCCESS);
}

void ra_timetest::Aadd_NE(void)
{
  sAadd d[] = {
    {pwr_cNTime, {0,1}, {0,1}},
    {{2682419400,500000000}, {0,499999999}, {2682419400,999999999}},
    {{2145913199,500000000}, {0,500000000}, {2145913200,0}},
    {{2145913200,000000000}, {0,500000000}, {2145913200,500000000}},
    {pwr_cNotATime, {0,500000000}, pwr_cNotATime},
    {{1587468600,0}, pwr_cNotADeltaTime, pwr_cNotATime},
    {{1587468600,0}, {-1,-500000000}, {1587468598,500000000}},
    {{1587468600,0}, {1,-500000000}, pwr_cNotATime},
    {{1587468600,0}, {-1,500000000}, pwr_cNotATime},
    {{1587468600,-500000000}, {1,500000000}, pwr_cNotATime},
    {{-1587468600,500000000}, {1,500000000}, {-1587468598,0}} // Invalid time?
  };
  
  pwr_tTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Aadd_NE(&result, &d[i].time, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Aadd_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tTime));
    time_Aadd_NE(0, &result, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Aadd", i);
      return;
    }
  }

  m_log->log('S', "Aadd_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tTime time1;
  pwr_tTime time2;
  int result;
} sAcomp;

void ra_timetest::Acomp(void)
{
  sAcomp d[] = {
    {pwr_cNTime, pwr_cNTime, 0},
    {{1587468600,500000000}, {1587468600,500000000}, 0},
    {{1587468600,500000000}, {1587468600,500000001}, -1},
    {{1587468600,500000000}, {1587468600,499999999}, 1}
  };
  
  int result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = time_Acomp(&d[i].time1, &d[i].time2);
    if (result != d[i].result) {
      m_log->log('E', "Acomp", i);
      return;
    }
  }

  m_log->log('S', "Acomp", TIME__SUCCESS);
}

void ra_timetest::Acomp_NE(void)
{
  sAcomp d[] = {
    {pwr_cNTime, pwr_cNTime, 0},
    {{1587468600,500000000}, {1587468600,500000000}, 0},
    {{1587468600,500000000}, {1587468600,500000001}, -1},
    {{1587468600,500000000}, {1587468600,499999999}, 1},
    {{1587468600,500000000}, {1587468600,-499999999}, -2},
    {{1587468600,-500000000}, {1587468600,499999999}, -2},
    {{1587468600,500000000}, pwr_cNotATime, -2},
    {pwr_cNotATime, {1587468600,500000000}, -2}
  };
  
  int result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = time_Acomp_NE(&d[i].time1, &d[i].time2);
    if (result != d[i].result) {
      m_log->log('E', "Acomp_NE", i);
      return;
    }
  }

  m_log->log('S', "Acomp_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tTime time1;
  pwr_tTime time2;
  pwr_tDeltaTime result;
} sAdiff;

void ra_timetest::Adiff(void)
{
  sAdiff d[] = {
    {pwr_cNTime, pwr_cNTime, 0},
    {{1587468600,500000000}, {1587468600,500000000}, pwr_cNDeltaTime},
    {{1587468600,500000000}, {1587468600,500000001}, {0,-1}},
    {{1587468500,500000000}, {1587468600,500000001}, {-100,-1}},
    {{1587468600,500000000}, {1587468600,499999999}, {0,1}},
    {{1587468600,500000000}, {1587468500,499999999}, {100,1}},
    {{2145913200,0}, {2145913199,500000000}, {0,500000000}},
    {{2245913200,000000000}, {2245913199,500000000}, {0,500000000}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Adiff(&result, &d[i].time1, &d[i].time2);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Adiff", i);
      return;
    }
  }

  m_log->log('S', "Adiff", TIME__SUCCESS);
}

void ra_timetest::Adiff_NE(void)
{
  sAdiff d[] = {
    {pwr_cNTime, pwr_cNTime, 0},
    {{1587468600,500000000}, {1587468600,500000000}, pwr_cNDeltaTime},
    {{1587468600,500000000}, {1587468600,500000001}, {0,-1}},
    {{1587468500,500000000}, {1587468600,500000001}, {-100,-1}},
    {{1587468600,500000000}, {1587468600,499999999}, {0,1}},
    {{1587468600,500000000}, {1587468500,499999999}, {100,1}},
    {{1587468600,500000000}, {1587468600,-499999999}, pwr_cNotADeltaTime},
    {{1587468600,-500000000}, {1587468600,499999999}, pwr_cNotADeltaTime},
    {{1587468600,500000000}, pwr_cNotATime, pwr_cNotADeltaTime},
    {pwr_cNotATime, {1587468600,500000000}, pwr_cNotADeltaTime},
    {{2145913200,0}, {2145913199,500000000}, {0,500000000}},
    {{2245913200,000000000}, {2245913199,500000000}, {0,500000000}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Adiff_NE(&result, &d[i].time1, &d[i].time2);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Adiff_NE", i);
      return;
    }
  }

  m_log->log('S', "Adiff_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tTime time;
  pwr_tDeltaTime dtime;
  pwr_tTime result;
} sAsub;

void ra_timetest::Asub(void)
{
  sAsub d[] = {
    {pwr_cNTime, pwr_cNDeltaTime, pwr_cNTime},
    {{1587468600,500000000}, pwr_cNDeltaTime, {1587468600,500000000}},
    {{1587468600,500000001}, {0,1}, {1587468600,500000000}},
    {{1587468600,500000001}, {100,1}, {1587468500,500000000}},
    {{1587468600,499999999}, {0,-1}, {1587468600,500000000}},
    {{1587468500,499999999}, {-100,-1}, {1587468600,500000000}},
    {{2145913199,500000000}, {0,-500000000}, {2145913200,0}},
    {{2245913199,500000000}, {0,-500000000}, {2245913200,000000000}}
  };
  
  pwr_tTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Asub(&result, &d[i].time, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Asub", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tTime));
    time_Asub(0, &result, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Asub", i);
      return;
    }
  }

  m_log->log('S', "Asub", TIME__SUCCESS);
}

void ra_timetest::Asub_NE(void)
{
  sAsub d[] = {
    {pwr_cNTime, pwr_cNDeltaTime, pwr_cNTime},
    {{1587468600,500000000}, pwr_cNDeltaTime, {1587468600,500000000}},
    {{1587468600,500000001}, {0,1}, {1587468600,500000000}},
    {{1587468600,500000001}, {100,1}, {1587468500,500000000}},
    {{1587468600,499999999}, {0,-1}, {1587468600,500000000}},
    {{1587468500,499999999}, {-100,-1}, {1587468600,500000000}},
    {{1587468600,-499999999}, {10,10}, pwr_cNotATime},
    {{1587468600,499999999}, {10,-10}, pwr_cNotATime},
    {{1587468600,499999999}, pwr_cNotADeltaTime, pwr_cNotATime},
    {pwr_cNotATime, {10,10}, pwr_cNotATime},
    {{2145913199,500000000}, {0,-500000000}, {2145913200,0}},
    {{2245913199,500000000}, {0,-500000000}, {2245913200,0}}
  };
  
  pwr_tTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Asub_NE(&result, &d[i].time, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Asub_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tTime));
    time_Asub_NE(0, &result, &d[i].dtime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "Asub", i);
      return;
    }
  }

  m_log->log('S', "Asub_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tDeltaTime result;
} sDabs;

void ra_timetest::Dabs(void)
{
  sDabs d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,1}, {0,1}},
    {{100,1}, {100,1}},
    {{0,-1}, {0,1}},
    {{-100,-1}, {100,1}},
    {{-100,0}, {100,0}},
    {{0,-500000000}, {0,500000000}},
    {{-22222,-500000000}, {22222,500000000}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dabs(&result, &d[i].time);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dabs", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dabs(0, &result);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dabs", i);
      return;
    }
  }

  m_log->log('S', "Dabs", TIME__SUCCESS);
}

void ra_timetest::Dabs_NE(void)
{
  sDabs d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,1}, {0,1}},
    {{100,1}, {100,1}},
    {{0,-1}, {0,1}},
    {{-100,-1}, {100,1}},
    {{-100,0}, {100,0}},
    {{0,-500000000}, {0,500000000}},
    {{-22222,-500000000}, {22222,500000000}},
    {{10,-10}, pwr_cNotADeltaTime},
    {{-10,10}, pwr_cNotADeltaTime},
    {pwr_cNotADeltaTime, pwr_cNotADeltaTime}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dabs_NE(&result, &d[i].time);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dabs_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dabs_NE(0, &result);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dabs_NE", i);
      return;
    }
  }

  m_log->log('S', "Dabs_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tDeltaTime atime;
  pwr_tDeltaTime result;
} sDadd;

void ra_timetest::Dadd(void)
{
  sDadd d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,1}, {0,1}, {0,2}},
    {{100,1}, {100,1}, {200,2}},
    {{0,-1}, {0,10}, {0,9}},
    {{-100,-10}, {100,1}, {0,-9}},
    {{-100,0}, {101,0}, {1,0}},
    {{0,500000000}, {0,500000000}, {1,0}},
    {{-22222,-500000000}, {22222,500000001}, {0,1}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dadd(&result, &d[i].time, &d[i].atime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dadd", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dadd(0, &result, &d[i].atime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dadd", i);
      return;
    }
  }

  m_log->log('S', "Dadd", TIME__SUCCESS);
}

void ra_timetest::Dadd_NE(void)
{
  sDadd d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, {0,0}},
    {{0,1}, {0,1}, {0,2}},
    {{102,1}, {100,0}, {202,1}},
    {{-2,-1}, {0,2}, {-1,-999999999}},
    {{-100,-1}, {100,1}, {0,0}},
    {{-100,-1}, {100,0}, {0,-1}},
    {{0,500000001}, {0,500000000}, {1,1}},
    {{-22222,-500000000}, {22222,500000001}, {0,1}},
    {{10,-10}, {1,1}, pwr_cNotADeltaTime},
    {{1,1}, {-10,10}, pwr_cNotADeltaTime},
    {pwr_cNotADeltaTime, {1,1}, pwr_cNotADeltaTime},
    {{1,1}, pwr_cNotADeltaTime, pwr_cNotADeltaTime}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dadd_NE(&result, &d[i].time, &d[i].atime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dadd_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dadd_NE(0, &result, &d[i].atime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dadd_NE", i);
      return;
    }
  }

  m_log->log('S', "Dadd_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tDeltaTime result;
} sDneg;

void ra_timetest::Dneg(void)
{
  sDneg d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,1}, {0,-1}},
    {{100,1}, {-100,-1}},
    {{0,-1}, {0,1}},
    {{-100,-1}, {100,1}},
    {{-100,0}, {100,0}},
    {{0,-500000000}, {0,500000000}},
    {{-22222,-500000000}, {22222,500000000}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dneg(&result, &d[i].time);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dneg", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dneg(0, &result);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dneg", i);
      return;
    }
  }

  m_log->log('S', "Dneg", TIME__SUCCESS);
}

void ra_timetest::Dneg_NE(void)
{
  sDneg d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,1}, {0,-1}},
    {{100,1}, {-100,-1}},
    {{0,-1}, {0,1}},
    {{-100,-1}, {100,1}},
    {{-100,0}, {100,0}},
    {{0,-500000000}, {0,500000000}},
    {{-22222,-500000000}, {22222,500000000}},
    {{10,-10}, pwr_cNotADeltaTime},
    {{-10,10}, pwr_cNotADeltaTime},
    {pwr_cNotADeltaTime, pwr_cNotADeltaTime}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dneg_NE(&result, &d[i].time);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dneg_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dneg_NE(0, &result);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dneg_NE", i);
      return;
    }
  }

  m_log->log('S', "Dneg_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tDeltaTime stime;
  pwr_tDeltaTime result;
} sDsub;

void ra_timetest::Dsub(void)
{
  sDsub d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,3}, {0,1}, {0,2}},
    {{300,3}, {100,1}, {200,2}},
    {{0,-1}, {0,10}, {0,-11}},
    {{-100,-10}, {100,1}, {-200,-11}},
    {{100,0}, {101,0}, {-1,0}},
    {{2,500000000}, {0,500000001}, {1,999999999}},
    {{-22222,-500000000}, {22222,500000001}, {-44445,-1}}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dsub(&result, &d[i].time, &d[i].stime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dsub", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dsub(0, &result, &d[i].stime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dsub", i);
      return;
    }
  }

  m_log->log('S', "Dsub", TIME__SUCCESS);
}

void ra_timetest::Dsub_NE(void)
{
  sDsub d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, pwr_cNDeltaTime},
    {{0,3}, {0,1}, {0,2}},
    {{300,3}, {100,1}, {200,2}},
    {{0,-1}, {0,10}, {0,-11}},
    {{-100,-10}, {100,1}, {-200,-11}},
    {{100,0}, {101,0}, {-1,0}},
    {{2,500000000}, {0,500000001}, {1,999999999}},
    {{-22222,-500000000}, {22222,500000001}, {-44445,-1}},
    {{10,-10}, {1,1}, pwr_cNotADeltaTime},
    {{1,1}, {-10,10}, pwr_cNotADeltaTime},
    {pwr_cNotADeltaTime, {1,1}, pwr_cNotADeltaTime},
    {{1,1}, pwr_cNotADeltaTime, pwr_cNotADeltaTime}
  };
  
  pwr_tDeltaTime result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Dsub_NE(&result, &d[i].time, &d[i].stime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dsub_NE", i);
      return;
    }
    memcpy(&result, &d[i].time, sizeof(pwr_tDeltaTime));
    time_Dsub_NE(0, &result, &d[i].stime);
    if (memcmp(&result, &d[i].result, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "Dsub_NE", i);
      return;
    }
  }

  m_log->log('S', "Dsub_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time1;
  pwr_tDeltaTime time2;
  int result;
} sDcomp;

void ra_timetest::Dcomp(void)
{
  sDcomp d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, 0},
    {{222,222222222}, {222,222222222}, 0},
    {{-222,-222222222}, {-222,-222222222}, 0},
    {{0,3}, {0,1}, 1},
    {{300,3}, {100,1}, 1},
    {{0,1}, {0,10}, -1},
    {{-100,-10}, {100,1}, -1},
    {{100,0}, {101,0}, -1},
    {{2,500000000}, {0,500000001}, 1},
    {{-22222,-500000000}, {22222,500000001}, -1}
  };
  
  int result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = time_Dcomp(&d[i].time1, &d[i].time2);
    if (result != d[i].result) {
      m_log->log('E', "Dcomp", i);
      return;
    }
    if (d[i].time2.tv_sec == 0 && d[i].time2.tv_nsec == 0) {
      result = time_Dcomp(&d[i].time1, 0);
      if (result != d[i].result) {
	m_log->log('E', "Dcomp", i);
	return;
      }
    }
  }

  m_log->log('S', "Dcomp", TIME__SUCCESS);
}

void ra_timetest::Dcomp_NE(void)
{
  sDcomp d[] = {
    {pwr_cNDeltaTime, pwr_cNDeltaTime, 0},
    {{222,222222222}, {222,222222222}, 0},
    {{-222,-222222222}, {-222,-222222222}, 0},
    {{0,3}, {0,1}, 1},
    {{300,3}, {100,1}, 1},
    {{0,1}, {0,10}, -1},
    {{-100,-10}, {100,1}, -1},
    {{100,0}, {101,0}, -1},
    {{2,500000000}, {0,500000001}, 1},
    {{-22222,-500000000}, {22222,500000001}, -1},
    {{10,-10}, {1,1}, -2},
    {{1,1}, {-10,10}, -2},
    {pwr_cNotADeltaTime, {1,1}, -2},
    {{1,1}, pwr_cNotADeltaTime, -2}
  };
  
  int result;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    result = time_Dcomp_NE(&d[i].time1, &d[i].time2);
    if (result != d[i].result) {
      m_log->log('E', "Dcomp", i);
      return;
    }
    if (d[i].time2.tv_sec == 0 && d[i].time2.tv_nsec == 0) {
      result = time_Dcomp_NE(&d[i].time1, 0);
      if (result != d[i].result) {
	m_log->log('E', "Dcomp", i);
	return;
      }
    }
  }

  m_log->log('S', "Dcomp_NE", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  char str[40];
  pwr_tStatus sts;
} sDtoAscii;

void ra_timetest::DtoAscii(void)
{
  sDtoAscii d[] = {
    {pwr_cNDeltaTime, "0:00:00.00", TIME__SUCCESS},
    {{0,222222222}, "0:00:00.22", TIME__SUCCESS},
    {{222,0}, "0:03:42.00", TIME__SUCCESS},
    {{222,222222222}, "0:03:42.22", TIME__SUCCESS},
    {{222222,222222222}, "2 13:43:42.22", TIME__SUCCESS},
    {{22222222,222222222}, "257 4:50:22.22", TIME__SUCCESS},
    {{0,-222222222}, "-0:00:00.22", TIME__SUCCESS},
    {{-222,0}, "-0:03:42.00", TIME__SUCCESS},
    {{-222,-222222222}, "-0:03:42.22", TIME__SUCCESS},
    {{-222222,-222222222}, "-2 13:43:42.22", TIME__SUCCESS},
    {{-22222222,-222222222}, "-257 4:50:22.22", TIME__SUCCESS},
    {{222,-222222222}, "", TIME__NADT},
    {{-222,222222222}, "", TIME__NADT}
  };
  
  char str[40];

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_DtoAscii(&d[i].time, 1, str, sizeof(str));
    if (m_sts != d[i].sts) {
      m_log->log('E', "DtoAscii wrong status", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(d[i].str, str) != 0) {
	m_log->log('E', "DtoAscii", i);
	return;
      }
    }
  }

  m_log->log('S', "DtoAscii", TIME__SUCCESS);
}

typedef struct {
  char str[40];
  pwr_tDeltaTime time;
  pwr_tStatus sts;
} sAsciiToD;

void ra_timetest::AsciiToD(void)
{
  sAsciiToD d[] = {
    {"0:00:00.00", pwr_cNDeltaTime, TIME__SUCCESS},
    {"0:00:00.22", {0,220000000}, TIME__SUCCESS},
    {"0:03:42.00", {222,0}, TIME__SUCCESS},
    {"0:03:42.22", {222,220000000}, TIME__SUCCESS},
    {"2 13:43:42.22", {222222,220000000}, TIME__SUCCESS},
    {"257 4:50:22.22", {22222222,220000000}, TIME__SUCCESS},
    {"-0:00:00.22", {0,-220000000}, TIME__SUCCESS},
    {"-0:03:42.00", {-222,0}, TIME__SUCCESS},
    {"-0:03:42.22", {-222,-220000000}, TIME__SUCCESS},
    {"-2 13:43:42.22", {-222222,-220000000}, TIME__SUCCESS},
    {"-257 4:50:22.22", {-22222222,-220000000}, TIME__SUCCESS},
    {"-0:03:42.22", {-222,-220000000}, TIME__SUCCESS},
    {"NotADeltaTime", pwr_cNotADeltaTime, TIME__SUCCESS}
  };
  
  pwr_tDeltaTime time;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_AsciiToD(d[i].str, &time);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AsciiToD wrong status", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (memcmp(&d[i].time, &time, sizeof(pwr_tDeltaTime)) != 0) {
	m_log->log('E', "AsciiToD", i);
	return;
      }
    }
  }

  m_log->log('S', "AsciiToD", TIME__SUCCESS);
}

typedef struct {
  pwr_tTime time;
  time_eFormat format;
  char str[40];
  pwr_tStatus sts;
} sAtoAscii;

void ra_timetest::AtoAscii(void)
{
  sAtoAscii d[] = {
    {pwr_cNTime, time_eFormat_DateAndTime, "01-JAN-1970 01:00:00.00", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_Time, "01:00:00.00", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_ComprDateAndTime, "70-01-01 01:00:00.00", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_NumDateAndTime, "1970-01-01 01:00:00.00", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_FileDateAndTime, "19700101_010000", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_FileDate, "19700101", TIME__SUCCESS},
    {pwr_cNTime, time_eFormat_TimeAndDate, "01:00:00 01/01/70", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_DateAndTime, "22-APR-2020 14:34:30.22", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_Time, "14:34:30.22", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_ComprDateAndTime, "20-04-22 14:34:30.22", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_NumDateAndTime, "2020-04-22 14:34:30.22", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_FileDateAndTime, "20200422_143430", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_FileDate, "20200422", TIME__SUCCESS},
    {{1587558870,220000000}, time_eFormat_TimeAndDate, "14:34:30 22/04/20", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_DateAndTime, "01-JUN-2050 15:00:00.00", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_Time, "15:00:00.00", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_ComprDateAndTime, "50-06-01 15:00:00.00", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_NumDateAndTime, "2050-06-01 15:00:00.00", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_FileDateAndTime, "20500601_150000", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_FileDate, "20500601", TIME__SUCCESS},
    {{2537701200,0}, time_eFormat_TimeAndDate, "15:00:00 01/06/50", TIME__SUCCESS},
    {{1587558870,-220000000}, time_eFormat_DateAndTime, "14:34:30 22/04/20", TIME__NAT},
    {{-1587558870,-220000000}, time_eFormat_DateAndTime, "14:34:30 22/04/20", TIME__NAT}
  };
  
  char str[40];

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_AtoAscii(&d[i].time, d[i].format, str, sizeof(str));
    if (m_sts != d[i].sts) {
      m_log->log('E', "AtoAscii wrong status", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(d[i].str, str) != 0) {
	m_log->vlog('E', "AtoAscii %s != %s, %d", d[i].str, str, i);
	return;
      }
    }
  }

  m_log->log('S', "AtoAscii", TIME__SUCCESS);
}

typedef struct {
  char str[40];
  pwr_tTime time;
  pwr_tStatus sts;
} sAsciiToA;

void ra_timetest::AsciiToA(void)
{
  sAsciiToA d[] = {
    {"01-JAN-1970 01:00:00.00", pwr_cNTime, TIME__SUCCESS},
    {"22-APR-2020 14:34:30.22", {1587558870,220000000}, TIME__SUCCESS},
    {"22-apr-2020 14:34:30.22", {1587558870,220000000}, TIME__SUCCESS},
    {"01-JUN-2050 15:00:00.00", {2537701200,0}, TIME__SUCCESS},
    {"01-JAN-2020 00:00:00.00", {1577833200,0}, TIME__SUCCESS},
    {"01-FEB-2020 00:00:00.00", {1580511600,0}, TIME__SUCCESS},
    {"01-MAR-2020 00:00:00.00", {1583017200,0}, TIME__SUCCESS},
    {"01-APR-2020 00:00:00.00", {1585692000,0}, TIME__SUCCESS},
    {"01-MAY-2020 00:00:00.00", {1588284000,0}, TIME__SUCCESS},
    {"01-JUN-2020 00:00:00.00", {1590962400,0}, TIME__SUCCESS},
    {"01-JUL-2020 00:00:00.00", {1593554400,0}, TIME__SUCCESS},
    {"01-AUG-2020 00:00:00.00", {1596232800,0}, TIME__SUCCESS},
    {"01-SEP-2020 00:00:00.00", {1598911200,0}, TIME__SUCCESS},
    {"01-OCT-2020 00:00:00.00", {1601503200,0}, TIME__SUCCESS},
    {"01-NOV-2020 00:00:00.00", {1604185200,0}, TIME__SUCCESS},
    {"01-DEC-2020 00:00:00.00", {1606777200,0}, TIME__SUCCESS},
    {"01-JAN-2020 00:00:00.00", {1577833200,0}, TIME__SUCCESS},
    {"01-JAN-2020 00:00:00.00", {1577833200,0}, TIME__SUCCESS},
    {"NotATime", pwr_cNotATime, TIME__SUCCESS},
    {"31-APR-2020 14:34:30.22", {0,0}, TIME__RANGE},
    {"22-ARP-2020 14:34:30.22", {0,0}, TIME__RANGE},
    {"22-APR-1820 14:34:30.22", {0,0}, TIME__RANGE},
    {"22-APR-2020 24:34:30.22", {0,0}, TIME__RANGE},
    {"22-APR-2020 14:64:30.22", {0,0}, TIME__RANGE},
    {"22-ApR-2020 14:34:70.22", {0,0}, TIME__RANGE}
  };
  
  pwr_tTime time;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_AsciiToA(d[i].str, &time);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AsciiToA wrong status", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (memcmp(&d[i].time, &time, sizeof(pwr_tTime)) != 0) {
	m_log->vlog('E', "AsciiToA %s, (%d,%d) != (%d,%d), %d", d[i].str, 
		    d[i].time.tv_sec, d[i].time.tv_nsec, 
		    time.tv_sec, time.tv_nsec, i);
	return;
      }
    }
  }

  m_log->log('S', "AsciiToA", TIME__SUCCESS);
}

typedef struct {
  int ms;
  pwr_tDeltaTime time;
} sMsToD;

void ra_timetest::MsToD(void)
{
  sMsToD d[] = {
    {0, pwr_cNDeltaTime},
    {1000, {1,0}},
    {123456789, {123456,789000000}},
    {-123456789, {-123456,-789000000}},
    {1, {0,1000000}}
  };
  
  pwr_tDeltaTime time;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_MsToD(&time, d[i].ms);
    if (memcmp(&d[i].time, &time, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "MsToD", i);
      return;
    }
  }

  m_log->log('S', "MsToD", TIME__SUCCESS);
}

typedef struct {
  pwr_tFloat32 f;
  pwr_tDeltaTime time;
} sFloatToD;

void ra_timetest::FloatToD(void)
{
  sFloatToD d[] = {
    {0, pwr_cNDeltaTime},
    {1.000, {1,0}},
    {456.789, {456,789000000}},
    {-123456.789, {-123456,-789000000}},
    {0.001, {0,1000000}}
  };
  
  pwr_tDeltaTime time;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_FloatToD(&time, d[i].f);
    if (!(d[i].time.tv_sec == time.tv_sec && abs(d[i].time.tv_nsec - time.tv_nsec) <= 100 * fabs(d[i].f))) {
      m_log->log('E', "FloatToD", i);
      return;
    }
  }

  m_log->log('S', "FloatToD", TIME__SUCCESS);
}

typedef struct {
  pwr_tFloat64 f;
  pwr_tDeltaTime time;
} sFloat64ToD;

void ra_timetest::Float64ToD(void)
{
  sFloat64ToD d[] = {
    {0, pwr_cNDeltaTime},
    {1.000, {1,0}},
    {456.789, {456,789000000}},
    {-123456.789, {-123456,-789000000}},
    {0.001, {0,1000000}}
  };
  
  pwr_tDeltaTime time;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_Float64ToD(&time, d[i].f);
    if (!(d[i].time.tv_sec == time.tv_sec && abs(d[i].time.tv_nsec - time.tv_nsec) <= 0.01 * fabs(d[i].f))) {
      m_log->log('E', "Float64ToD", i);
      return;
    }
  }

  m_log->log('S', "Float64ToD", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tFloat32 f;
} sDToFloat;

void ra_timetest::DToFloat(void)
{
  sDToFloat d[] = {
    {pwr_cNDeltaTime, 0},
    {{1,0}, 1.000},
    {{456,789000000}, 456.789},
    {{-123456,-789000000}, -123456.789},
    {{0,1000000}, 0.001}
  };
  
  pwr_tFloat32 f;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_DToFloat(&f, &d[i].time);
    if (fabs(d[i].f - f) > FLT_EPSILON * fabs(d[i].f)) {
      m_log->log('E', "DToFloat", i);
      return;
    }
  }

  m_log->log('S', "DToFloat", TIME__SUCCESS);
}

typedef struct {
  pwr_tDeltaTime time;
  pwr_tFloat64 f;
} sDToFloat64;

void ra_timetest::DToFloat64(void)
{
  sDToFloat64 d[] = {
    {pwr_cNDeltaTime, 0},
    {{1,0}, 1.000},
    {{456,789000000}, 456.789},
    {{-123456,-789000000}, -123456.789},
    {{0,1000000}, 0.001}
  };
  
  pwr_tFloat64 f;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    time_DToFloat64(&f, &d[i].time);
    if (fabs(d[i].f - f) > DBL_EPSILON * fabs(d[i].f)) {
      m_log->log('E', "DToFloat64", i);
      return;
    }
  }

  m_log->log('S', "DToFloat64", TIME__SUCCESS);
}

void ra_timetest::ZeroD(void)
{
  pwr_tDeltaTime t = {123,456};

  time_ZeroD(&t);
  if (!(t.tv_sec == 0 && t.tv_nsec == 0)) {
    m_log->log('E', "ZeroD");
    return;
  }

  m_log->log('S', "ZeroD", TIME__SUCCESS);
}

void ra_timetest::GetTime(void)
{
  pwr_tTime time, ctime;
  struct timespec t;
  int sts;
  pwr_tDeltaTime diff;
  pwr_tFloat32 fdiff;
  
  sts = clock_gettime(CLOCK_REALTIME, &t);
  if (sts) {
    m_log->log('E', "GetTime, clock_gettime failed", sts);
    return;
  }
  time_GetTime(&time);

  ctime.tv_sec = t.tv_sec;
  ctime.tv_nsec = t.tv_nsec;
  time_Adiff(&diff, &time, &ctime);
  time_DToFloat(&fdiff, &diff);
  if (fdiff > 0.001) {
    m_log->log('E', "GetTime, time doesn't match");
    return;
  }

  m_log->log('S', "GetTime", TIME__SUCCESS);
}

void ra_timetest::GetTimeMonotonic(void)
{
  pwr_tTime time, ctime;
  struct timespec t;
  int sts;
  pwr_tDeltaTime diff;
  pwr_tFloat32 fdiff;
  
  sts = clock_gettime(CLOCK_MONOTONIC, &t);
  if (sts) {
    m_log->log('E', "GetTimeMonotonic, clock_gettime failed", sts);
    return;
  }
  time_GetTimeMonotonic(&time);

  ctime.tv_sec = t.tv_sec;
  ctime.tv_nsec = t.tv_nsec;
  time_Adiff(&diff, &time, &ctime);
  time_DToFloat(&fdiff, &diff);
  if (fdiff > 0.001) {
    m_log->log('E', "GetTimeMonotonic, time doesn't match");
    return;
  }

  m_log->log('S', "GetTimeMonotonic", TIME__SUCCESS);
}


// Constructor
ra_timetest::ra_timetest()
{
  m_log = new tst_log(&m_sts, "co-Time", "$pwrp_log/time.log");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
ra_timetest::~ra_timetest()
{
  delete m_log;
}

int main()
{
  ra_timetest time;
  pwr_tStatus sts;

  setenv("TZ", "Europe/Stockholm", 1);
  tzset();

  sts = gdh_Init("ra_timetest");
  if (EVEN(sts)) {
    time.log()->log('S', "Time gdh_Init", sts);  
    exit(0);
  }

  time.IsNull();
  time.Aadd();
  time.Aadd_NE();
  time.Acomp();
  time.Acomp_NE();
  time.Adiff();
  time.Adiff_NE();
  time.Asub();
  time.Asub_NE();
  time.Dabs();
  time.Dabs_NE();
  time.Dadd();
  time.Dadd_NE();
  time.Dneg();
  time.Dneg_NE();
  time.Dsub();
  time.Dsub_NE();
  time.Dcomp();
  time.Dcomp_NE();
  time.DtoAscii();
  time.AsciiToD();
  time.AtoAscii();
  time.AsciiToA();
  time.MsToD();
  time.FloatToD();
  time.Float64ToD();
  time.DToFloat();
  time.DToFloat64();
  time.ZeroD();
  time.GetTime();
  time.GetTimeMonotonic();
}


