

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "antlr/AST.hpp"
#include "wb_wbllexer.hpp"
#include "wb_wblparser.hpp"
#include "wb_wblnode.h"

#include "wb_vrepwbl.h"
#include "wb_orepwbl.h"
#include "wb_ldh_msg.h"
#include "wb_dbs.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_tdrep.h"
#include "wb_ldh_msg.h"
#include "co_msgwindow.h"

extern "C" {
#include "co_dcli.h"
}

wb_vrepwbl::~wb_vrepwbl()
{
  for ( int i = 0; i < file_cnt; i++) {
    delete file[i]->parser;
    delete file[i]->lexer;
    delete file[i];
  }

  if ( m_merep != m_erep->merep())
    delete m_merep;
}

wb_orep *wb_vrepwbl::object(pwr_tStatus *sts, pwr_tOid oid)
{
  if ( oid.vid != m_vid) {
    *sts = LDH__BADOBJID;
    return 0;
  }
  
  ref_wblnode n = findObject( oid.oix);
  if ( !n) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return new wb_orepwbl( this, n);
}

wb_orep *wb_vrepwbl::object(pwr_tStatus *sts, wb_name &name)
{
  ref_wblnode n = find( name.name());
  if ( !n) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return new wb_orepwbl( this, n);
}

wb_vrep *wb_vrepwbl::next()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

void wb_vrepwbl::error( const char *msg, const char *file, int line_number)
{
  char str[400];
  sprintf( str, "Wbl error: %s, %s line: %d", msg, file, line_number);
  MsgWindow::message( 'E', str);
  // cout << "Wbl error: " << msg << ", " << file << " line: " << line_number << endl;
  error_cnt++;
}


void wb_vrepwbl::info()
{

  cout << "Volume : " << volume_name << " " << volume_class << " " << m_vid << endl;

  // for ( iterator_type_list it = m_type_list.begin(); 
  //	it != m_type_list.end(); it++)
  //  cout << "Type_list : " << it->first << " " << it->second->name << 
  //    "    line: " << it->second->line_number << endl;

  for ( iterator_tid_list it = m_tid_list.begin(); 
        it != m_tid_list.end(); it++)
    cout << "Tid_list : " << it->first << " " << it->second->name() << 
      "    size: " << it->second->o->ty.size << " elem: " << it->second->o->ty.elements <<  endl;

  for ( iterator_class_list it = m_class_list.begin(); 
        it != m_class_list.end(); it++)
    cout << "Class_list : " << it->first << " " << it->second->name() << 
      "    line: " << it->second->line_number << endl;

  for ( iterator_cid_list it = m_cid_list.begin(); 
        it != m_cid_list.end(); it++)
    cout << "Cid_list : " << it->first << " " << it->second->name() << 
      "    line: " << it->second->line_number << endl;

  if ( root_object)
    root_object->info(0);
}

bool
wb_vrepwbl::createSnapshot(const char *fileName)
{
  try {
    wb_dbs dbs(this);
        
    if ( fileName)
      dbs.setFileName( fileName);

    dbs.importVolume(*this);

    return true;
  } catch (wb_error& e) {
    return false;
  }
}

#if 0 
void wb_vrepwbl::iterObject( wb_dbs *dbs)
{
  if ( root_object)
    root_object->iterObject( dbs);
}

void wb_vrepwbl::iterDbody( wb_dbs *dbs)
{
  if ( root_object)
    root_object->iterDbody( dbs);
}

void wb_vrepwbl::iterRbody( wb_dbs *dbs)
{
  if ( root_object)
    root_object->iterRbody( dbs);
}
#endif
bool
wb_vrepwbl::exportVolume(wb_import &i)
{
  try {
    i.importVolume(*this);

    return true;
  } catch (wb_error& e) {
    return false;
  }
}

bool wb_vrepwbl::exportHead(wb_import &i)
{
  if ( root_object)
    return root_object->exportHead(i);
  else
    return false;
}

bool wb_vrepwbl::exportDbody(wb_import &i)
{
  if ( root_object)
    return root_object->exportDbody(i);
  else
    return false;
  
}

bool wb_vrepwbl::exportRbody(wb_import &i)
{
  if ( root_object)
    return root_object->exportRbody(i);
  else
    return false;
}

bool wb_vrepwbl::exportDocBlock(wb_import &i)
{
  if ( root_object)
    return root_object->exportDocBlock(i);
  else
    return false;
}

bool wb_vrepwbl::exportMeta(wb_import &i)
{
  return false;
}

bool wb_vrepwbl::exportTree(wb_treeimport &i, pwr_tOid oid)
{
  ref_wblnode n = findObject( oid.oix);
  if ( !n)
    throw wb_error(LDH__NOSUCHOBJ);

  n->exportTree( i, true);
  return true;
}

static int wbl_sort_files( const void *file1, const void *file2)
{
  return ( strcmp( (*(wb_wblfile **)file1)->file_name,
	(*(wb_wblfile **)(file2))->file_name));
}

