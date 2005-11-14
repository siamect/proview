/* 
 * Proview   $Id: xtt_c_sound.cpp,v 1.1 2005-11-14 16:17:13 claes Exp $
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
 */

/* wb_c_sound.c -- xtt methods of the Sound class. */

#include "pwr_baseclasses.h"
#include "flow_std.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xtt_menu.h"
#include "xtt_xnav.h"
#include "rt_xnav_msg.h"
#include "pwr_privilege.h"

// Play
static pwr_tStatus Play( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->sound( &ip->Pointed);
  return 1;
}

static pwr_tStatus PlayFilter( xmenu_sMenuCall *ip)
{
  if ( ((XNav *)ip->EditorContext)->sound_attached())
    return 1;
  return XNAV__INSENSITIVE;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to xtt should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindXttMethods(Sound) = {
  pwr_BindXttMethod(Play),
  pwr_BindXttMethod(PlayFilter),
  pwr_NullMethod
};





