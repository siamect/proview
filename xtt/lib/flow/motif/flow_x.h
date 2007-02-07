/* 
 * Proview   $Id: flow_x.h,v 1.1 2007-02-07 15:36:52 claes Exp $
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

#ifndef flow_x_h
#define flow_x_h

#if defined __cplusplus
extern "C" {
#endif

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void flow_SetInputFocus( Widget w);
Boolean flow_IsManaged( Widget w);
Boolean flow_IsShell( Widget w);
Boolean flow_IsRealized( Widget w);
void flow_AddCloseVMProtocolCb( Widget shell, XtCallbackProc callback,
	void *client_data);
void flow_Bell( Widget w);
void flow_UnmapWidget( Widget w);
void flow_MapWidget( Widget w);
Screen *flow_Screen( Widget w);
Display *flow_Display( Widget w);
Window flow_Window( Widget w);
int flow_IsViewable( Widget w);

#if defined __cplusplus
}
#endif
#endif
