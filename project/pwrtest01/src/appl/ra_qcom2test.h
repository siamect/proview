

#include "pwr.h"
#include "rt_qcom.h"
#include "co_tst_log.h"

class ra_qcom2test {
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_tVid m_avid;
  pwr_tVid m_bvid;
  int m_is_a;


 public:
  ra_qcom2test();
  ~ra_qcom2test();
  tst_log *log() {return m_log;}

  void Put();
};
