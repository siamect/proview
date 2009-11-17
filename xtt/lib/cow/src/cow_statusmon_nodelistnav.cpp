/* 
 * Proview   $Id: co_statusmon_nodelistnav.cpp,v 1.10 2008-12-03 12:00:38 claes Exp $
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

/* co_statusmon_nodelistnav.cpp -- Status Monitor. */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "cow_wow.h"
#include "cow_msgwindow.h"
#include "rt_gdh.h"
#include "co_syi.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "cow_statusmon_nodelistnav.h"
#include "statussrv_utl.h"
#include "rt_pwr_msg.h"

#include "xnav_bitmap_map8.h"
#include "xnav_bitmap_map10.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_map14.h"
#include "xnav_bitmap_map16.h"
#include "xnav_bitmap_map18.h"
#include "xnav_bitmap_map20.h"
#include "xnav_bitmap_map24.h"
#include "xnav_bitmap_openmap8.h"
#include "xnav_bitmap_openmap10.h"
#include "xnav_bitmap_openmap12.h"
#include "xnav_bitmap_openmap14.h"
#include "xnav_bitmap_openmap16.h"
#include "xnav_bitmap_openmap18.h"
#include "xnav_bitmap_openmap20.h"
#include "xnav_bitmap_openmap24.h"
#include "xnav_bitmap_attr8.h"
#include "xnav_bitmap_attr10.h"
#include "xnav_bitmap_attr12.h"
#include "xnav_bitmap_attr14.h"
#include "xnav_bitmap_attr16.h"
#include "xnav_bitmap_attr18.h"
#include "xnav_bitmap_attr20.h"
#include "xnav_bitmap_attr24.h"

const char NodelistNav::config_file[40] = "$HOME/rt_statusmon.dat";

//
//  Free pixmaps
//
void NodelistNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void NodelistNavBrow::allocate_pixmaps()
{
  flow_sPixmapData pixmap_data;
  int i;

  i = 0;
  pixmap_data[i].width =xnav_bitmap_map8_width;
  pixmap_data[i].height =xnav_bitmap_map8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map8_bits;
  pixmap_data[i].width =xnav_bitmap_map10_width;
  pixmap_data[i].height =xnav_bitmap_map10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map10_bits;
  pixmap_data[i].width =xnav_bitmap_map12_width;
  pixmap_data[i].height =xnav_bitmap_map12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map12_bits;
  pixmap_data[i].width =xnav_bitmap_map14_width;
  pixmap_data[i].height =xnav_bitmap_map14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map14_bits;
  pixmap_data[i].width =xnav_bitmap_map16_width;
  pixmap_data[i].height =xnav_bitmap_map16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map16_bits;
  pixmap_data[i].width =xnav_bitmap_map18_width;
  pixmap_data[i].height =xnav_bitmap_map18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map18_bits;
  pixmap_data[i].width =xnav_bitmap_map20_width;
  pixmap_data[i].height =xnav_bitmap_map20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map20_bits;
  pixmap_data[i].width =xnav_bitmap_map20_width;
  pixmap_data[i].height =xnav_bitmap_map20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map20_bits;
  pixmap_data[i].width =xnav_bitmap_map24_width;
  pixmap_data[i].height =xnav_bitmap_map24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_map24_bits;
  
  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_map);
  
  i = 0;
  pixmap_data[i].width =xnav_bitmap_openmap8_width;
  pixmap_data[i].height =xnav_bitmap_openmap8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap8_bits;
  pixmap_data[i].width =xnav_bitmap_openmap10_width;
  pixmap_data[i].height =xnav_bitmap_openmap10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap10_bits;
  pixmap_data[i].width =xnav_bitmap_openmap12_width;
  pixmap_data[i].height =xnav_bitmap_openmap12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap12_bits;
  pixmap_data[i].width =xnav_bitmap_openmap14_width;
  pixmap_data[i].height =xnav_bitmap_openmap14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap14_bits;
  pixmap_data[i].width =xnav_bitmap_openmap16_width;
  pixmap_data[i].height =xnav_bitmap_openmap16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap16_bits;
  pixmap_data[i].width =xnav_bitmap_openmap18_width;
  pixmap_data[i].height =xnav_bitmap_openmap18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap18_bits;
  pixmap_data[i].width =xnav_bitmap_openmap20_width;
  pixmap_data[i].height =xnav_bitmap_openmap20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap20_bits;
  pixmap_data[i].width =xnav_bitmap_openmap20_width;
  pixmap_data[i].height =xnav_bitmap_openmap20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap20_bits;
  pixmap_data[i].width =xnav_bitmap_openmap24_width;
  pixmap_data[i].height =xnav_bitmap_openmap24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap24_bits;

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_openmap);

  i = 0;
  pixmap_data[i].width =xnav_bitmap_attr8_width;
  pixmap_data[i].height =xnav_bitmap_attr8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr8_bits;
  pixmap_data[i].width =xnav_bitmap_attr10_width;
  pixmap_data[i].height =xnav_bitmap_attr10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr10_bits;
  pixmap_data[i].width =xnav_bitmap_attr12_width;
  pixmap_data[i].height =xnav_bitmap_attr12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr12_bits;
  pixmap_data[i].width =xnav_bitmap_attr14_width;
  pixmap_data[i].height =xnav_bitmap_attr14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr14_bits;
  pixmap_data[i].width =xnav_bitmap_attr16_width;
  pixmap_data[i].height =xnav_bitmap_attr16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr16_bits;
  pixmap_data[i].width =xnav_bitmap_attr18_width;
  pixmap_data[i].height =xnav_bitmap_attr18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr18_bits;
  pixmap_data[i].width =xnav_bitmap_attr20_width;
  pixmap_data[i].height =xnav_bitmap_attr20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr20_bits;
  pixmap_data[i].width =xnav_bitmap_attr20_width;
  pixmap_data[i].height =xnav_bitmap_attr20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr20_bits;
  pixmap_data[i].width =xnav_bitmap_attr24_width;
  pixmap_data[i].height =xnav_bitmap_attr24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attr24_bits;
  
  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attr);

}

