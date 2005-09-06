/* 
 * Proview   $Id: wb_attrname.cpp,v 1.5 2005-09-06 10:43:30 claes Exp $
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

#include <string.h>
#include "wb_attrname.h"

wb_attrname::wb_attrname( const char *name)
{
  try {
    char str[80];
    if ( *name != '.') {
      strcpy( str, ".");
      strcat( str, name);
    }
    else
      strcpy( str, name);
    m_nrep = new wb_nrep( str);
    m_sts = LDH__SUCCESS;
    m_nrep->ref();
  }
  catch ( wb_error &e) {
    m_sts = e.sts();
    m_nrep = 0;
  }
}

