/** 
 * Proview   $Id: co_error.h,v 1.2 2005-09-01 14:57:52 claes Exp $
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

#ifndef co_error_h
#define co_error_h

#include "pwr.h"
#include "co_status.h"

class co_error : public co_status
{
 public:
  co_error() {};
  co_error( pwr_tStatus sts) : co_status(sts) {}
};

class co_error_str : public co_error
{
  string m_error_str;
    
 public:
  co_error_str(string str) : m_error_str(str) {} 
  co_error_str( pwr_tStatus sts, string str): co_error(sts), m_error_str(str) {}
  string what() const
  {
    string s;
    if ( m_sts)
      s = m_error_str + ", " + co_error::what();
    else
      s = m_error_str;
    return s;
  }
};

#endif
