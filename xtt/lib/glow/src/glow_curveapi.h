#ifndef glow_curveapi_h
#define glow_curveapi_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef void *CurveCtx;
#ifndef glow_api_h
typedef void *GlowCtx;
#endif
#endif

#include "glow.h"

typedef CurveCtx *curve_tCtx;

void    curve_Zoom( CurveCtx *ctx, double factor);
void	curve_GetZoom( CurveCtx *ctx, double *factor_x, double *factor_y);
void	curve_GetPreferedZoomY( CurveCtx *ctx, int height, double *factor_y);

#if defined __cplusplus
}
#endif
#endif
