/* co_platform.c -- platform routines

   PROVIEW/R
   Copyright (C)  2002 by Cell Network AB.

   This module contains data structures that descibes a PROVIEW platform  
   */


#include "co_platform.h"


/*
*  co_GetOwnPlatform
*/
co_sPlatform* co_GetOwnPlatform(co_sPlatform *platform)
{
    platform->os = co_dHostOS;
    platform->hw = co_dHostHW;
    co_GetOwnFormat(&platform->fm);

    return platform;    
}


co_mFormat* co_GetOwnFormat(co_mFormat *format)
{
    format->m = 0;
    format->b.bo = co_dHostByteOrder;
    format->b.ft = co_dHostFloatType;
    format->b.ct = co_eCT_ascii;

    return format;
}
