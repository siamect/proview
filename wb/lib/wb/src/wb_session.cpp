#include "wb_session.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh.h"
#include "wb_vrepmem.h"
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

  m_sts = triggAnteCreate( parent, cdef.cid());
  if ( evenSts()) return wb_object();
  m_sts = triggAnteAdopt( parent, cdef.cid());
  if ( evenSts()) return wb_object();

  orep = m_vrep->createObject(&m_sts, cdef, d, name);

  wb_object o = wb_object(m_sts, orep);
  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectCreated);
  m_srep->eventNewFamily( ep, o);
  triggPostCreate( o);
  triggPostAdopt( parent, o);
  m_srep->eventSend( ep);
  return o;
}

wb_object wb_session::copyObject(wb_object o, wb_destination d, wb_name name)
{
  wb_orep *orep = 0;
    
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

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

  m_sts = triggAnteCreate( parent, o.cid());
  if ( evenSts()) return wb_object();
  m_sts = triggAnteAdopt( parent, o.cid());
  if ( evenSts()) return wb_object();

  orep = m_vrep->copyObject(&m_sts, (wb_orep*)o, d, name);

  wb_object onew = wb_object(m_sts, orep);
  ldh_sEvent *ep = m_srep->eventStart( onew.oid(), ldh_eEvent_ObjectCreated);
  m_srep->eventNewFamily( ep, onew);
  triggPostCreate( onew);
  triggPostAdopt( parent, onew);
  m_srep->eventSend( ep);

  return onew;
}

bool wb_session::moveObject(wb_object o, wb_destination d)
{   
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

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

  wb_object old_parent = o.parent();
  m_sts = triggAnteMove( o, parent);
  if ( evenSts()) return false;
  m_sts = triggAnteUnadopt( old_parent, o);
  if ( evenSts()) return false;
  m_sts = triggAnteAdopt( parent, o.cid());
  if ( evenSts()) return false;

  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectMoved);
  m_srep->eventOldFamily( ep, o);

  bool rsts = m_vrep->moveObject(&m_sts, (wb_orep*)o, d);

  triggPostMove( o);
  triggPostUnadopt( old_parent, o);
  triggPostAdopt( parent, o);

  m_srep->eventNewFamily( ep, o);
  m_srep->eventSend( ep);

  return rsts;
}


bool wb_session::renameObject(wb_object o, wb_name name)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool sts = m_vrep->renameObject(&m_sts, (wb_orep*)o, name);

  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectRenamed);
  m_srep->eventSend( ep);
  return sts;
}

bool wb_session::deleteObject(wb_object o)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  if (!isLocal( o)) {
    m_sts = LDH__OTHERVOLUME;
    return false;
  }
  if ( o.first()) {
    m_sts = LDH__HAS_CHILD;
    return false;
  }

  pwr_tStatus sts;
  wb_object parent = o.parent();
  sts = triggAnteUnadopt( parent, o);

  ldh_sEvent *ep = m_srep->eventStart( o.oid(), ldh_eEvent_ObjectDeleted);
  m_srep->eventOldFamily( ep, o);

  sts = triggPostUnadopt( parent, o);

  bool rsts = m_vrep->deleteObject(&m_sts, (wb_orep*)o);
  m_srep->eventSend( ep);
  return rsts;
}

bool wb_session::deleteFamily(wb_object o)
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

  bool rsts = m_vrep->deleteFamily(&m_sts, (wb_orep*)o);
  m_srep->eventSend( ep);
  return rsts;
}

bool wb_session::writeAttribute(wb_attribute &a, void *p, size_t size)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool sts =  m_vrep->writeAttribute(&m_sts, (wb_orep*)a, a.bix(), a.offset(), a.size(), p);
  ldh_sEvent *ep = m_srep->eventStart( a.aoid(), ldh_eEvent_AttributeModified);
  m_srep->eventSend( ep);
  return sts;
}

bool wb_session::writeAttribute(wb_attribute &a, void *p)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  bool sts =  m_vrep->writeAttribute(&m_sts, (wb_orep*)a, a.bix(), a.offset(), a.size(), p);

  ldh_sEvent *ep = m_srep->eventStart( a.aoid(), ldh_eEvent_AttributeModified);
  m_srep->eventSend( ep);
  return sts;
}

