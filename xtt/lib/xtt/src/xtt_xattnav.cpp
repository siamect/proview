/* wb_xattnav.cpp -- Display object info

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "rt_types.h"
#include "pwr_baseclasses.h"
#include "rt_xatt_msg.h"
#include "rt_mh_net.h"
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

#include "xtt_xatt.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "xtt_xnav_brow.h"
#include "xtt_xnav_crr.h"
#include "xtt_item.h"
extern "C" {
#include "pwr_privilege.h"
#include "co_mrm_util.h"
}

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

static char null_str[] = "";

static int xattnav_init_brow_cb( FlowCtx *fctx, void *client_data);

void XAttNav::message( char sev, char *text)
{
  if ( message_cb)
    (message_cb)( parent_ctx, sev, text);
}


//
// Create the navigator widget
//
XAttNav::XAttNav(
	void 		*xa_parent_ctx,
	Widget		xa_parent_wid,
	xattnav_eType   xa_type,
	char 		*xa_name,
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	Widget 		*w,
	pwr_tStatus 	*status) :
	parent_ctx(xa_parent_ctx), parent_wid(xa_parent_wid),
	type(xa_type), objar(*xa_objar), 
	advanced_user(xa_advanced_user), 
	bypass(0),
	trace_started(0), message_cb(NULL), close_cb(0), change_value_cb(0),
        popup_menu_cb(0), start_trace_cb(0), is_authorized_cb(0),
	displayed(0)
{
  strcpy( name, xa_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	xattnav_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
XAttNav::~XAttNav()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

void XAttNav::set_inputfocus()
{
//  brow_SetInputFocus( brow->ctx);

  if ( !displayed)
    return;

  XtCallAcceptFocus( brow_widget, CurrentTime);
}

//
// Check that the current selected item is valid for change
//
int XAttNav::check_attr( int *multiline, brow_tObject *node, char *name,
		char **init_value, int *size)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*base_item;

  *init_value = 0;
  *multiline = 0;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return XATT__NOSELECT;

  brow_GetUserData( node_list[0], (void **)&base_item);
  *node = node_list[0];
  free( node_list);

  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Collect:
    {
      ItemBaseAttr *item = (ItemBaseAttr *)base_item;

      strcpy( name, item->attr);
      return 1;
    }
    default:
      return XATT__NOCHANGE;
  }
}



//
// Callbacks from brow
//
static int xattnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  XAttNav		*xattnav;
  Item	 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &xattnav);
  xattnav->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( xattnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( xattnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( xattnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xattnav->brow->ctx, object);
      if ( !brow_IsVisible( xattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xattnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( xattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( xattnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( xattnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( xattnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xattnav->brow->ctx, object);
      if ( !brow_IsVisible( xattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xattnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( xattnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
    {
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
            sts = xattnav_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( xattnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( xattnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( xattnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( xattnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( xattnav->brow->ctx);
      }
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( xattnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case xnav_eItemType_Attr: 
	  ((ItemAttr *)item)->close( xattnav->brow, 0, 0);
          break;
        case xnav_eItemType_AttrArrayElem: 
	  ((ItemAttrArrayElem *)item)->close( xattnav->brow, 0, 0);
          break;
        case xnav_eItemType_AttrArray: 
	  ((ItemAttrArray *)item)->close( xattnav->brow, 0, 0);
          break;
        case xnav_eItemType_AttrObject: 
	  ((ItemAttrObject *)item)->close( xattnav->brow, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( xattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xattnav->brow->ctx, object);
      if ( !brow_IsVisible( xattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( xattnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_MB3Down:
    {
      brow_SetClickSensitivity( xattnav->brow->ctx, 
				      flow_mSensitivity_MB3Press);
      break;
    }
    case flow_eEvent_MB3Press:
    {
      // Popup menu
      Widget popup;
      pwr_sAttrRef attrref;
      int sts;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          if ( cdh_ObjidIsNull( item->objid))
            break;

          memset( &attrref, 0, sizeof(attrref));
          switch ( item->type) {
            case xnav_eItemType_Attr:
            case xnav_eItemType_AttrArrayElem:
            case xnav_eItemType_Collect:
            {
              char attr_str[140];

              sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
              if ( EVEN(sts)) return sts;

              strcat( attr_str, ".");
              strcat( attr_str, item->name);
              sts = gdh_NameToAttrref( pwr_cNObjid, attr_str, &attrref);
              if ( EVEN(sts)) return sts;

              (xattnav->popup_menu_cb)( xattnav->parent_ctx, attrref,
		     (unsigned long)xmenu_eItemType_Attribute,
		     (unsigned long)xmenu_mUtility_AttrEditor, NULL, &popup);

              break;
            }
            case xnav_eItemType_Crossref:
            {
              ItemCrossref *itemc = (ItemCrossref *)item;

              memset( &attrref, 0, sizeof(attrref));
              attrref.Objid = itemc->objid;
              (xattnav->popup_menu_cb)( xattnav->parent_ctx, attrref,
		     (unsigned long)xmenu_eItemType_Crossref,
		     (unsigned long)xmenu_mUtility_AttrEditor, 
		     itemc->ref_name, &popup);
              break;
            }
	    default:
              ;
          }
          if ( !popup) 
            break;

          mrm_PositionPopup( popup, xattnav->brow_widget, 
			       event->any.x_pixel + 8, event->any.y_pixel);
          XtManageChild(popup);

          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;
      pwr_tStatus	sts;

      brow_GetSelectedNodes( xattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type) {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArrayElem:
	sts = item->open_children( xattnav->brow, 0, 0);
	if (ODD(sts)) break;

	if ( xattnav->advanced_user && xattnav->change_value_cb)
	  (xattnav->change_value_cb)( xattnav->parent_ctx);
	break;
      case xnav_eItemType_AttrArray: 
	((ItemAttrArray *)item)->open_attributes( xattnav->brow, 0, 0);
	break;
      case xnav_eItemType_AttrObject: 
	((ItemAttrObject *)item)->open_attributes( xattnav->brow, 0, 0);
	break;
      case xnav_eItemType_Enum:
	if ( xattnav->advanced_user)
	  ((ItemEnum *)item)->set_value();
	break;
      case xnav_eItemType_Mask:
	if ( xattnav->advanced_user)
	  ((ItemMask *)item)->toggle_value();
	break;
      default:
	;
      }
      free( node_list);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case xnav_eItemType_Attr:
	      ((ItemAttr *)item)->open_children( xattnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_AttrArray:
	      ((ItemAttrArray *)item)->open_attributes( xattnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_AttrObject:
	      ((ItemAttrObject *)item)->open_attributes( xattnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_AttrArrayElem:
	      ((ItemAttrArrayElem *)item)->open_children( xattnav->brow,
			event->object.x, event->object.y);
              break;
            case xnav_eItemType_Crossref:
              if ( xattnav->start_trace_cb) {
                (xattnav->start_trace_cb)( xattnav->parent_ctx, item->objid, 
				  ((ItemCrossref *)item)->ref_name);
                if ( xattnav->close_cb) {
                  (xattnav->close_cb)( xattnav->parent_ctx);
                  return 1;
                }
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
    case flow_eEvent_Radiobutton:
    {
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type) {
	  case xnav_eItemType_Enum: 
	    if ( !event->radiobutton.value)
	      ((ItemEnum *)item)->set_value();
	    break;
	  case xnav_eItemType_Mask: 
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
    case flow_eEvent_Map:
    {
      xattnav->displayed = 1;
      break;
    }

    default:
      ;
  }
  return 1;
}

static int xattnav_trace_connect_bc( brow_tObject object, char *name, 
		       char *attr, flow_eTraceType type, void **p)
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
    case xnav_eItemType_Enum:
    case xnav_eItemType_Mask:
    case xnav_eItemType_AttrArrayElem:
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
    default:
      ;
  }      
  return 1;
}

static int xattnav_trace_disconnect_bc( brow_tObject object)
{
  Item 		*base_item;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
    {
      ItemAttr	*item = (ItemAttr *) base_item;

      gdh_UnrefObjectInfo( item->subid);
      break;
    }
    default:
      ;
  }
  return 1;
}

static int xattnav_trace_scan_bc( brow_tObject object, void *p)
{
  Item		*base_item;
  char		buf[120];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case xnav_eItemType_Attr:
    case xnav_eItemType_AttrArrayElem:
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

      xnav_attrvalue_to_string( item->type_id, item->tid, p, buf, sizeof(buf), &len, NULL);
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
    default:
      ;
  }
  return 1;
}

static void xattnav_trace_scan( XAttNav *xattnav)
{
  int time = 200;

  if ( xattnav->trace_started)
  {
    brow_TraceScan( xattnav->brow->ctx);

    xattnav->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(xattnav->brow_widget) , time,
	(XtTimerCallbackProc)xattnav_trace_scan, xattnav);
  }
}

int	XAttNav::crossref()
{
  int sts;
  char name[120];
  pwr_tClassId classid;

  sts = gdh_AttrrefToName ( &objar, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttrRefTid( &objar, &classid);
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
    case pwr_cClass_Iv:
    case pwr_cClass_Ii:
    case pwr_cClass_Io:
    case pwr_cClass_Co:
      sts = xnav_crr_signal( brow, NULL, name, NULL);
      break;
    default:
      /* Not a signal */
      sts = xnav_crr_object( brow, NULL, name, NULL);
  }
  // if ( EVEN(sts))
  //  xnav->message(' ', XNav::get_message(sts));

  return XATT__SUCCESS;
}

