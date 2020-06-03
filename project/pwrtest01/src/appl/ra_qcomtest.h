

#include "pwr.h"
#include "rt_qcom.h"
#include "co_tst_log.h"

class ra_qcomtest {
  pwr_tStatus m_sts;
  tst_log *m_log;


 public:
  ra_qcomtest();
  ~ra_qcomtest();
  tst_log *log() {return m_log;}

  void AidCompare();
  void AidIsEqual();
  void AidIsNotEqual();
  void AidIsNull();
  void AidIsNotNull();
  void QidCompare();
  void QidIsEqual();
  void QidIsNotEqual();
  void QidIsNull();
  void QidIsNotNull();
  void QidToString();
  void Alloc();
  void CreateQ();
  void Put();
  void Get();
  void MyBus();
  void NextNode();
  void MyNode();
  void Forwarding();
  void Signalling();
  void BindEvent();
  void Request();
};
