#include "glow_std.h"


#include <iostream.h>
#include <math.h>
#include "glow_transform.h"

double GlowTransform::x( double x1, double y1) 
{
  return x1 * a11 + y1 * a12 + a13;
}

double GlowTransform::y( double x1, double y1)
{
  return x1 * a21 + y1 * a22 + a23;
}


GlowTransform GlowTransform::operator* (const GlowTransform p)
{
  static GlowTransform tmp;
  tmp.a11 = a11 * p.a11 + a12 * p.a21;
  tmp.a12 = a11 * p.a12 + a12 * p.a22;
  tmp.a13 = a11 * p.a13 + a12 * p.a23 + a13;
  tmp.a21 = a21 * p.a11 + a22 * p.a21;
  tmp.a22 = a21 * p.a12 + a22 * p.a22;
  tmp.a23 = a21 * p.a13 + a22 * p.a23 + a23;
  tmp.rotation = rotation + p.rotation;
  return tmp;
}

void GlowTransform::set_from_stored( GlowTransform *t) 
{
  a11 = t->a11 * s_a11 + t->a12 * s_a21;
  a12 = t->a11 * s_a12 + t->a12 * s_a22;
  a13 = t->a11 * s_a13 + t->a12 * s_a23 + t->a13;
  a21 = t->a21 * s_a11 + t->a22 * s_a21;
  a22 = t->a21 * s_a12 + t->a22 * s_a22;
  a23 = t->a21 * s_a13 + t->a22 * s_a23 + t->a23;
  rotation = s_rotation + t->rotation;
}

void GlowTransform::scale( double sx, double sy, double x0, double y0)
{
  a13 = a13 * sx + x0 * ( 1 - sx);
  a23 = a23 * sy + y0 * ( 1 - sy);
  a11 *= sx;
  a12 *= sx;
  a21 *= sy;
  a22 *= sy;
}

void GlowTransform::scale_from_stored( double sx, double sy, double x0, double y0)
{
  a13 = s_a13 * sx + x0 * ( 1 - sx);
  a23 = s_a23 * sy + y0 * ( 1 - sy);
  a11 = s_a11 * sx;
  a12 = s_a12 * sx;
  a21 = s_a21 * sy;
  a22 = s_a22 * sy;
}

void GlowTransform::rotate( double angel, double x0, double y0)
{
  double sin_a;
  double cos_a;
  GlowTransform tmp( *this);
  if ( -90.01 < angel && angel < -89.99)
  {
    sin_a = -1.0;
    cos_a = 0.0;
  }
  else
  {
    sin_a = sin( angel/180*3.14159);
    cos_a = cos( angel/180*3.14159);
  }

  a11 = tmp.a11 * cos_a - tmp.a21 * sin_a;
  a12 = tmp.a12 * cos_a - tmp.a22 * sin_a;
  a13 = tmp.a13 * cos_a - tmp.a23 * sin_a + x0 * ( 1 - cos_a) + y0 * sin_a;
  a21 = tmp.a11 * sin_a + tmp.a21 * cos_a;
  a22 = tmp.a21 * sin_a + tmp.a22 * cos_a;
  a23 = tmp.a13 * sin_a + tmp.a23 * cos_a + y0 * ( 1 - cos_a) - x0 * sin_a;
  rotation += angel;
}

void GlowTransform::rotate_from_stored( double angel, double x0, double y0)
{

  double sin_a;
  double cos_a;
  if ( -90.01 < s_rotation + angel  && s_rotation + angel < -89.99)
  {
    sin_a = -1.0;
    cos_a = 0.0;
  }
  else
  {
    sin_a = sin( (s_rotation + angel)/180*3.14159);
    cos_a = cos( (s_rotation + angel)/180*3.14159);
  }

  a11 = s_a11 * cos_a - s_a21 * sin_a;
  a12 = s_a12 * cos_a - s_a22 * sin_a;
  a13 = s_a13 * cos_a - s_a23 * sin_a + x0 * ( 1 - cos_a) + y0 * sin_a;
  a21 = s_a11 * sin_a + s_a21 * cos_a;
  a22 = s_a21 * sin_a + s_a22 * cos_a;
  a23 = s_a13 * sin_a + s_a23 * cos_a + y0 * ( 1 - cos_a) - x0 * sin_a;
  rotation = s_rotation + angel;
}

void GlowTransform::move( double x0, double y0)
{
  a13 += x0;
  a23 += y0;
}

void GlowTransform::move_from_stored( double x0, double y0)
{
  a13 = s_a13 + x0;
  a23 = s_a23 + y0;
}

void GlowTransform::posit( double x0, double y0)
{
  a13 = x0;
  a23 = y0;
}

double GlowTransform::x( GlowTransform *t, double x1, double y1)
{ 
  GlowTransform tmp = *t * *this;

  return tmp.x( x1, y1);
}

double GlowTransform::y( GlowTransform *t, double x1, double y1)
{ 
  GlowTransform tmp = *t * *this;

  return tmp.y( x1, y1);
}

void GlowTransform::reverse( double x, double y, double *rx, double *ry)
{
  if ( a11 == 0 || ( a12 * a21 - a11 * a22) == 0)
  {
    if ( a11 == 0 && a22 == 0 && a12 != 0 && a21 != 0)
    {
      *ry = (x - a13) / a12;
      *rx = (y - a23) / a21;
    }
    else
    {
      *ry = *rx = 0;
    }
    return;
  }
  *ry = ( a11 * ( a23 - y) - a21 * ( a13 - x)) / ( a12 * a21 - a11 * a22);
  *rx = ( x - a12 * *ry - a13) / a11;
}

double GlowTransform::vertical_scale( GlowTransform *t)
{
  if ( !t)
    return sqrt( a12*a12 + a22*a22);

  GlowTransform tmp = *t * *this;

  return sqrt( tmp.a12*tmp.a12 + tmp.a22*tmp.a22);
}

void GlowTransform::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Transform) << endl;
  fp << int(glow_eSave_Transform_a11) << FSPACE << a11 << endl;
  fp << int(glow_eSave_Transform_a12) << FSPACE << a12 << endl;
  fp << int(glow_eSave_Transform_a13) << FSPACE << a13 << endl;
  fp << int(glow_eSave_Transform_a21) << FSPACE << a21 << endl;
  fp << int(glow_eSave_Transform_a22) << FSPACE << a22 << endl;
  fp << int(glow_eSave_Transform_a23) << FSPACE << a23 << endl;
  fp << int(glow_eSave_Transform_rotation) << FSPACE << rotation << endl;
  fp << int(glow_eSave_End) << endl;
}

void GlowTransform::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Transform: break;
      case glow_eSave_Transform_a11: fp >> a11; break;
      case glow_eSave_Transform_a12: fp >> a12; break;
      case glow_eSave_Transform_a13: fp >> a13; break;
      case glow_eSave_Transform_a21: fp >> a21; break;
      case glow_eSave_Transform_a22: fp >> a22; break;
      case glow_eSave_Transform_a23: fp >> a23; break;
      case glow_eSave_Transform_rotation: fp >> rotation; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowTransform:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  store();
}

