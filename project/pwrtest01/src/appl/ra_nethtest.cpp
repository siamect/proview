/*
  Test neth functions.
*/


#include <stdio.h>
#include <unistd.h>
#include "pwr.h"
#include "rt_gdh.h"
#include "ra_nethtest.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rt_mvol_msg.h"
#include "co_cdh_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"


typedef struct {
  pwr_tOName name;
  pwr_tStatus sts1;
  pwr_tStatus sts2;
  pwr_tOid oid;
} sObjidToName;

// NameToObjid and ObjidToName
void ra_nethtest::ObjidToName(void)
{
  sObjidToName d[] = {
    {"Test01b-Neth", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01b-Neth-ƒ≈÷‰Âˆ", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01b-Neth-LongName90123456789012345678901", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01b-Neth-TooLongName234567890123456789012", CDH__SEGLEN, 0, pwr_cNOid}
  };
  pwr_tOName oname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &d[i].oid);
    if (m_sts != d[i].sts1) {
      m_log->log('E', "ObjidToName, NameToObjid wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(d[i].oid, oname, sizeof(oname), cdh_mNName);
      if (m_sts != d[i].sts2) {
	m_log->log('E', "ObjidToName, wrong sts", d[i].name, m_sts);
	return;
      }
      else if (ODD(m_sts)) {
	if (strcmp(oname, d[i].name) != 0) {
	  m_log->log('E', "ObjidToName, wrong object", d[i].name);
	  return;
	}
      }
    }
  }

  m_log->log('S', "ObjidToName", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName pname;
  pwr_tObjName cname;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid poid;
  pwr_tOid coid;
} sGetChild;

void ra_nethtest::GetChild(void)
{
  sGetChild d[] = {
    {"Test01b-RootObject", "Child1", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid, pwr_cNOid},
    {"Test01b-RootObject-ChildLess", "", pwr_cClass_PlantHier, GDH__NO_CHILD, pwr_cNOid, pwr_cNOid}
  };
  pwr_tAName cname;
  pwr_tOid coid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].pname, &d[i].poid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, gdh_NameToObjid", d[i].pname, m_sts);
      return;
    }
    sprintf(cname, "%s-%s", d[i].pname, d[i].cname);
    m_sts = gdh_NameToObjid(cname, &d[i].coid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, gdh_NameToObjid", d[i].cname, m_sts);
      return;
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_GetChild(d[i].poid, &coid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetChild, wrong sts", d[i].pname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (cdh_ObjidIsNotEqual(coid, d[i].coid)) {
	m_log->log('E', "GetChild, wrong object", d[i].cname);
	return;
      }
    }
  }

  m_log->log('S', "GetChild", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tStatus sts;
} sGetParent;

