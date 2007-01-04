/* 
 * Proview   $Id: ge_convert.cpp,v 1.5 2007-01-04 08:18:35 claes Exp $
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

/* ge_convert.cpp
   Conversion between different versions of Ge. 
   3.4 -> 4.0 
*/


#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

extern "C" {
#include "pwr_class.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_dcli.h"
#include "co_ccm_msg.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>
#include <Xm/SelectioB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"

#include "ge_graph.h"
#include "co_dcli_msg.h"
#include "ge_msg.h"
#include "ge.h"
#include "ge_dyn.h"

extern "C" {
#include "flow_x.h"
#if LDH
# include "wb_wccm.h"
#endif
}

#if defined OS_LINUX
static void get_class_dyntype( int trace_type, int *d, int *a, int *anim_sequence_p)
{
  int dyn_type;
  int action_type;

    switch ( trace_type) {
    case graph_eTrace_Dig:
      dyn_type = ge_mDynType_DigLowColor;
      action_type = 0;
      break;
    case graph_eTrace_DigWithError:
      dyn_type = ge_mDynType_DigLowColor | ge_mDynType_DigError;
      action_type = 0;
      break;
    case graph_eTrace_DigTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = 0;
      break;
    case graph_eTrace_DigToneWithError:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor | ge_mDynType_DigError;
      action_type = 0;
      break;
    case graph_eTrace_Annot:
      dyn_type = ge_mDynType_Value;
      action_type = 0;
      break;
    case graph_eTrace_DigWithText:
      dyn_type = ge_mDynType_DigLowColor | ge_mDynType_DigText;
      action_type = 0;
      break;
    case graph_eTrace_Bar:
      dyn_type = ge_mDynType_Bar;
      action_type = 0;
      break;
    case graph_eTrace_Trend:
      dyn_type = ge_mDynType_Trend;
      action_type = 0;
      break;
    case graph_eTrace_DigBorder:
      dyn_type = ge_mDynType_DigBorder;
      action_type = 0;
      break;
    case graph_eTrace_AnnotWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_Value | ge_mDynType_DigLowColor;
      action_type = 0;
      break;
    case graph_eTrace_DigTwo:
      dyn_type = ge_mDynType_DigLowColor | ge_mDynType_DigColor;
      action_type = 0;
      break;
    case graph_eTrace_DigToneTwo:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor | ge_mDynType_DigColor;
      action_type = 0;
      break;
    case graph_eTrace_Invisible:
      dyn_type = ge_mDynType_Invisible;
      action_type = 0;
      break;
    case graph_eTrace_Rotate:
      dyn_type = ge_mDynType_Rotate;
      action_type = 0;
      break;
    case graph_eTrace_AnalogShift:
      dyn_type = ge_mDynType_AnalogShift;
      action_type = 0;
      break;
    case graph_eTrace_Animation: 
      dyn_type = ge_mDynType_Animation;
      action_type = 0;
      *anim_sequence_p = (int) ge_eAnimSequence_Cycle;
      break;
    case graph_eTrace_DigAnimation:
      dyn_type = ge_mDynType_Animation;
      action_type = 0;
      *anim_sequence_p = (int) ge_eAnimSequence_Dig;
      break;
    case graph_eTrace_AnimationForwBack:
      dyn_type = ge_mDynType_Animation;
      action_type = 0;
      *anim_sequence_p = (int) ge_eAnimSequence_ForwBack;
      break;
    case graph_eTrace_DigShift:
      dyn_type = ge_mDynType_DigShift;
      action_type = 0;
      break;
    case graph_eTrace_Move:
      dyn_type = ge_mDynType_Move;
      action_type = 0;
      break;
    case graph_eTrace_SliderBackground:
      dyn_type = ge_mDynType_SliderBackground;
      action_type = 0;
      break;
    case graph_eTrace_Video:
      dyn_type = ge_mDynType_Video;
      action_type = 0;
      break;
    case graph_eTrace_No:
      dyn_type = 0;
      action_type = 0;
      break;
    case graph_eTrace_SetDig:
      dyn_type = 0;
      action_type = ge_mActionType_SetDig;
      break;
    case graph_eTrace_ResetDig:
      dyn_type = 0;
      action_type = ge_mActionType_ResetDig;
      break;
    case graph_eTrace_ToggleDig:
      dyn_type = 0;
      action_type = ge_mActionType_ToggleDig;
      break;
    case graph_eTrace_Slider:
      dyn_type = 0;
      action_type = ge_mActionType_Slider;
      break;
    case graph_eTrace_AnnotInput:
      dyn_type = ge_mDynType_Value;
      action_type = ge_mActionType_ValueInput;
      break;
    case graph_eTrace_Command:
      dyn_type = 0;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_CommandConfirm:
      dyn_type = 0;
      action_type = ge_mActionType_Command | ge_mActionType_Confirm;
      break;
    case graph_eTrace_SetDigConfirm:
      dyn_type = 0;
      action_type = ge_mActionType_SetDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_ResetDigConfirm:
      dyn_type = 0;
      action_type = ge_mActionType_ResetDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_ToggleDigConfirm:
      dyn_type = 0;
      action_type = ge_mActionType_ToggleDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_SetDigWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_SetDig;
      break;
    case graph_eTrace_ResetDigWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_ResetDig;
      break;
    case graph_eTrace_ToggleDigWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_ToggleDig;
      break;
    case graph_eTrace_AnnotInputWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_Value | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_ValueInput;
      break;
    case graph_eTrace_SetDigConfirmWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_SetDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_ResetDigConfirmWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_ResetDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_ToggleDigConfirmWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_ToggleDig | ge_mActionType_Confirm;
      break;
    case graph_eTrace_DigWithCommand:
      dyn_type = ge_mDynType_DigLowColor;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_DigWithErrorAndCommand:
      dyn_type = ge_mDynType_DigLowColor | ge_mDynType_DigError;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_DigToneWithCommand:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_DigToneWithErrorAndCommand:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor | ge_mDynType_DigError;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_StoDigWithTone:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigLowColor;
      action_type = ge_mActionType_StoDig;
      break;
    case graph_eTrace_DigTwoWithCommand:
      dyn_type = ge_mDynType_DigLowColor | ge_mDynType_DigColor;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_DigToneTwoWithCommand:
      dyn_type = ge_mDynType_Tone | ge_mDynType_DigColor;
      action_type = ge_mActionType_Command;
      break;
    case graph_eTrace_IncrAnalog:
      dyn_type = 0;
      action_type = ge_mActionType_IncrAnalog;
      break;
    case graph_eTrace_RadioButton:
      dyn_type = 0;
      action_type = ge_mActionType_RadioButton;
      break;
    case graph_eTrace_DigShiftWithToggleDig:
      dyn_type = ge_mDynType_DigShift;
      action_type = ge_mActionType_ToggleDig;
      break;
    default:
      dyn_type = ge_mDynType_No;
      action_type = ge_mActionType_No;
    }
    *d = dyn_type;
    *a = action_type;
}

int Graph::convert()
{


  grow_Convert( grow->ctx, glow_eConvert_V34);

  if ( grow_IsSubGraph( grow->ctx)) {
    // Convert trace_type to dyn_type and dyn_action_type
    grow_sAttrInfo	*grow_info_sub;
    grow_sAttrInfo	*grow_info_p;
    int			grow_info_cnt;
    char transtab[][32] = {	 	"DynType",		"DynType",
					"DynActionType",	"Action",
					"DynColor1",		"Color1",
					"DynColor2",		"Color2",
					"DynColor3",		"Color3",
					"DynColor4",		"Color4",
					"DynAttr1",		"AnimSequence",	
					"DynAttr2",		"",
					"DynAttr3",		"",
					"DynAttr4",		"",
					"Dynamic",		"",
					""};

	
    grow_GetSubGraphAttrInfo( grow->ctx, (char *)transtab, &grow_info_sub, 
				  &grow_info_cnt);
    grow_info_p = grow_info_sub;
    int *dyn_type_p = (int *)grow_info_p[0].value_p;
    int *action_type_p = (int *)grow_info_p[1].value_p;
    int *anim_sequence_p = (int *)grow_info_p[6].value_p;
   
    int trace_type = *(int *)grow_info_p->value_p;
    int dyn_type;
    int action_type;

    get_class_dyntype( trace_type, &dyn_type, &action_type, anim_sequence_p);

    *dyn_type_p = dyn_type;
    *action_type_p = action_type;

    if ( grow_IsSlider( grow->ctx))
      *action_type_p |= ge_mActionType_Slider;
    grow_FreeSubGraphAttrInfo( grow_info_sub);
  }
  //  else {
    grow_tObject 	*objectlist, *object_p;
    int 		object_cnt;
    int		i;
    grow_tNodeClass	*nodeclass_list;
    int			nodeclass_count;

    // Check first that all nodeclasses are intern
    grow_GetNodeClassList( grow->ctx, &nodeclass_list, &nodeclass_count);
    for ( i = 0; i < nodeclass_count; i++) {
      if ( grow_NodeClassIsExtern( nodeclass_list[i])) {
	// Assume already converted
	message( 'E', "Extern nodeclass found, conversion aborted");
	printf("** Extern nodeclass, conversion aborted\n");
	return 0;
      }
    }

    // Convert all objects
    grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);
    object_p = objectlist;
    for ( i = 0; i < object_cnt; i++) {
      if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode ||
	   grow_GetObjectType( *object_p) == glow_eObjectType_GrowSlider ||
	   grow_GetObjectType( *object_p) == glow_eObjectType_GrowGroup ||
	   grow_GetObjectType( *object_p) == glow_eObjectType_GrowBar ||
	   grow_GetObjectType( *object_p) == glow_eObjectType_GrowTrend) {
	convert_object( *object_p);
      }
      object_p++;
    } 

    // Convert all nodeclasses
    int			nc_dyn_type, nc_action_type;

    grow_GetNodeClassList( grow->ctx, &nodeclass_list, &nodeclass_count);
    for ( i = 0; i < nodeclass_count; i++) {
      grow_sAttrInfo	*grow_info_sub;
      grow_sAttrInfo	*grow_info_p;
      int		grow_info_cnt;

      if ( grow_NodeClassIsExtern( nodeclass_list[i]))
	// Assume already converted
	continue;

      grow_GetObjectAttrInfo( nodeclass_list[i], (char *)0, &grow_info_sub, 
				  &grow_info_cnt);
      grow_info_p = grow_info_sub;
      int *dyn_type_p = (int *)grow_info_p[6].value_p;
      int *action_type_p = (int *)grow_info_p[7].value_p;
      int *anim_sequence_p = (int *)grow_info_p[5].value_p;
   
      grow_GetNodeClassDynType( nodeclass_list[i], &nc_dyn_type, &nc_action_type);
      get_class_dyntype( nc_dyn_type, &nc_dyn_type, &nc_action_type, anim_sequence_p);
      *dyn_type_p = nc_dyn_type;
      *action_type_p = nc_action_type;

      if ( grow_IsSliderClass( nodeclass_list[i]))
	*action_type_p |= ge_mActionType_Slider;
      grow_FreeObjectAttrInfo( grow_info_sub);

      // Convert all objects
      grow_GetNodeClassObjectList( nodeclass_list[i], &objectlist, &object_cnt);
      object_p = objectlist;
      for ( int j = 0; j < object_cnt; j++) {
	if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode ||
	     grow_GetObjectType( *object_p) == glow_eObjectType_GrowSlider ||
	     grow_GetObjectType( *object_p) == glow_eObjectType_GrowGroup ||
	     grow_GetObjectType( *object_p) == glow_eObjectType_GrowBar ||
	     grow_GetObjectType( *object_p) == glow_eObjectType_GrowTrend) {
	  convert_object( *object_p);
	}
	object_p++;
      } 


    }

    //  }
  grow_Redraw( grow->ctx);
  return 1;
}

