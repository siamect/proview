/* 
 * Proview   $Id: flow_annot.cpp,v 1.5 2005-09-01 14:56:12 claes Exp $
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


#include "flow_std.h"

#include <iostream.h>
#include <math.h>
#include <stdlib.h>

#include "flow_annot.h"
#include "flow_node.h"
#include "flow_draw.h"
#include "flow_browctx.h"

void FlowAnnot::zoom()
{
  p.zoom();
}

void FlowAnnot::nav_zoom()
{
  p.nav_zoom();
}

void FlowAnnot::print_zoom()
{
  p.print_zoom();
}

void FlowAnnot::traverse( int x, int y)
{
  p.traverse( x, y);
}

void FlowAnnot::print( void *pos, void *node, int highlight)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((FlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);
  double x;

  if ( size <= 0)
    return;

  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
      {
        x = ((FlowPoint *)pos)->print_z_x + MAX( p.print_z_x,
           ((FlowNode *)node)->rel_annot_x[number] * ctx->print_zoom_factor);
      }
      else
        x = p.print_z_x + ((FlowPoint *)pos)->print_z_x;
      ctx->print_ps->text( x, 
	p.print_z_y + ((FlowPoint *)pos)->print_z_y, 
	((FlowNode *) node)->annotv[number], 
	strlen(((FlowNode *) node)->annotv[number]), draw_type, size);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int z_width, z_height;
      int z_x = int( p.print_z_x + ((FlowPoint *)pos)->print_z_x);
      int z_y = int( p.print_z_y + ((FlowPoint *)pos)->print_z_y);
      int len = 0;
      int line_cnt = 0;
      char *line = ((FlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, text_size, &z_width, &z_height);
      z_height = int( ctx->print_zoom_factor / ctx->base_zoom_factor * z_height);
      for ( s = ((FlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            ctx->print_ps->text( z_x, z_y + line_cnt * z_height, line, len, 
		draw_type, size);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        ctx->print_ps->text( z_x, z_y + line_cnt * z_height, line, 
		len, draw_type, size);
      break;
    }
  }
}

void FlowAnnot::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Annot) << endl;
  fp << int(flow_eSave_Annot_number) << FSPACE << number << endl;
  fp << int(flow_eSave_Annot_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Annot_text_size) << FSPACE << text_size << endl;
  fp << int(flow_eSave_Annot_display_level) << FSPACE << int(display_level) << endl;
  fp << int(flow_eSave_Annot_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_Annot_annot_type) << FSPACE << int(annot_type) << endl;
  fp << int(flow_eSave_End) << endl;
}

void FlowAnnot::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Annot: break;
      case flow_eSave_Annot_number: fp >> number; break;
      case flow_eSave_Annot_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Annot_text_size: fp >> text_size; break;
      case flow_eSave_Annot_display_level: fp >> tmp; display_level = (flow_mDisplayLevel)tmp; break;
      case flow_eSave_Annot_p: p.open( fp); break;
      case flow_eSave_Annot_annot_type: fp >> tmp; 
	annot_type = (flow_eAnnotType)tmp; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowAnnot:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowAnnot::draw( void *pos, int highlight, int hot, void *node)
{
  int x;

  if ( !(display_level & ctx->display_level))
    return;

  if ( !((FlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
      {
        x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor));
      }
      else
        x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      flow_draw_text( ctx, x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	((FlowNode *) node)->annotv[number], 
	strlen(((FlowNode *) node)->annotv[number]), draw_type, idx, 
	highlight, 0);
      if ( ((FlowNode *) node)->annotv_inputmode[number])
        flow_draw_move_input( ctx, 
	  ((FlowNode *) node)->annotv_input[number],
	  x, p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y,
	  flow_ePosition_Absolute);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int z_width, z_height;
      int z_x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      int z_y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((FlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height);
      for ( s = ((FlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            flow_draw_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        flow_draw_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
      break;
    }
  }
}

void FlowAnnot::erase( void *pos, int hot, void *node)
{
  int x;

  if ( !(display_level & ctx->display_level))
    return;

  if ( !((FlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
      {
        x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor));
      }
      else
        x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      flow_draw_text_erase( ctx, x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	((FlowNode *) node)->annotv[number], 
	strlen(((FlowNode *) node)->annotv[number]), draw_type, idx, 0);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int z_width, z_height;
      int z_x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      int z_y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((FlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height);
      for ( s = ((FlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            flow_draw_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        flow_draw_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
      break;
    }
  }
}

void FlowAnnot::nav_draw( void *pos, int highlight, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((FlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      flow_draw_nav_text( ctx, 
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	((FlowNode *) node)->annotv[number], 
	strlen(((FlowNode *) node)->annotv[number]), draw_type, idx, 
	highlight, 0);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int z_width, z_height;
      int z_x = p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x;
      int z_y = p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((FlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height);
      for ( s = ((FlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            flow_draw_nav_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        flow_draw_nav_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
      break;
    }
  }
}

void FlowAnnot::nav_erase( void *pos, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((FlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      flow_draw_nav_text_erase( ctx,
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	((FlowNode *) node)->annotv[number], 
	strlen(((FlowNode *) node)->annotv[number]), draw_type, idx, 0);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int z_width, z_height;
      int z_x = p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x;
      int z_y = p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((FlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height);
      for ( s = ((FlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            flow_draw_nav_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        flow_draw_nav_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
      break;
    }
  }
}

int FlowAnnot::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
/**
  if ( p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x < x && 
       x < p2.z_x  + ((FlowPoint *)pos)->z_x - ctx->offset_x &&
       p1.z_y  + ((FlowPoint *)pos)->z_y - ctx->offset_y < y && 
       y < p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y)
  {
    cout << "Event handler: Hit in text" << endl;
    return 1;
  }  
  else
***/
    return 0;
}

