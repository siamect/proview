#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growaxis.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowAxis::GrowAxis( GlowCtx *glow_ctx, char *name, double x1, double y1, 
		double x2, double y2, glow_eDrawType border_d_type, 
		int line_w, int t_size, glow_eDrawType t_drawtype, 
	        int nodraw) : 
		GrowRect(glow_ctx,name,x1,y1,x2-x1,y2-y1,border_d_type,line_w,
		0,glow_mDisplayLevel_1,0,1,glow_eDrawType_Line,1),
		text_size(t_size), text_drawtype(t_drawtype),
                max_value(100), min_value(0), lines(11),
		longquotient(1),
                valuequotient(1), increment(0)
{
  strcpy( format, "%3.0f");

  configure();
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

GrowAxis::~GrowAxis()
{
  if ( ctx->nodraw) return;
  erase();
  nav_erase();
}

void GrowAxis::configure() 
{
  if ( lines <= 1)
    lines = 2;
  if ( longquotient <= 0)
    longquotient = 1;
  if ( valuequotient <= 0)
    valuequotient = 1;
  increment = (max_value - min_value) / (lines - 1);
}

void GrowAxis::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowAxis) << endl;
  fp << int(glow_eSave_GrowAxis_max_value) << FSPACE << max_value << endl;
  fp << int(glow_eSave_GrowAxis_min_value) << FSPACE << min_value << endl;
  fp << int(glow_eSave_GrowAxis_rect_part) << endl;
  GrowRect::save( fp, mode);
  fp << int(glow_eSave_GrowAxis_lines) << FSPACE << lines << endl;
  fp << int(glow_eSave_GrowAxis_longquotient) << FSPACE << longquotient << endl;
  fp << int(glow_eSave_GrowAxis_valuequotient) << FSPACE << valuequotient << endl;
  fp << int(glow_eSave_GrowAxis_format) << FSPACE << format << endl;
  fp << int(glow_eSave_GrowAxis_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_GrowAxis_text_drawtype) << FSPACE << int(text_drawtype) << endl;
  fp << int(glow_eSave_End) << endl;
}

void GrowAxis::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int           tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowAxis: break;
      case glow_eSave_GrowAxis_max_value: fp >> max_value; break;
      case glow_eSave_GrowAxis_min_value: fp >> min_value; break;
      case glow_eSave_GrowAxis_rect_part: 
        GrowRect::open( fp);
        break;
      case glow_eSave_GrowAxis_lines: fp >> lines; break;
      case glow_eSave_GrowAxis_longquotient: fp >> longquotient; break;
      case glow_eSave_GrowAxis_valuequotient: fp >> valuequotient; break;
      case glow_eSave_GrowAxis_format:
        fp.get();
        fp.getline( format, sizeof(format));
        break;
      case glow_eSave_GrowAxis_text_size: fp >> text_size; break;
      case glow_eSave_GrowAxis_text_drawtype: fp >> tmp; text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowAxis:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  configure();
}

void GrowAxis::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();
  }
}

void GrowAxis::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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
       	obj_ll_y <= *ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();

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

void GrowAxis::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y);

  if ( x_right_pix >= ll_x &&
       x_left_pix <= ur_x &&
       y_high_pix >= ll_y &&
       y_low_pix <= ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
  }
}

