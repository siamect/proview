/* 
 * Proview   $Id: xtt_xnav.cpp,v 1.42 2008-10-31 12:51:36 claes Exp $
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
 */

/* xtt_xnav.cpp -- Display plant and node hierarchy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <vector>

#include "co_nav_help.h"
#include "pwr_privilege.h"
#include "rt_gdh.h"
#include "rt_gdb.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_api_user.h"
#include "co_msg.h"
#include "co_syi.h"
#include "pwr_baseclasses.h"
#include "rt_xnav_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_lng.h"
#include "co_error.h"
#include "co_xhelp.h"
#include "co_wow.h"
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "xtt_menu.h"
#include "xtt_xatt.h"
#include "xtt_xcrr.h"
#include "xtt_ge.h"
#include "xtt_ev.h"
#include "xtt_op.h"
#include "xtt_audio.h"
#include "rt_trace.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

static char null_str[] = "";

//
//  Get trace attribute
//
int XNav::get_trace_attr( pwr_sAttrRef *arp, char *attr)
{
  int sts;
  pwr_tClassId classid;
  pwr_tAName objname;

  sts = gdh_GetAttrRefTid( arp, &classid);
  if ( EVEN(sts)) return sts;

  switch ( classid) {
  case pwr_cClass_Di:
  case pwr_cClass_Dv:
  case pwr_cClass_Do:
  case pwr_cClass_Po:
  case pwr_cClass_Av:
  case pwr_cClass_Ai:
  case pwr_cClass_Ao:
  case pwr_cClass_Ii:
  case pwr_cClass_Io:
  case pwr_cClass_Iv:
  case pwr_cClass_Sv:
    strcpy( attr, "ActualValue");	
    break;   
  case pwr_cClass_ChanDi:
  case pwr_cClass_ChanDo:
  case pwr_cClass_ChanAi:
  case pwr_cClass_ChanAo:
  case pwr_cClass_ChanIi:
  case pwr_cClass_ChanIo:
    sts = gdh_AttrrefToName( arp, objname, sizeof(objname), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    strcat( objname, ".SigChanCon");
    sts = gdh_GetObjectInfo ( objname, arp, sizeof( *arp));
    if (EVEN(sts)) return sts;
    strcpy( attr, "ActualValue");
    break;
  case pwr_cClass_ChanCo:
    sts = gdh_AttrrefToName ( arp, objname, sizeof(objname), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    strcat( objname, ".SigChanCon");
    sts = gdh_GetObjectInfo ( objname, arp, sizeof( *arp));
    if (EVEN(sts)) return sts;
    strcpy( attr, "AbsValue");	
    break;
  case pwr_cClass_trans :
    strcpy( attr, "Cond");
    break;   
  case pwr_cClass_order :
  case pwr_cClass_dorder :
  case pwr_cClass_porder :
  case pwr_cClass_lorder :
  case pwr_cClass_sorder :
    strcpy( attr, "Status[0]"); /* Status[0] */
    break;   
  case pwr_cClass_ssbegin :
  case pwr_cClass_ssend :
  case pwr_cClass_step :
  case pwr_cClass_initstep :
  case pwr_cClass_substep :
    strcpy( attr, "Order[0]"); /* Order[0] */
    break;   
  case pwr_cClass_cstoao :
  case pwr_cClass_cstoav :
  case pwr_cClass_cstoap :
    strcpy( attr, "Cond");
    break;   
  case pwr_cClass_and :
  case pwr_cClass_or :
  case pwr_cClass_xor :
  case pwr_cClass_edge :
  case pwr_cClass_sr_s :
  case pwr_cClass_sr_r :
  case pwr_cClass_pulse :
  case pwr_cClass_wait :
  case pwr_cClass_timer :
  case pwr_cClass_inv :
  case pwr_cClass_waith :
  case pwr_cClass_darithm :
  case pwr_cClass_Even :
  case pwr_cClass_Odd :
  case pwr_cClass_Equal :
  case pwr_cClass_GreaterEqual :
  case pwr_cClass_GreaterThan :
  case pwr_cClass_LessEqual :
  case pwr_cClass_LessThan :
    strcpy( attr, "Status");
    break;   
  case pwr_cClass_DSup :
  case pwr_cClass_ASup :
    strcpy( attr, "Action");
    break;   
  case pwr_cClass_csub :
    strcpy( attr, "in");
    break;   
  case pwr_cClass_sum :
  case pwr_cClass_limit :
  case pwr_cClass_select :
  case pwr_cClass_ramp :
  case pwr_cClass_filter :
  case pwr_cClass_speed :
  case pwr_cClass_curve :
  case pwr_cClass_adelay :
  case pwr_cClass_aarithm :
  case pwr_cClass_timint :
  case pwr_cClass_IAbs :
  case pwr_cClass_IAdd :
  case pwr_cClass_IDiv :
  case pwr_cClass_ILimit :
  case pwr_cClass_IMax :
  case pwr_cClass_IMin :
  case pwr_cClass_IMul :
  case pwr_cClass_ISel :
  case pwr_cClass_ISub :
  case pwr_cClass_IMux :
  case pwr_cClass_Add :
  case pwr_cClass_Div :
  case pwr_cClass_Mul :
  case pwr_cClass_Max :
  case pwr_cClass_Min :
  case pwr_cClass_Mux :
  case pwr_cClass_Sub :
  case pwr_cClass_Sin :
  case pwr_cClass_Cos :
  case pwr_cClass_Tan :
  case pwr_cClass_ASin :
  case pwr_cClass_ACos :
  case pwr_cClass_ATan :
  case pwr_cClass_Exp :
  case pwr_cClass_Log :
  case pwr_cClass_Ln :
  case pwr_cClass_Sqrt :
  case pwr_cClass_Abs :
  case pwr_cClass_Mod :
    strcpy( attr, "ActVal");
    break;   
  case pwr_cClass_maxmin :
    strcpy( attr, "MaxVal");
    break;   
  case pwr_cClass_comph :
    strcpy( attr, "High");
    break;   
  case pwr_cClass_compl :
    strcpy( attr, "Low");
    break;   
  case pwr_cClass_pid :
    strcpy( attr, "OutVal");
    break;   
  case pwr_cClass_mode :
    strcpy( attr, "OutVal");
    break;   
  default:
    return XNAV__NOTRACEATTR;
  }
  return 1;
}

