/*
  Test qcom functions.
*/

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <math.h>
#include <pthread.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_syi.h"
#include "rt_gdh.h"
#include "ra_qcom2test.h"
#include "rt_qcom_msg.h"
#include "rt_hash_msg.h"
#include "rt_qdb_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

//static int plog = 1;

typedef struct {
  char		msg[1000];
  qcom_eBtype	btype;
  qcom_eStype	stype;
  pwr_tStatus	sts;
} sPut;

void ra_qcom2test::Put(void)
{
  sPut d[] = {
    {"Some message 1", (qcom_eBtype)2001, (qcom_eStype)1, QCOM__SUCCESS},
    {"Some message 2", (qcom_eBtype)32, (qcom_eStype)31, QCOM__SUCCESS},
    {"A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message,"
     "A longer message,  A longer message,  A longer message,  A longer message,  A longer message",
     (qcom_eBtype)2020, (qcom_eStype)31, QCOM__SUCCESS}
  };
    
  qcom_sQattr attr;
  qcom_sPut put;
  qcom_sGet get;
  qcom_sQid sqid = {3337,m_bvid};

  if (m_is_a) {
    // Send
    struct timespec t = {0, 100000000};

    m_log->vlog('D', "Put start");
    for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
	
      put.reply = sqid;
      put.type.b = d[i].btype;
      put.type.s = d[i].stype;
      put.size = strlen(d[i].msg) + 1;
      put.data = d[i].msg;

      qcom_Put(&m_sts, &sqid, &put);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Put", m_sts);
	return;
      }
      m_log->vlog('D', "Put message %d", i);
      nanosleep(&t, 0);
    }
    m_log->log('S', "Put", QCOM__SUCCESS);
  }
  else {
    // Receive
    attr.type = qcom_eQtype_private;
    attr.quota = 100;
    qcom_CreateQ(&m_sts, &sqid, &attr, "queue3337");
    if (m_sts == QCOM__QALLREXIST) {
      qcom_DeleteQ(&m_sts, &sqid);
      qcom_CreateQ(&m_sts, &sqid, &attr, "queue3337");
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "Put, qcom_CreateQ", m_sts);
      return;
    }
  
    m_log->vlog('D', "Put start");
    for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
      int tmo = 180000;
      get.data = 0;
      qcom_Get(&m_sts, &sqid, &get, tmo);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Get", m_sts);
	return;
      }

      if (strcmp(d[i].msg, (char *)get.data) != 0 ||
	  get.type.b != d[i].btype ||
	  get.type.s != d[i].stype ||
	  get.size != strlen(d[i].msg) + 1) {
	m_log->vlog('E', "Put, content differs, %d", i);
	return;
      }
      m_log->vlog('D', "Put message %d", i);
      qcom_Free(&m_sts, get.data);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Free", m_sts);
	return;
      }
    }
    m_log->log('S', "Put", QCOM__SUCCESS);
  }

  // Cyclic send
  if (m_is_a) {
    // Send
    m_log->vlog('D', "Cyclic put start");
    for (unsigned int i = 0; i < 10000; i++) {
      struct timespec t = {0, 2000000};
	
      put.reply = sqid;
      put.type.b = d[0].btype;
      put.type.s = d[0].stype;
      put.size = strlen(d[0].msg) + 1;
      put.data = d[0].msg;

      qcom_Put(&m_sts, &sqid, &put);
      if (EVEN(m_sts)) {
	if (i < 100) {
	  m_log->log('E', "Cyclic put, qcom_Put", m_sts);
	  return;
	}
	else {
	  if (m_sts != QDB__QUOTAEXCEEDED) {
	    m_log->log('E', "Put, qcom_Put", m_sts);
	    return;
	  }
	}
      }
      m_log->vlog('D', "Cyclic put %d", i);
      nanosleep(&t, 0);
    }
    m_log->log('S', "Cyclic put", QCOM__SUCCESS);
  }
  else {
    // Receive
    m_log->vlog('D', "Cyclic put start");
    for (unsigned int i = 0; i < 10000; i++) {
      int tmo = 2000;
      get.data = 0;
      qcom_Get(&m_sts, &sqid, &get, tmo);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Get", m_sts);
	return;
      }
      if (strcmp((char *)d[0].msg, (char *)get.data) != 0 ||
	  get.type.b != d[0].btype ||
	  get.type.s != d[0].stype ||
	  get.size != strlen(d[0].msg) + 1) {
	m_log->vlog('E', "Put, content differs, %d", i);
	return;
      }
      m_log->vlog('D', "Cyclic put %d", i);
      qcom_Free(&m_sts, get.data);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Free", m_sts);
	return;
      }
    }
    m_log->log('S', "Cyclic put", QCOM__SUCCESS);
  }

  // Check quota
  if (m_is_a) {
    // Send
    m_log->vlog('D', "Quota put start");
    for (unsigned int i = 0; i < 110; i++) {
      struct timespec t = {0, 1000000};
	
      put.reply = sqid;
      put.type.b = d[0].btype;
      put.type.s = d[0].stype;
      put.size = strlen(d[0].msg) + 1;
      put.data = d[0].msg;

      qcom_Put(&m_sts, &sqid, &put);
      if (EVEN(m_sts)) {
	if (i < 100) {
	  m_log->log('E', "Put, qcom_Put", m_sts);
	  return;
	}
	else {
	  if (m_sts != QDB__QUOTAEXCEEDED) {
	    m_log->log('E', "Put, qcom_Put", m_sts);
	    return;
	  }
	}
      }
      m_log->vlog('D', "Quota put %i", i);
      nanosleep(&t, 0);
    }
    m_log->log('S', "Cyclic put", QCOM__SUCCESS);
  }
  else {
    // Receive
    m_log->vlog('D', "Quota put start");
    for (unsigned int i = 0; i < 110; i++) {
      int tmo = 200;
      get.data = 0;
      qcom_Get(&m_sts, &sqid, &get, tmo);
      if (i == 0)
	sleep(5);
      if (EVEN(m_sts)) {
	if (i > 100 && m_sts == QCOM__TMO)
	  continue;
	else {
	  m_log->log('E', "Put, qcom_Get", m_sts);
	  return;
	}
      }
      if (strcmp((char *)d[0].msg, (char *)get.data) != 0 ||
	  get.type.b != d[0].btype ||
	  get.type.s != d[0].stype ||
	  get.size != strlen(d[0].msg) + 1) {
	m_log->vlog('E', "Put, content differs, %d", i);
	return;
      }
      m_log->vlog('D', "Quota put %i", i);
      qcom_Free(&m_sts, get.data);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Free", m_sts);
	return;
      }
    }
    m_log->log('S', "Cyclic put", QCOM__SUCCESS);
  }

  if (!m_is_a) {
    qcom_DeleteQ(&m_sts, &sqid);
    if (EVEN(m_sts)) {
      m_log->log('E', "Put, qcom_DeleteQ", m_sts);
      return;
    }
  }
  
  m_log->log('S', "Put", QCOM__SUCCESS);
}

