//
// Example of a proview application program
//

#include "rt_appl.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"
#include "co_error.h"


void rt_appl::init()
{
  qcom_sQid qini;
  qcom_sQattr qAttr;
  pwr_tStatus sts;

  // Init error and status logger with a unic application index per node.
  errh_Init( m_name, m_anix);
  errh_SetStatus( PWR__APPLSTARTUP);

  // Init database
  sts = gdh_Init("rs_appl");
  if ( EVEN(sts)) {
    errh_Fatal( "gdh_Init, %m", sts);
    errh_SetStatus( PWR__APPLTERM);
    exit(sts);
  }

  // Create a queue to receive stop and restart events
  if (!qcom_Init(&sts, 0, "rs_appl")) {
    errh_Fatal("qcom_Init, %m", sts); 
    errh_SetStatus( PWR__APPLTERM);
    exit(sts);
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, &m_qid, &qAttr, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    errh_SetStatus( PWR__APPLTERM);
    exit(sts);
  } 

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &m_qid, &qini)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    errh_SetStatus( PWR__APPLTERM);
    exit(-1);
  }
}

void rt_appl::register_appl( char *name)
{
  pwr_tOid oid;
  pwr_tStatus sts;

  // Get configuration object
  sts = gdh_NameToObjid( name, &oid);
  if ( EVEN(sts)) throw co_error(sts);

  aproc_RegisterObject( oid);
}

void rt_appl::mainloop()
{
  pwr_tStatus sts;
  int tmo;
  char mp[2000];
  qcom_sGet get;
  int swap = 0;
  bool first_scan = true;

  try {
    open();
  }
  catch ( co_error& e) {
    errh_Error( (char *)e.what().c_str());
    errh_Fatal( "rs_appl aborting");
    errh_SetStatus( PWR__APPLTERM);
    exit(0);
  }

  aproc_TimeStamp();
  errh_SetStatus( PWR__ARUN);

  first_scan = true;
  for (;;) {
    if ( first_scan) {
      tmo = (int) (m_scantime * 1000 - 1);
    }

    get.maxSize = sizeof(mp);
    get.data = mp;
    qcom_Get( &sts, &m_qid, &get, tmo);
    if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
      if ( !swap) {
	aproc_TimeStamp();
	scan();
      }
    } 
    else {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.oldPlcStop && !swap) {
	errh_SetStatus( PWR__APPLRESTART);
        swap = 1;
	close();
      } else if (new_event.b.swapDone && swap) {
        swap = 0;
	open();
	errh_SetStatus( PWR__ARUN);
      } else if (new_event.b.terminate) {
	exit(0);
      }
    }
    first_scan = false;
  }
}






