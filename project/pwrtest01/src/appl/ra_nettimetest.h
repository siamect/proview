

#include "pwr.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_tst_log.h"

class ra_nettimetest {
  pwr_tStatus m_sts;
  tst_log *m_log;


 public:
  ra_nettimetest();
  ~ra_nettimetest();
  tst_log *log() {return m_log;}

  void NetTimeToTime();
  void NetTimeToDeltaTime();
};
