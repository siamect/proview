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
#include "ge_dyn.h"
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

static attrnav_sEnumElement elem_dyn_type[] = {
	{ (int) ge_mDynType_Inherit		, "Inherit"},
	{ (int) ge_mDynType_Tone		, "Tone"},
	{ (int) ge_mDynType_DigLowColor		, "DigLowColor"},
	{ (int) ge_mDynType_DigColor		, "DigColor"},
	{ (int) ge_mDynType_AnalogColor		, "AnalogColor"},
	{ (int) ge_mDynType_DigWarning		, "DigWarning"},
	{ (int) ge_mDynType_DigError		, "DigError"},
	{ (int) ge_mDynType_DigFlash		, "DigFlash"},
	{ (int) ge_mDynType_FillLevel       	, "FillLevel"},
	{ (int) ge_mDynType_Invisible		, "Invisible"},
	{ (int) ge_mDynType_DigBorder		, "DigBorder"},
	{ (int) ge_mDynType_DigText		, "DigText"},
	{ (int) ge_mDynType_AnalogText		, "AnalogText"},
	{ (int) ge_mDynType_Value		, "Value"},
	{ (int) ge_mDynType_Rotate		, "Rotate"},
	{ (int) ge_mDynType_Move		, "Move"},
	{ (int) ge_mDynType_AnalogShift		, "AnalogShift"},
	{ (int) ge_mDynType_DigShift		, "DigShift"},
	{ (int) ge_mDynType_Animation		, "Animation"},
	{ (int) ge_mDynType_Bar			, "Bar"},
	{ (int) ge_mDynType_Trend		, "Trend"},
	{ (int) ge_mDynType_FastCurve		, "FastCurve"},
	{ (int) ge_mDynType_SliderBackground   	, "SliderBackground"},
	{ (int) ge_mDynType_Video		, "Video"},
	{ (int) ge_mDynType_Table		, "Table"},
	{ 0, ""}};

static attrnav_sEnumElement elem_dyn_type_tone[] = {
	{ (int) ge_mDynType_Inherit		, "Inherit"},
	{ (int) ge_mDynType_Tone		, "Tone"},
	{ (int) ge_mDynType_DigLowColor		, "DigLowTone"},
	{ (int) ge_mDynType_DigColor		, "DigTone"},
	{ (int) ge_mDynType_AnalogColor		, "AnalogTone"},
	{ (int) ge_mDynType_DigWarning     	, "DigToneWarning"},
	{ (int) ge_mDynType_DigError       	, "DigToneError"},
	{ (int) ge_mDynType_DigFlash       	, "DigFlash"},
	{ (int) ge_mDynType_FillLevel       	, "FillLevel"},
	{ (int) ge_mDynType_Invisible		, "Invisible"},
	{ (int) ge_mDynType_DigBorder		, "DigBorder"},
	{ (int) ge_mDynType_DigText		, "DigText"},
	{ (int) ge_mDynType_AnalogText		, "AnalogText"},
	{ (int) ge_mDynType_Value		, "Value"},
	{ (int) ge_mDynType_Rotate		, "Rotate"},
	{ (int) ge_mDynType_Move		, "Move"},
	{ (int) ge_mDynType_AnalogShift		, "AnalogShift"},
	{ (int) ge_mDynType_DigShift		, "DigShift"},
	{ (int) ge_mDynType_Animation		, "Animation"},
	{ (int) ge_mDynType_Bar			, "Bar"},
	{ (int) ge_mDynType_Trend		, "Trend"},
	{ (int) ge_mDynType_FastCurve		, "FastCurve"},
	{ (int) ge_mDynType_SliderBackground   	, "SliderBackground"},
	{ (int) ge_mDynType_Video		, "Video"},
	{ (int) ge_mDynType_Table		, "Table"},
	{ 0, ""}};

static attrnav_sEnumElement elem_action_type[] = {
	{ (int) ge_mActionType_Inherit		, "Inherit"},
	{ (int) ge_mActionType_PopupMenu	, "PopupMenu"},
	{ (int) ge_mActionType_SetDig		, "SetDig"},
	{ (int) ge_mActionType_ResetDig		, "ResetDig"},
	{ (int) ge_mActionType_ToggleDig	, "ToggleDig"},
	{ (int) ge_mActionType_StoDig		, "StoDig"},
	{ (int) ge_mActionType_Command		, "Command"},
	{ (int) ge_mActionType_CommandDoubleClick , "CommandDoubleClick"},
	{ (int) ge_mActionType_Help		, "Help"},
	{ (int) ge_mActionType_OpenGraph	, "OpenGraph"},
	{ (int) ge_mActionType_CloseGraph	, "CloseGraph"},
	{ (int) ge_mActionType_OpenURL		, "OpenURL"},
	{ (int) ge_mActionType_Confirm		, "Confirm"},
	{ (int) ge_mActionType_IncrAnalog	, "IncrAnalog"},
	{ (int) ge_mActionType_RadioButton	, "RadioButton"},
	{ (int) ge_mActionType_ValueInput	, "ValueInput"},
	{ (int) ge_mActionType_TipText		, "ToolTip"},
	{ (int) ge_mActionType_InputFocus      	, "InputFocus"},
	{ (int) ge_mActionType_PulldownMenu     , "PulldownMenu"},
	{ (int) ge_mActionType_OptionMenu       , "OptionMenu"},
	{ (int) ge_mActionType_Slider		, "Slider"},
	{ 0, ""}};

