/** 
 * Proview   $Id: co_xhelpnav.cpp,v 1.15 2008-11-10 08:01:26 claes Exp $
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

/* co_xhelpnav.cpp -- helptext navigator */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_utils.h"
#include "co_xhelpnav.h"

#include "xnav_bitmap_morehelp8.h"
#include "xnav_bitmap_morehelp10.h"
#include "xnav_bitmap_morehelp12.h"
#include "xnav_bitmap_morehelp14.h"
#include "xnav_bitmap_morehelp16.h"
#include "xnav_bitmap_morehelp18.h"
#include "xnav_bitmap_morehelp20.h"
#include "xnav_bitmap_morehelp24.h"
#include "xnav_bitmap_closehelp8.h"
#include "xnav_bitmap_closehelp10.h"
#include "xnav_bitmap_closehelp12.h"
#include "xnav_bitmap_closehelp14.h"
#include "xnav_bitmap_closehelp16.h"
#include "xnav_bitmap_closehelp18.h"
#include "xnav_bitmap_closehelp20.h"
#include "xnav_bitmap_closehelp24.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

static int help_cmp_items( const void *node1, const void *node2);

static void xhelpnav_open_URL( CoXHelpNav *xhelpnav, char *url)
{
  if ( xhelpnav->open_URL_cb)
    (xhelpnav->open_URL_cb)( xhelpnav->parent_ctx, url);
}

//
//  Free pixmaps
//
void CoXHelpNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_morehelp);
  brow_FreeAnnotPixmap( ctx, pixmap_closehelp);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void CoXHelpNavBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;
	
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

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_morehelp);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_closehelp8_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp8_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp10_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp10_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp12_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp12_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp14_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp14_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp16_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp16_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp18_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp18_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp24_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_closehelp);
}

//
// Create nodeclasses
//
void CoXHelpNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "NavigatorDefault", 
		flow_eNodeGroup_Common, &nc_object);
  brow_AddFrame( nc_object, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_object, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_object, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_object, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_object, 7, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_object, 11, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create text

  brow_CreateNodeClass( ctx, "NavigatorText", 
		flow_eNodeGroup_Common, &nc_text);
  brow_AddFrame( nc_text, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_text, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_text, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_text, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_text, 7, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_text, 11, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);

  // Create Header

  brow_CreateNodeClass( ctx, "NavigatorHead", 
		flow_eNodeGroup_Common, &nc_header);
  brow_AddFrame( nc_header, 0, 0, 20, 1.4, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_header, 0, 0.2, 0.4, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_header, 2, 1.0, 0,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_header, 8, 1.0, 1,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		1);

  // Create Header2

  brow_CreateNodeClass( ctx, "NavigatorHeadLarge", 
		flow_eNodeGroup_Common, &nc_headerlarge);
  brow_AddFrame( nc_headerlarge, 0, 0, 20, 2.0, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_headerlarge, 0, 0.2, 0.4, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_headerlarge, 2, 1.0, 0,
		flow_eDrawType_TextHelveticaBold, 6, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_headerlarge, 8, 1.0, 1,
		flow_eDrawType_TextHelveticaBold, 6, flow_eAnnotType_OneLine, 
		1);

  // Create Horizontal line

  brow_CreateNodeClass( ctx, "NavigatorLine", 
		flow_eNodeGroup_Common, &nc_line);
  // brow_AddFrame( nc_line, 0, 0, 20, 0.9, flow_eDrawType_Line, -1, 1);
  brow_AddFrame( nc_line, 0, 0, 20, 0.15, flow_eDrawType_LineGray, 2, 1);

}

void CoXHelpNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, userdata);
  brow_SetWhiteBackground( ctx);
}


void CoXHelpNav::clear()
{
  brow_DeleteAll( brow->ctx);
}



