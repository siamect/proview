/* rt_ge.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "co_dcli.h"
#include "ge_msg.h"
}
 
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_graph.h"
#include "ge_attr.h"
#include "ge_dyn.h"
#include "ge_msg.h"

extern "C" {
#include "co_mrm_util.h"
#include "co_msg.h"
#include "flow_x.h"
}
#include "co_lng.h"

#if defined OS_VMS
# pragma message disable (INTSIGNCHANGE)
#endif
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

typedef	struct {
	char		TypeStr[20];
	pwr_eType	Type;
	int		Size;
	} graph_sTypeStr;

static const    graph_sTypeStr	graph_type_table[] = {
    {"Boolean",	pwr_eType_Boolean,  sizeof(pwr_tBoolean)},
    {"Float32",	pwr_eType_Float32,  sizeof(pwr_tFloat32)},
    {"Float64",	pwr_eType_Float64,  sizeof(pwr_tFloat64)},
    {"Char",	pwr_eType_Char,	    sizeof(pwr_tChar)},
    {"Int8",	pwr_eType_Int8,	    sizeof(pwr_tInt8)},
    {"Int16",	pwr_eType_Int16,    sizeof(pwr_tInt16)},
    {"Int32",	pwr_eType_Int32,    sizeof(pwr_tInt32)},
    {"UInt8",	pwr_eType_UInt8,    sizeof(pwr_tUInt8)},
    {"UInt16",	pwr_eType_UInt16,   sizeof(pwr_tUInt16)},
    {"UInt32",	pwr_eType_UInt32,   sizeof(pwr_tUInt32)},
    {"Objid",	pwr_eType_Objid,    sizeof(pwr_tObjid)},
    {"Time",	pwr_eType_Time,     sizeof(pwr_tTime)},
    {"DeltaTime", pwr_eType_DeltaTime, sizeof(pwr_tDeltaTime)},
    {"AttrRef", pwr_eType_AttrRef,  sizeof(pwr_sAttrRef)},
    {"Status", pwr_eType_Status,  sizeof(pwr_tStatus)},
    {"NetStatus", pwr_eType_NetStatus,  sizeof(pwr_tNetStatus)}
    };

static char null_str[] = "";

static void graph_remove_space( char *out_str, char *in_str);
static int graph_init_grow_cb( GrowCtx *ctx, void *client_data);
static int graph_init_grow_base_cb( GlowCtx *fctx, void *client_data);
static void graph_attr_redraw_cb( Attr *attrctx);
static void graph_attr_close_cb( Attr *attrctx);
static void graph_attr_store_cb( void *g, grow_tObject object);
static int graph_attr_recall_cb( void *g, grow_tObject object, int idx, 
				 GeDyn **old);
static int graph_attr_set_data_cb( void *g, grow_tObject object, 
				 GeDyn *data);
static void graph_subgraphattr_redraw_cb( Attr *attrctx);
static void graph_subgraphattr_close_cb( Attr *attrctx);
static void graph_graphattr_redraw_cb( Attr *attrctx);
static void graph_graphattr_close_cb( Attr *attrctx);
static int graph_trace_disconnect_bc( grow_tObject object);
static int graph_trace_scan_bc( grow_tObject object, void *p);
static int graph_trace_connect_bc( grow_tObject object, 
	glow_sTraceData *trace_data);
static void graph_trace_scan( Graph *graph);
static int graph_trace_grow_cb( GlowCtx *ctx, glow_tEvent event);
static int graph_get_subgraph_info_cb( void *g, char *name, 
	attr_sItem **itemlist, int *itemlist_cnt);
static int graph_get_dyn_info_cb( void *g, GeDyn *dyn, 
	attr_sItem **itemlist, int *itemlist_cnt);
static int graph_reconfigure_attr_cb( void *g, grow_tObject object,
	attr_sItem **itemlist, int *itemlist_cnt, void **client_data);
static int graph_get_plant_select_cb( void *g, char *value);
static int graph_get_current_colors_cb( void *g, glow_eDrawType *fill_color, 
					glow_eDrawType *border_color, glow_eDrawType *text_color);
static int graph_grow_cb( GlowCtx *ctx, glow_tEvent event);


void Graph::message( pwr_tStatus sts)
{
  char msg[80];

  msg_GetMsg( sts, msg, sizeof(msg));
  message('I', msg);
}

void Graph::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);

  if ( scriptmode && strcmp( text, "") != 0)
    printf( "GE-%c-Message, %s\n", sev, text);
}

//
// Create the navigator widget
//
Graph::Graph(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status,
	char *xn_default_path,
	graph_eMode graph_mode,
	int scrollbar,
	int xn_gdh_init_done,
	char *xn_object_name,
	int xn_use_default_access,
	unsigned int xn_default_access) :
	attr_list( 1, NULL),
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	grow_cnt(0), ldhses(0),
	message_cb(NULL), get_current_subgraph_cb(NULL), close_cb(NULL),
	get_current_colors_cb(NULL), set_current_colors_cb(NULL),
	init_cb(NULL), cursor_motion_cb(NULL), change_text_cb(NULL),
	change_value_cb(NULL), confirm_cb(NULL), load_graph_cb(NULL),
	get_plant_select_cb(NULL), display_in_xnav_cb(NULL), 
	message_dialog_cb(NULL), is_authorized_cb(NULL), 
	traverse_focus_cb(NULL), set_focus_cb(NULL), get_ldhses_cb(NULL),
	get_current_objects_cb(NULL), popup_menu_cb(NULL), call_method_cb(NULL),
	linewidth(1), linetype(glow_eLineType_Solid), textsize(0), textbold(0), 
	border_color(1), fill_color(1), fill(0), border(1), shadow(0),
	grid_size_x(1), grid_size_y(1), con_type(glow_eConType_Routed),
	con_corner(glow_eCorner_Rounded),
	conpoint_direction(glow_eDirection_Center),
	trace_started(0), gdh_init_done(xn_gdh_init_done), arglist_cnt(0),
	corner_round_amount(0.5), mode(graph_mode), scan_time(0.5),
	fast_scan_time(0.5), animation_scan_time(0.5), closing_down(0),
        current_mb1_down(0),
	slow_scan_cnt(0), fast_scan_cnt(0), 
	displayed(0), ccm_func_registred(0), verify(0), 
	scriptmode(0), current_cmd_object(0), graph_object_data(0),
	graph_object_scan(0), graph_object_close(0), local_db(0),
	use_default_access(xn_use_default_access), 
	default_access(xn_default_access), keep_mode(false)
{
  strcpy( name, xn_name);
  strcpy( default_path, xn_default_path);
  memset( arglist_stack, 0, sizeof(arglist_stack));
  if ( xn_object_name)
    strcpy( object_name, xn_object_name);
  else
    strcpy( object_name, "");
  strcpy( filename, "");

  if ( scrollbar)
    form_widget = ScrolledGrowCreate( parent_wid, name, NULL, 0, 
	graph_init_grow_base_cb, this, (Widget *)&grow_widget);
  else
    grow_widget = form_widget = GrowCreate( parent_wid, name, NULL, 0, 
	graph_init_grow_base_cb, this);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  gbl.load_config( this);

  *status = 1;
}

//
//  Delete a nav context
//
Graph::~Graph()
{
  Attr		*attrctx;
  int		sts;

  closing_down = 1;

  if ( trace_started)
    close_trace( 0);

  sts = attr_list.get_first( (void **) &attrctx);
  while ( sts)
  {
    delete attrctx;
    attr_list.remove( (void *) attrctx);
    sts = attr_list.get_first( (void **) &attrctx);
  }

  localdb_free();

  for ( int i = 0; i < grow_cnt; i++)
    delete grow_stack[i];
  delete grow;
  XtDestroyWidget( form_widget);
}

GraphGrow::~GraphGrow()
{
  if ( this != ((Graph *)graph)->grow_stack[0] &&
       this != ((Graph *)graph)->grow)
    grow_DeleteSecondaryCtx( ctx);
}
//
//  Print
//
void Graph::print( char *filename)
{
  grow_Print( grow->ctx, filename);
}

int Graph::create_navigator( Widget parent)
{
  Arg 		args[20];
  int		i;

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,800);i++;
  nav_widget = GrowCreateNav( parent, "navigator",
        args, i, grow_widget);
  XtManageChild( nav_widget);

  return 1;
}

//
//  Rotate
//
void Graph::rotate( double angel)
{
  grow_RotateSelectedObjects( grow->ctx, angel, glow_eRotationPoint_Center);
}

//
//  Push
//
void Graph::push_select()
{
  grow_PushSelectedObjects( grow->ctx);
}

//
//  Pop
//
void Graph::pop_select()
{
  grow_PopSelectedObjects( grow->ctx);
}

void Graph::set_move_restriction( glow_eMoveRestriction restriction)
{
  grow_SetMoveRestrictions( grow->ctx, restriction, 0, 0, NULL);
}

void Graph::set_scale_equal( int equal)
{
  grow_SetScaleEqual( grow->ctx, equal);
}

void Graph::align_select( glow_eAlignDirection direction)
{
  grow_AlignSelect( grow->ctx, direction);
}

void Graph::equidistance_select( glow_eAlignDirection direction)
{
  grow_EquiDistanceSelect( grow->ctx, direction);
}

void Graph::set_default_layout()
{
  grow_SetDefaultLayout( grow->ctx);
}


//
//  Cut
//
void Graph::cut()
{
  grow_Cut( grow->ctx);
}

//
//  Copy
//
void Graph::copy()
{
  grow_Copy( grow->ctx);
}

//
//  Paste
//
void Graph::paste()
{
  grow_Paste( grow->ctx);
}

//
//  Store/Restore geometry
//
void Graph::store_geometry()
{
  grow_StoreGeometry( grow->ctx);
}
void Graph::restore_geometry()
{
  grow_RestoreGeometry( grow->ctx);
}

//
//  Get next subgraph
//
void Graph::get_next_subgraph( char *next)
{
  grow_GetNextSubgraph( grow->ctx, next);
}

//
//  Set next subgraph
//
void Graph::set_next_subgraph( char *next)
{
  grow_SetNextSubgraph( grow->ctx, next);
}

//
//  Get name
//
void Graph::get_name( char *name)
{
  grow_GetName( grow->ctx, name);
}

//
//  Set name
//
void Graph::set_name( char *name)
{
  grow_SetName( grow->ctx, name);
  grow_SetModified( grow->ctx, 1);
}
//
//  Get java name
//
int Graph::get_java_name( char *name)
{
  return grow_GetJavaName( grow->ctx, name);
}

//
//  Set java name
//
void Graph::set_java_name( char *name)
{
  char current_name[80];

  grow_GetJavaName( grow->ctx, current_name);
  if ( strcmp( current_name, name) != 0) {
    grow_SetJavaName( grow->ctx, name);
    grow_SetModified( grow->ctx, 1);
  }
}

//
//  Is java application
//
int Graph::is_javaapplication()
{
  return grow_IsJavaApplication( grow->ctx);
}

//
//  Is java applet
//
int Graph::is_javaapplet()
{
  return grow_IsJavaApplet( grow->ctx);
}

//
//  Set name
//
void Graph::clear_all()
{
  grow_New( grow->ctx);
}

//
//  Get filename
//
void Graph::get_filename( char *inname, char *outname)
{
  char fname[120];

  // Add default directory
  if ( !strchr( inname, ':') && !strchr( inname, '/'))
  {
    strcpy( fname, default_path);
    strcat( fname, inname);
  }
  else
    strcpy( fname, inname);
  dcli_get_defaultfilename( fname, fname, ".pwg");
  dcli_translate_filename( outname, fname);
}

//
//  Save
//
int Graph::save( char *filename)
{
  char fname[120];
  int grow_version, graph_version;

  grow_GetVersion( grow->ctx, &grow_version, &graph_version);
  if ( graph_version < 4000) {
    // Needs to be converted
    message( 'E', "Unable to save, graph needs conversion");
    return 0;
  }

  get_filename( filename, fname);
  grow_Save( grow->ctx, fname);
  grow_SetModified( grow->ctx, 0);

  return 1;
}

//
//  Save as subgraph
//
int Graph::save_subgraph( char *filename)
{
  char fname[120];
  int grow_version, graph_version;

  grow_GetVersion( grow->ctx, &grow_version, &graph_version);
  if ( graph_version < 4000) {
    // Needs to be converted
    message( 'E', "Unable to save, graph needs conversion");
    return 0;
  }

  get_filename( filename, fname);
  grow_SaveSubGraph( grow->ctx, fname);
  return 1;
}

//
//  Open
//
void Graph::open( char *filename)
{
  int sts;
  char fname[120];
  int grow_version, graph_version;

  if ( trace_started)
    close_trace( 0);

  grow->grow_setup();

  // Set temporary language translation on class graphs
  if ( strcmp( object_name, "") != 0)
    grow_EnableEvent( grow->ctx, glow_eEvent_Translate, 
		      glow_eEventType_CallBack, graph_grow_cb);

  if ( strcmp( filename, "_none_.pwg") != 0) {
    get_filename( filename, fname);
    sts = grow_Open( grow->ctx, fname);
    if ( EVEN(sts))
      message( 'E', "Unable to open file");
  }

  grow_SetModified( grow->ctx, 0);
  strcpy( this->filename, filename);

  grow_GetVersion( grow->ctx, &grow_version, &graph_version);
  if ( graph_version < 4000) {
    // Needs to be converted
    message( 'E', "Old version, graph needs conversion");
  }
}

//
//  Open subgraph
//
void Graph::open_subgraph( char *filename)
{
  char fname[120];

  dcli_translate_filename( fname, filename);
  grow_OpenSubGraph( grow->ctx, fname);
}

//
//  Set subgraph path
//
void Graph::set_subgraph_path( int path_cnt, char *path)
{
  grow_SetPath( grow->ctx, path_cnt, path);
}

//
//  Zoom
//
void Graph::zoom( double zoom_factor)
{
  grow_Zoom( grow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void Graph::unzoom()
{
  grow_UnZoom( grow->ctx);
}

//
//  Return to base zoom factor
//
void Graph::set_mode( grow_eMode mode, bool keep)
{
  if ( mode == grow_eMode_EditPolyLine)
  {
    grow_tObject 	*sel_list;
    int		sel_count;

    grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
    if ( !( sel_count == 1 && 
       grow_GetObjectType( *sel_list) == glow_eObjectType_GrowPolyLine))
    {
      message( 'E', "Select one polyline object");
      return;
    }
  }
  grow_SetMode( grow->ctx, mode);
  keep_mode = keep;
}

void Graph::flip( glow_eFlipDirection dir)
{
  grow_FlipSelectedObjects( grow->ctx, dir);
}

int Graph::get_default_size( int *width, int *height)
{
  return grow_GetDefaultWindowSize( grow->ctx, width, height);
}

int Graph::group_select( grow_tObject *object, char *last_group)
{
  grow_tObject group;
  char last_group_name[80];
  int sts;
  GeDyn *data;

  sts = grow_GroupSelect( grow->ctx, &group, last_group_name);
  if ( EVEN(sts)) return sts;

  GeDyn *dyn = new GeDyn( this);
  grow_SetUserData( group, (void *)dyn);

  grow_SetModified( grow->ctx, 1);

  if ( strcmp( last_group_name, "") != 0) {  
    // Try to recover dynamics
    sts = recall.get( &data, last_group_name);
    if ( ODD(sts)) {
      delete data;
      *object = group;
      strcpy( last_group, last_group_name);
      return GE__RECALLDATA_FOUND;
    }
  }
  return 1;
}


int Graph::set_recall_data( grow_tObject object, char *name)
{
  int sts;
  GeDyn *data;

  sts = recall.get( &data, name);
  if ( EVEN(sts)) return sts;

  grow_SetUserData( object, (void *)data);
  return 1;
}

int Graph::ungroup_select( int force)
{
  grow_tObject 	*sel_list;
  int		sel_count;
  int           i;
  GeDyn 	*dyn;
  char          name[80];

  if ( !force) {
    grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
    for ( i = 0; i < sel_count; i++) {
      if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup) {
	grow_GetUserData( sel_list[i], (void **)&dyn);
	if ( dyn->get_dyntype( sel_list[i]) || dyn->get_actiontype( sel_list[i]))
          return GE__GROUPDYNDATA;
      }
    }
  }
  else {
    grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
    for ( i = 0; i < sel_count; i++) {
      if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup) {
        grow_GetObjectName( sel_list[i], name);	
	grow_GetUserData( sel_list[i], (void **)&dyn);
	if ( dyn->get_dyntype( sel_list[i]) || dyn->get_actiontype( sel_list[i]))
          recall.insert( dyn, name, sel_list[i]);
      }
    }
  }
  grow_SetModified( grow->ctx, 1);

  return grow_UngroupSelect( grow->ctx);
}

void Graph::set_inputfocus( int focus)
{
  // Arg 		args[2];
  Pixel 	bg, fg;

  // if ( !displayed)
  //  return;

  XtVaGetValues( form_widget, XmNbackground, &bg, XmNforeground, &fg, NULL);
  if ( !focus)
  {
    // XtSetArg(args[0], XmNborderColor, bg);
    // XtSetValues( form_widget, args, 1);
  }
  else
  {
    if ( flow_IsViewable( grow_widget)) {
      XtCallAcceptFocus( grow_widget, CurrentTime);
      // XtSetArg(args[0], XmNborderColor, fg);
      // XtSetValues( form_widget, args, 1);
    }
  }
}

void Graph::set_gridsize( double gridsize)
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  grid_size_x = gridsize;
  grid_size_y = gridsize;
  mask = 0;
  mask |= grow_eAttr_grid_size_x;
  grow_attr.grid_size_y = grid_size_x;
  mask |= grow_eAttr_grid_size_y;
  grow_attr.grid_size_x = grid_size_y;
  grow_SetAttributes( grow->ctx, &grow_attr, mask); 
  grow_Redraw( grow->ctx);
}

void Graph::set_grid( int grid_on)
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  grid = grid_on;
  mask = 0;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = grid_on;
  grow_SetAttributes( grow->ctx, &grow_attr, mask); 
}

int Graph::get_conclass( glow_eDrawType drawtype, int linewidth,
	glow_eConType contype, glow_eCorner corner, double round_amount, 
	grow_tConClass *cc)
{
  char name[80];
  int sts;
  int r_amount;

  if ( corner != glow_eCorner_Rounded)
    r_amount = 0;
  else
    r_amount = int( 10.0 * round_amount + 0.1);

  sprintf( name, "cc_%d_%d_%d_%d", contype, linewidth, drawtype, r_amount);

  sts = grow_FindConClassByName( grow->ctx, name, cc);
  if ( EVEN(sts))
  {
    // Create the conclass
    grow_CreateConClass( grow->ctx, name,
	contype, corner, drawtype,
	linewidth, 0.4 * linewidth, 0.6 * linewidth, round_amount, 
	glow_eConGroup_Common, cc);
  }
  return 1;
}

void Graph::change_text( grow_tObject object, char *text)
{
  // Check that object exist
  if ( !grow_FindObject( grow->ctx, object))
    return;

  if ( grow_GetObjectType( object) != glow_eObjectType_GrowText)
    return;

  grow_SetObjectText( object, text);
  grow_SetModified( grow->ctx, 1);
}

void Graph::change_name( grow_tObject object, char *name)
{
  // Check that object exist
  if ( !grow_FindObject( grow->ctx, object))
    return;

  grow_SetObjectName( object, name);
  grow_SetModified( grow->ctx, 1);
}

void Graph::change_select_text()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  char		text[80];

  grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
  if ( sel_count == 1 && 
       grow_GetObjectType( *sel_list) == glow_eObjectType_GrowText)
  {
    if ( change_text_cb)
    {
      grow_GetObjectText( *sel_list, text);
      (change_text_cb)( parent_ctx, *sel_list, text);
    }
  }
  else
    message( 'E', "Select one text object");
}

void Graph::change_select_name()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  char		name[80];

  grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
  if ( sel_count == 1)
  {
    if ( change_name_cb)
    {
      grow_GetObjectName( *sel_list, name);
      (change_name_cb)( parent_ctx, *sel_list, name);
    }
  }
  else
    message( 'E', "Select one object");
}

void Graph::change_value( grow_tObject object, char *text)
{
  // Check that object exist
  if ( !grow_FindObject( grow->ctx, object))
    return;

  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  if ( dyn->action_type & ge_mActionType_Confirm) {
    glow_sEvent event;

    event.event = glow_eEvent_MB1Click;

    // Trigger the confirm action
    dyn->total_action_type = ge_mActionType( dyn->total_action_type & ~ge_mActionType_ValueInput);
    strncpy( confirm_text, text, sizeof(confirm_text));
    dyn->action( object, &event);
    dyn->total_action_type = ge_mActionType( dyn->total_action_type | ge_mActionType_ValueInput);
  }
  else {
    dyn->change_value( object, text);

    // Send a Key_Tab event
    glow_sEvent event;

    event.event = glow_eEvent_Key_Tab;
    event.object.object = object;
    event.object.object_type = grow_GetObjectType( object);
    dyn->action( object, &event);
  }
}

void Graph::set_select_text_color()
{
  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigTextColor( grow->ctx, get_text_drawtype());
}

void Graph::set_select_fill_color()
{
  glow_eDrawType drawtype = get_fill_drawtype();

  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigFillColor( grow->ctx, drawtype);

  if ( grow_AnySelectIsCon( grow->ctx))
  {
    grow_tConClass	cc;
    int			i;
    grow_tCon		*conlist, *con_ptr;
    int			con_cnt;
    glow_eDrawType	con_drawtype;
    int			con_linewidth;
    glow_eConType	con_contype;
    glow_eCorner 	con_corner;
    double		con_corner_round_amount;

    grow_GetSelectedCons( grow->ctx, &conlist, &con_cnt);
    con_ptr = conlist;
    for ( i = 0; i < con_cnt; i++)
    {
      grow_GetConAttributes( *con_ptr, &con_drawtype, &con_linewidth, 
	&con_contype, &con_corner, &con_corner_round_amount);
      get_conclass( drawtype, con_linewidth, con_contype, con_corner,
		con_corner_round_amount, &cc);
      grow_ChangeConConClass( *con_ptr, cc);
      con_ptr++;
    }
    if ( con_cnt)
      free( (char *)conlist);
  }
}

void Graph::set_select_border_color()
{
  glow_eDrawType drawtype = get_border_drawtype();

  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigBorderColor( grow->ctx, drawtype);
}

void Graph::set_select_color_tone( glow_eDrawTone tone)
{
  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigColorTone( grow->ctx, tone);
  if ( tone == glow_eDrawTone_No)
    // Reset the fillcolor also
    grow_SetSelectOrigFillColor( grow->ctx, glow_eDrawType_No);
}

void Graph::incr_select_color_lightness( int lightness)
{
  grow_DisableHighlight( grow->ctx);
  grow_IncrSelectOrigColLightness( grow->ctx, lightness);
}

void Graph::incr_select_color_intensity( int intensity)
{
  grow_DisableHighlight( grow->ctx);
  grow_IncrSelectOrigColIntensity( grow->ctx, intensity);
}

void Graph::incr_select_color_shift( int shift)
{
  grow_DisableHighlight( grow->ctx);
  grow_IncrSelectOrigColorShift( grow->ctx, shift);
}

void Graph::set_select_linewidth( int width)
{
  linewidth = width;

  grow_DisableHighlight( grow->ctx);
  grow_SetSelectLineWidth( grow->ctx, width);

  if ( grow_AnySelectIsCon( grow->ctx))
  {
    grow_tConClass	cc;
    int			i;
    grow_tCon		*conlist, *con_ptr;
    int			con_cnt;
    glow_eDrawType	con_drawtype;
    int			con_linewidth;
    glow_eConType	con_contype;
    glow_eCorner 	con_corner;
    double		con_corner_round_amount;

    grow_GetSelectedCons( grow->ctx, &conlist, &con_cnt);
    con_ptr = conlist;
    for ( i = 0; i < con_cnt; i++)
    {
      grow_GetConAttributes( *con_ptr, &con_drawtype, &con_linewidth, 
	&con_contype, &con_corner, &con_corner_round_amount);
      get_conclass( con_drawtype, width, con_contype, con_corner,
		con_corner_round_amount, &cc);
      grow_ChangeConConClass( *con_ptr, cc);
      con_ptr++;
    }
    if ( con_cnt)
      free( (char *)conlist);
  }
}

void Graph::set_select_linetype( glow_eLineType type)
{
  linetype = type;

  grow_DisableHighlight( grow->ctx);
  grow_SetSelectLineType( grow->ctx, type);
}

void Graph::set_select_fill( int fill)
{
  grow_SetSelectFill( grow->ctx, fill);
}

void Graph::set_select_border( int border)
{
  grow_SetSelectBorder( grow->ctx, border);
}

void Graph::set_select_shadow( int border)
{
  grow_SetSelectShadow( grow->ctx, shadow);
}

void Graph::set_select_textsize( int size)
{
  int textsize;

  switch( size)
  {
    case 0: textsize = 0; break;
    case 1: textsize = 1; break;
    case 2: textsize = 2; break;
    case 3: textsize = 4; break;
    case 4: textsize = 6; break;
    case 5: textsize = 8; break;
  }

  grow_SetSelectTextSize( grow->ctx, textsize);
}

void Graph::set_select_textbold( int bold)
{
  grow_SetSelectTextBold( grow->ctx, bold);
}

void Graph::set_background_color()
{
  glow_eDrawType fill_color, border_color, text_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color, &text_color);
  grow_SetBackgroundColor( grow->ctx, fill_color);
}

void Graph::set_nav_background_color()
{
  glow_eDrawType color;

  grow_GetBackgroundColor( grow->ctx, &color);
  grow_SetBackgroundColor( grow->ctx, color);
}

void Graph::set_show_grid( int show)
{
  grow_SetShowGrid( grow->ctx, show);
}

glow_eDrawType Graph::get_border_drawtype()
{
  glow_eDrawType fill_color, border_color, text_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color, &text_color);

  return border_color;
}

glow_eDrawType Graph::get_fill_drawtype()
{
  glow_eDrawType fill_color, border_color, text_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color, &text_color);

  return fill_color;
}

glow_eDrawType Graph::get_text_drawtype()
{
  glow_eDrawType fill_color, border_color, text_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color, &text_color);

  return text_color;
}

void Graph::select_all_cons()
{
  grow_tObject 	*objectlist, *object_p;
  int 		object_cnt;
  int		i;

  grow_SelectClear( grow->ctx);
  grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);
  object_p = objectlist;
  for ( i = 0; i < object_cnt; i++)
  {
   if ( grow_GetObjectType( *object_p) == glow_eObjectType_Con)
   {
      grow_SetHighlight( *object_p, 1);
      grow_SelectInsert( grow->ctx, *object_p);
    }
    object_p++;
  }
}

void Graph::select_all_objects()
{
  grow_tObject 	*objectlist, *object_p;
  int 		object_cnt;
  int		i;

  grow_SelectClear( grow->ctx);
  grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);
  object_p = objectlist;
  for ( i = 0; i < object_cnt; i++)
  {
    if ( grow_GetObjectType( *object_p) != glow_eObjectType_Con)
    {
      grow_SetHighlight( *object_p, 1);
      grow_SelectInsert( grow->ctx, *object_p);
    }
    object_p++;
  }
}

int Graph::get_selected_object( grow_tObject *object)
{
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( grow->ctx, &sel_list, &sel_count);

  if ( sel_count == 0)
    return GE__NOSELECT;

  if ( grow_GetObjectType( *sel_list) == glow_eObjectType_Con)
    return GE__NOSELECT;

  if ( sel_count > 1)
    return GE__MANYSELECT;

  *object = *sel_list;

  return GE__SUCCESS;
}

int Graph::is_subgraph()
{
  return grow_IsSubGraph( grow->ctx);
}

int Graph::is_authorized( unsigned int access)
{
  if ( is_authorized_cb) {
    if ( use_default_access && (access & pwr_mAccess_Default))
      return (is_authorized_cb)( parent_ctx, default_access);
    else
      return (is_authorized_cb)( parent_ctx, access);
  }
  return 1;
}

int Graph::get_attr_items( grow_tObject object, attr_sItem **itemlist,
	int *item_cnt, void **client_data)
{
  static attr_sItem	items[100];
  int			i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;

  memset( items, 0, sizeof(items));
  if ( grow_GetObjectType( object) == glow_eObjectType_GrowNode ||
       grow_GetObjectType( object) == glow_eObjectType_GrowGroup)
  {
    GeDyn *dyn;
    char *transtab;
    grow_GetUserData( object, (void **)&dyn);

    dyn->get_transtab( object, &transtab);
    if ( transtab)
      grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info,
			      &grow_info_cnt);


    *itemlist = items; 
    *item_cnt = 0;

    grow_info_p = grow_info;
    for ( i = *item_cnt; i < grow_info_cnt + *item_cnt; i++) {
      items[i].value = grow_info_p->value_p;
      strcpy( items[i].name, grow_info_p->name);
      items[i].type = grow_info_p->type;
      items[i].size = grow_info_p->size;
      items[i].minlimit = 0;
      items[i].maxlimit = 0;
      items[i].noedit = grow_info_p->no_edit;
      items[i].multiline = grow_info_p->multiline;
      grow_info_p++;
    }

    *client_data = (void *)grow_info;

    *item_cnt = grow_info_cnt;
    dyn->get_attributes( object, items, item_cnt);

    return 1;

  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowBar)
  {
    char transtab[][32] = {	 	"MaxValue",		"Bar.MaxValue",
					"MinValue",		"Bar.MinValue",
					"BarValue",		"Bar.Value",
					"BarColor",		"Bar.BarColor",
					"BarBorderColor",	"Bar.BorderColor",
					"BarBorderWidth",	"Bar.BorderWidth",
					"Dynamic",		"Dynamic",
					""};
    GeDyn *dyn;

    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

    grow_GetUserData( object, (void **)&dyn);

    *item_cnt = 0;
    dyn->get_attributes( object, items, item_cnt);

    *client_data = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowWindow)
  {
    char transtab[][32] = {	 	"FileName",		"Window.FileName",
					"WindowScale",   	"Window.Scale",
					"VerticalScrollbar",   	"Window.VerticalScrollbar",
					"HorizontalScrollbar",	"Window.HorizontalScrollbar",
					"ScrollbarWidth", 	"Window.ScrollbarWidth",
					"ScrollbarColor", 	"Window.ScrollbarColor",
					"ScrollbarBgColor", 	"Window.ScrollbarBgColor",
					""};
    GeDyn *dyn;

    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

    grow_GetUserData( object, (void **)&dyn);

    *item_cnt = 0;
    dyn->get_attributes( object, items, item_cnt);

    *client_data = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowTable)
  {
    char transtab[][32] = {	 	"Rows",			"Table.Rows",
					"Columns",   		"Table.Columns",
					"RowHeight", 		"Table.RowHeight",
					"HeaderRow",   		"Table.HeaderRow",
					"HeaderColumn",   	"Table.HeaderColumn",
					"HeaderRowHeight", 	"Table.HeaderRowHeight",
					"HeaderTextSize", 	"Table.HeaderTextSize",
					"HeaderTextBold", 	"Table.HeaderTextBold",
					"HeaderTextColor", 	"Table.HeaderTextColor",
					"Options", 		"Table.Options",
					"VerticalScrollbar",   	"Table.VerticalScrollbar",
					"HorizontalScrollbar",	"Table.HorizontalScrollbar",
					"ScrollbarWidth", 	"Table.ScrollbarWidth",
					"ScrollbarColor", 	"Table.ScrollbarColor",
					"ScrollbarBgColor", 	"Table.ScrollbarBgColor",
					"ColumnWidth1",        	"Column1.Width",
					"HeaderText1", 		"Column1.HeaderText",
					"ColumnWidth2",        	"Column2.Width",
					"HeaderText2", 		"Column2.HeaderText",
					"ColumnWidth3",        	"Column3.Width",
					"HeaderText3", 		"Column3.HeaderText",
					"ColumnWidth4",        	"Column4.Width",
					"HeaderText4", 		"Column4.HeaderText",
					"ColumnWidth5",        	"Column5.Width",
					"HeaderText5", 		"Column5.HeaderText",
					"ColumnWidth6",        	"Column6.Width",
					"HeaderText6", 		"Column6.HeaderText",
					"ColumnWidth7",        	"Column7.Width",
					"HeaderText7", 		"Column7.HeaderText",
					"ColumnWidth8",        	"Column8.Width",
					"HeaderText8", 		"Column8.HeaderText",
					"ColumnWidth9",        	"Column9.Width",
					"HeaderText9", 		"Column9.HeaderText",
					"ColumnWidth10",       	"Column10.Width",
					"HeaderText10",        	"Column10.HeaderText",
					"ColumnWidth11",       	"Column11.Width",
					"HeaderText11",        	"Column11.HeaderText",
					"ColumnWidth12",       	"Column12.Width",
					"HeaderText12",        	"Column12.HeaderText",
					""};
    GeDyn *dyn;

    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

    grow_GetUserData( object, (void **)&dyn);

    *item_cnt = 0;
    dyn->get_attributes( object, items, item_cnt);

    *client_data = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowFolder)
  {
    char transtab[][32] = {	 	"Folders",		"Folder.NumberOfFolders",
					"ColorSelected",   	"Folder.ColorSelected",
					"ColorUnselected",   	"Folder.ColorUnselected",
					"HeaderHeight",   	"Folder.HeaderHeight",
					"FileName1",   		"Folder1.FileName",
					"Text1",   		"Folder1.Text",
					"Scale1",		"Folder1.Scale",
					"VerticalScrollbar1", 	"Folder1.VerticalScrollbar",
					"HorizontalScrollbar1",	"Folder1.HorizontalScrollbar",
					"FileName2",   		"Folder2.FileName",
					"Text2",   		"Folder2.Text",
					"Scale2",		"Folder2.Scale",
					"VerticalScrollbar2", 	"Folder2.VerticalScrollbar",
					"HorizontalScrollbar2",	"Folder2.HorizontalScrollbar",
					"FileName3",   		"Folder3.FileName",
					"Text3",   		"Folder3.Text",
					"Scale3",		"Folder3.Scale",
					"VerticalScrollbar3", 	"Folder3.VerticalScrollbar",
					"HorizontalScrollbar3",	"Folder3.HorizontalScrollbar",
					"FileName4",   		"Folder4.FileName",
					"Text4",   		"Folder4.Text",
					"Scale4",		"Folder4.Scale",
					"VerticalScrollbar4", 	"Folder4.VerticalScrollbar",
					"HorizontalScrollbar4",	"Folder4.HorizontalScrollbar",
					"FileName5",   		"Folder5.FileName",
					"Text5",   		"Folder5.Text",
					"Scale5",		"Folder5.Scale",
					"VerticalScrollbar5", 	"Folder5.VerticalScrollbar",
					"HorizontalScrollbar5",	"Folder5.HorizontalScrollbar",
					"FileName6",   		"Folder6.FileName",
					"Text6",   		"Folder6.Text",
					"Scale6",		"Folder6.Scale",
					"VerticalScrollbar6", 	"Folder6.VerticalScrollbar",
					"HorizontalScrollbar6",	"Folder6.HorizontalScrollbar",
					"FileName7",   		"Folder7.FileName",
					"Text7",   		"Folder7.Text",
					"Scale7",		"Folder7.Scale",
					"VerticalScrollbar7", 	"Folder7.VerticalScrollbar",
					"HorizontalScrollbar7",	"Folder7.HorizontalScrollbar",
					"FileName8",   		"Folder8.FileName",
					"Text8",   		"Folder8.Text",
					"Scale8",		"Folder8.Scale",
					"VerticalScrollbar8", 	"Folder8.VerticalScrollbar",
					"HorizontalScrollbar8",	"Folder8.HorizontalScrollbar",
					"FileName9",   		"Folder9.FileName",
					"Text9",   		"Folder9.Text",
					"Scale9",		"Folder9.Scale",
					"VerticalScrollbar9", 	"Folder9.VerticalScrollbar",
					"HorizontalScrollbar9",	"Folder9.HorizontalScrollbar",
					"FileName10",   	"Folder10.FileName",
					"Text10",   		"Folder10.Text",
					"Scale10",		"Folder10.Scale",
					"VerticalScrollbar10", 	"Folder10.VerticalScrollbar",
					"HorizontalScrollbar10","Folder10.HorizontalScrollbar",
					"FileName11",   	"Folder11.FileName",
					"Text11",   		"Folder11.Text",
					"Scale11",		"Folder11.Scale",
					"VerticalScrollbar11", 	"Folder11.VerticalScrollbar",
					"HorizontalScrollbar11","Folder11.HorizontalScrollbar",
					"FileName12",   	"Folder12.FileName",
					"Text12",   		"Folder12.Text",
					"Scale12",		"Folder12.Scale",
					"VerticalScrollbar12", 	"Folder12.VerticalScrollbar",
					"HorizontalScrollbar12","Folder12.HorizontalScrollbar",
					""};
    GeDyn *dyn;

    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

    grow_GetUserData( object, (void **)&dyn);

    *item_cnt = 0;
    dyn->get_attributes( object, items, item_cnt);

    *client_data = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowTrend)
  {
    GeDyn *dyn;

    grow_GetUserData( object, (void **)&dyn);

    if ( dyn->dyn_type & ge_mDynType_FastCurve) {
      char transtab[][32] = {	 	"NoOfPoints",		"FastCurve.NoOfPoints",
					"ScanTime",		"",
					"CurveWidth",		"FastCurve.CurveLineWidth",
					"FillCurve",		"FastCurve.FillCurve",
					"HorizontalLines",	"FastCurve.HorizontalLines",
					"VerticalLines",	"FastCurve.VerticalLines",
					"MaxValue1",		"FastCurve.MaxValue1",
					"MinValue1",		"FastCurve.MinValue1",
					"CurveColor1",		"FastCurve.CurveColor1",
					"CurveFillColor1",	"FastCurve.CurveFillColor1",
					"MaxValue2",		"FastCurve.MaxValue2",
					"MinValue2",		"FastCurve.MinValue2",
					"CurveColor2",		"FastCurve.CurveColor2",
					"CurveFillColor2",	"FastCurve.CurveFillColor2",
					"Dynamic",		"",
					""};
      grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

      *item_cnt = 0;
      dyn->get_attributes( object, items, item_cnt);
    }
    else {
      char transtab[][32] = {	 	"NoOfPoints",		"Trend.NoOfPoints",
					"ScanTime",		"Trend.ScanTime",
					"CurveWidth",		"Trend.CurveLineWidth",
					"FillCurve",		"Trend.FillCurve",
					"HorizontalLines",	"Trend.HorizontalLines",
					"VerticalLines",	"Trend.VerticalLines",
					"MaxValue1",		"Trend.MaxValue1",
					"MinValue1",		"Trend.MinValue1",
					"CurveColor1",		"Trend.CurveColor1",
					"CurveFillColor1",	"Trend.CurveFillColor1",
					"MaxValue2",		"Trend.MaxValue2",
					"MinValue2",		"Trend.MinValue2",
					"CurveColor2",		"Trend.CurveColor2",
					"CurveFillColor2",	"Trend.CurveFillColor2",
					"Dynamic",		"",
					""};
      grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);

      *item_cnt = 0;
      dyn->get_attributes( object, items, item_cnt);
    }
    *client_data = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowAxis)
  {
    char transtab[][32] = {             "MaxValue",	        "MaxValue",
                                        "MinValue",	        "MinValue",
                                        "Lines",	        "Lines",
					"LongQuotient",	        "LongQuotient",
					"ValueQuotient",	"ValueQuotient",
					"Dynamic",		"Dynamic",
					""};
    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
    *item_cnt = 0;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowSlider)
  {
    char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"Direction",		"Slider.Direction",
					"MaxValue",		"Slider.MaxValue",
					"MinValue",		"Slider.MinValue",
					"MaxPos",		"Slider.MaxPosition",
					"MinPos",		"Slider.MinPosition",
					"Dynamic",		"",
					""};
    GeDyn *dyn;

    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info,
		&grow_info_cnt);

    grow_GetUserData( object, (void **)&dyn);

    grow_info_p = grow_info;
    *item_cnt = 0;
    for ( i = *item_cnt; i < grow_info_cnt + *item_cnt; i++) {
      items[i].value = grow_info_p->value_p;
      strcpy( items[i].name, grow_info_p->name);
      items[i].type = grow_info_p->type;
      items[i].size = grow_info_p->size;
      items[i].minlimit = 0;
      items[i].maxlimit = 0;
      items[i].noedit = grow_info_p->no_edit;
      items[i].multiline = grow_info_p->multiline;
      grow_info_p++;
    }

    *item_cnt = grow_info_cnt;
    dyn->get_attributes( object, items, item_cnt);

    *itemlist = items;
    *client_data = (void *)grow_info;
    
    return 1;
  }
  else {
    grow_GetObjectAttrInfo( object, NULL, &grow_info, &grow_info_cnt);
    *item_cnt = 0;
  }

  grow_info_p = grow_info;
  for ( i = *item_cnt; i < grow_info_cnt + *item_cnt; i++)
  {
    items[i].value = grow_info_p->value_p;
    strcpy( items[i].name, grow_info_p->name);
    items[i].type = grow_info_p->type;
    items[i].size = grow_info_p->size;
    items[i].minlimit = grow_info_p->minlimit;
    items[i].maxlimit = grow_info_p->maxlimit;
    items[i].noedit = grow_info_p->no_edit;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }

  *itemlist = items;
  *item_cnt += grow_info_cnt;
  *client_data = (void *)grow_info;

  return 1;
}

int Graph::edit_attributes( grow_tObject object)
{
  attr_sItem 	*itemlist;
  int 		item_cnt;
  Attr	        *attr;
  void		*client_data;

  get_attr_items( object, &itemlist, &item_cnt, &client_data);

  attr = new Attr( parent_wid, this, object, itemlist, item_cnt);
  attr->client_data = client_data;
  attr->close_cb = graph_attr_close_cb;
  attr->redraw_cb = graph_attr_redraw_cb;
  attr->get_subgraph_info_cb = &graph_get_subgraph_info_cb;
  attr->get_dyn_info_cb = &graph_get_dyn_info_cb;
  attr->reconfigure_attr_cb = &graph_reconfigure_attr_cb;
  attr->store_cb = &graph_attr_store_cb;
  attr->recall_cb = &graph_attr_recall_cb;
  attr->set_data_cb = &graph_attr_set_data_cb;
  attr->get_plant_select_cb = &graph_get_plant_select_cb;
  attr->get_current_colors_cb = &graph_get_current_colors_cb;
  attr_list.insert( (void *) attr);
  grow_SetModified( grow->ctx, 1);
  return 1;
}


static int graph_get_plant_select_cb( void *g, char *value)
{
  Graph	*graph = (Graph *)g;
  if ( graph->get_plant_select_cb)
    return (graph->get_plant_select_cb) (graph->parent_ctx, value);
  return 0;
}

static int graph_get_current_colors_cb( void *g, glow_eDrawType *fill_color, 
					glow_eDrawType *border_color, glow_eDrawType *text_color)
{
  Graph	*graph = (Graph *)g;

  if ( graph->get_current_colors_cb) {
    (graph->get_current_colors_cb) (graph->parent_ctx, fill_color, border_color, text_color);
    return 1;
  }
  return 0;
}

static int graph_reconfigure_attr_cb( void *g, grow_tObject object,
	attr_sItem **itemlist, int *itemlist_cnt, void **client_data)
{
  Graph	*graph = (Graph *)g;

  return graph->get_attr_items( object, itemlist, itemlist_cnt, 
	client_data);
}

static void graph_attr_store_cb( void *g, grow_tObject object)
{
  Graph	*graph = (Graph *)g;
  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  graph->recall.insert( dyn, "", object);
}

static int graph_attr_recall_cb( void *g, grow_tObject object, int idx, 
				 GeDyn **old_dyn)
{
  Graph	*graph = (Graph *)g;
  GeDyn *dyn;
  int sts;

  if ( grow_GetObjectType( object) == glow_eObjectType_GrowNode ||
       grow_GetObjectType( object) == glow_eObjectType_GrowSlider ||
       grow_GetObjectType( object) == glow_eObjectType_GrowGroup ||
       grow_GetObjectType( object) == glow_eObjectType_GrowWindow ||
       grow_GetObjectType( object) == glow_eObjectType_GrowTrend ||
       grow_GetObjectType( object) == glow_eObjectType_GrowTable ||
       grow_GetObjectType( object) == glow_eObjectType_GrowBar) {
    sts = graph->recall.get( &dyn, idx);
    if ( ODD(sts)) {
      grow_GetUserData( object, (void **)old_dyn);
      grow_SetUserData( object, (void *)dyn);
    }
    return sts;
  }
  else
    return 0;
}

static int graph_attr_set_data_cb( void *g, grow_tObject object,
				 GeDyn *data)
{
  grow_SetUserData( object, (void *)data);
  return 1;
}

static int graph_get_dyn_info_cb( void *g, GeDyn *dyn,
	attr_sItem **itemlist, int *itemlist_cnt)
{
  static attr_sItem	items[40];

  *itemlist = items; 
  *itemlist_cnt = 0;

  dyn->get_attributes( 0, items, itemlist_cnt);
  return 1;
}

static int graph_get_subgraph_info_cb( void *g, char *name, 
	attr_sItem **itemlist, int *itemlist_cnt)
{
  Graph	*graph = (Graph *)g;
  static attr_sItem	items[10];
  int			i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  int			sts;
  grow_tObject		object;
  int			dyn_type;
  int			dyn_action_type;

  sts = grow_FindNodeClassByName( graph->grow->ctx, name, &object);
  if ( EVEN(sts)) return sts;

  grow_GetObjectAttrInfo( object, NULL, &grow_info, &grow_info_cnt);
  grow_GetNodeClassDynType( object, &dyn_type, &dyn_action_type);

  grow_info_p = grow_info;
  for ( i = 0; i < grow_info_cnt; i++)
  {
    items[i].value = grow_info_p->value_p;
    strcpy( items[i].name, grow_info_p->name);
    if ( grow_info_p->type == glow_eType_TraceColor) {
      if ( dyn_type & ge_mDynType_Tone)
	items[i].type = glow_eType_ToneOrColor;
      else
	items[i].type = glow_eType_Color;
    }
    else
      items[i].type = grow_info_p->type;
    items[i].size = grow_info_p->size;
    items[i].minlimit = 0;
    items[i].maxlimit = 0;
    items[i].noedit = 1;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }
  *itemlist = items;
  *itemlist_cnt = grow_info_cnt;
  return 1;
}

int Graph::edit_subgraph_attributes()
{
  attr_sItem	items[40];
  int		i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  Attr   		*attr;
  int			dyn_type;
  int			dyn_action_type;
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

  grow_GetSubGraphAttrInfo( grow->ctx, (char *)transtab, &grow_info, &grow_info_cnt);
  grow_GetSubGraphDynType( grow->ctx, &dyn_type, &dyn_action_type);

  grow_info_p = grow_info;
  memset( items, 0, sizeof(items));
  for ( i = 0; i < grow_info_cnt; i++) {
    items[i].value = grow_info_p->value_p;
    strcpy( items[i].name, grow_info_p->name);

    if ( grow_info_p->type == glow_eType_TraceColor) {
      if ( dyn_type & ge_mDynType_Tone)
	items[i].type = glow_eType_ToneOrColor;
      else
	items[i].type = glow_eType_Color;
    }
    else if ( grow_info_p->type == glow_eType_DynType)
      items[i].type = ge_eAttrType_DynType;
    else if ( grow_info_p->type == glow_eType_ActionType)
      items[i].type = ge_eAttrType_ActionType;
    else
      items[i].type = grow_info_p->type;
    if ( strcmp( grow_info_p->name, "AnimSequence") == 0)
      items[i].type = ge_eAttrType_AnimSequence;
    items[i].size = grow_info_p->size;
    items[i].minlimit = 0;
    items[i].maxlimit = 0;
    items[i].noedit = 0;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }

  attr = new Attr( parent_wid, this, NULL, (attr_sItem *)&items, i);
  attr->client_data = (void *)grow_info;
  attr->close_cb = graph_subgraphattr_close_cb;
  attr->redraw_cb = graph_subgraphattr_redraw_cb;
  attr_list.insert( (void *) attr);
  grow_SetModified( grow->ctx, 1);
  return 1;
}

int Graph::edit_graph_attributes()
{
  attr_sItem	items[20];
  int		i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  Attr		*attr;

  grow_GetGraphAttrInfo( grow->ctx, &grow_info, &grow_info_cnt);

  grow_info_p = grow_info;
  for ( i = 0; i < grow_info_cnt; i++)
  {
    items[i].value = grow_info_p->value_p;
    strcpy( items[i].name, grow_info_p->name);
    items[i].type = grow_info_p->type;
    items[i].size = grow_info_p->size;
    items[i].minlimit = 0;
    items[i].maxlimit = 0;
    items[i].noedit = 0;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }

  attr = new Attr( parent_wid, this, NULL, (attr_sItem *)&items, i);
  attr->client_data = (void *)grow_info;
  attr->close_cb = graph_graphattr_close_cb;
  attr->redraw_cb = graph_graphattr_redraw_cb;
  attr_list.insert( (void *) attr);
  grow_SetModified( grow->ctx, 1);
  return 1;
}
//
// Callbacks from grow
//
static int graph_grow_cb( GlowCtx *ctx, glow_tEvent event)
{
  Graph		*graph;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &graph);
  if ( graph->closing_down)
    return 1;

  if ( event->event != glow_eEvent_CursorMotion)
    graph->message( ' ', null_str);

  if ( graph->trace_started)
  {
    return graph_trace_grow_cb( ctx, event);
  }

  if ( event->any.type == glow_eEventType_CreateCon)
  {
    grow_tConClass	cc;
    grow_tCon		con;
    char		name[80];

    if ( !event->con_create.dest_object) {
      // Create a ConGlue object
      grow_tObject t1;
      double x, y;
      glow_eDirection dir;
      double margin = 1;

      // Select direction
      grow_GetNodeConPoint( event->con_create.source_object, 
			    event->con_create.source_conpoint,
			    &x, &y, &dir);
      if ( event->con_create.y < y + margin) {
	if ( event->con_create.x > x + margin)
	  event->con_create.dest_conpoint = 3;
	else if ( event->con_create.x < x - margin)
	  event->con_create.dest_conpoint = 1;
	else
	  event->con_create.dest_conpoint = 2;
      }
      else if ( event->con_create.y > y + margin) {
	if ( event->con_create.x > x + margin)
	  event->con_create.dest_conpoint = 3;
	else if ( event->con_create.x < x - margin)
	  event->con_create.dest_conpoint = 1;
	else
	  event->con_create.dest_conpoint = 0;
      }
      else {
	if ( event->con_create.x > x + margin)
	  event->con_create.dest_conpoint = 3;
	else if ( event->con_create.x < x - margin)
	  event->con_create.dest_conpoint = 1;
	else
	  event->con_create.dest_conpoint = 0;
      }
      sprintf( name, "O%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
      grow_CreateGrowConGlue( graph->grow->ctx, name, 
			      event->con_create.x, event->con_create.y, &t1);
      event->con_create.dest_object = t1;
    }

    graph->get_conclass( graph->get_fill_drawtype(), 
		graph->linewidth, graph->con_type, graph->con_corner,
		graph->corner_round_amount, &cc);
    sprintf( name, "C%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
    grow_CreateCon( graph->grow->ctx, "", cc,
	event->con_create.source_object, event->con_create.dest_object, 
	event->con_create.source_conpoint, event->con_create.dest_conpoint,
	NULL, &con, 0, NULL, NULL, graph->border, graph->shadow);
    grow_SetModified( graph->grow->ctx, 1);
    return 1;
  }
  switch ( event->event)
  {
    case glow_eEvent_CreateGrowObject:
      // Create some object
      switch ( event->create_grow_object.mode)
      {
        case grow_eMode_Rect:
        {
	  grow_tObject r1;

          grow_CreateGrowRect( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2 - event->create_grow_object.x, 
	    event->create_grow_object.y2 - event->create_grow_object.y,
	    graph->get_border_drawtype(), graph->linewidth, 0, 
	    glow_mDisplayLevel_1, graph->fill, graph->border, graph->shadow,
	    graph->get_fill_drawtype(), NULL, &r1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_RectRounded:
        {
	  grow_tObject r1;

          grow_CreateGrowRectRounded( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2 - event->create_grow_object.x, 
	    event->create_grow_object.y2 - event->create_grow_object.y,
	    graph->get_border_drawtype(), graph->linewidth, 0, 
	    glow_mDisplayLevel_1, graph->fill, graph->border, graph->shadow,
	    graph->get_fill_drawtype(), NULL, &r1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Line:
        {
	  grow_tObject l1;

          grow_CreateGrowLine( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2, event->create_grow_object.y2,
	    graph->get_border_drawtype(), graph->linewidth, 0, NULL, &l1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( graph->linetype != glow_eLineType_Solid)
	    grow_SetObjectLinetype( l1, graph->linetype);

	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_PolyLine:
        {
	  glow_sPoint points[2];
	  int 	point_cnt;

          if ( event->create_grow_object.first_line)
          {
	    points[0].x = event->create_grow_object.x2;
	    points[0].y = event->create_grow_object.y2;
	    points[1].x = event->create_grow_object.x;
	    points[1].y = event->create_grow_object.y;
	    point_cnt = 2;
	    grow_CreateGrowPolyLine( graph->grow->ctx, "", 
		(glow_sPoint *)&points, point_cnt,
	      	graph->get_border_drawtype(), graph->linewidth, 0, 
		0, graph->border, 0,
		graph->get_fill_drawtype(), 0, NULL, 
		&graph->current_polyline);
          }
          else
	  {
	    points[0].x = event->create_grow_object.x;
	    points[0].y = event->create_grow_object.y;
	    point_cnt = 1;
	    grow_AddPolyLinePoints( graph->current_polyline,
		(glow_sPoint *)&points, point_cnt);
	  }
          grow_SetModified( graph->grow->ctx, 1);
          break;
        }
        case grow_eMode_Circle:
        {
	  grow_tObject a1;

          grow_CreateGrowArc( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2, event->create_grow_object.y2,
	    0, 360, graph->get_border_drawtype(), graph->linewidth,
	    graph->fill, graph->border, graph->shadow, graph->get_fill_drawtype(), NULL, &a1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
      }
    case glow_eEvent_SelectClear:
      grow_ResetSelectHighlight( graph->grow->ctx);
      break;
    case glow_eEvent_ObjectDeleted:
      if ( graph->current_polyline && 
	   event->object.object == graph->current_polyline)
      {
        grow_PolylineEnd( graph->grow->ctx);
        graph->current_polyline = 0;
      }
      grow_SetModified( graph->grow->ctx, 1);

      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowWindow ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTrend ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTable ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowBar) {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
        delete dyn;
      }
      break;
    case glow_eEvent_MB3Click:
      if ( grow_Mode( graph->grow->ctx) == grow_eMode_PolyLine && graph->current_polyline) {
	if ( graph->fill)
	  grow_SetObjectFill( graph->current_polyline, 1);
	if ( graph->shadow)
	  grow_SetObjectShadow( graph->current_polyline, 1);
      }
      grow_PolylineEnd( graph->grow->ctx);
      graph->current_polyline = 0;
      grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
      grow_EnableHighlight( graph->grow->ctx);
      grow_SelectClear( graph->grow->ctx);
      grow_SetMoveRestrictions( graph->grow->ctx, glow_eMoveRestriction_No, 0, 0, NULL);
      grow_SetScaleEqual( graph->grow->ctx, 0);
      graph->keep_mode = false;
      break;
    case glow_eEvent_MB1DoubleClick:
      if ( event->object.object_type != glow_eObjectType_NoObject &&
           event->object.object_type != glow_eObjectType_Con)
      {
        graph->edit_attributes( event->object.object);
      }
      break;
    case glow_eEvent_MB2DoubleClick:
      if ( event->object.object_type != glow_eObjectType_NoObject)
      {
        grow_DeleteObject( graph->grow->ctx, event->object.object);
        grow_SetModified( graph->grow->ctx, 1);
      }
      else
      {
        grow_tObject 	*sel_list;
        int		sel_count;

        grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
        while( sel_count)
        {
          grow_DeleteObject( graph->grow->ctx, *sel_list);
          grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
        }
        grow_SetModified( graph->grow->ctx, 1);
      }
      break;
    case glow_eEvent_MB2Click:
    {
      char 		sub_name[80] = "graph";
      char 		filename[200];
      char 		name[80];
      grow_tNodeClass	nc;
      grow_tNode	n1;
      int		sts;
      char		dev[80], dir[80], file[80], type[32];
      int		version;

      // Create subgraph object
      sts = (graph->get_current_subgraph_cb)( graph->parent_ctx, sub_name, 
		filename);
      if ( EVEN(sts))
      { 
        graph->message( 'E', "Select a SubGraph");
        break;
      }
      dcli_parse_filename( filename, dev, dir, file, type, &version);
      cdh_ToLower( sub_name, file);
      if ( strcmp( type, ".pwsg") == 0)
      {
        sts = grow_FindNodeClassByName( graph->grow->ctx, 
		sub_name, &nc);
        if ( EVEN(sts))
        {
          // Load the subgraph
          grow_OpenSubGraph( graph->grow->ctx, filename);
        }
        sts = grow_FindNodeClassByName( graph->grow->ctx, 
		sub_name, &nc);
        if ( EVEN(sts))
        {
          graph->message( 'E', "Unable to open subgraph");
          break;
        }

        sprintf( name, "O%d", grow_GetNextObjectNameNumber( graph->grow->ctx));

        if ( !grow_IsSliderClass( nc))
          grow_CreateGrowNode( graph->grow->ctx, name, nc, event->any.x, event->any.y, 
		NULL, &n1);
        else
          grow_CreateGrowSlider( graph->grow->ctx, name, nc, event->any.x, event->any.y, 
		NULL, &n1);

	if ( graph->shadow)
	  grow_SetObjectShadow( n1, 1);

	GeDyn *dyn = new GeDyn( graph);
        grow_SetUserData( n1, (void *)dyn);
        if ( grow_IsSliderClass( nc))
	  dyn->action_type = ge_mActionType( dyn->action_type | ge_mActionType_Slider);

      }
      else if ( strcmp( type, ".gif") == 0 || strcmp( type, ".jpg") == 0)
      {
	grow_tObject i1;

        if ( strncmp( dir, "jpwr/", 5) == 0) {
          strcpy( filename, dir);
          strcat( filename, file);
          strcat( filename, type);
        }
	else {
          strcpy( filename, file);
          strcat( filename, type);
        }
        grow_CreateGrowImage( graph->grow->ctx, "", filename, 
	    event->create_grow_object.x, event->create_grow_object.y,
            NULL, &i1);
      }
      else if ( strcmp( type, ".component") == 0) {
	if ( strcmp( sub_name, "pwr_trend") == 0) {
	  grow_tObject t1;
	  graph->create_trend( &t1, event->create_grow_object.x, 
			       event->create_grow_object.y, 
			       (unsigned int)ge_mDynType_Trend);
	}
	if ( strcmp( sub_name, "pwr_fastcurve") == 0) {
	  grow_tObject t1;
	  graph->create_trend( &t1, event->create_grow_object.x, 
			       event->create_grow_object.y, 
			       (unsigned int)ge_mDynType_FastCurve);
	}
	else if ( strcmp( sub_name, "pwr_bar") == 0) {
	  grow_tObject t1;
	  graph->create_bar( &t1, event->create_grow_object.x, event->create_grow_object.y);
	}
	else if ( strcmp( sub_name, "pwr_window") == 0) {
	  grow_tObject t1;
	  graph->create_window( &t1, event->create_grow_object.x, event->create_grow_object.y);
	}
	else if ( strcmp( sub_name, "pwr_table") == 0) {
	  grow_tObject t1;
	  graph->create_table( &t1, event->create_grow_object.x, event->create_grow_object.y);
	}
	else if ( strcmp( sub_name, "pwr_folder") == 0) {
	  grow_tObject t1;
	  graph->create_folder( &t1, event->create_grow_object.x, event->create_grow_object.y);
	}
	else if ( strcmp( sub_name, "pwr_axis") == 0) {
	  grow_tObject t1;
	  graph->create_axis( &t1, event->create_grow_object.x, event->create_grow_object.y);
	}
	else if ( strcmp( sub_name, "pwr_conglue") == 0) {
	  grow_tObject t1;

	  sprintf( name, "O%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
	  grow_CreateGrowConGlue( graph->grow->ctx, name, 
				  event->create_grow_object.x, event->create_grow_object.y, &t1);
	}
      }
      grow_SetModified( graph->grow->ctx, 1);
      break;
    }
    case glow_eEvent_MB1Click:
      // Create object or select

      if ( graph->set_focus_cb)
        (graph->set_focus_cb)( graph->parent_ctx, graph);

      switch ( grow_Mode( graph->grow->ctx))
      {
        case grow_eMode_ConPoint:
        {
	  grow_tObject cp1;

          grow_CreateGrowConPoint( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    grow_GetNextConPointNumber( graph->grow->ctx),
	    graph->conpoint_direction, NULL, &cp1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Annot:
        {
	  grow_tObject a1;

          grow_CreateGrowAnnot( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    1, glow_eDrawType_TextHelveticaBold, graph->get_text_drawtype(),
	    3, glow_eAnnotType_OneLine,
	    0, glow_mDisplayLevel_1, NULL, &a1);
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Text:
        {
	  grow_tObject t1;
	  glow_eDrawType drawtype;
	  int textsize;

          if ( graph->textbold)
            drawtype = glow_eDrawType_TextHelveticaBold;
          else
            drawtype = glow_eDrawType_TextHelvetica;

          switch( graph->textsize)
          {
            case 0: textsize = 0; break;
            case 1: textsize = 1; break;
            case 2: textsize = 2; break;
            case 3: textsize = 4; break;
            case 4: textsize = 6; break;
            case 5: textsize = 8; break;
          }
          grow_CreateGrowText( graph->grow->ctx, "", "",
	    event->create_grow_object.x, event->create_grow_object.y, 
	    drawtype, graph->get_text_drawtype(), textsize, glow_mDisplayLevel_1,
	    NULL, &t1);
          if ( graph->change_text_cb)
            (graph->change_text_cb)( graph->parent_ctx, t1, "");
          grow_SetModified( graph->grow->ctx, 1);
	  if ( !graph->keep_mode)
	    grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Edit:
        {
          // Select
          if ( event->object.object_type == glow_eObjectType_NoObject)
            grow_SelectClear( graph->grow->ctx);
          else
          {
            if ( grow_FindSelectedObject( graph->grow->ctx, event->object.object))
            {
              grow_SelectClear( graph->grow->ctx);
            }
            else
            {
              grow_SelectClear( graph->grow->ctx);
              grow_SetHighlight( event->object.object, 1);
              grow_SelectInsert( graph->grow->ctx, event->object.object);
            }
          }
        }
        default:
          ;
      }
      break;
    case glow_eEvent_MB1ClickShift:
      /* Select */

      if ( graph->set_focus_cb)
        (graph->set_focus_cb)( graph->parent_ctx, graph);

      if ( event->object.object_type != glow_eObjectType_NoObject)
      {
        if ( grow_FindSelectedObject( graph->grow->ctx, event->object.object))
        {
          grow_SetHighlight( event->object.object, 0);
          grow_SelectRemove( graph->grow->ctx, event->object.object);
        }
        else
        {
          grow_SetHighlight( event->object.object, 1);
          grow_SelectInsert( graph->grow->ctx, event->object.object);
        }
      }
      break;
    case glow_eEvent_MB1Press:
      /* Select region */
      grow_SetSelectHighlight( graph->grow->ctx);
      break;
    case glow_eEvent_MB1PressShift:
      /* Select region */
      grow_SetSelectHighlight( graph->grow->ctx);
      break;
    case glow_eEvent_PasteSequenceStart:
    {
      grow_tObject 	*move_list;
      int		move_count;
      int		i;
      char		name[80];

      grow_GetMoveList( graph->grow->ctx, &move_list, &move_count);
      for ( i = 0; i < move_count; i++)
      {
        switch ( grow_GetObjectType( move_list[i]))
        {
          case glow_eObjectType_Con:
            sprintf( name, "C%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
            grow_SetObjectName( move_list[i], name);
            break;
          case glow_eObjectType_Node:
          case glow_eObjectType_GrowNode:
          case glow_eObjectType_GrowSlider:
            sprintf( name, "O%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
            grow_SetObjectName( move_list[i], name);
            break;
          default:
            ;
        }
      }
      grow_SetModified( graph->grow->ctx, 1);
      break;
    }
    case glow_eEvent_GrowDynamics:
      graph->exec_dynamic( event->dynamics.object, event->dynamics.code,
		event->dynamics.dynamic_type);
      break;
    case glow_eEvent_CursorMotion:
      if ( graph->cursor_motion_cb)
        (graph->cursor_motion_cb) (graph->parent_ctx, event->any.x, 
		event->any.y);
      break;
    case glow_eEvent_MB1DoubleClickCtrl:
    case glow_eEvent_MB1DoubleClickShiftCtrl:
    {
      char		attr_name[120];
      int		sts;

      if ( event->object.object_type != glow_eObjectType_NoObject)
      {
        if ( ! graph->get_plant_select_cb)
          break;

        sts = (graph->get_plant_select_cb) (graph->parent_ctx, attr_name);
        if ( EVEN(sts)) {
          graph->message( 'E', "Select an object in the Plant palette");
          break;
        }

        graph->connect( event->object.object, attr_name, 
		event->event == glow_eEvent_MB1DoubleClickShiftCtrl);
        grow_SetModified( graph->grow->ctx, 1);
      }
      break;
    }
    case glow_eEvent_MB1DoubleClickShift:
    {

      if ( event->object.object_type != glow_eObjectType_NoObject &&
           (grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup)) {

        glow_eDrawType		color;
	GeDyn *dyn;

	grow_GetUserData( event->object.object, (void **)&dyn);

        color = graph->get_fill_drawtype();
	dyn->set_color( event->object.object, color);
      }
      break;
    }
    case glow_eEvent_Key_Tab:
    {
      if ( graph->traverse_focus_cb)
        (graph->traverse_focus_cb)( graph->parent_ctx, graph);
      break;
    }
    case glow_eEvent_Map:
    {
      graph->displayed = 1;
      break;
    }
    case glow_eEvent_Translate:
    {
      char new_text[200];
      int sts;

      sts = Lng::translate( event->translate.text, new_text);
      if ( sts)
        event->translate.new_text = new_text;
      return sts;
    }
    default:
      ;
  }
  return 1;
}

