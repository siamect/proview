/* 
 * Proview   $Id: wb_vsel_gtk.cpp,v 1.2 2008-06-24 07:52:21 claes Exp $
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

/* wb_vsel_gtk.cpp -- select volume */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "pwr_systemclasses.h"
#include "rt_load.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb.h"
#include "wb_ldh.h"
#include "wb_foe.h"
#include "wb_utl_api.h"
#include "wb_lfu.h"
#include "co_login.h"
#include "co_login_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh_msg.h"
#include "co_msgwindow.h"
#include "co_wow_gtk.h"
#include "wb_vsel_gtk.h"

#define	BEEP	    putchar( '\7' );

/*  Local function prototypes.	*/

static void vsel_exit_qb_yes( void *ctx, void *cbdata);

static void vsel_exit_qb_yes( void *ctx, void *cbdata)
{
  WVsel *vsel = (WVsel *) ctx;

  if (vsel->vsel_bc_cancel != NULL)
    (vsel->vsel_bc_cancel) ();
  delete vsel;
}


/*************************************************************************
* Name:		void	vsel_activate_ok()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselGtk::activate_ok( GtkWidget *w, gpointer data)
{
  WVselGtk *vsel = (WVselGtk *)data;
  char		*text;
  char   	selected_text[80];
  GtkTreeIter   iter;
  pwr_tVid	vid;
  pwr_tVid	*volume_ptr;
  int	        volume_cnt;
  int		sts;
  char		*s;
  
  GtkTreeSelection *selection = gtk_tree_view_get_selection( 
				 GTK_TREE_VIEW(vsel->widgets.volumelist));
  if ( gtk_tree_selection_get_selected( selection, NULL, &iter)) {
    gtk_tree_model_get( GTK_TREE_MODEL( vsel->store), &iter, 0, 
			&text, -1);
    strcpy( selected_text, text);
  }

  for ( s = selected_text; *s != ' '; s++) ;
  *s = 0;

  sts = ldh_VolumeNameToId( vsel->wbctx, selected_text, &vid);
  if ( EVEN(sts)) return;

  volume_ptr = (pwr_tVolumeId *) calloc( vsel->volume_count, 
					 sizeof( pwr_tVolumeId));
  *volume_ptr = vid;
  volume_cnt = 1;

  if (vsel->vsel_bc_success != NULL)
    sts = (vsel->vsel_bc_success) ( vsel, volume_ptr, volume_cnt);
  free( (char *) volume_ptr);

  if ( ODD(sts)) {
    if (vsel->vsel_bc_cancel != NULL)
      (vsel->vsel_bc_cancel) ();
    delete vsel;
  }
  else if ( sts == LDH__VOLALRATT || LDH__VOLIDALREXI) {
    vsel->wow->DisplayError( "Error",
			     "Volume is already open");
  }
}

/*************************************************************************
* Name:		void	vsel_activate_cancel()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselGtk::activate_cancel( GtkWidget *w, gpointer data)
{
  WVsel *vsel = (WVsel *)data;

  if (vsel->vsel_bc_cancel != NULL)
    (vsel->vsel_bc_cancel) ();
  delete vsel;
}


/*************************************************************************
* Name:		void	vsel_activate_close()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselGtk::activate_close( GtkWidget *w, gpointer data)
{
  WVsel *vsel = (WVsel *)data;

  if ( (vsel->vsel_bc_time_to_exit) ( vsel->parent_ctx)) {
    vsel->wow->DisplayQuestion( vsel,
			 "Pwr exit", "Do you really want to exit ?",
			 vsel_exit_qb_yes, NULL, NULL);
  }
  else {
    if (vsel->vsel_bc_cancel != NULL)
      (vsel->vsel_bc_cancel) ();
    delete vsel;
  }
}

/*************************************************************************
* Name:		void	vsel_activate_showall()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselGtk::activate_showall( GtkWidget *w, gpointer data)
{
  WVsel *vsel = (WVsel *)data;
  pwr_tStatus sts;

  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM( w));
  vsel->all = set;

  sts = vsel->load_volumelist();
  WFoe::error_msg(sts);
}


/*************************************************************************
*
* Name:		int	vsel_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char *	name		I	Name of the created object
*
* Description:
*	Create a new login window
**************************************************************************/

