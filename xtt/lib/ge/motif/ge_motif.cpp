/* 
 * Proview   $Id: ge_motif.cpp,v 1.6 2008-10-31 12:51:33 claes Exp $
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

/* ge.cpp -- Graphical editor window */


#include "glow_std.h"

#if defined OS_VMS || defined OS_LINUX
#define LDH 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <vector>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/ToggleB.h>

#include "rt_gdh.h"
#include "rt_load.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "flow_x.h"
#include "co_mrm_util.h"
#include "co_xhelp.h"
#include "co_wow_motif.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "glow.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_colpalwidget_motif.h"
#include "glow_msg.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"
#include "ge_graph_motif.h"
#include "ge_motif.h"
#include "ge_subpalette_motif.h"
#include "ge_subgraphs_motif.h"
#include "ge_util.h"
#include "ge_msg.h"
#include "wb_wnav_selformat.h"
#include "wb_nav_motif.h"

void GeMotif::create_list( const char *title, const char *texts,
			   void (action_cb)( void *, char *), void *ctx) 
{
  CoWowMotif wow( toplevel);
  wow.CreateList( title, texts, action_cb, 0, ctx);
}

void GeMotif::subgraphs_new()
{
  int sts;

  subgraphs = new SubGraphsMotif( this, toplevel, "SubGraphs",
		(void *) graph->grow->ctx,
		&subgraphs_widget, &sts);
  subgraphs->message_cb = &Ge::message_cb;
  subgraphs->close_cb = &Ge::subgraphs_close_cb;
}

void GeMotif::set_title( char *title)
{
  Arg 		args[2];

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( toplevel, args, 1);
}

void GeMotif::open_input_dialog( const char *text, const char *title,
			    const char *init_text,
			    void (*x_india_ok_cb)( Ge *, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( (char*) text, (char*) "ISO8859-1") ); i++;
  XtSetValues( india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( (char*) title, (char*) "ISO8859-1") ); i++;
  XtSetValues( india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( india_text, (char*) init_text);

  XtManageChild( india_widget);

  india_ok_cb = x_india_ok_cb;
}

void GeMotif::message( char severity, const char *message)
{
  Arg 		args[3];
  XmString	cstr;

  if ( strcmp( message, "") == 0) {
    XtUnmanageChild( msg_label);
  }
  else {
    XtManageChild( msg_label);
    cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
    XtSetArg(args[0],XmNlabelString, cstr);
    XtSetArg(args[1],XmNheight,30);
    XtSetValues( msg_label, args, 2);
    XmStringFree( cstr);
  }
}

void GeMotif::status_msg( char *pos_str)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( pos_str, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  // XtSetArg(args[1],XmNwidth, 280);
  XtSetValues( cursor_position, args, 1);
  XmStringFree( cstr);

  int width = 0;
  int height = 0;
  XtSetArg(args[0],XmNwidth, &width);
  XtSetArg(args[1],XmNheight, &height);
  XtGetValues( cursor_position, args, 2);
  if ( width != 320)
    XtResizeWidget( cursor_position, 320, height, 0);
}

void GeMotif::change_text_cb( void *ge_ctx, void *text_object, const char *text)
{
  GeMotif *gectx = (GeMotif *)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open)  {
    XtUnmanageChild( gectx->cmd_input);
    XtManageChild( gectx->msg_label);
    gectx->set_prompt( "");
    gectx->text_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtUnmanageChild( gectx->msg_label);
    XtManageChild( gectx->cmd_input);
  }
  gectx->message( ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);

  XmTextSetString( gectx->cmd_input, (char*) text);
  XmTextSetInsertionPosition( gectx->cmd_input, strlen(text));
  XmTextSetSelection( gectx->cmd_input, 0, strlen(text), CurrentTime);
  gectx->set_prompt( "Enter text >");
  gectx->text_input_open = 1;
  gectx->current_text_object = text_object;
}

void GeMotif::change_name_cb( void *ge_ctx, void *text_object, char *text)
{
  GeMotif *gectx = (GeMotif *)ge_ctx;

  if ( gectx->text_input_open || gectx->name_input_open || 
       gectx->value_input_open)  {
    XtUnmanageChild( gectx->cmd_input);
    gectx->set_prompt( "");
    gectx->name_input_open = 0;
    gectx->text_input_open = 0;
    gectx->value_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtManageChild( gectx->cmd_input);
    XtUnmanageChild( gectx->msg_label);
  }
  gectx->message( ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);
//  XtCallAcceptFocus( gectx->cmd_input, CurrentTime);

  XmTextSetString( gectx->cmd_input, text);
  XmTextSetInsertionPosition( gectx->cmd_input, strlen(text));
  XmTextSetSelection( gectx->cmd_input, 0, strlen(text), CurrentTime);
  gectx->set_prompt( "Enter name >");
  gectx->name_input_open = 1;
  gectx->current_text_object = text_object;
}

void GeMotif::change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  Arg  	args[1];
  GeMotif    *gectx = (GeMotif *)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open ||
       gectx->name_input_open)  {
    XtUnmanageChild( gectx->cmd_input);
    gectx->set_prompt( "");
    gectx->value_input_open = 0;
    gectx->text_input_open = 0;
    gectx->name_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtUnmanageChild( gectx->msg_label);
    XtManageChild( gectx->cmd_input);
  }
  gectx->message( ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);

  XtSetArg(args[0],XmNvalue, text);
  XtSetValues( gectx->cmd_input, args, 1);
  gectx->set_prompt( "Enter value >");
  gectx->value_input_open = 1;
  gectx->current_value_object = value_object;
}

