#ifndef glow_growmenu_h
#define glow_growmenu_h

#include "glow_growrect.h"

/*! \file glow_growmenu.h
    \brief Contains the GrowMenu class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing a menu object.
/*! A GrowMenu object is a pulldown menu.

  The GrowMenu class contains function for drawing the object, and handle events when the 
  object is clicked on, moved etc.
*/

class GrowMenu : public GrowRect {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param name		Name (max 31 char).
    \param menu_info		Menu info.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param min_w	Minimum width.
    \param border_d_type Border color.
    \param line_w	Linewidth of border.
    \param fill_rect	Rectangle is filled.
    \param display_border Border is visible.
    \param fill_d_type	Fill color.
    \param t_size	Text size.
    \param t_drawtype	Drawtype for text.
    \param t_color	Text color.
    \param t_color_disabled Text color for disabled item.
    \param parent	Parent menu.
    \param nodraw	Don't draw the object now.
  */
  GrowMenu( GlowCtx *glow_ctx, char *name, glow_sMenuInfo *menu_info, double x = 0, double y = 0, 
	    double min_w = 0,
	    glow_eDrawType border_d_type = glow_eDrawType_Line, 
	    int line_w = 1, 
	    int fill_rect = 0, int display_border = 1, 
	    glow_eDrawType fill_d_type = glow_eDrawType_Line, int t_size = 2, 
	    glow_eDrawType t_drawtype = glow_eDrawType_TextHelvetica, 
	    glow_eDrawType t_color = glow_eDrawType_Line, 
	    glow_eDrawType t_color_disabled = glow_eDrawType_Line, 
	    GlowArrayElem *parent = 0, int nodraw = 0);

  //! Destructor
  /*! Remove the object from context, and erase it from the screen.
   */
  ~GrowMenu();

  void save( ofstream& fp, glow_eSaveMode mode) {}
  void open( ifstream& fp) {}

  //! Erase the object
  void erase()
	{ erase( (GlowTransform *)NULL, hot, NULL);};

  //! Erase the object in the navigator window.
  void nav_erase() {}

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
  void nav_draw(int ll_x, int ll_y, int ur_x, int ur_y) {}

  glow_eObjectType type() { return glow_eObjectType_GrowMenu;};

  glow_sMenuInfo	info;			//!< Button text and types.
  int			text_size;		//!< Button text size.
  glow_eDrawType	text_drawtype;		//!< Drawtype  for button text.
  glow_eDrawType	text_color;		//!< Color for button text.
  glow_eDrawType	text_color_disabled;	//!< Color for disabled button text.
  int			item_cnt;		//!< Number of menu items.
  double		item_height;		//!< Menu item height in pixel.
  int			current_item;		//!< Hot item.
  int			new_item;		//!< New current item.
  int			old_item;		//!< Current item in previous event.
  GlowArrayElem		*parent_menu;		//!< Parent menu.
  double		min_width;		//!< Minimum width of menu.
  int			input_focus;		//!< This menu has input focus.

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

  void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode) {}
  void nav_erase( GlowTransform *t, void *node) {}

  void export_javabean( GlowTransform *t, void *node,
			 glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc,  ofstream &fp) {}

  int event_handler( glow_eEvent event, int x, int y, double fx, double fy);
  int event_handler( glow_eEvent event, double fx, double fy);
  int local_event_handler( glow_eEvent event, double x, double y);

  //! Check this menu object is a child to the specified menu object, and delete it if it is.
  /*! \param parent	The parent menu object. */
  void delete_menu_child( GlowArrayElem *parent);

  //! Get color, textsize and text colors for menu.
  /*!
    \param t_size	Text size.
    \param fill_color	Fill color.
    \param t_drawtype	Drawtype for text.
    \param t_color	Text color.
    \param t_color_disabled Text color for disabled button.
  */
  void get_menu_char( int *t_size, glow_eDrawType *fill_color, glow_eDrawType *t_drawtype,
		      glow_eDrawType *t_color, glow_eDrawType *t_color_disabled);

  void shift_current_item( int shift);
  int get_current_item( int *item);
  void set_input_focus( int focus) { input_focus = focus;}
  int get_input_focus() { return input_focus;}
  void get_submenu_position( int item, double *x, double *y);
  int get_parent( GlowArrayElem **parent);
};


/*@}*/
#endif









