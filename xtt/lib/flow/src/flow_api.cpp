/* 
 * Proview   $Id: flow_api.cpp,v 1.16 2008-10-03 14:19:19 claes Exp $
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

#include "flow_std.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_rect.h"
#include "flow_line.h"
#include "flow_arc.h"
#include "flow_text.h"
#include "flow_pixmap.h"
#include "flow_pushbutton.h"
#include "flow_array_elem.h"
#include "flow_array.h"
#include "flow_nodeclass.h"
#include "flow_node.h"
#include "flow_conclass.h"
#include "flow_con.h"
#include "flow_conpoint.h"
#include "flow_annot.h"
#include "flow_draw.h"
#include "flow_tiptext.h"
#include "flow_api.h"

int flow_Save( flow_tCtx ctx, char *filename)
{
  return ctx->save( filename, flow_eSaveMode_Edit);
}

int flow_Open( flow_tCtx ctx, char *filename)
{
  return ctx->open( filename, flow_eSaveMode_Edit);
}

int flow_SaveTrace( flow_tCtx ctx, char *filename)
{
  return ctx->save( filename, flow_eSaveMode_Trace);
}

int flow_OpenTrace( flow_tCtx ctx, char *filename)
{
  return ctx->open( filename, flow_eSaveMode_Trace);
}

void flow_DeleteAll( flow_tCtx ctx)
{
  ctx->delete_all();
}

void flow_DeleteNode( flow_tNode node)
{
  ((FlowNode *)node)->ctx->delete_object((FlowArrayElem *)node);
}

void flow_DeleteConnection( flow_tCon con)
{
  ((FlowCon *)con)->ctx->delete_object((FlowArrayElem *)con);
}

int flow_FindSelectedObject( flow_tCtx ctx, flow_tObject object)
{
  return ctx->select_find( (FlowArrayElem *)object);
}

void flow_ResetHighlightAll( flow_tCtx ctx)
{
  ctx->set_highlight( 0);
}

void flow_ResetInverseAll( flow_tCtx ctx)
{
  ctx->set_inverse( 0);
}

void flow_ResetSelectHighlight( flow_tCtx ctx)
{
  ctx->set_select_highlight( 0);
}

void flow_SetSelectHighlight( flow_tCtx ctx)
{
  ctx->set_select_highlight( 1);
}

void flow_ResetSelectInverse( flow_tCtx ctx)
{
  ctx->set_select_inverse( 0);
}

void flow_SetSelectInverse( flow_tCtx ctx)
{
  ctx->set_select_inverse( 1);
}

void flow_SelectInsert( flow_tCtx ctx, flow_tObject object)
{
  ctx->select_insert( (FlowArrayElem *)object);
}

void flow_SelectRemove( flow_tCtx ctx, flow_tObject object)
{
  ctx->select_remove( (FlowArrayElem *)object);
}

void flow_SelectClear( flow_tCtx ctx)
{
  ctx->select_clear();
}

void flow_ConPointSelectInsert( flow_tCtx ctx, flow_tObject object, int num)
{
  ctx->conpoint_select( (FlowArrayElem *)object, num);
}

void flow_ConPointSelectClear( flow_tCtx ctx)
{
  ctx->conpoint_select_clear();
}

void flow_GetSelectedNodes( flow_tCtx ctx, flow_tNode **nodes, int *num)
{
  ctx->get_selected_nodes( (FlowArrayElem ***) nodes, num);
}

void flow_GetSelectedCons( flow_tCtx ctx, flow_tCon **cons, int *num)
{
  ctx->get_selected_cons( (FlowArrayElem ***) cons, num);
}

void flow_SetHighlight( flow_tObject object, int value)
{
  ((FlowArrayElem *) object)->set_highlight( value);
}

void flow_GetHighlight( flow_tObject object, int *value)
{
  *value = ((FlowArrayElem *) object)->get_highlight();
}

void flow_SetInverse( flow_tObject object, int value)
{
  ((FlowArrayElem *) object)->set_inverse( value);
}

void flow_CreateNode( flow_tCtx ctx, char *name, flow_tNodeClass nc,
	double x, double y, void *user_data, flow_tNode *node)
{
  FlowNode *n1;
  n1 = new FlowNode( ctx, name, (FlowNodeClass *)nc, x, y);
  n1->set_user_data( user_data);
  ctx->insert( n1);
  ctx->nav_zoom();
  *node = (flow_tNode) n1;
}

void flow_CreateCon( flow_tCtx ctx, char *name, flow_tConClass cc,
	flow_tNode source, flow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, flow_tCon *con, 
	int point_num, double *x_vect, double *y_vect, int *rsts)
{
  FlowCon *c1;
  c1 = new FlowCon( ctx, name, (FlowConClass *)cc, (FlowNode *)source,
	(FlowNode *)dest, source_conpoint, dest_conpoint, rsts, 0, point_num,
	x_vect, y_vect);
  if ( EVEN(*rsts)) return;

  c1->set_user_data( user_data);
  ctx->insert( c1);
  ctx->nav_zoom();
  *con = (flow_tCon) c1;
}

void flow_CreatePasteNode( flow_tCtx ctx, char *name, flow_tNodeClass nc,
	double x, double y, void *user_data, flow_tNode *node)
{
  FlowNode *n1;
  n1 = new FlowNode( ctx, name, (FlowNodeClass *)nc, x, y, 1);
  n1->set_user_data( user_data);
  ctx->paste_insert( n1);
  *node = (flow_tNode) n1;
}

void flow_CreatePasteCon( flow_tCtx ctx, char *name, flow_tConClass cc,
	flow_tNode source, flow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, flow_tCon *con, 
	int point_num, double *x_vect, double *y_vect, int *rsts)
{
  FlowCon *c1;
  c1 = new FlowCon( ctx, name, (FlowConClass *)cc, (FlowNode *)source,
	(FlowNode *)dest, source_conpoint, dest_conpoint, rsts, 1, point_num,
	x_vect, y_vect);
  if ( EVEN(*rsts)) return;

  c1->set_user_data( user_data);
  ctx->paste_insert( c1);
  *con = (flow_tCon) c1;
}

void flow_SetAnnotation( flow_tNode node, int number, char *text, int size)
{
  ((FlowNode *)node)->set_annotation( number, text, size, 0);
}

void flow_SetPasteNodeAnnotation( flow_tNode node, int number, char *text, int size)
{
  ((FlowNode *)node)->set_annotation( number, text, size, 1);
}

extern "C" void	flow_EnableEvent( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

void	flow_DisableEvent( FlowCtx *ctx, flow_eEvent event)
{
  ctx->disable_event( event);
}

void	flow_DisableEventAll( FlowCtx *ctx)
{
  ctx->disable_event_all();
}

void flow_Cut( flow_tCtx ctx)
{
  ctx->cut();
}

void flow_Copy( flow_tCtx ctx)
{
  ctx->copy();
}

void flow_Paste( flow_tCtx ctx)
{
  ctx->paste();
}

void flow_PasteClear( flow_tCtx ctx)
{
  ctx->paste_clear();
}

void flow_CreateRect( flow_tCtx ctx, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, flow_tObject *rect)
{
  *rect = (flow_tObject) new FlowRect( ctx, x, y, width, height, draw_type,
	line_width);
}

void flow_CreateLine( flow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width, flow_tObject *line)
{
  *line = (flow_tObject) new FlowLine( ctx, x1, y1, x2, y2, draw_type,
	line_width);
}

void flow_CreateArc( flow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	flow_eDrawType draw_type, int line_width, flow_tObject *arc)
{
  *arc = (flow_tObject) new FlowArc( ctx, x1, y1, x2, y2, 
	angel1, angel2, draw_type, line_width);
}

void flow_CreateText( flow_tCtx ctx, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size, flow_tObject *text)
{
  *text = (flow_tObject) new FlowText( ctx, text_str, x, y, draw_type,
	text_size);
}

void flow_CreatePixmap( flow_tCtx ctx, flow_sPixmapData *pixmap_data, 
	double x, double y, flow_eDrawType draw_type, int size, 
	flow_tObject *pixmap)
{
  *pixmap = (flow_tObject) new FlowPixmap( ctx, pixmap_data, x, y,
	draw_type, size);
}

void flow_CreateAnnot( flow_tCtx ctx, double x, double y, int number,
	flow_eDrawType draw_type, int text_size, flow_tObject *annot)
{
  *annot = (flow_tObject) new FlowAnnot( ctx, x, y, number, draw_type,
	text_size);
}

void flow_CreateConPoint( flow_tCtx ctx, double x, double y, int number,
	flow_eDirection direction, flow_tObject *conpoint)
{
  *conpoint = (flow_tObject) new FlowConPoint( ctx, x, y, number, direction);
}

void flow_AddRect( flow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, flow_mDisplayLevel display_level)
{
  FlowRect *rect = new FlowRect( ((FlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, line_width, 0, display_level);
  ((FlowNodeClass *)nc)->insert( rect);
  
}

void flow_AddFilledRect( flow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, flow_mDisplayLevel display_level)
{
  FlowRect *rect = new FlowRect( ((FlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, 0, 0, display_level, 1);
  ((FlowNodeClass *)nc)->insert( rect);
}

void flow_AddLine( flow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width)
{
  FlowLine *line = new FlowLine( ((FlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, draw_type, line_width);
  ((FlowNodeClass *)nc)->insert( line);
}

void flow_AddArc( flow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	flow_eDrawType draw_type, int line_width)
{
  FlowArc *arc = new FlowArc( ((FlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, angel1, angel2, draw_type, line_width);
  ((FlowNodeClass *)nc)->insert( arc);
}

void flow_AddText( flow_tNodeClass nc, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size)
{
  FlowText *text = new FlowText( ((FlowNodeClass *)nc)->ctx, 
	text_str, x, y, draw_type, text_size);
  ((FlowNodeClass *)nc)->insert( text);
}

void flow_AddAnnot( flow_tNodeClass nc, double x, double y, int number,
	flow_eDrawType draw_type, int text_size, flow_eAnnotType annot_type,
	flow_mDisplayLevel display_level)
{
  FlowAnnot *annot = new FlowAnnot( ((FlowNodeClass *)nc)->ctx, 
	x, y, number, draw_type, text_size, annot_type, 0, display_level);
  ((FlowNodeClass *)nc)->insert( annot);
}

void flow_AddConPoint( flow_tNodeClass nc, double x, double y, int number,
	flow_eDirection direction)
{
  FlowConPoint *conpoint = new FlowConPoint( ((FlowNodeClass *)nc)->ctx, 
	x, y, number, direction);
  ((FlowNodeClass *)nc)->insert( conpoint);
}

void flow_CreatePushButton( flow_tCtx ctx, char *text, double x, double y, 
	double width, double height, flow_tObject *pushbutton)
{
  *pushbutton = (flow_tObject) new FlowPushButton( ctx, text, x, y, 
	width, height);
  ctx->insert( (FlowArrayElem *) *pushbutton);
}

void flow_CreateNodeClass( flow_tCtx ctx, char *name, flow_eNodeGroup group,
	flow_tNodeClass *nodeclass)
{
  *nodeclass = (flow_tNodeClass) new FlowNodeClass( ctx, name, group);
  ctx->nodeclass_insert( (FlowArrayElem *) *nodeclass);
}

void flow_NodeClassAdd( flow_tNodeClass nc, flow_tObject object)
{
  ((FlowNodeClass *)nc)->insert( (FlowArrayElem *)object);
}

void flow_CreateConClass( flow_tCtx ctx, char *name,
	flow_eConType con_type, flow_eCorner corner, flow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, flow_eConGroup group, 
	flow_tConClass *conclass)
{
  *conclass = (flow_tConClass) new FlowConClass( ctx, name, con_type,
	corner, line_type, line_width, arrow_width, arrow_length,
	round_corner_amount, group);
  ctx->conclass_insert( (FlowArrayElem *) *conclass);
}

void flow_SetDefaultConClass( flow_tCtx ctx, flow_tConClass conclass)
{
  ctx->set_default_conclass( conclass);
}

flow_tConClass flow_GetDefaultConClass( flow_tCtx ctx)
{
  return ctx->get_default_conclass();
}

void flow_GetSelectList( flow_tCtx ctx, flow_tObject **list, int *cnt)
{
  ctx->get_selectlist( (FlowArrayElem ***)list, cnt);
}

void flow_GetPasteList( flow_tCtx ctx, flow_tObject **list, int *cnt)
{
  ctx->get_pastelist( (FlowArrayElem ***)list, cnt);
}

void flow_GetObjectList( flow_tCtx ctx, flow_tObject **list, int *cnt)
{
  ctx->get_objectlist( (FlowArrayElem ***)list, cnt);
}

void flow_GetConPointSelectList( flow_tCtx ctx, flow_tObject **list, int **num_list, int *cnt)
{
  ctx->get_conpoint_selectlist( (FlowArrayElem ***)list, num_list, cnt);
}

flow_eObjectType flow_GetObjectType( flow_tObject object)
{
  return ((FlowArrayElem *)object)->type();
}

void flow_MeasureNode( flow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y)
{
  ((FlowNode *)node)->measure( ll_x, ll_y, ur_x, ur_y);
}

int flow_PrintRegion( flow_tCtx ctx, double ll_x, double ll_y,
	double ur_x, double ur_y, char *filename)
{
  return ctx->print_region( ll_x, ll_y, ur_x, ur_y, filename);
}

int flow_PrintPdfRegion( flow_tCtx ctx, double ll_x, double ll_y,
			 double ur_x, double ur_y, char *filename)
{
  return ctx->print_pdf_region( ll_x, ll_y, ur_x, ur_y, filename);
}

void flow_GetUserData( flow_tObject object, void **user_data)
{
  ((FlowArrayElem *)object)->get_user_data( user_data);
}

void flow_SetUserData( flow_tObject object, void *user_data)
{
  ((FlowArrayElem *)object)->set_user_data( user_data);
}

void flow_GetCtxUserData( flow_tCtx ctx, void **user_data)
{
  ((FlowCtx *)ctx)->get_user_data( user_data);
}

void flow_SetCtxUserData( flow_tCtx ctx, void *user_data)
{
  ((FlowCtx *)ctx)->set_user_data( user_data);
}

flow_tCtx flow_GetCtx( flow_tObject object)
{
  return (flow_tCtx)((FlowArrayElem *)object)->get_ctx();
}

void flow_SetTraceAttr( flow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType trace_attr_type, int inverted)
{
  ((FlowArrayElem *)object)->set_trace_attr( trace_object, trace_attribute,
	trace_attr_type, inverted);
}

void flow_GetTraceAttr( flow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType *trace_attr_type, int *inverted)
{
  ((FlowArrayElem *)object)->get_trace_attr( trace_object, trace_attribute,
	trace_attr_type, inverted);
}

extern "C" int flow_TraceInit( flow_tCtx ctx, int (*trace_connect_func)( flow_tObject, 
	char *, char *, flow_eTraceType, void **), 
	int (*trace_disconnect_func)( flow_tObject),
	int (*trace_scan_func)( flow_tObject, void *))
{
  return ctx->trace_init( trace_connect_func, trace_disconnect_func,
		trace_scan_func);
}

void flow_TraceClose( flow_tCtx ctx)
{
  ctx->trace_close();
}

void flow_TraceScan( flow_tCtx ctx)
{
  ctx->trace_scan();
}

void flow_RemoveTraceObjects( flow_tCtx ctx)
{
  ctx->remove_trace_objects();
}

void flow_Zoom( flow_tCtx ctx, double zoom_factor)
{
  ctx->zoom( zoom_factor);
}

void flow_ZoomAbsolute( flow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_absolute( zoom_factor);
}

void flow_Scroll( flow_tCtx ctx, double x, double y)
{
  ctx->scroll( x, y);
}

void flow_SetAttributes( flow_tCtx ctx, flow_sAttributes *attr, 
	unsigned long mask)
{
  if ( mask & flow_eAttr_base_zoom_factor)
  {
    ctx->zoom_factor *= attr->base_zoom_factor / ctx->base_zoom_factor;
    ctx->base_zoom_factor = attr->base_zoom_factor;
  }
  if ( mask & flow_eAttr_offset_x)
    ctx->offset_x = attr->offset_x;
  if ( mask & flow_eAttr_offset_y)
    ctx->offset_y = attr->offset_y;
  if ( mask & flow_eAttr_grid_size_x)
    ctx->grid_size_x = attr->grid_size_x;
  if ( mask & flow_eAttr_grid_size_y)
    ctx->grid_size_y = attr->grid_size_y;
  if ( mask & flow_eAttr_grid_on)
    ctx->grid_on = attr->grid_on;
  if ( mask & flow_eAttr_user_highlight)
    ctx->user_highlight = attr->user_highlight;
  if ( mask & flow_eAttr_draw_delta)
    ctx->draw_delta = attr->draw_delta;
  if ( mask & flow_eAttr_grafcet_con_delta)
    ctx->grafcet_con_delta = attr->grafcet_con_delta;
  if ( mask & flow_eAttr_refcon_width)
    ctx->refcon_width = attr->refcon_width;
  if ( mask & flow_eAttr_refcon_height)
    ctx->refcon_height = attr->refcon_height;
  if ( mask & flow_eAttr_refcon_linewidth)
    ctx->refcon_linewidth = attr->refcon_linewidth;
  if ( mask & flow_eAttr_refcon_textsize)
    ctx->refcon_textsize = attr->refcon_textsize;
  if ( mask & flow_eAttr_application_paste)
    ctx->application_paste = attr->application_paste;
  if ( mask & flow_eAttr_select_policy)
    ctx->select_policy = attr->select_policy;
  if ( mask & flow_eAttr_display_level)
    ctx->display_level = attr->display_level;
  if ( mask & flow_eAttr_zoom_factor)
    ctx->zoom_factor = attr->zoom_factor;
}

void flow_GetAttributes( flow_tCtx ctx, flow_sAttributes *attr)
{
  attr->base_zoom_factor = ctx->base_zoom_factor;
  attr->offset_x = ctx->offset_x;
  attr->offset_y = ctx->offset_y;
  attr->grid_size_x = ctx->grid_size_x;
  attr->grid_size_y = ctx->grid_size_y;
  attr->grid_on = ctx->grid_on;
  attr->user_highlight = ctx->user_highlight;
  attr->draw_delta = ctx->draw_delta;
  attr->grafcet_con_delta = ctx->grafcet_con_delta;
  attr->refcon_width = ctx->refcon_width;
  attr->refcon_height = ctx->refcon_height;
  attr->refcon_linewidth = ctx->refcon_linewidth;
  attr->refcon_textsize = ctx->refcon_textsize;
  attr->application_paste = ctx->application_paste;
  attr->select_policy = ctx->select_policy;
  attr->display_level = ctx->display_level;
  attr->zoom_factor = ctx->zoom_factor;
}

void flow_PositionToPixel( flow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y)
{
  ctx->position_to_pixel( x, y, pix_x, pix_y);
}

void flow_PixelToPosition( flow_tCtx ctx, int pix_x, int pix_y, 
			   double *x, double *y)
{
  ctx->pixel_to_position( pix_x, pix_y, x, y);
}

void flow_UnZoom( flow_tCtx ctx)
{
  ctx->unzoom();
}

void flow_CenterObject( flow_tCtx ctx, flow_tObject object)
{
  ctx->center_object( (FlowArrayElem *)object);
}

void flow_MoveSelectedNodes( flow_tCtx ctx, double delta_x, double delta_y, int grid)
{
  ctx->move_selected_nodes( delta_x, delta_y, grid);
}

void flow_GetNodePosition( flow_tNode node, double *x, double *y)
{
  ((FlowNode *)node)->get_node_position( x, y);
}

void flow_GetConPosition( flow_tCon con, double *x_arr[], double *y_arr[],
	int *num)
{
  ((FlowCon *)con)->get_con_position( x_arr, y_arr, num);
}

void flow_MeasureAnnotation( flow_tNodeClass node_class, int number, char *text, 
	double *width, double *height)
{
  ((FlowNodeClass *)node_class)->measure_annotation( number, text, width, height);
}

void flow_MeasureAnnotText( flow_tCtx ctx, char *text, flow_eDrawType draw_type,
		int text_size, flow_eAnnotType annot_type,
		double *width, double *height, int *rows)
{
  flow_measure_annot_text( ctx, text, draw_type, text_size, annot_type, 
		width, height, rows);
}

flow_eNodeGroup flow_GetNodeGroup( flow_tNode node)
{
  return ((FlowNode *)node)->get_group();
}

flow_eConGroup flow_GetConGroup( flow_tCon con)
{
  return ((FlowCon *)con)->get_group();
}

void flow_DeleteNodeCons( flow_tNode node)
{
  ((FlowNode *)node)->ctx->delete_node_cons( node);
}

void flow_GetObjectName( flow_tObject object, char *name)
{
  ((FlowNode *)object)->get_object_name( name);
}

void flow_Reconfigure( flow_tCtx ctx)
{
  ctx->reconfigure();
}

void flow_SetNodraw( flow_tCtx ctx)
{
  ctx->set_nodraw();
}

void flow_ResetNodraw( flow_tCtx ctx)
{
  ctx->reset_nodraw();
}

void flow_Redraw( flow_tCtx ctx)
{
  ctx->redraw();
}

int flow_FindByName( flow_tCtx ctx, char *name, flow_tObject *object)
{
  return ctx->find_by_name( name, (FlowArrayElem **)object);
}

int flow_FindByNameNoCase( flow_tCtx ctx, char *name, flow_tObject *object)
{
  return ctx->find_by_name_no_case( name, (FlowArrayElem **)object);
}

int flow_GetConPointTraceAttr( flow_tObject object, int num, char *trace_attr, 
	flow_eTraceType *type)
{
  return ((FlowNode *)object)->get_conpoint_trace_attr( num, trace_attr, type);
}

int flow_GetConPoint( flow_tObject object,  int num, double *x, double *y, 
	flow_eDirection *dir)
{
  return ((FlowNode *)object)->get_conpoint( num, x, y, dir);
}

void flow_SetClickSensitivity( flow_tCtx ctx, int value)
{
  ctx->fdraw->set_click_sensitivity( ctx, value);
}

void flow_SetNoConObstacle( flow_tNodeClass nc, int no_obstacle)
{
  ((FlowNodeClass *)nc)->set_no_con_obstacle( no_obstacle);
}

int flow_GetNextObject( flow_tCtx ctx, flow_tNode object, flow_eDirection dir,
			flow_tNode *next)
{
  return ctx->get_next_object( (FlowArrayElem *)object, dir,
			       (FlowArrayElem **)next);
}

int flow_GetNextConPoint( flow_tCtx ctx, flow_tNode object, int cp_num, flow_eDirection dir,
			  flow_tNode *next, int *next_cp_num)
{
  return ctx->get_next_conpoint( (FlowArrayElem *)object, cp_num, dir,
				 (FlowArrayElem **)next, next_cp_num);
}

int flow_IsVisible( flow_tCtx ctx, flow_tObject object, flow_eVisible type)
{
  return ctx->is_visible( (FlowArrayElem *)object, type);
}

int flow_LoadNodeClass( flow_tCtx ctx, char *fname, flow_tNodeClass *nodeclass)
{
  *nodeclass = (flow_tNodeClass) new FlowNodeClass( ctx, "tmp", flow_eNodeGroup_Common);
  int sts = (*(FlowNodeClass **)nodeclass)->load( fname);
  if ( ODD(sts))
    ctx->nodeclass_insert( (FlowArrayElem *) *nodeclass);
  return sts;
}

void flow_SetTipText( flow_tCtx ctx, flow_tObject object, char *text, int x, int y)
{
  ctx->tiptext->draw_text( (FlowArrayElem *)object, text, x, y);
}

void flow_RemoveTipText( flow_tCtx ctx)
{
  ctx->tiptext->remove();
}

int flow_PasteStop( flow_tCtx ctx)
{
  return ctx->paste_stop();
}

int flow_PendingPaste( flow_tCtx ctx)
{
  return ctx->pending_paste();
}

int flow_PendingPasteStop( flow_tCtx ctx)
{
  return ctx->pending_paste_stop();
}