int	XAttNav::object_attr()
{
  int	       	sts;
  pwr_tClassId	classid;
  unsigned long	elements;
  Item   	*item;
  int		attr_exist;
  int		i;
  gdh_sAttrDef 	*bd;
  int 		rows;
  char		aname[120];
  char		name[240];
  char		*s;

  brow_SetNodraw( brow->ctx);

  // Get objid for rtbody or sysbody

  sts = gdh_AttrrefToName ( &objar, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  s = strchr( name, '.');
  if ( s != 0)
    strcpy( aname, s + 1);
  else
    strcpy( aname, "");

  sts = gdh_GetAttrRefTid( &objar, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetObjectBodyDef( classid, &bd, &rows, objar.Objid);
  if ( EVEN(sts)) return sts;

  for ( i = 0; i < rows; i++) {
    if ( bd[i].flags & gdh_mAttrDef_Shadowed)
      continue;
    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	 bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
      continue;

    if ( strcmp( aname, "") == 0)
      strcpy( name, bd[i].attrName);
    else {
      strcpy( name, aname);
      strcat( name, ".");
      strcat( name, bd[i].attrName);
    }

    elements = 1;
    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ) {
      attr_exist = 1;
      item = (Item *) new ItemAttrArray( brow, objar.Objid, 0, 
					 flow_eDest_IntoLast,
					 name,
					 bd[i].attr->Param.Info.Elements, 
					 bd[i].attr->Param.Info.Type, 
					 bd[i].attr->Param.TypeRef, 
					 bd[i].attr->Param.Info.Size,
					 bd[i].attr->Param.Info.Flags, 0);
    }
    else if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS ) {
      attr_exist = 1;
      item = (Item *) new ItemAttrObject( brow, objar.Objid, 0, 
					  flow_eDest_IntoLast,
					  name,
					  bd[i].attr->Param.TypeRef,
					  bd[i].attr->Param.Info.Size,
					  bd[i].attr->Param.Info.Flags, 0, 0);
    }
    else {
      attr_exist = 1;
      item = (Item *) new ItemAttr( brow, objar.Objid, 0, 
				    flow_eDest_IntoLast, 
				    name,
				    bd[i].attr->Param.Info.Type, 
				    bd[i].attr->Param.TypeRef, 
				    bd[i].attr->Param.Info.Size,
				    bd[i].attr->Param.Info.Flags, 0, 
				    item_eDisplayType_Attr);
    } 
  }
  free( (char *)bd);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return XATT__SUCCESS;
}

