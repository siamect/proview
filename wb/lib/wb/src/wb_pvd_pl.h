/* 
 * Proview   $Id: wb_pvd_pl.h,v 1.3 2006-09-14 14:16:20 claes Exp $
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

#ifndef wb_pvd_pl_h
#define wb_pvd_pl_h

#include "co_pvd_file.h"

typedef enum {
  pl_mFlags_ProjectModified 	= 1 << 16,
  pl_mFlags_PathModified 	= 1 << 17,
  pl_mFlags_Disabled 		= 1 << 18
} pl_mFlags;

class wb_pvd_pl : public co_pvd_file {
public:
  wb_pvd_pl()
  { 
    pwr_tStatus sts;
    load( &sts); 
  }
  void writeAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer);
  void createObject( co_procom *pcom, pwr_tOix destoix, int desttype,
		     pwr_tCid cid, char *name);
  void load( pwr_tStatus *sts);
  void save( pwr_tStatus *sts);
  bool check_list( pwr_tStatus *sts);
  void process_list( pwr_tStatus *sts);
  void save_list( pwr_tStatus *sts);
  void save_item( pwr_tOix oix, ofstream &of);
  char *basename( char *version);
  bool create_hier( char *hier, pwr_tOix *oix);

  static void confirm_actions_ok( void *ctx, void *data);
};

#endif