static attrnav_sEnumElement elem_color[] = {
	{ (int) glow_eDrawType_Line, 		"Black"},
	{ (int) glow_eDrawType_LineRed, 	"Red"},
	{ (int) glow_eDrawType_LineGray, 	"Grey"},
	{ (int) glow_eDrawType_Color4,	 	"White"},
	{ (int) glow_eDrawType_Color5, 		"YellowGreen"},
	{ (int) glow_eDrawType_Color6, 		"Yellow"},
	{ (int) glow_eDrawType_Color7, 		"Gold"},
	{ (int) glow_eDrawType_Color8, 		"Orange"},
	{ (int) glow_eDrawType_Color9, 		"OrangeRed"},
	{ (int) glow_eDrawType_Color10, 	"Red"},
	{ (int) glow_eDrawType_Color11, 	"RedViolet"},
	{ (int) glow_eDrawType_Color12, 	"Violet"},
	{ (int) glow_eDrawType_Color13, 	"BlueViolet"},
	{ (int) glow_eDrawType_Color14, 	"BlueBlueViolet"},
	{ (int) glow_eDrawType_Color15, 	"Blue"},
	{ (int) glow_eDrawType_Color16, 	"BlueBlueGreen"},
	{ (int) glow_eDrawType_Color17, 	"BlueGreen"},
	{ (int) glow_eDrawType_Color18, 	"GreenGreenBlue"},
	{ (int) glow_eDrawType_Color19, 	"Green"},
	{ (int) glow_eDrawType_Color20, 	"GreenGreenYellow"},
	{ (int) glow_eDrawType_Color21, 	"GrayFix1"},
	{ (int) glow_eDrawType_Color22, 	"GrayFix2"},
	{ (int) glow_eDrawType_Color23, 	"GrayFix3"},
	{ (int) glow_eDrawType_Color24, 	"GrayFix4"},
	{ (int) glow_eDrawType_Color25, 	"GrayFix5"},
	{ (int) glow_eDrawType_Color26, 	"GrayFix6"},
	{ (int) glow_eDrawType_Color27, 	"GrayFix7"},
	{ (int) glow_eDrawType_Color28, 	"GrayFix8"},
	{ (int) glow_eDrawType_Color29, 	"GrayFix9"},
	{ (int) glow_eDrawType_Color30, 	"GrayFix10"},
	{ (int) glow_eDrawType_Color31, 	"GrayLow1"},
	{ (int) glow_eDrawType_Color32, 	"GrayLow2"},
	{ (int) glow_eDrawType_Color33, 	"GrayLow3"},
	{ (int) glow_eDrawType_Color34, 	"GrayLow4"},
	{ (int) glow_eDrawType_Color35, 	"GrayLow5"},
	{ (int) glow_eDrawType_Color36, 	"GrayLow6"},
	{ (int) glow_eDrawType_Color37, 	"GrayLow7"},
	{ (int) glow_eDrawType_Color38, 	"GrayLow8"},
	{ (int) glow_eDrawType_Color39, 	"GrayLow9"},
	{ (int) glow_eDrawType_Color40, 	"GrayLow10"},
	{ (int) glow_eDrawType_Color41, 	"GrayMedium1"},
	{ (int) glow_eDrawType_Color42, 	"GrayMedium2"},
	{ (int) glow_eDrawType_Color43, 	"GrayMedium3"},
	{ (int) glow_eDrawType_Color44, 	"GrayMedium4"},
	{ (int) glow_eDrawType_Color45, 	"GrayMedium5"},
	{ (int) glow_eDrawType_Color46, 	"GrayMedium6"},
	{ (int) glow_eDrawType_Color47, 	"GrayMedium7"},
	{ (int) glow_eDrawType_Color48, 	"GrayMedium8"},
	{ (int) glow_eDrawType_Color49, 	"GrayMedium9"},
	{ (int) glow_eDrawType_Color50, 	"GrayMedium10"},
	{ (int) glow_eDrawType_Color51, 	"GrayHigh1"},
	{ (int) glow_eDrawType_Color52, 	"GrayHigh2"},
	{ (int) glow_eDrawType_Color53, 	"GrayHigh3"},
	{ (int) glow_eDrawType_Color54, 	"GrayHigh4"},
	{ (int) glow_eDrawType_Color55, 	"GrayHigh5"},
	{ (int) glow_eDrawType_Color56, 	"GrayHigh6"},
	{ (int) glow_eDrawType_Color57, 	"GrayHigh7"},
	{ (int) glow_eDrawType_Color58, 	"GrayHigh8"},
	{ (int) glow_eDrawType_Color59, 	"GrayHigh9"},
	{ (int) glow_eDrawType_Color60, 	"GrayHigh10"},
	{ (int) glow_eDrawType_Color61, 	"YellowGreenLow1"},
	{ (int) glow_eDrawType_Color62, 	"YellowGreenLow2"},
	{ (int) glow_eDrawType_Color63, 	"YellowGreenLow3"},
	{ (int) glow_eDrawType_Color64, 	"YellowGreenLow4"},
	{ (int) glow_eDrawType_Color65, 	"YellowGreenLow5"},
	{ (int) glow_eDrawType_Color66, 	"YellowGreenLow6"},
	{ (int) glow_eDrawType_Color67, 	"YellowGreenLow7"},
	{ (int) glow_eDrawType_Color68, 	"YellowGreenLow8"},
	{ (int) glow_eDrawType_Color69, 	"YellowGreenLow9"},
	{ (int) glow_eDrawType_Color70, 	"YellowGreenLow10"},
	{ (int) glow_eDrawType_Color71, 	"YellowGreenMedium1"},
	{ (int) glow_eDrawType_Color72, 	"YellowGreenMedium2"},
	{ (int) glow_eDrawType_Color73, 	"YellowGreenMedium3"},
	{ (int) glow_eDrawType_Color74, 	"YellowGreenMedium4"},
	{ (int) glow_eDrawType_Color75, 	"YellowGreenMedium5"},
	{ (int) glow_eDrawType_Color76, 	"YellowGreenMedium6"},
	{ (int) glow_eDrawType_Color77, 	"YellowGreenMedium7"},
	{ (int) glow_eDrawType_Color78, 	"YellowGreenMedium8"},
	{ (int) glow_eDrawType_Color79, 	"YellowGreenMedium9"},
	{ (int) glow_eDrawType_Color80, 	"YellowGreenMedium10"},
	{ (int) glow_eDrawType_Color81, 	"YellowGreenHigh1"},
	{ (int) glow_eDrawType_Color82, 	"YellowGreenHigh2"},
	{ (int) glow_eDrawType_Color83, 	"YellowGreenHigh3"},
	{ (int) glow_eDrawType_Color84, 	"YellowGreenHigh4"},
	{ (int) glow_eDrawType_Color85, 	"YellowGreenHigh5"},
	{ (int) glow_eDrawType_Color86, 	"YellowGreenHigh6"},
	{ (int) glow_eDrawType_Color87, 	"YellowGreenHigh7"},
	{ (int) glow_eDrawType_Color88, 	"YellowGreenHigh8"},
	{ (int) glow_eDrawType_Color89, 	"YellowGreenHigh9"},
	{ (int) glow_eDrawType_Color90, 	"YellowGreenHigh10"},
	{ (int) glow_eDrawType_Color91, 	"YellowLow1"},
	{ (int) glow_eDrawType_Color92, 	"YellowLow2"},
	{ (int) glow_eDrawType_Color93, 	"YellowLow3"},
	{ (int) glow_eDrawType_Color94, 	"YellowLow4"},
	{ (int) glow_eDrawType_Color95, 	"YellowLow5"},
	{ (int) glow_eDrawType_Color96, 	"YellowLow6"},
	{ (int) glow_eDrawType_Color97, 	"YellowLow7"},
	{ (int) glow_eDrawType_Color98, 	"YellowLow8"},
	{ (int) glow_eDrawType_Color99, 	"YellowLow9"},
	{ (int) glow_eDrawType_Color100, 	"YellowLow10"},
	{ (int) glow_eDrawType_Color101, 	"YellowMedium1"},
	{ (int) glow_eDrawType_Color102, 	"YellowMedium2"},
	{ (int) glow_eDrawType_Color103, 	"YellowMedium3"},
	{ (int) glow_eDrawType_Color104, 	"YellowMedium4"},
	{ (int) glow_eDrawType_Color105, 	"YellowMedium5"},
	{ (int) glow_eDrawType_Color106, 	"YellowMedium6"},
	{ (int) glow_eDrawType_Color107, 	"YellowMedium7"},
	{ (int) glow_eDrawType_Color108, 	"YellowMedium8"},
	{ (int) glow_eDrawType_Color109, 	"YellowMedium9"},
	{ (int) glow_eDrawType_Color110, 	"YellowMedium10"},
	{ (int) glow_eDrawType_Color111, 	"YellowHigh1"},
	{ (int) glow_eDrawType_Color112, 	"YellowHigh2"},
	{ (int) glow_eDrawType_Color113, 	"YellowHigh3"},
	{ (int) glow_eDrawType_Color114, 	"YellowHigh4"},
	{ (int) glow_eDrawType_Color115, 	"YellowHigh5"},
	{ (int) glow_eDrawType_Color116, 	"YellowHigh6"},
	{ (int) glow_eDrawType_Color117, 	"YellowHigh7"},
	{ (int) glow_eDrawType_Color118, 	"YellowHigh8"},
	{ (int) glow_eDrawType_Color119, 	"YellowHigh9"},
	{ (int) glow_eDrawType_Color120, 	"YellowHigh10"},
	{ (int) glow_eDrawType_Color121, 	"OrangeLow1"},
	{ (int) glow_eDrawType_Color122, 	"OrangeLow2"},
	{ (int) glow_eDrawType_Color123, 	"OrangeLow3"},
	{ (int) glow_eDrawType_Color124, 	"OrangeLow4"},
	{ (int) glow_eDrawType_Color125, 	"OrangeLow5"},
	{ (int) glow_eDrawType_Color126, 	"OrangeLow6"},
	{ (int) glow_eDrawType_Color127, 	"OrangeLow7"},
	{ (int) glow_eDrawType_Color128, 	"OrangeLow8"},
	{ (int) glow_eDrawType_Color129, 	"OrangeLow9"},
	{ (int) glow_eDrawType_Color130, 	"OrangeLow10"},
	{ (int) glow_eDrawType_Color131, 	"OrangeMedium1"},
	{ (int) glow_eDrawType_Color132, 	"OrangeMedium2"},
	{ (int) glow_eDrawType_Color133, 	"OrangeMedium3"},
	{ (int) glow_eDrawType_Color134, 	"OrangeMedium4"},
	{ (int) glow_eDrawType_Color135, 	"OrangeMedium5"},
	{ (int) glow_eDrawType_Color136, 	"OrangeMedium6"},
	{ (int) glow_eDrawType_Color137, 	"OrangeMedium7"},
	{ (int) glow_eDrawType_Color138, 	"OrangeMedium8"},
	{ (int) glow_eDrawType_Color139, 	"OrangeMedium9"},
	{ (int) glow_eDrawType_Color140, 	"OrangeMedium10"},
	{ (int) glow_eDrawType_Color141, 	"OrangeHigh1"},
	{ (int) glow_eDrawType_Color142, 	"OrangeHigh2"},
	{ (int) glow_eDrawType_Color143, 	"OrangeHigh3"},
	{ (int) glow_eDrawType_Color144, 	"OrangeHigh4"},
	{ (int) glow_eDrawType_Color145, 	"OrangeHigh5"},
	{ (int) glow_eDrawType_Color146, 	"OrangeHigh6"},
	{ (int) glow_eDrawType_Color147, 	"OrangeHigh7"},
	{ (int) glow_eDrawType_Color148, 	"OrangeHigh8"},
	{ (int) glow_eDrawType_Color149, 	"OrangeHigh9"},
	{ (int) glow_eDrawType_Color150, 	"OrangeHigh10"},
	{ (int) glow_eDrawType_Color151, 	"RedLow1"},
	{ (int) glow_eDrawType_Color152, 	"RedLow2"},
	{ (int) glow_eDrawType_Color153, 	"RedLow3"},
	{ (int) glow_eDrawType_Color154, 	"RedLow4"},
	{ (int) glow_eDrawType_Color155, 	"RedLow5"},
	{ (int) glow_eDrawType_Color156, 	"RedLow6"},
	{ (int) glow_eDrawType_Color157, 	"RedLow7"},
	{ (int) glow_eDrawType_Color158, 	"RedLow8"},
	{ (int) glow_eDrawType_Color159, 	"RedLow9"},
	{ (int) glow_eDrawType_Color160, 	"RedLow10"},
	{ (int) glow_eDrawType_Color161, 	"RedMedium1"},
	{ (int) glow_eDrawType_Color162, 	"RedMedium2"},
	{ (int) glow_eDrawType_Color163, 	"RedMedium3"},
	{ (int) glow_eDrawType_Color164, 	"RedMedium4"},
	{ (int) glow_eDrawType_Color165, 	"RedMedium5"},
	{ (int) glow_eDrawType_Color166, 	"RedMedium6"},
	{ (int) glow_eDrawType_Color167, 	"RedMedium7"},
	{ (int) glow_eDrawType_Color168, 	"RedMedium8"},
	{ (int) glow_eDrawType_Color169, 	"RedMedium9"},
	{ (int) glow_eDrawType_Color170, 	"RedMedium10"},
	{ (int) glow_eDrawType_Color171, 	"RedHigh1"},
	{ (int) glow_eDrawType_Color172, 	"RedHigh2"},
	{ (int) glow_eDrawType_Color173, 	"RedHigh3"},
	{ (int) glow_eDrawType_Color174, 	"RedHigh4"},
	{ (int) glow_eDrawType_Color175, 	"RedHigh5"},
	{ (int) glow_eDrawType_Color176, 	"RedHigh6"},
	{ (int) glow_eDrawType_Color177, 	"RedHigh7"},
	{ (int) glow_eDrawType_Color178, 	"RedHigh8"},
	{ (int) glow_eDrawType_Color179, 	"RedHigh9"},
	{ (int) glow_eDrawType_Color180, 	"RedHigh10"},
	{ (int) glow_eDrawType_Color181, 	"MagentaLow1"},
	{ (int) glow_eDrawType_Color182, 	"MagentaLow2"},
	{ (int) glow_eDrawType_Color183, 	"MagentaLow3"},
	{ (int) glow_eDrawType_Color184, 	"MagentaLow4"},
	{ (int) glow_eDrawType_Color185, 	"MagentaLow5"},
	{ (int) glow_eDrawType_Color186, 	"MagentaLow6"},
	{ (int) glow_eDrawType_Color187, 	"MagentaLow7"},
	{ (int) glow_eDrawType_Color188, 	"MagentaLow8"},
	{ (int) glow_eDrawType_Color189, 	"MagentaLow9"},
	{ (int) glow_eDrawType_Color190, 	"MagentaLow10"},
	{ (int) glow_eDrawType_Color191, 	"MagentaMedium1"},
	{ (int) glow_eDrawType_Color192, 	"MagentaMedium2"},
	{ (int) glow_eDrawType_Color193, 	"MagentaMedium3"},
	{ (int) glow_eDrawType_Color194, 	"MagentaMedium4"},
	{ (int) glow_eDrawType_Color195, 	"MagentaMedium5"},
	{ (int) glow_eDrawType_Color196, 	"MagentaMedium6"},
	{ (int) glow_eDrawType_Color197, 	"MagentaMedium7"},
	{ (int) glow_eDrawType_Color198, 	"MagentaMedium8"},
	{ (int) glow_eDrawType_Color199, 	"MagentaMedium9"},
	{ (int) glow_eDrawType_Color200, 	"MagentaMedium10"},
	{ (int) glow_eDrawType_Color201, 	"MagentaHigh1"},
	{ (int) glow_eDrawType_Color202, 	"MagentaHigh2"},
	{ (int) glow_eDrawType_Color203, 	"MagentaHigh3"},
	{ (int) glow_eDrawType_Color204, 	"MagentaHigh4"},
	{ (int) glow_eDrawType_Color205, 	"MagentaHigh5"},
	{ (int) glow_eDrawType_Color206, 	"MagentaHigh6"},
	{ (int) glow_eDrawType_Color207, 	"MagentaHigh7"},
	{ (int) glow_eDrawType_Color208, 	"MagentaHigh8"},
	{ (int) glow_eDrawType_Color209, 	"MagentaHigh9"},
	{ (int) glow_eDrawType_Color210, 	"MagentaHigh10"},
	{ (int) glow_eDrawType_Color211, 	"BlueLow1"},
	{ (int) glow_eDrawType_Color212, 	"BlueLow2"},
	{ (int) glow_eDrawType_Color213, 	"BlueLow3"},
	{ (int) glow_eDrawType_Color214, 	"BlueLow4"},
	{ (int) glow_eDrawType_Color215, 	"BlueLow5"},
	{ (int) glow_eDrawType_Color216, 	"BlueLow6"},
	{ (int) glow_eDrawType_Color217, 	"BlueLow7"},
	{ (int) glow_eDrawType_Color218, 	"BlueLow8"},
	{ (int) glow_eDrawType_Color219, 	"BlueLow9"},
	{ (int) glow_eDrawType_Color220, 	"BlueLow10"},
	{ (int) glow_eDrawType_Color221, 	"BlueMedium1"},
	{ (int) glow_eDrawType_Color222, 	"BlueMedium2"},
	{ (int) glow_eDrawType_Color223, 	"BlueMedium3"},
	{ (int) glow_eDrawType_Color224, 	"BlueMedium4"},
	{ (int) glow_eDrawType_Color225, 	"BlueMedium5"},
	{ (int) glow_eDrawType_Color226, 	"BlueMedium6"},
	{ (int) glow_eDrawType_Color227, 	"BlueMedium7"},
	{ (int) glow_eDrawType_Color228, 	"BlueMedium8"},
	{ (int) glow_eDrawType_Color229, 	"BlueMedium9"},
	{ (int) glow_eDrawType_Color230, 	"BlueMedium10"},
	{ (int) glow_eDrawType_Color231, 	"BlueHigh1"},
	{ (int) glow_eDrawType_Color232, 	"BlueHigh2"},
	{ (int) glow_eDrawType_Color233, 	"BlueHigh3"},
	{ (int) glow_eDrawType_Color234, 	"BlueHigh4"},
	{ (int) glow_eDrawType_Color235, 	"BlueHigh5"},
	{ (int) glow_eDrawType_Color236, 	"BlueHigh6"},
	{ (int) glow_eDrawType_Color237, 	"BlueHigh7"},
	{ (int) glow_eDrawType_Color238, 	"BlueHigh8"},
	{ (int) glow_eDrawType_Color239, 	"BlueHigh9"},
	{ (int) glow_eDrawType_Color240, 	"BlueHigh10"},
	{ (int) glow_eDrawType_Color241, 	"SeaBlueLow1"},
	{ (int) glow_eDrawType_Color242, 	"SeaBlueLow2"},
	{ (int) glow_eDrawType_Color243, 	"SeaBlueLow3"},
	{ (int) glow_eDrawType_Color244, 	"SeaBlueLow4"},
	{ (int) glow_eDrawType_Color245, 	"SeaBlueLow5"},
	{ (int) glow_eDrawType_Color246, 	"SeaBlueLow6"},
	{ (int) glow_eDrawType_Color247, 	"SeaBlueLow7"},
	{ (int) glow_eDrawType_Color248, 	"SeaBlueLow8"},
	{ (int) glow_eDrawType_Color249, 	"SeaBlueLow9"},
	{ (int) glow_eDrawType_Color250, 	"SeaBlueLow10"},
	{ (int) glow_eDrawType_Color251, 	"SeaBlueMedium1"},
	{ (int) glow_eDrawType_Color252, 	"SeaBlueMedium2"},
	{ (int) glow_eDrawType_Color253, 	"SeaBlueMedium3"},
	{ (int) glow_eDrawType_Color224, 	"SeaBlueMedium4"},
	{ (int) glow_eDrawType_Color255, 	"SeaBlueMedium5"},
	{ (int) glow_eDrawType_Color256, 	"SeaBlueMedium6"},
	{ (int) glow_eDrawType_Color257, 	"SeaBlueMedium7"},
	{ (int) glow_eDrawType_Color258, 	"SeaBlueMedium8"},
	{ (int) glow_eDrawType_Color259, 	"SeaBlueMedium9"},
	{ (int) glow_eDrawType_Color260, 	"SeaBlueMedium10"},
	{ (int) glow_eDrawType_Color261, 	"SeaBlueHigh1"},
	{ (int) glow_eDrawType_Color262, 	"SeaBlueHigh2"},
	{ (int) glow_eDrawType_Color263, 	"SeaBlueHigh3"},
	{ (int) glow_eDrawType_Color264, 	"SeaBlueHigh4"},
	{ (int) glow_eDrawType_Color265, 	"SeaBlueHigh5"},
	{ (int) glow_eDrawType_Color266, 	"SeaBlueHigh6"},
	{ (int) glow_eDrawType_Color267, 	"SeaBlueHigh7"},
	{ (int) glow_eDrawType_Color268, 	"SeaBlueHigh8"},
	{ (int) glow_eDrawType_Color269, 	"SeaBlueHigh9"},
	{ (int) glow_eDrawType_Color270, 	"SeaBlueHigh10"},
	{ (int) glow_eDrawType_Color271, 	"GreenLow1"},
	{ (int) glow_eDrawType_Color272, 	"GreenLow2"},
	{ (int) glow_eDrawType_Color273, 	"GreenLow3"},
	{ (int) glow_eDrawType_Color274, 	"GreenLow4"},
	{ (int) glow_eDrawType_Color275, 	"GreenLow5"},
	{ (int) glow_eDrawType_Color276, 	"GreenLow6"},
	{ (int) glow_eDrawType_Color277, 	"GreenLow7"},
	{ (int) glow_eDrawType_Color278, 	"GreenLow8"},
	{ (int) glow_eDrawType_Color279, 	"GreenLow9"},
	{ (int) glow_eDrawType_Color280, 	"GreenLow10"},
	{ (int) glow_eDrawType_Color281, 	"GreenMedium1"},
	{ (int) glow_eDrawType_Color282, 	"GreenMedium2"},
	{ (int) glow_eDrawType_Color283, 	"GreenMedium3"},
	{ (int) glow_eDrawType_Color284, 	"GreenMedium4"},
	{ (int) glow_eDrawType_Color285, 	"GreenMedium5"},
	{ (int) glow_eDrawType_Color286, 	"GreenMedium6"},
	{ (int) glow_eDrawType_Color287, 	"GreenMedium7"},
	{ (int) glow_eDrawType_Color288, 	"GreenMedium8"},
	{ (int) glow_eDrawType_Color289, 	"GreenMedium9"},
	{ (int) glow_eDrawType_Color290, 	"GreenMedium10"},
	{ (int) glow_eDrawType_Color291, 	"GreenHigh1"},
	{ (int) glow_eDrawType_Color292, 	"GreenHigh2"},
	{ (int) glow_eDrawType_Color293, 	"GreenHigh3"},
	{ (int) glow_eDrawType_Color294, 	"GreenHigh4"},
	{ (int) glow_eDrawType_Color295, 	"GreenHigh5"},
	{ (int) glow_eDrawType_Color296, 	"GreenHigh6"},
	{ (int) glow_eDrawType_Color297, 	"GreenHigh7"},
	{ (int) glow_eDrawType_Color298, 	"GreenHigh8"},
	{ (int) glow_eDrawType_Color299, 	"GreenHigh9"},
	{ (int) glow_eDrawType_Color300, 	"GreenHigh10"},
	{ (int) glow_eDrawType_Inherit, 	"Inherit"},
	{ 0, ""}};

