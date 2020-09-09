

#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "co_tst_log.h"

class wa_ldhtest {
  pwr_tStatus m_sts;
  ldh_tWorkbench m_ldhwb;
  ldh_tSession m_ldhses;
  ldh_tVolContext m_volctx;
  pwr_tObjName m_vname;
  pwr_tVid m_vid;

  tst_log *m_log;
  pwr_tOid m_test;
  pwr_tOid m_test_h1;
  pwr_tOid m_test_h2;
  pwr_tOid m_root;

  pwr_tStatus check_object(pwr_tOid oid);

 public:
  wa_ldhtest();
  ~wa_ldhtest();
  void vname(char *n) {strncpy(m_vname, n, sizeof(m_vname));}
  void Init();
  void Close();
  void ObjidToName();
  void CreateObject();
  void DeleteObject();
  void DeleteObjectTree();
  void ChangeObjectName();
  void MoveObject();
  void GetRootList();
  void GetChild();
  void GetParent();
  void GetNextSibling();
  void GetPreviousSibling();
  void NameToAttrRef();
  void AttrRefToName();
  void AttrArefToObjectAref();
  void GetClassList();
  void GetNextObject();
  void GetNextAttrRef();
  void ClassNameToId();
  void ClassIdToName();
  void GetSuperClass();
  void GetEnumValueDef();
  void GetMaskBitDef();
  void GetObjectClass();
  void GetAttrRefTid();
  void GetVolumeList();
  void GetNextVolume();
  void GetVolumeClass();
  void GetVidInfo();
  void GetVolumeInfo();
  void AttachVolume();
  void OpenWB();
  void OpenSession();
  void CopyObject();
  void CopyObjectTrees();
  void CreateVolume();
  void GetObjectPar();
  void SetObjectPar();
};
