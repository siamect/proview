/* 
 * Proview   $Id: wb_foe_gtk.cpp,v 1.11 2007-09-26 11:51:46 claes Exp $
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

//   wb_foe_gtk.cpp
//   This module creates function object editor and handles
//   the graphical editor.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_browapi.h"
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "wb_watt_gtk.h"
#include "wb_gre_gtk.h"
#include "wb_foe_gtk.h"
#include "wb_login.h"
#include "wb_pal_gtk.h"
#include "wb_nav_gtk.h"
#include "co_wow_gtk.h"

//	Callback from the menu.
void WFoeGtk::activate_save( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_save();
}

//	Callback from the menu.
void WFoeGtk::activate_exit( GtkWidget *w, gpointer data)
{
}

//	Callback from the menu.
void WFoeGtk::activate_winddelete( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_winddelete();
}


//	Callback from the menu.
void WFoeGtk::activate_quit( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_quit();
}

//	Callback from the menu.
void WFoeGtk::activate_savetrace( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_savetrace();
}

void WFoeGtk::activate_restoretrace( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_restoretrace();
}

//	Callback from the menu.
void WFoeGtk::activate_print( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_print();
}

//	Callback from the menu.
void WFoeGtk::activate_printoverv( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_printoverv();
}

//	Callback from the menu.
void WFoeGtk::activate_printselect( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_printselect();
}

//	Callback from the menu.
void WFoeGtk::activate_printpdf( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_printpdf();
}

//	Callback from the menu.
void WFoeGtk::activate_syntax( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_syntax();
}

//	Callback from the menu.
void WFoeGtk::activate_compile( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_compile();
}

//	Callback from the menu.
void WFoeGtk::activate_delete( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_delete();
}

void WFoeGtk::activate_createobject( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  double x, y;

  foe->gre->pixel_to_position( foe->gre->popup_menu_x, foe->gre->popup_menu_y, &x, &y);
  foe->activate_createobject( (float)x, (float)y);
}

//	Callback from the menu.
void WFoeGtk::activate_changetext( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_changetext();
}

//	Callback from the menu.
void WFoeGtk::activate_cut( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_cut();
}

//	Callback from the menu.
void WFoeGtk::activate_copy( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_copy();
}

//	Callback from the menu.
void WFoeGtk::activate_paste( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_paste();
}

void WFoeGtk::activate_select_nextright(GtkWidget *w, gpointer foe)
{
  ((WFoe *)foe)->activate_select_nextobject( flow_eDirection_Right);
}

void WFoeGtk::activate_select_nextleft(GtkWidget *w, gpointer foe)
{
  ((WFoe *)foe)->activate_select_nextobject( flow_eDirection_Left);
}

void WFoeGtk::activate_select_nextup(GtkWidget *w, gpointer foe)
{
  ((WFoe *)foe)->activate_select_nextobject( flow_eDirection_Up);
}

void WFoeGtk::activate_select_nextdown(GtkWidget *w, gpointer foe)
{
  ((WFoe *)foe)->activate_select_nextobject( flow_eDirection_Down);
}


//	Callback from the menu.
void WFoeGtk::activate_attribute( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_attribute();
}


//	Callback from the menu.
void WFoeGtk::activate_subwindow( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_subwindow();
}

//	Callback from the menu.
void WFoeGtk::activate_undelete( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_undelete();
}

//	Callback from the menu.
void WFoeGtk::activate_unselect( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_unselect();
}

//
//	Callback from the menu.
//	Sets gridsize.
//
void WFoeGtk::activate_gridsize1( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->gre->set_grid_size( 0.01);
}

void WFoeGtk::activate_gridsize2( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->gre->set_grid_size( 0.02);
}

void WFoeGtk::activate_gridsize3( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->gre->set_grid_size( 0.05);
}

void WFoeGtk::activate_gridsize4( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->gre->set_grid_size( 0.10);
}

void WFoeGtk::activate_gridsize5( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->gre->set_grid_size( 0.20);
}

//
//	Callback from the menu.
//	Draws or deletes the grid.
//

void WFoeGtk::activate_showgrid( GtkWidget *w, gpointer data)
{
#if 0
  if ( data->set > 1) return;
  if ( foe->msg_label_id != 0 ) foe->message( ""); 

  if(data->set == 1) {
    foe->gre->drawgrid();
  }
  else  /* data->set == 0 */ {
    foe->gre->undrawgrid();
  }
#endif
}

//	Callback from the menu.
void WFoeGtk::activate_redraw( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_redraw();
}

//	Callback from the menu.
void WFoeGtk::activate_zoomin( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_zoomin();
}

//	Callback from the menu.
void WFoeGtk::activate_zoomout( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_zoomout();
}

//	Callback from the menu.
void WFoeGtk::activate_unzoom( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_unzoom();
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeGtk::activate_palette_con( GtkWidget *w, gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;
  int set;

  if ( w != foe->widgets.view_conpal)
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_conpal), TRUE);
  set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_conpal));

  if ( set)
    g_object_set( ((WFoeGtk *)foe)->widgets.con_palette, "visible", TRUE, NULL);
  else
    g_object_set( ((WFoeGtk *)foe)->widgets.con_palette, "visible", FALSE, NULL);
  foe->con_palette_managed = set;
  foe->con_palctx->set_inputfocus( set);
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeGtk::activate_palette_object( GtkWidget *w, gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;
  int set;

  if ( w != foe->widgets.view_objectpal)
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_objectpal), TRUE);
  set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_objectpal));

  if ( set)
    g_object_set( ((WFoeGtk *)foe)->widgets.node_palette, "visible", TRUE, NULL);
  else
    g_object_set( ((WFoeGtk *)foe)->widgets.node_palette, "visible", FALSE, NULL);
  foe->node_palette_managed = set;
  foe->node_palctx->set_inputfocus( set);
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeGtk::activate_palette_plant( GtkWidget *w, gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;
  int set;

  if ( w != foe->widgets.view_plantpal)
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_plantpal), TRUE);
  set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(foe->widgets.view_plantpal));

  if ( set)
    g_object_set( ((WFoeGtk *)foe)->widgets.nav_palette, "visible", TRUE, NULL);
  else
    g_object_set( ((WFoeGtk *)foe)->widgets.nav_palette, "visible", FALSE, NULL);
  foe->nav_palette_managed = set;
  foe->navctx->set_inputfocus( set);
}

