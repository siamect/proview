#include "glow_std.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "glow_growctx.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"

void	curve_Zoom( CurveCtx *ctx, double factor)
{
  ctx->zoom( factor);
}

void	curve_GetZoom( CurveCtx *ctx, double *factor_x, double *factor_y)
{
  ctx->get_zoom( factor_x, factor_y);
}

void	curve_GetPreferedZoomY( CurveCtx *ctx, int height, double *factor_y)
{
  ctx->get_prefered_zoom_y( height, factor_y);
}












