
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
#include "wb_erep.h"
#include "wb_ldh_msg.h"

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

wb_vrep *wb_vrepwbl::next() const
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

void wb_vrepwbl::error( char *msg, char *file, int line_number)
{
  cout << "Wbl error: " << msg << ", " << file << " line: " << line_number << endl;
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
    cout << "Tid_list : " << it->first << " " << it->second->name << 
      "    size: " << it->second->ty_size << " elem: " << it->second->ty_elements <<  endl;

  for ( iterator_class_list it = m_class_list.begin(); 
	it != m_class_list.end(); it++)
    cout << "Class_list : " << it->first << " " << it->second->name << 
      "    line: " << it->second->line_number << endl;

  for ( iterator_cid_list it = m_cid_list.begin(); 
	it != m_cid_list.end(); it++)
    cout << "Cid_list : " << it->first << " " << it->second->name << 
      "    line: " << it->second->line_number << endl;

  // for ( int i = 0; i < file_cnt; i++)
  //   rootAST[i]->info(0);
}

bool
wb_vrepwbl::createSnapshot(char *fileName)
{
    try {
        wb_dbs dbs(this);
        
        dbs.buildFile();

        return true;
    } catch (wb_error& e) {
        return false;
    }
}

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

int wb_vrepwbl::load( char *fname)
{
  int i;
  char file_spec[200];

  if (1) {
    // Load all wb_load files in directory

    // Load volume
    sprintf( file_spec, "%s/*_v.wb_load", fname);
    load_files( file_spec);

    // Load class hierarchies
    sprintf( file_spec, "%s/*_ch_*.wb_load", fname);
    load_files( file_spec);

    // Load types
    sprintf( file_spec, "%s/*_t_*.wb_load", fname);
    load_files( file_spec);

    // Load types
    sprintf( file_spec, "%s/*_td_*.wb_load", fname);
    load_files( file_spec);

    // Load classes
    sprintf( file_spec, "%s/*_c_*.wb_load", fname);
    load_files( file_spec);

    // Load some baseclasses...
    // sprintf( file_spec, "%s/pwrb_c_*.wb_load", fname);
    // load_files( file_spec);

    // Load classes
    // sprintf( file_spec, "%s/*_c*.wb_load", fname);
    // load_files( file_spec);
  }
  for ( i = 0; i < file_cnt; i++)
    file[i]->rootAST->registerNode( this);

  for ( i = 0; i < file_cnt; i++) {
    file[i]->rootAST->link( this, 0);
  }
  // if ( root_object)
  //  root_object->info_link( 0);

  if ( root_object)
    root_object->build( 1);

  return 1;
}

int wb_vrepwbl::classNameToCid( char *name, pwr_tCid *cid)
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

ref_wblnode wb_vrepwbl::findClass( char *name)
{
  string sname(name);
  iterator_class_list it = m_class_list.find( sname);
  if ( it == m_class_list.end())
    return 0;
  return it->second;
}

ref_wblnode wb_vrepwbl::findType( char *name)
{
  wb_name wname( name);

  if ( wname.hasVolume() && !wname.volumeIsEqual(volume_name))
    return 0;

  string sname(wname.object());
  iterator_type_list it = m_type_list.find( sname); 
  if ( it == m_type_list.end())
    return 0;
  if ( !it->second->is_built)
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


int wb_vrepwbl::getTypeInfo( char *name, pwr_tTid *tid, pwr_eType *type, int *size,
			     int *elements)
{
  bool type_extern = false;
  char tname[80];
  wb_name wname( name);
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
      ref_wblnode n = findType( tname);
      if ( n) {
        *tid = n->m_tid;
        *type = (pwr_eType) n->ty_tid;
        *size = n->ty_size;
        *elements = 1;
        return 1;
      }
      else
        type_extern = true;
    }
  }
  if ( type_extern) {
    return 0; // TODO...
  }
  return 1;
}

int wb_vrepwbl::getTypeInfo( pwr_tTid tid, pwr_eType *type, int *size,
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
        if ( !n->is_built)
          n->build( 0);
        *type = (pwr_eType) n->ty_tid;
        *size = n->ty_size;
        *elements = 1;
      }
      else {
        // Search type in other volumes TODO...     
        return 0;
      }
    }
  }
  return 1;
}