void graph_userdata_save_cb( void *f, grow_tObject object)
{
  ofstream *fp = (ofstream *)f;
  GeDyn *dyn;


  grow_GetUserData( object, (void **)&dyn);

  dyn->save( *fp);
}

void graph_userdata_open_cb( void *f, grow_tObject object)
{
  ifstream *fp = (ifstream *)f;
  Graph *graph;

  grow_GetCtxUserData( grow_GetCtx( object), (void **) &graph);
  GeDyn *dyn = new GeDyn( graph);
  grow_SetUserData( object, (void *)dyn);

  dyn->open( *fp);
}

void graph_userdata_copy_cb( grow_tObject object, void *old_data, void **new_data)
{
  GeDyn *dyn = (GeDyn *)old_data;
  GeDyn *new_dyn = new GeDyn( *dyn);

  *new_data = (void *) new_dyn;
}

int	GraphGbl::load_config( void *graph)
{
  return 1;
}

int Graph::grow_pop()
{
  GrowCtx *secondary_ctx;

  if ( grow_cnt >= GRAPH_GROW_MAX)
    return 0;
  grow_CreateSecondaryCtx( grow->ctx, &secondary_ctx,
        graph_init_grow_cb, (void *)this, glow_eCtxType_Grow);

  grow_ChangeCtx( grow_widget, grow->ctx, grow_stack[grow_cnt]->ctx);
  *grow = *grow_stack[grow_cnt];
  grow_cnt++;
  return 1;
}

