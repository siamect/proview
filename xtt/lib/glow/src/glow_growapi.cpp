/* 
 * Proview   $Id: glow_growapi.cpp,v 1.34 2007-09-12 08:56:36 claes Exp $
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

#include "glow_std.h"

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "glow_ctx.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_line.h"
#include "glow_arc.h"
#include "glow_text.h"
#include "glow_array_elem.h"
#include "glow_array.h"
#include "glow_nodeclass.h"
#include "glow_node.h"
#include "glow_conclass.h"
#include "glow_con.h"
#include "glow_conpoint.h"
#include "glow_annot.h"
#include "glow_draw.h"
#include "glow_growcurve.h"
#include "glow_growapi.h"
#include "glow_growrect.h"
#include "glow_growrectrounded.h"
#include "glow_growline.h"
#include "glow_growarc.h"
#include "glow_growconpoint.h"
#include "glow_growsubannot.h"
#include "glow_growannot.h"
#include "glow_growpolyline.h"
#include "glow_growtext.h"
#include "glow_growbar.h"
#include "glow_growtrend.h"
#include "glow_growwindow.h"
#include "glow_growtable.h"
#include "glow_growfolder.h"
#include "glow_grownode.h"
#include "glow_growslider.h"
#include "glow_growimage.h"
#include "glow_growaxis.h"
#include "glow_growgroup.h"
#include "glow_growconglue.h"
#include "glow_growmenu.h"

/*! \file glow_growapi.cpp
    \brief Contains c API for grow. */
/*! \addtogroup GrowApi */
/*@{*/

static char *growapi_translate( char *transtab, char *name);

int grow_Save( grow_tCtx ctx, char *filename)
{
  return ctx->save( filename, glow_eSaveMode_Edit);
}

int grow_Open( grow_tCtx ctx, char *filename)
{
  return ctx->open( filename, glow_eSaveMode_Edit);
}

int grow_SaveTrace( grow_tCtx ctx, char *filename)
{
  return ctx->save( filename, glow_eSaveMode_Trace);
}

int grow_OpenTrace( grow_tCtx ctx, char *filename)
{
  return ctx->open( filename, glow_eSaveMode_Trace);
}

void grow_DeleteObject( grow_tCtx ctx, grow_tObject object)
{
  ctx->delete_object((GlowArrayElem *)object);
}

void grow_DeleteAll( grow_tCtx ctx)
{
  ctx->delete_all();
}

int grow_FindSelectedObject( grow_tCtx ctx, grow_tObject object)
{
  return ctx->select_find( (GlowArrayElem *)object);
}

void grow_ResetHighlightAll( grow_tCtx ctx)
{
  ctx->set_highlight( 0);
}

void grow_ResetInverseAll( grow_tCtx ctx)
{
  ctx->set_inverse( 0);
}

void grow_ResetSelectInverse( grow_tCtx ctx)
{
  ctx->set_select_inverse( 0);
}

void grow_ResetSelectHighlight( grow_tCtx ctx)
{
  ctx->set_select_highlight( 0);
}

void grow_SetSelectHighlight( grow_tCtx ctx)
{
  ctx->set_select_highlight( 1);
}

void grow_SetSelectInverse( grow_tCtx ctx)
{
  ctx->set_select_inverse( 1);
}

void grow_SelectInsert( grow_tCtx ctx, grow_tObject object)
{
  ctx->select_insert( (GlowArrayElem *)object);
}

void grow_SelectRemove( grow_tCtx ctx, grow_tObject object)
{
  ctx->select_remove( (GlowArrayElem *)object);
}

void grow_SelectClear( grow_tCtx ctx)
{
  ctx->select_clear();
}

void grow_GetSelectedNodes( grow_tCtx ctx, grow_tNode **nodes, int *num)
{
  ctx->get_selected_nodes( (GlowArrayElem ***) nodes, num);
}

void grow_GetSelectedCons( grow_tCtx ctx, grow_tCon **cons, int *num)
{
  ctx->get_selected_cons( (GlowArrayElem ***) cons, num);
}

void grow_GetNodeClassList( grow_tCtx ctx, grow_tNodeClass **nodeclasses, 
	int *num)
{
  ctx->get_nodeclasslist( (GlowArrayElem ***) nodeclasses, num);
}

void grow_GetNodeGroupList( grow_tCtx ctx, grow_tNodeClass **nodegroups, 
	int *num)
{
  ctx->get_nodegrouplist( (GlowArrayElem ***) nodegroups, num);
}

void grow_GetNodeClassObjectList( grow_tObject nodeclass, grow_tObject **objects, 
	int *num)
{
  ((GlowNodeClass *)nodeclass)->get_objectlist( (GlowArrayElem ***) objects, num);
}

void grow_SetHighlight( grow_tObject object, int value)
{
  ((GlowArrayElem *) object)->set_highlight( value);
}

void grow_GetHighlight( grow_tObject object, int *value)
{
  *value = ((GlowArrayElem *) object)->get_highlight();
}

void grow_SetInverse( grow_tObject object, int value)
{
  ((GlowArrayElem *) object)->set_inverse( value);
}

void grow_CreateNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node)
{
  GlowNode *n1;
  n1 = new GlowNode( ctx, name, (GlowNodeClass *)nc, x, y);
  n1->set_user_data( user_data);
  ctx->insert( n1);
  ctx->nav_zoom();
  *node = (grow_tNode) n1;
}

void grow_CreateGrowNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node)
{
  GrowNode *n1;
  n1 = new GrowNode( ctx, name, (GlowNodeClass *)nc, x, y);
  n1->set_user_data( user_data);
  ctx->insert( n1);
  ctx->nav_zoom();
  *node = (grow_tNode) n1;
}

void grow_CreateGrowSlider( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node)
{
  GrowSlider *n1;
  n1 = new GrowSlider( ctx, name, (GlowNodeClass *)nc, x, y);
  n1->set_user_data( user_data);
  ctx->insert( n1);
  ctx->nav_zoom();
  *node = (grow_tNode) n1;
}

void grow_CreateCon( grow_tCtx ctx, char *name, grow_tConClass cc,
	grow_tNode source, grow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, grow_tCon *con, 
	int point_num, double *x_vect, double *y_vect, int border, int shadow)
{
  GlowCon *c1;
  c1 = new GlowCon( ctx, name, (GlowConClass *)cc, (GlowNode *)source,
	(GlowNode *)dest, source_conpoint, dest_conpoint, 0, point_num,
	x_vect, y_vect, border, shadow);
  c1->set_user_data( user_data);
  ctx->insert( c1);
  ctx->nav_zoom();
  *con = (grow_tCon) c1;
}

void grow_CreatePasteNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node)
{
  GlowNode *n1;
  n1 = new GlowNode( ctx, name, (GlowNodeClass *)nc, x, y, 1);
  n1->set_user_data( user_data);
  ctx->paste_insert( n1);
  *node = (grow_tNode) n1;
}

void grow_SetObjectInputFocus( grow_tNode node, int focus)
{
  ((GlowArrayElem *)node)->set_input_focus( focus);
}

void grow_SetAnnotation( grow_tNode node, int number, char *text, int size)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)node)->set_annotation( number, text, size, 0);
}

void grow_SetAnnotationBrief( grow_tNode node, int number, char *text, int size)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)node)->set_annotation( number, text, size, 0, 1);
}

void grow_GetAnnotation( grow_tNode node, int number, char *text, int size)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)node)->get_annotation( number, text, size);
}

void grow_OpenAnnotationInput( grow_tNode node, int number)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode)
    ((GrowNode *)node)->open_annotation_input( number);
}

void grow_SetAnnotationSelection( grow_tNode node, int selection)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode)
    ((GrowNode *)node)->set_annotation_selection( selection);
}

void grow_CloseAnnotationInput( grow_tNode node, int number)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode)
    ((GrowNode *)node)->close_annotation_input( number);
}

void grow_CloseAnnotationInputAll( GrowCtx *ctx)
{
  ctx->close_annotation_input_all();
}

int grow_AnnotationInputIsOpen( grow_tNode node, int number)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode)
    return ((GrowNode *)node)->annotation_input_is_open( number);
  else
    return 0;
}

void grow_SetPasteNodeAnnotation( grow_tNode node, int number, char *text, int size)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode)
    ((GrowNode *)node)->set_annotation( number, text, size, 1);
}

