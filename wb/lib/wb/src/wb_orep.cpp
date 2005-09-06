/* 
 * Proview   $Id: wb_orep.cpp,v 1.7 2005-09-06 10:43:31 claes Exp $
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

#include "wb_orep.h"
#include "co_time.h"

wb_orep::wb_orep() : m_nRef(0) 
{
  // printf( "orep C: %x %d\n", this, ++ocnt);
}

void wb_orep::unref()
{
  if (--m_nRef == 0) {
    // printf( "orep  : %x %d\n", this, --ocnt);  
    delete this;
  }
}

wb_orep *wb_orep::ref()
{
  m_nRef++;
  return this;
}

pwr_tTime wb_orep::modTime()
{
  pwr_tTime t = ohTime();
  pwr_tTime rbt = rbTime();
  pwr_tTime dbt = dbTime();

  if ( time_Acomp( &rbt, &t) == 1)
    t = rbt;
  if ( time_Acomp( &dbt, &rbt) == 1)
    t = dbt;
  return t;
}

pwr_tTime wb_orep::treeModTime()
{
  pwr_tStatus sts;
  pwr_tTime t = modTime();
  pwr_tTime tchild;
  wb_orep *after;

  for ( wb_orep *child = first( &sts); 
        ODD(sts);
	child = after) {
    child->ref();

    tchild = child->treeModTime();
    if ( time_Acomp( &tchild, &t) == 1)
      t = tchild;

    after = child->after( &sts);
    child->unref();
  }
  return t;
}
