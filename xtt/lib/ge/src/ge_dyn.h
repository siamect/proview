#ifndef ge_dyn_h
#define ge_dyn_h

/* ge_dyn.h -- Ge predefined dynamics and actions

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef ge_graph_h
#include "ge_graph.h"
#endif

#ifndef glow_h
#include "glow.h"
#endif

/*! \file ge_dyn.h
    \brief Contains the Graph class and some related classes GraphApplList, GraphRecallBuff, GraphGbl and GraphGrow. */
/*! \addtogroup GeDyn */
/*@{*/

#define TABLE_MAX_COL 12
#define DYN_FAST_MAX 10
#define FAST_CURVES 10
#define fast_mFunction_BeforeTrigg 4

  //! Types of animation sequences.
  typedef enum {
    ge_eAnimSequence_Inherit,	//!< Inherit from nodeclass.
    ge_eAnimSequence_Cycle,	//!< Cyclic animation.
    ge_eAnimSequence_Dig,	//!< Animation between two resct positions.
    ge_eAnimSequence_ForwBack	//!< Animation first in forward directon and then in backward, and so on.
  } ge_eAnimSequence;

  //! Values for a limit type.
  typedef enum {
    ge_eLimitType_Gt, 	//!< Greater then.
    ge_eLimitType_Lt,	//!< Less then.
    ge_eLimitType_Ge, 	//!< Greater then or equal.
    ge_eLimitType_Le,	//!< Less then or equal.
    ge_eLimitType_Eq	//!< Equal.
  } ge_eLimitType;

  //! Mask for initial and first input focus.
  typedef enum {
    ge_mInputFocus_InitialFocus		= 1 << 0, 	//!< Initial input focus.
    ge_mInputFocus_FirstHorizontal	= 1 << 1, 	//!< First horizontal input focus.
    ge_mInputFocus_FirstVertical 	= 1 << 2, 	//!< First vertical input focus.
    ge_mInputFocus_FirstTab		= 1 << 3,      	//!< First tab input focus.
    ge_mInputFocus_LastHorizontal	= 1 << 4, 	//!< Last horizontal input focus.
    ge_mInputFocus_LastVertical 	= 1 << 5 	//!< Last vertical input focus.
  } ge_mInputFocus;

  //! Dyn attributes displayed in attribute editor
  typedef enum {
    ge_eDynAttr_All,
    ge_eDynAttr_Menu
  } ge_eDynAttr;

  //! Status value handled by GeStatus
  typedef enum {
    ge_ePwrStatus_No,
    ge_ePwrStatus_Success,
    ge_ePwrStatus_Warning,
    ge_ePwrStatus_Error,
    ge_ePwrStatus_Fatal
  } ge_ePwrStatus;
  

  //! Priority order for dyntypes and actiontypes. Lower value gives higher priority.
  typedef enum {
    ge_eDynPrio_HostObject,
    ge_eDynPrio_Invisible,
    ge_eDynPrio_DigFlash,
    ge_eDynPrio_DigError,
    ge_eDynPrio_DigWarning,
    ge_eDynPrio_AnalogColor,
    ge_eDynPrio_DigColor,
    ge_eDynPrio_DigLowColor,
    ge_eDynPrio_DigBorder,
    ge_eDynPrio_DigText,
    ge_eDynPrio_FillLevel,
    ge_eDynPrio_Value,
    ge_eDynPrio_ValueInput,
    ge_eDynPrio_Rotate,
    ge_eDynPrio_Move,
    ge_eDynPrio_AnalogShift,
    ge_eDynPrio_DigShift,
    ge_eDynPrio_Animation,
    ge_eDynPrio_Bar,
    ge_eDynPrio_Trend,
    ge_eDynPrio_FastCurve,
    ge_eDynPrio_AnalogText,
    ge_eDynPrio_Table,
    ge_eDynPrio_SliderBackground,
    ge_eDynPrio_Video,
    ge_eDynPrio_StatusColor,
    ge_eDynPrio_PopupMenu,
    ge_eDynPrio_Confirm,
    ge_eDynPrio_SetDig,
    ge_eDynPrio_ResetDig,
    ge_eDynPrio_ToggleDig,
    ge_eDynPrio_StoDig,
    ge_eDynPrio_Command,
    ge_eDynPrio_Help,
    ge_eDynPrio_OpenGraph,
    ge_eDynPrio_OpenURL,
    ge_eDynPrio_CommandDoubleClick,
    ge_eDynPrio_IncrAnalog,
    ge_eDynPrio_RadioButton,
    ge_eDynPrio_Slider,
    ge_eDynPrio_TipText,
    ge_eDynPrio_PulldownMenu,
    ge_eDynPrio_OptionMenu,
    ge_eDynPrio_InputFocus,

    // This should always be last
    ge_eDynPrio_CloseGraph = 10000
  } ge_eDynPrio;

  //! Dynamic types
  typedef enum {
    ge_mDynType_No		= 0,
    ge_mDynType_Inherit        	= 1 << 0,
    ge_mDynType_Tone		= 1 << 1,
    ge_mDynType_DigLowColor	= 1 << 2,
    ge_mDynType_DigColor       	= 1 << 3,
    ge_mDynType_DigError       	= 1 << 4,
    ge_mDynType_DigWarning	= 1 << 5,
    ge_mDynType_DigFlash       	= 1 << 6,
    ge_mDynType_Invisible 	= 1 << 7,
    ge_mDynType_DigBorder      	= 1 << 8,
    ge_mDynType_DigText		= 1 << 9,
    ge_mDynType_Value		= 1 << 10,
    ge_mDynType_AnalogColor	= 1 << 11,
    ge_mDynType_Rotate		= 1 << 12,
    ge_mDynType_Move		= 1 << 13,
    ge_mDynType_AnalogShift	= 1 << 14,
    ge_mDynType_DigShift       	= 1 << 15,
    ge_mDynType_Animation      	= 1 << 16,
    ge_mDynType_Bar		= 1 << 17,
    ge_mDynType_Trend		= 1 << 18,
    ge_mDynType_SliderBackground = 1 << 19,
    ge_mDynType_Video		= 1 << 20,
    ge_mDynType_FillLevel	= 1 << 21,
    ge_mDynType_FastCurve	= 1 << 22,
    ge_mDynType_AnalogText	= 1 << 23,
    ge_mDynType_Table		= 1 << 24,
    ge_mDynType_StatusColor    	= 1 << 25,
    ge_mDynType_HostObject    	= 1 << 26
  } ge_mDynType;

  //! Action types.
  typedef enum {
    ge_mActionType_No		= 0,
    ge_mActionType_Inherit	= 1 << 0,
    ge_mActionType_PopupMenu    = 1 << 1,
    ge_mActionType_SetDig      	= 1 << 2,
    ge_mActionType_ResetDig	= 1 << 3,
    ge_mActionType_ToggleDig    = 1 << 4,
    ge_mActionType_StoDig      	= 1 << 5,
    ge_mActionType_Command	= 1 << 6,
    ge_mActionType_CommandDoubleClick = 1 << 7,
    ge_mActionType_Confirm	= 1 << 8,
    ge_mActionType_IncrAnalog	= 1 << 9,
    ge_mActionType_RadioButton	= 1 << 10,
    ge_mActionType_Slider      	= 1 << 11,
    ge_mActionType_ValueInput   = 1 << 12,
    ge_mActionType_TipText    	= 1 << 13,
    ge_mActionType_Help		= 1 << 14,
    ge_mActionType_OpenGraph	= 1 << 15,
    ge_mActionType_OpenURL	= 1 << 16,
    ge_mActionType_InputFocus	= 1 << 17,
    ge_mActionType_CloseGraph	= 1 << 18,
    ge_mActionType_PulldownMenu	= 1 << 19,
    ge_mActionType_OptionMenu = 1 << 20
  } ge_mActionType;

  //! Instances. A bitmask where each bit represents an instance.
  typedef enum {
    ge_mInstance_1		= 1 << 0,
    ge_mInstance_2		= 1 << 1,
    ge_mInstance_3		= 1 << 2,
    ge_mInstance_4		= 1 << 3,
    ge_mInstance_5		= 1 << 4,
    ge_mInstance_6		= 1 << 5,
    ge_mInstance_7		= 1 << 6,
    ge_mInstance_8		= 1 << 7,
    ge_mInstance_9		= 1 << 8,
    ge_mInstance_10		= 1 << 9,
    ge_mInstance_11		= 1 << 10,
    ge_mInstance_12		= 1 << 11,
    ge_mInstance_13		= 1 << 12,
    ge_mInstance_14		= 1 << 13,
    ge_mInstance_15		= 1 << 14,
    ge_mInstance_16		= 1 << 15,
    ge_mInstance_17		= 1 << 16,
    ge_mInstance_18		= 1 << 17,
    ge_mInstance_19		= 1 << 18,
    ge_mInstance_20		= 1 << 19,
    ge_mInstance_21		= 1 << 20,
    ge_mInstance_22		= 1 << 21,
    ge_mInstance_23		= 1 << 22,
    ge_mInstance_24		= 1 << 23,
    ge_mInstance_25		= 1 << 24,
    ge_mInstance_26		= 1 << 25,
    ge_mInstance_27		= 1 << 26,
    ge_mInstance_28		= 1 << 27,
    ge_mInstance_29		= 1 << 28,
    ge_mInstance_30		= 1 << 29,
    ge_mInstance_31		= 1 << 30,
    ge_mInstance_32		= 1 << 31
  } ge_mInstance;

