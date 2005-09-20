/* 
 * Proview   $Id: wb_cdrep.cpp,v 1.28 2005-09-20 13:14:28 claes Exp $
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

#include "co_cdh.h"
#include "co_time.h"
#include "wb_cdrep.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_ldh_msg.h"
#include "co_dbs.h"
#include "wb_orepdbs.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"
extern "C" {
#include "rt_conv.h"
}

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

wb_cdrep::~wb_cdrep()
{
  if ( m_orep)
    m_orep->unref();
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, pwr_tCid cid) : m_nRef(0), m_orep(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( cid);
  m_orep = mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, wb_name name) : m_nRef(0), m_orep(0)
{
  char str[80];
  strcpy( str, "Class-");
  strcat( str, name.object());
  wb_name n = wb_name( str);
  m_orep = mvrep->object( &m_sts, n);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, const wb_orep& o) : m_nRef(0), m_orep(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( o.cid());
  m_orep = mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( const wb_orep& o) : m_nRef(0), m_orep(0)
{
  pwr_tStatus sts;
  
  wb_cdrep *cdrep = o.vrep()->merep()->cdrep( &sts, o);
  m_sts = sts;
  if ( ODD(m_sts)) {
    m_orep = cdrep->m_orep;
    m_orep->ref();
    delete cdrep;
  }
}

wb_cdrep::wb_cdrep( wb_adrep *adrep) : m_nRef(0)
{
  pwr_tStatus sts;
  wb_orep *orep = adrep->m_orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  orep->ref();
  m_orep = orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  m_sts = LDH__SUCCESS;
  orep->unref();
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, const char *bname)
{
  wb_name n(bname);
  
  wb_orep *orep = m_orep->vrep()->child( sts, m_orep, n);
  if ( EVEN(*sts))
    return 0;
  wb_bdrep *bdrep = new wb_bdrep( *orep);
  return bdrep;
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, pwr_eBix bix)
{
  wb_orep *orep = m_orep->vrep()->first( sts, m_orep);
  wb_orep *old;
  while ( ODD(*sts)) {
    if ( orep->cid() == pwr_eClass_ObjBodyDef &&
         cdh_oixToBix( orep->oid().oix) ==  bix) {
      wb_bdrep *bdrep = new wb_bdrep( *orep);
      return bdrep;
    }
    old = orep;
    orep = m_orep->vrep()->after( sts, orep);

    // Delete
    old->ref();
    old->unref();
  }
  return 0;
}

wb_adrep *wb_cdrep::adrep( pwr_tStatus *sts, const char *aname)
{
  wb_attrname n = wb_attrname( aname);
  if ( n.evenSts()) {
    *sts = n.sts();
    return 0;
  }

  wb_bdrep *bd = bdrep( sts, pwr_eBix_rt);
  if ( ODD(*sts)) {
    wb_adrep *adrep = bd->adrep( sts, aname);
    delete bd;
    if ( ODD(*sts))
      return adrep;
  }
  bd = bdrep( sts, pwr_eBix_dev);
  if ( ODD(*sts)) {
    wb_adrep *adrep = bd->adrep( sts, aname);
    delete bd;
    if ( ODD(*sts))
      return adrep;
  }

#if 0
  wb_orep *orep_attr;
  wb_orep *old;
  wb_orep *orep = m_orep->first( sts);
  while ( ODD(*sts)) {
    orep->ref();
    if ( orep->cid() == pwr_eClass_ObjBodyDef) {
      wb_name objectname = wb_name( n.attribute());
      orep_attr = orep->vrep()->child( sts, orep, objectname);
      if ( ODD(*sts) /* && aname.attributeIsEqual( orep_attr->name()) */ ) {
        wb_adrep *adrep = new wb_adrep( *orep_attr);
        orep->unref();
        return adrep;
      }
    }
    old = orep;
    orep = orep->after( sts);
    old->unref();
  }
#endif
  *sts = LDH__NOSUCHATTR;
  return 0;
}

wb_orep *wb_cdrep::classBody( pwr_tStatus *sts, const char *bname)
{
  wb_name n(bname);
  
  wb_orep *orep = m_orep->vrep()->child( sts, m_orep, n);
  if ( EVEN(*sts))
    return 0;
  return orep;
}

pwr_tCid wb_cdrep::cid()
{
  return cdh_ClassObjidToId( m_orep->oid());
}

