#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growcurve.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowCurve::GrowCurve( GlowCtx *glow_ctx, char *name, glow_sCurveData *data,
                double x, double y,
		double w, double h, glow_eDrawType border_d_type, int line_w,
		glow_mDisplayLevel display_lev,
		int fill_rect, int display_border, 
		glow_eDrawType fill_d_type, int nodraw) : 
                GrowTrend(glow_ctx,name,x,y,w,h,border_d_type,line_w,
		display_lev, fill_rect, display_border, fill_d_type, 1)
{
  if ( data)
    configure_curves( data);
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

void GrowCurve::configure_curves( glow_sCurveData *data)
{
  glow_eDrawType dt, dt_fill;
  int points;
  glow_sPoint *pointarray;
  glow_sPoint *point_p;
  int	i, idx;
  double y_value, x_value;

  // Remove old curves
  ctx->nodraw++;
  for ( i = 0; i < curve_cnt; i++) {
    if ( curve[i])
      delete curve[i];
  }
  ctx->nodraw--;



  curve_cnt = data->curves - 1;
  no_of_points = data->rows;

  for ( i = 0; i < curve_cnt; i++) {
    max_value[i] = data->max_value[i+1];
    min_value[i] = data->min_value[i+1];
    curve_drawtype[i] = data->color[i+1];
    curve_fill_drawtype[i] = data->fillcolor[i+1];
  }

  points = no_of_points;
  if ( fill_curve)
    points += 2;
  curve_width = min( DRAW_TYPE_SIZE, max( 1, curve_width));

  pointarray = (glow_sPoint *) calloc( points, sizeof(glow_sPoint));
  for ( idx = 0; idx < curve_cnt; idx++) {
    point_p = pointarray;
    for ( i = 0; i < points; i++)
    {
      if ( !fill_curve) {
        if ( data->x_reverse)
          x_value = ur.x - (data->data[0][i] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
        else
          x_value = ll.x + (data->data[0][i] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);

        x_value = max( ll.x, min( x_value, ur.x));

        y_value = ur.y - (data->data[idx+1][i] - min_value[idx]) / 
	        (max_value[idx] - min_value[idx]) * (ur.y - ll.y);

        y_value = max( ll.y, min( y_value, ur.y));

        point_p->y = y_value;
        point_p->x = x_value;
      }
      else
      {
        if ( i == 0) {
          point_p->y = ur.y;
          // point_p->x = ur.x;
          if ( data->x_reverse)
            point_p->x = ur.x - (data->data[0][i] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
          else
            point_p->x = ll.x + (data->data[0][i] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
        }
        else if ( i == points - 1) {
          point_p->y = ur.y;
          if ( data->x_reverse)
            point_p->x = ur.x - (data->data[0][i-2] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
          else
            point_p->x = ll.x + (data->data[0][i-2] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
          // point_p->x = ll.x;
        }
        else {
          if ( data->x_reverse)
            x_value = ur.x - (data->data[0][i-1] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);
          else
            x_value = ll.x + (data->data[0][i-1] - data->min_value[0]) / 
	        (data->max_value[0] - data->min_value[0]) * (ur.x - ll.x);

          x_value = max( ll.x, min( x_value, ur.x));

          y_value = ur.y - (data->data[idx+1][i-1] - min_value[idx]) / 
	        (max_value[idx] - min_value[idx]) * (ur.y - ll.y);

          y_value = max( ll.y, min( y_value, ur.y));

          point_p->y = y_value;
          point_p->x = x_value;
        }
      }
      point_p++;
    }

    if ( curve_drawtype[idx] != glow_eDrawType_Inherit)
      dt = curve_drawtype[idx];
    else
      dt = draw_type;

    if ( curve_fill_drawtype[idx] != glow_eDrawType_Inherit)
      dt_fill = curve_fill_drawtype[idx];
    else
      dt_fill = draw_type;

    ctx->nodraw++;
    curve[idx] = new GrowPolyLine( ctx, "", pointarray, points, dt, 
	curve_width,
	0, fill_curve, 1, 0, dt_fill);
    ctx->nodraw--;
  }
  free( (char *) pointarray);

  draw();
}


void GrowCurve::add_points( glow_sCurveData *data)
{
  int	idx;
  double y_value;

  // Remove old curves
  ctx->nodraw++;
  for ( idx = 0; idx < curve_cnt; idx++) {
    y_value = ur.y - (data->data[idx+1][0] - min_value[idx]) / 
	        (max_value[idx] - min_value[idx]) * (ur.y - ll.y);

    y_value = max( ll.y, min( y_value, ur.y));

    if ( !fill)
      erase();
    if ( !fill_curve)
      curve[idx]->add_and_shift_y_value( y_value);
    else
      curve[idx]->add_and_shift_y_value_filled( y_value);
  }
  ctx->nodraw--;

  draw();
  // draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  ctx->nav_draw( 0, 0, ctx->nav_window_width, ctx->nav_window_height);
  // nav_draw( (GlowTransform *) NULL, highlight, NULL, NULL);
}






