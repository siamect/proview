/* ge_attrnav.cpp -- Display object info

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"


#include "ge_attr.h"
#include "ge_attrnav.h"
#include "ge_msg.h"

#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_openmap12.h"
#include "xnav_bitmap_attr12.h"
#include "xnav_bitmap_attrarra12.h"

#if defined OS_VMS
# pragma message disable (INTSIGNCHANGE)
#endif


#define ATTRNAV__INPUT_SYNTAX 2
#define ATTRNAV__OBJNOTFOUND 4
#define ATTRNAV__STRINGTOLONG 6
#define ATTRNAV__ITEM_NOCREA 8
#define ATTRNAV__SUCCESS 1

static attrnav_sEnumElement elem_trace_type[] = {
	{ (int) graph_eTrace_Inherit		, "Inherit"},
	{ (int) graph_eTrace_Dig		, "Dig"},
	{ (int) graph_eTrace_DigWithCommand	, "DigWithCommand"},
	{ (int) graph_eTrace_DigWithError	, "DigWithError"},
	{ (int) graph_eTrace_DigWithErrorAndCommand , "DigWithErrorAndCommand"},
	{ (int) graph_eTrace_DigTwo 		, "DigTwoWithError"},
	{ (int) graph_eTrace_DigTwoWithCommand 	, "DigTwoWithErrorAndCmd"},
	{ (int) graph_eTrace_DigTone		, "DigTone"},
	{ (int) graph_eTrace_DigToneWithCommand	 , "DigToneWithCommand"},
	{ (int) graph_eTrace_DigToneWithError	, "DigToneWithError"},
	{ (int) graph_eTrace_DigToneWithErrorAndCommand	, "DigToneWithErrorAndCommand"},
	{ (int) graph_eTrace_DigToneTwo		, "DigToneTwoWithError"},
	{ (int) graph_eTrace_DigToneTwoWithCommand, "DigToneTwoWithErrorAndCmd"},
	{ (int) graph_eTrace_DigWithText	, "DigWithText"},
	{ (int) graph_eTrace_DigBorder		, "DigBorder"},
	{ (int) graph_eTrace_Invisible		, "Invisible"},
	{ (int) graph_eTrace_Annot		, "Value"},
	{ (int) graph_eTrace_AnnotWithTone	, "ValueWithTone"},
	{ (int) graph_eTrace_AnnotInput		, "ValueInput"},
	{ (int) graph_eTrace_AnnotInputWithTone	, "ValueInputWithTone"},
	{ (int) graph_eTrace_Rotate		, "Rotate"},
	{ (int) graph_eTrace_Move		, "Move"},
	{ (int) graph_eTrace_AnalogShift	, "AnalogShift"},
	{ (int) graph_eTrace_Animation		, "Animation"},
	{ (int) graph_eTrace_AnimationForwBack	, "AnimationForwBack"},
	{ (int) graph_eTrace_DigAnimation	, "DigAnimation"},
	{ (int) graph_eTrace_DigShift		, "DigShift"},
	{ (int) graph_eTrace_DigShiftWithToggleDig  , "DigShiftWithToggleDig"},
	{ (int) graph_eTrace_Bar		, "Bar"},
	{ (int) graph_eTrace_Trend		, "Trend"},
	{ (int) graph_eTrace_SetDig		, "SetDig"},
	{ (int) graph_eTrace_SetDigConfirm	, "SetDigConfirm"},
	{ (int) graph_eTrace_SetDigWithTone	, "SetDigWithTone"},
	{ (int) graph_eTrace_SetDigConfirmWithTone , "SetDigConfirmWithTone"},
	{ (int) graph_eTrace_ResetDig		, "ResetDig"},
	{ (int) graph_eTrace_ResetDigConfirm	, "ResetDigConfirm"},
	{ (int) graph_eTrace_ResetDigWithTone	, "ResetDigWithTone"},
	{ (int) graph_eTrace_ResetDigConfirmWithTone , "ResetDigConfirmWithTone"},
	{ (int) graph_eTrace_ToggleDig		, "ToggleDig"},
	{ (int) graph_eTrace_ToggleDigConfirm	, "ToggleDigConfirm"},
	{ (int) graph_eTrace_ToggleDigWithTone	, "ToggleDigWithTone"},
	{ (int) graph_eTrace_ToggleDigConfirmWithTone , "ToggleDigConfirmWithTone"},
	{ (int) graph_eTrace_StoDigWithTone	, "StoDigWithTone"},
	{ (int) graph_eTrace_Command		, "Command"},
	{ (int) graph_eTrace_CommandConfirm	, "CommandConfirm"},
	{ (int) graph_eTrace_IncrAnalog		, "IncrAnalog"},
	{ (int) graph_eTrace_RadioButton	, "RadioButton"},
	{ (int) graph_eTrace_Slider		, "Slider"},
	{ (int) graph_eTrace_SliderBackground	, "SliderBackground"},
	{ (int) graph_eTrace_Video		, "Video"},
	{ (int) graph_eTrace_No		        , "No"},
	{ 0, ""}};

static attrnav_sEnumElement elem_color[] = {
	{ (int) glow_eDrawType_Line, 		"Black"},
	{ (int) glow_eDrawType_LineRed, 	"Red"},
	{ (int) glow_eDrawType_LineGray, 	"Grey"},
	{ (int) glow_eDrawType_Color4,	 	"White"},
	{ (int) glow_eDrawType_Color5, 		"Green"},
	{ (int) glow_eDrawType_Color6, 		"Yellow"},
	{ (int) glow_eDrawType_Color7, 		"Blue"},
	{ (int) glow_eDrawType_Color8, 		"Violet"},
	{ (int) glow_eDrawType_Color9, 		"DarkGrey"},
	{ (int) glow_eDrawType_Color10, 	"VeryDarkGray"},
	{ (int) glow_eDrawType_Color11, 	"Gray1"},
	{ (int) glow_eDrawType_Color12, 	"Gray2"},
	{ (int) glow_eDrawType_Color13, 	"Gray3"},
	{ (int) glow_eDrawType_Color14, 	"Gray4"},
	{ (int) glow_eDrawType_Color15, 	"Gray5"},
	{ (int) glow_eDrawType_Color16, 	"Gray6"},
	{ (int) glow_eDrawType_Color17, 	"Gray7"},
	{ (int) glow_eDrawType_Color18, 	"Gray8"},
	{ (int) glow_eDrawType_Color19, 	"Gray9"},
	{ (int) glow_eDrawType_Color20, 	"Gray10"},
	{ (int) glow_eDrawType_Color21, 	"Yellow1"},
	{ (int) glow_eDrawType_Color22, 	"Yellow2"},
	{ (int) glow_eDrawType_Color23, 	"Yellow3"},
	{ (int) glow_eDrawType_Color24, 	"Yellow4"},
	{ (int) glow_eDrawType_Color25, 	"Yellow5"},
	{ (int) glow_eDrawType_Color26, 	"Yellow6"},
	{ (int) glow_eDrawType_Color27, 	"Yellow7"},
	{ (int) glow_eDrawType_Color28, 	"Yellow8"},
	{ (int) glow_eDrawType_Color29, 	"Yellow9"},
	{ (int) glow_eDrawType_Color30, 	"Yellow10"},
	{ (int) glow_eDrawType_Color31, 	"Gold1"},
	{ (int) glow_eDrawType_Color32, 	"Gold2"},
	{ (int) glow_eDrawType_Color33, 	"Gold3"},
	{ (int) glow_eDrawType_Color34, 	"Gold4"},
	{ (int) glow_eDrawType_Color35, 	"Gold5"},
	{ (int) glow_eDrawType_Color36, 	"Gold6"},
	{ (int) glow_eDrawType_Color37, 	"Gold7"},
	{ (int) glow_eDrawType_Color38, 	"Gold8"},
	{ (int) glow_eDrawType_Color39, 	"Gold9"},
	{ (int) glow_eDrawType_Color40,		"Gold10"},
	{ (int) glow_eDrawType_Color41, 	"Orange1"},
	{ (int) glow_eDrawType_Color42, 	"Orange2"},
	{ (int) glow_eDrawType_Color43, 	"Orange3"},
	{ (int) glow_eDrawType_Color44, 	"Orange4"},
	{ (int) glow_eDrawType_Color45, 	"Orange5"},
	{ (int) glow_eDrawType_Color46, 	"Orange6"},
	{ (int) glow_eDrawType_Color47, 	"Orange7"},
	{ (int) glow_eDrawType_Color48, 	"Orange8"},
	{ (int) glow_eDrawType_Color49, 	"Orange9"},
	{ (int) glow_eDrawType_Color50, 	"Orange10"},
	{ (int) glow_eDrawType_Color51, 	"Red1"},
	{ (int) glow_eDrawType_Color52, 	"Red2"},
	{ (int) glow_eDrawType_Color53, 	"Red3"},
	{ (int) glow_eDrawType_Color54, 	"Red4"},
	{ (int) glow_eDrawType_Color55, 	"Red5"},
	{ (int) glow_eDrawType_Color56, 	"Red6"},
	{ (int) glow_eDrawType_Color57, 	"Red7"},
	{ (int) glow_eDrawType_Color58, 	"Red8"},
	{ (int) glow_eDrawType_Color59, 	"Red9"},
	{ (int) glow_eDrawType_Color60,		"Red10"},
	{ (int) glow_eDrawType_Color61, 	"Magenta1"},
	{ (int) glow_eDrawType_Color62, 	"Magenta2"},
	{ (int) glow_eDrawType_Color63, 	"Magenta3"},
	{ (int) glow_eDrawType_Color64, 	"Magenta4"},
	{ (int) glow_eDrawType_Color65, 	"Magenta5"},
	{ (int) glow_eDrawType_Color66, 	"Magenta6"},
	{ (int) glow_eDrawType_Color67, 	"Magenta7"},
	{ (int) glow_eDrawType_Color68, 	"Magenta8"},
	{ (int) glow_eDrawType_Color69, 	"Magenta9"},
	{ (int) glow_eDrawType_Color70, 	"Magenta10"},
	{ (int) glow_eDrawType_Color71, 	"Blue1"},
	{ (int) glow_eDrawType_Color72, 	"Blue2"},
	{ (int) glow_eDrawType_Color73, 	"Blue3"},
	{ (int) glow_eDrawType_Color74, 	"Blue4"},
	{ (int) glow_eDrawType_Color75, 	"Blue5"},
	{ (int) glow_eDrawType_Color76, 	"Blue6"},
	{ (int) glow_eDrawType_Color77, 	"Blue7"},
	{ (int) glow_eDrawType_Color78, 	"Blue8"},
	{ (int) glow_eDrawType_Color79, 	"Blue9"},
	{ (int) glow_eDrawType_Color80, 	"Blue10"},
	{ (int) glow_eDrawType_Color81, 	"SeaBlue1"},
	{ (int) glow_eDrawType_Color82, 	"SeaBlue2"},
	{ (int) glow_eDrawType_Color83, 	"SeaBlue3"},
	{ (int) glow_eDrawType_Color84, 	"SeaBlue4"},
	{ (int) glow_eDrawType_Color85, 	"SeaBlue5"},
	{ (int) glow_eDrawType_Color86, 	"SeaBlue6"},
	{ (int) glow_eDrawType_Color87, 	"SeaBlue7"},
	{ (int) glow_eDrawType_Color88, 	"SeaBlue8"},
	{ (int) glow_eDrawType_Color89, 	"SeaBlue9"},
	{ (int) glow_eDrawType_Color90, 	"SeaBlue10"},
	{ (int) glow_eDrawType_Color91, 	"Green1"},
	{ (int) glow_eDrawType_Color92, 	"Green2"},
	{ (int) glow_eDrawType_Color93, 	"Green3"},
	{ (int) glow_eDrawType_Color94, 	"Green4"},
	{ (int) glow_eDrawType_Color95, 	"Green5"},
	{ (int) glow_eDrawType_Color96, 	"Green6"},
	{ (int) glow_eDrawType_Color97, 	"Green7"},
	{ (int) glow_eDrawType_Color98, 	"Green8"},
	{ (int) glow_eDrawType_Color99, 	"Green9"},
	{ (int) glow_eDrawType_Color100, 	"Green10"},
	{ (int) glow_eDrawType_Inherit, 	"Inherit"},
	{ 0, ""}};

static attrnav_sEnumElement elem_tone[] = {
	{ (int) glow_eDrawTone_No, 	"No"},
	{ (int) glow_eDrawTone_Gray, 	"Gray"},
	{ (int) glow_eDrawTone_Yellow,	"Yellow"},
	{ (int) glow_eDrawTone_Gold, 	"Gold"},
	{ (int) glow_eDrawTone_Orange, 	"Orange"},
	{ (int) glow_eDrawTone_Red, 	"Red"},
	{ (int) glow_eDrawTone_Magenta, "Magenta"},
	{ (int) glow_eDrawTone_Blue, 	"Blue"},
	{ (int) glow_eDrawTone_Seablue, "Seablue"},
	{ (int) glow_eDrawTone_Green, 	"Green"},
	{ (int) glow_eDrawTone_DarkGray, "DarkGray"},
	{ 0, ""}};

static attrnav_sEnumElement elem_tone_or_color[] = {
	{ (int) glow_eDrawTone_No, 	"NoTone"},
	{ (int) glow_eDrawTone_Gray, 	"ToneGray"},
	{ (int) glow_eDrawTone_Yellow,	"ToneYellow"},
	{ (int) glow_eDrawTone_Gold, 	"ToneGold"},
	{ (int) glow_eDrawTone_Orange, 	"ToneOrange"},
	{ (int) glow_eDrawTone_Red, 	"ToneRed"},
	{ (int) glow_eDrawTone_Magenta, "ToneMagenta"},
	{ (int) glow_eDrawTone_Blue, 	"ToneBlue"},
	{ (int) glow_eDrawTone_Seablue, "ToneSeablue"},
	{ (int) glow_eDrawTone_Green, 	"ToneGreen"},
	{ (int) glow_eDrawTone_DarkGray, "ToneDarkGray"},
	{ (int) glow_eDrawType_Color12, 	"Gray2"},
	{ (int) glow_eDrawType_Color13, 	"Gray3"},
	{ (int) glow_eDrawType_Color14, 	"Gray4"},
	{ (int) glow_eDrawType_Color15, 	"Gray5"},
	{ (int) glow_eDrawType_Color16, 	"Gray6"},
	{ (int) glow_eDrawType_Color17, 	"Gray7"},
	{ (int) glow_eDrawType_Color18, 	"Gray8"},
	{ (int) glow_eDrawType_Color19, 	"Gray9"},
	{ (int) glow_eDrawType_Color20, 	"Gray10"},
	{ (int) glow_eDrawType_Color21, 	"Yellow1"},
	{ (int) glow_eDrawType_Color22, 	"Yellow2"},
	{ (int) glow_eDrawType_Color23, 	"Yellow3"},
	{ (int) glow_eDrawType_Color24, 	"Yellow4"},
	{ (int) glow_eDrawType_Color25, 	"Yellow5"},
	{ (int) glow_eDrawType_Color26, 	"Yellow6"},
	{ (int) glow_eDrawType_Color27, 	"Yellow7"},
	{ (int) glow_eDrawType_Color28, 	"Yellow8"},
	{ (int) glow_eDrawType_Color29, 	"Yellow9"},
	{ (int) glow_eDrawType_Color30, 	"Yellow10"},
	{ (int) glow_eDrawType_Color31, 	"Gold1"},
	{ (int) glow_eDrawType_Color32, 	"Gold2"},
	{ (int) glow_eDrawType_Color33, 	"Gold3"},
	{ (int) glow_eDrawType_Color34, 	"Gold4"},
	{ (int) glow_eDrawType_Color35, 	"Gold5"},
	{ (int) glow_eDrawType_Color36, 	"Gold6"},
	{ (int) glow_eDrawType_Color37, 	"Gold7"},
	{ (int) glow_eDrawType_Color38, 	"Gold8"},
	{ (int) glow_eDrawType_Color39, 	"Gold9"},
	{ (int) glow_eDrawType_Color40,		"Gold10"},
	{ (int) glow_eDrawType_Color41, 	"Orange1"},
	{ (int) glow_eDrawType_Color42, 	"Orange2"},
	{ (int) glow_eDrawType_Color43, 	"Orange3"},
	{ (int) glow_eDrawType_Color44, 	"Orange4"},
	{ (int) glow_eDrawType_Color45, 	"Orange5"},
	{ (int) glow_eDrawType_Color46, 	"Orange6"},
	{ (int) glow_eDrawType_Color47, 	"Orange7"},
	{ (int) glow_eDrawType_Color48, 	"Orange8"},
	{ (int) glow_eDrawType_Color49, 	"Orange8"},
	{ (int) glow_eDrawType_Color50, 	"Orange18"},
	{ (int) glow_eDrawType_Color51, 	"Red1"},
	{ (int) glow_eDrawType_Color52, 	"Red2"},
	{ (int) glow_eDrawType_Color53, 	"Red3"},
	{ (int) glow_eDrawType_Color54, 	"Red4"},
	{ (int) glow_eDrawType_Color55, 	"Red5"},
	{ (int) glow_eDrawType_Color56, 	"Red6"},
	{ (int) glow_eDrawType_Color57, 	"Red7"},
	{ (int) glow_eDrawType_Color58, 	"Red8"},
	{ (int) glow_eDrawType_Color59, 	"Red9"},
	{ (int) glow_eDrawType_Color60,		"Red10"},
	{ (int) glow_eDrawType_Color61, 	"Magenta1"},
	{ (int) glow_eDrawType_Color62, 	"Magenta2"},
	{ (int) glow_eDrawType_Color63, 	"Magenta3"},
	{ (int) glow_eDrawType_Color64, 	"Magenta4"},
	{ (int) glow_eDrawType_Color65, 	"Magenta5"},
	{ (int) glow_eDrawType_Color66, 	"Magenta6"},
	{ (int) glow_eDrawType_Color67, 	"Magenta7"},
	{ (int) glow_eDrawType_Color68, 	"Magenta8"},
	{ (int) glow_eDrawType_Color69, 	"Magenta9"},
	{ (int) glow_eDrawType_Color70, 	"Magenta10"},
	{ (int) glow_eDrawType_Color71, 	"Blue1"},
	{ (int) glow_eDrawType_Color72, 	"Blue2"},
	{ (int) glow_eDrawType_Color73, 	"Blue3"},
	{ (int) glow_eDrawType_Color74, 	"Blue4"},
	{ (int) glow_eDrawType_Color75, 	"Blue5"},
	{ (int) glow_eDrawType_Color76, 	"Blue6"},
	{ (int) glow_eDrawType_Color77, 	"Blue7"},
	{ (int) glow_eDrawType_Color78, 	"Blue8"},
	{ (int) glow_eDrawType_Color79, 	"Blue9"},
	{ (int) glow_eDrawType_Color80, 	"Blue10"},
	{ (int) glow_eDrawType_Color81, 	"SeaBlue1"},
	{ (int) glow_eDrawType_Color82, 	"SeaBlue2"},
	{ (int) glow_eDrawType_Color83, 	"SeaBlue3"},
	{ (int) glow_eDrawType_Color84, 	"SeaBlue4"},
	{ (int) glow_eDrawType_Color85, 	"SeaBlue5"},
	{ (int) glow_eDrawType_Color86, 	"SeaBlue6"},
	{ (int) glow_eDrawType_Color87, 	"SeaBlue7"},
	{ (int) glow_eDrawType_Color88, 	"SeaBlue8"},
	{ (int) glow_eDrawType_Color89, 	"SeaBlue9"},
	{ (int) glow_eDrawType_Color90, 	"SeaBlue10"},
	{ (int) glow_eDrawType_Color91, 	"Green1"},
	{ (int) glow_eDrawType_Color92, 	"Green2"},
	{ (int) glow_eDrawType_Color93, 	"Green3"},
	{ (int) glow_eDrawType_Color94, 	"Green4"},
	{ (int) glow_eDrawType_Color95, 	"Green5"},
	{ (int) glow_eDrawType_Color96, 	"Green6"},
	{ (int) glow_eDrawType_Color97, 	"Green7"},
	{ (int) glow_eDrawType_Color98, 	"Green8"},
	{ (int) glow_eDrawType_Color99, 	"Green9"},
	{ (int) glow_eDrawType_Color100, 	"Green10"},
	{ (int) glow_eDrawType_Inherit, 	"Inherit"},
	{ 0, ""}};

static attrnav_sEnumElement elem_direction[] = {
	{ (int) glow_eDirection_Right, 	"Right"},
	{ (int) glow_eDirection_Left, 	"Left"},
	{ (int) glow_eDirection_Up, 	"Down"},
	{ (int) glow_eDirection_Down, 	"Up"},
	{ 0, ""}};

static attrnav_sEnumElement elem_access[] = {
	{ (unsigned int) pwr_mPrv_RtRead, 	"RtRead"},
	{ (unsigned int) pwr_mPrv_RtWrite, 	"RtWrite"},
	{ (unsigned int) pwr_mPrv_System, 	"System"},
	{ (unsigned int) pwr_mPrv_Maintenance, 	"Maintenance"},
	{ (unsigned int) pwr_mPrv_Process, 	"Process"},
	{ (unsigned int) pwr_mPrv_Instrument, 	"Instrument"},
	{ (unsigned int) pwr_mPrv_Operator1, 	"Operator1"},
	{ (unsigned int) pwr_mPrv_Operator2, 	"Operator2"},
	{ (unsigned int) pwr_mPrv_Operator3, 	"Operator3"},
	{ (unsigned int) pwr_mPrv_Operator4, 	"Operator4"},
	{ (unsigned int) pwr_mPrv_Operator5, 	"Operator5"},
	{ (unsigned int) pwr_mPrv_Operator6, 	"Operator6"},
	{ (unsigned int) pwr_mPrv_Operator7, 	"Operator7"},
	{ (unsigned int) pwr_mPrv_Operator8, 	"Operator8"},
	{ (unsigned int) pwr_mPrv_Operator9, 	"Operator9"},
	{ (unsigned int) pwr_mPrv_Operator10, 	"Operator10"},
	{ (unsigned int) pwr_mAccess_Default, 	"Default"},
	{ 0, ""}};

static attrnav_sEnumElement elem_cycle[] = {
	{ (int) glow_eCycle_Inherit, 	"Inherit"},
	{ (int) glow_eCycle_Slow, 	"Slow"},
	{ (int) glow_eCycle_Fast, 	"Fast"},
	{ 0, ""}};

static attrnav_sEnumElement elem_mb3_action[] = {
	{ (int) glow_eMB3Action_No, 	"No"},
	{ (int) glow_eMB3Action_Close, 	"Close"},
	{ (int) glow_eMB3Action_PopupMenu, "PopupMenu"},
	{ (int) glow_eMB3Action_Both, 	"Both"},
	{ 0, ""}};

static attrnav_sEnum enum_types[] = {
	{ (int) glow_eType_TraceType, 	(attrnav_sEnumElement *) &elem_trace_type},
	{ (int) glow_eType_Direction, 	(attrnav_sEnumElement *) &elem_direction},
	{ (int) glow_eType_Color, 	(attrnav_sEnumElement *) &elem_color},
	{ (int) glow_eType_Tone,	(attrnav_sEnumElement *) &elem_tone},
	{ (int) glow_eType_ToneOrColor, (attrnav_sEnumElement *) &elem_tone_or_color},
	{ (int) glow_eType_Cycle, 	(attrnav_sEnumElement *) &elem_cycle},
	{ (int) glow_eType_MB3Action, 	(attrnav_sEnumElement *) &elem_mb3_action},
	{ 0, NULL}};

static attrnav_sEnum mask_types[] = {
	{ (int) glow_eType_Access, 	(attrnav_sEnumElement *) &elem_access},
	{ 0, NULL}};



static char null_str[] = "";

static void attrnav_trace_scan( AttrNav *attrnav);
static int attrnav_trace_scan_bc( brow_tObject object, void *p);
static int attrnav_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, /* flow_eDrawType color, */ void **p);
static int attrnav_trace_disconnect_bc( brow_tObject object);
static int attrnav_init_brow_cb( FlowCtx *fctx, void *client_data);