int Graph::grow_push()
{

  if ( grow_cnt == 1)
     return 0;

  grow_cnt--;
  grow_ChangeCtx( grow_widget, grow_stack[grow_cnt]->ctx, 
		grow_stack[grow_cnt-1]->ctx);
  *grow = *grow_stack[grow_cnt-1];
  delete grow_stack[grow_cnt];
  return 1;
}

int Graph::grow_push_all()
{
  while( grow_push())
    ;
  return 1;
}

void GraphGrow::grow_setup()
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  mask = 0;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_grid_size_x;
  grow_attr.grid_size_y = 1;
  mask |= grow_eAttr_grid_size_y;
  grow_attr.grid_size_x = 1;
  mask |= grow_eAttr_select_policy;
  grow_attr.select_policy = glow_eSelectPolicy_Surround;
  mask |= grow_eAttr_default_hot_mode;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.default_hot_mode = glow_eHotMode_SingleObject;
  else
    grow_attr.default_hot_mode = glow_eHotMode_TraceAction;
  mask |= grow_eAttr_grafcet_con_delta;
  grow_attr.grafcet_con_delta = 0.6;
  mask |= grow_eAttr_enable_bg_pixmap;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.enable_bg_pixmap = 0;
  else
    grow_attr.enable_bg_pixmap = 1; 
  mask |= grow_eAttr_double_buffer_on;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.double_buffer_on = 1;
  else
    grow_attr.double_buffer_on = 0; 
  grow_SetAttributes( ctx, &grow_attr, mask); 
  grow_SetCtxUserData( ctx, graph);
  grow_SetMoveRestrictions( ctx, glow_eMoveRestriction_No, 0, 0, NULL);

  grow_DisableEventAll( ctx);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClick, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB2DoubleClick, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClickCtrl, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClickShiftCtrl, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClickShift, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Click, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1ClickShift, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_SelectClear, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_ObjectDeleted, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Press, glow_eEventType_RegionSelect, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1PressShift, glow_eEventType_RegionAddSelect, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB2Press, glow_eEventType_CreateCon,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Press, glow_eEventType_MoveNode, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_CreateGrowObject, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB3Click, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB2Click, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_PasteSequenceStart, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_GrowDynamics, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_CursorMotion, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Map, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Tab, glow_eEventType_CallBack,
	graph_grow_cb);

  grow_RegisterUserDataCallbacks( ctx, graph_userdata_save_cb, graph_userdata_open_cb,
				  graph_userdata_copy_cb);
}

