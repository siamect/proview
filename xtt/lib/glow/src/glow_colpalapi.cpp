#include "glow_std.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "glow_ctx.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"

extern "C" void	colpal_EnableEvent( ColPalCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

void	colpal_DisableEvent( ColPalCtx *ctx, glow_eEvent event)
{
  ctx->disable_event( event);
}

extern "C" void	colpal_DisableEventAll( ColPalCtx *ctx)
{
  ctx->disable_event_all();
}

void colpal_GetCtxUserData( colpal_tCtx ctx, void **user_data)
{
  ((GlowCtx *)ctx)->get_user_data( user_data);
}

void colpal_SetCtxUserData( colpal_tCtx ctx, void *user_data)
{
  ((GlowCtx *)ctx)->set_user_data( user_data);
}

void colpal_GetCurrentColors( colpal_tCtx ctx, glow_eDrawType *fill_color, 
	glow_eDrawType *border_color, glow_eDrawType *text_color)
{
  *fill_color = ctx->current_fill;
  *border_color = ctx->current_border;
  *text_color = ctx->current_text;
}

void colpal_SetCurrentColors( colpal_tCtx ctx, glow_eDrawType fill_color, 
	glow_eDrawType border_color, glow_eDrawType text_color)
{
  ctx->current_fill = fill_color;
  ctx->current_border = border_color;
  ctx->current_text = text_color;
}