// 	Callback from the menu.
void WFoeGtk::activate_refcon( GtkWidget *w, gpointer data)
{ 
  WFoe *foe = (WFoe *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  foe->activate_refcon( set);
}

// 	Callback from the menu.
void WFoeGtk::activate_showexeord( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  foe->activate_showexeord( set);
}


//	Callback from the menu.
void WFoeGtk::activate_plcattribute( GtkWidget *w, gpointer data)
{  
  WFoe *foe = (WFoe *)data;

  foe->activate_plcattribute();
}

//	Callback from the menu.
//	Not yet implemented.
void WFoeGtk::activate_search( GtkWidget *w, gpointer data)
{
}

//	Callback from the menu.
void WFoeGtk::activate_searchobject( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_searchobject();
}

//	Callback from the menu.
void WFoeGtk::activate_searchstring( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_searchstring();
}

//	Callback from the menu.
void WFoeGtk::activate_searchnext( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_searchnext();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeGtk::activate_getobj( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_getobj();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeGtk::activate_expand( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_expand();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeGtk::activate_compress( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_compress();
}

//
//	Display help window for topic plc editor
//
void WFoeGtk::activate_help( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_help();
}

//
//	Display class help for selected object
//
void WFoeGtk::activate_helpclass( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  foe->activate_helpclass();
}

//	Callbacks from the toggle buttons under the function entry.
//	This callbacks are called when the value of a toggle button is changed.
//	The function pulldown menu is a radio box which ensure that 
//	one and only one toggle button is set at any time:
//	If the callback is generated by the radio entry 
//	(and not directly by the user ) the event is 0.
//
//	These callbacks are use to determine in which mode the user works: 
// 	SG 01.05.91
//
void WFoeGtk::activate_trace_togg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  foe->activate_trace_togg( set);
}

//
//	Callbacks from the toggle buttons under the customize entry.
//
void WFoeGtk::activate_simulate_togg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  foe->activate_simulate_togg( set);
}

void WFoeGtk::activate_view_togg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  if ( w != ((WFoeGtk *)foe)->widgets.view_togg)
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WFoeGtk *)foe)->widgets.view_togg), TRUE);
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(((WFoeGtk *)foe)->widgets.view_togg));

  foe->activate_view_togg( set);
}

void WFoeGtk::activate_edit_togg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  if ( w != ((WFoeGtk *)foe)->widgets.edit_togg)
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WFoeGtk *)foe)->widgets.edit_togg), TRUE);
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(((WFoeGtk *)foe)->widgets.edit_togg));

  foe->activate_edit_togg( set);
}


//
//	Callback from the menu.
//
void WFoeGtk::valchanged_textinput( GtkWidget *w, gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;
  char	*value, *valueutf8;
  int	sts;

  if ( foe->widgets.textinput == 0)
    return;

  valueutf8 = gtk_editable_get_chars( GTK_EDITABLE(foe->widgets.textinput), 0, -1);
  if ( *valueutf8 == 0) {
    g_free(valueutf8);
    return;
  }
  value = g_convert( valueutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( valueutf8);

  strcpy( foe->searchstring, value);

  foe->message( "");
  gtk_editable_delete_text( GTK_EDITABLE(foe->widgets.textinput), 0, -1);
  g_object_set( foe->widgets.textinput, "visible", FALSE, NULL);

  /* Call the specified function */
  sts = (foe->textinput_func) (foe, value);
  foe->error_msg( sts);

  g_free( value);
}

//
//      callback from the caution widget 
//	call any function interessed by the yes aknowledge from the caution
//      box. The address of this function has been previoulsly carefully saved
//      if the context. ( foe->Yes_procedure )
// 	Author : SG
//	Date : 13.02.91

void WFoeGtk::yes_popupmsg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  g_object_set( ((WFoeGtk *)foe)->widgets.caution, "visible", FALSE, NULL);

  if ( foe->popupmsg_yes_p != NULL)
    (foe->popupmsg_yes_p) (foe ) ;
}

//
//      callback from the caution widget 
//	call any function interessed by the no aknowledge from the caution
//      box. The address of this function has been previoulsly carefully saved
//      if the context. ( foe->Yes_procedure )
// 	Author : SG
// 	Date : 13.02.91
//
void WFoeGtk::no_popupmsg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  g_object_set( ((WFoeGtk *)foe)->widgets.caution, "visible", FALSE, NULL);

  if ( foe->popupmsg_no_p != NULL)
    (foe->popupmsg_no_p)( foe);
}

//
//       Callback form the caution widget 
//
void WFoeGtk::cancel_popupmsg( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;
  g_object_set( ((WFoeGtk *)foe)->widgets.caution, "visible", FALSE, NULL);

  if ( foe->popupmsg_cancel_p != NULL)
    (foe->popupmsg_cancel_p)( foe);
}

//
//	Callback from the popup menu when poping down.
//

static gboolean wfoegtk_reset_current_node( void *data)
{
  WFoe *foe = (WFoe *)data;

  if ( foe->popupmenu_node != 0) {
    foe->gre->node_unselect( foe->popupmenu_node);
  }
  return FALSE;
}

void WFoeGtk::popup_unmap_cb( GtkWidget *w, gpointer data)
{
  WFoe *foe = (WFoe *)data;

  if ( foe->popupmenu_node != 0) {
    /* Wait some time and take away select if nobody
       else has done that */
    g_timeout_add( 50, wfoegtk_reset_current_node, data);
  }
  foe->normal_cursor();	
}

//
//  Modifies the popupmenu.
//
void foegtk_menu_position_func( GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
				gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;
  *x = foe->popupmenu_x;
  *y = foe->popupmenu_y;
  *push_in = FALSE;
}


typedef void (*gtk_tCallback)(GtkWidget *,gpointer);

int WFoeGtk::modify_popup( unsigned long popupmenu_mask, int x, int y)
{
  static char buttontext[][40] = { "ObjectEditor", "SubWindow", "Connect", "Delete",
				   "Paste", "Copy", "Cut", "PrintSelect", "HelpClass",
                                   "CreateObject"};
  static gtk_tCallback menu_callbacks[] = { WFoeGtk::activate_attribute,
					    WFoeGtk::activate_subwindow,
					    WFoeGtk::activate_getobj,
					    WFoeGtk::activate_delete,
					    WFoeGtk::activate_paste,
					    WFoeGtk::activate_copy,
					    WFoeGtk::activate_cut,
					    WFoeGtk::activate_printselect,
					    WFoeGtk::activate_helpclass,
					    WFoeGtk::activate_createobject};

  popupmenu_x = x + 5;
  popupmenu_y = y;
  GtkMenu *menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  for ( int i = 0; i < 10; i++) {
    if ( popupmenu_mask & (1 << i)) {
      GtkWidget *w = gtk_menu_item_new_with_label( buttontext[i]);
      g_signal_connect( w, "activate", 
			G_CALLBACK(menu_callbacks[i]), this);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), w);
      gtk_widget_show(w);
    }
  }
  g_signal_connect( menu, "deactivate", 
		    G_CALLBACK(popup_unmap_cb), this);
  gtk_menu_popup( menu, NULL, NULL, foegtk_menu_position_func, 
		  this, 0, gtk_get_current_event_time());
  widgets.popupmenu = (GtkWidget *)menu;

  return FOE__SUCCESS;
}

#if 0
void WFoeGtk::enable_set_focus( WFoeGtk *foe)
{
  foe->set_focus_disabled--;
}

void WFoeGtk::disable_set_focus( WFoeGtk *foe, int time)
{
  foe->set_focus_disabled++;
  foe->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( foe->parent_wid), time,
	(XtTimerCallbackProc)enable_set_focus, foe);
}
#endif

gboolean WFoeGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WFoeGtk *foe = (WFoeGtk *)data;

  if ( foe->focustimer.disabled())
    return FALSE;

  if ( foe->nav_palette_managed)
    foe->navctx->set_inputfocus( 1);
  else if ( foe->node_palette_managed)
    foe->node_palctx->set_inputfocus( 1);
  else if ( foe->con_palette_managed)
    foe->con_palctx->set_inputfocus( 1);

  foe->focustimer.disable( 400);
  return FALSE;
}