//
// Create nodeclasses
//
void NodelistNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  // Nodeclass for node
  brow_CreateNodeClass( ctx, "Node", 
		flow_eNodeGroup_Common, &nc_node);
  brow_AddAnnotPixmap( nc_node, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_node, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_DarkGray);
  brow_AddRect( nc_node, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_node, 2.2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_node, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_node, 25, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddFrame( nc_node, 0, 0, 35, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "Attr", 
		flow_eNodeGroup_Common, &nc_attr);
  brow_AddAnnotPixmap( nc_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr, 2.2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_attr, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create system status attribute nodeclass

  brow_CreateNodeClass( ctx, "SysStsAttr", 
		flow_eNodeGroup_Common, &nc_sys_sts_attr);
  brow_AddAnnotPixmap( nc_sys_sts_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_sys_sts_attr, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_DarkGray);
  brow_AddRect( nc_sys_sts_attr, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_sys_sts_attr, 2.2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_sys_sts_attr, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_sys_sts_attr, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create system status attribute nodeclass

  brow_CreateNodeClass( ctx, "StsAttr", 
		flow_eNodeGroup_Common, &nc_sts_attr);
  brow_AddAnnotPixmap( nc_sts_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_sts_attr, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_DarkGray);
  brow_AddRect( nc_sts_attr, 1.3, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_sts_attr, 2.2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_sts_attr, 5, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_sts_attr, 10, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_sts_attr, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);
}

void NodelistNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, nodelistnav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	NodelistNav::brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int NodelistNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  int sts;
  NodelistNav *nodelistnav = (NodelistNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  nodelistnav->brow = new NodelistNavBrow( ctx, (void *)nodelistnav);

  nodelistnav->brow->brow_setup();
  nodelistnav->brow->create_nodeclasses();

  if ( strcmp( nodelistnav->nodename, "") != 0) {
    nodelistnav->add_node( nodelistnav->nodename, "", "");
    nodelistnav->node_list[0].item->open_children( nodelistnav, 0, 0);
  }
  else
    nodelistnav->read();

  sts = brow_TraceInit( ctx, trace_connect_bc, 
			trace_disconnect_bc, trace_scan_bc);
  nodelistnav->trace_started = 1;

  nodelistnav->trace_start();

  return 1;
}

NodelistNav::NodelistNav( void *nodelist_parent_ctx, MsgWindow *nodelistnav_msg_window, 
			  char *nodelistnav_nodename, int nodelistnav_mode, 
			  int nodelistnav_view_node_descr, int nodelistnav_msgw_pop) :
  parent_ctx(nodelist_parent_ctx),
  nodelist_size(0), trace_started(0), scantime(4000), first_scan(1),
  msg_window(nodelistnav_msg_window), msgw_pop(nodelistnav_msgw_pop),
  mode(nodelistnav_mode), view_node_descr(nodelistnav_view_node_descr)
{
  if ( nodelistnav_nodename)
    strcpy( nodename, nodelistnav_nodename);
  else
    strcpy( nodename, "");
}


//
//  Delete ev
//
NodelistNav::~NodelistNav()
{
}

NodelistNavBrow::~NodelistNavBrow()
{
  free_pixmaps();
}

void NodelistNav::read()
{
  char line[400];
  char line_part[3][256];
  int sts;
  FILE *fp;
  pwr_tFileName fname;
  ItemNode *item;
  brow_tNode dest = 0;


  dcli_translate_filename( fname, config_file);
  fp = fopen( fname, "r");
  if ( !fp)
    return;

  while( 1) {
    sts = dcli_read_line( line, sizeof(line), fp);
    if ( !sts)
      break;
    
    dcli_trim( line, line);
    
    if ( line[0] == 0 || line[0] == '#' || line[0] == '!')
      continue;

    int num = dcli_parse( line, " ", "", (char *)line_part, 
			  sizeof(line_part)/sizeof(line_part[0]), sizeof(line_part[0]), 0);

    NodelistNode node( line_part[0]);
    if ( num >= 2)
      strncpy( node.opplace, line_part[1], sizeof(node.opplace));
    if ( num >= 3)
      strncpy( node.description, line_part[2], sizeof(node.description));

    node_list.push_back( node);
  }
  fclose( fp);


  brow_SetNodraw( brow->ctx);
  brow_DeleteAll( brow->ctx);

  for ( int i = 0; i < (int)node_list.size(); i++) {
    
    item = new ItemNode( this, node_list[i].node_name, node_list[i].description, dest, flow_eDest_After);
    dest = item->node;
    node_list[i].item = item;
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

//
//  Zoom
//
void NodelistNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void NodelistNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

void NodelistNav::set_mode( int nodelist_mode)
{
  mode = nodelist_mode;
}

void NodelistNav::set_nodraw()
{
  brow_SetNodraw( brow->ctx);
}

void NodelistNav::reset_nodraw()
{
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

//
// Callbacks from brow
//
int NodelistNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  NodelistNav		*nodelistnav;
  ItemNode 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted) {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &nodelistnav);
  switch ( event->event) {
  case flow_eEvent_Key_Up: {
    brow_tNode	*nodelist;
    int		node_count;
    brow_tObject	object;
    int		sts;

    brow_GetSelectedNodes( nodelistnav->brow->ctx, &nodelist, &node_count);
    if ( !node_count) {
      sts = brow_GetLastVisible( nodelistnav->brow->ctx, &object);
      if ( EVEN(sts)) return 1;
    }
    else {
      if ( !brow_IsVisible( nodelistnav->brow->ctx, nodelist[0], flow_eVisible_Partial)) {
	sts = brow_GetLastVisible( nodelistnav->brow->ctx, &object);
	if ( EVEN(sts)) return 1;
      }
      else {
	sts = brow_GetPrevious( nodelistnav->brow->ctx, nodelist[0], &object);
	if ( EVEN(sts)) {
	  sts = brow_GetLast( nodelistnav->brow->ctx, &object);
	  if ( EVEN(sts)) {
	    if ( node_count)
	      free( nodelist);
	    return 1;
	  }
	}
      }
    }
    brow_SelectClear( nodelistnav->brow->ctx);
    brow_SetInverse( object, 1);
    brow_SelectInsert( nodelistnav->brow->ctx, object);
    if ( !brow_IsVisible( nodelistnav->brow->ctx, object, flow_eVisible_Full))
      brow_CenterObject( nodelistnav->brow->ctx, object, 0.25);
    if ( node_count)
      free( nodelist);
    break;
  }
  case flow_eEvent_Key_Down: {
    brow_tNode	*nodelist;
    int		node_count;
    brow_tObject	object;
    int		sts;

    brow_GetSelectedNodes( nodelistnav->brow->ctx, &nodelist, &node_count);
    if ( !node_count) {
      sts = brow_GetFirstVisible( nodelistnav->brow->ctx, &object);
      if ( EVEN(sts)) return 1;
    }
    else {
      if ( !brow_IsVisible( nodelistnav->brow->ctx, nodelist[0], flow_eVisible_Partial)) {
	sts = brow_GetFirstVisible( nodelistnav->brow->ctx, &object);
	if ( EVEN(sts)) return 1;
      }
      else {
	sts = brow_GetNext( nodelistnav->brow->ctx, nodelist[0], &object);
	if ( EVEN(sts)) {
	  sts = brow_GetFirst( nodelistnav->brow->ctx, &object);
	  if ( EVEN(sts)) {
	    if ( node_count)
	      free( nodelist);
	    return 1;
	  }
	}
      }
    }
    brow_SelectClear( nodelistnav->brow->ctx);
    brow_SetInverse( object, 1);
    brow_SelectInsert( nodelistnav->brow->ctx, object);
    if ( !brow_IsVisible( nodelistnav->brow->ctx, object, flow_eVisible_Full))
      brow_CenterObject( nodelistnav->brow->ctx, object, 0.75);
    if ( node_count)
      free( nodelist);
    break;
  }
  case flow_eEvent_Key_PageDown: {
    brow_Page( nodelistnav->brow->ctx, 0.9);
    break;
  }
  case flow_eEvent_Key_PageUp: {
    brow_Page( nodelistnav->brow->ctx, -0.9);
    break;
  }
  case flow_eEvent_ScrollDown: {
    brow_Page( nodelistnav->brow->ctx, 0.1);
    break;
  }
  case flow_eEvent_ScrollUp: {
    brow_Page( nodelistnav->brow->ctx, -0.1);
    break;
  }
  case flow_eEvent_SelectClear:
    brow_ResetSelectInverse( nodelistnav->brow->ctx);
    break;
  case flow_eEvent_MB1Click:
    // Select
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      double ll_x, ll_y, ur_x, ur_y;
        
      brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
      if ( event->object.x < ll_x + 1.0) {
	// Simulate doubleclick
	flow_tEvent doubleclick_event;
	int		sts;
	
	doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
	memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
	doubleclick_event->event = flow_eEvent_MB1DoubleClick;
	sts = brow_cb( ctx, doubleclick_event);
	free( (char *) doubleclick_event);
	return sts;
      }

      if ( brow_FindSelectedObject( nodelistnav->brow->ctx, event->object.object)) {
	brow_SelectClear( nodelistnav->brow->ctx);
      }
      else {
	brow_SelectClear( nodelistnav->brow->ctx);
	brow_SetInverse( event->object.object, 1);
	brow_SelectInsert( nodelistnav->brow->ctx, event->object.object);
      }
      break;
    default:
      brow_SelectClear( nodelistnav->brow->ctx);
    }
    break;
  case flow_eEvent_Key_Right: {
    brow_tNode	*nodelist;
    int		node_count;
    
    brow_GetSelectedNodes( nodelistnav->brow->ctx, &nodelist, &node_count);
    if ( !node_count)
      return 1;
    
    brow_GetUserData( nodelist[0], (void **)&item);
    item->open_children( nodelistnav, 0, 0);
    nodelistnav->force_trace_scan();
    free( nodelist);
    break;
  }
  case flow_eEvent_Key_Left: {
    brow_tNode	*nodelist;
    int		node_count;
    brow_tObject	object;
    int		sts;

    brow_GetSelectedNodes( nodelistnav->brow->ctx, &nodelist, &node_count);
    if ( !node_count)
      return 1;

    if ( brow_IsOpen( nodelist[0]))
      // Close this node
      object = nodelist[0];
    else {
      // Close parent
      sts = brow_GetParent( nodelistnav->brow->ctx, nodelist[0], &object);
      if ( EVEN(sts)) {
	free( nodelist);
	return 1;
      }
    }
    brow_GetUserData( object, (void **)&item);
    item->close( nodelistnav, 0, 0);
    brow_SelectClear( nodelistnav->brow->ctx);
    brow_SetInverse( object, 1);
    brow_SelectInsert( nodelistnav->brow->ctx, object);
    if ( !brow_IsVisible( nodelistnav->brow->ctx, object, flow_eVisible_Full))
      brow_CenterObject( nodelistnav->brow->ctx, object, 0.25);
    free( nodelist);
    break;
  }
  case flow_eEvent_MB1DoubleClick: {
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      brow_GetUserData( event->object.object, (void **)&item);
      item->open_children( nodelistnav, event->object.x, event->object.y);
      break;
    default:
      ;
    }
    break;
  }
  case flow_eEvent_MB3Down: {
    brow_SetClickSensitivity( nodelistnav->brow->ctx, 
			      flow_mSensitivity_MB3Press);
    break;
  }
  default:
    ;
  }
  return 1;
}

int NodelistNav::trace_scan_bc( brow_tObject object, void *p)
{
  ItemBase     	*base_item;
  char		buf[200];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type) {
  case nodelistnav_eItemType_Attr: {
    ItemAttr	*item = (ItemAttr *)base_item;

    if ( item->size == 0)
      break;

    if ( !item->first_scan) {
      if ( item->size > (int) sizeof(item->old_value) && 
	   item->type_id == pwr_eType_String &&
	   strlen((char *)p) < sizeof(item->old_value) && 
	   strcmp( (char *)p, item->old_value) == 0)
	// No change since last time
	return 1;
      else if ( memcmp( item->old_value, p, item->size) == 0)
	// No change since last time
	return 1;
    }
    else
      item->first_scan = 0;

    attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
    brow_SetAnnotation( object, 1, buf, len);
    memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
    break;
  }
  case nodelistnav_eItemType_AttrSysSts: {
    ItemAttrSysSts	*item = (ItemAttrSysSts *)base_item;

    if ( item->size == 0)
      break;

    if ( !item->first_scan) {
      if ( item->size > (int) sizeof(item->old_value) && 
	   item->type_id == pwr_eType_String &&
	   strlen((char *)p) < sizeof(item->old_value) && 
	   strcmp( (char *)p, item->old_value) == 0)
	// No change since last time
	return 1;
      else if ( memcmp( item->old_value, p, item->size) == 0)
	// No change since last time
	return 1;
    }
    else
      item->first_scan = 0;

    attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
    brow_SetAnnotation( object, 1, buf, len);
    memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));

    flow_eDrawType color;
    pwr_tStatus sts = *(pwr_tStatus *)item->status_p;

    if ( sts == 0)
      color = flow_eDrawType_DarkGray;
    else {
      switch ( sts & 7) {
      case 3:
      case 1:
	color = flow_eDrawType_Green;
	break;
      case 0:
	color = flow_eDrawType_Yellow;
	break;
      case 2:
      case 4:
	color = flow_eDrawType_LineRed;
	break;
      default:
	color = flow_eDrawType_DarkGray;
      }
    }
    
    brow_SetFillColor( item->node, color);

    break;
  }
  case nodelistnav_eItemType_AttrSts: {
    ItemAttrSts	*item = (ItemAttrSts *)base_item;

    if ( !item->first_scan) {
      if (  strcmp( (char *)p, item->old_value) == 0 &&
	    strcmp( item->name_p, item->old_name) == 0)
	// No change since last time
	return 1;
    }
    else
      item->first_scan = 0;

    attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
    brow_SetAnnotation( object, 1, item->name_p, strlen(item->name_p));
    brow_SetAnnotation( object, 2, item->value_p, strlen(item->value_p));
    strcpy( item->old_value, item->value_p);
    strcpy( item->old_name, item->name_p);

    flow_eDrawType color;
    pwr_tStatus sts = *(pwr_tStatus *)item->status_p;

    if ( sts == 0)
      color = flow_eDrawType_DarkGray;
    else {
      switch ( sts & 7) {
      case 3:
      case 1:
	color = flow_eDrawType_Green;
	break;
      case 0:
	color = flow_eDrawType_Yellow;
	break;
      case 2:
      case 4:
	color = flow_eDrawType_LineRed;
	break;
      default:
	color = flow_eDrawType_DarkGray;
      }
    }
    
    brow_SetFillColor( item->node, color);

    break;
  }
  default:
    ;
  }
  return 1;
}

