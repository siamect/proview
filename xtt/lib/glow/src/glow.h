#ifndef glow_h
#define glow_h

#if defined __cplusplus
extern "C" {
#endif


#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define DRAW_MP 6

#define DRAW_TYPE_SIZE 9
#define DRAW_FONT_SIZE 9
#define DRAW_PIXMAP_SIZE 9
#define TREND_MAX_CURVES 11

#define FSPACE " "

typedef enum {
	glow_eType_Boolean,
	glow_eType_Int,
	glow_eType_Double,
	glow_eType_String,
	glow_eType_TraceType,
	glow_eType_Direction,
	glow_eType_Color,
	glow_eType_Tone,
	glow_eType_ToneOrColor,
	glow_eType_TraceColor,
	glow_eType_Access,
	glow_eType_Cycle,
	glow_eType_MB3Action
	} glow_eType;

typedef enum {
	glow_eCtxType_Glow,
	glow_eCtxType_Brow,
	glow_eCtxType_Grow,
	glow_eCtxType_ColPal,
	glow_eCtxType_Curve
	} glow_eCtxType;

typedef enum {
	glow_eSelectPolicy_Surround,
	glow_eSelectPolicy_Partial,
	glow_eSelectPolicy_Both
	} glow_eSelectPolicy;

typedef enum {
	glow_eMB3Action_No,
	glow_eMB3Action_Close,
	glow_eMB3Action_PopupMenu,
	glow_eMB3Action_Both
	} glow_eMB3Action;

typedef enum {
	glow_eHotMode_Default,
	glow_eHotMode_Disabled,
	glow_eHotMode_SingleObject,
	glow_eHotMode_TraceAction
	} glow_eHotMode;

typedef enum {
	glow_eObjectType_NoObject,
	glow_eObjectType_Node,
	glow_eObjectType_Con,
	glow_eObjectType_Rect,
	glow_eObjectType_Line,
	glow_eObjectType_Arc,
	glow_eObjectType_Text,
	glow_eObjectType_ConPoint,
	glow_eObjectType_Annot,
	glow_eObjectType_NodeClass,
	glow_eObjectType_ConClass,
	glow_eObjectType_Arrow,
	glow_eObjectType_Pixmap,
	glow_eObjectType_AnnotPixmap,
	glow_eObjectType_Radiobutton,
	glow_eObjectType_Frame,
	glow_eObjectType_GrowRect,
	glow_eObjectType_GrowLine,
	glow_eObjectType_GrowArc,
	glow_eObjectType_GrowConPoint,
	glow_eObjectType_GrowSubAnnot,
	glow_eObjectType_PolyLine,
	glow_eObjectType_GrowPolyLine,
	glow_eObjectType_Point,
	glow_eObjectType_GrowNode,
	glow_eObjectType_GrowAnnot,
	glow_eObjectType_GrowText,
	glow_eObjectType_GrowBar,
	glow_eObjectType_GrowTrend,
	glow_eObjectType_GrowSlider,
	glow_eObjectType_GrowImage,
	glow_eObjectType_NodeGroup,
	glow_eObjectType_GrowGroup,
        glow_eObjectType_GrowAxis
	} glow_eObjectType;

typedef enum {
	glow_eDirection_Center,
	glow_eDirection_Right,
	glow_eDirection_Left,
	glow_eDirection_Up,
	glow_eDirection_Down
	} glow_eDirection;

typedef enum {
	glow_eAlignDirection_CenterCenter,
	glow_eAlignDirection_Right,
	glow_eAlignDirection_Left,
	glow_eAlignDirection_Up,
	glow_eAlignDirection_Down,
	glow_eAlignDirection_CenterVert,
	glow_eAlignDirection_CenterHoriz
	} glow_eAlignDirection;

typedef enum {
	glow_eNodeGroup_Common,
	glow_eNodeGroup_Document,
	glow_eNodeGroup_Trace
	} glow_eNodeGroup;

typedef enum {
	glow_eConGroup_Common,
	glow_eConGroup_Trace
	} glow_eConGroup;

typedef enum {
	glow_eMoveRestriction_No,
	glow_eMoveRestriction_Disable,
	glow_eMoveRestriction_Vertical,
	glow_eMoveRestriction_Horizontal,
	glow_eMoveRestriction_VerticalSlider,
	glow_eMoveRestriction_HorizontalSlider
	} glow_eMoveRestriction;

typedef enum {
	glow_eAnnotType_OneLine,
	glow_eAnnotType_MultiLine
	} glow_eAnnotType;

typedef enum {
	glow_eCycle_Inherit,
	glow_eCycle_Slow,
	glow_eCycle_Fast
	} glow_eCycle;

typedef enum {
	glow_eExportPass_Init,
	glow_eExportPass_Shape,
	glow_eExportPass_Draw,
	glow_eExportPass_Declare,
	glow_eExportPass_Attributes,
	glow_eExportPass_End
	} glow_eExportPass;

typedef enum {
	glow_eConType_Straight,
	glow_eConType_Fixed,
	glow_eConType_AllFixed,
	glow_eConType_Routed,
	glow_eConType_StepDiv,
	glow_eConType_StepConv,
	glow_eConType_TransDiv,
	glow_eConType_TransConv,
	glow_eConType_StraightOneArrow,
	glow_eConType_Reference
	} glow_eConType;

typedef enum {
	glow_eCorner_Rounded,
	glow_eCorner_Right
	} glow_eCorner;

typedef enum {
	glow_eDynamicType_Object,
	glow_eDynamicType_SubGraph,
	glow_eDynamicType_SubGraphEnd
	} glow_eDynamicType;

typedef enum {
	glow_eDrawType_Line,
	glow_eDrawType_LineRed,
	glow_eDrawType_LineGray,
	glow_eDrawType_Color4,
	glow_eDrawType_Color5,
	glow_eDrawType_Color6,
	glow_eDrawType_Color7,
	glow_eDrawType_Color8,
	glow_eDrawType_Color9,
	glow_eDrawType_Color10,
	glow_eDrawType_Color11,
	glow_eDrawType_Color12,
	glow_eDrawType_Color13,
	glow_eDrawType_Color14,
	glow_eDrawType_Color15,
	glow_eDrawType_Color16,
	glow_eDrawType_Color17,
	glow_eDrawType_Color18,
	glow_eDrawType_Color19,
	glow_eDrawType_Color20,
	glow_eDrawType_Color21,
	glow_eDrawType_Color22,
	glow_eDrawType_Color23,
	glow_eDrawType_Color24,
	glow_eDrawType_Color25,
	glow_eDrawType_Color26,
	glow_eDrawType_Color27,
	glow_eDrawType_Color28,
	glow_eDrawType_Color29,
	glow_eDrawType_Color30,
	glow_eDrawType_Color31,
	glow_eDrawType_Color32,
	glow_eDrawType_Color33,
	glow_eDrawType_Color34,
	glow_eDrawType_Color35,
	glow_eDrawType_Color36,
	glow_eDrawType_Color37,
	glow_eDrawType_Color38,
	glow_eDrawType_Color39,
	glow_eDrawType_Color40,
	glow_eDrawType_Color41,
	glow_eDrawType_Color42,
	glow_eDrawType_Color43,
	glow_eDrawType_Color44,
	glow_eDrawType_Color45,
	glow_eDrawType_Color46,
	glow_eDrawType_Color47,
	glow_eDrawType_Color48,
	glow_eDrawType_Color49,
	glow_eDrawType_Color50,
	glow_eDrawType_Color51,
	glow_eDrawType_Color52,
	glow_eDrawType_Color53,
	glow_eDrawType_Color54,
	glow_eDrawType_Color55,
	glow_eDrawType_Color56,
	glow_eDrawType_Color57,
	glow_eDrawType_Color58,
	glow_eDrawType_Color59,
	glow_eDrawType_Color60,
	glow_eDrawType_Color61,
	glow_eDrawType_Color62,
	glow_eDrawType_Color63,
	glow_eDrawType_Color64,
	glow_eDrawType_Color65,
	glow_eDrawType_Color66,
	glow_eDrawType_Color67,
	glow_eDrawType_Color68,
	glow_eDrawType_Color69,
	glow_eDrawType_Color70,
	glow_eDrawType_Color71,
	glow_eDrawType_Color72,
	glow_eDrawType_Color73,
	glow_eDrawType_Color74,
	glow_eDrawType_Color75,
	glow_eDrawType_Color76,
	glow_eDrawType_Color77,
	glow_eDrawType_Color78,
	glow_eDrawType_Color79,
	glow_eDrawType_Color80,
	glow_eDrawType_Color81,
	glow_eDrawType_Color82,
	glow_eDrawType_Color83,
	glow_eDrawType_Color84,
	glow_eDrawType_Color85,
	glow_eDrawType_Color86,
	glow_eDrawType_Color87,
	glow_eDrawType_Color88,
	glow_eDrawType_Color89,
	glow_eDrawType_Color90,
	glow_eDrawType_Color91,
	glow_eDrawType_Color92,
	glow_eDrawType_Color93,
	glow_eDrawType_Color94,
	glow_eDrawType_Color95,
	glow_eDrawType_Color96,
	glow_eDrawType_Color97,
	glow_eDrawType_Color98,
	glow_eDrawType_Color99,
	glow_eDrawType_Color100,
	glow_eDrawType_LineErase,
	glow_eDrawType_LineDashed,
	glow_eDrawType_LineDashedRed,
	glow_eDrawType_TextHelvetica,
	glow_eDrawType_TextHelveticaBold,
	glow_eDrawType_TextHelveticaErase,
	glow_eDrawType_TextHelveticaEraseBold,
	glow_eDrawType__,
	glow_eDrawType_Inherit = 9999,
	glow_eDrawType_No = 10000
	} glow_eDrawType;

typedef enum {
	glow_eDrawTone_No,
	glow_eDrawTone_Gray,
	glow_eDrawTone_Yellow,
	glow_eDrawTone_Gold,
	glow_eDrawTone_Orange,
	glow_eDrawTone_Red,
	glow_eDrawTone_Magenta,
	glow_eDrawTone_Blue,
	glow_eDrawTone_Seablue,
	glow_eDrawTone_Green,
	glow_eDrawTone_DarkGray,
	glow_eDrawTone__
	} glow_eDrawTone;

typedef enum {
        glow_eDrawCursor_Normal,
        glow_eDrawCursor_CrossHair,
        glow_eDrawCursor_Hand,
        glow_eDrawCursor_DiamondCross,
        glow_eDrawCursor__
        } glow_eDrawCursor;
                              
typedef enum {
	glow_eDrawFont_HelveticaBold,
	glow_eDrawFont_Helvetica,
	glow_eDrawFont__
	} glow_eDrawFont;

typedef enum {
	glow_eMoveType_Frozen,
	glow_eMoveType_Route
	} glow_eMoveType;

typedef enum {
	glow_eScaleType_LowerLeft,
	glow_eScaleType_LowerRight,
	glow_eScaleType_UpperRight,
	glow_eScaleType_UpperLeft,
	glow_eScaleType_Left,
	glow_eScaleType_Up,
	glow_eScaleType_Right,
	glow_eScaleType_Down,
	glow_eScaleType_Center,
	glow_eScaleType_FixPoint,
	glow_eScaleType_No
	} glow_eScaleType;

typedef enum {
	glow_eRotationPoint_LowerLeft,
	glow_eRotationPoint_LowerRight,
	glow_eRotationPoint_UpperRight,
	glow_eRotationPoint_UpperLeft,
	glow_eRotationPoint_Center,
	glow_eRotationPoint_FixPoint,
	glow_eRotationPoint_Zero
	} glow_eRotationPoint;

typedef enum {
	glow_eEventType_CallBack,
	glow_eEventType_RegionSelect,
	glow_eEventType_RegionAddSelect,
	glow_eEventType_CreateCon,
	glow_eEventType_CreateNode,
	glow_eEventType_MoveNode,
	glow_eEventType_Object,
	glow_eEventType_AnnotationInput,
	glow_eEventType_Radiobutton,
	glow_eEventType_CreateGrowObject,
	glow_eEventType_Dynamics,
	glow_eEventType_ColorTone,
	glow_eEventType_Translate
	} glow_eEventType;

typedef enum {
	glow_eEvent_Null,
	glow_eEvent_MB1Click,
	glow_eEvent_MB1DoubleClick,
	glow_eEvent_MB1Press,
	glow_eEvent_MB2Click,
	glow_eEvent_MB2DoubleClick,
	glow_eEvent_MB2Press,
	glow_eEvent_MB1ClickShift,
	glow_eEvent_MB1DoubleClickShift,
	glow_eEvent_MB1PressShift,
	glow_eEvent_MB2ClickShift,
	glow_eEvent_MB2DoubleClickShift,
	glow_eEvent_MB2PressShift,
	glow_eEvent_MB1ClickCtrl,
	glow_eEvent_MB1DoubleClickCtrl,
	glow_eEvent_MB1PressCtrl,
	glow_eEvent_MB2ClickCtrl,
	glow_eEvent_MB2DoubleClickCtrl,
	glow_eEvent_MB2PressCtrl,
	glow_eEvent_MB1ClickShiftCtrl,
	glow_eEvent_MB1DoubleClickShiftCtrl,
	glow_eEvent_MB1PressShiftCtrl,
	glow_eEvent_MB2ClickShiftCtrl,
	glow_eEvent_MB2DoubleClickShiftCtrl,
	glow_eEvent_MB2PressShiftCtrl,
	glow_eEvent_MB3Click,
	glow_eEvent_MB3Press,
	glow_eEvent_ButtonRelease,
	glow_eEvent_ButtonMotion,
	glow_eEvent_Exposure,
	glow_eEvent_Enter,
	glow_eEvent_Leave,
	glow_eEvent_CursorMotion,
	glow_eEvent_Init,
	glow_eEvent_PasteSequenceStart,
	glow_eEvent_VisibilityUnobscured,
	glow_eEvent_VisibilityObscured,
	glow_eEvent_SelectClear,
	glow_eEvent_ObjectMoved,
	glow_eEvent_ObjectDeleted,
	glow_eEvent_AnnotationInput,
	glow_eEvent_Radiobutton,
	glow_eEvent_Key_Return,
	glow_eEvent_Key_Up,
	glow_eEvent_Key_Down,
	glow_eEvent_Key_Right,
	glow_eEvent_Key_Left,
	glow_eEvent_Key_PageUp,
	glow_eEvent_Key_PageDown,
	glow_eEvent_Key_BackSpace,
	glow_eEvent_Key_PF1,
	glow_eEvent_Key_PF2,
	glow_eEvent_Key_PF3,
	glow_eEvent_Key_PF4,
	glow_eEvent_CreateGrowObject,
	glow_eEvent_GrowDynamics,
	glow_eEvent_SliderMoveStart,
	glow_eEvent_SliderMoved,
	glow_eEvent_HotRequest,
	glow_eEvent_MB1Down,
	glow_eEvent_MB1Up,
	glow_eEvent_MB2Down,
	glow_eEvent_MB2Up,
	glow_eEvent_MB3Down,
	glow_eEvent_MB3Up,
	glow_eEvent_Key_Tab,
	glow_eEvent_Map,
	glow_eEvent_Unmap,
	glow_eEvent_Resized,
	glow_eEvent_Translate,
	glow_eEvent__
	} glow_eEvent;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	} glow_sEventAny, *glow_tEventAny;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eObjectType	object_type;
	void			*object;
	} glow_sEventObject, *glow_tEventObject;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	void			*source_object;
	void			*dest_object;
	int			source_conpoint;
	int			dest_conpoint;
	} glow_sEventConCreate, *glow_tEventConCreate;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eObjectType	object_type;
	void			*object;
	int			number;
	char			*text;
	} glow_sEventAnnotInput, *glow_tEventAnnotInput;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eObjectType	object_type;
	void			*object;
	int			number;
	int			value;
	} glow_sEventRadiobutton, *glow_tEventRadiobutton;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	double			x2;
	double			y2;
	int			mode;
	int			first_line;
	} glow_sEventCreateGrowObject, *glow_tEventCreateGrowObject;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eObjectType	object_type;
	void			*object;
	char			*code;
	glow_eDynamicType	dynamic_type;
	} glow_sEventDynamics, *glow_tEventDynamics;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eDrawTone		tone;
	} glow_sEventColorTone, *glow_tEventColorTone;