//
//	Start the attribute editor and store the attribute context pointer 
//	together with WFoe and the nodepointer to use when a backcall
//	comes.
//
WAtt *WFoeGtk::watt_new( void *a_parent_ctx,
			 ldh_tSesContext a_ldhses,
			 pwr_sAttrRef a_aref,
			 int a_editmode,
			 int a_advanced_user,
			 int a_display_objectname)
{
  WAttGtk *watt = new WAttGtk( parent_wid, a_parent_ctx,
			       a_ldhses, a_aref, a_editmode, a_advanced_user,
			       a_display_objectname);
  return watt;
}

//
//	Destroys a foe instance.
//	Destroys the widget and frees allocated memory for the 
//	context.
//
void WFoeGtk::destroy()
{
  gre->wind->hw.foe = 0;

  /* Delete controled modules */
  delete gre;
  delete node_palctx;
  delete con_palctx;
  delete navctx;

  /* Destroy the widget */
  gtk_widget_destroy( toplevel);

  /* Delete the context */
  delete this;
}

void WFoeGtk::set_title()
{
  if ( function == EDIT) {
    pwr_tOName new_title;
    
    strcpy( new_title, "*** ");
    cdh_StrncpyCutOff( &new_title[4], name, sizeof(new_title)-4, 0);

    char *titleutf8 = g_convert( new_title, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    gtk_window_set_title( GTK_WINDOW(toplevel), titleutf8);
    g_free( titleutf8);
  }
  else if ( function == VIEW) {
    pwr_tOName new_title;

    strcpy( new_title, name);

    char *titleutf8 = g_convert( new_title, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    gtk_window_set_title( GTK_WINDOW(toplevel), titleutf8);
    g_free( titleutf8);
  }
}

WFoe *WFoeGtk::subwindow_new( void			*f_parent_ctx,
				char			*f_name,
				pwr_tObjid     	plcprogram,
				ldh_tWBContext 	ldhwbctx,
				ldh_tSesContext	ldhsesctx,
				vldh_t_node    	nodeobject,
				unsigned long	windowindex,
				unsigned long	new_window,
				int	       		f_map_window,
				ldh_eAccess    	f_access,
				foe_eFuncAccess	function_access,
				pwr_tStatus 	*sts)
{
  return new WFoeGtk( f_parent_ctx, widgets.foe_window, f_name, plcprogram,
		      ldhwbctx, ldhsesctx, nodeobject, windowindex,
		      new_window, f_map_window, f_access, function_access,
		      sts);
}


//
//	Displays a message in the foe window.
//	Changes the label of a label widget which id is stored in the context.
//	Author: SG
// 	Date : 25.01.91 
//    	To avoid unnecessary call to this routine the context variable :
//     	foe.msg_label_id is set to null if the the message to be displayed 
//     	is "" . This case occurs when you want to erase a previous message.
//
void WFoeGtk::message( char *new_label) 
{
  int 		l;

  gre->search_rectangle_delete();
  l= strlen (new_label);
  msg_label_id = ( l !=0 );
  if ( l == 0 ) new_label = " " ;
  char *new_labelutf8 = g_convert( new_label, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  gtk_label_set_text( GTK_LABEL(widgets.label), new_labelutf8);
  g_free( new_labelutf8);
}

//
// This routine is called by any one who wants to send a little message.
// in a ( modal) message box. No callback is expected. We just want to
// be sure that the user see the message.
// Author: SG
// Date : 24.02.91
//
void WFoeGtk::msgbox( char *new_label) 
{
  wow->DisplayError( "Message", new_label);
}

//
// This routine is called by any one who wants to send a little caution message.
// The calling routine send also the address of the function it want to
// be executed when the yes button is pressed in the caution box.
// Author: SG
// Date : 13.02.91
//
void WFoeGtk::popupmsg( char *new_label, void (* yes_procedure)(WFoe *), 
			void (* no_procedure)(WFoe *),
			void (* cancel_procedure)(WFoe *)) 
{
  create_confirm_dialog();
  char *new_labelutf8 = g_convert( new_label, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  g_object_set( widgets.caution_label, "label", new_labelutf8, NULL);
  g_free( new_labelutf8);

  popupmsg_yes_p = yes_procedure ;        
  popupmsg_no_p = no_procedure ;        
  popupmsg_cancel_p = cancel_procedure ;        
}

//
// Constructor
//
WFoeGtk::WFoeGtk( void		*f_parent_ctx,
		  GtkWidget    	*f_parent_wid,
		  char		*f_name,
		  pwr_tObjid    plcprogram,
		  ldh_tWBContext ldhwbctx,
		  ldh_tSesContext ldhsesctx,
		  vldh_t_node   nodeobject,
		  unsigned long	windowindex,
		  unsigned long	new_window,
		  int	       	f_map_window,
		  ldh_eAccess   f_access,
		  foe_eFuncAccess function_access,
		  pwr_tStatus   *sts) :
  WFoe(f_parent_ctx,f_name,plcprogram,ldhwbctx,ldhsesctx,nodeobject,
       windowindex,new_window,f_map_window,f_access,function_access,sts),
  parent_wid(f_parent_wid)
{

  memset( &widgets, 0, sizeof(widgets));

  *sts = new_local( plcprogram, ldhwbctx, ldhsesctx,
		    nodeobject, windowindex, new_window, function_access);

  wow = new CoWowGtk( toplevel);
}


//
// Constructor
//
WFoeGtk::WFoeGtk( void *f_parent_ctx,
		      GtkWidget *f_parent_wid,
		      char *f_name,
		      pwr_tObjid plcprogram,
		      ldh_tWBContext ldhwbctx,
		      ldh_tSesContext ldhsesctx,
		      int f_map_window,
		      ldh_eAccess f_access,
		      pwr_tStatus *sts) :
  WFoe(f_parent_ctx,f_name,plcprogram,ldhwbctx,ldhsesctx,f_map_window,
       f_access,sts),
  parent_wid(f_parent_wid)
{
  int		size;
  pwr_tFullName	new_name;
  ldh_tSesContext ldhses;

  memset( &widgets, 0, sizeof(widgets));

  /* Check that it is possible to open a session */
  *sts = ldh_OpenSession(&ldhses, ldh_SessionToVol( ldhsesctx),
			access, ldh_eUtility_PlcEditor);
  if ( EVEN(*sts)) return;

  *sts = ldh_CloseSession(ldhses);

  /* New title */
  *sts = ldh_ObjidToName( ldhsesctx, plcprogram, ldh_eName_Hierarchy,
			  new_name, sizeof( new_name), &size);
  error_msg(*sts);
  if( EVEN(*sts)) return;

  strcpy( name, new_name);
  *sts = new_local( plcprogram,
		    ldhwbctx, ldhsesctx, 0, 0, 0,
		    foe_eFuncAccess_Edit);

  wow = new CoWowGtk( toplevel);
}

//
// Pop the window
//
void WFoeGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer foe)
{
  WFoeGtk::activate_quit(w, foe);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

//
// Create a new foe window
// SG 09.02.91 : Create a new toplevel widget and give it an pixmap icon
// SG : 13.02.91 Fetch a caution box and save the id of the widget in the 
//               foe context for futher use
// SG : 24.02.91 Fetch a widget ( again !) . This time a message box
// SG : 24.03.91 Send the routine the desired position (x_top , y_top) 
// and the desired size ( width_adb , height_adb ) for the window to be created.
// The two first parameters are set to the top level widget
// and the two last paramters are set to the adb widget (see in uil file
// the widget hierarchy )
// SG 14.04.91 A new widget to fetch , the help widget ! 
// SG 2.05.91 New parameter : function ( edit , trace , view , simulate ..)
// to indicate which neted events and which menu entries should be alllowed.
//
pwr_tStatus WFoeGtk::create_window( int x_top,
				    int y_top,
				    int width_adb,
				    int height_adb, 
				    ldh_tSesContext ldhsession,
				    foe_eFuncAccess function_access)
{
  const int	window_width = 900;
  const int    	window_height = 800;
  const int    	palette_width = 220;
  pwr_tStatus 	sts;
  pwr_tFileName fname;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", window_height,
			   "default-width", window_width,
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(WFoeGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  widgets.save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_g);
  g_signal_connect(widgets.save, "activate", G_CALLBACK(WFoeGtk::activate_save), this);

  // Submenu Print
  GtkWidget *file_print_documents = gtk_menu_item_new_with_mnemonic( "_Documents");
  g_signal_connect( file_print_documents, "activate", 
		    G_CALLBACK(WFoeGtk::activate_print), this);

  GtkWidget *file_print_overv = gtk_menu_item_new_with_mnemonic( "_Overview");
  g_signal_connect( file_print_overv, "activate", 
		    G_CALLBACK(WFoeGtk::activate_printoverv), this);

  GtkWidget *file_print_select = gtk_menu_item_new_with_mnemonic( "_Selected Documents");
  g_signal_connect( file_print_select, "activate", 
		    G_CALLBACK(WFoeGtk::activate_printselect), this);

  GtkWidget *file_print_pdf = gtk_menu_item_new_with_mnemonic( "To Pdf _File");
  g_signal_connect( file_print_pdf, "activate", 
		    G_CALLBACK(WFoeGtk::activate_printpdf), this);

  GtkWidget *file_print = gtk_menu_item_new_with_mnemonic( "_Print");
  GtkMenu *file_print_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_print_menu), file_print_documents);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_print_menu), file_print_overv);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_print_menu), file_print_select);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_print_menu), file_print_pdf);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_print),
			    GTK_WIDGET(file_print_menu));

  widgets.syntax = gtk_menu_item_new_with_mnemonic( "Synta_x");
  g_signal_connect( widgets.syntax, "activate", 
		    G_CALLBACK(WFoeGtk::activate_syntax), this);

  widgets.compile = gtk_image_menu_item_new_with_mnemonic( "_Build");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(widgets.compile), 
				 gtk_image_new_from_stock( "gtk-execute", GTK_ICON_SIZE_MENU));
  g_signal_connect( widgets.compile, "activate", 
		    G_CALLBACK(WFoeGtk::activate_compile), this);

  widgets.plcattribute = gtk_menu_item_new_with_mnemonic( "Plc _Attributes...");
  g_signal_connect( widgets.plcattribute, "activate", 
		    G_CALLBACK(WFoeGtk::activate_plcattribute), this);

  widgets.winddelete = gtk_menu_item_new_with_mnemonic( "_Delete Window");
  g_signal_connect( widgets.winddelete, "activate", 
		    G_CALLBACK(WFoeGtk::activate_winddelete), this);

  widgets.savetrace = gtk_menu_item_new_with_mnemonic( "Save _Trace");
  g_signal_connect( widgets.savetrace, "activate", 
		    G_CALLBACK(WFoeGtk::activate_savetrace), this);

  widgets.restoretrace = gtk_menu_item_new_with_mnemonic( "_Restore Trace");
  g_signal_connect( widgets.restoretrace, "activate", 
		    G_CALLBACK(WFoeGtk::activate_restoretrace), this);

  widgets.exit = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(widgets.exit, "activate", G_CALLBACK(WFoeGtk::activate_quit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.save);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.syntax);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.compile);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.plcattribute);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.winddelete);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.savetrace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.restoretrace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), widgets.exit);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Edit Entry
  widgets.undelete = gtk_menu_item_new_with_mnemonic( "_Undo Delete");
  g_signal_connect( widgets.undelete, "activate", 
		    G_CALLBACK(WFoeGtk::activate_undelete), this);

  widgets.unselect = gtk_menu_item_new_with_mnemonic( "_Undo Select");
  g_signal_connect( widgets.unselect, "activate", 
		    G_CALLBACK(WFoeGtk::activate_unselect), this);

  widgets.cut = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_g);
  g_signal_connect(widgets.cut, "activate", G_CALLBACK(WFoeGtk::activate_cut), this);

  widgets.copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_g);
  g_signal_connect(widgets.copy, "activate", G_CALLBACK(WFoeGtk::activate_copy), this);

  widgets.paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_g);
  g_signal_connect(widgets.paste, "activate", G_CALLBACK(WFoeGtk::activate_paste), this);

  GtkWidget *edit_select_nextright = gtk_menu_item_new_with_mnemonic( "Next Right");
  g_signal_connect( edit_select_nextright, "activate",
		    G_CALLBACK(activate_select_nextright), this);
  gtk_widget_add_accelerator( edit_select_nextright, "activate", accel_g,
			      GDK_Right, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkWidget *edit_select_nextleft = gtk_menu_item_new_with_mnemonic( "Next Left");
  g_signal_connect( edit_select_nextleft, "activate",
		    G_CALLBACK(activate_select_nextleft), this);
  gtk_widget_add_accelerator( edit_select_nextleft, "activate", accel_g,
			      GDK_Left, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkWidget *edit_select_nextup = gtk_menu_item_new_with_mnemonic( "Next Up");
  g_signal_connect( edit_select_nextup, "activate",
		    G_CALLBACK(activate_select_nextup), this);
  gtk_widget_add_accelerator( edit_select_nextup, "activate", accel_g,
			      GDK_Up, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkWidget *edit_select_nextdown = gtk_menu_item_new_with_mnemonic( "Next Down");
  g_signal_connect( edit_select_nextdown, "activate",
		    G_CALLBACK(activate_select_nextdown), this);
  gtk_widget_add_accelerator( edit_select_nextdown, "activate", accel_g,
			      GDK_Down, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkWidget *edit_select = gtk_menu_item_new_with_mnemonic( "Select");
  GtkMenu *edit_select_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_select_menu), edit_select_nextright);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_select_menu), edit_select_nextleft);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_select_menu), edit_select_nextup);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_select_menu), edit_select_nextdown);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_select),
			    GTK_WIDGET(edit_select_menu));

  widgets.connect = gtk_menu_item_new_with_mnemonic( "C_onnect");
  g_signal_connect( widgets.connect, "activate", 
		    G_CALLBACK(WFoeGtk::activate_getobj), this);
  gtk_widget_add_accelerator( widgets.connect, "activate", accel_g,
			      'q', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  widgets.del = gtk_image_menu_item_new_from_stock(GTK_STOCK_DELETE, NULL);
  g_signal_connect(widgets.del, "activate", G_CALLBACK(WFoeGtk::activate_delete), this);

  widgets.changetext = gtk_menu_item_new_with_mnemonic( "C_hange Text");
  g_signal_connect( widgets.changetext, "activate", 
		    G_CALLBACK(WFoeGtk::activate_changetext), this);
  gtk_widget_add_accelerator( widgets.changetext, "activate", accel_g,
			      't', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  widgets.expand = gtk_menu_item_new_with_mnemonic( "E_xpand Object");
  g_signal_connect( widgets.expand, "activate", 
		    G_CALLBACK(WFoeGtk::activate_expand), this);
  gtk_widget_add_accelerator( widgets.expand, "activate", accel_g,
  			      GDK_Right, GdkModifierType(GDK_SHIFT_MASK), 
  			      GTK_ACCEL_VISIBLE);

  widgets.compress = gtk_menu_item_new_with_mnemonic( "C_ompress Object");
  g_signal_connect( widgets.compress, "activate", 
		    G_CALLBACK(WFoeGtk::activate_compress), this);
  gtk_widget_add_accelerator( widgets.compress, "activate", accel_g,
  			      GDK_Left, GdkModifierType(GDK_SHIFT_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkMenu *edit_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.undelete);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.unselect);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.cut);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.copy);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.paste);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.del);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_select);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.expand);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), widgets.compress);

  widgets.edit_entry = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), widgets.edit_entry);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(widgets.edit_entry), GTK_WIDGET(edit_menu));

  // Search menu
  GtkWidget *search_object = gtk_menu_item_new_with_mnemonic( "_Object");
  g_signal_connect( search_object, "activate", 
		    G_CALLBACK(WFoeGtk::activate_searchobject), this);

  GtkWidget *search_string = gtk_menu_item_new_with_mnemonic( "_String");
  g_signal_connect( search_string, "activate", 
		    G_CALLBACK(WFoeGtk::activate_searchstring), this);
  gtk_widget_add_accelerator( search_string, "activate", accel_g,
  			      'f', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *search_next = gtk_menu_item_new_with_mnemonic( "_Next");
  g_signal_connect( search_next, "activate", 
		    G_CALLBACK(WFoeGtk::activate_searchnext), this);
  gtk_widget_add_accelerator( search_next, "activate", accel_g,
  			      'g', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkMenu *search_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(search_menu), search_object);
  gtk_menu_shell_append(GTK_MENU_SHELL(search_menu), search_string);
  gtk_menu_shell_append(GTK_MENU_SHELL(search_menu), search_next);

  widgets.search_entry = gtk_menu_item_new_with_mnemonic("_Search");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), widgets.search_entry);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(widgets.search_entry), GTK_WIDGET(search_menu));


  // View Entry

  // Submenu Palette
  GSList *view_pal_group = NULL;
  widgets.view_objectpal = gtk_radio_menu_item_new_with_mnemonic( view_pal_group, 
								  "_Object");
  view_pal_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(widgets.view_objectpal));
  g_signal_connect( widgets.view_objectpal, "activate", 
		    G_CALLBACK(WFoeGtk::activate_palette_object), this);

  widgets.view_conpal = gtk_radio_menu_item_new_with_mnemonic( view_pal_group, 
							       "_Connection");
  view_pal_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(widgets.view_conpal));
  g_signal_connect( widgets.view_conpal, "activate", 
		    G_CALLBACK(WFoeGtk::activate_palette_con), this);

  widgets.view_plantpal = gtk_radio_menu_item_new_with_mnemonic( view_pal_group, 
								 "_Plant");
  g_signal_connect( widgets.view_plantpal, "activate", 
		    G_CALLBACK(WFoeGtk::activate_palette_plant), this);

  GtkWidget *view_pal = gtk_menu_item_new_with_mnemonic( "_Palette");
  GtkMenu *view_pal_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_pal_menu), widgets.view_objectpal);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_pal_menu), widgets.view_conpal);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_pal_menu), widgets.view_plantpal);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_pal),
			    GTK_WIDGET(view_pal_menu));

  GtkWidget *view_refcon = gtk_check_menu_item_new_with_mnemonic( "_Reference Connections");
  gtk_widget_add_accelerator( view_refcon, "activate", accel_g,
			      'r', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);
  g_signal_connect( view_refcon, "activate", 
		    G_CALLBACK(WFoeGtk::activate_refcon), this);

  // Submenu gridsize
  GSList *view_gs_group = NULL;
  GtkWidget *view_gs_1 = gtk_radio_menu_item_new_with_mnemonic( view_gs_group, "_1");
  view_gs_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gs_1));
  g_signal_connect( view_gs_1, "activate", 
		    G_CALLBACK(WFoeGtk::activate_gridsize1), this);

  GtkWidget *view_gs_2 = gtk_radio_menu_item_new_with_mnemonic( view_gs_group, "_2");
  view_gs_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gs_2));
  g_signal_connect( view_gs_2, "activate", 
		    G_CALLBACK(WFoeGtk::activate_gridsize2), this);

  GtkWidget *view_gs_3 = gtk_radio_menu_item_new_with_mnemonic( view_gs_group, "_3");
  view_gs_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gs_3));
  g_signal_connect( view_gs_3, "activate", 
		    G_CALLBACK(WFoeGtk::activate_gridsize3), this);

  GtkWidget *view_gs_4 = gtk_radio_menu_item_new_with_mnemonic( view_gs_group, "_4");
  view_gs_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gs_4));
  g_signal_connect( view_gs_4, "activate", 
		    G_CALLBACK(WFoeGtk::activate_gridsize4), this);

  GtkWidget *view_gs_5 = gtk_radio_menu_item_new_with_mnemonic( view_gs_group, "_5");
  g_signal_connect( view_gs_5, "activate", 
		    G_CALLBACK(WFoeGtk::activate_gridsize5), this);

  GtkWidget *view_gs = gtk_menu_item_new_with_mnemonic( "_Grid Size");
  GtkMenu *view_gs_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gs_menu), view_gs_1);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gs_menu), view_gs_2);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gs_menu), view_gs_3);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gs_menu), view_gs_4);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gs_menu), view_gs_5);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_gs),
			    GTK_WIDGET(view_gs_menu));

  GtkWidget *view_showgrid = gtk_check_menu_item_new_with_mnemonic( "_Show Grid");
  g_signal_connect( view_showgrid, "activate", 
		    G_CALLBACK(WFoeGtk::activate_showgrid), this);

  // Zoom submenu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect( view_zoom_in, "activate", 
		    G_CALLBACK(WFoeGtk::activate_zoomin), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
  			      'i', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect( view_zoom_out, "activate", 
		    G_CALLBACK(WFoeGtk::activate_zoomout), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
  			      'o', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect( view_zoom_reset, "activate", 
		    G_CALLBACK(WFoeGtk::activate_unzoom), this);
  gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
  			      'b', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom = gtk_menu_item_new_with_mnemonic( "_Zoom");
  GtkMenu *view_zoom_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_zoom_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_zoom_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_zoom_menu), view_zoom_reset);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_zoom),
			    GTK_WIDGET(view_zoom_menu));


  GtkWidget *view_showexeord = gtk_check_menu_item_new_with_mnemonic( "S_how Execute Order");
  g_signal_connect( view_showexeord, "activate", 
		    G_CALLBACK(WFoeGtk::activate_showexeord), this);

  widgets.redraw = gtk_menu_item_new_with_mnemonic( "R_edraw");
  g_signal_connect( widgets.redraw, "activate", 
		    G_CALLBACK(WFoeGtk::activate_redraw), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_pal);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_refcon);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_gs);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_showgrid);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_showexeord);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), widgets.redraw);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Functions Entry
  GtkWidget *functions_openobj = gtk_menu_item_new_with_mnemonic( "_Open Object...");
  g_signal_connect( functions_openobj, "activate", 
		    G_CALLBACK(WFoeGtk::activate_attribute), this);
  gtk_widget_add_accelerator( functions_openobj, "activate", accel_g,
  			      'a', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_subwind = gtk_menu_item_new_with_mnemonic( "Open _Subwindow");
  g_signal_connect( functions_subwind, "activate", 
		    G_CALLBACK(WFoeGtk::activate_subwindow), this);
  gtk_widget_add_accelerator( functions_openobj, "activate", accel_g,
  			      'j', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);


  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_openobj);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_subwind);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("_Functions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // Mode entry
  widgets.view_togg = gtk_check_menu_item_new_with_mnemonic( "_View");
  g_signal_connect( widgets.view_togg, "activate", 
		    G_CALLBACK(WFoeGtk::activate_view_togg), this);

  widgets.edit_togg = gtk_check_menu_item_new_with_mnemonic( "_Edit");
  g_signal_connect( widgets.edit_togg, "activate", 
		    G_CALLBACK(WFoeGtk::activate_edit_togg), this);
  gtk_widget_add_accelerator( widgets.edit_togg, "activate", accel_g,
  			      'e', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  widgets.trace_togg = gtk_check_menu_item_new_with_mnemonic( "_Trace");
  g_signal_connect( widgets.trace_togg, "activate", 
		    G_CALLBACK(WFoeGtk::activate_trace_togg), this);

  widgets.simulate_togg = gtk_check_menu_item_new_with_mnemonic( "_Simulate");
  g_signal_connect( widgets.simulate_togg, "activate", 
		    G_CALLBACK(WFoeGtk::activate_simulate_togg), this);

  GtkMenu *mode_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), widgets.view_togg);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), widgets.edit_togg);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), widgets.trace_togg);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), widgets.simulate_togg);

  GtkWidget *mode = gtk_menu_item_new_with_mnemonic("_Mode");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), mode);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mode), GTK_WIDGET(mode_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic("_Help");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_help, "activate", G_CALLBACK(WFoeGtk::activate_help), this);
  gtk_widget_add_accelerator( help_help, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  widgets.tools_save = gtk_button_new();
  gtk_container_add( GTK_CONTAINER( widgets.tools_save), 
	  gtk_image_new_from_stock( "gtk-save", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(widgets.tools_save, "clicked", G_CALLBACK(WFoeGtk::activate_save), this);
  g_object_set( widgets.tools_save, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, widgets.tools_save, "Save", "");

  GtkWidget *tools_edit = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_edit.png");
  gtk_container_add( GTK_CONTAINER( tools_edit), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_edit, "clicked", G_CALLBACK(WFoeGtk::activate_edit_togg), this);
  g_object_set( tools_edit, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_edit, "Set edit mode", "");

  GtkWidget *tools_view = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_view.png");
  gtk_container_add( GTK_CONTAINER( tools_view), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_view, "clicked", G_CALLBACK(WFoeGtk::activate_view_togg), this);
  g_object_set( tools_view, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_view, "Set view mode", "");

  widgets.tools_build = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(widgets.tools_build), 
	  gtk_image_new_from_stock( "gtk-execute", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(widgets.tools_build, "clicked", G_CALLBACK(WFoeGtk::activate_compile), this);
  g_object_set( widgets.tools_build, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, widgets.tools_build, "Build Program", "");

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(WFoeGtk::activate_zoomin), this);
  g_object_set( tools_zoom_in, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(WFoeGtk::activate_zoomout), this);
  g_object_set( tools_zoom_out, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(WFoeGtk::activate_unzoom), this);
  g_object_set( tools_zoom_reset, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_reset, "Zoom reset", "");

  GtkWidget *tools_objectpalette = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_objectpalette.png");
  gtk_container_add( GTK_CONTAINER( tools_objectpalette), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_objectpalette, "clicked", G_CALLBACK(WFoeGtk::activate_palette_object), this);
  g_object_set( tools_objectpalette, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_objectpalette, "Show Object palette", "");

  GtkWidget *tools_conpalette = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_conpalette.png");
  gtk_container_add( GTK_CONTAINER( tools_conpalette), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_conpalette, "clicked", G_CALLBACK(WFoeGtk::activate_palette_con), this);
  g_object_set( tools_conpalette, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_conpalette, "Show Connection palette", "");

  GtkWidget *tools_plantpalette = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_navpalette.png");
  gtk_container_add( GTK_CONTAINER( tools_plantpalette), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_plantpalette, "clicked", G_CALLBACK(WFoeGtk::activate_palette_plant), this);
  g_object_set( tools_plantpalette, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_plantpalette, "Show Plant Hierarchy", "");

  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  widgets.label = gtk_label_new( "");
  gtk_widget_set_size_request( widgets.label, -1, 25);
  widgets.textinput = gtk_entry_new();
  gtk_widget_set_size_request( widgets.textinput, -1, 25);
  g_signal_connect( widgets.textinput, "activate", 
  		    G_CALLBACK(WFoeGtk::valchanged_textinput), this);

  gtk_box_pack_start( GTK_BOX(statusbar), widgets.label, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), widgets.textinput, TRUE, TRUE, 20);
  gtk_widget_show_all( statusbar);


  widgets.subpane = gtk_vpaned_new();
  GtkWidget *palbox = gtk_hbox_new( FALSE, 0);
  node_palctx = new PalGtk( this, widgets.subpane, "Objects",
			    ldhsession, "PlcEditorPalette",
			    &widgets.node_palette, &sts);
  node_palette_managed = 1;

  con_palctx = new PalGtk( this, widgets.subpane, "Connections",
			   ldhsession, "PlcEditorConPalette", 
			   &widgets.con_palette, &sts);

  navctx = new NavGtk( this, widgets.subpane, "Plant",
		       ldhsession, "NavigatorW1",
		       &widgets.nav_palette, &sts);

  gtk_box_pack_start( GTK_BOX(palbox), widgets.node_palette, TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(palbox), widgets.con_palette, TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(palbox), widgets.nav_palette, TRUE, TRUE, 0);

  gtk_paned_pack1( GTK_PANED(widgets.subpane), palbox, TRUE, TRUE);
  
  widgets.pane = gtk_hpaned_new();
  gre = new WGreGtk( this, widgets.pane, "Name");

  ((WGreGtk *)gre)->new_navigator( widgets.subpane);
  gtk_paned_pack2( GTK_PANED(widgets.subpane), ((WGreGtk *)gre)->nav_widget, FALSE, TRUE);
  gtk_widget_show_all( widgets.subpane);

  gtk_paned_pack1( GTK_PANED(widgets.pane), ((WGreGtk *)gre)->form_widget, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(widgets.pane), widgets.subpane, FALSE, TRUE);
  gtk_widget_show_all( widgets.pane);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(widgets.pane), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 3);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);
  gtk_widget_show_all( toplevel);

  g_object_set( widgets.con_palette, "visible", FALSE, NULL);
  g_object_set( widgets.nav_palette, "visible", FALSE, NULL);
  g_object_set( widgets.textinput, "visible", FALSE, NULL);
  gtk_paned_set_position( GTK_PANED(widgets.pane), window_width - palette_width);
  gtk_paned_set_position( GTK_PANED(widgets.subpane), window_height - palette_width);

  gre->init();
  function_setup();

