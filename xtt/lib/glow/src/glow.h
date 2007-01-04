/* 
 * Proview   $Id: glow.h,v 1.16 2007-01-04 07:57:38 claes Exp $
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

#ifndef glow_h
#define glow_h

#if defined __cplusplus
extern "C" {
#endif

/*! \file glow.h
  \brief Common includefile for glow.

  glow.h contains common declarations of enum an structs
*/


#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif


#define GLOW_VERSION 4001
#define DRAW_MP 6

#define DRAW_TYPE_SIZE 9
#define DRAW_FONT_SIZE 9
#define DRAW_PIXMAP_SIZE 9
#define TREND_MAX_CURVES 11
#define TABLE_MAX_COL 12

#define FSPACE " "

/** \addtogroup GlowEnum */
/*@{*/

typedef void *glow_tPixmap;
typedef void *glow_tImImage;
typedef void *glow_tImData;
typedef void *glow_tWidget;

//!  Types of glow variables, used when displaying varables in attribute editors.
typedef enum {
  glow_eType_Boolean,  		//!< Type is a boolean
  glow_eType_Int,  		//!< Type is an integer
  glow_eType_Double,  		//!< Type is a double
  glow_eType_String,  		//!< Type is a string
  glow_eType_DynType, 		//!< Type of dynamics
  glow_eType_ActionType,  	//!< Type of action
  glow_eType_Direction,  	//!< Type is glow_eDirection
  glow_eType_Color,  		//!< Type is a drawtype (glow_eDrawType)
  glow_eType_Tone,  		//!< Type is a colortone ( glow_eDrawTone)
  glow_eType_ToneOrColor,  	//!< Type is a tone or, if the value is greater then glow_eDrawTone__, a drawtype
  glow_eType_TraceColor,
  glow_eType_Access,  		//!< Type is glow_mAccess
  glow_eType_Cycle,  		//!< Type is glow_eCycle
  glow_eType_MB3Action,  	//!< Type is glow_eMB3Action
  glow_eType_Relief,  		//!< Type is glow_eRelief
  glow_eType_InputFocusMark,  	//!< Type is glow_eInputFocusMark
  glow_eType_TextSize,	  	//!< Type is a text size, glow_eTextSize
  glow_eType_Adjustment	  	//!< Type is glow_eAdjustment
} glow_eType;

//! Type of Ctx class
/*! The type is returned bye type() function for the Ctx. */
typedef enum {
	glow_eCtxType_Glow,
	glow_eCtxType_Brow,
	glow_eCtxType_Grow,	//!< Type is grow context
	glow_eCtxType_ColPal,
	glow_eCtxType_Curve	//!< Type is a curve context
	} glow_eCtxType;

//! Policy when selecting an area with the select rectangle
typedef enum {
	glow_eSelectPolicy_Surround,    //!< All parts of an object has to be inside the select rectangle
	glow_eSelectPolicy_Partial,	//!< If only a part of the object is inside the select rectangle, it will be selected
	glow_eSelectPolicy_Both		//!< the policy will be Partial when the select rectangle is dragged from right to left, and Surround when dragged from left to right

	} glow_eSelectPolicy;

//! Action when MB3 is clicked
typedef enum {
  glow_eMB3Action_No,  		//!< No action on MB3 click  
  glow_eMB3Action_Close,  	//!< Close the window
  glow_eMB3Action_PopupMenu, 	//!< Display a popup menu, when this is defined for the object
  glow_eMB3Action_Both 		//!< Close the window, if the click doesn't hit an object, otherwise it will display the popmenu for the object.
} glow_eMB3Action;

typedef enum {
  glow_eTextSize_8	= 0,
  glow_eTextSize_10	= 1,
  glow_eTextSize_12	= 2,
  glow_eTextSize_14	= 4,
  glow_eTextSize_18	= 6,   
  glow_eTextSize_24	= 8
} glow_eTextSize;

//! Critera used to decide when an object is hot
typedef enum {
	glow_eHotMode_Default,
	glow_eHotMode_Disabled,
	glow_eHotMode_SingleObject,
	glow_eHotMode_TraceAction
	} glow_eHotMode;

//! Type of cursor when an object is hot
typedef enum {
  glow_mHotType_CursorCrossHair 	= 1 << 0,	//!< Hot cursor is a crosshair
  glow_mHotType_CursorHand 		= 1 << 1  	//!< Hot cursor is a hand
} glow_mHotType;

//! Type of line
typedef enum {
  glow_eLineType_Solid,  	//!< Solid line
  glow_eLineType_Dashed1,  	//!< Dashed line, short dashes
  glow_eLineType_Dashed2,  	//!< Dashed line, medium dashes
  glow_eLineType_Dashed3,  	//!< Dashed line, long dashes
  glow_eLineType_Dotted,  	//!< Dotted line
  glow_eLineType_DotDashed1,  	//!< DotDashed line, short dashes
  glow_eLineType_DotDashed2  	//!< DotDashed line, long dashes
} glow_eLineType;


//! Type of object
/*! The class an object is an instance of. Returned by the type() function. */
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
        glow_eObjectType_GrowAxis,
        glow_eObjectType_GrowRectRounded,
        glow_eObjectType_GrowConGlue,
        glow_eObjectType_GrowMenu,
        glow_eObjectType_GrowWindow,
        glow_eObjectType_GrowScrollBar,
        glow_eObjectType_GrowTable,
        glow_eObjectType_GrowFolder
	} glow_eObjectType;

//! Direction of a connection points, sliders etc
/*! The Up and Down direction might cause some confusion, because Up is increasing y-coordinate,
  and Down is decreasing, which is the opposite directions on the screen 
*/
typedef enum {
  glow_eDirection_Center,  	//!< No specific direction
  glow_eDirection_Right,  	//!< Direction right
  glow_eDirection_Left,  	//!< Direction left
  glow_eDirection_Up,  		//!< Direction Up
  glow_eDirection_Down  	//!< Direction down
} glow_eDirection;

//! Adjustment of annotations
typedef enum {
  glow_eAdjustment_Center,  	//!< Adjustment center
  glow_eAdjustment_Right,  	//!< Adjustment right
  glow_eAdjustment_Left  	//!< Adjustment left
} glow_eAdjustment;

//! Type of mirror
typedef enum {
  glow_eFlipDirection_Vertical,   //!< Mirroring through a vertical line
  glow_eFlipDirection_Horizontal  //!< Mirroring through a horizontal line
} glow_eFlipDirection;

//! Vertical or horizontal direction
typedef enum {
  glow_eDir_Vertical,   //!< Vertical direction
  glow_eDir_Horizontal  //!< Horizontal direction
} glow_eDir;

//! Visibility enum
typedef enum {
  glow_eVis_Visible,   	//!< Object is visible
  glow_eVis_Invisible, 	//!< Object is invisible
  glow_eVis_Dimmed  	//!< Object is dimmed insensible
} glow_eVis;

//! Different kind of alignment functions
typedef enum {
  glow_eAlignDirection_CenterCenter,
  glow_eAlignDirection_Right,  	//!< Align the right side of the objects vertically
  glow_eAlignDirection_Left, 	//!< Align the left side of the objects vertically
  glow_eAlignDirection_Up, 	//!< Align the upper side of the objects horizontally
  glow_eAlignDirection_Down,  	//!< Align the lower side of the object horizontally
  glow_eAlignDirection_CenterVert,  //!< Align the center position of the object vertically
  glow_eAlignDirection_CenterHoriz  //!< Align the center position of the objects horizontally
} glow_eAlignDirection;

//! Groups a node can belong to, only Common is used in glow
typedef enum {
  glow_eNodeGroup_Common,
  glow_eNodeGroup_Document,
  glow_eNodeGroup_Trace
} glow_eNodeGroup;

//! Groups a connection can belong to, only Common is used in glow
typedef enum {
  glow_eConGroup_Common,
  glow_eConGroup_Trace
} glow_eConGroup;
  
