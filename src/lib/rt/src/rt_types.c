/* co_types.c -- Translation of enums and masks

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_ssabclasses.h"
#include "pwr_nmpsclasses.h"
#include "rt_mh_net.h"
#include "rt_types.h"

/* Enums */

static types_sEnumElement elem_OperatingSystem[] = {
	{ (unsigned int) pwr_mOpSys_VAX_ELN	, "VAXELN"},
	{ (unsigned int) pwr_mOpSys_VAX_VMS	, "OpenVMS on VAX"},
	{ (unsigned int) pwr_mOpSys_AXP_VMS	, "OpenVMS on Alpha"},
	{ (unsigned int) pwr_mOpSys_PPC_LYNX	, "LynxOS on PowerPC"},
	{ (unsigned int) pwr_mOpSys_X86_LYNX	, "LynxOS on x86"},
	{ (unsigned int) pwr_mOpSys_PPC_LINUX	, "Linux on PowerPC"},
	{ (unsigned int) pwr_mOpSys_X86_LINUX 	, "Linux on x86"},
	{ (unsigned int) pwr_mOpSys_AXP_LINUX	, "Linux on Alpha"},
	{ 0, ""}};

static types_sEnumElement elem_EventType[] = {
	{ (unsigned int) mh_eEvent_Info		, "Info"},
	{ (unsigned int) mh_eEvent_Alarm	, "Alarm"},
	{ 0, ""}};

static types_sEnumElement elem_EventPrio[] = {
	{ (unsigned int) mh_eEventPrio_A	, "A"},
	{ (unsigned int) mh_eEventPrio_B	, "B"},
	{ (unsigned int) mh_eEventPrio_C	, "C"},
	{ (unsigned int) mh_eEventPrio_D	, "D"},
	{ 0, ""}};

static types_sEnumElement elem_DocumentOrientation[] = {
	{ 0	, "Portrait"},
	{ 1	, "Landscape"},
	{ 0, ""}};

static types_sEnumElement elem_DocumentSize[] = {
	{ 0	, "A0"},
	{ 1	, "A1"},
	{ 2	, "A2"},
	{ 3	, "A3"},
	{ 4	, "A4"},
	{ 5	, "A5"},
	{ 0, ""}};

static types_sEnumElement elem_Ai_FilterType[] = {
	{ 0	, "No"},
	{ 1	, "Exponential"},
	{ 0, ""}};

static types_sEnumElement elem_Di_FilterType[] = {
	{ 0	, "No"},
	{ 1	, "SetAndResetDelay"},
	{ 0, ""}};

static types_sEnumElement elem_TextAttribute[] = {
	{ 0	, "SmallBold"},
	{ 1	, "Small"},
	{ 2	, "Medium"},
	{ 3	, "Large"},
	{ 0, ""}};

static types_sEnumElement elem_FrameAttribute[] = {
	{ 0	, "Grey"},
	{ 1	, "Black"},
	{ 2	, "No"},
	{ 0, ""}};

static types_sEnumElement elem_WebTarget[] = {
	{ 0	, "RightFrame"},
	{ 1	, "ParentWindow"},
	{ 2	, "SeparateWindow"},
	{ 0, ""}};

static types_sEnumElement elem_Mode_OpMod[] = {
	{ 1	, "Manual"},
	{ 2	, "Auto"},
	{ 4	, "Cascade"},
	{ 0, ""}};

static types_sEnumElement elem_Mode_AccMod[] = {
	{ 1	, "Manual"},
	{ 2	, "Auto"},
	{ 3	, "Manual / Auto"},
	{ 4	, "Cascade"},
	{ 5	, "Cascade / Manual"},
	{ 6	, "Cascade / Auto"},
	{ 7	, "Cascade / Auto / Manual"},
	{ 0, ""}};

static types_sEnumElement elem_Node_EmergBreakSelect[] = {
	{ 0	, "NoAction"},
	{ 1	, "Reboot"},
	{ 2	, "FixedOutputValues"},
	{ 3	, "Stop I/O"},
	{ 0, ""}};

static types_sEnumElement elem_Pid_PidAlg[] = {
	{ 1	, "I"},
	{ 3	, "I+P"},
	{ 6	, "P"},
	{ 7	, "PI"},
	{ 11	, "I+PD"},
	{ 14	, "P+D"},
	{ 15	, "PI+D"},
	{ 30	, "PD"},
	{ 31	, "PID"},
	{ 0, ""}};