#if 0
  Arg	args[20];
  int	sts;
  Widget *children;
  int i, num_children;
  XmString   	TitleStr;
  Dimension icon_w, icon_h;

  /* DRM database hierarchy related variables */
  MrmHierarchy s_MrmH;
  MrmType dclass;
  char		uid_filename[200] = {"pwr_exe:wb_foe.uid"};
  char		*uid_filename_p = uid_filename;
  static Atom	MwmDeleteWind = 0;
  static char translations[] = "\
<FocusIn>: foe_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
    {
      {"foe_inputfocus",      (XtActionProc) action_inputfocus}
    };
       
  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts)) {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

  /*
   * Now start the module creation
   */

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /*
   * Create a new main widget
   */
  /*
   * Open the UID files (the output of the UIL compiler) in the hierarchy
   * Register the items DRM needs to bind for us.
   * Create a new neted-widget
   * Close the hierarchy
   * Compile the additional button translations and augment and add actions
   */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_MrmH);
  if (sts != MrmSUCCESS) {
    printf("can't open hierarchy\n");
    return 0;
  }

  register_callbacks();

  /* SG 09.02.91 : create the new hierarchy. By the way use a pixmap define
  with help of wb_foe_ptrn.h */

  /* create a pixmap for the icon */
  if ( icon_pixmap == 0 )
    sts = MrmFetchBitmapLiteral(s_MrmH,"icon",
				XtScreen(parent_wid), XtDisplay(parent_wid),
				&icon_pixmap, &icon_w, &icon_h);  
 
  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  XtSetArg(args[i],XtNiconPixmap,icon_pixmap);  i++;
  
  /* SG 11.02.91 Save the id of the top in the context */ 
  parent_wid = XtCreatePopupShell("plcEditor", 
		     topLevelShellWidgetClass, parent_wid, args, i);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext( parent_wid), 
						actions, XtNumber(actions));

  /* the positioning of a top level can only be define after the creation
      of the widget . So i do it know: 
     SG 24.02.91 use the parameters received x and y 
  */

  i=0;
  XtSetArg(args[i],XmNx,x_top);i++;
  XtSetArg(args[i],XmNy,y_top);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  if ( ! map_window) {
    XtSetArg(args[i],XmNmappedWhenManaged,FALSE); i++;
  }

  XtSetValues( parent_wid, args, i);

  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_MrmH, "foe_window", parent_wid ,
		name, args, 1,
		&widgets.foe_window, &dclass);
  if (sts != MrmSUCCESS) printf("can't fetch foedit widget\n");

  /* SG 24.02.91 Set the width and hight desired for the main window to his
	child the adb widget */
  i=0;
  XtSetArg(args[i],XmNwidth,width_adb);i++;
  XtSetArg(args[i],XmNheight,height_adb);i++;
  XtSetValues( widgets.adb  ,args,i);

  /* SG 24.02.91
     Get a litle message box widget that you will make appears when you want by
     calling foe_msgbox
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_msgbox", 
			widgets.foe_window,
			&widgets.msgbox,
			&dclass) != MrmSUCCESS)
	printf("can't fetch msgbox \n");

  TitleStr = XmStringCreateLtoR("PlcEdit Message" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( widgets.msgbox, args, 1);
  XmStringFree(TitleStr);


  /* SG 13.02.91 
     Get a litle caution widget that you will make appears when you want by
     calling foe_popupmsg 
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_popupmsg", 
			widgets.foe_window,
			&widgets.caution,
			&dclass) != MrmSUCCESS)
    printf("can't fetch popupmsg\n");

  TitleStr = XmStringCreateLtoR("PlcEdit Confirm" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( widgets.caution, args, 1);
  XmStringFree(TitleStr);


  /* create the pane widgets */
  /* create the palette module */
  /* SG 07.03.91 put two palletes in the sub pane widget */

  node_palctx = pal_new( this, widgets.subpane, "Objects",
		ldhsession, "PlcEditorPalette", 
		&widgets.node_palette, &sts);
  node_palette_managed = 1;

  navctx = nav_new( this, widgets.subpane, "Plant",
		ldhsession, "NavigatorW1",
		&widgets.nav_palette, &sts);
  XtUnmanageChild( widgets.nav_palette);

  con_palctx = pal_new( this, widgets.subpane, "Connections",
		ldhsession, "PlcEditorConPalette", 
		&widgets.con_palette, &sts);
  XtUnmanageChild( widgets.con_palette);


  /* Turn off traversal for sash widgets */
  XtVaGetValues(widgets.subpane, 
		XmNchildren, &children,
		XmNnumChildren, &num_children,
		NULL);
  for (i = 0; i < num_children; i++) {
    if (XmIsSash(children[i]))
      XtVaSetValues(children[i], XmNtraversalOn, False, NULL);
  }

  /* create the gredit module */
  XtManageChild(widgets.foe_window);


  gre = new WGre( this, widgets.pane, "Name");

  gre->new_navigator( widgets.subpane);

  if ( map_window)
    XtPopup(parent_wid, XtGrabNone);
  else
    XtRealizeWidget( parent_wid);

  gre->init();

  /* create the setup ie:
     enable netedevents,
     enable menu entries
     setup backcalls from the gredit module */

  function_setup();

  MrmCloseHierarchy(s_MrmH);

  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( widgets.foe_window, compiled_translations);

  XtUnmanageChild(widgets.textinput);

  if ( !(login_prv.priv & pwr_mPrv_DevPlc ) || 
       (function_access == foe_eFuncAccess_View)) {
    /* Mode Edit and Simulate is not allowed */
    i=0;
    XtSetArg(args[i],XmNsensitive,0); i++;

    XtSetValues( widgets.edit_togg,args,i);
    XtSetValues( widgets.simulate_togg,args,i);
  }

  if (MwmDeleteWind == 0)
    MwmDeleteWind = XInternAtom(XtDisplay(parent_wid), "WM_DELETE_WINDOW", FALSE);

  if (MwmDeleteWind != 0) {
    XtVaSetValues(parent_wid, XmNdeleteResponse, XmDO_NOTHING, NULL);
    XmAddWMProtocolCallback(parent_wid, MwmDeleteWind, 
			    (XtCallbackProc)activate_quit, this);
  }

  wow_GetAtoms( parent_wid, 0, &objid_atom, 0);

  /* SG 09.02.91 a top level should always be realized !
   * ML 16.03.94 changed Realize to Popup
   */