//! Restrictions in movement when moving objects, creating lines etc
typedef enum {
  glow_eMoveRestriction_No,  		//!< No restrictions
  glow_eMoveRestriction_Disable,  	//!< Movement is disabled
  glow_eMoveRestriction_Vertical, 	//!< Only vertical movement
  glow_eMoveRestriction_Horizontal,  	//!< Only horizontal movement
  glow_eMoveRestriction_VerticalSlider, //!< Restrictions used for a vertical slider
  glow_eMoveRestriction_HorizontalSlider //!< Restrictions user for a horizontal slider
} glow_eMoveRestriction;

//! Type of annotation
typedef enum {
  glow_eAnnotType_OneLine,   	//!< Annotation with a single line
  glow_eAnnotType_MultiLine  	//!< Annotation with several lines
} glow_eAnnotType;

//! Cycle used, for update scan
typedef enum {
  glow_eCycle_Inherit,  	//!< Inherit the cycle from the subgraph
  glow_eCycle_Slow,  		//!< Slow cycle
  glow_eCycle_Fast  		//!< Fast cycle

} glow_eCycle;

//! Userdata callback type
typedef enum {
  glow_eUserdataCbType_Node,	//!< Userdata in a GrowNode
  glow_eUserdataCbType_Ctx,	//!< Userdata in GrowCtx
  glow_eUserdataCbType_NodeClass //!< Userdata in a GlowNodeClass
} glow_eUserdataCbType;

//! Pass for java export
typedef enum {
	glow_eExportPass_Init,
	glow_eExportPass_Shape,
	glow_eExportPass_Draw,
	glow_eExportPass_Declare,
	glow_eExportPass_Attributes,
	glow_eExportPass_End
	} glow_eExportPass;

//! Connection types
typedef enum {
  glow_eConType_Straight,  	//!< A straigh line
  glow_eConType_Fixed,  	//!< A line with fix coordinates
  glow_eConType_AllFixed,
  glow_eConType_Routed,  	//!< A routed connection. The breakpoints are calculated dependent on the position other objects.
  glow_eConType_StepDiv,   	//!< A Grafcet step divergence connection
  glow_eConType_StepConv,  	//!< A Grafcet step convergence connection
  glow_eConType_TransDiv,  	//!< A Grafcet trans divergence connection
  glow_eConType_TransConv,  	//!< A Grafcet trans convergence connection
  glow_eConType_StraightOneArrow, //!< A stright line with one arrow
  glow_eConType_Reference  	//!< A reference connection
} glow_eConType;

//! Type of corners in a connection
typedef enum {
  glow_eCorner_Rounded,  	//!< Rounded corners
  glow_eCorner_Right  		//!< Right corners
} glow_eCorner;

typedef enum {
	glow_eDynamicType_Object,
	glow_eDynamicType_SubGraph,
	glow_eDynamicType_SubGraphEnd
	} glow_eDynamicType;

//! Type of shadow drawn for an object
typedef enum {
  glow_eRelief_Up,   //!< The shadow is lighter on the left-upper side, and darker on the right-lower side  
  glow_eRelief_Down  //!< The shadow is darker on the left-upper side, and lighter on the left-lower side
} glow_eRelief;

//! Color index for a color
/*! The drawtype is index in an array that contains the gc for colors in the color palette. 
 The 300 first are the colors in the color palette, the seven last are used for erase, and texts. */
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
	glow_eDrawType_Color101,
	glow_eDrawType_Color102,
	glow_eDrawType_Color103,
	glow_eDrawType_Color104,
	glow_eDrawType_Color105,
	glow_eDrawType_Color106,
	glow_eDrawType_Color107,
	glow_eDrawType_Color108,
	glow_eDrawType_Color109,
	glow_eDrawType_Color110,
	glow_eDrawType_Color111,
	glow_eDrawType_Color112,
	glow_eDrawType_Color113,
	glow_eDrawType_Color114,
	glow_eDrawType_Color115,
	glow_eDrawType_Color116,
	glow_eDrawType_Color117,
	glow_eDrawType_Color118,
	glow_eDrawType_Color119,
	glow_eDrawType_Color120,
	glow_eDrawType_Color121,
	glow_eDrawType_Color122,
	glow_eDrawType_Color123,
	glow_eDrawType_Color124,
	glow_eDrawType_Color125,
	glow_eDrawType_Color126,
	glow_eDrawType_Color127,
	glow_eDrawType_Color128,
	glow_eDrawType_Color129,
	glow_eDrawType_Color130,
	glow_eDrawType_Color131,
	glow_eDrawType_Color132,
	glow_eDrawType_Color133,
	glow_eDrawType_Color134,
	glow_eDrawType_Color135,
	glow_eDrawType_Color136,
	glow_eDrawType_Color137,
	glow_eDrawType_Color138,
	glow_eDrawType_Color139,
	glow_eDrawType_Color140,
	glow_eDrawType_Color141,
	glow_eDrawType_Color142,
	glow_eDrawType_Color143,
	glow_eDrawType_Color144,
	glow_eDrawType_Color145,
	glow_eDrawType_Color146,
	glow_eDrawType_Color147,
	glow_eDrawType_Color148,
	glow_eDrawType_Color149,
	glow_eDrawType_Color150,
	glow_eDrawType_Color151,
	glow_eDrawType_Color152,
	glow_eDrawType_Color153,
	glow_eDrawType_Color154,
	glow_eDrawType_Color155,
	glow_eDrawType_Color156,
	glow_eDrawType_Color157,
	glow_eDrawType_Color158,
	glow_eDrawType_Color159,
	glow_eDrawType_Color160,
	glow_eDrawType_Color161,
	glow_eDrawType_Color162,
	glow_eDrawType_Color163,
	glow_eDrawType_Color164,
	glow_eDrawType_Color165,
	glow_eDrawType_Color166,
	glow_eDrawType_Color167,
	glow_eDrawType_Color168,
	glow_eDrawType_Color169,
	glow_eDrawType_Color170,
	glow_eDrawType_Color171,
	glow_eDrawType_Color172,
	glow_eDrawType_Color173,
	glow_eDrawType_Color174,
	glow_eDrawType_Color175,
	glow_eDrawType_Color176,
	glow_eDrawType_Color177,
	glow_eDrawType_Color178,
	glow_eDrawType_Color179,
	glow_eDrawType_Color180,
	glow_eDrawType_Color181,
	glow_eDrawType_Color182,
	glow_eDrawType_Color183,
	glow_eDrawType_Color184,
	glow_eDrawType_Color185,
	glow_eDrawType_Color186,
	glow_eDrawType_Color187,
	glow_eDrawType_Color188,
	glow_eDrawType_Color189,
	glow_eDrawType_Color190,
	glow_eDrawType_Color191,
	glow_eDrawType_Color192,
	glow_eDrawType_Color193,
	glow_eDrawType_Color194,
	glow_eDrawType_Color195,
	glow_eDrawType_Color196,
	glow_eDrawType_Color197,
	glow_eDrawType_Color198,
	glow_eDrawType_Color199,
	glow_eDrawType_Color200,
	glow_eDrawType_Color201,
	glow_eDrawType_Color202,
	glow_eDrawType_Color203,
	glow_eDrawType_Color204,
	glow_eDrawType_Color205,
	glow_eDrawType_Color206,
	glow_eDrawType_Color207,
	glow_eDrawType_Color208,
	glow_eDrawType_Color209,
	glow_eDrawType_Color210,
	glow_eDrawType_Color211,
	glow_eDrawType_Color212,
	glow_eDrawType_Color213,
	glow_eDrawType_Color214,
	glow_eDrawType_Color215,
	glow_eDrawType_Color216,
	glow_eDrawType_Color217,
	glow_eDrawType_Color218,
	glow_eDrawType_Color219,
	glow_eDrawType_Color220,
	glow_eDrawType_Color221,
	glow_eDrawType_Color222,
	glow_eDrawType_Color223,
	glow_eDrawType_Color224,
	glow_eDrawType_Color225,
	glow_eDrawType_Color226,
	glow_eDrawType_Color227,
	glow_eDrawType_Color228,
	glow_eDrawType_Color229,
	glow_eDrawType_Color230,
	glow_eDrawType_Color231,
	glow_eDrawType_Color232,
	glow_eDrawType_Color233,
	glow_eDrawType_Color234,
	glow_eDrawType_Color235,
	glow_eDrawType_Color236,
	glow_eDrawType_Color237,
	glow_eDrawType_Color238,
	glow_eDrawType_Color239,
	glow_eDrawType_Color240,
	glow_eDrawType_Color241,
	glow_eDrawType_Color242,
	glow_eDrawType_Color243,
	glow_eDrawType_Color244,
	glow_eDrawType_Color245,
	glow_eDrawType_Color246,
	glow_eDrawType_Color247,
	glow_eDrawType_Color248,
	glow_eDrawType_Color249,
	glow_eDrawType_Color250,
	glow_eDrawType_Color251,
	glow_eDrawType_Color252,
	glow_eDrawType_Color253,
	glow_eDrawType_Color254,
	glow_eDrawType_Color255,
	glow_eDrawType_Color256,
	glow_eDrawType_Color257,
	glow_eDrawType_Color258,
	glow_eDrawType_Color259,
	glow_eDrawType_Color260,
	glow_eDrawType_Color261,
	glow_eDrawType_Color262,
	glow_eDrawType_Color263,
	glow_eDrawType_Color264,
	glow_eDrawType_Color265,
	glow_eDrawType_Color266,
	glow_eDrawType_Color267,
	glow_eDrawType_Color268,
	glow_eDrawType_Color269,
	glow_eDrawType_Color270,
	glow_eDrawType_Color271,
	glow_eDrawType_Color272,
	glow_eDrawType_Color273,
	glow_eDrawType_Color274,
	glow_eDrawType_Color275,
	glow_eDrawType_Color276,
	glow_eDrawType_Color277,
	glow_eDrawType_Color278,
	glow_eDrawType_Color279,
	glow_eDrawType_Color280,
	glow_eDrawType_Color281,
	glow_eDrawType_Color282,
	glow_eDrawType_Color283,
	glow_eDrawType_Color284,
	glow_eDrawType_Color285,
	glow_eDrawType_Color286,
	glow_eDrawType_Color287,
	glow_eDrawType_Color288,
	glow_eDrawType_Color289,
	glow_eDrawType_Color290,
	glow_eDrawType_Color291,
	glow_eDrawType_Color292,
	glow_eDrawType_Color293,
	glow_eDrawType_Color294,
	glow_eDrawType_Color295,
	glow_eDrawType_Color296,
	glow_eDrawType_Color297,
	glow_eDrawType_Color298,
	glow_eDrawType_Color299,
	glow_eDrawType_Color300 = 299,
	glow_eDrawType_LineErase = 300,
	glow_eDrawType_LineDashed = 301,
	glow_eDrawType_LineDashedRed = 302,
	glow_eDrawType_TextHelvetica = 303,
	glow_eDrawType_TextHelveticaBold = 304,
	glow_eDrawType_TextHelveticaErase = 305,
	glow_eDrawType_TextHelveticaEraseBold = 306,
	glow_eDrawType__ = 307,
	glow_eDrawType_Inherit = 9999,
	glow_eDrawType_No = 10000
	} glow_eDrawType;

