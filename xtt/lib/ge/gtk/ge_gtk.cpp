/* 
 * Proview   $Id: ge_gtk.cpp,v 1.5 2007-01-12 07:58:06 claes Exp $
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

/* ge_gtk.cpp -- Graphical editor window */


#include "glow_std.h"

#if defined OS_VMS || defined OS_LINUX
#define LDH 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_xhelp.h"
#include "co_wow_gtk.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "glow.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_colpalwidget_gtk.h"
#include "glow_msg.h"


#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"
#include "ge_graph_gtk.h"
#include "ge_gtk.h"
#include "ge_subpalette_gtk.h"
#include "ge_subgraphs_gtk.h"
#include "ge_util.h"
#include "ge_msg.h"
#include "wb_wnav_selformat.h"
#include "co_wow_gtk.h"
#include "wb_nav_gtk.h"


static GtkWidget *image_button( char *filename)
{
  
  GtkWidget *image, *box;
  pwr_tFileName fname;

  box = gtk_button_new();
  // gtk_container_set_border_width(GTK_CONTAINER(box), 2);
  
  dcli_translate_filename( fname, filename);
  image = gtk_image_new_from_file( fname);

  gtk_container_add( GTK_CONTAINER( box), image);
  gtk_widget_show( image);

  return box;
}

static GtkWidget *image_widget( char *filename)
{
  
  GtkWidget *image;
  pwr_tFileName fname;

  dcli_translate_filename( fname, filename);
  image = gtk_image_new_from_file( fname);

  return image;
}


void GeGtk::create_list( char *title, char *texts,
			   void (action_cb)( void *, char *), void *ctx) 
{
  CoWowGtk wow( toplevel);
  wow.CreateList( title, texts, action_cb, ctx);
}

void GeGtk::subgraphs_new()
{
  int sts;

  subgraphs = new SubGraphsGtk( this, toplevel, "SubGraphs",
		(void *) graph->grow->ctx,
		&subgraphs_widget, &sts);
  subgraphs->message_cb = &Ge::message_cb;
  subgraphs->close_cb = &Ge::subgraphs_close_cb;
}

void GeGtk::set_title( char *title)
{
  gtk_window_set_title( GTK_WINDOW(toplevel), title);
}

void GeGtk::open_input_dialog( char *text, char *title,
			    char *init_text,
			    void (*x_india_ok_cb)( Ge *, char *))
{
  g_object_set( india_widget, 
		"visible", TRUE, 
		"title", title,
		NULL);

  gtk_label_set_text( GTK_LABEL(india_label), text);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(india_text), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(india_text), init_text, 
			    strlen(init_text), &pos);
  india_ok_cb = x_india_ok_cb;
}

void GeGtk::message( char severity, char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

void GeGtk::status_msg( char *pos_str)
{
  gtk_label_set_text( GTK_LABEL(cursor_position), pos_str);
}

void GeGtk::change_text_cb( void *ge_ctx, void *text_object, char *text)
{
  GeGtk *gectx = (GeGtk *)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open)  {
    g_object_set( gectx->cmd_input, "visible", FALSE, NULL);
    g_object_set( gectx->msg_label, "visible", TRUE, NULL);
    gectx->set_prompt( "");
    gectx->text_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    g_object_set( gectx->msg_label, "visible", FALSE, NULL);
    g_object_set( gectx->cmd_input, "visible", TRUE, NULL);
  }
  gectx->message( ' ', "");
  gtk_widget_grab_focus( gectx->cmd_input);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(gectx->cmd_input), text, strlen(text), &pos);

  // Select the text
  gtk_editable_set_position( GTK_EDITABLE(gectx->cmd_input), -1);
  gtk_editable_select_region( GTK_EDITABLE(gectx->cmd_input), 0, -1);

  gectx->set_prompt( "Enter text >");
  gectx->recall_entry->set_recall_buffer( gectx->text_recall);
  gectx->text_input_open = 1;
  gectx->current_text_object = text_object;
}

void GeGtk::change_name_cb( void *ge_ctx, void *text_object, char *text)
{
  GeGtk *gectx = (GeGtk *)ge_ctx;

  if ( gectx->text_input_open || gectx->name_input_open || 
       gectx->value_input_open)  {
    g_object_set( gectx->cmd_input, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->name_input_open = 0;
    gectx->text_input_open = 0;
    gectx->value_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    g_object_set( gectx->cmd_input, "visible", TRUE, NULL);
    g_object_set( gectx->msg_label, "visible", FALSE, NULL);
  }
  gectx->message( ' ', "");
  gtk_widget_grab_focus( gectx->cmd_input);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(gectx->cmd_input), text, strlen(text), &pos);

  // Select the text
  gtk_editable_set_position( GTK_EDITABLE(gectx->cmd_input), -1);
  gtk_editable_select_region( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gectx->set_prompt( "Enter name >");
  gectx->recall_entry->set_recall_buffer( gectx->name_recall);
  gectx->name_input_open = 1;
  gectx->current_text_object = text_object;
}

void GeGtk::change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  GeGtk    *gectx = (GeGtk *)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open ||
       gectx->name_input_open)  {
    g_object_set( gectx->cmd_input, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->value_input_open = 0;
    gectx->text_input_open = 0;
    gectx->name_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    g_object_set( gectx->cmd_input, "visible", TRUE, NULL);
    g_object_set( gectx->msg_label, "visible", FALSE, NULL);
  }
  gectx->message( ' ', "");
  gtk_widget_grab_focus( gectx->cmd_input);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(gectx->cmd_input), text, strlen(text), &pos);

  // Select the text
  // gtk_editable_set_position( GTK_EDITABLE(gectx->cmd_input), -1);
  // gtk_editable_select_region( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gectx->set_prompt( "Enter value >");
  gectx->recall_entry->set_recall_buffer( gectx->value_recall);
  gectx->value_input_open = 1;
  gectx->current_value_object = value_object;
}

int GeGtk::get_plant_select( char *select_name)
{
#ifdef LDH
  pwr_sAttrRef	attrref;
  int		is_attrref;
  int		sts;
  pwr_tAName   	str;
  pwr_tAName 	buff;

  if ( !ldhses) {
    sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), GDK_SELECTION_TYPE_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }

  sts = ((Nav *)plantctx)->get_select( &attrref, &is_attrref);
  if ( ODD( sts)) {
    if ( !wnav_format_selection( ldhses, attrref, 0, is_attrref, wnav_eSelectionMode_Normal,
				 0, 1, 1, buff))
      return 0;

    strcpy( select_name, buff);
    return 1;
  }
  else {
    sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), GDK_SELECTION_TYPE_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }
#else
  pwr_tStatus sts;
  GeGtk	*gectx = (GeGtk *)ge_ctx;

  sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), graph_atom);
  if ( ODD(sts))
    strcpy( select_name, str);
  else {
    sts = CoWowGtk::GetSelection( toplevel, str, sizeof(str), GDK_ATOM_STRING);
    if ( ODD(sts))
      strcpy( select_name, str);
  }
  return sts;
#endif
  return 0;
}

void GeGtk::open_yesnodia( char *text, char *title, 
	void (*yes_cb)( Ge *), void (*no_cb)( Ge *))
{
  if ( yesnodia_open)  {
    g_object_set( yesnodia_widget, "visible", FALSE, NULL);
    yesnodia_open = 0;
    return;
  }

  g_object_set( yesnodia_widget, "visible", TRUE, NULL);

  message( ' ', "");

  gtk_window_set_title( GTK_WINDOW(yesnodia_widget), title);
  gtk_label_set_text( GTK_LABEL(yesnodia_label), text);
  yesnodia_open = 1;
  yesnodia_yes_cb = yes_cb;
  yesnodia_no_cb = no_cb;
}

void GeGtk::confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  GeGtk *gectx = (GeGtk *)ge_ctx;

  if ( gectx->confirm_open)  {
    g_object_set( gectx->confirm_widget, "visible", FALSE, NULL);
    gectx->confirm_open = 0;
    return;
  }

  g_object_set( gectx->confirm_widget, "visible", TRUE, NULL);

  gectx->message( ' ', "");

  gtk_label_set_text( GTK_LABEL(gectx->confirm_label), text);
  gectx->confirm_open = 1;
  gectx->current_confirm_object = confirm_object;
}

void GeGtk::set_prompt( char *prompt)
{
  if ( strcmp(prompt, "") == 0) {
    g_object_set( cmd_prompt, "visible", FALSE, NULL);
    g_object_set( msg_label, "visible", TRUE, NULL);
  }
  else {
    g_object_set( msg_label, "visible", FALSE, NULL);
    g_object_set( cmd_prompt, "visible", TRUE, NULL);
    gtk_label_set_text( GTK_LABEL(cmd_prompt), prompt);
  }
}

//
//  Callbackfunctions from menu entries
//
void GeGtk::activate_change_text(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_change_text();
}

void GeGtk::activate_change_name(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_change_name();
}

void GeGtk::activate_preview_start(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_preview_start();
}

void GeGtk::activate_preview_stop(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_preview_stop();
}

void GeGtk::activate_cut(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_cut();
}

void GeGtk::activate_copy(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_copy();
}

void GeGtk::activate_rotate(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_rotate();
}

void GeGtk::activate_rotate90(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_rotate90();
}

void GeGtk::activate_flip_vert(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_flip_vert();
}

void GeGtk::activate_flip_horiz(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_flip_horiz();
}

void GeGtk::activate_pop(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_pop();
}

void GeGtk::activate_push(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_push();
}

void GeGtk::activate_edit_polyline(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_edit_polyline();
}

void GeGtk::activate_scale_equal(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_scale_equal();
}

void GeGtk::activate_move_horizontal(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_move_horizontal();
}

void GeGtk::activate_move_vertical(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_move_vertical();
}

void GeGtk::activate_move_reset(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_move_reset();
}

void GeGtk::activate_align_horiz_up(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_horiz_up();
}

void GeGtk::activate_align_horiz_down(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_horiz_down();
}

void GeGtk::activate_align_horiz_center(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_horiz_center();
}

void GeGtk::activate_align_vert_left(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_vert_left();
}

