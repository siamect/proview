/* ge.cpp -- Graphical editor window

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#include "glow_std.h"

#if defined OS_VMS || defined OS_LINUX

#define LDH 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/ToggleB.h>

extern "C" {
#include "rt_gdh.h"
#include "rt_load.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "flow_x.h"
#include "co_mrm_util.h"
#include "co_wow.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "glow.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_colpalwidget.h"
#include "glow_msg.h"


#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"
#include "ge_graph.h"
#include "ge.h"
#include "ge_subpalette.h"
#include "ge_subgraphs.h"
#include "ge_util.h"
#include "ge_msg.h"
#include "wb_wnav_selformat.h"
extern "C" {
#include "ge_plant.h"
}

typedef struct {
    char	name[40];
    char	prev[40];
} ge_tPrevPage;

typedef struct ge_sCtx {
  void 		*parent_ctx;
  Widget	parent_wid;
  char 		name[80];
  Widget	grow_widget;
  Widget	form_widget;
  Widget	main_pane;
  Widget	palette_pane;
  Widget	colorpalette_widget;
  Widget	colpal_main_widget;
  Widget	plant_widget;
  Widget	subpalette_widget;
  Widget	subgraphs_widget;
  Widget	toplevel;
  Widget	india_widget;
  Widget	india_label;
  Widget	india_text;
  Widget	cursor_position;
  Graph		*graph;
  SubPalette	*subpalette;
  SubGraphs	*subgraphs;
  colpal_tCtx	colorpalette_ctx;
  void		*root_item;
  Widget	msg_label;
  Widget	cmd_prompt;
  Widget	cmd_input;
  Widget	graph_form;
  Widget	confirm_widget;
  Widget	yesnodia_widget;
  Widget	grid_on_w;
  Widget	grid_size_w;
  Widget	grid_size_10_w;
  Widget	grid_size_05_w;
  Widget	grid_size_02_w;
  Widget	grid_size_01_w;
  Widget	show_grid_w;
  int		text_input_open;
  int		name_input_open;
  int		value_input_open;
  int		command_open;
  int		confirm_open;
  int		yesnodia_open;
  void		(*yesnodia_yes_cb)( struct ge_sCtx *);
  void		(*yesnodia_no_cb)( struct ge_sCtx *);
  void		(*india_ok_cb)( struct ge_sCtx *, char *);
  void		*current_text_object;
  void		*current_value_object;
  void		*current_confirm_object;
  ldh_tSesContext ldhses;
  void		*plantctx;
  int		exit_when_close;
  ge_tPrevPage	prev_table[40];
  int		prev_count;
  char		text_recall[30][160];
  int		text_current_recall;
  char		name_recall[30][160];
  int		name_current_recall;
  char		value_recall[30][160];
  int		value_current_recall;
  char		cmd_recall[30][160];
  int		cmd_current_recall;
  void		*focused_component;
  grow_tObject  recover_object;
  char          recover_name[80];
  Atom		graph_atom;
} *ge_tCtx;

// Static variables
static int ge_subpalette_get_select( void *gectx, char *text, char *filename);
static void ge_message( void *ctx, char severity, char *message);
static void ge_update( ge_tCtx gectx);
static void ge_set_prompt( ge_tCtx gectx, char *prompt);
static void ge_export_javabean( ge_tCtx gectx, char *name);
static void ge_export_gejava( ge_tCtx gectx, char *name);
static void ge_open_input_dialog( ge_tCtx gectx, char *text, char *title,
	char *init_text,
	void (*india_ok_cb)( ge_tCtx, char *));
static void ge_save_graph( ge_tCtx gectx, char *name);
static void ge_save_graph_and_close( ge_tCtx gectx, char *name);
static void ge_valchanged_cmd_input( Widget w, XEvent *event);
static int ge_set_focus( ge_tCtx gectx, void *component);

/* Not used any more... */
#if 0
static int ge_save( ge_tCtx gectx)
{
  char name[40];

  gectx->graph->get_name( name);
  if ( strcmp( name, "") == 0)
  {
    if ( ! gectx->graph->is_subgraph())
      ge_open_input_dialog( gectx, "Graph name", "Save Graph", "",
	ge_save_graph);
    else
      ge_open_input_dialog( gectx, "SubGraph name", "Save SubGraph", "", 
	ge_save_graph);
    return 0;
  }
  else
    ge_save_graph( gectx, name);
  return 1;
}
#endif

 
void ge_set_title( ge_tCtx gectx)
{
  Arg 		args[2];
  char          title[80];
  char          name[80];
  char          *s;
  int           page;

  gectx->graph->get_name( name);
  if ( (s = strstr( name, "__p")) &&
       (sscanf( s+3, "%d", &page) == 1)) {
    *s = 0;
    sprintf( title, "PwR Ge   %s   Page %d", name, page);
  }
  else {
    if ( gectx->graph->trace_started)
      sprintf( title, "PwR Ge   %s   Preview", name);
    else
      sprintf( title, "PwR Ge   %s", name);
  }
  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( gectx->toplevel, args, 1);
}

void ge_prevtable_insert( ge_tCtx gectx, char *name, char *prev)
{
  if ( gectx->prev_count >= 
	int( sizeof(gectx->prev_table)/sizeof(gectx->prev_table[0])))
    return;

  // Check if alread inserted
  for ( int i = 0; i < gectx->prev_count; i++)
  {
    if ( strcmp( name, gectx->prev_table[i].name) == 0)
    {
      strcpy( gectx->prev_table[i].prev, prev);
      return;
    }
  }

  strcpy( gectx->prev_table[gectx->prev_count].name, name);   
  strcpy( gectx->prev_table[gectx->prev_count].prev, prev);   
  gectx->prev_count++;
}

int ge_prevtable_get( ge_tCtx gectx, char *name, char *prev)
{
  for ( int i = 0; i < gectx->prev_count; i++)
  {
    if ( strcmp( name, gectx->prev_table[i].name) == 0)
    {
      strcpy( prev, gectx->prev_table[i].prev);
      return 1;
    }
  }
  return 0;
}

void ge_prevtable_clear( ge_tCtx gectx)
{
  gectx->prev_count = 0;
}

static void ge_save_and_close( ge_tCtx gectx)
{
  char name[40];

  gectx->graph->get_name( name);
  if ( strcmp( name, "") == 0)
  {
    if ( ! gectx->graph->is_subgraph())
      ge_open_input_dialog( gectx, "Graph name", "Save Graph", "",
	ge_save_graph_and_close);
    else
      ge_open_input_dialog( gectx, "SubGraph name", "Save SubGraph", "", 
	ge_save_graph_and_close);
  }
  else
  {
    ge_save_graph( gectx, name);

    ge_del( (void *)gectx);
  }
}

static void ge_ungroup_yes_cb( ge_tCtx gectx)
{
  gectx->graph->ungroup_select(1);
}

static void ge_ungroup_no_cb( ge_tCtx gectx)
{
}

static void ge_recover_dynprop_yes_cb( ge_tCtx gectx)
{
  gectx->graph->set_recall_data( gectx->recover_object, gectx->recover_name);
}

static void ge_recover_dynprop_no_cb( ge_tCtx gectx)
{
}

static void ge_exit_save_cb( ge_tCtx gectx)
{
  ge_save_and_close( gectx);
}

static void ge_exit_nosave_cb( ge_tCtx gectx)
{
  ge_del( (void *)gectx);
}

static void ge_open_input_dialog( ge_tCtx gectx, char *text, char *title,
	char *init_text,
	void (*india_ok_cb)( ge_tCtx, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( text, "ISO8859-1") ); i++;
  XtSetValues( gectx->india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") ); i++;
  XtSetValues( gectx->india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( gectx->india_text, init_text);

  XtManageChild( gectx->india_widget);

  gectx->india_ok_cb = india_ok_cb;
}

static void ge_load_graph_cb( void *ge_ctx, char *name)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;
  char		graphname[80];
  char 		*s;

  strcpy( graphname, name);
  if ( (s = strrchr( graphname, '.')))
    *s = 0;
  gectx->graph->set_name( graphname);

  gectx->graph->clear_all();
  gectx->graph->set_show_grid( 0);
  strcat( graphname, ".pwg");
  gectx->graph->open( graphname);
  ge_update( gectx);
}


static void ge_save_graph( ge_tCtx gectx, char *name)
{
  char filename[80];
  char graphname[80];
  char oldname[80];
  char *s;
  char next[40];
  int sts;

  if ( ! gectx->graph->is_subgraph() )
  {
    cdh_ToLower( graphname, name);
    if ( (s = strrchr( graphname, '.')))
      *s = 0;
    gectx->graph->get_name( oldname);
    if ( strcmp( graphname, oldname) != 0 && strcmp( oldname, "") != 0)
      // Name changed
      gectx->graph->set_java_name( "");


    gectx->graph->set_name( graphname);

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwg");

    sts = gectx->graph->save( filename);
    if ( EVEN(sts)) return;
    ge_message( gectx, 'I', "Graph saved");

    ge_set_title( gectx);
  }
  else
  {

    cdh_ToLower( graphname, name);
    if ( (s = strrchr( graphname, '.')))
      *s = 0;

    gectx->graph->get_name( oldname);
    if ( strcmp( graphname, oldname) != 0 && strcmp( oldname, "") != 0)
      // Name changed
      gectx->graph->set_java_name( "");

    gectx->graph->set_name( graphname);

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwsg");
    sts = gectx->graph->save_subgraph( filename);
    if ( EVEN(sts)) return;

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwg");
    sts = gectx->graph->save( filename);
    if ( EVEN(sts)) return;

    // Store this to enable previous page later
    gectx->graph->get_next_subgraph( next);
    if ( strcmp( next, "") == 0)
      ge_prevtable_insert( gectx, next, name);

    ge_set_title( gectx);

    ge_message( gectx, 'I', "SubGraph saved");
  }
}

void ge_save( void *ge_ctx, char *name)
{
  ge_save_graph( (ge_tCtx) ge_ctx, name);
}

static void ge_clear_all( ge_tCtx gectx)
{
  int		path_cnt;
  char		*path;

  gectx->graph->set_show_grid( 0);
  gectx->graph->set_gridsize( 1);
  gectx->graph->set_grid( 0);
  gectx->graph->clear_all();
  gectx->subpalette->get_path( &path_cnt, &path);
  gectx->graph->set_subgraph_path( path_cnt, path);
  ge_update( gectx);
  ge_set_title( gectx);

}

void ge_clear( void *ge_ctx)
{
  ge_clear_all( (ge_tCtx)ge_ctx);
}

static void ge_save_graph_and_close( ge_tCtx gectx, char *name)
{
  char filename[80];
  char graphname[80];
  char *s;
  int sts;

  if ( ! gectx->graph->is_subgraph() )
  {
    cdh_ToLower( graphname, name);
    if ( (s = strrchr( graphname, '.')))
      *s = 0;
    gectx->graph->set_name( graphname);

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwg");

    sts = gectx->graph->save( filename);
    if ( EVEN(sts)) return;
    ge_message( gectx, 'I', "Graph saved");
  }
  else
  {
    char filename[80];
    char graphname[80];
    char *s;

    cdh_ToLower( graphname, name);
    if ( (s = strrchr( graphname, '.')))
      *s = 0;
    gectx->graph->set_name( graphname);

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwsg");
    sts = gectx->graph->save_subgraph( filename);
    if ( EVEN(sts)) return;

    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".pwg");
    gectx->graph->save( filename);
    ge_message( gectx, 'I', "SubGraph saved");
  }

  ge_del( (void *)gectx);
}