void ra_nethtest::GetParent(void)
{
  sGetParent d[] = {
    {"Test01b", GDH__NO_PARENT},
    {"Test01b-RootObject-Child1", HASH__SUCCESS},
    {"Test01b-RootObject-ChildLess", HASH__SUCCESS}
  };
  pwr_tOName pname, name;
  pwr_tOid coid, poid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &coid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = gdh_GetParent(coid, &poid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetParent, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(poid, pname, sizeof(pname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetChild, gdh_ObjidToName", d[i].name, m_sts);
	return;
      }

      strcpy(name, d[i].name);
      char *s = strrchr(name, '-');
      if (!s) {
	m_log->log('E', "GetParent, erroneous name", d[i].name);
	return;
      }
      *s = 0;
      if (strcmp(name, pname) != 0) {
	m_log->log('E', "GetParent, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetParent", GDH__SUCCESS);
}

// Constructor
ra_nethtest::ra_nethtest()
{
  pwr_tTime stime;

  m_log = new tst_log(&m_sts, "rt-Neth", "$pwrp_log/neth.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");

  m_sts = gdh_Init("ra_nethtest");
  if (EVEN(m_sts)) {
    m_log->log('F', "gdh_Init", m_sts);
    exit(0);
  }

  // Wait for pwrtest01b
  for (int i = 0; i < 100; i++) {
    m_sts = gdh_GetObjectInfo("VolPwrtest01b:Nodes-PwrTest01b.SystemTime", &stime, sizeof(stime));
    sleep(2);
    printf("sts %d\n", m_sts);
    if (ODD(m_sts))
      break;
  }
  if (EVEN(m_sts)) {
    m_log->log('F', "pwrtest01b timeout", m_sts);
    exit(0);
  }
}

typedef struct {
  pwr_tOName name;
  pwr_tOName nextname;
  pwr_tStatus sts;
} sGetNextSibling;

void ra_nethtest::GetNextSibling(void)
{
  sGetNextSibling d[] = {
    {"Test01b-RootObject-Child1", "Test01b-RootObject-Child2", HASH__SUCCESS},
    {"Test01b-RootObject-Child2", "Test01b-RootObject-Child3", HASH__SUCCESS},
    {"Test01b-RootObject-Child3", "Test01b-RootObject-Child4", HASH__SUCCESS},
    {"Test01b-RootObject-Child4", "Test01b-RootObject-ChildLess", HASH__SUCCESS},
    {"Test01b-RootObject-ChildLess", "", GDH__NO_SIBLING}
  };
  pwr_tOid oid, nextoid, noid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextSibling, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = gdh_GetNextSibling(oid, &nextoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextSibling, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_NameToObjid(d[i].nextname, &noid);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextSibling, gdh_NameToObjid", d[i].name, m_sts);
	return;
      }

      if (cdh_ObjidIsNotEqual(noid, nextoid)) {
	m_log->log('E', "GetNextSibling, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetNextSibling", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName previousname;
  pwr_tStatus sts;
} sGetPreviousSibling;

void ra_nethtest::GetPreviousSibling(void)
{
  sGetPreviousSibling d[] = {
    {"Test01b-RootObject-ChildLess", "Test01b-RootObject-Child4", HASH__SUCCESS},
    {"Test01b-RootObject-Child4", "Test01b-RootObject-Child3", HASH__SUCCESS},
    {"Test01b-RootObject-Child3", "Test01b-RootObject-Child2", HASH__SUCCESS},
    {"Test01b-RootObject-Child2", "Test01b-RootObject-Child1", HASH__SUCCESS},
    {"Test01b-RootObject-Child1", "", GDH__NO_SIBLING}
  };
  pwr_tOid oid, previousoid, poid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetPreviousSibling, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = gdh_GetPreviousSibling(oid, &previousoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetPreviousSibling, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_NameToObjid(d[i].previousname, &poid);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetPreviousSibling, gdh_NameToObjid", d[i].name, m_sts);
	return;
      }

      if (cdh_ObjidIsNotEqual(poid, previousoid)) {
	m_log->log('E', "GetPreviousSibling, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetPreviousSibling", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  pwr_tOName result;
  pwr_tStatus sts;
} sNameToAttrRef;

void ra_nethtest::NameToAttrref(void)
{
  sNameToAttrRef d[] = {
    {"Test01b-RootObject-Child1.Photo", "Test01b-RootObject-Child1.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", "Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", "Test01b-RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", "Test01b-RootObject-Child1-A1.Value[99]", GDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  pwr_tAName aname2;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].aname, &aref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "NameToAttrref, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_AttrrefToName(&aref, aname2, sizeof(aname2), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "NameToAttrref, gdh_AttrrefToName", d[i].aname, m_sts);
	return;
      }
      if (strcmp(aname2, d[i].result) != 0) {
	m_log->vlog('E', "NameToAttrref, %s != %s, idx %d", aname2, d[i].result, i);
	return;
      }
    }
  }

  m_log->log('S', "NameToAttrref", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  int nametype;
  pwr_tOName result;
  pwr_tStatus sts;
} sAttrRefToName;

void ra_nethtest::AttrrefToName(void)
{
  sAttrRefToName d[] = {
    {"Test01b-RootObject-Child1.Photo", cdh_mNName, "Test01b-RootObject-Child1.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1.Photo", cdh_mName_object, "Child1", GDH__SUCCESS},
    {"Test01b-RootObject-Child1.Photo", cdh_mName_attribute, "Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1.Photo", cdh_mName_object | cdh_mName_attribute, "Child1.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1.Photo", cdh_mName_volumeStrict, "VolPwrTest01b:Test01b-RootObject-Child1.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1.Photo", cdh_mName_pathStrict, "Test01b-RootObject-Child1.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mNName, "Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object, "P1", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_attribute, "CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object | cdh_mName_attribute, "P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_volumeStrict, "VolPwrTest01b:Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_pathStrict, "Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mNName, "Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_object, "P1", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_attribute, "CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_volumeStrict, "VolPwrTest01b:Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_pathStrict, "Test01b-RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", cdh_mNName, "Test01b-RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", cdh_mName_object, "A1", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", cdh_mName_attribute, "Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", cdh_mName_volumeStrict, "VolPwrTest01b:Test01b-RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value", cdh_mName_pathStrict, "Test01b-RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mNName, "Test01b-RootObject-Child1-A1.Value[99]", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mName_object, "A1", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mName_attribute, "Value", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mName_object | cdh_mName_attribute | cdh_mName_index, "A1.Value[99]", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mName_volumeStrict, "VolPwrTest01b:Test01b-RootObject-Child1-A1.Value[99]", GDH__SUCCESS},
    {"Test01b-RootObject-Child1-A1.Value[99]", cdh_mName_pathStrict, "Test01b-RootObject-Child1-A1.Value[99]", GDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  pwr_tAName aname2;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrrefToName, gdh_NameToAttrref", m_sts);
      return;
    }

    m_sts = gdh_AttrrefToName(&aref, aname2, sizeof(aname2), d[i].nametype);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttrrefToName, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(aname2, d[i].result) != 0) {
	m_log->vlog('E', "AttrrefToName, %s != %s, idx %d", aname2, d[i].result, i);
	return;
      }
    }
  }

  m_log->log('S', "AttrRefToName", GDH__SUCCESS);
}

typedef struct {
  pwr_tAName aname;
  pwr_tDisableAttr dis;
  pwr_tStatus sts;
} sArefDisabled;

void ra_nethtest::ArefDisabled(void)
{
  sArefDisabled d[] = {
    {"Test01b-Neth-P1.CircuitBreaker", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.CircuitBreaker.NotTripped", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.Motor.TempSwitch", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.Motor.TempSwitch.Switch", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.Motor.TempSensor", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.Motor.TempSensor.Value", 1, HASH__SUCCESS},
    {"Test01b-Neth-P1.Contactor", 0, HASH__SUCCESS},
    {"Test01b-Neth-P1.Contactor.Order", 0, HASH__SUCCESS},
    {"Test01b-Neth-P1", 0, GDH__NOATTR}
  };
  pwr_tAttrRef aref;
  pwr_tDisableAttr dis;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "ArefDisabled", d[i].aname, m_sts);
      return;
    }
    m_sts = gdh_ArefDisabled(&aref, &dis);
    if (m_sts != d[i].sts) {
      m_log->log('E', "ArefDisabled", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (dis != d[i].dis) {
	m_log->log('E', "ArefDisabled", d[i].aname, m_sts);
	return;
      }
    }
  }

  m_log->log('S', "ArefDisabled", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tUInt32 size;
  pwr_tStatus sts;
} sGetObjectSize;

void ra_nethtest::GetObjectSize(void)
{
  sGetObjectSize d[] = {
    {"Test01b-Neth-A1", sizeof(pwr_sClass_AArray100), HASH__SUCCESS},
    {"Test01b-Neth-P1", sizeof(pwr_sClass_BaseFcPPO3PumpAggr), HASH__SUCCESS},
    {"Test01b-Neth-Av1", sizeof(pwr_sClass_Av), HASH__SUCCESS},
    {"Test01b-Neth-Dv1", sizeof(pwr_sClass_Dv), HASH__SUCCESS},
    {"VolPwrTest01b:", sizeof(pwr_sClass_RootVolume), HASH__SUCCESS},
    {"Test01b", sizeof(pwr_sClass_PlantHier), HASH__SUCCESS}
  };
  pwr_tOid oid;
  pwr_tUInt32 size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectSize", "gdh_NameToObjid", m_sts);
      return;
    }

    m_sts = gdh_GetObjectSize(oid, &size);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetObjectSize", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].size != size) {
	m_log->log('E', "GetSuperClass, size doesn't match");
	return;
      }
    }
  }

  m_log->log('S', "GetObjectSize", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetObjectClass;

void ra_nethtest::GetObjectClass(void)
{
  sGetObjectClass d[] = {
    {"Test01b-Neth-A1", pwr_cClass_AArray100, HASH__SUCCESS},
    {"Test01b-Neth-P1", pwr_cClass_BaseFcPPO3PumpAggr, HASH__SUCCESS},
    {"Test01b-Neth-Av1", pwr_cClass_Av, HASH__SUCCESS},
    {"Test01b-Neth-Dv1", pwr_cClass_Dv, HASH__SUCCESS},
    {"VolPwrTest01b:", pwr_cClass_RootVolume, HASH__SUCCESS},
    {"Test01b", pwr_cClass_PlantHier, HASH__SUCCESS}
  };
 
  pwr_tOid oid;
  pwr_tCid cid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectClass, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_GetObjectClass(oid, &cid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetObjectClass", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].cid != cid) {
	m_log->log('E', "GetObjectClass, object class doesn't match");
	return;
      }
    }
  }

  m_log->log('S', "GetObjectClass", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tTid tid;
  pwr_tOName tname;
  pwr_tStatus sts;
} sGetAttrRefTid;

void ra_nethtest::GetAttrRefTid(void)
{
  sGetAttrRefTid d[] = {
    {"Test01b-Neth-A1.Value", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01b-Neth-A1.Value[0]", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01b.Description", 0, "pwrs:Type-$String80", GDH__SUCCESS},
    {"Test01b-Neth-P1.CircuitBreaker.NotTripped.ActualValue", pwr_eType_Boolean, "", GDH__SUCCESS},
    {"Test01b-Neth-Av1.ActualValue", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01b-Neth-Dv1.ActualValue", pwr_eType_Boolean, "", GDH__SUCCESS},
    {"Test01b-Neth-A1", pwr_cClass_AArray100, "", GDH__SUCCESS},
    {"Test01b-Neth-P1", pwr_cClass_BaseFcPPO3PumpAggr, "", GDH__SUCCESS},
    {"Test01b-Neth-Av1", pwr_cClass_Av, "", GDH__SUCCESS},
    {"Test01b-Neth-Dv1", pwr_cClass_Dv, "", GDH__SUCCESS},
    {"VolPwrTest01b:", pwr_cClass_RootVolume, "", GDH__SUCCESS},
    {"Test01b", pwr_cClass_PlantHier, "", GDH__SUCCESS}
  };
 
  pwr_tAttrRef aref;
  pwr_tTid tid, dtid;
  pwr_tOid toid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetAttrRefTid, gdh_NameToAttrref", d[i].name, m_sts);
      return;
    }

    if (d[i].tid == 0) {
      m_sts = gdh_NameToObjid(d[i].tname, &toid);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetAttrRefTid, gdh_NameToObjid", d[i].name, m_sts);
	return;
      }
      dtid = cdh_TypeObjidToId(toid);
    }
    else
      dtid = d[i].tid;

    m_sts = gdh_GetAttrRefTid(&aref, &tid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetAttrRefTid", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (dtid != tid) {
	m_log->log('E', "GetAttrRefTid, tid doesn't match", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetAttrRefTid", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tBoolean location;
  pwr_tStatus sts;
} sGetObjectLocation;

void ra_nethtest::GetObjectLocation(void)
{
  sGetObjectLocation d[] = {
    {"Test01b-Neth-A1", 0, HASH__SUCCESS},
    {"Test01b-Neth", 0, HASH__SUCCESS},
    {"Test01b-Neth-A1", 0, HASH__SUCCESS},
    {"VolPwrTest01b:", 0, HASH__SUCCESS}
  };
 
  pwr_tOid oid;
  pwr_tBoolean location;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectLocation, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_GetObjectLocation(oid, &location);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetObjectLocation", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].location != location) {
	m_log->log('E', "GetObjectLocation, location doesn't match");
	return;
      }
    }
  }

  m_log->log('S', "GetObjectLocation", GDH__SUCCESS);
}

// Destructor
ra_nethtest::~ra_nethtest()
{
  //gdh_DeleteObjectTree(m_dynroot);

  delete m_log;
}

int main()
{
  ra_nethtest neth;

  setenv("TZ", "Europe/Stockholm", 1);
  tzset();

  neth.ObjidToName();
  neth.GetChild();
  neth.GetParent();
  neth.GetNextSibling();
  neth.GetPreviousSibling();
  neth.NameToAttrref();
  neth.AttrrefToName();
  neth.ArefDisabled();
  neth.GetObjectSize();
  neth.GetObjectClass();
  neth.GetAttrRefTid();
  neth.GetObjectLocation();
}
