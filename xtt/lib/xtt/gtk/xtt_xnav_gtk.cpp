/* 
 * Proview   $Id: xtt_xnav_gtk.cpp,v 1.7 2008-07-17 11:23:07 claes Exp $
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

/* xtt_xnav_gtk.cpp -- Display plant and node hierarchy */

typedef void *Widget;

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_nav_help.h"
#include "rt_gdh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_msg.h"
#include "rt_xnav_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_curvectx.h"

#include "rt_trace_gtk.h"
#include "co_lng.h"
#include "co_error.h"
#include "co_xhelp.h"
#include "co_wow_gtk.h"
#include "co_login_gtk.h"
#include "xtt_xnav_gtk.h"
#include "xtt_item.h"
#include "xtt_menu.h"
#include "xtt_xatt_gtk.h"
#include "xtt_xcrr_gtk.h"
#include "xtt_ge_gtk.h"
#include "xtt_block_gtk.h"
#include "xtt_trend_gtk.h"
#include "xtt_dshist_gtk.h"
#include "xtt_fast_gtk.h"
#include "xtt_xattone_gtk.h"
#include "xtt_clog_gtk.h"
#include "xtt_ev_gtk.h"
#include "xtt_op_gtk.h"
#include "xtt_hist_gtk.h"
#include "ge_curve_gtk.h"
#include "xtt_fileview_gtk.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

//
// Create the navigator widget
//
XNavGtk::XNavGtk( void *xn_parent_ctx,
		  GtkWidget *xn_parent_wid,
		  char *xn_name,
		  GtkWidget **w,
		  xnav_sStartMenu *root_menu,
		  char *xn_opplace_name,
		  int xn_op_close_button,
		  pwr_tStatus *status) :
  XNav( xn_parent_ctx, xn_name, root_menu, xn_opplace_name, xn_op_close_button, status),
  parent_wid(xn_parent_wid)
{

  form_widget = scrolledbrowwidgetgtk_new(
		 init_brow_base_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  displayed = 1;

  // Create the root item
  *w = form_widget;

  menu_tree_build( root_menu);
  gbl.load_config( this);

  for ( int i = 0; i < XNAV_LOGG_MAX; i++)
    logg[i].init( i, (void *)this);

  wow = new CoWowGtk( parent_wid);
  trace_timerid = wow->timer_new();
  *status = 1;
}

//
//  Delete a nav context
//
XNavGtk::~XNavGtk()
{
  closing_down = 1;

  if ( mcp) {
    free( mcp);
    mcp = 0;
  }
  menu_tree_free();
  for ( int i = 0; i < brow_cnt; i++) {
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    brow_stack[i]->free_pixmaps();
    delete brow_stack[i];
  }
  brow_DeleteSecondaryCtx( collect_brow->ctx);
  collect_brow->free_pixmaps();
  delete collect_brow;
  delete brow;
  if ( op)
    delete op;
  gtk_widget_destroy( form_widget);
}

void XNavGtk::set_inputfocus()
{
  if ( displayed) {
    gtk_widget_grab_focus( brow_widget);
  }
}

void XNavGtk::create_popup_menu( pwr_sAttrRef attrref,
				   xmenu_eItemType item_type,
				   xmenu_mUtility caller, unsigned int priv,
				   char *arg, int x, int y)
{
  int x1, y1;

  CoWowGtk::PopupPosition( brow_widget, x + 8, y, &x1, &y1);
  get_popup_menu( attrref, item_type, caller, priv, arg, x1, y1);
}

//
//  Pop xnav window
//
void XNavGtk::pop()
{
  GtkWidget *parent, *top;

  parent = gtk_widget_get_parent( form_widget);
  while( parent) {
    top = parent;
    parent = gtk_widget_get_parent( parent);
  }
  displayed = 1;
  gtk_window_present( GTK_WINDOW(top));
}

RtTrace *XNavGtk::plctrace_new( pwr_tOid oid, pwr_tStatus *sts)
{
  return new RtTraceGtk( this, form_widget, oid, sts);
}

XAtt *XNavGtk::xatt_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts)
{
  return new XAttGtk( form_widget, this, arp, advanced_user, sts);
}