void XAttNav::enable_events()
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	xattnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	xattnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int xattnav_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  int sts;
  XAttNav *xattnav = (XAttNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  xattnav->brow = new XNavBrow( ctx, (void *)xattnav, brow_eUserType_XAttNav);

  xattnav->brow->brow_setup();
  xattnav->brow->create_nodeclasses();
  xattnav->enable_events();

  // Create the root item
  switch ( xattnav->type) {
    case xattnav_eType_Object:
      xattnav->object_attr();
      break;
    case xattnav_eType_CrossRef:
      xattnav->crossref();
      break;
    default:
      ;
  }
  sts = brow_TraceInit( ctx, xattnav_trace_connect_bc, 
		xattnav_trace_disconnect_bc, xattnav_trace_scan_bc);
  xattnav->trace_started = 1;

  xattnav_trace_scan( xattnav);

  return 1;
}



int XAttNav::object_exist( brow_tObject object)
{
  brow_tObject 	*object_list;
  int		object_cnt;
  int		i;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++)
  {
    if ( object_list[i] == object)
      return 1;
  }
  return 0;
}

//
// Set attribute value
//
int XAttNav::set_attr_value( brow_tObject node, char *name, char *value_str)
{
  Item		*base_item;
  int		sts;
  char 		buff[1024];
  char          attr_str[120];

  // Check authorization
  if ( is_authorized_cb) {
    if ( !(is_authorized_cb( parent_ctx, 
			     pwr_mAccess_RtWrite | pwr_mAccess_System))) {
      message( 'E', "Not authorized for this operation");
      return XATT__NOTAUTHORIZED;
    }
  }

  // Check that object still exist
  if ( !object_exist( node))
    return XATT__DISAPPEARD;

  brow_GetUserData( node, (void **)&base_item);

  switch( base_item->type) {
    case xnav_eItemType_AttrArrayElem:
    case xnav_eItemType_Attr:
    {
      ItemAttr *item = (ItemAttr *)base_item;

      // Check that objid is still the same
      if ( strcmp(item->attr, name) != 0)
        return XATT__DISAPPEARD;

      sts = gdh_ObjidToName( item->objid, 
	    	attr_str, sizeof(attr_str), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      strcat( attr_str, ".");
      strcat( attr_str, item->attr);

      sts =  xnav_attr_string_to_value( item->type_id, 
	value_str, buff, sizeof(buff), item->size);
      if ( EVEN(sts))
        message( 'E', "Input syntax error");
      else
      {
        sts = gdh_SetObjectInfo( attr_str, buff, item->size);
        if ( EVEN(sts)) return sts;
      }
      return sts;
    }
    default:
      ;
  }
  return 1;
}

void XAttNav::redraw()
{
    brow_Redraw( brow->ctx, 0);
}

int XAttNav::select_by_name( char *name)
{
  Item		*base_item;
  brow_tObject 	*object_list;
  int		object_cnt;
  int		i;
  int           found;
  brow_tObject  object;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  found = 0;
  for ( i = 0; i < object_cnt; i++)
  {
    brow_GetUserData( object_list[i], (void **)&base_item);

    switch( base_item->type) {
      case xnav_eItemType_Attr:
      case xnav_eItemType_AttrArrayElem:
      {
        ItemBaseAttr *item = (ItemBaseAttr *)base_item;

	if ( strcmp( name, item->attr) == 0) {
          object = object_list[i];
          found = 1;
        }
        break;
      }
      default:
        ;
    }
    if ( found)
      break;
  }
  if ( !found)
    return XATT__ATTRNOTFOUND;

  brow_SelectClear( brow->ctx);
  brow_SetInverse( object, 1);
  brow_SelectInsert( brow->ctx, object);
  return XATT__SUCCESS;
}

void XAttNav::start_trace()
{
  brow_tNode	*node_list;
  int		node_count;
  ItemCrossref	*item;

  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  switch( item->type)
  {
    case xnav_eItemType_Crossref:
      if ( start_trace_cb)
        (start_trace_cb)( parent_ctx, item->objid, item->ref_name);
      break;
    default:
      ;
  }
}

void XAttNav::swap( int mode)
{
  if ( mode == 0) {
    if ( trace_started) {
      brow_TraceClose( brow->ctx);
      XtRemoveTimeOut( trace_timerid);
    }
  }
  else if ( mode == 1) {
    if ( trace_started) {
      brow_TraceInit( brow->ctx, xattnav_trace_connect_bc, 
		      xattnav_trace_disconnect_bc, xattnav_trace_scan_bc);
      xattnav_trace_scan( this);
    }
  }
}








