#include "glow_std.h"


#include <iostream.h>
#include <math.h>

#include "glow_annot.h"
#include "glow_node.h"
#include "glow_draw.h"
#include "glow_browctx.h"

void GlowAnnot::zoom()
{
  p.zoom();
}

void GlowAnnot::nav_zoom()
{
  p.nav_zoom();
}

void GlowAnnot::print_zoom()
{
  p.print_zoom();
}

void GlowAnnot::traverse( int x, int y)
{
  p.traverse( x, y);
}

void GlowAnnot::print( void *pos, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((GlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);
  double x;

  if ( size <= 0)
    return;

  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
      {
        x = ((GlowPoint *)pos)->print_z_x + max( p.print_z_x,
           ((GlowNode *)node)->rel_annot_x[number] * ctx->print_zoom_factor);
      }
      else
        x = p.print_z_x + ((GlowPoint *)pos)->print_z_x;
      ctx->print_ps->text( x, 
	p.print_z_y + ((GlowPoint *)pos)->print_z_y, 
	((GlowNode *) node)->annotv[number], 
	strlen(((GlowNode *) node)->annotv[number]), draw_type, size);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int z_width, z_height, z_descent;
      int z_x = int( p.print_z_x + ((GlowPoint *)pos)->print_z_x);
      int z_y = int( p.print_z_y + ((GlowPoint *)pos)->print_z_y);
      int len = 0;
      int line_cnt = 0;
      char *line = ((GlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, text_size, &z_width,
	 &z_height, &z_descent);
      z_height = int( ctx->print_zoom_factor / ctx->base_zoom_factor * z_height);
      for ( s = ((GlowNode *) node)->annotv[number]; *s; s++)
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

void GlowAnnot::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Annot) << endl;
  fp << int(glow_eSave_Annot_number) << FSPACE << number << endl;
  fp << int(glow_eSave_Annot_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Annot_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_Annot_display_level) << FSPACE << int(display_level) << endl;
  fp << int(glow_eSave_Annot_p) << endl;
  p.save( fp, mode);
  fp << int(glow_eSave_Annot_annot_type) << FSPACE << int(annot_type) << endl;
  fp << int(glow_eSave_End) << endl;
}

void GlowAnnot::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Annot: break;
      case glow_eSave_Annot_number: fp >> number; break;
      case glow_eSave_Annot_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Annot_text_size: fp >> text_size; break;
      case glow_eSave_Annot_display_level: fp >> tmp; display_level = (glow_mDisplayLevel)tmp; break;
      case glow_eSave_Annot_p: p.open( fp); break;
      case glow_eSave_Annot_annot_type: fp >> tmp; 
	annot_type = (glow_eAnnotType)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowAnnot:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowAnnot::draw( void *pos, int highlight, int hot, void *node)
{
  int x;

  if ( !(display_level & ctx->display_level))
    return;

  if ( !((GlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
      {
        x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor_x));
      }
      else
        x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
      glow_draw_text( ctx, x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	((GlowNode *) node)->annotv[number], 
	strlen(((GlowNode *) node)->annotv[number]), draw_type, idx, 
	highlight, 0);
      if ( ((GlowNode *) node)->annotv_inputmode[number])
        glow_draw_move_input( ctx, 
	  ((GlowNode *) node)->annotv_input[number],
	  x, p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y,
	  glow_ePosition_Absolute);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int z_width, z_height, z_descent;
      int z_x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
      int z_y = p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((GlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height, &z_descent);
      for ( s = ((GlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            glow_draw_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        glow_draw_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
      break;
    }
  }
}

void GlowAnnot::erase( void *pos, int hot, void *node)
{
  int x;

  if ( !(display_level & ctx->display_level))
    return;

  if ( !((GlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
      {
        x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor_x));
      }
      else
        x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
      glow_draw_text_erase( ctx, x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	((GlowNode *) node)->annotv[number], 
	strlen(((GlowNode *) node)->annotv[number]), draw_type, idx, 0);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int z_width, z_height, z_descent;
      int z_x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
      int z_y = p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((GlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height, &z_descent);
      for ( s = ((GlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            glow_draw_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        glow_draw_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
      break;
    }
  }
}

void GlowAnnot::nav_draw( void *pos, int highlight, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((GlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      glow_draw_nav_text( ctx, 
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	((GlowNode *) node)->annotv[number], 
	strlen(((GlowNode *) node)->annotv[number]), draw_type, idx, 
	highlight, 0);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int z_width, z_height, z_descent;
      int z_x = p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x;
      int z_y = p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((GlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height, &z_descent);
      for ( s = ((GlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            glow_draw_nav_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        glow_draw_nav_text( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, highlight, 0);
      break;
    }
  }
}

void GlowAnnot::nav_erase( void *pos, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( !((GlowNode *) node)->annotv[number])
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      glow_draw_nav_text_erase( ctx,
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	((GlowNode *) node)->annotv[number], 
	strlen(((GlowNode *) node)->annotv[number]), draw_type, idx, 0);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int z_width, z_height, z_descent;
      int z_x = p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x;
      int z_y = p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y;
      int len = 0;
      int line_cnt = 0;
      char *line = ((GlowNode *) node)->annotv[number];
      char *s;
      draw_get_text_extent( ctx, "", 0, draw_type, idx, &z_width, &z_height, &z_descent);
      for ( s = ((GlowNode *) node)->annotv[number]; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
            glow_draw_nav_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
        glow_draw_nav_text_erase( ctx, z_x, z_y + line_cnt * z_height, line, 
	  	len, draw_type, idx, 0);
      break;
    }
  }
}

int GlowAnnot::event_handler( void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
/***
  if ( p1.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x < x && 
       x < p2.z_x  + ((GlowPoint *)pos)->z_x - ctx->offset_x &&
       p1.z_y  + ((GlowPoint *)pos)->z_y - ctx->offset_y < y && 
       y < p2.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y)
  {
    cout << "Event handler: Hit in text" << endl;
    return 1;
  }  
  else
***/
    return 0;
}

void GlowAnnot::get_borders( double pos_x, double pos_y,
	double *x_right, double *x_left, double *y_high, double *y_low, 
	void *node)
{
  if ( !((GlowNode *) node)->annotv[number])
    return;
  if ( display_level != glow_mDisplayLevel_1)
    return;
/*
  if ( pos_x + p1.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p2.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p1.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_x + p2.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_y + p1.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p2.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p1.y > *y_high)
    *y_high = pos_y + p2.y;
  if ( pos_y + p2.y > *y_high)
    *y_high = pos_y + p2.y;
*/
}

void GlowAnnot::measure_annot( char *text, double *width, double *height)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int	z_width, z_height, z_descent;

  if ( idx < 0)
  {
    *width = 0;
    *height = 0;
    return;
  }
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      draw_get_text_extent( ctx, text, strlen(text), draw_type, idx, 
		&z_width, &z_height, &z_descent);
      break;
    case glow_eAnnotType_MultiLine:
    {
      int l_width, l_height, l_descent;
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
		&l_width, &l_height, &l_descent);
	    z_width = max( z_width, l_width);
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
		&l_width, &l_height, &l_descent);
	z_width = max( z_width, l_width);
        line_cnt++;
      }
      z_height = line_cnt * l_height;      
      break;
    }
  }
  *width = z_width / ctx->zoom_factor_y;
  *height = z_height / ctx->zoom_factor_y;
}

void GlowAnnot::configure_annotations( void *pos, void *node)
{
  double width, height;

  if ( !((GlowNode *) node)->annotv[number])
    return;

  if ( relative_pos)
    ((GlowNode *)node)->rel_annot_x[number] = 
		((GlowNode *)node)->relative_annot_x;
  measure_annot( ((GlowNode *) node)->annotv[number], &width, &height);
  ((GlowNode *)node)->relative_annot_x = 
	max( p.x, ((GlowNode *)node)->rel_annot_x[number]) + 
	width + ((BrowCtx *)ctx)->annotation_space;
}

void GlowAnnot::open_annotation_input( void *pos, void *node)
{

  if ( !((GlowNode *) node)->annotv[number])
    return;

  int x, width_pix, height_pix;
  double width, height;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      measure_annot( ((GlowNode *) node)->annotv[number], &width, &height);
      width_pix = int( width * ctx->zoom_factor_y);
      height_pix = int( height * ctx->zoom_factor_y);
      if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
      {
        x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annot_x[number] * ctx->zoom_factor_x));
      }
      else
        x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
      glow_draw_create_input( ctx, x,
		p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y,
		((GlowNode *) node)->annotv[number],
		strlen(((GlowNode *) node)->annotv[number]), idx, width_pix, 
		height_pix, node, number, 
		&((GlowNode *) node)->annotv_input[number]);
      break;
    case glow_eAnnotType_MultiLine:
        // Not yet implemented...
      break;
  }
}

void GlowAnnot::close_annotation_input( void *node)
{

  if ( !((GlowNode *) node)->annotv_inputmode[number])
    return;

  glow_draw_close_input( ctx, ((GlowNode *) node)->annotv_input[number]);
}

int GlowAnnot::get_annotation_input( void *node, char **text)
{

  if ( !((GlowNode *) node)->annotv_inputmode[number])
    return 0;

  return glow_draw_get_input( ctx, ((GlowNode *) node)->annotv_input[number], 
	text);
}

void GlowAnnot::move_widgets( void *node, int x, int y)
{
  if ( !((GlowNode *) node)->annotv_inputmode[number])
    return;

  glow_draw_move_input( ctx,  ((GlowNode *) node)->annotv_input[number], 
	x, y, glow_ePosition_Relative);
}

ostream& operator<< ( ostream& o, const GlowAnnot a)
{
  o << '(' << a.p.x << ',' << a.p.y << ')'; 
  return o;
}

void glow_measure_annot_text( GlowCtx *ctx, char *text, glow_eDrawType draw_type, 
		int text_size, glow_eAnnotType annot_type, 
		double *width, double *height, int *rows)
{
  int	z_width, z_height, z_descent;

  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      draw_get_text_extent( ctx, text, strlen(text), draw_type, text_size, 
		&z_width, &z_height, &z_descent);
      *rows = 1;
      break;
    case glow_eAnnotType_MultiLine:
    {
      int l_width, l_height, l_descent;
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
		&l_width, &l_height, &l_descent);
	    z_width = max( z_width, l_width);
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
		&l_width, &l_height, &l_descent);
	z_width = max( z_width, l_width);
        line_cnt++;
      }
      z_height = line_cnt * l_height;      
      *rows = max( line_cnt, 1);
      break;
    }
  }
  *width = 1.0 * z_width / ctx->base_zoom_factor;
  *height = 1.0 * z_height / ctx->base_zoom_factor;
}

