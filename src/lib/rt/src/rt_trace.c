/* rt_trace.c -- trace in runtime environment

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if !defined OS_ELN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined OS_LINUX
#include <sys/stat.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_privilege.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/DialogS.h>

#include "co_cdh.h"
#include "co_api.h"
#include "co_mrm_util.h"
#include "rt_gdh.h"
#include "flow.h"
#include "flow_api.h"
#include "flow_x.h"
#include "flow_widget.h"
#include "rt_trace.h"

#define GOEN_F_GRID 0.05

// Until xtt_menu.h i unavailable...
#define xmenu_mUtility_Trace     (1 << 1)
#define xmenu_mUtility_Simulate  (1 << 2)
#define xmenu_eItemType_Object   1

static int trace_start( tra_tCtx tractx);
static int trace_stop( tra_tCtx tractx);
#if 0
static pwr_tStatus	trace_aanalyse_set_value(
    tra_tCtx	tractx,
    char	*valuestr
);
#endif
static void trace_changevalue (
    tra_tCtx	    tractx,
    flow_tNode	    fnode
);
static int trace_get_selected_node( tra_tCtx tractx, flow_tObject *node);
static int trace_get_objid( tra_tCtx tractx, flow_tObject node,
		pwr_tObjid *objid);
static void trace_close_cb( tra_tCtx child_tractx);
pwr_tStatus trace_viewsetup( tra_tCtx tractx);
pwr_tStatus trace_simsetup( tra_tCtx tractx);
pwr_tStatus trace_trasetup( tra_tCtx tractx);
static char	*trace_IdToStr(  pwr_tObjid 	objid);

static void trace_get_filename( pwr_tObjid window_objid, char *filename)
{
#if defined OS_VMS
  sprintf( filename, "pwrp_load:pwr_%s.flw", trace_IdToStr( window_objid));
#else
  {
    char *s;

    if ( (s = getenv( "pwrp_load")) == NULL)
    {
      printf( "** pwr_load is not defined\n");
      return;
    }
    sprintf( filename, "%s/pwr_%s.flw", s, trace_IdToStr( window_objid));
  }
#endif
}

static int trace_connect_bc( flow_tObject object, char *name, char *attr, 
	flow_eTraceType type, void **p)
{
  char		attr_str[160];
  int		size;
  pwr_tSubid	*subid_p, subid;
  int		sts;

/*  printf( "Connecting %s.%s\n", name, attr);  */

  if ( strcmp( name, "") == 0 || strcmp( attr, "") == 0)
    return 1;

  switch( type)
  {
    case flow_eTraceType_Boolean:
      size = sizeof(pwr_tBoolean);
      break;
    case flow_eTraceType_Int32:
      size = sizeof(pwr_tInt32);
      break;
    case flow_eTraceType_Float32:
      size = sizeof(pwr_tFloat32);
      break;
    default:
      size = sizeof(pwr_tInt32);
  }

  strcpy( attr_str, name);
  strcat( attr_str, ".");
  strcat( attr_str, attr);  

  if ( flow_GetObjectType( object) == flow_eObjectType_Node)
  {
    sts = gdh_RefObjectInfo( attr_str, (void *)p, &subid, size);
    if ( EVEN(sts)) return sts;
      
    subid_p = (pwr_tSubid *) XtMalloc( sizeof(pwr_tSubid));
    *subid_p = subid;
    flow_SetUserData( object, (void *) subid_p);
  }
  return 1;
}

static int trace_disconnect_bc( flow_tObject object)
{
  pwr_tSubid	*subid_p;
  int 		sts;
  char          name[120];
  char          attr[32];
  flow_eTraceType type;

  if ( flow_GetObjectType( object) == flow_eObjectType_Node) {

    flow_GetTraceAttr( object, name, attr, &type);
    if ( !( strcmp( name, "") == 0 || strcmp( attr, "") == 0)) {
      flow_GetUserData( object, (void **) &subid_p);
      sts = gdh_UnrefObjectInfo( *subid_p);
      XtFree( (char *) subid_p);
    }
  }
  return 1;
}


static void trace_scan( tra_tCtx tractx)
{
  if ( tractx->trace_started)
  {
    flow_TraceScan( tractx->flow_ctx);

    tractx->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(tractx->flow_widget), tractx->scan_time * 1000,
	(XtTimerCallbackProc)trace_scan, tractx);
  }
}


static int trace_init_flow( FlowCtx *ctx, void *client_data)
{
  return 1;
}


static void tra_create_form( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->form = w;
}

static void tra_create_menu( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->menu = w;
}

static void tra_activate_close( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  if ( tractx->close_cb)
    (tractx->close_cb)(tractx);
}

