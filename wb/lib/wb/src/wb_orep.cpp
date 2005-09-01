/** 
 * Proview   $Id: wb_orep.cpp,v 1.5 2005-09-01 14:57:58 claes Exp $
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