XCrr *XNavGtk::xcrr_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts)
{
  return new XCrrGtk( form_widget, this, arp, advanced_user, sts);
}

Ev *XNavGtk::ev_new( char *eve_name, char *ala_name, char *blk_name,
		      pwr_tObjid ev_user, int display_ala, int display_eve,
		      int display_blk, int display_return, int display_ack,
		      int ev_beep, pwr_tStatus *status)
{
  return new EvGtk( this, parent_wid, eve_name, ala_name, blk_name,
		    ev_user, display_ala, display_eve, display_blk,
		    display_return, display_ack, ev_beep, status);
}

Hist *XNavGtk::hist_new( char *title, pwr_tOid oid, pwr_tStatus *sts)
{
  return new HistGtk( this, parent_wid, title, oid, sts);
}

Block *XNavGtk::block_new( pwr_tAttrRef *arp, char *name, unsigned int priv,
			     pwr_tStatus *sts)
{
  return new BlockGtk( this, parent_wid, arp, name, priv, sts);
}

Op *XNavGtk::op_new( char *opplace, pwr_tStatus *sts)
{
  return new OpGtk( this, parent_wid, opplace, sts);
}

XttTrend *XNavGtk::xtttrend_new( char *name, pwr_tAttrRef *objar, pwr_tAttrRef *plotgroup,
				  pwr_tStatus *sts)
{
  GtkWidget *w;

  return new XttTrendGtk( this, parent_wid, name, &w, objar, plotgroup, sts);
}

XttDsHist *XNavGtk::xttdshist_new( char *name, pwr_tOid *oid, pwr_tOName *aname,
				   sevcli_tCtx scctx, pwr_tStatus *sts)
{
  GtkWidget *w;

  return new XttDsHistGtk( this, parent_wid, name, &w, oid, aname, scctx, sts);
}

XttFast *XNavGtk::xttfast_new( char *name, pwr_tAttrRef *objar, pwr_tStatus *sts)
{
  GtkWidget *w;

  return new XttFastGtk( this, parent_wid, name, &w, objar, sts);
}

XAttOne *XNavGtk::xattone_new( pwr_tAttrRef *objar, char *title, unsigned int priv,
				  pwr_tStatus *sts)
{
  return new XAttOneGtk( parent_wid, this, objar, title, priv, sts);
}

CLog *XNavGtk::clog_new( char *name, pwr_tStatus *sts)
{
  return new CLogGtk( this, parent_wid, name, sts);
}

