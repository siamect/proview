/* 
 * Proview   $Id: wb_session.cpp,v 1.24 2007-12-06 10:55:04 claes Exp $
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

#include "wb_session.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh.h"
#include "wb_vrepmem.h"
#include "wb_recix.h"
#include "pwr_baseclasses.h"

static ldh_sMenuItem ldh_lMenuItem[100];

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
  {"ObjectAttr",       	'o',  ldh_eMenuSet_ObjectAttr},
  {"Array",       	'y',  ldh_eMenuSet_Array},
  {"-",			'\0', ldh_eMenuSet_}
};

wb_session::wb_session(wb_volume &v) : wb_volume(v)
{
  m_srep = new wb_srep( v);
  m_srep->ref();
}

wb_session::wb_session(wb_session &s): wb_volume(s.m_vrep), m_srep(s.m_srep)
{
  if ( m_srep)
    m_srep->ref();
}

wb_session::~wb_session()
{
  m_srep->unref();
}

wb_session& wb_session::operator=(const wb_session& x)
{
  if ( x.m_srep)
    x.m_srep->ref();
  if ( m_srep)
    m_srep->unref();
  m_srep = x.m_srep;
  m_sts = x.sts();
  return *this;
}

wb_object wb_session::createObject(wb_cdef cdef, wb_destination d, wb_name name)
{
  wb_orep *orep = 0;
    
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  validateDestination(d, cdef.cid());
  if (evenSts()) throw wb_error(sts());

  wb_object parent;
  switch (d.code()) {
  case ldh_eDest_IntoFirst:
  case ldh_eDest_IntoLast:
    parent = object(d.oid());
    break;
  case ldh_eDest_After:
  case ldh_eDest_Before:
    parent = object(d.oid()).parent();
    break;
  default:
    throw wb_error(LDH__NODEST);
  }

  m_sts = triggAnteCreate(parent, cdef.cid());
  if (evenSts()) throw wb_error(sts());
  m_sts = triggAnteAdopt(parent, cdef.cid());
  if (evenSts()) throw wb_error(sts());

  orep = m_vrep->createObject(&m_sts, cdef, d, name);
  if (evenSts()) throw wb_error(sts());
  m_srep->update();

  wb_object o = wb_object(m_sts, orep);
  ldh_sEvent *ep = m_srep->eventStart(o.oid(), ldh_eEvent_ObjectCreated);
  m_srep->eventNewFamily(ep, o);
  triggPostCreate(o);
  triggPostAdopt(parent, o);
  m_srep->eventSend(ep);
  return o;
}

wb_object wb_session::copyObject(wb_object o, wb_destination d, wb_name name)
{
  wb_orep *orep = 0;
    
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  validateDestination(d, o.cid());
  if (evenSts()) throw wb_error(sts());

  wb_object parent;
  switch ( d.code()) {
  case ldh_eDest_IntoFirst:
  case ldh_eDest_IntoLast:
    parent = object( d.oid());
    break;
  case ldh_eDest_After:
  case ldh_eDest_Before:
    parent = object( d.oid()).parent();
    break;
  default:
    throw wb_error(LDH__NODEST);
  }

  m_sts = triggAnteCreate(parent, o.cid());
  if (evenSts()) throw wb_error(sts());
  m_sts = triggAnteAdopt(parent, o.cid());
  if (evenSts()) throw wb_error(sts());

  if ( m_vrep->vid() == o.vid()) {
    orep = m_vrep->copyObject(&m_sts, (wb_orep*)o, d, name);
    if (evenSts()) throw wb_error(sts());
    
    orep->ref();
  }
  else {
    wb_cdef c = cdef( o.cid());

    orep = m_vrep->createObject(&m_sts, c, d, name); 
    orep->ref();
    wb_attribute rba(o.sts(), (wb_orep*)o, "RtBody");
    if ( rba) {
      void *p = rba.value();
      wb_attribute rban(m_sts, orep, "RtBody");
      writeAttribute( rban, p);
    }
    wb_attribute dba(o.sts(), (wb_orep*)o, "DevBody");
    if ( dba) {
      void *p = dba.value();
      wb_attribute dban(m_sts, orep, "DevBody");
      writeAttribute( dban, p);
    }
    wb_attribute sba(o.sts(), (wb_orep*)o, "SysBody");
    if ( sba) {
      void *p = sba.value();
      wb_attribute sban(m_sts, orep, "SysBody");
      writeAttribute( sban, p);
    }
  }
  m_srep->update();

  wb_object onew = wb_object(m_sts, orep);
  orep->unref();
  ldh_sEvent *ep = m_srep->eventStart(onew.oid(), ldh_eEvent_ObjectCreated);
  m_srep->eventNewFamily(ep, onew);
  triggPostCreate(onew);
  triggPostAdopt(parent, onew);
  m_srep->eventSend(ep);

  return onew;
}

bool wb_session::moveObject(wb_object o, wb_destination d)
{   
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  validateDestination(d, o.cid());
  if (evenSts()) return false;

  // Check that object is not ancestor to destination
  wb_object dest = object(d.oid());
  while ( dest) {
    if ( cdh_ObjidIsEqual( dest.oid(), o.oid()))
      throw wb_error(LDH__BADDEST);
    dest = dest.parent();
  }

  wb_object parent;
  switch (d.code()) {
  case ldh_eDest_IntoFirst:
  case ldh_eDest_IntoLast:
    parent = object(d.oid());
    break;
  case ldh_eDest_After:
  case ldh_eDest_Before:
    parent = object(d.oid()).parent();
    break;
  default:
  throw wb_error(LDH__NODEST);
  }

  wb_object old_parent = o.parent();
  m_sts = triggAnteMove(o, parent, old_parent);
  if (evenSts()) return false;
  m_sts = triggAnteUnadopt(old_parent, o);
  if (evenSts()) return false;
  m_sts = triggAnteAdopt(parent, o.cid());
  if (evenSts()) return false;

  ldh_sEvent *ep = m_srep->eventStart(o.oid(), ldh_eEvent_ObjectMoved);
  m_srep->eventOldFamily(ep, o);

  bool rsts = m_vrep->moveObject(&m_sts, (wb_orep*)o, d);
  m_srep->update();

  triggPostMove(o);
  triggPostUnadopt(old_parent, o);
  triggPostAdopt(parent, o);

  m_srep->eventNewFamily( ep, o);
  m_srep->eventSend( ep);

  return rsts;
}


bool wb_session::renameObject(wb_object o, wb_name name)
{
  m_sts = LDH__SUCCESS;
  
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool ok = m_vrep->renameObject(&m_sts, (wb_orep*)o, name);
  if (!ok) return ok;

  m_srep->update();
  ldh_sEvent *ep = m_srep->eventStart(o.oid(), ldh_eEvent_ObjectRenamed);
  m_srep->eventSend(ep);
  return ok;
}

bool wb_session::deleteObject(wb_object o)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  if (!isLocal(o)) {
    m_sts = LDH__OTHERVOLUME;
    return false;
  }
  if (o.first()) {
    m_sts = LDH__HAS_CHILD;
    return false;
  }

  pwr_tStatus sts;
  wb_object parent = o.parent();
  sts = triggAnteUnadopt(parent, o);

  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectDeleted);
  m_srep->eventOldFamily( ep, o);

  sts = triggPostUnadopt( parent, o);

  bool rsts = m_vrep->deleteObject(&m_sts, (wb_orep*)o);
  m_srep->update();
  m_srep->eventSend( ep);
  return rsts;
}

bool wb_session::deleteFamily(wb_object o, bool storeix)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  if (!isLocal( o)) {
    m_sts = LDH__OTHERVOLUME;
    return false;
  }

  pwr_tStatus sts;
  wb_object parent = o.parent();
  sts = triggAnteUnadopt( parent, o);

  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectDeleted);
  m_srep->eventOldFamily( ep, o);

  sts = triggPostUnadopt( parent, o);

  if ( storeix)
    m_srep->recix_add( o);
  bool rsts = m_vrep->deleteFamily(&m_sts, (wb_orep*)o);
  m_srep->update();
  m_srep->eventSend( ep);
  return rsts;
}

bool wb_session::writeAttribute(wb_attribute &a, void *p, size_t size)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool sts =  m_vrep->writeAttribute(&m_sts, (wb_orep*)a, a.bix(), a.offset(), a.size(), p);
  m_srep->update();
  ldh_sEvent *ep = m_srep->eventStart( a.aoid(), ldh_eEvent_AttributeModified);
  m_srep->eventSend( ep);
  return sts;
}

bool wb_session::writeAttribute(wb_attribute &a, void *p)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool sts =  m_vrep->writeAttribute(&m_sts, (wb_orep*)a, a.bix(), a.offset(), a.size(), p);
  m_srep->update();

  ldh_sEvent *ep = m_srep->eventStart( a.aoid(), ldh_eEvent_AttributeModified);
  m_srep->eventSend( ep);
  return sts;
}

bool wb_session::copyOset( pwr_sAttrRef *arp, bool keepref, bool ignore_errors)
{
  char name[32];
  pwr_tStatus sts;
  m_sts = LDH__SUCCESS;

  // Avoid copying objects in plcprograms
  pwr_sAttrRef *ap = arp;
  while ( cdh_ObjidIsNotNull( ap->Objid)) {
    wb_object o = object( ap->Objid);
    if ( !o) return o.sts();
    o = o.parent();
    while ( o) {
      pwr_sAttrRef *ap2 = arp;
      while ( cdh_ObjidIsNotNull( ap2->Objid)) {
	if ( o.cid() == pwr_cClass_plc) {
	  m_sts = LDH__COPYPLCOBJECT;
	  return false;
	}
	ap2++;
      }
      o = o.parent();
    }
    ap++;
  }

  pwr_tVid vid = m_vrep->erep()->nextVolatileVid( &m_sts, name);
  if ( EVEN(m_sts)) return false;

  wb_vrepmem *mem = new wb_vrepmem( m_vrep->erep(), vid);
  mem->name( name);
  m_vrep->erep()->addBuffer( &sts, mem);
  if ( ignore_errors)
    mem->importIgnoreErrors();

  ap = arp;
  while ( cdh_ObjidIsNotNull( ap->Objid)) {

    // Check selected object is not child to another selected object
    bool found = false;
    wb_object o = object( ap->Objid);
    if ( !o) return o.sts();
    o = o.parent();
    while ( o) {
      pwr_sAttrRef *ap2 = arp;
      while ( cdh_ObjidIsNotNull( ap2->Objid)) {
	if ( cdh_ObjidIsEqual( ap2->Objid, o.oid())) {
	  found = true;
	  break;
	}
	ap2++;
      }
      if ( found)
	break;
      o = o.parent();
    }
    if ( found) {
      ap++;
      continue;
    }

    if ( m_vrep->vid() == ap->Objid.vid)
      m_vrep->exportTree( *mem, ap->Objid);
    else {
      // Other volume
      wb_vrep *vrep = m_vrep->erep()->volume( &sts, ap->Objid.vid);
      vrep->exportTree( *mem, ap->Objid);
    }
    ap++;
  }
  return mem->importTree( keepref);  
}

bool wb_session::cutOset( pwr_sAttrRef *arp, bool keepref)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  m_sts = LDH__SUCCESS;

  copyOset( arp, keepref, false);
  if ( EVEN(m_sts)) return false;

  //ldh_sEvent *ep = m_srep->eventStart( pwr_cNOid, ldh_eEvent_ObjectTreeDeleted);

  pwr_sAttrRef *ap = arp;
  while ( cdh_ObjidIsNotNull( ap->Objid)) {
    wb_object o = object( ap->Objid);
    if ( !o) {
      ap++;
      continue;
    }

    ldh_sEvent *ep = m_srep->eventStart(o.oid(), ldh_eEvent_ObjectDeleted);
    m_srep->eventOldFamily(ep, o);
    deleteFamily(o, 0);
    m_srep->eventSend(ep);

    ap++;
  }
  m_srep->update();
  //m_srep->eventSend( ep);

  return true;
}

bool wb_session::pasteOset( pwr_tOid doid, ldh_eDest dest, 
			    bool keepoid, bool recycleix, char *buffer)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  m_sts = LDH__SUCCESS;
  wb_destination d = wb_destination( doid, dest);

  wb_vrepmem *mem;
  // Get last buffer
  if ( !buffer) {
    mem = (wb_vrepmem *) m_vrep->erep()->bufferVolume( &m_sts);
    if ( EVEN(m_sts)) return false;

    wb_vrepmem *prev;
    while ( mem) {
      prev = mem;
      mem = (wb_vrepmem *)mem->next();
    }
    mem = prev;
  }
  else {
    // Get specified buffer
    mem = (wb_vrepmem *) m_vrep->erep()->bufferVolume( &m_sts, buffer);
    if ( EVEN(m_sts)) return false;
  }

  if ( mem == m_vrep) {
    m_sts = LDH__PASTESELF;
    return false;
  }

  // Check that rootobjects are valid for this destination
  pwr_tStatus sts;
  wb_orep *orep = mem->object( &sts);
  while( ODD(sts)) {
    orep->ref();
    validateDestination( d, orep->cid());
    if ( evenSts()) return false;
    wb_orep *prev = orep;
    orep = orep->after( &sts);
    prev->unref();
  }

  // Trigg ante adopt
  wb_object parent;
  switch ( dest) {
  case ldh_eDest_IntoFirst:
  case ldh_eDest_IntoLast:
    parent = object( doid);
    break;
  case ldh_eDest_After:
  case ldh_eDest_Before:
    parent = object( doid).parent();
    break;
  default:
  throw wb_error(LDH__NODEST);
  }

  if ( parent) {
    wb_orep *orep = mem->object( &sts);
    while( ODD(sts)) {
      orep->ref();
      m_sts = triggAnteAdopt( parent, orep->cid());
      if ( evenSts()) return false;
      wb_orep *prev = orep;
      orep = orep->after( &sts);
      prev->unref();
    }
  }

  wb_recix *recix = 0;
  if ( recycleix)
    recix = m_srep->recix();
  pwr_tOid *olist;
  mem->exportPaste( *m_vrep, doid, dest, keepoid, recix, &olist);
  m_srep->update();
  if ( recycleix)
    m_srep->recix_clear();

  if ( parent) {
    for ( pwr_tOid *oidp = olist; cdh_ObjidIsNotNull(*oidp); oidp++) {
      wb_object o = object(*oidp);
      triggPostAdopt( parent, o);
    }
  }
  free( olist);

  if ( dest == ldh_eDest_After || dest == ldh_eDest_Before)
    doid = pwr_cNOid;
  ldh_sEvent *ep = m_srep->eventStart( doid, ldh_eEvent_ObjectTreeCopied);
  m_srep->eventSend( ep);

  return true;
}

pwr_tStatus wb_session::getMenu( ldh_sMenuCall *ip)
{
  pwr_tStatus		sts;
  ldh_sMenuItem		*Item = (ldh_sMenuItem *) &ldh_lMenuItem;
  pwr_tUInt32		i;
  pwr_tObjName		MenuFolder;
  pwr_tString80		Menu;
  pwr_tBoolean		isSame = FALSE;
  pwr_tClassId		Class;
  pwr_tObjid		Object;
  int			nItems = 0;
  wb_name               cn;

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsEqual(ip->Pointed.Objid, ip->Selected[i].Objid) &&
	ip->Pointed.Offset == ip->Selected[i].Offset &&
	ip->Pointed.Flags.m == ip->Selected[i].Flags.m) {
      isSame = TRUE;
      break;
    }
  }

  sprintf(MenuFolder, "%sP%cs%c%c",
    ldh_lUtility[((wb_session *)ip->PointedSession)->utility()].Name,
    ldh_lMenuSet[ip->PointedSet].Char,
    ldh_lMenuSet[ip->SelectedSet].Char,
    (isSame ? 's' : 'n')
  );
  /* Find generic menues of pointed object */
  switch ( ip->PointedSet) {
  case ldh_eMenuSet_Object: {
    sprintf(Menu, "pwrs:Class-$Object-%s-Pointed", MenuFolder);

    wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, pwr_eClass_Object);
    if ( EVEN(sts)) return sts;

    wb_orep *o = m_vrep->erep()->object( &sts, Menu);
    if ( EVEN(sts)) return sts;
    o->ref();

    Object = o->oid();
    void *o_menu_body;

    wb_orep *o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
    while ( ODD(sts)) {
      o_menu->ref();
      getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
      wb_orep *prev = o_menu; 
      o_menu = cdrep->menuAfter( &sts, o_menu, &o_menu_body);
      prev->unref();
    }
    delete cdrep;
    o->unref();
    break;
  }
  case ldh_eMenuSet_ObjectAttr: {
    pwr_tObjName OMenuFolder;

    sprintf(OMenuFolder, "%sP%cs%c%c",
	    ldh_lUtility[((wb_session *)ip->PointedSession)->utility()].Name,
	    'x', 'x', 'x');
    sprintf(Menu, "pwrs:Class-$Object-%s-Pointed", OMenuFolder);

    wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, pwr_eClass_Object);
    if ( EVEN(sts)) return sts;

    wb_orep *o = m_vrep->erep()->object( &sts, Menu);
    if ( EVEN(sts)) return sts;
    o->ref();

    Object = o->oid();
    void *o_menu_body;

    wb_orep *o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
    while ( ODD(sts)) {
      o_menu->ref();
      getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
      wb_orep *prev = o_menu; 
      o_menu = cdrep->menuAfter( &sts, o_menu, &o_menu_body);
      prev->unref();
    }
    delete cdrep;
    o->unref();
    break;
  }
  default:
    ;
  }
  /* Find specific menues of pointed object */

  switch ( ip->PointedSet) {
  case ldh_eMenuSet_Object:
  case ldh_eMenuSet_ObjectAttr: {
    if (ip->PointedSet == ldh_eMenuSet_Class) {
      Class = cdh_ClassObjidToId(ip->Pointed.Objid);
    }
    else {
      wb_attribute a = attribute( &ip->Pointed);
      if ( !a) return sts;
      Class = a.tid();
    }

    wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, Class);
    while ( ODD(sts)) {

      cn = cdrep->longName();
      sprintf(Menu, "%s-%s-Pointed", cn.name(), MenuFolder);

      wb_orep *o = m_vrep->erep()->object( &sts, Menu);
      if ( ODD(sts)) {
	o->ref();
      
	Object = o->oid();

	void *o_menu_body;
	wb_orep *o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
	while ( ODD(sts)) {
	  o_menu->ref();
	  getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
	  wb_orep *prev = o_menu; 
	  o_menu = cdrep->menuAfter( &sts, o_menu, &o_menu_body);
	  prev->unref();
	}
	o->unref();
      }
      // Get menuitems for superclass
      wb_cdrep *super_cdrep = cdrep->super( &sts);
      if ( super_cdrep) {
	delete cdrep;
	cdrep = super_cdrep;      
      }
    }
    delete cdrep;
    break;
  }
  default:
    ;
  }

  switch (ip->SelectedSet) {
  case ldh_eMenuSet_Attribute:
  case ldh_eMenuSet_Array:
  case ldh_eMenuSet_ObjectAttr:
    break;
  case ldh_eMenuSet_Class:
  case ldh_eMenuSet_Many:
  case ldh_eMenuSet_Object: {
    if ( ip->PointedSet != ldh_eMenuSet_Object)
      break;

    /* Find generic menues for selected object(s) */
    sprintf(Menu, "pwrs:Class-$Object-%s-Selected", MenuFolder);
    wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, pwr_eClass_Object);
    if ( EVEN(sts)) return sts;

    wb_orep *o = m_vrep->erep()->object( &sts, Menu);
    if ( ODD(sts)) {
      o->ref();

      Object = o->oid();

      void *o_menu_body;
      wb_orep *o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
      while ( ODD(sts)) {
        o_menu->ref();
        getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
	wb_orep *prev = o_menu; 
        o_menu = cdrep->menuAfter( &sts, o_menu, &o_menu_body);
        prev->unref();
      }
      o->unref();
    }
    delete cdrep;

    /* Find specific menues for selected object(s) */
    if (ip->PointedSet == ldh_eMenuSet_Class) {
      Class = cdh_ClassObjidToId(ip->Pointed.Objid);
    }
    else {
      if ( m_vrep->vid() == ip->Pointed.Objid.vid)
	o = m_vrep->object( &sts, ip->Pointed.Objid);
      else
	// Other volume
	o = m_vrep->erep()->object( &sts, ip->Pointed.Objid);
      if ( EVEN(sts)) return sts;
      o->ref();
      Class = o->cid();
      o->unref();
    }

    cdrep = m_vrep->merep()->cdrep( &sts, Class);
    while ( ODD(sts)) {

      cn = cdrep->longName();
      sprintf(Menu, "%s-%s-Selected", cn.name(), MenuFolder);
    
      o = m_vrep->erep()->object( &sts, Menu);
      if ( ODD(sts)) {
	o->ref();

	Object = o->oid();

	void *o_menu_body;
	wb_orep *o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
	while ( ODD(sts)) {
	  o_menu->ref();
	  getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
	  wb_orep *prev = o_menu; 
	  o_menu = cdrep->menuAfter( &sts, o_menu, &o_menu_body);
	  prev->unref();
	}
	o->unref();
      }

      // Get menuitems for superclass
      wb_cdrep *super_cdrep = cdrep->super( &sts);
      if ( super_cdrep) {
	delete cdrep;
	cdrep = super_cdrep;      
      }
    }
    delete cdrep;

    break;
  }
  default:
    break;
  }

  Item->Level = 0;
  ip->ItemList = ldh_lMenuItem;
  ip->ItemCount = nItems - 1;
  return LDH__SUCCESS;
}

