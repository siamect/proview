

#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "co_tst_log.h"

class ra_nethtest {
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_tOid m_root;
  pwr_tOid m_child;
  pwr_tOid m_dynroot;

  pwr_tStatus check_object(pwr_tOid oid);

 public:
  ra_nethtest();
  ~ra_nethtest();
  void ObjidToName(void);
  void GetChild(void);
  void GetParent(void);
  void GetNextSibling(void);
  void GetPreviousSibling(void);
  void NameToAttrref(void);
  void AttrrefToName(void);
  void ArefDisabled(void);
  void GetObjectSize(void);
  void GetObjectClass(void);
  void GetAttrRefTid(void);
  void GetObjectLocation(void);
};