#endif
  return FOE__SUCCESS;
}


void WFoeGtk::set_mode_button_state( int mode, int state)
{
  gboolean active = state ? TRUE : FALSE;
  switch ( mode) {
  case VIEW:
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(widgets.view_togg), active);
    break;
  case EDIT:
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(widgets.edit_togg), active);
    break;
  case TRACE:
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(widgets.trace_togg), active);
    break;
  case SIMULATE:
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(widgets.simulate_togg), active);
    break;
  }
}

//
//	Show the text input widget.
//
int WFoeGtk::get_textinput( char *msg, int (*function)(WFoe *, char *))
{
  /* Print the text */
  message( msg);

  /* View the input widget */
  g_object_set( widgets.textinput, "visible", TRUE, NULL);
  gtk_widget_grab_focus( widgets.textinput);

  /* Insert the function in foe */
  textinput_func = function;
  
  return FOE__SUCCESS;
}

//
// Disable or enable the menu entries that are relevant for the trace function.
//
int WFoeGtk::edit_set_entries()
{
  set_title();

  if ( !classeditor) {
    gtk_widget_set_sensitive( widgets.save,TRUE);
    gtk_widget_set_sensitive( widgets.exit,TRUE);
    gtk_widget_set_sensitive( widgets.syntax,TRUE);
    gtk_widget_set_sensitive( widgets.compile,TRUE);
    gtk_widget_set_sensitive( widgets.savetrace,TRUE);
    gtk_widget_set_sensitive( widgets.restoretrace,TRUE);
    gtk_widget_set_sensitive( widgets.redraw,TRUE);
    gtk_widget_set_sensitive( widgets.plcattribute,TRUE);
    gtk_widget_set_sensitive( widgets.winddelete,TRUE);
    gtk_widget_set_sensitive( widgets.edit_entry,TRUE);
    gtk_widget_set_sensitive( widgets.cut,TRUE);
    gtk_widget_set_sensitive( widgets.copy,TRUE);
    gtk_widget_set_sensitive( widgets.paste,TRUE);
    gtk_widget_set_sensitive( widgets.undelete,TRUE);
    gtk_widget_set_sensitive( widgets.unselect,TRUE);
    gtk_widget_set_sensitive( widgets.connect,TRUE);
    gtk_widget_set_sensitive( widgets.del,TRUE);
    gtk_widget_set_sensitive( widgets.changetext,TRUE);
    gtk_widget_set_sensitive( widgets.expand,TRUE);
    gtk_widget_set_sensitive( widgets.compress,TRUE);
    gtk_widget_set_sensitive( widgets.tools_save,TRUE);
    gtk_widget_set_sensitive( widgets.tools_build,TRUE);
  }
  else {
    gtk_widget_set_sensitive( widgets.save,TRUE);
    gtk_widget_set_sensitive( widgets.exit,TRUE);
    gtk_widget_set_sensitive( widgets.syntax,FALSE);
    gtk_widget_set_sensitive( widgets.compile,FALSE);
    gtk_widget_set_sensitive( widgets.savetrace,FALSE);
    gtk_widget_set_sensitive( widgets.restoretrace,FALSE);
    gtk_widget_set_sensitive( widgets.redraw,TRUE);
    gtk_widget_set_sensitive( widgets.plcattribute,TRUE);
    gtk_widget_set_sensitive( widgets.winddelete,TRUE);
    gtk_widget_set_sensitive( widgets.edit_entry,TRUE);
    gtk_widget_set_sensitive( widgets.cut,TRUE);
    gtk_widget_set_sensitive( widgets.copy,TRUE);
    gtk_widget_set_sensitive( widgets.paste,TRUE);
    gtk_widget_set_sensitive( widgets.undelete,TRUE);
    gtk_widget_set_sensitive( widgets.unselect,TRUE);
    gtk_widget_set_sensitive( widgets.connect,TRUE);
    gtk_widget_set_sensitive( widgets.del,TRUE);
    gtk_widget_set_sensitive( widgets.changetext,TRUE);
    gtk_widget_set_sensitive( widgets.expand,TRUE);
    gtk_widget_set_sensitive( widgets.compress,TRUE);
    gtk_widget_set_sensitive( widgets.tools_save,TRUE);
    gtk_widget_set_sensitive( widgets.tools_build,FALSE);
  }

  return FOE__SUCCESS ;
}

