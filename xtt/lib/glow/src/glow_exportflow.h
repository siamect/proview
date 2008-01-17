/* 
 * Proview   $Id: glow_exportflow.h,v 1.1 2008-01-17 14:17:05 claes Exp $
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

#ifndef glow_exportflow_h
#define glow_exportflow_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_ctx.h"

class GrowCtx;
class GrowRect;
class GrowLine;
class GrowConPoint;
class GrowText;
class GrowSubAnnot;
class GrowArc;

class GlowExportFlow {
 public:
  GlowExportFlow( GrowCtx *glow_ctx) : ctx( glow_ctx) {}
  int export_flow( char *filename);
  void array( GlowArray *o);
  void rect( GrowRect *o);
  void line( GrowLine *o);
  void conpoint( GrowConPoint *o);
  void text( GrowText *o);
  void annot( GrowSubAnnot *o);
  void arc( GrowArc *o);
  void point( GlowPoint *o, GlowTransform *trf);

  ofstream fp;
  GrowCtx *ctx;
};

#endif
