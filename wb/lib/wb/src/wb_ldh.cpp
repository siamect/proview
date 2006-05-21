/* 
 * Proview   $Id: wb_ldh.cpp,v 1.55 2006-05-21 22:30:50 lw Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#if 1
/* wb_ldh.c -- local data handler
  This module contains the API-routines to the Local Data Handler, LDH.  */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <fstream.h>
#ifdef OS_VMS
#include <descrip.h>
#include <libdef.h>
#include <lnmdef.h>
#include <starlet.h>
#include <lib$routines.h>
#endif
#include <stdarg.h>
#include "pwr.h"
#include "wb_ldhi.h"
#include "pwr_class.h"
#include "wb_ldh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "pwr_vararg.h"
#include "co_ver.h"
#include "rt_gdh.h"
#include "wb_ldh.h"
#include "wb_ldhld.h"
// #include "wb_ldhdb.h"
#include "wb_pwrs.h"
#include "wb_env.h"
#include "wb_session.h"
#include "wb_object.h"
#include "wb_volume.h"
#include "wb_error.h"
#include "wb_vrepmem.h"
#include "wb_vrepwbl.h"
#include "wb_vrepdbs.h"
#include "wb_merep.h"
#include "wb_db.h"
#include "wb_print_wbl.h"
#include "pwr_baseclasses.h"
#include <X11/Intrinsic.h>

pwr_dImport pwr_BindClasses(System);
pwr_dImport pwr_BindClasses(Base);
#if NOT_YET_IMPLEMENTED
static ldh_sMenuItem ldh_lMenuItem[100];

static int ldh_gLog_a = 0;
static int ldh_gLog_b = 0;
static int ldh_gLog_c = 0;

static struct {
  pwr_tString32 Name;
  ldh_eUtility Value;
} ldh_lUtility[] = {
  {"__",		ldh_eUtility__},
  {"Navigator",		ldh_eUtility_Navigator},
  {"Configurator",	ldh_eUtility_Configurator},
  {"-",			ldh_eUtility_}
};

static struct {
  pwr_tString32 Name;
  pwr_tChar Char;
  ldh_eMenuSet Value;
} ldh_lMenuSet[] = {
  {"__",		'\0', ldh_eMenuSet__},
  {"Attribute",		'a',  ldh_eMenuSet_Attribute},
  {"Class",		'c',  ldh_eMenuSet_Class},
  {"Many",		'm',  ldh_eMenuSet_Many},
  {"None",		'n',  ldh_eMenuSet_None},
  {"Object",		'o',  ldh_eMenuSet_Object},
  {"-",			'\0', ldh_eMenuSet_}
};
#endif
/*
  Open a db volume.
*/
pwr_tStatus
ldh_AttachVolume(ldh_tWorkbench	workbench, pwr_tVid vid, ldh_tVolume *volume)
{
  wb_env *wb = (wb_env *)workbench;

  wb_volume *v = new wb_volume();
  *v = wb->volume(vid);
  if (!*v) return v->sts();
    
  *volume = (ldh_tVolume) v;

  return LDH__SUCCESS;
}

