/* ge_subgraphs.cpp -- Display object info

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

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
extern "C" {
#include "flow_x.h"
}

#include "ge_attr.h"
#include "ge_subgraphs.h"
#include "ge_graph.h"

#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_openmap12.h"
#include "xnav_bitmap_attr12.h"

#define SUBGRAPHS__INPUT_SYNTAX 2
#define SUBGRAPHS__OBJNOTFOUND 4
#define SUBGRAPHS__STRINGTOLONG 6
#define SUBGRAPHS__ITEM_NOCREA 8
#define SUBGRAPHS__SUCCESS 1

static char null_str[] = "";

static int subgraphs_init_brow_cb( FlowCtx *fctx, void *client_data);
static void subgraphs_attr_close_cb( Attr *attrctx);
static void subgraphs_attr_redraw_cb( Attr *attrctx);
static void subgraphs_trace_scan( SubGraphs *subgraphs);
static int subgraphs_trace_scan_bc( brow_tObject object, void *p);
static int subgraphs_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, /* flow_eDrawType color, */ void **p);
static int subgraphs_trace_disconnect_bc( brow_tObject object);

static void subgraphs_create_sg_form( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
  subgraphs->subgraphs_form = w;
}

static void subgraphs_activate_exit( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
  if ( subgraphs->close_cb)
    (subgraphs->close_cb)( subgraphs);
  else
    delete subgraphs;
}

static void subgraphs_activate_attr( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    subgraphs->edit_attributes( item->nodeclass);
}

static void subgraphs_activate_set_extern( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    item->set_extern( 1);
}

static void subgraphs_activate_set_intern( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    item->set_extern( 0);
}

static void subgraphs_activate_help( Widget w, SubGraphs *subgraphs, XmAnyCallbackStruct *data)
{
}

//
// Convert attribute string to value
//
void SubGraphs::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
//  Free pixmaps
//
void SubGraphsBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void SubGraphsBrow::allocate_pixmaps()
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
	    pixmap_data[i].width =xnav_bitmap_attr12_width;
	    pixmap_data[i].height =xnav_bitmap_attr12_height;
	    pixmap_data[i].bits =xnav_bitmap_attr12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attr);

}


//
// Create the navigator widget
//
SubGraphs::SubGraphs(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	void *xn_growctx,
	Widget *w,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	trace_started(0), message_cb(NULL), close_cb(NULL), 
	grow_ctx(xn_growctx), attrlist(NULL)
{
  char		uid_filename[120] = {"pwr_exe:ge_subgraphs.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static MrmRegisterArg	reglist[] = {
        { "subgraphs_ctx", 0 },
	{"subgraphs_activate_exit",(caddr_t)subgraphs_activate_exit },
	{"subgraphs_activate_attr",(caddr_t)subgraphs_activate_attr },
	{"subgraphs_activate_set_extern",(caddr_t)subgraphs_activate_set_extern },
	{"subgraphs_activate_set_intern",(caddr_t)subgraphs_activate_set_intern },
	{"subgraphs_activate_help",(caddr_t)subgraphs_activate_help },
	{"subgraphs_create_sg_form",(caddr_t)subgraphs_create_sg_form }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

#if !defined OS_VMS
  {
    char *s;

    if ( (s = getenv( "pwr_exe")) == NULL)
    {
      printf( "** pwr_exe is not defined\n");
      exit(0);
    }
    sprintf( uid_filename, "%s/ge_subgraphs.uid", s);
  }
#endif

  // Create object context
  reglist[0].value = (caddr_t) this;

  // Motif
  MrmInitialize();

  strcpy( title, "Loaded SubGraphs");

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, "subgraphs_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

  i = 0;
  XtSetArg(args[i],XmNwidth,500);i++;
  XtSetArg(args[i],XmNheight,1200);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( subgraphs_form,
	name, NULL, 0, 
	subgraphs_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);


  XtPopup( parent_wid, XtGrabNone);

  // Create the root item
  *w = toplevel;

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)subgraphs_activate_exit, this);

  *status = 1;
}

//
//  Delete a nav context
//
SubGraphs::~SubGraphs()
{
  subgraphs_tAttr attrlist_p, next_p;

  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  // Delete all attr-widgets in attrlist
  attrlist_p = attrlist;
  next_p = NULL;
  while( attrlist_p)
  {
    next_p = attrlist_p->next;
    delete attrlist_p->attrctx;
    free( (char *) attrlist_p);
    attrlist_p = next_p;
  }

  delete brow;
  XtDestroyWidget( parent_wid);
}

SubGraphsBrow::~SubGraphsBrow()
{
  free_pixmaps();
}

void SubGraphs::set_inputfocus()
{
  brow_SetInputFocus( brow->ctx);
}

//
//  Return associated class of selected object
//

int SubGraphs::get_select( void **subgraph_item)
{
  brow_tNode	*node_list;
  int		node_count;
  SubGraphBaseItem *item;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  *subgraph_item = (void *)item;
  return 1;
}