//
// Create the navigator widget
//
CoXHelpNav::CoXHelpNav(
	void *xn_parent_ctx,
	char *xn_name,
	xhelp_eUtility xn_utility,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx),
	brow_cnt(0), closing_down(0), displayed(0), utility(xn_utility),
	search_node(0), search_strict(false), open_URL_cb(0)
{
  strcpy( name, xn_name);
  strcpy( search_str, "");
  *status = 1;
}

//
//  Delete a nav context
//
CoXHelpNav::~CoXHelpNav()
{
}

//
//  Print
//
void CoXHelpNav::print( char *filename)
{
  brow_Print( brow->ctx, filename);
}


//
//  Zoom
//
void CoXHelpNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void CoXHelpNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

//
//  Get zoom
//
void CoXHelpNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

//
// Callbacks from brow
//
static int xhelpnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  CoXHelpNav		*xhelpnav;
  HItem 		*item;

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &xhelpnav);
  if ( xhelpnav->closing_down)
    return 1;

  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( xhelpnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( xhelpnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( xhelpnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( xhelpnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts))
	    return 1;
        }
      }
      brow_SelectClear( xhelpnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xhelpnav->brow->ctx, object);
      if ( !brow_IsVisible( xhelpnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xhelpnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( xhelpnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( xhelpnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( xhelpnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( xhelpnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts))
	    return 1;
        }
      }
      brow_SelectClear( xhelpnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xhelpnav->brow->ctx, object);
      if ( !brow_IsVisible( xhelpnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xhelpnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_ShiftDown:
    case flow_eEvent_Key_PageDown: {
      brow_Page( xhelpnav->brow->ctx, 0.95);
      break;
    }
    case flow_eEvent_Key_ShiftUp:
    case flow_eEvent_Key_PageUp: {
      brow_Page( xhelpnav->brow->ctx, -0.95);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( xhelpnav->brow->ctx, 0.10);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( xhelpnav->brow->ctx, -0.10);
      break;
    }
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      brow_tNode	first;
      int		node_count;
      pwr_tStatus	sts;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);

      sts = brow_GetFirst( xhelpnav->brow->ctx, &first);
      if ( ODD(sts) && first == node_list[0]) {
	xhelpnav->next_topic();
	free( node_list);
	break;
      }

      item->doubleclick_action( xhelpnav->brow, xhelpnav, 0, 0);
      break;
    }
    case flow_eEvent_Key_ShiftRight:
    {
      xhelpnav->next_topic();
      break;
    }
    case flow_eEvent_Key_ShiftLeft:
    {
      xhelpnav->previous_topic();
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
	xhelpnav->brow_push();
        return 1;
      }

      if ( brow_IsOpen( node_list[0])) {
        // Close this node
        object = node_list[0];
        free( node_list);
      }
      else
      {
        // Close parent
        sts = brow_GetParent( xhelpnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
	  xhelpnav->brow_push();
          return 1;
        }
      }
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( xhelpnav->brow->ctx);
      break;
    case flow_eEvent_ObjectDeleted:
      brow_GetUserData( event->object.object, (void **)&item);
      delete item;
      break;
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          item->doubleclick_action( xhelpnav->brow, xhelpnav,
			event->object.x, event->object.y);
          break;
        default:
          ;
      }
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
            sts = xhelpnav_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( xhelpnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( xhelpnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( xhelpnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( xhelpnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( xhelpnav->brow->ctx);
      }
      break;
    case flow_eEvent_Resized:
      brow_Redraw( xhelpnav->brow->ctx, 0);
      break;
    default:
      ;
  }
  return 1;
}

int CoXHelpNav::brow_pop()
{
  BrowCtx *secondary_ctx;

  if ( brow_cnt >= XHELPNAV_BROW_MAX)
    return 0;
  brow_CreateSecondaryCtx( brow->ctx, &secondary_ctx,
        CoXHelpNav::init_brow_cb, (void *)this, flow_eCtxType_Brow);

  brow_ChangeCtx( brow->ctx, brow_stack[brow_cnt]->ctx);
  *brow = *brow_stack[brow_cnt];
  brow_cnt++;
  return 1;
}

int CoXHelpNav::brow_push()
{
  if ( brow_cnt == 1)
     return 0;

  brow_cnt--;
  brow_ChangeCtx( brow_stack[brow_cnt]->ctx, 
		brow_stack[brow_cnt-1]->ctx);
  *brow = *brow_stack[brow_cnt-1];
  delete brow_stack[brow_cnt];

  return 1;
}

int CoXHelpNav::brow_push_all()
{
  while( brow_push())
    ;
  return 1;
}

void  CoXHelpNav::enable_events( CoXHelpNavBrow *brow)
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF1, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF2, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftLeft, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftUp, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftDown, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Resized, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int CoXHelpNav::init_brow_base_cb( FlowCtx *fctx, void *client_data)
{
  CoXHelpNav *xhelpnav = (CoXHelpNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  xhelpnav->brow = new CoXHelpNavBrow( ctx, (void *)xhelpnav);
  xhelpnav->brow_stack[0] = new CoXHelpNavBrow( ctx, (void *)xhelpnav);
  xhelpnav->brow_cnt++;

  xhelpnav->brow->brow_setup();
  xhelpnav->brow->create_nodeclasses();

  memcpy( xhelpnav->brow_stack[0], xhelpnav->brow, sizeof( *xhelpnav->brow));
  xhelpnav->enable_events( xhelpnav->brow);

  return 1;
}

int CoXHelpNav::init_brow_cb( BrowCtx *ctx, void *client_data)
{
  CoXHelpNav *xhelpnav = (CoXHelpNav *) client_data;

  xhelpnav->brow_stack[xhelpnav->brow_cnt] = new CoXHelpNavBrow( ctx, (void *)xhelpnav);

  xhelpnav->brow_stack[xhelpnav->brow_cnt]->brow_setup();
  xhelpnav->brow_stack[xhelpnav->brow_cnt]->create_nodeclasses();
  xhelpnav->enable_events( xhelpnav->brow_stack[xhelpnav->brow_cnt]);

  return 1;
}

pwr_tStatus CoXHelpNav::search( char *str, bool strict)
{
  search_node = 0;
  search_strict = strict;

  strncpy( search_str, str, sizeof(search_str));
  if ( !strict)
    cdh_ToUpper( search_str, search_str);

  return search_exec( false);
}

#define XHELP__SEARCHNOTSTARTED 2;
#define XHELP__SUCCESS 1;
#define XHELP__SEARCHNOTFOUND 4;

pwr_tStatus CoXHelpNav::search_next()
{
  if ( strcmp( search_str, "") == 0)
    return XHELP__SEARCHNOTSTARTED;

  return search_exec( false);
}

pwr_tStatus CoXHelpNav::search_next_reverse()
{
  if ( strcmp( search_str, "") == 0)
    return XHELP__SEARCHNOTSTARTED;

  return search_exec( true);
}

pwr_tStatus CoXHelpNav::search_exec( bool reverse)
{
  brow_tObject 	*object_list;
  int		object_cnt;
  HItem		*item;
  bool          active = false;
  bool		hit = false;
  int		idx;

  if ( !search_node)
    active = true;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( int i = 0; i < object_cnt; i++) {
    if ( reverse)
      idx = object_cnt - 1 - i;
    else
      idx = i;

    if ( !active) {
      if ( search_node == object_list[idx])
	active = true;
      continue;
    }
    brow_GetUserData( object_list[idx], (void **)&item);
    hit = item->search( search_str, search_strict);
    if ( hit) {
      // Select and center node
      brow_SelectClear( brow->ctx);
      brow_SetInverse( object_list[idx], 1);
      brow_SelectInsert( brow->ctx, object_list[idx]);
      if ( !brow_IsVisible( brow->ctx, object_list[idx], flow_eVisible_Full))
        brow_CenterObject( brow->ctx, object_list[idx], 0.25);
      search_node = object_list[idx];
      return XHELP__SUCCESS;
    }
  }
  // Not found
  search_node = 0;
  return XHELP__SEARCHNOTFOUND;
}

HItemHeader::HItemHeader( CoXHelpNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_Header;
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

HItemHelpLine::HItemHelpLine( CoXHelpNavBrow *brow, const char *item_name, 
			      brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_HelpLine;
  brow_CreateNode( brow->ctx, "header", brow->nc_line,
		dest, dest_code, (void *)this, 1, &node);
}

HItemHelpImage::HItemHelpImage( CoXHelpNavBrow *brow, const char *item_name, brow_tNodeClass nc,
				const char *item_link, const char *item_bookmark, 
				const char *item_file_name, navh_eHelpFile help_file_type, 
				brow_tNode dest, flow_eDest dest_code)
  : file_type(help_file_type)
{
  type = xhelpnav_eHItemType_HelpImage;
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");

  brow_CreateNode( brow->ctx, "image", nc,
		dest, dest_code, (void *)this, 1, &node);
  if ( link[0] != 0)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

int HItemHelpImage::doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y)
{
  int sts;

  if ( strcmp( link, "") != 0)
  {
    if ( strncmp( link, "$web:", 5) == 0) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, &link[5]);
    }
    else if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1, true);
        if (EVEN(sts))
          sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1, true);
      }
      else
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1, true);
    }
  }
  return 1;
}

