/* rt_bck_print.c -- Print backup data

   Proview
   Copyright (C) 2003 by SSAB Oxelosund AB.

   <Description>.  */

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

int print_data( char *dataname, FILE *fp)
{
  int		sts;
  char		*s;
  pwr_tClassId	classid;
  char		*object_p;
  char		objectname[120];
  char		attributename[120];
  pwr_tObjid	objid;
  int 		object_backup;
  int		array_element;
  int		index;
  int		nr;

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

int print_object( pwr_tObjid objid, pwr_tClassId classid, char *object_p, 
		  int offset, char *prefix, FILE *fp)
{
  int		sts;
  pwr_tObjid	attr;
  char		classname[80];
  char		hiername[80];
  char		parname[80];
  char		fullname[80];
  char		*s;
  unsigned long	elements;
  pwr_sParInfo	parinfo;
  pwr_tObjid	body;
  pwr_tClassId	attr_class;
  int		i, j;
  char		objectname[120];

  sts = gdh_ObjidToName( cdh_ClassIdToObjid(classid), classname,
			  sizeof(classname), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;


  for ( j = 0; j < 2; j++) {
    strcpy( hiername, classname);
    if ( j == 0)
      strcat( hiername, "-RtBody");
    else
      strcat( hiername, "-SysBody");

    sts = gdh_NameToObjid( hiername, &body);
    if ( EVEN(sts)) 
      continue;

    /* Get first attribute of the body */
    i = 0;
    sts = gdh_GetChild( body, &attr);
    while ( ODD(sts)) {
      sts = gdh_ObjidToName( attr, hiername, sizeof(hiername),
			     cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      /* Skip hierarchy */
      s = strrchr( hiername, '-');
      if ( s == 0)
	strcpy( parname, hiername);
      else
	strcpy( parname, s + 1);

      /* Get attr info for this attribute */
      strcpy( fullname, hiername);
      sts = gdh_GetObjectInfo( fullname, &parinfo, sizeof(parinfo));
      if (EVEN(sts)) return sts;
      sts = gdh_GetObjectClass( attr, &attr_class);
      if ( EVEN(sts)) return sts;

      if ( parinfo.Flags & PWR_MASK_RTVIRTUAL || 
	   parinfo.Flags & PWR_MASK_PRIVATE) {
	/* This attribute does not contain any useful information, take the
	   next one */
	sts = gdh_GetNextSibling ( attr, &attr);
	i++;
	continue;
      }

      elements = 1;
      if ( parinfo.Flags & PWR_MASK_ARRAY)
 	elements = parinfo.Elements;

      if ( 0 /* parinfo.Flags & PWR_MASK_SUBCLASS */) {
	strcpy( objectname, prefix);
	strcat( objectname, ".");
	strcat( objectname, parname);
	/* classid = ... */
	print_object( objid, classid, object_p, offset + parinfo.Offset, objectname, fp);
      }
      else
        print_attr( object_p, objid, prefix, parname, parinfo.Type, parinfo.Size, 
		  offset + parinfo.Offset, elements, fp);

      sts = gdh_GetNextSibling ( attr, &attr);
      i++;
    }
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
  pwr_tObjid objid;
  char objname[120];
  FILE *fp = 0;
  pwr_tString80 dataname;
  int sts;

  if ( filename) {
    dcli_translate_filename( fname, filename);
    fp = fopen( fname, "w");
    if ( !fp) return 0;
  }

  sts = gdh_Init("pwr_bck_print");
  if (EVEN(sts)) return sts;

  sts = gdh_GetClassList( pwr_cClass_Backup, &objid);
  while ( ODD(sts)) {
    sts = gdh_ObjidToName( objid, objname, sizeof(objname), cdh_mName_volumeStrict); 
    if ( EVEN(sts)) return sts;

    if ( fp)
      fprintf( fp, "// %s\n", objname);
    else
      printf( "// %s\n", objname);

    strcat( objname, ".DataName");
    sts = gdh_GetObjectInfo( objname, dataname, sizeof(dataname));

    sts = print_data( dataname, fp);
    if ( EVEN(sts)) error_msg( sts, fp);
    
    sts = gdh_GetNextObject( objid, &objid);
  }

  if ( fp)
    fclose(fp);	     

  return 1;
}







