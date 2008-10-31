/* 
 * Proview   $Id: glow_curveapi.cpp,v 1.4 2008-10-31 12:51:35 claes Exp $
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

#include "glow_std.h"

#include <iostream>
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

void	curve_Unzoom( CurveCtx *ctx)
{
  ctx->unzoom();
}

void	curve_GetZoom( CurveCtx *ctx, double *factor_x, double *factor_y)
{
  ctx->get_zoom( factor_x, factor_y);
}

void	curve_GetPreferedZoomY( CurveCtx *ctx, int height, double *factor_y)
{
  ctx->get_prefered_zoom_y( height, factor_y);
}

void curve_Scroll( CurveCtx *ctx, double value)
{
  ctx->scroll( value);
}










