#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_node.h"
#include "glow_draw.h"
#include "glow_conpoint.h"
#include "glow_browctx.h"
#include "glow_growctx.h"
#include "glow_tracedata.h"
#include "glow_grownode.h"

GlowNode::GlowNode( GlowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw, int rel_annot_pos) : 
	x_right(x1), x_left(x1), y_high(y1), y_low(y1), 
	obst_x_right(x1), obst_x_left(x1), obst_y_high(y1), obst_y_low(y1),
	hot(0), ctx(glow_ctx), nc(node_class), nc_root(node_class), 
	pos(glow_ctx, x1,y1), stored_pos(glow_ctx, x1, y1),
	highlight(0), inverse(0), level(0), node_open(0),
	relative_annot_pos(rel_annot_pos), relative_annot_x(0)
{
  double x_grid, y_grid;
  strncpy( n_name, name, sizeof(n_name));
  n_name[sizeof(n_name)-1] = 0;
  memset( refcon_cnt, 0, sizeof(refcon_cnt));
  memset( annotv, 0, sizeof(annotv));
  memset( annotpixmapv, 0, sizeof(annotpixmapv));
  memset( annotsize, 0, sizeof(annotsize));
  memset( annotv_inputmode, 0, sizeof(annotv_inputmode));
  memset( annotv_input, 0, sizeof(annotv_input));
  memset( rbuttonv, 0, sizeof(rbuttonv));
  memset( rel_annot_x, 0, sizeof(rel_annot_x));
  memset( rel_annotpixmap_x, 0, sizeof(rel_annotpixmap_x));
  if ( !nc)
    return;
  if ( ctx->grid_on)
  {
    ctx->find_grid( x1, y1, &x_grid, &y_grid);
    pos.posit( x_grid, y_grid);
  }
  x_left = y_low = 1e37;
  x_right = y_high = -1e37;
  get_node_borders();
  if ( nc->group == glow_eNodeGroup_Document)
  {
    obst_x_left = obst_y_low = 1e37;
    obst_x_right = obst_y_high = -1e37;
    get_node_obstacle_borders();
  }
  else
  {
    obst_x_left = x_left;
    obst_x_right = x_right;
    obst_y_low = y_low;
    obst_y_high = y_high;
  }
  zoom();
  if ( !nodraw && ctx->type() != glow_eCtxType_Grow)
    draw(  0, 0, ctx->window_width, ctx->window_height);
}

void GlowNode::copy_from( const GlowNode& n) 
{
  memcpy( this, &n, sizeof(n));
  for ( int i = 0; i < 10; i++)
  {
    if ( annotsize[i])
    {
      annotv[i] = (char *) calloc( 1, annotsize[i]);
      memcpy( annotv[i], n.annotv[i], annotsize[i]);
    }
  }
}

