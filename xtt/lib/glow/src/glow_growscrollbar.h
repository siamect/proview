/* 
 * Proview   $Id: glow_growscrollbar.h,v 1.5 2008-10-31 12:51:35 claes Exp $
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

#ifndef glow_growscrollbar_h
#define glow_growscrollbar_h

#include "glow_growrect.h"
#include "glow_tracedata.h"

/*! \file glow_growscrollbar.h
    \brief Contains the GrowScrollBar class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for handling a scrollbar object.
/*! A GrowScrollBar object makes it possible to change the viewed area in a window object.

  The GrowScrollBar class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/

class GrowScrollBar : public GrowRect {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param w		Width.
    \param h		Height.
    \param dir		Direction.
    \param border_d_type Border color.
    \param line_w	Linewidth of border.
    \param display_lev	Displaylevel when this object is visible.
    \param fill_d_type	Fill color.
    \param bar_d_type	Bar color.
    \param nodraw	Don't draw the object now.
  */
  GrowScrollBar( GrowCtx *glow_ctx, const char *name, double x = 0, double y = 0, 
		 double w = 0, double h = 0, glow_eDir dir = glow_eDir_Vertical,
		 glow_eDrawType border_d_type = glow_eDrawType_Line, 
		 int line_w = 1,
		 glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		 glow_eDrawType fill_d_type = glow_eDrawType_Line, 
		 glow_eDrawType bar_d_type = glow_eDrawType_LightGray, int nodraw = 0);

  //! Destructor
  /*! Remove the object from context, and erase it from the screen.
   */
  ~GrowScrollBar();


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

  //! Erase the object
  void erase( GlowWind *w)
	{ erase( w, (GlowTransform *)NULL, hot, NULL);};

  //! Set object highlight.
  /*!
    \param on	If 1, set highlight. If 0, reset highlight.
  */
  void set_highlight( int on);

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_GrowScrollBar;};

  double		max_value;		//!< Max value for bar value
  double		min_value;		//!< Min value for bar value
  double		bar_value;		//!< Bar value
  double		bar_length;		//!< Bar length
  void 			*user_data;		//!< User data.
  glow_eDir		direction;		//!< Vertical or horizontal direction.
  int			movement_active;	//!< The scrollbar i currently moved.
  double		start_pos;		//!< Cursor start position for movement.
  double		start_value;		//!< Bar value when movement is started.
  void			*callback_userdata;	//!< Userdata in value change callback.
  void			(*value_changed_cb)( void *, double); //!< Callback when value is changed
  glow_eDrawType	bar_color;		//!< Color of bar.

  void register_value_changed_cb( void *userdata, void (*value_changed)(void *, double)) {
    callback_userdata = userdata, value_changed_cb = value_changed; }

  int event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
		     double fy);


  //! Draw the object.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
    \param colornode	The node that controls the color of the object. Can be zero.

    The object is drawn with border, fill and shadow. If t is not zero, the current tranform is
    multiplied with the parentnodes transform, to give the appropriate coordinates for the drawing.
  */
  void draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, void *colornode);

  //! Erase the object.
  /*!
    \param t		Transform of parent node.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( GlowWind *w, GlowTransform *t, int hot, void *node);

  //! Redraw the area inside the objects border.
  void draw() { draw( &ctx->mw,0,0,0,0,0);}

  //! Set the bar value
  /*!
    \param value	Bar value.
    \param length	Bar length.
    \return		The actual new bar value.
  */
  double set_value( double value, double length = 0);

  //! Set the range for the bar value
  /*!
    \param min		Min value.
    \param max		Max value.
  */
  void set_range( double min, double max);

  //! Set user data.
  /*!
    \param data User data.
  */
  void set_user_data( void *data) { user_data = data;};

  //! Get user data.
  /*!
    \param data User data.
  */
  void get_user_data( void **data) { *data = user_data;};

  //! Export the object as a javabean.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param shape_cnt	Current index in a shape vector.
    \param node_cnt	Counter used for javabean name. Not used for this kind of object.
    \param in_nc	Member of a nodeclass. Not used for this kind of object.
    \param fp		Output file.

    The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
    java code for the bean.
  */
  void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc,  ofstream &fp);


  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  void set_position( double x, double y, double width, double height) 
    { ll.posit(x,y); ur.posit(x+width,y+height);}

  void set_colors( glow_eDrawType bg_color, glow_eDrawType b_color)
    { fill_drawtype = bg_color; bar_color = b_color;}

  void set_shadow( int shadowval) { shadow = shadowval;}
};


/*@}*/
#endif









