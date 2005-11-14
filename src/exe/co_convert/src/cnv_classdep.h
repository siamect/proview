/* 
 * Proview   $Id: cnv_classdep.h,v 1.1 2005-11-14 16:11:23 claes Exp $
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

#ifndef cnv_classdep_h
#define cnv_classdep_h

#if defined __cplusplus
extern "C" {
#endif

#include <vector>
#include "pwr.h"
#include "cnv_ctx.h"


using namespace std;

class CdpItem {
 public:
  CdpItem() : idx(0), fth(0), fch(0), lch(0), fws(0), bws(0) {}

  pwr_tObjName name;
  pwr_tObjName supername;
  pwr_tObjName volumename;
  int idx;
  int fth;
  int fch;
  int lch;
  int fws;
  int bws;
};


class CnvClassDep {
 private:
  vector<CdpItem> classlist;

 public:
  CnvClassDep( CnvCtx *cnv_ctx) : ctx(cnv_ctx) {}
  virtual ~CnvClassDep() {}

  CnvCtx	*ctx;
  
  int read();
  bool find( char *name, int *idx);
  void print( int idx, int level);
  void print_html_classtable( int idx);
  void print_html_classtable_children( ofstream& fp, int idx);
};

#if defined __cplusplus
}
#endif
#endif