//
// Callbacks from brow
//
static int subgraphs_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  SubGraphs		*subgraphs;
  ItemSubGraph 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &subgraphs);
  subgraphs->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( subgraphs->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( subgraphs->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( subgraphs->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( subgraphs->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( subgraphs->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( subgraphs->brow->ctx, object);
      if ( !brow_IsVisible( subgraphs->brow->ctx, object))
        brow_CenterObject( subgraphs->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( subgraphs->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( subgraphs->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( subgraphs->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( subgraphs->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( subgraphs->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( subgraphs->brow->ctx, object);
      if ( !brow_IsVisible( subgraphs->brow->ctx, object))
        brow_CenterObject( subgraphs->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( subgraphs->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          if ( brow_FindSelectedObject( subgraphs->brow->ctx, event->object.object))
          {
            brow_SelectClear( subgraphs->brow->ctx);
          }
          else
          {
            brow_SelectClear( subgraphs->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( subgraphs->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( subgraphs->brow->ctx);
      }
      break;
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( subgraphs->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type)
      {
        case subgraphs_eItemType_SubGraph: 
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

      brow_GetSelectedNodes( subgraphs->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case subgraphs_eItemType_SubGraph: 
          break;
        default:
          ;
      }
      brow_SelectClear( subgraphs->brow->ctx);
      brow_SetInverse( node_list[0], 1);
      brow_SelectInsert( subgraphs->brow->ctx, node_list[0]);
      if ( !brow_IsVisible( subgraphs->brow->ctx, node_list[0]))
        brow_CenterObject( subgraphs->brow->ctx, node_list[0], 0.25);
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
            case subgraphs_eItemType_SubGraph: 
              subgraphs->edit_attributes( item->nodeclass);
              break;
            default:
              ;
          }
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


//
// Create nodeclasses
//
void SubGraphsBrow::create_nodeclasses()
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
  brow_AddAnnot( nc_object, 9, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_object, 11, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

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

int	SubGraphs::object_attr()
{
  int	i, j;
  grow_tNodeClass *list;
  grow_tNodeClass *list_p;
  int	list_cnt;
  char	name[80];

  grow_GetNodeClassList( (grow_tCtx) grow_ctx, &list, &list_cnt);
  brow_SetNodraw( brow->ctx);

  list_p = list;
  for ( i = 0; i < list_cnt; i++)
  {

    grow_sAttrInfo	*grow_info, *grow_info_p;
    int			grow_info_cnt;
    int			*extern_p;

    grow_GetObjectAttrInfo( (grow_tObject)*list_p, NULL, &grow_info, &grow_info_cnt);

    grow_info_p = grow_info;
    extern_p = 0;
    for ( j = 0; j < grow_info_cnt; j++)
    {
      if ( strcmp( "Extern", grow_info_p->name) == 0)
      {
        extern_p = (int *)grow_info_p->value_p;
        break;
      }
      grow_info_p++;
    }
    grow_FreeObjectAttrInfo( grow_info);

    grow_GetNodeClassName( *list_p, name);

    new ItemSubGraph( this, name, extern_p, *list_p,
	NULL, flow_eDest_IntoLast);
    list_p++;
  }
    
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return SUBGRAPHS__SUCCESS;
}

void SubGraphsBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, subgraphs);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	subgraphs_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int subgraphs_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  SubGraphs *subgraphs = (SubGraphs *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  int sts;

  subgraphs->brow = new SubGraphsBrow( ctx, (void *)subgraphs);

  subgraphs->brow->brow_setup();
  subgraphs->brow->create_nodeclasses();

  // Create the root item
  subgraphs->object_attr();

  sts = brow_TraceInit( ctx, subgraphs_trace_connect_bc, 
		subgraphs_trace_disconnect_bc, subgraphs_trace_scan_bc);
  subgraphs->trace_started = 1;

  subgraphs_trace_scan( subgraphs);

  return 1;
}

ItemSubGraph::ItemSubGraph( SubGraphs *subgraphs, 
	char *item_name, int *item_extern_p, void *item_nodeclass,
	brow_tNode dest, flow_eDest dest_code) :
	SubGraphBaseItem(subgraphs_eItemType_SubGraph), 
	nodeclass(item_nodeclass), extern_p( item_extern_p), 
	old_extern(0), first_scan(0)
	
{

  strcpy( name, item_name);

  brow_CreateNode( subgraphs->brow->ctx, item_name, 
		subgraphs->brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, subgraphs->brow->pixmap_leaf);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));

  if ( *extern_p)
    brow_SetAnnotation( node, 1, "Extern", strlen("Extern"));
  else
    brow_SetAnnotation( node, 1, "Intern", strlen("Intern"));
  brow_SetTraceAttr( node, item_name, "", flow_eTraceType_User);
}

int SubGraphs::edit_attributes( void *object)
{
  attr_sItem	items[10];
  int		i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  Attr		        *attr;
  subgraphs_tAttr 	attrlist_p;
  graph_eTrace		trace_type;

  grow_GetObjectAttrInfo( (grow_tObject)object, NULL, &grow_info, &grow_info_cnt);
  grow_GetNodeClassTraceType( object, (glow_eTraceType *) &trace_type);

  grow_info_p = grow_info;
  for ( i = 0; i < grow_info_cnt; i++)
  {
    items[i].value = grow_info_p->value_p;
    strcpy( items[i].name, grow_info_p->name);
    if ( grow_info_p->type == glow_eType_TraceColor)
    {
      switch ( trace_type)
      {
        case graph_eTrace_DigTone:
        case graph_eTrace_DigToneWithError:
          items[i].type = glow_eType_ToneOrColor;
          break;
        default:
          items[i].type = glow_eType_Color;
      }
    }
    else
      items[i].type = grow_info_p->type;
    items[i].size = grow_info_p->size;
    items[i].minlimit = 0;
    items[i].maxlimit = 0;
    if ( strcmp( grow_info_p->name, "Extern") == 0)
      items[i].noedit = 0;
    else
      items[i].noedit = 1;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }

  attr = new Attr( parent_wid, this, object, (attr_sItem *)&items, i);
  attr->client_data = (void *)grow_info;
  attr->close_cb = subgraphs_attr_close_cb;
  attr->redraw_cb = subgraphs_attr_redraw_cb;

  // Insert in attrlist
  attrlist_p = (subgraphs_tAttr) calloc( 1, sizeof( *attrlist_p));
  attrlist_p->attrctx = attr;
  attrlist_p->next = attrlist;
  attrlist = attrlist_p;
  return 1;
}

static void subgraphs_attr_close_cb( Attr *attrctx)
{
  SubGraphs *subgraphs = (SubGraphs *) attrctx->parent_ctx;
  subgraphs_tAttr attrlist_p, prev_p;

  printf( "Here in attr close\n");
//  if ( attrctx->object)
//    grow_FreeObjectAttrInfo( (grow_sAttrInfo *)attrctx->client_data);
//  else
//    grow_FreeSubGraphAttrInfo( (grow_sAttrInfo *)attrctx->client_data);

  // Remove from attrlist
  attrlist_p = subgraphs->attrlist;
  prev_p = NULL;
  while( attrlist_p)
  {
    if ( attrlist_p->attrctx == attrctx)
    {
      if ( prev_p)
        prev_p->next = attrlist_p->next;
      else
        subgraphs->attrlist = attrlist_p->next;
      free( (char *)attrlist_p);
      break;
    }
    prev_p = attrlist_p;
    attrlist_p = attrlist_p->next;    
  }

  delete attrctx;
}

static void subgraphs_attr_redraw_cb( Attr *attrctx)
{
  printf( "Here in attr redraw\n");
}


static void subgraphs_trace_scan( SubGraphs *subgraphs)
{
  int time = 200;

  if ( subgraphs->trace_started)
  {
    brow_TraceScan( subgraphs->brow->ctx);

    subgraphs->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(subgraphs->brow_widget) , time,
	(XtTimerCallbackProc)subgraphs_trace_scan, subgraphs);
  }
}

//void SubGraphs::force_trace_scan()
//{
//  if ( trace_started)
//    brow_TraceScan( brow->ctx);
//}

static int subgraphs_trace_scan_bc( brow_tObject object, void *p)
{
  SubGraphBaseItem	*base_item;
  char		buf[20];
  int		len;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case subgraphs_eItemType_SubGraph:
    {
      ItemSubGraph	*item;

      item = (ItemSubGraph *)base_item;
      if ( !item->first_scan)
      {
        if ( *item->extern_p == item->old_extern)
          // No change since last time
          return 1;
      }
      else
        item->first_scan = 0;

      if ( *item->extern_p)
        len = sprintf( buf, "Extern");
      else
        len = sprintf( buf, "Intern");
      brow_SetAnnotation( object, 1, buf, len);
      item->old_extern = *item->extern_p;
      break;
    }
    default:
      ;
  }
  return 1;
}

static int subgraphs_trace_connect_bc( brow_tObject object, char *name, char *attr, 
	flow_eTraceType type, /* flow_eDrawType color, */ void **p)
{
  SubGraphBaseItem 	*base_item;

  if ( strcmp(name,"") == 0)
    return 1;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    case subgraphs_eItemType_SubGraph:
    {
      ItemSubGraph	*item;

      item = (ItemSubGraph *) base_item;
      *p = item->extern_p;
      break;
    }
    default:
      ;
  }      
  return 1;
}

static int subgraphs_trace_disconnect_bc( brow_tObject object)
{
  SubGraphBaseItem 		*base_item;

  brow_GetUserData( object, (void **)&base_item);
  switch( base_item->type)
  {
    default:
      ;
  }
  return 1;
}


