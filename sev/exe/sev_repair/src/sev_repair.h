/* 
 * Proview   $Id$
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
 */

#ifndef sev_repair_h
#define sev_repair_h

#include <vector>
#include <map>

#include "pwr.h"
#include "pwr_class.h"
#include "rt_sev_net.h"
#include "sev_db.h"

class sev_repair {
 public:

  sev_repair() {}

  pwr_tStatus m_sts;
  sev_db *m_db;

  int init();
  int check();
};
#endif