static void ge_export_javabean( ge_tCtx gectx, char *name)
{
  char filename[80];
  char *s;
  int sts;

  if ( gectx->graph->is_subgraph())
  {
    char beanname[80];

    strcpy( beanname, name);
    if ( (s = strrchr( beanname, '.')))
      *s = 0;

    gectx->graph->set_java_name( beanname);
    cdh_ToLower( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".java");

    sts = gectx->graph->export_javabean( filename, beanname);
    if (ODD(sts))
      ge_message( gectx, 'I', "Javabean exported");
    else
      ge_message( gectx, 'E', "Java export error");
  }
  else
  {
    char framename[80];
    char appletname[80];

    strcpy( framename, name);
    if ( (s = strrchr( framename, '.')))
      *s = 0;

    strcpy( appletname, name);
    if ( (s = strrchr( appletname, '.')))
      *s = 0;
    strcat( appletname, "_A");

    gectx->graph->set_java_name( framename);
    strcpy( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".java");

    // Export frame
    sts = gectx->graph->export_javaframe( filename, framename, 0, 0);
    if ( EVEN(sts))
    {
      ge_message( gectx, 'E', "Java export error");
      return;
    }

    // Export applet
    strcpy( filename, appletname);
    strcat( filename, ".java");

    sts = gectx->graph->export_javaframe( filename, appletname, 1, 0);
    if ( EVEN(sts))
    {
      ge_message( gectx, 'E', "Java export error");
      return;
    }

    // Export html
    strcpy( filename, "$pwrp_web/");
    strcpy( filename, framename);
    strcat( filename, ".html");
    cdh_ToLower( filename, filename);

    sts = gectx->graph->export_javaframe( filename, appletname, 0, 1);
    if ( EVEN(sts))
    {
      ge_message( gectx, 'E', "Java export error");
      return;
    }
    ge_message( gectx, 'I', "Java frame and applet exported");
  }
}

static void ge_export_gejava( ge_tCtx gectx, char *name)
{
  char filename[80];
  char *s;
  int sts;
  char cmd[200];

  if ( gectx->graph->is_subgraph())
  {
    ge_message( gectx, 'E', "Unable to save subgraph as ge java");
  }
  else
  {
    char framename[80];
    char appletname[80];
    char systemname[80];

    strcpy( framename, name);
    if ( (s = strrchr( framename, '.')))
      *s = 0;

    strcpy( appletname, name);
    if ( (s = strrchr( appletname, '.')))
      *s = 0;
    strcat( appletname, "_A");

    gectx->graph->set_java_name( framename);
    strcpy( filename, name);
    if ( ! strrchr( filename, '.'))
      strcat( filename, ".java");

    // Export frame
    if ( gectx->graph->is_javaapplication()) {
      sts = gectx->graph->export_gejava( filename, framename, 0, 0);
      if ( EVEN(sts)) {
        ge_message( gectx, 'E', "Java export error");
        return;
      }

      // Compile frame
      gectx->graph->get_systemname( systemname);

      sprintf( cmd, "$pwr_exe/ge_javac.sh java %s %s", filename, systemname);
      sts = system( cmd);
      if ( sts != 0) {
        ge_message( gectx, 'E', "Java compilation errors");
        return;
      }
    }

    if ( gectx->graph->is_javaapplet()) {

      strcpy( filename, appletname);
      strcat( filename, ".java");

      // Export applet
      sts = gectx->graph->export_gejava( filename, appletname, 1, 0);
      if ( EVEN(sts))
      {
        ge_message( gectx, 'E', "Java export error");
        return;
      }

      // Compile applet
      gectx->graph->get_systemname( systemname);

      sprintf( cmd, "$pwr_exe/ge_javac.sh java_web %s %s", filename, systemname);
      sts = system( cmd);
      if ( sts != 0) {
        ge_message( gectx, 'E', "Java compilation errors");
        return;
      }

      // Export html
      strcpy( filename, "$pwrp_web/");
      strcat( filename, framename);
      strcat( filename, ".html");
      cdh_ToLower( filename, filename);

      sts = gectx->graph->export_gejava( filename, framename, 0, 1);
      if ( EVEN(sts)) {
        ge_message( gectx, 'E', "Java export error");
        return;
      }
    }

    if ( gectx->graph->is_javaapplication() && gectx->graph->is_javaapplet())
      ge_message( gectx, 'I', "Java frame and applet exported");
    else if ( gectx->graph->is_javaapplication())
      ge_message( gectx, 'I', "Java frame exported");
    else if ( gectx->graph->is_javaapplet())
      ge_message( gectx, 'I', "Java applet exported");
    else
      ge_message( gectx, 'I', "This graph is not java frame or applet");
  }
}

static void ge_open_graph( ge_tCtx gectx, char *name)
{
  char filename[80];
  char graphname[80];
  int		path_cnt;
  char		*path;
  char		*s;
 
  gectx->graph->clear_all();
  gectx->graph->set_show_grid( 0);

  cdh_ToLower( filename, name);
  strcpy( graphname, filename);
  if ( (s = strrchr( graphname, '.')))
    *s = 0;
  gectx->graph->set_name( graphname);
  if ( ! strrchr( filename, '.'))
    strcat( filename, ".pwg");
  gectx->graph->open( filename);

  gectx->subpalette->get_path( &path_cnt, &path);
  gectx->graph->set_subgraph_path( path_cnt, path);

  ge_update( gectx);
  ge_set_title( gectx);
}

void ge_open( void *ge_ctx, char *name)
{
  ge_open_graph( (ge_tCtx)ge_ctx, name);
}

static void ge_rotate( ge_tCtx gectx, char *value_str)
{
  float value;
  int	sts;

  sts = sscanf( value_str, "%f", &value);  
  if ( sts != 1)
  {
    ge_message( gectx, 'E', "Syntax error");
    return;
  }

  gectx->graph->rotate( double(value));
}

static void ge_message( void *ctx, char severity, char *message)
{
  ge_tCtx	gectx = (ge_tCtx) ctx;
  Arg 		args[3];
  XmString	cstr;

  if ( strcmp( message, "") == 0) {
    XtUnmanageChild( gectx->msg_label);
  }
  else {
    XtManageChild( gectx->msg_label);
    cstr=XmStringCreateLtoR( message, "ISO8859-1");
    XtSetArg(args[0],XmNlabelString, cstr);
    XtSetArg(args[1],XmNheight,30);
    XtSetValues( gectx->msg_label, args, 2);
    XmStringFree( cstr);
  }
}

static int ge_subpalette_get_select( void *gectx, char *text, char *filename)
{
  return ((ge_tCtx)gectx)->subpalette->get_select( text, filename);
}

static void ge_colorpalette_get_current( void *gectx, glow_eDrawType *fill_color,
	glow_eDrawType *border_color, glow_eDrawType *text_color)
{
  colpal_GetCurrentColors( ((ge_tCtx)gectx)->colorpalette_ctx, 
	fill_color, border_color, text_color);
}

static void ge_colorpalette_set_current( void *gectx, glow_eDrawType fill_color,
	glow_eDrawType border_color, glow_eDrawType text_color)
{
  colpal_SetCurrentColors( ((ge_tCtx)gectx)->colorpalette_ctx, 
	fill_color, border_color, text_color);
}

static void ge_subgraphs_close_cb( SubGraphs *subgraphs)
{
  ((ge_tCtx) subgraphs->parent_ctx)->subgraphs = NULL;
  delete subgraphs;
}