void FlowAnnot::get_borders( double pos_x, double pos_y,
	double *x_right, double *x_left, double *y_high, double *y_low, 
	void *node)
{
  if ( !((FlowNode *) node)->annotv[number])
    return;
  if ( display_level != flow_mDisplayLevel_1)
    return;

  if ( annot_type == flow_eAnnotType_MultiLine)
  {
    double 	width, height;
    int		rows;

    flow_measure_annot_text( ctx, ((FlowNode *) node)->annotv[number], 
		draw_type, text_size, annot_type, &width, &height, &rows);
    if ( pos_x + p.x < *x_left)
      *x_left = pos_x + p.x;
    if ( pos_x + p.x + width > *x_right)
      *x_right = pos_x + p.x + width;
    if ( pos_y + p.y < *y_low)
      *y_low = pos_y + p.y;
    if ( pos_y + p.y + height > *y_high)
      *y_high = pos_y + p.y + height;
  }
}

void FlowAnnot::measure_annot( char *text, double *width, double *height)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int	z_width, z_height;

  if ( idx < 0)
  {
    *width = 0;
    *height = 0;
    return;
  }
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      draw_get_text_extent( ctx, text, strlen(text), draw_type, idx, 
		&z_width, &z_height);
      break;
    case flow_eAnnotType_MultiLine:
    {
      int l_width, l_height;
      int len = 0;
      int line_cnt = 0;
      char *line = text;
      char *s;
      z_width = 0;
      for ( s = text; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
	  {
            draw_get_text_extent( ctx, line, len, draw_type, idx, 
		&l_width, &l_height);
	    z_width = MAX( z_width, l_width);
	  }
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
      {
        draw_get_text_extent( ctx, line, len, draw_type, idx, 
		&l_width, &l_height);
	z_width = MAX( z_width, l_width);
        line_cnt++;
      }
      z_height = line_cnt * l_height;      
      break;
    }
  }
  *width = z_width / ctx->zoom_factor;
  *height = z_height / ctx->zoom_factor;
}

void FlowAnnot::configure_annotations( void *pos, void *node)
{
  double width, height;

  if ( !((FlowNode *) node)->annotv[number])
    return;

  if ( relative_pos)
    ((FlowNode *)node)->rel_annot_x[number] = 
		((FlowNode *)node)->relative_annot_x;
  measure_annot( ((FlowNode *) node)->annotv[number], &width, &height);
  ((FlowNode *)node)->relative_annot_x = 
	MAX( p.x, ((FlowNode *)node)->rel_annot_x[number]) + 
	width + ((BrowCtx *)ctx)->annotation_space;
}

void FlowAnnot::open_annotation_input( void *pos, void *node)
{

  if ( !((FlowNode *) node)->annotv[number])
    return;

  int x, width_pix, height_pix;
  double width, height;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (text_size +4) - 4);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      measure_annot( ((FlowNode *) node)->annotv[number], &width, &height);
      width_pix = int( width * ctx->zoom_factor);
      height_pix = int( height * ctx->zoom_factor);
      if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
      {
        x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor));
      }
      else
        x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      flow_draw_create_input( ctx, x,
		p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y,
		((FlowNode *) node)->annotv[number],
		strlen(((FlowNode *) node)->annotv[number]), idx, width_pix, 
		height_pix, node, number, 
		&((FlowNode *) node)->annotv_input[number]);
      break;
    case flow_eAnnotType_MultiLine:
        // Not yet implemented...
      break;
  }
}

void FlowAnnot::close_annotation_input( void *node)
{

  if ( !((FlowNode *) node)->annotv_inputmode[number])
    return;

  flow_draw_close_input( ctx, ((FlowNode *) node)->annotv_input[number]);
}

int FlowAnnot::get_annotation_input( void *node, char **text)
{

  if ( !((FlowNode *) node)->annotv_inputmode[number])
    return 0;

  return flow_draw_get_input( ctx, ((FlowNode *) node)->annotv_input[number], 
	text);
}

void FlowAnnot::move_widgets( void *node, int x, int y)
{
  if ( !((FlowNode *) node)->annotv_inputmode[number])
    return;

  flow_draw_move_input( ctx,  ((FlowNode *) node)->annotv_input[number], 
	x, y, flow_ePosition_Relative);
}

ostream& operator<< ( ostream& o, const FlowAnnot a)
{
  o << '(' << a.p.x << ',' << a.p.y << ')'; 
  return o;
}

void flow_measure_annot_text( FlowCtx *ctx, char *text, flow_eDrawType draw_type, 
		int text_size, flow_eAnnotType annot_type, 
		double *width, double *height, int *rows)
{
  int	z_width, z_height;

  switch ( annot_type) {
    case flow_eAnnotType_OneLine:
      draw_get_text_extent( ctx, text, strlen(text), draw_type, text_size, 
		&z_width, &z_height);
      *rows = 1;
      break;
    case flow_eAnnotType_MultiLine:
    {
      int l_width, l_height;
      int len = 0;
      int line_cnt = 0;
      char *line = text;
      char *s;
      z_width = 0;
      for ( s = text; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
	  {
            draw_get_text_extent( ctx, line, len, draw_type, text_size, 
		&l_width, &l_height);
	    z_width = MAX( z_width, l_width);
	  }
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
      {
        draw_get_text_extent( ctx, line, len, draw_type, text_size, 
		&l_width, &l_height);
	z_width = MAX( z_width, l_width);
        line_cnt++;
      }
      z_height = line_cnt * l_height;      
      *rows = MAX( line_cnt, 1);
      break;
    }
  }
  *width = 1.0 * z_width / ctx->base_zoom_factor;
  *height = 1.0 * z_height / ctx->base_zoom_factor;
}

