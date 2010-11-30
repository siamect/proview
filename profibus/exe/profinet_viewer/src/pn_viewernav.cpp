/** 
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

/* pn_viewernav.cpp -- Profinet viewer */

#ifdef PWRE_CONF_PNAK

#include "glow_std.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

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
#include "xnav_bitmap_morehelp8.h"
#include "xnav_bitmap_morehelp10.h"
#include "xnav_bitmap_morehelp12.h"
#include "xnav_bitmap_morehelp14.h"
#include "xnav_bitmap_morehelp16.h"
#include "xnav_bitmap_morehelp18.h"
#include "xnav_bitmap_morehelp20.h"
#include "xnav_bitmap_morehelp24.h"
#include "xnav_bitmap_attr8.h"
#include "xnav_bitmap_attr10.h"
#include "xnav_bitmap_attr12.h"
#include "xnav_bitmap_attr14.h"
#include "xnav_bitmap_attr16.h"
#include "xnav_bitmap_attr18.h"
#include "xnav_bitmap_attr20.h"
#include "xnav_bitmap_attr24.h"

#include "pn_viewernav.h"
#include "rt_pb_msg.h"

//
//  Free pixmaps
//
void PnViewerNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
  brow_FreeAnnotPixmap( ctx, pixmap_edit);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void PnViewerNavBrow::allocate_pixmaps()
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


  i = 0;
  pixmap_data[i].width =xnav_bitmap_morehelp8_width;
  pixmap_data[i].height =xnav_bitmap_morehelp8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp8_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp10_width;
  pixmap_data[i].height =xnav_bitmap_morehelp10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp10_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp12_width;
  pixmap_data[i].height =xnav_bitmap_morehelp12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp12_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp14_width;
  pixmap_data[i].height =xnav_bitmap_morehelp14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp14_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp16_width;
  pixmap_data[i].height =xnav_bitmap_morehelp16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp16_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp18_width;
  pixmap_data[i].height =xnav_bitmap_morehelp18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp18_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
  pixmap_data[i].width =xnav_bitmap_morehelp24_width;
  pixmap_data[i].height =xnav_bitmap_morehelp24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp24_bits;
  
  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_edit);
}

//
// Create nodeclasses
//
void PnViewerNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create device class

  brow_CreateNodeClass( ctx, "ViewDevice", 
		flow_eNodeGroup_Common, &nc_device);
  brow_AddAnnotPixmap( nc_device, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_device, 1.4, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_device, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_device, 20, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_device, 32, 0.6, 3,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_device, 0, 0, 35, 0.83, flow_eDrawType_LineGray, -1, 1);

  brow_CreateNodeClass( ctx, "ViewAttr", 
		flow_eNodeGroup_Common, &nc_attr);
  brow_AddAnnotPixmap( nc_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr, 1, 1.0, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr, 1.6, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 10, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 15, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 30, 0.6, 3,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_attr, 0, 0, 35, 0.83, flow_eDrawType_LineGray, -1, 1);

}

void PnViewerNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, viewernav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	PnViewerNav::brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int PnViewerNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  PnViewerNav *viewernav = (PnViewerNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  viewernav->brow = new PnViewerNavBrow( ctx, (void *)viewernav);

  viewernav->brow->brow_setup();
  viewernav->brow->create_nodeclasses();

  return 1;
}

PnViewerNav::PnViewerNav( void *l_parent_ctx) :
  parent_ctx(l_parent_ctx), change_value_cb(0)
{
}


//
//  Delete ev
//
PnViewerNav::~PnViewerNav()
{
}

PnViewerNavBrow::~PnViewerNavBrow()
{
  free_pixmaps();
}


//
//  Zoom
//
void PnViewerNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void PnViewerNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

//
//  Get selected device
//
int PnViewerNav::get_selected_device( ItemDevice **device)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemDevice   	*item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( node_count != 1)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  switch ( item->type) {
  case viewitem_eItemType_Device:
    *device = item;
    break;
  default:
    return 0;
  }
  return 1;
}

void PnViewerNav::message( int severity, const char *msg)
{
  if ( message_cb)
    (message_cb) (parent_ctx, severity, msg);
}