//
// Convert attribute string to value
//
int  attrnav_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size)
{

  switch ( type_id )
  {
    case glow_eType_Boolean:
    {
      if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
        return ATTRNAV__INPUT_SYNTAX;
      if ( *(pwr_tBoolean *)buffer_ptr > 1)
        return ATTRNAV__INPUT_SYNTAX;
      break;
    }
    case glow_eType_Double:
    {
      pwr_tFloat32 f;
      pwr_tFloat64 d;
      if ( sscanf( value_str, "%f", &f) != 1)
        return ATTRNAV__INPUT_SYNTAX;
      d = f;
      memcpy( buffer_ptr, (char *) &d, sizeof(d));

      break;
    }
    case glow_eType_Int:
    case glow_eType_TraceType:
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Access:
    case glow_eType_Cycle:
    case glow_eType_MB3Action:
    {
      if ( sscanf( value_str, "%u", (int *)buffer_ptr) != 1)
        return ATTRNAV__INPUT_SYNTAX;
      break;
    }
    case glow_eType_String:
    {
      if ( (int) strlen( value_str) >= attr_size)
        return ATTRNAV__STRINGTOLONG;
      strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
      break;
    }
  }
  return 1;
}

//
// Convert attribute value to string
//
void  attrnav_attrvalue_to_string( int type_id, void *value_ptr, 
	char *str, int size, int *len, char *format)
{

  if ( value_ptr == 0)
  {
    strcpy( str, "UNDEFINED");
    return;
  }

  switch ( type_id )
  {
    case glow_eType_Boolean:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(pwr_tBoolean *)value_ptr);
      else
        *len = sprintf( str, format, *(pwr_tBoolean *)value_ptr);
      break;
    }
    case glow_eType_Double:
    {
      if ( !format)
        *len = sprintf( str, "%f", *(double *)value_ptr);
      else
        *len = sprintf( str, format, *(double *)value_ptr);
      break;
    }
    case glow_eType_Int:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(int *)value_ptr);
      else
        *len = sprintf( str, format, *(int *)value_ptr);
      break;
    }
    case glow_eType_String:
    {
      strncpy( str, (char *)value_ptr, size);
      str[size-1] = 0;
      *len = strlen(str);
      break;
    }
    case glow_eType_TraceType:
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Cycle:
    case glow_eType_MB3Action:
    {
      attrnav_sEnumElement	*elem_p;
      attrnav_sEnum		*enum_p;
      int			found;

      found = 0;
      for ( enum_p = enum_types; enum_p->elements; enum_p++)
      {
        if ( enum_p->num == type_id)
        {
          elem_p = enum_p->elements;
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        attrnav_attrvalue_to_string( glow_eType_Int, value_ptr, 
		str, size, len, format);
        return;
      }

      found = 0;
      for ( ; elem_p->name[0] != 0; elem_p++)
      {
        if ( elem_p->num == *(int *)value_ptr)
        {
          strncpy( str, elem_p->name, size);
          str[size-1] = 0;
          *len = strlen(str);
          found = 1;
          break;
        }
      }

      if ( !found)
      {
        attrnav_attrvalue_to_string( glow_eType_Int, value_ptr, 
		str, size, len, format);
        return;
      }
      break;
    }
    case glow_eType_Access:
    {
      *len = sprintf( str, "%u", *(unsigned int *)value_ptr);
      break;
    }
  }
}

