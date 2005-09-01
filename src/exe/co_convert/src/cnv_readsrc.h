/* 
 * Proview   $Id: cnv_readsrc.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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
 */

#ifndef cnv_readsrc_h
#define cnv_readsrc_h

/* cnv_readsrc.h -- Read source files */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_ctx.h"
#include "cnv_readwbl.h"

using namespace std;


class CnvReadSrc {
 public:
  CnvReadSrc( CnvCtx *cnv_ctx) :
    ctx(cnv_ctx), state(0), linetype(cread_eLine_No), doc_fresh(0), 
    html_class_open(0)
    {}

  CnvCtx 	*ctx;
  int	 	state;
  char		src_aref[80];
  char		src_aref_text[200];
  ofstream 	fp_src_html;
  cread_eLine	linetype;
  int		doc_fresh;
  int		html_class_open;

  int read_src( char *filename);
  int html_init( char *filename);
  int html_close();
  int html_line( char *line);
  int html_aref();
  static void filename_to_html( char *fname, char *filename);
  char *low( char *in);
};

#if defined __cplusplus
}
#endif
#endif



