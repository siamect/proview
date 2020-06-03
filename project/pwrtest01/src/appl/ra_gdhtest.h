

#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "co_tst_log.h"

class ra_gdhtest {
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_tOid m_root;
  pwr_tOid m_child;
  pwr_tOid m_dynroot;

  pwr_tStatus check_object(pwr_tOid oid);

 public:
  ra_gdhtest();
  ~ra_gdhtest();
  void Init();
  void ObjidToName(void);
  void ObjidToPointer(void);
  void MountObjidToPointer(void);
  void AttrRefToPointer(void);
  void NameToPointer(void);
  void CreateObject(void);
  void DeleteObject(void);
  void DeleteObjectTree(void);
  void RenameObject(void);
  void MoveObject(void);
  void GetRootList(void);
  void GetChild(void);
  void GetParent(void);
  void GetNextSibling(void);
  void GetPreviousSibling(void);
  void GetNodeIndex(void);
  void GetObjectNodeIndex(void);
  void GetNodeObject(void);
  void GetAttributeCharacteristics(void);
  void GetAttributeCharAttrref(void);
  void NameToAttrref(void);
  void AttrrefToName(void);
  void ClassAttrToAttrref(void);
  void AttrArefToObjectAref(void);
  void ArefANameToAref(void);
  void AttrValueToString(void);
  void AttrStringToValue(void);
  void ArefDisabled(void);
  void GetClassList(void);
  void GetNextObject(void);
  void GetPreviousObject(void);
  void GetClassListAttrRef(void);
  void GetNextAttrRef(void);
  void ClassNameToId(void);
  void GetSuperClass(void);
  void GetEnumValueDef(void);
  void GetMaskBitDef(void);
  void FWriteObject(void);
  void GetSecurityInfo(void);
  void TranslateRtdbPointer(void);
  void GetObjectSize(void);
  void GetDynamicAttrSize(void);
  void GetObjectClass(void);
  void GetAttrRefTid(void);
  void GetObjectLocation(void);
  void GetObjectDLCount(void);
  void GetVolumeList(void);
  void GetNextVolume(void);
  void VolumeIdToName(void);
  void GetVolumeInfo(void);
  void DLRefObjectInfoAttrref(void);
  void GetTimeDL(void);
  void SetTimeDL(void);
  void GetDeltaTimeDL(void);
  void SetDeltaTimeDL(void);
  void GetStrDL(void);
  void SetStrDL(void);
  void GetObjectInfoTime(void);
  void SetObjectInfoTime(void);
  void GetObjectInfoDeltaTime(void);
  void SetObjectInfoDeltaTime(void);
  void GetObjectInfoStr(void);
  void SetObjectInfoStr(void);
};