void GeGtk::activate_align_vert_right(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_vert_right();
}

void GeGtk::activate_align_vert_center(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_align_vert_center();
}

void GeGtk::activate_equid_vert_up(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_vert_up();
}

void GeGtk::activate_equid_vert_down(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_vert_down();
}

void GeGtk::activate_equid_vert_center(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_vert_center();
}

void GeGtk::activate_equid_horiz_left(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_horiz_left();
}

void GeGtk::activate_equid_horiz_right(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_horiz_right();
}

void GeGtk::activate_equid_horiz_center(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_equid_horiz_center();
}

void GeGtk::activate_select_cons(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_select_cons();
}

void GeGtk::activate_select_objects(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_select_objects();
}

void GeGtk::activate_group(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_group();
}

void GeGtk::activate_ungroup(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_ungroup();
}

void GeGtk::activate_connect(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_connect();
}

void GeGtk::activate_connectsecond(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_connectsecond();
}

void GeGtk::activate_objectattributes(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_objectattributes();
}

void GeGtk::activate_show_grid( GtkWidget *w, gpointer data)
{
  GeGtk *gectx = (GeGtk *)data;

  gboolean bset = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(gectx->show_grid_w));

  if ( w != gectx->show_grid_w)
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(gectx->show_grid_w), !bset);

  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(gectx->show_grid_w));
  ((Ge *)gectx)->activate_show_grid( set);
}

void GeGtk::activate_paste(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_paste();
}

void GeGtk::activate_command(GtkWidget *w, gpointer data)
{
  GeGtk *gectx = (GeGtk *)data;

  if ( gectx->command_open) {
    g_object_set( gectx->cmd_input, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->command_open = 0;
    return;
  }

  if ( gectx->text_input_open)
    gectx->text_input_open = 0;
  else if ( gectx->name_input_open)
    gectx->name_input_open = 0;
  else if ( gectx->value_input_open)
    gectx->text_input_open = 0;
  else {
    g_object_set( gectx->msg_label, "visible", FALSE, NULL);
    g_object_set( gectx->cmd_input, "visible", TRUE, NULL);
  }
  gectx->message( ' ', "");

  gtk_widget_grab_focus( gectx->cmd_input);

  gtk_editable_delete_text( GTK_EDITABLE(gectx->cmd_input), 0, -1);
  gectx->set_prompt( "ge >        ");
  gectx->recall_entry->set_recall_buffer( gectx->cmd_recall);
  gectx->command_open = 1;
}

void GeGtk::activate_exit(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_exit();
}

void GeGtk::activate_print(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_print();
}

void GeGtk::activate_new(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_new();
}

void GeGtk::activate_save(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_save();
}

void GeGtk::activate_save_as(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_save_as();
}

void GeGtk::activate_export_javabean(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_javabean();
}

void GeGtk::activate_export_javabean_as(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_javabean_as();
}

void GeGtk::activate_export_gejava(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_gejava();
}

void GeGtk::activate_export_gejava_as(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_gejava_as();
}

void GeGtk::activate_export_java(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_java();
}

void GeGtk::activate_export_java_as(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_export_java_as();
}

void GeGtk::graph_file_selected_cb( void *ctx, char *filename, wow_eFileSelType file_type)
{
  Ge *ge = (Ge *)ctx;
  ge->open_graph( filename);
}

void GeGtk::image_file_selected_cb( void *ctx, char *filename, wow_eFileSelType file_type)
{
  Ge *ge = (Ge *)ctx;
  pwr_tCmd cmd;
  pwr_tFileName str;
  char *s;

  dcli_translate_filename( str, "$pwrp_pop/");
  sprintf( cmd, "cp %s %s", filename, str);
  
  system( cmd);
  sprintf( str, "Image imported, %s", filename);
  ge->message( 'I', str);

  if ( (s = strrchr( filename, '/')))
    strcpy( str, s+1);
  else
    strcpy( str, filename);

  if ( (s = strrchr( str, '.')))
    *s = 0;

  sprintf( cmd, "Local-Images-%s", str);
  ge->subpalette->select_by_name( cmd);
}

void GeGtk::activate_import_graph(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->wow->CreateFileSelDia( "Graph Selection", (void *)gectx,
					graph_file_selected_cb, wow_eFileSelType_Graph);
}

void GeGtk::activate_import_image(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->wow->CreateFileSelDia( "Image Selection", (void *)gectx,
					image_file_selected_cb, wow_eFileSelType_Image);
}

void GeGtk::activate_generate_web(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_generate_web();
}

void GeGtk::activate_creanextpage(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_creanextpage();
}

void GeGtk::activate_nextpage(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_nextpage();
}

void GeGtk::activate_prevpage(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_prevpage();
}

void GeGtk::activate_graph_attr(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_graph_attr();
}

void GeGtk::activate_open(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_open();
}

void GeGtk::activate_subgraphs(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_subgraphs();
}

void GeGtk::activate_rect(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_rect( keep);
}

void GeGtk::activate_rectrounded(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_rectrounded( keep);
}

void GeGtk::activate_line(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_line( keep);
}

void GeGtk::activate_polyline(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_polyline( keep);
}

void GeGtk::activate_circle(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_circle( keep);
}

void GeGtk::activate_text(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_text( keep);
}

void GeGtk::activate_annot(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_annot( keep);
}

void GeGtk::activate_conpoint(GtkWidget *w, gpointer gectx)
{
  GdkEvent *e = gtk_get_current_event();
  bool keep = (e->button.state & GDK_SHIFT_MASK) ? true : false;

  ((Ge *)gectx)->activate_conpoint( keep);
}

void GeGtk::activate_fill( GtkWidget *w, gpointer gectx)
{
  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w));
  ((Ge *)gectx)->activate_fill( set);
}

void GeGtk::activate_border( GtkWidget *w, gpointer gectx)
{
  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w));
  ((Ge *)gectx)->activate_border( set);
}

void GeGtk::activate_shadow( GtkWidget *w, gpointer gectx)
{
  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w));
  ((Ge *)gectx)->activate_shadow( set);
}

void GeGtk::activate_incr_lightness(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_incr_lightness();
}

void GeGtk::activate_decr_lightness(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_decr_lightness();
}

void GeGtk::activate_incr_intensity(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_incr_intensity();
}

void GeGtk::activate_decr_intensity(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_decr_intensity();
}

void GeGtk::activate_incr_shift(GtkWidget *w, gpointer gectx){

  ((Ge *)gectx)->activate_incr_shift();
}

void GeGtk::activate_decr_shift(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_decr_shift();
}

void GeGtk::activate_scale(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_scale();
}

void GeGtk::activate_grid( GtkWidget *w, gpointer gectx)
{
  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w));
  ((Ge *)gectx)->activate_grid( set);
}

void GeGtk::activate_linewidth_1(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 1);
  printf( "linewidth 1\n");
}
void GeGtk::activate_linewidth_2(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 2);
  printf( "linewidth 2\n");
}
void GeGtk::activate_linewidth_3(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 3);
}
void GeGtk::activate_linewidth_4(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 4);
}
void GeGtk::activate_linewidth_5(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 5);
}
void GeGtk::activate_linewidth_6(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 6);
}
void GeGtk::activate_linewidth_7(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 7);
}
void GeGtk::activate_linewidth_8(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linewidth( 8);
}

void GeGtk::activate_linetype1(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype1();
}

void GeGtk::activate_linetype2(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype2();
}

void GeGtk::activate_linetype3(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype3();
}

void GeGtk::activate_linetype4(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype4();
}

void GeGtk::activate_linetype5(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype5();
}

void GeGtk::activate_linetype6(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype6();
}

void GeGtk::activate_linetype7(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_linetype7();
}

void GeGtk::activate_gridsize_4(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_gridsize( 1);
}

void GeGtk::activate_gridsize_3(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_gridsize( 0.5);
}

void GeGtk::activate_gridsize_2(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_gridsize( 0.2);
}

void GeGtk::activate_gridsize_1(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_gridsize( 0.1);
}

void GeGtk::activate_textsize_0(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 0);
}
void GeGtk::activate_textsize_1(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 1);
}

void GeGtk::activate_textsize_2(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 2);
}

void GeGtk::activate_textsize_3(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 3);
}

void GeGtk::activate_textsize_4(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 4);
}

void GeGtk::activate_textsize_5(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_textsize( 5);
}

void GeGtk::activate_textbold( GtkWidget *w, gpointer gectx)
{
  int set = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(w));
  ((Ge *)gectx)->activate_textbold( set);
}

void GeGtk::activate_zoom_in(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_zoom_in();
}

void GeGtk::activate_zoom_out(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_zoom_out();
}

void GeGtk::activate_zoom_reset(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_zoom_reset();
}

void GeGtk::activate_view_plant(GtkWidget *w, gpointer data)
{
#ifdef LDH
  Ge *ge = (Ge *)data;

  if ( !ge->ldhses)
    return;

  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM( ((GeGtk *)ge)->view_plant_w));
  if ( w != ((GeGtk *)ge)->view_plant_w) {
    set = !set;
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( ((GeGtk *)ge)->view_plant_w), set ? TRUE : FALSE);
  }

  if ( set) {
    g_object_set( ((GeGtk *)ge)->plant_widget, "visible", TRUE, NULL);
    g_object_set( ((GeGtk *)ge)->subpalette_widget, "visible", FALSE, NULL);
    ge->plant_mapped = 1;
    ge->subpalette_mapped = 0;
  }
  else {
    g_object_set( ((GeGtk *)ge)->plant_widget, "visible", FALSE, NULL);
    g_object_set( ((GeGtk *)ge)->subpalette_widget, "visible", TRUE, NULL);
    ge->plant_mapped = 0;
    ge->subpalette_mapped = 1;
  }
  ge->set_focus(0);
#endif
}


void GeGtk::activate_concorner_right(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_concorner_right();
}

void GeGtk::activate_concorner_rounded(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_concorner_rounded();
}

void GeGtk::activate_round_amount_1(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_round_amount( 0.2);
}

void GeGtk::activate_round_amount_2(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_round_amount( 0.5);
}