extern "C" void	grow_EnableEvent( GrowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

void	grow_DisableEvent( GrowCtx *ctx, glow_eEvent event)
{
  ctx->disable_event( event);
}

extern "C" void	grow_DisableEventAll( GrowCtx *ctx)
{
  ctx->disable_event_all();
}

void grow_Cut( grow_tCtx ctx)
{
  ctx->cut();
}

void grow_Copy( grow_tCtx ctx)
{
  ctx->copy();
}

void grow_Paste( grow_tCtx ctx)
{
  ctx->paste();
}

void grow_PasteClear( grow_tCtx ctx)
{
  ctx->paste_clear();
}

void grow_CreateRect( grow_tCtx ctx, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	grow_tObject *rect)
{
  *rect = (grow_tObject) new GlowRect( ctx, x, y, width, height, draw_type,
	line_width, fix_line_width);
}

void grow_CreateLine( grow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	grow_tObject *line)
{
  *line = (grow_tObject) new GlowLine( ctx, x1, y1, x2, y2, draw_type,
	line_width, fix_line_width);
}

void grow_CreateArc( grow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width, grow_tObject *arc)
{
  *arc = (grow_tObject) new GlowArc( ctx, x1, y1, x2, y2, 
	angel1, angel2, draw_type, line_width);
}

void grow_CreateText( grow_tCtx ctx, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size, grow_tObject *text)
{
  *text = (grow_tObject) new GlowText( ctx, text_str, x, y, draw_type, glow_eDrawType_Line,
	text_size);
}

void grow_AddRect( grow_tNodeClass nc, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data)
{
  GrowRect *r1;
  r1 = new GrowRect( ((GlowNodeClass *)nc)->ctx, name, x, y, width, height, 
		     draw_type, line_width, fix_line_width, display_level, 
		     fill_rect, border, shadow, fill_draw_type);
  r1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( r1);
  
}

void grow_AddLine( grow_tNodeClass nc, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	void *user_data)
{
  GrowLine *l1;
  l1 = new GrowLine( ((GlowNodeClass *)nc)->ctx, name, x1, y1, x2, y2, 
		     draw_type, line_width, fix_line_width);
  l1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( l1);
}

void grow_AddPolyLine( grow_tNodeClass nc, char *name, 
	glow_sPoint *pointarray, int point_cnt,
	glow_eDrawType draw_type, int line_width, int fix_line_width,
	int fill, int border, int shadow, glow_eDrawType fill_draw_type, 
	int closed, void *user_data)
{
  GrowPolyLine *l1;
  l1 = new GrowPolyLine( ((GlowNodeClass *)nc)->ctx, name, pointarray, point_cnt, draw_type, 
	line_width, fix_line_width, fill, border, shadow, fill_draw_type, closed);
  l1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( l1);
}

void grow_AddArc( grow_tNodeClass nc, char *name, 
	double x1, double y1, double x2, double y2,
	int angel1, int angel2, glow_eDrawType draw_type, 
	int line_width, int fill_arc, int border, int shadow, glow_eDrawType fill_draw_type,
	void *user_data)
{
  GrowArc *a1;
  a1 = new GrowArc( ((GlowNodeClass *)nc)->ctx, name, x1, y1, x2, y2, 
		    angel1, angel2, draw_type, 
		    line_width, fill_arc, border, shadow, fill_draw_type);
  a1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( a1);
}

void grow_AddText( grow_tNodeClass nc, char *name, 
	char *text, double x, double y,
	glow_eDrawType draw_type, glow_eDrawType color, int t_size, 
	glow_eFont t_font, glow_mDisplayLevel display_level, void *user_data)
{
  GrowText *t1;
  t1 = new GrowText( ((GlowNodeClass *)nc)->ctx, name, text, x, y, draw_type, color,
		     t_size, t_font, display_level);
  t1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( t1);
}

void grow_AddAnnot( grow_tNodeClass nc,
	double x, double y, int annot_num, glow_eDrawType d_type, glow_eDrawType color_d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	void *user_data)
{
  GrowAnnot *a1;
  a1 = new GrowAnnot( ((GlowNodeClass *)nc)->ctx, x, y, annot_num, d_type, 
			 color_d_type, t_size, a_type, rel_pos, display_lev);
  a1->set_user_data( user_data);
  ((GlowNodeClass *)nc)->insert( a1);
}

void grow_CreateNodeClass( grow_tCtx ctx, char *name, glow_eNodeGroup group,
	grow_tNodeClass *nodeclass)
{
  *nodeclass = (grow_tNodeClass) new GlowNodeClass( ctx, name, group);
  ctx->nodeclass_insert( (GlowArrayElem *) *nodeclass);
}

void grow_NodeClassAdd( grow_tNodeClass nc, grow_tObject object)
{
  ((GlowNodeClass *)nc)->insert( (GlowArrayElem *)object);
}

void grow_CreateConClass( grow_tCtx ctx, char *name,
	glow_eConType con_type, glow_eCorner corner, glow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, glow_eConGroup group, 
	grow_tConClass *conclass)
{
  *conclass = (grow_tConClass) new GlowConClass( ctx, name, con_type,
	corner, line_type, line_width, arrow_width, arrow_length,
	round_corner_amount, group);
  ctx->conclass_insert( (GlowArrayElem *) *conclass);
}

void grow_GetSelectList( grow_tCtx ctx, grow_tObject **list, int *cnt)
{
  ctx->get_selectlist( (GlowArrayElem ***)list, cnt);
}

void grow_GetObjectList( grow_tCtx ctx, grow_tObject **list, int *cnt)
{
  ctx->get_objectlist( (GlowArrayElem ***)list, cnt);
}

void grow_GetMoveList( grow_tCtx ctx, grow_tObject **list, int *cnt)
{
  ctx->get_movelist( (GlowArrayElem ***)list, cnt);
}

void grow_SetObjectName( grow_tObject object, char *name)
{
  if ( strlen(name) > 31)
    return;
  ((GlowArrayElem *)object)->set_object_name( name);
}

glow_eObjectType grow_GetObjectType( grow_tObject object)
{
  return ((GlowArrayElem *)object)->type();
}

void grow_MeasureNode( grow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y)
{
  ((GlowNode *)node)->measure( ll_x, ll_y, ur_x, ur_y);
}

void grow_Print( grow_tCtx ctx, char *filename, double x0, double x1, int end)
{
  ctx->print( filename, x0, x1, end);
}

void grow_GetUserData( grow_tObject object, void **user_data)
{
  ((GlowArrayElem *)object)->get_user_data( user_data);
}

void grow_SetUserData( grow_tObject object, void *user_data)
{
  ((GlowArrayElem *)object)->set_user_data( user_data);
}

void grow_GetCtxUserData( grow_tCtx ctx, void **user_data)
{
  ((GlowCtx *)ctx)->get_user_data( user_data);
}

void grow_SetCtxUserData( grow_tCtx ctx, void *user_data)
{
  ((GlowCtx *)ctx)->set_user_data( user_data);
}

grow_tCtx grow_GetCtx( grow_tObject object)
{
  return (grow_tCtx)((GlowArrayElem *)object)->get_ctx();
}

void grow_SetTraceAttr( grow_tObject object, GlowTraceData *trace_data)
{
  ((GlowArrayElem *)object)->set_trace_attr( trace_data);
}

void grow_GetTraceAttr( grow_tObject object, GlowTraceData **trace_data)
{
  ((GlowArrayElem *)object)->get_trace_attr( trace_data);
}

void grow_SetTraceData( grow_tObject object, void *trace_data)
{
  ((GlowNode *)object)->set_trace_data( trace_data);
}

extern "C" int grow_TraceInit( grow_tCtx ctx, 
	int (*trace_connect_func)( grow_tObject, GlowTraceData *),
	int (*trace_disconnect_func)( grow_tObject),
	int (*trace_scan_func)( grow_tObject, void *))
{
  return ctx->trace_init( trace_connect_func, trace_disconnect_func,
	trace_scan_func);
}

void grow_TraceClose( grow_tCtx ctx)
{
  ctx->trace_close();
}

void grow_TraceScan( grow_tCtx ctx)
{
  ctx->trace_scan();
}

void grow_RemoveTraceObjects( grow_tCtx ctx)
{
  ctx->remove_trace_objects();
}

void grow_Zoom( grow_tCtx ctx, double zoom_factor)
{
  ctx->zoom( zoom_factor);
}

void grow_ZoomX( grow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_x( zoom_factor);
}

void grow_ZoomY( grow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_y( zoom_factor);
}

void grow_ZoomAbsolute( grow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_absolute( zoom_factor);
}

void grow_GetZoom( grow_tCtx ctx, double *zoom_factor)
{
  *zoom_factor = ctx->mw.zoom_factor_x;
}

void grow_Scroll( grow_tCtx ctx, double x, double y)
{
  ctx->scroll( x, y);;
}

void grow_SetAttributes( grow_tCtx ctx, grow_sAttributes *attr, 
	unsigned long mask)
{
  if ( mask & grow_eAttr_base_zoom_factor)
  {
    ctx->mw.zoom_factor_x *= attr->base_zoom_factor / ctx->mw.base_zoom_factor;
    ctx->mw.zoom_factor_y *= attr->base_zoom_factor / ctx->mw.base_zoom_factor;
    ctx->mw.base_zoom_factor = attr->base_zoom_factor;
  }
  if ( mask & grow_eAttr_offset_x)
    ctx->mw.offset_x = attr->offset_x;
  if ( mask & grow_eAttr_offset_y)
    ctx->mw.offset_y = attr->offset_y;
  if ( mask & grow_eAttr_grid_size_x)
    ctx->grid_size_x = attr->grid_size_x;
  if ( mask & grow_eAttr_grid_size_y)
    ctx->grid_size_y = attr->grid_size_y;
  if ( mask & grow_eAttr_grid_on)
    ctx->grid_on = attr->grid_on;
  if ( mask & grow_eAttr_user_highlight)
    ctx->user_highlight = attr->user_highlight;
  if ( mask & grow_eAttr_draw_delta)
    ctx->draw_delta = attr->draw_delta;
  if ( mask & grow_eAttr_grafcet_con_delta)
    ctx->grafcet_con_delta = attr->grafcet_con_delta;
  if ( mask & grow_eAttr_refcon_width)
    ctx->refcon_width = attr->refcon_width;
  if ( mask & grow_eAttr_refcon_height)
    ctx->refcon_height = attr->refcon_height;
  if ( mask & grow_eAttr_refcon_linewidth)
    ctx->refcon_linewidth = attr->refcon_linewidth;
  if ( mask & grow_eAttr_refcon_textsize)
    ctx->refcon_textsize = attr->refcon_textsize;
  if ( mask & grow_eAttr_application_paste)
    ctx->application_paste = attr->application_paste;
  if ( mask & grow_eAttr_select_policy)
    ctx->select_policy = attr->select_policy;
  if ( mask & grow_eAttr_display_level)
    ctx->display_level = attr->display_level;
  if ( mask & grow_eAttr_default_hot_mode)
    ctx->default_hot_mode = ctx->hot_mode = attr->default_hot_mode;
  if ( mask & grow_eAttr_enable_bg_pixmap)
    ctx->enable_bg_pixmap = attr->enable_bg_pixmap;
  if ( mask & grow_eAttr_double_buffer_on) {
    if ( !ctx->mw.window->double_buffer_on)
      ctx->mw.window->double_buffer_on = attr->double_buffer_on;
    if ( ctx->mw.window->double_buffer_on) {
      ctx->gdraw->create_buffer( &ctx->mw);
      // ctx->mw.window->double_buffered = 1;
    }
  }
  if ( mask & grow_eAttr_hot_mode) {
    ctx->default_hot_mode = attr->hot_mode;
    ctx->hot_mode = attr->hot_mode;
  }
  if ( mask & grow_eAttr_initial_position)
    ctx->initial_position = attr->initial_position;
}

void grow_GetAttributes( grow_tCtx ctx, grow_sAttributes *attr, 
	unsigned long mask)
{
  if ( mask & grow_eAttr_base_zoom_factor)
    attr->base_zoom_factor = ctx->mw.base_zoom_factor;
  if ( mask & grow_eAttr_offset_x)
    attr->offset_x = ctx->mw.offset_x;
  if ( mask & grow_eAttr_offset_y)
    attr->offset_y = ctx->mw.offset_y;
  if ( mask & grow_eAttr_grid_size_x)
    attr->grid_size_x = ctx->grid_size_x;
  if ( mask & grow_eAttr_grid_size_y)
    attr->grid_size_y = ctx->grid_size_y;
  if ( mask & grow_eAttr_grid_on)
    attr->grid_on = ctx->grid_on;
  if ( mask & grow_eAttr_user_highlight)
    attr->user_highlight = ctx->user_highlight;
  if ( mask & grow_eAttr_draw_delta)
    attr->draw_delta = ctx->draw_delta;
  if ( mask & grow_eAttr_grafcet_con_delta)
    attr->grafcet_con_delta = ctx->grafcet_con_delta;
  if ( mask & grow_eAttr_refcon_width)
    attr->refcon_width = ctx->refcon_width;
  if ( mask & grow_eAttr_refcon_height)
    attr->refcon_height = ctx->refcon_height;
  if ( mask & grow_eAttr_refcon_linewidth)
    attr->refcon_linewidth = ctx->refcon_linewidth;
  if ( mask & grow_eAttr_refcon_textsize)
    attr->refcon_textsize = ctx->refcon_textsize;
  if ( mask & grow_eAttr_application_paste)
    attr->application_paste = ctx->application_paste;
  if ( mask & grow_eAttr_select_policy)
    attr->select_policy = ctx->select_policy;
  if ( mask & grow_eAttr_display_level)
    attr->display_level = ctx->display_level;
  if ( mask & grow_eAttr_default_hot_mode)
    attr->default_hot_mode = ctx->default_hot_mode;
  if ( mask & grow_eAttr_enable_bg_pixmap)
    attr->enable_bg_pixmap = ctx->enable_bg_pixmap;
  if ( mask & grow_eAttr_double_buffer_on)
    attr->double_buffer_on = ctx->mw.window->double_buffer_on;
  if ( mask & grow_eAttr_hot_mode)
    attr->hot_mode = ctx->hot_mode;
  if ( mask & grow_eAttr_initial_position)
    attr->initial_position = (glow_eDirection) ctx->initial_position;
}

void grow_PositionToPixel( grow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y)
{
  ctx->position_to_pixel( x, y, pix_x, pix_y);
}

void grow_UnZoom( grow_tCtx ctx)
{
  ctx->unzoom();
}

void grow_CenterObject( grow_tCtx ctx, grow_tObject object)
{
  ctx->center_object( (GlowArrayElem *)object);
}

void grow_GetNodePosition( grow_tNode node, double *x, double *y)
{
  ((GlowNode *)node)->get_node_position( x, y);
}

void grow_MeasureAnnotText( grow_tCtx ctx, char *text, glow_eDrawType draw_type,
			    int text_size, glow_eAnnotType annot_type, glow_eFont font,
			    double *width, double *height, int *rows)
{
  glow_measure_annot_text( ctx, text, draw_type, text_size, annot_type, font,
		width, height, rows);
}

glow_eNodeGroup grow_GetNodeGroup( grow_tNode node)
{
  return ((GlowNode *)node)->get_group();
}

void grow_GetObjectName( grow_tObject object, char *name)
{
  ((GlowNode *)object)->get_object_name( name);
}

void grow_GetNodeClassName( grow_tNodeClass nodeclass, char *name)
{
  ((GlowNodeClass *)nodeclass)->get_object_name( name);
}

void grow_Reconfigure( grow_tCtx ctx)
{
  ctx->reconfigure();
}

void grow_SetNodraw( grow_tCtx ctx)
{
  ctx->set_nodraw();
}

void grow_ResetNodraw( grow_tCtx ctx)
{
  ctx->reset_nodraw();
}

void grow_SetDeferedRedraw( grow_tCtx ctx)
{
  ctx->set_defered_redraw();
}

void grow_RedrawDefered( grow_tCtx ctx)
{
  ctx->redraw_defered();
}

void grow_Redraw( grow_tCtx ctx)
{
  ctx->redraw();
}

void grow_SetInputFocus( grow_tCtx ctx)
{
  ctx->gdraw->set_inputfocus( &ctx->mw);
}

void grow_SetMode( grow_tCtx ctx, grow_eMode mode)
{
  ctx->set_mode( mode);
}

grow_eMode grow_Mode( grow_tCtx ctx)
{
  return ctx->mode();
}

void grow_CreateGrowRect( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *rect)
{
  GrowRect *r1;
  r1 = new GrowRect( ctx, name, x, y, width, height, draw_type, line_width,
	fix_line_width, display_level, fill_rect, border, shadow, fill_draw_type);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *rect = (grow_tObject) r1;
}

void grow_CreateGrowRectRounded( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *rect)
{
  GrowRectRounded *r1;
  r1 = new GrowRectRounded( ctx, name, x, y, width, height, draw_type, line_width,
	fix_line_width, display_level, fill_rect, border, shadow, fill_draw_type);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *rect = (grow_tObject) r1;
}

void grow_CreateGrowBar( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *bar)
{
  GrowBar *r1;
  r1 = new GrowBar( ctx, name, x, y, width, height, draw_type, line_width,
	display_level, fill_rect, border, fill_draw_type);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *bar = (grow_tObject) r1;
}

void grow_CreateGrowTrend( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *trend)
{
  GrowTrend *r1;
  r1 = new GrowTrend( ctx, name, x, y, width, height, draw_type, line_width,
	display_level, fill_rect, border, fill_draw_type);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *trend = (grow_tObject) r1;
}

void grow_CreateGrowCurve( grow_tCtx ctx, char *name, glow_sCurveData *data,
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *curve)
{
  GrowCurve *r1;
  r1 = new GrowCurve( ctx, name, data,  x, y, width, height, draw_type,
        line_width, display_level, fill_rect, border, fill_draw_type);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *curve = (grow_tObject) r1;
}

void grow_CreateGrowWindow( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *window)
{
  GrowWindow *r1;
  r1 = new GrowWindow( ctx, name, x, y, width, height, draw_type, line_width,
	display_level);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *window = (grow_tObject) r1;
}

void grow_CreateGrowTable( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	int fill, glow_eDrawType fillcolor,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *window)
{
  GrowTable *r1;
  r1 = new GrowTable( ctx, name, x, y, width, height, draw_type, line_width,
	fill, fillcolor, display_level);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *window = (grow_tObject) r1;
}

void grow_CreateGrowFolder( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
        glow_eDrawType draw_type, int line_width, glow_eDrawType selected_color,
	glow_eDrawType unselected_color,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *window)
{
  GrowFolder *r1;
  r1 = new GrowFolder( ctx, name, x, y, width, height, draw_type, line_width,
	selected_color, unselected_color, display_level);
  r1->set_user_data( user_data);
  ctx->insert( r1);
  ctx->nav_zoom();
  *window = (grow_tObject) r1;
}

void grow_CreateGrowLine( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	void *user_data,
	grow_tObject *line)
{
  GrowLine *l1;
  l1 = new GrowLine( ctx, name, x1, y1, x2, y2, draw_type, line_width,
	fix_line_width);
  l1->set_user_data( user_data);
  ctx->insert( l1);
  ctx->nav_zoom();
  *line = (grow_tObject) l1;
}

void grow_CreateGrowPolyLine( grow_tCtx ctx, char *name, 
	glow_sPoint *pointarray, int point_cnt,
	glow_eDrawType draw_type, int line_width, int fix_line_width,
	int fill, int border, int shadow, glow_eDrawType fill_draw_type, 
	int closed, void *user_data,
	grow_tObject *polyline)
{
  GrowPolyLine *l1;
  l1 = new GrowPolyLine( ctx, name, pointarray, point_cnt, draw_type, 
	line_width, fix_line_width, fill, border, shadow, fill_draw_type, closed);
  l1->set_user_data( user_data);
  ctx->insert( l1);
  ctx->nav_zoom();
  *polyline = (grow_tObject) l1;
}

void grow_CreateGrowArc( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	int angel1, int angel2, glow_eDrawType draw_type, 
	int line_width, int fill_arc, int border, int shadow, glow_eDrawType fill_draw_type,
	void *user_data,
	grow_tObject *arc)
{
  GrowArc *a1;
  a1 = new GrowArc( ctx, name, x1, y1, x2, y2, angel1, angel2, draw_type, 
	line_width, fill_arc, border, shadow, fill_draw_type);
  a1->set_user_data( user_data);
  ctx->insert( a1);
  ctx->nav_zoom();
  *arc = (grow_tObject) a1;
}

void grow_CreateGrowConPoint( grow_tCtx ctx, char *name, 
	double x, double y, int cp_num, glow_eDirection d,
	void *user_data, grow_tObject *conpoint)
{
  GrowConPoint *cp1;
  cp1 = new GrowConPoint( ctx, name, x, y, cp_num, d);
  cp1->set_user_data( user_data);
  ctx->insert( cp1);
  ctx->nav_zoom();
  *conpoint = (grow_tObject) cp1;
}

void grow_CreateGrowAnnot( grow_tCtx ctx, char *name, 
	double x, double y, int annot_num, glow_eDrawType d_type, glow_eDrawType color_d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	void *user_data, grow_tObject *annot)
{
  GrowSubAnnot *a1;
  a1 = new GrowSubAnnot( ctx, name, x, y, annot_num, d_type, color_d_type, t_size,
	a_type, rel_pos, display_lev);
  a1->set_user_data( user_data);
  ctx->insert( a1);
  ctx->nav_zoom();
  *annot = (grow_tObject) a1;
}

void grow_CreateGrowText( grow_tCtx ctx, char *name, 
	char *text, double x, double y,
	glow_eDrawType draw_type, glow_eDrawType color, int t_size, 
	glow_eFont t_font, glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *text_object)
{
  GrowText *t1;
  t1 = new GrowText( ctx, name, text, x, y, draw_type, color, t_size, t_font,
		     display_level);
  t1->set_user_data( user_data);
  ctx->insert( t1);
  ctx->nav_zoom();
  *text_object = (grow_tObject) t1;
}

void grow_CreateGrowImage( grow_tCtx ctx, char *name, char *filename,
	double x1, double y1, void *user_data, grow_tObject *image)
{
  GrowImage *i1;
  i1 = new GrowImage( ctx, name, x1, y1, filename);
  i1->set_user_data( user_data);
  ctx->insert( i1);
  ctx->nav_zoom();
  *image = (grow_tObject) i1;
}

void grow_CreateGrowAxis( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int text_size, 
	glow_eDrawType text_drawtype, void *user_data,
	grow_tObject *axis)
{
  GrowAxis *l1;
  l1 = new GrowAxis( ctx, name, x1, y1, x2, y2, draw_type, line_width,
	text_size, text_drawtype);
  l1->set_user_data( user_data);
  ctx->insert( l1);
  ctx->nav_zoom();
  *axis = (grow_tObject) l1;
}

void grow_CreateGrowConGlue( grow_tCtx ctx, char *name, 
	double x, double y, grow_tObject *conglue)
{
  GrowConGlue *r1;
  r1 = new GrowConGlue( ctx, name, x, y);
  ctx->insert( r1);
  ctx->nav_zoom();
  *conglue = (grow_tObject) r1;
}

void grow_CreateGrowMenu( grow_tCtx ctx, char *name, glow_sMenuInfo *info,
			  double x, double y, double min_width,
			  glow_eDrawType draw_type, int line_width,
			  int fill_rect, int border, glow_eDrawType fill_draw_type,
			  int text_size, glow_eDrawType text_drawtype,
			  glow_eDrawType text_color, glow_eDrawType disabled_text_color,
			  grow_tObject parent, grow_tObject *menu)
{
  GrowMenu *r1;
  r1 = new GrowMenu( ctx, name, info, x, y, min_width, draw_type, line_width,
		     fill_rect, border, fill_draw_type, text_size, text_drawtype, 
		     text_color, disabled_text_color, (GlowArrayElem *)parent);
  ctx->insert( r1);
  *menu = (grow_tObject) r1;
}

int grow_SaveSubGraph( grow_tCtx ctx, char *filename)
{
  return ctx->save_subgraph( filename, glow_eSaveMode_SubGraph);
}

int grow_OpenSubGraph( grow_tCtx ctx, char *filename)
{
  return ctx->open_subgraph( filename, glow_eSaveMode_SubGraph);
}

int grow_OpenSubGraphFromName( grow_tCtx ctx, char *name)
{
  return ctx->open_subgraph_from_name( name, glow_eSaveMode_SubGraph);
}

int grow_FindObjectByName( grow_tCtx ctx, char *name, grow_tObject *object)
{
  return ctx->find_by_name( name, (GlowArrayElem **)object);
}

int grow_FindNodeClassByName( grow_tCtx ctx, char *name, grow_tObject *object)
{
  return ctx->find_nc_by_name( name, (GlowArrayElem **)object);
}

int grow_FindConClassByName( grow_tCtx ctx, char *name, grow_tObject *object)
{
  return ctx->find_cc_by_name( name, (GlowArrayElem **)object);
}

int grow_GetNextConPointNumber( grow_tCtx ctx)
{
  return ctx->get_next_conpoint_num();
}

int grow_GetNextObjectNameNumber( grow_tCtx ctx)
{
  return ctx->get_next_objectname_num();
}

int grow_GetObjectAttrInfo( grow_tObject object, char *transtab, 
	grow_sAttrInfo **info, int *attr_cnt)
{
  grow_sAttrInfo *attrinfo;
  int i, j;

  attrinfo = (grow_sAttrInfo *) calloc( 100, sizeof(grow_sAttrInfo)); 

  i = 0;
  switch ( ((GrowRect *)object)->type())
  {
    case glow_eObjectType_GrowRect:
    {
      GrowRect *op = (GrowRect *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "shadow_width");
      attrinfo[i].value_p = &op->shadow_width;
      attrinfo[i].type = glow_eType_Double;
      attrinfo[i].minlimit = 0;
      attrinfo[i].maxlimit = 50;
      attrinfo[i++].size = sizeof( op->shadow_width);
      
      strcpy( attrinfo[i].name, "shadow_contrast");
      attrinfo[i].value_p = &op->shadow_contrast;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i].minlimit = 1;
      attrinfo[i].maxlimit = 3;
      attrinfo[i++].size = sizeof( op->shadow_contrast);
      
      strcpy( attrinfo[i].name, "relief");
      attrinfo[i].value_p = &op->relief;
      attrinfo[i].type = glow_eType_Relief;
      attrinfo[i++].size = sizeof( op->relief);
      
      strcpy( attrinfo[i].name, "invisible");
      attrinfo[i].value_p = &op->invisible;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->invisible);
      
      strcpy( attrinfo[i].name, "fixcolor");
      attrinfo[i].value_p = &op->fixcolor;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fixcolor);
      
      strcpy( attrinfo[i].name, "disable_shadow");
      attrinfo[i].value_p = &op->disable_shadow;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->disable_shadow);
      
      strcpy( attrinfo[i].name, "fixposition");
      attrinfo[i].value_p = &op->fixposition;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fixposition);
      
      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowRectRounded:
    {
      GrowRectRounded *op = (GrowRectRounded *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "round_amount");
      attrinfo[i].value_p = &op->round_amount;
      attrinfo[i].type = glow_eType_Double;
      attrinfo[i++].size = sizeof( op->round_amount);
      
      strcpy( attrinfo[i].name, "shadow_width");
      attrinfo[i].value_p = &op->shadow_width;
      attrinfo[i].type = glow_eType_Double;
      attrinfo[i].minlimit = 0;
      attrinfo[i].maxlimit = 50;
      attrinfo[i++].size = sizeof( op->shadow_width);
      
      strcpy( attrinfo[i].name, "shadow_contrast");
      attrinfo[i].value_p = &op->shadow_contrast;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i].minlimit = 1;
      attrinfo[i].maxlimit = 3;
      attrinfo[i++].size = sizeof( op->shadow_contrast);
      
      strcpy( attrinfo[i].name, "relief");
      attrinfo[i].value_p = &op->relief;
      attrinfo[i].type = glow_eType_Relief;
      attrinfo[i++].size = sizeof( op->relief);
      
      strcpy( attrinfo[i].name, "disable_shadow");
      attrinfo[i].value_p = &op->disable_shadow;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->disable_shadow);
      
      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowPolyLine:
    {
      GrowPolyLine *op = (GrowPolyLine *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "shadow_width");
      attrinfo[i].value_p = &op->shadow_width;
      attrinfo[i].type = glow_eType_Double;
      attrinfo[i].minlimit = 0;
      attrinfo[i].maxlimit = 50;
      attrinfo[i++].size = sizeof( op->shadow_width);
      
      strcpy( attrinfo[i].name, "shadow_contrast");
      attrinfo[i].value_p = &op->shadow_contrast;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i].minlimit = 1;
      attrinfo[i].maxlimit = 3;
      attrinfo[i++].size = sizeof( op->shadow_contrast);
      
      strcpy( attrinfo[i].name, "relief");
      attrinfo[i].value_p = &op->relief;
      attrinfo[i].type = glow_eType_Relief;
      attrinfo[i++].size = sizeof( op->relief);
      
      strcpy( attrinfo[i].name, "disable_shadow");
      attrinfo[i].value_p = &op->disable_shadow;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->disable_shadow);
      
      strcpy( attrinfo[i].name, "fill_eq_border");
      attrinfo[i].value_p = &op->fill_eq_border;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fill_eq_border);
      
      strcpy( attrinfo[i].name, "fill_eq_light");
      attrinfo[i].value_p = &op->fill_eq_light;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fill_eq_light);
      
      strcpy( attrinfo[i].name, "fill_eq_shadow");
      attrinfo[i].value_p = &op->fill_eq_shadow;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fill_eq_shadow);
      
      strcpy( attrinfo[i].name, "fixcolor");
      attrinfo[i].value_p = &op->fixcolor;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fixcolor);
      
      strcpy( attrinfo[i].name, "fixposition");
      attrinfo[i].value_p = &op->fixposition;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->fixposition);
      
      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowLine:
    {
      GrowLine *op = (GrowLine *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowArc:
    {
      GrowArc *op = (GrowArc *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "angel1");
      attrinfo[i].value_p = &op->angel1;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->angel1);
      
      strcpy( attrinfo[i].name, "angel2");
      attrinfo[i].value_p = &op->angel2;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->angel2);
      
      strcpy( attrinfo[i].name, "shadow_width");
      attrinfo[i].value_p = &op->shadow_width;
      attrinfo[i].type = glow_eType_Double;
      attrinfo[i].minlimit = 0;
      attrinfo[i].maxlimit = 50;
      attrinfo[i++].size = sizeof( op->shadow_width);
      
      strcpy( attrinfo[i].name, "shadow_contrast");
      attrinfo[i].value_p = &op->shadow_contrast;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i].minlimit = 1;
      attrinfo[i].maxlimit = 3;
      attrinfo[i++].size = sizeof( op->shadow_contrast);
      
      strcpy( attrinfo[i].name, "relief");
      attrinfo[i].value_p = &op->relief;
      attrinfo[i].type = glow_eType_Relief;
      attrinfo[i++].size = sizeof( op->relief);
      
      strcpy( attrinfo[i].name, "disable_shadow");
      attrinfo[i].value_p = &op->disable_shadow;
      attrinfo[i].type = glow_eType_Boolean;
      attrinfo[i++].size = sizeof( op->disable_shadow);
      
      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowConPoint:
    {
      GrowConPoint *op = (GrowConPoint *)object;

      strcpy( attrinfo[i].name, "Number");
      attrinfo[i].value_p = &op->number;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->number);
      
      strcpy( attrinfo[i].name, "Direction");
      attrinfo[i].value_p = &op->direction;
      attrinfo[i].type = glow_eType_Direction;
      attrinfo[i++].size = sizeof( op->direction);
      
      break;
    }
    case glow_eObjectType_GrowSubAnnot:
    {
      GrowSubAnnot *op = (GrowSubAnnot *)object;

      strcpy( attrinfo[i].name, "TextSize");
      attrinfo[i].value_p = &op->text_size;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->text_size);
      
      strcpy( attrinfo[i].name, "Number");
      attrinfo[i].value_p = &op->number;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->number);
      
      strcpy( attrinfo[i].name, "Adjustment");
      attrinfo[i].value_p = &op->adjustment;
      attrinfo[i].type = glow_eType_Adjustment;
      attrinfo[i++].size = sizeof( op->adjustment);
      
      strcpy( attrinfo[i].name, "Font");
      attrinfo[i].value_p = &op->font;
      attrinfo[i].type = glow_eType_Font;
      attrinfo[i++].size = sizeof( op->font);
      
      break;
    }
    case glow_eObjectType_GrowText:
    {
      GrowText *op = (GrowText *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "Text");
      attrinfo[i].value_p = &op->text;
      attrinfo[i].type = glow_eType_String;
      attrinfo[i++].size = sizeof( op->text);

      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowImage:
    {
      GrowImage *op = (GrowImage *)object;
      char *dynamic;
      int  dynsize;

      strcpy( attrinfo[i].name, "Image");
      attrinfo[i].value_p = &op->image_filename;
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = sizeof( op->image_filename);
      attrinfo[i++].info_type = grow_eInfoType_Image;

      strcpy( attrinfo[i].name, "Dynamic");
      op->get_dynamic( &dynamic, &dynsize);
      attrinfo[i].value_p = malloc( 1024);
      if ( dynsize)
        strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
      else
        strcpy( (char *) attrinfo[i].value_p, "");
      attrinfo[i].type = glow_eType_String;
      attrinfo[i].size = 1024;
      attrinfo[i].multiline = 1;
      attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      break;
    }
    case glow_eObjectType_GrowBar:
    {
      GrowBar *op = (GrowBar *)object;
      char *name;
      char *dynamic;
      int  dynsize;

      if ( (name = growapi_translate( transtab, "MaxValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->max_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->max_value);
      }

      if ( (name = growapi_translate( transtab, "MinValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->min_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->min_value);
      }

      if ( (name = growapi_translate( transtab, "BarValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->bar_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->bar_value);
      }

      if ( (name = growapi_translate( transtab, "BarColor")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->bar_drawtype;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->bar_drawtype);
      }

      if ( (name = growapi_translate( transtab, "BarBorderColor")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->bar_bordercolor;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->bar_bordercolor);
      }

      if ( (name = growapi_translate( transtab, "BarBorderWidth")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->bar_borderwidth;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->bar_borderwidth);
      }

      if ( (name = growapi_translate( transtab, "Dynamic")))
      {
        strcpy( attrinfo[i].name, name);
        op->get_dynamic( &dynamic, &dynsize);
        attrinfo[i].value_p = malloc( 1024);
        if ( dynsize)
          strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
        else
          strcpy( (char *) attrinfo[i].value_p, "");
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 1024;
        attrinfo[i].multiline = 1;
        attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      }
      break;
    }
    case glow_eObjectType_GrowTrend:
    {
      GrowTrend *op = (GrowTrend *)object;
      char *name;
      char *dynamic;
      int  dynsize;

      if ( (name = growapi_translate( transtab, "NoOfPoints")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->no_of_points;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->no_of_points);
      }

      if ( (name = growapi_translate( transtab, "ScanTime")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scan_time;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->scan_time);
      }

      if ( (name = growapi_translate( transtab, "CurveWidth")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->curve_width;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->curve_width);
      }

      if ( (name = growapi_translate( transtab, "FillCurve")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->fill_curve;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->fill_curve);
      }

      if ( (name = growapi_translate( transtab, "HorizontalLines")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->horizontal_lines;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->horizontal_lines);
      }

      if ( (name = growapi_translate( transtab, "VerticalLines")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->vertical_lines;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->vertical_lines);
      }

      if ( (name = growapi_translate( transtab, "MaxValue1")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->y_max_value[0];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->y_max_value[0]);
      }

      if ( (name = growapi_translate( transtab, "MinValue1")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->y_min_value[0];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->y_min_value[0]);
      }
      
      if ( (name = growapi_translate( transtab, "CurveColor1")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->curve_drawtype[0];
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->curve_drawtype[0]);
      }

      if ( (name = growapi_translate( transtab, "CurveFillColor1")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->curve_fill_drawtype[0];
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->curve_fill_drawtype[0]);
      }

      if ( (name = growapi_translate( transtab, "MaxValue2")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->y_max_value[1];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->y_max_value[1]);
      }

      if ( (name = growapi_translate( transtab, "MinValue2")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->y_min_value[1];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->y_min_value[1]);
      }
      
      if ( (name = growapi_translate( transtab, "CurveColor2")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->curve_drawtype[1];
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->curve_drawtype[1]);
      }

      if ( (name = growapi_translate( transtab, "CurveFillColor2")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->curve_fill_drawtype[1];
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->curve_fill_drawtype[1]);
      }

      if ( (name = growapi_translate( transtab, "Dynamic")))
      {
        strcpy( attrinfo[i].name, name);
        op->get_dynamic( &dynamic, &dynsize);
        attrinfo[i].value_p = malloc( 1024);
        if ( dynsize)
          strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
        else
          strcpy( (char *) attrinfo[i].value_p, "");
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 1024;
        attrinfo[i].multiline = 1;
        attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      }
      break;
    }
    case glow_eObjectType_GrowWindow:
    {
      GrowWindow *op = (GrowWindow *)object;
      char *name;

      if ( (name = growapi_translate( transtab, "FileName"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->input_file_name;
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->input_file_name);
      }
      if ( (name = growapi_translate( transtab, "WindowScale"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->window_scale;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->window_scale);
      }
      if ( (name = growapi_translate( transtab, "VerticalScrollbar"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->vertical_scrollbar;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->vertical_scrollbar);
      }
      if ( (name = growapi_translate( transtab, "HorizontalScrollbar"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->horizontal_scrollbar;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->horizontal_scrollbar);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarWidth"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_width;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->scrollbar_width);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_color;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->scrollbar_color);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarBgColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_bg_color;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->scrollbar_bg_color);
      }
      if ( (name = growapi_translate( transtab, "Owner"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->owner;
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->owner);
      }

      break;
    }
    case glow_eObjectType_GrowTable:
    {
      GrowTable *op = (GrowTable *)object;
      char *name;

      if ( (name = growapi_translate( transtab, "Rows"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->rows;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->rows);
      }
      if ( (name = growapi_translate( transtab, "Columns"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->columns;
        attrinfo[i].type = glow_eType_Int;
	attrinfo[i].minlimit = 1;
	attrinfo[i].maxlimit = 12;
        attrinfo[i++].size = sizeof( op->columns);
      }
      if ( (name = growapi_translate( transtab, "HeaderRow"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_row;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->header_row);
      }
      if ( (name = growapi_translate( transtab, "HeaderColumn"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_column;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->header_column);
      }
      if ( (name = growapi_translate( transtab, "HeaderRowHeight"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_row_height;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->header_row_height);
      }
      if ( (name = growapi_translate( transtab, "RowHeight"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->row_height;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->row_height);
      }
      if ( (name = growapi_translate( transtab, "HeaderTextSize"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_text_size;
        attrinfo[i].type = glow_eType_TextSize;
        attrinfo[i++].size = sizeof( op->header_text_size);
      }
      if ( (name = growapi_translate( transtab, "HeaderTextBold"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_text_bold;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->header_text_bold);
      }
      if ( (name = growapi_translate( transtab, "HeaderTextColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_text_color;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->header_text_color);
      }
      if ( (name = growapi_translate( transtab, "SelectColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->select_drawtype;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->select_drawtype);
      }
      if ( (name = growapi_translate( transtab, "Options"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->options;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->options);
      }
      if ( (name = growapi_translate( transtab, "VerticalScrollbar"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->vertical_scrollbar;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->vertical_scrollbar);
      }
      if ( (name = growapi_translate( transtab, "HorizontalScrollbar"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->horizontal_scrollbar;
        attrinfo[i].type = glow_eType_Boolean;
        attrinfo[i++].size = sizeof( op->horizontal_scrollbar);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarWidth"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_width;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->scrollbar_width);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_color;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->scrollbar_color);
      }
      if ( (name = growapi_translate( transtab, "ScrollbarBgColor"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->scrollbar_bg_color;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->scrollbar_bg_color);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth1"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[0];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment1"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[0];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText1"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[0];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth2"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[1];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment2"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[1];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText2"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[1];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth3"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[2];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment3"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[2];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText3"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[2];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth4"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[3];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment4"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[3];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText4"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[3];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth5"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[4];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment5"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[4];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText5"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[4];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth6"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[5];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment6"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[5];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText6"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[5];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth7"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[6];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment7"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[6];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText7"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[6];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth8"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[7];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment8"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[7];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText8"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[7];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth9"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[8];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment9"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[8];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText9"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[8];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth10"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[9];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment10"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[9];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText10"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[9];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth11"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[10];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment11"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[10];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText11"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[10];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnWidth12"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_width[11];
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->column_width[0]);
      }
      if ( (name = growapi_translate( transtab, "ColumnAdjustment12"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->column_adjustment[11];
        attrinfo[i].type = glow_eType_Adjustment;
        attrinfo[i++].size = sizeof( op->column_adjustment[0]);
      }
      if ( (name = growapi_translate( transtab, "HeaderText12"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = op->header_text[11];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->header_text[0]);
      }

      break;
    }
    case glow_eObjectType_GrowFolder:
    {
      GrowFolder *op = (GrowFolder *)object;
      char *name;

      if ( (name = growapi_translate( transtab, "Folders"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->folders;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->folders);
      }
      if ( (name = growapi_translate( transtab, "HeaderHeight"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->header_height;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->header_height);
      }
      if ( (name = growapi_translate( transtab, "ColorSelected"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->color_selected;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->color_selected);
      }
      if ( (name = growapi_translate( transtab, "ColorUnselected"))) {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->color_unselected;
        attrinfo[i].type = glow_eType_Color;
        attrinfo[i++].size = sizeof( op->color_unselected);
      }
      for ( int j = 0; j < 12; j++) {
	char tname[32];

	sprintf( tname, "FileName%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = op->folder_file_names[j];
	  attrinfo[i].type = glow_eType_String;
	  attrinfo[i++].size = sizeof( op->folder_file_names[0]);
	}
	sprintf( tname, "Text%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = op->folder_text[j];
	  attrinfo[i].type = glow_eType_String;
	  attrinfo[i++].size = sizeof( op->folder_text[0]);
	}
	sprintf( tname, "Scale%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = &op->folder_scale[j];
	  attrinfo[i].type = glow_eType_Double;
	  attrinfo[i++].size = sizeof( op->folder_scale[0]);
	}
	sprintf( tname, "VerticalScrollbar%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = &op->folder_v_scrollbar[j];
	  attrinfo[i].type = glow_eType_Boolean;
	  attrinfo[i++].size = sizeof( op->folder_v_scrollbar[0]);
	}
	sprintf( tname, "HorizontalScrollbar%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = &op->folder_h_scrollbar[j];
	  attrinfo[i].type = glow_eType_Boolean;
	  attrinfo[i++].size = sizeof( op->folder_h_scrollbar[0]);
	}
	sprintf( tname, "Owner%d", j+1);
	if ( (name = growapi_translate( transtab, tname))) {
	  strcpy( attrinfo[i].name, name);
	  attrinfo[i].value_p = op->folder_owner[j];
	  attrinfo[i].type = glow_eType_String;
	  attrinfo[i++].size = sizeof( op->folder_owner[0]);
	}
      }
      break;
    }
    case glow_eObjectType_GrowAxis:
    {
      GrowAxis *op = (GrowAxis *)object;
      char *name;
      char *dynamic;
      int  dynsize;

      if ( (name = growapi_translate( transtab, "MaxValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->max_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->max_value);
      }

      if ( (name = growapi_translate( transtab, "MinValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->min_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->min_value);
      }

      if ( (name = growapi_translate( transtab, "Lines")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->lines;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->lines);
      }

      if ( (name = growapi_translate( transtab, "LongQuotient")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->longquotient;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->longquotient);
      }

      if ( (name = growapi_translate( transtab, "ValueQuotient")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->valuequotient;
        attrinfo[i].type = glow_eType_Int;
        attrinfo[i++].size = sizeof( op->valuequotient);
      }

      if ( (name = growapi_translate( transtab, "Format")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->format;
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->format);
      }

      if ( (name = growapi_translate( transtab, "Dynamic")))
      {
        strcpy( attrinfo[i].name, name);
        op->get_dynamic( &dynamic, &dynsize);
        attrinfo[i].value_p = malloc( 1024);
        if ( dynsize)
          strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
        else
          strcpy( (char *) attrinfo[i].value_p, "");
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 1024;
        attrinfo[i].multiline = 1;
        attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      }
      break;
    }
    case glow_eObjectType_GrowNode:
    case glow_eObjectType_GrowGroup:
    {
      GrowNode *op = (GrowNode *)object;
      char *dynamic;
      int  dynsize;
      char *name;
      char annot_name[32];

      if ( (name = growapi_translate( transtab, "SubGraph")))
      {
        if ( ((GrowNode *)object)->type() == glow_eObjectType_GrowGroup)
          strcpy( attrinfo[i].name, "Group");
        else
          strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->nc->nc_name;
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].no_edit = 1;
        attrinfo[i++].size = sizeof( op->nc->nc_name);
      }
      
      for ( j = 0; j < 10; j++)
      {
        if ( op->nc->check_annotation( j))
        {
          sprintf( annot_name, "A%d", j);
          if ( (name = growapi_translate( transtab, annot_name)))
          {
            strcpy( attrinfo[i].name, name);
            attrinfo[i].value_p = malloc( 80);
            attrinfo[i].type = glow_eType_String;
            attrinfo[i].size = 80;
	    attrinfo[i].info_type = grow_eInfoType_Annot;
	    attrinfo[i].number = j;
            op->get_annotation( j, (char *)attrinfo[i].value_p, 80);
            i++;
          }
        }
      }

      for ( j = 0; j < op->nc->arg_cnt; j++)
      {
        strcpy( attrinfo[i].name, op->nc->argname[j]);
        attrinfo[i].value_p = malloc( 80);
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 80;
	attrinfo[i].info_type = grow_eInfoType_Arg;
	attrinfo[i].number = j;
        op->get_argument( j, (char *)attrinfo[i].value_p, 80);
        i++;
      }

      if ( (name = growapi_translate( transtab, "Dynamic")))
      {
        strcpy( attrinfo[i].name, name);
        op->get_dynamic( &dynamic, &dynsize);
        attrinfo[i].value_p = malloc( 1024);
        if ( dynsize)
          strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
        else
          strcpy( (char *) attrinfo[i].value_p, "");
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 1024;
        attrinfo[i].multiline = 1;
        attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      }
      break;
    }
    case glow_eObjectType_GrowSlider:
    {
      GrowSlider *op = (GrowSlider *)object;
      char *dynamic;
      int  dynsize;
      char *name;
      char annot_name[32];

      if ( (name = growapi_translate( transtab, "SubGraph")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->nc->nc_name;
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].no_edit = 1;
        attrinfo[i++].size = sizeof( op->nc->nc_name);
      }
      
      if ( (name = growapi_translate( transtab, "Direction")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->direction;
        attrinfo[i].type = glow_eType_Direction;
        attrinfo[i++].size = sizeof( op->direction);
      }

      if ( (name = growapi_translate( transtab, "MaxValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->max_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->max_value);
      }

      if ( (name = growapi_translate( transtab, "MinValue")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->min_value;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->min_value);
      }

      if ( (name = growapi_translate( transtab, "MaxPos")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->max_pos;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->max_pos);
      }
      
      if ( (name = growapi_translate( transtab, "MinPos")))
      {
        strcpy( attrinfo[i].name, name);
        attrinfo[i].value_p = &op->min_pos;
        attrinfo[i].type = glow_eType_Double;
        attrinfo[i++].size = sizeof( op->min_pos);
      }
      
      for ( j = 0; j < 10; j++)
      {
        if ( op->nc->check_annotation( j))
        {
          sprintf( annot_name, "A%d", j);
          if ( (name = growapi_translate( transtab, annot_name)))
          {
            strcpy( attrinfo[i].name, name);
            attrinfo[i].value_p = malloc( 80);
            attrinfo[i].type = glow_eType_String;
            attrinfo[i].size = 80;
	    attrinfo[i].info_type = grow_eInfoType_Annot;
	    attrinfo[i].number = j;
            op->get_annotation( j, (char *)attrinfo[i].value_p, 80);
            i++;
          }
        }
      }

      for ( j = 0; j < op->nc->arg_cnt; j++)
      {
        strcpy( attrinfo[i].name, op->nc->argname[j]);
        attrinfo[i].value_p = malloc( 80);
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 80;
	attrinfo[i].info_type = grow_eInfoType_Arg;
	attrinfo[i].number = j;
        op->get_argument( j, (char *)attrinfo[i].value_p, 80);
        i++;
      }

      if ( (name = growapi_translate( transtab, "Dynamic")))
      {
        strcpy( attrinfo[i].name, name);
        op->get_dynamic( &dynamic, &dynsize);
        attrinfo[i].value_p = malloc( 1024);
        if ( dynsize)
          strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
        else
          strcpy( (char *) attrinfo[i].value_p, "");
        attrinfo[i].type = glow_eType_String;
        attrinfo[i].size = 1024;
        attrinfo[i].multiline = 1;
        attrinfo[i++].info_type = grow_eInfoType_Dynamic;
      }
      break;
    }
    case glow_eObjectType_NodeClass:
    {
      GlowNodeClass *op = (GlowNodeClass *)object;

      strcpy( attrinfo[i].name, "Name");
      attrinfo[i].value_p = &op->nc_name;
      attrinfo[i].type = glow_eType_String;
      attrinfo[i++].size = sizeof( op->nc_name);

      strcpy( attrinfo[i].name, "Extern");
      attrinfo[i].value_p = &op->nc_extern;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->nc_extern);

      strcpy( attrinfo[i].name, "NoConObstacle");
      attrinfo[i].value_p = &op->no_con_obstacle;
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->no_con_obstacle);

      strcpy( attrinfo[i].name, "Color1");
      attrinfo[i].value_p = &op->dyn_color[0];
      attrinfo[i].type = glow_eType_TraceColor;
      attrinfo[i++].size = sizeof( op->dyn_color[0]);

      strcpy( attrinfo[i].name, "Color2");
      attrinfo[i].value_p = &op->dyn_color[1];
      attrinfo[i].type = glow_eType_TraceColor;
      attrinfo[i++].size = sizeof( op->dyn_color[0]);

      strcpy( attrinfo[i].name, "Attr1");
      attrinfo[i].value_p = &op->dyn_attr[0];
      attrinfo[i].type = glow_eType_Int;
      attrinfo[i++].size = sizeof( op->dyn_attr[0]);

      strcpy( attrinfo[i].name, "DynType");
      attrinfo[i].value_p = &op->dyn_type;
      attrinfo[i].type = glow_eType_DynType;
      attrinfo[i++].size = sizeof( op->dyn_type);

      strcpy( attrinfo[i].name, "Action");
      attrinfo[i].value_p = &op->dyn_action_type;
      attrinfo[i].type = glow_eType_ActionType;
      attrinfo[i++].size = sizeof( op->dyn_action_type);

      strcpy( attrinfo[i].name, "Cycle");
      attrinfo[i].value_p = &op->cycle;
      attrinfo[i].type = glow_eType_Cycle;
      attrinfo[i++].size = sizeof( op->cycle);

      for ( j = 0; j < op->arg_cnt; j++)
      {
        sprintf( attrinfo[i].name, "Arg%d", j+1);
        attrinfo[i].value_p = &op->argname[j];
        attrinfo[i].type = glow_eType_String;
        attrinfo[i++].size = sizeof( op->argname[0]);
      }
      break;
    }
    default:
      ;
  }
  attrinfo[i].info_type = grow_eInfoType_End;
  *attr_cnt = i;
  *info = attrinfo;
  return 1;
}

int grow_GetSubGraphAttrInfo( grow_tCtx ctx, char *transtab, grow_sAttrInfo **info,
	int *attr_cnt)
{
  grow_sAttrInfo *attrinfo;
  int i;
  char *dynamic;
  int  dynsize;
  char *name;

  attrinfo = (grow_sAttrInfo *) calloc( 30, sizeof(grow_sAttrInfo)); 

  i = 0;

  if ( (name = growapi_translate( transtab, "DynType"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_type;
    attrinfo[i].type = glow_eType_DynType;
    attrinfo[i++].size = sizeof( ctx->dyn_type);
  }
  if ( (name = growapi_translate( transtab, "DynActionType"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_action_type;
    attrinfo[i].type = glow_eType_ActionType;
    attrinfo[i++].size = sizeof( ctx->dyn_action_type);
  }
  if ( (name = growapi_translate( transtab, "DynColor1"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_color[0];
    attrinfo[i].type = glow_eType_TraceColor;
    attrinfo[i++].size = sizeof( ctx->dyn_color[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynColor2"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_color[1];
    attrinfo[i].type = glow_eType_TraceColor;
    attrinfo[i++].size = sizeof( ctx->dyn_color[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynColor3"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_color[2];
    attrinfo[i].type = glow_eType_TraceColor;
    attrinfo[i++].size = sizeof( ctx->dyn_color[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynColor4"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_color[3];
    attrinfo[i].type = glow_eType_TraceColor;
    attrinfo[i++].size = sizeof( ctx->dyn_color[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynAttr1"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_attr[0];
    attrinfo[i].type = glow_eType_Int;
    attrinfo[i++].size = sizeof( ctx->dyn_attr[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynAttr2"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_attr[1];
    attrinfo[i].type = glow_eType_Int;
    attrinfo[i++].size = sizeof( ctx->dyn_attr[0]);      
  }
  if ( (name = growapi_translate( transtab, "DynAttr3"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_attr[2];
    attrinfo[i].type = glow_eType_Int;
    attrinfo[i++].size = sizeof( ctx->dyn_attr[0]);
  }
  if ( (name = growapi_translate( transtab, "DynAttr4"))) {
    strcpy( attrinfo[i].name, name);
    attrinfo[i].value_p = &ctx->dyn_attr[3];
    attrinfo[i].type = glow_eType_Int;
    attrinfo[i++].size = sizeof( ctx->dyn_attr[0]);
  }

  strcpy( attrinfo[i].name, "NoConObstacle");
  attrinfo[i].value_p = &ctx->no_con_obstacle;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->no_con_obstacle);
      
  strcpy( attrinfo[i].name, "Slider");
  attrinfo[i].value_p = &ctx->slider;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->slider);
      
  strcpy( attrinfo[i].name, "Subgraph");
  attrinfo[i].value_p = &ctx->subgraph;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->subgraph);
      
  strcpy( attrinfo[i].name, "NextSubgraph");
  attrinfo[i].value_p = &ctx->next_subgraph;
  attrinfo[i].type = glow_eType_String;
  attrinfo[i++].size = sizeof( ctx->next_subgraph);
      
  strcpy( attrinfo[i].name, "AnimationCount");
  attrinfo[i].value_p = &ctx->animation_count;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->animation_count);
      
  strcpy( attrinfo[i].name, "JavaName");
  attrinfo[i].value_p = &ctx->java_name;
  attrinfo[i].type = glow_eType_String;
  attrinfo[i++].size = sizeof( ctx->java_name);
      
  strcpy( attrinfo[i].name, "Cycle");
  attrinfo[i].value_p = &ctx->cycle;
  attrinfo[i].type = glow_eType_Cycle;
  attrinfo[i++].size = sizeof( ctx->cycle);
      
  strcpy( attrinfo[i].name, "x0");
  attrinfo[i].value_p = &ctx->x0;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->x0);
      
  strcpy( attrinfo[i].name, "y0");
  attrinfo[i].value_p = &ctx->y0;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->y0);
      
  strcpy( attrinfo[i].name, "x1");
  attrinfo[i].value_p = &ctx->x1;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->x1);
      
  strcpy( attrinfo[i].name, "y1");
  attrinfo[i].value_p = &ctx->y1;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->y1);
      
  strcpy( attrinfo[i].name, "InputFocusMark");
  attrinfo[i].value_p = &ctx->input_focus_mark;
  attrinfo[i].type = glow_eType_InputFocusMark;
  attrinfo[i++].size = sizeof( ctx->input_focus_mark);
      
  strcpy( attrinfo[i].name, "Dynamic");
  ctx->get_dynamic( &dynamic, &dynsize);
  attrinfo[i].value_p = malloc( 1024);
  if ( dynsize)
    strncpy( (char *) attrinfo[i].value_p, dynamic, 1024);
  else
    strcpy( (char *) attrinfo[i].value_p, "");
  attrinfo[i].type = glow_eType_String;
  attrinfo[i].size = 1024;
  attrinfo[i].multiline = 1;
  attrinfo[i++].info_type = grow_eInfoType_Dynamic;

  attrinfo[i].info_type = grow_eInfoType_End;
  *attr_cnt = i;
  *info = attrinfo;
  return 1;
}

int grow_GetGraphAttrInfo( grow_tCtx ctx, grow_sAttrInfo **info,
	int *attr_cnt)
{
  grow_sAttrInfo *attrinfo;
  int i;

  attrinfo = (grow_sAttrInfo *) calloc( 20, sizeof(grow_sAttrInfo)); 

  i = 0;

  strcpy( attrinfo[i].name, "subgraph");
  attrinfo[i].value_p = &ctx->subgraph;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->subgraph);
      
  strcpy( attrinfo[i].name, "x0");
  attrinfo[i].value_p = &ctx->x0;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->x0);
      
  strcpy( attrinfo[i].name, "y0");
  attrinfo[i].value_p = &ctx->y0;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->y0);
      
  strcpy( attrinfo[i].name, "x1");
  attrinfo[i].value_p = &ctx->x1;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->x1);
      
  strcpy( attrinfo[i].name, "y1");
  attrinfo[i].value_p = &ctx->y1;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->y1);
      
  strcpy( attrinfo[i].name, "Scantime");
  attrinfo[i].value_p = &ctx->scantime;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->scantime);
      
  strcpy( attrinfo[i].name, "FastScantime");
  attrinfo[i].value_p = &ctx->fast_scantime;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->fast_scantime);
      
  strcpy( attrinfo[i].name, "AnimationScantime");
  attrinfo[i].value_p = &ctx->animation_scantime;
  attrinfo[i].type = glow_eType_Double;
  attrinfo[i++].size = sizeof( ctx->animation_scantime);
      
  strcpy( attrinfo[i].name, "JavaWidth");
  attrinfo[i].value_p = &ctx->java_width;
  attrinfo[i].type = glow_eType_Int;
  attrinfo[i++].size = sizeof( ctx->java_width);

  strcpy( attrinfo[i].name, "IsJavaApplet");
  attrinfo[i].value_p = &ctx->is_javaapplet;
  attrinfo[i].type = glow_eType_Boolean;
  attrinfo[i++].size = sizeof( ctx->is_javaapplet);

  strcpy( attrinfo[i].name, "IsJavaFrame");
  attrinfo[i].value_p = &ctx->is_javaapplication;
  attrinfo[i].type = glow_eType_Boolean;
  attrinfo[i++].size = sizeof( ctx->is_javaapplication);

  strcpy( attrinfo[i].name, "BackgroundImage");
  attrinfo[i].value_p = &ctx->background_image;
  attrinfo[i].type = glow_eType_String;
  attrinfo[i++].size = sizeof( ctx->background_image);
      
  strcpy( attrinfo[i].name, "BackgroundTiled");
  attrinfo[i].value_p = &ctx->background_tiled;
  attrinfo[i].type = glow_eType_Boolean;
  attrinfo[i++].size = sizeof( ctx->background_tiled);
      
  strcpy( attrinfo[i].name, "DoubleBuffered");
  attrinfo[i].value_p = &ctx->mw.window->double_buffered;
  attrinfo[i].type = glow_eType_Boolean;
  attrinfo[i++].size = sizeof( ctx->mw.window->double_buffered);
      
  strcpy( attrinfo[i].name, "MB3Action");
  attrinfo[i].value_p = &ctx->mb3_action;
  attrinfo[i].type = glow_eType_MB3Action;
  attrinfo[i++].size = sizeof( ctx->mb3_action);
      
  strcpy( attrinfo[i].name, "Translate");
  attrinfo[i].value_p = &ctx->translate_on;
  attrinfo[i].type = glow_eType_Boolean;
  attrinfo[i++].size = sizeof( ctx->translate_on);
      
  attrinfo[i].info_type = grow_eInfoType_End;
  *attr_cnt = i;
  *info = attrinfo;
  return 1;
}

void grow_FreeSubGraphAttrInfo( grow_sAttrInfo *attrinfo)
{
  grow_FreeObjectAttrInfo( attrinfo);
}

void grow_FreeGraphAttrInfo( grow_sAttrInfo *attrinfo)
{
  grow_FreeObjectAttrInfo( attrinfo);
}

void grow_FreeObjectAttrInfo( grow_sAttrInfo *attrinfo)
{
  grow_sAttrInfo *info_p;

  info_p = attrinfo;
  while ( info_p->info_type != grow_eInfoType_End)
  {
    switch( info_p->info_type)
    {
      case grow_eInfoType_Annot:
        free( (char *) info_p->value_p);
        break;
      case grow_eInfoType_Dynamic:
        free( (char *) info_p->value_p);
        break;
      default:
        ;
    }
    info_p++;
  }
  free( (char *)attrinfo);
}

void grow_UpdateObject(  grow_tCtx ctx, grow_tObject object,
	grow_sAttrInfo *info)
{
  grow_sAttrInfo *info_p;

  switch ( ((GrowRect *)object)->type())
  {
    case glow_eObjectType_GrowRect:
    case glow_eObjectType_GrowRectRounded:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowRect *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowRect *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowRect *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowRect *)object)->zoom();
      ((GrowRect *)object)->nav_zoom();
      ((GrowRect *)object)->get_node_borders();
      ((GrowRect *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowPolyLine:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowPolyLine *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowPolyLine *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowPolyLine *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowPolyLine *)object)->zoom();
      ((GrowPolyLine *)object)->nav_zoom();
      ((GrowPolyLine *)object)->get_node_borders();
      ((GrowPolyLine *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowLine:
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowLine *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowLine *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowLine *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }
      ((GrowLine *)object)->zoom();
      ((GrowLine *)object)->nav_zoom();
      ((GrowLine *)object)->get_node_borders();
      ((GrowLine *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowArc:
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowArc *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowArc *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowArc *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }
      ((GrowArc *)object)->zoom();
      ((GrowArc *)object)->nav_zoom();
      ((GrowArc *)object)->get_node_borders();
      ((GrowArc *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowConPoint:
      ((GrowConPoint *)object)->move(0,0,0);
      ((GrowConPoint *)object)->zoom();
      ((GrowConPoint *)object)->nav_zoom();
      ((GrowConPoint *)object)->get_node_borders();
      ((GrowConPoint *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowSubAnnot:
      ((GrowSubAnnot *)object)->text.text_size = ((GrowSubAnnot *)object)->text_size;
      sprintf( ((GrowSubAnnot *)object)->text.text, "A%d",
		((GrowSubAnnot *)object)->number);
      ((GrowSubAnnot *)object)->move(0,0,0);
      ((GrowSubAnnot *)object)->zoom();
      ((GrowSubAnnot *)object)->nav_zoom();
      ((GrowSubAnnot *)object)->get_node_borders();
      ((GrowSubAnnot *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowText:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowText *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowText *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowText *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowText *)object)->zoom();
      ((GrowText *)object)->nav_zoom();
      ((GrowText *)object)->get_node_borders();
      ((GrowText *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowImage:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowImage *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowImage *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowImage *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          case grow_eInfoType_Image:
          {
            ((GrowImage *)object)->insert_image( (char *)info_p->value_p);
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowImage *)object)->zoom();
      ((GrowImage *)object)->nav_zoom();
      ((GrowImage *)object)->get_node_borders();
      ((GrowImage *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowBar:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowBar *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowBar *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowBar *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowBar *)object)->zoom();
      ((GrowBar *)object)->nav_zoom();
      ((GrowBar *)object)->get_node_borders();
      ((GrowBar *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowTrend:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowTrend *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowTrend *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowTrend *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowTrend *)object)->configure_curves();
      ((GrowTrend *)object)->zoom();
      ((GrowTrend *)object)->nav_zoom();
      ((GrowTrend *)object)->get_node_borders();
      ((GrowTrend *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowWindow:
      ((GrowWindow *)object)->update_attributes();
      break;
    case glow_eObjectType_GrowTable:
      ((GrowTable *)object)->update_attributes();
      break;
    case glow_eObjectType_GrowFolder:
      ((GrowFolder *)object)->update_attributes();
      break;
    case glow_eObjectType_GrowAxis:
      // Set changed dynamic
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowAxis *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowTrend *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowAxis *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowAxis *)object)->configure();
      ((GrowAxis *)object)->zoom();
      ((GrowAxis *)object)->nav_zoom();
      ((GrowAxis *)object)->get_node_borders();
      ((GrowAxis *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowNode:
    case glow_eObjectType_GrowGroup:
      // Set changed annotations
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Annot:
          {
            char annot_value[80];
            ((GrowNode *)object)->get_annotation( info_p->number, annot_value, 
			sizeof(annot_value));
            if ( strcmp( annot_value, (char *) info_p->value_p) != 0)
              ((GrowNode *)object)->set_annotation( info_p->number, 
		(char *)info_p->value_p, strlen( (char *)info_p->value_p), 0);
            break;
          }
          case grow_eInfoType_Arg:
          {
            ((GrowNode *)object)->set_argument( info_p->number, 
		(char *)info_p->value_p, strlen( (char *)info_p->value_p));
            break;
          }
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowNode *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowNode *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowNode *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowNode *)object)->move(0,0,0);
      ((GrowNode *)object)->zoom();
      ((GrowNode *)object)->nav_zoom();
      ((GrowNode *)object)->get_node_borders();
      ((GrowNode *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    case glow_eObjectType_GrowSlider:
      // Set changed annotations
      info_p = info;
      while ( info_p->info_type != grow_eInfoType_End)
      {
        switch( info_p->info_type)
        {
          case grow_eInfoType_Annot:
          {
            char annot_value[80];
            ((GrowSlider *)object)->get_annotation( info_p->number, annot_value, 
			sizeof(annot_value));
            if ( strcmp( annot_value, (char *) info_p->value_p) != 0)
              ((GrowSlider *)object)->set_annotation( info_p->number, 
		(char *)info_p->value_p, strlen( (char *)info_p->value_p), 0);
            break;
          }
          case grow_eInfoType_Arg:
          {
            ((GrowSlider *)object)->set_argument( info_p->number, 
		(char *)info_p->value_p, strlen( (char *)info_p->value_p));
            break;
          }
          case grow_eInfoType_Dynamic:
          {
            char *dynamic;
	    int  dynsize;

            ((GrowSlider *)object)->get_dynamic( &dynamic, &dynsize);
	    if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
              break;

            if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
              ((GrowSlider *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            else if ( !dynsize)
              ((GrowSlider *)object)->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
            break;
          }
          default:
            ;
        }
        info_p++;
      }

      ((GrowNode *)object)->move(0,0,0);
      ((GrowNode *)object)->zoom();
      ((GrowNode *)object)->nav_zoom();
      ((GrowNode *)object)->get_node_borders();
      ((GrowNode *)object)->draw( &ctx->mw, INT_MIN, INT_MIN, INT_MAX, INT_MAX);
      break;
    default:
      ;
  }
  ctx->zoom( 1);
}

void grow_UpdateSubGraph(  grow_tCtx ctx, grow_sAttrInfo *info)
{
  grow_sAttrInfo *info_p;

  // Set changed dynamic
  info_p = info;
  while ( info_p->info_type != grow_eInfoType_End)
  {
    switch( info_p->info_type)
    {
      case grow_eInfoType_Dynamic:
      {
        char *dynamic;
        int  dynsize;

        ctx->get_dynamic( &dynamic, &dynsize);
        if ( strcmp( (char *) info_p->value_p, "") == 0 && !dynsize)
          break;

        if ( dynsize && strcmp( dynamic, (char *) info_p->value_p) != 0)
          ctx->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
        else if ( !dynsize)
          ctx->set_dynamic( (char *)info_p->value_p,
			strlen( (char *)info_p->value_p));
        break;
      }
      default:
        ;
    }
    info_p++;
  }
  ctx->zoom( 1);
}

void grow_UpdateGraph(  grow_tCtx ctx, grow_sAttrInfo *info)
{
  grow_sAttrInfo *info_p;

  // Set changed dynamic
  info_p = info;
  while ( info_p->info_type != grow_eInfoType_End)
  {
    switch( info_p->info_type)
    {
      default:
        ;
    }
    info_p++;
  }
  ctx->zoom( 1);
}

int grow_CheckAnnotation( grow_tNodeClass nodeclass, int num)
{
  return ((GlowNodeClass *)nodeclass)->check_annotation( num);
}

void grow_New( grow_tCtx ctx)
{
  ctx->clear_all( 1);
  ctx->reset_background();
}

void grow_SetName( grow_tCtx ctx, char *name)
{
  ctx->set_name( name);
}

void grow_GetName( grow_tCtx ctx, char *name)
{
  ctx->get_name( name);
}

void grow_PolylineEnd( grow_tCtx ctx)
{
  ctx->polyline_end();
}

void grow_AddPolyLinePoints( grow_tObject object, glow_sPoint *point_array, 
	int point_cnt)
{
  ((GrowPolyLine *)object)->add_points( point_array, point_cnt);
}

void grow_SetObjectFill( grow_tObject object, int fill)
{
  ((GlowArrayElem *)object)->set_fill( fill);
}

void grow_SetObjectShadow( grow_tObject object, int shadow)
{
  ((GlowArrayElem *)object)->set_shadow( shadow);
}

void grow_SetObjectShadowWidth( grow_tObject object, double width)
{
  ((GlowArrayElem *)object)->set_shadow_width( width);
}

void grow_SetObjectDrawtype( grow_tObject object, glow_eDrawType drawtype)
{
  ((GlowArrayElem *)object)->set_drawtype( drawtype);
}

void grow_SetObjectLinewidth( grow_tObject object, int linewidth)
{
  ((GlowArrayElem *)object)->set_linewidth( linewidth);
}

void grow_SetObjectLinetype( grow_tObject object, glow_eLineType type)
{
  ((GlowArrayElem *)object)->set_linetype( type);
}

void grow_ExecDynamic( grow_tCtx ctx)
{
  ctx->exec_dynamic();
}

void grow_SetObjectPosition( grow_tObject object, double x, double y)
{
  ((GlowArrayElem *)object)->set_position( x, y);
}

void grow_SetObjectScale( grow_tObject object, double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  ((GlowArrayElem *)object)->set_scale( scale_x, scale_y, x0, y0, type);
}

void grow_SetObjectScalePos( grow_tObject object, double x, double y, 
	double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  ((GlowArrayElem *)object)->set_scale_pos( x, y, scale_x, scale_y, x0, y0, 
					    type);
}

void grow_SetObjectRotation( grow_tObject object, double angel, double x0,
	double y0, glow_eRotationPoint type)
{
  ((GlowArrayElem *)object)->set_rotation( angel, x0, y0, type);
}

void grow_SetObjectFillColor( grow_tObject object, glow_eDrawType drawtype)
{
  ((GlowArrayElem *)object)->set_fill_color( drawtype);
}

void grow_ResetObjectFillColor( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_fill_color();
}

void grow_SetObjectBorderColor( grow_tObject object, glow_eDrawType drawtype)
{
  ((GlowArrayElem *)object)->set_border_color( drawtype);
}

void grow_ResetObjectBorderColor( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_border_color();
}

void grow_SetObjectTextColor( grow_tObject object, glow_eDrawType drawtype)
{
  ((GlowArrayElem *)object)->set_text_color( drawtype);
}

void grow_ResetObjectTextColor( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_text_color();
}

void grow_SetSelectOrigBorderColor( grow_tCtx ctx, glow_eDrawType drawtype)
{
  ctx->set_select_original_border_color( drawtype);
}

void grow_SetSelectOrigTextColor( grow_tCtx ctx, glow_eDrawType drawtype)
{
  ctx->set_select_original_text_color( drawtype);
}

void grow_SetSelectOrigFillColor( grow_tCtx ctx, glow_eDrawType drawtype)
{
  ctx->set_select_original_fill_color( drawtype);
}

void grow_SetSelectOrigColorTone( grow_tCtx ctx, glow_eDrawTone tone)
{
  ctx->set_select_original_color_tone( tone);
}

void grow_SetSelectOrigColLightness( grow_tCtx ctx, int lightness)
{
  ctx->set_select_original_color_lightness( lightness);
}

void grow_IncrSelectOrigColLightness( grow_tCtx ctx, int lightness)
{
  ctx->incr_select_original_color_lightness( lightness);
}

void grow_SetSelectOrigColIntensity( grow_tCtx ctx, int intensity)
{
  ctx->set_select_original_color_intensity( intensity);
}

void grow_IncrSelectOrigColIntensity( grow_tCtx ctx, int intensity)
{
  ctx->incr_select_original_color_intensity( intensity);
}

void grow_SetSelectOrigColorShift( grow_tCtx ctx, int shift)
{
  ctx->set_select_original_color_shift( shift);
}

void grow_IncrSelectOrigColorShift( grow_tCtx ctx, int shift)
{
  ctx->incr_select_original_color_shift( shift);
}

void grow_SetObjectColorTone( grow_tObject object, glow_eDrawTone tone)
{
  ((GlowArrayElem *)object)->set_color_tone( tone);
}

void grow_ResetObjectColorTone( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_color_tone();
}

void grow_SetObjectColorLightness( grow_tObject object, int lightness)
{
  ((GlowArrayElem *)object)->set_color_lightness( lightness);
}

void grow_ResetObjectColorLightness( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_color_lightness();
}

void grow_SetObjectColorIntensity( grow_tObject object, int intensity)
{
  ((GlowArrayElem *)object)->set_color_intensity( intensity);
}

void grow_ResetObjectColorIntensity( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_color_intensity();
}

void grow_SetObjectColorShift( grow_tObject object, int shift)
{
  ((GlowArrayElem *)object)->set_color_shift( shift);
}

void grow_IncrObjectColorShift( grow_tObject object, int shift)
{
  ((GlowArrayElem *)object)->incr_color_shift( shift);
}

void grow_ResetObjectColorShift( grow_tObject object)
{
  ((GlowArrayElem *)object)->reset_color_shift();
}

void grow_SetObjectColorInverse( grow_tObject object, int inverse)
{
  if ( ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)object)->set_color_inverse( inverse);
}

void grow_SetObjectVisibility( grow_tObject object, glow_eVis visibility)
{
  ((GlowArrayElem *)object)->set_visibility( visibility);
}

void grow_RotateSelectedObjects( grow_tCtx ctx, double angel, 
	glow_eRotationPoint type)
{
  ctx->rotate_select( angel, type);
}

void grow_FlipSelectedObjects( grow_tCtx ctx, glow_eFlipDirection dir)
{
  ctx->flip_select( dir);
}

void grow_SetBackgroundColor( grow_tCtx ctx, glow_eDrawType color)
{
  ctx->set_background( color);
}

void grow_GetBackgroundColor( grow_tCtx ctx, glow_eDrawType *color)
{
  ctx->get_background( color);
}

void grow_SetBackgroundImage( grow_tCtx ctx, char *image)
{
  ctx->set_background_image( image);
}

void grow_GetBackgroundImage( grow_tCtx ctx, char *image, int *tiled)
{
  ctx->get_background_image( image);
  *tiled = ctx->background_tiled;
}

int grow_GetBackgroundImageSize( grow_tCtx ctx, int *width, int *height)
{
  return ctx->get_background_image_size( width, height);
}

void grow_ResetBackgroundColor( grow_tCtx ctx)
{
  ctx->reset_background();
}

void grow_DisableHighlight( grow_tCtx ctx)
{
  ctx->disable_highlight();
}

void grow_EnableHighlight( grow_tCtx ctx)
{
  ctx->enable_highlight();
}

void grow_RefSubGraphArgNames( grow_tCtx ctx, char **argnames, int **argtypes,
	int **arg_cnt)
{
  *argnames = (char *)ctx->argname;
  *argtypes = ctx->argtype;
  *arg_cnt = &ctx->arg_cnt;
}

void grow_GetSubGraphDynamic( grow_tCtx ctx, char **dynamic, int *size)
{
  *dynamic = ctx->dynamic;
  *size = ctx->dynamicsize;
}

void grow_GetObjectArgs( grow_tObject object, char **argnames, int **argtypes,
	char ***argvalues, int *arg_cnt)
{
  *argnames = (char *) ((GrowNode *)object)->nc->argname;
  *argtypes = ((GrowNode *)object)->nc->argtype;
  *argvalues = ((GrowNode *)object)->argv;
  *arg_cnt = ((GrowNode *)object)->nc->arg_cnt;
}

void grow_PushSelectedObjects( grow_tCtx ctx)
{
  ctx->push_select();
}

void grow_PopSelectedObjects( grow_tCtx ctx)
{
  ctx->pop_select();
}

void grow_SetDefaultLayout( grow_tCtx ctx)
{
  ctx->set_default_layout();
}

void grow_SetLayout( grow_tCtx ctx, double x0, double y0, double x1,
		     double y1)
{
  ctx->set_layout( x0, y0, x1, y1);
}

void grow_SetPath( grow_tCtx ctx, int path_cnt, char *path)
{
  ctx->set_path( path_cnt, path);
}

void grow_GetObjectClassDynType( grow_tObject object, int *dyn_type,
				   int *dyn_action_type)
{
  if ( ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowGroup) {

    GlowNodeClass *base_nc = ((GrowNode *)object)->nc->get_base_nc();
    *dyn_type = base_nc->dyn_type;
    *dyn_action_type = base_nc->dyn_action_type;
  }
  else {
    *dyn_type = 0;
    *dyn_action_type = 0;
  }
}

void grow_GetObjectClassCycle( grow_tObject object, glow_eCycle *cycle)
{
  GlowNodeClass *base_nc = ((GrowNode *)object)->nc->get_base_nc();
  *cycle = base_nc->cycle;
}

void grow_GetNodeClassDynType( grow_tNodeClass nodeclass, int *dyn_type,
				 int *dyn_action_type)
{
  GlowNodeClass *base_nc = ((GlowNodeClass *)nodeclass)->get_base_nc();
  *dyn_type = base_nc->dyn_type;
  *dyn_action_type = base_nc->dyn_action_type;
}

void grow_GetObjectClassUserData( grow_tObject object, void **user_data)
{
  GlowNodeClass *base_nc = ((GrowNode *)object)->nc->get_base_nc();
  base_nc->get_user_data( user_data);
}

void grow_GetSubGraphDynType( grow_tCtx ctx, int *dyn_type, int *dyn_action_type)
{
  *dyn_type = ctx->dyn_type;
  *dyn_action_type = ctx->dyn_action_type;
}

void grow_GetSubGraphTraceColor( grow_tCtx ctx, glow_eDrawType *color,
	glow_eDrawType *color2)
{
  *color = ctx->dyn_color[0];
  *color2 = ctx->dyn_color[1];
}

void grow_GetObjectClassTraceColor( grow_tObject object, glow_eDrawType *color,
	glow_eDrawType *color2)
{
  *color = ((GrowNode *)object)->nc->dyn_color[0];
  *color2 = ((GrowNode *)object)->nc->dyn_color[1];
}

void grow_GetObjectClassDynAttr( grow_tObject object, int *attr1,
	int *attr2)
{
  *attr1 = ((GrowNode *)object)->nc->dyn_attr[0];
  *attr2 = ((GrowNode *)object)->nc->dyn_attr[1];
}

void grow_SetSelectLineWidth( grow_tCtx ctx, int linewidth)
{
  ctx->set_select_linewidth( linewidth);
}

void grow_SetSelectFill( grow_tCtx ctx, int fill)
{
  ctx->set_select_fill( fill);
}

void grow_SetSelectBorder( grow_tCtx ctx, int border)
{
  ctx->set_select_border( border);
}

void grow_SetSelectShadow( grow_tCtx ctx, int shadow)
{
  ctx->set_select_shadow( shadow);
}

void grow_SetSelectLineType( grow_tCtx ctx, glow_eLineType type)
{
  ctx->set_select_linetype( type);
}

int grow_AnySelectIsCon( grow_tCtx ctx)
{
  return ctx->any_select_is_con();
}

void grow_ChangeSelectConClass( grow_tCtx ctx, grow_tConClass conclass)
{
  ctx->change_select_conclass( (GlowArrayElem *)conclass);
}

void grow_GetConAttributes( grow_tCon con, glow_eDrawType *drawtype,
	int *linewidth, glow_eConType *contype, glow_eCorner *corner, 
	double *round_corner_amount)
{
  *drawtype = ((GlowCon *)con)->cc->draw_type;
  *linewidth = ((GlowCon *)con)->cc->line_width;
  *contype = ((GlowCon *)con)->cc->con_type;
  *corner = ((GlowCon *)con)->cc->corner;
  *round_corner_amount = ((GlowCon *)con)->cc->round_corner_amount;
}

void grow_ChangeConConClass( grow_tCon con, grow_tConClass conclass)
{
  ((GlowCon *)con)->change_conclass( (GlowConClass *)conclass);
}

static char *growapi_translate( char *transtab, char *name)
{
  static char return_name[32];
  char	*name_p;

  if ( !transtab)
  {
    strcpy( return_name, name);
    return return_name;
  }

  name_p = transtab;
  while( strcmp( name_p, "") != 0)
  {
    if ( strcmp( name_p, name) == 0)
    {
      name_p += 32;
      if ( strcmp( name_p, "") == 0)
        return 0;
      else
      {
        strcpy( return_name, name_p);
        return return_name;
      }
    }
    name_p += 64;
  }
  // Not found, return the untranslated name
  strcpy( return_name, name);
  return return_name;
}

int grow_FindObject( grow_tCtx ctx, grow_tObject object)
{
  return ctx->find( (GlowArrayElem *)object);
}

void grow_SetObjectText( grow_tObject object, char *text)
{
  ((GrowText *)object)->set_text( text);
}

void grow_GetObjectText( grow_tObject object, char *text)
{
  ((GrowText *)object)->get_text( text);
}

void grow_SetSelectTextSize( grow_tCtx ctx, int size)
{
  ctx->set_select_textsize( size);
}

void grow_SetSelectTextBold( grow_tCtx ctx, int bold)
{
  ctx->set_select_textbold( bold);
}

void grow_SetSelectTextFont( grow_tCtx ctx, glow_eFont font)
{
  ctx->set_select_textfont( font);
}

void grow_SetBarValue( grow_tObject object, double value)
{
  ((GrowBar *)object)->set_value( value);
}

void grow_AddTrendValue( grow_tObject object, double value, int idx)
{
 ((GrowTrend *)object)->add_value( value, idx);
}

int grow_IsSliderClass( grow_tNodeClass nodeclass)
{
  return ((GlowNodeClass *)nodeclass)->is_slider();
}

void grow_SetMoveRestrictions( grow_tCtx ctx, glow_eMoveRestriction restriction,
	double max_limit, double min_limit, grow_tObject object)
{
  ctx->set_move_restrictions( restriction, max_limit, min_limit, 
	(GlowArrayElem *)object);
}

glow_eMoveRestriction grow_GetMoveRestrictions( grow_tCtx ctx)
{
  return ctx->move_restriction;
}

void grow_SetScaleEqual( grow_tCtx ctx, int equal)
{
  ctx->set_scale_equal( equal);
}

int grow_GetScaleEqual( grow_tCtx ctx)
{
  return ctx->scale_equal;
}

void grow_GetSliderInfo( grow_tObject object, glow_eDirection *direction,
	double *max_value, double *min_value, double *max_pos, double *min_pos)
{
  ((GrowSlider *)object)->get_info( direction, max_value, min_value, 
	max_pos, min_pos);
}

void grow_GetSliderInfoPixel( grow_tObject object, glow_eDirection *direction,
	double *max_pos, double *min_pos, int bg_dyntype)
{
  ((GrowSlider *)object)->get_info_pixel( direction, max_pos, min_pos, bg_dyntype);
}

void grow_GetSliderOrigo( grow_tObject object, glow_eDirection direction,
			  double *pos)
{
  ((GrowSlider *)object)->get_origo( direction, pos);
}

void grow_SetSliderInfo( grow_tObject object, glow_eDirection direction,
	double max_value, double min_value, double max_pos, double min_pos)
{
  ((GrowSlider *)object)->set_info( direction, max_value, min_value, 
	max_pos, min_pos);
}

void grow_ObjectToCtx( grow_tObject object, grow_tCtx *ctx)
{
  ((GlowArrayElem *)object)->get_ctx( (void **) ctx);
}

void grow_StoreTransform( grow_tObject object)
{
  ((GlowArrayElem *)object)->store_transform();
}

int grow_TransformIsStored( grow_tObject object)
{
  return (int) ((GlowArrayElem *)object)->transform_is_stored();
}

void grow_AlignSelect( grow_tCtx ctx, glow_eAlignDirection direction)
{
  ctx->align_select( direction);
}

void grow_EquiDistanceSelect( grow_tCtx ctx, glow_eAlignDirection direction)
{
  ctx->equidistance_select( direction);
}

int grow_IsSubGraph( grow_tCtx ctx)
{
  return ctx->is_subgraph();
}

void grow_GetTrendScanTime( grow_tObject object, double *time)
{
  ((GrowTrend *)object)->get_scan_time( time);
}

void grow_SetTrendScanTime( grow_tObject object, double time)
{
  ((GrowTrend *)object)->set_scan_time( time);
}

void grow_SetSliderRange( grow_tObject object, double min, double max)
{
  ((GrowSlider *)object)->set_range( min, max);
}

void grow_SetBarRange( grow_tObject object, double min, double max)
{
  ((GrowBar *)object)->set_range( min, max);
}

void grow_SetTrendRangeY( grow_tObject object, int curve, 
	double min, double max)
{
  ((GrowTrend *)object)->set_range_y( curve, min, max);
}

void grow_SetTrendXYRangeY( grow_tObject object, int curve, 
	double min, double max)
{
  ((GrowTrend *)object)->set_xy_range_y( curve, min, max);
}

void grow_SetTrendXYRangeX( grow_tObject object, int curve, 
	double min, double max)
{
  ((GrowTrend *)object)->set_xy_range_x( curve, min, max);
}

void grow_SetTrendXYNoOfCurves( grow_tObject object, int noofcurves)
{
  ((GrowTrend *)object)->set_xy_noofcurves( noofcurves);
}

void grow_SetTrendFillCurve( grow_tObject object, int fill)
{
  ((GrowTrend *)object)->set_fill_curve( fill);
}

void grow_SetTrendLines( grow_tObject object, int vert_lines, 
	int horiz_lines)
{
  ((GrowTrend *)object)->set_lines( vert_lines, horiz_lines);
}

void grow_SetAxisConf( grow_tObject object, double max_val, double min_val, 
     int no_of_lines, int long_quot, int value_quot, double rot, char *format)
{
  ((GrowAxis *)object)->set_conf( max_val, min_val, no_of_lines,
      long_quot, value_quot, rot, format);
}

void grow_SetModified( grow_tCtx ctx, int modified)
{
  ctx->set_modified( modified);
}

int grow_GetModified( grow_tCtx ctx)
{
  return ctx->get_modified();
}

void grow_MeasureJavaBean( grow_tCtx ctx, double *x_right, double *x_left,
	double *y_high, double *y_low)
{
  ctx->measure_javabean( x_right, x_left, y_high, y_low);
}

void grow_ToPixel( grow_tCtx ctx, double x, double y,
		   double *x_pix, double *y_pix)
{
  ctx->to_pixel( x, y, x_pix, y_pix);
}

void grow_SetJavaFrame( grow_tCtx ctx, double *x_right, double *x_left,
	double *y_high, double *y_low)
{
  ctx->set_javaframe( x_right, x_left, y_high, y_low);
}

void grow_MeasureNodeClassJavaBean( grow_tNodeClass nodeclass, double *x_right, double *x_left,
	double *y_high, double *y_low)
{
  ((GlowNodeClass *)nodeclass)->measure_javabean( x_right, x_left, y_high, y_low);
}

void grow_ExportJavaBean( grow_tCtx ctx, ofstream& fp, int components)
{
  ctx->export_javabean( fp, components);
}

void grow_ExportNodeClassJavaBean( grow_tCtx ctx, grow_tNodeClass nc, 
	ofstream& fp, int components)
{
  ctx->export_nodeclass_javabean( (GlowArrayElem *)nc, fp, components);
}

void grow_ExportNcJavaBeanFont( grow_tCtx ctx, grow_tNodeClass nc, 
	ofstream& fp, int components)
{
  ctx->export_nc_javabean_font( (GlowArrayElem *)nc, fp, components);
}

void grow_SetJavaName( grow_tCtx ctx, char *name)
{
  ctx->set_java_name( name);
}

int grow_GetJavaName( grow_tCtx ctx, char *name)
{
  return ctx->get_java_name( name);
}

int grow_IsJavaApplet( grow_tCtx ctx)
{
  return ctx->is_javaapplet;
}

int grow_IsJavaApplication( grow_tCtx ctx)
{
  return ctx->is_javaapplication;
}

int grow_GetObjectClassJavaName( grow_tObject object, char *name)
{
  return ((GrowNode *)object)->nc->get_java_name( name);
}

int grow_GetNodeClassJavaName( grow_tNodeClass nodeclass, char *name)
{
  return ((GlowNodeClass *)nodeclass)->get_java_name( name);
}

void grow_GetObjectAnnotationNumbers( grow_tObject object, int **numbers,
	int *cnt)
{
  if ( ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)object)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)object)->nc->get_annotation_numbers( numbers, cnt);
  else {
    *cnt = 0;
    *numbers = 0;
  }
}

void grow_GetSubgraphAnnotNumbers( grow_tCtx ctx, int **numbers,
	int *cnt)
{
  ctx->get_annotation_numbers( numbers, cnt);
}

int grow_IsSlider( grow_tCtx ctx)
{
  return ctx->is_slider();
}

char *grow_ColorToName( glow_eDrawType drawtype)
{
  return GlowColor::color_to_name( drawtype);
}

char *grow_ColorToneToName( glow_eDrawType drawtype)
{
  return GlowColor::colortone_to_name( drawtype);
}

int grow_SetObjectNextNodeClass( grow_tObject object)
{
  return ((GrowNode *)object)->set_next_nodeclass();
}

int grow_SetObjectPrevNodeClass( grow_tObject object)
{
  return ((GrowNode *)object)->set_previous_nodeclass();
}

void grow_SetObjectFirstNodeClass( grow_tObject object)
{
  ((GrowNode *)object)->set_root_nodeclass();
}

void grow_SetObjectLastNodeClass( grow_tObject object)
{
  ((GrowNode *)object)->set_last_nodeclass();
}

int grow_SetObjectNodeClassByIndex( grow_tObject object, int idx)
{
  return ((GrowNode *)object)->set_nodeclass_by_index( idx);
}

void grow_GetObjectAnimationCount( grow_tObject object, int *count)
{
  ((GrowNode *)object)->get_animation_count( count);
}

void grow_GetScanTime( grow_tCtx ctx, double *scantime, double *fast_scantime,
	double *animation_scantime)
{
  *scantime = ctx->scantime;
  *fast_scantime = ctx->fast_scantime;
  *animation_scantime = ctx->animation_scantime;
}

void grow_SetShowGrid( grow_tCtx ctx, int show) 
{
  ctx->set_show_grid( show);
}

int grow_GetShowGrid( grow_tCtx ctx) 
{
  return ctx->get_show_grid();
}

int grow_IsNextNodeClass( grow_tNodeClass nodeclass)
{
  return ((GlowNodeClass *)nodeclass)->is_next();
}

int grow_GetNodeClassPages( grow_tNodeClass nodeclass)
{
  return ((GlowNodeClass *)nodeclass)->get_pages();
}

void grow_SetNextSubgraph( grow_tCtx ctx, char *next)
{
  strcpy( ctx->next_subgraph, next);
}

void grow_GetNextSubgraph( grow_tCtx ctx, char *next)
{
  strcpy( next, ctx->next_subgraph);
}

void grow_StoreGeometry( grow_tCtx ctx)
{
  ctx->store_geometry();
}

void grow_RestoreGeometry( grow_tCtx ctx)
{
  ctx->restore_geometry();
}

void grow_SetClickSensitivity( grow_tCtx ctx, int value)
{
  ctx->gdraw->set_click_sensitivity( &ctx->mw, value);
}

void grow_MoveNode( grow_tNode node, double x, double y)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowGroup)
    ((GrowNode *)node)->move_to( x, y);
}

void grow_GetTextExtent( grow_tCtx ctx, char *text, int len, 
	glow_eDrawType draw_type, int text_size, glow_eFont font, double *width,
	double *height, double *descent)
{
  ctx->get_text_extent( text, len, draw_type, text_size, font,
	width, height, descent);
}

int grow_GetDefaultWindowSize( grow_tCtx ctx, int *width, int *height)
{
  return ctx->get_default_window_size( width, height);
}

int grow_GroupSelect( grow_tCtx ctx, grow_tObject* group, char *last_group_name)
{
  return ctx->group_select( (GlowArrayElem **)group, last_group_name);
}

int grow_UngroupSelect( grow_tCtx ctx)
{
  return ctx->ungroup_select();
}

void grow_CurveConfigure( grow_tObject object, glow_sCurveData *data)
{
  ((GrowCurve *)object)->configure_curves( data);
}

void grow_CurveAddPoints( grow_tObject object, glow_sCurveData *data)
{
  ((GrowCurve *)object)->add_points( data);
}

int grow_GetObjectGroup( grow_tCtx ctx, grow_tObject object, 
			  grow_tObject *group)
{
  return ctx->get_object_group( (GlowArrayElem *)object, 
				(GlowArrayElem **)group);
}

void grow_GetGroupObjectList( grow_tObject group, grow_tObject **list, 
			      int *cnt)
{
  ((GrowGroup *)group)->get_objectlist( (GlowArrayElem ***)list, cnt);
}

void grow_SetNodeClassExtern( grow_tNodeClass nodeclass, int ext)
{
  ((GlowNodeClass *)nodeclass)->set_extern( ext);
}

int grow_GetBackgroundObjectLimits( grow_tCtx ctx, glow_eTraceType type,
	double x, double y, grow_tObject *background,
	double *min, double *max, glow_eDirection *direction)
{
  return ctx->get_background_object_limits( type, x, y, 
		  (GlowArrayElem **)background, min, max, direction);
}

int grow_GetTranslate( grow_tCtx ctx)
{
  return ctx->translate_on;
}

glow_eMB3Action grow_GetMB3Action( grow_tCtx ctx)
{
  return ctx->mb3_action;
}

void grow_GetNodeClassOrigo( grow_tObject object, double *x, double *y)
{
  ((GrowNode *)object)->get_nodeclass_origo((GlowTransform *)NULL, x, y);
}

int grow_ImageUpdate( grow_tObject object)
{
  return ((GrowImage *)object)->update();
}

#if defined OS_VMS
extern "C" 
#endif
void grow_RegisterUserDataCallbacks( grow_tCtx ctx,
				     void (*save)( void *, void *, glow_eUserdataCbType),
				     void (*open)( void *, void *, glow_eUserdataCbType),
				     void (*copy)( void *, void *, void **, glow_eUserdataCbType))
{
  ((GrowCtx *)ctx)->register_userdata_callbacks( 
	   (glow_tUserDataSaveCb) save,
	   (glow_tUserDataOpenCb) open,
	   (glow_tUserDataCopyCb) copy);
}

void grow_GetVersion( grow_tCtx ctx, int *grow_version, int *graph_version)
{
  *graph_version = ctx->version;
  *grow_version = GLOW_VERSION;
}

void grow_UpdateVersion( grow_tCtx ctx)
{
  ctx->version = GLOW_VERSION;
}

void grow_SetTipText( grow_tCtx ctx, grow_tObject object, char *text, int x, int y)
{
  ctx->tiptext->draw_text( (GlowArrayElem *)object, text, x, y);
}

void grow_SetObjectLevelFillColor( grow_tObject object, glow_eDrawType color)
{
  ((GrowNode *)object)->set_level_fill_color( color);
}

void grow_SetObjectLevelColorTone( grow_tObject object, glow_eDrawTone tone)
{
  ((GrowNode *)object)->set_level_color_tone( tone);
}

void grow_SetObjectLevelDirection( grow_tObject object, glow_eDirection direction)
{
  ((GrowNode *)object)->set_level_direction( direction);
}

void grow_SetObjectFillLevel( grow_tObject object, double level)
{
  ((GrowNode *)object)->set_fill_level( level);
}

int grow_GetObjectLimits( grow_tObject object, double *min, double *max, 
			   glow_eDirection *direction)
{
  return ((GrowNode *)object)->get_limits( min, max, direction);
}

int grow_GetObjectLimitsPixel( grow_tObject object, double *min, double *max,
			       glow_eDirection *direction)
{
  return ((GrowNode *)object)->get_limits_pixel( min, max, direction);
}

void grow_SetTrendInfo( grow_tObject object, glow_sTrendInfo *info)
{
  ((GrowTrend *)object)->set_trend_info( info);
}

void grow_SetBarInfo( grow_tObject object, glow_sBarInfo *info)
{
  ((GrowBar *)object)->set_bar_info( info);
}

void grow_GetBarInfo( grow_tObject object, glow_sBarInfo *info)
{
  ((GrowBar *)object)->get_bar_info( info);
}

void grow_SetAxisInfo( grow_tObject object, glow_sAxisInfo *info)
{
  ((GrowAxis *)object)->set_axis_info( info);
}

void grow_SetTableInfo( grow_tObject object, glow_sTableInfo *info)
{
  ((GrowTable *)object)->set_table_info( info);
}

void grow_GetTableInfo( grow_tObject object, glow_sTableInfo *info)
{
  ((GrowTable *)object)->get_table_info( info);
}

void grow_SetCellValue( grow_tObject object, int column, int row, char *value)
{
  ((GrowTable *)object)->set_cell_value( column, row, value);
}

void grow_SetSelectedCell( grow_tObject object, int column, int row)
{
  ((GrowTable *)object)->set_selected_cell( column, row);
}

int grow_GetSelectedCell( grow_tObject object, int *column, int *row)
{
  return ((GrowTable *)object)->get_selected_cell( column, row);
}

int grow_TableMakeCellVisible( grow_tObject object, int column, int row)
{
  return ((GrowTable *)object)->make_cell_visible( column, row);
}

void grow_Convert( grow_tCtx ctx, glow_eConvert version)
{
  ((GrowCtx *)ctx)->convert( version);
}

int grow_NodeClassIsExtern( grow_tNodeClass nodeclass)
{
  GlowNodeClass *base_nc = ((GlowNodeClass *)nodeclass)->get_base_nc();
  return base_nc->nc_extern;
}

int grow_GetNodeConPoint( grow_tNode node, int num, double *x, double *y, glow_eDirection *dir)
{
  if ( ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowNode ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowSlider ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowConGlue ||
       ((GlowArrayElem *)node)->type() == glow_eObjectType_GrowGroup)
    return ((GrowNode *)node)->get_conpoint( num, x, y, dir);
  return 0;
}

void grow_InputFocusInitEvent( grow_tCtx ctx)
{
  ((GrowCtx *)ctx)->inputfocus_init_event();
}

int grow_GetTrendNoOfPoints( grow_tObject object)
{
  return ((GrowTrend *)object)->get_no_of_points();
}

void grow_SetTrendData( grow_tObject object, double *data[3], int data_curves, int data_points)
{
 ((GrowTrend *)object)->set_data( data, data_curves, data_points);
}

void grow_SetTrendXYCurveColor( grow_tObject object, int curve, glow_eDrawType curve_color,
				glow_eDrawType fill_color)
{
  ((GrowTrend *)object)->set_xy_curve_color( curve, curve_color, fill_color);
}

void grow_SetTrendXYData( grow_tObject object, double *y_data, double *x_data, int curve_idx, 
			  int data_points)
{
  ((GrowTrend *)object)->set_xy_data( y_data, x_data, curve_idx, data_points);
}

int grow_GetObjectAnnotInfo( grow_tObject object, int num, int *text_size, glow_eDrawType *text_drawtype,
			glow_eDrawType *text_color, glow_eDrawType *bg_color, double *scale)
{
  return ((GrowNode *)object)->get_annotation_info( num, text_size, text_drawtype, text_color, bg_color,
						    scale);
}

void grow_GetMenuChar( grow_tObject menu, int *t_size, glow_eDrawType *fill_color, glow_eDrawType *t_drawtype,
		       glow_eDrawType *t_color, glow_eDrawType *t_color_disabled)
{
  ((GrowMenu *)menu)->get_menu_char( t_size, fill_color, t_drawtype, t_color, t_color_disabled);
}

void grow_MenuShiftCurrentItem( grow_tObject menu, int shift)
{
  ((GrowMenu *)menu)->shift_current_item( shift);
}

int grow_MenuGetCurrentItem( grow_tObject menu, int *item)
{
  return ((GrowMenu *)menu)->get_current_item( item);
}

void grow_SetMenuInputFocus( grow_tObject menu, int focus)
{
  ((GrowMenu *)menu)->set_input_focus( focus);
}

int grow_GetMenuInputFocus( grow_tObject menu)
{
  return ((GrowMenu *)menu)->get_input_focus();
}

void grow_GetSubmenuPosition( grow_tObject menu, int item, double *x, double *y)
{
  ((GrowMenu *)menu)->get_submenu_position( item, x, y);
}

int grow_GetMenuParent( grow_tObject menu, grow_tObject *parent)
{
  return ((GrowMenu *)menu)->get_parent( (GlowArrayElem **)parent);
}

int grow_SetFolderIndex( grow_tObject folder, int idx)
{
  return ((GrowFolder *)folder)->set_folder( idx);
}

int grow_SetWindowSource( grow_tObject window, char *source)
{
  return ((GrowWindow *)window)->set_source( source);
}

void grow_GetWindowSize( grow_tCtx ctx, int *width, int *height)
{
  *width = ctx->mw.window_width;
  *height = ctx->mw.window_height;
}

int grow_IsJava( char *name, int *is_frame, int *is_applet, char *java_name)
{
  int sts;

  GrowCtx *ctx = new GrowCtx( "tmp");
  ctx->set_nodraw();
  sts = ctx->open( name, glow_eSaveMode_ReadConfigOnly);
  if ( ODD(sts)) {
    *is_frame = ctx->is_javaapplication;
    *is_applet = ctx->is_javaapplet;
    strcpy( java_name, ctx->java_name);
  }
  delete ctx;
  return sts;
}

void grow_GetOwner( grow_tCtx ctx, char *owner)
{
  
  ((GrowCtx *)ctx)->get_owner( owner);
}

void grow_SetOwner( grow_tCtx ctx, char *owner)
{
  
  ((GrowCtx *)ctx)->set_owner( owner);
}

int grow_GetNodeClassAnnotBackground( grow_tNodeClass nodeclass, glow_eDrawType *background)
{
  return ((GlowNodeClass *)nodeclass)->get_annot_background( 0, 0, background);
}

void grow_SetSelectScale( grow_tCtx ctx, double scale_x, double scale_y,
			  glow_eScaleType type)
{
  ctx->set_select_scale( scale_x, scale_y, type);
}

int grow_GetNextObject( grow_tCtx ctx, grow_tObject object, glow_eDirection dir,
			 grow_tObject *next)
{
  return ctx->get_next_object( (GlowArrayElem *)object, dir, (GlowArrayElem **)next);
}

int grow_IsVisible( grow_tCtx ctx, grow_tObject object, glow_eVisible type)
{
  return ctx->is_visible( (GlowArrayElem *)object, type);
}


/*@}*/