//
// Convert attribute string to value
//
int XNav::attr_string_to_value( int type_id, char *value_str, 
				void *buffer_ptr, int buff_size, int attr_size)
{
  int		sts;

  switch ( type_id ) {
  case pwr_eType_Boolean: {
    if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    if ( *(pwr_tBoolean *)buffer_ptr > 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Float32: {
    if ( strcmp( value_str, "FLT_MIN") == 0)
      *(float *)buffer_ptr = FLT_MIN;
    else if ( strcmp( value_str, "FLT_NMIN") == 0)
      *(float *)buffer_ptr = -FLT_MIN;
    else if ( strcmp( value_str, "FLT_MAX") == 0)
      *(float *)buffer_ptr = FLT_MAX;
    else if ( strcmp( value_str, "FLT_NMAX") == 0)
      *(float *)buffer_ptr = -FLT_MAX;
    else if ( sscanf( value_str, "%f", (float *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Float64: {
    pwr_tFloat32 f;
    pwr_tFloat64 d;
    if ( sscanf( value_str, "%f", &f) != 1)
      return XNAV__INPUT_SYNTAX;
    d = f;
    memcpy( buffer_ptr, (char *) &d, sizeof(d));

    break;
  }
  case pwr_eType_Char: {
    if ( sscanf( value_str, "%c", (char *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Int8: {
    pwr_tInt8 	i8;
    pwr_tInt16	i16;
    if ( sscanf( value_str, "%hd", &i16) != 1)
      return XNAV__INPUT_SYNTAX;
    i8 = i16;
    memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
    break;
  }
  case pwr_eType_Int16: {
    if ( sscanf( value_str, "%hd", (short *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Int32:
  case pwr_eType_Status:
  case pwr_eType_NetStatus: {
    if ( strcmp( value_str, "INT_MIN") == 0)
      *(int *)buffer_ptr = INT_MIN;
    else if ( strcmp( value_str, "INT_MAX") == 0)
      *(int *)buffer_ptr = INT_MAX;
    else if ( sscanf( value_str, "%d", (int *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_Int64: {
    if ( sscanf( value_str, pwr_dFormatInt64, ( pwr_tInt64 *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_UInt8: {
    pwr_tUInt8 	i8;
    pwr_tUInt16	i16;
    if ( sscanf( value_str, "%hu", &i16) != 1)
      return XNAV__INPUT_SYNTAX;
    i8 = i16;
    memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
    break;
  }
  case pwr_eType_UInt16: {
    if ( sscanf( value_str, "%hu", (unsigned short *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_Enum:
  case pwr_eType_DisableAttr: {
    if ( sscanf( value_str, "%lu", (unsigned long *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_UInt64: {
    if ( sscanf( value_str, pwr_dFormatUInt64, (pwr_tUInt64 *)buffer_ptr) != 1)
      return XNAV__INPUT_SYNTAX;
    break;
  }
  case pwr_eType_String:
  case pwr_eType_Text: {
    if ( (int) strlen( value_str) >= attr_size)
      return XNAV__STRINGTOLONG;
    strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
    break;
  }
  case pwr_eType_Objid: {
    pwr_tObjid	objid;

    if ( strcmp( value_str, "0") == 0)
      objid = pwr_cNObjid;
    else {
      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    }
    memcpy( buffer_ptr, &objid, sizeof(objid));
    break;
  }
  case pwr_eType_ClassId: {
    pwr_tClassId	classid;
    pwr_tObjid	objid;

    sts = gdh_NameToObjid ( value_str, &objid);
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    classid = cdh_ClassObjidToId( objid);
    memcpy( buffer_ptr, (char *) &classid, sizeof(classid));
    break;
  }
  case pwr_eType_TypeId:
  case pwr_eType_CastId: {
    pwr_tTypeId	val_typeid;
    pwr_tObjid	objid;

    sts = gdh_NameToObjid ( value_str, &objid);
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    val_typeid = cdh_TypeObjidToId( objid);
    memcpy( buffer_ptr, (char *) &val_typeid, sizeof(val_typeid));
    break;
  }
  case pwr_eType_ObjectIx: {
    pwr_tObjectIx	objectix;

    sts = cdh_StringToObjectIx( value_str, &objectix); 
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    memcpy( buffer_ptr, (char *) &objectix, sizeof(objectix));
    break;
  }
  case pwr_eType_VolumeId: {
    pwr_tVolumeId	volumeid;

    sts = cdh_StringToVolumeId( value_str, &volumeid); 
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    memcpy( buffer_ptr, (char *) &volumeid, sizeof(volumeid));
    break;
  }
  case pwr_eType_RefId: {
    pwr_tRefId	subid;

    sts = cdh_StringToSubid( value_str, &subid);
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    memcpy( buffer_ptr, (char *) &subid, sizeof(subid));
    break;
  }
  case pwr_eType_AttrRef: {
    pwr_sAttrRef	attrref;

    if ( strcmp( value_str, "0") == 0)
      attrref = pwr_cNAttrRef;
    else {
      sts = gdh_NameToAttrref ( pwr_cNObjid, value_str, &attrref);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    }
    memcpy( buffer_ptr, &attrref, sizeof(attrref));
    break;
  }
  case pwr_eType_DataRef: {
    pwr_tDataRef	dataref;

    sts = gdh_NameToAttrref ( pwr_cNObjid, value_str, &dataref.Aref);
    if (EVEN(sts)) return XNAV__OBJNOTFOUND;
    dataref.Ptr = 0;
    memcpy( buffer_ptr, &dataref, sizeof(dataref));
    break;
  }
  case pwr_eType_Time: {
    pwr_tTime	time;

    sts = time_AsciiToA( value_str, &time);
    if (EVEN(sts)) return XNAV__INPUT_SYNTAX;
    memcpy( buffer_ptr, (char *) &time, sizeof(time));
    break;
  }
  case pwr_eType_DeltaTime: {
    pwr_tDeltaTime deltatime;

    sts = time_AsciiToD( value_str, &deltatime);
    if (EVEN(sts)) return XNAV__INPUT_SYNTAX;
    memcpy( buffer_ptr, (char *) &deltatime, sizeof(deltatime));
    break;
  }
  }
  return 1;
}

//
// Convert attribute value to string
//
void XNav::attrvalue_to_string( int type_id, pwr_tTid tid, void *value_ptr, 
				char *str, int size, int *len, char *format)
{
  pwr_tObjid		objid;
  pwr_sAttrRef		*attrref;
  int			sts;
  char			timstr[64];

  if ( value_ptr == 0) {
    strcpy( str, "UNDEFINED");
    return;
  }

  switch ( type_id ) {
  case pwr_eType_Boolean: {
    if ( !format)
      *len = snprintf( str, size, "%d", *(pwr_tBoolean *)value_ptr);
    else
      *len = snprintf( str, size, format, *(pwr_tBoolean *)value_ptr);
    break;
  }
  case pwr_eType_Float32: {
    if ( *(float *)value_ptr == FLT_MIN) {
      strcpy( str, "FLT_MIN");
      *len = strlen(str);
    }
    else if ( *(float *)value_ptr == -FLT_MIN) {
      strcpy( str, "FLT_NMIN");
      *len = strlen(str);
    }
    else if ( *(float *)value_ptr == FLT_MAX) {
      strcpy( str, "FLT_MAX");
      *len = strlen(str);
    }
    else if ( *(float *)value_ptr == -FLT_MAX) {
      strcpy( str, "FLT_NMAX");
      *len = strlen(str);
    }
    else {
      if ( !format)
	*len = snprintf( str, size, "%g", *(float *)value_ptr);
      else
	*len = snprintf( str, size, format, *(float *)value_ptr);
    }
    break;
  }
  case pwr_eType_Float64: {
    if ( !format)
      *len = snprintf( str, size, "%g", *(double *)value_ptr);
    else
      *len = snprintf( str, size, format, *(double *)value_ptr);
    break;
  }
  case pwr_eType_Char: {
    if ( !format)
      *len = snprintf( str, size, "%c", *(char *)value_ptr);
    else
      *len = snprintf( str, size, format, *(char *)value_ptr);
    break;
  }
  case pwr_eType_Int8: {
    if ( !format)
      *len = snprintf( str, size, "%d", *(char *)value_ptr);
    else
      *len = snprintf( str, size, format, *(char *)value_ptr);
    break;
  }
  case pwr_eType_Int16: {
    if ( !format)
      *len = snprintf( str, size, "%hd", *(short *)value_ptr);
    else
      *len = snprintf( str, size, format, *(short *)value_ptr);
    break;
  }
  case pwr_eType_Int32: {
    if ( *(int *)value_ptr == INT_MIN) {
      strcpy( str, "INT_MIN");
      *len = strlen(str);
    }
    else if ( *(int *)value_ptr == INT_MAX) {
      strcpy( str, "INT_MAX");
      *len = strlen(str);
    }
    else {
      if ( !format)
	*len = snprintf( str, size, "%d", *(int *)value_ptr);
      else
	*len = snprintf( str, size, format, *(int *)value_ptr);
    }
    break;
  }
  case pwr_eType_Int64: {
    if ( !format)
      *len = snprintf( str, size, pwr_dFormatInt64, *(pwr_tInt64 *)value_ptr);
    else
      *len = snprintf( str, size, format, *(pwr_tInt64 *)value_ptr);
    break;
  }
  case pwr_eType_UInt8: {
    if ( !format)
      *len = snprintf( str, size, "%u", *(unsigned char *)value_ptr);
    else
      *len = snprintf( str, size, format, *(unsigned char *)value_ptr);
    break;
  }
  case pwr_eType_UInt16: {
    if ( !format)
      *len = snprintf( str, size, "%hu", *(unsigned short *)value_ptr);
    else
      *len = snprintf( str, size, format, *(unsigned short *)value_ptr);
    break;
  }
  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_DisableAttr: {
    if ( !format)
      *len = snprintf( str, size, "%u", *(unsigned int *)value_ptr);
    else
      *len = snprintf( str, size, format, *(unsigned int *)value_ptr);
    break;
  }
  case pwr_eType_UInt64: {
    if ( !format)
      *len = snprintf( str, size, pwr_dFormatUInt64, *(pwr_tUInt64 *)value_ptr);
    else
      *len = snprintf( str, size, format, *(pwr_tUInt64 *)value_ptr);
    break;
  }
  case pwr_eType_Enum: {
    gdh_sValueDef *valuedef;
    int 		rows;
    bool		converted = false;

    sts = gdh_GetEnumValueDef( tid, &valuedef, &rows);
    if ( ODD(sts)) {

      for ( int i = 0; i < rows; i++) {
	if ( valuedef[i].Value->Value == *(pwr_tInt32 *)value_ptr) {
	  strcpy( str, valuedef[i].Value->Text);
	  *len = strlen(str);
	  converted = true;
	  break;
	}
      }
      free( (char *)valuedef);
    }
    if ( !converted) {
      if ( !format)
	*len = snprintf( str, size, "%d", *(unsigned int *)value_ptr);
      else
	*len = snprintf( str, size, format, *(unsigned int *)value_ptr);
      break;
    }
    break;
  }
  case pwr_eType_String: {
    strncpy( str, (char *)value_ptr, size);
    str[size-1] = 0;
    *len = strlen(str);
    break;
  }
  case pwr_eType_Text: {
    char *s, *t;

    for ( s = (char *)value_ptr, t = str; *s != 10 && *s != 0; s++, t++) {
      if ( t - str >= size - 1)
	break;
      *t = *s;
    }
    *t = 0;
    *len = strlen(str);
    break;
  }
  case pwr_eType_Objid: {
    pwr_tOName hiername;

    objid = *(pwr_tObjid *)value_ptr;
    if ( !objid.oix)
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
			      cdh_mName_volumeStrict);
    else
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
			      cdh_mNName);
    if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
    *len = snprintf( str, size, "%s", hiername);
    break;
  }
  case pwr_eType_AttrRef: {
    pwr_tAName hiername;

    attrref = (pwr_sAttrRef *) value_ptr;
    sts = gdh_AttrrefToName( attrref, hiername, sizeof(hiername), cdh_mNName);
    if (EVEN(sts)) {
      strcpy( str, "");
      *len = 0;
      break;
    }
    *len = snprintf( str, size, "%s", hiername);
    break;
  }
  case pwr_eType_DataRef: {
    pwr_tAName hiername;
    pwr_tDataRef *dataref;

    dataref = (pwr_tDataRef *) value_ptr;
    sts = gdh_AttrrefToName( &dataref->Aref, hiername, sizeof(hiername), cdh_mNName);
    if (EVEN(sts)) {
      strcpy( str, "");
      *len = 0;
      break;
    }
    *len = snprintf( str, size, "%s", hiername);
    break;
  }
  case pwr_eType_Time: {
    sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_DateAndTime, 
			 timstr, sizeof(timstr));
    if ( EVEN(sts))
      strcpy( timstr, "-");
    *len = snprintf( str, size, "%s", timstr);
    break;
  }
  case pwr_eType_DeltaTime: {
    sts = time_DtoAscii( (pwr_tDeltaTime *) value_ptr, 1, 
			 timstr, sizeof(timstr));
    if ( EVEN(sts))
      strcpy( timstr, "Undefined time");
    *len = snprintf( str, size, "%s", timstr);
    break;
  }
  case pwr_eType_ObjectIx: {
    *len = snprintf( str, size, "%s", cdh_ObjectIxToString( NULL, 
				   *(pwr_tObjectIx *) value_ptr, 1));
    break;
  }
  case pwr_eType_ClassId: {
    pwr_tOName hiername;

    objid = cdh_ClassIdToObjid( *(pwr_tClassId *) value_ptr);
    sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
    if (EVEN(sts)) {
      strcpy( str, "");
      *len = 0;
      break;
    }
    *len = snprintf( str, size, "%s", hiername);
    break;
  }
  case pwr_eType_TypeId:
  case pwr_eType_CastId: {
    pwr_tOName hiername;

    objid = cdh_TypeIdToObjid( *(pwr_tTypeId *) value_ptr);
    sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
    if (EVEN(sts)) {
      strcpy( str, "");
      *len = 0;
      break;
    }
    *len = snprintf( str, size, "%s", hiername);
    break;
  }
  case pwr_eType_VolumeId: {
    *len = snprintf( str, size, "%s", cdh_VolumeIdToString( NULL, 
						     *(pwr_tVolumeId *) value_ptr, 1, 0));
    break;
  }
  case pwr_eType_RefId: {
    *len = snprintf( str, size, "%s", cdh_SubidToString( NULL, 
						  *(pwr_tSubid *) value_ptr, 1));
    break;
  }
  case pwr_eType_NetStatus:
  case pwr_eType_Status: {
    msg_GetMsg( *(pwr_tStatus *)value_ptr, str, size);
    *len = strlen( str);
    break;
  }

    // XNav specials
  case xnav_eType_ShortTime: {
    sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_Time, 
			 timstr, sizeof(timstr));
    if ( EVEN(sts))
      strcpy( timstr, "Undefined time");
    *len = snprintf( str, size, "%s", timstr);
    break;
  }
  case xnav_eType_GdbNodeFlags: {
    if ( ((gdb_mNode *)value_ptr)->b.up)
      *len = sprintf( str, "Up");
    else if ( ((gdb_mNode *)value_ptr)->b.active)
      *len = sprintf( str, "Active");
    else if ( ((gdb_mNode *)value_ptr)->b.connected)
      *len = sprintf( str, "Connected");
    else
      *len = sprintf( str, "Down");
    break;
  }
  }
}

void XNav::message( char sev, const char *text)
{
  if ( message_cb)
    (message_cb)( parent_ctx, sev, text);
}

int XNav::collect_insert( pwr_sAttrRef *arp)
{
  ItemCollect 	*item;
  int		sts;
  pwr_tAName   	attr;
  char		*s;
  pwr_tAName   	obj_name;
  pwr_sAttrRef 	ar;
  pwr_tTypeId 	a_type_id;
  unsigned int 	a_size;
  unsigned int 	a_offset;
  unsigned int 	a_dim;
  pwr_tTid	a_tid;
  pwr_tAName	name;

  sts = gdh_AttrrefToName ( arp, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  if ( !arp->Flags.b.Object && !arp->Flags.b.ObjectAttr) {
    if ( (s = strchr( name, '.')) == 0)
      return 0;
    strcpy( attr, s+1);

    sts = gdh_GetAttributeCharAttrref( arp, &a_type_id, &a_size, &a_offset, 
	&a_dim);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttrRefTid( arp, &a_tid);
    if ( EVEN(sts)) return sts;
  }
  else {
    sts = get_trace_attr( arp, attr);
    if ( EVEN(sts)) return sts;
    strcpy( obj_name, name);
    strcat( obj_name, ".");
    strcat( obj_name, attr);
    sts = gdh_NameToAttrref( pwr_cNObjid, obj_name, &ar);
    if ( EVEN(sts)) return sts;

    strcpy( attr, strchr(obj_name, '.') + 1);

    sts = gdh_GetAttributeCharAttrref( &ar, &a_type_id, &a_size, &a_offset, 
	&a_dim);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttrRefTid( arp, &a_tid);
    if ( EVEN(sts)) return sts;
  }

  item = new ItemCollect( collect_brow, arp->Objid, attr, NULL, 
		flow_eDest_IntoLast, a_type_id, a_tid, a_size, 0);
  message( 'I', "Object inserted");
  return 1;
}

int XNav::collect_show()
{
  if ( brow->ctx == collect_brow->ctx) {
    // Hide
    brow_ChangeCtx( collect_brow->ctx, 
		brow_stack[brow_cnt-1]->ctx);
    *brow = *brow_stack[brow_cnt-1];
  }
  else {
    // Show
    brow_ChangeCtx( brow_stack[brow_cnt-1]->ctx, collect_brow->ctx);
    *brow = *collect_brow;
  }
  return 1;
}

int XNav::collect_remove()
{

  if ( brow->ctx == collect_brow->ctx) {
    brow_tNode	*node_list;
    int		node_count;

    brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
    if ( node_count) {
      for ( int i = 0; i < node_count; i++)
	brow_DeleteNode( collect_brow->ctx, node_list[i]);

      free( node_list);
      return XNAV__SUCCESS;
    }
    else
      return XNAV__NOSELECT;
  }
  return XNAV__NOTCOLLECTW;
}

void XNav::collect_clear()
{
  brow_DeleteAll( collect_brow->ctx);
}

void XNav::clear()
{
  brow_DeleteAll( brow->ctx);
}


//
//  Show crossreferences
//
void XNav::show_crossref()
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;

  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count) {
    message( 'E', "Select an object");
    return;
  }

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  try {
    switch( item->type) {
    case xnav_eItemType_Object:
    case xnav_eItemType_Table:
      ((ItemBaseObject *)item)->open_crossref( brow, 0, 0);
      break;
    case xnav_eItemType_AttrObject:
      item->open_crossref( brow, 0, 0);
      break;
    case xnav_eItemType_Channel:
      ((ItemChannel *)item)->open_crossref( brow, 0, 0);
      break;
    default:
      message( 'I', "Open crossreferences from popup menu");
    }
  }
  catch ( co_error& e) {
    brow_push_all();
    brow_Redraw( brow->ctx, 0);
    message('E', (char *)e.what().c_str());
  }
}

//
//  Start trace
//
void XNav::start_trace_selected()
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;

  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count) {
    message( 'E', "Select an object");
    return;
  }

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  try {
    switch( item->type) {
    case xnav_eItemType_Object: 
    case xnav_eItemType_Plc: 
    case xnav_eItemType_Crossref: 
      item->open_trace( brow, 0, 0);
      break;
    default:
      message( 'E', "Can't start trace for this object type");
    }
  }
  catch ( co_error& e) {
    brow_push_all();
    brow_Redraw( brow->ctx, 0);
    message('E', (char *)e.what().c_str());
  }
}

void XNav::start_trace( pwr_tObjid objid, char *object_str)
{
    RtTrace 	*tractx;
    pwr_tOName  name;
    char   	title[220];
    pwr_tStatus	sts;
    pwr_tClassId classid;
    pwr_tObjid	window_objid;

    sts = gdh_GetObjectClass( objid, &classid);
    if ( EVEN(sts)) return;
    if ( classid == pwr_cClass_plc) {
      // Take the first child
      sts = gdh_GetChild( objid, &window_objid);
      if ( EVEN(sts)) return;
    }
    else
      window_objid = objid;

    if ( appl.find( applist_eType_Trace, window_objid, (void **) &tractx)) {
      tractx->pop();
      if ( object_str)
        sts = tractx->search_object( object_str);
    }
    else {
      // New trace window
      sts = gdh_ObjidToName( window_objid, name, sizeof(name), cdh_mNName); 
      strcpy( title, "Trace ");
      strcat( title, name);

      tractx = plctrace_new( window_objid, &sts);
      if ( ODD(sts)) {
	tractx->help_cb = trace_help_cb;
	tractx->close_cb = trace_close_cb;
	tractx->subwindow_cb = trace_subwindow_cb;
	tractx->display_object_cb = trace_display_object_cb;
	tractx->collect_insert_cb = trace_collect_insert_cb;
	tractx->is_authorized_cb = is_authorized_cb;
        tractx->popup_menu_cb = xnav_popup_menu_cb;
        tractx->call_method_cb = xnav_call_method_cb;
        appl.insert( applist_eType_Trace, (void *)tractx, window_objid, "", NULL);

        if ( object_str)
          sts = tractx->search_object( object_str);
      }    
      else {
	delete tractx;
        message('E', "Unable to start trace for this object");
      }
    }

}

#if 0
int XNav::open_help()
{
  int sts;

  if ( !xhelp) {
    xhelp = new XHelp( parent_wid, (void *)this, &sts);
    if ( EVEN(sts)) return sts;
  }
  else {
    xhelp->clear();
    xhelp->pop();
  }
  return XNAV__SUCCESS;
}
#endif

int XNav::open_object( pwr_sAttrRef *arp)
{
  XAtt *xatt;
  int sts;
 
  if ( appl.find( applist_eType_Attr, arp, (void **) &xatt)) {
    xatt->pop();
  }
  else {
    xatt = xatt_new( arp, gbl.advanced_user, &sts);
    if ( ODD(sts))
      xatt->close_cb = xatt_close_cb;
      xatt->popup_menu_cb = xnav_popup_menu_cb;
      xatt->call_method_cb = xnav_call_method_cb;
      xatt->is_authorized_cb = is_authorized_cb;
      appl.insert( applist_eType_Attr, (void *)xatt, arp, "", NULL);
  }
  return XNAV__SUCCESS;
}

int XNav::open_crossref( pwr_sAttrRef *arp)
{
  XCrr *xcrr;
  int sts;
 
  if ( appl.find( applist_eType_Crossref, arp, (void **) &xcrr)) {
    xcrr->pop();
  }
  else {
    xcrr = xcrr_new( arp, gbl.advanced_user, &sts);
    if ( ODD(sts))
      xcrr->close_cb = xcrr_close_cb;
      xcrr->popup_menu_cb = xnav_popup_menu_cb;
      xcrr->start_trace_cb = xnav_start_trace_cb;
      appl.insert( applist_eType_Crossref, (void *)xcrr, arp, "", NULL);
  }
  return XNAV__SUCCESS;
}

//
// Create a navigator item. The class of item depends of the class
// of the object.
//
int XNav::create_object_item( pwr_tObjid objid, 
	brow_tNode dest, flow_eDest dest_code, void **item,
	int is_root)
{
  int sts;
  pwr_tClassId classid;

  sts = gdh_GetObjectClass( objid, &classid);
  if ( EVEN(sts)) return sts;

  switch( classid)
  {
    default:
      *item = (void *) new ItemObject( brow, objid, dest, dest_code, is_root);
      break;
  }
  return 1;
}

//
// Create the navigator widget
//
XNav::XNav(
	void *xn_parent_ctx,
	const char *xn_name,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	int xn_op_close_button,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx),
	brow_cnt(0), TraceList(NULL), trace_started(0),
	message_cb(NULL), close_cb(NULL), map_cb(NULL), change_value_cb(NULL),
	set_dimension_cb(NULL), selection_changed_cb(0), ccm_func_registred(0), verify(0),
	menu_tree(NULL), ev(0), op(0), clog(0), closing_down(0), opplace_p(0),
	base_priv(pwr_mPrv_System), priv(pwr_mPrv_System), displayed(0),
        current_logging_index(-1), search_last_found(0), search_compiled(0), 
	attach_audio(0), audio(0), op_close_button(xn_op_close_button), cologin(0), scctx(0)
{
  strcpy( name, xn_name);
  strcpy( opplace_name, xn_opplace_name);
  strcpy( base_user, "");
  strcpy( user, "");
  *status = 1;
}

//
//  Delete a nav context
//
XNav::~XNav()
{
}

//
//  Print
//
void XNav::print( char *filename)
{
  brow_Print( brow->ctx, filename);
}

//
//  Get zoom
//
void XNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

//
//  Zoom
//
void XNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void XNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}


//
// Set attribute value
//
int XNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*base_item;
  pwr_tAName   	attr_str;
  int		sts;
  char		buffer[400];
  
  // Check authorization
  if ( !((priv & pwr_mPrv_RtWrite) ||
         (priv & pwr_mPrv_System))) {
    message('E', "Not authorized for this operation");
    return 0;
  }
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
  case xnav_eItemType_Attr:
  case xnav_eItemType_Collect:
  case xnav_eItemType_AttrArrayElem: {
    ItemAttr	*item;

    item = (ItemAttr *)base_item;
    sts = gdh_ObjidToName( item->objid, 
			   attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    strcat( attr_str, ".");
    strcat( attr_str, item->attr);

    sts = attr_string_to_value( item->type_id, value_str, 
				buffer, sizeof(buffer), item->size);
    if ( EVEN(sts)) return sts;

    sts = gdh_SetObjectInfo( attr_str, buffer, item->size);
    if ( EVEN(sts)) {
      message(' ', get_message(sts));
      return sts;
    }
    break;
  }
  case xnav_eItemType_Local: {
    ItemLocal	*item;
    int out_of_range = 0;

    item = (ItemLocal *)base_item;

    sts = attr_string_to_value( item->type_id, value_str, 
				buffer, sizeof(buffer), item->size);
    if ( EVEN(sts)) return sts;

    if ( !(item->max_limit == 0 && item->min_limit == 0)) {
      switch( item->type_id) {
      case pwr_eType_UInt32:
	if ( *(pwr_tUInt32 *)buffer < item->min_limit ||
	     *(pwr_tUInt32 *)buffer > item->max_limit)
	  out_of_range = 1;
	break;
      case pwr_eType_Int32:
	if ( *(pwr_tInt32 *)buffer < item->min_limit ||
	     *(pwr_tInt32 *)buffer > item->max_limit)
	  out_of_range = 1;
	break;
      case pwr_eType_Float32:
	if ( *(pwr_tFloat32 *)buffer < item->min_limit ||
	     *(pwr_tFloat32 *)buffer > item->max_limit)
	  out_of_range = 1;
	break;
      case pwr_eType_Float64:
	if ( *(pwr_tFloat64 *)buffer < item->min_limit ||
	     *(pwr_tFloat64 *)buffer > item->max_limit)
	  out_of_range = 1;
	break;
      default:
	;
      }
      if ( out_of_range) {
	message( 'E', "Value is out of range");
	return 0;
      }
    }
    memcpy( item->value_p, buffer, item->size);
    break;
  }
  case xnav_eItemType_ObjectStruct: {
    ItemObjectStruct	*item;

    item = (ItemObjectStruct *)base_item;

    sts = attr_string_to_value( item->type_id, value_str,
				buffer, sizeof(buffer), item->size);
    if ( EVEN(sts)) return sts;

    memcpy( item->value_p, buffer, item->size);
    break;
  }
  case xnav_eItemType_Table:
  case xnav_eItemType_Device:
  case xnav_eItemType_Channel:
  case xnav_eItemType_RemNode:
  case xnav_eItemType_RemTrans:
  case xnav_eItemType_Plc: {
    ItemTable	*item = (ItemTable *)base_item;
    int idx = item->change_value_idx;

    sts = attr_string_to_value( item->col.elem[idx].type_id, value_str, 
				buffer, sizeof(buffer), item->col.elem[idx].size);
    if ( EVEN(sts)) return sts;

    memcpy( item->col.elem[idx].value_p, buffer, item->col.elem[idx].size);
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
int XNav::check_attr_value()
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*base_item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return XNAV__NOSELECT;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type) {
  case xnav_eItemType_Attr:
  case xnav_eItemType_AttrArrayElem:
  case xnav_eItemType_Collect:
    return 1;
  case xnav_eItemType_Local: {
    ItemLocal	*item;

    item = (ItemLocal *)base_item;

    if ( item->nochange) {
      message('E', "Attribute can't be changed");
      return XNAV__NOCHANGE;
    }
    return 1;
  }
  case xnav_eItemType_ObjectStruct: {
    ItemObjectStruct	*item;
    pwr_tBoolean is_local;
    int sts;

    item = (ItemObjectStruct *)base_item;

    if ( item->nochange) {
      message('E', "Attribute can't be changed");
      return XNAV__NOCHANGE;
    }
    sts = gdh_GetObjectLocation( item->objid, &is_local);
    if ( EVEN(sts) || !is_local)
      return XNAV__NOCHANGE;
    return 1;
  }
  default:
    return XNAV__NOCHANGE;
  }
}


//
//  Return associated class of selected object
//
int XNav::get_select( pwr_sAttrRef *attrref, int *is_attr)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  pwr_tAName   	attr_str;
  int		sts;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  sts = gdh_ObjidToName( item->objid, 
			 attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  memset( attrref, 0, sizeof(*attrref));
  switch( item->type) {
  case xnav_eItemType_Attr:
  case xnav_eItemType_AttrArray:
  case xnav_eItemType_AttrObject:
  case xnav_eItemType_AttrArrayElem:
    strcat( attr_str, ".");
    strcat( attr_str, item->name);
    sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, attrref);
    if ( EVEN(sts)) return sts;
    *is_attr = 1;
    break;
  case xnav_eItemType_Collect:
    sts = gdh_NameToAttrref( pwr_cNObjid, item->name, attrref);
    if ( EVEN(sts)) return sts;
    *is_attr = 1;
    break;
  default:
    sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, attrref);
    *is_attr = 0;
    if ( EVEN(sts)) return sts;
  }
  return 1;
}

int XNav::get_select_all( pwr_sAttrRef **attrref, int **is_attr)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  pwr_tAName   	attr_str;
  int		sts;
  pwr_sAttrRef  *ap;
  int           *ip;
  int           i;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  ap = (pwr_sAttrRef *) calloc( node_count + 1, sizeof(pwr_sAttrRef));
  ip = (int *) calloc( node_count, sizeof(int));

  *attrref = ap;
  *is_attr = ip;
  for ( i = 0; i < node_count; i++) {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
			   attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type) {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArray:
    case xnav_eItemType_AttrObject:
    case xnav_eItemType_AttrArrayElem:
      strcat( attr_str, ".");
      strcat( attr_str, item->name);
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    case xnav_eItemType_Collect:
      sts = gdh_NameToAttrref( pwr_cNObjid, item->name, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    default:
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      *ip = 0;
      if ( EVEN(sts)) return sts;
    }
    ap++;
    ip++;
  }
  free( node_list);
  return 1;
}

int XNav::get_all_objects( pwr_sAttrRef **attrref, int **is_attr)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  pwr_tAName   	attr_str;
  int		sts;
  pwr_sAttrRef  *ap;
  int           *ip;
  int           i;
  
  brow_GetObjectList( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  ap = (pwr_sAttrRef *) calloc( node_count + 1, sizeof(pwr_sAttrRef));
  ip = (int *) calloc( node_count, sizeof(int));

  *attrref = ap;
  *is_attr = ip;
  for ( i = 0; i < node_count; i++) {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
			   attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type) {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArray:
    case xnav_eItemType_AttrObject:
    case xnav_eItemType_AttrArrayElem:
      strcat( attr_str, ".");
      strcat( attr_str, item->name);
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    case xnav_eItemType_Collect:
      sts = gdh_NameToAttrref( pwr_cNObjid, item->name, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    default:
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      *ip = 0;
      if ( EVEN(sts)) return sts;
    }
    ap++;
    ip++;
  }
  return 1;
}

int XNav::get_all_collect_objects( pwr_sAttrRef **attrref, int **is_attr)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  pwr_tAName   	attr_str;
  int		sts;
  pwr_sAttrRef  *ap;
  int           *ip;
  int           i;
  
  brow_GetObjectList( collect_brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  ap = (pwr_sAttrRef *) calloc( node_count + 1, sizeof(pwr_sAttrRef));
  ip = (int *) calloc( node_count, sizeof(int));

  *attrref = ap;
  *is_attr = ip;
  for ( i = 0; i < node_count; i++) {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
			   attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type) {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArray:
    case xnav_eItemType_AttrObject:
    case xnav_eItemType_AttrArrayElem:
      strcat( attr_str, ".");
      strcat( attr_str, item->name);
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    case xnav_eItemType_Collect:
      sts = gdh_NameToAttrref( pwr_cNObjid, item->name, ap);
      if ( EVEN(sts)) return sts;
      *ip = 1;
      break;
    default:
      sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, ap);
      *ip = 0;
      if ( EVEN(sts)) return sts;
    }
    ap++;
    ip++;
  }
  return 1;
}

int XNav::is_authorized( unsigned int access, int msg)
{
  if (!(priv & access)) {
    if ( msg)
      message( 'I', "Not authorized for this operation");
    return 0;
  }
  return 1;
}



#if 0 // Currently not used, avoid compiler warnings
//
// Convert type_id to name
//
static void  xnav_type_id_to_name( int type_id, char *type_id_name)
{
  switch( type_id)
  {
    case pwr_eType_Boolean: strcpy( type_id_name, "Boolean"); break;
    case pwr_eType_Float32: strcpy( type_id_name, "Float32"); break;
    case pwr_eType_Float64: strcpy( type_id_name, "Float64"); break;
    case pwr_eType_Char: strcpy( type_id_name, "Char"); break;
    case pwr_eType_Int8: strcpy( type_id_name, "Int8"); break;
    case pwr_eType_Int16: strcpy( type_id_name, "Int16"); break;
    case pwr_eType_Int32: strcpy( type_id_name, "Int32"); break;
    case pwr_eType_Int64: strcpy( type_id_name, "Int64"); break;
    case pwr_eType_UInt8: strcpy( type_id_name, "UInt8"); break;
    case pwr_eType_UInt16: strcpy( type_id_name, "UInt16"); break;
    case pwr_eType_UInt32: strcpy( type_id_name, "UInt32"); break;
    case pwr_eType_UInt64: strcpy( type_id_name, "UInt64"); break;
    case pwr_eType_Objid: strcpy( type_id_name, "Objid"); break;
    case pwr_eType_Buffer: strcpy( type_id_name, "Buffer"); break;
    case pwr_eType_String: strcpy( type_id_name, "String"); break;
    case pwr_eType_Enum: strcpy( type_id_name, "Enum"); break;
    case pwr_eType_Struct: strcpy( type_id_name, "Struct"); break;
    case pwr_eType_Mask: strcpy( type_id_name, "Mask"); break;
    case pwr_eType_Array: strcpy( type_id_name, "Array"); break;
    case pwr_eType_Time: strcpy( type_id_name, "Time"); break;
    case pwr_eType_Text: strcpy( type_id_name, "Text"); break;
    case pwr_eType_AttrRef: strcpy( type_id_name, "AttrRef"); break;
    case pwr_eType_UInt64: strcpy( type_id_name, "UInt64"); break;
    case pwr_eType_Int64: strcpy( type_id_name, "Int64"); break;
    case pwr_eType_ClassId: strcpy( type_id_name, "ClassId"); break;
    case pwr_eType_TypeId: strcpy( type_id_name, "TypeId"); break;
    case pwr_eType_VolumeId: strcpy( type_id_name, "VolumeId"); break;
    case pwr_eType_ObjectIx: strcpy( type_id_name, "ObjectIx"); break;
    case pwr_eType_RefId: strcpy( type_id_name, "RefId"); break;
    case pwr_eType_DeltaTime: strcpy( type_id_name, "DeltaTime"); break;
    case pwr_eType_Status: strcpy( type_id_name, "Status"); break;
    case pwr_eType_NetStatus: strcpy( type_id_name, "NetStatus"); break;
    default: strcpy( type_id_name, "");
  }
}
#endif

void XNav::trace_subwindow_cb( void *ctx, pwr_tObjid objid)
{
  XNav *xnav = (XNav *) ctx;

  xnav->start_trace( objid, NULL);
}

void XNav::trace_display_object_cb( void *ctx, pwr_tObjid objid)
{
  XNav *xnav = (XNav *) ctx;
  pwr_sAttrRef aref = cdh_ObjidToAref( objid);

  xnav->display_object( &aref, 0);
  xnav->pop();
}

int XNav::is_authorized_cb( void *xnav, unsigned int access)
{
  return (((XNav *)xnav)->priv & access) != 0;
}

void XNav::trace_collect_insert_cb( void *ctx, pwr_tObjid objid)
{
  XNav 		*xnav = (XNav *) ctx;
  pwr_sAttrRef 	attrref;
  pwr_tAName   	attr_str;
  int		sts;

  sts = gdh_ObjidToName( objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return;

  sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
  if ( EVEN(sts)) return;

  xnav->collect_insert( &attrref);
}

void XNav::trace_close_cb( RtTrace *tractx)
{
  ((XNav *)tractx->parent_ctx)->appl.remove( (void *)tractx);
  delete tractx;
}

void XNav::trace_help_cb( RtTrace *tractx, const char *key)
{
  XNav *xnav = (XNav *) tractx->parent_ctx;
  int sts;

  xnav->brow_push_all();
  pwr_tObjid objid;
  char objid_str[40];

  sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Project, NULL, 0);
  if ( EVEN(sts)) {
    // Try to convert to objid and search for objid as topic
    sts = gdh_NameToObjid ( key, &objid);
    if ( ODD(sts)) {
      cdh_ObjidToString( objid_str, objid, 1);
      sts = CoXHelp::dhelp( objid_str, "", navh_eHelpFile_Project, NULL, 0);
    }
  }
  if ( EVEN(sts))
    sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Base, NULL, 0);
  if ( EVEN(sts))
    xnav->message( 'E', "Unable to find topic");
  else
    xnav->message( ' ', null_str);
  // xnav->pop();
}

void XNav::xatt_close_cb( void *xnav, void *xatt)
{
  ((XNav *)xnav)->appl.remove( xatt);
  delete (XAtt *)xatt;
}

void XNav::xcrr_close_cb( void *xnav, void *xcrr)
{
  ((XNav *)xnav)->appl.remove( xcrr);
  delete (XCrr *)xcrr;
}

//
// Callbacks from brow
//
int XNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  XNav		*xnav;
  Item 		*item;

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &xnav);
  if ( xnav->closing_down)
    return 1;

  if ( !xnav->is_authorized())
    return 1;

  xnav->message( ' ', null_str);
  try {
    switch ( event->event) {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( xnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( xnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( xnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( xnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
	    if ( node_count)
	      free( node_list);
	    return 1;
	  }
        }
      }
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xnav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      if ( xnav->selection_changed_cb)
	(xnav->selection_changed_cb)( xnav->parent_ctx);
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( xnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( xnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( xnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( xnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
	    if ( node_count)
	      free( node_list);
	    return 1;
	  }
        }
      }
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      if ( xnav->selection_changed_cb)
	(xnav->selection_changed_cb)( xnav->parent_ctx);
      break;
    }
    case flow_eEvent_Key_PageDown: {
      brow_Page( xnav->brow->ctx, 0.8);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( xnav->brow->ctx, -0.8);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( xnav->brow->ctx, 0.1);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( xnav->brow->ctx, -0.1);
      break;
    }
    case flow_eEvent_Key_PF1:
    case flow_eEvent_Key_ShiftRight:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      item->open_attributes( xnav->brow, 0, 0);
      break;
    }
    case flow_eEvent_Key_PF2:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      item->open_crossref( xnav->brow, 0, 0);
      break;
    }
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;
      int		sts;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type) {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArrayElem:
      case xnav_eItemType_Collect:
      case xnav_eItemType_ObjectStruct:
	sts = item->open_children( xnav->brow, 0, 0);
	if (ODD(sts)) break;

	// if even sts continue
      case xnav_eItemType_Local:
	if ( xnav->gbl.advanced_user && xnav->change_value_cb)
	  (xnav->change_value_cb)( xnav->parent_ctx);
	break;
      case xnav_eItemType_Enum:
	if ( xnav->gbl.advanced_user) {
	  if ( !xnav->is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System)) {
	    xnav->message('E', "No write access");
	    break;
	  }
	  ((ItemEnum *)item)->set_value();
	}
	break;
      case xnav_eItemType_Mask:
	if ( xnav->gbl.advanced_user) {
	  if ( !xnav->is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System)) {
	    xnav->message('E', "No write access");
	    break;
	  }
	  ((ItemMask *)item)->toggle_value();
	}
	break;
      default:
	sts = item->open_children( xnav->brow, 0, 0);
	if ( sts == XNAV__NOCHILD && xnav->gbl.advanced_user)
	  item->open_attributes( xnav->brow, 0, 0);
      }
      break;
    }
    case flow_eEvent_Key_PF3:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type)
      {
        case xnav_eItemType_Attr:
        case xnav_eItemType_AttrArrayElem:
        case xnav_eItemType_Collect:
        case xnav_eItemType_Local:
        case xnav_eItemType_ObjectStruct:
          if ( xnav->change_value_cb)
            (xnav->change_value_cb)( xnav->parent_ctx);
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Key_PF4:
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
	xnav->brow_push();
        return 1;
      }

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( xnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
	  xnav->brow_push();
	  if ( xnav->selection_changed_cb)
	    (xnav->selection_changed_cb)( xnav->parent_ctx);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      item->close( xnav->brow, 0, 0);
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xnav->brow->ctx, object, 0.25);
      free( node_list);
      if ( xnav->selection_changed_cb)
	(xnav->selection_changed_cb)( xnav->parent_ctx);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( xnav->brow->ctx);
      break;
    case flow_eEvent_ObjectDeleted:
      brow_GetUserData( event->object.object, (void **)&item);
      delete item;
      break;
    case flow_eEvent_MB3Down:
    {
      brow_SetClickSensitivity( xnav->brow->ctx, 
				      flow_mSensitivity_MB3Press);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type) {
	  case xnav_eItemType_HelpHeader: 
	    item->close( xnav->brow, event->object.x, event->object.y);
	  default:
	    item->open_children( xnav->brow, event->object.x, event->object.y);
          }
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1ClickShift:
    {
      // Add elect
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
	  if ( event->object.x < ll_x + 1.0)
          {
            // Simulate doubleclick
            flow_tEvent doubleclick_event;

            doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
            memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
            doubleclick_event->event = flow_eEvent_MB1DoubleClickShift;
            sts = brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_MB1DoubleClickShift:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
	  item->open_attributes( xnav->brow, event->object.x, event->object.y);
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1DoubleClickShiftCtrl:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
	  item->open_trace( xnav->brow, event->object.x, event->object.y);
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1Click:
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
	  if ( event->object.x < ll_x + 1.0)
          {
            // Simulate doubleclick
            flow_tEvent doubleclick_event;

            doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
            memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
            doubleclick_event->event = flow_eEvent_MB1DoubleClick;
            sts = brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( xnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( xnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( xnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( xnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( xnav->brow->ctx);
      }
      if ( xnav->selection_changed_cb)
	(xnav->selection_changed_cb)( xnav->parent_ctx);
      break;
    case flow_eEvent_Radiobutton:
    {
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type) {
	  case xnav_eItemType_Enum: 
	    if ( !xnav->is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System)) {
	      xnav->message('E', "No write access");
	      break;
	    }
	    if ( !event->radiobutton.value)
	      ((ItemEnum *)item)->set_value();
	    break;
	  case xnav_eItemType_Mask: 
	    if ( !xnav->is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System)) {
	      xnav->message('E', "No write access");
	      break;
	    }
	    ((ItemMask *)item)->set_value( !event->radiobutton.value);
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
    case flow_eEvent_MB3Press:
    {            
      // Popup menu
      pwr_sAttrRef attrref;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          if ( cdh_ObjidIsNull( item->objid))
            break;

          switch ( item->type) {
            case xnav_eItemType_Attr:
            case xnav_eItemType_AttrArrayElem:
            {
              pwr_tAName attr_str;

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              memset( &attrref, 0, sizeof(attrref));
              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
              if ( EVEN(sts)) return sts;

              xnav->create_popup_menu( attrref, 
				 xmenu_eItemType_Attribute,
				 xmenu_mUtility_XNav, xnav->priv, NULL,
				 event->any.x_pixel, event->any.y_pixel);
              break;
            }
            case xnav_eItemType_AttrObject:
            {
              pwr_tAName attr_str;

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              memset( &attrref, 0, sizeof(attrref));
              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
              if ( EVEN(sts)) return sts;

              xnav->create_popup_menu( attrref, 
				 xmenu_eItemType_AttrObject,
				 xmenu_mUtility_XNav, xnav->priv, NULL,
				 event->any.x_pixel, event->any.y_pixel);
              break;
            }
            case xnav_eItemType_Collect:
            {
              pwr_tAName attr_str;

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              memset( &attrref, 0, sizeof(attrref));
              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, item->name, &attrref);
              if ( EVEN(sts)) return sts;

              xnav->create_popup_menu( attrref, 
				 xmenu_eItemType_Attribute,
				 xmenu_mUtility_XNav, xnav->priv, NULL,
				 event->any.x_pixel, event->any.y_pixel);
              break;
            }
            case xnav_eItemType_Crossref:
            {
              ItemCrossref *itemc = (ItemCrossref *)item;
              attrref = cdh_ObjidToAref( itemc->objid);

              xnav->create_popup_menu( attrref, 
				 xmenu_eItemType_Crossref,
				 xmenu_mUtility_XNav, xnav->priv, itemc->ref_name,
				 event->any.x_pixel, event->any.y_pixel);
              break;
            }
	    default:
              attrref = cdh_ObjidToAref( item->objid);
              xnav->create_popup_menu( attrref, 
				 xmenu_eItemType_Object,
				 xmenu_mUtility_XNav, xnav->priv, NULL,
				 event->any.x_pixel, event->any.y_pixel);

          }
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Resized:
      brow_Redraw( xnav->brow->ctx, 0);
      break;
    default:
      ;
    }
  }
  catch ( co_error& e) {
    xnav->brow_push_all();
    brow_Redraw( xnav->brow->ctx, 0);
    xnav->message('E', (char *)e.what().c_str());
  }
  return 1;
}

void XNav::trace_scan( void *data)
{
  XNav *xnav = (XNav *)data;
  int time = int( xnav->gbl.scantime * 1000);

  if ( xnav->trace_started)
  {
    flow_TraceScan( xnav->brow->ctx);

    xnav->trace_timerid->add( time, trace_scan, xnav);
  }
  xnav->update_alarminfo();

  if ( xnav->ev)
    xnav->ev->update( xnav->gbl.scantime);
  if ( xnav->op)
    xnav->op->scan();
}

void XNav::force_trace_scan()
{
  if ( trace_started)
    flow_TraceScan( brow->ctx);
}

int XNav::trace_scan_bc( brow_tObject object, void *p)
{
  Item		*base_item;
  char		buf[400];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Collect:
    {
      ItemAttr	*item;

      item = (ItemAttr *)base_item;
      if ( !item->first_scan)
      {
        if ( item->size > (int) sizeof(item->old_value) && 
	     item->type_id == pwr_eType_String &&
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

      attrvalue_to_string( item->type_id, item->tid, p, buf, sizeof(buf), &len, NULL);
      brow_SetAnnotation( object, 1, buf, len);
      memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
      break;
    }
    case xnav_eItemType_Enum: {
      ItemEnum	*item;

      item = (ItemEnum *)base_item;
      if ( !item->first_scan) {
        if ( memcmp( item->old_value, p, sizeof(pwr_tEnum)) == 0)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *(pwr_tEnum *)p == item->num)
	brow_SetRadiobutton( object, 0, 1);
      else
	brow_SetRadiobutton( object, 0, 0);
      memcpy( item->old_value, p, sizeof(pwr_tEnum));
      break;
    }
    case xnav_eItemType_Mask: {
      ItemMask	*item;

      item = (ItemMask *)base_item;
      if ( !item->first_scan) {
        if ( memcmp( item->old_value, p, sizeof(pwr_tMask)) == 0)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *(pwr_tMask *)p & item->num)
	brow_SetRadiobutton( object, 0, 1);
      else
	brow_SetRadiobutton( object, 0, 0);
      memcpy( item->old_value, p, sizeof(pwr_tMask));
      break;
    }
    case xnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *)base_item;
      if ( !item->first_scan)
      {
        if ( item->size > (int) sizeof(item->old_value) && 
	     item->type_id == pwr_eType_String &&
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

      attrvalue_to_string( item->type_id, 0, p, buf, sizeof(buf), &len, NULL);
      brow_SetAnnotation( object, 1, buf, len);
      memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
      break;
    }
    case xnav_eItemType_ObjectStruct:
    {
      ItemObjectStruct	*item;

      item = (ItemObjectStruct *)base_item;
      if ( !item->first_scan)
      {
        if ( item->size > (int) sizeof(item->old_value) && 
	     item->type_id == pwr_eType_String &&
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

      attrvalue_to_string( item->type_id, 0, p, buf, sizeof(buf), &len, NULL);
      brow_SetAnnotation( object, 1, buf, len);
      memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
      break;
    }
    case xnav_eItemType_TableHeader:
    {
      ItemTableHeader	*item = (ItemTableHeader *)base_item;

      if ( item->scan)
        (item->scan)(item->xnav);
      break;
    }
    case xnav_eItemType_Table:
    case xnav_eItemType_Device:
    case xnav_eItemType_Channel:
    case xnav_eItemType_RemNode:
    case xnav_eItemType_RemTrans:
    case xnav_eItemType_Plc:
    {
      int nochange;
      int i;
      ItemTable *item = (ItemTable *)base_item;

      if ( item->scan)
        (item->scan)(item->xnav);

      for ( i = 0; i < item->col.elem_cnt; i++)
      {
        if ( item->col.elem[i].type_id == xnav_eType_Empty)
          continue;
        else if ( item->col.elem[i].type_id == xnav_eType_FixStr)
        {
          // Fix string
          if ( item->first_scan)
            brow_SetAnnotation( object, i, item->col.elem[i].fix_str, 
		strlen(item->col.elem[i].fix_str));
        }
	else
	{
          if ( ! item->col.elem[i].value_p)
            continue;

          if ( !item->first_scan)
          {
            nochange = 0;
            if ( item->col.elem[i].size > (int) sizeof(item->old_value[i]) && 
	       item->col.elem[i].type_id == pwr_eType_String &&
	       strlen((char *)item->col.elem[i].value_p) < 
		sizeof(item->old_value[i]) && 
	       strcmp( (char *)item->col.elem[i].value_p, item->old_value[i]) 
		== 0)
              // No change since last time
              nochange = 1;
            else if ( memcmp( item->old_value[i], item->col.elem[i].value_p, 
		item->col.elem[i].size) == 0)
              // No change since last time
              nochange = 1;
          }
          else
            nochange = 0;

          if ( !nochange)
          {
            attrvalue_to_string( item->col.elem[i].type_id, 0,
		item->col.elem[i].value_p, buf, sizeof(buf), &len,
		item->col.elem[i].format);
            brow_SetAnnotation( object, i, buf, len);
            memcpy( item->old_value[i], item->col.elem[i].value_p,
		min(item->col.elem[i].size, (int) sizeof(item->old_value[i])));
          }
        }
      }
      if ( item->first_scan)
        item->first_scan = 0;
      break;
    }
    default:
      ;
  }
  return 1;
}

int XNav::trace_connect_bc( brow_tObject object, char *name, char *attr, 
			    flow_eTraceType type, void **p)
{
  pwr_tAName   	attr_str;
  int		sts;
  Item 		*base_item;

  /*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_Enum:
    case xnav_eItemType_Mask:
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Collect:
    {
      ItemAttr	*item;

      item = (ItemAttr *) base_item;
      strcpy( attr_str, name);
      strcat( attr_str, ".");
      strcat( attr_str, attr);
      sts = gdh_RefObjectInfo( attr_str, p, &item->subid, item->size);
      if ( EVEN(sts)) return sts;
      break;
    }
    case xnav_eItemType_Local:
    {
      *p = ((ItemLocal *)base_item)->value_p;
      break;
    }
    case xnav_eItemType_ObjectStruct:
    {
      *p = ((ItemObjectStruct *)base_item)->value_p;
      break;
    }
    case xnav_eItemType_Table:
    case xnav_eItemType_TableHeader:
    case xnav_eItemType_Device:
    case xnav_eItemType_Channel:
    case xnav_eItemType_RemNode:
    case xnav_eItemType_RemTrans:
    case xnav_eItemType_Plc:
    {
      ItemTable	*item = (ItemTable *) base_item;

      *p = (void *) item;
      break;
    }
    default:
      ;
  }      
  return 1;
}

int XNav::trace_disconnect_bc( brow_tObject object)
{
  Item 		*base_item;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Collect:
    {
      ItemAttr	*item = (ItemAttr *) base_item;

      gdh_UnrefObjectInfo( item->subid);
      break;
    }
    case xnav_eItemType_ObjectStruct:
    {
      ItemObjectStruct	*item = (ItemObjectStruct *) base_item;

      if ( cdh_RefIdIsNotNull( item->subid))
        gdh_UnrefObjectInfo( item->subid);
      break;
    }
    case xnav_eItemType_TableHeader:
    {
      ItemTableHeader	*item = (ItemTableHeader *) base_item;

      if ( item->disconnect)
        (item->disconnect)(item->xnav);

      break;
    }
    case xnav_eItemType_Table:
    case xnav_eItemType_Device:
    case xnav_eItemType_Channel:
    case xnav_eItemType_RemNode:
    case xnav_eItemType_RemTrans:
    case xnav_eItemType_Plc:
    {
      ItemTable	*item = (ItemTable *) base_item;

      if ( item->disconnect)
        (item->disconnect)(item->xnav);

      for ( int i = 0; i < item->subid.subid_cnt; i++)
        gdh_UnrefObjectInfo( item->subid.subid[i]);
      break;
    }
    default:
      ;
  }
  return 1;
}

int XNav::update_alarminfo()
{
  flow_tObject 	*object_list;
  int		object_cnt;
  Item	 	*item;
  int		i;
  pwr_tStatus	sts;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++) {
    brow_GetUserData( object_list[i], (void **)&item);

    switch ( item->type) {
    case xnav_eItemType_Object: {
      pwr_tUInt32 alarm_level, max_alarm_level;
      pwr_tUInt32 block_level, max_block_level, visibility;
      ItemObject *oitem = (ItemObject *)item;

      sts = gdh_GetAlarmInfo( oitem->objid, &alarm_level, &max_alarm_level, 
			      &block_level, &max_block_level, &visibility);
      if ( EVEN(sts)) break;

      // Update alarm pixmap
      if ( alarm_level != oitem->alarm_level ||
	   max_alarm_level != oitem->max_alarm_level) {
	if ( alarm_level && max_alarm_level)
	  brow_SetAnnotPixmap( object_list[i], 2, brow->pixmap_alarm2);
	else if ( alarm_level)
	  brow_SetAnnotPixmap( object_list[i], 2, brow->pixmap_arrowright);
	else if ( max_alarm_level)
	  brow_SetAnnotPixmap( object_list[i], 2, brow->pixmap_arrowdown);
	else
	  brow_RemoveAnnotPixmap( object_list[i], 2);
	oitem->alarm_level = alarm_level;
	oitem->max_alarm_level = max_alarm_level;
      }

      // Update block pixmap
      if ( block_level != oitem->block_level ||
	   max_block_level != oitem->max_block_level) {
	if ( block_level && max_block_level)
	  brow_SetAnnotPixmap( object_list[i], 3, brow->pixmap_block2);
	else if ( block_level)
	  brow_SetAnnotPixmap( object_list[i], 3, brow->pixmap_blockr);
	else if ( max_block_level)
	  brow_SetAnnotPixmap( object_list[i], 3, brow->pixmap_blockd);
	else
	  brow_RemoveAnnotPixmap( object_list[i], 3);
	oitem->block_level = block_level;
	oitem->max_block_level = max_block_level;
      }
      break;
    }
    default: ;
    }
  }
  return 1;
}

int XNav::display_object( pwr_tObjid objid, int open)
{
  pwr_sAttrRef aref = cdh_ObjidToAref( objid);
  return display_object( &aref, open);
}

int XNav::display_object( pwr_sAttrRef *arp, int open)
{
#define PARENTLIST_SIZE 40
#define MOUNTLIST_SIZE 40
  pwr_tObjid    parent_list[PARENTLIST_SIZE];
  pwr_tObjid    mountobject_list[MOUNTLIST_SIZE];
  pwr_tObjid    mounted_list[MOUNTLIST_SIZE];
  int           parent_cnt = 0;
  int           mount_cnt = 0;
  int           i;
  ItemObject    *item;
  int           sts;
  pwr_sMountObject *mount_p;
  int           is_mountobject = 0;

  // Get all mount-objects
  sts = gdh_GetClassList( pwr_eClass_MountObject, &mountobject_list[mount_cnt]);
  while( ODD(sts)) {
    if ( mount_cnt == MOUNTLIST_SIZE)
      break;

#if 0   
    pwr_tOName n;
    sts = gdh_ObjidToName ( objid, n, sizeof(n), cdh_mNName);
    if ( ODD(sts))
      sts = gdh_NameToObjid( n, &mounted_list[mount_cnt]);
    if ( ODD(sts))
      mount_cnt++;
#endif

      sts = gdh_MountObjidToPointer( mountobject_list[mount_cnt], (void **)&mount_p);
      if ( ODD(sts)) {
	mounted_list[mount_cnt] = mount_p->Object;
	mount_cnt++;
      }

    sts = gdh_GetNextObject( mountobject_list[mount_cnt-1],
                &mountobject_list[mount_cnt]);
  }

  sts = gdh_GetParent( arp->Objid, &parent_list[parent_cnt]);
  while( ODD(sts)) {
    for ( i = 0; i < mount_cnt; i++) {
      if ( cdh_ObjidIsEqual( parent_list[parent_cnt], mounted_list[i])) {
        // Replace real parent with mountobject
        parent_list[parent_cnt] = mountobject_list[i];
        is_mountobject = 1;
        break;
      }
    }
    if ( parent_cnt == PARENTLIST_SIZE)
      return 0;
    parent_cnt++;
    if ( is_mountobject) {
      sts = gdh_GetLocalParent( parent_list[parent_cnt-1], &parent_list[parent_cnt]);
      is_mountobject = 0;
    }
    else
      sts = gdh_GetParent( parent_list[parent_cnt-1], &parent_list[parent_cnt]);
  }
  
  brow_push_all();
  show_database();
  brow_SetNodraw( brow->ctx);

  try {
    for ( i = parent_cnt; i > 0; i--) {
      sts = find( parent_list[i - 1], (void **) &item);
      if ( EVEN(sts)) return sts;
      item->open_children( brow, 0, 0);
    }
    sts = find( arp->Objid, (void **) &item);
    if ( EVEN(sts)) return sts;

    if ( arp->Flags.b.Object) {
      brow_SetInverse( item->node, 1);
      brow_SelectInsert( brow->ctx, item->node);
      if ( open) {
	item->open_children( brow, 0, 0);
      }
      brow_ResetNodraw( brow->ctx);
      brow_Redraw( brow->ctx, 0);

      if ( open)
	brow_CenterObject( brow->ctx, item->node, 0.00);
      else
	brow_CenterObject( brow->ctx, item->node, 0.80);
    }
    else {
      pwr_tAName name;
      cdh_sParseName parsename;
      cdh_sParseName *pn;
      Item *aitem;
      char idx[20];

      sts = gdh_AttrrefToName( arp, name, sizeof(name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) goto display_error;

      pn = cdh_ParseName( &sts, &parsename, pwr_cNObjid, name, 0);
      if ( pn->nAttribute == 0) {
	brow_SetInverse( item->node, 1);
	brow_SelectInsert( brow->ctx, item->node);
	brow_ResetNodraw( brow->ctx);
	brow_Redraw( brow->ctx, 0);
	brow_CenterObject( brow->ctx, item->node, 0.80);
	return 1;
      }

      item->open_attributes( brow, 0, 0);

      strcpy( name, pn->attribute[0].name.orig);
      for ( i = 0; i < (int) pn->nAttribute; i++) {
	sts = find( arp->Objid, name, (void **) &aitem);
	if ( EVEN(sts)) goto display_error;

	switch( aitem->type) {
	case xnav_eItemType_AttrArray:
	  if ( pn->hasIndex[i]){
	    aitem->open_attributes( brow, 0, 0);
	    sprintf( idx, "[%d]", pn->index[i]);
	    strcat( name, idx);

	    sts = find( arp->Objid, name, (void **) &aitem);
	    if ( EVEN(sts)) goto display_error;

	  }
	  break;
	default:
	  ;
	}
	if ( i != (int) pn->nAttribute - 1) {
	  aitem->open_attributes( brow, 0, 0);
	  strcat( name, ".");
	  strcat( name, pn->attribute[i+1].name.orig);
	}
      }
      brow_SetInverse( aitem->node, 1);
      brow_SelectInsert( brow->ctx, aitem->node);

      brow_ResetNodraw( brow->ctx);
      brow_Redraw( brow->ctx, 0);

      brow_CenterObject( brow->ctx, aitem->node, 0.80);
    }

  }
  catch ( co_error& e) {
    brow_push_all();
    brow_Redraw( brow->ctx, 0);
    message('E', (char *)e.what().c_str());
  }
  return 1;

 display_error:
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return sts;
}

int XNav::find( pwr_tObjid objid, void **item)
{
  flow_tObject 	*object_list;
  int		object_cnt;
  Item	 	*object_item;
  int		i;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++) {
    brow_GetUserData( object_list[i], (void **)&object_item);
    if ( cdh_ObjidIsEqual( object_item->objid, objid)) {
      *item = (void *) object_item;
      return 1;
    }
  }
  return 0;
}

int XNav::find( pwr_tObjid objid, char *attr, void **item)
{
  flow_tObject 	*object_list;
  int		object_cnt;
  Item	 	*object_item;
  int		i;
  pwr_tOName	item_attr;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++) {
    brow_GetUserData( object_list[i], (void **)&object_item);
    cdh_SuppressSuperAll( item_attr, object_item->name);
    if ( cdh_ObjidIsEqual( object_item->objid, objid) && 
	 strcmp( attr, item_attr) == 0) {
      *item = (void *) object_item;
      return 1;
    }
  }
  return 0;
}

int	XNav::setup()
{
  brow_pop();
  brow_SetNodraw( brow->ctx);
  new ItemHeader( brow, "Title", "Setup",  NULL, flow_eDest_IntoLast);

  new ItemLocal( brow, "OpPlace", "setup_opPlace", 
        pwr_eType_String, sizeof( gbl.OpPlace), 0, 0, 1,
	(void *) gbl.OpPlace, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "DefaultDirectory", "setup_defaultdirectory", 
	pwr_eType_String, sizeof( gbl.default_directory), 0, 0, 0,
	(void *) gbl.default_directory, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "Scantime", "setup_scantime", 
	pwr_eType_Float64, sizeof( gbl.scantime), 0.010, 10, 0,
	(void *) &gbl.scantime, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmBeep", "setup_alarmbeep",
	pwr_eType_Boolean, sizeof( gbl.AlarmBeep), 0, 1, 0,
	(void *) &gbl.AlarmBeep, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmReturn", "setup_alarmreturn", 
	pwr_eType_Boolean, sizeof( gbl.AlarmReturn), 0, 1, 0,
	(void *) &gbl.AlarmReturn, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmAck", "setup_alarmack", 
	pwr_eType_Boolean, sizeof( gbl.AlarmAck), 0, 1, 0,
	(void *) &gbl.AlarmAck, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "HideOperatorWindow", "setup_hideopwind", 
	pwr_eType_Boolean, sizeof( gbl.hide_opwind), 0, 1, 0,
	(void *) &gbl.hide_opwind, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "HideStatusBar", "setup_hidestatusbar", 
	pwr_eType_Boolean, sizeof( gbl.hide_statusbar), 0, 1, 0,
	(void *) &gbl.hide_statusbar, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "SetupScript", "setup_setupscript", 
	pwr_eType_String, sizeof(gbl.setupscript), 0, 0, 0,
	(void *) gbl.setupscript, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "Verify", "setup_verify", 
	pwr_eType_Int32, sizeof( gbl.verify), 0, 1, 0,
	(void *) &gbl.verify, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AdvancedUser", "setup_advanceduser", 
	pwr_eType_Int32, sizeof( gbl.advanced_user), 0, 1, 0,
	(void *) &gbl.advanced_user, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "ShowTrueDb", "setup_truedb", 
	pwr_eType_Int32, sizeof( gbl.show_truedb), 0, 1, 0,
	(void *) &gbl.show_truedb, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "ShowAllAttributes", "setup_allattr", 
	pwr_eType_Int32, sizeof( gbl.show_truedb), 0, 1, 0,
	(void *) &gbl.show_allattr, NULL, flow_eDest_IntoLast);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int	XNav::show_logging( int index)
{
  char title[80];
  char text[80];
  char command[200];
  int entry = index + 1;

  current_logging_index = index;

  sprintf( title, "Logging entry %d", index + 1);
  strcpy( title, Lng::translate(title));

  brow_pop();
  brow_SetNodraw( brow->ctx);
  new ItemHeader( brow, "Title", title,  NULL, flow_eDest_IntoLast);

  new ItemLocal( brow, Lng::translate("Active"), "logg_Active", 
	pwr_eType_Boolean, sizeof( logg[0].active), 0, 0, 1,
	(void *) &logg[index].active, NULL, flow_eDest_IntoLast);

  sprintf( command, "logging set/insert/entry=%d", entry);
  new ItemCommand( brow, Lng::translate("Insert"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging start/entry=%d", entry);
  new ItemCommand( brow, Lng::translate("Start"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging stop/entry=%d", entry);
  new ItemCommand( brow, Lng::translate("Stop"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging store/entry=%d/file=\"rtt_store_logg%d.rtt_com\"",
         entry, entry);
  new ItemCommand( brow, Lng::translate("Store"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "@rtt_store_logg%d", entry);
  new ItemCommand( brow, Lng::translate("Restore"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "open loggfile/entry=current");
  new ItemCommand( brow, Lng::translate("ShowFile"), 0, NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  new ItemLocal( brow, Lng::translate("Time (ms)"), "logg_Time", 
	pwr_eType_Int32, sizeof( logg[0].logg_time), 10, 100000, 0,
	(void *) &logg[index].logg_time, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, Lng::translate("File"), "logg_File", 
	pwr_eType_String, sizeof( logg[0].logg_filename), 0, 0, 0,
	(void *) logg[index].logg_filename, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, Lng::translate("Type"), "logg_Type",
	pwr_eType_Int32, sizeof( logg[0].logg_type), 0, 0, 0,
	(void *) &logg[index].logg_type, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, Lng::translate("BufferSize"), "logg_BufferSize", 
	pwr_eType_Int32, sizeof( logg[0].wanted_buffer_size), 0, 0, 0,
	(void *) &logg[index].wanted_buffer_size, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, Lng::translate("FullBufferStop"), "logg_BufferStop", 
	pwr_eType_Boolean, sizeof( logg[0].intern), 0, 0, 0,
	(void *) &logg[index].intern, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, Lng::translate("ShortName"), "logg_ShortName", 
	pwr_eType_Boolean, sizeof( logg[0].print_shortname), 0, 0, 0,
	(void *) &logg[index].print_shortname, NULL, flow_eDest_IntoLast);
  for ( int i = 0; i < RTT_LOGG_MAXPAR; i++) {
    sprintf( text, "%s%d", Lng::translate("Parameter"), i);
    new ItemLocal( brow, text, text, 
	pwr_eType_String, sizeof( logg[0].parameterstr[0]), 0, 0, 0,
	(void *) logg[index].parameterstr[i], NULL, flow_eDest_IntoLast);
  }
  new ItemLocal( brow, Lng::translate("ConditionParameter"), "logg_CondPar", 
	pwr_eType_String, sizeof( logg[0].conditionstr), 0, 0, 0,
	(void *) logg[index].conditionstr, NULL, flow_eDest_IntoLast);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int	XNavGbl::setupscript_exec( XNav *xnav)
{
  char cmd[80];

  if ( strcmp( setupscript, "") == 0)
    strcpy( setupscript, "$HOME/xtt_setup");

  strcpy( cmd, "@");
  strcat( cmd, setupscript);
  xnav->command( cmd);

  return XNAV__SUCCESS;
}

int	XNavGbl::load_config( XNav *xnav)
{
  int		sts;

  if ( strcmp( xnav->opplace_name, "") == 0)
    return 0;

  strcpy( OpPlace, xnav->opplace_name);

  sts = gdh_NameToPointer( xnav->opplace_name, (void **)&xnav->opplace_p);
  if ( EVEN(sts)) return sts;

  sts = gdh_NameToObjid( xnav->opplace_name, &xnav->gbl.OpObject);
  if ( EVEN(sts)) return sts;


  AlarmBeep = xnav->opplace_p->AlarmBell;
  AlarmReturn = (xnav->opplace_p->EventListEvents & pwr_mEventListMask_AlarmReturn) != 0;
  AlarmAck = (xnav->opplace_p->EventListEvents & pwr_mEventListMask_AlarmAck) != 0;
  hide_opwind = (xnav->opplace_p->OpWindLayout & pwr_mOpWindLayoutMask_HideOperatorWindow) != 0;
  hide_statusbar = (xnav->opplace_p->OpWindLayout & pwr_mOpWindLayoutMask_HideStatusBar) != 0;
  op_wind_pop = xnav->opplace_p->OpWindPop;
  strcpy( setupscript, xnav->opplace_p->SetupScript);
  dcli_trim( setupscript, setupscript);

  return XNAV__SUCCESS;
}

int XNav::brow_pop()
{
  BrowCtx *secondary_ctx;

  if ( brow_cnt >= XNAV_BROW_MAX)
    return 0;
  brow_CreateSecondaryCtx( brow->ctx, &secondary_ctx,
			   init_brow_cb, (void *)this, flow_eCtxType_Brow);

  brow_ChangeCtx( brow->ctx, brow_stack[brow_cnt]->ctx);
  *brow = *brow_stack[brow_cnt];
  brow_cnt++;
  return 1;
}

int XNav::brow_push()
{
  char push_cmd[200];

  if ( brow->ctx == collect_brow->ctx)
  {
    collect_show();
    return 1;
  }

  if ( brow_cnt == 1)
     return 0;

  strcpy( push_cmd, brow->push_command);

  brow_cnt--;
  brow_ChangeCtx( brow_stack[brow_cnt]->ctx, 
		brow_stack[brow_cnt-1]->ctx);
  *brow = *brow_stack[brow_cnt-1];
  delete brow_stack[brow_cnt];

  if ( strcmp( push_cmd, "") != 0)
    command( push_cmd);

  return 1;
}

int XNav::brow_push_all()
{
  while( brow_push())
    ;
  return 1;
}

void XNav::set_push_command( char *cmd)
{
  brow_stack[brow_cnt-1]->set_push_command( cmd);
  brow->set_push_command( cmd);
}

void XNav::menu_tree_build( xnav_sStartMenu *root)
{
  menu_tree = menu_tree_build_children( root, NULL);
}

xnav_sMenu *XNav::menu_tree_build_children( xnav_sStartMenu *first_child,
	xnav_sMenu *parent)
{
  xnav_sStartMenu 	*start_menu_p;
  xnav_sMenu		*menu_p, *prev;
  xnav_sMenu		*return_menu = NULL;
  int			first = 1;

  if ( !first_child)
    return NULL;

  start_menu_p = first_child;
  while ( strcmp( start_menu_p->title, ""))
  {
    switch ( start_menu_p->item_type)
    {
      case xnav_eItemType_Menu:
        menu_p = (xnav_sMenu *) calloc( 1, sizeof(xnav_sMenu));        
        menu_p->parent = parent;
        menu_p->item_type = start_menu_p->item_type;
        strcpy( menu_p->title, Lng::translate( start_menu_p->title));
        menu_p->child_list = menu_tree_build_children( 
		(xnav_sStartMenu *)start_menu_p->action, menu_p);
        if ( first)
        {
          return_menu = menu_p;
          first = 0;
        }
        else
          prev->next = menu_p;
        prev = menu_p;
        break;
      case xnav_eItemType_Command:
        menu_p = (xnav_sMenu *) calloc( 1, sizeof(xnav_sMenu));        
        menu_p->parent = parent;
        menu_p->item_type = start_menu_p->item_type;
        strcpy( menu_p->title, Lng::translate(start_menu_p->title));
        strcpy( menu_p->command, (char *)start_menu_p->action);
	menu_p->pixmap = (menu_ePixmap) start_menu_p->pixmap;
        if ( first)
        {
          return_menu = menu_p;
          first = 0;
        }
        else
          prev->next = menu_p;
        prev = menu_p;
        break;
      default:
        ;
    }
    start_menu_p++;
  }
  return return_menu;
}

void XNav::menu_tree_free()
{
  menu_tree_free_children( menu_tree);
}

void XNav::menu_tree_free_children( xnav_sMenu *first_child)
{
  xnav_sMenu *menu_p, *next;

  menu_p = next = first_child;  
  while( next)
  {
    menu_p = next;
    next = menu_p->next;
    menu_tree_free_children( menu_p->child_list);

    free( (char *) menu_p);
  }
}

int XNav::menu_tree_delete( char *name)
{
  int		sts;
  xnav_sMenu 	*delete_item, *mp;

  sts = menu_tree_search( name, &delete_item);
  if ( EVEN(sts)) return sts;

  if ( !delete_item->parent)
  {
    if ( menu_tree == delete_item)
      menu_tree = delete_item->next;
    else
    {
      for ( mp = menu_tree; mp->next != delete_item; mp = mp->next)
        ;
      mp->next = delete_item->next;
    }

    // Reconfigure the root menu
    brow_push_all();
    brow_SetNodraw( brow->ctx);
    brow_DeleteAll( brow->ctx);
    ((ItemMenu *)root_item)->open_children( brow, 0, 0);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }
  else
  {
    if ( delete_item->parent->child_list == delete_item)
      delete_item->parent->child_list = delete_item->next;
    else
    {
      for ( mp = delete_item->parent->child_list; mp->next != delete_item; mp = mp->next)
        ;
      mp->next = delete_item->next;
    }
  }
  free( (char *)delete_item);

  return 1;
}

int XNav::menu_tree_insert( char *title, int item_type, char *command, menu_ePixmap pixmap,
	char *destination, int dest_code, xnav_sMenu **menu_item)
{
  xnav_sMenu 	*dest_item;
  xnav_sMenu 	*menu_p, *child_p, *mp;
  int		sts;
  int		first_child = 0;

  if ( destination)
  {
    sts = menu_tree_search( destination, &dest_item);
    if ( EVEN(sts)) return sts;
  }

  switch ( item_type)
  {
    case xnav_eItemType_Command:
      menu_p = (xnav_sMenu *) calloc( 1, sizeof(xnav_sMenu));        
      menu_p->item_type = item_type;
      strcpy( menu_p->title, title);
      strcpy( menu_p->command, command);
      menu_p->pixmap = pixmap;
      break;
    case xnav_eItemType_Menu:
    default:
      menu_p = (xnav_sMenu *) calloc( 1, sizeof(xnav_sMenu));        
      menu_p->item_type = item_type;
      strcpy( menu_p->title, title);
      menu_p->pixmap = pixmap;
  }

  if ( !destination)
  {
    // Insert first
    menu_p->next = menu_tree;
    menu_tree = menu_p;
  }
  else
  {
    switch( dest_code)
    {
      case xnav_eDestCode_After:
        menu_p->next = dest_item->next;
        menu_p->parent = dest_item->parent;
        dest_item->next = menu_p;
        break;
      case xnav_eDestCode_Before:
        if ( !dest_item->parent)
        {
          if ( dest_item == menu_tree)
          {
            menu_p->next = dest_item;
            menu_tree = menu_p;
          }
          else
          {
            for ( mp = menu_tree; mp->next != dest_item; mp = mp->next)
              ;
            menu_p->next = mp->next;
            mp->next = menu_p;
          }
        }
        else
        {
          if ( dest_item == dest_item->parent->child_list)
          {
            menu_p->next = dest_item;
            dest_item->parent->child_list = menu_p;
          }
          else
          {
            for ( mp = dest_item->parent->child_list; mp->next != dest_item; mp = mp->next)
              ;
            menu_p->next = mp->next;
            mp->next = menu_p;
          }
        }
        menu_p->parent = dest_item->parent;
        break;
      case xnav_eDestCode_FirstChild:
        if ( !dest_item->child_list)
          first_child = 1;
        menu_p->next = dest_item->child_list;
        menu_p->parent = dest_item;
        dest_item->child_list = menu_p;
        break;
      case xnav_eDestCode_LastChild:
        if ( !dest_item->child_list)
        {
          first_child = 1;
          dest_item->child_list = menu_p;
        }
        else
        {
          for( child_p = dest_item->child_list; child_p->next; 
		child_p = child_p->next)
            ;
          child_p->next = menu_p;
        }
        menu_p->parent = dest_item;
        break;
    }
  }

  if ( menu_p->parent == NULL ||
       (menu_p->parent->parent == NULL && first_child))
  {
    // Reconfigure the root menu
    brow_push_all();
    brow_SetNodraw( brow->ctx);
    brow_DeleteAll( brow->ctx);
    ((ItemMenu *)root_item)->open_children( brow, 0, 0);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }

  *menu_item = menu_p;
  return 1;
}

int XNav::menu_tree_search( char *name, xnav_sMenu **menu_item)
{
  char upname[80];
  cdh_ToUpper( upname, name);
  return menu_tree_search_children( upname, menu_tree, menu_item);
}

int XNav::menu_tree_search_children( char *name, xnav_sMenu *child_list,
		xnav_sMenu **menu_item)
{
  xnav_sMenu *menu_p;
  char *s;
  char *next_search_name;
  char search_name[80];
  char up_title[80];
  int final_search;

  if ( !child_list)
    return 0;

  strcpy( search_name, name);
  s = strchr( search_name, '-');
  if ( s == 0)
  {
    final_search = 1;
  }
  else
  {
    final_search = 0;
    next_search_name = s + 1;
    *s = 0;
  }

  menu_p = child_list;
  while( menu_p)
  {
    cdh_ToUpper( up_title, menu_p->title);
    if ( strcmp( up_title, search_name) == 0)
    {
      if ( final_search)
      {
        *menu_item = menu_p;
        return 1;
      }
      else
      {
        return menu_tree_search_children( next_search_name, 
		menu_p->child_list, menu_item);
      }
    }
    menu_p = menu_p->next;
  }
  return 0;
}

void  XNav::enable_events( XNavBrow *brow)
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF1, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF2, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Resized, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int XNav::init_brow_base_cb( FlowCtx *fctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  BrowCtx *secondary_ctx;
  int		sts;

  xnav->brow = new XNavBrow( ctx, (void *)xnav, brow_eUserType_XNav);
  xnav->brow_stack[0] = new XNavBrow( ctx, (void *)xnav, brow_eUserType_XNav);
  xnav->brow_cnt++;

  xnav->brow->brow_setup();
  xnav->brow->create_nodeclasses();

  memcpy( xnav->brow_stack[0], xnav->brow, sizeof( *xnav->brow));
  xnav->enable_events( xnav->brow);

  // Create the root item
  xnav->root_item = new ItemMenu( xnav->brow, "Root",
	NULL, flow_eDest_After, &xnav->menu_tree, 1);

  // Open the root item
  ((ItemMenu *)xnav->root_item)->open_children( xnav->brow, 0, 0);

  sts = brow_TraceInit( ctx, trace_connect_bc, 
		trace_disconnect_bc, trace_scan_bc);
  xnav->trace_started = 1;
  trace_scan( xnav);

  brow_CreateSecondaryCtx( xnav->brow_stack[0]->ctx, &secondary_ctx,
			   init_brow_collect_cb, (void *)xnav, flow_eCtxType_Brow);

  // Start operator window
  if ( strcmp( xnav->opplace_name, "") != 0)
  {
    pwr_tCmd cmd;

    if ( !xnav->gbl.hide_opwind) {
      strcpy( cmd, "open op ");
      strcat( cmd, xnav->opplace_name);
      if ( xnav->op_close_button || 
	   !(xnav->opplace_p->OpWindLayout & pwr_mOpWindLayoutMask_HideCloseButton))
	strcat( cmd, " /closebutton");
      xnav->command( cmd);

      xnav->load_ev_from_opplace();
    }
    xnav->login_from_opplace();
  }
  else
    xnav->login();

  // Execute the setup script
  xnav->gbl.setupscript_exec( xnav);

  return 1;
}

int XNav::load_ev_from_opplace()
{
  char cmd[100];

  strcpy( cmd, "eventlist load");
  command( cmd);
  return XNAV__SUCCESS;
}

int XNav::login_from_opplace()
{
  int sts;
  unsigned int privilege;
  char	systemgroup[80];
  pwr_sSecurity sec;
  char username[80];

  sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", &systemgroup, 
		sizeof(systemgroup));
  if ( EVEN(sts)) return sts;


  priv = base_priv = 0;

  sts = gdh_GetSecurityInfo( &sec);
  if ( ODD(sts) && sec.XttUseOpsysUser)
    syi_UserName( username, sizeof(username));
  else
    strcpy( username, opplace_p->UserName);

  sts = user_GetUserPriv( systemgroup, username, &privilege);
  if ( EVEN(sts)) {
    priv = base_priv = 0;
    return sts;
  }
  strcpy( user, username);
  strcpy( base_user, username);
  priv = base_priv = privilege;

  if (op)
    op->set_title( user);

  return XNAV__SUCCESS;
}

int XNav::login()
{
  int sts;
  unsigned int privilege;
  char	systemgroup[80];
  pwr_sSecurity sec;
  char username[80];

  priv = base_priv = 0;

  sts = gdh_GetSecurityInfo( &sec);
  if ( EVEN(sts)) return sts;

  if ( sec.XttUseOpsysUser) {
    syi_UserName( username, sizeof(username));
  
    sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", &systemgroup, 
			      sizeof(systemgroup));
    if ( EVEN(sts)) return sts;
    
    sts = user_GetUserPriv( systemgroup, username, &privilege);
    if ( EVEN(sts)) return sts;

    strcpy( user, username);
    strcpy( base_user, username);
    priv = base_priv = privilege;
    return XNAV__SUCCESS;
  }
  priv = base_priv = sec.DefaultXttPriv;
  return XNAV__SUCCESS;
}

void XNav::open_login()
{
  pwr_tCmd cmd;
  strcpy( cmd, "login");
  command( cmd);
}

void XNav::logout()
{
  pwr_tCmd cmd;
  strcpy( cmd, "logout");
  command( cmd);
}

int XNav::init_brow_collect_cb( BrowCtx *ctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;

  xnav->collect_brow = new XNavBrow( ctx, (void *)xnav, brow_eUserType_XNav);

  xnav->collect_brow->brow_setup();
  xnav->collect_brow->create_nodeclasses();
  xnav->enable_events( xnav->collect_brow);

  return 1;
}

int XNav::init_brow_cb( BrowCtx *ctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;

  xnav->brow_stack[xnav->brow_cnt] = new XNavBrow( ctx, (void *)xnav, brow_eUserType_XNav);

  xnav->brow_stack[xnav->brow_cnt]->brow_setup();
  xnav->brow_stack[xnav->brow_cnt]->create_nodeclasses();
  xnav->enable_events( xnav->brow_stack[xnav->brow_cnt]);

  return 1;
}


ApplListElem::ApplListElem( applist_eType al_type, void *al_ctx, 
	pwr_sAttrRef *al_arp, const char *al_name, const char *al_instance):
	type(al_type), ctx(al_ctx), aref(*al_arp), next(NULL)
{
  strcpy( name, al_name);
  if ( al_instance)
    strcpy( instance, al_instance);
  else
    strcpy( instance, "");
}

void ApplList::insert( applist_eType type, void *ctx, 
	pwr_sAttrRef *arp, const char *name, const char *instance)
{
  ApplListElem *elem = new ApplListElem( type, ctx, arp, name, instance);
  elem->next = root;
  root = elem;
}

void ApplList::insert( applist_eType type, void *ctx, 
	pwr_tObjid objid, const char *name, const char *instance)
{
  pwr_sAttrRef aref = cdh_ObjidToAref( objid);

  ApplListElem *elem = new ApplListElem( type, ctx, &aref, name, instance);
  elem->next = root;
  root = elem;
}

void ApplList::remove( void *ctx)
{
  ApplListElem *elem;
  ApplListElem *prev;

  for ( elem = root; elem; elem = elem->next) {
    if ( elem->ctx == ctx) {
      if ( elem == root)
        root = elem->next;
      else
        prev->next = elem->next;    
      delete elem;
      return;
    }
    prev = elem;
  }
}

int ApplList::find( applist_eType type, pwr_sAttrRef *arp, void **ctx)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next) {
    if ( elem->type == type && cdh_ObjidIsEqual( elem->aref.Objid, arp->Objid) &&
	elem->aref.Offset == arp->Offset && elem->aref.Size == arp->Size) {
      *ctx = elem->ctx;
      return 1;
    }
  }
  return 0;
}

int ApplList::find( applist_eType type, pwr_tObjid objid, void **ctx)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next) {
    if ( elem->type == type && cdh_ObjidIsEqual( elem->aref.Objid, objid) &&
	 elem->aref.Flags.b.Object) {
      *ctx = elem->ctx;
      return 1;
    }
  }
  return 0;
}

int ApplList::find( applist_eType type, const char *name, const char *instance, void **ctx)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next) {
    if ( elem->type == type && cdh_NoCaseStrcmp( name, elem->name) == 0) {
      if ( instance && strcmp( elem->instance, "") != 0) {
        if ( cdh_NoCaseStrcmp( instance, elem->instance) == 0) {
          *ctx = elem->ctx;
          return 1;
        }
      }
      else {
        *ctx = elem->ctx;
        return 1;
      }
    }
  }
  return 0;
}

void ApplList::swap( int mode)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next) {
    switch( elem->type) {
    case applist_eType_Graph:
      ((XttGe *)elem->ctx)->swap( mode);
      break;
    case applist_eType_Trace:
      ((RtTrace *)elem->ctx)->swap( mode);
      break;
    case applist_eType_Attr:
      ((XAtt *)elem->ctx)->swap( mode);
      break;
    default: ;
    }
  }
}

char *XNav::get_message( int sts)
{
  static char msg[256];

  return msg_GetMsg( sts, msg, sizeof(msg));
}

int XNav::show_object_as_struct( 
			pwr_tObjid	objid, 
			char		*type_str, 
			char		*file_str)
{
  dcli_sStructElement *e_list;
  dcli_sStructElement *e_ptr;
  int		sts;
  char		*s;
  char		title[80];
  int		i;
  char 		*parameter_ptr;
  char 		*object_ptr;
  pwr_tRefId 	subid;
  pwr_tOName   	objname;
  char		attr_str[80];


  // Get object name
  sts = gdh_ObjidToName ( objid, objname, sizeof(objname), 
			  cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  // Create a title with objname without volume and classname
  s = strchr( objname, ':');
  if ( s)
    s++;
  else
    s = objname;
  strcpy( title, s);

  // Link to object
  sts = gdh_RefObjectInfo( objname,
		(pwr_tAddress *) &object_ptr, 
		&subid, 0);
  if ( EVEN(sts)) {
    message('E', "Unable to link to object");
    return XNAV__OBJECTNOTFOUND;
  }

  // Find the struct
  sts = dcli_readstruct_find( file_str, type_str, &e_list);
  if ( EVEN(sts)) return sts;

  brow_pop();
  brow_SetNodraw( brow->ctx);
  new ItemHeader( brow, "Title", title,  NULL, flow_eDest_IntoLast);


  i = 0;
  parameter_ptr = object_ptr;
  for ( e_ptr = e_list; e_ptr; e_ptr = e_ptr->next)
  {
    if ( e_ptr->struct_begin)
      continue;
    if ( i != 0)
      subid = pwr_cNDlid;
    sprintf( attr_str, "_A_ %d %d %d %d", objid.vid, objid.oix,
		(int)(parameter_ptr - object_ptr), e_ptr->size);

    new ItemObjectStruct( brow, e_ptr->name, attr_str, 
	e_ptr->type, e_ptr->size, 0,
	(void *) parameter_ptr, objid, subid, NULL, flow_eDest_IntoLast);

    i = 0;
    parameter_ptr += e_ptr->size;
  }
  dcli_readstruct_free( e_list);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  return XNAV__SUCCESS;
}

void XNav::swap( int mode)
{
  if ( !mode)
    printf( "XNav swap start\n");
  else
    printf( "XNav swap done\n");

  appl.swap( mode);

  if ( mode == 0) {
    if ( trace_started) {
      brow_TraceClose( brow->ctx);
      trace_timerid->remove();
    }
  }
  else if ( mode == 1) {
    if ( trace_started) {
      brow_TraceInit( brow->ctx, trace_connect_bc, 
			    trace_disconnect_bc, trace_scan_bc);
      trace_scan( this);
    }
  }
}

int XNav::sound( pwr_tAttrRef *sound)
{
  if ( !attach_audio)
    return 0;

  if ( !audio)
    audio = new XttAudio( wow);

  if ( audio->audio_ok) {
    audio->beep( sound);
    return 1;
  }
  return 0;
}

int XNav::sound_attached()
{
  if ( !attach_audio)
    return 0;

  if ( !audio)
    audio = new XttAudio( wow);

  if ( audio->audio_ok)
    return 1;
  return 0;
}

