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
#include "ge_msg.h"

extern "C" {
#include "co_mrm_util.h"
}
#include "co_lng.h"

#if defined OS_VMS
# pragma message disable (INTSIGNCHANGE)
#endif
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

// Until xtt_menu.h i unavailable...
#define xmenu_mUtility_Ge        (1 << 5)
#define xmenu_eItemType_Object   1

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
    {"AttrRef", pwr_eType_AttrRef,  sizeof(pwr_sAttrRef)}
    };

static char null_str[] = "";

static void graph_remove_space( char *out_str, char *in_str);
static int graph_init_grow_cb( GrowCtx *ctx, void *client_data);
static int graph_init_grow_base_cb( GlowCtx *fctx, void *client_data);
static void graph_attr_redraw_cb( Attr *attrctx);
static void graph_attr_close_cb( Attr *attrctx);
static void graph_attr_store_cb( void *g, grow_tObject object);
static int graph_attr_recall_cb( void *g, grow_tObject object, int idx, 
				 glow_sTraceData **old);
static int graph_attr_set_data_cb( void *g, grow_tObject object, 
				 glow_sTraceData *data);
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
static int graph_reconfigure_attr_cb( void *g, grow_tObject object,
	attr_sItem **itemlist, int *itemlist_cnt, void **client_data);
static int  graph_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size);
static int graph_get_typeid( graph_sTraceData *data);
static int graph_grow_cb( GlowCtx *ctx, glow_tEvent event);


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
	get_current_objects_cb(NULL), popup_menu_cb(NULL),
	linewidth(1), textsize(0), textbold(0), border_color(1), fill_color(1),
	fill(0), border(1),
	grid_size_x(1), grid_size_y(1), con_type(glow_eConType_Straight),
	con_corner(glow_eCorner_Right),
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
	default_access(xn_default_access)
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
void Graph::save( char *filename)
{
  char fname[120];

  get_filename( filename, fname);
  grow_Save( grow->ctx, fname);
  grow_SetModified( grow->ctx, 0);
}

//
//  Save as subgraph
//
void Graph::save_subgraph( char *filename)
{
  char fname[120];

  get_filename( filename, fname);
  grow_SaveSubGraph( grow->ctx, fname);
}

//
//  Open
//
void Graph::open( char *filename)
{
  int sts;
  char fname[120];

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
void Graph::set_mode( grow_eMode mode)
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
  glow_sTraceData data;

  sts = grow_GroupSelect( grow->ctx, &group, last_group_name);
  if ( EVEN(sts)) return sts;

  grow_SetModified( grow->ctx, 1);

  if ( strcmp( last_group_name, "") != 0) {  
    // Try to recover dynamics
    sts = recall.get( &data, last_group_name);
    if ( ODD(sts)) {
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
  glow_sTraceData data;

  sts = recall.get( &data, name);
  if ( EVEN(sts)) return sts;

  grow_SetTraceAttr( object, &data);
  return 1;
}

int Graph::ungroup_select( int force)
{
  grow_tObject 	*sel_list;
  int		sel_count;
  int           i;
  glow_sTraceData *trace_data;
  char          name[80];

  if ( !force) {
    grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
    for ( i = 0; i < sel_count; i++) {
      if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup)
      {
        grow_GetTraceAttr( sel_list[i], &trace_data);
        if ( trace_data->attr_type != 0) {
          return GE__GROUPDYNDATA;
        }
      }
    }
  }
  else {
    grow_GetSelectList( grow->ctx, &sel_list, &sel_count);
    for ( i = 0; i < sel_count; i++) {
      if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup)
      {
        grow_GetObjectName( sel_list[i], name);
        grow_GetTraceAttr( sel_list[i], &trace_data);
        if ( trace_data->attr_type != 0) {
          recall.insert( trace_data, name);
        }
      }
    }
  }
  grow_SetModified( grow->ctx, 1);

  return grow_UngroupSelect( grow->ctx);
}

