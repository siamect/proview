#ifndef glow_growconglue_h
#define glow_growconglue_h

#include "glow_grownode.h"
#include "glow_con.h"

/*! \file glow_growconglue.h
    \brief Contains the GrowConGlue class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing subgraph object.
/*! A GrowConGlue object is an object that is either the endpoint for a connection, or a connection object
  between a number of connections. The GrowConGlue has four connectionpoints in the directions left, right,
  up and down. The shape of the object is adjusted after the size, color and shadow of the connections.

  The GrowConGlue class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/
class GrowConGlue : public GrowNode {
 public:

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param x1		x coordinate for position.
    \param y1		y coordinate for position.
    \param nodraw	Don't draw the object now.
  */
  GrowConGlue( GlowCtx *glow_ctx, char *name, double x1 = 0, double y1 = 0,
		int nodraw = 0);

  //! Noargs constructor
  GrowConGlue() {}

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
  glow_eObjectType type() { return glow_eObjectType_GrowConGlue;};

  int                 line_width_up;	//!< Width in the up direction.
  int                 line_width_down;	//!< Width in the down direction.
  int                 line_width_left;	//!< Width in the left direction.
  int                 line_width_right; //!< Width in the right direction.
  int		      border;		//!< Draw with border.


  //! Erase the object.
  /*!
    \param t		Transform of parent node.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( GlowTransform *t, int hot, void *node);

  //! Erase the object in the navigation window.
  /*!
    \param t		Transform of parent node.
    \param node		Parent node. Can be zero.
  */
  void nav_erase( GlowTransform *t, void *node);

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

  void configure( GlowCon *con);

  //! Moves object to alignment line or point.
  /*!
    \param x	x coordinate of alignment point.
    \param y	y coordinate of alignment point.
    \param direction Type of alignment.
  */
  void align( double x, double y, glow_eAlignDirection direction);
  //! Export the object as a java bean.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param shape_cnt	Current index in a shape vector.
    \param node_cnt	Counter used for javabean name. Not used for this kind of object.
    \param in_nc	Member of a nodeclass. Not used for this kind of object.
    \param fp		Output file.

    The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
    java code for the java bean.
  */
  void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);

  //! A connected connection is modified
  /*!
    \param con		The modified connection.

    This function is called every time a connected connection is modified, so the GrowConGlue object
    can adust its layout to the connection.
  */
  void con_modified( GlowCon *con) { configure( con); draw();}

  //! Add a transform to the current transform.
  /*!
    \param t		Transform.

    The transform is multiplied to the current transform, to give the new transform for the object.
  */
  void set_transform( GlowTransform *t);

  //! Calculate the actual direction of a connectionpoint due to the rotation of the object.
  /*!
    \param point	Connection point number.
  */
  glow_eDirection conpoint_to_direction( int point);
};

/*@}*/
#endif















