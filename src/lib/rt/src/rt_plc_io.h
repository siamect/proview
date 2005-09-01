/* 
 * Proview   $Id: rt_plc_io.h,v 1.4 2005-09-01 14:57:56 claes Exp $
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
  pwr_sAttrRef	AttrRef;
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

