#include "wb_session.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh.h"

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
  m_srep = new wb_srep(v);
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

wb_object wb_session::createObject(wb_cdef cdef, wb_destination d, wb_name name)
{
  wb_orep *orep = 0;
    
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  orep = m_vrep->createObject(&m_sts, cdef, d, name);
  return wb_object(m_sts, orep);
}

wb_object wb_session::copyObject(wb_object o, wb_destination d, wb_name name)
{
  wb_orep *orep = 0;
    
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  orep = m_vrep->copyObject(&m_sts, (wb_orep*)o, d, name);
  return wb_object(m_sts, orep);
}

bool wb_session::copyOset(wb_oset *o, wb_destination d)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  return m_vrep->copyOset(&m_sts, o, d);
}

bool wb_session::moveObject(wb_object o, wb_destination d)
{   
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  return m_vrep->moveObject(&m_sts, (wb_orep*)o, d);
}


bool wb_session::renameObject(wb_object o, wb_name name)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  return m_vrep->renameObject(&m_sts, (wb_orep*)o, name);
}

bool wb_session::deleteObject(wb_object o)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  return m_vrep->deleteObject(&m_sts, (wb_orep*)o);
    
/*
  if (!o.isLocal())
  return LDH__OTHERVOLUME;
    
  if (o.hasChild())
  return LDH__HAS_CHILD;

  o.triggAnteUnadopt();
  o.unaAdopt();
  o.triggPostUnadopt();
*/
}

bool wb_session::deleteFamily(wb_object o)
{
  if (isReadonly())
    throw wb_error_str("ReadOnlySession");

  return m_vrep->deleteFamily(&m_sts, (wb_orep*)o);
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
  if ( ODD(sts)) {
    o_menu->ref();
    getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
    o_menu->unref();
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
    if ( ODD(sts)) {
      o_menu->ref();
      getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
      o_menu->unref();
    }
    delete cdrep;
    o->unref();
  }

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
      if ( ODD(sts)) {
        o_menu->ref();
        getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
        o_menu->unref();
      }
      delete cdrep;
      o->unref();
    }

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
      if ( ODD(sts)) {
        o_menu->ref();
        getAllMenuItems( ip, &Item, cdrep, o_menu, o_menu_body, 0, &nItems, 0);
        o_menu->unref();
      }
      delete cdrep;
      o->unref();
    }

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
    if ( ODD(sts)) {
      child->ref();
      getAllMenuItems(ip, Item, cdrep, child, child_body, Level, nItems, 0);
      child->unref();
    }

    void *next_body;
    wb_orep *next = cdrep->menuAfter( &sts, o, &next_body);
    if ( ODD(sts)) {
      next->ref();
      getAllMenuItems(ip, Item, cdrep, next, next_body, Level, nItems, 0);
      next->unref();
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

