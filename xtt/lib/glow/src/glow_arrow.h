/* 
 * Proview   $Id: glow_arrow.h,v 1.5 2008-10-31 12:51:35 claes Exp $
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

#ifndef glow_arrow_h
#define glow_arrow_h

#include <stdlib.h>
#include <iostream>
#include "glow.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


/*! \file glow_arrow.h
    \brief Contains the GlowArrow class. */
/*! \addtogroup Glow */
/*@{*/


//! Base class for an arrow.
/*! The GlowArrow class is used by GlowCon, when drawing arrows in connections.
*/

class GlowArrow : public GlowArrayElem {
 public:

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param x1		x coordinate for first point.
    \param y1		y coordinate for first point.
    \param x2		x coordinate for second point.
    \param y2		y coordinate for second point.
    \param w		Arrow width.
    \param l		Arrow length.
    \param d_type 	Color.
  */
  GlowArrow( GrowCtx *glow_ctx, double x1, double y1, double x2, 
		double y2, double w, double l,
	   	glow_eDrawType d_type = glow_eDrawType_Line);

  //! Adjust pixel coordinates to current zoom factor.
  void zoom();

  //! Adjust pixel coordinates for navigaion window to current zoom factor.
  void nav_zoom();

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
		double *x_left, double *y_high, double *y_low, void *node) 
		{};

  int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_Arrow;};

  //! Move the arrow to the specified coordinates.
  /*!
    \param pos		Position. Should be zero.
    \param x1		x coordinate of first point.
    \param y1		y coordinate of first point.
    \param x2		x coordinate of second point.
    \param y2		y coordinate of second point.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.

    Both endpoints are given new coordinates, so the direction can be entirely different.
  */
  void move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot);

  //! Move the arrow.
  /*!
    \param pos		Position. Should be zero.
    \param delta_x	Moved distance in x direction.
    \param delta_y	Moved distance in y direction.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
  */
  void shift( void *pos, double delta_x, double delta_y, 
	int highlight, int hot);

  //! Set the color.
  /*!
    \param drawtype	Color.
  */
  void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};

  GrowCtx *ctx;    	//!< Glow context.
  GlowPoint p_dest;	//!< Point of destination corner of arrow.
  GlowPoint p1;		//!< Point of first corner.
  GlowPoint p2;		//!< Point of second corner.
  double arrow_width;	//!< Arrow width.
  double arrow_length;	//!< Arrow lenght.
  glow_eDrawType draw_type; //!< Arrow color.
  int	line_width;	//!< Line width.
};

/*@}*/
#endif