static void tra_activate_print( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tCtx ctx = (flow_tCtx)tractx->flow_ctx;
  flow_tObject 	*list;
  int		cnt;
  double	ll_x, ll_y, ur_x, ur_y;
  flow_tNode	n;

  /* Get selected object */
  flow_GetSelectList( ctx, &list, &cnt);
  if ( cnt > 0 && flow_GetObjectType( *list) == flow_eObjectType_Node)  
  {
    n = *list;
    flow_MeasureNode( n, &ll_x, &ll_y, &ur_x, &ur_y);
    flow_PrintRegion( ctx, ll_x, ll_y, ur_x, ur_y, "test.ps");
  }
  else
    printf("No such node\n");
}

static void tra_activate_savetrace( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  char		filename[120];

  trace_get_filename( tractx->objid, filename);
  strcat( filename, "t");

  flow_SaveTrace( tractx->flow_ctx, filename);
}

static void tra_activate_restoretrace( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  char		filename[120];

  if ( !tractx->trace_started)
    return;

  trace_get_filename( tractx->objid, filename);
  strcat( filename, "t");
  flow_OpenTrace( tractx->flow_ctx, filename);
}

static void tra_activate_display_object( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tObject 	node;
  int		sts;
  pwr_tObjid	objid;

  sts = trace_get_selected_node( tractx, &node);
  if (EVEN(sts)) return;

  sts = trace_get_objid( tractx, node, &objid);
  if (EVEN(sts)) return;

  if ( tractx->call_method_cb)
  {
    pwr_sAttrRef attrref;
              
    memset( &attrref, 0, sizeof(attrref));
    attrref.Objid = objid;
    (tractx->call_method_cb)(tractx->cp.parent_ctx,
			     "$Object-RtNavigator",
			     "$Object-RtNavigatorFilter",
			     attrref, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_Trace, NULL);
  }
}

static void tra_activate_collect_insert( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tObject 	node;
  int		sts;
  pwr_tObjid	objid;

  sts = trace_get_selected_node( tractx, &node);
  if (EVEN(sts)) return;

  sts = trace_get_objid( tractx, node, &objid);
  if (EVEN(sts)) return;

  if ( tractx->call_method_cb)
  {
    pwr_sAttrRef attrref;
              
    memset( &attrref, 0, sizeof(attrref));
    attrref.Objid = objid;
    (tractx->call_method_cb)(tractx->cp.parent_ctx,
			     "$Object-Collect",
			     "$Object-CollectFilter",
			     attrref, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_Trace, NULL);
  }
}

static void tra_activate_open_object( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tObject 	node;
  int		sts;
  pwr_tObjid	objid;

  sts = trace_get_selected_node( tractx, &node);
  if (EVEN(sts)) return;

  sts = trace_get_objid( tractx, node, &objid);
  if (EVEN(sts)) return;

  if ( tractx->call_method_cb)
  {
    pwr_sAttrRef attrref;
              
    memset( &attrref, 0, sizeof(attrref));
    attrref.Objid = objid;
    (tractx->call_method_cb)(tractx->cp.parent_ctx,
			     "$Object-OpenObject",
			     NULL,
			     attrref, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_Trace, NULL);
  }
}

static void tra_activate_show_cross( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tObject 	node;
  int		sts;
  pwr_tObjid	objid;

  sts = trace_get_selected_node( tractx, &node);
  if (EVEN(sts)) return;

  sts = trace_get_objid( tractx, node, &objid);
  if (EVEN(sts)) return;

  if ( tractx->call_method_cb)
  {
    pwr_sAttrRef attrref;
              
    memset( &attrref, 0, sizeof(attrref));
    attrref.Objid = objid;
    (tractx->call_method_cb)(tractx->cp.parent_ctx,
			     "$Object-OpenCrossref",
			     "$Object-OpenCrossrefFilter",
			     attrref, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_Trace, NULL);
  }
}

static void tra_activate_open_classgraph( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_tObject 	node;
  int		sts;
  pwr_tObjid	objid;

  sts = trace_get_selected_node( tractx, &node);
  if (EVEN(sts)) return;

  sts = trace_get_objid( tractx, node, &objid);
  if (EVEN(sts)) return;

  if ( tractx->call_method_cb)
  {
    pwr_sAttrRef attrref;
              
    memset( &attrref, 0, sizeof(attrref));
    attrref.Objid = objid;
    (tractx->call_method_cb)(tractx->cp.parent_ctx,
			     "$Object-OpenClassGraph",
			     "$Object-OpenClassGraphFilter",
			     attrref, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_Trace, NULL);
  }
}

static void tra_activate_trace( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  trace_trasetup( tractx);
  if ( !tractx->trace_started)
  {
    trace_start( tractx);
  }
}

static void tra_activate_simulate( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  trace_simsetup( tractx);
  if ( !tractx->trace_started)
  {
    trace_start( tractx);
  }
}