static attrnav_sEnumElement elem_tone[] = {
	{ (int) glow_eDrawTone_No, 	"No"},
	{ (int) glow_eDrawTone_Gray, 	"Gray"},
	{ (int) glow_eDrawTone_YellowGreen,	"YellowGreen"},
	{ (int) glow_eDrawTone_Yellow, 	"Yellow"},
	{ (int) glow_eDrawTone_Orange, 	"Orange"},
	{ (int) glow_eDrawTone_Red, 	"Red"},
	{ (int) glow_eDrawTone_Magenta, "Magenta"},
	{ (int) glow_eDrawTone_Blue, 	"Blue"},
	{ (int) glow_eDrawTone_Seablue, "Seablue"},
	{ (int) glow_eDrawTone_Green, 	"Green"},
	{ (int) glow_eDrawTone_DarkGray, "DarkGray"},
	{ 0, ""}};

static attrnav_sEnumElement elem_tone_or_color[] = {
	{ (int) glow_eDrawTone_No, 		"NoTone"},
	{ (int) glow_eDrawTone_Gray, 		"ToneGray"},
	{ (int) glow_eDrawTone_YellowGreen,     "ToneYellowGreen"},
	{ (int) glow_eDrawTone_Yellow, 		"ToneYellow"},
	{ (int) glow_eDrawTone_Orange, 		"ToneOrange"},
	{ (int) glow_eDrawTone_Red, 		"ToneRed"},
	{ (int) glow_eDrawTone_Magenta, 	"ToneMagenta"},
	{ (int) glow_eDrawTone_Blue, 		"ToneBlue"},
	{ (int) glow_eDrawTone_Seablue, 	"ToneSeablue"},
	{ (int) glow_eDrawTone_Green, 		"ToneGreen"},
	{ (int) glow_eDrawTone_DarkGray,	"ToneDarkGray"},
	{ (int) glow_eDrawTone_LightGray,	"ToneLightGray"},
	{ (int) glow_eDrawTone_DarkYellowGreen,			"ToneDarkYellowGreen"},
	{ (int) glow_eDrawTone_LightYellowGreen,	       	"ToneLightYellowGreen"},
	{ (int) glow_eDrawTone_YellowGreenHighSaturation,      	"ToneYellowGreenHighSaturation"},
	{ (int) glow_eDrawTone_YellowGreenLowSaturation,       	"ToneYellowGreenLowSaturation"},
	{ (int) glow_eDrawTone_DarkYellowGreenHighSaturation,	"ToneDarkYellowGreenHighSaturation"},
	{ (int) glow_eDrawTone_DarkYellowGreenLowSaturation,   	"ToneDarkYellowGreenLowSaturation"},
	{ (int) glow_eDrawTone_LightYellowGreenHighSaturation,	"ToneLightYellowGreenHighSaturation"},
	{ (int) glow_eDrawTone_LightYellowGreenLowSaturation,	"ToneLightYellowGreenLowSaturation"},
	{ (int) glow_eDrawTone_DarkYellow,			"ToneDarkYellow"},
	{ (int) glow_eDrawTone_LightYellow,			"ToneLightYellow"},
	{ (int) glow_eDrawTone_YellowHighSaturation,		"ToneYellowHighSaturation"},
	{ (int) glow_eDrawTone_YellowLowSaturation,		"ToneYellowLowSaturation"},
	{ (int) glow_eDrawTone_DarkYellowHighSaturation,       	"ToneDarkYellowHighSaturation"},
	{ (int) glow_eDrawTone_DarkYellowLowSaturation,		"ToneDarkYellowLowSaturation"},
	{ (int) glow_eDrawTone_LightYellowHighSaturation,      	"ToneLightYellowHighSaturation"},
	{ (int) glow_eDrawTone_LightYellowLowSaturation,       	"ToneLightYellowLowSaturation"},
	{ (int) glow_eDrawTone_DarkOrange,			"ToneDarkOrange"},
	{ (int) glow_eDrawTone_LightOrange,			"ToneLightOrange"},
	{ (int) glow_eDrawTone_OrangeHighSaturation,		"ToneOrangeHighSaturation"},
	{ (int) glow_eDrawTone_OrangeLowSaturation,		"ToneOrangeLowSaturation"},
	{ (int) glow_eDrawTone_DarkOrangeHighSaturation,	"ToneDarkOrangeHighSaturation"},
	{ (int) glow_eDrawTone_DarkOrangeLowSaturation,		"ToneDarkOrangeLowSaturation"},
	{ (int) glow_eDrawTone_LightOrangeHighSaturation,	"ToneLightOrangeHighSaturation"},
	{ (int) glow_eDrawTone_LightOrangeLowSaturation,	"ToneLightOrangeLowSaturation"},
	{ (int) glow_eDrawTone_DarkRed,				"ToneDarkRed"},
	{ (int) glow_eDrawTone_LightRed,			"ToneLightRed"},
	{ (int) glow_eDrawTone_RedHighSaturation,		"ToneRedHighSaturation"},
	{ (int) glow_eDrawTone_RedLowSaturation,		"ToneRedLowSaturation"},
	{ (int) glow_eDrawTone_DarkRedHighSaturation,		"ToneDarkRedHighSaturation"},
	{ (int) glow_eDrawTone_DarkRedLowSaturation,		"ToneDarkRedLowSaturation"},
	{ (int) glow_eDrawTone_LightRedHighSaturation,		"ToneLightRedHighSaturation"},
	{ (int) glow_eDrawTone_LightRedLowSaturation,		"ToneLightRedLowSaturation"},
	{ (int) glow_eDrawTone_DarkMagenta,			"ToneDarkMagenta"},
	{ (int) glow_eDrawTone_LightMagenta,			"ToneLightMagenta"},
	{ (int) glow_eDrawTone_MagentaHighSaturation,		"ToneMagentaHighSaturation"},
	{ (int) glow_eDrawTone_MagentaLowSaturation,		"ToneMagentaLowSaturation"},
	{ (int) glow_eDrawTone_DarkMagentaHighSaturation,	"ToneDarkMagentaHighSaturation"},
	{ (int) glow_eDrawTone_DarkMagentaLowSaturation,	"ToneDarkMagentaLowSaturation"},
	{ (int) glow_eDrawTone_LightMagentaHighSaturation,	"ToneLightMagentaHighSaturation"},
	{ (int) glow_eDrawTone_LightMagentaLowSaturation,	"ToneLightMagentaLowSaturation"},
	{ (int) glow_eDrawTone_DarkBlue,			"ToneDarkBlue"},
	{ (int) glow_eDrawTone_LightBlue,			"ToneLightBlue"},
	{ (int) glow_eDrawTone_BlueHighSaturation,		"ToneBlueHighSaturation"},
	{ (int) glow_eDrawTone_BlueLowSaturation,		"ToneBlueLowSaturation"},
	{ (int) glow_eDrawTone_DarkBlueHighSaturation,		"ToneDarkBlueHighSaturation"},
	{ (int) glow_eDrawTone_DarkBlueLowSaturation,		"ToneDarkBlueLowSaturation"},
	{ (int) glow_eDrawTone_LightBlueHighSaturation,		"ToneLightBlueHighSaturation"},
	{ (int) glow_eDrawTone_LightBlueLowSaturation,		"ToneLightBlueLowSaturation"},
	{ (int) glow_eDrawTone_DarkSeablue,			"ToneDarkSeablue"},
	{ (int) glow_eDrawTone_LightSeablue,			"ToneLightSeablue"},
	{ (int) glow_eDrawTone_SeablueHighSaturation,		"ToneSeablueHighSaturation"},
	{ (int) glow_eDrawTone_SeablueLowSaturation,		"ToneSeablueLowSaturation"},
	{ (int) glow_eDrawTone_DarkSeablueHighSaturation,	"ToneDarkSeablueHighSaturation"},
	{ (int) glow_eDrawTone_DarkSeablueLowSaturation,	 "ToneDarkSeablueLowSaturation"},
	{ (int) glow_eDrawTone_LightSeablueHighSaturation,	"ToneLightSeablueHighSaturation"},
	{ (int) glow_eDrawTone_LightSeablueLowSaturation,	"ToneLightSeablueLowSaturation"},
	{ (int) glow_eDrawTone_DarkGreen,			"ToneDarkGreen"},
	{ (int) glow_eDrawTone_LightGreen,			"ToneLightGreen"},
	{ (int) glow_eDrawTone_GreenHighSaturation,		"ToneGreenHighSaturation"},
	{ (int) glow_eDrawTone_GreenLowSaturation,		"ToneGreenLowSaturation"},
	{ (int) glow_eDrawTone_DarkGreenHighSaturation,		"ToneDarkGreenHighSaturation"},
	{ (int) glow_eDrawTone_DarkGreenLowSaturation,		"ToneDarkGreenLowSaturation"},
	{ (int) glow_eDrawTone_LightGreenHighSaturation,	"ToneLightGreenHighSaturation"},
	{ (int) glow_eDrawTone_LightGreenLowSaturation,		"ToneLightGreenLowSaturation"},
	{ (int) glow_eDrawType_Color82, 	"YellowGreenHigh2"},
	{ (int) glow_eDrawType_Color83, 	"YellowGreenHigh3"},
	{ (int) glow_eDrawType_Color84, 	"YellowGreenHigh4"},
	{ (int) glow_eDrawType_Color85, 	"YellowGreenHigh5"},
	{ (int) glow_eDrawType_Color86, 	"YellowGreenHigh6"},
	{ (int) glow_eDrawType_Color87, 	"YellowGreenHigh7"},
	{ (int) glow_eDrawType_Color88, 	"YellowGreenHigh8"},
	{ (int) glow_eDrawType_Color89, 	"YellowGreenHigh9"},
	{ (int) glow_eDrawType_Color90, 	"YellowGreenHigh10"},
	{ (int) glow_eDrawType_Color91, 	"YellowLow1"},
	{ (int) glow_eDrawType_Color92, 	"YellowLow2"},
	{ (int) glow_eDrawType_Color93, 	"YellowLow3"},
	{ (int) glow_eDrawType_Color94, 	"YellowLow4"},
	{ (int) glow_eDrawType_Color95, 	"YellowLow5"},
	{ (int) glow_eDrawType_Color96, 	"YellowLow6"},
	{ (int) glow_eDrawType_Color97, 	"YellowLow7"},
	{ (int) glow_eDrawType_Color98, 	"YellowLow8"},
	{ (int) glow_eDrawType_Color99, 	"YellowLow9"},
	{ (int) glow_eDrawType_Color100, 	"YellowLow10"},
	{ (int) glow_eDrawType_Color101, 	"YellowMedium1"},
	{ (int) glow_eDrawType_Color102, 	"YellowMedium2"},
	{ (int) glow_eDrawType_Color103, 	"YellowMedium3"},
	{ (int) glow_eDrawType_Color104, 	"YellowMedium4"},
	{ (int) glow_eDrawType_Color105, 	"YellowMedium5"},
	{ (int) glow_eDrawType_Color106, 	"YellowMedium6"},
	{ (int) glow_eDrawType_Color107, 	"YellowMedium7"},
	{ (int) glow_eDrawType_Color108, 	"YellowMedium8"},
	{ (int) glow_eDrawType_Color109, 	"YellowMedium9"},
	{ (int) glow_eDrawType_Color110, 	"YellowMedium10"},
	{ (int) glow_eDrawType_Color111, 	"YellowHigh1"},
	{ (int) glow_eDrawType_Color112, 	"YellowHigh2"},
	{ (int) glow_eDrawType_Color113, 	"YellowHigh3"},
	{ (int) glow_eDrawType_Color114, 	"YellowHigh4"},
	{ (int) glow_eDrawType_Color115, 	"YellowHigh5"},
	{ (int) glow_eDrawType_Color116, 	"YellowHigh6"},
	{ (int) glow_eDrawType_Color117, 	"YellowHigh7"},
	{ (int) glow_eDrawType_Color118, 	"YellowHigh8"},
	{ (int) glow_eDrawType_Color119, 	"YellowHigh9"},
	{ (int) glow_eDrawType_Color120, 	"YellowHigh10"},
	{ (int) glow_eDrawType_Color121, 	"OrangeLow1"},
	{ (int) glow_eDrawType_Color122, 	"OrangeLow2"},
	{ (int) glow_eDrawType_Color123, 	"OrangeLow3"},
	{ (int) glow_eDrawType_Color124, 	"OrangeLow4"},
	{ (int) glow_eDrawType_Color125, 	"OrangeLow5"},
	{ (int) glow_eDrawType_Color126, 	"OrangeLow6"},
	{ (int) glow_eDrawType_Color127, 	"OrangeLow7"},
	{ (int) glow_eDrawType_Color128, 	"OrangeLow8"},
	{ (int) glow_eDrawType_Color129, 	"OrangeLow9"},
	{ (int) glow_eDrawType_Color130, 	"OrangeLow10"},
	{ (int) glow_eDrawType_Color131, 	"OrangeMedium1"},
	{ (int) glow_eDrawType_Color132, 	"OrangeMedium2"},
	{ (int) glow_eDrawType_Color133, 	"OrangeMedium3"},
	{ (int) glow_eDrawType_Color134, 	"OrangeMedium4"},
	{ (int) glow_eDrawType_Color135, 	"OrangeMedium5"},
	{ (int) glow_eDrawType_Color136, 	"OrangeMedium6"},
	{ (int) glow_eDrawType_Color137, 	"OrangeMedium7"},
	{ (int) glow_eDrawType_Color138, 	"OrangeMedium8"},
	{ (int) glow_eDrawType_Color139, 	"OrangeMedium9"},
	{ (int) glow_eDrawType_Color140, 	"OrangeMedium10"},
	{ (int) glow_eDrawType_Color141, 	"OrangeHigh1"},
	{ (int) glow_eDrawType_Color142, 	"OrangeHigh2"},
	{ (int) glow_eDrawType_Color143, 	"OrangeHigh3"},
	{ (int) glow_eDrawType_Color144, 	"OrangeHigh4"},
	{ (int) glow_eDrawType_Color145, 	"OrangeHigh5"},
	{ (int) glow_eDrawType_Color146, 	"OrangeHigh6"},
	{ (int) glow_eDrawType_Color147, 	"OrangeHigh7"},
	{ (int) glow_eDrawType_Color148, 	"OrangeHigh8"},
	{ (int) glow_eDrawType_Color149, 	"OrangeHigh9"},
	{ (int) glow_eDrawType_Color150, 	"OrangeHigh10"},
	{ (int) glow_eDrawType_Color151, 	"RedLow1"},
	{ (int) glow_eDrawType_Color152, 	"RedLow2"},
	{ (int) glow_eDrawType_Color153, 	"RedLow3"},
	{ (int) glow_eDrawType_Color154, 	"RedLow4"},
	{ (int) glow_eDrawType_Color155, 	"RedLow5"},
	{ (int) glow_eDrawType_Color156, 	"RedLow6"},
	{ (int) glow_eDrawType_Color157, 	"RedLow7"},
	{ (int) glow_eDrawType_Color158, 	"RedLow8"},
	{ (int) glow_eDrawType_Color159, 	"RedLow9"},
	{ (int) glow_eDrawType_Color160, 	"RedLow10"},
	{ (int) glow_eDrawType_Color161, 	"RedMedium1"},
	{ (int) glow_eDrawType_Color162, 	"RedMedium2"},
	{ (int) glow_eDrawType_Color163, 	"RedMedium3"},
	{ (int) glow_eDrawType_Color164, 	"RedMedium4"},
	{ (int) glow_eDrawType_Color165, 	"RedMedium5"},
	{ (int) glow_eDrawType_Color166, 	"RedMedium6"},
	{ (int) glow_eDrawType_Color167, 	"RedMedium7"},
	{ (int) glow_eDrawType_Color168, 	"RedMedium8"},
	{ (int) glow_eDrawType_Color169, 	"RedMedium9"},
	{ (int) glow_eDrawType_Color170, 	"RedMedium10"},
	{ (int) glow_eDrawType_Color171, 	"RedHigh1"},
	{ (int) glow_eDrawType_Color172, 	"RedHigh2"},
	{ (int) glow_eDrawType_Color173, 	"RedHigh3"},
	{ (int) glow_eDrawType_Color174, 	"RedHigh4"},
	{ (int) glow_eDrawType_Color175, 	"RedHigh5"},
	{ (int) glow_eDrawType_Color176, 	"RedHigh6"},
	{ (int) glow_eDrawType_Color177, 	"RedHigh7"},
	{ (int) glow_eDrawType_Color178, 	"RedHigh8"},
	{ (int) glow_eDrawType_Color179, 	"RedHigh9"},
	{ (int) glow_eDrawType_Color180, 	"RedHigh10"},
	{ (int) glow_eDrawType_Color181, 	"MagentaLow1"},
	{ (int) glow_eDrawType_Color182, 	"MagentaLow2"},
	{ (int) glow_eDrawType_Color183, 	"MagentaLow3"},
	{ (int) glow_eDrawType_Color184, 	"MagentaLow4"},
	{ (int) glow_eDrawType_Color185, 	"MagentaLow5"},
	{ (int) glow_eDrawType_Color186, 	"MagentaLow6"},
	{ (int) glow_eDrawType_Color187, 	"MagentaLow7"},
	{ (int) glow_eDrawType_Color188, 	"MagentaLow8"},
	{ (int) glow_eDrawType_Color189, 	"MagentaLow9"},
	{ (int) glow_eDrawType_Color190, 	"MagentaLow10"},
	{ (int) glow_eDrawType_Color191, 	"MagentaMedium1"},
	{ (int) glow_eDrawType_Color192, 	"MagentaMedium2"},
	{ (int) glow_eDrawType_Color193, 	"MagentaMedium3"},
	{ (int) glow_eDrawType_Color194, 	"MagentaMedium4"},
	{ (int) glow_eDrawType_Color195, 	"MagentaMedium5"},
	{ (int) glow_eDrawType_Color196, 	"MagentaMedium6"},
	{ (int) glow_eDrawType_Color197, 	"MagentaMedium7"},
	{ (int) glow_eDrawType_Color198, 	"MagentaMedium8"},
	{ (int) glow_eDrawType_Color199, 	"MagentaMedium9"},
	{ (int) glow_eDrawType_Color200, 	"MagentaMedium10"},
	{ (int) glow_eDrawType_Color201, 	"MagentaHigh1"},
	{ (int) glow_eDrawType_Color202, 	"MagentaHigh2"},
	{ (int) glow_eDrawType_Color203, 	"MagentaHigh3"},
	{ (int) glow_eDrawType_Color204, 	"MagentaHigh4"},
	{ (int) glow_eDrawType_Color205, 	"MagentaHigh5"},
	{ (int) glow_eDrawType_Color206, 	"MagentaHigh6"},
	{ (int) glow_eDrawType_Color207, 	"MagentaHigh7"},
	{ (int) glow_eDrawType_Color208, 	"MagentaHigh8"},
	{ (int) glow_eDrawType_Color209, 	"MagentaHigh9"},
	{ (int) glow_eDrawType_Color210, 	"MagentaHigh10"},
	{ (int) glow_eDrawType_Color211, 	"BlueLow1"},
	{ (int) glow_eDrawType_Color212, 	"BlueLow2"},
	{ (int) glow_eDrawType_Color213, 	"BlueLow3"},
	{ (int) glow_eDrawType_Color214, 	"BlueLow4"},
	{ (int) glow_eDrawType_Color215, 	"BlueLow5"},
	{ (int) glow_eDrawType_Color216, 	"BlueLow6"},
	{ (int) glow_eDrawType_Color217, 	"BlueLow7"},
	{ (int) glow_eDrawType_Color218, 	"BlueLow8"},
	{ (int) glow_eDrawType_Color219, 	"BlueLow9"},
	{ (int) glow_eDrawType_Color220, 	"BlueLow10"},
	{ (int) glow_eDrawType_Color221, 	"BlueMedium1"},
	{ (int) glow_eDrawType_Color222, 	"BlueMedium2"},
	{ (int) glow_eDrawType_Color223, 	"BlueMedium3"},
	{ (int) glow_eDrawType_Color224, 	"BlueMedium4"},
	{ (int) glow_eDrawType_Color225, 	"BlueMedium5"},
	{ (int) glow_eDrawType_Color226, 	"BlueMedium6"},
	{ (int) glow_eDrawType_Color227, 	"BlueMedium7"},
	{ (int) glow_eDrawType_Color228, 	"BlueMedium8"},
	{ (int) glow_eDrawType_Color229, 	"BlueMedium9"},
	{ (int) glow_eDrawType_Color230, 	"BlueMedium10"},
	{ (int) glow_eDrawType_Color231, 	"BlueHigh1"},
	{ (int) glow_eDrawType_Color232, 	"BlueHigh2"},
	{ (int) glow_eDrawType_Color233, 	"BlueHigh3"},
	{ (int) glow_eDrawType_Color234, 	"BlueHigh4"},
	{ (int) glow_eDrawType_Color235, 	"BlueHigh5"},
	{ (int) glow_eDrawType_Color236, 	"BlueHigh6"},
	{ (int) glow_eDrawType_Color237, 	"BlueHigh7"},
	{ (int) glow_eDrawType_Color238, 	"BlueHigh8"},
	{ (int) glow_eDrawType_Color239, 	"BlueHigh9"},
	{ (int) glow_eDrawType_Color240, 	"BlueHigh10"},
	{ (int) glow_eDrawType_Color241, 	"SeaBlueLow1"},
	{ (int) glow_eDrawType_Color242, 	"SeaBlueLow2"},
	{ (int) glow_eDrawType_Color243, 	"SeaBlueLow3"},
	{ (int) glow_eDrawType_Color244, 	"SeaBlueLow4"},
	{ (int) glow_eDrawType_Color245, 	"SeaBlueLow5"},
	{ (int) glow_eDrawType_Color246, 	"SeaBlueLow6"},
	{ (int) glow_eDrawType_Color247, 	"SeaBlueLow7"},
	{ (int) glow_eDrawType_Color248, 	"SeaBlueLow8"},
	{ (int) glow_eDrawType_Color249, 	"SeaBlueLow9"},
	{ (int) glow_eDrawType_Color250, 	"SeaBlueLow10"},
	{ (int) glow_eDrawType_Color251, 	"SeaBlueMedium1"},
	{ (int) glow_eDrawType_Color252, 	"SeaBlueMedium2"},
	{ (int) glow_eDrawType_Color253, 	"SeaBlueMedium3"},
	{ (int) glow_eDrawType_Color224, 	"SeaBlueMedium4"},
	{ (int) glow_eDrawType_Color255, 	"SeaBlueMedium5"},
	{ (int) glow_eDrawType_Color256, 	"SeaBlueMedium6"},
	{ (int) glow_eDrawType_Color257, 	"SeaBlueMedium7"},
	{ (int) glow_eDrawType_Color258, 	"SeaBlueMedium8"},
	{ (int) glow_eDrawType_Color259, 	"SeaBlueMedium9"},
	{ (int) glow_eDrawType_Color260, 	"SeaBlueMedium10"},
	{ (int) glow_eDrawType_Color261, 	"SeaBlueHigh1"},
	{ (int) glow_eDrawType_Color262, 	"SeaBlueHigh2"},
	{ (int) glow_eDrawType_Color263, 	"SeaBlueHigh3"},
	{ (int) glow_eDrawType_Color264, 	"SeaBlueHigh4"},
	{ (int) glow_eDrawType_Color265, 	"SeaBlueHigh5"},
	{ (int) glow_eDrawType_Color266, 	"SeaBlueHigh6"},
	{ (int) glow_eDrawType_Color267, 	"SeaBlueHigh7"},
	{ (int) glow_eDrawType_Color268, 	"SeaBlueHigh8"},
	{ (int) glow_eDrawType_Color269, 	"SeaBlueHigh9"},
	{ (int) glow_eDrawType_Color270, 	"SeaBlueHigh10"},
	{ (int) glow_eDrawType_Color271, 	"GreenLow1"},
	{ (int) glow_eDrawType_Color272, 	"GreenLow2"},
	{ (int) glow_eDrawType_Color273, 	"GreenLow3"},
	{ (int) glow_eDrawType_Color274, 	"GreenLow4"},
	{ (int) glow_eDrawType_Color275, 	"GreenLow5"},
	{ (int) glow_eDrawType_Color276, 	"GreenLow6"},
	{ (int) glow_eDrawType_Color277, 	"GreenLow7"},
	{ (int) glow_eDrawType_Color278, 	"GreenLow8"},
	{ (int) glow_eDrawType_Color279, 	"GreenLow9"},
	{ (int) glow_eDrawType_Color280, 	"GreenLow10"},
	{ (int) glow_eDrawType_Color281, 	"GreenMedium1"},
	{ (int) glow_eDrawType_Color282, 	"GreenMedium2"},
	{ (int) glow_eDrawType_Color283, 	"GreenMedium3"},
	{ (int) glow_eDrawType_Color284, 	"GreenMedium4"},
	{ (int) glow_eDrawType_Color285, 	"GreenMedium5"},
	{ (int) glow_eDrawType_Color286, 	"GreenMedium6"},
	{ (int) glow_eDrawType_Color287, 	"GreenMedium7"},
	{ (int) glow_eDrawType_Color288, 	"GreenMedium8"},
	{ (int) glow_eDrawType_Color289, 	"GreenMedium9"},
	{ (int) glow_eDrawType_Color290, 	"GreenMedium10"},
	{ (int) glow_eDrawType_Color291, 	"GreenHigh1"},
	{ (int) glow_eDrawType_Color292, 	"GreenHigh2"},
	{ (int) glow_eDrawType_Color293, 	"GreenHigh3"},
	{ (int) glow_eDrawType_Color294, 	"GreenHigh4"},
	{ (int) glow_eDrawType_Color295, 	"GreenHigh5"},
	{ (int) glow_eDrawType_Color296, 	"GreenHigh6"},
	{ (int) glow_eDrawType_Color297, 	"GreenHigh7"},
	{ (int) glow_eDrawType_Color298, 	"GreenHigh8"},
	{ (int) glow_eDrawType_Color299, 	"GreenHigh9"},
	{ (int) glow_eDrawType_Color300, 	"GreenHigh10"},
	{ (int) glow_eDrawType_Inherit, 	"Inherit"},
	{ 0, ""}};

