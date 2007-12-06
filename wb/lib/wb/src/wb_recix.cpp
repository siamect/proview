/* 
 * Proview   $Id: wb_recix.cpp,v 1.1 2007-12-06 10:55:04 claes Exp $
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

/* wb_recix.cpp    Recycle objid. Stores name and oix of deleted tree
                   to recycle the oix when a similar tree is copied.*/

#include <iostream.h>
#include "wb_recix.h"

bool wb_recix::add( const char *str, pwr_tOix ix) {
  string key = string(str);
  m_ix[ key] = ix;

  return true;
}

bool wb_recix::add_object_tree( wb_object &o) {

  clear();

  wb_object p = o.parent();
  if ( p)
    strcpy( m_srcroot, p.longName().c_str());

  add( o.longName().c_str(), o.oid().oix);
  for ( wb_object ch = o.first(); ch; ch = ch.after())
    add_object_ctree( ch);


  // for (  ix_iterator it = m_ix.begin(); it != m_ix.end(); it++) {
  //   cout << " Object: " << it->second << "  " << it->first << endl;
  // }  

  return true;
}

bool wb_recix::add_object_ctree( wb_object &o) {

  add( o.longName().c_str(), o.oid().oix);
  for ( wb_object ch = o.first(); ch; ch = ch.after())
    add_object_tree( ch);
  return true;
}

bool wb_recix::get( char *str, pwr_tOix *ix) {
  pwr_tOName keystr;

  if ( (int) strlen(str) <= m_destroot_len)
    return false;

  if ( strcmp( m_srcroot, "") != 0) {
    strcpy( keystr, m_srcroot);
    if ( m_destroot_len)
      strcat( keystr, &str[m_destroot_len]);
    else {
      strcat( keystr, "-");
      strcat( keystr, str);
    }
  }
  else {
    if ( m_destroot_len)
      strcat( keystr, &str[m_destroot_len + 1]);
    else {
      strcat( keystr, str);
    }
  }

  string key = string(keystr);

  ix_iterator it = m_ix.find( key);
  if ( it == m_ix.end())
    return false;
  *ix = it->second;
  return true;
}

void wb_recix::clear() {
  m_ix.clear();
}