void Graph::set_inputfocus( int focus)
{
//  grow_SetInputFocus( grow->ctx);
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
    XtCallAcceptFocus( grow_widget, CurrentTime);
    XtSetArg(args[0], XmNborderColor, fg);
    XtSetValues( form_widget, args, 1);
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
  glow_sTraceData *trace_data;
  char		buf[200];
  int		sts;
  graph_sTraceData *data;
  char		parsed_name[120];
  int		inverted;
  int		attr_type, attr_size;

  // Check that object exist
  if ( !grow_FindObject( grow->ctx, object))
    return;

  grow_GetTraceAttr( object, &trace_data);
  parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);

  grow_GetUserData( object, (void **)&data);
  if ( !data)
    return;
  if ( !data->annot_size )
  {
    pwr_sAttrRef 	ar;
    pwr_tTypeId 	a_type_id;
    unsigned int 	a_size;
    unsigned int 	a_offset;
    unsigned int 	a_dim;

    sts = gdh_NameToAttrref ( pwr_cNObjid, parsed_name, &ar);
    if ( EVEN(sts)) return;
    sts = gdh_GetAttributeCharAttrref( &ar, &a_type_id, &a_size,
		&a_offset, &a_dim);
    if ( EVEN(sts)) return;
    data->annot_typeid = a_type_id;
    data->annot_size = a_size;
  }
  sts = graph_attr_string_to_value( data->annot_typeid, text,
	(void *)&buf, sizeof( buf), sizeof(buf));
  if ( EVEN(sts)) return;

  if ( strcmp( trace_data->data[4], "") != 0)
  {
    // Max value is supplied
    float	maxvalue;
    int 	max_exceeded = 0;
    sts = sscanf( trace_data->data[4], "%f", &maxvalue);
    if ( sts == 1)
    {
      switch ( data->annot_typeid) 
      {
        case pwr_eType_Float32:
          if ( float( *(pwr_tFloat32 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_Float64:
          if ( float( *(pwr_tFloat64 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_Int32:
          if ( float( *(pwr_tInt32 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_UInt32:
          if ( float( *(pwr_tUInt32 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_Int16:
          if ( float( *(pwr_tInt16 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_UInt16:
          if ( float( *(pwr_tUInt16 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_Int8:
          if ( float( *(pwr_tInt8 *) buf) > maxvalue) max_exceeded = 1;
          break;
        case pwr_eType_UInt8:
          if ( float( *(pwr_tUInt8 *) buf) > maxvalue) max_exceeded = 1;
          break;
      }
    }    
    if ( max_exceeded)
    {
      if ( message_dialog_cb)
        (message_dialog_cb)( parent_ctx, "Maxvalue exceeded");
      return;
    }
  }
  if ( strcmp( trace_data->data[3], "") != 0)
  {
    // Min value is supplied
    float	minvalue;
    int 	min_exceeded = 0;
    sts = sscanf( trace_data->data[3], "%f", &minvalue);
    if ( sts == 1)
    {
      switch ( data->annot_typeid) 
      {
        case pwr_eType_Float32:
          if ( float( *(pwr_tFloat32 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_Float64:
          if ( float( *(pwr_tFloat64 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_Int32:
          if ( float( *(pwr_tInt32 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_UInt32:
          if ( float( *(pwr_tUInt32 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_Int16:
          if ( float( *(pwr_tInt16 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_UInt16:
          if ( float( *(pwr_tUInt16 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_Int8:
          if ( float( *(pwr_tInt8 *) buf) < minvalue) min_exceeded = 1;
          break;
        case pwr_eType_UInt8:
          if ( float( *(pwr_tUInt8 *) buf) < minvalue) min_exceeded = 1;
          break;
      }
    }    
    if ( min_exceeded)
    {
      if ( message_dialog_cb)
        (message_dialog_cb)( parent_ctx, "Value below minvalue");
      return;
    }
  }

  if ( data->db[0] == graph_eDatabase_Local)
    sts = localdb_set_value( parsed_name, &buf, data->annot_size);
  else
    sts = gdh_SetObjectInfo( parsed_name, &buf, data->annot_size);
  if ( EVEN(sts)) printf("AnnotationInput error: %s\n", trace_data->data[0]);
}

void Graph::set_select_fill_color()
{
  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigFillColor( grow->ctx, get_fill_drawtype());
}

void Graph::set_select_border_color()
{
  glow_eDrawType drawtype = get_border_drawtype();

  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigBorderColor( grow->ctx, drawtype);

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

void Graph::set_select_color_tone( glow_eDrawTone tone)
{
  grow_DisableHighlight( grow->ctx);
  grow_SetSelectOrigColorTone( grow->ctx, tone);
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

void Graph::set_select_fill( int fill)
{
  grow_SetSelectFill( grow->ctx, fill);
}

void Graph::set_select_border( int border)
{
  grow_SetSelectBorder( grow->ctx, border);
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
  glow_eDrawType fill_color, border_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color);
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
  glow_eDrawType fill_color, border_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color);

  return border_color;
}

glow_eDrawType Graph::get_fill_drawtype()
{
  glow_eDrawType fill_color, border_color;

  (get_current_colors_cb)( parent_ctx, &fill_color, &border_color);

  return fill_color;
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
  static attr_sItem	items[20];
  int			i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  graph_eTrace		trace_type;
  glow_sTraceData	*trace_data;

  if ( grow_GetObjectType( object) == glow_eObjectType_GrowNode ||
       grow_GetObjectType( object) == glow_eObjectType_GrowGroup)
  {
    grow_GetTraceAttr( object, &trace_data);
    trace_type = (graph_eTrace) trace_data->attr_type;

    if ( trace_type == graph_eTrace_Inherit)
      grow_GetObjectClassTraceType( object, (glow_eTraceType *) &trace_type);
    switch ( trace_type)
    {
      case graph_eTrace_No:
      case graph_eTrace_Inherit:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"",
					"TraceRefObject",	"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Dig:
      case graph_eTrace_DigBorder:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"InvisAttr",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Invisible:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigWithCommand:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"InvisAttr",
					"TraceData3",		"",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigWithText:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"LowText",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"A1",			"Text",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigWithError:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"WarningAttr",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigTwo:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"Attr2",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"Color2",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigTwoWithCommand:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"Attr2",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"Color2",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigWithErrorAndCommand:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"WarningAttr",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowColor",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigTone:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"InvisAttr",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigToneWithCommand:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"InvisAttr",
					"TraceData3",		"",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigToneWithError:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"WarnAttr",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigToneWithErrorAndCommand:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"WarnAttr",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigToneTwo:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"Attr2",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"Tone2",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigToneTwoWithCommand:
      {
        char transtab[][32] = {		"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ErrorAttr",
					"TraceData3",		"Attr2",
					"TraceData4",		"CmdClick",
					"TraceData5",		"CmdDoubleClick",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"Tone2",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Annot:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"Format",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_AnnotInput:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"Format",
					"TraceData3",		"",
					"TraceData4",		"MinValue",
					"TraceData5",		"MaxValue",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_AnnotWithTone:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"Format",
					"TraceData3",		"ToneAttr",
					"TraceData4",		"InvisAttr",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_AnnotInputWithTone:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"Format",
					"TraceData3",		"ToneAttr",
					"TraceData4",		"MinValue",
					"TraceData5",		"MaxValue",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Rotate:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"x0",
					"TraceData3",		"y0",
					"TraceData4",		"ToneAttr",
					"TraceData5",		"InvisAttr",
					"TraceData6",		"Factor",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Move:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"XAttr",
					"TraceData2",		"YAttr",
					"TraceData3",		"ToneAttr",
					"TraceData4",		"InvisAttr",
					"TraceData5",		"Factor",
					"TraceData6",		"XOffset",
					"TraceData7",		"YOffset",
					"TraceData8",		"ScaleXAttr",
					"TraceData9",		"ScaleYAttr",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_AnalogShift:
      case graph_eTrace_DigShift:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"ShiftAttr",
					"TraceData2",		"ToneAttr",
					"TraceData3",		"InvisAttr",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Animation:
      case graph_eTrace_AnimationForwBack:
      case graph_eTrace_DigAnimation:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"AnimationAttr",
					"TraceData2",		"ToneAttr",
					"TraceData3",		"InvisAttr",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_SetDig:
      case graph_eTrace_ResetDig:
      case graph_eTrace_ToggleDig:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_RadioButton:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_DigShiftWithToggleDig:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"ToggleAttr",
					"TraceData2",		"ShiftAttr",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_Command:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Command",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_SetDigConfirm:
      case graph_eTrace_ResetDigConfirm:
      case graph_eTrace_ToggleDigConfirm:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ConfirmText",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_SetDigConfirmWithTone:
      case graph_eTrace_ResetDigConfirmWithTone:
      case graph_eTrace_ToggleDigConfirmWithTone:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"ConfirmText",
					"TraceData3",		"ToneAttr",
					"TraceData4",		"InvisAttr",
					"TraceData5",		"LowText",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_SetDigWithTone:
      case graph_eTrace_ResetDigWithTone:
      case graph_eTrace_ToggleDigWithTone:
      case graph_eTrace_StoDigWithTone:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"ToneAttr",
					"TraceData4",		"InvisAttr",
					"TraceData5",		"LowText",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"LowTone",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_CommandConfirm:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Command",
					"TraceData2",		"ConfirmText",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      case graph_eTrace_IncrAnalog:
      {
        char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"Increment",
					"TraceData3",		"MinValue",
					"TraceData4",		"MaxValue",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"Type",
					"TraceColor",		"",
					"TraceColor2",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"",
					"TraceRefObject",	"RefObject",
					"A1",			"Text",
					"Dynamic",		"",
					""};
        grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
        break;
      }
      default:
        grow_GetObjectAttrInfo( object, NULL, &grow_info, &grow_info_cnt);
    }
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowBar)
  {
    char transtab[][32] = {	 	"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"",
					"TraceColor",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"MaxValue",		"MaxValue",
					"MinValue",		"MinValue",
					"BarValue",		"BarValue",
					"BarColor",		"BarColor",
					"BarBorderColor",	"BarBorderColor",
					"BarBorderWidth",	"BarBorderWidth",
					"Dynamic",		"Dynamic",
					""};
    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowTrend)
  {
    char transtab[][32] = {	 	"TraceData1", 		"Attribute1",
					"TraceData2",		"Attribute2",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"",
					"TraceColor",		"",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"NoOfPoints",		"NoOfPoints",
					"ScanTime",		"ScanTime",
					"CurveWidth",		"CurveLineWidth",
					"FillCurve",		"FillCurve",
					"HorizontalLines",	"HorizontalLines",
					"VerticalLines",	"VerticalLines",
					"MaxValue1",		"MaxValue1",
					"MinValue1",		"MinValue1",
					"CurveColor1",		"CurveColor1",
					"CurveFillColor1",	"CurveFillColor1",
					"MaxValue2",		"MaxValue2",
					"MinValue2",		"MinValue2",
					"CurveColor2",		"CurveColor2",
					"CurveFillColor2",	"CurveFillColor2",
					"Dynamic",		"Dynamic",
					""};
    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
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
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowSlider)
  {
    char transtab[][32] = {	 	"SubGraph",		"SubGraph",
					"TraceData1", 		"Attribute",
					"TraceData2",		"",
					"TraceData3",		"",
					"TraceData4",		"",
					"TraceData5",		"",
					"TraceData6",		"",
					"TraceData7",		"",
					"TraceData8",		"",
					"TraceData9",		"",
					"TraceAttrType",	"",
					"TraceColor",		"",
					"TraceAccess",		"Access",
					"TraceCycle",		"Cycle",
					"TraceRefObject",	"RefObject",
					"Direction",		"Direction",
					"MaxValue",		"MaxValue",
					"MinValue",		"MinValue",
					"MaxPos",		"MaxPosition",
					"MinPos",		"MinPosition",
					"Dynamic",		"",
					""};
    grow_GetObjectAttrInfo( object, (char *)transtab, &grow_info, 
		&grow_info_cnt);
  }
  else
    grow_GetObjectAttrInfo( object, NULL, &grow_info, &grow_info_cnt);

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
        case graph_eTrace_DigToneWithCommand:
        case graph_eTrace_DigToneWithErrorAndCommand:
        case graph_eTrace_SetDigConfirmWithTone:
        case graph_eTrace_ResetDigConfirmWithTone:
        case graph_eTrace_ToggleDigConfirmWithTone:
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_AnnotInputWithTone:
        case graph_eTrace_AnnotWithTone:
        case graph_eTrace_DigToneTwo:
        case graph_eTrace_DigToneTwoWithCommand:
        case graph_eTrace_StoDigWithTone:
        case graph_eTrace_Rotate:
        case graph_eTrace_Move:
        case graph_eTrace_AnalogShift:
        case graph_eTrace_Animation:
        case graph_eTrace_AnimationForwBack:
        case graph_eTrace_DigAnimation:
        case graph_eTrace_DigShift:
        case graph_eTrace_DigWithText:
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
    items[i].noedit = grow_info_p->no_edit;
    items[i].multiline = grow_info_p->multiline;
    grow_info_p++;
  }

  *itemlist = items;
  *item_cnt = grow_info_cnt;
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
  attr->reconfigure_attr_cb = &graph_reconfigure_attr_cb;
  attr->store_cb = &graph_attr_store_cb;
  attr->recall_cb = &graph_attr_recall_cb;
  attr->set_data_cb = &graph_attr_set_data_cb;
  attr_list.insert( (void *) attr);
  grow_SetModified( grow->ctx, 1);
  return 1;
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
  glow_sTraceData *trace_data;

  grow_GetTraceAttr( object, &trace_data);

  graph->recall.insert( trace_data, "");
}

static int graph_attr_recall_cb( void *g, grow_tObject object, int idx, 
				 glow_sTraceData **old)
{
  Graph	*graph = (Graph *)g;
  glow_sTraceData trace_data;
  glow_sTraceData *old_p;
  static glow_sTraceData old_trace_data;
  int sts;

  sts = graph->recall.get( &trace_data, idx);
  if ( ODD(sts)) {
    grow_GetTraceAttr( object, &old_p);
    old_trace_data = *old_p;
    grow_SetTraceAttr( object, &trace_data);
    *old = &old_trace_data;
  }
  return sts;
}

static int graph_attr_set_data_cb( void *g, grow_tObject object,
				 glow_sTraceData *data)
{

  grow_SetTraceAttr( object, data);
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
  graph_eTrace		trace_type;

  sts = grow_FindNodeClassByName( graph->grow->ctx, name, &object);
  if ( EVEN(sts)) return sts;

  grow_GetObjectAttrInfo( object, NULL, &grow_info, &grow_info_cnt);
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
        case graph_eTrace_DigToneWithCommand:
        case graph_eTrace_DigToneWithErrorAndCommand:
        case graph_eTrace_SetDigConfirmWithTone:
        case graph_eTrace_ResetDigConfirmWithTone:
        case graph_eTrace_ToggleDigConfirmWithTone:
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_AnnotInputWithTone:
        case graph_eTrace_AnnotWithTone:
        case graph_eTrace_DigToneTwo:
        case graph_eTrace_DigToneTwoWithCommand:
        case graph_eTrace_StoDigWithTone:
        case graph_eTrace_Rotate:
        case graph_eTrace_Move:
        case graph_eTrace_AnalogShift:
        case graph_eTrace_Animation:
        case graph_eTrace_AnimationForwBack:
        case graph_eTrace_DigAnimation:
        case graph_eTrace_DigShift:
        case graph_eTrace_DigWithText:
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
  attr_sItem	items[20];
  int		i;
  grow_sAttrInfo	*grow_info, *grow_info_p;
  int			grow_info_cnt;
  Attr   		*attr;
  graph_eTrace		trace_type;

  grow_GetSubGraphAttrInfo( grow->ctx, &grow_info, &grow_info_cnt);
  grow_GetSubGraphTraceType( grow->ctx, (glow_eTraceType *) &trace_type);

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
        case graph_eTrace_DigToneWithCommand:
        case graph_eTrace_DigToneWithErrorAndCommand:
        case graph_eTrace_SetDigConfirmWithTone:
        case graph_eTrace_ResetDigConfirmWithTone:
        case graph_eTrace_ToggleDigConfirmWithTone:
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_AnnotInputWithTone:
        case graph_eTrace_AnnotWithTone:
        case graph_eTrace_DigToneTwo:
        case graph_eTrace_DigToneTwoWithCommand:
        case graph_eTrace_StoDigWithTone:
        case graph_eTrace_Rotate:
        case graph_eTrace_Move:
        case graph_eTrace_AnalogShift:
        case graph_eTrace_Animation:
        case graph_eTrace_AnimationForwBack:
        case graph_eTrace_DigAnimation:
        case graph_eTrace_DigShift:
        case graph_eTrace_DigWithText:
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

    if ( !event->con_create.dest_object)
      return 1;
    graph->get_conclass( graph->get_border_drawtype(), 
		graph->linewidth, graph->con_type, graph->con_corner,
		graph->corner_round_amount, &cc);
    sprintf( name, "C%d", grow_GetNextObjectNameNumber( graph->grow->ctx));
    grow_CreateCon( graph->grow->ctx, "", cc,
	event->con_create.source_object, event->con_create.dest_object, 
	event->con_create.source_conpoint, event->con_create.dest_conpoint,
	NULL, &con, 0, NULL, NULL);
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
	    glow_mDisplayLevel_1, graph->fill, graph->border,
	    graph->get_fill_drawtype(), NULL, &r1);
          grow_SetModified( graph->grow->ctx, 1);
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
          grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Axis:
        {
	  grow_tObject l1;
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

          grow_CreateGrowAxis( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2, event->create_grow_object.y2,
	    graph->get_border_drawtype(), graph->linewidth, 
            textsize, drawtype, NULL, &l1);
          grow_SetModified( graph->grow->ctx, 1);
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
            if ( graph->fill)
              grow_CreateGrowPolyLine( graph->grow->ctx, "", 
		(glow_sPoint *)&points, point_cnt,
	      	graph->get_border_drawtype(), graph->linewidth, 0, 
		0, graph->border, 
		graph->get_fill_drawtype(), 0, NULL, 
		&graph->current_polyline);
            else
              grow_CreateGrowPolyLine( graph->grow->ctx, "", 
		(glow_sPoint *)&points, point_cnt,
	      	graph->get_border_drawtype(), graph->linewidth, 0, 
		graph->fill, graph->border, 
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
	    graph->fill, graph->border, graph->get_fill_drawtype(), NULL, &a1);
          grow_SetModified( graph->grow->ctx, 1);
          grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Bar:
        {
	  grow_tObject r1;

          grow_CreateGrowBar( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2 - event->create_grow_object.x, 
	    event->create_grow_object.y2 - event->create_grow_object.y,
	    graph->get_border_drawtype(), graph->linewidth,
	    glow_mDisplayLevel_1, graph->fill, graph->border,
	    graph->get_fill_drawtype(), NULL, &r1);
          grow_SetModified( graph->grow->ctx, 1);
          grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Trend:
        {
	  grow_tObject r1;

          grow_CreateGrowTrend( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    event->create_grow_object.x2 - event->create_grow_object.x, 
	    event->create_grow_object.y2 - event->create_grow_object.y,
	    graph->get_border_drawtype(), graph->linewidth,
	    glow_mDisplayLevel_1, graph->fill, graph->border,
	    graph->get_fill_drawtype(), NULL, &r1);
          grow_SetModified( graph->grow->ctx, 1);
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
      break;
    case glow_eEvent_MB3Click:
      if ( graph->fill && graph->current_polyline &&
	   grow_Mode( graph->grow->ctx) == grow_eMode_PolyLine)
      {
        grow_SetObjectFill( graph->current_polyline, 1);
//        grow_SetObjectDrawtype( graph->current_polyline, graph->get_border_drawtype());
      }
      grow_PolylineEnd( graph->grow->ctx);
      graph->current_polyline = 0;
      grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
      grow_EnableHighlight( graph->grow->ctx);
      grow_SelectClear( graph->grow->ctx);
      grow_SetMoveRestrictions( graph->grow->ctx, glow_eMoveRestriction_No, 0, 0, NULL);
      grow_SetScaleEqual( graph->grow->ctx, 0);
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
          grow_SetMode( graph->grow->ctx, grow_eMode_Edit);
          break;
        }
        case grow_eMode_Annot:
        {
	  grow_tObject a1;

          grow_CreateGrowAnnot( graph->grow->ctx, "", 
	    event->create_grow_object.x, event->create_grow_object.y, 
	    1, glow_eDrawType_TextHelveticaBold, 3, glow_eAnnotType_OneLine,
	    0, glow_mDisplayLevel_1, NULL, &a1);
          grow_SetModified( graph->grow->ctx, 1);
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
	    drawtype, textsize, glow_mDisplayLevel_1,
	    NULL, &t1);
          if ( graph->change_text_cb)
            (graph->change_text_cb)( graph->parent_ctx, t1, "");
          grow_SetModified( graph->grow->ctx, 1);
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
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup))
      {
        glow_sTraceData 	*trace_data;
	char			msg[80];
        glow_eDrawType		color;
        char			*color_name;
        graph_eTrace		trace_type;

        grow_GetTraceAttr( event->object.object, &trace_data);
        trace_type = (graph_eTrace) trace_data->attr_type;
        if ( trace_type == graph_eTrace_Inherit)
          grow_GetObjectClassTraceType( event->object.object, (glow_eTraceType *) &trace_type);

        color = graph->get_fill_drawtype();
        trace_data->color = color;

        switch( trace_type)
        {
          case graph_eTrace_DigTone:
          case graph_eTrace_DigToneWithCommand:
          case graph_eTrace_DigToneWithError:
          case graph_eTrace_DigToneWithErrorAndCommand:
          case graph_eTrace_DigToneTwo:
          case graph_eTrace_DigToneTwoWithCommand:
          case graph_eTrace_DigWithText:
            color_name = grow_ColorToneToName( color);
            if ( strcmp( color_name, "") == 0)
              color_name = grow_ColorToName( color);
            sprintf( msg, "LowTone set to %s", color_name);
            graph->message( 'E', msg);
            break;
          default:
            sprintf( msg, "LowColor set to %s", grow_ColorToName( color));
            graph->message( 'E', msg);
        }
        grow_SetModified( graph->grow->ctx, 1);
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
  grow_EnableEvent( ctx, glow_eEvent_HotRequest, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Down, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB1Up, glow_eEventType_CallBack,
	graph_grow_cb);
  grow_EnableEvent( ctx, glow_eEvent_MB3Down, glow_eEventType_CallBack,
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
  int		sts;
  graph_sTraceData *data;
  pwr_tSubid	subid1, subid2;
  int		size1, size2;
  int		inverted1, inverted2;
  graph_eTrace	trace_type = (graph_eTrace) trace_data->attr_type;
  glow_eDrawType trace_color = trace_data->color;
  glow_eDrawType trace_color2 = trace_data->color2;
  glow_eDrawType color;
  glow_eCycle   cycle = trace_data->cycle;
  void		*p1, *p2;
  char		parsed_name[120];
  static int 	dummy = 0;
  GrowCtx 	*ctx;
  Graph 	*graph;
  graph_eDatabase db;
  int		attr_type, attr_size;

  grow_ObjectToCtx( object, &ctx);
  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &graph);

  if ( grow_GetObjectType( object) == glow_eObjectType_GrowNode ||
       grow_GetObjectType( object) == glow_eObjectType_GrowGroup)
  {
    if ( trace_type == graph_eTrace_Inherit)
      grow_GetObjectClassTraceType( object, (glow_eTraceType *) &trace_type);
    if ( trace_color == glow_eDrawType_Inherit)
      grow_GetObjectClassTraceColor( object, &trace_color, &color);
    if ( trace_color2 == glow_eDrawType_Inherit)
      grow_GetObjectClassTraceColor( object, &color, &trace_color2);
    if ( cycle == glow_eCycle_Inherit)
      grow_GetObjectClassCycle( object, &cycle);
    switch ( trace_type)
    {
      case graph_eTrace_No:
        break;
      case graph_eTrace_Dig:
      case graph_eTrace_DigWithCommand:
      case graph_eTrace_DigBorder:
      {
//        printf( "Connecting %s (Dig)\n", trace_data->data[0]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Color out of range, %s\n", trace_data->data[0]);
          return 0;
        }

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, 
		&inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[0] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
          if ( EVEN(sts)) return sts;

          data->access = trace_data->access;
          data->p[0] = p1;
          data->db[0] = db;
          data->subid[0] = subid1;
          data->type = trace_type;
          data->size[0] = size1;
          data->inverted[0] = inverted1;
        }
        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        if ( data->p[0] || data->p[1])
          trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_Invisible:
      case graph_eTrace_RadioButton:
      {
//        printf( "Connecting %s (Invis)\n", trace_data->data[0]);

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          break;

        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->inverted[0] = inverted1;
        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_DigShiftWithToggleDig:
      {
//        printf( "Connecting %s (Invis)\n", trace_data->data[1]);

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0) {
          // Use data[0] instead
	  db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
	  if ( strcmp( parsed_name,"") == 0) break;
        }
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->inverted[0] = inverted1;
        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_DigWithText:
      {
//        printf( "Connecting %s (Dig)\n", trace_data->data[0]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Color out of range, %s\n", trace_data->data[0]);
          return 0;
        }
        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->inverted[0] = inverted1;
        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;

        if ( strcmp( trace_data->data[1], "") != 0)
        {
          strcpy( data->low_text, trace_data->data[1]);
          grow_GetAnnotation( object, 1, data->high_text, sizeof(data->high_text));
        }
        break;
      }
      case graph_eTrace_DigWithError:
      case graph_eTrace_DigTwo:
      case graph_eTrace_DigTwoWithCommand:
      case graph_eTrace_DigWithErrorAndCommand:
      {
//        printf( "Connecting %s %s (DigWithError)\n", trace_data->data[0], 
//		trace_data->data[1]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Color out of range, %s\n", trace_data->data[0]);
          return 0;
        }

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->inverted[0] = inverted1;
        data->color = trace_color;
        data->color2 = trace_color2;
        data->cycle = cycle;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_DigTone:
      case graph_eTrace_DigToneWithCommand:
      {
//        printf( "Connecting %s (DigTone)\n", trace_data->data[0]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Tone out of range, %s\n", trace_data->data[0]);
          return 0;
        }
        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[0] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
          if ( EVEN(sts)) return sts;

          data->access = trace_data->access;
          data->p[0] = p1;
          data->db[0] = db;
          data->subid[0] = subid1;
          data->type = trace_type;
          data->size[0] = size1;
          data->inverted[0] = inverted1;
        }
        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        if ( data->p[0] || data->p[1])
          trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_DigToneWithError:
      case graph_eTrace_DigToneWithErrorAndCommand:
      case graph_eTrace_DigToneTwo:
      case graph_eTrace_DigToneTwoWithCommand:
      {
//        printf( "Connecting %s %s (DigToneWithError)\n", trace_data->data[0], 
//		trace_data->data[1]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Tone out of range, %s\n", trace_data->data[0]);
          return 0;
        }

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->inverted[0] = inverted1;
        data->color = trace_color;
        data->color2 = trace_color2;
        data->cycle = cycle;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_Annot:
      case graph_eTrace_AnnotInput:
      {

//        printf( "Connecting %s (Annot)\n", trace_data->data[0]);

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, 
		&inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        switch ( db) {
          case graph_eDatabase_Gdh:
            sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, attr_size);
            if ( EVEN(sts)) return sts;
            if ( attr_type != 0)
              data->annot_typeid = attr_type;
            else
              data->annot_typeid = graph_get_typeid( data);
            break;
          case graph_eDatabase_Local:
            p1 = graph->localdb_ref_or_create( parsed_name, attr_type);
	    data->annot_typeid = attr_type;
	    if ( attr_type == pwr_eType_String)
	      data->annot_size = 80;
	    else
	      data->annot_size = 4;
          case graph_eDatabase_User:
	    data->annot_typeid = attr_type;
	    if ( attr_type == pwr_eType_String)
	      data->annot_size = 80;
	    else
	      data->annot_size = 4;
            break;
          default:
            ;
        }
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = attr_size;
        data->cycle = cycle;
        data->first_scan = 1;
        strcpy( data->format, trace_data->data[1]);
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_AnnotWithTone:
      case graph_eTrace_AnnotInputWithTone:
      {
//        printf( "Connecting %s (Annot)\n", trace_data->data[0]);

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, 
		&inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        switch ( db) {
          case graph_eDatabase_Gdh:
            sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, attr_size);
            if ( EVEN(sts)) return sts;
            if ( attr_type != 0)
              data->annot_typeid = attr_type;
            else
              data->annot_typeid = graph_get_typeid( data);
            break;
          case graph_eDatabase_Local:
            p1 = graph->localdb_ref_or_create( parsed_name, attr_type);
	    data->annot_typeid = attr_type;
	    if ( attr_type == pwr_eType_String)
	      data->annot_size = 80;
	    else
	      data->annot_size = 4;
          case graph_eDatabase_User:
	    data->annot_typeid = attr_type;
	    if ( attr_type == pwr_eType_String)
	      data->annot_size = 80;
	    else
	      data->annot_size = 4;
            break;
          default:
            ;
        }
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = attr_size;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[3], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }

        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        strcpy( data->format, trace_data->data[1]);
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        break;
      }
      case graph_eTrace_Rotate:
      {
//        printf( "Connecting %s (Rotate)\n", trace_data->data[0]);
        float f;

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;
        data->cycle = cycle;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[3], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[4], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }

        data->color = trace_color;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        grow_StoreTransform( object);
        if ( strcmp( trace_data->data[5], "") != 0)
        {
          sts = sscanf( trace_data->data[5], "%f", &f);
          if ( sts == 0)
            data->factor = 1.0;
          else
            data->factor = f;
        }
        else
          data->factor = 1.0;
        if ( strcmp( trace_data->data[1], "") != 0 && 
	     strcmp( trace_data->data[2], "") != 0)
        {
          sts = sscanf( trace_data->data[1], "%f", &f);
          if ( sts != 0)
          {
            data->x0 = f;
            sts = sscanf( trace_data->data[2], "%f", &f);
            if ( sts != 0)
              data->y0 = f;
          }
          if ( sts != 0)
            data->rotation_point = glow_eRotationPoint_FixPoint;
          else
            data->rotation_point = glow_eRotationPoint_Zero;
        }
        else
          data->rotation_point = glow_eRotationPoint_Zero;
        break;
      }
      case graph_eTrace_Move:
      {
//        printf( "Connecting %s (Move)\n", trace_data->data[0]);
        float f;
        double ur_x, ur_y;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->type = trace_type;
        data->cycle = cycle;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[0] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[0] = p2;
          data->db[0] = db;
          data->subid[0] = subid2;
          data->size[0] = size2;
          data->inverted[0] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[3], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[3] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[3] = p2;
          data->subid[3] = subid2;
          data->size[3] = size2;
          data->inverted[3] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[7], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[4] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[4] = p2;
          data->subid[4] = subid2;
          data->size[4] = size2;
          data->inverted[4] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[8], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[5] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[5] = p2;
          data->subid[5] = subid2;
          data->size[5] = size2;
          data->inverted[5] = inverted2;
        }

        data->color = trace_color;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;
        grow_StoreTransform( object);
        if ( strcmp( trace_data->data[4], "") != 0)
        {
          sts = sscanf( trace_data->data[4], "%f", &f);
          if ( sts == 0)
            data->factor = 1.0;
          else
            data->factor = f;
        }
        else
          data->factor = 1.0;

        if ( strcmp( trace_data->data[5], "") != 0)
        {
          sts = sscanf( trace_data->data[5], "%f", &f);
          if ( sts == 0)
            data->x0 = 0;
          else
            data->x0 = f;
        }
        else
          data->x0 = 0;

        if ( strcmp( trace_data->data[6], "") != 0)
        {
          sts = sscanf( trace_data->data[6], "%f", &f);
          if ( sts == 0)
            data->y0 = 0;
          else
            data->y0 = f;
        }
        else
          data->y0 = 0;

        grow_MeasureNode( object, &data->x_orig, &data->y_orig, &ur_x, &ur_y);
        if ( data->p[0] || data->p[1] || data->p[4] || data->p[5])
          trace_data->p = (void *)&dummy;

        break;
      }
      case graph_eTrace_AnalogShift:
      {
//        printf( "Connecting %s (AnalogShift)\n", trace_data->data[0]);

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }

        data->color = trace_color;
        data->cycle = cycle;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;

        break;
      }
      case graph_eTrace_IncrAnalog:
      {
//        printf( "Connecting %s (IncrAnalog)\n", trace_data->data[0]);

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->p[0] = p1;
        data->db[0] = db;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;

        data->color = trace_color;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;

        break;
      }
      case graph_eTrace_Animation:
      case graph_eTrace_AnimationForwBack:
      case graph_eTrace_DigAnimation:
      case graph_eTrace_DigShift:
      {
//        printf( "Connecting %s (Animation)\n", trace_data->data[0]);
        int sts;

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          return 1;
        sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
        if ( EVEN(sts)) return sts;

        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->cycle = cycle;
        data->p[0] = p1;
        data->db[0] = db;
        data->inverted[0] = inverted1;
        data->p[1] = 0;
        data->subid[0] = subid1;
        data->type = trace_type;
        data->size[0] = size1;

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }

        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[2] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[2] = p2;
          data->subid[2] = subid2;
          data->size[2] = size2;
          data->inverted[2] = inverted2;
        }

        data->color = trace_color;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        trace_data->p = (void *)&dummy;

        break;
      }
      case graph_eTrace_SetDigConfirmWithTone:
      case graph_eTrace_ResetDigConfirmWithTone:
      case graph_eTrace_ToggleDigConfirmWithTone:
      case graph_eTrace_SetDigWithTone:
      case graph_eTrace_ResetDigWithTone:
      case graph_eTrace_ToggleDigWithTone:
      case graph_eTrace_StoDigWithTone:
      {
//        printf( "Connecting %s (DigTone)\n", trace_data->data[2]);
        if ( trace_color < 0 || trace_color >= glow_eDrawType__)
        {
          printf( "** Tone out of range, %s\n", trace_data->data[2]);
          return 0;
        }
        data = (graph_sTraceData *) calloc( 1, sizeof( *data));
        data->access = trace_data->access;
        data->type = trace_type;
        data->cycle = cycle;

        size1 = 4;
	db = graph->parse_attr_name( trace_data->data[2], parsed_name, &inverted1, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[0] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
          if ( EVEN(sts)) return sts;

          data->p[0] = p1;
          data->db[0] = db;
          data->subid[0] = subid1;
          data->size[0] = size1;
          data->inverted[0] = inverted1;
        }
        size2 = 4;
	db = graph->parse_attr_name( trace_data->data[3], parsed_name, &inverted2, &attr_type, &attr_size);
        if ( strcmp( parsed_name,"") == 0)
          data->p[1] = 0;
        else
        {
          sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
          if ( EVEN(sts)) return sts;

          data->p[1] = p2;
          data->subid[1] = subid2;
          data->size[1] = size2;
          data->inverted[1] = inverted2;
        }
        if ( strcmp( trace_data->data[4], "") != 0)
        {
          strcpy( data->low_text, trace_data->data[4]);
          grow_GetAnnotation( object, 1, data->high_text, sizeof(data->high_text));
        }
        data->color = trace_color;
        data->first_scan = 1;
        grow_SetUserData( object, (void *)data);
        if ( data->p[0] || data->p[1])
          trace_data->p = (void *)&dummy;
        break;
      }
      default:
        ;
    }
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowBar)
  {
//     printf( "Connecting %s (Bar)\n", trace_data->data[0]);
     if ( cycle == glow_eCycle_Inherit)
       cycle = glow_eCycle_Slow;

     size1 = 4;
     db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
     if ( strcmp( parsed_name,"") == 0)
       return 1;
     sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
     if ( EVEN(sts)) return sts;

     data = (graph_sTraceData *) calloc( 1, sizeof( *data));
     data->access = trace_data->access;
     data->cycle = cycle;
     data->p[0] = p1;
     data->db[0] = db;
     data->p[1] = 0;
     data->subid[0] = subid1;
     data->type = graph_eTrace_Bar;
     data->size[0] = size1;
     data->inverted[0] = inverted1;
     data->first_scan = 1;
     grow_SetUserData( object, (void *)data);
     trace_data->p = (void *)&dummy;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowTrend)
  {
//     printf( "Connecting %s %s(Trend)\n", trace_data->data[0], trace_data->data[1]);
     if ( cycle == glow_eCycle_Inherit)
       cycle = glow_eCycle_Slow;

     size1 = 4;
     db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
     if ( strcmp( parsed_name,"") == 0)
       return 1;
       
     sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
     if ( EVEN(sts)) return sts;

     data = (graph_sTraceData *) calloc( 1, sizeof( *data));
     data->access = trace_data->access;
     data->cycle = cycle;
     data->p[0] = p1;
     data->db[0] = db;
     data->subid[0] = subid1;
     data->type = graph_eTrace_Trend;
     data->size[0] = size1;
     data->inverted[0] = inverted1;
     data->trend_boolean = (strstr( trace_data->data[0], "##Boolean") != 0);
     data->first_scan = 1;
     grow_SetUserData( object, (void *)data);
     grow_GetTrendScanTime( object, &data->scan_time);
     data->acc_time = data->scan_time;

     size2 = 4;
     db = graph->parse_attr_name( trace_data->data[1], parsed_name, &inverted2, &attr_type, &attr_size);
     if ( strcmp( parsed_name,"") == 0)
       data->p[1] = 0;
     else
     {
       sts = graph->ref_object_info( cycle, parsed_name, &p2, &subid2, size2);
       if ( EVEN(sts)) return sts;

       data->p[1] = p2;
       data->subid[1] = subid2;
       data->size[1] = size2;
       data->inverted[1] = inverted2;
       data->trend_boolean |= (strstr( trace_data->data[1], "##Boolean") != 0) << 1;
     }

     trace_data->p = (void *)&dummy;
  }
  else if ( grow_GetObjectType( object) == glow_eObjectType_GrowSlider)
  {
//     printf( "Connecting %s (Slider)\n", trace_data->data[0]);
     if ( cycle == glow_eCycle_Inherit)
       grow_GetObjectClassCycle( object, &cycle);

     size1 = 4;
     db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted1, &attr_type, &attr_size);
     if ( strcmp( parsed_name,"") == 0)
       return 1;
     sts = graph->ref_object_info( cycle, parsed_name, &p1, &subid1, size1);
     if ( EVEN(sts)) return sts;

     data = (graph_sTraceData *) calloc( 1, sizeof( *data));
     data->access = trace_data->access;
     data->cycle = cycle;
     data->p[0] = p1;
     data->db[0] = db;
     data->p[1] = 0;
     data->subid[0] = subid1;
     data->type = graph_eTrace_Slider;
     data->size[0] = size1;
     data->inverted[0] = inverted1;
     data->first_scan = 1;
     grow_StoreTransform( object);
     grow_SetUserData( object, (void *)data);
     trace_data->p = (void *)&dummy;

     // Set type to slider to get hot
     trace_data->attr_type = (glow_eTraceType)graph_eTrace_Slider;
     grow_SetTraceAttr( object, trace_data);

     // Get min and max position from sider background
     double max_value, min_value, max_pos, min_pos;
     glow_eDirection direction;
     double ll_x, ll_y, ur_x, ur_y;
     grow_tObject background;
     double origo;

     grow_MeasureNode( object, &ll_x, &ll_y, &ur_x, &ur_y);
     grow_GetSliderInfo( object, &direction, 
		&max_value, &min_value, &max_pos, &min_pos);
     sts = grow_GetBackgroundObjectLimits( graph->grow->ctx, 
	(glow_eTraceType)graph_eTrace_SliderBackground,
	(ll_x + ur_x) / 2, (ll_y + ur_y) / 2, &background,
	&min_pos, &max_pos, &direction);
     if ( ODD(sts)) {
       grow_GetSliderOrigo( object, direction, &origo);

       switch( direction) {
         case glow_eDirection_Down:
           grow_SetSliderInfo( object, direction,
		max_value, min_value, max_pos - origo, min_pos - origo);

           grow_MoveNode( object, ll_x, min_pos - origo);
           break;
         case glow_eDirection_Up:
           grow_SetSliderInfo( object, direction,
		max_value, min_value, max_pos - (ur_y - ll_y - origo), 
		min_pos - (ur_y - ll_y - origo));
           grow_MoveNode( object, ll_x, min_pos - (ur_y - ll_y - origo));
           break;
         case glow_eDirection_Left:
           grow_SetSliderInfo( object, direction,
		max_value, min_value, max_pos - (ur_x - ll_x - origo), 
		min_pos - (ur_x - ll_x - origo));
           grow_MoveNode( object, min_pos - (ur_x - ll_x - origo), ll_y);
           break;
         case glow_eDirection_Right:
           grow_SetSliderInfo( object, direction,
		max_value, min_value, max_pos - origo, min_pos - origo);

           grow_MoveNode( object, min_pos - origo, ll_y);
           break;
         default:
	   ;
       }
       grow_StoreTransform( object);
     }
  }
  return 1;
}

static int graph_trace_disconnect_bc( grow_tObject object)
{
  graph_sTraceData *data;
  int i;

  grow_GetUserData( object, (void **)&data);
  if ( !data)
    return 1;
  switch( data->type)
  {
    case 0:
      grow_SetAnnotation( object, 1, "", 0);
      break;
    default:
      ;
  }
  if ( data->p[0] && data->db[0] == graph_eDatabase_Gdh)
    gdh_UnrefObjectInfo( data->subid[0]);
  for ( i = 1; i < (int)(sizeof(data->subid)/sizeof(data->subid[0])); i++) {
    if ( data->p[i])
      gdh_UnrefObjectInfo( data->subid[i]);
  }
  free( (char *) data);
  return 1;
}


static int graph_trace_scan_bc( grow_tObject object, void *p)
{
  graph_sTraceData *data;
  char		buf[120];
  int		len;
  GrowCtx 	*ctx;
  Graph 	*graph;

  grow_ObjectToCtx( object, &ctx);
  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &graph);

  grow_GetUserData( object, (void **)&data);

  if ( data->cycle == glow_eCycle_Inherit)
    return 1;
  if ( data->cycle == glow_eCycle_Slow && graph->slow_scan_cnt != 0 &&
       !(data->type == graph_eTrace_Animation ||
         data->type == graph_eTrace_AnimationForwBack ||
         data->type == graph_eTrace_DigAnimation))
    return 1;
  if ( data->cycle == glow_eCycle_Fast && graph->fast_scan_cnt != 0 &&
       !(data->type == graph_eTrace_Animation ||
         data->type == graph_eTrace_AnimationForwBack ||
         data->type == graph_eTrace_DigAnimation))
    return 1;

  switch( data->type)
  {
    case graph_eTrace_Dig:
    case graph_eTrace_DigWithCommand:
    {
      if ( !data->first_scan)
      {
        if ( data->p[0] && !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( data->p[1] && !data->p[0])
        {
          if ( *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[0])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      break;
    }
    case graph_eTrace_DigWithText:
    {
      if ( !data->first_scan)
      {
        if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
          // No change since last time
          return 1;
      }
      else
        data->first_scan = 0;

      if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
           (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
      {
        if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
          grow_SetObjectFillColor( object, data->color);
        else
          grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        if ( data->low_text[0])
          grow_SetAnnotation( object, 1, data->low_text, strlen(data->low_text));
      }
      else
      {
        if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
          grow_ResetObjectFillColor( object);
        else
          grow_ResetObjectColorTone( object);
        if ( data->low_text[0])
          grow_SetAnnotation( object, 1, data->high_text, strlen(data->low_text));
      }
      *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      break;
    }
    case graph_eTrace_DigBorder:
    {
      if ( !data->first_scan)
      {
        if ( data->p[0] && !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( data->p[1] && !data->p[0])
        {
          if ( *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[0])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
           (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectBorderColor( object, data->color);
        else
          grow_ResetObjectBorderColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      break;
    }
    case graph_eTrace_Invisible:
    {
      if ( !data->first_scan)
      {
        if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
          // No change since last time
          return 1;
      }
      else
        data->first_scan = 0;

      if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
           (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        grow_SetObjectVisibility( object, 1);
      else
        grow_SetObjectVisibility( object, 0);
      *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      break;
    }
    case graph_eTrace_DigWithError:
    case graph_eTrace_DigWithErrorAndCommand:
    {
      if ( !data->first_scan)
      {
        if ( !data->p[1] && !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectFillColor( object, glow_eDrawType_LineRed);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      else if ( !data->p[1] && data->p[2])
      {
        if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectFillColor( object, glow_eDrawType_Color6);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      else if ( !data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
	     (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      else
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectFillColor( object, glow_eDrawType_LineRed);
        else if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectFillColor( object, glow_eDrawType_Color6);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_DigTwo:
    case graph_eTrace_DigTwoWithCommand:
    {
      if ( !data->first_scan)
      {
        if ( !data->p[1] && !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectFillColor( object, glow_eDrawType_LineRed);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      else if ( !data->p[1] && data->p[2])
      {
        if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectFillColor( object, data->color2);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      else if ( !data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
	     (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      else
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectFillColor( object, glow_eDrawType_LineRed);
        else if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectFillColor( object, data->color2);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFillColor( object, data->color);
        else
          grow_ResetObjectFillColor( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_DigTone:
    case graph_eTrace_DigToneWithCommand:
    case graph_eTrace_SetDigConfirmWithTone:
    case graph_eTrace_ResetDigConfirmWithTone:
    case graph_eTrace_ToggleDigConfirmWithTone:
    case graph_eTrace_SetDigWithTone:
    case graph_eTrace_ResetDigWithTone:
    case graph_eTrace_ToggleDigWithTone:
    case graph_eTrace_StoDigWithTone:
    {
      if ( !data->first_scan)
      {
        if ( data->p[0] && !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( data->p[1] && !data->p[0])
        {
          if ( *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[0])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->low_text[0])
            grow_SetAnnotation( object, 1, data->low_text, strlen(data->low_text));
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->low_text[0])
            grow_SetAnnotation( object, 1, data->high_text, strlen(data->high_text));
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      break;
    }
    case graph_eTrace_DigToneWithError:
    case graph_eTrace_DigToneWithErrorAndCommand:
    {
      if ( !data->first_scan)
      {
        if ( !data->p[1] && !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Red);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      else if ( !data->p[1] && data->p[2])
      {
        if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Yellow);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      else if ( !data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
	     (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        else
          grow_ResetObjectColorTone( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      else
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Red);
        }
        else if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Yellow);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_DigToneTwo:
    case graph_eTrace_DigToneTwoWithCommand:
    {
      if ( !data->first_scan)
      {
        if ( !data->p[1] && !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0])
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( *(pwr_tBoolean *)data->old_value[0] == *(pwr_tBoolean *)data->p[0] &&
               *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      if ( data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Red);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      else if ( !data->p[1] && data->p[2])
      {
        if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__ ||
               data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          if ( data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color2);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color2);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__ ||
               data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      else if ( !data->p[1] && !data->p[2])
      {
        if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
	     (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        else
          grow_ResetObjectColorTone( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }
      else
      {
        if ( (!data->inverted[1] && *(pwr_tBoolean *)data->p[1]) ||
	     (data->inverted[1] && ! *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_SetObjectColorTone( object, glow_eDrawTone_Red);
        }
        else if ( (!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
	     (data->inverted[2] && ! *(pwr_tBoolean *)data->p[2]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__ ||
               data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          if ( data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color2);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color2);
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
                  (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__ ||
               data->color2 >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_AnnotWithTone:
    case graph_eTrace_AnnotInputWithTone:
    {
      if ( !data->first_scan)
      {
        if ( data->p[2])
        {
	  if ( *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2] && 
               ((!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
                (data->inverted[2] && !*(pwr_tBoolean *)data->p[2])))
            // Invisible
            return 1;
        }
        if ( !data->p[1] && !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0 )
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      switch( data->annot_typeid) {
        case pwr_eType_Float32:
          len = sprintf( buf, data->format, *(pwr_tFloat32 *) data->p[0]);
          break;
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
          len = sprintf( buf, data->format, *(pwr_tInt32 *) data->p[0]);
          break;
        case pwr_eType_String:
          len = sprintf( buf, data->format, (char *)data->p[0]);
          break;
        case pwr_eType_Objid:
	{
          int sts;
          char name[80];
          pwr_tObjid objid = *(pwr_tObjid *)data->p[0];
          sts = gdh_ObjidToName ( objid, name, sizeof(name), 
			 cdh_mName_object);
          if ( EVEN(sts))
            strcpy( name, "");
          len = sprintf( buf, "%s", name);
          break;
        }
        default:
	{
          int sts;
          sts = cdh_AttrValueToString( (pwr_eType) data->annot_typeid, 
                data->p[0], buf, sizeof(buf));
          if ( EVEN(sts))
            sprintf( buf, "Invalid type");
          len = strlen(buf);
        }
      }
      grow_SetAnnotation( object, 1, buf, len);
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[2] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[2] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_Annot:
    case graph_eTrace_AnnotInput:
    {
      if ( !data->first_scan)
      {
        if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0 )
          // No change since last time
          return 1;
      }
      else
        data->first_scan = 0;

      switch( data->annot_typeid) {
        case pwr_eType_Float32:
          len = sprintf( buf, data->format, *(pwr_tFloat32 *) data->p[0]);
          break;
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
          len = sprintf( buf, data->format, *(pwr_tInt32 *) data->p[0]);
          break;
        case pwr_eType_String:
          len = sprintf( buf, data->format, (char *)data->p[0]);
          break;
        case pwr_eType_Objid:
	{
          int sts;
          char name[80];
          pwr_tObjid objid = *(pwr_tObjid *)data->p[0];
          sts = gdh_ObjidToName ( objid, name, sizeof(name), 
			 cdh_mName_object);
          if ( EVEN(sts))
            strcpy( name, "");
          len = sprintf( buf, "%s", name);
          break;
        }
        default:
	{
          int sts;
          sts = cdh_AttrValueToString( (pwr_eType) data->annot_typeid,
             data->p[0], buf, sizeof(buf));
          if ( EVEN(sts))
            sprintf( buf, "Invalid type");
          len = strlen(buf);
        }
      }

      grow_SetAnnotation( object, 1, buf, len);
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));
      break;
    }
    case graph_eTrace_Rotate:
    {
      double value;

      if ( !data->first_scan)
      {
        if ( data->p[2])
        {
	  if ( *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2] && 
               ((!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
                (data->inverted[2] && !*(pwr_tBoolean *)data->p[2])))
            // Invisible
            return 1;
        }
        if ( !data->p[1] && !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0 )
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      value = *(float *)data->p[0] * data->factor;

      grow_SetObjectRotation( object, value, data->x0, data->y0, 
		data->rotation_point);
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[2] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[2] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_Move:
    {
      double 	move_x, move_y, scale_x, scale_y;
      int	update = 0;

      if ( !data->first_scan)
      {
        // See if anything is changed since last time
        if ( data->p[0] &&
             memcmp( data->old_value[0], data->p[0], data->size[0]) != 0 )
          update = 1;
        else if ( data->p[1] &&
             memcmp( data->old_value[1], data->p[1], data->size[1]) != 0 )
          update = 1;
        else if ( data->p[2] &&
             memcmp( data->old_value[2], data->p[2], data->size[2]) != 0 )
          update = 1;
        else if ( data->p[3] &&
             memcmp( data->old_value[3], data->p[3], data->size[3]) != 0 )
          update = 1;
        else if ( data->p[4] &&
             memcmp( data->old_value[4], data->p[4], data->size[4]) != 0 )
          update = 1;
        else if ( data->p[5] &&
             memcmp( data->old_value[5], data->p[5], data->size[5]) != 0 )
          update = 1;
        if ( !update)
          return 1;
      }
      else
        data->first_scan = 0;

      if ( data->p[4] || data->p[5]) {
        if ( data->p[4])
          scale_x = *(float *)data->p[4];
        else
          scale_x = 1;

        if ( data->p[5])
          scale_y = *(float *)data->p[5];
        else
          scale_y = 1;

      
        if ( !(data->p[0] || data->p[1]))
          grow_SetObjectScale( object, scale_x, scale_y, 0, 0,
			     glow_eScaleType_LowerLeft);
        if ( data->p[4])
          memcpy( data->old_value[4], data->p[4], min(data->size[4], (int) sizeof(data->old_value[4])));
        if ( data->p[5])
          memcpy( data->old_value[5], data->p[5], min(data->size[5], (int) sizeof(data->old_value[5])));


        if ( data->p[0] || data->p[1]) {
          if ( data->p[0])
            move_x = data->x_orig + (*(float *)data->p[0] - data->x0) * data->factor;
          else
            move_x = data->x_orig;

          if ( data->p[1])
            move_y = data->y_orig + (*(float *)data->p[1] - data->y0) * data->factor;
          else
            move_y = data->y_orig;

          grow_SetObjectScalePos( object, move_x, move_y, 
			     scale_x, scale_y, 0, 0,
			     glow_eScaleType_LowerLeft);
          if ( data->p[0])
            memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));
          if ( data->p[1])
            memcpy( data->old_value[1], data->p[1], min(data->size[1], (int) sizeof(data->old_value[1])));
        }
      }
      else {
        if ( data->p[0] || data->p[1]) {
          if ( data->p[0])
            move_x = (*(float *)data->p[0] - data->x0) * data->factor;
          else
            move_x = 0;

          if ( data->p[1])
            move_y = (*(float *)data->p[1] - data->y0) * data->factor;
          else
            move_y = 0;

	  grow_SetObjectPosition( object, move_x, move_y);

          if ( data->p[0])
            memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));
          if ( data->p[1])
            memcpy( data->old_value[1], data->p[1], min(data->size[1], (int) sizeof(data->old_value[1])));
        }
      }

      if ( data->p[2])
      {
        if ( (!data->inverted[2] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[2] && *(pwr_tBoolean *)data->p[2]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      if ( data->p[3])
      {
        if ( (!data->inverted[3] && !*(pwr_tBoolean *)data->p[3]) ||
             (data->inverted[3] && *(pwr_tBoolean *)data->p[3]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[3] = *(pwr_tBoolean *)data->p[3];
      }
      break;
    }
    case graph_eTrace_AnalogShift:
    {
      double value;

      if ( !data->first_scan)
      {
        if ( data->p[2])
        {
	  if ( *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2] && 
               ((!data->inverted[2] && *(pwr_tBoolean *)data->p[2]) ||
                (data->inverted[2] && !*(pwr_tBoolean *)data->p[2])))
            // Invisible
            return 1;
        }
        if ( !data->p[1] && !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0 )
            // No change since last time
            return 1;
        }
        else if ( !data->p[2])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1])
            // No change since last time
            return 1;
        }
        else if ( !data->p[1])
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
        else
        {
          if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0  &&
	       *(pwr_tBoolean *)data->old_value[1] == *(pwr_tBoolean *)data->p[1] &&
	       *(pwr_tBoolean *)data->old_value[2] == *(pwr_tBoolean *)data->p[2])
            // No change since last time
            return 1;
        }
      }
      else
        data->first_scan = 0;

      value = *(float *)data->p[0];

      grow_SetObjectNodeClassByIndex( object, int(value));
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[2] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[2] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_Animation:
    case graph_eTrace_AnimationForwBack:
    {
      int max_count;
      int sts;

      if ( data->first_scan)
      {
        data->animation_count = 0;
        data->animation_direction = 1;
        data->first_scan = 0;
      }

      if ( (!data->inverted[0] && *(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && !*(pwr_tBoolean *)data->p[0]))
      {
        if ( data->animation_direction == 0)
        {
          // Animation has been stopped
          data->animation_count = 0;
          data->animation_direction = 1;
        }

        grow_GetObjectAnimationCount( object, &max_count);
        data->animation_count++;
        if ( data->animation_count >= max_count)
        {
          // Shift nodeclass
          if ( data->animation_direction == 1)
          {
            // Shift forward

            sts = grow_SetObjectNextNodeClass( object);
            if ( EVEN(sts))
            {
              if ( data->type == graph_eTrace_Animation)
              {
                // Start from the beginning again
                grow_SetObjectNodeClassByIndex( object, 1);
//                grow_SetObjectFirstNodeClass( object);
              }
              else
              {
                // Change direction
                data->animation_direction = 2;
                sts = grow_SetObjectPrevNodeClass( object);
              }
            }
            data->animation_count = 0;
          }
          else
          {
            // Shift backward

            sts = grow_SetObjectPrevNodeClass( object);
            if ( EVEN(sts))
            {
              // Change direction
              data->animation_direction = 1;
              sts = grow_SetObjectNextNodeClass( object);
            }
            data->animation_count = 0;
          }
        }
      }
      else
      {
        if ( data->animation_direction != 0)
        {
          // Stop and reset animation
          data->animation_direction = 0;
          grow_SetObjectFirstNodeClass( object);
        }
      }

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_DigAnimation:
    {
      int max_count;
      int sts;

      if ( data->first_scan)
      {
        data->animation_count = 0;
        data->animation_direction = 0;
        data->first_scan = 0;
        if ( (!data->inverted[0] && *(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && !*(pwr_tBoolean *)data->p[0]))
          grow_SetObjectLastNodeClass( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }

      if ( *(pwr_tBoolean *)data->old_value[0] != *(pwr_tBoolean *)data->p[0])
      {
        // Start animation
        if ( (!data->inverted[0] && *(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && !*(pwr_tBoolean *)data->p[0]))
        {
          data->animation_count = 0;
          data->animation_direction = 1;
        }
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
        {
          data->animation_direction = 2;
          data->animation_count = 0;
        }
      }

      if ( data->animation_direction != 0)
      {
        grow_GetObjectAnimationCount( object, &max_count);
        data->animation_count++;
        if ( data->animation_count >= max_count)
        {
          // Shift nodeclass
          if ( data->animation_direction == 1)
          {
            // Shift forward

            sts = grow_SetObjectNextNodeClass( object);
            if ( EVEN(sts))
            {
              // End of animation
              data->animation_count = 0;
              data->animation_direction = 0;
            }
            data->animation_count = 0;
          }
          else
          {
            // Shift backward

            sts = grow_SetObjectPrevNodeClass( object);
            if ( EVEN(sts))
            {
              // End of animation
              data->animation_count = 0;
              data->animation_direction = 0;
            }
            data->animation_count = 0;
          }
        }
      }
      *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_DigShift:
    case graph_eTrace_RadioButton:
    case graph_eTrace_DigShiftWithToggleDig:
    {
      if ( data->first_scan)
      {
        data->first_scan = 0;
        if ( (!data->inverted[0] && *(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && !*(pwr_tBoolean *)data->p[0]))
          grow_SetObjectLastNodeClass( object);
        *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];
      }

      if ( *(pwr_tBoolean *)data->old_value[0] != *(pwr_tBoolean *)data->p[0])
      {
        // Start animation
        if ( (!data->inverted[0] && *(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && !*(pwr_tBoolean *)data->p[0]))
          grow_SetObjectLastNodeClass( object);
        else if ( (!data->inverted[0] && !*(pwr_tBoolean *)data->p[0]) ||
             (data->inverted[0] && *(pwr_tBoolean *)data->p[0]))
          grow_SetObjectFirstNodeClass( object);
      }
      *(pwr_tBoolean *)data->old_value[0] = *(pwr_tBoolean *)data->p[0];

      if ( data->p[1])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[1]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[1]))
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_SetObjectFillColor( object, data->color);
          else
            grow_SetObjectColorTone( object, (glow_eDrawTone) data->color);
        }
        else
        {
          if ( data->color >= (glow_eDrawType) glow_eDrawTone__)
            grow_ResetObjectFillColor( object);
          else
            grow_ResetObjectColorTone( object);
        }
        *(pwr_tBoolean *)data->old_value[1] = *(pwr_tBoolean *)data->p[1];
      }
      if ( data->p[2])
      {
        if ( (!data->inverted[1] && !*(pwr_tBoolean *)data->p[2]) ||
             (data->inverted[1] && *(pwr_tBoolean *)data->p[2]))
          grow_SetObjectVisibility( object, 1);
        else
          grow_SetObjectVisibility( object, 0);
        *(pwr_tBoolean *)data->old_value[2] = *(pwr_tBoolean *)data->p[2];
      }
      break;
    }
    case graph_eTrace_Bar:
    {
      if ( !data->first_scan)
      {
        if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0)
          // No change since last time
          return 1;
      }
      else
        data->first_scan = 0;

      grow_SetBarValue( object, double( *(float *) data->p[0]));
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));
      break;
    }
    case graph_eTrace_Trend:
    {
      if ( data->trend_hold)
        break;

      if ( data->first_scan)
        data->first_scan = 0;

      data->acc_time += graph->scan_time;
      if ( data->acc_time + DBL_EPSILON >= data->scan_time)
      {
        if ( data->trend_boolean & 1)
          grow_AddTrendValue( object, double( *(pwr_tBoolean *) data->p[0]), 0);
        else
          grow_AddTrendValue( object, double( *(float *) data->p[0]), 0);
        if ( data->p[1])
        {
          if ( data->trend_boolean & 2)
            grow_AddTrendValue( object, double( *(pwr_tBoolean *) data->p[1]), 1);
          else
            grow_AddTrendValue( object, double( *(float *) data->p[1]), 1);
        }
        data->acc_time = 0;
      }
      break;
    }
    case graph_eTrace_Slider:
    {
      double max_value, min_value, max_pos, min_pos;
      glow_eDirection direction;

      if ( !data->first_scan)
      {
        if ( memcmp( data->old_value[0], data->p[0], data->size[0]) == 0)
          // No change since last time
          return 1;
      }
      else
        data->first_scan = 0;

      grow_GetSliderInfo( object, &direction, 
		&max_value, &min_value, &max_pos, &min_pos);
      if ( min_pos != max_pos)
      {
        if ( graph->current_slider != object &&
	     max_value != min_value)
        {
          float value = *(float *) data->p[0];
	  double pos_x, pos_y;
          
          switch ( direction)
          {
            case glow_eDirection_Down:
              pos_y = (max_value - value) / (max_value - min_value) *
		(max_pos - min_pos);
              if ( pos_y < 0)
                pos_y = 0;
              else if ( pos_y > max_pos - min_pos)
                pos_y = max_pos - min_pos;
              pos_x = 0;
              break;
            case glow_eDirection_Right:
              pos_x = max_pos - min_pos - 
		(value - min_value) / (max_value - min_value) *
		(max_pos - min_pos);
              if ( pos_x < 0)
                pos_x = 0;
              else if ( pos_x > max_pos - min_pos)
                pos_x = max_pos - min_pos;
              pos_y = 0;
              break;
            case glow_eDirection_Left:
              pos_x = max_pos - min_pos - 
		(max_value - value) / (max_value - min_value) *
		(max_pos - min_pos);
              if ( pos_x < 0)
                pos_x = 0;
              else if ( pos_x > max_pos - min_pos)
                pos_x = max_pos - min_pos;
              pos_y = 0;
              break;
            default:   // Up
              pos_y = (value - min_value) / (max_value - min_value) *
		(max_pos - min_pos);
              if ( pos_y < 0)
                pos_y = 0;
              else if ( pos_y > max_pos - min_pos)
                pos_y = max_pos - min_pos;
              pos_x = 0;
          }
          grow_SetObjectPosition( object, pos_x, pos_y);
        }
      }
      memcpy( data->old_value[0], data->p[0], min(data->size[0], (int) sizeof(data->old_value[0])));
      break;
    }
    default:
      ;
  }
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

  switch ( event->event)
  {
    case glow_eEvent_MB1Down:
    {
      if ( event->object.object_type != glow_eObjectType_NoObject &&
           (grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
            grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup))
      {
        int		trace_type;
        glow_sTraceData	*trace_data;
	pwr_tBoolean	value;
	int		sts;
        char		parsed_name[120];
        int		inverted;
	int		attr_type, attr_size;

        grow_GetTraceAttr( event->object.object, &trace_data);
        if ( !graph->is_authorized( trace_data->access))
          break;

        graph->current_mb1_down = event->object.object;
        trace_type = trace_data->attr_type;
        if ( trace_type == graph_eTrace_Inherit)
          grow_GetObjectClassTraceType( event->object.object, 
		(glow_eTraceType *) &trace_type);
        switch ( trace_type)
        {
          case graph_eTrace_StoDigWithTone:
	    value = 1;
            graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts)) printf("StoDig error: %s\n", trace_data->data[0]);
            grow_SetObjectColorInverse( event->object.object, 1);
            break;
          case graph_eTrace_SetDig:
          case graph_eTrace_SetDigWithTone:
          case graph_eTrace_ResetDig:
          case graph_eTrace_ResetDigWithTone:
          case graph_eTrace_ToggleDig:
          case graph_eTrace_ToggleDigWithTone:
          case graph_eTrace_Command:
          case graph_eTrace_SetDigConfirm:
          case graph_eTrace_ResetDigConfirm:
          case graph_eTrace_ToggleDigConfirm:
          case graph_eTrace_CommandConfirm:
          case graph_eTrace_ToggleDigConfirmWithTone:
          case graph_eTrace_SetDigConfirmWithTone:
          case graph_eTrace_ResetDigConfirmWithTone:
          case graph_eTrace_IncrAnalog:
          case graph_eTrace_RadioButton:
          case graph_eTrace_DigShiftWithToggleDig:
	    grow_SetClickSensitivity( graph->grow->ctx, glow_mSensitivity_MB1Click);
            grow_SetObjectColorInverse( event->object.object, 1);
            break;
          case graph_eTrace_DigWithCommand:
          case graph_eTrace_DigWithErrorAndCommand:
          case graph_eTrace_DigTwoWithCommand:
          case graph_eTrace_DigToneWithCommand:
          case graph_eTrace_DigToneWithErrorAndCommand:
          case graph_eTrace_DigToneTwoWithCommand:
            if ( graph->command_cb && strcmp(trace_data->data[4], "") != 0)
	      grow_SetClickSensitivity( graph->grow->ctx, 
			glow_mSensitivity_MB1Click | glow_mSensitivity_MB1DoubleClick);
	    else
	      grow_SetClickSensitivity( graph->grow->ctx, glow_mSensitivity_MB1Click);
            grow_SetObjectColorInverse( event->object.object, 1);
            break;
          default:
            ;
        }
      }
      break;
    }
    case glow_eEvent_MB1Up:
    {
      if ( !graph->current_mb1_down)
        break;
      if ( grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( graph->current_mb1_down) == glow_eObjectType_GrowGroup)
      {
        int		trace_type;
        glow_sTraceData	*trace_data;
	pwr_tBoolean	value;
	int		sts;
        char		parsed_name[120];
        int		inverted;
	int		attr_type, attr_size;

        grow_GetTraceAttr( graph->current_mb1_down, &trace_data);
        if ( !graph->is_authorized( trace_data->access))
          break;

        trace_type = trace_data->attr_type;
        if ( trace_type == graph_eTrace_Inherit)
          grow_GetObjectClassTraceType( graph->current_mb1_down, 
		(glow_eTraceType *) &trace_type);
        switch ( trace_type)
        {
          case graph_eTrace_StoDigWithTone:
            grow_SetObjectColorInverse( graph->current_mb1_down, 0);
	    value = 0;
            graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts)) printf("StoDig error: %s\n", trace_data->data[0]);
            break;
          case graph_eTrace_SetDig:
          case graph_eTrace_SetDigWithTone:
          case graph_eTrace_ResetDig:
          case graph_eTrace_ResetDigWithTone:
          case graph_eTrace_ToggleDig:
          case graph_eTrace_ToggleDigWithTone:
          case graph_eTrace_Command:
          case graph_eTrace_DigWithCommand:
          case graph_eTrace_DigWithErrorAndCommand:
          case graph_eTrace_DigTwoWithCommand:
          case graph_eTrace_DigToneWithCommand:
          case graph_eTrace_DigToneWithErrorAndCommand:
          case graph_eTrace_DigToneTwoWithCommand:
          case graph_eTrace_SetDigConfirm:
          case graph_eTrace_ResetDigConfirm:
          case graph_eTrace_ToggleDigConfirm:
          case graph_eTrace_CommandConfirm:
          case graph_eTrace_ToggleDigConfirmWithTone:
          case graph_eTrace_SetDigConfirmWithTone:
          case graph_eTrace_ResetDigConfirmWithTone:
          case graph_eTrace_IncrAnalog:
          case graph_eTrace_RadioButton:
          case graph_eTrace_DigShiftWithToggleDig:
            grow_SetObjectColorInverse( graph->current_mb1_down, 0);
            break;
          default:
            ;
        }
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
        glow_sTraceData	*trace_data;

        grow_GetTraceAttr( event->object.object, &trace_data);

        return graph->is_authorized( trace_data->access);
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
        double max_value, min_value, max_pos, min_pos;
        glow_eDirection direction;
        glow_sTraceData	*trace_data;
        graph_sTraceData *data;

        grow_GetUserData( event->object.object, (void **)&data);
        grow_GetTraceAttr( event->object.object, &trace_data);
        if ( !graph->is_authorized( trace_data->access) ||
             data->slider_disabled)
        {
          grow_SetMoveRestrictions( graph->grow->ctx, 
		glow_eMoveRestriction_Disable, 0, 0, NULL);
          graph->current_slider = NULL;
          break;
        }
        grow_GetSliderInfo( event->object.object, &direction, 
		&max_value, &min_value, &max_pos, &min_pos);
        if ( direction == glow_eDirection_Right || 
             direction == glow_eDirection_Left)
          grow_SetMoveRestrictions( graph->grow->ctx, 
		glow_eMoveRestriction_HorizontalSlider,
		max_pos, min_pos, event->object.object);
        else
          grow_SetMoveRestrictions( graph->grow->ctx, 
		glow_eMoveRestriction_VerticalSlider,
		max_pos, min_pos, event->object.object);
        graph->current_slider = event->object.object;
      }
      break;
    }
    case glow_eEvent_SliderMoved:
    {
      double 		max_value, min_value, max_pos, min_pos;
      glow_eDirection 	direction;
      float 		value;
      int		sts;
      double		ll_x, ll_y, ur_x, ur_y;
      glow_sTraceData	*trace_data;
      char		parsed_name[120];
      int		inverted;
      int		attr_type, attr_size;

      grow_GetTraceAttr( event->object.object, &trace_data);

      grow_GetSliderInfo( event->object.object, &direction, 
		&max_value, &min_value, &max_pos, &min_pos);
      if ( min_pos != max_pos)
      {
        grow_MeasureNode( event->object.object, &ll_x, &ll_y, &ur_x, &ur_y);
        
        switch ( direction)
        {
          case glow_eDirection_Down:
            value = float( (max_pos - ll_y) / (max_pos - min_pos) *
		(max_value - min_value) + min_value);
            break;
          case glow_eDirection_Right:
            value = float( (max_pos - ll_x) 
		/ (max_pos - min_pos) *
		(max_value - min_value) + min_value);
            break;
          case glow_eDirection_Left:
            value = float( (ll_x - min_pos) / (max_pos - min_pos) *
		(max_value - min_value) + min_value);
            break;
          default:
            value = float( (ll_y - min_pos) / (max_pos - min_pos) *
		(max_value - min_value) + min_value);
        }
        if ( value > max_value)
          value = max_value;
        if ( value < min_value)
          value = min_value;

        graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
        if ( EVEN(sts)) printf("Slider error: %s\n", trace_data->data[0]);
      }
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
      Widget          popup;
      glow_sTraceData *trace_data;
      int	      sts;
      char	      parsed_name[120];
      int	      inverted;
      int	      attr_type, attr_size;
      char            *s;
      pwr_sAttrRef    attrref;
      char            name[80];

      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( !( grow_GetObjectType( event->object.object) == 
	                                   glow_eObjectType_GrowNode ||
              grow_GetObjectType( event->object.object) == 
	                                   glow_eObjectType_GrowGroup ||
              grow_GetObjectType( event->object.object) == 
	                                   glow_eObjectType_GrowBar ||
              grow_GetObjectType( event->object.object) == 
	                                   glow_eObjectType_GrowTrend))
        break;
      if ( graph->mode != graph_eMode_Runtime)
        break;


      grow_GetTraceAttr( event->object.object, &trace_data);
      graph->parse_attr_name( trace_data->ref_object, parsed_name, &inverted,
			      &attr_type, &attr_size);
      if ( inverted) {
	// The ref_object is an objid-attribute that containts the object 
        memset( &attrref, 0, sizeof(attrref));
	sts = gdh_GetObjectInfo( parsed_name, &attrref.Objid, 
				 sizeof(attrref.Objid));
	if ( EVEN(sts)) break;
        if ( cdh_ObjidIsNull( attrref.Objid))
          break;
      }
      else {
        if ( (s = strrchr( parsed_name, '.')))
          *s = 0;

        memset( &attrref, 0, sizeof(attrref));
        sts = gdh_NameToObjid( parsed_name, &attrref.Objid);
        if ( EVEN(sts)) break;
      }
      switch( grow_GetMB3Action( graph->grow->ctx)) {
        case glow_eMB3Action_PopupMenu:
	case glow_eMB3Action_Both:
          if ( graph->popup_menu_cb)
          {

            // Display popup menu
            grow_GetName( graph->grow->ctx, name);

            (graph->popup_menu_cb)( graph->parent_ctx, attrref, 
				      xmenu_eItemType_Object,
				      xmenu_mUtility_Ge, name, &popup);
            if ( !popup)
              break;

            mrm_PositionPopup( popup, graph->grow_widget, 
			       event->any.x_pixel + 8, event->any.y_pixel);
            XtManageChild(popup);
          }
          break;
	default:
	  ;
      }
      break;
    }
    case glow_eEvent_MB1Click:
    {
      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup)
      {
        int		trace_type;
        glow_sTraceData	*trace_data;
	pwr_tBoolean	value;
	int		sts;
        char		parsed_name[120];
        int		inverted;
	int		attr_type, attr_size;
	graph_eDatabase db;

        grow_GetTraceAttr( event->object.object, &trace_data);
        trace_type = trace_data->attr_type;
        if ( trace_type == graph_eTrace_Inherit)
          grow_GetObjectClassTraceType( event->object.object, 
		(glow_eTraceType *) &trace_type);

        if ( strcmp( trace_data->data[0], "") == 0)
          break;
        if ( !graph->is_authorized( trace_data->access))
          break;
        switch ( trace_type)
        {
          case graph_eTrace_SetDig:
          case graph_eTrace_SetDigWithTone:
	    value = 1;
            db = graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    switch ( db) {
	      case graph_eDatabase_Local:
	        sts = graph->localdb_set_value( parsed_name, &value, sizeof(value));
	        if ( EVEN(sts)) printf("SetDig error: %s\n", trace_data->data[0]);
	        break;
	      case graph_eDatabase_Gdh:
	        sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	        if ( EVEN(sts)) printf("SetDig error: %s\n", trace_data->data[0]);
	        break;
              default:
                ;
            }
            break;
          case graph_eTrace_ResetDig:
          case graph_eTrace_ResetDigWithTone:
	    value = 0;
            graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts)) printf("ResetDig error: %s\n", trace_data->data[0]);
            break;
          case graph_eTrace_ToggleDig:
          case graph_eTrace_ToggleDigWithTone:
          case graph_eTrace_DigShiftWithToggleDig:
            graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    sts = gdh_GetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts))
            {
              printf("ToggleDig error: %s\n", trace_data->data[0]);
              break;
            }
	    value = !value;
	    sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts)) printf("ToggleDig error: %s\n", trace_data->data[0]);
            break;
          case graph_eTrace_IncrAnalog:
          {
            float value_f, incr, maxvalue, minvalue;

            graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
	    sts = gdh_GetObjectInfo( parsed_name, &value_f, sizeof(value_f));
	    if ( EVEN(sts))
            {
              printf("IncrAnalog error: %s\n", trace_data->data[0]);
              break;
            }
	    // Increment value
            if ( strcmp( trace_data->data[1], "") != 0)
            {
              sts = sscanf( trace_data->data[1], "%f", &incr);
              if ( sts == 0)
                incr = 1.0;
            }
            else
              incr = 1.0;

	    // Minimum value
            if ( strcmp( trace_data->data[2], "") != 0)
            {
              sts = sscanf( trace_data->data[2], "%f", &minvalue);
              if ( sts == 0)
                minvalue = 0;
            }
            else
              minvalue = 0;

	    // Maximum value
            if ( strcmp( trace_data->data[3], "") != 0)
            {
              sts = sscanf( trace_data->data[3], "%f", &maxvalue);
              if ( sts == 0)
                maxvalue = 0;
            }
            else
              maxvalue = 0;

	    value_f += incr;
	    if ( !( minvalue == 0 && maxvalue == 0))
            {
              value_f = max( value_f, minvalue);
              value_f = min( value_f, maxvalue);
            }
	    sts = gdh_SetObjectInfo( parsed_name, &value_f, sizeof(value_f));
	    if ( EVEN(sts)) printf("IncrAnalog error: %s\n", trace_data->data[0]);
            break;
          }
          case graph_eTrace_Command:
            if ( graph->command_cb) {
              char cmd[200];

              graph->get_command( trace_data->data[0], cmd);
              (graph->command_cb)( graph->parent_ctx, cmd);
            }
            break;
          case graph_eTrace_DigWithCommand:
          case graph_eTrace_DigWithErrorAndCommand:
          case graph_eTrace_DigTwoWithCommand:
          case graph_eTrace_DigToneWithCommand:
          case graph_eTrace_DigToneWithErrorAndCommand:
          case graph_eTrace_DigToneTwoWithCommand:
            if ( graph->command_cb && strcmp(trace_data->data[3], "") != 0) {
              char cmd[200];

              graph->get_command( trace_data->data[3], cmd);
              (graph->command_cb)( graph->parent_ctx, cmd);
            }
            break;
          case graph_eTrace_SetDigConfirm:
          case graph_eTrace_ResetDigConfirm:
          case graph_eTrace_ToggleDigConfirm:
          case graph_eTrace_CommandConfirm:
          case graph_eTrace_ToggleDigConfirmWithTone:
          case graph_eTrace_SetDigConfirmWithTone:
          case graph_eTrace_ResetDigConfirmWithTone:
            if ( graph->confirm_cb)
            {
              (graph->confirm_cb)( graph->parent_ctx, 
			event->object.object, trace_data->data[1]);
            }
            break;
          case graph_eTrace_AnnotInput:
          case graph_eTrace_AnnotInputWithTone:
//            grow_OpenAnnotationInput( event->object.object, 1);
            if ( graph->change_value_cb)
            {
              char str[80] = "";

              if ( strrchr( trace_data->data[1], 's'))
                // If string format, put current string in input label
                grow_GetAnnotation( event->object.object, 1, str, sizeof(str));

              (graph->change_value_cb)( graph->parent_ctx, 
			event->object.object, str);
            }
            break;
          case graph_eTrace_RadioButton:
	  {
            grow_tObject group;
            grow_tObject 	*objectlist, *object_p;
            int 		object_cnt;
            int		i;
            int		gm_trace_type;
            glow_sTraceData	*gm_trace_data;


            sts = grow_GetObjectGroup( graph->grow->ctx, event->object.object,
				       &group);
            if ( EVEN(sts)) break;

            grow_GetGroupObjectList( group, &objectlist, &object_cnt);
            object_p = objectlist;
            for ( i = 0; i < object_cnt; i++)
            {
              if ( *object_p != event->object.object &&
                   grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode)
              {
                value = 0;
                grow_GetTraceAttr( *object_p, &gm_trace_data);
                gm_trace_type = gm_trace_data->attr_type;
                if ( gm_trace_type == graph_eTrace_Inherit)
                  grow_GetObjectClassTraceType( *object_p, 
			       (glow_eTraceType *) &gm_trace_type);
		if ( gm_trace_type == graph_eTrace_RadioButton ) {
                  graph->parse_attr_name( gm_trace_data->data[0], parsed_name,
					  &inverted, &attr_type, &attr_size);
	          sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
                }
              }
              object_p++;
            }

	    value = 1;

            graph->parse_attr_name( trace_data->data[0], parsed_name,
                      &inverted, &attr_type, &attr_size);
	    sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
	    if ( EVEN(sts)) printf("ResetDig error: %s\n", trace_data->data[0]);
            break;
          }
        }
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
        int		trace_type;
        glow_sTraceData	*trace_data;

        grow_GetTraceAttr( event->object.object, &trace_data);
        trace_type = trace_data->attr_type;
        if ( trace_type == graph_eTrace_Inherit)
          grow_GetObjectClassTraceType( event->object.object, 
		(glow_eTraceType *) &trace_type);

        if ( strcmp( trace_data->data[0], "") == 0)
          break;

        if ( !graph->is_authorized( trace_data->access))
          break;

        switch ( trace_type)
        {
          case graph_eTrace_DigWithCommand:
          case graph_eTrace_DigWithErrorAndCommand:
          case graph_eTrace_DigTwoWithCommand:
          case graph_eTrace_DigToneWithCommand:
          case graph_eTrace_DigToneWithErrorAndCommand:
          case graph_eTrace_DigToneTwoWithCommand:
            if ( graph->command_cb && strcmp(trace_data->data[4], "") != 0) {
              char cmd[200];

              graph->get_command( trace_data->data[4], cmd);
              (graph->command_cb)( graph->parent_ctx, cmd);
            }
            break;
          default: ;
        }
      }
      break;
    }
    case glow_eEvent_MB1DoubleClickCtrl:
    {
      int		trace_type;
      glow_sTraceData	*trace_data;
      int		sts;
      pwr_sAttrRef 	attrref;
      char		parsed_name[120];
      int		inverted;
      int		attr_type, attr_size;

      if ( event->object.object_type == glow_eObjectType_NoObject)
        break;
      if ( !(grow_GetObjectType( event->object.object) == glow_eObjectType_GrowNode ||
             grow_GetObjectType( event->object.object) == glow_eObjectType_GrowGroup))
        break;

      grow_GetTraceAttr( event->object.object, &trace_data);
      trace_type = trace_data->attr_type;
      if ( trace_type == graph_eTrace_Inherit)
        grow_GetObjectClassTraceType( event->object.object, 
		(glow_eTraceType *) &trace_type);

      if ( strcmp( trace_data->data[0], "") == 0)
        break;
      switch ( trace_type)
      {
        case graph_eTrace_Dig:
        case graph_eTrace_DigWithError:
        case graph_eTrace_DigTone:
        case graph_eTrace_DigToneWithError:
        case graph_eTrace_Annot:
        case graph_eTrace_DigWithText:
        case graph_eTrace_Bar:
        case graph_eTrace_Trend:
        case graph_eTrace_DigBorder:
        case graph_eTrace_Invisible:
        case graph_eTrace_AnnotWithTone:
        case graph_eTrace_Rotate:
        case graph_eTrace_Move:
        case graph_eTrace_AnalogShift:
        case graph_eTrace_Animation:
        case graph_eTrace_AnimationForwBack:
        case graph_eTrace_DigAnimation:
        case graph_eTrace_DigShift:
        case graph_eTrace_SetDig:
        case graph_eTrace_ResetDig:
        case graph_eTrace_ToggleDig:
        case graph_eTrace_Slider:
        case graph_eTrace_AnnotInput:
        case graph_eTrace_SetDigConfirm:
        case graph_eTrace_ResetDigConfirm:
        case graph_eTrace_ToggleDigConfirm:
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_AnnotInputWithTone:
        case graph_eTrace_SetDigConfirmWithTone:
        case graph_eTrace_ResetDigConfirmWithTone:
        case graph_eTrace_ToggleDigConfirmWithTone:
        case graph_eTrace_DigWithCommand:
        case graph_eTrace_DigWithErrorAndCommand:
        case graph_eTrace_DigTwo:
        case graph_eTrace_DigTwoWithCommand:
        case graph_eTrace_DigToneWithCommand:
        case graph_eTrace_DigToneWithErrorAndCommand:
        case graph_eTrace_DigToneTwo:
        case graph_eTrace_DigToneTwoWithCommand:
        case graph_eTrace_StoDigWithTone:
        case graph_eTrace_IncrAnalog:
        case graph_eTrace_RadioButton:
        case graph_eTrace_DigShiftWithToggleDig:
          graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
          sts = gdh_NameToAttrref( pwr_cNObjid, trace_data->data[0], &attrref);
          if ( EVEN(sts)) return 1;

          if ( graph->display_in_xnav_cb)
            (graph->display_in_xnav_cb)( graph->parent_ctx, attrref.Objid);
          break;
        default:
          ;
      }
      break;
    }
    case glow_eEvent_AnnotationInput:
    {
      pwr_tFloat32	value;
      int		sts;
      glow_sTraceData	*trace_data;
      char		parsed_name[120];
      int		inverted;
      int		attr_type, attr_size;

      grow_GetTraceAttr( event->object.object, &trace_data);

      sts = sscanf( event->annot_input.text, "%f", &value);
      if ( sts != 1)
      {
        printf("Input syntax error\n");
        break;
      }
      graph->parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
      sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
      if ( EVEN(sts)) printf("AnnotationInput error: %s\n", trace_data->data[0]);
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
  int		trace_type;
  glow_sTraceData *trace_data;
  pwr_tBoolean	value;
  int		sts;
  char		parsed_name[120];
  int		inverted;
  int		attr_type, attr_size;

  grow_GetTraceAttr( object, &trace_data); 
  trace_type = trace_data->attr_type;

  if ( trace_type == graph_eTrace_Inherit)
    grow_GetObjectClassTraceType( object, 
		(glow_eTraceType *) &trace_type);

  if ( strcmp( trace_data->data[0], "") == 0)
    return;
  switch ( trace_type)
  {
    case graph_eTrace_SetDigConfirm:
    case graph_eTrace_SetDigConfirmWithTone:
      value = 1;
      parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
      sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
      if ( EVEN(sts)) printf("SetDig error: %s\n", trace_data->data[0]);
      break;
    case graph_eTrace_ResetDigConfirm:
    case graph_eTrace_ResetDigConfirmWithTone:
      value = 0;
      parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
      sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
      if ( EVEN(sts)) printf("ResetDig error: %s\n", trace_data->data[0]);
      break;
    case graph_eTrace_ToggleDigConfirm:
    case graph_eTrace_ToggleDigConfirmWithTone:
      parse_attr_name( trace_data->data[0], parsed_name, &inverted, &attr_type, &attr_size);
      sts = gdh_GetObjectInfo( parsed_name, &value, sizeof(value));
      if ( EVEN(sts))
      {
        printf("ToggleDig error: %s\n", trace_data->data[0]);
        break;
      }
      value = !value;
      sts = gdh_SetObjectInfo( parsed_name, &value, sizeof(value));
      if ( EVEN(sts)) printf("ToggleDig error: %s\n", trace_data->data[0]);
      break;
    case graph_eTrace_CommandConfirm:
      if ( command_cb)
        (command_cb)( parent_ctx, trace_data->data[0]);
      break;
    default: ;
  }
}

void Graph::connect( grow_tObject object, char *attr_name, int second)
{
  char		alias_name[32];
  int		trace_type;
  char		msg[200];
  glow_sTraceData *trace_data;

  switch( grow_GetObjectType( object))
  {
    case glow_eObjectType_GrowNode:
    case glow_eObjectType_GrowGroup:
    {
      grow_GetTraceAttr( object, &trace_data);
      trace_type = trace_data->attr_type;

      if ( trace_type == graph_eTrace_Inherit)
        grow_GetObjectClassTraceType( object, 
		(glow_eTraceType *) &trace_type);
      switch ( trace_type)
      {
        case graph_eTrace_Invisible:
        case graph_eTrace_Annot:
        case graph_eTrace_DigWithText:
        case graph_eTrace_Rotate:
        case graph_eTrace_Move:
        case graph_eTrace_SetDig:
        case graph_eTrace_ResetDig:
        case graph_eTrace_ToggleDig:
        case graph_eTrace_AnnotInput:
        case graph_eTrace_SetDigConfirm:
        case graph_eTrace_ResetDigConfirm:
        case graph_eTrace_ToggleDigConfirm:
        case graph_eTrace_IncrAnalog:
        case graph_eTrace_RadioButton:
          if ( second)
          {
            message( 'E', "Nothing to connect for this object");
            return;
          }
          strcpy( alias_name, "Attribute");
          strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_DigShift:
        case graph_eTrace_AnalogShift:
          if ( second)
            strcpy( alias_name, "ToneAttr");
          else
            strcpy( alias_name, "ShiftAttr");
          if ( second)
            strcpy( trace_data->data[1], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_Animation:
        case graph_eTrace_AnimationForwBack:
        case graph_eTrace_DigAnimation:

          if ( second)
            strcpy( alias_name, "ToneAttr");
          else
            strcpy( alias_name, "AnimationAttr");
          if ( second)
            strcpy( trace_data->data[1], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_Dig:
        case graph_eTrace_DigWithCommand:
        case graph_eTrace_DigBorder:
        case graph_eTrace_DigTone:
        case graph_eTrace_DigToneWithCommand:
          if ( second)
            strcpy( alias_name, "InvisAttr");
          else
            strcpy( alias_name, "Attribute");
          if ( second)
            strcpy( trace_data->data[1], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_SetDigConfirmWithTone:
        case graph_eTrace_ResetDigConfirmWithTone:
        case graph_eTrace_ToggleDigConfirmWithTone:
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_StoDigWithTone:
          if ( second)
            strcpy( alias_name, "ToneAttr");
          else
            strcpy( alias_name, "Attribute");
          if ( second)
            strcpy( trace_data->data[2], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_DigWithError:
        case graph_eTrace_DigWithErrorAndCommand:
        case graph_eTrace_DigTwo:
        case graph_eTrace_DigTwoWithCommand:
        case graph_eTrace_DigToneWithError:
        case graph_eTrace_DigToneWithErrorAndCommand:
        case graph_eTrace_DigToneTwo:
        case graph_eTrace_DigToneTwoWithCommand:
          if ( second)
            strcpy( alias_name, "ErrorAttr");
          else
            strcpy( alias_name, "Attribute");
          if ( second)
            strcpy( trace_data->data[1], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_AnnotWithTone:
        case graph_eTrace_AnnotInputWithTone:
          if ( second)
            strcpy( alias_name, "ToneAttr");
          else
            strcpy( alias_name, "Attribute");
          if ( second)
            strcpy( trace_data->data[2], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        case graph_eTrace_DigShiftWithToggleDig:
          if ( second)
            strcpy( alias_name, "ShiftAttr");
          else
            strcpy( alias_name, "ToggleAttr");
          if ( second)
            strcpy( trace_data->data[1], attr_name);
          else
            strcpy( trace_data->data[0], attr_name);
          break;
        default:
          message( 'E', "Nothing to connect for this object");
          return;
      }

      grow_SetTraceAttr( object, trace_data);
      sprintf( msg, "%s connected to '%s'", alias_name, attr_name);
      message( 'I', msg);
      break;
    }
    case glow_eObjectType_GrowSlider:
    {
      if ( second)
      {
        message( 'E', "Nothing to connect for this object");
        return;
      }

      grow_GetTraceAttr( object, &trace_data);
      strcpy( trace_data->data[0], attr_name);
      grow_SetTraceAttr( object, trace_data);
      sprintf( msg, "Attribute connected to '%s'", attr_name);
      message( 'I', msg);
      break;
    }
    case glow_eObjectType_GrowBar:
    {
      if ( second)
      {
        message( 'E', "Nothing to connect for this object");
        return;
      }

      grow_GetTraceAttr( object, &trace_data);
      strcpy( trace_data->data[0], attr_name);
      grow_SetTraceAttr( object, trace_data);
      sprintf( msg, "Attribute connected to '%s'", attr_name);
      message( 'I', msg);
      break;
    }
    case glow_eObjectType_GrowTrend:
    {
      grow_GetTraceAttr( object, &trace_data);
      if ( second)
        strcpy( trace_data->data[1], attr_name);
      else
        strcpy( trace_data->data[0], attr_name);

      grow_SetTraceAttr( object, trace_data);
      if ( second)
        sprintf( msg, "Attribute2 connected to '%s'", attr_name);
      else
        sprintf( msg, "Attribute1 connected to '%s'", attr_name);
      message( 'I', msg);
      break;
    }
    default:
      message( 'E', "Nothing to connect for this object");
  }

}

int Graph::set_object_focus( char *name, int empty)
{
  char str[80] = "";
  glow_sTraceData *trace_data;
  int  trace_type;
  int  sts;
  grow_tObject object;


  if ( !change_value_cb)
    return 0;

  sts = grow_FindObjectByName( grow->ctx, name, &object);
  if ( EVEN(sts)) return GE__OBJNOTFOUND;

  grow_GetTraceAttr( object, &trace_data);
  trace_type = trace_data->attr_type;
  if ( trace_type == graph_eTrace_Inherit)
    grow_GetObjectClassTraceType( object, (glow_eTraceType *) &trace_type);

  if ( strcmp( trace_data->data[0], "") == 0)
    return 0;
  if ( !is_authorized( trace_data->access))
    return GE__NOACCESS;

  switch ( trace_type)
  {
    case graph_eTrace_AnnotInput:
    case graph_eTrace_AnnotInputWithTone:

      if ( !empty && strrchr( trace_data->data[1], 's'))
        // If string format, put current string in input label
        grow_GetAnnotation( object, 1, str, sizeof(str));

      (change_value_cb)( parent_ctx, object, str);
      break;
    default:
      ;
  }
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
	int *inverted, int *type, int *size)
{
  char str[200];
  char str1[200];
  char *s, *s1;
  int elements;

  graph_remove_space( str, name);
  
  if ( (s = strstr( str, "$user")))
  {
    if ( (s = strchr( str, '#')))
    {
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
    if ( str[0] == '!')
    {
      *inverted = 1;
      graph_remove_space( str, &str[1]);
    }
    else
      *inverted = 0;
    strcpy( parsed_name, str);

    return graph_eDatabase_User;
  }
  if ( (s = strstr( str, "$local.")))
  {
    strcpy( parsed_name, s + strlen("$local."));
    if ( (s = strchr( parsed_name, '#')))
    {
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
    if ( str[0] == '!')
    {
      *inverted = 1;
      graph_remove_space( str, &str[1]);
    }
    else
      *inverted = 0;

    return graph_eDatabase_Local;
  }

  if ( (s = strstr( str, "$object")))
  {
    strcpy( str1, s + strlen("$object"));
    strcpy( s, object_name);
    strcat( str, str1);
  }
  if ( (s = strstr( str, "##")))
    string_to_type( s + 2, (pwr_eType *)type, size, &elements);

  if ( (s = strchr( str, '#')))
  {
    *s = 0;
    if ( (s1 = strchr( s+1, '[')))
      strcat( str, s1);
  }

  if ( str[0] == '!')
  {
    *inverted = 1;
    graph_remove_space( str, &str[1]);
    strcpy( parsed_name, str);
  }
  else
  {
    *inverted = 0;
    strcpy( parsed_name, str);
  }
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
static int  graph_attr_string_to_value( int type_id, char *value_str, 
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

static int graph_get_typeid( graph_sTraceData *data)
{
  // Guess the type of the annot variable...
  if ( strchr( data->format, 'f'))
    return pwr_eType_Float32;
  if ( strchr( data->format, 'd'))
    return pwr_eType_Int32;
  if ( strchr( data->format, 's'))
    return pwr_eType_String;
  if ( strchr( data->format, 'o'))
    return pwr_eType_Objid;
  return pwr_eType_Int32;
}

void GraphRecallBuff::insert( glow_sTraceData *data, char *data_key)
{
  int i;
  char new_key[80];

  if ( !data_key)
    strcpy( new_key, "");
  else
    strcpy( new_key, data_key);

  if ( strcmp( key[0], new_key) == 0 && memcmp( data, &buff[0], sizeof(buff[0])) == 0)
    // Identical with last one
    return;

  for ( i = cnt; i > 0; i--) {
    buff[i] = buff[i-1];
    strcpy( key[i], key[i-1]);
  }
  buff[0] = *data;
  strcpy( key[0], new_key);
  cnt++;
  if ( cnt > size)
    cnt = size;
}

int GraphRecallBuff::get( glow_sTraceData *data, int idx)
{
  if ( idx >= cnt)
    return 0;
  *data = buff[ idx];
  return 1;
}

int GraphRecallBuff::get( glow_sTraceData *data, char *k)
{
  for ( int i = 0; i < cnt; i++) {
    if ( strcmp( k, key[i]) == 0) {
      *data = buff[ i];
      return 1;
    }
  }
  return 0;
}
