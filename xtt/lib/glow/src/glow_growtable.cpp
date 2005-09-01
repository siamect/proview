/* 
 * Proview   $Id: glow_growtable.cpp,v 1.8 2005-09-01 14:57:54 claes Exp $
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

#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "co_dcli.h"
#include "glow_growtable.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_growscrollbar.h"

GrowTable::GrowTable( GlowCtx *glow_ctx, char *name, double x, double y, 
		double w, double h, glow_eDrawType border_d_type, int line_w, int fill_rect,
		glow_eDrawType fill_d_type, glow_mDisplayLevel display_lev, int nodraw) : 
		GrowRect(glow_ctx,name,x,y,w,h,border_d_type,line_w,0,
		display_lev,fill_rect,1,0,fill_d_type,nodraw), user_data(0),
		vertical_scrollbar(0), horizontal_scrollbar(0),
		scrollbar_width(0.5), v_scrollbar(0), h_scrollbar(0),
		v_value(0), h_value(0), table_x0(0), table_x1(0), table_y0(0), table_y1(0),
		scrollbar_color(glow_eDrawType_LightGray), scrollbar_bg_color(glow_eDrawType_MediumGray),
		window_scale(1), y_low_offs(0), x_left_offs(0), rows(10), columns(2), header_row(0), 
		header_column(0), text_size(1),
		text_drawtype(glow_eDrawType_TextHelvetica), text_color_drawtype(glow_eDrawType_Line),
		header_text_size(1),
		header_text_drawtype(glow_eDrawType_TextHelvetica), header_text_color(glow_eDrawType_Line),
		header_row_height(0.8), row_height(0.6), value_size(0), cell_value(0), selected_cell_row(-1),
		selected_cell_column(-1), input_focus(0), options(glow_mTableOptions_No)
{
  for ( int i = 0; i < TABLE_MAX_COL; i++) {
    strcpy( header_text[i], "");
    column_width[i] = 4;
    column_size[i] = 12;
  }

  configure();

  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
}

GrowTable::~GrowTable()
{
  if ( !ctx->nodraw) {
    erase();
    nav_erase();
  }
  if ( v_scrollbar)
    delete v_scrollbar;
  if ( h_scrollbar)
    delete h_scrollbar;

  if ( cell_value)
    free( cell_value);
}

void GrowTable::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowTable) << endl;
  fp << int(glow_eSave_GrowTable_scrollbar_width) << FSPACE << scrollbar_width << endl;
  fp << int(glow_eSave_GrowTable_scrollbar_color) << FSPACE << scrollbar_color << endl;
  fp << int(glow_eSave_GrowTable_scrollbar_bg_color) << FSPACE << scrollbar_bg_color << endl;
  fp << int(glow_eSave_GrowTable_vertical_scrollbar) << FSPACE << vertical_scrollbar << endl;
  fp << int(glow_eSave_GrowTable_horizontal_scrollbar) << FSPACE << horizontal_scrollbar << endl;
  fp << int(glow_eSave_GrowTable_window_scale) << FSPACE << window_scale << endl;
  fp << int(glow_eSave_GrowTable_rows) << FSPACE << rows << endl;
  fp << int(glow_eSave_GrowTable_columns) << FSPACE << columns << endl;
  fp << int(glow_eSave_GrowTable_header_row) << FSPACE << header_row << endl;
  fp << int(glow_eSave_GrowTable_header_column) << FSPACE << header_column << endl;
  fp << int(glow_eSave_GrowTable_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_GrowTable_text_drawtype) << FSPACE << text_drawtype << endl;
  fp << int(glow_eSave_GrowTable_text_color_drawtype) << FSPACE << text_color_drawtype << endl;
  fp << int(glow_eSave_GrowTable_header_text_size) << FSPACE << header_text_size << endl;
  fp << int(glow_eSave_GrowTable_header_text_drawtype) << FSPACE << header_text_drawtype << endl;
  fp << int(glow_eSave_GrowTable_header_text_color) << FSPACE << header_text_color << endl;
  fp << int(glow_eSave_GrowTable_header_text_bold) << FSPACE << header_text_bold << endl;
  fp << int(glow_eSave_GrowTable_header_row_height) << FSPACE << header_row_height << endl;
  fp << int(glow_eSave_GrowTable_row_height) << FSPACE << row_height << endl;
  fp << int(glow_eSave_GrowTable_options) << FSPACE << options << endl;
  for ( int i = 0; i < TABLE_MAX_COL; i++) {
    fp << int(glow_eSave_GrowTable_column_width1)+2*i << FSPACE << column_width[i] << endl;
    fp << int(glow_eSave_GrowTable_header_text1)+2*i << FSPACE << header_text[i] << endl;
  }
  fp << int(glow_eSave_GrowTable_rect_part) << endl;
  GrowRect::save( fp, mode);
  if ( user_data && ctx->userdata_save_callback) {
    fp << int(glow_eSave_GrowTable_userdata_cb) << endl;
    (ctx->userdata_save_callback)(&fp, this, glow_eUserdataCbType_Node);
  }
  fp << int(glow_eSave_End) << endl;
}

void GrowTable::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowTable: break;
      case glow_eSave_GrowTable_scrollbar_width: fp >> scrollbar_width; break;
      case glow_eSave_GrowTable_scrollbar_color: fp >> tmp; scrollbar_color = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_scrollbar_bg_color: fp >> tmp;
	scrollbar_bg_color = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_vertical_scrollbar: fp >> vertical_scrollbar; break;
      case glow_eSave_GrowTable_horizontal_scrollbar: fp >> horizontal_scrollbar; break;
      case glow_eSave_GrowTable_window_scale: fp >> window_scale; break;
      case glow_eSave_GrowTable_rows: fp >> rows; break;
      case glow_eSave_GrowTable_columns: fp >> columns; break;
      case glow_eSave_GrowTable_header_row: fp >> header_row; break;
      case glow_eSave_GrowTable_header_column: fp >> header_column; break;
      case glow_eSave_GrowTable_text_size: fp >> text_size; break;
      case glow_eSave_GrowTable_text_drawtype: fp >> tmp; text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_text_color_drawtype: fp >> tmp; text_color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_header_text_size: fp >> header_text_size; break;
      case glow_eSave_GrowTable_header_text_drawtype: fp >> tmp; header_text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_header_text_color: fp >> tmp; header_text_color = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTable_header_text_bold: fp >> header_text_bold; break;
      case glow_eSave_GrowTable_header_row_height: fp >> header_row_height; break;
      case glow_eSave_GrowTable_row_height: fp >> row_height; break;
      case glow_eSave_GrowTable_options: fp >> tmp; options = (glow_mTableOptions)tmp; break;
      case glow_eSave_GrowTable_column_width1: fp >> column_width[0]; break;
      case glow_eSave_GrowTable_header_text1: fp.get(); fp.getline( header_text[0], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width2: fp >> column_width[1]; break;
      case glow_eSave_GrowTable_header_text2: fp.get(); fp.getline( header_text[1], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width3: fp >> column_width[2]; break;
      case glow_eSave_GrowTable_header_text3: fp.get(); fp.getline( header_text[2], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width4: fp >> column_width[3]; break;
      case glow_eSave_GrowTable_header_text4: fp.get(); fp.getline( header_text[3], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width5: fp >> column_width[4]; break;
      case glow_eSave_GrowTable_header_text5: fp.get(); fp.getline( header_text[4], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width6: fp >> column_width[5]; break;
      case glow_eSave_GrowTable_header_text6: fp.get(); fp.getline( header_text[5], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width7: fp >> column_width[6]; break;
      case glow_eSave_GrowTable_header_text7: fp.get(); fp.getline( header_text[6], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width8: fp >> column_width[7]; break;
      case glow_eSave_GrowTable_header_text8: fp.get(); fp.getline( header_text[7], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width9: fp >> column_width[8]; break;
      case glow_eSave_GrowTable_header_text9: fp.get(); fp.getline( header_text[8], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width10: fp >> column_width[9]; break;
      case glow_eSave_GrowTable_header_text10: fp.get(); fp.getline( header_text[9], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width11: fp >> column_width[10]; break;
      case glow_eSave_GrowTable_header_text11: fp.get(); fp.getline( header_text[10], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_column_width12: fp >> column_width[11]; break;
      case glow_eSave_GrowTable_header_text12: fp.get(); fp.getline( header_text[11], sizeof(header_text[0])); break;
      case glow_eSave_GrowTable_rect_part: 
        GrowRect::open( fp);
        break;
      case glow_eSave_GrowTable_userdata_cb:
	if ( ctx->userdata_open_callback)
	  (ctx->userdata_open_callback)(&fp, this, glow_eUserdataCbType_Node);
	break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowTable:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }

  configure();
  configure_scrollbars();
}

void GrowTable::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;

  if ( ll_x > ur_x)
  {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if ( ll_y > ur_y)
  {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y >= ll_y &&
       	y_low * ctx->zoom_factor_y- ctx->offset_y <= ur_y) {
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowTable::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ll_x = int( x_left * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ur_y = int( y_high * ctx->zoom_factor_y) - ctx->offset_y;
  int   obj_ll_y = int( y_low * ctx->zoom_factor_y) - ctx->offset_y;


  if ( *ll_x > *ur_x)
  {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if ( *ll_y > *ur_y)
  {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y) {
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);

    // Increase the redraw area
    if ( obj_ur_x > *ur_x)
      *ur_x = obj_ur_x;
    if ( obj_ur_y > *ur_y)
      *ur_y = obj_ur_y;
    if ( obj_ll_x < *ll_x)
      *ll_x = obj_ll_x;
    if ( obj_ll_y < *ll_y)
      *ll_y = obj_ll_y;
  }
}

void GrowTable::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y);

  if ( x_right_pix >= ll_x &&
       x_left_pix <= ur_x &&
       y_high_pix >= ll_y &&
       y_low_pix <= ur_y) {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
  }
}

void GrowTable::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowTable::draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  glow_eDrawType drawtype;

  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  int text_idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  text_idx = min( text_idx, DRAW_TYPE_SIZE-1);
  int header_text_idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (header_text_size +4) - 4);
  header_text_idx = min( header_text_idx, DRAW_TYPE_SIZE-1);

  int ll_x, ll_y, ur_x, ur_y;
  double dx1, dy1, dx2, dy2;

  if (!t) {
    dx1 = trf.x( ll.x, ll.y);
    dy1 = trf.y( ll.x, ll.y);
    dx2 = trf.x( ur.x, ur.y);
    dy2 = trf.y( ur.x, ur.y);
  }
  else {
    dx1 = trf.x( t, ll.x, ll.y);
    dy1 = trf.y( t, ll.x, ll.y);
    dx2 = trf.x( t, ur.x, ur.y);
    dy2 = trf.y( t, ur.x, ur.y);
  }
  dx1 = min( dx1, dx2);
  dx2 = max( dx1, dx2);
  dy1 = min( dy1, dy2);
  dy2 = max( dy1, dy2);

  if ( v_scrollbar) {
    if ( !h_scrollbar)
      v_scrollbar->set_position( dx2 - scrollbar_width, dy1 + y_low_offs, scrollbar_width, 
				 dy2 - (dy1 + y_low_offs));
    else
      v_scrollbar->set_position( dx2 - scrollbar_width, dy1 + y_low_offs, scrollbar_width, 
				 dy2 - (dy1 + y_low_offs) - scrollbar_width);
    v_scrollbar->draw( 0, 0, 0, 0, 0);

  }
  if ( h_scrollbar) {
    if ( !v_scrollbar)
      h_scrollbar->set_position( dx1 + x_left_offs, dy2 - scrollbar_width, dx2 - (dx1 + x_left_offs),
				 scrollbar_width);
    else
      h_scrollbar->set_position( dx1 + x_left_offs, dy2 - scrollbar_width, 
				 dx2 - (dx1 + x_left_offs) - scrollbar_width, scrollbar_width);
    h_scrollbar->draw( 0, 0, 0, 0, 0);
  }

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
					     highlight, (GrowNode *)colornode, 0);

  glow_eDrawType light_drawtype;
  glow_eDrawType dark_drawtype;
  dark_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fill_drawtype, 2, 0);
  light_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fill_drawtype, -2, 0);

  ll_x = int( dx1 * ctx->zoom_factor_x) - ctx->offset_x;
  ll_y = int( dy1 * ctx->zoom_factor_y) - ctx->offset_y;
  ur_x = int( dx2 * ctx->zoom_factor_x) - ctx->offset_x;
  ur_y = int( dy2 * ctx->zoom_factor_y) - ctx->offset_y;

  int o_ll_x = int( (dx1 + x_left_offs) * ctx->zoom_factor_x) - ctx->offset_x;
  int o_ll_y = int( (dy1 + y_low_offs) * ctx->zoom_factor_y) - ctx->offset_y;
  int o_ur_x = int( (dx2 - vertical_scrollbar * scrollbar_width) * ctx->zoom_factor_x) - ctx->offset_x;
  int o_ur_y = int( (dy2 - horizontal_scrollbar * scrollbar_width) * ctx->zoom_factor_y) - ctx->offset_y;

  int t_ll_x = o_ll_x - int( h_value * ctx->zoom_factor_x);
  int t_ll_y = o_ll_y - int( v_value * ctx->zoom_factor_y);;
  int t_ur_x = t_ll_x + int( table_x1 * ctx->zoom_factor_x);
  int t_ur_y = t_ll_y + int( table_y1 * ctx->zoom_factor_y);

  double x;
  double y;
  int offs;
  int header_h = 0;
  int header_w = 0;
  int text_offs = 10;

  if ( header_row)
    header_h = int(header_row_height * ctx->zoom_factor_y);
  if ( header_column)
    header_w = int(column_width[0] * ctx->zoom_factor_x);

  if ( header_row) {

    if ( fill)
      glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, header_h, fill_drawtype);

    glow_draw_set_clip_rectangle( ctx, ll_x + header_w, ll_y, ur_x, ll_y + header_h);

    if ( shadow) {
      x = t_ll_x;
      y = ll_y;
      for ( int i = header_column; i < columns + 1; i++) {
	if ( x > ur_x)
	  break;
	if ( i != header_column)
	  glow_draw_line( ctx, int(x)-1, ll_y, int(x)-1, ll_y + header_h, dark_drawtype, 0, 0);
	if ( i != columns)
	  glow_draw_line( ctx, int(x)+1, ll_y, int(x)+1, ll_y + header_h, light_drawtype, 0, 0);
	x += column_width[i] * ctx->zoom_factor_x;
      }
      glow_draw_line( ctx, ll_x, ll_y+1, t_ur_x, ll_y+1, light_drawtype, 0, 0);
      glow_draw_line( ctx, ll_x, ll_y+header_h-1, t_ur_x, ll_y+header_h-1, dark_drawtype, 1, 0);
    }
    x = t_ll_x;
    y = ll_y;

    for ( int i = header_column; i < columns + 1; i++) {
      if ( x > ur_x)
	break;
      glow_draw_line( ctx, int(x), ll_y, int(x), ll_y + header_h, drawtype, idx, 0);
      x += column_width[i] * ctx->zoom_factor_x;
    }

    x = t_ll_x;
    y = ll_y;

    for ( int i = header_column; i < columns; i++) {
      if ( header_text_idx >= 0 && strcmp( header_text[i], "") != 0)
	glow_draw_text( ctx, int(x + text_offs), int(y + header_h - 4),
		    header_text[i], strlen(header_text[i]), header_text_drawtype, header_text_color,
		    header_text_idx, highlight, 0);
      x += column_width[i] * ctx->zoom_factor_x;
      if ( x > ur_x)
	break;
    }
    glow_draw_reset_clip_rectangle( ctx);

    if ( header_w) {
      if ( shadow) {
	glow_draw_line( ctx, ll_x+header_w-1, ll_y, ll_x+header_w-1, ll_y+header_h, dark_drawtype, 1, 0);
	glow_draw_line( ctx, ll_x-1, ll_y+header_h-1, ll_x+header_w, ll_y+header_h-1, dark_drawtype, 1, 0);
	glow_draw_line( ctx, ll_x, ll_y+1, ll_x + header_w, ll_y+1, light_drawtype, 0, 0);
	glow_draw_line( ctx, ll_x+1, ll_y, ll_x+1, ll_y+header_h, light_drawtype, 0, 0);
      }
      glow_draw_line( ctx, ll_x+header_w, ll_y, ll_x+header_w, ll_y+header_h, drawtype, idx, 0);
    }
    glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x, header_h, drawtype, idx, 0);

    if ( header_column) {
      // Draw header of header column header
      x = ll_x;
      y = ll_y;
      if ( header_text_idx >= 0 && strcmp( header_text[0], "") != 0)
	glow_draw_text( ctx, int(x + text_offs), int(y + header_h - 4),
			header_text[0], strlen(header_text[0]), header_text_drawtype, header_text_color,
			header_text_idx, highlight, 0);
    }
  }

  if ( header_column) {

    if ( fill)
      glow_draw_fill_rect( ctx, ll_x, ll_y + header_h, header_w, ur_y - ll_y - header_h, fill_drawtype);

    glow_draw_set_clip_rectangle( ctx, ll_x, ll_y + header_h, ll_x + header_w, ur_y);

    // Draw selected cell, if cell in header column
    if ( selected_cell_row >= 0 && selected_cell_column == 0) {
      x = ll_x;
      y = t_ll_y + row_height * ctx->zoom_factor_y * selected_cell_row;
      glow_draw_fill_rect( ctx, int(x), int(y), header_w, int(row_height * ctx->zoom_factor_y), dark_drawtype);
    }

    if ( shadow) {
      x = ll_x;
      y = t_ll_y;
      for ( int i = 0; i < rows + 1; i++) {
	if ( y > ur_y)
	  break;
	if ( y > ll_y) {
	  if ( i != 0)
	    glow_draw_line( ctx, ll_x, int(y)-1, ll_x + header_w, int(y)-1, dark_drawtype, 0, 0);
	  if ( i != rows)
	    glow_draw_line( ctx, ll_x, int(y)+1, ll_x + header_w, int(y)+1, light_drawtype, 0, 0);
	}
	y += row_height * ctx->zoom_factor_y;
      }
      glow_draw_line( ctx, ll_x+1, ll_y+header_h, ll_x+1, ur_y, light_drawtype, 0, 0);
      glow_draw_line( ctx, ll_x+header_w-1, ll_y+header_h, ll_x+header_w-1, ur_y, dark_drawtype, 1, 0);
    }
    x = ll_x;
    y = t_ll_y;

    for ( int i = 0; i < rows; i++) {
      y += row_height * ctx->zoom_factor_y;
      if ( y - row_height * ctx->zoom_factor_y > ur_y)
	break;
      if ( y > ll_y) {
	glow_draw_line( ctx, ll_x, int(y), ll_x + header_w, int(y), drawtype, idx, 0);

	offs = column_size[0] * i;
	if ( text_idx >= 0 && strcmp( cell_value + offs, "") != 0)
	  glow_draw_text( ctx, int(x + text_offs), int(y - 2),
		    cell_value + offs, strlen(cell_value + offs), text_drawtype, text_color_drawtype,
		    text_idx, highlight, 0);
      }
    }
    glow_draw_reset_clip_rectangle( ctx);
    glow_draw_rect( ctx, ll_x, ll_y + header_h-1, header_w, ur_y - ll_y - header_h+1, drawtype, idx, 0);
  }

  // Draw table
  if ( fill)
    glow_draw_fill_rect( ctx, o_ll_x, o_ll_y, o_ur_x - o_ll_x, o_ur_y - o_ll_y, fill_drawtype);

  glow_draw_set_clip_rectangle( ctx, o_ll_x, o_ll_y, o_ur_x, o_ur_y);

  if ( selected_cell_row >= 0 && !(header_column && selected_cell_column == 0)) {
    // Draw selected cell, if cell not in header column
    x = t_ll_x;
    for ( int i = header_column; i < selected_cell_column; i++)
      x += column_width[i] * ctx->zoom_factor_x;
    y = t_ll_y + row_height * ctx->zoom_factor_y * selected_cell_row;
    glow_draw_fill_rect( ctx, int(x), int(y), int(column_width[selected_cell_column] * ctx->zoom_factor_x), 
			 int(row_height * ctx->zoom_factor_y), dark_drawtype);
  }

  if ( shadow) {
    x = t_ll_x;
    y = t_ll_y;

    for ( int i = header_column; i < columns + 1; i++) {
      if ( x > ur_x)
	break;
      if ( x > ll_x) {
	if ( i != header_column)
	  glow_draw_line( ctx, int(x)-1, t_ll_y, int(x)-1, t_ur_y, dark_drawtype, 0, 0);
	if ( i != columns)
	  glow_draw_line( ctx, int(x)+1, t_ll_y, int(x)+1, t_ur_y, light_drawtype, 0, 0);
      }
      x += column_width[i] * ctx->zoom_factor_x;
    }

    for ( int i = 0; i < rows + 1; i++) {
      if ( y > ur_y)
	break;
      if ( y > ll_y) {
	if ( i != 0)
	  glow_draw_line( ctx, t_ll_x, int(y-1), t_ur_x, int(y)-1, dark_drawtype, 0, 0);
	if ( i != rows)
	  glow_draw_line( ctx, t_ll_x, int(y)+1, t_ur_x, int(y)+1, light_drawtype, 0, 0);
      }
      y += row_height * ctx->zoom_factor_y;
    }
  }

  x = t_ll_x;
  y = t_ll_y;

  for ( int i = header_column; i < columns + 1; i++) {
    if ( x > ur_x)
      break;
    if ( x > ll_x)
      glow_draw_line( ctx, int(x), t_ll_y, int(x), t_ur_y, drawtype, idx, 0);
    x += column_width[i] * ctx->zoom_factor_x;
  }

  for ( int i = 0; i < rows + 1; i++) {
    if ( y > ur_y) 
      break;
    if ( y > ll_y)
      glow_draw_line( ctx, t_ll_x, int(y), t_ur_x, int(y), drawtype, idx, 0);
    y += row_height * ctx->zoom_factor_y;
  }

  // Draw text values
  int column_offs = header_column * column_size[0] * rows;
  x = t_ll_x;
  int x_border = ll_x - int(column_width[0] * ctx->zoom_factor_x);
  for ( int i = header_column; i < columns; i++) {
    if ( x > ur_x)
      break;

    if ( x > x_border) {
      // Draw column i
      y = t_ll_y;
      for ( int j = 0; j < rows; j++) {
	y += row_height * ctx->zoom_factor_y;
	if ( y - row_height * ctx->zoom_factor_y > ur_y)
	  break;

	if ( options & glow_mTableOptions_ZeroIfHeader) {
	  // Don't draw the row if the value in the first column is zero
	  if ( strcmp( cell_value + j * column_size[0], "") == 0)
	    continue;
	}
	if ( options & glow_mTableOptions_ZeroIfHeaderIs0) {
	  // Don't draw the row if the value in the first column is zero
	  char *s;
	  for ( s = cell_value + j * column_size[0]; *s && *s == ' '; s++) ;
	  if ( strcmp( s, "0") == 0)
	    continue;
	}

	if ( y > ll_y) {
	  offs = column_offs + column_size[i] * j;
	  if ( text_idx >= 0 && strcmp( cell_value + offs, "") != 0)
	    glow_draw_text( ctx, int(x + text_offs), int(y - 2),
			    cell_value + offs, strlen(cell_value + offs), text_drawtype, text_color_drawtype,
			    text_idx, highlight, 0);
	}
      }
    }
    x += column_width[i] * ctx->zoom_factor_x;
    column_offs += column_size[i] * rows;
  }

  glow_draw_reset_clip_rectangle( ctx);


  // Draw frame
  glow_draw_rect( ctx, o_ll_x, o_ll_y, ur_x - o_ll_x, ur_y - o_ll_y, drawtype, idx, 0);
  if ( input_focus) {
    glow_draw_line( ctx, ll_x - 2, ll_y - 2, ll_x - 2, ur_y + 2, glow_eDrawType_DarkGray, 0, 0);
    glow_draw_line( ctx, ll_x - 2, ll_y - 2, ur_x + 2, ll_y - 2, glow_eDrawType_DarkGray, 0, 0);
    glow_draw_line( ctx, ll_x - 2, ur_y + 2, ur_x + 2, ur_y + 2, glow_eDrawType_LightGray, 0, 0);
    glow_draw_line( ctx, ur_x + 2, ll_y - 2, ur_x + 2, ur_y + 2, glow_eDrawType_LightGray, 0, 0);
  }
}

void GrowTable::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  ctx->set_draw_buffer_only();
  glow_draw_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, idx);
  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  ctx->reset_draw_buffer_only();
}

void GrowTable::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  glow_eDrawType drawtype;
  int idx;
  idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( fill)
    glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, fill_drawtype);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
  glow_draw_nav_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
		      drawtype, idx, 0);
}

void GrowTable::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  glow_draw_nav_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			    idx);
  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	glow_eDrawType_LineErase);
}

void GrowTable::trace_scan()
{
  if ( trace.p && ctx->trace_scan_func)
    ctx->trace_scan_func( (void *) this, trace.p);
}

int GrowTable::trace_init()
{
  int sts = 1;

  sts = ctx->trace_connect_func( (void *) this, &trace);
  return sts;
}

void GrowTable::trace_close()
{
  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);

}

void GrowTable::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowTable::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase();
    nav_erase();
    switch ( direction)
    {
      case glow_eAlignDirection_CenterVert:
        dx = x - (x_right + x_left) / 2;
        dy = 0;
        break;        
      case glow_eAlignDirection_CenterHoriz:
        dx = 0;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_CenterCenter:
        dx = x - (x_right + x_left) / 2;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_Right:
        dx = x - x_right;
        dy = 0;
        break;        
      case glow_eAlignDirection_Left:
        dx = x - x_left;
        dy = 0;
        break;        
      case glow_eAlignDirection_Up:
        dx = 0;
        dy = y - y_high;
        break;        
      case glow_eAlignDirection_Down:
        dx = 0;
        dy = y - y_low;
        break;        
    }
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;

    draw();
}

void GrowTable::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double cwidth[TABLE_MAX_COL];

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  for ( int i = 0; i < columns; i++)
    cwidth[i] = column_width[i] * ctx->zoom_factor_x;

  ((GrowCtx *)ctx)->export_jbean->table( ll_x, ll_y, ur_x, ur_y,
				       fill_drawtype, fill, rows, columns, header_row,
				       header_column, text_size, text_drawtype,
				       header_row_height * ctx->zoom_factor_y,
				       row_height * ctx->zoom_factor_y, (double *)cwidth,
				       (char *)header_text,
				       pass, shape_cnt, node_cnt, fp);
}

void GrowTable::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
  }
  }  
}

int GrowTable::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts, v_sts, h_sts;

  v_sts = h_sts = 0;
  if ( v_scrollbar)
    v_sts = v_scrollbar->event_handler( event, x, y, fx, fy);
  if (h_scrollbar)
    h_sts = h_scrollbar->event_handler( event, x, y, fx, fy);
  if ( v_sts || h_sts) {
    if ( event == ctx->event_move_node)
      return 0;
    return 1;
  }

  sts = GrowRect::event_handler( event, x, y, fx, fy);
  if ( event == glow_eEvent_ButtonMotion)
    return 0;

  if ( !ctx->trace_started)
    return sts;

  switch ( event) {
  case glow_eEvent_MB1Click:
  case glow_eEvent_MB3Press: {
    int column = -1;
    int row = -1;

    // Find out which cell
    double o_ll_x = x_left + x_left_offs;
    double o_ll_y = y_low + y_low_offs;
    double o_ur_x = x_right - vertical_scrollbar * scrollbar_width;
    double o_ur_y = y_high - horizontal_scrollbar * scrollbar_width;

    double t_ll_x = o_ll_x - h_value;
    double t_ll_y = o_ll_y - v_value;

    if ( o_ll_y <= fy && fy <= o_ur_y) {
      if ( header_column &&
	   x_left <= fx && fx <= x_left + x_left_offs)
	column = 0;
      else if ( o_ll_x <= fx && fx <= o_ur_x) {
	double column_x_right = t_ll_x;
	for ( int i = header_column; i < columns; i++) {
	  column_x_right += column_width[i];
	  if ( fx <= column_x_right) {
	    column = i;
	    break;
	  }
	}
      }
      double row_y_high = t_ll_y;
      for ( int i = 0; i < rows; i++) {
	row_y_high += row_height;
	if ( fy <= row_y_high) {
	  row = i;
	  break;
	}
      }
    }

    if ( row != -1 && column != -1)
      ((GrowCtx *)ctx)->send_table_callback( this, event, fx, fy, column, row);
    break;
  }
  default: ;
  }

  return sts;
}

void GrowTable::update_attributes()
{
  configure();
  configure_scrollbars();

  if ( v_scrollbar)
    v_scrollbar->set_colors( scrollbar_bg_color, scrollbar_color);
  if ( h_scrollbar)
    h_scrollbar->set_colors( scrollbar_bg_color, scrollbar_color);

  if ( header_text_bold)
    header_text_drawtype = glow_eDrawType_TextHelveticaBold;
  else
    header_text_drawtype = glow_eDrawType_TextHelvetica;
}

void GrowTable::set_transform_from_stored( GlowTransform *t)
{
  GrowRect::set_transform_from_stored( t);
  configure_scrollbars();
}

void GrowTable::configure()
{
  table_x0 = table_y0 = 0;
  table_x1 = 0;
  for ( int i = 0; i < columns; i++) {
    if ( header_column && i == 0)
      continue;
    table_x1 += column_width[i];
  }
  table_y1 = row_height * rows;

  if ( header_row)
    y_low_offs = header_row_height;
  else
    y_low_offs = 0;

  if ( header_column)
    x_left_offs = column_width[0];
  else
    x_left_offs = 0;

  value_size = 0;
  for ( int i = 0; i < columns; i++)
    value_size += rows * column_size[i];

  if ( cell_value)
    free(cell_value);

  cell_value = (char *) calloc( 1, value_size);
}

void GrowTable::configure_scrollbars()
{
  double x0, y0, width, height;

  if ( vertical_scrollbar && ! v_scrollbar) {
    x0 = x_right - scrollbar_width;
    y0 = y_low + y_low_offs;
    width = scrollbar_width;
    if ( horizontal_scrollbar)
      height = y_high - (y_low + y_low_offs) - scrollbar_width;
    else
      height = y_high - (y_low + y_low_offs);

    v_scrollbar = new GrowScrollBar( ctx, "vScrollbar", x0, y0, width, height,
				     glow_eDir_Vertical, glow_eDrawType_Line, 1, display_level, 
				     scrollbar_bg_color, scrollbar_color, 1);
    v_scrollbar->register_value_changed_cb( (void *)this, &v_value_changed_cb);
    v_scrollbar->set_value( table_y0 * window_scale, y_high - (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar);
    v_scrollbar->set_range( table_y0 * window_scale, table_y1 * window_scale);
    v_scrollbar->set_shadow( shadow);
    v_value = table_y0 * window_scale;
  }
  else if ( !vertical_scrollbar && v_scrollbar) {
    delete v_scrollbar;
    v_scrollbar = 0;
    v_value = table_y0 * window_scale;
  }
  else if ( v_scrollbar) {
    // Reconfigure range and length
    v_scrollbar->set_value( table_y0 * window_scale, y_high - (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar);
    v_value = table_y0 * window_scale;
    v_scrollbar->set_range( table_y0 * window_scale, table_y1 * window_scale);
    v_scrollbar->set_shadow( shadow);
  }
  else
    v_value = table_y0 * window_scale;

  if ( horizontal_scrollbar && ! h_scrollbar) {
    x0 = x_left + x_left_offs;
    y0 = y_high - scrollbar_width;
    height = scrollbar_width;
    if ( vertical_scrollbar)
      width = x_right - (x_left + x_left_offs) - scrollbar_width;
    else
      width = x_right - (x_left + x_left_offs);

    h_scrollbar = new GrowScrollBar( ctx, "vScrollbar", x0, y0, width, height,
				     glow_eDir_Horizontal, glow_eDrawType_Line, 1, display_level, 
				     scrollbar_bg_color, scrollbar_color, 1);
    h_scrollbar->register_value_changed_cb( (void *)this, &h_value_changed_cb);
    h_scrollbar->set_value( table_x0 * window_scale, x_right - (x_left + x_left_offs) - 
			    scrollbar_width * vertical_scrollbar);
    h_scrollbar->set_range( table_x0 * window_scale, table_x1 * window_scale);
    h_scrollbar->set_shadow( shadow);
    h_value = table_x0 * window_scale;
  }
  else if ( !horizontal_scrollbar && h_scrollbar) {
    delete h_scrollbar;
    h_scrollbar = 0;
    h_value = table_x0 * window_scale;    
  }
  else if ( h_scrollbar) {
    // Reconfigure lenght and range
    h_scrollbar->set_value( table_x0 * window_scale, x_right - (x_left + x_left_offs) - 
			    scrollbar_width * vertical_scrollbar);
    h_value = table_x0 * window_scale;
    h_scrollbar->set_range( table_x0 * window_scale, table_x1 * window_scale);
    h_scrollbar->set_shadow( shadow);
  }
  else
    h_value = table_x0 * window_scale;    
}

void GrowTable::v_value_changed_cb( void *o, double value)
{
  GrowTable *gw = (GrowTable *) o;

  if ( gw->v_value != value) {
    gw->v_value = value;
    gw->draw();
  }
}

void GrowTable::h_value_changed_cb( void *o, double value)
{
  GrowTable *gw = (GrowTable *) o;

  if ( gw->h_value != value) {
    gw->h_value = value;
    gw->draw();
  }
}

void GrowTable::set_textsize( int size) 
{ 
  text_size = size;
  draw();
}

void GrowTable::set_textbold( int bold) 
{ 
  if ( ( bold && draw_type == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && draw_type == glow_eDrawType_TextHelvetica))
    return;

  if ( bold)
    text_drawtype = glow_eDrawType_TextHelveticaBold;
  else
    text_drawtype = glow_eDrawType_TextHelvetica;
  draw();
}

void GrowTable::get_table_info( glow_sTableInfo *info)
{
  info->columns = columns;
  info->rows = rows;
  for ( int i = 0; i < TABLE_MAX_COL; i++)
    info->column_size[i] = column_size[i];
}

void GrowTable::set_table_info( glow_sTableInfo *info)
{
  for ( int i = 0; i < TABLE_MAX_COL; i++)
    column_size[i] = info->column_size[i];

  configure();
}

void GrowTable::set_cell_value( int column, int row, char *value)
{
  int offs = 0;
  for ( int i = 0; i < column; i++)
    offs += rows * column_size[i];
  offs += row * column_size[column];
  strncpy( cell_value + offs, value, column_size[column]);

  draw();
}

void GrowTable::set_selected_cell( int column, int row)
{
  if ( selected_cell_column == column &&
       selected_cell_row == row)
    return;

  if ( column >= columns || row >= rows)
    return;

  selected_cell_column = column;
  selected_cell_row = row;
  draw();
}

int GrowTable::get_selected_cell( int *column, int *row)
{
  if ( selected_cell_row == -1)
    return 0;

  *column = selected_cell_column;
  *row = selected_cell_row;
  return 1;
}

void GrowTable::set_input_focus( int focus)
{
  if ( focus && !input_focus) {
    input_focus = 1;
    draw();

    ctx->register_inputfocus( this, 1);
  }
  else if ( !focus && input_focus) {
    erase();
    input_focus = 0;
    draw();

    ctx->register_inputfocus( this, 0);
  }
}

int GrowTable::make_cell_visible( int column, int row)
{
  int i;
  double cell_x_left, cell_x_right, cell_y_low, cell_y_high;
  double scroll_y, scroll_x;

  if ( column >= columns || row >= rows || column < 0 || row < 0)
    return 0;

  // Calculate the current position of the cell

  // Find out which cell
  double o_ll_x = x_left + x_left_offs;
  double o_ll_y = y_low + y_low_offs;
  double o_ur_x = x_right - vertical_scrollbar * scrollbar_width;
  double o_ur_y = y_high - horizontal_scrollbar * scrollbar_width;

  double t_ll_x = o_ll_x - h_value;
  double t_ll_y = o_ll_y - v_value;

  scroll_x = 0;
  if ( !(header_column && column == 0)) {
    cell_x_left = t_ll_x;
    for ( i = header_column; i < column; i++)
      cell_x_left += column_width[i];
    cell_x_right = cell_x_left + column_width[i+1];

    if ( cell_x_left < o_ll_x)
      scroll_x = cell_x_left - o_ll_x;
    else if ( cell_x_right > o_ur_x) {
      scroll_x = cell_x_right - o_ur_x;
      if ( scroll_x > cell_x_left - o_ll_x)
	scroll_x = cell_x_left - o_ll_x;
    }
  }
  cell_y_low = t_ll_y + row * row_height;
  cell_y_high = cell_y_low + row_height;

  scroll_y = 0;
  int table_size = int((o_ur_y - o_ll_y) / row_height);
  if ( cell_y_low < o_ll_y - row_height/20)
    // scroll_y = cell_y_low - o_ll_y;
    scroll_y = min( cell_y_low - o_ll_y, - int( table_size/3) * row_height);
  else if ( cell_y_high > o_ur_y + row_height/20)
    // scroll_y = cell_y_high - o_ur_y;
    scroll_y = max( cell_y_high - o_ur_y, int( table_size/3) * row_height); 
  
  if ( scroll_x != 0 && horizontal_scrollbar) {
    h_value += scroll_x;
    h_value = h_scrollbar->set_value( h_value);
  }
  if ( scroll_y != 0 && vertical_scrollbar) {
    v_value += scroll_y;
    v_value = v_scrollbar->set_value( v_value);
  }
  if ( (scroll_x != 0 && horizontal_scrollbar) ||
       (scroll_y != 0 && vertical_scrollbar)) {
    draw();
    return 1;
  }
  return 0;
}
