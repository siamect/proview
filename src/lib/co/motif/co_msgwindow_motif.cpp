/** 
 * Proview   $Id: co_msgwindow_motif.cpp,v 1.1 2007-01-04 07:51:41 claes Exp $
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

/* co_msgwindow_motif.cpp -- Message window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" {
#include "flow_x.h"
}
#include "co_lng.h"
#include "co_xhelp.h"
#include "co_msgwindow_motif.h"
#include "co_msglist_motif.h"
#include "co_wow_motif.h"


MsgWindowMotif::MsgWindowMotif(
	void *msg_parent_ctx,
	Widget	msg_parent_wid,
	char *msg_name,
	pwr_tStatus *status) :
  MsgWindow( parent_ctx, msg_name, status), parent_wid(msg_parent_wid)
{
  char		uid_filename[120] = {"$pwr_exe/wb_msgwindow.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;

  static char msg_translations[] =
    "<FocusIn>: msg_inputfocus()\n";
  static XtTranslations msg_compiled_translations = NULL;

  static XtActionsRec msg_actions[] =
  {
    {"msg_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "msg_ctx", 0 },
	{"msg_activate_exit",(caddr_t)activate_exit },
	{"msg_activate_clear",(caddr_t)activate_clear },
	{"msg_activate_zoom_in",(caddr_t)activate_zoom_in },
	{"msg_activate_zoom_out",(caddr_t)activate_zoom_out },
	{"msg_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{"msg_activate_help",(caddr_t)activate_help },
	{"msg_activate_help_message",(caddr_t)activate_help_message },
	{"msg_create_form",(caddr_t)create_form }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  *status = 1;
  strcpy( name, msg_name);

  reglist[0].value = (caddr_t) this;

  // Motif
  MrmInitialize();


  // Save the context structure in the widget
  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  dcli_translate_filename( uid_filename, uid_filename);
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell( name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "msg_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

  if ( msg_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel), 
		msg_actions, XtNumber(msg_actions));
    msg_compiled_translations = XtParseTranslationTable( msg_translations);
  }
  XtOverrideTranslations( toplevel, msg_compiled_translations);


  i = 0;
  XtSetArg(args[i],XmNwidth,900);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel, args,i);
    
  XtManageChild( toplevel);

  // Create msgnav
  msgnav = new MsgListMotif( this, form, &nav_widget);
  msgnav->find_wnav_cb = MsgWindow::msgw_find_wnav_cb;
  msgnav->find_plc_cb = MsgWindow::msgw_find_plc_cb;

//  XtManageChild( form_widget);

  XtRealizeWidget( parent_wid);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)activate_exit, this);

  wow = new CoWowMotif( parent_wid);
}


//
//  Delete window
//
MsgWindowMotif::~MsgWindowMotif()
{
  if ( parent_wid)
    XtDestroyWidget( parent_wid);
  if ( msgnav)
    delete msgnav;
}

void MsgWindowMotif::map()
{
  if ( nodraw) {
    deferred_map = 1;
    return;
  }
  else
    deferred_map = 0;

  if ( !displayed)
  {
    flow_MapWidget( parent_wid);
    displayed = 1;
  }
  else
  {
    flow_UnmapWidget( parent_wid);
    flow_MapWidget( parent_wid);
  }
}

void MsgWindowMotif::unmap()
{
  if ( displayed)
  {
    flow_UnmapWidget( parent_wid);
    displayed = 0;
  }
}

void MsgWindowMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  MsgWindow *msgw;

  XtSetArg    (args[0], XmNuserData, &msgw);
  XtGetValues (w, args, 1);

  if ( msgw && msgw->displayed)
    msgw->msgnav->set_input_focus();
}

void MsgWindowMotif::activate_exit( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  flow_UnmapWidget( ((MsgWindowMotif *)msgw)->parent_wid);
  msgw->displayed = 0;
}

void MsgWindowMotif::activate_clear( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  msgw->msgnav->clear();
  msgw->size = 0;
}

void MsgWindowMotif::activate_zoom_in( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  msgw->msgnav->zoom( 1.2);
}

void MsgWindowMotif::activate_zoom_out( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  msgw->msgnav->zoom( 5.0/6);
}

void MsgWindowMotif::activate_zoom_reset( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  msgw->msgnav->unzoom();
}

void MsgWindowMotif::activate_help( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  CoXHelp::dhelp( "messagewindow_refman", 0, navh_eHelpFile_Other, "$pwr_lang/man_dg.dat", 
		  true);
}

void MsgWindowMotif::activate_help_message( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  CoXHelp::dhelp( "message window", 0, navh_eHelpFile_Base, 0, true);
}

void MsgWindowMotif::create_form( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data)
{
  ((MsgWindowMotif *)msgw)->form = w;
}