void wb_session::getAllMenuItems( ldh_sMenuCall	*ip, ldh_sMenuItem **Item, wb_cdrep *cdrep,
				  wb_orep *o, void *o_body, pwr_tUInt32 Level,
				  int *nItems, int AddSeparator)
{
  pwr_sMenuButton	*mbp;
  pwr_sMenuCascade	*mcp;
  wb_tMethod            method;
  wb_tMethod            filter;
  pwr_tStatus           sts;

  Level++;
  memset(*Item, 0, sizeof(**Item));

  if(AddSeparator) {
    (*Item)->Level = Level;
    (*Item)->Item = ldh_eMenuItem_Separator;
    (*Item)->MenuObject = pwr_cNObjid;
    (*Item)++;
    (*nItems)++;
  } 
  else if ( o->cid() == pwr_eClass_MenuButton) {
    mbp = (pwr_sMenuButton *)o_body;
    (*Item)->Level = Level;
    (*Item)->Item = ldh_eMenuItem_Button;
    (*Item)->MenuObject = o->oid();

    (*Item)->Flags.f.Sensitive = 0;
    if ( mbp->MethodName[0] != 0) {
      m_vrep->erep()->method( &sts, mbp->MethodName, &method);
      if ( ODD(sts))
        (*Item)->Flags.f.Sensitive = 1;
    }
    if ( (*Item)->Flags.f.Sensitive == 1) {
      m_vrep->erep()->method( &sts, mbp->FilterName, &filter);
      if ( ODD(sts))
        (*Item)->Flags.f.Sensitive = ((wb_tMethodMenuFilter) filter)( ip, mbp);
    }

    strcpy((*Item)->Name, mbp->ButtonName);
    (*Item)->Method = method;
    (*Item)++;
    (*nItems)++;
  } 
  else if ( o->cid() == pwr_eClass_MenuSeparator) {
    (*Item)->Level = Level;
    (*Item)->Item = ldh_eMenuItem_Separator;
    (*Item)->MenuObject = o->oid();
    (*Item)++;
    (*nItems)++;
  } else if (o->cid() == pwr_eClass_MenuCascade) {
    mcp = (pwr_sMenuCascade *)o_body;
    (*Item)->Level = Level;
    (*Item)->Item = ldh_eMenuItem_Cascade;
    (*Item)->Flags.f.Sensitive = 1;
    strcpy((*Item)->Name, mcp->ButtonName);
    (*Item)->MenuObject = o->oid();
    (*Item)++;
    (*nItems)++;
    
    void *child_body;
    wb_orep *child = cdrep->menuFirst( &sts, o, &child_body);
    while ( ODD(sts)) {
      child->ref();
      getAllMenuItems(ip, Item, cdrep, child, child_body, Level, nItems, 0);
      wb_orep *prev = child;
      child = cdrep->menuAfter( &sts, child, &child_body);
      prev->unref();
    }
  }
}

