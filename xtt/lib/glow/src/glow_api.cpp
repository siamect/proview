#include "glow_std.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_line.h"
#include "glow_arc.h"
#include "glow_text.h"
#include "glow_pixmap.h"
#include "glow_pushbutton.h"
#include "glow_array_elem.h"
#include "glow_array.h"
#include "glow_nodeclass.h"
#include "glow_node.h"
#include "glow_conclass.h"
#include "glow_con.h"
#include "glow_conpoint.h"
#include "glow_annot.h"
#include "glow_draw.h"
#include "glow_api.h"

int glow_Save( glow_tCtx ctx, char *filename)
{
  return ctx->save( filename, glow_eSaveMode_Edit);
}

int glow_Open( glow_tCtx ctx, char *filename)
{
  return ctx->open( filename, glow_eSaveMode_Edit);
}

int glow_SaveTrace( glow_tCtx ctx, char *filename)
{
  return ctx->save( filename, glow_eSaveMode_Trace);
}

int glow_OpenTrace( glow_tCtx ctx, char *filename)
{
  return ctx->open( filename, glow_eSaveMode_Trace);
}

void glow_DeleteNode( glow_tNode node)
{
  ((GlowNode *)node)->ctx->delete_object((GlowArrayElem *)node);
}

void glow_DeleteConnection( glow_tCon con)
{
  ((GlowCon *)con)->ctx->delete_object((GlowArrayElem *)con);
}

int glow_FindSelectedObject( glow_tCtx ctx, glow_tObject object)
{
  return ctx->select_find( (GlowArrayElem *)object);
}

void glow_ResetHighlightAll( glow_tCtx ctx)
{
  ctx->set_highlight( 0);
}

void glow_ResetInverseAll( glow_tCtx ctx)
{
  ctx->set_inverse( 0);
}

void glow_ResetSelectHighlight( glow_tCtx ctx)
{
  ctx->set_select_highlight( 0);
}

void glow_SetSelectHighlight( glow_tCtx ctx)
{
  ctx->set_select_highlight( 1);
}

void glow_ResetSelectInverse( glow_tCtx ctx)
{
  ctx->set_select_inverse( 0);
}

void glow_SetSelectInverse( glow_tCtx ctx)
{
  ctx->set_select_inverse( 1);
}

void glow_SelectInsert( glow_tCtx ctx, glow_tObject object)
{
  ctx->select_insert( (GlowArrayElem *)object);
}

void glow_SelectRemove( glow_tCtx ctx, glow_tObject object)
{
  ctx->select_remove( (GlowArrayElem *)object);
}

void glow_SelectClear( glow_tCtx ctx)
{
  ctx->select_clear();
}

void glow_GetSelectedNodes( glow_tCtx ctx, glow_tNode **nodes, int *num)
{
  ctx->get_selected_nodes( (GlowArrayElem ***) nodes, num);
}

void glow_GetSelectedCons( glow_tCtx ctx, glow_tCon **cons, int *num)
{
  ctx->get_selected_cons( (GlowArrayElem ***) cons, num);
}

void glow_SetHighlight( glow_tObject object, int value)
{
  ((GlowArrayElem *) object)->set_highlight( value);
}

void glow_GetHighlight( glow_tObject object, int *value)
{
  *value = ((GlowArrayElem *) object)->get_highlight();
}

void glow_SetInverse( glow_tObject object, int value)
{
  ((GlowArrayElem *) object)->set_inverse( value);
}

void glow_CreateNode( glow_tCtx ctx, char *name, glow_tNodeClass nc,
	double x, double y, void *user_data, glow_tNode *node)
{
  GlowNode *n1;
  n1 = new GlowNode( ctx, name, (GlowNodeClass *)nc, x, y);
  n1->set_user_data( user_data);
  ctx->insert( n1);
  ctx->nav_zoom();
  *node = (glow_tNode) n1;
}

void glow_CreateCon( glow_tCtx ctx, char *name, glow_tConClass cc,
	glow_tNode source, glow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, glow_tCon *con, 
	int point_num, double *x_vect, double *y_vect)
{
  GlowCon *c1;
  c1 = new GlowCon( ctx, name, (GlowConClass *)cc, (GlowNode *)source,
	(GlowNode *)dest, source_conpoint, dest_conpoint, 0, point_num,
	x_vect, y_vect);
  c1->set_user_data( user_data);
  ctx->insert( c1);
  ctx->nav_zoom();
  *con = (glow_tCon) c1;
}

void glow_CreatePasteNode( glow_tCtx ctx, char *name, glow_tNodeClass nc,
	double x, double y, void *user_data, glow_tNode *node)
{
  GlowNode *n1;
  n1 = new GlowNode( ctx, name, (GlowNodeClass *)nc, x, y, 1);
  n1->set_user_data( user_data);
  ctx->paste_insert( n1);
  *node = (glow_tNode) n1;
}

