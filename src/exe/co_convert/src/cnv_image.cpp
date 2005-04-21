
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






