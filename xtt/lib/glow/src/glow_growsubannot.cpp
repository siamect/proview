#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growsubannot.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowSubAnnot::GrowSubAnnot( GlowCtx *glow_ctx, char *name, double x, double y,
	int annot_num, glow_eDrawType d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	int nodraw) :
	GlowAnnot(glow_ctx,x,y,annot_num,d_type,t_size,a_type,
	rel_pos,display_lev),
    	hot(0), pzero(ctx), highlight(0), inverse(0), 
	text( ctx, "", x, y, d_type, t_size),
	rect( ctx, x, y - ctx->draw_delta, ctx->draw_delta, ctx->draw_delta,
	glow_eDrawType_LineGray),
	user_data(NULL)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();

  sprintf( text.text, "A%d", annot_num);
  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
    text.p.posit( p.x, p.y);
    rect.ll.posit( p.x, p.y - ctx->draw_delta);
    rect.ur.posit( p.x + ctx->draw_delta, p.y);
  }
  if ( !nodraw)
  {
    rect.draw( (void *)&pzero, highlight, hot, NULL);
    text.draw( (void *)&pzero, highlight, hot, NULL);
  }
  get_node_borders();
}

GrowSubAnnot::~GrowSubAnnot()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

void GrowSubAnnot::move( int delta_x, int delta_y, int grid)
{
  double x1, y1;

  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    erase();
    nav_erase();
    ctx->find_grid( x_left + double( delta_x) / ctx->zoom_factor_x,
	y_low + double( delta_y) / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase();
    nav_erase();
    dx = double( delta_x) / ctx->zoom_factor_x;
    dy = double( delta_y) / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowSubAnnot::move_noerase( int delta_x, int delta_y, int grid)
{
  double x1, y1;
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    ctx->find_grid( x_left + double( delta_x) / ctx->zoom_factor_x,
	y_low + double( delta_y) / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    dx = double( delta_x) / ctx->zoom_factor_x;
    dy = double( delta_y) / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

int GrowSubAnnot::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = rect.event_handler( (void *)&pzero, 
		event, x, y, NULL);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event)
  {
    case glow_eEvent_CursorMotion:
    {
      int redraw = 0;

      if ( ctx->hot_found)
        sts = 0;
      else
      {
        sts = rect.event_handler( (void *)&pzero, event, x, y, NULL);
        if ( sts)
          ctx->hot_found = 1;
      }
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
          ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
//          text.draw( (void *)&pzero, highlight, hot, NULL);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = rect.event_handler( (void *)&pzero, 
		event, x, y, NULL);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowSubAnnot::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  if ( mode == glow_eSaveMode_SubGraph)
  {
    // Save as GrowAnnot
    fp << int(glow_eSave_GrowAnnot) << endl;
    fp << int(glow_eSave_GrowAnnot_annot_part) << endl;
    GlowAnnot::save( fp, mode);
    fp << int(glow_eSave_GrowAnnot_trf) << endl;
    trf.save( fp, mode);
    fp << int(glow_eSave_End) << endl;
  }
  else
  {
    fp << int(glow_eSave_GrowSubAnnot) << endl;
    fp << int(glow_eSave_GrowSubAnnot_n_name) << FSPACE << n_name << endl;
    fp << int(glow_eSave_GrowSubAnnot_x_right) << FSPACE << x_right << endl;
    fp << int(glow_eSave_GrowSubAnnot_x_left) << FSPACE << x_left << endl;
    fp << int(glow_eSave_GrowSubAnnot_y_high) << FSPACE << y_high << endl;
    fp << int(glow_eSave_GrowSubAnnot_y_low) << FSPACE << y_low << endl;
    fp << int(glow_eSave_GrowSubAnnot_text) << endl;
    text.save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_rect) << endl;
    rect.save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_annot_part) << endl;
    GlowAnnot::save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_trf) << endl;
    trf.save( fp, mode);
    fp << int(glow_eSave_End) << endl;
  }
}

