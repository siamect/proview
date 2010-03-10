/* 
 * Proview   $Id: glow_tracedata.h,v 1.2 2005-09-01 14:57:54 claes Exp $
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

#ifndef glow_tracedata_h
#define glow_tracedata_h

#include <string.h>
#include "glow.h"
#include "pwr_privilege.h"

typedef pwr_mAccess glow_mAccess;
#define glow_mAccess_RtDefault pwr_mAccess_RtDefault

class GlowTraceData {
  public:
    GlowTraceData() : attr_type(glow_eTraceType_Boolean),
	color(glow_eDrawType_Inherit), color2(glow_eDrawType_Inherit),
        p(NULL), access(glow_mAccess_RtDefault), cycle(glow_eCycle_Inherit)
	{
	  strcpy( data[0], "");
	  strcpy( data[1], "");
	  strcpy( data[2], "");
	  strcpy( data[3], "");
	  strcpy( data[4], "");
	  strcpy( data[5], "");
	  strcpy( data[6], "");
	  strcpy( data[7], "");
	  strcpy( data[8], "");
	  strcpy( data[9], "");
	  strcpy( ref_object, "");
	};

    char		data[10][120];
    glow_eTraceType	attr_type;
    glow_eDrawType	color;
    glow_eDrawType	color2;
    void		*p;
    glow_mAccess	access;
    glow_eCycle         cycle;
    char                ref_object[120];
};
#endif
