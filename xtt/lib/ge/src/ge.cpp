/* 
 * Proview   $Id: ge.cpp,v 1.29 2008-01-24 09:28:01 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

/* ge.cpp -- Graphical editor window */


#include <vector.h>
#include "glow_std.h"

#if defined OS_VMS || defined OS_LINUX
#define LDH 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "rt_gdh.h"
#include "rt_load.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_xhelp.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_msg.h"


#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_graph.h"
#include "ge.h"
#include "ge_subpalette.h"
#include "ge_subgraphs.h"
#include "ge_util.h"
#include "ge_msg.h"
#include "wb_wnav_selformat.h"
#include "wb_nav.h"


void Ge::set_title()
{
  char          title[80];
  char          name[80];
  char          *s;
  int           page;

  graph->get_name( name);
  if ( (s = strstr( name, "__p")) &&
       (sscanf( s+3, "%d", &page) == 1)) {
    *s = 0;
    sprintf( title, "PwR Ge   %s   Page %d", name, page);
  }
  else {
    if ( graph->trace_started)
      sprintf( title, "PwR Ge   %s   Preview", name);
    else
      sprintf( title, "PwR Ge   %s", name);
  }
  set_title( title);
}

void Ge::prevtable_insert( char *name, char *prev)
{
  if ( prev_count >= 
	int( sizeof(prev_table)/sizeof(prev_table[0])))
    return;

  // Check if alread inserted
  for ( int i = 0; i < prev_count; i++)
  {
    if ( strcmp( name, prev_table[i].name) == 0)
    {
      strcpy( prev_table[i].prev, prev);
      return;
    }
  }

  strcpy( prev_table[prev_count].name, name);   
  strcpy( prev_table[prev_count].prev, prev);   
  prev_count++;
}

int Ge::prevtable_get( char *name, char *prev)
{
  for ( int i = 0; i < prev_count; i++)
  {
    if ( strcmp( name, prev_table[i].name) == 0)
    {
      strcpy( prev, prev_table[i].prev);
      return 1;
    }
  }
  return 0;
}

void Ge::prevtable_clear()
{
  prev_count = 0;
}

void Ge::save_and_close()
{
  char name[40];

  graph->get_name( name);
  if ( strcmp( name, "") == 0) {
    if ( ! graph->is_subgraph())
      open_input_dialog( "Graph name", "Save Graph", "",
			 Ge::save_graph_and_close);
    else
      open_input_dialog( "SubGraph name", "Save SubGraph", "", 
			 Ge::save_graph_and_close);
  }
  else {
    save_graph( this, name);

    delete this;
  }
}

void Ge::ungroup_yes_cb( Ge *gectx)
{
  gectx->graph->ungroup_select(1);
}

void Ge::ungroup_no_cb( Ge *gectx)
{
}

void Ge::recover_dynprop_yes_cb( Ge *gectx)
{
  gectx->graph->set_recall_data( gectx->recover_object, gectx->recover_name);
}

void Ge::recover_dynprop_no_cb( Ge *gectx)
{
}

void Ge::exit_save_cb( Ge *gectx)
{
  gectx->save_and_close();
}

void Ge::exit_nosave_cb( Ge *gectx)
{
  delete gectx;
}

void Ge::load_graph_cb( void *ge_ctx, char *name)
{
  Ge		*gectx = (Ge *)ge_ctx;
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
  gectx->update();
}


void Ge::save_graph( Ge *gectx, char *name)
{
  char filename[80];
  char graphname[80];
  char oldname[80];
  char *s;
  char next[40];
  int sts;

  if ( ! gectx->graph->is_subgraph() ) {
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
    gectx->message( 'I', "Graph saved");

    gectx->set_title();
  }
  else {

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
      gectx->prevtable_insert( next, name);

    gectx->set_title();

    gectx->message( 'I', "SubGraph saved");
  }
}

void Ge::save( char *name)
{
  save_graph( this, name);
}

void Ge::clear_all()
{
  int		path_cnt;
  char		*path;

  graph->set_show_grid( 0);
  graph->set_gridsize( 1);
  graph->set_grid( 0);
  graph->clear_all();
  subpalette->get_path( &path_cnt, &path);
  graph->set_subgraph_path( path_cnt, path);
  update();
  set_title();

}

void Ge::clear()
{
  clear_all();
}

void Ge::save_graph_and_close( Ge *gectx, char *name)
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
    gectx->message( 'I', "Graph saved");
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
    gectx->message( 'I', "SubGraph saved");
  }

  delete gectx;
}