int NodelistNav::trace_connect_bc( brow_tObject object, char *name, char *attr, 
				   flow_eTraceType type, void **p)
{
  ItemBase 		*base_item;

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type) {
  case nodelistnav_eItemType_Attr: {
    ItemAttr	*item = (ItemAttr *) base_item;

    if (item->size == 0)
      break;

    *p = item->value_p;
    break;
  }
  case nodelistnav_eItemType_AttrSysSts: {
    ItemAttrSysSts	*item = (ItemAttrSysSts *) base_item;

    if (item->size == 0)
      break;

    *p = item->value_p;
    break;
  }
  case nodelistnav_eItemType_AttrSts: {
    ItemAttrSts	*item = (ItemAttrSts *) base_item;

    *p = item->value_p;
    break;
  }
  default:
    ;
  }      
  return 1;
}

int NodelistNav::trace_disconnect_bc( brow_tObject object)
{
  return 1;
}

void NodelistNav::force_trace_scan()
{
  if ( trace_started)
    brow_TraceScan( brow->ctx);
}

void NodelistNav::message( pwr_tStatus sts, const char *node, int idx, const char *text)
{
  int severity;
  char msg[200];
  pwr_tOid oid = { 0, 1};

  if ( !msg_window)
    return;

  switch ( sts & 7) {
  case 3:
    severity = 'S';
  case 1:
    severity = 'I';
    break;
  case 0: 
    severity = 'W';
    break;
  case 2:
    severity = 'E';
    break;
  case 4:
    severity = 'F';
    break;
  default:
    severity = ' ';
  }

  strcpy( msg, node);
  strcat( msg, "  ");
  strncat( msg, text, sizeof(msg) - strlen(msg));
  oid.oix = idx;

  msgw_ePop pop;
  if ( msgw_pop)
    pop = msgw_ePop_Default;
  else
    pop = msgw_ePop_No;

  msg_window->msg( severity, msg, pop, oid);
  if ( EVEN(sts))
    beep();
}