int wb_vrepwbl::getClassInfo( pwr_tCid cid, int *rsize, int *dsize)
{
  // Search type in this volume
  ref_wblnode n = findClass( cid);
  if ( n) {
    if ( !n->is_built)
      n->build( 0);

    // Get body object
    *rsize = 0;
    *dsize = 0;
    wb_wblnode *n_body = n->o_fch;
    while( n_body) {
      if ( n_body->isObjBodyDef() && 
	   (n_body->b_bix == wbl_eBix_SysBody || n_body->b_bix == wbl_eBix_RtBody)) 
	*rsize = n_body->b_size;
      else if ( n_body->isObjBodyDef() && n_body->b_bix == wbl_eBix_DevBody) 
	*dsize = n_body->b_size;
      n_body = n_body->o_fws;
    }
    return 1;
  }
  else {
    // Search type in other volumes TODO...     
    return 0;
  }
  return 0;
}


#define IF_ATTR( attribute, type, elem, level) \
 if ( attr->attributeIsEqual( #attribute, level)) { \
        *size = sizeof( o.attribute); \
        *offset = (unsigned long) &o.attribute - (unsigned long) &o; \
        *tid = type; \
        *elements = elem; }


int wb_vrepwbl::getAttrInfo( char *attr, int bix, pwr_tCid cid, int *size,
		     int *offset, pwr_tTid *tid, int *elements)
{
  char attr_name[200];
  int a_size;
  int a_offset = 0;
  pwr_tTid a_tid;
  int a_elements;

  strcpy( attr_name, ".");
  strcat( attr_name, attr);

  wb_name aname( attr_name);
  if ( aname.evenSts()) return 0;

  if ( getAttrInfoRec( &aname, bix, cid, &a_size, &a_offset, 
		       &a_tid, &a_elements, 0)) {
    *size = a_size;
    *offset = a_offset;
    *tid = a_tid;
    *elements = a_elements;
    return 1;
  }
  return 0;
}

int wb_vrepwbl::getAttrInfoRec( wb_name *attr, int bix, pwr_tCid cid, int *size,
		     int *offset, pwr_tTid *tid, int *elements, int level)
{
  switch( cid) {
    case pwr_eClass_Type:
    {
      pwr_sType o;

      if ( bix != wbl_eBix_SysBody)
        return 0;
      if ( attr->attributeIsEqual("Type", level)) {
        *size = sizeof( o.Type);
        *offset = (unsigned long) &o.Type - (unsigned long) &o;
        *tid = pwr_eType_TypeId;
        *elements = 1;
      }
      else if ( attr->attributeIsEqual( "Size", level)) {
        *size = sizeof( o.Size);
        *offset = (unsigned long) &o.Size - (unsigned long) &o;
        *tid = pwr_eType_Int32;
        *elements = 1;
      }
      return 1;
    }
    case pwr_eClass_TypeDef:
    {
      pwr_sTypeDef o;

      if ( bix != wbl_eBix_SysBody)
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

      if ( bix != wbl_eBix_SysBody)
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

      if ( bix != wbl_eBix_SysBody)
        return 0;
      IF_ATTR( Description, pwr_eType_String, 1, level)
      else IF_ATTR( NextOix, pwr_eType_ObjectIx, 1, level)
      else IF_ATTR( RtVersion, pwr_eType_UInt32, 1, level)
      else IF_ATTR( RtCreTime, pwr_eType_Time, 1, level)
      else IF_ATTR( RtCreator, pwr_eType_String, 1, level)
      else IF_ATTR( RtCardinality, pwr_eType_UInt32, 1, level)
      else IF_ATTR( RtBodySize, pwr_eType_UInt32, 1, level)
      else IF_ATTR( NextCix, pwr_eType_ObjectIx, 1, level)
      return 1;
    }
    case pwr_eClass_ClassHier:
    case pwr_eClass_TypeHier:
      return 0;
    case pwr_eClass_ObjBodyDef:
    {
      pwr_sObjBodyDef o;

      if ( bix != wbl_eBix_SysBody)
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

      if ( bix != wbl_eBix_SysBody)
        return 0;
      if ( attr->attributeIsEqual( "PgmName", level)) {
        *size = sizeof( o.Info.PgmName);
        *offset = (unsigned long) &o.Info.PgmName - (unsigned long) &o;
        *tid = pwr_eType_String;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "Type", level)) {
        *size = sizeof( o.Info.Type);
        *offset = (unsigned long) &o.Info.Type - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "Offset", level)) {
        *size = sizeof( o.Info.Offset);
        *offset = (unsigned long) &o.Info.Offset - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "Size", level)) {
        *size = sizeof( o.Info.Size);
        *offset = (unsigned long) &o.Info.Size - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "Flags", level)) {
        *size = sizeof( o.Info.Flags);
        *offset = (unsigned long) &o.Info.Flags - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "Elements", level)) {
        *size = sizeof( o.Info.Elements);
        *offset = (unsigned long) &o.Info.Elements - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      if ( attr->attributeIsEqual( "ParamIndex", level)) {
        *size = sizeof( o.Info.ParamIndex);
        *offset = (unsigned long) &o.Info.ParamIndex - (unsigned long) &o;
        *tid = pwr_eType_UInt32;
        *elements = 1;
      }
      else IF_ATTR( TypeRef, pwr_eType_TypeId, 1, level)
      return 1;
    }
    default:
    {
      // Search type in this volume
      ref_wblnode n = findClass( cid);
      if ( n) {
        if ( !n->is_built)
          n->build( 0);

        // Get body object
        wb_wblnode *n_body = n->o_fch;
        while( n_body) {
          if ( n_body->isObjBodyDef() && n_body->b_bix == bix) 
            break;
	  n_body = n_body->o_fws;
	}
        if ( !n_body)
          break;

	// Find attribute
        wb_wblnode *n_attr = n_body->o_fch;
        while( n_attr) {
          if ( n_attr->isAttribute() && attr->attributeIsEqual( n_attr->name, level))
            break;
	  n_attr = n_attr->o_fws;
	}
        if ( !n_attr)
          break;

        if ( attr->hasAttrIndex( level)) {
          int index = attr->attrIndex( level);
          if ( index >= n_attr->a_elements)
            return 0;

          *offset += n_attr->a_offset + index * n_attr->a_size / n_attr->a_elements;
          *size = n_attr->a_size / n_attr->a_elements;
        }
        else {
          *offset += n_attr->a_offset;
          *size = n_attr->a_size;
	}
        if ( attr->hasAttribute( level + 1)) {
          if ( !getAttrInfoRec( attr, bix, cid, size, offset, tid, elements, level + 1))
            return 0;
        }
        *tid = n_attr->a_tid;
        *elements = n_attr->a_elements;
        return 1;
      }
      else {
        // Search type in other volumes TODO...     
        return 0;
      }
    }
  }
  return 0;
}