bool wb_session::copyOset( pwr_sAttrRef *arp, bool keepref)
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

    m_vrep->exportTree( *mem, ap->Objid);  
    ap++;
  }
  return mem->importTree( keepref);  
}

bool wb_session::cutOset( pwr_sAttrRef *arp, bool keepref)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  m_sts = LDH__SUCCESS;

  copyOset( arp, keepref);
  if ( EVEN(m_sts)) return false;

  ldh_sEvent *ep = m_srep->eventStart( pwr_cNOid, ldh_eEvent_ObjectTreeDeleted);

  pwr_sAttrRef *ap = arp;
  while ( cdh_ObjidIsNotNull( ap->Objid)) {
    wb_object o = object( ap->Objid);
    if ( !o) {
      ap++;
      continue;
    }

    deleteFamily( o);
    ldh_sEvent *e = m_srep->eventStart( pwr_cNOid, ldh_eEvent_ObjectDeleted);
    m_srep->eventOldFamily( e, o);

    ap++;
  }
  m_srep->eventSend( ep);

  return true;
}

bool wb_session::pasteOset( pwr_tOid doid, ldh_eDest dest, 
				   bool keepoid, char *buffer)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  m_sts = LDH__SUCCESS;

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
    pwr_tStatus sts;
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
  pwr_tOid *olist;
  mem->exportPaste( *m_vrep, doid, dest, keepoid, &olist);

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
    if (cdh_ObjidIsEqual(ip->Pointed.Objid, ip->Selected[i].Objid)) {
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

  /* Find specific menues of pointed object */

  if (ip->PointedSet == ldh_eMenuSet_Class) {
    Class = cdh_ClassObjidToId(ip->Pointed.Objid);
  }
  else {
    o = m_vrep->erep()->object( &sts, ip->Pointed.Objid);
    if ( EVEN(sts)) return sts;
    o->ref();
    Class = o->cid();
    o->unref();
  }

  cdrep = m_vrep->merep()->cdrep( &sts, Class);
  if ( EVEN(sts)) return sts;

  cn = cdrep->longName();
  sprintf(Menu, "%s-%s-Pointed", cn.name(), MenuFolder);

  o = m_vrep->erep()->object( &sts, Menu);
  if ( ODD(sts)) {
    o->ref();

    Object = o->oid();

    o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
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

  switch (ip->SelectedSet) {
  case ldh_eMenuSet_Attribute:
  case ldh_eMenuSet_Class:
  case ldh_eMenuSet_Many:
  case ldh_eMenuSet_Object:
    /* Find generic menues for selected object(s) */
    sprintf(Menu, "pwrs:Class-$Object-%s-Selected", MenuFolder);
    cdrep = m_vrep->merep()->cdrep( &sts, pwr_eClass_Object);
    if ( EVEN(sts)) return sts;

    o = m_vrep->erep()->object( &sts, Menu);
    if ( ODD(sts)) {
      o->ref();

      Object = o->oid();

      o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
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
      o = m_vrep->erep()->object( &sts, ip->Pointed.Objid);
      if ( EVEN(sts)) return sts;
      o->ref();
      Class = o->cid();
      o->unref();
    }

    cdrep = m_vrep->merep()->cdrep( &sts, Class);
    if ( EVEN(sts)) return sts;

    cn = cdrep->longName();
    sprintf(Menu, "%s-%s-Selected", cn.name(), MenuFolder);

    o = m_vrep->erep()->object( &sts, Menu);
    if ( ODD(sts)) {
      o->ref();

      Object = o->oid();

      o_menu = cdrep->menuFirst( &sts, o, &o_menu_body);
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

    break;
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
  // ldh_sEvent		*ep;

  // ep = eventStart(sp, pwr_cNObjid, ldh_eEvent_MenuMethodCalled);

  if (method != NULL)
    sts = (*method)(mcp);

  // eventSend(sp, ep);

  return sts;
}








