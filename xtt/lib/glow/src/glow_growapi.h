/* 
 * Proview   $Id: glow_growapi.h,v 1.22 2007-01-04 07:57:38 claes Exp $
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

#ifndef glow_growapi_h
#define glow_growapi_h

/*! \file glow_growapi.h
    \brief Contains c API for grow. */
/*! \addtogroup GrowApi */
/*@{*/


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

#if defined __cplusplus
extern "C" {
#endif



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
    int			type;
    int			size;
    grow_eInfoType	info_type;
    int			number;
    int			multiline;
    int			no_edit;
    double		maxlimit;
    double		minlimit;
  } grow_sAttrInfo;

  //! Structs to set attributes for the grow context.
  typedef struct {
    double	base_zoom_factor;
    int		offset_x;
    int		offset_y;
    double	grid_size_x;
    double	grid_size_y;
    int		grid_on;
    int		user_highlight;
    double	draw_delta;
    double	grafcet_con_delta;
    double	refcon_width;
    double	refcon_height;
    int		refcon_linewidth;
    int		refcon_textsize;
    int		application_paste;
    glow_eSelectPolicy select_policy;
    int		display_level;
    glow_eHotMode default_hot_mode;
    int     	enable_bg_pixmap;
    int     	double_buffer_on;
    glow_eHotMode hot_mode;
    glow_eDirection initial_position;
  } grow_sAttributes;

  typedef enum {
    grow_eAttr_base_zoom_factor        	= 1 << 0,
    grow_eAttr_offset_x			= 1 << 1,
    grow_eAttr_offset_y			= 1 << 2,
    grow_eAttr_grid_size_x		= 1 << 3,
    grow_eAttr_grid_size_y		= 1 << 4,
    grow_eAttr_grid_on			= 1 << 5,
    grow_eAttr_user_highlight		= 1 << 6,
    grow_eAttr_draw_delta	       	= 1 << 7,
    grow_eAttr_grafcet_con_delta       	= 1 << 8,
    grow_eAttr_refcon_width		= 1 << 9,
    grow_eAttr_refcon_height		= 1 << 10,
    grow_eAttr_refcon_linewidth		= 1 << 11,
    grow_eAttr_refcon_textsize		= 1 << 12,
    grow_eAttr_application_paste       	= 1 << 13,
    grow_eAttr_select_policy		= 1 << 14,
    grow_eAttr_display_level		= 1 << 15,
    grow_eAttr_default_hot_mode		= 1 << 16,
    grow_eAttr_enable_bg_pixmap     	= 1 << 17,
    grow_eAttr_double_buffer_on     	= 1 << 18,
    grow_eAttr_hot_mode             	= 1 << 19,
    grow_eAttr_initial_position     	= 1 << 20
  } grow_eAttribute;


  typedef GrowCtx *grow_tCtx;
  typedef void *grow_tNode;
  typedef void *grow_tCon;
  typedef void *grow_tObject;
  typedef void *grow_tNodeClass;
  typedef void *grow_tConClass;
  
  //! Save grow to file.
  /*!
    \param ctx		Grow context.
    \param filename	Name of file to save context in.
  */
  int grow_Save( grow_tCtx ctx, char *filename);

  //! Open grow from file.
  /*!
    \param ctx		Grow context.
    \param filename	Name of file to open context from.
  */
  int grow_Open( grow_tCtx ctx, char *filename);

  int grow_SaveTrace( grow_tCtx ctx, char *filename);
  int grow_OpenTrace( grow_tCtx ctx, char *filename);

  //! Delete an object.
  /*!
    \param ctx		Grow context.
    \param object	Object to delete.
  */
  void grow_DeleteObject( grow_tCtx ctx, grow_tObject object);

  //! Delete all objects.
  /*! \param ctx		Grow context. */
  void grow_DeleteAll( grow_tCtx ctx);

  void grow_DeleteNode( grow_tCtx ctx, grow_tNode node);
  void grow_CloseNode( grow_tCtx ctx, grow_tNode node);

  //! Check if an object is selected.
  /*!
    \param ctx		Grow context.
    \param object	Object to check.
    \return		Returns 1 if object is found in the selectlist, else 0.
  */
  int grow_FindSelectedObject( grow_tCtx ctx, grow_tObject object);

  //! Reset highlight on all objects.
  /*! \param ctx		Grow context. */
  void grow_ResetHighlightAll( grow_tCtx ctx);

  void grow_ResetInverseAll( grow_tCtx ctx);

  //! Set highlight on all selected objects.
  /*! \param ctx		Grow context. */
  void grow_SetSelectHighlight( grow_tCtx ctx);

  void grow_SetSelectInverse( grow_tCtx ctx);

  //! Reset highlight on all selected objects.
  /*! \param ctx		Grow context. */
  void grow_ResetSelectHighlight( grow_tCtx ctx);

  void grow_ResetSelectInverse( grow_tCtx ctx);

  //! Insert an object in select list.
  /*! 
    \param ctx		Grow context. 
    \param object	Object to insert.
  */
  void grow_SelectInsert( grow_tCtx ctx, grow_tObject object);

  //! Remove an object from selectlist.
  /*! 
    \param ctx		Grow context. 
    \param object	Object to remove.
  */
  void grow_SelectRemove( grow_tCtx ctx, grow_tObject object);

  //! Clear the select list.
  /*! \param ctx		Grow context. */
  void grow_SelectClear( grow_tCtx ctx);

  //! Get list of selected nodes, i.e. object of type GlowNode.
  /*!
    \param ctx		Grow context.
    \param nodes       	List of selected nodes.
    \param num		Number of selected nodes.

    The list should be freed by the caller.
  */
  void grow_GetSelectedNodes( grow_tCtx ctx, grow_tNode **nodes, int *num);

  //! Get list of selected connections.
  /*!
    \param ctx		Grow context.
    \param cons		List of selected connections.
    \param num		Number of selected connections.

    The list should be freed by the caller.
  */
  void grow_GetSelectedCons( grow_tCtx ctx, grow_tCon **cons, int *num);

  //! Get list of nodeclasses.
  /*!
    \param ctx		Grow context.
    \param nodeclasses 	Pointer to list of nodeclasse.
    \param num		Number of nodeclasses in list.
  */
  void grow_GetNodeClassList( grow_tCtx ctx, grow_tNodeClass **nodeclasses, 
	int *num);

  //! Get list of groups
  /*!
    \param ctx		Grow context.
    \param nodegroups  	Pointer to list of groups.
    \param num		Number of groups in list.

    The list should be freed by the caller.
  */
  void grow_GetNodeGroupList( grow_tCtx ctx, grow_tNodeClass **nodegroups, 
	int *num);

  //! Get the list of objects in a nodeclass.
  /*!
    \param nodeclass	Nodeclass.
    \param objects     	Pointer to list of objects.
    \param num		Number of objects in list.
  */
  void grow_GetNodeClassObjectList( grow_tObject nodeclass, grow_tObject **objects, 
	int *num);

  //! Set or reset highligt on an object.
  /*!
    \param object	Object to change highlight status on.
    \param value	1 set hightlight, 0 reset highlight.
  */
  void grow_SetHighlight( grow_tObject object, int value);

  //! Get highlight of in object.
  /*!
    \param object	Object to get highlight of.
    \param value	1 object is highlighted, 0 not highlighted.
  */
  void grow_GetHighlight( grow_tObject object, int *value);

  void grow_SetInverse( grow_tObject object, int value);

  void grow_CreateNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);

  //! Create a GrowNode object.
  /*!
    \param ctx		Grow context.
    \param name		Name of object.
    \param nc		Nodeclass for the object.
    \param x		x coordinate for position of object.
    \param y		y coordinate for position of object.
    \param user_data	User data.
    \param node		Created GrowNode object.
  */
  void grow_CreateGrowNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);

  //! Create a GrowSlider object.
  /*!
    \param ctx		Grow context.
    \param name		Name of object.
    \param nc		Nodeclass for the object.
    \param x		x coordinate for position of object.
    \param y		y coordinate for position of object.
    \param user_data	User data.
    \param node		Created GrowSlider object.
  */
  void grow_CreateGrowSlider( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);

  //! Create a connection object.
  /*!
    \param ctx		Grow context.
    \param name		Name of object.
    \param cc		Connections class for the object.
    \param source	Source node.
    \param dest		Destination node.
    \param source_conpoint Connection point number of source.
    \param dest_conpoint Coonection point number of destination.
    \param user_data	User data.
    \param con		Created GlowCon object.
    \param point_num	Number of points in connection route.
    \param x_vect	Array with x coordinates for connection route. Can be zero.
    \param y_vect	Array with y coordinates for connection route. Can be zero.
    \param border	The connection is draw with border lines.
    \param shadow	The connection is draw with shadow.

    Create a connection between two GrowNode objects. The connection is drawn between
    the specified connections point in each node. If the the connectionclass is of type
    routed connection, x_vect and y_vect can contain points for the route. If x_vect and
    y_vect are zero, a route will be calculated for the connection.
  */
  void grow_CreateCon( grow_tCtx ctx, char *name, grow_tConClass cc,
	grow_tNode source, grow_tNode dest, int source_conpoint, 
	int dest_conpoint, void *user_data, grow_tCon *con, 
	int point_num, double *x_vect, double *y_vect, int border, int shadow);

  //! Create a GrowNode object and put it into the paste list.
  /*!
    \param ctx		Grow context.
    \param name		Name of object.
    \param nc		Nodeclass for the object.
    \param x		x coordinate for position of object.
    \param y		y coordinate for position of object.
    \param user_data	User data.
    \param node		Created GrowNode object.
  */
  void grow_CreatePasteNode( grow_tCtx ctx, char *name, grow_tNodeClass nc,
	double x, double y, void *user_data, grow_tNode *node);

  //! Set or reset input focus to a node or group.
  /*!
    \param node		Node or group to set focus to.
    \param focus	if 1 input focus is set, else focus is reset. */
  void grow_SetObjectInputFocus( grow_tNode node, int focus);

  //! Set the text of an annotation in a GrowNode object.
  /*!
    \param node		Node object.
    \param number	Annotation number.
    \param text		Text to set in annotation.
    \param size		Length of text.
  */
  void grow_SetAnnotation( grow_tNode node, int number, char *text, int size);

  //! Set the text of an annotation in a GrowNode object. Just redraw the background of the annotation.
  /*!
    \param node		Node object.
    \param number	Annotation number.
    \param text		Text to set in annotation.
    \param size		Length of text.
  */
  void grow_SetAnnotationBrief( grow_tNode node, int number, char *text, int size);

  //! Get the text of an annotation in a GrowNode object.
  /*!
    \param node		Node object.
    \param number	Annotation number.
    \param text		Buffer to copy the text into.
    \param size		Size of buffer.
  */
  void grow_GetAnnotation( grow_tNode node, int number, char *text, int size);

  //! Open an annotation for input.
  /*!
    \param node		Node object.
    \param number	Annotation number.
  */
  void grow_OpenAnnotationInput( grow_tNode node, int number);

  //! Set text selection of annotation.
  /*!
    \param node		Node object.
    \param selection	1 selected, 0 not selected.
  */
  void grow_SetAnnotationSelection( grow_tNode node, int selection);

  //! Check if an annotation is open for input.
  /*!
    \param node		Node object.
    \param number      	Annotation number.
    \return		1 annotation is open, 0 annotation is not open.
  */
  int grow_AnnotationInputIsOpen( grow_tNode node, int number);

  //! Close the annotation for input.
  /*!
    \param node		Node object.
    \param number	Annotation number.
  */
  void grow_CloseAnnotationInput( grow_tNode node, int number);

  //! Close annotation input of all nodes.
  /*! \param ctx	Grow context. */
  void grow_CloseAnnotationInputAll( GrowCtx *ctx);

  //! Set the text of on annotation of an GrowNode object in the paste list.
  /*!
    \param node		Node object.
    \param number	Annotation number.
    \param text		Text to set in annotation.
    \param size		Length of text.
  */
  void grow_SetPasteNodeAnnotation( grow_tNode node, int number, char *text, 
				    int size);

  //! Enable an event, an register a callback function for the event.
  /*!
    \param ctx		Grow context.
    \param event	Event.
    \param event_type	Type of event.
    \param event_cb	Callback function for the event.
  */
  void grow_EnableEvent( GrowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));

  //! Disable an event.
  /*!
    \param ctx		Grow context.
    \param event	Event to disable.
  */
  void grow_DisableEvent( GrowCtx *ctx, glow_eEvent event);

  //! Disable all event.
  /*! \param ctx		Grow context. */
  void grow_DisableEventAll( GrowCtx *ctx);

  //! Cut the selected object and put them in the paste list.
  /*! \param ctx		Grow context. */
  void grow_Cut( grow_tCtx ctx);

  //! Copy the selected object, and put the copies in the paste list.
  /*! \param ctx		Grow context. */
  void grow_Copy( grow_tCtx ctx);

  //! Paste the objects in the paste list into the work area.
  /*! \param ctx		Grow context. */
  void grow_Paste( grow_tCtx ctx);

  //! Clear the paste list.
  /*! \param ctx		Grow context. */
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
  void grow_AddRect( grow_tNodeClass nc, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data);
  void grow_AddLine( grow_tNodeClass nc, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	void *user_data);
  void grow_AddPolyLine( grow_tNodeClass, char *name, 
	glow_sPoint *pointarray, int point_cnt,
	glow_eDrawType draw_type, int line_width, int fix_line_width,
	int fill, int border, int shadow, glow_eDrawType fill_draw_type, 
	int closed, void *user_data);
  void grow_AddArc( grow_tNodeClass nc, char *name, 
	double x1, double y1, double x2, double y2,
	int angel1, int angel2, glow_eDrawType draw_type, 
	int line_width, int fill_arc, int border, int shadow, glow_eDrawType fill_draw_type,
	void *user_data);
  void grow_AddText( grow_tNodeClass nc, char *name, 
	char *text, double x, double y,
	glow_eDrawType draw_type, glow_eDrawType color, int t_size, 
	glow_mDisplayLevel display_level, void *user_data);
  void grow_AddAnnot( grow_tNodeClass nc, 
	double x, double y, int annot_num, glow_eDrawType d_type, glow_eDrawType color_d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	void *user_data);
  void grow_CreateNodeClass( grow_tCtx ctx, char *name, glow_eNodeGroup group,
	grow_tNodeClass *nodeclass);
  void grow_NodeClassAdd( grow_tNodeClass nc, grow_tObject object);

  //! Create a connection class.
  /*!
    \param ctx		Grow context.
    \param name		Name of connection class.
    \param con_type	Type of connection.
    \param corner       Type of corner, rounded or straight.
    \param line_type	Color.
    \param line_width	Line width.
    \param arrow_width	Width of arrow.
    \param arrow_length Length of arrow.
    \param round_corner_amount The size of the arcs in rounded corners.
    \param group	Group a connection is a memeber of.
    \param conclass	The created connection class.
  */
  void grow_CreateConClass( grow_tCtx ctx, char *name,
	glow_eConType con_type, glow_eCorner corner, glow_eDrawType line_type,
	int line_width, double arrow_width, double arrow_length, 
	double round_corner_amount, glow_eConGroup group, 
	grow_tConClass *conclass);

  //! Get list of selected objects.
  /*!
    \param ctx		Grow context.
    \param list		List of selected objects.
    \param cnt		Number of objects in list.
  */
  void grow_GetSelectList( grow_tCtx ctx, grow_tObject **list, int *cnt);

  //! Get list of all objects in the grow context.
  /*!
    \param ctx		Grow context.
    \param list		List of objects.
    \param cnt		Number of objects in list.
  */
  void grow_GetObjectList( grow_tCtx ctx, grow_tObject **list, int *cnt);

  //! Get the type of an object.
  /*!
    \param object	Object to get the type of.
    \return 		The type of the object.
  */
  glow_eObjectType grow_GetObjectType( grow_tObject object);

  //! Get the list object that i currently moved.
  /*!
    \param ctx		Grow context.
    \param list		List of moved objects.
    \param cnt		Number of objects in list.
  */
  void grow_GetMoveList( grow_tCtx ctx, grow_tObject **list, int *cnt);

  //! Set the name of an object.
  /*!
    \param object	Object to change the name for.
    \param name		Name to set.
  */
  void grow_SetObjectName( grow_tObject object, char *name);

  //! Measure a GrowNode object.
  /*!
    \param node		GrowNode object.
    \param ll_x		x coordinate for lower left corner of node.
    \param ll_y		y coordinate for lower left corner of node.
    \param ur_x		x coordinate for upper right corner of node.
    \param ur_y		y coordinate for upper right corner of node.
  */
  void grow_MeasureNode( grow_tNode node, double *ll_x, double *ll_y,
			 double *ur_x, double *ur_y);

  //! Print context to postscript file.
  void grow_Print( grow_tCtx ctx, char *filename, double x0, double x1, int end);

  //! Get user data of an object.
  /*!
    \param object	Object.
    \param user_data	The user stored in the object.
  */
  void grow_GetUserData( grow_tObject object, void **user_data);

  //! Set user data into an object.
  /*!
    \param object	Object.
    \param user_data	Userdata to store in the object.
  */
  void grow_SetUserData( grow_tObject object, void *user_data);

  //! Get user data of the context.
  /*!
    \param ctx		Grow context.
    \param user_data	User data stored in the context.
  */
  void grow_GetCtxUserData( grow_tCtx ctx, void **user_data);

  //! Set user data of the context.
  /*!
    \param ctx		Grow context.
    \param user_data	User data to store in the context.
  */
  void grow_SetCtxUserData( grow_tCtx ctx, void *user_data);

  //! Get grow contect from an object.
  /*!
    \param object	An object.
    \return 		Grow context.
  */
  grow_tCtx grow_GetCtx( grow_tObject object);

  void grow_SetTraceAttr( grow_tObject object, GlowTraceData *trace_data);
  void grow_GetTraceAttr( grow_tObject object, GlowTraceData **trace_data);
  void grow_SetTraceData( grow_tObject object, void *trace_data);

  //! Start trace and register backcall functions.
  /*!
    \param ctx 			Grow context.
    \param trace_connect_func	Backcall function for trace connects of an object.
    \param trace_disconnect_func Backcall function for trace disconnects of an object.
    \param trace_scan_func	Backcall function for trace scan of on object.
  */
  int grow_TraceInit( grow_tCtx ctx, 
	int (*trace_connect_func)( grow_tObject, GlowTraceData *),
	int (*trace_disconnect_func)( grow_tObject),
	int (*trace_scan_func)( grow_tObject, void *));

  //! Close trace.
  /*! \param ctx	Grow context. */
  void grow_TraceClose( grow_tCtx ctx);

  //! Scan trace.
  /*! \param ctx	Grow context. */
  void grow_TraceScan( grow_tCtx ctx);

  void grow_RemoveTraceObjects( grow_tCtx ctx);

  //! Zoom with a specified zoom factor.
  /*! 
    \param ctx		Grow context. 
    \param zoom_factor	Zoom factor.
  */
  void grow_Zoom( grow_tCtx ctx, double zoom_factor);

  //! Zoom in x direction with a specified zoom factor.
  /*! 
    \param ctx		Grow context. 
    \param zoom_factor	Zoom factor in x direction.
  */
  void grow_ZoomX( grow_tCtx ctx, double zoom_factor);

  //! Zoom in y direction with a specified zoom factor.
  /*! 
    \param ctx		Grow context. 
    \param zoom_factor	Zoom factor in y direction.
  */
  void grow_ZoomY( grow_tCtx ctx, double zoom_factor);

  //! Zoom to an absolute zoom factor.
  /*! 
    \param ctx		Grow context. 
    \param zoom_factor	Absolute zoom factor.
  */
  void grow_ZoomAbsolute( grow_tCtx ctx, double zoom_factor);

  //! Get the current zoom factor.
  /*! 
    \param ctx		Grow context. 
    \param zoom_factor	Zoom factor.
  */
  void grow_GetZoom( grow_tCtx ctx, double *zoom_factor);

  //! Set attributes of the grow context.
  /*! 
    \param ctx		Grow context.
    \param attr	        Data structure with attribute values.
    \param mask		Mask of the attributes to set.
  */
  void grow_SetAttributes( grow_tCtx ctx, grow_sAttributes *attr,
	unsigned long mask);

  //! Get attributes of the grow context.
  /*! 
    \param ctx		Grow context.
    \param attr	        Data structure with attribute values.
    \param mask		Mask of the attributes to get.
  */
  void grow_GetAttributes( grow_tCtx ctx, grow_sAttributes *attr,
	unsigned long mask);

  //! Convert coordinates for a point to pixel.
  /*!
    \param ctx		Grow context.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param pix_x       	x coordinate in pixel.
    \param pix_y       	y coordinate in pixel.
  */
  void grow_PositionToPixel( grow_tCtx ctx, double x, double y, 
		int *pix_x, int *pix_y);

  //! Unzoom. Reset to original zoom factor.
  /*! \param ctx  	Grow context. */
  void grow_UnZoom( grow_tCtx ctx);

  //! Position the view so that the specified object is in the center of the window.
  /*!
    \param ctx		Grow context.
    \param object	Object to center.
  */
  void grow_CenterObject( grow_tCtx ctx, grow_tObject object);

  void grow_GetNodePosition( grow_tNode node, double *x, double *y);

  //! Measure the size a text would have in an annotation with specified attributes.
  /*!
    \param ctx		Grow context.
    \param text		Text to measure.
    \param draw_type 	Text drawtype.
    \param text_size	Text size.
    \param annot_type	Type of annotation.
    \param width	Width of text.
    \param height	Height of text.
    \param rows		Number of rows in text.
  */
  void grow_MeasureAnnotText( grow_tCtx ctx, char *text, glow_eDrawType draw_type,
		int text_size, glow_eAnnotType annot_type,
		double *width, double *height, int *rows);

  //! Get the group of a node.
  /*!
    \param node		Node object.
    \return		The group of the node.
  */
  glow_eNodeGroup grow_GetNodeGroup( grow_tNode node);

  //! Get the name of an object.
  /*!
    \param object	Object.
    \param name		The name of the object.
  */
  void grow_GetObjectName( grow_tObject object, char *name);

  //! Get the name of a nodeclass.
  /*!
    \param nodeclass	Nodeclass.
    \param name		The name of the nodeclass.
  */ 
  void grow_GetNodeClassName( grow_tNodeClass nodeclass, char *name);

  void grow_Reconfigure( grow_tCtx ctx);

  //! Set no draw
  /*!
    \param ctx		Grow context.

    Nothing will be drawn in the window until grow_ResetNoDraw is called.
    After grow_ResetNoDraw the function grow_Redraw should be called to update the window.
  */
  void grow_SetNodraw( grow_tCtx ctx);

  //! Reset no draw.
  /*! \param ctx	Grow context. */
  void grow_ResetNodraw( grow_tCtx ctx);

  //! Set defered redraw.
  /*! \param ctx	Grow context.
    The redraw will be deferd until a call to redraw_defered() is made.
  */
  void	grow_SetDeferedRedraw( grow_tCtx ctx);

  //! Execute the defered redrawings.
  /*! \param ctx	Grow context.
    Execute redrawing the defered redrawing area since the call to set_defered_redraw().
  */
  void	grow_RedrawDefered( grow_tCtx ctx);


  //! Redraw the window.
  /*! \param ctx	Grow context. */
  void grow_Redraw( grow_tCtx ctx);

  void grow_SetInputFocus( grow_tCtx ctx);

  //! Set editing mode of grow.
  /*!
    \param ctx		Grow context.
    \param mode		Editing mode.
  */
  void grow_SetMode( grow_tCtx ctx, grow_eMode mode);

  //! Get grow edit mode.
  /*!
    \param ctx		Grow context.
    \return		Edit mode.
  */
  grow_eMode grow_Mode( grow_tCtx ctx);

  //! Create a rectangle, i.e an object of class GrowRect.
  /*!
    \param ctx	       Grow context.
    \param name	       Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param fix_line_width Line width is independent of scale.
    \param display_level Display levels when the object is visible.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param shadow	Shadow is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param rect		Created GrowRect object.
  */
  void grow_CreateGrowRect( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *rect);

  //! Create a rectangle with rounded corners, i.e an object of class GrowRectRounded.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param fix_line_width Line width is independent of scale.
    \param display_level Display levels when the object is visible.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param shadow	Shadow is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param rect		Created GrowRectRounded object.
  */
  void grow_CreateGrowRectRounded( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, int shadow, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *rect);

  //! Create a bar object, i.e an object of class GrowBar.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param display_level Display levels when the object is visible.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param bar		Created GrowBar object.
  */
  void grow_CreateGrowBar( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *bar);

  //! Create a trend object, i.e an object of class GrowTrend.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param display_level Display levels when the object is visible.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param trend       	Created GrowTrend object.
  */
  void grow_CreateGrowTrend( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *trend);

  //! Create a curve object, i.e an object of class GrowCurve.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param data		Data for curves.
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param display_level Display levels when the object is visible.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param curve       	Created GrowCurve object.
  */
  void grow_CreateGrowCurve( grow_tCtx ctx, char *name, glow_sCurveData *data,
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, int fill_rect, 
	int border, glow_eDrawType fill_draw_type, void *user_data,
	grow_tObject *curve);

  //! Create a window object, i.e an object of class GrowWindow.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param display_level Display levels when the object is visible.
    \param user_data	User data.
    \param bar		Created GrowWindow object.
  */
  void grow_CreateGrowWindow( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *bar);

  //! Create a table object, i.e an object of class GrowTable.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param fill		Table filled.
    \param fillcolor	Fill color.
    \param display_level Display levels when the object is visible.
    \param user_data	User data.
    \param bar		Created GrowWindow object.
  */
  void grow_CreateGrowTable( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	int fill, glow_eDrawType fillcolor,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *bar);

  //! Create a folder object, i.e an object of class GrowFolder.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param width	Width of the rectangle.
    \param height	Height of the rectangle.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param selected_color Color of selected folder.
    \param unselected_color Color of unseleced folder.
    \param display_level Display levels when the object is visible.
    \param user_data	User data.
    \param bar		Created GrowFolder object.
  */
  void grow_CreateGrowFolder( grow_tCtx ctx, char *name, 
	double x, double y, double width, double height,
	glow_eDrawType draw_type, int line_width,
	glow_eDrawType selected_color, glow_eDrawType unselected_color,
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *bar);

  //! Create a line, i.e an object of class GrowRect.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x1		x coordinate for first point.
    \param y1		y coordinate for first point.
    \param x2		x coordinate for second point.
    \param y2		y coordinate for second point.
    \param draw_type	Line color.
    \param line_width	Line width.
    \param fix_line_width Line width is independent of scale.
    \param user_data	User data.
    \param line		Created GrowLine object.
  */
  void grow_CreateGrowLine( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int fix_line_width, 
	void *user_data,
	grow_tObject *line); 

  //! Create a polyline, i.e an object of class GrowRect.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param pointarray	Pointer to array with points.
    \param point_cnt	Number of points i pointarray.
    \param draw_type	Line color.
    \param line_width	Line width.
    \param fix_line_width Line width is independent of scale.
    \param fill		Fill polyline.
    \param border	Border is visible.
    \param shadow	Shadow is visible.
    \param fill_draw_type Fill color.
    \param closed	Polyline is closed.
    \param user_data	User data.
    \param polyline    	Created GrowPolyLine object.
  */
 void grow_CreateGrowPolyLine( grow_tCtx ctx, char *name, 
	glow_sPoint *pointarray, int point_cnt,
	glow_eDrawType draw_type, int line_width, int fix_line_width,
	int fill, int border, int shadow, glow_eDrawType fill_draw_type, 
	int closed, void *user_data,
	grow_tObject *polyline);

  //! Create an arc, i.e an object of class GrowArc.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x1		x coordinate for first corner of a rectangle that surrouds the elipse of the arc.
    \param y1		y coordinate for first corner of a rectangle that surrouds the elipse of the arc.
    \param x2		x coordinate for second corner of a rectangle that surrouds the elipse of the arc.
    \param y2		y coordinate for second corner of a rectangle that surrouds the elipse of the arc.
    \param angel1      	Start angel of the arc in degrees from the x axis.
    \param angel2      	Lengh of the arc in degrees from the start angel.
    \param draw_type	Border color.
    \param line_width	Linewidth of border.
    \param fill_arc    	Arc is filled.
    \param border	Border is visible.
    \param shadow	Shadow is visible.
    \param fill_draw_type Fill color.
    \param user_data	User data.
    \param arc    	Created GrowArc object.
  */
  void grow_CreateGrowArc( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	int angel1, int angel2, glow_eDrawType draw_type, 
	int line_width, int fill_arc, int border, int shadow, glow_eDrawType fill_draw_type,
	void *user_data,
	grow_tObject *arc);

  //! Create a connection point in a subgraph, i.e an object of class GrowConPoint.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param cp_num  	Conpoint number.
    \param d 		Conpoint direction.
    \param user_data	User data.
    \param conpoint   	Created GrowConPoint object.
  */
  void grow_CreateGrowConPoint( grow_tCtx ctx, char *name, 
	double x, double y, int cp_num, glow_eDirection d,
	void *user_data, grow_tObject *conpoint);

  //! Create an annotation in a subgraph, i.e an object of class GrowAnnot.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param annot_num	Annotation number.
    \param d_type 	Text drawtype.
    \param color_d_type Color drawtype.
    \param t_size	Text size.
    \param a_type	Type of annotation. One line or multiline.
    \param rel_pos	No used.
    \param display_lev	Level when the object is visible.
    \param user_data	User data.
    \param annot   	Created GrowAnnot object.
  */
  void grow_CreateGrowAnnot( grow_tCtx ctx, char *name, 
	double x, double y, int annot_num, glow_eDrawType d_type, glow_eDrawType color_d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	void *user_data, grow_tObject *annot);

  //! Create a text, i.e an object of class GrowText.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param text		Text.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param draw_type 	Text drawtype.
    \param color	Text color.
    \param t_size	Text size.
    \param display_level Level when the object is visible.
    \param user_data	User data.
    \param text_object 	Created GrowText object.
  */
  void grow_CreateGrowText( grow_tCtx ctx, char *name, 
	char *text, double x, double y,
	glow_eDrawType draw_type, glow_eDrawType color, int t_size, 
	glow_mDisplayLevel display_level, void *user_data,
	grow_tObject *text_object);

  //! Create an image object, i.e an object of class GrowImage.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param filename	Image filename.
    \param x1		x coordinate for position.
    \param y1		y coordinate for position.
    \param user_data	User data.
    \param image   	Created GrowImage object.
  */
  void grow_CreateGrowImage( grow_tCtx ctx, char *name, char *filename, 
	double x1, double y1, void *user_data, grow_tObject *image);

  //! Create an image object, i.e an object of class GrowImage.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x1		x coordinate for first corner.
    \param y1		y coordinate for first corner.
    \param x2		x coordinate for second corner.
    \param y2		y coordinate for second corner.
    \param draw_type 	Border color.
    \param line_width	Linewidth of border.
    \param text_size	Text size.
    \param text_drawtype Drawtype for text.
    \param user_data	User data.
    \param axis   	Created GrowImage object.
  */
  void grow_CreateGrowAxis( grow_tCtx ctx, char *name, 
	double x1, double y1, double x2, double y2,
	glow_eDrawType draw_type, int line_width, int text_size,
        glow_eDrawType text_drawtype, void *user_data,
	grow_tObject *axis);

  //! Create a connection glue object, i.e. an object of class GrowConGlue.
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param conglue   	Created GrowConGlue object.
  */
  void grow_CreateGrowConGlue( grow_tCtx ctx, char *name, 
	double x, double y, grow_tObject *conglue);

  //! Create a pulldown menu
  /*!
    \param ctx		Grow context.
    \param name		Object name (max 31 char).
    \param info	        Text and type of the buttons.
    \param x		x coordinate for position of lower left corner of the rectangle.
    \param y		y coordinate for position of lower left corner of the rectangle.
    \param min_width	Minimum width for the menu.
    \param draw_type	Border color.
    \param line_width	Line width of border.
    \param fill_rect	Rectangle is filled.
    \param border	Border is visible.
    \param fill_draw_type Fill color.
    \param text_size	Size of button text.
    \param text_drawtype Button text drawtype.
    \param text_color	Button text color.
    \param disabled_text_color Text color for disabled button.
    \param parent	Parent menu.
    \param menu		Created GrowMenu object.
  */
  void grow_CreateGrowMenu( grow_tCtx ctx, char *name, glow_sMenuInfo *info,
			    double x, double y, double min_width,
			    glow_eDrawType draw_type, int line_width,
			    int fill_rect, int border, glow_eDrawType fill_draw_type,
			    int text_size, glow_eDrawType text_drawtype,
			    glow_eDrawType text_color, glow_eDrawType disabled_text_color,
			    grow_tObject parent, grow_tObject *menu);

  //! Save subgraph to file.
  /*!
    \param ctx		Grow ctx.
    \param filename	Name of file to save subgraph in.
  */
  int grow_SaveSubGraph( grow_tCtx ctx, char *filename);

  //! Open subgraph from a file.
  /*!
    \param ctx		Grow ctx.
    \param filename	Name of file to read subgraph from.
  */
  int grow_OpenSubGraph( grow_tCtx ctx, char *filename);

  //! Open a subgraph with a specific name.
  /*!
    \param ctx		Grow ctx.
    \param name		Name subgraph.
  */
  int grow_OpenSubGraphFromName( grow_tCtx ctx, char *name);

  //! Find an object with a specified name.
  /*!
    \param ctx		Grow context.
    \param name		Name of object to search for.
    \param object	Found object.
    \return		Returns 1 if the object if found, else 0.
  */
  int grow_FindObjectByName( grow_tCtx ctx, char *name, grow_tObject *object);

  //! Find an nodeclass with a specified name.
  /*!
    \param ctx		Grow context.
    \param name		Name of nodeclass to search for.
    \param object	Found nodeclass.
    \return		Returns 1 if the nodeclass if found, else 0.
  */
  int grow_FindNodeClassByName( grow_tCtx ctx, char *name, grow_tObject *object);

  //! Find an connection class with a specified name.
  /*!
    \param ctx		Grow context.
    \param name		Name of conclass to search for.
    \param object	Found conclass.
    \return		Returns 1 if the conclass is found, else 0.
  */
  int grow_FindConClassByName( grow_tCtx ctx, char *name, grow_tObject *object);


  //! Get next connection point number for a subgraph
  /*! 
    \param ctx	Grow context.
    \return 	Next connection point number.

    Returns the number for next connectionpoint and increments the counter.
  */
  int grow_GetNextConPointNumber( grow_tCtx ctx);

  //! Get next objectname number.
  /*! 
    \param ctx	Grow context.
    \return Next objectname number.

    Returns the number for next objectname and increments the counter.
  */
  int grow_GetNextObjectNameNumber( grow_tCtx ctx);

  //! Get attributes for an object.
  /*!
    \param object	Object to get attributes for.
    \param transtab	Translation table.
    \param info		Array with attribute info.
    \param attr_cnt	Number of attributes in array.

    The info should be freed by the caller with a call to grow_FreeObjectAttrInfo.
  */
  int grow_GetObjectAttrInfo( grow_tObject object, char *transtab, 
			      grow_sAttrInfo **info, int *attr_cnt);

  //! Get attributes for a grow subgraph context.
  /*!
    \param ctx		Grow context for a subgraph.
    \param transtab	Translation table.
    \param info		Array with attribute info.
    \param attr_cnt	Number of attributes in array.

    The info should be freed by the caller with a call to grow_FreeSubGraphAttrInfo.
  */
  int grow_GetSubGraphAttrInfo( grow_tCtx ctx, char *transtab, grow_sAttrInfo **info,
			      int *attr_cnt);

  //! Get attributes for a grow context.
  /*!
    \param ctx		Grow context.
    \param info		Array with attribute info.
    \param attr_cnt	Number of attributes in array.

    The info should be freed by the caller with a call to grow_FreeGraphAttrInfo.
  */
  int grow_GetGraphAttrInfo( grow_tCtx ctx, grow_sAttrInfo **info,
	int *attr_cnt);

  //! Free the info array returned by grow_GetObjectAttrInfo.
  /*! \param attrinfo	Info array to free. */
  void grow_FreeObjectAttrInfo( grow_sAttrInfo *attrinfo);

  //! Free the info array returned by grow_GetSubGraphAttrInfo.
  /*! \param attrinfo	Info array to free. */
  void grow_FreeSubGraphAttrInfo( grow_sAttrInfo *attrinfo);

  //! Free the info array returned by grow_GetGraphAttrInfo.
  /*! \param attrinfo	Info array to free. */
  void grow_FreeGraphAttrInfo( grow_sAttrInfo *attrinfo);

  //! Update the attributes of an object with the values in an info array.
  /*!
    \param ctx		Grow context.
    \param object	Object to update.
    \param info		Info array.
  */
  void grow_UpdateObject(  grow_tCtx ctx, grow_tObject object,
			   grow_sAttrInfo *info);

  //! Update the attributes of subgraph context with the values in an info array.
  /*!
    \param ctx		Grow context for a subgraph.
    \param info		Info array.
  */
  void grow_UpdateSubGraph(  grow_tCtx ctx, grow_sAttrInfo *info);

  //! Update the attributes of grow context with the values in an info array.
  /*!
    \param ctx		Grow context.
    \param info		Info array.
  */
  void grow_UpdateGraph(  grow_tCtx ctx, grow_sAttrInfo *info);

  //! Check if an annotation with this number exists.
  /*!
    \param nodeclass 	Nodeclass for the annotation.
    \param num		Annotation number.
  */
  int grow_CheckAnnotation( grow_tNodeClass nodeclass, int num);

  //! Clear the context.
  /*! \param ctx	Grow context. */
  void grow_New( grow_tCtx ctx);

  //! Set the name of the context.
  /*!
    \param ctx		Grow context.
    \param name		Name (max 32 char).
  */
  void grow_SetName( grow_tCtx ctx, char *name);

  //! Get the context name.
  /*!
    \param ctx		Grow context.
    \param name		Name of the context.
  */
  void grow_GetName( grow_tCtx ctx, char *name);

  //! End a polyline creation sequence.
  /*! \param ctx	Grow context. */
  void grow_PolylineEnd( grow_tCtx ctx);

  //! Add points to a polyline.
  /*!
    \param object	Polyline object.
    \param point_array	Array width points to add.
    \param point_cnt	Number of points in the array.
  */
  void grow_AddPolyLinePoints( grow_tObject object, glow_sPoint *point_array, 
			       int point_cnt);

  //! Set fill for an object.
  /*!
    \param object	Object.
    \param fill		Fill value. 1 the object is drawn with fill, 0 without fill.
  */
  void grow_SetObjectFill( grow_tObject object, int fill);

  //! Set shadow for an object.
  /*!
    \param object	Object.
    \param shadow     	Shadow value. 1 the object is drawn with shadow, 0 without shadow.
  */
  void grow_SetObjectShadow( grow_tObject object, int shadow);

  //! Set shadow width for an object.
  /*!
    \param object	Object.
    \param width     	Shadow width in percentage of the smallest of object width and height.
  */
  void grow_SetObjectShadowWidth( grow_tObject object, double width);

  //! Set object drawtype.
  /*!
    \param object	Object.
    \param drawtype	Draw type.
  */
  void grow_SetObjectDrawtype( grow_tObject object, glow_eDrawType drawtype);

  //! Set object linewidth.
  /*!
    \param object	Object.
    \param linewidth	Line width.
  */
  void grow_SetObjectLinewidth( grow_tObject object, int linewidth);

  //! Set object line type.
  /*!
    \param object	Object.
    \param type		Line type.
  */
  void grow_SetObjectLinetype( grow_tObject object, glow_eLineType type);

  //! Execute dynamic code for all objects in the context.
  /*! \param ctx	Grow context. */
  void grow_ExecDynamic( grow_tCtx ctx);

  //! Set the position of an object relative the last stored transform.
  /*!
    \param object	Object to set position for.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
  */
  void grow_SetObjectPosition( grow_tObject object, double x, double y);

  //! Scale an object. The scale is made form the last stored transform.
  /*!
    \param object	Object to scale.
    \param scale_x	Scale factor in x direction.
    \param scale_y	Scale factor in y direction.
    \param x0		x coordinate for scale point.
    \param y0		y coordinate for scale point.
    \param type		Position or type of scale point.
  */
  void grow_SetObjectScale( grow_tObject object, double scale_x, double scale_y, 
			    double x0, double y0, glow_eScaleType type);

  //! Set position of an object and scale the object. The position and scale is relative the last stored transform.
  /*! 
    \param object	Object.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param scale_x	Scale factor in x direction.
    \param scale_y	Scale factor in y direction.
    \param x0		x coordinate for scale point.
    \param y0		y coordinate for scale point.
    \param type		Position or type of scale point.
  */
  void grow_SetObjectScalePos( grow_tObject object, double x, double y, 
			       double scale_x, double scale_y, 
			       double x0, double y0, glow_eScaleType type);

  //! Rotate an object. The rotations imade form the last stored transform.
  /*!
    \param object	Object to rotate.
    \param angel	Rotation angel in degrees.
    \param x0		x coordinate of rotation point.
    \param y0		y coordinate of rotation point.
    \param type		Type of rotation point.
  */
  void grow_SetObjectRotation( grow_tObject object, double angel, double x0,
			       double y0, glow_eRotationPoint type);

  //! Set object fill color.
  /*!
    \param object	Object.
    \param drawtype	Fill color.
  */
  void grow_SetObjectFillColor( grow_tObject object, glow_eDrawType drawtype);

  //! Reset object fillcolor to original fillcolor.
  /*! \param object	Object.  */
  void grow_ResetObjectFillColor( grow_tObject object);

  //! Set object border color.
  /*!
    \param object	Object.
    \param drawtype	Border color.
  */
  void grow_SetObjectBorderColor( grow_tObject object, glow_eDrawType drawtype);

  //! Reset object bordercolor to original bordercolor.
  /*! \param object	Object.  */
  void grow_ResetObjectBorderColor( grow_tObject object);

  //! Set object text color.
  /*!
    \param object	Object.
    \param drawtype	Text color.
  */
  void grow_SetObjectTextColor( grow_tObject object, glow_eDrawType drawtype);

  //! Reset object textcolor to original textcolor.
  /*! \param object	Object.  */
  void grow_ResetObjectTextColor( grow_tObject object);

  //! Set original border color on all selected objects.
  /*!
    \param ctx		Grow context.
    \param drawtype	Original border color.
  */
  void grow_SetSelectOrigBorderColor( grow_tCtx ctx, glow_eDrawType drawtype);

  //! Set original text color on all selected objects.
  /*!
    \param ctx		Grow context.
    \param drawtype	Original text color.
  */
  void grow_SetSelectOrigTextColor( grow_tCtx ctx, glow_eDrawType drawtype);

  //! Set original fill color on all selected objects.
  /*!
    \param ctx		Grow context.
    \param drawtype	Original fill color.
  */
  void grow_SetSelectOrigFillColor( grow_tCtx ctx, glow_eDrawType drawtype);

  //! Set original color tone on all selected objects.
  /*!
    \param ctx		Grow context.
    \param tone		Original color tone.
  */
  void grow_SetSelectOrigColorTone( grow_tCtx ctx, glow_eDrawTone tone);

  //! Set original color lightness on all selected objects.
  /*!
    \param ctx		Grow context.
    \param lightness	Original color lightness.
  */
  void grow_SetSelectOrigColLightness( grow_tCtx ctx, int lightness);

  //! Increase original color lightntess on all selected objects.
  /*!
    \param ctx		Grow context.
    \param lightness	Original color lightness increment.
  */
  void grow_IncrSelectOrigColLightness( grow_tCtx ctx, int lightness);

  //! Set original color intensity on all selected objects.
  /*!
    \param ctx		Grow context.
    \param intensity	Original color intensity.
  */
  void grow_SetSelectOrigColIntensity( grow_tCtx ctx, int intensity);
 
  //! Increase original color intensity on all selected objects.
  /*!
    \param ctx		Grow context.
    \param intensity	Original color intensity increment.
  */
  void grow_IncrSelectOrigColIntensity( grow_tCtx ctx, int intensity);

  //! Set original color shift on all selected objects.
  /*!
    \param ctx		Grow context.
    \param shift	Original color shift.
  */
  void grow_SetSelectOrigColorShift( grow_tCtx ctx, int shift);
 
  //! Increase original color shift on all selected objects.
  /*!
    \param ctx		Grow context.
    \param shift	Original color shift increment.
  */
  void grow_IncrSelectOrigColorShift( grow_tCtx ctx, int shift);

  //! Set object color tone.
  /*!
    \param object      	Object.
    \param tone		Color tone.
  */
  void grow_SetObjectColorTone( grow_tObject object, glow_eDrawTone tone);

  //! Reset object color tone to original color tone.
  /*! \param object      	Object. */
  void grow_ResetObjectColorTone( grow_tObject object);

  //! Set object color lightness.
  /*!
    \param object      	Object.
    \param lightness   	Color lightness.
  */
  void grow_SetObjectColorLightness( grow_tObject object, int lightness);

  //! Reset object color lightness to original color lightness.
  /*! \param object      	Object. */
  void grow_ResetObjectColorLightness( grow_tObject object);

  //! Set object color intensity.
  /*!
    \param object      	Object.
    \param intensity   	Color intensity.
  */
  void grow_SetObjectColorIntensity( grow_tObject object, int intensity);

  //! Reset object color intensity to original color intensity.
  /*! \param object      	Object. */
  void grow_ResetObjectColorIntensity( grow_tObject object);

  //! Set object color shift.
  /*!
    \param object      	Object.
    \param shift   	Color shift.
  */
  void grow_SetObjectColorShift( grow_tObject object, int shift);

  //! Increase object color shift.
  /*!
    \param object      	Object.
    \param shift   	Color shift increment.
  */
  void grow_IncrObjectColorShift( grow_tObject object, int shift);

  //! Reset object color shift to original color shift.
  /*! \param object      	Object. */
  void grow_ResetObjectColorShift( grow_tObject object);

  //! Set object color inverse.
  /*!
    \param object      	Object.
    \param inverse   	Color inverse.
  */
  void grow_SetObjectColorInverse( grow_tObject object, int inverse);

  //! Set object visibility.
  /*!
    \param object      	Object.
    \param visibility   	1 object is visible, 0 object is invisible.
  */
  void grow_SetObjectVisibility( grow_tObject object, glow_eVis visibility);

  //! Rotate selected objects.
  /*!
    \param ctx		Grow context.
    \param angel	Rotation angel in degrees.
    \param type		Type of rotation point.
  */
  void grow_RotateSelectedObjects( grow_tCtx ctx, double angel, 
				   glow_eRotationPoint type);

  //! Flip selected objects.
  /*!
    \param ctx		Grow context.
    \param dir		Flip direction. Vertical or horizontal flip.
  */
  void grow_FlipSelectedObjects( grow_tCtx ctx, glow_eFlipDirection dir);

  //! Set background color.
  /*!
    \param ctx		Grow context.
    \param color	Background color.
  */
  void grow_SetBackgroundColor( grow_tCtx ctx, glow_eDrawType color);

  //! Get background color.
  /*!
    \param ctx		Grow context.
    \param color	Background color.
  */
  void grow_GetBackgroundColor( grow_tCtx ctx, glow_eDrawType *color);

  //! Set background image.
  /*!
    \param ctx		Grow context.
    \param image	Name of imagefile.
  */
  void grow_SetBackgroundImage( grow_tCtx ctx, char *image);

  //! Get background image.
  /*!
    \param ctx		Grow context.
    \param image	Name of image file.
    \param tiled	Image is tiled.
  */
  void grow_GetBackgroundImage( grow_tCtx ctx, char *image, int *tiled);

  //! Get size of background image pixmap.
  /*!
    \param ctx		Grow context.
    \param width	Width in pixel.
    \param height	Height in pixel.
  */
  int grow_GetBackgroundImageSize( grow_tCtx ctx, int *width, int *height);

  //! Reset background color.
  /*! \param ctx	Grow context. */
  void grow_ResetBackgroundColor( grow_tCtx ctx);

  //! Disable highligth function.
  /*! \param ctx	Grow context. */
  void grow_DisableHighlight( grow_tCtx ctx);

  //! Enable highlight function.
  /*! \param ctx	Grow context. */
  void grow_EnableHighlight( grow_tCtx ctx);

  //! Get references to the dynamic code arguments of a subgraph context.
  /*!
    \param ctx		Grow subgraph context.
    \param argnames	Pointer to array with argument names.
    \param argtypes	Pointer to array with argument types.
    \param arg_cnt	Pointer to number of arguments.
  */
  void grow_RefSubGraphArgNames( grow_tCtx ctx, char **argnames, int **argtypes,
				 int **arg_cnt);

  //! Get the dynamic code of a subgraph.
  /*!
    \param ctx		Grow subgraph context.
    \param dynamic      Dynamic code.
    \param size		Size of dynamic code.
  */
  void grow_GetSubGraphDynamic( grow_tCtx ctx, char **dynamic, int *size);

  //! Get references to the dynamic code arguments of an object.
  /*!
    \param object      	Object.
    \param argnames	Pointer to array with argument names.
    \param argtypes	Pointer to array with argument types.
    \param argvalues	Pointer to array with argument values.
    \param arg_cnt	Pointer to number of arguments.
  */
  void grow_GetObjectArgs( grow_tObject object, char **argnames, int **argtypes,
			   char ***argvalues, int *arg_cnt);

  //! Push the selected objects.
  /*! \param ctx	Grow context. */
  void grow_PushSelectedObjects( grow_tCtx ctx);

  //! Pop the selected objects.
  /*! \param ctx	Grow context. */
  void grow_PopSelectedObjects( grow_tCtx ctx);

  //! Set default layout. Adjust zoom factor to the current size of the window.
  /*! \param ctx	Grow context. */
  void grow_SetDefaultLayout( grow_tCtx ctx);


  //! Set default layout to the specified values.
  /*!
    \param ctx		Grow context.
    \param x0		x coordinate for left border.
    \param y0		y coordinate for low border.
    \param x1		x coordinate for right border.
    \param y1		y coordinate for high border.
  */
  void grow_SetLayout( grow_tCtx ctx, double x0, double y0, double x1,
		       double y1);

  //! Set search path for subgraphs and images.
  /*!
    \param ctx			Grow context.
    \param path_cnt		Number of paths in path_vect.
    \param path			Array of paths char[10][80].
  */
  void grow_SetPath( grow_tCtx ctx, int path_cnt, char *path);

  //! Get dyntype and actiontype for the nodeclass of an object.
  /*!
    \param object	Object.
    \param dyn_type	Dyntype of the object's nodeclass.
    \param dyn_action_type Actiontype of the object's nodeclass.
  */
  void grow_GetObjectClassDynType( grow_tObject object, int *dyn_type, int *dyn_action_type);

  //! Get cycle for the nodeclass of an object.
  /*!
    \param object	Object.
    \param cycle	Cycle of the object's nodeclass.
  */
  void grow_GetObjectClassCycle( grow_tObject object, glow_eCycle *cycle);

  //! Get userdata for the nodeclass of an object.
  /*!
    \param object	Object.
    \param user_data	Userdata of the object's nodeclass.
  */
  void grow_GetObjectClassUserData( grow_tObject object, void **user_data);

  //! Get nodeclass dyntype and actiontype.
  /*!
    \param nodeclass	Nodeclass.
    \param dyn_type	Dyntype of the nodeclass.
    \param dyn_action_type Actiontype of the nodeclass.
  */
  void grow_GetNodeClassDynType( grow_tNodeClass nodeclass, int *dyn_type, int *dyn_action_type);

  //! Get the dyntype and actiontype of a grow subgraph context.
  /*!
    \param ctx		Grow subgraph context.
    \param dyn_type	Dyntype of the subgraph context.
    \param dyn_action_type Actiontype of the subgraph context.
  */
  void grow_GetSubGraphDynType( grow_tCtx ctx, int *dyn_type, int *dyn_action_type);

  //! Get the dynamic colors of a grow subgraph context.
  /*!
    \param ctx		Grow subgraph context.
    \param color	First dynamic color.
    \param color2	Second dynamic color.
  */
  void grow_GetSubGraphTraceColor( grow_tCtx ctx, glow_eDrawType *color,
				   glow_eDrawType *color2);

  //! Get the dynamic colors for an object's nodeclass.
  /*!
    \param object	Object.
    \param color	First dynamic color of the object's nodeclass.
    \param color2	Second dynamic color of the object's nodeclass.
  */
  void grow_GetObjectClassTraceColor( grow_tObject object, glow_eDrawType *color,
				      glow_eDrawType *color2);


  //! Get the dynamic attributes for an object's nodeclass.
  /*!
    \param object	Object.
    \param attr1	First dynamic attribute of the object's nodeclass.
    \param attr2	Second dynamic attribute of the object's nodeclass.
  */
  void grow_GetObjectClassDynAttr( grow_tObject object, int *attr1,
				   int *attr2);

  //! Set linewidth on all selected objects.
  /*!
    \param ctx		Grow context.
    \param linewidth	Line width.
  */
  void grow_SetSelectLineWidth( grow_tCtx ctx, int linewidth);

  //! Set or reset fill on all selected objects.
  /*!
    \param ctx		Grow context.
    \param fill		Fill.
  */
  void grow_SetSelectFill( grow_tCtx ctx, int fill);

  //! Set or reset border on all selected objects.
  /*!
    \param ctx		Grow context.
    \param border      	Border.
  */
  void grow_SetSelectBorder( grow_tCtx ctx, int border);

  //! Set or reset shadow on all selected objects.
  /*!
    \param ctx		Grow context.
    \param shadow      	Shadow.
  */
  void grow_SetSelectShadow( grow_tCtx ctx, int shadow);

  //! Set linetype on all selected objects.
  /*!
    \param ctx		Grow context.
    \param type		Line type.
  */
  void grow_SetSelectLineType( grow_tCtx ctx, glow_eLineType type);

  //! Check if any of the selected object is a connection.
  /*!
    \param ctx		Grow context.
    \return		1 if a connection is found, else 0.
  */
  int grow_AnySelectIsCon( grow_tCtx ctx);

  //! Change the connection class for all selected connections.
  /*!
    \param ctx		Grow context.
    \param conclass	Connections class to change to.
  */
  void grow_ChangeSelectConClass( grow_tCtx ctx, grow_tConClass conclass);

  //! Get attributes for a connection.
  /*!
    \param con		Connection.
    \param drawtype	Color.
    \param linewidth	Line width.
    \param contype	Type of connection.
    \param corner	Type of corners.
    \param round_corner_amount Size of the arcs for rounden corners.
  */
  void grow_GetConAttributes( grow_tCon con, glow_eDrawType *drawtype,
			      int *linewidth, glow_eConType *contype, glow_eCorner *corner, 
			      double *round_corner_amount);

  //! Change the connection class for a connection.
  /*!
    \param con		Connections.
    \param conclass	Connection class to change to.
  */
  void grow_ChangeConConClass( grow_tCon con, grow_tConClass conclass);

  //! Find an object.
  /*! 
    \param ctx		Grow context.
    \param object	Object to search for.
    \return		1 if object is found, else 0.
  */
  int grow_FindObject( grow_tCtx ctx, grow_tObject object);

  //! Set the text of a GrowText object.
  /*!
    \param object	A GrowText object.
    \param text		Text.
  */
  void grow_SetObjectText( grow_tObject object, char *text);

  //! Get the text of a GrowText object.
  /*!
    \param object	A GrowText object.
    \param text		Text.
  */
  void grow_GetObjectText( grow_tObject object, char *text);

  //! Set text size on all selected objects.
  /*!
    \param ctx		Grow context.
    \param size		Text size.
  */
  void grow_SetSelectTextSize( grow_tCtx ctx, int size);

  //! Set or reset text bold on all selected objects.
  /*!
    \param ctx		Grow context.
    \param bold		Bold.
  */
  void grow_SetSelectTextBold( grow_tCtx ctx, int bold);

  //! Set the value of a bar object.
  /*!
    \param object	A GrowBar object.
    \param value	Value to set.
  */
  void grow_SetBarValue( grow_tObject object, double value);

  //! Add a value to a trend.
  /*!
    \param object	A GrowTrend object.
    \param value	Value to add.
    \param idx		Curve index.
  */
  void grow_AddTrendValue( grow_tObject object, double value, int idx);

  //! Check if a nodeclass is a slider.
  /*!
    \param nodeclass	Nodeclass to check.
    \return		Retuns 1 if nodeclass is a slider, else 0.
  */
  int grow_IsSliderClass( grow_tNodeClass nodeclass);


  //! Restrict movement of moved objects.
  /*!
    \param ctx			Grow context.
    \param restriction		Type of restriction.
    \param max_limit		Max limit of movement. Used for slider movement.
    \param min_limit		Min limit of movement. Used for slider movement.
    \param object		Object for restrictions. User for slider movement.
  */
  void grow_SetMoveRestrictions( grow_tCtx ctx, glow_eMoveRestriction restriction,
				 double max_limit, double min_limit, grow_tObject object);

  //! Get current movement restrictions.
  /*!
    \param ctx			Grow context.
    \return			Type of restriction.
  */
  glow_eMoveRestriction grow_GetMoveRestrictions( grow_tCtx ctx);

  //! Set or reset scaletype to equal scale.
  /*!
    \param ctx		Grow context.
    \param equal	Scaleing of objects are equal in x and y direction.

    Scale equal means that when the scale of an objects is changed, the proportions in x and y
    direction is kept.
  */
  void grow_SetScaleEqual( grow_tCtx ctx, int equal);

  //! Get current scaletype.
  /*!
    \param ctx		Grow context.
    \return		If 1 scaletype is equal.
  */
  int grow_GetScaleEqual( grow_tCtx ctx);

  //! Get info for a slider object.
  /*!
    \param object       Slider object.
    \param direction	Direction of slider movement.
    \param max_value	Maximum value.
    \param min_value	Minimum value.
    \param min_pos	Minimum position. Max value of coordinate in slider direction.
    \param max_pos	Maximum position. Min value of coordinate in slider direction.
  */
  void grow_GetSliderInfo( grow_tObject object, glow_eDirection *direction,
			   double *max_value, double *min_value, double *max_pos, double *min_pos);

  //! Get info for a slider object in pixel.
  /*!
    \param object	Slider object.
    \param direction	Direction of slider movement.
    \param min_pos	Minimum position. Max value of coordinate in slider direction in pixel.
    \param max_pos	Maximum position. Min value of coordinate in slider direction in pixel.
    \param bg_dyntype	Dyntype for the background object of the slider.
  */
  void grow_GetSliderInfoPixel( grow_tObject object, glow_eDirection *direction,
				double *max_pos, double *min_pos, int bg_dyntype);

  //! Get the zero position of the slider.
  /*!
    \param object	Slider object.
    \param direction	Direction of the slider.
    \param pos		Position of the zero point.

    The zero point for the slider is the point in the slider object that corresponds to the slider value.
    This can be a x coordinate or an y coordinate dependent of the actual direction of the slider.
    The zero point for a slider is stored i y0 for the NodeClass of the slider, in the nodeclass 
    coordinate system.
  */
  void grow_GetSliderOrigo( grow_tObject object, glow_eDirection direction,
			    double *pos);

  //! Set info for a slider object.
  /*!
    \param object       Slider object.
    \param direction	Direction of slider movement.
    \param max_value	Maximum value.
    \param min_value	Minimum value.
    \param min_pos	Minimum position. Max value of coordinate in slider direction.
    \param max_pos	Maximum position. Min value of coordinate in slider direction.
  */
  void grow_SetSliderInfo( grow_tObject object, glow_eDirection direction,
			   double max_value, double min_value, double max_pos, double min_pos);

  //! Get grow context from object.
  /*!
    \param object	Object.
    \param ctx		Returned grow context.
  */
  void grow_ObjectToCtx( grow_tObject object, grow_tCtx *ctx);

  //! Store the current transform of an object.
  /*!
    \param object	Object.

    The stored transform is starting point for positioning, scaling and rotating, in the functions
    grow_SetObjectPosition, grow_SetObjectScale, grow_SetObjectScalePos and grow_SetObjectRotation.
  */
  void grow_StoreTransform( grow_tObject object);

  //! Check if there is a stored transform
  /*! 
    \return Returns 1 if the transform is previously stored with a call to grow_StoreTransform.
  */
  int grow_TransformIsStored( grow_tObject object);

  //! Align the selected objects.
  /*!
    \param ctx		Grow context.
    \param direction	Direction for alignment.
  */
  void grow_AlignSelect( grow_tCtx ctx, glow_eAlignDirection direction);

  //! Set equal distance between selected objects.
  /*! 
    \param ctx		Grow context.
    \param direction   	Direction in which to order objects. 
  */
  void grow_EquiDistanceSelect( grow_tCtx ctx, glow_eAlignDirection direction);

  //! Check if grow context is a subgraph context.
  /*!
    \param ctx		Grow context.
    \return		Returns 1 if context is a subgraph, else 0.
  */
  int grow_IsSubGraph( grow_tCtx ctx);

  //! Set the range of a slider object.
  /*!
    \param object	Slider object.
    \param min		Minimum value of range.
    \param max		Maximum value of range.
  */
  void grow_SetSliderRange( grow_tObject object, double min, double max);

  //! Set the range of a bar object.
  /*!
    \param object	Bar object.
    \param min		Minimum value of range.
    \param max		Maximum value of range.
  */
  void grow_SetBarRange( grow_tObject object, double min, double max);

  //! Set fill curve in trend object
  /*!
    \param object	Trend object.
    \param fill		Trend curves are drawn filled.
  */
  void grow_SetTrendFillCurve( grow_tObject object, int fill);

  //! Set the range of a trend object.
  /*!
    \param object	Trend object.
    \param curve	Curve number.
    \param min		Minimum value of range.
    \param max		Maximum value of range.
  */
  void grow_SetTrendRange( grow_tObject object, int curve,
			   double min, double max);

  //! Get the scantime of a trend object.
  /*!
    \param object	Trend object.
    \param time		Scantime in seconds.
  */
  void grow_GetTrendScanTime( grow_tObject object, double *time);

  //! Set the scantime of a trend object.
  /*!
    \param object	Trend object.
    \param time		Scantime in seconds.
  */
  void grow_SetTrendScanTime( grow_tObject object, double time);

  //! Set number of vertical and horizontal lines in a trend object.
  /*!
    \param object	Trend object.
    \param vert_lines	Number of vertical lines.
    \param horiz_lines	Number of horizontal lines.
  */
  void grow_SetTrendLines( grow_tObject object, int vert_lines, 
			   int horiz_lines);

  //! Set configuration values for an axis object.
  /*!
    \param object	Axis object.
    \param max_val	Maximum value of axis.
    \param min_val	Minimum value of axis.
    \param no_of_lines	Number of scale division lines.
    \param long_quot	Quotient of long scale division lines.
    \param value_quot	Quotient of scale division values.
    \param rot		Rotation of axis object in degrees.
    \param format	Format of scale division values i c syntax.
  */
  void grow_SetAxisConf( grow_tObject object, double max_val, double min_val, 
			 int no_of_lines, int long_quot, int value_quot, double rot, 
			 char *format);

  //! Set that graph is modified or not since last save.
  /*! 
    \param ctx			Grow context.
    \param modified		1: graph is modified, 0: graph is not modified.
    
  */
  void grow_SetModified( grow_tCtx ctx, int modified);

  //! Get if graph is modified or not since last save.
  /*! 
    \param ctx		Grow context.
    \return  		Returns 1 if graph is modified, 0 if graph is not modified. 
  */
  int grow_GetModified( grow_tCtx ctx);

  //! Measure the extension of a subgraph.
  /*!
    \param ctx			Grow context.
    \param x_right		Right border in pixel.
    \param x_left		Left border in pixel.
    \param y_high		High border in pixel.
    \param y_low		Low border in pixel.

    If x0, y0, x1, y1 are set these are used as borders, otherwise the borders are calculated
    by meauring all objects i the contexts. The extension are given in pixels width the 
    current zoom factor.
  */
  void grow_MeasureJavaBean( grow_tCtx ctx, double *x_right, double *x_left,
			     double *y_high, double *y_low);

  //! Convert coordinates to pixel.
  /*!
    \param ctx		Grow context.
    \param x		x coordinate.
    \param y		y coordinate.
    \param x_pix	x coordinate in pixel.
    \param y_pix	y coordinate in pixel.
  */
  void grow_ToPixel( grow_tCtx ctx, double x, double y,
		     double *x_pix, double *y_pix);


  //! Zoom to the java size of the picture.
  /*!
    \param ctx		Grow ctx.
    \param x_right	Right border in pixel.
    \param x_left	Left border in pixel.
    \param y_high	High border in pixel.
    \param y_low	Low border in pixel.

    The java size is determined by java_width. The picture is zoomed to the level
    where the border width of the picture equals java_width.
  */
  void grow_SetJavaFrame( grow_tCtx ctx, double *x_right, double *x_left,
			  double *y_high, double *y_low);

  //! Measure the extent in pixel of a nodeclass for the current zoomfactor.
  /*!
    \param nodeclass		Node class to measure.
    \param x_right		Right border in pixel.
    \param x_left		Left border in pixel.
    \param y_high		High border in pixel.
    \param y_low		Low border in pixel.
  */
  void grow_MeasureNodeClassJavaBean( grow_tNodeClass nodeclass, double *x_right, double *x_left,
				      double *y_high, double *y_low);

  //! Export context as a javabean.
  /*!
    \param ctx		Grow context
    \param fp		Output file.
    \param components	0: Paint. 1: Declare components. 2: Set component attributes.
  */
  void grow_ExportJavaBean( grow_tCtx ctx, ofstream& fp, int components);

  //! Export a nodeclass as javabean.
  /*!
    \param ctx		Grow context.
    \param nc		Node class.
    \param fp		Output file.
    \param components	0: Paint. 1: Declare components. 2: Set component attributes.
  */
  void grow_ExportNodeClassJavaBean( grow_tCtx ctx, grow_tNodeClass nc, 
				     ofstream& fp, int components);

  //! Export annotation fonts in a nodeclass.
  /*!
    \param ctx		Grow context.
    \param nc		Nodeclass.
    \param fp		Output file.
    \param components	0: Paint. 1: Declare components. 2: Set component attributes.
  */
  void grow_ExportNcJavaBeanFont( grow_tCtx ctx, grow_tNodeClass nc, 
				  ofstream& fp, int components);

  //! Set java name of the context.
  /*!
    \param ctx		Grow context.
    \param name 	Name of java class for the graph. 
  */
  void grow_SetJavaName( grow_tCtx ctx, char *name);

  //! Get java name of the context.
  /*!
    \param ctx		Grow context.
    \param name 	Name of java class for the graph. 
  */
  int grow_GetJavaName( grow_tCtx ctx, char *name);

  //! Check if context is configured to be exported as a java applet.
  /*!
    \param ctx		Grow context.
    \return		1 if context can be exported as a java applet, else 0.
  */
  int grow_IsJavaApplet( grow_tCtx ctx);

  //! Check if context is configured to be exported as a java frame.
  /*!
    \param ctx		Grow context.
    \return		1 if context can be exported as a java frame, else 0.
  */
  int grow_IsJavaApplication( grow_tCtx ctx);

  //! Get java name for the nodeclass of an object.
  /*!
    \param object	Object.
    \param name		Java name. If no java name is set, the nodeclass name is returned.
    \return		Returns 1 if a java name is set, else 0.
  */
  int grow_GetObjectClassJavaName( grow_tObject object, char *name);

  //! Get java name for a nodeclass.
  /*!
    \param nodeclass	Nodeclass.
    \param name		Java name. If no java name is set, the nodeclass name is returned.
    \return		Returns 1 if a java name is set, else 0.
  */
  int grow_GetNodeClassJavaName( grow_tNodeClass nodeclass, char *name);

  //! Get the numbers of all annotations in an objects
  /*!
    \param object      	Object.
    \param numbers      Buffer with an array with the annotation numbers.
    \param cnt		Number of annotations.

    The buffer should be freed by the caller.
  */
  void grow_GetObjectAnnotationNumbers( grow_tObject object, int **numbers,
					int *cnt);

  //! Get the numbers of all annotations in a subgraph context.
  /*!
    \param ctx		Grow subgraph context.
    \param numbers      Buffer with an array with the annotation numbers.
    \param cnt		Number of annotations.

    The buffer should be freed by the caller.
  */
  void grow_GetSubgraphAnnotNumbers( grow_tCtx ctx, int **numbers,
				     int *cnt);

  //! Check if a grow sugraph context is a slider.
  /*!
    \param ctx		Grow subgraph context.
    \return		1 if context is configured as a slider, else 0.
  */
  int grow_IsSlider( grow_tCtx ctx);

  //! Get the name of a color.
  /*!
    \param drawtype	Color.
    \return		The name of the color.
  */
  char *grow_ColorToName( glow_eDrawType drawtype);

  //! Get the name of a color tone.
  /*!
    \param drawtype	Color tone.
    \return		The name of the color tone.
  */
  char *grow_ColorToneToName( glow_eDrawType drawtype);

  //! Set next page to the current page for an object.
  /*!
    \param object	Object to set page for.
    \return		Returns 1 if there is a next page, else 0.
  */
  int grow_SetObjectNextNodeClass( grow_tObject object);

  //! Set previous page to the current page for an object.
  /*!
    \param object	Object to set page for.
    \return		Returns 1 if there is a previous page, else 0.
  */
  int grow_SetObjectPrevNodeClass( grow_tObject object);

  //! Set first page to the current page for an object.
  /*! \param object	Object to set page for.*/
  void grow_SetObjectFirstNodeClass( grow_tObject object);

  //! Set last page to the current page for an object.
  /*! \param object	Object to set page for.*/
  void grow_SetObjectLastNodeClass( grow_tObject object);

  //! Set page for an object to the page of the specified index.
  /*!
    \param object	Object to set page for.
    \param idx		Index for the page to set.
  */
  int grow_SetObjectNodeClassByIndex( grow_tObject object, int idx);

  //! Get the animation count of the current page.
  /*!
    \param object	Object.
    \param count	The animation count of the current page.
  */
  void grow_GetObjectAnimationCount( grow_tObject object, int *count);

  //! Get scantime for the context.
  /*!
    \param ctx		Grow context.
    \param scantime	Scantime for slow cycle.
    \param fast_scantime Scantime for fast cycle.
    \param animation_scantime Scantime for animations.
  */
  void grow_GetScanTime( grow_tCtx ctx, double *scantime, double *fast_scantime, 
			 double *animation_scantime);

  //! Display or erase the gridpoints.
  /*!
    \param ctx		Grow context.
    \param show		1: show the gridpoints, 0: hide.
  */
  void grow_SetShowGrid( grow_tCtx ctx, int show);

  //! Get if gridpoints are displayed.
  /*!
    \param ctx		Grow context.
    \return		1: gridpoints are displayed, 0: hidden.
  */
  int grow_GetShowGrid( grow_tCtx ctx);

  //! Check if a nodeclass is a next page.
  /*!
    \param nodeclass	Nodeclass to check.
    \return 		1 if the nodeclass is a next page, 0 if it is the first page. 
  */
  int grow_IsNextNodeClass( grow_tNodeClass nodeclass);

  //! Get number of pages in a nodeclass.
  /*!
    \param nodeclass	Nodeclass.
    \return 		Number of pages for this nodeclass. 
  */
  int grow_GetNodeClassPages( grow_tNodeClass nodeclass); 

  //! Get the name of the subgraph for the next page for this context.
  /*!
    \param ctx		Grow subgraph context.
    \param next		The name of the subgraph for next page.
  */
  void grow_GetNextSubgraph( grow_tCtx ctx, char *next);

  //! Set the name of the subgraph for the next page for this context.
  /*!
    \param ctx		Grow subgraph context.
    \param next		The name of the subgraph for next page.
  */
  void grow_SetNextSubgraph( grow_tCtx ctx, char *next);

  //! Store the current zoomfactor and offsets.
  /*! 
    \param ctx		Grow context.

    The stored geometry is restored with grow_RestoreGeometry(); 
  */
  void grow_StoreGeometry( grow_tCtx ctx);

  //! Restore the stored geometry.
  /*!
    \param ctx		Grow context.

    Restore the zoomfactor and offsets previously stored with grow_StoreGeometry().
  */
  void grow_RestoreGeometry( grow_tCtx ctx);

  //! Set click sensitivity for the window.
  /*!
    \param ctx		Grow context.
    \param value	Mask of type glow_mSensitivity.
  */
  void grow_SetClickSensitivity( grow_tCtx ctx, int value);

  //! Move a node object or group to a specific point.
  /*!
    \param node		A object of type GrowNode, GrowSlider or GrowGroup.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
  */
  void grow_MoveNode( grow_tNode node, double x, double y);

  //! Get extent of a text
  /*!
    \param ctx		Grow context.
    \param text		Text.
    \param len		Lenght of text.
    \param draw_type	Text drawtype.
    \param text_size	Size of text.
    \param width	Returned width of text.
    \param height	Returned height of text.
    \param descent	Returnd descent of text.
  */
  void grow_GetTextExtent( grow_tCtx ctx, char *text, int len, 
			   glow_eDrawType draw_type, int text_size, double *width,
			   double *height, double *descent);

  //! Get the default window size
  /*!
    \param ctx		Grow context.
    \param width	Window width in pixel.
    \param height	Window height in pixel.
    \return		Returns 1 if size is configured, otherwise 0.

    The window size is configured in x0, y0, x1 and y1. From these the window
    size in pixel is calculated.
  */
  int grow_GetDefaultWindowSize( grow_tCtx ctx, int *width, int *height);

  //! Group the selected objects.
  /*!
    \param ctx		Grow context.
    \param group	Created group.
    \param last_group_name Name of last group.

    The last group is the group name registred as last group among the majority of the
    group members
  */
  int grow_GroupSelect( grow_tCtx ctx, grow_tObject *group, char *last_group_name);

  //! Ungroup selected groups.
  /*!
    \param ctx		Grow context.
    \return		Always returns 1.
  */
  int grow_UngroupSelect( grow_tCtx ctx);

  //! Configure the curves of a curve object.
  /*!
    \param object	Curve object.
    \param data		Configuration data for the curves.

    Calculate position of the points of the curves and create a polyline for each curve.
  */
  void grow_CurveConfigure( grow_tObject object, glow_sCurveData *data);

  //! Add a new value to the specified curve in a curve object.
  /*!
    \param object	Curve object.
    \param data		Data for the new values of the curves.

    Add the new value first in all the curves, and shift the other values one step forward.
  */
  void grow_CurveAddPoints( grow_tObject object, glow_sCurveData *data);

  //! Get the group the specified object is a member of.
  /*!
    \param ctx		Grow context.
    \param object	Object to get group for.
    \param group	Found group for the object.
    \return		Returns 1 if a group is found, 0 if the object isn't a memeber of a group.
  */
  int grow_GetObjectGroup( grow_tCtx ctx, grow_tObject object, 
			   grow_tObject *group);

  //! Get list of the group members of a group.
  /*!
    \param group	Group.
    \param list		Returned list of members.
    \param cnt		Number of objects in list.
  */
  void grow_GetGroupObjectList( grow_tObject group, grow_tObject **list, 
				int *cnt);

  //! Set a nodeclass extern or intern.
  /*!
    \param nodeclass	Nodeclass to set extern or intern.
    \param ext		1 set extern, 0 set intern.

    If the nodeclass is extern it is not saved in the Glow context file, and read from
    the subgraph file of the nodeclass, the next time the graph is opened.
  */
  void grow_SetNodeClassExtern( grow_tNodeClass nodeclass, int ext);

  //! Get the limits configured in a background object for a slider.
  /*!
    \param ctx		Grow context.
    \param type		Dyntype for the background object.
    \param x		x coordinate for the foreground object.
    \param y		y coordinate for the foreground object.
    \param background  	Found background object.
    \param min		Min limit for background object.
    \param max		Max limit for background object.
    \param direction	Direction if background object.

    The background object is an object with the specified dyntype that covers the specified
    coordinates. The limits are stored in y0 and y1 for the nodeclass of the objects.
  */
  int grow_GetBackgroundObjectLimits( grow_tCtx ctx, glow_eTraceType type,
				      double x, double y, grow_tObject *background,
				      double *min, double *max, glow_eDirection *direction);

  //! Get if translation is on or off.
  /*!
    \param ctx		Grow context.
    \return		1 if translation is on, else 0.
  */
  int grow_GetTranslate( grow_tCtx ctx);

  //! Get MB3 action configured for the context.
  /*!
    \param ctx		Grow context.
    \return		Returns the configured MB3 action.
  */
  glow_eMB3Action grow_GetMB3Action( grow_tCtx ctx);


  //! Get coordinates of origo for a nodeclass of an object.
  /*!
    \param object	Object.
    \param x		x coordinate of nodeclass origo.
    \param y		y coordinate of nodeclass origo.
  */
  void grow_GetNodeClassOrigo( grow_tObject object, double *x, double *y);

  //! Update the image of an image object.
  /*!
    \param object	An image object of type GrowImage.
    
    If there is a new date on the image file, the file is read again, and the image is updated.
  */
  int grow_ImageUpdate( grow_tObject object);


  //! Register callback functions for userdata handling.
  /*!
    \param ctx	       	Grow context.
    \param save		Callback function that will be called when userdata is saved to file.
    \param open		Callback function that will be called when userdata is opened from file.
    \param copy		Callback function that will be called when userdata is copied.
  */
  void grow_RegisterUserDataCallbacks( grow_tCtx ctx,
				       void (*save)( void *, void *, glow_eUserdataCbType),
				       void (*open)( void *, void *, glow_eUserdataCbType),
				       void (*copy)( void *, void *, void **, glow_eUserdataCbType));

  //! Get grow versions.
  /*!
    \param ctx			Grow context.
    \param grow_version		Grow version.
    \param graph_version	Version of the current context.
  */
  void grow_GetVersion( grow_tCtx ctx, int *grow_version, int *graph_version);

  //! Update the version of the context to the grow version.
  /*! \param ctx	Grow context. */
  void grow_UpdateVersion( grow_tCtx ctx);

  //! Set tooltip text for the current hot object.
  /*!
    \param ctx		Grow context.
    \param object	Current object to set tooltip text for.
    \param text		Tooltip text for the object.
    \param x		x coordinate for the text in pixel.
    \param y		y coordinate for the text in pixel.
  */
  void grow_SetTipText( grow_tCtx ctx, grow_tObject object, char *text, int x, int y);

  //! Set object level fill color.
  /*!
    \param object	Object.
    \param color	Fill level color.
  */
  void grow_SetObjectLevelFillColor( grow_tObject object, glow_eDrawType color);
  //! Set object level fillcolor.
  /*!
    \param object	Object.
    \param tone		Level fillcolor.
  */
  void grow_SetObjectLevelColorTone( grow_tObject object, glow_eDrawTone tone);

  //! Set object level direction.
  /*!
    \param object	Object.
    \param direction	Level direction.
  */
  void grow_SetObjectLevelDirection( grow_tObject object, glow_eDirection direction);

  //! Set the fill level of an object.
  /*!
    \param object	Object.
    \param level	Fill level.
  */
  void grow_SetObjectFillLevel( grow_tObject object, double level);

  //! Get the limits for the level dynamics for an object.
  /*!
    \param object	Object.
    \param min		Min limit.
    \param max		Max limit.
    \param direction 	Direction of the node.

    The limits are stored in y0 and y1 in the nodeclass for the object, in the nodeclass
    coordinate system, and measured in Up direction. These coordinates are transformed with 
    the node tranformation matrix, and the direction is determined from the rotation of the node.
  */
  int grow_GetObjectLimits( grow_tObject object, double *min, double *max, 
			    glow_eDirection *direction);

  //! Get the limits in pixel for the level dynamics for an object.
  /*!
    \param object	Object.
    \param min		Min limit.
    \param max		Max limit.
    \param direction 	Direction of the node.

    The limits are stored in y0 and y1 in the nodeclass for the object, in the nodeclass
    coordinate system, and measured in Up direction. These coordinates are transformed with 
    the node tranformation matrix, and the direction is determined from the rotation of the node.
  */
  int grow_GetObjectLimitsPixel( grow_tObject object, double *min, double *max,
				 glow_eDirection *direction);

  //! Set parameters for a trend object.
  /*!
    \param object	Trend object.
    \param info		Info struct.
  */
  void grow_SetTrendInfo( grow_tObject object, glow_sTrendInfo *info);

  //! Set parameters for a bar object.
  /*!
    \param object	Bar object.
    \param info		Info struct.
  */
  void grow_SetBarInfo( grow_tObject object, glow_sBarInfo *info);

  //! Get parameters for a bar object.
  /*!
    \param object	Bar object.
    \param info		Info struct.
  */
  void grow_GetBarInfo( grow_tObject object, glow_sBarInfo *info);

  //! Set parameters for an axis object.
  /*!
    \param object	Axis object.
    \param info		Info struct.
  */
  void grow_SetAxisInfo( grow_tObject object, glow_sAxisInfo *info);

  //! Conversion between different versions of Glow
  /*!
    \param ctx		Grow context.
    \param version	Version to convert to.
  */

  //! Set parameters for a table object.
  /*!
    \param object	Table object.
    \param info		Info struct.
  */
  void grow_SetTableInfo( grow_tObject object, glow_sTableInfo *info);

  //! Get parameters for a table object.
  /*!
    \param object	Table object.
    \param info		Info struct.
  */
  void grow_GetTableInfo( grow_tObject object, glow_sTableInfo *info);

  //! Set value for a cell in a table object.
  /*!
    \param object	Table object.
    \param column	Cell column.
    \param row		Cell row.
    \param value	Cell value.
  */
  void grow_SetCellValue( grow_tObject object, int column, int row, char *value);

  //! Set selected cell in a table object.
  /*!
    \param object	Table object.
    \param column	Cell column.
    \param row		Cell row.
  */
  void grow_SetSelectedCell( grow_tObject object, int column, int row);

  //! Get selected cell in a table object.
  /*!
    \param object	Table object.
    \param column	Cell column.
    \param row		Cell row.
  */
  int grow_GetSelectedCell( grow_tObject object, int *column, int *row);

  //! Check if a cell is visible, and scroll if not.
  /*!
    \param object	Table object.
    \param column	Cell column.
    \param row		Cell row.
  */
  int grow_TableMakeCellVisible( grow_tObject object, int column, int row);

  void grow_Convert( grow_tCtx ctx, glow_eConvert version);

  //! Check if a nodeclass is extern.
  /*!
    \param nodeclass	Nodeclass to check.
    \return		1 if nodeclass is extern, else 0.
  */
  int grow_NodeClassIsExtern( grow_tNodeClass nodeclass);

  //! Get info for a connections point in a node object.
  /*!
    \param node		Node object.
    \param num		Connection point number to get info for.
    \param x		x coordinate of conpoint.
    \param y		y coordinate of conpoint.
    \param dir		Direction of conpoint.
  */
  int grow_GetNodeConPoint( grow_tNode node, int num, double *x, double *y, glow_eDirection *dir);

  //! Send an input focus init event.
  /*! \param ctx	Grow context. */
  void grow_InputFocusInitEvent( grow_tCtx ctx);

  //! Set fast curve data for a GrowTrend object.
  /*!
    \param object	GrowTrend object. 
    \param data		Pointers to data arrays.
    \param data_curves	Number of curves in data.
    \param data_points	Number of points in each curve.
  */
  void grow_SetTrendData( grow_tObject object, double *data[3], int data_curves, int data_points);

  //! Get text size and color for an annotation.
  /*!
    \param object	A GrowNode object.
    \param num		Annotation number.
    \param text_size	Annotation text size.
    \param text_drawtype Annotation text drawtype.
    \param text_color	Annotation text color.
    \param bg_color	Annotation background color.
    \param scale	Vertical scale.
    \return		0 if annotation doesn't exist, else 1.
  */
  int grow_GetObjectAnnotInfo( grow_tObject object, int num, int *text_size, glow_eDrawType *text_drawtype,
				glow_eDrawType *text_color, glow_eDrawType *bg_color, double *scale);

  //! Get color, textsize and text colors for a menu.
  /*!
    \param menu		Menu object.
    \param t_size	Text size.
    \param fill_color	Fill color.
    \param t_drawtype	Drawtype for text.
    \param t_color	Text color.
    \param t_color_disabled Text color for disabled button.
  */
  void grow_GetMenuChar( grow_tObject menu, int *t_size, glow_eDrawType *fill_color, glow_eDrawType *t_drawtype,
			 glow_eDrawType *t_color, glow_eDrawType *t_color_disabled);


  void grow_MenuShiftCurrentItem( grow_tObject menu, int shift);

  int grow_MenuGetCurrentItem( grow_tObject menu, int *item);
  void grow_SetMenuInputFocus( grow_tObject menu, int focus);
  int grow_GetMenuInputFocus( grow_tObject menu);
  void grow_GetSubmenuPosition( grow_tObject menu, int item, double *x, double *y);
  int grow_GetMenuParent( grow_tObject menu, grow_tObject *parent);
  int grow_SetFolderIndex( grow_tObject folder, int idx);
  int grow_SetWindowSource( grow_tObject window, char *source);
  void grow_GetWindowSize( grow_tCtx ctx, int *width, int *height);
  int grow_IsJava( char *name, int *is_frame, int *is_applet, char *java_name);
  void grow_GetOwner( grow_tCtx ctx, char *owner);
  void grow_SetOwner( grow_tCtx ctx, char *owner);
  int grow_GetNodeClassAnnotBackground( grow_tNodeClass nodeclass, glow_eDrawType *background);

/*@}*/
#if defined __cplusplus
}
#endif
#endif