//! Tags used when writing objects to a .pwg fil.
/*! Every data written is preceeded by a tag to identify the data. 
  All tags for all dynamic classes are defined here.
*/
  typedef enum {
    ge_eSave_Dyn		       	= 1,
    ge_eSave_DigLowColor	       	= 2,
    ge_eSave_DigColor			= 3,
    ge_eSave_DigError			= 7,
    ge_eSave_DigWarning			= 8,
    ge_eSave_Invisible			= 9,
    ge_eSave_DigBorder			= 10,
    ge_eSave_DigText			= 11,
    ge_eSave_Value			= 12,
    ge_eSave_ValueInput			= 13,
    ge_eSave_Rotate			= 14,
    ge_eSave_Move		       	= 15,
    ge_eSave_AnalogShift	       	= 17,
    ge_eSave_DigShift			= 18,
    ge_eSave_Animation			= 19,
    ge_eSave_Bar		       	= 22,
    ge_eSave_Trend			= 23,
    ge_eSave_SliderBackground	       	= 24,
    ge_eSave_Video			= 25,
    ge_eSave_DigFlash			= 26,
    ge_eSave_FillLevel			= 27,
    ge_eSave_FastCurve	       		= 28,
    ge_eSave_AnalogText	       		= 29,
    ge_eSave_Table	       		= 30,
    ge_eSave_StatusColor             	= 31,
    ge_eSave_HostObject             	= 32,
    ge_eSave_PopupMenu			= 50,
    ge_eSave_SetDig			= 51,
    ge_eSave_ResetDig			= 52,
    ge_eSave_ToggleDig			= 53,
    ge_eSave_StoDig			= 54,
    ge_eSave_Command			= 55,
    ge_eSave_CommandDC			= 56,
    ge_eSave_Confirm			= 57,
    ge_eSave_IncrAnalog			= 58,
    ge_eSave_RadioButton	       	= 59,
    ge_eSave_Slider			= 60,
    ge_eSave_AnalogColor	       	= 61,
    ge_eSave_TipText			= 62,
    ge_eSave_Help		       	= 63,
    ge_eSave_OpenGraph			= 64,
    ge_eSave_OpenURL			= 65,
    ge_eSave_InputFocus			= 66,
    ge_eSave_CloseGraph			= 67,
    ge_eSave_PulldownMenu      		= 68,
    ge_eSave_OptionMenu      		= 69,
    ge_eSave_End		       	= 99,
    ge_eSave_Dyn_dyn_type	       	= 100,
    ge_eSave_Dyn_action_type	       	= 101,
    ge_eSave_Dyn_access			= 102,
    ge_eSave_Dyn_cycle			= 103,
    ge_eSave_Dyn_ref_object	       	= 104,
    ge_eSave_DigLowColor_attribute	= 200,
    ge_eSave_DigLowColor_color		= 201,
    ge_eSave_DigColor_attribute		= 300,
    ge_eSave_DigColor_color		= 301,
    ge_eSave_DigColor_instance		= 302,
    ge_eSave_DigColor_instance_mask    	= 303,
    ge_eSave_DigError_attribute		= 700,
    ge_eSave_DigWarning_attribute      	= 800,
    ge_eSave_Invisible_attribute       	= 900,
    ge_eSave_Invisible_dimmed       	= 901,
    ge_eSave_Invisible_instance		= 902,
    ge_eSave_Invisible_instance_mask   	= 903,
    ge_eSave_DigBorder_attribute       	= 1000,
    ge_eSave_DigBorder_color		= 1001,
    ge_eSave_DigText_attribute		= 1100,
    ge_eSave_DigText_low_text		= 1101,
    ge_eSave_Value_attribute		= 1200,
    ge_eSave_Value_format	       	= 1201,
    ge_eSave_Value_instance      	= 1202,
    ge_eSave_Value_instance_mask 	= 1203,
    ge_eSave_Value_zero_blank 		= 1204,
    ge_eSave_ValueInput_attribute      	= 1300,
    ge_eSave_ValueInput_format		= 1301,
    ge_eSave_ValueInput_min_value      	= 1302,
    ge_eSave_ValueInput_max_value      	= 1303,
    ge_eSave_ValueInput_clear      	= 1304,
    ge_eSave_ValueInput_popup      	= 1305,
    ge_eSave_ValueInput_unselect      	= 1306,
    ge_eSave_Rotate_attribute		= 1400,
    ge_eSave_Rotate_x0			= 1401,
    ge_eSave_Rotate_y0			= 1402,
    ge_eSave_Rotate_factor             	= 1403,
    ge_eSave_Move_move_x_attribute     	= 1500,
    ge_eSave_Move_move_y_attribute    	= 1501,
    ge_eSave_Move_x_offset	       	= 1502,
    ge_eSave_Move_y_offset	        = 1503,
    ge_eSave_Move_factor	       	= 1504,
    ge_eSave_Move_scale_x_attribute     = 1505,
    ge_eSave_Move_scale_y_attribute    	= 1506,
    ge_eSave_Move_scale_factor      	= 1507,
    ge_eSave_Move_scale_type      	= 1508,
    ge_eSave_AnalogShift_attribute     	= 1700,
    ge_eSave_DigShift_attribute		= 1800,
    ge_eSave_Animation_attribute       	= 1900,
    ge_eSave_Animation_sequence		= 1901,
    ge_eSave_Bar_attribute	   	= 2200,
    ge_eSave_Bar_minvalue_attr	   	= 2201,
    ge_eSave_Bar_maxvalue_attr	   	= 2202,
    ge_eSave_Trend_attribute1	   	= 2300,
    ge_eSave_Trend_attribute2	   	= 2301,
    ge_eSave_Trend_minvalue_attr1     	= 2302,
    ge_eSave_Trend_maxvalue_attr1     	= 2303,
    ge_eSave_Trend_minvalue_attr2      	= 2304,
    ge_eSave_Trend_maxvalue_attr2      	= 2305,
    ge_eSave_DigFlash_attribute		= 2600,
    ge_eSave_DigFlash_color		= 2601,
    ge_eSave_FillLevel_attribute       	= 2700,
    ge_eSave_FillLevel_color		= 2701,
    ge_eSave_FillLevel_direction       	= 2702,
    ge_eSave_FillLevel_min_value       	= 2703,
    ge_eSave_FillLevel_max_value       	= 2704,
    ge_eSave_FillLevel_minvalue_attr    = 2705,
    ge_eSave_FillLevel_maxvalue_attr    = 2706,
    ge_eSave_FastCurve_fast_object   	= 2800,
    ge_eSave_FastCurve_curve_index1   	= 2801,
    ge_eSave_FastCurve_curve_index2   	= 2802,
    ge_eSave_AnalogText_super   	= 2900,
    ge_eSave_Table_attribute1	   	= 3000,
    ge_eSave_Table_format1	   	= 3001,
    ge_eSave_Table_sel_attribute1      	= 3002,
    ge_eSave_Table_attribute2	   	= 3003,
    ge_eSave_Table_format2	   	= 3004,
    ge_eSave_Table_sel_attribute2      	= 3005,
    ge_eSave_Table_attribute3	   	= 3006,
    ge_eSave_Table_format3	   	= 3007,
    ge_eSave_Table_sel_attribute3      	= 3008,
    ge_eSave_Table_attribute4	   	= 3009,
    ge_eSave_Table_format4	   	= 3010,
    ge_eSave_Table_sel_attribute4      	= 3011,
    ge_eSave_Table_attribute5	   	= 3012,
    ge_eSave_Table_format5	   	= 3013,
    ge_eSave_Table_sel_attribute5      	= 3014,
    ge_eSave_Table_attribute6	   	= 3015,
    ge_eSave_Table_format6	   	= 3016,
    ge_eSave_Table_sel_attribute6      	= 3017,
    ge_eSave_Table_attribute7	   	= 3018,
    ge_eSave_Table_format7	   	= 3019,
    ge_eSave_Table_sel_attribute7      	= 3020,
    ge_eSave_Table_attribute8	   	= 3021,
    ge_eSave_Table_format8	   	= 3022,
    ge_eSave_Table_sel_attribute8      	= 3023,
    ge_eSave_Table_attribute9	   	= 3024,
    ge_eSave_Table_format9	   	= 3025,
    ge_eSave_Table_sel_attribute9      	= 3026,
    ge_eSave_Table_attribute10	   	= 3027,
    ge_eSave_Table_format10	   	= 3028,
    ge_eSave_Table_sel_attribute10     	= 3029,
    ge_eSave_Table_attribute11	   	= 3030,
    ge_eSave_Table_format11	   	= 3031,
    ge_eSave_Table_sel_attribute11     	= 3032,
    ge_eSave_Table_attribute12	   	= 3033,
    ge_eSave_Table_format12	   	= 3034,
    ge_eSave_Table_sel_attribute12     	= 3035,
    ge_eSave_StatusColor_attribute     	= 3100,
    ge_eSave_StatusColor_nostatus_color = 3101,
    ge_eSave_HostObject_object     	= 3200,
    ge_eSave_PopupMenu_ref_object      	= 5000,
    ge_eSave_SetDig_attribute		= 5100,
    ge_eSave_SetDig_instance		= 5101,
    ge_eSave_SetDig_instance_mask      	= 5102,
    ge_eSave_ResetDig_attribute		= 5200,
    ge_eSave_ResetDig_instance		= 5201,
    ge_eSave_ResetDig_instance_mask    	= 5202,
    ge_eSave_ToggleDig_attribute       	= 5300,
    ge_eSave_StoDig_attribute		= 5400,
    ge_eSave_Command_command		= 5500,
    ge_eSave_CommandDC_command		= 5600,
    ge_eSave_Confirm_text	       	= 5700,
    ge_eSave_IncrAnalog_attribute      	= 5800,
    ge_eSave_IncrAnalog_increment      	= 5801,
    ge_eSave_IncrAnalog_min_value      	= 5802,
    ge_eSave_IncrAnalog_max_value      	= 5803,
    ge_eSave_RadioButton_attribute     	= 5900,
    ge_eSave_Slider_attribute	       	= 6000,
    ge_eSave_Slider_minvalue_attr     	= 6001,
    ge_eSave_Slider_maxvalue_attr     	= 6002,
    ge_eSave_AnalogColor_attribute      = 6100,
    ge_eSave_AnalogColor_limit        	= 6101,
    ge_eSave_AnalogColor_limit_type     = 6102,
    ge_eSave_AnalogColor_color       	= 6103,
    ge_eSave_AnalogColor_instance      	= 6104,
    ge_eSave_AnalogColor_instance_mask 	= 6105,
    ge_eSave_TipText_text              	= 6200,
    ge_eSave_Help_topic        		= 6300,
    ge_eSave_Help_bookmark             	= 6400,
    ge_eSave_OpenGraph_graph_object    	= 6401,
    ge_eSave_OpenURL_url               	= 6500,
    ge_eSave_InputFocus_initial_focus	= 6600,
    ge_eSave_InputFocus_next_horizontal	= 6601,
    ge_eSave_InputFocus_next_vertical	= 6602,
    ge_eSave_InputFocus_next_tab	= 6603,
    ge_eSave_PulldownMenu_button_mask	= 6800,
    ge_eSave_PulldownMenu_items_text0	= 6801,
    ge_eSave_PulldownMenu_items_text1	= 6802,
    ge_eSave_PulldownMenu_items_text2	= 6803,
    ge_eSave_PulldownMenu_items_text3	= 6804,
    ge_eSave_PulldownMenu_items_text4	= 6805,
    ge_eSave_PulldownMenu_items_text5	= 6806,
    ge_eSave_PulldownMenu_items_text6	= 6807,
    ge_eSave_PulldownMenu_items_text7	= 6808,
    ge_eSave_PulldownMenu_items_text8	= 6809,
    ge_eSave_PulldownMenu_items_text9	= 6810,
    ge_eSave_PulldownMenu_items_text10	= 6811,
    ge_eSave_PulldownMenu_items_text11	= 6812,
    ge_eSave_PulldownMenu_items_text12	= 6813,
    ge_eSave_PulldownMenu_items_text13	= 6814,
    ge_eSave_PulldownMenu_items_text14	= 6815,
    ge_eSave_PulldownMenu_items_text15	= 6816,
    ge_eSave_PulldownMenu_items_text16	= 6817,
    ge_eSave_PulldownMenu_items_text17	= 6818,
    ge_eSave_PulldownMenu_items_text18	= 6819,
    ge_eSave_PulldownMenu_items_text19	= 6820,
    ge_eSave_PulldownMenu_items_text20	= 6821,
    ge_eSave_PulldownMenu_items_text21	= 6822,
    ge_eSave_PulldownMenu_items_text22	= 6823,
    ge_eSave_PulldownMenu_items_text23	= 6824,
    ge_eSave_PulldownMenu_items_text24	= 6825,
    ge_eSave_PulldownMenu_items_text25	= 6826,
    ge_eSave_PulldownMenu_items_text26	= 6827,
    ge_eSave_PulldownMenu_items_text27	= 6828,
    ge_eSave_PulldownMenu_items_text28	= 6829,
    ge_eSave_PulldownMenu_items_text29	= 6830,
    ge_eSave_PulldownMenu_items_text30	= 6831,
    ge_eSave_PulldownMenu_items_text31	= 6832,
    ge_eSave_PulldownMenu_items_dyn0	= 6833,
    ge_eSave_PulldownMenu_items_dyn1	= 6834,
    ge_eSave_PulldownMenu_items_dyn2	= 6835,
    ge_eSave_PulldownMenu_items_dyn3	= 6836,
    ge_eSave_PulldownMenu_items_dyn4	= 6837,
    ge_eSave_PulldownMenu_items_dyn5	= 6838,
    ge_eSave_PulldownMenu_items_dyn6	= 6839,
    ge_eSave_PulldownMenu_items_dyn7	= 6840,
    ge_eSave_PulldownMenu_items_dyn8	= 6841,
    ge_eSave_PulldownMenu_items_dyn9	= 6842,
    ge_eSave_PulldownMenu_items_dyn10	= 6843,
    ge_eSave_PulldownMenu_items_dyn11	= 6844,
    ge_eSave_PulldownMenu_items_dyn12	= 6845,
    ge_eSave_PulldownMenu_items_dyn13	= 6846,
    ge_eSave_PulldownMenu_items_dyn14	= 6847,
    ge_eSave_PulldownMenu_items_dyn15	= 6848,
    ge_eSave_PulldownMenu_items_dyn16	= 6849,
    ge_eSave_PulldownMenu_items_dyn17	= 6850,
    ge_eSave_PulldownMenu_items_dyn18	= 6851,
    ge_eSave_PulldownMenu_items_dyn19	= 6852,
    ge_eSave_PulldownMenu_items_dyn20	= 6853,
    ge_eSave_PulldownMenu_items_dyn21	= 6854,
    ge_eSave_PulldownMenu_items_dyn22	= 6855,
    ge_eSave_PulldownMenu_items_dyn23	= 6856,
    ge_eSave_PulldownMenu_items_dyn24	= 6857,
    ge_eSave_PulldownMenu_items_dyn25	= 6858,
    ge_eSave_PulldownMenu_items_dyn26	= 6859,
    ge_eSave_PulldownMenu_items_dyn27	= 6860,
    ge_eSave_PulldownMenu_items_dyn28	= 6861,
    ge_eSave_PulldownMenu_items_dyn29	= 6862,
    ge_eSave_PulldownMenu_items_dyn30	= 6863,
    ge_eSave_PulldownMenu_items_dyn31	= 6864,
    ge_eSave_OptionMenu_button_mask	= 6900,
    ge_eSave_OptionMenu_items_text0	= 6901,
    ge_eSave_OptionMenu_items_text1	= 6902,
    ge_eSave_OptionMenu_items_text2	= 6903,
    ge_eSave_OptionMenu_items_text3	= 6904,
    ge_eSave_OptionMenu_items_text4	= 6905,
    ge_eSave_OptionMenu_items_text5	= 6906,
    ge_eSave_OptionMenu_items_text6	= 6907,
    ge_eSave_OptionMenu_items_text7	= 6908,
    ge_eSave_OptionMenu_items_text8	= 6909,
    ge_eSave_OptionMenu_items_text9	= 6910,
    ge_eSave_OptionMenu_items_text10	= 6911,
    ge_eSave_OptionMenu_items_text11	= 6912,
    ge_eSave_OptionMenu_items_text12	= 6913,
    ge_eSave_OptionMenu_items_text13	= 6914,
    ge_eSave_OptionMenu_items_text14	= 6915,
    ge_eSave_OptionMenu_items_text15	= 6916,
    ge_eSave_OptionMenu_items_text16	= 6917,
    ge_eSave_OptionMenu_items_text17	= 6918,
    ge_eSave_OptionMenu_items_text18	= 6919,
    ge_eSave_OptionMenu_items_text19	= 6920,
    ge_eSave_OptionMenu_items_text20	= 6921,
    ge_eSave_OptionMenu_items_text21	= 6922,
    ge_eSave_OptionMenu_items_text22	= 6923,
    ge_eSave_OptionMenu_items_text23	= 6924,
    ge_eSave_OptionMenu_items_text24	= 6925,
    ge_eSave_OptionMenu_items_text25	= 6926,
    ge_eSave_OptionMenu_items_text26	= 6927,
    ge_eSave_OptionMenu_items_text27	= 6928,
    ge_eSave_OptionMenu_items_text28	= 6929,
    ge_eSave_OptionMenu_items_text29	= 6930,
    ge_eSave_OptionMenu_items_text30	= 6931,
    ge_eSave_OptionMenu_items_text31	= 6932,
    ge_eSave_OptionMenu_items_enum0	= 6933,
    ge_eSave_OptionMenu_items_enum1	= 6934,
    ge_eSave_OptionMenu_items_enum2	= 6935,
    ge_eSave_OptionMenu_items_enum3	= 6936,
    ge_eSave_OptionMenu_items_enum4	= 6937,
    ge_eSave_OptionMenu_items_enum5	= 6938,
    ge_eSave_OptionMenu_items_enum6	= 6939,
    ge_eSave_OptionMenu_items_enum7	= 6940,
    ge_eSave_OptionMenu_items_enum8	= 6941,
    ge_eSave_OptionMenu_items_enum9	= 6942,
    ge_eSave_OptionMenu_items_enum10	= 6943,
    ge_eSave_OptionMenu_items_enum11	= 6944,
    ge_eSave_OptionMenu_items_enum12	= 6945,
    ge_eSave_OptionMenu_items_enum13	= 6946,
    ge_eSave_OptionMenu_items_enum14	= 6947,
    ge_eSave_OptionMenu_items_enum15	= 6948,
    ge_eSave_OptionMenu_items_enum16	= 6949,
    ge_eSave_OptionMenu_items_enum17	= 6950,
    ge_eSave_OptionMenu_items_enum18	= 6951,
    ge_eSave_OptionMenu_items_enum19	= 6952,
    ge_eSave_OptionMenu_items_enum20	= 6953,
    ge_eSave_OptionMenu_items_enum21	= 6954,
    ge_eSave_OptionMenu_items_enum22	= 6955,
    ge_eSave_OptionMenu_items_enum23	= 6956,
    ge_eSave_OptionMenu_items_enum24	= 6957,
    ge_eSave_OptionMenu_items_enum25	= 6958,
    ge_eSave_OptionMenu_items_enum26	= 6959,
    ge_eSave_OptionMenu_items_enum27	= 6960,
    ge_eSave_OptionMenu_items_enum28	= 6961,
    ge_eSave_OptionMenu_items_enum29	= 6962,
    ge_eSave_OptionMenu_items_enum30	= 6963,
    ge_eSave_OptionMenu_items_enum31	= 6964,
    ge_eSave_OptionMenu_attribute	= 6965
  } ge_eSave;