void GraphGrow::grow_trace_setup()
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  mask = 0;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_grid_size_x;
  grow_attr.grid_size_y = 1;
  mask |= grow_eAttr_grid_size_y;
  grow_attr.grid_size_x = 1;
  mask |= grow_eAttr_select_policy;
  grow_attr.select_policy = glow_eSelectPolicy_Surround;
  mask |= grow_eAttr_default_hot_mode;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.default_hot_mode = glow_eHotMode_SingleObject;
  else
    grow_attr.default_hot_mode = glow_eHotMode_TraceAction;
  mask |= grow_eAttr_grafcet_con_delta;
  grow_attr.grafcet_con_delta = 0.6;
  mask |= grow_eAttr_enable_bg_pixmap;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.enable_bg_pixmap = 0;
  else
    grow_attr.enable_bg_pixmap = 1; 
  mask |= grow_eAttr_double_buffer_on;
  if ( ((Graph *)graph)->mode == graph_eMode_Development)
    grow_attr.double_buffer_on = 1;
  else
    grow_attr.double_buffer_on = 0; 
  grow_SetAttributes( ctx, &grow_attr, mask); 
  grow_SetCtxUserData( ctx, graph);
  grow_SetMoveRestrictions( ctx, glow_eMoveRestriction_Disable, 0, 0, NULL);

  grow_DisableEventAll( ctx);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClick, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB2DoubleClick, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1DoubleClickCtrl, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Click, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1ClickShift, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_SelectClear, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_ObjectDeleted, glow_eEventType_CallBack, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Press, glow_eEventType_RegionSelect, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1PressShift, glow_eEventType_RegionAddSelect, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Press, glow_eEventType_MoveNode, 
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB3Click, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB3Press, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB2Click, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_GrowDynamics, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_CursorMotion, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_SliderMoved, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_SliderMoveStart, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_AnnotationInput, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_InputFocusLost, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_InputFocusGained, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_InputFocusInit, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_HotRequest, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Down, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Up, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB3Down, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_TipText, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Tab, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_ShiftTab, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Escape, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Return, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Left, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Right, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Up, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_Down, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_Key_CtrlAscii, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MenuActivated, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MenuCreate, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MenuDelete, glow_eEventType_CallBack,
	graph_grow_cb);

}

