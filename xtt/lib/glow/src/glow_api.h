#ifndef glow_api_h
#define glow_api_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef glow_browapi_h
typedef void *GlowCtx;
typedef struct {
	char		data[5][120];
	glow_eTraceType	attr_type;
	glow_eDrawType	color;
	void		*p;
	} glow_sTraceData, GlowTraceData;
#endif
#else
typedef GlowTraceData glow_sTraceData;
#endif

#include "glow.h"

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
	glow_eSelectPolicy select_policy;
	int	display_level;
} glow_sAttributes;

typedef enum {
	glow_eAttr_base_zoom_factor 	= 1 << 0,
	glow_eAttr_offset_x		= 1 << 1,
	glow_eAttr_offset_y		= 1 << 2,
	glow_eAttr_grid_size_x		= 1 << 3,
	glow_eAttr_grid_size_y		= 1 << 4,
	glow_eAttr_grid_on		= 1 << 5,
	glow_eAttr_user_highlight	= 1 << 6,
	glow_eAttr_draw_delta		= 1 << 7,
	glow_eAttr_grafcet_con_delta	= 1 << 8,
	glow_eAttr_refcon_width		= 1 << 9,
	glow_eAttr_refcon_height	= 1 << 10,
	glow_eAttr_refcon_linewidth	= 1 << 11,
	glow_eAttr_refcon_textsize	= 1 << 12,
	glow_eAttr_application_paste	= 1 << 13,
	glow_eAttr_select_policy	= 1 << 14,
	glow_eAttr_display_level	= 1 << 15
	} glow_eAttribute;


typedef GlowCtx *glow_tCtx;
typedef void *glow_tNode;
typedef void *glow_tCon;
typedef void *glow_tObject;
typedef void *glow_tNodeClass;
typedef void *glow_tConClass;

int glow_Save( glow_tCtx ctx, char *filename);
int glow_Open( glow_tCtx ctx, char *filename);
int glow_SaveTrace( glow_tCtx ctx, char *filename);
int glow_OpenTrace( glow_tCtx ctx, char *filename);
void glow_DeleteNode( glow_tNode node);
void glow_DeleteConnection( glow_tCon con);
int glow_FindSelectedObject( glow_tCtx ctx, glow_tObject object);
void glow_ResetHighlightAll( glow_tCtx ctx);
void glow_ResetInverseAll( glow_tCtx ctx);
void glow_SetSelectHighlight( glow_tCtx ctx);
void glow_ResetSelectHighlight( glow_tCtx ctx);
void glow_SetSelectInverse( glow_tCtx ctx);
void glow_ResetSelectInverse( glow_tCtx ctx);
void glow_SelectInsert( glow_tCtx ctx, glow_tObject object);
void glow_SelectRemove( glow_tCtx ctx, glow_tObject object);
void glow_SelectClear( glow_tCtx ctx);
void glow_GetSelectedNodes( glow_tCtx ctx, glow_tNode **nodes, int *num);
void glow_GetSelectedCons( glow_tCtx ctx, glow_tCon **cons, int *num);
void glow_SetHighlight( glow_tObject object, int value);
void glow_GetHighlight( glow_tObject object, int *value);
void glow_SetInverse( glow_tObject object, int value);
void glow_CreateNode( glow_tCtx ctx, char *name, glow_tNodeClass nc,
	double x, double y, void *user_data, glow_tNode *node);
void glow_CreateCon( glow_tCtx ctx, char *name, glow_tConClass cc,
	glow_tNode source, glow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, glow_tCon *con, 
	int point_num, double *x_vect, double *y_vect);
void glow_CreatePasteNode( glow_tCtx ctx, char *name, glow_tNodeClass nc,
	double x, double y, void *user_data, glow_tNode *node);
void glow_CreatePasteCon( glow_tCtx ctx, char *name, glow_tConClass cc,
	glow_tNode source, glow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, glow_tCon *con, 
	int point_num, double *x_vect, double *y_vect);
void glow_SetAnnotation( glow_tNode node, int number, char *text, int size);
void glow_SetPasteNodeAnnotation( glow_tNode node, int number, char *text, 
	int size);
void glow_EnableEvent( GlowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));
void glow_DisableEvent( GlowCtx *ctx, glow_eEvent event);
void glow_DisableEventAll( GlowCtx *ctx);
void glow_Cut( glow_tCtx ctx);
void glow_Copy( glow_tCtx ctx);
void glow_Paste( glow_tCtx ctx);
void glow_PasteClear( glow_tCtx ctx);
void glow_CreateRect( glow_tCtx ctx, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, glow_tObject *rect);
void glow_CreateLine( glow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width, glow_tObject *line);
void glow_CreateArc( glow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width, glow_tObject *arc);
void glow_CreateText( glow_tCtx ctx, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size, glow_tObject *text);
void glow_CreatePixmap( glow_tCtx ctx, glow_sPixmapData *pixmap_data, 
	double x, double y, glow_eDrawType draw_type, int size, 
	glow_tObject *pixmap);