void AttrNav::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
//  Free pixmaps
//
void AttrNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void AttrNavBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_leaf12_width;
	    pixmap_data[i].height =xnav_bitmap_leaf12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_leaf12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_leaf);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_map12_width;
	    pixmap_data[i].height =xnav_bitmap_map12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_map12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_map);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_openmap12_width;
	    pixmap_data[i].height =xnav_bitmap_openmap12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_openmap12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_openmap);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attr12_width;
	    pixmap_data[i].height =xnav_bitmap_attr12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attr12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attr);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrarra12_width;
	    pixmap_data[i].height =xnav_bitmap_attrarra12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrarra12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrarray);

}


//
// Create the navigator widget
//
AttrNav::AttrNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	attr_sItem  *xn_itemlist,
	int xn_item_cnt,
	Widget *w,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	itemlist(xn_itemlist),item_cnt(xn_item_cnt),
	trace_started(0),
	message_cb(NULL)
{
  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	attrnav_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
AttrNav::~AttrNav()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

AttrNavBrow::~AttrNavBrow()
{
  free_pixmaps();
}

void AttrNav::set_inputfocus()
{
//  brow_SetInputFocus( brow->ctx);
  XtCallAcceptFocus( brow_widget, CurrentTime);
}

//
// Set attribute value
//
int AttrNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemLocal	*item;
  int		sts;
  char		buffer[1024];
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  switch( item->type)
  {
    case attrnav_eItemType_Local:
    {
      sts = attrnav_attr_string_to_value( item->type_id, value_str, 
		buffer, sizeof(buffer), item->size);
      if ( EVEN(sts)) return sts;

      memcpy( item->value_p, buffer, item->size);
      break;
    }
    default:
      ;
  }
  return 1;
}