static attrnav_sEnumElement elem_instance_mask[] = {
     // { (int) ge_mInstance_1, 	"1"},
	{ (int) ge_mInstance_2, 	"2"},
	{ (int) ge_mInstance_3, 	"3"},
	{ (int) ge_mInstance_4, 	"4"},
	{ (int) ge_mInstance_5, 	"5"},
	{ (int) ge_mInstance_6, 	"6"},
	{ (int) ge_mInstance_7, 	"7"},
	{ (int) ge_mInstance_8, 	"8"},
	{ (int) ge_mInstance_9, 	"9"},
	{ (int) ge_mInstance_10, 	"10"},
	{ (int) ge_mInstance_11, 	"11"},
	{ (int) ge_mInstance_12, 	"12"},
	{ (int) ge_mInstance_13, 	"13"},
	{ (int) ge_mInstance_14, 	"14"},
	{ (int) ge_mInstance_15, 	"15"},
	{ (int) ge_mInstance_16, 	"16"},
	{ (int) ge_mInstance_17, 	"17"},
	{ (int) ge_mInstance_18, 	"18"},
	{ (int) ge_mInstance_19, 	"19"},
	{ (int) ge_mInstance_20, 	"20"},
	{ (int) ge_mInstance_21, 	"21"},
	{ (int) ge_mInstance_22, 	"22"},
	{ (int) ge_mInstance_23, 	"23"},
	{ (int) ge_mInstance_24, 	"24"},
	{ (int) ge_mInstance_25, 	"25"},
	{ (int) ge_mInstance_26, 	"26"},
	{ (int) ge_mInstance_27, 	"27"},
	{ (int) ge_mInstance_28, 	"28"},
	{ (int) ge_mInstance_29, 	"29"},
	{ (int) ge_mInstance_30, 	"30"},
	{ (int) ge_mInstance_31, 	"31"},
	{ (int) ge_mInstance_32, 	"32"},
	{ 0, ""}};

