/* ge_subpalette.cpp -- Display object info

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
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

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_attr.h"
#include "ge_subpalette.h"

#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_openmap12.h"
#include "ge_bitmap_valve1.h"
#include "ge_bitmap_valve2.h"
#include "ge_bitmap_pump3.h"
#include "ge_bitmap_valve4.h"
#include "ge_bitmap_ind5.h"
#include "ge_bitmap_ind6.h"
#include "ge_bitmap_pushbutton7.h"
#include "ge_bitmap_frame8.h"
#include "ge_bitmap_crosscon9.h"
#include "ge_bitmap_threewaycon10.h"
#include "ge_bitmap_cornercon11.h"
#include "ge_bitmap_arrow12.h"
#include "ge_bitmap_checkvalve13.h"
#include "ge_bitmap_tank14.h"
#include "ge_bitmap_filter15.h"
#include "ge_bitmap_pressureswitch16.h"
#include "ge_bitmap_hydrpump17.h"
#include "ge_bitmap_pressuregauge18.h"
#include "ge_bitmap_releasevalve19.h"
#include "ge_bitmap_releasevalve20.h"
#include "ge_bitmap_frame21.h"
#include "ge_bitmap_pincon22.h"
#include "ge_bitmap_motor23.h"
#include "ge_bitmap_speedswitch24.h"

#define SUBPALETTE__INPUT_SYNTAX 2
#define SUBPALETTE__OBJNOTFOUND 4
#define SUBPALETTE__STRINGTOLONG 6
#define SUBPALETTE__ITEM_NOCREA 8
#define SUBPALETTE__SUCCESS 1

static char null_str[] = "";

static int subpalette_init_brow_cb( FlowCtx *fctx, void *client_data);

//
// Convert attribute string to value
//
void SubPalette::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
//  Free pixmaps
//
void SubPaletteBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  for ( int i = 0; i < SUBP_PIXMAPS_SIZE; i++)
  {
    if ( !pixmaps[i])
      break;
    brow_FreeAnnotPixmap( ctx, pixmaps[i]);
  }
}

//
//  Create pixmaps for leaf, closed map and open map
//
void SubPaletteBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_leaf12_width;
	    pixmap_data[i].height =xnav_bitmap_leaf12_height;
	    pixmap_data[i].bits =xnav_bitmap_leaf12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_leaf);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_map12_width;
	    pixmap_data[i].height =xnav_bitmap_map12_height;
	    pixmap_data[i].bits =xnav_bitmap_map12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_map);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_openmap12_width;
	    pixmap_data[i].height =xnav_bitmap_openmap12_height;
	    pixmap_data[i].bits =xnav_bitmap_openmap12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_openmap);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_valve1_width;
	    pixmap_data[i].height = ge_bitmap_valve1_height;
	    pixmap_data[i].bits = ge_bitmap_valve1_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[1]);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_valve2_width;
	    pixmap_data[i].height = ge_bitmap_valve2_height;
	    pixmap_data[i].bits = ge_bitmap_valve2_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[2]);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_pump3_width;
	    pixmap_data[i].height = ge_bitmap_pump3_height;
	    pixmap_data[i].bits = ge_bitmap_pump3_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[3]);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_valve4_width;
	    pixmap_data[i].height = ge_bitmap_valve4_height;
	    pixmap_data[i].bits = ge_bitmap_valve4_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[4]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_ind5_width;
	    pixmap_data[i].height = ge_bitmap_ind5_height;
	    pixmap_data[i].bits = ge_bitmap_ind5_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[5]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_ind6_width;
	    pixmap_data[i].height = ge_bitmap_ind6_height;
	    pixmap_data[i].bits = ge_bitmap_ind6_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[6]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_pushbutton7_width;
	    pixmap_data[i].height = ge_bitmap_pushbutton7_height;
	    pixmap_data[i].bits = ge_bitmap_pushbutton7_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[7]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_frame8_width;
	    pixmap_data[i].height = ge_bitmap_frame8_height;
	    pixmap_data[i].bits = ge_bitmap_frame8_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[8]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_crosscon9_width;
	    pixmap_data[i].height = ge_bitmap_crosscon9_height;
	    pixmap_data[i].bits = ge_bitmap_crosscon9_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[9]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_threewaycon10_width;
	    pixmap_data[i].height = ge_bitmap_threewaycon10_height;
	    pixmap_data[i].bits = ge_bitmap_threewaycon10_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[10]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_cornercon11_width;
	    pixmap_data[i].height = ge_bitmap_cornercon11_height;
	    pixmap_data[i].bits = ge_bitmap_cornercon11_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[11]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_arrow12_width;
	    pixmap_data[i].height = ge_bitmap_arrow12_height;
	    pixmap_data[i].bits = ge_bitmap_arrow12_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[12]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_checkvalve13_width;
	    pixmap_data[i].height = ge_bitmap_checkvalve13_height;
	    pixmap_data[i].bits = ge_bitmap_checkvalve13_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[13]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_tank14_width;
	    pixmap_data[i].height = ge_bitmap_tank14_height;
	    pixmap_data[i].bits = ge_bitmap_tank14_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[14]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_filter15_width;
	    pixmap_data[i].height = ge_bitmap_filter15_height;
	    pixmap_data[i].bits = ge_bitmap_filter15_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[15]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_pressureswitch16_width;
	    pixmap_data[i].height = ge_bitmap_pressureswitch16_height;
	    pixmap_data[i].bits = ge_bitmap_pressureswitch16_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[16]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_hydrpump17_width;
	    pixmap_data[i].height = ge_bitmap_hydrpump17_height;
	    pixmap_data[i].bits = ge_bitmap_hydrpump17_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[17]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_pressuregauge18_width;
	    pixmap_data[i].height = ge_bitmap_pressuregauge18_height;
	    pixmap_data[i].bits = ge_bitmap_pressuregauge18_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[18]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_releasevalve19_width;
	    pixmap_data[i].height = ge_bitmap_releasevalve19_height;
	    pixmap_data[i].bits = ge_bitmap_releasevalve19_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[19]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_releasevalve20_width;
	    pixmap_data[i].height = ge_bitmap_releasevalve20_height;
	    pixmap_data[i].bits = ge_bitmap_releasevalve20_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[20]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_frame21_width;
	    pixmap_data[i].height = ge_bitmap_frame21_height;
	    pixmap_data[i].bits = ge_bitmap_frame21_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[21]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_pincon22_width;
	    pixmap_data[i].height = ge_bitmap_pincon22_height;
	    pixmap_data[i].bits = ge_bitmap_pincon22_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[22]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_motor23_width;
	    pixmap_data[i].height = ge_bitmap_motor23_height;
	    pixmap_data[i].bits = ge_bitmap_motor23_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[23]);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width = ge_bitmap_speedswitch24_width;
	    pixmap_data[i].height = ge_bitmap_speedswitch24_height;
	    pixmap_data[i].bits = ge_bitmap_speedswitch24_bits;
          }
	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmaps[24]);
}


//
// Create the navigator widget
//
SubPalette::SubPalette(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	trace_started(0),
	message_cb(NULL), traverse_focus_cb(NULL), set_focus_cb(NULL),
	menu_tree(NULL), path_cnt(0), displayed(0)
{
  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	subpalette_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
SubPalette::~SubPalette()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

SubPaletteBrow::~SubPaletteBrow()
{
  free_pixmaps();
}

void SubPalette::set_inputfocus( int focus)
{
//  brow_SetInputFocus( brow->ctx);
  Arg 		args[2];
  Pixel 	bg, fg;

  if ( !displayed)
    return;

  XtVaGetValues( form_widget, XmNbackground, &bg, XmNforeground, &fg, NULL);
  if ( !focus)
  {
    XtSetArg(args[0], XmNborderColor, bg);
    XtSetValues( form_widget, args, 1);
  }
  else
  {
    XtCallAcceptFocus( brow_widget, CurrentTime);
    XtSetArg(args[0], XmNborderColor, fg);
    XtSetValues( form_widget, args, 1);
  }
}

//
//  Return associated class of selected object
//

int SubPalette::get_select( char *text, char *filename)
{
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  int		sts;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  sts = 0;
  switch( item->type)
  {
    case subpalette_eItemType_File:
      strcpy( filename, ((ItemFile *)item)->filename);
      strcpy( text, ((ItemFile *)item)->name);
      sts = 1;
      break;
    default:
      ;
  }
  return sts;
}


//
// Callbacks from brow
//
static int subpalette_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  SubPalette		*subpalette;
  ItemLocalSubGraphs 	*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &subpalette);
  subpalette->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( subpalette->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( subpalette->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( subpalette->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( subpalette->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( subpalette->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( subpalette->brow->ctx, object);
      if ( !brow_IsVisible( subpalette->brow->ctx, object))
        brow_CenterObject( subpalette->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( subpalette->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( subpalette->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( subpalette->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( subpalette->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( subpalette->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( subpalette->brow->ctx, object);
      if ( !brow_IsVisible( subpalette->brow->ctx, object))
        brow_CenterObject( subpalette->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( subpalette->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      if ( subpalette->set_focus_cb)
        (subpalette->set_focus_cb)( subpalette->parent_ctx, subpalette);

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
            sts = subpalette_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( subpalette->brow->ctx, event->object.object))
          {
            brow_SelectClear( subpalette->brow->ctx);
          }
          else
          {
            brow_SelectClear( subpalette->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( subpalette->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( subpalette->brow->ctx);
      }
      break;
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( subpalette->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type)
      {
        case subpalette_eItemType_LocalSubGraphs: 
	  ((ItemLocalSubGraphs *)item)->open_children( subpalette, 0, 0);
          break;
        case subpalette_eItemType_Menu: 
	  ((ItemMenu *)item)->open_children( subpalette, 0, 0);
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

      brow_GetSelectedNodes( subpalette->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case subpalette_eItemType_LocalSubGraphs: 
	  ((ItemLocalSubGraphs *)item)->close( subpalette, 0, 0);
          break;
        case subpalette_eItemType_Menu: 
	  ((ItemMenu *)item)->close( subpalette, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( subpalette->brow->ctx);
      brow_SetInverse( node_list[0], 1);
      brow_SelectInsert( subpalette->brow->ctx, node_list[0]);
      if ( !brow_IsVisible( subpalette->brow->ctx, node_list[0]))
        brow_CenterObject( subpalette->brow->ctx, node_list[0], 0.25);
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
            case subpalette_eItemType_LocalSubGraphs: 
	      ((ItemLocalSubGraphs *)item)->open_children( subpalette,
			event->object.x, event->object.y);
              break;
            case subpalette_eItemType_Menu: 
	      ((ItemMenu *)item)->open_children( subpalette,
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
    case flow_eEvent_Key_Tab:
    {
      printf("Subpalette: Tab detected\n");
      if ( subpalette->traverse_focus_cb)
        (subpalette->traverse_focus_cb)( subpalette->parent_ctx, subpalette);
      break;
    }
    case flow_eEvent_Map:
    {
      subpalette->displayed = 1;
      break;
    }
    default:
      ;
  }
  return 1;
}


//
// Create nodeclasses
//
void SubPaletteBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "NavigatorDefault", 
		flow_eNodeGroup_Common, &nc_object);
  brow_AddFrame( nc_object, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_object, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_object, 1.5, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnotPixmap( nc_object, 1, 7.8, 0.1, flow_eDrawType_Line, 2, 0);

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

int	SubPalette::object_attr()
{
  char setup_filename[120];

  dcli_translate_filename( setup_filename, "pwr_exe:pwr_ge_setup.dat");

  brow_SetNodraw( brow->ctx);

  menu_tree_build( setup_filename);

  // Create the root item
  root_item = new ItemMenu( this, "Root",
	NULL, flow_eDest_After, &menu_tree, 1);

  // Open the root item
  ((ItemMenu *)root_item)->open_children( this, 0, 0);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return SUBPALETTE__SUCCESS;
}

void SubPalette::menu_tree_build( char *filename)
{
  ifstream	fp;
  int		line_cnt = 0;

  if ( !check_file( filename))
    return;

  fp.open( filename);
#ifndef OS_VMS
  if ( !fp)
    return;
#endif

  line_cnt = 0;
  menu_tree = menu_tree_build_children( &fp, &line_cnt, filename, NULL);
  fp.close();
}

subpalette_sMenu *SubPalette::menu_tree_build_children( ifstream *fp, int *line_cnt,
		char *filename, subpalette_sMenu *parent)
{
  subpalette_sMenu	*menu_p, *prev;
  subpalette_sMenu	*return_menu = NULL;
  int			first = 1;
  int			nr;
  char			line[140];
  char			file[120];
  char			type[120];
  int			pixmap;

  while ( 1)
  {
    fp->getline( line, sizeof( line));
    if ( line[0] == 0)
      break;
    (*line_cnt)++;
    if ( line[0] == '!' || line[0] == '#')
      continue;
    nr = sscanf( line, "%s %s %s %d", type, name, file, &pixmap);
    if ( nr < 1 )
      printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

    if ( strcmp( type, "{") == 0)
    {
      if ( nr != 1 || !menu_p)
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);
      menu_p->child_list = menu_tree_build_children( fp, line_cnt, filename,
		menu_p);
    }
    else if ( strcmp( type, "}") == 0)
    {
      if ( nr != 1 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);
      return return_menu;
    }
    else if ( strcmp( type, "menu") == 0)
    {
      if ( nr != 2 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      menu_p = (subpalette_sMenu *) calloc( 1, sizeof(subpalette_sMenu));        
      menu_p->parent = parent;
      menu_p->item_type = subpalette_eItemType_Menu;
      strcpy( menu_p->title, name);
      if ( first)
      {
        return_menu = menu_p;
        first = 0;
      }
      else
        prev->next = menu_p;
      prev = menu_p;
    }
    else if ( strcmp( type, "localsubgraphs") == 0)
    {
      if ( nr != 3 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      menu_p = (subpalette_sMenu *) calloc( 1, sizeof(subpalette_sMenu));        
      menu_p->parent = parent;
      menu_p->item_type = subpalette_eItemType_LocalSubGraphs;
      strcpy( menu_p->title, name);
      strcpy( menu_p->file, file);
      if ( first)
      {
        return_menu = menu_p;
        first = 0;
      }
      else
        prev->next = menu_p;
      prev = menu_p;
    }
    else if ( strcmp( type, "subgraph") == 0)
    {
      if ( nr != 4 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      menu_p = (subpalette_sMenu *) calloc( 1, sizeof(subpalette_sMenu));
      menu_p->parent = parent;
      menu_p->item_type = subpalette_eItemType_File;
      strcpy( menu_p->title, name);
      dcli_get_defaultfilename( file, menu_p->file, ".pwsg");
      menu_p->pixmap = pixmap;
      if ( first)
      {
        return_menu = menu_p;
        first = 0;
      }
      else
        prev->next = menu_p;
      prev = menu_p;
    }
    else if ( strcmp( type, "path") == 0)
    {
      if ( nr != 1 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      fp->getline( line, sizeof( line));
      if ( line[0] == 0)
        break;
      (*line_cnt)++;
      if ( line[0] == '!' || line[0] == '#')
        continue;
      nr = sscanf( line, "%s %s %s %d", type, name, file, &pixmap);
      if ( nr < 1 )
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      if ( strcmp( type, "{") != 0)
        printf( "** Syntax error in file %s, line %d", filename, *line_cnt);

      path_cnt = 0;
      while( 1)
      {
        fp->getline( line, sizeof( line));
        if ( line[0] == 0)
          break;
        (*line_cnt)++;
        if ( line[0] == '!' || line[0] == '#')
          continue;
        nr = sscanf( line, "%s %s %s %d", type, name, file, &pixmap);
        if ( nr < 1 )
          printf( "** Syntax error in file %s, line %d", filename, *line_cnt);
        if ( strcmp( type, "}") == 0)
        {
          if ( nr != 1 )
            printf( "** Syntax error in file %s, line %d", filename, *line_cnt);
          break;
        }
        else
        {
          if ( path_cnt > 10)
            break;

          strcpy( path[path_cnt], type);
          path_cnt++;
        }
      }
    }
  }

  return return_menu;
}

void SubPalette::menu_tree_free()
{
  menu_tree_free_children( menu_tree);
}

void SubPalette::menu_tree_free_children( subpalette_sMenu *first_child)
{
  subpalette_sMenu *menu_p, *next;

  menu_p = next = first_child;  
  while( next)
  {
    next = menu_p->next;
    free( (char *) menu_p);
  }
}

void SubPaletteBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, subpalette);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Tab, flow_eEventType_CallBack, 
	subpalette_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	subpalette_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int subpalette_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  SubPalette *subpalette = (SubPalette *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  subpalette->brow = new SubPaletteBrow( ctx, (void *)subpalette);

  subpalette->brow->brow_setup();
  subpalette->brow->create_nodeclasses();

  // Create the root item
  subpalette->object_attr();

  return 1;
}

ItemLocalSubGraphs::ItemLocalSubGraphs( SubPalette *subpalette, 
	char *item_name, char *item_filename,
	brow_tNode dest, flow_eDest dest_code) :
	Item(subpalette_eItemType_LocalSubGraphs)
{

  strcpy( name, item_name);
  strcpy( filename, item_filename);

  brow_CreateNode( subpalette->brow->ctx, item_name, 
		subpalette->brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

ItemFile::ItemFile( SubPalette *subpalette, 
	char *item_name, char *item_filename, int item_pixmap,
	brow_tNode dest, flow_eDest dest_code) :
	Item(subpalette_eItemType_File), pixmap(item_pixmap)
{

  strcpy( name, item_name);
  strcpy( filename, item_filename);

  brow_CreateNode( subpalette->brow->ctx, item_name, 
		subpalette->brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_leaf);
  if ( subpalette->brow->pixmaps[pixmap])
    brow_SetAnnotPixmap( node, 1, subpalette->brow->pixmaps[pixmap]);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemLocalSubGraphs::open_children( SubPalette *subpalette, double x, double y)
{
  double	node_x, node_y;
  int		child_exist;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( subpalette->brow->ctx);
    brow_CloseNode( subpalette->brow->ctx, node);
    if ( brow_IsOpen( node) & subpalette_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & subpalette_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_map);
    brow_ResetOpen( node, subpalette_mOpen_All);
    brow_ResetNodraw( subpalette->brow->ctx);
    brow_Redraw( subpalette->brow->ctx, node_y);
  }
  else
  {
    int			sts;
    char		found_file[120];
    char		fname[120];
    char		dev[80], dir[80], file[80], type[80];
    int			version;
    char		text[80];
    char		*s;
    int			idx;
    char		file_str[5][80];
    int		        nr;
    int                 i;

    // Create some children
    brow_SetNodraw( subpalette->brow->ctx);

    child_exist = 0;

    nr = dcli_parse( filename, ",", "", (char *)file_str,
		sizeof( file_str) / sizeof( file_str[0]), sizeof( file_str[0]), 0);
    for ( i = 0; i < nr; i++)
    {
      dcli_translate_filename( fname, file_str[i]);
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
      if ( ODD(sts))
      {
        dcli_parse_filename( found_file, dev, dir, file, type, &version);
        cdh_ToLower( text, file);
        text[0] = toupper( text[0]);

        // Skip next pages in animations
        if ( !((s = strstr( text, "__p")) && sscanf( s+3, "%d", &idx)))
        {
          new ItemFile( subpalette, text, found_file, 0, node, flow_eDest_IntoLast);
          child_exist = 1;
        }
      }
      while ( ODD(sts))
      {
        sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
        if ( ODD(sts))
        {
          dcli_parse_filename( found_file, dev, dir, file, type, &version);
          cdh_ToLower( text, file);
          text[0] = toupper( text[0]);

          if ( !((s = strstr( text, "__p")) && sscanf( s+3, "%d", &idx)))
          {
            new ItemFile( subpalette, text, found_file, 0, node, flow_eDest_IntoLast);
            child_exist = 1;
          }
        }
      }
      dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    }
    if ( child_exist)
    {
      brow_SetOpen( node, subpalette_mOpen_Children);
      brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_openmap);
    }
    brow_ResetNodraw( subpalette->brow->ctx);
    if ( child_exist)
      brow_Redraw( subpalette->brow->ctx, node_y);
  }
  return 1;
}

int ItemLocalSubGraphs::close( SubPalette *subpalette, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( subpalette->brow->ctx);
    brow_CloseNode( subpalette->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_map);
    brow_ResetOpen( node, subpalette_mOpen_All);
    brow_ResetNodraw( subpalette->brow->ctx);
    brow_Redraw( subpalette->brow->ctx, node_y);
  }
  return 1;
}

ItemMenu::ItemMenu( SubPalette *subpalette, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, subpalette_sMenu **item_child_list,
	int item_is_root) :
	Item( subpalette_eItemType_Menu), node(NULL), child_list(item_child_list), 
	is_root(item_is_root)
{
  type = subpalette_eItemType_Menu;
  strcpy( name, item_name);
  if ( !is_root)
  {
    brow_CreateNode( subpalette->brow->ctx, name, subpalette->brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);

    // Set pixmap
    if ( *child_list)
      brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_leaf);
    // Set object name annotation
    brow_SetAnnotation( node, 0, name, strlen(name));
  }
}

int ItemMenu::open_children( SubPalette *subpalette, double x, double y)
{
  int		action_open;

  if ( !is_root)
  {
    if ( !brow_IsOpen( node))
      action_open = 1;
    else 
      action_open = 0;
  }
  if ( action_open || is_root)
  {
    // Display childlist
    double	node_x, node_y;
    Item 		*item;
    subpalette_sMenu	*menu;

    if ( !is_root)
      brow_GetNodePosition( node, &node_x, &node_y);
    else
      node_y = 0;
    brow_SetNodraw( subpalette->brow->ctx);
    menu = *child_list;
    while ( menu)
    {
      switch ( menu->item_type)
      {
        case subpalette_eItemType_Menu:
          item = (Item *) new ItemMenu( subpalette, menu->title, node, 
		flow_eDest_IntoLast, &menu->child_list,
		0);
          break;
        case subpalette_eItemType_File:
          item = (Item *) new ItemFile( subpalette, menu->title, menu->file,
		menu->pixmap, node, flow_eDest_IntoLast);
          break;
        case subpalette_eItemType_LocalSubGraphs:
          item = (Item *) new ItemLocalSubGraphs( subpalette, 
		menu->title, menu->file, node, flow_eDest_IntoLast);
          break;
    
        default:
          ;
      }
      menu = menu->next;
      if ( !is_root)
      {
        brow_SetOpen( node, subpalette_mOpen_Children);
        brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_openmap);
      }
    }
    brow_ResetNodraw( subpalette->brow->ctx);
    brow_Redraw( subpalette->brow->ctx, node_y);
  }
  else
    close( subpalette, x, y);
  return 1;
}

int ItemMenu::close( SubPalette *subpalette, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( subpalette->brow->ctx);
    brow_CloseNode( subpalette->brow->ctx, node);
    if ( brow_IsOpen( node) & subpalette_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & subpalette_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, subpalette->brow->pixmap_map);
    brow_ResetOpen( node, subpalette_mOpen_All);
    brow_ResetNodraw( subpalette->brow->ctx);
    brow_Redraw( subpalette->brow->ctx, node_y);
  }
  return 1;
}

