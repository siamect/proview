/* 
 * Proview   $Id: glow_annot.h,v 1.6 2008-10-31 12:51:35 claes Exp $
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

#ifndef glow_annot_h
#define glow_annot_h

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "glow.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowAnnot : public GlowArrayElem {
  public:
    GlowAnnot( GrowCtx *glow_ctx, double x = 0, double y = 0, 
	       int annot_num = 0, glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold,
	       glow_eDrawType color_d_type = glow_eDrawType_Line,
	       int t_size = 2, glow_eAnnotType a_type = glow_eAnnotType_OneLine, 
	       int rel_pos = 0, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1) : 
      ctx(glow_ctx), number(annot_num), p(glow_ctx,x,y), draw_type(d_type),
      text_size(t_size), annot_type(a_type), relative_pos(rel_pos),
      display_level(display_lev), color_drawtype(color_d_type),
      font(glow_eFont_Helvetica) {};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void traverse( int x, int y) { p.traverse( x, y);}

    GrowCtx *ctx;    
    int	number;
    GlowPoint p;
    glow_eDrawType draw_type;
    int	 text_size;
    glow_eAnnotType annot_type;
    int relative_pos;
    glow_mDisplayLevel display_level;
    glow_eDrawType color_drawtype;
    glow_eFont font;
};

void glow_measure_annot_text( GrowCtx *ctx, char *text, glow_eDrawType draw_type, 
			      int text_size, glow_eAnnotType annot_type, glow_eFont font,
			      double *width, double *height, int *rows);

#endif
