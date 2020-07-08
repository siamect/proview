

#include "pwr.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_tst_log.h"

class ra_timetest {
  pwr_tStatus m_sts;
  tst_log *m_log;


 public:
  ra_timetest();
  ~ra_timetest();
  tst_log *log() {return m_log;}

  void IsNull();
  void Aadd();
  void Aadd_NE();
  void Acomp();
  void Acomp_NE();
  void Adiff();
  void Adiff_NE();
  void Asub();
  void Asub_NE();
  void Dabs();
  void Dabs_NE();
  void Dadd();
  void Dadd_NE();
  void Dneg();
  void Dneg_NE();
  void Dsub();
  void Dsub_NE();
  void Dcomp();
  void Dcomp_NE();
  void DtoAscii();
  void AsciiToD();
  void AtoAscii();
  void AsciiToA();
  void MsToD();
  void FloatToD();
  void Float64ToD();
  void DToFloat();
  void DToFloat64();
  void ZeroD();
  void GetTime();
  void GetTimeMonotonic();
};