//
// Backcall routine called at creation of the grow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int graph_init_grow_base_cb( GlowCtx *fctx, void *client_data)
{
  Graph *graph = (Graph *) client_data;
  GrowCtx *ctx = (GrowCtx *)fctx;
  GrowCtx *secondary_ctx;

  graph->grow = new GraphGrow( ctx, (void *)graph);
  graph->grow_stack[0] = new GraphGrow( ctx, (void *)graph);
  graph->grow_cnt++;

  graph->grow->grow_setup();

  memcpy( graph->grow_stack[0], graph->grow, sizeof( *graph->grow));

  grow_CreateSecondaryCtx( graph->grow_stack[0]->ctx, &secondary_ctx,
        graph_init_grow_cb, (void *)graph, glow_eCtxType_Grow);

  if ( graph->init_cb)
    (graph->init_cb) (graph->parent_ctx);
  return 1;
}


static int graph_init_grow_cb( GrowCtx *ctx, void *client_data)
{
  Graph *graph = (Graph *) client_data;

  graph->grow_stack[graph->grow_cnt] = new GraphGrow( ctx, (void *)graph);

  graph->grow_stack[graph->grow_cnt]->grow_setup();

  return 1;
}

static void graph_attr_redraw_cb( Attr *attrctx)
{
  Graph *graph = (Graph *) attrctx->parent_ctx;

  if ( attrctx->client_data)
    grow_UpdateObject( graph->grow->ctx, attrctx->object,
		       (grow_sAttrInfo *)attrctx->client_data);
}

