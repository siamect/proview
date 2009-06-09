/* 
 * Proview   $Id: sev_server.cpp,v 1.7 2008-11-26 15:27:28 claes Exp $
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

#if defined PWRE_CONF_MYSQL


#include "pwr.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_error.h"
#include "co_cnf.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"
#include "rt_gdh.h"
#include "sev_server.h"
#include "rt_sev_net.h"
#include "sev_dbms.h"
#include "rt_sev_msg.h"
#include "rt_pwr_msg.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"

#define sev_cGarbageInterval 120

int sev_server::init( int noneth)
{
  qcom_sNode		node;
  pwr_tNid		nid;
  pwr_tStatus		sts;
  qcom_sAid		aid;
  qcom_sQid 		qini;
  sev_dbms_env 		*env;
  pwr_tFileName 	envname;
  char 			socket[200];

  qcom_Init( &m_sts, &aid, "sev_server");
  if ( EVEN(m_sts)) throw co_error(m_sts);

  m_noneth = noneth;
  if (!m_noneth) {
    // Check server config object
    pwr_tOid conf_oid;

    sts = gdh_Init( "sev_server");
    if ( EVEN(m_sts)) throw co_error(m_sts);

    sts = gdh_GetClassList( pwr_cClass_SevServer, &conf_oid);
    if ( EVEN(sts)) {
      errh_CErrLog( PWR__SRVNOTCONF, 0);
      exit(0);
    }
  }

  sprintf( envname, "$pwrp_db/%s.db", sev_dbms_env::dbName());
  dcli_translate_filename( envname, envname);
  
  env = new sev_dbms_env( envname);
  env->open( envname);
  if ( !env->exists()) {
    cnf_get_value( "mysqlSocket", socket);
    env->create( envname, "localhost", "pwrp", "", sev_dbms_env::dbName(), 50, 
		 socket);

    env->open( envname);

    if ( !env->createDb()) {
      errh_Fatal("Failed to create to database");
      exit(0);
    }
  }
  else {    
    if ( !env->openDb()) {
      errh_Fatal("Failed to connect to database");
      exit(0);
    }
  }
  
  m_db = new sev_dbms( env);

  m_db->get_items( &m_sts);

  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    sev_item_key items_key( m_db->m_items[i].oid, m_db->m_items[i].attr[0].aname);
    m_item_key[items_key] = i;
  }

  // Create a queue to server
  qcom_sQattr attr;
  qcom_sQid qid;

  qid.qix = sev_eProcSevServer;
  qid.nid = 0;
  attr.type = qcom_eQtype_private;
  attr.quota = 200;

  if (!qcom_CreateQ(&sts, &qid, &attr, "SevServer")) {
    if ( sts == QCOM__QALLREXIST) {
      if ( !qcom_AttachQ(&sts, &qid)) {
	if ( !qcom_DeleteQ(&sts, &qid))
	  throw co_error(sts);	     
	if ( !qcom_CreateQ(&sts, &qid, &attr, "SevServer"))
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

  return 1;
}

int sev_server::connect()
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    tgt.nid = m_nodes[i].nid;
    tgt.qix = sev_eProcSevClient;
    

    put.reply.nid = m_nodes[0].nid;
    put.reply.qix = sev_eProcSevServer;
    put.type.b = (qcom_eBtype) sev_cMsgClass;
    put.type.s = (qcom_eStype) sev_eMsgType_NodeUp;
    put.msg_id = m_msg_id++;
    put.size = sizeof(sev_sMsgAny);
    msg  = (sev_sMsgAny *) qcom_Alloc(&lsts, put.size);

    msg->Type = sev_eMsgType_NodeUp;
    put.data = msg;

    if ( !qcom_Put( &sts, &tgt, &put)) {
      qcom_Free( &sts, put.data);
    }    
  }

  return 1;
}

int sev_server::request_items( pwr_tNid nid)
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  

  tgt.nid = nid;
  tgt.qix = sev_eProcSevClient;
    
  
  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItemsRequest;
  put.msg_id = m_msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc(&lsts, put.size);

  msg->Type = sev_eMsgType_HistItemsRequest;
  put.data = msg;
  
  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
  }    

  return 1;
}

int sev_server::send_itemlist( qcom_sQid tgt)
{
  int item_cnt = 0;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int		size;

  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted)
      continue;
    item_cnt++;
  }

  if ( !item_cnt)
    return 1;

  size = sizeof(sev_sMsgHistItems) + (item_cnt - 1) * sizeof(sev_sHistItem);
  
  put.size = size;
  put.data = qcom_Alloc(&lsts, put.size);

  ((sev_sMsgHistItems *)put.data)->Type = sev_eMsgType_HistItems;

  int idx = 0;
  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted)
      continue;
    ((sev_sMsgHistItems *)put.data)->Items[idx].oid = m_db->m_items[i].oid;
    strcpy( ((sev_sMsgHistItems *)put.data)->Items[idx].oname, m_db->m_items[i].oname);
    ((sev_sMsgHistItems *)put.data)->Items[idx].storagetime = 
      net_DeltaTimeToNetTime(&m_db->m_items[i].storagetime);
    ((sev_sMsgHistItems *)put.data)->Items[idx].creatime = 
      net_TimeToNetTime( &m_db->m_items[i].creatime);
    ((sev_sMsgHistItems *)put.data)->Items[idx].modtime = 
      net_TimeToNetTime( &m_db->m_items[i].modtime);
    strcpy( ((sev_sMsgHistItems *)put.data)->Items[idx].description, m_db->m_items[i].description);
    strcpy( ((sev_sMsgHistItems *)put.data)->Items[idx].attr[0].aname, m_db->m_items[i].attr[0].aname);
    ((sev_sMsgHistItems *)put.data)->Items[idx].attrnum = m_db->m_items[i].attrnum;
    ((sev_sMsgHistItems *)put.data)->Items[idx].attr[0].type = m_db->m_items[i].attr[0].type;
    ((sev_sMsgHistItems *)put.data)->Items[idx].attr[0].size = m_db->m_items[i].attr[0].size;
    strcpy( ((sev_sMsgHistItems *)put.data)->Items[idx].attr[0].unit, m_db->m_items[i].attr[0].unit);
    ((sev_sMsgHistItems *)put.data)->Items[idx].scantime = m_db->m_items[i].scantime;
    ((sev_sMsgHistItems *)put.data)->Items[idx].deadband = m_db->m_items[i].deadband;
    ((sev_sMsgHistItems *)put.data)->Items[idx].options = m_db->m_items[i].options;
    idx++;
  }
  
  if ( !item_cnt)
    ((sev_sMsgHistItems *)put.data)->Status = SEV__NOITEMS;
  else
    ((sev_sMsgHistItems *)put.data)->Status = SEV__SUCCESS;

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItems;
  put.msg_id = m_msg_id++;

  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
    return 0;
  }    
  return 1;
}

int sev_server::delete_item( qcom_sQid tgt, sev_sMsgHistItemDelete *rmsg)
{
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;

  put.size = sizeof(sev_sMsgHistItemStatus);
  put.data = qcom_Alloc(&lsts, put.size);

  m_db->delete_item( &sts, rmsg->Oid, rmsg->AName);

  ((sev_sMsgHistItemStatus *)put.data)->Type = sev_eMsgType_HistItemStatus;

  ((sev_sMsgHistItemStatus *)put.data)->Oid = rmsg->Oid;
  strcpy( ((sev_sMsgHistItemStatus *)put.data)->AName, rmsg->AName);
  ((sev_sMsgHistItemStatus *)put.data)->Status = sts;

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItemStatus;
  put.msg_id = m_msg_id++;

  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
    return 0;
  }    
  return 1;
}

int sev_server::mainloop()
{
  qcom_sQid qid;
  int tmo = 1000;
  qcom_sGet get;
  void *mp;
  pwr_tStatus sts;
  pwr_tTime next_garco, currenttime;
  pwr_tDeltaTime garco_interval;

  qid.nid = 0;
  qid.qix = sev_eProcSevServer;

  time_FloatToD( &garco_interval, sev_cGarbageInterval);
  time_GetTime( &currenttime);
  time_Aadd( &next_garco, &currenttime, &garco_interval);

  for (;;) {
    memset( &get, 0, sizeof(get));
    mp = qcom_Get(&sts, &qid, &get, tmo);

    time_GetTime( &currenttime);
    if ( time_Acomp( &currenttime, &next_garco) == 1) {
      garbage_collector();
      time_Aadd( &next_garco, &next_garco, &garco_interval);
    }
    if ( sts == QCOM__TMO || !mp)
      continue;

    switch (get.type.b) {
    case sev_cMsgClass:
      switch ( get.type.s) {
      case sev_eMsgType_NodeUp:
	request_items( get.reply.nid);
	break;
      case sev_eMsgType_HistItems:
	check_histitems( (sev_sMsgHistItems *) mp, get.size);
	break;
      case sev_eMsgType_HistDataStore:
	receive_histdata( (sev_sMsgHistDataStore *) mp, get.size);
	break;
      case sev_eMsgType_HistDataGetRequest:
	send_histdata( get.reply, (sev_sMsgHistDataGetRequest *) mp, get.size);
	break;
      case sev_eMsgType_HistItemsRequest:
	send_itemlist( get.reply);
	break;
      case sev_eMsgType_HistItemDelete:
	delete_item( get.reply, (sev_sMsgHistItemDelete *) mp);
	break;
      default: ;
      }
      break;
    case qcom_eBtype_event: {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.terminate)
	exit(0);
      break;
    }
    default: ;
    }

    qcom_Free( &sts, mp);
  }
}

int sev_server::check_histitems( sev_sMsgHistItems *msg, unsigned int size)
{
  unsigned int idx;
  pwr_tDeltaTime storagetime;
  int item_cnt = (size - sizeof(sev_sMsgHistItems)) / sizeof(sev_sHistItem) + 1;

  if ( item_cnt <= 0)
    return 0;

  // Remove all refid's for this node
  pwr_tNid nid = msg->Items[0].sevid.nid;

  for ( iterator_refid it = m_refid.begin(); it != m_refid.end(); it++) {
    if ( it->first.id.nid == nid)
      m_refid.erase( it);
  }
  
  for ( int i = 0; i < item_cnt; i++) {

    // Deadband requires id variable
    if ( msg->Items[i].options & pwr_mSevOptionsMask_UseDeadBand)
      msg->Items[i].options |= pwr_mSevOptionsMask_ReadOptimized;

    // printf( "Received: %s.%s\n", msg->Items[i].oname, msg->Items[i].attr[0].aname);
    storagetime = net_NetTimeToDeltaTime( &msg->Items[i].storagetime);
    if ( !m_db->check_item( &m_sts, msg->Items[i].oid, msg->Items[i].oname, msg->Items[i].attr[0].aname,
			    storagetime, msg->Items[i].attr[0].type, msg->Items[i].attr[0].size, 
			    msg->Items[i].description, msg->Items[i].attr[0].unit, msg->Items[i].scantime, 
			    msg->Items[i].deadband, msg->Items[i].options, &idx)) {
      m_db->add_item( &m_sts, msg->Items[i].oid, msg->Items[i].oname, msg->Items[i].attr[0].aname,
		      storagetime, msg->Items[i].attr[0].type, msg->Items[i].attr[0].size, 
		      msg->Items[i].description, msg->Items[i].attr[0].unit, msg->Items[i].scantime, 
		      msg->Items[i].deadband, msg->Items[i].options, &idx);
      if ( EVEN(m_sts)) return m_sts;
      
      sev_item_key item_key( msg->Items[i].oid, msg->Items[i].attr[0].aname);
      m_item_key[item_key] = idx;
    }

    m_db->m_items[idx].sevid = msg->Items[i].sevid;
    sev_refid sevid(msg->Items[i].sevid);
    m_refid[sevid] = idx;
  }

  printf( "----  Node up ----\n");
  // for ( iterator_refid it = m_refid.begin(); it != m_refid.end(); it++)
    //    printf( "Refid: %d,%d  Name %s\n", it->first.id.nid, it->first.id.rix, m_db->m_items[it->second].oname);

  return 1;
}

int sev_server::receive_histdata( sev_sMsgHistDataStore *msg, unsigned int size)
{
  sev_sHistData *dp = (sev_sHistData *)&msg->Data;
  pwr_tTime time;

  while ( (char *)dp - (char *)msg < (int)size) {
    sev_refid sevid(dp->sevid);
    iterator_refid it = m_refid.find( sevid);
    if ( it == m_refid.end()) {
      dp = (sev_sHistData *)((char *)dp + sizeof( *dp) - sizeof(dp->data) +  dp->size);
      continue;
    }
    unsigned int idx = it->second;

    time = net_NetTimeToTime( &msg->Time);
    m_db->store_value( &m_sts, idx, 0, time, &dp->data, dp->size);

    dp = (sev_sHistData *)((char *)dp + sizeof( *dp) - sizeof(dp->data) +  dp->size);
  }

  return 1;
}

int sev_server::send_histdata( qcom_sQid tgt, sev_sMsgHistDataGetRequest *rmsg, unsigned int size)
{
  pwr_tTime *tbuf;
  void *vbuf;
  unsigned int rows;
  sev_sMsgHistDataGet 	*msg;
  int msize;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  pwr_tTime	starttime, endtime;
  
  sev_item_key item_key( rmsg->Oid, rmsg->AName);
  iterator_item_key it = m_item_key.find( item_key);
  if ( it == m_item_key.end())
    m_sts = SEV__NOSUCHITEM;
  else
    m_sts = SEV__SUCCESS;
  
  int idx = 0;
  if ( ODD(m_sts)) {
    idx = it->second;

    starttime = net_NetTimeToTime( &rmsg->StartTime);
    endtime = net_NetTimeToTime( &rmsg->EndTime);
    m_db->get_values( &m_sts, rmsg->Oid, m_db->m_items[idx].options, m_db->m_items[idx].deadband, 
		      rmsg->AName, m_db->m_items[idx].attr[0].type, m_db->m_items[idx].attr[0].size, 
		      m_db->m_items[idx].scantime,
		      &starttime, &endtime, rmsg->NumPoints, &tbuf,  &vbuf, &rows);
  }
  if ( ODD(m_sts))
    msize = rows * ( sizeof(pwr_tTime) + m_db->m_items[idx].attr[0].size) + sizeof(*msg) - sizeof(msg->Data);
  else
    msize = sizeof(*msg);

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistDataGet;
  put.msg_id = m_msg_id++;
  put.size = msize;
  msg  = (sev_sMsgHistDataGet *) qcom_Alloc(&lsts, put.size);

  put.data = msg;

  msg->Type = sev_eMsgType_HistDataGet;
  msg->Oid = rmsg->Oid;
  strncpy( msg->AName, rmsg->AName, sizeof(msg->AName));
  if ( ODD(m_sts)) {
    msg->NumPoints = rows;
    msg->VType = m_db->m_items[idx].attr[0].type;
    msg->VSize = m_db->m_items[idx].attr[0].size;
  }
  msg->Status = m_sts;

  if ( ODD(m_sts) && rows) {
    memcpy( &msg->Data, tbuf, sizeof(pwr_tTime) * rows);
    memcpy( (char *)&msg->Data + sizeof(pwr_tTime) * rows, vbuf, m_db->m_items[idx].attr[0].size * rows);
  }
  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
  }    

  return 1;
}

void sev_server::garbage_collector()
{
  pwr_tTime currenttime, limit;

  time_GetTime( &currenttime);
  
  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted)
      continue;
    if ( m_db->m_items[i].storagetime.tv_sec == 0)
      continue;

    time_Asub( &limit, &currenttime, &m_db->m_items[i].storagetime);

    m_db->delete_old_data( &m_sts, m_db->m_items[i].oid, m_db->m_items[i].attr[0].aname, 
			   m_db->m_items[i].options, limit);
  }
}

int main (int argc, char *argv[])
{
  sev_server srv;

  int noneth = 0;

  if ( argc > 1 && strcmp( argv[1], "-n") == 0)
    noneth = 1;

  srv.init( noneth);
  srv.connect();
  srv.mainloop();
}
#else
int main(){}
#endif
