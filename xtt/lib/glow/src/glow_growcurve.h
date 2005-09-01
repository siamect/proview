/* 
 * Proview   $Id: glow_growcurve.h,v 1.3 2005-09-01 14:57:53 claes Exp $
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

#ifndef glow_growcurve_h
#define glow_growcurve_h

#include "glow_growtrend.h"


/*! \file glow_growcurve.h
    \brief Contains the GrowCurve class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing curves in a trend window.
/*! A GrowTrend object is an object that draws a number of trend curves, where the x-axis is the 
  time dimension, and the y-axis displayes the trend value of a parameter. New values are added as time
  passes. The curves are drawn filled or unfilled.
*/

class GrowCurve : public GrowTrend {
 public:

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param data		Initial data for the curves.
    \param name		Name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param w		Width.
    \param h		Height.
    \param border_d_type Border color.
    \param line_w	Linewidth of border.
    \param display_lev	Displaylevel when this object is visible.
    \param fill_rect	Rectangle is filled.
    \param display_border Border is visible.
    \param fill_d_type	Fill color.
    \param nodraw	Don't draw the object now.
  */
  GrowCurve( GlowCtx *glow_ctx, char *name, glow_sCurveData *data,
                double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
	        glow_eDrawType fill_d_type = glow_eDrawType_Line, 
	        int nodraw = 0);

  //! Configure the curves
  /*!
    \param data		Configuration data for the curves.

    Calculate position of the points of the curves and create a polyline for each curve.
  */
  void configure_curves( glow_sCurveData *data);

  //! Add a new value to the specified curve
  /*!
    \param data		Data for the new values of the curves.

    Add the new value first in all the curves, and shift the other values one step forward.
  */
  void add_points( glow_sCurveData *data);
};


/*@}*/
#endif

