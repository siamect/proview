/*
  Test qmon functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_syi.h"
#include "rt_gdh.h"
#include "ra_qmontest.h"
#include "rt_qcom_msg.h"
#include "rt_hash_msg.h"
#include "rt_qdb_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static int plog = 1;

#define cServerVid "_V0.254.254.203"
#define cServerQix 3337
#define cClientVid "_V0.254.254.203"
#define cClientQix 3338

typedef enum {
  eTest_Put = 1,
  eTest_PutResult = 2,
  eTest_Put10k = 3,
  eTest_Put10kResult = 4,
  eTest_Put100k = 5,
  eTest_Put100kResult = 6,
  eTest_PutCyclic = 7,
  eTest_PutCyclicResult = 8,
  eTest_End = 99
} eTest;

void ra_qmontest::evaluate(int action, int result)
{
  switch(action) {
  case eTest_PutResult:
    if (result < m_qquota) {
      m_log->vlog('E', "Put, messages less than queue quota, %d/%d", m_sent, result);
      m_errcnt++;
    }
    else
      m_log->vlog('S', "Put, message sent/received, %d/%d", m_sent, result);

    break;
  case eTest_Put10kResult:
    if (result < m_qquota) {
      m_log->vlog('E', "Put10k, messages less than queue quota, %d/%d", m_sent, result);
      m_errcnt++;
    }
    else
      m_log->vlog('S', "Put10k, message sent/received, %d/%d", m_sent, result);
    break;
  case eTest_Put100kResult:
    if (result < m_qquota) {
      m_log->vlog('E', "Put100k, messages less than queue quota, %d/%d", m_sent, result);
      m_errcnt++;
    }
    else
      m_log->vlog('S', "Put100k, message sent/received, %d/%d", m_sent, result);
    break;
  case eTest_PutCyclicResult:
    if (m_sent != result) {
      m_log->vlog('E', "PutCyclic, message lost, %d/%d", m_sent, result);
      m_errcnt++;
    }
    else
      m_log->vlog('S', "PutCyclic, message sent/received, %d/%d", m_sent, result);
    break;
  default: ;
  }
}

void ra_qmontest::SPut()
{
  qcom_sPut put;
  int cnt = 1;

  m_sent = 0;
  for (int i = 0; i < 1000; i++) {
    put.reply = m_clientq;
    put.type.b = (qcom_eBtype)0;
    put.type.s = (qcom_eStype)0;
    put.size = sizeof(cnt);
    put.data = &cnt;

    qcom_Put(&m_sts, &m_clientq, &put);

    m_sent++;
    cnt++;
  }
  if (plog)
    m_log->vlog('D', "SPut sent messages %d", m_sent);
}

void ra_qmontest::CPut()
{
  qcom_sGet get;
  qcom_sPut put;
  int cnt;
  int action;

  action = 1;
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_Put;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(action);
  put.data = &action;
  
  if (plog)
    m_log->vlog('D', "Send request Put %d", action);
  qcom_Put(&m_sts, &m_serverq, &put);

  for (;;) {
    get.data = 0;
    qcom_Get(&m_sts, &m_clientq, &get, 1000);
    if (m_sts == QCOM__TMO) {
      break;
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "clientq, qcom_Get", m_sts);
      exit(0);
    }
    cnt = *(int *)get.data;
    qcom_Free(&m_sts, get.data);
  }
  m_log->vlog('D', "CPut received messages %d", cnt);

  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_PutResult;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(cnt);
  put.data = &cnt;
  qcom_Put(&m_sts, &m_serverq, &put);
}

void ra_qmontest::SPut10k()
{
  qcom_sPut put;
  int cnt = 1;
  unsigned int size = 10000;
  unsigned int *data;

  data = (unsigned int *)malloc(size);
  for (unsigned int i = 0; i < size/sizeof(int); i++)
    data[i] = 0x55555555;

  m_sent = 0;
  for (int i = 0; i < 1000; i++) {
    data[0] = cnt;
    put.reply = m_clientq;
    put.type.b = (qcom_eBtype)0;
    put.type.s = (qcom_eStype)0;
    put.size = size;
    put.data = data;

    qcom_Put(&m_sts, &m_clientq, &put);

    m_sent++;
    cnt++;
  }
  if (plog)
    m_log->vlog('D', "SPut10k sent messages %d", m_sent);
}

void ra_qmontest::CPut10k()
{
  qcom_sGet get;
  qcom_sPut put;
  int cnt;
  unsigned int size = 10000;
  int action;
  int error_logged = 0;

  action = 1;
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_Put10k;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(action);
  put.data = &action;
  
  if (plog)
    m_log->vlog('D', "Send request Put10k %d", action);
  qcom_Put(&m_sts, &m_serverq, &put);

  for (;;) {
    get.data = 0;
    qcom_Get(&m_sts, &m_clientq, &get, 1000);
    if (m_sts == QCOM__TMO) {
      break;
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "clientq, qcom_Get", m_sts);
      exit(0);
    }
    cnt = *(int *)get.data;

    if (get.size != size) {
      if (!error_logged) {
	m_log->vlog('X', "put10k, size mismatch %d", get.size);
	error_logged = 1;
      }
      m_errcnt++;
    }
    for (unsigned int i = 1; i < size/sizeof(int); i++) {
      if (((int *)get.data)[i] != 0x55555555) {
	if (!error_logged) {
	  m_log->vlog('X', "put10k, content mismatch %d", i);
	  error_logged = 1;
	}
      }
    }

    qcom_Free(&m_sts, get.data);
  }
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_Put10kResult;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(cnt);
  put.data = &cnt;
  qcom_Put(&m_sts, &m_serverq, &put);
  m_log->vlog('D', "CPut10k received messages %d", cnt);
}

void ra_qmontest::SPut100k()
{
  qcom_sPut put;
  int cnt = 1;
  unsigned int size = 10000;
  unsigned int *data;

  data = (unsigned int *)malloc(size);
  for (unsigned int i = 0; i < size/sizeof(int); i++)
    data[i] = 0x55555555;

  m_sent = 0;
  for (int i = 0; i < 1000; i++) {
    data[0] = cnt;
    put.reply = m_clientq;
    put.type.b = (qcom_eBtype)0;
    put.type.s = (qcom_eStype)0;
    put.size = size;
    put.data = data;

    qcom_Put(&m_sts, &m_clientq, &put);

    m_sent++;
    cnt++;
  }
  if (plog)
    m_log->vlog('D', "SPut100k sent messages %d", m_sent);
}

void ra_qmontest::CPut100k()
{
  qcom_sGet get;
  qcom_sPut put;
  int cnt;
  unsigned int size = 10000;
  int action;
  int error_logged = 0;

  action = 1;
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_Put100k;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(action);
  put.data = &action;
  
  if (plog)
    m_log->vlog('D', "Send request Put100k %d", action);
  qcom_Put(&m_sts, &m_serverq, &put);

  for (;;) {
    get.data = 0;
    qcom_Get(&m_sts, &m_clientq, &get, 1000);
    if (m_sts == QCOM__TMO) {
      break;
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "clientq, qcom_Get", m_sts);
      exit(0);
    }
    cnt = *(int *)get.data;

    if (get.size != size) {
      if (!error_logged) {
	m_log->vlog('X', "put100k, size mismatch %d", get.size);
	error_logged = 1;
      }
      m_errcnt++;
    }
    for (unsigned int i = 1; i < size/sizeof(int); i++) {
      if (((int *)get.data)[i] != 0x55555555) {
	if (!error_logged) {
	  m_log->vlog('X', "put100k, content mismatch %d", i);
	  error_logged = 1;
	}
      }
    }

    qcom_Free(&m_sts, get.data);
  }
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_Put100kResult;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(cnt);
  put.data = &cnt;
  qcom_Put(&m_sts, &m_serverq, &put);
  m_log->vlog('D', "CPut100k received messages %d", cnt);
}

void ra_qmontest::SPutCyclic()
{
  qcom_sPut put;
  int cnt = 1;
  float t = 0.01;

  m_sent = 0;
  for (int i = 0; i < 1000; i++) {
    put.reply = m_clientq;
    put.type.b = (qcom_eBtype)0;
    put.type.s = (qcom_eStype)0;
    put.size = sizeof(cnt);
    put.data = &cnt;

    qcom_Put(&m_sts, &m_clientq, &put);
    time_Sleep(t);

    m_sent++;
    cnt++;
  }
  if (plog)
    m_log->vlog('D', "SPutCyclic sent messages %d", m_sent);
}

void ra_qmontest::CPutCyclic()
{
  qcom_sGet get;
  qcom_sPut put;
  int cnt;
  int action;

  action = 2;
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_PutCyclic;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(action);
  put.data = &action;
  
  if (plog)
    m_log->vlog('D', "Send request PutCyclic %d", action);
  qcom_Put(&m_sts, &m_serverq, &put);

  for (;;) {
    get.data = 0;
    qcom_Get(&m_sts, &m_clientq, &get, 1000);
    if (m_sts == QCOM__TMO) {
      break;
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "clientq, qcom_Get", m_sts);
      exit(0);
    }
    cnt = *(int *)get.data;
    qcom_Free(&m_sts, get.data);
  }
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_PutCyclicResult;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(cnt);
  put.data = &cnt;
  qcom_Put(&m_sts, &m_serverq, &put);
  m_log->vlog('D', "CPutCyclic received messages %d", cnt);
}
void ra_qmontest::SEnd()
{
#if 0
  if (m_errcnt)
    m_log->vlog('E', "Errors %d", m_errcnt);
  else
    m_log->log('S', "Success", QCOM__SUCCESS);
#endif
}

void ra_qmontest::CEnd()
{
  qcom_sPut put;
  int action;

  action = eTest_End;
  put.reply = m_clientq;
  put.type.b = (qcom_eBtype)eTest_End;
  put.type.s = (qcom_eStype)0;
  put.size = sizeof(action);
  put.data = &action;
  
  if (plog)
    m_log->vlog('D', "Send request End %d", action);
  qcom_Put(&m_sts, &m_serverq, &put);

  if (m_errcnt)
    m_log->vlog('E', "Errors %d", m_errcnt);
  else
    m_log->log('S', "Success", QCOM__SUCCESS);
}

void ra_qmontest::exec()
{
  if (isServer()) {
    // Server
    qcom_sGet get;
    int action;
    int end = 0;

    for (;;) {

      get.data = 0;
      qcom_Get(&m_sts, &m_serverq, &get, qcom_cTmoEternal);
      if (EVEN(m_sts)) {
	m_log->log('E', "serverq, qcom_Get", m_sts);
	exit(0);
      }
      action = get.type.b;

      if (plog)
	m_log->vlog('D', "Action request %d", action);
      switch (action) {
      case eTest_Put:
	SPut();
	break;
      case eTest_Put10k:
	SPut10k();
	break;
      case eTest_Put100k:
	SPut100k();
	break;
      case eTest_PutCyclic:
	SPutCyclic();
	break;
      case eTest_PutResult:
      case eTest_Put10kResult:
      case eTest_Put100kResult:
      case eTest_PutCyclicResult: {
	int result = *(int *)get.data;
	evaluate(action, result);
	break;
      }
      case eTest_End:
	SEnd();
	end = 1;
	break;
      }
      qcom_Free(&m_sts, get.data);
      if (end)
	break;
    }
  }
  else {
    // Client
    CPut();
    sleep(1);
    CPut10k();
    sleep(1);
    CPut100k();
    sleep(1);
    CPutCyclic();
    CEnd();
  }
}



// Constructor
ra_qmontest::ra_qmontest(eProc ptype) : m_ptype(ptype), m_qquota(500), m_errcnt(0), 
   m_sent(0)
{
  qcom_sQattr attr;
  pwr_tVid vid;  

  if (isServer())
    m_log = new tst_log(&m_sts, "rt-Qmon", "$pwrp_log/qmon.log");
  else
    m_log = new tst_log(&m_sts, "rt-QmonClient", "$pwrp_log/qmonc.log");
  if (EVEN(m_sts))
    printf("** Unable to open log file");

  m_sts = gdh_Init("ra_qmontest");
  if (EVEN(m_sts)) {
    m_log->log('S', "qmon gdh_Init", m_sts);  
    exit(0);
  }

  cdh_StringToVolumeId(cServerVid, &vid);
  m_serverq.qix = cServerQix;
  m_serverq.nid = vid;
  cdh_StringToVolumeId(cClientVid, &vid);
  m_clientq.qix = cClientQix;
  m_clientq.nid = vid;

  if (isServer()) {
    attr.type = qcom_eQtype_private;
    attr.quota = m_qquota;
    qcom_CreateQ(&m_sts, &m_serverq, &attr, "queue3348");    
    if (m_sts == QCOM__QALLREXIST) {
      qcom_StealQ(&m_sts, &m_serverq);
      //qcom_AttachQ(&m_sts, &m_serverq);
      //if (m_sts == QCOM__ALLRATTACHED)
      //m_sts = QCOM__SUCCESS;
      //qcom_DeleteQ(&m_sts, &m_serverq);
      //qcom_CreateQ(&m_sts, &m_serverq, &attr, "queue3348");
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "create serverq, qcom_CreateQ", m_sts);
      exit(0);
    }

    for (;;) {
      qcom_AttachQ(&m_sts, &m_clientq);
      if (m_sts == QCOM__NOQ)
	sleep(1);
      else if (m_sts == QCOM__ALLRATTACHED)
	break;
      else if (EVEN(m_sts)) {
	m_log->log('E', "attach clientq, qcom_AttachQ", m_sts);
	exit(0);
      }
      else
	break;
    }
  }
  else {
    // Client
    attr.type = qcom_eQtype_private;
    attr.quota = 10;
    qcom_CreateQ(&m_sts, &m_clientq, &attr, "queue3348");    
    if (m_sts == QCOM__QALLREXIST) {
      qcom_StealQ(&m_sts, &m_clientq);
      //if (m_sts == QCOM__ALLRATTACHED)
      //m_sts = QCOM__SUCCESS;
      //qcom_DeleteQ(&m_sts, &m_clientq);
      //qcom_CreateQ(&m_sts, &m_clientq, &attr, "queue3348");
    }
    if (EVEN(m_sts)) {
      m_log->log('E', "create clientq, qcom_CreateQ", m_sts);
      exit(0);
    }

    for (;;) {
      qcom_AttachQ(&m_sts, &m_serverq);
      if (m_sts == QCOM__NOQ)
	sleep(1);
      else if (m_sts == QCOM__ALLRATTACHED)
        break;
      else if (EVEN(m_sts)) {
	m_log->log('E', "attach serverq, qcom_AttachQ", m_sts);
	exit(0);
      }
      else
	break;
    }
  }
}

// Destructor
ra_qmontest::~ra_qmontest()
{
  delete m_log;
}

int main(int argc, char *argv[])
{
  eProc ptype = eProc_Server;
  if (argc > 1 && strcmp(argv[1], "2") == 0)
    ptype = eProc_Client;

  ra_qmontest qmon(ptype);
  qmon.exec();

}