//
// Disable or enable the menu entries that are relevant for the trace function.
//
int WFoeGtk::view_set_entries()
{  
  set_title();

  gtk_widget_set_sensitive( widgets.save,FALSE);
  gtk_widget_set_sensitive( widgets.exit,TRUE);
  gtk_widget_set_sensitive( widgets.syntax,FALSE);
  gtk_widget_set_sensitive( widgets.compile,FALSE);
  gtk_widget_set_sensitive( widgets.redraw,FALSE);
  gtk_widget_set_sensitive( widgets.plcattribute,FALSE);
  gtk_widget_set_sensitive( widgets.winddelete,FALSE);
  gtk_widget_set_sensitive( widgets.cut,FALSE);
  gtk_widget_set_sensitive( widgets.paste,FALSE);
  gtk_widget_set_sensitive( widgets.undelete,FALSE);
  gtk_widget_set_sensitive( widgets.unselect,FALSE);
  gtk_widget_set_sensitive( widgets.connect,FALSE);
  gtk_widget_set_sensitive( widgets.del,FALSE);
  gtk_widget_set_sensitive( widgets.changetext,FALSE);
  gtk_widget_set_sensitive( widgets.expand,FALSE);
  gtk_widget_set_sensitive( widgets.compress,FALSE);
  gtk_widget_set_sensitive( widgets.tools_save,FALSE);
  gtk_widget_set_sensitive( widgets.tools_build,FALSE);

  return FOE__SUCCESS;
}

