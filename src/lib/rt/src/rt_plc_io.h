/* rt_plc_io.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#define UC_NORMAL	0
#define UC_READ		1
#define UC_WRITE	2
#define UC_READ2	3
#define UC_WRITE2	4

#define	_z_	,


/* Direct link table */
typedef struct 
{
  void		**Pointer;
  pwr_tObjid	Objid;
  pwr_tClassId	ObjType;
  pwr_tUInt32	Size;
  pwr_tUInt32	UseCode;
  }	plc_t_rtdbref;


typedef struct 	{
	pwr_tBoolean	ActualValue;
	}	plc_sClass_Di;

typedef struct 	{
	pwr_tBoolean	ActualValue;
	}	plc_sClass_Do;

typedef struct 	{
	pwr_tBoolean	ActualValue;
	}	plc_sClass_Po;

typedef struct 	{
	pwr_tBoolean	ActualValue;
	}	plc_sClass_Dv;

typedef struct 	{
	pwr_tInt32	RawValue;
	}	plc_sClass_Co;

typedef struct 	{
	pwr_tFloat32	ActualValue;
	}	plc_sClass_Ai;

typedef struct 	{
	pwr_tFloat32	ActualValue;
	}	plc_sClass_Ao;

typedef struct 	{
	pwr_tFloat32	ActualValue;
	}	plc_sClass_Av;

typedef struct 	{
	pwr_tInt32	ActualValue;
	}	plc_sClass_Ii;

typedef struct 	{
	pwr_tInt32	ActualValue;
	}	plc_sClass_Io;

typedef struct 	{
	pwr_tInt32	ActualValue;
	}	plc_sClass_Iv;

