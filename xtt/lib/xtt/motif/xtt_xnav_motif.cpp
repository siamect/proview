/* 
 * Proview   $Id: xtt_xnav_motif.cpp,v 1.4 2008-10-31 12:51:36 claes Exp $
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

/* xtt_xnav_motif.cpp -- Display plant and node hierarchy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "pwr.h"
#include "co_nav_help.h"

extern "C" {
#include "pwr_privilege.h"
#include "rt_gdh.h"
#include "rt_gdb.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_api.h"
#include "co_msg.h"
#include "pwr_baseclasses.h"
#include "rt_xnav_msg.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/CascadeBG.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/SeparatoG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_curvectx.h"

#include "flow_x.h"
#include "xtt_trace_motif.h"
#include "cow_mrm_util.h"
#include "co_lng.h"
#include "co_error.h"
#include "cow_xhelp.h"
#include "cow_wow_motif.h"
#include "cow_login_motif.h"
#include "xtt_xnav_motif.h"
#include "xtt_item.h"
#include "xtt_menu.h"
#include "xtt_xatt_motif.h"
#include "xtt_xcrr_motif.h"
#include "xtt_ge_motif.h"
#include "xtt_block_motif.h"
#include "xtt_trend_motif.h"
#include "xtt_fast_motif.h"
#include "xtt_xattone_motif.h"
#include "xtt_clog_motif.h"
#include "xtt_ev_motif.h"
#include "xtt_op_motif.h"
#include "xtt_hist_motif.h"
#include "ge_curve_motif.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

//
// Create the navigator widget
//
XNavMotif::XNavMotif( void *xn_parent_ctx,
		      Widget	xn_parent_wid,
		      const char *xn_name,
		      Widget *w,
		      xnav_sStartMenu *root_menu,
		      char *xn_opplace_name,
		      int xn_op_close_button,
		      pwr_tStatus *status) :
  XNav( xn_parent_ctx, xn_name, root_menu, xn_opplace_name, xn_op_close_button, status),
  parent_wid(xn_parent_wid)
{

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	init_brow_base_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);
  displayed = 1;

  // Create the root item
  *w = form_widget;

  menu_tree_build( root_menu);
  gbl.load_config( this);

  for ( int i = 0; i < XNAV_LOGG_MAX; i++)
    logg[i].init( i, (void *)this);

  wow = new CoWowMotif( parent_wid);
  trace_timerid = wow->timer_new();
  *status = 1;
}

//
//  Delete a nav context
//
XNavMotif::~XNavMotif()
{
  closing_down = 1;

  if ( mcp) {
    free( mcp);
    mcp = 0;
  }
  menu_tree_free();
  for ( int i = 0; i < brow_cnt; i++) {
    brow_stack[i]->free_pixmaps();
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    delete brow_stack[i];
  }
  brow_DeleteSecondaryCtx( collect_brow->ctx);
  delete collect_brow;
  collect_brow->free_pixmaps();
  delete brow;
  if ( op)
    delete op;
  XtDestroyWidget( form_widget);
}

void XNavMotif::set_inputfocus()
{
  // printf( "%d XNav inputfocus %d\n", displayed, (int) brow_widget);
  if ( displayed) {
    XtCallAcceptFocus( brow_widget, CurrentTime);
  }
//  brow_SetInputFocus( brow->ctx);
}

void XNavMotif::create_popup_menu( pwr_sAttrRef attrref,
				   xmenu_eItemType item_type,
				   xmenu_mUtility caller, unsigned int priv,
				   char *arg, int x, int y)
{
  int x1, y1;

  CoWowMotif::PopupPosition( brow_widget, x + 8, y, &x1, &y1);
  get_popup_menu( attrref, item_type, caller, priv, arg, x1, y1);
}

//
//  Pop xnav window
//
static Boolean set_displayed( void *xnav)
{
  ((XNav *)xnav)->displayed = 1;
  return True;
}

void XNavMotif::pop()
{
  Widget parent, top;

  parent = XtParent( parent_wid);
  while( parent)
  {
    top = parent;
    if ( flow_IsShell( top))
      break;
    parent = XtParent( parent);
  }
  displayed = 0;
  flow_UnmapWidget( top);
  flow_MapWidget( top);

  // A fix to avoid a krash in setinputfocus
  XtAppAddWorkProc( XtWidgetToApplicationContext(top),
			(XtWorkProc)set_displayed, (XtPointer)this);
}

RtTrace *XNavMotif::plctrace_new( pwr_tOid oid, pwr_tStatus *sts)
{
  return new RtTraceMotif( this, form_widget, oid, sts);
}

XAtt *XNavMotif::xatt_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts)
{
  return new XAttMotif( form_widget, this, arp, advanced_user, sts);
}

XCrr *XNavMotif::xcrr_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts)
{
  return new XCrrMotif( form_widget, this, arp, advanced_user, sts);
}

Ev *XNavMotif::ev_new( char *eve_name, char *ala_name, char *blk_name,
		       pwr_tObjid ev_user, int display_ala, int display_eve,
		       int display_blk, int display_return, int display_ack,
		       int ev_beep, pwr_tMask ev_pop_mask, int ev_eventname_seg,
		       pwr_tStatus *status)
{
  return new EvMotif( this, parent_wid, eve_name, ala_name, blk_name,
		      ev_user, display_ala, display_eve, display_blk,
		      display_return, display_ack, ev_beep, ev_pop_mask, ev_eventname_seg, status);
}

Hist *XNavMotif::hist_new( char *title, pwr_tOid oid, pwr_tStatus *sts)
{
  return new HistMotif( this, parent_wid, title, oid, sts);
}

Block *XNavMotif::block_new( pwr_tAttrRef *arp, char *name, unsigned int priv,
			     pwr_tStatus *sts)
{
  return new BlockMotif( this, parent_wid, arp, name, priv, sts);
}

Op *XNavMotif::op_new( char *opplace, pwr_tStatus *sts)
{
  return new OpMotif( this, parent_wid, opplace, sts);
}

XttTrend *XNavMotif::xtttrend_new( char *name, pwr_tAttrRef *objar, pwr_tAttrRef *plotgroup,
				  pwr_tStatus *sts)
{
  Widget w;

  return new XttTrendMotif( this, parent_wid, name, &w, objar, plotgroup, sts);
}

XttFast *XNavMotif::xttfast_new( char *name, pwr_tAttrRef *objar, pwr_tStatus *sts)
{
  Widget w;

  return new XttFastMotif( this, parent_wid, name, &w, objar, sts);
}

XAttOne *XNavMotif::xattone_new( pwr_tAttrRef *objar, char *title, unsigned int priv,
				  pwr_tStatus *sts)
{
  return new XAttOneMotif( parent_wid, this, objar, title, priv, sts);
}

CLog *XNavMotif::clog_new( const char *name, pwr_tStatus *sts)
{
  return new CLogMotif( this, parent_wid, name, sts);
}

XttGe *XNavMotif::xnav_ge_new( const char *name, const char *filename, int scrollbar, int menu, 
			       int navigator, int width, int height, int x, int y, 
			       double scan_time, const char *object_name, 
			       int use_default_access, unsigned int access,
			       int (*command_cb) (XttGe *, char *),
			       int (*get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
			       int (*is_authorized_cb) (void *, unsigned int))
{
  return new XttGeMotif( parent_wid, this, name, filename, scrollbar, menu, navigator,
			 width, height, x, y, scan_time, object_name, use_default_access,
			 access, command_cb, get_current_objects_cb, is_authorized_cb);
}

GeCurve *XNavMotif::gecurve_new( char *name, char *filename, GeCurveData *data,
				 int pos_right)
{
  return new GeCurveMotif( this, parent_wid, name, filename, data, pos_right);
}

CoLogin *XNavMotif::login_new( const char      	*name,
			       const char      	*groupname,
			       void		(* bc_success)( void *),
			       void		(* bc_cancel)( void *),
			       pwr_tStatus  	*status)
{
  return new CoLoginMotif( this, parent_wid, name, groupname, bc_success, bc_cancel, status);
}

void XNavMotif::bell( int time)
{
  XBell( flow_Display( brow_widget), time);
}

void XNavMotif::get_popup_menu( pwr_sAttrRef attrref,
				xmenu_eItemType item_type, 
				xmenu_mUtility caller, 
				unsigned int priv, char *arg, int x, int y)
{
  int 		i;
  Widget 	popup;
  Arg		args[2];

  get_popup_menu_items( attrref, item_type, caller, priv, arg);

  i = 0;
  popup = build_menu( parent_wid, MENU_POPUP, "", mcp, 
		      popup_button_cb, (void *) this, 
		      (xmenu_sMenuItem *) mcp->ItemList, &i); 
  if (popup != NULL) 
    XtAddCallback( popup, XmNunmapCallback, 
		   (XtCallbackProc)popup_unmap_cb, mcp);

  XtSetArg(args[0], XmNx, x);
  XtSetArg(args[1], XmNy, y);
  XtSetValues( popup, args, 2);
  XtManageChild(popup);
}

Widget XNavMotif::build_menu( Widget Parent,
			      int   MenuType,
			      const char *MenuTitle,
			      void *MenuUserData,
			      void (*Callback)( Widget, XNav *, XmAnyCallbackStruct *),
			      void *CallbackData,
			      xmenu_sMenuItem *Items,
			      int *idx)
{
  Widget Menu, Cascade, W;
  int i;
  unsigned int Level;
  XmString Str;
  WidgetClass Class;
  Arg ArgList[5]; 
  XmFontList fontlist;
  XFontStruct *font;
  XmFontListEntry fontentry;

  // Set default fontlist
  font = XLoadQueryFont( flow_Display(Parent),
  	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
  fontentry = XmFontListEntryCreate( (char*) "tag1", XmFONT_IS_FONT, font);
  fontlist = XmFontListAppendEntry( NULL, fontentry);
  XtFree( (char *)fontentry);

  i = 0;
  XtSetArg(ArgList[i], XmNuserData, MenuUserData); i++;
  XtSetArg(ArgList[i], XmNbuttonFontList, fontlist); i++;
  XtSetArg(ArgList[i], XmNlabelFontList, fontlist); i++;
  if (MenuType == MENU_PULLDOWN || MenuType == MENU_OPTION)
    Menu = XmCreatePulldownMenu(Parent, (char*) "_pulldown", ArgList, i);
  else if (MenuType == MENU_POPUP)
    Menu = XmCreatePopupMenu(Parent, (char*) "_popup", ArgList, i);  
  else  {
    XtWarning("Invalid menu type passed to BuildMenu().");
    return NULL;
  }

  if (MenuType == MENU_PULLDOWN) {
    Str = XmStringCreateSimple((char*) MenuTitle);	
    Cascade = XtVaCreateManagedWidget(MenuTitle,
	    xmCascadeButtonGadgetClass, Parent,
	    XmNsubMenuId,   Menu,
	    XmNlabelString, Str,
	    NULL);
    XmStringFree(Str);
  } 
  else if (MenuType == MENU_OPTION) {
    Str = XmStringCreateSimple((char*) MenuTitle);
    XtSetArg(ArgList[0], XmNsubMenuId, Menu);
    XtSetArg(ArgList[1], XmNlabelString, Str);
    Cascade = XmCreateOptionMenu(Parent, (char*) MenuTitle, ArgList, 2);
    XmStringFree(Str);
  }

  XmFontListFree( fontlist);

  Level = Items[*idx].Level;

  for (; Items[*idx].Level != 0 && Items[*idx].Level >= Level; (*idx)++) {
    if (Items[*idx].Item == xmenu_eMenuItem_Cascade ||
      Items[*idx].Item == xmenu_eMenuItem_Ref) {
      if (MenuType == MENU_OPTION) {
        XtWarning("You can't have submenus from option menu items.");
        return NULL;
      } 
      else {
        i = *idx;
        (*idx)++;	
        build_menu(Menu, MENU_PULLDOWN, 
		   Lng::translate( Items[i].Name), MenuUserData, 
		   Callback, CallbackData, Items, idx);
        (*idx)--;
      }
    }
    else {
      if (Items[*idx].Item == xmenu_eMenuItem_Separator)
        Class = xmSeparatorGadgetClass;
      else
        Class = xmPushButtonGadgetClass;
 
      W = XtVaCreateManagedWidget(Lng::translate( Items[*idx].Name), 
		    Class, Menu,
		    XmNuserData, *idx,
		    XmNsensitive, (Boolean)(Items[*idx].Flags.f.Sensitive == 1),
		    NULL);

      if (Callback && Class == xmPushButtonGadgetClass)
        XtAddCallback(W, XmNactivateCallback, (XtCallbackProc) Callback, 
		      (XtPointer) CallbackData);
    }
  }

  return MenuType == MENU_POPUP ? Menu : Cascade;
}

void XNavMotif::popup_unmap_cb(Widget w, xmenu_sMenuCall *ip, XmAnyCallbackStruct *data)
{
  // XtFree( (char *)ip);
  XtDestroyWidget(w);
}

void XNavMotif::popup_button_cb( Widget w, XNav *xnav, XmAnyCallbackStruct *data)
{
  Widget menu;
  int idx;
  pwr_tStatus sts;

  // Find the menu widget
  menu = XtParent(w);
  while (1) {
    if (strcmp(XtName(menu), "_popup") == 0 || 
	  strcmp(XtName(menu), "_pulldown") == 0)
      break;
    menu = XtParent(menu);
  }

  XtVaGetValues (w, XmNuserData, &idx, NULL);

  mcp->ChosenItem = idx;
  // xnav->set_clock_cursor();
  sts = CallMenuMethod( mcp, mcp->ChosenItem);
  if (EVEN(sts))
    xnav->message( 'E', XNav::get_message(sts));
  // xnav->reset_cursor();

}

