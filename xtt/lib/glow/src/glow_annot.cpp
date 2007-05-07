/* 
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


#include <iostream.h>
#include <math.h>

#include "glow_annot.h"
#include "glow_node.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_browctx.h"

void GlowAnnot::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Annot) << endl;
  fp << int(glow_eSave_Annot_number) << FSPACE << number << endl;
  fp << int(glow_eSave_Annot_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Annot_color_drawtype) << FSPACE << int(color_drawtype) << endl;
  fp << int(glow_eSave_Annot_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_Annot_display_level) << FSPACE << int(display_level) << endl;
  fp << int(glow_eSave_Annot_p) << endl;
  p.save( fp, mode);
  fp << int(glow_eSave_Annot_annot_type) << FSPACE << int(annot_type) << endl;
  fp << int(glow_eSave_Annot_font) << FSPACE << int(font) << endl;
  fp << int(glow_eSave_End) << endl;
}

void GlowAnnot::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Annot: break;
      case glow_eSave_Annot_number: fp >> number; break;
      case glow_eSave_Annot_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Annot_color_drawtype: fp >> tmp; color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_Annot_text_size: fp >> text_size; break;
      case glow_eSave_Annot_display_level: fp >> tmp; display_level = (glow_mDisplayLevel)tmp; break;
      case glow_eSave_Annot_p: p.open( fp); break;
      case glow_eSave_Annot_annot_type: fp >> tmp; 
	annot_type = (glow_eAnnotType)tmp; break;
      case glow_eSave_Annot_font: fp >> tmp; 
	font = (glow_eFont)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowAnnot:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}


void glow_measure_annot_text( GrowCtx *ctx, char *text, glow_eDrawType draw_type, 
			      int text_size, glow_eAnnotType annot_type, glow_eFont font,
			      double *width, double *height, int *rows)
{
  int	z_width, z_height, z_descent;

  switch ( annot_type) {
    case glow_eAnnotType_OneLine:
      ctx->gdraw->get_text_extent( text, strlen(text), draw_type, text_size, 
				   font, &z_width, &z_height, &z_descent);
      *rows = 1;
      break;
    case glow_eAnnotType_MultiLine:
    {
      int l_width, l_height, l_descent;
      int len = 0;
      int line_cnt = 0;
      char *line = text;
      char *s;
      z_width = 0;
      for ( s = text; *s; s++)
      {
        if ( *s == 10)
	{
	  if ( len)
	  {
            ctx->gdraw->get_text_extent( line, len, draw_type, text_size, font, 
					 &l_width, &l_height, &l_descent);
	    z_width = max( z_width, l_width);
	  }
	  len = 0;
	  line = s+1;
	  line_cnt++;
	}
	else
	  len++;
      }
      if ( len)
      {
        ctx->gdraw->get_text_extent( line, len, draw_type, text_size, 
				     font, &l_width, &l_height, &l_descent);
	z_width = max( z_width, l_width);
        line_cnt++;
      }
      z_height = line_cnt * l_height;      
      *rows = max( line_cnt, 1);
      break;
    }
  }
  *width = 1.0 * z_width / ctx->mw.base_zoom_factor;
  *height = 1.0 * z_height / ctx->mw.base_zoom_factor;
}