void wb_cdrep::templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p, pwr_tOid o)
{
  pwr_tStatus status;

  // Search for template in localWb
  wb_vrep *localwb = m_orep->vrep()->erep()->volume( &status, ldh_cWBVolLocal);
  if ( ODD(status)) {
    char name[120];
    sprintf( name, "Templates-%s", m_orep->name());
    wb_name n = wb_name(name);
    wb_orep *templ = localwb->object( &status, n);
    if ( ODD(status) && templ->cid() == cid()) {
      templ->ref();
      localwb->readBody( &status, templ, bix, p);
      templ->unref();
      if ( ODD(status)) {
	if ( cdh_ObjidIsNotNull(o))
	  updateTemplate( bix, p, o, templ->oid());
	*sts = LDH__SUCCESS;
	return;
      }
    }
  }

  // Get objid for template object
  pwr_tOid oid;
  int cix = cdh_oixToCix( m_orep->oid().oix);  
  oid.vid = m_orep->oid().vid;
  oid.oix = cdh_cixToOix( cix, pwr_eBix_template, 0);

  wb_orep *orep = m_orep->vrep()->object( sts, oid);
  if ( EVEN(*sts)) return;

  m_orep->vrep()->readBody( sts, orep, bix, p);
  if ( cdh_ObjidIsNotNull(oid))
    updateTemplate( bix, p, o, oid);

  // Delete
  orep->ref();
  orep->unref();
}

pwr_mClassDef wb_cdrep::flags()
{
  pwr_sClassDef *classdef;
  pwr_tStatus sts;

  classdef = (pwr_sClassDef *) m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, 0);
  if ( EVEN(sts))
    throw wb_error( sts);

  return classdef->Flags;
}

size_t wb_cdrep::size( pwr_eBix bix)
{
  size_t size;
  pwr_tStatus sts;

  wb_bdrep *bd = bdrep( &sts, bix);
  if ( EVEN(sts)) return 0;

  size = bd->size();
  delete bd;
  return size;
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
  wb_orep *orep = m_orep->vrep()->child( sts, m_orep, cb_name);
  if ( EVEN(*sts)) return;
  orep->ref();

  if ( orep->cid() != pwr_eClass_DbCallBack) {
    *sts = LDH__NOSUCHOBJ;
    orep->unref();
    return;
  }

  pwr_sDbCallBack *dbcallback;

  dbcallback = (pwr_sDbCallBack *) m_orep->vrep()->readBody( sts, orep, pwr_eBix_sys, 0);
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
  wb_orep *prev;
  wb_orep *orep = m_orep->vrep()->first( sts, m_orep);
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
    orep = orep->after( sts);
    prev->ref();
    prev->unref();
  }
  if ( EVEN(*sts)) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  void *menubody;

  menubody = m_orep->vrep()->readBody( sts, orep, pwr_eBix_sys, 0);
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
  wb_orep *prev;
  wb_orep *after = orep->after( sts);
  if ( EVEN(*sts)) return 0;

  bool found = false;
  while ( ODD(*sts)) {
    switch ( after->cid()) {
    case pwr_eClass_Menu:
    case pwr_eClass_MenuButton:
    case pwr_eClass_MenuCascade:
    case pwr_eClass_MenuSeparator:
      found = true;
      break;
    default:
      ;
    }
    if ( found)
      break;
    prev = after;
    after = after->after( sts);
    prev->ref();
    prev->unref();
  }
  if ( !found) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  void *menubody;

  menubody = m_orep->vrep()->readBody( sts, after, pwr_eBix_sys, 0);
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
  wb_orep *first = orep->first( sts);
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

  menubody = m_orep->vrep()->readBody( sts, first, pwr_eBix_sys, 0);
  if ( EVEN(*sts)) {
    first->ref();
    first->unref();
    return 0;
  }

  if ( o)
    *o = menubody;
  return first;
}

pwr_tTime wb_cdrep::ohTime()
{
  return m_orep->ohTime();
}

pwr_tTime wb_cdrep::modTime()
{
  return m_orep->treeModTime();
}

pwr_tTime wb_cdrep::structModTime()
{
  pwr_tStatus sts;
  pwr_tTime t = m_orep->modTime();

  wb_bdrep *rbd = bdrep( &sts, pwr_eBix_rt);
  if ( ODD(sts)) {
    pwr_tTime rbt = rbd->modTime();
    if ( time_Acomp( &rbt, &t) == 1)
      t = rbt;
    delete rbd;
  }
  wb_bdrep *dbd = bdrep( &sts, pwr_eBix_dev);
  if ( ODD(sts)) {
    pwr_tTime dbt = dbd->modTime();
    if ( time_Acomp( &dbt, &t) == 1)
      t = dbt;
  }
  return t;
}

wb_cdrep* wb_cdrep::super( pwr_tStatus *sts)
{
  wb_bdrep *bd = bdrep( sts, pwr_eBix_rt);
  if ( EVEN(*sts)) return 0;

  wb_adrep *adrep = bd->super(sts);
  if ( EVEN(*sts)) { delete bd; return 0;}

  wb_cdrep *cdrep = m_orep->vrep()->merep()->cdrep(sts, adrep->subClass());
  if ( EVEN(*sts)) { delete adrep; delete bd; return 0;}

  delete bd;
  delete adrep;
  return cdrep;
}

