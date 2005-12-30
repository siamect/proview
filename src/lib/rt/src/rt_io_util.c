/* 
 * Proview   $Id: rt_io_util.c,v 1.1 2005-12-30 15:45:31 claes Exp $
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

#include <stdlib.h>
#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "co_cdh.h"
#include "rt_gdh_msg.h"
#include "rt_io_msg.h"
#include "rt_io_base.h"
#include "rt_io_util.h"

#define IO_CLASSES_SIZE 	200

/*----------------------------------------------------------------------------*\
  Find classes belonging to an specific IO-type.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_GetIoTypeClasses( 
  io_eType	type,
  pwr_tClassId 	**classes,
  int		*size
)
{
  pwr_tStatus 	sts;
  pwr_sClassDef cdef;
  pwr_tOid	oid;
  int		add_class;
  pwr_sAttrRef  aref;

  *size = 0;
  *classes = calloc( IO_CLASSES_SIZE, sizeof(pwr_tCid));

  for ( sts = gdh_GetClassList( pwr_eClass_ClassDef, &oid);
	ODD(sts);
	sts = gdh_GetNextObject( oid, &oid)) {
    
    aref = cdh_ObjidToAref( oid);
    sts = gdh_GetObjectInfoAttrref( &aref, &cdef, sizeof(cdef));
    if ( EVEN(sts)) return sts;

    add_class = 0;
    switch ( type) {
    case io_eType_Agent:
      if ( cdef.Flags.b.IOAgent)
	add_class = 1;
      break;
    case io_eType_Rack:
      if ( cdef.Flags.b.IORack)
	add_class = 1;
      break;
    case io_eType_Card:
      if ( cdef.Flags.b.IOCard)
	add_class = 1;
      break;
    default:
      return IO__NOMETHOD;
    }

    if ( add_class) {
      if ( *size >= IO_CLASSES_SIZE)
	return IO__CLASSEXCEED;

      (*classes)[ *size] = cdh_ClassObjidToId( oid);
      (*size)++;
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Check if class is an IO-type.
\*----------------------------------------------------------------------------*/
int io_CheckClassIoType( 
  io_eType	type,
  pwr_tCid 	cid
)
{
  pwr_tStatus 	sts;
  pwr_sClassDef cdef;
  pwr_sAttrRef aref;

  aref = cdh_ObjidToAref( cdh_ClassIdToObjid( cid));
  sts = gdh_GetObjectInfoAttrref( &aref, &cdef, sizeof(cdef));
  if ( EVEN(sts)) return 0;

  switch ( type) {
  case io_eType_Agent:
    if ( cdef.Flags.b.IOAgent)
      return 1;
    break;
  case io_eType_Rack:
    if ( cdef.Flags.b.IORack)
      return 1;
    break;
  case io_eType_Card:
    if ( cdef.Flags.b.IOCard)
      return 1;
    break;
  default: ;
  }
  return 0;
}

