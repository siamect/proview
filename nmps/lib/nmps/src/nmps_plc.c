/* 
 * Proview   $Id: nmps_plc.c,v 1.4 2008-04-25 11:27:29 claes Exp $
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

#if defined(OS_VMS) || defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS
#include <stdio.h>
#include <float.h>
#include <string.h>
#endif

#ifdef OS_ELN
#include stdio
#include float
#include string
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_plc.h"
#include "rt_nmps_lock.h"
#include "nmps.h"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define	NMPS_OPTYPE_FRONT	0
#define	NMPS_OPTYPE_BACK	1
#define	NMPS_OPTYPE_UNIT	2
#define	NMPS_OPTYPE_FORWARD_FRONT	3
#define	NMPS_OPTYPE_FORWARD_BACK	4
#define	NMPS_OPTYPE_FORWARD_UNIT	5
#define	NMPS_OPTYPE_REVERSE_FRONT	6
#define	NMPS_OPTYPE_REVERSE_BACK	7
#define	NMPS_OPTYPE_REVERSE_UNIT	8


/* 		NMPS RUTINER			*/

/*      NMpsCell
*/
void NMpsCell_init_time( pwr_sClass_NMpsCell  *object)
{
	pwr_sAttrRef 	attrref;
	plc_t_DataInfo	*data_index;
	int		i, sts;

	/* Get new object pointers at restart av plc program */
	data_index = (plc_t_DataInfo *) &object->Data1P;
	for ( i = 1; i <= object->LastIndex; i++)
	{
	  attrref.Objid = data_index->Data_ObjId;
	  attrref.Offset = 0;
	  attrref.Size = 4;
	  attrref.Flags.b.Indirect = 0;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
			(pwr_tAddress *) &data_index->DataP, &data_index->Data_Dlid);
	  data_index++;
	}
	if ( object->LastIndex)
	{
	  data_index = (plc_t_DataInfo *) &object->Data1P;
	  data_index += object->LastIndex - 1;
	  memcpy( &object->DataLP, data_index, sizeof( *data_index));
	  memcpy( &object->DataLastP, data_index, sizeof( *data_index));
	}
	object->InitTime = 0;
	object->ReloadDone |= NMPS_CELL_INITIALIZED;
}

void NMpsCell_init( pwr_sClass_NMpsCell  *object)
{
	plc_t_DataInfo	*data_index;
	int		i;
	pwr_tStatus	sts;

	nmps_create_lock( &sts);

	if ( !(object->Function & NMPS_CELLFUNC_BACKUP))
	  NMpsCell_init_time( object);
	else
	{
	  /* Backup, reset all nonvalid pointers */
	  data_index = (plc_t_DataInfo *) &object->Data1P;
	  for ( i = 1; i <= object->LastIndex; i++)
	  {
	    data_index->DataP = NULL;
	    data_index++;
	  }
	  if ( object->LastIndex)
	  {
	    object->DataLP = NULL;
	    object->DataLastP = NULL;
	  }
	  object->InitTime = 1;
	}
}

/*_*
  @aref nmpscell NMpsCell
*/