void Ge::export_javabean( Ge *gectx, char *name)
{
  char filename[80];
  char *s;
  int sts;

  if ( gectx->graph->is_subgraph()) {
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
      gectx->message( 'I', "Javabean exported");
    else
      gectx->message( 'E', "Java export error");
  }
  else {
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
    if ( EVEN(sts)) {
      gectx->message( 'E', "Java export error");
      return;
    }

    // Export applet
    strcpy( filename, appletname);
    strcat( filename, ".java");

    sts = gectx->graph->export_javaframe( filename, appletname, 1, 0);
    if ( EVEN(sts)) {
      gectx->message( 'E', "Java export error");
      return;
    }

    // Export html
    strcpy( filename, "$pwrp_web/");
    strcpy( filename, framename);
    strcat( filename, ".html");
    cdh_ToLower( filename, filename);

    sts = gectx->graph->export_javaframe( filename, appletname, 0, 1);
    if ( EVEN(sts)) {
      gectx->message( 'E', "Java export error");
      return;
    }
    gectx->message( 'I', "Java frame and applet exported");
  }
}

void Ge::export_gejava( Ge *gectx, char *name)
{
  char filename[80];
  char *s;
  int sts;
  char cmd[200];

  if ( gectx->graph->is_subgraph()) {
    gectx->message( 'E', "Unable to save subgraph as ge java");
  }
  else {
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
        gectx->message( 'E', "Java export error");
        return;
      }
      else if ( sts == GE__ISBASECLASS)
	return;

      // Compile frame
      gectx->graph->get_systemname( systemname);

      sprintf( cmd, "$pwr_exe/ge_javac.sh java %s %s", filename, systemname);
      sts = system( cmd);
      if ( sts != 0) {
        gectx->message( 'E', "Java compilation errors");
        return;
      }
    }

    if ( gectx->graph->is_javaapplet()) {

      strcpy( filename, appletname);
      strcat( filename, ".java");

      // Export applet
      sts = gectx->graph->export_gejava( filename, appletname, 1, 0);
      if ( EVEN(sts)) {
        gectx->message( 'E', "Java export error");
        return;
      }
      else if ( sts == GE__ISBASECLASS)
	return;

      // Compile applet
      gectx->graph->get_systemname( systemname);

      sprintf( cmd, "$pwr_exe/ge_javac.sh java_web %s %s", filename, systemname);
      sts = system( cmd);
      if ( sts != 0) {
        gectx->message( 'E', "Java compilation errors");
        return;
      }

      // Export html
      strcpy( filename, "$pwrp_web/");
      strcat( filename, framename);
      strcat( filename, ".html");
      cdh_ToLower( filename, filename);

      sts = gectx->graph->export_gejava( filename, framename, 0, 1);
      if ( EVEN(sts)) {
        gectx->message( 'E', "Java export error");
        return;
      }
    }

    if ( gectx->graph->is_javaapplication() && gectx->graph->is_javaapplet())
      gectx->message( 'I', "Java frame and applet exported");
    else if ( gectx->graph->is_javaapplication())
      gectx->message( 'I', "Java frame exported");
    else if ( gectx->graph->is_javaapplet())
      gectx->message( 'I', "Java applet exported");
    else
      gectx->message( 'I', "This graph is not java frame or applet");
  }
}

void Ge::open_graph( char *name)
{
  char filename[80];
  char graphname[80];
  int		path_cnt;
  char		*path;
  char		*s;
 
  graph->clear_all();
  graph->set_show_grid( 0);

  cdh_ToLower( filename, name);
  strcpy( graphname, filename);
  if ( (s = strrchr( graphname, '.')))
    *s = 0;
  graph->set_name( graphname);
  if ( ! strrchr( filename, '.'))
    strcat( filename, ".pwg");
  graph->open( filename);

  subpalette->get_path( &path_cnt, &path);
  graph->set_subgraph_path( path_cnt, path);

  update();
  set_title();
}

void Ge::open( char *name)
{
  open_graph( name);
}

void Ge::rotate( Ge *gectx, char *value_str)
{
  float value;
  int	sts;

  sts = sscanf( value_str, "%f", &value);  
  if ( sts != 1) {
    gectx->message( 'E', "Syntax error");
    return;
  }

  gectx->graph->rotate( double(value));
}

int Ge::subpalette_get_select( void *gectx, char *text, char *filename)
{
  return ((Ge *)gectx)->subpalette->get_select( text, filename);
}

void Ge::colorpalette_get_current( void *gectx, glow_eDrawType *fill_color,
	glow_eDrawType *border_color, glow_eDrawType *text_color)
{
  colpal_GetCurrentColors( ((Ge *)gectx)->colorpalette_ctx, 
	fill_color, border_color, text_color);
}

void Ge::colorpalette_set_current( void *gectx, glow_eDrawType fill_color,
	glow_eDrawType border_color, glow_eDrawType text_color)
{
  colpal_SetCurrentColors( ((Ge *)gectx)->colorpalette_ctx, 
	fill_color, border_color, text_color);
}