static void graph_subgraphattr_redraw_cb( Attr *attrctx)
{
  Graph *graph = (Graph *) attrctx->parent_ctx;
  char 	*argnames;
  int  	*argtypes;
  int  	*arg_cnt;
  char	*code;
  int		size;

  grow_UpdateSubGraph( graph->grow->ctx,
	(grow_sAttrInfo *)attrctx->client_data);

  grow_GetSubGraphDynamic( graph->grow->ctx, &code, &size);
  if ( size)
  {
    grow_RefSubGraphArgNames( graph->grow->ctx, &argnames, &argtypes, &arg_cnt);
    graph->get_argnames( code, argnames, argtypes, arg_cnt);
  }
}

static void graph_graphattr_redraw_cb( Attr *attrctx)
{
  Graph *graph = (Graph *) attrctx->parent_ctx;

  grow_UpdateGraph( graph->grow->ctx,
	(grow_sAttrInfo *)attrctx->client_data);
}

static void graph_attr_close_cb( Attr *attrctx)
{
//  grow_FreeObjectAttrInfo( (grow_sAttrInfo *)attrctx->client_data);
  Graph *graph = (Graph *) attrctx->parent_ctx;

  if ( attrctx->client_data)
    grow_UpdateObject( graph->grow->ctx, attrctx->object,
		       (grow_sAttrInfo *)attrctx->client_data);

  ((Graph *)attrctx->parent_ctx)->attr_list.remove( (void *) attrctx);
  delete attrctx;
}

static void graph_subgraphattr_close_cb( Attr *attrctx)
{
//  grow_FreeSubGraphAttrInfo( (grow_sAttrInfo *)attrctx->client_data);
  ((Graph *)attrctx->parent_ctx)->attr_list.remove( (void *) attrctx);
  delete attrctx;
}

static void graph_graphattr_close_cb( Attr *attrctx)
{
//  grow_FreeGraphAttrInfo( (grow_sAttrInfo *)attrctx->client_data);
  ((Graph *)attrctx->parent_ctx)->attr_list.remove( (void *) attrctx);
  delete attrctx;
}


int Graph::is_modified()
{
  return grow_GetModified( grow->ctx);
}

int Graph::init_trace()
{
  int sts;

  if ( grow_GetModified( grow->ctx))
  {
    message( 'E', "Session is not saved");
    return 0;
  }

  if ( !gdh_init_done)
  {
    sts = gdh_Init("ge");
    if ( EVEN(sts)) return sts;
  }


  if ( !trace_started)
  {
    grow_GetScanTime( grow->ctx, &scan_time, &fast_scan_time, 
		    &animation_scan_time);
    if ( fast_scan_time < animation_scan_time)
      fast_scan_time = animation_scan_time;
    if ( scan_time < animation_scan_time)
      scan_time = animation_scan_time;

    // Set subscription defaults
    int dt = int( scan_time * 1000);
    int tmo = int( MAX(2 * dt, 10000));
    sts = gdh_SetSubscriptionDefaults( dt, tmo);

    grow->grow_trace_setup();

    // Look for object graph
    if ( strcmp( object_name, "") != 0)
      init_object_graph(0);

    sts = grow_TraceInit( grow->ctx, graph_trace_connect_bc, 
		graph_trace_disconnect_bc, graph_trace_scan_bc);

    // Look for object graph
    if ( strcmp( object_name, "") != 0)
      init_object_graph(1);
    trace_started = 1;
    current_mb1_down = 0;

    graph_trace_scan( this);
    grow_InputFocusInitEvent( grow->ctx);
  }
  return 1;
}

void Graph::close_trace( int reload)
{
  char graphname[80];

  if ( trace_started)
  {
    XtRemoveTimeOut( trace_timerid);

    grow_TraceClose( grow->ctx);

    if ( graph_object_close)
      (graph_object_close)( this);

    trace_started = 0;

    if ( reload)
    {
      grow->grow_setup();

      // Refresh the graph by loading it from file

      if ( load_graph_cb)
      {
        get_name( graphname);
        (load_graph_cb)( parent_ctx, graphname);
      }
    }
  }
}

static void graph_trace_scan( Graph *graph)
{
  int time = int( graph->animation_scan_time * 1000);

  if ( graph->trace_started)
  {
    if ( graph->graph_object_scan)
      (graph->graph_object_scan)( graph);

    grow_TraceScan( graph->grow->ctx);

    graph->slow_scan_cnt++;
    if ( graph->slow_scan_cnt >= 
	 int(graph->scan_time / graph->animation_scan_time + 0.5))
      graph->slow_scan_cnt = 0;

    graph->fast_scan_cnt++;
    if ( graph->fast_scan_cnt >= 
	 int(graph->fast_scan_time / graph->animation_scan_time + 0.5))
      graph->fast_scan_cnt = 0;

    graph->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(graph->grow_widget), time,
	(XtTimerCallbackProc)graph_trace_scan, graph);
  }
}

static int graph_trace_connect_bc( grow_tObject object, 
	glow_sTraceData *trace_data)
{
  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  if ( !dyn)
    return 1;

  dyn->connect( object, trace_data);

  return 1;
}

static int graph_trace_disconnect_bc( grow_tObject object)
{
  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  if ( !dyn)
    return 1;

  dyn->disconnect( object);

  return 1;
}


static int graph_trace_scan_bc( grow_tObject object, void *p)
{
  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  if ( !dyn)
    return 1;

  if ( dyn->cycle == glow_eCycle_Inherit)
    return 1;
  if ( dyn->cycle == glow_eCycle_Slow && dyn->graph->slow_scan_cnt != 0 &&
       !(dyn->total_dyn_type & ge_mDynType_Animation))
    return 1;
  if ( dyn->cycle == glow_eCycle_Fast && dyn->graph->fast_scan_cnt != 0 &&
       !(dyn->total_dyn_type & ge_mDynType_Animation))
    return 1;

  dyn->scan( object);

  return 1;
}

