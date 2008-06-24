/* 
 * Proview   $Id: rt_pvd_udb.h,v 1.2 2008-06-24 07:15:31 claes Exp $
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

// Provider for user database

#ifndef rt_pvd_udb_h
#define rt_pvd_udb_h

#include "rt_pvd_file.h"
#include "co_user.h"

class rt_pvd_udb : public rt_pvd_file {
public:
  rt_pvd_udb( pvd_eEnv env = pvd_eEnv_Wb) : rt_pvd_file(env), gu(0), menu_cnt(0)
  { 
    pwr_tStatus sts;
    load( &sts); 
  }
  void load( pwr_tStatus *rsts);
  void load_systemgroup( SystemList *systemgroup);
  void load_user( UserList *user, SystemList *sg);
  void save( pwr_tStatus *sts);
  char *groupname( char *name);
  void writeAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer);

  GeUser *gu;
  int menu_stack[100];
  int menu_cnt;
};

#endif