int wb_vrepwbl::nameToOid( char *name, pwr_tOid *oid)
{
  ref_wblnode n = find( name);
  if ( n) {
    *oid = n->m_oid;
    return 1;
  }
  // TODO global
  return 0;
}

int wb_vrepwbl::getTemplateBody( pwr_tCid cid, int bix, int *size, void **body)
{
  switch ( cid) {
    case pwr_eClass_Type:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sType);
      *body = calloc( 1, *size);
      return 1;
    case pwr_eClass_TypeDef:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sTypeDef);
      *body = calloc( 1, *size);
      return 1;
    case pwr_eClass_ClassDef:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sClassDef);
      *body = calloc( 1, *size);
      return 1;
    case pwr_eClass_ClassVolume:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sClassVolume);
      *body = calloc( 1, *size);
      return 1;
    case pwr_eClass_ClassHier:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = 0;
      *body = 0;
      return 1;
    case pwr_eClass_TypeHier:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = 0;
      *body = 0;
      return 1;
    case pwr_eClass_ObjBodyDef:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sObjBodyDef);
      *body = calloc( 1, *size);
      return 1;
    case pwr_eClass_Param:
      if ( bix != wbl_eBix_SysBody)
        return 0;
      *size = sizeof( pwr_sParam);
      *body = calloc( 1, *size);
      return 1;
    default:
    {
      // Search class in this volume
      ref_wblnode n = findClass( cid);
      if ( n) {
        if ( !n->is_built)
          n->build( 0);

        if ( !n->c_template)
          return 0;

        if ( bix == wbl_eBix_SysBody || bix == wbl_eBix_RtBody) {
          *size = n->c_template->rbody_size;
          if ( *size) {
            *body = calloc( 1, *size);
            memcpy( *body, n->c_template->rbody, *size);
          }
        }
        else if ( bix == wbl_eBix_DevBody) {
          *size = n->c_template->dbody_size;
          if ( *size) {
            *body = calloc( 1, *size);
            memcpy( *body, n->c_template->dbody, *size);
          }
        }
        return 1;
      }
      else {
        // Search type in other volumes TODO...     
        return 0;
      }
    }
  }
  return 0;
}

int wb_vrepwbl::load_files( char *file_spec)
{
  char found_file[200];
  int sts;

  sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  while( ODD(sts)) {
    ifstream s(found_file);

    if ( file_cnt >= WBL_FILEMAX)
      return 0;

    file[file_cnt] = new wb_wblfile();
    file[file_cnt]->lexer = new wb_wbllexer(s);
    file[file_cnt]->lexer->setFilename( found_file);

    file[file_cnt]->parser = new wb_wblparser(*file[file_cnt]->lexer);
    file[file_cnt]->parser->setFilename( found_file);
    file[file_cnt]->parser->setASTNodeFactory( wb_wblnode::factory);

    try {
      // Parse the input expression
      file[file_cnt]->parser->unit();
      file[file_cnt]->rootAST = file[file_cnt]->parser->getAST();
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
      return 0;
    }
    file_cnt++;
    sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_END);

  return 1;
}