void glow_CreateAnnot( glow_tCtx ctx, double x, double y, int number, 
	glow_eDrawType draw_type, int text_size, glow_tObject *annot);
void glow_CreateConPoint( glow_tCtx ctx, double x, double y, int number,
	glow_eDirection direction, glow_tObject *conpoint);
void glow_AddRect( glow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, glow_mDisplayLevel display_level);
void glow_AddFilledRect( glow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, glow_mDisplayLevel display_level);
void glow_AddLine( glow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width);
void glow_AddArc( glow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width);
void glow_AddText( glow_tNodeClass nc, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size);
void glow_AddAnnot( glow_tNodeClass nc, double x, double y, int number,
	glow_eDrawType draw_type, int text_size, glow_eAnnotType annot_type,
	glow_mDisplayLevel display_level);
void glow_AddConPoint( glow_tNodeClass nc, double x, double y, int number,
	glow_eDirection direction);
void glow_CreatePushButton( glow_tCtx ctx, char *text, double x, double y, 
	double width, double height, glow_tObject *pushbutton);
void glow_CreateNodeClass( glow_tCtx ctx, char *name, glow_eNodeGroup group,
	glow_tNodeClass *nodeclass);
void glow_NodeClassAdd( glow_tNodeClass nc, glow_tObject object);
void glow_CreateConClass( glow_tCtx ctx, char *name,
	glow_eConType con_type, glow_eCorner corner, glow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, glow_eConGroup group,
	glow_tConClass *conclass);
void glow_SetDefaultConClass( glow_tCtx ctx, glow_tConClass conclass);
glow_tConClass glow_GetDefaultConClass( glow_tCtx ctx);
void glow_GetSelectList( glow_tCtx ctx, glow_tObject **list, int *cnt);
void glow_GetObjectList( glow_tCtx ctx, glow_tObject **list, int *cnt);
glow_eObjectType glow_GetObjectType( glow_tObject object);
void glow_MeasureNode( glow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y);
void glow_PrintRegion( glow_tCtx ctx, double ll_x, double ll_y,
	double ur_x, double ur_y, char *filename);
void glow_GetUserData( glow_tObject object, void **user_data);
void glow_SetUserData( glow_tObject object, void *user_data);
void glow_GetCtxUserData( glow_tCtx ctx, void **user_data);
void glow_SetCtxUserData( glow_tCtx ctx, void *user_data);
glow_tCtx glow_GetCtx( glow_tObject object);
void glow_SetTraceAttr( glow_tObject object, GlowTraceData *trace_data);
void glow_GetTraceAttr( glow_tObject object, GlowTraceData **trace_data);
int glow_TraceInit( glow_tCtx ctx, 
	int (*trace_connect_func)( glow_tObject, GlowTraceData *),
	int (*trace_disconnect_func)( glow_tObject),
	int (*trace_scan_func)( glow_tObject, void *));
void glow_TraceClose( glow_tCtx ctx);
void glow_TraceScan( glow_tCtx ctx);
void glow_RemoveTraceObjects( glow_tCtx ctx);
void glow_Zoom( glow_tCtx ctx, double zoom_factor);
void glow_ZoomAbsolute( glow_tCtx ctx, double zoom_factor);
void glow_SetAttributes( glow_tCtx ctx, glow_sAttributes *attr,
	unsigned long mask);
void glow_PositionToPixel( glow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y);
void glow_UnZoom( glow_tCtx ctx);
void glow_CenterObject( glow_tCtx ctx, glow_tObject object);
void glow_GetNodePosition( glow_tNode node, double *x, double *y);
void glow_GetConPosition( glow_tCon con, double *x_arr[], double *y_arr[],
	int *num);
void glow_MeasureAnnotation( glow_tNodeClass node_class, int number, char *text, 
	double *width, double *height);
void glow_MeasureAnnotText( glow_tCtx ctx, char *text, glow_eDrawType draw_type,
		int text_size, glow_eAnnotType annot_type,
		double *width, double *height, int *rows);
glow_eNodeGroup glow_GetNodeGroup( glow_tNode node);
glow_eConGroup glow_GetConGroup( glow_tCon con);
void glow_DeleteNodeCons( glow_tNode node);
void glow_GetObjectName( glow_tObject object, char *name);
void glow_Reconfigure( glow_tCtx ctx);
void glow_SetNodraw( glow_tCtx ctx);
void glow_ResetNodraw( glow_tCtx ctx);
void glow_Redraw( glow_tCtx ctx);
int glow_FindByName( glow_tCtx ctx, char *name, glow_tObject *object);

#if defined __cplusplus
}
#endif
#endif
