#ifndef glow_tiptext_h
#define glow_tiptext_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"

class GlowCtx;
class GlowArrayElem;

/*! \file glow_tiptext.h
    \brief Contains the GlowTipText class. */
/*! \addtogroup Glow */
/*@{*/


//! Draw tooltip text.
/*! A tooltip text is a text frame that becomes visible some time after the cursor has visited an object, it
  the cursor still rest on that object. When the object is hit by the cursor, draw_text() should be called.
  The text will be displayed within 1 second if not the remove_text() or remove() function are called.
  The text is visible until the remove_text() or remove() function are called.
*/

class GlowTipText {
 public:
  
  //! Constructor
  /*!
    \param gctx		Glow context.
  */
  GlowTipText( GlowCtx *gctx) : ctx(gctx), active(false), timer_id(0) {}

  //! Destructor
  /*! Removes the timer if it is set.
   */
  ~GlowTipText();

  GlowCtx *ctx;		//!< Glow context.
  int text_x;		//!< x coordinate in pixels for text.
  int text_y;		//!< y coordinate in pixels for text.
  int text_width;	//!< Text width in pixels.
  int text_height;	//!< Text height in pixels.
  int text_descent;	//!< Text descent in pixels.
  GlowArrayElem *text_object;  //!< Object that activated the current tip text.
  char tiptext[200];	//!< Current tip text.
  bool active;		//!< Tip text is active, i.e. timer is running or text is displayed.
  void *timer_id;	//!< Timer id.

  //! Activate tooltip.
  /*!
    \param e	Object that owns the tiptext.
    \param text	The tooltip text.
    \param x	x coordinate in pixel for the text.
    \param y	y coordinate in pixel for the text.

    This function is called when the cursor hits an object. It starts a timer, and if
    no remove function is called, the text will be displayed within 1 second, and remain until
    a remove function is called.
  */
  void draw_text( GlowArrayElem *e, char *text, int x, int y);

  //! Remove or inactivate the text for the specified object.
  /*!
    \param e	Object which the text should be removed for.
  */
  void remove_text( GlowArrayElem *e);

  //! Remove or inactivate texts for any object.
  /*! If the timer is running, it is stopped. If the text is diplayed, it is erased.
   */
  void remove();

  //! Draw the current tooltip text.
  /*! Function to draw the text and rectangle of the tooltip.
   */
  void draw();

};

/*@}*/
#endif