int wb_vrepwbl::load( const char *fname)
{
  int i;
  char file_spec[200];
  pwr_tStatus sts, rsts;
  int file_cnt_sort = 0;

  rsts = LDH__SUCCESS;
  wblparser_error_cnt = 0;

  MsgWindow::dset_nodraw();

  if ( strstr( fname, ".wb_load") != 0 || strstr( fname, ".wb_dmp") != 0) {
    sts = load_files( fname);
    if ( EVEN(sts)) {
      MsgWindow::dreset_nodraw();
      return sts;
    }
  }
  else {
    // Load all wb_load files in directory

    // Load volume
    sprintf( file_spec, "%s/*_v.wb_load", fname);
    sts = load_files( file_spec);
    if ( EVEN(sts)) rsts = sts;

    // Load class hierarchies
    sprintf( file_spec, "%s/*_ch_*.wb_load", fname);
    sts = load_files( file_spec);
    if ( EVEN(sts)) rsts = sts;

    file_cnt_sort = file_cnt;

    // Load types
    sprintf( file_spec, "%s/*_t_*.wb_load", fname);
    sts = load_files( file_spec);
    if ( EVEN(sts)) rsts = sts;

    // Load types
    sprintf( file_spec, "%s/*_td_*.wb_load", fname);
    sts = load_files( file_spec);
    if ( EVEN(sts)) rsts = sts;

    // Load classes
    sprintf( file_spec, "%s/*_c_*.wb_load", fname);
    sts = load_files( file_spec);
    if ( EVEN(sts)) rsts = sts;

  }

  if ( file_cnt > file_cnt_sort)
    qsort( &file[file_cnt_sort], file_cnt - file_cnt_sort, sizeof(file[0]), 
	   wbl_sort_files);

  for ( i = 0; i < file_cnt; i++) {
    for ( ref_wblnode n = file[i]->rootAST; n; n = n->getNextSibling())
      n->registerNode( this);
  }
  for ( i = 0; i < file_cnt; i++) {
    file[i]->rootAST->link( this, 0);
  }
  // if ( root_object)
  //  root_object->info_link( 0);

  cout << "\n-- Building volume " << volume_name << endl;
  if ( root_object) {
    root_object->build( 1);
    root_object->postBuild();
  }
  // info();

  if ( !file_cnt) {
    MsgWindow::message( 'F', "No file found, ", fname);
    return LDH__NOSUCHFILE;
  }
  if ( strcmp( volume_name, "") == 0) {
    MsgWindow::message( 'F', "No volume found, ", fname);
    error_cnt++;
  }

  if ( error_cnt || wblparser_error_cnt) {
    char str[80];
    sprintf( str, "Errors when loading volume: %d errors found", error_cnt + wblparser_error_cnt);
    MsgWindow::message( 'F', str);
  }
  else
    MsgWindow::message( 'I', "Volume", volume_name, "loaded");

  MsgWindow::dreset_nodraw();

  // If classvolume, insert itself into its merep
  if ( cid() == pwr_eClass_ClassVolume ) {
    m_merep = new wb_merep( *m_erep->merep(), this);
    wb_mvrep *mvrep = m_merep->volume( &sts, vid());
    if ( ODD(sts))
      m_merep->removeDbs( &sts, mvrep);
    m_merep->addDbs( &sts, (wb_mvrep *)this);
    m_nRef--;
  }
  return rsts;
}

int wb_vrepwbl::classNameToCid( const char *name, pwr_tCid *cid)
{
  // cout << "classNameToCid : " << name << endl;

  string sname(name);
  iterator_class_list it = m_class_list.find( sname); 
  if ( it == m_class_list.end())
    return 0;
  *cid = it->second->Cid();
  return 1;
}

ref_wblnode wb_vrepwbl::findObject( pwr_tOix oix)
{
  iterator_oix_list it = m_oix_list.find( oix);
  if ( it == m_oix_list.end())
    return 0;
  return it->second;
}

ref_wblnode wb_vrepwbl::findClass( const char *name)
{
  string sname(name);
  iterator_class_list it = m_class_list.find( sname);
  if ( it == m_class_list.end())
    return 0;
  return it->second;
}

ref_wblnode wb_vrepwbl::findType( const char *name)
{
  wb_name wname = wb_name( name);
  if ( wname.evenSts()) return 0;

  if ( wname.hasVolume() && !wname.volumeIsEqual(volume_name))
    return 0;

  string sname(wname.object());
  iterator_type_list it = m_type_list.find( sname); 
  if ( it == m_type_list.end())
    return 0;
  if ( !it->second->o->is_built)
    it->second->build( 0);
  return it->second;
}

ref_wblnode wb_vrepwbl::findClass( pwr_tCid cid)
{
  iterator_cid_list it = m_cid_list.find( cid); 
  if ( it == m_cid_list.end())
    return 0; 
  return it->second;
}

ref_wblnode wb_vrepwbl::findType( pwr_tTid tid)
{
  iterator_tid_list it = m_tid_list.find( tid); 
  if ( it == m_tid_list.end())
    return 0; 
  return it->second;
}