class GeDynElem;
class Graph;

//! This class handles the dynamics for an object.
/*!
  Values for access, cycle, dyntype and actiontype are stored in the class.
  For every bit in dyntype and actiontype, an object for the corresponding dynamic class
  is created and inserted into the element list. At dynamic scan and when action is executed,
  this list is traversed and scan and action functions for all the elements are called.
*/
class GeDyn {
 public:
  GeDynElem	*elements;	      	//!< List of dynamic object specified in dyntype and actiontype.
  Graph		*graph;		       	//!< Graph.
  bool		ignore_color;		//!< All remaining elements with lower prio should ignore color dynamics.
  bool		reset_color;		//!< An element of higher prio is passing color dynamics to elements of lower prio.
  bool		ignore_invisible;	//!< All remaining elements with lower prio should ignore invisible dynamics.
  bool		reset_invisible;	//!< An element of higher prio is passing invisible dynamics to elements of lower prio.
  ge_mDynType	dyn_type;		//!< Dynamic type.
  ge_mDynType	total_dyn_type;		//!< Total dynamic type, including the dyntype of the nodeclass if Inherit is set.
  ge_mActionType action_type;		//!< Action type.
  ge_mActionType total_action_type;	//!< Total action type, including the actiontype of the nodeclass if Inherit is set.
  glow_mAccess	access;			//!< Access of the object.
  glow_eCycle	cycle;			//!< Cycle in which the scan is executed.
  bool		display_access;		//!< Display acess in attrbute editor.
  ge_eDynAttr	attr_editor;		//!< Controls attributes displayed in attribute editor.

