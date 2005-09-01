/* 
 * Proview   $Id: glow_annot.h,v 1.3 2005-09-01 14:57:53 claes Exp $
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
#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowAnnot : public GlowArrayElem {
  public:
    GlowAnnot( GlowCtx *glow_ctx, double x = 0, double y = 0, 
	int annot_num = 0, glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold,
	glow_eDrawType color_d_type = glow_eDrawType_Line,
	int t_size = 2, glow_eAnnotType a_type = glow_eAnnotType_OneLine, 
	int rel_pos = 0, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1) : 
	ctx(glow_ctx), number(annot_num), p(glow_ctx,x,y), draw_type(d_type),
	text_size(t_size), annot_type(a_type), relative_pos(rel_pos),
	display_level(display_lev), color_drawtype(color_d_type) {};
    friend ostream& operator<< ( ostream& o, const GlowAnnot cp);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);
    void print( void *pos, void *node);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_Annot;};
    void measure_annot( char *text, double *width, double *height);
    void configure_annotations( void *pos, void *node);
    void open_annotation_input( void *pos, void *node);
    void close_annotation_input( void *node);
    int get_annotation_input( void *node, char **text);
    void move_widgets( void *node, int x, int y);
    GlowCtx *ctx;    
    int	number;
    GlowPoint p;
    glow_eDrawType draw_type;
    int	 text_size;
    glow_eAnnotType annot_type;
    int relative_pos;
    glow_mDisplayLevel display_level;
    glow_eDrawType color_drawtype;
};

void glow_measure_annot_text( GlowCtx *ctx, char *text, glow_eDrawType draw_type, 
		int text_size, glow_eAnnotType annot_type, 
		double *width, double *height, int *rows);

#endif