static types_sEnumElement elem_CSup_DelayAction[] = {
	{ 0	, "No"},
	{ 1	, "Message"},
 	{ 2	, "EmergencyBreak"},
	{ 0, ""}};

static types_sEnumElement elem_ChanAi_SensorPolyType[] = {
	{ 0	, "SignalValue"},
	{ 1	, "Linear"},
	{ 2	, "Parabolic"},
	{ 3	, "SquareRoot"},
	{ 4	, "Signed SquareRoot"},
	{ 0, ""}};

static types_sEnumElement elem_Co_PI24BO_NoOfBits[] = {
	{ 16	, "16"},
	{ 24	, "24"},
	{ 0, ""}};

static types_sEnumElement elem_Co_PI24BO_MulCount[] = {
	{ 2	, "2"},
	{ 4	, "4"},
	{ 0, ""}};

static types_sEnumElement elem_Co_PI24BO_DivCount[] = {
	{ 0	, "No"},
	{ 16	, "16"},
	{ 0, ""}};



/* Masks */

static types_sEnumElement mask_Attribute_Flags[] = {
	{ pwr_mAdef_pointer	, "Pointer"},
	{ pwr_mAdef_array	, "Array"},
	{ pwr_mAdef_backup	, "Backup"},
	{ pwr_mAdef_changelog	, "ChangeLog"},
	{ pwr_mAdef_state	, "State"},
	{ pwr_mAdef_const	, "Const"},
	{ pwr_mAdef_rtvirtual	, "RtVirtual"},
	{ pwr_mAdef_devbodyref	, "DevBodyRef"},
	{ pwr_mAdef_dynamic	, "Dynamic"},
	{ pwr_mAdef_objidself	, "ObjidSelf"},
	{ pwr_mAdef_noedit	, "NoEdit"},
	{ pwr_mAdef_invisible	, "Invisible"},
	{ pwr_mAdef_refdirect	, "RefDirect"},
	{ pwr_mAdef_noinvert	, "NoInvert"},
	{ pwr_mAdef_noremove	, "NoRemove"},
	{ pwr_mAdef_rtdbref	, "RtdbRef"},
	{ pwr_mAdef_private	, "Private"},
	{ 0, ""}};

static types_sEnumElement mask_ClassDef_Flags[] = {
	{ pwr_mClassDef_DevOnly	, "DevOnly"},
	{ pwr_mClassDef_System	, "System"},
	{ pwr_mClassDef_Multinode , "Multinode"},
	{ pwr_mClassDef_ObjXRef	, "ObjXRef"},
	{ pwr_mClassDef_RtBody	, "RtBody"},
	{ pwr_mClassDef_AttrXRef, "AttrXRef"},
	{ pwr_mClassDef_ObjRef	, "ObjRef"},
	{ pwr_mClassDef_AttrRef	, "AttrRef"},
	{ pwr_mClassDef_TopObject , "TopObject"},
	{ pwr_mClassDef_NoAdopt	, "NoAdopt"},
	{ pwr_mClassDef_Template, "Template"},
	{ pwr_mClassDef_IO	, "IO"},
	{ 0, ""}};

static types_sEnumElement mask_ObjBodyDef_Flags[] = {
	{ pwr_mAdef_rtvirtual	, "RtVirtual"},
	{ 0, ""}};

static types_sEnumElement mask_CardMask1[] = {
	{ 1 << 0	, "Channel 1"},
	{ 1 << 1	, "Channel 2"},
	{ 1 << 2	, "Channel 3"},
	{ 1 << 3	, "Channel 4"},
	{ 1 << 4	, "Channel 5"},
	{ 1 << 5	, "Channel 6"},
	{ 1 << 6	, "Channel 7"},
	{ 1 << 7	, "Channel 8"},
	{ 1 << 8	, "Channel 9"},
	{ 1 << 9	, "Channel 10"},
	{ 1 << 10	, "Channel 11"},
	{ 1 << 11	, "Channel 12"},
	{ 1 << 12	, "Channel 13"},
	{ 1 << 13	, "Channel 14"},
	{ 1 << 14	, "Channel 15"},
	{ 1 << 15	, "Channel 16"},
	{ 0, ""}};