void wb_cdrep::convertSubClass( pwr_tCid cid, wb_merep *merep,
				void *body_source, void *body_target)
{
  pwr_tStatus sts;
  wb_cdrep *cdrep_source = merep->cdrep( &sts, cid);
  if ( EVEN(sts)) throw wb_error(sts);
  wb_cdrep *cdrep_target = m_orep->vrep()->merep()->cdrep( &sts, cid);
  if ( EVEN(sts)) return;

  pwr_eBix bix = pwr_eBix_rt;

  wb_bdrep *bdrep_target = cdrep_target->bdrep( &sts, bix);
  if ( EVEN(sts)) return;

  wb_bdrep *bdrep_source = cdrep_source->bdrep( &sts, bix);
  if ( EVEN(sts)) return;

  wb_adrep *adrep_source = bdrep_source->adrep( &sts);

  // Indentify attribute with the same aix
  while ( ODD(sts)) {
    bool found = false;
    wb_adrep *adrep_target = bdrep_target->adrep( &sts);
    while ( ODD(sts)) {
      if ( adrep_source->index() == adrep_target->index()) {
	found = true;
	break;
      }
      wb_adrep *prev = adrep_target;
      adrep_target = adrep_target->next( &sts);
      delete prev;
    }
    if ( found) {	      
      if ( adrep_source->isClass()) {
	if ( adrep_source->subClass() != adrep_target->subClass())
	  return;

	// Convert subclass
	convertSubClass( adrep_target->subClass(), merep, 
			(char *)body_source + adrep_source->offset(),
			(char *)body_target + adrep_target->offset());
      }
      else {
	// Convert attribute
	if ( adrep_source->type() == adrep_target->type()) {
	  memcpy( (char *)body_target + adrep_target->offset(), 
		  (char *)body_source + adrep_source->offset(),
		  min( adrep_target->size(), adrep_source->size()));
	}
	else {
	  int cidx = conv_GetIdx(adrep_source->type(), adrep_target->type());
	  if (cidx == conv_eIdx_invalid)
	    cidx = conv_eIdx_zero; /* Zero the attribute */

	  int size;
	  pwr_mAdef flags;
	  flags.m = adrep_target->flags();

	  if (!conv_Fctn[cidx](adrep_target->nElement(), 
			       adrep_target->size() / adrep_target->nElement(), 
			       (char *)body_target + adrep_target->offset(), 
			       &size,
			       adrep_source->nElement(), 
			       adrep_source->size() / adrep_source->nElement(), 
			       (char *)body_source + adrep_source->offset(),
			       flags))
	    throw wb_error(LDH__ATTRCONV);
	}
      }
      delete adrep_target;
    }
    wb_adrep *prev = adrep_source;
    adrep_source = adrep_source->next( &sts);
    delete prev;
  }
  delete bdrep_target;
  delete bdrep_source;
  delete cdrep_target;
  delete cdrep_source;
}

