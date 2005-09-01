/** 
 * Proview   $Id: co_platform.c,v 1.4 2005-09-01 14:57:52 claes Exp $
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

/* co_platform.c -- platform routines
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
    platform->bo = co_dHostByteOrder;
    platform->ft = co_dHostFloatType;

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

