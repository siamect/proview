/* 
 * Proview   $Id: glow_grownode.h,v 1.16 2008-12-01 16:32:40 claes Exp $
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

#ifndef glow_grownode_h
#define glow_grownode_h

#include "glow_node.h"

/*! \file glow_grownode.h
    \brief Contains the GrowNode class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing subgraph object.
/*! A GrowNode object is an object that is built of base object, such as rectangles, arcs,
  connectionpoints and even other GrowNode objects. The specification of these objects are 
  stored in a NodeClass, and the GrowNode object has a pointer to it's NodeClass. 

  The GrowNode class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/
class GrowNode : public GlowNode {
 public:

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param node_class	Pointer to NodeClass object.
    \param x1		x coordinate for position.
    \param y1		y coordinate for position.
    \param nodraw	Don't draw the object now.
    \param rel_annot_pos Not used.
  */
  GrowNode( GrowCtx *glow_ctx, const char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw = 0, int rel_annot_pos = 0);

  //! Noargs constructor
  GrowNode() {};

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return object_type;};

  //! Make this object a copy of another image object.
  /*!
    \param n	Object to copy.
  */
  void copy_from( const GrowNode& n);

  //! Erase the object
  void erase( GlowWind *w)
	{ erase( w, (GlowTransform *)NULL, hot, NULL);};

  //! Update the borders of the object.
  /*! The borders of the object is stored in x_right, x_left, y_high and y_low.
   */
  void get_node_borders();
  
  //! Calculate the border for a set of objects or for a parent node.
  /*!
    \param t		Transform.
    \param x1_right	Right limit.
    \param x1_left	Left limit.
    \param y1_high	High limit.
    \param y1_low	Low limit.
    
    If the borders of the objects exceeds a limit, the limit is adjusted to the
    border of the object.
  */
  void get_borders( GlowTransform *t,
	double *x1_right, double *x1_left, double *y1_high, double *y1_low);

  //! Save the content of the object to file.
  /*!
    \param fp	Ouput file.
    \param mode	Save as graph or subgraph.
  */
  void save( ofstream& fp, glow_eSaveMode mode);

  //! Read the content of the object from file.
  /*!
    \param fp	Input file.
  */
  void open( ifstream& fp);

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

  //! Move the object to a specific point.
  /*!
    \param x	x coordinate for position.
    \param y	y coordinate for position.
  */
  void move_to( double x, double y);

  //! Set object highlight.
  /*!
    \param on	If 1, set highlight. If 0, reset highlight.
  */
  void set_highlight( int on);

  //! Close trace
  /*! Calls the trace disconnect callback.
   */
  void trace_close();

  //! Detects if the object is hit by an event in transformed coordinates
  /*!
    \param event	Current event.
    \param fx		x coordinate of event in the objects transformed coordinate system.
    \param fy		y coordinate of event in the objects transformed coordinate system.
    \return 		Returns 1 if the object is hit, else 0.

    Compares the coordinates of the event with the borders of the object.
    If the event is inside the borders, 1 is returned, otherwise 0 is returned.
  */
  int event_handler( GlowWind *w, glow_eEvent event, double fx, double fy);

  //! Event handler
  /*!
    \param event	Current event.
    \param x		Not used.
    \param y		Not used.
    \param fx		x coordinate of event.
    \param fy		y coordinate of event.
    \return		Returns 1 if the object is hit, else 0.

    Detects if the object is hit by the event, and if it is, performes the appropriate
    action: changes the cursor, draws the object hot, and registers the object as
    current callback object.
  */
  int event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx, double fy);

  //! Set hot
  /*! Increase the linewith to mark that the cursor points at the node.
   */
  void set_hot( int on);

  char 	       	*dynamic;	//!< Dynamic code.
  int 		dynamicsize;	//!< Size of dynamic code.
  glow_eDrawType original_border_drawtype; //!< Original border color.
  glow_eDrawType original_text_drawtype; //!< Original text color.
  glow_eDrawType original_fill_drawtype; //!< Original fill color.
  glow_eDrawType fill_drawtype;	//!< Fill color. 
  glow_eDrawType text_drawtype; //!< Text color.
  glow_eDrawType draw_type;	//!< Border color.
  glow_eDrawType level_fill_drawtype; //!< Level fill color.
  glow_eDrawTone original_color_tone; //!< Original color tone.
  glow_eDrawTone color_tone;	//!< Color tone.
  glow_eDrawTone level_color_tone; //!< Level color tone.
  int		original_color_lightness; //!< Original color lightness.
  int		color_lightness; //!< Color lightness.
  int		original_color_intensity; //!< Original color lightness.
  int		color_intensity; //!< Color intensity.
  int		original_color_shift; //!< Original color intensity.
  int		color_shift;	//!< Color shift.
  int		color_inverse;	//!< Color inverse.
  int		line_width;	//!< Line width.
  int		invisible;	//!< Object is invisible.
  int		dimmed;		//!< Object is dimmed.
  GlowTransform trf;		//!< Transformation matrix of object.
  char	*argv[20];		//!< Arguments in dynamic code.
  int		argsize[20];	//!< Size of arguments is dynamic code.
  glow_eObjectType object_type;	//!< Object type.
  GrowNode    *root_node;	//!< The root node or group.
  char        	last_group[32]; //!< The last group the object was a member of.
  bool	flip_horizontal;	//!< The object is flipped horizontally.
  bool	flip_vertical;		//!< The object is flipped vertically.
  double	fill_level;	//!< The fill level.
  glow_eDirection level_direction; //!< Direction of fill level.
  int 		shadow;	       	//!< Object should be drawn with shadows.
  int		input_position;	//!< Cursor position for input annotation.
  int		input_selected; //!< Text in input annotation is selected.
  glow_eGradient gradient;      //!< Type of gradient.
  glow_eDrawType text_type;	//!< Text type, bold or normal.
  glow_eFont	text_font;	//!< Text font.
    
  //! Set dynamic code
  /*!
    \param code		Dynamic code.
    \param size		Size of dynamic code.
  */
  void set_dynamic( char *code, int size);

  //! Get dynamic code
  /*!
    \param code		Dynamic code.
    \param size		Size of dynamic code.
  */
  void get_dynamic( char **code, int *size) { *code = dynamic; 
	*size = dynamicsize;};

  //! Execute the dynamic code.
  void exec_dynamic();

  //! Set the fill color.
  /*! \param drawtype	Fill color. */
  void set_fill_color( glow_eDrawType drawtype) 
	{ if ( fill_drawtype == drawtype) return;
          fill_drawtype = drawtype; 
          color_tone = glow_eDrawTone_No;
          color_shift = 0;
          color_lightness = 0;
          color_intensity = 0;
          draw();};

  //! Reset the fill color to the original fill color.
  void reset_fill_color() 
	{ if ( fill_drawtype == original_fill_drawtype) return;
          fill_drawtype = original_fill_drawtype; 
          color_tone = original_color_tone;
          color_shift = original_color_shift;
          color_lightness = original_color_lightness;
          color_intensity = original_color_intensity;
          draw();};

  //! Set the border color.
  /*!
    \param drawtype	Border color.
  */
  void set_border_color( glow_eDrawType drawtype) 
	{ if ( draw_type == drawtype) return;
          draw_type = drawtype;
          color_tone = glow_eDrawTone_No;
          color_shift = 0;
          color_lightness = 0;
          color_intensity = 0;
          draw();}; 

  //! Reset the border color to the original border color.
  void reset_border_color()
	{ if ( draw_type == original_border_drawtype) return;
          draw_type = original_border_drawtype; 
          draw();};

  //! Set the text color.
  /*!
    \param drawtype	Text color.
  */
  void set_text_color( glow_eDrawType drawtype) 
	{ if ( text_drawtype == drawtype) return;
          text_drawtype = drawtype;
          color_tone = glow_eDrawTone_No;
          color_shift = 0;
          color_lightness = 0;
          color_intensity = 0;
          draw();};

  //! Reset the text color to the original text color.
  void reset_text_color()
	{ if ( text_drawtype == original_text_drawtype) return;
          text_drawtype = original_text_drawtype; 
          draw();};


  //! Set the original fill color.
  /*!
    \param drawtype	Fill color.
  */
  void set_original_fill_color( glow_eDrawType drawtype) 
	{ original_color_tone = glow_eDrawTone_No;
          color_tone = glow_eDrawTone_No;
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
	  original_fill_drawtype = drawtype; 
          reset_fill_color();};

  //! Set the original border color.
  /*!
    \param drawtype	Border color.
  */
  void set_original_border_color( glow_eDrawType drawtype) 
	{ original_color_tone = glow_eDrawTone_No;
          color_tone = glow_eDrawTone_No;
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
	  original_border_drawtype = drawtype;
          reset_border_color();};

  //! Set the original text color.
  /*!
    \param drawtype	Text color.
  */
  void set_original_text_color( glow_eDrawType drawtype) 
	{ original_color_tone = glow_eDrawTone_No;
          color_tone = glow_eDrawTone_No;
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
	  original_text_drawtype = drawtype;
          reset_text_color();};

  //! Set the original color tone
  /*!
    \param tone 	Color tone.
  */
  void set_original_color_tone( glow_eDrawTone tone) 
	{ original_color_tone = tone; 
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
          reset_color_tone();};

  //! Set the color tone
  /*!
    \param tone 	Color tone.
  */
  void set_color_tone( glow_eDrawTone tone) 
	{ if ( color_tone == tone) return;
          color_tone = tone; 
          color_shift = 0;
          draw();};

  //! Reset color tone to original color tone.
  void reset_color_tone() 
	{ if ( color_tone == original_color_tone) return;
          color_tone = original_color_tone; 
          color_shift = original_color_shift;
          draw();};

  //! Set the original color lightness.
  /*!
    \param lightness 	Color lightness.
  */
  void set_original_color_lightness( int lightness) 
	{ original_color_lightness = lightness; 
          reset_color_lightness();};

  //! Increment the original color lightness.
  /*!
    \param lightness 	Increment of color lightness.
  */
  void incr_original_color_lightness( int lightness) 
	{ original_color_lightness += lightness; 
          if ( original_color_lightness > 9) original_color_lightness = 9;
          if ( original_color_lightness < -9) original_color_lightness = -9;
          reset_color_lightness();};

  //! Set the color lightness.
  /*!
    \param lightness 	Color lightness.
  */
  void set_color_lightness( int lightness) 
	{ if ( color_lightness == lightness) return;
          color_lightness = lightness; 
          draw();};

  //! Reset color lightness to original color lightness.
  void reset_color_lightness() 
	{ if ( color_lightness == original_color_lightness) return;
          color_lightness = original_color_lightness;
          draw();};

  //! Set the original color intensity.
  /*!
    \param intensity 	Color intensity.
  */
  void set_original_color_intensity( int intensity) 
	{ original_color_intensity = intensity; 
          reset_color_intensity();};

  //! Increment the original color intensity.
  /*!
    \param intensity 	Increment of color intensity.
  */
  void incr_original_color_intensity( int intensity) 
	{ original_color_intensity += intensity; 
          if ( original_color_intensity > 2) original_color_intensity = 2;
          if ( original_color_intensity < -3) original_color_intensity = -3;
          reset_color_intensity();};

  //! Set the color intensity.
  /*!
    \param intensity 	Color intensity.
  */
  void set_color_intensity( int intensity) 
	{ if ( color_intensity == intensity) return;
          color_intensity = intensity; 
          draw();};

  //! Reset the color intensity to original color intensity.
  void reset_color_intensity() 
	{ if ( color_intensity == original_color_intensity) return;
          color_intensity = original_color_intensity;
          draw();};

  //! Set the original color shift.
  /*!
    \param shift 	Color shift.
  */
  void set_original_color_shift( int shift) 
	{ original_color_shift = shift; 
          reset_color_shift();};

  //! Increment the original color shift.
  /*!
    \param shift 	Increment of color shift.
  */
  void incr_original_color_shift( int shift) 
	{ original_color_shift += shift; 
          reset_color_shift();};

  //! Increment the color shift.
  /*!
    \param shift 	Increment of color shift.
  */
  void incr_color_shift( int shift) 
	{ if ( !shift) return;
          color_shift += shift; 
          draw();};

  //! Set the original color shift.
  /*!
    \param shift 	Color shift.
  */
  void set_color_shift( int shift) 
	{ if ( color_shift == shift) return;
          color_shift = shift; 
          draw();};

  //! Reset the color shift to original color shift.
  void reset_color_shift() 
	{ if ( color_shift == original_color_shift) return;
          color_shift = original_color_shift;
          draw();};

  //! Set the color inverse.
  /*!
    \param inverse 	Color inverse.
  */
  void set_color_inverse( int inverse) 
	{ color_inverse = inverse;
          draw();};

  //! Set the level fill color.
  /*!
    \param color 	Level fill color.
  */
  void set_level_fill_color( glow_eDrawType color)
        { level_fill_drawtype = color;}


  //! Set the level color tone.
  /*!
    \param tone 	Level color tone.
  */
  void set_level_color_tone( glow_eDrawTone tone)
        { level_color_tone = tone;}

  //! Set the fill level.
  /*!
    \param level 	Fill level.
  */
  void set_fill_level( double level)
        { fill_level = level; draw();}


  //! Set the level direction.
  /*!
    \param dir 	Level direction.
  */
  void set_level_direction( glow_eDirection dir)
        { level_direction = dir;}

  //! Set object visibility.
  /*!
    \param visibility 	The visibility of the object.
  */
  void set_visibility( glow_eVis visibility);

  //! Set position relative last stored transform.
  /*!
    \param x	x coordinate.
    \param y	y coordinate.
  */
  void set_position( double x, double y);

  //! Set the scale and position of the object. The position and scale is relative the last stored transform.
  /*!
    \param x		x coordinate of position.
    \param y		y coordinate of position.
    \param scale_x	Scale factor in x direction.
    \param scale_y	Scale factor in y direction.
    \param x0		x coordinate of scale point.
    \param y0		y coordinate of scale point.
    \param type		Type of scaling.
  */
  void set_scale_pos( double x, double y, double scale_x, double scale_y, 
		double x0, double y0, glow_eScaleType type);

  //! Set the scale of the object. The scale is made from the last stored transform.
  /*!
    \param scale_x	Scale factor in x direction.
    \param scale_y	Scale factor in y direction.
    \param x0		x coordinate of scale point.
    \param y0		y coordinate of scale point.
    \param type		Type of scaling.
  */
  void set_scale( double scale_x, double scale_y, double x0, double y0, 
		glow_eScaleType type);

  //! Rotate the object. The rotation is made from the last stored transform.
  /*!
    \param angle	Rotation angle in degrees.
    \param x0		x coordinate of rotation point.
    \param y0		y coordinate of rotation point.
    \param type		Type of rotation point.
  */
  void set_rotation( double angle, 
		double x0, double y0, glow_eRotationPoint type);

  //! Redraw the area inside the objects border.
  void draw();

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

    The transorm is multiplied with the stored transform, to give to new transform for the object.
  */
  void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };

  //! Get info for a connection point
  /*!
    \param num		Connection point number for which info should be returned.
    \param x		x coordinate for connection point.
    \param y		y coordinate for connection point.
    \param dir		Direction of the connection point.
  */
  int get_conpoint( int num, double *x, double *y, glow_eDirection *dir);

  //! Set the text of an annotation
  /*!
    \param num		Annotation number.
    \param text		The text.
    \param size		The length of the text.
    \param nodraw	Don't draw now.
    \param brief	Just redraw the background object for the annotation, not the whole area.
  */
  void set_annotation( int num, const char *text, int size, int nodraw, int brief = 0);

  //! Store the current transform
  /*! The stored transform can be used as starting point for future scaling or rotations.
   */
  void store_transform() { trf.store(); };

  //! Check if there is a stored transform
  /*! 
    \return Returns true if the transform is previously stored with a call to store_tranform.
  */
  bool transform_is_stored() { return trf.is_stored(); };

  //! Get dynamic code argument
  /*!
    \param num		Argument number.
    \param arg		The argument string.
    \param size		Lenght of the argument string.
  */
  int get_argument( int num, char *arg, int size);

  //! Set dynamic code argument
  /*!
    \param num		Argument number.
    \param arg		The argument string.
    \param size		Lenght of the argument string.
  */
  int set_argument( int num, char *arg, int size);

  //! Set the line width
  /*!
    \param linewidth	Line width.
  */
  void set_linewidth( int linewidth);

  void get_ctx( void **c) { *c = (void *)ctx;}; 	//!< Should be replace by get_ctx() !!

  //! Redraw an annotation background
  /*!
    \param t		Transform of parent node or group.
    \param x		x coordinate for the annotation.
    \param y		y coordinate for the annotation.

    The first filled GrowRect object is considered to be the background for an annotation.
    The GrowRect object is found and redrawn.
  */
  int draw_annot_background(  GlowTransform *t, double x, double y)
	{ return nc->draw_annot_background( t, (void *)this, x, y);}; 


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

  //! Set next page to the current page. 
  /*! \return Returns 1 if there is a next page, else 0. */
  int set_next_nodeclass();

  //! Set previous page to current page.
  /*! \return Returns 1 if there is a previous page, else 0. */
  int set_previous_nodeclass();

  //! Set first page to current page.
  void set_root_nodeclass();

  //! Set last page to current page.
  void set_last_nodeclass();

  //! Set page to the supplied index.
  /*!
    \param idx		Index of the page.
  */
  int set_nodeclass_by_index( int idx);

  //! Set nodeclass or page.
  /*!
    \param new_nc	NodeClass for the page.
  */
  void set_nodeclass( GlowNodeClass *new_nc);

  //! Get the animation count of the nodeclass.
  /*!
    \param count	The animation count of the nodeclass.
  */
  void get_animation_count( int *count) { *count = nc->animation_count;}; 

  //! Check if an node class is used by this node
  /*!
    \param nodeclass	Nodeclass to check.
    \return		Return 1 if the nodeclass is found, else 0.
  */
  int find_nc( GlowArrayElem *nodeclass);

  //! The node is ungrouped
  /*! This function should be called when a group where the object is a member is dissolved.
   */
  void ungroup() { root_node = 0;};


  //! Register the name of the last group
  /*!
    \param name		The name of the group the object until now was a member of.

    The last group is used to recover data for a group that is desolved, when the members are regrouped.
  */
  void set_last_group( char *name) { strcpy( last_group, name); };

  //! Return the name of the last group the object was a member of
  /*!
    \return The name of the last group the object was a member of.
  */
  char *get_last_group() { return last_group; };

  //! Determine whether the node is sensitive for any action.
  /*!
    \return 	Return 1 if the node is sensitive for any action, else 0.

    The function sends a hot_request callback to check whether the node is sensitive or not.
  */
  int is_sensitive();

  int is_click_sensitive(); 		//!< Obsolete
  int is_refobject_sensitive();		//!< Obsolete

  //! Get the limits for the level dynamics
  /*!
    \param min		Min limit.
    \param max		Max limit.
    \param direction 	Direction of the node.

    The limits are stored in y0 and y1 in the nodeclass for the object, in the nodeclass
    coordinate system, and measured in Up direction. These coordinates are transformed with 
    the node tranformation matrix, and the direction is determined from the rotation of the node.
  */
  int get_limits( double *min, double *max, glow_eDirection *direction);


  //! Get the limits in pixel for the level dynamics
  /*!
    \param min		Min limit.
    \param max		Max limit.
    \param direction 	Direction of the node.

    The limits are stored in y0 and y1 in the nodeclass for the object, in the nodeclass
    coordinate system, and measured in Up direction. These coordinates are transformed with 
    the node tranformation matrix, and the direction is determined from the rotation of the node.
  */
  int get_limits_pixel( double *min, double *max, glow_eDirection *direction);

  //! Get the limits for a background object
  /*!
    \param t		Transform.
    \param type		Dyntype of the background object.
    \param x		x coordinate of foreground object.
    \param y		y coordinate of foreground object.
    \param background	The found background object.
    \param min		Min value for the background object.
    \param max		Max value for the background object.
    \param direction	Direction of the background object.

    Check if this node matches the specified dyntype and the coordinates of the foreground object.
    The limits are stored in y0 and y1 in the nodeclass for object, in the nodeclass
    coordinate system, and measured in Up direction. These coordinates are transformed with 
    the node tranformation matrix, and the direction is determined from the rotation of the node.
  */
  int get_background_object_limits( GlowTransform *t, glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction);

  //! Get coordinates for the nodeclass origo
  /*!
    \param t	Transform of parent node or group.
    \param x	x coordinate of nodeclass origo.
    \param y	y coordinate of nodeclass origo.
  */
  void get_nodeclass_origo( GlowTransform *t, double *x, double *y);

  //! Redraw all connections connected to this node
  /*! Find all connection that is connected to this node and redraw them.
   */
  virtual void call_redraw_node_cons();


  //! Mirror the object around a horizontal or vertical mirror line.
  /*!
    \param x0	x coordinate of mirror line.
    \param y0	y coordinate of mirror line.
    \param dir	Direction of mirror.
  */
  void flip( double x0, double y0, glow_eFlipDirection dir);

  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  //! Set shadow.
  /*!
    \param shadowval	If 1 the object will be draw with shadow, if 0 the object will be drawn without shadow.
  */
  void set_shadow( int shadowval) { shadow = shadowval; draw();}

  //! Set Gradient.
  /*!
    \param gradientval	Type of gradient.
  */
  void set_gradient( glow_eGradient gradientval) { gradient = gradientval; draw();}

  //! Set root node.
  /*!
    \param node		Rootnode.
  */
  void set_rootnode( void *node) { root_node = (GrowNode *) node;}

  //! Set or reset input focus.
  /*! \param focus	if 1 focus is set, else focus is reset. */
  void set_input_focus( int focus, glow_eEvent event);

  //! Open an annotation for input.
  /*! \param num	Annotation number. */
  void open_annotation_input( int num);

  //! Close an annotation for input
  /*! \param num	Annotation number. */
  void close_annotation_input( int num);

  //! Close any annotation for input.
  void close_annotation_input();

  //! Set text selection of open annotation
  /*! \param selection	1 selected, 0 not selected. */
  void set_annotation_selection( int selection);
  
  //! Handle an annotation input event.
  /*! 
    \param event	Key event. 
    \param keycode	Key code for pressed key.
  */
  void annot_input_event( glow_eEvent event, int keycode);

  //! Get text size and color for an annotation.
  /*!
    \param num		Annotation number.
    \param t_size	Annotation text size.
    \param t_drawtype Annotation text drawtype.
    \param t_color	Annotation text color.
    \param bg_color	Annotation background color.
    \param scale	Annotation scale.
    \param font		Annotation font.
    \return		0 if annotation doesn't exist, else 1.
  */
  int get_annotation_info( int num, int *t_size, glow_eDrawType *t_drawtype, glow_eDrawType *t_color,
			   glow_eDrawType *bg_color, double *scale, glow_eFont *font);
  int get_java_name( char *name) { return nc->get_java_name( name);}
  void set_textbold( int bold);
  void set_textfont( glow_eFont textfont);

  //! Destructor
  /*! Remove the object from the context and erase it from the screen.
   */
  ~GrowNode();
};

/*@}*/
#endif




