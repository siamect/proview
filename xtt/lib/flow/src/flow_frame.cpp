#include "flow_std.h"


#include <iostream.h>
#include "flow_frame.h"
#include "flow_node.h"
#include "flow_browctx.h"
#include "flow_draw.h"


void FlowFrame::save( ofstream& fp, flow_eSaveMode mode)
{
  // nyi
}

void FlowFrame::open( ifstream& fp)
{
  // nyi
}

void FlowFrame::draw( void *pos, int highlight, int hot, void *node)
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

  if ( ctx->type() == flow_eCtxType_Brow)
    ur_x = ((BrowCtx *)ctx)->frame_x_right;
  else
    ur_x = ((FlowNode *)node)->x_right;
  ur_y = ((FlowNode *)node)->y_high;

  idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_rect( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	ll.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	int( ur_x * ctx->zoom_factor - ll.z_x - ((FlowPoint *)pos)->z_x - 1), 
	int( ur_y * ctx->zoom_factor - ll.z_y - ((FlowPoint *)pos)->z_y - 1), 
	draw_type, idx, highlight);
}

void FlowFrame::erase( void *pos, int hot, void *node)
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
    idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  if ( ctx->type() == flow_eCtxType_Brow)
    ur_x = ((BrowCtx *)ctx)->frame_x_right;
  else
    ur_x = ((FlowNode *)node)->x_right;
  ur_y = ((FlowNode *)node)->y_high;

  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);

  flow_draw_rect_erase( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, ll.z_y + 
	((FlowPoint *)pos)->z_y - ctx->offset_y, 
	int( ur_x * ctx->zoom_factor - ll.z_x - ((FlowPoint *)pos)->z_x - 1), 
	int( ur_y * ctx->zoom_factor - ll.z_y - ((FlowPoint *)pos)->z_y - 1), 
	idx);
}

void FlowFrame::get_borders( double pos_x, double pos_y, double *x_right, 
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

