/* rt_xtt.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_nav_help.h"

extern "C" {
#include "pwr_privilege.h"
#include "rt_gdh.h"
#include "rt_gdb.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_api.h"
#include "co_msg.h"
#include "pwr_baseclasses.h"
#include "rt_xnav_msg.h"
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

extern "C" {
#include "flow_x.h"
#include "rt_trace.h"
#include "co_mrm_util.h"
}
#include "co_lng.h"
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "xtt_menu.h"
#include "xtt_xatt.h"
#include "xtt_xcrr.h"
#include "xtt_ge.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

static char null_str[] = "";

static void xnav_trace_close_cb( tra_tCtx tractx);
static void xnav_trace_help_cb(tra_tCtx tractx, char *key);
static void xnav_trace_subwindow_cb( void *ctx, pwr_tObjid objid);
static void xnav_trace_display_object_cb( void *ctx, pwr_tObjid objid);
static int xnav_is_authorized_cb( void *xnav, unsigned int access);
static void xnav_trace_collect_insert_cb( void *ctx, pwr_tObjid objid);
static void xnav_xatt_close_cb( void *xnav, void *xatt);
static void xnav_xcrr_close_cb( void *xnav, void *xcrr);
static void xnav_trace_scan( XNav *xnav);
static int xnav_trace_scan_bc( brow_tObject object, void *p);
// static void  xnav_type_id_to_name( int type_id, char *type_id_name);
static int xnav_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, void **p);
static int xnav_trace_disconnect_bc( brow_tObject object);
static int xnav_init_brow_cb( BrowCtx *ctx, void *client_data);
static int xnav_init_brow_base_cb( FlowCtx *fctx, void *client_data);
static int xnav_init_brow_collect_cb( BrowCtx *ctx, void *client_data);

//
//  Get trace attribute
//
int xnav_get_trace_attr( pwr_tObjid objid, char *attr)
{
  int sts;
  pwr_tClassId classid;
  char objname[100];

  sts = gdh_GetObjectClass( objid, &classid);
  if ( EVEN(sts)) return sts;

  switch ( classid)
  {
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
      strcpy( attr, "ActualValue");	
      break;   
    case pwr_cClass_ChanDi:
    case pwr_cClass_ChanDo:
    case pwr_cClass_ChanAi:
    case pwr_cClass_ChanAo:
    case pwr_cClass_ChanIi:
    case pwr_cClass_ChanIo:
      sts = gdh_ObjidToName ( objid, objname, sizeof(objname), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      strcat( objname, ".SigChanCon");
      sts = gdh_GetObjectInfo ( objname, &objid, sizeof( objid));
      if (EVEN(sts)) return sts;
      strcpy( attr, "ActualValue");
      break;
    case pwr_cClass_ChanCo:
      sts = gdh_ObjidToName ( objid, objname, sizeof(objname), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      strcat( objname, ".SigChanCon");
      sts = gdh_GetObjectInfo ( objname, &objid, sizeof( objid));
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
int  xnav_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size)
{
  int		sts;

  switch ( type_id )
  {
    case pwr_eType_Boolean:
    {
      if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      if ( *(pwr_tBoolean *)buffer_ptr > 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Float32:
    {
      if ( sscanf( value_str, "%f", (float *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Float64:
    {
      pwr_tFloat32 f;
      pwr_tFloat64 d;
      if ( sscanf( value_str, "%f", &f) != 1)
        return XNAV__INPUT_SYNTAX;
      d = f;
      memcpy( buffer_ptr, (char *) &d, sizeof(d));

      break;
    }
    case pwr_eType_Char:
    {
      if ( sscanf( value_str, "%c", (char *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Int8:
    {
      pwr_tInt8 	i8;
      pwr_tInt16	i16;
      if ( sscanf( value_str, "%hd", &i16) != 1)
        return XNAV__INPUT_SYNTAX;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_Int16:
    {
      if ( sscanf( value_str, "%hd", (short *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Int32:
    {
      if ( sscanf( value_str, "%d", (int *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_UInt8:
    {
      pwr_tUInt8 	i8;
      pwr_tUInt16	i16;
      if ( sscanf( value_str, "%hu", &i16) != 1)
        return XNAV__INPUT_SYNTAX;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_UInt16:
    {
      if ( sscanf( value_str, "%hu", (unsigned short *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_UInt32:
    case pwr_eType_Mask:
    case pwr_eType_Enum:
    {
      if ( sscanf( value_str, "%lu", (unsigned long *)buffer_ptr) != 1)
        return XNAV__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_String:
    {
      if ( (int) strlen( value_str) >= attr_size)
        return XNAV__STRINGTOLONG;
      strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
      break;
    }
    case pwr_eType_Objid:
    {
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	memcpy( buffer_ptr, &objid, sizeof(objid));
      break;
    }
    case pwr_eType_ClassId:
    {
      pwr_tClassId	classid;
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	classid = cdh_ClassObjidToId( objid);
      memcpy( buffer_ptr, (char *) &classid, sizeof(classid));
      break;
    }
    case pwr_eType_TypeId:
    {
      pwr_tTypeId	val_typeid;
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	val_typeid = cdh_TypeObjidToId( objid);
      memcpy( buffer_ptr, (char *) &val_typeid, sizeof(val_typeid));
      break;
    }
    case pwr_eType_ObjectIx:
    {
      pwr_tObjectIx	objectix;

      sts = cdh_StringToObjectIx( value_str, &objectix); 
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &objectix, sizeof(objectix));
      break;
    }
    case pwr_eType_VolumeId:
    {
      pwr_tVolumeId	volumeid;

      sts = cdh_StringToVolumeId( value_str, &volumeid); 
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &volumeid, sizeof(volumeid));
      break;
    }
    case pwr_eType_RefId:
    {
      pwr_tRefId	subid;

      sts = cdh_StringToSubid( value_str, &subid);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &subid, sizeof(subid));
      break;
    }
    case pwr_eType_AttrRef:
    {
      pwr_sAttrRef	attrref;

      sts = gdh_NameToAttrref ( pwr_cNObjid, value_str, &attrref);
      if (EVEN(sts)) return XNAV__OBJNOTFOUND;
  	memcpy( buffer_ptr, &attrref, sizeof(attrref));
      break;
    }
    case pwr_eType_Time:
    {
      pwr_tTime	time;

      sts = time_AsciiToA( value_str, &time);
      if (EVEN(sts)) return XNAV__INPUT_SYNTAX;
  	memcpy( buffer_ptr, (char *) &time, sizeof(time));
      break;
    }
    case pwr_eType_DeltaTime:
    {
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
void  xnav_attrvalue_to_string( int type_id, void *value_ptr, 
	char *str, int size, int *len, char *format)
{
  char			hiername[120];
  pwr_tObjid		objid;
  pwr_sAttrRef		*attrref;
  int			sts;
  char			timstr[64];

  if ( value_ptr == 0)
  {
    strcpy( str, "UNDEFINED");
    return;
  }

  switch ( type_id )
  {
    case pwr_eType_Boolean:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(pwr_tBoolean *)value_ptr);
      else
        *len = sprintf( str, format, *(pwr_tBoolean *)value_ptr);
      break;
    }
    case pwr_eType_Float32:
    {
      if ( !format)
        *len = sprintf( str, "%f", *(float *)value_ptr);
      else
        *len = sprintf( str, format, *(float *)value_ptr);
      break;
    }
    case pwr_eType_Float64:
    {
      if ( !format)
        *len = sprintf( str, "%f", *(double *)value_ptr);
      else
        *len = sprintf( str, format, *(double *)value_ptr);
      break;
    }
    case pwr_eType_Char:
    {
      if ( !format)
        *len = sprintf( str, "%c", *(char *)value_ptr);
      else
        *len = sprintf( str, format, *(char *)value_ptr);
      break;
    }
    case pwr_eType_Int8:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(char *)value_ptr);
      else
        *len = sprintf( str, format, *(char *)value_ptr);
      break;
    }
    case pwr_eType_Int16:
    {
      if ( !format)
        *len = sprintf( str, "%hd", *(short *)value_ptr);
      else
        *len = sprintf( str, format, *(short *)value_ptr);
      break;
    }
    case pwr_eType_Int32:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(int *)value_ptr);
      else
        *len = sprintf( str, format, *(int *)value_ptr);
      break;
    }
    case pwr_eType_UInt8:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(unsigned char *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned char *)value_ptr);
      break;
    }
    case pwr_eType_UInt16:
    {
      if ( !format)
        *len = sprintf( str, "%hd", *(unsigned short *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned short *)value_ptr);
      break;
    }
    case pwr_eType_UInt32:
    case pwr_eType_Mask:
    case pwr_eType_Enum:
    {
      if ( !format)
        *len = sprintf( str, "%d", *(unsigned int *)value_ptr);
      else
        *len = sprintf( str, format, *(unsigned int *)value_ptr);
      break;
    }
    case pwr_eType_String:
    {
      strncpy( str, (char *)value_ptr, size);
      str[size-1] = 0;
      *len = strlen(str);
      break;
    }
    case pwr_eType_Objid:
    {
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
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_AttrRef:
    {
      attrref = (pwr_sAttrRef *) value_ptr;
      sts = gdh_AttrrefToName ( attrref, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_Time:
    {
      sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_DateAndTime, 
		timstr, sizeof(timstr));
      if ( EVEN(sts))
        strcpy( timstr, "-");
      *len = sprintf( str, "%s", timstr);
      break;
    }
    case pwr_eType_DeltaTime:
    {
      sts = time_DtoAscii( (pwr_tDeltaTime *) value_ptr, 1, 
		timstr, sizeof(timstr));
      if ( EVEN(sts))
        strcpy( timstr, "Undefined time");
      *len = sprintf( str, "%s", timstr);
      break;
    }
    case pwr_eType_ObjectIx:
    {
      *len = sprintf( str, "%s", cdh_ObjectIxToString( NULL, 
		*(pwr_tObjectIx *) value_ptr, 1));
      break;
    }
    case pwr_eType_ClassId:
    {
      objid = cdh_ClassIdToObjid( *(pwr_tClassId *) value_ptr);
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_TypeId:
    {
      objid = cdh_TypeIdToObjid( *(pwr_tTypeId *) value_ptr);
      sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), cdh_mNName);
      if (EVEN(sts))
      {
        strcpy( str, "");
        *len = 0;
        break;
      }
      *len = sprintf( str, "%s", hiername);
      break;
    }
    case pwr_eType_VolumeId:
    {
      *len = sprintf( str, "%s", cdh_VolumeIdToString( NULL, 
		*(pwr_tVolumeId *) value_ptr, 1, 0));
      break;
    }
    case pwr_eType_RefId:
    {
      *len = sprintf( str, "%s", cdh_SubidToString( NULL, 
		*(pwr_tSubid *) value_ptr, 1));
      break;
    }

    // XNav specials
    case xnav_eType_ShortTime:
    {
      sts = time_AtoAscii( (pwr_tTime *) value_ptr, time_eFormat_Time, 
		timstr, sizeof(timstr));
      if ( EVEN(sts))
        strcpy( timstr, "Undefined time");
      *len = sprintf( str, "%s", timstr);
      break;
    }
    case xnav_eType_GdbNodeFlags:
    {
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

void XNav::message( char sev, char *text)
{
  if ( message_cb)
    (message_cb)( parent_ctx, sev, text);
}

int XNav::collect_insert( pwr_sAttrRef *attrref)
{
  ItemCollect 	*item;
  int		sts;
  char		attr[40];
  char		*s;
  char 		obj_name[120];
  pwr_sAttrRef 	ar;
  pwr_tTypeId 	a_type_id;
  unsigned int 	a_size;
  unsigned int 	a_offset;
  unsigned int 	a_dim;

  sts = gdh_AttrrefToName ( attrref, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  if ( (s = strrchr( name, '.')) != 0)
  {
    strcpy( attr, s+1);

    sts = gdh_GetAttributeCharAttrref( attrref, &a_type_id, &a_size, &a_offset, 
	&a_dim);
    if ( EVEN(sts)) return sts;
  }
  else
  {
    sts = xnav_get_trace_attr( attrref->Objid, attr);
    if ( EVEN(sts))
      return sts;
    sts = gdh_ObjidToName ( attrref->Objid, obj_name, sizeof(obj_name), 
	cdh_mNName);
    if ( EVEN(sts)) return sts;
    strcat( obj_name, ".");
    strcat( obj_name, attr);
    sts = gdh_NameToAttrref( pwr_cNObjid, obj_name, &ar);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharAttrref( &ar, &a_type_id, &a_size, &a_offset, 
	&a_dim);
    if ( EVEN(sts)) return sts;
  }

  item = new ItemCollect( collect_brow, attrref->Objid, attr, NULL, 
		flow_eDest_IntoLast, a_type_id, a_size, 0);
  message( 'I', "Object inserted");
  return 1;
}

int XNav::collect_show()
{
  if ( brow->ctx == collect_brow->ctx)
  {
    // Hide
    brow_ChangeCtx( brow_widget, collect_brow->ctx, 
		brow_stack[brow_cnt-1]->ctx);
    *brow = *brow_stack[brow_cnt-1];
  }
  else
  {
    // Show
    brow_ChangeCtx( brow_widget, brow_stack[brow_cnt-1]->ctx, collect_brow->ctx);
    *brow = *collect_brow;
  }
  return 1;
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
//  Pop xnav window
//
static Boolean set_displayed( void *xnav)
{
  ((XNav *)xnav)->displayed = 1;
  return True;
}

void XNav::pop()
{
  Widget parent, top;

  parent = XtParent( parent_wid);
  while( parent)
  {
    top = parent;
    if ( flow_IsShell( top))
      break;
    parent = XtParent( parent);
  }
  displayed = 0;
  flow_UnmapWidget( top);
  flow_MapWidget( top);

  // A fix to avoid a krash in setinputfocus
  XtAppAddWorkProc( XtWidgetToApplicationContext(top),
			(XtWorkProc)set_displayed, (XtPointer)this);
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
  if ( !node_count)
  {
    message( 'E', "Select an object");
    return;
  }

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  switch( item->type)
  {
    case xnav_eItemType_Object:
    case xnav_eItemType_Table:
      ((ItemBaseObject *)item)->open_crossref( brow, this, 0, 0);
      break;
    case xnav_eItemType_Channel:
      ((ItemChannel *)item)->open_crossref( brow, this, 0, 0);
      break;
    default:
      message( 'E', "Can't show crossreferences for this object type");
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
  if ( !node_count)
  {
    message( 'E', "Select an object");
    return;
  }

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  switch( item->type)
  {
    case xnav_eItemType_Object: 
      ((ItemObject *)item)->open_trace( brow, this, 0, 0);
      break;
    case xnav_eItemType_Plc: 
      ((ItemPlc *)item)->open_trace( brow, this, 0, 0);
      break;
    case xnav_eItemType_Crossref: 
      ((ItemCrossref *)item)->open_trace( brow, this, 0, 0);
      break;
    default:
      message( 'E', "Can't start trace for this object type");
  }
}

void XNav::start_trace( pwr_tObjid objid, char *object_str)
{
    tra_tCtx 	tractx;
    char     	name[80];
    char   	title[100];
    int		sts;
    pwr_tClassId classid;
    pwr_tObjid	window_objid;

    sts = gdh_GetObjectClass( objid, &classid);
    if ( EVEN(sts)) return;
    if ( classid == pwr_cClass_plc)
    {
      // Take the first child
      sts = gdh_GetChild( objid, &window_objid);
      if ( EVEN(sts)) return;
    }
    else
      window_objid = objid;

    if ( appl.find( applist_eType_Trace, window_objid, (void **) &tractx))
    {
      trace_pop( tractx);
      if ( object_str)
        sts = trace_search_object( tractx, object_str);
    }
    else
    {
      // New trace window
      sts = gdh_ObjidToName( window_objid, name, sizeof(name), cdh_mNName); 
      strcpy( title, "Trace ");
      strcat( title, name);

      tractx = trace_new( this, form_widget, window_objid, xnav_trace_close_cb,
		xnav_trace_help_cb,
		xnav_trace_subwindow_cb, xnav_trace_display_object_cb,
		xnav_trace_collect_insert_cb, xnav_is_authorized_cb);
      if (tractx)
      {
        tractx->popup_menu_cb = xnav_popup_menu_cb;
        tractx->call_method_cb = xnav_call_method_cb;
        appl.insert( applist_eType_Trace, (void *)tractx, window_objid, "", NULL);

        if ( object_str)
          sts = trace_search_object( tractx, object_str);
      }    
      else
        message('E', "Unable to start trace for this object");
    }

}

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

int XNav::open_object( pwr_tObjid objid)
{
  XAtt *xatt;
  int sts;
 
  if ( appl.find( applist_eType_Attr, objid, (void **) &xatt)) {
    xatt->pop();
  }
  else {
    xatt = new XAtt( form_widget, this, objid, gbl.advanced_user, &sts);
    if ( ODD(sts))
      xatt->close_cb = xnav_xatt_close_cb;
      xatt->popup_menu_cb = xnav_popup_menu_cb;
      xatt->call_method_cb = xnav_call_method_cb;
      xatt->is_authorized_cb = xnav_is_authorized_cb;
      appl.insert( applist_eType_Attr, (void *)xatt, objid, "", NULL);
  }
  return XNAV__SUCCESS;
}

int XNav::open_crossref( pwr_tObjid objid)
{
  XCrr *xcrr;
  int sts;
 
  if ( appl.find( applist_eType_Crossref, objid, (void **) &xcrr)) {
    xcrr->pop();
  }
  else {
    xcrr = new XCrr( form_widget, this, objid, gbl.advanced_user, &sts);
    if ( ODD(sts))
      xcrr->close_cb = xnav_xcrr_close_cb;
      xcrr->popup_menu_cb = xnav_popup_menu_cb;
      xcrr->start_trace_cb = xnav_start_trace_cb;
      appl.insert( applist_eType_Crossref, (void *)xcrr, objid, "", NULL);
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
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	brow_cnt(0), TraceList(NULL), trace_started(0),
	message_cb(NULL), close_cb(NULL), map_cb(NULL), change_value_cb(NULL),
	set_dimension_cb(NULL), ccm_func_registred(0), verify(0),
	menu_tree(NULL), ev(NULL), op(NULL), closing_down(0),
	base_priv(pwr_mPrv_System), priv(pwr_mPrv_System), displayed(0),
        current_logging_index(-1), search_last_found(0), search_compiled(0),
	xhelp(0)
{
  strcpy( name, xn_name);
  strcpy( opplace_name, xn_opplace_name);
  strcpy( base_user, "");
  strcpy( user, "");

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	xnav_init_brow_base_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);
  displayed = 1;

  // Create the root item
  *w = form_widget;

  menu_tree_build( root_menu);
  gbl.load_config( this);

  for ( int i = 0; i < XNAV_LOGG_MAX; i++)
    logg[i].init( i, (void *)this);

  *status = 1;
}

//
//  Delete a nav context
//
XNav::~XNav()
{
  closing_down = 1;

  menu_tree_free();
  for ( int i = 0; i < brow_cnt; i++) {
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    brow_stack[i]->free_pixmaps();
    delete brow_stack[i];
  }
  brow_DeleteSecondaryCtx( collect_brow->ctx);
  collect_brow->free_pixmaps();
  delete collect_brow;
  delete brow;
  if ( op)
    delete op;
  XtDestroyWidget( form_widget);
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

void XNav::set_inputfocus()
{
  // printf( "%d XNav inputfocus %d\n", displayed, (int) brow_widget);
  if ( displayed) {
    XtCallAcceptFocus( brow_widget, CurrentTime);
  }
//  brow_SetInputFocus( brow->ctx);
}

//
// Set attribute value
//
int XNav::set_attr_value( char *value_str)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*base_item;
  char		attr_str[120];
  int		sts;
  char		buffer[80];
  
  // Check authorization
  if ( !((priv & pwr_mPrv_RtWrite) ||
         (priv & pwr_mPrv_System)))
  {
    message('E', "Not authorized for this operation");
    return 0;
  }
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_Collect:
    case xnav_eItemType_AttrArrayElem:
    {
      ItemAttr	*item;

      item = (ItemAttr *)base_item;
      sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      strcat( attr_str, ".");
      strcat( attr_str, item->attr);

      sts = xnav_attr_string_to_value( item->type_id, value_str, 
		buffer, sizeof(buffer), item->size);
      if ( EVEN(sts)) return sts;

      sts = gdh_SetObjectInfo( attr_str, buffer, item->size);
      if ( EVEN(sts)) return sts;
      break;
    }
    case xnav_eItemType_Local:
    {
      ItemLocal	*item;
      int out_of_range = 0;

      item = (ItemLocal *)base_item;

      sts = xnav_attr_string_to_value( item->type_id, value_str, 
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
    case xnav_eItemType_ObjectStruct:
    {
      ItemObjectStruct	*item;

      item = (ItemObjectStruct *)base_item;

      sts = xnav_attr_string_to_value( item->type_id, value_str,
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
    case xnav_eItemType_Plc:
    {
      ItemTable	*item = (ItemTable *)base_item;
      int idx = item->change_value_idx;

      sts = xnav_attr_string_to_value( item->col.elem[idx].type_id, value_str, 
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

  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Collect:
      return 1;
    case xnav_eItemType_Local:
    {
      ItemLocal	*item;

      item = (ItemLocal *)base_item;

      if ( item->nochange) {
        message('E', "Attribute can't be changed");
        return XNAV__NOCHANGE;
      }
      return 1;
    }
    case xnav_eItemType_ObjectStruct:
    {
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
  char		attr_str[120];
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
  switch( item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArray:
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
  char		attr_str[120];
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
  for ( i = 0; i < node_count; i++)
  {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type)
    {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArray:
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
  char		attr_str[120];
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
  for ( i = 0; i < node_count; i++)
  {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type)
    {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArray:
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
  char		attr_str[120];
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
  for ( i = 0; i < node_count; i++)
  {
    brow_GetUserData( node_list[i], (void **)&item);

    sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    switch( item->type)
    {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArray:
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
    case pwr_eType_UInt8: strcpy( type_id_name, "UInt8"); break;
    case pwr_eType_UInt16: strcpy( type_id_name, "UInt16"); break;
    case pwr_eType_UInt32: strcpy( type_id_name, "UInt32"); break;
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
    default: strcpy( type_id_name, "");
  }
}
#endif

static void xnav_trace_subwindow_cb( void *ctx, pwr_tObjid objid)
{
  XNav *xnav = (XNav *) ctx;

  xnav->start_trace( objid, NULL);
}

static void xnav_trace_display_object_cb( void *ctx, pwr_tObjid objid)
{
  XNav *xnav = (XNav *) ctx;

  xnav->display_object( objid, 0);
  xnav->pop();
}

static int xnav_is_authorized_cb( void *xnav, unsigned int access)
{
  return (((XNav *)xnav)->priv & access) != 0;
}

static void xnav_trace_collect_insert_cb( void *ctx, pwr_tObjid objid)
{
  XNav 		*xnav = (XNav *) ctx;
  pwr_sAttrRef 	attrref;
  char		attr_str[140];
  int		sts;

  sts = gdh_ObjidToName( objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return;

  sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
  if ( EVEN(sts)) return;

  xnav->collect_insert( &attrref);
}

static void xnav_trace_close_cb(tra_tCtx tractx)
{
  ((XNav *)tractx->cp.parent_ctx)->appl.remove( (void *)tractx);
  trace_del( tractx);
}

static void xnav_trace_help_cb(tra_tCtx tractx, char *key)
{
  XNav *xnav = (XNav *) tractx->cp.parent_ctx;
  int sts;

  xnav->brow_push_all();
  pwr_tObjid objid;
  char objid_str[40];

  xnav->open_help();

  sts = xnav->xhelp->help( key, "", navh_eHelpFile_Project, NULL);
  if ( EVEN(sts)) {
    // Try to convert to objid and search for objid as topic
    sts = gdh_NameToObjid ( key, &objid);
    if ( ODD(sts)) {
      cdh_ObjidToString( objid_str, objid, 1);
      sts = xnav->xhelp->help( objid_str, "", navh_eHelpFile_Project, NULL);
    }
  }
  if ( EVEN(sts))
    sts = xnav->xhelp->help( key, "", navh_eHelpFile_Base, NULL);
  if ( EVEN(sts))
    xnav->message( 'E', "Unable to find topic");
  else
    xnav->message( ' ', null_str);
  // xnav->pop();
}

static void xnav_xatt_close_cb( void *xnav, void *xatt)
{
  ((XNav *)xnav)->appl.remove( xatt);
  delete (XAtt *)xatt;
}

static void xnav_xcrr_close_cb( void *xnav, void *xcrr)
{
  ((XNav *)xnav)->appl.remove( xcrr);
  delete (XCrr *)xcrr;
}

//
// Callbacks from brow
//
static int xnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  XNav		*xnav;
  Item 		*item;

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &xnav);
  if ( xnav->closing_down)
    return 1;

  xnav->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( xnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( xnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( xnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object))
        brow_CenterObject( xnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( xnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( xnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( xnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object))
        brow_CenterObject( xnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
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
      switch( item->type)
      {
        case xnav_eItemType_Plc: 
	  ((ItemPlc *)item)->open_attributes( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Object: 
        case xnav_eItemType_Table:
        case xnav_eItemType_Device:
        case xnav_eItemType_Channel:
        case xnav_eItemType_RemNode:
        case xnav_eItemType_RemTrans:
	  ((ItemBaseObject *)item)->open_attributes( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_AttrArray: 
	  ((ItemAttrArray *)item)->open_attributes( xnav->brow, 0, 0);
          break;
        default:
          ;
      }
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
      switch( item->type)
      {
        case xnav_eItemType_Object:
        case xnav_eItemType_Table:
	  ((ItemBaseObject *)item)->open_crossref( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_Channel:
	  ((ItemChannel *)item)->open_crossref( xnav->brow, xnav, 0, 0);
          break;
        default:
          ;
      }
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
      switch( item->type)
      {
        case xnav_eItemType_Plc: 
	  ((ItemPlc *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_File: 
	  ((ItemFile *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_Object: 
	  sts = ((ItemBaseObject *)item)->open_children( xnav->brow, 0, 0);
          if ( sts == XNAV__NOCHILD && xnav->gbl.advanced_user)
	    ((ItemBaseObject *)item)->open_attributes( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Table:
	  ((ItemBaseObject *)item)->open_children( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Device:
	  ((ItemDevice *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_Channel:
	  ((ItemChannel *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_RemNode:
	  ((ItemRemNode *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_RemTrans:
	  ((ItemRemTrans *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_AttrArray: 
	  ((ItemAttrArray *)item)->open_children( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Menu: 
	  ((ItemMenu *)item)->open_children( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Command: 
	  ((ItemCommand *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_Help: 
	  ((ItemHelp *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_HelpBold: 
	  ((ItemHelpBold *)item)->open_children( xnav->brow, xnav, 0, 0);
          break;
        case xnav_eItemType_Attr:
        case xnav_eItemType_AttrArrayElem:
        case xnav_eItemType_Collect:
        case xnav_eItemType_Local:
        case xnav_eItemType_ObjectStruct:
          if ( xnav->gbl.advanced_user && xnav->change_value_cb)
            (xnav->change_value_cb)( xnav->parent_ctx);
          break;
        default:
          ;
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
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case xnav_eItemType_Object: 
        case xnav_eItemType_Table: 
        case xnav_eItemType_Device: 
        case xnav_eItemType_Channel: 
        case xnav_eItemType_RemNode: 
        case xnav_eItemType_RemTrans: 
	  ((ItemBaseObject *)item)->close( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Plc: 
	  ((ItemPlc *)item)->close( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_AttrArray: 
	  ((ItemAttrArray *)item)->close( xnav->brow, 0, 0);
          break;
        case xnav_eItemType_Menu: 
	  ((ItemMenu *)item)->close( xnav->brow, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( xnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xnav->brow->ctx, object);
      if ( !brow_IsVisible( xnav->brow->ctx, object))
        brow_CenterObject( xnav->brow->ctx, object, 0.25);
      free( node_list);
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
          switch( item->type)
          {
            case xnav_eItemType_Plc: 
	      ((ItemPlc *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_File: 
	      ((ItemFile *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Object: 
            case xnav_eItemType_Table: 
	      ((ItemObject *)item)->open_children( xnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Device: 
	      ((ItemDevice *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Channel: 
	      ((ItemChannel *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_RemNode: 
	      ((ItemRemNode *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_RemTrans: 
	      ((ItemRemTrans *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_AttrArray: 
	      ((ItemAttrArray *)item)->open_children( xnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Menu: 
	      ((ItemMenu *)item)->open_children( xnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Command: 
	      ((ItemCommand *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Help: 
              ((ItemHelp *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_HelpBold: 
              ((ItemHelpBold *)item)->open_children( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_HelpHeader: 
              ((ItemHelpHeader *)item)->close( xnav->brow, xnav,
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
            sts = xnav_brow_cb( ctx, doubleclick_event);
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
          switch( item->type)
          {
            case xnav_eItemType_Object: 
            case xnav_eItemType_Table: 
            case xnav_eItemType_Device: 
            case xnav_eItemType_Channel: 
            case xnav_eItemType_RemNode: 
            case xnav_eItemType_RemTrans: 
	      ((ItemObject *)item)->open_attributes( xnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Plc: 
	      ((ItemPlc *)item)->open_attributes( xnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_AttrArray: 
	      ((ItemAttrArray *)item)->open_attributes( xnav->brow,
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
    case flow_eEvent_MB1DoubleClickShiftCtrl:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case xnav_eItemType_Object: 
            case xnav_eItemType_Table: 
	      ((ItemObject *)item)->open_trace( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Plc: 
	      ((ItemPlc *)item)->open_trace( xnav->brow, xnav,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Crossref: 
	      ((ItemCrossref *)item)->open_trace( xnav->brow, xnav,
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
            sts = xnav_brow_cb( ctx, doubleclick_event);
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
      break;
    case flow_eEvent_MB3Press:
    {            
      // Popup menu
      Widget popup;
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
              char attr_str[140];

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              memset( &attrref, 0, sizeof(attrref));
              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
              if ( EVEN(sts)) return sts;

              popup = xnav_create_popup_menu( xnav, attrref, 
				   xmenu_eItemType_Attribute,
				   xmenu_mUtility_XNav, xnav->priv, NULL);
              break;
            }
            case xnav_eItemType_Collect:
            {
              char attr_str[140];

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              memset( &attrref, 0, sizeof(attrref));
              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, item->name, &attrref);
              if ( EVEN(sts)) return sts;

              popup = xnav_create_popup_menu( xnav, attrref, 
				   xmenu_eItemType_Attribute,
				   xmenu_mUtility_XNav, xnav->priv, NULL);
              break;
            }
            case xnav_eItemType_Crossref:
            {
              ItemCrossref *itemc = (ItemCrossref *)item;
              memset( &attrref, 0, sizeof(attrref));
              attrref.Objid = itemc->objid;

              popup = xnav_create_popup_menu( xnav, attrref, 
			     xmenu_eItemType_Crossref,
			     xmenu_mUtility_XNav, xnav->priv, itemc->ref_name);
              break;
            }
	    default:
              memset( &attrref, 0, sizeof(attrref));
              attrref.Objid = item->objid;
              popup = xnav_create_popup_menu( xnav, attrref, 
				   xmenu_eItemType_Object,
				   xmenu_mUtility_XNav, xnav->priv, NULL);

          }
          if ( !popup) 
            break;

          mrm_PositionPopup( popup, xnav->brow_widget, 
			       event->any.x_pixel + 8, event->any.y_pixel);
          XtManageChild(popup);

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
  return 1;
}

static void xnav_trace_scan( XNav *xnav)
{
  int time = int( xnav->gbl.scantime * 1000);

  if ( xnav->trace_started)
  {
    flow_TraceScan( xnav->brow->ctx);

    xnav->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(xnav->brow_widget) , time,
	(XtTimerCallbackProc)xnav_trace_scan, xnav);
  }
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

static int xnav_trace_scan_bc( brow_tObject object, void *p)
{
  Item		*base_item;
  char		buf[120];
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

      xnav_attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
      brow_SetAnnotation( object, 1, buf, len);
      memcpy( item->old_value, p, min(item->size, (int) sizeof(item->old_value)));
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

      xnav_attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
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

      xnav_attrvalue_to_string( item->type_id, p, buf, sizeof(buf), &len, NULL);
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
            xnav_attrvalue_to_string( item->col.elem[i].type_id,
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

static int xnav_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, void **p)
{
  char		attr_str[160];
  int		sts;
  Item 		*base_item;

  /*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
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

static int xnav_trace_disconnect_bc( brow_tObject object)
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


int XNav::display_object( pwr_tObjid objid, int open)
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

    sts = gdh_ObjidToPointer( mountobject_list[mount_cnt], (void **)&mount_p);
    if ( ODD(sts)) {
      mounted_list[mount_cnt] = mount_p->Object;
      mount_cnt++;
    }
    sts = gdh_GetNextObject( mountobject_list[mount_cnt-1],
                &mountobject_list[mount_cnt]);
  }

  sts = gdh_GetParent( objid, &parent_list[parent_cnt]);
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

  for ( i = parent_cnt; i > 0; i--)
  {
    sts = find( parent_list[i - 1], (void **) &item);
    if ( EVEN(sts)) return sts;
    item->open_children( brow, 0, 0);
  }
  sts = find( objid, (void **) &item);
  if ( EVEN(sts)) return sts;
  brow_SetInverse( item->node, 1);
  brow_SelectInsert( brow->ctx, item->node);
  if ( open) {
    ((ItemBaseObject *)item)->open_children( brow, 0, 0);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  if ( open)
    brow_CenterObject( brow->ctx, item->node, 0.00);
  else
    brow_CenterObject( brow->ctx, item->node, 0.80);

  return 1;
}

int XNav::find( pwr_tObjid objid, void **item)
{
  flow_tObject 	*object_list;
  int		object_cnt;
  Item	 	*object_item;
  int		i;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++)
  {
    brow_GetUserData( object_list[i], (void **)&object_item);
    if ( cdh_ObjidIsEqual( object_item->objid, objid))
    {
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

  new ItemLocal( brow, "ConfigureObject", "setup_configureObject", 
        pwr_eType_String, sizeof( gbl.ConfigureObject), 0, 0, 1,
	(void *) gbl.ConfigureObject, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "DefaultDirectory", "setup_defaultdirectory", 
	pwr_eType_String, sizeof( gbl.default_directory), 0, 0, 0,
	(void *) gbl.default_directory, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "Scantime", "setup_scantime", 
	pwr_eType_Float64, sizeof( gbl.scantime), 0.010, 10, 0,
	(void *) &gbl.scantime, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmMessage", "setup_alarmmessage",
        pwr_eType_Boolean, sizeof( gbl.AlarmMessage), 0, 1, 0,
	(void *) &gbl.AlarmMessage, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmBeep", "setup_alarmbeep",
	pwr_eType_Boolean, sizeof( gbl.AlarmBeep), 0, 1, 0,
	(void *) &gbl.AlarmBeep, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmReturn", "setup_alarmreturn", 
	pwr_eType_Boolean, sizeof( gbl.AlarmReturn), 0, 1, 0,
	(void *) &gbl.AlarmReturn, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AlarmAck", "setup_alarmack", 
	pwr_eType_Boolean, sizeof( gbl.AlarmAck), 0, 1, 0,
	(void *) &gbl.AlarmAck, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "SymbolFilename", "setup_symbolfilename", 
	pwr_eType_String, sizeof(gbl.symbolfilename), 0, 0, 0,
	(void *) gbl.symbolfilename, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "Verify", "setup_verify", 
	pwr_eType_Int32, sizeof( gbl.verify), 0, 1, 0,
	(void *) &gbl.verify, NULL, flow_eDest_IntoLast);
  new ItemLocal( brow, "AdvancedUser", "setup_advanceduser", 
	pwr_eType_Int32, sizeof( gbl.advanced_user), 0, 1, 0,
	(void *) &gbl.advanced_user, NULL, flow_eDest_IntoLast);

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
  new ItemCommand( brow, Lng::translate("Insert"), NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging start/entry=%d", entry);
  new ItemCommand( brow, Lng::translate("Start"), NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging stop/entry=%d", entry);
  new ItemCommand( brow, Lng::translate("Stop"), NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "logging store/entry=%d/file=rtt_store_logg%d.rtt_com",
         entry, entry);
  new ItemCommand( brow, Lng::translate("Store"), NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "@rtt_store_logg%d", entry);
  new ItemCommand( brow, Lng::translate("Restore"), NULL,
		flow_eDest_IntoLast, command, 0, brow->pixmap_action);

  sprintf( command, "open loggfile/entry=current");
  new ItemCommand( brow, Lng::translate("ShowFile"), NULL,
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
	pwr_eType_Int32, sizeof( logg[0].buffer_size), 0, 0, 0,
	(void *) &logg[index].buffer_size, NULL, flow_eDest_IntoLast);
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

int	XNavGbl::symbolfile_exec( void *xnav)
{
  char cmd[80];

  if ( strcmp( symbolfilename, "") != 0)
  {
    strcpy( cmd, "@");
    strcat( cmd, symbolfilename);
    ((XNav *)xnav)->command( cmd);
  }
  return XNAV__SUCCESS;
}

int	XNavGbl::load_config( void *xnav)
{
  pwr_tClassId	classid;
  pwr_tObjid	objid;
  pwr_tObjid	node;
  char		config_name[80];
  char		parname[80];
  int		found;
  int		sts;
  char		*s;
  pwr_tFloat32	float32_val;

  if ( ConfigureObject[0])
  {
    strcpy( config_name, ConfigureObject);
    sts = gdh_NameToObjid ( config_name, &objid);
    if ( EVEN(sts)) return sts;
  }
  else
  {
    /* Try to find a RttConfig object for the node */
    sts = gdh_GetNodeObject ( 0, &node);
    if ( EVEN(sts)) return sts;

    /* Look for a RttConfig object as a child to the node object 
       with the name RttConfig */
    found = 0;
    sts = gdh_GetChild( node, &objid);
    while ( ODD(sts))
    {
      sts = gdh_GetObjectClass( objid, &classid);
      if ( EVEN(sts)) return sts;
      if ( classid == pwr_cClass_RttConfig)
      {
        sts = gdh_ObjidToName ( objid, config_name, sizeof(config_name), cdh_mNName);
        if (EVEN(sts)) return sts;
        if ( (s = strrchr( config_name, '-')))
          s++;
        else
          s = config_name; 
        cdh_ToUpper( s, s);
        if ( !strcmp( s, "RTTCONFIG"))
        {
          found = 1;
          break;
        }
      }	    
      sts = gdh_GetNextSibling ( objid, &objid);
    }
    if ( found == 0)
      return XNAV__OBJNOTFOUND;

    sts = gdh_ObjidToName ( objid, config_name, sizeof(config_name), cdh_mNName);
    if (EVEN(sts)) return sts;

    strcpy( ConfigureObject, config_name);
  }

  /* rtt_UserObject */
  strcpy( parname, config_name);
  strcat( parname, ".UserObject");
  sts = gdh_GetObjectInfo ( parname, &UserObject, sizeof( UserObject));

  /* AlarmAutoLoad */
  strcpy( parname, config_name);
  strcat( parname, ".AlarmAutoLoad");
  sts = gdh_GetObjectInfo ( parname, &AlarmAutoLoad, 
		sizeof( AlarmAutoLoad)); 

  /* AlarmMessage */
  strcpy( parname, config_name);
  strcat( parname, ".AlarmMessage");
  sts = gdh_GetObjectInfo ( parname, &AlarmMessage, sizeof( AlarmMessage)); 

  /* AlarmBeep */
  strcpy( parname, config_name);
  strcat( parname, ".AlarmBeep");
  sts = gdh_GetObjectInfo ( parname, &AlarmBeep, sizeof( AlarmBeep)); 

  /* AlarmReturn */
  strcpy( parname, config_name);
  strcat( parname, ".AlarmReturn");
  sts = gdh_GetObjectInfo ( parname, &AlarmReturn, sizeof( AlarmReturn)); 

  /* AlarmAck */
  strcpy( parname, config_name);
  strcat( parname, ".AlarmAck");
  sts = gdh_GetObjectInfo ( parname, &AlarmAck, sizeof( AlarmAck)); 

  /* DefaultDirectory */
  strcpy( parname, config_name);
  strcat( parname, ".DefaultDirectory");
  sts = gdh_GetObjectInfo ( parname, default_directory, 
		sizeof( default_directory)); 
  if ( EVEN(sts)) strcpy( default_directory, "");

  /* ScanTime */
  strcpy( parname, config_name);
  strcat( parname, ".ScanTime");
  sts = gdh_GetObjectInfo ( parname, &float32_val, 
		sizeof( float32_val)); 
  if (ODD(sts) && float32_val != 0)
    scantime = float32_val;

  /* SymbolFileName */
  strcpy( parname, config_name);
  strcat( parname, ".SymbolFileName");
  sts = gdh_GetObjectInfo ( parname, symbolfilename,
		sizeof( symbolfilename)); 
  if ( EVEN(sts))
    strcpy( symbolfilename, "");

  return XNAV__SUCCESS;
}

