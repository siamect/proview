#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growtext.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowText::GrowText( GlowCtx *glow_ctx, char *name, char *text1, double x, double y, 
		glow_eDrawType d_type, glow_eDrawType color_d_type, int t_size, 
		glow_mDisplayLevel display_lev, int nodraw) : 
		GlowText(glow_ctx,text1,x,y,d_type,color_d_type,t_size,display_lev), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), user_data(NULL),
		dynamic(0), dynamicsize(0), original_color_drawtype(glow_eDrawType_Line)
		
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();
  strcpy( last_group, "");

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
  }
  get_node_borders();
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
}

GrowText::~GrowText()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

void GrowText::move( double delta_x, double delta_y, int grid)
{
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
    ctx->find_grid( x_left + delta_x / ctx->zoom_factor_x,
	y_low + delta_y / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase();
    nav_erase();
    dx = delta_x / ctx->zoom_factor_x;
    dy = delta_y / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowText::move_noerase( int delta_x, int delta_y, int grid)
{
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
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

int GrowText::local_event_handler( glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = x_left;
  ll_y = y_low;
  ur_x = x_right;
  ur_y = y_high;

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y)
  {
//    cout << "Event handler: Hit in rect" << endl;
    return 1;
  }  
  else
    return 0;
}

int GrowText::event_handler( glow_eEvent event, double fx, double fy)
{
  return local_event_handler( event, fx, fy);
}

int GrowText::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = local_event_handler( event, fx, fy);
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

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
        sts = 0;
      else if ( ctx->hot_found)
        sts = 0;
      else
      {
        sts = local_event_handler( event, fx, fy);
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
        if ( !ctx->hot_found)
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
//          ((GlowRect *)this)->draw( (void *)&pzero, highlight, hot, NULL);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = local_event_handler( event, fx, fy);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowText::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowText) << endl;
  fp << int(glow_eSave_GrowText_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowText_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowText_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowText_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowText_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowText_original_color_drawtype) << FSPACE 
		<< int(original_color_drawtype) << endl;
  fp << int(glow_eSave_GrowText_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowText_dynamic) << endl;
  if( dynamic)
  {
    fp << "\"";
    for ( s  = dynamic; *s; s++)
    {
      if ( *s == '"')
        fp << "\\";
      fp << *s;
    }
    fp << "\"" << endl;
  }
  fp << int(glow_eSave_GrowText_text_part) << endl;
  GlowText::save( fp, mode);
  fp << int(glow_eSave_GrowText_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowText::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		j;
  char		c;
  char          *new_text;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowText: break;
      case glow_eSave_GrowText_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowText_x_right: fp >> x_right; break;
      case glow_eSave_GrowText_x_left: fp >> x_left; break;
      case glow_eSave_GrowText_y_high: fp >> y_high; break;
      case glow_eSave_GrowText_y_low: fp >> y_low; break;
      case glow_eSave_GrowText_original_color_drawtype: fp >> 
		tmp; original_color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowText_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowText_dynamic:
        fp.getline( dummy, sizeof(dummy));
        if ( dynamicsize)
        {
          dynamic = (char *) calloc( 1, dynamicsize);
	  fp.get();
          for ( j = 0; j < dynamicsize; j++)
	  {
	    if ((c = fp.get()) == '"')
	    {
	      if ( dynamic[j-1] == '\\')
	        j--;
	      else
              {
	        dynamic[j] = 0;
                break;
              }
	    }
            dynamic[j] = c;
	  }
          fp.getline( dummy, sizeof(dummy));
        }
        break;
      case glow_eSave_GrowText_text_part: 
        GlowText::open( fp);
        break;
      case glow_eSave_GrowText_trf: trf.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowText:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }

  // Translate the text to current language
  if ( ((GrowCtx *)ctx)->translate_on && 
       ctx->event_callback[glow_eEvent_Translate]) {
    if ( ((GrowCtx *)ctx)->translate_cb( this, text, &new_text))
      strcpy( text, new_text);
      get_node_borders();
  }
}

void GrowText::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowText::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowText::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowText::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowText::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowText::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowText::set_dynamic( char *code, int size)
{
  if ( !dynamic)
  {
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  else if ( dynamicsize < size+1)
  {
    free( dynamic);
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  strncpy( dynamic, code, size+1);
}

void GrowText::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowText::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == x && trf.a23 == y)
     return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.posit( x, y);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowText::set_scale( double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.s_a11 && trf.s_a22 &&
       fabs( scale_x - trf.a11 / trf.s_a11) < FLT_EPSILON &&
       fabs( scale_y - trf.a22 / trf.s_a22) < FLT_EPSILON)
    return;

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eScaleType_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eScaleType_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.scale_from_stored( scale_x, scale_y, x0, y0);
  get_node_borders();

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x_left = old_x_left;
      y_low = old_y_low;
      break;
    case glow_eScaleType_LowerRight:
      x_right = old_x_right;
      y_low = old_y_low;
      break;
    case glow_eScaleType_UpperRight:
      x_right = old_x_right;
      y_high = old_y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x_left = old_x_left;
      y_high = old_y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowText::set_rotation( double angel, 
	double x0, double y0, glow_eRotationPoint type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( fabs( angel - trf.rotation + trf.s_rotation) < FLT_EPSILON)
    return;

  switch( type)
  {
    case glow_eRotationPoint_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eRotationPoint_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eRotationPoint_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eRotationPoint_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eRotationPoint_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_high + y_low) / 2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.rotate_from_stored( angel, x0, y0);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowText::draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  int x1, y1;
  int z_width, z_height, z_descent;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int highl = highlight;
  if ( node)
    highl = ((GrowNode *)node)->highlight; 

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }

  if ( strcmp( text, ""))
  {
    if ( highl)
    {
      draw_get_text_extent( ctx, text, strlen(text), draw_type, max( 0, idx), 
		&z_width, &z_height, &z_descent);
//      glow_draw_rect( ctx, x1, y1 - z_height, z_width, z_height, 
//	glow_eDrawType_FillHighlight, max( 1, min( idx + hot, DRAW_TYPE_SIZE-1)), 0);
      glow_draw_rect( ctx, x1, y1 - (z_height-z_descent), z_width, z_height, 
	glow_eDrawType_FillHighlight, max( 1, min( idx + hot, 2)), 0);
    }
    else if ( hot && !node)
    {
      draw_get_text_extent( ctx, text, strlen(text), draw_type, max( 0, idx), 
		&z_width, &z_height, &z_descent);
      glow_draw_rect( ctx, x1, y1 - (z_height-z_descent), z_width, z_height,
	glow_eDrawType_LineGray, max( min(idx,2), 1), 0);
    }
    if ( idx >= 0) {
      glow_eDrawType color = ((GrowCtx *)ctx)->get_drawtype( color_drawtype, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 2);
      glow_draw_text( ctx, x1, y1, text, strlen(text), draw_type, color, idx, highlight, 
		0);
    }
  }
  else
  {
    draw_get_text_extent( ctx, "A", 1, draw_type, idx, &z_width, &z_height,
	&z_descent);
    glow_draw_rect( ctx, x1, y1 - (z_height-z_descent), z_width, z_height, 
	glow_eDrawType_LineGray, idx, 0);
  }
}

void GrowText::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int x1, y1;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int z_width, z_height, z_descent;
  int highl = highlight;
  if ( node)
    highl = ((GrowNode *)node)->highlight; 

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }

  ctx->set_draw_buffer_only();
  if ( strcmp( text, ""))
  {
    if ( highl)
    {
      draw_get_text_extent( ctx, text, strlen(text), draw_type, max( 0, idx),
		 &z_width, &z_height, &z_descent);
      glow_draw_rect_erase( ctx, x1, y1 - (z_height-z_descent), z_width, z_height, 
		max( 1, min( idx + hot, 2)));
    }
    else if ( hot && !node)
    {
      draw_get_text_extent( ctx, text, strlen(text), draw_type, max( 0, idx), 
		&z_width, &z_height, &z_descent);
      glow_draw_rect_erase( ctx, x1, y1 - (z_height-z_descent), z_width, z_height, 
		max (1, min(idx,2)));
    }
    if ( idx >= 0)
      glow_draw_text_erase( ctx, x1, y1, text, strlen(text), draw_type, idx, 0);
  }
  else
  {
    draw_get_text_extent( ctx, "A", 1, draw_type, idx, &z_width, &z_height, 
	&z_descent);
    glow_draw_rect_erase( ctx, x1, y1 - (z_height-z_descent), z_width, z_height, idx);
  }
  ctx->reset_draw_buffer_only();
}