//
// Check that the current selected item is valid for change
//
int AttrNav::check_attr_value( int *multiline, char **value)
{
  brow_tNode	*node_list;
  int		node_count;
  ItemLocal	*base_item;
  static char   buf[120];
  int           len;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return GE__NOATTRSEL;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type)
  {
    case attrnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *)base_item;

      if ( base_item->noedit) {
        *multiline = 0;
        *value = 0;
        return GE__ATTRNOEDIT;
      }
      *multiline = base_item->multiline;
      // if ( base_item->type_id == glow_eType_String)
      attrnav_attrvalue_to_string( item->type_id, item->value_p, buf, 
				   sizeof(buf), &len, NULL);
      *value = buf;

        // *value = (char *) base_item->value_p;
	// else
        // *value = 0;
      return GE__SUCCESS;
    }
    default:
      *multiline = 0;
      *value = 0;
      return GE__ATTRNOEDIT;
  }
}


//
// Callbacks from brow
//
static int attrnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  AttrNav		*attrnav;
  ItemLocal 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &attrnav);
  attrnav->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( attrnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( attrnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( attrnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object))
        brow_CenterObject( attrnav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( attrnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( attrnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( attrnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object))
        brow_CenterObject( attrnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( attrnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          if ( brow_FindSelectedObject( attrnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( attrnav->brow->ctx);
         }
          else
          {
            brow_SelectClear( attrnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( attrnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( attrnav->brow->ctx);
      }
      break;
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( attrnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case attrnav_eItemType_Local:
	  ((ItemLocal *)item)->close( attrnav, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( attrnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( attrnav->brow->ctx, object);
      if ( !brow_IsVisible( attrnav->brow->ctx, object))
        brow_CenterObject( attrnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( attrnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case attrnav_eItemType_Local:
          if ( ((ItemLocal *)item)->parent || ((ItemLocal *)item)->subgraph)
	    ((ItemLocal *)item)->open_children( attrnav, 0, 0);
          else if ( !((ItemLocal *)item)->parent && attrnav->change_value_cb)
	    (attrnav->change_value_cb) ( attrnav->parent_ctx);
          break;
        case attrnav_eItemType_Enum: 
        {
          int value;

          brow_GetRadiobutton( node_list[0], 0, &value);
          if ( !value)
          {
             brow_SetRadiobutton( node_list[0], 0, 1);
	     *(int *)((ItemEnum *)item)->value_p = ((ItemEnum *)item)->num;

             if ( ((ItemEnum *)item)->type_id == glow_eType_TraceType &&
                  attrnav->reconfigure_attr_cb) {
	       (attrnav->reconfigure_attr_cb)(attrnav->parent_ctx);
               return FLOW__DESTROYED;
	     }
          }
          break;
        }
        case attrnav_eItemType_Mask: 
        {
          int value;

          brow_GetRadiobutton( node_list[0], 0, &value);
          if ( value)
          {
             brow_SetRadiobutton( node_list[0], 0, 0);
	     *(unsigned int *)((ItemEnum *)item)->value_p &= 
			~(((ItemMask *)item)->mask);
          }
          else
          {
             brow_SetRadiobutton( node_list[0], 0, 1);
	     *(unsigned int *)((ItemEnum *)item)->value_p |= 
			((ItemMask *)item)->mask;
          }
          break;
        }
        default:
          ;
      }
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case attrnav_eItemType_Local: 
	      ((ItemLocal *)item)->open_children( attrnav,
			event->object.x, event->object.y);
              break;
            default:
              ;
          }
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_Radiobutton:
    {
      printf( "Radiobutton\n");
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case attrnav_eItemType_Enum: 
              if ( !event->radiobutton.value)
              {
                 brow_SetRadiobutton( event->radiobutton.object, 
			event->radiobutton.number, !event->radiobutton.value);
	         *(int *)((ItemEnum *)item)->value_p = ((ItemEnum *)item)->num;

                 if ( ((ItemEnum *)item)->type_id == glow_eType_TraceType &&
                      attrnav->reconfigure_attr_cb) {
	           (attrnav->reconfigure_attr_cb)(attrnav->parent_ctx);
                   return FLOW__DESTROYED;
                 }
              }
              break;
            case attrnav_eItemType_Mask: 
               brow_SetRadiobutton( event->radiobutton.object, 
			event->radiobutton.number, !event->radiobutton.value);
               if ( event->radiobutton.value)
	         *(unsigned int *)((ItemMask *)item)->value_p &= 
			~(((ItemMask *)item)->mask);
               else
	         *(unsigned int *)((ItemMask *)item)->value_p |= 
			((ItemMask *)item)->mask;
              break;
            default:
              ;
          }
          break;
        default:
          ;
      }


      break;
    }
    default:
      ;
  }
  return 1;
}

static void attrnav_trace_scan( AttrNav *attrnav)
{
  int time = 200;

  if ( attrnav->trace_started)
  {
    brow_TraceScan( attrnav->brow->ctx);

    attrnav->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(attrnav->brow_widget) , time,
	(XtTimerCallbackProc)attrnav_trace_scan, attrnav);
  }
}

void AttrNav::force_trace_scan()
{
  if ( trace_started)
    brow_TraceScan( brow->ctx);
}

static int attrnav_trace_scan_bc( brow_tObject object, void *p)
{
  ItemLocal		*base_item;
  char		buf[120];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case attrnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *)base_item;
      if ( !item->first_scan)
      {
        if ( item->size > (int) sizeof(item->old_value) && 
	     item->type_id == glow_eType_String &&
	     strlen((char *)p) < sizeof(item->old_value) && 
	     strcmp( (char *)p, item->old_value) == 0)
          // No change since last time
          return 1;
        else if ( memcmp( item->old_value, p, item->size) == 0)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      attrnav_attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
      brow_SetAnnotation( object, 1, buf, len);
      memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
      break;
    }
    case attrnav_eItemType_Enum:
    {
      ItemEnum	*item;

      item = (ItemEnum *)base_item;
      if ( !item->first_scan)
      {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *(int *)p == item->num)
        brow_SetRadiobutton( item->node, 0, 1);
      else
        brow_SetRadiobutton( item->node, 0, 0);

      item->old_value = *(int *) p;
      break;
    }
    case attrnav_eItemType_Mask:
    {
      ItemMask	*item;

      item = (ItemMask *)base_item;
      if ( !item->first_scan)
      {
        if ( item->old_value == *(int *)p)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *(unsigned int *)p & item->mask)
        brow_SetRadiobutton( item->node, 0, 1);
      else
        brow_SetRadiobutton( item->node, 0, 0);

      item->old_value = *(int *) p;
      break;
    }
    default:
      ;
  }
  return 1;
}

static int attrnav_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, /* flow_eDrawType color, */ void **p)
{
  ItemLocal 		*base_item;

  /*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case attrnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *) base_item;
      *p = item->value_p;
      break;
    }
    case attrnav_eItemType_Enum:
    {
      ItemEnum	*item;

      item = (ItemEnum *) base_item;
      *p = item->value_p;
      break;
    }
    case attrnav_eItemType_Mask:
    {
      ItemMask	*item;

      item = (ItemMask *) base_item;
      *p = item->value_p;
      break;
    }
    default:
      ;
  }      
  return 1;
}

static int attrnav_trace_disconnect_bc( brow_tObject object)
{
  ItemLocal 		*base_item;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    default:
      ;
  }
  return 1;
}


//
// Create nodeclasses
//
void AttrNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "NavigatorDefault", 
		flow_eNodeGroup_Common, &nc_object);
  brow_AddFrame( nc_object, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_object, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_object, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_object, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_object, 7, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_object, 11, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorAttr", 
		flow_eNodeGroup_Common, &nc_attr);
  brow_AddFrame( nc_attr, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create multiline attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorAttrMultiLine", 
		flow_eNodeGroup_Common, &nc_attr_multiline);
  brow_AddFrame( nc_attr_multiline, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_multiline, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_multiline, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_multiline, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_MultiLine, 
		1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorEnum", 
		flow_eNodeGroup_Common, &nc_enum);
  brow_AddRadiobutton( nc_enum, 12, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddFrame( nc_enum, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_enum, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_enum, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
 
  // Create table nodeclass

  brow_CreateNodeClass( ctx, "NavigatorTable", 
		flow_eNodeGroup_Common, &nc_table);
  brow_AddFrame( nc_table, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_table, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_table, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 12, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 16, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 20, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 24, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 28, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 32, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 35, 0.6, 8,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 38, 0.6, 9,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create Header

  brow_CreateNodeClass( ctx, "NavigatorHead", 
		flow_eNodeGroup_Common, &nc_header);
  brow_AddFrame( nc_header, 0, 0, 20, 0.8, flow_eDrawType_LineGray, 2, 1);
  brow_AddAnnotPixmap( nc_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create TableHeader

  brow_CreateNodeClass( ctx, "NavigatorTableHead", 
		flow_eNodeGroup_Common, &nc_table_header);
  brow_AddFrame( nc_table_header, 0, 0, 20, 0.8, flow_eDrawType_LineGray, 2, 1);
  brow_AddAnnotPixmap( nc_table_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 12, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 16, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 20, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 24, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 28, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 32, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 35, 0.6, 8,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 38, 0.6, 9,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

}

int	AttrNav::object_attr()
{
  attr_sItem *item_p;
  int	i;

  brow_SetNodraw( brow->ctx);

  item_p = itemlist;
  for ( i = 0; i < item_cnt; i++)
  {
    new ItemLocal( this, item_p->name, "LocalAttr", 
	item_p->type, item_p->size, item_p->minlimit, item_p->maxlimit,
	item_p->value, item_p->multiline, item_p->noedit, 
	NULL, flow_eDest_IntoLast);
    
    item_p++;
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return ATTRNAV__SUCCESS;
}

void AttrNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, attrnav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	attrnav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	attrnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int attrnav_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  AttrNav *attrnav = (AttrNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  int		sts;

  attrnav->brow = new AttrNavBrow( ctx, (void *)attrnav);

  attrnav->brow->brow_setup();
  attrnav->brow->create_nodeclasses();

  // Create the root item
  attrnav->object_attr();

  sts = brow_TraceInit( ctx, attrnav_trace_connect_bc, 
		attrnav_trace_disconnect_bc, attrnav_trace_scan_bc);
  attrnav->trace_started = 1;

  attrnav_trace_scan( attrnav);

  return 1;
}

ItemLocal::ItemLocal( AttrNav *attrnav, char *item_name, char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, void *attr_value_p, int attr_multiline,
	int attr_noedit,
	brow_tNode dest, flow_eDest dest_code) :
	value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
	min_limit(attr_min_limit), max_limit(attr_max_limit),
	multiline(attr_multiline), noedit(attr_noedit), parent(0),
	subgraph(0)
{

  type = attrnav_eItemType_Local;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  if ( !multiline)
    brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

  else
    brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_attr_multiline, 
		dest, dest_code, (void *) this, 1, &node);
  switch( type_id)
  {
    case glow_eType_TraceType:
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Access:
    case glow_eType_Cycle:
    case glow_eType_MB3Action:
      if ( !noedit)
      {
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
        parent = 1;
      }
      else
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
      break;
    default:
    {
      if ( strcmp( name, "SubGraph") == 0)
      {
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
        subgraph = 1;
      }
      else
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
    }
  }

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

int ItemLocal::open_children( AttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else if ( parent && !noedit)
  {
    attrnav_sEnumElement	*elem_p;
    attrnav_sEnum		*enum_p;
    int				found;

    found = 0;
    for ( enum_p = enum_types; enum_p->elements; enum_p++)
    {
      if ( enum_p->num == type_id)
      {
        elem_p = enum_p->elements;
        found = 1;
        break;
      }
    }
    if ( found)
    {
      // Create some children
      brow_SetNodraw( attrnav->brow->ctx);

      for ( ; elem_p->name[0] != 0; elem_p++)
      {
        new ItemEnum( attrnav, elem_p->name, elem_p->num, type_id, 
		this->value_p, node, flow_eDest_IntoLast);
      }
    }
    else
    {
      for ( enum_p = mask_types; enum_p->elements; enum_p++)
      {
        if ( enum_p->num == type_id)
        {
          elem_p = enum_p->elements;
          found = 1;
          break;
        }
      }
      if ( !found)
        return 0;

      // Create some children
      brow_SetNodraw( attrnav->brow->ctx);

      for ( ; elem_p->name[0] != 0; elem_p++)
      {
        new ItemMask( attrnav, elem_p->name, (unsigned int) elem_p->num, 
		type_id, this->value_p, node, flow_eDest_IntoLast);
      }
    }

    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  else if ( subgraph)
  {
    attr_sItem  *itemlist, *item_p;
    int 	item_cnt;
    int		sts;
    int		i;

    sts = (attrnav->get_subgraph_info_cb)( attrnav->parent_ctx, 
	(char *)value_p, &itemlist, &item_cnt);
    if ( EVEN(sts)) return 0;

    brow_SetNodraw( attrnav->brow->ctx);

    item_p = itemlist;
    for ( i = 0; i < item_cnt; i++)
    {
      new ItemLocal( attrnav, item_p->name, "LocalAttr", 
	item_p->type, item_p->size, item_p->minlimit, item_p->maxlimit,
	item_p->value, item_p->multiline, item_p->noedit, 
	node, flow_eDest_IntoLast);
      item_p++;
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

int ItemLocal::close( AttrNav *attrnav, double x, double y)
{
  double	node_x, node_y;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( attrnav->brow->ctx);
    brow_CloseNode( attrnav->brow->ctx, node);
    if ( brow_IsOpen( node) & attrnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & attrnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
    brow_ResetOpen( node, attrnav_mOpen_All);
    brow_ResetNodraw( attrnav->brow->ctx);
    brow_Redraw( attrnav->brow->ctx, node_y);
  }
  return 1;
}

ItemEnum::ItemEnum( AttrNav *attrnav, char *item_name, int item_num, 
	int item_type_id, void *attr_value_p, 
	brow_tNode dest, flow_eDest dest_code) :
	num(item_num), type_id(item_type_id), value_p(attr_value_p), first_scan(1)
{

  type = attrnav_eItemType_Enum;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_enum, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  if ( *(int *)value_p == num)
    brow_SetRadiobutton( node, 0, 1);
  else
    brow_SetRadiobutton( node, 0, 0);
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}


ItemMask::ItemMask( AttrNav *attrnav, char *item_name, unsigned int item_mask, 
	int item_type_id, void *attr_value_p, 
	brow_tNode dest, flow_eDest dest_code) :
	mask(item_mask), type_id(item_type_id), value_p(attr_value_p), first_scan(1)
{

  type = attrnav_eItemType_Mask;

  strcpy( name, item_name);

  brow_CreateNode( attrnav->brow->ctx, item_name, attrnav->brow->nc_enum, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  if ( *(unsigned int *)value_p & mask)
    brow_SetRadiobutton( node, 0, 1);
  else
    brow_SetRadiobutton( node, 0, 0);
  brow_SetTraceAttr( node, name, "", flow_eTraceType_User);
}


