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

typedef ColPalCtx *colpal_tCtx;

void colpal_EnableEvent( ColPalCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));
void colpal_DisableEvent( ColPalCtx *ctx, glow_eEvent event);
void colpal_DisableEventAll( ColPalCtx *ctx);
void colpal_GetCtxUserData( colpal_tCtx ctx, void **user_data);
void colpal_SetCtxUserData( colpal_tCtx ctx, void *user_data);
void colpal_GetCurrentColors( colpal_tCtx ctx, glow_eDrawType *fill_color, 
	glow_eDrawType *border_color);
void colpal_SetCurrentColors( colpal_tCtx ctx, glow_eDrawType fill_color, 
	glow_eDrawType border_color);

#if defined __cplusplus
}
#endif
#endif