int GeMotif::get_plant_select( char *select_name)
{
#if LDH
  pwr_sAttrRef	attrref;
  int		is_attrref;
  int		sts;
  pwr_tAName   	str;
  pwr_tAName 	buff;

  if ( !ldhses) {
    sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), XA_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }

  sts = plant_get_select( plantctx, &attrref, &is_attrref);
  if ( ODD( sts)) {
    if ( !wnav_format_selection( ldhses, attrref, 0, is_attrref, wnav_eSelectionMode_Normal,
				 0, 1, 1, buff))
      return 0;

    strcpy( select_name, buff);
    return 1;
  }
  else {
    sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), XA_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }
#else
  pwr_tStatus sts;
  GeMotif	*gectx = (GeMotif *)ge_ctx;

  sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), graph_atom);
  if ( ODD(sts))
    strcpy( select_name, str);
  else {
    sts = CoWowMotif::GetSelection( toplevel, str, sizeof(str), XA_STRING);
    if ( ODD(sts))
      strcpy( select_name, str);
  }
  return sts;
#endif
}

void GeMotif::open_yesnodia( const char *text, const char *title, 
	void (*yes_cb)( Ge *), void (*no_cb)( Ge *))
{
  Arg 		args[3];
  XmString	cstr;

  if ( yesnodia_open)  {
    XtUnmanageChild( yesnodia_widget);
    yesnodia_open = 0;
    return;
  }

  XtManageChild( yesnodia_widget);

  message( ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( (char*) text, (char*) "ISO8859-1"));
  XtSetArg(args[1], XmNdialogTitle,
		cstr=XmStringCreateLtoR( (char*) title, (char*) "ISO8859-1") );
  XtSetValues( yesnodia_widget, args, 2);
  XmStringFree( cstr);
  yesnodia_open = 1;
  yesnodia_yes_cb = yes_cb;
  yesnodia_no_cb = no_cb;
}

void GeMotif::confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  Arg 		args[1];
  GeMotif      	*gectx = (GeMotif *)ge_ctx;

  if ( gectx->confirm_open)  {
    XtUnmanageChild( gectx->confirm_widget);
    gectx->confirm_open = 0;
    return;
  }

  XtManageChild( gectx->confirm_widget);

  gectx->message( ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, (char*) "ISO8859-1"));
  XtSetValues( gectx->confirm_widget, args, 1);
  gectx->confirm_open = 1;
  gectx->current_confirm_object = confirm_object;
}

void GeMotif::set_prompt( const char *prompt)
{
  Arg 		args[1];
  XmString	cstr;

  if ( strcmp( prompt, "") == 0)
    XtUnmanageChild( cmd_prompt);
  else {
    XtManageChild( cmd_prompt);
    cstr=XmStringCreateLtoR( (char*) prompt, (char*) "ISO8859-1");
    XtSetArg(args[0],XmNlabelString, cstr);
    XtSetValues( cmd_prompt, args, 1);
    XmStringFree( cstr);
  }
}

//
//  Callbackfunctions from menu entries
//
void GeMotif::activate_change_text( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_change_text();
}

void GeMotif::activate_change_name( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_change_name();
}

void GeMotif::activate_preview_start( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_preview_start();
}

void GeMotif::activate_preview_stop( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_preview_stop();
}

void GeMotif::activate_cut( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_cut();
}

void GeMotif::activate_copy( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_copy();
}

void GeMotif::activate_rotate( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_rotate();
}

void GeMotif::activate_rotate90( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_rotate90();
}

void GeMotif::activate_flip_vert( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_flip_vert();
}

void GeMotif::activate_flip_horiz( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_flip_horiz();
}

void GeMotif::activate_pop( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_pop();
}

void GeMotif::activate_push( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_push();
}

void GeMotif::activate_edit_polyline( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_edit_polyline();
}

void GeMotif::activate_scale_equal( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_scale_equal();
}

void GeMotif::activate_move_horizontal( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_move_horizontal();
}

void GeMotif::activate_move_vertical( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_move_vertical();
}

void GeMotif::activate_move_reset( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_move_reset();
}

void GeMotif::activate_align_horiz_up( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_horiz_up();
}

void GeMotif::activate_align_horiz_down( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_horiz_down();
}

void GeMotif::activate_align_horiz_center( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_horiz_center();
}

void GeMotif::activate_align_vert_left( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_vert_left();
}

void GeMotif::activate_align_vert_right( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_vert_right();
}

void GeMotif::activate_align_vert_center( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_align_vert_center();
}

void GeMotif::activate_equid_vert_up( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_vert_up();
}

void GeMotif::activate_equid_vert_down( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_vert_down();
}

void GeMotif::activate_equid_vert_center( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_vert_center();
}

void GeMotif::activate_equid_horiz_left( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_horiz_left();
}

void GeMotif::activate_equid_horiz_right( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_horiz_right();
}

void GeMotif::activate_equid_horiz_center( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_equid_horiz_center();
}

void GeMotif::activate_select_cons( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_select_cons();
}

void GeMotif::activate_select_objects( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_select_objects();
}

void GeMotif::activate_group( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_group();
}

void GeMotif::activate_ungroup( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_ungroup();
}

void GeMotif::activate_connect( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_connect();
}

void GeMotif::activate_connectsecond( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_connectsecond();
}

void GeMotif::activate_objectattributes( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_objectattributes();
}

void GeMotif::activate_show_grid( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_show_grid( data->set);
}

void GeMotif::activate_paste( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_paste();
}

