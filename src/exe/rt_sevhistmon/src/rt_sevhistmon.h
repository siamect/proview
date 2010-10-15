/* 
 * Proview   $Id: rt_sevhistmon.h,v 1.5 2008-11-24 15:21:33 claes Exp $
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

#include <vector>

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
  pwr_tBoolean  disabled;
};

class sev_sevhistobjectattr {
 public:
  pwr_tAttrRef 	aref;
  pwr_tString80 description;
  pwr_tString16 unit;
  pwr_tAName	aname;
  pwr_tRefId	refid;
  void 	        *datap;
  pwr_eType     type;
  unsigned int	size;
};

class sev_sevhistobject {
 public:
  pwr_tAttrRef 	aref;
  pwr_tAName	aname;
  pwr_tDeltaTime storagetime;
  pwr_tFloat32  deadband;
  pwr_tMask  	options;
  pwr_tRefId	sevid;
  pwr_tString80 description;
  pwr_tFloat32  scantime;
  unsigned int  datasize;
  pwr_tBoolean  disabled;
  vector<sev_sevhistobjectattr> sevhistobjectattrlist;  
};

class sev_sevhistthread {
 public:
  sev_sevhistthread() : configerror(0) {}
  pwr_tOid 	oid;
  pwr_tFloat32 	scantime;
  char 		nodename[80];
  pwr_tNid 	nid;
  unsigned int 	size;
  pwr_sClass_SevHistThread *threadp;
  pwr_tRefId 	refid;
  int 		configerror;
  vector<sev_sevhist> sevhistlist;  
  vector<sev_sevhistobject> sevhistobjectlist;  
};

class sev_node {
 public:
  sev_node() : is_server(0), connected(0), ctime(0), status(0) {}
  pwr_tNid 	nid;
  char 		name[80];
  int		is_server;
  int		connected;
  double       	ctime;
  pwr_tStatus   status;
};

class rt_sevhistmon {
 public:

  rt_sevhistmon() : m_msg_id(0), m_next_rix(0), m_loopcnt(0), m_allconnected(0), m_server_status(0), 
    m_swap(0) {}

  pwr_tStatus m_sts;
  vector<sev_sevhistthread> m_hs;
  vector<sev_node> m_nodes;
  unsigned int m_msg_id;
  unsigned int m_next_rix;
  unsigned int m_loopcnt;
  float m_scantime;
  pwr_sClass_SevHistMonitor *m_confp;
  pwr_tRefId m_conf_refid;
  int m_allconnected;
  pwr_tStatus m_server_status;
  int m_swap;

  int init();
  int init_objects();
  int init_sevhistobjects();
  void insert_sevhistobjectattr(pwr_sAttrRef    *aref,
                                pwr_tAName      objectname,
                                int hs_idx,
                                vector<sev_sevhistobjectattr>    *listP);
  int get_sevhistobjectattributes (pwr_tAName      objectname,
                                   vector<sev_sevhistobjectattr> *listP,
                                   int hs_idx,
                                   pwr_tBoolean    first);
  bool correct_histtype(const pwr_eType type);
  int close();
  int close_objects();
  int mainloop();
  void set_status();
  int connect();
  int retry_connect();
  bool send_connect( pwr_tNid nid, pwr_tStatus *sts);
  bool send_server_status_request( pwr_tStatus *sts);
  bool send_server_status_request( pwr_tNid nid, pwr_tStatus *sts);
  void receive_server_status( sev_sMsgServerStatus *msg, pwr_tNid nid);
  int send_itemlist( pwr_tNid nid);
  int send_data();
};
#endif
