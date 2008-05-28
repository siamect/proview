/* 
 * Proview   $Id: wb_pvd_gvl.h,v 1.4 2008-05-28 11:46:34 claes Exp $
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

#ifndef wb_pvd_gvl_h
#define wb_pvd_gvl_h

#include "rt_pvd_file.h"

class wb_pvd_gvl : public rt_pvd_file {
public:
  wb_pvd_gvl()
  { 
    pwr_tStatus sts;
    load( &sts); 
  }
  void load( pwr_tStatus *sts);
  void save( pwr_tStatus *sts);
  void save_item( pwr_tOix oix, ofstream &of);
  bool check_list( pwr_tStatus *sts);
};

#endif

