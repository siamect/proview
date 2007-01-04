/* 
 * Proview   $Id: wb_login_gtk.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "wb_login_msg.h"
#include "wb_login_gtk.h"
#include "wb_utl_api.h"
#include "co_api.h"
#include "co_dcli.h"

//
//	Callback from the pushbutton.
//
void WLoginGtk::activate_ok( GtkWidget *w, gpointer data)
{
  WLogin *loginctx = (WLogin *)data;
  loginctx->activate_ok();
}

//
//	Callback from the pushbutton.
//
void WLoginGtk::activate_cancel( GtkWidget *w, gpointer data)
{ 
  WLogin *loginctx = (WLogin *)data;
  loginctx->activate_cancel();
}

//
//	Callback when value changed.
//
void WLoginGtk::valchanged_passwordvalue( GtkWidget *w, gpointer data)
{
  WLogin *loginctx = (WLogin *)data;

  gtk_widget_activate( ((WLoginGtk *)loginctx)->widgets.okbutton);
}

//
//	Callback when value changed.
//
void WLoginGtk::valchanged_usernamevalue( GtkWidget *w, gpointer data)
{
  WLogin *loginctx = (WLogin *)data;
  gtk_widget_grab_focus( ((WLoginGtk *)loginctx)->widgets.passwordvalue);
}

static gint login_delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WLogin *loginctx = (WLogin *)data;

  loginctx->activate_cancel();
  return TRUE;
}

//
// Constructor
//
WLoginGtk::WLoginGtk( void		*wl_parent_ctx,
			  GtkWidget     *wl_parent_wid,
			  char		*wl_name,
			  char		*wl_groupname,
			  void		(* wl_bc_success)(),
			  void		(* wl_bc_cancel)(),
			  pwr_tStatus  	*status) :
  WLogin(wl_parent_ctx,wl_name,wl_groupname,wl_bc_success,wl_bc_cancel,status),
  parent_wid(wl_parent_wid)
{
  const int	window_width = 500;
  const int    	window_height = 200;

  // Create an input dialog
  widgets.toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						 "default-height", window_height,
						 "default-width", window_width,
						 "title", "Proview Login",
						 NULL);

  g_signal_connect( widgets.toplevel, "delete_event", G_CALLBACK(login_delete_event), this);

  widgets.usernamevalue = gtk_entry_new();
  gtk_widget_set_size_request( widgets.usernamevalue, -1, 20);
  g_signal_connect( widgets.usernamevalue, "activate", 
  		    G_CALLBACK(valchanged_usernamevalue), this);

  GtkWidget *usernamelabel = gtk_label_new("Username");
  gtk_widget_set_size_request( usernamelabel, -1, 20);

  widgets.passwordvalue = gtk_entry_new();
  gtk_widget_set_size_request( widgets.passwordvalue, -1, 20);
  g_signal_connect( widgets.passwordvalue, "activate", 
  		    G_CALLBACK(valchanged_passwordvalue), this);

  GtkWidget *passwordlabel = gtk_label_new("Password");
  gtk_widget_set_size_request( passwordlabel, -1, 20);
  gtk_entry_set_visibility( GTK_ENTRY(widgets.passwordvalue), FALSE);

  pwr_tFileName fname;
  dcli_translate_filename( fname, "$pwr_exe/proview_icon2.png");
  GtkWidget *india_image = gtk_image_new_from_file( fname);

  widgets.okbutton = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( widgets.okbutton, 70, 25);
  g_signal_connect( widgets.okbutton, "clicked", 
  		    G_CALLBACK(activate_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(activate_cancel), this);

  widgets.label = gtk_label_new("");

  GtkWidget *vbox1 = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox1), usernamelabel, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(vbox1), passwordlabel, FALSE, FALSE, 15);

  GtkWidget *vbox2 = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox2), widgets.usernamevalue, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(vbox2), widgets.passwordvalue, FALSE, FALSE, 15);

  GtkWidget *hbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), india_image, FALSE, FALSE, 25);
  gtk_box_pack_start( GTK_BOX(hbox), vbox1, FALSE, FALSE, 15);
  gtk_box_pack_end( GTK_BOX(hbox), vbox2, TRUE, TRUE, 15);

  GtkWidget *india_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(india_hboxbuttons), widgets.okbutton, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_hboxbuttons), india_cancel, FALSE, FALSE, 0);

  GtkWidget *india_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vbox), hbox, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(india_vbox), widgets.label, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(india_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_vbox), india_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(widgets.toplevel), india_vbox);

  gtk_widget_show_all( widgets.toplevel);
  gtk_widget_grab_focus( widgets.usernamevalue);

#if 0
  Arg	args[20];
  int	sts;
  int 	i;
  /* DRM database hierarchy related variables */
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		uid_filename[200] = {"pwr_exe:wb_login.uid"};
  char		*uid_filename_p = uid_filename;


  static MrmRegisterArg	reglist[] = {
  /* First the context variable */
  { "login_ctx", 0 },

  /* Callbacks for the controlled login widget */
  {"login_create_adb",(caddr_t)create_adb},
  {"login_create_label",(caddr_t)create_label},
  {"login_create_usernamevalue",(caddr_t)create_usernamevalue},
  {"login_create_passwordvalue",(caddr_t)create_passwordvalue},
  {"login_valchanged_usernamevalue",(caddr_t)valchanged_usernamevalue},
  {"login_valchanged_passwordvalue",(caddr_t)valchanged_passwordvalue},
  {"login_activate_ok",(caddr_t)activate_ok},
  {"login_activate_cancel",(caddr_t)activate_cancel},
  };

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts))
  {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

/*
 * Now start the module creation
 */

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /*
   * Create a new widget
   * Open the UID files (the output of the UIL compiler) in the hierarchy
   * Register the items DRM needs to bind for us.
   * Create a new widget
   * Close the hierarchy
   * Compile the additional button translations and augment and add actions
   */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open hierarchy\n");

  reglist[0].value = (caddr_t) this;

  MrmRegisterNames(reglist, reglist_num);

  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  
  /* Save the id of the top in the context */ 
  widgets.toplevel = XtCreatePopupShell (
    "login", topLevelShellGtkWidgetClass, parent_wid, args, i);

  /* the positioning of a top level can only be define after the creation
   *   of the widget . So i do it now: 
   *  use the parameters received x and y 
   */

  i=0;
  XtSetArg(args[i],XmNx,100);i++;
  XtSetArg(args[i],XmNy,100);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  XtSetValues( widgets.toplevel  ,args,i);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchGtkWidgetOverride(s_DRMh, "login_window",
    widgets.toplevel, name, args, 1, 
    &widgets.login_window, &dclass);

  if (sts != MrmSUCCESS) printf("can't fetch utedit widget\n");

  XtManageChild(widgets.login_window);

  /* SG 09.02.91 a top level should always be realized ! */
  XtPopup( widgets.toplevel, XtGrabNone );

  MrmCloseHierarchy(s_DRMh);


#endif
  *status = 1;
}

//
// Destructor
//
WLoginGtk::~WLoginGtk()
{
  /* Destroy the widget */
  gtk_widget_destroy( widgets.toplevel);
}

//
//	Get values in username and password.
//
pwr_tStatus WLoginGtk::get_values()
{
  char	passwd[40];
  char	username[40];
  char	*value;
  pwr_tStatus	sts;

  /* Get UserName */
  value = gtk_editable_get_chars( GTK_EDITABLE(widgets.usernamevalue), 0, -1);
  strcpy(username, value);
  g_free( value);

  /* Get Password */
  value = gtk_editable_get_chars( GTK_EDITABLE(widgets.passwordvalue), 0, -1);
  strcpy(passwd, value);

  sts = user_check( groupname, username, passwd);
  if ( EVEN(sts))
    return sts;

  return LOGIN__SUCCESS;
}

//
//	Displays a message in the login window.
//
void WLoginGtk::message( char *new_label)
{
  gtk_label_set_text( GTK_LABEL(widgets.label), new_label);
}