void NMpsCell_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsCell  *object)
{
	int		i, sts;
	plc_t_DataInfo	*data_max;
	plc_t_DataInfo	*data_last;
	plc_t_DataInfo	*data_index;
	plc_t_DataInfo	data_tmp;
	int		size;
	pwr_sAttrRef	extern_attrref;
	gdh_tDlid	data_dlid;
	unsigned long	*data_pointer;
	int		reloaddone;
#if defined OS_LINUX
	plc_t_DataInfo	cpy_buf[NMPS_CELL_SIZE];
#endif

	if ( object->InitTime) {
	  if ( !(object->ReloadDone & NMPS_CELL_RELOADDONE))
	    return;
	  else {
	    nmps_Lock;
	    NMpsCell_init_time( object);
	    object->ReloadDone &= ~NMPS_CELL_RELOADDONE;
	  }
	}
	else
	  nmps_Lock;


	if ( object->FrontNew) object->FrontNew = 0;
	if ( object->RearNew) object->RearNew = 0;
	if ( object->MaxSize > NMPS_CELL_SIZE)
	  object->MaxSize = NMPS_CELL_SIZE;

	if ( object->ResetObjectP)
	{
	  if ( *object->ResetObjectP)
	  {
	    data_index = (plc_t_DataInfo *) &object->Data1P;
	    for ( i = 1; i <= object->LastIndex; i++)
	    {
	      sts = gdh_DLUnrefObjectInfo( data_index->Data_Dlid);
	      if ( object->Function & NMPS_CELLFUNC_RESETDELETE)
	        sts = gdh_DeleteObject( data_index->Data_ObjId);
	      data_index++;
	    }
	    reloaddone = object->ReloadDone;
	    memset( &object->CellFull, 0, sizeof(pwr_sClass_NMpsCell) -
			((unsigned long)(&object->CellFull) -
			(unsigned long)object));
	    object->ReloadDone = reloaddone;
	    if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	  }
	}
	if ( object->ExternFlag)
	{
	  /* Insert new object in Front position */

	  switch( object->ExternOpType)
	  {
	    case NMPS_OPTYPE_EXTINSERT:
	      object->ExternIndex = 1;
	    case NMPS_OPTYPE_EXTINSERT_IDX:
	      /* Direct link to extern objid */
	      if ( object->CellFull)
	        object->ExternStatus = 2; /* Felkod !!! */
	      else if ( object->ExternIndex > object->LastIndex + 1)
	        object->ExternStatus = 2; /* Felkod !!! */
	      else
	      {
	        extern_attrref.Objid = object->ExternObjId;
	        extern_attrref.Offset = 0;
	        extern_attrref.Size = 4;
	        extern_attrref.Flags.b.Indirect = 0;
	        object->ExternStatus = gdh_DLRefObjectInfoAttrref ( 
			&extern_attrref, 
			(pwr_tAddress *) &data_pointer, &data_dlid);
	      }
	      if ( ODD( object->ExternStatus))
	      {
	        object->ExternFlag = 0;
	        data_index = (plc_t_DataInfo *) &object->Data1P;
	        data_index += object->ExternIndex - 1;

	        if ( object->LastIndex >= object->ExternIndex)
	        {
#if defined OS_LINUX
	          memcpy( &cpy_buf, data_index,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
	          memcpy( (char *) data_index + sizeof( *data_max), cpy_buf,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
#else
	          memcpy( (char *) data_index + sizeof( *data_max),
			data_index,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
#endif
	        }
		data_index->DataP = (pwr_tFloat32 *) data_pointer;
	        data_index->Data_ObjId = object->ExternObjId;
	        data_index->Data_Front = 1;
	        data_index->Data_Back = 1;
	        data_index->Data_Dlid = data_dlid;

	        object->LastIndex++;
	        if ( object->LastIndex == object->MaxSize)
	          object->CellFull = 1;
	        data_last = (plc_t_DataInfo *) &object->Data1P;
	        data_last += object->LastIndex - 1;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	        if ( object->ExternOpType == NMPS_OPTYPE_EXTINSERT)
	          object->FrontNew = 1;
	      }
	      else
	        object->ExternFlag = 0;
	      break;
	    case NMPS_OPTYPE_EXTDELETE_OBJID:
	    case NMPS_OPTYPE_EXTREMOVE_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	    case NMPS_OPTYPE_EXTDELETE:
	      if ( object->ExternOpType == NMPS_OPTYPE_EXTDELETE)
	        object->ExternIndex = 1;
	    case NMPS_OPTYPE_EXTDELETE_IDX:
	      if ( object->ExternIndex > object->LastIndex)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      object->ExternFlag = 0;
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      sts = gdh_DLUnrefObjectInfo( data_index->Data_Dlid);
	      if ( object->Function & NMPS_CELLFUNC_DELETE &&
	           object->ExternOpType != NMPS_OPTYPE_EXTREMOVE_OBJID)
	        sts = gdh_DeleteObject( data_index->Data_ObjId);

	      if ( object->ExternIndex < object->LastIndex)
	      {
	        size = (object->LastIndex - object->ExternIndex)
			* sizeof ( *data_max);
#if defined OS_LINUX
	        memcpy( &cpy_buf, (char *) data_index + sizeof( *data_max), size);
	        memcpy( data_index, &cpy_buf, size);
#else
	        memcpy( data_index, (char *) data_index + sizeof( *data_max),
			 size);
#endif
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->CellFull = 0;
	      object->LastIndex--;
	      object->ExternFlag = 0;
	      if ( object->LastIndex > 0 )
	      {
	        data_last--;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	      }
	      else
	      {
	        memset( &object->DataLP, 0, sizeof( *data_last));
	        memset( &object->DataLastP, 0, sizeof( *data_last));
	      }
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	    case NMPS_OPTYPE_EXTMOVEFORW_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      if ( object->ExternIndex >= object->LastIndex)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      if ( !(data_index->Data_Front && data_index->Data_Back))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      memcpy( &data_tmp, data_index+1, sizeof( *data_index));
	      memcpy( data_index+1, data_index, sizeof( *data_index));
	      memcpy( data_index, &data_tmp, sizeof( *data_index));
	      object->ExternFlag = 0;

	      if ( object->ExternIndex == object->LastIndex -1)
	      {
	        data_last = (plc_t_DataInfo *) &object->Data1P;
	        data_last += object->LastIndex - 1;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	      }
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	    case NMPS_OPTYPE_EXTMOVEBACKW_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      if ( object->ExternIndex <= 1)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      if ( !(data_index->Data_Front && data_index->Data_Back))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      memcpy( &data_tmp, data_index-1, sizeof( *data_index));
	      memcpy( data_index-1, data_index, sizeof( *data_index));
	      memcpy( data_index, &data_tmp, sizeof( *data_index));
	      object->ExternFlag = 0;

	      if ( object->ExternIndex == object->LastIndex)
	      {
	        data_last = (plc_t_DataInfo *) &object->Data1P;
	        data_last += object->LastIndex - 1;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	      }
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	if ( object->InFlag)
	{
	  /* Insert new object in Front position */

	  switch( object->InOpType)
	  {
	    case NMPS_OPTYPE_FORWARD_FRONT:
	      /* Insert at front of object front of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      if ( object->LastIndex > 0)
	      {
#if defined OS_LINUX
	        memcpy( &cpy_buf, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
	        memcpy( &object->Data2P, &cpy_buf,
	          object->LastIndex * sizeof ( *data_max));
#else
	        memcpy( &object->Data2P, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
#endif
	      }
	      object->Data1P = object->InPointer;
	      object->Data1_ObjId = object->InObjId;
	      object->Data1_Front = 1;
	      object->Data1_Back = 0;
	      object->InFlag = 0;
	      object->Data1_Dlid = object->InDlid;
	      object->FrontNew = 1;
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memcpy( &object->DataLP, data_last, sizeof( *data_last));
	      memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      break;
	    case NMPS_OPTYPE_FORWARD_BACK:
	      if ( memcmp( &object->Data1_ObjId, &object->InObjId, 
			sizeof(pwr_tObjid)))
	      {
	        /* This it not the correct object */
	        object->InFlag = 0;
		nmps_Unlock;
	        return;
	      }
	      object->Data1_Back = 1;
	      if ( object->LastIndex == 1)
              {
	        object->DataL_Back = 1;
	        object->DataLast_Back = 1;
	      }
	      object->InFlag = 0;
	      break;
	    case NMPS_OPTYPE_FORWARD_UNIT:
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      if ( object->LastIndex > 0)
	      {
#if defined OS_LINUX
	        memcpy( &cpy_buf, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
	        memcpy( &object->Data2P, &cpy_buf,
	          object->LastIndex * sizeof ( *data_max));
#else
	        memcpy( &object->Data2P, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
#endif
	      }
	      object->Data1P = object->InPointer;
	      object->Data1_ObjId = object->InObjId;
	      object->Data1_Front = 1;
	      object->Data1_Back = 1;
	      object->Data1_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->FrontNew = 1;
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex;
	      memcpy( &object->DataLP, data_last, sizeof( *data_last));
	      memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      break;
	    case NMPS_OPTYPE_REVERSE_BACK:
	      /* Insert at rear of object back of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex;
	      data_last->DataP = object->InPointer;
	      data_last->Data_ObjId = object->InObjId;
	      data_last->Data_Front = 0;
	      data_last->Data_Back = 1;
	      data_last->Data_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->RearNew = 1;
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      memcpy( &object->DataLP, data_last, sizeof( *data_last));
	      memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      break;
	    case NMPS_OPTYPE_REVERSE_UNIT:
	      /* Insert at rear of object all of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex;
	      data_last->DataP = object->InPointer;
	      data_last->Data_ObjId = object->InObjId;
	      data_last->Data_Front = 1;
	      data_last->Data_Back = 1;
	      data_last->Data_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->LastIndex++;
	      object->RearNew = 1;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      memcpy( &object->DataLP, data_last, sizeof( *data_last));
	      memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      break;
	    case NMPS_OPTYPE_REVERSE_FRONT:
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      if ( memcmp( &data_last->Data_ObjId, &object->InObjId,
			sizeof(pwr_tObjid)))
	      {
	        /* This it not the correct object */
	        object->InFlag = 0;
		nmps_Unlock;
	        return;
	      }
	      data_last->Data_Front = 1;
	      object->DataL_Front = 1;
	      object->DataLast_Front = 1;
	      object->InFlag = 0;
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	if ( object->OutRearFlag)
	{
	  /* Remove an object in Rear position */

	  switch( object->OutRearOpType)
	  {
	    case NMPS_OPTYPE_BACK:
	    case NMPS_OPTYPE_UNIT:

	      /* Get the last data */
	      if ( object->LastIndex == 0)
	      {
	        object->OutRearFlag = 0;
		nmps_Unlock;
		return;
	      }

	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->OutRearFlag = 0;
	      object->CellFull = 0;
	      object->LastIndex--;
	      if ( object->LastIndex != 0)
	      {
	        data_last--;
	        memcpy( &object->DataLP, data_last, sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      }
	      else
	      {
	        memset( &object->DataLP, 0, sizeof( *data_last));
	        memset( &object->DataLastP, 0, sizeof( *data_last));
	      }
	      break;
	    case NMPS_OPTYPE_FRONT:
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      data_last->Data_Front = 0;
	      object->OutRearFlag = 0;
	      object->DataL_Front = 0;  /* Korrigeras !!! */
	      object->DataLast_Front = 0;  /* Korrigeras !!! */
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	if ( object->OutFrontFlag)
	{
	  /* Remove an object in Front position */

	  switch( object->OutFrontOpType)
	  {
	    case NMPS_OPTYPE_FRONT:
	    case NMPS_OPTYPE_UNIT:

	      /* Get the last data */
	      if ( object->LastIndex == 0)
	      {
	        object->OutFrontFlag = 0;
		nmps_Unlock;
		return;
	      }
#if defined OS_LINUX
	      memcpy( &cpy_buf, &object->Data2P,
		(object->LastIndex - 1) * sizeof( *data_last)); 
	      memcpy( &object->Data1P, &cpy_buf,
		(object->LastIndex - 1) * sizeof( *data_last)); 
#else
	      memcpy( &object->Data1P, &object->Data2P,
		(object->LastIndex - 1) * sizeof( *data_last)); 
#endif
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->CellFull = 0;
	      object->LastIndex--;
	      object->OutFrontFlag = 0;
	      if ( object->LastIndex != 0)
	      {
	        data_last--;
	        memcpy( &object->DataLP, data_last, sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last, sizeof( *data_last));
	      }
	      else
	      {
	        memset( &object->DataLP, 0, sizeof( *data_last));
	        memset( &object->DataLastP, 0, sizeof( *data_last));
	      }
	      break;
	    case NMPS_OPTYPE_BACK:
	      object->Data1_Back = 0;
	      object->OutFrontFlag = 0;
	      if ( object->LastIndex == 1)
	      {
	        object->DataL_Back = 0;
	        object->DataLast_Back = 0;
	      }
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	object->NumberOfData = object->LastIndex;

	nmps_Unlock;
}


/*      NMpsStoreCell
*/
void NMpsStoreCell_init( pwr_sClass_NMpsStoreCell  *object)
{
	plc_t_DataInfo	*data_index;
	int		i;
	pwr_tStatus 	sts;

	nmps_create_lock( &sts);

	if ( !(object->Function & NMPS_CELLFUNC_BACKUP))
	  NMpsCell_init_time( (pwr_sClass_NMpsCell *) object);
	else
	{
	  /* Backup, reset all nonvalid pointers */
	  data_index = (plc_t_DataInfo *) &object->Data1P;
	  for ( i = 1; i <= object->LastIndex; i++)
	  {
	    data_index->DataP = NULL;
	    data_index++;
	  }
	  if ( object->LastIndex)
	  {
	    object->DataLP = NULL;
	    object->DataLastP = NULL;
	  }
	  object->InitTime = 1;
	}
}

/*_*
  NMpsStoreCell

  @aref nmpsstorecell NMpsStoreCell
*/
void NMpsStoreCell_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsStoreCell  *object)
{
	int		i, sts;
	plc_t_DataInfo	*data_max;
	plc_t_DataInfo	*data_last;
	plc_t_DataInfo	*data_index;
	plc_t_DataInfo	data_tmp;
	int		size;
	pwr_sAttrRef	extern_attrref;
	gdh_tDlid	data_dlid;
	unsigned long	*data_pointer;
	int		reloaddone;
#if defined OS_LINUX
	plc_t_DataInfo	cpy_buf[NMPS_CELL_SIZE];
#endif

	if ( object->InitTime)
	{
	  if ( !(object->ReloadDone & NMPS_CELL_RELOADDONE))
	    return;
	  else
	  {
	    nmps_Lock;
	    NMpsCell_init_time( (pwr_sClass_NMpsCell *)object);
	    object->ReloadDone &= ~NMPS_CELL_RELOADDONE;
	  }
	}
	else
	  nmps_Lock;

	if ( object->FrontNew) object->FrontNew = 0;
	if ( object->RearNew) object->RearNew = 0;
	if ( object->MaxSize > NMPS_CELL_SIZE)
	  object->MaxSize = NMPS_CELL_SIZE;

	if ( object->ResetObjectP)
	{
	  if ( *object->ResetObjectP)
	  {
	    data_index = (plc_t_DataInfo *) &object->Data1P;
	    for ( i = 1; i <= object->LastIndex; i++)
	    {
	      sts = gdh_DLUnrefObjectInfo( data_index->Data_Dlid);
	      if ( object->Function & NMPS_CELLFUNC_RESETDELETE)
	        sts = gdh_DeleteObject( data_index->Data_ObjId);
	      data_index++;
	    }
	    reloaddone = object->ReloadDone;
	    memset( &object->CellFull, 0, sizeof(pwr_sClass_NMpsStoreCell) -
			((unsigned long)(&object->CellFull) -
			(unsigned long)object));
	    object->ReloadDone = reloaddone;
	    if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	  }
	}
	if ( object->ExternFlag)
	{
	  /* Insert new object in Front position */

	  switch( object->ExternOpType)
	  {
	    case NMPS_OPTYPE_EXTINSERT:
	    case NMPS_OPTYPE_EXTINSERTSELECT:
	      object->ExternIndex = 1;
	    case NMPS_OPTYPE_EXTINSERT_IDX:
	      /* Direct link to extern objid */
	      if ( object->CellFull)
	        object->ExternStatus = 2; /* Felkod !!! */
	      else if ( object->ExternIndex > object->LastIndex + 1)
	        object->ExternStatus = 2; /* Felkod !!! */
	      else
	      {
	        extern_attrref.Objid = object->ExternObjId;
	        extern_attrref.Offset = 0;
	        extern_attrref.Size = 4;
	        extern_attrref.Flags.b.Indirect = 0;
	        object->ExternStatus = gdh_DLRefObjectInfoAttrref ( &extern_attrref, 
			(pwr_tAddress *) &data_pointer, &data_dlid);
	      }
	      if ( ODD( object->ExternStatus))
	      {
	        object->ExternFlag = 0;
	        data_index = (plc_t_DataInfo *) &object->Data1P;
	        data_index += object->ExternIndex - 1;

	        if ( object->LastIndex >= object->ExternIndex)
	        {
#if defined OS_LINUX
	          memcpy( &cpy_buf, data_index,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
	          memcpy( (char *) data_index + sizeof( *data_max), cpy_buf,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
#else
	          memcpy( (char *) data_index + sizeof( *data_max),
			data_index,
	          	(object->LastIndex - object->ExternIndex + 1)
			* sizeof ( *data_max));
#endif
	        }
		data_index->DataP = (pwr_tFloat32 *) data_pointer;
	        data_index->Data_ObjId = object->ExternObjId;
	        data_index->Data_Front = 1;
	        data_index->Data_Back = 1;
	        data_index->Data_Dlid = data_dlid;
		if ( object->ExternOpType == NMPS_OPTYPE_EXTINSERTSELECT)
		{
		  data_index->Data_Select = 1;
	  	}
	        object->LastIndex++;
	        if ( object->LastIndex == object->MaxSize)
	          object->CellFull = 1;
	        if (( object->ExternOpType == NMPS_OPTYPE_EXTINSERT) ||
		( object->ExternOpType == NMPS_OPTYPE_EXTINSERTSELECT))
	          object->FrontNew = 1;
	        if (( object->SelectIndex != 0) &&
	            ( object->SelectIndex >= object->ExternIndex))
		  object->SelectIndex++;
	      }
	      else
	        object->ExternFlag = 0;
	      break;
	    case NMPS_OPTYPE_EXTSELECT_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      for ( i = 1; i <= object->LastIndex; i++)
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
				sizeof(pwr_tObjid)))
	        {
	          data_index->Data_Select = 1;
	          break;
	        }
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId, 
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	      }
	      object->ExternFlag = 0;
	      break;
	    case NMPS_OPTYPE_EXTUNSELECT_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      for ( i = 1; i <= object->LastIndex; i++)
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
				sizeof(pwr_tObjid)))
	        {
		  if ( data_index->Data_Select)
	            object->DataSelected--;
	          data_index->Data_Select = 0;
		  if ( i == object->SelectIndex)
	          {
	            if ( object->DataSelected > 0)
	            {
	              data_last = (plc_t_DataInfo *) &object->Data1P;
	              data_last += object->LastIndex - 1;
	              for ( i = object->LastIndex; i > 0; i--)
	              {
	                if ( data_last->Data_Select)
	                { 
	                  memcpy( &object->DataLP, data_last,
				sizeof( *data_last));
	                  memcpy( &object->DataLastP, data_last,
				sizeof( *data_last));
	                  object->SelectIndex = i;		
	                  break;
	                }
	                data_last--;
	              }
	            }
	            else
	            {
	              memset( &object->DataLP, 0, sizeof( *data_last));
	              memset( &object->DataLastP, 0, sizeof( *data_last));
	              object->SelectIndex = 0;	          
	            }
	          }
	          break;
	        }
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	      }
	      object->ExternFlag = 0;
	      break;
	    case NMPS_OPTYPE_EXTDELETE_OBJID:
	    case NMPS_OPTYPE_EXTREMOVE_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
				sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
				sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	    case NMPS_OPTYPE_EXTDELETE:
	      if ( object->ExternOpType == NMPS_OPTYPE_EXTDELETE)
	        object->ExternIndex = 1;
	    case NMPS_OPTYPE_EXTDELETE_IDX:
	      if ( object->ExternIndex > object->LastIndex)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      object->ExternFlag = 0;
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      sts = gdh_DLUnrefObjectInfo( data_index->Data_Dlid);
	      if ( object->Function & NMPS_CELLFUNC_DELETE &&
	           object->ExternOpType != NMPS_OPTYPE_EXTREMOVE_OBJID)
	        sts = gdh_DeleteObject( data_index->Data_ObjId);

	      /* Look for new last selected if data is selected */
	      if ( data_index->Data_Select)
	      {
	        object->DataSelected--;
	        if ( object->SelectIndex == object->ExternIndex)
	        {
	          if ( object->DataSelected > 0)
	          {
	            data_index = (plc_t_DataInfo *) &object->Data1P;
	            data_index += object->LastIndex - 1;
	            for ( i = object->LastIndex; i > 0; i--)
	            {
	              if ( data_index->Data_Select && i != object->ExternIndex)
	              { 
	                memcpy( &object->DataLP, data_index,
				sizeof( *data_index));
	                memcpy( &object->DataLastP, data_index,
				sizeof( *data_index));
	                object->SelectIndex = i;		
	                break;
	              }
	              data_index--;
	            }
	          }
	          else
	          {
	            memset( &object->DataLP, 0, sizeof( *data_last));
	            memset( &object->DataLastP, 0, sizeof( *data_last));
	            object->SelectIndex = 0;	          
	          }
	        }
	      }

	      if ( object->ExternIndex < object->LastIndex)
	      {
	        size = (object->LastIndex - object->ExternIndex)
			* sizeof ( *data_max);
#if defined OS_LINUX
	        memcpy( &cpy_buf, (char *) data_index + sizeof( *data_max), size);
	        memcpy( data_index, &cpy_buf, size);
#else
	        memcpy( data_index, (char *) data_index + sizeof( *data_max),
			 size);
#endif
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->CellFull = 0;
	      object->LastIndex--;
	      object->ExternFlag = 0;
	      data_last--;
	      if (( object->SelectIndex != 0) &&
	               ( object->SelectIndex > object->ExternIndex))
		object->SelectIndex--;
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	    case NMPS_OPTYPE_EXTMOVEFORW_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      if ( object->ExternIndex >= object->LastIndex)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      if ( !(data_index->Data_Front && data_index->Data_Back))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      memcpy( &data_tmp, data_index+1, sizeof( *data_index));
	      memcpy( data_index+1, data_index, sizeof( *data_index));
	      memcpy( data_index, &data_tmp, sizeof( *data_index));

	      if ( object->SelectIndex == object->ExternIndex)
	        object->SelectIndex++;
	      else if ( object->SelectIndex == object->ExternIndex + 1)
	        object->SelectIndex--;

	      object->ExternFlag = 0;

	      if ( (object->DataSelected == 0) && (object->ExternIndex == object->LastIndex -1))
	      {
	        data_last = (plc_t_DataInfo *) &object->Data1P;
	        data_last += object->LastIndex - 1;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	      }
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	    case NMPS_OPTYPE_EXTMOVEBACKW_OBJID:
	      data_index = (plc_t_DataInfo *) &object->Data1P;
   	      for ( object->ExternIndex = 1; 
   		    object->ExternIndex <= object->LastIndex; 
   		    object->ExternIndex++) 
	      {
	        if ( !memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	          break;
	        data_index++;
	      }
	      if ( memcmp( &data_index->Data_ObjId, &object->ExternObjId,
			sizeof(pwr_tObjid)))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      if ( object->ExternIndex <= 1)
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->ExternIndex - 1;
	      if ( !(data_index->Data_Front && data_index->Data_Back))
	      {
	        object->ExternStatus = 2; /* Felkod !!! */
	        object->ExternFlag = 0;
		break;
	      }
	      memcpy( &data_tmp, data_index-1, sizeof( *data_index));
	      memcpy( data_index-1, data_index, sizeof( *data_index));
	      memcpy( data_index, &data_tmp, sizeof( *data_index));

	      if ( object->SelectIndex == object->ExternIndex)
	        object->SelectIndex--;
	      else if ( object->SelectIndex == object->ExternIndex - 1)
	        object->SelectIndex++;

	      object->ExternFlag = 0;

	      if ( (object->DataSelected == 0) && (object->ExternIndex == object->LastIndex))
	      {
	        data_last = (plc_t_DataInfo *) &object->Data1P;
	        data_last += object->LastIndex - 1;
	        memcpy( &object->DataLP, data_last,
			sizeof( *data_last));
	        memcpy( &object->DataLastP, data_last,
			sizeof( *data_last));
	      }
	      object->ExternStatus = 1; /* Felkod Success!!! */
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	/* Count and find new selected objects */
	object->DataSelected = 0;
	data_index = (plc_t_DataInfo *) &object->Data1P;
	for ( i = 1; i <= object->LastIndex; i++)
	{
	  if ( data_index->Data_Select && !data_index->Data_OldSelect)
	  {
	    memcpy( &object->DataLP, data_index,
			sizeof( *data_index));
	    memcpy( &object->DataLastP, data_index,
	 		sizeof( *data_index));
	    object->SelectIndex = i;
	    object->RearNew = 1;
	    if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	  }
	  if ( data_index->Data_Select )
	    object->DataSelected++;
	  data_index->Data_OldSelect = data_index->Data_Select;
	  data_index++;
	}
	if (( object->Function & NMPS_CELLFUNC_SINGELSELECT) ||
		( object->Function & NMPS_CELLFUNC_ONESELECT))
	{
	  /* Unselect everone exept last selected */
	  if ( object->DataSelected > 1)
	  {
	    data_index = (plc_t_DataInfo *) &object->Data1P;
	    for ( i = 1; i <= object->LastIndex; i++)
	    {
	      if ( i != object->SelectIndex)
	        data_index->Data_Select = 0;
	      data_index++;
	    }
	    object->DataSelected = 1;
	    if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	  }
	}
	if ( object->Function & NMPS_CELLFUNC_ONESELECT)
	{
	  if (( object->DataSelected == 0) && ( object->LastIndex != 0))
	  {
	    data_index = (plc_t_DataInfo *) &object->Data1P;
	    data_index += object->LastIndex - 1;
	    data_index->Data_Select = 1;
	    data_index->Data_OldSelect = 1;
	    memcpy( &object->DataLP, data_index, sizeof( *data_last));
	    memcpy( &object->DataLastP, data_index, sizeof( *data_last));
	    object->SelectIndex = object->LastIndex;
	    object->DataSelected = 1;
	    if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	  }
	}
	if ( object->DataSelected == 0)
	{
	  memset( &object->DataLP, 0, sizeof( *data_last));
	  memset( &object->DataLastP, 0, sizeof( *data_last));
	  object->SelectIndex = 0;
	}

	if ( object->InFlag)
	{
	  /* Insert new object in Front position */

	  switch( object->InOpType)
	  {
	    case NMPS_OPTYPE_FORWARD_FRONT:
	      /* Insert at front of object front of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      if ( object->LastIndex > 0)
	      {
#if defined OS_LINUX
	        memcpy( &cpy_buf, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
	        memcpy( &object->Data2P, &cpy_buf,
	          object->LastIndex * sizeof ( *data_max));
#else
	        memcpy( &object->Data2P, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
#endif
	      }
	      object->Data1P = object->InPointer;
	      object->Data1_ObjId = object->InObjId;
	      object->Data1_Front = 1;
	      object->Data1_Back = 0;
	      object->Data1_Select = 0;
	      object->InFlag = 0;
	      object->Data1_Dlid = object->InDlid;
	      object->FrontNew = 1;
	      object->LastIndex++;
	      if ( object->DataSelected > 0)
	        object->SelectIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      break;
	    case NMPS_OPTYPE_FORWARD_BACK:
	      if ( memcmp( &object->Data1_ObjId, &object->InObjId,
			sizeof(pwr_tObjid)))
	      {
	        /* This it not the correct object */
	        object->InFlag = 0;
		nmps_Unlock;
	        return;
	      }
	      object->Data1_Back = 1;
	      if ( object->SelectIndex == 1)
	      {
	        object->DataL_Back = 1;
	        object->DataLast_Back = 1;
	      }
	      object->InFlag = 0;
	      break;
	    case NMPS_OPTYPE_FORWARD_UNIT:
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      if ( object->LastIndex > 0)
	      {
#if defined OS_LINUX
	        memcpy( &cpy_buf, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
	        memcpy( &object->Data2P, &cpy_buf,
	          object->LastIndex * sizeof ( *data_max));
#else
	        memcpy( &object->Data2P, &object->Data1P,
	          object->LastIndex * sizeof ( *data_max));
#endif
	      }
	      object->Data1P = object->InPointer;
	      object->Data1_ObjId = object->InObjId;
	      object->Data1_Front = 1;
	      object->Data1_Back = 1;
	      object->Data1_Select = 0;
	      object->Data1_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->FrontNew = 1;
	      if ( object->DataSelected > 0)
	        object->SelectIndex++;
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      break;
	    case NMPS_OPTYPE_REVERSE_BACK:
	      /* Insert at rear of object back of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex;
	      data_last->DataP = object->InPointer;
	      data_last->Data_ObjId = object->InObjId;
	      data_last->Data_Front = 0;
	      data_last->Data_Back = 1;
	      data_last->Data_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      break;
	    case NMPS_OPTYPE_REVERSE_UNIT:
	      /* Insert at rear of object all of data */
	      if ( object->CellFull) {
		nmps_Unlock;
		return;
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex;
	      data_last->DataP = object->InPointer;
	      data_last->Data_ObjId = object->InObjId;
	      data_last->Data_Front = 1;
	      data_last->Data_Back = 1;
	      data_last->Data_Dlid = object->InDlid;
	      object->InFlag = 0;
	      object->LastIndex++;
	      if ( object->LastIndex == object->MaxSize)
	        object->CellFull = 1;
	      break;
	    case NMPS_OPTYPE_REVERSE_FRONT:
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      if ( memcmp( &data_last->Data_ObjId, &object->InObjId,
			sizeof(pwr_tObjid)))
	      {
	        /* This it not the correct object */
	        object->InFlag = 0;
		nmps_Unlock;
	        return;
	      }
	      data_last->Data_Front = 1;
	      object->InFlag = 0;
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	if ( object->OutRearFlag)
	{
	  /* Remove an object in Rear position */

	  switch( object->OutRearOpType)
	  {
	    case NMPS_OPTYPE_UNIT:

	      /* Get the last data */
	      if ( object->LastIndex == 0)
	      {
	        object->OutRearFlag = 0;
		nmps_Unlock;
		return;
	      }

	      data_index = (plc_t_DataInfo *) &object->Data1P;
	      data_index += object->SelectIndex - 1;

	      if ( object->SelectIndex < object->LastIndex)
	      {
	        size = (object->LastIndex - object->SelectIndex)
			* sizeof ( *data_max);
#if defined OS_LINUX
	        memcpy( &cpy_buf, (char *) data_index + sizeof( *data_max), size);
	        memcpy( data_index, &cpy_buf, size);
#else
	        memcpy( data_index, (char *) data_index + sizeof( *data_max),
			 size);
#endif
	      }
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->CellFull = 0;
	      object->LastIndex--;
	      object->DataSelected--;
	      /* Find next selected, search backwards */
	      if ( object->DataSelected > 0)
	      {
	        data_index = (plc_t_DataInfo *) &object->Data1P;
	        data_index += object->LastIndex - 1;
	        for ( i = object->LastIndex; i > 0; i--)
	        {
	          if ( data_index->Data_Select)
	          { 
	            memcpy( &object->DataLP, data_index,
			sizeof( *data_index));
	            memcpy( &object->DataLastP, data_index,
			sizeof( *data_index));
	            object->SelectIndex = i;		
	            break;
	          }
	          data_index--;
	        }
	      }
	      else
	      {
	        memset( &object->DataLP, 0, sizeof( *data_last));
	        memset( &object->DataLastP, 0, sizeof( *data_last));
	        object->SelectIndex = 0;	          
	      }
	      object->OutRearFlag = 0;
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	if ( object->OutFrontFlag)
	{
	  /* Remove an object in Front position */

	  switch( object->OutFrontOpType)
	  {
	    case NMPS_OPTYPE_FRONT:
	    case NMPS_OPTYPE_UNIT:

	      /* Get the last data */
	      if ( object->LastIndex == 0)
	      {
	        object->OutFrontFlag = 0;
		nmps_Unlock;
		return;
	      }

	      if ( object->Data1_Select)
	      {
	        object->DataSelected--;
	        if ( object->SelectIndex == 1)
	        {
	          if ( object->DataSelected > 0)
	          {
	            data_index = (plc_t_DataInfo *) &object->Data1P;
	            data_index += object->LastIndex - 1;
	            for ( i = object->LastIndex; i > 1; i--)
	            {
	              if ( data_index->Data_Select)
	              { 
	                memcpy( &object->DataLP, data_index,
				sizeof( *data_index));
	                memcpy( &object->DataLastP, data_index,
				sizeof( *data_index));
	                object->SelectIndex = i;		
	                break;
	              }
	              data_index--;
	            }
	          }
	          else
	          {
	            memset( &object->DataLP, 0, sizeof( *data_last));
	            memset( &object->DataLastP, 0, sizeof( *data_last));
	            object->SelectIndex = 0;	          
	          }
	        }
	      }
	      if ( object->SelectIndex > 0)
	        object->SelectIndex--;

#if defined OS_LINUX
	      memcpy( &cpy_buf, &object->Data2P, 
		(object->LastIndex - 1) * sizeof( *data_last)); 
	      memcpy( &object->Data1P, &cpy_buf, 
		(object->LastIndex - 1) * sizeof( *data_last)); 
#else
	      memcpy( &object->Data1P, &object->Data2P, 
		(object->LastIndex - 1) * sizeof( *data_last)); 
#endif
	      data_last = (plc_t_DataInfo *) &object->Data1P;
	      data_last += object->LastIndex - 1;
	      memset( data_last, 0, sizeof( *data_last));
	      object->CellFull = 0;
	      object->LastIndex--;
	      object->OutFrontFlag = 0;
	      break;
	    case NMPS_OPTYPE_BACK:
	      object->Data1_Back = 0;
	      object->OutFrontFlag = 0;
	      if ( object->LastIndex == object->SelectIndex)
	      {
	        object->DataL_Back = 0;
	        object->DataLast_Back = 0;
	      }
	      break;
	  }
	  if (object->Function & NMPS_CELLFUNC_BACKUP) object->BackupNow = 1;
	}
	object->NumberOfData = object->DataSelected;

	nmps_Unlock;
}

/*_*
  NMpsOutCell

  @aref nmpsoutcell NMpsOutCell
*/
void NMpsOutCell_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsOutCell  *object)
{
	int		sts;

	if ( object->FrontNew) object->FrontNew = 0;

	if ( object->InFlag)
	{
	  switch( object->InOpType)
	  {
	    case NMPS_OPTYPE_FORWARD_UNIT:
	      sts = gdh_DLUnrefObjectInfo ( object->InDlid);
	      if ( object->Function & NMPS_CELLFUNC_DELETE )
	        sts = gdh_DeleteObject( object->InObjId);
	      object->InFlag = 0;
	      object->FrontNew = 1;
	      break;
	  }
	}
}


/*_*
  NMpsTrp

  @aref nmpstrp NMpsTrp
*/
void NMpsTrp_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsTrp  *object)
{
	if (object->Status)
	  object->Status = 0;

	if ( ( *(object->TriggForwFrontP) && !object->OldTriggForwFront) ||
             ( *(object->TriggForwFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	       object->FuncAllForward )
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	    /* Check that both Back and Front is in the in cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->InP))->DataL_Back &&
	            ((pwr_sClass_NMpsCell *)(object->InP))->DataL_Front))
	    {
	      object->Status = 1;
	      goto trp_return;
	    }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
	  	 NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType =
		NMPS_OPTYPE_UNIT;
	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( *(object->TriggForwFrontP) && 
			!object->OldTriggForwFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }

	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData != 0)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trp_return;
	        }
	      }
	    }

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllForward = 0;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trp_return;
            }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType =
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 1)
	      object->FuncAllForward = 0;
	    else
	      object->FuncAllForward = 1;
	  }
	  else
	  {
	    /* Move front of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trp_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_FORWARD_FRONT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType = NMPS_OPTYPE_FRONT;
	  }
	}

	if ( *(object->TriggForwBackP) && !object->OldTriggForwBack)
	{
	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trp_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trp_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType = 
		NMPS_OPTYPE_FORWARD_BACK;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType = NMPS_OPTYPE_BACK;
	}

	if ( *(object->TriggReverseBackP) && !object->OldTriggReverseBack)
	{
	  /* Check destination */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	  {
	    /* Destination cell is full */
	    object->Status = 1;
	    goto trp_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trp_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trp_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_REVERSE_BACK;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_BACK;
	}

	if ( ( *(object->TriggReverseFrontP) && !object->OldTriggReverseFront) ||
             ( *(object->TriggReverseFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	     object->FuncAllReverse)
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    /* Check that both Front and Back is in the cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Back &&
	            ((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Front))
	    {
	      object->Status = 1;
	      goto trp_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_UNIT;

	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    if ( *(object->TriggReverseFrontP) && 
			!object->OldTriggReverseFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }	        

	      /* Check destination */
	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trp_return;
	        }
	      }
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllReverse = 0;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      object->Status = 1;
	      goto trp_return;
	    }

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trp_return;
            }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 1)
	      object->FuncAllReverse = 0;
	    else
	      object->FuncAllReverse = 1;
	  }
	  else
	  {
	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trp_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trp_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_REVERSE_FRONT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType =
		NMPS_OPTYPE_FRONT;
	  }
	}

trp_return:
	object->OldTriggForwFront = *(object->TriggForwFrontP);
	object->OldTriggReverseFront = *(object->TriggReverseFrontP);
	object->OldTriggForwBack = *(object->TriggForwBackP);
	object->OldTriggReverseBack = *(object->TriggReverseBackP);

	if ( object->Function & NMPS_TRPFUNC_TRIGGRESET)
	{
	  /* Reset the trigg flags */
	  object->TriggForwFront = 0;
	  object->TriggReverseFront = 0;
	  object->TriggForwBack = 0;
	  object->TriggReverseBack = 0;
	}
}

/*_*
  NMpsTrpRR

  @aref nmpstrprr NMpsTrpRR
*/
void NMpsTrpRR_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsTrpRR  *object)
{
	if (object->Status)
	  object->Status = 0;

	if ( ( *(object->TriggForwFrontP) && !object->OldTriggForwFront) ||
             ( *(object->TriggForwFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	       object->FuncAllForward )
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	    /* Check that both Back and Front is in the in cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->InP))->DataL_Back &&
	            ((pwr_sClass_NMpsCell *)(object->InP))->DataL_Front))
	    {
	      object->Status = 1;
	      goto trprr_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
	  	 NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType =
		NMPS_OPTYPE_UNIT;
	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( *(object->TriggForwFrontP) && 
			!object->OldTriggForwFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }

	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData != 0)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trprr_return;
	        }
	      }
	    }

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllForward = 0;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trprr_return;
            }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType =
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 1)
	      object->FuncAllForward = 0;
	    else
	      object->FuncAllForward = 1;
	  }
	  else
	  {
	    /* Move front of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->DataLP )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trprr_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_REVERSE_BACK;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType = NMPS_OPTYPE_FRONT;
	  }
	}

	if ( *(object->TriggForwBackP) && !object->OldTriggForwBack)
	{
	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trprr_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->DataLP )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trprr_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->DataLP;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->DataL_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType = 
		NMPS_OPTYPE_REVERSE_FRONT;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutRearOpType = NMPS_OPTYPE_BACK;
	}

	if ( *(object->TriggReverseBackP) && !object->OldTriggReverseBack)
	{
	  /* Check destination */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	  {
	    /* Destination cell is full */
	    object->Status = 1;
	    goto trprr_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trprr_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->DataLP )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trprr_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataLP;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_REVERSE_BACK;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearOpType = 
		NMPS_OPTYPE_FRONT;
	}

	if ( ( *(object->TriggReverseFrontP) && !object->OldTriggReverseFront) ||
             ( *(object->TriggReverseFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	     object->FuncAllReverse)
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag ) return;

	    /* Check that both Front and Back is in the cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Back &&
	            ((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Front))
	    {
	      object->Status = 1;
	      goto trprr_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearOpType = 
		NMPS_OPTYPE_UNIT;

	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    if ( *(object->TriggReverseFrontP) && 
			!object->OldTriggReverseFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }	        

	      /* Check destination */
	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->InP))->NumberOfData)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trprr_return;
	        }
	      }
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllReverse = 0;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->DataLP )
	    {
	      object->Status = 1;
	      goto trprr_return;
	    }

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trprr_return;
            }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_REVERSE_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearOpType = 
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 1)
	      object->FuncAllReverse = 0;
	    else
	      object->FuncAllReverse = 1;
	  }
	  else
	  {
	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->NumberOfData == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;

	      goto trprr_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->DataLP != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->DataLP )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trprr_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataLP;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->DataL_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_REVERSE_FRONT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutRearOpType =
		NMPS_OPTYPE_BACK;
	  }
	}

trprr_return:
	object->OldTriggForwFront = *(object->TriggForwFrontP);
	object->OldTriggReverseFront = *(object->TriggReverseFrontP);
	object->OldTriggForwBack = *(object->TriggForwBackP);
	object->OldTriggReverseBack = *(object->TriggReverseBackP);

	if ( object->Function & NMPS_TRPFUNC_TRIGGRESET)
	{
	  /* Reset the trigg flags */
	  object->TriggForwFront = 0;
	  object->TriggReverseFront = 0;
	  object->TriggForwBack = 0;
	  object->TriggReverseBack = 0;
	}
}

/*_*
  NMpsTrpFF

  @aref nmpstrpff NMpsTrpFF
*/
void NMpsTrpFF_exec(
  plc_sThread		*tp,
  pwr_sClass_NMpsTrpFF  *object)
{
	if (object->Status)
	  object->Status = 0;

	if ( ( *(object->TriggForwFrontP) && !object->OldTriggForwFront) ||
             ( *(object->TriggForwFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	       object->FuncAllForward )
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag ) return;

	    /* Check that both Back and Front is in the in cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->InP))->Data1_Back &&
	            ((pwr_sClass_NMpsCell *)(object->InP))->Data1_Front))
	    {
	      object->Status = 1;
	      goto trpff_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
	  	 NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontOpType =
		NMPS_OPTYPE_UNIT;
	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    /* Move all of data */
	    /* Check destination */
	    if ( *(object->TriggForwFrontP) && 
			!object->OldTriggForwFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }

	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex != 0)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trpff_return;
	        }
	      }
	    }

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllForward = 0;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag ) return;

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trpff_return;
            }
 
	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontOpType =
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 1)
	      object->FuncAllForward = 0;
	    else
	      object->FuncAllForward = 1;
	  }
	  else
	  {
	    /* Move front of data */
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->Data1P == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trpff_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType =
		NMPS_OPTYPE_FORWARD_FRONT;
	    ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontOpType = 
		NMPS_OPTYPE_BACK;
	  }
	}

	if ( *(object->TriggForwBackP) && !object->OldTriggForwBack)
	{
	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trpff_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->OutRearFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->Data1P != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trpff_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1P;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->InP))->Data1_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->InOpType = 
		NMPS_OPTYPE_FORWARD_BACK;
	  ((pwr_sClass_NMpsCell *)(object->InP))->OutFrontOpType = 
		NMPS_OPTYPE_FRONT;
	}

	if ( *(object->TriggReverseBackP) && !object->OldTriggReverseBack)
	{
	  /* Check destination */
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	  {
	    /* Destination cell is full */
	    object->Status = 1;
	    goto trpff_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	  /* Check source */
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	  {
	    /* Nothing to transport */
	    object->Status = 1;
	    goto trpff_return;
	  }
	  if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	  if ( ((pwr_sClass_NMpsCell *)(object->InP))->Data1P == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	  {
	    /* This should be a Front trigg */
	    object->Status = 1;
	    goto trpff_return;
	  }

	  /* Transport data */
	  ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	  ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_FORWARD_FRONT;
	  ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_BACK;
	}

	if ( ( *(object->TriggReverseFrontP) && !object->OldTriggReverseFront) ||
             ( *(object->TriggReverseFrontP) && object->Function & NMPS_TRPFUNC_LEVEL) ||
	     object->FuncAllReverse)
	{
	  if ( object->Function & NMPS_TRPFUNC_UNIT)
	  {
	    /* Check destination */
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    /* Check that both Front and Back is in the cell */
	    if ( !( ((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Back &&
	            ((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Front))
	    {
	      object->Status = 1;
	      goto trpff_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_UNIT;

	  }
	  else if (( object->Function & NMPS_TRPFUNC_ALL) ||
	 	( object->Function & NMPS_TRPFUNC_ALLEMPTY))
	  {
	    if ( *(object->TriggReverseFrontP) && 
			!object->OldTriggReverseFront)
	    {
	      /* This is the first object to move */
	      if ( object->FuncAllReverse || object->FuncAllForward)
	      {
	        /* We are already busy moving last trigg */
	        return;
	      }	        

	      /* Check destination */
	      if ( object->Function & NMPS_TRPFUNC_ALLEMPTY)
	      {
	        if ( ((pwr_sClass_NMpsCell *)(object->InP))->LastIndex)
	        {
	          /* Destination cell is not empty */
	          object->Status = 1;
	          goto trpff_return;
	        }
	      }
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->CellFull)
	    {
	      /* Destination cell is full */
	      object->Status = 1;
	      object->FuncAllReverse = 0;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->InFlag ) return;

	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->Data1P == 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      object->Status = 1;
	      goto trpff_return;
	    }

            if ( object->Function & NMPS_TRPFUNC_SLOW &&
                 object->FuncAllForward == 1)
            {
              /* Wait till next scan */
              object->FuncAllForward = 2;
              goto trpff_return;
            }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType = 
		NMPS_OPTYPE_FORWARD_UNIT;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType = 
		NMPS_OPTYPE_UNIT;

	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 1)
	      object->FuncAllReverse = 0;
	    else
	      object->FuncAllReverse = 1;
	  }
	  else
	  {
	    /* Check source */
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->LastIndex == 0 )
	    {
	      /* Nothing to transport */
	      object->Status = 1;
	      goto trpff_return;
	    }
	    if ( ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag ) return;

	    if ( ((pwr_sClass_NMpsCell *)(object->InP))->Data1P != 
	       ((pwr_sClass_NMpsCell *)(object->OutP))->Data1P )
	    {
	      /* This should be a Back trigg */
	      object->Status = 1;
	      goto trpff_return;
	    }

	    /* Transport data */
	    ((pwr_sClass_NMpsCell *)(object->InP))->InPointer =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1P;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InObjId =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_ObjId;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InDlid =
		((pwr_sClass_NMpsCell *)(object->OutP))->Data1_Dlid;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontFlag = 1;
	    ((pwr_sClass_NMpsCell *)(object->InP))->InOpType =
		NMPS_OPTYPE_FORWARD_BACK;
	    ((pwr_sClass_NMpsCell *)(object->OutP))->OutFrontOpType =
		NMPS_OPTYPE_FRONT;
	  }
	}

trpff_return:
	object->OldTriggForwFront = *(object->TriggForwFrontP);
	object->OldTriggReverseFront = *(object->TriggReverseFrontP);
	object->OldTriggForwBack = *(object->TriggForwBackP);
	object->OldTriggReverseBack = *(object->TriggReverseBackP);

	if ( object->Function & NMPS_TRPFUNC_TRIGGRESET)
	{
	  /* Reset the trigg flags */
	  object->TriggForwFront = 0;
	  object->TriggReverseFront = 0;
	  object->TriggForwBack = 0;
	  object->TriggReverseBack = 0;
	}
}

/*_*
  DataSelect

  @aref dataselect DataSelect
*/
void DataSelect_exec(
  plc_sThread		*tp,
  pwr_sClass_DataSelect  *object)
{
	int	i;
	float	**DataP;

	i = *object->IndexP + FLT_EPSILON;
	if ( i < 1)
	  i = 1;
	if ( i > *object->MaxIndexP)
	  i = *object->MaxIndexP + FLT_EPSILON;
	i--;

	DataP = (float **)((char *) &object->DataIn1P +
		i * (sizeof(object->DataIn1) + sizeof(object->DataIn1P)));

	memcpy( &object->OutDataP, *DataP,
		sizeof( object->OutDataP) + sizeof( object->OutData_ObjId));
}



/*      NMpsMirrorCell
*/
void NMpsMirrorCell_init(object)
pwr_sClass_NMpsMirrorCell  *object;
{
	pwr_sAttrRef 		attrref;
	plc_t_DataInfoMirCell	*data_index;
	int			i, sts;

	/* Get new object pointers at restart av plc program */
	data_index = (plc_t_DataInfoMirCell *) &object->Data1P;
	memset( &attrref, 0, sizeof(attrref));
	for ( i = 0; i < object->LastIndex; i++)
	{
	  attrref.Objid = data_index->Data_ObjId;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
			(pwr_tAddress *) &data_index->DataP, 
			&data_index->Data_Dlid);
	  data_index++;
	}
	if ( object->LastIndex)
	{
	  data_index = (plc_t_DataInfoMirCell *) &object->Data1P;
	  data_index += object->LastIndex - 1;
	  memcpy( &object->DataLastP, data_index, sizeof( *data_index));
	}
	else
	{
	  memset( &object->DataLastP, 0, sizeof( *data_index));
	}
	object->InitFlag = 1;
}

/*_*
  NMpsMirrorCell

  @aref nmpsmirrorcell NMpsMirrorCell
*/
void NMpsMirrorCell_exec(
  plc_sThread		*tp,
pwr_sClass_NMpsMirrorCell  *object)
{
	pwr_sAttrRef 		attrref;
	plc_t_DataInfoMirCell	*data_index;
	int			i, sts;

	if ( !object->UpdateFlag)
	  return;

	/* Get new object pointers to new objects */
	data_index = (plc_t_DataInfoMirCell *) object->TempArea;
	memset( &attrref, 0, sizeof(attrref));
	for ( i = 0; i < object->TempLastIndex; i++)
	{
	  if ( data_index->DataP == 0)
	  {
	    attrref.Objid = data_index->Data_ObjId;
	    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
			(pwr_tAddress *) &data_index->DataP, 
			&data_index->Data_Dlid);
	  }
	  data_index++;
	}
	memcpy( &object->Data1P, object->TempArea, sizeof(object->TempArea));
	object->LastIndex = object->TempLastIndex;
	if ( object->LastIndex)
	{
	  data_index = (plc_t_DataInfoMirCell *) &object->Data1P;
	  data_index += object->LastIndex - 1;
	  memcpy( &object->DataLastP, data_index, sizeof( *data_index));
	}
	else
	{
	  memset( &object->DataLastP, 0, sizeof( *data_index));
	}
	if ( object->LastIndex == object->MaxSize)
	  object->CellFull = 1;
	else
	  object->CellFull = 0;

	object->UpdateFlag = 0;
	if ( object->BackupNowMsg)
	{
	  object->BackupNow = 1;
	  object->BackupNowMsg = 0;
	}
}


int	nmps_RemoveData( pwr_tObjid objid)
{
	int				sts;
	pwr_tObjid			cell_objid;
	pwr_sClass_NMpsCell		*cell_p;
	pwr_sClass_NMpsStoreCell	*scell_p;

	sts = gdh_GetClassList ( pwr_cClass_NMpsCell, &cell_objid);
	while( ODD(sts))
	{
	  sts = gdh_ObjidToPointer ( cell_objid, (pwr_tAddress *) &cell_p);
	  if ( EVEN(sts)) return sts;
	  cell_p->ExternOpType = NMPS_OPTYPE_EXTDELETE_OBJID;
	  cell_p->ExternObjId  = objid;
	  cell_p->ExternFlag   = 1;
	  sts = gdh_GetNextObject ( cell_objid, &cell_objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsStoreCell, &cell_objid);
	while( ODD(sts))
	{
	  sts = gdh_ObjidToPointer ( cell_objid, (pwr_tAddress *) &scell_p);
	  if ( EVEN(sts)) return sts;
	  scell_p->ExternOpType = NMPS_OPTYPE_EXTDELETE_OBJID;
	  scell_p->ExternObjId  = objid;
	  scell_p->ExternFlag   = 1;
	  sts = gdh_GetNextObject ( cell_objid, &cell_objid);
	}
	return 1;
}

int	nmps_RemoveAndDeleteData( pwr_tObjid objid)
{
	int				sts;
	pwr_tObjid			cell_objid;
	pwr_sClass_NMpsCell		*cell_p;
	pwr_sClass_NMpsStoreCell	*scell_p;

	sts = gdh_GetClassList ( pwr_cClass_NMpsCell, &cell_objid);
	while( ODD(sts))
	{
	  sts = gdh_ObjidToPointer ( cell_objid, (pwr_tAddress *) &cell_p);
	  if ( EVEN(sts)) return sts;
	  cell_p->ExternOpType = NMPS_OPTYPE_EXTDELETE_OBJID;
	  cell_p->ExternObjId  = objid;
	  cell_p->ExternFlag   = 1;
	  sts = gdh_GetNextObject ( cell_objid, &cell_objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsStoreCell, &cell_objid);
	while( ODD(sts))
	{
	  sts = gdh_ObjidToPointer ( cell_objid, (pwr_tAddress *) &scell_p);
	  if ( EVEN(sts)) return sts;
	  scell_p->ExternOpType = NMPS_OPTYPE_EXTDELETE_OBJID;
	  scell_p->ExternObjId  = objid;
	  scell_p->ExternFlag   = 1;
	  sts = gdh_GetNextObject ( cell_objid, &cell_objid);
	}

	sts = gdh_DeleteObject(objid);
	if ( EVEN(sts)) return sts;

	return 1;
}

/*_*
  DataFWrite

  @aref datafwrite DataFWrite
*/
void DataFWrite_exec( plc_sThread		*tp,
		      pwr_sClass_DataFWrite     *o)
{
  if ( o->Error)
    o->Error = 0;

  if ( *o->ConditionP && !o->CondOld) {
    pwr_tAttrRef aref = cdh_ObjidToAref( *(pwr_tOid *)((char *)o->DataP + pwr_cAlignLW));

    o->WriteStatus = gdh_FWriteObject( *o->FileNameP, &aref);
    if ( EVEN(o->WriteStatus))
      o->Error = 1;

    if ( o->ConditionP == &o->Condition)
      o->Condition = 0;
  }
  o->CondOld = *o->ConditionP;
}

/*_*
  DataFRead

  @aref datafread DataFRead
*/
void DataFRead_exec( plc_sThread		*tp,
		     pwr_sClass_DataFRead       *o)
{
  if ( o->Error)
    o->Error = 0;

  if ( *o->ConditionP && !o->CondOld) {
    pwr_tAttrRef aref = cdh_ObjidToAref( *(pwr_tOid *)((char *)o->DataP + pwr_cAlignLW));

    o->ReadStatus = gdh_FReadObject( *o->FileNameP, &aref);
    if ( EVEN(o->ReadStatus))
      o->Error = 1;

    if ( o->ConditionP == &o->Condition)
      o->Condition = 0;
  }
  o->CondOld = *o->ConditionP;
}