GlowNode::~GlowNode()
{
  if ( !nc)
    return;
  for ( int i = 0; i < 10; i++)
  {
    if ( annotv_inputmode[i])
      close_annotation_input( i);
  }
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;
  if ( ctx->type() == glow_eCtxType_Grow)
    return;

  erase();
  nav_erase();

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ctx->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  ctx->redraw_defered();
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

int GlowNode::get_conpoint( int num, double *x, double *y, glow_eDirection *dir)
{
  int sts;

  sts = nc->get_conpoint( num, x, y, dir);
  if ( ODD(sts))
  {
    *x += pos.x;
    *y += pos.y;
  }
  return sts;
}

void GlowNode::move( int delta_x, int delta_y, int grid)
{
  double x, y, x_grid, y_grid;

  if ( grid)
  {
    /* Move to closest grid point */
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor_x;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor_y;
    ctx->find_grid( x, y, &x_grid, &y_grid);
    if ( fabs( x_grid - pos.x) < DBL_EPSILON &&
         fabs( y_grid - pos.y) < DBL_EPSILON)
      return;
    erase();
    nav_erase();
    pos.posit( x_grid, y_grid);
    x_left = y_low = 1e37;
    x_right = y_high = -1e37;
    get_node_borders();
    if ( nc->group == glow_eNodeGroup_Document)
    {
      obst_x_left = obst_y_low = 1e37;
      obst_x_right = obst_y_high = -1e37;
      get_node_obstacle_borders();
    }
    else
    {
      obst_x_left = x_left;
      obst_x_right = x_right;
      obst_y_low = y_low;
      obst_y_high = y_high;
    }

/*
    x_right += (x_grid - pos.x);
    x_left += (x_grid - pos.x);
    y_high += (y_grid - pos.y);
    y_low += (y_grid - pos.y);
    obst_x_right += (x_grid - pos.x);
    obst_x_left += (x_grid - pos.x);
    obst_y_high += (y_grid - pos.y);
    obst_y_low += (y_grid - pos.y);
*/
  }
  else
  {
    erase();
    nav_erase();
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor_x;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor_y;
    pos.posit( x, y);
    x_right += 1.0 * delta_x / ctx->zoom_factor_x;
    x_left += 1.0 * delta_x / ctx->zoom_factor_x;
    y_high += 1.0 * delta_y / ctx->zoom_factor_y;
    y_low += 1.0 * delta_y / ctx->zoom_factor_y;
    obst_x_right += 1.0 * delta_x / ctx->zoom_factor_x;
    obst_x_left += 1.0 * delta_x / ctx->zoom_factor_x;
    obst_y_high += 1.0 * delta_y / ctx->zoom_factor_y;
    obst_y_low += 1.0 * delta_y / ctx->zoom_factor_y;
  }
//  ctx->set_defered_redraw();
//  ctx->redraw_node_cons( this);
  ctx->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
//  ctx->redraw_defered();

}

void GlowNode::move_noerase( int delta_x, int delta_y, int grid)
{
  double x, y, x_grid, y_grid;

  if ( grid)
  {
    /* Move to closest grid point */
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor_x;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor_y;
    ctx->find_grid( x, y, &x_grid, &y_grid);
    if ( fabs( x_grid - pos.x) < DBL_EPSILON &&
         fabs( y_grid - pos.y) < DBL_EPSILON)
      return;
    x_right += (x_grid - pos.x);
    x_left += (x_grid - pos.x);
    y_high += (y_grid - pos.y);
    y_low += (y_grid - pos.y);
    obst_x_right += (x_grid - pos.x);
    obst_x_left += (x_grid - pos.x);
    obst_y_high += (y_grid - pos.y);
    obst_y_low += (y_grid - pos.y);
    pos.posit( x_grid, y_grid);
  }
  else
  {
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor_x;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor_y;
    pos.posit( x, y);
    x_right += 1.0 * delta_x / ctx->zoom_factor_x;
    x_left += 1.0 * delta_x / ctx->zoom_factor_x;
    y_high += 1.0 * delta_y / ctx->zoom_factor_y;
    y_low += 1.0 * delta_y / ctx->zoom_factor_y;
    obst_x_right += 1.0 * delta_x / ctx->zoom_factor_x;
    obst_x_left += 1.0 * delta_x / ctx->zoom_factor_x;
    obst_y_high += 1.0 * delta_y / ctx->zoom_factor_y;
    obst_y_low += 1.0 * delta_y / ctx->zoom_factor_y;
  }
//  ctx->set_defered_redraw();
//  ctx->redraw_node_cons( this);
  ctx->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
//  ctx->redraw_defered();
}

void GlowNode::print( double ll_x, double ll_y, double ur_x, double ur_y) 
{ 
  double tmp;

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

  if ( x_right >= ll_x &&
       x_left <= ur_x &&
       y_high >= ll_y &&
       y_low <= ur_y)
    nc->print( &pos, (void *)this);
}

void GlowNode::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  int i;
  char *s;

  if ( (mode == glow_eSaveMode_Trace && nc->group != glow_eNodeGroup_Trace) ||
       (mode == glow_eSaveMode_Edit && nc->group == glow_eNodeGroup_Trace) )
    return;

  fp << int(glow_eSave_Node) << endl;
  fp << int(glow_eSave_Node_nc) << FSPACE << nc->nc_name << endl;
  fp << int(glow_eSave_Node_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_Node_refcon_cnt) << endl;
  for ( i = 0; i < MAX_CONPOINTS; i++)
    fp << refcon_cnt[i] << endl;
  fp << int(glow_eSave_Node_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_Node_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_Node_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_Node_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_Node_obst_x_right) << FSPACE << obst_x_right << endl;
  fp << int(glow_eSave_Node_obst_x_left) << FSPACE << obst_x_left << endl;
  fp << int(glow_eSave_Node_obst_y_high) << FSPACE << obst_y_high << endl;
  fp << int(glow_eSave_Node_obst_y_low) << FSPACE << obst_y_low << endl;
  fp << int(glow_eSave_Node_annotsize) << endl;
  for ( i = 0; i < 10; i++)
    fp << annotsize[i] << endl;
  fp << int(glow_eSave_Node_annotv) << endl;
  for ( i = 0; i < 10; i++)
  {
    if( annotsize[i])
    {
      fp << "\"";
      for ( s  = annotv[i]; *s; s++)
      {
        if ( *s == '"')
	  fp << "\\";
        fp << *s;
      }
      fp << "\"" << endl;
    }
  }
  fp << int(glow_eSave_Node_pos) << endl;
  pos.save( fp, mode);
  fp << int(glow_eSave_Node_trace_data1) << FSPACE << trace.data[0] << endl;
  fp << int(glow_eSave_Node_trace_data2) << FSPACE << trace.data[1] << endl;
  fp << int(glow_eSave_Node_trace_data3) << FSPACE << trace.data[2] << endl;
  fp << int(glow_eSave_Node_trace_data4) << FSPACE << trace.data[3] << endl;
  fp << int(glow_eSave_Node_trace_data5) << FSPACE << trace.data[4] << endl;
  fp << int(glow_eSave_Node_trace_data6) << FSPACE << trace.data[5] << endl;
  fp << int(glow_eSave_Node_trace_data7) << FSPACE << trace.data[6] << endl;
  fp << int(glow_eSave_Node_trace_data8) << FSPACE << trace.data[7] << endl;
  fp << int(glow_eSave_Node_trace_data9) << FSPACE << trace.data[8] << endl;
  fp << int(glow_eSave_Node_trace_data10) << FSPACE << trace.data[9] << endl;
  fp << int(glow_eSave_Node_trace_attr_type) << FSPACE << int(trace.attr_type) << endl;
  fp << int(glow_eSave_Node_trace_color) << FSPACE << int(trace.color) << endl;
  fp << int(glow_eSave_Node_trace_color2) << FSPACE << int(trace.color2) << endl;
  fp << int(glow_eSave_Node_access) << FSPACE << (unsigned int)(trace.access) << endl;
  fp << int(glow_eSave_Node_cycle) << FSPACE << (unsigned int)(trace.cycle) << endl;
  fp << int(glow_eSave_Node_ref_object) << FSPACE << trace.ref_object << endl;
  fp << int(glow_eSave_End) << endl;  
}