typedef struct {
	glow_eEvent		event;
	glow_eEventType		type;
	int			x_pixel;
	int			y_pixel;
	double			x;
	double			y;
	glow_eObjectType	object_type;
	void			*object;
        char		        *text;
        char		        *new_text;
	} glow_sEventTranslate, *glow_tEventTranslate;

typedef union {
	glow_eEvent		event;
	glow_sEventAny		any;
	glow_sEventObject	object;
	glow_sEventConCreate	con_create;
	glow_sEventAnnotInput	annot_input;
	glow_sEventRadiobutton	radiobutton;
	glow_sEventCreateGrowObject create_grow_object;
	glow_sEventDynamics	dynamics;
	glow_sEventColorTone	colortone;
	glow_sEventTranslate	translate;
	} glow_sEvent, *glow_tEvent;

typedef enum {
	glow_eTraceType_Boolean,
	glow_eTraceType_Int32,
	glow_eTraceType_Float32,
	glow_eTraceType_User
	} glow_eTraceType;

typedef enum {
	glow_eSaveMode_Edit,
	glow_eSaveMode_Trace,
	glow_eSaveMode_SubGraph
	} glow_eSaveMode;

typedef enum {
	glow_eSave_Ctx				= 1,
	glow_eSave_Array			= 2,
	glow_eSave_NodeClass			= 3,
	glow_eSave_ConClass			= 4,
	glow_eSave_Rect				= 5,
	glow_eSave_Line				= 6,
	glow_eSave_Point			= 7,
	glow_eSave_Arc				= 8,
	glow_eSave_Text				= 9,
	glow_eSave_Node				= 10,
	glow_eSave_Con				= 11,
	glow_eSave_ConPoint			= 12,
	glow_eSave_Annot			= 13,
	glow_eSave_Arrow			= 14,
	glow_eSave_Pixmap			= 15,
	glow_eSave_AnnotPixmap			= 16,
	glow_eSave_Radiobutton			= 17,
	glow_eSave_Frame			= 18,
	glow_eSave_GrowRect			= 19,
	glow_eSave_GrowLine			= 20,
	glow_eSave_GrowConPoint			= 21,
	glow_eSave_GrowCtx			= 22,
	glow_eSave_GrowSubAnnot			= 23,
	glow_eSave_GrowArc			= 24,
	glow_eSave_PolyLine			= 25,
	glow_eSave_GrowPolyLine			= 26,
	glow_eSave_GrowNode			= 27,
	glow_eSave_Transform			= 28,
	glow_eSave_GrowAnnot			= 29,
	glow_eSave_GrowText			= 30,
	glow_eSave_GrowBar			= 31,
	glow_eSave_GrowTrend			= 32,
	glow_eSave_GrowSlider			= 33,
	glow_eSave_GrowImage			= 34,
	glow_eSave_GrowGroup			= 35,
	glow_eSave_NodeGroup		        = 36,
	glow_eSave_GrowAxis		        = 37,
	glow_eSave_End				= 99,
	glow_eSave_Ctx_zoom_factor_x		= 100,
	glow_eSave_Ctx_base_zoom_factor		= 101,
	glow_eSave_Ctx_offset_x			= 102,
	glow_eSave_Ctx_offset_y			= 103,
	glow_eSave_Ctx_nav_zoom_factor_x	= 104,
	glow_eSave_Ctx_print_zoom_factor	= 105,
	glow_eSave_Ctx_nav_offset_x		= 106,
	glow_eSave_Ctx_nav_offset_y		= 107,
	glow_eSave_Ctx_x_right			= 108,
	glow_eSave_Ctx_x_left			= 109,
	glow_eSave_Ctx_y_high			= 110,
	glow_eSave_Ctx_y_low			= 111,
	glow_eSave_Ctx_window_width		= 112,
	glow_eSave_Ctx_window_height		= 113,
	glow_eSave_Ctx_nav_window_width		= 114,
	glow_eSave_Ctx_nav_window_height	= 115,
	glow_eSave_Ctx_nav_rect_ll_x		= 116,
	glow_eSave_Ctx_nav_rect_ll_y		= 117,
	glow_eSave_Ctx_nav_rect_ur_x		= 118,
	glow_eSave_Ctx_nav_rect_ur_y		= 119,
	glow_eSave_Ctx_nav_rect_hot		= 120,
	glow_eSave_Ctx_name			= 121,
	glow_eSave_Ctx_user_highlight		= 122,
	glow_eSave_Ctx_a_nc			= 123,
	glow_eSave_Ctx_a_cc			= 124,
	glow_eSave_Ctx_a			= 125,
	glow_eSave_Ctx_grid_size_x		= 126,
	glow_eSave_Ctx_grid_size_y		= 127,
	glow_eSave_Ctx_grid_on			= 128,
	glow_eSave_Ctx_draw_delta		= 129,
	glow_eSave_Ctx_refcon_width		= 130,
	glow_eSave_Ctx_refcon_height		= 131,
	glow_eSave_Ctx_refcon_textsize		= 132,
	glow_eSave_Ctx_refcon_linewidth		= 133,
	glow_eSave_Ctx_grow		        = 134,
	glow_eSave_Ctx_zoom_factor_y	        = 135,
	glow_eSave_Ctx_nav_zoom_factor_y	= 136,
	glow_eSave_Array_a			= 200,
	glow_eSave_NodeClass_nc_name		= 300,
	glow_eSave_NodeClass_a			= 301,
	glow_eSave_NodeClass_group		= 302,
	glow_eSave_NodeClass_dynamic		= 303,
	glow_eSave_NodeClass_dynamicsize	= 304,
	glow_eSave_NodeClass_arg_cnt		= 305,
	glow_eSave_NodeClass_argname		= 306,
	glow_eSave_NodeClass_argtype		= 307,
	glow_eSave_NodeClass_trace_attr_type	= 308,
	glow_eSave_NodeClass_trace_color	= 309,
	glow_eSave_NodeClass_no_con_obstacle	= 310,
	glow_eSave_NodeClass_slider		= 311,
	glow_eSave_NodeClass_java_name		= 312,
	glow_eSave_NodeClass_trace_color2	= 313,
	glow_eSave_NodeClass_next_nodeclass	= 314,
	glow_eSave_NodeClass_animation_count	= 315,
	glow_eSave_NodeClass_cycle	        = 316,
	glow_eSave_NodeClass_y0	                = 317,
	glow_eSave_NodeClass_y1	                = 318,
	glow_eSave_NodeClass_x0	                = 319,
	glow_eSave_NodeClass_x1	                = 320,
	glow_eSave_ConClass_cc_name		= 400,
	glow_eSave_ConClass_con_type		= 401,
	glow_eSave_ConClass_corner		= 402,
	glow_eSave_ConClass_draw_type		= 403,
	glow_eSave_ConClass_line_width		= 404,
	glow_eSave_ConClass_arrow_width		= 405,
	glow_eSave_ConClass_arrow_length	= 406,
	glow_eSave_ConClass_round_corner_amount	= 407,
	glow_eSave_ConClass_group		= 408,
	glow_eSave_Rect_draw_type		= 500,
	glow_eSave_Rect_line_width		= 501,
	glow_eSave_Rect_ll			= 502,
	glow_eSave_Rect_ur			= 503,
	glow_eSave_Rect_display_level		= 504,
	glow_eSave_Rect_fill			= 505,
	glow_eSave_Line_draw_type		= 600,
	glow_eSave_Line_line_width		= 601,
	glow_eSave_Line_p1			= 602,
	glow_eSave_Line_p2			= 603,
	glow_eSave_Point_x			= 700,
	glow_eSave_Point_y			= 701,
	glow_eSave_Arc_angel1			= 800,
	glow_eSave_Arc_angel2			= 801,
	glow_eSave_Arc_draw_type		= 802,
	glow_eSave_Arc_line_width		= 803,
	glow_eSave_Arc_ll			= 804,
	glow_eSave_Arc_ur			= 805,
	glow_eSave_Arc_fill			= 806,
	glow_eSave_Text_text_size		= 900,
	glow_eSave_Text_draw_type		= 901,
	glow_eSave_Text_text			= 902,
	glow_eSave_Text_p			= 903,
	glow_eSave_Node_nc			= 1000,
	glow_eSave_Node_pos			= 1001,
	glow_eSave_Node_n_name			= 1002,
	glow_eSave_Node_annotsize		= 1003,
	glow_eSave_Node_annotv			= 1004,
	glow_eSave_Node_refcon_cnt		= 1005,
	glow_eSave_Node_x_right			= 1006,
	glow_eSave_Node_x_left			= 1007,
	glow_eSave_Node_y_high			= 1008,
	glow_eSave_Node_y_low			= 1009,
	glow_eSave_Node_trace_data1		= 1010,
	glow_eSave_Node_trace_data2		= 1011,
	glow_eSave_Node_trace_attr_type		= 1012,
	glow_eSave_Node_obst_x_right		= 1013,
	glow_eSave_Node_obst_x_left		= 1014,
	glow_eSave_Node_obst_y_high		= 1015,
	glow_eSave_Node_obst_y_low		= 1016,
	glow_eSave_Node_trace_color		= 1017,
	glow_eSave_Node_trace_data3		= 1018,
	glow_eSave_Node_trace_data4		= 1019,
	glow_eSave_Node_trace_data5		= 1020,
	glow_eSave_Node_trace_data6		= 1021,
	glow_eSave_Node_trace_data7		= 1022,
	glow_eSave_Node_trace_data8		= 1023,
	glow_eSave_Node_trace_data9		= 1024,
	glow_eSave_Node_trace_data10		= 1025,
	glow_eSave_Node_access			= 1026,
	glow_eSave_Node_trace_color2		= 1027,
	glow_eSave_Node_cycle		        = 1028,
	glow_eSave_Node_ref_object		= 1029,
	glow_eSave_Con_x_right			= 1100,
	glow_eSave_Con_x_left			= 1101,
	glow_eSave_Con_y_high			= 1102,
	glow_eSave_Con_y_low			= 1103,
	glow_eSave_Con_cc			= 1104,
	glow_eSave_Con_dest_node		= 1105,
	glow_eSave_Con_source_node		= 1106,
	glow_eSave_Con_dest_conpoint		= 1107,
	glow_eSave_Con_source_conpoint		= 1108,
	glow_eSave_Con_dest_direction		= 1109,
	glow_eSave_Con_source_direction		= 1110,
	glow_eSave_Con_line_a			= 1111,
	glow_eSave_Con_arc_a			= 1112,
	glow_eSave_Con_arrow_a			= 1113,
	glow_eSave_Con_ref_a			= 1114,
	glow_eSave_Con_p_num			= 1115,
	glow_eSave_Con_l_num			= 1116,
	glow_eSave_Con_a_num			= 1117,
	glow_eSave_Con_arrow_num		= 1118,
	glow_eSave_Con_ref_num			= 1119,
	glow_eSave_Con_point_x			= 1120,
	glow_eSave_Con_point_y			= 1121,
	glow_eSave_Con_source_ref_cnt		= 1122,
	glow_eSave_Con_dest_ref_cnt		= 1123,
	glow_eSave_Con_c_name			= 1124,
	glow_eSave_Con_trace_object		= 1125,
	glow_eSave_Con_trace_attribute		= 1126,
	glow_eSave_Con_trace_attr_type		= 1127,
	glow_eSave_Con_temporary_ref		= 1128,
	glow_eSave_ConPoint_number		= 1200,
	glow_eSave_ConPoint_direction		= 1201,
	glow_eSave_ConPoint_p			= 1202,
	glow_eSave_ConPoint_trace_attribute	= 1203,
	glow_eSave_ConPoint_trace_attr_type	= 1204,
	glow_eSave_ConPoint_trf			= 1205,
	glow_eSave_Annot_number			= 1300,
	glow_eSave_Annot_draw_type		= 1301,
	glow_eSave_Annot_text_size		= 1302,
	glow_eSave_Annot_p			= 1303,
	glow_eSave_Annot_annot_type		= 1304,
	glow_eSave_Annot_display_level		= 1305,
	glow_eSave_Arrow_arrow_width		= 1400,
	glow_eSave_Arrow_arrow_length		= 1401,
	glow_eSave_Arrow_draw_type		= 1402,
	glow_eSave_Arrow_line_width		= 1403,
	glow_eSave_Arrow_p_dest			= 1404,
	glow_eSave_Arrow_p1			= 1405,
	glow_eSave_Arrow_p2			= 1406,
	glow_eSave_GrowRect_x_right		= 1900,
	glow_eSave_GrowRect_x_left		= 1901,
	glow_eSave_GrowRect_y_high		= 1902,
	glow_eSave_GrowRect_y_low		= 1903,
	glow_eSave_GrowRect_n_name		= 1904,
	glow_eSave_GrowRect_rect_part		= 1905,
	glow_eSave_GrowRect_dynamic		= 1906,
	glow_eSave_GrowRect_dynamicsize		= 1907,
	glow_eSave_GrowRect_original_border_drawtype = 1908,
	glow_eSave_GrowRect_original_fill_drawtype = 1909,
	glow_eSave_GrowRect_fill_drawtype 	= 1910,
	glow_eSave_GrowRect_border 		= 1911,
	glow_eSave_GrowRect_trf 		= 1912,
	glow_eSave_GrowLine_x_right		= 2000,
	glow_eSave_GrowLine_x_left		= 2001,
	glow_eSave_GrowLine_y_high		= 2002,
	glow_eSave_GrowLine_y_low		= 2003,
	glow_eSave_GrowLine_n_name		= 2004,
	glow_eSave_GrowLine_line_part		= 2005,
	glow_eSave_GrowLine_dynamic		= 2006,
	glow_eSave_GrowLine_dynamicsize		= 2007,
	glow_eSave_GrowLine_trf			= 2008,
	glow_eSave_GrowLine_original_border_drawtype = 2009,
	glow_eSave_GrowConPoint_x_right		= 2100,
	glow_eSave_GrowConPoint_x_left		= 2101,
	glow_eSave_GrowConPoint_y_high		= 2102,
	glow_eSave_GrowConPoint_y_low		= 2103,
	glow_eSave_GrowConPoint_n_name		= 2104,
	glow_eSave_GrowConPoint_arc		= 2105,
	glow_eSave_GrowConPoint_conpoint_part	= 2106,
	glow_eSave_GrowCtx_conpoint_num_cnt	= 2200,
	glow_eSave_GrowCtx_objectname_cnt	= 2201,
	glow_eSave_GrowCtx_name			= 2202,
	glow_eSave_GrowCtx_background_color	= 2203,
	glow_eSave_GrowCtx_dynamic		= 2204,
	glow_eSave_GrowCtx_dynamicsize		= 2205,
	glow_eSave_GrowCtx_arg_cnt		= 2206,
	glow_eSave_GrowCtx_argname		= 2207,
	glow_eSave_GrowCtx_argtype		= 2208,
	glow_eSave_GrowCtx_x0			= 2209,
	glow_eSave_GrowCtx_y0			= 2210,
	glow_eSave_GrowCtx_x1			= 2211,
	glow_eSave_GrowCtx_y1			= 2212,
	glow_eSave_GrowCtx_path_cnt		= 2213,
	glow_eSave_GrowCtx_path			= 2214,
	glow_eSave_GrowCtx_trace_attr_type	= 2215,
	glow_eSave_GrowCtx_trace_color		= 2216,
	glow_eSave_GrowCtx_no_con_obstacle	= 2217,
	glow_eSave_GrowCtx_slider		= 2218,
	glow_eSave_GrowCtx_subgraph		= 2219,
	glow_eSave_GrowCtx_java_name		= 2220,
	glow_eSave_GrowCtx_trace_color2		= 2221,
	glow_eSave_GrowCtx_next_subgraph	= 2222,
	glow_eSave_GrowCtx_animation_count	= 2223,
	glow_eSave_GrowCtx_scantime		= 2224,
	glow_eSave_GrowCtx_animation_scantime	= 2225,
	glow_eSave_GrowCtx_java_width		= 2226,
	glow_eSave_GrowCtx_background_image    	= 2227,
	glow_eSave_GrowCtx_background_tiled    	= 2228,
	glow_eSave_GrowCtx_double_buffered    	= 2229,
	glow_eSave_GrowCtx_is_javaapplet    	= 2230,
	glow_eSave_GrowCtx_is_javaapplication   = 2231,
	glow_eSave_GrowCtx_fast_scantime	= 2232,
	glow_eSave_GrowCtx_cycle	        = 2233,
	glow_eSave_GrowCtx_mb3_action	        = 2234,
	glow_eSave_GrowCtx_translate_on	        = 2235,
	glow_eSave_GrowSubAnnot_x_right		= 2300,
	glow_eSave_GrowSubAnnot_x_left		= 2301,
	glow_eSave_GrowSubAnnot_y_high		= 2302,
	glow_eSave_GrowSubAnnot_y_low		= 2303,
	glow_eSave_GrowSubAnnot_n_name		= 2304,
	glow_eSave_GrowSubAnnot_text		= 2305,
	glow_eSave_GrowSubAnnot_rect		= 2306,
	glow_eSave_GrowSubAnnot_annot_part	= 2307,
	glow_eSave_GrowSubAnnot_trf		= 2308,
	glow_eSave_GrowArc_x_right		= 2400,
	glow_eSave_GrowArc_x_left		= 2401,
	glow_eSave_GrowArc_y_high		= 2402,
	glow_eSave_GrowArc_y_low		= 2403,
	glow_eSave_GrowArc_n_name		= 2404,
	glow_eSave_GrowArc_arc_part		= 2405,
	glow_eSave_GrowArc_dynamic		= 2406,
	glow_eSave_GrowArc_dynamicsize		= 2407,
	glow_eSave_GrowArc_original_border_drawtype = 2408,
	glow_eSave_GrowArc_original_fill_drawtype = 2409,
	glow_eSave_GrowArc_fill_drawtype 	= 2410,
	glow_eSave_GrowArc_border 		= 2411,
	glow_eSave_GrowArc_trf 			= 2412,
	glow_eSave_PolyLine_draw_type		= 2500,
	glow_eSave_PolyLine_line_width		= 2501,
	glow_eSave_PolyLine_a_points		= 2502,
	glow_eSave_PolyLine_fill		= 2503,
	glow_eSave_PolyLine_closed_line		= 2504,
	glow_eSave_GrowPolyLine_x_right		= 2600,
	glow_eSave_GrowPolyLine_x_left		= 2601,
	glow_eSave_GrowPolyLine_y_high		= 2602,
	glow_eSave_GrowPolyLine_y_low		= 2603,
	glow_eSave_GrowPolyLine_n_name		= 2604,
	glow_eSave_GrowPolyLine_polyline_part	= 2605,
	glow_eSave_GrowPolyLine_dynamic		= 2606,
	glow_eSave_GrowPolyLine_dynamicsize	= 2607,
	glow_eSave_GrowPolyLine_original_border_drawtype = 2608,
	glow_eSave_GrowPolyLine_original_fill_drawtype = 2609,
	glow_eSave_GrowPolyLine_fill_drawtype 	= 2610,
	glow_eSave_GrowPolyLine_border 		= 2611,
	glow_eSave_GrowPolyLine_trf 		= 2612,
	glow_eSave_GrowPolyLine_fill_eq_border 	= 2613,
	glow_eSave_GrowNode_node_part		= 2700,
	glow_eSave_GrowNode_dynamic		= 2701,
	glow_eSave_GrowNode_dynamicsize		= 2702,
	glow_eSave_GrowNode_original_border_drawtype = 2703,
	glow_eSave_GrowNode_original_fill_drawtype = 2704,
	glow_eSave_GrowNode_fill_drawtype 	= 2705,
	glow_eSave_GrowNode_draw_type 		= 2706,
	glow_eSave_GrowNode_trf 		= 2707,
	glow_eSave_GrowNode_original_color_tone = 2708,
	glow_eSave_GrowNode_color_tone 		= 2709,
	glow_eSave_GrowNode_original_color_lightness = 2710,
	glow_eSave_GrowNode_color_lightness	= 2711,
	glow_eSave_GrowNode_original_color_intensity = 2712,
	glow_eSave_GrowNode_color_intensity 	= 2713,
	glow_eSave_GrowNode_original_color_shift = 2714,
	glow_eSave_GrowNode_color_shift 	= 2715,
	glow_eSave_GrowNode_arg_cnt		= 2716,
	glow_eSave_GrowNode_argv		= 2717,
	glow_eSave_GrowNode_argsize		= 2718,
	glow_eSave_GrowNode_line_width		= 2719,
	glow_eSave_GrowNode_invisible		= 2720,
	glow_eSave_Transform_a11		= 2800,
	glow_eSave_Transform_a12		= 2801,
	glow_eSave_Transform_a13		= 2802,
	glow_eSave_Transform_a21		= 2803,
	glow_eSave_Transform_a22		= 2804,
	glow_eSave_Transform_a23		= 2805,
	glow_eSave_Transform_rotation		= 2806,
	glow_eSave_GrowAnnot_annot_part		= 2907,
	glow_eSave_GrowAnnot_trf		= 2908,
	glow_eSave_GrowText_x_right		= 3000,
	glow_eSave_GrowText_x_left		= 3001,
	glow_eSave_GrowText_y_high		= 3002,
	glow_eSave_GrowText_y_low		= 3003,
	glow_eSave_GrowText_n_name		= 3004,
	glow_eSave_GrowText_text_part		= 3005,
	glow_eSave_GrowText_dynamic		= 3006,
	glow_eSave_GrowText_dynamicsize		= 3007,
	glow_eSave_GrowText_original_border_drawtype = 3008,
	glow_eSave_GrowText_trf 		= 3009,
	glow_eSave_GrowBar_max_value 		= 3100,
	glow_eSave_GrowBar_min_value 		= 3101,
	glow_eSave_GrowBar_bar_value 		= 3102,
	glow_eSave_GrowBar_bar_drawtype 	= 3103,
	glow_eSave_GrowBar_direction 		= 3104,
	glow_eSave_GrowBar_rect_part 		= 3105,
	glow_eSave_GrowBar_trace_data1		= 3106,
	glow_eSave_GrowBar_trace_data2		= 3107,
	glow_eSave_GrowBar_trace_attr_type	= 3108,
	glow_eSave_GrowBar_trace_color		= 3109,
	glow_eSave_GrowBar_bar_bordercolor	= 3110,
	glow_eSave_GrowBar_bar_borderwidth	= 3111,
	glow_eSave_GrowBar_trace_data3		= 3112,
	glow_eSave_GrowBar_trace_data4		= 3113,
	glow_eSave_GrowBar_trace_data5		= 3114,
	glow_eSave_GrowBar_trace_data6		= 3115,
	glow_eSave_GrowBar_trace_data7		= 3116,
	glow_eSave_GrowBar_trace_data8		= 3117,
	glow_eSave_GrowBar_trace_data9		= 3118,
	glow_eSave_GrowBar_trace_data10		= 3119,
	glow_eSave_GrowBar_access		= 3120,
	glow_eSave_GrowBar_cycle		= 3121,
	glow_eSave_GrowBar_ref_object		= 3122,
	glow_eSave_GrowTrend_max_value_0 	= 3200,
	glow_eSave_GrowTrend_min_value_0 	= 3201,
	glow_eSave_GrowTrend_curve_drawtype_0 	= 3202,
	glow_eSave_GrowTrend_rect_part 		= 3203,
	glow_eSave_GrowTrend_trace_data1	= 3204,
	glow_eSave_GrowTrend_trace_data2	= 3205,
	glow_eSave_GrowTrend_trace_attr_type	= 3206,
	glow_eSave_GrowTrend_trace_color	= 3207,
	glow_eSave_GrowTrend_horizontal_lines	= 3208,
	glow_eSave_GrowTrend_vertical_lines	= 3209,
	glow_eSave_GrowTrend_fill_curve		= 3210,
	glow_eSave_GrowTrend_curve_fill_drawtype_0 = 3211,
	glow_eSave_GrowTrend_no_of_points 	= 3212,
	glow_eSave_GrowTrend_curve_width 	= 3213,
	glow_eSave_GrowTrend_scan_time 		= 3214,
	glow_eSave_GrowTrend_max_value_1 	= 3215,
	glow_eSave_GrowTrend_min_value_1 	= 3216,
	glow_eSave_GrowTrend_curve_drawtype_1 	= 3217,
	glow_eSave_GrowTrend_curve_fill_drawtype_1 = 3218,
	glow_eSave_GrowTrend_trace_data3	= 3219,
	glow_eSave_GrowTrend_trace_data4	= 3220,
	glow_eSave_GrowTrend_trace_data5	= 3221,
	glow_eSave_GrowTrend_trace_data6	= 3222,
	glow_eSave_GrowTrend_trace_data7	= 3223,
	glow_eSave_GrowTrend_trace_data8	= 3224,
	glow_eSave_GrowTrend_trace_data9	= 3225,
	glow_eSave_GrowTrend_trace_data10	= 3226,
	glow_eSave_GrowTrend_access		= 3227,
	glow_eSave_GrowTrend_cycle		= 3228,
	glow_eSave_GrowTrend_ref_object		= 3229,
	glow_eSave_GrowSlider_grownode_part	= 3300,
	glow_eSave_GrowSlider_direction		= 3301,
	glow_eSave_GrowSlider_max_value		= 3302,
	glow_eSave_GrowSlider_min_value		= 3303,
	glow_eSave_GrowSlider_max_pos		= 3304,
	glow_eSave_GrowSlider_min_pos		= 3305,
	glow_eSave_GrowImage_x_right		= 3400,
	glow_eSave_GrowImage_x_left		= 3401,
	glow_eSave_GrowImage_y_high		= 3402,
	glow_eSave_GrowImage_y_low		= 3403,
	glow_eSave_GrowImage_n_name		= 3404,
	glow_eSave_GrowImage_image_filename	= 3405,
	glow_eSave_GrowImage_dynamic		= 3406,
	glow_eSave_GrowImage_dynamicsize	= 3407,
	glow_eSave_GrowImage_trf 		= 3408,
	glow_eSave_GrowImage_display_level     	= 3409,
	glow_eSave_GrowImage_ll 		= 3410,
	glow_eSave_GrowImage_ur 		= 3411,
	glow_eSave_GrowImage_color_tone 	= 3412,
	glow_eSave_GrowImage_color_lightness 	= 3413,
	glow_eSave_GrowImage_color_intensity	= 3414,
	glow_eSave_GrowImage_color_shift 	= 3415,
	glow_eSave_GrowGroup_grownode_part	= 3500,
	glow_eSave_GrowGroup_nc	                = 3501,
	glow_eSave_NodeGroup_nodeclass_part     = 3600,
	glow_eSave_GrowAxis_max_value     	= 3700,
	glow_eSave_GrowAxis_min_value 	        = 3701,
	glow_eSave_GrowAxis_rect_part 		= 3702,
	glow_eSave_GrowAxis_lines 		= 3703,
	glow_eSave_GrowAxis_longquotient 	= 3704,
	glow_eSave_GrowAxis_valuequotient 	= 3705,
	glow_eSave_GrowAxis_format 		= 3706,
	glow_eSave_GrowAxis_text_size 		= 3707,
	glow_eSave_GrowAxis_text_drawtype 	= 3708
	} glow_eSave;