static types_sEnumElement mask_CardMask2[] = {
	{ 1 << 0	, "Channel 17"},
	{ 1 << 1	, "Channel 18"},
	{ 1 << 2	, "Channel 19"},
	{ 1 << 3	, "Channel 20"},
	{ 1 << 4	, "Channel 21"},
	{ 1 << 5	, "Channel 22"},
	{ 1 << 6	, "Channel 23"},
	{ 1 << 7	, "Channel 24"},
	{ 1 << 8	, "Channel 25"},
	{ 1 << 9	, "Channel 26"},
	{ 1 << 10	, "Channel 27"},
	{ 1 << 11	, "Channel 28"},
	{ 1 << 12	, "Channel 29"},
	{ 1 << 13	, "Channel 30"},
	{ 1 << 14	, "Channel 31"},
	{ 1 << 15	, "Channel 32"},
	{ 0, ""}};

static types_sEnumElement mask_EventFlags[] = {
	{ (unsigned int) mh_mEventFlags_Return	, "Return"},
	{ (unsigned int) mh_mEventFlags_Ack	, "Ack"},
	{ (unsigned int) mh_mEventFlags_Bell	, "Bell"},
	{ (unsigned int) mh_mEventFlags_Force	, "Force"},
	{ (unsigned int) mh_mEventFlags_InfoWindow , "InfoWindow"},
	{ (unsigned int) mh_mEventFlags_Returned , "Returned"},
	{ (unsigned int) mh_mEventFlags_NoObject , "NoObject"},
	{ 0, ""}};

static types_sEnumElement mask_NMpsCell_Function[] = {
	{ 1 << 2	, "DeleteWhenRemove"},
	{ 1 << 3	, "DeteteWhenReset"},
	{ 0, ""}};
static types_sEnumElement mask_NMpsOutCell_Function[] = {
	{ 1 << 2	, "DeleteWhenRemove"},
	{ 0, ""}};
static types_sEnumElement mask_NMpsStoreCell_Function[] = {
	{ 1 << 0	, "MaxOneSelected"},
	{ 1 << 2	, "AlwaysOneSelected"},
	{ 1 << 2	, "DeleteWhenRemove"},
	{ 1 << 3	, "DeteteWhenReset"},
	{ 0, ""}};

static types_sEnumElement mask_NMpsTrp_Function[] = {
	{ 0	, "MoveFront/Rear"},
	{ 1	, "MoveWholeObject"},
	{ 2	, "MoveAllObjects"},
	{ 4	, "MoveAllObjectsIfEmpty"},
	{ 8	, "ResetTrigg"},
	{ 16	, "Slow"},
	{ 32	, "LevelTrigg"},
	{ 0, ""}};

static types_sEnum enum_types[] = {
{ pwr_cClass_Ai, "FilterType",(types_sEnumElement *) &elem_Ai_FilterType},
{ pwr_cClass_Di, "FilterType",(types_sEnumElement *) &elem_Di_FilterType},
{ pwr_cClass_mode, "OpMod",(types_sEnumElement *) &elem_Mode_OpMod},
{ pwr_cClass_mode, "AccMod",(types_sEnumElement *) &elem_Mode_AccMod},
{ pwr_eClass_Node, "EmergBreakSelect",(types_sEnumElement *) &elem_Node_EmergBreakSelect},
{ pwr_cClass_pid, "PidAlg",(types_sEnumElement *) &elem_Pid_PidAlg},
{ pwr_cClass_CycleSup, "DelayAction",(types_sEnumElement *) &elem_CSup_DelayAction},
{ pwr_cClass_ChanAi, "SensorPolyType",(types_sEnumElement *) &elem_ChanAi_SensorPolyType},
{ pwr_cClass_Co_PI24BO, "NoOfBits",(types_sEnumElement *) &elem_Co_PI24BO_NoOfBits},
{ pwr_cClass_Co_PI24BO, "MulCount",(types_sEnumElement *) &elem_Co_PI24BO_MulCount},
{ pwr_cClass_Co_PI24BO, "DivCount",(types_sEnumElement *) &elem_Co_PI24BO_DivCount},
{ 0, 		"OperatingSystem", (types_sEnumElement *) &elem_OperatingSystem},
{ 0, 		"EventType",	(types_sEnumElement *) &elem_EventType},
{ 0, 		"EventPriority",(types_sEnumElement *) &elem_EventPrio},
{ 0, 		"TextAttribute",(types_sEnumElement *) &elem_TextAttribute},
{ 0, 		"FrameAttribute",(types_sEnumElement *) &elem_FrameAttribute},
{ 0, 		"DocumentOrientation",(types_sEnumElement *) &elem_DocumentOrientation},
{ 0, 		"DocumentSize",(types_sEnumElement *) &elem_DocumentSize},
{ 0, 		"WebTarget",(types_sEnumElement *) &elem_WebTarget},
{ 0, "", NULL}};

