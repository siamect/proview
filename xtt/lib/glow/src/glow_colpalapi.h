/* 
 * Proview   $Id: glow_colpalapi.h,v 1.3 2005-09-01 14:57:53 claes Exp $
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

#ifndef glow_colpalapi_h
#define glow_colpalapi_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef void *ColPalCtx;
#ifndef glow_api_h
typedef void *GlowCtx;
#endif
#endif

#include "glow.h"
 
/*! \file glow_colpalapi.h
    \brief Contains c API for the color palette, i.e. the ColPalCtx class. */
/*! \addtogroup GlowColPalApi */
/*@{*/

typedef ColPalCtx *colpal_tCtx;

  //! Enable an event an register a callback function for the event.
  /*!
    \param ctx		Colorpalette context.
    \param event	Event to enable.
    \param event_type	Eventtype of the event.
    \param event_cb	Callback function for the event.
  */
  void colpal_EnableEvent( ColPalCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));

  //! Disable an event.
  /*!
    \param ctx		Colorpalette context.
    \param event	Event to disable.
  */
  void colpal_DisableEvent( ColPalCtx *ctx, glow_eEvent event);

  //! Disable all events.
  /*!
    \param ctx		Colorpalette context.
  */
  void colpal_DisableEventAll( ColPalCtx *ctx);

  //! Get user data.
  /*!
    \param ctx		Colorpalette context.
    \param user_data	User data.
  */
  void colpal_GetCtxUserData( colpal_tCtx ctx, void **user_data);

  //! Set user data.
  /*!
    \param ctx		Colorpalette context.
    \param user_data	User data.
  */
  void colpal_SetCtxUserData( colpal_tCtx ctx, void *user_data);

  //! Get the currently selected colors.
  /*!
    \param ctx		Colorpalette context.
    \param fill_color	Selected fill color.
    \param border_color	Selected border color.
    \param text_color	Selected text color.
  */
  void colpal_GetCurrentColors( colpal_tCtx ctx, glow_eDrawType *fill_color, 
				glow_eDrawType *border_color, glow_eDrawType *text_color);

  //! Set the currently selected colors.
  /*!
    \param ctx		Colorpalette context.
    \param fill_color	Fill color.
    \param border_color	Border color.
    \param text_color	Text color.
  */
  void colpal_SetCurrentColors( colpal_tCtx ctx, glow_eDrawType fill_color, 
				glow_eDrawType border_color, glow_eDrawType text_color);

/*@}*/
#if defined __cplusplus
}
#endif
#endif