XttGe *XNavGtk::xnav_ge_new( char *name, char *filename, int scrollbar, int menu, 
			     int navigator, int width, int height, int x, int y, 
			     double scan_time, char *object_name, 
			     int use_default_access, unsigned int access,
			     int (*command_cb) (XttGe *, char *),
			     int (*get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
			     int (*is_authorized_cb) (void *, unsigned int))
{
  return new XttGeGtk( parent_wid, this, name, filename, scrollbar, menu, navigator,
		    width, height, x, y, scan_time, object_name, use_default_access,
		    access, command_cb, get_current_objects_cb, is_authorized_cb);
}

GeCurve *XNavGtk::gecurve_new( char *name, char *filename, GeCurveData *data,
				 int pos_right)
{
  return new GeCurveGtk( this, parent_wid, name, filename, data, pos_right);
}

XttFileview *XNavGtk::fileview_new( pwr_tOid oid, char *title, char *dir, char *pattern,
				    int type, char *target_attr, char *trigger_attr, char *filetype)
{
  return new XttFileviewGtk( this, parent_wid, oid, title, dir, pattern, type, target_attr,
			     trigger_attr, filetype);
}

CoLogin *XNavGtk::login_new( char		*name,
			     char		*groupname,
			     void		(* bc_success)( void *),
			     void		(* bc_cancel)( void *),
			     pwr_tStatus  	*status)
{
  return new CoLoginGtk( this, parent_wid, name, groupname, bc_success, bc_cancel, status);
}

void XNavGtk::bell( int time)
{
  gdk_display_beep( gtk_widget_get_display( brow_widget));
}

void XNavGtk::get_popup_menu( pwr_sAttrRef attrref,
			      xmenu_eItemType item_type, 
			      xmenu_mUtility caller, 
			      unsigned int priv, char *arg, int x, int y)
{
  int 		i;
  GtkWidget 	*popup;

  get_popup_menu_items( attrref, item_type, caller, priv, arg);

  i = 0;
  popup = build_menu( parent_wid, MENU_POPUP, "", mcp, 
		      popup_button_cb, (void *) this, 
		      (xmenu_sMenuItem *) mcp->ItemList, &i); 
  if ( !popup)
    return;

  popupmenu_x = x;
  popupmenu_y = y;
  gtk_menu_popup( GTK_MENU(popup), NULL, NULL, menu_position_func, 
  		  this, 0, gtk_get_current_event_time());
}

void XNavGtk::menu_position_func( GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
				  gpointer data)
{
  XNavGtk *xnav = (XNavGtk *)data;

  *x = xnav->popupmenu_x;
  *y = xnav->popupmenu_y;
  *push_in = FALSE;
}

GtkWidget *XNavGtk::build_menu( GtkWidget *Parent,
				int   MenuType,
				char *MenuTitle,
				void *MenuUserData,
				void (*Callback)( GtkWidget *, gpointer),
				void *CallbackData,
				xmenu_sMenuItem *Items,
				int *idx)
{
  GtkWidget *Menu, *W;
  int i;
  unsigned int Level;

  Menu = (GtkWidget *) g_object_new( GTK_TYPE_MENU, NULL);
  g_object_set_data( (GObject *)Menu, "userdata", (gpointer)MenuUserData);

  Level = Items[*idx].Level;

  for (; Items[*idx].Level != 0 && Items[*idx].Level >= Level; (*idx)++) {
    if (Items[*idx].Item == xmenu_eMenuItem_Cascade ||
      Items[*idx].Item == xmenu_eMenuItem_Ref) {
      if (MenuType == MENU_OPTION) {
        printf("You can't have submenus from option menu items.");
        return NULL;
      } 
      else {
        i = *idx;
	GtkWidget *sub = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8(Items[*idx].Name));
        (*idx)++;	
        W = build_menu(Menu, MENU_PULLDOWN, 
		       Lng::translate( Items[i].Name), MenuUserData, 
		       Callback, CallbackData, Items, idx);
        (*idx)--;
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(sub), GTK_WIDGET(W));
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), sub);
	gtk_widget_show(sub);
      }
    }
    else {
      if (Items[*idx].Item == xmenu_eMenuItem_Separator) {
	// Separator
	W = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), W);
	gtk_widget_show(W);
      }
      else {
	// Pushbutton
	W = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8(Items[*idx].Name));
	gtk_widget_set_sensitive( W, Items[*idx].Flags.f.Sensitive ? TRUE : FALSE);
	g_object_set_data( (GObject *)W, "userdata", (gpointer)*idx);
	if ( Callback)
	  g_signal_connect( W, "activate", 
			    G_CALLBACK(Callback), CallbackData);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), W);
	gtk_widget_show(W);
      } 
    }
  }

  return Menu;
}

void XNavGtk::popup_button_cb( GtkWidget *w, gpointer data)
{
  XNav *xnav = (XNav *)data;
  int idx;
  pwr_tStatus sts;

  idx = (int) g_object_get_data( (GObject *)w, "userdata");

  xnav->mcp->ChosenItem = idx;
  //xnav->set_clock_cursor();
  sts = CallMenuMethod( xnav->mcp, xnav->mcp->ChosenItem);
  if (EVEN(sts))
    xnav->message( 'E', XNav::get_message(sts));
  //xnav->reset_cursor();
}

