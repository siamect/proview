/* 
 * Proview   $Id: glow_growimage.h,v 1.8 2008-10-31 12:51:35 claes Exp $
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

#ifndef glow_growimage_h
#define glow_growimage_h

#include "glow_transform.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_draw.h"

/*! \file glow_growimage.h
    \brief Contains the GrowImage class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing an image.
/*! GrowImage uses Imlib to read a gif, jpeg or other file format, and to render the image to the appropriate scale.
  The class contains functions to draw the image, to change the colors, and to detect and handle events.
*/
class GrowImage : public GlowArrayElem {
 public:

  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param imagefile	Image filename.
    \param display_lev	Displaylevel when this object is visible.
  */
  GrowImage( GrowCtx *glow_ctx, const char *name, double x = 0, double y = 0, 
	     const char *imagefile = 0,
	     glow_mDisplayLevel display_lev = glow_mDisplayLevel_1);

  //! Noargs constructor
  GrowImage() {};

  //! Make this object a copy of another image object.
  /*!
    \param im	Object to copy.
  */
  void copy_from( const GrowImage& im);

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
  int	event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx, double fy);

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

  //! Detects if the object is hit by an event in local coordinates
  /*!
    \param event	Current event.
    \param x		x of event coordinate in the objects local coordinate system.
    \param y		y of event coordinate in the objects local coordinate system.
    \return 		Returns 1 if the object is hit, else 0.

    Compares the coordinates of the event with the borders of the object.
    If the event is inside the borders, 1 is returned, otherwise 0 is returned.
  */
  int local_event_handler( glow_eEvent event, double x, double y);
  
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
	{ get_borders( (GlowTransform *) NULL, x1_right, x1_left, y1_high, y1_low);};

  //! Update the borders of the object.
  /*! The borders of the object is stored in x_right, x_left, y_high and y_low.
   */
  void get_node_borders()
	{ x_left = y_low = 1e37; x_right = y_high = -1e37;
	  get_borders( (GlowTransform *) NULL, &x_right, &x_left, &y_high, &y_low);};

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

  //! Erase the object
  void erase( GlowWind *w)
	{ erase( w, (GlowTransform *)NULL, hot, NULL);};

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
  glow_eObjectType type() { return glow_eObjectType_GrowImage;};

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

  //! Moves object to alignment line or point.
  /*!
    \param x	x coordinate of alignment point.
    \param y	y coordinate of alignment point.
    \param direction Type of alignment.
  */
  void align( double x, double y, glow_eAlignDirection direction);

  int update();

  GlowPoint 	ll;		//!< Lower left point of image.
  GlowPoint 	ur;		//!< Upper right point of image.
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
  void 		*user_data;    	//!< User data.
  char        	image_filename[120];
  char 		*dynamic;	//!< Dynamic code.
  int 		dynamicsize;	//!< Size of dynamic code.
  GlowTransform trf;		//!< Transformation matrix of object.
  glow_tImData  imlib;		//!< Pointer to imlib
  glow_tImImage image;		//!< The rendered and scaled image.
  glow_tImImage original_image; //!< The original image.
  glow_tPixmap  pixmap;		//!< Pixmap of the image.
  glow_tPixmap  nav_pixmap;	//!< Pixmap of the image in navigation window.
  glow_tPixmap  clip_mask;	//!< Clip mask if transparent GIF image.
  glow_tPixmap  nav_clip_mask;	//!< Clip mask in navigation window i transparent GIF image.
  GrowCtx 	*ctx;		//!< Pointer to Grow context.
  glow_mDisplayLevel display_level; //!< Display level when this object is visible.
  int       	current_width;		//!< Current width of the image.
  int       	current_height;		//!< Current height of the image.
  int       	current_nav_width; 	//!< Current widht of the image in navigation window.
  int       	current_nav_height; 	//!< Current height of the image in navigation window.
  glow_eDrawTone color_tone;		//!< Color tone of the object.
  int		color_lightness; 	//!< Color lightness of the object.
  int		color_intensity; 	//!< Color intensity of the object.
  int		color_shift;		//!< Color shift of the object.
  int		color_inverse;		//!< Color inverse of the object.
  glow_eDrawTone current_color_tone;	 //!< Current color tone of the image.
  int		current_color_lightness; //!< Current color lightness of the image.
  int		current_color_intensity; //!< Current color intensity of the image.
  int		current_color_shift; 	//!< Current color shift of the image.
  int		current_color_inverse; 	//!< Current color inverse of the image.
  int         current_direction; //! Current direction of the image.
  glow_eDrawTone current_nav_color_tone; //!< Current color tone of the image in the navitation window.
  int		current_nav_color_lightness; //!< Current color lightness of the image in the navitation window.
  int		current_nav_color_intensity; //!< Current color intensity of the image in the navitation window.
  int		current_nav_color_shift; //!< Current color shift of the image in the navitation window.
  int		current_nav_color_inverse; //!< Current color inverse of the image in the navitation window.
  int         	current_nav_direction; 	//!< Current direction of the image in the navitation window.
  char        	last_group[32];		//!< The last group the object was a member of.
  int		date;			//!< Date of the image file.
  char		filename[256];		//!< Name of the image file with full path.
  bool		flip_vertical;		//!< The object is flipped vertically.
  bool		flip_horizontal;	//!< The object is flipped horizontally.
  bool		current_flip_vertical;	//!< Current vertical flip.
  bool		current_flip_horizontal; //!< Current horizontal flip.
  int		rotation;
  int		current_rotation;
  glow_eDrawTone c_color_tone;
  int		c_color_lightness;
  int		c_color_intensity;
  int		c_color_shift;
  int		c_color_inverse;
  float         factor_intens;
  float		factor_light;
  int		fixposition;	//!< Can't be moved.
 
  //! Read the image file and create pixmaps for the image.
  /*!
    \param imagefile	Name of imagefile. The file is looked for in the context's search path.

    Look for the file in the directories of the context's serachpath. Uses Imlib to read the file and
    create pixmap of desired scale. Changes the color to fit the color tone, intensity, lighness, etc.
  */
  int insert_image( const char *imagefile);

  //! Zoom to the current zoom factor
  void zoom();

  //! Zoom to the current navigation window zoom factor.
  void nav_zoom();

  // Traverse coordinates of ll and ur the distance (x,y)
  void traverse( int x, int y);

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

  //! Get grow context.
  /*!
    \return The context.
  */
  void *get_ctx() { return this->ctx;};

  //! Execute the dynamic code.
  void exec_dynamic();

  //! Set the position of the object, in absoute coordinates.
  /*!
    \param x	x coordinate.
    \param y	y coordinate.
  */
  void set_position( double x, double y);

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

  //! Rotate the object. The rotation is made form the last stored transform.
  /*!
    \param angle	Rotation angle in degrees.
    \param x0		x coordinate of rotation point.
    \param y0		y coordinate of rotation point.
    \param type		Type of rotation point.
  */
  void set_rotation( double angle, 
		double x0, double y0, glow_eRotationPoint type);

  //! Mirror the object around a horizontal or vertical mirror line.
  /*!
    \param x0	x coordinate of mirror line.
    \param y0	y coordinate of mirror line.
    \param dir	Direction of mirror.
  */
  void flip( double x0, double y0, glow_eFlipDirection dir);

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

  //! Set the original color tone
  /*!
    \param tone 	Color tone.
  */
  void set_original_color_tone( glow_eDrawTone tone) 
	{ set_color_tone( tone);}

  //! Set the color tone
  /*!
    \param tone 	Color tone.
  */
  void set_color_tone( glow_eDrawTone tone) 
	{ if ( color_tone == tone) return;
          color_tone = tone; 
          color_shift = 0;
          draw();};

  //! Set the original color lightness.
  /*!
    \param lightness 	Color lightness.
  */
  void set_original_color_lightness( int lightness) 
	{ set_color_lightness( lightness);};

  //! Increment the original color lightness.
  /*!
    \param lightness 	Increment of color lightness.
  */
  void incr_original_color_lightness( int lightness) 
	{ color_lightness += lightness; 
          if ( color_lightness > 5) color_lightness = 5;
          if ( color_lightness < -5) color_lightness = -5;
          draw();};

  //! Set the color lightness.
  /*!
    \param lightness 	Color lightness.
  */
  void set_color_lightness( int lightness) 
	{ if ( color_lightness == lightness) return;
          color_lightness = lightness; 
          draw();};

  //! Set the original color intensity.
  /*!
    \param intensity 	Color intensity.
  */
  void set_original_color_intensity( int intensity) 
	{ set_color_intensity( intensity);};

  //! Increment the original color intensity.
  /*!
    \param intensity 	Increment of color intensity.
  */
  void incr_original_color_intensity( int intensity)
	{ color_intensity += intensity;
          if ( color_intensity > 5) color_intensity = 5;
          if ( color_intensity < -5) color_intensity = -5;
          draw();};

  //! Set the color intensity.
  /*!
    \param intensity 	Color intensity.
  */
  void set_color_intensity( int intensity) 
	{ if ( color_intensity == intensity) return;
          color_intensity = intensity; 
          draw();};

  //! Set the original color shift.
  /*!
    \param shift 	Color shift.
  */
  void set_original_color_shift( int shift) 
	{ set_color_shift( shift);};

  //! Increment the original color shift.
  /*!
    \param shift 	Increment of color shift.
  */
  void incr_original_color_shift( int shift) 
	{ incr_color_shift( shift);};

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

  //! Set the color inverse.
  /*!
    \param inverse 	Color inverse.
  */
  void set_color_inverse( int inverse) 
	{ color_inverse = inverse;
          draw();};

  //! Calculate the color of the image dependent on the color tone, brightness, intensity etc.
  /*!
    \param image 	The Imlib image.
    \param node		The node that controls the color of the object.
  */
  int set_image_color( glow_tImImage image, void *node);

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

  //! Store the current transform
  /*! The stored transform can be used as starting point for future scaling or rotations.
   */
  void store_transform() { trf.store(); };

  void get_ctx( void **c) { *c = (void *)ctx;}; 	//!< Should be replace by get_ctx() !!

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

  static void pixel_cb( void *data, unsigned char *rgb);

  //! Destructor
  /*! Remove object from context, delete the image and erase if from the screen.
   */
  ~GrowImage();

};

//! Read an image from file file and return the pixmap of the image.
/*!
  \param ctx		Grow context.
  \param imagefile	Name of image file.
  \param width		Desired width of pixmap.
  \param height		Desired height of pixmap.
  \param pixmap		The pixmap of the image.
  \param w		Width of the pixmap.
  \param h		Height of the pixmap.

  If width and height is not zero, the image is scaled to the desired width and height.
  If they are zero, the size from the image file is kept, and returned in w and h.
*/
int grow_image_to_pixmap( GrowCtx *ctx, char *imagefile, 
	    int width, int height, glow_tPixmap *pixmap, glow_tImImage *image, int *w, int *h);

/*@}*/
#endif




