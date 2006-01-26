/* 
 * Proview   $Id: nmps_plc_macro.h,v 1.2 2006-01-26 08:15:12 claes Exp $
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

/*		 PREPROCESSOR RUTINER					    */

#if defined OS_ELN
# include float
#else
# include <float.h>
#endif

typedef struct {
	char		Key[40];
	pwr_tStatus	Status;
	char		Data[1];
	} nmps_sTrans;

typedef struct {
	pwr_tUInt32	*DataP;
	pwr_tObjid	Data_ObjId;
	pwr_tBoolean	Data_Front;
	pwr_tBoolean	Data_Back;
	pwr_tRefId	Data_Dlid;
	pwr_tBoolean	Data_Select;
	pwr_tBoolean	Data_OldSelect;
	} plc_t_DataInfo;

typedef struct {
	pwr_tUInt32	*DataP;
	pwr_tObjid	Data_ObjId;
	pwr_tRefId	Data_Dlid;
	} plc_t_DataInfoMirCell;

/*_*
  DataCopy

  @aref datacopy DataCopy
*/
#define datacopy_exec( object, datasize)				\
  if ( *object->ConditionP )						\
  {									\
    if ( *object->DataToP != 0 && *object->DataFromP != 0)		\
      memcpy( *object->DataToP, *object->DataFromP, datasize);		\
  }

/*_*
  DataReset

  @aref datareset DataReset
*/
#define datareset_exec( object, datasize)				\
  if ( *object->ConditionP )						\
  {									\
    if ( *object->DataP != 0 )						\
      memset( *object->DataP, 0, datasize);				\
  }

/*_*
  NMpsCellSubWind

  @aref nmpscellsubwind NMpsCellSubWind
*/
#define NMpsCellSubWind_exec( object, subwindow)			\
  if ( !(object->InitTime && !object->ReloadDone))			\
  {									\
    object->CurrentIndex = 0;						\
    memset( &(object->DataCurrentP), 0,	 				\
		sizeof( plc_t_DataInfo) - 16);				\
    subwindow;								\
    for ( object->Idx = 1; object->Idx <=	 			\
	object->LastIndex; object->Idx++ )				\
    {									\
      memcpy( &(object->DataCurrentP), 					\
		(char *) &object->Data1P +				\
		(object->Idx - 1) * sizeof( plc_t_DataInfo), 		\
		sizeof( plc_t_DataInfo) - 16);				\
      object->CurrentIndex = object->Idx;				\
      subwindow;							\
    }									\
  }

/*_*
  NMpsStoreCellSubWind

  @aref nmpscellstoresubwind NMpsStoreCellSubWind
*/
#define NMpsStoreCellSubWind_exec( object, subwindow)			\
  if ( !(object->InitTime && !object->ReloadDone))			\
  {									\
    object->CurrentIndex = 0;						\
    memset( &(object->DataCurrentP), 0,	 				\
		sizeof( plc_t_DataInfo) - 16);				\
    subwindow;								\
    for ( object->Idx = 1; object->Idx <=	 			\
	object->LastIndex; object->Idx++ )				\
    {									\
      memcpy( &(object->DataCurrentP), 					\
		(char *) &object->Data1P +				\
		(object->Idx - 1) * sizeof( plc_t_DataInfo), 		\
		sizeof( plc_t_DataInfo) - 16);				\
      object->CurrentIndex = object->Idx;				\
      subwindow;							\
    }									\
  }

/*_*
  CLoop

  @aref cloop CLoop
*/
#define CLoop_exec( object, subwindow)					\
  if ( *object->ConditionP)						\
  {									\
    object->CurrentIndex = object->StartIndex;				\
    for ( object->CurrentIndex = object->StartIndex;			\
	object->CurrentIndex <=	object->StopIndex + FLT_EPSILON; 	\
	object->CurrentIndex += object->Increment)			\
    {									\
      subwindow;							\
    }									\
  }

/*_*
  GetDap

  @aref getdap GetDap
*/
#define GetDap_exec( object, attribute)					\
  memcpy( &object->OutDataP, &attribute, 				\
	sizeof( object->OutDataP) + sizeof( object->OutData_ObjId));

/*_*
  GetObjidp

  @aref getobjidp GetObjidp
*/
#define GetObjidp_exec( object, attribute)				\
  object->ObjId = attribute;

