#ifndef glow_growapi_h
#define glow_growapi_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef void *GrowCtx;
typedef struct {
	char		data[10][120];
	glow_eTraceType	attr_type;
	glow_eDrawType	color;
	glow_eDrawType	color2;
	void		*p;
	glow_eAccess	access;
	glow_eCycle	cycle;
        char            ref_object[120];
	} glow_sTraceData, GlowTraceData;
#ifndef glow_api_h
typedef void *GlowCtx;
#endif
#else
typedef GlowTraceData glow_sTraceData;
#endif


#include "glow.h"

typedef enum {
	grow_eInfoType__,
	grow_eInfoType_End,
	grow_eInfoType_Annot,
	grow_eInfoType_Dynamic,
	grow_eInfoType_Arg,
        grow_eInfoType_Image
	} grow_eInfoType;

typedef struct {
	char		name[80];
	void		*value_p;
	int		type;
	int		size;
	grow_eInfoType	info_type;
	int		number;
	int		multiline;
	int		no_edit;
	} grow_sAttrInfo;

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
	glow_eHotMode default_hot_mode;
        int     enable_bg_pixmap;
        int     double_buffer_on;
        glow_eHotMode hot_mode;
        glow_eDirection initial_position;
} grow_sAttributes;

typedef enum {
	grow_eAttr_base_zoom_factor 	= 1 << 0,
	grow_eAttr_offset_x		= 1 << 1,
	grow_eAttr_offset_y		= 1 << 2,
	grow_eAttr_grid_size_x		= 1 << 3,
	grow_eAttr_grid_size_y		= 1 << 4,
	grow_eAttr_grid_on		= 1 << 5,
	grow_eAttr_user_highlight	= 1 << 6,
	grow_eAttr_draw_delta		= 1 << 7,
	grow_eAttr_grafcet_con_delta	= 1 << 8,
	grow_eAttr_refcon_width		= 1 << 9,
	grow_eAttr_refcon_height	= 1 << 10,
	grow_eAttr_refcon_linewidth	= 1 << 11,
	grow_eAttr_refcon_textsize	= 1 << 12,
	grow_eAttr_application_paste	= 1 << 13,
	grow_eAttr_select_policy	= 1 << 14,
	grow_eAttr_display_level	= 1 << 15,
	grow_eAttr_default_hot_mode	= 1 << 16,
	grow_eAttr_enable_bg_pixmap     = 1 << 17,
	grow_eAttr_double_buffer_on     = 1 << 18,
	grow_eAttr_hot_mode             = 1 << 19,
	grow_eAttr_initial_position     = 1 << 20
	} grow_eAttribute;


typedef GrowCtx *grow_tCtx;
typedef void *grow_tNode;
typedef void *grow_tCon;
typedef void *grow_tObject;
typedef void *grow_tNodeClass;
typedef void *grow_tConClass;

int grow_Save( grow_tCtx ctx, char *filename);
int grow_Open( grow_tCtx ctx, char *filename);
int grow_SaveTrace( grow_tCtx ctx, char *filename);
int grow_OpenTrace( grow_tCtx ctx, char *filename);
void grow_DeleteObject( grow_tCtx ctx, grow_tObject object);
void grow_DeleteAll( grow_tCtx ctx);
void grow_DeleteNode( grow_tCtx ctx, grow_tNode node);
void grow_CloseNode( grow_tCtx ctx, grow_tNode node);
int grow_FindSelectedObject( grow_tCtx ctx, grow_tObject object);
void grow_ResetHighlightAll( grow_tCtx ctx);
void grow_ResetInverseAll( grow_tCtx ctx);
void grow_SetSelectHighlight( grow_tCtx ctx);
void grow_SetSelectInverse( grow_tCtx ctx);
void grow_ResetSelectHighlight( grow_tCtx ctx);
void grow_ResetSelectInverse( grow_tCtx ctx);
void grow_SelectInsert( grow_tCtx ctx, grow_tObject object);
void grow_SelectRemove( grow_tCtx ctx, grow_tObject object);
void grow_SelectClear( grow_tCtx ctx);
void grow_GetSelectedNodes( grow_tCtx ctx, grow_tNode **nodes, int *num);
void grow_GetSelectedCons( grow_tCtx ctx, grow_tCon **cons, int *num);
void grow_GetNodeClassList( grow_tCtx ctx, grow_tNodeClass **nodeclasses, 
	int *num);