HItemHeaderLarge::HItemHeaderLarge( CoXHelpNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_HeaderLarge;
  brow_CreateNode( brow->ctx, "header", brow->nc_headerlarge,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

bool HItemHeaderLarge::search( char *str, bool strict)
{
  char text[256];

  brow_GetAnnotation( node, 0, text, sizeof(text));
  if ( text[0] == 0)
    return false;
  if ( !strict)
    cdh_ToUpper( text, text);
  if ( strstr( text, str) != 0)
    return true;
  return false;
}

HItemHelpHeader::HItemHelpHeader( CoXHelpNavBrow *brow, const char *item_name, const char *title, 
				  bool base, brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_HelpHeader;
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
  if ( !base)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_closehelp);
}

bool HItemHelpHeader::search( char *str, bool strict)
{
  char text[256];

  brow_GetAnnotation( node, 0, text, sizeof(text));
  if ( text[0] == 0)
    return false;
  if ( !strict)
    cdh_ToUpper( text, text);
  if ( strstr( text, str) != 0)
    return true;
  return false;
}

int HItemHelpHeader::doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y)
{
  if ( xhelpnav)
    xhelpnav->brow_push();
  return 1;
}

HItemHelp::HItemHelp( CoXHelpNavBrow *brow, const char *item_name, const char *text, const char *text2, 
		      const char *text3, const char *item_link, const char *item_bookmark, 
		      const char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
		      brow_tNode dest, flow_eDest dest_code) :
	file_type(help_file_type), index(help_index)
{
  type = xhelpnav_eHItemType_Help;
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_text,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

bool HItemHelp::search( char *str, bool strict)
{
  char text[256];

  for ( int i = 0; i < 3; i++) {
    brow_GetAnnotation( node, i, text, sizeof(text));
    if ( text[0] == 0)
      continue;
    if ( !strict)
      cdh_ToUpper( text, text);
    if ( strstr( text, str) != 0)
      return true;
  }
  return false;
}

int HItemHelp::doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xhelpnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( strncmp( link, "$web:", 5) == 0) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, &link[5]);
    }
    else if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1, true);
        if (EVEN(sts))
          sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1, true);
      }
      else
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1, true);
    }
  }
  return 1;
}

