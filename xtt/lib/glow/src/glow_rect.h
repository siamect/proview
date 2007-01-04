/* 
 * Proview   $Id: glow_rect.h,v 1.4 2007-01-04 07:57:39 claes Exp $
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

#ifndef glow_rect_h
#define glow_rect_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"

/*! \file glow_rect.h
    \brief Contains the GlowRect class. */
/*! \addtogroup Glow */
/*@{*/


//! Base class for a rectangle.
/*! The full implementation of a rectangle is in the GrowRect class. The GlowRect class is still used in
  GlowCon for drawing reference connections.
*/

class GlowRect : public GlowArrayElem {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param w		Width.
    \param h		Height.
    \param d_type 	Border color.
    \param line_w	Linewidth of border.
    \param fix_line_w	Linewidth independent of scale.
    \param display_lev	Displaylevel when this object is visible.
    \param fill_rect	Rectangle is filled.
  */
  GlowRect( GrowCtx *glow_ctx, double x = 0, double y = 0, double w = 0, 
		double h = 0, glow_eDrawType d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0) : 
	ctx(glow_ctx), ll(glow_ctx,x,y), ur(glow_ctx,x+w,y+h), 
	draw_type(d_type), line_width(line_w), fix_line_width(fix_line_w),
	display_level(display_lev), fill(fill_rect) {};

  //! Adjust pixel coordinates to current zoom factor.
  void zoom();

  //! Adjust pixel coordinates for navigaion window to current zoom factor.
  void nav_zoom();

  void print_zoom();		//!< Not used
  void traverse( int x, int y); //!< Not used

  //! Event handler
  /*!
    \param pos		Position of object. Should be zero.
    \param event	Current event.
    \param x		x coordinate of event.
    \param y		y coordinate of event.
    \param node		Parent node. Can be zero.
    \return		Returns 1 if the object is hit, else 0.

    Detects if the object is hit by the event.
  */
  int	event_handler( GlowWind *w, void *pos, glow_eEvent event, int x, int y, void *node);

  //! Not implemented
  void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};

  //! Save the content of the object to file.
  /*!
    \param fp	Ouput file.
    \param mode	Not used.
  */
  void save( ofstream& fp, glow_eSaveMode mode);

  //! Read the content of the object from file.
  /*!
    \param fp	Input file.
  */
  void open( ifstream& fp);

  //! Draw the object.
  /*!
    \param pos		Position of object. Should be zero.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.

    Draw the object, without borders or shadow.
  */
  void draw( GlowWind *w, void *pos, int highlight, int hot, void *node);

  //! Erase the object.
  /*!
    \param pos		Position of object. Should be zero.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( GlowWind *w, void *pos, int hot, void *node);

  //! Calculate the border for a set of objects or for a parent node.
  /*!
    \param pos_x        x coordinate for position.
    \param pos_y        y coordinate for position.
    \param x_right	Right limit.
    \param x_left	Left limit.
    \param y_high	High limit.
    \param y_low	Low limit.
    \param node		Parent node. Can be zero.
    
    If the borders of the objects exceeds a limit, the limit is adjusted to the
    border of the object.
  */
  void get_borders( double pos_x, double pos_y, double *x_right, 
		    double *x_left, double *y_high, double *y_low, void *node);

  //! Move the rectangle to the specified coordinates.
  /*!
    \param pos		Position. Should be zero.
    \param x		x coordinate of first point.
    \param y		y coordinate of first point.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
  */
  void move( void *pos, double x, double y, int highlight, int hot);

  //! Move the rectangle.
  /*!
    \param pos		Position. Should be zero.
    \param delta_x	Moved distance in x direction.
    \param delta_y	Moved distance in y direction.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
  */
  void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
  int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_Rect;};

  GrowCtx *ctx;    //!< Glow context

  //! Get the width.
  /*!
    \return The width of the rectangle.
  */
  double width() { return ur.x - ll.x;};

  //! Get the height.
  /*!
    \return The height of the rectangle.
  */
  double height() { return ur.y - ll.y;};


  //! Set the linewidth.
  /*!
    \param linewidth	Linewidth in range 0 to 8. 0 gives a linewidth of 1 pixel at original zoom. 1 -> 2 pixel etc.
  */
  void set_linewidth( int linewidth) {line_width = linewidth;};

  //! Set fill.
  /*!
    \param fillval	If 1 the object will be draw with fill, if 0 the object will be drawn without fill.
  */
  void set_fill( int fillval) { fill = fillval;};

  GlowPoint 	ll;		//!< Lower left point of rectangle.
  GlowPoint 	ur;		//!< Upper right point of rectangle.
  glow_eDrawType draw_type;	//!< Border color.
  int		line_width;	//!< Line width.
  int  		fix_line_width;	//!< Linewidth is independent of zoom factor.
  glow_mDisplayLevel display_level; //!< Displaylevel when the objects is visible.
  int fill;			//!< The rectangel is filled.
};

/*@}*/
#endif





