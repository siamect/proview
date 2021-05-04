/*
  Test gdh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_lck.h"
#include "ra_gdhtest.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rt_mvol_msg.h"
#include "co_cdh_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"


pwr_tStatus ra_gdhtest::check_object(pwr_tOid oid)
{
  pwr_tOid p, child, next, prev;
  pwr_tStatus sts;

  sts = gdh_GetParent(oid, &p);
  if (ODD(sts)) {
    int found = 0;
    for (sts = gdh_GetChild(p, &child); 
	 ODD(sts); 
	 sts = gdh_GetNextSibling(child, &child)) {
      if (cdh_ObjidIsEqual(oid, child)) {
	found = 1;
	break;
      }
    }
    if (!found)
      return GDH__NO_CHILD;
  }

  sts = gdh_GetNextSibling(oid, &next);
  if (ODD(sts)) {
    sts = gdh_GetPreviousSibling(next, &prev);
    if (EVEN(sts))
      return sts;

    if (cdh_ObjidIsNotEqual(oid, prev))
      return GDH__NO_SIBLING;
  }
  
  sts = gdh_GetPreviousSibling(oid, &prev);
  if (ODD(sts)) {
    sts = gdh_GetNextSibling(prev, &next);
    if (EVEN(sts))
      return sts;

    if (cdh_ObjidIsNotEqual(oid, next))
      return GDH__NO_SIBLING;
  }

  for (sts = gdh_GetChild(oid, &child);
       ODD(sts); 
       sts = gdh_GetNextSibling(child, &child)) {
    sts = gdh_GetParent(child, &p);
    if (cdh_ObjidIsNotEqual(oid, p))
      return GDH__NO_CHILD;
  }
  return GDH__SUCCESS;
}


void ra_gdhtest::Init(void)
{
  m_sts = gdh_Init("ra_gdhtest");
  if (EVEN(m_sts)) {
    m_log->log('F', "Init", m_sts);
    exit(0);
  }

  m_log->log('S', "Init", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tStatus sts1;
  pwr_tStatus sts2;
  pwr_tOid oid;
} sObjidToName;

// NameToObjid and ObjidToName
void ra_gdhtest::ObjidToName(void)
{
  sObjidToName d[] = {
    {"Test01c-Gdh", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-ƒ≈÷‰Âˆ", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-LongName90123456789012345678901", HASH__SUCCESS, HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-TooLongName234567890123456789012", CDH__SEGLEN, 0, pwr_cNOid}
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
  pwr_tOName name;
  int offset;
  pwr_tTypeId type;
  int size;
  char valuestr[80];
  pwr_tStatus sts;
  pwr_tOid oid;
} sObjidToPointer;

void ra_gdhtest::ObjidToPointer(void)
{
  sObjidToPointer d[] = {
    {"Test01c-Gdh-Av1", offsetof(pwr_sClass_Av, Description), pwr_eType_String, 80, "Av with value 222", HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-Dv1", offsetof(pwr_sClass_Dv, Description), pwr_eType_String, 80, "Dv with value true", HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-W1", offsetof(pwr_sClass_Pwrt01_WriteClass, Value), pwr_eType_Float32, 4, "9", HASH__SUCCESS, pwr_cNOid}
  };
  char value[80];
  char *p;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &d[i].oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "ObjidToPointer, NameToObjid wrong sts", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_ObjidToPointer(d[i].oid, (void **)&p);
    if (m_sts != d[i].sts) {
      m_log->log('E', "ObjidToPointer, wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_AttrStringToValue(d[i].type, d[i].valuestr, value, sizeof(value), 
				    d[i].size);
      if (EVEN(m_sts)) {
	m_log->log('E', "ObjidToPointer, ", d[i].name, m_sts);
	return;
      }
      p += d[i].offset;
      switch (d[i].type) {
      case pwr_eType_String:
      case pwr_eType_Text:
	if (strcmp(value, p) != 0) {
	  m_log->log('E', "ObjidToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      default:
	if (memcmp(value, p, d[i].size) != 0) {
	  m_log->log('E', "ObjidToPointer, wrong value", d[i].name);
	  return;
	}
      }
    }
  }

  m_log->log('S', "ObjidToPointer", GDH__SUCCESS);
}

void ra_gdhtest::MountObjidToPointer(void)
{
  sObjidToPointer d[] = {
    {"Test01c-Gdh-PlcMountTest", offsetof(pwr_sClass_MountObject, Description), pwr_eType_String, 80, "Mounting test object", HASH__SUCCESS, pwr_cNOid}
  };
  char value[80];
  char *p;
  pwr_tOid oid;
  pwr_tOName mname;

  // Find objid for mount objects
  for (m_sts = gdh_GetClassList(pwr_eClass_MountObject, &oid);
       ODD(m_sts);
       m_sts = gdh_GetNextSibling(oid, &oid)) {
    m_sts = gdh_ObjidToName(oid, mname, sizeof(mname), cdh_mNName);
    if (EVEN(m_sts)) {
      m_log->log('E', "MountObjidToPointer, gdh_ObjidToName", m_sts);
      return;
    }
    for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
      if (strcmp(mname, d[i].name) == 0) {
	d[i].oid = oid;
	break;
      }
    }
  }
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (cdh_ObjidIsNull(d[i].oid)) {
      m_log->log('E', "MountObjidToPointer, mount object not found", d[i].name);
      return;
    }
    m_sts = gdh_MountObjidToPointer(d[i].oid, (void **)&p);
    if (m_sts != d[i].sts) {
      m_log->log('E', "MountObjidToPointer, wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_AttrStringToValue(d[i].type, d[i].valuestr, value, sizeof(value), 
				    d[i].size);
      if (EVEN(m_sts)) {
	m_log->log('E', "MountObjidToPointer, ", d[i].name, m_sts);
	return;
      }
      p += d[i].offset;
      switch (d[i].type) {
      case pwr_eType_String:
      case pwr_eType_Text:
	if (strcmp(value, p) != 0) {
	  m_log->log('E', "MountObjidToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      default:
	if (memcmp(value, p, d[i].size) != 0) {
	  m_log->log('E', "MountObjidToPointer, wrong value", d[i].name);
	  return;
	}
      }
    }
  }

  m_log->log('S', "MountObjidToPointer", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tTypeId type;
  int size;
  char valuestr[80];
  pwr_tStatus sts;
} sAttrRefToPointer;

void ra_gdhtest::AttrRefToPointer(void)
{
  sAttrRefToPointer d[] = {
    {"Test01c-Gdh-Av1.Description", pwr_eType_String, 80, "Av with value 222", HASH__SUCCESS},
    {"Test01c-Gdh-Dv1.Description", pwr_eType_String, 80, "Dv with value true", HASH__SUCCESS},
    {"Test01c-Gdh-W1.Value", pwr_eType_Float32, 4, "9", HASH__SUCCESS}
  };
  pwr_tAttrRef aref;
  char value[80];
  char *p;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrRefToPointer, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_AttrRefToPointer(&aref, (void **)&p);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttrRefToPointer, wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_AttrStringToValue(d[i].type, d[i].valuestr, value, sizeof(value), 
				    d[i].size);
      if (EVEN(m_sts)) {
	m_log->log('E', "AttrRefToPointer, ", d[i].name, m_sts);
	return;
      }
      switch (d[i].type) {
      case pwr_eType_String:
      case pwr_eType_Text:
	if (strcmp(value, p) != 0) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      case pwr_eType_Float32:
	if (fabs(*(pwr_tFloat32 *)value - *(pwr_tFloat32 *)p) > FLT_EPSILON * 100) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      default:
	if (memcmp(value, p, d[i].size) != 0) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
      }
    }
  }

  m_log->log('S', "AttrRefToPointer", GDH__SUCCESS);
}

void ra_gdhtest::NameToPointer(void)
{
  sObjidToPointer d[] = {
    {"Test01c-Gdh-Av1", offsetof(pwr_sClass_Av, Description), pwr_eType_String, 80, "Av with value 222", HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-Dv1", offsetof(pwr_sClass_Dv, Description), pwr_eType_String, 80, "Dv with value true", HASH__SUCCESS, pwr_cNOid},
    {"Test01c-Gdh-W1", offsetof(pwr_sClass_Pwrt01_WriteClass, Value), pwr_eType_Float32, 4, "9", HASH__SUCCESS, pwr_cNOid}
  };
  char value[80];
  char *p;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToPointer(d[i].name, (void **)&p);
    if (m_sts != d[i].sts) {
      m_log->log('E', "NameToPointer, wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_AttrStringToValue(d[i].type, d[i].valuestr, value, sizeof(value), 
				    d[i].size);
      if (EVEN(m_sts)) {
	m_log->log('E', "NameToPointer, ", d[i].name, m_sts);
	return;
      }
      p += d[i].offset;
      switch (d[i].type) {
      case pwr_eType_String:
      case pwr_eType_Text:
	if (strcmp(value, p) != 0) {
	  m_log->log('E', "NameToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      default:
	if (memcmp(value, p, d[i].size) != 0) {
	  m_log->log('E', "NameToPointer, wrong value", d[i].name);
	  return;
	}
      }
    }
  }

  m_log->log('S', "NameToPointer", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid oid;
} sCreateObject;

void ra_gdhtest::CreateObject(void)
{
  sCreateObject d[] = {
    {"pwrNode-Test", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H1", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H2", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H3", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H3-ƒ≈÷‰Âˆ", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H3-LongName90123456789012345678901", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H3-TooLongName234567890123456789012", pwr_cClass_PlantHier, CDH__SEGLEN, pwr_cNOid},
    {"pwrNode-Test-H3-Invalid#Name", pwr_cClass_PlantHier, CDH__INVCHAR, pwr_cNOid},
    {"pwrNode-Test-H3-H4-NoParent", pwr_cClass_PlantHier, GDH__BADPARENT, pwr_cNOid}
  };
  pwr_tOName oname;
  
  // Clear if objects already are created
  m_sts = gdh_NameToObjid(d[0].name, &d[0].oid);
  if (ODD(m_sts))
    m_sts = gdh_DeleteObjectTree(d[0].oid);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_CreateObject(d[i].name, d[i].cid, 0, &d[i].oid,
				pwr_cNOid, 0, pwr_cNOid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "CreateObject", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(d[i].oid, oname, sizeof(oname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "CreateObject gdh_ObjidToName", d[i].name, m_sts);
	return;
      }
      if (strcmp(oname, d[i].name) != 0) {
	m_log->log('E', "CreateObject, wrong object", d[i].name);
	return;
      }
      m_sts = check_object(d[i].oid);
      if (EVEN(m_sts)) {
	m_log->log('E', "CreateObject, check object", d[i].name, m_sts);
	return;
      }
      if (i == 0)
	m_dynroot = d[i].oid;
    }
  }
  m_sts = gdh_DeleteObjectTree(d[3].oid);

  m_log->log('S', "CreateObject", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid oid;
} sDeleteObject;

void ra_gdhtest::DeleteObject(void)
{
  sDeleteObject d[] = {
    {"pwrNode-Test-H2-Del1", pwr_cClass_PlantHier, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H2-Del2", pwr_cClass_AArray100, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H2-Del3", pwr_cClass_Pwrt01_Roller, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H2-Del4", pwr_cClass_Pwrt01_Motor, HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H2-Del5", pwr_cClass_Pwrt01_Component, HASH__SUCCESS, pwr_cNOid}
  };
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_CreateObject(d[i].name, d[i].cid, 0, &d[i].oid,
				pwr_cNOid, 0, pwr_cNOid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "DeleteObject", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_DeleteObject(d[i].oid);
      if (EVEN(m_sts)) {
	m_log->log('E', "DeleteObject", m_sts);
	return;
      }
    }
  }

  // Try to delete an nonexisting object
  m_sts = gdh_DeleteObject(d[0].oid);
  if (ODD(m_sts)) {
    m_log->log('E', "DeleteObject returned noexisting object");
    return;
  }
  else if (m_sts != GDH__NOSUCHOBJ) {
    m_log->log('E', "DeleteObject wrong sts", m_sts);
    return;
  }

  m_log->log('S', "DeleteObject", GDH__SUCCESS);
}

void ra_gdhtest::DeleteObjectTree(void)
{
  pwr_tOName name;
  pwr_tAName cname;
  pwr_tOid oid, coid;

  // First create and then delete an object.
  strcpy(name, "pwrNode-Test-TestObject");
  m_sts = gdh_CreateObject(name, pwr_cClass_AArray100, 0, &oid,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "DeleteObject, error in gdh_CreateObject", m_sts);
    return;
  }
  for (int i = 0; i < 10; i++) {
    sprintf(cname, "%s-ch%d", name, i);
  
    m_sts = gdh_CreateObject(cname, pwr_cClass_AArray100, 0, &coid,
			     pwr_cNOid, 0, pwr_cNOid);
    if (EVEN(m_sts)) {
      m_log->log('E', "DeleteObjectTree, error in gdh_CreateObject", m_sts);
      return;
    }
  }
  m_sts = gdh_DeleteObjectTree(oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "DeleteObject", m_sts);
    return;
  }

  // Try to delete an nonexisting object
  oid.oix = 0xffffffff;
  oid.vid = m_root.vid;
  m_sts = gdh_DeleteObjectTree(oid);
  if (ODD(m_sts)) {
    m_log->log('E', "DeleteObjectTree returned noexisting object");
    return;
  }
  else if (m_sts != GDH__NOSUCHOBJ) {
    m_log->log('E', "DeleteObjectTree wrong sts", m_sts);
    return;
  }

  m_log->log('S', "DeleteObjectTree", GDH__SUCCESS);
}

void ra_gdhtest::RenameObject(void)
{
  pwr_tOName name;
  pwr_tOid oid, oid2;

  // First create and then rename an object.
  strcpy(name, "pwrNode-Test-TestObject");
  m_sts = gdh_CreateObject(name, pwr_cClass_AArray100, 0, &oid,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "RenameObject, error in gdh_CreateObject", m_sts);
    return;
  }
  m_sts = gdh_RenameObject(oid, (char *)"NewName");
  if (EVEN(m_sts)) {
    m_log->log('E', "RenameObject", m_sts);
    return;
  }
  m_sts = gdh_NameToObjid("pwrNode-Test-NewName", &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "RenameObject object not found", m_sts);
    return;
  }

  // Try to rename an nonexisting object
  oid2.oix = 0xffffffff;
  oid2.vid = m_root.vid;
  m_sts = gdh_RenameObject(oid2, (char *)"NewName2");
  if (ODD(m_sts)) {
    m_log->log('E', "RenameObject returned noexisting object");
    return;
  }
  else if (m_sts != HASH__NOTFOUND) {
    m_log->log('E', "RenameObject wrong sts", m_sts);
    return;
  }

  m_sts = gdh_DeleteObject(oid);

  m_log->log('S', "RenameObject", GDH__SUCCESS);
}

void ra_gdhtest::MoveObject(void)
{
  pwr_tOName name;
  pwr_tAName cname;
  pwr_tOid oid, oid2, p1, p2, c[100];

  // First create and then move an object.
  strcpy(name, "pwrNode-Test-H1-TestObject");
  m_sts = gdh_CreateObject(name, pwr_cClass_AArray100, 0, &oid,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in gdh_CreateObject", m_sts);
    return;
  }
  m_sts = gdh_NameToObjid("pwrNode-Test-H2", &oid2);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in gdh_NameToObjid", m_sts);
    return;
  }

  m_sts = gdh_MoveObject(oid, oid2);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject", m_sts);
    return;
  }
  m_sts = gdh_NameToObjid("pwrNode-Test-H2-TestObject", &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject object not found", m_sts);
    return;
  }
  m_sts = check_object(oid);

  // Try to move an nonexisting object
  oid2.oix = 0xffffffff;
  oid2.vid = m_dynroot.vid;
  m_sts = gdh_MoveObject(oid, oid2);
  if (ODD(m_sts)) {
    m_log->log('E', "MoveObject moved noexisting object");
    return;
  }
  else if (m_sts != GDH__NOSUCHOBJ) {
    m_log->log('E', "MoveObject wrong sts", m_sts);
    return;
  }

  m_sts = gdh_DeleteObject(oid);

  // Move several objects 
  strcpy(name, "pwrNode-Test-H1-P1");
  m_sts = gdh_CreateObject(name, pwr_cClass_PlantHier, 0, &p1,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in gdh_CreateObject", m_sts);
    return;
  }
  strcpy(name, "pwrNode-Test-H1-P2");
  m_sts = gdh_CreateObject(name, pwr_cClass_PlantHier, 0, &p2,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in gdh_CreateObject", m_sts);
    return;
  }
  for (int i = 0; i < 100; i++) {
    strcpy(name, "pwrNode-Test-H1-P1");
    sprintf(cname, "%s-c%d", name, i);
    m_sts = gdh_CreateObject(cname, pwr_cClass_AArray100, 0, &c[i],
			     pwr_cNOid, 0, pwr_cNOid);
    if (EVEN(m_sts)) {
      m_log->log('E', "MoveObject, error in gdh_CreateObject", m_sts);
      return;
    }
  }
  for (int i = 0; i < 100; i++) {
    m_sts = gdh_MoveObject(c[i], p2);
    if (EVEN(m_sts)) {
      m_log->log('E', "MoveObject, error in gdh_MoveObject", m_sts);
      return;
    }
  }
  m_sts = check_object(p1);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in p1", m_sts);
    return;
  }
  m_sts = check_object(p2);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in p2", m_sts);
    return;
  }
  for (int i = 0; i < 100; i++) {
    m_sts = check_object(c[i]);
    if (EVEN(m_sts)) {
      m_log->log('E', "MoveObject, error in c", m_sts);
      return;
    }
  }
  
  m_sts = gdh_DeleteObjectTree(p1);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in DeleteObjectTree(p1)", m_sts);
    return;
  }
  m_sts = gdh_DeleteObjectTree(p2);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in DeleteObjectTree(p2)", m_sts);
    return;
  }
  
  m_log->log('S', "MoveObject", GDH__SUCCESS);
}

void ra_gdhtest::GetRootList(void)
{
  pwr_tOName name;

  m_sts = gdh_GetRootList(&m_root);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetRootList", m_sts);
    return;
  }
  m_sts = gdh_ObjidToName(m_root, name, sizeof(name), cdh_mNName);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetRootList", "ObjidToName failed", m_sts);
    return;
  }
  if (strcmp(name, "RootObject") != 0) {
    m_log->log('E', "GetRootList", "Wrong root object");
    return;
  }
  m_log->log('S', "GetRootList", GDH__SUCCESS);
}


typedef struct {
  pwr_tOName pname;
  pwr_tObjName cname;
  pwr_tCid cid;
  pwr_tStatus sts;
  int create;
  pwr_tOid poid;
  pwr_tOid coid;
} sGetChild;

void ra_gdhtest::GetChild(void)
{
  sGetChild d[] = {
    {"RootObject", "Child1", pwr_cClass_PlantHier, HASH__SUCCESS, 0, pwr_cNOid, pwr_cNOid},
    {"RootObject-ChildLess", "", pwr_cClass_PlantHier, GDH__NO_CHILD, 0, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest", "Ch1", pwr_cClass_PlantHier, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1", "Ch2", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2", "Ch3", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3", "Ch4", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4", "Ch5", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5", "Ch6", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6", "Ch7", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7", "Ch8", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8", "Ch9", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9", "Ch10", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10", "Ch11", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10-Ch11", "Ch12", pwr_cClass_AArray100, HASH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"pwrNode-ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10-Ch11-Ch12", "", pwr_cClass_AArray100, GDH__NO_CHILD, 0, pwr_cNOid, pwr_cNOid}
  };
  pwr_tAName cname;
  pwr_tOid coid, poid;

  m_sts = gdh_CreateObject((char *)"pwrNode-ChTest", pwr_cClass_PlantHier, 0, &poid,
			   pwr_cNOid, 0, pwr_cNOid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetChild, gdh_CreateObject ChTest", m_sts);
    return;
  }
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].pname, &d[i].poid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, gdh_NameToObjid", d[i].pname, m_sts);
      return;
    }
    sprintf(cname, "%s-%s", d[i].pname, d[i].cname);
    if (d[i].create) {
      m_sts = gdh_CreateObject(cname, d[i].cid, 0, &d[i].coid,
			       pwr_cNOid, 0, pwr_cNOid);
      if (m_sts != d[i].sts) {
	m_log->log('E', "GetChild, gdh_CreateObject", d[i].cname, m_sts);
	return;
      }
    }
    else {
      m_sts = gdh_NameToObjid(cname, &d[i].coid);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetChild, gdh_NameToObjid", d[i].cname, m_sts);
	return;
      }
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

  // Enter nonexisting parent
  coid.oix = 0xffffffff;
  coid.vid = m_root.vid;
  m_sts = gdh_GetChild(coid, &coid);
  if (ODD(m_sts)) {
    m_log->log('E', "GetChild returned noexisting object");
    return;
  }
  else if (m_sts != GDH__NOTMOUNTED) {
    m_log->log('E', "GetChild wrong sts", m_sts);
    return;
  }

  m_child = d[0].coid;
  m_sts = gdh_DeleteObjectTree(poid);

  m_log->log('S', "GetChild", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tStatus sts;
} sGetParent;

void ra_gdhtest::GetParent(void)
{
  sGetParent d[] = {
    {"RootObject", GDH__NO_PARENT},
    {"RootObject-Child1", HASH__SUCCESS},
    {"RootObject-ChildLess", HASH__SUCCESS}
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

  // Enter nonexisting parent
  poid.oix = 0xffffffff;
  poid.vid = m_root.vid;
  m_sts = gdh_GetParent(poid, &poid);
  if (ODD(m_sts)) {
    m_log->log('E', "GetParent returned noexisting object");
    return;
  }
  else if (m_sts != GDH__NOTMOUNTED) {
    m_log->log('E', "GetParent wrong sts", m_sts);
    return;
  }

  m_log->log('S', "GetParent", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName nextname;
  pwr_tStatus sts;
} sGetNextSibling;

void ra_gdhtest::GetNextSibling(void)
{
  sGetNextSibling d[] = {
    {"RootObject-Child1", "RootObject-Child2", HASH__SUCCESS},
    {"RootObject-Child2", "RootObject-Child3", HASH__SUCCESS},
    {"RootObject-Child3", "RootObject-Child4", HASH__SUCCESS},
    {"RootObject-Child4", "RootObject-ChildLess", HASH__SUCCESS},
    {"RootObject-ChildLess", "", GDH__NO_SIBLING}
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

void ra_gdhtest::GetPreviousSibling(void)
{
  sGetPreviousSibling d[] = {
    {"RootObject-ChildLess", "RootObject-Child4", HASH__SUCCESS},
    {"RootObject-Child4", "RootObject-Child3", HASH__SUCCESS},
    {"RootObject-Child3", "RootObject-Child2", HASH__SUCCESS},
    {"RootObject-Child2", "RootObject-Child1", HASH__SUCCESS},
    {"RootObject-Child1", "", GDH__NO_SIBLING}
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


void ra_gdhtest::GetNodeIndex(void)
{
  pwr_tNid nid;

  m_sts = gdh_GetNodeIndex(&nid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetNodeIndex", m_sts);
    return;
  }
  if (nid != m_root.vid) {
    m_log->log('E', "GetNodeIndex erroneus idx");
    return;
  }

  m_log->log('S', "GetNodeIndex", GDH__SUCCESS);
}

void ra_gdhtest::GetObjectNodeIndex(void)
{
  pwr_tNid nid;

  m_sts = gdh_GetObjectNodeIndex(m_root, &nid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetObjectNodeIndex", m_sts);
    return;
  }
  if (nid != m_root.vid) {
    m_log->log('E', "GetObjectNodeIndex erroneus idx");
    return;
  }

  m_log->log('S', "GetObjectNodeIndex", GDH__SUCCESS);
}

void ra_gdhtest::GetNodeObject(void)
{
  pwr_tOid oid, oid2;

  m_sts = gdh_GetNodeObject(pwr_cNNid, &oid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetNodeObject", m_sts);
    return;
  }
  m_sts = gdh_NameToObjid("Nodes-PwrTest01c", &oid2);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetNodeObject, can't find node object", m_sts);
    return;
  }
  if (cdh_ObjidIsNotEqual(oid, oid2)) {
    m_log->log('E', "GetNodeObject erroneus objid");
    return;
  }

  m_log->log('S', "GetNodeObject", GDH__SUCCESS);
}

void ra_gdhtest::GetAttributeCharacteristics(void)
{
  pwr_tAName aname;
  pwr_tTypeId a_tid;
  pwr_tUInt32 a_size, a_offs, a_elem;

  // Get ordinary attribute
  sprintf(aname, "RootObject-Child1.Photo");
  m_sts = gdh_GetAttributeCharacteristics(aname, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharacteristics", m_sts);
    return;
  }

  if (a_tid != pwr_eType_String) {
    m_log->log('E', "GetAttributeCharacteristics type error");
    return;
  }
  if (a_size != sizeof(pwr_tURL)) {
    m_log->log('E', "GetAttributeCharacteristics size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_PlantHier, Photo)) {
    m_log->log('E', "GetAttributeCharacteristics offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharacteristics element error");
    return;
  }

  // Get object attribute
  sprintf(aname, "RootObject-Child1-P1.CircuitBreaker.NotTripped");
  m_sts = gdh_GetAttributeCharacteristics(aname, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharacteristics", m_sts);
    return;
  }

  if (a_tid != pwr_cClass_Di) {
    m_log->log('E', "GetAttributeCharacteristics type error");
    return;
  }
  if (a_size != sizeof(pwr_sClass_Di)) {
    m_log->log('E', "GetAttributeCharacteristics size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_BaseFcPPO3MotorAggr, CircuitBreaker.NotTripped)) {
    m_log->log('E', "GetAttributeCharacteristics offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharacteristics element error");
    return;
  }

  // Get ordinary attribute in objectattr
  sprintf(aname, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo");
  m_sts = gdh_GetAttributeCharacteristics(aname, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharacteristics", m_sts);
    return;
  }

  if (a_tid != pwr_eType_String) {
    m_log->log('E', "GetAttributeCharacteristics type error");
    return;
  }
  if (a_size != sizeof(pwr_tURL)) {
    m_log->log('E', "GetAttributeCharacteristics size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_BaseFcPPO3MotorAggr, CircuitBreaker.NotTripped.Photo)) {
    m_log->log('E', "GetAttributeCharacteristics offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharacteristics element error");
    return;
  }

  // Get array attribute
  sprintf(aname, "RootObject-Child1-A1.Value");
  m_sts = gdh_GetAttributeCharacteristics(aname, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharacteristics", m_sts);
    return;
  }
  if (a_tid != pwr_eType_Float32) {
    m_log->log('E', "GetAttributeCharacteristics type error");
    return;
  }
  if (a_size != 100 * sizeof(pwr_tFloat32)) {
    m_log->log('E', "GetAttributeCharacteristics size error");
    return;
  }
  if (a_offs != 0) {
    m_log->log('E', "GetAttributeCharacteristics offset error");
    return;
  }
  if (a_elem != 100) {
    m_log->log('E', "GetAttributeCharacteristics element error");
    return;
  }

  // Get array element attributes
  for (int i = 0; i < 100; i++) {
    sprintf(aname, "RootObject-Child1-A1.Value[%d]", i);
    m_sts = gdh_GetAttributeCharacteristics(aname, &a_tid, &a_size, &a_offs, &a_elem);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetAttributeCharacteristics", m_sts);
      return;
    }
    if (a_tid != pwr_eType_Float32) {
      m_log->log('E', "GetAttributeCharacteristics type error");
      return;
    }
    if (a_size != sizeof(pwr_tFloat32)) {
      m_log->log('E', "GetAttributeCharacteristics size error");
      return;
    }
    if (a_offs != i * sizeof(pwr_tFloat32)) {
      m_log->log('E', "GetAttributeCharacteristics offset error");
      return;
    }
    if (a_elem != 1) {
      m_log->log('E', "GetAttributeCharacteristics element error");
      return;
    }
  }
  m_log->log('S', "GetAttributeCharacteristics", GDH__SUCCESS);
}

void ra_gdhtest::GetAttributeCharAttrref(void)
{
  pwr_tAttrRef aref;
  pwr_tAName aname;
  pwr_tTypeId a_tid;
  pwr_tUInt32 a_size, a_offs, a_elem;

  // Get ordinary attribute
  sprintf(aname, "RootObject-Child1.Photo");
  m_sts = gdh_NameToAttrref(pwr_cNOid, aname, &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeChar, can't get aref", m_sts);
    return;
  }
  m_sts = gdh_GetAttributeCharAttrref(&aref, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharAttrref", m_sts);
    return;
  }

  if (a_tid != pwr_eType_String) {
    m_log->log('E', "GetAttributeCharAttrref type error");
    return;
  }
  if (a_size != sizeof(pwr_tURL)) {
    m_log->log('E', "GetAttributeCharAttrref size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_PlantHier, Photo)) {
    m_log->log('E', "GetAttributeCharAttrref offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharAttrref element error");
    return;
  }

  // Get object attribute
  sprintf(aname, "RootObject-Child1-P1.CircuitBreaker.NotTripped");
  m_sts = gdh_NameToAttrref(pwr_cNOid, aname, &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeChar, can't get aref", m_sts);
    return;
  }
  m_sts = gdh_GetAttributeCharAttrref(&aref, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharAttrref", m_sts);
    return;
  }

  if (a_tid != pwr_cClass_Di) {
    m_log->log('E', "GetAttributeCharAttrref type error");
    return;
  }
  if (a_size != sizeof(pwr_sClass_Di)) {
    m_log->log('E', "GetAttributeCharAttrref size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_BaseFcPPO3MotorAggr, CircuitBreaker.NotTripped)) {
    m_log->log('E', "GetAttributeCharAttrref offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharAttrref element error");
    return;
  }

  // Get ordinary attribute in objectattr
  sprintf(aname, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo");
  m_sts = gdh_NameToAttrref(pwr_cNOid, aname, &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeChar, can't get aref", m_sts);
    return;
  }
  m_sts = gdh_GetAttributeCharAttrref(&aref, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharAttrref", m_sts);
    return;
  }

  if (a_tid != pwr_eType_String) {
    m_log->log('E', "GetAttributeCharAttrref type error");
    return;
  }
  if (a_size != sizeof(pwr_tURL)) {
    m_log->log('E', "GetAttributeCharAttrref size error");
    return;
  }
  if (a_offs != offsetof(pwr_sClass_BaseFcPPO3MotorAggr, CircuitBreaker.NotTripped.Photo)) {
    m_log->log('E', "GetAttributeCharAttrref offset error");
    return;
  }
  if (a_elem != 1) {
    m_log->log('E', "GetAttributeCharAttrref element error");
    return;
  }

  // Get array attribute
  sprintf(aname, "RootObject-Child1-A1.Value");
  m_sts = gdh_NameToAttrref(pwr_cNOid, aname, &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeChar, can't get aref", m_sts);
    return;
  }
  m_sts = gdh_GetAttributeCharAttrref(&aref, &a_tid, &a_size, &a_offs, &a_elem);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetAttributeCharAttrref", m_sts);
    return;
  }
  if (a_tid != pwr_eType_Float32) {
    m_log->log('E', "GetAttributeCharAttrref type error");
    return;
  }
  if (a_size != 100 * sizeof(pwr_tFloat32)) {
    m_log->log('E', "GetAttributeCharAttrref size error");
    return;
  }
  if (a_offs != 0) {
    m_log->log('E', "GetAttributeCharAttrref offset error");
    return;
  }
  if (a_elem != 100) {
    m_log->log('E', "GetAttributeCharAttrref element error");
    return;
  }

  // Get array element attributes
  for (int i = 0; i < 100; i++) {
    sprintf(aname, "RootObject-Child1-A1.Value[%d]", i);
    m_sts = gdh_NameToAttrref(pwr_cNOid, aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetAttributeChar, can't get aref", m_sts);
      return;
    }
    m_sts = gdh_GetAttributeCharAttrref(&aref, &a_tid, &a_size, &a_offs, &a_elem);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetAttributeCharAttrref", m_sts);
      return;
    }
    if (a_tid != pwr_eType_Float32) {
      m_log->log('E', "GetAttributeCharAttrref type error");
      return;
    }
    if (a_size != sizeof(pwr_tFloat32)) {
      m_log->log('E', "GetAttributeCharAttrref size error");
      return;
    }
    if (a_offs != i * sizeof(pwr_tFloat32)) {
      m_log->log('E', "GetAttributeCharAttrref offset error");
      return;
    }
    if (a_elem != 1) {
      m_log->log('E', "GetAttributeCharAttrref element error");
      return;
    }
  }
  m_log->log('S', "GetAttributeCharAttrref", GDH__SUCCESS);
}


typedef struct {
  pwr_tOName aname;
  pwr_tOName result;
  pwr_tStatus sts;
} sNameToAttrRef;

void ra_gdhtest::NameToAttrref(void)
{
  sNameToAttrRef d[] = {
    {"RootObject-Child1.Photo", "RootObject-Child1.Photo", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", "RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", "RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", "RootObject-Child1-A1.Value[99]", GDH__SUCCESS}
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

void ra_gdhtest::AttrrefToName(void)
{
  sAttrRefToName d[] = {
    {"RootObject-Child1.Photo", cdh_mNName, "RootObject-Child1.Photo", GDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_object, "Child1", GDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_attribute, "Photo", GDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_object | cdh_mName_attribute, "Child1.Photo", GDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_volumeStrict, "VolPwrTest01c:RootObject-Child1.Photo", GDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_pathStrict, "RootObject-Child1.Photo", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mNName, "RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object, "P1", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_attribute, "CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object | cdh_mName_attribute, "P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_volumeStrict, "VolPwrTest01c:RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_pathStrict, "RootObject-Child1-P1.CircuitBreaker.NotTripped", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mNName, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_object, "P1", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_attribute, "CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_volumeStrict, "VolPwrTest01c:RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_pathStrict, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mNName, "RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_object, "A1", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_attribute, "Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_volumeStrict, "VolPwrTest01c:RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_pathStrict, "RootObject-Child1-A1.Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mNName, "RootObject-Child1-A1.Value[99]", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_object, "A1", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_attribute, "Value", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_object | cdh_mName_attribute | cdh_mName_index, "A1.Value[99]", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_volumeStrict, "VolPwrTest01c:RootObject-Child1-A1.Value[99]", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_pathStrict, "RootObject-Child1-A1.Value[99]", GDH__SUCCESS}
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

void ra_gdhtest::ClassAttrToAttrref(void)
{
  pwr_tAttrRef aref;
  pwr_tAName aname;

  m_sts = gdh_ClassAttrToAttrref(pwr_cClass_PlantHier, ".Photo", &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ClassAttrToAttrref", m_sts);
    return;
  }
  
  aref.Objid = m_root;
  m_sts = gdh_AttrrefToName(&aref, aname, sizeof(aname), cdh_mName_pathStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "ClassAttrToAttrref", m_sts);
    return;
  }
  if (strcmp(aname, "RootObject.Photo") != 0) {
    m_log->log('E', "ClassAttrToAttrref wrong name", aname);
    return;
  }

  // Test without point
  m_sts = gdh_ClassAttrToAttrref(pwr_cClass_PlantHier, "Photo", &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ClassAttrToAttrref", m_sts);
    return;
  }
  aref.Objid = m_root;
  m_sts = gdh_AttrrefToName(&aref, aname, sizeof(aname), cdh_mName_pathStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "ClassAttrToAttrref", m_sts);
    return;
  }
  if (strcmp(aname, "RootObject.Photo") != 0) {
    m_log->log('E', "ClassAttrToAttrref wrong name", aname);
    return;
  }
    
  // Test erroneous attribute
  m_sts = gdh_ClassAttrToAttrref(pwr_cClass_PlantHier, ".NoName", &aref);
  if (ODD(m_sts)) {
    m_log->log('E', "ClassAttrToAttrref erroneous ODD status", m_sts);
    return;
  }

  m_log->log('S', "ClassAttrToAttrref", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  pwr_tOName oname;
  pwr_tStatus sts;
} sAttrArefToObjectAref;

void ra_gdhtest::AttrArefToObjectAref(void)
{
  sAttrArefToObjectAref d[] = {
    {"RootObject-Child1.Photo", "RootObject-Child1", GDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", "RootObject-Child1-A1", GDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", "RootObject-Child1-P1.CircuitBreaker", GDH__SUCCESS}
  };


  pwr_tAttrRef aref, oaref;
  pwr_tAName aname;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrArefToObjectAref", "gdh_NameToAttrref", m_sts);
      return;
    }

    m_sts = gdh_AttrArefToObjectAref(&aref, &oaref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttrArefToObjetAref, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_AttrrefToName(&oaref, aname, sizeof(aname), cdh_mName_pathStrict);
      if (EVEN(m_sts)) {
	m_log->log('E', "AttrArefToObjectAref", m_sts);
	return;
      }
      if (strcmp(aname, d[i].oname) != 0) {
	m_log->vlog('E', "AttrArefToObjectAref wrong name, %s != %s", aname, d[i].oname);
	return;
      }
    }
  }

  m_log->log('S', "AttrArefToObjectAref", GDH__SUCCESS);
}

void ra_gdhtest::ArefANameToAref(void)
{
  pwr_tAttrRef aref, oaref;
  pwr_tAName aname;

  // Test ordinary attribute
  m_sts = gdh_NameToAttrref(pwr_cNOid, "RootObject-Child1", &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", "gdh_NameToAttrref", m_sts);
    return;
  }

  m_sts = gdh_ArefANameToAref(&aref, "Photo", &oaref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  
  m_sts = gdh_AttrrefToName(&oaref, aname, sizeof(aname), cdh_mName_pathStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  if (strcmp(aname, "RootObject-Child1.Photo") != 0) {
    m_log->log('E', "ArefANameToAref wrong name", aname);
    return;
  }

  // Test attrobject attribute
  m_sts = gdh_NameToAttrref(pwr_cNOid, "RootObject-Child1-P1.CircuitBreaker", &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", "gdh_NameToAttrref", m_sts);
    return;
  }

  m_sts = gdh_ArefANameToAref(&aref, "NotTripped", &oaref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  
  m_sts = gdh_AttrrefToName(&oaref, aname, sizeof(aname), cdh_mName_pathStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  if (strcmp(aname, "RootObject-Child1-P1.CircuitBreaker.NotTripped") != 0) {
    m_log->log('E', "ArefANameToAref wrong name", aname);
    return;
  }

  // Test array element
  m_sts = gdh_NameToAttrref(pwr_cNOid, "RootObject-Child1-A1", &aref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", "gdh_NameToAttrref", m_sts);
    return;
  }

  m_sts = gdh_ArefANameToAref(&aref, "Value[99]", &oaref);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  
  m_sts = gdh_AttrrefToName(&oaref, aname, sizeof(aname), cdh_mName_pathStrict);
  if (EVEN(m_sts)) {
    m_log->log('E', "ArefANameToAref", m_sts);
    return;
  }
  if (strcmp(aname, "RootObject-Child1-A1.Value[99]") != 0) {
    m_log->log('E', "ArefANameToAref wrong name", aname);
    return;
  }

  m_log->log('S', "ArefANameToAref", GDH__SUCCESS);
}

typedef union {
  pwr_tBoolean b;
  pwr_tInt32 i8;
  pwr_tUInt32 ui8;
  pwr_tInt32 i16;
  pwr_tUInt32 ui16;
  pwr_tInt32 i32;
  pwr_tUInt32 ui32;
  pwr_tInt32 i64;
  pwr_tUInt32 ui64;
  pwr_tFloat32 f32;
  pwr_tFloat64 f64;
  char s40[40];
  pwr_tTime t;
  pwr_tDeltaTime dt;
  pwr_tOid oid;
  pwr_tAttrRef aref;
} uValueToString;

typedef struct {
  pwr_eType	type;
  uValueToString value;
  char		format[20];
  char		str[40];
} sValueToString;

void ra_gdhtest::AttrValueToString(void)
{
  int len;
  char str[100];
  sValueToString d[] = {
    {pwr_eType_Boolean,	{.b=1}, 	"",	"1"},
    {pwr_eType_Char,	{.i8=65}, 	"",	"A"},
    {pwr_eType_Int8,	{.i8=120}, 	"",	"120"},
    {pwr_eType_Int8,	{.i8=120}, 	"%04d",	"0120"},
    {pwr_eType_UInt8,	{.i8=222}, 	"",	"222"},
    {pwr_eType_UInt8,	{.i8=222}, 	"%04u",	"0222"},
    {pwr_eType_Int16,	{.i16=222}, 	"",	"222"},
    {pwr_eType_Int16,	{.i16=222}, 	"%04hd", "0222"},
    {pwr_eType_UInt16,	{.i16=222}, 	"",	"222"},
    {pwr_eType_UInt16,	{.i16=222}, 	"%04hu", "0222"},
    {pwr_eType_Int32,	{.i32=222}, 	"",	"222"},
    {pwr_eType_Int32,	{.i32=222}, 	"%04d",	"0222"},
    {pwr_eType_UInt32,	{.i32=222}, 	"",	"222"},
    {pwr_eType_UInt32,	{.i32=222}, 	"%04u",	"0222"},
    {pwr_eType_Int64,	{.i64=222}, 	"",	"222"},
    {pwr_eType_UInt64,	{.i64=222}, 	"",	"222"},
    {pwr_eType_Mask,	{.i32=0xffff}, 	"",	"65535"},
    {pwr_eType_Mask,	{.i32=0xffff}, 	"%08d",	"00065535"},
    {pwr_eType_Enum,	{.i32=222}, 	"",	"222"},
    {pwr_eType_Enum,	{.i32=222}, 	"%04d",	"0222"},
    {pwr_eType_Float32,	{.f32=222.22}, 	"",	"222.22"},
    {pwr_eType_Float32,	{.f32=1.2345}, 	"%4.2f", "1.23"},
    {pwr_eType_Float64,	{.f64=666.66}, 	"",	"666.66"},
    {pwr_eType_Float64,	{.f64=444.44}, 	"%3.0F", "444"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "", "15-APR-2020 15:19:49.50"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%1t", "15:19:49"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%2t", "15:19:49.50"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%3t", "20-04-15 15:19:49"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%4t", "15-APR-2020"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%5t", "20-04-15"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "%6t", "15:19:49 15/04/20"},
    {pwr_eType_DeltaTime,{.dt={54223,500000000}}, "", "15:03:43.50"},
    {pwr_eType_Objid,	{.oid={2147483649,33488587}}, "", "pwrNode"},
    {pwr_eType_Objid,	{.oid={0,33488587}}, "", "1_254_254_203:"},
    {pwr_eType_AttrRef,	{.aref={{2147483649,33488587},0,0,0,0}}, "", "pwrNode"},
    {pwr_eType_ClassId,	{.i32=pwr_cClass_and}, "",	"Class-And"},
    {pwr_eType_String,	{.s40={'S','o','m','e',' ','s','t','r','i','n','g',0,}}, "",	"Some string"},
    {pwr_eType_Text,	{.s40={'S','o','m','e',' ','t','e','x','t',0,}}, "",	"Some text"}
  };
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_AttrValueToString(d[i].type, 0, &d[i].value, str, sizeof(str),
				  &len, 
				  strcmp(d[i].format, "") ? d[i].format : NULL);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrValueToString", d[i].str, m_sts);
      return;
    }
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "AttrValueToString", str);
      return;
    }
  }

  m_log->log('S', "AttrValueToString", GDH__SUCCESS);
}

typedef struct {
  pwr_eType	type;
  uValueToString value;
  int 		size;
  char		str[40];
} sStringToValue;

void ra_gdhtest::AttrStringToValue(void)
{
  char value[100];
  sStringToValue d[] = {
    {pwr_eType_Boolean,	{.b=1}, 	sizeof(pwr_tBoolean),	"1"},
    {pwr_eType_Char,	{.i8=65}, 	sizeof(pwr_tChar),	"A"},
    {pwr_eType_Int8,	{.i8=120}, 	sizeof(pwr_tInt8),	"120"},
    {pwr_eType_UInt8,	{.i8=222}, 	sizeof(pwr_tUInt8),	"222"},
    {pwr_eType_Int16,	{.i16=222}, 	sizeof(pwr_tInt16),	"222"},
    {pwr_eType_UInt16,	{.i16=222}, 	sizeof(pwr_tUInt16),	"222"},
    {pwr_eType_Int32,	{.i32=222}, 	sizeof(pwr_tInt32),	"222"},
    {pwr_eType_UInt32,	{.i32=222}, 	sizeof(pwr_tUInt32),	"222"},
    {pwr_eType_Int64,	{.i64=222}, 	sizeof(pwr_tInt64),	"222"},
    {pwr_eType_UInt64,	{.i64=222}, 	sizeof(pwr_tUInt64),	"222"},
    {pwr_eType_Mask,	{.i32=0xffff}, 	sizeof(pwr_tMask),	"65535"},
    {pwr_eType_Enum,	{.i32=222}, 	sizeof(pwr_tEnum),	"222"},
    {pwr_eType_Float32,	{.f32=222.22}, 	sizeof(pwr_tFloat32),	"222.22"},
    {pwr_eType_Float64,	{.f64=666.66}, 	sizeof(pwr_tFloat64),	"666.66"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, sizeof(pwr_tTime), "15-APR-2020 15:19:49.50"},
    {pwr_eType_DeltaTime,{.dt={54223,500000000}}, sizeof(pwr_tDeltaTime), "15:03:43.50"},
    {pwr_eType_Objid,	{.oid={2147483649,33488587}}, sizeof(pwr_tObjid), "pwrNode"},
    {pwr_eType_Objid,	{.oid={0,33488587}}, sizeof(pwr_tObjid), "1_254_254_203:"},
    {pwr_eType_AttrRef,	{.aref={{2147483649,33488587},0,0,0,0}}, sizeof(pwr_tAttrRef), "pwrNode"},
    {pwr_eType_ClassId,	{.i32=pwr_cClass_and}, sizeof(pwr_tClassId),	"pwrb:Class-And"},
    {pwr_eType_String,	{.s40={'S','o','m','e',' ','s','t','r','i','n','g',0,}}, 40,	"Some string"},
    {pwr_eType_Text,	{.s40={'S','o','m','e',' ','t','e','x','t',0,}}, 40,	"Some text"}
  };
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_AttrStringToValue(d[i].type, d[i].str, value, sizeof(value), 
				  d[i].size);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrStringToValue", d[i].str, m_sts);
      return;
    }
    switch (d[i].type) {
    case pwr_eType_Float32:
      if (fabs(d[i].value.f32 - *(pwr_tFloat32 *)&value) > 200*FLT_EPSILON) {
	m_log->log('E', "AttrStringToValue", d[i].str);
	return;
      }
      break;
    case pwr_eType_Float64:
      if (fabs(d[i].value.f64 - *(pwr_tFloat64 *)&value) > 600*FLT_EPSILON) {
	m_log->log('E', "AttrStringToValue", d[i].str);
	return;
      }
      break;
    case pwr_eType_AttrRef:
      // Only test objid
      if (memcmp(&d[i].value, value, sizeof(pwr_tOid)) != 0) {
	m_log->log('E', "AttrStringToValue", d[i].str);
	return;
      }
      break;
    default:
      if (memcmp(&d[i].value, value, d[i].size) != 0) {
	m_log->log('E', "AttrStringToValue", d[i].str);
	return;
      }
    }
  }

  m_log->log('S', "AttrStringToValue", GDH__SUCCESS);
}

typedef struct {
  pwr_tAName aname;
  pwr_tDisableAttr dis;
  pwr_tStatus sts;
} sArefDisabled;

void ra_gdhtest::ArefDisabled(void)
{
  sArefDisabled d[] = {
    {"Test01c-Gdh-P1.CircuitBreaker", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.CircuitBreaker.NotTripped", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Motor.TempSwitch", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Motor.TempSwitch.Switch", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Motor.TempSensor", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Motor.TempSensor.Value", 1, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Contactor", 0, GDH__SUCCESS},
    {"Test01c-Gdh-P1.Contactor.Order", 0, GDH__SUCCESS},
    {"Test01c-Gdh-P1", 0, GDH__NOATTR}
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
  pwr_tCid cid;
  pwr_tOName name;
  pwr_tStatus sts;
} sGetClassList;

void ra_gdhtest::GetClassList(void)
{
  sGetClassList d[] = {
    {pwr_cClass_Node, "Nodes-PwrTest01c", HASH__SUCCESS},
    {pwr_cClass_Security, "Nodes-PwrTest01c-Security", HASH__SUCCESS},
    {pwr_cClass_OpPlace, "Nodes-PwrTest01c-OpPlaces-Op", HASH__SUCCESS},
    {pwr_cClass_plc, "Test01c-First-Plc", HASH__SUCCESS},
    {pwr_cClass_and, "Test01c-First-Plc-W-And0", HASH__SUCCESS},
    {pwr_cClass_Dv, "Test01c-First-Dv1", HASH__SUCCESS},
    {pwr_cClass_Component, "", GDH__NO_TYPE}
  };
  pwr_tOid oid;
  pwr_tOName oname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_GetClassList(d[i].cid, &oid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetClassList", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(oid, oname, sizeof(oname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetClassList gdh_ObjidToName", d[i].name, m_sts);
	return;
      }
      if (strcmp(oname, d[i].name) != 0) {
	m_log->log('E', "GetClassList, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetClassList", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName next;
  pwr_tStatus sts;
} sGetNextObject;

void ra_gdhtest::GetNextObject(void)
{
  sGetNextObject d[] = {
    {"Test01c-First-Dv1", "Test01c-First-Dv2", HASH__SUCCESS},
    {"Test01c-First-Dv2", "Test01c-First-Dv3", HASH__SUCCESS},
    {"Test01c-First-Dv3", "Test01c-First-Dv4", HASH__SUCCESS},
    {"Test01c-First-Aggr1", "Test01c-First-Aggr2", HASH__SUCCESS},
    {"Test01c-First-Aggr2", "Test01c-First-Aggr3", HASH__SUCCESS},
    {"Test01c-First-Aggr3", "Test01c-First-Aggr4", HASH__SUCCESS},
    {"Test01c-First-Aggr4", "", GDH__NO_TYPE},
    {"Test01c-First-Plc-W-And0", "Test01c-First-Plc-W-And1", HASH__SUCCESS},
    {"Test01c-First-Plc-W-And1", "Test01c-First-Plc-W-And2", HASH__SUCCESS},
    {"Test01c-First-Plc-W-And2", "Test01c-First-Plc-W-And3", HASH__SUCCESS}
  };
  pwr_tOid oid, nextoid;
  pwr_tOName nextname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextObject", d[i].next, m_sts);
      return;
    }

    m_sts = gdh_GetNextObject(oid, &nextoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextObject", d[i].next, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(nextoid, nextname, sizeof(nextname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextObject gdh_ObjidToName", d[i].next, m_sts);
	return;
      }
      if (strcmp(nextname, d[i].next) != 0) {
	m_log->log('E', "GetNextObject, wrong object", d[i].next);
	return;
      }
    }
  }

  m_log->log('S', "GetNextObject", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName previous;
  pwr_tStatus sts;
} sGetPreviousObject;

void ra_gdhtest::GetPreviousObject(void)
{
  sGetPreviousObject d[] = {
    {"Test01c-First-Dv4", "Test01c-First-Dv3", HASH__SUCCESS},
    {"Test01c-First-Dv3", "Test01c-First-Dv2", HASH__SUCCESS},
    {"Test01c-First-Dv1", "", GDH__NO_TYPE},
    {"Test01c-First-Aggr4", "Test01c-First-Aggr3", HASH__SUCCESS},
    {"Test01c-First-Aggr3", "Test01c-First-Aggr2", HASH__SUCCESS},
    {"Test01c-First-Aggr2", "Test01c-First-Aggr1", HASH__SUCCESS},
    {"Test01c-First-Aggr1", "", GDH__NO_TYPE},
    {"Test01c-First-Plc-W-And3", "Test01c-First-Plc-W-And2", HASH__SUCCESS},
    {"Test01c-First-Plc-W-And2", "Test01c-First-Plc-W-And1", HASH__SUCCESS},
    {"Test01c-First-Plc-W-And1", "Test01c-First-Plc-W-And0", HASH__SUCCESS},
    {"Test01c-First-Plc-W-And0", "", GDH__NO_TYPE}
  };
  pwr_tOid oid, previousoid;
  pwr_tOName previousname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetPreviousObject", d[i].previous, m_sts);
      return;
    }

    m_sts = gdh_GetPreviousObject(oid, &previousoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetPreviousObject", d[i].previous, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_ObjidToName(previousoid, previousname, sizeof(previousname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetPreviousObject gdh_ObjidToName", d[i].previous, m_sts);
	return;
      }
      if (strcmp(previousname, d[i].previous) != 0) {
	m_log->log('E', "GetPreviousObject, wrong object", d[i].previous);
	return;
      }
    }
  }

  m_log->log('S', "GetPreviousObject", GDH__SUCCESS);
}

void ra_gdhtest::GetClassListAttrRef(void)
{
  sGetClassList d[] = {
    {pwr_cClass_Node, "Nodes-PwrTest01c", MVOL__SUCCESS},
    {pwr_cClass_Security, "Nodes-PwrTest01c-Security", MVOL__SUCCESS},
    {pwr_cClass_OpPlace, "Nodes-PwrTest01c-OpPlaces-Op", MVOL__SUCCESS},
    {pwr_cClass_plc, "Test01c-First-Plc", MVOL__SUCCESS},
    {pwr_cClass_and, "Test01c-First-Plc-W-And0", MVOL__SUCCESS},
    {pwr_cClass_Dv, "Test01c-First-Dv1", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Roller, "Test01c-First-R1", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Motor, "Test01c-First-M1", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-Comp1", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Contactor, "Test01c-First-C1", MVOL__SUCCESS}
  };
  pwr_tAttrRef aref;
  pwr_tAName aname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_GetClassListAttrRef(d[i].cid, &aref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetClassListAttrRef", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_AttrrefToName(&aref, aname, sizeof(aname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetClassListAttrRef gdh_AttrrefToName", d[i].name, m_sts);
	return;
      }
      if (strcmp(aname, d[i].name) != 0) {
	m_log->log('E', "GetClassListAttrRef, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetClassListAttrRef", GDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tOName name;
  pwr_tOName next;
  pwr_tStatus sts;
} sGetNextAttrRef;

void ra_gdhtest::GetNextAttrRef(void)
{
  sGetNextAttrRef d[] = {
    {pwr_cClass_Dv, "Test01c-First-Dv1", "Test01c-First-Dv2", MVOL__SUCCESS},
    {pwr_cClass_Dv, "Test01c-First-Dv2", "Test01c-First-Dv3", MVOL__SUCCESS},
    {pwr_cClass_Dv, "Test01c-First-Dv3", "Test01c-First-Dv4", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-Comp1", "Test01c-First-C1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-C1.Super", "Test01c-First-M1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-M1.Super", "Test01c-First-M1.Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-M1.Contactor1.Super", "Test01c-First-R1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Super", "Test01c-First-R1.Motors[0].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[0].Super", "Test01c-First-R1.Motors[0].Contactor2.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[0].Contactor2.Super", "Test01c-First-R1.Motors[1].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[1].Super", "Test01c-First-R1.Motors[1].Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[1].Contactor1.Super", "Test01c-First-R1.Motors[2].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[2].Super", "Test01c-First-R1.Motors[2].Contactor2.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[2].Contactor2.Super", "Test01c-First-R1.Motors[3].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[3].Super", "Test01c-First-R1.Motors[3].Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[3].Contactor1.Super", "Test01c-First-R1.Motors[4].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[4].Super", "Test01c-First-R1.Motors[4].Contactor2.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[4].Contactor2.Super", "Test01c-First-R1.Motors[5].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[5].Super", "Test01c-First-R1.Motors[5].Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[5].Contactor1.Super", "Test01c-First-R1.Motors[6].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[6].Super", "Test01c-First-R1.Motors[6].Contactor2.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[6].Contactor2.Super", "Test01c-First-R1.Motors[7].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[7].Super", "Test01c-First-R1.Motors[7].Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[7].Contactor1.Super", "Test01c-First-R1.Motors[8].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[8].Super", "Test01c-First-R1.Motors[8].Contactor1.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[8].Contactor1.Super", "Test01c-First-R1.Motors[9].Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[9].Super", "Test01c-First-R1.Motors[9].Contactor2.Super", MVOL__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Test01c-First-R1.Motors[9].Contactor2.Super", "", GDH__NO_TYPE}
  };
  pwr_tAttrRef aref, nextaref;
  pwr_tAName nextname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextAttrRef", d[i].next, m_sts);
      return;
    }

    m_sts = gdh_GetNextAttrRef(d[i].cid, &aref, &nextaref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextAttrRef", d[i].next, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_AttrrefToName(&nextaref, nextname, sizeof(nextname), cdh_mNName);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextAttrRef gdh_AttrrefToName", d[i].next, m_sts);
	return;
      }
      if (strcmp(nextname, d[i].next) != 0) {
	m_log->log('E', "GetNextAttrRef, wrong object", d[i].next);
	return;
      }
    }
  }

  m_log->log('S', "GetNextAttrRef", GDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tObjName cname;
  pwr_tStatus sts;
} sClassNameToId;

void ra_gdhtest::ClassNameToId(void)
{
  sClassNameToId d[] = {
    {pwr_cClass_Node, "$Node", HASH__SUCCESS},
    {pwr_cClass_Security, "$Security", HASH__SUCCESS},
    {pwr_cClass_OpPlace, "OpPlace", HASH__SUCCESS},
    {pwr_cClass_plc, "PlcPgm", HASH__SUCCESS},
    {pwr_cClass_and, "And", HASH__SUCCESS},
    {pwr_cClass_Dv, "Dv", HASH__SUCCESS},
    {pwr_cClass_Pwrt01_Component, "Pwrt01_Component", HASH__SUCCESS},
    {pwr_cClass_Pwrt01_Motor, "Pwrt01_Motor", HASH__SUCCESS},
    {pwr_cClass_Pwrt01_Roller, "Pwrt01_Roller", HASH__SUCCESS},
    {pwr_cClass_Component, "NoValidClassName", GDH__BADOBJTYPE}
  };
  pwr_tCid cid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_ClassNameToId(d[i].cname, &cid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "ClassNameToId", d[i].cname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].cid != cid) {
	m_log->log('E', "ClassNameToId, wrong cid", d[i].cname);
	return;
      }
    }
  }

  m_log->log('S', "ClassNameToId", GDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tCid supercid;
  pwr_tStatus sts;
} sGetSuperClass;

void ra_gdhtest::GetSuperClass(void)
{
  sGetSuperClass d[] = {
    {pwr_cClass_BasePumpAggr, pwr_cClass_BaseMotorAggr, HASH__SUCCESS},
    {pwr_cClass_BaseMotorAggr, pwr_cClass_Aggregate, HASH__SUCCESS},
    {pwr_cClass_Aggregate, 0, GDH__NOSUCHCLASS},
    {pwr_cClass_BaseTempSensor, pwr_cClass_BaseSensor, HASH__SUCCESS},
    {pwr_cClass_BaseSensor, pwr_cClass_Component, HASH__SUCCESS},
    {pwr_cClass_Component, 0, GDH__NOSUCHCLASS},
    {pwr_cClass_Pwrt01_Roller, pwr_cClass_Pwrt01_Component, HASH__SUCCESS},
    {pwr_cClass_Pwrt01_Component, 0, GDH__NOSUCHCLASS},
    {0xffffffff, 0, HASH__NOTFOUND}
  };
  pwr_tCid supercid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_GetSuperClass(d[i].cid, &supercid, pwr_cNOid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetSuperClass", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].supercid != supercid) {
	m_log->log('E', "GetSuperClass, wrong cid");
	return;
      }
    }
  }

  m_log->log('S', "GetSuperClass", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName tname;
  pwr_tObjName name1;
  pwr_tInt32 value1;
  pwr_tObjName name2;
  pwr_tInt32 value2;
  pwr_tStatus sts;
} sGetEnumValueDef;

void ra_gdhtest::GetEnumValueDef(void)
{
  sGetEnumValueDef d[] = {
    {"pwrb:Type-YesNoEnum", "No", 0, "Yes", 1, HASH__SUCCESS},
    {"pwrb:Type-ColorThemeEnum", "Standard", 0, "Custom", 100, HASH__SUCCESS},
    {"CVolPwrtest01:Type-Pwrt01_MotorEnum", "Off", 0, "On", 1, HASH__SUCCESS}
  };

  pwr_tOid oid;
  gdh_sValueDef *vd;
  pwr_tTid tid;
  int rows;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].tname, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetEnumValueDef", "gdh_NameToObjid", m_sts);
      return;
    }
    tid = cdh_TypeObjidToId(oid);
    m_sts = gdh_GetEnumValueDef(tid, &vd, &rows);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetEnumValueDef", "wrong status", m_sts);
      if (ODD(m_sts))
	free(vd);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(vd[0].Name, d[i].name1) != 0 ||
	  vd[0].Value->Value != d[i].value1 ||
	  strcmp(vd[rows-1].Name, d[i].name2) != 0 ||
	  vd[rows-1].Value->Value != d[i].value2) {
	m_log->log('E', "GetEnumValueDef", "name of value doesn't match");
	free(vd);
	return;
      }
      free(vd);
    }
  }
  m_log->log('S', "GetEnumValueDef", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName tname;
  pwr_tObjName name1;
  pwr_tUInt32 value1;
  pwr_tObjName name2;
  pwr_tUInt32 value2;
  pwr_tStatus sts;
} sGetMaskBitDef;

void ra_gdhtest::GetMaskBitDef(void)
{
  sGetMaskBitDef d[] = {
    {"pwrb:Type-CardMask1_1", "Channel1", 1, "Channel16", 32768, HASH__SUCCESS},
    {"pwrb:Type-CardMask2_1", "Channel17", 1, "Channel32", 32768, HASH__SUCCESS},
    {"pwrb:Type-PidModeMask", "Manual", 1, "Cascade", 4, HASH__SUCCESS},
    {"CVolPwrtest01:Type-Pwrt01_MotorMask", "Off", 1, "Running", 8, HASH__SUCCESS}
  };

  pwr_tOid oid;
  gdh_sBitDef *bd;
  pwr_tTid tid;
  int rows;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].tname, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetMaskBitDef", "gdh_NameToObjid", m_sts);
      return;
    }
    tid = cdh_TypeObjidToId(oid);
    m_sts = gdh_GetMaskBitDef(tid, &bd, &rows);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetMaskBitDef", "wrong status", m_sts);
      if (ODD(m_sts))
	free(bd);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(bd[0].Name, d[i].name1) != 0 ||
	  bd[0].Bit->Value != d[i].value1 ||
	  strcmp(bd[rows-1].Name, d[i].name2) != 0 ||
	  bd[rows-1].Bit->Value != d[i].value2) {
	m_log->log('E', "GetMaskBitDef", "name of value doesn't match");
	free(bd);
	return;
      }
      free(bd);
    }
  }
  m_log->log('S', "GetMaskBitDef", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName owrite;
  pwr_tOName oread;
  pwr_tFileName fname;
  unsigned int size;
  pwr_tStatus sts_write;
  pwr_tStatus sts_read;
} sFWriteObject;

void ra_gdhtest::FWriteObject(void)
{
  sFWriteObject d[] = {
    {"Test01c-Gdh-A1", "Test01c-Gdh-A2", "/tmp/a1.dat", sizeof(pwr_cClass_AArray100), GDH__SUCCESS, GDH__SUCCESS},
    {"Test01c-Gdh-W1", "Test01c-Gdh-W2", "/tmp/w1.dat", sizeof(pwr_cClass_Pwrt01_WriteClass), GDH__SUCCESS, GDH__SUCCESS}
  };

  pwr_tAttrRef aref_write, aref_read;
  void *p_write, *p_read;
  pwr_tCmd cmd;
  pwr_tFileName fname;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].owrite, &aref_write);
    if (EVEN(m_sts)) {
      m_log->log('E', "FWriteObject", "gdh_NameToAttrref", m_sts);
      return;
    }
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].oread, &aref_read);
    if (EVEN(m_sts)) {
      m_log->log('E', "FWriteObject", "gdh_NameToAttrref", m_sts);
      return;
    }
    m_sts = gdh_FWriteObject(d[i].fname, &aref_write);
    if (m_sts != d[i].sts_write) {
      m_log->log('E', "FWriteObject", "wrong status", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = gdh_FReadObject(d[i].fname, &aref_read);
      if (m_sts != d[i].sts_read) {
	m_log->log('E', "FReadObject", "wrong status", m_sts);
	return;
      }
      m_sts = gdh_AttrRefToPointer(&aref_write, &p_write);
      if (EVEN(m_sts)) {
	m_log->log('E', "FWriteObject", "gdh_AttrRefToPointer", m_sts);
	return;
      }
      m_sts = gdh_AttrRefToPointer(&aref_read, &p_read);
      if (EVEN(m_sts)) {
	m_log->log('E', "FWriteObject", "gdh_AttrRefToPointer", m_sts);
	return;
      }
      if (memcmp(p_write, p_read, d[i].size) != 0) {
	m_log->log('E', "FWriteObject", "content differs");
	return;
      }
    }
    
    // Remove file
    dcli_translate_filename(fname, d[i].fname);
    sprintf(cmd, "rm -f %s", fname);
    system(cmd);
  }
  m_log->log('S', "FWriteObject", GDH__SUCCESS);
}

void ra_gdhtest::GetSecurityInfo(void)
{
  pwr_sSecurity security;
  pwr_sSecurity dsec = {0, 4, 0, "Security"};

  m_sts = gdh_GetSecurityInfo(&security);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetSecurityInfo", m_sts);
    return;
  }

  if (security.DefaultWebPriv != dsec.DefaultWebPriv ||
      security.DefaultXttPriv != dsec.DefaultXttPriv ||
      security.XttUseOpsysUser != dsec.XttUseOpsysUser ||
      strcmp(security.WebSystemGroup, dsec.WebSystemGroup) != 0) {
    m_log->log('E', "GetSecurityInfo", "content mismatch");
    return;
  }
  m_log->log('S', "GetSecurityInfo", GDH__SUCCESS);
}

void ra_gdhtest::TranslateRtdbPointer(void)
{
  pwr_sClass_Av *avp;
  pwr_sClass_Dv *dvp;
  pwr_tFloat32 *fp;
  pwr_tBoolean *bp;
  unsigned long ofp;
  unsigned long obp;
  
  m_sts = gdh_NameToPointer("Test01c-Gdh-Av1", (void **)&avp);
  if (EVEN(m_sts)) {
    m_log->log('E', "TranslateRtdbPointer", "gdh_NameToPointer", m_sts);
    return;
  }

  fp = (pwr_tFloat32 *)gdh_TranslateRtdbPointer((unsigned long)avp->ActualValue);
  if (fabs(*fp - 222) > FLT_EPSILON) {
    m_log->log('E', "TranslateRtdbPointer, value error");
    return;
  }

  gdh_StoreRtdbPointer(&ofp, fp);
  if (ofp != (unsigned long)avp->ActualValue) {
    m_log->log('E', "TranslateRtdbPointer, pointer value error");
    return;
  }

  m_sts = gdh_NameToPointer("Test01c-Gdh-Dv1", (void **)&dvp);
  if (EVEN(m_sts)) {
    m_log->log('E', "TranslateRtdbPointer", "gdh_NameToPointer", m_sts);
    return;
  }

  bp = (pwr_tBoolean *)gdh_TranslateRtdbPointer((unsigned long)dvp->ActualValue);
  if (*bp != 1) {
    m_log->log('E', "TranslateRtdbPointer, value error");
    return;
  }

  gdh_StoreRtdbPointer(&obp, bp);
  if (obp != (unsigned long)dvp->ActualValue) {
    m_log->log('E', "TranslateRtdbPointer, pointer value error");
    return;
  }

  m_log->log('S', "TranslateRtdbPointer", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tUInt32 size;
  pwr_tStatus sts;
} sGetObjectSize;

void ra_gdhtest::GetObjectSize(void)
{
  sGetObjectSize d[] = {
    {"Test01c-Gdh-A1", sizeof(pwr_sClass_AArray100), HASH__SUCCESS},
    {"Test01c-Gdh-P1", sizeof(pwr_sClass_BaseFcPPO3PumpAggr), HASH__SUCCESS},
    {"Test01c-Gdh-W1", sizeof(pwr_sClass_Pwrt01_WriteClass), HASH__SUCCESS},
    {"Test01c-Gdh-Av1", sizeof(pwr_sClass_Av), HASH__SUCCESS},
    {"Test01c-Gdh-Dv1", sizeof(pwr_sClass_Dv), HASH__SUCCESS},
    {"VolPwrTest01c:", sizeof(pwr_sClass_RootVolume), HASH__SUCCESS},
    {"Test01c", sizeof(pwr_sClass_PlantHier), HASH__SUCCESS}
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
  pwr_tObjName aname;
  pwr_tCid cid;
  pwr_tUInt32 size;
  pwr_tUInt32 asize;
  pwr_tStatus sts;
  pwr_tOid oid;
} sGetDynamicAttrSize;

void ra_gdhtest::GetDynamicAttrSize(void)
{
  sGetDynamicAttrSize d[] = {
    {"pwrNode-Test-H1-DynSize1", ".Attr", pwr_cClass_Pwrt01_DynAttr, 24, 24-sizeof(pwr_tString16), HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H1-DynSize2", ".Attr", pwr_cClass_Pwrt01_DynAttr, 416, 416-sizeof(pwr_tString16), HASH__SUCCESS, pwr_cNOid},
    {"pwrNode-Test-H1-DynSize3", ".Attr", pwr_cClass_Pwrt01_DynAttr, 4016, 4016-sizeof(pwr_tString16), HASH__SUCCESS, pwr_cNOid}
  };
  pwr_tUInt32 size, asize;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_CreateObject(d[i].name, d[i].cid, d[i].size, &d[i].oid,
			     pwr_cNOid, 0, pwr_cNOid);
    if (m_sts == GDH__DUPLNAME)
      m_sts = gdh_NameToObjid(d[i].name, &d[i].oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDynamicAttrSize, gdh_CreateObject", m_sts);
      return;
    }

    m_sts = gdh_GetObjectSize(d[i].oid, &size);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetDynamicAttrSize", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].size != size) {
	m_log->log('E', "GetDynamicAttrSize, object size doesn't match");
	return;
      }
    }

    m_sts = gdh_GetDynamicAttrSize(d[i].oid, d[i].aname, &asize); 
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDynamicAttrSize", m_sts);
      return;
    }
    if (asize != d[i].asize) {
      m_log->log('E', "GetDynamicAttrSize, attr size doesn't match");
      return;
    }
  }

  m_log->log('S', "GetDynamicAttrSize", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetObjectClass;

void ra_gdhtest::GetObjectClass(void)
{
  sGetObjectClass d[] = {
    {"Test01c-Gdh-A1", pwr_cClass_AArray100, HASH__SUCCESS},
    {"Test01c-Gdh-P1", pwr_cClass_BaseFcPPO3PumpAggr, HASH__SUCCESS},
    {"Test01c-Gdh-W1", pwr_cClass_Pwrt01_WriteClass, HASH__SUCCESS},
    {"Test01c-Gdh-Av1", pwr_cClass_Av, HASH__SUCCESS},
    {"Test01c-Gdh-Dv1", pwr_cClass_Dv, HASH__SUCCESS},
    {"VolPwrTest01c:", pwr_cClass_RootVolume, HASH__SUCCESS},
    {"Test01c", pwr_cClass_PlantHier, HASH__SUCCESS}
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

void ra_gdhtest::GetAttrRefTid(void)
{
  sGetAttrRefTid d[] = {
    {"Test01c-Gdh-A1.Value", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01c-Gdh-A1.Value[0]", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01c.Description", 0, "pwrs:Type-$String80", GDH__SUCCESS},
    {"Test01c-Gdh-P1.CircuitBreaker.NotTripped.ActualValue", pwr_eType_Boolean, "", GDH__SUCCESS},
    {"Test01c-Gdh-Av1.ActualValue", pwr_eType_Float32, "", GDH__SUCCESS},
    {"Test01c-Gdh-Dv1.ActualValue", pwr_eType_Boolean, "", GDH__SUCCESS},
    {"Test01c-Gdh-A1", pwr_cClass_AArray100, "", GDH__SUCCESS},
    {"Test01c-Gdh-P1", pwr_cClass_BaseFcPPO3PumpAggr, "", GDH__SUCCESS},
    {"Test01c-Gdh-W1", pwr_cClass_Pwrt01_WriteClass, "", GDH__SUCCESS},
    {"Test01c-Gdh-Av1", pwr_cClass_Av, "", GDH__SUCCESS},
    {"Test01c-Gdh-Dv1", pwr_cClass_Dv, "", GDH__SUCCESS},
    {"VolPwrTest01c:", pwr_cClass_RootVolume, "", GDH__SUCCESS},
    {"Test01c", pwr_cClass_PlantHier, "", GDH__SUCCESS}
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

void ra_gdhtest::GetObjectLocation(void)
{
  sGetObjectLocation d[] = {
    {"Test01c-Gdh-A1", 1, HASH__SUCCESS},
    {"Test01c-Gdh", 1, HASH__SUCCESS},
    {"Test01c-Gdh-A1", 1, HASH__SUCCESS},
    {"VolPwrTest01c:", 1, HASH__SUCCESS}
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

typedef struct {
  pwr_tOName name1;
  pwr_tOName name2;
  pwr_tUInt32 n1;
  pwr_tUInt32 n2;
  pwr_tStatus sts;
} sGetObjectDLCount;

void ra_gdhtest::GetObjectDLCount(void)
{
  sGetObjectDLCount d[] = {
    {"Test01c-Gdh-A1.Value[0]", "", 5, 0, HASH__SUCCESS},
    {"Test01c-Gdh-A1", "", 10, 0, HASH__SUCCESS},
    {"Test01c-Gdh-Av1", "Test01c-Gdh-Av1.ActualValue", 7, 7, HASH__SUCCESS},
    {"Test01c-Gdh-Av1.Description", "Test01c-Gdh-Av1.ActualValue", 4, 10, HASH__SUCCESS},
    {"Test01c-Gdh", "", 0, 0, HASH__SUCCESS}
  };
 
  pwr_tAttrRef aref1, aref2;
  pwr_tUInt32 count;
  void *p;
  pwr_tDlid dlid1[10], dlid2[10];
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name1, &aref1);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectDLCount, gdh_NameToAttrref", d[i].name1, m_sts);
      return;
    }

    if (d[i].n2 > 0) {
      m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name2, &aref2);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetObjectDLCount, gdh_NameToAttrref", d[i].name2, m_sts);
	return;
      }
    }

    for (unsigned int j = 0; j < d[i].n1; j++) {
      m_sts = gdh_DLRefObjectInfoAttrref(&aref1, &p, &dlid1[j]);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetObjectDLCount, gdh_DLRefObjectInfoAttrref", d[i].name1, m_sts);
	return;
      }
    }
    for (unsigned int j = 0; j < d[i].n2; j++) {
      m_sts = gdh_DLRefObjectInfoAttrref(&aref2, &p, &dlid2[j]);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetObjectDLCount, gdh_DLRefObjectInfoAttrref", d[i].name2, m_sts);
	return;
      }
    }

    m_sts = gdh_GetObjectDLCount(aref1.Objid, &count);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetObjectDLCount", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if ( count != d[i].n1 + d[i].n2) {
	m_log->log('E', "GetObjectDLCount, count doesn't match", d[i].name1);
	return;
      }
    }

    for (unsigned int j = 0; j < d[i].n1; j++) {
      m_sts = gdh_DLUnrefObjectInfo(dlid1[j]);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetObjectDLCount, gdh_DLUnrefObjectInfo", d[i].name1, m_sts);
	return;
      }
    }
    for (unsigned int j = 0; j < d[i].n2; j++) {
      m_sts = gdh_DLUnrefObjectInfo(dlid2[j]);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetObjectDLCount, gdh_DLUnrefObjectInfo", d[i].name2, m_sts);
	return;
      }
    }

  }

  m_log->log('S', "GetObjectDLCount", GDH__SUCCESS);
}

void ra_gdhtest::GetVolumeList(void)
{
  pwr_tObjName name;
  pwr_tVid vid;

  m_sts = gdh_GetVolumeList(&vid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetVolumeList", m_sts);
    return;
  }
  m_sts = gdh_VolumeIdToName(vid, name, sizeof(name));
  if (EVEN(m_sts)) {
    m_log->log('E', "GetVolumeList, gdh_VolumeIdToName", m_sts);
    return;
  }
  if (strcmp(name, "VolPwrTest01c") != 0) {
    m_log->log('E', "GetVolumeList, wrong volume", name);
    return;
  }

  m_log->log('S', "GetVolumeList", GDH__SUCCESS);
}

typedef struct {
  char vid[20];
  char nextvid[20];
  pwr_tStatus sts;
} sGetNextVolume;

void ra_gdhtest::GetNextVolume(void)
{
  sGetNextVolume d[] = {
    {"_V0.254.254.203", "_V254.254.254.245", GDH__SUCCESS},
    {"_V254.254.254.245", "_V0.0.0.1", GDH__SUCCESS},
    {"_V0.0.0.1", "_V0.0.0.2", GDH__SUCCESS},
    {"_V0.0.240.254", "_V1.254.254.203", GDH__SUCCESS},
    {"_V1.254.254.203", "", GDH__NOSUCHVOL}
  };
  char volumes[][80] = {"_V0.0.0.1", "_V0.0.0.2", "_V0.0.250.7", "_V0.254.254.203", 
			"_V0.0.240.254", "_V254.254.254.245"};
  pwr_tVid vids[] = {0, 0, 0, 0, 0, 0};
  pwr_tVid vid, nvid;
  int found;
  
  for (unsigned int i = 0; i < sizeof(vids)/sizeof(vids[0]); i++) {
    m_sts = cdh_StringToVolumeId(volumes[i], &vids[i]);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextVolume, cdh_StringToVolumeId", volumes[i], m_sts);
      return;
    }
  }

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextVolume, cdh_StringToVolumeId", d[i].vid, m_sts);
      return;
    }

    m_sts = gdh_GetNextVolume(vid, &nvid);
    if (m_sts == GDH__NOSUCHVOL)
      continue;
    else if (m_sts != GDH__SUCCESS) {
      m_log->log('E', "GetNextVolume", d[i].vid, m_sts);
      return;
    }
    found = 0;
    for ( unsigned int j = 0; j < sizeof(vids)/sizeof(vids[0]); j++) {
      if (nvid == vids[j]) {
	found = 1;
	break;
      }
    }
    if (!found) {
      m_log->vlog('E', "GetNextVolume, unknown volume %s", 
		  cdh_VolumeIdToString(0, 0, nvid, 1, 0));
      return;
    }
  }

  m_log->log('S', "GetNextVolume", GDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vid[20];
  pwr_tStatus sts;
} sVolumeIdToName;

void ra_gdhtest::VolumeIdToName(void)
{
  sVolumeIdToName d[] = {
    {"VolPwrTest01c", "_V0.254.254.203", GDH__SUCCESS},
    {"rt", "_V254.254.254.245", GDH__SUCCESS},
    {"pwrs", "_V0.0.0.1", GDH__SUCCESS},
    {"pwrb", "_V0.0.0.2", GDH__SUCCESS},
    {"Simul", "_V0.0.0.8", GDH__SUCCESS},
    {"BaseComponent", "_V0.0.0.10", GDH__SUCCESS},
    {"NMps", "_V0.0.1.1", GDH__SUCCESS},
    {"Profibus", "_V0.0.250.7", GDH__SUCCESS},
    {"CVolPwrtest01", "_V0.0.240.254", GDH__SUCCESS},
    {"1_254_254_203", "_V1.254.254.203", GDH__SUCCESS}
  };
  pwr_tVid vid;
  pwr_tObjName name;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "VolumeIdToName, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_VolumeIdToName(vid, name, sizeof(name));
    if (m_sts != d[i].sts) {
      m_log->log('E', "VolumeIdToName", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(d[i].name, name) != 0) {
	m_log->log('E', "GetNextVolume, wrong volume name", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "VolumeIdToName", GDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vid[20];
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetVolumeInfo;

void ra_gdhtest::GetVolumeInfo(void)
{
  sGetVolumeInfo d[] = {
    {"VolPwrTest01c", "_V0.254.254.203", pwr_eClass_RootVolume, GDH__SUCCESS},
    {"rt", "_V254.254.254.245", pwr_eClass_SharedVolume, GDH__SUCCESS},
    {"pwrs", "_V0.0.0.1", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"pwrb", "_V0.0.0.2", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"Simul", "_V0.0.0.8", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"BaseComponent", "_V0.0.0.10", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"NMps", "_V0.0.1.1", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"Profibus", "_V0.0.250.7", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"CVolPwrtest01", "_V0.0.240.254", pwr_eClass_ClassVolume, GDH__SUCCESS},
    {"1_254_254_203", "_V1.254.254.203", pwr_eClass_SystemVolume, GDH__SUCCESS}
  };
  pwr_tVid vid;
  gdh_sVolumeInfo info;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetVolumeInfo, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_GetVolumeInfo(vid, &info);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetVolumeInfo", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(d[i].name, info.name) != 0 || 
	  d[i].cid != info.cid) {
	m_log->log('E', "GetNextVolume, data doesn't match", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetVolumeInfo", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tTypeId type;
  int size;
  char valuestr[80];
  pwr_tDlid dlid;
  pwr_tStatus sts;
} sDLRefObjectInfoAttrref;

void ra_gdhtest::DLRefObjectInfoAttrref(void)
{
  sDLRefObjectInfoAttrref d[] = {
    {"Test01c-Gdh-Av1.ActualValue", pwr_eType_Float32, sizeof(pwr_tFloat32), "222", pwr_cNDlid, GDH__SUCCESS},
    {"Test01c-Gdh-Av1.Description", pwr_eType_String, 80, "Av with value 222", pwr_cNDlid, GDH__SUCCESS},
    {"Test01c-Gdh-Dv1.ActualValue", pwr_eType_Boolean, sizeof(pwr_tBoolean), "1", pwr_cNDlid, GDH__SUCCESS},
    {"Test01c-Gdh-Dv1.Description", pwr_eType_String, 80, "Dv with value true", pwr_cNDlid, GDH__SUCCESS},
    {"Test01c-Gdh-W1.Value", pwr_eType_Float32, 4, "9", pwr_cNDlid, GDH__SUCCESS}
  };
 
  pwr_tAttrRef aref;
  char value[80];
  char *p;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "DLRefObjectInfoAttrref, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &d[i].dlid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "DLRefObjectInfoAttrref, wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = gdh_AttrStringToValue(d[i].type, d[i].valuestr, value, sizeof(value), 
				    d[i].size);
      if (EVEN(m_sts)) {
	m_log->log('E', "DLRefObjectInfoAttrref, gdh_AttrStringToValue", d[i].name, m_sts);
	return;
      }
      switch (d[i].type) {
      case pwr_eType_String:
      case pwr_eType_Text:
	if (strcmp(value, p) != 0) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      case pwr_eType_Float32:
	if (fabs(*(pwr_tFloat32 *)value - *(pwr_tFloat32 *)p) > FLT_EPSILON * 100) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
	break;
      default:
	if (memcmp(value, p, d[i].size) != 0) {
	  m_log->log('E', "AttrRefToPointer, wrong value", d[i].name);
	  return;
	}
      }
    }
    m_sts = gdh_DLUnrefObjectInfo(d[i].dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "DLRefObjectInfoAttrref, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "DLRefObjectInfoAttrref", GDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  char valuestr[80];
} sGetTimeDL;

void ra_gdhtest::GetTimeDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-ATv1.ActualValue", "21-MAY-2020 8:24:00.00"}
  };
 
  pwr_tAttrRef aref;
  pwr_tDlid dlid;
  pwr_tTime value;
  pwr_tTime *p;
  pwr_tTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetTimeDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetTimeDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    gdh_GetTimeDL(p, &pvalue);

    m_sts = time_AsciiToA(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetTimeDL, time_AsciiToA", d[i].name, m_sts);
      return;
    }
    if (memcmp(&value, &pvalue, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "GetTimeDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetTimeDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "GetTimeDL", GDH__SUCCESS);
}

void ra_gdhtest::SetTimeDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2020 8:24:00.00"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2020 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2021 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2022 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2023 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2024 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2025 9:39:06.22"}
  };
 
  pwr_tAttrRef aref;
  pwr_tTime value;
  pwr_tTime *p;
  pwr_tTime pvalue;
  pwr_tDlid dlid;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetTimeDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetTimeDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = time_AsciiToA(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetTimeDL, time_AsciiToA", d[i].name, m_sts);
      return;
    }

    gdh_SetTimeDL(p, &value);
    gdh_GetTimeDL(p, &pvalue);

    if (memcmp(&value, &pvalue, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "SetTimeDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetTimeDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "SetTimeDL", GDH__SUCCESS);
}

void ra_gdhtest::GetDeltaTimeDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-DTv1.ActualValue", "12:12:12.12"}
  };
 
  pwr_tAttrRef aref;
  pwr_tDlid dlid;
  pwr_tDeltaTime value;
  pwr_tDeltaTime *p;
  pwr_tDeltaTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDeltaTimeDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDeltaTimeDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    gdh_GetDeltaTimeDL(p, &pvalue);

    m_sts = time_AsciiToD(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDeltaTimeDL, time_AsciiToA", d[i].name, m_sts);
      return;
    }
    if (memcmp(&value, &pvalue, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "GetDeltaTimeDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetDeltaTimeDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "GetDeltaTimeDL", GDH__SUCCESS);
}

void ra_gdhtest::SetDeltaTimeDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-DTv2.ActualValue", "2 8:24:00.00"},
    {"Test01c-Gdh-DTv2.ActualValue", "55 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "56 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "57 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "58 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "59 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "60 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "61 09:39:06.22"}
  };
 
  pwr_tAttrRef aref;
  pwr_tDeltaTime value;
  pwr_tDeltaTime *p;
  pwr_tDeltaTime pvalue;
  pwr_tDlid dlid;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetDeltaTimeDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetDeltaTimeDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = time_AsciiToD(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetDeltaTimeDL, time_AsciiToA", d[i].name, m_sts);
      return;
    }

    gdh_SetDeltaTimeDL(p, &value);
    gdh_GetDeltaTimeDL(p, &pvalue);

    if (memcmp(&value, &pvalue, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "SetDeltaTimeDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetDeltaTimeDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "SetDeltaTimeDL", GDH__SUCCESS);
}

void ra_gdhtest::GetStrDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-Sv1.ActualValue", "This is a test string"}
  };
 
  pwr_tAttrRef aref;
  pwr_tDlid dlid;
  char *p;
  pwr_tString80 pvalue;
  
  lck_Create(&m_sts, lck_eLock_Str);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetStrDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetStrDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    gdh_GetStrDL(p, pvalue, sizeof(pwr_tString80));

    if (strcmp(d[i].valuestr, pvalue) != 0) {
      m_log->log('E', "GetStrDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetStrDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "GetStrDL", GDH__SUCCESS);
}

void ra_gdhtest::SetStrDL(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-Sv2.ActualValue", "Test string"},
    {"Test01c-Gdh-Sv2.ActualValue", "Test string2345678901234567890123456789012345678901234567890123456789012345678"}
  };
 
  pwr_tAttrRef aref;
  char *p;
  pwr_tString80 pvalue;
  pwr_tDlid dlid;
  
  lck_Create(&m_sts, lck_eLock_Str);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToAttrref(pwr_cNOid, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetStrDL, NameToAttrref", d[i].name, m_sts);
      return;
    }

    m_sts = gdh_DLRefObjectInfoAttrref(&aref, (void **)&p, &dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetStrDL, gdh_DLRefObjectInfoAttrref", d[i].name, m_sts);
      return;
    }

    gdh_SetStrDL(p, d[i].valuestr, sizeof(pwr_tString80));
    gdh_GetStrDL(p, pvalue, sizeof(pwr_tString80));

    if (strcmp(d[i].valuestr, pvalue) != 0) {
      m_log->log('E', "SetStrDL, wrong value", d[i].name);
      return;
    }
    m_sts = gdh_DLUnrefObjectInfo(dlid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetStrDL, gdh_DLUnrefObjectInfo", d[i].name, m_sts);
      return;
    }
  }

  m_log->log('S', "SetStrDL", GDH__SUCCESS);
}

void ra_gdhtest::GetObjectInfoTime(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-ATv1.ActualValue", "21-MAY-2020 8:24:00.00"}
  };
 
  pwr_tTime value;
  pwr_tTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    gdh_GetObjectInfoTime(d[i].name, &pvalue);

    m_sts = time_AsciiToA(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectInfoTime, time_AsciiToA", d[i].name, m_sts);
      return;
    }
    if (memcmp(&value, &pvalue, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "GetObjectInfoTime, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "GetObjectInfoTime", GDH__SUCCESS);
}

void ra_gdhtest::SetObjectInfoTime(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2020 8:24:00.00"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2020 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2021 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2022 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2023 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2024 9:39:06.22"},
    {"Test01c-Gdh-ATv2.ActualValue", "21-MAY-2025 9:39:06.22"}
  };
 
  pwr_tTime value;
  pwr_tTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_AsciiToA(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectInfoTime, time_AsciiToA", d[i].name, m_sts);
      return;
    }

    gdh_SetObjectInfoTime(d[i].name, &value);
    gdh_GetObjectInfoTime(d[i].name, &pvalue);

    if (memcmp(&value, &pvalue, sizeof(pwr_tTime)) != 0) {
      m_log->log('E', "SetObjectInfoTime, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "SetObjectInfoTime", GDH__SUCCESS);
}

void ra_gdhtest::GetObjectInfoDeltaTime(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-DTv1.ActualValue", "12:12:12.12"}
  };
 
  pwr_tDeltaTime value;
  pwr_tDeltaTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    gdh_GetObjectInfoDeltaTime(d[i].name, &pvalue);

    m_sts = time_AsciiToD(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectInfoDeltaTime, time_AsciiToA", d[i].name, m_sts);
      return;
    }
    if (memcmp(&value, &pvalue, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "GetObjectInfoDeltaTime, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "GetObjectInfoDeltaTime", GDH__SUCCESS);
}

void ra_gdhtest::SetObjectInfoDeltaTime(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-DTv2.ActualValue", "2 8:24:00.00"},
    {"Test01c-Gdh-DTv2.ActualValue", "55 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "56 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "57 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "58 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "59 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "60 09:39:06.22"},
    {"Test01c-Gdh-DTv2.ActualValue", "61 09:39:06.22"}
  };
 
  pwr_tDeltaTime value;
  pwr_tDeltaTime pvalue;
  
  lck_Create(&m_sts, lck_eLock_Time);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = time_AsciiToD(d[i].valuestr, &value);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectInfoDeltaTime, time_AsciiToA", d[i].name, m_sts);
      return;
    }

    gdh_SetObjectInfoDeltaTime(d[i].name, &value);
    gdh_GetObjectInfoDeltaTime(d[i].name, &pvalue);

    if (memcmp(&value, &pvalue, sizeof(pwr_tDeltaTime)) != 0) {
      m_log->log('E', "SetObjectInfoDeltaTime, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "SetObjectInfoDeltaTime", GDH__SUCCESS);
}

void ra_gdhtest::GetObjectInfoStr(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-Sv1.ActualValue", "This is a test string"}
  };
 
  pwr_tString80 pvalue;
  
  lck_Create(&m_sts, lck_eLock_Str);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    gdh_GetObjectInfoStr(d[i].name, pvalue, sizeof(pwr_tString80));

    if (strcmp(d[i].valuestr, pvalue) != 0) {
      m_log->log('E', "GetObjectInfoStr, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "GetObjectInfoStr", GDH__SUCCESS);
}

void ra_gdhtest::SetObjectInfoStr(void)
{
  sGetTimeDL d[] = {
    {"Test01c-Gdh-Sv2.ActualValue", "Test string"},
    {"Test01c-Gdh-Sv2.ActualValue", "Test string2345678901234567890123456789012345678901234567890123456789012345678"}
  };
 
  pwr_tString80 pvalue;
  
  lck_Create(&m_sts, lck_eLock_Str);

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {

    gdh_SetObjectInfoStr(d[i].name, d[i].valuestr, sizeof(pwr_tString80));
    gdh_GetObjectInfoStr(d[i].name, pvalue, sizeof(pwr_tString80));

    if (strcmp(d[i].valuestr, pvalue) != 0) {
      m_log->log('E', "SetObjectInfoStr, wrong value", d[i].name);
      return;
    }
  }

  m_log->log('S', "SetObjectInfoStr", GDH__SUCCESS);
}

// Constructor
ra_gdhtest::ra_gdhtest()
{
  m_log = new tst_log(&m_sts, "rt-Gdh", "$pwrp_log/gdh.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
ra_gdhtest::~ra_gdhtest()
{
  gdh_DeleteObjectTree(m_dynroot);

  delete m_log;
}

int main()
{
  ra_gdhtest gdh;

  // setenv("TZ", "Europe/Stockholm", 1);
  // tzset();

  gdh.Init();
  gdh.ObjidToName();
  gdh.ObjidToPointer();
  gdh.MountObjidToPointer();
  gdh.AttrRefToPointer();
  gdh.NameToPointer();
  gdh.CreateObject();
  gdh.DeleteObject();
  gdh.DeleteObjectTree();
  gdh.RenameObject();
  gdh.MoveObject();
  gdh.GetRootList();
  gdh.GetChild();
  gdh.GetParent();
  gdh.GetNextSibling();
  gdh.GetPreviousSibling();
  gdh.GetNodeIndex();
  gdh.GetObjectNodeIndex();
  gdh.GetNodeObject();
  gdh.GetAttributeCharacteristics();
  gdh.GetAttributeCharAttrref();
  gdh.NameToAttrref();
  gdh.AttrrefToName();
  gdh.ClassAttrToAttrref();
  gdh.AttrArefToObjectAref();
  gdh.ArefANameToAref();
  gdh.AttrValueToString();
  gdh.AttrStringToValue();
  gdh.ArefDisabled();
  gdh.GetClassList();
  gdh.GetNextObject();
  gdh.GetPreviousObject();
  gdh.GetClassListAttrRef();
  gdh.GetNextAttrRef();
  gdh.ClassNameToId();
  gdh.GetSuperClass();
  gdh.GetEnumValueDef();
  gdh.GetMaskBitDef();
  gdh.FWriteObject();
  gdh.GetSecurityInfo();
  gdh.TranslateRtdbPointer();
  gdh.GetObjectSize();
  gdh.GetDynamicAttrSize();
  gdh.GetObjectClass();
  gdh.GetAttrRefTid();
  gdh.GetObjectLocation();
  gdh.GetObjectDLCount();
  gdh.GetVolumeList();
  gdh.GetNextVolume();
  gdh.VolumeIdToName();
  gdh.GetVolumeInfo();
  gdh.DLRefObjectInfoAttrref();
  gdh.GetTimeDL();
  gdh.SetTimeDL();
  gdh.GetDeltaTimeDL();
  gdh.SetDeltaTimeDL();
  gdh.GetStrDL();
  gdh.SetStrDL();
  gdh.GetObjectInfoTime();
  gdh.SetObjectInfoTime();
  gdh.GetObjectInfoDeltaTime();
  gdh.SetObjectInfoDeltaTime();
  gdh.GetObjectInfoStr();
  gdh.SetObjectInfoStr();
}

