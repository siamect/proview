#ifndef glow_arc_h
#define glow_arc_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"

/*! \file glow_arc.h
    \brief Contains the GlowArc class. */
/*! \addtogroup Glow */
/*@{*/


//! Base class for a arc.
/*! The full implementation of an arc is in the GrowArc class. 
  The GlowArc class is still used by GlowCon, when drawing connections.
*/

class GlowArc : public GlowArrayElem {
  public:

  //! Noargs constructor.
  GlowArc() {};

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param x1		x coordinate for first corner of a rectangle that surrouds the elipse of the arc.
    \param y1		y coordinate for first corner of a rectangle that surrouds the elipse of the arc.
    \param x2		x coordinate for second corner of a rectangle that surrouds the elipse of the arc.
    \param y2		y coordinate for second corner of a rectangle that surrouds the elipse of the arc.
    \param ang1		Start angel of the arc in degrees from the x axis.
    \param ang2		Lengh of the arc in degrees from the start angel.
    \param d_type 	Border color.
    \param line_w	Linewidth of border.
    \param fill_arc	Arc is filled.
  */
  GlowArc( GlowCtx *glow_ctx, double x1 = 0, double y1 = 0, double x2 = 0, 
		double y2 = 0, int ang1 = 0, int ang2 = 0, 
		glow_eDrawType d_type = glow_eDrawType_Line, int line_w = 1,
		int fill_arc = 0) : 
	ctx(glow_ctx), angel1(ang1), angel2(ang2),
	ll(glow_ctx,x1,y1), ur(glow_ctx,x2,y2),
	draw_type(d_type), line_width(line_w), fill(fill_arc)
      {}

  friend ostream& operator<< ( ostream& o, const GlowArc a);

  //! Adjust pixel coordinates to current zoom factor.
  void zoom();

  //! Adjust pixel coordinates for navigaion window to current zoom factor.
  void nav_zoom();

  void print_zoom();		//!< Not used.
  void traverse( int x, int y);	//!< Not used.

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
  int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);

  //! Not implemented
  void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};

  //! Print postscript. Not used.
  void print( void *pos, void *node);

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
  void draw( void *pos, int highlight, int hot, void *node);

  //! Draw border and shadow of the arc.
  /*!
    \param border	Draw border.
    \param shadow	Draw shadow.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.

    Draw border and shadow of the object. The border always has color black and linewidth 1 pixel.
    The shadow also always has linewith 1 pixel.
  */
  void draw_shadow( int border, int shadow, int highlight, int hot);

  //! Draw the object in the navigation window.
  /*!
    \param pos		Position of object. Should be zero.
    \param highlight	Draw with highlight colors.
    \param node		Parent node. Can be zero.

    Draw the object, without borders or shadow.
  */
  void nav_draw( void *pos, int highlight, void *node);

  //! Erase the object.
  /*!
    \param pos		Position of object. Should be zero.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( void *pos, int hot, void *node);

  //! Not used
  void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};

  //! Erase the object in the navigation window.
  /*!
    \param pos		Position of object. Should be zero.
    \param node		Parent node. Can be zero.
  */
  void nav_erase( void *pos, void *node);

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

  //! Move the arc to the specified coordinates.
  /*!
    \param pos		Position. Should be zero.
    \param x1		x coordinate of first point.
    \param y1		y coordinate of first point.
    \param x2		x coordinate of second point.
    \param y2		y coordinate of second point.
    \param ang1		Start angel in degrees.
    \param ang2		Length of arc in degrees.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.

    Both points are given new coordinates, so the direction and length of the arc can 
    be entirely different.
  */
  void move( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot);

  //! Move the arc to the specified coordinates without erase.
  /*!
    \param pos		Position. Should be zero.
    \param x1		x coordinate of first point.
    \param y1		y coordinate of first point.
    \param x2		x coordinate of second point.
    \param y2		y coordinate of second point.
    \param ang1		Start angel in degrees.
    \param ang2		Length of arc in degrees.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.

    Both points are given new coordinates, so the direction and length of the arc can 
    be entirely different.
  */
  void move_noerase( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot);

  //! Move the arc.
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
  void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};

  //! Set the linewidth.
  /*!
    \param linewidth	Linewidth in range 0 to 8. 0 gives a linewidth of 1 pixel at original zoom. 1 -> 2 pixel etc.
  */
  void set_linewidth( int linewidth) {line_width = linewidth;};

  //! Set fill.
  /*!
    \param fillval		Draw the object with fill.
  */
  void set_fill( int fillval) { fill = fillval;};

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_Arc;};

  //! Export the object as a java shape.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param shape_cnt	Current index in a shape vector.
    \param node_cnt	Counter used for javabean name. Not used for this kind of object.
    \param in_nc	Member of a nodeclass. Not used for this kind of object.
    \param fp		Output file.

    The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
    java code for the shape.
  */
  void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);

  //! Export the shadow of the object as a java shape.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param shape_cnt	Current index in a shape vector.
    \param node_cnt	Counter used for javabean name. Not used for this kind of object.
    \param in_nc	Member of a nodeclass. Not used for this kind of object.
    \param fp		Output file.
    \param border	The object is drawn with border.
    \param shadow	The object is drawn with shadow.

    The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
    java code for the shape.
  */
  void export_javabean_shadow( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp, int border, int shadow);

  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  GlowCtx *ctx;    	//!< Glow context.
  int	      angel1;	//!< Start angel or arc from x-axis in degrees.
  int	      angel2;	//!< Length of arc in degrees.
  GlowPoint ll;		//!< Lower left corner of rectangle that surroundes the elipse of the arc.
  GlowPoint ur;		//!< Upper right corner of rectangel that surroundes the elipse of the arc.
  glow_eDrawType draw_type; //!< Border color.
  int	 line_width;	//!< Line width of border.
  int fill;		//!< Fill the object.
};

/*@}*/
#endif



