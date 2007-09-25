/* 
 * Proview   $Id: flow_api.h,v 1.11 2007-09-25 13:11:00 claes Exp $
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

#ifndef flow_api_h
#define flow_api_h

#include "flow.h"

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef flow_browapi_h
typedef void *FlowCtx;
#endif
#endif


typedef struct {
	double	base_zoom_factor;
	int	offset_x;
	int	offset_y;
	double	grid_size_x;
	double	grid_size_y;
	int	grid_on;
	int	user_highlight;
	double	draw_delta;
	double	grafcet_con_delta;
	double	refcon_width;
	double	refcon_height;
	int	refcon_linewidth;
	int	refcon_textsize;
	int	application_paste;
	flow_eSelectPolicy select_policy;
	int	display_level;
        double  zoom_factor;
} flow_sAttributes;

typedef enum {
	flow_eAttr_base_zoom_factor 	= 1 << 0,
	flow_eAttr_offset_x		= 1 << 1,
	flow_eAttr_offset_y		= 1 << 2,
	flow_eAttr_grid_size_x		= 1 << 3,
	flow_eAttr_grid_size_y		= 1 << 4,
	flow_eAttr_grid_on		= 1 << 5,
	flow_eAttr_user_highlight	= 1 << 6,
	flow_eAttr_draw_delta		= 1 << 7,
	flow_eAttr_grafcet_con_delta	= 1 << 8,
	flow_eAttr_refcon_width		= 1 << 9,
	flow_eAttr_refcon_height	= 1 << 10,
	flow_eAttr_refcon_linewidth	= 1 << 11,
	flow_eAttr_refcon_textsize	= 1 << 12,
	flow_eAttr_application_paste	= 1 << 13,
	flow_eAttr_select_policy	= 1 << 14,
	flow_eAttr_display_level	= 1 << 15,
	flow_eAttr_zoom_factor		= 1 << 16
	} flow_eAttribute;


typedef FlowCtx *flow_tCtx;
typedef void *flow_tNode;
typedef void *flow_tCon;
typedef void *flow_tObject;
typedef void *flow_tNodeClass;
typedef void *flow_tConClass;

int flow_Save( flow_tCtx ctx, char *filename);
int flow_Open( flow_tCtx ctx, char *filename);
int flow_SaveTrace( flow_tCtx ctx, char *filename);
int flow_OpenTrace( flow_tCtx ctx, char *filename);
void flow_DeleteAll( flow_tCtx ctx);
void flow_DeleteNode( flow_tNode node);
void flow_DeleteConnection( flow_tCon con);
int flow_FindSelectedObject( flow_tCtx ctx, flow_tObject object);
void flow_ResetHighlightAll( flow_tCtx ctx);
void flow_ResetInverseAll( flow_tCtx ctx);
void flow_SetSelectHighlight( flow_tCtx ctx);
void flow_ResetSelectHighlight( flow_tCtx ctx);
void flow_SetSelectInverse( flow_tCtx ctx);
void flow_ResetSelectInverse( flow_tCtx ctx);
void flow_SelectInsert( flow_tCtx ctx, flow_tObject object);
void flow_SelectRemove( flow_tCtx ctx, flow_tObject object);
void flow_SelectClear( flow_tCtx ctx);
void flow_GetSelectedNodes( flow_tCtx ctx, flow_tNode **nodes, int *num);
void flow_GetSelectedCons( flow_tCtx ctx, flow_tCon **cons, int *num);
void flow_SetHighlight( flow_tObject object, int value);
void flow_GetHighlight( flow_tObject object, int *value);
void flow_SetInverse( flow_tObject object, int value);
void flow_CreateNode( flow_tCtx ctx, char *name, flow_tNodeClass nc,
	double x, double y, void *user_data, flow_tNode *node);
void flow_CreateCon( flow_tCtx ctx, char *name, flow_tConClass cc,
	flow_tNode source, flow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, flow_tCon *con, 
	int point_num, double *x_vect, double *y_vect);
void flow_CreatePasteNode( flow_tCtx ctx, char *name, flow_tNodeClass nc,
	double x, double y, void *user_data, flow_tNode *node);
void flow_CreatePasteCon( flow_tCtx ctx, char *name, flow_tConClass cc,
	flow_tNode source, flow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, flow_tCon *con, 
	int point_num, double *x_vect, double *y_vect);
void flow_SetAnnotation( flow_tNode node, int number, char *text, int size);
void flow_SetPasteNodeAnnotation( flow_tNode node, int number, char *text, 
	int size);
void flow_EnableEvent( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event));
void flow_DisableEvent( FlowCtx *ctx, flow_eEvent event);
void flow_DisableEventAll( FlowCtx *ctx);
void flow_Cut( flow_tCtx ctx);
void flow_Copy( flow_tCtx ctx);
void flow_Paste( flow_tCtx ctx);
void flow_PasteClear( flow_tCtx ctx);
void flow_CreateRect( flow_tCtx ctx, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, flow_tObject *rect);
void flow_CreateLine( flow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width, flow_tObject *line);
void flow_CreateArc( flow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	flow_eDrawType draw_type, int line_width, flow_tObject *arc);
void flow_CreateText( flow_tCtx ctx, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size, flow_tObject *text);
void flow_CreatePixmap( flow_tCtx ctx, flow_sPixmapData *pixmap_data, 
	double x, double y, flow_eDrawType draw_type, int size, 
	flow_tObject *pixmap);
void flow_CreateAnnot( flow_tCtx ctx, double x, double y, int number, 
	flow_eDrawType draw_type, int text_size, flow_tObject *annot);
void flow_CreateConPoint( flow_tCtx ctx, double x, double y, int number,
	flow_eDirection direction, flow_tObject *conpoint);
void flow_AddRect( flow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, int line_width, flow_mDisplayLevel display_level);
void flow_AddFilledRect( flow_tNodeClass nc, double x, double y, 
	double width, double height,
	flow_eDrawType draw_type, flow_mDisplayLevel display_level);
void flow_AddLine( flow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	flow_eDrawType draw_type, int line_width);
void flow_AddArc( flow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	flow_eDrawType draw_type, int line_width);
void flow_AddText( flow_tNodeClass nc, char *text_str, double x, double y, 
	flow_eDrawType draw_type, int text_size);
void flow_AddAnnot( flow_tNodeClass nc, double x, double y, int number,
	flow_eDrawType draw_type, int text_size, flow_eAnnotType annot_type,
	flow_mDisplayLevel display_level);
void flow_AddConPoint( flow_tNodeClass nc, double x, double y, int number,
	flow_eDirection direction);
void flow_CreatePushButton( flow_tCtx ctx, char *text, double x, double y, 
	double width, double height, flow_tObject *pushbutton);
void flow_CreateNodeClass( flow_tCtx ctx, char *name, flow_eNodeGroup group,
	flow_tNodeClass *nodeclass);
void flow_NodeClassAdd( flow_tNodeClass nc, flow_tObject object);
void flow_CreateConClass( flow_tCtx ctx, char *name,
	flow_eConType con_type, flow_eCorner corner, flow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, flow_eConGroup group,
	flow_tConClass *conclass);
void flow_SetDefaultConClass( flow_tCtx ctx, flow_tConClass conclass);
flow_tConClass flow_GetDefaultConClass( flow_tCtx ctx);
void flow_GetSelectList( flow_tCtx ctx, flow_tObject **list, int *cnt);
void flow_GetObjectList( flow_tCtx ctx, flow_tObject **list, int *cnt);
flow_eObjectType flow_GetObjectType( flow_tObject object);
void flow_MeasureNode( flow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y);
int flow_PrintRegion( flow_tCtx ctx, double ll_x, double ll_y,
	double ur_x, double ur_y, char *filename);
int flow_PrintPdfRegion( flow_tCtx ctx, double ll_x, double ll_y,
			 double ur_x, double ur_y, char *filename);
void flow_GetUserData( flow_tObject object, void **user_data);
void flow_SetUserData( flow_tObject object, void *user_data);
void flow_GetCtxUserData( flow_tCtx ctx, void **user_data);
void flow_SetCtxUserData( flow_tCtx ctx, void *user_data);
flow_tCtx flow_GetCtx( flow_tObject object);
void flow_SetTraceAttr( flow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType trace_attr_type, int inverted);
void flow_GetTraceAttr( flow_tObject object, char *trace_object, 
		char *trace_attribute, flow_eTraceType *trace_attr_type, int *inverted);
int flow_TraceInit( flow_tCtx ctx, int (*trace_connect_func)( flow_tObject, 
	char *, char *, flow_eTraceType, void **), 
	int (*trace_disconnect_func)( flow_tObject),
	int (*trace_scan_func)( flow_tObject, void *));
void flow_TraceClose( flow_tCtx ctx);
void flow_TraceScan( flow_tCtx ctx);
void flow_RemoveTraceObjects( flow_tCtx ctx);
void flow_Zoom( flow_tCtx ctx, double zoom_factor);
void flow_ZoomAbsolute( flow_tCtx ctx, double zoom_factor);
void flow_Scroll( flow_tCtx ctx, double x, double y);
void flow_SetAttributes( flow_tCtx ctx, flow_sAttributes *attr,
	unsigned long mask);
void flow_GetAttributes( flow_tCtx ctx, flow_sAttributes *attr);
void flow_PositionToPixel( flow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y);
void flow_PixelToPosition( flow_tCtx ctx, int pix_x, int pix_y, 
			   double *x, double *y);
void flow_UnZoom( flow_tCtx ctx);
void flow_CenterObject( flow_tCtx ctx, flow_tObject object);
void flow_GetNodePosition( flow_tNode node, double *x, double *y);
void flow_GetConPosition( flow_tCon con, double *x_arr[], double *y_arr[],
	int *num);
void flow_MeasureAnnotation( flow_tNodeClass node_class, int number, char *text, 
	double *width, double *height);
void flow_MeasureAnnotText( flow_tCtx ctx, char *text, flow_eDrawType draw_type,
		int text_size, flow_eAnnotType annot_type,
		double *width, double *height, int *rows);
flow_eNodeGroup flow_GetNodeGroup( flow_tNode node);
flow_eConGroup flow_GetConGroup( flow_tCon con);
void flow_DeleteNodeCons( flow_tNode node);
void flow_GetObjectName( flow_tObject object, char *name);
void flow_Reconfigure( flow_tCtx ctx);
void flow_SetNodraw( flow_tCtx ctx);
void flow_ResetNodraw( flow_tCtx ctx);
void flow_Redraw( flow_tCtx ctx);
int flow_FindByName( flow_tCtx ctx, char *name, flow_tObject *object);
int flow_FindByNameNoCase( flow_tCtx ctx, char *name, flow_tObject *object);
int flow_GetConPointTraceAttr( flow_tObject object, int num, char *trace_attr, 
	flow_eTraceType *type);
int flow_GetConPoint( flow_tObject object,  int num, double *x, double *y, 
	flow_eDirection *dir);
void flow_SetClickSensitivity( flow_tCtx ctx, int value);
void flow_SetNoConObstacle( flow_tNodeClass nc, int no_obstacle);
int flow_GetNextObject( flow_tCtx ctx, flow_tNode object, flow_eDirection dir,
			flow_tNode *next);
int flow_IsVisible( flow_tCtx ctx, flow_tObject object, flow_eVisible type);

#if defined __cplusplus
}
#endif
#endif