HItemHelpBold::HItemHelpBold( CoXHelpNavBrow *brow, const char *item_name, const char *text, 
			      const char *text2, const char *text3, const char *item_link, 
			      const char *item_bookmark, const char *item_file_name, 
			      navh_eHelpFile help_file_type, int help_index, brow_tNode dest, 
			      flow_eDest dest_code) :
  file_type(help_file_type), index(help_index)
{
  type = xhelpnav_eHItemType_HelpBold;
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

bool HItemHelpBold::search( char *str, bool strict)
{
  char text[256];

  for ( int i = 0; i < 3; i++) {
    brow_GetAnnotation( node, i, text, sizeof(text));
  if ( text[0] == 0)
    continue;
    if ( !strict)
      cdh_ToUpper( text, text);
    if ( strstr( text, str) != 0)
      return true;
  }
  return false;
}

int HItemHelpBold::doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xhelpnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( strncmp( link, "$web:", 5) == 0) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, &link[5]);
    }
    else if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
      // Open the url
      xhelpnav_open_URL( xhelpnav, link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1, true);
        if (EVEN(sts))
          sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1, true);
      }
      else
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1, true);
    }
  }
  return 1;
}


static void trim( char *str)
{
  if ( !str)
    return;

  unsigned char *s = (unsigned char *)str;
  while ( *s) {
    if ( *s < ' ')
      *s = ' ';
    s++;
  }
}