#define glow_eDrawType_FillHighlight glow_eDrawType_Color174
#define glow_eDrawType_LineHighlight glow_eDrawType_Color176
#define glow_eDrawType_ColorYellow glow_eDrawType_Color115
#define glow_eDrawType_ColorRed glow_eDrawType_Color177
#define glow_eDrawType_DarkGray glow_eDrawType_Color30
#define glow_eDrawType_MediumGray glow_eDrawType_Color25
#define glow_eDrawType_LightGray glow_eDrawType_Color21

//! Draw tone is a set of colors with the same color tone, displayed on the same row in the color palette.
/*! The first nine are pure color tones, the following are shifted to lighter, darker, 
  more or less saturated appearance */
typedef enum {
	glow_eDrawTone_No,
	glow_eDrawTone_Gray,
	glow_eDrawTone_YellowGreen,
	glow_eDrawTone_Yellow,
	glow_eDrawTone_Orange,
	glow_eDrawTone_Red,
	glow_eDrawTone_Magenta,
	glow_eDrawTone_Blue,
	glow_eDrawTone_Seablue,
	glow_eDrawTone_Green,
	glow_eDrawTone_DarkGray,
	glow_eDrawTone_DarkYellowGreen,
	glow_eDrawTone_DarkYellow,
	glow_eDrawTone_DarkOrange,
	glow_eDrawTone_DarkRed,
	glow_eDrawTone_DarkMagenta,
	glow_eDrawTone_DarkBlue,
	glow_eDrawTone_DarkSeablue,
	glow_eDrawTone_DarkGreen,
	glow_eDrawTone_LightGray,
	glow_eDrawTone_LightYellowGreen,
	glow_eDrawTone_LightYellow,
	glow_eDrawTone_LightOrange,
	glow_eDrawTone_LightRed,
	glow_eDrawTone_LightMagenta,
	glow_eDrawTone_LightBlue,
	glow_eDrawTone_LightSeablue,
	glow_eDrawTone_LightGreen,
	glow_eDrawTone_GrayHighSaturation,
	glow_eDrawTone_YellowGreenHighSaturation,
	glow_eDrawTone_YellowHighSaturation,
	glow_eDrawTone_OrangeHighSaturation,
	glow_eDrawTone_RedHighSaturation,
	glow_eDrawTone_MagentaHighSaturation,
	glow_eDrawTone_BlueHighSaturation,
	glow_eDrawTone_SeablueHighSaturation,
	glow_eDrawTone_GreenHighSaturation,
	glow_eDrawTone_DarkGrayHighSaturation,
	glow_eDrawTone_DarkYellowGreenHighSaturation,
	glow_eDrawTone_DarkYellowHighSaturation,
	glow_eDrawTone_DarkOrangeHighSaturation,
	glow_eDrawTone_DarkRedHighSaturation,
	glow_eDrawTone_DarkMagentaHighSaturation,
	glow_eDrawTone_DarkBlueHighSaturation,
	glow_eDrawTone_DarkSeablueHighSaturation,
	glow_eDrawTone_DarkGreenHighSaturation,
	glow_eDrawTone_LightGrayHighSaturation,
	glow_eDrawTone_LightYellowGreenHighSaturation,
	glow_eDrawTone_LightYellowHighSaturation,
	glow_eDrawTone_LightOrangeHighSaturation,
	glow_eDrawTone_LightRedHighSaturation,
	glow_eDrawTone_LightMagentaHighSaturation,
	glow_eDrawTone_LightBlueHighSaturation,
	glow_eDrawTone_LightSeablueHighSaturation,
	glow_eDrawTone_LightGreenHighSaturation,
	glow_eDrawTone_GrayLowSaturation,
	glow_eDrawTone_YellowGreenLowSaturation,
	glow_eDrawTone_YellowLowSaturation,
	glow_eDrawTone_OrangeLowSaturation,
	glow_eDrawTone_RedLowSaturation,
	glow_eDrawTone_MagentaLowSaturation,
	glow_eDrawTone_BlueLowSaturation,
	glow_eDrawTone_SeablueLowSaturation,
	glow_eDrawTone_GreenLowSaturation,
	glow_eDrawTone_DarkGrayLowSaturation,
	glow_eDrawTone_DarkYellowGreenLowSaturation,
	glow_eDrawTone_DarkYellowLowSaturation,
	glow_eDrawTone_DarkOrangeLowSaturation,
	glow_eDrawTone_DarkRedLowSaturation,
	glow_eDrawTone_DarkMagentaLowSaturation,
	glow_eDrawTone_DarkBlueLowSaturation,
	glow_eDrawTone_DarkSeablueLowSaturation,
	glow_eDrawTone_DarkGreenLowSaturation,
	glow_eDrawTone_LightGrayLowSaturation,
	glow_eDrawTone_LightYellowGreenLowSaturation,
	glow_eDrawTone_LightYellowLowSaturation,
	glow_eDrawTone_LightOrangeLowSaturation,
	glow_eDrawTone_LightRedLowSaturation,
	glow_eDrawTone_LightMagentaLowSaturation,
	glow_eDrawTone_LightBlueLowSaturation,
	glow_eDrawTone_LightSeablueLowSaturation,
	glow_eDrawTone_LightGreenLowSaturation,
	glow_eDrawTone__
	} glow_eDrawTone;