/*
  Delete a db volume
*/
pwr_tStatus
ldh_DeleteVolume(ldh_tWorkbench workbench, pwr_tVid vid)
{
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_DetachVolume(ldh_tWorkbench workbench, ldh_tVolume volume)
{
  wb_volume *v = (wb_volume *)volume;
  delete v;

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetVolumeList(ldh_tWorkbench workbench, pwr_tVid *vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume();
  if (!v) return v.sts();
    
  *vid = v.vid();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetBufferList(ldh_tWorkbench workbench, pwr_tVid *vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->bufferVolume();
  if (!v) return v.sts();
    
  *vid = v.vid();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetNextVolume(ldh_tWorkbench workbench, pwr_tVid vid, pwr_tVid *new_vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume(vid);
  if (!v) return v.sts();
  v = v.next();
  if (!v) return v.sts();
    
  *new_vid = v.vid();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetPreviousVolume(ldh_tWorkbench workbench, pwr_tVid vid, pwr_tVid *new_vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume(vid);
  if (!v) return v.sts();
  //v = v.prev();
  if (!v) return v.sts();
    
  *new_vid = v.vid();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_LoadVolume(ldh_tWorkbench	workbench, char *name, ldh_tVolume *volume)
{
#if 0
  //wb_env *wb = (wb_env *)workbench;

  wb_vrepwbl lv(name);
    
  wb_volume *v = new wb_volume(lv);
    
  *volume = (ldh_tVolume) v;
#endif
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_CreatVolumeSnapshot(ldh_tWorkbench	workbench, char *name, ldh_tVolume *volume)
{
  //wb_env *wb = (wb_env *)workbench;
    
  wb_volume *v = (wb_volume *)volume;
  v->createSnapshot(name, 0);

  return LDH__SUCCESS;
}

/*  Convert a volume name to its corresponding
    volume identity.  */

pwr_tStatus
ldh_VolumeNameToId(ldh_tWorkbench workbench, char *name, pwr_tVid *vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume(name);
  if (!v) return v.sts();

  *vid = v.vid();
    
  return v.sts();
}

pwr_tStatus
ldh_GetVolumeClass(ldh_tWorkbench workbench, pwr_tVid vid, pwr_tCid *cid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume(vid);
  if (!v) return v.sts();

  *cid = v.cid();
    
  return v.sts();
}

/* Open a load file volume */
pwr_tStatus
ldh_OpenVolume(ldh_tWorkbench workbench, ldh_tSession *session, pwr_tVid vid)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->snapshot(vid);
  if (!v) return v.sts();

  // wb_srep *srep = new wb_srep(v);
  wb_session *sp = new wb_session(v);
    
  *session = (ldh_tSession)sp;

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_CallMenuMethod(ldh_sMenuCall *mcp, int index)
{
  wb_session *sp = (wb_session *) mcp->PointedSession; 

  return sp->callMenuMethod( mcp, index);
}

pwr_tStatus
ldh_GetMethod( ldh_tSession Session, char *Name, wb_tMethod *method)
{
  pwr_tStatus sts;

  wb_session *sp = (wb_session *)Session;
  wb_erep *erep = sp->env();

  erep->method( &sts, Name, method);
  return sts;
}


pwr_tStatus
ldh_ChangeObjectName(ldh_tSession session, pwr_tOid oid, char *name)
{
  wb_session *sp = (wb_session *)session;

  try {
    wb_name n(name);
    if ( !n) return n.sts();

    wb_object o = sp->object(oid);
    if (!o) return o.sts();
    if (!sp->isLocal(o)) return LDH__OTHERVOLUME;

    sp->renameObject(o, n);
    return sp->sts();
  }
  catch ( wb_error& e) {
    return e.sts();
  }
}


pwr_tStatus
ldh_CheckAttrXRef(ldh_tSession session, pwr_sAttrRef *aref)
{
  wb_session *sp = (wb_session *)session;
    
  wb_attribute a = sp->attribute(aref);
  return a.checkXref();
    
}

/* Check an object cross reference.
   The reference is identified by object id and name of
   cross reference attribute.  */
pwr_tStatus
ldh_CheckObjXRef(ldh_tSession session, pwr_tObjid oid, pwr_tObjName aname)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object(oid);
  return o.checkXref(aname);    
}

pwr_tStatus
ldh_CheckObjXRefs(ldh_tSession session, pwr_tOid p, pwr_tObjName paname, pwr_tOid s, pwr_tObjName saname)
{
  return LDH__NYI;
}

pwr_tStatus
ldh_ClassNameToId(ldh_tSession session, pwr_tCid *cid, char *name)
{
  wb_session *sp = (wb_session *)session;
  wb_name n(name);
  if ( EVEN(n.sts()))
    return n.sts();
    
  wb_cdef c = sp->cdef(n);
  if (!c) return c.sts();
    
  *cid = c.cid();
  return c.sts();
}

pwr_tStatus
ldh_CloseSession(ldh_tSession session)
{
  try {
    wb_session *sp = (wb_session *)session;

    //sp->close();
    //sp->unref();
    delete sp;
        
    return LDH__SUCCESS;
  }
  catch (wb_error& e) {
    return e.sts();
  }
  return LDH__SUCCESS;
}


pwr_tStatus
ldh_CloseWB(ldh_tWorkbench workbench)
{
  try {
    wb_env *env = (wb_env *)workbench;

    delete env;;

    return LDH__SUCCESS;
  }
  catch (wb_error& e) {
    return e.sts();
  }
}


pwr_tStatus
ldh_CopyObject(ldh_tSession session, pwr_tObjid *oid, char *name, pwr_tObjid srcoid, pwr_tObjid dstoid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;

  try {
  wb_object s_o = sp->object(srcoid);
  if ( !s_o) return s_o.sts();
  wb_object d_o = sp->object(dstoid);
  if ( !d_o) return d_o.sts();
  wb_destination d = d_o.destination(dest);
  wb_object o = sp->copyObject(s_o, d, name);

  if (!o) return o.sts();
    
  *oid = o.oid();
  return o.sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }
}

pwr_tStatus
ldh_CreateObject(ldh_tSession session, pwr_tOid *oid, char *name, pwr_tCid cid, pwr_tOid doid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;

  try {
    wb_name n(name);
        
    wb_cdef cdef = sp->cdef(cid);
    
    if (cdh_ObjidIsNull(doid)) {
      doid.vid = sp->vid();
      dest = ldh_eDest_IntoLast;
    }
      
    wb_object d_o = sp->object(doid);
    wb_destination d = d_o.destination(dest);
    wb_object o = sp->createObject(cdef, d, n);
    if (!o) return sp->sts();
    
    *oid = o.oid();
    return o.sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }
}

pwr_tStatus
ldh_CreateVolume(ldh_tWorkbench workbench, ldh_tSession *session, pwr_tVid vid, char *name, pwr_tCid cid)
{
  wb_env *ep = (wb_env *)workbench;

  wb_name n(name);
    
  wb_volume v = ep->createVolume(vid, cid, name);
  if (!v) return v.sts();
    
  // wb_srep *srep = new wb_srep(v);
  wb_session *sp = new wb_session(v);
  *session = (ldh_tSession)sp;
    
  return sp->sts();
}

pwr_tStatus
ldh_DeleteObject(ldh_tSession session, pwr_tOid oid)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  try {
  sp->deleteObject(o);
  return sp->sts();
  }
  catch ( wb_error& e) {
    return e.sts();
  }
}

pwr_tStatus
ldh_DeleteObjectTree(ldh_tSession session, pwr_tOid oid)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  try {
  return sp->deleteFamily(o);
  }
  catch ( wb_error& e) {
    return e.sts();
  }
}


pwr_tStatus
ldh_GetAttrDef(ldh_tSession session, pwr_tCid cid, char *bname, char *aname, ldh_sParDef *adef)
{
  wb_session *sp = (wb_session *)session;

  wb_adef a = sp->adef(cid, bname, aname);
  if (!a) return a.sts();
    
  strcpy(adef->ParName, a.name());
  adef->ParLevel = 1;
  adef->ParClass = (pwr_eClass) a.cid();
  adef->Par = (pwr_uParDef *) a.body();
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetAttrRef(ldh_tSession session, pwr_tOid oid, char *aname, pwr_sAttrRef *aref)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  wb_attrname n(aname);
  wb_attribute a = sp->attribute(o, n);
  if (!a) return a.sts();
    
  *aref = a.aref();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetAttrXRefDef(ldh_tSession session, pwr_sAttrRef *aref, pwr_sAttrXRef *xref)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute(aref);
  if (!a) return a.sts();
    
  a.adrep()->body( xref);

  return a.sts();
}

/*  Get first child of an object.  */

pwr_tStatus
ldh_GetChild(ldh_tSession session, pwr_tOid oid, pwr_tOid *coid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
  o = o.first();
  if (!o) return o.sts();    
  *coid = o.oid();

  return o.sts();
}


/*  Get first child of an object. If MountObject, get child of mounted object */

pwr_tStatus
ldh_GetChildMnt(ldh_tSession session, pwr_tOid oid, pwr_tOid *coid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  if ( o.cid() == pwr_eClass_MountObject) {
    wb_attribute a(o.sts(), o, "SysBody", "Object");
    if ( !a) return a.sts();

    pwr_tOid moid;
    a.value( &moid);

    wb_object mo = sp->object(moid);
    if ( mo)
      mo = mo.first();
    if ( mo) {
      *coid = mo.oid();
      return mo.sts();
    }
  }
  o = o.first();
  if (!o) return o.sts();    
  *coid = o.oid();

  return o.sts();
}


pwr_tStatus
ldh_GetClassBody(ldh_tSession session, pwr_tCid cid, char *bname, pwr_tCid *bcid, char **body, int *size)
{
  wb_session *sp = (wb_session *)session;
  wb_cdef c = sp->cdef(cid);
  if ( !c) 
    return c.sts();
  wb_object o = c.classBody(bname);
  if ( !o)
    return o.sts();

  if ( ((wb_orep *)o)->vrep()->type() == ldh_eVolRep_Db)
    return LDH__NYI;

  *bcid = o.cid();
  *size = o.rbSize();
  wb_attribute a = wb_attribute( o.sts(), o);
  *body = (char *) a.value(0);

  return o.sts();
}

pwr_tStatus
ldh_GetClassList(ldh_tSession session, pwr_tCid cid, pwr_tOid *oid)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object(cid);
  if (!o) return o.sts();

  *oid = o.oid();
  return o.sts();
}

/* Returns the objid of the next object of the same class.  */

pwr_tStatus
ldh_GetNextObject(ldh_tSession session, pwr_tOid oid, pwr_tOid *new_oid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  o = o.next();
  if (!o) return o.sts();
    
  *new_oid = o.oid();
    
  return o.sts();
}

pwr_tStatus
ldh_GetClassListAttrRef(ldh_tSession session, pwr_tCid cid, 
			pwr_sAttrRef *arp)
{
  wb_session *sp = (wb_session *)session;

  sp->aref( cid, arp);
  return sp->sts();
}

/* Returns the attrref of the next object of the same class.  */

pwr_tStatus
ldh_GetNextAttrRef(ldh_tSession session, pwr_tCid cid, pwr_sAttrRef *arp, 
		   pwr_sAttrRef *new_arp)
{
  wb_session *sp = (wb_session *)session;

  sp->nextAref( cid, arp, new_arp);
  return sp->sts();
}

/* Returns the first attribute object of the specified class in
   the specified object. */

pwr_tStatus
ldh_GetObjectClassList(ldh_tSession session, pwr_tCid cid, 
			pwr_tOid oid, pwr_sAttrRef *arp)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object( oid);
  if ( !o) return o.sts();

  sp->aref( cid, o, arp);
  return sp->sts();
}

/* Returns the attrref of the next attribute object of the same class in
   the current object.  */

pwr_tStatus
ldh_GetNextObjectAttrRef(ldh_tSession session, pwr_tCid cid, pwr_sAttrRef *arp, 
		   pwr_sAttrRef *new_arp)
{
  wb_session *sp = (wb_session *)session;

  sp->nextObjectAref( cid, arp, new_arp);
  return sp->sts();
}

pwr_tStatus
ldh_GetMenu(ldh_tSession session, ldh_sMenuCall *ip)
{
  wb_session *sp = (wb_session *)session;

  return sp->getMenu( ip);
}

pwr_tStatus
ldh_GetNextSibling(ldh_tSession session, pwr_tOid oid, pwr_tOid *noid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  o = o.after();
  if (!o) return o.sts();
    
  *noid = o.oid();
  return o.sts();
}

/* Gets a named body of an object. Buffer space to keep the body is
   allocated by the routine and a pointer to the buffer is returned
   in the body parameter. It is the responsibility of the caller to free
   this space when no longer needed. Use XtFree().  */

pwr_tStatus
ldh_GetObjectBody(ldh_tSession session, pwr_tOid oid, char *bname, void **buff, int *size)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute( oid, bname);
  if ( !a) return a.sts();

  *buff = malloc(a.size());
  if (*buff == NULL) return LDH__INSVIRMEM;
  a.value( *buff);
  if (size != NULL) *size = a.size();
  
  return LDH__SUCCESS;

}

pwr_tStatus
ldh_GetObjectBodyDef(ldh_tSession session, pwr_tCid cid, char *bname,
                     int maxlev, ldh_sParDef **bdef, int *rows)
{
  wb_session *sp = (wb_session *)session;
  wb_cdef c = sp->cdef(cid);
  if (!c) return c.sts();
  wb_bdef b = c.bdef(bname);
  if (!b) return b.sts();

  wb_adef asuper[20];
  int scnt = 0;
  asuper[scnt++] = b.adef();
  if ( asuper[scnt-1] && asuper[scnt-1].isSuperClass()) {
    // Count rows
    *rows = 0;
    while ( asuper[scnt-1] && asuper[scnt-1].isSuperClass()) {
      wb_cdef subc = sp->cdef( asuper[scnt-1].subClass());
      wb_bdef subb = subc.bdef(pwr_eBix_rt);
      *rows += subb.nAttribute() - 1;
      asuper[scnt++] = subb.adef();
    }
    *rows += b.nAttribute();

    *bdef = (ldh_sParDef *) calloc(1, sizeof(ldh_sParDef) * *rows);

    int j = 0;
    for ( int i = scnt - 1; i >= 0; i--) {
      for (wb_adef a = asuper[i]; a; a = a.next()) {
	if ( a && a.isSuperClass())
	  continue;

	strcpy((*bdef)[j].ParName, "");
	for ( int ii = i; ii > 0; ii--)
	  strcat( (*bdef)[j].ParName, "Super.");
	strcat((*bdef)[j].ParName, a.name());
	(*bdef)[j].ParLevel = 1;
	(*bdef)[j].ParClass = (pwr_eClass)a.cid();
	(*bdef)[j].Flags = ldh_mParDef_Super;
	(*bdef)[j++].Par = (pwr_uParDef *) a.body();
      }
      if ( j > *rows)
	// Something is wrong
	break;
    }
    // Detect shadowed attributes
    for ( j = 0; j < *rows - 1; j++) {
      char *s1 = strrchr( (*bdef)[j].ParName, '.');
      if ( !s1)
	s1 = (*bdef)[j].ParName;
      else
	s1++;
      for ( int i = j + 1; i < *rows; i++) {
	char *s2 = strrchr( (*bdef)[i].ParName, '.');
	if ( !s2)
	  s2 = (*bdef)[i].ParName;
	else
	  s2++;
	if ( cdh_NoCaseStrcmp( s1, s2) == 0) {
	  (*bdef)[j].Flags |= ldh_mParDef_Shadowed;
	  break;
	}
      }
    }
  }
  else {  
    *rows = b.nAttribute();
    *bdef = (ldh_sParDef *) calloc(1, sizeof(ldh_sParDef) * *rows);
    if (*bdef == NULL) return LDH__INSVIRMEM;

    for (wb_adef a = b.adef(); a; a = a.next()) {
      strcpy((*bdef)[a.index()].ParName, a.name());
      (*bdef)[a.index()].ParLevel = 1;
      (*bdef)[a.index()].ParClass = (pwr_eClass)a.cid();
      (*bdef)[a.index()].Par = (pwr_uParDef *) a.body();
    }
  }    
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetTrueObjectBodyDef(ldh_tSession session, pwr_tCid cid, char *bname,
                     int maxlev, ldh_sParDef **bdef, int *rows)
{
  wb_session *sp = (wb_session *)session;
  wb_cdef c = sp->cdef(cid);
  if (!c) return c.sts();
  wb_bdef b = c.bdef(bname);
  if (!b) return b.sts();

  *rows = b.nAttribute();
  *bdef = (ldh_sParDef *) calloc(1, sizeof(ldh_sParDef) * *rows);
  if (*bdef == NULL) return LDH__INSVIRMEM;

  for (wb_adef a = b.adef(); a; a = a.next()) {
    strcpy((*bdef)[a.index()].ParName, a.name());
    (*bdef)[a.index()].ParLevel = 1;
    (*bdef)[a.index()].ParClass = (pwr_eClass)a.cid();
    (*bdef)[a.index()].Par = (pwr_uParDef *) a.body();
  }    
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetObjectBuffer(ldh_tSession session, pwr_tOid oid, char *bname,
                    char *aname, pwr_eClass *bufferclass, char **value, int *size)
{
  wb_session *sp = (wb_session*)session;
  wb_attribute a = sp->attribute(oid, bname, aname);
  if (!a) return a.sts();

  *value = (char *)calloc(1, a.size());
  *size = a.size();

  a.value( *value);
  *bufferclass = (pwr_eClass)a.subClass();
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetObjectClass(ldh_tSession session, pwr_tOid oid, pwr_tCid *cid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  *cid = o.cid();
    
  return o.sts();
}

pwr_tStatus
ldh_GetAttrRefTid(ldh_tSession session, pwr_sAttrRef *arp, pwr_tTid *tid)
{
  wb_session *sp = (wb_session *)session;

  if ( arp->Flags.b.Object) {
    wb_object o = sp->object(arp->Objid);
    if (!o) return o.sts();
    
    *tid = o.cid();
    return o.sts();
  }

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
  *tid = a.tid();
    
  return a.sts();
}

pwr_tStatus
ldh_GetAttrRefOrigTid(ldh_tSession session, pwr_sAttrRef *arp, pwr_tTid *tid)
{
  wb_session *sp = (wb_session *)session;

  if ( arp->Flags.b.Object) {
    wb_object o = sp->object(arp->Objid);
    if (!o) return o.sts();
    
    *tid = o.cid();
    return o.sts();
  }

  try {
  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
  *tid = a.originalTid();
    
  return a.sts();
  }
  catch ( wb_error& e) {
    return e.sts();
  }
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetAttrRefType(ldh_tSession session, pwr_sAttrRef *arp, pwr_eType *type)
{
  wb_session *sp = (wb_session *)session;

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
  *type = a.type();
    
  return a.sts();
}


pwr_tStatus
ldh_GetObjectContext(ldh_tSession session, pwr_tOid oid, ldh_sObjContext **octx)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  o.user();
    
  return LDH__NYI;
}

/*  Give information about an object.  */

pwr_tStatus
ldh_GetObjectInfo(ldh_tSession session, pwr_tOid oid, ldh_sObjInfo *ip)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  ip->Volume = o.vid();

  return o.sts();
}

pwr_tStatus
ldh_GetObjectPar(ldh_tSession session, pwr_tOid oid, char *bname, char *aname, char **buff, int *size)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute(oid, bname, aname);
  if (!a) return a.sts();
    
    
  *buff = (char *)calloc(1, a.size());
  if (*buff == NULL) return LDH__INSVIRMEM;
  a.value( *buff);
  if (size != 0)
    *size = a.size();

  return a.sts();
}

pwr_tStatus
ldh_GetAttrObjectPar(ldh_tSession session, pwr_sAttrRef *arp, char *bname, char *aname, char **buff, int *size)
{
  wb_session *sp = (wb_session *)session;

  if ( arp->Flags.b.Object) {
    wb_attribute a = sp->attribute(arp->Objid, bname, aname);
    if (!a) return a.sts();    
    
    *buff = (char *)calloc(1, a.size());
    if (*buff == NULL) return LDH__INSVIRMEM;
    a.value( *buff);
    if (size != 0)
      *size = a.size();

    return a.sts();
  }
  else if ( arp->Flags.b.ObjectAttr) {
    char name[240];

    wb_attribute aarp = sp->attribute( arp);
    if ( !aarp) return aarp.sts();

    strcpy( name, aarp.attrName());
    strcat( name, ".");
    strcat( name, aname);

    wb_attribute a = sp->attribute( name);
    if (!a) return a.sts();    
    
    *buff = (char *)calloc(1, a.size());
    if (*buff == NULL) return LDH__INSVIRMEM;
    a.value( *buff);
    if (size != 0)
      *size = a.size();

    return a.sts();
  }
  else {
    wb_attribute a = sp->attribute( arp);
    if (!a) return a.sts();    
    
    *buff = (char *)calloc(1, a.size());
    if (*buff == NULL) return LDH__INSVIRMEM;
    a.value( *buff);
    if (size != 0)
      *size = a.size();

    return a.sts();
  }
}

pwr_tStatus
ldh_GetObjXRefDef(ldh_tSession session, pwr_sAttrRef *aref, pwr_sObjXRef *ObjXRef)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute(aref);
  if (!a) return a.sts();
    
  a.adrep()->body( ObjXRef);
   
  return a.sts();
}

pwr_tStatus
ldh_GetParent(ldh_tSession session, pwr_tOid oid, pwr_tOid *poid)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  o = o.parent();
  if (!o) return o.sts();
    
  *poid = o.oid();
    
  return o.sts();
}