/*_*
  Func

  @aref func Func
*/
#define Func_exec( object, subwindow)					\
  if ( object->ConditionP) object->Condition = *object->ConditionP;	\
  if ( object->A1P) object->A1 = *object->A1P;			\
  if ( object->A2P) object->A2 = *object->A2P;			\
  if ( object->A3P) object->A3 = *object->A3P;			\
  if ( object->A4P) object->A4 = *object->A4P;			\
  if ( object->A5P) object->A5 = *object->A5P;			\
  if ( object->A6P) object->A6 = *object->A6P;			\
  if ( object->A7P) object->A7 = *object->A7P;			\
  if ( object->A8P) object->A8 = *object->A8P;			\
  if ( object->d1P) object->d1 = *object->d1P;			\
  if ( object->d2P) object->d2 = *object->d2P;			\
  if ( object->d3P) object->d3 = *object->d3P;			\
  if ( object->d4P) object->d4 = *object->d4P;			\
  if ( object->d5P) object->d5 = *object->d5P;			\
  if ( object->d6P) object->d6 = *object->d6P;			\
  if ( object->d7P) object->d7 = *object->d7P;			\
  if ( object->d8P) object->d8 = *object->d8P;			\
  if ( object->Condition) {					\
    subwindow;							\
  }

/*_*
  FuncExtend

  @aref funcextend FuncExtend
*/
#define FuncExtend_exec( object)				\
  if ( object->A1P) object->A1 = *object->A1P;			\
  if ( object->A2P) object->A2 = *object->A2P;			\
  if ( object->A3P) object->A3 = *object->A3P;			\
  if ( object->A4P) object->A4 = *object->A4P;			\
  if ( object->A5P) object->A5 = *object->A5P;			\
  if ( object->A6P) object->A6 = *object->A6P;			\
  if ( object->A7P) object->A7 = *object->A7P;			\
  if ( object->A8P) object->A8 = *object->A8P;			\
  if ( object->d1P) object->d1 = *object->d1P;			\
  if ( object->d2P) object->d2 = *object->d2P;			\
  if ( object->d3P) object->d3 = *object->d3P;			\
  if ( object->d4P) object->d4 = *object->d4P;			\
  if ( object->d5P) object->d5 = *object->d5P;			\
  if ( object->d6P) object->d6 = *object->d6P;			\
  if ( object->d7P) object->d7 = *object->d7P;			\
  if ( object->d8P) object->d8 = *object->d8P;

/*_*
  FuncInput

  @aref funcinput FuncInput
*/
#define FuncInput_exec( object, funcobject)				\
  object->A1 = funcobject->A1;					\
  object->A2 = funcobject->A2;					\
  object->A3 = funcobject->A3;					\
  object->A4 = funcobject->A4;					\
  object->A5 = funcobject->A5;					\
  object->A6 = funcobject->A6;					\
  object->A7 = funcobject->A7;					\
  object->A8 = funcobject->A8;					\
  object->d1 = funcobject->d1;					\
  object->d2 = funcobject->d2;					\
  object->d3 = funcobject->d3;					\
  object->d4 = funcobject->d4;					\
  object->d5 = funcobject->d5;					\
  object->d6 = funcobject->d6;					\
  object->d7 = funcobject->d7;					\
  object->d8 = funcobject->d8;

/*_*
  FuncOutput

  @aref funcoutput FuncOutput
*/
#define FuncOutput_exec( object, funcobject)				\
  funcobject->OA1 = *object->OA1P;					\
  funcobject->OA2 = *object->OA2P;					\
  funcobject->OA3 = *object->OA3P;					\
  funcobject->OA4 = *object->OA4P;					\
  funcobject->OA5 = *object->OA5P;					\
  funcobject->OA6 = *object->OA6P;					\
  funcobject->OA7 = *object->OA7P;					\
  funcobject->OA8 = *object->OA8P;					\
  funcobject->od1 = *object->od1P;					\
  funcobject->od2 = *object->od2P;					\
  funcobject->od3 = *object->od3P;					\
  funcobject->od4 = *object->od4P;					\
  funcobject->od5 = *object->od5P;					\
  funcobject->od6 = *object->od6P;					\
  funcobject->od7 = *object->od7P;					\
  funcobject->od8 = *object->od8P;

/*_*
  NMpsMirrorCellSubWind

  @aref nmpsmirrorcellsubwind NMpsMirroCellSubWind
*/
#define NMpsMirrorCellSubWind_exec( object, subwindow)			\
  object->CurrentIndex = 0;						\
  memset( &(object->DataCurrentP), 0,	 				\
		sizeof( plc_t_DataInfoMirCell) - sizeof(pwr_tRefId));	\
  subwindow;								\
  for ( object->Idx = 1; object->Idx <=	 				\
	object->LastIndex; object->Idx++ )				\
  {									\
    memcpy( &(object->DataCurrentP), 					\
		(char *) &object->Data1P +				\
		(object->Idx - 1) * sizeof( plc_t_DataInfoMirCell),	\
		sizeof( plc_t_DataInfoMirCell) - sizeof(pwr_tRefId));   \
    object->CurrentIndex = object->Idx;					\
    subwindow;								\
  }