static void tra_activate_view( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  if ( tractx->trace_started)
  {
    trace_stop( tractx);
    trace_viewsetup( tractx);
  }
}

static void tra_activate_zoomin( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_Zoom( tractx->flow_ctx, 1.0/0.7);
}

static void tra_activate_zoomout( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_Zoom( tractx->flow_ctx, 0.7);
}

static void tra_activate_zoomreset( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  flow_UnZoom( tractx->flow_ctx);
}

static void tra_activate_scantime1( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->scan_time = 0.5;
}

static void tra_activate_scantime2( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->scan_time = 0.2;
}

static void tra_activate_scantime3( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->scan_time = 0.1;
}

static void tra_activate_scantime4( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->scan_time = 0.05;
}

static void tra_activate_scantime5( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  tractx->scan_time = 0.02;
}

static void tra_activate_help( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  if ( tractx->help_cb)
    (tractx->help_cb)(tractx, "trace");
}

static void tra_activate_helpplc( Widget w, tra_tCtx tractx, XmAnyCallbackStruct *data)
{
  char	name[120];
  int 	sts;

  sts = gdh_ObjidToName( tractx->objid, name, sizeof(name), cdh_mNName); 
  if (EVEN(sts)) return;

  if ( tractx->help_cb)
    (tractx->help_cb)(tractx, name);
}