int
ldh_IsAncestor(ldh_tSession session, pwr_tOid ancestor, pwr_tOid oid)
{
  wb_session *sp = (wb_session *)session;

  wb_object o = sp->object(oid);
  if ( !o) return o.sts();

  wb_object ao = sp->object(ancestor);
  if ( !ao) return ao.sts();

  return (int) sp->isAncestor( ao, o);
}

/* Get previous object in the list of objects of one class
   in one volume.  */
pwr_tStatus
ldh_GetPreviousObject(ldh_tSession session, pwr_tOid oid, pwr_tOid *noid)
{
  try {
    wb_session *sp = (wb_session *)session;

    wb_object o = sp->object(oid);
    if (!o) return o.sts();

    o = o.previous();
    if (!o) return o.sts();
    
    *noid = o.oid();
    
    return o.sts();

  } catch (wb_error& e) {

    return e.sts();

  }
}

pwr_tStatus
ldh_GetPreviousSibling(ldh_tSession session, pwr_tOid oid, pwr_tOid *noid)
{
  try {
    wb_session *sp = (wb_session *)session;

    wb_object o = sp->object(oid);
    o = o.before();
    *noid = o.oid();
    
    return o.sts();

  } catch (wb_error& e) {

    return e.sts();

  }    
}

/* Get the number of different references from the current
   object to other objects.  */