static void ge_status_msg( void *ge_ctx, double x, double y)
{
  Arg 		args[2];
  char		pos_str[200];
  XmString	cstr;
  ge_tCtx       gectx = (ge_tCtx) ge_ctx;
  glow_eMoveRestriction move_restriction = gectx->graph->get_move_restriction();
  int           equal_scale = gectx->graph->get_scale_equal();
  char		mr_str[20];
  char		es_str[20];
  char		em_str[20];
  static double old_x = 0;
  static double old_y = 0;

  if ( x == 0 && y == 0) {
    x = old_x;
    y = old_y;
  }
  old_x = x;
  old_y = y;

  switch( move_restriction) {
  case glow_eMoveRestriction_Vertical:
    strcpy( mr_str, "MoveVert");
    break;
  case glow_eMoveRestriction_Horizontal:
    strcpy( mr_str, "MoveHoriz");
    break;
  default:
    strcpy( mr_str, "         ");
  }

  if ( equal_scale)
    strcpy( es_str, "EqualScale");
  else
    strcpy( es_str, "          ");


  switch ( gectx->graph->get_mode()) {
  case grow_eMode_Rect:
    strcpy( em_str, "Rect     ");
    break;
  case grow_eMode_Line:
    strcpy( em_str, "Line     ");
    break;
  case grow_eMode_PolyLine:
    strcpy( em_str, "PolyLine ");
    break;
  case grow_eMode_Text:
    strcpy( em_str, "Text     ");
    break;
  case grow_eMode_Circle:
    strcpy( em_str, "Circle   ");
    break;
  case grow_eMode_Annot:
    strcpy( em_str, "Annot    ");
    break;
  case grow_eMode_ConPoint:
    strcpy( em_str, "ConPoint ");
    break;
  case grow_eMode_EditPolyLine:
    strcpy( em_str, "EditPolyL");
    break;
  case grow_eMode_RectRounded:
    strcpy( em_str, "RectRound");
    break;
  case grow_eMode_Scale:
    strcpy( em_str, "Scale    ");
    break;
  default:
    strcpy( em_str, "         ");
  }

  sprintf( pos_str, "%s %s %s %5.2f, %5.2f", em_str, mr_str, es_str, x, y);
  cstr=XmStringCreateLtoR( pos_str, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  // XtSetArg(args[1],XmNwidth, 280);
  XtSetValues( gectx->cursor_position, args, 1);
  XmStringFree( cstr);

  int width = 0;
  int height = 0;
  XtSetArg(args[0],XmNwidth, &width);
  XtSetArg(args[1],XmNheight, &height);
  XtGetValues( gectx->cursor_position, args, 2);
  if ( width != 320)
    XtResizeWidget( gectx->cursor_position, 320, height, 0);
}

static void ge_change_text_cb( void *ge_ctx, void *text_object, char *text)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open) 
  {
    XtUnmanageChild( gectx->cmd_input);
    XtManageChild( gectx->msg_label);
    ge_set_prompt( gectx, "");
    gectx->text_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtUnmanageChild( gectx->msg_label);
    XtManageChild( gectx->cmd_input);
  }
  ge_message( gectx, ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);

  XmTextSetString( gectx->cmd_input, text);
  XmTextSetInsertionPosition( gectx->cmd_input, strlen(text));
  XmTextSetSelection( gectx->cmd_input, 0, strlen(text), CurrentTime);
  ge_set_prompt( gectx, "Enter text >");
  gectx->text_input_open = 1;
  gectx->current_text_object = text_object;
}

static void ge_change_name_cb( void *ge_ctx, void *text_object, char *text)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->text_input_open || gectx->name_input_open || 
       gectx->value_input_open) 
  {
    XtUnmanageChild( gectx->cmd_input);
    ge_set_prompt( gectx, "");
    gectx->name_input_open = 0;
    gectx->text_input_open = 0;
    gectx->value_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtManageChild( gectx->cmd_input);
    XtUnmanageChild( gectx->msg_label);
  }
  ge_message( gectx, ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);
//  XtCallAcceptFocus( gectx->cmd_input, CurrentTime);

  XmTextSetString( gectx->cmd_input, text);
  XmTextSetInsertionPosition( gectx->cmd_input, strlen(text));
  XmTextSetSelection( gectx->cmd_input, 0, strlen(text), CurrentTime);
  ge_set_prompt( gectx, "Enter name >");
  gectx->name_input_open = 1;
  gectx->current_text_object = text_object;
}

static void ge_change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  Arg 		args[1];
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->text_input_open || gectx->value_input_open ||
       gectx->name_input_open) 
  {
    XtUnmanageChild( gectx->cmd_input);
    ge_set_prompt( gectx, "");
    gectx->value_input_open = 0;
    gectx->text_input_open = 0;
    gectx->name_input_open = 0;
    return;
  }

  if ( gectx->command_open)
    gectx->command_open = 0;
  else {
    XtUnmanageChild( gectx->msg_label);
    XtManageChild( gectx->cmd_input);
  }
  ge_message( gectx, ' ', "");
  XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);

  XtSetArg(args[0],XmNvalue, text);
  XtSetValues( gectx->cmd_input, args, 1);
  ge_set_prompt( gectx, "Enter value >");
  gectx->value_input_open = 1;
  gectx->current_value_object = value_object;
}

static int ge_command_cb( void *ge_ctx, char *command)
{
  printf( "** Command not defined in this environment\n  '%s'\n", command);
  return 0;
}

static int ge_get_plant_select_cb( void *ge_ctx, char *select_name)
{
#if LDH
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;
  pwr_sAttrRef	attrref;
  int		is_attrref;
  int		sts;
  char		str[120];
  char buff[256];

  if ( !gectx->ldhses) {
    sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), gectx->graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), XA_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }

  sts = plant_get_select( gectx->plantctx, &attrref, &is_attrref);
  if ( ODD( sts)) {
    if ( !wnav_format_selection( gectx->ldhses, attrref, 0, is_attrref, wnav_eSelectionMode_Normal,
				 0, 1, 1, buff))
      return 0;

    strcpy( select_name, buff);
    return 1;
  }
  else {
    sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), gectx->graph_atom);
    if ( ODD(sts))
      strcpy( select_name, str);
    else {
      sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), XA_STRING);
      if ( ODD(sts))
	strcpy( select_name, str);
    }
    return sts;
  }
#else
  pwr_tStatus sts;
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), gectx->graph_atom);
  if ( ODD(sts))
    strcpy( select_name, str);
  else {
    sts = wow_GetSelection( gectx->toplevel, str, sizeof(str), XA_STRING);
    if ( ODD(sts))
      strcpy( select_name, str);
  }
  return sts;
#endif
}

static void ge_open_yesnodia( ge_tCtx gectx, char *text, char *title, 
	void (*yes_cb)( ge_tCtx), void (*no_cb)( ge_tCtx))
{
  Arg 		args[3];
  XmString	cstr;

  if ( gectx->yesnodia_open) 
  {
    XtUnmanageChild( gectx->yesnodia_widget);
    gectx->yesnodia_open = 0;
    return;
  }

  XtManageChild( gectx->yesnodia_widget);

  ge_message( gectx, ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetArg(args[1], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") );
  XtSetValues( gectx->yesnodia_widget, args, 2);
  XmStringFree( cstr);
  gectx->yesnodia_open = 1;
  gectx->yesnodia_yes_cb = yes_cb;
  gectx->yesnodia_no_cb = no_cb;
}

static void ge_confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  Arg 		args[1];
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->confirm_open) 
  {
    XtUnmanageChild( gectx->confirm_widget);
    gectx->confirm_open = 0;
    return;
  }

  XtManageChild( gectx->confirm_widget);

  ge_message( gectx, ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetValues( gectx->confirm_widget, args, 1);
  gectx->confirm_open = 1;
  gectx->current_confirm_object = confirm_object;
}

static void ge_set_prompt( ge_tCtx gectx, char *prompt)
{
  Arg 		args[1];
  XmString	cstr;

  if ( strcmp( prompt, "") == 0)
    XtUnmanageChild( gectx->cmd_prompt);
  else {
    XtManageChild( gectx->cmd_prompt);
    cstr=XmStringCreateLtoR( prompt, "ISO8859-1");
    XtSetArg(args[0],XmNlabelString, cstr);
    XtSetValues( gectx->cmd_prompt, args, 1);
    XmStringFree( cstr);
  }
}

//
//  Callbackfunctions from menu entries
//
static void ge_activate_change_text( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->change_select_text();
}

static void ge_activate_change_name( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->change_select_name();
}

static void ge_activate_preview_start( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->init_trace();
  ge_set_title( gectx);
}

static void ge_activate_preview_stop( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->close_trace( 1);
  ge_set_title( gectx);
}

static void ge_activate_cut( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->cut();
}

static void ge_activate_copy( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->copy();
}

static void ge_activate_rotate( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ge_open_input_dialog( gectx, "Value (degrees)", "Rotate", "",
	ge_rotate);
}

static void ge_activate_rotate90( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->rotate( -90.0);
}

static void ge_activate_flip_vert( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->flip( glow_eFlipDirection_Vertical);
}

static void ge_activate_flip_horiz( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->flip( glow_eFlipDirection_Horizontal);
}

static void ge_activate_pop( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->pop_select();
}

static void ge_activate_push( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->push_select();
}

static void ge_activate_edit_polyline( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_mode( grow_eMode_EditPolyLine, false);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_scale_equal( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_scale_equal( 1);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_move_horizontal( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_move_restriction( glow_eMoveRestriction_Horizontal);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_move_vertical( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_move_restriction( glow_eMoveRestriction_Vertical);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_move_reset( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_move_restriction( glow_eMoveRestriction_No);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_align_horiz_up( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_Down);
}

static void ge_activate_align_horiz_down( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_Up);
}

static void ge_activate_align_horiz_center( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_CenterHoriz);
}

static void ge_activate_align_vert_left( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_Left);
}

static void ge_activate_align_vert_right( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_Right);
}

static void ge_activate_align_vert_center( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->align_select( glow_eAlignDirection_CenterVert);
}

static void ge_activate_equid_vert_up( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_Down);
}

static void ge_activate_equid_vert_down( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_Up);
}

static void ge_activate_equid_vert_center( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_CenterVert);
}

static void ge_activate_equid_horiz_left( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_Left);
}

static void ge_activate_equid_horiz_right( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_Right);
}

static void ge_activate_equid_horiz_center( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->equidistance_select( glow_eAlignDirection_CenterHoriz);
}

static void ge_activate_select_cons( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->select_all_cons();
}

static void ge_activate_select_objects( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->select_all_objects();
}