  //! Constructor.
  /*! \param d_graph	Graph. 
      \param d_attr_editor Controls attributes displayed in attribute editor.
  */
  GeDyn( Graph *d_graph, ge_eDynAttr d_attr_editor = ge_eDynAttr_All) : 
    elements(0), graph(d_graph), ignore_color(false), 
    reset_color(false), ignore_invisible(false), reset_invisible(false),
    dyn_type(ge_mDynType_Inherit), total_dyn_type(ge_mDynType_Inherit), 
    action_type(ge_mActionType_Inherit), total_action_type(ge_mActionType_Inherit),
    access(glow_mAccess_AllRt), cycle(glow_eCycle_Inherit), attr_editor( d_attr_editor)
    {}

  //! Copy constructor.
  GeDyn( const GeDyn& x); 

  //! Get list of attributes.
  /*!
    \param object	Object.
    \param info		List of attribute items.
    \param item_count	Number of items in list.
  */
  void get_attributes( grow_tObject object, attr_sItem *info, int *item_count);

  //! Get translation table. Rename or hide the attributes from grow.
  /*!
    \param object	Object.
    \param transtab	Translation table.
  */
  void get_transtab( grow_tObject object, char **transtab);

  //! Update elementlist from dyntype and actiontype.
  void update_elements();

  //! Insert an element into the element list.
  /*! \param e		Element to insert. */
  void insert_element( GeDynElem *e);

  //! Disconnect dyamics.
  int disconnect( grow_tObject object);

  //! Connect dynamics.
  int connect( grow_tObject object, glow_sTraceData *trace_data);

  //! Scan dynamics.
  int scan( grow_tObject object);

  //! Save dynamic data to file.
  void save( ofstream& fp);

  //! Open dynamic data from file.
  void open( ifstream& fp);

  //! Execute action for an event.
  int action( grow_tObject object, glow_tEvent event);

  //! Execute action after confirm.
  int confirmed_action( grow_tObject object, glow_tEvent event);
  int change_value( grow_tObject object, char *text);
  ge_mDynType get_dyntype( grow_tObject object);
  ge_mActionType get_actiontype( grow_tObject object);
  glow_eDrawType get_color1( grow_tObject object, glow_eDrawType color);
  glow_eDrawType get_color2( grow_tObject object, glow_eDrawType color);
  void set_attribute( grow_tObject object, char *attr_name, int second);
  void set_color( grow_tObject object, glow_eDrawType color);
  int get_attr_typeid();
  void set_access( glow_mAccess acc);
  void set_dyn( ge_mDynType type, ge_mActionType action);
  void unset_inherit( grow_tObject object);
  void set_command( char *cmd);
  void set_hostobject( char *hostobject);
  void get_hostobject( char *hostobject);
  void set_value_input( char *format, double min_value, double max_value);
  int *ref_slider_disabled();
  int *ref_trend_hold();
  double *ref_trend_scantime();
  void *get_p();
  void set_p( void *p);
  void update();
  void export_java( grow_tObject object, ofstream& fp, char *var_name);
  void export_java_object( grow_tObject object, ofstream& fp, char *var_name);
  GeDynElem *create_dyn_element( int mask, int instance);
  GeDynElem *create_action_element( int mask, int instance);
  GeDynElem *copy_element( GeDynElem& x);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  graph_eDatabase parse_attr_name( char *name, char *parsed_name, 
				   int *inverted, int *type, int *size, int *elem = 0);
  void merge( GeDyn& x);
  static char *cmd_cnv( char *instr);
  static int instance_to_number( int instance);
  static void replace_attribute( char *attribute, int attr_size, char *from, char *to, int *cnt, int strict);
  static char *printstr( char *str);
};