void glow_CreatePasteCon( glow_tCtx ctx, char *name, glow_tConClass cc,
	glow_tNode source, glow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, glow_tCon *con, 
	int point_num, double *x_vect, double *y_vect)
{
  GlowCon *c1;
  c1 = new GlowCon( ctx, name, (GlowConClass *)cc, (GlowNode *)source,
	(GlowNode *)dest, source_conpoint, dest_conpoint, 1, point_num,
	x_vect, y_vect);
  c1->set_user_data( user_data);
  ctx->paste_insert( c1);
  *con = (glow_tCon) c1;
}

void glow_SetAnnotation( glow_tNode node, int number, char *text, int size)
{
  ((GlowNode *)node)->set_annotation( number, text, size, 0);
}

void glow_SetPasteNodeAnnotation( glow_tNode node, int number, char *text, int size)
{
  ((GlowNode *)node)->set_annotation( number, text, size, 1);
}

extern "C" void	glow_EnableEvent( GlowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

void	glow_DisableEvent( GlowCtx *ctx, glow_eEvent event)
{
  ctx->disable_event( event);
}

void	glow_DisableEventAll( GlowCtx *ctx)
{
  ctx->disable_event_all();
}

void glow_Cut( glow_tCtx ctx)
{
  ctx->cut();
}

void glow_Copy( glow_tCtx ctx)
{
  ctx->copy();
}

void glow_Paste( glow_tCtx ctx)
{
  ctx->paste();
}

void glow_PasteClear( glow_tCtx ctx)
{
  ctx->paste_clear();
}

void glow_CreateRect( glow_tCtx ctx, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, glow_tObject *rect)
{
  *rect = (glow_tObject) new GlowRect( ctx, x, y, width, height, draw_type,
	line_width);
}

void glow_CreateLine( glow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width, glow_tObject *line)
{
  *line = (glow_tObject) new GlowLine( ctx, x1, y1, x2, y2, draw_type,
	line_width);
}

void glow_CreateArc( glow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width, glow_tObject *arc)
{
  *arc = (glow_tObject) new GlowArc( ctx, x1, y1, x2, y2, 
	angel1, angel2, draw_type, line_width);
}

void glow_CreateText( glow_tCtx ctx, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size, glow_tObject *text)
{
  *text = (glow_tObject) new GlowText( ctx, text_str, x, y, draw_type,
	text_size);
}

void glow_CreatePixmap( glow_tCtx ctx, glow_sPixmapData *pixmap_data, 
	double x, double y, glow_eDrawType draw_type, int size, 
	glow_tObject *pixmap)
{
  *pixmap = (glow_tObject) new GlowPixmap( ctx, pixmap_data, x, y,
	draw_type, size);
}

void glow_CreateAnnot( glow_tCtx ctx, double x, double y, int number,
	glow_eDrawType draw_type, int text_size, glow_tObject *annot)
{
  *annot = (glow_tObject) new GlowAnnot( ctx, x, y, number, draw_type,
	text_size);
}

void glow_CreateConPoint( glow_tCtx ctx, double x, double y, int number,
	glow_eDirection direction, glow_tObject *conpoint)
{
  *conpoint = (glow_tObject) new GlowConPoint( ctx, x, y, number, direction);
}

void glow_AddRect( glow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, glow_mDisplayLevel display_level)
{
  GlowRect *rect = new GlowRect( ((GlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, line_width, 0, display_level);
  ((GlowNodeClass *)nc)->insert( rect);
  
}

void glow_AddFilledRect( glow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, glow_mDisplayLevel display_level)
{
  GlowRect *rect = new GlowRect( ((GlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, 0, 0, display_level, 1);
  ((GlowNodeClass *)nc)->insert( rect);
}

void glow_AddLine( glow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width)
{
  GlowLine *line = new GlowLine( ((GlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, draw_type, line_width);
  ((GlowNodeClass *)nc)->insert( line);
}

void glow_AddArc( glow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width)
{
  GlowArc *arc = new GlowArc( ((GlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, angel1, angel2, draw_type, line_width);
  ((GlowNodeClass *)nc)->insert( arc);
}

void glow_AddText( glow_tNodeClass nc, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size)
{
  GlowText *text = new GlowText( ((GlowNodeClass *)nc)->ctx, 
	text_str, x, y, draw_type, text_size);
  ((GlowNodeClass *)nc)->insert( text);
}

void glow_AddAnnot( glow_tNodeClass nc, double x, double y, int number,
	glow_eDrawType draw_type, int text_size, glow_eAnnotType annot_type,
	glow_mDisplayLevel display_level)
{
  GlowAnnot *annot = new GlowAnnot( ((GlowNodeClass *)nc)->ctx, 
	x, y, number, draw_type, text_size, annot_type, 0, display_level);
  ((GlowNodeClass *)nc)->insert( annot);
}

void glow_AddConPoint( glow_tNodeClass nc, double x, double y, int number,
	glow_eDirection direction)
{
  GlowConPoint *conpoint = new GlowConPoint( ((GlowNodeClass *)nc)->ctx, 
	x, y, number, direction);
  ((GlowNodeClass *)nc)->insert( conpoint);
}

void glow_CreatePushButton( glow_tCtx ctx, char *text, double x, double y, 
	double width, double height, glow_tObject *pushbutton)
{
  *pushbutton = (glow_tObject) new GlowPushButton( ctx, text, x, y, 
	width, height);
  ctx->insert( (GlowArrayElem *) *pushbutton);
}

void glow_CreateNodeClass( glow_tCtx ctx, char *name, glow_eNodeGroup group,
	glow_tNodeClass *nodeclass)
{
  *nodeclass = (glow_tNodeClass) new GlowNodeClass( ctx, name, group);
  ctx->nodeclass_insert( (GlowArrayElem *) *nodeclass);
}

void glow_NodeClassAdd( glow_tNodeClass nc, glow_tObject object)
{
  ((GlowNodeClass *)nc)->insert( (GlowArrayElem *)object);
}

void glow_CreateConClass( glow_tCtx ctx, char *name,
	glow_eConType con_type, glow_eCorner corner, glow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, glow_eConGroup group, 
	glow_tConClass *conclass)
{
  *conclass = (glow_tConClass) new GlowConClass( ctx, name, con_type,
	corner, line_type, line_width, arrow_width, arrow_length,
	round_corner_amount, group);
  ctx->conclass_insert( (GlowArrayElem *) *conclass);
}

void glow_SetDefaultConClass( glow_tCtx ctx, glow_tConClass conclass)
{
  ctx->set_default_conclass( conclass);
}

glow_tConClass glow_GetDefaultConClass( glow_tCtx ctx)
{
  return ctx->get_default_conclass();
}

void glow_GetSelectList( glow_tCtx ctx, glow_tObject **list, int *cnt)
{
  ctx->get_selectlist( (GlowArrayElem ***)list, cnt);
}

void glow_GetObjectList( glow_tCtx ctx, glow_tObject **list, int *cnt)
{
  ctx->get_objectlist( (GlowArrayElem ***)list, cnt);
}

glow_eObjectType glow_GetObjectType( glow_tObject object)
{
  return ((GlowArrayElem *)object)->type();
}

void glow_MeasureNode( glow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y)
{
  ((GlowNode *)node)->measure( ll_x, ll_y, ur_x, ur_y);
}

void glow_PrintRegion( glow_tCtx ctx, double ll_x, double ll_y,
	double ur_x, double ur_y, char *filename)
{
  ctx->print_region( ll_x, ll_y, ur_x, ur_y, filename);
}

void glow_GetUserData( glow_tObject object, void **user_data)
{
  ((GlowArrayElem *)object)->get_user_data( user_data);
}

void glow_SetUserData( glow_tObject object, void *user_data)
{
  ((GlowArrayElem *)object)->set_user_data( user_data);
}

void glow_GetCtxUserData( glow_tCtx ctx, void **user_data)
{
  ((GlowCtx *)ctx)->get_user_data( user_data);
}

void glow_SetCtxUserData( glow_tCtx ctx, void *user_data)
{
  ((GlowCtx *)ctx)->set_user_data( user_data);
}

glow_tCtx glow_GetCtx( glow_tObject object)
{
  return (glow_tCtx)((GlowArrayElem *)object)->get_ctx();
}

void glow_SetTraceAttr( glow_tObject object, GlowTraceData *trace_data)
{
  ((GlowArrayElem *)object)->set_trace_attr( trace_data);
}

void glow_GetTraceAttr( glow_tObject object, GlowTraceData **trace_data)
{
  ((GlowArrayElem *)object)->get_trace_attr( trace_data);
}

extern "C" int glow_TraceInit( glow_tCtx ctx, 
	int (*trace_connect_func)( glow_tObject, GlowTraceData *),
	int (*trace_disconnect_func)( glow_tObject),
	int (*trace_scan_func)( glow_tObject, void *))
{
  return ctx->trace_init( trace_connect_func, trace_disconnect_func,
		trace_scan_func);
}

void glow_TraceClose( glow_tCtx ctx)
{
  ctx->trace_close();
}

void glow_TraceScan( glow_tCtx ctx)
{
  ctx->trace_scan();
}

void glow_RemoveTraceObjects( glow_tCtx ctx)
{
  ctx->remove_trace_objects();
}

void glow_Zoom( glow_tCtx ctx, double zoom_factor)
{
  ctx->zoom( zoom_factor);
}

void glow_ZoomAbsolute( glow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_absolute( zoom_factor);
}

void glow_SetAttributes( glow_tCtx ctx, glow_sAttributes *attr, 
	unsigned long mask)
{
  if ( mask & glow_eAttr_base_zoom_factor)
  {
    ctx->zoom_factor_x *= attr->base_zoom_factor / ctx->base_zoom_factor;
    ctx->zoom_factor_y *= attr->base_zoom_factor / ctx->base_zoom_factor;
    ctx->base_zoom_factor = attr->base_zoom_factor;
  }
  if ( mask & glow_eAttr_offset_x)
    ctx->offset_x = attr->offset_x;
  if ( mask & glow_eAttr_offset_y)
    ctx->offset_y = attr->offset_y;
  if ( mask & glow_eAttr_grid_size_x)
    ctx->grid_size_x = attr->grid_size_x;
  if ( mask & glow_eAttr_grid_size_y)
    ctx->grid_size_y = attr->grid_size_y;
  if ( mask & glow_eAttr_grid_on)
    ctx->grid_on = attr->grid_on;
  if ( mask & glow_eAttr_user_highlight)
    ctx->user_highlight = attr->user_highlight;
  if ( mask & glow_eAttr_draw_delta)
    ctx->draw_delta = attr->draw_delta;
  if ( mask & glow_eAttr_grafcet_con_delta)
    ctx->grafcet_con_delta = attr->grafcet_con_delta;
  if ( mask & glow_eAttr_refcon_width)
    ctx->refcon_width = attr->refcon_width;
  if ( mask & glow_eAttr_refcon_height)
    ctx->refcon_height = attr->refcon_height;
  if ( mask & glow_eAttr_refcon_linewidth)
    ctx->refcon_linewidth = attr->refcon_linewidth;
  if ( mask & glow_eAttr_refcon_textsize)
    ctx->refcon_textsize = attr->refcon_textsize;
  if ( mask & glow_eAttr_application_paste)
    ctx->application_paste = attr->application_paste;
  if ( mask & glow_eAttr_select_policy)
    ctx->select_policy = attr->select_policy;
  if ( mask & glow_eAttr_display_level)
    ctx->display_level = attr->display_level;
}

void glow_PositionToPixel( glow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y)
{
  ctx->position_to_pixel( x, y, pix_x, pix_y);
}

void glow_UnZoom( glow_tCtx ctx)
{
  ctx->unzoom();
}

void glow_CenterObject( glow_tCtx ctx, glow_tObject object)
{
  ctx->center_object( (GlowArrayElem *)object);
}

void glow_GetNodePosition( glow_tNode node, double *x, double *y)
{
  ((GlowNode *)node)->get_node_position( x, y);
}

void glow_GetConPosition( glow_tCon con, double *x_arr[], double *y_arr[],
	int *num)
{
  ((GlowCon *)con)->get_con_position( x_arr, y_arr, num);
}

void glow_MeasureAnnotation( glow_tNodeClass node_class, int number, char *text, 
	double *width, double *height)
{
  ((GlowNodeClass *)node_class)->measure_annotation( number, text, width, height);
}

void glow_MeasureAnnotText( glow_tCtx ctx, char *text, glow_eDrawType draw_type,
		int text_size, glow_eAnnotType annot_type,
		double *width, double *height, int *rows)
{
  glow_measure_annot_text( ctx, text, draw_type, text_size, annot_type, 
		width, height, rows);
}

glow_eNodeGroup glow_GetNodeGroup( glow_tNode node)
{
  return ((GlowNode *)node)->get_group();
}

glow_eConGroup glow_GetConGroup( glow_tCon con)
{
  return ((GlowCon *)con)->get_group();
}

void glow_DeleteNodeCons( glow_tNode node)
{
  ((GlowNode *)node)->ctx->delete_node_cons( node);
}

void glow_GetObjectName( glow_tObject object, char *name)
{
  ((GlowNode *)object)->get_object_name( name);
}

void glow_Reconfigure( glow_tCtx ctx)
{
  ctx->reconfigure();
}

void glow_SetNodraw( glow_tCtx ctx)
{
  ctx->set_nodraw();
}

void glow_ResetNodraw( glow_tCtx ctx)
{
  ctx->reset_nodraw();
}

void glow_Redraw( glow_tCtx ctx)
{
  ctx->redraw();
}

int glow_FindByName( glow_tCtx ctx, char *name, glow_tObject *object)
{
  return ctx->find_by_name( name, (GlowArrayElem **)object);
}
