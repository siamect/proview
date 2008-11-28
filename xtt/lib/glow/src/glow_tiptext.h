/* 
 * Proview   $Id: glow_tiptext.h,v 1.5 2008-11-28 17:13:45 claes Exp $
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

#ifndef glow_tiptext_h
#define glow_tiptext_h

#include <iostream>
#include <fstream>

#include "glow.h"

class GrowCtx;
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
  GlowTipText( GrowCtx *gctx) : ctx(gctx), active(false), timer_id(0), text_size(2) {}

  //! Destructor
  /*! Removes the timer if it is set.
   */
  ~GlowTipText();

  GrowCtx *ctx;		//!< Glow context.
  int text_x;		//!< x coordinate in pixels for text.
  int text_y;		//!< y coordinate in pixels for text.
  int text_width;	//!< Text width in pixels.
  int text_height;	//!< Text height in pixels.
  int text_descent;	//!< Text descent in pixels.
  GlowArrayElem *text_object;  //!< Object that activated the current tip text.
  char tiptext[200];	//!< Current tip text.
  bool active;		//!< Tip text is active, i.e. timer is running or text is displayed.
  void *timer_id;	//!< Timer id.
  int  text_size;	//!< Text size.

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