void grow_GetNodeGroupList( grow_tCtx ctx, grow_tNodeClass **nodegroups, 
	int *num);
void grow_GetNodeClassObjectList( grow_tObject nodeclass, grow_tObject **objects, 
	int *num);
void grow_SetHighlight( grow_tObject object, int value);
void grow_GetHighlight( grow_tObject object, int *value);
void grow_SetInverse( grow_tObject object, int value);
void grow_CreateNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);
void grow_CreateGrowNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);
void grow_CreateGrowSlider( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);
void grow_CreateCon( grow_tCtx ctx, char *name, grow_tConClass cc,
	grow_tNode source, grow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, grow_tCon *con, 
	int point_num, double *x_vect, double *y_vect);
void grow_CreatePasteNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);
void grow_SetAnnotation( grow_tNode node, int number, char *text, int size);
void grow_GetAnnotation( grow_tNode node, int number, char *text, int size);
void grow_OpenAnnotationInput( grow_tNode node, int number);
int grow_AnnotationInputIsOpen( grow_tNode node, int number);
int grow_GetAnnotationInput( grow_tNode node, int number, char **text);
void grow_CloseAnnotationInput( grow_tNode node, int number);
void grow_SetPasteNodeAnnotation( grow_tNode node, int number, char *text, 
	int size);
void grow_EnableEvent( GrowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));
void grow_DisableEvent( GrowCtx *ctx, glow_eEvent event);
void grow_DisableEventAll( GrowCtx *ctx);
void grow_Cut( grow_tCtx ctx);
void grow_Copy( grow_tCtx ctx);
void grow_Paste( grow_tCtx ctx);
void grow_PasteClear( grow_tCtx ctx);
void grow_CreateRect( grow_tCtx ctx, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	grow_tObject *rect);
void grow_CreateLine( grow_tCtx ctx, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	grow_tObject *line);
void grow_CreateArc( grow_tCtx ctx, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width, grow_tObject *arc);
void grow_CreateText( grow_tCtx ctx, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size, grow_tObject *text);
void grow_CreatePixmap( grow_tCtx ctx, glow_sPixmapData *pixmap_data, 
	double x, double y, glow_eDrawType draw_type, int size, 
	grow_tObject *pixmap);
void grow_CreateAnnotPixmap( grow_tCtx ctx, int number, 
	double x, double y, glow_eDrawType draw_type, int size, int relative_pos,
	grow_tObject *annot_pixmap);
void grow_CreateAnnot( grow_tCtx ctx, double x, double y, int number, 
	glow_eDrawType draw_type, int text_size, grow_tObject *annot);
void grow_AddRect( grow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width);
void grow_AddFrame( grow_tNodeClass nc, double x, double y, 
	double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width);
void grow_AddLine( grow_tNodeClass nc, double x1, double y1, 
	double x2, double y2,
	glow_eDrawType draw_type, int line_width);
void grow_AddArc( grow_tNodeClass nc, double x1, double y1, 
	double x2, double y2, int angel1, int angel2,
	glow_eDrawType draw_type, int line_width);
void grow_AddText( grow_tNodeClass nc, char *text_str, double x, double y, 
	glow_eDrawType draw_type, int text_size);
void grow_AddAnnot( grow_tNodeClass nc, double x, double y, int number,
	glow_eDrawType draw_type, int text_size, glow_eAnnotType annot_type,
	int relative_pos);
void grow_AddAnnotPixmap( grow_tNodeClass nc, int number,
	double x, double y, glow_eDrawType draw_type, int size, int relative_pos);
void grow_AddRadiobutton( grow_tNodeClass nc, double x, double y, 
	double width, double height, int number,
	glow_eDrawType draw_type, int line_width);
void grow_CreatePushButton( grow_tCtx ctx, char *text, double x, double y, 
	double width, double height, grow_tObject *pushbutton);