int WFoeGtk::get_selection( char *str, int len)
{
  int sts;

  sts = CoWowGtk::GetSelection( toplevel, str, len, objid_atom);
  if ( EVEN(sts))
    sts = CoWowGtk::GetSelection( toplevel, str, len, GDK_TARGET_STRING);

  return sts;
}

static gint confirm_delete_event( GtkWidget *w, GdkEvent *event, gpointer foe)
{
  g_object_set( ((WFoeGtk *)foe)->widgets.caution, "visible", FALSE, NULL);
  return TRUE;
}

void WFoeGtk::create_confirm_dialog()
{
  if ( widgets.caution) {
    g_object_set( widgets.caution, "visible", TRUE, NULL);
    return;
  }

  // Create a confirm window
  widgets.caution = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						"default-height", 150,
						"default-width", 400,
						"title", "Confirm",
						"window-position", GTK_WIN_POS_CENTER,
						NULL);
  g_signal_connect( widgets.caution, "delete_event", G_CALLBACK(confirm_delete_event), this);
  widgets.caution_label = gtk_label_new("");
  GtkWidget *confirm_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *confirm_ok = gtk_button_new_with_label( "Yes");
  gtk_widget_set_size_request( confirm_ok, 70, 25);
  g_signal_connect( confirm_ok, "clicked", 
  		    G_CALLBACK( yes_popupmsg), this);

  GtkWidget *confirm_no = gtk_button_new_with_label( "No");
  gtk_widget_set_size_request( confirm_no, 70, 25);
  g_signal_connect( confirm_no, "clicked", 
  		    G_CALLBACK( no_popupmsg), this);

  GtkWidget *confirm_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( confirm_cancel, 70, 25);
  g_signal_connect( confirm_cancel, "clicked", 
  		    G_CALLBACK( cancel_popupmsg), this);

  GtkWidget *confirm_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), confirm_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), widgets.caution_label, TRUE, TRUE, 15);

  GtkWidget *confirm_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(confirm_hboxbuttons), confirm_ok, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_hboxbuttons), confirm_no, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_hboxbuttons), confirm_cancel, FALSE, FALSE, 0);

  GtkWidget *confirm_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), confirm_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_vbox), confirm_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(widgets.caution), confirm_vbox);
  gtk_widget_show_all( widgets.caution);
}