int wb_vrepwbl::getTypeInfo( const char *name, pwr_tTid *tid, pwr_eType *type, size_t *size,
                             int *elements)
{
  bool type_extern = false;
  char tname[80];

  wb_name wname = wb_name(name);

  if ( wname.evenSts() || !wname.hasObject())
    return 0;
  strcpy( tname, wname.object());
  if ( wname.hasVolume() && !wname.volumeIsEqual(volume_name))
    type_extern = true;

  if ( !type_extern) {
    if ( strcmp( tname, "Boolean") == 0) 
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Boolean, type, size, elements);
    else if ( strcmp( tname, "Float32") == 0) 
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Float32, type, size, elements);
    else if ( strcmp( tname, "Float64") == 0) 
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Float64, type, size, elements);
    else if ( strcmp( tname, "Char") == 0) 
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Char, type, size, elements);
    else if ( strcmp( tname, "Int8") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Int8, type, size, elements);
    else if ( strcmp( tname, "Int16") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Int16, type, size, elements);
    else if ( strcmp( tname, "Int32") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Int32, type, size, elements);
    else if ( strcmp( tname, "UInt8") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_UInt8, type, size, elements);
    else if ( strcmp( tname, "UInt16") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_UInt16, type, size, elements);
    else if ( strcmp( tname, "UInt32") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_UInt32, type, size, elements);
    else if ( strcmp( tname, "Objid") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Objid, type, size, elements);
    else if ( strcmp( tname, "Enum") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Enum, type, size, elements);
    else if ( strcmp( tname, "Mask") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Mask, type, size, elements);
    else if ( strcmp( tname, "Time") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Time, type, size, elements);
    else if ( strcmp( tname, "AttrRef") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_AttrRef, type, size, elements);
    else if ( strcmp( tname, "Int64") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_Int64, type, size, elements);
    else if ( strcmp( tname, "UInt64") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_UInt64, type, size, elements);
    else if ( strcmp( tname, "ClassId") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_ClassId, type, size, elements);
    else if ( strcmp( tname, "TypeId") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_TypeId, type, size, elements);
    else if ( strcmp( tname, "ObjectIx") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_ObjectIx, type, size, elements);
    else if ( strcmp( tname, "RefId") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_RefId, type, size, elements);
    else if ( strcmp( tname, "DeltaTime") == 0)
      return getTypeInfo( *tid = (pwr_tTid)pwr_eType_DeltaTime, type, size, elements);
    else {
#if 0
      if ( wname.segmentIsEqual( "Class", 0)) {
	ref_wblnode n = findClass( tname);
	if ( n) {
	  size_t dsize;

	  *elements = 1;
	  *tid = n->o->c.cid;
	  return getClassInfo( *tid, size, &dsize);
	}
      }
      else 
#endif
      {
	ref_wblnode n = findType( tname);
	if ( n) {
	  *tid = n->o->m_tid;
	  *type = (pwr_eType) n->o->ty.type;
	  *size = n->o->ty.size;
	  *elements = 1;
	  return 1;
	}
      }
      type_extern = true;
    }
  }
  if ( type_extern) {
    // Fetch from other volume
    pwr_tStatus sts;

    wb_tdrep *tdrep = m_merep->tdrep( &sts, wname);
    if ( EVEN(sts)) return 0;

    *tid = tdrep->tid();
    *type = tdrep->type();
    *size = tdrep->size();
    *elements = tdrep->nElement();
    delete tdrep;
  }
  return 1;
}

int wb_vrepwbl::getTypeInfo( pwr_tTid tid, pwr_eType *type, size_t *size,
                             int *elements)
{
  switch ( tid) {
  case pwr_eType_Boolean:
    *type = pwr_eType_Boolean;
    *size = sizeof(pwr_tBoolean);
    *elements = 1;
    break;
  case pwr_eType_Float32:
    *type = pwr_eType_Float32;
    *size = sizeof(pwr_tFloat32);
    *elements = 1;
    break;
  case pwr_eType_Float64:
    *type = pwr_eType_Float64;
    *size = sizeof(pwr_tFloat64);
    *elements = 1;
    break;
  case pwr_eType_Char:
    *type = pwr_eType_Char;
    *size = sizeof(pwr_tChar);
    *elements = 1;
    break;
  case pwr_eType_Int8:
    *type = pwr_eType_Int8;
    *size = sizeof(pwr_tInt8);
    *elements = 1;
    break;
  case pwr_eType_Int16:
    *type = pwr_eType_Int16;
    *size = sizeof(pwr_tInt16);
    *elements = 1;
    break;
  case pwr_eType_Int32:
    *type = pwr_eType_Int32;
    *size = sizeof(pwr_tInt32);
    *elements = 1;
    break;
  case pwr_eType_UInt8:
    *type = pwr_eType_UInt8;
    *size = sizeof(pwr_tUInt8);
    *elements = 1;
    break;
  case pwr_eType_UInt16:
    *type = pwr_eType_UInt16;
    *size = sizeof(pwr_tUInt16);
    *elements = 1;
    break;
  case pwr_eType_UInt32:
    *type = pwr_eType_UInt32;
    *size = sizeof(pwr_tUInt32);
    *elements = 1;
    break;
  case pwr_eType_Objid:
    *type = pwr_eType_Objid;
    *size = sizeof(pwr_tObjid);
    *elements = 1;
    break;
  case pwr_eType_Enum:
    *type = pwr_eType_Enum;
    *size = sizeof(pwr_tEnum);
    *elements = 1;
    break;
  case pwr_eType_Mask:
    *type = pwr_eType_Mask;
    *size = sizeof(pwr_tMask);
    *elements = 1;
    break;
  case pwr_eType_Time:
    *type = pwr_eType_Time;
    *size = sizeof(pwr_tTime);
    *elements = 1;
    break;
  case pwr_eType_AttrRef:
    *type = pwr_eType_AttrRef;
    *size = sizeof(pwr_sAttrRef);
    *elements = 1;
    break;
  case pwr_eType_UInt64:
    *type = pwr_eType_UInt64;
    *size = sizeof(pwr_tUInt64);
    *elements = 1;
    break;
  case pwr_eType_Int64:
    *type = pwr_eType_Int64;
    *size = sizeof(pwr_tInt64);
    *elements = 1;
    break;
  case pwr_eType_ClassId:
    *type = pwr_eType_ClassId;
    *size = sizeof(pwr_tClassId);
    *elements = 1;
    break;
  case pwr_eType_TypeId:
    *type = pwr_eType_TypeId;
    *size = sizeof(pwr_tTypeId);
    *elements = 1;
    break;
  case pwr_eType_ObjectIx:
    *type = pwr_eType_ObjectIx;
    *size = sizeof(pwr_tObjectIx);
    *elements = 1;
    break;
  case pwr_eType_RefId:
    *type = pwr_eType_RefId;
    *size = sizeof(pwr_tRefId);
    *elements = 1;
    break;
  case pwr_eType_DeltaTime:
    *type = pwr_eType_DeltaTime;
    *size = sizeof(pwr_tDeltaTime);
    *elements = 1;
    break;
  default:
  {
    // Search type in this volume
    ref_wblnode n = findType( tid);
    if ( n) {
      if ( !n->o->is_built)
        n->build( 0);
      *type = (pwr_eType) n->o->ty.type;
      *size = n->o->ty.size;
      *elements = 1;
    }
    else {
      // Search type in other volumes
      pwr_tStatus sts;

      try {
	wb_tdrep *tdrep = m_merep->tdrep( &sts, tid);
	if ( EVEN(sts)) return 0;

	*type = tdrep->type();
	*size = tdrep->size();
	*elements = tdrep->nElement();
	delete tdrep;
      } catch (wb_error& e) {
	// Try cdrep
	wb_cdrep *cdrep = m_merep->cdrep( &sts, tid);
	if ( EVEN(sts)) return 0;

	*type = (pwr_eType) cdrep->cid();
	*size = cdrep->size( pwr_eBix_rt);
	*elements = 1;
	delete cdrep;
      }
    }
  }
  }
  return 1;
}