pwr_tStatus wb_session::callMenuMethod( ldh_sMenuCall *mcp, int Index)
{
  pwr_tStatus sts = LDH__SUCCESS;
  wb_tMethodMenu method = (wb_tMethodMenu) mcp->ItemList[Index].Method;

  ldh_sEvent *ep = m_srep->eventStart( pwr_cNOid, ldh_eEvent_MenuMethodCalled);

  if (method != NULL)
    sts = (*method)(mcp);

  m_srep->eventSend( ep);

  return sts;
}

bool wb_session::validateDestination( wb_destination d, pwr_tCid cid)
{

  wb_object parent;
  wb_object o = object(d.oid());

  // Get parent
  if (o) {
    switch(d.code()) {
    case ldh_eDest_After:
    case ldh_eDest_Before:
      parent = o.parent();
      break;
    default:
      parent = o;
    }
  }
  if (!parent) {
    // No parent, check if valid top object (for vrepmem all objects are valid topobjects)
    wb_cdef c = cdef(cid);
    if (!c.flags().b.TopObject && type() != ldh_eVolRep_Mem) {
      m_sts = LDH__NOTOP;
      return false;
    }
  }
  else {
    // Check if adoption is legal
    if (parent.flags().b.NoAdopt) {
      m_sts = LDH__NOADOPT;
      return false;
    }
  }
  m_sts = LDH__SUCCESS;
  return true;
}

