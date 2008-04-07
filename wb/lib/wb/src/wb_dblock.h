/* 
 * Proview   $Id: wb_dblock.h,v 1.4 2008-04-07 14:53:06 claes Exp $
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

#ifndef wb_dblock_h
#define wb_dblock_h

#include <vector.h>

class wb_lockfile
{
 public:
  wb_lockfile( char *name) : removed(false)
    { strcpy( fname, name); }
  pwr_tFileName fname;
  pwr_tTime date;
  bool removed;
};

class wb_dblock
{
 private:
  static vector<wb_lockfile> m_lockfiles;

  static char *lockname( char *name);
  static char *lockvname( char *fname);

 public:
  static bool is_locked( char *name, char *user = 0);
  static void dblock( char *name);
  static void dbunlock( char *name);
  static void dbunlock_all();
  static bool check( char *name);
};

#endif