static void ge_activate_group( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;
  grow_tObject object;
  char last_group[80];
  char msg[80];

  sts = gectx->graph->group_select( &object, last_group);
  if ( sts == GLOW__NOSELECT)
    ge_message( gectx, 'E', "Select objects to form at group");
  else if ( sts == GLOW__GROUPCLASS)
    ge_message( gectx, 'E', "Unable to group this kind of object");
  else if ( sts == GE__RECALLDATA_FOUND) {
    gectx->recover_object =  object;
    strcpy( gectx->recover_name, last_group);
    sprintf( msg, "Do you want to recover dynamic properties for group %s", last_group);
    ge_open_yesnodia( gectx, 
        msg, 
        "Recover dynamic properties", 
	ge_recover_dynprop_yes_cb, ge_recover_dynprop_no_cb);
  }
}

static void ge_activate_ungroup( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;

  sts = gectx->graph->ungroup_select( 0);
  if ( sts == GE__GROUPDYNDATA) {

    ge_open_yesnodia( gectx, 
        "Group has dynamic properties that will be lost\nDo you want to ungroup", 
        "Ungroup", 
	ge_ungroup_yes_cb, ge_ungroup_no_cb);

  }  
}

static void ge_activate_connect( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;
  grow_tObject object;
  char	name[120];

  sts = gectx->graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    ge_message( gectx, 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    ge_message( gectx, 'E', "More than one object is selected");
    return;
  }
  sts = ge_get_plant_select_cb( (void *)gectx, name);
  if ( EVEN(sts))
  {
    ge_message( gectx, 'E', "Select an object in the plant hierarchy");
    return;
  }

  gectx->graph->connect( object, name, 0);
}

static void ge_activate_connectsecond( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;
  grow_tObject object;
  char	name[120];

  sts = gectx->graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    ge_message( gectx, 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    ge_message( gectx, 'E', "More than one object is selected");
    return;
  }
  sts = ge_get_plant_select_cb( (void *)gectx, name);
  if ( EVEN(sts))
  {
    ge_message( gectx, 'E', "Select an object in the plant hierarchy");
    return;
  }

  gectx->graph->connect( object, name, 1);
}

static void ge_activate_objectattributes( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;
  grow_tObject object;

  sts = gectx->graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    ge_message( gectx, 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    ge_message( gectx, 'E', "More than one object is selected");
    return;
  }
  gectx->graph->edit_attributes( object);
}

static void ge_activate_show_grid( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->graph->set_show_grid( data->set);
}

static void ge_activate_paste( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->paste();
}

static void ge_activate_command( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  Arg 		args[1];

  if ( gectx->command_open) 
  {
    XtUnmanageChild( gectx->cmd_input);
    ge_set_prompt( gectx, "");
    gectx->command_open = 0;
    return;
  }

  if ( gectx->text_input_open)
    gectx->text_input_open = 0;
  else if ( gectx->name_input_open)
    gectx->name_input_open = 0;
  else if ( gectx->value_input_open)
    gectx->text_input_open = 0;
  else {
    XtUnmanageChild( gectx->msg_label);
    XtManageChild( gectx->cmd_input);
  }
  ge_message( gectx, ' ', "");
  // XmProcessTraversal( gectx->cmd_input, XmTRAVERSE_CURRENT);
  // XtSetKeyboardFocus( gectx->toplevel, gectx->cmd_input);
  XtCallAcceptFocus( gectx->cmd_input, CurrentTime);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( gectx->cmd_input, args, 1);
  ge_set_prompt( gectx, "ge >        ");
  gectx->command_open = 1;
}

static void ge_activate_exit( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  if ( gectx->graph->is_modified())
  {
    ge_open_yesnodia( gectx, "Do you wan't to save changes", "Save", 
	ge_exit_save_cb, ge_exit_nosave_cb);
    return;
  }

  ge_del( (void *)gectx);
}

static void ge_activate_print( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->print( "pwrp_tmp:graph.ps");
}

static void ge_activate_new( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ge_clear_all( gectx);
}

static void ge_activate_save( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[40];

  if ( gectx->graph->trace_started) {
    ge_message( gectx, 'I', "Nothing to save");
    return;
  }

  gectx->graph->get_name( name);
  if ( strcmp( name, "") == 0)
  {
    if ( ! gectx->graph->is_subgraph())
      ge_open_input_dialog( gectx, "Graph name", "Save Graph", "",
	ge_save_graph);
    else
      ge_open_input_dialog( gectx, "SubGraph name", "Save SubGraph", "", 
	ge_save_graph);
  }
  else
    ge_save_graph( gectx, name);
}

static void ge_activate_save_as( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[40];

  if ( gectx->graph->trace_started) {
    ge_message( gectx, 'E', "Preview is active");
    return;
  }

  gectx->graph->get_name( name);
  if ( ! gectx->graph->is_subgraph())
    ge_open_input_dialog( gectx, "Graph name", "Save Graph", name, 
	ge_save_graph);
  else
    ge_open_input_dialog( gectx, "SubGraph name", "Save SubGraph", name,
	ge_save_graph);
}

static void ge_activate_export_javabean( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[80];
  char default_name[80];

  if ( ! gectx->graph->get_java_name( name))
  {
    gectx->graph->get_name( name);
    if ( strcmp( name, "") != 0)
    {
      if ( strncmp( name, "pwr_", 4) == 0) {
        strcpy( default_name, "Jop");
        strcat( default_name, &name[4]);
        default_name[3] = _toupper( default_name[3]);
      }
      else {
        strcpy( default_name, name);
        default_name[0] = _toupper( default_name[0]);
      }
    }
    else
      strcpy( default_name, "");
    ge_open_input_dialog( gectx, "JavaBean name", "Export JavaBean", 
	default_name, ge_export_javabean);
  }
  else
    ge_export_javabean( gectx, name);
}

static void ge_activate_export_javabean_as( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[80];
  char default_name[80];

  if ( !(gectx->graph->is_javaapplication() || gectx->graph->is_javaapplet())) {
    ge_message( gectx, 'I', "This graph is not java frame or applet");
    return;
  }

  if ( !gectx->graph->get_java_name( name))
  {
    gectx->graph->get_name( name);
    if ( strcmp( name, "") != 0)
    {
      if ( strncmp( name, "pwr_", 4) == 0) {
        strcpy( default_name, "Jop");
        strcat( default_name, &name[4]);
        default_name[3] = _toupper( default_name[3]);
      }
      else {
        strcpy( default_name, name);
        default_name[0] = _toupper( default_name[0]);
      }
    }
    else
      strcpy( default_name, "");
  }
  else
    strcpy( default_name, name);
  ge_open_input_dialog( gectx, "JavaBean name", "Export JavaBean", 
	default_name, ge_export_javabean);
}

static void ge_activate_export_gejava( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[80];
  char default_name[80];

  if ( !(gectx->graph->is_javaapplication() || gectx->graph->is_javaapplet())) {
    ge_message( gectx, 'I', "This graph is not java frame or applet");
    return;
  }

  if ( !gectx->graph->get_java_name( name))
  {
    gectx->graph->get_name( name);
    if ( strcmp( name, "") != 0)
    {
      if ( strncmp( name, "pwr_c_", 6) == 0) {
        strcpy( default_name, "Jopc");
        strcat( default_name, &name[6]);
        default_name[4] = _toupper( default_name[4]);
      }
      else if ( strncmp( name, "pwr_", 4) == 0)
      {
        strcpy( default_name, "Jop");
        strcat( default_name, &name[4]);
        default_name[3] = _toupper( default_name[3]);
      }
      else
      {
        strcpy( default_name, name);
        default_name[0] = _toupper( default_name[0]);
      }
    }
    else
      strcpy( default_name, "");
    ge_open_input_dialog( gectx, "Java name", "Export GeJava",
	default_name, ge_export_gejava);
  }
  else
    ge_export_gejava( gectx, name);
}

static void ge_activate_export_gejava_as( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[80];
  char default_name[80];

  if ( !gectx->graph->get_java_name( name))
  {
    gectx->graph->get_name( name);
    if ( strcmp( name, "") != 0)
    {
      if ( strncmp( name, "pwr_c_", 6) == 0) {
        strcpy( default_name, "Jopc");
        strcat( default_name, &name[6]);
        default_name[4] = _toupper( default_name[4]);
      }
      else if ( strncmp( name, "pwr_", 4) == 0)
      {
        strcpy( default_name, "Jop");
        strcat( default_name, &name[4]);
        default_name[3] = _toupper( default_name[3]);
      }
      else
      {
        strcpy( default_name, name);
        default_name[0] = _toupper( default_name[0]);
      }
    }
    else
      strcpy( default_name, "");
  }
  else
    strcpy( default_name, name);
  ge_open_input_dialog( gectx, "Java name", "Export GeJava", 
	default_name, ge_export_gejava);
}

static void ge_activate_export_java( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  if ( gectx->graph->is_subgraph())
    ge_activate_export_javabean( w, gectx, data);
  else
    ge_activate_export_gejava( w, gectx, data);
}

static void ge_activate_export_java_as( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  if ( gectx->graph->is_subgraph())
    ge_activate_export_javabean_as( w, gectx, data);
  else
    ge_activate_export_gejava_as( w, gectx, data);
}

static void ge_activate_generate_web( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;

  // gectx->graph->generate_web();
  if ( !gectx->ldhses) {
    ge_message( gectx, 'E', "Ge is not connected to workbench");
    return;
  }

  sts = Graph::generate_web( gectx->ldhses);
  if ( sts == GE__NONODEOBJECT)
    ge_message( gectx, 'E', "Can't find $Node object");
  else if ( sts == GE__NOWEBHANDLER)
    ge_message( gectx, 'E', "Can't find WebHandler object");
  else if ( sts == GE__WEBHANDLERS)
    ge_message( gectx, 'E', "More than one WebHandler object found");
  else if ( EVEN(sts))
    ge_message( gectx, 'E', "Error from generate web");
  else
    ge_message( gectx, 'I', "Web-pages generated");
}

