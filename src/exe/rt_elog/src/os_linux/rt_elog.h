#ifndef rt_elog_h
#define rt_elog_h

#ifndef pwr_h
#include "pwr.h"
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <db.h>
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_mh_def.h"
#include "rt_mh_outunit.h"
#include "co_dcli.h"

#define DUMPFILE "rt_eventlog.dump"
#define DATABASE "$pwrp_db/rt_eventlog.db"

#define DATABASETYPE DB_HASH
#define DATABASESTAT_STRUCT DB_BTREE_STAT

#define RT_ELOG_UNKNOWN_ERROR -2
#define RT_ELOG_OK             0
#define RT_ELOG_DB_EMPTY       1

/*typedef struct s_Event sEvent;
typedef struct s_Key sKey;
typedef struct s_HelCB sHelCB;
*/
typedef struct s_Event {
  mh_eEvent EventType;
  pwr_tTime EventTime;
  union {
    mh_sMessage message;
    mh_sAck ack;
    mh_sReturn ret;
    mh_sBlock block;
  } Mess;
}sEvent;

typedef struct s_Key {
  mh_sEventId Id;
  pwr_tTime EventTime;
}sKey;

typedef struct s_HelCB {
  int MaxStoreLSize;
  int StoreLSize;
  int Cardinality;
  int MaxCardinality;
  pwr_tInt32 ScanTime;
  pwr_tNodeId Nid;
  pwr_tInt64 StoreIdx;
}sHelCB;
#endif
