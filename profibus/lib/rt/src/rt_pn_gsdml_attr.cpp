/* 
 * Proview   $Id: rt_pn_gsdml_attr.cpp,v 1.6 2008-10-31 12:51:29 claes Exp $
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

/* rt_pn_gsdml_attr.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

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
#include "rt_pn_gsdml_attr.h"
#include "rt_pn_gsdml_attrnav.h"


// Static member variables
char	GsdmlAttr::value_recall[30][160];

void GsdmlAttr::gsdmlattr_message( void *attr, char severity, const char *message)
{
  ((GsdmlAttr *)attr)->message( severity, message);
}

void GsdmlAttr::gsdmlattr_change_value_cb( void *attr_ctx)
{
  GsdmlAttr *attr = (GsdmlAttr *) attr_ctx;
  attr->change_value();
}

//
//  Callbackfunctions from menu entries
//
void GsdmlAttr::activate_exit()
{
  if ( close_cb) {
    if ( attrnav->is_modified()) {
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

void GsdmlAttr::activate_help()
{
  int sts;

  if ( help_cb)
    sts = (help_cb)( parent_ctx, "pn_device_editor /helpfile=\"$pwr_exe/profibus_xtthelp.dat\"");
  
}

void GsdmlAttr::activate_copy()
{
  ItemPnSlot *item;
  int sts;

  sts = attrnav->get_select( (ItemPn **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PnSlot) {
    message('E', "Only slots can be copied");
    return;
  }
  
  attrnav->dev_data.copy_slot( item->slotdata->slot_idx);

  message('I', "Slot copied");
}

void GsdmlAttr::activate_cut()
{
  ItemPnSlot *item;
  int sts;

  sts = attrnav->get_select( (ItemPn **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a slot");
    return;
  }

  if ( item->type != attrnav_eItemType_PnSlot) {
    message('E', "Only slots can be cut");
    return;
  }
  
  attrnav->dev_data.cut_slot( item->slotdata->slot_idx);
  attrnav->redraw();
}

void GsdmlAttr::activate_paste()
{
  ItemPnSlot *item;
  int sts;

  sts = attrnav->get_select( (ItemPn **) &item);
  if ( EVEN(sts)) {
    message('E', "Select a slot");
    return;
  }

  if ( item->type != attrnav_eItemType_PnSlot) {
    message('E', "Select a slot");
    return;
  }

  attrnav->dev_data.paste_slot( item->slotdata->slot_idx);  
  attrnav->redraw();
}

void GsdmlAttr::activate_viewio( int set)
{
  attrnav->set_viewio( set);
}

void GsdmlAttr::activate_zoom_in()
{
  double zoom_factor;

  attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  attrnav->zoom( 1.18);
}

void GsdmlAttr::activate_zoom_out()
{
  double zoom_factor;

  attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  attrnav->zoom( 1.0 / 1.18);
}

void GsdmlAttr::activate_zoom_reset()
{
  attrnav->unzoom();
}

void GsdmlAttr::activate_print()
{
  char filename[80] = "pwrp_tmp:wnav.ps";
  char cmd[200];
  int sts;

  dcli_translate_filename( filename, filename);
  attrnav->print( filename);

  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
}

void GsdmlAttr::activate_cmd_ok()
{
  int sts;

  attrnav->save( data_filename);

  if ( save_cb) {
    sts = (save_cb)( parent_ctx);
    if ( EVEN(sts))
      message( 'E', "Error saving profibus data");
    else
      attrnav->set_modified(0);
  }  
}

void GsdmlAttr::cmd_close_apply_cb( void *ctx, void *data)
{
  GsdmlAttr *attr = (GsdmlAttr *)ctx;
  int sts;

  attr->attrnav->save( attr->data_filename);

  if ( attr->save_cb) {
    sts = (attr->save_cb)( attr->parent_ctx);
    if ( EVEN(sts))
      attr->message( 'E', "Error saving profibus data");
    else
     (attr->close_cb)( attr->parent_ctx);
  }  
}

void GsdmlAttr::cmd_close_no_cb( void *ctx, void *data)
{
  GsdmlAttr *attr = (GsdmlAttr *)ctx;

  (attr->close_cb)( attr->parent_ctx);
}

void GsdmlAttr::activate_cmd_ca()
{

  if ( close_cb) {
    if ( edit_mode && attrnav->is_modified()) {
      wow->DisplayQuestion( (void *)this, "Apply",
			    "Do you want to apply changes",
			    cmd_close_apply_cb, cmd_close_no_cb, 0);

    }
    else
      (close_cb)( parent_ctx);
  }
}

GsdmlAttr::~GsdmlAttr()
{
}

GsdmlAttr::GsdmlAttr(  void *a_parent_ctx,
		       void *a_object,
		       pn_gsdml *a_gsdml,
		       int a_edit_mode,
		       const char *a_data_filename) :
  parent_ctx(a_parent_ctx), gsdml(a_gsdml), edit_mode(a_edit_mode), input_open(0), 
  object(a_object), 
  close_cb(0), save_cb(0), help_cb(0), client_data(0), recall_idx(-1),
  value_current_recall(0)
{
  dcli_translate_filename( data_filename, a_data_filename);
}

