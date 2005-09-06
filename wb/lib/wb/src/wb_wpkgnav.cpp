/* 
 * Proview   $Id: wb_wpkgnav.cpp,v 1.5 2005-09-06 10:43:32 claes Exp $
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

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_load.h"
#include "wb_pkg_msg.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "wb_wpkg.h"
#include "wb_wpkgnav.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"
#include "wb_error.h"

static char null_str[] = "";

static int wpkgnav_init_brow_cb( FlowCtx *fctx, void *client_data);

void WPkgNav::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}


//
// Create the navigator widget
//
WPkgNav::WPkgNav(
	void 		*wa_parent_ctx,
	Widget		wa_parent_wid,
	char 		*wa_name,
	wb_eUtility	wa_utility,
	Widget 		*w,
	pwr_tStatus 	*status) :
	parent_ctx(wa_parent_ctx), parent_wid(wa_parent_wid),
	message_cb(0), set_clock_cursor_cb(0), reset_cursor_cb(0), 
	utility(wa_utility), displayed(0),
	display_mode(wpkg_mDisplayMode__)
{
  strcpy( name, wa_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	wpkgnav_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
WPkgNav::~WPkgNav()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

void WPkgNav::set_inputfocus()
{
  if ( !displayed)
    return;

  XtCallAcceptFocus( brow_widget, CurrentTime);
}


//
// Callbacks from brow
//
static int wpkgnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  WPkgNav		*wpkgnav;
  WItemPkg	       	*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &wpkgnav);
  wpkgnav->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wpkgnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( wpkgnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( wpkgnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( wpkgnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( wpkgnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wpkgnav->brow->ctx, object);
      if ( !brow_IsVisible( wpkgnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wpkgnav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wpkgnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( wpkgnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( wpkgnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( wpkgnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( wpkgnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wpkgnav->brow->ctx, object);
      if ( !brow_IsVisible( wpkgnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wpkgnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( wpkgnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
    {
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
	  if ( event->object.x < ll_x + 1.0)
          {
            // Simulate doubleclick
            flow_tEvent doubleclick_event;

            doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
            memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
            doubleclick_event->event = flow_eEvent_MB1DoubleClick;
            sts = wpkgnav_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( wpkgnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( wpkgnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( wpkgnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( wpkgnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( wpkgnav->brow->ctx);
      }
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wpkgnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( wpkgnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      item->close( wpkgnav->brow, 0, 0);
      brow_SelectClear( wpkgnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wpkgnav->brow->ctx, object);
      if ( !brow_IsVisible( wpkgnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wpkgnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;
      int		sts;

      brow_GetSelectedNodes( wpkgnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      (wpkgnav->set_clock_cursor_cb)( wpkgnav->parent_ctx);
      sts = item->open_children( wpkgnav->brow, 0, 0, wpkgnav->display_mode);
      (wpkgnav->reset_cursor_cb)( wpkgnav->parent_ctx);

      free( node_list);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
	  (wpkgnav->set_clock_cursor_cb)( wpkgnav->parent_ctx);
	  item->open_children( wpkgnav->brow, event->object.x, event->object.y, wpkgnav->display_mode);
	  (wpkgnav->reset_cursor_cb)( wpkgnav->parent_ctx);
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1ClickShift:
    {
      // Add elect
      switch ( event->object.object_type) {
      case flow_eObjectType_Node:
	if ( brow_FindSelectedObject( wpkgnav->brow->ctx, event->object.object)) {
	  brow_SetInverse( event->object.object, 0);
	  brow_SelectRemove( wpkgnav->brow->ctx, event->object.object);
	}
	else {
	  brow_SetInverse( event->object.object, 1);
	  brow_SelectInsert( wpkgnav->brow->ctx, event->object.object);
	}
	break;
      default:
	;
      }
      break;
    }
    case flow_eEvent_MB1Press:
      // Select region
      brow_SetSelectInverse( wpkgnav->brow->ctx);
      break;
    case flow_eEvent_MB1PressShift:
      // Add select region
      brow_SetSelectInverse( wpkgnav->brow->ctx);
      break;
    case flow_eEvent_Map:
    {
      wpkgnav->displayed = 1;
      break;
    }

    default:
      ;
  }
  return 1;
}

int	WPkgNav::root_objects()
{
  char fname[200];
  char line[200];
  char line_item[6][80];
  int num;
  int sts;

  dcli_translate_filename( fname, load_cNameDistribute);
  ifstream is( fname);

  while ( is.getline( line, sizeof(line))) {
    dcli_trim( line, line);
    if ( line[0] == '#' || line[0] == '!')
      continue;

    num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
    if ( !num)
      continue;

    if ( strcmp( cdh_Low(line_item[0]), "node") == 0) {
      pwr_mOpSys opsys;
      int bus;
      int dstatus;

      if ( num != 5)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[2], "%d", (int *)&opsys);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[3], "%d", &bus);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[4], "%d", &dstatus);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      new WItemPkgNode( brow, line_item[1], line_item[1], bus, opsys, 0,
			flow_eDest_IntoLast);
    }
    else
      continue;
  }

  is.close();



  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return PKG__SUCCESS;
}

void WPkgNav::enable_events()
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Press, flow_eEventType_RegionSelect, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1PressShift, flow_eEventType_RegionAddSelect, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	wpkgnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int wpkgnav_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  WPkgNav *wpkgnav = (WPkgNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  wpkgnav->brow = new WNavBrow( ctx, (void *)wpkgnav);

  wpkgnav->brow->ldh_cb_used = 0;
  wpkgnav->brow->brow_setup();
  wpkgnav->brow->create_nodeclasses();
  wpkgnav->enable_events();

  // Create the root item
  wpkgnav->root_objects();

  return 1;
}

void WPkgNav::redraw()
{
    brow_Redraw( brow->ctx, 0);
}

void WPkgNav::refresh_node( WItemPkg *item)
{
  if ( brow_IsOpen( item->node)) {
    item->close( brow, 0, 0);
    item->open_children( brow, 0, 0, display_mode);
  }
}

WItemPkg *WPkgNav::get_parent( WItemPkg *item)
{
  brow_tObject parent_node;
  WItemPkg *parent;
  int sts;

  sts = brow_GetParent( brow->ctx, item->node, &parent_node);
  if ( EVEN(sts))
    return 0;
  brow_GetUserData( parent_node, (void **)&parent);

  return parent;
}

void WPkgNav::delete_package( WItemPkgPackage *item)
{
  char cmd[200];

  sprintf( cmd, "rm $pwrp_load/%s", item->packagename);
  system( cmd);
}

int WPkgNav::get_select( WItemPkg ***items, int *item_cnt)
{
  brow_tNode	*node_list;
  int		node_count;
  WItemPkg	*item, **itemlist;

  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( node_count == 0) {
    *items = 0;
    return PKG__NOSELECT;
  }
  itemlist = (WItemPkg **) calloc( node_count, sizeof( WItemPkg *));
  for ( int i = 0; i < node_count; i++) {
    brow_GetUserData( node_list[i], (void **)&item);
    itemlist[i] = item;
  }
  *item_cnt = node_count;
  *items = itemlist;
  return PKG__SUCCESS;
}

int WItemPkg::close( WNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node)) {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & wnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    brow_ResetOpen( node, wnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

WItemPkgNode::WItemPkgNode( WNavBrow *brow, char *item_name,
			    char *item_nodename, int item_bus, pwr_mOpSys item_opsys,
			    brow_tNode dest, flow_eDest dest_code) 
  : bus(item_bus), opsys(item_opsys)
{
  strcpy( nodename, item_nodename);

  brow_CreateNode( brow->ctx, name, brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
  brow_SetAnnotation( node, 0, nodename, strlen(nodename));
}

int WItemPkgNode::open_children( WNavBrow *brow, double x, double y, int display_mode)
{
  char cmd[200];
  char tmpfile[200] = "$pwrp_tmp/wpkg.tmp";
  char line[200];
  char line_item[6][80];
  int num;
  char timestr[32];
  pwr_tTime time;
  int pkg_cnt = 0;
  WItemPkgPackage    *item;
  int inserted = 0;
  brow_tObject child;
  int sts;

  if ( brow_IsOpen( node)) {
    close( brow, 0, 0);
    return 1;
  }

  sprintf( cmd, "pwr_pkg.sh -lp $pwrp_load/pwrp_pkg_%s_*.tgz > %s", nodename, tmpfile);
  system( cmd);
  dcli_translate_filename( tmpfile, tmpfile);
  
  ifstream is( tmpfile);

  brow_SetNodraw( brow->ctx);
  while ( is.getline( line, sizeof(line))) {
    dcli_trim( line, line);
    if ( line[0] == '#' || line[0] == '!')
      continue;

    num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
    if ( num != 6)
      continue;

    sprintf( timestr, "%s %s", line_item[4], line_item[5]);
    time_AsciiToA( timestr, &time);

    sts = brow_GetChild( brow->ctx, node, &child);
    while ( ODD(sts)) {
      brow_GetUserData( child, (void **)&item);
      if ( item->time.tv_sec < time.tv_sec) {
	new WItemPkgPackage( brow, line_item[0], line_item[0], time, child,
			     flow_eDest_Before);
	inserted = 1;
	break;
      }
      sts = brow_GetNextSibling( brow->ctx, child, &child);
    }
    if ( !inserted)
      new WItemPkgPackage( brow, line_item[0], line_item[0], time, node,
			 flow_eDest_IntoLast);

    pkg_cnt++;
  }

  is.close();

  if ( pkg_cnt) {
    brow_SetOpen( node, wnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, brow->pixmap_openmap);
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return 1;
}


WItemPkgPackage::WItemPkgPackage( WNavBrow *brow, char *item_name,
			    char *item_packagename, pwr_tTime item_time,
			    brow_tNode dest, flow_eDest dest_code) 
  : time(item_time)
{
  char timestr[32];

  strcpy( packagename, item_packagename);

  time_AtoAscii( &time, time_eFormat_DateAndTime, timestr, sizeof(timestr));
  brow_CreateNode( brow->ctx, name, brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
  brow_SetAnnotation( node, 0, packagename, strlen(packagename));
  brow_SetAnnotation( node, 1, timestr, strlen(timestr));
}

int WItemPkgPackage::open_children( WNavBrow *brow, double x, double y, int display_mode)
{
  char cmd[200];
  char tmpfile[200];
  char line[200];
  char line_item[6][80];
  int num;
  char timestr[32];
  pwr_tTime time;
  int file_cnt = 0;
  WItemPkgFile    *item;
  WItemPkgPackage *next_item = 0;
  int inserted = 0;
  brow_tObject child;
  int sts;
  brow_tObject next;
  int next_was_open = 0;

  if ( brow_IsOpen( node)) {
    close( brow, 0, 0);
    return 1;
  }

  sprintf( tmpfile, "$pwrp_tmp/%s.tmp", packagename);
  sprintf( cmd, "pwr_pkg.sh -lf $pwrp_load/%s > %s", packagename, tmpfile);
  system( cmd);
  dcli_translate_filename( tmpfile, tmpfile);
  
  ifstream is( tmpfile);

  brow_SetNodraw( brow->ctx);

  if ( display_mode & wpkg_mDisplayMode_FileDiff) {
    // Display only file with newer date compared to previous package
    sts = brow_GetNextSibling( brow->ctx, node, &next);
    if ( ODD(sts)) {
      brow_GetUserData( next, (void **)&next_item);
      if ( brow_IsOpen( next)) {
	next_item->close( brow, 0, 0);
	next_was_open = 1;
      }
      next_item->open_children( brow, 0, 0, wpkg_mDisplayMode__);
    }
    else
      // No next sibling, display all files
      display_mode &= ~wpkg_mDisplayMode_FileDiff;
  }

  while ( is.getline( line, sizeof(line))) {
    dcli_trim( line, line);
    if ( line[0] == '-')
      continue;

    num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
    if ( num != 3)
      continue;

    sprintf( timestr, "%s %s", line_item[0], line_item[1]);
    time_AsciiToA( timestr, &time);

    if ( display_mode & wpkg_mDisplayMode_FileDiff) {
      // Check if this file is changed
      int keep = 1;
      sts = brow_GetChild( brow->ctx, next, &child);
      while ( ODD(sts)) {
	brow_GetUserData( child, (void **)&item);
	if ( strcmp( item->filename, line_item[2]) == 0) {
	  if ( item->time.tv_sec == time.tv_sec)
	    keep = 0;
	  break;
	}
	sts = brow_GetNextSibling( brow->ctx, child, &child);
      }
      if ( !keep)
	continue;
    }
    if ( display_mode & wpkg_mDisplayMode_FileOrderTime) {
      // Order in time
      sts = brow_GetChild( brow->ctx, node, &child);
      inserted = 0;
      while ( ODD(sts)) {
	brow_GetUserData( child, (void **)&item);
	if ( item->time.tv_sec < time.tv_sec) {
	  new WItemPkgFile( brow, "File", line_item[2], time, display_mode, child,
			    flow_eDest_Before);
	  inserted = 1;
	  break;
	}
	sts = brow_GetNextSibling( brow->ctx, child, &child);
      }
      if ( !inserted)
	new WItemPkgFile( brow, "File", line_item[2], time, display_mode, node,
			  flow_eDest_IntoLast);
      
      file_cnt++;
    }
    else {
      new WItemPkgFile( brow, "File", line_item[2], time, display_mode, node,
			flow_eDest_IntoLast);      
      file_cnt++;
    }
  }

  is.close();

  if ( display_mode & wpkg_mDisplayMode_FileDiff && next_item) {
    next_item->close( brow, 0, 0);
    if ( next_was_open)
      next_item->open_children( brow, 0, 0, display_mode);
  }

  if ( file_cnt) {
    brow_SetOpen( node, wnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, brow->pixmap_openmap);
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return 1;
}

WItemPkgFile::WItemPkgFile( WNavBrow *brow, char *item_name,
			    char *item_filename, pwr_tTime item_time, int dmode,
			    brow_tNode dest, flow_eDest dest_code) 
  : time(item_time)
{
  char timestr[32];

  strcpy( filename, item_filename);

  time_AtoAscii( &time, time_eFormat_DateAndTime, timestr, sizeof(timestr));
  brow_CreateNode( brow->ctx, name, brow->nc_table,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);

  if ( dmode & wpkg_mDisplayMode_FilePath)
    brow_SetAnnotation( node, 0, filename, strlen(filename));
  else {
    char *s;
    if ( (s = strrchr( filename, '/')))
      brow_SetAnnotation( node, 0, s+1, strlen(s+1));
    else
      brow_SetAnnotation( node, 0, filename, strlen(filename));
  }
  brow_SetAnnotation( node, 3, timestr, strlen(timestr));
}

