void GrowAxis::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowAxis::draw( GlowTransform *t, int highlight, int hot, void *node, 
		      void *colornode)
{
  if ( ctx->nodraw)
    return;
  int i;
  int idx;
  int x, y;
  char text[20];
  int line_length;
  int x_text, y_text;
  int z_height, z_width, z_descent;
  int max_z_width = 0;
  double rotation;
  glow_eDrawType drawtype;
  int text_idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  text_idx = min( text_idx, DRAW_TYPE_SIZE-1);

  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( t, hot, node);
      return;
    }
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_Color59,
		 highlight, (GrowNode *)colornode, 0);


  if ( 45 >= rotation || rotation > 315)
  {
    // Vertical line to the right and values to the left

    glow_draw_line( ctx, ur_x, ll_y, ur_x, ur_y, drawtype, idx, 0);

    // Calculate max value text width
    for ( i = 0; i < lines; i++)
    {
      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);
        if ( max_z_width < z_width)
          max_z_width = z_width;
      }
    }
    x_text = ll_x + max_z_width;
    line_length = ur_x - ll_x - max_z_width;
    if ( line_length < 3)
      line_length = 3;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_line( ctx, ur_x - line_length, y, 
          ur_x, y, drawtype, idx, 0);
      else
        glow_draw_line( ctx, ur_x -  int( 2.0 / 3 * line_length), y, 
          ur_x, y, drawtype, idx, 0);
      sprintf( text, format, max_value - i * increment);

      if ( text_idx >= 0 && max_z_width < ur_x - ll_x &&
           i % valuequotient == 0) {
        if ( i == lines - 1)
          y_text = y;
        else if ( i == 0)
          y_text = y + z_height - z_descent - 3;
        else
          y_text = y + (z_height-z_descent)/2;
        glow_draw_text( ctx, ll_x, y_text,
		    text, strlen(text), text_drawtype, text_idx, highlight, 0);
      }
    }
  }
  else if ( 45 < rotation && rotation <= 135)  
  {
    // Horizontal line at bottom and values to the top

    glow_draw_line( ctx, ll_x, ur_y, ur_x, ur_y, drawtype, idx, 0);

    // Calculate max value text height
    draw_get_text_extent( ctx, "0", 1, text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);

    line_length = ur_y - ll_y - z_height;
    if ( line_length < 3)
      line_length = 3;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * (lines - 1- i));
      if ( i % longquotient == 0)
        glow_draw_line( ctx, x, ur_y - line_length, x, 
          ur_y, drawtype, idx, 0);
      else
        glow_draw_line( ctx, x, ur_y -  int( 2.0 / 3 * line_length), x, 
          ur_y, drawtype, idx, 0);

      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);

        if ( text_idx >= 0 && z_height < ur_y - ll_y ) {
          if ( i == lines - 1)
            x_text = x;
          else if ( i == 0)
            x_text = x - z_width;
          else
            x_text = x - (z_width)/2;
          glow_draw_text( ctx, x_text, ll_y + z_height - z_descent,
		    text, strlen(text), text_drawtype, text_idx, highlight, 0);
        }
      }
    }
  }
  else if ( 135 < rotation && rotation <= 225)
  {
    // Vertical line to the left and values to the right

    glow_draw_line( ctx, ll_x, ll_y, ll_x, ur_y, drawtype, idx, 0);

    // Calculate max value text width
    for ( i = 0; i < lines; i++)
    {
      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);
        if ( max_z_width < z_width)
          max_z_width = z_width;
      }
    }
    x_text = ur_x - max_z_width;
    line_length = ur_x - ll_x - max_z_width;
    if ( line_length < 3)
      line_length = 3;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * ( lines - 1 - i));
      if ( i % longquotient == 0)
        glow_draw_line( ctx, ll_x, y, 
          ll_x + line_length, y, drawtype, idx, 0);
      else
        glow_draw_line( ctx, ll_x, y, 
          ll_x + int( 2.0 / 3 * line_length), y, drawtype, idx, 0);
      sprintf( text, format, max_value - i * increment);

      if ( text_idx >= 0 && max_z_width < ur_x - ll_x &&
           i % valuequotient == 0) {
        if ( i == lines - 1)
          y_text = y + z_height - z_descent - 3;
        else if ( i == 0)
          y_text = y;
        else
          y_text = y + (z_height-z_descent)/2;
        glow_draw_text( ctx, x_text, y_text,
		    text, strlen(text), text_drawtype, text_idx, highlight, 0);
      }
    }
  }
  else // if ( 225 < rotation && rotation <= 315)
  {
    // Horizontal line at top and values at the bottom

    glow_draw_line( ctx, ll_x, ll_y, ur_x, ll_y, drawtype, idx, 0);

    // Calculate max value text height
    draw_get_text_extent( ctx, "0", 1, text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);

    line_length = ur_y - ll_y - (z_height - z_descent);
    if ( line_length < 3)
      line_length = 3;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_line( ctx, x, ll_y, x, 
          ll_y + line_length, drawtype, idx, 0);
      else
        glow_draw_line( ctx, x, ll_y, x, 
          ll_y  +  int( 2.0 / 3 * line_length), drawtype, idx, 0);
      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);

        if ( text_idx >= 0 && z_height - z_descent < ur_y - ll_y) {
          if ( i == lines - 1)
            x_text = x - z_width;
          else if ( i == 0)
            x_text = x;
          else
            x_text = x - (z_width)/2;
          glow_draw_text( ctx, x_text, ur_y,
		    text, strlen(text), text_drawtype, text_idx, highlight, 0);
        }
      }
    }
  }

}


