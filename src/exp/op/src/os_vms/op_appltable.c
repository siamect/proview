/* op_appltable.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This file contains the initializations for the different
   applstates.  */

#include "pwr.h"
#include "op_appltable.h"

void appltab_Init (
    APPLTABLESTRUCT **ApplTablePP,
    int *NoInApplTableP
)
{
    static APPLTABLESTRUCT ApplTable[2];
    int I=0;

    *ApplTablePP = ApplTable;
    *NoInApplTableP = 0;
  
}
