/* 
 * Proview   $Id: cnv_image.cpp,v 1.3 2005-09-01 14:57:47 claes Exp $
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


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "cnv_image.h"


static ImlibData *imlib = 0;
static Display *display = 0;


int cnv_get_image( char *fname, ImlibImage **image, Pixmap *pixmap)
{
#if defined IMLIB
  if ( !imlib) {
    display = XOpenDisplay(NULL);
    imlib = Imlib_init( display);
  }
#endif

  *image = Imlib_load_image( imlib, fname);
  if ( !*image)
    return 0;
  *pixmap = Imlib_move_image( imlib, *image);

  return 1;
}

void cnv_free_image( ImlibImage *image, Pixmap pixmap)
{
  Imlib_free_pixmap( imlib, pixmap);
  Imlib_destroy_image( imlib, image);
}

void cnv_print_image( ImlibImage *image, char *filename)
{
  Imlib_save_image( imlib, image, filename, 0);
}






