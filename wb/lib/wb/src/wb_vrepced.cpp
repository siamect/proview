/* 
 * Proview   $Id: wb_vrepced.cpp,v 1.4 2008-10-31 12:51:32 claes Exp $
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

#include "pwr_version.h"
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
#include "cow_msgwindow.h"
#include "co_tree.h"
#include "co_dcli.h"
#include "co_time.h"

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

  if ( name.oddSts())
    strcpy( name_str, name.object());

  wb_name n( name_str);
  wb_orep *o = m_vrep->createObject( sts, cdef, d, n, soix);
  if ( ODD(*sts))
    setFlagsNewAttribute( sts, o);

  return o;
}


wb_orep *wb_vrepced::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, 
				wb_name &name, pwr_tOix oix)
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

  if ( strcmp( orep->name(), "Template") == 0) {
    *sts = LDH__CLASSMISPLACED;
    return 0;
  }

  classeditorCheck( code, dest, orep->cid(), &soix, name_str, sts, false);
  if ( EVEN( *sts)) return 0;

  if ( name.oddSts())
    strcpy( name_str, name.object());

  wb_name n( name_str);

  wb_orep *o = m_vrep->copyObject( sts, orep, d, n, soix);
  if ( ODD(*sts))
    setFlagsNewAttribute( sts, o);

  return o;
}

bool wb_vrepced::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d)
{
  wb_orep *dest;
  ldh_eDest code = d.code();

  dest = m_vrep->object( sts, d.oid());
  if ( !dest) {
    *sts = LDH__BADDEST;
    return 0;
  }

  if ( !classeditorCheckMove( orep, code, dest, sts)) {
    if ( EVEN( *sts))
      return 0;
  }

  return m_vrep->moveObject( sts, orep, d);
}

bool wb_vrepced::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
  // If attribute object, change body object version time
  switch ( orep->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Intern:
  case pwr_eClass_Output:
  case pwr_eClass_Buffer:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef: {
    wb_orep *p = orep->parent( sts);
    if ( p) {
      pwr_sObjBodyDef pbody;
      m_vrep->readBody( sts, p, pwr_eBix_sys, &pbody);
      m_vrep->writeBody( sts, p, pwr_eBix_sys, &pbody);      
    }
    break;
  }
  default: ;
  }

  return m_vrep->deleteObject( sts, orep);
}

bool wb_vrepced::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  // If attribute object, change body object version time
  switch ( orep->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Intern:
  case pwr_eClass_Output:
  case pwr_eClass_Buffer:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef: {
    wb_orep *p = orep->parent( sts);
    if ( p) {
      pwr_sObjBodyDef pbody;
      m_vrep->readBody( sts, p, pwr_eBix_sys, &pbody);
      m_vrep->writeBody( sts, p, pwr_eBix_sys, &pbody);      
    }
    break;
  }
  default: ;
  }

  return m_vrep->deleteFamily( sts, orep);
}

bool wb_vrepced::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name) 
{
  return m_vrep->renameObject( sts, orep, name);
}

bool wb_vrepced::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  pwr_tTid tid;
  bool new_typeref = false;

  //  Check if change of TypeRef in attribute object
  switch ( o->cid()) {
    case pwr_eClass_Param:
    case pwr_eClass_Intern:
      if ( bix == pwr_eBix_sys && offset == offsetof( pwr_sParam, TypeRef)) {
	m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParam, TypeRef),
			       sizeof(tid), &tid);
	if ( tid != *(pwr_tTid *)p && cdh_tidIsCid( *(pwr_tTid *)p))
	  new_typeref = true;
      }
      break;
  case pwr_eClass_Buffer:
    if ( bix == pwr_eBix_sys && offset == offsetof( pwr_sBuffer, Class)) {
      m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sBuffer, Class),
			     sizeof(tid), &tid);
      if ( tid != *(pwr_tTid *)p && cdh_tidIsCid( *(pwr_tTid *)p))
	new_typeref = true;
    }
    break;
  default: ;
  }


  bool bsts = m_vrep->writeAttribute( sts, o, bix, offset, size, p);
  if ( ODD(*sts) && new_typeref)
    setFlagsNewAttribute( sts, o);

  return bsts;
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

  for ( sTypeBuild *tp = (sTypeBuild *)tree_Minimum( sts, m_typebuild_th);
	tp != NULL;
	tp = (sTypeBuild *)tree_Successor( sts, m_typebuild_th, tp)) {
    if ( !tp->built) {
      wb_orep *to = m_vrep->object( sts, cdh_TypeIdToObjid( tp->tid));
      to->ref();
      if ( !buildType( sts, to))
	return false;      
      to->unref();
    }
  }

  for ( sClassBuild *cp = (sClassBuild *)tree_Minimum( sts, m_classbuild_th);
	cp != NULL;
	cp = (sClassBuild *)tree_Successor( sts, m_classbuild_th, cp)) {
    if ( !cp->built) {
      wb_orep *co = m_vrep->object( sts, cdh_ClassIdToObjid( cp->cid));
      co->ref();
      if ( !buildClass( sts, co))
	return false;      
      co->unref();
    }
  }

  if ( m_errorCount) {
    char msg[80];
    sprintf( msg, "ClassVolume syntax error, %d errors found", m_errorCount);
    MsgWindow::message( 'E', msg, msgw_ePop_Yes);
  }
  tree_DeleteTable(sts, m_typebuild_th);

  m_vrep->commit( sts);

  printStructFile( false);
  printStructFile( true);
  printPaletteFile();

  createSnapshot( 0, 0);

  // Replace new dbs-file in global merep
  wb_mvrep *mv = m_erep->merep()->volume( sts, m_vrep->vid());
  if ( ODD(*sts))
    m_erep->merep()->removeDbs( sts, mv);

  pwr_tFileName vname;
  strcpy( vname, "$pwrp_load/");
  strcat( vname, m_vrep->name());
  strcat( vname, ".dbs");
  cdh_ToLower( vname, vname);
  dcli_translate_filename( vname, vname);
	  
  try {
    wb_vrepdbs *mvs = new wb_vrepdbs( m_erep, vname);
    mvs->load();
    m_erep->merep()->addDbs( sts, (wb_mvrep *)mvs);
    MsgWindow::message( 'I', "Volume loaded from snapshot file", vname);
  }
  catch ( wb_error& e) {
    MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
    return false;
  }

  updateMeta();

  // Create template objects for new classes

  for ( sClassBuild *cp = (sClassBuild *)tree_Minimum( sts, m_classbuild_th);
	cp != NULL;
	cp = (sClassBuild *)tree_Successor( sts, m_classbuild_th, cp)) {
    cp->built = 0;
    cp->build_started = 0;
  }

  for ( sClassBuild *cp = (sClassBuild *)tree_Minimum( sts, m_classbuild_th);
	cp != NULL;
	cp = (sClassBuild *)tree_Successor( sts, m_classbuild_th, cp)) {
    wb_orep *co = m_vrep->object( sts, cdh_ClassIdToObjid( cp->cid));

    co->ref();

    if ( !buildTemplate( sts, co))
      return false;

    co->unref();
  }

  tree_DeleteTable(sts, m_classbuild_th);

  // Create new dbs with new templatevalues
  m_vrep->commit( sts);

  createSnapshot( 0, 0);

  // Replace new dbs-file in global merep
  mv = m_erep->merep()->volume( sts, m_vrep->vid());
  if ( ODD(*sts))
    m_erep->merep()->removeDbs( sts, mv);

  strcpy( vname, "$pwrp_load/");
  strcat( vname, m_vrep->name());
  strcat( vname, ".dbs");
  cdh_ToLower( vname, vname);
  dcli_translate_filename( vname, vname);
	  
  try {
    wb_vrepdbs *mvs = new wb_vrepdbs( m_erep, vname);
    mvs->load();
    m_erep->merep()->addDbs( sts, (wb_mvrep *)mvs);
    MsgWindow::message( 'I', "Volume loaded from snapshot file", vname);
  }
  catch ( wb_error& e) {
    MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
    return false;
  }

  updateMeta();

  *sts = LDH__SUCCESS;
  return true;
}

bool wb_vrepced::abort(pwr_tStatus *sts) 
{
  return m_vrep->abort( sts);
}

// Reset NewAttribute bit in Flags
bool wb_vrepced::resetFlagsNewAttribute( pwr_tStatus *sts, wb_orep *o, pwr_tUInt32 flags)
{
  flags &= ~PWR_MASK_NEWATTRIBUTE;

  switch ( o->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  case pwr_eClass_Intern:
  case pwr_eClass_Buffer:
    m_vrep->writeAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags), 
			    sizeof(flags), &flags);
    if ( EVEN(*sts)) return false;
    break;
  default: ;
  }
  return true;
}

bool wb_vrepced::setFlagsNewAttribute( pwr_tStatus *sts, wb_orep *o)
{
  pwr_tUInt32 flags;

  switch ( o->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Intern:
  case pwr_eClass_Buffer:
    m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags),
			   sizeof(flags), &flags);
    if ( EVEN(*sts)) return false;
    break;
  default:
    return true;
  }
  
  if ( flags & PWR_MASK_NEWATTRIBUTE)
    return true;

  flags |= PWR_MASK_NEWATTRIBUTE;

  switch ( o->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Intern:
  case pwr_eClass_Buffer:
    m_vrep->writeAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags),
			    sizeof(flags), &flags);
    if ( EVEN(*sts)) return false;
    break;
  default:
    return true;
  }
  return true;
}

bool wb_vrepced::setPgmName( pwr_tStatus *sts, wb_orep *o)
{
  pwr_tPgmName pgmname;

  switch ( o->cid()) {
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  case pwr_eClass_Intern:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef:
  case pwr_eClass_Buffer:
    m_vrep->readAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParInfo, PgmName),
			   sizeof(pgmname), pgmname);
    if ( EVEN(*sts)) return false;

    if ( strcmp( pgmname, "") == 0) {
      strcpy( pgmname, o->name());
      m_vrep->writeAttribute( sts, o, pwr_eBix_sys, offsetof( pwr_sParInfo, PgmName),
			      sizeof(pgmname), pgmname);
      if ( EVEN(*sts)) return false;
    }
    break;
  default:
    return true;
  }
  return true;
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

bool wb_vrepced::importPasteObject(pwr_tOid destination, ldh_eDest destcode,
				   bool keepoid, pwr_tOid oid, 
				   pwr_tCid cid, pwr_tOid poid,
				   pwr_tOid boid, const char *name, pwr_mClassDef flags,
				   size_t rbSize, size_t dbSize, void *rbody, void *dbody,
				   pwr_tOid woid, pwr_tOid *roid) 
{
  pwr_tStatus sts;
  pwr_tObjName namel;
  wb_orep *po = 0;
  static pwr_tOix last_template_oix;
  static pwr_tOix last_template_boix;

  if ( cdh_ObjidIsNull( poid) && cdh_ObjidIsNull( boid)) {
    // Root object
    wb_orep *dest = m_vrep->object( &sts, destination);
    if ( !dest)
      throw wb_error(LDH__BADDEST);

    dest->ref();
    switch ( destcode) {
    case ldh_eDest_After:
      po = dest->parent( &sts);
      break;
    case ldh_eDest_IntoFirst:
      po = dest;
      break;
    default:
      throw wb_error(LDH__NYI);
    }
    po->ref();
    dest->unref();
  }
  else if ( cdh_ObjidIsNull( poid)) {
	
    pwr_tOid boidl;
    boidl.oix = m_vrep->importTranslate( boid.oix);
    boidl.vid = m_vrep->vid();
    wb_orep *bo = m_vrep->object( &sts, boidl);
    if ( bo) {
      bo->ref();
      po = bo->parent( &sts);
      bo->unref();

      if ( po)
	po->ref();
    }
  }
  else {
    pwr_tOid poidl;

    poidl.oix = m_vrep->importTranslate( poid.oix);
    poidl.vid = m_vrep->vid();

    po = m_vrep->object( &sts, poidl);
    po->ref();

    if ( strcmp( name, "Template") == 0 && 
	 po && po->cid() == pwr_eClass_ClassDef &&
	 !cdh_ObjidIsNull( boid)) {
      // Unable to paste a template object correctly, remove it

      // Link any forward sibling to backward sibling
      last_template_oix = oid.oix;
      last_template_boix = boid.oix;
      //pwr_tOix boix = m_vrep->importTranslate( boid.oix);
      //m_vrep->importTranslationTableInsert( oid.oix, boix);
      return true;
    }
  }
  // Link forward sibling to remove template
  if ( last_template_oix && boid.oix == last_template_oix)
    boid.oix = last_template_boix;

  pwr_tOid woidl;
  woidl.oix = oid.oix;
  strcpy( namel, name);
  if ( !classeditorCheck( ldh_eDest_IntoLast, po, cid, &woidl.oix,
			  namel, &sts, true)) {
    throw wb_error(sts);
    return 0;
  }
  if ( woidl.oix)
    woidl.vid = m_vrep->vid();
  else
    woidl.vid = 0;
    
  if ( po)
    po->unref();

  return m_vrep->importPasteObject( destination, destcode, keepoid, oid, cid, poid, boid, 
				    name, flags, rbSize, dbSize, rbody, dbody, woidl, roid);
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
	fth->unref();
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
      case pwr_eClass_Object:
      case pwr_cClass_PlcTemplate:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	fth->unref();
	return false;
      }
      break;
    case pwr_eClass_ClassHier:
      switch ( cid) {
      case pwr_eClass_ClassDef:
	break;
      default:
	*sts = LDH__CLASSMISPLACED;
	fth->unref();
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
	fth->unref();
	return false;
      }
      break;
    case pwr_eClass_TypeDef:
      switch ( cid) {
      case pwr_eClass_Bit:
      case pwr_eClass_Value:
	break;
      default:
	fth->unref();
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
      fth->unref();
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
      fth->unref();
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
      if ( fth)
	fth->unref();
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
      if ( fth)
	fth->unref();
      *sts = LDH__CLASSMISPLACED;
      return false;
    }

    // Get next tix
    nextTix( sts, &tix);
    if ( EVEN(*sts)) return false;

    while ( 1) {
      pwr_tOid oid;
      wb_orep *o;

      *oix = cdh_tixToOix( 1, tix);
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
      if ( fth)
	fth->unref();
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
	if ( ODD(*sts)) {
	  *sts = LDH__CLASSMISPLACED;
	  if ( fth)
	    fth->unref();
	  check->ref();
	  check->unref();
	  return false;
	}
	strcpy( name, "RtBody");
      }
      else if ( !devbody_found) {
	*oix = cdh_cixToOix( cdh_oixToCix(fth->oid().oix), pwr_eBix_dev, 0);
	wb_orep *check = m_vrep->object( sts, *oix);
	if ( ODD(*sts)) {
	  *sts = LDH__CLASSMISPLACED;
	  if ( fth)
	    fth->unref();
	  check->ref();
	  check->unref();
	  return false;
	}
	strcpy( name, "DevBody");
      }
      else {
	if ( fth)
	  fth->unref();
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
      if ( ODD(*sts)) {
	*sts = LDH__CLASSMISPLACED;
	if ( fth)
	  fth->unref();
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
      if ( fth)
	fth->unref();
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
      if ( fth)
	fth->unref();
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
      if ( fth)
	fth->unref();
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

bool wb_vrepced::classeditorCheckMove( wb_orep *o, ldh_eDest dest_code, 
				       wb_orep *dest, pwr_tStatus *sts)
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
      switch ( o->cid()) {
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
	fth->unref();
	return false;
      }
      break;
    case pwr_eClass_ClassDef:
      switch ( o->cid()) {
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
	fth->unref();
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_ClassHier:
      switch ( o->cid()) {
      case pwr_eClass_ClassDef:
	break;
      default:
	fth->unref();
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_TypeHier:
      switch ( o->cid()) {
      case pwr_eClass_Type:
      case pwr_eClass_TypeDef:
	break;
      default:
	fth->unref();
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    case pwr_eClass_TypeDef:
      switch ( o->cid()) {
      case pwr_eClass_Bit:
      case pwr_eClass_Value:
	break;
      default:
	fth->unref();
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      break;
    default: ;
    }
  }

  switch ( o->cid()) {
  case pwr_eClass_ClassHier:
  case pwr_eClass_TypeHier: {
    // Top object
    if ( fth) {
      fth->unref();
      *sts = LDH__CLASSMISPLACED;
      return false;
    }
    break;
  }
  case pwr_eClass_ClassDef:
  case pwr_eClass_TypeDef:
  case pwr_eClass_ObjBodyDef:
  case pwr_eClass_Param:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  case pwr_eClass_Intern:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef:
  case pwr_eClass_Buffer:
  case pwr_cClass_PlcTemplate:
  case pwr_eClass_GraphPlcNode: {
    if ( fth) {
      wb_orep *ftho = o->parent( sts);
      if ( ftho)
	ftho->ref();
      if ( !ftho || ( ftho && cdh_ObjidIsNotEqual( ftho->oid(), fth->oid()))) {
	if ( ftho)
	  ftho->unref();
	*sts = LDH__CLASSMISPLACED;
	return false;
      }
      if ( ftho)
	ftho->unref();
    }
    else {
      *sts = LDH__CLASSMISPLACED;
      return false;
    }
    break;
  }
  default: ;
  }
  *sts = LDH__SUCCESS;
  return true;
}

void wb_vrepced::error( const char *msg, wb_orep *o)
{
  char text[400];

  sprintf( text, "%s, %s", msg, o->longName().c_str()); 
  MsgWindow::message( 'E', text, msgw_ePop_No, o->oid());
  m_errorCount++;
}

bool wb_vrepced::buildTemplate( pwr_tStatus *sts, wb_orep *co)
{
  pwr_tStatus asts;
  int modified = 0;

  pwr_tCid cocid = cdh_ClassObjidToId(co->oid());

  sClassBuild *cp = (sClassBuild *)tree_Find( sts, m_classbuild_th, &cocid);
  if ( !cp) return false;

  if ( cp->build_started) {
    error( "Circular class dependency detected", co);
    return false;
  }
  cp->build_started = 1;

  wb_cdrep *cd = m_vrep->merep()->cdrep( sts, cocid);
  if ( EVEN(*sts)) return false;
  cd->ref();

  bool new_template = false;
  wb_name ntemplate("Template");

  pwr_tOid toid;
  toid.oix = cdh_cixToOix( cdh_cidToCix(cd->cid()), pwr_eBix_template, 0);
  toid.vid = m_vid;
  wb_orep *to = m_vrep->object( sts, toid);
  if ( EVEN(*sts)) {
    // Create a new template object
    wb_cdef cdef(cd);
    wb_destination d( co->oid(), ldh_eDest_IntoLast);
    pwr_tOix woix = cdh_cixToOix( cdh_cidToCix(cocid), pwr_eBix_template, 0);

    to = m_vrep->createObject( sts, cdef, d, ntemplate, woix);
    if ( EVEN(*sts)) return false;
    
    new_template = true;
    modified++;
  }
  to->ref();

  void *tbody = calloc( 1, cd->size(pwr_eBix_rt));
  m_vrep->readBody( sts, to, pwr_eBix_rt, tbody);
  if ( EVEN(*sts)) return false;

  pwr_tOid boid;
  boid.vid = m_vid;
  boid.oix = cdh_cixToOix( cdh_oixToCix(co->oid().oix), pwr_eBix_rt, 0);

  wb_orep *bo = m_vrep->object( sts, boid);
  if ( EVEN(*sts)) {
    cp->built = 1;
    return true;
  }

  bo->ref();
  wb_orep *ao = bo->first( &asts);
  wb_orep *next_ao;
  while ( ODD(asts)) {
    pwr_tTid typeref;
    pwr_tUInt32 flags;
    pwr_tUInt32 elements;
    pwr_tUInt32 offset;
    pwr_tUInt32 size;
    pwr_tOid toid;

    ao->ref();
    switch ( ao->cid()) {
    case pwr_eClass_Param:
    case pwr_eClass_Input:
    case pwr_eClass_Output:
    case pwr_eClass_Intern: {

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, TypeRef), 
			     sizeof(typeref), &typeref);
      if ( EVEN(*sts)) return false;

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Flags), 
			     sizeof(flags), &flags);
      if ( EVEN(*sts)) return false;

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Elements), 
			     sizeof(elements), &elements);
      if ( EVEN(*sts)) return false;

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParam, Info.Offset), 
			     sizeof(offset), &offset);
      if ( EVEN(*sts)) return false;

      if ( !cdh_tidIsCid( typeref)) {
	// Type attribute
	if ( flags & PWR_MASK_NEWATTRIBUTE)
	  resetFlagsNewAttribute( sts, ao, flags);
	next_ao = ao->after( &asts);
	ao->unref();
	ao = next_ao;
	continue;
      }
      break;
    }
    case pwr_eClass_Buffer: {

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Class), 
			     sizeof(typeref), &typeref);
      if ( EVEN(*sts)) return false;

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Flags), 
			     sizeof(flags), &flags);
      if ( EVEN(*sts)) return false;

      m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sBuffer, Info.Elements), 
			     sizeof(elements), &elements);
      if ( EVEN(*sts)) return false;
      break;
    }
    default:
      next_ao = ao->after( &asts);
      ao->unref();
      ao = next_ao;
      continue;
    }
      
    if ( !(new_template || flags & PWR_MASK_NEWATTRIBUTE)) {
      next_ao = ao->after( &asts);
      ao->unref();
      ao = next_ao;
      continue;
    }

    if ( elements == 0)
      elements = 1;


    // Class attribute

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

      cot->ref();
      if ( !cpt->built) {
	if ( !buildTemplate( sts, cot))
	  return false;
      }
	      
      // Get size from RtBody
      pwr_tOid boid;
      boid.oix = cdh_cixToOix( cdh_cidToCix(typeref), pwr_eBix_rt, 0);
      boid.vid = m_vid;
      wb_orep *bot = m_vrep->object( sts, boid);
      if ( !bot) {
	error( "Error in TypeRef, no RtBody found", ao);
	break;
      }
      bot->ref();
      m_vrep->readAttribute( sts, bot, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, Size), 
			     sizeof(size), &size);
      if ( EVEN(*sts)) return false;
      bot->unref();
      cot->unref();

      // Get template
      toid.oix = cdh_cixToOix( cdh_cidToCix(typeref), pwr_eBix_template, 0);
      toid.vid = m_vid;
      wb_orep *tot = m_vrep->object( sts, toid);
      if ( EVEN(*sts)) return false;

      tot->ref();

      void *tot_body = calloc( 1, size);
      m_vrep->readBody( sts, tot, pwr_eBix_rt, tot_body);
      if ( EVEN(*sts)) return false;

      for ( int i = 0; i < (int)elements; i++) {
	memcpy( (char *)tbody + offset + i * size, tot_body, size);
      }
      modified++;

      tot->unref();
      free( tot_body);
    }
    else {
      wb_cdrep *cd_type = m_vrep->merep()->cdrep( sts, typeref);
      if ( EVEN(*sts)) {
	error( "Error in TypeRef", ao);
	break;
      }
      cd_type->ref();

      size = cd_type->size( pwr_eBix_rt);
      void *tot_body = calloc( 1, size);

      toid.oix = cdh_cixToOix( cdh_cidToCix(typeref), pwr_eBix_template, 0);
      toid.vid = m_vid;
      cd_type->templateBody( sts, pwr_eBix_rt, tot_body, to->oid());

      for ( int i = 0; i < (int)elements; i++) {
	memcpy( (char *)tbody + offset + i * size, tot_body, size);
      }
      modified++;

      cd_type->unref();
      free( tot_body);
    }
    
    wb_adrep *adrep = new wb_adrep( *ao);
    adrep->ref();
    pwr_tAttrRef aref = adrep->aref();
    aref.Objid = to->oid();

    if ( cdh_CidToVid( typeref) == m_vid)
      updateTemplateRef( adrep, (char *) tbody + offset, aref, toid);
    else
      updateTemplateRef( adrep, (char *) tbody + offset, aref, to->oid());
    adrep->unref();

    if ( flags & PWR_MASK_NEWATTRIBUTE)
      resetFlagsNewAttribute( sts, ao, flags);

    next_ao = ao->after( &asts);
    ao->unref();
    ao = next_ao;
  }
  
  if ( modified) {
    m_vrep->writeBody( sts, to, pwr_eBix_rt, tbody);
    if ( EVEN(*sts)) return false;
  }

  bo->unref();
  to->unref();
  cd->unref();
  free( tbody);

  cp->built = 1;
  return true;
}

void wb_vrepced::updateTemplateRef( wb_adrep *subattr, char *body, pwr_tAttrRef aref,
				    pwr_tOid toid)
{
  pwr_tStatus sts;
  pwr_tCid cid = subattr->subClass();
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, cid);
  if ( EVEN(sts)) throw wb_error(sts);
  wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
  if ( EVEN(sts)) throw wb_error(sts);

  cdrep->ref();
  bdrep->ref();
  int subattr_elements = subattr->isArray() ? subattr->nElement() : 1;

  for ( int i = 0; i < subattr_elements; i++) {
    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      adrep->ref();
      if ( adrep->isClass()) {
	pwr_tAttrRef aaref = aref;
	aaref.Offset += i * bdrep->size();
	updateTemplateRef( adrep, (char *)body + i * subattr->size() / subattr_elements + 
			   adrep->offset(), aaref, toid);
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( cdh_ObjidIsEqual( *oidp, toid))
	      *oidp = aref.Objid;
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( cdh_ObjidIsEqual( arp->Objid, toid)) {
	      arp->Objid = aref.Objid;
	      arp->Body = aref.Body;
	      arp->Offset += aref.Offset;
	    }
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
      prev->unref();
    }
  }
  bdrep->unref();
  cdrep->unref();
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

    tot->ref();
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

    tot->unref();
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
  int intern_cnt = 0;
  int input_cnt = 0;
  int output_cnt = 0;
  bool has_rtbody = false;
  bool has_objxref = false;
  bool has_objref = false;
  bool has_attrref = false;
  bool has_attrxref = false;
  pwr_tTime ctime = pwr_cNTime;
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

      bo->ref();
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

	  // Counters for inputs, interns and outputs
	  switch ( ao->cid()) {
	  case pwr_eClass_Input:
	    input_cnt++;
	    break;
	  case pwr_eClass_Output:
	    output_cnt++;
	    break;
	  case pwr_eClass_Intern:
	    intern_cnt++;
	    break;
	  default: ;
	  }
	  
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

	      cot->ref();
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
	      bot->ref();
	      m_vrep->readAttribute( sts, bot, pwr_eBix_sys, offsetof( pwr_sObjBodyDef, Size), 
				 sizeof(size), &size);
	      if ( EVEN(*sts)) return false;
	      bot->unref();
	      
	      pwr_mClassDef cdflags;
	      m_vrep->readAttribute( sts, cot, pwr_eBix_sys, offsetof( pwr_sClassDef, Flags), 
				 sizeof(cdflags), &cdflags);
	      if ( EVEN(*sts)) return false;

	      pwr_tTime cot_time = cot->ohTime();
	      if ( time_Acomp( &cot_time, &ctime) == 1)
		ctime = cot_time;

	      cot->unref();

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

	      pwr_tTime cd_time = cd->structModTime();
	      if ( time_Acomp( &cd_time, &ctime) == 1)
		ctime = cd_time;

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
	    offset = pwr_AlignLW( offset) + pwr_cAlignLW;
	  }
	  if ( cdh_tidIsCid( typeref) ||
	       flags & PWR_MASK_POINTER ||
	       typeref == pwr_eType_Time || 
	       typeref == pwr_eType_DeltaTime ||
	       typeref == pwr_eType_Int64 ||
	       typeref == pwr_eType_UInt64 ||
	       typeref == pwr_eType_Float64 ||
	       typeref == pwr_eType_CastId ||
	       typeref == pwr_eType_DisableAttr ||
	       strcmp( ao->name(), "TimerFlag") == 0)
	    offset = pwr_AlignLW( offset);

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

	  // Alignment for next attribute
	  if ( flags & PWR_MASK_POINTER && !(flags & PWR_MASK_PRIVATE))
	    offset += pwr_cAlignLW;
	  else
	    offset += size;

	  if ( cdh_tidIsCid( typeref) ||
	       flags & PWR_MASK_POINTER ||
	       typeref == pwr_eType_CastId ||
	       typeref == pwr_eType_DisableAttr)
	    offset = pwr_AlignLW( offset);
	  else
	    offset = pwr_AlignW( offset);

	  paramindex++;
	  break;
	}
	case pwr_eClass_AttrXRef:
	case pwr_eClass_ObjXRef: {
	  pwr_tTid typeref;
	  pwr_tInt32 type;
	  pwr_tUInt32 flags;
	  pwr_tUInt32 elements;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Flags), 
				 sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->readAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Elements), 
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
	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Flags),
				  sizeof(flags), &flags);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Elements),
				  sizeof(elements), &elements);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Size),
				  sizeof(size), &size);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Offset),
				  sizeof(offset), &offset);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, ParamIndex),
				  sizeof(paramindex), &paramindex);
	  if ( EVEN(*sts)) return false;

	  m_vrep->writeAttribute( sts, ao, pwr_eBix_sys, offsetof( pwr_sParInfo, Type),
				  sizeof(type), &type);
	  if ( EVEN(*sts)) return false;

	  offset += pwr_AlignW( size);
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

	  offset = pwr_AlignLW( offset);
	  
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

	  offset += pwr_AlignLW( size);
	  paramindex++;
	  break;
	}
	default: ;
	}

	setPgmName( sts, ao);
	
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

      pwr_tTime bo_time = bo->treeModTime();
      if ( time_Acomp( &bo_time, &ctime) == 1)
	ctime = bo_time;

      bo->unref();
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

  // Set number of Input, Outputs and Interns in GraphPlcNode object
  if ( m_vrep->vid() > cdh_cSystemClassVolMax) {
    bool gfound = false;
    wb_orep *next_ch;
    wb_orep *ch = co->first( &asts);
    while ( ODD(asts)) {
      ch->ref();

      switch(  ch->cid()) {
      case pwr_eClass_GraphPlcNode: {
	pwr_sGraphPlcNode gbody;
  
	m_vrep->readBody( sts, ch, pwr_eBix_rt, &gbody);
	if ( EVEN(*sts)) return false;

	gbody.parameters[0] = input_cnt;
	gbody.parameters[1] = intern_cnt;
	gbody.parameters[2] = output_cnt;

	if ( gbody.default_mask[0] == 0 && gbody.default_mask[1] == 0) {
	  unsigned int mask = 0;
	  unsigned int m = 0;
	  for ( int i = 0; i < input_cnt; i++) {
	    m = i ? 2 * m : 1;
	    mask += m;
	  }
	  gbody.default_mask[0] = mask;

	  mask = 0;
	  m = 0;
	  for ( int i = 0; i < output_cnt; i++) {
	    m = i ? 2 * m : 1;
	    mask += m;
	  }
	  gbody.default_mask[1] = mask;
	  m_vrep->writeBody( sts, ch, pwr_eBix_rt, &gbody);
	  if ( EVEN(*sts)) return false;
	}
	break;
      }
      default: ;	
      }
      if ( gfound) {
	ch->unref();
	break;
      }
      next_ch = ch->after( &asts);
      ch->unref();
      ch = next_ch;
    }
  }

  // Update version
  pwr_tTime ohtime = co->ohTime();
  if ( time_Acomp( &ctime, &ohtime) == 1)
    m_vrep->ohTime( sts, co, ctime);


  cp->built = 1;

  *sts = LDH__SUCCESS;
  return true;
}

void wb_vrepced::printPaletteFile()
{
  // Print new palette file
  pwr_tStatus psts, sts;
  int menu_found = 0;
  int allclasses_found = 0;
  int palette_found = 0;
  PalFileMenu *menu = PalFile::config_tree_build( 0, pal_cLocalPaletteFile, 
					     pal_eNameType_All, "", 0);
  PalFileMenu *mp, *mp2, *mp3, *mp4;
  wb_orep *cedch, *cedcd, *cedgn, *otmp;

  // Add menu "NavigatorPalette-AllClasses-'volumename' if not found
  for ( mp = menu; mp; mp = mp->next) {
    if ( mp->item_type == pal_eMenuType_Palette &&
	 cdh_NoCaseStrcmp( mp->title, "NavigatorPalette") == 0) {
      for ( mp2 = mp->child_list; mp2; mp2 = mp2->next) {
	if ( mp2->item_type == pal_eMenuType_Menu &&
	     cdh_NoCaseStrcmp( mp2->title, "AllClasses") == 0) {
	  for ( mp3 = mp2->child_list; mp3; mp3 = mp3->next) {
	    if ( mp3->item_type == pal_eMenuType_ClassVolume &&
		 cdh_NoCaseStrcmp( mp3->title, m_name) == 0) {
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
    mp3 = new PalFileMenu( m_name, pal_eMenuType_ClassVolume, mp2);
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
	     cdh_NoCaseStrcmp( mp2->title, m_name) == 0) {
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
    mp2 = new PalFileMenu( m_name, pal_eMenuType_Menu, mp);
    mp2->next = mp->child_list;
    mp->child_list = mp2;
  }


  for ( cedch = object( &sts); ODD(sts);) {
    cedch->ref();
    if ( cedch->cid() == pwr_eClass_ClassHier) {
      for ( cedcd = cedch->first( &sts); ODD(sts);) {
	cedcd->ref();
	for ( cedgn = cedcd->first( &sts); ODD(sts);) {
	  cedgn->ref();
	  if ( cedgn->cid() == pwr_eClass_GraphPlcNode) {
	    // Add to menu
	    mp4 = mp3;
	    mp3 = new PalFileMenu( (char *)cedcd->name(), pal_eMenuType_Class, mp2);
	    if ( !mp2->child_list)
	      mp2->child_list = mp3;
	    else
	      mp4->next = mp3;
	    break;
	  }
	  otmp = cedgn;
	  cedgn = cedgn->after( &sts);
	  otmp->unref();
	}
	otmp = cedcd;
	cedcd = cedcd->after( &sts);
	otmp->unref();
      }
      cedch->unref();
      break;
    }
    otmp = cedch;
    cedch = cedch->after( &sts);
    otmp->unref();
  }

  PalFile::config_tree_print( pal_cLocalPaletteFile, menu, &psts);
}

char *wb_vrepced::fill(  ofstream &fp, int len) 
{
  static char str[200];
  
  char *s = str;
  for ( int i = 0; i < len; i++) {
    *s = ' ';
    s++;
    if ( i >= (int)sizeof(str))
      break;      
  }
  *s = 0;
  return str;
}

char *wb_vrepced::typeRefToName( pwr_tStatus *sts, pwr_tTid typeref, bool hpp)
{
  static char str[100];
  pwr_tObjName pgmname;
  bool is_systemclass = false;
  bool is_class = false;

  strcpy( str, "");
  if ( cdh_TidToVid( typeref) == m_vid) {
    wb_orep *o_typeref = m_vrep->object( sts, cdh_TypeIdToObjid( typeref));
    if ( !o_typeref) 
      return str;

    o_typeref->ref();
    
    if ( cdh_tidIsCid( typeref)) {
      pwr_sObjBodyDef typeref_body;
      
      pwr_tOid boid;
      boid.oix = cdh_cixToOix( cdh_cidToCix(typeref),pwr_eBix_sys,0);
      boid.vid = m_vrep->vid();
      wb_orep *o_typeref_bd = m_vrep->object( sts, boid);
      if ( EVEN(*sts)) return str;
      
      m_vrep->readBody( sts, o_typeref_bd, pwr_eBix_sys, &typeref_body);
      if ( EVEN(*sts)) return str;
      
      if ( strcmp( typeref_body.StructName, "") != 0)
	strcpy( pgmname, typeref_body.StructName);
      else
	strcpy( pgmname, o_typeref->name());
      o_typeref->unref();
      
      is_class = true;
    }
    else {
      // TypeRef is type in this volume
      pwr_sTypeDef typeref_body;
      
      m_vrep->readBody( sts, o_typeref, pwr_eBix_sys, &typeref_body);
      if ( EVEN(*sts)) return str;
      
      if ( strcmp( typeref_body.PgmName, "") != 0)
	strcpy( pgmname, typeref_body.PgmName);
      else
	strcpy( pgmname, o_typeref->name());
      o_typeref->unref();      
    }
  }
  else {
    if ( cdh_tidIsCid( typeref)) {
      // TypeRef is cid in other volume
      wb_cdrep *cdrep = m_vrep->merep()->cdrep( sts, typeref);
      if ( EVEN(*sts)) return str;

      if ( cdh_CidToVid( cdrep->cid()) == 1)
	is_systemclass = true;
      
      wb_bdrep *bdrep = cdrep->bdrep( sts, pwr_eBix_sys);
      if ( EVEN(*sts)) return str;
      
      strcpy( pgmname, bdrep->structName());
      if ( strcmp( pgmname, "") == 0)
	strcpy( pgmname, cdrep->name());
      delete bdrep;
      delete cdrep;
      
      is_class = true;
    }
    else {
      // TypeRef is type is other volume
      wb_tdrep *tdrep = m_vrep->merep()->tdrep( sts, typeref);
      if ( EVEN(*sts)) return str;
      
      strcpy( pgmname, tdrep->pgmName());
      if ( strcmp( pgmname, "") == 0)
	strcpy( pgmname, tdrep->name());
      delete tdrep;
    }
  }
  *sts = LDH__SUCCESS;
  if ( hpp) {
    if ( is_class)
      sprintf( str, "pwr_Class_%s", pgmname);
    else
      sprintf( str, "pwr_t%s", pgmname);
  }
  else {
    if ( is_systemclass)
      sprintf( str, "pwr_s%s", pgmname);
    else if ( is_class)
      sprintf( str, "pwr_sClass_%s", pgmname);
    else
      sprintf( str, "pwr_t%s", pgmname);
      
  }
  return str;
}

void wb_vrepced::printStructFile( bool hpp)
{
  // Print new include file
  pwr_tStatus sts;
  wb_orep *o_root, *o_tdef, *o_cdef, *o_value, *o_bdef, *o_adef, *otmp;
  ofstream fp;
  pwr_tFileName filename;
  pwr_tFileName fname, incname;
  char *s;
  char alignstr[40];

  if ( hpp)
    sprintf( filename, "$pwrp_inc/pwr_%sclasses.hpp", m_vrep->name());
  else
    sprintf( filename, "$pwrp_inc/pwr_%sclasses.h", m_vrep->name());
  cdh_ToLower( filename, filename);
  dcli_translate_filename( fname, filename);

  fp.open( fname);
  if ( !fp) {
    error( "Unable to open structfile", 0);
    return;
  }

  // Get name for #ifndef
  if ( (s = strrchr( filename, '/')))
    strcpy( incname, s+1);
  else
    strcpy( incname, filename);

  for ( s = incname; *s; s++) {
    if ( *s == '.')
      *s = '_';
  }


  fp << 
    "/*      Proview " << pwrv_cPwrVersionStr << " " << filename << " */" << endl <<
    endl <<
    "/*      Generated by wb_ced. */" << endl <<
    "/*      Do not edit this file. */" << endl << 
    endl <<
    "#ifndef " << incname << endl <<
    "#define " << incname << endl << 
    endl <<
    "#ifndef pwr_class_h" << endl <<
    "#include \"pwr_class.h\"" << endl <<
    "#endif" << endl << endl << endl;

  // Print Types
  for ( o_root = object( &sts); ODD(sts);) {
    o_root->ref();
    if ( o_root->cid() == pwr_eClass_TypeHier) {
      for ( o_tdef = o_root->first( &sts); ODD(sts);) {
	o_tdef->ref();
	if ( o_tdef->cid() == pwr_eClass_TypeDef) {
	  // Print TypeDef
	  pwr_sTypeDef tdef_body;
	  pwr_tObjName typeref_pgmname, tdef_pgmname;

	  m_vrep->readBody( &sts, o_tdef, pwr_eBix_sys, &tdef_body);
	  if ( EVEN(sts)) throw wb_error(sts);

	  // Get TypeRef
	  strcpy( typeref_pgmname, typeRefToName( &sts, tdef_body.TypeRef, false));
	  if ( EVEN(sts)) {
	    error( "Error in TypeRef", o_tdef);
	    throw wb_error(sts);
	  }

	  if ( strcmp( tdef_body.PgmName, "") != 0)
	    strcpy( tdef_pgmname, tdef_body.PgmName);
	  else
	    strcpy( tdef_pgmname, o_tdef->name());

	  fp << "/*_* " << typeref_pgmname << " : " << tdef_pgmname << endl <<
	    "    @Aref " << tdef_pgmname << " " << tdef_pgmname << endl << 
	    "*/" << endl << endl;
	  fp << "typedef " << typeref_pgmname << " pwr_t" << tdef_pgmname;
	  if ( tdef_body.Elements > 1)
	    fp << "[" << tdef_body.Elements << "];" << endl << endl;
	  else
	    fp << ";" << endl << endl;

	  bool value_found = false;
	  bool bit_found = false;
	  for ( o_value = o_tdef->first( &sts); ODD(sts);) {
	    o_value->ref();
	    if ( o_value->cid() == pwr_eClass_Value) {
	      // Print value enum
	      if ( !value_found) {
		value_found = true;
		
		fp << "typedef enum {" << endl;
	      }
	      pwr_sValue value_body;

	      m_vrep->readBody( &sts, o_value, pwr_eBix_sys, &value_body);
	      
	      fp << "  pwr_e" << tdef_pgmname << "_" << value_body.PgmName << 
		fill( fp,30-strlen(tdef_pgmname)-strlen(value_body.PgmName)) << 
		" = " << value_body.Value << "," << endl;
	    }
	    else if ( o_value->cid() == pwr_eClass_Bit) {
	      // Print mask enum
	      if ( !bit_found) {
		bit_found = true;
		
		fp << "typedef enum {" << endl;
	      }
	      pwr_sBit bit_body;

	      m_vrep->readBody( &sts, o_value, pwr_eBix_sys, &bit_body);
	      
	      fp << "  pwr_m" << tdef_pgmname << "_" << bit_body.PgmName 
		 << " = " << bit_body.Value << "," << endl;
	    }
	    otmp = o_value;
	    o_value = o_value->after( &sts);
	    otmp->unref();
	  }
	  if ( value_found) {
	    fp << "} pwr_e" << tdef_pgmname << ";" << endl << endl << endl;
	  }
	  else if ( bit_found) {
	    fp << "} pwr_m" << tdef_pgmname << ";" << endl << endl << endl;
	  }
	}
	otmp = o_tdef;
	o_tdef = o_tdef->after( &sts);
	otmp->unref();
      }
      o_root->unref();
      break;
    }
    otmp = o_root;
    o_root = o_root->after( &sts);
    otmp->unref();
  }


  // Print classes
  for ( o_root = object( &sts); ODD(sts);) {
    o_root->ref();
    if ( o_root->cid() == pwr_eClass_ClassHier) {
      for ( o_cdef = o_root->first( &sts); ODD(sts);) {
	o_cdef->ref();

	if ( o_cdef->cid() == pwr_eClass_ClassDef) {
	  pwr_sClassDef cdef_body;
	  bool body_found = false;

	  m_vrep->readBody( &sts, o_cdef, pwr_eBix_sys, &cdef_body);
	  if ( EVEN(sts)) throw wb_error(sts);

	  for ( o_bdef = o_cdef->first( &sts); ODD(sts);) {
	    o_bdef->ref();

	    if ( o_bdef->cid() == pwr_eClass_ObjBodyDef) {
	      pwr_sObjBodyDef bdef_body;
	      pwr_tObjName pgmname;
	      pwr_eBix bix = cdh_oixToBix( o_bdef->oid().oix);
	      char structstype[3];
#if 0
	      int struct_filler_cnt = 0;
#endif

	      m_vrep->readBody( &sts, o_bdef, pwr_eBix_sys, &bdef_body);
	      if ( EVEN(sts)) throw wb_error(sts);

	      if ( strcmp( bdef_body.StructName, "") != 0)
		strcpy( pgmname, bdef_body.StructName);
	      else
		strcpy( pgmname, o_cdef->name());

	      if ( bix == pwr_eBix_dev)
		strcpy( structstype, "sd");
	      else
		strcpy( structstype, "s");
	      
	      if ( !body_found) {
		fp <<
		  "#ifndef pwr_cClass_" << pgmname << endl <<
		  "#define pwr_cClass_" << pgmname << " " << o_cdef->cid() << "UL" << endl << endl;
		body_found = true;
	      }

	      if ( bix ==  pwr_eBix_rt && cdef_body.Flags.b.DevOnly) {
		fp << 
		  "/*   Class : " << o_cdef->name() << endl;
		fp <<
		  "     Body :  " << o_bdef->name() << endl;
		fp << 
		  "     Body is virtual" << endl <<
		  "*/" << endl << endl;
		otmp = o_bdef;
		o_bdef = o_bdef->after( &sts);
		otmp->unref();
		continue;
	      }

	      bool attr_found = false;
	      int attr_next_alignlw = 0;
	      for ( o_adef = o_bdef->first( &sts); ODD(sts);) {
		o_adef->ref();

		if ( !attr_found) {
		  attr_found = true;
		  
		  fp << 
		    "/*_* Class : " << o_cdef->name() << endl;
		  fp <<
		    "     Body :  " << o_bdef->name() << endl;
		  fp << 
		    "     @Aref " << o_cdef->name() << " pwr_" << structstype << "Class_" << pgmname << endl <<
		    "*/" << endl << endl;

		  if ( hpp) {
		    if ( bix == pwr_eBix_dev)
		      fp << "class pwr_dClass_" << pgmname;
		    else
		      fp << "class pwr_Class_" << pgmname;
		  }
		  else
		    fp << "typedef struct {" << endl;
		}

		switch ( o_adef->cid()) {
		case pwr_eClass_Param:
		case pwr_eClass_Intern:
		case pwr_eClass_Input:
		case pwr_eClass_Output: {
		  pwr_sIntern adef_body;
		  pwr_tObjName attr_pgmname;
		  pwr_tObjName attr_typeref_pgmname;
		  char pointertype[2];


		  m_vrep->readBody( &sts, o_adef, pwr_eBix_sys, &adef_body);
		  if ( EVEN(sts)) throw wb_error(sts);
		  
		  if ( strcmp( adef_body.Info.PgmName, "") != 0)
		    strcpy( attr_pgmname, adef_body.Info.PgmName);
		  else
		    strcpy( attr_pgmname, o_adef->name());

		  // Get TypeRef
		  strcpy( attr_typeref_pgmname, typeRefToName( &sts, adef_body.TypeRef, hpp));
		  if ( EVEN(sts)) {
		    error( "Error in TypeRef", o_tdef);
		    throw wb_error(sts);
		  }

		  if ( attr_next_alignlw || 
		       adef_body.Info.Flags & PWR_MASK_POINTER ||
		       adef_body.Info.Flags & PWR_MASK_CLASS ||
		       o_adef->cid() == pwr_eClass_Input ||
		       o_adef->cid() == pwr_eClass_Buffer ||
		       adef_body.TypeRef == pwr_eType_Int64 ||
		       adef_body.TypeRef == pwr_eType_UInt64 ||
		       adef_body.TypeRef == pwr_eType_Float64 ||
		       adef_body.TypeRef == pwr_eType_Time ||
		       adef_body.TypeRef == pwr_eType_DeltaTime ||
		       adef_body.TypeRef == pwr_eType_CastId ||
		       adef_body.TypeRef == pwr_eType_DisableAttr ||
		       adef_body.TypeRef == pwr_eType_Int64 ||
		       strcmp( o_adef->name(), "TimerFlag") == 0)
		    strcpy( alignstr, " pwr_dAlignLW");
		  else
		    strcpy( alignstr, " pwr_dAlignW");
		  
		  if ( adef_body.Info.Flags & PWR_MASK_CLASS ||
		       adef_body.Info.Flags & PWR_MASK_POINTER ||
		       o_adef->cid() == pwr_eClass_Buffer ||
		       adef_body.TypeRef == pwr_eType_CastId ||
		       adef_body.TypeRef == pwr_eType_DisableAttr)
		    // Align next attribute on longword
		    attr_next_alignlw = 1;
		  else
		    attr_next_alignlw = 0;

		  
		  bool super_attr = false;
		  if ( hpp && adef_body.Info.ParamIndex == 0) {
		    if ( adef_body.Info.Flags & PWR_MASK_SUPERCLASS) {
		      fp << " : public " << attr_typeref_pgmname << " {" << endl <<
			" public:" << endl;
		      super_attr = true;
		    }
		    else
		      fp << " {" << endl <<
			" public:" << endl;
		  }

		  if ( adef_body.Info.Flags & PWR_MASK_POINTER)
		    strcpy( pointertype, "*");
		  else
		    strcpy( pointertype, "");

		  if ( !super_attr) {
		    if ( o_adef->cid() == pwr_eClass_Input)
		      fp << "  " << attr_typeref_pgmname << 
			fill( fp, 35-strlen(attr_typeref_pgmname)) << " " << pointertype << 
			"*" << attr_pgmname << "P" << alignstr << ";"  << endl;
		    fp << "  " << attr_typeref_pgmname << 
		      fill( fp, 35-strlen(attr_typeref_pgmname)) << " " << pointertype << 
		      attr_pgmname;

		    if ( adef_body.Info.Elements > 1)
		      fp << "[" << adef_body.Info.Elements << "]" << alignstr << ";" << endl;
		    else
		      fp << alignstr << ";" << endl;

#if 0
		    int filler;
		    if ( adef_body.Info.Size < 4) {
		      filler = 4 - ((adef_body.Info.Elements * adef_body.Info.Size) % 4);
		      if ( filler == 4)
			filler = 0;
		    }
		    else
		      filler = 0;
		    
		    if ( filler) {
		      fp <<
			"  " << "char" << fill( fp, 36 - strlen("char"));
		      if ( filler == 1) {
			fp << "filler_" << struct_filler_cnt << ";" << endl;
		      }
		      else {
			fp << "filler_" << struct_filler_cnt << "[" << filler << "];" << endl;
		      }
		      struct_filler_cnt++;
		    }
#endif
		  }

		  break;
		}
		case pwr_eClass_Buffer: {
		  pwr_sBuffer adef_body;
		  pwr_tObjName attr_pgmname;
		  pwr_tObjName attr_typeref_pgmname;

		  strcpy( alignstr, " pwr_dAlignLW");
		  attr_next_alignlw = 1;

		  m_vrep->readBody( &sts, o_adef, pwr_eBix_sys, &adef_body);
		  if ( EVEN(sts)) throw wb_error(sts);
		  
		  if ( strcmp( adef_body.Info.PgmName, "") != 0)
		    strcpy( attr_pgmname, adef_body.Info.PgmName);
		  else
		    strcpy( attr_pgmname, o_adef->name());

		  if ( hpp && adef_body.Info.ParamIndex == 0)
		    fp << " {" << endl <<
		      " public:" << endl;

		  // Get TypeRef
		  strcpy( attr_typeref_pgmname, typeRefToName( &sts, adef_body.Class, hpp));
		  if ( EVEN(sts)) {
		    error( "Error in TypeRef", o_tdef);
		    throw wb_error(sts);
		  }

		  fp << "  " << attr_typeref_pgmname << 
		    fill( fp, 35-strlen(attr_typeref_pgmname)) << " " << attr_pgmname;


		  if ( adef_body.Info.Elements > 1)
		    fp << "[" << adef_body.Info.Elements << "]" << alignstr << ";" << endl;
		  else
		    fp << alignstr << ";" << endl;

		  break;
		}
		case pwr_eClass_ObjXRef: {
		  pwr_sObjXRef adef_body;
		  pwr_tObjName attr_pgmname;

		  if ( attr_next_alignlw)
		    strcpy( alignstr, " pwr_dAlignLW");
		  else
		    strcpy( alignstr, " pwr_dAlignW");
		  attr_next_alignlw = 0;

		  m_vrep->readBody( &sts, o_adef, pwr_eBix_sys, &adef_body);
		  if ( EVEN(sts)) throw wb_error(sts);
		  
		  if ( hpp && adef_body.Info.ParamIndex == 0)
		    fp << " {" << endl <<
		      " public:" << endl;

		  if ( strcmp( adef_body.Info.PgmName, "") != 0)
		    strcpy( attr_pgmname, adef_body.Info.PgmName);
		  else
		    strcpy( attr_pgmname, o_adef->name());

		  fp << "  " << "pwr_tOid" << 
		    fill( fp, 35-strlen("pwr_tOid")) << " " << attr_pgmname;

		  if ( adef_body.Info.Elements > 1)
		    fp << "[" << adef_body.Info.Elements << "]" << alignstr << ";" << endl;
		  else
		    fp << alignstr << ";" << endl;

		  break;
		}
		case pwr_eClass_AttrXRef: {
		  pwr_sAttrXRef adef_body;
		  pwr_tObjName attr_pgmname;

		  if ( attr_next_alignlw)
		    strcpy( alignstr, " pwr_dAlignLW");
		  else
		    strcpy( alignstr, " pwr_dAlignW");
		  attr_next_alignlw = 0;


		  m_vrep->readBody( &sts, o_adef, pwr_eBix_sys, &adef_body);
		  if ( EVEN(sts)) throw wb_error(sts);
		  
		  if ( hpp && adef_body.Info.ParamIndex == 0)
		    fp << " {" << endl <<
		      " public:" << endl;

		  if ( strcmp( adef_body.Info.PgmName, "") != 0)
		    strcpy( attr_pgmname, adef_body.Info.PgmName);
		  else
		    strcpy( attr_pgmname, o_adef->name());

		  fp << "  " << "pwr_tAttrRef" << 
		    fill( fp, 35-strlen("pwr_tAttrRef")) << " " << attr_pgmname;

		  if ( adef_body.Info.Elements > 1)
		    fp << "[" << adef_body.Info.Elements << "]" << alignstr << ";" << endl;
		  else
		    fp << alignstr << ";" << endl;

		  break;
		}
		default:
		  ;
		}

		otmp = o_adef;
		o_adef = o_adef->after( &sts);
		otmp->unref();

	      }
	      
	      if ( attr_found) {
		if ( hpp)
		  fp << "};" << endl << endl;
		else
		  fp << "} pwr_" << structstype << "Class_" << pgmname <<  ";" << endl << endl;
	      }
	    }

	    otmp = o_bdef;
	    o_bdef = o_bdef->after( &sts);
	    otmp->unref();
	  }
	  if ( body_found)
	    fp << "#endif" << endl << endl << endl;

	}

	otmp = o_cdef;
	o_cdef = o_cdef->after( &sts);
	otmp->unref();
      }
    }
    otmp = o_root;
    o_root = o_root->after( &sts);
    otmp->unref();
  }

  fp << "#endif" << endl;
  fp.close();
}

