

#include "pwr.h"
#include "rt_qcom.h"
#include "co_tst_log.h"

typedef enum {
  eProc_Server = 1,
  eProc_Client = 2
} eProc;

class ra_qmontest {
  pwr_tStatus m_sts;
  tst_log *m_log;
  qcom_sQid m_serverq;
  qcom_sQid m_clientq;
  eProc m_ptype;
  int m_qquota;
  int m_errcnt;
  int m_sent;  


 public:
  ra_qmontest(eProc ptype);
  ~ra_qmontest();
  tst_log *log() {return m_log;}
  void set_ptype(eProc ptype) {m_ptype = ptype;}
  bool isClient() {return m_ptype == eProc_Client;}
  bool isServer() {return m_ptype == eProc_Server;}
  void exec();
  void evaluate(int action, int result);
  void SPut();
  void CPut();
  void SPut10k();
  void CPut10k();
  void SPut100k();
  void CPut100k();
  void SPutCyclic();
  void CPutCyclic();
  void SEnd();
  void CEnd();
};
