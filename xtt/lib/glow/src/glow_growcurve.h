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