void GrowAxis::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  int i;
  glow_eDrawType drawtype;
  double rotation;
  int idx;
  int x, y;
  int line_length;

  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_Color59,
		 0, (GrowNode *)colornode, 0);

  if ( 45 >= rotation || rotation > 315)
  {
    // Vertical line to the right and values to the left

    glow_draw_nav_line( ctx, ur_x, ll_y, ur_x, ur_y, drawtype, idx, 0);

    line_length = ur_x - ll_x;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_nav_line( ctx, ur_x - line_length, y, 
          ur_x, y, drawtype, idx, 0);
      else
        glow_draw_nav_line( ctx, ur_x -  int( 2.0 / 3 * line_length), y, 
          ur_x, y, drawtype, idx, 0);
    }
  }
  else if ( 45 < rotation && rotation <= 135)  
  {
    // Horizontal line at bottom and values to the top

    glow_draw_nav_line( ctx, ll_x, ur_y, ur_x, ur_y, drawtype, idx, 0);

    line_length = ur_y - ll_y;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * (lines - 1- i));
      if ( i % longquotient == 0)
        glow_draw_nav_line( ctx, x, ur_y - line_length, x, 
          ur_y, drawtype, idx, 0);
      else
        glow_draw_nav_line( ctx, x, ur_y -  int( 2.0 / 3 * line_length), x, 
          ur_y, drawtype, idx, 0);
    }
  }
  else if ( 135 < rotation && rotation <= 225)
  {
    // Vertical line to the left and values to the right

    glow_draw_nav_line( ctx, ll_x, ll_y, ll_x, ur_y, drawtype, idx, 0);

    line_length = ur_x - ll_x;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * ( lines - 1 - i));
      if ( i % longquotient == 0)
        glow_draw_nav_line( ctx, ll_x, y, 
          ll_x + line_length, y, drawtype, idx, 0);
      else
        glow_draw_nav_line( ctx, ll_x, y, 
          ll_x + int( 2.0 / 3 * line_length), y, drawtype, idx, 0);
    }
  }
  else // if ( 225 < rotation && rotation <= 315)
  {
    // Horizontal line at top and values at the bottom

    glow_draw_nav_line( ctx, ll_x, ll_y, ur_x, ll_y, drawtype, idx, 0);

    line_length = ur_y - ll_y;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_nav_line( ctx, x, ll_y, x, 
          ll_y + line_length, drawtype, idx, 0);
      else
        glow_draw_nav_line( ctx, x, ll_y, x, 
          ll_y  +  int( 2.0 / 3 * line_length), drawtype, idx, 0);
    }
  }
}

void GrowAxis::erase( GlowTransform *t, int hot, void *node)
{
  GrowRect::erase( t, hot, node);
}

void GrowAxis::nav_erase( GlowTransform *t, void *node)
{
  int i;
  double rotation;
  int idx;
  int x, y;
  int line_length;

  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);


  if ( 45 >= rotation || rotation > 315)
  {
    // Vertical line to the right and values to the left

    glow_draw_nav_line_erase( ctx, ur_x, ll_y, ur_x, ur_y, idx);

    line_length = ur_x - ll_x;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_nav_line_erase( ctx, ur_x - line_length, y, 
          ur_x, y, idx);
      else
        glow_draw_nav_line_erase( ctx, ur_x -  int( 2.0 / 3 * line_length), y, 
          ur_x, y, idx);
    }
  }
  else if ( 45 < rotation && rotation <= 135)  
  {
    // Horizontal line at bottom and values to the top

    glow_draw_nav_line_erase( ctx, ll_x, ur_y, ur_x, ur_y, idx);

    line_length = ur_y - ll_y;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * (lines - 1- i));
      if ( i % longquotient == 0)
        glow_draw_nav_line_erase( ctx, x, ur_y - line_length, x, 
          ur_y, idx);
      else
        glow_draw_nav_line_erase( ctx, x, ur_y -  int( 2.0 / 3 * line_length), x, 
          ur_y, idx);
    }
  }
  else if ( 135 < rotation && rotation <= 225)
  {
    // Vertical line to the left and values to the right

    glow_draw_nav_line_erase( ctx, ll_x, ll_y, ll_x, ur_y, idx);

    line_length = ur_x - ll_x;

    for ( i = 0; i < lines; i++)
    {
      y = int( ll_y + double(ur_y - ll_y) / (lines - 1) * ( lines - 1 - i));
      if ( i % longquotient == 0)
        glow_draw_nav_line_erase( ctx, ll_x, y, 
          ll_x + line_length, y, idx);
      else
        glow_draw_nav_line_erase( ctx, ll_x, y, 
          ll_x + int( 2.0 / 3 * line_length), y, idx);
    }
  }
  else // if ( 225 < rotation && rotation <= 315)
  {
    // Horizontal line at top and values at the bottom

    glow_draw_nav_line_erase( ctx, ll_x, ll_y, ur_x, ll_y, idx);

    line_length = ur_y - ll_y;

    for ( i = 0; i < lines; i++)
    {
      x = int( ll_x + double(ur_x - ll_x) / (lines - 1) * i);
      if ( i % longquotient == 0)
        glow_draw_nav_line_erase( ctx, x, ll_y, x, 
          ll_y + line_length, idx);
      else
        glow_draw_nav_line_erase( ctx, x, ll_y, x, 
          ll_y  +  int( 2.0 / 3 * line_length), idx);
    }
  }
}