void wb_vrepwbl::getClassFlags( pwr_tStatus *sts, pwr_tCid cid, pwr_mClassDef *flags)
{
  // Search type in this volume
  ref_wblnode n = findClass( cid);
  if ( n) {
    if ( !n->o->is_built)
      n->build( 0);

    *flags = n->o->m_flags;
    *sts = LDH__SUCCESS;
  }
  else {
    // Search type in other volumes
    wb_cdrep *cdrep = m_merep->cdrep( sts, cid);
    if ( EVEN(*sts)) return;
    
    *flags = cdrep->flags();

    delete cdrep;
    *sts = LDH__SUCCESS;
  }
}

int wb_vrepwbl::getClassInfo( pwr_tCid cid, size_t *rsize, size_t *dsize)
{
  // Search type in this volume
  ref_wblnode n = findClass( cid);
  if ( n) {
    if ( !n->o->is_built)
      n->build( 0);

    // Get body object
    *rsize = 0;
    *dsize = 0;
    wb_wblnode *n_body = n->o->fch;
    while( n_body) {
      if ( n_body->isObjBodyDef() && 
           (n_body->o->b.bix == pwr_eBix_sys || n_body->o->b.bix == pwr_eBix_rt)) 
        *rsize = n_body->o->b.size;
      else if ( n_body->isObjBodyDef() && n_body->o->b.bix == pwr_eBix_dev) 
        *dsize = n_body->o->b.size;
      n_body = n_body->o->fws;
    }
    return 1;
  }
  else {
    // Search type in other volumes TODO...     
    pwr_tStatus sts;

    wb_cdrep *cdrep = m_merep->cdrep( &sts, cid);
    if ( EVEN(sts)) return 0;
    
    wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
    if ( ODD(sts)) {
      *rsize = bdrep->size();
      delete bdrep;
    }
    else
      *rsize = 0;

    bdrep = cdrep->bdrep( &sts, pwr_eBix_dev);
    if ( ODD(sts)) {
      *dsize = bdrep->size();
      delete bdrep;
    }
    else
      *dsize = 0;

    delete cdrep;
    return 1;
  }
  return 0;
}