//! Cursors used
typedef enum {
  glow_eDrawCursor_Normal,  		//!< The original cursor
  glow_eDrawCursor_CrossHair,  		//!< Crosshair cursor
  glow_eDrawCursor_Hand,  		//!< Hand cursor
  glow_eDrawCursor_DiamondCross,	//!< Diamond cross cursor
  glow_eDrawCursor_BottomLeftCorner,	//!< Bottom left corner cursor
  glow_eDrawCursor_BottomRightCorner,	//!< Bottom right corner cursor
  glow_eDrawCursor_BottomSide,		//!< Bottom side cursor
  glow_eDrawCursor_TopLeftCorner,	//!< Top left corner cursor
  glow_eDrawCursor_TopRightCorner,	//!< Top right corner cursor
  glow_eDrawCursor_TopSide,		//!< Top side cursor
  glow_eDrawCursor_RightSide,		//!< Right side cursor
  glow_eDrawCursor_LeftSide,		//!< Left side cursor
  glow_eDrawCursor__
} glow_eDrawCursor;
                              
//! Types of fonts
typedef enum {
  glow_eDrawFont_HelveticaBold,  	//!< Helvetica bold
  glow_eDrawFont_Helvetica,  		//!< Helvetica normal
  glow_eDrawFont__
} glow_eDrawFont;

//! How to move a routed connection
typedef enum {
	glow_eMoveType_Frozen,
	glow_eMoveType_Route
	} glow_eMoveType;

//! How to mark input focus
typedef enum {
  glow_eInputFocusMark_Relief,		//!< Draw a relief around the object.
  glow_eInputFocusMark_No		//!< Don't mark at all.
} glow_eInputFocusMark;

//! Position of scale point when scaling an object
typedef enum {
  glow_eScaleType_LowerLeft,  		//!< Scale from lower left point  
  glow_eScaleType_LowerRight,  		//!< Scale from lower right point  
  glow_eScaleType_UpperRight,  		//!< Scale from upper right point  
  glow_eScaleType_UpperLeft,  		//!< Scale from upper left point  
  glow_eScaleType_Left,  		//!< Scale in x direction from left side  
  glow_eScaleType_Up,  			//!< Scale in y direction from upper side
  glow_eScaleType_Right,  		//!< Scale in x direction from right side  
  glow_eScaleType_Down, 		//!< Scale in y direction from lower side  
  glow_eScaleType_Center,  		//!< Scale point is center of object
  glow_eScaleType_FixPoint,  		//!< Scalepoint is supplied by user
  glow_eScaleType_No  			//!< Scale is disabled
} glow_eScaleType;

//! Position of rotation point when rotating an object
typedef enum {
  glow_eRotationPoint_LowerLeft,  	//!< Rotationpoint is lower left  
  glow_eRotationPoint_LowerRight,  	//!< Rotationpoint is lower right 
  glow_eRotationPoint_UpperRight,  	//!< Rotationpoint is upper right 
  glow_eRotationPoint_UpperLeft,  	//!< Rotationpoint is upper left 
  glow_eRotationPoint_Center,  		//!< Rotationpoint is the center of the object  
  glow_eRotationPoint_FixPoint,  	//!< Rotationpoint is supplied by the user
  glow_eRotationPoint_Zero  		//!< Rotationpoint is origo
} glow_eRotationPoint;

//! Type of event
/*! Glow has a number of event that is enabled with the enble_event() function. With this
  function you enable an event type and connect it to a specific mouse klick action. 
  The event has data structure glow_sEventAny as default, but some of the event types
  has extended data structures.
*/
typedef enum {
  glow_eEventType_CallBack,  		//!< An ordinary callback. Event structure is glow_sEventAny
  glow_eEventType_RegionSelect,  	//!< Region selected callback. Event structure is glow_sEventAny
  glow_eEventType_RegionAddSelect,  	//!< Region add selected callback. Event structure is  glow_sEventAny
  glow_eEventType_CreateCon,  		//!< Create connection callback. Event structure is  glow_sEventConCreate
  glow_eEventType_CreateNode,  		//!< Create node callback. Event structure is glow_sEventAny
  glow_eEventType_MoveNode,  		//!< Node moved callback. Event structure is glow_sEventAny
  glow_eEventType_Object,  		//!< Callback with a specified object. Event structure is glow_sEventObject
  glow_eEventType_AnnotationInput,  	//!< Annotation input callback. Event structure is glow_sEventAnnotInput
  glow_eEventType_Radiobutton,  	//!< Radiobutton clicked callback. Event structure is glow_sEventRadiobutton
  glow_eEventType_CreateGrowObject, 	//!< Create grow object callback. Event structure is glow_sEventCreateGrowObject
  glow_eEventType_Dynamics,  		//!< Execute dynamics callback. Event structure is glow_sEventDynamics

  glow_eEventType_ColorTone,
  glow_eEventType_Translate,  		//!< Translation callback. Event structure is glow_sEventTranslate
  glow_eEventType_KeyAscii,  		//!< Ascii key callback.
  glow_eEventType_Menu,  		//!< Menu callback.
  glow_eEventType_Table  		//!< Table callback.
} glow_eEventType;

