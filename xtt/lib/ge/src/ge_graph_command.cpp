/* ge_graph_command.cpp -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines for handling of command line in ge. */


#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

extern "C" {
#include "pwr_class.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_dcli.h"
#include "co_ccm_msg.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>
#include <Xm/SelectioB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_graph.h"
#include "co_dcli_msg.h"
#include "ge_msg.h"
#include "ge.h"

extern "C" {
#include "flow_x.h"
#if LDH
# include "wb_wccm.h"
#endif
}

static char ge_version[] = "V3.4.0";
static Graph *current_graph;

static void graph_group_replace_attr( grow_tObject group, char *from_str, char *to_str, 
				      int *replace_cnt, int strict);

static int	graph_show_func(void		*client_data,
				void		*client_flag);
static int	graph_save_func(void		*client_data,
				void		*client_flag);
static int	graph_open_func(void		*client_data,
				void		*client_flag);
static int	graph_new_func(	void		*client_data,
				void		*client_flag);
static int	graph_set_func(	void		*client_data,
				void		*client_flag);
static int	graph_add_func(	void		*client_data,
				void		*client_flag);
static int	graph_create_func( void		*client_data,
				void		*client_flag);
static int	graph_replace_func( void		*client_data,
				void		*client_flag);
static int	graph_rotate_func( void		*client_data,
				void		*client_flag);
static int	graph_scale_func( void		*client_data,
				void		*client_flag);
static int	graph_move_func( void		*client_data,
				void		*client_flag);
static int	graph_exit_func( void		*client_data,
				void		*client_flag);
static int	graph_quit_func( void		*client_data,
				void		*client_flag);
static int	graph_group_func( void		*client_data,
				void		*client_flag);
static int	graph_select_func( void		*client_data,
				void		*client_flag);
static int	graph_export_func( void		*client_data,
				void		*client_flag);
static int	graph_replace_func( void	*client_data,
				void		*client_flag);

dcli_tCmdTable	graph_command_table[] = {
		{
			"SHOW",
			&graph_show_func,
			{ "dcli_arg1", "dcli_arg2",
			""}
		},
		{
			"SAVE",
			&graph_save_func,
			{ "dcli_arg1", "/FILE",
			""}
		},
		{
			"OPEN",
			&graph_open_func,
			{ "dcli_arg1", "/FILE",
			""}
		},
		{
			"NEW",
			&graph_new_func,
			{""}
		},
		{
			"SET",
			&graph_set_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4",
			"/ON", "/OFF",
			""}
		},
		{
			"ADD",
			&graph_add_func,
			{ "dcli_arg1", "dcli_arg2", "/X1", "/Y1",
			""}
		},
		{
			"ROTATE",
			&graph_rotate_func,
			{ "dcli_arg1", "/ANGEL",
			""}
		},
		{
			"SCALE",
			&graph_scale_func,
			{ "dcli_arg1", "/SCALEX", "/SCALEY", "/X", "/Y",
			""}
		},
		{
			"MOVE",
			&graph_move_func,
			{ "dcli_arg1", "/X", "/Y",
			""}
		},
		{
			"CREATE",
			&graph_create_func,
			{ "dcli_arg1", "/X1", "/Y1", "/X2", "/Y2", 
			"/WIDTH", "/HEIGHT", "/ANGEL1", "/ANGEL2",
			"/TEXT", "/SUBGRAPH",
			""}
		},
		{
			"REPLACE",
			&graph_replace_func,
			{ "dcli_arg1", "/FROM", "/TO", "/STRICT",
			""}
		},
		{
			"EXIT",
			&graph_exit_func,
			{""}
		},
		{
			"QUIT",
			&graph_quit_func,
			{""}
		},
		{
			"GROUP",
			&graph_group_func,
			{""}
		},
		{
			"SELECT",
			&graph_select_func,
			{"dcli_arg1", ""}
		},
		{
			"EXPORT",
			&graph_export_func,
			{"dcli_arg1", ""}
		},
		{"",}};

static void graph_store_graph( Graph *graph)
{
  current_graph = graph;  
}

static void graph_get_stored_graph( Graph **graph)
{
  *graph = current_graph;
}

static int	graph_show_func(void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "VERSION", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW VERSION"
    char	message_str[80];
	 
    strcpy( message_str, "Ge version ");
    strcat( message_str, ge_version);
    graph->message('I', message_str);
  }
  else
  {
    graph->message('E', "Unknown qualifier");
    return GE__QUAL;
  }
  return GE__SUCCESS;
}

static int	graph_save_func(void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	file_str[80];
  char	name[40];

  if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
  {
    if ( strlen( file_str) > 39)
    {
      graph->message('E', "Filename to long");
      return GE__FILENAME;
    }
    strcpy( name, file_str);    
  }
  else if ( ODD( dcli_get_qualifier( "dcli_arg1", file_str)))
  {
    if ( strlen( file_str) > 39)
    {
      graph->message('E', "Filename to long");
      return GE__FILENAME;
    }
    strcpy( name, file_str);
  }
  else
    graph->get_name( name);

  ge_save( graph->parent_ctx, name);
  return GE__SUCCESS;
}

static int	graph_quit_func(void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  ge_del( graph->parent_ctx);
  return GE__SUCCESS;
}

static int	graph_exit_func(void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;
  char	name[40];

  graph->get_name( name);
  if ( strcmp( name, "") == 0)
  {
    graph->message('E', "Graph has no name, 'save as' or 'quit'");
    return GE__NONAME;
  }

  ge_save( graph->parent_ctx, name);
  ge_del( graph->parent_ctx);
  return GE__SUCCESS;
}

static int	graph_open_func(void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char file_str[80];
  char name[80];

  if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
  {
    if ( strlen( file_str) > 39)
    {
      graph->message('E', "Filename to long");
      return GE__FILENAME;
    }
    strcpy( name, file_str);    
  }
  else if ( ODD( dcli_get_qualifier( "dcli_arg1", file_str)))
  {
    if ( strlen( file_str) > 39)
    {
      graph->message('E', "Filename to long");
      return GE__FILENAME;
    }
    strcpy( name, file_str);
  }
  else
  {
    graph->message('E', "Enter filename");
    return GE__SYNTAX;
  }

  ge_open( graph->parent_ctx, name);
  return GE__SUCCESS;
}