int NodelistNav::select_node( int idx)
{
  if ( idx >= (int)node_list.size())
    return 0;

  brow_tObject object = node_list[idx].item->node;

  brow_SelectClear( brow->ctx);
  brow_SetInverse( object, 1);
  brow_SelectInsert( brow->ctx, object);
  if ( !brow_IsVisible( brow->ctx, object, flow_eVisible_Full))
    brow_CenterObject( brow->ctx, object, 0.25);

  return 1;
}

int NodelistNav::update_nodes()
{
  pwr_tStatus sts;
  statussrv_sGetStatus response;
  int nodraw = 0;
  pwr_tStatus current_status;
  char current_status_str[120];

  for ( int i = 0; i < (int) node_list.size(); i++) {
    sts = statussrv_GetStatus( node_list[i].node_name, &response);

    if ( !first_scan && ODD(sts) && EVEN(node_list[i].connection_sts))
      message( sts, node_list[i].node_name, i, "Connection up to server");
    else if ( (!first_scan && EVEN(sts) && ODD(node_list[i].connection_sts)) ||
	      (first_scan && EVEN(sts)))
      message( sts, node_list[i].node_name, i, "Connection down to server");
    node_list[i].connection_sts = sts;

    if ( ODD( sts)) {
      if ( memcmp( &node_list[i].item->data.BootTime, &response.BootTime, sizeof(pwr_tTime)) &&
	   node_list[i].init_done) {
	// System restarted
	message( 2, node_list[i].node_name, i, "Proview restarted");
      }
      if ( memcmp( &node_list[i].item->data.RestartTime, &response.RestartTime, sizeof(pwr_tTime)) &&
	   node_list[i].init_done) {
	// System restarted
	message( 2, node_list[i].node_name, i, "Proview restarted");
      }
    }

    if ( EVEN(sts)) {
      current_status = response.SystemStatus;
      strncpy( current_status_str, response.SystemStatusStr, sizeof(current_status_str));
    }
    else {
      switch ( mode) {
      case nodelist_eMode_SystemStatus:
	current_status = response.SystemStatus;
	strncpy( current_status_str, response.SystemStatusStr, sizeof(current_status_str));
	break;
      case nodelist_eMode_Status1:
	current_status = response.UserStatus[0];
	strncpy( current_status_str, response.UserStatusStr[0], sizeof(current_status_str));
	break;
      case nodelist_eMode_Status2:
	current_status = response.UserStatus[1];
	strncpy( current_status_str, response.UserStatusStr[1], sizeof(current_status_str));
	break;
      case nodelist_eMode_Status3:
	current_status = response.UserStatus[2];
	strncpy( current_status_str, response.UserStatusStr[2], sizeof(current_status_str));
	break;
      case nodelist_eMode_Status4:
	current_status = response.UserStatus[3];
	strncpy( current_status_str, response.UserStatusStr[3], sizeof(current_status_str));
	break;
      case nodelist_eMode_Status5:
	current_status = response.UserStatus[4];
	strncpy( current_status_str, response.UserStatusStr[4], sizeof(current_status_str));
	break;
      }
    }
    if ( node_list[i].item->data.CurrentStatus != current_status) {
      // Change color
      if ( !nodraw) {
	brow_SetNodraw( brow->ctx);
	nodraw = 1;
      }

      node_list[i].item->update_color( this, current_status);

      // Message if switch to error
      if ( !first_scan && ODD(sts)) {
	message( current_status, node_list[i].node_name, i, current_status_str);
      }
      else if ( EVEN(current_status) && ODD(sts))
	// Message even status first scan
	message( current_status, node_list[i].node_name, i, current_status_str);
    }
      
    if ( strcmp( node_list[i].item->data.CurrentStatusStr, current_status_str) != 0)
      brow_SetAnnotation( node_list[i].item->node, 2, current_status_str, 
			  strlen(current_status_str));

    node_list[i].item->data.CurrentStatus = current_status;
    strncpy( node_list[i].item->data.CurrentStatusStr, current_status_str, 
	     sizeof(node_list[i].item->data.SystemStatusStr));

    node_list[i].item->data.SystemStatus = response.SystemStatus;
    strncpy( node_list[i].item->data.SystemStatusStr, response.SystemStatusStr, 
	     sizeof(node_list[i].item->data.SystemStatusStr));

    if ( ODD(sts)) {
      if ( strcmp( node_list[i].item->data.Description, response.Description) != 0 &&
	   view_node_descr == 0 && strcmp( node_list[i].item->node_descr, "") == 0)
	brow_SetAnnotation( node_list[i].item->node, 1, response.Description, 
			    strlen(response.Description));

      strncpy( node_list[i].item->data.Description, response.Description, 
	       sizeof(node_list[i].item->data.Description));
      strncpy( node_list[i].item->data.Version, response.Version, 
	       sizeof(node_list[i].item->data.Version));
      node_list[i].item->data.SystemTime = response.SystemTime;
      node_list[i].item->data.BootTime = response.BootTime;
      node_list[i].item->data.RestartTime = response.RestartTime;
      node_list[i].item->data.Restarts = response.Restarts;

      if ( !node_list[i].init_done)
	node_list[i].init_done = 1;
      if ( node_list[i].item->syssts_open) {
	statussrv_GetExtStatus( node_list[i].node_name, &node_list[i].item->xdata);
      }
    }

  }
  if ( nodraw) {
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }
  first_scan = 0;
  return 1;
}