static void ge_activate_creanextpage( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char old_name[40];
  char name[40];
  char next_name[40];
  int page;
  char *s;

  if ( ! gectx->graph->is_subgraph()) {
    ge_message( gectx, 'E', "This is not a subgraph");
    return;
  }

  if ( gectx->graph->is_modified()) {
    ge_message( gectx, 'E', "Subgraph is not saved");
    return;
  }
    
  // Get next subgraph name
  gectx->graph->get_name( old_name);
  strcpy( name, old_name);
  if ( (s = strstr( name, "__p")) && 
       (sscanf( s+3, "%d", &page) == 1)) 
    sprintf( s+3, "%d", page+1);
  else
    strcat( name, "__p2");

  // Store next name in graph
  gectx->graph->get_next_subgraph( next_name);
  if ( strcmp( next_name, name) == 0) {
    // Next subgraph is already created, check file...
    ge_message( gectx, 'E', "Subgraph is already created");
    return;
  }

  gectx->graph->set_next_subgraph( name);
  ge_save_graph( gectx, old_name);
  ge_prevtable_insert( gectx, name, old_name);

  gectx->graph->set_name( name);
  gectx->graph->set_next_subgraph( "");
  gectx->graph->set_java_name( "");
  ge_set_title( gectx);

  ge_message( gectx, 'I', "New page created");
}

static void ge_activate_nextpage( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char next[40];
  char name[40];

  if ( ! gectx->graph->is_subgraph())
  {
    ge_message( gectx, 'E', "Only subgraphs can have a next page");
    return;
  }

  if ( gectx->graph->is_modified())
  {
    ge_message( gectx, 'E', "Subgraph is not saved");
    return;
  }
    
  gectx->graph->get_next_subgraph( next);
  if ( strcmp( next, "") == 0)
  {
    ge_message( gectx, 'E', "No next page is found");
    return;
  }

  // Store this to enable previous page later
  gectx->graph->get_name( name);
  ge_prevtable_insert( gectx, next, name);

  gectx->graph->set_nodraw();
  gectx->graph->store_geometry();
  ge_open_graph( gectx, next);
  gectx->graph->restore_geometry();
  gectx->graph->reset_nodraw();
  gectx->graph->redraw();
}

static void ge_activate_prevpage( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char name[40];
  char prev[40];
  int sts;

  if ( gectx->graph->is_modified())
  {
    ge_message( gectx, 'E', "Subgraph is not saved");
    return;
  }
    
  gectx->graph->get_name( name);
  sts = ge_prevtable_get( gectx, name, prev);
  if ( EVEN(sts))
  {
    ge_message( gectx, 'E', "No previous page is found");
    return;
  }

  gectx->graph->set_nodraw();
  gectx->graph->store_geometry();
  ge_open_graph( gectx, prev);
  gectx->graph->restore_geometry();
  gectx->graph->reset_nodraw();
  gectx->graph->redraw();
}

static void ge_activate_graph_attr( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  if ( ! gectx->graph->is_subgraph())
    gectx->graph->edit_graph_attributes();
  else
    gectx->graph->edit_subgraph_attributes();
}

static void ge_open_list_cb( void *ctx, char *text)
{
  ge_open_graph( (ge_tCtx) ctx, text);
}

static int ge_sort_files( const void *file1, const void *file2)
{
  return ( strcmp( (char *)file1, (char *)file2));
}

static void ge_activate_open( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ge_prevtable_clear( gectx);
  int file_cnt;
  int allocated, old_allocated;
  pwr_tString80 *file_p;
  pwr_tString80 *old_file_p;
  char found_file[80];
  char fname[80];
  int sts;
  char dev[80];
  char dir[80];
  char file[80];
  char type[80];
  int version;

  // Get the pwg files and order them
  dcli_translate_filename( fname, "$pwrp_pop/*.pwg");

  file_cnt = 0;
  allocated = 0;
  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {
    if ( strstr( found_file, "__p")) { 
      // Skip subgraph pages
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
      continue;
    }
    file_cnt++;
    if ( file_cnt > allocated - 1) {
      if ( allocated == 0) {
        allocated = 100;
        file_p = (pwr_tString80 *) malloc( allocated * sizeof(*file_p));
      }
      else {
        old_file_p = file_p;
        old_allocated = allocated;
        allocated += 100;
        file_p = (pwr_tString80 *) malloc( allocated * sizeof(*file_p));
        memcpy( file_p, old_file_p, old_allocated * sizeof(*file_p));
        free( old_file_p);
      }
    }
    dcli_parse_filename( found_file, dev, dir, file, type, &version);
    strcpy( file_p[file_cnt - 1], file);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);

  if ( !file_cnt) {
    return;
  }

  strcpy( file_p[file_cnt], "");
    
  qsort( file_p, file_cnt, sizeof(*file_p), ge_sort_files);

  wow_CreateList( gectx->toplevel, "Open Graph", 
         (char *)file_p, ge_open_list_cb, (void *)gectx);

  free( file_p);

  //  ge_open_input_dialog( gectx, "Graph name", "Open Graph", "", 
  //	ge_open_graph);
}

static void ge_activate_subgraphs( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int sts;

  if ( gectx->subgraphs)
  {
    ge_message( gectx, 'I', "Loaded SubGraphs already active");
    return;
  }
  gectx->subgraphs = new SubGraphs( gectx, gectx->toplevel, "SubGraphs",
		(void *) gectx->graph->grow->ctx,
		&gectx->subgraphs_widget, &sts);
  gectx->subgraphs->message_cb = &ge_message;
  gectx->subgraphs->close_cb = &ge_subgraphs_close_cb;
}

static void ge_activate_rect( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_Rect, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_rectrounded( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_RectRounded, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_line( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_Line, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_polyline( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_PolyLine, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_circle( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_Circle, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_text( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_Text, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_annot( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_Annot, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_conpoint( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  bool keep = false;

  if ( data->event->type == ButtonRelease && 
       data->event->xbutton.state & ShiftMask)
    keep = true;

  gectx->graph->set_mode( grow_eMode_ConPoint, keep);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_fill( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->graph->set_fill( data->set);
  gectx->graph->set_select_fill( data->set);
}

static void ge_activate_border( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->graph->set_border( data->set);
  gectx->graph->set_select_border( data->set);
}

static void ge_activate_shadow( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->graph->set_shadow( data->set);
  gectx->graph->set_select_shadow( data->set);
}

static void ge_activate_incr_lightness( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_lightness(1);
}

static void ge_activate_decr_lightness( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_lightness( -1);
}

static void ge_activate_incr_intensity( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_intensity(1);
}

static void ge_activate_decr_intensity( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_intensity( -1);
}

static void ge_activate_incr_shift( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_shift(1);
}

static void ge_activate_decr_shift( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->incr_select_color_shift( -1);
}

static void ge_activate_scale( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_mode( grow_eMode_Scale, false);
  ge_status_msg( gectx, 0, 0);
}

static void ge_activate_grid( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  gectx->graph->set_grid( data->set);
}

static void ge_activate_linewidth_1( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 1);
    gectx->graph->set_select_linewidth( 1);
  }
}

static void ge_activate_linewidth_2( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 2);
    gectx->graph->set_select_linewidth( 2);
  }
}

static void ge_activate_linewidth_3( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 3);
    gectx->graph->set_select_linewidth( 3);
  }
}
static void ge_activate_linewidth_4( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 4);
    gectx->graph->set_select_linewidth( 4);
  }
}
static void ge_activate_linewidth_5( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 5);
    gectx->graph->set_select_linewidth( 5);
  }
}
static void ge_activate_linewidth_6( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 6);
    gectx->graph->set_select_linewidth( 6);
  }
}
static void ge_activate_linewidth_7( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 7);
    gectx->graph->set_select_linewidth( 7);
  }
}
static void ge_activate_linewidth_8( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linewidth( 8);
    gectx->graph->set_select_linewidth( 8);
  }
}

static void ge_activate_linetype1( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_Solid);
    gectx->graph->set_select_linetype( glow_eLineType_Solid);
  }
}

static void ge_activate_linetype2( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_Dashed1);
    gectx->graph->set_select_linetype( glow_eLineType_Dashed1);
  }
}

static void ge_activate_linetype3( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_Dashed2);
    gectx->graph->set_select_linetype( glow_eLineType_Dashed2);
  }
}

static void ge_activate_linetype4( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_Dashed3);
    gectx->graph->set_select_linetype( glow_eLineType_Dashed3);
  }
}

static void ge_activate_linetype5( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_Dotted);
    gectx->graph->set_select_linetype( glow_eLineType_Dotted);
  }
}

static void ge_activate_linetype6( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_DotDashed1);
    gectx->graph->set_select_linetype( glow_eLineType_DotDashed1);
  }
}

static void ge_activate_linetype7( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_linetype( glow_eLineType_DotDashed2);
    gectx->graph->set_select_linetype( glow_eLineType_DotDashed2);
  }
}

static void ge_activate_gridsize_4( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
    gectx->graph->set_gridsize( 1);
}

static void ge_activate_gridsize_3( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
    gectx->graph->set_gridsize( 0.5);
}

static void ge_activate_gridsize_2( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
    gectx->graph->set_gridsize( 0.2);
}

static void ge_activate_gridsize_1( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
    gectx->graph->set_gridsize( 0.1);
}

static void ge_activate_textsize_0( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 0);
    gectx->graph->set_select_textsize( 0);
  }
}
static void ge_activate_textsize_1( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 1);
    gectx->graph->set_select_textsize( 1);
  }
}

static void ge_activate_textsize_2( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 2);
    gectx->graph->set_select_textsize( 2);
  }
}

static void ge_activate_textsize_3( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 3);
    gectx->graph->set_select_textsize( 3);
  }
}

static void ge_activate_textsize_4( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 4);
    gectx->graph->set_select_textsize( 4);
  }
}

static void ge_activate_textsize_5( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textsize( 5);
    gectx->graph->set_select_textsize( 5);
  }
}

static void ge_activate_textbold( Widget w, ge_tCtx gectx, XmToggleButtonCallbackStruct *data)
{
  if ( data->set)
  {
    gectx->graph->set_textbold( 1);
    gectx->graph->set_select_textbold( 1);
  }
  else
  {
    gectx->graph->set_textbold( 0);
    gectx->graph->set_select_textbold( 0);
  }
}