int Graph::convert_object( grow_tObject object) 
{
  glow_sTraceData *trace_data;
  int trace_type;
  int dyn_type;
  int action_type;
  glow_eDrawType trace_color;
  glow_eDrawType trace_color2;
  GeDyn *dyn;
  GeDynElem *elem;

  if ( grow_GetObjectType( object) == glow_eObjectType_GrowNode ||
       grow_GetObjectType( object) == glow_eObjectType_GrowGroup ||
       grow_GetObjectType( object) == glow_eObjectType_GrowSlider) {

    grow_GetTraceAttr( object, &trace_data);
    trace_type = trace_data->attr_type;
    trace_color = trace_data->color;
    trace_color2 = trace_data->color2;


    if ( grow_GetObjectType( object) == glow_eObjectType_GrowSlider)
      trace_type = graph_eTrace_Slider;

    if ( trace_type == graph_eTrace_Inherit)
      grow_GetObjectClassDynType( object, &trace_type, &action_type);

    dyn = new GeDyn( this);
    grow_SetUserData( object, (void *) dyn);

    switch ( trace_type) {
    case graph_eTrace_Dig:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigWithError:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigWarning);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigWarning) {
	  GeDigWarning *e = (GeDigWarning *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigTone:
      dyn->total_dyn_type = ge_mDynType_Tone;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigToneWithError:
      dyn->total_dyn_type = ge_mDynType_Tone;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigWarning);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigWarning) {
	  GeDigWarning *e = (GeDigWarning *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Annot:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Value);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Value) {
	  GeValue *e = (GeValue *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  strcpy( e->format, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigWithText:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  strcpy( e->low_text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Bar:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Bar);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Bar) {
	  GeBar *e = (GeBar *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Trend:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Trend);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Trend) {
	  GeTrend *e = (GeTrend *) elem;

	  strcpy( e->attribute1, trace_data->data[0]);
	  strcpy( e->attribute2, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigBorder:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int) dyn->total_dyn_type | ge_mDynType_DigBorder);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int) dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigBorder) {
	  GeDigBorder *e = (GeDigBorder *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_AnnotWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Value);
      if ( strcmp( trace_data->data[1], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[2], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Value) {
	  GeValue *e = (GeValue *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  strcpy( e->format, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigTwo:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigColor);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigColor) {
	  GeDigColor *e = (GeDigColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color2;
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigToneTwo:
      dyn->total_dyn_type = ge_mDynType_Tone;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( strcmp( trace_data->data[1], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigColor);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigColor) {
	  GeDigColor *e = (GeDigColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color2;
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Invisible:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Rotate:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Rotate);
      if ( strcmp( trace_data->data[3], "") != 0) {
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
      }
      if ( strcmp( trace_data->data[4], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Rotate) {
	  GeRotate *e = (GeRotate *) elem;
	  float f;
	  int sts;

	  strcpy( e->attribute, trace_data->data[0]);
	  if ( strcmp( trace_data->data[5], "") != 0) {
	    sts = sscanf( trace_data->data[5], "%f", &f);
	    if ( sts == 0)
	      e->factor = 1.0;
	    else
	      e->factor = f;
	  }
	  else
	    e->factor = 1.0;
	  if ( strcmp( trace_data->data[1], "") != 0 && 
	       strcmp( trace_data->data[2], "") != 0) {
	    sts = sscanf( trace_data->data[1], "%f", &f);
	    if ( sts != 0) {
            e->x0 = f;
            sts = sscanf( trace_data->data[2], "%f", &f);
            if ( sts != 0)
              e->y0 = f;
	    }
	  }
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_AnalogShift:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_AnalogShift);
      if ( strcmp( trace_data->data[1], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[2], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type| ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_AnalogShift) {
	  GeAnalogShift *e = (GeAnalogShift *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Animation: 
    case graph_eTrace_DigAnimation: 
    case graph_eTrace_AnimationForwBack: 
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Animation);
      if ( strcmp( trace_data->data[1], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[2], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Animation) {
	  GeAnimation *e = (GeAnimation *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  if ( trace_type == graph_eTrace_Animation)
	    e->sequence = ge_eAnimSequence_Cycle;
	  else if ( trace_type == graph_eTrace_DigAnimation)
	    e->sequence = ge_eAnimSequence_Dig;
	  if ( trace_type == graph_eTrace_AnimationForwBack)
	    e->sequence = ge_eAnimSequence_ForwBack;
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigShift:
//      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigShift);
      if ( strcmp( trace_data->data[1], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[2], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigShift) {
	  GeDigShift *e = (GeDigShift *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[1]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Move:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Move);
      if ( strcmp( trace_data->data[2], "") != 0) {
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
      }
      if ( strcmp( trace_data->data[3], "") != 0) 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type| ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Move) {
	  GeMove *e = (GeMove *) elem;
	  float f;
	  int sts;

	  strcpy( e->move_x_attribute, trace_data->data[0]);
	  strcpy( e->move_y_attribute, trace_data->data[1]);
	  strcpy( e->scale_x_attribute, trace_data->data[7]);
	  strcpy( e->scale_y_attribute, trace_data->data[8]);
	  if ( strcmp( trace_data->data[4], "") != 0) {
	    sts = sscanf( trace_data->data[4], "%f", &f);
	    if ( sts == 0)
	      e->factor = 1.0;
	    else
	      e->factor = f;
	  }
	  else
	    e->factor = 1.0;
	  if ( strcmp( trace_data->data[5], "") != 0 && 
	       strcmp( trace_data->data[6], "") != 0) {
	    sts = sscanf( trace_data->data[5], "%f", &f);
	    if ( sts != 0) {
            e->x_offset = f;
            sts = sscanf( trace_data->data[6], "%f", &f);
            if ( sts != 0)
              e->y_offset = f;
	    }
	  }
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_SliderBackground:
      dyn->total_dyn_type = ge_mDynType_SliderBackground;
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Video:
      dyn->total_dyn_type = ge_mDynType_Video;
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_No:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_SetDig:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_SetDig);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_SetDig) {
	  GeSetDig *e = (GeSetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ResetDig:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ResetDig);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ResetDig) {
	  GeResetDig *e = (GeResetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ToggleDig:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ToggleDig);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ToggleDig) {
	  GeToggleDig *e = (GeToggleDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Slider:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Slider);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_Slider) {
	  GeSlider *e = (GeSlider *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_AnnotInput:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Value);
      dyn->total_action_type = ge_mActionType_ValueInput;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Value) {
	  GeValue *e = (GeValue *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  strcpy( e->format, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_ValueInput) {
	  GeValueInput *e = (GeValueInput *) elem;
	  float f;
	  int sts;
	
	  if ( strcmp( trace_data->data[3], "") != 0 && 
	       strcmp( trace_data->data[4], "") != 0) {
	    sts = sscanf( trace_data->data[3], "%f", &f);
	    if ( sts != 0) {
            e->min_value = f;
            sts = sscanf( trace_data->data[4], "%f", &f);
            if ( sts != 0)
              e->max_value = f;
	    }
	  }
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_Command:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Command);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_Command) {
	  GeCommand *e = (GeCommand *) elem;

	  strcpy( e->command, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_CommandConfirm:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Command);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_Command) {
	  GeCommand *e = (GeCommand *) elem;

	  strcpy( e->command, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_SetDigConfirm:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_SetDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_SetDig) {
	  GeSetDig *e = (GeSetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ResetDigConfirm:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ResetDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ResetDig) {
	  GeResetDig *e = (GeResetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ToggleDigConfirm:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ToggleDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ToggleDig) {
	  GeToggleDig *e = (GeToggleDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_SetDigWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_SetDig;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_SetDig) {
	  GeSetDig *e = (GeSetDig *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ResetDigWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_ResetDig;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ResetDig) {
	  GeResetDig *e = (GeResetDig *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ToggleDigWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_ToggleDig;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_ToggleDig) {
	  GeToggleDig *e = (GeToggleDig *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_AnnotInputWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Value);
      if ( strcmp( trace_data->data[2], "") != 0) { 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      dyn->total_action_type = ge_mActionType_ValueInput;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_Value) {
	  GeValue *e = (GeValue *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  strcpy( e->format, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->action_type == ge_mActionType_ValueInput) {
	  GeValueInput *e = (GeValueInput *) elem;
	  float f;
	  int sts;
	
	  if ( strcmp( trace_data->data[3], "") != 0 && 
	       strcmp( trace_data->data[4], "") != 0) {
	    sts = sscanf( trace_data->data[3], "%f", &f);
	    if ( sts != 0) {
            e->min_value = f;
            sts = sscanf( trace_data->data[4], "%f", &f);
            if ( sts != 0)
              e->max_value = f;
	    }
	  }
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_SetDigConfirmWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) { 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_SetDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;
	
	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_SetDig) {
	  GeSetDig *e = (GeSetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ResetDigConfirmWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) { 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ResetDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;
	
	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_ResetDig) {
	  GeResetDig *e = (GeResetDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_ToggleDigConfirmWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) { 
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_No;
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_ToggleDig);
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Confirm);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;
	
	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_ToggleDig) {
	  GeToggleDig *e = (GeToggleDig *) elem;

	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_Confirm) {
	  GeConfirm *e = (GeConfirm *) elem;

	  strcpy( e->text, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigWithCommand:
    case graph_eTrace_DigToneWithCommand:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( trace_type == graph_eTrace_DigToneWithCommand)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
      if ( strcmp( trace_data->data[1], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Command);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_CommandDoubleClick);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;
	
	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->action_type == ge_mActionType_Command) {
	  GeCommand *e = (GeCommand *) elem;

	  strcpy( e->command, trace_data->data[3]);
	}
	else if ( elem->action_type == ge_mActionType_CommandDoubleClick) {
	  GeCommandDoubleClick *e = (GeCommandDoubleClick *) elem;

	  strcpy( e->command, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigWithErrorAndCommand:
    case graph_eTrace_DigToneWithErrorAndCommand:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( trace_type == graph_eTrace_DigToneWithErrorAndCommand)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
      if ( strcmp( trace_data->data[1], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type| ge_mDynType_DigWarning);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Command);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_CommandDoubleClick);
      if ( strcmp( trace_data->ref_object, "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;
	
	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigWarning) {
	  GeDigWarning *e = (GeDigWarning *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	}
	else if ( elem->action_type == ge_mActionType_Command) {
	  GeCommand *e = (GeCommand *) elem;

	  strcpy( e->command, trace_data->data[3]);
	}
	else if ( elem->action_type == ge_mActionType_CommandDoubleClick) {
	  GeCommandDoubleClick *e = (GeCommandDoubleClick *) elem;

	  strcpy( e->command, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_StoDigWithTone:
      dyn->total_dyn_type = ge_mDynType_No;
      if ( strcmp( trace_data->data[2], "") != 0) {
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      }
      if ( strcmp( trace_data->data[3], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Invisible);
      if ( strcmp( trace_data->data[4], "") != 0)
 	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigText);
      dyn->total_action_type = ge_mActionType_StoDig;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->action_type == ge_mActionType_StoDig) {
	  GeStoDig *e = (GeStoDig *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;

	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_Invisible) {
	  GeInvisible *e = (GeInvisible *) elem;

	  strcpy( e->attribute, trace_data->data[3]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigText) {
	  GeDigText *e = (GeDigText *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  strcpy( e->low_text, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigTwoWithCommand:
    case graph_eTrace_DigToneTwoWithCommand:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigLowColor);
      if ( trace_type == graph_eTrace_DigToneTwoWithCommand)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Tone);
      if ( strcmp( trace_data->data[1], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigError);
      if ( strcmp( trace_data->data[2], "") != 0)
	dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_DigColor);
      dyn->total_action_type = ge_mActionType_No;
      if ( strcmp( trace_data->data[3], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_Command);
      if ( strcmp( trace_data->data[4], "") != 0)
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_CommandDoubleClick);
      if ( strcmp( trace_data->ref_object, "") != 0)
        dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
	if ( elem->dyn_type == ge_mDynType_DigLowColor) {
	  GeDigLowColor *e = (GeDigLowColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  e->color = trace_data->color;
	}
	else if ( elem->dyn_type == ge_mDynType_DigError) {
	  GeDigError *e = (GeDigError *) elem;
	
	  strcpy( e->attribute, trace_data->data[1]);
	}
	else if ( elem->dyn_type == ge_mDynType_DigColor) {
	  GeDigColor *e = (GeDigColor *) elem;
	
	  strcpy( e->attribute, trace_data->data[2]);
	  e->color = trace_data->color2;
	}
	else if ( elem->action_type == ge_mActionType_Command) {
	  GeCommand *e = (GeCommand *) elem;

	  strcpy( e->command, trace_data->data[3]);
	}
	else if ( elem->action_type == ge_mActionType_CommandDoubleClick) {
	  GeCommandDoubleClick *e = (GeCommandDoubleClick *) elem;

	  strcpy( e->command, trace_data->data[4]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_IncrAnalog:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_IncrAnalog;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
        if ( elem->action_type == ge_mActionType_IncrAnalog) {
	  GeIncrAnalog *e = (GeIncrAnalog *) elem;
	  float f;
	  int sts;
	
	  strcpy( e->attribute, trace_data->data[0]);
	  if ( strcmp( trace_data->data[2], "") != 0 && 
	       strcmp( trace_data->data[3], "") != 0) {
	    sts = sscanf( trace_data->data[2], "%f", &f);
	    if ( sts != 0) {
	      e->min_value = f;
	      sts = sscanf( trace_data->data[3], "%f", &f);
	      if ( sts != 0)
              e->max_value = f;
	    }
	  }
	  if ( strcmp( trace_data->data[1], "") != 0) {
	    sts = sscanf( trace_data->data[1], "%f", &f);
	    if ( sts != 0)
	      e->increment = f;
	  }
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_RadioButton:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_RadioButton;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
        if ( elem->action_type == ge_mActionType_RadioButton) {
	  GeRadioButton *e = (GeRadioButton *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    case graph_eTrace_DigShiftWithToggleDig:
      dyn->total_dyn_type = ge_mDynType_DigShift;
      dyn->total_action_type = ge_mActionType_ToggleDig;
      if ( strcmp( trace_data->ref_object, "") != 0) 
	dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
      dyn->update_elements();
      for ( elem = dyn->elements; elem; elem = elem->next) {
        if ( elem->dyn_type == ge_mDynType_DigShift) {
	  GeDigShift *e = (GeDigShift *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
        else if ( elem->action_type == ge_mActionType_ToggleDig) {
	  GeToggleDig *e = (GeToggleDig *) elem;
	
	  strcpy( e->attribute, trace_data->data[0]);
	}
	else if ( elem->action_type == ge_mActionType_PopupMenu) {
	  GePopupMenu *e = (GePopupMenu *) elem;

	  strcpy( e->ref_object, trace_data->ref_object);
	}
      }
      break;
    default:
      dyn->total_dyn_type = ge_mDynType_No;
      dyn->total_action_type = ge_mActionType_No;

    }

    if ( trace_data->attr_type == (int)graph_eTrace_Inherit) {
      int anim_seq;
      get_class_dyntype( trace_type, &dyn_type, &action_type, &anim_seq);
      dyn->dyn_type =(ge_mDynType)((int)dyn->total_dyn_type & ~dyn_type | ge_mDynType_Inherit);
      dyn->action_type = (ge_mActionType)((int)dyn->total_action_type & ~action_type | ge_mActionType_Inherit);
    }
    else { 
      dyn->dyn_type = dyn->total_dyn_type;
      dyn->action_type = dyn->total_action_type;
    }
    dyn->cycle = trace_data->cycle;
    dyn->access = trace_data->access;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowTrend) {

    grow_GetTraceAttr( object, &trace_data);
    trace_type = trace_data->attr_type;
    trace_color = trace_data->color;
    trace_color2 = trace_data->color2;

    dyn = new GeDyn( this);
    grow_SetUserData( object, (void *) dyn);

    dyn->total_dyn_type = ge_mDynType_No;
    dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Trend);
    dyn->total_action_type = ge_mActionType_No;
    if ( strcmp( trace_data->ref_object, "") != 0) 
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
    dyn->update_elements();
    for ( elem = dyn->elements; elem; elem = elem->next) {
      if ( elem->dyn_type == ge_mDynType_Trend) {
	GeTrend *e = (GeTrend *) elem;
	
	strcpy( e->attribute1, trace_data->data[0]);
	strcpy( e->attribute2, trace_data->data[1]);
      }
      else if ( elem->action_type == ge_mActionType_PopupMenu) {
	GePopupMenu *e = (GePopupMenu *) elem;

	strcpy( e->ref_object, trace_data->ref_object);
      }
    }
    dyn->dyn_type = dyn->total_dyn_type;
    dyn->action_type = dyn->total_action_type;
    dyn->cycle = trace_data->cycle;
    dyn->access = trace_data->access;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowBar) {

    grow_GetTraceAttr( object, &trace_data);
    trace_type = trace_data->attr_type;
    trace_color = trace_data->color;
    trace_color2 = trace_data->color2;

    dyn = new GeDyn( this);
    grow_SetUserData( object, (void *) dyn);

    dyn->total_dyn_type = ge_mDynType_No;
    dyn->total_dyn_type = (ge_mDynType)((int)dyn->total_dyn_type | ge_mDynType_Bar);
    dyn->total_action_type = ge_mActionType_No;
    if ( strcmp( trace_data->ref_object, "") != 0) 
      dyn->total_action_type = (ge_mActionType)((int)dyn->total_action_type | ge_mActionType_PopupMenu);
    dyn->update_elements();
    for ( elem = dyn->elements; elem; elem = elem->next) {
      if ( elem->dyn_type == ge_mDynType_Bar) {
	GeBar *e = (GeBar *) elem;
	
	strcpy( e->attribute, trace_data->data[0]);
      }
      else if ( elem->action_type == ge_mActionType_PopupMenu) {
	GePopupMenu *e = (GePopupMenu *) elem;
	
	strcpy( e->ref_object, trace_data->ref_object);
      }
    }
    dyn->dyn_type = dyn->total_dyn_type;
    dyn->action_type = dyn->total_action_type;
    dyn->cycle = trace_data->cycle;
    dyn->access = trace_data->access;
  }


  if ( grow_GetObjectType( object) == glow_eObjectType_GrowGroup) {
    grow_tObject 	*objectlist, *object_p;
    int 		object_cnt;
    int		i;

    grow_GetGroupObjectList( object, &objectlist, &object_cnt);
    object_p = objectlist;
    for ( i = 0; i < object_cnt; i++) {
      convert_object( *object_p);
      object_p++;
    }
  } 

  return 1;
}

#else

int Graph::convert()
{
  return 1;
}
#endif