//! Glow events
typedef enum {
  glow_eEvent_Null,  			//!< Event is disabled
  glow_eEvent_MB1Click,			//!< MB1 click event
  glow_eEvent_MB1DoubleClick,		//!< MB1 double click event
  glow_eEvent_MB1Press,			//!< MB1 press event
  glow_eEvent_MB2Click,			//!< MB2 click event
  glow_eEvent_MB2DoubleClick,		//!< MB2 double click event
  glow_eEvent_MB2Press,			//!< MB2 press event
  glow_eEvent_MB1ClickShift,		//!< MB1 click shift event
  glow_eEvent_MB1DoubleClickShift,	//!< MB1 double click shift event
  glow_eEvent_MB1PressShift,		//!< MB1 press shift event
  glow_eEvent_MB2ClickShift,		//!< MB2 click shift event
  glow_eEvent_MB2DoubleClickShift,	//!< MB2 double click shift event
  glow_eEvent_MB2PressShift,		//!< MB2 press shift event
  glow_eEvent_MB1ClickCtrl,		//!< MB1 click ctrl event
  glow_eEvent_MB1DoubleClickCtrl,	//!< MB1 double click ctrl event
  glow_eEvent_MB1PressCtrl,		//!< MB1 press ctrl event
  glow_eEvent_MB2ClickCtrl,		//!< MB2 click ctrl event
  glow_eEvent_MB2DoubleClickCtrl,	//!< MB2 double click ctrl event
  glow_eEvent_MB2PressCtrl,		//!< MB2 press ctrl event
  glow_eEvent_MB1ClickShiftCtrl,	//!< MB1 click shift ctrl event
  glow_eEvent_MB1DoubleClickShiftCtrl,	//!< MB1 double click shift ctrl event
  glow_eEvent_MB1PressShiftCtrl,	//!< MB1 press shift ctrl event
  glow_eEvent_MB2ClickShiftCtrl,	//!< MB2 click shift ctrl event
  glow_eEvent_MB2DoubleClickShiftCtrl,	//!< MB2 double click shift ctrl event
  glow_eEvent_MB2PressShiftCtrl,	//!< MB2 press shift ctrl event
  glow_eEvent_MB3Click,			//!< MB3 click event
  glow_eEvent_MB3Press,			//!< MB3 press event
  glow_eEvent_ButtonRelease,		//!< Button released event
  glow_eEvent_ButtonMotion,		//!< Button motion event  
  glow_eEvent_Exposure,			//!< Exposure event
  glow_eEvent_Enter,			//!< Cursor entered window event
  glow_eEvent_Leave,			//!< Cursor left window event
  glow_eEvent_CursorMotion,		//!< Cursor motion event
  glow_eEvent_Init,			//!< Initialize event
  glow_eEvent_PasteSequenceStart,	//!< Start of paste sequence event
  glow_eEvent_VisibilityUnobscured,	//!< Visibility unobscured event
  glow_eEvent_VisibilityObscured,	//!< Visibility obscured event
  glow_eEvent_SelectClear,		//!< Select list is cleared event
  glow_eEvent_ObjectMoved,		//!< Object is moved event
  glow_eEvent_ObjectDeleted,		//!< Object is deleted event
  glow_eEvent_AnnotationInput,		//!< Annotation input event
  glow_eEvent_Radiobutton,		//!< Radionbutton is clicked event
  glow_eEvent_Key_Return,		//!< Key Return event
  glow_eEvent_Key_Up,			//!< Key Up event
  glow_eEvent_Key_Down,			//!< Key Down event
  glow_eEvent_Key_Right,		//!< Key Right event
  glow_eEvent_Key_Left,			//!< Key Left event
  glow_eEvent_Key_PageUp,		//!< Key PageUp event
  glow_eEvent_Key_PageDown,		//!< Key PageDown event
  glow_eEvent_Key_BackSpace,		//!< Key BackSpace event
  glow_eEvent_Key_PF1,			//!< Key PF1 event
  glow_eEvent_Key_PF2,			//!< Key PF2 event
  glow_eEvent_Key_PF3,			//!< Key PF3 event
  glow_eEvent_Key_PF4,			//!< Key PF4 event
  glow_eEvent_CreateGrowObject,		//!< Create grow object event
  glow_eEvent_GrowDynamics,		//!< Execute dynamics event
  glow_eEvent_SliderMoveStart,		//!< Start of slider motion sequence event
  glow_eEvent_SliderMoved,		//!< Slider move event
  glow_eEvent_HotRequest,		//!< Hot request on object event
  glow_eEvent_MB1Down,			//!< MB1 down event
  glow_eEvent_MB1Up,			//!< MB1 up event
  glow_eEvent_MB2Down,			//!< MB2 down event
  glow_eEvent_MB2Up,			//!< MB2 up event
  glow_eEvent_MB3Down,			//!< MB3 down event
  glow_eEvent_MB3Up,			//!< MB3 up event
  glow_eEvent_Key_Tab,			//!< Key Tab event
  glow_eEvent_Map,			//!< Window mapped event
  glow_eEvent_Unmap,			//!< Window unmapped event
  glow_eEvent_Resized,			//!< Window resized event
  glow_eEvent_Translate,		//!< String translation event
  glow_eEvent_TipText,			//!< ToolTip event
  glow_eEvent_Key_Ascii,       		//!< Key ascii
  glow_eEvent_InputFocusLost,          	//!< Input focus lost
  glow_eEvent_InputFocusGained,        	//!< Input focus gained
  glow_eEvent_InputFocusInit,        	//!< Inititalize input focus.
  glow_eEvent_Key_CtrlAscii,        	//!< Key ctrl ascii.
  glow_eEvent_Key_ShiftTab,        	//!< Key Shift Tab event.
  glow_eEvent_Key_Escape,        	//!< Key Escape event.
  glow_eEvent_MenuActivated,        	//!< Menu is activated.
  glow_eEvent_MenuCreate,        	//!< Create pulldown menu.
  glow_eEvent_MenuDelete,        	//!< Delete pulldown menu.
  glow_eEvent__
} glow_eEvent;

typedef enum {
	glow_eTraceType_Boolean,
	glow_eTraceType_Int32,
	glow_eTraceType_Float32,
	glow_eTraceType_User
	} glow_eTraceType;

//! Type of item in a pulldown menu
typedef enum {
  glow_eMenuItem_Button,		//!< Item is a button
  glow_eMenuItem_ButtonDisabled,	//!< Item is disabled.
  glow_eMenuItem_PulldownMenu		//!< Item is a pulldown menu.
} glow_eMenuItem;

//! Save mode
typedef enum {
  glow_eSaveMode_Edit,		//!< Save as graph.
  glow_eSaveMode_Trace,		//!< Not used.
  glow_eSaveMode_SubGraph,	//!< Save as subgraph.
  glow_eSaveMode_ReadConfigOnly	//!< Read only configuration data.
} glow_eSaveMode;

