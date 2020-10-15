/*
  Test ldh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "wb_ldh_msg.h"
#include "co_cdh_msg.h"
#include "co_tree_msg.h"
#include "wb_ldh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest02classes.h"
#include "wa_ldhtest.h"

pwr_tStatus wa_ldhtest::check_object(pwr_tOid oid)
{
  pwr_tOid p, child, next, prev;
  pwr_tStatus sts;

  sts = ldh_GetParent(m_ldhses, oid, &p);
  if (ODD(sts)) {
    int found = 0;
    for (sts = ldh_GetChild(m_ldhses, p, &child); 
	 ODD(sts); 
	 sts = ldh_GetNextSibling(m_ldhses, child, &child)) {
      if (cdh_ObjidIsEqual(oid, child)) {
	found = 1;
	break;
      }
    }
    if (!found)
      return LDH__NO_CHILD;
  }

  sts = ldh_GetNextSibling(m_ldhses, oid, &next);
  if (ODD(sts)) {
    sts = ldh_GetPreviousSibling(m_ldhses, next, &prev);
    if (EVEN(sts))
      return sts;

    if (cdh_ObjidIsNotEqual(oid, prev))
      return LDH__NO_SIBLING;
  }
  
  sts = ldh_GetPreviousSibling(m_ldhses, oid, &prev);
  if (ODD(sts)) {
    sts = ldh_GetNextSibling(m_ldhses, prev, &next);
    if (EVEN(sts))
      return sts;

    if (cdh_ObjidIsNotEqual(oid, next))
      return LDH__NO_SIBLING;
  }

  for (sts = ldh_GetChild(m_ldhses, oid, &child);
       ODD(sts); 
       sts = ldh_GetNextSibling(m_ldhses, child, &child)) {
    sts = ldh_GetParent(m_ldhses, child, &p);
    if (cdh_ObjidIsNotEqual(oid, p))
      return LDH__NO_CHILD;
  }
  return LDH__SUCCESS;
}

typedef struct {
  pwr_tOName name;
  pwr_tStatus sts1;
  pwr_tStatus sts2;
  pwr_tOid oid;
} sObjidToName;

// NameToObjid and ObjidToName
void wa_ldhtest::ObjidToName(void)
{
  sObjidToName d[] = {
    {"Test02-Ldh", LDH__SUCCESS, LDH__SUCCESS, pwr_cNOid},
    {"Test02-Ldh-ƒ≈÷‰Âˆ", LDH__SUCCESS, LDH__SUCCESS, pwr_cNOid},
    {"Test02-Ldh-LongName90123456789012345678901", LDH__SUCCESS, LDH__SUCCESS, pwr_cNOid},
    {"Test02-Ldh-TooLongName234567890123456789012", LDH__BADNAME, 0, pwr_cNOid}
  };
  pwr_tOName oname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &d[i].oid, d[i].name);
    if (m_sts != d[i].sts1) {
      m_log->log('E', "ObjidToName, NameToObjid wrong sts", d[i].name, m_sts);
      return;
    }
    else if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, d[i].oid, ldh_eName_Hierarchy, oname, sizeof(oname), &size);
      if (m_sts != d[i].sts2) {
	m_log->log('E', "ObjidToName, wrong sts", d[i].name, m_sts);
	return;
      }
      else if (ODD(m_sts)) {
	if (strcmp(oname, d[i].name) != 0) {
	  m_log->vlog('E', "ObjidToName, wrong object %s != %s, idx %d", oname, d[i].name, i);
	  return;
	}
      }
    }
  }

  m_log->log('S', "ObjidToName", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName dest;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid oid;
} sCreateObject;

void wa_ldhtest::CreateObject(void)
{
  sCreateObject d[] = {
    {"Test", "", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"H1", "Test", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"H2", "Test", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"H3", "Test", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"ƒ≈÷‰Âˆ", "Test-H3", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"LongName90123456789012345678901", "Test-H3", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"TooLongName234567890123456789012", "Test-H3", pwr_cClass_PlantHier, LDH__BADNAME, pwr_cNOid},
    {"Invalid#Name", "Test-H3", pwr_cClass_PlantHier, LDH__BADNAME, pwr_cNOid}
  };
  pwr_tOid dest;
  pwr_tOName oname, name;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (strcmp(d[i].dest, "") != 0) {
      strcpy(name, d[i].dest);
      strcat(name, "-");
      strcat(name, d[i].name);

      m_sts = ldh_NameToObjid(m_ldhses, &dest, d[i].dest);
      if (EVEN(m_sts)) {
	m_log->log('E', "CreateObject, destination ldh_NameToObjid", d[i].name, m_sts);
	return;
      }
    }
    else {
      strcpy(name, d[i].name);
      dest.oix = 0;
      dest.vid = m_vid;
    }

    m_sts = ldh_CreateObject(m_ldhses, &d[i].oid, d[i].name, d[i].cid, dest,
			     ldh_eDest_IntoLast);
    if (m_sts != d[i].sts) {
      m_log->log('E', "CreateObject", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, d[i].oid, ldh_eName_Hierarchy, oname, sizeof(oname), &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "CreateObject ldh_ObjidToName", d[i].name, m_sts);
	return;
      }
      if (strcmp(oname, name) != 0) {
	m_log->vlog('E', "CreateObject, wrong object %s != %s, idx %d", name, oname, i);
	return;
      }
      m_sts = check_object(d[i].oid);
      if (EVEN(m_sts)) {
	m_log->log('E', "CreateObject, check object", d[i].name, m_sts);
	return;
      }
    }
    if (i == 0)
      m_test = d[i].oid;
    else if (i == 1)
      m_test_h1 = d[i].oid;
    else if (i == 2)
      m_test_h2 = d[i].oid;
  }
  m_sts = ldh_DeleteObjectTree(m_ldhses, d[3].oid, 0);
  if (EVEN(m_sts)) {
    m_log->log('E', "CreateObject, ldh_DeleteObjectTree", "", m_sts);
    return;
  }

  m_log->log('S', "CreateObject", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName dest;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid oid;
} sDeleteObject;

void wa_ldhtest::DeleteObject(void)
{
  sDeleteObject d[] = {
    {"Del1", "Test-H2", pwr_cClass_PlantHier, LDH__SUCCESS, pwr_cNOid},
    {"Del2", "Test-H2", pwr_cClass_AArray100, LDH__SUCCESS, pwr_cNOid},
    {"Del3", "Test-H2", pwr_cClass_Pwrt02_Roller, LDH__SUCCESS, pwr_cNOid},
    {"Del4", "Test-H2", pwr_cClass_Pwrt02_Motor, LDH__SUCCESS, pwr_cNOid},
    {"Del5", "Test-H2", pwr_cClass_Pwrt02_Component, LDH__SUCCESS, pwr_cNOid}
  };
  

  pwr_tOName name;
  pwr_tOid dest;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (strcmp(d[i].dest, "") != 0) {
      strcpy(name, d[i].dest);
      strcat(name, "-");
      strcat(name, d[i].name);

      m_sts = ldh_NameToObjid(m_ldhses, &dest, d[i].dest);
      if (EVEN(m_sts)) {
	m_log->log('E', "DeleteObject, destination ldh_NameToObjid", d[i].dest, m_sts);
	return;
      }
    }
    else {
      strcpy(name, d[i].name);
      dest.oix = 0;
      dest.vid = m_vid;
    }

    m_sts = ldh_CreateObject(m_ldhses, &d[i].oid, d[i].name, d[i].cid, dest,
			     ldh_eDest_IntoLast);
    if (m_sts != d[i].sts) {
      m_log->log('E', "DeleteObject, ldh_CreateObject", d[i].name, m_sts);
      return;
    }
  }

  for (int i = (int)sizeof(d)/sizeof(d[0]) - 1; i >= 0 ; i--) {

    m_sts = ldh_DeleteObject(m_ldhses, d[i].oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "DeleteObject", m_sts);
      return;
    }
  }

  // Try to delete an nonexisting object
  pwr_tOid oid;
  oid.oix = 0xffffffff;
  oid.vid = m_vid;
  m_sts = ldh_DeleteObject(m_ldhses, oid);
  if (ODD(m_sts)) {
    m_log->log('E', "DeleteObject returned noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "DeleteObject wrong sts", m_sts);
    return;
  }

  m_log->log('S', "DeleteObject", LDH__SUCCESS);
}

void wa_ldhtest::DeleteObjectTree(void)
{
  pwr_tOName name;
  pwr_tAName cname;
  pwr_tOid oid, coid;

  // First create and then delete an object.
  strcpy(name, "TestObject");
  m_sts = ldh_CreateObject(m_ldhses, &oid, name, pwr_cClass_AArray100, m_test,
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "DeleteObjectTree, error in ldh_CreateObject", m_sts);
    return;
  }
  for (int i = 0; i < 10; i++) {
    sprintf(cname, "ch%d", i);
  
    m_sts = ldh_CreateObject(m_ldhses, &coid, cname, pwr_cClass_AArray100, oid, 
			     ldh_eDest_IntoLast);
    if (EVEN(m_sts)) {
      m_log->log('E', "DeleteObjectTree, error in ldh_CreateObject", m_sts);
      return;
    }
  }
  m_sts = ldh_DeleteObjectTree(m_ldhses, oid, 0);
  if (EVEN(m_sts)) {
    m_log->log('E', "DeleteObject", m_sts);
    return;
  }

  // Try to delete an nonexisting object
  oid.oix = 0xffffffff;
  oid.vid = m_vid;
  m_sts = ldh_DeleteObjectTree(m_ldhses, oid, 0);
  if (ODD(m_sts)) {
    m_log->log('E', "DeleteObjectTree returned noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "DeleteObjectTree wrong sts", m_sts);
    return;
  }

  m_log->log('S', "DeleteObjectTree", LDH__SUCCESS);
}

void wa_ldhtest::ChangeObjectName(void)
{
  pwr_tOName name;
  pwr_tOid oid, oid2;

  // First create and then rename an object.
  strcpy(name, "TestObject");
  m_sts = ldh_CreateObject(m_ldhses, &oid, name, pwr_cClass_AArray100, m_test, 
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "RenameObject, error in ldh_CreateObject", m_sts);
    return;
  }
  m_sts = ldh_ChangeObjectName(m_ldhses, oid, (char *)"NewName");
  if (EVEN(m_sts)) {
    m_log->log('E', "ChangeObjectName", m_sts);
    return;
  }
  m_sts = ldh_NameToObjid(m_ldhses, &oid, "Test-NewName");
  if (EVEN(m_sts)) {
    m_log->log('E', "ChangeObjectName, object not found", m_sts);
    return;
  }

  // Try to rename an nonexisting object
  oid2.oix = 0xffffffff;
  oid2.vid = m_vid;
  m_sts = ldh_ChangeObjectName(m_ldhses, oid2, (char *)"NewName2");
  if (ODD(m_sts)) {
    m_log->log('E', "ChangeObjectName returned noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "ChangeObjectName wrong sts", m_sts);
    return;
  }

  m_sts = ldh_DeleteObjectTree(m_ldhses, oid, 0);

  m_log->log('S', "ChangeObjectName", LDH__SUCCESS);
}

void wa_ldhtest::MoveObject(void)
{
  pwr_tOName name;
  pwr_tAName cname;
  pwr_tOid oid, oid2, p1, p2, c[100];

  // First create and then move an object.
  strcpy(name, "TestObject");
  m_sts = ldh_CreateObject(m_ldhses, &oid, name, pwr_cClass_AArray100, m_test_h1, 
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in ldh_CreateObject", m_sts);
    return;
  }

  m_sts = ldh_MoveObject(m_ldhses, oid, m_test_h2, ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject", m_sts);
    return;
  }

  m_sts = ldh_NameToObjid(m_ldhses, &oid, "Test-H2-TestObject");
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject object not found", m_sts);
    return;
  }
  m_sts = check_object(oid);

  // Try to move an nonexisting object
  oid2.oix = 0xffffffff;
  oid2.vid = m_vid;
  m_sts = ldh_MoveObject(m_ldhses, oid2, oid, ldh_eDest_IntoLast);
  if (ODD(m_sts)) {
    m_log->log('E', "MoveObject moved noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "MoveObject wrong sts", m_sts);
    return;
  }

  m_sts = ldh_DeleteObject(m_ldhses, oid);

  // Move several objects 
  strcpy(name, "P1");
  m_sts = ldh_CreateObject(m_ldhses, &p1, name, pwr_cClass_AArray100, m_test_h1, 
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in ldh_CreateObject", m_sts);
    return;
  }
  strcpy(name, "P2");
  m_sts = ldh_CreateObject(m_ldhses, &p2, name, pwr_cClass_AArray100, m_test_h1, 
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in ldh_CreateObject", m_sts);
    return;
  }
  for (int i = 0; i < 100; i++) {
    sprintf(cname, "c%d", i);
    m_sts = ldh_CreateObject(m_ldhses, &c[i], cname, pwr_cClass_AArray100, p1, 
			   ldh_eDest_IntoLast);
    if (EVEN(m_sts)) {
      m_log->log('E', "MoveObject, error in ldh_CreateObject", m_sts);
      return;
    }
  }
  for (int i = 0; i < 100; i++) {
    m_sts = ldh_MoveObject(m_ldhses, c[i], p2, ldh_eDest_IntoLast);
    if (EVEN(m_sts)) {
      m_log->log('E', "MoveObject, error in ldh_MoveObject", m_sts);
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
      m_log->log('E', "MoveObject, error in check_object", m_sts);
      return;
    }
  }
  
  m_sts = ldh_DeleteObjectTree(m_ldhses, p1, 0);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in DeleteObjectTree(p1)", m_sts);
    return;
  }
  m_sts = ldh_DeleteObjectTree(m_ldhses, p2, 0);
  if (EVEN(m_sts)) {
    m_log->log('E', "MoveObject, error in DeleteObjectTree(p2)", m_sts);
    return;
  }
  
  m_log->log('S', "MoveObject", LDH__SUCCESS);
}

void wa_ldhtest::GetRootList(void)
{
  pwr_tOName name;
  int size;

  m_sts = ldh_GetRootList(m_ldhses, &m_root);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetRootList", m_sts);
    return;
  }
  m_sts = ldh_ObjidToName(m_ldhses, m_root, ldh_eName_Hierarchy, name, sizeof(name), &size);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetRootList", "ObjidToName failed", m_sts);
    return;
  }
  if (strcmp(name, "RootObject") != 0) {
    m_log->log('E', "GetRootList", "Wrong root object");
    return;
  }
  m_log->log('S', "GetRootList", LDH__SUCCESS);
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

void wa_ldhtest::GetChild(void)
{
  sGetChild d[] = {
    {"RootObject", "Child1", pwr_cClass_PlantHier, LDH__SUCCESS, 0, pwr_cNOid, pwr_cNOid},
    {"RootObject-ChildLess", "", pwr_cClass_PlantHier, LDH__NO_CHILD, 0, pwr_cNOid, pwr_cNOid},
    {"ChTest", "Ch1", pwr_cClass_PlantHier, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1", "Ch2", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2", "Ch3", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3", "Ch4", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4", "Ch5", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5", "Ch6", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6", "Ch7", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7", "Ch8", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8", "Ch9", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9", "Ch10", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10", "Ch11", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10-Ch11", "Ch12", pwr_cClass_AArray100, LDH__SUCCESS, 1, pwr_cNOid, pwr_cNOid},
    {"ChTest-Ch1-Ch2-Ch3-Ch4-Ch5-Ch6-Ch7-Ch8-Ch9-Ch10-Ch11-Ch12", "", pwr_cClass_AArray100, LDH__NO_CHILD, 0, pwr_cNOid, pwr_cNOid}
  };
  pwr_tAName cname;
  pwr_tOid coid, poid, oid;

  oid.oix = 0;
  oid.vid = m_vid;
  m_sts = ldh_CreateObject(m_ldhses, &poid, "ChTest", pwr_cClass_PlantHier, oid, 
			   ldh_eDest_IntoLast);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetChild, ldh_CreateObject ChTest", m_sts);
    return;
  }
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &d[i].poid, d[i].pname);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, ldh_NameToObjid", d[i].pname, m_sts);
      return;
    }
    sprintf(cname, "%s-%s", d[i].pname, d[i].cname);
    if (d[i].create) {
      m_sts = ldh_CreateObject(m_ldhses, &d[i].coid, cname, d[i].cid, d[i].poid, 
			       ldh_eDest_IntoLast);
      if (m_sts != d[i].sts) {
	m_log->log('E', "GetChild, ldh_CreateObject", d[i].cname, m_sts);
	return;
      }
    }
    else if (strcmp(d[i].cname, "") != 0) {
      m_sts = ldh_NameToObjid(m_ldhses, &d[i].coid, cname);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetChild, ldh_NameToObjid", d[i].cname, m_sts);
	return;
      }
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_GetChild(m_ldhses ,d[i].poid, &coid);
    if (m_sts != d[i].sts) {
      m_log->vlog('E', "GetChild, wrong sts", d[i].pname, m_sts);
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
  coid.vid = m_vid;
  m_sts = ldh_GetChild(m_ldhses, coid, &coid);
  if (ODD(m_sts)) {
    m_log->log('E', "GetChild returned noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "GetChild wrong sts", m_sts);
    return;
  }

  //m_child = d[0].coid;
  m_sts = ldh_DeleteObjectTree(m_ldhses, poid, 0);

  m_log->log('S', "GetChild", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tStatus sts;
} sGetParent;

void wa_ldhtest::GetParent(void)
{
  sGetParent d[] = {
    {"RootObject", LDH__NO_PARENT},
    {"RootObject-Child1", LDH__SUCCESS},
    {"RootObject-ChildLess", LDH__SUCCESS}
  };
  pwr_tOName pname, name;
  pwr_tOid coid, poid;
  int size;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &coid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetChild, gdh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = ldh_GetParent(m_ldhses, coid, &poid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetParent, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, poid, ldh_eName_Hierarchy, pname, sizeof(pname), &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetChild, ldh_ObjidToName", d[i].name, m_sts);
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
  m_sts = ldh_GetParent(m_ldhses, poid, &poid);
  if (ODD(m_sts)) {
    m_log->log('E', "GetParent returned noexisting object");
    return;
  }
  else if (m_sts != LDH__NOSUCHOBJ) {
    m_log->log('E', "GetParent wrong sts", m_sts);
    return;
  }

  m_log->log('S', "GetParent", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName nextname;
  pwr_tStatus sts;
} sGetNextSibling;

void wa_ldhtest::GetNextSibling(void)
{
  sGetNextSibling d[] = {
    {"RootObject-Child1", "RootObject-Child2", LDH__SUCCESS},
    {"RootObject-Child2", "RootObject-Child3", LDH__SUCCESS},
    {"RootObject-Child3", "RootObject-Child4", LDH__SUCCESS},
    {"RootObject-Child4", "RootObject-ChildLess", LDH__SUCCESS},
    {"RootObject-ChildLess", "", LDH__NO_SIBLING}
  };
  pwr_tOid oid, nextoid, noid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextSibling, ldh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = ldh_GetNextSibling(m_ldhses, oid, &nextoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextSibling, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_NameToObjid(m_ldhses, &noid, d[i].nextname);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextSibling, ldh_NameToObjid", d[i].name, m_sts);
	return;
      }

      if (cdh_ObjidIsNotEqual(noid, nextoid)) {
	m_log->log('E', "GetNextSibling, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetNextSibling", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName previousname;
  pwr_tStatus sts;
} sGetPreviousSibling;

void wa_ldhtest::GetPreviousSibling(void)
{
  sGetPreviousSibling d[] = {
    {"RootObject-ChildLess", "RootObject-Child4", LDH__SUCCESS},
    {"RootObject-Child4", "RootObject-Child3", LDH__SUCCESS},
    {"RootObject-Child3", "RootObject-Child2", LDH__SUCCESS},
    {"RootObject-Child2", "RootObject-Child1", LDH__SUCCESS},
    {"RootObject-Child1", "", LDH__NOSUCHOBJ}
  };
  pwr_tOid oid, previousoid, poid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetPreviousSibling, ldh_NameToObjid", d[i].name, m_sts);
      return;
    }
    m_sts = ldh_GetPreviousSibling(m_ldhses, oid, &previousoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetPreviousSibling, wrong sts", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_NameToObjid(m_ldhses, &poid, d[i].previousname);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetPreviousSibling, ldh_NameToObjid", d[i].name, m_sts);
	return;
      }

      if (cdh_ObjidIsNotEqual(poid, previousoid)) {
	m_log->log('E', "GetPreviousSibling, wrong object", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetPreviousSibling", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  pwr_tOName result;
  pwr_tStatus sts;
} sNameToAttrRef;

void wa_ldhtest::NameToAttrRef(void)
{
  sNameToAttrRef d[] = {
    {"RootObject-Child1.Photo", "RootObject-Child1.Photo", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", "RootObject-Child1-P1.CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", "RootObject-Child1-A1.Value", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", "RootObject-Child1-A1.Value[99]", LDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  char *aname2;
  int size;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].aname, &aref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "NameToAttrRef, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_AttrRefToName(m_ldhses, &aref, cdh_mNName, &aname2, &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "NameToAttrRef, ldh_AttrRefToName", d[i].aname, m_sts);
	return;
      }
      if (strcmp(aname2, d[i].result) != 0) {
	m_log->vlog('E', "NameToAttrRef, %s != %s, idx %d", aname2, d[i].result, i);
	return;
      }
    }
  }

  m_log->log('S', "NameToAttrRef", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  int nametype;
  pwr_tOName result;
  pwr_tStatus sts;
} sAttrRefToName;

void wa_ldhtest::AttrRefToName(void)
{
  sAttrRefToName d[] = {
    {"RootObject-Child1.Photo", cdh_mNName, "RootObject-Child1.Photo", LDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_object, "Child1", LDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_attribute, ".Photo", LDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_object | cdh_mName_attribute, "Child1.Photo", LDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_volumeStrict, "VolPwrtest02:RootObject-Child1.Photo", LDH__SUCCESS},
    {"RootObject-Child1.Photo", cdh_mName_pathStrict, "RootObject-Child1.Photo", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mNName, "RootObject-Child1-P1.CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object, "P1", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_attribute, ".CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_object | cdh_mName_attribute, "P1.CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_volumeStrict, "VolPwrtest02:RootObject-Child1-P1.CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", cdh_mName_pathStrict, "RootObject-Child1-P1.CircuitBreaker.NotTripped", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mNName, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_object, "P1", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_attribute, ".CircuitBreaker.NotTripped.Photo", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_volumeStrict, "VolPwrtest02:RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", cdh_mName_pathStrict, "RootObject-Child1-P1.CircuitBreaker.NotTripped.Photo", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mNName, "RootObject-Child1-A1.Value", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_object, "A1", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_attribute, ".Value", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_volumeStrict, "VolPwrtest02:RootObject-Child1-A1.Value", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value", cdh_mName_pathStrict, "RootObject-Child1-A1.Value", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mNName, "RootObject-Child1-A1.Value[99]", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_object, "A1", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_attribute, ".Value[99]", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_object | cdh_mName_attribute | cdh_mName_index, "A1.Value[99]", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_volumeStrict, "VolPwrtest02:RootObject-Child1-A1.Value[99]", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", cdh_mName_pathStrict, "RootObject-Child1-A1.Value[99]", LDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  char *aname2;
  int size;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrRefToName, ldh_NameToAttrRef", m_sts);
      return;
    }

    m_sts = ldh_AttrRefToName(m_ldhses, &aref, d[i].nametype, &aname2, &size);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttrRefToName, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(aname2, d[i].result) != 0) {
	m_log->vlog('E', "AttrRefToName, %s != %s, idx %d", aname2, d[i].result, i);
	return;
      }
    }
  }

  m_log->log('S', "AttrRefToName", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  pwr_tOName oname;
  pwr_tStatus sts;
} sAttrArefToObjectAref;

void wa_ldhtest::AttrArefToObjectAref(void)
{
  sAttrArefToObjectAref d[] = {
    {"RootObject-Child1.Photo", "RootObject-Child1", LDH__SUCCESS},
    {"RootObject-Child1-A1.Value[99]", "RootObject-Child1-A1", LDH__SUCCESS},
    {"RootObject-Child1-P1.CircuitBreaker.NotTripped", "RootObject-Child1-P1.CircuitBreaker", LDH__SUCCESS}
  };


  pwr_tAttrRef aref, oaref;
  char *aname;
  int size;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].aname, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrArefToObjectAref", "ldh_NameToAttrRef", m_sts);
      return;
    }

    m_sts = ldh_AttrArefToObjectAref(m_ldhses, &aref, &oaref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttrArefToObjetAref, wrong sts", d[i].aname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_AttrRefToName(m_ldhses, &oaref, cdh_mName_pathStrict, &aname, &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "AttrArefToObjectAref, ldh_AttrRefToName", m_sts);
	return;
      }
      if (strcmp(aname, d[i].oname) != 0) {
	m_log->vlog('E', "AttrArefToObjectAref wrong name, %s != %s", aname, d[i].oname);
	return;
      }
    }
  }

  m_log->log('S', "AttrArefToObjectAref", LDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tOName name;
  pwr_tStatus sts;
} sGetClassList;

void wa_ldhtest::GetClassList(void)
{
  sGetClassList d[] = {
    {pwr_cClass_Node, "Nodes-Pwrtest02", LDH__SUCCESS},
    {pwr_cClass_Security, "Nodes-Pwrtest02-Security", LDH__SUCCESS},
    {pwr_cClass_OpPlace, "Nodes-Pwrtest02-OpPlaces-Op", LDH__SUCCESS},
    {pwr_cClass_plc, "PlcTest-H1-Plc", LDH__SUCCESS},
    {pwr_cClass_and, "PlcTest-H1-Plc-W-And0", LDH__SUCCESS},
    {pwr_cClass_Dv, "Test02-First-Dv1", LDH__SUCCESS},
    {pwr_cClass_Component, "", LDH__NOSUCHOBJ}
  };
  pwr_tOid oid;
  pwr_tOName oname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_GetClassList(m_ldhses, d[i].cid, &oid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetClassList", d[i].name, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, oid, ldh_eName_Hierarchy, oname, sizeof(oname), &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetClassList ldh_ObjidToName", d[i].name, m_sts);
	return;
      }
      if (strcmp(oname, d[i].name) != 0) {
	m_log->vlog('E', "GetClassList, wrong object, %s != %s", d[i].name, oname);
	return;
      }
    }
  }

  m_log->log('S', "GetClassList", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName next;
  pwr_tStatus sts;
} sGetNextObject;

void wa_ldhtest::GetNextObject(void)
{
  sGetNextObject d[] = {
    {"Test02-First-Dv1", "Test02-First-Dv2", LDH__SUCCESS},
    {"Test02-First-Dv2", "Test02-First-Dv3", LDH__SUCCESS},
    {"Test02-First-Dv3", "Test02-First-Dv4", LDH__SUCCESS},
    {"Test02-First-Aggr1", "Test02-First-Aggr2", LDH__SUCCESS},
    {"Test02-First-Aggr2", "Test02-First-Aggr3", LDH__SUCCESS},
    {"Test02-First-Aggr3", "Test02-First-Aggr4", LDH__SUCCESS},
    {"Test02-First-Aggr4", "", LDH__NOSUCHOBJ}
  };
  pwr_tOid oid, nextoid;
  pwr_tOName nextname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextObject, ldh_NameToObjid", d[i].next, m_sts);
      return;
    }

    m_sts = ldh_GetNextObject(m_ldhses, oid, &nextoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextObject", d[i].next, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, nextoid, ldh_eName_Hierarchy, nextname, sizeof(nextname), &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextObject ldh_ObjidToName", d[i].next, m_sts);
	return;
      }
      if (strcmp(nextname, d[i].next) != 0) {
	m_log->vlog('E', "GetNextObject, wrong object, %s != %s", d[i].next, nextname);
	return;
      }
    }
  }

  m_log->log('S', "GetNextObject", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tOName previous;
  pwr_tStatus sts;
} sGetPreviousObject;

#if 0
void wa_ldhtest::GetPreviousObject(void)
{
  sGetPreviousObject d[] = {
    {"Test02-First-Dv4", "Test02-First-Dv3", LDH__SUCCESS},
    {"Test02-First-Dv3", "Test02-First-Dv2", LDH__SUCCESS},
    {"Test02-First-Dv1", "", LDH__NOSUCHOBJ},
    {"Test02-First-Aggr4", "Test02-First-Aggr3", LDH__SUCCESS},
    {"Test02-First-Aggr3", "Test02-First-Aggr2", LDH__SUCCESS},
    {"Test02-First-Aggr2", "Test02-First-Aggr1", LDH__SUCCESS},
    {"Test02-First-Aggr1", "", LDH__NOSUCHOBJ}
  };
  pwr_tOid oid, previousoid;
  pwr_tOName previousname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetPreviousObject, ldh_NameToObjid", d[i].previous, m_sts);
      return;
    }

    m_sts = ldh_GetPreviousObject(m_ldhses, oid, &previousoid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetPreviousObject", d[i].previous, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_ObjidToName(m_ldhses, previousoid, ldh_eName_Hierarchy, previousname, sizeof(previousname), &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetPreviousObject ldh_ObjidToName", d[i].previous, m_sts);
	return;
      }
      if (strcmp(previousname, d[i].previous) != 0) {
	m_log->vlog('E', "GetPreviousObject, wrong object, %s != %s", d[i].previous, previousname);
	return;
      }
    }
  }

  m_log->log('S', "GetPreviousObject", LDH__SUCCESS);
}
#endif

typedef struct {
  pwr_tCid cid;
  pwr_tOName name;
  pwr_tOName next;
  pwr_tStatus sts;
} sGetNextAttrRef;

void wa_ldhtest::GetNextAttrRef(void)
{
  sGetNextAttrRef d[] = {
    {pwr_cClass_Dv, "Test02-First-Dv1", "Test02-First-Dv2", LDH__SUCCESS},
    {pwr_cClass_Dv, "Test02-First-Dv2", "Test02-First-Dv3", LDH__SUCCESS},
    {pwr_cClass_Dv, "Test02-First-Dv3", "Test02-First-Dv4", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-Comp1", "Test02-First-C1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-C1.Super", "Test02-First-M1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-M1.Super", "Test02-First-M1.Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-M1.Contactor1.Super", "Test02-First-R1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Super", "Test02-First-R1.Motors[0].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[0].Super", "Test02-First-R1.Motors[0].Contactor2.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[0].Contactor2.Super", "Test02-First-R1.Motors[1].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[1].Super", "Test02-First-R1.Motors[1].Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[1].Contactor1.Super", "Test02-First-R1.Motors[2].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[2].Super", "Test02-First-R1.Motors[2].Contactor2.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[2].Contactor2.Super", "Test02-First-R1.Motors[3].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[3].Super", "Test02-First-R1.Motors[3].Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[3].Contactor1.Super", "Test02-First-R1.Motors[4].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[4].Super", "Test02-First-R1.Motors[4].Contactor2.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[4].Contactor2.Super", "Test02-First-R1.Motors[5].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[5].Super", "Test02-First-R1.Motors[5].Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[5].Contactor1.Super", "Test02-First-R1.Motors[6].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[6].Super", "Test02-First-R1.Motors[6].Contactor2.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[6].Contactor2.Super", "Test02-First-R1.Motors[7].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[7].Super", "Test02-First-R1.Motors[7].Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[7].Contactor1.Super", "Test02-First-R1.Motors[8].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[8].Super", "Test02-First-R1.Motors[8].Contactor1.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[8].Contactor1.Super", "Test02-First-R1.Motors[9].Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[9].Super", "Test02-First-R1.Motors[9].Contactor2.Super", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Test02-First-R1.Motors[9].Contactor2.Super", "", LDH__NONEXT}
  };
  pwr_tAttrRef aref, nextaref;
  char *nextname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextAttrRef, ldh_NameToAttrRef", d[i].next, m_sts);
      return;
    }

    m_sts = ldh_GetNextAttrRef(m_ldhses, d[i].cid, &aref, &nextaref);
    if (m_sts != d[i].sts && m_sts != TREE__FOUND) {
      m_log->log('E', "GetNextAttrRef", d[i].next, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_AttrRefToName(m_ldhses, &nextaref, cdh_mNName, &nextname, &size);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextAttrRef gdh_AttrrefToName", d[i].next, m_sts);
	return;
      }
      if (strcmp(nextname, d[i].next) != 0) {
	m_log->vlog('E', "GetNextAttrRef, wrong object, %s != %s", d[i].next, nextname);
	return;
      }
    }
  }

  m_log->log('S', "GetNextAttrRef", LDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tObjName cname;
  pwr_tStatus sts;
} sClassNameToId;

void wa_ldhtest::ClassNameToId(void)
{
  sClassNameToId d[] = {
    {pwr_cClass_Node, "$Node", LDH__SUCCESS},
    {pwr_cClass_Security, "$Security", LDH__SUCCESS},
    {pwr_cClass_OpPlace, "OpPlace", LDH__SUCCESS},
    {pwr_cClass_plc, "PlcPgm", LDH__SUCCESS},
    {pwr_cClass_and, "And", LDH__SUCCESS},
    {pwr_cClass_Dv, "Dv", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Pwrt02_Component", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Motor, "Pwrt02_Motor", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Roller, "Pwrt02_Roller", LDH__SUCCESS},
    {pwr_cClass_Component, "NoValidClassName", LDH__NOCLASS}
  };
  pwr_tCid cid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_ClassNameToId(m_ldhses, &cid, d[i].cname);
    if (m_sts != d[i].sts) {
      m_log->log('E', "ClassNameToId", d[i].cname, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].cid != cid) {
	m_log->vlog('E', "ClassNameToId, wrong cid %s, %d != %d", d[i].cname, d[i].cid, cid);
	return;
      }
    }
  }

  m_log->log('S', "ClassNameToId", LDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tObjName cname;
  pwr_tStatus sts;
} sClassIdToName;

void wa_ldhtest::ClassIdToName(void)
{
  sClassIdToName d[] = {
    {pwr_cClass_Node, "$Node", LDH__SUCCESS},
    {pwr_cClass_Security, "$Security", LDH__SUCCESS},
    {pwr_cClass_OpPlace, "OpPlace", LDH__SUCCESS},
    {pwr_cClass_plc, "PlcPgm", LDH__SUCCESS},
    {pwr_cClass_and, "And", LDH__SUCCESS},
    {pwr_cClass_Dv, "Dv", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, "Pwrt02_Component", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Motor, "Pwrt02_Motor", LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Roller, "Pwrt02_Roller", LDH__SUCCESS},
    {0, "NoValidClassName", LDH__NOCLASS}
  };
  pwr_tObjName cname;
  int size;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_ClassIdToName(m_ldhses, d[i].cid, cname, sizeof(cname), &size);
    if (m_sts != d[i].sts) {
      m_log->vlog('E', "ClassIdToName, idx %d, sts %d", i, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(d[i].cname, cname) != 0) {
	m_log->vlog('E', "ClassIdToName, wrong name, %s != %s", d[i].cname, cname);
	return;
      }
    }
  }

  m_log->log('S', "ClassIdToName", LDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tCid supercid;
  pwr_tStatus sts;
} sGetSuperClass;

void wa_ldhtest::GetSuperClass(void)
{
  sGetSuperClass d[] = {
    {pwr_cClass_BasePumpAggr, pwr_cClass_BaseMotorAggr, LDH__SUCCESS},
    {pwr_cClass_BaseMotorAggr, pwr_cClass_Aggregate, LDH__SUCCESS},
    {pwr_cClass_Aggregate, 0, LDH__NOCLASS},
    {pwr_cClass_BaseTempSensor, pwr_cClass_BaseSensor, LDH__SUCCESS},
    {pwr_cClass_BaseSensor, pwr_cClass_Component, LDH__SUCCESS},
    {pwr_cClass_Component, 0, LDH__NOCLASS},
    {pwr_cClass_Pwrt02_Roller, pwr_cClass_Pwrt02_Component, LDH__SUCCESS},
    {pwr_cClass_Pwrt02_Component, 0, LDH__NOCLASS},
    {0xffffffff, 0, LDH__NOCLASS}
  };
  pwr_tCid supercid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_GetSuperClass(m_ldhses, d[i].cid, &supercid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetSuperClass", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].supercid != supercid) {
	m_log->vlog('E', "GetSuperClass, wrong cid, idx %d", i);
	return;
      }
    }
  }

  m_log->log('S', "GetSuperClass", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName tname;
  pwr_tObjName name1;
  pwr_tInt32 value1;
  pwr_tObjName name2;
  pwr_tInt32 value2;
  pwr_tStatus sts;
} sGetEnumValueDef;

void wa_ldhtest::GetEnumValueDef(void)
{
  sGetEnumValueDef d[] = {
    {"pwrb:Type-YesNoEnum", "No", 0, "Yes", 1, LDH__SUCCESS},
    {"pwrb:Type-ColorThemeEnum", "Standard", 0, "Custom", 100, LDH__SUCCESS},
    {"CVolPwrtest02:Type-Pwrt02_MotorEnum", "Off", 0, "On", 1, LDH__SUCCESS}
  };

  pwr_tOid oid;
  ldh_sValueDef *vd;
  pwr_tTid tid;
  int rows;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].tname);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetEnumValueDef", "ldh_NameToObjid", m_sts);
      return;
    }
    tid = cdh_TypeObjidToId(oid);
    m_sts = ldh_GetEnumValueDef(m_ldhses, tid, &vd, &rows);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetEnumValueDef", "wrong status", m_sts);
      if (ODD(m_sts))
	free(vd);
      return;
    }
    if (ODD(m_sts)) {
      if (strcmp(vd[0].Name, d[i].name1) != 0 ||
	  vd[0].Value.Value != d[i].value1 ||
	  strcmp(vd[rows-1].Name, d[i].name2) != 0 ||
	  vd[rows-1].Value.Value != d[i].value2) {
	m_log->log('E', "GetEnumValueDef", "name of value doesn't match");
	free(vd);
	return;
      }
      free(vd);
    }
  }
  m_log->log('S', "GetEnumValueDef", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName tname;
  pwr_tObjName name1;
  pwr_tUInt32 value1;
  pwr_tObjName name2;
  pwr_tUInt32 value2;
  pwr_tStatus sts;
} sGetMaskBitDef;

void wa_ldhtest::GetMaskBitDef(void)
{
  sGetMaskBitDef d[] = {
    {"pwrb:Type-CardMask1_1", "Channel1", 1, "Channel16", 32768, LDH__SUCCESS},
    {"pwrb:Type-CardMask2_1", "Channel17", 1, "Channel32", 32768, LDH__SUCCESS},
    {"pwrb:Type-PidModeMask", "Manual", 1, "Cascade", 4, LDH__SUCCESS},
    {"CVolPwrtest02:Type-Pwrt02_MotorMask", "Off", 1, "Running", 8, LDH__SUCCESS}
  };

  pwr_tOid oid;
  ldh_sBitDef *bd;
  pwr_tTid tid;
  int rows;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].tname);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetMaskBitDef", "ldh_NameToObjid", m_sts);
      return;
    }
    tid = cdh_TypeObjidToId(oid);
    m_sts = ldh_GetMaskBitDef(m_ldhses, tid, &bd, &rows);
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
	m_log->log('E', "GetMaskBitDef", "name or value doesn't match");
	free(bd);
	return;
      }
      free(bd);
    }
  }
  m_log->log('S', "GetMaskBitDef", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetObjectClass;

void wa_ldhtest::GetObjectClass(void)
{
  sGetObjectClass d[] = {
    {"Test02-Ldh-A1", pwr_cClass_AArray100, LDH__SUCCESS},
    {"Test02-Ldh-P1", pwr_cClass_BaseFcPPO3PumpAggr, LDH__SUCCESS},
    {"Test02-Ldh-Av1", pwr_cClass_Av, LDH__SUCCESS},
    {"Test02-Ldh-Dv1", pwr_cClass_Dv, LDH__SUCCESS},
    {"VolPwrTest02:", pwr_cClass_RootVolume, LDH__SUCCESS},
    {"Test02", pwr_cClass_PlantHier, LDH__SUCCESS}
  };
 
  pwr_tOid oid;
  pwr_tCid cid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].name);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectClass, ldh_NameToObjid", d[i].name, m_sts);
      return;
    }

    m_sts = ldh_GetObjectClass(m_ldhses, oid, &cid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetObjectClass", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].cid != cid) {
	m_log->vlog('E', "GetObjectClass, object class doesn't match, idx %d", i);
	return;
      }
    }
  }

  m_log->log('S', "GetObjectClass", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tTid tid;
  pwr_tOName tname;
  pwr_tStatus sts;
} sGetAttrRefTid;

void wa_ldhtest::GetAttrRefTid(void)
{
  sGetAttrRefTid d[] = {
    {"Test02-Ldh-A1.Value", pwr_eType_Float32, "", LDH__SUCCESS},
    {"Test02-Ldh-A1.Value[0]", pwr_eType_Float32, "", LDH__SUCCESS},
    {"Test02.Description", 0, "pwrs:Type-$String80", LDH__SUCCESS},
    {"Test02-Ldh-P1.CircuitBreaker.NotTripped.ActualValue", pwr_eType_Boolean, "", LDH__SUCCESS},
    {"Test02-Ldh-Av1.ActualValue", pwr_eType_Float32, "", LDH__SUCCESS},
    {"Test02-Ldh-Dv1.ActualValue", pwr_eType_Boolean, "", LDH__SUCCESS},
    {"Test02-Ldh-A1", pwr_cClass_AArray100, "", LDH__SUCCESS},
    {"Test02-Ldh-P1", pwr_cClass_BaseFcPPO3PumpAggr, "", LDH__SUCCESS},
    {"Test02-Ldh-Av1", pwr_cClass_Av, "", LDH__SUCCESS},
    {"Test02-Ldh-Dv1", pwr_cClass_Dv, "", LDH__SUCCESS},
    {"VolPwrTest02:", pwr_cClass_RootVolume, "", LDH__SUCCESS},
    {"Test02", pwr_cClass_PlantHier, "", LDH__SUCCESS}
  };
 
  pwr_tAttrRef aref;
  pwr_tTid tid, dtid;
  pwr_tOid toid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].name, &aref);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetAttrRefTid, ldh_NameToAttrref", d[i].name, m_sts);
      return;
    }

    if (d[i].tid == 0) {
      m_sts = ldh_NameToObjid(m_ldhses, &toid, d[i].tname);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetAttrRefTid, ldh_NameToObjid", d[i].name, m_sts);
	return;
      }
      dtid = cdh_TypeObjidToId(toid);
    }
    else
      dtid = d[i].tid;

    m_sts = ldh_GetAttrRefTid(m_ldhses, &aref, &tid);
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

  m_log->log('S', "GetAttrRefTid", LDH__SUCCESS);
}

void wa_ldhtest::GetVolumeList(void)
{
  pwr_tObjName name;
  pwr_tVid vid;
  int size;

  m_sts = ldh_GetVolumeList(m_ldhwb, &vid);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetVolumeList", m_sts);
    return;
  }
  m_sts = ldh_VolumeIdToName(m_ldhwb, vid, name, sizeof(name), &size);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetVolumeList, ldh_VolumeIdToName", m_sts);
    return;
  }
  if (strcmp(name, "VolPwrtest02") != 0) {
    m_log->log('E', "GetVolumeList, wrong volume", name);
    return;
  }

  m_log->log('S', "GetVolumeList", LDH__SUCCESS);
}

typedef struct {
  char vid[20];
  char nextvid[20];
  pwr_tStatus sts;
} sGetNextVolume;

void wa_ldhtest::GetNextVolume(void)
{
  sGetNextVolume d[] = {
    {"_V0.254.254.210", "_V0.0.0.1", LDH__SUCCESS},
    {"_V0.0.0.1", "_V0.0.0.2", LDH__SUCCESS},
    {"_V0.0.1.4", "_V0.0.240.210", LDH__SUCCESS},
    {"_V1.254.254.203", "", LDH__NOSUCHVOL}
  };
  pwr_tVid vid, nextvid, nvid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetNextVolume, cdh_StringToVolumeId", d[i].vid, m_sts);
      return;
    }
    if (strcmp(d[i].nextvid, "") != 0) {
      m_sts = cdh_StringToVolumeId(d[i].nextvid, &nextvid);
      if (EVEN(m_sts)) {
	m_log->log('E', "GetNextVolume, cdh_StringToVolumeId", d[i].nextvid, m_sts);
	return;
      }
    }

    m_sts = ldh_GetNextVolume(m_ldhwb, vid, &nvid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetNextVolume", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (nvid != nextvid) {
	char nvid_str[40];
	char nextvid_str[40];

	m_log->vlog('E', "GetNextVolume, wrong volume %s != %s, idx %d", 
		    cdh_VolumeIdToString(nvid_str, sizeof(nvid_str), nvid, 0, 0), 
		    cdh_VolumeIdToString(nextvid_str, sizeof(nextvid_str), nextvid, 0, 0), i);
	return;
      }
    }
  }

  m_log->log('S', "GetNextVolume", LDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vid[20];
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetVolumeClass;

void wa_ldhtest::GetVolumeClass(void)
{
  sGetVolumeClass d[] = {
    {"VolPwrTest02", "_V0.254.254.210", pwr_eClass_RootVolume, LDH__SUCCESS},
    {"rt", "_V254.254.254.245", pwr_eClass_SharedVolume, LDH__SUCCESS},
    {"pwrs", "_V0.0.0.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"pwrb", "_V0.0.0.2", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Simul", "_V0.0.0.8", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"BaseComponent", "_V0.0.0.10", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"NMps", "_V0.0.1.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Profibus", "_V0.0.250.7", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"CVolPwrtest02", "_V0.0.240.210", pwr_eClass_ClassVolume, LDH__SUCCESS}
  };
  pwr_tVid vid;
  pwr_tCid cid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetVolumeClass, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = ldh_GetVolumeClass(m_ldhwb, vid, &cid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetVolumeClass", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (d[i].cid != cid) {
	m_log->vlog('E', "GetVolumeClass, class doesn't match %s, idx %d", d[i].name, i);
	return;
      }
    }
  }

  m_log->log('S', "GetVolumeClass", LDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vid[20];
  pwr_tCid cid;
  pwr_tStatus sts;
} sGetVidInfo;

void wa_ldhtest::GetVidInfo(void)
{
  sGetVidInfo d[] = {
    {"VolPwrTest02", "_V0.254.254.210", pwr_eClass_RootVolume, LDH__SUCCESS},
    {"rt", "_V254.254.254.245", pwr_eClass_SharedVolume, LDH__SUCCESS},
    {"pwrs", "_V0.0.0.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"pwrb", "_V0.0.0.2", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Simul", "_V0.0.0.8", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"BaseComponent", "_V0.0.0.10", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"NMps", "_V0.0.1.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Profibus", "_V0.0.250.7", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"CVolPwrtest02", "_V0.0.240.210", pwr_eClass_ClassVolume, LDH__SUCCESS}
  };
  pwr_tVid vid;
  ldh_sVolumeInfo info;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetVidInfo, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = ldh_GetVidInfo(m_ldhwb, vid, &info);
    if (m_sts != d[i].sts) {
      m_log->log('E', "GetVidInfo", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (vid != info.Volume ||
	  d[i].cid != info.Class) {
	m_log->log('E', "GetVidInfo, data doesn't match", d[i].name);
	return;
      }
    }
  }

  m_log->log('S', "GetVidInfo", LDH__SUCCESS);
}

void wa_ldhtest::GetVolumeInfo(void)
{
  ldh_sVolumeInfo info;
  
  m_sts = ldh_GetVolumeInfo(m_volctx, &info);
  if (EVEN(m_sts)) {
    m_log->log('E', "GetVolumeInfo", m_sts);
    return;
  }

  if (info.Volume != m_vid ||
      info.Class != pwr_eClass_RootVolume) {
    m_log->log('E', "GetVolumeInfo, data doesn't match");
    return;
  }

  m_log->log('S', "GetVolumeInfo", LDH__SUCCESS);
}


void wa_ldhtest::Close(void)
{
  m_sts = ldh_DetachVolume(m_ldhwb, m_volctx);
  if (EVEN(m_sts)) {
    m_log->log('F', "Close, ldh_DetachVolume", m_sts);
    exit(0);
  }

  m_sts = ldh_CloseSession(m_ldhses);
  if (EVEN(m_sts)) {
    m_log->log('F', "Close, ldh_CloseSession", m_sts);
    exit(0);
  }

  m_log->log('S', "Close", LDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vid[20];
  pwr_tCid cid;
  pwr_tStatus sts;
} sAttachVolume;

void wa_ldhtest::AttachVolume(void)
{
  sAttachVolume d[] = {
    {"VolPwrTest02", "_V0.254.254.210", pwr_eClass_RootVolume, LDH__SUCCESS},
    {"rt", "_V254.254.254.245", pwr_eClass_SharedVolume, LDH__SUCCESS},
    {"pwrs", "_V0.0.0.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"pwrb", "_V0.0.0.2", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Simul", "_V0.0.0.8", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"BaseComponent", "_V0.0.0.10", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"NMps", "_V0.0.1.1", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"Profibus", "_V0.0.250.7", pwr_eClass_ClassVolume, LDH__SUCCESS},
    {"CVolPwrtest02", "_V0.0.240.210", pwr_eClass_ClassVolume, LDH__SUCCESS}
  };
  pwr_tVid vid;
  ldh_tVolContext volctx;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vid, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "AttachVolume, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = ldh_AttachVolume(m_ldhwb, vid, &volctx);
    if (m_sts != d[i].sts) {
      m_log->log('E', "AttachVolume", d[i].vid, m_sts);
      return;
    }
    if (ODD(m_sts)) {
      m_sts = ldh_DetachVolume(m_ldhwb, volctx);
      if (EVEN(m_sts)) {
	m_log->log('E', "AttachVolume, ldh_DetachVolume", d[i].vid, m_sts);
	return;
      }
    }
  }

  m_log->log('S', "AttachVolume", LDH__SUCCESS);
}

void wa_ldhtest::OpenWB(void)
{
  m_sts = ldh_OpenWB(&m_ldhwb, m_vname, 0);
  if (EVEN(m_sts)) {
    m_log->log('F', "Init, ldh_OpenWB", m_sts);
    delete this;
    exit(0);
  }
  m_log->log('S', "OpenWB", LDH__SUCCESS);
}

typedef struct {
  ldh_eAccess access;
  ldh_eUtility utility;
} sOpenSession;

void wa_ldhtest::OpenSession(void)
{
  sOpenSession d[] = {
    {ldh_eAccess_ReadWrite, ldh_eUtility_Pwr},
    {ldh_eAccess_ReadOnly, ldh_eUtility_Pwr},
    {ldh_eAccess_SharedReadWrite, ldh_eUtility_Pwr}
  };
  pwr_tVid vid;
  ldh_tVolContext volctx;
  ldh_tSession ldhses;
  char vidstr[20] = "_V0.254.254.210";
  
  m_sts = cdh_StringToVolumeId(vidstr, &vid);
  if (EVEN(m_sts)) {
    m_log->log('E', "OpenSession, cdh_StringToVolumeId", m_sts);
    return;
  }

  m_sts = ldh_AttachVolume(m_ldhwb, vid, &volctx);
  if (EVEN(m_sts)) {
    m_log->log('E', "OpenSession, ldh_AttachVolume", m_sts);
    return;
  }
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_OpenSession(&ldhses, volctx, d[i].access, d[i].utility);
    if (EVEN(m_sts)) {
      m_log->log('E', "OpenSession, ldh_AttachVolume", m_sts);
      return;
    }

    m_sts = ldh_CloseSession(ldhses);
    if (EVEN(m_sts)) {
      m_log->log('E', "OpenSession, ldh_CloseSession", m_sts);
      return;
    }
  }

  m_sts = ldh_DetachVolume(m_ldhwb, volctx);
  if (EVEN(m_sts)) {
    m_log->log('E', "OpenSession, ldh_DetachVolume", m_sts);
    return;
  }

  m_log->log('S', "OpenSession", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName src;
  pwr_tOName dest;
  ldh_eDest dtype;
  pwr_tObjName name;
  pwr_tStatus sts;
  pwr_tOid oid;
} sCopyObject;

void wa_ldhtest::CopyObject(void)
{
  sCopyObject d[] = {
    {"Test02-First", "Test02-First", ldh_eDest_IntoLast, "Tmp", LDH__SUCCESS},
    {"Test02-First-Dv1", "Test02-First-Tmp", ldh_eDest_IntoFirst, "Dv1", LDH__SUCCESS},
    {"Test02-First-Dv2", "Test02-First-Tmp", ldh_eDest_IntoLast, "Dv2", LDH__SUCCESS},
    {"Test02-First-Aggr1", "Test02-First-Tmp", ldh_eDest_IntoLast, "Aggr1", LDH__SUCCESS},
    {"Test02-First-Aggr2", "Test02-First-Tmp", ldh_eDest_IntoLast, "Aggr2", LDH__SUCCESS},
    {"Test02-First-R1", "Test02-First-Tmp", ldh_eDest_IntoLast, "R1", LDH__SUCCESS},
    {"Test02-First-C1", "Test02-First-Tmp", ldh_eDest_IntoLast, "C1", LDH__SUCCESS},
    {"Test02-First-M1", "Test02-First-Tmp", ldh_eDest_IntoLast, "M1", LDH__SUCCESS}
  };

  pwr_tOid soid, doid;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &soid, d[i].src);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObject, ldh_NameToObjid", d[i].src, m_sts);
      return;
    }

    m_sts = ldh_NameToObjid(m_ldhses, &doid, d[i].dest);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObject, ldh_NameToObjid", d[i].dest, m_sts);
      return;
    }

    m_sts = ldh_CopyObject(m_ldhses, &d[i].oid, d[i].name, soid, doid, d[i].dtype);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObject", m_sts);
      return;
    }
    
    m_sts = check_object(d[i].oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObject, error in check_object", m_sts);
      return;
    }
  }

  m_sts = ldh_DeleteObjectTree(m_ldhses, d[0].oid, 0);
  if (EVEN(m_sts)) {
    m_log->log('E', "CopyObject, error in DeleteObjectTree", m_sts);
    return;
  }

  m_log->log('S', "CopyObject", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName src;
  pwr_tOName dest;
  ldh_eDest dtype;
  pwr_tOid oid;
  pwr_tStatus sts;
} sCopyObjectTrees;

void wa_ldhtest::CopyObjectTrees(void)
{
  sCopyObjectTrees d[] = {
    {"Test02-First", "Test02-Ldh", ldh_eDest_IntoLast, LDH__SUCCESS},
    {"Test02-First", "Test02-Ldh", ldh_eDest_IntoFirst, LDH__SUCCESS},
    {"Test02-First", "Test02-Ldh-P1", ldh_eDest_After, LDH__SUCCESS},
    {"Test02-First", "Test02-Ldh-P1", ldh_eDest_Before, LDH__SUCCESS}
  };

  pwr_tAttrRef saref[2];
  pwr_tOid soid, doid;
  pwr_tObjName sname;
  pwr_tOName dname;
  char *s;

  memset(saref, 0, sizeof(saref));

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToObjid(m_ldhses, &soid, d[i].src);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObject, src ldh_NameToObjid", d[i].src, m_sts);
      return;
    }
    saref[0] = cdh_ObjidToAref(soid);

    m_sts = ldh_NameToObjid(m_ldhses, &doid, d[i].dest);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObjectTrees, dest ldh_NameToObjid", d[i].dest, m_sts);
      return;
    }

    m_sts = ldh_CopyObjectTrees(m_ldhses, saref, doid, d[i].dtype, 0, 1, 0, 0);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObjectTrees", m_sts);
      return;
    }    

    cdh_CutNameSegments(sname, d[i].src, 1);
    strcpy(dname, d[i].dest);
    switch (d[i].dtype) {
    case ldh_eDest_IntoFirst:
    case ldh_eDest_IntoLast:
      break;
    case ldh_eDest_After:
    case ldh_eDest_Before:
      s = strrchr(dname, '-');
      if (s)
	*s = 0;
      break;
    default:
      ;
    }
    strcat(dname, "-");
    strcat(dname, sname);

    m_sts = ldh_NameToObjid(m_ldhses, &d[i].oid, dname);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObjectTrees, tree ldh_NameToObjid", m_sts);
      return;
    }
    m_sts = ldh_DeleteObjectTree(m_ldhses, d[i].oid, 0);
    if (EVEN(m_sts)) {
      m_log->log('E', "CopyObjectTrees, error in DeleteObjectTree", m_sts);
      return;
    }
  }


  m_log->log('S', "CopyObjectTrees", LDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  char vidstr[20];
  pwr_tCid vclass;
  ldh_eVolRep volrep;
  pwr_tStatus sts;
} sCreateVolume;

void wa_ldhtest::CreateVolume(void)
{
  sCreateVolume d[] = {
    {"CVolTest1", "_V0.0.44.44", pwr_eClass_ClassVolume, ldh_eVolRep_Wbl, LDH__NOSUCHVOL} // Error status for not configured volume?
  };

  pwr_tVid vid;
  ldh_tSession ldhses;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].vidstr, &vid);
    if (EVEN(m_sts)) {
      m_log->log('E', "CreateVolume, cdh_StringToVolumeId", d[i].name, m_sts);
      return;
    }

    m_sts = ldh_CreateVolume(m_ldhwb, &ldhses, vid, d[i].name, d[i].vclass,
			     d[i].volrep, 0);
    if (m_sts != d[i].sts) {
      m_log->log('E', "CreateVolume", m_sts);
      return;
    }

    m_sts = ldh_DeleteVolume(m_ldhwb, vid); // Does nothing
    if (EVEN(m_sts)) {
      m_log->log('E', "CreateVolume, ldh_DeleteVolume", m_sts);
      return;
    }

  }
  m_log->log('S', "CreateVolume", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  int valtype;
  int ival;
  float fval;
  pwr_tString80 sval;
  pwr_tStatus sts;
} sGetObjectPar;

void wa_ldhtest::GetObjectPar(void)
{
  sGetObjectPar d[] = {
    {"Test02-Pwrwb-Av1.InitialValue", 2, 0, 22.22, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Dv1.InitialValue", 1, 0, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Dv2.InitialValue", 1, 1, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-L1.Value.InitialValue", 2, 0, 33.33, "", LDH__SUCCESS},
    {"Test02-Pwrwb-L1.LimitHH.Limit", 2, 0, 94, "", LDH__SUCCESS},
    {"Test02-Pwrwb-L1.Description", 3, 0, 0, "BaseLevelSensor used for python tests", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Boolean", 1, 1, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int8", 1, 22, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int16", 1, 333, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int32", 1, 4444, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int64", 1, 55555, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt8", 1, 66, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt16", 1, 777, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt32", 1, 8888, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt64", 1, 99999, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Float32", 2, 0, 22.22, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Float64", 2, 0, 4444.4444, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.String80", 3, 0, 0, "Why don't you have wings to fly with?", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Time", 3, 0, 0, "15-MAY-2020 08:27:50.50", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.DeltaTime", 3, 0, 0, "27 8:27:50.05", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Objid", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-Values", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.AttrRef", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-Values.AttrRef", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.BooleanArray[4]", 1, 1, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int8Array[4]", 1, 22, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int16Array[4]", 1, 333, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int32Array[4]", 1, 4444, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Int64Array[4]", 1, 55555, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt8Array[4]", 1, 66, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt16Array[4]", 1, 777, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt32Array[4]", 1, 8888, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.UInt64Array[4]", 1, 99999, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Float32Array[4]", 2, 0, 22.22, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.Float64Array[4]", 2, 0, 4444.4444, "", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.String80Array[4]", 3, 0, 0, "Why don't you have wings to fly with?", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.TimeArray[4]", 3, 0, 0, "15-MAY-2020 08:27:50.50", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.DeltaTimeArray[4]", 3, 0, 0, "27 8:27:50.05", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.ObjidArray[4]", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-Values", LDH__SUCCESS},
    {"Test02-Pwrwb-Values.AttrRefArray[4]", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-Values.AttrRefArray[4]", LDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  pwr_tTid tid;
  char *buff;
  int size;
  char *s;
  int index = 0;
  int offs;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].aname, &aref); 
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectPar, ldh_NameToAttrRef", d[i].aname, m_sts);
      return;
    }

    m_sts = ldh_GetAttrRefTid(m_ldhses, &aref, &tid);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectPar, ldh_GetAttrRefTid", d[i].aname, m_sts);
      return;
    }

    s = strchr(d[i].aname, '.');
    m_sts = ldh_GetObjectPar(m_ldhses, aref.Objid, "RtBody", s+1, &buff, &size);
    if (EVEN(m_sts)) {
      m_log->log('E', "GetObjectPar", d[i].aname, m_sts);
      return;
    }

    if (d[i].aname[strlen(d[i].aname)-1] == ']') {
      s = strrchr(d[i].aname, '[');
      if (s)
	sscanf(s+1, "%d", &index);
    }

    switch (d[i].valtype) {
    case 1:
      switch (tid) {
      case pwr_eType_Boolean: {
	offs = index * sizeof(pwr_tBoolean);
	if ((pwr_tBoolean)d[i].ival != *(pwr_tBoolean *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tBoolean *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Int8: {
	offs = index * sizeof(pwr_tInt8);
	if ((pwr_tInt8)d[i].ival != *(pwr_tInt8 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tInt8 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Int16: {
	offs = index * sizeof(pwr_tInt16);
	if ((pwr_tInt16)d[i].ival != *(pwr_tInt16 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tInt16 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Int32: {
	offs = index * sizeof(pwr_tInt32);
	if ((pwr_tInt32)d[i].ival != *(pwr_tInt32 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tInt32 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Int64: {
	offs = index * sizeof(pwr_tInt64);
	if ((pwr_tInt64)d[i].ival != *(pwr_tInt64 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tInt64 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_UInt8: {
	offs = index * sizeof(pwr_tUInt8);
	if ((pwr_tUInt8)d[i].ival != *(pwr_tUInt8 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tUInt8 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_UInt16: {
	offs = index * sizeof(pwr_tUInt16);
	if ((pwr_tUInt16)d[i].ival != *(pwr_tUInt16 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tUInt16 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_UInt32: {
	offs = index * sizeof(pwr_tUInt32);
	if ((pwr_tUInt32)d[i].ival != *(pwr_tUInt32 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tUInt32 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_UInt64: {
	offs = index * sizeof(pwr_tUInt64);
	if ((pwr_tUInt64)d[i].ival != *(pwr_tUInt64 *)(buff+offs)) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %d != %d", d[i].aname, d[i].ival, *(pwr_tUInt64 *)(buff+offs));
	  return;
	}
	break;
      }
      }
      break;
    case 2:
      switch (tid) {
      case pwr_eType_Float32: {
	offs = index * sizeof(pwr_tFloat32);
	if (fabs((pwr_tFloat32)d[i].fval - *(pwr_tFloat32 *)(buff+offs)) > FLT_EPSILON*1000) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %f != %f", d[i].aname, d[i].fval, *(pwr_tFloat32 *)(buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Float64: {
	offs = index * sizeof(pwr_tFloat64);
	if (fabs((pwr_tFloat64)d[i].fval - *(pwr_tFloat64 *)(buff+offs)) > FLT_EPSILON*10000) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %f != %f", d[i].aname, d[i].fval, *(pwr_tFloat64 *)(buff+offs));
	  return;
	}
	break;
      }
      }
      break;
    case 3:
      switch (tid) {
      case pwr_eType_String: {
	offs = index * sizeof(pwr_tFloat32);
	if (strcmp(d[i].sval, (buff+offs)) != 0) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %s != %s", d[i].aname, d[i].sval, (buff+offs));
	  return;
	}
	break;
      }
      case pwr_eType_Time: {
	char timstr[40];
	offs = index * sizeof(pwr_tTime);
	time_AtoAscii((pwr_tTime *)(buff+offs), time_eFormat_DateAndTime, timstr, sizeof(timstr));
	if (strcmp(d[i].sval, timstr) != 0) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %s != %s", d[i].aname, d[i].sval, timstr);
	  return;
	}
	break;
      }
      case pwr_eType_DeltaTime: {
	char timstr[40];
	offs = index * sizeof(pwr_tTime);
	time_DtoAscii((pwr_tDeltaTime *)(buff+offs), 1, timstr, sizeof(timstr));
	if (strcmp(d[i].sval, timstr) != 0) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %s != %s", d[i].aname, d[i].sval, timstr);
	  return;
	}
	break;
      }
      case pwr_eType_Objid: {
	pwr_tOName oname;
	int size;
	offs = index * sizeof(pwr_tOid);
	m_sts = ldh_ObjidToName(m_ldhses, *(pwr_tOid *)(buff+offs), ldh_eName_VolPath, oname, sizeof(oname), &size);
	if (strcmp(d[i].sval, oname) != 0) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %s != %s", d[i].aname, d[i].sval, oname);
	  return;
	}
	break;
      }
      case pwr_eType_AttrRef: {
        char *aname;
	int size;
	offs = index * sizeof(pwr_tAttrRef);
	m_sts = ldh_AttrRefToName(m_ldhses, (pwr_tAttrRef *)(buff+offs), cdh_mName_volumeStrict, &aname, &size);
	if (strcmp(d[i].sval, aname) != 0) {
	  m_log->vlog('E', "GetObjectPar, %s, value doesn't match %s != %s", d[i].aname, d[i].sval, aname);
	  return;
	}
	break;
      }
      }
      break;
    }
    free(buff);
  }

  m_log->log('S', "GetObjectPar", LDH__SUCCESS);
}

typedef struct {
  pwr_tOName aname;
  int valtype;
  int ival;
  float fval;
  pwr_tString80 sval;
  pwr_tStatus sts;
} sSetObjectPar;

void wa_ldhtest::SetObjectPar(void)
{
  sSetObjectPar d[] = {
    {"Test02-Pwrwb-SetValues.Boolean", 1, 1, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Int8", 1, 22, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Int16", 1, 333, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Int32", 1, 4444, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Int64", 1, 55555, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.UInt8", 1, 66, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.UInt16", 1, 777, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.UInt32", 1, 8888, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.UInt64", 1, 99999, 0, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Float32", 2, 0, 22.22, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Float64", 2, 0, 4444.4444, "", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.String80", 3, 0, 0, "Why don't you have wings to fly with?", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Time", 3, 0, 0, "15-MAY-2020 08:27:50.50", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.DeltaTime", 3, 0, 0, "27 8:27:50.05", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.Objid", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-SetValues", LDH__SUCCESS},
    {"Test02-Pwrwb-SetValues.AttrRef", 3, 0, 0, "VolPwrtest02:Test02-Pwrwb-SetValues.AttrRef", LDH__SUCCESS}
  };

  pwr_tAttrRef aref;
  pwr_tTid tid;
  char buff[80];
  int size;
  char *s;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = ldh_NameToAttrRef(m_ldhses, d[i].aname, &aref); 
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectPar, ldh_NameToAttrRef", d[i].aname, m_sts);
      return;
    }

    m_sts = ldh_GetAttrRefTid(m_ldhses, &aref, &tid);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectPar, ldh_GetAttrRefTid", d[i].aname, m_sts);
      return;
    }

    switch (d[i].valtype) {
    case 1:
      switch (tid) {
      case pwr_eType_Boolean: {
	pwr_tBoolean val = (pwr_tBoolean)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tBoolean);
	break;
      }
      case pwr_eType_Int8: {
	pwr_tInt8 val = (pwr_tInt8)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tInt8);
	break;
      }
      case pwr_eType_Int16: {
	pwr_tInt16 val = (pwr_tInt16)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tInt16);
	break;
      }
      case pwr_eType_Int32: {
	pwr_tInt32 val = (pwr_tInt32)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tInt32);
	break;
      }
      case pwr_eType_Int64: {
	pwr_tInt64 val = (pwr_tInt64)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tInt64);
	break;
      }
      case pwr_eType_UInt8: {
	pwr_tUInt8 val = (pwr_tUInt8)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tUInt8);
	break;
      }
      case pwr_eType_UInt16: {
	pwr_tUInt16 val = (pwr_tUInt16)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tUInt16);
	break;
      }
      case pwr_eType_UInt32: {
	pwr_tUInt32 val = (pwr_tUInt32)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tUInt32);
	break;
      }
      case pwr_eType_UInt64: {
	pwr_tUInt64 val = (pwr_tUInt64)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tUInt64);
	break;
      }
      }
      break;
    case 2:
      switch (tid) {
      case pwr_eType_Float32: {
	pwr_tFloat32 val = (pwr_tFloat32)d[i].fval;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tFloat32);
	break;
      }
      case pwr_eType_Float64: {
	pwr_tFloat64 val = (pwr_tFloat64)d[i].ival;
	memcpy(buff, &val, sizeof(val));
	size = sizeof(pwr_tFloat64);
	break;
      }
      }
      break;
    case 3:
      switch (tid) {
      case pwr_eType_String: {
	memcpy(buff, d[i].sval, sizeof(d[i].sval));
	size = sizeof(pwr_tString80);
	break;
      }
      case pwr_eType_Time: {
        pwr_tTime time;
	time_AsciiToA(d[i].sval, &time);
	memcpy(buff, &time, sizeof(time));
	size = sizeof(pwr_tTime);
	break;
      }
      case pwr_eType_DeltaTime: {
        pwr_tDeltaTime time;
	time_AsciiToD(d[i].sval, &time);
	memcpy(buff, &time, sizeof(time));
	size = sizeof(pwr_tDeltaTime);
	break;
      }
      case pwr_eType_Objid: {
	pwr_tOid oid;
	m_sts = ldh_NameToObjid(m_ldhses, &oid, d[i].sval);
	memcpy(buff, &oid, sizeof(oid));
	size = sizeof(pwr_tOid);
	break;
      }
      case pwr_eType_AttrRef: {
	pwr_tAttrRef aref;
	m_sts = ldh_NameToAttrRef(m_ldhses, d[i].sval, &aref);
	memcpy(buff, &aref, sizeof(aref));
	size = sizeof(pwr_tAttrRef);
	break;
      }
      }
      break;
    }

    s = strchr(d[i].aname, '.');
    m_sts = ldh_SetObjectPar(m_ldhses, aref.Objid, "RtBody", s+1, buff, size);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectPar", d[i].aname, m_sts);
      return;
    }
    char *rbuff;
    int rsize;
    m_sts = ldh_GetObjectPar(m_ldhses, aref.Objid, "RtBody", s+1, &rbuff, &rsize);
    if (EVEN(m_sts)) {
      m_log->log('E', "SetObjectPar", d[i].aname, m_sts);
      return;
    }
    if (memcmp(buff, rbuff, size) != 0) {
      m_log->log('E', "SetObjectPar, value differs", d[i].aname, m_sts);
      return;
    }
    free(rbuff);

  }

  m_log->log('S', "SetObjectPar", LDH__SUCCESS);
}

void wa_ldhtest::Init(void)
{
  m_sts = ldh_VolumeNameToId(m_ldhwb, m_vname, &m_vid);
  if (EVEN(m_sts)) {
    m_log->log('F', "Init, ldh_VolumeNameToId", m_sts);
    exit(0);
  }
  
  m_sts = ldh_AttachVolume(m_ldhwb, m_vid, &m_volctx);
  if (EVEN(m_sts)) {
    m_log->log('F', "Init, ldh_AttachVolume", m_sts);
    exit(0);
  }

  m_sts = ldh_OpenSession(&m_ldhses, m_volctx, ldh_eAccess_ReadWrite, ldh_eUtility_Pwr);
  if (EVEN(m_sts)) {
    m_log->log('F', "Init, ldh_OpenSession", m_sts);
    exit(0);
  }

  m_log->log('S', "Init", LDH__SUCCESS);
}

// Constructor
wa_ldhtest::wa_ldhtest()
{
  strcpy(m_vname, "");
  m_log = new tst_log(&m_sts, "wb_test-Ldh", "$pwrp_log/ldh.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
wa_ldhtest::~wa_ldhtest()
{
  ldh_DeleteObjectTree(m_ldhses, m_test, 0);

  delete m_log;
}

int main(int argc, char *argv[])
{
  wa_ldhtest ldh;

  if (argc > 1)
    ldh.vname(argv[1]);

  setenv("TZ", "Europe/Stockholm", 1);
  tzset();

  ldh.OpenWB();
  ldh.AttachVolume();
  ldh.OpenSession();
  ldh.Init();
  ldh.ObjidToName();
  ldh.CreateObject();
  ldh.DeleteObject();
  ldh.DeleteObjectTree();
  ldh.ChangeObjectName();
  ldh.MoveObject();
  ldh.GetRootList();
  ldh.GetChild();
  ldh.GetParent();
  ldh.GetNextSibling();
  ldh.GetPreviousSibling();
  ldh.NameToAttrRef();
  ldh.AttrRefToName();
  ldh.AttrArefToObjectAref();
  ldh.GetClassList();
  ldh.GetNextObject();
  ldh.GetNextAttrRef();
  ldh.ClassNameToId();
  ldh.ClassIdToName();
  ldh.GetSuperClass();
  ldh.GetEnumValueDef();
  ldh.GetMaskBitDef();
  ldh.GetObjectClass();
  ldh.GetAttrRefTid();
  ldh.GetVolumeList();
  ldh.GetNextVolume();
  ldh.GetVolumeClass();
  ldh.GetVidInfo();
  ldh.GetVolumeInfo();
  ldh.CopyObject();
  ldh.CopyObjectTrees();
  ldh.CreateVolume();
  ldh.GetObjectPar();
  ldh.SetObjectPar();
  ldh.Close();
}
