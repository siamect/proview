/* 
 * Proview   $Id: cnv_wbltoxtthelp.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_wbltoxtthelp_h
#define cnv_wbltoxtthelp_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_readwbl.h"
#include "cnv_wblto.h"

using namespace std;

class CnvReadWbl;

class CnvWblToXtthelp : public CnvWblTo {
 public:
  CnvWblToXtthelp( CnvCtx *cnv_ctx) : 
    ctx(cnv_ctx), xtthelp_index_open(0), xtthelp_in_topic(0) {}
  virtual ~CnvWblToXtthelp() {}

  CnvCtx  	*ctx;
  ofstream    	fp_xtthelp_index;
  int	      	xtthelp_index_open;
  int         	xtthelp_in_topic;
  ofstream 	fp_tmp;

  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int attribute_exec();
  int bit_exec();
  Cnv_eWblToType type() { return Cnv_eWblToType_Xtthelp;}
  int index_open() { return xtthelp_index_open;}
    
};

#if defined __cplusplus
}
#endif
#endif