typedef enum {
	glow_ePosition_Absolute,
	glow_ePosition_Relative
	} glow_ePosition;

typedef struct {
	int			width;
	int			height;
	char			*bits;
	} glow_sPixmapDataElem;

typedef glow_sPixmapDataElem glow_sPixmapData[DRAW_PIXMAP_SIZE];

typedef struct {
	glow_sPixmapData	pixmap_data;
	void			*pixmaps;
	} glow_sAnnotPixmap;

typedef struct {
	double	x;
	double	y;
	} glow_sPoint;

typedef enum {
  glow_eDest__ = 0,
  glow_eDest_IntoFirst,  /* Object inserted as first son. */
  glow_eDest_IntoLast,   /* Object inserted as last son. */
  glow_eDest_After,      /* Object inserted after destination object.*/
  glow_eDest_Before,     /* Object inserted before destination object. */
  glow_eDest_
} glow_eDest;


typedef enum {
	glow_mDisplayLevel_1		= 1 << 0,
	glow_mDisplayLevel_2		= 1 << 2,
	glow_mDisplayLevel_3		= 1 << 3,
	glow_mDisplayLevel_4		= 1 << 4,
	glow_mDisplayLevel_5		= 1 << 5,
	glow_mDisplayLevel_6		= 1 << 6
	} glow_mDisplayLevel;

typedef enum {
	glow_mSensitivity_MB1Click	= 1 << 0,
	glow_mSensitivity_MB1DoubleClick = 1 << 1,
	glow_mSensitivity_MB1Press	= 1 << 2,
	glow_mSensitivity_MB2Click	= 1 << 3,
	glow_mSensitivity_MB2DoubleClick = 1 << 4,
	glow_mSensitivity_MB2Press	= 1 << 5,
	glow_mSensitivity_MB3Click	= 1 << 6,
	glow_mSensitivity_MB3DoubleClick = 1 << 7,
	glow_mSensitivity_MB3Press	= 1 << 8
	} glow_mSensitivity;

typedef struct {
       int curves;
       int rows;
       int x_reverse;
       double  max_value[TREND_MAX_CURVES + 1];
       double  min_value[TREND_MAX_CURVES + 1];
       double  *data[TREND_MAX_CURVES + 1];
       glow_eDrawType color[TREND_MAX_CURVES + 1];
       glow_eDrawType fillcolor[TREND_MAX_CURVES + 1];
       } glow_sCurveData;

#if defined __cplusplus
}
#endif
#endif


















