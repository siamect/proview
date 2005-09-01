/** 
 * Proview   $Id: co_status.h,v 1.2 2005-09-01 14:57:52 claes Exp $
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

#ifndef co_status_h
#define co_status_h

#include <string>
#include "pwr.h"
extern "C" {
#include "co_msg.h"
}
using namespace std;

class co_status
{
 protected:
  pwr_tStatus m_sts;

 public:
  co_status() : m_sts(0) {}
  co_status( pwr_tStatus sts) : m_sts(sts) {}
  virtual ~co_status() {};
  pwr_tStatus sts() const { return m_sts;}
  bool evenSts() const { return EVEN(m_sts);}
  bool oddSts() const { return ODD(m_sts);}
  virtual string what() const
  { 
    char msg[200];

    msg_GetMsg( m_sts, msg, sizeof(msg));
    string s(msg);
    return s;
  }
};

#endif