//! Virtual baseclass for dynamic elements.
class GeDynElem {
 public:
  GeDyn		*dyn;		//!< Parent GeDyn object.
  ge_mDynType	dyn_type;	//!< Dyntype of the element.
  ge_mActionType action_type;	//!< Actiontype of the element.
  ge_eDynPrio	prio;		//!< Priority of the element.
  ge_mInstance	instance_mask;	//!< Instance mask. Only valid in the first instance.
  ge_mInstance	instance;	//!< Instance of the element
  GeDynElem	*next;		//!< Pointer to next element in list.

  //! Constructor.
  /*!
    \param e_dyn	Parent GeDyn object.
    \param e_dyn_type	Dyntype of the element.
    \param e_action_type Actiontype of the element.
    \param e_prio	Priority of the element.
  */
  GeDynElem( GeDyn *e_dyn, ge_mDynType e_dyn_type, ge_mActionType e_action_type, 
	     ge_eDynPrio e_prio):
    dyn(e_dyn), dyn_type(e_dyn_type), action_type(e_action_type), prio(e_prio), 
    instance_mask(ge_mInstance_1), instance(ge_mInstance_1), next(0)
    {}

  //! Copy constructor
  /*! \param x	Element to copy. */
  GeDynElem( const GeDynElem& x) : dyn(x.dyn), dyn_type(x.dyn_type), 
    action_type(x.action_type), prio(x.prio), next(0) {}

  //! Disconnect the element.
  /*! \param object 	Owner object. */
  virtual int disconnect( grow_tObject object);

  //! Connect the element.
  /*! 
    \param object 	Owner object. 
    \param trace_data	Trace info stored in grow for the object.
  */
  virtual int connect( grow_tObject object, glow_sTraceData *trace_data) { return 1;}

  //! Scan the element.
  /*! \param object 	Owner object. */
  virtual int scan( grow_tObject object) { return 1;}

  //! Get list of attributes.
  /*!
    \param attrinfo    	List of attribute items.
    \param item_count	Number of items in list.
  */
  virtual void get_attributes( attr_sItem *attrinfo, int *item_count) {}

  //! Get translation table. Rename or hide the attributes from grow.
  /*!
    \param tt	Translation table.
    \return	Returns 0 if a special translation table is found, else 1.
  */
  virtual int get_transtab( char **tt);

  //! Save dynamic data to file.
  /*! \param fp		Output file. */
  virtual void save( ofstream& fp) {}

  //! Open dynamic data from file.
  /*! \param fp		Input file. */
  virtual void open( ifstream& fp) {}

  //! Execute action for an event.
  /*!
    \param object	Owner object of the dynamics.
    \param event	Event.
  */
  virtual int action( grow_tObject object, glow_tEvent event) { return 1;}

  //! Change value for ValueInput element.
  /*! 
    \param object 	Owner object. 
    \param text		Text from value input field.
  */
  virtual int change_value( grow_tObject object, char *text) { return 1;}

  //! Set an 'Attribute' attribute, i.e. a reference to the database attribute.
  /*!
    \param object	Owner object.
    \param attr_name	Database reference.
    \param cnt		Counter to decide if first or second attribute is to be set.
  */
  virtual void set_attribute( grow_tObject object, char *attr_name, int *cnt) {}

  //! Set color or color tone.
  /*!
    \param object	Owner object.
    \param color	Color to set.
  */
  virtual int set_color( grow_tObject object, glow_eDrawType color) { return 0;}

  virtual void update() {}

  //! Export java code for the dynamic element.
  /*!
    \param object	Owner object.
    \param fp		Output file.
    \param first	First element in element list.
    \param var_name	Name of objects java variable name.
  */
  virtual int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name) { return 0;}

  //! Replace an attribute string
  /*!
    \param from		Attribute string to replace.
    \param to		New attribute string.
    \param cnt		Counter of replaced attributes.
    \param strict	If 1 the comparation is case sensitive.
  */
  virtual void replace_attribute( char *from, char *to, int *cnt, int strict) {}

  //! Destructor
  virtual ~GeDynElem() {};
};

//! Set the supplied fill color when the signal is low.
class GeDigLowColor : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for digital attribute.
  glow_eDrawType  color;	//!< Color to set when value is low.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;
  

  GeDigLowColor( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigLowColor, (ge_mActionType) 0, ge_eDynPrio_DigLowColor),
    color(glow_eDrawType_Inherit)
    { strcpy( attribute, "");}
  GeDigLowColor( const GeDigLowColor& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), color(x.color)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int set_color( grow_tObject object, glow_eDrawType color);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Set the supplied fillcolor when the value is high.
