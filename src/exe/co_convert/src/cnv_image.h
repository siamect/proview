/* 
 * Proview   $Id: cnv_image.h,v 1.5 2008-10-31 12:51:30 claes Exp $
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

#include <fstream>
#include <stdlib.h>

using namespace std;

typedef void *cnv_tImImage;
typedef void *cnv_tPixmap;

int cnv_get_image( char *fname, cnv_tImImage *image, cnv_tPixmap *pixmap);
void cnv_free_image( cnv_tImImage image, cnv_tPixmap pixmap);
void cnv_print_image( cnv_tImImage image, char *filename);
int cnv_image_width( cnv_tImImage image);
int cnv_image_height( cnv_tImImage image);
void cnv_image_pixel_iter( cnv_tImImage image, 
			   void (* pixel_cb)(void *, ofstream&, unsigned char *), 
			   void *userdata, ofstream& fp);

#endif
