/* 
 * Proview   $Id: wb.h,v 1.8 2005-09-06 10:43:30 claes Exp $
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

#ifndef wb_h
#define wb_h

/* wb.h -- work bench */

#ifndef pwr_h
#include "pwr.h"
#endif

#define WB_CLASS_NAME "PWR_DEV"

typedef enum {
	wb_eType_Volume,
	wb_eType_Directory,
	wb_eType_Class,
	wb_eType_Buffer,
	wb_eType_ClassEditor
} wb_eType;

typedef enum {
	wb_eUtility_Wtt 		= 999,
	wb_eUtility_PlcEditor 		= 1000,
	wb_eUtility_AttributeEditor	= 1001,
	wb_eUtility_Distributor		= 1002,
	wb_eUtility_SpreadsheetEditor  	= 1003
} wb_eUtility;

#endif