void GlowNode::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  char		nc_name[80];
  int		i, j;
  char		c;
  int		tmp;
  unsigned int	utmp;
  int		sts;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Node: break;
      case glow_eSave_Node_nc:
        fp.get();
        fp.getline( nc_name, sizeof(nc_name));
        if ( ctx->type() != glow_eCtxType_Grow ||
             ((GrowNode *)this)->type() != glow_eObjectType_GrowGroup)
        {
          nc = (GlowNodeClass *) ctx->get_nodeclass_from_name( nc_name);
          if ( !nc && ctx->type() == glow_eCtxType_Grow)
          {
            // If grow, load subgraph
            sts = ((GrowCtx *)ctx)->open_subgraph_from_name( nc_name, glow_eSaveMode_SubGraph);
            if ( ODD(sts))
            {
              nc = (GlowNodeClass *) ctx->get_nodeclass_from_name( nc_name);
              if ( nc) 
                nc->nc_extern = 1;
            }
          }
          if ( !nc) 
            cout << "GlowNode:nodeclass not found: " << nc_name << endl;
          nc_root = nc;
        }
        break;
      case glow_eSave_Node_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_Node_refcon_cnt:
        for ( i = 0; i < MAX_CONPOINTS; i++)
          fp >> refcon_cnt[i];
        break;
      case glow_eSave_Node_x_right: fp >> x_right; break;
      case glow_eSave_Node_x_left: fp >> x_left; break;
      case glow_eSave_Node_y_high: fp >> y_high; break;
      case glow_eSave_Node_y_low: fp >> y_low; break;
      case glow_eSave_Node_obst_x_right: fp >> obst_x_right; break;
      case glow_eSave_Node_obst_x_left: fp >> obst_x_left; break;
      case glow_eSave_Node_obst_y_high: fp >> obst_y_high; break;
      case glow_eSave_Node_obst_y_low: fp >> obst_y_low; break;
      case glow_eSave_Node_annotsize:
        for ( i = 0; i < 10; i++)
          fp >> annotsize[i];
        break;
      case glow_eSave_Node_annotv:
        fp.getline( dummy, sizeof(dummy));
        for ( i = 0; i < 10; i++)
        {
          if ( annotsize[i])
          {
            annotv[i] = (char *) calloc( 1, annotsize[i]);
	    fp.get();
            for ( j = 0; j < annotsize[i]; j++)
	    {
	      if ((c = fp.get()) == '"')
	      {
	        if ( annotv[i][j-1] == '\\')
	          j--;
	        else
                {
	          annotv[i][j] = 0;
                  break;
                }
	      }
              annotv[i][j] = c;
	    }
            fp.getline( dummy, sizeof(dummy));
          }
        }
        break;
      case glow_eSave_Node_pos: pos.open( fp); break;
      case glow_eSave_Node_trace_data1:
        fp.get();
        fp.getline( trace.data[0], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data2:
        fp.get();
        fp.getline( trace.data[1], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data3:
        fp.get();
        fp.getline( trace.data[2], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data4:
        fp.get();
        fp.getline( trace.data[3], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data5:
        fp.get();
        fp.getline( trace.data[4], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data6:
        fp.get();
        fp.getline( trace.data[5], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data7:
        fp.get();
        fp.getline( trace.data[6], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data8:
        fp.get();
        fp.getline( trace.data[7], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data9:
        fp.get();
        fp.getline( trace.data[8], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data10:
        fp.get();
        fp.getline( trace.data[9], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_attr_type: fp >> tmp; trace.attr_type = (glow_eTraceType)tmp; break;
      case glow_eSave_Node_trace_color: fp >> tmp; trace.color = (glow_eDrawType)tmp; break;
      case glow_eSave_Node_trace_color2: fp >> tmp; trace.color2 = (glow_eDrawType)tmp; break;
      case glow_eSave_Node_access: fp >> utmp; trace.access = (glow_mAccess)utmp; break;
      case glow_eSave_Node_cycle: fp >> tmp; trace.cycle = (glow_eCycle)tmp; break;
      case glow_eSave_Node_ref_object:
        fp.get();
        fp.getline( trace.ref_object, sizeof(trace.ref_object));
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowNode:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  if ( ctx->trace_started)
    trace_init();
}

void GlowNode::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
  }
}

void GlowNode::draw_inverse() 
{
  double x_r;

  if ( ctx->type() == glow_eCtxType_Brow)
    x_r = ((BrowCtx *)ctx)->frame_x_right;
  else
    x_r = x_right;

  glow_draw_fill_rect( ctx,
       		int( x_left * ctx->zoom_factor_x - ctx->offset_x),
       		int( y_low * ctx->zoom_factor_y - ctx->offset_y),
		int( x_r * ctx->zoom_factor_x - x_left * ctx->zoom_factor_x),
  	     	int( y_high * ctx->zoom_factor_y - y_low * ctx->zoom_factor_y),
		glow_eDrawType_Line);
  nc->draw_inverse( &pos, 0, (void *)this);
}

void GlowNode::erase() 
{
  if ( !inverse)
    nc->erase( &pos, hot, (void *)this);
  else
  {
    double x_r;
    if ( ctx->type() == glow_eCtxType_Brow)
      x_r = ((BrowCtx *)ctx)->frame_x_right;
    else
      x_r = x_right;

    glow_draw_fill_rect( ctx,
       	int( x_left * ctx->zoom_factor_x - ctx->offset_x),
       	int( y_low * ctx->zoom_factor_y - ctx->offset_y),
	int( x_r * ctx->zoom_factor_x - x_left * ctx->zoom_factor_x),
       	int( y_high * ctx->zoom_factor_y - y_low * ctx->zoom_factor_y),
	glow_eDrawType_LineErase);
  }
}

void GlowNode::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
    if ( x_right_pix - x_left_pix < 10 && y_high_pix - y_low_pix < 10)
      glow_draw_nav_rect( ctx, x_left_pix, y_low_pix, 
	x_right_pix - x_left_pix, y_high_pix - y_low_pix,
	glow_eDrawType_Line, 0, highlight);
    else
      nc->nav_draw( &pos, highlight, (void *)this);
  }
}

void GlowNode::nav_erase() 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y);

  if ( x_right_pix - x_left_pix < 10 && y_high_pix - y_low_pix < 10)
    glow_draw_nav_rect_erase( ctx, x_left_pix, y_low_pix, 
	x_right_pix - x_left_pix, y_high_pix - y_low_pix,
	0);
  else
    nc->nav_erase( &pos, (void *)this);
}

void GlowNode::set_highlight( int on)
{
  highlight = on;
  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);
}

void GlowNode::set_hot( int on)
{
  if ( hot != on)
  {
    if ( hot && !inverse)
      nc->erase( &pos, hot, (void *)this);
    hot = on;
    if ( !inverse)
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
    nc->nav_draw( &pos, highlight, (void *)this);
  }
}

void GlowNode::set_inverse( int on)
{
  inverse = on;

  if ( !inverse)
  {
    double x_r;
    if ( ctx->type() == glow_eCtxType_Brow)
      x_r = ((BrowCtx *)ctx)->frame_x_right;
    else
      x_r = x_right;

    glow_draw_fill_rect( ctx,
       	int( x_left * ctx->zoom_factor_x - ctx->offset_x),
       	int( y_low * ctx->zoom_factor_y - ctx->offset_y),
	int( x_r * ctx->zoom_factor_x - x_left * ctx->zoom_factor_x),
       	int( y_high * ctx->zoom_factor_y - y_low * ctx->zoom_factor_y + 1),
	glow_eDrawType_LineErase);
    nc->draw( &pos, highlight, hot, (void *)this);
  }
  else
  {
    draw_inverse();
  }
}

void GlowNode::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround || 
       nc->group == glow_eNodeGroup_Document)
  {
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else
  {
    if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
      ctx->select_insert( this);
  }
}

void GlowNode::set_annotation( int num, char *text, int size, int nodraw)
{

  if ( !nodraw)
  {
    if ( relative_annot_pos)
      erase();
    else
      nc->erase_annotation( &pos, highlight, hot, (void *) this, num);
  }
  if ( !annotv[num])
  {
    annotv[num] = (char *) calloc( 1, size+1);
    annotsize[num] = size+1;
  }
  else if ( annotsize[num] < size+1)
  {
    free( annotv[num]);
    annotv[num] = (char *) calloc( 1, size+1);
    annotsize[num] = size+1;
  }
  strncpy( annotv[num], text, size+1);
  if ( nodraw)
    return;

  if ( relative_annot_pos)
  {
    relative_annot_x = 0;
    nc->configure_annotations( &pos, (void *)this);
    if ( !inverse)
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
  }
  else
  {
    nc->draw_annotation( &pos, highlight, hot, (void *) this, num);
    if ( inverse)
      draw_inverse();
  }

  nc->nav_draw( &pos, highlight, (void *)this);

}

void GlowNode::get_annotation( int num, char *text, int size)
{
  if ( !annotv[num])
    strcpy( text, "");
  else
  {
    strncpy( text, annotv[num], size);
    text[size-1] = 0;
  }
}

void GlowNode::open_annotation_input( int num)
{
  if ( annotv_inputmode[num])
    return;

  annotv_inputmode[num] = 1;
  nc->open_annotation_input( &pos, this, num);

  if ( ctx->type() == glow_eCtxType_Brow)
    ((BrowCtx *)ctx)->configure( y_low);
  ctx->widget_cnt++;
}

void GlowNode::close_annotation_input( int num)
{
  if ( !annotv_inputmode[num])
    return;

  nc->close_annotation_input( this, num);
  annotv_inputmode[num] = 0;
  ctx->widget_cnt--;

  if ( ctx->type() == glow_eCtxType_Brow)
    ((BrowCtx *)ctx)->configure( y_low);
}

int GlowNode::get_annotation_input( int num, char **text)
{
  if ( !annotv_inputmode[num])
    return 0;

  return nc->get_annotation_input( this, num, text);
}

void GlowNode::measure_annotation( int num, char *text, double *width, 
	double *height)
{
  nc->measure_annotation( num, text, width, height);
}

void GlowNode::set_annot_pixmap( int num, glow_sAnnotPixmap *pixmap, 
	int nodraw)
{
  annotpixmapv[num] = pixmap;
  if ( relative_annot_pos)
  {
    relative_annot_x = 0;
    nc->configure_annotations( &pos, (void *)this);
  }

  if ( nodraw)
    return;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);

}

