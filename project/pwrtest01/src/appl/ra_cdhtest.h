

#include "pwr.h"
#include "co_cdh.h"
#include "co_tst_log.h"

class ra_cdhtest {
  pwr_tStatus m_sts;
  tst_log *m_log;


 public:
  ra_cdhtest();
  ~ra_cdhtest();
  tst_log *log() {return m_log;}

  void ObjidCompare();
  void ObjidIsEqual();
  void ObjidIsNotEqual();
  void ObjidIsNull();
  void ObjidIsNotNull();
  void ClassObjidToId();
  void ClassIdToObjid();
  void TypeObjidToId();
  void TypeIdToObjid();
  void TypeIdToIndex();
  void ObjidToAref();
  void AttrValueToString();
  void StringToAttrValue();
  void MaskToBinaryString();
  void StringToClassId();
  void StringToObjid();
  void StringToAref();
  void StringToTypeId();
  void StringToVolumeId();
  void StringToObjectIx();
  void ClassIdToString();
  void TypeIdToString();
  void ObjectIxToString();
  void ArefToString();
  void NodeIdToString();
  void OidToString();
  void OidToFnString();
  void VolumeIdToString();
  void VolumeIdToFnString();
  void SubidToString();
  void DlidToString();
  void Low();
  void OpSysToStr();
  void OpSysToDirStr();
  void StringToObjectName();
  void CutNameSegments();
  void AttrSize();
  void AttrElemSize();
};
