#ifndef glow_conclass_h
#define glow_conclass_h

#include <iostream.h>
#include <fstream.h>
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_array_elem.h"
#include "glow_array.h"

/*! \file glow_conclass.h
    \brief Contains the GlowConClass class. */
/*! \addtogroup Glow */
/*@{*/


//! Class object for connections.
/*! The GlowConClass object contains the definition of a connection style.
  Every GlowCon connection belongs to a connection class, which the color, linewidth, 
  cornerstyle for the connection.
*/

class GlowConClass : public GlowArrayElem {
 public:
  //! Constructor
  /*!
    \param glow_ctx	Glow context.
    \param name		Conclass name.
    \param contype	Type of connection.
    \param cornertype	Type of corner, rounded or straight.
    \param d_type	Color.
    \param line_w	Line width.
    \param arrow_w	Width of arrow.
    \param arrow_l	Length of arrow.
    \param round_corner_amnt The size of the arcs in rounded corners.
    \param grp		Group a connection is a member of.
  */
  GlowConClass( GlowCtx *glow_ctx, char *name, glow_eConType contype,
	glow_eCorner cornertype, glow_eDrawType d_type, int line_w,
	double arrow_w = 0, double arrow_l = 0, double round_corner_amnt = 0.5,
	glow_eConGroup grp = glow_eConGroup_Common);

  friend ostream& operator<< ( ostream& o, const GlowConClass cc);

  void zoom() {};
  void nav_zoom() {};
  void print_zoom() {};
  void traverse( int x, int y) {};
  void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
	{};
  int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node)
		 { return 0;};
  void print( GlowPoint *pos, void *node) {};

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
  void draw( GlowPoint *pos, int highlight, int hot, void *node) {};
  void nav_draw( GlowPoint *pos, int highlight, void *node) {};
  void erase( GlowPoint *pos, int hot, void *node) {};
  void nav_erase( GlowPoint *pos, void *node) {};

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_ConClass;};

  //! Get object name
  /*!
    \param name		Object name.
  */
  void get_object_name( char *name);

  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  GlowCtx	*ctx;		//!< Glow ctx.
  GlowPoint	zero;
  char	cc_name[32];		//!< Con class name.
  glow_eConType con_type;	//!< Connection type.
  glow_eCorner corner;		//!< Type of corners, rounded or straight.
  glow_eDrawType draw_type;	//!< Connection color.
  int	 line_width;		//!< Line width.
  double arrow_width;		//!< Width of arrow.
  double arrow_length;		//!< Length of arrow.
  double round_corner_amount;	//!< The size of the arcs in rounded corners.
  glow_eConGroup group;		//!< The group a connection belongs to.
};

/*@}*/
#endif

