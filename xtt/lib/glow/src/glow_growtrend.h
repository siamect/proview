#ifndef glow_growtrend_h
#define glow_growtrend_h

#include "glow_growrect.h"
#include "glow_growpolyline.h"
#include "glow_tracedata.h"

/*! \file glow_growtrend.h
    \brief Contains the GrowTrend class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing a trend object.
/*! A GrowTrend object is an object that draws a number of trend curves, where the x-axis is the 
  time dimension, and the y-axis displayes the trend value of a parameter. New values are added as time
  passes. The curves are drawn filled or unfilled.

  The GrowTrend class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/

class GrowTrend : public GrowRect {
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
    \param fill_rect	Rectangle is filled.
    \param display_border Border is visible.
    \param fill_d_type	Fill color.
    \param nodraw	Don't draw the object now.
  */
  GrowTrend( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
		glow_eDrawType fill_d_type = glow_eDrawType_Line, int nodraw = 0);

  //! Destructor
  /*! Remove the object from context, and erase it from the screen.
   */
  ~GrowTrend();

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
  void erase()
	{ erase( (GlowTransform *)NULL, hot, NULL);};

  //! Erase the object in the navigator window.
  void nav_erase()
	{ nav_erase( (GlowTransform *)NULL, NULL);};

  //! Draw the objects if any part is inside the drawing area.
  /*!
    \param ll_x		Lower left x coordinate of drawing area.
    \param ll_y		Lower left y coordinate of drawing area.
    \param ur_x		Upper right x coordinate of drawing area.
    \param ur_y		Upper right y coordinate of drawing area.
  */
  void draw( int ll_x, int ll_y, int ur_x, int ur_y);

  //! Draw the objects if any part is inside the drawing area, and extends the drawing area.
  /*!
    \param ll_x		Lower left x coordinate of drawing area.
    \param ll_y		Lower left y coordinate of drawing area.
    \param ur_x		Upper right x coordinate of drawing area.
    \param ur_y		Upper right y coordinate of drawing area.

    If some part of object is inside the drawing area, and also outside the drawing area,
    the drawingarea is extended so it contains the whole objects.
  */
  void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);

  //! Drawing in the navigation window. See the corresponding draw function.
  void nav_draw(int ll_x, int ll_y, int ur_x, int ur_y);

  //! Set object highlight.
  /*!
    \param on	If 1, set highlight. If 0, reset highlight.
  */
  void set_highlight( int on);

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_GrowTrend;};

  void set_trace_attr( GlowTraceData *attr);	//!< Obsolete
  void get_trace_attr( GlowTraceData **attr);	//!< Obsolete

  //! Set number of verticals and horizontal lines.
  /*!
    \param v	Number of vertical lines.
    \param h	Number of horizontal lines.
  */
  void set_lines( int v, int h) { vertical_lines = v; horizontal_lines = h;};

  double		max_value[TREND_MAX_CURVES];	//!< Max values of the curves.
  double		min_value[TREND_MAX_CURVES];	//!< Min values of the curves.
  int			horizontal_lines;		//!< Number of horizontal lines.
  int			vertical_lines;			//!< Number of vertical lines.
  int			fill_curve;			//!< The curves are filled.
  int			no_of_points;			//!< Number of points in each curve.
  int			curve_width;			//!< Line width of the curves.
  glow_eDrawType	curve_drawtype[TREND_MAX_CURVES]; //!< Color of the curves.
  glow_eDrawType	curve_fill_drawtype[TREND_MAX_CURVES]; //!< Fill color of the curves.
  GlowTraceData		trace;				//!< Obsolete
  GrowPolyLine		*curve[TREND_MAX_CURVES];	//!< The polyline object for the curves.
  int			curve_cnt;			//!< Number of curves.
  double		scan_time;			//!< Scantime. Time interval between two points.
  void 			*user_data;			//!< User data.

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
  void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);

  //! Erase the object.
  /*!
    \param t		Transform of parent node.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( GlowTransform *t, int hot, void *node);

  //! Redraw the area inside the objects border.
  void draw();

  //! Draw the object in the navigation window.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param highlight	Draw with highlight colors.
    \param node		Parent node. Can be zero.
    \param colornode	The node that controls the color of the object. Can be zero.
  */
  void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);

  //! Erase the object in the navigation window.
  /*!
    \param t		Transform of parent node.
    \param node		Parent node. Can be zero.
  */
  void nav_erase( GlowTransform *t, void *node);

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

  //! Add a new value to the specified curve
  /*!
    \param value	New value.
    \param idx		Curve number.

    Add the new value first in the curve, and shift the other values one step forward.
  */
  void add_value( double value, int idx);

  //! Configure the curves
  /*! Calculate position of the points of the curves and create a polyline for each curve.
   */
  void configure_curves();

  //! Moves object to alignment line or point.
  /*!
    \param x	x coordinate of alignment point.
    \param y	y coordinate of alignment point.
    \param direction Type of alignment.
  */
  void align( double x, double y, glow_eAlignDirection direction);

  //! Set scantime
  /*!
    \param time		Scantime in seconds.
  */
  void set_scan_time( double time);

  //! Get scantime
  /*!
    \param time		Scantime in seconds.
  */
  void get_scan_time( double *time) { *time = scan_time;};

  //! Set the range for the specified curve.
  /*!
    \param curve	Number of curve.
    \param min		Min value.
    \param max		Max value.
  */
  void set_range( int curve, double min, double max);

  //! Set fill for curves.
  /*!
    \param fill		Curves is drawn with fill.
  */
  void set_fill_curve( int fill) { fill_curve = fill;};

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

  //! Set parameters for the trend.
  /*!
    \param info		Info struct.
  */
  void set_trend_info( glow_sTrendInfo *info);

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
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);

  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  void set_data( double *data[3], int data_curves, int data_points);
};


/*@}*/
#endif