bool wb_vrepwbl::registerObject( pwr_tOix oix, ref_wblnode node)
{
#if 0
  cout << "RegObj: " << oix << "  " << node->name << endl;

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

void wb_vrepwbl::registerClass( char *name, pwr_tCid cid, ref_wblnode node)
{
  string sname(name);

  m_cid_list[cid] = node;
  m_class_list[sname] = node;
}

void wb_vrepwbl::registerType( char *name, pwr_tTid tid, ref_wblnode node)
{
  string sname(name);

  m_tid_list[tid] = node;
  m_type_list[sname] = node;
}

void wb_vrepwbl::registerVolume( char *name, pwr_tCid cid, pwr_tVid vid, ref_wblnode node)
{
  m_vid = vid;
  strcpy( volume_name, name);
  strcpy( m_name, name);
  m_cid = cid;
  volume_node = node;
}


ref_wblnode wb_vrepwbl::find( char *name)
{
  wb_name oname(name);

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

  if ( volume_node->o_fch) {
    orep = new wb_orepwbl( (wb_vrepwbl *)this, volume_node->o_fch);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NOSUCHOBJ;

  return orep;
}

wb_orep *wb_vrepwbl::ancestor(pwr_tStatus *sts, wb_orep *o)
{
  wb_orepwbl *orep = 0;

  wb_wblnode *n = ((wb_orepwbl *)o)->wblNode();
  while ( n->o_fth)
    ;

  orep = new wb_orepwbl( (wb_vrepwbl *)this, n);

  *sts = LDH__SUCCESS;
  return orep;
}

wb_orep *wb_vrepwbl::parent(pwr_tStatus *sts, wb_orep *o)
{
    wb_orepwbl *orep = 0;
        
    if ( ((wb_orepwbl *)o)->wblNode()->o_fth) {
      orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o_fth);
      *sts = LDH__SUCCESS;
    }
    else
      *sts = LDH__NO_PARENT;
    
    return orep;
}

wb_orep *wb_vrepwbl::after(pwr_tStatus *sts, wb_orep *o)
{
    wb_orepwbl *orep = 0;
    
    if ( ((wb_orepwbl *)o)->wblNode()->o_fws) {
      orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o_fws);
      *sts = LDH__SUCCESS;
    }
    else
      *sts = LDH__NO_SIBLING;
    return orep;
}

wb_orep *wb_vrepwbl::before(pwr_tStatus *sts, wb_orep *o)
{
    wb_orepwbl *orep = 0;
    
    if ( ((wb_orepwbl *)o)->wblNode()->o_bws) {
      orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o_bws);
      *sts = LDH__SUCCESS;
    }
    else
      *sts = LDH__NO_SIBLING;
    
    return orep;
}

wb_orep *wb_vrepwbl::first(pwr_tStatus *sts, wb_orep *o)
{
    wb_orepwbl *orep = 0;
    
    if ( ((wb_orepwbl *)o)->wblNode()->o_fch) {
      orep = new wb_orepwbl( (wb_vrepwbl *)this, ((wb_orepwbl *)o)->wblNode()->o_fch);
      *sts = LDH__SUCCESS;
    }
    else
      *sts = LDH__NO_CHILD;
    
    return orep;
}

wb_orep *wb_vrepwbl::child(pwr_tStatus *sts, wb_orep *o, char *name)
{
    return 0;
}

wb_orep *wb_vrepwbl::last(pwr_tStatus *sts, wb_orep *o)
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

wb_orep *wb_vrepwbl::next(pwr_tStatus *sts, wb_orep *o)
{
    return 0;
}

wb_orep *wb_vrepwbl::previous(pwr_tStatus *sts, wb_orep *o)
{
    return 0;
}

void wb_vrepwbl::objectName(wb_orep *o, char *str) const
{
    *str = 0;
        
    // Count ancestors
    int cnt = 0;
    wb_wblnode *n = ((wb_orepwbl *)o)->wblNode();
    while ( n) {
      cnt++;
      n = n->o_fth;
    }

    wb_wblnode **vect = (wb_wblnode **) calloc( cnt, sizeof(vect));

    n = ((wb_orepwbl *)o)->wblNode();
    for ( int i = 0; i < cnt; i++) {
      vect[i] = n;
      n = n->o_fth;
    }

    for ( int i = cnt - 1; i >= 0; i--) {
      strcat( str, vect[i]->name);
      if ( i == cnt - 1)
        strcat( str, ":");
      else if ( i != 0)
        strcat( str, "-");
    }
    free( vect);
}

