

#include "pwr.h"
#include "rt_aproc.h"
#include "co_tst_log.h"

class ra_aproctest {
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_sNode *m_nodep;
  errh_eAnix m_anix;

 public:
  ra_aproctest();
  ~ra_aproctest();
  tst_log *log() {return m_log;}

  void RegisterObject();
  void TimeStamp();
};