static types_sEnum mask_types[] = {
{ pwr_eClass_Param, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_Input, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_Output, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_Intern, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_AttrXRef, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_ObjXRef, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_Buffer, "Flags", (types_sEnumElement *) &mask_Attribute_Flags},
{ pwr_eClass_ClassDef, "Flags", (types_sEnumElement *) &mask_ClassDef_Flags},
{ pwr_eClass_ObjBodyDef, "Flags", (types_sEnumElement *) &mask_ObjBodyDef_Flags},
{ pwr_cClass_NMpsCell, "Function", (types_sEnumElement *) &mask_NMpsCell_Function},
{ pwr_cClass_NMpsOutCell, "Function", (types_sEnumElement *) &mask_NMpsOutCell_Function},
{ pwr_cClass_NMpsStoreCell, "Function", (types_sEnumElement *) &mask_NMpsStoreCell_Function},
{ pwr_cClass_NMpsTrp, "Function",(types_sEnumElement *) &mask_NMpsTrp_Function},
{ pwr_cClass_NMpsTrpFF, "Function",(types_sEnumElement *) &mask_NMpsTrp_Function},
{ pwr_cClass_NMpsTrpRR, "Function",(types_sEnumElement *) &mask_NMpsTrp_Function},
{ 0, "OperatingSystem", (types_sEnumElement *) &elem_OperatingSystem},
{ 0, "EventFlags", 	(types_sEnumElement *) &mask_EventFlags},
{ 0, "ConvMask", 	(types_sEnumElement *) &mask_CardMask1},
{ 0, "ConvMask1", 	(types_sEnumElement *) &mask_CardMask1},
{ 0, "ConvMask2", 	(types_sEnumElement *) &mask_CardMask2},
{ 0, "InvMask1", 	(types_sEnumElement *) &mask_CardMask1},
{ 0, "InvMask2", 	(types_sEnumElement *) &mask_CardMask2},
{ 0, "TestMask1", 	(types_sEnumElement *) &mask_CardMask1},
{ 0, "TextMask2", 	(types_sEnumElement *) &mask_CardMask2},
{ 0, "", NULL}};

int types_find_enum( pwr_tClassId classid, char *name, 
	types_sEnumElement **elem_p)
{
  types_sEnum		*enum_p;

  for ( enum_p = enum_types; enum_p->elements; enum_p++)
  {
    if ( classid == enum_p->classid || enum_p->classid == 0)
    {
      if ( strcmp( name, enum_p->name) == 0)
      {
        *elem_p = enum_p->elements;
        return 1;
      }
    }
  }
  return 0;
}

int types_translate_enum( pwr_tClassId classid, char *name, unsigned int num, 
	char *buf)
{
  types_sEnum		*enum_p;
  types_sEnumElement	*elem_p;
  int			found;

  for ( enum_p = enum_types; enum_p->elements; enum_p++)
  {
    if ( classid == enum_p->classid || enum_p->classid == 0)
    {
      if ( strcmp( name, enum_p->name) == 0)
      {
        elem_p = enum_p->elements;
        found = 1;
        break;
      }
    }
  }
  if ( !found)
    return 0;


  for ( ; elem_p->name[0] != 0; elem_p++)
  {
    if ( elem_p->num == num)
    {
      strcpy( buf, elem_p->name);
      return 1;
    }
  }
  return 0;
}

int types_find_mask( pwr_tClassId classid, char *name, 
		types_sEnumElement **elem_p)
{
  types_sEnum		*enum_p;

  for ( enum_p = mask_types; enum_p->elements; enum_p++)
  {
    if ( classid == enum_p->classid || enum_p->classid == 0)
    {
      if ( strcmp( name, enum_p->name) == 0)
      {
        *elem_p = enum_p->elements;
        return 1;
      }
    }
  }
  return 0;
}