//! Tags used when writing objects to a .pwg fil.
/*! Every data written is preceeded by a tag to identify the data. 
  All tags for all classes are defined here.
*/
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
	glow_eSave_GrowRectRounded	      	= 38,
	glow_eSave_GrowConGlue		      	= 39,
	glow_eSave_GrowWindow		      	= 40,
	glow_eSave_GrowFolder		      	= 41,
	glow_eSave_GrowTable		      	= 42,
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
	glow_eSave_Ctx_version			= 137,
	glow_eSave_Array_a			= 200,
	glow_eSave_NodeClass_nc_name		= 300,
	glow_eSave_NodeClass_a			= 301,
	glow_eSave_NodeClass_group		= 302,
	glow_eSave_NodeClass_dynamic		= 303,
	glow_eSave_NodeClass_dynamicsize	= 304,
	glow_eSave_NodeClass_arg_cnt		= 305,
	glow_eSave_NodeClass_argname		= 306,
	glow_eSave_NodeClass_argtype		= 307,
	glow_eSave_NodeClass_dyn_type		= 308,
	glow_eSave_NodeClass_dyn_color1		= 309,
	glow_eSave_NodeClass_no_con_obstacle	= 310,
	glow_eSave_NodeClass_slider		= 311,
	glow_eSave_NodeClass_java_name		= 312,
	glow_eSave_NodeClass_dyn_color2		= 313,
	glow_eSave_NodeClass_next_nodeclass	= 314,
	glow_eSave_NodeClass_animation_count	= 315,
	glow_eSave_NodeClass_cycle	        = 316,
	glow_eSave_NodeClass_y0	                = 317,
	glow_eSave_NodeClass_y1	                = 318,
	glow_eSave_NodeClass_x0	                = 319,
	glow_eSave_NodeClass_x1	                = 320,
	glow_eSave_NodeClass_dyn_action_type    = 321,
	glow_eSave_NodeClass_dyn_color3	        = 322,
	glow_eSave_NodeClass_dyn_color4	        = 323,
	glow_eSave_NodeClass_dyn_attr1	        = 324,
	glow_eSave_NodeClass_dyn_attr2	        = 325,
	glow_eSave_NodeClass_dyn_attr3	        = 326,
	glow_eSave_NodeClass_dyn_attr4	        = 327,
	glow_eSave_NodeClass_input_focus_mark   = 328,
	glow_eSave_NodeClass_userdata_cb	= 329,
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
	glow_eSave_Text_color_drawtype	       	= 904,
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
	glow_eSave_Con_border			= 1129,
	glow_eSave_Con_shadow			= 1130,
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
	glow_eSave_Annot_color_drawtype		= 1306,
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
	glow_eSave_GrowRect_shadow_width      	= 1913,
	glow_eSave_GrowRect_relief             	= 1914,
	glow_eSave_GrowRect_shadow             	= 1915,
	glow_eSave_GrowRect_shadow_contrast     = 1916,
	glow_eSave_GrowRect_disable_shadow      = 1917,
	glow_eSave_GrowRect_invisible      	= 1918,
	glow_eSave_GrowRect_fixcolor      	= 1919,
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
	glow_eSave_GrowLine_line_type 		= 2010,
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
	glow_eSave_GrowCtx_dyn_type		= 2215,
	glow_eSave_GrowCtx_dyn_color1		= 2216,
	glow_eSave_GrowCtx_no_con_obstacle	= 2217,
	glow_eSave_GrowCtx_slider		= 2218,
	glow_eSave_GrowCtx_subgraph		= 2219,
	glow_eSave_GrowCtx_java_name		= 2220,
	glow_eSave_GrowCtx_dyn_color2		= 2221,
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
	glow_eSave_GrowCtx_dyn_action_type    	= 2236,
	glow_eSave_GrowCtx_dyn_color3	        = 2237,
	glow_eSave_GrowCtx_dyn_color4	        = 2238,
	glow_eSave_GrowCtx_dyn_attr1	        = 2239,
	glow_eSave_GrowCtx_dyn_attr2	        = 2240,
	glow_eSave_GrowCtx_dyn_attr3	        = 2241,
	glow_eSave_GrowCtx_dyn_attr4	        = 2242,
	glow_eSave_GrowCtx_input_focus_mark     = 2243,
	glow_eSave_GrowCtx_userdata_cb	        = 2244,
	glow_eSave_GrowSubAnnot_x_right		= 2300,
	glow_eSave_GrowSubAnnot_x_left		= 2301,
	glow_eSave_GrowSubAnnot_y_high		= 2302,
	glow_eSave_GrowSubAnnot_y_low		= 2303,
	glow_eSave_GrowSubAnnot_n_name		= 2304,
	glow_eSave_GrowSubAnnot_text		= 2305,
	glow_eSave_GrowSubAnnot_rect		= 2306,
	glow_eSave_GrowSubAnnot_annot_part	= 2307,
	glow_eSave_GrowSubAnnot_trf		= 2308,
	glow_eSave_GrowSubAnnot_adjustment     	= 2309,
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
	glow_eSave_GrowArc_shadow_width      	= 2413,
	glow_eSave_GrowArc_relief      		= 2414,
	glow_eSave_GrowArc_shadow      		= 2415,
	glow_eSave_GrowArc_shadow_contrast      = 2416,
	glow_eSave_GrowArc_disable_shadow       = 2417,
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
	glow_eSave_GrowPolyLine_shadow_width    = 2614,
	glow_eSave_GrowPolyLine_relief         	= 2615,
	glow_eSave_GrowPolyLine_shadow        	= 2616,
	glow_eSave_GrowPolyLine_shadow_contrast = 2617,
	glow_eSave_GrowPolyLine_disable_shadow  = 2618,
	glow_eSave_GrowPolyLine_fill_eq_light   = 2619,
	glow_eSave_GrowPolyLine_fill_eq_shadow  = 2620,
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
	glow_eSave_GrowNode_userdata_cb		= 2721,
	glow_eSave_GrowNode_original_text_drawtype = 2722,
	glow_eSave_GrowNode_text_drawtype 	= 2723,
	glow_eSave_GrowNode_shadow 		= 2724,
	glow_eSave_Transform_a11		= 2800,
	glow_eSave_Transform_a12		= 2801,
	glow_eSave_Transform_a13		= 2802,
	glow_eSave_Transform_a21		= 2803,
	glow_eSave_Transform_a22		= 2804,
	glow_eSave_Transform_a23		= 2805,
	glow_eSave_Transform_rotation		= 2806,
	glow_eSave_GrowAnnot_adjustment		= 2901,
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
	glow_eSave_GrowText_original_color_drawtype = 3008,
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
	glow_eSave_GrowBar_userdata_cb		= 3123,
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
	glow_eSave_GrowTrend_userdata_cb	= 3230,
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
	glow_eSave_GrowAxis_text_drawtype 	= 3708,
	glow_eSave_GrowAxis_text_color_drawtype	= 3709,
	glow_eSave_GrowRectRounded_x_right	= 3800,
	glow_eSave_GrowRectRounded_x_left      	= 3801,
	glow_eSave_GrowRectRounded_y_high      	= 3802,
	glow_eSave_GrowRectRounded_y_low       	= 3803,
	glow_eSave_GrowRectRounded_n_name      	= 3804,
	glow_eSave_GrowRectRounded_rect_part   	= 3805,
	glow_eSave_GrowRectRounded_dynamic     	= 3806,
	glow_eSave_GrowRectRounded_dynamicsize 	= 3807,
	glow_eSave_GrowRectRounded_original_border_drawtype = 3808,
	glow_eSave_GrowRectRounded_original_fill_drawtype = 3809,
	glow_eSave_GrowRectRounded_fill_drawtype = 3810,
	glow_eSave_GrowRectRounded_border 	= 3811,
	glow_eSave_GrowRectRounded_trf 		= 3812,
	glow_eSave_GrowRectRounded_round_amount	= 3813,
	glow_eSave_GrowRectRounded_shadow_width = 3814,
	glow_eSave_GrowRectRounded_relief      	= 3815,
	glow_eSave_GrowRectRounded_shadow      	= 3816,
	glow_eSave_GrowRectRounded_shadow_contrast = 3817,
	glow_eSave_GrowRectRounded_disable_shadow = 3818,
	glow_eSave_GrowConGlue_line_width_up 	= 3900,
	glow_eSave_GrowConGlue_line_width_down 	= 3901,
	glow_eSave_GrowConGlue_line_width_left 	= 3902,
	glow_eSave_GrowConGlue_line_width_right = 3903,
	glow_eSave_GrowConGlue_node_part 	= 3904,
	glow_eSave_GrowConGlue_border	 	= 3905,
	glow_eSave_GrowWindow_file_name	 	= 4000,
	glow_eSave_GrowWindow_rect_part	 	= 4001,
	glow_eSave_GrowWindow_userdata_cb	= 4002,
	glow_eSave_GrowWindow_scrollbar_width	= 4003,
	glow_eSave_GrowWindow_vertical_scrollbar = 4004,
	glow_eSave_GrowWindow_horizontal_scrollbar = 4005,
	glow_eSave_GrowWindow_scrollbar_color 	= 4006,
	glow_eSave_GrowWindow_scrollbar_bg_color = 4007,
	glow_eSave_GrowWindow_window_scale 	= 4008,
	glow_eSave_GrowWindow_owner	 	= 4009,
	glow_eSave_GrowFolder_folders 		= 4100,
	glow_eSave_GrowFolder_text_size        	= 4101,
	glow_eSave_GrowFolder_text_drawtype    	= 4102,
	glow_eSave_GrowFolder_text_color_drawtype = 4103,
	glow_eSave_GrowFolder_header_height 	= 4104,
	glow_eSave_GrowFolder_window_part 	= 4105,
	glow_eSave_GrowFolder_color_selected 	= 4106,
	glow_eSave_GrowFolder_color_unselected	= 4107,
	glow_eSave_GrowFolder_folder_file_names1 = 4120,
	glow_eSave_GrowFolder_folder_text1 	= 4121,
	glow_eSave_GrowFolder_folder_scale1    	= 4122,
	glow_eSave_GrowFolder_folder_v_scrollbar1 = 4123,
	glow_eSave_GrowFolder_folder_h_scrollbar1 = 4124,
	glow_eSave_GrowFolder_folder_file_names2 = 4125,
	glow_eSave_GrowFolder_folder_text2 	= 4126,
	glow_eSave_GrowFolder_folder_scale2    	= 4127,
	glow_eSave_GrowFolder_folder_v_scrollbar2 = 4128,
	glow_eSave_GrowFolder_folder_h_scrollbar2 = 4129,
	glow_eSave_GrowFolder_folder_file_names3 = 4130,
	glow_eSave_GrowFolder_folder_text3 	= 4131,
	glow_eSave_GrowFolder_folder_scale3    	= 4132,
	glow_eSave_GrowFolder_folder_v_scrollbar3 = 4133,
	glow_eSave_GrowFolder_folder_h_scrollbar3 = 4134,
	glow_eSave_GrowFolder_folder_file_names4 = 4135,
	glow_eSave_GrowFolder_folder_text4 	= 4136,
	glow_eSave_GrowFolder_folder_scale4    	= 4137,
	glow_eSave_GrowFolder_folder_v_scrollbar4 = 4138,
	glow_eSave_GrowFolder_folder_h_scrollbar4 = 4139,
	glow_eSave_GrowFolder_folder_file_names5 = 4140,
	glow_eSave_GrowFolder_folder_text5 	= 4141,
	glow_eSave_GrowFolder_folder_scale5    	= 4142,
	glow_eSave_GrowFolder_folder_v_scrollbar5 = 4143,
	glow_eSave_GrowFolder_folder_h_scrollbar5 = 4144,
	glow_eSave_GrowFolder_folder_file_names6 = 4145,
	glow_eSave_GrowFolder_folder_text6 	= 4146,
	glow_eSave_GrowFolder_folder_scale6    	= 4147,
	glow_eSave_GrowFolder_folder_v_scrollbar6 = 4148,
	glow_eSave_GrowFolder_folder_h_scrollbar6 = 4149,
	glow_eSave_GrowFolder_folder_file_names7 = 4150,
	glow_eSave_GrowFolder_folder_text7 	= 4151,
	glow_eSave_GrowFolder_folder_scale7    	= 4152,
	glow_eSave_GrowFolder_folder_v_scrollbar7 = 4153,
	glow_eSave_GrowFolder_folder_h_scrollbar7 = 4154,
	glow_eSave_GrowFolder_folder_file_names8 = 4155,
	glow_eSave_GrowFolder_folder_text8 	= 4156,
	glow_eSave_GrowFolder_folder_scale8    	= 4157,
	glow_eSave_GrowFolder_folder_v_scrollbar8 = 4158,
	glow_eSave_GrowFolder_folder_h_scrollbar8 = 4159,
	glow_eSave_GrowFolder_folder_file_names9 = 4160,
	glow_eSave_GrowFolder_folder_text9 	= 4161,
	glow_eSave_GrowFolder_folder_scale9    	= 4162,
	glow_eSave_GrowFolder_folder_v_scrollbar9 = 4163,
	glow_eSave_GrowFolder_folder_h_scrollbar9 = 4164,
	glow_eSave_GrowFolder_folder_file_names10 = 4165,
	glow_eSave_GrowFolder_folder_text10 	= 4166,
	glow_eSave_GrowFolder_folder_scale10   	= 4167,
	glow_eSave_GrowFolder_folder_v_scrollbar10 = 4168,
	glow_eSave_GrowFolder_folder_h_scrollbar10 = 4169,
	glow_eSave_GrowFolder_folder_file_names11 = 4170,
	glow_eSave_GrowFolder_folder_text11 	= 4171,
	glow_eSave_GrowFolder_folder_scale11   	= 4172,
	glow_eSave_GrowFolder_folder_v_scrollbar11 = 4173,
	glow_eSave_GrowFolder_folder_h_scrollbar11 = 4174,
	glow_eSave_GrowFolder_folder_file_names12 = 4175,
	glow_eSave_GrowFolder_folder_text12 	= 4176,
	glow_eSave_GrowFolder_folder_scale12   	= 4177,
	glow_eSave_GrowFolder_folder_v_scrollbar12 = 4178,
	glow_eSave_GrowFolder_folder_h_scrollbar12 = 4179,
	glow_eSave_GrowFolder_folder_owner1 	= 4180,
	glow_eSave_GrowFolder_folder_owner2 	= 4181,
	glow_eSave_GrowFolder_folder_owner3 	= 4182,
	glow_eSave_GrowFolder_folder_owner4 	= 4183,
	glow_eSave_GrowFolder_folder_owner5 	= 4184,
	glow_eSave_GrowFolder_folder_owner6 	= 4185,
	glow_eSave_GrowFolder_folder_owner7 	= 4186,
	glow_eSave_GrowFolder_folder_owner8 	= 4187,
	glow_eSave_GrowFolder_folder_owner9 	= 4188,
	glow_eSave_GrowFolder_folder_owner10 	= 4189,
	glow_eSave_GrowFolder_folder_owner11 	= 4190,
	glow_eSave_GrowFolder_folder_owner12 	= 4191,
	glow_eSave_GrowTable_rect_part	 	= 4200,
	glow_eSave_GrowTable_userdata_cb	= 4202,
	glow_eSave_GrowTable_scrollbar_width	= 4203,
	glow_eSave_GrowTable_vertical_scrollbar = 4204,
	glow_eSave_GrowTable_horizontal_scrollbar = 4205,
	glow_eSave_GrowTable_scrollbar_color 	= 4206,
	glow_eSave_GrowTable_scrollbar_bg_color = 4207,
	glow_eSave_GrowTable_window_scale 	= 4208,
	glow_eSave_GrowTable_rows 		= 4209,
	glow_eSave_GrowTable_columns 		= 4210,
	glow_eSave_GrowTable_header_row		= 4211,
	glow_eSave_GrowTable_header_column     	= 4212,
	glow_eSave_GrowTable_text_size        	= 4213,
	glow_eSave_GrowTable_text_drawtype    	= 4214,
	glow_eSave_GrowTable_header_row_height 	= 4215,
	glow_eSave_GrowTable_row_height    	= 4216,
	glow_eSave_GrowTable_text_color_drawtype = 4217,
	glow_eSave_GrowTable_header_text_size   = 4218,
	glow_eSave_GrowTable_header_text_drawtype = 4219,
	glow_eSave_GrowTable_header_text_color  = 4220,
	glow_eSave_GrowTable_header_text_bold   = 4221,
	glow_eSave_GrowTable_options		= 4222,
	glow_eSave_GrowTable_column_width1    	= 4240,
	glow_eSave_GrowTable_header_text1    	= 4241,
	glow_eSave_GrowTable_column_width2    	= 4242,
	glow_eSave_GrowTable_header_text2    	= 4243,
	glow_eSave_GrowTable_column_width3    	= 4244,
	glow_eSave_GrowTable_header_text3    	= 4245,
	glow_eSave_GrowTable_column_width4    	= 4246,
	glow_eSave_GrowTable_header_text4    	= 4247,
	glow_eSave_GrowTable_column_width5    	= 4248,
	glow_eSave_GrowTable_header_text5    	= 4249,
	glow_eSave_GrowTable_column_width6    	= 4250,
	glow_eSave_GrowTable_header_text6    	= 4251,
	glow_eSave_GrowTable_column_width7    	= 4252,
	glow_eSave_GrowTable_header_text7    	= 4253,
	glow_eSave_GrowTable_column_width8    	= 4254,
	glow_eSave_GrowTable_header_text8    	= 4255,
	glow_eSave_GrowTable_column_width9    	= 4256,
	glow_eSave_GrowTable_header_text9    	= 4257,
	glow_eSave_GrowTable_column_width10    	= 4258,
	glow_eSave_GrowTable_header_text10    	= 4259,
	glow_eSave_GrowTable_column_width11    	= 4260,
	glow_eSave_GrowTable_header_text11    	= 4261,
	glow_eSave_GrowTable_column_width12    	= 4262,
	glow_eSave_GrowTable_header_text12    	= 4263,
	glow_eSave_GrowTable_select_drawtype   	= 4264,
	glow_eSave_GrowTable_column_adjustment1 = 4265,
	glow_eSave_GrowTable_column_adjustment2 = 4266,
	glow_eSave_GrowTable_column_adjustment3 = 4267,
	glow_eSave_GrowTable_column_adjustment4 = 4268,
	glow_eSave_GrowTable_column_adjustment5 = 4269,
	glow_eSave_GrowTable_column_adjustment6 = 4270,
	glow_eSave_GrowTable_column_adjustment7 = 4271,
	glow_eSave_GrowTable_column_adjustment8 = 4272,
	glow_eSave_GrowTable_column_adjustment9 = 4273,
	glow_eSave_GrowTable_column_adjustment10 = 4274,
	glow_eSave_GrowTable_column_adjustment11 = 4275,
	glow_eSave_GrowTable_column_adjustment12 = 4276
	} glow_eSave;