bool wb_session::castAttribute( pwr_sAttrRef *arp, pwr_tCid cid)
{
  wb_attribute a = attribute(arp);
  if (!a) return a.sts();
    
  if ( !(a.flags() & PWR_MASK_CASTATTR)) {
    m_sts = 0;
    return false;
  }

  wb_cdef c = cdef( cid);
  //if ( c.size( pwr_eBix_rt) != a.size()) {
  //  m_sts = 0;
  //  return false;
  //}

  // pwr_tTid tid = a.tid();

  pwr_tCastId castid;
  if ( cid == a.originalTid())
    castid = pwr_cNClassId;
  else
    castid = cid;

  pwr_sAttrRef cast_aref = cdh_ArefToCastAref( arp);
  wb_attribute cast_a = attribute(&cast_aref);
  if (!cast_a) return cast_a.sts();

  try {
    writeAttribute( cast_a, &castid);
  }
  catch (wb_error& e) {
    m_sts = e.sts();
    return false;
  }  

  void *body = calloc( 1, c.size( pwr_eBix_rt));
  c.templateBody( &m_sts, pwr_eBix_rt, body, arp->Objid);

  try {
    writeAttribute( a, body);
  }
  catch (wb_error& e) {
    m_sts = e.sts();
    return false;
  }  

  return true;
}

bool wb_session::disableAttribute( pwr_sAttrRef *arp, pwr_tDisableAttr disable)
{
  wb_attribute a = attribute(arp);
  if (!a) return a.sts();
    
  if ( !(a.flags() & PWR_MASK_DISABLEATTR)) {
    m_sts = 0;
    return false;
  }

  pwr_sAttrRef dis_aref = cdh_ArefToDisableAref( arp);
  wb_attribute dis_a = attribute(&dis_aref);
  if (!dis_a) return dis_a.sts();

  try {
    writeAttribute( dis_a, &disable);
  }
  catch (wb_error& e) {
    m_sts = e.sts();
    return false;
  }  
  return true;
}

bool wb_session::commit()
{
  // Store time in volume object
  pwr_tOid oid = pwr_cNOid;
  pwr_tTime time;

  clock_gettime(CLOCK_REALTIME, &time);
  oid.vid = m_vrep->vid();

  wb_orep *orep = m_vrep->object( &m_sts, oid); 
  if ( oddSts()) {
    orep->ref();
    wb_attribute modtime(m_sts, orep, "SysBody", "Modified");
    if ( modtime.oddSts())
      writeAttribute( modtime, &time);
    orep->unref();
  }

  return m_srep->commit(&m_sts);
}





