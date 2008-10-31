/* 
 * Proview   $Id: xtt_ge.cpp,v 1.16 2008-10-31 12:51:36 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "glow.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "co_lng.h"
#include "xtt_ge.h"
#include "ge_graph.h"
#include "xtt_xnav.h"

void XttGe::graph_init_cb( void *client_data)
{
  XttGe *ge = (XttGe *) client_data;
  char fname[120];
  int		default_width;
  int		default_height;
  int		sts;

  cdh_ToLower( fname, ge->filename);
  if ( ! strrchr( fname, '.'))
    strcat( fname, ".pwg");
  ge->graph->open( fname);

  ge->graph->init_trace();

  if ( ge->width == 0 || ge->height == 0) {
    sts = ge->graph->get_default_size( &default_width, &default_height);
    if ( ODD(sts)) {
      ge->set_size( default_width, default_height);
    }
  }

  ge->graph->set_default_layout();

}

void XttGe::graph_close_cb( void *client_data)
{
  XttGe *ge = (XttGe *) client_data;
  delete ge;
}

int XttGe::ge_command_cb( void *ge_ctx, char *cmd)
{
  XttGe	*ge = (XttGe *)ge_ctx;
  int		sts;

  if ( ge->command_cb)
  {
    sts = (ge->command_cb)( ge, cmd);
    return sts;
  }
  return 0;
}

int XttGe::ge_sound_cb( void *ge_ctx, pwr_tAttrRef *aref)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->sound_cb)
    return (ge->sound_cb)( ge->parent_ctx, aref);

  return 0;
}

void XttGe::ge_display_in_xnav_cb( void *ge_ctx, pwr_sAttrRef *arp)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->display_in_xnav_cb)
    (ge->display_in_xnav_cb)( ge->parent_ctx, arp);
}

void XttGe::ge_popup_menu_cb( void *ge_ctx, pwr_sAttrRef attrref,
			   unsigned long item_type, unsigned long utility, 
			   char *arg, int x, int y)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->popup_menu_cb)
    (ge->popup_menu_cb)( ge->parent_ctx, attrref, item_type, utility, 
			    arg, x, y);
}

int XttGe::ge_call_method_cb( void *ge_ctx, char *method, char *filter,
			   pwr_sAttrRef attrref, unsigned long item_type, 
			   unsigned long utility, char *arg)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->call_method_cb)
    return (ge->call_method_cb)( ge->parent_ctx, method, filter, attrref, item_type, utility, 
			    arg);
  else return 0;
}

int XttGe::ge_is_authorized_cb( void *ge_ctx, unsigned int access)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->is_authorized_cb)
    return (ge->is_authorized_cb)( ge->parent_ctx, access);
  return 0;
}

int XttGe::ge_get_current_objects_cb( void *ge_ctx, pwr_sAttrRef **alist,
				   int **is_alist)
{
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->get_current_objects_cb)
    return (ge->get_current_objects_cb)( ge->parent_ctx, alist, is_alist);
  return 0;
}

void XttGe::message_cb( void *ctx, char severity, const char *msg)
{
  ((XttGe *)ctx)->message( severity, msg);
}

void XttGe::message( char severity, const char *msg)
{
  if ( strcmp( msg, "") != 0)
    printf("** XttGe: %s\n", msg);
}


int XttGe::set_object_focus( const char *name, int empty)
{
  return graph->set_object_focus( name, empty);
}

int XttGe::set_folder_index( const char *name, int idx)
{
  return graph->set_folder_index( name, idx);
}

int XttGe::set_subwindow_source( const char *name, char *source)
{
  return graph->set_subwindow_source( name, source);
}

XttGe::~XttGe()
{
}

void XttGe::print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/graph.ps");
  graph->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s 1", filename);
  system(cmd);
}

void XttGe::swap( int mode)
{
  graph->swap( mode);
}

XttGe::XttGe( void *xg_parent_ctx, const char *xg_name, const char *xg_filename,
	      int xg_scrollbar, int xg_menu, int xg_navigator, int xg_width, int xg_height,
	      int x, int y, double scan_time, const char *object_name,
	      int use_default_access, unsigned int access,
	      int (*xg_command_cb) (XttGe *, char *),
	      int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
	      int (*xg_is_authorized_cb) (void *, unsigned int)) :
  parent_ctx(xg_parent_ctx), scrollbar(xg_scrollbar),
  navigator(xg_navigator), menu(xg_menu), current_value_object(0),
  current_confirm_object(0), value_input_open(0), confirm_open(0), 
  command_cb(xg_command_cb), close_cb(0), help_cb(0), display_in_xnav_cb(0), 
  is_authorized_cb(xg_is_authorized_cb), popup_menu_cb(0), call_method_cb(0), 
  get_current_objects_cb(xg_get_current_objects_cb), sound_cb(0),
  width(xg_width), height(xg_height)
{
  strcpy( filename, xg_filename);
  strcpy( name, xg_name);
}

