/*
  Test qcom functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <pthread.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_syi.h"
#include "rt_gdh.h"
#include "ra_qcomtest.h"
#include "rt_qcom_msg.h"
#include "rt_hash_msg.h"
#include "rt_qdb_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static int plog = 1;

typedef struct {
  qcom_sAid aid1;
  qcom_sAid aid2;
  int sts;
} sAidCompare;

void ra_qcomtest::AidCompare(void)
{
  sAidCompare d[] = {
    {{0,0}, {0,0}, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, -1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, -1},
    {{11111,1}, {11112,2}, -1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_AidCompare(&d[i].aid1, &d[i].aid2);
    if (sts != d[i].sts) {
      m_log->log('E', "AidCompare", i);
      return;
    }
  }

  m_log->log('S', "AidCompare", QCOM__SUCCESS);
}

void ra_qcomtest::AidIsEqual(void)
{
  sAidCompare d[] = {
    {{0,0}, {0,0}, 1},
    {{11111,1}, {11111,1}, 1},
    {{11112,1}, {11111,1}, 0},
    {{11111,1}, {11112,1}, 0},
    {{11112,2}, {11111,1}, 0},
    {{11111,2}, {11112,1}, 0},
    {{11112,1}, {11111,2}, 0},
    {{11111,1}, {11112,2}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_AidIsEqual(&d[i].aid1, &d[i].aid2);
    if (sts != d[i].sts) {
      m_log->log('E', "AidIsEqual", i);
      return;
    }
  }

  m_log->log('S', "AidIsEqual", QCOM__SUCCESS);
}

void ra_qcomtest::AidIsNotEqual(void)
{
  sAidCompare d[] = {
    {{0,0}, {0,0}, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, 1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, 1},
    {{11111,1}, {11112,2}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_AidIsNotEqual(&d[i].aid1, &d[i].aid2);
    if (sts != d[i].sts) {
      m_log->log('E', "AidIsNotEqual", i);
      return;
    }
  }

  m_log->log('S', "AidIsNotEqual", QCOM__SUCCESS);
}

typedef struct {
  qcom_sAid aid;
  int sts;
} sAidIsNull;

void ra_qcomtest::AidIsNull(void)
{
  sAidIsNull d[] = {
    {{0,0}, 1},
    {{11111,1}, 0},
    {{11112,1}, 0},
    {{0,1}, 0},
    {{11112,0}, 0},
    {{11111,0}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_AidIsNull(&d[i].aid);
    if (sts != d[i].sts) {
      m_log->log('E', "AidIsNull", i);
      return;
    }
  }

  m_log->log('S', "AidIsNull", QCOM__SUCCESS);
}

void ra_qcomtest::AidIsNotNull(void)
{
  sAidIsNull d[] = {
    {{0,0}, 0},
    {{11111,1}, 1},
    {{11112,1}, 1},
    {{0,1}, 1},
    {{11112,0}, 1},
    {{11111,0}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_AidIsNotNull(&d[i].aid);
    if (sts != d[i].sts) {
      m_log->log('E', "AidIsNotNull", i);
      return;
    }
  }

  m_log->log('S', "AidIsNotNull", QCOM__SUCCESS);
}

typedef struct {
  qcom_sQid qid1;
  qcom_sQid qid2;
  int sts;
} sQidCompare;

void ra_qcomtest::QidCompare(void)
{
  sQidCompare d[] = {
    {{0,0}, {0,0}, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, -1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, -1},
    {{11111,1}, {11112,2}, -1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_QidCompare(&d[i].qid1, &d[i].qid2);
    if (sts != d[i].sts) {
      m_log->log('E', "QidCompare", i);
      return;
    }
  }

  m_log->log('S', "QidCompare", QCOM__SUCCESS);
}

void ra_qcomtest::QidIsEqual(void)
{
  sQidCompare d[] = {
    {{0,0}, {0,0}, 1},
    {{11111,1}, {11111,1}, 1},
    {{11112,1}, {11111,1}, 0},
    {{11111,1}, {11112,1}, 0},
    {{11112,2}, {11111,1}, 0},
    {{11111,2}, {11112,1}, 0},
    {{11112,1}, {11111,2}, 0},
    {{11111,1}, {11112,2}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_QidIsEqual(&d[i].qid1, &d[i].qid2);
    if (sts != d[i].sts) {
      m_log->log('E', "QidIsEqual", i);
      return;
    }
  }

  m_log->log('S', "QidIsEqual", QCOM__SUCCESS);
}

void ra_qcomtest::QidIsNotEqual(void)
{
  sQidCompare d[] = {
    {{0,0}, {0,0}, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, 1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, 1},
    {{11111,1}, {11112,2}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_QidIsNotEqual(&d[i].qid1, &d[i].qid2);
    if (sts != d[i].sts) {
      m_log->log('E', "QidIsNotEqual", i);
      return;
    }
  }

  m_log->log('S', "QidIsNotEqual", QCOM__SUCCESS);
}

typedef struct {
  qcom_sQid qid;
  int sts;
} sQidIsNull;

void ra_qcomtest::QidIsNull(void)
{
  sQidIsNull d[] = {
    {{0,0}, 1},
    {{11111,1}, 0},
    {{11112,1}, 0},
    {{0,1}, 0},
    {{11112,0}, 0},
    {{11111,0}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_QidIsNull(&d[i].qid);
    if (sts != d[i].sts) {
      m_log->log('E', "QidIsNull", i);
      return;
    }
  }

  m_log->log('S', "QidIsNull", QCOM__SUCCESS);
}

void ra_qcomtest::QidIsNotNull(void)
{
  sQidIsNull d[] = {
    {{0,0}, 0},
    {{11111,1}, 1},
    {{11112,1}, 1},
    {{0,1}, 1},
    {{11112,0}, 1},
    {{11111,0}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = qcom_QidIsNotNull(&d[i].qid);
    if (sts != d[i].sts) {
      m_log->log('E', "QidIsNotNull", i);
      return;
    }
  }

  m_log->log('S', "QidIsNotNull", QCOM__SUCCESS);
}

typedef struct {
  qcom_sQid	qid;
  int		prefix;
  char		str[40];
} sQidToString;

void ra_qcomtest::QidToString(void)
{
  sQidToString d[] = {
    {{0,0}, 0, "0.0.0.0:0"},
    {{0,0}, 1, "_Q0.0.0.0:0"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 0, "0.1.1.1:1"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 1, "_Q0.1.1.1:1"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 0, "200.44.55.66:77777"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 1, "_Q200.44.55.66:77777"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, "");
    qcom_QidToString(str, &d[i].qid, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "QidToString", i);
      return;
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, qcom_QidToString(0, &d[i].qid, d[i].prefix));
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "QidToString, ObjidToString", i);
      return;
    }
  }

  m_log->log('S', "QidToString", QCOM__SUCCESS);
}

typedef struct {
  unsigned int	size;
  pwr_tStatus	asts;
  pwr_tStatus	fsts;
} sAlloc;

void ra_qcomtest::Alloc(void)
{
  sAlloc d[] = {
    {0, QCOM__SUCCESS, QCOM__SUCCESS},
    {4, QCOM__SUCCESS, QCOM__SUCCESS},
    {400, QCOM__SUCCESS, QCOM__SUCCESS},
    {40000, QCOM__SUCCESS, QCOM__SUCCESS},
    {4000000, QCOM__SUCCESS, QCOM__SUCCESS},
    {40000000, QCOM__SUCCESS, QCOM__SUCCESS}
  };
    
  void *p;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    p = qcom_Alloc(&m_sts, d[i].size);
    if (m_sts != d[i].asts) {
      m_log->vlog('E', "Alloc", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      qcom_Free(&m_sts, p);
      if (m_sts != d[i].asts) {
	m_log->vlog('E', "Alloc, Free", m_sts);
	return;
      }
    }
  }

  m_log->log('S', "Alloc", QCOM__SUCCESS);
}

typedef struct {
  qcom_sQid	qid;
  char		name[32];
  qcom_eQtype 	type;
  int 		quota;
  pwr_tStatus	csts;
  pwr_tBoolean	creturn;
  pwr_tStatus	dsts;
  pwr_tBoolean	dreturn;
} sCreateQ;

void ra_qcomtest::CreateQ(void)
{
  sCreateQ d[] = {
    {{0,0}, "queue1", qcom_eQtype_private, 100, QCOM__SUCCESS, 1, HASH__SUCCESS, 1},
    {{3333,0}, "queue3333", qcom_eQtype_private, 100, QCOM__SUCCESS, 1, HASH__SUCCESS, 1},
    {{3334,0}, "queue3334", qcom_eQtype_forward, 100, QCOM__SUCCESS, 1, HASH__SUCCESS, 1},
    {{3335,0}, "queue3335", qcom_eQtype_broadcast, 100, QCOM__SUCCESS, 1, HASH__SUCCESS, 1},
    {{3336,0}, "queue3336", qcom_eQtype_event, 100, QCOM__SUCCESS, 1, HASH__SUCCESS, 1}
  };
    
  qcom_sQattr attr;
  pwr_tBoolean b;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    attr.type = d[i].type;
    attr.quota = d[i].quota;
    b = qcom_CreateQ(&m_sts, &d[i].qid, &attr, d[i].name);
    if (m_sts != d[i].csts || b != d[i].creturn) {
      m_log->log('E', "CreateQ", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      b = qcom_DeleteQ(&m_sts, &d[i].qid);
      if (m_sts != d[i].dsts || b != d[i].dreturn) {
	m_log->log('E', "CreateQ, qcom_DeleteQ", m_sts);
	return;
      }
    }
  }

  m_log->log('S', "CreateQ", QCOM__SUCCESS);
}

typedef struct {
  char		msg[1000];
  qcom_eBtype	btype;
  qcom_eStype	stype;
  pwr_tStatus	sts;
} sPut;

void ra_qcomtest::Put(void)
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
  qcom_sQid sqid = {3337,0};

  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &sqid, &attr, "queue3337");
  if (EVEN(m_sts)) {
    m_log->log('E', "Put, qcom_CreateQ", m_sts);
    return;
  }
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
	
    put.reply = sqid;
    put.type.b = d[i].btype;
    put.type.s = d[i].stype;
    put.size = strlen(d[i].msg) + 1;
    put.data = d[i].msg;

    qcom_Put(&m_sts, &sqid, &put);
    if (ODD(m_sts)) {
      get.data = 0;
      qcom_Get(&m_sts, &sqid, &get, qcom_cTmoEternal);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Get", m_sts);
	return;
      }

      if (strcmp((char *)put.data, (char *)get.data) != 0 ||
	  get.type.b != d[i].btype ||
	  get.type.s != d[i].stype ||
	  get.size != strlen(d[i].msg) + 1) {
	m_log->vlog('E', "Put, content differs, %d", i);
	return;
      }
      qcom_Free(&m_sts, get.data);
      if (EVEN(m_sts)) {
	m_log->log('E', "Put, qcom_Free", m_sts);
	return;
      }
    }
  }

  // Check quota
  for (unsigned int i = 0; i < 110; i++) {
	
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

  }
  for (unsigned int i = 0; i < 100; i++) {
    get.data = 0;
    qcom_Get(&m_sts, &sqid, &get, qcom_cTmoEternal);
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
    qcom_Free(&m_sts, get.data);
    if (EVEN(m_sts)) {
      m_log->log('E', "Put, qcom_Free", m_sts);
      return;
    }
  }

  qcom_DeleteQ(&m_sts, &sqid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Put, qcom_DeleteQ", m_sts);
    return;
  }
  
  m_log->log('S', "Put", QCOM__SUCCESS);
}

typedef struct {
  char		msg[1000];
  qcom_eBtype	btype;
  qcom_eStype	stype;
  int		tmotest;
  int		tmo;
  int		alloc;
  pwr_tStatus	sts;
} sGet;

void ra_qcomtest::Get(void)
{
  sGet d[] = {
    {"Some message 1", (qcom_eBtype)2001, (qcom_eStype)1, 0, qcom_cTmoEternal, 1, QCOM__SUCCESS},
    {"Some message 2", (qcom_eBtype)32, (qcom_eStype)31, 0, 20, 0, QCOM__SUCCESS},
    {"Some message 3", (qcom_eBtype)32, (qcom_eStype)31, 1, 20, 0, QCOM__TMO},
    {"Some message 4", (qcom_eBtype)32, (qcom_eStype)31, 1, 200, 0, QCOM__TMO},
    {"Some message 5", (qcom_eBtype)32, (qcom_eStype)31, 1, 2000, 0, QCOM__TMO},
    {"Some message 6", (qcom_eBtype)32, (qcom_eStype)31, 1, 10000, 0, QCOM__TMO}
  };
    
  qcom_sQattr attr;
  qcom_sPut put;
  qcom_sGet get;
  qcom_sQid sqid = {3338,0};
  pwr_tTime before, after;

  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &sqid, &attr, "queue3338");
  if (EVEN(m_sts)) {
    m_log->log('E', "Get, qcom_CreateQ", m_sts);
    return;
  }

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
	
    put.reply = sqid;
    put.type.b = d[i].btype;
    put.type.s = d[i].stype;
    put.size = strlen(d[i].msg) + 1;
    put.data = d[i].msg;

    if (!d[i].tmotest) {
      qcom_Put(&m_sts, &sqid, &put);
      if (EVEN(m_sts)) {
	m_log->log('E', "Get, qcom_Put", m_sts);
	return;
      }
    }

    if (d[i].alloc) {
      get.maxSize = strlen(d[i].msg) + 1;
      get.data = malloc(get.maxSize);
    }
    else 
      get.data = 0;
    time_GetTime(&before);
    qcom_Get(&m_sts, &sqid, &get, d[i].tmo);
    time_GetTime(&after);
    if (m_sts != d[i].sts) {
      m_log->log('E', "Get, qcom_Get", m_sts);
      return;
    }
    if (!d[i].tmotest) {
      if (strcmp((char *)put.data, (char *)get.data) != 0 ||
	  get.type.b != d[i].btype ||
	  get.type.s != d[i].stype ||
	  get.size != strlen(d[i].msg) + 1) {
	m_log->vlog('E', "Get, content differs, %d", i);
	return;
      }
      if (d[i].alloc)
	free(get.data);
      else {
	qcom_Free(&m_sts, get.data);
	if (EVEN(m_sts)) {
	  m_log->log('E', "Get, qcom_Free", m_sts);
	  return;
	}
      }
    }
    else {
      // Check time
      pwr_tDeltaTime dt;
      pwr_tFloat32 df;
      int di;

      time_Adiff(&dt, &after, &before);
      time_DToFloat(&df, &dt);
      di = int(df * 1000 + 0.5);
      if (abs(d[i].tmo - di) > 1) {
	m_log->vlog('E', "Get, timeout mismatch %d %d", d[i].tmo, di);
	return;
      }
    }
  }

  qcom_DeleteQ(&m_sts, &sqid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Get, qcom_DeleteQ", m_sts);
    return;
  }

  m_log->log('S', "Get", QCOM__SUCCESS);
}

void ra_qcomtest::MyBus(void)
{
  qcom_tBus bus;
  char *s;
  int num, pwr_bus_id;
    

  bus = qcom_MyBus(&m_sts);
  if (EVEN(m_sts)) {
    m_log->log('E', "MyBus", m_sts);
    return;
  }
  s = getenv("PWR_BUS_ID");
  if (!s) {
    m_log->log('E', "MyBus, couldn't get PWR_BUS_ID");
    return;
  }
  num = sscanf(s, "%d", &pwr_bus_id);
  if (num != 1) {
    m_log->log('E', "MyBus, PWR_BUS_ID syntax error");
    return;
  }
  if (pwr_bus_id != bus) {
    m_log->vlog('E', "MyBus, bus differs %d %d", bus, pwr_bus_id);
    return;
  }

  m_log->log('S', "MyBus", QCOM__SUCCESS);
}

void ra_qcomtest::MyNode(void)
{
  qcom_sNode node;
  char name[80], nname[80];

  syi_NodeName(&m_sts, nname, sizeof(nname));
  if (EVEN(m_sts)) {
    m_log->log('E', "MyNode, syi_Nodename", m_sts);
    return;    
  }

  qcom_MyNode(&m_sts, &node);
  if (EVEN(m_sts)) {
    m_log->log('E', "MyNode, syi_Nodename", m_sts);
    return;    
  }
  strncpy(name, qcom_NodeName(node.nid), sizeof(name));
  if (strcmp(name, nname) != 0) {
    m_log->vlog('E', "MyNode, node not found %s", nname);
    return;    
  }

  m_log->log('S', "MyNode", QCOM__SUCCESS);
}

void ra_qcomtest::NextNode(void)
{
  pwr_tNid nid;
  qcom_sNode node;
  char name[80], nname[80];
  int found = 0;

  syi_NodeName(&m_sts, nname, sizeof(nname));
  if (EVEN(m_sts)) {
    m_log->log('E', "NextNode, syi_Nodename", m_sts);
    return;    
  }

  for (nid = qcom_cNNid; qcom_NextNode(&m_sts, &node, nid); nid = node.nid) {
    strncpy(name, qcom_NodeName(node.nid), sizeof(name));
    if (strcmp(name, nname) == 0)
      found = 1;
  }
  if (!found) {
    m_log->vlog('E', "NextNode, node not found %s", nname);
    return;    
  }

  m_log->log('S', "NextNode", QCOM__SUCCESS);
}

typedef struct {
  char		msg[1000];
  qcom_eBtype	btype;
  qcom_eStype	stype;
} sForwarding;

void ra_qcomtest::Forwarding(void)
{
  sForwarding d[] = {
    {"Some message 1", (qcom_eBtype)2001, (qcom_eStype)1},
    {"Some message 2", (qcom_eBtype)32, (qcom_eStype)31},
    {"Some message 3", (qcom_eBtype)32, (qcom_eStype)31},
    {"Some message 4", (qcom_eBtype)32, (qcom_eStype)31},
    {"Some message 5", (qcom_eBtype)32, (qcom_eStype)31},
    {"Some message 6", (qcom_eBtype)32, (qcom_eStype)31}
  };
    
  qcom_sQattr attr;
  qcom_sPut put;
  qcom_sGet get;
  qcom_sQid private_qid = {3339,0};
  qcom_sQid forw1_qid = {3340,0};
  qcom_sQid forw2_qid = {3341,0};

  // Create two forwarding queues and one private
  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &private_qid, &attr, "queue3339");
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_CreateQ", m_sts);
    return;
  }
  attr.type = qcom_eQtype_forward;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &forw1_qid, &attr, "queue3340");
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_CreateQ", m_sts);
    return;
  }
  attr.type = qcom_eQtype_forward;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &forw2_qid, &attr, "queue3341");
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_CreateQ", m_sts);
    return;
  }

  // Bind private queue to forwarding queues
  qcom_Bind(&m_sts, &private_qid, &forw1_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_Bind", m_sts);
    return;
  }
  qcom_Bind(&m_sts, &private_qid, &forw2_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_Bind", m_sts);
    return;
  }

  // Put and get each message
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
	
    put.reply = private_qid;
    put.type.b = d[i].btype;
    put.type.s = d[i].stype;
    put.size = strlen(d[i].msg) + 1;
    put.data = d[i].msg;

    if (EVEN(i)) 
      qcom_Put(&m_sts, &forw1_qid, &put);
    else
      qcom_Put(&m_sts, &forw2_qid, &put);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Put", m_sts);
      return;
    }
    get.data = 0;

    qcom_Get(&m_sts, &private_qid, &get, qcom_cTmoEternal);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Get", m_sts);
      return;
    }

    if (strcmp((char *)put.data, (char *)get.data) != 0 ||
	get.type.b != d[i].btype ||
	get.type.s != d[i].stype ||
	get.size != strlen(d[i].msg) + 1) {
      m_log->vlog('E', "Forwarding, content differs, %d", i);
      return;
    }
    qcom_Free(&m_sts, get.data);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Free", m_sts);
      return;
    }
  }

  // Put all messages first and then get
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
	
    put.reply = private_qid;
    put.type.b = d[i].btype;
    put.type.s = d[i].stype;
    put.size = strlen(d[i].msg) + 1;
    put.data = d[i].msg;

    if (EVEN(i)) 
      qcom_Put(&m_sts, &forw1_qid, &put);
    else
      qcom_Put(&m_sts, &forw2_qid, &put);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Put", m_sts);
      return;
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    get.data = 0;

    qcom_Get(&m_sts, &private_qid, &get, qcom_cTmoEternal);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Get", m_sts);
      return;
    }

    if (strcmp((char *)d[i].msg, (char *)get.data) != 0 ||
	get.type.b != d[i].btype ||
	get.type.s != d[i].stype ||
	get.size != strlen(d[i].msg) + 1) {
      m_log->vlog('E', "Forwarding, content differs, %d", i);
      return;
    }
    qcom_Free(&m_sts, get.data);
    if (EVEN(m_sts)) {
      m_log->log('E', "Forwarding, qcom_Free", m_sts);
      return;
    }
  }

  qcom_Unbind(&m_sts, &forw1_qid, &private_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_UnbindQ", m_sts);
    return;
  }
  qcom_Unbind(&m_sts, &forw2_qid, &private_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_Unbind", m_sts);
    return;
  }

  qcom_DeleteQ(&m_sts, &private_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_DeleteQ", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &forw1_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_DeleteQ", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &forw2_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Forwarding, qcom_DeleteQ", m_sts);
    return;
  }

  m_log->log('S', "Forwarding", QCOM__SUCCESS);
}


typedef struct {
  int is_event;
  int wait_and;
  char msg[100];
  qcom_eBtype	btype;
  qcom_eStype	stype;
  int mask;
  int event;
} sSignallingData;

typedef struct {
  ra_qcomtest *qcomtest;
  int errcnt;
  qcom_sQid event_qid;
  qcom_sQid msg_qid;
  unsigned int thread_waiting;
  unsigned int thread_awake;
  sSignallingData d[12];
} sSignalling;

static void *signal_listen(void *arg)
{
  sSignalling *d = (sSignalling *)arg;
  pwr_tStatus sts;
  int event;
  qcom_sGet get;

  for (unsigned int i = 0; i < sizeof(d->d)/sizeof(d->d[0]); i++) {

    d->thread_waiting = 1;
    d->qcomtest->log()->vlog('D', "Thread waiting %d", i);
      
    if (d->d[i].wait_and) {
      qcom_WaitAnd(&sts, &d->msg_qid, &d->event_qid, d->d[i].mask, 
		  qcom_cTmoEternal);    
    }
    else {
      qcom_WaitOr(&sts, &d->msg_qid, &d->event_qid, d->d[i].mask, 
		  qcom_cTmoEternal, &event);
      
      if (plog)
	d->qcomtest->log()->vlog('D', "Thread awake %d %d", i, event);
      if (event != d->d[i].event) {
	d->qcomtest->log()->vlog('X', "Signalling, event differs (%d != %d), %d", 
				 event, d->d[i].event, i);
	d->errcnt++;
      }
    
      if (event == 0) {
	get.data = 0;
	qcom_Get(&sts, &d->msg_qid, &get, 100);
	if EVEN(sts) {
	  d->qcomtest->log()->log('X', "Signalling, qcom_Get", sts);
	  d->errcnt++;
	}
	else {
	  if (strcmp(d->d[i].msg, (char *)get.data) != 0) {
	    d->qcomtest->log()->vlog('X', "Get, content differs, %d", i);
	    d->errcnt++;
	  }
	  qcom_Free(&sts, get.data);
	  if (EVEN(sts)) {
	    d->qcomtest->log()->log('X', "Get, qcom_Free", sts);
	    d->errcnt++;
	  }
	}
      }
    }
    d->thread_waiting = 0;
    d->thread_awake++;
    time_Sleep(0.1);
  }
  pthread_exit(0);
}

void ra_qcomtest::Signalling(void)
{
  sSignalling d = {
    this, 0, {3342,0}, {3343,0}, 0, 0, {
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x101, 0x100},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x104, 0x004},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0xfff, 0x1},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x100, 0x100},
      {0, 0, "Some message 1", (qcom_eBtype)5, (qcom_eStype)5, 0x000, 0x000},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x100, 0x100},
      {0, 0, "Some message 2", (qcom_eBtype)5, (qcom_eStype)5, 0x000, 0x000},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x100, 0x100},
      {0, 0, "Some message 3", (qcom_eBtype)5, (qcom_eStype)5, 0x000, 0x000},
      {1, 0, "", (qcom_eBtype)0, (qcom_eStype)0, 0x100, 0x100},
      {1, 1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x100, 0x100},
      {1, 1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x104, 0x104}
    }};

  qcom_sQattr attr;
  pthread_t tid;
  int psts;
  qcom_sPut put;

  // Create event queue
  attr.type = qcom_eQtype_event;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.event_qid, &attr, "queue3342");
  if (EVEN(m_sts)) {
    m_log->log('E', "Signalling, qcom_CreateQ", m_sts);
    return;
  }

  // Create message queue
  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.msg_qid, &attr, "queue3343");
  if (EVEN(m_sts)) {
    m_log->log('E', "Signalling, qcom_CreateQ", m_sts);
    return;
  }

  // Create a listening thread
  psts = pthread_create(&tid, NULL, signal_listen, (void*)&d);
  if (psts != 0) {
    m_log->vlog('E', "Signalling, pthread_create %d", psts);
    return;
  }
  time_Sleep(1);
  for (unsigned int i = 0; i < sizeof(d.d)/sizeof(d.d[0]); i++) {
	
    if (d.d[i].is_event) {
      if (plog)
	m_log->vlog('D', "Signalling %d %d", i, d.d[i].event);
      qcom_SignalOr(&m_sts, &d.event_qid, d.d[i].event);
      time_Sleep(0.1);
      if (plog)
	m_log->vlog('D', "Signalling %d 0", i);
      qcom_SignalAnd(&m_sts, &d.event_qid, 0);
    }
    else {
      put.reply = d.msg_qid;
      put.type.b = d.d[i].btype;
      put.type.s = d.d[i].stype;
      put.size = strlen(d.d[i].msg) + 1;
      put.data = d.d[i].msg;

      m_log->vlog('D', "Putting %d", i);
      qcom_Put(&m_sts, &d.msg_qid, &put);
      if (EVEN(m_sts)) {
	m_log->log('E', "Signalling, qcom_Put", m_sts);
	return;
      }
    }

    time_Sleep(0.1);
    if (d.thread_awake != i + 1) {
      m_log->vlog('E', "Signalling, index mismatch (%d != %d)",
		  d.thread_awake, i+1);
      return;
    }
    time_Sleep(0.1);
  }
  
  pthread_join(tid, NULL);

  qcom_DeleteQ(&m_sts, &d.event_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Signalling, qcom_DeleteQ", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &d.msg_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Signalling, qcom_DeleteQ", m_sts);
    return;
  }

  if (d.errcnt)
    m_log->vlog('E', "Signalling, %d errors", d.errcnt);
  else
    m_log->log('S', "Signalling", QCOM__SUCCESS);
}

typedef struct {
  int is_event;
  char msg[100];
  qcom_eBtype	btype;
  qcom_eStype	stype;
  int event;
} sBindEventData;

typedef struct {
  ra_qcomtest *qcomtest;
  int errcnt;
  qcom_sQid event_qid;
  qcom_sQid msg_qid;
  unsigned int thread_waiting;
  unsigned int thread_awake;
  sBindEventData d[10];
} sBindEvent;

static void *bindevent_listen(void *arg)
{
  sBindEvent *d = (sBindEvent *)arg;
  pwr_tStatus sts;
  qcom_sGet get;
  qcom_sEvent *ep;

  for (unsigned int i = 0; i < sizeof(d->d)/sizeof(d->d[0]); i++) {
    
    d->thread_waiting = 1;
    d->qcomtest->log()->vlog('D', "Thread waiting %d", i);

    get.data = 0;
    qcom_Get(&sts, &d->msg_qid, &get, qcom_cTmoEternal);
    if EVEN(sts) {
      d->qcomtest->log()->log('X', "BindEvent, qcom_Get", sts);
      d->errcnt++;
    }
    else {
      d->qcomtest->log()->vlog('D', "Thread awake %d %d", i, get.type.b);
      switch(get.type.b) {
      case qcom_eBtype_event: 
	ep = (qcom_sEvent *)get.data;
	if (ep->mask != d->d[i].event) {
	  d->qcomtest->log()->vlog('X', "BindEvent, event differs %d != %d", 
				  ep->mask, d->d[i].event);	  
	  d->errcnt++;
	}
	break;
      default:
	if (strcmp(d->d[i].msg, (char *)get.data) != 0) {
	  d->qcomtest->log()->vlog('X', "Get, content differs, %d", i);
	  d->errcnt++;
	}
      }
      qcom_Free(&sts, get.data);
      if (EVEN(sts)) {
	d->qcomtest->log()->log('X', "Get, qcom_Free", sts);
	d->errcnt++;
      }
    }
  
    d->thread_waiting = 0;
    d->thread_awake++;
    time_Sleep(0.1);
  }
  pthread_exit(0);
}

void ra_qcomtest::BindEvent(void)
{
  sBindEvent d = {
    this, 0, {3344,0}, {3345,0}, 0, 0, {
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {0, "Some message 1", (qcom_eBtype)5, (qcom_eStype)5, 0x000},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {0, "Some message 2", (qcom_eBtype)5, (qcom_eStype)5, 0x000},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004},
      {0, "Some message 3", (qcom_eBtype)5, (qcom_eStype)5, 0x000},
      {1, "", (qcom_eBtype)0, (qcom_eStype)0, 0x004}
    }};

  qcom_sQattr attr;
  pthread_t tid;
  int psts;
  qcom_sPut put;

  // Create event queue
  attr.type = qcom_eQtype_event;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.event_qid, &attr, "queue3344");
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_CreateQ", m_sts);
    return;
  }

  // Create message queue
  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.msg_qid, &attr, "queue3345");
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_CreateQ", m_sts);
    return;
  }

  // Bind event que to message queue
  qcom_Bind(&m_sts, &d.msg_qid, &d.event_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_Bind", m_sts);
    return;
  }

  // Create a listening thread
  psts = pthread_create(&tid, NULL, bindevent_listen, (void*)&d);
  if (psts != 0) {
    m_log->vlog('E', "BindEvent, pthread_create %d", psts);
    return;
  }



  time_Sleep(1);
  for (unsigned int i = 0; i < sizeof(d.d)/sizeof(d.d[0]); i++) {
	
    if (d.d[i].is_event) {
      if (plog)
	m_log->vlog('D', "BindEvent %d %d", i, d.d[i].event);
      qcom_SignalOr(&m_sts, &d.event_qid, d.d[i].event);
      time_Sleep(0.1);
      //if (plog)
      //m_log->vlog('D', "BindEvent %d 0", i);
      //qcom_SignalAnd(&m_sts, &d.event_qid, 0);
    }
    else {
      put.reply = d.msg_qid;
      put.type.b = d.d[i].btype;
      put.type.s = d.d[i].stype;
      put.size = strlen(d.d[i].msg) + 1;
      put.data = d.d[i].msg;

      m_log->vlog('D', "Putting %d", i);
      qcom_Put(&m_sts, &d.msg_qid, &put);
      if (EVEN(m_sts)) {
	m_log->log('X', "BindEvent, qcom_Put", m_sts);
	d.errcnt++;
      }
    }

    time_Sleep(0.1);
    if (d.thread_awake != i + 1) {
      m_log->vlog('X', "BindEvent, index mismatch (%d != %d)",
		  d.thread_awake, i+1);
      d.errcnt++;
    }
    time_Sleep(0.1);
  }
  //time_Sleep(10000);
  pthread_join(tid, NULL);

  qcom_Unbind(&m_sts, &d.event_qid, &d.msg_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_Unbind", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &d.event_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_DeleteQ", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &d.msg_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "BindEvent, qcom_DeleteQ", m_sts);
    return;
  }

  if (d.errcnt)
    m_log->vlog('E', "BindEvent, %d errors", d.errcnt);
  else
    m_log->log('S', "BindEvent", QCOM__SUCCESS);
}

typedef struct {
  char msg[100];
  char rmsg[100];
  qcom_eBtype	btype;
  qcom_eStype	stype;
} sRequestData;

typedef struct {
  ra_qcomtest *qcomtest;
  int errcnt;
  qcom_sQid request_qid;
  qcom_sQid reply_qid;
  unsigned int thread_awake;
  sRequestData d[12];
} sRequest;

static void *request_listen(void *arg)
{
  sRequest *d = (sRequest *)arg;
  pwr_tStatus sts;
  qcom_sGet get;
  qcom_sPut put;

  for (unsigned int i = 0; i < sizeof(d->d)/sizeof(d->d[0]); i++) {

    if (plog)
      d->qcomtest->log()->vlog('D', "Thread waiting %d", i);

    get.data = 0;
    qcom_Get(&sts, &d->request_qid, &get, qcom_cTmoEternal);
    if EVEN(sts) {
      d->qcomtest->log()->log('X', "Request thread, qcom_Get", sts);
      d->errcnt++;
    }
    else {
      if (strcmp(d->d[i].msg, (char *)get.data) != 0) {
	d->qcomtest->log()->vlog('X', "Request thread, content differs, %d", i);
	d->errcnt++;
      }

      put.reply = d->reply_qid;
      put.type.b = d->d[i].btype;
      put.type.s = d->d[i].stype;
      put.size = strlen(d->d[i].rmsg) + 1;
      put.data = d->d[i].rmsg;

      qcom_Reply(&sts, &get, &put);

      qcom_Free(&sts, get.data);
      if (EVEN(sts)) {
	d->qcomtest->log()->log('X', "Request thread, qcom_Free", sts);
	d->errcnt++;
      }
    }
    d->thread_awake++;
  }
  pthread_exit(0);
}

void ra_qcomtest::Request(void)
{
  sRequest d = {
    this, 0, {3346,0}, {3347,0}, 0, {
      {"Request message 1", "Reply message 1", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 2", "Reply message 2", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 3", "Reply message 3", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 4", "Reply message 4", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 5", "Reply message 5", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 6", "Reply message 6", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 7", "Reply message 7", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 8", "Reply message 8", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 9", "Reply message 9", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 10", "Reply message 10", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 11", "Reply message 11", (qcom_eBtype)5, (qcom_eStype)5},
      {"Request message 12", "Reply message 12", (qcom_eBtype)5, (qcom_eStype)5}
    }};

  qcom_sQattr attr;
  pthread_t tid;
  int psts;
  qcom_sPut put;
  qcom_sGet get;

  // Create request queue
  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.request_qid, &attr, "queue3346");
  if (EVEN(m_sts)) {
    m_log->log('E', "Request, qcom_CreateQ", m_sts);
    return;
  }

  // Create reply queue
  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qcom_CreateQ(&m_sts, &d.reply_qid, &attr, "queue3346");
  if (EVEN(m_sts)) {
    m_log->log('E', "Request, qcom_CreateQ", m_sts);
    return;
  }

  // Create a listening thread
  psts = pthread_create(&tid, NULL, request_listen, (void*)&d);
  if (psts != 0) {
    m_log->vlog('E', "Request, pthread_create %d", psts);
    return;
  }
  time_Sleep(1);
  for (unsigned int i = 0; i < sizeof(d.d)/sizeof(d.d[0]); i++) {
	
    put.reply = d.reply_qid;
    put.type.b = d.d[i].btype;
    put.type.s = d.d[i].stype;
    put.size = strlen(d.d[i].msg) + 1;
    put.data = d.d[i].msg;

    get.data = 0;

    if (plog)
      m_log->vlog('D', "Requesting %d", i);
    qcom_Request(&m_sts, &d.request_qid, &put, &d.reply_qid, &get, 
		 100000, 0);
    if (EVEN(m_sts)) {
      m_log->log('E', "Request, qcom_Request", m_sts);
      return;
    }

    if (strcmp(d.d[i].rmsg, (char *)get.data) != 0) {
      m_log->vlog('E', "Request, reply content differs %d", i);
      return;
    }

    qcom_Free(&m_sts, get.data);
    if (EVEN(m_sts)) {
      m_log->log('E', "Request, qcom_Free", m_sts);
      return;
    }

    time_Sleep(0.1);
    if (d.thread_awake != i + 1) {
      m_log->vlog('E', "Request, index mismatch (%d != %d)",
		  d.thread_awake, i+1);
      return;
    }
    time_Sleep(0.1);
  }
  
  pthread_join(tid, NULL);

  qcom_DeleteQ(&m_sts, &d.request_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Request, qcom_DeleteQ", m_sts);
    return;
  }
  qcom_DeleteQ(&m_sts, &d.reply_qid);
  if (EVEN(m_sts)) {
    m_log->log('E', "Request, qcom_DeleteQ", m_sts);
    return;
  }

  if (d.errcnt)
    m_log->vlog('E', "Request, %d errors", d.errcnt);
  else
    m_log->log('S', "Request", QCOM__SUCCESS);
}

// Constructor
ra_qcomtest::ra_qcomtest()
{
  m_log = new tst_log(&m_sts, "rt-Qcom", "$pwrp_log/qcom.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
ra_qcomtest::~ra_qcomtest()
{
  delete m_log;
}

int main()
{
  ra_qcomtest qcom;
  pwr_tStatus sts;

  sts = gdh_Init("ra_qcomtest");
  if (EVEN(sts)) {
    qcom.log()->log('S', "qcom gdh_Init", sts);  
    exit(0);
  }

  qcom.AidCompare();
  qcom.AidIsEqual();
  qcom.AidIsNotEqual();
  qcom.AidIsNull();
  qcom.AidIsNotNull();
  qcom.QidCompare();
  qcom.QidIsEqual();
  qcom.QidIsNotEqual();
  qcom.QidIsNull();
  qcom.QidIsNotNull();
  qcom.QidToString();
  qcom.Alloc();
  qcom.CreateQ();
  qcom.Put();
  qcom.Get();
  qcom.MyBus();
  qcom.MyNode();
  qcom.NextNode();
  qcom.Forwarding();
  qcom.Signalling();
  qcom.BindEvent();
  qcom.Request();
}