//
// Callbacks from grow
//
static int graph_trace_grow_cb( GlowCtx *ctx, glow_tEvent event)
{
  Graph		*graph;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &graph);
  graph->message( ' ', null_str);

  switch ( event->event) {

    case glow_eEvent_ObjectDeleted: {
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowWindow ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTrend ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTable ||
	   grow_GetObjectType( event->object.object) == glow_eObjectType_GrowBar) {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
        delete dyn;
      }
      break;
    }
    case glow_eEvent_MB1Down:
    {
      if ( event->object.object_type != glow_eObjectType_NoObject &&
           (grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowSlider ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowWindow ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTrend ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTable ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowBar))
      {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
        graph->current_mb1_down = event->object.object;

      }
      break;
    }
    case glow_eEvent_MB1Up:
    {
      if ( !graph->current_mb1_down)
        break;
      if ( grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowGroup ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowWindow ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowTrend ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowTable ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowBar)
      {
	GeDyn *dyn;

        grow_GetUserData( graph->current_mb1_down, (void **)&dyn);
	dyn->action( graph->current_mb1_down, event);
        graph->current_mb1_down = 0;

      }
      break;
    }
    case glow_eEvent_MB3Down:
    {
      if ( graph->mode == graph_eMode_Runtime) {
        switch( grow_GetMB3Action( graph->grow->ctx)) {
	  case glow_eMB3Action_Close:
            grow_SetClickSensitivity( graph->grow->ctx, 
				      glow_mSensitivity_MB3Click);
            break;
	  case glow_eMB3Action_PopupMenu:
            grow_SetClickSensitivity( graph->grow->ctx, 
				      glow_mSensitivity_MB3Press);
            break;
	  default:
            ;
        }
      }
      break;
    }
    case glow_eEvent_HotRequest:
    {
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowSlider ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup)
      {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
        if ( graph->is_authorized( dyn->access) &&
	     dyn->get_actiontype( event->object.object)) {
	  if ( dyn->get_actiontype( event->object.object) & ~ge_mActionType_PopupMenu)
	    return int(glow_mHotType_CursorCrossHair);
	  else
	    return int(glow_mHotType_CursorHand);
	}
	else
	  return 0;
      }
      break;
    }
    case glow_eEvent_TipText:
    {
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowSlider ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup)
      {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
      }
      break;
    }
    case glow_eEvent_SliderMoveStart:
    {
      if ( event->object.object_type == glow_eObjectType_NoObject)
      {
        grow_SetMoveRestrictions( graph->grow->ctx, 
		glow_eMoveRestriction_Disable, 0, 0, NULL);
        graph->current_slider = NULL;
      }
      else
      {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);

      }
      break;
    }
    case glow_eEvent_SliderMoved:
    {
      GeDyn *dyn;

      grow_GetUserData( event->object.object, (void **)&dyn);
      dyn->action( event->object.object, event);
      break;
    }
    case glow_eEvent_MB3Click:
    {

      if ( graph->mode == graph_eMode_Runtime)
      {
        switch( grow_GetMB3Action( graph->grow->ctx)) {
	  case glow_eMB3Action_Close:
	  case glow_eMB3Action_Both:
            // Close
            if ( graph->close_cb)
              (graph->close_cb)( graph->parent_ctx);
            break;
	  default:
	    ;
        }
      }
      break;
    }
    case glow_eEvent_MB3Press:
    {
      if ( event->any.type == glow_eEventType_Table) {
	switch( grow_GetMB3Action( graph->grow->ctx)) {
	case glow_eMB3Action_PopupMenu:
	case glow_eMB3Action_Both: {
	  GeDyn *dyn;
	  
	  grow_GetUserData( event->table.object, (void **)&dyn);
	  dyn->action( event->table.object, event);
	  break;
	}
	default: ;
	}
	break;
      }

      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( !( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
              grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup ||
              grow_GetObjectType( event->object.object) == glow_eObjectType_GrowWindow ||
              grow_GetObjectType( event->object.object) == glow_eObjectType_GrowBar ||
              grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTable ||
              grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTrend))
        break;
      if ( graph->mode != graph_eMode_Runtime)
        break;

      switch( grow_GetMB3Action( graph->grow->ctx)) {
      case glow_eMB3Action_PopupMenu:
      case glow_eMB3Action_Both: {
	GeDyn *dyn;

	grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
	break;
      }
      default: ;
      }
    }
    case glow_eEvent_MB1Click:
    {
      GeDyn *dyn;

      if ( event->any.type == glow_eEventType_Table) {
	grow_GetUserData( event->table.object, (void **)&dyn);
	dyn->action( event->table.object, event);
	break;
      }

      if ( event->object.object_type == glow_eObjectType_NoObject ||
	   grow_GetObjectType( event->object.object) != glow_eObjectType_GrowMenu) {
      // Close any open menu, if not click in menu
	glow_sEvent e;
	grow_tObject 	*objectlist;
	int 		object_cnt, cnt;
	int		i;

	e.event = glow_eEvent_MenuDelete;
	e.any.type = glow_eEventType_Menu;
	e.menu.object = 0;
	
	grow_GetObjectList( graph->grow->ctx, &objectlist, &object_cnt);
	for ( i = 0; i < object_cnt; i++) {
	  if ( (grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowNode ||
		grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowGroup) &&
	       (event->object.object_type == glow_eObjectType_NoObject ||
		objectlist[i] != event->object.object)) {
	    grow_GetUserData( objectlist[i], (void **)&dyn);
	    dyn->action( objectlist[i], &e);
	    grow_GetObjectList( graph->grow->ctx, &objectlist, &cnt);
	    if ( cnt != object_cnt)
	      // Something is deleted
	      break;
	  }
	}
      }

      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowWindow ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowBar ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTable ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowTrend)
      {

        grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);

      }
      break;
    }
    case glow_eEvent_MB1DoubleClick:
    {
      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup)
      {
	GeDyn *dyn;

        grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
      }
      break;
    }
    case glow_eEvent_MB1DoubleClickCtrl: {
      break;
    }
    case glow_eEvent_AnnotationInput: {
      GeDyn *dyn;
      
      grow_GetUserData( event->annot_input.object, (void **)&dyn);
      dyn->action( event->annot_input.object, event);
      
      break;
    }
    case glow_eEvent_InputFocusLost:
    case glow_eEvent_InputFocusGained: {
      GeDyn *dyn;

      grow_GetUserData( event->object.object, (void **)&dyn);
      dyn->action( event->object.object, event);

      break;
    }
    case glow_eEvent_Key_Tab:
    case glow_eEvent_Key_ShiftTab:
    case glow_eEvent_Key_Escape:
    case glow_eEvent_Key_Left:
    case glow_eEvent_Key_Right:
    case glow_eEvent_Key_Up:
    case glow_eEvent_Key_Down:
    case glow_eEvent_Key_Return:
    case glow_eEvent_InputFocusInit: {
      GeDyn *dyn;

      if ( event->object.object_type == glow_eObjectType_NoObject) {
	grow_tObject 	*objectlist;
	int 		object_cnt;
	int		i;
	
	grow_GetObjectList( graph->grow->ctx, &objectlist, &object_cnt);
	for ( i = 0; i < object_cnt; i++) {
	  if ( grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowNode ||
	       grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowSlider ||
	       grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowGroup) {
	    grow_GetUserData( objectlist[i], (void **)&dyn);
	    dyn->action( objectlist[i], event);
	  }
	}
      }
      else {
	grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
      }
      break;
    }

    case glow_eEvent_Key_CtrlAscii: {
      if ( event->object.object_type != glow_eObjectType_NoObject) {
	GeDyn *dyn;

	grow_GetUserData( event->object.object, (void **)&dyn);
	dyn->action( event->object.object, event);
      }
    }

    case glow_eEvent_MenuActivated:
    case glow_eEvent_MenuCreate:
    case glow_eEvent_MenuDelete: {
      grow_tObject 	*objectlist;
      int 		object_cnt;
      int		new_object_cnt;
      int		i;
      GeDyn		*dyn;

      grow_GetObjectList( graph->grow->ctx, &objectlist, &object_cnt);
      for ( i = 0; i < object_cnt; i++) {
	if ( grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowNode ||
	     grow_GetObjectType( objectlist[i]) == glow_eObjectType_GrowGroup) {

	  
	  grow_GetUserData( objectlist[i], (void **)&dyn);
	  dyn->action( objectlist[i], event);

	  // Check if anything is deleted
	  grow_GetObjectList( graph->grow->ctx, &objectlist, &new_object_cnt);
	  if ( new_object_cnt != object_cnt)
	    break;
	}
      }
      break;
    }

    case glow_eEvent_Translate: {
      char new_text[200];
      int sts;

      sts = Lng::translate( event->translate.text, new_text);
      if ( sts)
        event->translate.new_text = new_text;
      return sts;
    }
    case glow_eEvent_GrowDynamics:
      graph->exec_dynamic( event->dynamics.object, event->dynamics.code,
		event->dynamics.dynamic_type);
      break;
    default:
      ;
  }
  return 1;
}

void Graph::confirm_ok( grow_tObject object)
{
  GeDyn *dyn;
  glow_sEvent event;

  grow_GetUserData( object, (void **)&dyn);
  event.event = glow_eEvent_MB1Click;
  if ( dyn->total_action_type & ge_mActionType_ValueInput)
    dyn->change_value( object, confirm_text);
  else
    dyn->confirmed_action( object, &event);

}

void Graph::connect( grow_tObject object, char *attr_name, int second)
{
  GeDyn *dyn;

  grow_GetUserData( object, (void **)&dyn);
  if ( dyn)
    dyn->set_attribute( object, attr_name, second);
  else
    message( 'E', "No dynamics for this object");

}

int Graph::set_object_focus( char *name, int empty)
{
  int  dyn_type;
  int  action_type;
  int  sts;
  grow_tObject object;
  GeDyn *dyn;

  if ( !change_value_cb)
    return 0;

  sts = grow_FindObjectByName( grow->ctx, name, &object);
  if ( EVEN(sts)) return GE__OBJNOTFOUND;

  grow_GetUserData( object, (void **)&dyn);
  if ( !dyn)
    return 0;

  dyn_type = dyn->get_dyntype( object);
  action_type = dyn->get_actiontype( object);

  if ( !is_authorized( dyn->access))
    return GE__NOACCESS;

  if ( action_type & ge_mActionType_InputFocus || action_type & ge_mActionType_ValueInput)
    grow_SetObjectInputFocus( object, 1);

  return GE__SUCCESS;
}

static void graph_remove_space( char *out_str, char *in_str)
{
  char *s;

  s = in_str;
  // Find first character not space or tab
  while ( *s)
  {
    if ( !(*s == 9 || *s == 32)) 
      break;
    s++;
  }
  strcpy( out_str, s);
  //
  s = out_str + strlen(out_str);
  s--;
  while( s >= out_str)
  {
    if ( !(*s == 9 || *s == 32)) 
      break;
    s--;
  }
  s++;
  *s = 0;
}

void Graph::get_command( char *in, char *out)
{
  char *s, *s0, *t0;

  if ( strcmp( object_name, "") == 0) {
    strcpy( out, in);
    return;
  }

  t0 = out;
  s0 = in;
  while ( (s = strstr( s0, "$object"))) {
    strncpy( t0, s0, s-s0);
    t0 += s - s0;
    strcpy( t0, object_name); 
    t0 += strlen(object_name);
    s0 = s + strlen("$object");
  }
  strcpy( t0, s0);
}

graph_eDatabase Graph::parse_attr_name( char *name, char *parsed_name, 
	int *inverted, int *type, int *size, int *elem)
{
  char str[200];
  char str1[200];
  char *s, *s1;
  int elements;

  graph_remove_space( str, name);
  
  if ( (s = strstr( str, "$user"))) {
    if ( (s = strchr( str, '#'))) {
      if ( strcmp( s, "##Float32") == 0)
        *type = pwr_eType_Float32;
      else if ( strcmp( s, "##Float64") == 0)
        *type = pwr_eType_Float64;
      else if ( strcmp( s, "##Int32") == 0)
        *type = pwr_eType_Int32;
      else if ( strcmp( s, "##Boolean") == 0)
        *type = pwr_eType_Boolean;
      else
        *type = pwr_eType_String;
      *s = 0;
    }
    if ( str[0] == '!') {
      *inverted = 1;
      graph_remove_space( str, &str[1]);
    }
    else
      *inverted = 0;
    strcpy( parsed_name, str);

    return graph_eDatabase_User;
  }
  if ( (s = strstr( str, "$local."))) {
    strcpy( parsed_name, s + strlen("$local."));
    if ( (s = strchr( parsed_name, '#'))) {
      if ( strcmp( s, "##Float32") == 0)
        *type = pwr_eType_Float32;
      else if ( strcmp( s, "##Float64") == 0)
        *type = pwr_eType_Float64;
      else if ( strcmp( s, "##Int32") == 0)
        *type = pwr_eType_Int32;
      else if ( strcmp( s, "##Boolean") == 0)
        *type = pwr_eType_Boolean;
      else
        *type = pwr_eType_String;
      *s = 0;
    }
    if ( str[0] == '!') {
      *inverted = 1;
      graph_remove_space( str, &str[1]);
    }
    else
      *inverted = 0;

    return graph_eDatabase_Local;
  }

  if ( (s = strstr( str, "$object"))) {
    strcpy( str1, s + strlen("$object"));
    strcpy( s, object_name);
    strcat( str, str1);
  }

  if ( (s = strstr( str, "$node"))) {
    char nodename[80];
    pwr_tOid oid;
    pwr_tStatus sts;

    sts = gdh_GetNodeObject( 0, &oid);
    if ( ODD(sts)) {
      sts = gdh_ObjidToName( oid, nodename, sizeof(nodename), cdh_mName_pathStrict);
      if ( ODD(sts)) {
	strcpy( str1, s + strlen("$node"));
	strcpy( s, nodename);
	strcat( str, str1);
      }
    }
  }

  if ( (s = strstr( str, "##")))
    string_to_type( s + 2, (pwr_eType *)type, size, &elements);

  if ( (s = strchr( str, '#'))) {
    *s = 0;
    if ( (s1 = strchr( s+1, '[')))
      strcat( str, s1);
  }

  if ( str[0] == '!') {
    *inverted = 1;
    graph_remove_space( str, &str[1]);
    strcpy( parsed_name, str);
  }
  else {
    *inverted = 0;
    strcpy( parsed_name, str);
  }
  if ( elem)
    *elem = elements;
  return graph_eDatabase_Gdh;
}