#define IF_ATTR( attribute, etype, elem, level) \
 if ( attr->attributeIsEqual( #attribute, level)) { \
        *size = sizeof( o.attribute); \
        *offset = (unsigned long) &o.attribute - (unsigned long) &o; \
        *tid = *type = etype; \
        *elements = elem; \
	*flags = 0;}


int wb_vrepwbl::getAttrInfo( const char *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
                             size_t *offset, pwr_tTid *tid, int *elements, pwr_eType *type,
			     int *flags)
{
  size_t a_size;
  size_t a_offset = 0;
  pwr_tTid a_tid;
  int a_elements;
  pwr_eType a_type;
  int a_flags;


  wb_attrname aname = wb_attrname(attr);
  if ( aname.evenSts()) return 0;

  if ( getAttrInfoRec( &aname, bix, cid, &a_size, &a_offset,
                       &a_tid, &a_elements, &a_type, &a_flags, 0)) {
    *size = a_size;
    *offset = a_offset;
    *tid = a_tid;
    *elements = a_elements;
    *type = a_type;
    *flags = a_flags;
    return 1;
  }
  return 0;
}

int wb_vrepwbl::getAttrInfoRec( wb_attrname *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
                                size_t *offset, pwr_tTid *tid, int *elements, pwr_eType *type,
				int *flags, int level)
{
  if ( level > 0)
    bix = pwr_eBix_rt;

  switch( cid) {
  case pwr_eClass_Type:
  {
    pwr_sType o;

    if ( bix != pwr_eBix_sys)
      return 0;
    if ( attr->attributeIsEqual("Type", level)) {
      *size = sizeof( o.Type);
      *offset = (unsigned long) &o.Type - (unsigned long) &o;
      *tid = *type = pwr_eType_TypeId;
      *elements = 1;
      *flags = 0;
    }
    else if ( attr->attributeIsEqual( "Size", level)) {
      *size = sizeof( o.Size);
      *offset = (unsigned long) &o.Size - (unsigned long) &o;
      *tid = *type = pwr_eType_Int32;
      *elements = 1;
      *flags = 0;
    }
    return 1;
  }
  case pwr_eClass_TypeDef:
  {
    pwr_sTypeDef o;

    if ( bix != pwr_eBix_sys)
      return 0;
    IF_ATTR( Type, pwr_eType_UInt32, 1, level)
    else IF_ATTR( Size, pwr_eType_Int32, 1, level)
    else IF_ATTR( TypeRef, pwr_eType_TypeId, 1, level)
    else IF_ATTR( Elements, pwr_eType_Int32, 1, level)
    return 1;
  }
  case pwr_eClass_ClassDef:
  {
    pwr_sClassDef o;

    if ( bix != pwr_eBix_sys)
      return 0;
    IF_ATTR( Editor, pwr_eType_UInt32, 1, level)
    else IF_ATTR( Method, pwr_eType_UInt32, 1, level)
    else IF_ATTR( Flags, pwr_eType_UInt32, 1, level)
    else IF_ATTR( NumOfObjBodies, pwr_eType_UInt32, 1, level)
    else IF_ATTR( PopEditor, pwr_eType_UInt32, 1, level)
    return 1;
  }
  case pwr_eClass_ClassVolume:
  {
    pwr_sClassVolume o;

    if ( bix != pwr_eBix_sys)
      return 0;
    IF_ATTR( Description, pwr_eType_String, 1, level)
    else IF_ATTR( NextOix, pwr_eType_ObjectIx, 1, level)
    else IF_ATTR( RtVersion, pwr_eType_UInt32, 1, level)
    else IF_ATTR( RtCreTime, pwr_eType_Time, 1, level)
    else IF_ATTR( RtCreator, pwr_eType_String, 1, level)
    else IF_ATTR( RtCardinality, pwr_eType_UInt32, 1, level)
    else IF_ATTR( RtBodySize, pwr_eType_UInt32, 1, level)
    else IF_ATTR( NextCix, pwr_eType_ObjectIx, 1, level)
    else if ( attr->attributeIsEqual( "NextTix", level)) {
      if ( attr->hasAttrIndex()) {
	*size = sizeof( o.NextTix[0]);
	*offset = (unsigned long) &o.NextTix - (unsigned long) &o + 
	  attr->attrIndex() * *size;
	*tid = *type = pwr_eType_ObjectIx;
	*elements = 1;
	*flags = 0;
      }
      else {
	*size = sizeof( o.NextTix);
	*offset = (unsigned long) &o.NextTix - (unsigned long) &o;
	*tid = *type = pwr_eType_ObjectIx;
	*elements = sizeof(o.NextTix)/sizeof(o.NextTix[0]);
	*flags = 0;
      }
    }
    return 1;
  }
  case pwr_eClass_ClassHier:
  case pwr_eClass_TypeHier:
    return 0;
  case pwr_eClass_ObjBodyDef:
  {
    pwr_sObjBodyDef o;

    if ( bix != pwr_eBix_sys)
      return 0;
    IF_ATTR( StructName, pwr_eType_String, 1, level)
    else IF_ATTR( NumOfParams, pwr_eType_UInt32, 1, level)
    else IF_ATTR( Size, pwr_eType_UInt32, 1, level)
    else IF_ATTR( NextAix, pwr_eType_ObjectIx, 1, level)
    else IF_ATTR( Flags, pwr_eType_Mask, 1, level)
    return 1;
  }
  case pwr_eClass_Param:
  {
    pwr_sParam o;

    if ( bix != pwr_eBix_sys)
      return 0;
    if ( attr->attributeIsEqual( "PgmName", level)) {
      *size = sizeof( o.Info.PgmName);
      *offset = (unsigned long) &o.Info.PgmName - (unsigned long) &o;
      *tid = *type = pwr_eType_String;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "Type", level)) {
      *size = sizeof( o.Info.Type);
      *offset = (unsigned long) &o.Info.Type - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "Offset", level)) {
      *size = sizeof( o.Info.Offset);
      *offset = (unsigned long) &o.Info.Offset - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "Size", level)) {
      *size = sizeof( o.Info.Size);
      *offset = (unsigned long) &o.Info.Size - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "Flags", level)) {
      *size = sizeof( o.Info.Flags);
      *offset = (unsigned long) &o.Info.Flags - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "Elements", level)) {
      *size = sizeof( o.Info.Elements);
      *offset = (unsigned long) &o.Info.Elements - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    if ( attr->attributeIsEqual( "ParamIndex", level)) {
      *size = sizeof( o.Info.ParamIndex);
      *offset = (unsigned long) &o.Info.ParamIndex - (unsigned long) &o;
      *tid = *type = pwr_eType_UInt32;
      *elements = 1;
      *flags = 0;
    }
    else IF_ATTR( TypeRef, pwr_eType_TypeId, 1, level)
           return 1;
  }
  default:
  {
    // Search type in this volume
    ref_wblnode n = findClass( cid);
    if ( n) {
      if ( !n->o->is_built)
        n->build( 0);

      // Get body object
      wb_wblnode *n_body = n->o->fch;
      while( n_body) {
        if ( n_body->isObjBodyDef() && n_body->o->b.bix == bix) 
          break;
        n_body = n_body->o->fws;
      }
      if ( !n_body)
        break;

      // Find attribute
      wb_wblnode *n_attr = n_body->o->fch;
      while( n_attr) {
        if ( n_attr->isAttribute() && attr->attributeIsEqual( n_attr->name(), level))
          break;
        n_attr = n_attr->o->fws;
      }
      if ( !n_attr)
        break;

      if ( attr->hasAttrIndex( level)) {
        int index = attr->attrIndex( level);
        if ( index >= n_attr->o->a.elements)
          return 0;

        *offset += n_attr->o->a.offset + index * n_attr->o->a.size / n_attr->o->a.elements;
        *size = n_attr->o->a.size / n_attr->o->a.elements;
      }
      else {
        *offset += n_attr->o->a.offset;
        *size = n_attr->o->a.size;
      }
      if ( attr->hasAttribute( level + 1)) {
        // Subclass: get cid from type of attr
        if ( !getAttrInfoRec( attr, bix, n_attr->o->a.tid, size, offset, tid, elements, type, flags,
                              level + 1)) {
          // Search in other volumes
	  size_t l_offset;

          if ( !m_erep->merep()->getAttrInfoRec( attr, bix, n_attr->o->a.tid, size, &l_offset, tid, 
						 elements, type, flags, level + 1))
	    return 0;
	  else {
	    *offset += l_offset;
	    return 1;
	  }
	}
	return 1;
      }
      *tid = n_attr->o->a.tid;
      *elements = n_attr->o->a.elements;
      *type = n_attr->o->a.type;
      *flags = n_attr->o->a.flags;
      return 1;
    }
    else {
      // Search type in other volumes
      return m_erep->merep()->getAttrInfoRec( attr, bix, cid, size, offset, tid, 
                                              elements, type, flags, level);
    }
  }

  }
  return 0;
}