void grow_CreateNodeClass( grow_tCtx ctx, char *name, glow_eNodeGroup group,
	grow_tNodeClass *nodeclass);
void grow_NodeClassAdd( grow_tNodeClass nc, grow_tObject object);
void grow_CreateConClass( grow_tCtx ctx, char *name,
	glow_eConType con_type, glow_eCorner corner, glow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, glow_eConGroup group, 
	grow_tConClass *conclass);
void grow_GetSelectList( grow_tCtx ctx, grow_tObject **list, int *cnt);
void grow_GetObjectList( grow_tCtx ctx, grow_tObject **list, int *cnt);
glow_eObjectType grow_GetObjectType( grow_tObject object);
void grow_GetMoveList( grow_tCtx ctx, grow_tObject **list, int *cnt);
void grow_SetObjectName( grow_tObject object, char *name);
void grow_MeasureNode( grow_tNode node, double *ll_x, double *ll_y,
	double *ur_x, double *ur_y);
void grow_Print( grow_tCtx ctx, char *filename);
void grow_GetUserData( grow_tObject object, void **user_data);
void grow_SetUserData( grow_tObject object, void *user_data);
void grow_GetCtxUserData( grow_tCtx ctx, void **user_data);
void grow_SetCtxUserData( grow_tCtx ctx, void *user_data);
grow_tCtx grow_GetCtx( grow_tObject object);
void grow_SetTraceAttr( grow_tObject object, GlowTraceData *trace_data);
void grow_GetTraceAttr( grow_tObject object, GlowTraceData **trace_data);
void grow_SetTraceData( grow_tObject object, void *trace_data);
int grow_TraceInit( grow_tCtx ctx, 
	int (*trace_connect_func)( grow_tObject, GlowTraceData *),
	int (*trace_disconnect_func)( grow_tObject),
	int (*trace_scan_func)( grow_tObject, void *));
void grow_TraceClose( grow_tCtx ctx);
void grow_TraceScan( grow_tCtx ctx);
void grow_RemoveTraceObjects( grow_tCtx ctx);
void grow_Zoom( grow_tCtx ctx, double zoom_factor);
void grow_ZoomX( grow_tCtx ctx, double zoom_factor);
void grow_ZoomY( grow_tCtx ctx, double zoom_factor);
void grow_ZoomAbsolute( grow_tCtx ctx, double zoom_factor);
void grow_GetZoom( grow_tCtx ctx, double *zoom_factor);
void grow_SetAttributes( grow_tCtx ctx, grow_sAttributes *attr,
	unsigned long mask);
void grow_PositionToPixel( grow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y);
void grow_UnZoom( grow_tCtx ctx);
void grow_CenterObject( grow_tCtx ctx, grow_tObject object);
void grow_GetNodePosition( grow_tNode node, double *x, double *y);
void grow_MeasureAnnotation( grow_tNodeClass node_class, int number, char *text, 
	double *width, double *height);
void grow_MeasureAnnotText( grow_tCtx ctx, char *text, glow_eDrawType draw_type,
		int text_size, glow_eAnnotType annot_type,
		double *width, double *height, int *rows);
glow_eNodeGroup grow_GetNodeGroup( grow_tNode node);
void grow_GetObjectName( grow_tObject object, char *name);
void grow_GetNodeClassName( grow_tNodeClass nodeclass, char *name);
void grow_Reconfigure( grow_tCtx ctx);
void grow_SetNodraw( grow_tCtx ctx);
void grow_ResetNodraw( grow_tCtx ctx);
void grow_Redraw( grow_tCtx ctx);
void grow_SetAnnotPixmap( grow_tNode node, int number, 
	glow_sAnnotPixmap *pixmap);
void grow_RemoveAnnotPixmap( grow_tNode node, int number);
void grow_AllocAnnotPixmap( grow_tCtx ctx, glow_sPixmapData *pixmap_data, 
		glow_sAnnotPixmap **pixmap);