/*************************************************************************
*
* Name:		int	trace_flow_cb()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Callback from flow.
**************************************************************************/
static int trace_flow_cb( FlowCtx *ctx, flow_tEvent event)
{
  char		name[80];
  tra_tCtx	tractx;
  static int	idx = 0;

  flow_GetCtxUserData( ctx, (void **)&tractx);

  if ( event->any.type == flow_eEventType_CreateCon)
  {
    if ( flow_GetNodeGroup( event->con_create.source_object) == 
		flow_eNodeGroup_Trace)
      return 1;

    if ( tractx->trace_started)
    {
      char			object_str[120];
      char			attr_str[80];
      flow_eTraceType		trace_type;
      flow_tNode		n1;
      flow_tCon			c1;
      int			sts;
      flow_eDirection 		direction;
      double			x, y;

      /* Create a trace object */
      sprintf( name, "Trace%d", idx++);

      /* Connect only output points */
      sts = flow_GetConPoint( event->con_create.source_object, 
		event->con_create.source_conpoint, &x, &y, &direction); 
      if ( EVEN(sts)) return 1;

      switch( direction)
      {
        case flow_eDirection_Center:
	case flow_eDirection_Left:
	case flow_eDirection_Up:
	case flow_eDirection_Down:
          return 1;
        default:
          ;
      }

      /* Find the trace attributes */
      flow_GetTraceAttr( event->con_create.source_object, object_str, attr_str, 
		&trace_type);

      /* Get attribute from connection point */
      sts = flow_GetConPointTraceAttr( event->con_create.source_object,
		event->con_create.source_conpoint, attr_str, &trace_type);

      if ( strcmp( attr_str, "") == 0)
        return 1;

      flow_CreateNode( ctx, name, tractx->trace_analyse_nc, event->object.x, event->object.y, NULL, &n1);
      flow_SetTraceAttr( n1, object_str, attr_str, trace_type);

      flow_CreateCon( ctx, name, tractx->trace_con_cc, 
	  	event->con_create.source_object, n1,
	  	event->con_create.source_conpoint, 0,
	  	NULL, &c1, 0, NULL, NULL);
    }
  }
  switch ( event->event)
  {
    case flow_eEvent_Init:
      break;
    case flow_eEvent_MB3Down:
    {
      flow_SetClickSensitivity( ctx, flow_mSensitivity_MB3Press);
      break;
    }
    case flow_eEvent_MB1Click:
      /* Select */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          if ( flow_FindSelectedObject( ctx, event->object.object))
          {
            flow_SelectClear( ctx);
          }
          else
          {
	    if ( flow_GetNodeGroup( event->object.object) == flow_eNodeGroup_Document)
              break;
            flow_SelectClear( ctx);
            flow_SetInverse( event->object.object, 1);
            flow_SelectInsert( ctx, event->object.object);
          }
          break;
        default:
          flow_SelectClear( ctx);
      }
      break;
    case flow_eEvent_MB2DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  if ( flow_GetNodeGroup( event->object.object) == 
		flow_eNodeGroup_Trace)
	  {
	    flow_DeleteNodeCons( event->object.object);
	    flow_DeleteNode( event->object.object);
	  }
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1Press:
    {
      /* Object moved */
      break;
    }
    case flow_eEvent_MB3Press:
    {
      char		object_str[120];
      char		attr_str[80];
      flow_eTraceType	trace_type;
      pwr_tObjid	objid;
      pwr_sAttrRef      attrref;
      int		sts;
      Widget            popup;
      unsigned int      utility;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  if ( flow_GetNodeGroup( event->object.object) != 
		flow_eNodeGroup_Trace)
          {
            flow_GetTraceAttr( event->object.object, object_str, attr_str,
			&trace_type);

            sts = gdh_NameToObjid( object_str, &objid);
            if ( EVEN(sts)) return 1;

              memset( &attrref, 0, sizeof(attrref));
              attrref.Objid = objid;

            if ( tractx->popup_menu_cb)
            {
              // Display popup menu
              utility = xmenu_mUtility_Trace;
              (tractx->popup_menu_cb)(tractx->cp.parent_ctx, attrref, 
				      xmenu_eItemType_Object,
				      utility, NULL, &popup);
              if ( !popup)
                break;

              mrm_PositionPopup( popup, tractx->flow_widget, 
			       event->any.x_pixel + 8, event->any.y_pixel);
              XtManageChild(popup);
            }
	  }
          break;
        default:
          ;
      }

      /* Object moved */
      break;
    }
    case flow_eEvent_MB1DoubleClick:
    {
      /* Open attribute editor */
      char		object_str[120];
      char		attr_str[80];
      flow_eTraceType	trace_type;
      pwr_tObjid	objid;
      int		sts;

      /* Display object */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  if ( flow_GetNodeGroup( event->object.object) != 
		flow_eNodeGroup_Trace)
          {
            flow_GetTraceAttr( event->object.object, object_str, attr_str, 
			&trace_type);

            sts = gdh_NameToObjid( object_str, &objid);
            if ( EVEN(sts)) return 1;

            if ( tractx->call_method_cb)
            {
              // Display crossreferences
              unsigned long utility = xmenu_mUtility_Trace;
              pwr_sAttrRef attrref;
              
              memset( &attrref, 0, sizeof(attrref));
              attrref.Objid = objid;
              (tractx->call_method_cb)(tractx->cp.parent_ctx, 
				       "$Object-OpenCrossref",
				       "$Object-OpenCrossrefFilter",
				       attrref, 
				       xmenu_eItemType_Object,
				       utility, NULL);
            }
            //if ( tractx->display_object_cb)
            //{
	      // Display the object in the parent context
            //  (tractx->display_object_cb)(tractx->cp.parent_ctx, objid);
            //}
	  }
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_MB1DoubleClickShift:
    {
      char			name[120];
      char			object_name[120];
      pwr_tObjid		objid;
      int			sts;
      tra_tCtx			new_tractx;
      trace_t_node 		*node;

      /* Open subwindow */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  if ( flow_GetNodeGroup( event->object.object) != 
		flow_eNodeGroup_Trace)
          {
            sts = gdh_ObjidToName( tractx->objid, name, sizeof(name), cdh_mNName); 
            if (EVEN(sts)) return 1;

            flow_GetObjectName( event->object.object, object_name);
	    strcat( name, "-");
	    strcat( name, object_name);

            sts = gdh_NameToObjid( name, &objid);
            if ( EVEN(sts)) return 1;

            /* Should check for ordercond or orderact window... */
            sts = gdh_GetChild( objid, &objid);
            if ( EVEN(sts)) return 1;

            if ( tractx->subwindow_cb)
            {
	      // The parent context will start the subwindow
              (tractx->subwindow_cb)(tractx->cp.parent_ctx, objid);
            }
            else
            {
              new_tractx = trace_new( tractx, tractx->flow_widget, objid,
			trace_close_cb, tractx->help_cb, NULL, 
			tractx->display_object_cb, tractx->collect_insert_cb,
			tractx->is_authorized_cb);

              if (tractx)
              {
                node = tractx->trace_list;
                tractx->trace_list = (trace_t_node *) XtMalloc(sizeof(trace_t_node));
                tractx->trace_list->Next = node;
                tractx->trace_list->tractx = new_tractx;
              }     
            }
          }
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_MB1ClickCtrl:
    {
      break;
    }
    case flow_eEvent_MB1DoubleClickShiftCtrl:
    {
      trace_changevalue( tractx, event->object.object);
      break;
    }
    case flow_eEvent_SelectClear:
      flow_ResetSelectInverse( ctx);
      break;
    default:
      ;
  }
  return 1;
}

static void trace_close_cb( tra_tCtx child_tractx)
{
  tra_tCtx tractx;
  trace_t_node *node, *fnode;

  tractx = (tra_tCtx) child_tractx->cp.parent_ctx;

  node = tractx->trace_list;
  if (node && node->tractx == child_tractx)
  {
    fnode = node;
    tractx->trace_list = node->Next;
    XtFree((char *)fnode);
  }
  else
  {
    for (; node; node = node->Next)
    {
      if (node->Next && node->Next->tractx == child_tractx)
      {
        fnode = node->Next;
        node->Next = fnode->Next;
        XtFree((char *)fnode);
        break;
      }
    }
  }

  trace_del( child_tractx);
}