int wb_vrepwbl::nameToOid( const char *name, pwr_tOid *oid)
{
  if ( strncmp( name, "_O", 2) == 0) {
    cdh_StringToObjid( name, oid);
    return 1;
  }

  ref_wblnode n = find( name);
  if ( n) {
    if ( !n->o->is_built)
      n->build( 0);

    *oid = n->o->m_oid;
    return 1;
  }

  // Search in other volume
  pwr_tStatus sts;

  wb_name na(name);
  
  wb_orep *orep = m_erep->object( &sts, na);
  if ( EVEN(sts))
    return 0;
  else {
    *oid = orep->oid();
    // Delete
    orep->ref();
    orep->unref();

    return 1;
  }
}

int wb_vrepwbl::nameToAttrRef( const char *name, pwr_sAttrRef *attrref)
{
  pwr_tOid oid;
  pwr_tStatus sts;
  size_t a_size;
  size_t a_offset;
  pwr_tTid a_tid;
  int a_elements;
  pwr_eType a_type;
  int a_flags;
  pwr_eBix bix;
  pwr_tCid cid;

  if ( strncmp( name, "_A", 2) == 0) {
    // Fix
    return 0;
  }

  wb_name aname = wb_name(name);
  if ( aname.evenSts()) return 0;

  if ( strncmp( aname.volume(), "$", 1) == 0 &&
       aname.segmentIsEqual( volume_name)) {
    char cname[120];
    pwr_sAttrRef aref;
    pwr_tCid cid;
    pwr_tVid vid;

    if ( aname.volumeIsEqual( "$PlcConnect"))
      vid = ldh_cPlcConnectVolume;
    else if ( aname.volumeIsEqual( "$PlcHost"))
      vid = ldh_cPlcHostVolume;
    else if ( aname.volumeIsEqual( "$IoConnect"))
      vid = ldh_cIoConnectVolume;

    strcpy( cname, aname.segmentsAll());
    char *s = strchr( cname, '-');
    if ( s)
      *s = ':';
    ref_wblnode n = findClass( aname.segment( aname.segments() - 1));
    if ( !n) return 0;

    cid = n->o->c.cid;

    if ( aname.hasAttribute()) {
      strcat( cname, "-Template.");
      strcat( cname, aname.attributesAll());
      sts = nameToAttrRef( cname, &aref);
      if ( EVEN(sts)) return sts;

      *attrref = aref;
      attrref->Objid.vid = vid;
      attrref->Objid.oix = cid;
    }
    else {
      *attrref = pwr_cNAttrRef;
      attrref->Objid.vid = vid;
      attrref->Objid.oix = cid;
      attrref->Flags.b.Object = 1;
    }
    return LDH__SUCCESS;
  }

  ref_wblnode n = find( aname.name( cdh_mName_volume | cdh_mName_path | cdh_mName_object));
  if ( n) {
    if ( !n->o->is_built)
      n->build( 0);

    oid = n->o->m_oid;
    cid = n->o->m_cid;
  }
  else {
    // Search in other volume
    pwr_tStatus sts;

    wb_orep *orep = m_erep->object( &sts, 
                                    aname.name( cdh_mName_volume | cdh_mName_path | cdh_mName_object));
    if ( EVEN(sts))
      return 0;
    else {
      oid = orep->oid();
      cid = orep->cid();
      // Delete
      orep->ref();
      orep->unref();
    }
  }

  if ( !aname.hasAttribute()) {
    // No attribute given, attrref to whole RtBody
    wb_cdrep *cdrep = m_merep->cdrep( &sts, cid);
    if ( EVEN(sts)) return sts;

    wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
    if ( EVEN(sts)) return sts;

    attrref->Objid = oid;
    attrref->Offset = 0;
    attrref->Size = bdrep->size();
    attrref->Body = cdh_TypeObjidToId( bdrep->boid());

    delete cdrep;
    delete bdrep;
  }
  else {
    aname.setShadowed(true);
    wb_attrname an = wb_attrname( aname.name( cdh_mName_attribute));
    if ( an.evenSts()) return an.sts();
    an.setShadowed( true);

    // Try rtbody
    bix = pwr_eBix_rt;
    sts = getAttrInfo( an.name(), bix, cid, &a_size,
                       &a_offset, &a_tid, &a_elements, &a_type, &a_flags);
    if ( EVEN(sts)) {
      // Try devbody
      bix = pwr_eBix_dev;
      sts = getAttrInfo( an.name(), pwr_eBix_dev, cid, &a_size,
                         &a_offset, &a_tid, &a_elements, &a_type, &a_flags);
      if ( EVEN(sts)) return sts;
    }
    attrref->Objid = oid;
    attrref->Offset = a_offset;
    attrref->Size = a_size;
    attrref->Body = a_tid;
    attrref->Flags.m = 0;
    if ( a_flags & pwr_mAdef_pointer && !(a_flags & pwr_mAdef_private))
      attrref->Flags.b.Indirect = 1;
  }
  return LDH__SUCCESS;
}

