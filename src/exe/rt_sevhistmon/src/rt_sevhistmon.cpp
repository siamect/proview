/* 
 * Proview   $Id: rt_sevhistmon.cpp,v 1.1 2008-07-17 11:33:09 claes Exp $
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
#include "co_error.h"
#include "rt_gdh.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_errh.h"
#include "rt_dshistmon.h"
#include "rt_sev_net.h"
#include "rt_sev_msg.h"

#define sevclient_cQix 121

int rt_dshistmon::init()
{
  pwr_tStatus sts;
  pwr_tOid hs_oid;
  pwr_tAttrRef h_aref;
  pwr_tAttrRef uaref;
  pwr_tTid a_tid;
  unsigned int a_size, a_offset, a_dim;
  pwr_tAName hname;
  qcom_sQid qid;
  qcom_sNode		node;
  pwr_tNid		nid;
  
  sts = gdh_Init( "rt_dshistmon");

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
	  co_error(sts);	     
	if ( !qcom_CreateQ(&sts, &qid, &attr, "SevClient"))
	  co_error(sts);
      }      
    }
    else
      throw co_error( sts);
  }

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

  // Get all DsHist and DsHistThread objects
  for ( sts = gdh_GetClassList( pwr_cClass_DsHistThread, &hs_oid);
	ODD(sts);
	sts = gdh_GetNextObject( hs_oid, &hs_oid)) {
    sev_dshistthread hs;
    pwr_sClass_DsHistThread *hs_p;
  
    m_sts = gdh_ObjidToPointer( hs_oid, (void **)&hs_p);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs.oid = hs_oid;
    hs.scantime = hs_p->ScanTime;
    strncpy( hs.nodename, hs_p->ServerNode, sizeof(hs.nodename));
    hs.size = 0;

    bool found = false;
    for ( unsigned int i = 0; i < m_nodes.size(); i++) {
      if ( cdh_NoCaseStrcmp( hs.nodename, m_nodes[i].name) == 0) {
	hs.nid = m_nodes[i].nid;
	found = true;
	break;
      }
    }
    if ( !found) {
      pwr_tOName oname;

      m_sts = gdh_ObjidToName( hs_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
      if ( EVEN(m_sts)) throw co_error(m_sts);

      errh_Error( "Unknown nodename, %s", oname);
      continue;
    }
    
    m_hs.push_back( hs);
  }

  for ( sts = gdh_GetClassListAttrRef( pwr_cClass_DsHist, &h_aref);
	ODD(sts);
	sts = gdh_GetNextAttrRef( pwr_cClass_DsHist, &h_aref, &h_aref)) {
    sev_dshist h;
    pwr_sClass_DsHist *h_p;
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
      errh_Error( "Invalid DsHist Attribute %s", hname);
      continue;
    }

    h.sevid.nid = m_nodes[0].nid;
    h.sevid.rix = m_next_rix++;
    h.storagetime = h_p->StorageTime;
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
      errh_Error( "Invalid DsHist Attribute type %s", hname);
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
    m_hs[hs_idx].dshistlist.push_back(h);
  }

  connect();

  return 1;
}

int rt_dshistmon::close()
{
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    for ( unsigned int j = 0; j < m_hs[i].dshistlist.size(); j++)
      gdh_UnrefObjectInfo( m_hs[i].dshistlist[j].refid);
  }
  m_hs.clear();
  return 1;
}


int rt_dshistmon::send_data()
{
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int msize;
  sev_sMsgHistDataStore *msg;
  sev_sHistData *dp;
  int		stime;

  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    stime = int(m_hs[i].scantime + 0.5);
    if ( !stime || m_loopcnt % stime != 0)
      continue;
	   
    msize = m_hs[i].dshistlist.size() * (sizeof(*dp) - sizeof(dp->data)) + m_hs[i].size; 
    msize += sizeof(*msg) - sizeof(msg->Data);

    msg = (sev_sMsgHistDataStore *) qcom_Alloc(&lsts, msize);

    put.size = msize;
    put.data = msg;

    msg->Type = sev_eMsgType_HistDataStore;
    clock_gettime( CLOCK_REALTIME, &msg->Time);

    dp = (sev_sHistData *) &msg->Data;
    for ( unsigned int j = 0; j < m_hs[i].dshistlist.size(); j++) {
      dp->sevid = m_hs[i].dshistlist[j].sevid;
      dp->type = m_hs[i].dshistlist[j].type;
      dp->size = m_hs[i].dshistlist[j].size;
      memcpy( &dp->data, m_hs[i].dshistlist[j].datap, dp->size);
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
      qcom_Free( &sts, put.data);
      continue;
    }    
  }
  return 1;
}

int rt_dshistmon::connect()
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {

    // Check if this node should be connected
    bool found = false;
    for ( unsigned int j = 0; j < m_hs.size(); j++) {
      if ( cdh_NoCaseStrcmp( m_nodes[i].name, m_hs[j].nodename) == 0) {
	found = true;
	break;
      }	
    }
    if ( !found)
      continue;

    tgt.nid = m_nodes[i].nid;
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

    if ( !qcom_Put( &sts, &tgt, &put)) {
      qcom_Free( &sts, put.data);
    }    
  }

  return 1;
}

int rt_dshistmon::send_itemlist( pwr_tNid nid)
{
  int item_cnt = 0;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int		size;
  pwr_tAName	aname;
  char		*s;

  // Count items for this node
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( nid == m_hs[i].nid)
      item_cnt += m_hs[i].dshistlist.size();
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
    if ( nid == m_hs[i].nid) {
      for ( unsigned int j = 0; j < m_hs[i].dshistlist.size(); j++) {
	((sev_sMsgHistItems *)put.data)->Items[k].oid = m_hs[i].dshistlist[j].aref.Objid;
	strcpy( aname, m_hs[i].dshistlist[j].aname); 
	s = strchr( aname, '.');
	if ( !s)
	  continue;
	*s = 0;
	strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].oname, aname);
	strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].aname, s + 1);
	((sev_sMsgHistItems *)put.data)->Items[k].storagetime = m_hs[i].dshistlist[j].storagetime;
	((sev_sMsgHistItems *)put.data)->Items[k].type = m_hs[i].dshistlist[j].type;
	((sev_sMsgHistItems *)put.data)->Items[k].size = m_hs[i].dshistlist[j].size;
	((sev_sMsgHistItems *)put.data)->Items[k].sevid = m_hs[i].dshistlist[j].sevid;
	strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].description,
		 m_hs[i].dshistlist[j].description, 
		 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].description));
	strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].unit,
		 m_hs[i].dshistlist[j].unit, 
		 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].unit));
	((sev_sMsgHistItems *)put.data)->Items[k].scantime = m_hs[i].dshistlist[j].scantime;
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
  return 1;
}

int rt_dshistmon::mainloop()
{
  qcom_sQid qid;
  int tmo = 1000;
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
      printf( "Message received\n");
      switch ( get.type.s) {
      case sev_eMsgType_NodeUp:
      case sev_eMsgType_HistItemsRequest:
	send_itemlist( get.sender.nid);
	break;
      default: ;
      }
      break;
    default: ;
    }

    qcom_Free( &sts, mp);
  }
}

int main()
{
  rt_dshistmon client;

  client.init();

  client.mainloop();

  client.close();
}