WVselGtk::WVselGtk (
  pwr_tStatus   *status,
  void		*wv_parent_ctx,
  GtkWidget	*wv_parent_wid,
  char		*wv_name,
  ldh_tWBContext wv_wbctx,
  char		*volumename,
  int		(*bc_success)( void *, pwr_tVolumeId *, int),
  void		(*bc_cancel)(),
  int		(*bc_time_to_exit)( void *),
  int		show_volumes,
  wb_eType      wv_wb_type) : 
  WVsel(status,wv_parent_ctx,wv_name,wv_wbctx,volumename,bc_success,bc_cancel,
	bc_time_to_exit,show_volumes,wv_wb_type), parent_wid(wv_parent_wid)
{
  int		sts;
  pwr_tVolumeId volid, *volume_ptr;
  pwr_tVolumeId	volume;
  pwr_tClassId	classid;
  char		volname[80];
  int		size;
  int		volume_count;
  int		display_window;
  GtkCellRenderer *text_renderer;
  GtkTreeViewColumn *name_column;

  strcpy( volname, "");
  if ( volumename != NULL && *volumename != 0 && !show_volumes)
    /* Start the navigater for this volume */
    strcpy( volname, volumename);
  else if ( !show_volumes && wb_type != wb_eType_Buffer)
  {
    /* If there is only one volume in the db, select this volume */
    volume_count = 0;

    sts = ldh_GetVolumeList( wbctx, &volume);
    while ( ODD(sts) )
    {
      sts = ldh_GetVolumeClass( wbctx, volume, &classid);
      if (EVEN(sts)) { *status = sts; return; }
  
      if ( classid == pwr_eClass_ClassVolume ||
 	   classid == pwr_eClass_WorkBenchVolume ||
	   volume == ldh_cRtVolume)
      {
        sts = ldh_GetNextVolume( wbctx, volume, &volume);
        continue;
      }
      sts = ldh_VolumeIdToName( wbctx, volume, volname, sizeof(volname),
	    &size);
      if (EVEN(sts)) { *status = sts; return; }
      volume_count++;

      sts = ldh_GetNextVolume( wbctx, volume, &volume);
    }
    if ( volume_count != 1)
      strcpy( volname, "");
  }

  
  //  If volume name is supplied, find this volume and open the navigator.

  if ( strcmp( volname, "") && wb_type != wb_eType_Buffer)
    /* Check syntax, if new volumes is found, show the window */
    sts = check_volumelist( 1, &display_window);

  if ( strcmp( volname, "") && !display_window)
  {
    sts = ldh_VolumeNameToId( wbctx, volname, &volid);
    WFoe::error_msg(sts);
    if ( ODD(sts))
    {
      volume_ptr = (pwr_tVolumeId *) calloc( 1, sizeof( pwr_tVolumeId));
      *volume_ptr = volid;
      (vsel_bc_success) ( (void *)this, volume_ptr, 1);
      if (vsel_bc_cancel != NULL)
        (vsel_bc_cancel) ();

      *status = LOGIN__SUCCESS;
      return;
    }
    else
      printf( "-- Unable to open volume '%s', volume doesn't exist\n",
	volname);
  }


  // Create the window
  widgets.toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", 400,
					 "default-width", 500,
					 "title", name,
					 NULL);

  CoWowGtk::SetWindowIcon( widgets.toplevel);

  // Menu
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(widgets.toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(WVselGtk::activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions Entry
  GtkWidget *functions_showall = gtk_check_menu_item_new_with_mnemonic( "_Show All Volumes");
  g_signal_connect( functions_showall, "activate", 
		    G_CALLBACK(WVselGtk::activate_showall), this);
  gtk_widget_add_accelerator( functions_showall, "activate", accel_g,
			      'a', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_showall);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("F_unctions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // List widget
  store = gtk_list_store_new( 1, G_TYPE_STRING);

  widgets.volumelist = (GtkWidget *) g_object_new(GTK_TYPE_TREE_VIEW,
				 "model", store,
				 "rules-hint", TRUE,
				 "headers-clickable", TRUE,
				 "reorderable", TRUE,
				 "enable-search", TRUE,
				 "search-column", 0,
				 NULL);

  text_renderer = gtk_cell_renderer_text_new();
  name_column = gtk_tree_view_column_new_with_attributes( NULL,
							  text_renderer,
							  "text", 0,
							  NULL);

  g_object_set( name_column,
		"resizable", TRUE,
		"clickable", TRUE,
		NULL);

  gtk_tree_view_append_column( GTK_TREE_VIEW(widgets.volumelist), name_column);

  GtkWidget *ok_button = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( ok_button, 70, 25);
  g_signal_connect( ok_button, "clicked", 
 		    G_CALLBACK(activate_ok), this);

  GtkWidget *cancel_button = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( cancel_button, 70, 25);
  g_signal_connect( cancel_button, "clicked", 
 		    G_CALLBACK(activate_cancel), this);

  GtkWidget *hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(hboxbuttons), ok_button, FALSE, FALSE, 20);
  gtk_box_pack_end( GTK_BOX(hboxbuttons), cancel_button, FALSE, FALSE, 20);

  GtkWidget *scrolled_window = gtk_scrolled_window_new( NULL, NULL);
  gtk_container_add( GTK_CONTAINER( scrolled_window), widgets.volumelist);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), hboxbuttons, FALSE, FALSE, 10);

  gtk_container_add( GTK_CONTAINER(widgets.toplevel), vbox);
  gtk_widget_show_all( widgets.toplevel);

  // GtkTreeSelection *selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(widgets.volumelist));
  // gtk_tree_selection_set_mode( selection, GTK_SELECTION_SINGLE);

  sts = load_volumelist();
  WFoe::error_msg(sts);

  // Set input focus to the scrolled list widget
  gtk_widget_grab_focus( widgets.volumelist);


  wow = new CoWowGtk( widgets.toplevel);

  if ( wb_type != wb_eType_Buffer)
    sts = check_volumelist( 0, &display_window);

  *status = LOGIN__SUCCESS;
}



WVselGtk::~WVselGtk()
{

  /* Destroy the widget */
  gtk_widget_destroy( widgets.toplevel);

}

void WVselGtk::list_clear()
{
  gtk_list_store_clear( store);
}

void WVselGtk::list_add_item( char *str)
{
  GtkTreeIter iter;

  gtk_list_store_append( store, &iter);
  gtk_list_store_set( store, &iter, 0, str, -1);
}