void grow_FreeAnnotPixmap( grow_tCtx ctx, glow_sAnnotPixmap *pixmap);
void grow_SetRadiobutton( grow_tNode node, int number, int value);
void grow_GetRadiobutton( grow_tNode node, int number, int *value);
int grow_CreateSecondaryCtx( grow_tCtx ctx, grow_tCtx *secondary_ctx,
        int (*init_proc)(grow_tCtx ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type);
void grow_DeleteSecondaryCtx( grow_tCtx ctx);
int grow_ChangeCtx( Widget w, grow_tCtx from_ctx, grow_tCtx to_ctx);
void grow_SetInputFocus( grow_tCtx ctx);
void grow_SetMode( grow_tCtx ctx, grow_eMode mode);
grow_eMode grow_Mode( grow_tCtx ctx);
void grow_CreateGrowRect( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *rect);
void grow_CreateGrowBar( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *bar);
void grow_CreateGrowTrend( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *trend);
void grow_CreateGrowCurve( grow_tCtx ctx, char *name, glow_sCurveData *data,
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *curve);
void grow_CreateGrowLine( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	void *user_data,
	grow_tObject *line);
void grow_CreateGrowPolyLine( grow_tCtx ctx, char *name, 
	glow_sPoint *pointarray, int point_cnt,
	glow_eDrawType draw_type, int line_width, int fix_line_width,
	int fill, int border, glow_eDrawType fill_draw_type, 
	int closed, void *user_data,
	grow_tObject *polyline);
void grow_CreateGrowArc( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	int angel1, int angel2, glow_eDrawType draw_type, 
	int line_width, int fill_arc, int border, glow_eDrawType fill_draw_type,
	void *user_data,
	grow_tObject *arc);
void grow_CreateGrowConPoint( grow_tCtx ctx, char *name, 
	double x, double y, int cp_num, glow_eDirection d,
	void *user_data, grow_tObject *conpoint);
void grow_CreateGrowAnnot( grow_tCtx ctx, char *name, 
	double x, double y, int annot_num, glow_eDrawType d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	void *user_data, grow_tObject *annot);
void grow_CreateGrowText( grow_tCtx ctx, char *name, 
	char *text, double x, double y,
	glow_eDrawType draw_type, int t_size, 
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *text_object);
void grow_CreateGrowImage( grow_tCtx ctx, char *name, char *filename, 
	double x1, double y1, void *user_data, grow_tObject *image);
void grow_CreateGrowAxis( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int text_size,
        glow_eDrawType text_drawtype, void *user_data,
	grow_tObject *axis);
int grow_SaveSubGraph( grow_tCtx ctx, char *filename);
int grow_OpenSubGraph( grow_tCtx ctx, char *filename);
int grow_OpenSubGraphFromName( grow_tCtx ctx, char *name);
int grow_FindObjectByName( grow_tCtx ctx, char *name, grow_tObject *object);
int grow_FindNodeClassByName( grow_tCtx ctx, char *name, grow_tObject *object);
int grow_FindConClassByName( grow_tCtx ctx, char *name, grow_tObject *object);
int grow_GetNextConPointNumber( grow_tCtx ctx);
int grow_GetNextObjectNameNumber( grow_tCtx ctx);
int grow_GetObjectAttrInfo( grow_tObject object, char *transtab, 
	grow_sAttrInfo **info, int *attr_cnt);
int grow_GetSubGraphAttrInfo( grow_tCtx ctx, grow_sAttrInfo **info,
	int *attr_cnt);
int grow_GetGraphAttrInfo( grow_tCtx ctx, grow_sAttrInfo **info,
	int *attr_cnt);
void grow_FreeObjectAttrInfo( grow_sAttrInfo *attrinfo);
void grow_FreeSubGraphAttrInfo( grow_sAttrInfo *attrinfo);
void grow_FreeGraphAttrInfo( grow_sAttrInfo *attrinfo);
void grow_UpdateObject(  grow_tCtx ctx, grow_tObject object,
	grow_sAttrInfo *info);
void grow_UpdateSubGraph(  grow_tCtx ctx, grow_sAttrInfo *info);
void grow_UpdateGraph(  grow_tCtx ctx, grow_sAttrInfo *info);
int grow_CheckAnnotation( grow_tNodeClass nodeclass, int num);
void grow_New( grow_tCtx ctx);
void grow_SetName( grow_tCtx ctx, char *name);
void grow_GetName( grow_tCtx ctx, char *name);
void grow_PolylineEnd( grow_tCtx ctx);
void grow_AddPolyLinePoints( grow_tObject object, glow_sPoint *point_array, 
	int point_cnt);
void grow_SetObjectFill( grow_tObject object, int fill);
void grow_SetObjectDrawtype( grow_tObject object, glow_eDrawType drawtype);
void grow_SetObjectLinewidth( grow_tObject object, int linewidth);
void grow_ExecDynamic( grow_tCtx ctx);
void grow_SetObjectPosition( grow_tObject object, double x, double y);
void grow_SetObjectScale( grow_tObject object, double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type);
void grow_SetObjectScalePos( grow_tObject object, double x, double y, 
	double scale_x, double scale_y, 
        double x0, double y0, glow_eScaleType type);
void grow_SetObjectRotation( grow_tObject object, double angel, double x0,
	double y0, glow_eRotationPoint type);
void grow_SetObjectFillColor( grow_tObject object, glow_eDrawType drawtype);
void grow_ResetObjectFillColor( grow_tObject object);
void grow_SetObjectBorderColor( grow_tObject object, glow_eDrawType drawtype);
void grow_ResetObjectBorderColor( grow_tObject object);
void grow_SetSelectOrigBorderColor( grow_tCtx ctx, glow_eDrawType drawtype);
void grow_SetSelectOrigFillColor( grow_tCtx ctx, glow_eDrawType drawtype);
void grow_SetSelectOrigColorTone( grow_tCtx ctx, glow_eDrawTone tone);
void grow_SetSelectOrigColLightness( grow_tCtx ctx, int lightness);
void grow_IncrSelectOrigColLightness( grow_tCtx ctx, int lightness);
void grow_SetSelectOrigColIntensity( grow_tCtx ctx, int intensity);
void grow_IncrSelectOrigColIntensity( grow_tCtx ctx, int intensity);
void grow_SetSelectOrigColorShift( grow_tCtx ctx, int shift);
void grow_IncrSelectOrigColorShift( grow_tCtx ctx, int shift);
void grow_SetObjectColorTone( grow_tObject object, glow_eDrawTone tone);
void grow_ResetObjectColorTone( grow_tObject object);
void grow_SetObjectColorLightness( grow_tObject object, int lightness);
void grow_ResetObjectColorLightness( grow_tObject object);
void grow_SetObjectColorIntensity( grow_tObject object, int intensity);
void grow_ResetObjectColorIntensity( grow_tObject object);
void grow_SetObjectColorShift( grow_tObject object, int shift);
void grow_IncrObjectColorShift( grow_tObject object, int shift);
void grow_ResetObjectColorShift( grow_tObject object);
void grow_SetObjectColorInverse( grow_tObject object, int inverse);
void grow_SetObjectVisibility( grow_tObject object, int visible);
void grow_RotateSelectedObjects( grow_tCtx ctx, double angel, 
	glow_eRotationPoint type);
void grow_SetBackgroundColor( grow_tCtx ctx, glow_eDrawType color);
void grow_GetBackgroundColor( grow_tCtx ctx, glow_eDrawType *color);
void grow_SetBackgroundImage( grow_tCtx ctx, char *image);
void grow_GetBackgroundImage( grow_tCtx ctx, char *image, int *tiled);
int grow_GetBackgroundImageSize( grow_tCtx ctx, int *width, int *height);
void grow_ResetBackgroundColor( grow_tCtx ctx);
void grow_DisableHighlight( grow_tCtx ctx);
void grow_EnableHighlight( grow_tCtx ctx);
void grow_RefSubGraphArgNames( grow_tCtx ctx, char **argnames, int **argtypes,
	int **arg_cnt);
void grow_GetSubGraphDynamic( grow_tCtx ctx, char **dynamic, int *size);
void grow_GetObjectArgs( grow_tObject object, char **argnames, int **argtypes,
	char ***argvalues, int *arg_cnt);
void grow_PushSelectedObjects( grow_tCtx ctx);
void grow_PopSelectedObjects( grow_tCtx ctx);
void grow_SetDefaultLayout( grow_tCtx ctx);
void grow_SetLayout( grow_tCtx ctx, double x0, double y0, double x1,
		     double y1);
void grow_SetPath( grow_tCtx ctx, int path_cnt, char *path);
void grow_GetObjectClassTraceType( grow_tObject object, glow_eTraceType *type);
void grow_GetObjectClassCycle( grow_tObject object, glow_eCycle *cycle);
void grow_GetNodeClassTraceType( grow_tNodeClass nodeclass, glow_eTraceType *type);
void grow_GetSubGraphTraceType( grow_tCtx ctx, glow_eTraceType *type);
void grow_GetSubGraphTraceColor( grow_tCtx ctx, glow_eDrawType *color,
	glow_eDrawType *color2);
void grow_GetObjectClassTraceColor( grow_tObject object, glow_eDrawType *color,
	glow_eDrawType *color2);
void grow_SetSelectLineWidth( grow_tCtx ctx, int linewidth);
void grow_SetSelectFill( grow_tCtx ctx, int fill);
void grow_SetSelectBorder( grow_tCtx ctx, int border);
int grow_AnySelectIsCon( grow_tCtx ctx);
void grow_ChangeSelectConClass( grow_tCtx ctx, grow_tConClass conclass);
void grow_GetConAttributes( grow_tCon con, glow_eDrawType *drawtype,
	int *linewidth, glow_eConType *contype, glow_eCorner *corner, 
	double *round_corner_amount);
void grow_ChangeConConClass( grow_tCon con, grow_tConClass conclass);
int grow_FindObject( grow_tCtx ctx, grow_tObject object);
void grow_SetObjectText( grow_tObject object, char *text);
void grow_GetObjectText( grow_tObject object, char *text);
void grow_SetSelectTextSize( grow_tCtx ctx, int size);
void grow_SetSelectTextBold( grow_tCtx ctx, int bold);
void grow_SetBarValue( grow_tObject object, double value);
void grow_AddTrendValue( grow_tObject object, double value, int idx);
int grow_IsSliderClass( grow_tNodeClass nodeclass);
void grow_SetMoveRestrictions( grow_tCtx ctx, glow_eMoveRestriction restriction,
	double max_limit, double min_limit, grow_tObject object);
void grow_SetScaleEqual( grow_tCtx ctx, int equal);
void grow_GetSliderInfo( grow_tObject object, glow_eDirection *direction,
	double *max_value, double *min_value, double *max_pos, double *min_pos);
void grow_GetSliderOrigo( grow_tObject object, glow_eDirection direction,
			  double *pos);
void grow_SetSliderInfo( grow_tObject object, glow_eDirection direction,
	double max_value, double min_value, double max_pos, double min_pos);
void grow_ObjectToCtx( grow_tObject object, grow_tCtx *ctx);
void grow_StoreTransform( grow_tObject object);
void grow_AlignSelect( grow_tCtx ctx, glow_eAlignDirection direction);
void grow_EquiDistanceSelect( grow_tCtx ctx, glow_eAlignDirection direction);
int grow_IsSubGraph( grow_tCtx ctx);
void grow_SetSliderRange( grow_tObject object, double min, double max);
void grow_SetBarRange( grow_tObject object, double min, double max);
void grow_SetTrendFillCurve( grow_tObject object, int fill);
void grow_SetTrendRange( grow_tObject object, int curve, 
	double min, double max);
void grow_GetTrendScanTime( grow_tObject object, double *time);
void grow_SetTrendScanTime( grow_tObject object, double time);
void grow_SetTrendLines( grow_tObject object, int vert_lines, 
	int horiz_lines);
void grow_SetAxisConf( grow_tObject object, double max_val, double min_val, 
	int no_of_lines, int long_quot, int value_quot, double rot, 
        char *format);
void grow_SetModified( grow_tCtx ctx, int modified);
int grow_GetModified( grow_tCtx ctx);
void grow_MeasureJavaBean( grow_tCtx ctx, double *x_right, double *x_left,
	double *y_high, double *y_low);
void grow_ToPixel( grow_tCtx ctx, double x, double y,
		   double *x_pix, double *y_pix);
void grow_SetJavaFrame( grow_tCtx ctx, double *x_right, double *x_left,
	double *y_high, double *y_low);
void grow_MeasureNodeClassJavaBean( grow_tNodeClass nodeclass, double *x_right, double *x_left,
	double *y_high, double *y_low);
void grow_ExportJavaBean( grow_tCtx ctx, ofstream& fp, int components);
void grow_ExportNodeClassJavaBean( grow_tCtx ctx, grow_tNodeClass nc, 
	ofstream& fp, int components);
void grow_ExportNcJavaBeanFont( grow_tCtx ctx, grow_tNodeClass nc, 
	ofstream& fp, int components);
void grow_SetJavaName( grow_tCtx ctx, char *name);
int grow_GetJavaName( grow_tCtx ctx, char *name);
int grow_IsJavaApplet( grow_tCtx ctx);
int grow_IsJavaApplication( grow_tCtx ctx);
int grow_GetObjectClassJavaName( grow_tObject object, char *name);
int grow_GetNodeClassJavaName( grow_tNodeClass nodeclass, char *name);
void grow_GetObjectAnnotationNumbers( grow_tObject object, int **numbers,
	int *cnt);
void grow_GetSubgraphAnnotNumbers( grow_tCtx ctx, int **numbers,
	int *cnt);
int grow_IsSlider( grow_tCtx ctx);
char *grow_ColorToName( glow_eDrawType drawtype);
char *grow_ColorToneToName( glow_eDrawType drawtype);
int grow_SetObjectNextNodeClass( grow_tObject object);
int grow_SetObjectPrevNodeClass( grow_tObject object);
void grow_SetObjectFirstNodeClass( grow_tObject object);
void grow_SetObjectLastNodeClass( grow_tObject object);
int grow_SetObjectNodeClassByIndex( grow_tObject object, int idx);
void grow_GetObjectAnimationCount( grow_tObject object, int *count);
void grow_GetScanTime( grow_tCtx ctx, double *scantime, double *fast_scantime, 
	double *animation_scantime);
void grow_SetShowGrid( grow_tCtx ctx, int show);
int grow_IsNextNodeClass( grow_tNodeClass nodeclass);
int grow_GetNodeClassPages( grow_tNodeClass nodeclass);
void grow_GetNextSubgraph( grow_tCtx ctx, char *next);
void grow_SetNextSubgraph( grow_tCtx ctx, char *next);
void grow_StoreGeometry( grow_tCtx ctx);
void grow_RestoreGeometry( grow_tCtx ctx);
void grow_SetClickSensitivity( grow_tCtx ctx, int value);
void grow_MoveNode( grow_tNode node, double x, double y);
void grow_GetTextExtent( grow_tCtx ctx, char *text, int len, 
	glow_eDrawType draw_type, int text_size, double *width,
	double *height, double *descent);
int grow_GetDefaultWindowSize( grow_tCtx ctx, int *width, int *height);
int grow_GroupSelect( grow_tCtx ctx, grow_tObject *group, char *last_group_name);
int grow_UngroupSelect( grow_tCtx ctx);
void grow_CurveConfigure( grow_tObject object, glow_sCurveData *data);
void grow_CurveAddPoints( grow_tObject object, glow_sCurveData *data);
int grow_GetObjectGroup( grow_tCtx ctx, grow_tObject object, 
			 grow_tObject *group);
void grow_GetGroupObjectList( grow_tObject group, grow_tObject **list, 
			      int *cnt);
void grow_SetNodeClassExtern( grow_tNodeClass nodeclass, int ext);
int grow_GetBackgroundObjectLimits( grow_tCtx ctx, glow_eTraceType type,
	double x, double y, grow_tObject *background,
	double *min, double *max, glow_eDirection *direction);
int grow_GetTranslate( grow_tCtx ctx);
glow_eMB3Action grow_GetMB3Action( grow_tCtx ctx);
void grow_GetNodeClassOrigo( grow_tObject object, double *x, double *y);

#if defined __cplusplus
}
#endif
#endif