void NodelistNav::save()
{
  pwr_tFileName fname;
  FILE *fp;

  dcli_translate_filename( fname, config_file);
  fp = fopen( fname, "w");
  if ( !fp)
    return;

  for ( int i = 0; i < (int)node_list.size(); i++) {
    fprintf( fp, "%s \"%s\" \"%s\"\n", node_list[i].node_name, node_list[i].opplace, 
	     node_list[i].description);
  }

  fclose( fp);
}

int NodelistNav::get_selected_node( char *name)
{
  brow_tNode	*nodelist;
  int		node_count;
  ItemNode     	*item;

  brow_GetSelectedNodes( brow->ctx, &nodelist, &node_count);
  if ( node_count != 1)
    return 0;

  brow_GetUserData( nodelist[0], (void **)&item);
  free( nodelist);

  if ( item->type != nodelistnav_eItemType_Node)
    return 0;

  strcpy( name, item->name);
  return 1;
}

int NodelistNav::get_selected_opplace( char *opplace, char *descr)
{
  brow_tNode	*nodelist;
  int		node_count;
  ItemNode     	*item;

  brow_GetSelectedNodes( brow->ctx, &nodelist, &node_count);
  if ( node_count != 1)
    return 0;

  brow_GetUserData( nodelist[0], (void **)&item);
  free( nodelist);

  if ( item->type != nodelistnav_eItemType_Node)
    return 0;

  for ( int i = 0; i < (int) node_list.size(); i++) {
    if ( node_list[i].item == item) {
      if ( opplace)
	strcpy( opplace, node_list[i].opplace);
      if ( descr)
	strcpy( descr, node_list[i].description);
      return 1;
    }
  }
  return 0;
}