static void ge_activate_zoom_in( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->zoom( 1.2);
}

static void ge_activate_zoom_out( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->zoom( 5.0/6);
}

static void ge_activate_zoom_reset( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->unzoom();
}

static void ge_activate_concorner_right( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_concorner( glow_eCorner_Right);
}

static void ge_activate_concorner_rounded( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_concorner( glow_eCorner_Rounded);
}

static void ge_activate_round_amount_1( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_corner_round_amount( 0.2);
}

static void ge_activate_round_amount_2( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_corner_round_amount( 0.5);
}

static void ge_activate_round_amount_3( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_corner_round_amount( 1.0);
}

static void ge_activate_round_amount_4( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_corner_round_amount( 2.0);
}

static void ge_activate_round_amount_5( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_corner_round_amount( 4.0);
}

static void ge_activate_contype_straight( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_Straight);
}

static void ge_activate_contype_routed( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_Routed);
}

static void ge_activate_contype_stronearr( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_StraightOneArrow);
}

static void ge_activate_contype_stepdiv( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_StepDiv);
}

static void ge_activate_contype_stepconv( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_StepConv);
}

static void ge_activate_contype_transdiv( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_TransDiv);
}

static void ge_activate_contype_transconv( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_contype( glow_eConType_TransConv);
}

static void ge_activate_condir_center( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_condir( glow_eDirection_Center);
}

static void ge_activate_condir_left( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_condir( glow_eDirection_Left);
}

static void ge_activate_condir_right( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_condir( glow_eDirection_Right);
}

static void ge_activate_condir_up( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_condir( glow_eDirection_Up);
}

static void ge_activate_condir_down( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_condir( glow_eDirection_Down);
}

static void ge_activate_background_color( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph->set_background_color();
}

static void ge_activate_help( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void ge_create_cursor_position( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->cursor_position = w;
}
static void ge_create_msg_label( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->msg_label = w;
}
static void ge_create_cmd_prompt( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->cmd_prompt = w;
}
static void ge_create_cmd_input( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, gectx);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) ge_valchanged_cmd_input, mrm_eUtility_Ge);

  gectx->cmd_input = w;
}
static void ge_create_graph_form( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph_form = w;
}
static void ge_create_main_pane( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->main_pane = w;
}
static void ge_create_palette_pane( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->palette_pane = w;
}
static void ge_create_widget_cb( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch ( key) {
    case 1:
      gectx->grid_on_w = w;
      break;
    case 2:
      gectx->grid_size_w = w;
      break;
    case 3:
      gectx->grid_size_10_w = w;
      break;
    case 4:
      gectx->grid_size_05_w = w;
      break;
    case 5:
      gectx->grid_size_02_w = w;
      break;
    case 6:
      gectx->grid_size_01_w = w;
      break;
    case 7:
      gectx->show_grid_w = w;
      break;
  }
}
static void ge_create_india_label( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->india_label = w;
}
static void ge_create_india_text( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->india_text = w;
}
static void ge_activate_india_ok( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( gectx->india_text);
  XtUnmanageChild( gectx->india_widget);

  (gectx->india_ok_cb)( gectx, value);
}
static void ge_activate_india_cancel( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( gectx->india_widget);
}
static void ge_activate_yesnodia_yes( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{

  XtUnmanageChild( gectx->yesnodia_widget);
  gectx->yesnodia_open = 0;

  (gectx->yesnodia_yes_cb)( gectx);
}
static void ge_activate_yesnodia_no( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{

  XtUnmanageChild( gectx->yesnodia_widget);
  gectx->yesnodia_open = 0;

  (gectx->yesnodia_no_cb)( gectx);
}
static void ge_activate_yesnodia_cancel( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{

  XtUnmanageChild( gectx->yesnodia_widget);
  gectx->yesnodia_open = 0;
}
static void ge_activate_confirm_ok( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( gectx->confirm_widget);
  gectx->confirm_open = 0;
  gectx->graph->confirm_ok( gectx->current_confirm_object);
}
static void ge_activate_confirm_cancel( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->confirm_open = 0;
  XtUnmanageChild( gectx->confirm_widget);
}
static void ge_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  ge_tCtx gectx;

  XtSetArg    (args[0], XmNuserData, &gectx);
  XtGetValues (w, args, 1);

  if ( gectx && gectx->graph)
    gectx->graph->set_inputfocus(1);
}

static void ge_valchanged_cmd_input( Widget w, XEvent *event)
{
  ge_tCtx 	gectx;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &gectx);
  XtGetValues(w, args, 1);

  if ( gectx->value_input_open)
    sts = mrm_TextInput( w, event, (char *)gectx->value_recall,
	sizeof(gectx->value_recall[0]),
	sizeof( gectx->value_recall)/sizeof(gectx->value_recall[0]),
	&gectx->value_current_recall);
  else if ( gectx->name_input_open)
    sts = mrm_TextInput( w, event, (char *)gectx->name_recall,
	sizeof(gectx->name_recall[0]),
	sizeof( gectx->name_recall)/sizeof(gectx->name_recall[0]),
	&gectx->name_current_recall);
  else if ( gectx->text_input_open)
    sts = mrm_TextInput( w, event, (char *)gectx->text_recall, sizeof(gectx->text_recall[0]),
	sizeof( gectx->text_recall)/sizeof(gectx->text_recall[0]),
	&gectx->text_current_recall);
  else
    sts = mrm_TextInput( w, event, (char *)gectx->cmd_recall, sizeof(gectx->cmd_recall[0]),
	sizeof( gectx->cmd_recall)/sizeof(gectx->cmd_recall[0]),
	&gectx->cmd_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( gectx->text_input_open)
    {
      gectx->graph->change_text( gectx->current_text_object, text);
      XtUnmanageChild( w);
      ge_set_prompt( gectx, "");
      gectx->text_input_open = 0;
    }
    else if ( gectx->name_input_open)
    {
      gectx->graph->change_name( gectx->current_text_object, text);
      XtUnmanageChild( w);
      ge_set_prompt( gectx, "");
      gectx->name_input_open = 0;
    }
    else if ( gectx->value_input_open)
    {
      gectx->graph->change_value( gectx->current_value_object, text);
      XtUnmanageChild( w);
      ge_set_prompt( gectx, "");
      gectx->value_input_open = 0;
    }
    else if ( gectx->command_open)
    {
      sts = gectx->graph->command( text);
      XtUnmanageChild( w);
      ge_set_prompt( gectx, "");
      gectx->command_open = 0;
    }
  }
}

//
// Callbacks from colorpalette
//
static int ge_colorpalette_cb( GlowCtx *ctx, glow_tEvent event)
{
  ge_tCtx	gectx;

  colpal_GetCtxUserData( (ColPalCtx *)ctx, (void **) &gectx);

  switch ( event->event)
  {
    case glow_eEvent_MB1Click:
      if ( event->any.type == glow_eEventType_ColorTone)
      {
        gectx->graph->set_select_color_tone( event->colortone.tone);
      }
      else
        gectx->graph->set_select_fill_color();
      break;
    case glow_eEvent_MB1ClickShift:
      gectx->graph->set_select_text_color();
      break;
    case glow_eEvent_MB2Click:
      gectx->graph->set_select_border_color();
      break;
    default:
      ;
  }
  return 1;
}

static int ge_init_colorpalette_cb( GlowCtx *fctx, void *client_data)
{
  ge_tCtx gectx = (ge_tCtx) client_data;
  gectx->colorpalette_ctx = (colpal_tCtx) fctx;
  colpal_SetCtxUserData( gectx->colorpalette_ctx, gectx);

  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB1Click, 
	glow_eEventType_CallBack, ge_colorpalette_cb);
  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB1ClickShift, 
	glow_eEventType_CallBack, ge_colorpalette_cb);
  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB2Click, 
	glow_eEventType_CallBack, ge_colorpalette_cb);

  return 1;
}

static int ge_get_ldhses_cb( void *ctx, ldh_tSesContext *ldhses, int load)
{
#if LDH
  ge_tCtx gectx = (ge_tCtx) ctx;
  ldh_tWBContext wbctx; 
  int	sts;
  pwr_tVolumeId	volid = 0;
  pwr_tClassId	classid;
  ldh_tVolContext 	volctx;

  if ( gectx->ldhses)
  {
    *ldhses = gectx->ldhses;
    return 1;
  }
  else if ( !load)
    return 0;
  else
  {
    // Open workbench and attatch some volume
    sts = ldh_OpenWB( &wbctx, 0);
    if ( EVEN(sts)) return sts;

    // Attach first rootvolume, or if no rootvolume exist some other volume
    sts = ldh_GetVolumeList( wbctx, &volid);
    while ( ODD(sts))
    {
      sts = ldh_GetVolumeClass( wbctx, volid, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_eClass_RootVolume)
        break;
      sts = ldh_GetNextVolume( wbctx, volid, &volid);
    }
    if ( volid == 0) return sts;

    // Open ldh session
    sts = ldh_AttachVolume( wbctx, volid, &volctx);
    if ( EVEN(sts)) return sts;

    sts = ldh_OpenSession( ldhses, volctx, ldh_eAccess_ReadWrite,
	ldh_eUtility_Pwr);
    if ( EVEN(sts)) return sts;
  }
  return 1;
#else
  return 0;
#endif
}

static int ge_traverse_focus( void *ctx, void *component)
{
  ge_tCtx gectx = (ge_tCtx) ctx;

  if ( component == (void *) gectx->graph)
  {
    ge_set_focus( gectx, gectx->subpalette);
  }
  else if ( component == (void *) gectx->subpalette)
  {
    ge_set_focus( gectx, gectx->graph);
  }
  else
    return 0;
  return 1;
}

static int ge_set_focus_cb( void *ctx, void *component)
{
  return ge_set_focus( (ge_tCtx)ctx, component);
}