void GeMotif::activate_command( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  Arg 		args[1];

  if ( gectx->command_open) {
    XtUnmanageChild( ((GeMotif *)gectx)->cmd_input);
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
    XtUnmanageChild( ((GeMotif *)gectx)->msg_label);
    XtManageChild( ((GeMotif *)gectx)->cmd_input);
  }
  gectx->message( ' ', "");
  // XmProcessTraversal( gectx->cmd_input, XmTRAVERSE_CURRENT);
  // XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);
  XtCallAcceptFocus( ((GeMotif *)gectx)->cmd_input, CurrentTime);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( ((GeMotif *)gectx)->cmd_input, args, 1);
  gectx->set_prompt( "ge >        ");
  gectx->command_open = 1;
}

void GeMotif::activate_exit( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_exit();
}

void GeMotif::activate_print( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_print();
}

void GeMotif::activate_new( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_new();
}

void GeMotif::activate_save( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_save();
}

void GeMotif::activate_save_as( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_save_as();
}

void GeMotif::activate_export_javabean( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_javabean();
}

void GeMotif::activate_export_javabean_as( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_javabean_as();
}

void GeMotif::activate_export_gejava( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_gejava();
}

void GeMotif::activate_export_gejava_as( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_gejava_as();
}

void GeMotif::activate_export_java( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_java();
}

void GeMotif::activate_export_java_as( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_export_java_as();
}

void GeMotif::activate_generate_web( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_generate_web();
}

void GeMotif::activate_creanextpage( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_creanextpage();
}

void GeMotif::activate_nextpage( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_nextpage();
}

void GeMotif::activate_prevpage( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_prevpage();
}

void GeMotif::activate_graph_attr( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_graph_attr();
}

void GeMotif::activate_open( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_open();
}

void GeMotif::activate_subgraphs( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_subgraphs();
}

void GeMotif::activate_rect( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_rect( keep);
}

void GeMotif::activate_rectrounded( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_rectrounded( keep);
}

void GeMotif::activate_line( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_line( keep);
}

void GeMotif::activate_polyline( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_polyline( keep);
}

void GeMotif::activate_circle( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_circle( keep);
}

void GeMotif::activate_text( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_text( keep);
}

void GeMotif::activate_annot( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_annot( keep);
}

void GeMotif::activate_conpoint( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->activate_conpoint( keep);
}

void GeMotif::activate_fill( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_fill( data->set);
}

void GeMotif::activate_border( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_border( data->set);
}

void GeMotif::activate_shadow( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_shadow( data->set);
}

void GeMotif::activate_incr_lightness( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_incr_lightness();
}

void GeMotif::activate_decr_lightness( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_decr_lightness();
}

void GeMotif::activate_incr_intensity( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_incr_intensity();
}

void GeMotif::activate_decr_intensity( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_decr_intensity();
}

void GeMotif::activate_incr_shift( Widget w, Ge *gectx, XmAnyCallbackStruct *data){

  gectx->activate_incr_shift();
}

void GeMotif::activate_decr_shift( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_decr_shift();
}

void GeMotif::activate_scale( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_scale();
}

void GeMotif::activate_grid( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_grid( data->set);
}

void GeMotif::activate_linewidth_1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 1);
}
void GeMotif::activate_linewidth_2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 2);
}
void GeMotif::activate_linewidth_3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 3);
}
void GeMotif::activate_linewidth_4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 4);
}
void GeMotif::activate_linewidth_5( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 5);
}
void GeMotif::activate_linewidth_6( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 6);
}
void GeMotif::activate_linewidth_7( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 7);
}
void GeMotif::activate_linewidth_8( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linewidth( 8);
}

void GeMotif::activate_linetype1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype1();
}

void GeMotif::activate_linetype2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype2();
}

void GeMotif::activate_linetype3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype3();
}

void GeMotif::activate_linetype4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype4();
}

void GeMotif::activate_linetype5( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype5();
}

void GeMotif::activate_linetype6( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype6();
}

void GeMotif::activate_linetype7( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_linetype7();
}

void GeMotif::activate_gridsize_4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_gridsize( 1);
}

void GeMotif::activate_gridsize_3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_gridsize( 0.5);
}

void GeMotif::activate_gridsize_2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_gridsize( 0.2);
}

void GeMotif::activate_gridsize_1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_gridsize( 0.1);
}

void GeMotif::activate_textsize_0( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 0);
}
void GeMotif::activate_textsize_1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 1);
}

void GeMotif::activate_textsize_2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 2);
}

void GeMotif::activate_textsize_3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 3);
}

void GeMotif::activate_textsize_4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 4);
}

void GeMotif::activate_textsize_5( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textsize( 5);
}

void GeMotif::activate_textfont_1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textfont( glow_eFont_Helvetica);
}
void GeMotif::activate_textfont_2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textfont( glow_eFont_Times);
}
void GeMotif::activate_textfont_3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textfont( glow_eFont_NewCenturySchoolbook);
}
void GeMotif::activate_textfont_4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_textfont( glow_eFont_Courier);
}

void GeMotif::activate_textbold( Widget w, Ge *gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->activate_textbold( data->set);
}

void GeMotif::activate_zoom_in( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_zoom_in();
}

void GeMotif::activate_zoom_out( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_zoom_out();
}

void GeMotif::activate_zoom_reset( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_zoom_reset();
}

void GeMotif::activate_concorner_right( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_concorner_right();
}

void GeMotif::activate_concorner_rounded( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_concorner_rounded();
}

void GeMotif::activate_round_amount_1( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_round_amount( 0.2);
}

void GeMotif::activate_round_amount_2( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_round_amount( 0.5);
}