//! Relative or absolute position for an annotation
typedef enum {
  glow_ePosition_Absolute,		//!< Absolute position
  glow_ePosition_Relative		//!< Relative position
} glow_ePosition;

typedef enum {
  glow_eDest__ = 0,
  glow_eDest_IntoFirst,  //!< Object inserted as first son.
  glow_eDest_IntoLast,   //!< Object inserted as last son.
  glow_eDest_After,      //!< Object inserted after destination object.
  glow_eDest_Before,     //!< Object inserted before destination object.
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

//! Bitmap for mouse click and press sensitivity
typedef enum {
  glow_mSensitivity_MB1Click		= 1 << 0,	//!< Sensitive for MB1 click
  glow_mSensitivity_MB1DoubleClick 	= 1 << 1,	//!< Sensivive for MB1 double click
  glow_mSensitivity_MB1Press		= 1 << 2,	//!< Sensitive for MB1 press
  glow_mSensitivity_MB2Click		= 1 << 3,	//!< Sensitive for MB2 click
  glow_mSensitivity_MB2DoubleClick 	= 1 << 4,	//!< Sensitive for MB2 double click
  glow_mSensitivity_MB2Press		= 1 << 5,	//!< Sensitive for MB2 press
  glow_mSensitivity_MB3Click		= 1 << 6,	//!< Sensitive for MB3 click
  glow_mSensitivity_MB3DoubleClick 	= 1 << 7,	//!< Sensitive for MB3 double click
  glow_mSensitivity_MB3Press		= 1 << 8	//!< Sensitive for MB3 press
} glow_mSensitivity;

//! Conversion version
typedef enum {
  glow_eConvert_V34
} glow_eConvert;


/*@}*/
/** \addtogroup GlowStruct */
/*@{*/

//! Default data structure for events
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
} glow_sEventAny, *glow_tEventAny;