class GeDigColor : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for digital attribute.
  glow_eDrawType  color;	//!< Color to set when value is high.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeDigColor( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, ge_mDynType_DigColor, (ge_mActionType) 0, ge_eDynPrio_DigColor),
    color(glow_eDrawType_Inherit)
    { strcpy( attribute, ""); instance = e_instance;}
  GeDigColor( const GeDigColor& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), color(x.color)
    { strcpy( attribute, x.attribute); 
    instance = x.instance; instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int set_color( grow_tObject object, glow_eDrawType color);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Set yellow fill color when the signal is low.
class GeDigWarning : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for digital attribute.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeDigWarning( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigWarning, (ge_mActionType) 0, ge_eDynPrio_DigWarning)
    { strcpy( attribute, "");}
  GeDigWarning( const GeDigWarning& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Set red fill color when the signal is high.
class GeDigError : public GeDynElem {
 public:
  char attribute[120];		//!< Database refrence for digital attribute.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeDigError( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigError, (ge_mActionType) 0, ge_eDynPrio_DigError)
    { strcpy( attribute, "");}
  GeDigError( const GeDigError& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Flash with the supplied color when the signal is high.
class GeDigFlash : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference to digital attribute.
  glow_eDrawType  color;	//!< Flash color when the signal is high.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;
  bool on;

  GeDigFlash( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigFlash, (ge_mActionType) 0, ge_eDynPrio_DigFlash),
    color(glow_eDrawType_Inherit), on(true)
    { strcpy( attribute, "");}
  GeDigFlash( const GeDigFlash& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), color(x.color), on(x.on)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict); 
  int set_color( grow_tObject object, glow_eDrawType color);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Make the object invisible when the signal is high.
class GeInvisible : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for digital attribute.
  int dimmed;			//!< Object is dimmed (not invisible)

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;
  int a_typeid;

  GeInvisible( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, ge_mDynType_Invisible, (ge_mActionType) 0, ge_eDynPrio_Invisible), 
    dimmed(0)
    { strcpy( attribute, ""); instance = e_instance;}
  GeInvisible( const GeInvisible& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), dimmed(x.dimmed)
    { strcpy( attribute, x.attribute);
    instance = x.instance; instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Set the supplied border color when the signal is high.
class GeDigBorder : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference to digital attribute.
  glow_eDrawType  color;	//!< Bordercolor to set when the signal is high.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeDigBorder( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigBorder, (ge_mActionType) 0, ge_eDynPrio_DigBorder),
    color(glow_eDrawType_Inherit)
    { strcpy( attribute, "");}
  GeDigBorder( const GeDigBorder& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), color(x.color)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Set supplied text when the signal is low.
class GeDigText : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for digital attribute.
  char low_text[80];		//!< Text to set when the signal is low.

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;
  char high_text[80];

  GeDigText( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigText, (ge_mActionType) 0, ge_eDynPrio_DigText)
    { strcpy( attribute, ""); strcpy( low_text, "");}
  GeDigText( const GeDigText& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute); strcpy( low_text, x.low_text);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Display the value of a signal.
class GeValue : public GeDynElem {
 public:
  char attribute[120];	  	//!< Database reference of an attribute with arbitrary type. 
  char format[80];		//!< Format of conversion from value to string.
  int zero_blank;		//!< Blank field when value i zero (integer or float).

  void *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  bool first_scan;
  char old_value[80];
  int annot_typeid;
  int annot_size;
  pwr_tTid tid;

  GeValue( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, ge_mDynType_Value, (ge_mActionType) 0, ge_eDynPrio_Value),
    zero_blank(0), annot_typeid(0), annot_size(0), tid(0)
    { strcpy( attribute, ""); strcpy( format, ""); instance = e_instance;}
  GeValue( const GeValue& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), zero_blank(x.zero_blank)
    { strcpy( attribute, x.attribute); strcpy( format, x.format);
    instance = x.instance; instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count); 
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Add input function to Value dynamics.
class GeValueInput : public GeDynElem {
 public:
  double min_value;		//!< Minimum value for input.
  double max_value;		//!< Maximum value for input.
  int clear;			//!< Clear input field it is when opened.
  int popup;			//!< Input in popup dialog.
  int unselect;			//!< Text not selected in input field when opened.
	
  int annot_typeid;
  int annot_size;
  GeValue *value_element;

  GeValueInput( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_ValueInput, 
	      ge_eDynPrio_ValueInput),
    min_value(0), max_value(0), clear(0), popup(0), unselect(0), value_element(0)
    {}
  GeValueInput( const GeValueInput& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), min_value(x.min_value),
    max_value(x.max_value), clear(x.clear), popup(x.popup), unselect(x.unselect)
    {}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int action( grow_tObject object, glow_tEvent event);
  int change_value( grow_tObject object, char *text);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Set the supplied fill color when the signal exceeds or is below a specified limit value.
class GeAnalogColor : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference of analog attribute.
  double limit;			//!< Limit value.
  ge_eLimitType limit_type;	//!< Type of limit.
  glow_eDrawType color;		//!< Color to set when limit value is exceeded.

  bool old_state;
  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  int type;
  graph_eDatabase db;
  bool first_scan;
  pwr_tFloat32 old_value;

  GeAnalogColor( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, ge_mDynType_AnalogColor, (ge_mActionType) 0, ge_eDynPrio_AnalogColor),
    limit(0), limit_type(ge_eLimitType_Gt), color(glow_eDrawType_Inherit), p(0)
    { strcpy( attribute, ""); instance = e_instance;}
  GeAnalogColor( const GeAnalogColor& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), limit(x.limit), 
    limit_type(x.limit_type), color(x.color) 
    { strcpy( attribute, x.attribute);
    instance = x.instance; instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int set_color( grow_tObject object, glow_eDrawType color);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Rotate the component.
class GeRotate : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference of analog attribute.
  double x0;			//!< x coordinate for rotation point.
  double y0;			//!< y coordinate for rotation point.
  double factor;		//!< Conversion factor from value to rotation angel in degrees.

  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  bool first_scan;
  pwr_tFloat32 old_value;
  glow_eRotationPoint rotation_point;

  GeRotate( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Rotate, (ge_mActionType) 0, ge_eDynPrio_Rotate),
    x0(0), y0(0), factor(1)
    { strcpy( attribute, "");}
  GeRotate( const GeRotate& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), x0(x.x0), y0(x.y0), factor(x.factor)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Move an scale the component.
class GeMove : public GeDynElem {
 public:
  char move_x_attribute[120];
  char move_y_attribute[120];
  char scale_x_attribute[120];
  char scale_y_attribute[120];
  double x_offset;
  double y_offset;
  double factor;
  double scale_factor;
  glow_eScaleType scale_type;

  pwr_tFloat32 *move_x_p;
  pwr_tSubid move_x_subid;
  int move_x_size;
  int move_x_type;
  graph_eDatabase move_x_db;
  pwr_tFloat64 move_x_old_value;
  pwr_tFloat32 *move_y_p;
  pwr_tSubid move_y_subid;
  int move_y_size;
  int move_y_type;
  graph_eDatabase move_y_db;
  pwr_tFloat64 move_y_old_value;
  pwr_tFloat32 *scale_x_p;
  pwr_tSubid scale_x_subid;
  int scale_x_size;
  int scale_x_type;
  graph_eDatabase scale_x_db;
  pwr_tFloat64 scale_x_old_value;
  pwr_tFloat32 *scale_y_p;
  pwr_tSubid scale_y_subid;
  int scale_y_size;
  int scale_y_type;
  graph_eDatabase scale_y_db;
  pwr_tFloat64 scale_y_old_value;
  bool first_scan;
  double x_orig;
  double y_orig;
  double width_orig;
  double height_orig;

  GeMove( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Move, (ge_mActionType) 0, ge_eDynPrio_Move),
    x_offset(0), y_offset(0), factor(1), scale_factor(1), 
    scale_type(glow_eScaleType_LowerLeft)
    {
      strcpy( move_x_attribute, "");
      strcpy( move_y_attribute, "");
      strcpy( scale_x_attribute, "");
      strcpy( scale_y_attribute, "");
    }
  GeMove( const GeMove& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), x_offset(x.x_offset), 
    y_offset(x.y_offset), factor(x.factor), scale_factor(x.scale_factor), 
    scale_type(x.scale_type)
    { 
      strcpy( move_x_attribute, x.move_x_attribute); 
      strcpy( move_y_attribute, x.move_y_attribute);
      strcpy( scale_x_attribute, x.scale_x_attribute); 
      strcpy( scale_y_attribute, x.scale_y_attribute);
    }
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Shift between different pages in the subgraph. The value of the signal determine the page number.
class GeAnalogShift : public GeDynElem {
 public:
  char attribute[120];

  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  int type;
  graph_eDatabase db;
  bool first_scan;
  pwr_tFloat32 old_value;

  GeAnalogShift( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_AnalogShift, (ge_mActionType) 0, ge_eDynPrio_AnalogShift)
    { strcpy( attribute, "");}
  GeAnalogShift( const GeAnalogShift& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Shift between two pages in the subgraph (first and last).
class GeDigShift : public GeDynElem {
 public:
  char attribute[120];

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeDigShift( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_DigShift, (ge_mActionType) 0, ge_eDynPrio_DigShift)
    { strcpy( attribute, "");}
  GeDigShift( const GeDigShift& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Different types of animations.
class GeAnimation : public GeDynElem {
 public:
  char attribute[120];
  ge_eAnimSequence sequence;

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;
  int animation_count;
  int animation_direction;

  GeAnimation( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Animation, (ge_mActionType) 0, ge_eDynPrio_Animation),
    sequence( ge_eAnimSequence_Inherit)
    { strcpy( attribute, "");}
  GeAnimation( const GeAnimation& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), sequence(x.sequence)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Continuous update of an image file.
class GeVideo : public GeDynElem {
 public:
  GeVideo( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Video, (ge_mActionType) 0, ge_eDynPrio_Video)
    {}
  GeVideo( const GeVideo& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    {}
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);

};

//! Color of status attribute.
class GeStatusColor : public GeDynElem {
 public:
  char attribute[120];
  glow_eDrawType nostatus_color;

  pwr_tStatus *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int attr_type;
  bool first_scan;
  ge_ePwrStatus old_status;
  pwr_tStatus old_value;
  bool on;

  GeStatusColor( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_StatusColor, (ge_mActionType) 0, ge_eDynPrio_StatusColor),
    nostatus_color(glow_eDrawType_Inherit), on(true)
    { strcpy( attribute, "");}
  GeStatusColor( const GeStatusColor& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), nostatus_color(x.nostatus_color)
    { strcpy(attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int set_color( grow_tObject object, glow_eDrawType color);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Changes the fill color up to a certain level of the component.
class GeFillLevel : public GeDynElem {
 public:
  char attribute[120];
  glow_eDrawType color;
  glow_eDirection direction;
  double min_value;
  double max_value;
  char minvalue_attr[120];
  char maxvalue_attr[120];

  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  bool first_scan;
  pwr_tFloat32 old_value;
  bool limits_found;
  double limit_min;
  double limit_max;
  pwr_tFloat32 *min_value_p;
  pwr_tFloat32 *max_value_p;
  pwr_tSubid min_value_subid;
  pwr_tSubid max_value_subid;

  GeFillLevel( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_FillLevel, (ge_mActionType) 0, ge_eDynPrio_FillLevel),
    color( glow_eDrawType_Inherit), direction( glow_eDirection_Down), min_value(0),
    max_value(100), limits_found(false),
    min_value_p(0), max_value_p(0)
    { strcpy( attribute, ""); strcpy( minvalue_attr, ""); strcpy( maxvalue_attr, "");}
  GeFillLevel( const GeFillLevel& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), color(x.color),
    direction(x.direction), min_value(x.min_value),max_value(x.max_value),
    min_value_p(0), max_value_p(0)
    { strcpy( attribute, x.attribute); strcpy( minvalue_attr, x.minvalue_attr);
    strcpy( maxvalue_attr, x.maxvalue_attr);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! HostObject connected to several attributes specified in the subgraph.
class GeHostObject : public GeDynElem {
 public:
  char hostobject[120];

  GeHostObject( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_HostObject, (ge_mActionType) 0, ge_eDynPrio_HostObject)
    { strcpy( hostobject, "");}
  GeHostObject( const GeHostObject& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy(hostobject, x.hostobject);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Display the methods popup menu.
class GePopupMenu : public GeDynElem {
 public:
  char ref_object[120];

  GePopupMenu( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_PopupMenu, ge_eDynPrio_PopupMenu)
    { strcpy( ref_object, "");}
  GePopupMenu( const GePopupMenu& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( ref_object, x.ref_object);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Set a digital signal
class GeSetDig : public GeDynElem {
 public:
  char attribute[120];

  GeSetDig( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_SetDig, ge_eDynPrio_SetDig)
    { strcpy( attribute, ""); instance = e_instance;}
  GeSetDig( const GeSetDig& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);
    instance = x.instance, instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Reset a digital signal
class GeResetDig : public GeDynElem {
 public:
  char attribute[120];

  GeResetDig( GeDyn *e_dyn, ge_mInstance e_instance = ge_mInstance_1) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_ResetDig, ge_eDynPrio_ResetDig)
    { strcpy( attribute, ""); instance = e_instance;}
  GeResetDig( const GeResetDig& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);
    instance = x.instance; instance_mask = x.instance_mask;}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Toggle a digital signal
class GeToggleDig : public GeDynElem {
 public:
  char attribute[120];

  GeToggleDig( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_ToggleDig, ge_eDynPrio_ToggleDig)
    { strcpy( attribute, "");}
  GeToggleDig( const GeToggleDig& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Sto a digital signal,
class GeStoDig : public GeDynElem {
 public:
  char attribute[120];

  GeStoDig( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_StoDig, ge_eDynPrio_StoDig)
    { strcpy( attribute, "");}
  GeStoDig( const GeStoDig& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Execute a command.
class GeCommand : public GeDynElem {
 public:
  char command[120];

  GeCommand( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_Command, ge_eDynPrio_Command)
    { strcpy( command, "");}
  GeCommand( const GeCommand& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( command, x.command);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Execute a command on double click.
class GeCommandDoubleClick : public GeDynElem {
 public:
  char command[120];

  GeCommandDoubleClick( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_CommandDoubleClick, 
	      ge_eDynPrio_CommandDoubleClick)
    { strcpy( command, "");}
  GeCommandDoubleClick( const GeCommandDoubleClick& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( command, x.command);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);

};

//! Display confirm dialog before execution of action.
class GeConfirm : public GeDynElem {
 public:
  char text[120];

  GeConfirm( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_Confirm, ge_eDynPrio_Confirm)
    { strcpy( text, "");}
  GeConfirm( const GeConfirm& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( text, x.text);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Increase an analog signal.
class GeIncrAnalog : public GeDynElem {
 public:
  char attribute[120];
  double increment;
  double min_value;
  double max_value;

  GeIncrAnalog( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_IncrAnalog, ge_eDynPrio_IncrAnalog), 
    increment(1), min_value(0), max_value(0)
    { strcpy( attribute, "");}
  GeIncrAnalog( const GeIncrAnalog& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), increment(x.increment),
    min_value(min_value), max_value(max_value)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Dynamics for a radiobutton.
class GeRadioButton : public GeDynElem {
 public:
  char attribute[120];

  pwr_tBoolean *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tBoolean old_value;

  GeRadioButton( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType)0, ge_mActionType_RadioButton, 
	      ge_eDynPrio_RadioButton)
    { strcpy( attribute, "");}
  GeRadioButton( const GeRadioButton& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute, x.attribute);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Display tooltip text.
class GeTipText : public GeDynElem {
 public:
  char text[200];

  GeTipText( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType)0, ge_mActionType_TipText, 
	      ge_eDynPrio_TipText)
    { strcpy( text, "");}
  GeTipText( const GeTipText& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( text, x.text);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);

};

//! Open help window.
class GeHelp : public GeDynElem {
 public:
  char topic[80];
  char bookmark[80];

  GeHelp( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_Help, ge_eDynPrio_Help)
    { strcpy( topic, ""); strcpy( bookmark, "");}
  GeHelp( const GeHelp& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( topic, x.topic); strcpy( bookmark, x.bookmark);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Open another graph.
class GeOpenGraph : public GeDynElem {
 public:
  char graph_object[80];

  GeOpenGraph( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_OpenGraph, ge_eDynPrio_OpenGraph)
    { strcpy( graph_object, "");}
  GeOpenGraph( const GeOpenGraph& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( graph_object, x.graph_object);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Open an URL.
class GeOpenURL : public GeDynElem {
 public:
  char url[200];

  GeOpenURL( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_OpenURL, ge_eDynPrio_OpenURL)
    { strcpy( url, "");}
  GeOpenURL( const GeOpenURL& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( url, x.url);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  int get_transtab( char **tt);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Handle input focus.
class GeInputFocus : public GeDynElem {
 public:
  int initial_focus;
  char next_horizontal[40];
  char next_vertical[40];
  char next_tab[40];
  grow_tObject prev_tab;

  GeInputFocus( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_InputFocus, ge_eDynPrio_InputFocus),
    initial_focus(0), prev_tab(0)
    { strcpy( next_horizontal, ""); strcpy( next_vertical, ""); strcpy( next_tab, "");}
  GeInputFocus( const GeInputFocus& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), initial_focus(x.initial_focus),
    prev_tab(0)
    { strcpy( next_horizontal, x.next_horizontal); strcpy( next_vertical, x.next_vertical);
      strcpy( next_tab, x.next_tab);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
};

//! Close current graph.
class GeCloseGraph : public GeDynElem {
 public:

  GeCloseGraph( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_CloseGraph, ge_eDynPrio_CloseGraph)
    {}
  GeCloseGraph( const GeCloseGraph& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    {}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Dynamics for a slider object.
class GeSlider : public GeDynElem {
 public:
  char attribute[120];
  char minvalue_attr[120];
  char maxvalue_attr[120];
  int slider_disabled;

  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  int inverted;
  bool first_scan;
  pwr_tFloat32 old_value;
  int attr_type;
  pwr_tFloat32 *min_value_p;
  pwr_tFloat32 *max_value_p;
  pwr_tFloat32 old_min_value;
  pwr_tFloat32 old_max_value;
  pwr_tSubid min_value_subid;
  pwr_tSubid max_value_subid;

  GeSlider( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType)0, ge_mActionType_Slider, ge_eDynPrio_Slider),
    min_value_p(0), max_value_p(0), old_min_value(0), old_max_value(0)
    { strcpy( attribute, ""); strcpy( minvalue_attr, ""); strcpy( maxvalue_attr, "");}
  GeSlider( const GeSlider& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio),
    min_value_p(0), max_value_p(0), old_min_value(0), old_max_value(0)
    { strcpy( attribute, x.attribute); strcpy( minvalue_attr, x.minvalue_attr);
    strcpy( maxvalue_attr, x.maxvalue_attr);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  int action( grow_tObject object, glow_tEvent event);
  void update() { first_scan = true;}
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Dynamics for a bar object.
class GeBar : public GeDynElem {
 public:
  char attribute[120];
  char minvalue_attr[120];
  char maxvalue_attr[120];

  pwr_tFloat32 *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  bool first_scan;
  pwr_tFloat32 old_value;
  int bar_typeid;
  pwr_tFloat32 *min_value_p;
  pwr_tFloat32 *max_value_p;
  pwr_tFloat32 old_min_value;
  pwr_tFloat32 old_max_value;
  pwr_tSubid min_value_subid;
  pwr_tSubid max_value_subid;

  GeBar( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Bar, (ge_mActionType) 0, ge_eDynPrio_Bar), 
    min_value_p(0), max_value_p(0), old_min_value(0), old_max_value(0)
    { strcpy( attribute, ""); strcpy( minvalue_attr, ""); strcpy( maxvalue_attr, "");}
  GeBar( const GeBar& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio),
    min_value_p(0), max_value_p(0), old_min_value(0), old_max_value(0)
    { strcpy( attribute, x.attribute); strcpy( minvalue_attr, x.minvalue_attr);
    strcpy( maxvalue_attr, x.maxvalue_attr);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  void configure( grow_tObject object);
};

//! Dynamics for a trend object.
class GeTrend : public GeDynElem {
 public:
  char attribute1[120];
  char attribute2[120];
  char minvalue_attr1[120];
  char maxvalue_attr1[120];
  char minvalue_attr2[120];
  char maxvalue_attr2[120];

  bool first_scan;
  double scan_time;
  double acc_time;
  int trend_hold;
  pwr_tFloat32 *p1;
  pwr_tSubid subid1;
  int size1;
  graph_eDatabase db1;
  int trend_typeid1;
  pwr_tFloat32 *p2;
  pwr_tSubid subid2;
  int size2;
  graph_eDatabase db2;
  int trend_typeid2;
  pwr_tFloat32 *min_value1_p;
  pwr_tFloat32 *max_value1_p;
  pwr_tFloat32 old_min_value1;
  pwr_tFloat32 old_max_value1;
  pwr_tSubid min_value_subid1;
  pwr_tSubid max_value_subid1;
  pwr_tFloat32 *min_value2_p;
  pwr_tFloat32 *max_value2_p;
  pwr_tFloat32 old_min_value2;
  pwr_tFloat32 old_max_value2;
  pwr_tSubid min_value_subid2;
  pwr_tSubid max_value_subid2;

  GeTrend( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Trend, (ge_mActionType) 0, ge_eDynPrio_Trend),
    min_value1_p(0), max_value1_p(0), old_min_value1(0), old_max_value1(0),
    min_value2_p(0), max_value2_p(0), old_min_value2(0), old_max_value2(0)
    { strcpy( attribute1, ""); strcpy( attribute2, "");
    strcpy( minvalue_attr1, ""); strcpy( maxvalue_attr1, "");
    strcpy( minvalue_attr2, ""); strcpy( maxvalue_attr2, "");}
  GeTrend( const GeTrend& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { strcpy( attribute1, x.attribute1); strcpy( attribute2, x.attribute2); 
    strcpy( minvalue_attr1, x.minvalue_attr1); strcpy( maxvalue_attr1, x.maxvalue_attr1); 
    strcpy( minvalue_attr2, x.minvalue_attr2); strcpy( maxvalue_attr2, x.maxvalue_attr2);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
};

//! Dynamics for a trend object.
class GeFastCurve : public GeDynElem {
 public:
  char fast_object[120];
  int curve_index1;
  int curve_index2;

  pwr_tBoolean 	*new_p;			//!< Pointer to New attribute, which indicates new curve data. 
  pwr_tRefId 	subid;			//!< Subid of New attribute in DsFastCurve.
  pwr_sAttrRef 	time_buff;		//!< Attrref to data array for time axis.
  pwr_sAttrRef 	buff[FAST_CURVES]; 	//!< Attrefs to data arrays for curves.
  pwr_eType  	type[FAST_CURVES]; 	//!< Types of curves.
  int	       	fast_idx[FAST_CURVES]; 	//!< Conversion from DsFastCurve index to GeCurveData index.
  int	       	curve_idx[DYN_FAST_MAX]; //!< Conversion from GeCurveData index to DsFastCurve index.
  int        	element_size[DYN_FAST_MAX]; //!< Size of data element in curves.
  int        	max_points;		//!< Max number of points in curves.
  pwr_tBoolean 	old_new;		//!< Old value of new. Used to detect edge of New.
  pwr_tMask	fast_function;		//!< Function configured in DsFastCurve object.
  bool		first_scan;		//!< Indicates that this is the first scan.
  pwr_sAttrRef 	trigg_index_attr;	//!< Attrref to TriggIndex attribute in DsFastCurve object.
  pwr_sAttrRef 	first_index_attr;	//!< Attrref to FirstIndex attribute in DsFastCurve object.
  pwr_sAttrRef 	last_index_attr;	//!< Attrref to LastIndex attribute in DsFastCurve object.
  int		fast_cnt;

  GeFastCurve( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_FastCurve, (ge_mActionType) 0, 
	      ge_eDynPrio_FastCurve), curve_index1(0), curve_index2(0)
    { strcpy( fast_object, "");}
  GeFastCurve( const GeFastCurve& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), 
    curve_index1(x.curve_index1), curve_index2(x.curve_index2)
    { strcpy( fast_object, x.fast_object);}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
};

//! Dynamics for a table object.
class GeTable : public GeDynElem {
 public:
  char attribute[TABLE_MAX_COL][120];
  char format[TABLE_MAX_COL][80];
  char sel_attribute[TABLE_MAX_COL][120];

  int columns;
  int rows;
  char *p[TABLE_MAX_COL];
  pwr_tSubid subid[TABLE_MAX_COL];
  int size[TABLE_MAX_COL];
  graph_eDatabase db[TABLE_MAX_COL];
  bool first_scan;
  char *old_value[TABLE_MAX_COL];
  int type_id[TABLE_MAX_COL];
  int elements[TABLE_MAX_COL];
  int is_headerref[TABLE_MAX_COL];
  char **headerref_p[TABLE_MAX_COL];
  pwr_tSubid *headerref_subid[TABLE_MAX_COL];

  pwr_tBoolean *sel_p[TABLE_MAX_COL];
  pwr_tSubid sel_subid[TABLE_MAX_COL];
  graph_eDatabase sel_db[TABLE_MAX_COL];
  int sel_elements[TABLE_MAX_COL];

  GeTable( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, ge_mDynType_Table, (ge_mActionType) 0, 
	      ge_eDynPrio_Table)
    { memset( attribute,0,sizeof(attribute)); memset( format,0,sizeof(format)); 
    memset( sel_attribute,0,sizeof(sel_attribute)); memset(old_value,0,sizeof(old_value));
    memset( is_headerref, 0, sizeof(is_headerref)); memset(headerref_p,0,sizeof(headerref_p));
    memset( sel_elements, 0, sizeof(sel_elements));
    }
  GeTable( const GeTable& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio)
    { memcpy( attribute, x.attribute,sizeof(attribute)); memcpy( format, x.format, sizeof(format));
    memcpy( sel_attribute, x.sel_attribute,sizeof(sel_attribute));
    memset( is_headerref, 0, sizeof(is_headerref)); memset(headerref_p,0,sizeof(headerref_p));
    }
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  int action( grow_tObject object, glow_tEvent event);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Pulldown menu.
class GePulldownMenu : public GeDynElem {
 public:
  GeDyn *items_dyn[32];
  char  items_text[80][32];
  unsigned int button_mask;
  grow_tObject menu_object;
  pwr_tTime focus_gained_time;

  GePulldownMenu( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_PulldownMenu, ge_eDynPrio_PulldownMenu),
    button_mask(1), menu_object(0)
    { 
      memset( items_dyn, 0, sizeof( items_dyn));
      memset( items_text, 0, sizeof( items_text));
      memset( &focus_gained_time, 0, sizeof( focus_gained_time));
      items_dyn[0] = new GeDyn( dyn->graph, ge_eDynAttr_Menu);
    }
  GePulldownMenu( const GePulldownMenu& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), button_mask(x.button_mask), menu_object(0)
    {
      memset( items_dyn, 0, sizeof( items_dyn));
      memset( &focus_gained_time, 0, sizeof( focus_gained_time));
      memcpy( items_text, x.items_text, sizeof(items_text)); 
      for ( int i = 0; i < 32; i++) {
	if ( x.items_dyn[i])
	  items_dyn[i] = new GeDyn( *x.items_dyn[i]);
      }
    }
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int get_transtab( char **tt);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Options menu.
class GeOptionMenu : public GeDynElem {
 public:
  char attribute[120];		//!< Database reference for analog attribute.
  unsigned int items_enum[32];
  char  items_text[80][32];
  unsigned int button_mask;
  grow_tObject menu_object;
  pwr_tTime focus_gained_time;

  void *p;
  pwr_tSubid subid;
  int size;
  graph_eDatabase db;
  bool first_scan;
  char old_value[8];
  int type_id;

  GeOptionMenu( GeDyn *e_dyn) : 
    GeDynElem(e_dyn, (ge_mDynType) 0, ge_mActionType_OptionMenu, ge_eDynPrio_OptionMenu),
    button_mask(1), menu_object(0)
    { 
      strcpy( attribute, "");
      for ( int i = 0; i < 32; i++)
        items_enum[i] = i;
      memset( items_text, 0, sizeof( items_text));
      memset( &focus_gained_time, 0, sizeof( focus_gained_time));
    }
  GeOptionMenu( const GeOptionMenu& x) : 
    GeDynElem(x.dyn,x.dyn_type,x.action_type,x.prio), button_mask(x.button_mask), menu_object(0)
    {
      memset( &focus_gained_time, 0, sizeof( focus_gained_time));
      strcpy( attribute, x.attribute);
      memcpy( items_text, x.items_text, sizeof(items_text)); 
      for ( int i = 0; i < 32; i++) {
	items_enum[i] = x.items_enum[i];
      }
    }
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event);
  int connect( grow_tObject object, glow_sTraceData *trace_data);
  int disconnect( grow_tObject object);
  int scan( grow_tObject object);
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int get_transtab( char **tt);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

//! Analog text.
class GeAnalogText : public GeOptionMenu {
 public:
  GeAnalogText( GeDyn *e_dyn) : 
    GeOptionMenu(e_dyn)
    { dyn_type = ge_mDynType_AnalogText; action_type = ge_mActionType_No; prio = ge_eDynPrio_AnalogText;}

  GeAnalogText( const GeAnalogText& x) : 
    GeOptionMenu(x)
    {}
  void get_attributes( attr_sItem *attrinfo, int *item_count);
  void save( ofstream& fp);
  void open( ifstream& fp);
  int action( grow_tObject object, glow_tEvent event) { return 1;}
  void set_attribute( grow_tObject object, char *attr_name, int *cnt);
  void replace_attribute( char *from, char *to, int *cnt, int strict);
  int export_java( grow_tObject object, ofstream& fp, bool first, char *var_name);
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif












