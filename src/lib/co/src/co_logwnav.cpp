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

/* co_logwnav.cpp -- History log window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"


#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "xnav_bitmap_save12.h"
#include "xnav_bitmap_build12.h"
#include "xnav_bitmap_package12.h"
#include "xnav_bitmap_copy12.h"
#include "xnav_bitmap_export12.h"

#include "co_logwnav.h"
#include "co_log.h"

//
//  Free pixmaps
//
void CoLogWNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_save);
  brow_FreeAnnotPixmap( ctx, pixmap_build);
  brow_FreeAnnotPixmap( ctx, pixmap_package);
  brow_FreeAnnotPixmap( ctx, pixmap_copy);
  brow_FreeAnnotPixmap( ctx, pixmap_export);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void CoLogWNavBrow::allocate_pixmaps()
{
  flow_sPixmapData pixmap_data;
  int i;

  for ( i = 0; i < 9; i++) {
    pixmap_data[i].width =xnav_bitmap_save12_width;
    pixmap_data[i].height =xnav_bitmap_save12_height;
    pixmap_data[i].bits = (char *)xnav_bitmap_save12_bits;
  }

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_save);

  for ( i = 0; i < 9; i++) {
    pixmap_data[i].width =xnav_bitmap_build12_width;
    pixmap_data[i].height =xnav_bitmap_build12_height;
    pixmap_data[i].bits = (char *)xnav_bitmap_build12_bits;
  }

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_build);

  for ( i = 0; i < 9; i++) {
    pixmap_data[i].width =xnav_bitmap_package12_width;
    pixmap_data[i].height =xnav_bitmap_package12_height;
    pixmap_data[i].bits = (char *)xnav_bitmap_package12_bits;
  }

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_package);

  for ( i = 0; i < 9; i++) {
    pixmap_data[i].width =xnav_bitmap_copy12_width;
    pixmap_data[i].height =xnav_bitmap_copy12_height;
    pixmap_data[i].bits = (char *)xnav_bitmap_copy12_bits;
  }

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_copy);

  for ( i = 0; i < 9; i++) {
    pixmap_data[i].width =xnav_bitmap_export12_width;
    pixmap_data[i].height =xnav_bitmap_export12_height;
    pixmap_data[i].bits = (char *)xnav_bitmap_export12_bits;
  }

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_export);
}

//
// Create nodeclasses
//
void CoLogWNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "LogDefault", 
		flow_eNodeGroup_Common, &nc_log);
  brow_AddAnnotPixmap( nc_log, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_log, 1.4, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_log, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_log, 13, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_log, 18, 0.6, 3,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_log, 35, 0.6, 4,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_log, 0, 0, 35, 0.83, flow_eDrawType_LineGray, -1, 1);

}

void CoLogWNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, logwnav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	CoLogWNav::brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int CoLogWNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  CoLogWNav *logwnav = (CoLogWNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  logwnav->brow = new CoLogWNavBrow( ctx, (void *)logwnav);

  logwnav->brow->brow_setup();
  logwnav->brow->create_nodeclasses();

  return 1;
}

CoLogWNav::CoLogWNav( void *l_parent_ctx, int l_show_item) :
  parent_ctx(l_parent_ctx), show_item(l_show_item)
{
}


//
//  Delete ev
//
CoLogWNav::~CoLogWNav()
{
}

CoLogWNavBrow::~CoLogWNavBrow()
{
  free_pixmaps();
}


//
//  Zoom
//
void CoLogWNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void CoLogWNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

//
// Callbacks from brow
//
int CoLogWNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  CoLogWNav		*logwnav;
  ItemLog 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &logwnav);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( logwnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( logwnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( logwnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( logwnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( logwnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( logwnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( logwnav->brow->ctx, object);
      if ( !brow_IsVisible( logwnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( logwnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( logwnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( logwnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( logwnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( logwnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( logwnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( logwnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( logwnav->brow->ctx, object);
      if ( !brow_IsVisible( logwnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( logwnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( logwnav->brow->ctx);
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
            sts = CoLogWNav::brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( logwnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( logwnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( logwnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( logwnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( logwnav->brow->ctx);
      }
      break;
    case flow_eEvent_Key_PageDown: {
      brow_Page( logwnav->brow->ctx, 0.95);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( logwnav->brow->ctx, -0.95);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( logwnav->brow->ctx, 0.10);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( logwnav->brow->ctx, -0.10);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      break;
    default:
      ;
  }
  return 1;
}

ItemLog::ItemLog( CoLogWNav *item_logwnav, const char *item_name,
		  pwr_tTime item_time, char *item_category, char *item_user, 
		  char *item_comment, brow_tNode dest, flow_eDest dest_code):
  logwnav(item_logwnav), time(item_time)
{
  char time_str[40];
  char *s;

  type = logwitem_eItemType_Log;

  strncpy( category, item_category, sizeof(category));
  strncpy( user, item_user, sizeof(user));
  if ( item_comment)
    strncpy( comment, item_comment, sizeof(comment));
  else
    strcpy( comment, "");

  brow_CreateNode( logwnav->brow->ctx, (char *)"Log", logwnav->brow->nc_log,
		dest, dest_code, (void *) this, 1, &node);

  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, 
	sizeof(time_str));
  time_str[17] = 0;

  int annot = 0;
  brow_SetAnnotation( node, annot++, time_str, strlen(time_str));
  brow_SetAnnotation( node, annot++, category, strlen(category));
  brow_SetAnnotation( node, annot++, user, strlen(user));
  if ( logwnav->show_item)
    brow_SetAnnotation( node, annot++, item_name, strlen(item_name));
  if ( item_comment)
    brow_SetAnnotation( node, annot++, item_comment, strlen(item_comment));

  if ( (s = strstr( category, "Save")))
    brow_SetAnnotPixmap( node, 0, logwnav->brow->pixmap_save);
  else if ( (s = strstr( category, "Build")))
    brow_SetAnnotPixmap( node, 0, logwnav->brow->pixmap_build);
  else if ( (s = strstr( category, "Copy")))
    brow_SetAnnotPixmap( node, 0, logwnav->brow->pixmap_copy);
  else if ( (s = strstr( category, "Package")))
    brow_SetAnnotPixmap( node, 0, logwnav->brow->pixmap_package);
  else if ( (s = strstr( category, "Export")))
    brow_SetAnnotPixmap( node, 0, logwnav->brow->pixmap_export);
           
}

void CoLogWNav::item_cb( void *ctx, pwr_tTime time, char *category, char *user, char *item, 
			 char *comment)
{
  CoLogWNav *logwnav = (CoLogWNav *)ctx;

  new ItemLog( logwnav, item, time, category, user, comment, 0, flow_eDest_IntoLast);
}

void CoLogWNav::show( char categories[][20], char *item)
{
  brow_SetNodraw( brow->ctx);
  CoLog::dget( categories, item, item_cb, this);
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  // View last items
  brow_tObject last;
  int sts;

  sts = brow_GetLast( brow->ctx, &last);
  if ( ODD(sts))
    brow_CenterObject( brow->ctx, last, 0.9);
}