//! Data structure for an object event
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;  	//!< Type of object
  void			*object;  	//!< Ponter to the object
} glow_sEventObject, *glow_tEventObject;

//! Data stucture for a connection created event
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double		y;  		//!< y-coordinate
  void			*source_object;	//!< Pointer to source object
  void			*dest_object;  	//!< Pointer to destination object
  int			source_conpoint;//!< Connection point number at source
  int			dest_conpoint;  //!< Connection point number at destination
} glow_sEventConCreate, *glow_tEventConCreate;

//! Data structure for a annotation input event
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;  	//!< Type of object
  void			*object;  	//!< Pointer to object
  int			number;  	//!< Number of annotation
  char			*text;  	//!< Input text
} glow_sEventAnnotInput, *glow_tEventAnnotInput;

//! Data structure for a radiobutton event
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  glow_eObjectType	object_type;  	//!< Type of object
  void			*object;  	//!< Pointer to object
  int			number;  	//!< Radiobutton number
  int			value;		//!< Radiobutton value
} glow_sEventRadiobutton, *glow_tEventRadiobutton;

//! Data stucture for a create grow object event
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate for first point
  double	       	y;  		//!< y-coordinate for first point
  double	       	x2;		//!< x-coordinate for second point
  double	       	y2;		//!< y-coordinate for second point
  int			mode;		//!< Current context mode (grow_eMode)
  int			first_line;	//!< First line of a polyline
} glow_sEventCreateGrowObject, *glow_tEventCreateGrowObject;

//! Data structure for a dynamics event. Not yet fully implemented.
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;	//!< Type of object
  void			*object;	//!< Pointer to object
  char			*code;		//!< Code
  glow_eDynamicType	dynamic_type;	//!< Dynamic type
} glow_sEventDynamics, *glow_tEventDynamics;

//! Data structure for color tone callback, when a color tone is selected in the color palette.
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eDrawTone	tone;		//!< Selected color tone
} glow_sEventColorTone, *glow_tEventColorTone;

//! Data structure for a translate event
/*! If translate on is set before opening a .pwg file, all annotations and text objects
  will cause en translation callback, and the text will be replaced by the translated text.
*/
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;	//!< Type of object
  void			*object;        //!< Pointer to object
  char		        *text;		//!< Text to translate
  char		        *new_text;	//!< Translated text
} glow_sEventTranslate, *glow_tEventTranslate;

//! Data structure for an ascii key event.
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;  	//!< Type of object that has input focus
  void			*object;  	//!< Pointer to object that has input focus
  int			ascii;		//!< Ascii key
} glow_sEventKeyAscii, *glow_tEventKeyAscii;

//! Data structure for a menu event.
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;  	//!< Type of object that has input focus
  void			*object;  	//!< Pointer to object that has input focus
  int			item;		//!< Activated item
} glow_sEventMenu, *glow_tEventMenu;

//! Data structure for a table event.
typedef struct {
  glow_eEvent		event;  	//!< Event
  glow_eEventType	type;  		//!< Event type
  int			x_pixel;  	//!< x-coordinate i pixels
  int			y_pixel;  	//!< y-coordinate i pixels
  double		x;  		//!< x-coordinate
  double	       	y;  		//!< y-coordinate
  glow_eObjectType	object_type;  	//!< Type of object
  void			*object;  	//!< Pointer to object
  int			column;  	//!< Activated cell column
  int			row;  		//!< Activated cell row
} glow_sEventTable, *glow_tEventTable;


//! Union for event data structures
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
	glow_sEventKeyAscii	key;
	glow_sEventMenu		menu;
	glow_sEventTable       	table;
	} glow_sEvent, *glow_tEvent;

//! Pixmap data structure
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

//! A point with x and y coordinate
typedef struct {
  double	x;		//!< x coordinate
  double	y;		//!< y coordinate
} glow_sPoint;

//! A point with integer x and y coordinate
typedef struct {
  short		x;		//!< x coordinate
  short		y;		//!< y coordinate
} glow_sPointX;

//! Configuration of curves
typedef struct {
  int curves;						//!< Number of curves
  int rows;						//!< Number of points
  int x_reverse;					//!< Reverse the curves when drawing them
  double  max_value[TREND_MAX_CURVES + 1];		//!< Max value for every curve
  double  min_value[TREND_MAX_CURVES + 1];		//!< Min value for every curve
  double  *data[TREND_MAX_CURVES + 1];			//!< Data for every curve
  glow_eDrawType color[TREND_MAX_CURVES + 1];		//!< Color of every curve
  glow_eDrawType fillcolor[TREND_MAX_CURVES + 1];	//!< Fill color for every curve
} glow_sCurveData;

//! Data for a GrowTrend object
typedef struct {
  double		max_value[TREND_MAX_CURVES];
  double		min_value[TREND_MAX_CURVES];
  double		scan_time;
  int			horizontal_lines;
  int			vertical_lines;
  int			fill_curve;
  int			no_of_points;
  int			curve_width;
  glow_eDrawType	curve_drawtype[TREND_MAX_CURVES];
  glow_eDrawType	curve_fill_drawtype[TREND_MAX_CURVES];
} glow_sTrendInfo;

//! Data for a GrowBar object
typedef struct {
  double		max_value;		//!< Min value
  double		min_value;		//!< Max value
  glow_eDrawType	bar_drawtype;		//!< Color of the bar
  glow_eDrawType	bar_bordercolor;	//!< Color of the border of the bar
  int			bar_borderwidth;	//!< Width of the border
} glow_sBarInfo;

//! Data for a GrowAxis object
typedef struct {
  double		max_value;		//!< Min value
  double		min_value;		//!< Max value
  int			lines;			//!< Number of perpendicular lines
  int                 	longquotient;		//!< Quotient of lines that is a bit longer
  int                 	valuequotient;		//!< Quotient of lines where av value is displayed
  char                	format[20];		//!< Format in c-syntax of the displayed values
} glow_sAxisInfo;

//! Shadow information calculated for a segment in a polyline
typedef struct {
  glow_eDrawType drawtype;		//!< Color for shadow in segment
  int x;			        //!< x-coordinate for shadow polygon for the corresponding polyline point
  int y;				//!< y-coordinate for shadow polygon for the corresponding polyline point
} glow_sShadowInfo;

//! Info for a pulldown menu item
typedef struct {
  char text[80];		//!< Text of menu item.
  glow_eMenuItem type;		//!< Type of menu item.
  bool occupied;		//!< Menu item is configured.
} glow_sMenuInfoItem;

//! Info for pulldown menu
typedef struct {
  glow_sMenuInfoItem item[32];		//!< Array with info for the menu items.
} glow_sMenuInfo;

//! Data for a GrowTable object
typedef struct {
  int			columns;		//!< Number of columns
  int			rows;			//!< Number of rows
  int		        column_size[TABLE_MAX_COL]; //!< Number of characters in each column
} glow_sTableInfo;

/*@}*/

#if defined __cplusplus
}
#endif
#endif


















