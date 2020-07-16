

#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "co_tst_log.h"

class ra_status {
  pwr_tStatus m_sts;
  tst_log *m_log;

 public:
  ra_status();
  ~ra_status();
  void test_status();
};