void Ge::subgraphs_close_cb( SubGraphs *subgraphs)
{
  ((Ge *) subgraphs->parent_ctx)->subgraphs = NULL;
  delete subgraphs;
}

void Ge::status_msg( void *ge_ctx, double x, double y)
{
  char		pos_str[200];
  Ge       	*gectx = (Ge *) ge_ctx;
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

  gectx->status_msg( pos_str);
}

int Ge::command_cb( void *ge_ctx, char *command)
{
  printf( "** Command not defined in this environment\n  '%s'\n", command);
  return 0;
}

//
//  Callbackfunctions from menu entries
//
void Ge::activate_change_text()
{
  graph->change_select_text();
}

void Ge::activate_change_name()
{
  graph->change_select_name();
}

void Ge::activate_preview_start()
{
  graph->init_trace();
  set_title();
}

void Ge::activate_preview_stop()
{
  graph->close_trace( 1);
  set_title();
}

void Ge::activate_cut()
{
  graph->cut();
}

void Ge::activate_copy()
{
  graph->copy();
}

void Ge::activate_rotate()
{
  open_input_dialog( "Value (degrees)", "Rotate", "",
	Ge::rotate);
}

void Ge::activate_rotate90()
{
  graph->rotate( -90.0);
}

void Ge::activate_flip_vert()
{
  graph->flip( glow_eFlipDirection_Vertical);
}

void Ge::activate_flip_horiz()
{
  graph->flip( glow_eFlipDirection_Horizontal);
}

void Ge::activate_pop()
{
  graph->pop_select();
}

void Ge::activate_push()
{
  graph->push_select();
}

void Ge::activate_edit_polyline()
{
  graph->set_mode( grow_eMode_EditPolyLine, false);
  status_msg( this, 0, 0);
}

void Ge::activate_scale_equal()
{
  graph->set_scale_equal( 1);
  status_msg( this, 0, 0);
}

void Ge::activate_move_horizontal()
{
  graph->set_move_restriction( glow_eMoveRestriction_Horizontal);
  status_msg( this, 0, 0);
}

void Ge::activate_move_vertical()
{
  graph->set_move_restriction( glow_eMoveRestriction_Vertical);
  status_msg( this, 0, 0);
}

void Ge::activate_move_reset()
{
  graph->set_move_restriction( glow_eMoveRestriction_No);
  status_msg( this, 0, 0);
}

void Ge::activate_align_horiz_up()
{
  graph->align_select( glow_eAlignDirection_Down);
}

void Ge::activate_align_horiz_down()
{
  graph->align_select( glow_eAlignDirection_Up);
}

void Ge::activate_align_horiz_center()
{
  graph->align_select( glow_eAlignDirection_CenterHoriz);
}

void Ge::activate_align_vert_left()
{
  graph->align_select( glow_eAlignDirection_Left);
}

void Ge::activate_align_vert_right()
{
  graph->align_select( glow_eAlignDirection_Right);
}

void Ge::activate_align_vert_center()
{
  graph->align_select( glow_eAlignDirection_CenterVert);
}

void Ge::activate_equid_vert_up()
{
  graph->equidistance_select( glow_eAlignDirection_Down);
}

void Ge::activate_equid_vert_down()
{
  graph->equidistance_select( glow_eAlignDirection_Up);
}

void Ge::activate_equid_vert_center()
{
  graph->equidistance_select( glow_eAlignDirection_CenterVert);
}

void Ge::activate_equid_horiz_left()
{
  graph->equidistance_select( glow_eAlignDirection_Left);
}

void Ge::activate_equid_horiz_right()
{
  graph->equidistance_select( glow_eAlignDirection_Right);
}

void Ge::activate_equid_horiz_center()
{
  graph->equidistance_select( glow_eAlignDirection_CenterHoriz);
}

void Ge::activate_select_cons()
{
  graph->select_all_cons();
}

void Ge::activate_select_objects()
{
  graph->select_all_objects();
}

void Ge::activate_select_nextobject( glow_eDirection dir)
{
  graph->select_nextobject( dir);
}

void Ge::activate_group()
{
  int sts;
  grow_tObject object;
  char last_group[80];
  char msg[80];

  sts = graph->group_select( &object, last_group);
  if ( sts == GLOW__NOSELECT)
    message( 'E', "Select objects to form at group");
  else if ( sts == GLOW__GROUPCLASS)
    message( 'E', "Unable to group this kind of object");
  else if ( sts == GE__RECALLDATA_FOUND) {
    recover_object =  object;
    strcpy( recover_name, last_group);
    sprintf( msg, "Do you want to recover dynamic properties for group %s", last_group);
    open_yesnodia( msg, 
        "Recover dynamic properties", 
	Ge::recover_dynprop_yes_cb, Ge::recover_dynprop_no_cb);
  }
}