void GeGtk::activate_round_amount_3(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_round_amount( 1.0);
}

void GeGtk::activate_round_amount_4(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_round_amount( 2.0);
}

void GeGtk::activate_round_amount_5(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_round_amount( 4.0);
}

void GeGtk::activate_contype_straight(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_straight();
}

void GeGtk::activate_contype_routed(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_routed();
}

void GeGtk::activate_contype_stronearr(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_stronearr();
}

void GeGtk::activate_contype_stepdiv(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_stepdiv();
}

void GeGtk::activate_contype_stepconv(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_stepconv();
}

void GeGtk::activate_contype_transdiv(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_transdiv();
}

void GeGtk::activate_contype_transconv(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_contype_transconv();
}

void GeGtk::activate_condir_center(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_condir_center();
}

void GeGtk::activate_condir_left(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_condir_left();
}

void GeGtk::activate_condir_right(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_condir_right();
}

void GeGtk::activate_condir_up(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_condir_up();
}

void GeGtk::activate_condir_down(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_condir_down();
}

void GeGtk::activate_background_color(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_background_color();
}

void GeGtk::activate_help(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_help();
}

void GeGtk::activate_india_ok(GtkWidget *w, gpointer gectx)
{
  char *text;

  text = gtk_editable_get_chars( GTK_EDITABLE(((GeGtk *)gectx)->india_text), 0, -1);
  g_object_set( ((GeGtk *)gectx)->india_widget, "visible", FALSE, NULL);
  ((Ge *)gectx)->activate_india_ok( text);
  g_free( text);
}
void GeGtk::activate_india_cancel(GtkWidget *w, gpointer gectx)
{
  g_object_set( ((GeGtk *)gectx)->india_widget, "visible", FALSE, NULL);
}
void GeGtk::activate_yesnodia_yes(GtkWidget *w, gpointer gectx)
{
  g_object_set( ((GeGtk *)gectx)->yesnodia_widget, "visible", FALSE, NULL);
  ((Ge *)gectx)->activate_yesnodia_yes();
}
void GeGtk::activate_yesnodia_no(GtkWidget *w, gpointer gectx)
{
  g_object_set( ((GeGtk *)gectx)->yesnodia_widget, "visible", FALSE, NULL);
  ((Ge *)gectx)->activate_yesnodia_no();
}
void GeGtk::activate_yesnodia_cancel(GtkWidget *w, gpointer gectx)
{
  g_object_set( ((GeGtk *)gectx)->yesnodia_widget, "visible", FALSE, NULL);
  ((Ge *)gectx)->activate_yesnodia_cancel();
}
void GeGtk::activate_confirm_ok(GtkWidget *w, gpointer gectx)
{
  g_object_set( ((GeGtk *)gectx)->confirm_widget, "visible", FALSE, NULL);
  ((Ge *)gectx)->activate_confirm_ok();
}
void GeGtk::activate_confirm_cancel(GtkWidget *w, gpointer gectx)
{
  ((Ge *)gectx)->activate_confirm_cancel();;
  g_object_set( ((GeGtk *)gectx)->confirm_widget, "visible", FALSE, NULL);
}

void GeGtk::valchanged_cmd_input( GtkWidget *w, gpointer data)
{
  GeGtk *gectx = (GeGtk *)data;
  int sts;
  char *text;

  text = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);

  if ( gectx->text_input_open) {
    gectx->graph->change_text( gectx->current_text_object, text);
    g_object_set( w, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->text_input_open = 0;
    gectx->set_focus(0);
  }
  else if ( gectx->name_input_open) {
    gectx->graph->change_name( gectx->current_text_object, text);
    g_object_set( w, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->name_input_open = 0;
    gectx->set_focus(0);
  }
  else if ( gectx->value_input_open) {
    gectx->graph->change_value( gectx->current_value_object, text);
    g_object_set( w, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->value_input_open = 0;
    gectx->set_focus(0);
  }
  else if ( gectx->command_open) {
    sts = gectx->graph->command( text);
    g_object_set( w, "visible", FALSE, NULL);
    gectx->set_prompt( "");
    gectx->command_open = 0;
    gectx->set_focus(0);
  }
  g_free(text);
}

void GeGtk::update()
{
  gboolean set;

  set = graph->grid ? TRUE : FALSE;
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(grid_on_w), set);

  set = graph->get_show_grid() ? TRUE : FALSE;
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_grid_w), set);

  if ( fabs( graph->grid_size_x - 0.1) < DBL_EPSILON)
    gtk_option_menu_set_history( GTK_OPTION_MENU(grid_size_w), 3);
  else if ( fabs( graph->grid_size_x - 0.2) < DBL_EPSILON)
    gtk_option_menu_set_history( GTK_OPTION_MENU(grid_size_w), 2);
  else if ( fabs( graph->grid_size_x - 0.5) < DBL_EPSILON)
    gtk_option_menu_set_history( GTK_OPTION_MENU(grid_size_w), 1);
  else
    gtk_option_menu_set_history( GTK_OPTION_MENU(grid_size_w), 0);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer gectx)
{
  ((Ge *)gectx)->activate_exit();

  return TRUE;
}

static gint india_delete_event( GtkWidget *w, GdkEvent *event, gpointer gectx)
{
  // Hide
  g_object_set( w, "visible", FALSE, NULL);
  return TRUE;
}

static gint confirm_delete_event( GtkWidget *w, GdkEvent *event, gpointer gectx)
{
  // Hide
  ((Ge *)gectx)->confirm_open = 0;
  g_object_set( w, "visible", FALSE, NULL);
  return TRUE;
}

static gint yesnodia_delete_event( GtkWidget *w, GdkEvent *event, gpointer gectx)
{
  // Hide
  ((Ge *)gectx)->yesnodia_open = 0;
  g_object_set( w, "visible", FALSE, NULL);
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

gboolean GeGtk::ge_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  GeGtk *gectx = (GeGtk *)data;

  if ( gectx->graph->trace_started) {
    if ( gectx && gectx->graph)
      gectx->graph->set_inputfocus(1);
  }
  else {
    if ( gectx->value_input_open || gectx->name_input_open ||
	 gectx->text_input_open || gectx->command_open)
      gtk_widget_grab_focus( gectx->cmd_input);
    else
      gectx->set_focus(0);
  }
  return FALSE;
}

GeGtk::~GeGtk()
{
  if ( subgraphs)
    delete subgraphs;
  delete recall_entry;
  delete text_recall;
  delete name_recall;
  delete value_recall;
  delete cmd_recall;
  delete graph;
  delete wow;
  gtk_widget_destroy( india_widget);
  gtk_widget_destroy( confirm_widget);
  gtk_widget_destroy( yesnodia_widget);
  gtk_widget_destroy( toplevel);

  if ( exit_when_close)
    exit(0);
}

