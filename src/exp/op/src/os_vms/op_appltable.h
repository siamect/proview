#ifndef op_appltable_h
#define op_appltable_h

/* op_appltable.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This file contains the definition of the applstruct which is
   the link between the mainprogram and each applstate.  */

typedef struct
{
    char ClassName[20];
    int (*Initialize) ();
    int (*Another) ();
    int (*Change) ();
} APPLTABLESTRUCT;

void appltab_Init (
    APPLTABLESTRUCT **ApplTablePP,
    int *NoInApplTableP
);

#endif