void Ge::activate_ungroup()
{
  int sts;

  sts = graph->ungroup_select( 0);
  if ( sts == GE__GROUPDYNDATA) {

    open_yesnodia(
        "Group has dynamic properties that will be lost\nDo you want to ungroup", 
        "Ungroup", 
	Ge::ungroup_yes_cb, Ge::ungroup_no_cb);

  }  
}

void Ge::activate_connect()
{
  int sts;
  grow_tObject object;
  char	name[120];

  sts = graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    message( 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    message( 'E', "More than one object is selected");
    return;
  }
  sts = Ge::get_plant_select_cb( (void *)this, name);
  if ( EVEN(sts)) {
    message( 'E', "Select an object in the plant hierarchy");
    return;
  }

  graph->connect( object, name, 0);
}

void Ge::activate_connectsecond()
{
  int sts;
  grow_tObject object;
  char	name[120];

  sts = graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    message( 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    message( 'E', "More than one object is selected");
    return;
  }
  sts = Ge::get_plant_select_cb( (void *)this, name);
  if ( EVEN(sts)) {
    message( 'E', "Select an object in the plant hierarchy");
    return;
  }

  graph->connect( object, name, 1);
}

void Ge::activate_objectattributes()
{
  int sts;
  grow_tObject object;

  sts = graph->get_selected_object( &object);
  if ( sts == GE__NOSELECT)
  {
    message( 'E', "No object is selected");
    return;
  }
  else if ( sts == GE__MANYSELECT)
  {
    message( 'E', "More than one object is selected");
    return;
  }
  graph->edit_attributes( object);
}

void Ge::activate_show_grid( int set)
{
  graph->set_show_grid( set);
}

void Ge::activate_paste()
{
  graph->paste();
}

void Ge::activate_command()
{
}

int Ge::get_plant_select_cb( void *ge_ctx, char *select_name)
{
  return ((Ge *)ge_ctx)->get_plant_select( select_name);
}

void Ge::activate_exit()
{
  if ( graph->is_modified())
  {
    open_yesnodia( "Do you wan't to save changes", "Save", 
	Ge::exit_save_cb, Ge::exit_nosave_cb);
    return;
  }

  delete this;
}

void Ge::activate_print()
{
  pwr_tFileName fname;
  pwr_tCmd cmd;

  dcli_translate_filename( fname, "$pwrp_tmp/graph.ps");
  graph->print( fname);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s 1", fname);
  system(cmd);
}

void Ge::activate_new()
{
  clear_all();
}

void Ge::activate_save()
{
  char name[40];

  if ( graph->trace_started) {
    message( 'I', "Nothing to save");
    return;
  }

  graph->get_name( name);
  if ( strcmp( name, "") == 0)
  {
    if ( ! graph->is_subgraph())
      open_input_dialog( "Graph name", "Save Graph", "",
	Ge::save_graph);
    else
      open_input_dialog( "SubGraph name", "Save SubGraph", "", 
	Ge::save_graph);
  }
  else
    Ge::save_graph( this, name);
}

void Ge::activate_save_as()
{
  char name[40];

  if ( graph->trace_started) {
    message( 'E', "Preview is active");
    return;
  }

  graph->get_name( name);
  if ( ! graph->is_subgraph())
    open_input_dialog( "Graph name", "Save Graph", name, 
	Ge::save_graph);
  else
    open_input_dialog( "SubGraph name", "Save SubGraph", name,
	Ge::save_graph);
}