pwr_tStatus
ldh_GetReferenceInfo(ldh_tSession session, pwr_tOid oid, ldh_sRefInfo *rip)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  try {
    sp->refinfo( o, rip);

  } catch (wb_error& e) {
    return e.sts();
  }
  return LDH__SUCCESS;
}

/* Get first object in root list.  */

pwr_tStatus
ldh_GetRootList(ldh_tSession session, pwr_tOid *oid)
{
  try {
    wb_session *sp = (wb_session *)session;
    wb_object o = sp->object();
    if (!o) return o.sts();

    *oid = o.oid();

    return o.sts();
  } catch (wb_error& e) {

    return e.sts();

  }    
}

/* Get first object in root list of volume with volume id 'vid'.  */

pwr_tStatus
ldh_GetVolumeRootList(ldh_tSession session, pwr_tVid vid, pwr_tOid *oid)
{
  wb_session *sp = (wb_session *)session;

  wb_env env = sp->env();
  wb_volume v = env.volume( vid);
  if (!v) return v.sts();
    
  wb_object o = v.object();
  if (!o) return o.sts();

  *oid = o.oid();

  return o.sts();
}

extern "C" pwr_tStatus
ldh_GetSessionInfo(ldh_tSession session, ldh_sSessInfo *ip)
{
  wb_session *sp = (wb_session *)session;

  ip->Access   = sp->access();
  ip->Utility = sp->utility();
  ip->Empty   = sp->isEmpty() ? 1 : 0;
    
  return LDH__SUCCESS;
}