int wb_vrepwbl::getTemplateBody( pwr_tCid cid, pwr_eBix bix, size_t *size, void **body)
{
  switch ( cid) {
  case pwr_eClass_Type:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sType);
    *body = calloc( 1, *size);
    return 1;
  case pwr_eClass_TypeDef:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sTypeDef);
    *body = calloc( 1, *size);
    return 1;
  case pwr_eClass_ClassDef:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sClassDef);
    *body = calloc( 1, *size);
    return 1;
  case pwr_eClass_ClassVolume:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sClassVolume);
    *body = calloc( 1, *size);
    return 1;
  case pwr_eClass_ClassHier:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = 0;
    *body = 0;
    return 1;
  case pwr_eClass_TypeHier:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = 0;
    *body = 0;
    return 1;
  case pwr_eClass_ObjBodyDef:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sObjBodyDef);
    *body = calloc( 1, *size);
    return 1;
  case pwr_eClass_Param:
    if ( bix != pwr_eBix_sys)
      return 0;
    *size = sizeof( pwr_sParam);
    *body = calloc( 1, *size);
    return 1;
  default:
  {
    // Search class in this volume
    ref_wblnode n = findClass( cid);
    if ( n) {
      if ( !n->o->is_built)
        n->build( 0);

      if ( !n->o->c.templ)
        return 0;

      if ( bix == pwr_eBix_sys || bix == pwr_eBix_rt) {
        *size = n->o->c.templ->o->rbody_size;
        if ( *size) {
          *body = calloc( 1, *size);
          memcpy( *body, n->o->c.templ->o->rbody, *size);
        }
      }
      else if ( bix == pwr_eBix_dev) {
        *size = n->o->c.templ->o->dbody_size;
        if ( *size) {
          *body = calloc( 1, *size);
          memcpy( *body, n->o->c.templ->o->dbody, *size);
        }
      }
      return 1;
    }
    else {
      // Search type in other volumes TODO...     
      pwr_tStatus sts;

      wb_cdrep *cdrep = m_merep->cdrep( &sts, cid);
      if ( EVEN(sts)) return 0;
    
      wb_bdrep *bdrep = cdrep->bdrep( &sts, bix);
      if ( EVEN(sts)) {
	delete cdrep;
	return 0;
      }

      *size = bdrep->size();
      delete bdrep;
         
      if ( *size) {
        *body = calloc( 1, *size);
        cdrep->templateBody( &sts, (pwr_eBix) bix, *body, pwr_cNObjid);
        if ( EVEN(sts)) {
          free( body);
          delete cdrep;
          return 0;
        }
      }
      delete cdrep;
      return 1;
    }
  }
  }
  return 0;
}

