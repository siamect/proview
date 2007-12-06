/* 
 * Proview   $Id: wb_recix.h,v 1.1 2007-12-06 10:55:04 claes Exp $
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

#ifndef wb_recix_h
#define wb_recix_h

#include <map>
#include <string>
#include "pwr.h"
#include "wb_object.h"

using namespace std;

class wb_recix {
  typedef map< string, pwr_tOix>::iterator ix_iterator;
  
 private:
  map< string, pwr_tOix> m_ix;
  pwr_tOName m_srcroot;
  pwr_tOName m_destroot;
  int m_destroot_len;

  bool add_object_ctree( wb_object &o);

 public:
  wb_recix() : m_destroot_len(0) {
    strcpy( m_srcroot, "");
    strcpy( m_destroot, "");
  }
  bool add_object_tree( wb_object &o);
  bool add( const char *str, pwr_tOix ix);
  void set_destination( char *dest) { 
    strncpy( m_destroot, dest, sizeof(m_destroot));
    m_destroot_len = strlen(m_destroot);
  }
  bool get( char *str, pwr_tOix *ix);
  void clear();
};

#endif




