/* op_appltypetab.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Contains the initializations for the different
   application specifik typestates.  */

#include "pwr.h"
#include "op_typetab.h"

pwr_tUInt32 appltypetab_Init(TypeTablePP,NoInTypeTableP)
TYPETABLESTRUCT **TypeTablePP;
int *NoInTypeTableP;
{
    static TYPETABLESTRUCT TypeTable[3];

    *TypeTablePP = TypeTable;
    *NoInTypeTableP = 0;

    return 1;
}