static int ge_set_focus( ge_tCtx gectx, void *component)
{
  if ( component == (void *)gectx->graph)
  {
    gectx->graph->set_inputfocus( 1);
    gectx->subpalette->set_inputfocus( 0);
    gectx->focused_component = component;
  }
  else if ( component == (void *)gectx->subpalette)
  {
    gectx->graph->set_inputfocus( 0);
    gectx->subpalette->set_inputfocus( 1);
    gectx->focused_component = component;
  }
  return 1;
}

int ge_generate_web( ldh_tSesContext ldhses)
{
  return Graph::generate_web( ldhses);
}

int ge_command( void *ge_ctx, char *cmd)
{
  ge_tCtx gectx = (ge_tCtx) ge_ctx;
  return gectx->graph->command( cmd);
}

void ge_message( void *ge_ctx, pwr_tStatus sts)
{
  ge_tCtx gectx = (ge_tCtx) ge_ctx;
  gectx->graph->message( sts);
}

static void ge_update( ge_tCtx gectx)
{
  Arg 	args[2];

  XmToggleButtonSetState(gectx->grid_on_w, gectx->graph->grid, 0);
  XmToggleButtonSetState(gectx->show_grid_w, gectx->graph->get_show_grid(), 0);

  if ( fabs( gectx->graph->grid_size_x - 0.1) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, gectx->grid_size_01_w);
  else if ( fabs( gectx->graph->grid_size_x - 0.2) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, gectx->grid_size_02_w);
  else if ( fabs( gectx->graph->grid_size_x - 0.5) < DBL_EPSILON)
    XtSetArg(args[0], XmNmenuHistory, gectx->grid_size_05_w);
  else
    XtSetArg(args[0], XmNmenuHistory, gectx->grid_size_10_w);
  XtSetValues( gectx->grid_size_w, args, 1);
}

void ge_del( void *ge_ctx)
{
  ge_tCtx gectx = (ge_tCtx) ge_ctx;
  int exit_when_close = gectx->exit_when_close;

#ifdef LDH
  if ( gectx->ldhses)
    ldh_CloseSession( gectx->ldhses);
#endif

  if ( gectx->subgraphs)
    delete gectx->subgraphs;
  delete gectx->graph;
  XtDestroyWidget( gectx->toplevel);

  if ( exit_when_close)
    exit(0);

}