/*************************************************************************
*
* Name:		help()
*
**************************************************************************/

static void *xhelpnav_help_insert_cb( void *ctx, navh_eItemType item_type, const char *text1,
		      const char *text2, const char *text3, const char *link, 
		      const char *bookmark, const char *file_name,
		      navh_eHelpFile file_type, int help_index, const char *bm)
{
  CoXHelpNav *xhelpnav = (CoXHelpNav *)ctx;
  char *llink = 0;
  char *t1 = 0;
  char *t2 = 0;
  char *t3 = 0;

  if ( link && strncmp( link, "$class:", 7) == 0)
    llink = (char *)link + 7;
  else if ( link)
    llink = (char *)link;

  if ( xhelpnav->init_help) {
    xhelpnav->brow_pop();
    brow_SetNodraw( xhelpnav->brow->ctx);
    xhelpnav->init_help = 0;
  }

  switch ( item_type) {
    case navh_eItemType_Help:
    case navh_eItemType_HelpCode:
    {      
      if ( text1) {
	t1 = (char *)calloc( 1, strlen(text1)+1);
	strcpy( t1, text1);
	trim( t1);
      }
      if ( text2) {
	t2 = (char *)calloc( 1, strlen(text2)+1);
	strcpy( t2, text2);
	trim( t2);
      }
      if ( text3) {
	t3 = (char *)calloc( 1, strlen(text3)+1);
	strcpy( t3, text3);
	trim( t3);
      }
      HItemHelp *item = new HItemHelp( xhelpnav->brow, "help", t1, t2, t3,
	     llink, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      free( t1);
      free( t2);
      free( t3);
      return item->node;
    }
    case navh_eItemType_HelpBold:
    {
      if ( text1) {
	t1 = (char *)calloc( 1, strlen(text1)+1);
	strcpy( t1, text1);
	trim( t1);
      }
      if ( text2) {
	t2 = (char *)calloc( 1, strlen(text2)+1);
	strcpy( t2, text2);
	trim( t2);
      }
      if ( text3) {
	t3 = (char *)calloc( 1, strlen(text3)+1);
	strcpy( t3, text3);
	trim( t3);
      }
      HItemHelpBold *item = new HItemHelpBold( xhelpnav->brow, "help", t1, t2,
	     t3, llink, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      free( t1);
      free( t2);
      free( t3);
      return item->node;
    }
    case navh_eItemType_HelpHeader:
    {      
      if ( text1) {
	t1 = (char *)calloc( 1, strlen(text1)+1);
	strcpy( t1, text1);
	trim( t1);
      }
      HItemHelpHeader *item = new HItemHelpHeader( xhelpnav->brow, "help", t1, xhelpnav->brow_cnt == 1,
	     NULL, flow_eDest_IntoLast);
      free( t1);
      return item->node;
    }
     case navh_eItemType_Header:
    {      
      if ( text1) {
	t1 = (char *)calloc( 1, strlen(text1)+1);
	strcpy( t1, text1);
	trim( t1);
      }
      HItemHeader *item = new HItemHeader( xhelpnav->brow, "help", t1,
	     NULL, flow_eDest_IntoLast);
      free( t1);
      return item->node;
    }
    case navh_eItemType_HeaderLarge:
    {      
      if ( text1) {
	t1 = (char *)calloc( 1, strlen(text1)+1);
	strcpy( t1, text1);
	trim( t1);
      }
      HItemHeaderLarge *item = new HItemHeaderLarge( xhelpnav->brow, "help", t1,
	     NULL, flow_eDest_IntoLast);
      free( t1);
      return item->node;
    }
    case navh_eItemType_HorizontalLine:
    {
      HItemHelpLine *item = new HItemHelpLine( xhelpnav->brow, "help",
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_Image:
    {
      brow_tNodeClass nc;

      brow_CreateNodeClass( xhelpnav->brow->ctx, "Image", flow_eNodeGroup_Common, &nc);
      brow_AddFrame( nc, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
      brow_AddAnnotPixmap( nc, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
      brow_AddImage( nc, text1, 2, 0);
      HItemHelpImage *item = new HItemHelpImage( xhelpnav->brow, "help", nc,
	     llink, bookmark, file_name, file_type, NULL, flow_eDest_IntoLast);
      return item->node;
    }
    default:
      return 0;
  }
}

int	CoXHelpNav::help( const char *help_key, const char *help_bookmark, 
			  navh_eHelpFile file_type, const char *file_name, int pop, bool strict)
{
  int sts;
  brow_tNode bookmark_node;
  brow_tNode prev, first;
  NavHelp *navhelp;

  switch ( utility) {
  case xhelp_eUtility_Xtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseXtt, xhelp_cFile_Project);
    break;
  case xhelp_eUtility_Wtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseWtt, xhelp_cFile_Project);
    break;
  default:
    return 0;
  }
  navhelp->insert_cb = xhelpnav_help_insert_cb;


  if ( pop)
    init_help = 1;
  else {
    init_help = 0;
    brow_SetNodraw( brow->ctx);
  }
  sts = navhelp->help( help_key, help_bookmark, file_type, 
		       file_name, &bookmark_node, strict);
  if ( EVEN(sts)) {
    if ( !pop || (pop && !init_help)) {
      brow_push();
      brow_ResetNodraw( brow->ctx);
    }
    return sts;
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  if ( bookmark_node) {
    brow_CenterObject( brow->ctx, bookmark_node, 0.0);
    sts = brow_GetPrevious( brow->ctx, bookmark_node, &prev);
    if ( ODD(sts))
    {
      brow_SelectClear( brow->ctx);
      brow_SetInverse( prev, 1);
      brow_SelectInsert( brow->ctx, prev);
    }
  }
  else {
    sts = brow_GetFirst( brow->ctx, &first);
    if ( ODD(sts))
      brow_CenterObject( brow->ctx, first, 0.0);
  }
  strncpy( brow_stack[brow_cnt-1]->current_key, help_key, sizeof(brow_stack[brow_cnt-1]->current_key));
  if ( file_name)
    strncpy( brow_stack[brow_cnt-1]->current_filename, file_name, sizeof(brow_stack[brow_cnt-1]->current_filename));
  else
    strcpy( brow_stack[brow_cnt-1]->current_filename, "");
  brow_stack[brow_cnt-1]->current_filetype = file_type;

  delete navhelp;

  return 1;
}

int CoXHelpNav::back()
{
  brow_push();
  return 1;
}

int	CoXHelpNav::next_topic()
{
  int sts;
  NavHelp *navhelp;
  char next_key[200];
  navh_eHelpFile current_filetype;
  char 		current_key[200];
  pwr_tFileName current_filename;

  current_filetype = brow_stack[brow_cnt-1]->current_filetype;
  strncpy( current_key, brow_stack[brow_cnt-1]->current_key, sizeof(current_key));
  strncpy( current_filename, brow_stack[brow_cnt-1]->current_filename, sizeof(current_filename));
  
  switch ( utility) {
  case xhelp_eUtility_Xtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseXtt, xhelp_cFile_Project);
    break;
  case xhelp_eUtility_Wtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseWtt, xhelp_cFile_Project);
    break;
  default:
    return 0;
  }

  sts = navhelp->get_next_key( current_key, current_filetype, 
			       current_filename[0] == 0 ? 0 : current_filename, 0, next_key);
  delete navhelp;
  if ( ODD(sts)) {
    brow_push();
    sts = help( next_key, 0, current_filetype, 
		current_filename[0] == 0 ? 0 : current_filename, 1, 0);
  }
  return sts;
}

int	CoXHelpNav::previous_topic()
{
  int 		sts;
  NavHelp 	*navhelp;
  char 		prev_key[200];
  navh_eHelpFile current_filetype;
  char 		current_key[200];
  pwr_tFileName current_filename;

  current_filetype = brow_stack[brow_cnt-1]->current_filetype;
  strncpy( current_key, brow_stack[brow_cnt-1]->current_key, sizeof(current_key));
  strncpy( current_filename, brow_stack[brow_cnt-1]->current_filename, sizeof(current_filename));
  
  switch ( utility) {
  case xhelp_eUtility_Xtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseXtt, xhelp_cFile_Project);
    break;
  case xhelp_eUtility_Wtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseWtt, xhelp_cFile_Project);
    break;
  default:
    return 0;
  }

  sts = navhelp->get_previous_key( current_key, current_filetype, 
				   current_filename[0] == 0 ? 0 : current_filename, 0, prev_key);
  delete navhelp;
  if ( ODD(sts)) {
    brow_push();
    sts = help( prev_key, 0, current_filetype, current_filename[0] == 0 ? 0 : current_filename, 1, 0);
  }
  return sts;
}

/*************************************************************************
*
* Name:		help_index()
*
**************************************************************************/

int	CoXHelpNav::help_index( navh_eHelpFile file_type, const char *file_name, int pop)
{
  int sts;
  brow_tObject 	*object_list;
  int		object_cnt;
  NavHelp *navhelp;

  switch ( utility) {
  case xhelp_eUtility_Xtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseXtt, xhelp_cFile_Project);
    break;
  case xhelp_eUtility_Wtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseWtt, xhelp_cFile_Project);
    break;
  default:
    return 0;
  }
  navhelp->insert_cb = xhelpnav_help_insert_cb;

  if (pop)
    brow_pop();
  brow_SetNodraw( brow->ctx);
  new HItemHelpHeader( brow, "help_index", "Index", brow_cnt == 1, NULL, flow_eDest_IntoLast);
  new HItemHelp( brow, "help_index", "",  "", "", "", "", NULL, 
		navh_eHelpFile_Base, 0, NULL, flow_eDest_IntoLast);

  sts = navhelp->help_index( file_type, file_name);

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);

  flow_qsort( &object_list[2], object_cnt - 2, sizeof(object_list[0]), 
	help_cmp_items);    

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return sts;
}

static int help_cmp_items( const void *node1, const void *node2)
{
  char	text1[80];
  char	text2[80];

  brow_GetAnnotation( *(brow_tNode *) node1, 0, text1, sizeof(text1));
  brow_GetAnnotation( *(brow_tNode *) node2, 0, text2, sizeof(text2));
  cdh_ToLower( text1, text1);
  cdh_ToLower( text2, text2);

  return ( strcmp( text1, text2));
}






