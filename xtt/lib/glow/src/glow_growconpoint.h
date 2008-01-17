/* 
 * Proview   $Id: glow_growconpoint.h,v 1.5 2008-01-17 14:17:05 claes Exp $
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

#ifndef glow_growconpoint_h
#define glow_growconpoint_h

#include "glow_conpoint.h"
#include "glow_arc.h"

class GlowExportFlow;

/*! \file glow_growconpoint.h
    \brief Contains the GrowConPoint class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for a connection point in a GrowNode.
/*! This class displays the connection point at edit time. When saved to file (.pwsg) it is
  saved as a GlowConPoint object.
*/
class GrowConPoint : public GlowConPoint {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param cp_num  	Conpoint number.
    \param d 		Conpoint direction.
    \param nodraw	Don't draw the object now.
  */
  GrowConPoint( GrowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
		int cp_num = 0, glow_eDirection d = glow_eDirection_Center,
		int nodraw =0);

  //! Destructor
  /*! Removes the object from the context's arrays, and erases it from the screen 
   */
  ~GrowConPoint();

  //! Event handler
  /*!
    \param event	Current event.
    \param x		Not used.
    \param y		Not used.
    \param fx		x coordinate of event.
    \param fy		y coordinate of event.
    \return		Returns 1 if the object is hit, else 0.

    Detects if the objet is hit by the event, and if it is, performes the appropriate
    action: changes the cursor, draws the object hot, and registers the object as
    current callback object.
  */
  int	event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx, double fy);
  
  //! Calculate the border for a set of objects or for a parent node.
  /*!
    \param t		Transform.
    \param x_right	Right limit.
    \param x_left	Left limit.
    \param y_high	High limit.
    \param y_low	Low limit.
    
    If the borders of the objects exceeds a limit, the limit is adjusted to the
    border of the object.
  */
  void get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low);

  //! Calculate the border for a set of objects, without transformation.
  /*!
    \param x1_right	Right limit.
    \param x1_left	Left limit.
    \param y1_high	High limit.
    \param y1_low	Low limit.
    
    If the borders of the objects exceeds a limit, the limit is adjusted to the
    border of the object.
  */
  void get_borders(
	double *x1_right, double *x1_left, double *y1_high, double *y1_low)
	{ get_borders( (GlowTransform *)NULL, x1_right, x1_left, y1_high, y1_low);};

  //! Get the borders of the object.
  /*!
    \param t		Transform.
    \param x_right	Right limit.
    \param x_left	Left limit.
    \param y_high	High limit.
    \param y_low	Low limit.
    
    Calls the get_border function for the arc object.
  */
  void get_node_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low);

  //! Update the borders of the object.
  /*! The borders of the object is stored in x_right, x_left, y_high and y_low.
   */
  void get_node_borders()
	{ x_left = y_low = 1e37; x_right = y_high = -1e37;
	  get_node_borders( (GlowTransform *)NULL, &x_right, &x_left, &y_high, &y_low);};

  //! Not implemented
  void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
  void zoom();
  void nav_zoom();

  //! Save the content of the object to file.
  /*!
    \param fp	Ouput file.
    \param mode	Save as subgraph or graph.

    If the mode is glow_eSaveMode_Subgraph only the GlowConPoint part of the conpoint is saved.
  */
  void save( ofstream& fp, glow_eSaveMode mode);

  //! Read the content of the object from file.
  /*!
    \param fp	Input file.
  */
  void open( ifstream& fp);

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

  //! Redraw the area inside the objects border.
  void draw();

  //! Erase the object
  void erase( GlowWind *w)
	{ erase( w, NULL, hot, NULL);};

  //! Move the object.
  /*!
    \param delta_x	Moved distance in x direction.
    \param delta_y	Moved distance in y direction.
    \param grid		Position object on grid point.
  */
  void move( double delta_x, double delta_y, int grid);

  //! Move the object without erase.
  /*!
    \param delta_x	Moved distance in x direction.
    \param delta_y	Moved distance in y direction.
    \param grid		Position object on grid point.
  */
  void move_noerase( int delta_x, int delta_y, int grid);

  //! Set object highlight.
  /*!
    \param on	If 1, set highlight. If 0, reset highlight.
  */
  void set_highlight( int on);

  //! Get object highlight.
  /*!
    \return Return 1 if object is highlighted, else 0.
  */
  int get_highlight() {return highlight;};

  //! Not implemented.
  void set_inverse( int on) {};

  //! Not implemented.
  int get_inverse() {return inverse;};

  //! Not used.
  void set_hot( int on) {};

  //! Insert object in select list, if it is inside the selection area. 
  /*!
    \param ll_x		Lower left x coordinate of selection area.
    \param ll_y		Lower left y coordinate of selection area.
    \param ur_x		Upper right x coordinate of selection area.
    \param ur_y		Upper right y coordinate of selection area.
    \param select_policy Current select policy.
  */
  void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy);

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_GrowConPoint;};

  //! Measure the extent of the object.
  /*!
    \param ll_x		Left border of the object.
    \param ll_y		Low border of the object.
    \param ur_x		Right border of the object.
    \param ur_y		High border of the object.
  */
  void	measure( double *ll_x, double *ll_y, double *ur_x, double *ur_y)
	{ *ll_x = x_left; *ll_y = y_low; *ur_x = x_right; *ur_y = y_high;};

  //! Get the object name
  /*!
    \param name		The name of the object.
  */
  void get_object_name( char *name) { strcpy( name, n_name);};

  //! Set the object name
  /*!
    \param name		The name of the object. Max 31 char.
  */
  void set_object_name( char *name) { strcpy( n_name, name);};

  double	x_right;	//!< Right border of object.
  double	x_left;		//!< Left border of object.
  double	y_high;		//!< High border of object.
  double	y_low;		//!< Low border of object.
  int		hot;		//!< Object is hot, i.e. the cursor is currently on the object.
  GlowPoint 	pzero;		//!< Not used.
  GlowPoint	stored_pos;	//!< Not used.
  char		n_name[32];	//!< Object name.	
  int		highlight;	//!< Object is highlighted, i.e. painted with red color.
  int		inverse;	//!< Not implemented.
  GlowArc	arc;		//!< The arc object that is the symbol of the connectionpoint.
  void 		*user_data;    	//!< User data.


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

  //! Get grow context.
  /*!
    \return The context.
  */
  void *get_ctx() { return this->ctx;};

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

  //! Add a transform to the current transform.
  /*!
    \param t		Transform.

    The transform is multiplied to the current transform, to give the new transform for the object.
  */
  void set_transform( GlowTransform *t);

  //! Add a transform to the stored transform.
  /*!
    \param t		Transform.

    The transform is multiplied with the stored transform, to give to new transform for the object.
  */
  void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };

  //! Store the current transform
  /*! The stored transform can be used as starting point for future scaling or rotations.
   */
  void store_transform() { trf.store(); };

  void get_ctx( void **c) { *c = (void *)ctx;}; //!< Should be replace by get_ctx() !!

  //! Moves object to alignment line or point.
  /*!
    \param x	x coordinate of alignment point.
    \param y	y coordinate of alignment point.
    \param direction Type of alignment.
  */
  void align( double x, double y, glow_eAlignDirection direction);

  void export_flow( GlowExportFlow *ef);

};

/*@}*/
#endif


