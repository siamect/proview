/* 
 * Proview   $Id: glow_point.cpp,v 1.5 2008-10-31 12:51:36 claes Exp $
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
#include "glow_growctx.h"

#include <iostream>
#include "glow_point.h"


GlowPoint::GlowPoint( GrowCtx *glow_ctx, double x1, double y1)
 : ctx(glow_ctx), x(x1), y(y1)
{
  (*this).zoom();
}

void GlowPoint::posit( double x1, double y1)
{
  x = x1;
  y = y1;
  z_x = int( x * ctx->mw.zoom_factor_x + 0.5);
  z_y = int( y * ctx->mw.zoom_factor_y + 0.5);
  nav_z_x = int( x * ctx->navw.zoom_factor_x + 0.5);
  nav_z_y = int( y * ctx->navw.zoom_factor_y + 0.5);
}

void GlowPoint::posit_z( int x1, int y1)
{
  x = 1.0 * x1 / ctx->mw.zoom_factor_x;
  y = 1.0 * y1 / ctx->mw.zoom_factor_y;
  z_x = x1;
  z_y = y1;
  nav_z_x = int( x * ctx->navw.zoom_factor_x);
  nav_z_y = int( y * ctx->navw.zoom_factor_y);
}

void GlowPoint::zoom()
{
  z_x = int( x * ctx->mw.zoom_factor_x + 0.5);
  z_y = int( y * ctx->mw.zoom_factor_y + 0.5);
  current_zoom_factor = ctx->mw.zoom_factor_x;
}

void GlowPoint::nav_zoom()
{
  nav_z_x = int( x * ctx->navw.zoom_factor_x + 0.5);
  nav_z_y = int( y * ctx->navw.zoom_factor_y + 0.5);
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
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GlowPoint: \"%d %s\"\n", type, dummy);      
    }

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

