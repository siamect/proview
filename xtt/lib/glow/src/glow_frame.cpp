#include "glow_std.h"


#include <stdlib.h>
#include <iostream.h>
#include "glow_frame.h"
#include "glow_node.h"
#include "glow_browctx.h"
#include "glow_draw.h"


void GlowFrame::save( ofstream& fp, glow_eSaveMode mode)
{
  // nyi
}

void GlowFrame::open( ifstream& fp)
{
  // nyi
}

void GlowFrame::draw( void *pos, int highlight, int hot, void *node)
{
  double ur_x, ur_y;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( pos, 1, node);
      return;
    }
  }

  if ( ctx->type() == glow_eCtxType_Brow)
    ur_x = ((BrowCtx *)ctx)->frame_x_right;
  else
    ur_x = ((GlowNode *)node)->x_right;
  ur_y = ((GlowNode *)node)->y_high;

  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_rect( ctx, ll.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	ll.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	int( ur_x * ctx->zoom_factor_x - ll.z_x - ((GlowPoint *)pos)->z_x - 1), 
	int( ur_y * ctx->zoom_factor_y - ll.z_y - ((GlowPoint *)pos)->z_y - 1), 
	draw_type, idx, highlight);
}

void GlowFrame::erase( void *pos, int hot, void *node)
{
  double ur_x, ur_y;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0) return;
  }
  else
  {
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  if ( ctx->type() == glow_eCtxType_Brow)
    ur_x = ((BrowCtx *)ctx)->frame_x_right;
  else
    ur_x = ((GlowNode *)node)->x_right;
  ur_y = ((GlowNode *)node)->y_high;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  glow_draw_rect_erase( ctx, ll.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, ll.z_y + 
	((GlowPoint *)pos)->z_y - ctx->offset_y, 
	int( ur_x * ctx->zoom_factor_x - ll.z_x - ((GlowPoint *)pos)->z_x - 1), 
	int( ur_y * ctx->zoom_factor_y - ll.z_y - ((GlowPoint *)pos)->z_y - 1), 
	idx);
}

void GlowFrame::get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
{
  if ( pos_x + ll.x < *x_left)
    *x_left = pos_x + ll.x;
  if ( pos_x + ur.x > *x_right)
    *x_right = pos_x + ur.x;
  if ( pos_y + ll.y < *y_low)
    *y_low = pos_y + ll.y;
  if ( pos_y + ur.y > *y_high)
    *y_high = pos_y + ur.y;
}

