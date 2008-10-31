/* 
 * Proview   $Id: wb_nav_motif.cpp,v 1.2 2008-10-31 12:51:31 claes Exp $
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

/* wb_nav_motif.cpp -- Display plant and node hiererachy or plc-editor */

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

extern "C" {
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "co_cdh.h"
#include "flow_x.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "wb_nav_motif.h"

//
// Create the navigator widget
//
NavMotif::NavMotif(
	void *nav_parent_ctx,
	Widget	nav_parent_wid,
	const char *nav_name,
	ldh_tSesContext nav_ldhses,
	const char *nav_root_name,
	Widget *w,
	pwr_tStatus *status
	) :
  Nav(nav_parent_ctx,nav_name,nav_ldhses,nav_root_name,status), 
  parent_wid(nav_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
NavMotif::~NavMotif()
{
  PalFile::config_tree_free( menu);
  free_pixmaps();
  XtDestroyWidget( form_widget);
}

static Boolean nav_sel_convert_cb(
    Widget  w,
    Atom    *selection,
    Atom    *target,
    Atom    *type_return,
    XtPointer	*value_return,
    unsigned long   *length_return,
    int	    *format_return)
{
  NavMotif      *nav;
  brow_tCtx	browctx;
  char		name[200];
  int 		sts;

  BrowCtxFromWidget( w, (void **) &browctx);
  brow_GetCtxUserData( browctx, (void **) &nav);

  if (*target == XA_TARGETS(flow_Display(nav->brow_widget))) {
    Atom *targetP;
    Atom *std_targets;
    unsigned long std_length;
    XSelectionRequestEvent *req = XtGetSelectionRequest( w, *selection, 
       (XtRequestId)NULL);

    XmuConvertStandardSelection( w, req->time, selection, target, type_return,
		(caddr_t *)&std_targets, &std_length, format_return);

    *value_return = XtMalloc( sizeof(Atom) * (std_length + 2));
    targetP = *(Atom **) value_return;
    *targetP++ = XA_STRING;
    *targetP++ = XA_TEXT(flow_Display(nav->brow_widget));
    *length_return = std_length + (targetP - (*(Atom **) value_return));
    bcopy((char *)std_targets, (char *)targetP, sizeof(Atom) * std_length);
    XtFree( (char *)std_targets);
    *type_return = XA_ATOM;
    *format_return = 32;
    return True;
  }

  if (*target == XA_STRING)
  {
    if ( !nav->get_plant_select_cb)
      return FALSE;

    sts = nav->get_plant_select_cb( nav->parent_ctx, name);
    if ( EVEN(sts)) return FALSE;
    *value_return = XtNewString(name);      
    *length_return = strlen(name) + 1;

    *type_return = XA_STRING;
    *format_return = 8;

    return TRUE;
  }
  return FALSE;
}

static void nav_sel_lose_cb( Widget w, Atom *selection)
{
  Nav	*nav;
  brow_tCtx	browctx;

  BrowCtxFromWidget( w, (void **) &browctx);
  brow_GetCtxUserData( browctx, (void **) &nav);

  brow_SelectClear( nav->brow_ctx);
}

void NavMotif::set_selection_owner()
{
  int sts;

  sts = XtOwnSelection( brow_widget, XA_PRIMARY, 
	XtLastTimestampProcessed(flow_Display(brow_widget)),  
	nav_sel_convert_cb, nav_sel_lose_cb , NULL);
  if ( !sts)
  {
     brow_SelectClear( brow_ctx);
     return;
  }	
}

void NavMotif::set_inputfocus( int focus)
{
  Arg 		args[2];
  Pixel 	bg, fg;

  if ( !displayed)
    return;

  XtVaGetValues( form_widget, XmNbackground, &bg, XmNforeground, &fg, NULL);
  if ( focus)
  {
    XtCallAcceptFocus( brow_widget, CurrentTime);
    XtSetArg(args[0], XmNborderColor, fg);
    XtSetValues( form_widget, args, 1);
  }
  else
  {
    XtSetArg(args[0], XmNborderColor, bg);
    XtSetValues( form_widget, args, 1);
  }
//  brow_SetInputFocus( brow->ctx);
}

