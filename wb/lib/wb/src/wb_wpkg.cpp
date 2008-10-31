/* 
 * Proview   $Id: wb_wpkg.cpp,v 1.7 2008-10-31 12:51:32 claes Exp $
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

/* wb_wpkg.cpp -- package window */

#include "glow_std.h"

#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_wow.h"
#include "wb_pkg_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wpkg.h"
#include "wb_wpkgnav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "wb_pkg.h"
#include "wb_error.h"
#include "co_xhelp.h"



void WPkg::message_cb( void *wpkg, char severity, const char *message)
{
  ((WPkg *)wpkg)->message( severity, message);
}

void WPkg::set_clock_cursor_cb( void *wpkg)
{
  ((WPkg *)wpkg)->set_clock_cursor();
}

void WPkg::reset_cursor_cb( void *wpkg)
{
  ((WPkg *)wpkg)->reset_cursor();
}

//
//  Callbackfunctions from menu entries
//
void WPkg::activate_distribute()
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  message( ' ', "");

  sts = wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    message( 'E', "Select a node or package");
    return;
  }

  if ( item_count > 1) {
    for ( int i = 0; i < item_count; i++) {
      if ( typeid( *itemlist[i]) != typeid( WItemPkgNode)) {
	message( 'E', "All selected items are not nodes");
	free( itemlist);
	return;
      }
    }
    
    set_clock_cursor();
    for ( int i = 0; i < item_count; i++) {
      try {
	wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[i])->nodename);
	delete pkg;
      }
      catch ( wb_error &e) {
	message(' ', (char *)e.what().c_str());
	reset_cursor();
	free( itemlist);
	return;
      }
      wpkgnav->refresh_node( itemlist[i]);

      char msg[80];
      sprintf( msg, "Distribution successfull to %s", ((WItemPkgNode *)itemlist[i])->nodename);
      message('I', msg);
      flush();
    }
    reset_cursor();
    free( itemlist);
  }
  else {
    // One is selected
    if ( typeid( *itemlist[0]) == typeid( WItemPkgNode)) {
      set_clock_cursor();
      try {
	wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[0])->nodename);
	delete pkg;
      }
      catch ( wb_error &e) {
	message(' ', (char *)e.what().c_str());
      }
      wpkgnav->refresh_node( itemlist[0]);

      char msg[80];
      sprintf( msg, "Distribution successfull to %s", ((WItemPkgNode *)itemlist[0])->nodename);
      message('I', msg);
      reset_cursor();
    }
    else if ( typeid( *itemlist[0]) == typeid( WItemPkgPackage)) {
      set_clock_cursor();
      wb_pkg::copyPackage( ((WItemPkgPackage *)itemlist[0])->packagename);
      char msg[80];
      sprintf( msg, "Distribution successfull of %s", ((WItemPkgPackage *)itemlist[0])->packagename);
      message('I', msg);
      reset_cursor();
    }
    else {
      message( 'E', "Select a node or package");
    }
  }
}

void WPkg::activate_createpkg()
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  message( ' ', "");

  sts = wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    message( 'E', "Select a node");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgNode)) {
      message( 'E', "All selected items are not nodes");
      free( itemlist);
      return;
    }
  }
    
  set_clock_cursor();
  for ( int i = 0; i < item_count; i++) {
    try {
      wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[i])->nodename, false);
      delete pkg;
    }
    catch ( wb_error &e) {
      message(' ', (char *)e.what().c_str());
      reset_cursor();
      free( itemlist);
      return;
    }
    wpkgnav->refresh_node( itemlist[i]);

    char msg[80];
    sprintf( msg, "Package created for node %s", ((WItemPkgNode *)itemlist[i])->nodename);
    message('I', msg);
    flush();
  }
  reset_cursor();
  free( itemlist);
}

void WPkg::activate_deletepkg()
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  message( ' ', "");

  sts = wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    message( 'E', "Select a package");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgPackage)) {
      message( 'E', "All selected items are not packages");
      free( itemlist);
      return;
    }
  }
  free( itemlist);
    
  wow->DisplayQuestion( this, "Delete package", 
			"Do you want to delete selected packages",
			deletepkg_ok, 0, 0);
}

void WPkg::deletepkg_ok( void *ctx, void *data)
{
  WPkg *wpkg = (WPkg *)ctx;

  WItemPkg **itemlist;
  int item_count;
  int sts;

  sts = wpkg->wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    wpkg->message( 'E', "Select a package");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgPackage)) {
      wpkg->message( 'E', "All selected items are not packages");
      free( itemlist);
      return;
    }
  }
  WItemPkg *node_item = wpkg->wpkgnav->get_parent( itemlist[0]);

  wpkg->set_clock_cursor();
  for ( int i = 0; i < item_count; i++) {
    wpkg->wpkgnav->delete_package( (WItemPkgPackage *)itemlist[i]);
  }
  wpkg->wpkgnav->refresh_node( node_item);
  wpkg->reset_cursor();
  free( itemlist);
}

void WPkg::activate_zoom_in()
{
  double zoom_factor;

  wpkgnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  wpkgnav->zoom( 1.18);
}

void WPkg::activate_zoom_out()
{
  double zoom_factor;

  wpkgnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  wpkgnav->zoom( 1.0 / 1.18);
}

void WPkg::activate_zoom_reset()
{
  wpkgnav->unzoom();
}

void WPkg::activate_dmode_filediff( int set)
{  
  if ( set) {
    display_mode |= wpkg_mDisplayMode_FileDiff;
    wpkgnav->set_display_mode( display_mode);
  }
  else {
    display_mode &= ~wpkg_mDisplayMode_FileDiff;
    wpkgnav->set_display_mode( display_mode);
  }
}

void WPkg::activate_dmode_filetime( int set)
{  
  if ( set) {
    display_mode |= wpkg_mDisplayMode_FileOrderTime;
    wpkgnav->set_display_mode( display_mode);
  }
  else {
    display_mode &= ~wpkg_mDisplayMode_FileOrderTime;
    wpkgnav->set_display_mode( display_mode);
  }
}

void WPkg::activate_dmode_filepath( int set)
{  
  if ( set) {
    display_mode |= wpkg_mDisplayMode_FilePath;
    wpkgnav->set_display_mode( display_mode);
  }
  else {
    display_mode &= ~wpkg_mDisplayMode_FilePath;
    wpkgnav->set_display_mode( display_mode);
  }
}

WPkg::~WPkg()
{
}

WPkg::WPkg(  void *wa_parent_ctx)
  : parent_ctx(wa_parent_ctx), close_cb(0), display_mode(0)
{
}
