#include "glow_std.h"


#include <iostream.h>
#include "glow_point.h"


GlowPoint::GlowPoint( GlowCtx *glow_ctx, double x1, double y1)
 : ctx(glow_ctx), x(x1), y(y1)
{
  (*this).zoom();
}

void GlowPoint::posit( double x1, double y1)
{
  x = x1;
  y = y1;
  z_x = int( x * ctx->zoom_factor_x + 0.5);
  z_y = int( y * ctx->zoom_factor_y + 0.5);
  nav_z_x = int( x * ctx->nav_zoom_factor_x + 0.5);
  nav_z_y = int( y * ctx->nav_zoom_factor_y + 0.5);
}

void GlowPoint::posit_z( int x1, int y1)
{
  x = 1.0 * x1 / ctx->zoom_factor_x;
  y = 1.0 * y1 / ctx->zoom_factor_y;
  z_x = x1;
  z_y = y1;
  nav_z_x = int( x * ctx->nav_zoom_factor_x);
  nav_z_y = int( y * ctx->nav_zoom_factor_y);
}

void GlowPoint::zoom()
{
  z_x = int( x * ctx->zoom_factor_x + 0.5);
  z_y = int( y * ctx->zoom_factor_y + 0.5);
  current_zoom_factor = ctx->zoom_factor_x;
}

void GlowPoint::nav_zoom()
{
  nav_z_x = int( x * ctx->nav_zoom_factor_x + 0.5);
  nav_z_y = int( y * ctx->nav_zoom_factor_y + 0.5);
}

void GlowPoint::print_zoom()
{
  print_z_x = x * ctx->print_zoom_factor;
  print_z_y = y * ctx->print_zoom_factor;
}

void GlowPoint::traverse( int x, int y)
{
}

void GlowPoint::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Point) << endl;
  fp << int(glow_eSave_Point_x) << FSPACE << x << endl;
  fp << int(glow_eSave_Point_y) << FSPACE << y << endl;
  fp << int(glow_eSave_End) << endl;
}

void GlowPoint::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Point: break;
      case glow_eSave_Point_x: fp >> x; break;
      case glow_eSave_Point_y: fp >> y; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowPoint:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

const GlowPoint& GlowPoint::operator+= (const GlowPoint p)
{
  x += p.x;  
  y += p.y;
  z_x += p.z_x;  
  z_y += p.z_y;
  return *this;
}

GlowPoint GlowPoint::operator+ (const GlowPoint p)
{
  GlowPoint tmp(*this);
  tmp += p;
  return tmp;
}

const GlowPoint& GlowPoint::operator-= (const GlowPoint p)
{
  x -= p.x;  
  y -= p.y;
  z_x -= p.z_x;  
  z_y -= p.z_y;
  return *this;
}

GlowPoint GlowPoint::operator- (const GlowPoint p)
{
  GlowPoint tmp(*this);
  tmp -= p;
  return tmp;
}

ostream& operator<< ( ostream& o, const GlowPoint p)
{
  o << '(' << p.x << ',' << p.y << ')' << '[' << p.z_x << ',' << p.z_y << ']' ;
  return o;
}