pwr_tStatus 
ldh_GetVidInfo( ldh_tWorkbench workbench, pwr_tVid vid, ldh_sVolumeInfo *ip)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume( vid);
  ip->Volume  = v.vid();
  ip->Class   = v.cid();
  ip->VolRep  = v.type();
  //ip->Version = v.time();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetVolumeInfo(ldh_tVolume volume, ldh_sVolumeInfo *ip)
{
  wb_volume *vp = (wb_volume *)volume;
    
  ip->Volume  = vp->vid();
  ip->Class   = vp->cid();
  ip->VolRep  = vp->type();
  //ip->Version = vp->time();

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetUniqueObjectName(ldh_tSession session, pwr_tOid oid, char *name)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  o.uniqueName(name);

  return o.sts();
}

pwr_tStatus
ldh_IsOkCreateObject(ldh_tSession session, pwr_tCid cid, pwr_tOid oid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  wb_destination d = o.destination(dest);
  if ( !sp->validateDestination( d, cid))
    return sp->sts();

  return LDH__SUCCESS;    
}

pwr_tStatus
ldh_IsOkCopyObject(ldh_tSession session, pwr_tOid oid, pwr_tOid doid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;
  wb_object d_o = sp->object(doid);
  if (!d_o) return d_o.sts();
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  wb_destination d = d_o.destination(dest);
  if ( !sp->validateDestination( d, o.cid()))
    return sp->sts();
    
  return LDH__SUCCESS;    
}

pwr_tStatus
ldh_IsOkMoveObject(ldh_tSession session, pwr_tOid oid, pwr_tOid doid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;
  wb_object d_o = sp->object(doid);
  if (!d_o) return d_o.sts();
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  wb_destination d = d_o.destination(dest);
  if ( !sp->validateDestination( d, o.cid()))
    return sp->sts();
    
  return LDH__SUCCESS;    
}

pwr_tStatus
ldh_MoveObject(ldh_tSession session, pwr_tOid oid, pwr_tOid doid, ldh_eDest dest)
{
  wb_session *sp = (wb_session *)session;
  wb_object d_o = sp->object(doid);
  if (!d_o) return d_o.sts();
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
  wb_destination d = d_o.destination(dest);

  try {
    sp->moveObject(o, d);
    return sp->sts();
  }
  catch ( wb_error& e) {
    return e.sts();
  }
}

pwr_tStatus
ldh_NameToAttrRef(ldh_tSession session, char *name, pwr_sAttrRef *arp)
{
  wb_session *sp = (wb_session *)session;

  wb_attribute a = sp->attribute(name);
  if (!a) return a.sts();
  
  a.aref(arp);

  return a.sts();
}

pwr_tStatus
ldh_ArefANameToAref(ldh_tSession session, pwr_sAttrRef *arp, char *aname, pwr_sAttrRef *oarp)
{
  wb_session *sp = (wb_session *)session;

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();

  wb_attribute oa( a, 0, aname);

  oa.aref(oarp);

  return oa.sts();
}


/*  Get the object identifier of a named object.  */

pwr_tStatus
ldh_NameToObjid(ldh_tSession session, pwr_tOid *oid, char *name)
{
  wb_session *sp = (wb_session *)session;
  pwr_tStatus sts;

  if ( strncmp( name, "_O", 2) == 0) {
    sts = cdh_StringToObjid( name, oid);
    if ( ODD(sts)) return sts;
  }
  wb_object o = sp->object(name);
  if (!o) return o.sts();
    
  *oid = o.oid();

  return o.sts();
}

/* Returns the name of an object identified by its objid.
   The caller is responsible for supplying a buffer at
   least as big as maxsize.  */

pwr_tStatus
ldh_ObjidToName(ldh_tSession session, pwr_tOid oid, int type, char *buf, int maxsize, int *size)
{
  wb_session *sp = (wb_session *)session;

  if (cdh_ObjidIsNull(oid)) {
    strncpy( buf, cdh_ObjidToString( NULL, oid, 1), maxsize);
    *size = strlen( buf);
    return LDH__SUCCESS;
  }
  
  switch ( type) {
  case ldh_eName_Object:
  {
    wb_object o = sp->object(oid);
    if (!o) { /* return o.sts(); */
      strncpy( buf, cdh_ObjidToString( NULL, oid, 1), maxsize);
      *size = strlen( buf);
      return LDH__SUCCESS;
    }

    try {
      char name[200];
      strcpy( name, o.name());
      *size = strlen( name);
      if ( *size > maxsize - 1) {
        return LDH__NAMEBUF;
      }
      
      strcpy( buf, name);
    }
    catch ( wb_error& e) {
      return e.sts();
    }
    break;
  }
  case ldh_eName_Objid:
  case ldh_eName_ObjectIx:
  case ldh_eName_OixString:
  case ldh_eName_VolumeId:
  case ldh_eName_VidString:
  {
    char str[80];

    wb_name n = wb_name( cdh_ObjidToString( NULL, oid, 1));
    strcpy( str, n.name( type));
    *size = strlen(str);
    if ( *size > maxsize - 1) {
      return LDH__NAMEBUF;
    }
    strcpy( buf, str);
    break;
  }
  default:
  {
    wb_object o = sp->object(oid);
    if (!o) { /* return o.sts(); */
      strncpy( buf, cdh_ObjidToString( NULL, oid, 1), maxsize);
      *size = strlen( buf);
      return LDH__SUCCESS;
    }
    try {
      char name[200];
      strcpy( name, o.longName().name( type));
      *size = strlen( name);
      if ( *size > maxsize - 1) {
        return LDH__NAMEBUF;
      }
      strcpy( buf, name);
    }
    catch ( wb_error& e) {
      return e.sts();
    }
    break;
  }
  }
  return LDH__SUCCESS;
}

/* Returns the name of a type identified by its type identifier.
   The caller is responsible for supplying a buffer at
   least as big as maxsize.  */

