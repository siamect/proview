/* 
 * Proview   $Id: sev_server.h,v 1.2 2008-09-05 08:38:58 claes Exp $
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

#ifndef sev_server_h
#define sev_server_h

#include <vector.h>
#include <map.h>

#include "pwr.h"
#include "pwr_class.h"
#include "rt_sev_net.h"
#include "sev_db.h"

class sev_node {
 public:
  pwr_tNodeId 	nid;
  char 		name[80];
};

class sev_refid {
 public:
  sev_refid( pwr_tRefId rid) : id(rid) {}
  bool operator<(const sev_refid& x) const {
    if ( id.nid < x.id.nid)
      return true;
    if ( id.rix < x.id.rix)
      return true;
    return false;
  }
  pwr_tRefId id;
};

class sev_item_key {
 public:
  sev_item_key( pwr_tOid oid, char *aname) : m_oid(oid) 
    {
      strncpy( m_aname, aname, sizeof(m_aname));
    }

  bool operator<(const sev_item_key& x) const {
    if ( m_oid.vid < x.m_oid.vid)
      return true;
    if ( m_oid.oix < x.m_oid.oix)
      return true;
    return strcmp( m_aname, x.m_aname);
  }
  pwr_tOid m_oid;
  pwr_tAName m_aname;
};

class sev_server {
 public:

  sev_server() : m_msg_id(0) {}

  typedef map<sev_refid, unsigned int>::iterator iterator_refid;
  typedef map<sev_item_key, unsigned int>::iterator iterator_item_key;

  pwr_tStatus m_sts;
  vector<sev_node> m_nodes;
  map<sev_refid, unsigned int> m_refid;
  map<sev_item_key, unsigned int> m_item_key;
  unsigned int m_msg_id;
  sev_db *m_db;

  int init();
  int connect();
  int request_items( pwr_tNid nid);
  int sev_server::mainloop();
  int check_histitems( sev_sMsgHistItems *msg, unsigned int size);
  int receive_histdata( sev_sMsgHistDataStore *msg, unsigned int size);
  int send_histdata( qcom_sQid tgt, sev_sMsgHistDataGetRequest *msg, unsigned int size);
  int send_itemlist( qcom_sQid tgt);
  int delete_item( qcom_sQid tgt, sev_sMsgHistItemDelete *rmsg);
  void garbage_collector();
};
#endif