int NodelistNav::set_node_data( char *node_name, char *opplace, char *descr)
{
  for ( int i = 0; i < (int) node_list.size(); i++) {
    if ( strcmp( node_list[i].node_name, node_name) == 0) {
      if ( opplace)
	strncpy( node_list[i].opplace, opplace, sizeof(node_list[i].opplace));
      if ( descr)
	strncpy( node_list[i].description, descr, sizeof(node_list[i].description));
      return 1;
    }
  }
  return 0;
}

void NodelistNav::remove_node( char *name)
{
  for ( int i = 0; i < (int) node_list.size(); i++) {
    if ( cdh_NoCaseStrcmp( name, node_list[i].node_name) == 0) {
      brow_DeleteNode( brow->ctx, node_list[i].item->node);

      for ( int j = i; j < (int)node_list.size() - 1; j++)
	node_list[j] = node_list[j + 1];
      node_list.pop_back();
      break;
    }
  }
}

void NodelistNav::add_node( const char *name, const char *description, const char *opplace)
{
  brow_tNode	*nodelist;
  int		node_count;
  ItemNode     	*item;
  int 		idx = 0;
  bool		found;

  if ( strcmp( name, "") == 0)
    return;

  brow_GetSelectedNodes( brow->ctx, &nodelist, &node_count);

  if ( node_count > 0) {
    // Get index for selected node
    found = false;
    for ( int i = 0; i < (int)node_list.size(); i++) {
      if ( node_list[i].item->node == nodelist[0]) {
	idx = i + 1;
	found = true;
	break;
      }
    }
    if ( !found)
      return;
    
    NodelistNode node( name);
    strncpy( node.opplace, opplace, sizeof(node.opplace));
    strncpy( node.description, description, sizeof(node.description));

    if ( idx == (int)node_list.size())
      node_list.push_back( node);
    else {
      node_list.push_back( node_list[node_list.size()-1]);
      for ( int i = node_list.size() - 1; i > idx; i--) {
	node_list[i] = node_list[i-1];
      }
      
      node_list[idx] = node;
    }    

    item = new ItemNode( this, name, node_list[idx].description, nodelist[0], 
			 flow_eDest_After);
    node_list[idx].item = item;
  }
  else {
    // Nothing selected, insert last
    NodelistNode node( name);
    strcpy( node.opplace, opplace);
    strcpy( node.description, description);
    node_list.push_back( node);

    item = new ItemNode( this, name, node.description, 0, flow_eDest_IntoLast);
    node_list[node_list.size()-1].item = item;
  }
}

//
// Convert attribute value to string
//
void NodelistNav::attrvalue_to_string( int type_id, void *value_ptr, 
				       char *str, int size, int *len, char *format)
{

  if ( value_ptr == 0) {
    strcpy( str, "UNDEFINED");
    return;
  }

  switch ( type_id ) {
  case pwr_eType_Boolean: {
    if ( !format)
      *len = sprintf( str, "%d", *(pwr_tBoolean *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tBoolean *)value_ptr);
    break;
  }
  case pwr_eType_Float32: {
    if ( !format)
      *len = sprintf( str, "%f", *(pwr_tFloat32 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tFloat32 *)value_ptr);
    break;
  }
  case pwr_eType_Int32: {
    if ( !format)
      *len = sprintf( str, "%d", *(pwr_tInt32 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tInt32 *)value_ptr);
    break;
  }
  case pwr_eType_String: {
    strncpy( str, (char *)value_ptr, size);
    str[size-1] = 0;
    *len = strlen(str);
    break;
  }
  case pwr_eType_Time: {
    char timstr[40];
    pwr_tStatus sts;

    sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_DateAndTime, 
			 timstr, sizeof(timstr));
    if ( EVEN(sts))
      strcpy( timstr, "-");
    *len = sprintf( str, "%s", timstr);
    break;
  }
  default: ;
  }
}

ItemNode::ItemNode( NodelistNav *item_nodelistnav, const char *item_name, const char *item_node_descr,
		    brow_tNode dest, flow_eDest dest_code):
  ItemBase( item_nodelistnav, item_name), syssts_open(0)
{
  type = nodelistnav_eItemType_Node;
  strcpy( name, item_name);
  strcpy( node_descr, item_node_descr);
  memset( &xdata, 0, sizeof(xdata));

  brow_CreateNode( nodelistnav->brow->ctx, item_name, nodelistnav->brow->nc_node,
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotation( node, 0, name, strlen(name));
  if ( nodelistnav->view_node_descr || strcmp( node_descr, "") != 0)
    brow_SetAnnotation( node, 1, node_descr, strlen(node_descr));
  else 
    brow_SetAnnotation( node, 1, data.Description, strlen(data.Description));
  brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);
}

