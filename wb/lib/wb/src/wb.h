#ifndef wb_h
#define wb_h

/* wb.h -- work bench

   PROVIEW/R
   Copyright (C) 1994-1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#define WB_CLASS_NAME "PWR_DEV"

typedef enum {
	wb_eType_Volume,
	wb_eType_Directory,
	wb_eType_Class,
	wb_eType_Buffer
} wb_eType;

typedef enum {
	wb_eUtility_Wtt 		= 999,
	wb_eUtility_PlcEditor 		= 1000,
	wb_eUtility_AttributeEditor	= 1001,
	wb_eUtility_Distributor		= 1002
} wb_eUtility;

#endif




