/* 
 * Proview   $Id$
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

/* cow_pb_gsd_attr.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "cow_wow.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "flow_msg.h"
#include "rt_pb_msg.h"
#include "cow_pb_gsd_attr.h"
#include "cow_pb_gsd_attrnav.h"


// Static member variables
char	GsdAttr::value_recall[30][160];

void GsdAttr::gsdattr_message( void *attr, char severity, const char *message)
{
  ((GsdAttr *)attr)->message( severity, message);
}

void GsdAttr::gsdattr_change_value_cb( void *attr_ctx)
{
  GsdAttr *attr = (GsdAttr *) attr_ctx;
  attr->change_value();
}

//
//  Callbackfunctions from menu entries
//
void GsdAttr::activate_exit()
{
  if ( close_cb) {
    if ( gsd->is_modified()) {
      wow->DisplayQuestion( (void *)this, "Apply",
			    "Do you want to apply changes",
			    cmd_close_apply_cb, cmd_close_no_cb, 0);

    }
    else
      (close_cb)( parent_ctx);
  }
  else
    delete this;
}

void GsdAttr::activate_help()
{
  int sts;

  if ( help_cb)
    sts = (help_cb)( parent_ctx, "pb_slave_editor /helpfile=\"$pwr_exe/profibus_xtthelp.dat\"");
  
}

void GsdAttr::activate_copy()
{
  ItemPbModule *item;
  int sts;

  sts = attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    message('E', "Only modules can be copied");
    return;
  }
  
  gsd->copy_module_conf( item->mconf);
  message('I', "Module copied");
}

void GsdAttr::activate_cut()
{
  ItemPbModule *item;
  int sts;

  sts = attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    message('E', "Only modules can be copied");
    return;
  }
  
  gsd->cut_module_conf( item->mconf);
}

void GsdAttr::activate_paste()
{
  ItemPbModule *item;
  int sts;

  sts = attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    message('E', "Only modules can be copied");
    return;
  }
  
  gsd->paste_module_conf( item->mconf);
}

void GsdAttr::activate_zoom_in()
{
  double zoom_factor;

  attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  attrnav->zoom( 1.18);
}

void GsdAttr::activate_zoom_out()
{
  double zoom_factor;

  attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  attrnav->zoom( 1.0 / 1.18);
}

void GsdAttr::activate_zoom_reset()
{
  attrnav->unzoom();
}

void GsdAttr::activate_print()
{
  char filename[80] = "pwrp_tmp:wnav.ps";
  char cmd[200];
  int sts;

  dcli_translate_filename( filename, filename);
  attrnav->print( filename);

  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
}

void GsdAttr::activate_cmd_ok()
{
  int sts;
  int idx;
  char msg[80];

  if ( save_cb) {    

    // Check syntax
    sts = gsd->syntax_check( &idx);
    if ( EVEN(sts)) {
      switch ( sts) {
      case PB__NOMODULENAME:
	sprintf( msg, "Syntax error in module %d, No module name", idx + 1);
	break;
      case PB__DUPLMODULENAME:
	sprintf( msg, "Syntax error in module %s, Duplicate module name", 
		 gsd->module_conf[idx].name);
	break;
      case PB__NOMODULECLASS:
	sprintf( msg, "Syntax error in module %s, Module class is missing", 
		 gsd->module_conf[idx].name);
	break;
      default:
	sprintf( msg, "Syntax error in module %d", idx + 1);
      }
      message( 'E', msg);
      return;
    }

    sts = (save_cb)( parent_ctx);
    if ( EVEN(sts))
      message( 'E', "Error saving profibus data");
    else
      gsd->set_modified(0);
  }
}

void GsdAttr::cmd_close_apply_cb( void *ctx, void *data)
{
  GsdAttr *attr = (GsdAttr *)ctx;
  int sts;

  sts = (attr->save_cb)( attr->parent_ctx);
  if ( EVEN(sts))
    attr->message( 'E', "Error saving profibus data");
  else {
    attr->gsd->set_modified(0);
    (attr->close_cb)( attr->parent_ctx);
  }
}

void GsdAttr::cmd_close_no_cb( void *ctx, void *data)
{
  GsdAttr *attr = (GsdAttr *)ctx;

  (attr->close_cb)( attr->parent_ctx);
}

void GsdAttr::activate_cmd_ca()
{
  if ( close_cb) {
    if ( gsd->is_modified()) {
      wow->DisplayQuestion( (void *)this, "Apply",
			    "Do you want to apply changes",
			    cmd_close_apply_cb, cmd_close_no_cb, 0);

    }
    else
      (close_cb)( parent_ctx);
  }
}

GsdAttr::~GsdAttr()
{
}

GsdAttr::GsdAttr(  void *a_parent_ctx,
		   void *a_object,
		   pb_gsd *a_gsd,
		   int a_edit_mode) :
  parent_ctx(a_parent_ctx), gsd(a_gsd), edit_mode(a_edit_mode), input_open(0), 
  object(a_object), 
  close_cb(0), save_cb(0), help_cb(0), client_data(0), recall_idx(-1),
  value_current_recall(0)
{
}

