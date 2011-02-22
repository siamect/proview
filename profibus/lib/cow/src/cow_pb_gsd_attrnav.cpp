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

/* cow_pb_gsd_attrnav.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow.h"
#include "co_dcli.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"


#include "cow_pb_gsd.h"
#include "cow_pb_gsd_attr.h"
#include "cow_pb_gsd_attrnav.h"
#include "rt_pb_msg.h"

#include "xnav_bitmap_leaf8.h"
#include "xnav_bitmap_leaf10.h"
#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_leaf14.h"
#include "xnav_bitmap_leaf16.h"
#include "xnav_bitmap_leaf18.h"
#include "xnav_bitmap_leaf20.h"
#include "xnav_bitmap_leaf24.h"
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
#include "xnav_bitmap_attrarra8.h"
#include "xnav_bitmap_attrarra10.h"
#include "xnav_bitmap_attrarra12.h"
#include "xnav_bitmap_attrarra14.h"
#include "xnav_bitmap_attrarra16.h"
#include "xnav_bitmap_attrarra18.h"
#include "xnav_bitmap_attrarra20.h"
#include "xnav_bitmap_attrarra24.h"
#include "xnav_bitmap_attrenum8.h"
#include "xnav_bitmap_attrenum10.h"
#include "xnav_bitmap_attrenum12.h"
#include "xnav_bitmap_attrenum14.h"
#include "xnav_bitmap_attrenum16.h"
#include "xnav_bitmap_attrenum18.h"
#include "xnav_bitmap_attrenum20.h"
#include "xnav_bitmap_attrenum24.h"

#define ATTRNAV__INPUT_SYNTAX 2
#define ATTRNAV__OBJNOTFOUND 4
#define ATTRNAV__STRINGTOLONG 6
#define ATTRNAV__ITEM_NOCREA 8
#define ATTRNAV__SUCCESS 1

static char null_str[] = "";


//
// Convert attribute string to value
//
int GsdAttrNav::attr_string_to_value( int type_id, char *value_str, 
				      void *buffer_ptr, int buff_size, int attr_size)
{

  switch ( type_id ) {
  case pwr_eType_Boolean: {
    if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
      return ATTRNAV__INPUT_SYNTAX;
    if ( *(pwr_tBoolean *)buffer_ptr > 1)
      return ATTRNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Float32: {
    pwr_tFloat32 f;
    if ( sscanf( value_str, "%f", &f) != 1)
      return ATTRNAV__INPUT_SYNTAX;
    memcpy( buffer_ptr, (char *) &f, sizeof(f));
    
    break;
  }
  case pwr_eType_Int32: {
    if ( sscanf( value_str, "%d", (int *)buffer_ptr) != 1)
      return ATTRNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_UInt32: {
    if ( sscanf( value_str, "%u", (int *)buffer_ptr) != 1)
      return ATTRNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_String: {
    if ( (int) strlen( value_str) >= attr_size)
      return ATTRNAV__STRINGTOLONG;
    strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
    break;
  }
  }
  return 1;
}

//
// Convert attribute value to string
//
void GsdAttrNav::attrvalue_to_string( int type_id, void *value_ptr, 
				      char *str, int size, int *len, char *format)
{

  if ( value_ptr == 0)  {
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
  case pwr_eType_Float32:{
    if ( !format)
      *len = sprintf( str, "%f", *(float *)value_ptr);
    else
        *len = sprintf( str, format, *(float *)value_ptr);
    break;
  }
  case pwr_eType_UInt32: {
    if ( !format)
      *len = sprintf( str, "%u", *(int *)value_ptr);
    else
      *len = sprintf( str, format, *(int *)value_ptr);
    break;
  }
  case pwr_eType_Int32: {
    if ( !format)
      *len = sprintf( str, "%d", *(int *)value_ptr);
    else
      *len = sprintf( str, format, *(int *)value_ptr);
    break;
  }
  case pwr_eType_String: {
    strncpy( str, (char *)value_ptr, size);
    str[size-1] = 0;
    *len = strlen(str);
    break;
  }
  }
}

void GsdAttrNav::message( char sev, const char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
//  Free pixmaps
//
void GsdAttrNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
  brow_FreeAnnotPixmap( ctx, pixmap_attrarray);
  brow_FreeAnnotPixmap( ctx, pixmap_attrenum);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void GsdAttrNavBrow::allocate_pixmaps()
{
  flow_sPixmapData pixmap_data;
  int i;

  i = 0;
  pixmap_data[i].width =xnav_bitmap_leaf8_width;
  pixmap_data[i].height =xnav_bitmap_leaf8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf8_bits;
  pixmap_data[i].width =xnav_bitmap_leaf10_width;
  pixmap_data[i].height =xnav_bitmap_leaf10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf10_bits;
  pixmap_data[i].width =xnav_bitmap_leaf12_width;
  pixmap_data[i].height =xnav_bitmap_leaf12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf12_bits;
  pixmap_data[i].width =xnav_bitmap_leaf14_width;
  pixmap_data[i].height =xnav_bitmap_leaf14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf14_bits;
  pixmap_data[i].width =xnav_bitmap_leaf16_width;
  pixmap_data[i].height =xnav_bitmap_leaf16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf16_bits;
  pixmap_data[i].width =xnav_bitmap_leaf18_width;
  pixmap_data[i].height =xnav_bitmap_leaf18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf18_bits;
  pixmap_data[i].width =xnav_bitmap_leaf20_width;
  pixmap_data[i].height =xnav_bitmap_leaf20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf20_bits;
  pixmap_data[i].width =xnav_bitmap_leaf20_width;
  pixmap_data[i].height =xnav_bitmap_leaf20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf20_bits;
  pixmap_data[i].width =xnav_bitmap_leaf24_width;
  pixmap_data[i].height =xnav_bitmap_leaf24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf24_bits;

  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_leaf);
  
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
  pixmap_data[i].width =xnav_bitmap_attrarra8_width;
  pixmap_data[i].height =xnav_bitmap_attrarra8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra8_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra10_width;
  pixmap_data[i].height =xnav_bitmap_attrarra10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra10_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra12_width;
  pixmap_data[i].height =xnav_bitmap_attrarra12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra12_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra14_width;
  pixmap_data[i].height =xnav_bitmap_attrarra14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra14_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra16_width;
  pixmap_data[i].height =xnav_bitmap_attrarra16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra16_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra18_width;
  pixmap_data[i].height =xnav_bitmap_attrarra18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra18_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra20_width;
  pixmap_data[i].height =xnav_bitmap_attrarra20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra20_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra20_width;
  pixmap_data[i].height =xnav_bitmap_attrarra20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra20_bits;
  pixmap_data[i].width =xnav_bitmap_attrarra24_width;
  pixmap_data[i].height =xnav_bitmap_attrarra24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra24_bits;
  
  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrarray);
  
  i = 0;
  pixmap_data[i].width =xnav_bitmap_attrenum8_width;
  pixmap_data[i].height =xnav_bitmap_attrenum8_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum8_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum10_width;
  pixmap_data[i].height =xnav_bitmap_attrenum10_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum10_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum12_width;
  pixmap_data[i].height =xnav_bitmap_attrenum12_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum12_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum14_width;
  pixmap_data[i].height =xnav_bitmap_attrenum14_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum14_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum16_width;
  pixmap_data[i].height =xnav_bitmap_attrenum16_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum16_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum18_width;
  pixmap_data[i].height =xnav_bitmap_attrenum18_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum18_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum20_width;
  pixmap_data[i].height =xnav_bitmap_attrenum20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum20_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum20_width;
  pixmap_data[i].height =xnav_bitmap_attrenum20_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum20_bits;
  pixmap_data[i].width =xnav_bitmap_attrenum24_width;
  pixmap_data[i].height =xnav_bitmap_attrenum24_height;
  pixmap_data[i++].bits = (char *)xnav_bitmap_attrenum24_bits;
  
  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrenum);
}


//
// Create the navigator widget
//
GsdAttrNav::GsdAttrNav(
	void *xn_parent_ctx,
	const char *xn_name,
	pb_gsd  *xn_gsd,
	int xn_edit_mode,
	pwr_tStatus *status) :
  parent_ctx(xn_parent_ctx),
  gsd(xn_gsd), edit_mode(xn_edit_mode), trace_started(0),
  message_cb(0), change_value_cb(0)
{
  strcpy( name, xn_name);
  strcpy( modelname, "");
  
  *status = 1;
}

//
//  Delete a nav context
//
GsdAttrNav::~GsdAttrNav()
{
}

GsdAttrNavBrow::~GsdAttrNavBrow()
{
  free_pixmaps();
}

//
//  Get current zoom factor
//
void GsdAttrNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

//
//  Zoom
//
void GsdAttrNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void GsdAttrNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

// Get selected item
int GsdAttrNav::get_select( ItemPb **item)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPb	*base_item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  *item = base_item;
  return 1;
}

//
// Set attribute value
//
int GsdAttrNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPb	*base_item;
  int 		sts;
  char		buffer[1024];
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);


  switch( base_item->type) {
    case attrnav_eItemType_PbBase: {
      ItemPbBase	*item = (ItemPbBase *)base_item;

      sts = attr_string_to_value( item->type_id, value_str, 
				  buffer, sizeof(buffer), item->size);
      if ( EVEN(sts)) return sts;

      if ( item->max_limit != 0 || item->min_limit != 0) {
	switch ( item->type_id) {
	case pwr_eType_Int32:
	case pwr_eType_UInt32:
	  if ( *(int *)&buffer < item->min_limit ||
	       *(int *)&buffer > item->max_limit) {
	    message( 'E', "Min or maxvalue exceeded");
	    return 0;
	  }
	  break;
	default: ;
	}
      }
      memcpy( item->value_p, buffer, item->size);
      gsd->set_modified(1);
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
int GsdAttrNav::check_attr_value( char **value)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPb	*base_item;
  static char   buf[200];
  int           len;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return PB__NOATTRSEL;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
    case attrnav_eItemType_PbBase: {
      ItemPbBase	*item = (ItemPbBase *)base_item;

      if ( item->noedit) {
        *value = 0;
        return PB__ATTRNOEDIT;
      }
      attrvalue_to_string( item->type_id, item->value_p, buf, 
			   sizeof(buf), &len, NULL);
      *value = buf;
      return PB__SUCCESS;
    }
    default:
      *value = 0;
      return PB__ATTRNOEDIT;
  }
  return PB__SUCCESS;
}


//
// Callbacks from brow
//
int GsdAttrNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  GsdAttrNav		*attrnav;
  ItemPb 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted) {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &attrnav);
  attrnav->message( ' ', null_str);
  switch ( event->event) {
    case flow_eEvent_Key_Up: {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( attrnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( attrnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( attrnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( attrnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
	    if ( node_count)
	      free( node_list);
	    return 1;
 	  }
        }
      }
      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( attrnav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Down: {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( attrnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( attrnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( attrnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( attrnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( attrnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( attrnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      switch ( event->object.object_type) {
        case flow_eObjectType_Node:
          brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
	  if ( event->object.x < ll_x + 1.0) {
            // Simulate doubleclick
            flow_tEvent doubleclick_event;

            doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
            memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
            doubleclick_event->event = flow_eEvent_MB1DoubleClick;
            sts = brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( attrnav->brow->ctx, event->object.object)) {
            brow_SelectClear( attrnav->brow->ctx);
	  }
          else {
            brow_SelectClear( attrnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( attrnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( attrnav->brow->ctx);
      }
      break;
    case flow_eEvent_MB2Click: {
      // Select
      brow_tNode	*node_list;
      int		node_count;
      int		sts;

      if ( !attrnav->edit_mode) {
	attrnav->message('E', "Not in edit mode");
	break;
      }

      switch ( event->object.object_type) {
        case flow_eObjectType_Node:
	  brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
	  if ( !node_count) {
	    attrnav->message('E', "Select a module");
	    break;
	  }
	  else if ( node_count > 1) {
	    attrnav->message('E', "Select one module");
	    free( node_list);
	    break;
	  }
	  
	  ItemPbModule *item_dest, *item_src;
	  brow_GetUserData( node_list[0], (void **)&item_src);
	  brow_GetUserData( event->object.object, (void **)&item_dest);
	  
	  if ( item_src->type != attrnav_eItemType_PbModule) {
	    attrnav->message('E', "Unable to move this object");
	    free( node_list);
	    break;
	  }
	  if ( item_dest->type != attrnav_eItemType_PbModule) {
	    attrnav->message('E', "Invalid destination");
	    free( node_list);
	    break;
	  }

	  sts = attrnav->gsd->move_module_conf( item_src->mconf, item_dest->mconf);
	  if ( ODD(sts)) {
            brow_SelectClear( attrnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( attrnav->brow->ctx, event->object.object);
	  }
	  free( node_list);

          break;
        default:
          brow_SelectClear( attrnav->brow->ctx);
      }
      break;
    }
    case flow_eEvent_Key_Left: {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else {
        // Close parent
        sts = brow_GetParent( attrnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts)) {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      item->close( attrnav, 0, 0);

      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( attrnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_Key_Right: {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type) {
        case attrnav_eItemType_PbBase:
        case attrnav_eItemType_PbEnum:
          if ( ((ItemPbBase *)item)->parent)
	    item->open_children( attrnav, 0, 0);
          else if ( !((ItemPbBase *)item)->parent && attrnav->change_value_cb)
	    (attrnav->change_value_cb) ( attrnav->parent_ctx);
          break;
        case attrnav_eItemType_PbEnumByteOrder:
	  item->open_children( attrnav, 0, 0);
          break;
        case attrnav_eItemType_PbEnumValue: {
          int value;
	  if ( !attrnav->edit_mode) {
	    attrnav->message('E', "Not in edit mode");
	    break;
	  }

	  brow_GetRadiobutton( node_list[0], 0, &value);
	  if ( !value) {
	    brow_SetRadiobutton( node_list[0], 0, 1);
	    *(int *)((ItemPbEnumValue *)item)->value_p = ((ItemPbEnumValue *)item)->num;
	    attrnav->gsd->set_modified(1);
	  }
          break;
	default:
	  item->open_children( attrnav, 0, 0);	    
        }
	break;
      }
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type) {
      case flow_eObjectType_Node:
	brow_GetUserData( event->object.object, (void **)&item);
	item->open_children( attrnav, event->object.x, event->object.y);
	break;
      default:
	;
      }
      break;
    case flow_eEvent_MB1DoubleClickCtrl:
      break;
    case flow_eEvent_Radiobutton: {
      if ( !attrnav->edit_mode) {
	attrnav->message('E', "Not in edit mode");
	break;
      }
      switch ( event->object.object_type) {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type) {
            case attrnav_eItemType_PbEnumValue: 
              if ( !event->radiobutton.value) {
                 brow_SetRadiobutton( event->radiobutton.object, 
			event->radiobutton.number, !event->radiobutton.value);
	         *(int *)((ItemPbEnumValue *)item)->value_p = ((ItemPbEnumValue *)item)->num;
		 attrnav->gsd->set_modified( 1);	    
              }
              break;
            default:
              ;
          }
          break;
        default:
          ;
      }

      break;
    }
    case flow_eEvent_Key_PageDown: {
      brow_Page( attrnav->brow->ctx, 0.8);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( attrnav->brow->ctx, -0.8);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( attrnav->brow->ctx, 0.1);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( attrnav->brow->ctx, -0.1);
      break;
    }
    default:
      ;
  }
  return 1;
}

void GsdAttrNav::trace_scan( void *data)
{
  GsdAttrNav *attrnav = (GsdAttrNav *)data;
  int time = 200;

  if ( attrnav->trace_started) {
    brow_TraceScan( attrnav->brow->ctx);

    attrnav->trace_timerid->add( time, trace_scan, attrnav);
  }
}

void GsdAttrNav::force_trace_scan()
{
  if ( trace_started)
    brow_TraceScan( brow->ctx);
}

int GsdAttrNav::trace_scan_bc( brow_tObject object, void *p)
{
  ItemPb       	*base_item;
  char		buf[200];
  int		len;
  GsdAttrNav	*attrnav;

  brow_GetUserData( object, (void **)&base_item);

  switch( base_item->type) {
    case attrnav_eItemType_PbBase: {
      ItemPbBase	*item;

      item = (ItemPbBase *)base_item;
      if ( item->size == 0)
        break;

      if ( !item->first_scan) {
        if ( item->size > (int) sizeof(item->old_value) && 
	     item->type_id == glow_eType_String &&
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
    case attrnav_eItemType_PbEnum: {
      ItemPbEnum	*item;

      item = (ItemPbEnum *)base_item;

      if ( !item->first_scan) {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

      attrnav->gsd->prm_text_val_to_str( item->enumtext, *(int *)p, buf);
      brow_SetAnnotation( object, 1, buf, strlen(buf));
      item->old_value = *(int *)p;
      break;
    }
    case attrnav_eItemType_PbEnumByteOrder: {
      ItemPbEnumByteOrder	*item;

      item = (ItemPbEnumByteOrder *)base_item;

      if ( !item->first_scan) {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

      if ( *(int *)p == 0)
	strcpy( buf, "LittleEndian");
      else
	strcpy( buf, "BigEndian");
      
      brow_SetAnnotation( object, 1, buf, strlen(buf));
      item->old_value = *(int *)p;
      break;
    }
    case attrnav_eItemType_PbModuleType: {
      ItemPbModuleType	*item;
      int sts;
      gsd_sModule *mp = 0;

      item = (ItemPbModuleType *)base_item;

      if ( !item->first_scan) {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }

      brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

      if ( *(int *)p == 0)
	strcpy( buf, "No");
      else {

	int idx = 1;
	for ( mp = attrnav->gsd->modulelist; mp; mp = mp->next) {
	  if ( idx++ == *(int *)p)
	    break;
	}
	if ( !mp)
	  strcpy( buf, "Unknown module");
	else
	  strcpy( buf, mp->Mod_Name);
      }
      brow_SetAnnotation( object, 1, buf, strlen(buf));
      item->old_value = *(int *)p;

      if ( !item->first_scan) {
	ItemPbModuleData       	*prm_item;

	attrnav->gsd->configure_module( item->mconf);
	
	// Update Data map
	brow_tObject odata;
	sts = brow_GetNextSibling( attrnav->brow->ctx, object, &odata);
	brow_GetUserData( odata, (void **)&prm_item);
	prm_item->update( attrnav);

	// Set default class
	if ( mp) {
	  pwr_tObjName mclass;
	  pwr_tCid *datap;
	  pwr_tCid mcid = 0;

	  sts = attrnav->search_class( pb_cModuleClassFile, attrnav->modelname,
			      mp->Mod_Name, mclass);
	  if ( ODD(sts)) {
	    for ( int i = 0; attrnav->gsd->module_classlist[i].cid; i++) {
	      if ( cdh_NoCaseStrcmp( mclass, attrnav->gsd->module_classlist[i].name) == 0) {
		mcid = attrnav->gsd->module_classlist[i].cid;
		break;
	      }
	    }
	  }

	  brow_tObject cobject;
	  sts = brow_GetNextSibling( attrnav->brow->ctx, odata, &cobject);
	  if ( ODD(sts)) {
	    ItemPb *item;

	    brow_GetUserData( cobject, (void **)&item);
	    if ( item->type == attrnav_eItemType_PbModuleClass) {	
	      brow_GetTraceData( cobject, (void **)&datap);
	      *datap = mcid;
	    }
	  }
	}	
      }
      else
        item->first_scan = 0;
	
      break;
    }
    case attrnav_eItemType_PbModuleClass: {
      ItemPbModuleClass	*item = (ItemPbModuleClass *)base_item;

      if ( !item->first_scan) {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

      if ( *(int *)p == 0)
	strcpy( buf, "No");
      else {
	int found = 0;
	int i;
	for ( i = 0; attrnav->gsd->module_classlist[i].cid; i++) {
	  if ( attrnav->gsd->module_classlist[i].cid == *(pwr_tCid *)p) {
	    found = 1;
	    break;
	  }
	}
	if ( !found)
	  strcpy( buf, "Unknown class");
	else
	  strcpy( buf, attrnav->gsd->module_classlist[i].name);
      }
      brow_SetAnnotation( object, 1, buf, strlen(buf));
      item->old_value = *(int *)p;      

      if ( item->first_scan)
        item->first_scan = 0;
	
      break;
    }
    case attrnav_eItemType_PbModule: {
      ItemPbModule	*item = (ItemPbModule *)base_item;

      brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

      if ( !item->mconf->module)
	strcpy( buf, "");
      else
	strcpy( buf, item->mconf->module->Mod_Name);
      if ( strcmp( buf, item->old_type) != 0) {
	brow_SetAnnotation( object, 2, buf, strlen(buf));
	strcpy( item->old_type, buf);
      }

      if ( !item->first_scan) {
        if ( strcmp( item->old_value, (char *)p) == 0)
          // No change since last time
          return 1;
      }

      strcpy( buf, (char *)p);
      brow_SetAnnotation( object, 1, buf, strlen(buf));
      strcpy( item->old_value, (char *)p);

      if ( item->first_scan)
        item->first_scan = 0;	
      break;
    }
    case attrnav_eItemType_PbEnumValue:
    {
      ItemPbEnumValue	*item;

      item = (ItemPbEnumValue *)base_item;
      if ( !item->first_scan)
      {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *(int *)p == item->num)
        brow_SetRadiobutton( item->node, 0, 1);
      else
        brow_SetRadiobutton( item->node, 0, 0);

      item->old_value = *(int *) p;
      break;
    }
    default:
      ;
  }
  return 1;
}

int GsdAttrNav::trace_connect_bc( brow_tObject object, char *name, char *attr, 
				  flow_eTraceType type, void **p)
{
  ItemPb 		*base_item;

  /*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type) {
    case attrnav_eItemType_PbBase:
    case attrnav_eItemType_PbEnum:
    case attrnav_eItemType_PbEnumByteOrder: {
      ItemPbBase	*item = (ItemPbBase *) base_item;
      if (item->size == 0)
	break;

      *p = item->value_p;
      break;
    }
    case attrnav_eItemType_PbModuleType: {
      ItemPbModuleType	*item = (ItemPbModuleType *) base_item;

      *p = &item->mconf->idx;
      break;
    }
    case attrnav_eItemType_PbModule: {
      ItemPbModule	*item = (ItemPbModule *) base_item;

      *p = &item->mconf->name;
      break;
    }
    case attrnav_eItemType_PbModuleClass: {
      ItemPbModuleClass	*item = (ItemPbModuleClass *) base_item;

      *p = &item->mconf->cid;
      break;
    }
    case attrnav_eItemType_PbEnumValue: {
      ItemPbEnumValue	*item = (ItemPbEnumValue *) base_item;

      *p = item->value_p;
      break;
    }
    default:
      ;
  }      
  return 1;
}

int GsdAttrNav::trace_disconnect_bc( brow_tObject object)
{
  ItemPb 		*base_item;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type) {
    default:
      ;
  }
  return 1;
}


//
// Create nodeclasses
//
void GsdAttrNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "NavigatorDefault", 
		flow_eNodeGroup_Common, &nc_object);
  brow_AddAnnotPixmap( nc_object, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_object, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_object, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_object, 7, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_object, 12, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_object, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorGsdAttr", 
		flow_eNodeGroup_Common, &nc_attr);
  brow_AddAnnotPixmap( nc_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 12, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_attr, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create multiline attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorGsdAttrMultiLine", 
		flow_eNodeGroup_Common, &nc_attr_multiline);
  brow_AddAnnotPixmap( nc_attr_multiline, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_multiline, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_multiline, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_MultiLine, 
		1);
  brow_AddFrame( nc_attr_multiline, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorEnum", 
		flow_eNodeGroup_Common, &nc_enum);
  brow_AddRadiobutton( nc_enum, 15, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddAnnotPixmap( nc_enum, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_enum, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddFrame( nc_enum, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);
 
  // Create table nodeclass

  brow_CreateNodeClass( ctx, "NavigatorTable", 
		flow_eNodeGroup_Common, &nc_table);
  brow_AddAnnotPixmap( nc_table, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_table, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table, 8, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 12, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 16, 0.6, 3,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 20, 0.6, 4,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 24, 0.6, 5,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 28, 0.6, 6,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 32, 0.6, 7,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 35, 0.6, 8,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 38, 0.6, 9,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_table, 0, 0, 20, 0.83, flow_eDrawType_LineGray, -1, 1);

  // Create Header

  brow_CreateNodeClass( ctx, "NavigatorHead", 
		flow_eNodeGroup_Common, &nc_header);
  brow_AddAnnotPixmap( nc_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddFrame( nc_header, 0, 0, 20, 0.83, flow_eDrawType_LineGray, 2, 1);

  // Create TableHeader

  brow_CreateNodeClass( ctx, "NavigatorTableHead", 
		flow_eNodeGroup_Common, &nc_table_header);
  brow_AddAnnotPixmap( nc_table_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 12, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 16, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 20, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 24, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 28, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 32, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 35, 0.6, 8,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 38, 0.6, 9,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddFrame( nc_table_header, 0, 0, 20, 0.83, flow_eDrawType_LineGray, 2, 1);

}

void GsdAttrNav::print( char *filename)
{
  brow_Print( brow->ctx, filename);
}

int	GsdAttrNav::object_attr()
{
  gsd_sKeyword *keyp;
  gsd_sData *datap;
  unsigned int type;
  int size;
  void *p;

  brow_SetNodraw( brow->ctx);

  for ( keyp = gsd->keywordlist, datap = gsd->datalist; keyp->type != gsd_End; keyp++, datap++) {
    if ( datap->found) {
      if ( !(strcmp( keyp->name, "Vendor_Name") == 0 ||
	     strcmp( keyp->name, "Model_Name") == 0))
	continue;
      switch ( keyp->type) {
      case gsd_ProfibusDP:
      case gsd_Boolean:
      case gsd_Unsigned8:
      case gsd_Unsigned16:
      case gsd_Unsigned32:
      case gsd_UserPrmDataLen:
	type = pwr_eType_UInt32;
	size = sizeof(pwr_tUInt32);
	p = &datap->value;
	break;
      case gsd_VString8:
	type = pwr_eType_String;
	size = 8;
	p = datap->data;
	break;
      case gsd_SlaveFamily:
      case gsd_VString32:
	type = pwr_eType_String;
	size = 32;
	p = datap->data;
	break;
      case gsd_VString256:
	type = pwr_eType_String;
	size = 256;
	p = datap->data;
	break;
      default:
	size = 0;
	break;
      }
      if ( size != 0)
	new ItemPbBase( this, keyp->name, "LocalGsdAttr", 
		       type, size, 0, 0,
		       p, 1, 0,
		       NULL, flow_eDest_IntoLast);
      if ( strcmp( keyp->name, "Model_Name") == 0)
	strcpy( modelname, (char *)p);

    }    
  }

  p = (void *) &gsd->address;
  new ItemPbBase( this, "Address", "LocalGsdAttr", 
		  pwr_eType_Int32, sizeof(pwr_tInt32), 0, 0,
		  p, 0, 0,
		  NULL, flow_eDest_IntoLast);

  p = (void *) &gsd->byte_order;
  new ItemPbEnumByteOrder( this, "ByteOrdering", "LocalGsdAttr", 
		       pwr_eType_Int32, sizeof(pwr_tInt32), 0, 0,
		       p, 0,
		       NULL, flow_eDest_IntoLast);

  new ItemPbMoreData( this, "SlaveGsdData", NULL, flow_eDest_IntoLast);
  new ItemPbPrmData( this, "UserPrmData", NULL, flow_eDest_IntoLast);

  for ( int i = 0; i < gsd->module_conf_cnt; i++) {
    new ItemPbModule( this, "Module", &gsd->module_conf[i],
		      NULL, flow_eDest_IntoLast);
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return PB__SUCCESS;
}

void GsdAttrNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, attrnav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB2Click, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	GsdAttrNav::brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int GsdAttrNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  GsdAttrNav *attrnav = (GsdAttrNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  int		sts;

  attrnav->brow = new GsdAttrNavBrow( ctx, (void *)attrnav);

  attrnav->brow->brow_setup();
  attrnav->brow->create_nodeclasses();

  // Create the root item
  attrnav->object_attr();

  sts = brow_TraceInit( ctx, trace_connect_bc, 
			trace_disconnect_bc, trace_scan_bc);
  attrnav->trace_started = 1;

  trace_scan( attrnav);

  return 1;
}

int GsdAttrNav::search_class( const char *filename, const char *model, 
			      const char *module, char *mclass)
{
  char line[200];
  char itemv[2][200];
  pwr_tFileName fname;
  int num;
  ifstream	fp;
  int in_model = 0;
  int in_par = 0;
  char lmodel[200];
  char lmodule[200];

  dcli_trim( lmodel, (char *)model);
  dcli_trim( lmodule, (char *)module);

  dcli_translate_filename( fname, filename);

  fp.open( fname);
  if ( !fp)
    return 0;

  while ( fp.getline( line, sizeof(line)) ) {
    if ( line[0] == '#')
      continue;

    num = dcli_parse( line, " 	", "",
		      (char *) itemv, sizeof( itemv)/sizeof( itemv[0]), 
		      sizeof( itemv[0]), 0);
    if ( num < 1)
      continue;

    dcli_trim( itemv[0], itemv[0]);
    if ( num >= 2)
      dcli_trim( itemv[1], itemv[1]);

    if ( cdh_NoCaseStrcmp( itemv[0], "Model") == 0) {
      if ( num < 2)
	continue;

      if ( in_model)
	continue;

      if ( cdh_NoCaseStrcmp( itemv[1], lmodel) == 0)
	in_model = 1;
    }

    if ( in_model) {
      if ( strcmp( itemv[0], "{") == 0)
	in_par = 1;
    }

    if ( in_par) {
      if ( num < 2)
	continue;

      if ( strcmp( itemv[0], "}") == 0)
	break;

      if ( strcmp( itemv[1], "-") == 0 || strcmp( itemv[1], "") == 0)
	continue;

      if ( cdh_NoCaseStrcmp( itemv[0], lmodule) == 0) {
	strncpy( mclass, itemv[1], sizeof(pwr_tObjName));
	fp.close();
	return 1;
      }
    }
  }

  fp.close();
  return 0;
}


ItemPbBase::ItemPbBase( GsdAttrNav *attrnav, const char *item_name, const char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, void *attr_value_p,
	int attr_noedit, gsd_sPrmText *attr_enumtext,
	brow_tNode dest, flow_eDest dest_code) :
	value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
	min_limit(attr_min_limit), max_limit(attr_max_limit),
	noedit(attr_noedit), enumtext(attr_enumtext),
	subgraph(0)
{

  type = attrnav_eItemType_PbBase;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  if ( enumtext)
    parent = 1;
  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

ItemPbEnum::ItemPbEnum( GsdAttrNav *attrnav, const char *item_name, const char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, void *attr_value_p,
	int attr_noedit, gsd_sPrmText *attr_enumtext,
	brow_tNode dest, flow_eDest dest_code) :
  ItemPbBase( attrnav, item_name, attr, attr_type, attr_size, attr_min_limit,
	      attr_max_limit, attr_value_p, attr_noedit,
	      attr_enumtext, dest, dest_code)
{
  type = attrnav_eItemType_PbEnum;
}

int ItemPbEnum::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else if ( parent && !noedit && enumtext) {
    int				found;

    found = 0;
    brow_SetNodraw( attrnav->brow->ctx);
    for ( gsd_sText *tp = enumtext->text_list; tp; tp = tp->next) {

        new ItemPbEnumValue( attrnav, tp->Text, tp->Prm_Data_Value, pwr_eType_UInt32, 
		this->value_p, node, flow_eDest_IntoLast);
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPb::close( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      switch ( type) {
      case attrnav_eItemType_PbEnum:
      case attrnav_eItemType_PbModuleType:
      case attrnav_eItemType_PbModuleClass:
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
	break;
      default:
	if ( parent)
	  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
	else
	  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
      }
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbEnumValue::ItemPbEnumValue( GsdAttrNav *attrnav, const char *item_name, int item_num, 
	int item_type_id, void *attr_value_p, 
	brow_tNode dest, flow_eDest dest_code) :
	num(item_num), type_id(item_type_id), value_p(attr_value_p), first_scan(1)
{

  type = attrnav_eItemType_PbEnumValue;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_enum, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  if ( *(int *)value_p == num)
    brow_SetRadiobutton( node, 0, 1);
  else
    brow_SetRadiobutton( node, 0, 0);
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

ItemPbModule::ItemPbModule( GsdAttrNav *attrnav, const char *item_name, 
			    gsd_sModuleConf *item_mconf,
			    brow_tNode dest, flow_eDest dest_code):
  mconf(item_mconf), first_scan(1)
{
  type = attrnav_eItemType_PbModule;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPbModule::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

#if 0
      new ItemPbBase( this, gsd->prm_dataitems[i].ref->prm_data->Ext_User_Prm_Data_Name, 
		      "LocalGsdAttr", 
		      type, size, 
		      gsd->prm_dataitems[i].ref->prm_data->Min_Value,
		      gsd->prm_dataitems[i].ref->prm_data->Max_Value,
		      p, 0, 0,
		      NULL, flow_eDest_IntoLast);
#endif 
    new ItemPbBase( attrnav, "ObjectName", "LocalGsdAttr", 
		    pwr_eType_String, 32, 0, 0,
		    mconf->name, 0, 0,
		    node, flow_eDest_IntoLast);

    new ItemPbModuleType( attrnav, "Type", mconf,
			  node, flow_eDest_IntoLast);
    new ItemPbModuleData( attrnav, "UserPrmData", mconf,
			  node, flow_eDest_IntoLast);
    if ( attrnav->gsd->module_classlist) {
      new ItemPbModuleClass( attrnav, "Class", mconf,
			     node, flow_eDest_IntoLast);
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbModuleType::ItemPbModuleType( GsdAttrNav *attrnav, const char *item_name, 
				    gsd_sModuleConf *item_mconf,
				    brow_tNode dest, flow_eDest dest_code):
  mconf(item_mconf), first_scan(1)
{
  type = attrnav_eItemType_PbModuleType;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPbModuleType::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    int idx = 0;
    new ItemPbEnumValue( attrnav, "No", idx++, pwr_eType_UInt32, 
			 &this->mconf->idx, node, flow_eDest_IntoLast);
    
    for ( gsd_sModule *mp = attrnav->gsd->modulelist; mp; mp = mp->next) {
      new ItemPbEnumValue( attrnav, mp->Mod_Name, idx++, pwr_eType_UInt32, 
			   &this->mconf->idx, node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbModuleData::ItemPbModuleData( GsdAttrNav *attrnav, const char *item_name, 
				    gsd_sModuleConf *item_mconf,
				    brow_tNode dest, flow_eDest dest_code):
  mconf(item_mconf)
{
  type = attrnav_eItemType_PbModuleData;
  parent = (mconf->prm_dataitems_cnt > 0);
  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}


void ItemPbModuleData::update( GsdAttrNav *attrnav)
{
  parent = (mconf->prm_dataitems_cnt > 0);

  // If open close
  if ( brow_IsOpen( node)) {
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( parent)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, 0);
  }
  else {
    if ( parent)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
  }
}

int ItemPbModuleData::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    if ( mconf->module) {
      unsigned int type;
      int size;
      void *p;

      for ( int i = 0; i < mconf->prm_dataitems_cnt; i++) {
	p = &mconf->prm_dataitems[i].value;
	switch( mconf->prm_dataitems[i].ref->prm_data->data_type) {
	case gsd_Signed32:
	case gsd_Signed16:
	case gsd_Signed8:
	  type = pwr_eType_UInt32;
	  break;
	default:
	  type = pwr_eType_Int32;
	}
	size = sizeof(pwr_tUInt32);

	if ( !mconf->prm_dataitems[i].ref->prm_data->prmtext)
	  new ItemPbBase( attrnav, mconf->prm_dataitems[i].ref->prm_data->Ext_User_Prm_Data_Name, 
		      "LocalGsdAttr", 
		      type, size, 
		      mconf->prm_dataitems[i].ref->prm_data->Min_Value,
		      mconf->prm_dataitems[i].ref->prm_data->Max_Value,
		      p, 0, 0,
		      node, flow_eDest_IntoLast);
	else
	  new ItemPbEnum( attrnav, mconf->prm_dataitems[i].ref->prm_data->Ext_User_Prm_Data_Name, 
		      "LocalGsdAttr", 
		      type, size, 0, 0,
		      p, 0, mconf->prm_dataitems[i].ref->prm_data->prmtext,
		      node, flow_eDest_IntoLast);
      }
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbModuleClass::ItemPbModuleClass( GsdAttrNav *attrnav, const char *item_name, 
				    gsd_sModuleConf *item_mconf,
				    brow_tNode dest, flow_eDest dest_code):
  mconf(item_mconf), first_scan(1)
{
  type = attrnav_eItemType_PbModuleClass;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPbModuleClass::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    for ( int i = 0; attrnav->gsd->module_classlist[i].cid; i++) {
      new ItemPbEnumValue( attrnav, attrnav->gsd->module_classlist[i].name, 
			   attrnav->gsd->module_classlist[i].cid, pwr_eType_UInt32, 
			   &this->mconf->cid, node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}


ItemPbPrmData::ItemPbPrmData( GsdAttrNav *attrnav, const char *item_name, 
				    brow_tNode dest, flow_eDest dest_code)
{
  type = attrnav_eItemType_PbPrmData;
  parent = (attrnav->gsd->prm_dataitems_cnt > 0);

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPbPrmData::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;
  unsigned int type;
  int size;
  void *p;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    for ( int i = 0; i < attrnav->gsd->prm_dataitems_cnt; i++) {
      p = &attrnav->gsd->prm_dataitems[i].value;
      type = pwr_eType_UInt32;
      size = sizeof(pwr_tUInt32);
      
      if ( !attrnav->gsd->prm_dataitems[i].ref->prm_data->prmtext)
	new ItemPbBase( attrnav, 
			attrnav->gsd->prm_dataitems[i].ref->prm_data->Ext_User_Prm_Data_Name, 
			"LocalGsdAttr", 
			type, size, 
			attrnav->gsd->prm_dataitems[i].ref->prm_data->Min_Value,
			attrnav->gsd->prm_dataitems[i].ref->prm_data->Max_Value,
			p, 0, 0,
			node, flow_eDest_IntoLast);
      else
	new ItemPbEnum( attrnav, 
			attrnav->gsd->prm_dataitems[i].ref->prm_data->Ext_User_Prm_Data_Name, 
			"LocalGsdAttr", 
			type, size, 0, 0,
			p, 0, attrnav->gsd->prm_dataitems[i].ref->prm_data->prmtext,
			node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbMoreData::ItemPbMoreData( GsdAttrNav *attrnav, const char *item_name, 
				brow_tNode dest, flow_eDest dest_code)
{
  type = attrnav_eItemType_PbMoreData;
  parent = 1;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPbMoreData::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;
  unsigned int type;
  int size;
  void *p;
  gsd_sKeyword *keyp;
  gsd_sData *datap;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    for ( keyp = attrnav->gsd->keywordlist, datap = attrnav->gsd->datalist; 
	  keyp->type != gsd_End; 
	  keyp++, datap++) {
      if ( strcmp( keyp->name, "Vendor_Name") == 0 ||
	   strcmp( keyp->name, "Model_Name") == 0)
	continue;
      if ( datap->found) {
	switch ( keyp->type) {
	case gsd_ProfibusDP:
	case gsd_Boolean:
	case gsd_Unsigned8:
	case gsd_Unsigned16:
	case gsd_Unsigned32:
	case gsd_UserPrmDataLen:
	  type = pwr_eType_UInt32;
	  size = sizeof(pwr_tUInt32);
	  p = &datap->value;
	  break;
	case gsd_VString8:
	  type = pwr_eType_String;
	  size = 8;
	  p = datap->data;
	  break;
	case gsd_SlaveFamily:
	case gsd_VString32:
	  type = pwr_eType_String;
	  size = 32;
	  p = datap->data;
	  break;
	case gsd_VString256:
	  type = pwr_eType_String;
	  size = 256;
	  p = datap->data;
	  break;
	default:
	  size = 0;
	  break;
	}
	if ( size != 0)
	  new ItemPbBase( attrnav, keyp->name, "LocalGsdAttr", 
			  type, size, 0, 0,
			  p, 1, 0,
			  node, flow_eDest_IntoLast);
      }    
    }      
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPbEnumByteOrder::ItemPbEnumByteOrder( GsdAttrNav *attrnav, const char *item_name, const char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, void *attr_value_p,
	int attr_noedit,
	brow_tNode dest, flow_eDest dest_code) :
  ItemPbBase( attrnav, item_name, attr, attr_type, attr_size, attr_min_limit,
	      attr_max_limit, attr_value_p, attr_noedit,
	      0, dest, dest_code)
{
  type = attrnav_eItemType_PbEnumByteOrder;
  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
}

int ItemPbEnumByteOrder::open_children( GsdAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else if ( !noedit) {
    int				found;

    found = 0;
    brow_SetNodraw( attrnav->brow->ctx);
    new ItemPbEnumValue( attrnav, "LittleEndian", 0, pwr_eType_UInt32, 
		this->value_p, node, flow_eDest_IntoLast);
    new ItemPbEnumValue( attrnav, "BigEndian", 1, pwr_eType_UInt32, 
		this->value_p, node, flow_eDest_IntoLast);
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