void GeMotif::activate_round_amount_3( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_round_amount( 1.0);
}

void GeMotif::activate_round_amount_4( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_round_amount( 2.0);
}

void GeMotif::activate_round_amount_5( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_round_amount( 4.0);
}

void GeMotif::activate_contype_straight( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_straight();
}

void GeMotif::activate_contype_routed( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_routed();
}

void GeMotif::activate_contype_stronearr( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_stronearr();
}

void GeMotif::activate_contype_stepdiv( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_stepdiv();
}

void GeMotif::activate_contype_stepconv( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_stepconv();
}

void GeMotif::activate_contype_transdiv( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_transdiv();
}

void GeMotif::activate_contype_transconv( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_contype_transconv();
}

void GeMotif::activate_condir_center( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_condir_center();
}

void GeMotif::activate_condir_left( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_condir_left();
}

void GeMotif::activate_condir_right( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_condir_right();
}

void GeMotif::activate_condir_up( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_condir_up();
}

void GeMotif::activate_condir_down( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_condir_down();
}

void GeMotif::activate_background_color( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_background_color();
}

void GeMotif::activate_help( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_help();
}

void GeMotif::create_cursor_position( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->cursor_position = w;
}
void GeMotif::create_msg_label( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->msg_label = w;
}
void GeMotif::create_cmd_prompt( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->cmd_prompt = w;
}
void GeMotif::create_cmd_input( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, gectx);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) GeMotif::valchanged_cmd_input, mrm_eUtility_Ge);

  ((GeMotif *)gectx)->cmd_input = w;
}
void GeMotif::create_graph_form( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->graph_form = w;
}
void GeMotif::create_main_pane( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->main_pane = w;
}
void GeMotif::create_palette_pane( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->palette_pane = w;
}
void GeMotif::create_widget_cb( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch ( key) {
    case 1:
      ((GeMotif *)gectx)->grid_on_w = w;
      break;
    case 2:
      ((GeMotif *)gectx)->grid_size_w = w;
      break;
    case 3:
      ((GeMotif *)gectx)->grid_size_10_w = w;
      break;
    case 4:
      ((GeMotif *)gectx)->grid_size_05_w = w;
      break;
    case 5:
      ((GeMotif *)gectx)->grid_size_02_w = w;
      break;
    case 6:
      ((GeMotif *)gectx)->grid_size_01_w = w;
      break;
    case 7:
      ((GeMotif *)gectx)->show_grid_w = w;
      break;
  }
}
void GeMotif::create_india_label( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->india_label = w;
}
void GeMotif::create_india_text( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  ((GeMotif *)gectx)->india_text = w;
}
void GeMotif::activate_india_ok( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( ((GeMotif *)gectx)->india_text);
  XtUnmanageChild( ((GeMotif *)gectx)->india_widget);

  gectx->activate_india_ok( value);
}
void GeMotif::activate_india_cancel( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((GeMotif *)gectx)->india_widget);
}
void GeMotif::activate_yesnodia_yes( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((GeMotif *)gectx)->yesnodia_widget);
  gectx->activate_yesnodia_yes();
}
void GeMotif::activate_yesnodia_no( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((GeMotif *)gectx)->yesnodia_widget);
  gectx->activate_yesnodia_no();
}
void GeMotif::activate_yesnodia_cancel( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((GeMotif *)gectx)->yesnodia_widget);
  gectx->activate_yesnodia_cancel();
}
void GeMotif::activate_confirm_ok( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((GeMotif *)gectx)->confirm_widget);
  gectx->activate_confirm_ok();
}
void GeMotif::activate_confirm_cancel( Widget w, Ge *gectx, XmAnyCallbackStruct *data)
{
  gectx->activate_confirm_cancel();;
  XtUnmanageChild( ((GeMotif *)gectx)->confirm_widget);
}
void GeMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Ge *gectx;

  XtSetArg    (args[0], XmNuserData, &gectx);
  XtGetValues (w, args, 1);

  if ( gectx && gectx->graph)
    gectx->graph->set_inputfocus(1);
}

void GeMotif::valchanged_cmd_input( Widget w, XEvent *event)
{
  Ge 	*gectx;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &gectx);
  XtGetValues(w, args, 1);

  if ( gectx->value_input_open)
    sts = mrm_TextInput( w, event, (char *)((GeMotif *)gectx)->value_recall,
	sizeof(((GeMotif *)gectx)->value_recall[0]),
	sizeof( ((GeMotif *)gectx)->value_recall)/sizeof(((GeMotif *)gectx)->value_recall[0]),
	&((GeMotif *)gectx)->value_current_recall);
  else if ( gectx->name_input_open)
    sts = mrm_TextInput( w, event, (char *)((GeMotif *)gectx)->name_recall,
	sizeof(((GeMotif *)gectx)->name_recall[0]),
	sizeof( ((GeMotif *)gectx)->name_recall)/sizeof(((GeMotif *)gectx)->name_recall[0]),
	&((GeMotif *)gectx)->name_current_recall);
  else if ( gectx->text_input_open)
    sts = mrm_TextInput( w, event, (char *)((GeMotif *)gectx)->text_recall, sizeof(((GeMotif *)gectx)->text_recall[0]),
	sizeof( ((GeMotif *)gectx)->text_recall)/sizeof(((GeMotif *)gectx)->text_recall[0]),
	&((GeMotif *)gectx)->text_current_recall);
  else
    sts = mrm_TextInput( w, event, (char *)((GeMotif *)gectx)->cmd_recall, sizeof(((GeMotif *)gectx)->cmd_recall[0]),
	sizeof( ((GeMotif *)gectx)->cmd_recall)/sizeof(((GeMotif *)gectx)->cmd_recall[0]),
	&((GeMotif *)gectx)->cmd_current_recall);
  if ( sts) {
    text = XmTextGetString( w);
    if ( gectx->text_input_open) {
      gectx->graph->change_text( gectx->current_text_object, text);
      XtUnmanageChild( w);
      gectx->set_prompt( "");
      gectx->text_input_open = 0;
    }
    else if ( gectx->name_input_open) {
      gectx->graph->change_name( gectx->current_text_object, text);
      XtUnmanageChild( w);
      gectx->set_prompt( "");
      gectx->name_input_open = 0;
    }
    else if ( gectx->value_input_open) {
      gectx->graph->change_value( gectx->current_value_object, text);
      XtUnmanageChild( w);
      gectx->set_prompt( "");
      gectx->value_input_open = 0;
    }
    else if ( gectx->command_open) {
      sts = gectx->graph->command( text);
      XtUnmanageChild( w);
      gectx->set_prompt( "");
      gectx->command_open = 0;
    }
  }
}

