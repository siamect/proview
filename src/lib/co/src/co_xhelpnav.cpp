/* co_xhelpnav.cpp -- helptext navigator

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_msg.h"
#include "pwr_baseclasses.h"
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
#include "flow_utils.h"

#include "co_xhelpnav.h"

extern "C" {
#include "flow_x.h"
#include "co_mrm_util.h"
#include "co_api.h"
}

#include "xnav_bitmap_morehelp8.h"
#include "xnav_bitmap_morehelp10.h"
#include "xnav_bitmap_morehelp12.h"
#include "xnav_bitmap_morehelp14.h"
#include "xnav_bitmap_morehelp16.h"
#include "xnav_bitmap_morehelp18.h"
#include "xnav_bitmap_morehelp20.h"
#include "xnav_bitmap_morehelp24.h"
#include "xnav_bitmap_closehelp8.h"
#include "xnav_bitmap_closehelp10.h"
#include "xnav_bitmap_closehelp12.h"
#include "xnav_bitmap_closehelp14.h"
#include "xnav_bitmap_closehelp16.h"
#include "xnav_bitmap_closehelp18.h"
#include "xnav_bitmap_closehelp20.h"
#include "xnav_bitmap_closehelp24.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

static int xhelpnav_init_brow_cb( BrowCtx *ctx, void *client_data);
static int xhelpnav_init_brow_base_cb( FlowCtx *fctx, void *client_data);
static int help_cmp_items( const void *node1, const void *node2);

static void xhelpnav_open_URL( char *link)
{
}

//
//  Free pixmaps
//
void XHelpNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_morehelp);
  brow_FreeAnnotPixmap( ctx, pixmap_closehelp);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void XHelpNavBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;
	
          i = 0;
	  pixmap_data[i].width =xnav_bitmap_morehelp8_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp8_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp10_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp10_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp12_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp12_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp14_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp14_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp16_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp16_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp18_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp18_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp24_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_morehelp);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_closehelp8_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp8_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp10_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp10_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp12_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp12_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp14_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp14_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp16_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp16_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp18_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp18_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_closehelp24_width;
	  pixmap_data[i].height =xnav_bitmap_closehelp24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_closehelp24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_closehelp);
}

//
// Create nodeclasses
//
void XHelpNavBrow::create_nodeclasses()
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

  // Create text

  brow_CreateNodeClass( ctx, "NavigatorText", 
		flow_eNodeGroup_Common, &nc_text);
  brow_AddFrame( nc_text, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_text, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_text, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_text, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_text, 7, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_text, 11, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
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

  // Create Header2

  brow_CreateNodeClass( ctx, "NavigatorHeadLarge", 
		flow_eNodeGroup_Common, &nc_headerlarge);
  brow_AddFrame( nc_headerlarge, 0, 0, 20, 1.4, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_headerlarge, 0, 0.5, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_headerlarge, 2, 1.0, 0,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_headerlarge, 8, 1.0, 1,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		1);

}

void XHelpNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, userdata);
}


void XHelpNav::clear()
{
  brow_DeleteAll( brow->ctx);
}


//
//  Pop xhelpnav window
//
static Boolean set_displayed( void *xhelpnav)
{
  ((XHelpNav *)xhelpnav)->displayed = 1;
  return True;
}

void XHelpNav::pop()
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
// Create the navigator widget
//
XHelpNav::XHelpNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	xhelp_eUtility xn_utility,
	Widget *w,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	brow_cnt(0), closing_down(0), displayed(0), utility(xn_utility)
{
  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	xhelpnav_init_brow_base_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);
  displayed = 1;

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
XHelpNav::~XHelpNav()
{
  closing_down = 1;

  for ( int i = 0; i < brow_cnt; i++) {
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    brow_stack[i]->free_pixmaps();
    delete brow_stack[i];
  }
  delete brow;
  XtDestroyWidget( form_widget);
}

//
//  Print
//
void XHelpNav::print( char *filename)
{
  brow_Print( brow->ctx, filename);
}


//
//  Zoom
//
void XHelpNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void XHelpNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

//
//  Get zoom
//
void XHelpNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

void XHelpNav::set_inputfocus()
{
  if ( displayed && flow_IsViewable( brow_widget)) {
    XtCallAcceptFocus( brow_widget, CurrentTime);
  }
}

//
// Callbacks from brow
//
static int xhelpnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  XHelpNav		*xhelpnav;
  HItem 		*item;

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &xhelpnav);
  if ( xhelpnav->closing_down)
    return 1;

  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( xhelpnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( xhelpnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( xhelpnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xhelpnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xhelpnav->brow->ctx, object);
      if ( !brow_IsVisible( xhelpnav->brow->ctx, object))
        brow_CenterObject( xhelpnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( xhelpnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( xhelpnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( xhelpnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( xhelpnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( xhelpnav->brow->ctx, object);
      if ( !brow_IsVisible( xhelpnav->brow->ctx, object))
        brow_CenterObject( xhelpnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      item->doubleclick_action( xhelpnav->brow, xhelpnav, 0, 0);
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( xhelpnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
	xhelpnav->brow_push();
        return 1;
      }

      if ( brow_IsOpen( node_list[0])) {
        // Close this node
        object = node_list[0];
        free( node_list);
      }
      else
      {
        // Close parent
        sts = brow_GetParent( xhelpnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
	  xhelpnav->brow_push();
          return 1;
        }
      }
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( xhelpnav->brow->ctx);
      break;
    case flow_eEvent_ObjectDeleted:
      brow_GetUserData( event->object.object, (void **)&item);
      delete item;
      break;
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          item->doubleclick_action( xhelpnav->brow, xhelpnav,
			event->object.x, event->object.y);
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
            sts = xhelpnav_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( xhelpnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( xhelpnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( xhelpnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( xhelpnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( xhelpnav->brow->ctx);
      }
      break;
    case flow_eEvent_Resized:
      brow_Redraw( xhelpnav->brow->ctx, 0);
      break;
    default:
      ;
  }
  return 1;
}

int XHelpNav::brow_pop()
{
  BrowCtx *secondary_ctx;

  if ( brow_cnt >= XHELPNAV_BROW_MAX)
    return 0;
  brow_CreateSecondaryCtx( brow->ctx, &secondary_ctx,
        xhelpnav_init_brow_cb, (void *)this, flow_eCtxType_Brow);

  brow_ChangeCtx( brow_widget, brow->ctx, brow_stack[brow_cnt]->ctx);
  *brow = *brow_stack[brow_cnt];
  brow_cnt++;
  return 1;
}

int XHelpNav::brow_push()
{
  if ( brow_cnt == 1)
     return 0;

  brow_cnt--;
  brow_ChangeCtx( brow_widget, brow_stack[brow_cnt]->ctx, 
		brow_stack[brow_cnt-1]->ctx);
  *brow = *brow_stack[brow_cnt-1];
  delete brow_stack[brow_cnt];

  return 1;
}

int XHelpNav::brow_push_all()
{
  while( brow_push())
    ;
  return 1;
}

void  XHelpNav::enable_events( XHelpNavBrow *brow)
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF1, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF2, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Resized, flow_eEventType_CallBack, 
	xhelpnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int xhelpnav_init_brow_base_cb( FlowCtx *fctx, void *client_data)
{
  XHelpNav *xhelpnav = (XHelpNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  xhelpnav->brow = new XHelpNavBrow( ctx, (void *)xhelpnav);
  xhelpnav->brow_stack[0] = new XHelpNavBrow( ctx, (void *)xhelpnav);
  xhelpnav->brow_cnt++;

  xhelpnav->brow->brow_setup();
  xhelpnav->brow->create_nodeclasses();

  memcpy( xhelpnav->brow_stack[0], xhelpnav->brow, sizeof( *xhelpnav->brow));
  xhelpnav->enable_events( xhelpnav->brow);

  return 1;
}

static int xhelpnav_init_brow_cb( BrowCtx *ctx, void *client_data)
{
  XHelpNav *xhelpnav = (XHelpNav *) client_data;

  xhelpnav->brow_stack[xhelpnav->brow_cnt] = new XHelpNavBrow( ctx, (void *)xhelpnav);

  xhelpnav->brow_stack[xhelpnav->brow_cnt]->brow_setup();
  xhelpnav->brow_stack[xhelpnav->brow_cnt]->create_nodeclasses();
  xhelpnav->enable_events( xhelpnav->brow_stack[xhelpnav->brow_cnt]);

  return 1;
}



HItemHeader::HItemHeader( XHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_Header;
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

HItemHeaderLarge::HItemHeaderLarge( XHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_HeaderLarge;
  brow_CreateNode( brow->ctx, "header", brow->nc_headerlarge,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

HItemHelpHeader::HItemHelpHeader( XHelpNavBrow *brow, char *item_name, char *title, bool base,
	brow_tNode dest, flow_eDest dest_code)
{
  type = xhelpnav_eHItemType_HelpHeader;
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
  if ( !base)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_closehelp);
}

int HItemHelpHeader::doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav, double x, double y)
{
  if ( xhelpnav)
    xhelpnav->brow_push();
  return 1;
}

HItemHelp::HItemHelp( XHelpNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
	file_type(help_file_type), index(help_index)
{
  type = xhelpnav_eHItemType_Help;
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_text,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

int HItemHelp::doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xhelpnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
      // Open the url
      xhelpnav_open_URL( link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1, true);
        if (EVEN(sts))
          sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1, true);
      }
      else
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1, true);
    }
  }
  return 1;
}

HItemHelpBold::HItemHelpBold( XHelpNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
        file_type(help_file_type), index(help_index)
{
  type = xhelpnav_eHItemType_HelpBold;
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

int HItemHelpBold::doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xhelpnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
      // Open the url
      xhelpnav_open_URL( link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1, true);
        if (EVEN(sts))
          sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1, true);
      }
      else
        sts = xhelpnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1, true);
    }
  }
  return 1;
}


/*************************************************************************
*
* Name:		help()
*
**************************************************************************/

