/* 
 * Proview   $Id: glow_curveapi.h,v 1.4 2007-01-15 13:19:09 claes Exp $
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

#ifndef glow_curveapi_h
#define glow_curveapi_h

#include "glow.h"

#if defined __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef void *CurveCtx;
#ifndef glow_api_h
typedef void *GlowCtx;
#endif
#endif


typedef CurveCtx *curve_tCtx;

void curve_Zoom( CurveCtx *ctx, double factor);
void curve_Unzoom( CurveCtx *ctx);
void curve_GetZoom( CurveCtx *ctx, double *factor_x, double *factor_y);
void curve_GetPreferedZoomY( CurveCtx *ctx, int height, double *factor_y);
void curve_Scroll( CurveCtx *ctx, double value);

#if defined __cplusplus
}
#endif
#endif