static attrnav_sEnumElement elem_inputfocus_mask[] = {
        { (int) ge_mInputFocus_InitialFocus, 	"InitialFocus"},
        { (int) ge_mInputFocus_FirstHorizontal,	"FirstHorizontal"},
        { (int) ge_mInputFocus_FirstVertical, 	"FirstVertical"},
        { (int) ge_mInputFocus_FirstTab, 	"FirstTab"},
        { (int) ge_mInputFocus_LastHorizontal,	"LastHorizontal"},
        { (int) ge_mInputFocus_LastVertical, 	"LastVertical"},
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

static attrnav_sEnumElement elem_input_focus_mark[] = {
	{ (int) glow_eInputFocusMark_Relief, 	"Relief"},
	{ (int) glow_eInputFocusMark_No,       	"No"},
	{ 0, ""}};

static attrnav_sEnumElement elem_anim_sequence[] = {
	{ (int) ge_eAnimSequence_Inherit, 	"Inherit"},
	{ (int) ge_eAnimSequence_Cycle, 	"Cyclic"},
	{ (int) ge_eAnimSequence_Dig, 		"Dig"},
	{ (int) ge_eAnimSequence_ForwBack, 	"ForwBack"},
	{ 0, ""}};

static attrnav_sEnumElement elem_limit_type[] = {
	{ (int) ge_eLimitType_Gt, 	"GreaterThan"},
	{ (int) ge_eLimitType_Lt, 	"LessThan"},
	{ 0, ""}};

static attrnav_sEnumElement elem_relief[] = {
	{ (int) glow_eRelief_Up, 	"Up"},
	{ (int) glow_eRelief_Down, 	"Down"},
	{ 0, ""}};

static attrnav_sEnumElement elem_text_size[] = {
	{ (int) glow_eTextSize_8, 	"8"},
	{ (int) glow_eTextSize_10, 	"10"},
	{ (int) glow_eTextSize_12, 	"12"},
	{ (int) glow_eTextSize_14, 	"14"},
	{ (int) glow_eTextSize_18, 	"18"},
	{ (int) glow_eTextSize_24, 	"24"},
	{ 0, ""}};

static attrnav_sEnum enum_types[] = {
	{ (int) glow_eType_Direction, 	(attrnav_sEnumElement *) &elem_direction},
	{ (int) glow_eType_Color, 	(attrnav_sEnumElement *) &elem_color},
	{ (int) glow_eType_Tone,	(attrnav_sEnumElement *) &elem_tone},
	{ (int) glow_eType_ToneOrColor, (attrnav_sEnumElement *) &elem_tone_or_color},
	{ (int) glow_eType_Cycle, 	(attrnav_sEnumElement *) &elem_cycle},
	{ (int) glow_eType_MB3Action, 	(attrnav_sEnumElement *) &elem_mb3_action},
	{ (int) ge_eAttrType_AnimSequence, (attrnav_sEnumElement *) &elem_anim_sequence},
	{ (int) ge_eAttrType_LimitType, (attrnav_sEnumElement *) &elem_limit_type},
	{ (int) glow_eType_Relief, 	(attrnav_sEnumElement *) &elem_relief},
	{ (int) glow_eType_TextSize, 	(attrnav_sEnumElement *) &elem_text_size},
	{ (int) glow_eType_InputFocusMark, (attrnav_sEnumElement *) &elem_input_focus_mark},
	{ 0, NULL}};

static attrnav_sEnum mask_types[] = {
	{ (int) glow_eType_Access, 	(attrnav_sEnumElement *) &elem_access},
	{ (int) ge_eAttrType_DynType, 	(attrnav_sEnumElement *) &elem_dyn_type},
	{ (int) ge_eAttrType_DynTypeTone, (attrnav_sEnumElement *) &elem_dyn_type_tone},
	{ (int) ge_eAttrType_ActionType, (attrnav_sEnumElement *) &elem_action_type},
	{ (int) ge_eAttrType_InstanceMask, (attrnav_sEnumElement *) &elem_instance_mask},
	{ (int) ge_eAttrType_InputFocus, (attrnav_sEnumElement *) &elem_inputfocus_mask},
	{ 0, NULL}};



static char null_str[] = "";

static void attrnav_trace_scan( AttrNav *attrnav);
static int attrnav_trace_scan_bc( brow_tObject object, void *p);
static int attrnav_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, /* flow_eDrawType color, */ void **p);
static int attrnav_trace_disconnect_bc( brow_tObject object);
static int attrnav_init_brow_cb( FlowCtx *fctx, void *client_data);
static char *attrnav_mask_to_string( int type_id, int value);

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
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Access:
    case glow_eType_Cycle:
    case glow_eType_MB3Action: 
    case glow_eType_InputFocusMark: 
    case glow_eType_Relief: 
    case glow_eType_TextSize: 
    case ge_eAttrType_DynType:
    case ge_eAttrType_DynTypeTone:
    case ge_eAttrType_ActionType:
    case ge_eAttrType_AnimSequence:
    case ge_eAttrType_LimitType:
    case ge_eAttrType_InstanceMask:
    case ge_eAttrType_InputFocus:
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
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Cycle:
    case glow_eType_MB3Action:
    case glow_eType_InputFocusMark:
    case glow_eType_Relief:
    case glow_eType_TextSize:
    case ge_eAttrType_AnimSequence:
    case ge_eAttrType_LimitType:
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
    case ge_eAttrType_DynType:
    case ge_eAttrType_DynTypeTone:
    case ge_eAttrType_ActionType:
    case ge_eAttrType_InstanceMask:
    case ge_eAttrType_InputFocus:
    {
      strncpy( str, attrnav_mask_to_string( type_id, *(int *)value_ptr), size);
      str[size-1] = 0;
      *len = strlen( str);
#if 0
      *len = sprintf( str, "%u", *(unsigned int *)value_ptr);
#endif
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

      if ( item->max_limit != 0 || item->min_limit != 0) {
	if ( item->type_id == glow_eType_Double ) {
	  if ( *(double *)&buffer < item->min_limit ||
	       *(double *)&buffer > item->max_limit) {
	    message( 'E', "Min or maxvalue exceeded");
	    return 0;
	  }
	}
	else if ( item->type_id == glow_eType_Int ) {
	  if ( *(int *)&buffer < item->min_limit ||
	       *(int *)&buffer > item->max_limit) {
	    message( 'E', "Min or maxvalue exceeded");
	    return 0;
	  }
	}
      }
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
  static char   buf[200];
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
          if ( ((ItemLocal *)item)->parent || ((ItemLocal *)item)->subgraph ||
	       item->type_id == ge_eAttrType_Dyn)
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
	  if ( (((ItemEnum *)item)->type_id == ge_eAttrType_DynType ||
	        ((ItemEnum *)item)->type_id == ge_eAttrType_DynTypeTone ||
		((ItemEnum *)item)->type_id == ge_eAttrType_ActionType ||
		((ItemEnum *)item)->type_id == ge_eAttrType_InstanceMask) &&
	       attrnav->reconfigure_attr_cb) {
	    (attrnav->reconfigure_attr_cb)(attrnav->parent_ctx);
	    return FLOW__DESTROYED;
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
    case flow_eEvent_MB1DoubleClickCtrl:
      switch ( event->object.object_type) {
      case flow_eObjectType_Node:
	brow_GetUserData( event->object.object, (void **)&item);
	switch( item->type) {
	case attrnav_eItemType_Local: 
	  if ( item->type_id == glow_eType_String) {
	    char attr_name[120];
	    int sts;

	    if ( ! attrnav->get_plant_select_cb)
	      break;
	    sts = (attrnav->get_plant_select_cb) (attrnav->parent_ctx, attr_name);
	    if ( EVEN(sts)) break;

	    strncpy( (char *)item->value_p, attr_name, item->size);
	  }
	  else if ( item->type_id == glow_eType_Color) {
	    int sts;
	    glow_eDrawType fill_color, border_color, text_color;

	    if ( ! attrnav->get_current_colors_cb)
	      break;
	    sts = (attrnav->get_current_colors_cb) (attrnav->parent_ctx, &fill_color,
							&border_color, &text_color);
	    if ( EVEN(sts)) break;

	    *(glow_eDrawType *)item->value_p = fill_color;
	  }
	  break;
	default: ;
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

	       if ( (((ItemMask *)item)->type_id == ge_eAttrType_DynType ||
	             ((ItemMask *)item)->type_id == ge_eAttrType_DynTypeTone ||
		     ((ItemMask *)item)->type_id == ge_eAttrType_ActionType ||
		     ((ItemMask *)item)->type_id == ge_eAttrType_InstanceMask) &&
		    attrnav->reconfigure_attr_cb) {
		 (attrnav->reconfigure_attr_cb)(attrnav->parent_ctx);
		 return FLOW__DESTROYED;
	       }
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
  char		buf[200];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case attrnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *)base_item;
      if ( item->size == 0)
        break;

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
      if (item->size == 0)
	break;

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
        item_p->value, item_p->multiline, item_p->noedit, item_p->mask,
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
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
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
	int attr_noedit, int attr_mask,
	brow_tNode dest, flow_eDest dest_code) :
	value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
	min_limit(attr_min_limit), max_limit(attr_max_limit),
	multiline(attr_multiline), noedit(attr_noedit), mask(attr_mask), parent(0),
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
    case glow_eType_Direction:
    case glow_eType_Color:
    case glow_eType_Tone:
    case glow_eType_ToneOrColor:
    case glow_eType_Access:
    case glow_eType_Cycle:
    case glow_eType_MB3Action:
    case glow_eType_InputFocusMark:
    case glow_eType_Relief:
    case glow_eType_TextSize:
    case ge_eAttrType_DynType:
    case ge_eAttrType_DynTypeTone:
    case ge_eAttrType_ActionType:
    case ge_eAttrType_AnimSequence:
    case ge_eAttrType_LimitType:
    case ge_eAttrType_InstanceMask:
    case ge_eAttrType_InputFocus:
      if ( !noedit)
      {
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
        parent = 1;
      }
      else
        brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attr);
      break;
    case ge_eAttrType_Dyn:
      brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_attrarray);
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
  else if ( type_id == ge_eAttrType_Dyn)
  {
    attr_sItem  *itemlist, *item_p;
    int 	item_cnt;
    int		sts;
    int		i;

    sts = (attrnav->get_dyn_info_cb)( attrnav->parent_ctx, 
	(GeDyn *)value_p, &itemlist, &item_cnt);
    if ( EVEN(sts)) return 0;

    brow_SetNodraw( attrnav->brow->ctx);

    item_p = itemlist;
    for ( i = 0; i < item_cnt; i++)
    {
      new ItemLocal( attrnav, item_p->name, "LocalAttr", 
	item_p->type, item_p->size, item_p->minlimit, item_p->maxlimit,
	item_p->value, item_p->multiline, item_p->noedit, item_p->mask,
	node, flow_eDest_IntoLast);
      item_p++;
    }
    brow_SetOpen( node, attrnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 0, attrnav->brow->pixmap_openmap);
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
	if ( mask && !(mask & elem_p->num))
	  continue;
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
	item_p->value, item_p->multiline, item_p->noedit, item_p->mask,
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

static char *attrnav_mask_to_string( int type_id, int value)
{
  attrnav_sEnumElement	*elem_p;
  attrnav_sEnum		*enum_p;
  int			found;
  bool			first = true;
  static char 		str[2000];

  found = 0;
  for ( enum_p = mask_types; enum_p->elements; enum_p++) {
    if ( enum_p->num == type_id) {
      elem_p = enum_p->elements;
      found = 1;
      break;
    }
  }
  if ( !found) {
    strcpy( str, "");
    return str;
  }

  strcpy( str, "");
  for ( ; elem_p->name[0] != 0; elem_p++) {
    if ( elem_p->num & value) {
      if ( !first)
	strcat( str, " | ");
      else
	first = false;
      strcat( str, elem_p->name);
    }
  }
  return str;
}
