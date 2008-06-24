/* 
 * Proview   $Id: wb_wnav_gtk.cpp,v 1.3 2008-06-24 07:52:21 claes Exp $
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

/* wb_wnav_gtk.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

extern "C" {
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_msg.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_wnav_msg.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_wccm.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "wb_wnav_gtk.h"
#include "wb_wnav_item.h"
#include "co_wow_gtk.h"
#include "co_login_gtk.h"
#include "wb_wge_gtk.h"
#include "ge_gtk.h"
#include "wb_wda_gtk.h"
#include "wb_foe_gtk.h"
#include "wb_utl_gtk.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

//
//  Pop wnav window
//

void WNavGtk::pop()
{
  GtkWidget *parent, *top;

  parent = gtk_widget_get_parent( toplevel);
  while( parent) {
    top = parent;
    parent = gtk_widget_get_parent( parent);
  }
  gtk_window_present( GTK_WINDOW(top));
}

//
// Create the navigator widget
//
WNavGtk::WNavGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	char *xn_layout,
	GtkWidget **w,
        ldh_tSesContext	xn_ldhses,
	wnav_sStartMenu *root_menu,
	wnav_eWindowType xn_type,
	pwr_tStatus *status) :
  WNav( xn_parent_ctx, xn_name, xn_layout, xn_ldhses, root_menu, xn_type, status),
  parent_wid(xn_parent_wid), trace_timerid(0)
{
  if ( window_type == wnav_eWindowType_No)
    return;

  GtkWidget *scrolledbrow = scrolledbrowwidgetgtk_new( WNav::init_brow_base_cb, 
					   this, &brow_widget);

  form_widget = gtk_frame_new( NULL);
  gtk_container_add( GTK_CONTAINER(form_widget), scrolledbrow);
  gtk_container_set_border_width( GTK_CONTAINER(scrolledbrow), 3);
  gtk_widget_show_all( brow_widget);

  // Create the root item
  *w = form_widget;

  CoWowGtk::GetAtoms( &graph_atom, &objid_atom, &attrref_atom);
  selection_widget = gtk_invisible_new();
  gtk_selection_add_target( selection_widget, GDK_SELECTION_PRIMARY,
			    GDK_SELECTION_TYPE_STRING, 1);
  gtk_selection_add_target( selection_widget, GDK_SELECTION_PRIMARY, graph_atom, 1);
  gtk_selection_add_target( selection_widget, GDK_SELECTION_PRIMARY, objid_atom, 1);
  gtk_selection_add_target( selection_widget, GDK_SELECTION_PRIMARY, attrref_atom, 1);
  g_signal_connect( selection_widget, "selection-get", 
		    G_CALLBACK( WNavGtk::sel_convert_cb), this);
  sel_lose_id = g_signal_connect( selection_widget, "selection-clear-event", 
		    G_CALLBACK( WNavGtk::sel_lose_cb), this);

  gbl.load_config( this);

  if ( root_menu && !ldhses)
    menu_tree_build( root_menu);

  wow = new CoWowGtk( parent_wid);

  *status = 1;
}

//
//  Delete a nav context
//
WNavGtk::~WNavGtk()
{
  closing_down = 1;

  g_signal_handler_disconnect( selection_widget, sel_lose_id);

  if ( trace_started)
    g_source_remove( trace_timerid);

  menu_tree_free();
  PalFile::config_tree_free( menu);
  for ( int i = 1; i < brow_cnt; i++)
  {
    brow_DeleteSecondaryCtx( brow_stack[brow_cnt]->ctx);
    brow_stack[brow_cnt]->free_pixmaps();
    delete brow_stack[i];
  }
  delete brow;
  delete wow;
  gtk_widget_destroy( form_widget);
}

void WNavGtk::set_inputfocus( int focus)
{
  if ( !displayed)
    return;

  if ( !focus) {
    GdkColor color;

    gdk_color_parse( "White", &color);
    gtk_widget_modify_bg( form_widget, GTK_STATE_NORMAL, &color);
  }
  else {
    GdkColor color;

    gdk_color_parse( "Black", &color);
    gtk_widget_modify_bg( form_widget, GTK_STATE_NORMAL, &color);
    gtk_widget_grab_focus( brow_widget);
  }
}

void WNavGtk::trace_start()
{
  WNavGtk::trace_scan( this);
}

static gboolean wnavgtk_trace_scan( void *data)
{
  WNavGtk::trace_scan( (WNavGtk *)data);
  return FALSE;
}

void WNavGtk::trace_scan( WNavGtk *wnav)
{
  int time = 1000;

  if ( wnav->trace_started)
  {
    brow_TraceScan( wnav->brow->ctx);

    wnav->trace_timerid = g_timeout_add( time, 
					 wnavgtk_trace_scan, wnav);
  }
}

int WNavGtk::get_selection( char *str, int len)
{
  int sts;

  sts = CoWowGtk::GetSelection( form_widget, str, len, objid_atom);
  if ( EVEN(sts))
    sts = CoWowGtk::GetSelection( form_widget, str, len, GDK_TARGET_STRING);

  return sts;
}

void WNavGtk::set_selection_owner()
{
  gboolean sts;

  sts = gtk_selection_owner_set( selection_widget, GDK_SELECTION_PRIMARY,
			   gtk_get_current_event_time());
  if ( !sts) {
     message('E', "Failed attempting to become primary selection owner");
     brow_SelectClear( brow->ctx);
     return;
  }	
  selection_owner = 1;
}

void WNavGtk::create_popup_menu( pwr_tAttrRef aref, int x, int y)
{
#if 0
  short x1, y1;
  Arg arg[2];

  if ( avoid_deadlock)
    return;

  XtSetArg( arg[0], XmNx, &x1);
  XtSetArg( arg[1], XmNy, &y1);
  XtGetValues( XtParent(brow_widget), arg, 2);

  (create_popup_menu_cb)( parent_ctx, aref, x + x1, y + y1);
  wnav_set_avoid_deadlock( this, 2000);
#endif
  (create_popup_menu_cb)( parent_ctx, aref, x, y);
}

Ge *WNavGtk::ge_new( char *graph_name)
{
  GeGtk *ge = new GeGtk( NULL, toplevel, ldhses, 0, graph_name);
  return ge;
}

WGe *WNavGtk::wge_new( char *name, char *filename, char *object_name, int modal) 
{ 
  WGe *wge = new WGeGtk( parent_wid, this, name, filename, 0,0,0,0,0,0,0, object_name,
			 modal);
  return wge;
}

wb_utl *WNavGtk::utl_new()
{
  wb_utl_gtk *utl = new wb_utl_gtk( parent_wid);
  return utl;
}

void WNavGtk::sel_convert_cb( GtkWidget  *w, GtkSelectionData *selection_data,
				 guint info, guint time_stamp, gpointer data)
{
  WNavGtk    	*wnav = (WNavGtk *)data;
  int 		sts;
  int		size;
  WItem		*item;
  pwr_tAName   	attr_str;
  pwr_sAttrRef attrref;
  char          name[200];
  char		*buffp;

  if ( selection_data->target == GDK_TARGET_STRING ||
       selection_data->target == wnav->graph_atom ||
       selection_data->target == wnav->objid_atom ||
       selection_data->target == wnav->attrref_atom) {
    brow_tNode	*node_list;
    int		node_count;
    wnav_eSelectionFormat format;
  
    if ( selection_data->target == wnav->graph_atom)
      format = wnav_eSelectionFormat_Graph;
    else if ( selection_data->target == wnav->objid_atom)
      format = wnav_eSelectionFormat_Objid;
    else if ( selection_data->target == wnav->attrref_atom)
      format = wnav_eSelectionFormat_Attrref;
    else
      format = wnav_eSelectionFormat_User;
	

    brow_GetSelectedNodes( wnav->brow->ctx, &node_list, &node_count);
    if ( !node_count)
      return;

    brow_GetUserData( node_list[0], (void **)&item);

    switch( item->type) {
    case wnav_eItemType_Attr:
    case wnav_eItemType_AttrInput:
    case wnav_eItemType_AttrInputInv:
    case wnav_eItemType_AttrInputF:
    case wnav_eItemType_AttrOutput:
    case wnav_eItemType_AttrArray:
    case wnav_eItemType_AttrArrayOutput:
    case wnav_eItemType_AttrArrayElem:
    case wnav_eItemType_AttrObject: {
      WItemBaseAttr *aitem = (WItemBaseAttr *)item;

      sts = ldh_ObjidToName( wnav->ldhses, item->objid, ldh_eName_Hierarchy,
	    	attr_str, sizeof(attr_str), &size);
      if ( EVEN(sts)) break;

      strcat( attr_str, ".");
      strcat( attr_str, aitem->name);
      sts = ldh_NameToAttrRef( wnav->ldhses, attr_str, &attrref);
      if ( EVEN(sts)) break;
      sts = (wnav->format_selection_cb)( wnav->parent_ctx, attrref, 
					 &buffp, 0, 1, format);
      if ( sts) {
	gtk_selection_data_set( selection_data, GDK_SELECTION_TYPE_STRING,
				8, (const guchar *)buffp, strlen(buffp));
	free( node_list);
	return;
      }
      break;
    }
    case wnav_eItemType_Object:
      memset( &attrref, 0, sizeof(attrref));
      attrref.Objid = item->objid;
      sts = (wnav->format_selection_cb)( wnav->parent_ctx, attrref, 
					 &buffp, 0, 0, format);
      if ( sts) {
	gtk_selection_data_set( selection_data, GDK_SELECTION_TYPE_STRING,
				8, (const guchar *)buffp, strlen(buffp));
	free( node_list);
	return;
      }
    default:
      brow_GetAnnotation( node_list[0], 0, name, sizeof(name));
      gtk_selection_data_set( selection_data, GDK_SELECTION_TYPE_STRING,
			      8, (const guchar *)name, strlen(name));
      free( node_list);
      return;
    }
    free( node_list);
  }
  strcpy( name, "");
  gtk_selection_data_set( selection_data, GDK_SELECTION_TYPE_STRING,
			  8, (const guchar *)name, strlen(name));
}

void WNavGtk::sel_lose_cb( GtkWidget *w, GdkEventSelection *event,
			   gpointer data)
{
  WNavGtk    	*wnav = (WNavGtk *)data;

  brow_SelectClear( wnav->brow->ctx);
  wnav->selection_owner = 0;
}

int WNavGtk::open_foe( char *name, pwr_tOid plcpgm,
		       void **foectx, int map_window,
		       ldh_eAccess access, pwr_tOid oid)
{
  pwr_tStatus sts;
  WFoe *foe;

  foe = WFoe::get( plcpgm);
  if ( foe)
    foe->pop();
  else 
    foe = new WFoeGtk( (void *)parent_ctx, parent_wid, name,
			 plcpgm, wbctx, ldhses,
			 map_window, access, &sts);
  if ( EVEN(sts)) return sts;

  if ( cdh_ObjidIsNotNull( oid))
    foe->center_object(oid);
  *foectx = foe;
  return sts;
}

void WNavGtk::wda_new( pwr_tOid oid, pwr_tCid cid, char *attribute,
			 int edit_mode, int advuser, int display_objectname)
{
  new WdaGtk( parent_wid, this, ldhses, oid, cid, attribute, edit_mode, 
	      advuser, display_objectname);
}

CoLogin *WNavGtk::login_new( char		*name,
			     char		*groupname,
			     void		(* bc_success)( void *),
			     void		(* bc_cancel)( void *),
			     pwr_tStatus  	*status)
{
  return new CoLoginGtk( this, parent_wid, name, groupname, bc_success, bc_cancel, status);
}

void WNavGtk::wge_subwindow_loop( WGe *wge)
{
  gtk_main();
#if 0
  XEvent 	Event;
  
  for (;;) {
    XtAppNextEvent( XtWidgetToApplicationContext( parent_wid), &Event);
    XtDispatchEvent( &Event);
	  
    if ( wge->subwindow_release) {
      wge->subwindow_release = 0;
      break;
    }
  }
#endif
}

void WNavGtk::wge_modal_loop( WGe *wge)
{
  gtk_main();
#if 0
  XEvent 	Event;
	  
  for (;;) {
    XtAppNextEvent( XtWidgetToApplicationContext( parent_wid), &Event);
    XtDispatchEvent( &Event);

    if ( wge->terminated) {
      appl.remove( (void *)wge);
      delete wge;
      break;      
    }
  }
#endif
}

#if 0
static char *wnav_dialog_convert_text( char *text)
{
  char *s, *t;
  static char new_text[200];

  for ( s = text, t = new_text; *s; s++, t++)
  {
    if ( *s == '\\' && *(s+1) == 'n')
    {
      *t = 10;
      s++;
    }
    else
     *t = *s;
    if ( t > &new_text[sizeof(new_text)-1])
      break;
  }
  *t = *s;
  return new_text;
}
#endif

static void wnav_message_dialog_ok( GtkWidget *w, gint arg1, gpointer wnav)
{
  ((WNav *)wnav)->dialog_ok = 1;
  gtk_main_quit();
}

static void wnav_continue_dialog_ok( GtkWidget *w, gpointer wnav)
{
  ((WNav *)wnav)->dialog_ok = 1;
  gtk_main_quit();
}

static void wnav_continue_dialog_cancel( GtkWidget *w, gpointer wnav)
{
  ((WNav *)wnav)->dialog_cancel = 1;
  gtk_main_quit();
}

static gboolean wnav_message_dialog_delete_event( GtkWidget *w, GdkEvent *event, gpointer wnav)
{
  ((WNav *)wnav)->dialog_cancel = 1;
  gtk_main_quit();
  return TRUE;
}

static void wnav_message_dialog_read( GtkWidget *w, gpointer data)
{
  WNav *wnav = (WNav *)data;
  char *value;

  wnav->dialog_ok = 1;
  value = gtk_editable_get_chars( GTK_EDITABLE(((WNavGtk *)wnav)->message_dialog_entry), 
				  0, -1);
  strncpy( wnav->dialog_value, value, sizeof(wnav->dialog_value));
  g_free( value);
  gtk_main_quit();
}

void WNavGtk::message_dialog( char *title, char *text)
{
  GtkWidget *dialog = gtk_message_dialog_new( GTK_WINDOW(toplevel),
					      GTK_DIALOG_MODAL, 
					      GTK_MESSAGE_ERROR,
					      GTK_BUTTONS_OK, text);
  g_signal_connect( dialog, "response", 
 		    G_CALLBACK(wnav_message_dialog_ok), this);
  gtk_window_set_title( GTK_WINDOW(dialog), title);
  gtk_widget_show_all( dialog);

  gtk_main();
  gtk_widget_destroy( dialog);
}

static void wnav_confirm_dialog_ok( void *ctx, void *data)
{
  ((WNav *)ctx)->dialog_ok = 1;
  gtk_main_quit();
}

static void wnav_confirm_dialog_cancel( void *ctx, void *data)
{
  ((WNav *)ctx)->dialog_cancel = 1;
  gtk_main_quit();
}

int WNavGtk::confirm_dialog( char *title, char *text, int display_cancel,
		int *cancel)
{
  dialog_ok = 0;
  dialog_cancel = 0;
  wow->DisplayQuestion( this, title, text, wnav_confirm_dialog_ok,
			wnav_confirm_dialog_cancel, 0);

  gtk_main();

  if ( dialog_ok) {
    if ( display_cancel)
      *cancel = 0;
    return 1;
  }
  if ( dialog_cancel) {
    if ( display_cancel)
      *cancel = 0;
    return 0;
  }
  return 0;
}

int WNavGtk::continue_dialog( char *title, char *text)
{
  // Create a question window
  GtkWidget *question_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						"default-height", 150,
						"default-width", 400,
						"title", title,
						NULL);
  g_signal_connect( question_widget, "delete_event", G_CALLBACK(wnav_message_dialog_delete_event), this);
  GtkWidget *question_label = gtk_label_new(text);
  GtkWidget *question_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *question_ok = gtk_button_new_with_label( "Continue");
  gtk_widget_set_size_request( question_ok, 70, 25);
  g_signal_connect( question_ok, "clicked", 
  		    G_CALLBACK(wnav_continue_dialog_ok), this);

  GtkWidget *question_no = gtk_button_new_with_label( "Quit");
  gtk_widget_set_size_request( question_no, 70, 25);
  g_signal_connect( question_no, "clicked", 
  		    G_CALLBACK(wnav_continue_dialog_cancel), this);

  GtkWidget *question_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(question_hboxtext), question_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(question_hboxtext), question_label, TRUE, TRUE, 15);

  GtkWidget *question_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(question_hboxbuttons), question_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(question_hboxbuttons), question_no, FALSE, FALSE, 0);

  GtkWidget *question_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(question_vbox), question_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(question_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(question_vbox), question_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(question_widget), question_vbox);
  gtk_widget_show_all( question_widget);

  dialog_ok = 0;
  dialog_cancel = 0;
  dialog_help = 0;
  gtk_main();

  gtk_widget_destroy( question_widget);
  if ( dialog_ok)
    return 1;
  if ( dialog_cancel)
    return 0;
  return 0;
}

int WNavGtk::prompt_dialog( char *title, char *text, char **value)
{
  // Create an input dialog
  GtkWidget *india_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
							"default-height", 150,
							"default-width", 350,
							"title", title,
							NULL);
  g_signal_connect( india_widget, "delete_event", G_CALLBACK(wnav_message_dialog_delete_event), this);
  message_dialog_entry = gtk_entry_new();
  GtkWidget *india_label = gtk_label_new(text);
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(wnav_message_dialog_read), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(wnav_confirm_dialog_cancel), this);

  GtkWidget *india_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_label, FALSE, FALSE, 15);
  gtk_box_pack_end( GTK_BOX(india_hboxtext), message_dialog_entry, TRUE, TRUE, 30);

  GtkWidget *india_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(india_hboxbuttons), india_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_hboxbuttons), india_cancel, FALSE, FALSE, 0);

  GtkWidget *india_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vbox), india_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(india_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_vbox), india_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(india_widget), india_vbox);
  gtk_widget_show_all( india_widget);

  gtk_main();

  if ( dialog_ok) {
    *value = dialog_value;
    gtk_widget_destroy( india_widget);
    return 1;
  }
  if ( dialog_cancel) {
    strcpy( dialog_value, "");
    *value = dialog_value;
    gtk_widget_destroy( india_widget);
    return 0;
  }
  return 0;
#if 0
  Widget 	dialog;
  XmString	text_str;
  XmString	title_str;
  Arg		args[10];
  int		i;
  XEvent 	Event;

  text_str = XmStringCreateLocalized( wnav_dialog_convert_text(text));
  title_str = XmStringCreateLocalized( title);
  i = 0;
  XtSetArg( args[i], XmNselectionLabelString, text_str); i++;
  XtSetArg( args[i], XmNdialogTitle, title_str); i++;
  if ( dialog_width && dialog_height)
  {
    XtSetArg( args[i], XmNwidth, dialog_width); i++;
    XtSetArg( args[i], XmNheight, dialog_height); i++;
    XtSetArg( args[i], XmNx, dialog_x); i++;
    XtSetArg( args[i], XmNy, dialog_y); i++;
  }
//  XtSetArg( args[i], XmNautoUnmanage, False); i++;

  dialog = XmCreatePromptDialog( parent_wid, "Info", args, i);
  XmStringFree( text_str);
  XmStringFree( title_str);

  XtUnmanageChild( XmSelectionBoxGetChild( dialog, XmDIALOG_HELP_BUTTON));

  XtAddCallback( dialog, XmNokCallback, wnav_message_dialog_read, this);
  XtAddCallback( dialog, XmNcancelCallback, wnav_message_dialog_cancel, this);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_cancel, this);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

  dialog_ok = 0;
  dialog_cancel = 0;
  dialog_help = 0;

  for (;;)
  {
    XtAppNextEvent( XtWidgetToApplicationContext( dialog), &Event);
    XtDispatchEvent( &Event);
    if ( dialog_ok)
    {
      *value = dialog_value;
      XtDestroyWidget( dialog);
      return 1;
    }
    if ( dialog_cancel)
    {
      strcpy( dialog_value, "");
      *value = dialog_value;
      XtDestroyWidget( dialog);
      return 0;
    }
  }
#endif
  return 0;
}