int Graph::type_to_string( pwr_eType type, char *type_buf, int  *size)
{
  int i;


  for (i = 0; i < int(sizeof(graph_type_table)/sizeof(graph_type_table[0])); i++)
  {
    if ( graph_type_table[i].Type == type)
    {
      strcpy(type_buf, graph_type_table[i].TypeStr);
      if (size)
        *size = graph_type_table[i].Size;
      return 1;
    }
  }
    
  if (type == pwr_eType_String)
  {
    strcpy(type_buf, "String");
    if (size)
      *size = 1; /* This is not the real size */
    return 1;
  }
  return 0;
}

void  Graph::string_to_type( char *type_str, pwr_eType *type, 
		int *size, int *elements)
{
  int	    		i;
  int			found;
  char			str[80];
  char			table_str[20];
  char			*s, *s1;
  cdh_ToUpper( str, type_str);

  // Check if there is a array size
  if (( s = strchr( str, '#'))) {
    if ( !( s1 = strchr( s+1, '[')))
      *elements = atoi(s + 1);
    else
      *elements = 1;
    *s = 0;
  }
  else
    *elements = 1;

  // Default to float
  *type = (pwr_eType) 0;
  *size = 4;

  found = 0;
  for ( i = 0; i < int(sizeof(graph_type_table)/sizeof(graph_type_table[0])); i++)
  {
    cdh_ToUpper( table_str, (char *)graph_type_table[i].TypeStr);
    if ( strcmp( table_str, str) == 0)
    {
      *size = graph_type_table[i].Size;
      *type = graph_type_table[i].Type;
      found = 1;
      break;
    }
  }

  if ( !found && strncmp("STRING", str, 6) == 0) 
  {
    *type = pwr_eType_String;
    if ( *(str+6) == 0)
      *size = 80;
    else
      *size = atoi( str + 6);
  }

  if ( *elements > 1)
    *size *= *elements;
} 

int Graph::ref_object_info( glow_eCycle cycle, char *name, void **data,
			    pwr_tSubid *subid, unsigned int size)
{
  int dt;
  if ( cycle == glow_eCycle_Fast)
    dt = int( fast_scan_time * 1000);
  else
    dt = int( scan_time * 1000);
  int tmo = int( MAX(2 * dt, 10000));
  gdh_SetSubscriptionDefaults( dt, tmo);

  return gdh_RefObjectInfo( name, data, subid, size);
}

void Graph::create_trend( grow_tObject *object, double x, double y,
			  unsigned int dyn_type)
{
  double width = 7;
  double height = 5;
  GeDyn *dyn;
  glow_sTrendInfo info;

  grow_CreateGrowTrend( grow->ctx, "", 
			    x, y, width, height,
			    glow_eDrawType_Color37,
			    1, glow_mDisplayLevel_1, 1, 1,
			    glow_eDrawType_Color40, NULL, 
			    object);
  dyn = new GeDyn( this);
  dyn->dyn_type = dyn->total_dyn_type = (ge_mDynType) dyn_type;
  dyn->update_elements();
  grow_SetUserData( *object, (void *)dyn);

  info.no_of_points = 100;
  info.scan_time = 0.5;
  info.fill_curve = 0;
  info.curve_width = 1;
  info.horizontal_lines = 4;
  info.vertical_lines = 4;
  info.min_value[0] = 0;
  info.min_value[1] = 0;
  info.max_value[0] = 100;
  info.max_value[1] = 100;
  info.curve_drawtype[0] = glow_eDrawType_Color145;
  info.curve_drawtype[1] = glow_eDrawType_Color295;
  info.curve_fill_drawtype[0] = glow_eDrawType_Color139;
  info.curve_fill_drawtype[1] = glow_eDrawType_Color289;
  grow_SetTrendInfo( *object, &info);

  grow_Redraw( grow->ctx);
}

void Graph::create_bar( grow_tObject *object, double x, double y)
{
  double width = 0.5;
  double height = 5;
  GeDyn *dyn;
  glow_sBarInfo info;

  grow_CreateGrowBar( grow->ctx, "", 
			    x, y, width, height,
			    glow_eDrawType_Line,
			    1, glow_mDisplayLevel_1, 1, 1,
			    glow_eDrawType_Color40, NULL, 
			    object);
  dyn = new GeDyn( this);
  dyn->dyn_type = dyn->total_dyn_type = ge_mDynType_Bar;
  dyn->update_elements();
  grow_SetUserData( *object, (void *)dyn);

  info.bar_drawtype = glow_eDrawType_Color147;
  info.bar_bordercolor = glow_eDrawType_Color145;
  info.bar_borderwidth = 1;
  info.max_value = 100;
  info.min_value = 0;
  grow_SetBarInfo( *object, &info);
  grow_Redraw( grow->ctx);
}

void Graph::create_window( grow_tObject *object, double x, double y)
{
  double width = 8;
  double height = 6;
  GeDyn *dyn;

  grow_CreateGrowWindow( grow->ctx, "", 
			    x, y, width, height,
			    glow_eDrawType_Line,
			    1, glow_mDisplayLevel_1, NULL, 
			    object);
  dyn = new GeDyn( this);
  dyn->update_elements();
  grow_SetUserData( *object, (void *)dyn);

  grow_Redraw( grow->ctx);
}

void Graph::create_table( grow_tObject *object, double x, double y)
{
  double width = 8;
  double height = 6;
  GeDyn *dyn;

  grow_CreateGrowTable( grow->ctx, "", 
			    x, y, width, height,
			    glow_eDrawType_Line,
			    1, 1, glow_eDrawType_Color33, glow_mDisplayLevel_1, NULL, 
			    object);
  dyn = new GeDyn( this);
  dyn->dyn_type = dyn->total_dyn_type = ge_mDynType_Table;
  dyn->update_elements();
  grow_SetUserData( *object, (void *)dyn);

  grow_Redraw( grow->ctx);
}

void Graph::create_folder( grow_tObject *object, double x, double y)
{
  double width = 8;
  double height = 6;
  GeDyn *dyn;

  grow_CreateGrowFolder( grow->ctx, "", 
			    x, y, width, height,
			    glow_eDrawType_Line, 1, 
			    glow_eDrawType_Color22, glow_eDrawType_Color25,
			    glow_mDisplayLevel_1, NULL, object);
  dyn = new GeDyn( this);
  dyn->update_elements();
  grow_SetUserData( *object, (void *)dyn);

  grow_Redraw( grow->ctx);
}

void Graph::create_axis( grow_tObject *object, double x, double y)
{
  double width = 1.2;
  double height = 5;
  glow_sAxisInfo info;

  grow_CreateGrowAxis( grow->ctx, "", 
		       x, y, x + width, y + height,
		       glow_eDrawType_Line, 1, 1,
		       glow_eDrawType_TextHelvetica, NULL, object);

  info.max_value = 100;
  info.min_value = 0;
  info.lines = 11;
  info.longquotient = 2;
  info.valuequotient = 2;
  strcpy( info.format, "%3.0f");
  grow_SetAxisInfo( *object, &info);
  grow_Redraw( grow->ctx);
}

void Graph::swap( int mode)
{


  if ( mode == 0) {
    // Swap starting
    if ( trace_started) {
      XtRemoveTimeOut( trace_timerid);
      grow_TraceClose( grow->ctx);
      trace_started = 0;
    }
  }
  else if ( mode == 1) {
    // Swap done
    if ( !trace_started) {
      grow_TraceInit( grow->ctx, graph_trace_connect_bc, 
			    graph_trace_disconnect_bc, graph_trace_scan_bc);
      trace_started = 1;
      graph_trace_scan( this);
    }
  }
}

void GraphApplList::insert( void *ctx)
{
  GraphApplList *appl_p = new GraphApplList( 0, ctx);  
  if ( next)
    next->prev = appl_p;
  appl_p->next = next;
  next = appl_p;
}

void GraphApplList::remove( void *ctx)
{
  GraphApplList *appl_p;

  for ( appl_p = next; appl_p; appl_p = appl_p->next)
  {
    if ( appl_p->ctx == ctx)
    {
      if ( !appl_p->prev)
      {
        next = appl_p->next;
        if ( appl_p->next)
          appl_p->next->prev = NULL;
      }
      else
      {
        appl_p->prev->next = appl_p->next;
        if ( appl_p->next)
          appl_p->next->prev = appl_p->prev;
      }
      delete appl_p;
      break;
    }
  }
}

int GraphApplList::get_first( void **ctx)
{
  if ( next)
  {
    *ctx = next->ctx;
    return 1;
  }
  else
    return 0;
}

//
// Convert attribute string to value
//
int  graph_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size)
{
  int		sts;

  switch ( type_id )
  {
    case pwr_eType_Boolean:
    {
      if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      if ( *(pwr_tBoolean *)buffer_ptr > 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Float32:
    {
      if ( sscanf( value_str, "%f", (float *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Float64:
    {
      pwr_tFloat32 f;
      pwr_tFloat64 d;
      if ( sscanf( value_str, "%f", &f) != 1)
        return GE__INPUT_SYNTAX;
      d = f;
      memcpy( buffer_ptr, (char *) &d, sizeof(d));

      break;
    }
    case pwr_eType_Char:
    {
      if ( sscanf( value_str, "%c", (char *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Int8:
    {
      pwr_tInt8 	i8;
      pwr_tInt16	i16;
      if ( sscanf( value_str, "%hd", &i16) != 1)
        return GE__INPUT_SYNTAX;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_Int16:
    {
      if ( sscanf( value_str, "%hd", (short *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_Int32:
    {
      if ( sscanf( value_str, "%d", (int *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_UInt8:
    {
      pwr_tUInt8 	i8;
      pwr_tUInt16	i16;
      if ( sscanf( value_str, "%hu", &i16) != 1)
        return GE__INPUT_SYNTAX;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_UInt16:
    {
      if ( sscanf( value_str, "%hu", (unsigned short *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_UInt32:
    {
      if ( sscanf( value_str, "%lu", (unsigned long *)buffer_ptr) != 1)
        return GE__INPUT_SYNTAX;
      break;
    }
    case pwr_eType_String:
    {
      if ( (int) strlen( value_str) >= attr_size)
        return GE__STRINGTOLONG;
      strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
      break;
    }
    case pwr_eType_ObjDId:
    {
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	memcpy( buffer_ptr, &objid, sizeof(objid));
      break;
    }
    case pwr_eType_ClassId:
    {
      pwr_tClassId	classid;
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	classid = cdh_ClassObjidToId( objid);
      memcpy( buffer_ptr, (char *) &classid, sizeof(classid));
      break;
    }
    case pwr_eType_TypeId:
    {
      pwr_tTypeId	val_typeid;
      pwr_tObjid	objid;

      sts = gdh_NameToObjid ( value_str, &objid);
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	val_typeid = cdh_TypeObjidToId( objid);
      memcpy( buffer_ptr, (char *) &val_typeid, sizeof(val_typeid));
      break;
    }
    case pwr_eType_ObjectIx:
    {
      pwr_tObjectIx	objectix;

      sts = cdh_StringToObjectIx( value_str, &objectix); 
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &objectix, sizeof(objectix));
      break;
    }
    case pwr_eType_VolumeId:
    {
      pwr_tVolumeId	volumeid;

      sts = cdh_StringToVolumeId( value_str, &volumeid); 
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &volumeid, sizeof(volumeid));
      break;
    }
    case pwr_eType_RefId:
    {
      pwr_tRefId	subid;

      sts = cdh_StringToSubid( value_str, &subid);
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	memcpy( buffer_ptr, (char *) &subid, sizeof(subid));
      break;
    }
    case pwr_eType_AttrRef:
    {
      pwr_sAttrRef	attrref;

      sts = gdh_NameToAttrref ( pwr_cNObjid, value_str, &attrref);
      if (EVEN(sts)) return GE__OBJNOTFOUND;
  	memcpy( buffer_ptr, &attrref, sizeof(attrref));
      break;
    }
    case pwr_eType_Time:
    {
      pwr_tTime	time;

      sts = time_AsciiToA( value_str, &time);
      if (EVEN(sts)) return GE__INPUT_SYNTAX;
  	memcpy( buffer_ptr, (char *) &time, sizeof(time));
      break;
    }
    case pwr_eType_DeltaTime:
    {
      pwr_tDeltaTime deltatime;

      sts = time_AsciiToD( value_str, &deltatime);
      if (EVEN(sts)) return GE__INPUT_SYNTAX;
  	memcpy( buffer_ptr, (char *) &deltatime, sizeof(deltatime));
      break;
    }
  }
  return 1;
}

GraphRecallBuff::~GraphRecallBuff()
{
  for ( int i = 0; i < cnt; i++)
    delete buff[i];
}
void GraphRecallBuff::insert( GeDyn *data, char *data_key, grow_tObject object)
{
  int i;
  char new_key[80];

  if ( !data_key)
    strcpy( new_key, "");
  else
    strcpy( new_key, data_key);

#if 0
  if ( strcmp( key[0], new_key) == 0 && memcmp( data, &buff[0], sizeof(buff[0])) == 0)
    // Identical with last one
  return;
#endif

  if ( cnt == size)
    delete buff[size-1];
  for ( i = cnt - 1; i > 0; i--) {
    buff[i] = buff[i-1];
    strcpy( key[i], key[i-1]);
  }
  buff[0] = new GeDyn(*data);
  buff[0]->unset_inherit( object);
  strcpy( key[0], new_key);
  cnt++;
  if ( cnt > size)
    cnt = size;
}

int GraphRecallBuff::get( GeDyn **data, int idx)
{
  if ( idx >= cnt)
    return 0;
  *data = new GeDyn( *buff[ idx]);
  return 1;
}

int GraphRecallBuff::get( GeDyn **data, char *k)
{
  for ( int i = 0; i < cnt; i++) {
    if ( strcmp( k, key[i]) == 0) {
      *data = new GeDyn( *buff[i]);
      return 1;
    }
  }
  return 0;
}