static int trace_get_objid( tra_tCtx tractx, flow_tObject node,
		pwr_tObjid *objid)
{
  char			name[120];
  char			object_name[120];
  int			sts;
  char			attr_str[80];
  flow_eTraceType	trace_type;

  /* Try flow node name */
  sts = gdh_ObjidToName( tractx->objid, name, sizeof(name), cdh_mNName); 
  if (EVEN(sts)) return 1;

  flow_GetObjectName( node, object_name);
  strcat( name, "-");
  strcat( name, object_name);

  sts = gdh_NameToObjid( name, objid);
  if ( EVEN(sts))
  {
    /* Try trace object */
    flow_GetTraceAttr( node, object_name, attr_str, &trace_type);

    sts = gdh_NameToObjid( object_name, objid);
    if ( EVEN(sts)) return sts;
  }
  return 1;
}

static int trace_get_selected_node( tra_tCtx tractx, flow_tObject *node)
{
  flow_tNode	*list;
  int		count;

  flow_GetSelectList( tractx->flow_ctx, &list, &count);
  if ( count == 0)
    return 0;
  *node = *list;
    return 1;
}
pwr_tStatus trace_viewsetup( tra_tCtx tractx) 
{
  flow_tCtx ctx = tractx->flow_ctx;

  flow_DisableEventAll( ctx);

  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	trace_flow_cb);

  return 1;
}

pwr_tStatus trace_simsetup( tra_tCtx tractx) 
{
  flow_tCtx ctx = tractx->flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_MoveNode, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Press, flow_eEventType_CreateCon, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClick, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickCtrl, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	trace_flow_cb);
  return 1;
}

pwr_tStatus trace_trasetup( tra_tCtx tractx)
{
  flow_tCtx ctx = tractx->flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_MoveNode, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Press, flow_eEventType_CreateCon, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClick, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickCtrl, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	trace_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	trace_flow_cb);
  return 1;
}

static int trace_start( tra_tCtx tractx)
{
  int	  	sts;
  double 	f_width, f_height;

  if ( !tractx->trace_started)
  {

    flow_ResetHighlightAll( tractx->flow_ctx);
    flow_SelectClear( tractx->flow_ctx);
    sts = flow_TraceInit( tractx->flow_ctx, trace_connect_bc, 
		trace_disconnect_bc, NULL);
    if ( EVEN(sts))
      return sts;
    tractx->trace_started = 1;

    trace_scan( tractx);

    /* Create node and con classes for trace */
    if ( !tractx->trace_analyse_nc)
    {
      f_width = 4*GOEN_F_GRID;
      f_height = GOEN_F_GRID;
      flow_CreateNodeClass( tractx->flow_ctx, "TraceNode", flow_eNodeGroup_Trace, 
		&tractx->trace_analyse_nc);
      flow_AddRect( tractx->trace_analyse_nc, 0, 0, f_width, f_height, 
		flow_eDrawType_Line, 1, flow_mDisplayLevel_1);
      flow_AddAnnot( tractx->trace_analyse_nc, f_width/8, 0.7*f_height, 0,
		flow_eDrawType_TextHelvetica, 4, flow_eAnnotType_OneLine,
		flow_mDisplayLevel_1);
      flow_AddConPoint( tractx->trace_analyse_nc, 0, 0.5*f_height, 0, 
		flow_eDirection_Left);
      flow_AddConPoint( tractx->trace_analyse_nc, f_width, 0.5*f_height, 1,
		flow_eDirection_Right);

      flow_CreateConClass( tractx->flow_ctx, "TraceCon", 
		flow_eConType_Straight, flow_eCorner_Right,
		flow_eDrawType_Line, 1, 0, 0, 0, flow_eConGroup_Trace,
		&tractx->trace_con_cc);	  
    }
  }
  return 1;
}

static int trace_stop( tra_tCtx tractx)
{

  if ( tractx->trace_started)
  {
    flow_TraceClose( tractx->flow_ctx);
    flow_ResetHighlightAll( tractx->flow_ctx);
    flow_SelectClear( tractx->flow_ctx);
    flow_RemoveTraceObjects( tractx->flow_ctx);
    tractx->trace_started = 0;
    XtRemoveTimeOut( tractx->trace_timerid);
  }
  return 1;
}


