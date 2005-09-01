/* 
 * Proview   $Id: cnv_image.h,v 1.3 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_image_h
#define cnv_image_h

#include <stdlib.h>

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#if defined OS_LINUX
#define IMLIB 1
#endif

#if defined IMLIB
# if defined OS_LYNX
#  define __NO_INCLUDE_WARN__ 1
# endif
# include <X11/extensions/shape.h>
# include <Imlib.h>
# if defined OS_LYNX
#  undef __NO_INCLUDE_WARN__
# endif
#else
typedef void *ImlibData;
typedef void *ImlibImage;
#endif

int cnv_get_image( char *fname, ImlibImage **image, Pixmap *pixmap);
void cnv_free_image( ImlibImage *image, Pixmap pixmap);
void cnv_print_image( ImlibImage *image, char *filename);

#endif