void GrowSubAnnot::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowSubAnnot: break;
      case glow_eSave_GrowSubAnnot_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowSubAnnot_x_right: fp >> x_right; break;
      case glow_eSave_GrowSubAnnot_x_left: fp >> x_left; break;
      case glow_eSave_GrowSubAnnot_y_high: fp >> y_high; break;
      case glow_eSave_GrowSubAnnot_y_low: fp >> y_low; break;
      case glow_eSave_GrowSubAnnot_text: text.open( fp); break;
      case glow_eSave_GrowSubAnnot_rect: rect.open( fp); break;
      case glow_eSave_GrowSubAnnot_annot_part: 
        GlowAnnot::open( fp);
        break;
      case glow_eSave_GrowSubAnnot_trf: trf.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowSubAnnot:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowSubAnnot::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
    {
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    }
    else
      draw_inverse();
  }
}

void GrowSubAnnot::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowSubAnnot::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	obj_ur_x = int( x_right * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ll_x = int( x_left * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ur_y = int( y_high * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  int   obj_ll_y = int( y_low * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);

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

void GrowSubAnnot::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
    rect.nav_draw( (void *)&pzero, highlight, NULL);
    text.nav_draw( (void *)&pzero, highlight, NULL);
  }
}

void GrowSubAnnot::set_highlight( int on)
{
  highlight = on;
  if ( !inverse)
  {
    rect.draw( (void *)&pzero, highlight, hot, NULL);
    text.draw( (void *)&pzero, highlight, hot, NULL);
  }
  else
    draw_inverse();
  rect.nav_draw( (void *)&pzero, highlight, NULL);
  text.nav_draw( (void *)&pzero, highlight, NULL);
}

void GrowSubAnnot::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround )
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

void GrowSubAnnot::zoom()
{
  GlowAnnot::zoom();
  rect.zoom();
  text.zoom();
}

void GrowSubAnnot::nav_zoom()
{
  GlowAnnot::nav_zoom();
  rect.nav_zoom();
  text.nav_zoom();
}

void GrowSubAnnot::draw( GlowTransform *t, int highlight, int hot, void *node,
			 void *colornode)
{
  if ( t)
  {
    GlowPoint p1(p);

    p1.x = trf.x( t, p.x, p.y) - trf.x( p.x, p.y);
    p1.y = trf.y( t, p.x, p.y) - trf.y( p.x, p.y);
    p1.zoom();
    rect.draw( (void *)&p1, highlight, hot, NULL);
    text.draw( (void *)&p1, highlight, hot, NULL);
  }
  else
  {
    rect.draw( (void *)&pzero, highlight, hot, NULL);
    text.draw( (void *)&pzero, highlight, hot, NULL);
  }
}

void GrowSubAnnot::erase( GlowTransform *t, int hot, void *node)
{
  ctx->set_draw_buffer_only();
  if ( t)
  {
    GlowPoint p1(p);

    p1.x = trf.x( t, p.x, p.y) - trf.x( p.x, p.y);
    p1.y = trf.y( t, p.x, p.y) - trf.y( p.x, p.y);
    p1.zoom();
    rect.erase( (void *)&p1, hot, NULL);
    text.erase( (void *)&p1, hot, NULL);
  }
  else
  {
    rect.erase( (void *)&pzero, hot, NULL);
    text.erase( (void *)&pzero, hot, NULL);
  }
  ctx->reset_draw_buffer_only();
}

void GrowSubAnnot::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  rect.nav_draw( (void *)&pzero, highlight, NULL);
  text.nav_draw( (void *)&pzero, highlight, NULL);
}

void GrowSubAnnot::nav_erase( GlowTransform *t, void *node)
{
  rect.nav_erase( (void *)&pzero, NULL);
  text.nav_erase( (void *)&pzero, NULL);
}


void GrowSubAnnot::set_transform( GlowTransform *t)
{
  double x1, y1;

  trf = *t * trf;

  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);

  get_node_borders();

}

void GrowSubAnnot::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy, x1, y1;

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

    x1 = trf.x( p.x, p.y);
    y1 = trf.y( p.x, p.y);
    rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
    text.move( (void *)&pzero, x1, y1, highlight, hot);
    ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
    ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}


void GrowSubAnnot::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int x1, y1;
  int bold;

  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }

  bold = (draw_type == glow_eDrawType_TextHelveticaBold);

  ((GrowCtx *)ctx)->export_jbean->annot( x1, y1, number,
	draw_type, bold, idx, pass, shape_cnt, node_cnt, fp);
  (*shape_cnt)++;
}