void GrowAxis::draw()
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

void GrowAxis::align( double x, double y, glow_eAlignDirection direction)
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

void GrowAxis::set_textsize( int size) 
{ 
  erase();
  nav_erase();
  text_size = size;
  get_node_borders();
  draw();
}

void GrowAxis::set_textbold( int bold) 
{ 
  if ( ( bold && text_drawtype == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && text_drawtype == glow_eDrawType_TextHelvetica))
    return;

  erase();
  nav_erase();
  if ( bold)
    text_drawtype = glow_eDrawType_TextHelveticaBold;
  else
    text_drawtype = glow_eDrawType_TextHelvetica;
  get_node_borders();
  draw();
}

void GrowAxis::set_range( double min, double max)
{
  max_value = max;
  min_value = min;
  configure();
}

void GrowAxis::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int i;
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rotation;
  int bold;
  char text[20];
  int line_length;
  int z_height, z_width, z_descent;
  int max_z_width = 0;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  bold = (text_drawtype == glow_eDrawType_TextHelveticaBold);

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

  if ( t)
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  else
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  // Calculate max value line width
  if ( 45 >= rotation || rotation > 315)
  {
    for ( i = 0; i < lines; i++)
    {
      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, idx), &z_width, &z_height, &z_descent);
        if ( max_z_width < z_width)
          max_z_width = z_width;
      }
    }
    line_length = int(ur_x - ll_x) - max_z_width;
  }
  else if ( 45 < rotation && rotation <= 135)  
  {
    draw_get_text_extent( ctx, "0", 1, text_drawtype, 
                max( 0, idx), &z_width, &z_height, &z_descent);

    line_length = int(ur_y - ll_y) - (z_height - z_descent);
  }
  else if ( 135 < rotation && rotation <= 225)
  {
    for ( i = 0; i < lines; i++)
    {
      if ( i % valuequotient == 0) {
        sprintf( text, format, max_value - i * increment);
        draw_get_text_extent( ctx, text, strlen(text), text_drawtype, 
                max( 0, idx), &z_width, &z_height, &z_descent);
        if ( max_z_width < z_width)
          max_z_width = z_width;
      }
    }
    line_length = int(ur_x - ll_x) - max_z_width;
  }
  else // if ( 225 < rotation && rotation <= 315)
  {
    draw_get_text_extent( ctx, "0", 1, text_drawtype, 
                max( 0, idx), &z_width, &z_height, &z_descent);

    line_length = int(ur_y - ll_y) - (z_height - z_descent);
  }
  if ( line_length < 3)
    line_length = 3;


  ((GrowCtx *)ctx)->export_jbean->axis( ll_x, ll_y, ur_x, ur_y,
     draw_type, min_value, max_value, lines, longquotient, valuequotient,
     line_length, line_width, rotation, bold, idx, format,
     pass, shape_cnt, node_cnt, fp);
}

void GrowAxis::set_conf( double max_val, double min_val, int no_of_lines, 
     int long_quot, int value_quot, double rot, char *value_format)
{
  max_value = max_val;
  min_value = min_val;
  lines = no_of_lines;
  longquotient = long_quot;
  valuequotient = value_quot;
  trf.rotation = rot;
  if ( format)
    strcpy( format, value_format);

  configure();
}



