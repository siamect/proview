/* co_platform.c -- platform routines

   PROVIEW/R
   Copyright (C)  2002 by Cell Network AB.

   This module contains data structures that descibes a PROVIEW platform  
   */


#include "co_platform.h"


/*
 * co_GetOwnFormat
 */
co_mFormat* co_GetOwnFormat(co_mFormat *format)
{
 
    format->m = 0;
    format->b.bo = co_dHostByteOrder;
    format->b.ct = co_dHostAsciiType;
    format->b.ft = co_dHostFloatType;

    return format;
}




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

/*
 * co_SetFormat
 */
co_mFormat* co_SetFormat(co_mFormat *format, co_eBO bo, co_eFT ft)
{
  format->m = 0;
  format->b.bo = bo;
  format->b.ft = ft;
  format->b.ct = co_eCT_ascii;

  return format;
}

