/* 
 * Proview   $Id: wb_tdef.h,v 1.6 2005-09-06 10:43:32 claes Exp $
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

#ifndef wb_tdef_h
#define wb_tdef_h

#include "wb_tdrep.h"
#include "pwr.h"
#include "wb_orep.h"
#include "wb_name.h"
#include "wb_ldh.h"

class wb_tdrep;
class wb_orep;

class wb_tdef : public wb_status
{
public:
  wb_tdrep *m_tdrep;
    
  wb_tdef();
  wb_tdef( wb_tdrep *tdrep);
  wb_tdef( wb_adef&);
  wb_tdef(wb_mvrep *, pwr_tTid);
  wb_tdef(const wb_tdef&);
  wb_tdef& operator=(const wb_tdef&);
  ~wb_tdef();

  operator bool() const { return m_tdrep != 0;}
  operator wb_tdrep*() const;
  bool operator==(wb_tdef&);
    
  size_t size();
  pwr_tTid tid();
  int nElement();
  const char *name() const;
  wb_name longName();
    
private:
  void check() const;            
};

#endif



