/* 
 * Proview   $Id: sev_server.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#include <vector>
#include <map>

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

// Struct for binary tree item
typedef struct {
  pwr_tOid oid;
  pwr_tAName aname;
} sev_sItemKey;

typedef struct {
  tree_sNode  node;
  sev_sItemKey key;
  int idx;
} sev_sItem;

class sev_server {
 public:

  sev_server() : m_server_status(0), m_item_key(0), m_msg_id(0) {}

  typedef map<sev_refid, unsigned int>::iterator iterator_refid;

  pwr_tStatus m_sts;
  pwr_tStatus m_server_status;
  vector<sev_node> m_nodes;
  map<sev_refid, unsigned int> m_refid;
  tree_sTable *m_item_key;
  unsigned int m_msg_id;
  sev_db *m_db;
  int m_noneth;

  int init( int noneth);
  int connect();
  int request_items( pwr_tNid nid);
  int mainloop();
  int check_histitems( sev_sMsgHistItems *msg, unsigned int size);
  int receive_histdata( sev_sMsgHistDataStore *msg, unsigned int size);
  int send_histdata( qcom_sQid tgt, sev_sMsgHistDataGetRequest *msg, unsigned int size);
  int send_itemlist( qcom_sQid tgt);
  int send_server_status( qcom_sQid tgt);
  int delete_item( qcom_sQid tgt, sev_sMsgHistItemDelete *rmsg);
  void garbage_collector();
};
#endif