//
// Callbacks from brow
//
int PnViewerNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  PnViewerNav		*viewernav;
  ItemDevice 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &viewernav);
  viewernav->message( ' ',"");
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( viewernav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( viewernav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( viewernav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( viewernav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( viewernav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( viewernav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( viewernav->brow->ctx, object);
      if ( !brow_IsVisible( viewernav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( viewernav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( viewernav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( viewernav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( viewernav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( viewernav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( viewernav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( viewernav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( viewernav->brow->ctx, object);
      if ( !brow_IsVisible( viewernav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( viewernav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;
      int		sts;

      brow_GetSelectedNodes( viewernav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type) {
      case viewitem_eItemType_Device:
	sts = item->open_children( viewernav);
	break;
      case viewitem_eItemType_DeviceAttr:
	(viewernav->change_value_cb)( viewernav->parent_ctx);
	break;
      default: ;
      }
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( viewernav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else {
        // Close parent
        sts = brow_GetParent( viewernav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
          return 1;
      }
      brow_GetUserData( object, (void **)&item);
      item->close( viewernav);
      brow_SelectClear( viewernav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( viewernav->brow->ctx, object);
      if ( !brow_IsVisible( viewernav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( viewernav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( viewernav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
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
            sts = PnViewerNav::brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( viewernav->brow->ctx, event->object.object))
          {
            brow_SelectClear( viewernav->brow->ctx);
          }
          else
          {
            brow_SelectClear( viewernav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( viewernav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( viewernav->brow->ctx);
      }
      break;
    case flow_eEvent_Key_PageDown: {
      brow_Page( viewernav->brow->ctx, 0.95);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( viewernav->brow->ctx, -0.95);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( viewernav->brow->ctx, 0.10);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( viewernav->brow->ctx, -0.10);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      break;
    default:
      ;
  }
  return 1;
}

//
// Set attribute value
//
int PnViewerNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemDeviceAttr *base_item;
  int		sts;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
  case viewitem_eItemType_DeviceAttr: {
    ItemDeviceAttr	*item;

    item = (ItemDeviceAttr *)base_item;

    if ( strcmp( item->name, "IP Address") == 0) {
      unsigned char address[4];
      int num;
      
      num = sscanf( value_str, "%hhu.%hhu.%hhu.%hhu", 
		    &address[3], &address[2], &address[1], &address[0]); 
      if ( num != 4)
	return PB__SYNTAX;

      // Change ip address in parent node
      brow_tNode parent;
      ItemDevice *pitem;

      sts = brow_GetParent( brow->ctx, item->node, &parent);
      if ( EVEN(sts)) return 0;
      
      brow_GetUserData( parent, (void **)&pitem);

      memcpy( pitem->ipaddress, address, sizeof(pitem->ipaddress));

      strcpy( (char *)item->p, value_str);
      brow_SetAnnotation( item->node, 1, (char *)item->p, strlen((char *)item->p));
    }
    else if ( strcmp( item->name, "DeviceName") == 0) {

      strcpy( (char *)item->p, value_str);
      brow_SetAnnotation( item->node, 1, (char *)item->p, strlen((char *)item->p));
      
      // Change parent annotation also
      brow_tNode parent;
      ItemDevice *pitem;
      
      sts = brow_GetParent( brow->ctx, item->node, &parent);
      if ( EVEN(sts)) return 0;
      
      brow_GetUserData( parent, (void **)&pitem);
      brow_SetAnnotation( pitem->node, 0, (char *)item->p, strlen((char *)item->p));
    }
    break;
  }
  default:
    ;
  }
  return 1;
}

//
// Check that the current selected item is valid for change
//
int PnViewerNav::check_attr_value()
{
  brow_tNode	*node_list;
  int		node_count;
  ItemDeviceAttr *base_item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
  case viewitem_eItemType_DeviceAttr:
    if (( strcmp( base_item->name, "DeviceName") == 0) ||
	( strcmp( base_item->name, "IP Address") == 0))
      return 1;
    else
      return PB__ATTRNOEDIT;
    break;
  default:
    return PB__ATTRNOEDIT;
  }
}

void PnViewerNav::set( vector<PnDevice>& dev_vect)
{
  brow_SetNodraw( brow->ctx);

  brow_DeleteAll( brow->ctx);

  for ( unsigned int i = 0; i < dev_vect.size(); i++) {
    new ItemDevice( this, "", dev_vect[i].ipaddress, dev_vect[i].macaddress, 
		    dev_vect[i].devname, dev_vect[i].vendorid, dev_vect[i].deviceid,
		    0, flow_eDest_IntoLast);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

void PnViewerNav::vendorid_to_str( unsigned int vendorid, char *vendorid_str, int size)
{
  pwr_tFileName fname;
  dcli_translate_filename( fname, "$pwr_exe/profinet_vendorid.dat");
  ifstream fp;
  char line[1024];
  int found = 0;
  unsigned int id;
  int num;
  char *s;

  fp.open( fname);
  if ( fp) {

    while ( fp.getline( line, sizeof(line))) {
      num = sscanf( line, "%u", &id);
      if ( num != 1)
	continue;

      if ( id == vendorid) {
	s = strchr( line, ' ');
	strncpy( vendorid_str, s+1, size);
	vendorid_str[size-1] = 0;
	found = 1;
	break;
      }
    }
    fp.close();
  }  

  if ( !found)
    sprintf( vendorid_str, "%d", vendorid);
}

void PnViewerNav::get_device_info( unsigned int vendorid, unsigned int deviceid,
				   char *info, int info_size, char *family, int family_size)
{
  pwr_tFileName fname;
  dcli_translate_filename( fname, "$pwr_exe/profinet_deviceid.dat");
  ifstream fp;
  char line[1024];
  int found = 0;
  unsigned int vid;
  unsigned int did;
  int num;

  fp.open( fname);
  if ( fp) {

    while ( fp.getline( line, sizeof(line))) {
      if ( strstr( line, "# Processing file:")) {
	if ( !fp.getline( line, sizeof(line)))
	  break;	

	num = sscanf( line, "%u %u", &vid, &did);
	if ( num != 2)
	  continue;

	if ( !( vid == vendorid && did == deviceid))
	  continue;

	if ( !fp.getline( line, sizeof(line)))
	  break;	
	
	strncpy( family, line, family_size);
	family[family_size-1] = 0;

	if ( !fp.getline( line, sizeof(line)))
	  break;	

	strncpy( info, line, info_size);
	info[info_size-1] = 0;
	found = 1;
	break;
      }
    }
    fp.close();
  }  

}

ItemDevice::ItemDevice( PnViewerNav *item_viewernav, const char *item_name,
			unsigned char *item_ipaddress, unsigned char *item_macaddress, 
			char *item_devname, int vendorid, int deviceid,
			brow_tNode dest, flow_eDest dest_code):
  viewernav(item_viewernav)
{
  type = viewitem_eItemType_Device;

  strcpy( infotext, "");
  strcpy( family, "");
  strncpy( devname, item_devname, sizeof(devname));
  memcpy( ipaddress, item_ipaddress, sizeof(ipaddress));
  memcpy( macaddress, item_macaddress, sizeof(macaddress));
  sprintf( ipaddress_str, "%hhu.%hhu.%hhu.%hhu", 
	   item_ipaddress[3], item_ipaddress[2], item_ipaddress[1], item_ipaddress[0]);
  sprintf( macaddress_str, "%02hhX-%02hhX-%02hhX-%02hhX-%02hhX-%02hhX", 
	   item_macaddress[5], item_macaddress[4], item_macaddress[3], 
	   item_macaddress[2], item_macaddress[1], item_macaddress[0]);
  sprintf( deviceid_str, "%d", deviceid);
  viewernav->vendorid_to_str( vendorid, vendorid_str, sizeof(vendorid_str));
  viewernav->get_device_info( vendorid, deviceid, infotext, sizeof(infotext), family,
			      sizeof(family));

  brow_CreateNode( viewernav->brow->ctx, (char *)"Device", viewernav->brow->nc_device,
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotation( node, 0, devname, strlen(devname));
  brow_SetAnnotation( node, 1, vendorid_str, strlen(vendorid_str));
  brow_SetAnnotation( node, 2, family, strlen(family));
  brow_SetAnnotation( node, 3, infotext, strlen(infotext));

  brow_SetAnnotPixmap( node, 0, viewernav->brow->pixmap_map);
           
}

int ItemDevice::open_children( PnViewerNav *viewernav)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( viewernav->brow->ctx);
    brow_CloseNode( viewernav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, viewernav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( viewernav->brow->ctx);
    brow_Redraw( viewernav->brow->ctx, node_y);
  }
  else
  {
    // Create some children
    brow_SetNodraw( viewernav->brow->ctx);

    new ItemDeviceAttr( viewernav, "DeviceName", pwr_eType_String, devname, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "IP Address", pwr_eType_String, ipaddress_str, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "MAC Address", pwr_eType_String, macaddress_str, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "Vendor Id", pwr_eType_String, vendorid_str, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "Device Id", pwr_eType_String, deviceid_str, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "Family", pwr_eType_String, family, node, 
			flow_eDest_IntoLast);
    new ItemDeviceAttr( viewernav, "InfoText", pwr_eType_String, infotext, node, 
			flow_eDest_IntoLast);

    brow_SetOpen( node, 1);
    brow_SetAnnotPixmap( node, 0, viewernav->brow->pixmap_openmap);
    brow_ResetNodraw( viewernav->brow->ctx);
    brow_Redraw( viewernav->brow->ctx, node_y);
  }
  return 1;
}

void ItemDevice::close( PnViewerNav *viewernav)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node)) {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( viewernav->brow->ctx);
    brow_CloseNode( viewernav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, viewernav->brow->pixmap_map);
    brow_ResetOpen( node, 1);
    brow_ResetNodraw( viewernav->brow->ctx);
    brow_Redraw( viewernav->brow->ctx, node_y);
  }
}

ItemDeviceAttr::ItemDeviceAttr( PnViewerNav *viewernav, const char *item_name, pwr_eType item_attr_type,
				void *item_p, brow_tNode dest, flow_eDest dest_code) :
  attr_type(item_attr_type), p(item_p)
{
  char value[80];

  type = viewitem_eItemType_DeviceAttr;
  strcpy( name, item_name);
  

  brow_CreateNode( viewernav->brow->ctx, (char *)"DeviceAttr", viewernav->brow->nc_attr,
		dest, dest_code, (void *) this, 1, &node);

  strncpy( value, (char *)p, sizeof(value));

  brow_SetAnnotation( node, 0, name, strlen(name));
  brow_SetAnnotation( node, 1, value, strlen(value));

  brow_SetAnnotPixmap( node, 0, viewernav->brow->pixmap_attr);  
  if ( strcmp( item_name, "DeviceName") == 0 ||
       strcmp( item_name, "IP Address") == 0)
    brow_SetAnnotPixmap( node, 1, viewernav->brow->pixmap_edit);  
}

#endif