void GeMotif::update()
{
  Arg 	args[2];

  XmToggleButtonSetState(grid_on_w, graph->grid, 0);
  XmToggleButtonSetState(show_grid_w, graph->get_show_grid(), 0);

  if ( fabs( graph->grid_size_x - 0.1) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, grid_size_01_w);
  else if ( fabs( graph->grid_size_x - 0.2) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, grid_size_02_w);
  else if ( fabs( graph->grid_size_x - 0.5) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, grid_size_05_w);
  else
    XtSetArg(args[0], XmNmenuHistory, grid_size_10_w);
  XtSetValues( grid_size_w, args, 1);
}

GeMotif::~GeMotif()
{
  if ( subgraphs)
    delete subgraphs;
  delete graph;
  delete wow;
  XtDestroyWidget( toplevel);

  if ( exit_when_close)
    exit(0);
}

GeMotif::GeMotif( 	void 	*x_parent_ctx, 
			Widget 	x_parent_widget,
			ldh_tSesContext	x_ldhses,
			int	x_exit_when_close,
			char	*graph_name) :
  Ge( x_parent_ctx, x_ldhses, x_exit_when_close),
  parent_wid(x_parent_widget), graph_atom(0), text_current_recall(0),
  name_current_recall(0), value_current_recall(0), cmd_current_recall(0)
{
  char		uid_filename[120] = {"pwr_exe:ge.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	ge_widget;
  int		path_cnt;
  char		*path;
  char		*s;
  char		tmp_name[80];
  char          systemname[80];

  static char translations[] =
    "<FocusIn>: ge_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "ge_inputfocus",      (XtActionProc) GeMotif::action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "ge_ctx", 0 },
	{(char*) "ge_activate_exit",(caddr_t)GeMotif::activate_exit },
	{(char*) "ge_activate_print",(caddr_t)GeMotif::activate_print },
	{(char*) "ge_activate_new",(caddr_t)GeMotif::activate_new },
	{(char*) "ge_activate_save",(caddr_t)GeMotif::activate_save },
	{(char*) "ge_activate_save_as",(caddr_t)GeMotif::activate_save_as },
	{(char*) "ge_activate_graph_attr",(caddr_t)GeMotif::activate_graph_attr },
	{(char*) "ge_activate_open",(caddr_t)GeMotif::activate_open },
	{(char*) "ge_activate_export_java",(caddr_t)GeMotif::activate_export_java },
	{(char*) "ge_activate_export_java_as",(caddr_t)GeMotif::activate_export_java_as },
	{(char*) "ge_activate_generate_web",(caddr_t)GeMotif::activate_generate_web },
	{(char*) "ge_activate_creanextpage",(caddr_t)GeMotif::activate_creanextpage },
	{(char*) "ge_activate_nextpage",(caddr_t)GeMotif::activate_nextpage },
	{(char*) "ge_activate_prevpage",(caddr_t)GeMotif::activate_prevpage },
	{(char*) "ge_activate_preview_start",(caddr_t)GeMotif::activate_preview_start },
	{(char*) "ge_activate_preview_stop",(caddr_t)GeMotif::activate_preview_stop },
	{(char*) "ge_activate_subgraphs",(caddr_t)GeMotif::activate_subgraphs },
	{(char*) "ge_activate_change_text",(caddr_t)GeMotif::activate_change_text },
	{(char*) "ge_activate_change_name",(caddr_t)GeMotif::activate_change_name },
	{(char*) "ge_activate_pop",(caddr_t)GeMotif::activate_pop },
	{(char*) "ge_activate_push",(caddr_t)GeMotif::activate_push },
	{(char*) "ge_activate_edit_polyline",(caddr_t)GeMotif::activate_edit_polyline },
	{(char*) "ge_activate_scale_equal",(caddr_t)GeMotif::activate_scale_equal },
	{(char*) "ge_activate_move_horizontal",(caddr_t)GeMotif::activate_move_horizontal },
	{(char*) "ge_activate_move_vertical",(caddr_t)GeMotif::activate_move_vertical },
	{(char*) "ge_activate_move_reset",(caddr_t)GeMotif::activate_move_reset },
	{(char*) "ge_activate_align_vert_left",(caddr_t)GeMotif::activate_align_vert_left },
	{(char*) "ge_activate_align_vert_right",(caddr_t)GeMotif::activate_align_vert_right },
	{(char*) "ge_activate_align_vert_center",(caddr_t)GeMotif::activate_align_vert_center },
	{(char*) "ge_activate_align_horiz_up",(caddr_t)GeMotif::activate_align_horiz_up },
	{(char*) "ge_activate_align_horiz_down",(caddr_t)GeMotif::activate_align_horiz_down },
	{(char*) "ge_activate_align_horiz_center",(caddr_t)GeMotif::activate_align_horiz_center },
	{(char*) "ge_activate_equid_horiz_left",(caddr_t)GeMotif::activate_equid_horiz_left },
	{(char*) "ge_activate_equid_horiz_right",(caddr_t)GeMotif::activate_equid_horiz_right },
	{(char*) "ge_activate_equid_horiz_center",(caddr_t)GeMotif::activate_equid_horiz_center },
	{(char*) "ge_activate_equid_vert_up",(caddr_t)GeMotif::activate_equid_vert_up },
	{(char*) "ge_activate_equid_vert_down",(caddr_t)GeMotif::activate_equid_vert_down },
	{(char*) "ge_activate_equid_vert_center",(caddr_t)GeMotif::activate_equid_vert_center },
	{(char*) "ge_activate_select_cons",(caddr_t)GeMotif::activate_select_cons },
	{(char*) "ge_activate_select_objects",(caddr_t)GeMotif::activate_select_objects },
	{(char*) "ge_activate_group",(caddr_t)GeMotif::activate_group },
	{(char*) "ge_activate_ungroup",(caddr_t)GeMotif::activate_ungroup },
	{(char*) "ge_activate_connect",(caddr_t)GeMotif::activate_connect },
	{(char*) "ge_activate_connectsecond",(caddr_t)GeMotif::activate_connectsecond },
	{(char*) "ge_activate_objectattributes",(caddr_t)GeMotif::activate_objectattributes },
	{(char*) "ge_activate_show_grid",(caddr_t)GeMotif::activate_show_grid },
	{(char*) "ge_activate_cut",(caddr_t)GeMotif::activate_cut },
	{(char*) "ge_activate_copy",(caddr_t)GeMotif::activate_copy },
	{(char*) "ge_activate_paste",(caddr_t)GeMotif::activate_paste },
	{(char*) "ge_activate_rotate",(caddr_t)GeMotif::activate_rotate },
	{(char*) "ge_activate_rotate90",(caddr_t)GeMotif::activate_rotate90 },
	{(char*) "ge_activate_flip_horiz",(caddr_t)GeMotif::activate_flip_horiz },
	{(char*) "ge_activate_flip_vert",(caddr_t)GeMotif::activate_flip_vert },
	{(char*) "ge_activate_command",(caddr_t)GeMotif::activate_command },
	{(char*) "ge_activate_rect",(caddr_t)GeMotif::activate_rect },
	{(char*) "ge_activate_rectrounded",(caddr_t)GeMotif::activate_rectrounded },
	{(char*) "ge_activate_line",(caddr_t)GeMotif::activate_line },
	{(char*) "ge_activate_polyline",(caddr_t)GeMotif::activate_polyline },
	{(char*) "ge_activate_circle",(caddr_t)GeMotif::activate_circle },
	{(char*) "ge_activate_text",(caddr_t)GeMotif::activate_text },
	{(char*) "ge_activate_annot",(caddr_t)GeMotif::activate_annot },
	{(char*) "ge_activate_conpoint",(caddr_t)GeMotif::activate_conpoint },
	{(char*) "ge_activate_fill",(caddr_t)GeMotif::activate_fill },
	{(char*) "ge_activate_border",(caddr_t)GeMotif::activate_border },
	{(char*) "ge_activate_shadow",(caddr_t)GeMotif::activate_shadow },
	{(char*) "ge_activate_incr_lightness",(caddr_t)GeMotif::activate_incr_lightness },
	{(char*) "ge_activate_decr_lightness",(caddr_t)GeMotif::activate_decr_lightness },
	{(char*) "ge_activate_incr_intensity",(caddr_t)GeMotif::activate_incr_intensity },
	{(char*) "ge_activate_decr_intensity",(caddr_t)GeMotif::activate_decr_intensity },
	{(char*) "ge_activate_incr_shift",(caddr_t)GeMotif::activate_incr_shift },
	{(char*) "ge_activate_decr_shift",(caddr_t)GeMotif::activate_decr_shift },
	{(char*) "ge_activate_scale",(caddr_t)GeMotif::activate_scale },
	{(char*) "ge_activate_grid",(caddr_t)GeMotif::activate_grid },
	{(char*) "ge_activate_linewidth_1",(caddr_t)GeMotif::activate_linewidth_1 },
	{(char*) "ge_activate_linewidth_2",(caddr_t)GeMotif::activate_linewidth_2 },
	{(char*) "ge_activate_linewidth_3",(caddr_t)GeMotif::activate_linewidth_3 },
	{(char*) "ge_activate_linewidth_4",(caddr_t)GeMotif::activate_linewidth_4 },
	{(char*) "ge_activate_linewidth_5",(caddr_t)GeMotif::activate_linewidth_5 },
	{(char*) "ge_activate_linewidth_6",(caddr_t)GeMotif::activate_linewidth_6 },
	{(char*) "ge_activate_linewidth_7",(caddr_t)GeMotif::activate_linewidth_7 },
	{(char*) "ge_activate_linewidth_8",(caddr_t)GeMotif::activate_linewidth_8 },
	{(char*) "ge_activate_linetype1",(caddr_t)GeMotif::activate_linetype1 },
	{(char*) "ge_activate_linetype2",(caddr_t)GeMotif::activate_linetype2 },
	{(char*) "ge_activate_linetype3",(caddr_t)GeMotif::activate_linetype3 },
	{(char*) "ge_activate_linetype4",(caddr_t)GeMotif::activate_linetype4 },
	{(char*) "ge_activate_linetype5",(caddr_t)GeMotif::activate_linetype5 },
	{(char*) "ge_activate_linetype6",(caddr_t)GeMotif::activate_linetype6 },
	{(char*) "ge_activate_linetype7",(caddr_t)GeMotif::activate_linetype7 },
	{(char*) "ge_activate_textsize_0",(caddr_t)GeMotif::activate_textsize_0 },
	{(char*) "ge_activate_textsize_1",(caddr_t)GeMotif::activate_textsize_1 },
	{(char*) "ge_activate_textsize_2",(caddr_t)GeMotif::activate_textsize_2 },
	{(char*) "ge_activate_textsize_3",(caddr_t)GeMotif::activate_textsize_3 },
	{(char*) "ge_activate_textsize_4",(caddr_t)GeMotif::activate_textsize_4 },
	{(char*) "ge_activate_textsize_5",(caddr_t)GeMotif::activate_textsize_5 },
	{(char*) "ge_activate_textfont_1",(caddr_t)GeMotif::activate_textfont_1 },
	{(char*) "ge_activate_textfont_2",(caddr_t)GeMotif::activate_textfont_2 },
	{(char*) "ge_activate_textfont_3",(caddr_t)GeMotif::activate_textfont_3 },
	{(char*) "ge_activate_textfont_4",(caddr_t)GeMotif::activate_textfont_4 },
	{(char*) "ge_activate_textbold",(caddr_t)GeMotif::activate_textbold },
	{(char*) "ge_activate_gridsize_1",(caddr_t)GeMotif::activate_gridsize_1 },
	{(char*) "ge_activate_gridsize_2",(caddr_t)GeMotif::activate_gridsize_2 },
	{(char*) "ge_activate_gridsize_3",(caddr_t)GeMotif::activate_gridsize_3 },
	{(char*) "ge_activate_gridsize_4",(caddr_t)GeMotif::activate_gridsize_4 },
	{(char*) "ge_activate_zoom_in",(caddr_t)GeMotif::activate_zoom_in },
	{(char*) "ge_activate_zoom_out",(caddr_t)GeMotif::activate_zoom_out },
	{(char*) "ge_activate_zoom_reset",(caddr_t)GeMotif::activate_zoom_reset },
	{(char*) "ge_activate_help",(caddr_t)GeMotif::activate_help },
	{(char*) "ge_create_widget_cb",(caddr_t)GeMotif::create_widget_cb },
	{(char*) "ge_create_cursor_position",(caddr_t)GeMotif::create_cursor_position },
	{(char*) "ge_create_msg_label",(caddr_t)GeMotif::create_msg_label },
	{(char*) "ge_create_cmd_prompt",(caddr_t)GeMotif::create_cmd_prompt },
	{(char*) "ge_create_cmd_input",(caddr_t)GeMotif::create_cmd_input },
	{(char*) "ge_create_graph_form",(caddr_t)GeMotif::create_graph_form },
	{(char*) "ge_create_main_pane",(caddr_t)GeMotif::create_main_pane },
	{(char*) "ge_create_palette_pane",(caddr_t)GeMotif::create_palette_pane },
	{(char*) "ge_activate_concorner_right",(caddr_t)GeMotif::activate_concorner_right },
	{(char*) "ge_activate_concorner_rounded",(caddr_t)GeMotif::activate_concorner_rounded },
	{(char*) "ge_activate_round_amount_1",(caddr_t)GeMotif::activate_round_amount_1 },
	{(char*) "ge_activate_round_amount_2",(caddr_t)GeMotif::activate_round_amount_2 },
	{(char*) "ge_activate_round_amount_3",(caddr_t)GeMotif::activate_round_amount_3 },
	{(char*) "ge_activate_round_amount_4",(caddr_t)GeMotif::activate_round_amount_4 },
	{(char*) "ge_activate_round_amount_5",(caddr_t)GeMotif::activate_round_amount_5 },
	{(char*) "ge_activate_contype_straight",(caddr_t)GeMotif::activate_contype_straight },
	{(char*) "ge_activate_contype_routed",(caddr_t)GeMotif::activate_contype_routed },
	{(char*) "ge_activate_contype_stronearr",(caddr_t)GeMotif::activate_contype_stronearr },
	{(char*) "ge_activate_contype_stepdiv",(caddr_t)GeMotif::activate_contype_stepdiv },
	{(char*) "ge_activate_contype_stepconv",(caddr_t)GeMotif::activate_contype_stepconv },
	{(char*) "ge_activate_contype_transdiv",(caddr_t)GeMotif::activate_contype_transdiv },
	{(char*) "ge_activate_contype_transconv",(caddr_t)GeMotif::activate_contype_transconv },
	{(char*) "ge_activate_condir_center",(caddr_t)GeMotif::activate_condir_center },
	{(char*) "ge_activate_condir_left",(caddr_t)GeMotif::activate_condir_left },
	{(char*) "ge_activate_condir_right",(caddr_t)GeMotif::activate_condir_right },
	{(char*) "ge_activate_condir_up",(caddr_t)GeMotif::activate_condir_up },
	{(char*) "ge_activate_condir_down",(caddr_t)GeMotif::activate_condir_down },
	{(char*) "ge_activate_background_color",(caddr_t)GeMotif::activate_background_color },
	{(char*) "ge_activate_india_ok",(caddr_t)GeMotif::activate_india_ok },
	{(char*) "ge_activate_india_cancel",(caddr_t)GeMotif::activate_india_cancel },
	{(char*) "ge_create_india_label",(caddr_t)GeMotif::create_india_label },
	{(char*) "ge_create_india_text",(caddr_t)GeMotif::create_india_text },
	{(char*) "ge_activate_yesnodia_yes",(caddr_t)GeMotif::activate_yesnodia_yes },
	{(char*) "ge_activate_yesnodia_no",(caddr_t)GeMotif::activate_yesnodia_no },
	{(char*) "ge_activate_yesnodia_cancel",(caddr_t)GeMotif::activate_yesnodia_cancel },
	{(char*) "ge_activate_confirm_ok",(caddr_t)GeMotif::activate_confirm_ok },
	{(char*) "ge_activate_confirm_cancel",(caddr_t)GeMotif::activate_confirm_cancel }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  strcpy( name, "PwR Ge");

  dcli_translate_filename( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Ge");

  if ( graph_name) {
    cdh_ToLower( tmp_name, graph_name);
    tmp_name[0] = toupper( tmp_name[0]);
    if ( (s = strrchr( tmp_name, '.')))
      *s = 0;
    strcat( title, " ");
    strcat( title, tmp_name);
  }

  reglist[0].value = (caddr_t) this;

  toplevel = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext( toplevel), 
						actions, XtNumber(actions));
 
  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "ge_window", toplevel,
			name, NULL, 0, &ge_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, (char*) "input_dialog", toplevel,
		&india_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, (char*) "confirm_dialog", toplevel,
		&confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, (char*) "yesno_dialog", toplevel,
		&yesnodia_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch yesno dialog\n");

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( ge_widget, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( toplevel ,args,i);
      
  XtManageChild( ge_widget);
  XtUnmanageChild( cmd_input);

  graph = new GraphMotif( this, graph_form, "",
		&grow_widget, &sts, "pwrp_pop:");
  graph->message_cb = &Ge::message_cb;
  graph->get_current_subgraph_cb = &Ge::subpalette_get_select;
  graph->get_current_colors_cb = &Ge::colorpalette_get_current;
  graph->set_current_colors_cb = &Ge::colorpalette_set_current;
  graph->cursor_motion_cb = &Ge::status_msg;
  graph->change_text_cb = &GeMotif::change_text_cb;
  graph->change_name_cb = &GeMotif::change_name_cb;
  graph->change_value_cb = &GeMotif::change_value_cb;
  graph->confirm_cb = &GeMotif::confirm_cb;
  graph->command_cb = &Ge::command_cb;
  graph->get_plant_select_cb = &Ge::get_plant_select_cb;
  graph->load_graph_cb = &Ge::load_graph_cb;
  graph->set_focus_cb = &Ge::set_focus_cb;
  graph->traverse_focus_cb = &Ge::traverse_focus;
  graph->get_ldhses_cb = &Ge::get_ldhses_cb;

//  i = 0;
//  XtSetArg(args[i], XmNtraversalOn, True);i++;
//  XtSetArg(args[i], XmNnavigationType, XmTAB_GROUP);i++;
//  XtSetValues( grow_widget, args,i);

  subpalette = new SubPaletteMotif( this, palette_pane, "SubGraphs",
		&subpalette_widget, &sts);
  subpalette->message_cb = &Ge::message_cb;
  subpalette->set_focus_cb = &Ge::set_focus_cb;
  subpalette->traverse_focus_cb = &Ge::traverse_focus;
  subpalette_mapped = 1;

  colpal_main_widget = ScrolledColPalCreate( palette_pane, 
		(char*) "Colors", NULL, 
		0, Ge::init_colorpalette_cb, this,
		&colorpalette_widget);
  XtManageChild( colpal_main_widget);

#ifdef LDH
  if ( ldhses) {
    plantctx = new NavMotif( this, palette_pane, "Plant",
			     ldhses, "NavigatorW1", 
			     &plant_widget, &sts);
    ((NavMotif *)plantctx)->get_plant_select_cb = Ge::get_plant_select_cb;
    plant_mapped = 1;
//  XtUnmanageChild( plant_widget);
  }
#endif

  ((GraphMotif *)graph)->create_navigator( palette_pane);

  XtPopup( toplevel, XtGrabNone);
  XtRealizeWidget( toplevel);

//  XmProcessTraversal( graph->grow_widget, XmTRAVERSE_CURRENT);
//  graph->set_inputfocus();

  // Set traversal off on sash widget
  {
    Widget *children;
    int num_children;
    XtVaGetValues( main_pane, 
		 XmNchildren, &children,
		 XmNnumChildren, &num_children, NULL);
    while ( num_children-- > 0)
      if ( XmIsSash( children[num_children]))
        XtVaSetValues( children[num_children],
		     XmNtraversalOn, False, NULL);
  }
  subpalette->get_path( &path_cnt, &path);
  graph->set_subgraph_path( path_cnt, path);

  if ( graph_name)
    open_graph( graph_name);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)GeMotif::activate_exit, this);

  // Get proview defined selection atoms
  CoWowMotif::GetAtoms( toplevel, &graph_atom, 0, 0);

  ge_get_systemname( systemname);
  graph->set_systemname( systemname); 
  wow = new CoWowMotif( toplevel);
}

void GeMotif::plant_del( void *plantctx)
{
  delete (NavMotif *) plantctx;
}

int GeMotif::plant_get_select( void *plantctx, pwr_sAttrRef *attrref, int *is_attr)
{
  return ((Nav *)plantctx)->get_select( attrref, is_attr);
}