void Ge::activate_export_javabean()
{
  char name[80];
  char default_name[80];

  if ( ! graph->get_java_name( name)) {
    graph->get_name( name);
    if ( strcmp( name, "") != 0) {
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
    open_input_dialog( "JavaBean name", "Export JavaBean", 
	default_name, Ge::export_javabean);
  }
  else
    Ge::export_javabean( this, name);
}

void Ge::activate_export_javabean_as()
{
  char name[80];
  char default_name[80];

  if ( !(graph->is_javaapplication() || graph->is_javaapplet())) {
    message( 'I', "This graph is not java frame or applet");
    return;
  }

  if ( !graph->get_java_name( name)) {
    graph->get_name( name);
    if ( strcmp( name, "") != 0) {
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
  open_input_dialog( "JavaBean name", "Export JavaBean", 
	default_name, Ge::export_javabean);
}

void Ge::activate_export_gejava()
{
  char name[80];
  char default_name[80];

  if ( !(graph->is_javaapplication() || graph->is_javaapplet())) {
    message( 'I', "This graph is not java frame or applet");
    return;
  }

  if ( !graph->get_java_name( name)) {
    graph->get_name( name);
    if ( strcmp( name, "") != 0) {
      if ( strncmp( name, "pwr_c_", 6) == 0) {
        strcpy( default_name, "Jopc");
        strcat( default_name, &name[6]);
        default_name[4] = _toupper( default_name[4]);
      }
      else if ( strncmp( name, "pwr_", 4) == 0) {
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
    open_input_dialog( "Java name", "Export GeJava",
	default_name, Ge::export_gejava);
  }
  else
    Ge::export_gejava( this, name);
}

void Ge::activate_export_gejava_as()
{
  char name[80];
  char default_name[80];

  if ( !graph->get_java_name( name)) {
    graph->get_name( name);
    if ( strcmp( name, "") != 0) {
      if ( strncmp( name, "pwr_c_", 6) == 0) {
        strcpy( default_name, "Jopc");
        strcat( default_name, &name[6]);
        default_name[4] = _toupper( default_name[4]);
      }
      else if ( strncmp( name, "pwr_", 4) == 0) {
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
  open_input_dialog( "Java name", "Export GeJava", 
	default_name, Ge::export_gejava);
}

void Ge::activate_export_java()
{
  if ( graph->is_subgraph())
    activate_export_javabean();
  else
    activate_export_gejava();
}

void Ge::activate_export_java_as()
{
  if ( graph->is_subgraph())
    activate_export_javabean_as();
  else
    activate_export_gejava_as();
}

void Ge::activate_export_plcfo()
{
  char name[80];

  graph->get_name( name);

  if ( strcmp( name, "") != 0)
    export_plcfo( this, name);
  else
    open_input_dialog( "PlcFo name", "Export PlcFo",
		       "", Ge::export_plcfo);
}

void Ge::activate_export_plcfo_as()
{
  char name[80];

  graph->get_name( name);

  open_input_dialog( "PlcFo name", "Export PlcFo", 
	name, Ge::export_plcfo);
}

void Ge::export_plcfo( Ge *gectx, char *filename)
{
  int sts;
  pwr_tFileName fname;

  sprintf( fname, "$pwrp_exe/%s.flwn", filename);
  dcli_translate_filename( fname, fname);

  sts = gectx->graph->export_plcfo( fname);
  if ( EVEN(sts))
    gectx->message( sts);
}

void Ge::activate_generate_web()
{
  int sts;

  // graph->generate_web();
  if ( !ldhses) {
    message( 'E', "Ge is not connected to workbench");
    return;
  }

  sts = Graph::generate_web( ldhses);
  if ( sts == GE__NONODEOBJECT)
    message( 'E', "Can't find $Node object");
  else if ( sts == GE__NOWEBHANDLER)
    message( 'E', "Can't find WebHandler object");
  else if ( sts == GE__WEBHANDLERS)
    message( 'E', "More than one WebHandler object found");
  else if ( EVEN(sts))
    message( 'E', "Error from generate web");
  else
    message( 'I', "Web-pages generated");
  Graph::generate_web_help();
}

void Ge::activate_creanextpage()
{
  char old_name[40];
  char name[40];
  char next_name[40];
  int page;
  char *s;

  if ( ! graph->is_subgraph()) {
    message( 'E', "This is not a subgraph");
    return;
  }

  if ( graph->is_modified()) {
    message( 'E', "Subgraph is not saved");
    return;
  }
    
  // Get next subgraph name
  graph->get_name( old_name);
  strcpy( name, old_name);
  if ( (s = strstr( name, "__p")) && 
       (sscanf( s+3, "%d", &page) == 1)) 
    sprintf( s+3, "%d", page+1);
  else
    strcat( name, "__p2");

  // Store next name in graph
  graph->get_next_subgraph( next_name);
  if ( strcmp( next_name, name) == 0) {
    // Next subgraph is already created, check file...
    message( 'E', "Subgraph is already created");
    return;
  }

  graph->set_next_subgraph( name);
  save_graph( this, old_name);
  prevtable_insert( name, old_name);

  graph->set_name( name);
  graph->set_next_subgraph( "");
  graph->set_java_name( "");
  set_title();

  message( 'I', "New page created");
}

void Ge::activate_nextpage()
{
  char next[40];
  char name[40];

  if ( ! graph->is_subgraph())
  {
    message( 'E', "Only subgraphs can have a next page");
    return;
  }

  if ( graph->is_modified())
  {
    message( 'E', "Subgraph is not saved");
    return;
  }
    
  graph->get_next_subgraph( next);
  if ( strcmp( next, "") == 0)
  {
    message( 'E', "No next page is found");
    return;
  }

  // Store this to enable previous page later
  graph->get_name( name);
  prevtable_insert( next, name);

  graph->set_nodraw();
  graph->store_geometry();
  open_graph( next);
  graph->restore_geometry();
  graph->reset_nodraw();
  graph->redraw();
}

void Ge::activate_prevpage()
{
  char name[40];
  char prev[40];
  int sts;

  if ( graph->is_modified())
  {
    message( 'E', "Subgraph is not saved");
    return;
  }
    
  graph->get_name( name);
  sts = prevtable_get( name, prev);
  if ( EVEN(sts))
  {
    message( 'E', "No previous page is found");
    return;
  }

  graph->set_nodraw();
  graph->store_geometry();
  open_graph( prev);
  graph->restore_geometry();
  graph->reset_nodraw();
  graph->redraw();
}

void Ge::activate_graph_attr()
{
  if ( ! graph->is_subgraph())
    graph->edit_graph_attributes();
  else
    graph->edit_subgraph_attributes();
}

void Ge::open_list_cb( void *ctx, char *text)
{
  ((Ge *)ctx)->open_graph( text);
}

int Ge::sort_files( const void *file1, const void *file2)
{
  return ( strcmp( (char *)file1, (char *)file2));
}

void Ge::activate_open()
{
  prevtable_clear();
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
    if ( strcmp( file, "") == 0)
      file_cnt--;
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);

  if ( !file_cnt) {
    return;
  }

  strcpy( file_p[file_cnt], "");
    
  qsort( file_p, file_cnt, sizeof(*file_p), Ge::sort_files);

  create_list( "Open Graph", 
	       (char *)file_p, Ge::open_list_cb, (void *)this);

  free( file_p);

  //  open_input_dialog( "Graph name", "Open Graph", "", 
  //	Ge::open_graph);
}

void Ge::activate_subgraphs()
{
  if ( subgraphs) {
    message( 'I', "Loaded SubGraphs already active");
    return;
  }
  subgraphs_new();
}

void Ge::activate_rect( bool keep)
{
  graph->set_mode( grow_eMode_Rect, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_rectrounded( bool keep)
{
  graph->set_mode( grow_eMode_RectRounded, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_line( bool keep)
{
  graph->set_mode( grow_eMode_Line, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_polyline( bool keep)
{
  graph->set_mode( grow_eMode_PolyLine, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_circle( bool keep)
{
  graph->set_mode( grow_eMode_Circle, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_text( bool keep)
{
  graph->set_mode( grow_eMode_Text, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_annot( bool keep)
{
  graph->set_mode( grow_eMode_Annot, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_conpoint( bool keep)
{
  graph->set_mode( grow_eMode_ConPoint, keep);
  status_msg( this, 0, 0);
}

void Ge::activate_fill( int set)
{
  graph->set_fill( set);
  graph->set_select_fill( set);
}

void Ge::activate_border( int set)
{
  graph->set_border( set);
  graph->set_select_border( set);
}

void Ge::activate_shadow( int set)
{
  graph->set_shadow( set);
  graph->set_select_shadow( set);
}

void Ge::activate_incr_lightness()
{
  graph->incr_select_color_lightness(1);
}

void Ge::activate_decr_lightness()
{
  graph->incr_select_color_lightness( -1);
}

void Ge::activate_incr_intensity()
{
  graph->incr_select_color_intensity(1);
}

void Ge::activate_decr_intensity()
{
  graph->incr_select_color_intensity( -1);
}

void Ge::activate_incr_shift(){

  graph->incr_select_color_shift(1);
}

void Ge::activate_decr_shift()
{
  graph->incr_select_color_shift( -1);
}

void Ge::activate_scale()
{
  graph->set_mode( grow_eMode_Scale, false);
  status_msg( this, 0, 0);
}

void Ge::activate_scale( double factor)
{
  graph->scale( factor, factor);
}

void Ge::activate_grid( int set)
{
  graph->set_grid( set);
}

void Ge::activate_linewidth( int width)
{
  graph->set_linewidth( width);
  graph->set_select_linewidth( width);
}

void Ge::activate_linetype1()
{
  graph->set_linetype( glow_eLineType_Solid);
  graph->set_select_linetype( glow_eLineType_Solid);
}

void Ge::activate_linetype2()
{
  graph->set_linetype( glow_eLineType_Dashed1);
  graph->set_select_linetype( glow_eLineType_Dashed1);
}

void Ge::activate_linetype3()
{
  graph->set_linetype( glow_eLineType_Dashed2);
  graph->set_select_linetype( glow_eLineType_Dashed2);
}

void Ge::activate_linetype4()
{
  graph->set_linetype( glow_eLineType_Dashed3);
  graph->set_select_linetype( glow_eLineType_Dashed3);
}

void Ge::activate_linetype5()
{
  graph->set_linetype( glow_eLineType_Dotted);
  graph->set_select_linetype( glow_eLineType_Dotted);
}

void Ge::activate_linetype6()
{
  graph->set_linetype( glow_eLineType_DotDashed1);
  graph->set_select_linetype( glow_eLineType_DotDashed1);
}

void Ge::activate_linetype7()
{
  graph->set_linetype( glow_eLineType_DotDashed2);
  graph->set_select_linetype( glow_eLineType_DotDashed2);
}

void Ge::activate_gridsize( double size)
{
  graph->set_gridsize( size);
}

void Ge::activate_textsize( int size)
{
  graph->set_textsize( size);
  graph->set_select_textsize( size);
}

void Ge::activate_textfont( glow_eFont font)
{
  graph->set_textfont( font);
  graph->set_select_textfont( font);
}

void Ge::activate_textbold( int set)
{
  if ( set) {
    graph->set_textbold( 1);
    graph->set_select_textbold( 1);
  }
  else {
    graph->set_textbold( 0);
    graph->set_select_textbold( 0);
  }
}

void Ge::activate_zoom_in()
{
  graph->zoom( 1.2);
}

void Ge::activate_zoom_out()
{
  graph->zoom( 5.0/6);
}

void Ge::activate_zoom_reset()
{
  graph->unzoom();
}

void Ge::activate_concorner_right()
{
  graph->set_concorner( glow_eCorner_Right);
}

void Ge::activate_concorner_rounded()
{
  graph->set_concorner( glow_eCorner_Rounded);
}

void Ge::activate_round_amount( double amount)
{
  graph->set_corner_round_amount( amount);
}

void Ge::activate_contype_straight()
{
  graph->set_contype( glow_eConType_Straight);
}

void Ge::activate_contype_routed()
{
  graph->set_contype( glow_eConType_Routed);
}

void Ge::activate_contype_stronearr()
{
  graph->set_contype( glow_eConType_StraightOneArrow);
}

void Ge::activate_contype_stepdiv()
{
  graph->set_contype( glow_eConType_StepDiv);
}

void Ge::activate_contype_stepconv()
{
  graph->set_contype( glow_eConType_StepConv);
}

void Ge::activate_contype_transdiv()
{
  graph->set_contype( glow_eConType_TransDiv);
}

void Ge::activate_contype_transconv()
{
  graph->set_contype( glow_eConType_TransConv);
}

void Ge::activate_condir_center()
{
  graph->set_condir( glow_eDirection_Center);
}

void Ge::activate_condir_left()
{
  graph->set_condir( glow_eDirection_Left);
}

void Ge::activate_condir_right()
{
  graph->set_condir( glow_eDirection_Right);
}

void Ge::activate_condir_up()
{
  graph->set_condir( glow_eDirection_Up);
}

void Ge::activate_condir_down()
{
  graph->set_condir( glow_eDirection_Down);
}

void Ge::activate_background_color()
{
  graph->set_background_color();
}

void Ge::activate_help()
{
  CoXHelp::dhelp( "index", "", navh_eHelpFile_Other, "$pwr_lang/man_geref.dat", false);  
}

void Ge::activate_help_subgraph()
{
  CoXHelp::dhelp( "index", "", navh_eHelpFile_Other, "$pwr_exe/man_subgraph.dat", false);  
}

void Ge::activate_india_ok( char *value)
{
  (india_ok_cb)( this, value);
}
void Ge::activate_india_cancel()
{
}
void Ge::activate_yesnodia_yes()
{
  yesnodia_open = 0;

  (yesnodia_yes_cb)( this);
}
void Ge::activate_yesnodia_no()
{
  yesnodia_open = 0;

  (yesnodia_no_cb)( this);
}
void Ge::activate_yesnodia_cancel()
{
  yesnodia_open = 0;
}
void Ge::activate_confirm_ok()
{
  confirm_open = 0;
  graph->confirm_ok( current_confirm_object);
}
void Ge::activate_confirm_cancel()
{
  confirm_open = 0;
}

//
// Callbacks from colorpalette
//
int Ge::colorpalette_cb( GlowCtx *ctx, glow_tEvent event)
{
  Ge 	*gectx;

  colpal_GetCtxUserData( (ColPalCtx *)ctx, (void **) &gectx);

  switch ( event->event) {
  case glow_eEvent_MB1Click:
    if ( event->any.type == glow_eEventType_ColorTone) {
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

int Ge::init_colorpalette_cb( GlowCtx *fctx, void *client_data)
{
  Ge *gectx = (Ge *) client_data;
  gectx->colorpalette_ctx = (colpal_tCtx) fctx;
  colpal_SetCtxUserData( gectx->colorpalette_ctx, gectx);

  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB1Click, 
	glow_eEventType_CallBack, Ge::colorpalette_cb);
  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB1ClickShift, 
	glow_eEventType_CallBack, Ge::colorpalette_cb);
  colpal_EnableEvent( gectx->colorpalette_ctx, glow_eEvent_MB2Click, 
	glow_eEventType_CallBack, Ge::colorpalette_cb);

  return 1;
}

int Ge::get_ldhses_cb( void *ctx, ldh_tSesContext *ldhses, int load)
{
#if LDH
  Ge *gectx = (Ge *) ctx;
  ldh_tWBContext wbctx; 
  int	sts;
  pwr_tVolumeId	volid = 0;
  pwr_tClassId	classid;
  ldh_tVolContext 	volctx;

  if ( gectx->ldhses) {
    *ldhses = gectx->ldhses;
    return 1;
  }
  else if ( !load)
    return 0;
  else {
    // Open workbench and attatch some volume
    sts = ldh_OpenWB( &wbctx, 0, 0);
    if ( EVEN(sts)) return sts;

    // Attach first rootvolume, or if no rootvolume exist some other volume
    sts = ldh_GetVolumeList( wbctx, &volid);
    while ( ODD(sts)) {
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

int Ge::traverse_focus( void *ctx, void *component)
{
  Ge *gectx = (Ge *) ctx;

  if ( component == (void *) gectx->graph) {
    if ( gectx->subpalette_mapped)
      gectx->set_focus( gectx->subpalette);
    else if ( gectx->plant_mapped)
      gectx->set_focus( gectx->plantctx);
  }
  else if ( component == (void *) gectx->subpalette) {
    gectx->set_focus( gectx->graph);
  }
  else if ( component == (void *) gectx->plantctx) {
    gectx->set_focus( gectx->graph);
  }
  else
    return 0;
  return 1;
}

int Ge::set_focus_cb( void *ctx, void *component)
{
  return ((Ge *)ctx)->set_focus( component);
}

int Ge::set_focus( void *component)
{
  if ( component == 0) {
    if ( subpalette_mapped)
      set_focus( subpalette);
    else if ( plant_mapped)
      set_focus( plantctx);
  }
  else if ( component == (void *)graph) {
    graph->set_inputfocus( 1);
    if ( subpalette_mapped)
      subpalette->set_inputfocus( 0);
    if ( plant_mapped)
      plantctx->set_inputfocus(0);
    focused_component = component;
  }
  else if ( component == (void *)subpalette) {
    if ( subpalette_mapped) {
      graph->set_inputfocus( 0);
      subpalette->set_inputfocus( 1);
      focused_component = component;
      if ( plant_mapped)
	plantctx->set_inputfocus(0);
    }
  }
  else if ( component == (void *)plantctx) {
    if ( plant_mapped) {
      if ( subpalette_mapped)
	subpalette->set_inputfocus(0);
      graph->set_inputfocus( 0);
      plantctx->set_inputfocus( 1);
      focused_component = component;
    }
  }

  return 1;
}

int Ge::generate_web( ldh_tSesContext x_ldhses)
{
  return Graph::generate_web( x_ldhses);
}

int Ge::command( char *cmd)
{
  return graph->command( cmd);
}

void Ge::message( pwr_tStatus sts)
{
  graph->message( sts);
}

void Ge::message_cb( void *ctx, char severity, char *message)
{
  ((Ge *)ctx)->message( severity, message);
}

void Ge::help_cb( void *ctx, char *topic, char *helpfile)
{
  CoXHelp::dhelp( topic, "", navh_eHelpFile_Other, helpfile, false);  
}

Ge::~Ge()
{
#ifdef LDH
  if ( ldhses)
    ldh_CloseSession( ldhses);
#endif
}

Ge::Ge( 	void 	*x_parent_ctx, 
		ldh_tSesContext	x_ldhses,
		int	x_exit_when_close) :
  parent_ctx(x_parent_ctx), graph(0), subpalette(0),
  subgraphs(0), colorpalette_ctx(0), text_input_open(0), name_input_open(0),
  value_input_open(0), command_open(0), confirm_open(0), yesnodia_open(0), 
  yesnodia_yes_cb(0), yesnodia_no_cb(0), india_ok_cb(0), current_text_object(0),
  current_value_object(0), current_confirm_object(0), ldhses(0), plantctx(0),
  exit_when_close(x_exit_when_close), prev_count(0), focused_component(0),
  recover_object(0), plant_mapped(0), subpalette_mapped(0)
{
  strcpy( name, "PwR Ge");

#ifdef LDH
  if ( x_ldhses) {
    // Open a new session
    pwr_tStatus sts = ldh_OpenSession( &ldhses, ldh_SessionToVol( x_ldhses),
         ldh_eAccess_ReadOnly, ldh_eUtility_PlcEditor);
    if ( EVEN(sts)) return;
  }
#endif
}








