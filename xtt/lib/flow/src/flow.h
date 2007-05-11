/* 
 * Proview   $Id: flow.h,v 1.12 2007-05-11 15:07:21 claes Exp $
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

#ifndef flow_h
#define flow_h

#if defined __cplusplus
extern "C" {
#endif


#ifndef ODD
#define ODD(a)	(((int)(a) & 1) != 0)
#endif

#ifndef EVEN
#define EVEN(a)	(((int)(a) & 1) == 0)
#endif

#ifndef MAX
#define MAX(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#endif

#ifndef MIN
#define MIN(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#endif

  //#ifndef __ALPHA
  //#define ABS(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
  //#endif

#define DRAW_TYPE_SIZE 9
#define DRAW_FONT_SIZE 9
#define DRAW_PIXMAP_SIZE 9

#define FSPACE " "

typedef char flow_tTraceObj[200];
typedef char flow_tTraceAttr[200];
typedef char flow_tName[80];
typedef void *flow_tPixmap;
typedef void *flow_tWidget;
typedef void *flow_tImData;
typedef void *flow_tImImage;

typedef enum {
	flow_eCtxType_Flow,
	flow_eCtxType_Brow
	} flow_eCtxType;

typedef enum {
	flow_eSelectPolicy_Surround,
	flow_eSelectPolicy_Partial
	} flow_eSelectPolicy;

typedef enum {
     	flow_eVisible_Full,
	flow_eVisible_Partial,
	flow_eVisible_Top,
	flow_eVisible_Bottom
	} flow_eVisible;

typedef enum {
	flow_eObjectType_NoObject,
	flow_eObjectType_Node,
	flow_eObjectType_Con,
	flow_eObjectType_Rect,
	flow_eObjectType_Line,
	flow_eObjectType_Arc,
	flow_eObjectType_Text,
	flow_eObjectType_ConPoint,
	flow_eObjectType_Annot,
	flow_eObjectType_NodeClass,
	flow_eObjectType_ConClass,
	flow_eObjectType_Arrow,
	flow_eObjectType_Pixmap,
	flow_eObjectType_AnnotPixmap,
	flow_eObjectType_Radiobutton,
	flow_eObjectType_Frame,
	flow_eObjectType_Image
	} flow_eObjectType;

typedef enum {
	flow_eDirection_Center,
	flow_eDirection_Right,
	flow_eDirection_Left,
	flow_eDirection_Up,
	flow_eDirection_Down
	} flow_eDirection;

typedef enum {
	flow_eNodeGroup_Common,
	flow_eNodeGroup_Document,
	flow_eNodeGroup_Trace
	} flow_eNodeGroup;

typedef enum {
	flow_eConGroup_Common,
	flow_eConGroup_Trace
	} flow_eConGroup;

typedef enum {
	flow_eAnnotType_OneLine,
	flow_eAnnotType_MultiLine
	} flow_eAnnotType;

typedef enum {
	flow_eConType_Straight,
	flow_eConType_Fixed,
	flow_eConType_AllFixed,
	flow_eConType_Routed,
	flow_eConType_StepDiv,
	flow_eConType_StepConv,
	flow_eConType_TransDiv,
	flow_eConType_TransConv,
	flow_eConType_StraightOneArrow,
	flow_eConType_Reference
	} flow_eConType;

typedef enum {
	flow_eCorner_Rounded,
	flow_eCorner_Right
	} flow_eCorner;

typedef enum {
	flow_eDrawType_Line,
	flow_eDrawType_LineRed,
	flow_eDrawType_LineGray,
	flow_eDrawType_LineErase,
	flow_eDrawType_LineDashed,
	flow_eDrawType_LineDashedRed,
	flow_eDrawType_TextHelvetica,
	flow_eDrawType_TextHelveticaBold,
	flow_eDrawType_TextHelveticaErase,
	flow_eDrawType_TextHelveticaEraseBold,
	flow_eDrawType__,
	flow_eDrawType_Green,
	flow_eDrawType_Yellow,
	flow_eDrawType_DarkGray,
	flow_eDrawType_Inherit = 9999
	} flow_eDrawType;

typedef enum {
        draw_eCursor_Normal,
        draw_eCursor_CrossHair,
        draw_eCursor_Hand,
        draw_eCursor_DiamondCross,
        draw_eCursor__
        } draw_eCursor;
                              
typedef enum {
	draw_eFont_HelveticaBold,
	draw_eFont_Helvetica,
	draw_eFont__
	} draw_eFont;

typedef enum {
	flow_eMoveType_Frozen,
	flow_eMoveType_Route
	} flow_eMoveType;

typedef enum {
	flow_eEventType_CallBack,
	flow_eEventType_RegionSelect,
	flow_eEventType_RegionAddSelect,
	flow_eEventType_CreateCon,
	flow_eEventType_CreateNode,
	flow_eEventType_MoveNode,
	flow_eEventType_Object,
	flow_eEventType_AnnotationInput,
	flow_eEventType_Radiobutton
	} flow_eEventType;

typedef enum {
	flow_eEvent_Null,
	flow_eEvent_MB1Click,
	flow_eEvent_MB1DoubleClick,
	flow_eEvent_MB1Press,
	flow_eEvent_MB2Click,
	flow_eEvent_MB2DoubleClick,
	flow_eEvent_MB2Press,
	flow_eEvent_MB1ClickShift,
	flow_eEvent_MB1DoubleClickShift,
	flow_eEvent_MB1PressShift,
	flow_eEvent_MB2ClickShift,
	flow_eEvent_MB2DoubleClickShift,
	flow_eEvent_MB2PressShift,
	flow_eEvent_MB1ClickCtrl,
	flow_eEvent_MB1DoubleClickCtrl,
	flow_eEvent_MB1PressCtrl,
	flow_eEvent_MB2ClickCtrl,
	flow_eEvent_MB2DoubleClickCtrl,
	flow_eEvent_MB2PressCtrl,
	flow_eEvent_MB1ClickShiftCtrl,
	flow_eEvent_MB1DoubleClickShiftCtrl,
	flow_eEvent_MB1PressShiftCtrl,
	flow_eEvent_MB2ClickShiftCtrl,
	flow_eEvent_MB2DoubleClickShiftCtrl,
	flow_eEvent_MB2PressShiftCtrl,
	flow_eEvent_MB3Click,
	flow_eEvent_MB3Press,
	flow_eEvent_ButtonRelease,
	flow_eEvent_ButtonMotion,
	flow_eEvent_Exposure,
	flow_eEvent_Enter,
	flow_eEvent_Leave,
	flow_eEvent_CursorMotion,
	flow_eEvent_Init,
	flow_eEvent_PasteSequenceStart,
	flow_eEvent_VisibilityUnobscured,
	flow_eEvent_VisibilityObscured,
	flow_eEvent_SelectClear,
	flow_eEvent_ObjectMoved,
	flow_eEvent_ObjectDeleted,
	flow_eEvent_AnnotationInput,
	flow_eEvent_Radiobutton,
	flow_eEvent_Key_Return,
	flow_eEvent_Key_Up,
	flow_eEvent_Key_Down,
	flow_eEvent_Key_Right,
	flow_eEvent_Key_Left,
	flow_eEvent_Key_PageUp,
	flow_eEvent_Key_PageDown,
	flow_eEvent_Key_BackSpace,
	flow_eEvent_Key_PF1,
	flow_eEvent_Key_PF2,
	flow_eEvent_Key_PF3,
	flow_eEvent_Key_PF4,
	flow_eEvent_Key_ShiftRight,
	flow_eEvent_Key_ShiftLeft,
	flow_eEvent_Key_ShiftUp,
	flow_eEvent_Key_ShiftDown,
	flow_eEvent_MB1Down,
	flow_eEvent_MB1Up,
	flow_eEvent_MB2Down,
	flow_eEvent_MB2Up,
	flow_eEvent_MB3Down,
	flow_eEvent_MB3Up,
	flow_eEvent_Key_Tab,
	flow_eEvent_Map,
	flow_eEvent_Unmap,
	flow_eEvent_Resized,
	flow_eEvent_ScrollUp,
	flow_eEvent_ScrollDown,
	flow_eEvent__
	} flow_eEvent;

typedef struct {
	flow_eEvent		event;
	flow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	} flow_sEventAny, *flow_tEventAny;

typedef struct {
	flow_eEvent		event;
	flow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	flow_eObjectType	object_type;
	void			*object;
	} flow_sEventObject, *flow_tEventObject;

typedef struct {
	flow_eEvent		event;
	flow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	void			*source_object;
	void			*dest_object;
	int			source_conpoint;
	int			dest_conpoint;
	} flow_sEventConCreate, *flow_tEventConCreate;

typedef struct {
	flow_eEvent		event;
	flow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	flow_eObjectType	object_type;
	void			*object;
	int			number;
	char			*text;
	} flow_sEventAnnotInput, *flow_tEventAnnotInput;

typedef struct {
	flow_eEvent		event;
	flow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	flow_eObjectType	object_type;
	void			*object;
	int			number;
	int			value;
	} flow_sEventRadiobutton, *flow_tEventRadiobutton;

typedef union {
	flow_eEvent		event;
	flow_sEventAny		any;
	flow_sEventObject	object;
	flow_sEventConCreate	con_create;
	flow_sEventAnnotInput	annot_input;
	flow_sEventRadiobutton	radiobutton;
	} flow_sEvent, *flow_tEvent;

typedef enum {
	flow_eType_Boolean,
	flow_eType_Int,
	flow_eType_Double,
	flow_eType_String,
	flow_eType_User
	} flow_eType;

typedef enum {
	flow_eTraceType_Boolean,
	flow_eTraceType_Int32,
	flow_eTraceType_Float32,
	flow_eTraceType_User
	} flow_eTraceType;

typedef enum {
	flow_eSaveMode_Edit,
	flow_eSaveMode_Trace
	} flow_eSaveMode;

typedef enum {
	flow_eSave_Ctx				= 1,
	flow_eSave_Array			= 2,
	flow_eSave_NodeClass			= 3,
	flow_eSave_ConClass			= 4,
	flow_eSave_Rect				= 5,
	flow_eSave_Line				= 6,
	flow_eSave_Point			= 7,
	flow_eSave_Arc				= 8,
	flow_eSave_Text				= 9,
	flow_eSave_Node				= 10,
	flow_eSave_Con				= 11,
	flow_eSave_ConPoint			= 12,
	flow_eSave_Annot			= 13,
	flow_eSave_Arrow			= 14,
	flow_eSave_Pixmap			= 15,
	flow_eSave_AnnotPixmap			= 16,
	flow_eSave_Radiobutton			= 17,
	flow_eSave_Frame			= 18,
	flow_eSave_End				= 99,
	flow_eSave_Ctx_zoom_factor		= 100,
	flow_eSave_Ctx_base_zoom_factor		= 101,
	flow_eSave_Ctx_offset_x			= 102,
	flow_eSave_Ctx_offset_y			= 103,
	flow_eSave_Ctx_nav_zoom_factor		= 104,
	flow_eSave_Ctx_print_zoom_factor	= 105,
	flow_eSave_Ctx_nav_offset_x		= 106,
	flow_eSave_Ctx_nav_offset_y		= 107,
	flow_eSave_Ctx_x_right			= 108,
	flow_eSave_Ctx_x_left			= 109,
	flow_eSave_Ctx_y_high			= 110,
	flow_eSave_Ctx_y_low			= 111,
	flow_eSave_Ctx_window_width		= 112,
	flow_eSave_Ctx_window_height		= 113,
	flow_eSave_Ctx_nav_window_width		= 114,
	flow_eSave_Ctx_nav_window_height	= 115,
	flow_eSave_Ctx_nav_rect_ll_x		= 116,
	flow_eSave_Ctx_nav_rect_ll_y		= 117,
	flow_eSave_Ctx_nav_rect_ur_x		= 118,
	flow_eSave_Ctx_nav_rect_ur_y		= 119,
	flow_eSave_Ctx_nav_rect_hot		= 120,
	flow_eSave_Ctx_name			= 121,
	flow_eSave_Ctx_user_highlight		= 122,
	flow_eSave_Ctx_a_nc			= 123,
	flow_eSave_Ctx_a_cc			= 124,
	flow_eSave_Ctx_a			= 125,
	flow_eSave_Ctx_grid_size_x		= 126,
	flow_eSave_Ctx_grid_size_y		= 127,
	flow_eSave_Ctx_grid_on			= 128,
	flow_eSave_Ctx_draw_delta		= 129,
	flow_eSave_Ctx_refcon_width		= 130,
	flow_eSave_Ctx_refcon_height		= 131,
	flow_eSave_Ctx_refcon_textsize		= 132,
	flow_eSave_Ctx_refcon_linewidth		= 133,
	flow_eSave_Array_a			= 200,
	flow_eSave_NodeClass_nc_name		= 300,
	flow_eSave_NodeClass_a			= 301,
	flow_eSave_NodeClass_group		= 302,
	flow_eSave_NodeClass_no_con_obstacle	= 303,
	flow_eSave_ConClass_cc_name		= 400,
	flow_eSave_ConClass_con_type		= 401,
	flow_eSave_ConClass_corner		= 402,
	flow_eSave_ConClass_draw_type		= 403,
	flow_eSave_ConClass_line_width		= 404,
	flow_eSave_ConClass_arrow_width		= 405,
	flow_eSave_ConClass_arrow_length	= 406,
	flow_eSave_ConClass_round_corner_amount	= 407,
	flow_eSave_ConClass_group		= 408,
	flow_eSave_Rect_draw_type		= 500,
	flow_eSave_Rect_line_width		= 501,
	flow_eSave_Rect_ll			= 502,
	flow_eSave_Rect_ur			= 503,
	flow_eSave_Rect_display_level		= 504,
	flow_eSave_Line_draw_type		= 600,
	flow_eSave_Line_line_width		= 601,
	flow_eSave_Line_p1			= 602,
	flow_eSave_Line_p2			= 603,
	flow_eSave_Point_x			= 700,
	flow_eSave_Point_y			= 701,
	flow_eSave_Arc_angel1			= 800,
	flow_eSave_Arc_angel2			= 801,
	flow_eSave_Arc_draw_type		= 802,
	flow_eSave_Arc_line_width		= 803,
	flow_eSave_Arc_ll			= 804,
	flow_eSave_Arc_ur			= 805,
	flow_eSave_Text_text_size		= 900,
	flow_eSave_Text_draw_type		= 901,
	flow_eSave_Text_text			= 902,
	flow_eSave_Text_p			= 903,
	flow_eSave_Node_nc			= 1000,
	flow_eSave_Node_pos			= 1001,
	flow_eSave_Node_n_name			= 1002,
	flow_eSave_Node_annotsize		= 1003,
	flow_eSave_Node_annotv			= 1004,
	flow_eSave_Node_refcon_cnt		= 1005,
	flow_eSave_Node_x_right			= 1006,
	flow_eSave_Node_x_left			= 1007,
	flow_eSave_Node_y_high			= 1008,
	flow_eSave_Node_y_low			= 1009,
	flow_eSave_Node_trace_object		= 1010,
	flow_eSave_Node_trace_attribute		= 1011,
	flow_eSave_Node_trace_attr_type		= 1012,
	flow_eSave_Node_obst_x_right		= 1013,
	flow_eSave_Node_obst_x_left		= 1014,
	flow_eSave_Node_obst_y_high		= 1015,
	flow_eSave_Node_obst_y_low		= 1016,
	flow_eSave_Node_trace_inverted		= 1017,
	flow_eSave_Con_x_right			= 1100,
	flow_eSave_Con_x_left			= 1101,
	flow_eSave_Con_y_high			= 1102,
	flow_eSave_Con_y_low			= 1103,
	flow_eSave_Con_cc			= 1104,
	flow_eSave_Con_dest_node		= 1105,
	flow_eSave_Con_source_node		= 1106,
	flow_eSave_Con_dest_conpoint		= 1107,
	flow_eSave_Con_source_conpoint		= 1108,
	flow_eSave_Con_dest_direction		= 1109,
	flow_eSave_Con_source_direction		= 1110,
	flow_eSave_Con_line_a			= 1111,
	flow_eSave_Con_arc_a			= 1112,
	flow_eSave_Con_arrow_a			= 1113,
	flow_eSave_Con_ref_a			= 1114,
	flow_eSave_Con_p_num			= 1115,
	flow_eSave_Con_l_num			= 1116,
	flow_eSave_Con_a_num			= 1117,
	flow_eSave_Con_arrow_num		= 1118,
	flow_eSave_Con_ref_num			= 1119,
	flow_eSave_Con_point_x			= 1120,
	flow_eSave_Con_point_y			= 1121,
	flow_eSave_Con_source_ref_cnt		= 1122,
	flow_eSave_Con_dest_ref_cnt		= 1123,
	flow_eSave_Con_c_name			= 1124,
	flow_eSave_Con_trace_object		= 1125,
	flow_eSave_Con_trace_attribute		= 1126,
	flow_eSave_Con_trace_attr_type		= 1127,
	flow_eSave_Con_temporary_ref		= 1128,
	flow_eSave_ConPoint_number		= 1200,
	flow_eSave_ConPoint_direction		= 1201,
	flow_eSave_ConPoint_p			= 1202,
	flow_eSave_ConPoint_trace_attribute	= 1203,
	flow_eSave_ConPoint_trace_attr_type	= 1204,
	flow_eSave_Annot_number			= 1300,
	flow_eSave_Annot_draw_type		= 1301,
	flow_eSave_Annot_text_size		= 1302,
	flow_eSave_Annot_p			= 1303,
	flow_eSave_Annot_annot_type		= 1304,
	flow_eSave_Annot_display_level		= 1305,
	flow_eSave_Arrow_arrow_width		= 1400,
	flow_eSave_Arrow_arrow_length		= 1401,
	flow_eSave_Arrow_draw_type		= 1402,
	flow_eSave_Arrow_line_width		= 1403,
	flow_eSave_Arrow_p_dest			= 1404,
	flow_eSave_Arrow_p1			= 1405,
	flow_eSave_Arrow_p2			= 1406
	} flow_eSave;

typedef enum {
	flow_ePosition_Absolute,
	flow_ePosition_Relative
	} flow_ePosition;

typedef struct {
	int			width;
	int			height;
	char			*bits;
	} flow_sPixmapDataElem;

typedef flow_sPixmapDataElem flow_sPixmapData[DRAW_PIXMAP_SIZE];

typedef struct {
	flow_sPixmapData	pixmap_data;
	void			*pixmaps;
	} flow_sAnnotPixmap;

typedef enum {
  flow_eDest__ = 0,
  flow_eDest_IntoFirst,  /* Object inserted as first son. */
  flow_eDest_IntoLast,   /* Object inserted as last son. */
  flow_eDest_After,      /* Object inserted after destination object.*/
  flow_eDest_Before,     /* Object inserted before destination object. */
  flow_eDest_
} flow_eDest;


typedef enum {
	flow_mDisplayLevel_1		= 1 << 0,
	flow_mDisplayLevel_2		= 1 << 2,
	flow_mDisplayLevel_3		= 1 << 3,
	flow_mDisplayLevel_4		= 1 << 4,
	flow_mDisplayLevel_5		= 1 << 5,
	flow_mDisplayLevel_6		= 1 << 6
	} flow_mDisplayLevel;

typedef enum {
	flow_mSensitivity_MB1Click       = 1 << 0,
	flow_mSensitivity_MB1DoubleClick = 1 << 1,
	flow_mSensitivity_MB1Press       = 1 << 2,
	flow_mSensitivity_MB2Click       = 1 << 3,
	flow_mSensitivity_MB2DoubleClick = 1 << 4,
	flow_mSensitivity_MB2Press       = 1 << 5,
	flow_mSensitivity_MB3Click       = 1 << 6,
	flow_mSensitivity_MB3DoubleClick = 1 << 7,
	flow_mSensitivity_MB3Press       = 1 << 8
	} flow_mSensitivity;


#if defined __cplusplus
}
#endif
#endif
