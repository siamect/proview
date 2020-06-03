

#include "pwr.h"
#include "rt_errh.h"
#include "co_tst_log.h"

class ra_errhtest {
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_sNode *m_nodep;
  errh_eAnix m_anix;

 public:
  ra_errhtest();
  ~ra_errhtest();
  tst_log *log() {return m_log;}
  void set_anix(errh_eAnix anix);

  void Init();
  void Success();
  void Info();
  void Warning();
  void Error();
  void Fatal();
  void CErrLog();
  void SetAnix();
  void SetStatus();
  void LogSuccess();
  void LogInfo();
  void LogError();
  void LogFatal();
};