static void trace_changevalue (
    tra_tCtx	    tractx,
    flow_tNode	    fnode
)
{
  pwr_tStatus 		sts;
  char			name[160];
  pwr_tBoolean		value;
  char			object_str[120];
  char			attr_str[80];
  flow_eTraceType	trace_type;

  if ( tractx->is_authorized_cb) {
    if ( !(tractx->is_authorized_cb)(tractx->cp.parent_ctx, 
	      pwr_mAccess_RtWrite | pwr_mAccess_System))
      return;
  }

  if ( flow_GetNodeGroup( fnode) == flow_eNodeGroup_Trace)
  {
    tractx->trace_changenode = fnode;

    /* Get a value */
/*
    foe_get_textinput( tractx, "Enter value : ", &trace_aanalyse_set_value);
*/
    return;
  }
  else
  {	    
    /* Toggle the value, start to get the current value */
    flow_GetTraceAttr( fnode, object_str, attr_str, &trace_type);
    strcpy( name, object_str);
    strcat( name, ".");
    strcat( name, attr_str);
    switch ( trace_type)
    {
      case flow_eTraceType_Boolean:
        sts = gdh_GetObjectInfo( name, &value, sizeof(value)); 
        if (EVEN(sts))
        {
          return;
        }

        /* Toggle the value */
        if ( value == 0)
          value = 1;
        else
          value = 0;

        sts = gdh_SetObjectInfo( name, &value, sizeof(value));
        if (EVEN(sts))
        {
          return;
        }
        break;
      default:
        ;
    }
  }
}

/*************************************************************************
*
* Name:		trace_aanalyse_set_value ()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
**************************************************************************/

#if 0
static pwr_tStatus	trace_aanalyse_set_value(
    tra_tCtx	tractx,
    char	*valuestr
)
{
  pwr_tStatus 		sts;
  char			name[160];
  pwr_tBoolean		boolean_value;
  pwr_tFloat32		float_value;
  char			object_str[120];
  char			attr_str[80];
  flow_eTraceType	trace_type;

  flow_GetTraceAttr( tractx->trace_changenode, object_str, attr_str, &trace_type);
  strcpy( name, object_str);
  strcat( name, ".");
  strcat( name, attr_str);
  switch ( trace_type)
  {
    case flow_eTraceType_Boolean:
      /* Convert to Boolean */
      if ( sscanf( valuestr, "%d", &boolean_value) != 1)
      {
        return 0;
      }

      sts = gdh_SetObjectInfo( name, &boolean_value, sizeof(boolean_value));
      if (EVEN(sts))
      {
        return 1;
       }
       break;
    case flow_eTraceType_Float32:
      /* Convert to float */
      if ( sscanf( valuestr, "%f", &float_value) != 1)
      {
        return 0;
      }

      sts = gdh_SetObjectInfo( name, &float_value, sizeof(float_value));
      if (EVEN(sts))
      {
        return 1;
       }
       break;
     default:
       ;
  }
  return 1;
}
#endif

static char	*trace_IdToStr(
  pwr_tObjid 	objid
)
{
	static char	str[80];
	unsigned char	volid[4];

	memcpy( &volid, &objid.vid, sizeof(volid));
	sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u_%8.8x",
		volid[3], volid[2], volid[1], volid[0], objid.oix);
	return str;
}

void	trace_del( tra_tCtx	tractx)
{
  trace_t_node *node, *fnode;

  trace_stop( tractx);

  /* Delete all trace children windows */
  for (node = tractx->trace_list; node;)
  {
    trace_del( node->tractx);
    fnode = node->Next;
    XtFree((char *)node);
    node = fnode;
  }

  /* Destroy the widgets */
  XtDestroyWidget( tractx->toplevel);

  /* free the context */
  XtFree((char *) tractx);

}

int trace_search_object( 	tra_tCtx	tractx,
				char		*object_str)
{
  flow_tObject 	object;
  int		sts;

  sts = flow_FindByNameNoCase( tractx->flow_ctx, object_str, &object);
  if ( EVEN(sts))
     return sts;

  flow_CenterObject( tractx->flow_ctx, object);
  flow_SelectClear( tractx->flow_ctx);
  flow_SetInverse( object, 1);
  flow_SelectInsert( tractx->flow_ctx, object);
  return 1;
}

void trace_pop( tra_tCtx	tractx)
{
  XtPopup( tractx->toplevel, XtGrabNone);
/*
  XtUnmapWidget( tractx->toplevel);
  XtMapWidget( tractx->toplevel);
*/
}