GeGtk::GeGtk( 	void 	*x_parent_ctx, 
		GtkWidget *x_parent_widget,
		ldh_tSesContext	x_ldhses,
		int	x_exit_when_close,
		char	*graph_name) :
  Ge( x_parent_ctx, x_ldhses, x_exit_when_close),
  parent_wid(x_parent_widget), graph_atom(0)
{
  int		path_cnt;
  char		*path;
  char          systemname[80];
  const int	window_width = 900;
  const int    	window_height = 800;
  const int    	palette_width = 200;
  pwr_tFileName	fname;
  char		title[200];
  char		tmp_name[200];
  char 		*s;

  strcpy( title, "PwR Ge");

  if ( graph_name) {
    cdh_ToLower( tmp_name, graph_name);
    tmp_name[0] = toupper( tmp_name[0]);
    if ( (s = strrchr( tmp_name, '.')))
      *s = 0;
    strcat( title, " ");
    strcat( title, tmp_name);
  }

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", title,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(ge_action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);
  GtkToolbar *tools2 = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  // File Entry
  GtkWidget *file_new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
  g_signal_connect(file_new, "activate", G_CALLBACK(activate_new), this);

  GtkWidget *file_open = gtk_menu_item_new_with_mnemonic( "_Open...");
  g_signal_connect( file_open, "activate", 
		    G_CALLBACK(activate_open), this);

  GtkWidget *file_save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_g);
  g_signal_connect(file_save, "activate", G_CALLBACK(activate_save), this);

  GtkWidget *file_save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel_g);
  g_signal_connect(file_save_as, "activate", G_CALLBACK(activate_save_as), this);

  GtkWidget *file_graph_attr = gtk_menu_item_new_with_mnemonic( "_Graph attributes...");
  g_signal_connect( file_graph_attr, "activate", 
		    G_CALLBACK(activate_graph_attr), this);

  GtkWidget *file_subgraphs = gtk_menu_item_new_with_mnemonic( "_Loaded Subgraphs...");
  g_signal_connect( file_subgraphs, "activate", 
		    G_CALLBACK(activate_subgraphs), this);

  GtkWidget *file_export_java = gtk_menu_item_new_with_mnemonic( "_Export Java");
  g_signal_connect( file_export_java, "activate", 
		    G_CALLBACK(activate_export_java), this);

  GtkWidget *file_export_java_as = gtk_menu_item_new_with_mnemonic( "E_xport Java as...");
  g_signal_connect( file_export_java_as, "activate", 
		    G_CALLBACK(activate_export_java_as), this);

  GtkWidget *file_import_graph = gtk_menu_item_new_with_mnemonic( "_Import Graph...");
  g_signal_connect( file_import_graph, "activate", 
		    G_CALLBACK(activate_import_graph), this);

  GtkWidget *file_import_image = gtk_menu_item_new_with_mnemonic( "_Import Image...");
  g_signal_connect( file_import_image, "activate", 
		    G_CALLBACK(activate_import_image), this);

  GtkWidget *file_nextpage = gtk_menu_item_new_with_mnemonic( "_Next Page");
  g_signal_connect( file_nextpage, "activate", 
		    G_CALLBACK(activate_nextpage), this);
  gtk_widget_add_accelerator( file_nextpage, "activate", accel_g,
			      'n', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *file_prevpage = gtk_menu_item_new_with_mnemonic( "P_revious Page");
  g_signal_connect( file_prevpage, "activate", 
		    G_CALLBACK(activate_prevpage), this);
  gtk_widget_add_accelerator( file_prevpage, "activate", accel_g,
			      'j', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *file_creanextpage = gtk_menu_item_new_with_mnemonic( "_Create Next Page");
  g_signal_connect( file_creanextpage, "activate", 
		    G_CALLBACK(activate_creanextpage), this);
  gtk_widget_add_accelerator( file_creanextpage, "activate", accel_g,
			      'n', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *file_print = gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT, accel_g);
  g_signal_connect(file_print, "activate", G_CALLBACK(activate_print), this);

  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_new);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_save);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_save_as);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_graph_attr);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_subgraphs);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_export_java);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_export_java_as);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_import_graph);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_import_image);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_nextpage);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_prevpage);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_creanextpage);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Edit entry
  GtkWidget *edit_cut = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_g);
  g_signal_connect(edit_cut, "activate", G_CALLBACK(activate_cut), this);

  GtkWidget *edit_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_g);
  g_signal_connect(edit_copy, "activate", G_CALLBACK(activate_copy), this);

  GtkWidget *edit_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_g);
  g_signal_connect(edit_paste, "activate", G_CALLBACK(activate_paste), this);

  GtkWidget *edit_rotate = gtk_menu_item_new_with_mnemonic( "_Rotate...");
  g_signal_connect( edit_rotate, "activate", 
		    G_CALLBACK(activate_rotate), this);
  gtk_widget_add_accelerator( edit_rotate, "activate", accel_g,
			      'r', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *edit_polyline = gtk_menu_item_new_with_mnemonic( "_Edit Polyline");
  g_signal_connect( edit_polyline, "activate", 
		    G_CALLBACK(activate_edit_polyline), this);

  GtkWidget *edit_change_text = gtk_menu_item_new_with_mnemonic( "Change _Text");
  g_signal_connect( edit_change_text, "activate", 
		    G_CALLBACK(activate_change_text), this);
  gtk_widget_add_accelerator( edit_change_text, "activate", accel_g,
			      't', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *edit_change_name = gtk_menu_item_new_with_mnemonic( "Change _Name");
  g_signal_connect( edit_change_name, "activate", 
		    G_CALLBACK(activate_change_name), this);

  GtkWidget *edit_command = gtk_menu_item_new_with_mnemonic( "_Command");
  g_signal_connect( edit_command, "activate", 
		    G_CALLBACK(activate_command), this);
  gtk_widget_add_accelerator( edit_command, "activate", accel_g,
			      'b', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *edit_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_cut);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_copy);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_paste);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_rotate);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_polyline);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_change_text);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_change_name);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_command);

  GtkWidget *edit = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));

  // Functions entry
  GtkWidget *functions_background_color = gtk_menu_item_new_with_mnemonic( "_Set Background Color");
  g_signal_connect( functions_background_color, "activate", 
		    G_CALLBACK(activate_background_color), this);

  GtkWidget *functions_pop = gtk_menu_item_new_with_mnemonic( "_Pop");
  g_signal_connect( functions_pop, "activate", 
		    G_CALLBACK(activate_pop), this);

  GtkWidget *functions_push = gtk_menu_item_new_with_mnemonic( "P_ush");
  g_signal_connect( functions_push, "activate", 
		    G_CALLBACK(activate_push), this);

  GtkWidget *functions_scale_equal = gtk_menu_item_new_with_mnemonic( "_Scale Equal");
  g_signal_connect( functions_scale_equal, "activate", 
		    G_CALLBACK(activate_scale_equal), this);
  gtk_widget_add_accelerator( functions_scale_equal, "activate", accel_g,
			      'e', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkWidget *functions_move_horizontal = gtk_menu_item_new_with_mnemonic( "_Horizontal");
  g_signal_connect( functions_move_horizontal, "activate", 
		    G_CALLBACK(activate_move_horizontal), this);
  gtk_widget_add_accelerator( functions_move_horizontal, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *functions_move_vertical = gtk_menu_item_new_with_mnemonic( "_Vertical");
  g_signal_connect( functions_move_vertical, "activate", 
		    G_CALLBACK(activate_move_vertical), this);
  gtk_widget_add_accelerator( functions_move_vertical, "activate", accel_g,
			      'g', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *functions_move_no = gtk_image_menu_item_new_from_stock(GTK_STOCK_NO, accel_g);
  g_signal_connect(functions_move_no, "activate", G_CALLBACK(activate_move_reset), this);


  GtkWidget *functions_move_restrictions = gtk_menu_item_new_with_mnemonic( "_Move Restrictions");
  GtkMenu *functions_move_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_move_menu), functions_move_horizontal);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_move_menu), functions_move_vertical);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_move_menu), functions_move_no);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_move_restrictions), 
			    GTK_WIDGET(functions_move_menu));

  // Submenu Align Vertical
  GtkWidget *functions_align_vert_left = gtk_menu_item_new_with_mnemonic( "_Left");
  g_signal_connect( functions_align_vert_left, "activate", 
		    G_CALLBACK(activate_align_vert_left), this);

  GtkWidget *functions_align_vert_right = gtk_menu_item_new_with_mnemonic( "_Right");
  g_signal_connect( functions_align_vert_right, "activate", 
		    G_CALLBACK(activate_align_vert_right), this);

  GtkWidget *functions_align_vert_center = gtk_menu_item_new_with_mnemonic( "_Center");
  g_signal_connect( functions_align_vert_center, "activate", 
		    G_CALLBACK(activate_align_vert_center), this);

  GtkWidget *functions_align_vert = gtk_menu_item_new_with_mnemonic( "_Vertical");
  GtkMenu *functions_align_vert_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_vert_menu), functions_align_vert_left);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_vert_menu), functions_align_vert_right);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_vert_menu), functions_align_vert_center);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_align_vert),
			    GTK_WIDGET(functions_align_vert_menu));


  // Submenu Align Horizontal
  GtkWidget *functions_align_horiz_top = gtk_menu_item_new_with_mnemonic( "_Top");
  g_signal_connect( functions_align_horiz_top, "activate",
		    G_CALLBACK(activate_align_horiz_up), this);

  GtkWidget *functions_align_horiz_bottom = gtk_menu_item_new_with_mnemonic( "_Bottom");
  g_signal_connect( functions_align_horiz_bottom, "activate",
		    G_CALLBACK(activate_align_horiz_down), this);

  GtkWidget *functions_align_horiz_center = gtk_menu_item_new_with_mnemonic( "_Center");
  g_signal_connect( functions_align_horiz_center, "activate",
		    G_CALLBACK(activate_align_horiz_center), this);

  GtkWidget *functions_align_horiz = gtk_menu_item_new_with_mnemonic( "_Horizontal");
  GtkMenu *functions_align_horiz_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_horiz_menu), functions_align_horiz_top);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_horiz_menu), functions_align_horiz_bottom);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_horiz_menu), functions_align_horiz_center);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_align_horiz),
			    GTK_WIDGET(functions_align_horiz_menu));

  // Submenu Align
  GtkWidget *functions_align = gtk_menu_item_new_with_mnemonic( "_Align");
  GtkMenu *functions_align_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_menu), functions_align_vert);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_align_menu), functions_align_horiz);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_align),
			    GTK_WIDGET(functions_align_menu));

  // Submenu Equid Horizontal
  GtkWidget *functions_equid_horiz_left = gtk_menu_item_new_with_mnemonic( "_Left");
  g_signal_connect( functions_equid_horiz_left, "activate", 
		    G_CALLBACK(activate_equid_horiz_left), this);

  GtkWidget *functions_equid_horiz_right = gtk_menu_item_new_with_mnemonic( "_Right");
  g_signal_connect( functions_equid_horiz_right, "activate", 
		    G_CALLBACK(activate_equid_horiz_right), this);

  GtkWidget *functions_equid_horiz_center = gtk_menu_item_new_with_mnemonic( "_Center");
  g_signal_connect( functions_equid_horiz_center, "activate", 
		    G_CALLBACK(activate_equid_horiz_center), this);

  GtkWidget *functions_equid_horiz = gtk_menu_item_new_with_mnemonic( "_Horizontal");
  GtkMenu *functions_equid_horiz_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_horiz_menu), functions_equid_horiz_left);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_horiz_menu), functions_equid_horiz_right);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_horiz_menu), functions_equid_horiz_center);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_equid_horiz),
			    GTK_WIDGET(functions_equid_horiz_menu));


  // Submenu Equid Vertical
  GtkWidget *functions_equid_vert_top = gtk_menu_item_new_with_mnemonic( "_Top");
  g_signal_connect( functions_equid_vert_top, "activate",
		    G_CALLBACK(activate_equid_vert_up), this);

  GtkWidget *functions_equid_vert_bottom = gtk_menu_item_new_with_mnemonic( "_Bottom");
  g_signal_connect( functions_equid_vert_bottom, "activate",
		    G_CALLBACK(activate_equid_vert_down), this);

  GtkWidget *functions_equid_vert_center = gtk_menu_item_new_with_mnemonic( "_Center");
  g_signal_connect( functions_equid_vert_center, "activate",
		    G_CALLBACK(activate_equid_vert_center), this);

  GtkWidget *functions_equid_vert = gtk_menu_item_new_with_mnemonic( "_Vertical");
  GtkMenu *functions_equid_vert_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_vert_menu), functions_equid_vert_top);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_vert_menu), functions_equid_vert_bottom);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_vert_menu), functions_equid_vert_center);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_equid_vert),
			    GTK_WIDGET(functions_equid_vert_menu));

  // Submenu Equid
  GtkWidget *functions_equid = gtk_menu_item_new_with_mnemonic( "Equi_Distance");
  GtkMenu *functions_equid_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_menu), functions_equid_horiz);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_equid_menu), functions_equid_vert);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_equid),
			    GTK_WIDGET(functions_equid_menu));

  GtkWidget *functions_select_cons = gtk_menu_item_new_with_mnemonic( "Select All _Connections");
  g_signal_connect( functions_select_cons, "activate",
		    G_CALLBACK(activate_select_cons), this);

  GtkWidget *functions_select_objects = gtk_menu_item_new_with_mnemonic( "Select All _Objects");
  g_signal_connect( functions_select_objects, "activate",
		    G_CALLBACK(activate_select_objects), this);

  GtkWidget *functions_group = gtk_menu_item_new_with_mnemonic( "_Group");
  g_signal_connect( functions_group, "activate",
		    G_CALLBACK(activate_group), this);

  GtkWidget *functions_ungroup = gtk_menu_item_new_with_mnemonic( "_Ungroup");
  g_signal_connect( functions_ungroup, "activate",
		    G_CALLBACK(activate_ungroup), this);

  GtkWidget *functions_connect = gtk_menu_item_new_with_mnemonic( "_Connect");
  g_signal_connect( functions_connect, "activate",
		    G_CALLBACK(activate_connect), this);
  gtk_widget_add_accelerator( functions_connect, "activate", accel_g,
			      'q', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *functions_connectsecond = gtk_menu_item_new_with_mnemonic( "C_onnect Second");
  g_signal_connect( functions_connectsecond, "activate",
		    G_CALLBACK(activate_connectsecond), this);

  GtkWidget *functions_objectattr = gtk_menu_item_new_with_mnemonic( "_Object Attributes");
  g_signal_connect( functions_objectattr, "activate",
		    G_CALLBACK(activate_objectattributes), this);
  gtk_widget_add_accelerator( functions_objectattr, "activate", accel_g,
			      'a', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *functions_show_grid = gtk_menu_item_new_with_mnemonic( "S_how Grid");
  g_signal_connect( functions_show_grid, "activate",
  		    G_CALLBACK(activate_show_grid), this);
  gtk_widget_add_accelerator( functions_show_grid, "activate", accel_g,
  			      'd', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_background_color);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_pop);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_push);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_scale_equal);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_move_restrictions);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_align);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_equid);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_select_cons);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_select_objects);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_group);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_ungroup);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_connect);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_connectsecond);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_objectattr);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_show_grid);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("_Functions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));


  // Menu Connections
  // Submenu Conpoint Direction
  GtkWidget *cons_condir_center = gtk_check_menu_item_new_with_mnemonic( "_Center");
  g_signal_connect( cons_condir_center, "activate", 
		    G_CALLBACK(activate_condir_center), this);

  GtkWidget *cons_condir_left = gtk_check_menu_item_new_with_mnemonic( "_Left");
  g_signal_connect( cons_condir_left, "activate", 
		    G_CALLBACK(activate_condir_left), this);

  GtkWidget *cons_condir_right = gtk_check_menu_item_new_with_mnemonic( "_Right");
  g_signal_connect( cons_condir_right, "activate", 
		    G_CALLBACK(activate_condir_right), this);

  GtkWidget *cons_condir_up = gtk_check_menu_item_new_with_mnemonic( "_Up");
  g_signal_connect( cons_condir_up, "activate",
		    G_CALLBACK(activate_condir_up), this);

  GtkWidget *cons_condir_down = gtk_check_menu_item_new_with_mnemonic( "_Down");
  g_signal_connect( cons_condir_down, "activate", 
		    G_CALLBACK(activate_condir_down), this);

  GtkWidget *cons_condir = gtk_menu_item_new_with_mnemonic( "Conpoint _Direction");
  GtkMenu *cons_condir_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_condir_menu), cons_condir_center);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_condir_menu), cons_condir_left);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_condir_menu), cons_condir_right);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_condir_menu), cons_condir_up);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_condir_menu), cons_condir_down);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(cons_condir),GTK_WIDGET(cons_condir_menu));

  // Submenu Corners
  GtkWidget *cons_corners_right = gtk_check_menu_item_new_with_mnemonic( "_Right");
  g_signal_connect( cons_corners_right, "activate",
		    G_CALLBACK(activate_concorner_right), this);

  GtkWidget *cons_corners_rounded = gtk_check_menu_item_new_with_mnemonic( "R_ounded");
  g_signal_connect( cons_corners_rounded, "activate",
		    G_CALLBACK(activate_concorner_rounded), this);

  GtkWidget *cons_corners = gtk_menu_item_new_with_mnemonic( "_Corners");
  GtkMenu *cons_corners_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_corners_menu), cons_corners_right);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_corners_menu), cons_corners_rounded);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(cons_corners),GTK_WIDGET(cons_corners_menu));

  // Submenu Corner Round Amount
  GtkWidget *cons_round_amount_1 = gtk_check_menu_item_new_with_mnemonic( "0.2");
  g_signal_connect( cons_round_amount_1, "activate",
		    G_CALLBACK(activate_round_amount_1), this);
  GtkWidget *cons_round_amount_2 = gtk_check_menu_item_new_with_mnemonic( "0.5");
  g_signal_connect( cons_round_amount_2, "activate",
		    G_CALLBACK(activate_round_amount_2), this);
  GtkWidget *cons_round_amount_3 = gtk_check_menu_item_new_with_mnemonic( "1.0");
  g_signal_connect( cons_round_amount_3, "activate",
		    G_CALLBACK(activate_round_amount_3), this);
  GtkWidget *cons_round_amount_4 = gtk_check_menu_item_new_with_mnemonic( "2.0");
  g_signal_connect( cons_round_amount_4, "activate",
		    G_CALLBACK(activate_round_amount_4), this);
  GtkWidget *cons_round_amount_5 = gtk_check_menu_item_new_with_mnemonic( "4.0");
  g_signal_connect( cons_round_amount_5, "activate",
		    G_CALLBACK(activate_round_amount_5), this);

  GtkWidget *cons_round_amount = gtk_menu_item_new_with_mnemonic( "Corners _Round Amount");
  GtkMenu *cons_round_amount_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_round_amount_menu), cons_round_amount_1);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_round_amount_menu), cons_round_amount_2);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_round_amount_menu), cons_round_amount_3);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_round_amount_menu), cons_round_amount_4);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_round_amount_menu), cons_round_amount_5);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(cons_round_amount),GTK_WIDGET(cons_round_amount_menu));

  // Submenu Connections Type
  GtkWidget *cons_type_straight = gtk_check_menu_item_new_with_mnemonic( "_Straight");
  g_signal_connect( cons_type_straight, "activate",
		    G_CALLBACK(activate_contype_straight), this);
  GtkWidget *cons_type_routed = gtk_check_menu_item_new_with_mnemonic( "_Routed");
  g_signal_connect( cons_type_routed, "activate",
		    G_CALLBACK(activate_contype_routed), this);
  GtkWidget *cons_type_straightonearrow = gtk_check_menu_item_new_with_mnemonic( "Straight _One Arrow");
  g_signal_connect( cons_type_straightonearrow, "activate",
		    G_CALLBACK(activate_contype_stronearr), this);
  GtkWidget *cons_type_stepdiv = gtk_check_menu_item_new_with_mnemonic( "St_ep Diverge");
  g_signal_connect( cons_type_stepdiv, "activate",
		    G_CALLBACK(activate_contype_stepdiv), this);
  GtkWidget *cons_type_stepconv = gtk_check_menu_item_new_with_mnemonic( "Ste_p Converge");
  g_signal_connect( cons_type_stepdiv, "activate",
		    G_CALLBACK(activate_contype_stepdiv), this);
  GtkWidget *cons_type_transdiv = gtk_check_menu_item_new_with_mnemonic( "_Trans Diverge");
  g_signal_connect( cons_type_transdiv, "activate",
		    G_CALLBACK(activate_contype_transdiv), this);
  GtkWidget *cons_type_transconv = gtk_check_menu_item_new_with_mnemonic( "Tr_ans Converge");
  g_signal_connect( cons_type_transconv, "activate",
		    G_CALLBACK(activate_contype_transconv), this);

  GtkWidget *cons_type = gtk_menu_item_new_with_mnemonic( "_Type");
  GtkMenu *cons_type_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_straight);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_routed);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_straightonearrow);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_stepdiv);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_stepconv);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_transdiv);
  gtk_menu_shell_append(GTK_MENU_SHELL(cons_type_menu), cons_type_transconv);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(cons_type),GTK_WIDGET(cons_type_menu));


  // Menu View
  GtkWidget *view_preview_start = gtk_menu_item_new_with_mnemonic( "_Preview");
  g_signal_connect( view_preview_start, "activate", 
		    G_CALLBACK(activate_preview_start), this);

  GtkWidget *view_preview_stop = gtk_menu_item_new_with_mnemonic( "Preview _Stop");
  g_signal_connect(view_preview_stop, "activate", G_CALLBACK(activate_preview_stop), this);

  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(activate_zoom_reset), this);

  view_plant_w = gtk_check_menu_item_new_with_mnemonic( "Vi_ew Plant");
  g_signal_connect( view_plant_w, "activate", 
		    G_CALLBACK(activate_view_plant), this);
  gtk_widget_add_accelerator( view_plant_w, "activate", accel_g,
			      'p', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_preview_start);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_preview_stop);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_plant_w);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar buttons

  // Shape buttons
  GtkWidget *tools_rect = image_button( "$pwr_exe/ge_rect.png");
  g_signal_connect(tools_rect, "clicked", G_CALLBACK(activate_rect), this);
  gtk_toolbar_append_widget( tools, tools_rect, "Draw a rectangle", "");

  GtkWidget *tools_rectrounded = image_button( "$pwr_exe/ge_roundedrect.png");
  g_signal_connect(tools_rectrounded, "clicked", G_CALLBACK(activate_rectrounded), this);
  gtk_toolbar_append_widget( tools, tools_rectrounded, "Draw a rounded rectangle", "");

  GtkWidget *tools_circ = image_button( "$pwr_exe/ge_circ.png");
  g_signal_connect(tools_circ, "clicked", G_CALLBACK(activate_circle), this);
  gtk_toolbar_append_widget( tools, tools_circ, "Draw a circle or elipse", "");

  GtkWidget *tools_line = image_button( "$pwr_exe/ge_line.png");
  g_signal_connect(tools_line, "clicked", G_CALLBACK(activate_line), this);
  gtk_toolbar_append_widget( tools, tools_line, "Draw a line", "");

  GtkWidget *tools_polyline = image_button( "$pwr_exe/ge_polyline.png");
  g_signal_connect(tools_polyline, "clicked", G_CALLBACK(activate_polyline), this);
  gtk_toolbar_append_widget( tools, tools_polyline, "Draw a polyline or polygon", "");

  GtkWidget *tools_text = image_button( "$pwr_exe/ge_text.png");
  g_signal_connect(tools_text, "clicked", G_CALLBACK(activate_text), this);
  gtk_toolbar_append_widget( tools, tools_text, "Draw a text", "");

  GtkWidget *tools_annot = image_button( "$pwr_exe/ge_annot.png");
  g_signal_connect(tools_annot, "clicked", G_CALLBACK(activate_annot), this);
  gtk_toolbar_append_widget( tools, tools_annot, "Insert an annotation", "");

  GtkWidget *tools_conpoint = image_button( "$pwr_exe/ge_conpoint.png");
  g_signal_connect(tools_conpoint, "clicked", G_CALLBACK(activate_conpoint), this);
  gtk_toolbar_append_widget( tools, tools_conpoint, "Insert a connectionpoint", "");

  // Border checkbutton
  GtkWidget *tools_border = gtk_toggle_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_border.png");
  gtk_container_add( GTK_CONTAINER(tools_border), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_border, "clicked", G_CALLBACK(activate_border), this);
  gtk_toolbar_append_widget( tools, tools_border, "Set border property on selected objects, and as default", "");

  // Fill checkbutton
  GtkWidget *tools_fill = gtk_toggle_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_fill.png");
  gtk_container_add( GTK_CONTAINER(tools_fill), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_fill, "clicked", G_CALLBACK(activate_fill), this);
  gtk_toolbar_append_widget( tools, tools_fill, "Set fill property on selected objects, and as default", "");

  // 3D checkbutton
  GtkWidget *tools_shadow = gtk_toggle_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_3d.png");
  gtk_container_add( GTK_CONTAINER(tools_shadow), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_shadow, "clicked", G_CALLBACK(activate_shadow), this);
  gtk_toolbar_append_widget( tools, tools_shadow, "Set 3D property on selected objects, and as default", "");


  // Pop button
  GtkWidget *tools_pop = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_pop.png");
  gtk_container_add( GTK_CONTAINER(tools_pop), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_pop, "clicked", G_CALLBACK(activate_pop), this);
  gtk_toolbar_append_widget( tools, tools_pop, "Pop selected objects", "");


  // Push button
  GtkWidget *tools_push = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_push.png");
  gtk_container_add( GTK_CONTAINER(tools_push), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_push, "clicked", G_CALLBACK(activate_push), this);
  gtk_toolbar_append_widget( tools, tools_push, "Push selected objects", "");


  // Move horizontal
  GtkWidget *tools_move_horiz = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_move_horiz.png");
  gtk_container_add( GTK_CONTAINER(tools_move_horiz), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_move_horiz, "clicked", G_CALLBACK(activate_move_horizontal), this);
  gtk_toolbar_append_widget( tools, tools_move_horiz, "Move horizontal", "");

  // Move vertical
  GtkWidget *tools_move_vert = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_move_vert.png");
  gtk_container_add( GTK_CONTAINER(tools_move_vert), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_move_vert, "clicked", G_CALLBACK(activate_move_vertical), this);
  gtk_toolbar_append_widget( tools, tools_move_vert, "Move vertical", "");

  // Group button
  GtkWidget *tools_group = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_group.png");
  gtk_container_add( GTK_CONTAINER(tools_group), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_group, "clicked", G_CALLBACK(activate_group), this);
  gtk_toolbar_append_widget( tools, tools_group, "Group selected objects", "");

  // Ungroup button
  GtkWidget *tools_ungroup = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_ungroup.png");
  gtk_container_add( GTK_CONTAINER(tools_ungroup), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_ungroup, "clicked", G_CALLBACK(activate_ungroup), this);
  gtk_toolbar_append_widget( tools, tools_ungroup, "Split selected group", "");


  // Align horizontal up
  GtkWidget *tools_align_horiz_down = image_button( "$pwr_exe/ge_align_horiz_bottom.png");
  g_signal_connect(tools_align_horiz_down, "clicked", G_CALLBACK(activate_align_horiz_down), this);
  gtk_toolbar_append_widget( tools, tools_align_horiz_down, "Align selected objects bottom side horizontal", "");

  // Align horizontal center
  GtkWidget *tools_align_horiz_center = image_button( "$pwr_exe/ge_align_horiz_center.png");
  g_signal_connect(tools_align_horiz_center, "clicked", G_CALLBACK(activate_align_horiz_center), this);
  gtk_toolbar_append_widget( tools, tools_align_horiz_center, "Align selected objects center horizontal", "");

  // Align horizontal up
  GtkWidget *tools_align_horiz_up = image_button( "$pwr_exe/ge_align_horiz_top.png");
  g_signal_connect(tools_align_horiz_up, "clicked", G_CALLBACK(activate_align_horiz_up), this);
  gtk_toolbar_append_widget( tools, tools_align_horiz_up, "Align selected objects top side horizontal", "");

  // Align vertical left
  GtkWidget *tools_align_vert_left = image_button( "$pwr_exe/ge_align_vert_left.png");
  g_signal_connect(tools_align_vert_left, "clicked", G_CALLBACK(activate_align_vert_left), this);
  gtk_toolbar_append_widget( tools, tools_align_vert_left, "Align selected objects left side vertical", "");

  // Align vertontal center
  GtkWidget *tools_align_vert_center = image_button( "$pwr_exe/ge_align_vert_center.png");
  g_signal_connect(tools_align_vert_center, "clicked", G_CALLBACK(activate_align_vert_center), this);
  gtk_toolbar_append_widget( tools, tools_align_vert_center, "Align selected objects center vertical", "");

  // Align vertical right
  GtkWidget *tools_align_vert_right = image_button( "$pwr_exe/ge_align_vert_right.png");
  g_signal_connect(tools_align_vert_right, "clicked", G_CALLBACK(activate_align_vert_right), this);
  gtk_toolbar_append_widget( tools, tools_align_vert_right, "Align selected objects right side vertcial", "");

  // Equidistance vertical left
  GtkWidget *tools_equid_horiz_left = image_button( "$pwr_exe/ge_equidist_left.png");
  g_signal_connect(tools_equid_horiz_left, "clicked", G_CALLBACK(activate_equid_horiz_left), this);
  gtk_toolbar_append_widget( tools, tools_equid_horiz_left, "Set equal distance between objects left side horizontal", "");

  // Equidistance horizontal bottom
  GtkWidget *tools_equid_vert_down = image_button( "$pwr_exe/ge_equidist_bottom.png");
  g_signal_connect(tools_equid_vert_down, "clicked", G_CALLBACK(activate_equid_vert_down), this);
  gtk_toolbar_append_widget( tools, tools_equid_vert_down, "Set equal distance between objects bottom side vertical", "");

  // View Planthierarchy
  GtkWidget *tools_view_plant = image_button( "$pwr_exe/foe_navpalette.png");
  g_signal_connect(tools_view_plant, "clicked", G_CALLBACK(activate_view_plant), this);
  gtk_toolbar_append_widget( tools, tools_view_plant, "View plant hierarchy", "");

  // Open Object attributes
  GtkWidget *tools_open_object = image_button( "$pwr_exe/xtt_object.png");
  g_signal_connect(tools_open_object, "clicked", G_CALLBACK(activate_objectattributes), this);
  gtk_toolbar_append_widget( tools, tools_open_object, "Open selected object", "");

  // Scale button
  GtkWidget *tools_scale = image_button( "$pwr_exe/ge_scale.png");
  g_signal_connect(tools_scale, "clicked", G_CALLBACK(activate_scale), this);
  gtk_toolbar_append_widget( tools2, tools_scale, "Scale selected objects", "");

  // Rotate 90 button
  GtkWidget *tools_rotate90 = image_button( "$pwr_exe/ge_flip.png");
  g_signal_connect(tools_rotate90, "clicked", G_CALLBACK(activate_rotate90), this);
  gtk_toolbar_append_widget( tools2, tools_rotate90, "Rotate selected objects 90 degrees", "");

  // Mirror horizontal button
  GtkWidget *tools_mirror_horiz = image_button( "$pwr_exe/ge_mirrorhorizontal.png");
  g_signal_connect(tools_mirror_horiz, "clicked", G_CALLBACK(activate_flip_horiz), this);
  gtk_toolbar_append_widget( tools2, tools_mirror_horiz, "Mirror selected objects horizontal", "");

  // Mirror vertical button
  GtkWidget *tools_mirror_vert = image_button( "$pwr_exe/ge_mirrorvertical.png");
  g_signal_connect(tools_mirror_vert, "clicked", G_CALLBACK(activate_flip_vert), this);
  gtk_toolbar_append_widget( tools2, tools_mirror_vert, "Mirror selected objects vertical", "");

  // Zoom buttons
  GtkWidget *tools_zoom_in = image_button( "$pwr_exe/ge_zoom_in.png");
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoom_in), this);
  gtk_toolbar_append_widget( tools2, tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = image_button( "$pwr_exe/ge_zoom_out.png");
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoom_out), this);
  gtk_toolbar_append_widget( tools2, tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_zoom_reset = image_button( "$pwr_exe/ge_zoom_reset.png");
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(activate_zoom_reset), this);
  gtk_toolbar_append_widget( tools2, tools_zoom_reset, "Zoom reset", "");


  // Line width option menu
  GtkWidget *tools_linewidth_1 = gtk_image_menu_item_new_with_label( "Linewidth 1");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_1.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_1), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_1, "activate", G_CALLBACK(activate_linewidth_1), this);
  GtkWidget *tools_linewidth_2 = gtk_image_menu_item_new_with_label( "Linewidth 2");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_2.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_2), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_2, "activate", G_CALLBACK(activate_linewidth_2), this);
  GtkWidget *tools_linewidth_3 = gtk_image_menu_item_new_with_label( "Linewidth 3");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_3.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_3), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_3, "activate", G_CALLBACK(activate_linewidth_3), this);
  GtkWidget *tools_linewidth_4 = gtk_image_menu_item_new_with_label( "Linewidth 4");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_4.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_4), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_4, "activate", G_CALLBACK(activate_linewidth_4), this);
  GtkWidget *tools_linewidth_5 = gtk_image_menu_item_new_with_label( "Linewidth 5");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_5.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_5), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_5, "activate", G_CALLBACK(activate_linewidth_5), this);
  GtkWidget *tools_linewidth_6 = gtk_image_menu_item_new_with_label( "Linewidth 6");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_6.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_6), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_6, "activate", G_CALLBACK(activate_linewidth_6), this);
  GtkWidget *tools_linewidth_7 = gtk_image_menu_item_new_with_label( "Linewidth 7");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_7.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_7), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_7, "activate", G_CALLBACK(activate_linewidth_7), this);
  GtkWidget *tools_linewidth_8 = gtk_image_menu_item_new_with_label( "Linewidth 8");
  dcli_translate_filename( fname, "$pwr_exe/ge_linewidth_8.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linewidth_8), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linewidth_8, "activate", G_CALLBACK(activate_linewidth_8), this);

  GtkMenu *linewidth_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_1);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_2);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_3);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_4);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_5);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_6);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_7);
  gtk_menu_shell_append( GTK_MENU_SHELL(linewidth_menu), tools_linewidth_8);
  GtkWidget *tools_linewidth_omenu = (GtkWidget *)g_object_new( GTK_TYPE_OPTION_MENU, 
								"menu", linewidth_menu, NULL);

  gtk_option_menu_set_history( GTK_OPTION_MENU(tools_linewidth_omenu), 0);
  gtk_toolbar_append_widget( tools2, tools_linewidth_omenu, "Line Width", "");

  // Line type option menu
  GtkWidget *tools_linetype_1 = gtk_image_menu_item_new_with_label( "Linetype 1");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_1.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_1), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_1, "activate", G_CALLBACK(activate_linetype1), this);
  GtkWidget *tools_linetype_2 = gtk_image_menu_item_new_with_label( "Linetype 2");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_2.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_2), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_2, "activate", G_CALLBACK(activate_linetype2), this);
  GtkWidget *tools_linetype_3 = gtk_image_menu_item_new_with_label( "Linetype 3");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_3.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_3), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_3, "activate", G_CALLBACK(activate_linetype3), this);
  GtkWidget *tools_linetype_4 = gtk_image_menu_item_new_with_label( "Linetype 4");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_4.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_4), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_4, "activate", G_CALLBACK(activate_linetype4), this);
  GtkWidget *tools_linetype_5 = gtk_image_menu_item_new_with_label( "Linetype 5");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_5.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_5), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_5, "activate", G_CALLBACK(activate_linetype5), this);
  GtkWidget *tools_linetype_6 = gtk_image_menu_item_new_with_label( "Linetype 6");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_6.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_6), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_6, "activate", G_CALLBACK(activate_linetype6), this);
  GtkWidget *tools_linetype_7 = gtk_image_menu_item_new_with_label( "Linetype 7");
  dcli_translate_filename( fname, "$pwr_exe/ge_linetype_7.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_linetype_7), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_linetype_7, "activate", G_CALLBACK(activate_linetype7), this);

  GtkMenu *linetype_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_1);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_2);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_3);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_4);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_5);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_6);
  gtk_menu_shell_append( GTK_MENU_SHELL(linetype_menu), tools_linetype_7);
  GtkWidget *tools_linetype_omenu = (GtkWidget *)g_object_new( GTK_TYPE_OPTION_MENU, 
								"menu", linetype_menu, NULL);

  gtk_option_menu_set_history( GTK_OPTION_MENU(tools_linetype_omenu), 0);
  gtk_toolbar_append_widget( tools2, tools_linetype_omenu, "Line Type", "");

  // Text size options menu
  GtkWidget *tools_textsize_8 = gtk_image_menu_item_new_with_label( "Textsize 8");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_8.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_8), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_8, "activate", G_CALLBACK(activate_textsize_0), this);
  GtkWidget *tools_textsize_10 = gtk_image_menu_item_new_with_label( "Textsize 10");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_10.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_10), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_10, "activate", G_CALLBACK(activate_textsize_1), this);
  GtkWidget *tools_textsize_12 = gtk_image_menu_item_new_with_label( "Textsize 12");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_12.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_12), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_12, "activate", G_CALLBACK(activate_textsize_2), this);
  GtkWidget *tools_textsize_14 = gtk_image_menu_item_new_with_label( "Textsize 14");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_14.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_14), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_14, "activate", G_CALLBACK(activate_textsize_3), this);
  GtkWidget *tools_textsize_18 = gtk_image_menu_item_new_with_label( "Textsize 18");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_18.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_18), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_18, "activate", G_CALLBACK(activate_textsize_4), this);
  GtkWidget *tools_textsize_24 = gtk_image_menu_item_new_with_label( "Textsize 24");
  dcli_translate_filename( fname, "$pwr_exe/ge_textsize_24.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(tools_textsize_24), 
				 gtk_image_new_from_file( fname));
  g_signal_connect(tools_textsize_24, "activate", G_CALLBACK(activate_textsize_5), this);

  GtkMenu *textsize_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_8);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_10);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_12);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_14);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_18);
  gtk_menu_shell_append( GTK_MENU_SHELL(textsize_menu), tools_textsize_24);
  GtkWidget *tools_textsize_omenu = (GtkWidget *)g_object_new( GTK_TYPE_OPTION_MENU, 
								"menu", textsize_menu, NULL);

  gtk_option_menu_set_history( GTK_OPTION_MENU(tools_textsize_omenu), 0);
  gtk_toolbar_append_widget( tools2, tools_textsize_omenu, "Text Size", "");

  // Bold checkbutton
  GtkWidget *tools_textbold = gtk_check_button_new_with_label( "Bold");
  gtk_toolbar_append_widget( tools2, tools_textbold, "Set bold on selected text, and as default", "");
  g_signal_connect(tools_textbold, "toggled", G_CALLBACK(activate_textbold), this);

  // Grid size options menu
  GtkWidget *tools_gridsize_4 = gtk_menu_item_new_with_label( "Gridsize 1.0");
  g_signal_connect(tools_gridsize_4, "activate", G_CALLBACK(activate_gridsize_4), this);
  GtkWidget *tools_gridsize_3 = gtk_menu_item_new_with_label( "Gridsize 0.5");
  g_signal_connect(tools_gridsize_3, "activate", G_CALLBACK(activate_gridsize_3), this);
  GtkWidget *tools_gridsize_2 = gtk_menu_item_new_with_label( "Gridsize 0.2");
  g_signal_connect(tools_gridsize_2, "activate", G_CALLBACK(activate_gridsize_2), this);
  GtkWidget *tools_gridsize_1 = gtk_menu_item_new_with_label( "Gridsize 0.1");
  g_signal_connect(tools_gridsize_1, "activate", G_CALLBACK(activate_gridsize_1), this);

  GtkMenu *gridsize_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append( GTK_MENU_SHELL(gridsize_menu), tools_gridsize_4);
  gtk_menu_shell_append( GTK_MENU_SHELL(gridsize_menu), tools_gridsize_3);
  gtk_menu_shell_append( GTK_MENU_SHELL(gridsize_menu), tools_gridsize_2);
  gtk_menu_shell_append( GTK_MENU_SHELL(gridsize_menu), tools_gridsize_1);
  GtkWidget *tools_gridsize_omenu = (GtkWidget *)g_object_new( GTK_TYPE_OPTION_MENU, 
								"menu", gridsize_menu, NULL);

  grid_size_w = tools_gridsize_omenu;
  gtk_option_menu_set_history( GTK_OPTION_MENU(tools_gridsize_omenu), 0);
  gtk_toolbar_append_widget( tools2, tools_gridsize_omenu, "Grid Size", "");

  // Show grid checkbutton
  show_grid_w = gtk_toggle_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_showgrid.png");
  gtk_container_add( GTK_CONTAINER(show_grid_w), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(show_grid_w, "clicked", G_CALLBACK(activate_show_grid), this);
  gtk_toolbar_append_widget( tools2, show_grid_w, "Show grid", "");

  // Snap to grid checkbutton
  grid_on_w = gtk_toggle_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_snap.png");
  gtk_container_add( GTK_CONTAINER(grid_on_w), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(grid_on_w, "clicked", G_CALLBACK(activate_grid), this);
  gtk_toolbar_append_widget( tools2, grid_on_w, "Snap to grid", "");

  // Brightness
  GtkWidget *tools_decr_lightness = image_button( "$pwr_exe/ge_arrowleft.png");
  gtk_toolbar_append_widget( tools2, tools_decr_lightness, "Decrease brightness", "");
  g_signal_connect(tools_decr_lightness, "clicked", G_CALLBACK(activate_decr_lightness), this);

  gtk_toolbar_append_widget( tools2, image_widget( "$pwr_exe/ge_brightness.png"), "", "");

  GtkWidget *tools_incr_lightness = image_button( "$pwr_exe/ge_arrowright.png");
  gtk_toolbar_append_widget( tools2, tools_incr_lightness, "Increase brightness", "");
  g_signal_connect(tools_incr_lightness, "clicked", G_CALLBACK(activate_incr_lightness), this);

  // Separator
  gtk_toolbar_append_widget( tools2, GTK_WIDGET(gtk_vseparator_new()), "", "");

  // Intensity
  GtkWidget *tools_decr_intensity = image_button( "$pwr_exe/ge_arrowleft.png");
  gtk_toolbar_append_widget( tools2, tools_decr_intensity, "Decrease color intensity", "");
  g_signal_connect(tools_decr_intensity, "clicked", G_CALLBACK(activate_decr_intensity), this);

  gtk_toolbar_append_widget( tools2, image_widget( "$pwr_exe/ge_color.png"), "", "");

  GtkWidget *tools_incr_intensity = image_button( "$pwr_exe/ge_arrowright.png");
  gtk_toolbar_append_widget( tools2, tools_incr_intensity, "Increase color intensity", "");
  g_signal_connect(tools_incr_intensity, "clicked", G_CALLBACK(activate_incr_intensity), this);

  // Separator
  gtk_toolbar_append_widget( tools2, GTK_WIDGET(gtk_vseparator_new()), "", "");

  // Shift
  GtkWidget *tools_decr_shift = image_button( "$pwr_exe/ge_arrowleft.png");
  gtk_toolbar_append_widget( tools2, tools_decr_shift, "Shift color", "");
  g_signal_connect(tools_decr_shift, "clicked", G_CALLBACK(activate_decr_shift), this);

  gtk_toolbar_append_widget( tools2, image_widget( "$pwr_exe/ge_colorshift.png"), "", "");

  GtkWidget *tools_incr_shift = image_button( "$pwr_exe/ge_arrowright.png");
  gtk_toolbar_append_widget( tools2, tools_incr_shift, "Shift color", "");
  g_signal_connect(tools_incr_shift, "clicked", G_CALLBACK(activate_incr_shift), this);


  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);

  // cmd_input = gtk_entry_new();
  text_recall = new CoWowRecall();
  name_recall = new CoWowRecall();
  value_recall = new CoWowRecall();
  cmd_recall = new CoWowRecall();
  recall_entry = new CoWowEntryGtk(cmd_recall);
  cmd_input = recall_entry->widget();

  gtk_widget_set_size_request( cmd_input, -1, 25);
  cursor_position = gtk_label_new( "");
  g_signal_connect( cmd_input, "activate", 
  		    G_CALLBACK(valchanged_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 0);
  gtk_box_pack_end( GTK_BOX(statusbar), cursor_position, FALSE, FALSE, 0);
  gtk_widget_show_all( statusbar);


  // Graph component
  int sts;

  graph = new GraphGtk( this, GTK_WIDGET(toplevel), "GraphGtk", &grow_widget, &sts, "pwrp_pop:",
			graph_eMode_Development, 1);
  graph->message_cb = &Ge::message_cb;
  graph->get_current_subgraph_cb = &Ge::subpalette_get_select;
  graph->get_current_colors_cb = &Ge::colorpalette_get_current;
  graph->set_current_colors_cb = &Ge::colorpalette_set_current;
  graph->cursor_motion_cb = &Ge::status_msg;
  graph->change_text_cb = &change_text_cb;
  graph->change_name_cb = &change_name_cb;
  graph->change_value_cb = &change_value_cb;
  graph->confirm_cb = &confirm_cb;
  graph->command_cb = &Ge::command_cb;
  graph->get_plant_select_cb = &Ge::get_plant_select_cb;
  graph->load_graph_cb = &Ge::load_graph_cb;
  graph->set_focus_cb = &Ge::set_focus_cb;
  graph->traverse_focus_cb = &Ge::traverse_focus;
  graph->get_ldhses_cb = &Ge::get_ldhses_cb;

  // Vertical palette pane
  GtkWidget *vpaned1 = gtk_vpaned_new();
  GtkWidget *vpaned2 = gtk_vpaned_new();

  GtkWidget *palbox = gtk_hbox_new( FALSE, 0);

  // SubGraphs palette
  subpalette = new SubPaletteGtk( this, palbox, "SubGraphs",
		&subpalette_widget, &sts);
  subpalette->message_cb = &Ge::message_cb;
  subpalette->set_focus_cb = &Ge::set_focus_cb;
  subpalette->traverse_focus_cb = &Ge::traverse_focus;
  gtk_box_pack_start( GTK_BOX(palbox), subpalette_widget, TRUE, TRUE, 0);
  gtk_widget_show( subpalette_widget);
  subpalette_mapped = 1;

  // Color palette
  colpal_main_widget = scrolledcolpalwidgetgtk_new( 
		Ge::init_colorpalette_cb, this,
		&colorpalette_widget);
  gtk_widget_show( colpal_main_widget);

#ifdef LDH
  if ( ldhses) {
    plantctx = new NavGtk( this, palbox, "Plant",
			   ldhses, "NavigatorW1", 
			   &plant_widget, &sts);
    ((NavGtk *)plantctx)->get_plant_select_cb = Ge::get_plant_select_cb;
    gtk_box_pack_start( GTK_BOX(palbox), plant_widget, TRUE, TRUE, 0);
  }
#endif

  gtk_paned_pack1( GTK_PANED(vpaned1), palbox, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(vpaned1), colpal_main_widget, FALSE, TRUE);

  ((GraphGtk *)graph)->create_navigator( vpaned1);
  gtk_paned_pack1( GTK_PANED(vpaned2), vpaned1, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(vpaned2), ((GraphGtk *)graph)->nav_widget, FALSE, TRUE);
  gtk_widget_show( ((GraphGtk *)graph)->nav_widget);


  // Horizontal pane
  GtkWidget *hpaned = gtk_hpaned_new();
  gtk_paned_pack1( GTK_PANED(hpaned), grow_widget, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(hpaned), vpaned2, FALSE, TRUE);
  gtk_widget_show( vpaned1);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools2), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(hpaned), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  // gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(cons_type_routed), TRUE);

  gtk_paned_set_position( GTK_PANED(hpaned), window_width - palette_width);
  gtk_paned_set_position( GTK_PANED(vpaned1), window_height - 380);
  gtk_paned_set_position( GTK_PANED(vpaned2), window_height - 290);

#ifdef LDH
  if ( ldhses)
    g_object_set( plant_widget, "visible", FALSE, NULL);
#endif
  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);

  subpalette->get_path( &path_cnt, &path);
  graph->set_subgraph_path( path_cnt, path);
  wow = new CoWowGtk( toplevel);

  // Create an input dialog
  india_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					     "default-height", 150,
					     "default-width", 350,
					     "title", "Input Dialog",
					     "window-position", GTK_WIN_POS_CENTER,
					     NULL);
  g_signal_connect( india_widget, "delete_event", G_CALLBACK(india_delete_event), this);
  india_text = gtk_entry_new();
  g_signal_connect( india_text, "activate", 
  		    G_CALLBACK(activate_india_ok), this);
  india_label = gtk_label_new("Graph Name");
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(activate_india_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(activate_india_cancel), this);

  GtkWidget *india_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_label, FALSE, FALSE, 15);
  gtk_box_pack_end( GTK_BOX(india_hboxtext), india_text, TRUE, TRUE, 30);

  GtkWidget *india_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(india_hboxbuttons), india_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_hboxbuttons), india_cancel, FALSE, FALSE, 0);

  GtkWidget *india_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vbox), india_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(india_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_vbox), india_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(india_widget), india_vbox);
  gtk_widget_show_all( india_widget);
  g_object_set( india_widget, "visible", FALSE, NULL);

  // Create a confirm window
  confirm_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					       "default-height", 150,
					       "default-width", 350,
					       "title", "Confirm",
					       "window-position", GTK_WIN_POS_CENTER,
					       NULL);
  g_signal_connect( confirm_widget, "delete_event", G_CALLBACK(confirm_delete_event), this);
  confirm_label = gtk_label_new("Graph Name");
  GtkWidget *confirm_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_WARNING,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *confirm_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( confirm_ok, 70, 25);
  g_signal_connect( confirm_ok, "clicked", 
  		    G_CALLBACK(activate_confirm_ok), this);
  GtkWidget *confirm_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( confirm_cancel, 70, 25);
  g_signal_connect( confirm_cancel, "clicked", 
  		    G_CALLBACK(activate_confirm_cancel), this);

  GtkWidget *confirm_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), confirm_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), confirm_label, TRUE, TRUE, 15);

  GtkWidget *confirm_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(confirm_hboxbuttons), confirm_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_hboxbuttons), confirm_cancel, FALSE, FALSE, 0);

  GtkWidget *confirm_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), confirm_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_vbox), confirm_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(confirm_widget), confirm_vbox);
  gtk_widget_show_all( confirm_widget);
  g_object_set( confirm_widget, "visible", FALSE, NULL);

							  
  // Create a Yes No Dialog
  yesnodia_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						"default-height", 150,
						"default-width", 350,
						"title", "Confirm",
						"window-position", GTK_WIN_POS_CENTER,
						NULL);
  g_signal_connect( yesnodia_widget, "delete_event", G_CALLBACK(yesnodia_delete_event), this);
  yesnodia_label = gtk_label_new("Graph Name");
  GtkWidget *yesnodia_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *yesnodia_yes = gtk_button_new_with_label( "Yes");
  gtk_widget_set_size_request( yesnodia_yes, 70, 25);
  g_signal_connect( yesnodia_yes, "clicked", 
  		    G_CALLBACK(activate_yesnodia_yes), this);
  GtkWidget *yesnodia_no = gtk_button_new_with_label( "No");
  gtk_widget_set_size_request( yesnodia_no, 70, 25);
  g_signal_connect( yesnodia_no, "clicked", 
  		    G_CALLBACK(activate_yesnodia_no), this);
  GtkWidget *yesnodia_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( yesnodia_cancel, 70, 25);
  g_signal_connect( yesnodia_cancel, "clicked", 
  		    G_CALLBACK(activate_yesnodia_cancel), this);

  GtkWidget *yesnodia_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(yesnodia_hboxtext), yesnodia_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(yesnodia_hboxtext), yesnodia_label, TRUE, TRUE, 15);

  GtkWidget *yesnodia_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(yesnodia_hboxbuttons), yesnodia_yes, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(yesnodia_hboxbuttons), yesnodia_no, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(yesnodia_hboxbuttons), yesnodia_cancel, FALSE, FALSE, 0);

  GtkWidget *yesnodia_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(yesnodia_vbox), yesnodia_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(yesnodia_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(yesnodia_vbox), yesnodia_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(yesnodia_widget), yesnodia_vbox);
  gtk_widget_show_all( yesnodia_widget);
  g_object_set( yesnodia_widget, "visible", FALSE, NULL);

							  

  if ( graph_name)
    open_graph( graph_name);
  else 
    graph->set_default_background_color();

  // Get proview defined selection atoms
  CoWowGtk::GetAtoms( &graph_atom, 0, 0);

  ge_get_systemname( systemname);
  graph->set_systemname( systemname);

}