void wb_cdrep::convertObject( wb_merep *merep, void *rbody, void *dbody,
			      size_t *cnv_rbody_size, size_t *cnv_dbody_size,
			      void **cnv_rbody, void **cnv_dbody)
{
  pwr_tStatus sts;
  wb_cdrep *cdrep_source = merep->cdrep( &sts, cid());
  if ( EVEN(sts)) throw wb_error(sts);

  for ( int i = 0; i < 2; i++) {
    int size_target, size_source;
    void *body_target, *body_source;
    pwr_eBix bix = i ? pwr_eBix_dev: pwr_eBix_rt;

    wb_bdrep *bdrep_target = bdrep( &sts, bix);
    if ( EVEN(sts)) {
      size_target = 0;
      body_target = 0;
    }
    else {
      wb_bdrep *bdrep_source = cdrep_source->bdrep( &sts, bix);
      if ( EVEN(sts)) {
	// New body, return empty body
	size_target = bdrep_target->size();
	body_target = calloc( 1, size_target);
      }
      else {
	// Convert body
	size_source = bdrep_source->size();
	body_source = (bix == pwr_eBix_rt) ? rbody : dbody;
	size_target = bdrep_target->size();
	body_target = calloc( 1, size_target);

	// memcpy( body_target, body_source, size_target);
	wb_adrep *adrep_source = bdrep_source->adrep( &sts);

	// Indentify attribute with the same aix
	while ( ODD(sts)) {
	  bool found = false;
	  wb_adrep *adrep_target = bdrep_target->adrep( &sts);
	  while ( ODD(sts)) {
	    if ( adrep_source->aix() == adrep_target->aix()) {
	      found = true;
	      break;
	    }
	    wb_adrep *prev = adrep_target;
	    adrep_target = adrep_target->next( &sts);
	    delete prev;
	  }
	  if ( found) {	      
	    if ( adrep_source->isClass()) {
	      if ( adrep_source->subClass() != adrep_target->subClass()) {
		wb_adrep *prev = adrep_source;
		adrep_source = adrep_source->next( &sts);
		delete prev;
		continue;
	      }
	      // Convert subclass
	      convertSubClass( adrep_target->subClass(), merep, 
			(char *)body_source + adrep_source->offset(),
			(char *)body_target + adrep_target->offset());
	    }
	    else {
	      // Convert attribute
	      if ( adrep_source->type() == adrep_target->type()) {
		memcpy( (char *)body_target + adrep_target->offset(), 
			(char *)body_source + adrep_source->offset(),
			min( adrep_target->size(), adrep_source->size()));
	      }
	      else {
		int cidx = conv_GetIdx(adrep_source->type(), adrep_target->type());
		if (cidx == conv_eIdx_invalid)
		  cidx = conv_eIdx_zero; /* Zero the attribute */

		int size = adrep_target->size() / adrep_target->nElement();
		pwr_mAdef flags;
		flags.m = adrep_target->flags();

		if (!conv_Fctn[cidx](adrep_target->nElement(), 
				     adrep_target->size() / adrep_target->nElement(), 
				     (char *)body_target + adrep_target->offset(), 
				     &size,
				     adrep_source->nElement(), 
				     adrep_source->size() / adrep_source->nElement(), 
				     (char *)body_source + adrep_source->offset(),
				     flags))
		  throw wb_error(LDH__ATTRCONV);
	      }
	    }
	    delete adrep_target;
	  }
	  wb_adrep *prev = adrep_source;
	  adrep_source = adrep_source->next( &sts);
	  delete prev;
	}
	delete bdrep_source;
      }
      delete bdrep_target;
    }
    if ( bix == pwr_eBix_rt) {
      *cnv_rbody_size = size_target;
      *cnv_rbody = body_target;
    }
    else {
      *cnv_dbody_size = size_target;
      *cnv_dbody = body_target;
    }
  }
  delete cdrep_source;
}


void wb_cdrep::updateTemplateSubClass( wb_adrep *subattr, char *body, pwr_tOid oid,
				       pwr_tOid toid)
{
  pwr_tStatus sts;
  pwr_tCid cid = subattr->subClass();
  wb_cdrep *cdrep = m_orep->vrep()->merep()->cdrep( &sts, cid);
  if ( EVEN(sts)) throw wb_error(sts);
  wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
  if ( EVEN(sts)) throw wb_error(sts);

  int subattr_elements = subattr->isArray() ? subattr->nElement() : 1;

  for ( int i = 0; i < subattr_elements; i++) {
    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      if ( adrep->isClass()) {
	updateTemplateSubClass( adrep, body + i * subattr->size() / subattr_elements + adrep->offset(),
			oid, toid);
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( cdh_ObjidIsEqual( *oidp, toid))
	      *oidp = oid;
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( cdh_ObjidIsEqual( arp->Objid, toid))
	      arp->Objid = oid;
	    arp++;
	  }
	  break;
	}
	default:
	  ;
	}
      }
      wb_adrep *prev = adrep;
      adrep = adrep->next( &sts);
      delete prev;
    }
  }
  delete bdrep;
  delete cdrep;
}

void wb_cdrep::updateTemplate( pwr_eBix bix, void *b, pwr_tOid oid, pwr_tOid toid)
{
  pwr_tStatus sts;
  int i;
  char *body = (char *)b;

  wb_bdrep *bd = bdrep( &sts, bix);
  if ( EVEN(sts)) throw wb_error(sts);

  wb_adrep *adrep = bd->adrep( &sts);
  while ( ODD(sts)) {
    int elements = adrep->isArray() ? adrep->nElement() : 1;
    if ( adrep->isClass()) {
      updateTemplateSubClass( adrep, body + adrep->offset(), oid, toid);
    }
    else {
      switch ( adrep->type()) {
      case pwr_eType_Objid: {
	pwr_tOid *oidp = (pwr_tOid *)(body + adrep->offset());
	for ( i = 0; i < elements; i++) {
	  if ( cdh_ObjidIsEqual( *oidp, toid))
	    *oidp = oid;
	  oidp++;
	}
	break;
      }
      case pwr_eType_AttrRef: {
	pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + adrep->offset());
	for ( i = 0; i < elements; i++) {
	  if ( cdh_ObjidIsEqual( arp->Objid, toid))
	    arp->Objid = oid;
	  arp++;
	}
	break;
      }
      default:
	;
      }
    }
    wb_adrep *prev = adrep;
    adrep = adrep->next( &sts);
    delete prev;
  }
  delete bd;
}

ldh_eVolRep wb_cdrep::vtype() const 
{ 
  return m_orep->vtype();
}