int ItemNode::update_color( NodelistNav *nodelistnav, pwr_tStatus system_status)
{
  flow_eDrawType color;

  if ( system_status == 0)
    color = flow_eDrawType_DarkGray;
  else {
    switch ( system_status & 7) {
    case 3:
    case 1:
      color = flow_eDrawType_Green;
      break;
    case 0:
      color = flow_eDrawType_Yellow;
      break;
    case 2:
    case 4:
      color = flow_eDrawType_LineRed;
      break;
    default:
      color = flow_eDrawType_DarkGray;
    }
  }

  brow_SetFillColor( node, color);
  return 1;
}

int ItemNode::open_children( NodelistNav *nodelistnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( nodelistnav->brow->ctx);
    brow_CloseNode( nodelistnav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( nodelistnav->brow->ctx);

    new ItemAttr( nodelistnav, "Description", "Description", 
	pwr_eType_String, sizeof(data.Description), data.Description,
	node, flow_eDest_IntoLast);

    new ItemAttrSysSts( nodelistnav, "SystemStatus", "SystemStatus", 
	pwr_eType_String, sizeof(data.SystemStatusStr), &data.SystemStatusStr,
	&data.SystemStatus, this, node, flow_eDest_IntoLast);

    new ItemAttr( nodelistnav, "SystemTime", "SystemTime", 
	pwr_eType_Time, sizeof(data.SystemTime), &data.SystemTime,
	node, flow_eDest_IntoLast);

    new ItemAttr( nodelistnav, "BootTime", "BootTime", 
	pwr_eType_Time, sizeof(data.BootTime), &data.BootTime,
	node, flow_eDest_IntoLast);

    new ItemAttr( nodelistnav, "RestartTime", "RestartTime", 
	pwr_eType_Time, sizeof(data.RestartTime), &data.RestartTime,
	node, flow_eDest_IntoLast);

    new ItemAttr( nodelistnav, "Restarts", "Restarts", 
	pwr_eType_Int32, sizeof(data.Restarts), &data.Restarts,
	node, flow_eDest_IntoLast);

    new ItemAttr( nodelistnav, "Version", "Version", 
	pwr_eType_String, sizeof(data.Version), &data.Version,
	node, flow_eDest_IntoLast);

    brow_SetOpen( node, 1);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_openmap);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemNode::close( NodelistNav *nodelistnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( nodelistnav->brow->ctx);
    brow_CloseNode( nodelistnav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);
  }
  return 1;
}

