/* 
 * Proview   $Id: rt_sevhistmon.cpp,v 1.6 2008-11-24 15:21:33 claes Exp $
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


#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_error.h"
#include "rt_gdh.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"
#include "rt_errh.h"
#include "rt_sevhistmon.h"
#include "rt_sev_net.h"
#include "rt_sev_msg.h"
#include "rt_pwr_msg.h"

#define sevclient_cQix 121

int rt_sevhistmon::init()
{
  pwr_tStatus sts;
  qcom_sQid qid;
  qcom_sQid 		qini;
  qcom_sNode		node;
  pwr_tNid		nid;
  pwr_tOid		conf_oid;
  pwr_tOName oname;
  
  errh_Init("pwr_sevhistmon", errh_eNAnix);
  // errh_SetStatus( PWR__SRVSTARTUP);

  sts = gdh_Init( "rt_sevhistmon");
  if ( EVEN(sts)) throw co_error(sts);

  // Get the config object
  sts = gdh_GetClassList( pwr_cClass_SevHistMonitor, &conf_oid);
  if ( EVEN(sts)) {
    errh_CErrLog( PWR__SRVNOTCONF, 0);
    exit(0);
  }

  m_sts = gdh_ObjidToName( conf_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
  if ( EVEN(m_sts)) throw co_error(m_sts);

  m_sts = gdh_RefObjectInfo( oname, (void **)&m_confp, &m_conf_refid, sizeof(*m_confp));
  if ( EVEN(m_sts)) throw co_error(m_sts);

  m_confp->Status = m_server_status = PWR__SRVSTARTUP;
  m_scantime = m_confp->ScanTime;
  if ( m_scantime < 0.02)
    m_scantime = 0.02;

  // Create a queue to server
  qcom_sQattr attr;

  qid.qix = sev_eProcSevClient;
  qid.nid = 0;
  attr.type = qcom_eQtype_private;
  attr.quota = 100;

  if (!qcom_CreateQ(&sts, &qid, &attr, "SevClient")) {
    if ( sts == QCOM__QALLREXIST) {
      if ( !qcom_AttachQ(&sts, &qid)) {
	if ( !qcom_DeleteQ(&sts, &qid))
	  throw co_error(sts);	     
	if ( !qcom_CreateQ(&sts, &qid, &attr, "SevClient"))
	  throw co_error(sts);
      }      
    }
    else
      throw co_error( sts);
  }

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &qid, &qini))
    throw co_error(sts);

  // Get all qcom nodes
  qcom_MyNode( &m_sts, &node);
  if ( EVEN(m_sts)) throw co_error(m_sts);

  sev_node myn;
  myn.nid = node.nid;
  strncpy( myn.name, node.name, sizeof(myn.name));
  m_nodes.push_back( myn);

  for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
    sev_node n;

    n.nid = node.nid;
    strncpy( n.name, node.name, sizeof(n.name));
    m_nodes.push_back( n);
  }

  while(EVEN(gdh_NethandlerRunning()))
    sleep(1);

  return init_objects();
}

int rt_sevhistmon::init_objects()
{
  pwr_tStatus sts;
  pwr_tOid hs_oid;
  pwr_tAttrRef h_aref;
  pwr_tAttrRef uaref;
  pwr_tTid a_tid;
  unsigned int a_size, a_offset, a_dim;
  pwr_tAName hname;
  pwr_tOName oname;

  // Get all SevHist and SevHistThread objects
  int thread_cnt = 0;
  for ( sts = gdh_GetClassList( pwr_cClass_SevHistThread, &hs_oid);
	ODD(sts);
	sts = gdh_GetNextObject( hs_oid, &hs_oid)) {
    sev_sevhistthread hs;
  
    m_sts = gdh_ObjidToName( hs_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    m_sts = gdh_RefObjectInfo( oname, (void **)&hs.threadp, &hs.refid, sizeof(*hs.threadp));
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs.oid = hs_oid;
    hs.scantime = hs.threadp->ScanTime;
    strncpy( hs.nodename, hs.threadp->ServerNode, sizeof(hs.nodename));
    hs.size = 0;

    bool found = false;
    for ( unsigned int i = 0; i < m_nodes.size(); i++) {
      if ( cdh_NoCaseStrcmp( hs.nodename, m_nodes[i].name) == 0) {
	hs.nid = m_nodes[i].nid;
	found = true;
	break;
      }
    }
    m_confp->ThreadObjects[thread_cnt++] = hs_oid;
    if ( !found) {
      pwr_tOName oname;

      m_sts = gdh_ObjidToName( hs_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
      if ( EVEN(m_sts)) throw co_error(m_sts);

      errh_Error( "Unknown nodename, %s", oname);
      hs.threadp->Status = SEV__UNKNOWNNODE;
      hs.configerror = 1;
    }
    else
      hs.threadp->Status = SEV__INIT;
    m_hs.push_back( hs);
  }

  for ( int i = thread_cnt; i < int(sizeof(m_confp->ThreadObjects)/sizeof(m_confp->ThreadObjects[0])); i++)
    m_confp->ThreadObjects[i] = pwr_cNOid;

  for ( sts = gdh_GetClassListAttrRef( pwr_cClass_SevHist, &h_aref);
	ODD(sts);
	sts = gdh_GetNextAttrRef( pwr_cClass_SevHist, &h_aref, &h_aref)) {
    sev_sevhist h;
    pwr_sClass_SevHist *h_p;
    int hs_idx;
    
    m_sts = gdh_AttrRefToPointer( &h_aref, (void **)&h_p);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    h.aref = h_p->Attribute;

    m_sts = gdh_AttrrefToName( &h_aref, hname, sizeof(hname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs_idx = -1;
    for ( int i = 0; i < (int) m_hs.size(); i++) {
      if ( cdh_ObjidIsEqual( h_p->ThreadObject, m_hs[i].oid)) {
	hs_idx = i;
	break;
      }
    }
    if ( hs_idx == -1) {
      errh_Error( "Invalid HistServerObject %s", hname);
      continue;
    }
    
    m_sts = gdh_GetAttributeCharAttrref( &h_p->Attribute, &a_tid, &a_size, &a_offset, &a_dim);
    if ( EVEN(m_sts)) {
      errh_Error( "Invalid SevHist Attribute %s", hname);
      continue;
    }

    h.sevid.nid = m_nodes[0].nid;
    h.sevid.rix = m_next_rix++;
    h.storagetime = h_p->StorageTime;
    h.deadband = h_p->DeadBand;
    h.options = h_p->Options;
    strncpy( h.description, h_p->Description, sizeof(h.description));

    // Get unit from attribute object
    sts = gdh_ArefANameToAref( &h_p->Attribute, "Unit", &uaref);
    if ( ODD(sts)) {
      sts = gdh_GetObjectInfoAttrref( &uaref, &h.unit, sizeof(h.unit));
      if ( EVEN(sts))
	strcpy( h.unit, "");
    }
    else
      strcpy( h.unit, "");

    h.type = (pwr_eType) a_tid;
    h.size = a_size;
    h.scantime = m_hs[hs_idx].scantime;

    switch ( h.type) {
    case pwr_eType_Boolean:
    case pwr_eType_Int64:
    case pwr_eType_Int32:
    case pwr_eType_Int16:
    case pwr_eType_Int8:
    case pwr_eType_UInt64:
    case pwr_eType_UInt32:
    case pwr_eType_UInt16:
    case pwr_eType_UInt8:
    case pwr_eType_Float32:
    case pwr_eType_Float64:
    case pwr_eType_String:
    case pwr_eType_Time:
      break;
    default:
      errh_Error( "Invalid SevHist Attribute type %s", hname);
      continue;
    }
    m_sts = gdh_AttrrefToName( &h_p->Attribute, h.aname, sizeof(h.aname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    m_sts = gdh_RefObjectInfo( h.aname, &h.datap, &h.refid, h.size);
    if ( EVEN(m_sts)) {
      errh_Error( "Unable to link to Attribute %s", h.aname);
      continue;
    }
    m_hs[hs_idx].size += h.size;
    m_hs[hs_idx].sevhistlist.push_back(h);
    m_hs[hs_idx].threadp->NoOfItems++;
  }

  set_status();

  return 1;
}

int rt_sevhistmon::close_objects()
{
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++)
      gdh_UnrefObjectInfo( m_hs[i].sevhistlist[j].refid);
    gdh_UnrefObjectInfo( m_hs[i].refid);
  }
  m_hs.clear();

  return 1;
}


int rt_sevhistmon::close()
{
  close_objects();
  m_confp->Status = PWR__SRVTERM;
  gdh_UnrefObjectInfo( m_conf_refid);
  return 1;
}

int rt_sevhistmon::send_data()
{
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int msize;
  sev_sMsgHistDataStore *msg;
  sev_sHistData *dp;
  int		stime;
  pwr_tTime	current_time;
  pwr_tStatus   conf_sts = SEV__SUCCESS;

  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;

    stime = int(m_hs[i].scantime / m_scantime + 0.5);
    if ( !stime || m_loopcnt % stime != 0)
      continue;
	   
    m_hs[i].threadp->ScanCount++;

    msize = m_hs[i].sevhistlist.size() * (sizeof(*dp) - sizeof(dp->data)) + m_hs[i].size; 
    msize += sizeof(*msg) - sizeof(msg->Data);

    msg = (sev_sMsgHistDataStore *) qcom_Alloc(&lsts, msize);

    put.size = msize;
    put.data = msg;

    msg->Type = sev_eMsgType_HistDataStore;
    time_GetTime( &current_time);
    msg->Time = net_TimeToNetTime( &current_time);

    dp = (sev_sHistData *) &msg->Data;
    for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++) {
      dp->sevid = m_hs[i].sevhistlist[j].sevid;
      dp->type = m_hs[i].sevhistlist[j].type;
      dp->size = m_hs[i].sevhistlist[j].size;
      memcpy( &dp->data, m_hs[i].sevhistlist[j].datap, dp->size);
      dp = (sev_sHistData *)((char *)dp + sizeof( *dp) - sizeof(dp->data) +  dp->size);
    }

    tgt.nid = m_hs[i].nid;
    tgt.qix = sev_eProcSevServer;
    
    put.reply.nid = m_nodes[0].nid;
    put.reply.qix = sev_eProcSevClient;
    put.type.b = (qcom_eBtype) sev_cMsgClass;
    put.type.s = (qcom_eStype) sev_eMsgType_HistDataStore;
    put.msg_id = m_msg_id++;

    if ( !qcom_Put( &sts, &tgt, &put)) {
      m_hs[i].threadp->ErrorCount++;
      m_hs[i].threadp->Status = sts;
      conf_sts = sts;
      qcom_Free( &sts, put.data);
      continue;
    }    
    m_hs[i].threadp->SendCount++;
    m_hs[i].threadp->Status = sts;
  }

  set_status();
  return 1;
}

void rt_sevhistmon::set_status()
{
  pwr_tStatus sts = m_server_status;

  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( EVEN( m_hs[i].threadp->Status) &&
	 errh_Severity( m_hs [i].threadp->Status) > errh_Severity( sts)) {
      sts = m_hs[i].threadp->Status;
    }
  }
  m_confp->Status = sts;
}

int rt_sevhistmon::retry_connect()
{
  pwr_tStatus	sts;  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {

    if ( m_nodes[i].is_server && !m_nodes[i].connected) {
      m_nodes[i].ctime += m_scantime;
      if ( m_nodes[i].ctime > 60) {
	m_nodes[i].ctime = 0;

	send_connect( m_nodes[i].nid, &sts);
      }
    }
  }
  return 1;
}

int rt_sevhistmon::connect()
{
  pwr_tStatus	sts;  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {

    // Check if this node should be connected
    bool found = false;
    for ( unsigned int j = 0; j < m_hs.size(); j++) {
      if ( m_hs[i].configerror)
	continue;
      if ( cdh_NoCaseStrcmp( m_nodes[i].name, m_hs[j].nodename) == 0) {
	found = true;
	break;
      }	
    }
    if ( !found)
      continue;

    m_nodes[i].is_server = 1;

    send_connect( m_nodes[i].nid, &sts);
  }
  return 1;
}

bool rt_sevhistmon::send_connect( pwr_tNid nid, pwr_tStatus *sts)
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus   lsts;  

  tgt.nid = nid;
  tgt.qix = sev_eProcSevServer;
    
  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevClient;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_NodeUp;
  put.msg_id = m_msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc(&lsts, put.size);
  put.data = msg;

  msg->Type = sev_eMsgType_NodeUp;

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
  }    

  return ODD(*sts);
}

int rt_sevhistmon::send_itemlist( pwr_tNid nid)
{
  int item_cnt = 0;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int		size;
  pwr_tAName	aname;
  char		*s;

  // Identify node
  bool found = false;
  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    if ( nid == m_nodes[i].nid) {
      found = true;
      m_nodes[i].connected = 1;
      break;
    }
  }

  // Count items for this node
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;
    if ( nid == m_hs[i].nid)
      item_cnt += m_hs[i].sevhistlist.size();
  }

  if ( item_cnt == 0)
    return 1;

  size = sizeof(sev_sMsgHistItems) + (item_cnt - 1) * sizeof(sev_sHistItem);
  
  put.size = size;
  put.data = qcom_Alloc(&lsts, put.size);

  ((sev_sMsgHistItems *)put.data)->Type = sev_eMsgType_HistItems;
  ((sev_sMsgHistItems *)put.data)->Status = SEV__SUCCESS;

  int k = 0;
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;
    if ( nid == m_hs[i].nid) {
      for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++) {
	((sev_sMsgHistItems *)put.data)->Items[k].oid = m_hs[i].sevhistlist[j].aref.Objid;
	strcpy( aname, m_hs[i].sevhistlist[j].aname); 
	s = strchr( aname, '.');
	if ( !s)
	  continue;
	*s = 0;
	strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].oname, aname);
	strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].aname, s + 1);
	((sev_sMsgHistItems *)put.data)->Items[k].storagetime = 
	  net_DeltaTimeToNetTime( &m_hs[i].sevhistlist[j].storagetime);
	((sev_sMsgHistItems *)put.data)->Items[k].deadband = m_hs[i].sevhistlist[j].deadband;
	((sev_sMsgHistItems *)put.data)->Items[k].options = m_hs[i].sevhistlist[j].options;
	((sev_sMsgHistItems *)put.data)->Items[k].attr[0].type = m_hs[i].sevhistlist[j].type;
	((sev_sMsgHistItems *)put.data)->Items[k].attr[0].size = m_hs[i].sevhistlist[j].size;
	((sev_sMsgHistItems *)put.data)->Items[k].sevid = m_hs[i].sevhistlist[j].sevid;
	strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].description,
		 m_hs[i].sevhistlist[j].description, 
		 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].description));
	strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].unit,
		 m_hs[i].sevhistlist[j].unit, 
		 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].attr[0].unit));
	((sev_sMsgHistItems *)put.data)->Items[k].scantime = m_hs[i].sevhistlist[j].scantime;
	k++;
      }
    }
  }

  tgt.nid = nid;
  tgt.qix = sev_eProcSevServer;

    
  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevClient;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItems;
  put.msg_id = m_msg_id++;

  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
    return 0;
  }    

  // If all nodes are connected, set running status
  if ( !m_allconnected) {
    bool all_conn = true;
    for ( unsigned int i = 0; i < m_nodes.size(); i++) {
      if ( m_nodes[i].is_server && !m_nodes[i].connected) {
	all_conn = false;
	break;
      }
    }
    if ( all_conn) {
      m_allconnected = 1;
      m_server_status = PWR__SRUN;
      set_status();
    }
  }
  return 1;
}

int rt_sevhistmon::mainloop()
{
  qcom_sQid qid;
  int tmo = int(1000 * m_scantime);
  qcom_sGet get;
  void *mp;
  pwr_tStatus sts;

  qid.nid = 0;
  qid.qix = sev_eProcSevClient;

  for (;;) {
    memset( &get, 0, sizeof(get));
    mp = qcom_Get(&sts, &qid, &get, tmo);
    if ( sts == QCOM__TMO || !mp) {
      m_loopcnt++;
      send_data();
      continue;
    }

    switch (get.type.b) {
    case sev_cMsgClass:
      switch ( get.type.s) {
      case sev_eMsgType_NodeUp:
      case sev_eMsgType_HistItemsRequest:
	send_itemlist( get.sender.nid);
	break;
      default: ;
      }
      break;
    case qcom_eBtype_event: {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.oldPlcStop && !m_swap) {
	m_swap = 1;
	// errh_SetStatus( PWR__SRVRESTART);
	m_confp->Status = PWR__SRVRESTART;
	close();
      } 
      else if (new_event.b.swapDone && m_swap) {
	m_swap = 0;
	try {
	  init_objects();
	  connect();
	}
	catch ( co_error e) {
	  errh_Error( "SevHistMonitor terminating, %m", e.sts());
	  exit(0);
	}
	// errh_SetStatus( PWR__SRUN);
	m_confp->Status = PWR__SRUN;
	errh_Info("Warm restart completed");
      }
      else if (new_event.b.terminate) {
	m_confp->Status = PWR__SRVTERM;
	exit(0);
      }
      break;
    }
    default: ;
    }

    qcom_Free( &sts, mp);

    if ( !m_allconnected)
      retry_connect();
  }

}

int main()
{
  rt_sevhistmon client;

  try {
    client.init();
    client.connect();
  }
  catch ( co_error e) {
    errh_Error( "SevHistMonitor terminating, %m", e.sts());
    exit(0);
  }

  client.mainloop();

  client.close();
}