static int	graph_new_func(	void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  ge_clear( graph->parent_ctx);
  return GE__SUCCESS;
}

static int	graph_set_func(	void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "VERIFY", strlen( arg1_str)) == 0)
  {
    graph->verify = 1;
  }
  else if ( strncmp( arg1_str, "NOVERIFY", strlen( arg1_str)) == 0)
  {
    graph->verify = 0;
  }
  else if ( strncmp( arg1_str, "FILL", strlen( arg1_str)) == 0)
  {
    graph->set_fill( 1);
  }
  else if ( strncmp( arg1_str, "NOFILL", strlen( arg1_str)) == 0)
  {
    graph->set_fill( 0);
  }
  else if ( strncmp( arg1_str, "BORDER", strlen( arg1_str)) == 0)
  {
    graph->set_border( 1);
  }
  else if ( strncmp( arg1_str, "NOBORDER", strlen( arg1_str)) == 0)
  {
    graph->set_border( 0);
  }
  else if ( strncmp( arg1_str, "GRID", strlen( arg1_str)) == 0)
  {
    graph->set_grid( 1);
  }
  else if ( strncmp( arg1_str, "NOGRID", strlen( arg1_str)) == 0)
  {
    graph->set_grid( 0);
  }
  else if ( strncmp( arg1_str, "LINEWIDTH", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int		line_width;
    int		sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( arg2_str, "%d", &line_width);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    if ( line_width < 1 || line_width > 8)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    graph->set_linewidth( line_width);
  }
  else if ( strncmp( arg1_str, "GRIDSIZE", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    float	grid_size;
    int		sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( arg2_str, "%f", &grid_size);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    graph->set_gridsize( double(grid_size));
  }
  else if ( strncmp( arg1_str, "TEXTSIZE", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int		text_size, size;
    int		sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( arg2_str, "%d", &text_size);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    switch( text_size)
    {
      case 8: size = 0; break;
      case 10: size = 1; break;
      case 12: size = 2; break;
      case 14: size = 3; break;
      case 18: size = 4; break;
      case 24: size = 5; break;
      default:
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
    }
    graph->set_textsize( size);
  }
  else if ( strncmp( arg1_str, "BOLD", strlen( arg1_str)) == 0)
  {
    graph->set_textbold( 1);
  }
  else if ( strncmp( arg1_str, "NOBOLD", strlen( arg1_str)) == 0)
  {
    graph->set_textbold( 0);
  }
  else if ( strncmp( arg1_str, "BACKGROUNDCOLOR", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int value;
    int sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->set_background_color();
    }
    else
    {
      sts = sscanf( arg2_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( value < 0 || value > 99)
      {
          graph->message('E', "Syntax error");
          return GE__SYNTAX;
      }
      grow_SetBackgroundColor( graph->grow->ctx, (glow_eDrawType) value);
    }
  }
  else if ( strncmp( arg1_str, "FILLCOLOR", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    glow_eDrawType fill_color, border_color;
    int value;
    int sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    sts = sscanf( arg2_str, "%d", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    if ( value < 0 || value > 99)
    {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
    }
    (graph->get_current_colors_cb)( graph->parent_ctx, &fill_color, &border_color);
    fill_color = (glow_eDrawType) value;
    (graph->set_current_colors_cb)( graph->parent_ctx, fill_color, border_color);
  }
  else if ( strncmp( arg1_str, "BORDERCOLOR", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    glow_eDrawType fill_color, border_color;
    int value;
    int sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    sts = sscanf( arg2_str, "%d", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    if ( value < 0 || value > 99)
    {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
    }
    (graph->get_current_colors_cb)( graph->parent_ctx, &fill_color, &border_color);
    border_color = (glow_eDrawType) value;
    (graph->set_current_colors_cb)( graph->parent_ctx, fill_color, border_color);
  }
  else if ( strncmp( arg1_str, "CURRENTOBJECT", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int		arg2_sts;
	
    arg2_sts = dcli_get_qualifier( "dcli_arg2", arg2_str);

    if ( strncmp( arg2_str, "FILLCOLOR", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( value < 0 || value > 99)
      {
          graph->message('E', "Syntax error");
          return GE__SYNTAX;
      }
      grow_SelectClear( graph->grow->ctx);
      grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);

      grow_SetSelectOrigFillColor( graph->grow->ctx, (glow_eDrawType) value);

      grow_SelectRemove( graph->grow->ctx, graph->current_cmd_object);
    }
    else if ( strncmp( arg2_str, "COLORTONE", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( value < 0 || value > 99)
      {
          graph->message('E', "Syntax error");
          return GE__SYNTAX;
      }
      grow_SelectClear( graph->grow->ctx);
      grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);

      grow_SetSelectOrigColorTone( graph->grow->ctx, (glow_eDrawTone) value);

      grow_SelectRemove( graph->grow->ctx, graph->current_cmd_object);
    }
    else if ( strncmp( arg2_str, "COLORLIGHTNESS", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      grow_SelectClear( graph->grow->ctx);
      grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);

      grow_SetSelectOrigColLightness( graph->grow->ctx, value);

      grow_SelectRemove( graph->grow->ctx, graph->current_cmd_object);
    }
    else if ( strncmp( arg2_str, "COLORINTENSITY", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      grow_SelectClear( graph->grow->ctx);
      grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);

      grow_SetSelectOrigColIntensity( graph->grow->ctx, value);

      grow_SelectRemove( graph->grow->ctx, graph->current_cmd_object);
    }
    else if ( strncmp( arg2_str, "COLORSHIFT", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      grow_SelectClear( graph->grow->ctx);
      grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);

      grow_SetSelectOrigColorShift( graph->grow->ctx, value);

      grow_SelectRemove( graph->grow->ctx, graph->current_cmd_object);
    }
    else if ( strncmp( arg2_str, "TRACETYPE", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;
      glow_sTraceData	*trace_data;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      grow_GetTraceAttr( graph->current_cmd_object, &trace_data);
      trace_data->attr_type = (glow_eTraceType) value;
      grow_SetTraceAttr( graph->current_cmd_object, trace_data);
    }
    else if ( strncmp( arg2_str, "TRACECOLOR", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;
      glow_sTraceData	*trace_data;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( value < 0 || value > 99)
      {
          graph->message('E', "Syntax error");
          return GE__SYNTAX;
      }

      grow_GetTraceAttr( graph->current_cmd_object, &trace_data);
      trace_data->color = (glow_eDrawType) value;
      grow_SetTraceAttr( graph->current_cmd_object, trace_data);
    }
    else if ( strncmp( arg2_str, "TRACECOLOR2", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      int	value;
      glow_sTraceData	*trace_data;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%d", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( value < 0 || value > 99)
      {
          graph->message('E', "Syntax error");
          return GE__SYNTAX;
      }

      grow_GetTraceAttr( graph->current_cmd_object, &trace_data);
      trace_data->color2 = (glow_eDrawType) value;
      grow_SetTraceAttr( graph->current_cmd_object, trace_data);
    }
    else if ( strncmp( arg2_str, "ACCESS", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      int	sts;
      unsigned int value;
      glow_sTraceData	*trace_data;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      sts = sscanf( arg3_str, "%u", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }

      grow_GetTraceAttr( graph->current_cmd_object, &trace_data);
      trace_data->access = (glow_mAccess) value;
      grow_SetTraceAttr( graph->current_cmd_object, trace_data);
    }
    else if ( strncmp( arg2_str, "ATTRIBUTES", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];
      char	arg4_str[80];
      attr_sItem 	*itemlist;
      attr_sItem        *item_p;
      int 		item_cnt;
      void		*client_data;
      char		attr_name[80];
      int		i_value;
      float		f_value;
      double		d_value;
      int               i;
      int		sts;
      int		found;

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg4", arg4_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      graph->get_attr_items( graph->current_cmd_object, &itemlist, &item_cnt, 
		      &client_data);

      found = 0;
      item_p = itemlist;
      for ( i = 0; i < item_cnt; i++)
      {
        cdh_ToUpper( attr_name, item_p->name);
        if ( strcmp( arg3_str, attr_name) == 0)
        {
          found = 1;
          break;
        }
        item_p++;
      }

      if ( !found)
      {
        graph->message('E', "No such attribute");
        return GE__NOATTR;
      }
      switch( item_p->type)
      {
        case glow_eType_Int:
        case glow_eType_TraceColor:
        case glow_eType_TraceType:
        case glow_eType_Access:
        case glow_eType_Boolean:
        case glow_eType_Direction:
        case glow_eType_Color:
        case glow_eType_ToneOrColor:
          sts = sscanf( arg4_str, "%d", &i_value);
          if ( sts != 1)
          {
            graph->message('E', "Syntax error");
            return GE__SYNTAX;
          }
          memcpy( item_p->value, (char *)&i_value, sizeof(i_value));
          break;
        case glow_eType_Double:
          sts = sscanf( arg4_str, "%f", &f_value);
          if ( sts != 1)
          {
            graph->message('E', "Syntax error");
            return GE__SYNTAX;
          } 
          d_value = double(f_value);
          memcpy( item_p->value, (char *)&d_value, sizeof(d_value));
          break;
        case glow_eType_String:
          strncpy( (char *) item_p->value, arg4_str, item_p->size);
          break;
        default:
          ;
      }
      grow_UpdateObject( graph->grow->ctx, graph->current_cmd_object,
	(grow_sAttrInfo *)client_data);

    }
    else if ( strncmp( arg2_str, "ATTR1", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      graph->connect( graph->current_cmd_object, arg3_str, 0);
    }
    else if ( strncmp( arg2_str, "ATTR2", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      graph->connect( graph->current_cmd_object, arg3_str, 1);
    }
    else if ( strncmp( arg2_str, "ANNOTATION", strlen( arg2_str)) == 0)
    {
      char	arg3_str[80];

      if ( !graph->current_cmd_object)
      {
        graph->message('E', "No current object");
        return GE__NOCURRENT;
      }
      if ( grow_GetObjectType( graph->current_cmd_object) !=
		glow_eObjectType_GrowNode)
      {
        graph->message('E', "Current object type doesn't support this operation"); 
        return GE__CURRENT_TYPE;
      }
      if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      grow_SetAnnotation( graph->current_cmd_object, 1, arg3_str, 
		strlen(arg3_str));
    }
    else
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
  }
  else if ( strncmp( arg1_str, "GRAPHATTRIBUTES", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    char	arg3_str[80];
    int		i, j;
    grow_sAttrInfo	*grow_info = 0;
    grow_sAttrInfo	*grow_info_sub;
    grow_sAttrInfo	*grow_info_p;
    int			grow_info_cnt;
    char		attr_name[80];
    int			i_value;
    float		f_value;
    double		d_value;
    int			sts;
    int			found;
	
    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
      
    if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    found = 0;
    for ( j = 0; j < 2; j++)
    {
      if ( j == 0)
      {
        grow_GetSubGraphAttrInfo( graph->grow->ctx, &grow_info_sub, &grow_info_cnt);
        grow_info_p = grow_info_sub;
      }
      else
      {
        grow_GetGraphAttrInfo( graph->grow->ctx, &grow_info, &grow_info_cnt);
        grow_info_p = grow_info;
      }
      for ( i = 0; i < grow_info_cnt; i++)
      {
        cdh_ToUpper( attr_name, grow_info_p->name);
        if ( strcmp( arg2_str, attr_name) == 0)
        {
          found = 1;
          break;
        }
        grow_info_p++;
      }
      if ( found)
        break;
    }
    if ( !found)
    {
      graph->message('E', "No such attribute");
      grow_FreeSubGraphAttrInfo( grow_info_sub);
      if ( grow_info)
        grow_FreeGraphAttrInfo( grow_info);
      return GE__NOATTR;
    }
    switch( grow_info_p->type)
    {
      case glow_eType_Int:
      case glow_eType_TraceColor:
      case glow_eType_TraceType:
      case glow_eType_Access:
      case glow_eType_Boolean:
      case glow_eType_Direction:
      case glow_eType_Color:
      case glow_eType_ToneOrColor:
        sts = sscanf( arg3_str, "%d", &i_value);
        if ( sts != 1)
        {
          graph->message('E', "Syntax error");
          grow_FreeSubGraphAttrInfo( grow_info_sub);
          if ( grow_info)
            grow_FreeGraphAttrInfo( grow_info);
          return GE__SYNTAX;
        }
        memcpy( grow_info_p->value_p, (char *)&i_value, sizeof(i_value));
        break;
      case glow_eType_Double:
        sts = sscanf( arg3_str, "%f", &f_value);
        if ( sts != 1)
        {
          graph->message('E', "Syntax error");
          grow_FreeSubGraphAttrInfo( grow_info_sub);
          if ( grow_info)
            grow_FreeGraphAttrInfo( grow_info);
          return GE__SYNTAX;
        } 
        d_value = double(f_value);
        memcpy( grow_info_p->value_p, (char *)&d_value, sizeof(d_value));
        break;
      case glow_eType_String:
        strncpy( (char *) grow_info_p->value_p, arg3_str, grow_info_p->size);
        break;
      default:
        ;
    }
    grow_FreeSubGraphAttrInfo( grow_info_sub);
    if ( grow_info)
      grow_FreeGraphAttrInfo( grow_info);
  }
  else if ( strncmp( arg1_str, "EXTERN", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int		sts;
    grow_tObject nodeclass;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str))) {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    cdh_ToLower( arg2_str, arg2_str);
    sts = grow_FindNodeClassByName( graph->grow->ctx, arg2_str, &nodeclass);
    if ( EVEN(sts)) {
      graph->message('E', "Subgraph not found");
      return GE__NOSUBGRAPH;
    }

    grow_SetNodeClassExtern( nodeclass, 1);
  }
  else if ( strncmp( arg1_str, "INTERN", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int		sts;
    grow_tObject nodeclass;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str))) {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    cdh_ToLower( arg2_str, arg2_str);
    sts = grow_FindNodeClassByName( graph->grow->ctx, arg2_str, &nodeclass);
    if ( EVEN(sts)) {
      graph->message('E', "Subgraph not found");
      return GE__NOSUBGRAPH;
    }

    grow_SetNodeClassExtern( nodeclass, 0);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}

static int	graph_add_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "POLYLINE", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x, y;
    glow_sPoint points[2];
    int 	point_cnt;

    if ( !graph->current_cmd_object)
    {
      graph->message('E', "No current object");
      return GE__NOCURRENT;
    }
    if ( grow_GetObjectType( graph->current_cmd_object) != 
		glow_eObjectType_GrowPolyLine)
    {
      graph->message('E', "Current object type doesn't support this operation"); 
      return GE__CURRENT_TYPE;
    }
    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y = value;
    
    points[0].x = x;
    points[0].y = y;
    point_cnt = 1;
    grow_AddPolyLinePoints( graph->current_cmd_object,
		(glow_sPoint *)&points, point_cnt);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}

static int	graph_rotate_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "CURRENTOBJECT", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double angel;

    if ( !graph->current_cmd_object)
    {
      graph->message('E', "No current object");
      return GE__NOCURRENT;
    }
    if ( EVEN( dcli_get_qualifier( "/ANGEL", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    angel = double(value);
    
    grow_StoreTransform( graph->current_cmd_object);
    grow_SetObjectRotation( graph->current_cmd_object, angel, 0,
	0, glow_eRotationPoint_Center);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}


static int	graph_select_func(	void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "CURRENTOBJECT", strlen( arg1_str)) == 0)
  {
    grow_SetHighlight( graph->current_cmd_object, 1);
    grow_SelectInsert( graph->grow->ctx, graph->current_cmd_object);
  }
  else if ( strncmp( arg1_str, "CLEAR", strlen( arg1_str)) == 0)
  {
    grow_SelectClear( graph->grow->ctx);
  }
  else
  {
    graph->message('E', "Syntax error");
  }
  return GE__SUCCESS;
}

static int	graph_export_func(	void		*client_data,
					void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "JAVA", strlen( arg1_str)) == 0)
  {
    char filename[120];
    char name[80];
    char graph_name[80];
    char *s;
    int sts;

    if ( ! graph->get_java_name( name)) {
      // Set default name
      graph->get_name( graph_name);
      if ( strcmp( graph_name, "") != 0) {
        if ( strncmp( graph_name, "pwr_", 4) == 0)
        {
          strcpy( name, "Jop");
          strcat( name, &graph_name[4]);
          name[3] = _toupper( name[3]);
        }
        else
        {
          strcpy( name, graph_name);
          name[0] = _toupper( name[0]);
        }
      }
      else {
        graph->message( 'E', "Graph is unnamed");
        return GE__NONAME;
      }
    }

    if ( graph->is_subgraph())
    {
      graph->message( 'E', "Unable to save subgraph as ge java");
    }
    else
    {
      char framename[80];
      char appletname[80];
      char systemname[80];
      char cmd[200];

      strcpy( framename, name);
      if ( (s = strrchr( framename, '.')))
        *s = 0;

      strcpy( appletname, name);
      if ( (s = strrchr( appletname, '.')))
        *s = 0;
      strcat( appletname, "_A");

      strcpy( filename, name);
      if ( ! strrchr( filename, '.'))
        strcat( filename, ".java");

      // Export frame
      if ( graph->is_javaapplication()) {
        sts = graph->export_gejava( filename, framename, 0, 0);
        if ( EVEN(sts)) {
          graph->message( 'E', "Java export error");
          return sts;
        }

        // Compile frame
        graph->get_systemname( systemname);

        sprintf( cmd, "$pwr_exe/ge_javac.sh java %s %s", filename, systemname);
        sts = system( cmd);
        if ( sts != 0) {
          graph->message( 'E', "Java compilation errors");
          return GE__JAVAC;
        }
      }

      if ( graph->is_javaapplet()) {
        // Export applet

        strcpy( filename, appletname);
        strcat( filename, ".java");

        sts = graph->export_gejava( filename, appletname, 1, 0);
        if ( EVEN(sts)) {
          graph->message( 'E', "Java export error");
          return sts;
        }

        // Compile applet
        graph->get_systemname( systemname);

        sprintf( cmd, "$pwr_exe/ge_javac.sh java_web %s %s", filename, systemname);
        sts = system( cmd);
        if ( sts != 0) {
          graph->message( 'E', "Java compilation errors");
          return GE__JAVAC;
        }

        // Export html
        strcpy( filename, "$pwrp_web/");
        strcat( filename, framename);
        strcat( filename, ".html");
        cdh_ToLower( filename, filename);

        sts = graph->export_gejava( filename, framename, 0, 1);
        if ( EVEN(sts)) {
          graph->message( 'E', "Java export error");
          return sts;
        }
      }
      if ( graph->is_javaapplication() && graph->is_javaapplet())
        graph->message( 'I', "Java frame and applet exported");
      else if ( graph->is_javaapplication())
        graph->message( 'I', "Java frame exported");
      else if ( graph->is_javaapplet())
        graph->message( 'I', "Java applet exported");
      else
        graph->message( 'I', "This graph is not java frame or applet");
    }
  }
  else
  {
    graph->message('E', "Syntax error");
  }
  return GE__SUCCESS;
}

static int	graph_group_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;
  int sts;
  grow_tObject group;
  char last_group_name[80];
	
  sts = grow_GroupSelect( graph->grow->ctx, &group, last_group_name);
  if ( EVEN(sts)) {
    graph->current_cmd_object = 0;
    graph->message( 'E', "Group error");
    return sts;
  }

  graph->current_cmd_object = group;

  return GE__SUCCESS;
}

static int	graph_scale_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "CURRENTOBJECT", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double scalex, scaley;
    glow_eScaleType scale_type;
    double x0, y0;

    if ( !graph->current_cmd_object)
    {
      graph->message('E', "No current object");
      return GE__NOCURRENT;
    }
    if ( EVEN( dcli_get_qualifier( "/SCALEX", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    scalex = double(value);
    
    if ( EVEN( dcli_get_qualifier( "/SCALEY", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    scaley = double(value);
    
    scale_type =  glow_eScaleType_LowerLeft;
    x0 = 0;
    y0 = 0;

    if ( ODD( dcli_get_qualifier( "/X", str)))
    {
      sts = sscanf( str, "%f", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      x0 = double(value);
      scale_type = glow_eScaleType_FixPoint;
    }

    if ( ODD( dcli_get_qualifier( "/Y", str)))
    {
      sts = sscanf( str, "%f", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      y0 = double(value);
      scale_type = glow_eScaleType_FixPoint;
    }    

    grow_StoreTransform( graph->current_cmd_object);
    grow_SetObjectScale( graph->current_cmd_object, scalex, scaley,
	x0, y0, scale_type);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}

static int	graph_move_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "CURRENTOBJECT", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x0, y0;

    if ( !graph->current_cmd_object)
    {
      graph->message('E', "No current object");
      return GE__NOCURRENT;
    }
    if ( EVEN( dcli_get_qualifier( "/X", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x0 = double(value);
    
    if ( EVEN( dcli_get_qualifier( "/Y", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y0 = double(value);
    
    grow_StoreTransform( graph->current_cmd_object);
    grow_SetObjectPosition( graph->current_cmd_object, x0, y0);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}

static int	graph_create_func( void		*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "RECTANGLE", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x, y, width, height;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y = value;
    
    if ( EVEN( dcli_get_qualifier( "/WIDTH", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    width = value;
    
    if ( EVEN( dcli_get_qualifier( "/HEIGHT", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    height = value;
    
    grow_CreateGrowRect( graph->grow->ctx, "", 
	    x, y, width, height,
	    graph->get_border_drawtype(), graph->linewidth, 0, 
	    glow_mDisplayLevel_1, graph->fill, graph->border,
	    graph->get_fill_drawtype(), NULL, &graph->current_cmd_object);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else if ( strncmp( arg1_str, "ARC", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x1, y1, x2, y2;
    int angel1, angel2;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/X2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x2 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y2 = value;
    
    if ( ODD( dcli_get_qualifier( "/ANGEL1", str)))
    {
      sts = sscanf( str, "%d", &angel1);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
    }
    else
      angel1 = 0;
    
    if ( ODD( dcli_get_qualifier( "/ANGEL2", str)))
    {
      sts = sscanf( str, "%d", &angel2);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
    }
    else 
      angel2 = 360;
    
    grow_CreateGrowArc( graph->grow->ctx, "", 
	x1, y1, x2, y2, angel1, angel2,
	graph->get_border_drawtype(), graph->linewidth,
	graph->fill, graph->border, graph->get_fill_drawtype(), NULL, 
	&graph->current_cmd_object);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else if ( strncmp( arg1_str, "LINE", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x1, y1, x2, y2;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/X2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x2 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y2 = value;
    
    grow_CreateGrowLine( graph->grow->ctx, "", 
	x1, y1, x2, y2,
	graph->get_border_drawtype(), graph->linewidth, 0, NULL, 
	&graph->current_cmd_object);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else if ( strncmp( arg1_str, "POLYLINE", strlen( arg1_str)) == 0)
  {
    char str[80];
    int sts;
    float value;
    double x1, y1, x2, y2;
    glow_sPoint points[2];
    int 	point_cnt;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/X2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x2 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y2", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y2 = value;
    
    points[0].x = x1;
    points[0].y = y1;
    points[1].x = x2;
    points[1].y = y2;
    point_cnt = 2;
    if ( graph->fill)
      grow_CreateGrowPolyLine( graph->grow->ctx, "", 
		(glow_sPoint *)&points, point_cnt,
	      	graph->get_border_drawtype(), graph->linewidth, 0, 
		0, graph->border, 
		graph->get_fill_drawtype(), 0, NULL, 
		&graph->current_cmd_object);
    else
      grow_CreateGrowPolyLine( graph->grow->ctx, "", 
		(glow_sPoint *)&points, point_cnt,
	      	graph->get_border_drawtype(), graph->linewidth, 0, 
		graph->fill, graph->border, 
		graph->get_fill_drawtype(), 0, NULL, 
		&graph->current_cmd_object);

    grow_SetModified( graph->grow->ctx, 1);
  }
  else if ( strncmp( arg1_str, "TEXT", strlen( arg1_str)) == 0)
  {
    char str[80];
    char text_str[80];
    int sts;
    float value;
    double x1, y1;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/TEXT", text_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    
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
    grow_CreateGrowText( graph->grow->ctx, "", text_str,
	    x1, y1,
	    drawtype, textsize, glow_mDisplayLevel_1,
	    NULL, &graph->current_cmd_object);
    grow_SetModified( graph->grow->ctx, 1);
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    char	str[80];
    int 	sts;
    float 	value;
    double 	x1, y1, x2, y2;
    char 	subgraph_str[80];
    char 	name[80];
    grow_tNodeClass nc;
    grow_tNode	n1;
    int		scale_x, scale_y;
    double	sx, sy;
    double	ll_x, ll_y, ur_x, ur_y;

    if ( EVEN( dcli_get_qualifier( "/X1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    x1 = value;
    
    if ( EVEN( dcli_get_qualifier( "/Y1", str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    sts = sscanf( str, "%f", &value);
    if ( sts != 1)
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    y1 = value;
    
    if ( ODD( dcli_get_qualifier( "/X2", str)))
    {
      sts = sscanf( str, "%f", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      x2 = value;
      if ( x1 == x2)
      {
        graph->message('E', "Invalid coordinate");
        return GE__COORDINATE;
      }
      scale_x = 1;
    }
    else
      scale_x = 0;
    
    if ( ODD( dcli_get_qualifier( "/Y2", str)))
    {
      sts = sscanf( str, "%f", &value);
      if ( sts != 1)
      {
        graph->message('E', "Syntax error");
        return GE__SYNTAX;
      }
      y2 = value;
      if ( y1 == y2)
      {
        graph->message('E', "Invalid coordinate");
        return GE__COORDINATE;
      }
      scale_y = 1;
    }
    else
      scale_y = 0;
    
    if ( EVEN( dcli_get_qualifier( "/SUBGRAPH", subgraph_str)))
    {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }
    cdh_ToLower( subgraph_str, subgraph_str);
    
    sts = grow_FindNodeClassByName( graph->grow->ctx, 
		subgraph_str, &nc);
    if ( EVEN(sts))
    {
      // Load the subgraph
        grow_OpenSubGraphFromName( graph->grow->ctx, subgraph_str);
    }
    sts = grow_FindNodeClassByName( graph->grow->ctx, 
		subgraph_str, &nc);
    if ( EVEN(sts))
    {
      graph->message( 'E', "Unable to open subgraph");
      return GE__SUBGRAPHLOAD;
    }

    sprintf( name, "O%d", grow_GetNextObjectNameNumber( graph->grow->ctx));

    if ( !grow_IsSliderClass( nc))
      grow_CreateGrowNode( graph->grow->ctx, name, nc, x1, y1, 
		NULL, &n1);
    else
      grow_CreateGrowSlider( graph->grow->ctx, name, nc, x1, y1, 
		NULL, &n1);

    grow_MoveNode( n1, x1, y1);
    if ( scale_x || scale_y)
    {
      grow_MeasureNode( n1, &ll_x, &ll_y, &ur_x, &ur_y);
      if ( scale_x)
        sx = (x2 - x1)/(ur_x - ll_x);
      else
        sx = 1;
      if ( scale_y)
        sy = (y2 - y1)/(ur_y - ll_y);
      else
        sy = 1;
      grow_StoreTransform( n1);
      grow_SetObjectScale( n1, sx, sy, x1, y1, glow_eScaleType_LowerLeft);
    }
    graph->current_cmd_object = n1;
    grow_SetModified( graph->grow->ctx, 1);
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}

static int	graph_replace_func( void	*client_data,
				void		*client_flag)
{
  Graph *graph = (Graph *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "ATTRIBUTE", strlen( arg1_str)) == 0)
  {
    grow_tObject 	*sel_list;
    int			sel_count;
    glow_sTraceData	*trace_data;
    bool		modified;
    char 		*s;
    char		tmp[120];
    char		from_str[120];
    char		to_str[120];
    int			offs;
    int			replace_cnt = 0;
    int			strict;
    char		str[120];

    if ( EVEN( dcli_get_qualifier( "/FROM", from_str))) {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    if ( EVEN( dcli_get_qualifier( "/TO", to_str))) {
      graph->message('E', "Syntax error");
      return GE__SYNTAX;
    }

    strict = ODD( dcli_get_qualifier( "/STRICT", 0));
    if ( !strict)
      cdh_ToLower( from_str, from_str);

    grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
    for ( int i = 0; i < sel_count; i++) {
      switch( grow_GetObjectType( sel_list[i])) {
      case glow_eObjectType_GrowGroup:
	graph_group_replace_attr( sel_list[i], from_str, to_str, &replace_cnt, strict);
	// Do this for groups also (no break)
      case glow_eObjectType_GrowNode:
      case glow_eObjectType_GrowSlider:
      case glow_eObjectType_GrowTrend:
      case glow_eObjectType_GrowBar:
	grow_GetTraceAttr( sel_list[i], &trace_data);
	modified = false;
	for ( int j = 0; 
	      j < (int)(sizeof( trace_data->data)/sizeof(trace_data->data[0]));
	      j++) {
	  strncpy( str, trace_data->data[j], sizeof(str));
	  if ( !strict)
	    cdh_ToLower( str, str);
	  s = strstr( str, from_str);
	  if ( s) {
	    offs = (int)( s - str);
	    strcpy( tmp, s + strlen(from_str));
	    strncpy( &trace_data->data[j][offs], to_str, 
		     sizeof(trace_data->data[0]) - offs);
	    trace_data->data[j][sizeof(trace_data->data[0])-1] = 0;
	    strncat( trace_data->data[j], tmp, 
		     sizeof(trace_data->data[0])-strlen(trace_data->data[j]));
	    modified = true;
	  }
	}
	strncpy( str, trace_data->ref_object, sizeof(str));
	if ( !strict)
	  cdh_ToLower( str, str);
	s = strstr( str, from_str);
	if ( s) {
	  offs = (int)( s - str);
	  strcpy( tmp, s + strlen(from_str));
	  strncpy( &trace_data->ref_object[offs], to_str, 
		   sizeof(trace_data->ref_object) - offs);
	  trace_data->ref_object[sizeof(trace_data->ref_object)-1] = 0;
	  strncat( trace_data->ref_object, tmp, 
		   sizeof(trace_data->ref_object)-strlen(trace_data->ref_object));
	  modified = true;
	}
	if ( modified) {
	  grow_SetTraceAttr( sel_list[i], trace_data);
	  replace_cnt++;
	}
	break;
      default:
	;
      }
    }
    if ( replace_cnt) {
      char msg[80];
      sprintf( msg, "%d attributes replaced", replace_cnt);
      graph->message('I', msg);
    }
    else
      graph->message('I', "Nothing replaced");
      
  }
  else
  {
    graph->message('E', "Syntax error");
    return GE__SYNTAX;
  }
  return GE__SUCCESS;
}


static void graph_group_replace_attr( grow_tObject group, char *from_str, char *to_str, 
				      int *replace_cnt, int strict)
{
  grow_tObject 	*objectlist;
  int 		object_cnt;
  glow_sTraceData  *trace_data;
  bool		modified;
  char 		*s;
  char		tmp[120];
  int		offs;
  char		str[120];
  
  grow_GetGroupObjectList( group, &objectlist, &object_cnt);
  for ( int i = 0; i < object_cnt; i++) {

    switch( grow_GetObjectType( objectlist[i])) {
    case glow_eObjectType_GrowGroup:
      graph_group_replace_attr( objectlist[i], from_str, to_str, replace_cnt, strict);
      // Do this for groups also (no break)
    case glow_eObjectType_GrowNode:
    case glow_eObjectType_GrowSlider:
    case glow_eObjectType_GrowTrend:
    case glow_eObjectType_GrowBar:
      grow_GetTraceAttr( objectlist[i], &trace_data);
      modified = false;
      for ( int j = 0; 
	    j < (int)(sizeof( trace_data->data)/sizeof(trace_data->data[0]));
	    j++) {
	strncpy( str, trace_data->data[j], sizeof(str));
	if ( strict)
	  cdh_ToLower( str, str);
	s = strstr( str, from_str);
	if ( s) {
	  offs = (int)( s - str);
	  strcpy( tmp, s + strlen(from_str));
	  strncpy( &trace_data->data[j][offs], to_str, 
		   sizeof(trace_data->data[0]) - offs);
	  trace_data->data[j][sizeof(trace_data->data[0])-1] = 0;
	  strncat( trace_data->data[j], tmp, 
		   sizeof(trace_data->data[0])-strlen(trace_data->data[j]));
	  modified = true;
	}
      }

      strncpy( str, trace_data->ref_object, sizeof(str));
      if ( strict)
	cdh_ToLower( str, str);
      s = strstr( str, from_str);
      if ( s) {
	offs = (int)( s - str);
	strcpy( tmp, s + strlen(from_str));
	strncpy( &trace_data->ref_object[offs], to_str, 
		 sizeof(trace_data->ref_object) - offs);
	trace_data->ref_object[sizeof(trace_data->ref_object)-1] = 0;
	strncat( trace_data->ref_object, tmp, 
		 sizeof(trace_data->ref_object)-strlen(trace_data->ref_object));
	modified = true;
      }
      if ( modified) {
	grow_SetTraceAttr( objectlist[i], trace_data);
	(*replace_cnt)++;
      }
      break;
    default:
      ;
    }
  }
}

int Graph::command( char* input_str)
{
  char		command[160];
  int		sts, sym_sts;
  char		symbol_value[80];

  dcli_toupper( input_str, input_str);
  sts = dcli_replace_symbol( input_str, command, sizeof(command));
  if ( EVEN(sts)) return sts;

  if ( input_str[0] == '@')
  {
    /* Read command file */
    sts = readcmdfile( &command[1]);
    if ( sts == DCLI__NOFILE)
    {
      message('E',"Unable to open file");
      return DCLI__SUCCESS;
    }
    else if ( EVEN(sts)) return sts;
    return DCLI__SUCCESS;
  }

  sts = dcli_cli( (dcli_tCmdTable *)&graph_command_table, command, (void *) this, 0);
  if ( sts == DCLI__COM_NODEF)
  {
    /* Try to find a matching symbol */
    sym_sts = dcli_get_symbol_cmd( command, symbol_value);
    if ( ODD(sym_sts))
    {
      if ( symbol_value[0] == '@')
      {
        /* Read command file */
        sts = readcmdfile( &symbol_value[1]);
        if ( sts == DCLI__NOFILE)
        {
          message('E',"Unable to open file");
          return DCLI__SUCCESS;
        }
        else if ( EVEN(sts)) return sts;
        return DCLI__SUCCESS;
      }
      sts = dcli_cli( (dcli_tCmdTable *)&graph_command_table, symbol_value, (void *) this, 0);
    }
    else if ( sym_sts == DCLI__SYMBOL_AMBIG)
      sts = sym_sts;
  }
  if (sts == DCLI__COM_AMBIG) message('E',"Ambiguous command");
  else if (sts == DCLI__COM_NODEF) message('E',"Undefined command");
  else if (sts == DCLI__QUAL_AMBIG) message('E',"Ambiguous qualifier");
  else if (sts == DCLI__QUAL_NODEF) message('E',"Undefined qualifier");
  else if (sts == DCLI__SYMBOL_AMBIG) message('E', "Ambiguous symbol abbrevation");

  return sts;
}

static int graph_gettextextent_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg 	*arg_p2; 	// Textsize
  ccm_s_arg 	*arg_p3; 	// Bold
  ccm_s_arg 	*arg_p4; 	// Width
  ccm_s_arg 	*arg_p5; 	// Height
  ccm_s_arg 	*arg_p6; 	// Descent
  Graph		*graph;
  glow_eDrawType draw_type;
  double	z_width, z_height, z_descent;
  int		text_idx;

  if ( arg_count < 4 || arg_count > 6)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  arg_p3 = arg_p2->next;
  arg_p4 = arg_p3->next;
  if ( arg_count > 4)
    arg_p5 = arg_p4->next;
  if ( arg_count > 5)
    arg_p6 = arg_p5->next;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;
  if ( arg_p3->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;
  if ( arg_p4->value_decl != CCM_DECL_FLOAT)
    return CCM__ARGMISM;
  if ( arg_count > 4 && arg_p5->value_decl != CCM_DECL_FLOAT)
    return CCM__ARGMISM;
  if ( arg_count > 5 && arg_p6->value_decl != CCM_DECL_FLOAT)
    return CCM__ARGMISM;

  graph_get_stored_graph( &graph);

  if ( arg_p3->value_int)
    draw_type = glow_eDrawType_TextHelveticaBold;
  else
    draw_type = glow_eDrawType_TextHelvetica;

  switch( arg_p2->value_int)
  {
   case 8: text_idx = 0; break;
   case 10: text_idx = 1; break;
   case 12: text_idx = 2; break;
   case 14: text_idx = 3; break;
   case 18: text_idx = 4; break;
   case 24: text_idx = 5; break;
   default:
    text_idx = 0;
  }
  grow_GetTextExtent( graph->grow->ctx, arg_list->value_string, 
	strlen(arg_list->value_string), draw_type,
	text_idx, &z_width, &z_height, &z_descent);

  arg_p4->value_float = float(z_width);
  arg_p4->value_returned = 1;
  
  if ( arg_count > 4)
  {
    arg_p5->value_float = float(z_height);
    arg_p5->value_returned = 1;
  }

  if ( arg_count > 5)
  {
    arg_p5->value_float = float(z_descent);
    arg_p5->value_returned = 1;
  }
  return 1;
}

static int graph_selectadd_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  Graph		*graph;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  graph_get_stored_graph( &graph);

  grow_SelectInsert( graph->grow->ctx, (grow_tObject) arg_list->value_int);
  return 1;
}

static int graph_getcurrentobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  Graph		*graph;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  graph_get_stored_graph( &graph);

  *return_int = (int) graph->current_cmd_object;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int graph_setextern_func(
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  Graph		*graph;
  int sts;
  grow_tObject nodeclass;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  graph_get_stored_graph( &graph);

  sts = grow_FindNodeClassByName( graph->grow->ctx, arg_list->value_string,
				  &nodeclass);
  if ( ODD(sts)) {
    grow_SetNodeClassExtern( nodeclass, 1);
    *return_int = 1;
    *return_decl = CCM_DECL_INT;
  }
  else {
    *return_int = 0;
    *return_decl = CCM_DECL_INT;
  }
  return 1;
}

static int graph_setintern_func(
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  Graph		*graph;
  int sts;
  grow_tObject nodeclass;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  graph_get_stored_graph( &graph);

  sts = grow_FindNodeClassByName( graph->grow->ctx, arg_list->value_string,
				  &nodeclass);
  if ( ODD(sts)) {
    grow_SetNodeClassExtern( nodeclass, 0);
    *return_int = 1;
    *return_decl = CCM_DECL_INT;
  }
  else {
    *return_int = 0;
    *return_decl = CCM_DECL_INT;
  }
  return 1;
}

static int graph_ccm_deffilename_func( char *outfile, char *infile, void *client_data)
{

  dcli_get_defaultfilename( infile, outfile, ".ge_com");
  return 1;
}

static int graph_ccm_errormessage_func( char *msg, int severity, void *client_data)
{
  Graph *graph = (Graph *) client_data; 

  if ( EVEN(severity))
    graph->message( 'I', msg);
  else
    graph->message( 'E', msg);
  return 1;
}

int	graph_externcmd_func( char *cmd, void *client_data)
{
  Graph *graph = (Graph *) client_data; 
  int sts;

  graph->scriptmode++;
  sts = graph->command( cmd);
  graph->scriptmode--;
  return sts;
}

#if LDH
static int graph_wccm_get_ldhsession_cb( void *ctx, ldh_tSesContext *ldhses)
{
  Graph *graph = (Graph *)ctx;
  int  sts;

  if ( graph->ldhses)
    *ldhses = graph->ldhses;
  else
  {
    if ( graph->get_ldhses_cb)
    {
      sts = (graph->get_ldhses_cb)( graph->parent_ctx, &graph->ldhses, 1);
      if ( EVEN(sts)) return sts;

      *ldhses = graph->ldhses;
    }
    else
      return 0;
  }
  return 1;
}
#endif

#if LDH
static int graph_wccm_get_wbctx_cb( void *ctx, ldh_tWBContext *wbctx)
{
  Graph *graph = (Graph *)ctx;
  int	sts;

  if ( graph->ldhses)
    *wbctx = ldh_SessionToWB( graph->ldhses);
  else
  {
    if ( graph->get_ldhses_cb)
    {
      sts = (graph->get_ldhses_cb)( graph->parent_ctx, &graph->ldhses, 1);
      if ( EVEN(sts)) return sts;

      *wbctx = ldh_SessionToWB( graph->ldhses);
    }
    else
      return 0;
  }
  return 1;
}
#endif


/*************************************************************************
*
* Name:		readcmdfile()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

int Graph::readcmdfile( 	char		*incommand)
{
  char		input_str[160];
  int		sts;
  int		appl_sts;

  if ( !ccm_func_registred)
  {
    sts = ccm_register_function( "GetTextExtent", graph_gettextextent_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetCurrentObject", graph_getcurrentobject_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "SelectAdd", graph_selectadd_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "SetExtern", graph_setextern_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "SetIntern", graph_setintern_func);
    if ( EVEN(sts)) return sts;
    ccm_func_registred = 1;

    // Register wb standard functions
#if LDH
    wccm_register( graph_wccm_get_wbctx_cb, graph_wccm_get_ldhsession_cb);
#endif
  }

  strcpy( input_str, incommand);
  dcli_remove_blank( input_str, input_str);
  graph_store_graph( this);
#if LDH
  if ( ldhses)
    wccm_store_ldhses( this, ldhses);
  else
    wccm_store_client( this);
#endif
  // Read and execute the command file
  sts = ccm_file_exec( input_str, graph_externcmd_func,
		graph_ccm_deffilename_func, graph_ccm_errormessage_func, 
		&appl_sts, verify, 0, NULL, 0, 0, NULL, (void *)this);
  if ( EVEN(sts)) return sts;

  return 1;
}

