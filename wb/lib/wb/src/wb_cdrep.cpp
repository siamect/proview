#include "co_cdh.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh_msg.h"
#include "co_dbs.h"
#include "wb_orepdbs.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"

static pwr_tString32 callBackName[] = {
  "__",         // ldh_eDbCallBack__
  "AnteCreate",	// ldh_eDbCallBack_AnteCreate
  "PostCreate",	// ldh_eDbCallBack_PostCreate
  "AnteDelete",	// ldh_eDbCallBack_AnteDelete
  "PostDelete",	// ldh_eDbCallBack_PostDelete
  "AnteMove",	// ldh_eDbCallBack_AnteMove
  "PostMove",	// ldh_eDbCallBack_PostMove
  "AnteRename",	// ldh_eDbCallBack_AnteRename
  "PostRename",	// ldh_eDbCallBack_PostRename
  "AnteAdopt",	// ldh_eDbCallBack_AnteAdopt
  "PostAdopt",	// ldh_eDbCallBack_PostAdopt
  "AnteUnadopt",// ldh_eDbCallBack_AnteUnadopt
  "PostUnadopt",// ldh_eDbCallBack_PostUnadopt
  "AnteUpdate",	// ldh_eDbCallBack_AnteUpdate
  "PostUpdate",	// ldh_eDbCallBack_PostUpdate
  "SyntaxCheck",// ldh_eDbCallBack_SyntaxCheck
  "-"           // ldh_eDbCallBack_
};

void wb_cdrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_cdrep *wb_cdrep::ref()
{
  m_nRef++;
  return this;
}

wb_cdrep::wb_cdrep() : m_nRef(0), m_orep(0), m_sts(LDH__NOCLASS)
{
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, pwr_tCid cid) : m_nRef(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( cid);
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, wb_name name) : m_nRef(0)
{
  char str[80];
  strcpy( str, "Class-");
  strcat( str, name.object());
  wb_name n = wb_name( str);
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, n);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, const wb_orep& o) : m_nRef(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( o.cid());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( const wb_orep& o) : m_nRef(0)
{
  pwr_tStatus sts;
  
  wb_cdrep *cdrep = o.vrep()->merep()->cdrep( &sts, o);
  m_sts = sts;
  m_orep = cdrep->m_orep;
  m_orep->ref();
  delete cdrep;
}

wb_cdrep::wb_cdrep( wb_adrep *adrep) : m_nRef(0)
{
  pwr_tStatus sts;
  wb_orepdbs *orep = (wb_orepdbs *) adrep->m_orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  orep->ref();
  m_orep = (wb_orepdbs *)orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  m_sts = LDH__SUCCESS;
  orep->unref();
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, const char *bname)
{
  wb_name n(bname);
  
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, n);
  if ( EVEN(*sts))
    return 0;
  wb_bdrep *bdrep = new wb_bdrep( *orep);
  return bdrep;
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, pwr_eBix bix)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->first( sts, m_orep);
  wb_orepdbs *old;
  while ( ODD(*sts)) {
    if ( orep->cid() == pwr_eClass_ObjBodyDef &&
         cdh_oixToBix( orep->oid().oix) ==  bix) {
      wb_bdrep *bdrep = new wb_bdrep( *orep);
      return bdrep;
    }
    old = orep;
    orep = (wb_orepdbs *)m_orep->m_vrep->after( sts, orep);

    // Delete
    old->ref();
    old->unref();
  }
  return 0;
}

wb_adrep *wb_cdrep::adrep( pwr_tStatus *sts, const char *aname)
{
  wb_name n = wb_name( aname);
  if ( n.evenSts()) {
    *sts = n.sts();
    return 0;
  }
  wb_orepdbs *orep_attr;
  wb_orepdbs *old;
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->first( sts);
  while ( ODD(*sts)) {
    orep->ref();
    if ( orep->cid() == pwr_eClass_ObjBodyDef) {
      orep_attr = (wb_orepdbs *)orep->m_vrep->child( sts, orep, n);
      if ( ODD(*sts)) {
        wb_adrep *adrep = new wb_adrep( *orep_attr);
        orep->unref();
        return adrep;
      }
    }
    old = orep;
    orep = (wb_orepdbs *)orep->after( sts);
    old->unref();
  }
  *sts = LDH__NOSUCHATTR;
  return 0;
}

wb_orep *wb_cdrep::classBody( pwr_tStatus *sts, const char *bname)
{
  wb_name n(bname);
  
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, n);
  if ( EVEN(*sts))
    return 0;
  return orep;
}

pwr_tCid wb_cdrep::cid()
{
  return cdh_ClassObjidToId( m_orep->oid());
}

