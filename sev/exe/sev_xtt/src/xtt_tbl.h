/* 
 * Proview   $Id: xtt_tbl.h,v 1.1 2008-07-17 11:18:31 claes Exp $
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

#ifndef xtt_tbl_h
#define xtt_tbl_h

#include "glow.h"
#include "rt_sevcli.h"

/* xtt_tbl.h -- Sev Table Viewer */


class TblNav;
class XttDsHist;

class XttTbl {
 public:
  XttTbl( void *xn_parent_ctx, sevcli_tCtx xn_sevcli);
  void 	*parent_ctx;
  char 	name[80];
  TblNav	*tblnav;
  sevcli_tCtx  sevcli;

  virtual void message( char severity, char *message) {}
  virtual XttDsHist *dshist_new( pwr_tOid oid, char *aname) { return 0;}

  void activate_opendshist();
  static void message( void *attr, char severity, char *message);
  virtual ~XttTbl();
};


#endif








