/* co_msglist.cpp -- Message list

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
//#include "pwr_baseclasses.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

extern "C" {
#include "flow_x.h"
#include "co_mrm_util.h"
}
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "xnav_bitmap_morehelp12.h"

#include "co_msglist.h"

static int msglist_init_brow_cb( FlowCtx *fctx, void *client_data);
static int msglist_brow_cb( FlowCtx *ctx, flow_tEvent event);

//
//  Free pixmaps
//
void MsgListBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_morehelp);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void MsgListBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_morehelp12_width;
	    pixmap_data[i].height =xnav_bitmap_morehelp12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_morehelp12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_morehelp);
}

//
// Create nodeclasses
//
void MsgListBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "EventDefault", 
		flow_eNodeGroup_Common, &nc_default);
  brow_AddFrame( nc_default, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_default, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_default, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_default, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_default, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Error
  brow_CreateNodeClass( ctx, "Error", 
		flow_eNodeGroup_Common, &nc_error);
  brow_AddFrame( nc_error, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_error, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_error, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_LineRed);
  brow_AddRect( nc_error, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_error, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_error, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_error, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Fatal
  brow_CreateNodeClass( ctx, "Fatal", 
		flow_eNodeGroup_Common, &nc_fatal);
  brow_AddFrame( nc_fatal, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_fatal, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_fatal, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_LineRed);
  brow_AddRect( nc_fatal, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_fatal, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_fatal, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_fatal, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Warning
  brow_CreateNodeClass( ctx, "Warning", 
		flow_eNodeGroup_Common, &nc_warning);
  brow_AddFrame( nc_warning, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_warning, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_warning, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Yellow);
  brow_AddRect( nc_warning, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_warning, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_warning, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_warning, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Info
  brow_CreateNodeClass( ctx, "Info", 
		flow_eNodeGroup_Common, &nc_info);
  brow_AddFrame( nc_info, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_info, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddFilledRect( nc_info, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Green);
  brow_AddRect( nc_info, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_info, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_info, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_info, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Success
  brow_CreateNodeClass( ctx, "Success", 
		flow_eNodeGroup_Common, &nc_success);
  brow_AddFrame( nc_success, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_success, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  // brow_AddFilledRect( nc_success, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Green);
  brow_AddRect( nc_success, 0.8, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_success, 1.4, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_success, 2.2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_success, 9, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine,
		0);
}

void MsgListBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, msglist);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	msglist_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	msglist_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int msglist_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  MsgList *msglist = (MsgList *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  msglist->brow = new MsgListBrow( ctx, (void *)msglist);

  msglist->brow->brow_setup();
  msglist->brow->create_nodeclasses();

  return 1;
}

MsgList::MsgList(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	Widget *w) :
  parent_ctx(ev_parent_ctx), parent_wid(ev_parent_wid), find_wnav_cb(0), find_plc_cb(0)
{
  form_widget = ScrolledBrowCreate( parent_wid, "MsgList", NULL, 0, 
	msglist_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
}


//
//  Delete ev
//
MsgList::~MsgList()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

MsgListBrow::~MsgListBrow()
{
  free_pixmaps();
}


void MsgList::set_input_focus()
{
  if ( flow_IsViewable( brow_widget))
    XtCallAcceptFocus( brow_widget, CurrentTime);
}

//
//  Zoom
//
void MsgList::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void MsgList::unzoom()
{
  brow_UnZoom( brow->ctx);
}

void MsgList::set_nodraw()
{
  brow_SetNodraw( brow->ctx);
}

void MsgList::reset_nodraw()
{
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

void MsgList::remove_oldest()
{
  brow_tObject last;

  brow_GetLast( brow->ctx, &last);
  brow_DeleteNode( brow->ctx, last);
}

void MsgList::clear()
{
  brow_DeleteAll( brow->ctx);
}

//
// Callbacks from brow
//
static int msglist_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  MsgList		*msglist;
  ItemMsg 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &msglist);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( msglist->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( msglist->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( msglist->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( msglist->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( msglist->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( msglist->brow->ctx, object);
      if ( !brow_IsVisible( msglist->brow->ctx, object))
        brow_CenterObject( msglist->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( msglist->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( msglist->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( msglist->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( msglist->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( msglist->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( msglist->brow->ctx, object);
      if ( !brow_IsVisible( msglist->brow->ctx, object))
        brow_CenterObject( msglist->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( msglist->brow->ctx);
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
            sts = msglist_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( msglist->brow->ctx, event->object.object))
          {
            brow_SelectClear( msglist->brow->ctx);
          }
          else
          {
            brow_SelectClear( msglist->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( msglist->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( msglist->brow->ctx);
      }
      break;
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( msglist->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( msglist->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case msglist_eItemType_Msg:
//	  ((ItemLocal *)item)->close( msglist, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( msglist->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( msglist->brow->ctx, object);
      if ( !brow_IsVisible( msglist->brow->ctx, object))
        brow_CenterObject( msglist->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( msglist->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      item->find();
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
	  item->find();
          break;
        default:
          ;
      }
      break;
    default:
      ;
  }
  return 1;
}

ItemMsg::ItemMsg( MsgList *item_msglist, char *item_name,
	char *item_text, int item_severity,
	brow_tNode dest, flow_eDest dest_code):
	msglist(item_msglist), severity(item_severity)
{
  char type_str[8];
  char time_str[40];

  type = msglist_eItemType_Msg;

  strncpy( text, item_text, sizeof(text));
  text[sizeof(text)-1] = 0;

  switch ( severity) {
  case 'S':
    brow_CreateNode( msglist->brow->ctx, "Success", msglist->brow->nc_success,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "S");
    break;
  case 'I':
    brow_CreateNode( msglist->brow->ctx, "Info", msglist->brow->nc_info,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "I");
    break;
  case 'E':
    brow_CreateNode( msglist->brow->ctx, "Error", msglist->brow->nc_error,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "E");
    break;
  case 'W':
    brow_CreateNode( msglist->brow->ctx, "Warning", msglist->brow->nc_warning,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "W");
    break;
  case 'F':
    brow_CreateNode( msglist->brow->ctx, "Fatal", msglist->brow->nc_fatal,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "F");
    break;
  default:
    brow_CreateNode( msglist->brow->ctx, "Default", msglist->brow->nc_default,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "");
  }

  brow_SetAnnotation( node, 0, type_str, strlen(type_str));
  clock_gettime( CLOCK_REALTIME, &time);
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, 
	sizeof(time_str));
  time_str[17] = 0;
  brow_SetAnnotation( node, 1, time_str, strlen(time_str));
  brow_SetAnnotation( node, 2, text, strlen(text));

//  brow_SetAnnotPixmap( node, 0, msglist->brow->pixmap_leaf);
}

ItemMsgObject::ItemMsgObject( MsgList *item_msglist, char *item_name,
			      char *item_text, int item_severity, pwr_tOid item_oid,
			      brow_tNode dest, flow_eDest dest_code):
    ItemMsg( item_msglist, item_name, item_text, item_severity, dest, dest_code),
    oid(item_oid)
{
  brow_SetAnnotPixmap( node, 0, msglist->brow->pixmap_morehelp);
}

void ItemMsgObject::find()
{
  if ( msglist->find_wnav_cb)
    (msglist->find_wnav_cb)( msglist->parent_ctx, oid);
}

ItemMsgObjectPlc::ItemMsgObjectPlc( MsgList *item_msglist, char *item_name,
			      char *item_text, int item_severity, pwr_tOid item_oid,
			      brow_tNode dest, flow_eDest dest_code):
    ItemMsg( item_msglist, item_name, item_text, item_severity, dest, dest_code),
    oid(item_oid)
{
  brow_SetAnnotPixmap( node, 0, msglist->brow->pixmap_morehelp);
}

void ItemMsgObjectPlc::find()
{
  if ( msglist->find_plc_cb)
    (msglist->find_plc_cb)( msglist->parent_ctx, oid);
}






