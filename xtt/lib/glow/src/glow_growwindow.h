/* 
 * Proview   $Id: glow_growwindow.h,v 1.8 2008-10-31 12:51:36 claes Exp $
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

#ifndef glow_growwindow_h
#define glow_growwindow_h

#include "glow_growrect.h"
#include "glow_tracedata.h"

/*! \file glow_growwindow.h
    \brief Contains the GrowWindow class. */
/*! \addtogroup Glow */
/*@{*/

class GrowCtx;
class GrowScrollBar;


//! Class for drawing a bar object.
/*! A GrowWindow is a component that displays another graph, inside its borders.

  The GrowWindow class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/

class GrowWindow : public GrowRect {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param w		Width.
    \param h		Height.
    \param border_d_type Border color.
    \param line_w	Linewidth of border.
    \param display_lev	Displaylevel when this object is visible.
    \param nodraw	Don't draw the object now.
  */
  GrowWindow( GrowCtx *glow_ctx, const char *name, double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int nodraw = 0);

  //! Destructor
  /*! Remove the object from context, and erase it from the screen.
   */
  ~GrowWindow();


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

  //! Draw the objects if any part is inside the drawing area.
  /*!
    \param ll_x		Lower left x coordinate of drawing area.
    \param ll_y		Lower left y coordinate of drawing area.
    \param ur_x		Upper right x coordinate of drawing area.
    \param ur_y		Upper right y coordinate of drawing area.
  */
  void draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y);

  //! Draw the objects if any part is inside the drawing area, and extends the drawing area.
  /*!
    \param ll_x		Lower left x coordinate of drawing area.
    \param ll_y		Lower left y coordinate of drawing area.
    \param ur_x		Upper right x coordinate of drawing area.
    \param ur_y		Upper right y coordinate of drawing area.

    If some part of object is inside the drawing area, and also outside the drawing area,
    the drawingarea is extended so it contains the whole objects.
  */
  void draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y);

  //! Set object highlight.
  /*!
    \param on	If 1, set highlight. If 0, reset highlight.
  */
  void set_highlight( int on);

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_GrowWindow;};

  char			file_name[80];		//!< Graph file name.
  char			input_file_name[80];	//!< Filename inserted in attribute editor.
  GlowTraceData		trace;			//!< Obsolete
  void 			*user_data;		//!< User data.
  GrowCtx		*window_ctx;		//!< Window grow context.  
  int			vertical_scrollbar;	//!< Draw vertical scrollbar.
  int			horizontal_scrollbar;	//!< Draw horizontal scrollbar.
  double		scrollbar_width;	//!< Width of scrollbar objects.
  GrowScrollBar		*v_scrollbar;		//!< Vertical scrollbar object.
  GrowScrollBar		*h_scrollbar;		//!< Horizontal scrollbar object.
  double		v_value;		//!< Value of vertical scrollbar.
  double		h_value;		//!< Value of horizontal scrollbar.
  double		wctx_x0;		//!< Coordinate for left border of window ctx.
  double		wctx_x1;		//!< Coordinate for right border of window ctx.
  double		wctx_y0;		//!< Coordinate for low border of window ctx.
  double		wctx_y1;		//!< Coordiante for high border of window ctx.
  glow_eDrawType	scrollbar_color;	//!< Color of scrollbar bar.
  glow_eDrawType	scrollbar_bg_color;	//!< Color of scrollbar background.
  double		window_scale;		//!< Scale of window ctx.
  double		y_low_offs;		//!< y low offset.
  int			input_focus;		//!< Window has input focus.
  char			owner[256];		//!< Window owner.

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
  void draw();

  //! Draw the object in the navigation window.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param highlight	Draw with highlight colors.
    \param node		Parent node. Can be zero.
    \param colornode	The node that controls the color of the object. Can be zero.
  */
  void draw_brief( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, void *colornode);

  //! Scan trace
  /*! Calls the trace scan callback for the object.
   */
  void trace_scan();

  //! Init trace
  /*! Calls the trace connect callback for the object.
   */
  int trace_init();

  //! Close trace
  /*! Calls the trace disconnect callback for the object.
   */
  void trace_close();

  //! Moves object to alignment line or point.
  /*!
    \param x	x coordinate of alignment point.
    \param y	y coordinate of alignment point.
    \param direction Type of alignment.
  */
  void align( double x, double y, glow_eAlignDirection direction);

  //! Set the range for the bar value
  /*!
    \param min		Min value.
    \param max		Max value.
  */

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

  int event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
		     double fy);

  //! Check if new filename
  void update_attributes();

  //! Create a new grow context
  void new_ctx();
  
  // Configure the scrollbars
  void configure_scrollbars();

  void set_transform_from_stored( GlowTransform *t);
  void set_fill( int fillval) {}
  void set_fill_color( glow_eDrawType drawtype) {}
  void set_original_fill_color( glow_eDrawType drawtype) {}
  void set_rotation( double angle, double x0, double y0, 
		glow_eRotationPoint type) {}
  void flip( double x0, double y0, glow_eFlipDirection dir) {}
  void set_transform( GlowTransform *t) {}
  void draw_background();
  void set_shadow( int shadowval) { shadow = shadowval; configure_scrollbars(); draw();}
  void zoom();
  int get_background_object_limits(GlowTransform *t,
				   glow_eTraceType type,
				   double x, double y, GlowArrayElem **background,
				   double *min, double *max, glow_eDirection *direction);
  void set_input_focus( int focus, glow_eEvent event);
  int set_source( char *source);

  static void v_value_changed_cb( void *o, double value);
  static void h_value_changed_cb( void *o, double value);
  static void redraw_cb( void *o);

};


/*@}*/
#endif









