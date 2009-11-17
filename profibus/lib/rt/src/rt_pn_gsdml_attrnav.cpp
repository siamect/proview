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

/* rt_pn_gsdml_attrnav.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"


#include "rt_pn_gsdml.h"
#include "rt_pn_gsdml_attr.h"
#include "rt_pn_gsdml_attrnav.h"
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
int GsdmlAttrNav::attr_string_to_value( int type_id, char *value_str, 
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
void GsdmlAttrNav::attrvalue_to_string( int type_id, void *value_ptr, 
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
  case pwr_eType_UInt8: {
    if ( !format)
      *len = sprintf( str, "%hhu", *(pwr_tUInt8 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tUInt8 *)value_ptr);
    break;
  }
  case pwr_eType_UInt16: {
    if ( !format)
      *len = sprintf( str, "%hu", *(pwr_tUInt16 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tUInt16 *)value_ptr);
    break;
  }
  case pwr_eType_UInt32: {
    if ( !format)
      *len = sprintf( str, "%u", *(int *)value_ptr);
    else
      *len = sprintf( str, format, *(int *)value_ptr);
    break;
  }
  case pwr_eType_Int8: {
    if ( !format)
      *len = sprintf( str, "%hhd", *(pwr_tInt8 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tInt8 *)value_ptr);
    break;
  }
  case pwr_eType_Int16: {
    if ( !format)
      *len = sprintf( str, "%hd", *(pwr_tInt16 *)value_ptr);
    else
      *len = sprintf( str, format, *(pwr_tInt16 *)value_ptr);
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
  default:
    *len = 0;
    str[0] = 0;
  }
}

void GsdmlAttrNav::message( char sev, const char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
//  Free pixmaps
//
void GsdmlAttrNavBrow::free_pixmaps()
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
void GsdmlAttrNavBrow::allocate_pixmaps()
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
GsdmlAttrNav::GsdmlAttrNav(
	void *xn_parent_ctx,
	const char *xn_name,
	pn_gsdml  *xn_gsdml,
	int xn_edit_mode,
	pwr_tStatus *status) :
  parent_ctx(xn_parent_ctx),
  gsdml(xn_gsdml), edit_mode(xn_edit_mode), trace_started(0),
  message_cb(0), change_value_cb(0), device_num(0), device_item(0), 
  device_confirm_active(0), device_read(0), viewio(0), time_ratio(0)
{
  strcpy( name, xn_name);

  *status = 1;
}

//
//  Delete a nav context
//
GsdmlAttrNav::~GsdmlAttrNav()
{
}

GsdmlAttrNavBrow::~GsdmlAttrNavBrow()
{
  free_pixmaps();
}

//
//  Get current zoom factor
//
void GsdmlAttrNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

//
//  Zoom
//
void GsdmlAttrNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void GsdmlAttrNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

// Get selected item
int GsdmlAttrNav::get_select( ItemPn **item)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPn	*base_item;
  
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
int GsdmlAttrNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPn	*base_item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  base_item->value_changed( this, value_str);

  return 1;
}

//
// Check that the current selected item is valid for change
//
int GsdmlAttrNav::check_attr_value( char **value)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemPn	*base_item;
  static char   buf[200];
  int           len;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return PB__NOATTRSEL;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
    case attrnav_eItemType_PnBase: {
      ItemPnBase	*item = (ItemPnBase *)base_item;

      if ( item->noedit) {
        *value = 0;
        return PB__ATTRNOEDIT;
      }
      attrvalue_to_string( item->type_id, item->value_p, buf, 
			   sizeof(buf), &len, NULL);
      *value = buf;
      return PB__SUCCESS;
    }
    case attrnav_eItemType_PnParValue: {
      ItemPnParValue *item = (ItemPnParValue *)base_item;
      int sts;

      if ( item->noedit)
        return PB__ATTRNOEDIT;

      sts = gsdml->datavalue_to_string( item->datatype, &item->data[item->byte_offset], 
				  item->size, buf, sizeof(buf));
      if ( EVEN(sts))
	strcpy( buf, "");
      *value = buf;
      return PB__SUCCESS;
    }
    default:
      return PB__ATTRNOEDIT;
  }
  return PB__SUCCESS;
}


//
// Callbacks from brow
//
int GsdmlAttrNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  GsdmlAttrNav		*attrnav;
  ItemPn 		*item;

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

    if ( !attrnav->edit_mode) {
      attrnav->message('E', "Not in edit mode");
      break;
    }

    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
	attrnav->message('E', "Select a slot");
	break;
      }
      else if ( node_count > 1) {
	attrnav->message('E', "Select one slot");
	free( node_list);
	break;
      }
	  
      ItemPnSlot *item_dest, *item_src;
      brow_GetUserData( node_list[0], (void **)&item_src);
      brow_GetUserData( event->object.object, (void **)&item_dest);
	  
      if ( item_src->type != attrnav_eItemType_PnSlot) {
	attrnav->message('E', "Unable to move this object");
	free( node_list);
	break;
      }
      if ( item_dest->type != attrnav_eItemType_PnSlot) {
	attrnav->message('E', "Invalid destination");
	free( node_list);
	break;
      }

#if 0	 
      sts = attrnav->gsdml->move_module_conf( item_src->mconf, item_dest->mconf);
      if ( ODD(sts)) {
	brow_SelectClear( attrnav->brow->ctx);
	brow_SetInverse( event->object.object, 1);
	brow_SelectInsert( attrnav->brow->ctx, event->object.object);
      }
#endif
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
    case attrnav_eItemType_PnBase:
    case attrnav_eItemType_PnParValue:
      if ( ((ItemPnBase *)item)->parent)
	item->open_children( attrnav, 0, 0);
      else if ( !((ItemPnBase *)item)->parent && attrnav->change_value_cb)
	(attrnav->change_value_cb) ( attrnav->parent_ctx);
      break;
    case attrnav_eItemType_PnEnumByteOrder:
      item->open_children( attrnav, 0, 0);
      break;
    case attrnav_eItemType_PnEnumValue: {
      int value;
      if ( !attrnav->edit_mode) {
	attrnav->message('E', "Not in edit mode");
	break;
      }
	
      brow_GetRadiobutton( node_list[0], 0, &value);
      if ( !value) {
	brow_SetRadiobutton( node_list[0], 0, 1);
	*(int *)((ItemPnEnumValue *)item)->value_p = ((ItemPnEnumValue *)item)->num;
	attrnav->set_modified(1);
      }
      break;
    }
    case attrnav_eItemType_PnParEnumBit: 
      if ( !attrnav->edit_mode) {
	attrnav->message('E', "Not in edit mode");
	break;
      }
      item->update( attrnav);
      break;
    default:
      item->open_children( attrnav, 0, 0);	    
    }
    break;
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
      case attrnav_eItemType_PnEnumValue: 
	if ( !event->radiobutton.value) {
	  brow_SetRadiobutton( event->radiobutton.object, 
			       event->radiobutton.number, !event->radiobutton.value);
	  *(int *)((ItemPnEnumValue *)item)->value_p = ((ItemPnEnumValue *)item)->num;
	  attrnav->set_modified( 1);	    
	}
	break;
      case attrnav_eItemType_PnParEnumBit: 
	item->update( attrnav);
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

void GsdmlAttrNav::trace_scan( void *data)
{
  GsdmlAttrNav *attrnav = (GsdmlAttrNav *)data;
  int time = 200;

  if ( attrnav->trace_started) {
    brow_TraceScan( attrnav->brow->ctx);

    attrnav->trace_timerid->add( time, trace_scan, attrnav);
  }
}

void GsdmlAttrNav::force_trace_scan()
{
  if ( trace_started)
    brow_TraceScan( brow->ctx);
}

void GsdmlAttrNav::device_changed_ok( void *ctx, void *data)
{
  GsdmlAttrNav *attrnav = (GsdmlAttrNav *)ctx;

  brow_DeleteAll( attrnav->brow->ctx);
  if ( !attrnav->device_read)
    attrnav->dev_data.device_reset();
  attrnav->object_attr();
  attrnav->device_confirm_active = 0;
  if ( attrnav->device_read)
    attrnav->device_read = 0;
  else
    attrnav->set_modified(1);

}

void GsdmlAttrNav::device_changed_cancel( void *ctx, void *data)
{
  long int old_device = (long int )data;
  GsdmlAttrNav *attrnav = (GsdmlAttrNav *)ctx;
  attrnav->device_num = old_device;
  attrnav->device_confirm_active = 0;
}

void GsdmlAttrNav::redraw()
{
  brow_DeleteAll( brow->ctx);
  object_attr();
}

int GsdmlAttrNav::trace_scan_bc( brow_tObject object, void *p)
{
  ItemPn       	*base_item;
  GsdmlAttrNav	*attrnav;

  brow_GetUserData( object, (void **)&base_item);
  brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

  base_item->scan( attrnav, p);

  return 1;
}

int GsdmlAttrNav::trace_connect_bc( brow_tObject object, char *name, char *attr, 
				  flow_eTraceType type, void **p)
{
  ItemPn 		*base_item;
  GsdmlAttrNav		*attrnav;

  /*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetCtxUserData( brow_GetCtx( object), (void **) &attrnav);

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type) {
    case attrnav_eItemType_PnBase:
    case attrnav_eItemType_PnEnumByteOrder: {
      ItemPnBase	*item = (ItemPnBase *) base_item;
      if (item->size == 0)
	break;

      *p = item->value_p;
      break;
    }
    case attrnav_eItemType_PnDevice: {
      *p = &attrnav->device_num;
      break;
    }
    case attrnav_eItemType_PnSlot: {
      ItemPnSlot	*item = (ItemPnSlot *) base_item;

      *p = &item->slotdata->module_enum_number;
      break;
    }
    case attrnav_eItemType_PnModuleType: {
      ItemPnModuleType	*item = (ItemPnModuleType *) base_item;

      *p = &attrnav->dev_data.slot_data[item->slot_idx]->module_enum_number;
      break;
    }
    case attrnav_eItemType_PnModuleClass: {
      ItemPnModuleClass	*item = (ItemPnModuleClass *) base_item;

      *p = &item->slotdata->module_class;
      break;
    }
    case attrnav_eItemType_PnEnumValue:
      *p = ((ItemPnEnumValue *)base_item)->value_p;
      break;
    case attrnav_eItemType_PnEnumTimeRatio:
      *p = ((ItemPnEnumTimeRatio *)base_item)->value_p;
      break;
    case attrnav_eItemType_PnParEnum:
    case attrnav_eItemType_PnParEnumBit:
    case attrnav_eItemType_PnParValue:
      *p = (void *)1;
      break;
    default:
      ;
  }      
  return 1;
}

int GsdmlAttrNav::trace_disconnect_bc( brow_tObject object)
{
  ItemPn 		*base_item;

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
void GsdmlAttrNavBrow::create_nodeclasses()
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

  brow_CreateNodeClass( ctx, "NavigatorGsdmlAttr", 
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

  brow_CreateNodeClass( ctx, "NavigatorGsdmlAttrMultiLine", 
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
  brow_AddRadiobutton( nc_enum, 18, 0.03, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
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

void GsdmlAttrNav::print( char *filename)
{
  brow_Print( brow->ctx, filename);
}

int	GsdmlAttrNav::object_attr()
{
  if ( device_num == 0)
    device_item = 0;
  else
    device_item = gsdml->ApplicationProcess->DeviceAccessPointList->
      DeviceAccessPointItem[device_num-1];

  brow_SetNodraw( brow->ctx);

  new ItemPnNetwork( this, "NetworkSettings", NULL, flow_eDest_IntoLast);

  new ItemPnEnumByteOrder( this, "ByteOrdering", "LocalGsdAttr", 
			   pwr_eType_Int32, sizeof(pwr_tInt32), (void *)&gsdml->byte_order, 0,
			   NULL, flow_eDest_IntoLast);

  new ItemPnDeviceInfo( this, "DeviceInfo", NULL, flow_eDest_IntoLast);
  new ItemPnDevice( this, "Device", NULL, flow_eDest_IntoLast);

  if ( device_num) {
    unsigned int slot_cnt = 0;

    GsdmlSlotData *sd;
    if ( dev_data.slot_data.size() <= slot_cnt) {
      sd = new GsdmlSlotData();
      sd->slot_number = 0;
      sd->slot_idx = slot_cnt;
      dev_data.slot_data.push_back(sd);
    }
    else {
      sd = dev_data.slot_data[slot_cnt];
      sd->slot_idx = slot_cnt;
    }

    new ItemPnDAP( this, "DAP", sd, NULL, flow_eDest_IntoLast);

    slot_cnt++;

    gsdml_ValuelistIterator iter( gsdml->ApplicationProcess->DeviceAccessPointList->
				  DeviceAccessPointItem[device_num-1]->Body.PhysicalSlots.list);
    for ( unsigned int i = iter.begin(); i != iter.end(); i = iter.next()) {
      if ( i == 0)
	// DAP already created
	continue;
      char name[80];
      sprintf( name, "Slot %u", i);

      if ( dev_data.slot_data.size() <= slot_cnt) {
	sd = new GsdmlSlotData();
	sd->slot_number = i;
	sd->slot_idx = slot_cnt;
	dev_data.slot_data.push_back(sd);
      }
      else {
	sd = dev_data.slot_data[slot_cnt];
	sd->slot_idx = slot_cnt;
	if ( i != sd->slot_number) {
	  printf( "GSML-Error, datafile corrupt, unexpected slot number\n");
	}
      }

      new ItemPnSlot( this, name, sd, NULL, flow_eDest_IntoLast);
      slot_cnt++;
    }    
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return PB__SUCCESS;
}

void GsdmlAttrNavBrow::brow_setup()
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
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB2Click, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	GsdmlAttrNav::brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int GsdmlAttrNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  GsdmlAttrNav *attrnav = (GsdmlAttrNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  int		sts;

  attrnav->brow = new GsdmlAttrNavBrow( ctx, (void *)attrnav);

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

int GsdmlAttrNav::save( const char *filename)
{
  int sts;

  dev_data.device_num = device_num;

  if ( device_num == 0)
    return PB__NODEVICE;

  strncpy( dev_data.device_text, (char *)device_item->ModuleInfo->Body.Name.p, sizeof(dev_data.device_text));
  dev_data.vendor_id = gsdml->DeviceIdentity->Body.VendorID;
  dev_data.device_id = gsdml->DeviceIdentity->Body.DeviceID;
  strncpy( dev_data.version, gsdml->ProfileHeader->Body.ProfileRevision, sizeof(dev_data.version));
  dev_data.byte_order = gsdml->byte_order;

  // Create IOCR data
  if ( dev_data.iocr_data.size() < 1) {
    GsdmlIOCRData *iod = new GsdmlIOCRData();
    dev_data.iocr_data.push_back( iod);
  }
  if ( dev_data.iocr_data.size() < 2) {
    GsdmlIOCRData *iod = new GsdmlIOCRData();
    dev_data.iocr_data.push_back( iod);
  }

  // TODO
  dev_data.iocr_data[0]->type = 1; // Input ?
  dev_data.iocr_data[0]->properties = 1; // Class 1
  dev_data.iocr_data[0]->send_clock_factor = 32; // 1 ms
  dev_data.iocr_data[0]->reduction_ratio = time_ratio; // send_time = 8 * 31.2 us * send_clock_factor
  dev_data.iocr_data[0]->api = 0;

  dev_data.iocr_data[1]->type = 2; // Output ?
  dev_data.iocr_data[1]->properties = dev_data.iocr_data[0]->properties;
  dev_data.iocr_data[1]->send_clock_factor = dev_data.iocr_data[0]->send_clock_factor;
  dev_data.iocr_data[1]->reduction_ratio = dev_data.iocr_data[0]->reduction_ratio;
  dev_data.iocr_data[1]->api = dev_data.iocr_data[0]->api;

  // Load channel diag
  dev_data.channel_diag_reset();
  if ( gsdml->ApplicationProcess->ChannelDiagList) {
    for ( unsigned int i = 0; i < gsdml->ApplicationProcess->ChannelDiagList->ChannelDiagItem.size(); i++) {
      GsdmlChannelDiag *cd = new GsdmlChannelDiag();

      cd->error_type = gsdml->ApplicationProcess->ChannelDiagList->ChannelDiagItem[i]->Body.ErrorType;
      strncpy( cd->name, (char *)gsdml->ApplicationProcess->ChannelDiagList->ChannelDiagItem[i]->
	       Body.Name.p, sizeof(cd->name));
      if ( gsdml->ApplicationProcess->ChannelDiagList->ChannelDiagItem[i]->Body.Help.p)
	strncpy( cd->help, (char *)gsdml->ApplicationProcess->ChannelDiagList->ChannelDiagItem[i]->
		 Body.Help.p, sizeof(cd->help));
      dev_data.channel_diag.push_back( cd);
    }
  }

  // Calculate subslot IO length
  for ( unsigned int i = 0; i < dev_data.slot_data.size(); i++) {
    if ( i == 0) {
      // Device access point
      gsdml_DeviceAccessPointItem *mi = device_item;

      for ( unsigned int j = 0; j < dev_data.slot_data[i]->subslot_data.size(); j++) {
	GsdmlSubslotData *ssd = dev_data.slot_data[i]->subslot_data[j];

	ssd->io_input_length = 0;
	ssd->io_output_length = 0;

	if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 0)
	  continue;

	gsdml_VirtualSubmoduleItem *vsd = 0;
	if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 1)
	  vsd = mi->VirtualSubmoduleList->VirtualSubmoduleItem[0];
	else {
	  for ( unsigned int k = 0; k < mi->VirtualSubmoduleList->VirtualSubmoduleItem.size(); k++) {
	    if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem[k]->Body.SubmoduleIdentNumber == 
		 ssd->submodule_ident_number) {
	      vsd = mi->VirtualSubmoduleList->VirtualSubmoduleItem[k];	      
	      break;
	    }
	  }
	}
	if ( !vsd) {
	  continue;
	}

	if ( !vsd->IOData)
	  continue;

	if ( vsd->IOData->Input) {
	  for ( unsigned k = 0; k < vsd->IOData->Input->DataItem.size(); k++) {

	    gsdml_DataItem *di = vsd->IOData->Input->DataItem[k];
	    gsdml_eValueDataType datatype;
	  
	    gsdml->string_to_value_datatype( di->Body.DataType, &datatype);
	    unsigned int len;

	    sts = gsdml->get_datavalue_length( datatype, di->Body.Length, &len);
	    if ( EVEN(sts)) printf( "GSDML-Error, Datatype %s not yet implemented\n", di->Body.DataType);
	  
	    ssd->io_input_length += len;	    
	  }
	}
	if ( vsd->IOData->Output) {
	  for ( unsigned k = 0; k < vsd->IOData->Output->DataItem.size(); k++) {
	  
	    gsdml_DataItem *di = vsd->IOData->Output->DataItem[k];
	    gsdml_eValueDataType datatype;
	  
	    gsdml->string_to_value_datatype( di->Body.DataType, &datatype);
	    unsigned int len;
	  
	    sts = gsdml->get_datavalue_length( datatype, di->Body.Length, &len);
	    if ( EVEN(sts)) printf( "GSDML-Error, Datatype %s not yet implemented\n", di->Body.DataType);
	  
	    ssd->io_output_length += len;	    
	  }
	}
      }
    }
    else {

      if ( dev_data.slot_data[i]->module_enum_number == 0)
	continue;

      gsdml_UseableModules *um = device_item->UseableModules;
      if ( !um)
	continue;

      gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->
	ModuleItemRef[dev_data.slot_data[i]->module_enum_number-1]->Body.ModuleItemTarget.p;

      for ( unsigned int j = 0; j < dev_data.slot_data[i]->subslot_data.size(); j++) {
	GsdmlSubslotData *ssd = dev_data.slot_data[i]->subslot_data[j];

	ssd->io_input_length = 0;	    
	ssd->io_output_length = 0;	    

	if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 0)
	  continue;

	gsdml_VirtualSubmoduleItem *vsd = 0;
	if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 1)
	  vsd = mi->VirtualSubmoduleList->VirtualSubmoduleItem[0];
	else {
	  for ( unsigned int k = 0; k < mi->VirtualSubmoduleList->VirtualSubmoduleItem.size(); k++) {
	    if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem[k]->Body.SubmoduleIdentNumber == 
		 ssd->submodule_ident_number) {
	      vsd = mi->VirtualSubmoduleList->VirtualSubmoduleItem[k];	      
	      break;
	    }
	  }
	}
	if ( !vsd) {
	  printf( "GSDML-Error, subslot number not found\n");
	  continue;
	}

	if ( !vsd->IOData)
	  continue;

	if ( vsd->IOData->Input) {
	  for ( unsigned k = 0; k < vsd->IOData->Input->DataItem.size(); k++) {

	    gsdml_DataItem *di = vsd->IOData->Input->DataItem[k];
	    gsdml_eValueDataType datatype;
      
	    gsdml->string_to_value_datatype( di->Body.DataType, &datatype);
	    unsigned int len;

	    sts = gsdml->get_datavalue_length( datatype, di->Body.Length, &len);
	    if ( EVEN(sts)) printf( "GSDML-Error, Datatype %s not yet implemented\n", di->Body.DataType);

	    ssd->io_input_length += len;	    
	  }
	}
	if ( vsd->IOData->Output) {
	  for ( unsigned k = 0; k < vsd->IOData->Output->DataItem.size(); k++) {

	    gsdml_DataItem *di = vsd->IOData->Output->DataItem[k];
	    gsdml_eValueDataType datatype;
      
	    gsdml->string_to_value_datatype( di->Body.DataType, &datatype);
	    unsigned int len;
	    
	    sts = gsdml->get_datavalue_length( datatype, di->Body.Length, &len);
	    if ( EVEN(sts)) printf( "GSDML-Error, Datatype %s not yet implemented\n", di->Body.DataType);

	    ssd->io_output_length += len;	    
	  }
	}
      }
    }
  }

  return dev_data.print( filename);

  // Unload channel diag
  dev_data.channel_diag_reset();
}

int GsdmlAttrNav::open( const char *filename)
{
  int sts;

  strncpy( dev_data.gsdmlfile, gsdml->gsdmlfile, sizeof(dev_data.gsdmlfile));

  sts = dev_data.read( filename);
  if ( sts == PB__GSDMLFILEMISMATCH)
    printf( "GSDML-Error, Gsdmlfile doesn't match original filename\n");
  if ( EVEN(sts)) return sts;

  device_num = dev_data.device_num;
  gsdml->byte_order = dev_data.byte_order;
  time_ratio = dev_data.iocr_data[0]->reduction_ratio;
  if ( device_num > 0) {
    if ( device_num > gsdml->ApplicationProcess->DeviceAccessPointList->
	 DeviceAccessPointItem.size()) {
      printf( "GSDML-Read Error: Device number out of range: %d\n", dev_data.device_num);
      return 0;
    }
    device_item = gsdml->ApplicationProcess->DeviceAccessPointList->
      DeviceAccessPointItem[device_num-1];

    // Check text
    if ( strcmp( dev_data.device_text, (char *)device_item->ModuleInfo->Body.Name.p) != 0)
      printf( "GSDML-Read Error: Device name differs \"%s\" \"%s\"\n", dev_data.device_text,
	      (char *)device_item->ModuleInfo->Body.Name.p);
    device_read = 1;
  }
  return 1;
}

ItemPnBase::ItemPnBase( GsdmlAttrNav *attrnav, const char *item_name, const char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, void *attr_value_p, int attr_noedit,
	brow_tNode dest, flow_eDest dest_code) :
	value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
	min_limit(attr_min_limit), max_limit(attr_max_limit),
	noedit(attr_noedit), subgraph(0)
{

  type = attrnav_eItemType_PnBase;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

int ItemPnBase::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[200];
  int len;

  if ( size == 0)
    return 1;

  if ( !first_scan) {
    if ( size > (int) sizeof(old_value) && 
	 type_id == glow_eType_String &&
	 strlen((char *)p) < sizeof(old_value) && 
	 strcmp( (char *)p, old_value) == 0)
      // No change since last time
      return 1;
    else if ( memcmp( old_value, p, size) == 0)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;

  attrnav->attrvalue_to_string( type_id, p, buf, sizeof(buf), &len, NULL);
  brow_SetAnnotation( node, 1, buf, len);
  memcpy( old_value, p, min(size, (int) sizeof(old_value)));

  return 1;
}

void ItemPnBase::value_changed( GsdmlAttrNav *attrnav, char *value_str)
{
  char buffer[1024];
  int sts;

  sts = attrnav->attr_string_to_value( type_id, value_str, 
				       buffer, sizeof(buffer), size);
  if ( EVEN(sts)) {
    attrnav->message( 'E', "Syntax error");
    return;
  }
  
  if ( max_limit != 0 || min_limit != 0) {
    switch ( type_id) {
    case pwr_eType_Int32:
    case pwr_eType_UInt32:
      if ( *(int *)&buffer < min_limit ||
	   *(int *)&buffer > max_limit) {
	attrnav->message( 'E', "Min or maxvalue exceeded");
	return;
      }
      break;
    default: ;
    }
  }
  memcpy( value_p, buffer, size);
  attrnav->set_modified(1);
}

int ItemPn::close( GsdmlAttrNav *attrnav, double x, double y)
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
      case attrnav_eItemType_PnDevice:
      case attrnav_eItemType_PnParEnum:
      case attrnav_eItemType_PnModuleType:
      case attrnav_eItemType_PnModuleClass:
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
	break;
      case attrnav_eItemType_PnSlot:
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
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

ItemPnEnumValue::ItemPnEnumValue( GsdmlAttrNav *attrnav, const char *item_name, int item_num, 
	int item_type_id, void *attr_value_p, 
	brow_tNode dest, flow_eDest dest_code) :
	num(item_num), type_id(item_type_id), value_p(attr_value_p), first_scan(1)
{

  type = attrnav_eItemType_PnEnumValue;

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

int ItemPnEnumValue::scan( GsdmlAttrNav *attrnav, void *p)
{
  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;

  if ( *(int *)p == num)
    brow_SetRadiobutton( node, 0, 1);
  else
    brow_SetRadiobutton( node, 0, 0);

  old_value = *(int *) p;
  return 1;
}

ItemPnDevice::ItemPnDevice( GsdmlAttrNav *attrnav, const char *item_name, 
			    brow_tNode dest, flow_eDest dest_code):
  old_value(0), first_scan(1)
{
  type = attrnav_eItemType_PnDevice;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnDevice::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      //brow_RemoveAnnotPixmap( node, 1);
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    int idx = 0;
    new ItemPnEnumValue( attrnav, "No", idx++, pwr_eType_UInt32, 
			 &attrnav->device_num, node, flow_eDest_IntoLast);
    
    for ( unsigned int i = 0; i < attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->DeviceAccessPointItem.size(); i++) {
      char name[80];
      strncpy( name, (char *)attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->DeviceAccessPointItem[i]->ModuleInfo->Body.Name.p, sizeof(name));
      new ItemPnEnumValue( attrnav, name, idx++, pwr_eType_UInt32, 
			   &attrnav->device_num, node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnDevice::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[200];

  // Note, first scan is set the two first scans to detect load from data file
  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }

  if ( attrnav->device_confirm_active)
    return 1;


  if ( !first_scan) {
    if (old_value == 0) {
      GsdmlAttrNav::device_changed_ok( attrnav, (void *)old_value);
      return 1;
    }
    else {
      attrnav->wow->DisplayQuestion( attrnav, "Device Changed", 
		     "All configuration data will be lost when changing the device.\n"
		     "Do you really want to change the device ?",
		     GsdmlAttrNav::device_changed_ok, GsdmlAttrNav::device_changed_cancel, 
		     (void *)old_value);
      attrnav->device_confirm_active = 1;
      return 1;
    }
  }

  if ( *(int *)p == 0)
    strcpy( buf, "No");
  else {
    if ( attrnav->device_num == 0)
      strcpy( buf, "No");
    else {
      strncpy( buf, (char *)attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->
	       DeviceAccessPointItem[attrnav->device_num-1]->ModuleInfo->Body.Name.p, 
	       sizeof(buf));
    }
  }
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = attrnav->device_num;

  if ( first_scan)
    first_scan = 0;

  return 1;
}

ItemPnSlot::ItemPnSlot( GsdmlAttrNav *attrnav, const char *item_name,
			GsdmlSlotData *item_slotdata, brow_tNode dest, flow_eDest dest_code) :
  slotdata(item_slotdata), old_value(0), first_scan(1)
{
  type = attrnav_eItemType_PnSlot;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnSlot::open_children( GsdmlAttrNav *attrnav, double x, double y)
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

    new ItemPnModuleType( attrnav, "ModuleType", slotdata->slot_number, slotdata->slot_idx,
			  node, flow_eDest_IntoLast);
    if ( slotdata->module_enum_number != 0) {
      new ItemPnModuleClass( attrnav, "ModuleClass", slotdata, node, flow_eDest_IntoLast);

      gsdml_UseableModules *um = attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->
	DeviceAccessPointItem[attrnav->device_num-1]->UseableModules;
      if ( um) {
	unsigned int subslot_number = 0;
	unsigned int subslot_index = 0;
	gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->
	  ModuleItemRef[slotdata->module_enum_number-1]->Body.ModuleItemTarget.p;

	if ( mi && mi->ModuleInfo) {
	  new ItemPnModuleInfo( attrnav, "ModuleInfo", mi->ModuleInfo,
				node, flow_eDest_IntoLast);
	}
	if ( mi->VirtualSubmoduleList) {
	  char subslot_name[80];

	  for ( unsigned int i = 0; i < mi->VirtualSubmoduleList->VirtualSubmoduleItem.size();
		i++) {
	    if ( strcmp( mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.FixedInSubslots.str, "") == 0) {
	      // FixedInSubslots not supplied, default subslot number is 1 

	      if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 1)
		subslot_number = 1;
	      else
		subslot_number++;

	      sprintf( subslot_name, "Subslot %d", subslot_number);
	    
	      GsdmlSubslotData *ssd;
	      if ( slotdata->subslot_data.size() <= subslot_index) {
		ssd = new GsdmlSubslotData();
		ssd->subslot_number = subslot_number;
		ssd->subslot_idx = subslot_index;
		ssd->submodule_ident_number = 
		    mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		slotdata->subslot_data.push_back(ssd);
	      }
	      else {
		ssd = slotdata->subslot_data[subslot_index];
		ssd->subslot_idx = subslot_index;
		ssd->submodule_ident_number = 
		    mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		if ( ssd->subslot_number != subslot_number) {
		  ssd->subslot_number = subslot_number;
		  printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
		}
	      }
	      
	      new ItemPnSubslot( attrnav, subslot_name, ssd,
				 mi->VirtualSubmoduleList->VirtualSubmoduleItem[0],
				 node, flow_eDest_IntoLast);
		
	    }
	    else {
	      // FixedInSubslots supplied, create all fixed subslots

	      gsdml_Valuelist *vl = new gsdml_Valuelist(  mi->VirtualSubmoduleList->
			    VirtualSubmoduleItem[i]->Body.FixedInSubslots.str);
	      gsdml_ValuelistIterator iter( vl);

	      for ( unsigned int j = iter.begin(); j != iter.end(); j = iter.next()) {
		subslot_number = j;
		sprintf( subslot_name, "Subslot %d", subslot_number);
	    
		GsdmlSubslotData *ssd;
		if ( slotdata->subslot_data.size() <= subslot_index) {
		  ssd = new GsdmlSubslotData();
		  ssd->subslot_number = subslot_number;
		  ssd->subslot_idx = subslot_index;
		  ssd->submodule_ident_number = 
		    mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		  slotdata->subslot_data.push_back(ssd);
		}
		else {
		  ssd = slotdata->subslot_data[subslot_index];
		  ssd->subslot_idx = subslot_index;
		  ssd->submodule_ident_number = 
		    mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		  if ( ssd->subslot_number != subslot_number) {
		    ssd->subslot_number = subslot_number;
		    printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
		  }
		}
		
		new ItemPnSubslot( attrnav, subslot_name, ssd,
				   mi->VirtualSubmoduleList->VirtualSubmoduleItem[i],
				   node, flow_eDest_IntoLast);
		subslot_index++;
	      }
	      delete vl;
	    }
	  }
	}
      }
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnSlot::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[200];

  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }

  if ( *(int *)p == 0)
    strcpy( buf, "No");
  else {
    gsdml_UseableModules *um = attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->
      DeviceAccessPointItem[attrnav->device_num-1]->UseableModules;
    if ( !um)
      return 1;
    gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->
      ModuleItemRef[slotdata->module_enum_number-1]->Body.ModuleItemTarget.p;
    if ( !mi || !mi->ModuleInfo->Body.Name.p)
      return 1;
    strncpy( buf, (char *) mi->ModuleInfo->Body.Name.p, sizeof(buf));
  }
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = *(int *)p;

  if ( first_scan)
    first_scan = 0;

  return 1;
}

ItemPnSubslot::ItemPnSubslot( GsdmlAttrNav *attrnav, const char *item_name, 
			      GsdmlSubslotData *item_subslotdata, 
			      gsdml_VirtualSubmoduleItem *item_virtualsubmodule,
			      brow_tNode dest, flow_eDest dest_code) :
  subslotdata(item_subslotdata), virtualsubmodule(item_virtualsubmodule)
{
  type = attrnav_eItemType_PnSubslot;

  strcpy( name, item_name);

  if ( virtualsubmodule && virtualsubmodule->RecordDataList)
    parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnSubslot::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    unsigned int record_index = 0;
    if ( virtualsubmodule) {      
      gsdml_RecordDataList *rl = virtualsubmodule->RecordDataList;
      if ( rl) {
	GsdmlDataRecord *dr;

	for ( unsigned int j = 0; j < rl->ParameterRecordDataItem.size(); j++) {
	  if ( subslotdata->data_record.size() <= record_index) {
	    dr = new GsdmlDataRecord();
	    dr->record_idx = record_index;
	    subslotdata->data_record.push_back(dr);
	  }
	  else {
	    dr = subslotdata->data_record[record_index];
	    dr->record_idx = record_index;
	  }

	  new ItemPnParRecord( attrnav, "", rl->ParameterRecordDataItem[j], dr,
			       node, flow_eDest_IntoLast);
	  record_index++;
	}
      }
    }

    if ( attrnav->viewio && virtualsubmodule->IOData) {
      new ItemPnIOData( attrnav, "IOData", virtualsubmodule->IOData,
			node, flow_eDest_IntoLast);
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnDAP::ItemPnDAP( GsdmlAttrNav *attrnav, const char *item_name, GsdmlSlotData *item_slotdata,
		      brow_tNode dest, flow_eDest dest_code) : slotdata(item_slotdata)
{
  type = attrnav_eItemType_PnDAP;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnDAP::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    if ( attrnav->device_num) {
      gsdml_DeviceAccessPointItem *mi = attrnav->device_item;
      unsigned int subslot_index = 0;
      unsigned int subslot_number = 0;

      if ( attrnav->device_item->ModuleInfo) {
	new ItemPnModuleInfo( attrnav, "ModuleInfo", attrnav->device_item->ModuleInfo,
			      node, flow_eDest_IntoLast);
      }

      if ( mi->VirtualSubmoduleList) {
	char subslot_name[80];

	for ( unsigned int i = 0; i < mi->VirtualSubmoduleList->VirtualSubmoduleItem.size();
	      i++) {
	  if ( strcmp( mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.FixedInSubslots.str, "") == 0) {
	    // FixedInSubslots not supplied, default subslot number is 1 

	    if ( mi->VirtualSubmoduleList->VirtualSubmoduleItem.size() == 1)
	      subslot_number = 1;
	    else
	      subslot_number++;
	    
	    sprintf( subslot_name, "Subslot %d", subslot_number);
	    
	    GsdmlSubslotData *ssd;
	    if ( slotdata->subslot_data.size() <= subslot_index) {
	      ssd = new GsdmlSubslotData();
	      ssd->subslot_number = subslot_number;
	      ssd->subslot_idx = subslot_index;
	      ssd->submodule_ident_number = 
		mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
	      slotdata->subslot_data.push_back(ssd);
	    }
	    else {
	      ssd = slotdata->subslot_data[subslot_index];
	      ssd->subslot_idx = subslot_index;
	      ssd->submodule_ident_number = 
		mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
	      if ( ssd->subslot_number != subslot_number) {
		ssd->subslot_number = subslot_number;
		printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
	      }
	    }
	    
	    new ItemPnSubslot( attrnav, subslot_name, ssd,
			       mi->VirtualSubmoduleList->VirtualSubmoduleItem[0],
			       node, flow_eDest_IntoLast);
		
	    subslot_index++;
	  }
	  else {
	    // FixedInSubslots supplied, create all fixed subslots

	    gsdml_Valuelist *vl = new gsdml_Valuelist(  mi->VirtualSubmoduleList->
			    VirtualSubmoduleItem[i]->Body.FixedInSubslots.str);
	    gsdml_ValuelistIterator iter( vl);

	    for ( unsigned int j = iter.begin(); j != iter.end(); j = iter.next()) {
	      subslot_number = j;
	      sprintf( subslot_name, "Subslot %d", subslot_number);
	      
	      GsdmlSubslotData *ssd;
	      if ( slotdata->subslot_data.size() <= subslot_index) {
		ssd = new GsdmlSubslotData();
		ssd->subslot_number = subslot_number;
		ssd->subslot_idx = subslot_index;
		ssd->submodule_ident_number = 
		  mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		slotdata->subslot_data.push_back(ssd);
	      }
	      else {
		ssd = slotdata->subslot_data[subslot_index];
		ssd->subslot_idx = subslot_index;
		ssd->submodule_ident_number = 
		    mi->VirtualSubmoduleList->VirtualSubmoduleItem[i]->Body.SubmoduleIdentNumber;
		if ( ssd->subslot_number != subslot_number) {
		  ssd->subslot_number = subslot_number;
		  printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
		}
	      }
		
	      new ItemPnSubslot( attrnav, subslot_name, ssd,
				 mi->VirtualSubmoduleList->VirtualSubmoduleItem[i],
				 node, flow_eDest_IntoLast);
	      subslot_index++;
	    }
	    delete vl;
	  }
	}
      }
      if ( attrnav->device_item->SystemDefinedSubmoduleList) {
	if ( attrnav->device_item->SystemDefinedSubmoduleList->InterfaceSubmoduleItem) {
	  gsdml_InterfaceSubmoduleItem *ii = attrnav->device_item->SystemDefinedSubmoduleList->
	    InterfaceSubmoduleItem;
	  char name[200] = "Interface ";

	  if ( attrnav->device_item->SubslotList) {
	    for ( unsigned int i = 0; i < attrnav->device_item->SubslotList->SubslotItem.size(); i++) {
	      if ( ii->Body.SubslotNumber == 
		   attrnav->device_item->SubslotList->SubslotItem[i]->Body.SubslotNumber) {
		strncat( name, (char *)attrnav->device_item->SubslotList->SubslotItem[i]->Body.TextId.p, sizeof(name)-strlen(name));
		strncat( name, " ", sizeof(name)-strlen(name));
		sprintf( &name[strlen(name)], "%hu", ii->Body.SubslotNumber);
	      }
	    }
	  }

	  GsdmlSubslotData *ssd;
	  if ( slotdata->subslot_data.size() <= subslot_index) {
	    ssd = new GsdmlSubslotData();
	    ssd->subslot_number = ii->Body.SubslotNumber;
	    ssd->subslot_idx = subslot_index;
	    ssd->submodule_ident_number = ii->Body.SubmoduleIdentNumber;
	    slotdata->subslot_data.push_back(ssd);
	  }
	  else {
	    ssd = slotdata->subslot_data[subslot_index];
	    ssd->subslot_idx = subslot_index;
	    ssd->submodule_ident_number = ii->Body.SubmoduleIdentNumber;
	    if ( ssd->subslot_number != ii->Body.SubslotNumber) {
	      ssd->subslot_number = ii->Body.SubslotNumber;
	      printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
	    }
	  }

	  new ItemPnInterfaceSubmodule( attrnav, name, ii, ssd, node, flow_eDest_IntoLast);
 	  subslot_index++;
	}
        for ( unsigned int j = 0; 
	      j < attrnav->device_item->SystemDefinedSubmoduleList->PortSubmoduleItem.size(); 
	      j++) {
	  gsdml_PortSubmoduleItem *pi = attrnav->device_item->SystemDefinedSubmoduleList->
	    PortSubmoduleItem[j];
	  char name[80] = "Port ";
	  
	  for ( unsigned int i = 0; i < attrnav->device_item->SubslotList->SubslotItem.size(); i++) {
	    if ( pi->Body.SubslotNumber == attrnav->device_item->SubslotList->SubslotItem[i]->Body.SubslotNumber) {
		strncat( name, (char *)attrnav->device_item->SubslotList->SubslotItem[i]->Body.TextId.p, sizeof(name)-strlen(name));
		strncat( name, " ", sizeof(name)-strlen(name));
		sprintf( &name[strlen(name)], "%hu", pi->Body.SubslotNumber);
	    }
	  }

	  GsdmlSubslotData *ssd;
	  if ( slotdata->subslot_data.size() <= subslot_index) {
	    ssd = new GsdmlSubslotData();
	    ssd->subslot_number = pi->Body.SubslotNumber;
	    ssd->subslot_idx = subslot_index;
	    ssd->submodule_ident_number = pi->Body.SubmoduleIdentNumber;
	    slotdata->subslot_data.push_back(ssd);
	  }
	  else {
	    ssd = slotdata->subslot_data[subslot_index];
	    ssd->subslot_idx = subslot_index;
	    ssd->submodule_ident_number = pi->Body.SubmoduleIdentNumber;
	    if ( ssd->subslot_number != pi->Body.SubslotNumber) {
	      ssd->subslot_number = pi->Body.SubslotNumber;
	      printf( "GSML-Error, datafile corrupt, unexpected subslot number\n");
	    }
	  }

	  new ItemPnPortSubmodule( attrnav, name, pi, ssd, node, flow_eDest_IntoLast);
 	  subslot_index++;
	}
      }
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnNetwork::ItemPnNetwork( GsdmlAttrNav *attrnav, const char *item_name, 
			      brow_tNode dest, flow_eDest dest_code)
{
  type = attrnav_eItemType_PnNetwork;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnNetwork::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    void *p = (void *) attrnav->dev_data.device_name;
    new ItemPnBase( attrnav, "DeviceName", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(attrnav->dev_data.device_name), 0, 0,
		    p, 0, node, flow_eDest_IntoLast);

    p = (void *) attrnav->dev_data.ip_address;
    new ItemPnBase( attrnav, "IP Address", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(attrnav->dev_data.ip_address), 0, 0,
		    p, 0, node, flow_eDest_IntoLast);

    p = (void *) attrnav->dev_data.subnet_mask;
    new ItemPnBase( attrnav, "Subnet Mask", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(attrnav->dev_data.subnet_mask), 0, 0,
		    p, 0, node, flow_eDest_IntoLast);

    p = (void *) attrnav->dev_data.mac_address;
    new ItemPnBase( attrnav, "MAC Address", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(attrnav->dev_data.mac_address), 0, 0,
		    p, 0, node, flow_eDest_IntoLast);

    if ( attrnav->device_item && attrnav->device_item->SystemDefinedSubmoduleList &&
	 attrnav->device_item->SystemDefinedSubmoduleList->InterfaceSubmoduleItem &&
	 attrnav->device_item->SystemDefinedSubmoduleList->InterfaceSubmoduleItem->ApplicationRelations &&
	 attrnav->device_item->SystemDefinedSubmoduleList->InterfaceSubmoduleItem->ApplicationRelations->
	 TimingProperties) {
      p = (void *) &attrnav->time_ratio;
      new ItemPnEnumTimeRatio( attrnav, "TimeRatio", 
			       attrnav->device_item->SystemDefinedSubmoduleList->InterfaceSubmoduleItem,
			       p, node, flow_eDest_IntoLast);
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnDeviceInfo::ItemPnDeviceInfo( GsdmlAttrNav *attrnav, const char *item_name, 
				    brow_tNode dest, flow_eDest dest_code)
{
  type = attrnav_eItemType_PnDeviceInfo;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnDeviceInfo::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    void *p = (void *) attrnav->gsdml->DeviceIdentity->Body.VendorName;
    new ItemPnBase( attrnav, "Vendor", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1,
		    node, flow_eDest_IntoLast);
    
    p = (void *) attrnav->gsdml->DeviceIdentity->Body.InfoText.p;
    new ItemPnBase( attrnav, "Text", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1,
		    node, flow_eDest_IntoLast);

    p = (void *) attrnav->gsdml->DeviceFunction->Body.MainFamily;
    new ItemPnBase( attrnav, "MainFamily", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1,
		    node, flow_eDest_IntoLast);

    p = (void *) attrnav->gsdml->DeviceFunction->Body.ProductFamily;
    new ItemPnBase( attrnav, "ProductFamily", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1,
		    node, flow_eDest_IntoLast);
    
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnInterfaceSubmodule::ItemPnInterfaceSubmodule( GsdmlAttrNav *attrnav, const char *item_name, 
						    gsdml_InterfaceSubmoduleItem *item_ii,
						    GsdmlSubslotData *item_subslotdata,
						    brow_tNode dest, flow_eDest dest_code) :
  ii(item_ii), subslotdata(item_subslotdata)
{
  type = attrnav_eItemType_PnInterfaceSubmodule;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnInterfaceSubmodule::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    char *p = (char *)ii->Body.TextId.p;
    new ItemPnBase( attrnav, "Text", "LocalGsdmlAttr", 
		  pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    
    p = ii->Body.SupportedRT_Class;
    new ItemPnBase( attrnav, "SupportedRT_Class", "LocalGsdmlAttr", 
		  pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    
    p = ii->Body.SupportedRT_Classes;
    new ItemPnBase( attrnav, "SupportedRT_Classes", "LocalGsdmlAttr", 
		  pwr_eType_String, sizeof(pwr_tString80), 0, 0,
 		  p, 1, node, flow_eDest_IntoLast);
    
    p = (char *) &ii->Body.IsochroneModeSupported;
    new ItemPnBase( attrnav, "IsochroneModeSupported", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    if ( *(pwr_tBoolean *)p) {
      p = ii->Body.IsochroneModeInRT_Classes;
      new ItemPnBase( attrnav, "IsochroneModeSupported", "LocalGsdmlAttr", 
		      pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		      p, 1, node, flow_eDest_IntoLast);
    }    

    p = ii->Body.SupportedProtocols;
    new ItemPnBase( attrnav, "SupportedProtocols", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = ii->Body.SupportedMibs;
    new ItemPnBase( attrnav, "SupportedMibs", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (char *) &ii->Body.NetworkComponentDiagnosisSupported;
    new ItemPnBase( attrnav, "NetworkComponentDiagnosisSupported", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    gsdml_RecordDataList *rl = ii->RecordDataList;
    if ( rl) {
      unsigned int record_index = 0;
      GsdmlDataRecord *dr;

      for ( unsigned int j = 0; j < rl->ParameterRecordDataItem.size(); j++) {

	if ( subslotdata->data_record.size() <= record_index) {
	  dr = new GsdmlDataRecord();
	  dr->record_idx = record_index;
	  subslotdata->data_record.push_back(dr);
	}
	else {
	  dr = subslotdata->data_record[record_index];
	  dr->record_idx = record_index;
	}
	
	new ItemPnParRecord( attrnav, "", rl->ParameterRecordDataItem[j], dr,
			     node, flow_eDest_IntoLast);
	record_index++;
      }
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnPortSubmodule::ItemPnPortSubmodule( GsdmlAttrNav *attrnav, const char *item_name, 
					  gsdml_PortSubmoduleItem *item_pi,
					  GsdmlSubslotData *item_subslotdata,			    
					  brow_tNode dest, flow_eDest dest_code) :
  pi(item_pi), subslotdata(item_subslotdata)
{
  type = attrnav_eItemType_PnPortSubmodule;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnPortSubmodule::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    char *p = (char *)pi->Body.TextId.p;
    new ItemPnBase( attrnav, "Text", "LocalGsdmlAttr", 
		  pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    
    p = pi->Body.MAUType;
    new ItemPnBase( attrnav, "MAUType", "LocalGsdmlAttr", 
		  pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    

    p = (char *) &pi->Body.MaxPortTxDelay;
    new ItemPnBase( attrnav, "MaxPortTxDelay", "LocalGsdmlAttr", 
		  pwr_eType_UInt16, sizeof(pwr_tUInt16), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    
    p = (char *) &pi->Body.MaxPortRxDelay;
    new ItemPnBase( attrnav, "MaxPortRxDelay", "LocalGsdmlAttr", 
		  pwr_eType_UInt16, sizeof(pwr_tUInt16), 0, 0,
		  p, 1, node, flow_eDest_IntoLast);
    
    p = (char *) &pi->Body.PortDeactivationSupported;
    new ItemPnBase( attrnav, "PortDeactivationSupported", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = pi->Body.LinkStateDiagnosisCapability;
    new ItemPnBase( attrnav, "LinkStateDiagnosisCapability", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(pwr_tString80), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (char *) &pi->Body.PowerBudgetControlSupported;
    new ItemPnBase( attrnav, "PowerBudgetControlSupported", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (char *) &pi->Body.SupportsRingportConfig;
    new ItemPnBase( attrnav, "SupportsRingportConfig", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (char *) &pi->Body.IsDefaultRingport;
    new ItemPnBase( attrnav, "IsDefaultRingport", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(pwr_tBoolean), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    gsdml_RecordDataList *rl = pi->RecordDataList;
    if ( rl) {
      GsdmlDataRecord *dr;
      unsigned int record_index = 0;

      for ( unsigned int j = 0; j < rl->ParameterRecordDataItem.size(); j++) {
	if ( subslotdata->data_record.size() <= record_index) {
	  dr = new GsdmlDataRecord();
	  dr->record_idx = record_index;
	  subslotdata->data_record.push_back(dr);
	}
	else {
	  dr = subslotdata->data_record[record_index];
	  dr->record_idx = record_index;
	}
	
	new ItemPnParRecord( attrnav, "", rl->ParameterRecordDataItem[j], dr,
			     node, flow_eDest_IntoLast);
	record_index++;
      }
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnModuleInfo::ItemPnModuleInfo( GsdmlAttrNav *attrnav, const char *item_name, 
			    gsdml_ModuleInfo *item_info,
			    brow_tNode dest, flow_eDest dest_code):
  info(item_info)
{
  type = attrnav_eItemType_PnModuleInfo;

  strcpy( name, item_name);
  parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_object, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnModuleInfo::open_children( GsdmlAttrNav *attrnav, double x, double y)
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

    new ItemPnBase( attrnav, "Name", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		    info->Body.Name.p, 1, node, flow_eDest_IntoLast);
    new ItemPnBase( attrnav, "Text", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		    info->Body.InfoText.p, 1, node, flow_eDest_IntoLast);
    if ( strcmp( info->Body.VendorName, "") != 0)
      new ItemPnBase( attrnav, "VendorName", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		      info->Body.VendorName, 1, node, flow_eDest_IntoLast);
    if ( strcmp( info->Body.OrderNumber, "") != 0)
      new ItemPnBase( attrnav, "OrderNumber", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		      info->Body.OrderNumber, 1, node, flow_eDest_IntoLast);
    if ( strcmp( info->Body.HardwareRelease, "") != 0)
      new ItemPnBase( attrnav, "HardwareRelease", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		      info->Body.HardwareRelease, 1, node, flow_eDest_IntoLast);
    if ( strcmp( info->Body.SoftwareRelease, "") != 0)
      new ItemPnBase( attrnav, "SoftwareRelease", "LocalGsdmlAttr", pwr_eType_String, 32, 0, 0,
		      info->Body.SoftwareRelease, 1, node, flow_eDest_IntoLast);
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnModuleType::ItemPnModuleType( GsdmlAttrNav *attrnav, const char *item_name, 
				    int item_slot_number, int item_slot_idx,
				    brow_tNode dest, flow_eDest dest_code):
  slot_number(item_slot_number), slot_idx(item_slot_idx), old_value(0), first_scan(1)
{
  type = attrnav_eItemType_PnModuleType;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnModuleType::open_children( GsdmlAttrNav *attrnav, double x, double y)
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
    new ItemPnEnumValue( attrnav, "No", idx++, pwr_eType_UInt32, 
			 &attrnav->dev_data.slot_data[slot_idx]->module_enum_number, 
			 node, flow_eDest_IntoLast);
    
    gsdml_UseableModules *um = attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->DeviceAccessPointItem[attrnav->device_num-1]->UseableModules;
    if ( !um)
      return 1;
    for ( unsigned int i = 0; i < um->ModuleItemRef.size(); i++) {
      if ( um->ModuleItemRef[i]->Body.AllowedInSlots.list &&
	   um->ModuleItemRef[i]->Body.AllowedInSlots.list->in_list(slot_number)) {
	char mname[160] = "ModuleName";
	gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->ModuleItemRef[i]->Body.ModuleItemTarget.p;
	if ( !mi || !mi->ModuleInfo->Body.Name.p)
	  continue;
	strncpy( mname, (char *) mi->ModuleInfo->Body.Name.p, sizeof(mname));
	new ItemPnEnumValue( attrnav, mname, idx, pwr_eType_UInt32, 
			     &attrnav->dev_data.slot_data[slot_idx]->module_enum_number, 
			     node, flow_eDest_IntoLast);
      }
      else if ( um->ModuleItemRef[i]->Body.FixedInSlots.list &&
		um->ModuleItemRef[i]->Body.FixedInSlots.list->in_list(slot_number)) {
	char mname[160] = "ModuleName";
	gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->ModuleItemRef[i]->Body.ModuleItemTarget.p;
	if ( !mi || !mi->ModuleInfo->Body.Name.p)
	  continue;
	strncpy( mname, (char *) mi->ModuleInfo->Body.Name.p, sizeof(mname));
	new ItemPnEnumValue( attrnav, mname, idx, pwr_eType_UInt32, 
			     &attrnav->dev_data.slot_data[slot_idx]->module_enum_number, 
			     node, flow_eDest_IntoLast);
      }
      idx++;
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnModuleType::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[200];

  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }

  if ( *(int *)p == 0)
    strcpy( buf, "No");
  else {
    gsdml_UseableModules *um = attrnav->gsdml->ApplicationProcess->DeviceAccessPointList->
      DeviceAccessPointItem[attrnav->device_num-1]->UseableModules;
    if ( !um)
      return 1;
    gsdml_ModuleItem *mi = (gsdml_ModuleItem *)um->
      ModuleItemRef[attrnav->dev_data.slot_data[slot_idx]->module_enum_number-1]->Body.ModuleItemTarget.p;
    if ( !mi || !mi->ModuleInfo->Body.Name.p)
      return 1;
    strncpy( buf, (char *) mi->ModuleInfo->Body.Name.p, sizeof(buf));
  }
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = *(int *)p;

  if ( !first_scan) {
    int sts;
    brow_tObject parentnode;
    sts = brow_GetParent( attrnav->brow->ctx, node, &parentnode);
    if ( EVEN(sts)) return 1;
	
    ItemPnSlot *parentitem;
    brow_GetUserData( parentnode, (void **)&parentitem);

    // Note, this object is deleted here !
    parentitem->close( attrnav, 0, 0);

    // Remove old subslot data
    if ( parentitem->slotdata)
      parentitem->slotdata->slot_reset();

    parentitem->open_children( attrnav, 0, 0);

    brow_SelectClear( attrnav->brow->ctx);
    brow_SetInverse( parentnode, 1);
    brow_SelectInsert( attrnav->brow->ctx, parentnode);
  }
  else
    first_scan = 0;

  return 1;
}

ItemPnParRecord::ItemPnParRecord( GsdmlAttrNav *attrnav, const char *item_name, 
				  gsdml_ParameterRecordDataItem *item_par_record,
				  GsdmlDataRecord *item_datarecord, brow_tNode dest, 
				  flow_eDest dest_code) :
  par_record(item_par_record), datarecord(item_datarecord)
{
  type = attrnav_eItemType_PnParRecord;
  if ( par_record->Ref.size() == 0)
    parent = 0;
  else
    parent = 1;
  strncpy( name, (char *)par_record->Body.Name.p, sizeof(name));

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, name, strlen(name));


  if ( !datarecord->data) {
    datarecord->data = (unsigned char *) calloc( 1, par_record->Body.Length);
    datarecord->data_length = par_record->Body.Length;
    datarecord->index = par_record->Body.Index;
    datarecord->transfer_sequence = par_record->Body.TransferSequence;

    attrnav->gsdml->set_par_record_default( datarecord->data, par_record->Body.Length, 
    					    par_record);

    for ( unsigned int i = 0; i < par_record->Const.size(); i++) {
      void *const_data;
      int len;
      gsdml_Const *con = par_record->Const[i];

      attrnav->gsdml->ostring_to_data( (unsigned char **)&const_data, con->Body.Data, 
				       par_record->Body.Length - con->Body.ByteOffset, &len);
      if ( len > (int)(par_record->Body.Length - con->Body.ByteOffset))
	len = par_record->Body.Length - con->Body.ByteOffset;
      memcpy( (char *)datarecord->data + con->Body.ByteOffset, const_data, len);
      free( const_data);
    }
  }
}


int ItemPnParRecord::open_children( GsdmlAttrNav *attrnav, double x, double y)
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

    for ( unsigned int i = 0; i < par_record->Ref.size(); i++) {
      gsdml_eValueDataType datatype;

      if ( !par_record->Ref[i]->Body.Visible)
	continue;

      attrnav->gsdml->string_to_value_datatype( par_record->Ref[i]->Body.DataType, &datatype);
      switch ( datatype) {
      case gsdml_eValueDataType_:
	break;
      case gsdml_eValueDataType_Bit:
      case gsdml_eValueDataType_BitArea:
	new ItemPnParEnum( attrnav, "", par_record->Ref[i], datatype, datarecord->data, node, 
			   flow_eDest_IntoLast);
	break;
      case gsdml_eValueDataType_Integer8:
      case gsdml_eValueDataType_Unsigned8:
	if ( strcmp( par_record->Ref[i]->Body.ValueItemTarget.ref, "") == 0)
	  new ItemPnParValue( attrnav, "", par_record->Ref[i], datatype, datarecord->data, node, 
			      flow_eDest_IntoLast);
	else
	  new ItemPnParEnum( attrnav, "", par_record->Ref[i], datatype, datarecord->data, node, 
			     flow_eDest_IntoLast);
	break;
      default:
	new ItemPnParValue( attrnav, "", par_record->Ref[i], datatype, datarecord->data, node, 
			    flow_eDest_IntoLast);
      }
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnParValue::ItemPnParValue( GsdmlAttrNav *attrnav, const char *item_name, 
				gsdml_Ref *item_value_ref, gsdml_eValueDataType item_datatype, 
				unsigned char *item_data, brow_tNode dest, flow_eDest dest_code) :
  value_ref(item_value_ref), datatype( item_datatype), data(item_data), first_scan(1),
  noedit(1)
{
  type = attrnav_eItemType_PnParValue;
  strncpy( name, (char *)value_ref->Body.TextId.p, sizeof(name));
  byte_offset = value_ref->Body.ByteOffset;
  noedit = !value_ref->Body.Changeable;

  switch ( datatype) {
  case gsdml_eValueDataType_Integer8:
  case gsdml_eValueDataType_Unsigned8:
    size = 1;
    break;
  case gsdml_eValueDataType_Integer16:
  case gsdml_eValueDataType_Unsigned16:
    size = 2;
    break;
  case gsdml_eValueDataType_Integer32:
  case gsdml_eValueDataType_Unsigned32:
  case gsdml_eValueDataType_Float32:
    size = 4;
    break;
  case gsdml_eValueDataType_Integer64:
  case gsdml_eValueDataType_Unsigned64:
  case gsdml_eValueDataType_Float64:
    size = 8;
    break;
  case gsdml_eValueDataType_VisibleString:
  case gsdml_eValueDataType_OctetString:
    size = value_ref->Body.Length;
    break;
  case gsdml_eValueDataType_Date:
  case gsdml_eValueDataType_TimeOfDayWithDate:
  case gsdml_eValueDataType_TimeOfDayWithoutDate:
  case gsdml_eValueDataType_TimeDiffWithDate:
  case gsdml_eValueDataType_TimeDiffWithoutDate:
  case gsdml_eValueDataType_NetworkTime:
  case gsdml_eValueDataType_NetworkTimeDiff:
    size = 8;
    break;
  default:
    size = 0;
  }
  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, name, strlen(name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnParValue::scan( GsdmlAttrNav *attrnav, void *p)
{
  if ( !first_scan) {
    if ( memcmp( &data[byte_offset], old_value, size) == 0)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;
  
  char buf[80];
  attrnav->gsdml->datavalue_to_string( datatype, &data[byte_offset], size, buf, sizeof(buf));
  brow_SetAnnotation( node, 1, buf, strlen(buf));

  memcpy( old_value, &data[byte_offset], size);
  return 1;
}

void ItemPnParValue::value_changed( GsdmlAttrNav *attrnav, char *value_str)
{
  int sts;

  unsigned char *buf = (unsigned char *)calloc( 1, size);

  sts = attrnav->gsdml->string_to_datavalue( datatype, buf,
					     size, value_str);
  if ( sts == PB__NYI) {
    attrnav->message('E', "Not yet implemented");
    return;
  }
  else if ( EVEN(sts)) {
    attrnav->message('E', "Input syntax error");
    return;
  }  

  // Check allowed values
  switch ( datatype) {
  case gsdml_eValueDataType_Integer8: {
    char val = *(char *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_SValuelist *vl = new gsdml_SValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Integer16: {
    short val = *(short *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_SValuelist *vl = new gsdml_SValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Integer32: {
    int val = *(int *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_SValuelist *vl = new gsdml_SValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Integer64: {
    pwr_tInt64 val = *(pwr_tInt64 *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_SValuelist *vl = new gsdml_SValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Unsigned8: {
    unsigned char val = *(unsigned char *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_Valuelist *vl = new gsdml_Valuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Unsigned16: {
    unsigned short val = *(unsigned short *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_Valuelist *vl = new gsdml_Valuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Unsigned32: {
    unsigned int val = *(unsigned int *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_Valuelist *vl = new gsdml_Valuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Unsigned64: {
    pwr_tUInt64 val = *(pwr_tUInt64 *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_Valuelist *vl = new gsdml_Valuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Float32: {
    float val = *(float *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_FValuelist *vl = new gsdml_FValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  case gsdml_eValueDataType_Float64: {
    double val = *(double *)buf;
    if ( strcmp( value_ref->Body.AllowedValues, "") != 0) {
      gsdml_FValuelist *vl = new gsdml_FValuelist( value_ref->Body.AllowedValues);
      if ( ODD(vl->sts())) {
	
	if ( !vl->in_list( val)) {
	  attrnav->message('E', "Value not allowed");
	  return;
	}
      }
      delete vl;
    }
    break;
  }
  default: ;
  }

  memcpy( &data[byte_offset], buf, size);

  free( buf);
  attrnav->set_modified(1);
}

ItemPnParEnum::ItemPnParEnum( GsdmlAttrNav *attrnav, const char *item_name, 
			      gsdml_Ref *item_value_ref, gsdml_eValueDataType item_datatype, 
			      unsigned char *item_data, brow_tNode dest, flow_eDest dest_code) :
  value_ref(item_value_ref), datatype(item_datatype), data(item_data), mask(0), old_value(0),
  first_scan(1), noedit(1)
{
  type = attrnav_eItemType_PnParEnum;
  strncpy( name, (char *)value_ref->Body.TextId.p, sizeof(name));
  byte_offset = value_ref->Body.ByteOffset;
  bit_offset = value_ref->Body.BitOffset;
  switch ( datatype) {
  case gsdml_eValueDataType_Bit:
    bit_length = 1;
    break;
  case gsdml_eValueDataType_BitArea:
    bit_length = value_ref->Body.BitLength;
    break;
  case gsdml_eValueDataType_Integer8:
  case gsdml_eValueDataType_Unsigned8:
    bit_length = 8;
    break;
  default: ;
  }
  for ( unsigned int i = 0; i < bit_length; i++)
    mask |= (mask << 1) | 1; 
  mask <<= bit_offset;
  noedit = !value_ref->Body.Changeable;

  // Get the values and corresponding texts
  gsdml_ValueItem *vi = (gsdml_ValueItem *)value_ref->Body.ValueItemTarget.p;
    
  gsdml_Valuelist *allowed_values = 0; 
  if ( strcmp( value_ref->Body.AllowedValues, "") != 0)
    allowed_values = new gsdml_Valuelist( value_ref->Body.AllowedValues);

  if ( vi && vi->Assignments) {
    for ( unsigned int i = 0; i < vi->Assignments->Assign.size(); i++) {
      ParEnumValue eval;
      int num;
      num = sscanf( vi->Assignments->Assign[i]->Body.Content, "%u", &eval.value);
      if ( num != 1)
	continue;
      if ( allowed_values && !allowed_values->in_list( eval.value))
	continue;

      strncpy( eval.text, (char *)vi->Assignments->Assign[i]->Body.TextId.p, sizeof(eval.text));
      eval.value <<= bit_offset;

      values.push_back( eval);
    }
  }
  if ( allowed_values)
    delete allowed_values;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
  brow_SetAnnotation( node, 0, name, strlen(name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}


int ItemPnParEnum::scan( GsdmlAttrNav *attrnav, void *p)
{
  if ( !first_scan) {
    if ( (*(unsigned int *)&data[byte_offset] & mask) == old_value)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;
  
  unsigned int value = *(unsigned int *)&data[byte_offset] & mask;
  for ( unsigned int i = 0; i < values.size(); i++) {
    if ( values[i].value == value) {
      brow_SetAnnotation( node, 1, values[i].text, strlen(values[i].text));
      break;
    }
  }

  old_value = value;
  return 1;
}

int ItemPnParEnum::open_children( GsdmlAttrNav *attrnav, double x, double y)
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

    gsdml_ValueItem *vi = (gsdml_ValueItem *)value_ref->Body.ValueItemTarget.p;
    
    if ( vi && vi->Assignments) {
      for ( unsigned int i = 0; i < values.size(); i++) {
	new ItemPnParEnumBit( attrnav, values[i].text, datatype, data, 
			      byte_offset, values[i].value, mask, noedit,
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

ItemPnParEnumBit::ItemPnParEnumBit( GsdmlAttrNav *attrnav, const char *item_name, 
				    gsdml_eValueDataType item_datatype, 
				    unsigned char *item_data, unsigned int item_byte_offset, 
				    unsigned int item_value, unsigned int item_mask,
				    int item_noedit, brow_tNode dest, flow_eDest dest_code) :
  datatype(item_datatype), data(item_data),
  byte_offset(item_byte_offset), value(item_value), mask(item_mask), first_scan(1),
  old_value(0), noedit(item_noedit)
{
  type = attrnav_eItemType_PnParEnumBit;
  strncpy( name, (char *)item_name, sizeof(name));

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_enum, 
		   dest, dest_code, (void *) this, 1, &node);
  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);

  brow_SetAnnotation( node, 0, name, strlen(name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnParEnumBit::scan( GsdmlAttrNav *attrnav, void *p)
{
  if ( !first_scan) {
    if ( (*(unsigned int *)&data[byte_offset] & mask) == old_value)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;
  
  unsigned int current_value = *(unsigned int *)&data[byte_offset] & mask;
  if ( current_value == value)
    brow_SetRadiobutton( node, 0, 1);
  else
    brow_SetRadiobutton( node, 0, 0);

  old_value = current_value;
  return 1;
}

void ItemPnParEnumBit::update( GsdmlAttrNav *attrnav)
{
  if ( noedit) {
    attrnav->message( 'E', "Parameter is not changeable");
    return;
  }  
  *(unsigned int *)&data[byte_offset] &= ~mask;
  *(unsigned int *)&data[byte_offset] |= value;
  attrnav->set_modified( 1);	    
}

ItemPnModuleClass::ItemPnModuleClass( GsdmlAttrNav *attrnav, const char *item_name, 
				    GsdmlSlotData *item_slotdata,
				    brow_tNode dest, flow_eDest dest_code):
  slotdata(item_slotdata), first_scan(1)
{
  type = attrnav_eItemType_PnModuleClass;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnModuleClass::open_children( GsdmlAttrNav *attrnav, double x, double y)
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

    for ( int i = 0; attrnav->gsdml->module_classlist[i].cid; i++) {
      new ItemPnEnumValue( attrnav, attrnav->gsdml->module_classlist[i].name, 
			   attrnav->gsdml->module_classlist[i].cid, pwr_eType_UInt32, 
			   &slotdata->module_class, node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnModuleClass::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[200];

  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }

  if ( *(int *)p == 0)
    strcpy( buf, "No");
  else {
    int found = 0;

    int i;
    for ( i = 0; attrnav->gsdml->module_classlist[i].cid; i++) {
      if ( attrnav->gsdml->module_classlist[i].cid == *(pwr_tCid *)p) {
	found = 1;
	break;
      }
    }
    if ( !found)
      strcpy( buf, "Unknown class");
    else
      strcpy( buf, attrnav->gsdml->module_classlist[i].name);
  }
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = *(int *)p;      

  if ( first_scan)
    first_scan = 0;
  return 1;
}

ItemPnIOData::ItemPnIOData( GsdmlAttrNav *attrnav, const char *item_name, 
			    gsdml_IOData *item_iodata,
			    brow_tNode dest, flow_eDest dest_code) :
  iodata(item_iodata)
{
  type = attrnav_eItemType_PnIOData;

  strcpy( name, item_name);

  if ( iodata->Input || iodata->Output)
    parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnIOData::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

#if 0
    // These are always 1 in this release
    void *p = (void *) &iodata->Body.IOPS_Length;
    new ItemPnBase( attrnav, "IOPS_Length", "LocalGsdmlAttr", 
		    pwr_eType_UInt8, sizeof(iodata->Body.IOPS_Length), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (void *) &iodata->Body.IOCS_Length;
    new ItemPnBase( attrnav, "IOCS_Length", "LocalGsdmlAttr", 
		    pwr_eType_UInt8, sizeof(iodata->Body.IOCS_Length), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);
#endif
    if ( iodata->Input) {
      new ItemPnInput( attrnav, "Input", iodata->Input,
		       node, flow_eDest_IntoLast);
    }
    if ( iodata->Output) {
      new ItemPnOutput( attrnav, "Output", iodata->Output,
		       node, flow_eDest_IntoLast);
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnInput::ItemPnInput( GsdmlAttrNav *attrnav, const char *item_name, 
			  gsdml_Input *item_input,
			  brow_tNode dest, flow_eDest dest_code) :
  input(item_input)
{
  type = attrnav_eItemType_PnInput;

  strcpy( name, item_name);

  if ( input->DataItem.size())
    parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnInput::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    for ( unsigned int i = 0; i < input->DataItem.size(); i++) {
      new ItemPnDataItem( attrnav, (char *)input->DataItem[i]->Body.TextId.p, input->DataItem[i],
			  node, flow_eDest_IntoLast);      
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnOutput::ItemPnOutput( GsdmlAttrNav *attrnav, const char *item_name, 
			  gsdml_Output *item_output,
			  brow_tNode dest, flow_eDest dest_code) :
  output(item_output)
{
  type = attrnav_eItemType_PnOutput;

  strcpy( name, item_name);

  if ( output->DataItem.size())
    parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnOutput::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    for ( unsigned int i = 0; i < output->DataItem.size(); i++) {
      new ItemPnDataItem( attrnav, (char *)output->DataItem[i]->Body.TextId.p, output->DataItem[i],
			  node, flow_eDest_IntoLast);      
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}


ItemPnDataItem::ItemPnDataItem( GsdmlAttrNav *attrnav, const char *item_name, 
			  gsdml_DataItem *item_dataitem,
			  brow_tNode dest, flow_eDest dest_code) :
  dataitem(item_dataitem)
{
  type = attrnav_eItemType_PnDataItem;

  strcpy( name, item_name);

  if ( dataitem->BitDataItem.size())
    parent = 1;

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  if ( parent)
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
  else
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetAnnotation( node, 1, dataitem->Body.DataType, strlen(dataitem->Body.DataType));
}

int ItemPnDataItem::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    void *p;
    if ( strcmp( dataitem->Body.DataType, "OctetString") == 0 ||
	 strcmp( dataitem->Body.DataType, "VisibleString") == 0) {
      p = (void *) &dataitem->Body.Length;
      new ItemPnBase( attrnav, "Length", "LocalGsdmlAttr", 
		      pwr_eType_UInt16, sizeof(dataitem->Body.Length), 0, 0,
		      p, 1, node, flow_eDest_IntoLast);
    }
    p = (void *) dataitem->Body.DataType;
    new ItemPnBase( attrnav, "Type", "LocalGsdmlAttr", 
		    pwr_eType_String, sizeof(dataitem->Body.DataType), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    p = (void *) &dataitem->Body.UseAsBits;
    new ItemPnBase( attrnav, "UseAsBits", "LocalGsdmlAttr", 
		    pwr_eType_Boolean, sizeof(dataitem->Body.UseAsBits), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    for ( unsigned int i = 0; i < dataitem->BitDataItem.size(); i++) {
      new ItemPnBitDataItem( attrnav, (char *)dataitem->BitDataItem[i]->Body.TextId.p, 
			  dataitem->BitDataItem[i], node, flow_eDest_IntoLast);      
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemPnBitDataItem::ItemPnBitDataItem( GsdmlAttrNav *attrnav, const char *item_name, 
			  gsdml_BitDataItem *item_bitdataitem,
			  brow_tNode dest, flow_eDest dest_code) :
  bitdataitem(item_bitdataitem)
{
  type = attrnav_eItemType_PnBitDataItem;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemPnBitDataItem::open_children( GsdmlAttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node)) {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children) {
      if ( parent)
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_map);
      else
	brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_leaf);
    }
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else {
    brow_SetNodraw( attrnav->brow->ctx);

    void *p = (void *) &bitdataitem->Body.BitOffset;
    new ItemPnBase( attrnav, "BitOffset", "LocalGsdmlAttr", 
		    pwr_eType_UInt8, sizeof(bitdataitem->Body.BitOffset), 0, 0,
		    p, 1, node, flow_eDest_IntoLast);

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}



ItemPnEnumByteOrder::ItemPnEnumByteOrder( GsdmlAttrNav *attrnav, const char *item_name, 
					  const char *attr,  int attr_type, int attr_size, 
					  void *attr_value_p, int attr_noedit,
					  brow_tNode dest, flow_eDest dest_code) :
  ItemPnBase( attrnav, item_name, attr, attr_type, attr_size, 0,
	      0, attr_value_p, attr_noedit,
	      dest, dest_code)
{
  type = attrnav_eItemType_PnEnumByteOrder;
  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);
}

int ItemPnEnumByteOrder::open_children( GsdmlAttrNav *attrnav, double x, double y)
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
    new ItemPnEnumValue( attrnav, "LittleEndian", 0, pwr_eType_UInt32, 
		this->value_p, node, flow_eDest_IntoLast);
    new ItemPnEnumValue( attrnav, "BigEndian", 1, pwr_eType_UInt32, 
		this->value_p, node, flow_eDest_IntoLast);
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnEnumByteOrder::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[80];

  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }
  else
    first_scan = 0;
  
  if ( *(int *)p == 0)
    strcpy( buf, "LittleEndian");
  else
    strcpy( buf, "BigEndian");
  
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = *(int *)p;

  return 1;
}

ItemPnEnumTimeRatio::ItemPnEnumTimeRatio( GsdmlAttrNav *attrnav, const char *item_name, 
					  gsdml_InterfaceSubmoduleItem *item_interfacesubmodule,
					  void *attr_value_p, brow_tNode dest, flow_eDest dest_code) :
  interfacesubmodule(item_interfacesubmodule), value_p(attr_value_p), first_scan(1), old_value(0)
{
  type = attrnav_eItemType_PnEnumTimeRatio;
  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		   dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrenum);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}

int ItemPnEnumTimeRatio::open_children( GsdmlAttrNav *attrnav, double x, double y)
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
    int				found;

    found = 0;
    if ( strcmp( interfacesubmodule->ApplicationRelations->TimingProperties->Body.ReductionRatio.str,
		 "") == 0)
      return 1;

    brow_SetNodraw( attrnav->brow->ctx);

    gsdml_Valuelist *vl = new gsdml_Valuelist( interfacesubmodule->ApplicationRelations->TimingProperties->
					       Body.ReductionRatio.str);
    gsdml_ValuelistIterator iter( vl);

    for ( unsigned int j = iter.begin(); j != iter.end(); j = iter.next()) {
      char enumtext[20];
      sprintf( enumtext, "%d", j);
		
      new ItemPnEnumValue( attrnav, enumtext, j, pwr_eType_UInt32, 
			   this->value_p, node, flow_eDest_IntoLast);
    }
    delete vl;

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemPnEnumTimeRatio::scan( GsdmlAttrNav *attrnav, void *p)
{
  char buf[80];

  if ( !first_scan) {
    if ( old_value == *(int *)p)
      // No change since last time
      return 1;
  }
  else {
    if ( *(int *)p == 0) {
      // Set initial value
      if ( strcmp( interfacesubmodule->ApplicationRelations->TimingProperties->Body.ReductionRatio.str,
		   "") != 0) {
	gsdml_Valuelist *vl = new gsdml_Valuelist( interfacesubmodule->ApplicationRelations->
						   TimingProperties->Body.ReductionRatio.str);
	gsdml_ValuelistIterator iter( vl);
	
	*(int *)p = iter.begin();
	delete vl;
      }
    }
    first_scan = 0;
  }
  
  sprintf( buf, "%d", *(int *)p);
  
  brow_SetAnnotation( node, 1, buf, strlen(buf));
  old_value = *(int *)p;

  return 1;
}
