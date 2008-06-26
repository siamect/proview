/* 
 * Proview   $Id: ge_convert.cpp,v 1.7 2008-06-26 13:23:59 claes Exp $
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

/* ge_convert.cpp
   Conversion between different versions of Ge. 
   3.4 -> 4.0 
*/


#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

#include "pwr_class.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_dcli.h"
#include "co_ccm_msg.h"
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"

#include "ge_graph.h"
#include "co_dcli_msg.h"
#include "ge_msg.h"
#include "ge.h"
#include "ge_dyn.h"

extern "C" {
#if LDH
# include "wb_wccm.h"
#endif
}

#if defined OS_LINUX

int Graph::convert()
{
  char name[80];
  grow_tObject *objectlist;
  int object_cnt;

  // grow_Convert( grow->ctx, glow_eConvert_V34);

  // Convert all objects
  grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);
  for ( int i = 0; i < object_cnt; i++) {
    grow_GetObjectName( objectlist[i], name);
    if ( strcmp( name, "") == 0) {
      sprintf( name, "O%d", grow_IncrNextObjectNameNumber( grow->ctx));
      grow_SetObjectName( objectlist[i], name);
    }

    if ( grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowGroup) {
      grow_tObject *grouplist;
      int group_cnt;

      grow_GetGroupObjectList( objectlist[i], &grouplist, &group_cnt);
      for ( int j = 0; j < group_cnt; j++) {
	grow_GetObjectName( grouplist[j], name);
	if ( strcmp( name, "") == 0) {
	  sprintf( name, "O%d", grow_IncrNextObjectNameNumber( grow->ctx));
	  grow_SetObjectName( grouplist[j], name);
	}
      }      
    }
      
  } 

  // grow_Redraw( grow->ctx);
  return 1;
}


#else

int Graph::convert()
{
  return 1;
}
#endif