void GrowText::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  int x1, y1;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->nav_zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }

  glow_draw_nav_text( ctx, x1, y1, text, strlen(text), draw_type, idx, 
		highlight, 0);
}

void GrowText::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int x1, y1;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->nav_zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }

  glow_draw_nav_text_erase( ctx, x1, y1, text, strlen(text), draw_type, idx,
		0);
}

void GrowText::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowText::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double ll_x, ur_x, ll_y, ur_y;
  int	z_width, z_height, z_descent;

  if ( t)
  {
    ll_x = trf.x( t, p.x, p.y);
    ur_y = trf.y( t, p.x, p.y);
  }
  else
  {
    ll_x = trf.x( p.x, p.y);
    ur_y = trf.y( p.x, p.y);
  }

  if ( strcmp( text, "") == 0)
    draw_get_text_extent( ctx, "A", 1, draw_type, text_size,
		&z_width, &z_height, &z_descent);
  else
  {
    double trf_scale = trf.vertical_scale( t);
    int idx = int( trf_scale * ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
    idx = max( 0, min( idx, DRAW_TYPE_SIZE-1));
    
    draw_get_text_extent( ctx, text, strlen(text), draw_type, idx,
		&z_width, &z_height, &z_descent);
  }
  ur_x = ll_x + double( z_width) / ctx->zoom_factor_y;
  ll_y = ur_y - double( z_height - z_descent) / ctx->zoom_factor_y;
  ur_y += double( z_descent) / ctx->zoom_factor_y;

  if ( display_level != glow_mDisplayLevel_1)
    return;
  if ( ll_x < *x_left)
    *x_left = ll_x;
  if ( ur_x > *x_right)
    *x_right = ur_x;
  if ( ll_y < *y_low)
    *y_low = ll_y;
  if ( ur_y > *y_high)
    *y_high = ur_y;
}