void *ge_new( 	void 	*parent_ctx, 
		Widget 	parent_widget,
		ldh_tSesContext	ldhses,
		int	exit_when_close,
		char	*graph_name)
{
  char		uid_filename[120] = {"pwr_exe:ge.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  ge_tCtx	gectx;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	ge_widget;
  char		name[] = "PwR Ge";
  int		path_cnt;
  char		*path;
  char		*s;
  char		tmp_name[80];
  char          systemname[80];

  static char translations[] =
    "<FocusIn>: ge_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"ge_inputfocus",      (XtActionProc) ge_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "ge_ctx", 0 },
	{"ge_activate_exit",(caddr_t)ge_activate_exit },
	{"ge_activate_print",(caddr_t)ge_activate_print },
	{"ge_activate_new",(caddr_t)ge_activate_new },
	{"ge_activate_save",(caddr_t)ge_activate_save },
	{"ge_activate_save_as",(caddr_t)ge_activate_save_as },
	{"ge_activate_graph_attr",(caddr_t)ge_activate_graph_attr },
	{"ge_activate_open",(caddr_t)ge_activate_open },
	{"ge_activate_export_java",(caddr_t)ge_activate_export_java },
	{"ge_activate_export_java_as",(caddr_t)ge_activate_export_java_as },
	{"ge_activate_generate_web",(caddr_t)ge_activate_generate_web },
	{"ge_activate_creanextpage",(caddr_t)ge_activate_creanextpage },
	{"ge_activate_nextpage",(caddr_t)ge_activate_nextpage },
	{"ge_activate_prevpage",(caddr_t)ge_activate_prevpage },
	{"ge_activate_preview_start",(caddr_t)ge_activate_preview_start },
	{"ge_activate_preview_stop",(caddr_t)ge_activate_preview_stop },
	{"ge_activate_subgraphs",(caddr_t)ge_activate_subgraphs },
	{"ge_activate_change_text",(caddr_t)ge_activate_change_text },
	{"ge_activate_change_name",(caddr_t)ge_activate_change_name },
	{"ge_activate_pop",(caddr_t)ge_activate_pop },
	{"ge_activate_push",(caddr_t)ge_activate_push },
	{"ge_activate_edit_polyline",(caddr_t)ge_activate_edit_polyline },
	{"ge_activate_scale_equal",(caddr_t)ge_activate_scale_equal },
	{"ge_activate_move_horizontal",(caddr_t)ge_activate_move_horizontal },
	{"ge_activate_move_vertical",(caddr_t)ge_activate_move_vertical },
	{"ge_activate_move_reset",(caddr_t)ge_activate_move_reset },
	{"ge_activate_align_vert_left",(caddr_t)ge_activate_align_vert_left },
	{"ge_activate_align_vert_right",(caddr_t)ge_activate_align_vert_right },
	{"ge_activate_align_vert_center",(caddr_t)ge_activate_align_vert_center },
	{"ge_activate_align_horiz_up",(caddr_t)ge_activate_align_horiz_up },
	{"ge_activate_align_horiz_down",(caddr_t)ge_activate_align_horiz_down },
	{"ge_activate_align_horiz_center",(caddr_t)ge_activate_align_horiz_center },
	{"ge_activate_equid_horiz_left",(caddr_t)ge_activate_equid_horiz_left },
	{"ge_activate_equid_horiz_right",(caddr_t)ge_activate_equid_horiz_right },
	{"ge_activate_equid_horiz_center",(caddr_t)ge_activate_equid_horiz_center },
	{"ge_activate_equid_vert_up",(caddr_t)ge_activate_equid_vert_up },
	{"ge_activate_equid_vert_down",(caddr_t)ge_activate_equid_vert_down },
	{"ge_activate_equid_vert_center",(caddr_t)ge_activate_equid_vert_center },
	{"ge_activate_select_cons",(caddr_t)ge_activate_select_cons },
	{"ge_activate_select_objects",(caddr_t)ge_activate_select_objects },
	{"ge_activate_group",(caddr_t)ge_activate_group },
	{"ge_activate_ungroup",(caddr_t)ge_activate_ungroup },
	{"ge_activate_connect",(caddr_t)ge_activate_connect },
	{"ge_activate_connectsecond",(caddr_t)ge_activate_connectsecond },
	{"ge_activate_objectattributes",(caddr_t)ge_activate_objectattributes },
	{"ge_activate_show_grid",(caddr_t)ge_activate_show_grid },
	{"ge_activate_cut",(caddr_t)ge_activate_cut },
	{"ge_activate_copy",(caddr_t)ge_activate_copy },
	{"ge_activate_paste",(caddr_t)ge_activate_paste },
	{"ge_activate_rotate",(caddr_t)ge_activate_rotate },
	{"ge_activate_rotate90",(caddr_t)ge_activate_rotate90 },
	{"ge_activate_flip_horiz",(caddr_t)ge_activate_flip_horiz },
	{"ge_activate_flip_vert",(caddr_t)ge_activate_flip_vert },
	{"ge_activate_command",(caddr_t)ge_activate_command },
	{"ge_activate_rect",(caddr_t)ge_activate_rect },
	{"ge_activate_rectrounded",(caddr_t)ge_activate_rectrounded },
	{"ge_activate_line",(caddr_t)ge_activate_line },
	{"ge_activate_polyline",(caddr_t)ge_activate_polyline },
	{"ge_activate_circle",(caddr_t)ge_activate_circle },
	{"ge_activate_text",(caddr_t)ge_activate_text },
	{"ge_activate_annot",(caddr_t)ge_activate_annot },
	{"ge_activate_conpoint",(caddr_t)ge_activate_conpoint },
	{"ge_activate_fill",(caddr_t)ge_activate_fill },
	{"ge_activate_border",(caddr_t)ge_activate_border },
	{"ge_activate_shadow",(caddr_t)ge_activate_shadow },
	{"ge_activate_incr_lightness",(caddr_t)ge_activate_incr_lightness },
	{"ge_activate_decr_lightness",(caddr_t)ge_activate_decr_lightness },
	{"ge_activate_incr_intensity",(caddr_t)ge_activate_incr_intensity },
	{"ge_activate_decr_intensity",(caddr_t)ge_activate_decr_intensity },
	{"ge_activate_incr_shift",(caddr_t)ge_activate_incr_shift },
	{"ge_activate_decr_shift",(caddr_t)ge_activate_decr_shift },
	{"ge_activate_scale",(caddr_t)ge_activate_scale },
	{"ge_activate_grid",(caddr_t)ge_activate_grid },
	{"ge_activate_linewidth_1",(caddr_t)ge_activate_linewidth_1 },
	{"ge_activate_linewidth_2",(caddr_t)ge_activate_linewidth_2 },
	{"ge_activate_linewidth_3",(caddr_t)ge_activate_linewidth_3 },
	{"ge_activate_linewidth_4",(caddr_t)ge_activate_linewidth_4 },
	{"ge_activate_linewidth_5",(caddr_t)ge_activate_linewidth_5 },
	{"ge_activate_linewidth_6",(caddr_t)ge_activate_linewidth_6 },
	{"ge_activate_linewidth_7",(caddr_t)ge_activate_linewidth_7 },
	{"ge_activate_linewidth_8",(caddr_t)ge_activate_linewidth_8 },
	{"ge_activate_linetype1",(caddr_t)ge_activate_linetype1 },
	{"ge_activate_linetype2",(caddr_t)ge_activate_linetype2 },
	{"ge_activate_linetype3",(caddr_t)ge_activate_linetype3 },
	{"ge_activate_linetype4",(caddr_t)ge_activate_linetype4 },
	{"ge_activate_linetype5",(caddr_t)ge_activate_linetype5 },
	{"ge_activate_linetype6",(caddr_t)ge_activate_linetype6 },
	{"ge_activate_linetype7",(caddr_t)ge_activate_linetype7 },
	{"ge_activate_textsize_0",(caddr_t)ge_activate_textsize_0 },
	{"ge_activate_textsize_1",(caddr_t)ge_activate_textsize_1 },
	{"ge_activate_textsize_2",(caddr_t)ge_activate_textsize_2 },
	{"ge_activate_textsize_3",(caddr_t)ge_activate_textsize_3 },
	{"ge_activate_textsize_4",(caddr_t)ge_activate_textsize_4 },
	{"ge_activate_textsize_5",(caddr_t)ge_activate_textsize_5 },
	{"ge_activate_textbold",(caddr_t)ge_activate_textbold },
	{"ge_activate_gridsize_1",(caddr_t)ge_activate_gridsize_1 },
	{"ge_activate_gridsize_2",(caddr_t)ge_activate_gridsize_2 },
	{"ge_activate_gridsize_3",(caddr_t)ge_activate_gridsize_3 },
	{"ge_activate_gridsize_4",(caddr_t)ge_activate_gridsize_4 },
	{"ge_activate_zoom_in",(caddr_t)ge_activate_zoom_in },
	{"ge_activate_zoom_out",(caddr_t)ge_activate_zoom_out },
	{"ge_activate_zoom_reset",(caddr_t)ge_activate_zoom_reset },
	{"ge_activate_help",(caddr_t)ge_activate_help },
	{"ge_create_widget_cb",(caddr_t)ge_create_widget_cb },
	{"ge_create_cursor_position",(caddr_t)ge_create_cursor_position },
	{"ge_create_msg_label",(caddr_t)ge_create_msg_label },
	{"ge_create_cmd_prompt",(caddr_t)ge_create_cmd_prompt },
	{"ge_create_cmd_input",(caddr_t)ge_create_cmd_input },
	{"ge_create_graph_form",(caddr_t)ge_create_graph_form },
	{"ge_create_main_pane",(caddr_t)ge_create_main_pane },
	{"ge_create_palette_pane",(caddr_t)ge_create_palette_pane },
	{"ge_activate_concorner_right",(caddr_t)ge_activate_concorner_right },
	{"ge_activate_concorner_rounded",(caddr_t)ge_activate_concorner_rounded },
	{"ge_activate_round_amount_1",(caddr_t)ge_activate_round_amount_1 },
	{"ge_activate_round_amount_2",(caddr_t)ge_activate_round_amount_2 },
	{"ge_activate_round_amount_3",(caddr_t)ge_activate_round_amount_3 },
	{"ge_activate_round_amount_4",(caddr_t)ge_activate_round_amount_4 },
	{"ge_activate_round_amount_5",(caddr_t)ge_activate_round_amount_5 },
	{"ge_activate_contype_straight",(caddr_t)ge_activate_contype_straight },
	{"ge_activate_contype_routed",(caddr_t)ge_activate_contype_routed },
	{"ge_activate_contype_stronearr",(caddr_t)ge_activate_contype_stronearr },
	{"ge_activate_contype_stepdiv",(caddr_t)ge_activate_contype_stepdiv },
	{"ge_activate_contype_stepconv",(caddr_t)ge_activate_contype_stepconv },
	{"ge_activate_contype_transdiv",(caddr_t)ge_activate_contype_transdiv },
	{"ge_activate_contype_transconv",(caddr_t)ge_activate_contype_transconv },
	{"ge_activate_condir_center",(caddr_t)ge_activate_condir_center },
	{"ge_activate_condir_left",(caddr_t)ge_activate_condir_left },
	{"ge_activate_condir_right",(caddr_t)ge_activate_condir_right },
	{"ge_activate_condir_up",(caddr_t)ge_activate_condir_up },
	{"ge_activate_condir_down",(caddr_t)ge_activate_condir_down },
	{"ge_activate_background_color",(caddr_t)ge_activate_background_color },
	{"ge_activate_india_ok",(caddr_t)ge_activate_india_ok },
	{"ge_activate_india_cancel",(caddr_t)ge_activate_india_cancel },
	{"ge_create_india_label",(caddr_t)ge_create_india_label },
	{"ge_create_india_text",(caddr_t)ge_create_india_text },
	{"ge_activate_yesnodia_yes",(caddr_t)ge_activate_yesnodia_yes },
	{"ge_activate_yesnodia_no",(caddr_t)ge_activate_yesnodia_no },
	{"ge_activate_yesnodia_cancel",(caddr_t)ge_activate_yesnodia_cancel },
	{"ge_activate_confirm_ok",(caddr_t)ge_activate_confirm_ok },
	{"ge_activate_confirm_cancel",(caddr_t)ge_activate_confirm_cancel }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

#if !defined OS_VMS
  {
    if ( (s = getenv( "pwr_exe")) == NULL)
    {
      printf( "** pwr_exe is not defined\n");
      exit(0);
    }
    sprintf( uid_filename, "%s/ge.uid", s);
  }
#endif

  // Create object context
  gectx = (ge_tCtx) calloc( 1, sizeof(*gectx));
  gectx->parent_ctx = parent_ctx;
  gectx->parent_wid = parent_widget;

#ifdef LDH
  if ( ldhses) {
    // Open a new session
    sts = ldh_OpenSession( &gectx->ldhses, ldh_SessionToVol( ldhses),
         ldh_eAccess_ReadOnly, ldh_eUtility_PlcEditor);
    if ( EVEN(sts)) return NULL;
  }
#endif
  gectx->exit_when_close = exit_when_close;

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Ge");
  if ( graph_name)
  {
    cdh_ToLower( tmp_name, graph_name);
    tmp_name[0] = toupper( tmp_name[0]);
    if ( (s = strrchr( tmp_name, '.')))
      *s = 0;
    strcat( title, " ");
    strcat( title, tmp_name);
  }

  reglist[0].value = (caddr_t) gectx;

  gectx->toplevel = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_widget, args, 0);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(gectx->toplevel), 
						actions, XtNumber(actions));
 
  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) gectx);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "ge_window", gectx->toplevel,
			name, NULL, 0, &ge_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "input_dialog", gectx->toplevel,
		&gectx->india_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "confirm_dialog", gectx->toplevel,
		&gectx->confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, "yesno_dialog", gectx->toplevel,
		&gectx->yesnodia_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch yesno dialog\n");

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( ge_widget, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( gectx->toplevel ,args,i);
      
  XtManageChild( ge_widget);
  XtUnmanageChild( gectx->cmd_input);

  gectx->graph = new Graph( gectx, gectx->graph_form, "",
		&gectx->grow_widget, &sts, "pwrp_pop:");
  gectx->graph->message_cb = &ge_message;
  gectx->graph->get_current_subgraph_cb = &ge_subpalette_get_select;
  gectx->graph->get_current_colors_cb = &ge_colorpalette_get_current;
  gectx->graph->set_current_colors_cb = &ge_colorpalette_set_current;
  gectx->graph->cursor_motion_cb = &ge_status_msg;
  gectx->graph->change_text_cb = &ge_change_text_cb;
  gectx->graph->change_name_cb = &ge_change_name_cb;
  gectx->graph->change_value_cb = &ge_change_value_cb;
  gectx->graph->confirm_cb = &ge_confirm_cb;
  gectx->graph->command_cb = &ge_command_cb;
  gectx->graph->get_plant_select_cb = &ge_get_plant_select_cb;
  gectx->graph->load_graph_cb = &ge_load_graph_cb;
  gectx->graph->set_focus_cb = &ge_set_focus_cb;
  gectx->graph->traverse_focus_cb = &ge_traverse_focus;
  gectx->graph->get_ldhses_cb = &ge_get_ldhses_cb;

//  i = 0;
//  XtSetArg(args[i], XmNtraversalOn, True);i++;
//  XtSetArg(args[i], XmNnavigationType, XmTAB_GROUP);i++;
//  XtSetValues( gectx->grow_widget, args,i);

  gectx->subpalette = new SubPalette( gectx, gectx->palette_pane, "SubGraphs",
		&gectx->subpalette_widget, &sts);
  gectx->subpalette->message_cb = &ge_message;
  gectx->subpalette->set_focus_cb = &ge_set_focus_cb;
  gectx->subpalette->traverse_focus_cb = &ge_traverse_focus;

  gectx->colpal_main_widget = ScrolledColPalCreate( gectx->palette_pane, 
		"Colors", NULL, 
		0, ge_init_colorpalette_cb, gectx,
		&gectx->colorpalette_widget);
  XtManageChild( gectx->colpal_main_widget);

#ifdef LDH
  if ( ldhses)
    gectx->plantctx = plant_new( gectx, gectx->palette_pane, "Plant",
		gectx->ldhses, "NavigatorW1", 
		&gectx->plant_widget, &sts, ge_get_plant_select_cb);
//  XtUnmanageChild( gectx->plant_widget);
#endif

  gectx->graph->create_navigator( gectx->palette_pane);

  XtPopup( gectx->toplevel, XtGrabNone);
  XtRealizeWidget( gectx->toplevel);

//  XmProcessTraversal( gectx->graph->grow_widget, XmTRAVERSE_CURRENT);
//  gectx->graph->set_inputfocus();

  // Set traversal off on sash widget
  {
    Widget *children;
    int num_children;
    XtVaGetValues( gectx->main_pane, 
		 XmNchildren, &children,
		 XmNnumChildren, &num_children, NULL);
    while ( num_children-- > 0)
      if ( XmIsSash( children[num_children]))
        XtVaSetValues( children[num_children],
		     XmNtraversalOn, False, NULL);
  }
  gectx->subpalette->get_path( &path_cnt, &path);
  gectx->graph->set_subgraph_path( path_cnt, path);

  if ( graph_name)
    ge_open_graph( gectx, graph_name);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( gectx->toplevel, 
	(XtCallbackProc)ge_activate_exit, gectx);

  // Get proview defined selection atoms
  wow_GetAtoms( gectx->toplevel, &gectx->graph_atom, 0, 0);

  ge_get_systemname( systemname);
  gectx->graph->set_systemname( systemname);

  return (void *)gectx;
}