void *xhelpnav_help_insert_cb( void *ctx, navh_eItemType item_type, char *text1,
		      char *text2, char *text3, char *link, 
		      char *bookmark, char *file_name,
		      navh_eHelpFile file_type, int help_index, char *bm)
{
  XHelpNav *xhelpnav = (XHelpNav *)ctx;

  if ( xhelpnav->init_help) {
    xhelpnav->brow_pop();
    brow_SetNodraw( xhelpnav->brow->ctx);
    xhelpnav->init_help = 0;
  }

  switch ( item_type) {
    case navh_eItemType_Help:
    {      
      HItemHelp *item = new HItemHelp( xhelpnav->brow, "help", text1, text2, text3,
	     link, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HelpBold:
    {
      HItemHelpBold *item = new HItemHelpBold( xhelpnav->brow, "help", text1, text2,
	     text3, link, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HelpHeader:
    {      
      HItemHelpHeader *item = new HItemHelpHeader( xhelpnav->brow, "help", text1, xhelpnav->brow_cnt == 1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
     case navh_eItemType_Header:
    {      
      HItemHeader *item = new HItemHeader( xhelpnav->brow, "help", text1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HeaderLarge:
    {      
      HItemHeaderLarge *item = new HItemHeaderLarge( xhelpnav->brow, "help", text1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    default:
      return 0;
  }
}

int	XHelpNav::help( char *help_key, char *help_bookmark, 
		navh_eHelpFile file_type, char *file_name, int pop, bool strict)
{
  int sts;
  brow_tNode bookmark_node;
  brow_tNode prev, first;
  NavHelp *navhelp;

  switch ( utility) {
  case xhelp_eUtility_Xtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseXtt, xhelp_cFile_Project);
    break;
  case xhelp_eUtility_Wtt:
    navhelp = new NavHelp( (void *)this, xhelp_cFile_BaseWtt, xhelp_cFile_Project);
    break;
  default:
    return 0;
  }
  navhelp->insert_cb = xhelpnav_help_insert_cb;

  if ( pop)
    init_help = 1;
  else {
    init_help = 0;
    brow_SetNodraw( brow->ctx);
  }
  sts = navhelp->help( help_key, help_bookmark, file_type, 
		       file_name, &bookmark_node, strict);
  if ( EVEN(sts)) {
    if ( !pop || (pop && !init_help)) {
      brow_push();
      brow_ResetNodraw( brow->ctx);
    }
    return sts;
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  if ( bookmark_node) {
    brow_CenterObject( brow->ctx, bookmark_node, 0.0);
    sts = brow_GetPrevious( brow->ctx, bookmark_node, &prev);
    if ( ODD(sts))
    {
      brow_SelectClear( brow->ctx);
      brow_SetInverse( prev, 1);
      brow_SelectInsert( brow->ctx, prev);
    }
  }
  else {
    sts = brow_GetFirst( brow->ctx, &first);
    if ( ODD(sts))
      brow_CenterObject( brow->ctx, first, 0.0);
  }
  delete navhelp;

  return 1;
}

/*************************************************************************
*
* Name:		help_index()
*
**************************************************************************/

int	XHelpNav::help_index( navh_eHelpFile file_type, char *file_name, int pop)
{
  int sts;
  brow_tObject 	*object_list;
  int		object_cnt;
  NavHelp *navhelp = new NavHelp( (void *)this, "$pwr_exe/xtt_help.dat",
				  "$pwrp_exe/xtt_help.dat");
  navhelp->insert_cb = xhelpnav_help_insert_cb;

  if (pop)
    brow_pop();
  brow_SetNodraw( brow->ctx);
  new HItemHelpHeader( brow, "help_index", "Index", brow_cnt == 1, NULL, flow_eDest_IntoLast);
  new HItemHelp( brow, "help_index", "",  "", "", "", "", NULL, 
		navh_eHelpFile_Base, 0, NULL, flow_eDest_IntoLast);

  sts = navhelp->help_index( file_type, file_name);

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);

  flow_qsort( &object_list[2], object_cnt - 2, sizeof(object_list[0]), 
	help_cmp_items);    

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return sts;
}

static int help_cmp_items( const void *node1, const void *node2)
{
  char	text1[80];
  char	text2[80];

  brow_GetAnnotation( *(brow_tNode *) node1, 0, text1, sizeof(text1));
  brow_GetAnnotation( *(brow_tNode *) node2, 0, text2, sizeof(text2));
  cdh_ToLower( text1, text1);
  cdh_ToLower( text2, text2);

  return ( strcmp( text1, text2));
}






