/** 
 * Proview   $Id: flow_x.c,v 1.1 2007-02-07 15:44:25 claes Exp $
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

//
// Interface to Xlib and Xt macros which is compiled with alignment on alpha.
// Proview   $Id: flow_x.c,v 1.1 2007-02-07 15:44:25 claes Exp $ is compiled without alignment X-macros doesn't work, and neither
// does the #pragma alignment...
//

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Protocols.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "flow_x.h"

void flow_SetInputFocus( Widget w)
{
  XSetInputFocus( XtDisplay(w), XtWindow(w), 
		RevertToParent, CurrentTime);
}

Boolean flow_IsShell( Widget w)
{
  return XtIsShell( w);
}

Boolean flow_IsManaged( Widget w)
{
  return XtIsManaged( w);
}

Boolean flow_IsRealized( Widget w)
{
  return XtIsRealized( w);
}

void flow_AddCloseVMProtocolCb( Widget shell, XtCallbackProc callback,
	void *client_data)
{
  Atom WM_DELETE_WINDOW = XmInternAtom( XtDisplay(shell), "WM_DELETE_WINDOW",
		False);
  XmAddWMProtocolCallback( shell, WM_DELETE_WINDOW, callback, client_data);
}

void flow_Bell( Widget w)
{
  XBell( XtDisplay(w), 100);
}

void flow_UnmapWidget( Widget w)
{
  XtUnmapWidget( w);
}

void flow_MapWidget( Widget w)
{
  XtMapWidget( w);
}

Screen *flow_Screen( Widget w)
{
  return XtScreen( w);
}

Display *flow_Display( Widget w)
{
  return XtDisplay(w);
}

Window flow_Window( Widget w)
{
  return XtWindow(w);
}

int flow_IsViewable( Widget w)
{
  XWindowAttributes xwa;
  if ( XGetWindowAttributes( XtDisplay(w), XtWindow(w), &xwa) &&
       xwa.map_state == IsViewable)
    return 1;
  return 0;
}
