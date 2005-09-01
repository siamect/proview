/* 
 * Proview   $Id: rt_bck_print.c,v 1.5 2005-09-01 14:57:48 claes Exp $
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
 */

/* rt_bck_print.c -- Print backup data */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "rt_gdh.h"
#include "rt_errh.h"

int print_object( pwr_tObjid objid, pwr_tClassId classid, char *object_p, 
		  int offset, char *prefix, FILE *fp);
int print_attribute( pwr_tObjid objid, pwr_tClassId classid, char *object_p, char *attributename,
		     int array_element, int index, FILE *fp);

static void  attrvalue_to_string( int type_id, void *value_ptr, 
	char *str, int size, int *len, char *format)
{
  char			hiername[120];
  pwr_tObjid		objid;
  pwr_sAttrRef		*attrref;
  int			sts;
  char			timstr[64];

  if ( value_ptr == 0)
  {
    strcpy( str, "UNDEFINED");
    return;
  }

  switch ( type_id )
  {
    case pwr_eType_Boolean:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(pwr_tBoolean *)value_ptr);
      else
        *len = sprintf( str, format, *(pwr_tBoolean *)value_ptr);
      break;
    }
    case pwr_eType_Float32:
    {
      if ( !format)
        *len = sprintf( str, "%f", *(float *)value_ptr);
      else
        *len = sprintf( str, format, *(float *)value_ptr);
      break;
    }
    case pwr_eType_Float64:
    {
      if ( !format)
        *len = sprintf( str, "%f", *(double *)value_ptr);
      else
        *len = sprintf( str, format, *(double *)value_ptr);
      break;
    }
    case pwr_eType_Char:
    {
      if ( !format)
        *len = sprintf( str, "%c", *(char *)value_ptr);
      else
        *len = sprintf( str, format, *(char *)value_ptr);
      break;
    }
    case pwr_eType_Int8:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(char *)value_ptr);
      else
        *len = sprintf( str, format, *(char *)value_ptr);
      break;
    }
    case pwr_eType_Int16:
    {
      if ( !format)
        *len = sprintf( str, "%hd", *(short *)value_ptr);
      else
        *len = sprintf( str, format, *(short *)value_ptr);
      break;
    }
    case pwr_eType_Int32:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(int *)value_ptr);
      else
        *len = sprintf( str, format, *(int *)value_ptr);
      break;
    }
    case pwr_eType_UInt8:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(unsigned char *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned char *)value_ptr);
      break;
    }
    case pwr_eType_UInt16:
    {
      if ( !format)
        *len = sprintf( str, "%hd", *(unsigned short *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned short *)value_ptr);
      break;
    }
    case pwr_eType_UInt32:
    case pwr_eType_Mask:
    case pwr_eType_Enum:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(unsigned int *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned int *)value_ptr);
      break;
    }
    case pwr_eType_String:
    {
      strncpy( str, (char *)value_ptr, size);
      str[size-1] = 0;
      *len = strlen(str);
      break;
    }
    case pwr_eType_Objid:
    {
      objid = *(pwr_tObjid *)value_ptr;
      if ( !objid.oix)
        sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
			 cdh_mName_volumeStrict);
      else
        sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
		cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_AttrRef:
    {
      attrref = (pwr_sAttrRef *) value_ptr;
      sts = gdh_AttrrefToName ( attrref, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_Time:
    {
      sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_DateAndTime, 
		timstr, sizeof(timstr));
      if ( EVEN(sts))
        strcpy( timstr, "-");
      *len = sprintf( str, "%s", timstr);
      break;
    }
    case pwr_eType_DeltaTime:
    {
      sts = time_DtoAscii( (pwr_tDeltaTime *) value_ptr, 1, 
		timstr, sizeof(timstr));
      if ( EVEN(sts))
        strcpy( timstr, "Undefined time");
      *len = sprintf( str, "%s", timstr);
      break;
    }
    case pwr_eType_ObjectIx:
    {
      *len = sprintf( str, "%s", cdh_ObjectIxToString( NULL, 
		*(pwr_tObjectIx *) value_ptr, 1));
      break;
    }
    case pwr_eType_ClassId:
    {
      objid = cdh_ClassIdToObjid( *(pwr_tClassId *) value_ptr);
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_TypeId:
    {
      objid = cdh_TypeIdToObjid( *(pwr_tTypeId *) value_ptr);
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_VolumeId:
    {
      *len = sprintf( str, "%s", cdh_VolumeIdToString( NULL, 
		*(pwr_tVolumeId *) value_ptr, 1, 0));
      break;
    }
    case pwr_eType_RefId:
    {
      *len = sprintf( str, "%s", cdh_SubidToString( NULL, 
		*(pwr_tSubid *) value_ptr, 1));
      break;
    }

  }
}

static int print_attr( char *object_p, pwr_tObjid objid,
		       char *object_name, char *attr_name, int type_id, int size, 
		       int offset, int elements, FILE *fp)
{
  int len;
  char buf[120];
  int i;

  if ( elements <= 1) {
    attrvalue_to_string( type_id, object_p + offset, buf, sizeof(buf), &len, NULL);
    if ( fp)
      fprintf( fp, "%s.%s %s\n", object_name, attr_name, buf);
    else
      printf( "%s.%s %s\n", object_name, attr_name, buf);
  }
  else {
    for ( i = 0; i < elements; i++) {
      attrvalue_to_string( type_id, object_p + offset + size / elements * i, buf, sizeof(buf),
			 &len, NULL);
      if ( fp)
	fprintf( fp, "%s.%s[%d] %s\n", object_name, attr_name, i, buf);
      else
	printf( "%s.%s[%d] %s\n", object_name, attr_name, i, buf);
    }
  }
  return 1;
}

int print_data( pwr_sAttrRef *arp, FILE *fp)
{
  int		sts;
  char		*s;
  pwr_tClassId	classid;
  char		*object_p;
  char		dataname[120];
  char		objectname[120];
  char		attributename[120];
  pwr_tObjid	objid;
  int 		object_backup;
  int		array_element;
  int		index;
  int		nr;

  sts = gdh_AttrrefToName( arp, dataname, sizeof(dataname), cdh_mNName);
  if ( EVEN(sts)) return sts;

  strcpy( objectname, dataname);
  if ( (s = strchr( objectname, '.'))) {
    *s = 0;
    object_backup = 0;
    strcpy( attributename, dataname);
    if ( (s = strchr( dataname, '['))) {
      array_element = 1;

      nr = sscanf( s+1, "%d", &index);
      if ( nr != 1) return 0;
    }
  }
  else {
    object_backup = 1;
  }

  sts = gdh_NameToObjid( objectname, &objid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( objid, (void **)&object_p);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetObjectClass( objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( object_backup) {
    print_object( objid, classid, object_p, 0, objectname, fp);
  }
  else {
    print_attribute( objid, classid, object_p, attributename, array_element, index, fp);
  }
  return 1;
}

int print_attribute( pwr_tObjid objid, pwr_tClassId classid, char *object_p, char *attributename,
		  int array_element, int index, FILE *fp)
{
  pwr_tTypeId tid;
  pwr_tUInt32 size, offs, elem;
  char objectname[120];
  char parname[120];
  char *s;
  int sts;
  char buf[1024];

  strcpy( objectname, attributename);
  s = strchr( objectname, '.');
  strcpy( parname, s+1);
  *s = 0;

  sts = gdh_GetAttributeCharacteristics( attributename, &tid, &size, &offs, &elem);
  if ( EVEN(sts)) return sts;

  if ( !array_element) {
    sts = gdh_GetObjectInfo( attributename, buf, sizeof(buf));
    if ( EVEN(sts)) return sts;

    print_attr( buf, objid, objectname, parname, tid, size, 
		0, elem, fp);
  }
  else {
    print_attr( object_p, objid, objectname, parname, tid, size, offs,
		elem, fp);
  }
  return 1;
}

int print_object( pwr_tOid oid, pwr_tCid cid, char *object_p, 
		  int offset, char *prefix, FILE *fp)
{
  int		sts;
  unsigned long	elements;
  int		i, j;
  char		objectname[120];
  gdh_sAttrDef 	*bd;
  int 		rows;
  char		idx[20];

  sts = gdh_GetObjectBodyDef( cid, &bd, &rows, oid);
  if ( EVEN(sts)) return sts;

  for ( i = 0; i < rows; i++) {

    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	 bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
      continue;

    elements = 1;
    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY)
      elements = bd[i].attr->Param.Info.Elements;
    else
      elements = 1;

    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS) {
      if ( elements == 1) {
	strcpy( objectname, prefix);
	strcat( objectname, ".");
	strcat( objectname, bd[i].attrName);
	print_object( oid, bd[i].attr->Param.TypeRef, object_p, 
		      offset + bd[i].attr->Param.Info.Offset, objectname, fp);
      }
      else {
	for ( j = 0; j < elements; j++) {
	  strcpy( objectname, prefix);
	  strcat( objectname, ".");
	  strcat( objectname, bd[i].attrName);
	  sprintf( idx, "[%d]", j);
	  strcat( objectname, idx);
	  print_object( oid, bd[i].attr->Param.TypeRef, object_p, 
			offset + bd[i].attr->Param.Info.Offset + 
			j * bd[i].attr->Param.Info.Size / elements, objectname, fp);
	}
      }
    }
    else
      print_attr( object_p, oid, prefix, bd[i].attrName, bd[i].attr->Param.Info.Type, 
		  bd[i].attr->Param.Info.Size,
		  offset + bd[i].attr->Param.Info.Offset, elements, fp);

  }
  return 1;
}

static void error_msg( pwr_tStatus sts, FILE *fp)
{
  if ( fp)
    fprintf( fp, "// Error: %d\n", sts);
  else
    printf( "// Error: %d\n", sts);
}

pwr_tStatus bck_print( char *filename)
{
  char fname[256];
  pwr_sAttrRef aref;
  char objname[120];
  FILE *fp = 0;
  pwr_sAttrRef dataname;
  int sts;

  if ( filename) {
    dcli_translate_filename( fname, filename);
    fp = fopen( fname, "w");
    if ( !fp) return 0;
  }

  sts = gdh_Init("pwr_bck_print");
  if (EVEN(sts)) return sts;

  sts = gdh_GetClassListAttrRef( pwr_cClass_Backup, &aref);
  while ( ODD(sts)) {
    if ( aref.Objid.vid < cdh_cUserVolMin) {
      // In template plc, continue
      sts = gdh_GetNextAttrRef( pwr_cClass_Backup, &aref, &aref);
      continue;
    }


    sts = gdh_AttrrefToName( &aref, objname, sizeof(objname), cdh_mName_volumeStrict); 
    if ( EVEN(sts)) return sts;

    if ( fp)
      fprintf( fp, "// %s\n", objname);
    else
      printf( "// %s\n", objname);

    strcat( objname, ".DataName");
    sts = gdh_GetObjectInfo( objname, &dataname, sizeof(dataname));

    sts = print_data( &dataname, fp);
    if ( EVEN(sts)) error_msg( sts, fp);
    
    sts = gdh_GetNextAttrRef( pwr_cClass_Backup, &aref, &aref);
  }

  if ( fp)
    fclose(fp);	     

  return 1;
}