int wb_vrepwbl::load_files( const char *file_spec)
{
  char found_file[200];
  int sts;

  sts = dcli_search_file( (char *)file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  while( ODD(sts)) {
    ifstream s(found_file);

    if ( strchr( found_file, '#')) {
      // Ignore files with '#'
      sts = dcli_search_file( (char *)file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
      continue;
    }

    if ( file_cnt >= WBL_FILEMAX)
      return 0;

    file[file_cnt] = new wb_wblfile();
    file[file_cnt]->lexer = new wb_wbllexer(s);
    file[file_cnt]->lexer->setFilename( found_file);

    file[file_cnt]->parser = new wb_wblparser(*file[file_cnt]->lexer);
    file[file_cnt]->parser->setFilename( found_file);

    // Antlr 2.7.3 seems to want an ASTFactory here...
    // wb_wblfactory *factory = new wb_wblfactory();
    // file[file_cnt]->parser->setASTNodeFactory( factory);
    file[file_cnt]->parser->setASTNodeFactory( wb_wblnode::factory);

    try {
      // Parse the input expression
      file[file_cnt]->parser->unit();
      AST *a = file[file_cnt]->parser->getAST();
      file[file_cnt]->rootAST = (wb_wblnode *) a; 
      if ( !file[file_cnt]->rootAST) {
        error("File empty", found_file, 0);
        file_cnt--;
      }
      else {
        struct stat info;
        if ( stat( found_file, &info) != -1) {
          file[file_cnt]->time.tv_sec = info.st_mtime;
          file[file_cnt]->time.tv_nsec = 0;
        }
        else
          error( "Unable to get file date", found_file, 0);
        strcpy( file[file_cnt]->file_name, found_file);
        file[file_cnt]->rootAST->setFile( file[file_cnt]);
      }
    }
    catch(exception& e) {
      cerr << "exception: " << e.what() << " " << found_file << " line: " << 
        file[file_cnt]->lexer->getLine() << endl;
      error_cnt++;
      return LDH__WBLPARSE;
    }
    file_cnt++;
    sts = dcli_search_file( (char *)file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( (char *)file_spec, found_file, DCLI_DIR_SEARCH_END);

  return 1;
}

bool wb_vrepwbl::registerObject( pwr_tOix oix, ref_wblnode node)
{

#if 0
  cout << "RegObj: " << oix << "  " << node->name() << endl;

  ref_wblnode n = findObject( oix);
  if ( n)
    // Already registred
    return 0;

  m_oix_list[oix] = node;
#endif
  pair<pwr_tOix, ref_wblnode>p(oix,node);
  pair<map<pwr_tOix,ref_wblnode>::iterator,bool>result = m_oix_list.insert(p);

  return result.second;
}

void wb_vrepwbl::registerClass( const char *name, pwr_tCid cid, ref_wblnode node)
{
  string sname(name);

  m_cid_list[cid] = node;
  m_class_list[sname] = node;
}

void wb_vrepwbl::registerType( const char *name, pwr_tTid tid, ref_wblnode node)
{
  string sname(name);

  m_tid_list[tid] = node;
  m_type_list[sname] = node;
}

void wb_vrepwbl::registerVolume( const char *name, pwr_tCid cid, pwr_tVid vid, ref_wblnode node)
{
  m_vid = vid;
  strcpy( volume_name, name);
  strcpy( m_name, name);
  m_cid = cid;
  volume_node = node;
}


ref_wblnode wb_vrepwbl::find( const char *name)
{
  wb_name oname = wb_name(name);

  if ( oname.evenSts() || (oname.hasVolume() && !oname.volumeIsEqual(volume_name)))
    return 0;

  if ( volume_node)
    return volume_node->find( &oname, 0);
  return 0;
}

void wb_vrepwbl::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_vrep *wb_vrepwbl::ref()
{
  m_nRef++;
  return this;
}

wb_orep *wb_vrepwbl::object(pwr_tStatus *sts)
{
  wb_orepwbl *orep = 0;

  if ( volume_node && volume_node->o->fch) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, volume_node->o->fch);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NOSUCHOBJ;

  return orep;
}

wb_orep *wb_vrepwbl::ancestor(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;

  wb_wblnode *n = ((wb_orepwbl *)o)->wblNode();
  while ( n->o->fth)
    ;

  orep = new wb_orepwbl( (wb_vrepwbl *)this, n);

  *sts = LDH__SUCCESS;
  return orep;
}

wb_orep *wb_vrepwbl::parent(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;
        
  if ( ((wb_orepwbl *)o)->wblNode()->o->fth && 
       ((wb_orepwbl *)o)->wblNode()->o->fth != root_object) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o->fth);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_PARENT;
    
  return orep;
}

wb_orep *wb_vrepwbl::after(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;
    
  if ( ((wb_orepwbl *)o)->wblNode()->o->fws) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o->fws);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_SIBLING;
  return orep;
}

wb_orep *wb_vrepwbl::before(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;
    
  if ( ((wb_orepwbl *)o)->wblNode()->o->bws) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o->bws);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_SIBLING;
    
  return orep;
}

wb_orep *wb_vrepwbl::first(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;
    
  if ( ((wb_orepwbl *)o)->wblNode()->o->fch) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o->fch);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_CHILD;
    
  return orep;
}

wb_orep *wb_vrepwbl::child(pwr_tStatus *sts, const wb_orep *o, wb_name &name)
{
  wb_orep *child = first( sts, o);
  while( ODD(*sts)) {
    if ( name.segmentIsEqual( child->name())) {
      *sts = LDH__SUCCESS;
      return child;
    }
    child->ref();
    wb_orep *prev = child;
    child = after( sts, child);
    prev->unref();
  }
  *sts = LDH__NOSUCHOBJ;
  return 0;
}

wb_orep *wb_vrepwbl::last(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepwbl *orep = 0;

  ref_wblnode n = ((wb_orepwbl *)o)->wblNode()->get_o_lch();
    
  if ( n) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, n);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_CHILD;
    
  return orep;
}

wb_orep *wb_vrepwbl::next(pwr_tStatus *sts, const wb_orep *o)
{
  return 0;
}

wb_orep *wb_vrepwbl::previous(pwr_tStatus *sts, const wb_orep *o)
{
  return 0;
}

void wb_vrepwbl::objectName(const wb_orep *o, char *str)
{
  *str = 0;
        
  // Count ancestors
  int cnt = 0;
  wb_wblnode *n = ((wb_orepwbl *)o)->wblNode();
  while ( n) {
    cnt++;
    n = n->o->fth;
  }

  wb_wblnode **vect = (wb_wblnode **) calloc( cnt, sizeof(vect));

  n = ((wb_orepwbl *)o)->wblNode();
  for ( int i = 0; i < cnt; i++) {
    vect[i] = n;
    n = n->o->fth;
  }

  for ( int i = cnt - 1; i >= 0; i--) {
    strcat( str, vect[i]->name());
    if ( i == cnt - 1)
      strcat( str, ":");
    else if ( i != 0)
      strcat( str, "-");
  }
  free( vect);
}

void *wb_vrepwbl::readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  *sts = LDH__SUCCESS;
    
  wb_wblnode *n = ((wb_orepwbl *) o)->wblNode();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->o->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, (char *)n->o->rbody + offset, MIN(n->o->rbody_size - offset, size));
      return p;
    }
    return (void *)((char *)n->o->rbody + offset);
  case pwr_eBix_dev:
    if ( n->o->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, (char *)n->o->dbody + offset, MIN(n->o->dbody_size - offset, size));
      return p;
    }
    return (void *)((char *)n->o->dbody + offset);
  default:
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
}

void *wb_vrepwbl::readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__SUCCESS;
    
  wb_wblnode *n = ((wb_orepwbl *) o)->wblNode();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->o->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, n->o->rbody, n->o->rbody_size);
      return p;
    }
    return n->o->rbody;
  case pwr_eBix_dev:
    if ( n->o->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, n->o->dbody, n->o->dbody_size);
      return p;
    }
    return n->o->dbody;
  default:
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
}
