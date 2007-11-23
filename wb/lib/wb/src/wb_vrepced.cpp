/* 
 * Proview   $Id: wb_vrepced.cpp,v 1.1 2007-11-23 14:25:09 claes Exp $
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

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "wb_vrepced.h"
#include "wb_orepced.h"
#include "wb_ldh_msg.h"
#include "wb_dbs.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_attribute.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_tdrep.h"
#include "wb_ldh_msg.h"
#include "wb_vrepwbl.h"
#include "wb_vrepref.h"
#include "wb_print_wbl.h"
#include "wb_volume.h"
#include "wb_palfile.h"
#include "pwr_baseclasses.h"
#include "co_msgwindow.h"
#include "co_tree.h"
#include "co_dcli.h"

typedef struct sTypeBuild 
{
  tree_sNode node;
  pwr_tTid   tid;
  int        built;
  int        build_started;
} sTypeBuild;

typedef struct sClassBuild 
{
  tree_sNode node;
  pwr_tCid   cid;
  int        built;
  int        build_started;
} sClassBuild;


wb_vrepced::~wb_vrepced()
{
  m_vrep->unref();
  m_erep->resetRefMerep();
}

void wb_vrepced::unref()
{
  if (--m_nRef == 0)
    delete this;
}

void wb_vrepced::name( const char *n) 
{ 
  strcpy( m_name, n);
  m_vrep->name( n);
}

wb_vrep *wb_vrepced::ref()
{
  m_nRef++;
  return this;
}


wb_orep *wb_vrepced::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name, pwr_tOix oix)
{
  wb_orep *dest;
  ldh_eDest code = d.code();
  pwr_tObjName name_str;
  pwr_tOix soix = 0;

  dest = m_vrep->object( sts, d.oid());
  if ( !dest) {
    *sts = LDH__BADDEST;
    return 0;
  }

  classeditorCheck( code, dest, cdef.cid(), &soix, name_str, sts, false);
  if ( EVEN( *sts)) return 0;

  wb_name n( name_str);
  return m_vrep->createObject( sts, cdef, d, n, soix);
}

wb_orep *wb_vrepced::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name)
{
  return m_vrep->copyObject( sts, orep, d, name);
}

bool wb_vrepced::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d)
{
  return m_vrep->moveObject( sts, orep, d);
}

bool wb_vrepced::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
  return m_vrep->deleteObject( sts, orep);
}

bool wb_vrepced::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  return m_vrep->deleteFamily( sts, orep);
}

bool wb_vrepced::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name) 
{
  return m_vrep->renameObject( sts, orep, name);
}

bool wb_vrepced::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  return m_vrep->writeAttribute( sts, o, bix, offset, size, p);
}

bool wb_vrepced::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
{
  return m_vrep->writeBody( sts, o, bix, p);
}

bool wb_vrepced::createSnapshot(const char *fileName, const pwr_tTime *time)
{
  return m_vrep->createSnapshot( fileName, time);
}

bool wb_vrepced::commit(pwr_tStatus *sts) 
{
  m_typebuild_th = tree_CreateTable(sts, sizeof(pwr_tTid), offsetof(sTypeBuild, tid), 
				     sizeof(sTypeBuild), 1000, tree_Comp_tid);
  m_classbuild_th = tree_CreateTable(sts, sizeof(pwr_tCid), offsetof(sClassBuild, cid), 
				     sizeof(sClassBuild), 1000, tree_Comp_cid);

  wb_orep *to = m_vrep->object( sts, pwr_eClass_TypeDef);
  while ( ODD(*sts)) {
    to->ref();
    pwr_tTid tid = cdh_TypeObjidToId( to->oid());
    tree_Insert( sts, m_typebuild_th, &tid);

    wb_orep *to_next = to->next( sts);
    to->unref();
    to = to_next;
  }

  wb_orep *co = m_vrep->object( sts, pwr_eClass_ClassDef);
  while ( ODD(*sts)) {
    co->ref();
    pwr_tCid cid = cdh_ClassObjidToId( co->oid());
    tree_Insert( sts, m_classbuild_th, &cid);

    wb_orep *co_next = co->next( sts);
    co->unref();
    co = co_next;
  }

  m_errorCount = 0;

  for (
    sTypeBuild *tp = (sTypeBuild *)tree_Minimum( sts, m_typebuild_th);
    tp != NULL;
    tp = (sTypeBuild *)tree_Successor( sts, m_typebuild_th, tp)
    ) {
    if ( !tp->built) {
      wb_orep *to = m_vrep->object( sts, cdh_TypeIdToObjid( tp->tid));
      if ( !buildType( sts, to))
	return false;      
    }
  }

  for (
    sClassBuild *cp = (sClassBuild *)tree_Minimum( sts, m_classbuild_th);
    cp != NULL;
    cp = (sClassBuild *)tree_Successor( sts, m_classbuild_th, cp)
    ) {
    if ( !cp->built) {
      wb_orep *co = m_vrep->object( sts, cdh_ClassIdToObjid( cp->cid));
      if ( !buildClass( sts, co))
	return false;      
    }
  }

  if ( m_errorCount) {
    char msg[80];
    sprintf( msg, "ClassVolume syntax error, %d errors found", m_errorCount);
    MsgWindow::message( 'E', msg, msgw_ePop_Yes);
  }
  tree_DeleteTable(sts, m_classbuild_th);

  m_vrep->commit( sts);
  *sts = LDH__SUCCESS;
  return true;
}

bool wb_vrepced::abort(pwr_tStatus *sts) 
{
  return m_vrep->abort( sts);
}

bool wb_vrepced::nextCix( pwr_tStatus *sts, pwr_tOix *cix)
{
  pwr_tOid oid;
  oid.vid = m_vid;
  oid.oix = 0;
  wb_orep *o = m_vrep->object( sts, oid);
  if ( EVEN(*sts)) return false;

  o->ref();

  m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sClassVolume, NextCix), 
			 sizeof(*cix), cix);
  if ( ODD(*sts)) {
    (*cix)++;
    m_vrep->writeAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sClassVolume, NextCix),
			    sizeof(*cix), cix);
  }
  o->unref();
  return ODD(*sts);
}

bool wb_vrepced::nextTix( pwr_tStatus *sts, pwr_tOix *tix)
{
  pwr_tOid oid;
  oid.vid = m_vid;
  oid.oix = 0;
  wb_orep *o = m_vrep->object( sts, oid);
  if ( EVEN(*sts)) return false;

  o->ref();

  m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sClassVolume, NextTix), 
			 sizeof(*tix), tix);
  if ( ODD(*sts)) {
    (*tix)++;
    m_vrep->writeAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sClassVolume, NextTix),
			    sizeof(*tix), tix);
  }
  o->unref();
  return ODD(*sts);
}

bool wb_vrepced::nextAix( pwr_tStatus *sts, wb_orep *co, pwr_tOix *aix)
{
  m_vrep->readAttribute( sts, co, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, NextAix), 
			 sizeof(*aix), aix);
  if ( ODD(*sts)) {
    (*aix)++;
    m_vrep->writeAttribute( sts, co, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, NextAix),
			    sizeof(*aix), aix);
  }
  return ODD(*sts);
}

bool wb_vrepced::classeditorCheck( ldh_eDest dest_code, wb_orep *dest, pwr_tCid cid,
				   pwr_tOix *oix, char *name, pwr_tStatus *sts, 
				   bool import_paste)
{
  wb_orep *fth;

  // Get father
  switch ( dest_code) {
  case ldh_eDest_After:
  case ldh_eDest_Before:
    fth = dest->parent( sts);
    break;
  case ldh_eDest_IntoFirst:
  case ldh_eDest_IntoLast:
    fth = dest;
    break;
  default: 
    return false;
  }
  if ( fth)
    fth->ref();

  if ( fth) {
    switch ( fth->cid()) {
    case pwr_eClass_ObjBodyDef:
      switch ( cid) {
      case pwr_eClass_Param:
      case pwr_eClass_Intern:
      case pwr_eClass_Input:
      case pwr_eClass_Output:
      case pwr_eClass_ObjXRef:
      case pwr_eClass_AttrXRef:
      case pwr_eClass_Buffer:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_ClassDef:
      switch ( cid) {
      case pwr_eClass_ObjBodyDef:
      case pwr_eClass_GraphPlcNode:
      case pwr_eClass_GraphPlcConnection:
      case pwr_eClass_GraphPlcWindow:
      case pwr_eClass_GraphPlcProgram:
      case pwr_eClass_RtMenu:
      case pwr_eClass_Menu:
      case pwr_eClass_RtMethod:
      case pwr_eClass_Method:
      case pwr_eClass_DbCallBack:
      case pwr_cClass_PlcTemplate:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_ClassHier:
      switch ( cid) {
      case pwr_eClass_ClassDef:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_TypeHier:
      switch ( cid) {
      case pwr_eClass_Type:
      case pwr_eClass_TypeDef:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_TypeDef:
      switch ( cid) {
      case pwr_eClass_Bit:
      case pwr_eClass_Value:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    default: ;
    }
  }

  switch ( cid) {
  case pwr_eClass_ClassHier: {
    // Top object, named Class
    if ( fth && fth->oid().oix != 0) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }
    if ( !import_paste) {
      strcpy( name, "Class");
    }
    *oix = 0;
    break;
  }
  case pwr_eClass_TypeHier: {
    // Top object, named Type
    if ( fth && fth->oid().oix != 0) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }
    if ( !import_paste) {
      strcpy( name, "Type");
    }
    *oix = 0;
    break;
  }
  case pwr_eClass_ClassDef: {
    // Child to ClassHier, oix from cix
    pwr_tOix cix;
    if ( !fth || fth->cid() != pwr_eClass_ClassHier) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    // Get next cix
    nextCix( sts, &cix);
    if ( EVEN(*sts)) return false;

    while ( 1) {
      pwr_tOid oid;
      wb_orep *o;

      *oix = cdh_cixToOix( cix, 0, 0);
      if ( EVEN(*sts)) return false;
      oid.oix = *oix;
      oid.vid = m_vid;
      if ( (o = m_vrep->object( sts, oid))) {
	// oix already exist, try next cix
	o->ref();
	o->unref();
	nextCix( sts, &cix);
	if ( EVEN(*sts)) return false;
      }
      else 
	break;
    }
    if ( !import_paste) 
      sprintf( name, "O%u", *oix);
    break;
  }

  case pwr_eClass_TypeDef: {
    // Child to TypeHier, oix from tix
    pwr_tOix tix;
    if ( !fth || fth->cid() != pwr_eClass_TypeHier) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    // Get next tix
    nextTix( sts, &tix);
    if ( EVEN(*sts)) return false;

    while ( 1) {
      pwr_tOid oid;
      wb_orep *o;

      *oix = cdh_tixToOix( tix, 0);
      if ( EVEN(*sts)) return false;
      oid.oix = *oix;
      oid.vid = m_vid;
      if ( (o = m_vrep->object( sts, oid))) {
	// oix already exist, try next tix
	o->ref();
	o->unref();
	nextTix( sts, &tix);
	if ( EVEN(*sts)) return false;
      }
      else 
	break;
    }
    if ( !import_paste) 
      sprintf( name, "O%u", *oix);
    break;
  }

  case pwr_eClass_ObjBodyDef: {
    // Child to ClassDef, oix from bix, named RtBody or DevBody
    if ( !fth || fth->cid() != pwr_eClass_ClassDef) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    if ( !import_paste) {
      bool rtbody_found = false;
      bool devbody_found = false;
      wb_orep *child = fth->first( sts); 
      while ( ODD(*sts)) {
	child->ref();
	if ( child->cid() == pwr_eClass_ObjBodyDef) {
	  if ( cdh_oixToBix( child->oid().oix) == pwr_eBix_rt)
	    rtbody_found = true;
	  else if ( cdh_oixToBix( child->oid().oix) == pwr_eBix_dev)
	    devbody_found = true;
	}
	wb_orep *next_child = child->after( sts);
	child->unref();
	child = next_child;
      }
      if ( !rtbody_found) {
	*oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), pwr_eBix_rt, 0);
	pwr_tOid woid;
	woid.oix = *oix;
	woid.vid = m_vid;
	wb_orep *check = m_vrep->object( sts, woid);
	if ( ODD(sts)) {
	  *sts = LDH__CLASSMISPLACED;
	  check->ref();
	  check->unref();
	  return false;
	}
	strcpy( name, "RtBody");
      }
      else if ( !devbody_found) {
	*oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), pwr_eBix_dev, 0);
	wb_orep *check = m_vrep->object( sts, *oix);
	if ( ODD(sts)) {
	  *sts = LDH__CLASSMISPLACED;
	  check->ref();
	  check->unref();
	  return false;
	}
	strcpy( name, "DevBody");
      }
      else {
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
    }
    else {
      // Use the name to choose oix
      if ( strcmp( name, "DevBody") == 0)
	*oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), pwr_eBix_dev, 0);
      else
	*oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), pwr_eBix_rt, 0);
      pwr_tOid woid;
      woid.oix = *oix;
      woid.vid = m_vid;
      wb_orep *check = m_vrep->object( sts, woid);
      if ( ODD(sts)) {
	*sts = LDH__CLASSMISPLACED;
	check->ref();
	check->unref();
	return false;
      }
    }
    break;
  }
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  case pwr_eClass_Intern:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef:
  case pwr_eClass_Buffer: {
    // Child to ObjBodyDef, oix from aix
    if ( !fth || fth->cid() != pwr_eClass_ObjBodyDef) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    pwr_tOix aix;
    nextAix( sts, fth, &aix);
    if ( EVEN(*sts)) return false;

    while( 1) {
      *oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), cdh_oixToBix(fth->oid().oix),
			   aix);
      pwr_tOid woid;
      woid.oix = *oix;
      woid.vid = m_vid;
      wb_orep *check = m_vrep->object( sts, woid);
      if ( EVEN(*sts))
	break;
      else {
	check->ref();
	check->unref();
	nextAix( sts, fth, &aix);
	if ( EVEN(*sts)) return false;
      }
    }
    if ( !import_paste)
      sprintf( name, "O%u", *oix);
    break;
  }

  case pwr_cClass_PlcTemplate: {
    // Child to ClassDef, named Code
    if ( !fth || fth->cid() != pwr_eClass_ClassDef) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    if ( !import_paste) {
      strcpy( name, "Code");
    }
    *oix = 0;
    break;
  }
  case pwr_eClass_GraphPlcNode: {
    // Child to ClassDef, named GraphPlcNode
    if ( !fth || fth->cid() != pwr_eClass_ClassDef) {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    if ( !import_paste) {
      strcpy( name, "GraphPlcNode");
    }
    *oix = 0;
    break;
  }
  default:
    *oix = 0;
  }

  if ( fth)
    fth->unref();
  *sts = LDH__SUCCESS;
  return true;
}

void wb_vrepced::error( char *msg, wb_orep *o)
{
  char text[400];

  sprintf( text, "%s, %s", msg, o->longName().c_str()); 
  MsgWindow::message( 'E', text, msgw_ePop_No, o->oid());
  m_errorCount++;
}

bool wb_vrepced::buildType( pwr_tStatus *sts, wb_orep *to)
{
  pwr_tTid typeref;
  pwr_tInt32 type;
  pwr_tUInt32 elements;
  pwr_tUInt32 size;
  pwr_tTid totid = cdh_TypeObjidToId( to->oid());

  sTypeBuild *tp = (sTypeBuild *)tree_Find( sts, m_typebuild_th, &totid);
  if ( !tp) return false;

  if ( tp->build_started) {
    error( "Circular type dependency detected", to);
    return false;
  }
  tp->build_started = 1;

  m_vrep->readAttribute( sts, to, pwr_eBix_sys, offsetof( pwr_sTypeDef, TypeRef), 
			 sizeof(typeref), &typeref);
  if ( EVEN(*sts)) return false;
  
  m_vrep->readAttribute( sts, to, pwr_eBix_sys, offsetof( pwr_sTypeDef, Elements), 
			 sizeof(elements), &elements);
  if ( EVEN(*sts)) return false;

  // Check if typeref in this volume
  if ( cdh_TidToVid( typeref) == m_vid) {
    sTypeBuild *tpt = (sTypeBuild *)tree_Find( sts, m_typebuild_th, &typeref);
    if ( !tpt) {
      error( "Error in TypeRef", to);
      return false;
    }
    wb_orep *tot = m_vrep->object( sts, cdh_TypeIdToObjid( typeref));
    if ( !tot)
      return false;

    if ( !tpt->built) {
      if ( !buildType( sts, tot))
	return false;
    }

    m_vrep->readAttribute( sts, tot, pwr_eBix_sys, offsetof( pwr_sTypeDef, Size), 
			   sizeof(size), &size);
    if ( EVEN(*sts)) return false;

    m_vrep->readAttribute( sts, tot, pwr_eBix_sys, offsetof( pwr_sTypeDef, Type), 
			   sizeof(type), &type);
    if ( EVEN(*sts)) return false;

    size = size * elements;
  }
  else {
    wb_tdrep *td = m_vrep->merep()->tdrep( sts, typeref);
    if ( EVEN(*sts)) {
      error( "Error in TypeRef", to);
      return false;
    }

    if ( elements < 1)
      elements = 1;
    size = td->size() * elements;
    type = td->type();
    delete td;
  }

  // Store data in typedef object
  m_vrep->writeAttribute( sts, to, pwr_eBix_sys, offsetof( pwr_sTypeDef, Size),
			  sizeof(size), &size);
  if ( EVEN(*sts)) return false;

  m_vrep->writeAttribute( sts, to, pwr_eBix_sys, offsetof( pwr_sTypeDef, Elements),
			  sizeof(elements), &elements);
  if ( EVEN(*sts)) return false;

  m_vrep->writeAttribute( sts, to, pwr_eBix_sys, offsetof( pwr_sTypeDef, Type),
			  sizeof(type), &type);
  if ( EVEN(*sts)) return false;
  
  tp->built = 1;
  return true;
}

bool wb_vrepced::buildClass( pwr_tStatus *sts, wb_orep *co)
{
  pwr_tOid oid;
  pwr_tStatus asts;
  pwr_tUInt32 numofobjbodies = 0;
  bool has_rtbody = false;
  bool has_objxref = false;
  bool has_objref = false;
  bool has_attrref = false;
  bool has_attrxref = false;
  pwr_tCid cocid = cdh_ClassObjidToId( co->oid());

  sClassBuild *cp = (sClassBuild *)tree_Find( sts, m_classbuild_th, &cocid);
  if ( !cp) return false;

  if ( cp->build_started) {
    error( "Circular class dependency detected", co);
    return false;
  }
  cp->build_started = 1;

  for ( int i = 0; i < 2; i++) {
    pwr_tUInt32 offset = 0;
    pwr_tUInt32 paramindex = 0;
    pwr_tUInt32 size;
    pwr_eBix bix = (i == 0) ? pwr_eBix_rt : pwr_eBix_dev;

    oid.vid = m_vid;
    oid.oix = cdh_cixToOix( cdh_oixToCix(co->oid().oix), bix, 0);

    wb_orep *bo = m_vrep->object( sts, oid);
    if ( bo) {
      if ( bix == pwr_eBix_rt)
	has_rtbody = true;
      numofobjbodies++;

      wb_orep *ao = bo->first( &asts);
      wb_orep *next_ao;
      while ( ODD(asts)) {

	ao->ref();
        switch ( ao->cid()) {
	case pwr_eClass_Param:
	case pwr_eClass_Input:
	case pwr_eClass_Output:
	case pwr_eClass_Intern: {
	  pwr_tTid typeref;
	  pwr_tInt32 type;
	  pwr_tUInt32 flags;
	  pwr_tUInt32 elements;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, TypeRef), 
				 sizeof(typeref), &typeref);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags), 
				 sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Elements), 
				 sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  if ( elements == 0)
	    elements = 1;

	  if ( elements > 1 && !( flags & PWR_MASK_ARRAY)) {
	    error( "Error in Elements or Flags.Array", ao);
	    break;
	  }
	       
	  if ( !cdh_tidIsCid( typeref)) {
	    // Type attribute

	    if ( flags & PWR_MASK_CLASS) {
	      error( "Error in Flags.Class", ao);
	      break;
	    }

	    if ( flags & PWR_MASK_SUPERCLASS) {
	      error( "Error in Flags.SuperClass", ao);
	      break;
	    }

	    if ( cdh_TidToVid( typeref) == m_vid) {
	      sTypeBuild *tpt = (sTypeBuild *)tree_Find( sts, m_typebuild_th, &typeref);
	      if ( !tpt) {
		error( "Error in TypeRef", ao);
		return false;
	      }
	      wb_orep *tot = m_vrep->object( sts, cdh_TypeIdToObjid( typeref));
	      if ( !tot)
		return false;

	      if ( !tpt->built) {
		if ( !buildType( sts, tot))
		  return false;
	      }

	      m_vrep->readAttribute( sts, tot, pwr_eBix_sys, offsetof( pwr_sTypeDef, Size), 
				     sizeof(size), &size);
	      if ( EVEN(*sts)) return false;
	      
	      m_vrep->readAttribute( sts, tot, pwr_eBix_sys, offsetof( pwr_sTypeDef, Type), 
				     sizeof(type), &type);
	      if ( EVEN(*sts)) return false;

	      size = size * elements;
	    }
	    else {
	      wb_tdrep *td = m_vrep->merep()->tdrep( sts, typeref);
	      if ( EVEN(*sts)) {
		error( "Error in TypeRef", ao);
		break;
	      }

	      type = td->type();
	      size = elements * td->size();
	      delete td;
	    }

	    if ( type == pwr_eType_AttrRef)
	      has_attrref = true;
	    else if ( type == pwr_eType_Objid)
	      has_objref = true;
	  }
	  else {
	    // Class attribute

	    if ( !(flags & PWR_MASK_CLASS))
	      flags |= PWR_MASK_CLASS;

	    if ( paramindex == 0 && strcmp( ao->name(), "Super") == 0 &&
		 !(flags & PWR_MASK_SUPERCLASS))
	      flags |= PWR_MASK_SUPERCLASS;

	    // Check if class in this volume
	    if ( cdh_CidToVid( typeref) == m_vid) {
	      sClassBuild *cpt = (sClassBuild *)tree_Find( sts, m_classbuild_th, &typeref);
	      if ( !cpt) {
		error( "Error in TypeRef", ao);
		break;
	      }
	      wb_orep *cot = m_vrep->object( sts, cdh_ClassIdToObjid( typeref));
	      if ( !cot)
		return false;

	      if ( !cpt->built) {
		if ( !buildClass( sts, cot))
		  return false;
	      }
	      type = typeref;
	      
	      // Get size from RtBody
	      pwr_tOid boid;
	      boid.oix = cdh_cixToOix( cdh_cidToCix(typeref), pwr_eBix_rt, 0);
	      boid.vid = m_vid;
	      wb_orep *bot = m_vrep->object( sts, boid);
	      if ( !bot) {
		error( "Error in TypeRef, no RtBody found", ao);
		break;
	      }

	      m_vrep->readAttribute( sts, bot, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, Size), 
				 sizeof(size), &size);
	      if ( EVEN(*sts)) return false;

	      pwr_mClassDef cdflags;
	      m_vrep->readAttribute( sts, cot, pwr_eBix_sys, offsetof( pwr_sClassDef, Flags), 
				 sizeof(cdflags), &cdflags);
	      if ( EVEN(*sts)) return false;

	      size = size * elements;

	      if ( cdflags.b.ObjRef)
		has_objref = true;
	      if ( cdflags.b.AttrRef)
		has_attrref = true;
	      if ( cdflags.b.ObjXRef)
		has_objxref = true;
	      if ( cdflags.b.AttrXRef)
		has_attrref = true;
	    }
	    else {
	      wb_cdrep *cd = m_vrep->merep()->cdrep( sts, typeref);
	      if ( EVEN(*sts)) {
		error( "Error in TypeRef", ao);
		break;
	      }

	      type = cd->cid();
	      size = elements * cd->size( pwr_eBix_rt);

	      if ( cd->flags().b.ObjRef)
		has_objref = true;
	      if ( cd->flags().b.AttrRef)
		has_attrref = true;
	      if ( cd->flags().b.ObjXRef)
		has_objxref = true;
	      if ( cd->flags().b.AttrXRef)
		has_attrref = true;
	      delete cd;
	    }
	  }
	  
	  if ( ao->cid() == pwr_eClass_Input) {
	    offset += sizeof(pwr_tFloat32*);
	  }

	  // Store data in Attribute object
	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags),
				  sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Elements),
				  sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Size),
				  sizeof(size), &size);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Offset),
				  sizeof(offset), &offset);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.ParamIndex),
				  sizeof(paramindex), &paramindex);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Type),
				  sizeof(type), &type);
	  if ( EVEN(*sts)) return false;

	  offset += size;
	  paramindex++;
	  break;
	}
	case pwr_eClass_AttrXRef:
	case pwr_eClass_ObjXRef: {
	  pwr_tTid typeref;
	  pwr_tInt32 type;
	  pwr_tUInt32 flags;
	  pwr_tUInt32 elements;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, TypeRef), 
				 sizeof(typeref), &typeref);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags), 
				 sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Elements), 
				 sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  if ( elements == 0)
	    elements = 1;

	  if ( elements > 1 && !( flags & PWR_MASK_ARRAY)) {
	    error( "Error in Elements or Flags.Array", ao);
	    break;
	  }
	       
	  if ( ao->cid() == pwr_eClass_ObjXRef)
	    typeref = pwr_eType_Objid;
	  else
	    typeref = pwr_eType_AttrRef;

	  wb_tdrep *td = m_vrep->merep()->tdrep( sts, typeref);
	  if ( EVEN(*sts)) {
	    error( "Error in TypeRef", ao);
	    break;
	  }

	  type = td->type();
	  size = elements * td->size();
	  delete td;

	  if ( ao->cid() == pwr_eClass_ObjXRef)
	    has_objxref = true;
	  else if ( ao->cid() == pwr_eClass_AttrXRef)
	    has_attrxref = true;

	  // Store data in Attribute object
	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags),
				  sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Elements),
				  sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Size),
				  sizeof(size), &size);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Offset),
				  sizeof(offset), &offset);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.ParamIndex),
				  sizeof(paramindex), &paramindex);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Type),
				  sizeof(type), &type);
	  if ( EVEN(*sts)) return false;

	  offset += size;
	  paramindex++;
	  break;
	}
	case pwr_eClass_Buffer: {
	  pwr_tCid cid;
	  pwr_tInt32 type;
	  pwr_tUInt32 flags;
	  pwr_tUInt32 elements;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Class), 
				 sizeof(cid), &cid);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Flags), 
				 sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Elements), 
				 sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  if ( elements == 0)
	    elements = 1;

	  if ( elements > 1 && !( flags & PWR_MASK_ARRAY)) {
	    error( "Error in Elements or Flags.Array", ao);
	    break;
	  }
	       
	  if ( !cdh_tidIsCid( cid)) {
	    error( "Error in Class", ao);
	    break;
	  }

	  wb_cdrep *cd = m_vrep->merep()->cdrep( sts, cid);
	  if ( EVEN(*sts)) {
	    error( "Error in Class", ao);
	    break;
	  }
	  
	  if ( !(flags & PWR_MASK_BUFFER))
	    flags |= PWR_MASK_BUFFER;

	  type = cd->cid();
	  size = elements * cd->size( pwr_eBix_rt);
	  delete cd;
	  
	  // Store data in Attribute object
	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Flags),
				  sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Elements),
				  sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Size),
				  sizeof(size), &size);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Offset),
				  sizeof(offset), &offset);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.ParamIndex),
				  sizeof(paramindex), &paramindex);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Type),
				  sizeof(type), &type);
	  if ( EVEN(*sts)) return false;

	  offset += size;
	  paramindex++;
	  break;
	}
	default: ;
	}
	
	next_ao = ao->after( &asts);
	ao->unref();
	ao = next_ao;
      }

      // Store data in Body object
      m_vrep->writeAttribute( sts, bo, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, Size),
			      sizeof(offset), &offset);
      if ( EVEN(*sts)) return false;
      
      m_vrep->writeAttribute( sts, bo, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, NumOfParams),
			      sizeof(paramindex), &paramindex);
      if ( EVEN(*sts)) return false;
    }
  }

  // Store data in ClassDef object
  m_vrep->writeAttribute( sts, co, pwr_eBix_sys, offsetof( pwr_sClassDef, NumOfObjBodies),
			  sizeof(numofobjbodies), &numofobjbodies);
  if ( EVEN(*sts)) return false;

  if ( has_rtbody || has_objref || has_attrref || has_objxref || has_attrxref) {
    pwr_mClassDef cflags;

    m_vrep->readAttribute( sts, co, pwr_eBix_sys, offsetof( pwr_sClassDef, Flags), 
			   sizeof(cflags), &cflags);
    if ( EVEN(*sts)) return false;

    if ( has_rtbody)
      cflags.b.RtBody = 1;
    if ( has_objref)
      cflags.b.ObjRef = 1;
    if ( has_attrref)
      cflags.b.AttrRef = 1;
    if ( has_objxref)
      cflags.b.ObjXRef = 1;
    if ( has_attrxref)
      cflags.b.AttrXRef = 1;
    m_vrep->writeAttribute( sts, co, pwr_eBix_sys, offsetof( pwr_sClassDef, Flags),
			  sizeof(cflags), &cflags);
    if ( EVEN(*sts)) return false;

  }

  cp->built = 1;

  *sts = LDH__SUCCESS;
  return true;
}

void wb_vrepced::printPaletteFile()
{
#if 0
  // Print new palette file
  pwr_tStatus psts;
  int menu_found = 0;
  int allclasses_found = 0;
  int palette_found = 0;
  PalFileMenu *menu = PalFile::config_tree_build( 0, pal_cLocalPaletteFile, 
					     pal_eNameType_All, "", 0);
  PalFileMenu *mp, *mp2, *mp3, *mp4;
  ced_object *cedch, *cedcd, *cedgn;

  // Add menu "NavigatorPalette-AllClasses-'volumename' if not found
  for ( mp = menu; mp; mp = mp->next) {
    if ( mp->item_type == pal_eMenuType_Palette &&
	 cdh_NoCaseStrcmp( mp->title, "NavigatorPalette") == 0) {
      for ( mp2 = mp->child_list; mp2; mp2 = mp2->next) {
	if ( mp2->item_type == pal_eMenuType_Menu &&
	     cdh_NoCaseStrcmp( mp2->title, "AllClasses") == 0) {
	  for ( mp3 = mp2->child_list; mp3; mp3 = mp3->next) {
	    if ( mp3->item_type == pal_eMenuType_ClassVolume &&
		 cdh_NoCaseStrcmp( mp3->title, volume_name) == 0) {
	      menu_found = 1;
	      break;
	    }
	  }
	  allclasses_found = 1;
	  break;
	}
      }
      palette_found = 1;
      break;
    }
  }

  if ( !palette_found) {
    // Create palette
    mp = new PalFileMenu( "NavigatorPalette", pal_eMenuType_Palette, 0);
    mp->next = menu;
    menu = mp;
  }
  if ( !allclasses_found) {
    // Create volume menu
    mp2 = new PalFileMenu( "AllClasses", pal_eMenuType_Menu, mp);
    mp2->next = mp->child_list;
    mp->child_list = mp2;
  }
  if ( !menu_found) {
    // Create volume menu
    mp3 = new PalFileMenu( volume_name, pal_eMenuType_ClassVolume, mp2);
    mp3->next = mp2->child_list;
    mp2->child_list = mp3;
  }


  // Replace menu "PlcEditorPalette-'volumename'-* with function object classes
  menu_found = 0;
  palette_found = 0;
  for ( mp = menu; mp; mp = mp->next) {
    if ( mp->item_type == pal_eMenuType_Palette &&
	 cdh_NoCaseStrcmp( mp->title, "PlcEditorPalette") == 0) {
      for ( mp2 = mp->child_list; mp2; mp2 = mp2->next) {
	if ( mp2->item_type == pal_eMenuType_Menu &&
	     cdh_NoCaseStrcmp( mp2->title, volume_name) == 0) {
	  // Remove
	  PalFile::config_tree_free( mp2->child_list);
	  mp2->child_list = 0;
	  menu_found = 1;
	  break;
	}
      }
      palette_found = 1;
      break;
    }
  }

  if ( !palette_found) {
    // Create palette
    mp = new PalFileMenu( "PlcEditorPalette", pal_eMenuType_Palette, 0);
    mp->next = menu;
    menu = mp;
  }
  if ( !menu_found) {
    // Create volume menu
    mp2 = new PalFileMenu( volume_name, pal_eMenuType_Menu, mp);
    mp2->next = mp->child_list;
    mp->child_list = mp2;
  }

  for ( cedch = root_object; cedch; cedch = cedch->fws) {
    if ( cedch->m_cid == pwr_eClass_ClassHier) {
      for ( cedcd = cedch->fch; cedcd; cedcd = cedcd->fws) {
	for ( cedgn = cedcd->fch; cedgn; cedgn = cedgn->fws) {
	  if ( cedgn->m_cid == pwr_eClass_GraphPlcNode) {
	    // Add to menu
	    mp4 = mp3;
	    mp3 = new PalFileMenu( cedcd->m_name, pal_eMenuType_Class, mp2);
	    if ( !mp2->child_list)
	      mp2->child_list = mp3;
	    else
	      mp4->next = mp3;
	    break;
	  }
	}
      }
      break;
    }
  }

  PalFile::config_tree_print( pal_cLocalPaletteFile, menu, &psts);
#endif
}