void trace_swap( tra_tCtx tractx, int mode)
{
  pwr_tStatus sts;

  if ( mode == 0) {
    if ( tractx->trace_started) {
      flow_TraceClose( tractx->flow_ctx);
      XtRemoveTimeOut( tractx->trace_timerid);
    }
  }
  else {
    if ( tractx->trace_started) {
      int version = 0;
#if defined OS_LINUX
      {
	struct stat info;

	if ( stat( tractx->filename, &info) != -1)
	  version = info.st_ctime;    
      }
#endif
      if ( tractx->version != version) {
	flow_sAttributes attr;
	char tfile[200];
	char *s;

	// Temporary file to store trace objects
	
	strcpy( tfile, "/tmp/");
	if ( (s = strrchr( tractx->filename, '/')))
	  strcat( tfile, s+1);
	else
	  strcat( tfile, tractx->filename);

	flow_GetAttributes( tractx->flow_ctx, &attr);
	flow_SaveTrace( tractx->flow_ctx, tfile);
	tractx->version = version;
	tractx->trace_started = 0;
	flow_SetNodraw( tractx->flow_ctx);
	flow_DeleteAll( tractx->flow_ctx);
	flow_Open( tractx->flow_ctx, tractx->filename);
	flow_SetAttributes( tractx->flow_ctx, &attr, ~0);
	flow_Zoom( tractx->flow_ctx, 1);
	flow_ResetNodraw( tractx->flow_ctx);
	flow_Redraw( tractx->flow_ctx);
	trace_start( tractx);
	flow_OpenTrace( tractx->flow_ctx, tfile);
      }
      else {
	sts = flow_TraceInit( tractx->flow_ctx, trace_connect_bc, 
			      trace_disconnect_bc, NULL);
	if ( EVEN(sts))
	  return;
	trace_scan( tractx);
      }
    }
  }
}

tra_tCtx trace_new( 	void 		*parent_ctx, 
			Widget 		parent_wid, 
			pwr_tObjid	objid,
			void 		(*close_cb)(tra_tCtx),
			void 		(*help_cb)(tra_tCtx, char *),
			void 		(*subwindow_cb)(void *, pwr_tObjid),
			void 		(*display_object_cb)(void *, pwr_tObjid),
			void 		(*collect_insert_cb)(void *, pwr_tObjid),
			int             (*is_authorized_cb)(void *, unsigned int) 
)
{
  FlowWidget	flow_widget;
  char		uid_filename[120] = {"xtt_trace.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  unsigned long sts;
  tra_tCtx	tractx;
  char 		name[80];
  char		filename[120];
  int		i;
  FILE		*fp;
  pwr_tObjid	window_objid;
  pwr_tClassId	class;
  char   	title[100];
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	trace_widget;
  static MrmRegisterArg	reglist[] = {
        { "tra_ctx", 0 },
	{"tra_activate_close",(caddr_t)tra_activate_close },
	{"tra_activate_print",(caddr_t)tra_activate_print },
	{"tra_activate_savetrace",(caddr_t)tra_activate_savetrace },
	{"tra_activate_restoretrace",(caddr_t)tra_activate_restoretrace },
	{"tra_activate_trace",(caddr_t)tra_activate_trace },
	{"tra_activate_display_object",(caddr_t)tra_activate_display_object },
	{"tra_activate_open_object",(caddr_t)tra_activate_open_object },
	{"tra_activate_show_cross",(caddr_t)tra_activate_show_cross },
	{"tra_activate_open_classgraph",(caddr_t)tra_activate_open_classgraph },
	{"tra_activate_collect_insert",(caddr_t)tra_activate_collect_insert },
	{"tra_activate_view",(caddr_t)tra_activate_view },
	{"tra_activate_simulate",(caddr_t)tra_activate_simulate },
	{"tra_activate_zoomin",(caddr_t)tra_activate_zoomin },
	{"tra_activate_zoomout",(caddr_t)tra_activate_zoomout },
	{"tra_activate_zoomreset",(caddr_t)tra_activate_zoomreset },
	{"tra_activate_scantime1",(caddr_t)tra_activate_scantime1 },
	{"tra_activate_scantime2",(caddr_t)tra_activate_scantime2 },
	{"tra_activate_scantime3",(caddr_t)tra_activate_scantime3 },
	{"tra_activate_scantime4",(caddr_t)tra_activate_scantime4 },
	{"tra_activate_scantime5",(caddr_t)tra_activate_scantime5 },
	{"tra_activate_help",(caddr_t)tra_activate_help },
	{"tra_activate_helpplc",(caddr_t)tra_activate_helpplc },
	{"tra_create_form",(caddr_t)tra_create_form },
	{"tra_create_menu",(caddr_t)tra_create_menu }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  lng_get_uid( uid_filename, uid_filename);

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName); 
  if (EVEN(sts)) return NULL;
  strcpy( title, "Trace ");
  strcat( title, name);

  /* Find plcwindow */
  sts = gdh_GetObjectClass( objid, &class);
  if ( EVEN(sts)) return NULL;

  if ( !(class == pwr_cClass_windowplc ||
         class == pwr_cClass_windowcond ||
         class == pwr_cClass_windoworderact ||
         class == pwr_cClass_windowsubstep ))
  {

    sts = gdh_GetChild( objid, &window_objid);
    if ( EVEN(sts)) return NULL;
  }
  else
    window_objid = objid; 

  sts = gdh_GetObjectClass( window_objid, &class);
  if ( EVEN(sts)) return NULL;

  if ( !(class == pwr_cClass_windowplc ||
         class == pwr_cClass_windowcond ||
         class == pwr_cClass_windoworderact ||
         class == pwr_cClass_windowsubstep ))
    return NULL;

  trace_get_filename( window_objid, filename);

  fp =  fopen( filename, "r");
  if ( !fp)
    return NULL;
  fclose( fp);

  /* Create object context */
  tractx = (tra_tCtx) XtCalloc( 1, sizeof(*tractx));
  tractx->cp.parent_wid = parent_wid;
  tractx->cp.parent_ctx = parent_ctx;
  tractx->objid = window_objid;
  tractx->close_cb = close_cb;
  tractx->help_cb = help_cb;
  tractx->subwindow_cb = subwindow_cb;
  tractx->display_object_cb = display_object_cb;
  tractx->collect_insert_cb = collect_insert_cb;
  tractx->is_authorized_cb = is_authorized_cb;
  tractx->scan_time = 0.5;
  reglist[0].value = (caddr_t) tractx;
 
  tractx->toplevel = XtCreatePopupShell( name, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) tractx);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "trace_window", tractx->toplevel,
			title, args, 1, &trace_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);


  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( tractx->toplevel ,args,i);
    
  XtManageChild( trace_widget);

  i = 0;