void GrowText::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}

void GrowText::set_text( char *new_text) 
{ 
  double y_low_old = y_low;
  double y_high_old = y_high;
  double x_left_old = x_left;
  double x_right_old = x_right;

  erase();
  nav_erase();
  strncpy( text, new_text, sizeof(text)-1);
  get_node_borders();
  // draw();

  y_low_old = min( y_low, y_low_old);
  y_high_old = max( y_high, y_high_old);
  x_left_old = min( x_left, x_left_old);
  x_right_old = max( x_right, x_right_old);
  ((GrowCtx *)ctx)->draw( x_left_old * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low_old * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right_old * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high_old * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left_old * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low_old * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right_old * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high_old * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowText::set_textsize( int size) 
{ 
  erase();
  nav_erase();
  text_size = size;
  get_node_borders();
  draw();
}

void GrowText::set_textbold( int bold) 
{ 
  if ( ( bold && draw_type == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && draw_type == glow_eDrawType_TextHelvetica))
    return;

  erase();
  nav_erase();
  if ( bold)
    draw_type = glow_eDrawType_TextHelveticaBold;
  else
    draw_type = glow_eDrawType_TextHelvetica;
  get_node_borders();
  draw();
}

void GrowText::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int x1, y1;
  int bold;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
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

  ((GrowCtx *)ctx)->export_jbean->text( x1, y1, text, 
	draw_type, color_drawtype, bold, idx, pass, shape_cnt, node_cnt, fp);
}

void GrowText::flip( double x0, double y0, glow_eFlipDirection dir)
{
  switch ( dir) {
  case glow_eFlipDirection_Horizontal:
    trf.store();
    set_scale( 1, -1, x0, y0, glow_eScaleType_FixPoint);
    break;
  case glow_eFlipDirection_Vertical:
    trf.store();
    set_scale( -1, 1, x0, y0, glow_eScaleType_FixPoint);
    break;
  }
}

void GrowText::align( double x, double y, glow_eAlignDirection direction)
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

void GrowText::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);

    break;
  }
  }  
}