int XNav::brow_pop()
{
  BrowCtx *secondary_ctx;

  if ( brow_cnt >= XNAV_BROW_MAX)
    return 0;
  brow_CreateSecondaryCtx( brow->ctx, &secondary_ctx,
        xnav_init_brow_cb, (void *)this, flow_eCtxType_Brow);

  brow_ChangeCtx( brow_widget, brow->ctx, brow_stack[brow_cnt]->ctx);
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
  brow_ChangeCtx( brow_widget, brow_stack[brow_cnt]->ctx, 
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

int XNav::menu_tree_insert( char *title, int item_type, char *command,
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
      break;
    case xnav_eItemType_Menu:
    default:
      menu_p = (xnav_sMenu *) calloc( 1, sizeof(xnav_sMenu));        
      menu_p->item_type = item_type;
      strcpy( menu_p->title, title);
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
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF1, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF2, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	xnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Resized, flow_eEventType_CallBack, 
	xnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int xnav_init_brow_base_cb( FlowCtx *fctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  BrowCtx *secondary_ctx;
  int		sts;

  xnav->brow = new XNavBrow( ctx, (void *)xnav);
  xnav->brow_stack[0] = new XNavBrow( ctx, (void *)xnav);
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

  sts = brow_TraceInit( ctx, xnav_trace_connect_bc, 
		xnav_trace_disconnect_bc, xnav_trace_scan_bc);
  xnav->trace_started = 1;
  xnav_trace_scan( xnav);

  brow_CreateSecondaryCtx( xnav->brow_stack[0]->ctx, &secondary_ctx,
        xnav_init_brow_collect_cb, (void *)xnav, flow_eCtxType_Brow);

  // Execute the symbolfile
  xnav->gbl.symbolfile_exec( xnav);

  // Start operator window
  if ( strcmp( xnav->opplace_name, "") != 0)
  {
    char cmd[100];
    strcpy( cmd, "open op ");
    strcat( cmd, xnav->opplace_name);
    xnav->command( cmd);

    xnav->load_ev_from_opplace();
    xnav->login_from_opplace();
  }
  return 1;
}

int XNav::load_ev_from_opplace()
{
  int	user_found = 0;
  pwr_sClass_OpPlace *op_p;
  pwr_sClass_User *user_p;
  pwr_tObjid user_objid;
  int sts;

  // Find the corresponding User-object
    
  sts = gdh_NameToPointer( opplace_name, (void **) &op_p);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetClassList( pwr_cClass_User, &user_objid);
  while ( ODD(sts))
  {
    sts = gdh_ObjidToPointer( user_objid, (void **)&user_p);
    if ( EVEN(sts)) return sts;

    if ( user_p->OpNumber == op_p->OpNumber)
    {
      user_found = 1;
      break;
    }
    sts = gdh_GetNextObject( user_objid, &user_objid);
  }

  if ( user_found )
  {
    char cmd[100];

    gbl.UserObject = user_objid;
    strcpy( cmd, "eventlist load");
    command( cmd);
    return XNAV__SUCCESS;
  }
  return 0;
}

int XNav::login_from_opplace()
{
  pwr_sClass_User *user_p;
  int sts;
  unsigned int privilege;
  char	systemgroup[80];

  sts = gdh_ObjidToPointer( gbl.UserObject, (void **) &user_p);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", &systemgroup, 
		sizeof(systemgroup));
  if ( EVEN(sts)) return sts;

  sts = user_GetUserPriv( systemgroup, user_p->UserName, &privilege);
  if ( EVEN(sts)) {
    // Failiure, set read priv
    priv = pwr_mPrv_RtRead;
    base_priv = pwr_mPrv_RtRead;
    return sts;
  }
  strcpy( user, user_p->UserName);
  strcpy( base_user, user_p->UserName);
  priv = privilege;
  base_priv = privilege;
  return XNAV__SUCCESS;
}

static int xnav_init_brow_collect_cb( BrowCtx *ctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;

  xnav->collect_brow = new XNavBrow( ctx, (void *)xnav);

  xnav->collect_brow->brow_setup();
  xnav->collect_brow->create_nodeclasses();
  xnav->enable_events( xnav->collect_brow);

  return 1;
}

static int xnav_init_brow_cb( BrowCtx *ctx, void *client_data)
{
  XNav *xnav = (XNav *) client_data;

  xnav->brow_stack[xnav->brow_cnt] = new XNavBrow( ctx, (void *)xnav);

  xnav->brow_stack[xnav->brow_cnt]->brow_setup();
  xnav->brow_stack[xnav->brow_cnt]->create_nodeclasses();
  xnav->enable_events( xnav->brow_stack[xnav->brow_cnt]);

  return 1;
}


ApplListElem::ApplListElem( applist_eType al_type, void *al_ctx, 
	pwr_tObjid al_objid, char *al_name, char *al_instance):
	type(al_type), ctx(al_ctx), objid(al_objid), next(NULL)
{
  strcpy( name, al_name);
  if ( al_instance)
    strcpy( instance, al_instance);
  else
    strcpy( instance, "");
}

void ApplList::insert( applist_eType type, void *ctx, 
	pwr_tObjid objid, char *name, char *instance)
{
  ApplListElem *elem = new ApplListElem( type, ctx, objid, name, instance);
  elem->next = root;
  root = elem;
}

void ApplList::remove( void *ctx)
{
  ApplListElem *elem;
  ApplListElem *prev;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->ctx == ctx)
    {
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

int ApplList::find( applist_eType type, pwr_tObjid objid, void **ctx)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->type == type && cdh_ObjidIsEqual( elem->objid, objid))
    {
      *ctx = elem->ctx;
      return 1;
    }
  }
  return 0;
}

int ApplList::find( applist_eType type, char *name, char *instance, void **ctx)
{
  ApplListElem *elem;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->type == type && cdh_NoCaseStrcmp( name, elem->name) == 0)
    {
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
      ge_swap( (ge_tCtx)elem->ctx, mode);
      break;
    case applist_eType_Trace:
      trace_swap( (tra_tCtx)elem->ctx, mode);
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
  char		objname[120];
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
      XtRemoveTimeOut( trace_timerid);
    }
  }
  else if ( mode == 1) {
    if ( trace_started) {
      brow_TraceInit( brow->ctx, xnav_trace_connect_bc, 
			    xnav_trace_disconnect_bc, xnav_trace_scan_bc);
      xnav_trace_scan( this);
    }
  }
}