/*
  XtSetArg(args[i],XmNwidth,790);i++;
  XtSetArg(args[i],XmNheight,560);i++;
*/
  XtSetArg( args[i], XmNtopAttachment, XmATTACH_WIDGET);i++;
  XtSetArg( args[i], XmNtopWidget, tractx->menu);i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  tractx->flow_widget = FlowCreate( tractx->form, "Flow window", args, i, 
	trace_init_flow, (void *)tractx);

  XtManageChild( (Widget) tractx->flow_widget);
/*
  XtRealizeWidget(tractx->toplevel);
*/
  XtPopup( tractx->toplevel, XtGrabNone);

  flow_widget = (FlowWidget) tractx->flow_widget;
  tractx->flow_ctx = (flow_tCtx)flow_widget->flow.flow_ctx;
  flow_SetCtxUserData( tractx->flow_ctx, tractx);

  /* Create navigator popup */

  i = 0;
  XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
  XtSetArg(args[i],XmNallowResize, TRUE); i++;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNx,500);i++;
  XtSetArg(args[i],XmNy,500);i++;

  tractx->nav_shell = XmCreateDialogShell( tractx->flow_widget, "Navigator",
        args, i);
  XtManageChild( tractx->nav_shell);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  tractx->nav_widget = FlowCreateNav( tractx->nav_shell, "navigator",
        args, i, tractx->flow_widget);
  XtManageChild( tractx->nav_widget);
  XtRealizeWidget(tractx->nav_shell);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( tractx->toplevel, 
	(XtCallbackProc)tra_activate_close, tractx);

  trace_viewsetup( tractx);
  flow_Open( tractx->flow_ctx, filename);
  trace_trasetup( tractx);
  trace_start( tractx);

  strcpy( tractx->filename, filename);
#if defined OS_LINUX
  {
    struct stat info;

    if ( stat( tractx->filename, &info) != -1)
      tractx->version = info.st_ctime;    
  }
#endif

  return tractx;
}

#if 0
main(  int argc, char *argv[])
{
  Arg 		args[20];
  unsigned long sts;
  char 		title[80];
  int		i;
  XtAppContext  app_ctx;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	toplevel;
  Widget	mainwindow;
  char		name[] = "Proview/R Trace";
  static char	*db_filename_vec[] = {"pwr_exe:wb.uid"};
  static int	db_filename_num = (sizeof db_filename_vec/sizeof db_filename_vec[0]);
  tra_tCtx	ctx;

  sts = gdh_Init();
  if (EVEN(sts))
  {
    printf( "** Gdh init error\n");
    return sts;
  }
  
 /* Motif */
  MrmInitialize();
  DXmInitialize();

  toplevel = XtVaAppInitialize (
		      &app_ctx, 
		      "mainwindow",
		      NULL, 0, 
		      &argc, argv, 
		      fbr, 
		      XtNallowShellResize,  True,
		      XtNtitle, title,
		      XmNmappedWhenManaged, True,
		      NULL);

  sts = MrmOpenHierarchy(db_filename_num, db_filename_vec, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", db_filename_vec[0]);

  sts = MrmFetchWidgetOverride( s_DRMh, "mainwindow", toplevel,
			"svn_svn", args, 0, &mainwindow, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

  XtManageChild(mainwindow);

  trace_new( toplevel, "testdata.flw", &ctx);

  XtAppMainLoop( app_ctx);  

  return (0);
}

#endif
#endif