ItemAttr::ItemAttr( NodelistNav *item_nodelistnav, const char *item_name, const char *attr, 
	int attr_type, int attr_size, void *attr_value_p,
	brow_tNode dest, flow_eDest dest_code) :
	ItemBase( item_nodelistnav, item_name), value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size)
{

  type = nodelistnav_eItemType_Attr;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  brow_CreateNode( nodelistnav->brow->ctx, item_name, nodelistnav->brow->nc_attr,
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

ItemAttrSysSts::ItemAttrSysSts( NodelistNav *item_nodelistnav, const char *item_name, const char *attr, 
				  int attr_type, int attr_size, void *attr_value_p, 
				  void *attr_status_p, ItemNode *attr_parent, 
				brow_tNode dest, flow_eDest dest_code) :
  ItemBase( item_nodelistnav, item_name), value_p(attr_value_p), status_p(attr_status_p), 
  first_scan(1), type_id(attr_type), size(attr_size), parent(attr_parent)
{

  type = nodelistnav_eItemType_AttrSysSts;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  brow_CreateNode( nodelistnav->brow->ctx, item_name, nodelistnav->brow->nc_sys_sts_attr,
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

int ItemAttrSysSts::open_children( NodelistNav *nodelistnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( nodelistnav->brow->ctx);
    brow_CloseNode( nodelistnav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);
    parent->syssts_open = 0;
    memset( &parent->xdata, 0, sizeof(parent->xdata));
  }
  else {
    parent->syssts_open = 1;
    brow_SetNodraw( nodelistnav->brow->ctx);

    new ItemAttrSts( nodelistnav, "Sys", "S1", 
		     parent->xdata.ServerStsStr[0],
		     &parent->xdata.ServerSts[0],
		     parent->xdata.ServerStsName[0],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S2", 
		     parent->xdata.ServerStsStr[1],
		     &parent->xdata.ServerSts[1],
		     parent->xdata.ServerStsName[1],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S3", 
		     parent->xdata.ServerStsStr[2],
		     &parent->xdata.ServerSts[2],
		     parent->xdata.ServerStsName[2],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S4", 
		     parent->xdata.ServerStsStr[3],
		     &parent->xdata.ServerSts[3],
		     parent->xdata.ServerStsName[3],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S5", 
		     parent->xdata.ServerStsStr[4],
		     &parent->xdata.ServerSts[4],
		     parent->xdata.ServerStsName[4],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S6", 
		     parent->xdata.ServerStsStr[5],
		     &parent->xdata.ServerSts[5],
		     parent->xdata.ServerStsName[5],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S7", 
		     parent->xdata.ServerStsStr[6],
		     &parent->xdata.ServerSts[6],
		     parent->xdata.ServerStsName[6],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S8", 
		     parent->xdata.ServerStsStr[7],
		     &parent->xdata.ServerSts[7],
		     parent->xdata.ServerStsName[7],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S9", 
		     parent->xdata.ServerStsStr[8],
		     &parent->xdata.ServerSts[8],
		     parent->xdata.ServerStsName[8],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S10", 
		     parent->xdata.ServerStsStr[9],
		     &parent->xdata.ServerSts[9],
		     parent->xdata.ServerStsName[9],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S11", 
		     parent->xdata.ServerStsStr[10],
		     &parent->xdata.ServerSts[10],
		     parent->xdata.ServerStsName[10],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S12", 
		     parent->xdata.ServerStsStr[11],
		     &parent->xdata.ServerSts[11],
		     parent->xdata.ServerStsName[11],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S13", 
		     parent->xdata.ServerStsStr[12],
		     &parent->xdata.ServerSts[12],
		     parent->xdata.ServerStsName[12],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S14", 
		     parent->xdata.ServerStsStr[13],
		     &parent->xdata.ServerSts[13],
		     parent->xdata.ServerStsName[13],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S15", 
		     parent->xdata.ServerStsStr[14],
		     &parent->xdata.ServerSts[14],
		     parent->xdata.ServerStsName[14],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S16", 
		     parent->xdata.ServerStsStr[15],
		     &parent->xdata.ServerSts[15],
		     parent->xdata.ServerStsName[15],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S17", 
		     parent->xdata.ServerStsStr[16],
		     &parent->xdata.ServerSts[16],
		     parent->xdata.ServerStsName[16],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S18", 
		     parent->xdata.ServerStsStr[17],
		     &parent->xdata.ServerSts[17],
		     parent->xdata.ServerStsName[17],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S19", 
		     parent->xdata.ServerStsStr[18],
		     &parent->xdata.ServerSts[18],
		     parent->xdata.ServerStsName[18],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Sys", "S20", 
		     parent->xdata.ServerStsStr[19],
		     &parent->xdata.ServerSts[19],
		     parent->xdata.ServerStsName[19],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A1", 
		     parent->xdata.ApplStsStr[0],
		     &parent->xdata.ApplSts[0],
		     parent->xdata.ApplStsName[0],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A2", 
		     parent->xdata.ApplStsStr[1],
		     &parent->xdata.ApplSts[1],
		     parent->xdata.ApplStsName[1],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A3", 
		     parent->xdata.ApplStsStr[2],
		     &parent->xdata.ApplSts[2],
		     parent->xdata.ApplStsName[2],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A4", 
		     parent->xdata.ApplStsStr[3],
		     &parent->xdata.ApplSts[3],
		     parent->xdata.ApplStsName[3],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A5", 
		     parent->xdata.ApplStsStr[4],
		     &parent->xdata.ApplSts[4],
		     parent->xdata.ApplStsName[4],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A6", 
		     parent->xdata.ApplStsStr[5],
		     &parent->xdata.ApplSts[5],
		     parent->xdata.ApplStsName[5],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A7", 
		     parent->xdata.ApplStsStr[6],
		     &parent->xdata.ApplSts[6],
		     parent->xdata.ApplStsName[6],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A8", 
		     parent->xdata.ApplStsStr[7],
		     &parent->xdata.ApplSts[7],
		     parent->xdata.ApplStsName[7],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A9", 
		     parent->xdata.ApplStsStr[8],
		     &parent->xdata.ApplSts[8],
		     parent->xdata.ApplStsName[8],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A10", 
		     parent->xdata.ApplStsStr[9],
		     &parent->xdata.ApplSts[9],
		     parent->xdata.ApplStsName[9],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A11", 
		     parent->xdata.ApplStsStr[10],
		     &parent->xdata.ApplSts[10],
		     parent->xdata.ApplStsName[10],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A12", 
		     parent->xdata.ApplStsStr[11],
		     &parent->xdata.ApplSts[11],
		     parent->xdata.ApplStsName[11],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A13", 
		     parent->xdata.ApplStsStr[12],
		     &parent->xdata.ApplSts[12],
		     parent->xdata.ApplStsName[12],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A14", 
		     parent->xdata.ApplStsStr[13],
		     &parent->xdata.ApplSts[13],
		     parent->xdata.ApplStsName[13],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A15", 
		     parent->xdata.ApplStsStr[14],
		     &parent->xdata.ApplSts[14],
		     parent->xdata.ApplStsName[14],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "A16", "A16", 
		     parent->xdata.ApplStsStr[15],
		     &parent->xdata.ApplSts[15],
		     parent->xdata.ApplStsName[15],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A17", 
		     parent->xdata.ApplStsStr[16],
		     &parent->xdata.ApplSts[16],
		     parent->xdata.ApplStsName[16],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A18", 
		     parent->xdata.ApplStsStr[17],
		     &parent->xdata.ApplSts[17],
		     parent->xdata.ApplStsName[17],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A19", 
		     parent->xdata.ApplStsStr[18],
		     &parent->xdata.ApplSts[18],
		     parent->xdata.ApplStsName[18],
		     this, node, flow_eDest_IntoLast);

    new ItemAttrSts( nodelistnav, "Appl", "A20", 
		     parent->xdata.ApplStsStr[19],
		     &parent->xdata.ApplSts[19],
		     parent->xdata.ApplStsName[19],
		     this, node, flow_eDest_IntoLast);

    brow_SetOpen( node, 1);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_openmap);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);

    statussrv_GetExtStatus( parent->name, &parent->xdata);
    nodelistnav->force_trace_scan();
  }
  return 1;
}

int ItemAttrSysSts::close( NodelistNav *nodelistnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( nodelistnav->brow->ctx);
    brow_CloseNode( nodelistnav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( nodelistnav->brow->ctx);
    brow_Redraw( nodelistnav->brow->ctx, node_y);
    parent->syssts_open = 0;
    memset( &parent->xdata, 0, sizeof(parent->xdata));
  }
  return 1;
}

ItemAttrSts::ItemAttrSts( NodelistNav *item_nodelistnav, const char *item_name, const char *attr, 
			  char *attr_value_p, 
			  pwr_tStatus *attr_status_p, char *attr_name_p, ItemAttrSysSts *attr_parent, 
			  brow_tNode dest, flow_eDest dest_code) :
  ItemBase( item_nodelistnav, item_name), value_p(attr_value_p), status_p(attr_status_p), 
  name_p(attr_name_p), first_scan(1), parent(attr_parent)
{

  type = nodelistnav_eItemType_AttrSts;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));
  memset( old_name, 0, sizeof(old_name));

  brow_CreateNode( nodelistnav->brow->ctx, item_name, nodelistnav->brow->nc_sts_attr,
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, nodelistnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}