void wb_cdrep::templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p)
{
  // Get objid for template object
  pwr_tOid oid;
  int cix = cdh_oixToCix( m_orep->oid().oix);  
  oid.vid = m_orep->oid().vid;
  oid.oix = cdh_cixToOix( cix, pwr_eBix_template, 0);

  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->object( sts, oid);
  if ( EVEN(*sts)) return;

  m_orep->m_vrep->readBody( sts, orep, bix, p);
  // Delete
  orep->ref();
  orep->unref();
}

pwr_mClassDef wb_cdrep::flags()
{
  pwr_sClassDef *classdef;
  pwr_tStatus sts;

  classdef = (pwr_sClassDef *) m_orep->m_vrep->readBody( &sts, m_orep, pwr_eBix_sys, 0);
  if ( EVEN(sts))
    throw wb_error( sts);

  return classdef->Flags;
}

const char *wb_cdrep::name() const
{
  return m_orep->name();
}

wb_name wb_cdrep::longName()
{
  return m_orep->longName();
}

void wb_cdrep::dbCallBack( pwr_tStatus *sts, ldh_eDbCallBack cb, char **methodName,
			   pwr_sDbCallBack **o)
{
  wb_name cb_name = wb_name( callBackName[cb]);
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, cb_name);
  if ( EVEN(*sts)) return;
  orep->ref();

  if ( orep->cid() != pwr_eClass_DbCallBack) {
    *sts = LDH__NOSUCHOBJ;
    orep->unref();
    return;
  }

  pwr_sDbCallBack *dbcallback;

  dbcallback = (pwr_sDbCallBack *) m_orep->m_vrep->readBody( sts, orep, pwr_eBix_sys, 0);
  if ( EVEN(*sts)) {
    orep->unref();
    return;
  }

  *methodName = dbcallback->MethodName;
  if ( o)
    *o = dbcallback;
}

// Get first menu object

wb_orep *wb_cdrep::menu( pwr_tStatus *sts, void **o)
{
  wb_orepdbs *prev;
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->first( sts, m_orep);
  while ( ODD(*sts)) {
    switch ( orep->cid()) {
    case pwr_eClass_Menu:
    case pwr_eClass_MenuButton:
    case pwr_eClass_MenuCascade:
    case pwr_eClass_MenuSeparator:
      break;
    default:
      ;
    }
    prev = orep;
    orep = (wb_orepdbs *)orep->after( sts);
    prev->ref();
    prev->unref();
  }
  if ( EVEN(*sts)) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  void *menubody;

  menubody = m_orep->m_vrep->readBody( sts, orep, pwr_eBix_sys, 0);
  if ( EVEN(*sts)) {
    orep->ref();
    orep->unref();
    return 0;
  }

  if ( o)
    *o = menubody;
  return orep;
}

// Get next menu sibling

wb_orep *wb_cdrep::menuAfter( pwr_tStatus *sts, wb_orep *orep, void **o)
{
  wb_orepdbs *prev;
  wb_orepdbs *after = (wb_orepdbs *)orep->after( sts);
  if ( EVEN(*sts)) return 0;

  while ( ODD(*sts)) {
    switch ( after->cid()) {
    case pwr_eClass_Menu:
    case pwr_eClass_MenuButton:
    case pwr_eClass_MenuCascade:
    case pwr_eClass_MenuSeparator:
      break;
    default:
      ;
    }
    prev = after;
    after = (wb_orepdbs *)orep->after( sts);
    prev->ref();
    prev->unref();
  }
  if ( EVEN(*sts)) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  void *menubody;

  menubody = m_orep->m_vrep->readBody( sts, after, pwr_eBix_sys, 0);
  if ( EVEN(*sts)) {
    after->ref();
    after->unref();
    return 0;
  }

  if ( o)
    *o = menubody;
  return after;
}

// Get first menu child of a menu object

wb_orep *wb_cdrep::menuFirst( pwr_tStatus *sts, wb_orep *orep, void **o)
{
  wb_orepdbs *first = (wb_orepdbs *)orep->first( sts);
  if ( EVEN(*sts)) return 0;

  switch ( first->cid()) {
  case pwr_eClass_Menu:
  case pwr_eClass_MenuButton:
  case pwr_eClass_MenuCascade:
  case pwr_eClass_MenuSeparator:
    break;
  default:
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  void *menubody;

  menubody = m_orep->m_vrep->readBody( sts, first, pwr_eBix_sys, 0);
  if ( EVEN(*sts)) {
    first->ref();
    first->unref();
    return 0;
  }

  if ( o)
    *o = menubody;
  return first;
}
