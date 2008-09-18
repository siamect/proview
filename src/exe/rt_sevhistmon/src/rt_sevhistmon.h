/* 
 * Proview   $Id: rt_sevhistmon.h,v 1.3 2008-09-18 15:06:13 claes Exp $
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

#ifndef rt_sevhistmon_h
#define rt_sevhistmon_h

#include <vector.h>

#include "pwr.h"
#include "pwr_class.h"

class sev_sevhist {
 public:
  pwr_tAttrRef 	aref;
  pwr_tAName	aname;
  pwr_tRefId	refid;
  void 		*datap;
  pwr_tDeltaTime storagetime;
  pwr_tFloat32  deadband;
  pwr_tMask  	options;
  pwr_eType	type;
  unsigned int	size;
  pwr_tRefId	sevid;
  pwr_tString80 description;
  pwr_tString16 unit;
  pwr_tFloat32  scantime;
};

class sev_sevhistthread {
 public:
  pwr_tOid oid;
  pwr_tFloat32 scantime;
  char nodename[80];
  pwr_tNid nid;
  unsigned int size;
  vector<sev_sevhist> sevhistlist;  
};

class sev_node {
 public:
  pwr_tNid 	nid;
  char 		name[80];
};

class rt_sevhistmon {
 public:

  rt_sevhistmon() : m_msg_id(0), m_next_rix(0), m_loopcnt(0) {}

  pwr_tStatus m_sts;
  vector<sev_sevhistthread> m_hs;
  vector<sev_node> m_nodes;
  unsigned int m_msg_id;
  unsigned int m_next_rix;
  unsigned int m_loopcnt;
  float m_scantime;

  int init();
  int close();
  int mainloop();
  int connect();
  int send_itemlist( pwr_tNid nid);
  int send_data();
};
#endif