void GlowNode::remove_annot_pixmap( int num)
{

  nc->erase( &pos, hot, (void *)this);
  nc->nav_erase( &pos, (void *)this);

  annotpixmapv[num] = 0;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);

}

void GlowNode::set_radiobutton( int num, int value, int nodraw)
{
  rbuttonv[num] = value;

  if ( nodraw)
    return;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);
}

void GlowNode::get_radiobutton( int num, int *value)
{
  *value = rbuttonv[num];
}

void GlowNode::conpoint_refcon_reconfig( int conpoint)
{
//  ctx->conpoint_refcon_erase( this, conpoint);
  refcon_cnt[conpoint] = 0;
  ctx->conpoint_refcon_redraw( this, conpoint);
}

int GlowNode::event_handler( glow_eEvent event, int x, int y)
{
  int sts;
  double distance;
  GlowConPoint *conpoint;

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = nc->event_handler( &pos, event, x, y, (void *)this);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
      store_position();
    }
    return sts;
  }
  else if ( event == ctx->event_create_con)
  {
    sts = nc->event_handler( &pos, event, x, y, (void *)this);
    if ( sts)
    {
      /* Find closest conpoint */
      distance = 1e10;
      conpoint = 0;
      nc->a.conpoint_select( &pos, x, y, &distance, 
		(void **) &conpoint);
      if ( conpoint)
      {
        ctx->con_create_source( this, conpoint->number, 
		pos.z_x - ctx->offset_x + conpoint->p.z_x, 
	 	pos.z_y - ctx->offset_y + conpoint->p.z_y);
      }
    }
    return sts;
  }
  switch ( event)
  {
    case glow_eEvent_ButtonRelease:
      if ( ctx->con_create_active)
      {
        sts = nc->event_handler( &pos, event, x, y, (void *)this);
        if ( sts)
        {
          /* Find closest conpoint */
          distance = 1e10;
          conpoint = 0;
          nc->a.conpoint_select( &pos, x, y, &distance, 
		(void **) &conpoint);
          if ( conpoint)
          {
            ctx->con_create_dest( this, conpoint->number, event, x, y);
          }
        }
      }
      break;
    case glow_eEvent_CursorMotion:
    {
      int redraw = 0;

      sts = nc->event_handler( &pos, event, x, y, (void *)this);
      if ( sts && !hot  && 
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
        hot = 1;
        redraw = 1;
      }
      if ( !sts && hot)
      {
        draw_set_cursor( ctx, glow_eDrawCursor_Normal);
        erase();
        hot = 0;
        redraw = 1;
      }
      if ( redraw)
      {
        if ( !inverse)
          nc->draw( &pos, highlight, hot, (void *)this);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = nc->event_handler( &pos, event, x, y, (void *)this);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}

void GlowNode::remove_notify()
{
  ctx->delete_node_cons( this);
}

void GlowNode::set_trace_attr( GlowTraceData *attr)
{
  memcpy( &trace, attr, sizeof(trace));

  if ( ctx->trace_started /* && strcmp( trace.data[0], "") != 0 */)
    ctx->trace_connect_func( (void *) this, &trace);
}

void GlowNode::get_trace_attr( GlowTraceData **attr)
{
  *attr = &trace;
}

void GlowNode::trace_scan()
{
  if ( ctx->trace_scan_func && trace.p)
    ctx->trace_scan_func( (void *) this, trace.p);
}

int GlowNode::trace_init()
{
  int sts;

  // if ( strcmp( trace.data[0], "") == 0)
  //  return 1;

  sts = ctx->trace_connect_func( (void *) this, &trace);
  return sts;
}

void GlowNode::trace_close()
{
  // if ( strcmp( trace.data[0], "") == 0)
  //  return;

  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);
}


//
// Configure the brow ctx
//
void GlowNode::configure( void *previous)
{
  double x, y;

  if ( !previous)
  {
    x = 0;
    y = 0;
  }
  else
  {
    x = ((BrowCtx *)ctx)->indentation * level;
    y = ((GlowNode *) previous)->y_high /* + 1.0 / ctx->zoom_factor_y */;
  }

  pos.posit( x, y);
  x_left = y_low = 1e37;
  x_right = y_high = -1e37;
  get_node_borders();

  // Fix, double size if inputmode
  int am = 0;
  for ( int i = 0; i < 10; i++)
    am +=  annotv_inputmode[i];
  if (am)
    y_high += y_high - y_low;
}

void GlowNode::move_widgets( int x, int y)
{
  int am = 0;
  for ( int i = 0; i < 10; i++)
    am +=  annotv_inputmode[i];
  if (am)
    nc->move_widgets( this, x, y);
}

ostream& operator<< ( ostream& o, const GlowNode n)
{
  o << "Node: " << n.n_name << " Class: " << n.nc->nc_name << " Pos: " << n.pos;
  return o;
}

