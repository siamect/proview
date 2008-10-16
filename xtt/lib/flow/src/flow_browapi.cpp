/* 
 * Proview   $Id: flow_browapi.cpp,v 1.13 2008-10-16 08:58:06 claes Exp $
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
#include "flow_browctx.h"
#include "flow_point.h"
#include "flow_rect.h"
#include "flow_frame.h"
#include "flow_line.h"
#include "flow_arc.h"
#include "flow_text.h"
#include "flow_pixmap.h"
#include "flow_annotpixmap.h"
#include "flow_image.h"
#include "flow_pushbutton.h"
#include "flow_array_elem.h"
#include "flow_array.h"
#include "flow_nodeclass.h"
#include "flow_node.h"
#include "flow_conclass.h"
#include "flow_con.h"
#include "flow_conpoint.h"
#include "flow_annot.h"
#include "flow_radiobutton.h"
#include "flow_tiptext.h"
#include "flow_draw.h"
#include "flow_browapi.h"

int brow_Save( brow_tCtx ctx, char *filename)
{
  return ctx->save( filename, flow_eSaveMode_Edit);
}

int brow_Open( brow_tCtx ctx, char *filename)
{
  return ctx->open( filename, flow_eSaveMode_Edit);
}

int brow_SaveTrace( brow_tCtx ctx, char *filename)
{
  return ctx->save( filename, flow_eSaveMode_Trace);
}

int brow_OpenTrace( brow_tCtx ctx, char *filename)
{
  return ctx->open( filename, flow_eSaveMode_Trace);
}

void brow_DeleteNode( brow_tCtx ctx, brow_tNode node)
{
  ctx->remove((FlowArrayElem *)node);
}

void brow_DeleteAll( brow_tCtx ctx)
{
  ctx->delete_all();
}

void brow_CloseNode( brow_tCtx ctx, brow_tNode node)
{
  ctx->close((FlowArrayElem *)node);
}

int brow_FindSelectedObject( brow_tCtx ctx, brow_tObject object)
{
  return ctx->select_find( (FlowArrayElem *)object);
}

void brow_ResetHighlightAll( brow_tCtx ctx)
{
  ctx->set_highlight( 0);
}

void brow_ResetInverseAll( brow_tCtx ctx)
{
  ctx->set_inverse( 0);
}

void brow_ResetSelectInverse( brow_tCtx ctx)
{
  ctx->set_select_inverse( 0);
}

void brow_ResetSelectHighlight( brow_tCtx ctx)
{
  ctx->set_select_highlight( 0);
}

void brow_SetSelectHighlight( brow_tCtx ctx)
{
  ctx->set_select_highlight( 1);
}

void brow_SetSelectInverse( brow_tCtx ctx)
{
  ctx->set_select_inverse( 1);
}

void brow_SelectInsert( brow_tCtx ctx, brow_tObject object)
{
  ctx->select_insert( (FlowArrayElem *)object);
}

void brow_SelectRemove( brow_tCtx ctx, brow_tObject object)
{
  ctx->select_remove( (FlowArrayElem *)object);
}

void brow_SelectClear( brow_tCtx ctx)
{
  ctx->select_clear();
}

void brow_GetSelectedNodes( brow_tCtx ctx, brow_tNode **nodes, int *num)
{
  ctx->get_selected_nodes( (FlowArrayElem ***) nodes, num);
}

void brow_SetHighlight( brow_tObject object, int value)
{
  ((FlowArrayElem *) object)->set_highlight( value);
}

void brow_GetHighlight( brow_tObject object, int *value)
{
  *value = ((FlowArrayElem *) object)->get_highlight();
}

void brow_SetInverse( brow_tObject object, int value)
{
  ((FlowArrayElem *) object)->set_inverse( value);
}

void brow_CreateNode( brow_tCtx ctx, char *name, brow_tNodeClass nc,
	brow_tNode destination, flow_eDest destination_code, 
	void *user_data, int relative_annot_pos, brow_tNode *node)
{
  FlowNode *n1;
  n1 = new FlowNode( ctx, name, (FlowNodeClass *)nc, 0, 0, 0, 
	relative_annot_pos);
  n1->set_user_data( user_data);
  ctx->insert( n1, (FlowArrayElem *)destination, destination_code);
  ctx->nav_zoom();
  *node = (brow_tNode) n1;
}

void brow_CreatePasteNode( brow_tCtx ctx, char *name, brow_tNodeClass nc,
	double x, double y, void *user_data, brow_tNode *node)
{
  FlowNode *n1;
  n1 = new FlowNode( ctx, name, (FlowNodeClass *)nc, x, y, 1);
  n1->set_user_data( user_data);
  ctx->paste_insert( n1);
  *node = (brow_tNode) n1;
}

void brow_SetAnnotation( brow_tNode node, int number, char *text, int size)
{
  ((FlowNode *)node)->set_annotation( number, text, size, 0);
}

void brow_GetAnnotation( brow_tNode node, int number, char *text, int size)
{
  ((FlowNode *)node)->get_annotation( number, text, size);
}

void brow_OpenAnnotationInput( brow_tNode node, int number)
{
  ((FlowNode *)node)->open_annotation_input( number);
}

void brow_CloseAnnotationInput( brow_tNode node, int number)
{
  ((FlowNode *)node)->close_annotation_input( number);
}

int brow_GetAnnotationInput( brow_tNode node, int number, char **text)
{
  return ((FlowNode *)node)->get_annotation_input( number, text);
}

int brow_AnnotationInputIsOpen( brow_tNode node, int number)
{
  return ((FlowNode *)node)->annotation_input_is_open( number);
}

void brow_SetPasteNodeAnnotation( brow_tNode node, int number, char *text, int size)
{
  ((FlowNode *)node)->set_annotation( number, text, size, 1);
}

extern "C" void	brow_EnableEvent( BrowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

void	brow_DisableEvent( BrowCtx *ctx, flow_eEvent event)
{
  ctx->disable_event( event);
}

extern "C" void	brow_DisableEventAll( BrowCtx *ctx)
{
  ctx->disable_event_all();
}

void brow_Cut( brow_tCtx ctx)
{
  ctx->cut();
}

void brow_Copy( brow_tCtx ctx)
{
  ctx->copy();
}

void brow_Paste( brow_tCtx ctx)
{
  ctx->paste();
}

void brow_PasteClear( brow_tCtx ctx)
{
  ctx->paste_clear();
}

void brow_CreateRect( brow_tCtx ctx, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, int fix_line_width, 
	brow_tObject *rect)
{
  *rect = (brow_tObject) new FlowRect( ctx, x, y, width, height, draw_type,
	line_width, fix_line_width);
}

void brow_CreateFilledRect( brow_tCtx ctx, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type,
	brow_tObject *rect)
{
  *rect = (brow_tObject) new FlowRect( ctx, x, y, width, height, draw_type,
	1, 0, flow_mDisplayLevel_1, 1);
}

void brow_CreateLine( brow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width, int fix_line_width, 
	brow_tObject *line)
{
  *line = (brow_tObject) new FlowLine( ctx, x1, y1, x2, y2, draw_type,
	line_width, fix_line_width);
}

void brow_CreateArc( brow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angle1, int angle2,
	flow_eDrawType draw_type, int line_width, brow_tObject *arc)
{
  *arc = (brow_tObject) new FlowArc( ctx, x1, y1, x2, y2, 
	angle1, angle2, draw_type, line_width);
}

void brow_CreateText( brow_tCtx ctx, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size, brow_tObject *text)
{
  *text = (brow_tObject) new FlowText( ctx, text_str, x, y, draw_type,
	text_size);
}

void brow_CreatePixmap( brow_tCtx ctx, flow_sPixmapData *pixmap_data, 
	double x, double y, flow_eDrawType draw_type, int size, 
	brow_tObject *pixmap)
{
  *pixmap = (brow_tObject) new FlowPixmap( ctx, pixmap_data, x, y,
	draw_type, size);
}

void brow_CreateAnnotPixmap( brow_tCtx ctx, int number, 
	double x, double y, flow_eDrawType draw_type, int size, int relative_pos, 
	brow_tObject *annot_pixmap)
{
  *annot_pixmap = (brow_tObject) new FlowAnnotPixmap( ctx, number, x, y,
	draw_type, size, relative_pos);
}

void brow_CreateAnnot( brow_tCtx ctx, double x, double y, int number,
	flow_eDrawType draw_type, int text_size, brow_tObject *annot)
{
  *annot = (brow_tObject) new FlowAnnot( ctx, x, y, number, draw_type,
	text_size);
}

void brow_AddRect( brow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, int fix_line_width)
{
  FlowRect *rect = new FlowRect( ((FlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, line_width, fix_line_width);
  ((FlowNodeClass *)nc)->insert( rect);
  
}

void brow_AddFilledRect( brow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type)
{
  FlowRect *rect = new FlowRect( ((FlowNodeClass *)nc)->ctx, x, y, 
	width, height, draw_type, 1, 0, flow_mDisplayLevel_1, 1);
  ((FlowNodeClass *)nc)->insert( rect);

}

void brow_AddFrame( brow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, int fix_line_width)
{
  FlowFrame *frame = new FlowFrame( ((FlowNodeClass *)nc)->ctx, x, y, 
	width, height,
	draw_type, line_width, fix_line_width);
  ((FlowNodeClass *)nc)->insert( frame);
  
}

void brow_AddLine( brow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width)
{
  FlowLine *line = new FlowLine( ((FlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, draw_type, line_width);
  ((FlowNodeClass *)nc)->insert( line);
}

void brow_AddArc( brow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angle1, int angle2,
	flow_eDrawType draw_type, int line_width)
{
  FlowArc *arc = new FlowArc( ((FlowNodeClass *)nc)->ctx, 
	x1, y1, x2, y2, angle1, angle2, draw_type, line_width);
  ((FlowNodeClass *)nc)->insert( arc);
}

void brow_AddImage( brow_tNodeClass nc, char *imagefile, double x, double y)
{
  FlowImage *image = new FlowImage( ((FlowNodeClass *)nc)->ctx, imagefile, x, y);
  ((FlowNodeClass *)nc)->insert( image);
  
}

void brow_AddText( brow_tNodeClass nc, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size)
{
  FlowText *text = new FlowText( ((FlowNodeClass *)nc)->ctx, 
	text_str, x, y, draw_type, text_size);
  ((FlowNodeClass *)nc)->insert( text);
}

void brow_AddAnnot( brow_tNodeClass nc, double x, double y, int number,
	flow_eDrawType draw_type, int text_size, flow_eAnnotType annot_type,
	int relative_pos)
{
  FlowAnnot *annot = new FlowAnnot( ((FlowNodeClass *)nc)->ctx, 
	x, y, number, draw_type, text_size, annot_type, relative_pos);
  ((FlowNodeClass *)nc)->insert( annot);
}

void brow_AddAnnotPixmap( brow_tNodeClass nc, int number,
	double x, double y, flow_eDrawType draw_type, int size, int relative_pos)
{
   FlowAnnotPixmap *annotpixmap = new FlowAnnotPixmap( 
	((FlowNodeClass *)nc)->ctx, number, x, y, draw_type, 
	size, relative_pos);
  ((FlowNodeClass *)nc)->insert( annotpixmap);
}

void brow_AddRadiobutton( brow_tNodeClass nc, double x, double y, 
	double width, double height, int number,
	flow_eDrawType draw_type, int line_width)
{
  FlowRadiobutton *rbutton = new FlowRadiobutton( ((FlowNodeClass *)nc)->ctx, 
	x, y, width, height, number, draw_type, line_width);
  ((FlowNodeClass *)nc)->insert( rbutton);
  
}

void brow_CreateNodeClass( brow_tCtx ctx, char *name, flow_eNodeGroup group,
	brow_tNodeClass *nodeclass)
{
  *nodeclass = (brow_tNodeClass) new FlowNodeClass( ctx, name, group);
  ctx->nodeclass_insert( (FlowArrayElem *) *nodeclass);
}

void brow_NodeClassAdd( brow_tNodeClass nc, brow_tObject object)
{
  ((FlowNodeClass *)nc)->insert( (FlowArrayElem *)object);
}

void brow_GetSelectList( brow_tCtx ctx, brow_tObject **list, int *cnt)
{
  ctx->get_selectlist( (FlowArrayElem ***)list, cnt);
}

void brow_GetObjectList( brow_tCtx ctx, brow_tObject **list, int *cnt)
{
  ctx->get_objectlist( (FlowArrayElem ***)list, cnt);
}

flow_eObjectType brow_GetObjectType( brow_tObject object)
{
  return ((FlowArrayElem *)object)->type();
}

void brow_MeasureNode( brow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y)
{
  ((FlowNode *)node)->measure( ll_x, ll_y, ur_x, ur_y);
}

int brow_Print( brow_tCtx ctx, char *filename)
{
  return ctx->print( filename);
}

void brow_GetUserData( brow_tObject object, void **user_data)
{
  ((FlowArrayElem *)object)->get_user_data( user_data);
}

void brow_SetUserData( brow_tObject object, void *user_data)
{
  ((FlowArrayElem *)object)->set_user_data( user_data);
}

void brow_GetCtxUserData( brow_tCtx ctx, void **user_data)
{
  ((FlowCtx *)ctx)->get_user_data( user_data);
}

void brow_SetCtxUserData( brow_tCtx ctx, void *user_data)
{
  ((FlowCtx *)ctx)->set_user_data( user_data);
}

brow_tCtx brow_GetCtx( brow_tObject object)
{
  return (brow_tCtx)((FlowArrayElem *)object)->get_ctx();
}

void brow_SetTraceAttr( brow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType trace_attr_type)
{
  ((FlowArrayElem *)object)->set_trace_attr( trace_object, trace_attribute,
	trace_attr_type, 0);
}

void brow_GetTraceAttr( brow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType *trace_attr_type)
{
  int inverted;

  ((FlowArrayElem *)object)->get_trace_attr( trace_object, trace_attribute,
	trace_attr_type, &inverted);
}

void brow_SetTraceData( brow_tObject object, void *trace_data)
{
  ((FlowNode *)object)->set_trace_data( trace_data);
}

extern "C" int brow_TraceInit( brow_tCtx ctx, int (*trace_connect_func)( brow_tObject, 
	char *, char *, flow_eTraceType, void **), 
	int (*trace_disconnect_func)( brow_tObject),
	int (*trace_scan_func)( brow_tObject, void *))
{
  return ctx->trace_init( trace_connect_func, trace_disconnect_func,
	trace_scan_func);
}

void brow_TraceClose( brow_tCtx ctx)
{
  ctx->trace_close();
}

void brow_TraceScan( brow_tCtx ctx)
{
  ctx->trace_scan();
}

void brow_RemoveTraceObjects( brow_tCtx ctx)
{
  ctx->remove_trace_objects();
}

void brow_Zoom( brow_tCtx ctx, double zoom_factor)
{
  ctx->zoom( zoom_factor);
}

void brow_GetZoom( brow_tCtx ctx, double *zoom_factor)
{
  ctx->get_zoom( zoom_factor);
}

void brow_ZoomAbsolute( brow_tCtx ctx, double zoom_factor)
{
  ctx->zoom_absolute( zoom_factor);
}

void brow_SetAttributes( brow_tCtx ctx, brow_sAttributes *attr, 
	unsigned long mask)
{
  if ( mask & brow_eAttr_base_zoom_factor)
  {
    ctx->zoom_factor *= attr->base_zoom_factor / ctx->base_zoom_factor;
    ctx->base_zoom_factor = attr->base_zoom_factor;
  }
  if ( mask & brow_eAttr_indentation)
    ctx->indentation = attr->indentation;
  if ( mask & brow_eAttr_annotation_space)
    ctx->annotation_space = attr->annotation_space;
}

void brow_PositionToPixel( brow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y)
{
  ctx->position_to_pixel( x, y, pix_x, pix_y);
}

void brow_UnZoom( brow_tCtx ctx)
{
  ctx->unzoom();
}

void brow_CenterObject( brow_tCtx ctx, brow_tObject object, double factor)
{
  ctx->center_object( (FlowArrayElem *)object, factor);
}

void brow_GetNodePosition( brow_tNode node, double *x, double *y)
{
  ((FlowNode *)node)->get_node_position( x, y);
}

void brow_MeasureAnnotation( brow_tNodeClass node_class, int number, char *text, 
	double *width, double *height)
{
  ((FlowNodeClass *)node_class)->measure_annotation( number, text, width, height);
}

void brow_MeasureAnnotText( brow_tCtx ctx, char *text, flow_eDrawType draw_type,
		int text_size, flow_eAnnotType annot_type,
		double *width, double *height, int *rows)
{
  flow_measure_annot_text( ctx, text, draw_type, text_size, annot_type, 
		width, height, rows);
}

flow_eNodeGroup brow_GetNodeGroup( brow_tNode node)
{
  return ((FlowNode *)node)->get_group();
}

void brow_GetObjectName( brow_tObject object, char *name)
{
  ((FlowNode *)object)->get_object_name( name);
}

void brow_Reconfigure( brow_tCtx ctx)
{
  ctx->reconfigure();
}

void brow_SetNodraw( brow_tCtx ctx)
{
  ctx->set_nodraw();
}

void brow_ResetNodraw( brow_tCtx ctx)
{
  ctx->reset_nodraw();
}

void brow_Redraw( brow_tCtx ctx, double y_redraw)
{
  ctx->configure( y_redraw);
}

void brow_SetAnnotPixmap( brow_tNode node, int number, 
	flow_sAnnotPixmap *pixmap)
{
  ((FlowNode *)node)->set_annot_pixmap( number, pixmap, 0);
}

void brow_RemoveAnnotPixmap( brow_tNode node, int number)
{
  ((FlowNode *)node)->remove_annot_pixmap( number);
}

void brow_AllocAnnotPixmap( brow_tCtx ctx, flow_sPixmapData *pixmap_data, 
		flow_sAnnotPixmap **pixmap)
{
  flow_annot_pixmap_create( ctx, pixmap_data, pixmap);
}

void brow_FreeAnnotPixmap( brow_tCtx ctx, flow_sAnnotPixmap *pixmap)
{
  flow_annot_pixmap_free( ctx, pixmap);
}

void brow_SetRadiobutton( brow_tNode node, int number, int value)
{
  ((FlowNode *)node)->set_radiobutton( number, value, 0);
}

void brow_GetRadiobutton( brow_tNode node, int number, int *value)
{
  ((FlowNode *)node)->get_radiobutton( number, value);
}

void brow_SetOpen( brow_tNode node, int mask)
{
  ((FlowNode *)node)->set_open( mask);
}

void brow_ResetOpen( brow_tNode node, int mask)
{
  ((FlowNode *)node)->reset_open( mask);
}

int brow_IsOpen( brow_tNode node)
{
  return ((FlowNode *)node)->is_open();
}

int brow_GetNext( brow_tCtx ctx, brow_tObject object, brow_tObject *next)
{
  return ctx->get_next( (FlowArrayElem *)object, (FlowArrayElem **)next);
}

int brow_GetPrevious( brow_tCtx ctx, brow_tObject object, brow_tObject *prev)
{
  return ctx->get_previous( (FlowArrayElem *)object, (FlowArrayElem **)prev);
}

int brow_GetFirst( brow_tCtx ctx, brow_tObject *first)
{
  return ctx->get_first( (FlowArrayElem **)first);
}

int brow_GetLast( brow_tCtx ctx, brow_tObject *last)
{
  return ctx->get_last( (FlowArrayElem **)last);
}

int brow_GetParent( brow_tCtx ctx, brow_tObject object, brow_tObject *parent)
{
  return ctx->get_parent( (FlowArrayElem *)object, (FlowArrayElem **)parent);
}

int brow_GetChild( brow_tCtx ctx, brow_tObject object, brow_tObject *child)
{
  return ctx->get_child( (FlowArrayElem *)object, (FlowArrayElem **)child);
}

int brow_GetNextSibling( brow_tCtx ctx, brow_tObject object, 
		brow_tObject *sibling)
{
  return ctx->get_next_sibling( (FlowArrayElem *)object, (FlowArrayElem **)sibling);
}

int brow_GetPreviousSibling( brow_tCtx ctx, brow_tObject object, 
		brow_tObject *sibling)
{
  return ctx->get_previous_sibling( (FlowArrayElem *)object, (FlowArrayElem **)sibling);
}

int brow_IsVisible( brow_tCtx ctx, brow_tObject object, flow_eVisible type)
{
  return ctx->is_visible( (FlowArrayElem *)object, type);
}

int brow_GetFirstVisible( brow_tCtx ctx, brow_tObject *object)
{
  return ctx->get_first_visible( (FlowArrayElem **)object);
}

int brow_GetLastVisible( brow_tCtx ctx, brow_tObject *object)
{
  return ctx->get_last_visible( (FlowArrayElem **)object);
}

int brow_Page( brow_tCtx ctx, double factor)
{
  return ctx->page( factor);
}

extern "C" int brow_CreateSecondaryCtx( brow_tCtx ctx, brow_tCtx *secondary_ctx,
        int (*init_proc)(brow_tCtx ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type)
{
  return ctx->fdraw->create_secondary_ctx( (FlowCtx *)ctx, 
	(void **)secondary_ctx, 
	(int (*)(FlowCtx *, void *))init_proc, 
	client_data, type);
}

void brow_DeleteSecondaryCtx( brow_tCtx ctx)
{
  ctx->fdraw->delete_secondary_ctx( (FlowCtx *)ctx);
}

int brow_ChangeCtx( brow_tCtx from_ctx, brow_tCtx to_ctx)
{
  return from_ctx->fdraw->change_ctx( (FlowCtx *)from_ctx, (FlowCtx *)to_ctx);
}

void brow_SetInputFocus( brow_tCtx ctx)
{
  ctx->fdraw->set_inputfocus( (FlowCtx *)ctx);
}

void brow_SetClickSensitivity( brow_tCtx ctx, int value)
{
  ctx->fdraw->set_click_sensitivity( (FlowCtx *)ctx, value);
}

void brow_SetWhiteBackground( brow_tCtx ctx)
{
  ctx->fdraw->set_white_background( (FlowCtx *)ctx);
}

void brow_SetFillColor( brow_tNode node, flow_eDrawType color)
{
  ((FlowNode *)node)->set_fillcolor( color);  
}

void brow_SetTipText( brow_tCtx ctx, brow_tObject object, char *text, int x, int y)
{
  ctx->tiptext->draw_text( (FlowArrayElem *)object, text, x, y);
}

void brow_SetInverseColor( brow_tCtx ctx, flow_eDrawType color)
{
  ctx->set_inverse_color( color);
}

int brow_GetObjectLevel( brow_tObject object)
{
  return ((FlowNode *)object)->get_level();
}
