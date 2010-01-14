/* 
 * Proview   $Id: cnv_image.cpp,v 1.7 2008-10-31 12:51:30 claes Exp $
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
 */


#include <iostream>
#include <fstream>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "cnv_image.h"

using namespace std;

//#define PWRE_CONF_GTK 1

#if defined PWRE_CONF_GTK

# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>

static int gdk_init_done = 0;
 
#elif defined PWRE_IMLIB

# include <Xm/Xm.h>
# include <Mrm/MrmPublic.h>
# ifndef _XtIntrinsic_h
#  include <X11/Intrinsic.h>
# endif
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# if defined OS_LYNX
#  define __NO_INCLUDE_WARN__ 1
# endif
# include <X11/extensions/shape.h>
# include <Imlib.h>
# if defined OS_LYNX
#  undef __NO_INCLUDE_WARN__
# endif

static ImlibData *imlib = 0;
static Display *display = 0;

#endif




int cnv_get_image( char *fname, cnv_tImImage *image, cnv_tPixmap *pixmap)
{
#if defined PWRE_CONF_GTK
  if ( !gdk_init_done) {
    gdk_init( 0, 0);
    gdk_init_done = 1;
  }

  *image = (cnv_tImImage *) gdk_pixbuf_new_from_file( fname, 0);
  if ( !*image)
    return 0;

#elif defined PWRE_IMLIB
  if ( !imlib) {
    display = XOpenDisplay(NULL);
    imlib = Imlib_init( display);
  }

  *image = (cnv_tImImage) Imlib_load_image( imlib, fname);
  if ( !*image)
    return 0;
  *pixmap = (cnv_tPixmap) Imlib_move_image( imlib, (ImlibImage *)*image);
#endif

  return 1;
}

void cnv_free_image( cnv_tImImage image, cnv_tPixmap pixmap)
{
#if defined PWRE_CONF_GTK
  gdk_pixbuf_unref( (GdkPixbuf *) image);

#elif defined PWRE_IMLIB

  Imlib_free_pixmap( imlib, (Pixmap) pixmap);
  Imlib_destroy_image( imlib, (ImlibImage *) image);
#endif
}

void cnv_print_image( cnv_tImImage image, char *filename)
{
#if defined PWRE_CONF_GTK
  char *s;
  char type[20];
  GError *error = NULL;

  s = strrchr( filename, '.');
  if ( !s)
    return;

  strcpy( type, s+1);

  if ( strcmp( type, "jpg") == 0)
    gdk_pixbuf_save( (GdkPixbuf *)image, filename, "jpeg", &error, "quality", "100", NULL);
  else
    gdk_pixbuf_save( (GdkPixbuf *)image, filename, type, &error, NULL);

#elif defined  PWRE_IMLIB
  Imlib_save_image( imlib, (ImlibImage *) image, filename, 0);
#endif
}

int cnv_image_width( cnv_tImImage image) 
{
#if defined PWRE_CONF_GTK
  return gdk_pixbuf_get_width( (GdkPixbuf *)image);
#elif defined  PWRE_IMLIB
  return ((ImlibImage *)image)->rgb_width;
#else
  return 0;
#endif
}

int cnv_image_height( cnv_tImImage image)
{
#if defined PWRE_CONF_GTK
  return gdk_pixbuf_get_height( (GdkPixbuf *)image);
#elif defined  PWRE_IMLIB
  return ((ImlibImage *)image)->rgb_height;
#else
  return 0;
#endif
}

void cnv_image_pixel_iter( cnv_tImImage image, 
			   void (* pixel_cb)(void *, ofstream&, unsigned char *), 
			   void *userdata, ofstream& fp)
{
#if defined PWRE_CONF_GTK
  unsigned char *rgb, *rgb_row;
  int 		rgb_height;
  int 		rgb_width;
  int		rowstride;
  int		n_channels;
  unsigned char null_rgb[4] = {0,0,0,0};
   
  rgb = gdk_pixbuf_get_pixels( (GdkPixbuf *)image);
  rgb_height = gdk_pixbuf_get_height( (GdkPixbuf *)image);
  rgb_width = gdk_pixbuf_get_width( (GdkPixbuf *)image);
  rowstride = gdk_pixbuf_get_rowstride( (GdkPixbuf *)image);
  n_channels = gdk_pixbuf_get_n_channels( (GdkPixbuf *)image);

  rgb_row = rgb;
  for ( int j = 0; j < rgb_height; j++) {
    rgb = rgb_row;
    for ( int i = 0; i < rgb_width; i++) {
      if ( n_channels >= 4) {
	if ( *(rgb+3))
	  (pixel_cb) ( userdata, fp, rgb);
	else
	  (pixel_cb) ( userdata, fp, null_rgb);
      }
      else if ( n_channels == 3)
	(pixel_cb) ( userdata, fp, rgb);
      rgb += n_channels;
    }
    rgb_row += rowstride;
  }

#elif defined  PWRE_IMLIB
  unsigned char *rgb;
  int 		rgb_height;
  int 		rgb_width;

  rgb = ((ImlibImage *)image)->rgb_data;
  rgb_height = cnv_image_height( image);
  rgb_width = cnv_image_width( image);

  for ( int i = 0; i < rgb_height * rgb_width * 3; i+=3) {
    (pixel_cb) ( userdata, fp, rgb);
    rgb += 3;
  }
#endif
}




