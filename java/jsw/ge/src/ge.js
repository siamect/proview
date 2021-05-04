"use strict";

if ( !Object.create) {
  Object.create = function(proto) {
    function ctor() {}
    ctor.prototype = proto;
    return new ctor();
  };
}

#jsc_include pwr.jsi
#jsc_include cli.jsi
#jsc_include gdh.jsi
#jsc_include glow.jsi




var DynC = {
  DYN__NO_PROPAGATE 			: 20001,
  eValueInput_Success			: 0,
  eValueInput_Error			: 1,
  eValueInput_SyntaxError	       	: 2,
  eValueInput_MinValueExceeded       	: 3,
  eValueInput_MaxValueExceeded       	: 4,

  eCurveDataType_XYArrays    		: 0,
  eCurveDataType_PointArray  		: 1,
  eCurveDataType_TableObject  	: 2,

  mInstance_1				: 1 << 0,
  mInstance_2				: 1 << 1,
  mInstance_3				: 1 << 2,
  mInstance_4				: 1 << 3,
  mInstance_5				: 1 << 4,
  mInstance_6				: 1 << 5,
  mInstance_7				: 1 << 6,
  mInstance_8				: 1 << 7,
  mInstance_9				: 1 << 8,
  mInstance_10			: 1 << 9,
  mInstance_11			: 1 << 10,
  mInstance_12			: 1 << 11,
  mInstance_13			: 1 << 12,
  mInstance_14			: 1 << 13,
  mInstance_15			: 1 << 14,
  mInstance_16			: 1 << 15,
  mInstance_17			: 1 << 16,
  mInstance_18			: 1 << 17,
  mInstance_19			: 1 << 18,
  mInstance_20			: 1 << 19,
  mInstance_21			: 1 << 20,
  mInstance_22			: 1 << 21,
  mInstance_23			: 1 << 22,
  mInstance_24			: 1 << 23,
  mInstance_25			: 1 << 24,
  mInstance_26			: 1 << 25,
  mInstance_27			: 1 << 26,
  mInstance_28			: 1 << 27,
  mInstance_29			: 1 << 28,
  mInstance_30			: 1 << 29,
  mInstance_31			: 1 << 30,
  mInstance_32			: 1 << 31,

  mDynType1_No			: 0,
  mDynType1_Inherit            	: 1 << 0,
  mDynType1_Tone	       	: 1 << 1,
  mDynType1_DigLowColor		: 1 << 2,
  mDynType1_DigColor           	: 1 << 3,
  mDynType1_DigError           	: 1 << 4,
  mDynType1_DigWarning		: 1 << 5,
  mDynType1_DigFlash           	: 1 << 6,
  mDynType1_Invisible 		: 1 << 7,
  mDynType1_DigBorder          	: 1 << 8,
  mDynType1_DigText            	: 1 << 9,
  mDynType1_Value	       	: 1 << 10,
  mDynType1_AnalogColor		: 1 << 11,
  mDynType1_Rotate	       	: 1 << 12,
  mDynType1_Move	       	: 1 << 13,
  mDynType1_AnalogShift		: 1 << 14,
  mDynType1_DigShift           	: 1 << 15,
  mDynType1_Animation          	: 1 << 16,
  mDynType1_Bar			: 1 << 17,
  mDynType1_Trend	       	: 1 << 18,
  mDynType1_SliderBackground   	: 1 << 19,
  mDynType1_Video	       	: 1 << 20,
  mDynType1_FillLevel	       	: 1 << 21,
  mDynType1_FastCurve	       	: 1 << 22,
  mDynType1_AnalogText	      	: 1 << 23,
  mDynType1_Table	       	: 1 << 24,
  mDynType1_StatusColor        	: 1 << 25,
  mDynType1_HostObject         	: 1 << 26,
  mDynType1_DigSound           	: 1 << 27,
  mDynType1_XY_Curve           	: 1 << 28,
  mDynType1_DigCommand         	: 1 << 29,
  mDynType1_Pie	    		: 1 << 30,
  mDynType1_BarChart           	: 1 << 31,

  mDynType2_No			: 0,
  mDynType2_Axis	       	: 1 << 0,
  mDynType2_DigTextColor       	: 1 << 1,
  mDynType2_TimeoutColor       	: 1 << 2,
  mDynType2_DigFourShift       	: 1 << 3,
  mDynType2_ScrollingText       : 1 << 4,
  mDynType2_ColorThemeLightness : 1 << 5,
  mDynType2_DigBackgroundColor  : 1 << 6,
  mDynType2_DigSwap	        : 1 << 7,
  mDynType2_DigScript	        : 1 << 8,
  mDynType2_RefUpdate	        : 1 << 9,
  mDynType2_DsTrend	        : 1 << 10,
  mDynType2_DsTrendCurve        : 1 << 11,
  mDynType2_SevHist	        : 1 << 12,

  mActionType1_No		: 0,
  mActionType1_Inherit		: 1 << 0,
  mActionType1_PopupMenu       	: 1 << 1,
  mActionType1_SetDig          	: 1 << 2,
  mActionType1_ResetDig		: 1 << 3,
  mActionType1_ToggleDig       	: 1 << 4,
  mActionType1_StoDig          	: 1 << 5,
  mActionType1_Command		: 1 << 6,
  mActionType1_CommandDoubleClick : 1 << 7,
  mActionType1_Confirm		: 1 << 8,
  mActionType1_IncrAnalog      	: 1 << 9,
  mActionType1_RadioButton     	: 1 << 10,
  mActionType1_Slider          	: 1 << 11,
  mActionType1_ValueInput      	: 1 << 12,
  mActionType1_TipText         	: 1 << 13,
  mActionType1_Help            	: 1 << 14,
  mActionType1_OpenGraph       	: 1 << 15,
  mActionType1_OpenURL		: 1 << 16,
  mActionType1_InputFocus      	: 1 << 17,
  mActionType1_CloseGraph      	: 1 << 18,
  mActionType1_PulldownMenu    	: 1 << 19,
  mActionType1_OptionMenu      	: 1 << 20,
  mActionType1_SetValue        	: 1 << 21,
  mActionType1_MethodToolbar   	: 1 << 22,
  mActionType1_MethodPulldownMenu : 1 << 23,
  mActionType1_Script	      	: 1 << 24,
  mActionType1_CatchSignal     	: 1 << 25,
  mActionType1_EmitSignal     	: 1 << 26,

  mActionType2_No	       	: 0,
    
  //! Priority order for dyntypes and actiontypes. Lower value gives higher priority.
  eDynPrio_DigSound			: 0,
  eDynPrio_HostObject			: 1,
  eDynPrio_Invisible			: 2,
  eDynPrio_DigFlash			: 3,
  eDynPrio_TimeoutColor	       	: 4,
  eDynPrio_DigError			: 5,
  eDynPrio_DigWarning			: 6,
  eDynPrio_AnalogColor	       	: 7,
  eDynPrio_DigColor			: 8,
  eDynPrio_DigLowColor	       	: 9,
  eDynPrio_DigBackgroundColor	       	: 10,
  eDynPrio_DigTextColor	       	: 11,
  eDynPrio_DigBorder			: 12,
  eDynPrio_DigText			: 13,
  eDynPrio_FillLevel			: 14,
  eDynPrio_Value			: 15,
  eDynPrio_ValueInput			: 16,
  eDynPrio_Rotate			: 17,
  eDynPrio_Move			: 18,
  eDynPrio_AnalogShift	       	: 19,
  eDynPrio_DigShift			: 20,
  eDynPrio_DigFourShift	       	: 21,
  eDynPrio_Animation			: 22,
  eDynPrio_Bar			: 23,
  eDynPrio_Trend			: 24,
  eDynPrio_FastCurve			: 25,
  eDynPrio_XY_Curve			: 26,
  eDynPrio_DsTrend			: 27,
  eDynPrio_DsTrendCurve			: 28,
  eDynPrio_SevHist			: 29,
  eDynPrio_AnalogText			: 30,
  eDynPrio_Table			: 31,
  eDynPrio_SliderBackground	       	: 32,
  eDynPrio_Video			: 33,
  eDynPrio_StatusColor	       		: 34,
  eDynPrio_PopupMenu			: 35,
  eDynPrio_Confirm			: 36,
  eDynPrio_SetDig			: 37,
  eDynPrio_ResetDig			: 38,
  eDynPrio_ToggleDig			: 39,
  eDynPrio_StoDig			: 40,
  eDynPrio_Help				: 41,
  eDynPrio_OpenGraph			: 42,
  eDynPrio_OpenURL			: 43,
  eDynPrio_CommandDoubleClick	       	: 44,
  eDynPrio_IncrAnalog			: 45,
  eDynPrio_RadioButton	       		: 46,
  eDynPrio_Slider			: 47,
  eDynPrio_TipText			: 48,
  eDynPrio_PulldownMenu	       		: 49,
  eDynPrio_OptionMenu			: 50,
  eDynPrio_InputFocus			: 51,
  eDynPrio_DigCommand			: 52,
  eDynPrio_SetValue			: 53,
  eDynPrio_Pie				: 54,
  eDynPrio_BarChart			: 55,
  eDynPrio_Axis				: 56,
  eDynPrio_MethodToolbar	       	: 57,
  eDynPrio_MethodPulldownMenu	       	: 58,
  eDynPrio_ScrollingText	       	: 59,
  eDynPrio_ColorThemeLightness       	: 60,
  eDynPrio_DigSwap		       	: 61,
  eDynPrio_DigScript		       	: 62,
  eDynPrio_CatchSignal           	: 63,
  eDynPrio_EmitSignal     	       	: 64,
  eDynPrio_Script  			: 9998,
  eDynPrio_Command  			: 9999,
  eDynPrio_CloseGraph 		: 10000,

  eSave_Dyn		       		: 1,
  eSave_DigLowColor	       		: 2,
  eSave_DigColor	       		: 3,
  eSave_DigError	       		: 7,
  eSave_DigWarning	       		: 8,
  eSave_Invisible	       		: 9,
  eSave_DigBorder	       		: 10,
  eSave_DigText	       		: 11,
  eSave_Value				: 12,
  eSave_ValueInput	       		: 13,
  eSave_Rotate	       		: 14,
  eSave_Move		       		: 15,
  eSave_AnalogShift	       		: 17,
  eSave_DigShift	       		: 18,
  eSave_Animation	       		: 19,
  eSave_Bar		       		: 22,
  eSave_Trend				: 23,
  eSave_SliderBackground     		: 24,
  eSave_Video				: 25,
  eSave_DigFlash	       		: 26,
  eSave_FillLevel	       		: 27,
  eSave_FastCurve   			: 28,
  eSave_AnalogText	    		: 29,
  eSave_Table	       			: 30,
  eSave_StatusColor         		: 31,
  eSave_HostObject         		: 32,
  eSave_DigSound	      		: 33,
  eSave_XY_Curve	      		: 34,
  eSave_DigCommand	       		: 35,
  eSave_Pie	             		: 36,
  eSave_BarChart	       		: 37,
  eSave_Axis	             		: 38,
  eSave_DigTextColor	             	: 39,
  eSave_TimeoutColor	             	: 40,
  eSave_DigFourShift	             	: 41,
  eSave_ScrollingText             	: 42,
  eSave_ColorThemeLightness        	: 43,
  eSave_DigBackgroundColor        	: 44,
  eSave_DigSwap	        	: 45,
  eSave_DigScript	        	: 46,
  eSave_RefUpdate	        	: 47,
  eSave_DsTrend	        		: 48,
  eSave_DsTrendCurve	        	: 49,
  eSave_PopupMenu	       		: 50,
  eSave_SetDig	       		: 51,
  eSave_ResetDig	       		: 52,
  eSave_ToggleDig	       		: 53,
  eSave_StoDig	       		: 54,
  eSave_Command	       		: 55,
  eSave_CommandDC	       		: 56,
  eSave_Confirm	       		: 57,
  eSave_IncrAnalog	       		: 58,
  eSave_RadioButton	       		: 59,
  eSave_Slider	       		: 60,
  eSave_AnalogColor	       		: 61,
  eSave_TipText	       		: 62,
  eSave_Help		       		: 63,
  eSave_OpenGraph	       		: 64,
  eSave_OpenURL	       		: 65,
  eSave_InputFocus	       		: 66,
  eSave_CloseGraph	       		: 67,
  eSave_PulldownMenu         		: 68,
  eSave_OptionMenu           		: 69,
  eSave_SetValue	       		: 70,
  eSave_MethodToolbar	       		: 71,
  eSave_MethodPulldownMenu	       	: 72,
  eSave_Script		       	: 73,
  eSave_CatchSignal		       	: 74,
  eSave_EmitSignal		       	: 75,
  eSave_SevHist			       	: 76,
  eSave_End		       		: 99,
  eSave_Dyn_dyn_type1	       		: 100,
  eSave_Dyn_action_type1      	: 101,
  eSave_Dyn_access	       		: 102,
  eSave_Dyn_cycle	       		: 103,
  eSave_Dyn_ref_object      		: 104,
  eSave_Dyn_dyn_type2	       		: 105,
  eSave_Dyn_action_type2     		: 106,
  eSave_DigLowColor_attribute		: 200,
  eSave_DigLowColor_color    		: 201,
  eSave_DigColor_attribute   		: 300,
  eSave_DigColor_color       		: 301,
  eSave_DigColor_instance    		: 302,
  eSave_DigColor_instance_mask 	: 303,
  eSave_DigError_attribute   		: 700,
  eSave_DigError_use_colortheme    	: 701,
  eSave_DigWarning_attribute  	: 800,
  eSave_DigWarning_use_colortheme  	: 801,
  eSave_Invisible_attribute 		: 900,
  eSave_Invisible_dimmed     		: 901,
  eSave_Invisible_instance   		: 902,
  eSave_Invisible_instance_mask 	: 903,
  eSave_DigBorder_attribute  		: 1000,
  eSave_DigBorder_color      		: 1001,
  eSave_DigText_attribute    		: 1100,
  eSave_DigText_low_text     		: 1101,
  eSave_DigText_instance     		: 1102,
  eSave_DigText_instance_mask 	: 1103,
  eSave_Value_attribute      		: 1200,
  eSave_Value_format	       		: 1201,
  eSave_Value_instance      		: 1202,
  eSave_Value_instance_mask 		: 1203,
  eSave_Value_zero_blank     		: 1204,
  eSave_Value_decimals_attr          	: 1205,
  eSave_Value_decimals_decr          	: 1206,
  eSave_ValueInput_attribute      	: 1300,
  eSave_ValueInput_format		: 1301,
  eSave_ValueInput_min_value      	: 1302,
  eSave_ValueInput_max_value      	: 1303,
  eSave_ValueInput_clear      	: 1304,
  eSave_ValueInput_popup      	: 1305,
  eSave_ValueInput_unselect      	: 1306,
  eSave_ValueInput_minvalue_attr   	: 1307,
  eSave_ValueInput_maxvalue_attr   	: 1308,
  eSave_ValueInput_escape_store    	: 1309,
  eSave_ValueInput_keyboard_type    	: 1310,
  eSave_ValueInput_update_open    	: 1311,
  eSave_Rotate_attribute		: 1400,
  eSave_Rotate_x0			: 1401,
  eSave_Rotate_y0			: 1402,
  eSave_Rotate_factor             	: 1403,
  eSave_Move_move_x_attribute     	: 1500,
  eSave_Move_move_y_attribute    	: 1501,
  eSave_Move_x_offset	       		: 1502,
  eSave_Move_y_offset	        	: 1503,
  eSave_Move_x_factor	       		: 1504,
  eSave_Move_scale_x_attribute     	: 1505,
  eSave_Move_scale_y_attribute    	: 1506,
  eSave_Move_scale_x_factor      	: 1507,
  eSave_Move_scale_type      		: 1508,
  eSave_Move_y_factor	       		: 1509,
  eSave_Move_scale_y_factor      	: 1510,
  eSave_AnalogShift_attribute     	: 1700,
  eSave_DigShift_attribute		: 1800,
  eSave_Animation_attribute       	: 1900,
  eSave_Animation_sequence		: 1901,
  eSave_Bar_attribute	   		: 2200,
  eSave_Bar_minvalue_attr	   	: 2201,
  eSave_Bar_maxvalue_attr	   	: 2202,
  eSave_Trend_attribute1	   	: 2300,
  eSave_Trend_attribute2	   	: 2301,
  eSave_Trend_minvalue_attr1     	: 2302,
  eSave_Trend_maxvalue_attr1     	: 2303,
  eSave_Trend_minvalue_attr2      	: 2304,
  eSave_Trend_maxvalue_attr2      	: 2305,
  eSave_Trend_hold_attr	      	: 2306,
  eSave_Trend_timerange_attr	      	: 2307,
  eSave_Trend_mark1_attr	      	: 2308,
  eSave_Trend_mark2_attr	      	: 2309,
  eSave_Trend_mark1_color	      	: 2310,
  eSave_Trend_mark2_color	      	: 2311,
  eSave_DigFlash_attribute		: 2600,
  eSave_DigFlash_color		: 2601,
  eSave_DigFlash_color2		: 2602,
  eSave_FillLevel_attribute       	: 2700,
  eSave_FillLevel_color		: 2701,
  eSave_FillLevel_direction       	: 2702,
  eSave_FillLevel_min_value       	: 2703,
  eSave_FillLevel_max_value       	: 2704,
  eSave_FillLevel_minvalue_attr    	: 2705,
  eSave_FillLevel_maxvalue_attr    	: 2706,
  eSave_FastCurve_fast_object   	: 2800,
  eSave_FastCurve_curve_index1   	: 2801,
  eSave_FastCurve_curve_index2   	: 2802,
  eSave_AnalogText_super   		: 2900,
  eSave_Table_attribute1	   	: 3000,
  eSave_Table_format1	   		: 3001,
  eSave_Table_sel_attribute1      	: 3002,
  eSave_Table_attribute2	   	: 3003,
  eSave_Table_format2	   		: 3004,
  eSave_Table_sel_attribute2      	: 3005,
  eSave_Table_attribute3	   	: 3006,
  eSave_Table_format3	   		: 3007,
  eSave_Table_sel_attribute3      	: 3008,
  eSave_Table_attribute4	   	: 3009,
  eSave_Table_format4	   		: 3010,
  eSave_Table_sel_attribute4      	: 3011,
  eSave_Table_attribute5	   	: 3012,
  eSave_Table_format5	   		: 3013,
  eSave_Table_sel_attribute5      	: 3014,
  eSave_Table_attribute6	   	: 3015,
  eSave_Table_format6	   		: 3016,
  eSave_Table_sel_attribute6      	: 3017,
  eSave_Table_attribute7	   	: 3018,
  eSave_Table_format7	   		: 3019,
  eSave_Table_sel_attribute7      	: 3020,
  eSave_Table_attribute8	   	: 3021,
  eSave_Table_format8	   		: 3022,
  eSave_Table_sel_attribute8      	: 3023,
  eSave_Table_attribute9	   	: 3024,
  eSave_Table_format9	   		: 3025,
  eSave_Table_sel_attribute9      	: 3026,
  eSave_Table_attribute10	   	: 3027,
  eSave_Table_format10	   	: 3028,
  eSave_Table_sel_attribute10     	: 3029,
  eSave_Table_attribute11	   	: 3030,
  eSave_Table_format11	   	: 3031,
  eSave_Table_sel_attribute11     	: 3032,
  eSave_Table_attribute12	   	: 3033,
  eSave_Table_format12	   	: 3034,
  eSave_Table_sel_attribute12     	: 3035,
  eSave_StatusColor_attribute     	: 3100,
  eSave_StatusColor_nostatus_color 	: 3101,
  eSave_StatusColor_use_colortheme 	: 3102,
  eSave_HostObject_object     	: 3200,
  eSave_DigSound_attribute     	: 3300,
  eSave_DigSound_soundobject     	: 3301,
  eSave_DigSound_level     		: 3302,
  eSave_DigSound_interval     	: 3303,
  eSave_DigSound_instance		: 3304,
  eSave_DigSound_instance_mask    	: 3305,
  eSave_XY_Curve_x_attr      		: 3400,
  eSave_XY_Curve_y_attr      		: 3401,
  eSave_XY_Curve_y_min_value 		: 3402,
  eSave_XY_Curve_y_max_value 		: 3403,
  eSave_XY_Curve_x_min_value 		: 3404,
  eSave_XY_Curve_x_max_value 		: 3405,
  eSave_XY_Curve_y_minvalue_attr 	: 3406,
  eSave_XY_Curve_y_maxvalue_attr 	: 3407,
  eSave_XY_Curve_x_minvalue_attr 	: 3408,
  eSave_XY_Curve_x_maxvalue_attr 	: 3409,
  eSave_XY_Curve_noofpoints 		: 3410,
  eSave_XY_Curve_noofpoints_attr 	: 3411,
  eSave_XY_Curve_update_attr 		: 3412,
  eSave_XY_Curve_datatype 		: 3413,
  eSave_XY_Curve_instance		: 3414,
  eSave_XY_Curve_instance_mask    	: 3415,
  eSave_XY_Curve_curve_color    	: 3416,
  eSave_XY_Curve_fill_color    	: 3417,
  eSave_XY_Curve_horizontal_padding  	: 3418,
  eSave_XY_Curve_x_mark1_attr    	: 3419,
  eSave_XY_Curve_x_mark2_attr    	: 3420,
  eSave_XY_Curve_y_mark1_attr    	: 3421,
  eSave_XY_Curve_y_mark2_attr    	: 3422,
  eSave_XY_Curve_mark1_color    	: 3423,
  eSave_XY_Curve_mark2_color    	: 3424,
  eSave_XY_Curve_hold_attr	    	: 3425,
  eSave_DigCommand_attribute      	: 3500,
  eSave_DigCommand_command      	: 3501,
  eSave_DigCommand_instance        	: 3502,
  eSave_DigCommand_instance_mask 	: 3503,
  eSave_DigCommand_level 		: 3504,
  eSave_Pie_attribute1      		: 3600,
  eSave_Pie_attribute2      		: 3601,
  eSave_Pie_attribute3      		: 3602,
  eSave_Pie_attribute4      		: 3603,
  eSave_Pie_attribute5      		: 3604,
  eSave_Pie_attribute6      		: 3605,
  eSave_Pie_attribute7      		: 3606,
  eSave_Pie_attribute8      		: 3607,
  eSave_Pie_attribute9      		: 3608,
  eSave_Pie_attribute10      		: 3609,
  eSave_Pie_attribute11      		: 3610,
  eSave_Pie_attribute12      		: 3611,
  eSave_Pie_fix_range      		: 3612,
  eSave_BarChart_attribute1      	: 3700,
  eSave_BarChart_attribute2      	: 3701,
  eSave_BarChart_attribute3      	: 3702,
  eSave_BarChart_attribute4      	: 3703,
  eSave_BarChart_attribute5      	: 3704,
  eSave_BarChart_attribute6      	: 3705,
  eSave_BarChart_attribute7      	: 3706,
  eSave_BarChart_attribute8      	: 3707,
  eSave_BarChart_attribute9      	: 3708,
  eSave_BarChart_attribute10      	: 3709,
  eSave_BarChart_attribute11      	: 3710,
  eSave_BarChart_attribute12      	: 3711,
  eSave_BarChart_fix_range      	: 3712,
  eSave_Axis_minvalue_attr      	: 3800,
  eSave_Axis_maxvalue_attr      	: 3801,
  eSave_Axis_keep_settings      	: 3802,
  eSave_DigTextColor_attribute        : 3900,
  eSave_DigTextColor_color		: 3901,
  eSave_TimeoutColor_time             : 4000,
  eSave_TimeoutColor_color		: 4001,
  eSave_DigFourShift_attribute1      	: 4100,
  eSave_DigFourShift_attribute2      	: 4101,
  eSave_DigFourShift_attribute3      	: 4102,
  eSave_ScrollingText_attribute      	: 4200,
  eSave_ScrollingText_direction      	: 4201,
  eSave_ScrollingText_speed      	: 4202,
  eSave_ScrollingText_bounce      	: 4203,
  eSave_DigBackgroundColor_attribute  : 4400,
  eSave_DigBackgroundColor_color      : 4401,
  eSave_DigBackgroundColor_instance   : 4402,
  eSave_DigBackgroundColor_instance_mask : 4403,
  eSave_DigSwap_attribute  		: 4500,
  eSave_DigSwap_reset_value  		: 4501,
  eSave_DigScript_attribute  		: 4600,
  eSave_DigScript_script  		: 4601,
  eSave_DigScript_script_len         	: 4602,
  eSave_DigScript_level  		: 4603,
  eSave_RefUpdate_attribute 		: 4700,
  eSave_RefUpdate_whole_graph 		: 4701,
  eSave_DsTrend_dstrend_object1 	: 4800,
  eSave_DsTrend_dstrend_object2 	: 4801,
  eSave_DsTrend_mark1_attr 		: 4802,
  eSave_DsTrend_mark2_attr 		: 4803,
  eSave_DsTrend_mark1_color 		: 4804,
  eSave_DsTrend_mark2_color 		: 4805,
  eSave_DsTrend_hold_attr 		: 4806,
  eSave_DsTrend_minvalue_attr1 		: 4807,
  eSave_DsTrend_maxvalue_attr1 		: 4808,
  eSave_DsTrend_minvalue_attr2 		: 4809,
  eSave_DsTrend_maxvalue_attr2 		: 4810,
  eSave_DsTrendCurve_dstrend_object 	: 4900,
  eSave_DsTrendCurve_mark1_attr 	: 4902,
  eSave_DsTrendCurve_mark2_attr 	: 4903,
  eSave_DsTrendCurve_mark1_color 	: 4904,
  eSave_DsTrendCurve_mark2_color 	: 4905,
  eSave_DsTrendCurve_hold_attr 		: 4906,
  eSave_DsTrendCurve_minvalue_attr1 	: 4907,
  eSave_DsTrendCurve_maxvalue_attr1 	: 4908,
  eSave_DsTrendCurve_minvalue_attr2 	: 4909,
  eSave_DsTrendCurve_maxvalue_attr2 	: 4910,
  eSave_PopupMenu_ref_object      	: 5000,
  eSave_SetDig_attribute		: 5100,
  eSave_SetDig_instance		: 5101,
  eSave_SetDig_instance_mask      	: 5102,
  eSave_ResetDig_attribute		: 5200,
  eSave_ResetDig_instance		: 5201,
  eSave_ResetDig_instance_mask    	: 5202,
  eSave_ToggleDig_attribute       	: 5300,
  eSave_StoDig_attribute		: 5400,
  eSave_Command_command		: 5500,
  eSave_CommandDC_command		: 5600,
  eSave_Confirm_text	       		: 5700,
  eSave_Confirm_on_set	       	: 5701,
  eSave_Confirm_on_reset	       	: 5702,
  eSave_IncrAnalog_attribute      	: 5800,
  eSave_IncrAnalog_increment      	: 5801,
  eSave_IncrAnalog_min_value      	: 5802,
  eSave_IncrAnalog_max_value      	: 5803,
  eSave_RadioButton_attribute     	: 5900,
  eSave_Slider_attribute	       	: 6000,
  eSave_Slider_minvalue_attr     	: 6001,
  eSave_Slider_maxvalue_attr     	: 6002,
  eSave_Slider_insensitive_attr    	: 6003,
  eSave_Slider_release_attr    	: 6004,
  eSave_AnalogColor_attribute      	: 6100,
  eSave_AnalogColor_limit        	: 6101,
  eSave_AnalogColor_limit_type     	: 6102,
  eSave_AnalogColor_color       	: 6103,
  eSave_AnalogColor_instance      	: 6104,
  eSave_AnalogColor_instance_mask 	: 6105,
  eSave_AnalogColor_border	 	: 6106,
  eSave_AnalogColor_common_attr	 	: 6107,
  eSave_TipText_text              	: 6200,
  eSave_Help_topic        		: 6300,
  eSave_Help_bookmark             	: 6400,
  eSave_OpenGraph_graph_object    	: 6401,
  eSave_OpenURL_url               	: 6500,
  eSave_InputFocus_initial_focus	: 6600,
  eSave_InputFocus_next_horizontal	: 6601,
  eSave_InputFocus_next_vertical	: 6602,
  eSave_InputFocus_next_tab		: 6603,
  eSave_PulldownMenu_button_mask	: 6800,
  eSave_PulldownMenu_items_text0	: 6801,
  eSave_PulldownMenu_items_text1	: 6802,
  eSave_PulldownMenu_items_text2	: 6803,
  eSave_PulldownMenu_items_text3	: 6804,
  eSave_PulldownMenu_items_text4	: 6805,
  eSave_PulldownMenu_items_text5	: 6806,
  eSave_PulldownMenu_items_text6	: 6807,
  eSave_PulldownMenu_items_text7	: 6808,
  eSave_PulldownMenu_items_text8	: 6809,
  eSave_PulldownMenu_items_text9	: 6810,
  eSave_PulldownMenu_items_text10	: 6811,
  eSave_PulldownMenu_items_text11	: 6812,
  eSave_PulldownMenu_items_text12	: 6813,
  eSave_PulldownMenu_items_text13	: 6814,
  eSave_PulldownMenu_items_text14	: 6815,
  eSave_PulldownMenu_items_text15	: 6816,
  eSave_PulldownMenu_items_text16	: 6817,
  eSave_PulldownMenu_items_text17	: 6818,
  eSave_PulldownMenu_items_text18	: 6819,
  eSave_PulldownMenu_items_text19	: 6820,
  eSave_PulldownMenu_items_text20	: 6821,
  eSave_PulldownMenu_items_text21	: 6822,
  eSave_PulldownMenu_items_text22	: 6823,
  eSave_PulldownMenu_items_text23	: 6824,
  eSave_PulldownMenu_items_text24	: 6825,
  eSave_PulldownMenu_items_text25	: 6826,
  eSave_PulldownMenu_items_text26	: 6827,
  eSave_PulldownMenu_items_text27	: 6828,
  eSave_PulldownMenu_items_text28	: 6829,
  eSave_PulldownMenu_items_text29	: 6830,
  eSave_PulldownMenu_items_text30	: 6831,
  eSave_PulldownMenu_items_text31	: 6832,
  eSave_PulldownMenu_items_dyn0	: 6833,
  eSave_PulldownMenu_items_dyn1	: 6834,
  eSave_PulldownMenu_items_dyn2	: 6835,
  eSave_PulldownMenu_items_dyn3	: 6836,
  eSave_PulldownMenu_items_dyn4	: 6837,
  eSave_PulldownMenu_items_dyn5	: 6838,
  eSave_PulldownMenu_items_dyn6	: 6839,
  eSave_PulldownMenu_items_dyn7	: 6840,
  eSave_PulldownMenu_items_dyn8	: 6841,
  eSave_PulldownMenu_items_dyn9	: 6842,
  eSave_PulldownMenu_items_dyn10	: 6843,
  eSave_PulldownMenu_items_dyn11	: 6844,
  eSave_PulldownMenu_items_dyn12	: 6845,
  eSave_PulldownMenu_items_dyn13	: 6846,
  eSave_PulldownMenu_items_dyn14	: 6847,
  eSave_PulldownMenu_items_dyn15	: 6848,
  eSave_PulldownMenu_items_dyn16	: 6849,
  eSave_PulldownMenu_items_dyn17	: 6850,
  eSave_PulldownMenu_items_dyn18	: 6851,
  eSave_PulldownMenu_items_dyn19	: 6852,
  eSave_PulldownMenu_items_dyn20	: 6853,
  eSave_PulldownMenu_items_dyn21	: 6854,
  eSave_PulldownMenu_items_dyn22	: 6855,
  eSave_PulldownMenu_items_dyn23	: 6856,
  eSave_PulldownMenu_items_dyn24	: 6857,
  eSave_PulldownMenu_items_dyn25	: 6858,
  eSave_PulldownMenu_items_dyn26	: 6859,
  eSave_PulldownMenu_items_dyn27	: 6860,
  eSave_PulldownMenu_items_dyn28	: 6861,
  eSave_PulldownMenu_items_dyn29	: 6862,
  eSave_PulldownMenu_items_dyn30	: 6863,
  eSave_PulldownMenu_items_dyn31	: 6864,
  eSave_OptionMenu_button_mask	: 6900,
  eSave_OptionMenu_items_text0	: 6901,
  eSave_OptionMenu_items_text1	: 6902,
  eSave_OptionMenu_items_text2	: 6903,
  eSave_OptionMenu_items_text3	: 6904,
  eSave_OptionMenu_items_text4	: 6905,
  eSave_OptionMenu_items_text5	: 6906,
  eSave_OptionMenu_items_text6	: 6907,
  eSave_OptionMenu_items_text7	: 6908,
  eSave_OptionMenu_items_text8	: 6909,
  eSave_OptionMenu_items_text9	: 6910,
  eSave_OptionMenu_items_text10	: 6911,
  eSave_OptionMenu_items_text11	: 6912,
  eSave_OptionMenu_items_text12	: 6913,
  eSave_OptionMenu_items_text13	: 6914,
  eSave_OptionMenu_items_text14	: 6915,
  eSave_OptionMenu_items_text15	: 6916,
  eSave_OptionMenu_items_text16	: 6917,
  eSave_OptionMenu_items_text17	: 6918,
  eSave_OptionMenu_items_text18	: 6919,
  eSave_OptionMenu_items_text19	: 6920,
  eSave_OptionMenu_items_text20	: 6921,
  eSave_OptionMenu_items_text21	: 6922,
  eSave_OptionMenu_items_text22	: 6923,
  eSave_OptionMenu_items_text23	: 6924,
  eSave_OptionMenu_items_text24	: 6925,
  eSave_OptionMenu_items_text25	: 6926,
  eSave_OptionMenu_items_text26	: 6927,
  eSave_OptionMenu_items_text27	: 6928,
  eSave_OptionMenu_items_text28	: 6929,
  eSave_OptionMenu_items_text29	: 6930,
  eSave_OptionMenu_items_text30	: 6931,
  eSave_OptionMenu_items_text31	: 6932,
  eSave_OptionMenu_items_enum0	: 6933,
  eSave_OptionMenu_items_enum1	: 6934,
  eSave_OptionMenu_items_enum2	: 6935,
  eSave_OptionMenu_items_enum3	: 6936,
  eSave_OptionMenu_items_enum4	: 6937,
  eSave_OptionMenu_items_enum5	: 6938,
  eSave_OptionMenu_items_enum6	: 6939,
  eSave_OptionMenu_items_enum7	: 6940,
  eSave_OptionMenu_items_enum8	: 6941,
  eSave_OptionMenu_items_enum9	: 6942,
  eSave_OptionMenu_items_enum10	: 6943,
  eSave_OptionMenu_items_enum11	: 6944,
  eSave_OptionMenu_items_enum12	: 6945,
  eSave_OptionMenu_items_enum13	: 6946,
  eSave_OptionMenu_items_enum14	: 6947,
  eSave_OptionMenu_items_enum15	: 6948,
  eSave_OptionMenu_items_enum16	: 6949,
  eSave_OptionMenu_items_enum17	: 6950,
  eSave_OptionMenu_items_enum18	: 6951,
  eSave_OptionMenu_items_enum19	: 6952,
  eSave_OptionMenu_items_enum20	: 6953,
  eSave_OptionMenu_items_enum21	: 6954,
  eSave_OptionMenu_items_enum22	: 6955,
  eSave_OptionMenu_items_enum23	: 6956,
  eSave_OptionMenu_items_enum24	: 6957,
  eSave_OptionMenu_items_enum25	: 6958,
  eSave_OptionMenu_items_enum26	: 6959,
  eSave_OptionMenu_items_enum27	: 6960,
  eSave_OptionMenu_items_enum28	: 6961,
  eSave_OptionMenu_items_enum29	: 6962,
  eSave_OptionMenu_items_enum30	: 6963,
  eSave_OptionMenu_items_enum31	: 6964,
  eSave_OptionMenu_attribute		: 6965,
  eSave_OptionMenu_text_attribute	: 6966,
  eSave_OptionMenu_size_attribute	: 6967,
  eSave_OptionMenu_update_attribute 	: 6968,
  eSave_OptionMenu_optionmenu_type 	: 6969,
  eSave_SetValue_attribute		: 7000,
  eSave_SetValue_value		: 7001,
  eSave_SetValue_instance		: 7002,
  eSave_SetValue_instance_mask     	: 7003,
  eSave_MethodToolbar_method_object   : 7100,
  eSave_MethodToolbar_toolbar_type    : 7101,
  eSave_MethodPulldownMenu_method_object : 7200,
  eSave_MethodPulldownMenu_menu_type  : 7201,
  eSave_Script_script_len		: 7300,
  eSave_Script_script			: 7301,
  eSave_CatchSignal_signal_name   	: 7400,
  eSave_EmitSignal_signal_name   	: 7500,
  eSave_EmitSignal_global   		: 7501,
  eSave_SevHist_sevhist_object1 	: 7600,
  eSave_SevHist_sevhist_object2 	: 7601,
  eSave_SevHist_attribute1 		: 7602,
  eSave_SevHist_attribute2 		: 7603,
  eSave_SevHist_server 			: 7604,
  eSave_SevHist_mark1_attr 		: 7605,
  eSave_SevHist_mark2_attr 		: 7606,
  eSave_SevHist_mark1_color 		: 7607,
  eSave_SevHist_mark2_color 		: 7608,
  eSave_SevHist_hold_attr 		: 7609,
  eSave_SevHist_minvalue_attr1 		: 7610,
  eSave_SevHist_maxvalue_attr1 		: 7611,
  eSave_SevHist_minvalue_attr2 		: 7612,
  eSave_SevHist_maxvalue_attr2 		: 7613,
  eSave_SevHist_timerange 		: 7614,
  eSave_SevHist_timerange_attr 		: 7615,
  eSave_SevHist_update_attr 		: 7616,
  eSave_SevHist_updatetime 		: 7617,

  eAnimSequence_Inherit      	: 0,
  eAnimSequence_Cycle		: 1,
  eAnimSequence_Dig		: 2,
  eAnimSequence_ForwBack	: 3,

  eLimitType_Gt		: 0,
  eLimitType_Lt		: 1,
  eLimitType_Ge		: 2,
  eLimitType_Le		: 3,
  eLimitType_Eq		: 4,

  ePwrStatus_No		: 0,
  ePwrStatus_Success		: 1,
  ePwrStatus_Warning		: 2,
  ePwrStatus_Error		: 3,
  ePwrStatus_Fatal		: 4,

  eMethodsMenuType_Object	: 0,
  eMethodsMenuType_Help	: 1,
  eMethodsMenuType_Simulate	: 2,

  eMethodToolbarType_Object	: 0,
  eMethodToolbarType_Simulate	: 1,

  method_toolbar_op_cnt 	: 12,
  method_toolbar_op_helpmask 	: 1 << 8,
  method_toolbar_op_subgraph 	: [ "pwr_mb2opengraph",
				    "pwr_mb2openobjectgraph",
				    "pwr_mb2trend",
				    "pwr_mb2history",
				    "pwr_mb2fast",
				    "pwr_mb2camera",
				    "pwr_mb2histevent",
				    "pwr_mb2blockevents",
				    "pwr_mb2help",
				    "pwr_mb2photo",
				    "pwr_mb2note",
				    "pwr_mb2parentgraph"],
  method_toolbar_mnt_cnt 	: 32,
  method_toolbar_mnt_helpmask 	: 1 << 4,
  method_toolbar_mnt_subgraph 	: [ "pwr_mb2openobject",
				    "pwr_mb2openplc",
				    "pwr_mb2rtnavigator",
				    "pwr_mb2crossreferences",
				    "pwr_mb2helpclass",
				    "pwr_mb2datasheet",
				    "pwr_mb2circuitdiagram",
				    "", "","","","","","","","","","","","","","","","","","","","","","","",
				    "pwr_mb2simulate"
				    ],

  method_toolbar_op_methods 	: [ "Graph", 
				    "Object Graph",
				    "Trend",
				    "History",
				    "Fast", 
				    "Camera",
				    "Event Log...",
				    "Block Events...", 
				    "Help",
				    "Photo", 
				    "Note",
				    "Parent Object Graph"],
  
  method_toolbar_mnt_methods 	: [ "Open Object",
				    "Open Plc",
				    "RtNavigator", 
				    "Crossreferences", 
				    "Help Class", 
				    "DataSheet", 
				    "CircuitDiagram", 
				    "","","","","","","","","","","","","","","","","","","","","","","","",
				    "Simulate"],

  method_toolbar_op_tooltip 	: [ "Graph", 
				    "Object graph",
				    "Trend", 
				    "History", 
				    "Fast curve", 
				    "Camera", 
				    "Event log", 
				    "Block events", 
				    "Help", 
				    "Photo", 
				    "Note",
				    "Open parent object graph"   
				    ],

  method_toolbar_mnt_tooltip 	: [ "Open object",
				    "Open plc",
				    "Navigator", 
				    "CrossReferences", 
				    "Help class", 
				    "Datasheet",
				    "CircuitDiagram", 
				    "", "","","","","","","","","","","","","","","","","","","","","","","",
				    "Simulate"]
};

function DynParsedAttrName() {
  this.name;
  this.tname;
  this.inverted;
  this.elements;
  this.index;
  this.type;
  this.database;
  this.bitmask;
}

function Dyn( graph) {

  this.debug = false;
  this.elements = [];
  this.graph = graph;
  this.object = null;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.access = 0;
  this.cycle = 0;
  this.total_dyn_type1 = 0;
  this.total_dyn_type2 = 0;
  this.total_action_type1 = 0;
  this.total_action_type2 = 0;

  this.repaintNow = false;
  this.ignoreColor = false;
  this.resetColor = false;
  this.ignoreBgColor = false;
  this.resetBgColor = false;

  this.getColor1 = function( object, color) {
    if ( color == Glow.eDrawType_Inherit) {
      return object.getClassTraceColor1();
    }
    return color;
  };

  this.getColor2 = function( object, color) {
    if ( color == Glow.eDrawType_Inherit) {
      return object.getClassTraceColor2();
    }
    return color;
  };


  this.merge = function( x) {
    var elem, xelem;
    var e;

    this.dyn_type1 = this.dyn_type1 | x.dyn_type1;
    this.total_dyn_type1 = this.total_dyn_type1 | x.total_dyn_type1;
    this.action_type1 = this.action_type1 | x.action_type1;
    this.total_action_type1 = this.total_action_type1 | x.total_action_type1;

    for ( var i = 0; i < x.elements.length; i++) {
      xelem = x.elements[i];

      for ( var j = 0; j < this.elements.length; j++) {
	elem = this.elements[j];
	if ( elem.dyn_type1 == xelem.dyn_type1 && 
	     elem.dyn_type2 == xelem.dyn_type2 && 		     
	     elem.action_type1 == xelem.action_type1 &&
	     elem.action_type2 == xelem.action_type2) {
	  // Element exists in both, use element in x, i.e. remove current element
	  if ( typeof elem.instance == 'undefined' || 
	       (typeof elem.instance != 'undefined' &&
		elem.instance == xelem.instance)) {
	    this.elements.splice(j, 1);
	    break;
	  }
	}
      }
      // Insert copy of x element
      e = this.copy_element( xelem);
      if ( e != null) {
	e.dyn = this;
	this.elements.push( e);
      }
    }
  };

  this.copy_element = function( x) {
    var e = {};
    for ( var attr in x) {
      if ( x.hasOwnProperty( attr)) {
	e[attr] = x[attr];
      }	
    }
    return e;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.debug) console.log( "Dyn : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Dyn: 
	break;
      case DynC.eSave_Dyn_dyn_type1: 
	this.dyn_type1 = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Dyn_dyn_type2: 
	this.dyn_type2 = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Dyn_action_type1: 
	this.action_type1 = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Dyn_action_type2: 
	this.action_type2 = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Dyn_access: 
	this.access = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Dyn_cycle: 
	this.cycle = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigLowColor: 
	elem = new DynDigLowColor(this); 
	break;
      case DynC.eSave_DigColor: 
	elem = new DynDigColor(this); 
	break;
      case DynC.eSave_DigWarning: 
	elem = new DynDigWarning(this); 
	break;
      case DynC.eSave_DigError: 
	elem = new DynDigError(this); 
	break;
      case DynC.eSave_DigFlash: 
	elem = new DynDigFlash(this); 
	break;
      case DynC.eSave_Invisible: 
	elem = new DynInvisible(this); 
	break;
      case DynC.eSave_DigBorder: 
	elem = new DynDigBorder(this); 
	break;
      case DynC.eSave_DigText: 
	elem = new DynDigText(this); 
	break;
      case DynC.eSave_Value: 
	elem = new DynValue(this); 
	break;
      case DynC.eSave_ValueInput: 
	elem = new DynValueInput(this); 
	break;
      case DynC.eSave_AnalogColor: 
	elem = new DynAnalogColor(this); 
	break;
      case DynC.eSave_Rotate: 
	elem = new DynRotate(this); 
	break;
      case DynC.eSave_Move: 
	elem = new DynMove(this); 
	break;
      case DynC.eSave_AnalogShift: 
	elem = new DynAnalogShift(this); 
	break;
      case DynC.eSave_DigShift: 
	elem = new DynDigShift(this); 
	break;
      case DynC.eSave_DigFourShift: 
	elem = new DynDigFourShift(this); 
	break;
      case DynC.eSave_ScrollingText: 
	elem = new DynScrollingText(this); 
	break;
      case DynC.eSave_ColorThemeLightness: 
	elem = new DynColorThemeLightness(this); 
	break;
      case DynC.eSave_DigBackgroundColor: 
	elem = new DynDigBackgroundColor(this); 
	break;
      case DynC.eSave_DigSwap: 
	elem = new DynDigSwap(this); 
	break;
      case DynC.eSave_DigScript: 
	elem = new DynDigScript(this); 
	break;
      case DynC.eSave_Animation: 
	elem = new DynAnimation(this); 
	break;
      case DynC.eSave_Video: 
	elem = new DynVideo(this); 
	break;
      case DynC.eSave_Bar: 
	elem = new DynBar(this); 
	break;
      case DynC.eSave_Trend: 
	elem = new DynTrend(this); 
	break;
      case DynC.eSave_DsTrend: 
	elem = new DynDsTrend(this); 
	break;
      case DynC.eSave_DsTrendCurve: 
	elem = new DynDsTrendCurve(this); 
	break;
      case DynC.eSave_SevHist: 
	elem = new DynSevHist(this); 
	break;
      case DynC.eSave_FillLevel: 
	elem = new DynFillLevel(this); 
	break;
      case DynC.eSave_FastCurve: 
	elem = new DynFastCurve(this); 
	break;
      case DynC.eSave_AnalogText: 
	elem = new DynAnalogText(this); 
	break;
      case DynC.eSave_Table: 
	elem = new DynTable(this); 
	break;
      case DynC.eSave_StatusColor: 
	elem = new DynStatusColor(this); 
	break;
      case DynC.eSave_Pie: 
	elem = new DynPie(this); 
	break;
      case DynC.eSave_BarChart: 
	elem = new DynBarChart(this); 
	break;
      case DynC.eSave_Axis: 
	elem = new DynAxis(this); 
	break;
      case DynC.eSave_DigTextColor: 
	elem = new DynDigTextColor(this); 
	break;
      case DynC.eSave_TimeoutColor: 
	elem = new DynTimeoutColor(this); 
	break;
      case DynC.eSave_HostObject: 
	elem = new DynHostObject(this); 
	break;
      case DynC.eSave_DigSound: 
	elem = new DynDigSound(this); 
	break;
      case DynC.eSave_XY_Curve: 
	elem = new DynXY_Curve(this); 
	break;
      case DynC.eSave_DigCommand: 
	elem = new DynDigCommand(this); 
	break;
      case DynC.eSave_PopupMenu: 
	elem = new DynPopupMenu(this); 
	break;
      case DynC.eSave_SetDig: 
	elem = new DynSetDig(this); 
	break;
      case DynC.eSave_ResetDig: 
	elem = new DynResetDig(this); 
	break;
      case DynC.eSave_ToggleDig: 
	elem = new DynToggleDig(this); 
	break;
      case DynC.eSave_StoDig: 
	elem = new DynStoDig(this); 
	break;
      case DynC.eSave_Command: 
	elem = new DynCommand(this); 
	break;
      case DynC.eSave_CommandDC: 
	elem = new DynCommandDoubleClick(this); 
	break;
      case DynC.eSave_Confirm: 
	elem = new DynConfirm(this); 
	break;
      case DynC.eSave_IncrAnalog: 
	elem = new DynIncrAnalog(this); 
	break;
      case DynC.eSave_RadioButton: 
	elem = new DynRadioButton(this); 
	break;
      case DynC.eSave_Slider: 
	elem = new DynSlider(this); 
	break;
      case DynC.eSave_TipText: 
	elem = new DynTipText(this); 
	break;
      case DynC.eSave_Help: 
	elem = new DynHelp(this); 
	break;
      case DynC.eSave_OpenGraph:
	elem = new DynOpenGraph(this); 
	break;
      case DynC.eSave_OpenURL: 
	elem = new DynOpenURL(this); 
	break;
      case DynC.eSave_InputFocus: 
	elem = new DynInputFocus(this); 
	break;
      case DynC.eSave_CloseGraph: 
	elem = new DynCloseGraph(this); 
	break;
      case DynC.eSave_PulldownMenu: 
	elem = new DynPulldownMenu(this); 
	break;
      case DynC.eSave_OptionMenu: 
	elem = new DynOptionMenu(this); 
	break;
      case DynC.eSave_SetValue: 
	elem = new DynSetValue(this); 
	break;
      case DynC.eSave_MethodToolbar: 
	elem = new DynMethodToolbar(this); 
	break;
      case DynC.eSave_MethodPulldownMenu: 
	elem = new DynMethodPulldownMenu(this); 
	break;
      case DynC.eSave_Script: 
	elem = new DynScript(this); 
	break;
      case DynC.eSave_CatchSignal: 
	elem = new DynCatchSignal(this); 
	break;
      case DynC.eSave_EmitSignal: 
	elem = new DynEmitSignal(this); 
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in Dyn");
	break;
      }
      if ( elem !== null) {
	this.elements.push(elem);
	i = elem.open( lines, i+1);
      }
      
      if ( end)
	break;
    }
    return i;
  };

  this.connect = function( object) {

    if ( (this.dyn_type1 & DynC.mDynType1_Inherit) !== 0) {
      var inherit_dyn_type1 = object.getClassDynType1();
      var inherit_dyn_type2 = object.getClassDynType2();
      this.total_dyn_type1 = this.dyn_type1 | inherit_dyn_type1;
      this.total_dyn_type2 = this.dyn_type2 | inherit_dyn_type2;
    }
    else {
      this.total_dyn_type1 = this.dyn_type1;
      this.total_dyn_type2 = this.dyn_type2;
    }
    
    if ( (this.action_type1 & DynC.mActionType1_Inherit) !== 0) {
      var inherit_action_type1 = object.getClassActionType1();
      var inherit_action_type2 = object.getClassActionType2();
      this.total_action_type1 = this.action_type1 | inherit_action_type1;
      this.total_action_type2 = this.action_type2 | inherit_action_type2;
    }
    else {
      this.total_action_type1 = this.action_type1;
      this.total_action_type2 = this.action_type2;
    }
    
    if ( this.cycle == Glow.eCycle_Inherit) {
      this.cycle = object.getClassCycle();
      if ( this.cycle == Glow.eCycle_Inherit)
	this.cycle = Glow.eCycle_Slow;
    }    
    for ( var i = 0; i < this.elements.length; i++) {
      this.elements[i].connect(object);
    }
    return 1;
  };


  this.disconnect = function() {
    for ( var i = 0; i < this.elements.length; i++) {
      this.elements[i].disconnect();
    }
  };

  this.scan = function( object) {
    this.repaintNow = false;
    this.ignoreColor = false;
    this.resetColor = false;
    this.ignoreBgColor = false;
    this.resetBgColor = false;
    
    for ( var i = 0; i < this.elements.length; i++) {
      this.elements[i].scan(object);
    }	
    if ( this.repaintNow)      
      object.draw();
  };

  this.action = function( object, event) {
    var sts;
    for ( var i = 0; i < this.elements.length; i++) {
      sts = this.elements[i].action(object, event);
      if ( sts == DynC.DYN__NO_PROPAGATE || sts == Glow.GLOW__TERMINATED ||
	   sts == Glow.GLOW__SUBTERMINATED)
	return sts;
    }
    return 1;
  };
  
  this.setTotal = function( object) {
    if ( object == null) {
      this.total_dyn_type1 = this.dyn_type1;
      this.total_dyn_type2 = this.dyn_type2;
      this.total_action_type1 = this.action_type1;
      this.total_action_type2 = this.action_type2;
    }
    else {
      if ( (this.dyn_type1 & DynC.mDynType1_Inherit) != 0) {
	var inherit_dyn_type1 = object.getClassDynType1();
	var inherit_dyn_type2 = object.getClassDynType2();
	this.total_dyn_type1 = this.dyn_type1 | inherit_dyn_type1;
	this.total_dyn_type2 = this.dyn_type2 | inherit_dyn_type2;
      }
      else {
	this.total_dyn_type1 = this.dyn_type1;
	this.total_dyn_type2 = this.dyn_type2;
      }
	    
      if ( (this.action_type1 & DynC.mActionType1_Inherit) != 0) {
	var inherit_action_type1 = object.getClassActionType1();
	var inherit_action_type2 = object.getClassActionType2();
	this.total_action_type1 = this.action_type1 | inherit_action_type1;
	this.total_action_type2 = this.action_type2 | inherit_action_type2;
      }
      else {
	this.total_action_type1 = this.action_type1;
	this.total_action_type2 = this.action_type2;
      }
    }
  };

  this.parseAttrName = function( name) {
    if ( name === null)
      return null;
    if ( (this.total_dyn_type1 & DynC.mDynType1_HostObject) !== 0) {
      var idx = name.indexOf("$hostobject");
      if ( idx != -1) {
	var hostobject = this.getHostObject();
	var n = name.substring(0, idx) + hostobject + name.substring(idx+11);
	return this.graph.parseAttrName( n);
      }	    
    }
    
    return this.graph.parseAttrName( name);
  };

  this.getHostObject = function() {
    for ( var i = 0; i < this.elements.length; i++) {
      if ( (this.elements[i].dyn_type1 & DynC.mDynType1_HostObject) !== 0)
	return this.elements[i].hostobject;
    }
    return "";
  };

  this.getDig = function( p, a_typeid, bitmask, db) {
    switch ( a_typeid) {
    case Pwr.eType_Boolean: {
      var bvalue = false;
      switch ( db) {
      case GraphIfc.eDatabase_Gdh:
	bvalue = this.graph.getGdh().getObjectRefInfo( p);
	break;
      case GraphIfc.eDatabase_Local:
	bvalue = this.graph.getLdb().getObjectRefInfo( p);
	break;
      default: break;
      }	
      return bvalue;
    }
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Int64:
    case Pwr.eType_UInt64: {
      var ivalue = 0;
      switch ( db) {
      case GraphIfc.eDatabase_Gdh:
	ivalue = this.graph.getGdh().getObjectRefInfo( p);
	break;
      case GraphIfc.eDatabase_Local:
	ivalue = this.graph.getLdb().getObjectRefInfo( p);
	break;
      default: break;
      }	

      return ivalue !== 0;
    }
    case GraphIfc.eType_Bit: {
      var tvalue = 0;
      switch ( db) {
      case GraphIfc.eDatabase_Gdh:
	tvalue = this.graph.getGdh().getObjectRefInfo( p);
	break;
      case GraphIfc.eDatabase_Local:
	tvalue = this.graph.getLdb().getObjectRefInfo( p);
	break;
      default: break;
      }
      return (tvalue & bitmask) !== 0;
    }
    case Pwr.eType_Float32:
    case Pwr.eType_Float64: {
      var fvalue = 0;
      switch ( db) {
      case GraphIfc.eDatabase_Gdh:
	fvalue = this.graph.getGdh().getObjectRefInfo( p);
	break;
      case GraphIfc.eDatabase_Local:
	fvalue = this.graph.getLdb().getObjectRefInfo( p);
	break;
      default: break;
      }	
      return fvalue > Number.MIN_VALUE;
    }
    case Pwr.eType_String: {
      switch ( db) {
      case GraphIfc.eDatabase_Gdh: {
	var svalue = graph.getGdh().getObjectRefInfo( p);
	return !(svalue === "");
      }	
      default: break;
      }
      return false;
    }
    default:
      return false;
    }
  };

  this.confirmedAction = function( event, object) {
    if ( this.elements == null)
      return;

    var e = new GlowEvent();
    e.event = event;	
    e.object = object;
    this.action_type1 &= ~DynC.mActionType1_Confirm;
    for ( var i = 0; i < this.elements.length; i++)
      this.elements[i].action( object,  e);
    this.action_type1 |= DynC.mActionType1_Confirm;
  };    

  this.instance_to_number = function( instance) {
    var inst = 1;
    var m = instance;
    while( m > 1) {
      m = m >> 1;
      inst++;
    }
    return inst;
  };

  this.valueInputAction = function( object, str) {
    if ( this.elements === null)
      return DynC.eValueInput_Error;

    var e = object;

    var ctx_popped = e.dyn.graph.ctxPop( object.dyn.object.ctx);	

    var sts = null;
    var minval = 0;
    var maxval = 0;
    if ( e.minvalue_attr !== null) {

      var pname = e.dyn.parseAttrName( e.minvalue_attr);
      if ( pname !== null && pname.type == Pwr.eType_Float32) {
	/** TODO
	CdhrFloat ret = e.dyn.graph.getGdh().getObjectInfoFloat( pname.name);
	if ( ret.evenSts()) {
	  if ( ctx_popped)
	    e.dyn.graph.ctxPush();
	  return DynC.eValueInput_Error;
	}
	minval = ret.value;
        **/
      }
      else 
        minval = e.min_value;
    }
    else
      minval = e.min_value;

    if ( e.maxvalue_attr !== null) {
      var pname = e.dyn.parseAttrName( e.maxvalue_attr);
      if ( pname === null || pname.name === "") 
	maxval = e.max_value;
      else {
	if ( pname.type == Pwr.eType_Float32) {
	  /** TODO
	  CdhrFloat ret = e.dyn.graph.getGdh().getObjectInfoFloat( pname.name);
	  if ( ret.evenSts()) {
	    if ( ctx_popped)
	      e.dyn.graph.ctxPush();
	    return DynC.eValueInput_Error;
	  }
	  maxval = ret.value;
	  **/
        }
	else
	  maxval = e.max_value;
      }
    }
    else
      maxval = e.max_value;

    switch ( e.a_typeid) {
    case Pwr.eType_Float32: {
      var inputValue = parseFloat( str.trim());
      if ( minval !== 0 && maxval !== 0 && inputValue < minval) {
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_MinValueExceeded;
      }
      if ( minval !== 0 && maxval !== 0 && inputValue > maxval) {
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_MaxValueExceeded;
      }
		
      var pname = e.dyn.parseAttrName(e.value_element.attribute);
      if ( pname === null || pname.name === "")
	break;
		    
      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = e.dyn.graph.getGdh().setObjectInfoFloat( pname.name, inputValue);
	break;
      case GraphIfc.eDatabase_Local:
	sts = e.dyn.graph.getLdb().setObjectInfo( graph, pname.name, inputValue);
	break;
      default:
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      if ( sts.evenSts()) {
	console.log( "setObjectInfoError " + sts);
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      break;
    }
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Int16:
    case Pwr.eType_UInt16:
    case Pwr.eType_Int8:
    case Pwr.eType_UInt8: {
      var inputValue = parseInt( str.trim(), 10);
      if ( minval !== 0 && maxval !== 0 && inputValue < minval) {
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_MinValueExceeded;
      }
      if ( minval !== 0 && maxval !== 0 && inputValue > maxval ) {
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_MaxValueExceeded;
      }
      
      var pname = e.dyn.parseAttrName(e.value_element.attribute);
      if ( pname === null || pname.name === "")
	break;
      
      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = e.dyn.graph.getGdh().setObjectInfoInt( pname.name, inputValue);
	break;
      case GraphIfc.eDatabase_Local:
	sts = e.dyn.graph.getLdb().setObjectInfo( graph, pname.name, inputValue);
	break;
      default:
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      if ( sts.evenSts()) {
	console.log( "setObjectInfoError " + sts);
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }		
      break;
    }
    case Pwr.eType_Boolean: {
      var inputValueInt = parseInt( str.trim(), 10);
      var inputValue;
      if ( inputValueInt === 0)
	inputValue = false;
      else if ( inputValueInt == 1)
	inputValue = true;
      else
	break;

      // valueElement.oldValueB = inputValue;

      var pname = e.dyn.parseAttrName(e.value_element.attribute);
      if ( pname === null || pname.name === "")
	break;
		    
      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = e.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, inputValue);
	break;
      case GraphIfc.eDatabase_Local:
	sts = e.dyn.graph.getLdb().setObjectInfo( graph, pname.name, inputValue);
	break;
      default:
	return DynC.eValueInput_Error;
      }
      if ( sts.evenSts()) {
	console.log( "setObjectInfoError " + sts);
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      break;
    }
    case Pwr.eType_String: {
      // valueElement.oldValueS = str;

      var pname = e.dyn.parseAttrName(e.value_element.attribute);
      if ( pname === null || pname.name === "")
	break;
		    
      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = e.dyn.graph.getGdh().setObjectInfoString( pname.name, str);
	break;
      case GraphIfc.eDatabase_Local:
	sts = e.dyn.graph.getLdb().setObjectInfo( graph, pname.name, str);
	break;
      default:
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      if ( sts.evenSts()) {
	console.log( "setObjectInfoError " + sts);
	if ( ctx_popped)
	  e.dyn.graph.ctxPush();
	return DynC.eValueInput_Error;
      }
      break;
    }
    }
    if ( ctx_popped)
      e.dyn.graph.ctxPush();

    return DynC.eValueInput_Success;
  };

  this.value_to_msg = function( value) {
    var str;
    if ( value === 0)
      str = "";
    else {
      switch ( value & 7) {
      case 3:
      case 1:
	str = "MSG-SUCCESS, ";
	break;
      case 0:
	str = "MSG-WARNING, ";
	break;
      case 2:
	str = "MSG-ERROR, ";
	break;
      case 4:
	str = "MSG-FATAL, ";
	break;
      default:
	str = "MSG-NO, ";
      }
      switch ( value) {
      case 134512649: str += "Success"; break;
      case 134512659: str += "Proview startup pu"; break;
      case 134512665: str += "Proview running"; break;
      case 134512731: str += "Server running"; break;
      case 134512795: str += "Application running"; break;
      case 134512820: str += "Process timeout"; break;
      default:
	str += "Message number " + value;
      }
    }
    return str;
  };
}

function DynReference( dyn, attribute) {
  this.name = attribute;
  this.pname;
  this.p = 0;
  this.database;
  this.inverted;
  this.attrFound = false;
  this.oldValue;
  this.bittmask;
  this.elements;
  this.typeid;
  this.sts = false;

  this.connect = function(dyn) {
    if ( !this.sts)
      return;
    this.sts = false;
    switch( this.pname.database) {
    case GraphIfc.eDatabase_Gdh:
      this.p = dyn.graph.getGdh().refObjectInfo( this.pname.tname, this.pname.type, 
						 this.pname.elements);
      break;
    case GraphIfc.eDatabase_Local:
      this.p = dyn.graph.getLdb().refObjectInfo( dyn.graph, this.pname.name);
      break;
    default:
      this.p = 0;
      break;
    }

    if ( this.p === 0)
      return;

    this.inverted = this.pname.inverted;
    this.typeid = this.pname.type;
    this.bitmask = this.pname.bitmask;
    this.database = this.pname.database;
    this.elements = this.pname.elements;
    this.pname = null;
    this.attrFound = true;
    this.sts = true;
  };

  this.disconnect = function(dyn) {
    if ( this.sts && this.database == GraphIfc.eDatabase_Gdh) {
      dyn.graph.getGdh().unrefObjectInfo( this.p);
      this.sts = false;
    }
  };

  this.get_ref_value = function(dyn) {
    var value = null;
    switch ( this.database) {
    case GraphIfc.eDatabase_Gdh:
      value = dyn.graph.getGdh().getObjectRefInfo( this.p);
      break;
    case GraphIfc.eDatabase_Local:
      value = dyn.graph.getLdb().getObjectRefInfo( this.p);
      break;
    }
    return value;
  };

  this.pname = dyn.parseAttrName( this.name);
  if ( this.pname === null || this.pname.name === "") 
    return;
  this.sts = true;
}

function DynDigLowColor(dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigLowColor;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigLowColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.attribute;
  this.color;
  this.firstScan = true;
	
  this.connect = function( o) {
    var object = o;
    this.color = this.dyn.getColor1(object, this.color);
    if ( this.color < 0 || this.color >= Glow.eDrawType__)
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigLowColor: " + this.attribute);
      return 1;
    }

    return 1;
  };

  this.disconnect = function() {
    if ( this.a !== null)
      this.a.disconnect(this.dyn);
  };

	
  this.scan = function( o) {
    var object = o;
    if ( this.a === null || !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor)
	return;
    }
    else
      this.firstScan = false;
	
    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( !value) {
	if ( this.color >= Glow.eDrawTone__)
	  object.setFillColor( this.color);
	else
	  object.setColorTone( this.color);
      }
      else {
	if ( this.color >= Glow.eDrawTone__)
	  object.resetFillColor();
	object.resetColorTone();
      }
      this.dyn.repaintNow = true;
    }
    else {
      if ( !value)
	object.setFillColor( this.color);
      else 
	object.resetFillColor();		
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigLowColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigLowColor: 
	break;
      case DynC.eSave_DigLowColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigLowColor_color: 
	this.color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
        console.log( "Syntax error in DynDigLowColor");
	break;
      }

      if ( end)
	break;
    }

    return i;		
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigColor( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigColor;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.attribute;
  this.color;
  this.firstScan = true;

  this.connect = function( object) {
    this.color = this.dyn.getColor2(object, this.color);
    if ( this.color < 0 || this.color >= Glow.eDrawType__)
      return 0;
    
    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigColor: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    if ( this.a !== null)
      this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( this.a === null || !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( value)
	  this.dyn.ignoreColor = true;
	return;
      }
    }
    else
      this.firstScan = false;
	
    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( value) {
	if ( this.color >= Glow.eDrawTone__) {
	  if ( this.dyn.resetColor)
	    object.resetFillColor();
	  object.setFillColor(this.color);
	}
	else
	  object.setColorTone( this.color);
	this.dyn.ignoreColor = true;
      }
      else {
	if ( this.color >= Glow.eDrawTone__)
	  object.resetFillColor();
	object.resetColorTone();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    else {
      if ( value) {
	object.setFillColor( this.color);
	this.dyn.ignoreColor = true;
      }
      else {
	object.resetFillColor();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigColor: 
	break;
      case DynC.eSave_DigColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigColor_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigColor_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigColor_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigColor");
	break;
      }
      
      if ( end)
	break;
    }		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigWarning( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigWarning;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigWarning;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.use_colortheme;
  this.firstScan = true;
					 
  
  this.connect = function( object) {
    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigWarning: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( value)
	  this.dyn.ignoreColor = true;
	return;
      }
    }
    else
      this.firstScan = false;
	
    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( value) {
	object.setColorTone( Glow.eDrawTone_Yellow);
	this.dyn.ignoreColor = true;
      }
      else {
	object.resetFillColor();
	object.resetColorTone();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    else {
      if (  value) {
	object.setFillColor( (this.use_colortheme !== 0) ? Glow.eDrawType_CustomColor41 : Glow.eDrawType_ColorYellow);
	this.dyn.ignoreColor = true;
      }
      else {
	object.resetFillColor();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigWarning : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigWarning: 
	break;
      case DynC.eSave_DigWarning_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigWarning_use_colortheme:
	this.use_colortheme = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigWarning");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigError( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigError;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigError;
  this.instance_mask = 0;
  this.instance = 0;

  
  this.attribute;
  this.use_colortheme;
  this.firstScan = true;

  this.connect = function( object) {
    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigError: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( value)
	  this.dyn.ignoreColor = true;
	return;
      }
    }
    else
      this.firstScan = false;
	
    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( value) {
	object.setColorTone( Glow.eDrawTone_Red);
	this.dyn.ignoreColor = true;
      }
      else {
	object.resetFillColor();
	object.resetColorTone();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    else {
      if (  value) {
	object.setFillColor( (this.use_colortheme !== 0) ? Glow.eDrawType_CustomColor11 : Glow.eDrawType_ColorRed);
	this.dyn.ignoreColor = true;
      }
      else {
	object.resetFillColor();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigError : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigError: 
	break;
      case DynC.eSave_DigError_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigError_use_colortheme:
	this.use_colortheme = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigError");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigFlash( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigFlash;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigFlash;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.color;
  this.color2;
  this.firstScan = true;
  this.on = false;

  this.connect = function( object) {
    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigFlash: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( !value)
	  return;
      }
    }
    else
      this.firstScan = false;
	
    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( value) {
	if ( this.on) {
	  if ( this.color >= Glow.eDrawTone__)
	    object.setColorTone(this.color);
	  else
	    object.setFillColor(this.color);
	  this.dyn.ignoreColor = true;
	}
	else {
	  object.resetFillColor();
	  object.resetColorTone();
	  this.dyn.resetColor = true;
	}
	this.on = !this.on;
      }
      else {
	object.resetFillColor();
	object.resetColorTone();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    else {
      if (  value) {
	if ( this.on) {
	  object.setFillColor(this.color);
	  this.dyn.ignoreColor = true;
	}
	else {
	  if ( this.color2 == Glow.eDrawType_Inherit) {
	    object.resetFillColor();
	    this.dyn.resetColor = true;
	  }
	  else {
	    object.setFillColor(this.color2);
	    this.dyn.ignoreColor = true;
	  }
	}
	this.on = !this.on;
      }
      else {
	object.resetFillColor();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigError : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigFlash: 
	break;
      case DynC.eSave_DigFlash_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigFlash_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigFlash_color2: 
	this.color2 = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigFlash");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynInvisible( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Invisible;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Invisible;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.dimmed;
  this.firstScan = true;
  this.cmd;
  this.command;

  this.connect = function( object) {

    if ( this.attribute !== null && this.attribute.toLowerCase().substring(0,5) == "$cmd(") {
      this.cmd = true;
      var idx = this.attribute.lastIndexOf(')');
      if ( idx != -1 && idx > 5)
	this.command = this.attribute.substring(5, idx);
      else
	this.command = this.attribute.substring(5);
      this.command = this.dyn.graph.getCommand(command);
    }
    else {
      this.a = new DynReference( this.dyn, this.attribute);
      this.a.connect(this.dyn);
      if ( !this.a.sts) {
	console.log("DigFlash: " + this.attribute);
	return 1;
      }
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

  this.scan = function( o) {

    if ( this.cmd) {
      if ( this.firstScan) {
	var sts = this.dyn.graph.command( this.command);
	console.log( "DynInvisible: " + this.command + ", value: " + sts);
	if ( sts === 0) {
	  if ( this.dimmed === 0)
	    o.setVisibility( Glow.eVis_Invisible);
	  else
	    o.setVisibility( Glow.eVis_Dimmed);
	}
	this.firstScan = false;
      }
      return;
    }

    if ( !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);


    if ( this.a.typeid == Pwr.eType_String)
      value = !value;

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( !value)
	  return;
      }
    }
    else
      this.firstScan = false;

    if ( value) {
      if ( this.dimmed === 0)
	o.setVisibility( Glow.eVis_Invisible);
      else
	o.setVisibility( Glow.eVis_Dimmed);
    }
    else {
      o.setVisibility( Glow.eVis_Visible);
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  };
	
  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynInvisible : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Invisible: 
	break;
      case DynC.eSave_Invisible_attribute: 
	if ( tokens.length > 1)
	  this.attribute = lines[i].substring(4);
	break;
      case DynC.eSave_Invisible_dimmed: 
	this.dimmed = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Invisible_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Invisible_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynInvisible");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}
 
function DynDigTextColor( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DigTextColor;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigTextColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.attribute;
  this.color;
  this.firstScan = true;
	
  this.connect = function( object) {
    this.color = this.dyn.getColor1(object, this.color);
    if ( this.color < 0 || this.color >= Glow.eDrawType__)
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigTextColor: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    if ( this.a !== null)
      this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( this.a ===  null || !this.a.sts || this.dyn.ignoreColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor)
	return;
    }
    else
      this.firstScan = false;
	
    if ( value)
      object.setTextColor( this.color);
    else 
      object.resetTextColor();		
    this.dyn.repaintNow = true;

    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DigTextColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigTextColor: 
	break;
      case DynC.eSave_DigTextColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigTextColor_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigTextColor");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigText( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigText;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigText;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.low_text;
  this.high_text;
  this.firstScan = true;

  this.connect = function( object) {

    if ( this.low_text === null)
      return 1;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigText: " + attribute);
      return 1;
    }
    this.high_text = object.getAnnotation(1);
    if ( this.high_text === null)
      this.high_text = "";
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };
	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value)
	return;
    }
    else
      this.firstScan = false;
	
    if ( value)
      object.setAnnotation(1, this.high_text);
    else
      object.setAnnotation(1, this.low_text);
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DigText : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigText: 
	break;
      case DynC.eSave_DigText_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigText_low_text: 
	if ( tokens.length > 1)
	  this.low_text = lines[i].substring(5);
	break;
      case DynC.eSave_DigText_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigText_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigText");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}
 
function DynDigBorder( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigBorder;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigBorder;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.color;
  this.firstScan = true;

  this.connect = function( object) {
    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigBorder: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value) {
	// No change since last time
	return;
      }
    }
    else
      this.firstScan = false;
	
    if ( value) {
      object.setBorderColor(this.color);
    }
    else {
      object.resetBorderColor();
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynBorder : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigBorder: 
	break;
      case DynC.eSave_DigBorder_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigBorder_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigBorder");
	break;
      }

      if ( end)
	break;
    }
    return  i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynValue( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Value;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Value;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.format = null;
  this.zero_blank;
  this.decimals_attr;
  this.decimals_decr;
  this.firstScan = true;
  this.oldValueB;
  this.oldValueF;
  this.oldValueI;
  this.oldValueS;
  this.cFormat =  null;

  this.read_decimals = function( dyn, attr, decr, format) {
    var 	decimals;

    var pname = dyn.parseAttrName( attr);
    if ( pname === null || pname.name === "") 
      return format;
	    
    switch ( pname.type) {
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Enum:
    case Pwr.eType_Mask:
      break;
    default:
      return format;
    }

    // TODO
    return format; // TODO
    var ret = this.dyn.graph.getGdh().getObjectInfoInt( pname.name);
    if ( ret.evenSts()) return format;

    decimals = ret.value - decr;
    if ( decimals < 0)
      decimals = 0;
    if ( decimals >= 10)
      return format;

    if ( format === null)
      return "%." + decimals + "f";

    // Print format, of replace digit between . and f
    var s = format.indexOf( 'f');
    if ( s == -1)
      return "%." + decimals + "f";
    else {
      if ( s < 2 || format.charAt(s-2) != '.')
	return "%." + decimals + "f";
      else
	return format.substring( 0, s-1) + decimals + format.substring(s);
    }
  };

  this.connect = function( object) {
    if ( this.format === null && this.decimals_attr === null)
      return 1;

    if ( typeof this.decimals_attr != 'undefined' && this.decimals_attr !== null && !(this.decimals_attr === "")) {
      this.format = this.read_decimals( this.dyn, this.decimals_attr, this.decimals_decr, this.format);
      console.log( "read_decimals " + this.format);
    }
    if ( this.format === null)
      return 1;
    this.cFormat = new GlowCFormat(this.format);

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("Value: " + attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    if ( !this.a.sts)
      return;

    var annot_num = this.dyn.instance_to_number( this.instance);

    switch ( this.a.typeid) {
    case Pwr.eType_Float32: {
      var value0 = this.a.get_ref_value(this.dyn);
      if ( typeof value0 == 'undefined')
	return;

      if ( value0 != this.oldValueF  || this.firstScan) {
	if ( this.cFormat !== null) {
	  var sb = this.cFormat.format( value0);
	  object.setAnnotation( annot_num, sb);
	}
	this.dyn.repaintNow = true;
	this.oldValueF = value0;
      }
      break;
    }
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Int16:
    case Pwr.eType_UInt16:
    case Pwr.eType_Int8:
    case Pwr.eType_UInt8: {
      var value0 = this.a.get_ref_value(this.dyn);
      if ( typeof value0 == 'undefined')
	return;

      if ( value0 != this.oldValueI || this.firstScan) {
	if ( this.cFormat !== null) {
	  var sb = this.cFormat.format( value0);
	  object.setAnnotation( annot_num, sb);
	}
	this.dyn.repaintNow = true;
	this.oldValueI = value0;
      }
      break;
    }
    case Pwr.eType_Boolean: {
      var value0 = this.a.get_ref_value(this.dyn);
      if ( typeof value0 == 'undefined')
	return;

      if ( value0 != this.oldValueB || this.firstScan) {
	if ( this.cFormat !== null) {
	  var sb = this.cFormat.format( value0);
	  object.setAnnotation( annot_num, sb);
	}
	this.dyn.repaintNow = true;
	this.oldValueB = value0;
      }
      break;
    }
    case Pwr.eType_String:
    case Pwr.eType_Objid:
    case Pwr.eType_Time:
    case Pwr.eType_DeltaTime: {
      var value0 = this.a.get_ref_value(this.dyn);
      if ( typeof value0 == 'undefined')
	return;

      if ( this.firstScan || !(value0 == this.oldValueS)) {
	if ( this.cFormat !== null) {
	  if ( this.a_typeid == Pwr.eType_String) {
	    var sb = this.cFormat.format( value0);
	    object.setAnnotation( annot_num, sb);
	  }
	  else
	    // TODO time format
	    object.setAnnotation( annot_num, value0);
	}
	this.dyn.repaintNow = true;
	this.oldValueS = value0;
      }
      break;
    }
    case Pwr.eType_Status:
    case Pwr.eType_NetStatus: {
      var value0 = this.a.get_ref_value(this.dyn);
      if ( typeof value0 == 'undefined')
	return;

      if ( value0 != this.oldValueI || this.firstScan) {
	if ( value0 === 0) {
	  object.setAnnotation( annot_num, "");
	  this.dyn.repaintNow = true;
	}
	else {
	  if ( this.a.database == GraphIfc.eDatabase_Gdh) {
	    var data = new Array(2);
	    data[0] = this;
	    data[1] = object;
	    this.dyn.graph.getGdh().getMsg( value0, this.scan2, data);
	  }
	}
      }
      /** TODO
	switch ( this.cFormat.type()) {
	case GlowCFormat.FRM_M: {
	  var cstr = this.dyn.graph.getGdh().getMsg( value0);
	  object.setAnnotation( annot_num, cstr.str);
	  this.dyn.repaintNow = true;
	  break;
	}
	case GlowCFormat.FRM_1M: {
	  var cstr = this.dyn.graph.getGdh().getMsgText( value0);
	  object.setAnnotation( annot_num, cstr.str);
	  this.dyn.repaintNow = true;
	  break;
	}
	}
      **/
      this.oldValueI = value0;
      break;
    }
    }
    if ( this.firstScan)
      this.firstScan = false;
  };

  this.scan2 = function( id, data, sts, value) {
    if ( sts & 1 != 0) {
      var self = data[0];
      var object = data[1];
      var annot_num = self.dyn.instance_to_number( self.instance);

      object.setAnnotation( annot_num, value);
      self.dyn.repaintNow = true;
    }
    else {
      object.setAnnotation( annot_num, "Unknown message");
      this.dyn.repaintNow = true;
    }
  }

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynBorder : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Value: 
	break;
      case DynC.eSave_Value_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_Value_format: 
	if ( tokens.length > 1)
	  this.format = tokens[1];
	break;
      case DynC.eSave_Value_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Value_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Value_zero_blank: 
	this.zero_blank = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Value_decimals_attr: 
	if ( tokens.length > 1)
	  this.decimals_attr = tokens[1];
	break;
      case DynC.eSave_Value_decimals_decr: 
	this.decimals_decr = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynValue");
	break;
      }

      if ( end)
	break;
    }
		
    if ( this.format !== null)
      this.cFormat = new GlowCFormat(this.format);

    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynValueInput( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_ValueInput;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_ValueInput;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;
  this.min_value;
  this.max_value;
  this.clear;
  this.popup;
  this.unselect;
  this.minvalue_attr;
  this.maxvalue_attr;
  this.escape_store;
  this.keyboard_type;
  this.update_open;
  this.value_element = null;
  this.a_typeid;
				       
  this.connect = function( object) {
    // Get the Value element
    this.value_element = null;
    for ( var j = 0; j < this.dyn.elements.length; j++) {
      if ( this.dyn.elements[j].dyn_type1 == DynC.mDynType1_Value) {
	this.value_element = this.dyn.elements[j];
	this.a_typeid = this.value_element.a.typeid;
	break;
      }
    }
	    
    return 1;
  };

  this.disconnect = function() {
  };

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      this.dyn.object = object;
      this.dyn.graph.openValueInputDialog( this.dyn, "Enter value", this);
      break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynValueInput : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_ValueInput: 
	break;
      case DynC.eSave_ValueInput_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_ValueInput_format: 
	break;
      case DynC.eSave_ValueInput_unselect: 
	this.unselect = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ValueInput_min_value: 
	this.min_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_ValueInput_max_value: 
	this.max_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_ValueInput_clear: 
	this.clear = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ValueInput_popup: 
	this.popup = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ValueInput_minvalue_attr: 
	if ( tokens.length > 1)
	  this.minvalue_attr = tokens[1];
	break;
      case DynC.eSave_ValueInput_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_ValueInput_escape_store: 
	this.escape_store = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ValueInput_keyboard_type: 
	this.keyboard_type = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ValueInput_update_open: 
	this.update_open = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynValueInput");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.scan = function( o) {
    return 1;
  };
}

function DynAnalogColor( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_AnalogColor;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_AnalogColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a  = null;
  this.attribute;
  this.limit;
  this.limit_type;
  this.color;
  this.border;
  this.common_attr;
  this.firstScan = true;
  this.oldValueF;
  this.oldValueI;
  this.oldState;
  this.isMainInstance = false;

  this.connect = function( object) {
    // Find the main instance
    var mainInstance = null;
    for ( var i = this.dyn.elements.length - 1; i >= 0; i--) {
      if ( this.dyn.elements[i].dyn_type1 == DynC.mDynType1_AnalogColor) {
	mainInstance = this.dyn.elements[i];
	break;
      }
    }

    if ( mainInstance !== null) {
      if ( mainInstance.a == null || !mainInstance.a.attrFound) {
	mainInstance.a = new DynReference( this.dyn, mainInstance.attribute);
	mainInstance.a.connect(this.dyn);
	if ( !mainInstance.a.sts)
	  return 1;
	mainInstance.isMainInstance = true;
      }
      if ( !this.isMainInstance) {
	this.a = new DynReference( this.dyn, mainInstance.attribute);
	this.a.p = mainInstance.a.p;
	this.a.attrFound = mainInstance.a.attrFound;
	this.a.typeid = mainInstance.a.typeid;
	this.a.database = mainInstance.a.database;
	this.a.sts = mainInstance.a.sts;
      }
    }
    return 1;
  };


  this.disconnect = function() {
    if ( this.isMainInstance)
      this.a.disconnect(this.dyn);
  };
	
  this.scan = function( object) {
    if ( !this.a.sts || this.dyn.ignoreColor)
      return;

    var state = false;
    var set_color = false;
    var reset_color = false;

    if ( this.a.typeid == Pwr.eType_Float32) {
      var value = this.a.get_ref_value(this.dyn);
      var i;

      if ( !this.firstScan) {
	if ( !this.dyn.resetColor && value == this.oldValueF) {
	  if ( this.oldState)
	    this.dyn.ignoreColor = true;
	  return;
	}
      }
      else
	this.firstScan = false;

      switch ( this.limit_type) {
      case DynC.eLimitType_Gt:
	state = (value > this.limit);
	break;
      case DynC.eLimitType_Lt:
	state = (value < this.limit);
	break;
      case DynC.eLimitType_Ge:
	state = (value >= this.limit);
	break;
      case DynC.eLimitType_Le:
	state = (value >= this.limit);
	break;
      case DynC.eLimitType_Eq:
	state = (value == this.limit);
	break;      
      }
      this.oldValueF = value;
    }
    else if ( this.a.typeid == Pwr.eType_Int32 || this.a.typeid == Pwr.eType_UInt32) {
      var value = this.dyn.graph.getGdh().getObjectRefInfo( this.a.p);
      var i;
      if ( !this.firstScan) {
	if ( !this.dyn.resetColor && value == this.oldValueI) {
	  if ( this.oldState)
	    this.dyn.ignoreColor = true;
	  return;
	}
      }
      else
	this.firstScan = false;
  
      switch ( this.limit_type) {
      case DynC.eLimitType_Gt:
	state = (value > this.limit);
	break;
      case DynC.eLimitType_Lt:
	state = (value < this.limit);
	break;
      case DynC.eLimitType_Ge:
	state = (value >= this.limit);
	break;
      case DynC.eLimitType_Le:
	state = (value >= this.limit);
	break;
      case DynC.eLimitType_Eq:
	state = (value == this.limit);
	break;      
      }
      this.oldValueI = value;
    }

    if ( state != this.oldState || this.dyn.resetColor || this.firstScan) {
      if ( state) {
	set_color = true;
	this.dyn.ignoreColor = true;
      }
      else {
	reset_color = true;
	this.dyn.resetColor = true;
      }
      this.oldState = state;
    }
    else if ( state)
      this.dyn.ignoreColor = true;


    if ( !set_color && !reset_color)
      return;

    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      if ( set_color) {
	if ( color >= Glow.eDrawTone__) {
	  if ( this.border === 0)
	    object.setFillColor( this.color);
	  else
	    object.setBorderColor( this.color);
	}
	else
	  object.setColorTone( this.color);
	this.dyn.ignoreColor = true;
      }
      else {
	if ( this.color >= Glow.eDrawTone__) {
	  if ( this.border === 0)
	    object.resetFillColor();
	  else
	    object.resetBorderColor();
	}
	object.resetColorTone();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    else {
      if ( set_color) {
	if ( this.border === 0)
	  object.setFillColor( this.color);
	else
	  object.setBorderColor( this.color);
	this.dyn.ignoreColor = true;
      }
      else {
	if ( this.border === 0)
	  object.resetFillColor();
	else
	  object.resetBorderColor();
	this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynAnalogColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_AnalogColor: 
	break;
      case DynC.eSave_AnalogColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_AnalogColor_limit: 
	this.limit = parseFloat( tokens[1]);
	break;
      case DynC.eSave_AnalogColor_limit_type: 
	this.limit_type = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_AnalogColor_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_AnalogColor_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_AnalogColor_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_AnalogColor_border: 
	this.border = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_AnalogColor_common_attr: 
	this.common_attr = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynAnalogColor");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };

}

function DynRotate( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Rotate;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Rotate;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.x0;
  this.y0;
  this.factor;
  this.firstScan = true;
  this.rotation_point;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("Rotate: " + this.attribute);
      return 1;
    }

    if ( this.x0 !== 0 || this.y0 !== 0)
      this.rotation_point = Glow.eRotationPoint_FixPoint;
    else
      this.rotation_point = Glow.eRotationPoint_Zero;
	    
    object.storeTransform();
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };
	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value0 = this.a.get_ref_value(this.dyn);
    if ( value0 != this.a.oldValue  || this.firstScan) {
      object.setRotation(value0 * this.factor, this.x0, this.y0, this.rotation_point);

      this.dyn.repaintNow = true;
      this.a.oldValue = value0;
    }
    if ( this.firstScan)
      this.firstScan = false;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynRotate : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Rotate: 
	break;
      case DynC.eSave_Rotate_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_Rotate_x0: 
	this.x0 = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Rotate_y0: 
	this.y0 = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Rotate_factor: 
	this.factor = parseFloat( tokens[1]);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynRotate");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}


function DynMove( dyn){
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Move;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Move;
  this.instance_mask = 0;
  this.instance = 0;

  this.move_x_a;
  this.move_y_a;
  this.scale_x_a;
  this.scale_y_a;
  this.move_x_attribute;
  this.move_y_attribute;
  this.scale_x_attribute;
  this.scale_y_attribute;
  this.x_factor;
  this.y_factor;
  this.x_offset;
  this.y_offset;
  this.scale_x_factor;
  this.scale_y_factor;
  this.scale_type;
  this.firstScan = true;
  this.x_orig;
  this.y_orig;
  this.width_orig;
  this.height_orig;
	
  this.connect = function( object) {

    this.move_x_a = new DynReference( this.dyn, this.move_x_attribute);
    this.move_x_a.connect(this.dyn);
    if ( this.move_x_a.sts) {
      switch ( this.move_x_a.typeid) {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32:
	break;
      default:
	this.move_x_a.disconnect(dyn);
	console.log("Move: " + this.move_x_attribute);
	return 1;
      }
    }

    this.move_y_a = new DynReference( this.dyn, this.move_y_attribute);
    this.move_y_a.connect(this.dyn);
    if ( this.move_y_a.sts) {
      switch ( this.move_y_a.typeid) {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32:
	break;
      default:
	this.move_y_a.disconnect(dyn);
	console.log("Move: " + this.move_y_attribute);
	return 1;
      }
    }

    this.scale_x_a = new DynReference( this.dyn, this.scale_x_attribute);
    this.scale_x_a.connect(this.dyn);
    if ( this.scale_x_a.sts) {
      switch ( this.scale_x_a.typeid) {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32:
	break;
      default:
	this.scale_x_a.disconnect(dyn);
	console.log("Move: " + this.scale_x_attribute);
	return 1;
      }
    }

    this.scale_y_a = new DynReference( this.dyn, this.scale_y_attribute);
    this.scale_y_a.connect(this.dyn);
    if ( this.scale_y_a.sts) {
      switch ( this.scale_y_a.typeid) {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32:
	break;
      default:
	this.scale_y_a.disconnect(dyn);
	console.log("Move: " + this.scale_y_attribute);
	return 1;
      }
    }

    if ( !object.transformIsStored()) {
      object.storeTransform();
      var geom = object.measure();
      this.x_orig = geom.ll_x;
      this.y_orig = geom.ll_y;
      this.width_orig = geom.ur_x - this.x_orig;
      this.height_orig = geom.ur_y - this.y_orig;
    }

    return 1;
  };

  this.disconnect = function() {
    this.move_x_a.disconnect(this.dyn);
    this.move_y_a.disconnect(this.dyn);
    this.scale_x_a.disconnect(this.dyn);
    this.scale_y_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var update = false;
  
    var move_x_value = 0;
    var move_y_value = 0;
    var scale_x_value = 0;
    var scale_y_value = 0;
    if ( this.move_x_a.sts) {
      move_x_value = this.move_x_a.get_ref_value(this.dyn);
    }

    if ( this.move_y_a.sts) {
      move_y_value = this.move_y_a.get_ref_value(this.dyn);
    }

    if ( this.scale_x_a.sts) {
      scale_x_value = this.scale_x_a.get_ref_value(this.dyn);
    }

    if ( this.scale_y_a.sts) {
      scale_y_value = this.scale_y_a.get_ref_value(this.dyn);
    }


    update = false;
    if ( !this.firstScan) {
      if ( this.move_x_a.sts && move_x_value != this.move_x_a.oldValue)
	update = true;
      else if ( this.move_y_a.sts && move_y_value != this.move_y_a.oldValue)
	update = true;
      else if ( this.scale_x_a.sts && scale_x_value != this.scale_x_a.oldValue)
	update = true;
      else if ( this.scale_y_a.sts && scale_y_value != this.scale_y_a.oldValue)
	update = true;
      if ( !update)
	return;

    }
    else
      this.firstScan = false;

    var move_x = 0;
    var move_y = 0;
    var scale_x = 1;
    var scale_y = 1;

    if ( this.scale_x_a.sts)
      scale_x = scale_x_value * this.scale_x_factor;
		
    if ( this.scale_y_a.sts)
      scale_y = scale_y_value * this.scale_y_factor;

      
    if ( !(this.move_x_a.sts || this.move_y_a.sts))
      object.set_scale( scale_x, scale_y, 0, 0, this.scale_type);
    if ( this.scale_x_a.sts)
      this.scale_x_a.oldValue = scale_x_value;
    if ( this.scale_y_a.sts)
      this.scale_y_a.oldValue =  scale_y_value;


    if ( this.move_x_a.sts || this.move_y_a.sts) {
      if ( this.move_x_a.sts) {
	var scale_offs = 0;
	// Adjust position for different scaletypes
	switch ( this.scale_type) {
	case Glow.eScaleType_LowerRight:
	case Glow.eScaleType_UpperRight:
	  scale_offs = this.width_orig * ( 1 - scale_x);
	  break;
	case Glow.eScaleType_Center:
	  scale_offs = this.width_orig * ( 1 - scale_x) / 2;
	  break;
	default: break;
	}

	move_x = this.x_orig + scale_offs + (move_x_value - this.x_offset) * this.x_factor;
      }
      else
	move_x = this.x_orig;
		
      if ( this.move_y_a.sts) {
	var scale_offs = 0;
	// Adjust position for different scaletypes
	switch ( this.scale_type) {
	case Glow.eScaleType_UpperRight:
	case Glow.eScaleType_UpperLeft:
	  scale_offs = this.height_orig * ( 1 - scale_y);
	  break;
	case Glow.eScaleType_Center:
	  scale_offs = this.height_orig * ( 1 - scale_y) / 2;
	  break;
	default: break;
	}
		    
	move_y = this.y_orig + scale_offs + (move_y_value - this.y_offset) * this.y_factor;
      }
      else
	move_y = this.y_orig;
		
      if ( Math.abs(scale_x) < Number.MIN_VALUE)
	scale_x = 10e-5;
      if ( Math.abs(scale_y) < Number.MIN_VALUE)
	scale_y = 10e-5;
		
      object.set_scale_pos( move_x, move_y, 
			    scale_x, scale_y, 0, 0,
			    this.scale_type);
      if ( this.move_x_a.sts)
	this.move_x_a.oldValue = move_x_value;
      if ( this.move_y_a.sts)
	this.move_y_a.oldValue = move_y_value;
    }
    else {
      if ( this.move_x_a.sts)
	move_x = (move_x_value - x_offset) * this.x_factor;
      else
	move_x = 0;
	    
      if ( this.move_y_a.sts) {
	move_y = (move_y_value - y_offset) * this.y_factor;
      }
      else
	move_y = 0;
  
      object.set_position( move_x, move_y);
	    
      if ( this.move_x_a.sts)
	this.move_x_a.oldValue = move_x_value;
      if ( this.move_y_a.sts)
	this.move_y_a.oldValue = move_y_value;
    }
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynMove : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Move: 
	break;
      case DynC.eSave_Move_move_x_attribute: 
	if ( tokens.length > 1)
	  this.move_x_attribute = tokens[1];
	break;
      case DynC.eSave_Move_move_y_attribute: 
	if ( tokens.length > 1)
	  this.move_y_attribute = tokens[1];
	break;
      case DynC.eSave_Move_scale_x_attribute: 
	if ( tokens.length > 1)
	  this.scale_x_attribute = tokens[1];
	break;
      case DynC.eSave_Move_scale_y_attribute: 
	if ( tokens.length > 1)
	  this.scale_y_attribute = tokens[1];
	break;
      case DynC.eSave_Move_x_offset: 
	this.x_offset = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_y_offset: 
	this.y_offset = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_x_factor: 
	this.x_factor = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_y_factor: 
	this.y_factor = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_scale_x_factor: 
	this.scale_x_factor = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_scale_y_factor: 
	this.scale_y_factor = parseFloat( tokens[1]);
	break;
      case DynC.eSave_Move_scale_type: 
	this.scale_type = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynMove");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynAnalogShift( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_AnalogShift;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_AnalogShift;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.firstScan = true;
  this.oldValueF;
  this.oldValueI;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("AnalogShift: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;

    var index = 0;

    switch ( this.a.typeid) {
    case Pwr.eType_Float32: {
      var value = this.a.get_ref_value(this.dyn);
      var i;

      if ( !this.firstScan) {
	if ( value == this.oldValueF) {
	  return;
	}
      }
      else
	this.firstScan = false;

      index = Math.floor( value + 0.5);
      this.oldValueF = value;

      break;
    }
    case Pwr.eType_Int32: {
      var value = this.a.get_ref_value(this.dyn);
      var i;

      if ( !this.firstScan) {
	if ( value == this.oldValueI) {
	  return;
	}
      }
      else
	this.firstScan = false;

      index = value;
      this.oldValueI = value;

      break;
    }
    }
    object.set_nodeclass_by_index(index);	    
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynAnalogShift : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_AnalogShift: 
	break;
      case DynC.eSave_AnalogShift_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynAnalogShift");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigShift( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigShift;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigShift;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.firstScan = true;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigShift: " + attribute);
      return 1;
    }

    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value)
	return;
    }
    else
      this.firstScan = false;
	
    if ( value) {
      object.set_last_nodeclass();
    }
    else {
      object.set_root_nodeclass();
    }
    this.a.oldValue = value;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigShift : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigShift: 
	break;
      case DynC.eSave_DigShift_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigShift");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynDigFourShift( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DigFourShift;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigFourShift;
  this.instance_mask = 0;
  this.instance = 0;

  this.a1;
  this.a2;
  this.a3;
  this.attribute1;
  this.attribute2;
  this.attribute3;
  this.firstScan = true;

  this.connect = function( object) {

    this.a1 = new DynReference( this.dyn, this.attribute1);
    this.a1.connect(this.dyn);
    if ( !this.a1.sts) {
      console.log("DigFourShift: " + this.attribute1);
      return 1;
    }

    this.a2 = new DynReference( this.dyn, this.attribute2);
    this.a2.connect(this.dyn);
    if ( !this.a2.sts) {
      console.log("DigFourShift: " + this.attribute2);
      return 1;
    }

    this.a3 = new DynReference( this.dyn, this.attribute3);
    this.a3.connect(this.dyn);
    if ( !this.a3.sts) {
      console.log("DigFourShift: " + this.attribute3);
      return 1;
    }

    return 1;
  };

  this.disconnect = function() {
    this.a1.disconnect(this.dyn);
    this.a2.disconnect(this.dyn);
    this.a3.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a1.attrFound || !this.a2.attrFound || !this.a3.attrFound)
      return;
    var value1 = this.dyn.getDig(this.a1.p, this.a1.typeid, this.a1.bitmask, this.a1.database);
    var value2 = this.dyn.getDig(this.a2.p, this.a2.typeid, this.a2.bitmask, this.a2.database);
    var value3 = this.dyn.getDig(this.a3.p, this.a3.typeid, this.a3.bitmask, this.a3.database);

    if ( this.a1.inverted)
      value1 = !value1;
    if ( this.a2.inverted)
      value2 = !value2;
    if ( this.a3.inverted)
      value3 = !value3;

    if ( !this.firstScan) {
      if ( this.a1.oldValue == value1 && this.a2.oldValue == value2 && this.a3.oldValue == value3)
	return;
    }
    else
      this.firstScan = false;
	
    if ( value3)
      object.set_nodeclass_by_index(2);	    
    else if ( value2)
      object.set_nodeclass_by_index(1);
    else if ( value1)
      object.set_root_nodeclass();
    else
      object.set_last_nodeclass();
    this.a1.oldValue = value1;
    this.a2.oldValue = value2;
    this.a3.oldValue = value3;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigFourShift : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigFourShift: 
	break;
      case DynC.eSave_DigFourShift_attribute1: 
	if ( tokens.length > 1)
	  this.attribute1 = tokens[1];
	break;
      case DynC.eSave_DigFourShift_attribute2: 
	if ( tokens.length > 1)
	  this.attribute2 = tokens[1];
	break;
      case DynC.eSave_DigFourShift_attribute3: 
	if ( tokens.length > 1)
	  this.attribute3 = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
        console.log( "Syntax error in DynDigFourShift");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynScrollingText( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_ScrollingText;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_ScrollingText;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.direction;
  this.speed;
  this.bounce;
  this.firstScan = true;
  this.offset = 0;
  this.osize;
	
  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("ScrollingText: " + this.attribute);
      return 1;
    }
    if ( !(this.a.typeid == Pwr.eType_String || this.a.typeid == Pwr.eType_Text)) {
      console.log("ScrollingText: " + this.attribute);
      this.a.disconnect(dyn);
      return 1;
    }

    var geom = object.measure();
    switch( this.direction) {
    case Glow.eDirection_Left:
    case Glow.eDirection_Right:
      this.osize = geom.ur_x - geom.ll_x;
      break;
    default:
      this.osize = geom.ur_y - geom.ll_y;
    }

    return 1;
  }

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  }

	
  this.scan = function( object) {

    var d = object.getAnnotationTextExtent( 1);
    if ( d === null)
      return;

    switch ( this.direction) {
    case Glow.eDirection_Left: {
      this.offset -= this.speed * this.dyn.graph.getAnimationScanTime();
      if ( this.bounce !== 0) {
	if ( d.width < this.osize) {
	  if ( this.offset < 0) {
	    this.offset = -this.offset;
	    this.direction = Glow.eDirection_Right;
	  }
	}
	else {
	  if ( this.offset < this.osize - d.width) {
	    this.offset += (this.osize - d.width) - this.offset;
	    this.direction = Glow.eDirection_Right;
	  }
	}
      }
      else {
	if ( this.offset < -d.width)
	  this.offset = this.osize;
      }
      object.setAnnotationTextOffset( 1, this.offset, 0);
      break;
    }
    case Glow.eDirection_Right: {
      this.offset += this.speed * this.dyn.graph.getAnimationScanTime();
      if ( this.bounce != 0) {
	if ( d.width < this.osize) {
	  if ( this.offset > this.osize - d.width) {
	    this.offset -= this.offset - (this.osize -d.width);
	    this.direction = Glow.eDirection_Left;
	  }
	}
	else {
	  if ( this.offset > 0) {
	    this.offset = -this.offset;
	    this.direction = Glow.eDirection_Left;
	  }
	}
      }
      else {
	if ( this.offset > this.osize)
	  this.offset = -d.width;
      }
      object.setAnnotationTextOffset( 1, this.offset, 0);
      break;
    }
    case Glow.eDirection_Up: {
      this.offset += this.speed * this.dyn.graph.getAnimationScanTime();
      if ( this.bounce != 0) {
	if ( this.offset > this.osize - d.height) {
	  this.offset -= this.offset- (this.osize - d.height);
	  this.direction = Glow.eDirection_Down;
	}
      }
      else {
	if ( this.offset > this.osize)
	  this.offset = -d.height;
      }
      object.setAnnotationTextOffset( 1, 0, this.offset);
      break;
    }
    case Glow.eDirection_Down: {
      this.offset -= this.speed * this.dyn.graph.getAnimationScanTime();
      if ( this.bounce != 0) {
	if ( this.offset < 0) {
	  this.offset = -this.offset;
	  this.direction = Glow.eDirection_Up;
	}
      }
      else {
	if ( this.offset < -d.height)
	  this.offset = this.osize;
      }
      object.setAnnotationTextOffset( 1, 0, this.offset);
      break;
    }
    default: ;
    }

    var value0 = this.a.get_ref_value(dyn);

    if ( this.firstScan || !(value0 == this.a.oldValue)) {
      object.setAnnotation(1, value0);
      this.dyn.repaintNow = true;
      this.a.oldValue = value0;
    }
    if ( this.firstScan)
      this.firstScan = false;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynScrollingText : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_ScrollingText: 
	break;
      case DynC.eSave_ScrollingText_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_ScrollingText_direction: 
	this.direction = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ScrollingText_speed: 
	this.speed = parseFloat( tokens[1]);
	break;
      case DynC.eSave_ScrollingText_bounce: 
	this.bounce = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynScrollingText");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynColorThemeLightness( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_ColorThemeLightness;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_ColorThemeLightness;
  this.instance_mask = 0;
  this.instance = 0;

  this.connect = function( o) {
    return 1;
  };

  this.disconnect = function() {
  };
	
  this.scan = function( o) {
    o.setColorThemeLightness();
    return 1;
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynColorThemeLightness : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_ColorThemeLightness: 
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynColorThemeLightness");
	break;
      }
      
      if ( end)
	break;
    }
    return i;
  };

}

function DynDigBackgroundColor( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DigBackgroundColor;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigBackgroundColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.attribute;
  this.color;
  this.firstScan = true;

  this.connect = function( object) {
    if ( this.color < 0 || this.color >= Glow.eDrawType__)
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigBackgroundColor: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    if ( this.a != null)
      this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( this.a === null || !this.a.sts || this.dyn.ignoreBgColor)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value && !this.dyn.resetColor) {
	// No change since last time
	if ( value)
	  this.dyn.ignoreBgColor = true;
	return;
      }
    }
    else
      this.firstScan = false;
	
    if ( value) {
      object.setBackgroundColor( this.color);
      this.dyn.ignoreBgColor = true;
    }
    else {
      object.resetBackgroundColor();
      this.dyn.resetBgColor = true;
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigBackgroundColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigBackgroundColor: 
	break;
      case DynC.eSave_DigBackgroundColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigBackgroundColor_color: 
	this.color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigBackgroundColor_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DigBackgroundColor_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end_ = true;
	break;
      default:
	console.log( "Syntax error in DynDigBackgroundColor");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynDigSwap( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DigSwap;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigSwap;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.reset_value;
  this.firstScan = true;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("DigSwap: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);

    if ( !this.firstScan) {
      this.a.oldValue = value;
      this.firstScan = false;
      return;
    }

    if ( this.a.inverted)
      value = !value;

    if ( this.a.oldValue == value) {
      // No change since last time
      return;
    }
	
    if ( value) {
      // Todo
      // this.dyn.graph.swap();
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigSwap : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigSwap: 
	break;
      case DynC.eSave_DigSwap_attribute: 
	if ( tokens.length > 1)
	  attribute = tokens[1];
	break;
      case DynC.eSave_DigSwap_reset_value: 
	reset_value = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigSwap");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

}

function DynAnimation( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Animation;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Animation;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.sequence;
  this.firstScan = true;
  this.animation_count;
  this.animation_direction;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("Animation: " + this.attribute);
      return 1;
    }

    if ( this.sequence == DynC.eAnimSequence_Inherit)
      this.sequence = object.get_nodeclass_dyn_attr1();

    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var max_count;
    var sts;

    if ( !this.a.sts)
      return;

    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);
    if ( this.a.inverted)
      value = !value;

    if ( this.firstScan) {
      this.animation_count = 0;
      this.animation_direction = 0;
      this.firstScan = false;
      if ( this.sequence == DynC.eAnimSequence_Dig) {
	if ( value)
	  object.set_last_nodeclass();
	this.a.oldValue = value;
      }
    }

    if ( this.sequence == DynC.eAnimSequence_Dig) {
      if ( this.a.oldValue != value) {
	// Start animation
	if ( value) {
	  this.animation_count = 0;
	  this.animation_direction = 1;
	}
	else if ( !value) {
	  this.animation_direction = 2;
	  this.animation_count = 0;
	}
      }

      if ( this.animation_direction !== 0) {
	max_count = object.get_animation_count();
	this.animation_count++;
	if ( this.animation_count >= max_count) {
	  // Shift nodeclass
	  if ( this.animation_direction == 1) {
	    // Shift forward

	    sts = object.set_next_nodeclass();
	    if ( (sts & 1) == 0) {
	      // End of animation
	      this.animation_count = 0;
	      this.animation_direction = 0;
	    }
	    this.animation_count = 0;
	  }
	  else {
	    // Shift backward

	    sts = object.set_previous_nodeclass();
	    if ( (sts & 1) === 0) {
	      // End of animation
	      this.animation_count = 0;
	      this.animation_direction = 0;
	    }
	    this.animation_count = 0;
	  }
	}
      }
    }
    else {
      if ( value) {
	if ( this.animation_direction == 0) {
	  // Animation has been stopped
	  this.animation_count = 0;
	  this.animation_direction = 1;
	}

	max_count = object.get_animation_count();
	this.animation_count++;
	if ( this.animation_count >= max_count) {
	  // Shift nodeclass
	  if ( this.animation_direction == 1) {
	    // Shift forward

	    sts = object.set_next_nodeclass();
	    if ((sts & 1) == 0) {
	      if ( this.sequence == DynC.eAnimSequence_Cycle) {
		// Start from the beginning again
		object.set_nodeclass_by_index( 1);
	      }
	      else {
		// Change direction
		this.animation_direction = 2;
		object.set_previous_nodeclass();
	      }
	    }
	    this.animation_count = 0;
	  }
	  else {
	    // Shift backward

	    sts = object.set_previous_nodeclass();
	    if ( (sts & 1) == 0) {
	      // Change direction
	      this.animation_direction = 1;
	      sts = object.set_next_nodeclass();
	    }
	    this.animation_count = 0;
	  }
	}
      }
      else {
	if ( this.animation_direction != 0) {
	  // Stop and reset animation
	  this.animation_direction = 0;
	  object.set_first_nodeclass();
	}
      }
    }
    this.a.oldValue = value;
  }

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "Animation : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Animation: 
	break;
      case DynC.eSave_Animation_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_Animation_sequence: 
	this.sequence = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynAnimation");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

}

function DynVideo( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Video;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Video;
  this.instance_mask = 0;
  this.instance = 0;

  this.connect = function( o) {
    return 1;
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "Video : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Video: 
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynVideo");
	break;
      }
      
      if ( end)
	break;
    }
    return i;    
  };
}

function DynBar( dyn)  {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Bar;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Bar;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.minvalue_a;
  this.maxvalue_a;
  this.attribute = null;
  this.minvalue_attr;
  this.maxvalue_attr;
  this.maxval = 0;
  this.minval;
  this.firstScan = true;
  this.old_value;
  this.old_ivalue;

  this.connect = function( object) {
    if ( this.attribute == null)
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("Bar: " + this.attribute);
      return 1;
    }

    this.minvalue_a = new DynReference( this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.maxvalue_a = new DynReference( this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    if ( !this.a.sts)
      return;
  
    var minval, maxval;
    if ( this.maxvalue_a.sts && this.minvalue_a.sts) {
      minval = this.minvalue_a.get_ref_value(this.dyn);
      maxval = this.maxvalue_a.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a.oldValue ||
	   maxval != this.maxvalue_a.oldValue) {
	object.set_range( minval, maxval);
	this.minvalue_a.oldValue = minval;
	this.maxvalue_a.oldValue = maxval;
      }
    }

    switch ( this.a.typeid) {
      case Pwr.eType_Float32: {
	var value = this.a.get_ref_value(this.dyn);

	if ( !this.firstScan) {
	  if ( Math.abs( this.old_value - value) < Number.MIN_VALUE)
	    // No change since last time
	    return;
	}
	else
	  this.firstScan = false;

	object.set_value(value);
	this.old_value = value;
	break;
      }
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32: {
	var value = this.a.get_ref_value(this.dyn);
	if ( !this.firstScan) {
	  if ( this.old_ivalue == value)
	    // No change since last time
	    return;
	}
	else
	  this.firstScan = false;

	object.set_value(value);
	this.old_ivalue = value;
	break;
      }
    }
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynBar : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Bar: 
	break;
      case DynC.eSave_Bar_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_Bar_minvalue_attr: 
	if ( tokens.length > 1)
	  this.minvalue_attr = tokens[1];
	break;
      case DynC.eSave_Bar_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynBar", i, key);
	break;
      }
      
      if ( end)
	break;
    }    
    return i;
  };

  this.action = function( object, e) {
    return 1;
  };
}

function DynTrend( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Trend;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Trend;
  this.instance_mask = 0;
  this.instance = 0;

  this.a1 = null;
  this.a2 = null;
  this.minvalue_a1 = null;
  this.maxvalue_a1 = null;
  this.minvalue_a2 = null;
  this.maxvalue_a2 = null;
  this.hold_a = null;
  this.timerange_a = null;
  this.mark1_a = null;
  this.mark2_a = null;
  this.attribute1 = null;
  this.attribute2 = null;
  this.minvalue_attr1 = null;
  this.maxvalue_attr1 = null;
  this.minvalue_attr2 = null;
  this.maxvalue_attr2 = null;
  this.hold_attr = null;
  this.timerange_attr = null;
  this.mark1_attr = null;
  this.mark2_attr = null;
  this.mark1_color;
  this.mark2_color;
  this.firstScan = true;
  this.scan_time;
  this.acc_time;
  this.no_of_points;
  this.trend_hold;
  this.orig_graph_scan_time;
  this.orig_graph_fast_scan_time;
  this.orig_graph_animation_scan_time;
								    
  this.connect = function( object) {

    this.a1 = new DynReference( this.dyn, this.attribute1);
    this.a1.connect(this.dyn);
    if ( !this.a1.sts) {
      console.log("Trend: " + this.attribute1);
      return 1;
    }

    if ( this.attribute2.trim() !== "") {
      this.a2 = new DynReference( this.dyn, this.attribute2);
      this.a2.connect(this.dyn);
      if ( !this.a2.sts) {
	console.log("Trend: " + this.attribute2);
	return 1;
      }
    }

    this.no_of_points = object.get_no_of_points();
    this.scan_time = object.get_scan_time();
    this.acc_time = this.scan_time;
    this.trend_hold = 0;

    if ( this.minvalue_attr1.trim() !== "") {
      this.minvalue_a1 = new DynReference( this.dyn, this.minvalue_attr1);
      this.minvalue_a1.connect(this.dyn);
      if ( !this.minvalue_a1.sts) {
	this.minvalue_a1 = null;
	console.log("Trend: " + this.minvalue_attr1);
      }
    }

    if ( this.maxvalue_attr1.trim() !== "") {
      this.maxvalue_a1 = new DynReference( this.dyn, this.maxvalue_attr1);
      this.maxvalue_a1.connect(this.dyn);
      if ( !this.maxvalue_a1.sts) {
	this.maxvalue_a1 = null;
	console.log("Trend: " + this.maxvalue_attr1);
      }
    }

    if ( this.minvalue_attr2.trim() !== "") {
      this.minvalue_a2 = new DynReference( this.dyn, this.minvalue_attr2);
      this.minvalue_a2.connect(this.dyn);
      if ( !this.minvalue_a2.sts) {
	this.minvalue_a2 = null;
	console.log("Trend: " + this.minvalue_attr2);
      }
    }

    if ( this.maxvalue_attr2.trim() !== "") {
      this.maxvalue_a2 = new DynReference( this.dyn, this.maxvalue_attr2);
      this.maxvalue_a2.connect(this.dyn);
      if ( !this.maxvalue_a2.sts) {
	this.maxvalue_a2 = null;
	console.log("Trend: " + this.maxvalue_attr2);
      }
    }

    if ( this.hold_attr.trim() !== "") {
      this.hold_a = new DynReference( this.dyn, this.hold_attr);
      this.hold_a.connect(this.dyn);
      if ( !this.hold_a.sts) {
	this.hold_a = null;
	console.log("Trend: " + this.hold_attr);
      }
    }

    if ( this.timerange_attr.trim() !== "") {
      this.timerange_a = new DynReference( this.dyn, this.timerange_attr);
      this.timerange_a.connect(this.dyn);
      if ( !this.timerange_a.sts) {
	this.timerange_a = null;
	console.log("Trend: " + this.timerange_attr);
      }

      this.orig_graph_scan_time = this.dyn.graph.getScanTime();
      this.orig_graph_fast_scan_time = this.dyn.graph.getFastScanTime();
      this.orig_graph_animation_scan_time = this.dyn.graph.getAnimationScanTime();
    }

    if ( this.mark1_attr !== null && this.mark1_attr.trim() !== "") {
      this.mark1_a = new DynReference( this.dyn, this.mark1_attr);
      this.mark1_a.connect(this.dyn);
      if ( !this.mark1_a.sts) {
	this.mark1_a = null;
	console.log("Trend: " + this.mark1_attr);
      }
    }

    if ( this.mark2_attr !== null && this.mark2_attr.trim() !== "") {
      this.mark2_a = new DynReference( this.dyn, this.mark2_attr);
      this.mark2_a.connect(this.dyn);
      if ( !this.mark2_a.sts) {
	this.mark2_a = null;
	console.log("Trend: " + this.mark2_attr);
      }
    }

    if ( this.mark1_color != Glow.eDrawType_Inherit || this.mark2_color != Glow.eDrawType_Inherit)
      object.set_mark_color( this.mark1_color, this.mark2_color);
    return 1;
  };

  this.disconnect = function() {
    this.a1.disconnect(this.dyn);
    if ( this.a2 != null)
      this.a2.disconnect(this.dyn);
    if ( this.minvalue_a1 != null)
      this.minvalue_a1.disconnect(this.dyn);
    if ( this.maxvalue_a1 != null)
      this.maxvalue_a1.disconnect(this.dyn);
    if ( this.minvalue_a2 != null)
      this.minvalue_a2.disconnect(this.dyn);
    if ( this.maxvalue_a2 != null)
      this.maxvalue_a2.disconnect(this.dyn);
    if ( this.hold_a != null)
      this.hold_a.disconnect(this.dyn);
    if ( this.timerange_a != null)
      this.timerange_a.disconnect(this.dyn);
    if ( this.mark1_a != null)
      this.mark1_a.disconnect(this.dyn);
    if ( this.mark2_a != null)
      this.mark2_a.disconnect(this.dyn);
  };

  this.scan = function( object) {

    if ( !this.a1.attrFound)
      return;
  
    if ( this.hold_a !== null) {
      var holdval = this.hold_a.get_ref_value(this.dyn);
      if ( holdval)
	return;
    }

    if ( this.timerange_a !== null) {
      var timerangeval = this.timerange_a.get_ref_value(this.dyn);
      if ( Math.abs(timerangeval - this.timerange_a.oldValue) > Number.MIN_VALUE) {
	var dt = timerangeval / this.no_of_points;
		     
	if ( dt >= 0.001) {
	  object.set_scan_time( dt);
	  this.scan_time = dt;
	  if ( this.cycle == Glow.eCycle_Slow) {
	    var current_graph_scan_time = this.dyn.graph.getScanTime();
	    var current_graph_fast_scan_time = this.dyn.graph.getFastScanTime();
	    var current_graph_animation_scan_time = this.dyn.graph.getAnimationScanTime();
	    if ( current_graph_scan_time > this.scan_time)
	      this.dyn.graph.setScanTime( this.scan_time);
	    else {
	      if ( this.scan_time > this.orig_graph_scan_time)
		this.dyn.graph.setScanTime( this.orig_graph_scan_time);
	      else
		this.dyn.graph.setScanTime( this.scan_time);
	    }
	    if ( current_graph_fast_scan_time > this.scan_time)
	      this.dyn.graph.setFastScanTime( this.scan_time);
	    else {
	      if ( this.scan_time > orig_graph_fast_scan_time)
		this.dyn.graph.setFastScanTime( this.orig_graph_fast_scan_time);
	      else
		this.dyn.graph.setFastScanTime( this.scan_time);
	    }
	    if ( current_graph_animation_scan_time > this.scan_time)
	      this.dyn.graph.setAnimationScanTime( this.scan_time);
	    else {
	      if ( this.scan_time > orig_graph_animation_scan_time)
		this.dyn.graph.setAnimationScanTime( this.orig_graph_animation_scan_time);
	      else
		this.dyn.graph.setAnimationScanTime( this.scan_time);
	    }
	  }
	  else {
	    // Fast cycle
	    var current_graph_fast_scan_time = this.dyn.graph.getFastScanTime();
	    var current_graph_animation_scan_time = this.dyn.graph.getAnimationScanTime();
	    if ( current_graph_fast_scan_time > this.scan_time)
	      this.dyn.graph.setFastScanTime( this.scan_time);
	    else {
	      if ( this.scan_time > orig_graph_fast_scan_time)
		this.dyn.graph.setFastScanTime( this.orig_graph_fast_scan_time);
	      else
		this.dyn.graph.setFastScanTime( this.scan_time);
	    }
	    if ( current_graph_animation_scan_time > this.scan_time)
	      this.dyn.graph.setAnimationScanTime( this.scan_time);
	    else {
	      if ( this.scan_time > orig_graph_animation_scan_time)
		this.dyn.graph.setAnimationScanTime( orig_graph_animation_scan_time);
	      else
		this.dyn.graph.setAnimationScanTime( this.scan_time);
	    }
	  }
	}
	this.timerange_a.oldValue = timerangeval;
      }
    }

    var minval, maxval;
    if ( this.maxvalue_a1 !== null && this.minvalue_a1 !== null) {
      minval = this.minvalue_a1.get_ref_value(this.dyn);
      maxval = this.maxvalue_a1.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a1.oldValue ||
	   maxval != this.maxvalue_a1.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 0, minval, maxval);
	this.minvalue_a1.oldValue = minval;
	this.maxvalue_a1.oldValue = maxval;
      }
    }

    if ( this.maxvalue_a2 !== null && this.minvalue_a2 !== null) {
      minval = this.minvalue_a2.get_ref_value(this.dyn);
      maxval = this.maxvalue_a2.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a2.oldValue ||
	   maxval != this.maxvalue_a2.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 1, minval, maxval);
	this.minvalue_a2.oldValue = minval;
	this.maxvalue_a2.oldValue = maxval;
      }
    }

    if ( this.mark1_a !== null) {
      var mark1val = this.mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark1( mark1val);
	this.mark1_a.oldValue = mark1val;
      }
    }
    if ( this.mark2_a !== null) {
      var mark2val = this.mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark2( mark2val);
	this.mark2_a.oldValue = mark2val;
      }
    }

    if ( this.firstScan)
      this.firstScan = false;

    if ( this.cycle == Glow.eCycle_Slow)
      this.acc_time += this.dyn.graph.getScanTime();
    else
      this.acc_time += this.dyn.graph.getFastScanTime();
    if ( this.acc_time + Number.MIN_VALUE >= this.scan_time) {
      if ( this.p1 != 0) {
	switch ( this.a1.typeid) {
	case Pwr.eType_Boolean: {
	  var value = this.a1.get_ref_value(this.dyn);
	  if ( value)
	    object.add_value( 1, 0);
	  else
	    object.add_value( 0, 0);
	  break;
	}
	case Pwr.eType_Float32: {
	  var value = this.a1.get_ref_value(this.dyn);
	  object.add_value( value, 0);
	  break;
	}
	case Pwr.eType_Int32:
	case Pwr.eType_UInt32: {
	  var value = this.a1.get_ref_value(this.dyn);
	  object.add_value( value, 0);
	  break;
	}
	default: ;
	}
      }
      if ( this.a2 != null && this.a2.sts) {
	switch ( this.a2.typeid) {
	case Pwr.eType_Boolean: {
	  var value = this.a2.get_ref_value(this.dyn);
	  if ( value)
	    object.add_value(1, 1);
	  else
	    object.add_value(0, 1);
	  break;
	}
	case Pwr.eType_Float32: {
	  var value = this.a2.get_ref_value(this.dyn);
	  object.add_value( value, 1);
	  break;
	}
	case Pwr.eType_Int32:
	case Pwr.eType_UInt32: {
	  var value = this.a2.get_ref_value(this.dyn);
	  object.add_value( value, 1);
	  break;
	}
	default: break;
	}
      }

      this.acc_time = 0;
    }
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynTrend : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Trend: 
	break;
      case DynC.eSave_Trend_attribute1: 
	if ( tokens.length > 1)
	  this.attribute1 = tokens[1];
	break;
      case DynC.eSave_Trend_attribute2: 
	if ( tokens.length > 1)
	  this.attribute2 = tokens[1];
	break;
      case DynC.eSave_Trend_minvalue_attr1: 
	if ( tokens.length > 1)
	  this.minvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_Trend_maxvalue_attr1: 
	if ( tokens.length > 1)
	  this.maxvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_Trend_minvalue_attr2: 
	if ( tokens.length > 1)
	  this.minvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_Trend_maxvalue_attr2: 
	if ( tokens.length > 1)
	  this.maxvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_Trend_hold_attr: 
	if ( tokens.length > 1)
	  this.hold_attr = tokens[1];
	break;
      case DynC.eSave_Trend_timerange_attr: 
	if ( tokens.length > 1)
	  this.timerange_attr = tokens[1];
	break;
      case DynC.eSave_Trend_mark1_attr: 
	if ( tokens.length > 1)
	  this.mark1_attr = tokens[1];
	break;
      case DynC.eSave_Trend_mark2_attr: 
	if ( tokens.length > 1)
	  this.mark2_attr = tokens[1];
	break;
      case DynC.eSave_Trend_mark1_color: 
	this.mark1_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Trend_mark2_color: 
	this.mark2_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynTrend", row, key);
	break;
      }

      if ( end)
	break;
    }    
    return i;
  };
}

function DynDsTrend( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DsTrend;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DsTrend;
  this.instance_mask = 0;
  this.instance = 0;

  this.minvalue_a1 = null;
  this.maxvalue_a1 = null;
  this.minvalue_a2 = null;
  this.maxvalue_a2 = null;
  this.hold_a = null;
  this.timerange_a = null;
  this.mark1_a = null;
  this.mark2_a = null;
  this.dstrend_object1 = null;
  this.dstrend_object2 = null;
  this.minvalue_attr1 = null;
  this.maxvalue_attr1 = null;
  this.minvalue_attr2 = null;
  this.maxvalue_attr2 = null;
  this.hold_attr = null;
  this.mark1_attr = null;
  this.mark2_attr = null;
  this.mark1_color;
  this.mark2_color;
  this.firstScan = true;
  this.scan_time;
  this.acc_time;
  this.no_of_points;
  this.trend_hold;
  this.orig_graph_scan_time;
  this.orig_graph_fast_scan_time;
  this.orig_graph_animation_scan_time;
  this.last_idx = new Array(2);
  this.last_buffer = new Array(2);
  this.dstrend_cnt = 0;
								    
  this.connect = function( object) {

    this.no_of_points = object.get_no_of_points();
    this.scan_time = object.get_scan_time();
    this.acc_time = this.scan_time;
    this.trend_hold = 0;
    this.last_idx[0] = -1;
    this.last_idx[1] = -1;
    this.last_buffer[0] = 0;
    this.last_buffer[1] = 0;

    if (this.dstrend_object1.trim() !== "") {
      this.dstrend_cnt++;
      if (this.dstrend_object2.trim() !== "")
	this.dstrend_cnt++;
    }

    if ( this.minvalue_attr1.trim() !== "") {
      this.minvalue_a1 = new DynReference( this.dyn, this.minvalue_attr1);
      this.minvalue_a1.connect(this.dyn);
      if ( !this.minvalue_a1.sts) {
	this.minvalue_a1 = null;
	console.log("Trend: " + this.minvalue_attr1);
      }
    }

    if ( this.maxvalue_attr1.trim() !== "") {
      this.maxvalue_a1 = new DynReference( this.dyn, this.maxvalue_attr1);
      this.maxvalue_a1.connect(this.dyn);
      if ( !this.maxvalue_a1.sts) {
	this.maxvalue_a1 = null;
	console.log("Trend: " + this.maxvalue_attr1);
      }
    }

    if ( this.minvalue_attr2.trim() !== "") {
      this.minvalue_a2 = new DynReference( this.dyn, this.minvalue_attr2);
      this.minvalue_a2.connect(this.dyn);
      if ( !this.minvalue_a2.sts) {
	this.minvalue_a2 = null;
	console.log("Trend: " + this.minvalue_attr2);
      }
    }

    if ( this.maxvalue_attr2.trim() !== "") {
      this.maxvalue_a2 = new DynReference( this.dyn, this.maxvalue_attr2);
      this.maxvalue_a2.connect(this.dyn);
      if ( !this.maxvalue_a2.sts) {
	this.maxvalue_a2 = null;
	console.log("Trend: " + this.maxvalue_attr2);
      }
    }

    if ( this.hold_attr.trim() !== "") {
      this.hold_a = new DynReference( this.dyn, this.hold_attr);
      this.hold_a.connect(this.dyn);
      if ( !this.hold_a.sts) {
	this.hold_a = null;
	console.log("Trend: " + this.hold_attr);
      }
    }

    if ( this.mark1_attr !== null && this.mark1_attr.trim() !== "") {
      this.mark1_a = new DynReference( this.dyn, this.mark1_attr);
      this.mark1_a.connect(this.dyn);
      if ( !this.mark1_a.sts) {
	this.mark1_a = null;
	console.log("Trend: " + this.mark1_attr);
      }
    }

    if ( this.mark2_attr !== null && this.mark2_attr.trim() !== "") {
      this.mark2_a = new DynReference( this.dyn, this.mark2_attr);
      this.mark2_a.connect(this.dyn);
      if ( !this.mark2_a.sts) {
	this.mark2_a = null;
	console.log("Trend: " + this.mark2_attr);
      }
    }

    if ( this.mark1_color != Glow.eDrawType_Inherit || this.mark2_color != Glow.eDrawType_Inherit)
      object.set_mark_color( this.mark1_color, this.mark2_color);
    return 1;
  };

  this.disconnect = function() {
    if ( this.minvalue_a1 != null)
      this.minvalue_a1.disconnect(this.dyn);
    if ( this.maxvalue_a1 != null)
      this.maxvalue_a1.disconnect(this.dyn);
    if ( this.minvalue_a2 != null)
      this.minvalue_a2.disconnect(this.dyn);
    if ( this.maxvalue_a2 != null)
      this.maxvalue_a2.disconnect(this.dyn);
    if ( this.hold_a != null)
      this.hold_a.disconnect(this.dyn);
    if ( this.mark1_a != null)
      this.mark1_a.disconnect(this.dyn);
    if ( this.mark2_a != null)
      this.mark2_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var new_curve = 0;
    var i;
    if ( this.hold_a !== null && !this.firstScan) {
      var holdval = this.hold_a.get_ref_value(this.dyn);
      if ( holdval)
	return;
    }

    var minval, maxval;
    if ( this.maxvalue_a1 !== null && this.minvalue_a1 !== null) {
      minval = this.minvalue_a1.get_ref_value(this.dyn);
      maxval = this.maxvalue_a1.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a1.oldValue ||
	   maxval != this.maxvalue_a1.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 0, minval, maxval);
	this.minvalue_a1.oldValue = minval;
	this.maxvalue_a1.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.maxvalue_a2 !== null && this.minvalue_a2 !== null) {
      minval = this.minvalue_a2.get_ref_value(this.dyn);
      maxval = this.maxvalue_a2.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a2.oldValue ||
	   maxval != this.maxvalue_a2.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 1, minval, maxval);
	this.minvalue_a2.oldValue = minval;
	this.maxvalue_a2.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.mark1_a !== null) {
      var mark1val = this.mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark1( mark1val);
	this.mark1_a.oldValue = mark1val;
      }
    }
    if ( this.mark2_a !== null) {
      var mark2val = this.mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark2( mark2val);
	this.mark2_a.oldValue = mark2val;
      }
    }

    if ( this.firstScan) {
      this.firstScan = false;
      new_curve = 1;
    }

    if (new_curve) {
      this.last_idx[0] = -1;
      this.last_idx[1] = -1;
    }

    if ( this.cycle == Glow.eCycle_Slow)
      this.acc_time += this.dyn.graph.getScanTime();
    else
      this.acc_time += this.dyn.graph.getFastScanTime();
    if ( new_curve || this.acc_time + Number.MIN_VALUE >= this.scan_time) {
      this.acc_time = 0;

      var data = new Array(4);
      data[0] = this;
      data[1] = object;
      data[2] = 0;
      data[3] = new_curve;	
      this.dyn.graph.getGdh().getDsTrend(this.dstrend_object1, 
	 this.last_idx[0], this.last_buffer[0], this.no_of_points, this.scan2, data);
    }
  };

  this.scan2 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var i, j;
      var self = data[0];
      var object = data[1];
      var curve_idx = data[2]
      var new_curve = data[3];
      self.last_idx[0] = result[0];
      self.last_buffer[0] = result[1];
      var size = result[2];
      var values = result[3];

      if (new_curve) {
	var vdata = new Array(self.no_of_points);
	var tdata = new Array(self.no_of_points);
	for (j = 0; j < self.no_of_points; j++) {
	  // tdata[self.no_of_points-j-1] = j / self.no_of_points * 100;
	  tdata[j] = j / self.no_of_points * 100;
	  if (j < size)
	    vdata[self.no_of_points-j-1] = values[j];
	}
        object.set_data(tdata, vdata, curve_idx, self.no_of_points);
      }
      else {
        for (i = 0; i < size; i++)
	  object.add_value( values[i], 0);
      }
      if (self.dstrend_cnt > 1) {
	var data = new Array(4);
	data[0] = self;
	data[1] = object;
	data[2] = 1;
	data[3] = new_curve;	
	self.dyn.graph.getGdh().getDsTrend(self.dstrend_object2, 
	   self.last_idx[1], self.last_buffer[1], self.no_of_points, self.scan3, data);
      }
    }
  };
  this.scan3 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var i, j;
      var self = data[0];
      var object = data[1];
      var curve_idx = data[2]
      var new_curve = data[3];
      self.last_idx[1] = result[0];
      self.last_buffer[1] = result[1];
      var size = result[2];
      var values = result[3];

      if (new_curve) {
	var vdata = new Array(self.no_of_points);
	var tdata = new Array(self.no_of_points);
	for (j = 0; j < self.no_of_points; j++) {
	  // tdata[self.no_of_points-j-1] = j / self.no_of_points * 100;
	  tdata[j] = j / self.no_of_points * 100;
	  if (j < size)
	    vdata[self.no_of_points-j-1] = values[j];
	}
        object.set_data(tdata, vdata, curve_idx, self.no_of_points);
      }
      else {
        for (i = 0; i < size; i++)
	  object.add_value( values[i], 1);
      }
    }
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDsTrend : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DsTrend: 
	break;
      case DynC.eSave_DsTrend_dstrend_object1: 
	if ( tokens.length > 1)
	  this.dstrend_object1 = tokens[1];
	break;
      case DynC.eSave_DsTrend_dstrend_object2: 
	if ( tokens.length > 1)
	  this.dstrend_object2 = tokens[1];
	break;
      case DynC.eSave_DsTrend_minvalue_attr1: 
	if ( tokens.length > 1)
	  this.minvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_DsTrend_maxvalue_attr1: 
	if ( tokens.length > 1)
	  this.maxvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_DsTrend_minvalue_attr2: 
	if ( tokens.length > 1)
	  this.minvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_DsTrend_maxvalue_attr2: 
	if ( tokens.length > 1)
	  this.maxvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_DsTrend_hold_attr: 
	if ( tokens.length > 1)
	  this.hold_attr = tokens[1];
	break;
      case DynC.eSave_DsTrend_mark1_attr: 
	if ( tokens.length > 1)
	  this.mark1_attr = tokens[1];
	break;
      case DynC.eSave_DsTrend_mark2_attr: 
	if ( tokens.length > 1)
	  this.mark2_attr = tokens[1];
	break;
      case DynC.eSave_DsTrend_mark1_color: 
	this.mark1_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DsTrend_mark2_color: 
	this.mark2_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDsTrend", row, key);
	break;
      }

      if ( end)
	break;
    }    
    return i;
  };
}

function DynDsTrendCurve( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_DsTrendCurve;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DsTrendCurve;
  this.instance_mask = 0;
  this.instance = 0;

  this.minvalue_a1 = null;
  this.maxvalue_a1 = null;
  this.minvalue_a2 = null;
  this.maxvalue_a2 = null;
  this.hold_a = null;
  this.timerange_a = null;
  this.mark1_a = null;
  this.mark2_a = null;
  this.dstrend_object = null;
  this.minvalue_attr1 = null;
  this.maxvalue_attr1 = null;
  this.minvalue_attr2 = null;
  this.maxvalue_attr2 = null;
  this.hold_attr = null;
  this.mark1_attr = null;
  this.mark2_attr = null;
  this.mark1_color;
  this.mark2_color;
  this.firstScan = true;
  this.scan_time;
  this.acc_time;
  this.no_of_points;
  this.trend_hold;
  this.orig_graph_scan_time;
  this.orig_graph_fast_scan_time;
  this.orig_graph_animation_scan_time;
  this.buff_cnt = 0;
  this.initialized = 0;
  this.no_of_points;

  this.cbid;
  this.displaytime;
  this.displayupdatetime;
  this.samples;
  this.resolution;
  this.elementtype1;
  this.elementtype2;
  this.scantime;
								    
  this.connect = function( object) {

    this.no_of_points = object.get_no_of_points();
    this.scan_time = object.get_scan_time();
    this.acc_time = this.scan_time;
    this.trend_hold = 0;

    if ( this.minvalue_attr1.trim() !== "") {
      this.minvalue_a1 = new DynReference( this.dyn, this.minvalue_attr1);
      this.minvalue_a1.connect(this.dyn);
      if ( !this.minvalue_a1.sts) {
	this.minvalue_a1 = null;
	console.log("Trend: " + this.minvalue_attr1);
      }
    }

    if ( this.maxvalue_attr1.trim() !== "") {
      this.maxvalue_a1 = new DynReference( this.dyn, this.maxvalue_attr1);
      this.maxvalue_a1.connect(this.dyn);
      if ( !this.maxvalue_a1.sts) {
	this.maxvalue_a1 = null;
	console.log("Trend: " + this.maxvalue_attr1);
      }
    }

    if ( this.minvalue_attr2.trim() !== "") {
      this.minvalue_a2 = new DynReference( this.dyn, this.minvalue_attr2);
      this.minvalue_a2.connect(this.dyn);
      if ( !this.minvalue_a2.sts) {
	this.minvalue_a2 = null;
	console.log("Trend: " + this.minvalue_attr2);
      }
    }

    if ( this.maxvalue_attr2.trim() !== "") {
      this.maxvalue_a2 = new DynReference( this.dyn, this.maxvalue_attr2);
      this.maxvalue_a2.connect(this.dyn);
      if ( !this.maxvalue_a2.sts) {
	this.maxvalue_a2 = null;
	console.log("Trend: " + this.maxvalue_attr2);
      }
    }

    if ( this.hold_attr.trim() !== "") {
      this.hold_a = new DynReference( this.dyn, this.hold_attr);
      this.hold_a.connect(this.dyn);
      if ( !this.hold_a.sts) {
	this.hold_a = null;
	console.log("Trend: " + this.hold_attr);
      }
    }

    if ( this.mark1_attr !== null && this.mark1_attr.trim() !== "") {
      this.mark1_a = new DynReference( this.dyn, this.mark1_attr);
      this.mark1_a.connect(this.dyn);
      if ( !this.mark1_a.sts) {
	this.mark1_a = null;
	console.log("Trend: " + this.mark1_attr);
      }
    }

    if ( this.mark2_attr !== null && this.mark2_attr.trim() !== "") {
      this.mark2_a = new DynReference( this.dyn, this.mark2_attr);
      this.mark2_a.connect(this.dyn);
      if ( !this.mark2_a.sts) {
	this.mark2_a = null;
	console.log("Trend: " + this.mark2_attr);
      }
    }

    this.no_of_points = object.get_no_of_points();
    if ( this.mark1_color != Glow.eDrawType_Inherit || this.mark2_color != Glow.eDrawType_Inherit)
      object.set_mark_color( this.mark1_color, this.mark2_color);

    this.dyn.graph.getGdh().getDsTrendCurveInfo(this.dstrend_object, 
	this.connect2, this);
  };

  this.connect2 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var self = data;
      self.cbid = result[0];
      self.resolution = result[1];
      self.samples = result[2];
      if (self.samples > self.no_of_points)
	self.samples = self.no_of_points;
      self.displayupdatetime = result[3];
      self.displaytime = result[4];      
      self.scantime = result[5];
      self.buff_cnt = result[6];
      self.elementtype1 = result[7];
      if (self.buff_cnt > 1)
        self.elementtype2 = result[8];
      self.initialized = 1;
    }
    return 1;
  };

  this.disconnect = function() {
    if ( this.minvalue_a1 != null)
      this.minvalue_a1.disconnect(this.dyn);
    if ( this.maxvalue_a1 != null)
      this.maxvalue_a1.disconnect(this.dyn);
    if ( this.minvalue_a2 != null)
      this.minvalue_a2.disconnect(this.dyn);
    if ( this.maxvalue_a2 != null)
      this.maxvalue_a2.disconnect(this.dyn);
    if ( this.hold_a != null)
      this.hold_a.disconnect(this.dyn);
    if ( this.mark1_a != null)
      this.mark1_a.disconnect(this.dyn);
    if ( this.mark2_a != null)
      this.mark2_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var new_curve = 0;
    var i;
    if ( !this.initialized)
      return;
    if ( this.hold_a !== null && !this.firstScan) {
      var holdval = this.hold_a.get_ref_value(this.dyn);
      if ( holdval)
	return;
    }

    var minval, maxval;
    if ( this.maxvalue_a1 !== null && this.minvalue_a1 !== null) {
      minval = this.minvalue_a1.get_ref_value(this.dyn);
      maxval = this.maxvalue_a1.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a1.oldValue ||
	   maxval != this.maxvalue_a1.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 0, minval, maxval);
	this.minvalue_a1.oldValue = minval;
	this.maxvalue_a1.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.maxvalue_a2 !== null && this.minvalue_a2 !== null) {
      minval = this.minvalue_a2.get_ref_value(this.dyn);
      maxval = this.maxvalue_a2.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a2.oldValue ||
	   maxval != this.maxvalue_a2.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 1, minval, maxval);
	this.minvalue_a2.oldValue = minval;
	this.maxvalue_a2.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.mark1_a !== null) {
      var mark1val = this.mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark1( mark1val);
	this.mark1_a.oldValue = mark1val;
      }
    }
    if ( this.mark2_a !== null) {
      var mark2val = this.mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark2( mark2val);
	this.mark2_a.oldValue = mark2val;
      }
    }

    if ( this.firstScan) {
      this.firstScan = false;
      new_curve = 1;
    }

    if ( this.cycle == Glow.eCycle_Slow)
      this.acc_time += this.dyn.graph.getScanTime();
    else
      this.acc_time += this.dyn.graph.getFastScanTime();
    if ( new_curve || this.acc_time + Number.MIN_VALUE >= this.scan_time) {
      this.acc_time = 0;

      var data = new Array(4);
      data[0] = this;
      data[1] = object;
      data[2] = 0;
      data[3] = new_curve;
      this.dyn.graph.getGdh().getDsTrendCurveBuffer(this.cbid,
	 this.samples, !new_curve, this.scan2, data);
    }
  };

  this.scan2 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var i, j, k;
      var size;
      var value;
      var values;
      var elementtype;
      var self = data[0];
      var object = data[1];
      var curve_idx = data[2];
      var new_curve = data[3];
      for (k = 0; k < self.buff_cnt; k++) {
	size = result[k * 3 + 1];
	elementtype = result[k * 3 + 2];
	values = result[k * 3 + 3];

	if (size > self.no_of_points)
	  size = self.no_of_points;
	if (new_curve) {
	  var vdata = new Array(size);
	  var tdata = new Array(size);
	  for (j = 0; j < size; j++) {
	    tdata[j] = j / size * 100;
	    vdata[size-j-1] = values[j];
	  }
	  object.set_data(tdata, vdata, k, size);
	}
	else {
	  for (i = 0; i < size; i++)
	    object.add_value( values[i], k);
	}
      }
    }
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDsTrendCurve : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DsTrendCurve: 
	break;
      case DynC.eSave_DsTrendCurve_dstrend_object: 
	if ( tokens.length > 1)
	  this.dstrend_object = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_minvalue_attr1: 
	if ( tokens.length > 1)
	  this.minvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_maxvalue_attr1: 
	if ( tokens.length > 1)
	  this.maxvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_minvalue_attr2: 
	if ( tokens.length > 1)
	  this.minvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_maxvalue_attr2: 
	if ( tokens.length > 1)
	  this.maxvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_hold_attr: 
	if ( tokens.length > 1)
	  this.hold_attr = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_mark1_attr: 
	if ( tokens.length > 1)
	  this.mark1_attr = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_mark2_attr: 
	if ( tokens.length > 1)
	  this.mark2_attr = tokens[1];
	break;
      case DynC.eSave_DsTrendCurve_mark1_color: 
	this.mark1_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_DsTrendCurve_mark2_color: 
	this.mark2_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDsTrendCurve", row, key);
	break;
      }

      if ( end)
	break;
    }    
    return i;
  };
}

function DynXY_Curve( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_XY_Curve;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_XY_Curve;
  this.instance_mask = 0;
  this.instance = 0;

  this.firstScan = true;
  this.x_a = null;
  this.y_a = null;
  this.y_minvalue_a = null;
  this.y_maxvalue_a = null;
  this.x_minvalue_a = null;
  this.x_maxvalue_a = null;
  this.noofpoints_a = null;
  this.update_a = null;
  this.x_mark1_a = null;
  this.x_mark2_a = null;
  this.y_mark1_a = null;
  this.y_mark2_a = null;
  this.dstrend_object1 = null;
  this.dstrend_object2 = null;
  this.y_minvalue_attr;
  this.y_maxvalue_attr;
  this.x_minvalue_attr;
  this.x_maxvalue_attr;
  this.noofpoints_attr;
  this.update_attr;
  this.hold_attr;
  this.x_mark1_attr = null;
  this.x_mark2_attr = null;
  this.y_mark1_attr = null;
  this.y_mark2_attr = null;
  this.mark1_color = Glow.eDrawType_Inherit;
  this.mark2_color = Glow.eDrawType_Inherit;
  this.y_min_value;
  this.y_max_value;
  this.x_min_value;
  this.x_max_value;
  this.horizontal_padding;
  this.datatype;
  this.curve_color = Glow.eDrawType_Inherit;
  this.fill_color = Glow.eDrawType_Inherit;
  this.noofpoints;
  this.noOfPoints;
  this.xAttrType;
  this.yAttrType;
  this.curveX = [];
  this.curveY = [];
  this.curve_number;
  this.object;
  var self = this;

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynXYCurve : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_XY_Curve: 
	break;
      case DynC.eSave_XY_Curve_x_attr: 
	if ( tokens.length > 1)
	  this.x_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_attr: 
	if ( tokens.length > 1)
	  this.y_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_minvalue_attr: 
	if ( tokens.length > 1)
	  this.y_minvalue_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.y_maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_x_minvalue_attr: 
	if ( tokens.length > 1)
	  this.x_minvalue_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_x_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.x_maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_noofpoints_attr: 
	if ( tokens.length > 1)
	  this.noofpoints_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_update_attr: 
	if ( tokens.length > 1)
	  this.update_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_hold_attr: 
	if ( tokens.length > 1)
	  this.hold_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_min_value: 
	this.y_min_value = parseFloat( tokens[1]);;
	break;
      case DynC.eSave_XY_Curve_y_max_value: 
	this.y_max_value = parseFloat( tokens[1]);;
	break;
      case DynC.eSave_XY_Curve_x_min_value: 
	this.x_min_value = parseFloat( tokens[1]);;
	break;
      case DynC.eSave_XY_Curve_x_max_value: 
	this.x_max_value = parseFloat( tokens[1]);;
	break;
      case DynC.eSave_XY_Curve_datatype: 
	this.datatype = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_instance_mask: 
	this.instance_mask = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_curve_color: 
	this.curve_color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_fill_color: 
	this.fill_color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_horizontal_padding: 
	this.horizontal_padding = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_x_mark1_attr: 
	if ( tokens.length > 1)
	  this.x_mark1_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_x_mark2_attr: 
	if ( tokens.length > 1)
	  this.x_mark2_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_mark1_attr: 
	if ( tokens.length > 1)
	  this.y_mark1_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_y_mark2_attr: 
	if ( tokens.length > 1)
	  this.y_mark2_attr = tokens[1];
	break;
      case DynC.eSave_XY_Curve_mark1_color: 
	this.mark1_color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_XY_Curve_mark2_color: 
	this.mark2_color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynXYCurve");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };


  this.connect = function( object) {
    if ( this.x_attr == null || this.y_attr == null)
      return 1;

    if ( this.update_attr.trim() !== "") {
      this.update_a = new DynReference( this.dyn, this.update_attr);
      this.update_a.connect(this.dyn);
      if ( !this.update_a.sts) {
	console.log( "XYCurve: " + this.update_attr);
	this.update_a = null;
      }
    }

    if ( this.noofpoints_attr.trim() !== "") {
      this.noofpoints_a = new DynReference( this.dyn, this.noofpoints_attr);
      this.noofpoints_a.connect(this.dyn);
      if ( !this.noofpoints_a.sts) {
	console.log( "XYCurve: " + this.noofpoints_attr);
	this.noofpoints_a = null;
      }
    }

    if ( this.y_minvalue_attr.trim() !== "") {
      this.y_minvalue_a = new DynReference( this.dyn, this.y_minvalue_attr);
      this.y_minvalue_a.connect(this.dyn);
      if ( !this.y_minvalue_a.sts) {
	console.log( "XYCurve: " + this.y_minvalue_attr);
	this.y_minvalue_a = null;
      }
    }

    if ( this.y_maxvalue_attr.trim() !== "") {
      this.y_maxvalue_a = new DynReference( this.dyn, this.y_maxvalue_attr);
      this.y_maxvalue_a.connect(this.dyn);
      if ( !this.y_maxvalue_a.sts) {
	console.log( "XYCurve: " + this.y_maxvalue_attr);
	this.y_maxvalue_a = null;
      }
    }

    if ( this.x_minvalue_attr.trim() !== "") {
      this.x_minvalue_a = new DynReference( this.dyn, this.x_minvalue_attr);
      this.x_minvalue_a.connect(this.dyn);
      if ( !this.x_minvalue_a.sts) {
	console.log( "XYCurve: " + this.x_minvalue_attr);
	this.x_minvalue_a = null;
      }
    }

    if ( this.x_maxvalue_attr.trim() !== "") {
      this.x_maxvalue_a = new DynReference( this.dyn, this.x_maxvalue_attr);
      this.x_maxvalue_a.connect(this.dyn);
      if ( !this.x_maxvalue_a.sts) {
	console.log( "XYCurve: " + this.x_maxvalue_attr);
	this.x_maxvalue_a = null;
      }
    }

    if ( this.y_mark1_attr.trim() !== "") {
      this.y_mark1_a = new DynReference( this.dyn, this.y_mark1_attr);
      this.y_mark1_a.connect(this.dyn);
      if ( !this.y_mark1_a.sts) {
	console.log( "XYCurve: " + this.y_mark1_attr);
	this.y_mark1_a = null;
      }
    }

    if ( this.y_mark2_attr.trim() !== "") {
      this.y_mark2_a = new DynReference( this.dyn, this.y_mark2_attr);
      this.y_mark2_a.connect(this.dyn);
      if ( !this.y_mark2_a.sts) {
	console.log( "XYCurve: " + this.y_mark2_attr);
	this.y_mark2_a = null;
      }
    }

    // Get curve number
    this.curve_number = 0;
    var m = this.instance;
    while( m != 0) {
      m = m >> 1;
      this.curve_number++;
    }

    // Get number of curves
    if ( this.instance == DynC.mInstance_1) {
      m = this.instance_mask;
      var noofcurves = 0;
      while( m != 0) {
	m = m >> 1;
	noofcurves++;
      }
      object.set_xy_noofcurves( noofcurves);
    }

    this.noofpoints = object.get_no_of_points();
    if ( Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE)
      object.set_xy_range_y( this.curve_number - 1, this.y_min_value, this.y_max_value);
    if ( Math.abs(this.x_max_value - this.x_min_value) > Number.MIN_VALUE)
      object.set_xy_range_x( this.curve_number - 1, this.x_min_value, this.x_max_value);
    //object.set_xy_curve_color( this.curve_number - 1, this.curve_color, this.fill_color);
    if ( this.mark1_color != Glow.eDrawType_Inherit || this.mark2_color != Glow.eDrawType_Inherit)
      object.set_mark_color( this.mark1_color, this.mark2_color);
    return 1;
  };

  this.disconnect = function() {
    if ( this.update_a !== null)
      this.update_a.disconnect(this.dyn);
    if ( this.noofpoints_a !== null)
      this.noofpoints_a.disconnect(this.dyn);
    if ( this.y_minvalue_a !== null)
      this.y_minvalue_a.disconnect(this.dyn);
    if ( this.y_maxvalue_a !== null)
      this.y_maxvalue_a.disconnect(this.dyn);
    if ( this.x_minvalue_a !== null)
      this.x_minvalue_a.disconnect(this.dyn);
    if ( this.x_maxvalue_a !== null)
      this.x_maxvalue_a.disconnect(this.dyn);
    if ( this.x_mark1_a !== null)
      this.x_mark1_a.disconnect(this.dyn);
    if ( this.x_mark2_a !== null)
      this.x_mark2_a.disconnect(this.dyn);
    if ( this.y_mark1_a !== null)
      this.y_mark1_a.disconnect(this.dyn);
    if ( this.y_mark2_a !== null)
      this.y_mark2_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var pname;
    var attrSize;
    var update = false;
    var size = 1;
    this.object = object;
    this.noOfPoints = this.noofpoints;

    if ( this.x_attr == null || this.y_attr == null)
      return;

    if ( this.firstScan)
      update = true;

    if ( this.update_a !== null) {
      var value = this.update_a.get_ref_value(this.dyn);
      if ( value && !this.update_a.oldValue)
	update = true;
      this.update_a.oldValue = value;
    }

    if ( this.noofpoints_a !== null) {
      var value = this.noofpoints_a.get_ref_value(this.dyn);
      if ( value != this.noofpoints_a.oldValue) {
	update = true;
	this.noofpoints = this.noOfPoints = value;
	this.noofpoints_a.oldValue = value;
      }
    }

    if ( this.y_minvalue_a !== null) {
      var value = this.y_minvalue_a.get_ref_value(this.dyn);
      if ( value != this.y_min_value) {
	this.y_min_value = value;
	if ( Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE)
	  object.set_xy_range_y( this.curve_number - 1, this.y_min_value, this.y_max_value);
	update = true;
      }
    }
    if ( this.y_maxvalue_a !== null) {
      var value = this.y_maxvalue_a.get_ref_value(this.dyn);
      if ( value != this.y_max_value) {
	this.y_max_value = value;
	if ( Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE)
	  object.set_xy_range_y( this.curve_number - 1, this.y_min_value, this.y_max_value);
	update = true;
      }
    }

    if ( this.x_minvalue_a !== null) {
      var value = this.x_minvalue_a.get_ref_value(this.dyn);
      if ( value != this.x_min_value) {
	this.x_min_value = value;
	if ( Math.abs(x_max_value - this.x_min_value) > Number.MIN_VALUE)
	  object.set_xy_range_x( this.curve_number - 1, this.x_min_value, this.x_max_value);
	update = true;
      }
    }
    if ( this.x_maxvalue_a !== null) {
      var value = this.x_maxvalue_a.get_ref_value(this.dyn);
      if ( value != this.x_max_value) {
	this.x_max_value = value;
	if ( Math.abs(x_max_value - this.x_min_value) > Number.MIN_VALUE)
	  object.set_xy_range_x( this.curve_number - 1, this.x_min_value, this.x_max_value);
	update = true;
      }
    }

    if ( this.x_mark1_a !== null) {
      var mark1val = this.x_mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.x_mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_x_mark1( mark1val);
	this.x_mark1_a.oldValue = mark1val;
      }
    }
    if ( this.x_mark2_a !== null) {
      var mark2val = this.x_mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.x_mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_x_mark2( mark2val);
	this.x_mark2_a.oldValue = mark2val;
      }
    }
    if ( this.y_mark1_a !== null) {
      var mark1val = this.y_mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.y_mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark1( mark1val);
	this.y_mark1_a.oldValue = mark1val;
      }
    }
    if ( this.y_mark2_a !== null) {
      var mark2val = this.y_mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.y_mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark2( mark2val);
	this.y_mark2_a.oldValue = mark2val;
      }
    }

    if ( update) {
      pname = this.dyn.parseAttrName( this.x_attr);
      attrSize = pname.elements;
      this.xAttrType = pname.type;

      switch ( this.datatype) {
      case DynC.eCurveDataType_XYArrays:
	if ( attrSize < this.noOfPoints)
	  this.noOfPoints = attrSize;
	size = this.noOfPoints;
	break;
      case DynC.eCurveDataType_PointArray:
	if ( attrSize/2 < this.noOfPoints)
	  this.noOfPoints = attrSize/2;
	size = this.noOfPoints * 2;
	break;
      case DynC.eCurveDataType_TableObject:
	if ( (attrSize-1)/2 < this.noOfPoints)
	  this.noOfPoints = (attrSize-1)/2;
	size = this.noOfPoints * 2 + 1;
	break;
      }
      
      // Read x-array
      switch ( this.xAttrType) {
      case Pwr.eType_Float32:
	this.dyn.graph.getGdh().getObjectInfoFloatArray( pname.name, size,
							 this.scan2, this);
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	this.dyn.graph.getGdh().getObjectInfoIntArray( pname.name, size,
						       this.scan2, this);
	break;
      default:
	return;
      }
    }
    this.firstScan = false;
  };

  this.scan2 = function( id, self, sts, value) {
    switch ( self.xAttrType) {
    case Pwr.eType_Float32:
      if ( !(sts & 1))
	return;
      
      switch ( self.datatype) {
      case DynC.eCurveDataType_XYArrays:
	self.curveX = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++)
	  self.curveX[i] = value[i];
	break;
      case DynC.eCurveDataType_PointArray:
	curveX = new Array(self.noOfPoints);
	curveY = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++) {
	  self.curveX[i] = value[2*i];
	  self.curveY[i] = value[2*i+1];
	}
	self.dyn.repaintNow = true;
	break;
      case DynC.eCurveDataType_TableObject:
	self.noOfPoints = Math.floor(value[0]);
	if ( self.noOfPoints > self.noofpoints)
	  self.noOfPoints = self.noofpoints;
	if ( attrSize < self.noOfPoints)
	  self.noOfPoints = attrSize;
	self.curveY = new Array(self.noOfPoints);
	self.curveX = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++) {
	  self.curveX[i] = value[2*i+1];
	  self.curveY[i] = value[2*i+2];
	}
	self.dyn.repaintNow = true;
	break;
      }
      break;
    case Pwr.eType_Int32:
    case Pwr.eType_Int16:
    case Pwr.eType_Int8:
    case Pwr.eType_UInt32:
    case Pwr.eType_UInt16:
    case Pwr.eType_UInt8:
      if ( !(sts & 1))
	return;    
      switch ( self.datatype) {
      case DynC.eCurveDataType_XYArrays:
	self.curveX = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++)
	  self.curveX[i] = value[i];
	break;
      case DynC.eCurveDataType_PointArray:
	self.curveX = new Array(self.noOfPoints);
	self.curveY = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++) {
	  self.curveX[i] = value[2*i];
	  self.curveY[i] = value[2*i+1];
	}
	self.dyn.repaintNow = true;
	break;
      case DynC.eCurveDataType_TableObject:
	self.noOfPoints = Math.floor(value[0]);
	if ( self.noOfPoints > self.noofpoints)
	  self.noOfPoints = self.noofpoints;
	if ( attrSize < self.noOfPoints)
	  self.noOfPoints = attrSize;
	self.curveY = new Array(self.noOfPoints);
	self.curveX = new Array(self.noOfPoints);
	for ( var i = 0; i < self.noOfPoints; i++) {
	  self.curveX[i] = value[2*i+1];
	  self.curveY[i] = value[2*i+2];
	}
	self.dyn.repaintNow = true;
	break;
      }
      break;
    default:
      return;
    }

    // Read y-array
    switch ( self.datatype) {
    case DynC.eCurveDataType_XYArrays:
      var pname = self.dyn.parseAttrName( self.y_attr);
      if ( pname.elements < self.noOfPoints)
	self.noOfPoints = pname.elements;
      self.yAttrType = pname.type;
      self.curveY = new Array(self.noOfPoints);

      switch ( self.yAttrType) {
      case Pwr.eType_Float32:
	self.dyn.graph.getGdh().getObjectInfoFloatArray( pname.name, self.noOfPoints,
							 self.scan3, self);
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	self.dyn.graph.getGdh().getObjectInfoIntArray( pname.name, self.noOfPoints,
						       self.scan3, self);
	break;
      default:
	return;
      }
      break;
    }
  };


  this.scan3 = function( id, self, sts, value) {
    if ( !(sts & 1))
      return;

    switch ( self.datatype) {
    case DynC.eCurveDataType_XYArrays:
      switch ( self.yAttrType) {
      case Pwr.eType_Float32:
	for ( var i = 0; i < self.noOfPoints; i++)
	  self.curveY[i] = value[i];
	self.dyn.repaintNow = true;
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	for ( var i = 0; i < self.noOfPoints; i++)
	  self.curveY[i] = value[i];
	
	self.dyn.repaintNow = true;
	
	break;
      default:
	return;
      }
      break;
    }
    
    self.object.set_xy_data( self.curveY, self.curveX, self.curve_number - 1, self.noOfPoints);
  };
};


function DynSevHist( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_SevHist;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_SevHist;
  this.instance_mask = 0;
  this.instance = 0;

  this.direction;
  this.timerange = 0;
  this.updatetime = 1;
  this.timerange_a = null;
  this.minvalue_a1 = null;
  this.maxvalue_a1 = null;
  this.minvalue_a2 = null;
  this.maxvalue_a2 = null;
  this.hold_a = null;
  this.update_a = null;
  this.mark1_a = null;
  this.mark2_a = null;
  this.sevhist_object1 = null;
  this.sevhist_object2 = null;
  this.attribute1 = null;
  this.attribute2 = null;
  this.timerange_attr = null;
  this.minvalue_attr1 = null;
  this.maxvalue_attr1 = null;
  this.minvalue_attr2 = null;
  this.maxvalue_attr2 = null;
  this.hold_attr = null;
  this.update_attr = null;
  this.mark1_attr = null;
  this.mark2_attr = null;
  this.mark1_color;
  this.mark2_color;
  this.firstScan = true;
  this.scan_time;
  this.acc_time;
  this.no_of_points;
  this.trend_hold;
  this.orig_graph_scan_time;
  this.orig_graph_fast_scan_time;
  this.orig_graph_animation_scan_time;
  this.buff_cnt = 0;
  this.initialized = 0;
  this.has_sevhist_object = 0;
  this.no_of_points;
  this.oid1 = new PwrtObjid(0,0);
  this.oid2 = new PwrtObjid(0,0);
  this.curve_cnt = 0;
  this.vtype1;
  this.vtype2;
								    
  this.connect = function( object) {

    this.no_of_points = object.get_no_of_points();
    this.direction = object.get_direction();
    this.scan_time = object.get_scan_time();
    this.acc_time = this.scan_time;
    this.trend_hold = 0;
    

    if ( this.minvalue_attr1.trim() !== "") {
      this.minvalue_a1 = new DynReference( this.dyn, this.minvalue_attr1);
      this.minvalue_a1.connect(this.dyn);
      if ( !this.minvalue_a1.sts) {
	this.minvalue_a1 = null;
	console.log("SevHist: " + this.minvalue_attr1);
      }
    }

    if ( this.maxvalue_attr1.trim() !== "") {
      this.maxvalue_a1 = new DynReference( this.dyn, this.maxvalue_attr1);
      this.maxvalue_a1.connect(this.dyn);
      if ( !this.maxvalue_a1.sts) {
	this.maxvalue_a1 = null;
	console.log("SevHist: " + this.maxvalue_attr1);
      }
    }

    if ( this.minvalue_attr2.trim() !== "") {
      this.minvalue_a2 = new DynReference( this.dyn, this.minvalue_attr2);
      this.minvalue_a2.connect(this.dyn);
      if ( !this.minvalue_a2.sts) {
	this.minvalue_a2 = null;
	console.log("SevHist: " + this.minvalue_attr2);
      }
    }

    if ( this.maxvalue_attr2.trim() !== "") {
      this.maxvalue_a2 = new DynReference( this.dyn, this.maxvalue_attr2);
      this.maxvalue_a2.connect(this.dyn);
      if ( !this.maxvalue_a2.sts) {
	this.maxvalue_a2 = null;
	console.log("SevHist: " + this.maxvalue_attr2);
      }
    }

    if ( this.hold_attr.trim() !== "") {
      this.hold_a = new DynReference( this.dyn, this.hold_attr);
      this.hold_a.connect(this.dyn);
      if ( !this.hold_a.sts) {
	this.hold_a = null;
	console.log("SevHist: " + this.hold_attr);
      }
    }

    if ( this.update_attr.trim() !== "") {
      this.update_a = new DynReference( this.dyn, this.update_attr);
      this.update_a.connect(this.dyn);
      if ( !this.update_a.sts) {
	this.update_a = null;
	console.log("SevHist: " + this.update_attr);
      }
    }

    if ( this.mark1_attr !== null && this.mark1_attr.trim() !== "") {
      this.mark1_a = new DynReference( this.dyn, this.mark1_attr);
      this.mark1_a.connect(this.dyn);
      if ( !this.mark1_a.sts) {
	this.mark1_a = null;
	console.log("SevHist: " + this.mark1_attr);
      }
    }

    if ( this.mark2_attr !== null && this.mark2_attr.trim() !== "") {
      this.mark2_a = new DynReference( this.dyn, this.mark2_attr);
      this.mark2_a.connect(this.dyn);
      if ( !this.mark2_a.sts) {
	this.mark2_a = null;
	console.log("SevHist: " + this.mark2_attr);
      }
    }

    if ( this.timerange_attr !== null && this.timerange_attr.trim() !== "") {
      this.timerange_a = new DynReference( this.dyn, this.timerange_attr);
      this.timerange_a.connect(this.dyn);
      if ( !this.timerange_a.sts) {
	this.timerange_a = null;
	console.log("SevHist: " + this.timerange_attr);
      }
    }

    this.no_of_points = object.get_no_of_points();
    if ( this.mark1_color != Glow.eDrawType_Inherit || this.mark2_color != Glow.eDrawType_Inherit)
      object.set_mark_color( this.mark1_color, this.mark2_color);
    if (this.timerange > Number.MIN_VALUE) {
      object.set_xy_range_x(0, this.timerange, 0);
      object.set_xy_range_x(1, this.timerange, 0);
    }
    if (this.sevhist_object1 !== "")
      this.dyn.graph.getGdh().getSevHistInfo(this.sevhist_object1, 
					     this.connect2, this);
    else if (this.attribute1 !== "") {
      this.curve_cnt++;
      if (this.attribute2 !== "")
	this.curve_cnt++;
      this.initialized = 1;
    }
  };

  this.connect2 = function( id, data, sts, result) {
    console.log("connect2", sts);
    if ( sts & 1 != 0) {
      var self = data;
      self.oid1 = result[0];
      self.attribute1 = result[1];
      self.server = result[2];
      self.has_sevhist_object;
      self.curve_cnt++;
      if (self.sevhist_object2 !== "")
	self.dyn.graph.getGdh().getSevHistInfo(self.sevhist_object2, 
					       self.connect3, self);
      else
	self.initialized = 1;
    }
    return 1;
  };

  this.connect3 = function( id, data, sts, result) {
    console.log("connect3", sts);
    if ( sts & 1 != 0) {
      var self = data;
      self.oid2 = result[0];
      self.attribute2 = result[1];
      self.server = result[2];
      self.curve_cnt++;
      self.initialized = 1;
    }
    return 1;
  };

  this.disconnect = function() {
    if ( this.timerange_a != null)
      this.timerange_a.disconnect(this.dyn);
    if ( this.minvalue_a1 != null)
      this.minvalue_a1.disconnect(this.dyn);
    if ( this.maxvalue_a1 != null)
      this.maxvalue_a1.disconnect(this.dyn);
    if ( this.minvalue_a2 != null)
      this.minvalue_a2.disconnect(this.dyn);
    if ( this.maxvalue_a2 != null)
      this.maxvalue_a2.disconnect(this.dyn);
    if ( this.hold_a != null)
      this.hold_a.disconnect(this.dyn);
    if ( this.update_a != null)
      this.update_a.disconnect(this.dyn);
    if ( this.mark1_a != null)
      this.mark1_a.disconnect(this.dyn);
    if ( this.mark2_a != null)
      this.mark2_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    var new_curve = 0;
    var i;
    if ( !this.initialized)
      return;
    if ( this.hold_a !== null && !this.firstScan) {
      var holdval = this.hold_a.get_ref_value(this.dyn);
      if ( holdval)
	return;
    }

    if ( this.update_a !== null) {
      var value = this.update_a.get_ref_value(this.dyn);
      if ( value && !this.update_a.oldValue)
	new_curve = 1;
      this.update_a.oldValue = value;
    }

    if ( this.timerange_a !== null) {
      this.timerange = this.timerange_a.get_ref_value(this.dyn);
      if ( this.timerange != this.timerange_a.oldValue) {
	object.set_xy_range_x(0, this.timerange, 0);
	if (this.curve_cnt > 1)
	  object.set_xy_range_x(1, this.timerange, 0);
	this.timerange_a.oldValue = this.timerange;
	new_curve = 1;
      }
    }

    var minval, maxval;
    if ( this.maxvalue_a1 !== null && this.minvalue_a1 !== null) {
      minval = this.minvalue_a1.get_ref_value(this.dyn);
      maxval = this.maxvalue_a1.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a1.oldValue ||
	   maxval != this.maxvalue_a1.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 0, minval, maxval);
	this.minvalue_a1.oldValue = minval;
	this.maxvalue_a1.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.maxvalue_a2 !== null && this.minvalue_a2 !== null) {
      minval = this.minvalue_a2.get_ref_value(this.dyn);
      maxval = this.maxvalue_a2.get_ref_value(this.dyn);
      if ( minval != this.minvalue_a2.oldValue ||
	   maxval != this.maxvalue_a2.oldValue) {
	if ( Math.abs( maxval - minval) > Number.MIN_VALUE)
	  object.set_range_y( 1, minval, maxval);
	this.minvalue_a2.oldValue = minval;
	this.maxvalue_a2.oldValue = maxval;
	new_curve = 1;
      }
    }

    if ( this.mark1_a !== null) {
      var mark1val = this.mark1_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark1val - this.mark1_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark1( mark1val);
	this.mark1_a.oldValue = mark1val;
      }
    }
    if ( this.mark2_a !== null) {
      var mark2val = this.mark2_a.get_ref_value(this.dyn);
      if ( this.firstScan || Math.abs( mark2val - this.mark2_a.oldValue) > Number.MIN_VALUE) {
	object.set_y_mark2( mark2val);
	this.mark2_a.oldValue = mark2val;
      }
    }

    if ( this.firstScan) {
      this.firstScan = false;
      new_curve = 1;
    }

    if ( this.cycle == Glow.eCycle_Slow)
      this.acc_time += this.dyn.graph.getScanTime();
    else
      this.acc_time += this.dyn.graph.getFastScanTime();
    if ( new_curve || this.acc_time + Number.MIN_VALUE >= this.updatetime) {
      //if (new_curve) {
      this.acc_time = 0;

      var data = new Array(2);
      data[0] = this;
      data[1] = object;
      this.dyn.graph.getGdh().getSevHistData(this.timerange, this.no_of_points,
	 this.server, this.oid1, this.attribute1, this.scan2, data);
    }
  };

  this.scan2 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var self = data[0];
      var object = data[1];
      self.vtype1 = result[0];
      var size = result[1];
      var tbuf = result[2];
      var vbuf = result[3];

      if (self.direction == Glow.eHorizDirection_Right) {
	for (var k = 0; k < tbuf.length; k++)
	  tbuf[k] = self.timerange - tbuf[k];
      }

      var tdata;
      var vdata;
      var points;
      switch (self.vtype1) {
      case Pwr.eType_Boolean:
	points = 0;
	size = 2*tbuf.length;
	tdata = new Array(size);
	vdata = new Array(size);
	for (var k = 0; k < tbuf.length; k++) {
	  if (k == 0) {
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	  else if (vbuf[k] != vbuf[k-1]) {
	    vdata[points] = vbuf[k-1];	  
	    tdata[points++] = tbuf[k];
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	  else if (k == tbuf.lenght - 1) {
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	}
	break;
      default:
	points = size;
	tdata = tbuf;
	vdata = vbuf;
      }
      object.set_xy_data(vdata, tdata, 0, points);
    }    
    if (self.curve_cnt > 1) {
      var data2 = new Array(2);
      data2[0] = self;
      data2[1] = object;
      self.dyn.graph.getGdh().getSevHistData(self.timerange, self.no_of_points,
	  self.server, self.oid2, self.attribute2, self.scan3, data2);
    }
  };

  this.scan3 = function( id, data, sts, result) {
    if ( sts & 1 != 0) {
      var self = data[0];
      var object = data[1];
      self.vtype2 = result[0];
      var size = result[1];
      var tbuf = result[2];
      var vbuf = result[3];
      //console.log("scan3", self.curve_cnt, size, self.vtype2);

      if (self.direction == Glow.eHorizDirection_Right) {
	for (var k = 0; k < tbuf.length; k++)
	  tbuf[k] = self.timerange - tbuf[k];
      }
      var tdata;
      var vdata;
      var points;
      switch (self.vtype1) {
      case Pwr.eType_Boolean:
	points = 0;
	size = 2*tbuf.length;
	tdata = new Array(size);
	vdata = new Array(size);
	for (var k = 0; k < tbuf.length; k++) {
	  if (k == 0) {
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	  else if (vbuf[k] != vbuf[k-1]) {
	    vdata[points] = vbuf[k-1];	  
	    tdata[points++] = tbuf[k];
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	  else if (k == tbuf.lenght - 1) {
	    vdata[points] = vbuf[k];
	    tdata[points++] = tbuf[k];
	  }
	}
	break;
      default:
	points = size;
	tdata = tbuf;
	vdata = vbuf;
      }
      object.set_xy_data(vdata, tdata, 1, points);
    }    
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynSevHist : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_SevHist: 
	break;
      case DynC.eSave_SevHist_sevhist_object1: 
	if ( tokens.length > 1)
	  this.sevhist_object1 = tokens[1];
	break;
      case DynC.eSave_SevHist_sevhist_object2: 
	if ( tokens.length > 1)
	  this.sevhist_object2 = tokens[1];
	break;
      case DynC.eSave_SevHist_attribute1: 
	if ( tokens.length > 1)
	  this.attribute1 = tokens[1];
	break;
      case DynC.eSave_SevHist_attribute2: 
	if ( tokens.length > 1)
	  this.attribute2 = tokens[1];
	break;
      case DynC.eSave_SevHist_server: 
	if ( tokens.length > 1)
	  this.server = tokens[1];
	break;
      case DynC.eSave_SevHist_timerange: 
	this.timerange = parseFloat( tokens[1]);
	break;
      case DynC.eSave_SevHist_updatetime: 
	this.updatetime = parseFloat( tokens[1]);
	break;
      case DynC.eSave_SevHist_timerange_attr: 
	if ( tokens.length > 1)
	  this.timerange_attr = tokens[1];
	break;
      case DynC.eSave_SevHist_minvalue_attr1: 
	if ( tokens.length > 1)
	  this.minvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_SevHist_maxvalue_attr1: 
	if ( tokens.length > 1)
	  this.maxvalue_attr1 = tokens[1];
	break;
      case DynC.eSave_SevHist_minvalue_attr2: 
	if ( tokens.length > 1)
	  this.minvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_SevHist_maxvalue_attr2: 
	if ( tokens.length > 1)
	  this.maxvalue_attr2 = tokens[1];
	break;
      case DynC.eSave_SevHist_hold_attr: 
	if ( tokens.length > 1)
	  this.hold_attr = tokens[1];
	break;
      case DynC.eSave_SevHist_update_attr: 
	if ( tokens.length > 1)
	  this.update_attr = tokens[1];
	break;
      case DynC.eSave_SevHist_mark1_attr: 
	if ( tokens.length > 1)
	  this.mark1_attr = tokens[1];
	break;
      case DynC.eSave_SevHist_mark2_attr: 
	if ( tokens.length > 1)
	  this.mark2_attr = tokens[1];
	break;
      case DynC.eSave_SevHist_mark1_color: 
	this.mark1_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_SevHist_mark2_color: 
	this.mark2_color = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynSevHist", row, key);
	break;
      }

      if ( end)
	break;
    }    
    return i;
  };
}


function DynPie( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Pie;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Pie;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute = new Array(GrowPie.PIE_MAX_SECTORS);
  this.sectors;
  this.min_value;
  this.max_value;
  this.fix_range;
  this.attr_type;
  this.connected = false;
  this.firstScan = true;

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynPie : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Pie: 
	break;
      case DynC.eSave_Pie_fix_range: 
	this.fix_range = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_Pie_attribute1: 
	if ( tokens.length > 1)
	  this.attribute[0] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute2: 
	if ( tokens.length > 1)
	  this.attribute[1] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute3: 
	if ( tokens.length > 1)
	  this.attribute[2] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute4: 
	if ( tokens.length > 1)
	  this.attribute[3] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute5: 
	if ( tokens.length > 1)
	  this.attribute[4] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute6: 
	if ( tokens.length > 1)
	  this.attribute[5] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute7: 
	if ( tokens.length > 1)
	  this.attribute[6] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute8: 
	if ( tokens.length > 1)
	  this.attribute[7] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute9: 
	if ( tokens.length > 1)
	  this.attribute[8] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute10: 
	if ( tokens.length > 1)
	  this.attribute[9] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute11: 
	if ( tokens.length > 1)
	  this.attribute[10] = tokens[1];
	break;
      case DynC.eSave_Pie_attribute12: 
	if ( tokens.length > 1)
	  this.attribute[11] = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynPie");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.connect = function( object) {

    var info = object.get_conf();
    this.min_value = info.min_val;
    this.max_value = info.max_val;
    this.sectors = info.sector_num;
    if ( this.sectors > GrowPie.PIE_MAX_SECTORS)
      this.sectors = GrowPie.PIE_MAX_SECTORS;
		 
    this.a = new Array(this.sectors);
    for ( var i = 0; i < this.sectors; i++) {
      if ( this.attribute[i] === null || this.attribute[i].trim() === "") {
	this.a[i] = null;
	continue;
      }

      this.a[i] = new DynReference( this.dyn, this.attribute[i]);
      this.a[i].connect(this.dyn);
      if ( !this.a[i].sts) {
	this.a[i] = null;
        console.log("Pie: " + this.attribute[i]);	
        return 1;
      }

      if ( i === 0)
	this.attr_type = this.a[0].typeid;
    }
    this.connected = true;
    return 1;
  };

  this.disconnect = function() {
    if ( !this.connected)
      return;
    for ( var i = 0; i < this.sectors; i++) {
      if ( this.a[i] !== null)
	this.a[i].disconnect(this.dyn);
    }
  };
	
  this.action = function( object, e) {
    return 1;
  };

  this.scan = function( object) {
    if ( !this.connected)
      return;

    switch ( this.attr_type) {
      case Pwr.eType_Float32: {
	var val = new Array(GrowPie.PIE_MAX_SECTORS);
	for ( var i = 0; i < this.sectors; i++)
	  val[i] = this.a[i].get_ref_value(this.dyn);

	if ( !this.firstScan) {
	  var update = 0;
	  for ( var i = 0; i < this.sectors; i++) {
	    if ( Math.abs( this.a[i].oldValue - val[i]) > Number.MIN_VALUE) {
	      update = 1;
	      break;
	    }
	  }
	  if ( update === 0)
	    return;
	}
	else
	  this.firstScan = false;
		
	if ( Math.abs( this.max_value - this.min_value) < Number.MIN_VALUE)
	  return;
		
	var dval = new Array(GrowPie.PIE_MAX_SECTORS);
	if ( this.fix_range != 0 || this.sectors == 1) {
	  for ( var i = 0; i < this.sectors; i++)
	    dval[i] = val[i] - this.min_value;
	}
	else {
	  var sum = 0;
	  for ( var i = 0; i < this.sectors; i++)
	    sum += val[i] - this.min_value;
	  for ( var i = 0; i < this.sectors; i++) {
	    if ( Math.abs( sum) < Number.MIN_VALUE)
	      dval[i] = 0;
	    else
	      dval[i] = (val[i] - this.min_value) / sum * 
		(this.max_value - this.min_value);
	  }
	}
		
	object.set_values( dval);
	for ( var i = 0; i < this.sectors; i++)
	  this.a[i].oldValue = val[i];
	break;
      }
      case Pwr.eType_Int32: {
	break;
      }
      default: ;
    }
  };
}

function DynBarChart( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_BarChart;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_BarChart;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute = new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS);
  this.bars;
  this.barsegments;
  this.min_value;
  this.max_value;
  this.fix_range;
  this.valueF;
  this.valueI;
  this.oldValueF;
  this.oldValueI;
  this.attr_type;
  this.firstScan = true;
  this.connected = false;

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynBarChart : " + lines[i]);

      switch ( key) {
      case DynC.eSave_BarChart: 
	break;
      case DynC.eSave_BarChart_fix_range: 
	this.fix_range = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_BarChart_attribute1: 
	if ( tokens.length > 1)
	  this.attribute[0] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute2: 
	if ( tokens.length > 1)
	  this.attribute[1] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute3: 
	if ( tokens.length > 1)
	  this.attribute[2] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute4: 
	if ( tokens.length > 1)
	  this.attribute[3] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute5: 
	if ( tokens.length > 1)
	  this.attribute[4] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute6: 
	if ( tokens.length > 1)
	  this.attribute[5] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute7: 
	if ( tokens.length > 1)
	  this.attribute[6] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute8: 
	if ( tokens.length > 1)
	  this.attribute[7] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute9: 
	if ( tokens.length > 1)
	  this.attribute[8] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute10: 
	if ( tokens.length > 1)
	  this.attribute[9] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute11: 
	if ( tokens.length > 1)
	  this.attribute[10] = tokens[1];
	break;
      case DynC.eSave_BarChart_attribute12: 
	if ( tokens.length > 1)
	  this.attribute[11] = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynBarChart");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.connect = function( object) {
    var info = object.get_conf();
    this.min_value = info.min_value;
    this.max_value = info.max_value;
    this.bars = info.bars;
    this.barsegments = info.barsegments;
		 
    this.a = new Array(this.barsegments);

    for ( var i = 0; i < this.barsegments; i++) {
      if ( this.attribute[i] === null || this.attribute[i].trim() === "") {
	console.log("BarChart", this.attributes[i]);
	return 1;
      }

      this.a[i] = new DynReference( this.dyn, this.attribute[i]);
      this.a[i].connect(this.dyn);
      
      if ( this.a[i].elements == 0)
	return 1;

      this.bars = Math.min( this.bars, this.a[i].elements);
      if ( i == 0) {
	this.attr_type = this.a[i].typeid;
	switch ( this.attr_type) {			
	case Pwr.eType_Float32:
	  this.valueF = new Array(this.barsegments);
	  break;
	case Pwr.eType_Int32:
	  this.valueI = new Array(this.barsegments);
	  break;
	default:
	  return 1;
	}
      }
      else {
	if ( this.attr_type != this.a[i].typeid) {
	  console.log("BarChart type", this.attribute[i], this.a[i].typeid);
	  return 1;
	}
      }
    }
    this.connected = true;
    return 1;
  }

  this.disconnect = function() {
    if ( !this.connected)
      return;
    for ( var i = 0; i < this.barsegments; i++)
      this.a[i].disconnect(this.dyn);
  }
	
  this.action = function( object, e) {
    return 1;
  };

  this.scan = function( object) {
    if ( !this.connected)
      return;
    switch ( this.attr_type) {
      case Pwr.eType_Float32: {    
	this.valueF = new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS);
	for ( var i = 0; i < this.barsegments; i++) {
	  this.valueF[i] = this.a[i].get_ref_value(this.dyn);
	  if ( this.valueF[i].length < this.bars) {
	    console.log("BarChart, value array to short", this.valueF[i].length);
	    break;
	  }
	}

	if ( !this.firstScan) {
	  var update = 0;
	  if ( this.oldValueF != null) {
	    for ( var i = 0; i < this.barsegments && this.valueF[i] != null; i++) {
	      if ( this.oldValueF[i] === null || this.valueF[i] === null)
		break;
	      for ( var j = 0; j < this.bars; j++) {
		if ( Math.abs( this.oldValueF[i][j] - this.valueF[i][j]) > Number.MIN_VALUE) {
		  update = 1;
		  break;
		}
	      }
	      if ( update == 1)
		break;
	    }
	    if ( update == 0)
	      return;
	  }
	}
	else
	  this.firstScan = false;
		
	if ( Math.abs( this.max_value - this.min_value) < Number.MIN_VALUE)
	  return;
		
	object.set_values( this.valueF[0], this.valueF[1], this.valueF[2], this.valueF[3], this.valueF[4], 
			   this.valueF[5], this.valueF[6], this.valueF[7], this.valueF[8], this.valueF[9], 
			   this.valueF[10], this.valueF[11]);
	this.oldValueF = this.valueF;

	break;
      }
      case Pwr.eType_Int32: {
	break;
      }
      default: ;
    }
  };
}

function DynTable( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_Table;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Table;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute = new Array(12);
  this.format = new Array(12);
  this.sel_attribute = new Array(12);
  this.cFormat;
  this.firstScan = true;
  this.oldValueB;
  this.oldValueF;
  this.oldValueI;
  this.oldValueS;
  this.p;
  this.hp = null;
  this.db;
  this.elements;
  this.type_id;
  this.size;
  this.headerRef;
  this.columns;
  this.rows;
  this.sel_p;
  this.sel_elements;
  this.bitmask;

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynTable : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Table: 
	break;
      case DynC.eSave_Table_attribute1: 
	if ( tokens.length > 1)
	  this.attribute[0] = tokens[1];
	break;
      case DynC.eSave_Table_format1: 
	if ( tokens.length > 1)
	  this.format[0] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute1: 
	if ( tokens.length > 1)
	  this.sel_attribute[0] = tokens[1];
	break;
      case DynC.eSave_Table_attribute2: 
	if ( tokens.length > 1)
	  this.attribute[1] = tokens[1];
	break;
      case DynC.eSave_Table_format2: 
	if ( tokens.length > 1)
	  this.format[1] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute2: 
	if ( tokens.length > 1)
	  this.sel_attribute[1] = tokens[1];
	break;
      case DynC.eSave_Table_attribute3: 
	if ( tokens.length > 1)
	  this.attribute[2] = tokens[1];
	break;
      case DynC.eSave_Table_format3: 
	if ( tokens.length > 1)
	  this.format[2] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute3: 
	if ( tokens.length > 1)
	  this.sel_attribute[2] = tokens[1];
	break;
      case DynC.eSave_Table_attribute4: 
	if ( tokens.length > 1)
	  this.attribute[3] = tokens[1];
	break;
      case DynC.eSave_Table_format4: 
	if ( tokens.length > 1)
	  this.format[3] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute4: 
	if ( tokens.length > 1)
	  this.sel_attribute[3] = tokens[1];
	break;
      case DynC.eSave_Table_attribute5: 
	if ( tokens.length > 1)
	  this.attribute[4] = tokens[1];
	break;
      case DynC.eSave_Table_format5: 
	if ( tokens.length > 1)
	  this.format[4] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute5: 
	if ( tokens.length > 1)
	  this.sel_attribute[4] = tokens[1];
	break;
      case DynC.eSave_Table_attribute6: 
	if ( tokens.length > 1)
	  this.attribute[5] = tokens[1];
	break;
      case DynC.eSave_Table_format6: 
	if ( tokens.length > 1)
	  this.format[5] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute6: 
	if ( tokens.length > 1)
	  this.sel_attribute[5] = tokens[1];
	break;
      case DynC.eSave_Table_attribute7: 
	if ( tokens.length > 1)
	  this.attribute[6] = tokens[1];
	break;
      case DynC.eSave_Table_format7: 
	if ( tokens.length > 1)
	  this.format[6] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute7: 
	if ( tokens.length > 1)
	  this.sel_attribute[6] = tokens[1];
	break;
      case DynC.eSave_Table_attribute8: 
	if ( tokens.length > 1)
	  this.attribute[7] = tokens[1];
	break;
      case DynC.eSave_Table_format8: 
	if ( tokens.length > 1)
	  this.format[7] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute8: 
	if ( tokens.length > 1)
	  this.sel_attribute[7] = tokens[1];
	break;
      case DynC.eSave_Table_attribute9: 
	if ( tokens.length > 1)
	  this.attribute[8] = tokens[1];
	break;
      case DynC.eSave_Table_format9: 
	if ( tokens.length > 1)
	  this.format[8] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute9: 
	if ( tokens.length > 1)
	  this.sel_attribute[8] = tokens[1];
	break;
      case DynC.eSave_Table_attribute10: 
	if ( tokens.length > 1)
	  this.attribute[9] = tokens[1];
	break;
      case DynC.eSave_Table_format10: 
	if ( tokens.length > 1)
	  this.format[9] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute10: 
	if ( tokens.length > 1)
	  this.sel_attribute[9] = tokens[1];
	break;
      case DynC.eSave_Table_attribute11: 
	if ( tokens.length > 1)
	  this.attribute[10] = tokens[1];
	break;
      case DynC.eSave_Table_format11: 
	if ( tokens.length > 1)
	  this.format[10] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute11: 
	if ( tokens.length > 1)
	  this.sel_attribute[10] = tokens[1];
	break;
      case DynC.eSave_Table_attribute12: 
	if ( tokens.length > 1)
	  this.attribute[11] = tokens[1];
	break;
      case DynC.eSave_Table_format12: 
	if ( tokens.length > 1)
	  this.format[11] = tokens[1];
	break;
      case DynC.eSave_Table_sel_attribute12: 
	if ( tokens.length > 1)
	  this.sel_attribute[11] = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynTable");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.connect = function( object) {
    var	sts;
    var	aname0 = null;

    var info = object.getTableInfo();
    this.columns = info.columns;
    this.rows = info.rows;

    this.db = new Array(this.columns);
    this.size = new Array(this.columns);
    this.elements = new Array(this.columns);
    this.type_id = new Array(this.columns);
    this.headerRef = new Array(this.columns);
    this.p = new Array(this.columns);
    for ( var i = 0; i < this.columns; i++)
      this.p[i] = 0;
    this.oldValueB = new Array(this.columns);
    this.oldValueF = new Array(this.columns);
    this.oldValueI = new Array(this.columns);
    this.oldValueS = new Array(this.columns);
    this.cFormat = new Array(this.columns);
    for ( var i = 0; i < this.columns; i++)
      this.cFormat[i] = new GlowCFormat( this.format[i]);
    this.sel_p = new Array(this.columns);
    this.sel_elements = new Array(this.columns);
    this.bitmask = new Array(this.columns);

    for ( var i = 0; i < this.columns; i++) {
      var pname = this.dyn.parseAttrName( this.attribute[i]);
      if ( pname === null || pname.name === "") 
	continue;

      if ( pname.elements == 0)
	continue;

      this.db[i] = pname.database;

      if ( pname.tname.substring( 0, 7) != "$header") { 
	switch ( this.db[i]) {
	case GraphIfc.eDatabase_Gdh:
	  this.p[i] = this.dyn.graph.getGdh().refObjectInfo( pname.tname, pname.type, pname.elements);
	  if ( i === 0)
	    aname0 = pname.name;
	  break;
	default:
	  ;
	}
	if ( this.p[i] === 0)
	  continue;

	this.size[i] = pname.elements;
	this.type_id[i] = pname.type;
	this.elements[i] = Math.min(pname.elements, this.rows);
	this.bitmask[i] = pname.bitmask;
      }
      else {
	this.headerRef[i] = true;

	if ( aname0 == null)
	  continue;
		    
	for ( var j = 0; j < this.elements[0]; j++) {
	  var hname = aname0 + "[" + j + "]";
	  var aname = "&(" + hname + ")" + pname.tname.substring(7);

	  if ( this.hp === null) {
	    this.hp = new Array(this.columns);
	    for ( var n = 0; n < this.columns; n++)
	      this.hp[n] = null;
	  }
	  if ( this.hp[i] == null) {
	    this.hp[i] = new Array(this.elements[0]);
	    for ( var k = 0; k < this.elements[0]; k++) 
	      this.hp[i][k] = 0;
	  }

	  this.hp[i][j] = this.dyn.graph.getGdh().refObjectInfo( aname, pname.type, pname.elements);
	  console.log("Table $header " + i + " hp " + this.hp[i][j]);
	  if ( j === 0) {
	    this.size[i] = pname.elements;
	    this.type_id[i] = pname.type;
	    this.elements[i] = this.elements[0];
	    this.bitmask[i] = pname.bitmask;
	  }
	}
      }

      switch ( this.type_id[i]) {
      case Pwr.eType_Float32:
	this.oldValueF[i] = new Array(this.rows);
	break;
      case Pwr.eType_Boolean:
      case GraphIfc.eType_Bit:
	oldValueB[i] = new Array(this.rows);
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	this.oldValueI[i] = new Array(this.rows);
	break;
      case Pwr.eType_String:
      case Pwr.eType_Objid:
      case Pwr.eType_AttrRef:
      case Pwr.eType_Time:
      case Pwr.eType_DeltaTime:
	this.oldValueS[i] = new Array(this.rows);
	break;
      }

    
      // Connect select array
      this.sel_p[i] = 0;
      pname = this.dyn.parseAttrName(this.sel_attribute[i]);
      if ( pname === null || pname.name === "") 
	continue;

      if ( pname.type != Pwr.eType_Boolean)
	continue;

      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	this.sel_p[i] = this.dyn.graph.getGdh().refObjectInfo( pname.tname, pname.type, pname.elements);
	if ( this.sel_p[i] !== 0) {
	  if ( this.sel_elements[i] == 0)
	    break;
	  this.sel_elements[i] = pname.elements;
	  if ( this.sel_elements[i] > this.elements[i])
	    this.sel_elements[i] = this.elements[i];
	}
	break;
      default:
	;
      }

    }
    object.setTableInfo(info);

    return 1;
  };

  this.disconnect = function() {
    for ( var i = 0; i < this.columns; i++) {
      if ( !this.headerRef[i]) {
	if ( this.p[i] != 0 && this.db[i] == GraphIfc.eDatabase_Gdh)
	  this.dyn.graph.getGdh().unrefObjectInfo( this.p[i]);
	this.p[i] = 0;
      }
      else {
	for ( var j = 0; j < this.elements[i]; j++) {
	  if ( this.hp[i][j] != 0)
	    this.dyn.graph.getGdh().unrefObjectInfo(this.hp[i][j]);
	}
      }
      switch ( this.type_id[i]) {
      case Pwr.eType_Float32:
	if ( this.oldValueF[i] !== null)
	  this.oldValueF[i] = null;
	break;
      case Pwr.eType_Boolean:
      case GraphIfc.eType_Bit:
	if ( this.oldValueB[i] !== null)
	  this.oldValueB[i] = null;
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	if ( this.oldValueI[i] !== null)
	  this.oldValueI[i] = null;
	break;
      case Pwr.eType_String:
      case Pwr.eType_Objid:
      case Pwr.eType_AttrRef:
      case Pwr.eType_Time:
      case Pwr.eType_DeltaTime:
	if ( this.oldValueS[i] !== null)
	  this.oldValueS[i] = null;
	break;
      }
      if ( this.sel_p[i] !== 0) {
	this.dyn.graph.getGdh().unrefObjectInfo( this.sel_p[i]);
	this.sel_p[i] = 0;
      }
    }
  };

  this.scan = function( object) {

    if ( this.p[0] == 0)
      return;

    for ( var i = 0; i < this.columns; i++) {
      if ( !this.headerRef[i]) {
	if ( this.p[i] === 0)
	  continue;
		
	switch ( this.type_id[i]) {
	case Pwr.eType_Float32: {
	  var val = this.dyn.graph.getGdh().getObjectRefInfo( this.p[i]);
	  if ( val === null)
	    break;
	  for ( var j = 0; j < Math.min(this.elements[i], val.length); j++) {
	    if ( this.oldValueF[i][j] != val[j] || this.firstScan) {
	      var sb = this.cFormat[i].format( val[j]);
	      object.setValue( sb, i, j);
	      this.oldValueF[i][j] = val[j];
	    }
	  }
	  break;
	}
	case Pwr.eType_Boolean: {
	  var val = this.dyn.graph.getGdh().getObjectRefInfo( this.p[i]);
	  if ( val === null)
	    break;
	  for ( var j = 0; j < Math.min( this.elements[i], val.length); j++) {
	    if ( this.firstScan || this.oldValueB[i][j] != val[j]) {
	      if ( val[j])
		object.setValue( "1", i, j);
	      else
		object.setValue( "0", i, j);
	      oldValueB[i][j] = val[j];
	    }
	  }
	  break;
	}
	case Pwr.eType_Int32:
	case Pwr.eType_Int16:
	case Pwr.eType_Int8:
	case Pwr.eType_UInt32:
	case Pwr.eType_UInt16:
	case Pwr.eType_UInt8: {
	  var val = this.dyn.graph.getGdh().getObjectRefInfo( this.p[i]);
	  if ( val == null)
	    break;
	  for ( var j = 0; j < Math.min(this.elements[i], val.length); j++) {
	    if ( this.oldValueI[i][j] != val[j] || this.firstScan) {
	      var sb = this.cFormat[i].format( val[j]);
	      object.setValue( sb, i, j);
	      this.oldValueI[i][j] = val[j];
	    }
	  }
	  break;
	}
	case Pwr.eType_String:
	case Pwr.eType_Objid:
	case Pwr.eType_AttrRef:
	case Pwr.eType_Time:
	case Pwr.eType_DeltaTime: {
	  var val = this.dyn.graph.getGdh().getObjectRefInfo( this.p[i]);
	  if ( val === null)
	    break;
	  for ( var j = 0; j < Math.min(this.elements[i], val.length); j++) {
	    if ( this.firstScan || this.oldValueS[i][j] != val[j]) {
	      switch ( this.type_id[i]) {
	      case Pwr.eType_AttrRef:
	      case Pwr.eType_Objid: {
		if ( this.format[i] == "%1o")
		  object.setValue(val[j], i, j);
		else {
		  var idx = val[j].lastIndexOf('-');
		  if ( idx == -1)
		    object.setValue(val[j], i, j);
		  else
		    object.setValue(val[j].substring(idx+1), i, j);
		}
					
		break;
	      }
	      default:
		object.setValue(val[j], i, j);
	      }
	      this.oldValueS[i][j] = val[j];
	    }
	  }
	  break;
	}
	case GraphIfc.eType_Bit: {
	  var val = this.dyn.graph.getGdh().getObjectRefInfo( this.p[i]);
	  if ( val == null)
	    break;
	  for ( var j = 0; j < Math.min(this.elements[i], val.length); j++) {
	    var bitval = ((this.bitmask[i] & val[j]) !== 0);
	    if ( this.oldValueB[i][j] != bitval || this.firstScan) {
	      if ( bitval)
		object.setValue( "1", i, j);
	      else
		object.setValue( "0", i, j);
	      this.oldValueB[i][j] = bitval;
	    }
	  }
	  break;
	}

	}
      }
      else {
	// $header reference
	for ( var j = 0; j < this.elements[i]; j++) {
	  if ( this.hp[i][j] == 0)
	    continue;

	  switch ( this.type_id[i]) {
	  case Pwr.eType_Float32: {
	    var val = this.dyn.graph.getGdh().getObjectRefInfo( this.hp[i][j]);
	    if ( typeof val === 'undefined')
	      break;

	    if ( this.oldValueF[i][j] != val || this.firstScan) {
	      var sb = this.cFormat[i].format( val);
	      object.setValue( sb, i, j);
	      this.oldValueF[i][j] = val;
	    }
	    break;
	  }
	  case Pwr.eType_Boolean: {
	    var val = this.dyn.graph.getGdh().getObjectRefInfo( this.hp[i][j]);
	    if ( typeof val === 'undefined')
	      break;

	    if ( this.firstScan || this.oldValueB[i][j] != val) {
	      if ( val)
		object.setValue( "1", i, j);
	      else
		object.setValue( "0", i, j);
	      object.setValue( sb, i, j);
	      this.oldValueB[i][j] = val;
	    }
	    break;
	  }
	  case Pwr.eType_Int32:
	  case Pwr.eType_Int16:
	  case Pwr.eType_Int8:
	  case Pwr.eType_UInt32:
	  case Pwr.eType_UInt16:
	  case Pwr.eType_UInt8: {
	    var val = this.dyn.graph.getGdh().getObjectRefInfo( this.hp[i][j]);
	    if ( typeof val == 'undefined')
	      break;

	    if ( this.oldValueI[i][j] != val || this.firstScan) {
	      var sb = this.cFormat[i].format( val);
	      object.setValue( sb, i, j);
	      this.oldValueI[i][j] = val;
	    }
	    break;
	  }
	  case Pwr.eType_String:
	  case Pwr.eType_Objid:
	  case Pwr.eType_AttrRef:
	  case Pwr.eType_Time:
	  case Pwr.eType_DeltaTime: {
	    var val = this.dyn.graph.getGdh().getObjectRefInfo( this.hp[i][j]);
	    if ( typeof val === 'undefined')
	      break;
	    if ( this.firstScan || oldValueS[i][j] != val) {
	      object.setValue( val, i, j);
	      this.oldValueS[i][j] = val;
	    }
	    break;
	  }
	  case GraphIfc.eType_Bit: {
	    var val = this.dyn.graph.getGdh().getObjectRefInfo( this.hp[i][j]);
	    if ( typeof val === 'undefined')
	      break;

	    var bitval = ((this.bitmask[i] & val) !== 0);
	    if ( this.oldValueB[i][j] != bitval || this.firstScan) {
	      if ( bitval)
		object.setValue( "1", i, j);
	      else
		object.setValue( "0", i, j);
	      this.oldValueB[i][j] = bitval;
	    }
	    break;
	  }			

	  }
	}
      }
    }

    // Examine select array
    var sel_found = false;
    for ( var i = 0; i < this.columns; i++) {
      if ( this.sel_p[i] === 0)
	continue;
      var val = this.dyn.graph.getGdh().getObjectRefInfo( this.sel_p[i]);
      if ( val === null)
	continue;
      for ( var j = 0; j < this.sel_elements[i]; j++) {
	if ( val[j]) {
	  sel_found = true;
	  object.setSelectedCell( i, j);
	}
      }
    }
    if ( !sel_found)
      object.setSelectedCell( -1, -1);

    if ( this.firstScan)
      this.firstScan = false;
  };

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      var column, row;
      var	value;

      if ( e.type != Glow.eEventType_Table)
	break;

      var event = e;
      row = object.getSelectedCellRow();
      column = object.getSelectedCellColumn();
      if ( row >= 0 && this.sel_p[column] !== 0) {
	// Reset previously selected
	var pname = this.dyn.parseAttrName( this.sel_attribute[column]);
	if ( pname === null || pname.name === "") 
	  break;
	value = false;
	var aname = pname.name + "[" + row + "]";
	this.dyn.graph.getGdh().setObjectInfoBoolean( aname, value);
      }
      if ( this.sel_p[event.column] !== 0 && 
	   !(event.column == column && event.row == row)) {
	// Set new selected, if not same as previous selected
	var pname = this.dyn.parseAttrName( this.sel_attribute[event.column]);
	if ( pname === null || pname.name === "") 
	  break;
	value = true;
	var aname = pname.name + "[" + event.row + "]";
	this.dyn.graph.getGdh().setObjectInfoBoolean( aname, value);
	object.setSelectedCell( event.column, event.row);
      }
      break;
    }
    return 1;
  };
}

function DynStatusColor( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_StatusColor;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_StatusColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.nostatus_color;
  this.use_colortheme;
  this.firstScan = true;
  this.oldStatus;
  this.on;


  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("StatusColor: " + attribute);
      return 1;
    }
    return 1;
  };


  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };
	
  this.scan = function( object) {
    if ( !this.a.sts || this.dyn.ignoreColor)
      return;

    if ( this.a.database == GraphIfc.eDatabase_Gdh && this.a.typeid == Pwr.eType_NetStatus) {
      // TODO get subscription oldness
    }

    var value = this.a.get_ref_value(this.dyn);

    if ( !this.firstScan && this.oldStatus != DynC.ePwrStatus_Fatal) {
      if ( this.a.oldValue == value && this.dyn.resetColor)
	// No change since last time
	return;
    }

    this.a.oldValue = value;
    if ( value === 0)
      value = DynC.ePwrStatus_No;
    else {
      switch ( value & 7) {
      case 3:
      case 1:
	value = DynC.ePwrStatus_Success;
	break;
      case 0:
	value = DynC.ePwrStatus_Warning;
	break;
      case 2:
	value = DynC.ePwrStatus_Error;
	break;
      case 4:
	value = DynC.ePwrStatus_Fatal;
	break;
      default:
	value = DynC.ePwrStatus_No;
      }
    }
    if ( !this.firstScan && this.oldStatus == value && this.oldStatus != DynC.ePwrStatus_Fatal)
      return;
    else
      this.firstScan = false;

    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) !== 0) {
      switch ( value) {
      case DynC.ePwrStatus_No:
	if ( this.nostatus_color >= Glow.eDrawTone__)
	  object.setFillColor( this.nostatus_color);
	else
	  object.setColorTone( this.nostatus_color);
	break;
      case DynC.ePwrStatus_Success:
	if ( nostatus_color >= Glow.eDrawTone__)
	  object.resetFillColor();
	object.resetColorTone();
	break;
      case DynC.ePwrStatus_Warning:
	object.setColorTone( (this.use_colortheme !== 0) ? Glow.eDrawType_CustomColor42 : Glow.eDrawTone_Yellow);
	break;
      case DynC.ePwrStatus_Error:
	object.setColorTone( (this.use_colortheme !== 0) ? Glow.eDrawType_CustomColor11 : Glow.eDrawTone_Red);
	break;
      case DynC.ePwrStatus_Fatal:
	this.on = !this.on;
	if ( this.on)
	  object.setColorTone( (this.use_colortheme !== 0) ? Glow.eDrawType_CustomColor11 : Glow.eDrawTone_Red);
	else {
	  if ( this.nostatus_color >= Glow.eDrawTone__)
	    object.setFillColor( this.nostatus_color);
	  else
	    object.setColorTone( this.nostatus_color);
	}
	break;
      }
      this.dyn.repaintNow = true;
    }
    else {
      switch ( value) {
      case DynC.ePwrStatus_No:
	object.setFillColor( this.nostatus_color);
	break;
      case DynC.ePwrStatus_Success:
	object.resetFillColor();
	break;
      case DynC.ePwrStatus_Warning:
	object.setFillColor( Glow.eDrawType_ColorYellow);
	break;
      case DynC.ePwrStatus_Error:
	object.setFillColor( Glow.eDrawType_ColorRed);
	break;
      case DynC.ePwrStatus_Fatal:
	this.on = !this.on;
	if ( this.on)
	  object.setFillColor( Glow.eDrawType_ColorRed);
	else
	  object.setFillColor( this.nostatus_color);
	break;
      }
      this.dyn.repaintNow = true;
    }
    this.oldStatus = value;
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynStatusColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_StatusColor: 
	break;
      case DynC.eSave_StatusColor_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_StatusColor_nostatus_color: 
	this.nostatus_color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_StatusColor_use_colortheme:
	this.use_colortheme = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynStatusColor");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}


function DynAxis( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = DynC.mDynType2_Axis;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Axis;
  this.instance_mask = 0;
  this.instance = 0;

  this.minvalue_a;
  this.maxvalue_a;
  this.minvalue_attr;
  this.maxvalue_attr;
  this.attr_type;
  this.firstScan = true;
  this.keep_settings = 0;

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynAxis : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_Axis: 
	break;
      case DynC.eSave_Axis_minvalue_attr: 
	if ( tokens.length > 1)
	  this.minvalue_attr = tokens[1];
	break;
      case DynC.eSave_Axis_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_Axis_keep_settings: 
	this.keep_settings = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynAxis");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.connect = function( o) {

    this.minvalue_a = new DynReference( this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);
    if ( !this.minvalue_a.sts)
      console.log( "Axis: " + this.minvalue_attr);
	    
    this.maxvalue_a = new DynReference( this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);
    if ( !this.maxvalue_a.sts)
      console.log( "Axis: " + this.maxvalue_attr);
	    
    if ( this.minvalue_a.sts && this.maxvalue_a.sts) {
      if ( this.minvalue_a.typeid != this.maxvalue_a.typeid)
	this.attr_type = 0;
      else
	this.attr_type = this.maxvalue_a.typeid;
    }
    else if ( this.maxvalue_a.sts)
      this.attr_type = this.maxvalue_a.typeid;
    else if ( this.minvalue_a.sts)
      this.attr_type = this.minvalue_a.typeid;
	    
    return 1;
  };

  this.disconnect = function() {
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
    return;
  };

  this.scan = function( o) {

    switch ( this.attr_type) {
      case Pwr.eType_Float32: {
	if ( !this.maxvalue_a.sts && !this.minvalue_a.sts)
	  return;

	var min_value = 0;
	var max_value = 0;

	if ( this.minvalue_a.sts)
	  min_value = this.minvalue_a.get_ref_value(this.dyn);
	if ( this.maxvalue_a.sts)
	  max_value = this.maxvalue_a.get_ref_value(this.dyn);
  
	if ( !(this.firstScan ||
	       (this.maxvalue_a.sts && ( max_value != this.maxvalue_a.oldValue)) ||
	       (this.minvalue_a.sts && ( min_value != this.minvalue_a.oldValue))))
	  return;
	       
	if ( this.firstScan)
	  this.firstScan = true;
	if ( this.minvalue_a.sts)
	  this.minvalue_a.oldValue = min_value;
	if ( this.maxvalue_a.sts)
	  this.maxvalue_a.oldValue = max_value;

	if ( max_value == min_value)
	  return;

	if ( o.type() == Glow.eObjectType_GrowAxis)
	  o.set_range( min_value, max_value, this.keep_settings);
	else if ( o.type() == Glow.eObjectType_GrowAxisArc)
	  o.set_range( min_value, max_value, this.keep_settings);
	break;
      }
      default: ;
    }
    return;
  };
}

function DynTimeoutColor( dyn) {
  this.dyn = dyn;
  this.dyn_type2 = DynC.mDynType2_TimeoutColor;
  this.dyn_type1 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_TimeoutColor;
  this.instance_mask = 0;
  this.instance = 0;

  this.time;
  this.color;
	
  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  }	
  this.scan = function( o) {
  }
  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynTimeoutColor : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_TimeoutColor: 
	break;
      case DynC.eSave_TimeoutColor_time: 
	this.time = Double.valueOf(tokens[1]);
	break;
      case DynC.eSave_TimeoutColor_color: 
	this.color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynTimeoutColor");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynHostObject( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_HostObject;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_HostObject;
  this.instance_mask = 0;
  this.instance = 0;

  this.hostobject;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  }	
  this.scan = function( o) {
  }
  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynHostObject : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_HostObject: 
	break;
      case DynC.eSave_HostObject_object: 
	if ( tokens.length > 1)
	  this.hostobject = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynHostObject");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };
  

}

function DynDigSound( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_DigSound;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_DigSound;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;
  this.soundobject;
  this.level;
  this.interval;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  }	
  this.scan = function( o) {
  }
  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynDigSound : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_DigSound: 
	break;
      case DynC.eSave_DigSound_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_DigSound_soundobject: 
	if ( tokens.length > 1)
	  this.soundobject = tokens[1];
	break;
      case DynC.eSave_DigSound_level: 
	this.level = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_DigSound_interval: 
	this.interval = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_DigSound_instance: 
	this.instance = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_DigSound_instance_mask: 
	this.instance_mask = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynDigSound");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynFillLevel( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_FillLevel;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_FillLevel;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.minvalue_a;
  this.maxvalue_a;
  this.attribute;
  this.color;
  this.direction;
  this.max_value;
  this.min_value;
  this.minvalue_attr;
  this.maxvalue_attr;
  this.firstScan = true;
  this.limits_found = false;
  this.limit_min;
  this.limit_max;

  this.connect = function( object) {

    this.color = this.dyn.getColor2( object, this.color);

    if ( this.color < 0 || this.color >= Glow.eDrawType__)
      return 0;

    if ( this.attribute.trim() === null)
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("FillLevel: " + attribute);
      return 1;
    }

    if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_Tone) != 0) {
      if ( this.color >= Glow.eDrawTone__)
	object.setLevelFillColor( this.color);
      else
	object.setLevelColorTone( this.color);
    }
    else
      object.setLevelFillColor( this.color);

    var limits = object.getLimits();
    if ( (limits.status & 1) !== 0) {
      this.limits_found = true;
      this.limit_min = limits.min;
      this.limit_max = limits.max;
      this.direction = limits.direction;
    }
    object.setLevelDirection( this.direction);

    this.minvalue_a = new DynReference( this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.maxvalue_a = new DynReference( this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    return 1;
  }

  this.disconnect = function() {
    if ( this.a === null)
      return;
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
  }

  this.scan = function( object) {
    if ( this.a === null || !this.a.sts)
      return;
  
    var minval, maxval;
    var pvalue = this.a.get_ref_value(this.dyn);
    if ( this.maxvalue_a.sts && this.minvalue_a.sts) {
      minval = this.minvalue_a.get_ref_value(this.dyn);
      maxval = this.maxvalue_a.get_ref_value(this.dyn);
      if ( minval != this.min_value ||
	   maxval != this.max_value) {
	this.min_value = minval;
	this.max_value = maxval;
	this.firstScan = true;
      }
    }

    if ( !this.firstScan) {
      if ( Math.abs( this.a.oldValue - pvalue) < Number.MIN_VALUE)
	// No change since last time
	return;
    }
    else
      this.firstScan = false;

    if ( this.max_value == this.min_value)
      return;

    var value = 0;
    if ( !this.limits_found)
      value = (pvalue - this.min_value) / (this.max_value - this.min_value);
    else {
      var geom = object.measure();
    
      switch ( this.direction) {
      case Glow.eDirection_Right:
	value = ((pvalue - this.min_value) / (this.max_value - this.min_value) * ( this.limit_max - this.limit_min) 
		 + (this.limit_min - geom.ll_x)) / (geom.ur_x - geom.ll_x);
	break;
      case Glow.eDirection_Left:
	value = ((pvalue - this.min_value) / (this.max_value - this.min_value) * ( this.limit_max - this.limit_min) 
		 + (geom.ur_x - this.limit_max)) / (geom.ur_x - geom.ll_x);
	break;
      case Glow.eDirection_Up:
	value = ((pvalue - this.min_value) / (this.max_value - this.min_value) * ( this.limit_max - this.limit_min) 
		 + (this.limit_min - geom.ll_y)) / (geom.ur_y - geom.ll_y);
	break;
      case Glow.eDirection_Down:
	value = ((pvalue - this.min_value) / (this.max_value - this.min_value) * ( this.limit_max - this.limit_min) 
		 + (geom.ur_y - this.limit_max)) / (geom.ur_y - geom.ll_y);
	break;
      default: ;
      }
    }
    object.setFillLevel( value);
    this.a.oldValue = pvalue;
  };

  this.action = function( object, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynFillLevel : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_FillLevel: 
	break;
      case DynC.eSave_FillLevel_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_FillLevel_color: 
	this.color = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_FillLevel_direction: 
	this.direction = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_FillLevel_min_value: 
	this.min_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_FillLevel_max_value: 
	this.max_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_FillLevel_minvalue_attr: 
	if ( tokens.length > 1)
	  this.minvalue_attr = tokens[1];
	break;
      case DynC.eSave_FillLevel_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynFillLevel");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynSetDig( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_SetDig;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_SetDig;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) !== 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
      if ( pname === null)
	return 1;
      var sts = null;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, true);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, true);
	break;
      default: break;
      }
      if ( sts.evenSts())
	console.log( "SetDig: " + pname.name);
      break;
    default: break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynSetDig : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_SetDig: 
	break;
      case DynC.eSave_SetDig_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_SetDig_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_SetDig_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynSetDig");
	break;
      }
      
      if ( end)
	break;
    }
    return i;
  };  
}

function DynResetDig( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_ResetDig;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_ResetDig;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) !== 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
      if ( pname === null)
	return 1;
      var sts = null;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, false);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, false);
	break;
      default: break;
      }
      if ( sts.evenSts())
	console.log( "ResetDig: " + pname.name);
      break;
    default: break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynResetDig : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_ResetDig: 
	break;
      case DynC.eSave_ResetDig_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_ResetDig_instance: 
	this.instance = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_ResetDig_instance_mask: 
	this.instance_mask = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynResetDig");
	break;
      }
      
      if ( end)
	break;
    }
    return i;
  };  
}

function DynToggleDig( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_ToggleDig;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_ToggleDig;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      this.dyn.graph.setClickActive(1);
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      this.dyn.graph.setClickActive( 0);
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
      if ( pname === null)
	return 1;
      var sts = null;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().toggleObjectInfo( pname.name);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().toggleObjectInfo( this.dyn.graph, pname.name);
	break;
      }
      if ( sts.evenSts())
	console.log( "ToggleDig: " + pname.name);
      break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynToggleDig : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_ToggleDig: 
	break;
      case DynC.eSave_ToggleDig_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynToggleDig");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynStoDig( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_StoDig;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_StoDig;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down: {
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;

      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
      var sts = null;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, true);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, true);
	break;
      }
      if ( sts.evenSts())
	console.log( "StoDig: " + pname.name);
      break;
    }
    case Glow.eEvent_MB1Up: {
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;

      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
      var sts = null;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, false);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, false);
	break;
      }
      if ( sts.evenSts())
        console.log( "StoDig: " + pname.name);
      break;
    }
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynToggleDig : " + lines[i]);

      switch ( key) {
      case DynC.eSave_StoDig: 
	break;
      case DynC.eSave_StoDig_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynStoDig");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynCommand( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_Command;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Command;
  this.instance_mask = 0;
  this.instance = 0;

  this.command;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      if ( this.command == null)
	return 1;

      var cmd = this.dyn.graph.getCommand( this.command);

      var r = this.dyn.graph.ctx.gdraw.get_clip_rectangle();
      if ( r != null)
	this.dyn.graph.ctx.gdraw.reset_clip_rectangle();
      
      this.dyn.graph.command( cmd);

      if ( r != null)
	this.dyn.graph.ctx.gdraw.set_clip_rectangle( r.x1, r.y1, r.x2, r.y2);

      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynCommand : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Command: 
	break;
      case DynC.eSave_Command_command: 
	if ( tokens.length > 1)
	  this.command = lines[i].substring(5);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynCommand");
	break;
      }

      if ( end)
	break;
    }		
    return i;
  };
}

function DynScript( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_Script;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Script;
  this.instance_mask = 0;
  this.instance = 0;

  this.script;
  this.script_len;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      if ( this.script == null)
	return 1;

      this.dyn.graph.script( this.script);
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynScript : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Script: 
	break;
      case DynC.eSave_Script_script_len: 
	this.script_len = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Script_script: 
	var idx;
	var send = false;
	this.script = "";
	i++;
	var line = lines[i].trim().substring(1);

	idx = 0;
	while ( line != null) {			    
	  while ( (idx = line.indexOf('"', idx)) != -1) {
	    if ( idx > 0 && (line.charAt(idx-1) == '\\')) {
	      line = line.substring(0, idx-1) + line.substring(idx);
	    }
	    else {				
	      if ( idx > 0)
		line = line.substring( 0, idx - 1);
	      else
		line = "";
	      this.script += line;
	      send = true;
	      break;
	    }
	  }
	  if ( send)
	    break;
	  this.script += line + "\n";
	  i++;
	  if ( i >= lines.length)
	    break;
	  
	  line = lines[i];
	}			
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynScript");
	break;
      }

      if ( end)
	break;
    }		
    console.log( "script : \"" + this.script + "\"");
    return i;
  };

}

function DynCommandDoubleClick( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_CommandDoubleClick;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_CommandDoubleClick;
  this.instance_mask = 0;
  this.instance = 0;

  this.command;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1ClickDoubleClick:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      if ( this.command == null)
	return 1;

      var cmd = this.dyn.graph.getCommand( this.command);

      this.dyn.graph.command( cmd);
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynCommandDoubleClick : " + lines[i]);

      switch ( key) {
      case DynC.eSave_CommandDC: 
	break;
      case DynC.eSave_CommandDC_command: 
	if ( tokens.length > 1)
	  this.command = lines[i].substring(5);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynCommandDoubleClick");
	break;
      }

      if ( end)
	break;
    }		
    return i;
  };
}

function DynConfirm( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_Confirm;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Confirm;
  this.instance_mask = 0;
  this.instance = 0;
  
  this.text;
  this.on_set;
  this.on_reset;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };

  this.action = function( object, e) {
    if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) == 0)
      return 1;

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      break;
    case Glow.eEvent_MB1Click:
    case Glow.eEvent_ValueChanged:

      var skip = 0;
      if ( ((this.on_set != 0 && this.on_reset == 0) || (this.on_reset != 0 && this.on_set == 0)) &&
	   (this.dyn.total_action_type1 & DynC.mActionType1_ToggleDig) != 0) {
	for ( var j = 0; j < this.dyn.elements.length; j++) {
	  if ( dyn.elements.get(j).action_type1 == DynC.mActionType1_ToggleDig) {
	    var pname = this.dyn.parseAttrName( this.dyn.elements.get(j).attribute);
	    if ( pname.name.substring(0,1) == "&")
	      pname.name = this.dyn.graph.get_reference_name( pname.name);

	    switch ( pname.database) {
	    case GraphIfc.eDatabase_Gdh:
	      // TODO
	      var ret = this.dyn.graph.getGdh().getObjectInfoBoolean( pname.name);
	      if (ret.oddSts()) {
		if ( (this.on_set != 0 && ret.value) || (this.on_reset != 0 && !ret.value))
		  skip = 1;
	      }
	      else
		console.log("Confirm: " + ret.getSts());
	      break;
	    case GraphIfc.eDatabase_Ccm:
	      // TODO
	      break;
	    default: ;
	    }
	    break;
	  }
	}
      }
      if ( skip != 0) {
	this.dyn.confirmedAction(  e.event, object);
	return 1;
      }

      this.dyn.graph.openConfirmDialog( this.dyn, this.text, object);
      break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynConfirm : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Confirm: 
	break;
      case DynC.eSave_Confirm_text: 
	if ( tokens.length > 1)
	  this.text = lines[i].substring(5);
	break;
      case DynC.eSave_Confirm_on_set: 
	this.on_set = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_Confirm_on_reset: 
	this.on_reset = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynConfirm");
	break;
      }

      if ( end)
	break;
    }
 		
    return i;
  };

}

function DynIncrAnalog( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_IncrAnalog;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_IncrAnalog;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;
  this.increment;
  this.min_value;
  this.max_value;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };

  this.action = function( object, e) {

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;

      var pname = this.dyn.parseAttrName( this.attribute);
      if ( pname == null)
	return 1;
      var typeId = pname.type;
      if ( pname.type < 0)
	typeId = Pwr.eType_Float32;
      switch ( typeId) {
      case Pwr.eType_Int32:
	switch (pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  this.dyn.graph.getGdh().getObjectInfoInt( pname.name, this.action2, this);
	  break;
	case GraphIfc.eDatabase_Local:
	  var ret = this.dyn.graph.getLdb().getObjectInfo( this.dyn.graph, pname.name);
	  this.action2(0, this, 1, ret.value);
	  break;
	}
	break;
      default:
	switch (pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  this.dyn.graph.getGdh().getObjectInfoFloat( pname.name, this.action2, this);
	  break;
	case GraphIfc.eDatabase_Local:
	  var ret = this.dyn.graph.getLdb().getObjectInfo( this.dyn.graph, pname.name);
	  this.action2(0, this, 1, ret.value);
	  break;
	}
      }
      break;
    }
    return 1;
  };

  this.action2 = function( id, self, sts, value) {
    if ( !(sts & 1))
      return;

    var pname = self.dyn.parseAttrName( self.attribute);
    if ( pname == null)
      return 1;
    var typeId = pname.type;
    if ( pname.type < 0)
      typeId = Pwr.eType_Float32;
    switch ( typeId) {
    case Pwr.eType_Int32: {
      value += self.increment;
      if ( !( self.min_value == 0 && self.max_value == 0)) {
	if ( value < self.min_value)
	  value = Math.floor(self.min_value);
	if ( value > self.max_value)
	  value = Math.floor(self. max_value);
      }
      
      var sts;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = self.dyn.graph.getGdh().setObjectInfoInt( pname.name, value);
	break;
      case GraphIfc.eDatabase_Local:
	sts = self.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	break;
      }
      if ( sts.evenSts())
	console.log( "IncrAnalog " + pname.name);
      break;
    }
    default: {
      value += self.increment;
      if ( !( self.min_value == 0 && self.max_value == 0)) {
	if ( value < self.min_value)
	  value = self.min_value;
	if ( value > self.max_value)
	  value = self.max_value;
      }
      var sts;
      switch (pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = self.dyn.graph.getGdh().setObjectInfoFloat( pname.name, value);
	break;
      case GraphIfc.eDatabase_Local:
	sts = self.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	break;
      }
      if ( sts.evenSts())
	console.log( "IncrAnalog " + pname.name);
    }
    }
    return 1;    
  };


  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynConfirm : " + lines[i]);

      switch ( key) {
      case DynC.eSave_IncrAnalog: 
	break;
      case DynC.eSave_IncrAnalog_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_IncrAnalog_increment: 
	this.increment = parseFloat( tokens[1]);
	break;
      case DynC.eSave_IncrAnalog_min_value: 
	this.min_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_IncrAnalog_max_value: 
	this.max_value = parseFloat( tokens[1]);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynIncrAnalog");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynRadioButton( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_RadioButton;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_RadioButton;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_RadioButton;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.attribute;
  this.firstScan = true;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("RadioButton: " + this.attribute);
      return 1;
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
  };

	
  this.scan = function( object) {
    if ( !this.a.sts)
      return;
    var value = this.dyn.getDig( this.a.p, this.a.typeid, this.a.bitmask, this.a.database);
    if ( this.a.inverted)
      value = !value;

    if ( !this.firstScan) {
      if ( this.a.oldValue == value)
	return;
    }
    else
      this.firstScan = false;
	
    if ( value) {
      object.set_last_nodeclass();
    }
    else {
      object.set_root_nodeclass();
    }
    this.a.oldValue = value;
  };

  this.action = function( object, e) {

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click: {
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) !== 0)
	break;

      var value;
      var group = this.dyn.graph.getCtx().get_object_group(object);
      if ( group == null) break;

      var list = group.get_object_list();

      for ( var i = 0; i < list.size(); i++) {
	var oe = list.get(i);
	if ( oe != e.object &&
	     oe.type() == Glow.eObjectType_GrowNode) {
	  value = false;
	  var gm_dyn = oe.getUserData();

	  if ( (gm_dyn.total_action_type1 & DynC.mActionType1_RadioButton) !== 0) {
	    for ( var j = 0; j < gm_dyn.elements.length; j++) {
	      if ( gm_dyn.elements[j].action_type1 == DynC.mActionType1_RadioButton) {
		var pname = this.dyn.parseAttrName( gm_dyn.elements[j].attribute);
		if ( pname.name.substring(0,1) == "&")
		  pname.name = this.dyn.graph.get_reference_name( pname.name);
		var sts;
		switch ( pname.database) {
		case GraphIfc.eDatabase_Gdh:
		  sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, value);
		  break;
		case GraphIfc.eDatabase_Local:
		  sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
		  break;
		case GraphIfc.eDatabase_Ccm:
		  // TODO
		  break;
		default: ;
		}
	      }
	    }
	  }
	}
      }
		
      var pname = this.dyn.parseAttrName( this.attribute);
      if ( pname.name.substring( 0, 1) == "&")
	pname.name = this.dyn.graph.get_reference_name( pname.name);

      value = true;
      var sts;
      switch ( pname.database) {
      case GraphIfc.eDatabase_Gdh:
	sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, value);
	break;
      case GraphIfc.eDatabase_Local:
	sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	break;
      case GraphIfc.eDatabase_Ccm:
	// TODO
	break;
      default: ;
      }
      break;
    }
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynRadioButton : " + lines[i]);

      switch ( key) {
      case DynC.eSave_RadioButton: 
	break;
      case DynC.eSave_RadioButton_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynRadioButton");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynTipText( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_TipText;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_TipText;
  this.instance_mask = 0;
  this.instance = 0;

  this.text;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    return 1;
  };
  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynTipText : " + lines[i]);

      switch ( key) {
      case DynC.eSave_TipText: 
	break;
      case DynC.eSave_TipText_text: 
	if ( tokens.length > 1)
	  this.text = lines[i].substring(5);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynTipText");
	break;
      }
      
      if ( end)
	break;
    }
    return i;
  };
}

function DynHelp( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_Help;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Help;
  this.instance_mask = 0;
  this.instance = 0;

  this.topic = null;
  this.bookmark = null;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( this.topic == null)
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var command;
      if ( this.bookmark !== null && this.bookmark !== "")
	command = "help " + this.topic + " /bookmark=" + this.bookmark;
      else
	command = "help " + this.topic;

      this.dyn.graph.command( command);
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynHelp : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Help: 
	break;
      case DynC.eSave_Help_topic: 
	if ( tokens.length > 1)
	  this.topic = tokens[1];
	break;
      case DynC.eSave_Help_bookmark: 
	if ( tokens.length > 1)
	  this.bookmark = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynHelp");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };
}

function DynOpenGraph( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_OpenGraph;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_OpenGraph;
  this.instance_mask = 0;
  this.instance = 0;

  this.graph_object;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( o, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      o.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      o.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) !== 0)
	break;
      var command = null;
      if ( !(this.graph_object === null || this.graph_object === ""))
	command = "open graph/object=" + this.graph_object;
      else {
	// Open classgraph for popup menu object
	if ( (this.dyn.total_action_type1 & DynC.mActionType1_PopupMenu) !== 0) {
	  for ( var i = 0; i < this.dyn.elements.length; i++) {
	    if ( this.dyn.elements[i].action_type1 == DynC.mActionType1_PopupMenu) {
	      command = "open graph/class/instance=" + this.dyn.elements[i].ref_object;
	      break;
	    }
	  }
	}
	else if ( (this.dyn.total_dyn_type1 & DynC.mDynType1_HostObject) !== 0) {
	  for ( var i = 0; i < this.dyn.elements.length; i++) {
	    if ( this.dyn.elements.get(i).dyn_type1 == DynC.mDynType1_HostObject) {
	      command = "open graph/class/instance=" + this.dyn.elements[i].hostobject;
	      break;
	    }
	  }
	}
      }

      this.dyn.graph.command( command);
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynOpenGraph : " + lines[i]);

      switch ( key) {
      case DynC.eSave_OpenGraph: 
	break;
      case DynC.eSave_OpenGraph_graph_object: 
	if ( tokens.length > 1)
	  this.graph_object = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynOpenGraph");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynOpenURL( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_OpenURL;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_OpenURL;
  this.instance_mask = 0;
  this.instance = 0;

  this.url;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var command = "open url \"" + this.url + "\"";

      this.dyn.graph.command(command);
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynOpenURL : " + lines[i]);

      switch ( key) {
      case DynC.eSave_OpenURL: 
	break;
      case DynC.eSave_OpenURL_url: 
	if ( tokens.length > 1)
	  this.url = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
        console.log( "Syntax error in DynOpenURL");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynInputFocus( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_InputFocus;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_InputFocus;
  this.instance_mask = 0;
  this.instance = 0;

  this.initial_focus;
  this.next_horizontal;
  this.next_vertical;
  this.next_tab;


  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( o, e) {
    return 1;
  };
  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynInputFocus : " + lines[i]);

      switch ( key) {
      case DynC.eSave_InputFocus: 
	break;
      case DynC.eSave_InputFocus_initial_focus: 
	this.initial_focus = parseInt(tokens[1], 10);
	break;
      case DynC.eSave_InputFocus_next_horizontal: 
	if ( tokens.length > 1)
	  this.next_horizontal = tokens[1];
	break;
      case DynC.eSave_InputFocus_next_vertical: 
	if ( tokens.length > 1)
	  this.next_vertical = tokens[1];
	break;
      case DynC.eSave_InputFocus_next_tab: 
	if ( tokens.length > 1)
	  this.next_tab = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynInputFocus");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynCloseGraph( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_CloseGraph;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_CloseGraph;
  this.instance_mask = 0;
  this.instance = 0;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( object) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      this.dyn.repaintNow = true;
      break;
    case Glow.eEvent_MB1Click:
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;

      this.dyn.graph.closeGraph();
      break;
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynCloseGraph : " + lines[i]);

      switch ( key) {
      case DynC.eSave_CloseGraph: 
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynCloseGraph");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };
}

function DynSlider( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_Slider;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_Slider;
  this.instance_mask = 0;
  this.instance = 0;

  this.a = null;
  this.maxvalue_a;
  this.minvalue_a;
  this.insensitive_a;
  this.attribute = null;
  this.minvalue_attr = null;
  this.maxvalue_attr = null;
  this.insensitive_attr = null;
  this.release_attr = null;
  this.old_value;
  this.old_ivalue;
  this.firstScan = true;
  this.min_value;
  this.max_value;
  this.slider_disabled = true;
  this.direction;

  this.connect = function( object) {
    if ( this.attribute.trim() === "")
      return 0;

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("FillLevel: " + attribute);
      return 1;
    }

    // Get min and max position from slider background
    var max_value, min_value, max_pos, min_pos;
    var background;
    var origo;

    if ( !object.transformIsStored()) {
      object.storeTransform();
      var g = object.measure();
      var info = object.get_info();
      var b = this.dyn.graph.getCtx().getBackgroundObjectLimits(DynC.mDynType1_SliderBackground,
								(g.ll_x + g.ur_x) / 2, (g.ll_y + g.ur_y) / 2);
      if ( (b.sts & 1) !== 0) {
	this.direction = b.direction;
	origo = object.get_origo(info.direction);

	switch( this.direction) {
	case Glow.eDirection_Down:
	  info.max_position = b.max - origo;
	  info.min_position = b.min - origo;
	  object.set_info(info);

	  object.move_to(g.ll_x, info.min_position);
	  break;
	case Glow.eDirection_Up:
	  info.max_position = b.max - (g.ur_y - g.ll_y - origo);
	  info.min_position = b.min - (g.ur_y - g.ll_y - origo);
	  object.set_info(info);
	  object.move_to(g.ll_x, info.min_position);
	  break;
	case Glow.eDirection_Left:
	  info.max_position = b.max - (g.ur_x - g.ll_x - origo);
	  info.min_position = b.min - (g.ur_x - g.ll_x - origo);
	  object.set_info(info);
	  object.move_to(info.min_position, g.ll_y);
	  break;
	case Glow.eDirection_Right:
	  info.max_position = b.max - origo;
	  info.min_position = b.min - origo;
	  object.set_info(info);
	  object.move_to(info.min_position, g.ll_y);
	  break;
	default:
	  ;
	}
      }
      else
	this.direction = info.direction;

      object.storeTransform();
    }
	    
    this.maxvalue_a = new DynReference( this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    this.minvalue_a = new DynReference( this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.insensitive_a = new DynReference( this.dyn, this.insensitive_attr);
    this.insensitive_a.connect(this.dyn);

    return 1;
  }

  this.disconnect = function() {
    if ( this.a == null || !this.a.sts)
      return;
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
    this.insensitive_a.disconnect(this.dyn);
  }
	
  this.scan = function( object) {
    if ( this.a == null || !this.a.sts)
      return;
	    
    var value = 0;
    var ivalue = 0;

    switch ( this.a.typeid) {
    case Pwr.eType_Float32:
      value = this.a.get_ref_value(this.dyn);
      if ( typeof value == 'undefined')
	value = 0;
      break;
    case Pwr.eType_Int32:
      ivalue = this.a.get_ref_value(this.dyn);
      if ( typeof ivalue == 'undefined')
	ivalue = 0;
      break;
    case Pwr.eType_Boolean: {
      var b;
      b = this.a.get_ref_value(this.dyn);
      if ( typeof b == 'undefined')
	b = false;
      ivalue = b ? 1 : 0;
      break;
    }
    }

    if ( this.insensitive_a.sts) {
      this.slider_disabled = this.insensitive_a.get_ref_value(this.dyn);
      if ( this.insensitive_a.inverted)
	this.slider_disabled = !this.slider_disabled;
    }
    else
      this.slider_disabled = false;
	    
    if ( this.maxvalue_a.sts && this.minvalue_a.sts) {
      this.max_value = this.maxvalue_a.get_ref_value(this.dyn);
      this.min_value = this.minvalue_a.get_ref_value(this.dyn);
      if ( this.max_value != this.maxvalue_a.oldValue ||
	   this.min_value != this.minvalue_a.oldValue) {
	if ( Math.abs( this.max_value - this.min_value) > Number.MIN_VALUE)
	  this.firstScan = true;
      }
      this.minvalue_a.oldValue = this.min_value;
      this.maxvalue_a.oldValue = this.max_value;
    }      
	    
    if ( !this.firstScan) {
      switch ( this.a.typeid) {
	case Pwr.eType_Float32:
	if ( Math.abs( this.old_value - value) < Number.MIN_VALUE)
	  // No change since last time
	  return;
	break;
	case Pwr.eType_Int32:
	case Pwr.eType_Boolean:
	if ( ivalue == this.old_ivalue)
	  return;
	break;
	default: ;
      }
    }
    else
      this.firstScan = false;

    switch ( this.a.typeid) {
    case Pwr.eType_Float32:
      this.old_value = value;
      break;
    case Pwr.eType_Int32:
    case Pwr.eType_Boolean:
      this.old_ivalue = ivalue;
      break;
    default: ;
    }

    var info = object.get_info();

    if ( !(this.maxvalue_a.sts && this.minvalue_a.sts && this.max_value != this.min_value)) {
      this.max_value = info.max_value;
      this.min_value = info.min_value;
    }
    if ( info.min_position != info.max_position) {
      if ( this.dyn.graph.getCurrentSlider() != object &&
	   this.max_value != this.min_value) {
	var pos_x, pos_y;

	switch ( this.a.typeid) {
	case Pwr.eType_Float32:
	  break;
	default:
	  value = ivalue;
	  break;
	}
          
	switch ( this.direction) {
	case Glow.eDirection_Down:
	  pos_y = (this.max_value - value) / (this.max_value - this.min_value) *
	  (info.max_position - info.min_position);
	  if ( pos_y < 0)
	    pos_y = 0;
	  else if ( pos_y > info.max_position - info.min_position)
	    pos_y = info.max_position - info.min_position;
	  pos_x = 0;
	  break;
	case Glow.eDirection_Right:
	  pos_x = info.max_position - info.min_position - 
	  (value - this.min_value) / (this.max_value - this.min_value) *
	  (info.max_position - info.min_position);
	  if ( pos_x < 0)
	    pos_x = 0;
	  else if ( pos_x > info.max_position - info.min_position)
	    pos_x = info.max_position - info.min_position;
	  pos_y = 0;
	  break;
	case Glow.eDirection_Left:
	  pos_x = info.max_position - info.min_position - 
	    (this.max_value - value) / (this.max_value - this.min_value) *
	    (info.max_position - info.min_position);
	  if ( pos_x < 0)
	    pos_x = 0;
	  else if ( pos_x > info.max_position - info.min_position)
	    pos_x = info.max_position - info.min_position;
	  pos_y = 0;
	  break;
	default:   // Up
	  pos_y = (value - this.min_value) / (this.max_value - this.min_value) *
	  (info.max_position - info.min_position);
	  if ( pos_y < 0)
	    pos_y = 0;
	  else if ( pos_y > info.max_position - info.min_position)
	    pos_y = info.max_position - info.min_position;
	  pos_x = 0;
	}
	object.set_position(pos_x, pos_y);
      }
    }

  };

  this.action = function( object, e) {
    if ( this.slider_disabled)
      return 1;

    switch ( e.event) {
    case Glow.eEvent_SliderMoveEnd: {
      var pname = this.dyn.parseAttrName( this.release_attr);
      if ( !(pname === null || pname.name === "")) {
	var sts = null;
	switch ( pname.type) {
	case Pwr.eType_Boolean: {
	  switch (pname.database) {
	  case GraphIfc.eDatabase_Gdh:
	    sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, 1);
	    break;
	  case GraphIfc.eDatabase_Local:
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, 1);
	    break;
	  }
	  break;
	}
	default: ;
	}
      }
      if ( this.dyn.graph.getCurrentSlider() == object)
	this.dyn.graph.setCurrentSlider(null);
      break;
    }
    case Glow.eEvent_SliderMoveStart: {
      if ( !this.dyn.graph.isAuthorized( this.dyn.access) ||
	   this.slider_disabled) {
	this.dyn.graph.getCtx().setMoveRestrictions(Glow.eMoveRestriction_Disable, 0, 0, null);
	this.dyn.graph.setCurrentSlider(null);
	break;
      }
      var info = object.get_info();
      if ( this.direction == Glow.eDirection_Right || 
	   this.direction == Glow.eDirection_Left)
	this.dyn.graph.getCtx().setMoveRestrictions( Glow.eMoveRestriction_HorizontalSlider,
						info.max_position, info.min_position, e.object);
      else
	this.dyn.graph.getCtx().setMoveRestrictions( Glow.eMoveRestriction_VerticalSlider,
						info.max_position, info.min_position, e.object);

      this.dyn.graph.setCurrentSlider( object);
      break;
    }
    case Glow.eEvent_SliderMoved: {
      var 		value;
    
      var info = object.get_info();
      if ( info.min_position != info.max_position) {
	if ( !(this.max_value_p !== 0 && this.min_value_p !== 0 && this.max_value != this.min_value)) {
	  this.max_value = info.max_value;
	  this.min_value = info.min_value;
	}
	var g = object.measure();
        
	switch ( this.direction) {
	case Glow.eDirection_Down:
	  value = ( (info.max_position - g.ll_y) / (info.max_position - info.min_position) *
			   (this.max_value - this.min_value) + this.min_value);
	  break;
	case Glow.eDirection_Right:
	  value = ( (info.max_position - g.ll_x) / (info.max_position - info.min_position) *
			   (this.max_value - this.min_value) + this.min_value);
	  break;
	case Glow.eDirection_Left:
	  value = ( (g.ll_x - info.min_position) / (info.max_position - info.min_position) *
			   (this.max_value - this.min_value) + this.min_value);
	  break;
	default:
	  value = ( (g.ll_y - info.min_position) / (info.max_position - info.min_position) *
			   (this.max_value - this.min_value) + this.min_value);
	}
	if ( value > this.max_value)
	  value = this.max_value;
	if ( value < this.min_value)
	  value = this.min_value;
      
	var pname = this.dyn.parseAttrName( this.attribute);
	if ( pname === null || pname.name === "") 
	  return 1;

	var sts;
	switch ( pname.type) {
	case Pwr.eType_Float32:
	  switch (pname.database) {
	  case GraphIfc.eDatabase_Gdh:
	    sts = this.dyn.graph.getGdh().setObjectInfoFloat( pname.name, value);
	    break;
	  case GraphIfc.eDatabase_Local:	    
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	    break;
	  }
	  break;
	case Pwr.eType_Boolean: {
	  var bvalue = (value > 0.5 ? true : false);
	  switch (pname.database) {
	  case GraphIfc.eDatabase_Gdh:
	    sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, bvalue);
	    break;
	  case GraphIfc.eDatabase_Local:
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, bvalue);
	    break;
	  }
	  break;
	}
	default: {
	  var ivalue = Math.floor (value > 0 ? value + 0.5 : value - 0.5);
	  switch (pname.database) {
	  case GraphIfc.eDatabase_Gdh:
	    sts = this.dyn.graph.getGdh().setObjectInfoInt( pname.name, ivalue);
	    break;
	  case GraphIfc.eDatabase_Local:
	    console.log("Set ivalue", ivalue, pname.name);
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, ivalue);
	    break;
	  }
	}
	}
	if ( sts.evenSts()) console.log("Slider error: " + this.attribute);
      }
      break;
    }
    default: ;    
    }
    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynSlider : " + lines[i]);

      switch ( key) {
      case DynC.eSave_Slider: 
	break;
      case DynC.eSave_Slider_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_Slider_minvalue_attr: 
	if ( tokens.length > 1)
	  this.minvalue_attr = tokens[1];
	break;
      case DynC.eSave_Slider_maxvalue_attr: 
	if ( tokens.length > 1)
	  this.maxvalue_attr = tokens[1];
	break;
      case DynC.eSave_Slider_insensitive_attr: 
	if ( tokens.length > 1)
	  this.insensitive_attr = tokens[1];
	break;
      case DynC.eSave_Slider_release_attr: 
	if ( tokens.length > 1)
	  this.release_attr = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynSlider");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynFastCurve( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_FastCurve;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_FastCurve;
  this.instance_mask = 0;
  this.instance = 0;

  this.fast_object;
  this.curve_index1;
  this.curve_index2;

  this.connect = function( object) {
  };
  this.disconnect = function() {
  };
  this.scan = function( object) {
  };
  this.action = function( object, e) {
  };

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynSlider : " + lines[i]);

      switch ( key) {
      case DynC.eSave_FastCurve: 
	break;
      case DynC.eSave_FastCurve_fast_object: 
	if ( tokens.length > 1)
	  this.fast_object = tokens[1];
	break;
      case DynC.eSave_FastCurve_curve_index1: 
	this.curve_index1 = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_FastCurve_curve_index2: 
	this.curve_index2 = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynFastCurve");
	break;
      }
      
      if ( end)
	break;
    }
    return i;
  };  
}

function DynPulldownMenu( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_PulldownMenu;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_PulldownMenu;
  this.instance_mask = 0;
  this.instance = 0;

  this.button_mask;
  this.items_text = new Array(32);
  this.items_dyn = new Array(32);
  this.menu_object = null;
  this.text_size;

  for ( var i = 0; i < 32; i++)
    this.items_dyn[i] = null;

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      if ( this.menu_object !== null) {
	// Close, delete this menu and all childmenues
	for ( var j = 0; j < 32; j++) {
	  if ( this.items_dyn[j] !== null &&
	       (this.items_dyn[j].action_type1 & DynC.mActionType1_PulldownMenu) !== 0)
	    this.items_dyn[j].action( null, e);
	}
	this.dyn.graph.getCtx().remove(this.menu_object);
	this.menu_object = null;
      }
      else if ( object !== null) {
	var ll_x, ll_y, ur_x, ur_y;
	var info = new GlowMenuInfo();

	var b_mask = 1;
	for ( var i = 0; i < 32; i++) {
	  if ( (b_mask & this.button_mask) != 0) {
	    info.item[i].occupied = true;
	    info.item[i].text = this.items_text[i];
			    
	    // Check access
	    if ( (this.items_dyn[i].action_type1 & DynC.mActionType1_PulldownMenu) !== 0)
	      info.item[i].type = Glow.eMenuItem_PulldownMenu;
	    else {
	      // Check access
	      if ( this.dyn.graph.isAuthorized( this.items_dyn[i].access)) {
		info.item[i].type = Glow.eMenuItem_Button;
		if ( (this.items_dyn[i].dyn_type1 & DynC.mDynType1_Invisible) !== 0) {
		  var sts;
		  var invis_element = null;
		  var command;

		  for ( var j = 0; j < this.dyn.elements.length; j++) {
		    if ( this.dyn.elements[j].dyn_type1 == DynC.mDynType1_Invisible) {
		      invis_element = this.dyn.elements[j];
		    }
		  }
		  if ( invis_element !== null) {
		    var pname = this.dyn.parseAttrName( invis_element.attribute);
		    if ( (pname !== null) && pname.name !== "") {
		      if ( pname.name.substring(0, 5) == "$cmd(") {
			var idx = pname.name.lastIndexOf(")");
			if ( idx != -1) {
			  command = pname.name.substring(5, idx);
			  command = this.dyn.graph.getCommand(command);
			  sts = this.dyn.graph.command(command);
			  if ( (sts & 1) == 0)
			    info.item[i].type = Glow.eMenuItem_ButtonDisabled;
			}
		      }
		    }
		    else {
		      /** TODO
		      this.dyn.graph.getGdh().getObjectInfoBoolean( pname.name, this.action2, data);
		      if (ret.oddSts()) {
			if ( (!pname.inverted && ret.value) || (pname.inverted && !ret.value)) {
			  if ( invis_element.dimmed != 0) 
			    info.item[i].type = Glow.eMenuItem_ButtonDisabled;
			  else
			    info.item[i].occupied = false;
			}		      
		      }
		      **/
		    }
		  }
		}
	      }
	      else
		info.item[i].type = Glow.eMenuItem_ButtonDisabled;
	    }
	  }
	  else
	    info.item[i].occupied = false;
	  b_mask = b_mask << 1;
	}

	// Get fillcolor, and textattributes from object
	var text_drawtype = 0, text_color = 0, bg_color = 0;
	var text_size = 0;
	var sts;
	var scale = 1;
	var text_font = 0;
		    
	sts = 0; // TODO sts = grow_GetObjectAnnotInfo( object, 1, &text_size, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
	if ( (sts & 1) == 0) {
	  text_size = 3;
	  text_drawtype = Glow.eDrawType_TextHelvetica;
	  text_font = Glow.eFont_LucidaSans;
	  text_color = Glow.eDrawType_Line;
	  bg_color = Glow.eDrawType_LightGray;
	}
	else if ( bg_color == Glow.eDrawType_No || bg_color == Glow.eDrawType_Inherit)
	  bg_color = Glow.eDrawType_LightGray;
		    
	var g = object.measure();
	this.menu_object = new GrowMenu( this.dyn.graph.getCtx());
	this.menu_object.init( "__Menu", info, g.ll_x, g.ur_y, g.ur_x - g.ll_x,
			       Glow.eDrawType_Line, 0, 1, 1, bg_color, text_size,
			       text_drawtype, text_color,
			       Glow.eDrawType_MediumGray, text_font);
	this.menu_object.set_scale( scale, scale, 0, 0, Glow.eScaleType_LowerLeft);
	this.dyn.graph.getCtx().insert( this.menu_object);

	// grow_SetMenuInputFocus( menu_object, 1);
      }

      break;
    case Glow.eEvent_MenuActivated:
      if ( this.menu_object == null)
	break;
      if ( e.object == this.menu_object) {
	if ( this.items_dyn[e.item] !== null) {
	  var event = new GlowEvent();
	  event.event = Glow.eEvent_MB1Click;
	  return this.items_dyn[e.item].action( e.object, event);
	}
      }
      else {
	for ( var j = 0; j < 32; j++) {
	  if ( this.items_dyn[j] !== null && 
	       (items_dyn[j].action_type1 & DynC.mActionType1_PulldownMenu) !== 0)
	    this.items_dyn[j].action( null, e);
	}
      }
      break;
    case Glow.eEvent_MenuDelete:
      if ( this.menu_object == null)
	break;
      if ( e.object == null || e.object == this.menu_object) {
	// Delete this menu
	this.dyn.graph.getCtx().remove(this.menu_object);
	this.menu_object = null;
      }
      break;

    default: ;
    }
    return 1;
  }

  this.connect = function( object) {
  };
  this.disconnect = function() {
  };

  this.scan = function( object) {
  };

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynPulldownMenu : " + lines[i]);

      switch ( key) {
      case DynC.eSave_PulldownMenu: 
	break;
      case DynC.eSave_PulldownMenu_button_mask: 
	this.button_mask = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_PulldownMenu_items_text0: 
	if ( tokens.length > 1)
	  this.items_text[0] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text1: 
	if ( tokens.length > 1)
	  this.items_text[1] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text2: 
	if ( tokens.length > 1)
	  this.items_text[2] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text3: 
	if ( tokens.length > 1)
	  this.items_text[3] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text4: 
	if ( tokens.length > 1)
	  this.items_text[4] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text5: 
	if ( tokens.length > 1)
	  this.items_text[5] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text6: 
	if ( tokens.length > 1)
	  this.items_text[6] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text7: 
	if ( tokens.length > 1)
	  this.items_text[7] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text8: 
	if ( tokens.length > 1)
	  this.items_text[8] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text9: 
	if ( tokens.length > 1)
	  this.items_text[9] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text10: 
	if ( tokens.length > 1)
	  this.items_text[10] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text11: 
	if ( tokens.length > 1)
	  this.items_text[11] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text12: 
	if ( tokens.length > 1)
	  this.items_text[12] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text13: 
	if ( tokens.length > 1)
	  this.items_text[13] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text14: 
	if ( tokens.length > 1)
	  this.items_text[14] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text15: 
	if ( tokens.length > 1)
	  this.items_text[15] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text16: 
	if ( tokens.length > 1)
	  this.items_text[16] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text17: 
	if ( tokens.length > 1)
	  this.items_text[17] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text18: 
	if ( tokens.length > 1)
	  this.items_text[18] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text19: 
	if ( tokens.length > 1)
	  this.items_text[19] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text20: 
	if ( tokens.length > 1)
	  this.items_text[20] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text21: 
	if ( tokens.length > 1)
	  this.items_text[21] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text22: 
	if ( tokens.length > 1)
	  this.items_text[22] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text23: 
	if ( tokens.length > 1)
	  this.items_text[23] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text24: 
	if ( tokens.length > 1)
	  this.items_text[24] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text25: 
	if ( tokens.length > 1)
	  this.items_text[25] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text26: 
	if ( tokens.length > 1)
	  this.items_text[26] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text27: 
	if ( tokens.length > 1)
	  this.items_text[27] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text28: 
	if ( tokens.length > 1)
	  this.items_text[28] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text29: 
	if ( tokens.length > 1)
	  this.items_text[29] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text30: 
	if ( tokens.length > 1)
	  this.items_text[30] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_text31: 
	if ( tokens.length > 1)
	  this.items_text[31] = tokens[1];
	break;
      case DynC.eSave_PulldownMenu_items_dyn0: 
	this.items_dyn[0] = new Dyn(dyn.graph);
	i = this.items_dyn[0].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn1: 
	this.items_dyn[1] = new Dyn(dyn.graph);
	i = this.items_dyn[1].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn2:
	this.items_dyn[2] = new Dyn(dyn.graph);
	i = this.items_dyn[2].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn3: 
	this.items_dyn[3] = new Dyn(dyn.graph);
	i = this.items_dyn[3].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn4: 
	this.items_dyn[4] = new Dyn(dyn.graph);
	i = this.items_dyn[4].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn5: 
	this.items_dyn[5] = new Dyn(dyn.graph);
	i = this.items_dyn[5].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn6: 
	this.items_dyn[6] = new Dyn(dyn.graph);
	i = this.items_dyn[6].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn7: 
	this.items_dyn[7] = new Dyn(dyn.graph);
	i = this.items_dyn[7].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn8: 
	this.items_dyn[8] = new Dyn(dyn.graph);
	i = this.items_dyn[8].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn9: 
	this.items_dyn[9] = new Dyn(dyn.graph);
	i = this.items_dyn[9].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn10: 
	this.items_dyn[10] = new Dyn(dyn.graph);
	i = this.items_dyn[10].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn11: 
	this.items_dyn[11] = new Dyn(dyn.graph);
	i = this.items_dyn[11].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn12: 
	this.items_dyn[12] = new Dyn(dyn.graph);
	i = this.items_dyn[12].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn13: 
	this.items_dyn[13] = new Dyn(dyn.graph);
	i = this.items_dyn[13].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn14: 
	this.items_dyn[14] = new Dyn(dyn.graph);
	i = this.items_dyn[14].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn15: 
	this.items_dyn[15] = new Dyn(dyn.graph);
	i = this.items_dyn[15].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn16: 
	this.items_dyn[16] = new Dyn(dyn.graph);
	i = this.items_dyn[16].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn17: 
	this.items_dyn[17] = new Dyn(dyn.graph);
	i = this.items_dyn[17].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn18: 
	this.items_dyn[18] = new Dyn(dyn.graph);
	i = this.items_dyn[18].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn19: 
	this.items_dyn[19] = new Dyn(dyn.graph);
	i = this.items_dyn[19].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn20: 
	this.items_dyn[20] = new Dyn(dyn.graph);
	i = this.items_dyn[20].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn21: 
	this.items_dyn[21] = new Dyn(dyn.graph);
	i = this.items_dyn[21].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn22: 
	this.items_dyn[22] = new Dyn(dyn.graph);
	i = this.items_dyn[22].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn23: 
	this.items_dyn[23] = new Dyn(dyn.graph);
	i = this.items_dyn[23].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn24: 
	this.items_dyn[24] = new Dyn(dyn.graph);
	i = this.items_dyn[24].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn25: 
	this.items_dyn[25] = new Dyn(dyn.graph);
	i = this.items_dyn[25].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn26: 
	this.items_dyn[26] = new Dyn(dyn.graph);
	i = this.items_dyn[26].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn27: 
	this.items_dyn[27] = new Dyn(dyn.graph);
	i = this.items_dyn[27].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn28: 
	this.items_dyn[28] = new Dyn(dyn.graph);
	i = this.items_dyn[28].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn29: 
	this.items_dyn[29] = new Dyn(dyn.graph);
	i = this.items_dyn[29].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn30: 
	this.items_dyn[30] = new Dyn(dyn.graph);
	i = this.items_dyn[30].open(lines, i+1);
	break;
      case DynC.eSave_PulldownMenu_items_dyn31: 
	this.items_dyn[31] = new Dyn(dyn.graph);
	i = this.items_dyn[31].open(lines, i+1);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynPulldownMenu");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };
}

function DynOptionMenu( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_OptionsMenu;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_OptionsMenu;
  this.instance_mask = 0;
  this.instance = 0;

  this.a;
  this.update_a = null;
  this.attribute;
  this.text_attribute;
  this.size_attribute;
  this.update_attribute;
  this.optionmenu_type;
  this.button_mask;
  this.items_text = new Array(32);
  this.items_enum = new Array(32);
  this.firstScan = true;
  this.oldValueF;
  this.oldValueI;
  this.menu_object = null;
  this.text_size;

  this.connect = function( object) {

    this.a = new DynReference( this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if ( !this.a.sts) {
      console.log("OptionMenu: " + this.attribute);
      return 1;
    }

    if ( this.text_attribute !== null && this.text_attribute !== "" && 
	 this.update_attribute != null && this.update_attribute !== "") {

      this.update_a = new DynReference( this.dyn, this.update_attribute);
      this.update_a.connect(this.dyn);
      if ( !this.update_a.sts) {
	console.log("OptionsMenu: " + this.update_attribute);
	return 1;
      }
    }
    return 1;
  };

  this.disconnect = function() {
    this.a.disconnect(this.dyn);
    if ( this.update_a != null)
      this.update_a.disconnect(this.dyn);
  };

  this.scan = function( object) {
    if ( !this.a.sts)
      return;

    var update_texts = false;
    if ( this.update_a !== null && this.update_a.sts) {

      var value = this.dyn.getDig( this.update_a.p, this.update_a.typeid, this.update_a.bitmask, this.update_a.database);

      if ( this.update_a.inverted)
	value = !value;

      if ( !this.firstScan) {
	if ( this.update_a.oldValue != value)
	  update_texts = true;
      }
      else {
	update_texts = false;
      }
      this.update_a.oldValue = value;
    }


    if ( update_texts) {
    }

    var enum_value = 0;
    switch ( this.a.typeid) {
    case Pwr.eType_Float32: {
      var value = this.a.get_ref_value(this.dyn);
      var i;

      if ( !this.firstScan) {
	if ( value == this.oldValueF) {
	  return;
	}
      }
      else
	this.firstScan = false;

      enum_value = Math.floor( value + 0.5);
      this.oldValueF = value;

      break;
    }
    case Pwr.eType_UInt32:
    case Pwr.eType_Int32: {
      var value = this.a.get_ref_value(this.dyn);
      var i;

      if ( !this.firstScan) {
	if ( value == this.oldValueI) {
	  return;
	}
      }
      else
	this.firstScan = false;

      enum_value = value;
      this.oldValueI = value;

      break;
    }
    default:
      return;
    }
	    
    if ( this.update_a === null || !this.update_a.sts) {
      var found = false;
      for ( var i = 0; i < 32; i++) {
	if ( this.items_enum[i] == enum_value) {
	  object.setAnnotation(1, this.items_text[i]);
	  found = true;
	  break;
	}
      }
      if ( !found)
	object.setAnnotation(1, "");
    }
    else {
    }

  }

  this.action = function( object, e) {

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      if ( this.menu_object !== null) {
	// Close, delete this menu
	this.dyn.graph.getCtx().remove( this.menu_object);
	this.menu_object = null;
      }
      else {
	var	ll_x, ll_y, ur_x, ur_y;
	var info = new GlowMenuInfo();

	if ( this.update_a === null || !this.update_a.sts) {
	  var b_mask = 1;
	  for ( var i = 0; i < 32; i++) {
	    if ( (b_mask & this.button_mask) !== 0) {
	      info.item[i].occupied = true;
	      info.item[i].text = this.items_text[i];
	    
	      // Check access
	      info.item[i].type = Glow.eMenuItem_Button;
	    }
	    else
	      info.item[i].occupied = false;
	    b_mask = b_mask << 1;
	  }
	}
	else {
	  for ( var i = 0; i < 32; i++) {
	    if ( i < this.text_size) {
	      info.item[i].text = this.items_text[i];
	      info.item[i].type = Glow.eMenuItem_Button;
	      info.item[i].occupied = true;
	    }
	    else
	      info.item[i].occupied = false;
	  }
	}

	// Get fillcolor, and textattributes from object
	var text_drawtype = 4, text_color = Glow.eDrawType_Line, bg_color = Glow.eDrawType_Color32;
	var tsize = 2;
	var sts;
	var scale = 1;
	var text_font = Glow.eFont_Helvetica;

	sts = 0; //grow_GetObjectAnnotInfo( object, 1, &tsize, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
	if ( (sts & 1) == 0) {
	  tsize = 2;
	  text_drawtype = Glow.eDrawType_TextHelveticaBold;
	  text_color = Glow.eDrawType_Line;
	  bg_color = Glow.eDrawType_LightGray;
	}
	else if ( bg_color == Glow.eDrawType_No || bg_color == Glow.eDrawType_Inherit)
	  bg_color = Glow.eDrawType_LightGray;
	
	var g = object.measure();
	this.menu_object = new GrowMenu( this.dyn.graph.getCtx());
	this.menu_object.init( "__Menu", info, g.ll_x, g.ur_y, g.ur_x - g.ll_x,
			       Glow.eDrawType_Line, 0, 1, 1, bg_color, tsize,
			       text_drawtype, text_color,
			       Glow.eDrawType_MediumGray, text_font);
	this.menu_object.set_scale( scale, scale, 0, 0, Glow.eScaleType_LowerLeft);
	this.dyn.graph.getCtx().insert( this.menu_object);
      }
      break;
    case Glow.eEvent_MenuActivated:
      if ( this.menu_object === null)
	break;
      if ( e.object == this.menu_object) {
	// Set enum value to attribute
      
	var pname = this.dyn.parseAttrName( this.attribute);
	if ( pname === null || pname.name === "") 
	  return 1;

	var sts = null;

	switch ( pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  switch ( pname.type) {
	  case Pwr.eType_Float32: {
	    var value = this.items_enum[e.item];
	    sts = this.dyn.graph.getGdh().setObjectInfoFloat( pname.name, value);
	    break;
	  }
	  case Pwr.eType_Int32:
	  case Pwr.eType_UInt32:
	  case Pwr.eType_Int16:
	  case Pwr.eType_UInt16:
	  case Pwr.eType_Int8:
	  case Pwr.eType_UInt8: {
	    var value = this.items_enum[e.item];
	    sts = this.dyn.graph.getGdh().setObjectInfoInt( pname.name, value);
	    break;
	  }
	  default: ;
	  }
	  break;
	case GraphIfc.eDatabase_Local:
	  switch ( pname.type) {
	  case Pwr.eType_Float32: {
	    var value = this.items_enum[e.item];
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	    break;
	  }
	  case Pwr.eType_Int32:
	  case Pwr.eType_UInt32:
	  case Pwr.eType_Int16:
	  case Pwr.eType_UInt16:
	  case Pwr.eType_Int8:
	  case Pwr.eType_UInt8: {
	    var value = this.items_enum[e.item];
	    sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, value);
	    break;
	  }
	  default: ;
	  }
	  break;
	case GraphIfc.eDatabase_Ccm: {
	  /* TODO
	     pwr_tInt32 value = items_enum[event->menu.item];
	     sts = dyn->graph->ccm_set_variable( parsed_name, type_id, &value);
	  */
	  break;
	}
	default : ;
	}

	if ( sts == null || sts.evenSts()) console.log("Option menu error: " + pname.name);
      }
      break;
    case Glow.eEvent_MenuDelete:
      if ( this.menu_object == null)
	break;
      if ( e.object === null || e.object == this.menu_object) {
	// Delete this menu
	this.dyn.graph.getCtx().remove( this.menu_object);
	this.menu_object = null;
      }
      break;
    default: ;
    }

    return 1;
  }

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "OptionsMenu : " + lines[i]);

      switch ( key) {
      case DynC.eSave_OptionMenu: 
	break;
      case DynC.eSave_OptionMenu_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_OptionMenu_text_attribute: 
	if ( tokens.length > 1)
	  this.text_attribute = tokens[1];
	break;
      case DynC.eSave_OptionMenu_size_attribute: 
	if ( tokens.length > 1)
	  this.size_attribute = tokens[1];
	break;
      case DynC.eSave_OptionMenu_update_attribute: 
	if ( tokens.length > 1)
	  this.update_attribute = tokens[1];
	break;
      case DynC.eSave_OptionMenu_optionmenu_type: 
	this.optionmenu_type = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_button_mask: 
	this.button_mask = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_text0: 
	if ( tokens.length > 1)
	  this.items_text[0] = tokens[1];
	break;
      case DynC.eSave_OptionMenu_items_text1: 
	if ( tokens.length > 1)
	  this.items_text[1] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text2: 
	if ( tokens.length > 1)
	  this.items_text[2] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text3: 
	if ( tokens.length > 1)
	  this.items_text[3] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text4: 
	if ( tokens.length > 1)
	  this.items_text[4] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text5: 
	if ( tokens.length > 1)
	  this.items_text[5] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text6: 
	if ( tokens.length > 1)
	  this.items_text[6] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text7: 
	if ( tokens.length > 1)
	  this.items_text[7] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text8: 
	if ( tokens.length > 1)
	  this.items_text[8] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text9: 
	if ( tokens.length > 1)
	  this.items_text[9] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text10: 
	if ( tokens.length > 1)
	  this.items_text[10] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text11: 
	if ( tokens.length > 1)
	  this.items_text[11] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text12: 
	if ( tokens.length > 1)
	  this.items_text[12] = tokens[1];
	break;
      case DynC.eSave_OptionMenu_items_text13: 
	if ( tokens.length > 1)
	  this.items_text[13] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text14: 
	if ( tokens.length > 1)
	  this.items_text[14] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text15: 
	if ( tokens.length > 1)
	  this.items_text[15] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text16: 
	if ( tokens.length > 1)
	  this.items_text[16] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text17: 
	if ( tokens.length > 1)
	  this.items_text[17] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text18: 
	if ( tokens.length > 1)
	  this.items_text[18] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text19: 
	if ( tokens.length > 1)
	  this.items_text[19] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text20: 
	if ( tokens.length > 1)
	  this.items_text[20] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text21: 
	if ( tokens.length > 1)
	  this.items_text[21] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text22: 
	if ( tokens.length > 1)
	  this.items_text[22] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text23: 
	if ( tokens.length > 1)
	  this.items_text[23] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text24: 
	if ( tokens.length > 1)
	  this.items_text[24] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text25: 
	if ( tokens.length > 1)
	  this.items_text[25] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text26: 
	if ( tokens.length > 1)
	  this.items_text[26] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text27: 
	if ( tokens.length > 1)
	  this.items_text[27] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text28: 
	if ( tokens.length > 1)
	  this.items_text[28] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text29: 
	if ( tokens.length > 1)
	  this.items_text[29] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text30: 
	if ( tokens.length > 1)
	  this.items_text[30] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_text31: 
	if ( tokens.length > 1)
	  this.items_text[31] = lines[i].substring(5);
	break;
      case DynC.eSave_OptionMenu_items_enum0: 
	this.items_enum[0] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum1: 
	this.items_enum[1] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum2: 
	this.items_enum[2] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum3: 
	this.items_enum[3] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum4: 
	this.items_enum[4] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum5: 
	this.items_enum[5] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum6: 
	this.items_enum[6] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum7: 
	this.items_enum[7] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum8: 
	this.items_enum[8] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum9: 
	this.items_enum[9] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum10: 
	this.items_enum[10] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum11: 
	this.items_enum[11] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum12: 
	this.items_enum[12] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum13: 
	this.items_enum[13] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum14: 
	this.items_enum[14] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum15: 
	this.items_enum[15] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum16: 
	this.items_enum[16] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum17: 
	this.items_enum[17] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum18: 
	this.items_enum[18] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum19: 
	this.items_enum[19] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum20: 
	this.items_enum[20] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum21: 
	this.items_enum[21] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum22: 
	this.items_enum[22] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum23: 
	this.items_enum[23] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum24: 
	this.items_enum[24] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum25: 
	this.items_enum[25] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum26: 
	this.items_enum[26] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum27: 
	this.items_enum[27] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum28: 
	this.items_enum[28] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum29: 
	this.items_enum[29] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum30: 
	this.items_enum[30] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_OptionMenu_items_enum31: 
	this.items_enum[31] = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynOptionMenu");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

}

function DynAnalogText( dyn) {
  DynOptionMenu.call( this, dyn);
  this.optionmenu_open = this.open;

  this.dyn = dyn;
  this.dyn_type1 = DynC.mDynType1_AnalogText;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_AnalogText;
  this.instance_mask = 0;
  this.instance = 0;

  this.action = function( o, e) {
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "AnalogText : " + lines[i]);

      switch ( key) {
      case DynC.eSave_AnalogText: 
	break;
      case DynC.eSave_AnalogText_super: 
	i = this.optionmenu_open( lines, i+1);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynAnalogText");
	break;
      }
      
      if ( end)
	break;
    }
		
    return i;
  };

}

DynAnalogText.prototype = Object.create(DynOptionMenu.prototype);


function  DynSetValue( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_SetValue;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_SetValue;
  this.instance_mask = 0;
  this.instance = 0;

  this.attribute;
  this.value;

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "AnalogText : " + lines[i]);

      switch ( key) {
      case DynC.eSave_SetValue: 
	break;
      case DynC.eSave_SetValue_attribute: 
	if ( tokens.length > 1)
	  this.attribute = tokens[1];
	break;
      case DynC.eSave_SetValue_value: 
	if ( tokens.length > 1)
	  this.value = tokens[1];
	break;
      case DynC.eSave_SetValue_instance: 
	this.instance = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_SetValue_instance_mask: 
	this.instance_mask = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynSetValue");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.connect = function( object) {
  };
  this.disconnect = function() {
  };
  this.scan = function( object) {
  };
  this.action = function( object, e) {

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      break;
    case Glow.eEvent_MB1Click: {
      var sts;
		
      if ( (this.dyn.action_type1 & DynC.mActionType1_Confirm) != 0)
	break;
		
      var pname = this.dyn.parseAttrName( this.attribute);
		
      switch ( pname.type) {
      case Pwr.eType_Float32: {
	var inputValue = parseFloat( this.value.trim());
		    
	switch ( pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  sts = this.dyn.graph.getGdh().setObjectInfoFloat( pname.name, inputValue);
	  break;
	case GraphIfc.eDatabase_Local:
	  sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, inputValue);
	  break;
	default: 
	  return 0;
	}
	if ( sts.evenSts()) {			
	  console.log( "setObjectInfoError " + sts);
	  return 0;
	}
	break;
      }
      case Pwr.eType_Int32:
      case Pwr.eType_UInt32:
      case Pwr.eType_Int16:
      case Pwr.eType_UInt16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt8:
      case Pwr.eType_Mask:
      case Pwr.eType_Enum: {
	var inputValue = parseInt( this.value.trim(), 10);
		    
	switch ( pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  console.log("SetValue", pname.name, inputValue);
	  sts = this.dyn.graph.getGdh().setObjectInfoInt( pname.name, inputValue);
	  break;
	case GraphIfc.eDatabase_Local:
	  sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, inputValue);
	  break;
	default: 
	  return 0;
	}
	if ( sts.evenSts()) {
	  console.log( "setObjectInfoError " + sts);
	  return 0;
	}		
	break;
      }
      case Pwr.eType_Boolean: {
	var inputValueInt = parseInt( this.value.trim(), 10);
	var inputValue;
	if ( inputValueInt == 0)
	  inputValue = false;
	else if ( inputValueInt == 1)
	  inputValue = true;
	else
	  break;
	
	switch ( pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  sts = this.dyn.graph.getGdh().setObjectInfoBoolean( pname.name, inputValue);
	  break;
	case GraphIfc.eDatabase_Local:
	  sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, inputValue);
	  break;
	default:
	  return 0;
	}
	if ( sts.evenSts()) {
	  console.log( "setObjectInfoError " + sts);
	  return 0;
	}
	break;
      }
      case Pwr.eType_String: {
	switch ( pname.database) {
	case GraphIfc.eDatabase_Gdh:
	  sts = this.dyn.graph.getGdh().setObjectInfoString( pname.name, this.value);
	  break;
	case GraphIfc.eDatabase_Local:
	  sts = this.dyn.graph.getLdb().setObjectInfo( this.dyn.graph, pname.name, this.value);
	  break;
	default:
	  return 0;
	}
	if ( sts.evenSts()) {
	  console.log( "setObjectInfoError " + sts);
	  return 0;
	}
	break;
      }		    
      default:
	;
      }
	       
      break;
    }
    default:
      return 0;
    }
    return 1;
  };
}


function DynMethodToolbar( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_MethodToolbar;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_MethodToolbar;
  this.instance_mask = 0;
  this.instance = 0;

  this.method_object;
  this.toolbar_type;

  this.pname;
  this.xm_mask_flags = 0;
  this.xm_mask_opmethods = 0;
  this.xm_mask_mntmethods = 0;
  this.mask_configure = 0;
  this.mask_store = 0;
  this.o;

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "MethodToolbar : " + lines[i]);

      switch ( key) {
      case DynC.eSave_MethodToolbar: 
	break;
      case DynC.eSave_MethodToolbar_method_object: 
	if ( tokens.length > 1)
	  this.method_object = tokens[1];
	break;
      case DynC.eSave_MethodToolbar_toolbar_type: 
	this.toolbar_type = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
        console.log( "Syntax error in DynMethodToolbar");
	break;
      }

      if ( end)
	break;
    }
    return i;
  };

  this.connect = function( object) {
    this.o = object;

    var pname = this.dyn.parseAttrName( this.method_object);
    if ( pname === null || pname.name === "") 
      return 1;

    this.pname_name = pname.name;

    var parsed_name = this.pname_name + ".XttMethodsMask.Flags";
    this.dyn.graph.getGdh().getObjectInfoInt( parsed_name, this.connect2, this);
  };

  this.connect2 = function( id, self, sts, value) {
    if ( sts & 1) {
      self.xm_mask_flags = value;

      if ( (self.xm_mask_flags & Pwrb.mXttMethodsFlagsMask_IsConfigured) == 0) {		    
	self.mask_configure = 1;
	self.mask_store = 1;
	self.connect5();
      }      
      else {
	var parsed_name = self.pname_name + ".XttMethodsMask.OpMethods";
	dyn.graph.getGdh().getObjectInfoInt( parsed_name, self.connect3, self);
      }
    }
    else {
      self.mask_configure = 1;
      self.connect5();
    }
  };

  this.connect3 = function( id, self, sts, value) {
    if ( !(sts & 1)) {
      console.log("DynMethodToolbar: " + self.pname_name);
      self.mask_configure = 1;
    }
    else
      self.xm_mask_opmethods = value;
    var parsed_name = self.pname_name + ".XttMethodsMask.MntMethods";
    dyn.graph.getGdh().getObjectInfoInt( parsed_name, self.connect4, self);
  };

  this.connect4 = function( id, self, sts, value) {    
    if ( !(sts & 1)) {
      console.log("DynMethodToolbar: " + self.pname_name);
      self.mask_configure = 1;
    }
    else
      self.xm_mask_mntmethods = value;
    self.connect5();
  };
  

  this.connect5 = function() {
    var     	cmd, command;
    var		sts;
    
    if ( this.mask_configure != 0) {
      this.xm_mask_opmethods = 0;
      for ( var i = 0; i < DynC.method_toolbar_op_cnt; i++) {
	if ( DynC.method_toolbar_op_methods[i] == "")
	  continue;
	if ( DynC.method_toolbar_op_methods[i] == "Parent Object Graph") {
	  if ( this.method_object.indexOf('.') != -1)
	    this.xm_mask_opmethods |= 1 << i;
	}
	else {
	  command = "check method/method=\"" + DynC.method_toolbar_op_methods[i] + "\"/object=" +
	    this.method_object;

	  command = this.dyn.graph.getCommand(command);
	  sts = this.dyn.graph.command(command);
	  if ( (sts & 1) != 0)
	    this.xm_mask_opmethods |= 1 << i;
	}
      }
      this.xm_mask_mntmethods = 0;
      for ( var i = 0; i < DynC.method_toolbar_mnt_cnt; i++) {
	if ( DynC.method_toolbar_mnt_methods[i] === "")
	  continue;
	command = "check method/method=\"" + DynC.method_toolbar_mnt_methods[i] + "\"/object=" +
	  this.method_object;
	
	command = this.dyn.graph.getCommand(command);
	sts = this.dyn.graph.command(command);
	if ( (sts & 1) != 0)
	  this.xm_mask_mntmethods |= 1 << i;
      }
      this.xm_mask_flags |= Pwrb.mXttMethodsFlagsMask_IsConfigured;
    }
    console.log("Method mask: " + this.xm_mask_opmethods + " " + this.xm_mask_mntmethods);

    var opmask = this.xm_mask_opmethods;
    var mntmask = this.xm_mask_mntmethods;
    var insensitive_opmask = 0;
    var insensitive_mntmask = 0;

    opmask &= ~Pwrb.mXttOpMethodsMask_OpenObjectGraph;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtEventsBlock | Pwr.mAccess_System))
      insensitive_opmask |= Pwrb.mXttOpMethodsMask_BlockEvents;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtNavigator | Pwr.mAccess_System | Pwr.mAccess_RtWrite | 
				  Pwr.mAccess_Maintenance | Pwr.mAccess_Process | Pwr.mAccess_Instrument))
      insensitive_opmask |= Pwrb.mXttOpMethodsMask_Note;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtNavigator | Pwr.mAccess_System))
      insensitive_mntmask |= Pwrb.mXttMntMethodsMask_RtNavigator;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtPlc | Pwr.mAccess_System))
      insensitive_mntmask |= Pwrb.mXttMntMethodsMask_OpenTrace;  

    if ( this.toolbar_type == DynC.eMethodToolbarType_Simulate)
      opmask = 0;

    this.o.configure( DynC.method_toolbar_op_subgraph, DynC.method_toolbar_mnt_subgraph,
		      DynC.method_toolbar_op_cnt, DynC.method_toolbar_mnt_cnt, opmask, mntmask,
		      insensitive_opmask, insensitive_mntmask);

    this.mask_store = 0; // Check call is not universal
    if ( this.mask_store != 0) {
      var psts;

      parsed_name = this.pname_name + ".XttMethodsMask.OpMethods";
      psts = this.dyn.graph.getGdh().setObjectInfoInt( parsed_name, this.xm_mask_opmethods);
      if ( psts.evenSts())
	console.log( "Set mask error " + parsed_name);

      parsed_name = this.pname_name + ".XttMethodsMask.MntMethods";
      psts = this.dyn.graph.getGdh().setObjectInfoInt( parsed_name, this.xm_mask_mntmethods);
      if ( psts.evenSts())
	console.log( "Set mask error " + parsed_name);

      parsed_name = this.pname_name + ".XttMethodsMask.Flags";
      psts = this.dyn.graph.getGdh().setObjectInfoInt( parsed_name, this.xm_mask_flags);
      if ( psts.evenSts())
	console.log( "Set mask error " + parsed_name);

    }

    return 1;
  };

  this.action = function( object, e) {

    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    switch ( e.event) {
    case Glow.eEvent_MB1Down:
      object.setColorInverse( 1);
      break;
    case Glow.eEvent_MB1Up:
      object.setColorInverse( 0);
      break;
    case Glow.eEvent_MB1Click: {
      var sts;
      var command;
      var idx = e.idx;
      var category = e.category;

      console.log( "Toolbar action " + category + " " + idx);

      if ( category == 1) {
	if ( DynC.method_toolbar_op_methods[idx] == "Parent Object Graph")
	  command = "open graph/class/parent/instance=" + this.method_object;
	else
	  command = "call method/method=\"" + DynC.method_toolbar_op_methods[idx] + 
	    "\"/object=" +  this.method_object;
      }
      else {
	command = "call method/method=\"" + DynC.method_toolbar_mnt_methods[idx] + 
	  "\"/object=" +  this.method_object;
      }

      command = this.dyn.graph.getCommand(command);
      sts = this.dyn.graph.command(command);
		
      break;
    }
    default:
      return 0;
    }
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( object) {
  };
}
 
function DynMethodPulldownMenu( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_MethodPulldownMenu;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_MethodPulldownMenu;
  this.instance_mask = 0;
  this.instance = 0;

  this.method_object;
  this.menu_type;
  this.opmask;
  this.mntmask;
  this.menu_object = null;
  this.text_size;
  this.o;

  this.open = function( lines, row) {
    var end = false;
    var i;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "MethodToolbar : " + lines[i]);

      switch ( key) {
      case DynC.eSave_MethodPulldownMenu: 
	break;
      case DynC.eSave_MethodPulldownMenu_method_object: 
	if ( tokens.length > 1)
	  this.method_object = tokens[1];
	break;
      case DynC.eSave_MethodPulldownMenu_menu_type: 
	this.menu_type = parseInt( tokens[1], 10);
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynMethodPulldownMenu");
	break;
      }

      if ( end)
	break;
    }
		
    return i;
  };

  this.pname_name;
  this.mask_configure = 0;
  this.mask_store = 0;
  this.xm_mask_flags = 0;
  this.xm_mask_opmethods = 0;
  this.xm_mask_mntmethods = 0;

  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
      return 1;

    this.o = object;
    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      if ( this.menu_object != null) {
	// Close, delete this menu
	this.dyn.graph.getCtx().remove(this.menu_object);
	this.menu_object = null;
      }
      else if ( object != null) {
	var	ll_x, ll_y, ur_x, ur_y;
	var	parsed_name;
    
	var pname = this.dyn.parseAttrName(this.method_object);
	if ( pname == null || pname.name === "") 
	  return 1;

        this.pname_name = pname.name;
	parsed_name = this.pname_name + ".XttMethodsMask.Flags";
	
	this.dyn.graph.getGdh().getObjectInfoInt( parsed_name, this.action2, this);
      }

      break;
    case Glow.eEvent_MenuActivated:
      if ( this.menu_object == null)
	break;
      if ( e.object == this.menu_object) {
      
	var sts;
	var command;
	var idx = 0;
	var found = 0;

	for ( var i = 0; i < DynC.method_toolbar_op_cnt; i++) {
	  if ( this.menu_type != DynC.eMethodsMenuType_Help && (DynC.method_toolbar_op_helpmask & (1 << i)) == 0)
	    continue;
	  if ( this.menu_type != DynC.eMethodsMenuType_Simulate)
	    continue;

	  if ( (this.opmask & (1 << i)) != 0)
	    idx++;
	  if ( e.item + 1 == idx) {
	    command = "call method/method=\"" + DynC.method_toolbar_op_methods[i] +
	      "\"/object=" + this.method_object;
	    command = this.dyn.graph.getCommand(command);
	    sts = this.dyn.graph.command(command);
	    found = 1;
	    break;
	  }
	}

	if ( found == 0) {
	  for ( var i = 0; i < DynC.method_toolbar_mnt_cnt; i++) {
	    if ( this.menu_type == DynC.eMethodsMenuType_Help && (DynC.method_toolbar_mnt_helpmask & (1 << i)) == 0)
	      continue;

	    if ( (this.mntmask & (1 << i)) != 0)
	      idx++;
	    if ( e.item + 1 == idx) {
	      command = "call method/method=\"" + DynC.method_toolbar_mnt_methods[i] +
		"\"/object=" + this.method_object;
	      command = this.dyn.graph.getCommand(command);
	      sts = this.dyn.graph.command(command);
	      found = 1;
	      break;
	    }
	  }
	}
      }
      break;
    case Glow.eEvent_MenuDelete:
      if ( this.menu_object == null)
	break;
      if ( e.object == null || e.object == this.menu_object) {
	// Delete this menu
	this.dyn.graph.getCtx().remove(this.menu_object);
	this.menu_object = null;
      }
      break;

    default: ;
    }
    return 1;
  };


  this.action2 = function( id, self, sts, value) {
    if ( sts & 1) {
      self.xm_mask_flags = value;

      if ( (self.xm_mask_flags & Pwrb.mXttMethodsFlagsMask_IsConfigured) == 0) {		    
	self.mask_configure = 1;
	self.mask_store = 1;
	self.action5();
      }      
      else {
	var parsed_name = self.pname_name + ".XttMethodsMask.OpMethods";

	self.dyn.graph.getGdh().getObjectInfoInt( parsed_name, self.action3, self);
      }
    }
    else {
      self.mask_configure = 1;
      self.action5();
    }
  };

  this.action3 = function( id, self, sts, value) {

    if ( !(sts & 1)) {
      console.log("DynMethodToolbar: " + self.pname__name);
      self.mask_configure = 1;
    }
    else
      self.xm_mask_opmethods = value;

    var parsed_name = self.pname_name + ".XttMethodsMask.MntMethods";
    self.dyn.graph.getGdh().getObjectInfoInt( parsed_name, self.action4, self);
  };

  this.action4 = function( id, self, sts, value) {
    if ( !(sts & 1)) {
      console.log("DynMethodToolbar: " + self.pname_name);
      self.mask_configure = 1;
    }
    else
      self.xm_mask_mntmethods = value;
    self.action5();
  };

  this.action5 = function() {
    var 	command;
    var		sts;

    if ( this.mask_configure != 0) {
      this.xm_mask_opmethods = 0;
      for ( var i = 0; i < DynC.method_toolbar_op_cnt; i++) {
	if ( DynC.method_toolbar_op_methods[i] == "Parent Object Graph") {
	  if ( this.method_object.indexOf('.') != -1)
	    this.xm_mask_opmethods |= 1 << i;
	}
	else {
	  command = "check method/method=\"" + DynC.method_toolbar_op_methods[i] + "\"/object=" +
	    this.method_object;
	  
	  command = this.dyn.graph.getCommand(command);
	  sts = this.dyn.graph.command(command);
	  if ( (sts & 1) != 0)
	    this.xm_mask_opmethods |= 1 << i;
	}
      }
      this.xm_mask_mntmethods = 0;
      for ( var i = 0; i < DynC.method_toolbar_mnt_cnt; i++) {
	command = "check method/method=\"" + DynC.method_toolbar_mnt_methods[i] + "\"/object=" +
	  this.method_object;
	
	command = dyn.graph.getCommand(command);
	var sts = dyn.graph.command(command);
	if ( (sts & 1) != 0)
	  this.xm_mask_mntmethods |= 1 << i;
      }
      this.xm_mask_flags |= Pwrb.mXttMethodsFlagsMask_IsConfigured;
    }
    
    this.opmask = this.xm_mask_opmethods;
    this.mntmask = this.xm_mask_mntmethods;
    var insensitive_opmask = 0;
    var insensitive_mntmask = 0;
    var info = new GlowMenuInfo();
    
    this.opmask &= ~Pwrb.mXttOpMethodsMask_OpenObjectGraph;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtEventsBlock | Pwr.mAccess_System))
      insensitive_opmask |= Pwrb.mXttOpMethodsMask_BlockEvents;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtNavigator | Pwr.mAccess_System | Pwr.mAccess_RtWrite | 
				       Pwr.mAccess_Maintenance | Pwr.mAccess_Process | Pwr.mAccess_Instrument))
      insensitive_opmask |= Pwrb.mXttOpMethodsMask_Note;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtNavigator | Pwr.mAccess_System))
      insensitive_mntmask |= Pwrb.mXttMntMethodsMask_RtNavigator;
    if ( !this.dyn.graph.isAuthorized( Pwr.mAccess_RtPlc | Pwr.mAccess_System))
      insensitive_mntmask |= Pwrb.mXttMntMethodsMask_OpenTrace;    
    

    var menu_idx = 0;
    for ( var i = 0; i < 32; i++)
      info.item[i].occupied = false;
    
    for ( var i = 0; i < DynC.method_toolbar_op_cnt; i++) {
      if ( menu_idx >= 32)
	break;
      
      if ( this.menu_type == DynC.eMethodsMenuType_Help && (DynC.method_toolbar_op_helpmask & (1 << i)) == 0)
	continue;
      if ( this.menu_type == DynC.eMethodsMenuType_Simulate)
	continue;
      
      if ( (this.opmask & (1 << i)) != 0) {
	info.item[menu_idx].type = Glow.eMenuItem_Button;
	info.item[menu_idx].text = new String(DynC.method_toolbar_op_methods[i]);
	info.item[menu_idx].occupied = true;
	if ( (insensitive_opmask & (1 << i)) != 0)
	  info.item[menu_idx].type = Glow.eMenuItem_ButtonDisabled;
	menu_idx++;
      }
    }
    for ( var i = 0; i < DynC.method_toolbar_mnt_cnt; i++) {
      if ( menu_idx >= 32)
	break;
      
      if ( this.menu_type == DynC.eMethodsMenuType_Help && (DynC.method_toolbar_mnt_helpmask & (1 << i)) == 0)
	continue;
      
      if ( (this.mntmask & (1 << i)) != 0) {
	info.item[menu_idx].type = Glow.eMenuItem_Button;
	info.item[menu_idx].text = DynC.method_toolbar_mnt_methods[i];
	info.item[menu_idx].occupied = true;
	if ( (insensitive_mntmask & (1 << i)) != 0)
	  info.item[menu_idx].type = Glow.eMenuItem_ButtonDisabled;
	menu_idx++;
      }
    }

    // Get fillcolor, and textattributes from object
    var text_drawtype = 0, text_color = 0, bg_color = 0;
    var text_size = 0;
    var scale = 1;
    var text_font = 0;
    
    sts = 0; // TODO sts = grow_GetObjectAnnotInfo( object, 1, &text_size, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
    if ( (sts & 1) == 0) {
      text_size = 3;
      text_drawtype = Glow.eDrawType_TextHelvetica;
      text_font = Glow.eFont_LucidaSans;
      text_color = Glow.eDrawType_Line;
      bg_color = Glow.eDrawType_LightGray;
    }
    else if ( bg_color == Glow.eDrawType_No || bg_color == Glow.eDrawType_Inherit)
      bg_color = Glow.eDrawType_LightGray;
    
    var g = this.o.measure();
    this.menu_object = new GrowMenu( this.dyn.graph.getCtx());
    this.menu_object.init( "__Menu", info, g.ll_x, g.ur_y, g.ur_x - g.ll_x,
			   Glow.eDrawType_Line, 0, 1, 1, bg_color, text_size,
			   text_drawtype, text_color,
			   Glow.eDrawType_MediumGray, text_font);
    this.menu_object.set_scale( scale, scale, 0, 0, Glow.eScaleType_LowerLeft);
    this.dyn.graph.getCtx().insert(this.menu_object);
    
    // grow_SetMenuInputFocus( menu_object, 1);
  };
  
  this.connect = function( object) {
  };
  this.disconnect = function() {
  };
  this.scan = function( object) {
  };
}

function DynPopupMenu( dyn) {
  this.dyn = dyn;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = DynC.mActionType1_PopupMenu;
  this.action_type2 = 0;
  this.prio = DynC.eDynPrio_PopupMenu;
  this.instance_mask = 0;
  this.instance = 0;

  this.ref_object;

  this.connect = function( o) {
    return 1;
  };
  this.disconnect = function() {
  };
  this.scan = function( o) {
    return 1;
  };
  this.action = function( object, e) {
    if ( !this.dyn.graph.isAuthorized( this.dyn.access))
		return 1;

    switch ( e.event) {
    case Glow.eEvent_MB3Press:
      // TODO
      //this.dyn.graph.openPopupMenu( ref_object, e.x, e.y);
      break;
    }
    return 1;
  };

  this.open = function( lines, row) {
    var end = false;
    var i;
    var elem;

    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if ( this.dyn.debug) console.log( "DynResetDig : " + lines[i]);

      elem = null;

      switch ( key) {
      case DynC.eSave_PopupMenu: 
	break;
      case DynC.eSave_PopupMenu_ref_object: 
	if ( tokens.length > 1)
	  this.ref_object = tokens[1];
	break;
      case DynC.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in DynPopupMenu");
	break;
      }
      
      if ( end)
	break;
    }
    
    return i;
  };
  
}

var GraphIfc = {
  eDatabase_Gdh 	: 0,
  eDatabase_User	: 1,
  eDatabase_Local 	: 2,
  eDatabase_Ccm 	: 3,

  eType_Bit 		: (1 << 15) + 1
};

function Graph( appl) {
  var self = this;
  this.appl = appl;
  this.ctx;
  this.baseCtx;
  this.gdraw;
  this.gdh;
  this.ldb;
  this.currentSlider;
  this.scan_time = 0.6;
  this.fast_scan_time = 0.15;
  this.animation_scan_time = 0.15;
  this.slow_scan_cnt = 0;
  this.fast_scan_cnt = 0;
  this.ctxStack = new Array(10);
  this.ctxStackCnt = 0;
  this.clickActive = 0;
  this.priv = 0;

  if ( typeof InstallTrigger !== 'undefined') {
    // Firefox is not os fast...
    this.scan_time = 1;
    this.fast_scan_time = 1;
    this.animation_scan_time = 1;
  }

  this.frame = new GrowFrame( this);
  this.priv = sessionStorage.getItem("pwr_privilege");
  console.log("pwr_privilege" ,this.priv);

  this.read_cb = function( lines, row) {
    self.frame.read_func( self.ctx, lines, row);
    self.frame.register_events( self.baseCtx);
    if ( self.frame.nogdh) {
      self.priv = 5;
      self.ctx.traceConnect();
      self.trace_cyclic();
    }
    else {
      self.gdh = new Gdh();
      self.gdh.open_cb = self.gdh_init_cb;
      self.gdh.init();
    }
  };

  this.frame.readGrowWeb( this.frame.get_filename(), this.read_cb);  
  this.ldb = new GraphLocalDb();

  // Set init values for Ge demo
  this.ldb.setObjectInfo( this, "$local.Demo-Ge-Dynamics-ScrollingText-ScrollingText.ActualValue##String80", "ProviewR");
  this.ldb.setObjectInfo( this, "$local.Demo-Ge-Subgraphs-DynamicAxis-Av2.ActualValue##Float32", 100);
  this.ldb.setObjectInfo( this, "$local.Demo-Ge-Subgraphs-DynamicAxisArc-Av2.ActualValue##Float32", 100);

  this.gdh_init_cb = function() {
    if ( self.priv == null)
      self.gdh.login( "", "", self.login_cb, self);

    self.ctx.traceConnect();
    self.gdh.refObjectInfoList( self.trace_connected);
  };

  this.login_cb = function( id, data, sts, result) {
    console.log( "Login:", sts, result);
    if ( sts & 1)
      self.priv = result;
    else
      self.priv = 0;
  };

  this.trace_connected = function( id, sts) {
    self.trace_cyclic();
  };

  this.trace_cyclic = function() {
    if ( self.frame.nogdh)
      self.trace_scan(0,0);
    else	
      self.gdh.getRefObjectInfoAll( self.trace_scan);
  };
  this.trace_scan = function( id, sts) {
    self.scan_time = self.ctx.scantime;
    self.fast_scan_time = self.ctx.fast_scantime;
    self.animation_scan_time = self.ctx.animation_scantime;

    self.ctx.traceScan();
    // if ( self.slow_scan_cnt == 0 || self.fast_scan_cnt == 0)
    self.ctx.draw();

    self.slow_scan_cnt++;
    if (self.slow_scan_cnt
        >= (self.scan_time / self.animation_scan_time))
      self.slow_scan_cnt = 0;

    self.fast_scan_cnt++;
    if (self.fast_scan_cnt
        >= (self.fast_scan_time / self.animation_scan_time))
      self.fast_scan_cnt = 0;

    self.timer = setTimeout( self.trace_cyclic, 1000 * self.animation_scan_time);
  };

  this.growUserdataOpen = function( lines, row, ctx, type) {
    var dyn;
    var ret = new UserdataCbReturn();

    switch ( type) {
    case Glow.eUserdataCbType_Ctx:
      if ( lines[row] != "1") {
	ret.row = row - 1;
	ret.userdata = null;
      }
      else {
	dyn = new Dyn(this);
	dyn.userdata = this;
	ret.userdata = dyn;
	ret.row = dyn.open( lines, row);
      }
      break;
    case Glow.eUserdataCbType_Node:
      dyn = new Dyn(this);
      dyn.userdata = this;
      ret.userdata = dyn;
      ret.row = dyn.open( lines, row);
      break;
    case Glow.eUserdataCbType_NodeClass:
      dyn = new Dyn(this);
      ret.row = dyn.open( lines, row);
      ret.userdata = dyn;
      break;
    default: 
      break;
    }
    return ret;
  };
  
  this.event_handler = function( e) {
    var ctx_popped = false;
    var dyn;
    var o;
    
    if (e.object_type != Glow.eObjectType_NoObject &&
	e.object !== null) {
      ctx_popped = this.ctxPop( e.object.ctx);
    }

    switch ( e.event) {
    case Glow.eEvent_MB1Click:
      if ( e.object_type == Glow.eObjectType_NoObject ||
	   e.object.type() != Glow.eObjectType_GrowMenu) {
	// Close any open menu, if not click in menu
	var event = new GlowEventMenu();
	event.event = Glow.eEvent_MenuDelete;
	event.type = Glow.eEventType_Menu;
	event.object = null;

	var list = this.ctx.get_object_list();
	for ( var i = 0; i < list.size(); i++) {
	  o = list.get(i);
	  if ( (o.type() == Glow.eObjectType_GrowNode ||
		o.type() == Glow.eObjectType_GrowGroup) &&
	       (e.object_type == Glow.eObjectType_NoObject ||
		o != e.object)) {
	    dyn = o.getUserData();
	    if ( dyn !== null)
	      dyn.action( o, event);
	    var old_size = list.size(); 
	    list = this.ctx.get_object_list();
	    if ( old_size != list.size())
	      // Something is deleted
	      break;
	  }
	}
      }
      // Note! no break
    case Glow.eEvent_MB1Up:
    case Glow.eEvent_MB1Down:
    case Glow.eEvent_MB1DoubleClick:
    case Glow.eEvent_MB3Press:
    case Glow.eEvent_ValueChanged:
    case Glow.eEvent_SliderMoveStart:
    case Glow.eEvent_SliderMoveEnd:
    case Glow.eEvent_SliderMoved:
    case Glow.eEvent_Signal:
      if ( e.object !== null) {
	var sts;
	dyn = e.object.getUserData();
	if ( dyn !== null)
	  sts = dyn.action( e.object, e);
      }
      break;
    case Glow.eEvent_MenuActivated:
    case Glow.eEvent_MenuCreate:
    case Glow.eEvent_MenuDelete: {
      var		old_size;
      var		sts;
      
      var list = this.ctx.get_object_list();
      
      for ( var i = 0; i < list.size(); i++) {
	o = list.get(i);
	if ( o.type() == Glow.eObjectType_GrowNode ||
	     o.type() == Glow.eObjectType_GrowGroup) {
	  
	  dyn = o.getUserData();
	  if ( dyn !== null) {
	    sts = dyn.action( o, e);
	    if ( sts == Glow.GLOW__TERMINATED)
	      return;
	    
	    // Check if anything is deleted
	    old_size = list.size();
	    list = this.ctx.get_object_list();
	    if ( old_size != list.size())
	      break;
	  }
	}
      }
      break;
    }
    default: break;
    }
    if ( ctx_popped)
      this.ctxPush();
  };

  this.ctxPop = function( nctx) {
    if ( this.ctx == nctx)
      return false;

    if ( this.ctxStackCnt >= 10) {
      console.log("** Graph ctx stack overflow");
      return false;
    }
    this.ctxStack[this.ctxStackCnt] = this.ctx;
    this.ctxStackCnt++;
    this.ctx = nctx;
    return true;
  };

  this.ctxPush = function() {
    if ( this.ctxStackCnt > 0) {
      --this.ctxStackCnt;
      this.ctx = this.ctxStack[this.ctxStackCnt];
    }
  };

  this.getCtx = function() {
    return this.ctx;
  };

  this.setSubwindowSource = function( name, source, owner) {
    var ctx = this.ctx;
    if ( this.ctxStackCnt > 0)
      ctx = this.ctxStack[this.ctxStackCnt-1];
    ctx.setSubwindowSource( name, source, owner);
  };


  this.setClickActive = function( active) {
    this.clickActive = active;
  };

  this.getClickActive = function() {
    return clickActive;
  };

  this.stringToType = function( str) {
    var idx;
    if ( (idx = str.indexOf('#')) != -1)
      str = str.substring(0, idx);
    if ( str.toLowerCase() == "boolean")
      return Pwr.eType_Boolean;
    if ( str.toLowerCase() == "float32")
      return Pwr.eType_Float32;
    if ( str.toLowerCase() == "float64")
      return Pwr.eType_Float64;
    if ( str.toLowerCase() == "char")
      return Pwr.eType_Char;
    if ( str.toLowerCase() == "int8")
      return Pwr.eType_Int8;
    if ( str.toLowerCase() == "int16")
      return Pwr.eType_Int16;
    if ( str.toLowerCase() == "int32")
      return Pwr.eType_Int32;
    if ( str.toLowerCase() == "int64")
      return Pwr.eType_Int64;
    if ( str.toLowerCase() == "uint8")
      return Pwr.eType_UInt8;
    if ( str.toLowerCase() == "uint16")
      return Pwr.eType_UInt16;
    if ( str.toLowerCase() == "uint32")
      return Pwr.eType_UInt32;
    if ( str.toLowerCase() == "uint64")
      return Pwr.eType_UInt64;
    if ( str.toLowerCase() == "objid")
      return Pwr.eType_Objid;
    if ( str.toLowerCase() == "time")
      return Pwr.eType_Time;
    if ( str.toLowerCase() == "deltatime")
      return Pwr.eType_DeltaTime;
    if ( str.toLowerCase() == "attrref")
      return Pwr.eType_AttrRef;
    if ( str.toLowerCase() == "status")
      return Pwr.eType_Status;
    if ( str.toLowerCase() == "netstatus")
      return Pwr.eType_NetStatus;
    if ( str.toLowerCase() == "enum")
      return Pwr.eType_Enum;
    if ( str.toLowerCase() == "mask")
      return Pwr.eType_Mask;
    if ( str.toLowerCase() ==  "bit")
      return GraphIfc.eType_Bit;
    if ( str.length >= 6 && str.substring(0,6).toLowerCase() == "string")
      return Pwr.eType_String;
    return 0;
  };

  this.stringToIndex = function( str) {
    var idx1, idx2;
    var index;

    if ( (idx1 = str.indexOf('[')) != -1 &&
	 (idx2 = str.indexOf(']')) != -1 &&
	 idx2 > idx1) {
      index = parseInt( str.substring(idx1+1, idx2), 10);
      if ( isNaN( index)) {
	console.log("Element syntax error, " + str);
	return 1;
      }
      return index;
    }
    return 1;
  };

  this.parseAttrName = function( name) {
    if ( name === null)
      return null;

    var idx, tidx, eidx;
    var pname = new DynParsedAttrName();

    var str = name.trim();
  	
    if ( (idx = str.indexOf("$local.")) != -1) {
      if ( (tidx = str.indexOf('#')) == -1)
	pname.name = str.substring(idx+1);
      else {
	pname.name = str.substring(idx+7, tidx);
	var type = str.substring(tidx).toUpperCase();
	if ( type == "##FLOAT32")
	  pname.type = Pwr.eType_Float32;
	else if ( type == "##FLOAT64")
	  pname.type = Pwr.eType_Float64;
	else if ( type == "##INT32")
	  pname.type = Pwr.eType_Int32;
	else if ( type == "##BOOLEAN")
	  pname.type = Pwr.eType_Boolean;
	else
	  pname.type = Pwr.eType_String;
      }
      if ( str.substring(0, 1) == "!") {
	str = str.substring(1);
	str = str.trim();
	pname.inverted = true;
      }
      else
	pname.inverted = false;

      pname.database = GraphIfc.eDatabase_Local;
      pname.tname = name;
      return pname;
    }

    if ( (idx = str.indexOf("$ccm.")) != -1) {
      if ( (tidx = str.indexOf('#')) == -1)
	pname.name = str.substring(idx+1);
      else {
	pname.name = str.substring(idx+5, tidx);
	var type2 = str.substring(tidx).toUpperCase();
	if ( type2 == "##FLOAT32")
	  pname.type = Pwr.eType_Float32;
	else if ( type2 == "##INT32")
	  pname.type = Pwr.eType_Int32;
	else if ( type2 == "##BOOLEAN")
	  pname.type = Pwr.eType_Boolean;
	else
	  pname.type = Pwr.eType_String;
      }
      if ( str.substring(0, 1) == "!") {
	str = str.substring(1);
	str = str.trim();
	pname.inverted = true;
      }
      else
	pname.inverted = false;

      pname.database = GraphIfc.eDatabase_Ccm;
      pname.tname = new String(pname.name);
      return pname;
    }



    if ( (idx = str.indexOf("$object")) != -1) {
      if ( this.appl !== null) {
	var oname = this.ctx.getOwner();
	str = str.substring(0, idx) + oname + str.substring(idx+7);
      }
    }

    pname.tname = new String(str);
	
    if ( (idx = str.indexOf('[')) == -1) {
      if ((eidx = str.lastIndexOf('#')) != -1 &&
	  str.charAt(eidx-1) != '#') {
	pname.elements = parseInt( str.substring(eidx+1), 10);
	if ( isNaN( pname.elements))
	  pname.elements = 1;
	str = str.substring(0, eidx);
      }
      else
	pname.elements = 1;
    }	    
    else {
      pname.index = this.stringToIndex(str);
      pname.elements = 1;
    }

    if ( (tidx = str.indexOf("##")) != -1) {
      if ( str.toLowerCase().substring( tidx+2, tidx+5) == "bit") {
	var typestr = str.substring(tidx+2);
	var bit = this.stringToIndex(typestr);
	pname.bitmask = 1 << bit;
	str = str.substring(0, tidx+5);
      }

      pname.type = this.stringToType( str.substring(tidx+2));
      str = str.substring(0, tidx);
    }
    else
      pname.type = 0;

    if ( str.substring(0,1) == "!") {
      str = str.substring(1);
      str = str.trim();
      pname.inverted = true;
      pname.tname = pname.tname.substring(1);
      pname.tname = pname.tname.trim();
    }
    else
      pname.inverted = false;

    pname.database = GraphIfc.eDatabase_Gdh;
    pname.name = str;

    return pname;
  };

  this.getLdb = function() {
    return this.ldb;
  };
  this.getGdh = function() {
    return this.gdh;
  };
  this.isAuthorized = function( access) {
    return ( this.priv & access) ? true : false;
    // return appl.isAuthorized( access);
  };

  this.traceConnect = function( object) {
    var ctx_popped = false;

    if ( object.ctx != this.ctx)
      ctx_popped = this.ctxPop( object.ctx);

    if ( object.userdata == null) {
      if ( ctx_popped)
	this.ctxPush();
      return;
    }

    var dyn = object.userdata;
    if ( object.type() == Glow.eObjectType_GrowNode) {
      var dyn_type1 = object.getClassDynType1();

      if ( (((dyn_type1 & DynC.mDynType1_HostObject) != 0 && 
	     (dyn.dyn_type1 & DynC.mDynType1_Inherit) != 0)) || 
	   (dyn.dyn_type1 & DynC.mDynType1_HostObject) != 0) {
	var nodeclass_dyn = object.getClassUserData();
	dyn.setTotal(null);
	if ( nodeclass_dyn != null) {
	  var old_dyn = dyn;
	  dyn = new Dyn( this);
	  dyn.merge( old_dyn);
	  dyn.total_dyn_type1 |= DynC.mDynType1_HostObject;
	  dyn.merge( nodeclass_dyn);
	  if ( old_dyn.cycle != Glow.eCycle_Inherit)
	    dyn.cycle = old_dyn.cycle;
	  if ( !(old_dyn.action_type1 == DynC.mActionType1_Inherit && old_dyn.action_type2 == 0))
	    dyn.access = old_dyn.access;
	  object.userdata = dyn;
	  dyn.setTotal(object);
	}
      }
    }

    object.userdata.connect( object);

    if ( ctx_popped)
      this.ctxPush();
  };
  this.traceDisconnect = function( o) {
    if ( o.userdata !== null)
      o.userdata.disconnect( o);
  };
  this.traceScan = function( o) {
    if ( o.userdata !== null) {
      var dyn = o.userdata;

      if (dyn.cycle == Glow.eCycle_Inherit)
	return 1;
      if ( dyn.cycle == Glow.eCycle_Slow && this.slow_scan_cnt != 0
	  && !(dyn.total_dyn_type1 & DynC.mDynType1_Animation
	       || dyn.total_dyn_type2 & DynC.mDynType2_ScrollingText))
	return 1;
      if (dyn.cycle == Glow.eCycle_Fast && this.fast_scan_cnt != 0
	  && !(dyn.total_dyn_type1 & DynC.mDynType1_Animation
	       || dyn.total_dyn_type2 & DynC.mDynType2_ScrollingText))
	return 1;

      o.userdata.scan( o);
    }
  };

  this.openValueInputDialog = function( dyn, text, elem) {
    var value = prompt( text, "");
    if ( value !== null)
      dyn.valueInputAction( elem, value);
  };

  this.openConfirmDialog = function( dyn, text, object) {
    if ( appl != null)
      appl.openConfirmDialog( dyn, text, object);
  };

  this.getScanTime = function() {
    return this.scan_time;
  };

  this.getFastScanTime = function() {
    return this.fast_scan_time;
  };

  this.getAnimationScanTime = function() {
    if ( this.scan_time < this.animation_scan_time)
      return this.scan_time;
    return this.animation_scan_time;
  };

  this.command = function( cmd) {
    if ( this.appl != null)
      return this.appl.command( cmd);
    return 0;
  };

  this.script = function( script) {
    if ( this.appl != null)
      return this.appl.script( script);
    return 0;
  };

  this.closeGraph = function() {
    window.close();
  };

  this.getCommand = function( cmd) {
    var str = cmd;
    var idx;

    while ( (idx = str.indexOf("$object")) != -1) {
      if ( appl != null) {
	var oname = this.ctx.getOwner();
	str = str.substring(0, idx) + oname + str.substring(idx+7);
      }
    }
    return str;
  };

  this.setCurrentSlider = function( slider) {
    this.currentSlider = slider;
  };

  this.getCurrentSlider = function() {
    return this.currentSlider;
  };

  this.loadCtx = function( file, read_cb) {
    return this.frame.readGrowWindow( file, read_cb);
  };
}

function LocalSub( owner, name, typeId, refId) {
  this.valueFloat = 0;
  this.valueInt = 0;
  this.valueBoolean = false;
  this.valueString = null;
  this.name = name;
  this.occupied = true;
  this.typeId = typeId;
  this.refId = refId;
  this.owner = owner;
  this.refCount = 0;

  this.ref = function() {
    this.refCount++;
  };
  this.unref = function() {
    this.refCount--;
  };
  this.getRefCount = function() {
    return this.refCount;
  };
}

function GraphLocalDb() {


  this.subscriptions = [];
  this.subscriptionCount = 1;

  // Insert dummy first
  this.subscriptions[0] = null;

  this.refObjectInfo = function( owner, attributeName) {
    var typeId = this.getTypeId(attributeName);
    var name = this.getName(attributeName);
    if ( typeId === 0) {
      return i;
    }

    var id = this.nameToId( owner, name);
    var sub;
    if ( id == -1) {
      id = this.subscriptionCount;
      sub = new LocalSub( owner, name, typeId, id);
      sub.ref();
      this.subscriptions[id] = sub;
      this.subscriptionCount++;
      return id;
    }
    else {
      sub = this.subscriptions[id];
      sub.ref();
      return id; 
    }
  };

  this.unrefObjectInfo = function( id) {
    var sub = this.subscriptions[id];
    if ( sub === null)
      return;
    sub.unref();
    if ( sub.getRefCount() <= 0)
      this.subscriptions[id] = null;
  };

  this.getObjectRefInfo = function( id) {
    var sub = this.subscriptions[id];
    if ( sub === null)
      return 0;
    if ( typeof sub.value == 'undefined')
      return 0;
    return sub.value;
  };

  this.getObjectInfo = function( owner, attributeName) {
    var name = this.getName( attributeName);
    var id = this.nameToId( owner, name);
    if ( id == -1)
      return new CdhrNumber( 0, 2);
    if ( typeof this.subscriptions[id].value == 'undefined')
      return new CdhrNumber( 0, 1);
    return new CdhrNumber( this.subscriptions[id].value, 1);
  };

  this.setObjectInfo = function( owner, attributeName, value) {
    var name = this.getName( attributeName);
    var id = this.nameToId( owner, name);
    if ( id == -1) {
      this.refObjectInfo( owner, attributeName);
      id = this.subscriptionCount - 1;
    }
    this.subscriptions[id].value = value;
    return new PwrtStatus(1);
  };

  this.toggleObjectInfo = function( owner, attributeName) {
    var name = this.getName( attributeName);
    var id = this.nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    this.subscriptions[id].value = 
	!this.subscriptions[id].value;
    return new PwrtStatus(1);
  };

  this.nameToId = function( owner, name) {
    for ( var i = 0; i < this.subscriptions.length; i++) {
      var sub = this.subscriptions[i];
      if ( sub !== null && owner == sub.owner && name.toUpperCase() === sub.name.toUpperCase())
	return i;
    }
    return -1;
  };

  this.getName = function( attrName) {
    if ( attrName.substring(0,7) == "$local.")
      attrName = attrName.substring(7);
    var idx1 = attrName.indexOf("##");
    if ( idx1 != -1)
      return attrName.substring(0, idx1);
    return attrName;
  };

  this.getTypeId = function( attrName) {
    var suffix;
    var idx1 = attrName.indexOf("##");
    if(idx1 < 0) {
      return Pwr.eType_Boolean;
    }

    idx1 += 2;
    var idx2 = attrName.indexOf('#', idx1);
    if ( idx2 < 0)
      suffix = attrName.substring(idx1).toUpperCase();
    else
      suffix = attrName.substring(idx1, idx2).toUpperCase();
    var idx2 = attrName.indexOf('#', idx1);
    if ( idx2 < 0)
      suffix = attrName.substring(idx1).toUpperCase();
    else
      suffix = attrName.substring(idx1, idx2).toUpperCase();
    if ( suffix == "BOOLEAN")
      return Pwr.eType_Boolean;
    if ( suffix == "FLOAT32")
      return Pwr.eType_Float32;
    if ( suffix == "INT32")
      return Pwr.eType_Int32;
    if ( suffix == "UINT32")
      return Pwr.eType_UInt32;
    if ( suffix == "INT16")
      return Pwr.eType_Int16;
    if ( suffix == "UINT16")
      return Pwr.eType_UInt16;
    if ( suffix == "INT8")
      return Pwr.eType_Int8;
    if ( suffix == "UINT8")
      return Pwr.eType_UInt8;
    if ( suffix == "CHAR")
      return Pwr.eType_Char;
    if ( suffix == "FLOAT64")
      return Pwr.eType_Float64;
    if ( suffix == "OBJID")
      return Pwr.eType_Objid;
    if ( suffix == "STRING")
      return Pwr.eType_String;
    if ( suffix == "TIME")
      return Pwr.eType_Time;
    if ( suffix == "DELTATIME")
      return Pwr.eType_DeltaTime;
    if ( suffix == "ATTRREF")
      return Pwr.eType_AttrRef;
    if ( suffix.substring(0, 6) == "STRING")
      return Pwr.eType_String;
    return 0;
  };

}

var cliTable = [ 
		new CliTable( "OPEN", 
			      ["cli_arg1", "cli_arg2", "/NAME", 
			       "/FILE", "/SCROLLBAR", "/WIDTH", "/HEIGHT", "/MENU", "/NAVIGATOR", 
			       "/CENTER", "/OBJECT", "/INSTANCE", "/NEW", "/CLASSGRAPH", "/ACCESS", "/PARENT"]),
		new CliTable( "EXIT", null),
		new CliTable( "HELP", 
			      ["cli_arg1", "cli_arg2", "cli_arg3",
			       "cli_arg4", "/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", "/INDEX",
			       "/BASE", "/RETURNCOMMAND", "/WIDTH", "/HEIGHT", "/VERSION"]),
		new CliTable( "SET", 
			      ["cli_arg1", "cli_arg2",
			       "/NAME", "/VALUE", "/BYPASS", "/SOURCE", "/OBJECT"]),
		new CliTable( "EXAMPLE",
			      ["/NAME", "/HIERARCHY"]),
		new CliTable( "CHECK", 
			      ["cli_arg1", "/METHOD", "/OBJECT"]),
		new CliTable( "CALL", 
			      ["cli_arg1", "/METHOD", "/OBJECT"]),
		new CliTable( "SET", 
			      ["cli_arg1", "dcli_arg2", "/CONTINUE"])
		];

function Appl() {
  this.graph = new Graph( this);

  this.command = function( cmd) {
    console.log("Command: " + cmd);
    var local_cmd = false;

    if ( !this.graph.isAuthorized(Pwr.mAccess_AllRt)) {
      consol.log("Not authorized");
      return 0;
    }

    if ( cmd.charAt(0) == '@') {
      // Execute a script TODO
      // new JopgCcm( this, gdh, cmd.substring(1),  null);
      return 1;
    }

    var cli = new Cli( cliTable);
    var command = cli.parse( cmd);
    if (cli.oddSts()) {
      if ( command == ("OPEN")) {
	if ( cli.qualifierFound("cli_arg1")) {

	  var jgraph = "JGRAPH";
	  var graph = "GRAPH";
	  var url = "URL";
	  var trend = "TREND";
	  var fast = "FAST";
	  var cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
	  if ( jgraph.length >= cli_arg1.length &&
	       jgraph.substring(0,cli_arg1.length) == cli_arg1) {
	    // Command is "OPEN JGRAPH"
	    console.log("Command is not implemented, \"OPEN JGRAPH\"");
	  }
	  else if ( graph.length >= cli_arg1.length &&
		    graph.substring(0,cli_arg1.length) == cli_arg1) {
	    // Command is "OPEN GRAPH"
	    var graphName = null;
	    var instanceValue = null;
	    var classGraph = false;
	    var objectValue = null;
					
	    if ( cli.qualifierFound("/OBJECT")) {
	      objectValue = cli.getQualValue("/OBJECT");
	      return 1;
	    }
	    if ( cli.qualifierFound("/INSTANCE")) {
	      instanceValue = cli.getQualValue("/INSTANCE");
	      classGraph = cli.qualifierFound("/CLASSGRAPH");		      
	      var parent = cli.qualifierFound("/PARENT");
	      if ( parent) {
		var idx = instanceValue.lastIndexOf( '.');
		if ( idx != -1 && idx !== 0)
		  instanceValue = instanceValue.substring( 0, idx);
		console.log( "open graph /parent: " + instanceValue);
	      }
	    }
	    if ( classGraph) {
	      console.log("Cmd classGraph");
	      var newwindow = window.open("", "_blank");
	      this.graph.gdh.getObjectFromName( instanceValue, GdhOp.GET_OP_METHOD_OBJECTGRAPH, this.open_objectgraph_cb, newwindow);	    
	    }
	    else {
	      if ( ! cli.qualifierFound("cli_arg2")) {
		console.log("Syntax error");
		return 0;
	      }
        					
	      graphName = cli.getQualValue("cli_arg2").toLowerCase();
	      if ( graphName.charAt(".pwg") == -1)
		graphName = graphName + ".pwg";

	      var href;
	      if ( this.graph.frame.nogdh) {
		if ( instanceValue === null)
		  href = "ge.html?graph=" + graphName + "&instance=no&gdh=no";
		else
		  href = "ge.html?graph=" + graphName + "&instance=" + instanceValue + "&gdh=no";
	      }
	      else {
		if ( instanceValue === null)
		  href = "ge.html?graph=" + graphName;
		else
		  href = "ge.html?graph=" + graphName + "&instance=" + instanceValue;
	      }
	      console.log( "Cmd found: open graph", graphName, instanceValue);
	      var newwindow = window.open( href, "_blank");
	    }
	  }
	  else if ( url.length >= cli_arg1.length &&
		    url.substring(0,cli_arg1.length) == cli_arg1) {
	    // Command is "OPEN URL"
	    var newFrame = true;
	    var frameName = null;
	    var urlValue = cli.getQualValue("cli_arg2");

	    if ( urlValue.substring(0,5) == "pwrb_" ||
		 urlValue.substring(0,5) == "pwrs_" ||
		 urlValue.substring(0,5) == "nmps_" ||
		 urlValue.substring(0,9) == "profibus_" ||
		 urlValue.substring(0,8) == "otherio_" ||
		 urlValue.substring(0,4) == "opc_" ||
		 urlValue.substring(0,14) == "basecomponent_" ||
		 urlValue.substring(0,4) == "abb_" ||
		 urlValue.substring(0,8) == "siemens_" ||
		 urlValue.substring(0,7) == "ssabox_")
	      // Object reference manual
	      urlValue = "$pwr_doc/" + getLang() + "/orm/" + urlValue;

	    console.log("open url " + urlValue);
	    this.openURL( urlValue, null); 
	  }
	  else if ( trend.length >= cli_arg1.length &&
		    trend.substring(0,cli_arg1.length) == (cli_arg1)) {
	    // Command is "OPEN TREND"
	    console.log("Cmd open trend");
	  }
	  else if ( fast.length >= cli_arg1.length &&
		    fast.substring(0,cli_arg1.length) == (cli_arg1)) {
	    // Command is "OPEN FAST"
	    console.log("Cmd open fast");
	  }
	  else if ( command == ("HELP")) {
	    console.log("Cmd help");
	  }
	}
      }
      else if ( command == ("SET")) {
	if ( cli.qualifierFound("cli_arg1")) {
	  var cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
	  var subwindow = "SUBWINDOW";
	  if ( subwindow.length >= cli_arg1.length &&
	       subwindow.substring(0,cli_arg1.length) == (cli_arg1)) {
	    // Command is "SET SUBWINDOW"

	    var name;
	    var graphstr;
	    var source;
	    var object;
	    var sts;

	    local_cmd = true;
	    if ( cli.qualifierFound("/NAME"))
	      name = cli.getQualValue("/NAME");
	    else {
	      console.log( "Cmd: name is missing\n");
	      return 0;
	    }
	    if ( cli.qualifierFound("/SOURCE"))
	      source = cli.getQualValue("/SOURCE");
	    else {
	      console.log( "Cmd: source is missing\n");
	      return 0;
	    }
	    if ( cli.qualifierFound("/OBJECT"))
	      object = cli.getQualValue("/OBJECT");
	    else
	      object = null;
	    if ( cli.qualifierFound("cli_arg2"))
	      graphstr = cli.getQualValue("cli_arg2").toLowerCase();
	    else {
	      console.log("Syntax error");
	      return 0;
	    }
				
	    if ( source.indexOf('.') == -1)
	      source = source + ".pwg";

	    this.graph.setSubwindowSource( name, source, object);
	  }
	}
      }
      else if ( command == ("HELP")) {
	var fileName = "xtt_help_";
	var bookmarkValue = null;

	if ( cli.qualifierFound("/VERSION")) {
	  fileName = this.pwrHost + "xtt_version_help_version.html";
	  this.openURL( fileName, null);
	}
	else {
	  if ( cli.qualifierFound("/BASE"))
	    // Not language dependent !! TODO
	    fileName = this.pwrHost + "help/xtt_help_";
	  else
	    fileName = "xtt_help_";
			
	  if ( cli.qualifierFound("cli_arg1"))
	    fileName += cli.getQualValue("cli_arg1").toLowerCase();
	  if ( cli.qualifierFound("cli_arg2"))
	    fileName += "_" + cli.getQualValue("cli_arg2").toLowerCase();
	  if ( cli.qualifierFound("cli_arg3"))
	    fileName += "_" + cli.getQualValue("cli_arg3").toLowerCase();
	  if ( cli.qualifierFound("cli_arg4"))
	    fileName += "_" + cli.getQualValue("cli_arg4").toLowerCase();
			
	  if ( fileName.substring(0,5) == "pwrb_" ||
	       fileName.substring(0,5) == "pwrs_" ||
	       fileName.substring(0,5) == "nmps_" ||
	       fileName.substring(0,9) == "profibus_" ||
	       fileName.substring(0,8) == "otherio_" ||
	       fileName.substring(0,4) == "opc_" ||
	       fileName.substring(0,14) == "basecomponent_" ||
	       fileName.substring(0,4) == "abb_" ||
	       fileName.substring(0,8) == "siemens_" ||
	       fileName.substring(0,7) == "ssabox_")
	    // Object reference manual
	    fileName = "$pwr_doc/orm/" + fileName;
			
	  if ( cli.qualifierFound("/BOOKMARK"))
	    bookmarkValue = cli.getQualValue("/BOOKMARK");
			
	  fileName += ".html";
	  console.log( "Loading helpfile \"" + fileName + "\"");
	  this.openURL( fileName, bookmarkValue);
	}
	local_cmd = true;
      }
      else if ( command == ("CHECK")) {
	if ( cli.qualifierFound("cli_arg1")) {

	  var methodstr = "METHOD";
	  var isattributestr = "ISATTRIBUTE";
	  var cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
	  if ( methodstr.length >= cli_arg1.length &&
	       methodstr.substring(0,cli_arg1.length) == (cli_arg1)) {
	    // Command is "CHECK METHOD"
	    var method;
	    var object;

	    if ( cli.qualifierFound("/METHOD"))
	      method = cli.getQualValue("/METHOD");
	    else {
	      console.log( "Cmd: Method is missing\n");
	      return 0;
	    }

	    if ( cli.qualifierFound("/OBJECT"))
	      object = cli.getQualValue("/OBJECT");
	    else {
	      console.log( "Cmd: Object is missing\n");
	      return 0;
	    }
	  }	
	  else if ( isattributestr.length >= cli_arg1.length &&
		    isattributestr.substring(0,cli_arg1.length) == (cli_arg1)) {
	    // Command is "CHECK ISATTRIBUTE"
	    var method;
	    var object;

	    if ( cli.qualifierFound("/OBJECT"))
	      object = cli.getQualValue("/OBJECT");
	    else {
	      console.log( "Cmd: Object is missing\n");
	      return 0;
	    }
	  }
	}
	else if ( command == ("CALL")) {
	  if ( cli.qualifierFound("cli_arg1")) {

	    var parameter = "METHOD";
	    var cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
	    if ( parameter.length >= cli_arg1.length &&
		 parameter.substring(0,cli_arg1.length) == (cli_arg1)) {
	      // Command is "CHECK METHOD"
	      var method;
	      var object;

	      if ( cli.qualifierFound("/METHOD"))
		method = cli.getQualValue("/METHOD");
	      else {
		console.log( "Cmd: Method is missing\n");
		return 0;
	      }

	      if ( cli.qualifierFound("/OBJECT"))
		object = cli.getQualValue("/OBJECT");
	      else {
		console.log( "Cmd: Object is missing\n");
		return 0;
	      }
	    }

	  }
	}
	else if ( command == ("SET")) {
	  return 1;
	}
      }
      else {
	console.log( "JopSpider: Parse error " + cli.getStsString());
	return 0;
      }
    }
    return 1;
  };
  this.script = function( script) {
    console.log( "Appl.script", script);
  };
  this.openURL = function( url, bookmark) {
    console.log( "URL: ", url);
    window.open( url, "_blank");
  }
  this.openConfirmDialog = function( dyn, text, object) {
    var res = confirm( text);
    if ( res)
      dyn.confirmedAction( Glow.eEvent_MB1Click, object);
  };
  this.open_objectgraph_cb = function( id, data, sts, result) {
    if ( (sts & 1) != 0) {
      console.log("param1", result.param1);
      data.location.href = "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      data.document.title = result.param1 + " " + result.fullname;
    }
    else
      data.document.write("Error status " + sts);
  };

}

var CFormatC = {
  not_string: /[^s]/,
  not_bool: /[^t]/,
  not_type: /[^T]/,
  not_primitive: /[^v]/,
  number: /[diefg]/,
  numeric_arg: /[bcdiefguxXm]/,
  json: /[j]/,
  not_json: /[^j]/,
  text: /^[^\x25]+/,
  modulo: /^\x25{2}/,
  placeholder: /^\x25(?:([1-9]\d*)\$|\(([^\)]+)\))?(\+)?(0|'[^$])?(-)?(\d+)?(?:\.(\d+))?([b-gijostTuvxXm])/,
  key: /^([a-z_][a-z_\d]*)/i,
  key_access: /^\.([a-z_][a-z_\d]*)/i,
  index_access: /^\[(\d+)\]/,
  sign: /^[\+\-]/
};

function GlowCFormat(key) {
  this.parse_tree;

  this.format = function() {
        var argv = arguments;
        var cursor = 0, tree_length = this.parse_tree.length, arg, output = '', i, k, ph, pad, pad_character, pad_length, is_positive, sign;
        for (i = 0; i < tree_length; i++) {
            if (typeof this.parse_tree[i] === 'string') {
                output += this.parse_tree[i];
            }
            else if (typeof this.parse_tree[i] === 'object') {
                ph = this.parse_tree[i]; // convenience purposes only
                if (ph.keys) { // keyword argument
                    arg = argv[cursor];
                    for (k = 0; k < ph.keys.length; k++) {
                        if (arg == undefined) {
                            throw new Error(sprintf('[sprintf] Cannot access property "%s" of undefined value "%s"', ph.keys[k], ph.keys[k-1]));
                        }
                        arg = arg[ph.keys[k]];
                    }
                }
                else if (ph.param_no) { // positional argument (explicit)
                    arg = argv[ph.param_no];
                }
                else { // positional argument (implicit)
                    arg = argv[cursor++];
                }

                if (CFormatC.not_type.test(ph.type) && CFormatC.not_primitive.test(ph.type) && arg instanceof Function) {
                    arg = arg();
                }

                if (CFormatC.numeric_arg.test(ph.type) && (typeof arg !== 'number' && isNaN(arg))) {
                    throw new TypeError( "[sprintf] expecting number but found");
                }

                if (CFormatC.number.test(ph.type)) {
                    is_positive = arg >= 0;
                }

                switch (ph.type) {
                    case 'b':
                        arg = parseInt(arg, 10).toString(2);
                        break;
                    case 'c':
                        arg = String.fromCharCode(parseInt(arg, 10));
                        break;
                    case 'd':
                    case 'i':
                        arg = parseInt(arg, 10);
                        break;
                    case 'j':
                        arg = JSON.stringify(arg, null, ph.width ? parseInt(ph.width,10) : 0);
                        break;
                    case 'e':
                        arg = ph.precision ? parseFloat(arg).toExponential(ph.precision) : parseFloat(arg).toExponential();
                        break;
                    case 'f':
                        arg = ph.precision ? parseFloat(arg).toFixed(ph.precision) : parseFloat(arg);
                        break;
                    case 'g':
                        arg = ph.precision ? String(Number(arg.toPrecision(ph.precision))) : parseFloat(arg);
                        break;
                    case 'o':
                        arg = (parseInt(arg, 10) >>> 0).toString(8);
                        break;
                    case 's':
                        arg = String(arg);
                        arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
                        break;
                    case 't':
                        arg = String(!!arg);
                        arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
                        break;
                    case 'T':
                        arg = Object.prototype.toString.call(arg).slice(8, -1).toLowerCase();
                        arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
                        break;
                    case 'u':
                        arg = parseInt(arg, 10) >>> 0;
                        break;
                    case 'v':
                        arg = arg.valueOf();
                        arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
                        break;
                    case 'x':
                         arg = (parseInt(arg, 10) >>> 0).toString(16);
                        break;
                    case 'X':
                        arg = (parseInt(arg, 10) >>> 0).toString(16).toUpperCase();
                        break;
                    case 'm':
                        arg = "Msg: " + parseInt(arg, 10);
                        break;
                }
                if (CFormatC.json.test(ph.type)) {
                    output += arg;
                }
                else {
                    if (CFormatC.number.test(ph.type) && (!is_positive || ph.sign)) {
                        sign = is_positive ? '+' : '-';
                        arg = arg.toString().replace(CFormatC.sign, '');
                    }
                    else {
                        sign = '';
                    }
                    pad_character = ph.pad_char ? ph.pad_char === '0' ? '0' : ph.pad_char.charAt(1) : ' ';
                    pad_length = ph.width - (sign + arg).length;
		    var pad_rpt = '';
		    for ( var i = 0; i < pad_length; i++)
		      pad_rpt += pad_character;
                    pad = ph.width ? (pad_length > 0 ? pad_rpt : '') : '';
                    output += ph.align ? sign + arg + pad : (pad_character === '0' ? sign + pad + arg : pad + sign + arg);
                }
            }
        }
        return output;
    };

    // var sprintf_cache = Object.create(null)

    this.parse = function(fmt) {
        // if (sprintf_cache[fmt]) {
        //    return sprintf_cache[fmt]
        //}

        var _fmt = fmt, match, parse_tree = [], arg_names = 0;
        while (_fmt) {
            if ((match = CFormatC.text.exec(_fmt)) !== null) {
                parse_tree.push(match[0]);
            }
            else if ((match = CFormatC.modulo.exec(_fmt)) !== null) {
                parse_tree.push('%');
            }
            else if ((match = CFormatC.placeholder.exec(_fmt)) !== null) {
                if (match[2]) {
                    arg_names |= 1;
                    var field_list = [], replacement_field = match[2], field_match = [];
                    if ((field_match = CFormatC.key.exec(replacement_field)) !== null) {
                        field_list.push(field_match[1]);
                        while ((replacement_field = replacement_field.substring(field_match[0].length)) !== '') {
                            if ((field_match = CFormatC.key_access.exec(replacement_field)) !== null) {
                                field_list.push(field_match[1]);
                            }
                            else if ((field_match = CFormatC.index_access.exec(replacement_field)) !== null) {
                                field_list.push(field_match[1]);
                            }
                            else {
                                throw new SyntaxError('[sprintf] failed to parse named argument key');
                            }
                        }
                    }
                    else {
                        throw new SyntaxError('[sprintf] failed to parse named argument key');
                    }
                    match[2] = field_list;
                }
                else {
                    arg_names |= 2;
                }
                if (arg_names === 3) {
                    throw new Error('[sprintf] mixing positional and named placeholders is not (yet) supported');
                }

                parse_tree.push(
                    {
                        placeholder: match[0],
                        param_no:    match[1],
                        keys:        match[2],
                        sign:        match[3],
                        pad_char:    match[4],
                        align:       match[5],
                        width:       match[6],
                        precision:   match[7],
                        type:        match[8]
                    }
                );
            }
            else {
                throw new SyntaxError('[sprintf] unexpected placeholder');
            }
            _fmt = _fmt.substring(match[0].length);
        }
        return parse_tree;
    };

  this.parse_tree = this.parse( key);
}

var appl = new Appl();