pwr_tStatus
ldh_TypeIdToName(ldh_tSession session, pwr_tTid tid, char *buff, int maxsize, int *size)
{
  wb_session *sp = (wb_session *)session;
  wb_tdef t = sp->tdef(tid);
  if (!t) return t.sts();
    
  //buff = t.name();
    
  return t.sts();
}

/* Returns the name of a class identified by its classid.
   The caller is responsible for supplying a buffer at
   least as big as maxsize.  */

pwr_tStatus
ldh_ClassIdToName(ldh_tSession session, pwr_tCid cid, char *buff, int maxsize, int *size)
{
  wb_session *sp = (wb_session *)session;
  wb_cdef c = sp->cdef(cid);
  if (!c) return c.sts();
    
  strncpy( buff, c.name(), maxsize);
  *size = strlen( buff);
    
  return c.sts();
}

/*  Returns the name of an object attribute identified by an
    attribute reference. A pointer to the name and the size
    of it is returned.  */

pwr_tStatus
ldh_AttrRefToName(ldh_tSession session, pwr_sAttrRef *arp, int nametype, char **aname, int *size)
{
  static char str[512];
  wb_session *sp = (wb_session *)session;
    
  try {
    
  switch ( nametype) {
  case ldh_eName_ArefExport:
  case ldh_eName_Objid:
  case ldh_eName_ObjectIx:
  case ldh_eName_OixString:
  case ldh_eName_VolumeId:
  case ldh_eName_VidString: {
    wb_name n = wb_name( cdh_ArefToString( NULL, arp, 1));
    strcpy( str, n.name( nametype));
    *aname = str;
    *size = strlen(str);
    break;
  }
  case ldh_eName_Ref: {
    wb_attribute a = sp->attribute(arp);
    if (!a) return a.sts();
    
    wb_name n = a.longName();
    strcpy( str, a.name());
    strcat( str, ".");
    strcat( str, n.attributesAll());
    *aname = str;
    *size = strlen(str);
    break;
  }
  case 0: { // cdh_mNName
    if ( arp->Objid.vid == sp->vid())
      nametype = cdh_mName_path | cdh_mName_attribute;
    else
      nametype = cdh_mName_volume | cdh_mName_attribute;

    wb_attribute a = sp->attribute(arp);
    if (!a) return a.sts();
    
    wb_name n = a.longName();
    strcpy( str, n.name( nametype));
    *aname = str;
    *size = strlen(str);
    break;
  }
  default: {
    wb_attribute a = sp->attribute(arp);
    if (!a) return a.sts();
    
    wb_name n = a.longName();
    strcpy( str, n.name( nametype));
    *aname = str;
    *size = strlen(str);
    break;
  }
  }
  }
  catch (wb_error& e) {
    return e.sts();
  }  
  return LDH__SUCCESS;
}

/*  Convert a volume identity to a corresponding name.
    
If the name of the volume is not available, the name
is given in an alphanumerical form:

_V0.123.34.63

*/
 
pwr_tStatus
ldh_VolumeIdToName(ldh_tWorkbench workbench, pwr_tVid vid, char *name, int maxsize, int *size)
{
  wb_env *wb = (wb_env *)workbench;
  wb_volume v = wb->volume(vid);
  if (!v) return v.sts();

  *size = strlen(v.name());
  strncpy( name, v.name(), maxsize);
  return LDH__SUCCESS;
}


void
ldh_AddThisSessionCallback(ldh_tSession session, void *editorContext,
                           ldh_tSessionCb receiveThisSession)
{
  wb_session *sp = (wb_session *)session;
  sp->editorContext(editorContext);
  sp->sendThisSession(receiveThisSession);
}


void
ldh_AddOtherSessionCallback(ldh_tSession session, void *editorContext,
                            ldh_tSessionCb receiveOtherSession)
{
  wb_session *sp = (wb_session *)session;
  sp->editorContext(editorContext);
  sp->sendOtherSession(receiveOtherSession);
}


pwr_tStatus
ldh_OpenSession(ldh_tSession *session, ldh_tVolume volume,
                ldh_eAccess access, ldh_eUtility utility)
{
  wb_volume *vp = (wb_volume *)volume;
    
  if (!*vp) return vp->sts();

  // wb_srep *srep = new wb_srep(vp);
  wb_session *sp = new wb_session(*vp);
    
  sp->access(access);
  if ( sp->evenSts()) {
    pwr_tStatus sts = sp->sts();
    delete sp;
    return sts;
  }
  sp->utility(utility);
    
  *session = (ldh_tSession)sp;

  return sp->sts();
}

/* This routine creates a new memory resident workbench and populates
   it with objects from database on disk.  */

pwr_tStatus
ldh_OpenWB(ldh_tWorkbench *workbench, char *db, unsigned int options)
{
  wb_erep *erep = new wb_erep( options);
  wb_env *env = new wb_env(erep);
  env->load( db);

  *workbench = (ldh_tWorkbench)env;
  return env->sts();
}

pwr_tStatus
ldh_ReadAttribute(ldh_tSession session, pwr_sAttrRef *arp, void *value, int size)
{
  wb_session *sp = (wb_session*)session;

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
    
  a.value( value);

  return LDH__SUCCESS;
}

/* Reads a named body of an object into a buffer supplied in the call.  */

pwr_tStatus
ldh_ReadObjectBody(ldh_tSession session, pwr_tObjid oid, char *bname, void *value, int size)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute( oid, bname);
  if ( !a) return a.sts();

  a.value( value);
  
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_RevertSession(ldh_tSession session)
{
  wb_session *sp = (wb_session*)session;

  return sp->abort();
}

/* Save all changes to objects done in the session.  */

pwr_tStatus
ldh_SaveSession(ldh_tSession session)
{
  wb_session *sp = (wb_session*)session;

  return sp->commit();
}

ldh_tVolume
ldh_SessionToVol(ldh_tSession session)
{
  wb_session *sp = (wb_session*)session;

    
  return (ldh_tVolume) sp;
}

ldh_tWorkbench
ldh_SessionToWB(ldh_tSession session)
{
  wb_session *sp = (wb_session *)session;
    
  wb_env *e = new wb_env(sp->env());         // Fix ... this is never deleted !!!
  return (ldh_tWorkbench) e;
}

/* Updates a named body of an object.  */

pwr_tStatus
ldh_SetObjectBody(ldh_tSession session, pwr_tOid oid, char *bname, char *value, int size)
{
  wb_session *sp = (wb_session *)session;
  wb_attribute a = sp->attribute( oid, bname);
  if ( !a) return a.sts();

  try {
    sp->writeAttribute(a, value);
    return sp->sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }  
}

pwr_tStatus
ldh_SetObjectBuffer(ldh_tSession session, pwr_tOid oid, char *bname, char *aname, char *value)
{
  wb_session *sp = (wb_session*)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();
  
  wb_attribute a = o.attribute(bname, aname);
  if (!a) return a.sts();
    
  try {
    sp->writeAttribute(a, value);
    return sp->sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }  
}


/* The same as ChangeObjectName but without notification.  */

pwr_tStatus
ldh_SetObjectName(ldh_tSession session, pwr_tOid oid, char *name)
{
  wb_session *sp = (wb_session*)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  try {
    wb_name n(name);
    if ( !n) return n.sts();

    sp->renameObject(o, n);
  }
  catch ( wb_error& e) {
    return e.sts();
  }
  return sp->sts();
}

pwr_tStatus
ldh_SetObjectPar(ldh_tSession session, pwr_tOid oid, char *bname,
                 char *aname, char *value, int size)
{
  wb_session *sp = (wb_session*)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();

  wb_attribute a = o.attribute(bname, aname);
  if (!a) return a.sts();


  try {
    sp->writeAttribute(a, value, size);
    return sp->sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }  
}

pwr_tStatus
ldh_SetSession(ldh_tSession session, ldh_eAccess access)
{
  wb_session *sp = (wb_session*)session;
  sp->access(access);

  return sp->sts();
}

pwr_tStatus
ldh_StringGetAttribute(ldh_tSession session, pwr_sAttrRef *arp, pwr_tUInt32 maxsize, char *string)
{
  wb_session *sp = (wb_session*)session;

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
    
  if ( strlen(a.longName().c_str()) >= maxsize)
    return LDH__NAMEBUF;
  strncpy( string, a.longName().c_str(), maxsize);
  return LDH__SUCCESS;
}

/* If write is false this routine only checks the string.  */
pwr_tStatus
ldh_StringSetAttribute(ldh_tSession session, pwr_sAttrRef *arp, char *string, pwr_tBoolean write)
{
  wb_session *sp = (wb_session*)session;
  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
    
  //return a.stringToValue(string, write);
  return LDH__NYI;
}

/*  Writes an attribute of an object from a buffer supplied in the call.  */

pwr_tStatus
ldh_WriteAttribute(ldh_tSession session, pwr_sAttrRef *arp, void *value, int size)
{
  wb_session *sp = (wb_session*)session;
  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();

  try {
    sp->writeAttribute(a, value);
    return sp->sts();
  }
  catch (wb_error& e) {
    return e.sts();
  }  
  return LDH__SUCCESS;
}

/*  Returns 1 if object is owned by the volume attached to
    the current session.  */

pwr_tBoolean
ldh_LocalObject(ldh_tSession session, pwr_tOid oid)
{
  wb_session *sp = (wb_session*)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  return sp->isLocal(o);
}

pwr_tStatus
ldh_SyntaxCheck(ldh_tSession session, int *errorcount, int *warningcount)
{
  wb_session *sp = (wb_session*)session;
  pwr_tStatus sts;

  sts = sp->syntaxCheck( errorcount, warningcount);
  return sts;
}

pwr_tStatus
ldh_CopyObjectTrees(ldh_tSession session, pwr_sAttrRef *arp, pwr_tOid doid, ldh_eDest dest, pwr_tBoolean self, int keepref)
{
  pwr_tStatus sts;

  sts = ldh_Copy( session, arp, keepref, false);
  if (EVEN(sts)) return sts;

  sts = ldh_Paste( session, doid, dest, 0, 0);  
  return sts;
}


/* Make a copy of object trees pointed at by AREF and
   put the copy in a pastebuffer associated whith the
   workbench context. The original object trees are
   left untouched.  */

pwr_tStatus
ldh_Copy(ldh_tSession session, pwr_sAttrRef *arp, int keepref, int ignore_errors)
{
  wb_session *sp = (wb_session*)session;

  try {
    sp->copyOset( arp, (keepref != 0), (ignore_errors != 0));
  }
  catch (wb_error& e) {
    return e.sts();
  }
  return sp->sts();
}

/* Make a copy of object trees pointed at by AREF and
   put the copy in a pastebuffer associated whith the
   workbench context. The original object trees are
   deleted but can be retrieved by reverting. */

pwr_tStatus
ldh_Cut(ldh_tSession session, pwr_sAttrRef *arp, int keepref)
{
  wb_session *sp = (wb_session*)session;

  try {
    sp->cutOset( arp, (keepref != 0));
  }
  catch (wb_error& e) {
    return e.sts();
  }
  return sp->sts();
}

pwr_tStatus
ldh_Paste(ldh_tSession session, pwr_tOid doid, ldh_eDest dest, int keepoid, char *buffer)
{
  wb_session *sp = (wb_session*)session;

  try {
    sp->pasteOset( doid, dest, (keepoid != 0), buffer);
  }
  catch (wb_error& e) {
    return e.sts();
  }
  return sp->sts();
}

pwr_tStatus
ldh_CreateLoadFile(ldh_tSession session)
{
  wb_session *sp = (wb_session*)session;

  try {
    sp->createSnapshot( 0, 0);
  }
  catch (wb_error& e) {
    return e.sts();
  }
  return sp->sts();
}

pwr_tStatus 
ldh_WbDump( ldh_tSession session, char *objname, char *dumpfile, int keep_name, 
	    int noindex, int nofocode) 
{
  wb_session *sp = (wb_session*)session;
  char fname[200];

  if ( sp->type() == ldh_eVolRep_Wbl 
       // || sp->cid() == pwr_eClass_ClassVolume
       )
    return LDH__NYI;

  dcli_translate_filename( fname, dumpfile);
  ofstream fp( fname);
  if ( !fp) return LDH__FILEOPEN;

  try {
    wb_print_wbl wprint( fp);
    if ( keep_name)
      wprint.keepName();
    if ( noindex)
      wprint.noIndex();
    if ( nofocode)
      wprint.noFoCode();
    if ( !objname)
      wprint.printVolume( *sp);
    else {
      wb_object o = sp->object( objname);
      wprint.printHierarchy( *sp, o);
    }
  }
  catch ( wb_error& e) {
    return e.sts();
  }
  return LDH__SUCCESS;
}

pwr_tStatus 
ldh_WbLoad( ldh_tSession session, char *loadfile, int ignore_oix) 
{
  wb_session *sp = (wb_session*)session;
  wb_erep *erep = sp->env();
  char fname[200];
  char db_name[200];
  char vname[32];

  if ( strstr( loadfile, ".dbs")) {
    // Load vrepdbs
    try {
      dcli_translate_filename( fname, loadfile);
      wb_vrepdbs *vdbs = new wb_vrepdbs( erep, fname);
      vdbs->load();

      cdh_ToLower( vname, vdbs->name());
      strcpy( db_name, "$pwrp_db/");
      strcat( db_name, vname);
      strcat( db_name, ".db");
      dcli_translate_filename( db_name, db_name);
	  
      // wb_db db( vdbs->vid());
      // db.create( vdbs->vid(), vdbs->cid(), vdbs->name(), db_name);
      // db.importVolume( *vdbs);
      wb_db db( pwr_cNVid);
      db.copy( *vdbs, db_name);      
      db.close();
      erep->merep()->copyFiles( db_name);

      delete vdbs;      
    }
    catch ( wb_error& e) {
      return e.sts();
    }
  }
  else if ( strstr( loadfile, ".wb_load") || strstr( loadfile, ".wb_dmp")) {
    // load vrepwbl
    try {
      dcli_translate_filename( fname, loadfile);
      wb_vrepwbl *vwbl = new wb_vrepwbl( erep);
      if ( ignore_oix)
	vwbl->ignoreOix();
      vwbl->load( fname);

      cdh_ToLower( vname, vwbl->name());
      strcpy( db_name, "$pwrp_db/");
      strcat( db_name, vname);
      strcat( db_name, ".db");
      dcli_translate_filename( db_name, db_name);
	  
      // wb_db db( vwbl->vid());
      // db.create( vwbl->vid(), vwbl->cid(), vwbl->name(), db_name);
      // db.importVolume( *vwbl);
      wb_db db( pwr_cNVid);
      db.copy( *vwbl, db_name);
      db.close();
      erep->merep()->copyFiles( db_name);

      delete vwbl;      
    }
    catch ( wb_error& e) {
      return e.sts();
    }
  }
  else
    return LDH__NYI;
  return LDH__SUCCESS;
}

ldh_eVolRep
ldh_VolRepType( ldh_tSession session)
{
  return ((wb_session*)session)->type();
}
	      
pwr_tStatus
ldh_GetDocBlock(ldh_tSession session, pwr_tOid oid, char **block, int *size)
{
  wb_session *sp = (wb_session*)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();
  
  if ( o.docBlock( block, size))
    return LDH__SUCCESS;

  return LDH__NOSUCHBUFFER;
}

pwr_tStatus
ldh_SetDocBlock(ldh_tSession session, pwr_tOid oid, char *block)
{
  wb_session *sp = (wb_session*)session;

  wb_object o = sp->object(oid);
  if (!o) return o.sts();
 
  if ( o.docBlock( block))
    return LDH__SUCCESS;

  return LDH__NOSUCHBUFFER;
}

pwr_tStatus
ldh_GetAttrRefInfo(ldh_tSession session, pwr_sAttrRef *arp, ldh_sAttrRefInfo *info)
{
  wb_session *sp = (wb_session *)session;
    
  try {
  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
    
  info->size = a.size();
  info->nElement = a.nElement();
  info->index = a.index();
  info->flags = a.flags();
  info->type = a.type();
  info->tid = a.tid();
  }
  catch ( wb_error& e) {
    return e.sts();
  }

  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetSuperClass( ldh_tSession session, pwr_tCid cid, pwr_tCid *super)
{
  wb_session *sp = (wb_session *)session;

  wb_cdef c = sp->cdef(cid);
  if (!c) return c.sts();

  wb_cdef csuper = c.super();
  if (!csuper) return csuper.sts();

  *super = csuper.cid();
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetMaskBitDef( ldh_tSession session, pwr_tTid tid, ldh_sBitDef **bitdef,
		   int *rows)
{
  wb_session *sp = (wb_session *)session;

  wb_object to = sp->object( cdh_TypeIdToObjid( tid));
  if (!to) return to.sts();

  int bit_cnt = 0;
  for ( wb_object bito = to.first(); bito; bito = bito.after()) {
    if ( bito.cid() == pwr_eClass_Bit)
      bit_cnt++;
  }
  if ( !bit_cnt)
    return LDH__NOTYPE;

  *bitdef = (ldh_sBitDef *) calloc( bit_cnt, sizeof(ldh_sBitDef));
  *rows = 0;
  for ( wb_object bito = to.first(); bito; bito = bito.after()) {
    if ( bito.cid() == pwr_eClass_Bit) {
      wb_attribute a = sp->attribute( bito.oid(), "SysBody");
      if ( !a) return a.sts();

      strcpy( (*bitdef)[*rows].Name, bito.name());
      (*bitdef)[*rows].Bit = (pwr_sBit *) a.value();
      (*rows)++;
    }
  }
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_GetEnumValueDef( ldh_tSession session, pwr_tTid tid, ldh_sValueDef **valuedef,
		   int *rows)
{
  wb_session *sp = (wb_session *)session;

  wb_object to = sp->object( cdh_TypeIdToObjid( tid));
  if (!to) return to.sts();

  int val_cnt = 0;
  for ( wb_object valo = to.first(); valo; valo = valo.after()) {
    if ( valo.cid() == pwr_eClass_Value)
      val_cnt++;
  }
  if ( !val_cnt)
    return LDH__NOTYPE;

  *valuedef = (ldh_sValueDef *) calloc( val_cnt, sizeof(ldh_sValueDef));
  *rows = 0;
  for ( wb_object valo = to.first(); valo; valo = valo.after()) {
    if ( valo.cid() == pwr_eClass_Value) {
      wb_attribute a = sp->attribute( valo.oid(), "SysBody");
      if ( !a) return a.sts();

      strcpy( (*valuedef)[*rows].Name, valo.name());
      (*valuedef)[*rows].Value = (pwr_sValue *) a.value();
      (*rows)++;
    }
  }
  return LDH__SUCCESS;
}

pwr_tStatus
ldh_CastAttribute(ldh_tSession session, pwr_sAttrRef *arp, pwr_tCid cid)
{
  wb_session *sp = (wb_session *)session;
    
  sp->castAttribute( arp, cid);
  return sp->sts();
}

pwr_tStatus
ldh_DisableAttribute(ldh_tSession session, pwr_sAttrRef *arp, pwr_tDisableAttr disable)
{
  wb_session *sp = (wb_session *)session;
    
  sp->disableAttribute( arp, disable);
  return sp->sts();
}

pwr_tStatus
ldh_GetSubClass(ldh_tSession session, pwr_tCid supercid, pwr_tCid subcid, pwr_tCid *nextsubcid)
{
  wb_session *sp = (wb_session *)session;

  sp->subClass( supercid, subcid, nextsubcid);
  return sp->sts();
}

pwr_tStatus
ldh_GetModTime(ldh_tSession session, pwr_tOid oid, pwr_tTime *time)
{
  wb_session *sp = (wb_session *)session;
  wb_object o = sp->object(oid);
  if (!o) return o.sts();
    
  *time = o.modTime();

  return o.sts();
}

pwr_tStatus
ldh_AttributeDisabled(ldh_tSession session, pwr_sAttrRef *arp, pwr_tDisableAttr *disabled)
{
  wb_session *sp = (wb_session*)session;

  wb_attribute a = sp->attribute(arp);
  if (!a) return a.sts();
    
  *disabled = a.disabled();

  return LDH__SUCCESS;
}

#endif