// Constructor
ra_qcom2test::ra_qcom2test()
{
  pwr_tStatus sts;
  pwr_tNid nid;
  pwr_tTime stime;

  cdh_StringToVolumeId("_V0.254.254.201", &m_avid);
  cdh_StringToVolumeId("_V0.254.254.202", &m_bvid);

  sts = gdh_Init("ra_qcom2test");
  if (EVEN(sts)) {
    m_log->log('S', "qcom2 gdh_Init", sts);  
    exit(0);
  }

  sts = gdh_GetNodeIndex(&nid);
  if (EVEN(sts)) {
    m_log->log('S', "qcom2 gdh_GetNodeIndex", sts);  
    exit(0);
  }

  if (nid == m_avid)
    m_is_a = 1;
  else
    m_is_a = 0;

  if (m_is_a)
    m_log = new tst_log(&m_sts, "rt-Qcom2a", "$pwrp_log/qcom2a.tlog");
  else
    m_log = new tst_log(&m_sts, "rt-Qcom2b", "$pwrp_log/qcom2b.tlog");

  if (EVEN(m_sts))
    printf("** Unable to open log file");

  if (m_is_a) {
    // Wait for pwrtest01b
    for (int i = 0; i < 100; i++) {
      m_sts = gdh_GetObjectInfo("VolPwrtest01b:Nodes-PwrTest01b.SystemTime", &stime, sizeof(stime));
      sleep(2);
      printf("sts %d\n", m_sts);
      if (ODD(m_sts))
	break;
    }
    if (EVEN(m_sts)) {
      m_log->log('F', "pwrtest01b timeout", m_sts);
      exit(0);
    }
  }
}

// Destructor
ra_qcom2test::~ra_qcom2test()
{
  delete m_log;
}

int main()
{
  ra_qcom2test qcom;

  qcom.Put();
}


