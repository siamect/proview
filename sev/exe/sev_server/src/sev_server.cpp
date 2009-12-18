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
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_error.h"
#include "co_cnf.h"
#include "co_tree.h"
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
#define sev_cGarbageCycle 86400

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

  m_server_status = PWR__SRVSTARTUP;

  qcom_Init( &m_sts, &aid, "sev_server");
  if ( EVEN(m_sts)) throw co_error(m_sts);

  errh_Init( "sev_server", (errh_eAnix)0);

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
      errh_Fatal("Failed to create to database '%s'", sev_dbms_env::dbName());
      exit(0);
    }
  }
  else {    
    if ( !env->openDb()) {
      errh_Fatal("Failed to connect to database '%s'", sev_dbms_env::dbName());
      exit(0);
    }
  }

  if( !env->checkAndUpdateVersion(constSevVersion) ) {
    errh_Fatal("Failed to upgrade tables to sev version %d db:'%s'", constSevVersion, sev_dbms_env::dbName());
    exit(0);
  }
    
  m_db = new sev_dbms( env);

  errh_Info("Database opened '%s'", sev_dbms_env::dbName());

  m_db->get_items( &m_sts);
  m_db->get_objectitems(&m_sts);

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

  // Wait for qmon to start
  sleep(5);

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
      errh_Info( "No connection to %s (%s)", m_nodes[i].name, cdh_NodeIdToString( 0, m_nodes[i].nid, 0, 0));
    }    
    else
      errh_Info( "Connect sent to %s (%s)", m_nodes[i].name, cdh_NodeIdToString( 0, m_nodes[i].nid, 0, 0));
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
  int itemattr_cnt = 0;

  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  int		size;

  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted)
      continue;
    item_cnt++;
    itemattr_cnt += m_db->m_items[i].attrnum;
  }

  if ( !item_cnt)
    return 1;

  size = sizeof(sev_sMsgHistItems) + (item_cnt - 1) * sizeof(sev_sHistItem);
  
  size += itemattr_cnt * sizeof(sev_sHistAttr) - item_cnt * sizeof(sev_sHistAttr);

  put.size = size;
  put.data = qcom_Alloc(&lsts, put.size);

  ((sev_sMsgHistItems *)put.data)->Type = sev_eMsgType_HistItems;

  ((sev_sMsgHistItems *)put.data)->NumItems = item_cnt;
  ((sev_sMsgHistItems *)put.data)->NumAttributes = itemattr_cnt;

  sev_sHistItem *itemPtr = ((sev_sMsgHistItems *)put.data)->Items;
  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted)
      continue;
    itemPtr->oid = m_db->m_items[i].oid;
    strcpy( itemPtr->oname, m_db->m_items[i].oname);
    itemPtr->storagetime = net_DeltaTimeToNetTime(&m_db->m_items[i].storagetime);
    itemPtr->creatime = net_TimeToNetTime( &m_db->m_items[i].creatime);
    itemPtr->modtime = net_TimeToNetTime( &m_db->m_items[i].modtime);
    strcpy( itemPtr->description, m_db->m_items[i].description);
    itemPtr->attrnum = m_db->m_items[i].attrnum;
    itemPtr->scantime = m_db->m_items[i].scantime;
    itemPtr->deadband = m_db->m_items[i].deadband;
    itemPtr->options = m_db->m_items[i].options;
    size_t j = 0;
    for( j = 0; j < m_db->m_items[i].attr.size(); j++ ) {
      strcpy( itemPtr->attr[j].aname, m_db->m_items[i].attr[j].aname);
      itemPtr->attr[j].type = m_db->m_items[i].attr[j].type;
      itemPtr->attr[j].size = m_db->m_items[i].attr[j].size;
      strcpy( itemPtr->attr[j].unit, m_db->m_items[i].attr[j].unit);
    }
    itemPtr = (sev_sHistItem *)&itemPtr->attr[j];   
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
/*
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

  ((sev_sMsgHistItems *)put.data)->NumItems = item_cnt;
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
*/

int sev_server::send_server_status( qcom_sQid tgt)
{
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;

  put.size = sizeof(sev_sMsgServerStatus);
  put.data = qcom_Alloc(&lsts, put.size);

  ((sev_sMsgServerStatus *)put.data)->Type = sev_eMsgType_ServerStatus;


  sts = m_server_status;

  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    if ( m_db->m_items[i].deleted || m_db->m_items[i].status == 0)
      continue;
    if ( errh_Severity( m_db->m_items[i].status) > errh_Severity( sts))
      sts = m_db->m_items[i].status;
  }

  ((sev_sMsgServerStatus *)put.data)->Status = sts;

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_ServerStatus;
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

  m_server_status = PWR__SRUN;

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
            errh_Info("Node up %s", cdh_NodeIdToString( 0, get.reply.nid, 0, 0));
            request_items( get.reply.nid);
            break;
          case sev_eMsgType_HistItems:
            errh_Info("Itemlist received %s", cdh_NodeIdToString( 0, get.reply.nid, 0, 0));
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
          case sev_eMsgType_ServerStatusRequest:
            send_server_status( get.reply);
            break;
          case sev_eMsgType_HistObjectDataGetRequest:
            send_objecthistdata( get.reply, (sev_sMsgHistDataGetRequest *) mp, get.size);
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

    if(msg->Items[i].attrnum > 1) {
      //printf( "Received: %s.%s AttrNum:%d\n", msg->Items[i].oname, msg->Items[i].attr[0].aname, msg->Items[i].attrnum);
      sev_sHistItem *buffP = &msg->Items[i];
      while(buffP < &msg->Items[item_cnt-1]) {
        //for(size_t j = 0; j < buffP->attrnum; j++) {
        //  printf( "Received: %s.%s\n", buffP->oname, buffP->attr[j].aname);
        //}
        char *s;
        char tablename[400];
        char tmpStr[400];
        char attributeName[400];
        strcpy( tmpStr, buffP->oname);
        //Point out attribute name
        s = strchr( tmpStr, '.');
        if (s)
        {
          *s = 0;
          strcpy( attributeName, s+1);
        }
        else
          attributeName[0] = '\0';

        storagetime = net_NetTimeToDeltaTime( &buffP->storagetime );
        sprintf( tablename, "obj_%s",  m_db->oid_to_table( buffP->oid, attributeName) );
        if ( !m_db->check_objectitem( &m_sts, 
                                      tablename, 
                                      buffP->oid, 
                                      buffP->oname, 
                                      attributeName,
                                      storagetime, 
                                      buffP->description, 
                                      buffP->scantime, 
                                      buffP->deadband, 
                                      buffP->options, 
                                      &idx)) {
          m_db->add_objectitem( &m_sts, 
                                tablename, 
                                buffP->oid, 
                                buffP->oname, 
                                attributeName,
                                storagetime, 
                                buffP->description,
                                buffP->scantime, 
                                buffP->deadband, 
                                buffP->options, 
                                &idx);
          if ( EVEN(m_sts)) return m_sts;
        }

        vector<sev_attr> oldattrVec = m_db->m_items[idx].attr;
        vector<sev_attr> newattrVec;
        m_db->m_items[idx].value_size = 0;
        //Check if any new attributes is found if so add column
        for(size_t j = 0; j < buffP->attrnum; j++) {
          //printf( "Received: %s.%s\n", buffP->oname, buffP->attr[j].aname);
          sev_attr newattr;
          strncpy( newattr.aname, buffP->attr[j].aname, sizeof(newattr.aname));
          newattr.type = buffP->attr[j].type;
          newattr.size = buffP->attr[j].size;
          m_db->m_items[idx].value_size += newattr.size;
          newattr.elem = 0;
          newattrVec.push_back(newattr);

          if ( !m_db->check_objectitemattr( &m_sts, 
                                            tablename, 
                                            buffP->oid, 
                                            buffP->attr[j].aname, 
                                            buffP->oname,
                                            buffP->attr[j].type, 
                                            buffP->attr[j].size, 
                                            &idx) ) {
            m_db->add_objectitemattr( &m_sts, 
                                      tablename, 
                                      buffP->oid, 
                                      buffP->attr[j].aname, 
                                      buffP->oname, 
                                      buffP->attr[j].type, 
                                      buffP->attr[j].size, 
                                      &idx);
            if ( EVEN(m_sts)) return m_sts;
          }
        }

        //Be sure that we have the correct attribute order. Use the list from the client
        m_db->m_items[idx].attr.clear();
        m_db->m_items[idx].attr = newattrVec;
        m_db->m_items[idx].attrnum = newattrVec.size();

        //If node is coming up again we do not want deadband to be active due to init of old_value
        m_db->m_items[idx].deadband_active = 0;
        m_db->m_items[idx].first_storage = 1;

        //TODO Check if some items in oldAttrVec no longer exists if so mark this item
        //so that databaseadministrator knows which columns he can delete

        //If something was wrong during checking of attributes we ignore this object
        if ( ODD(m_sts) )
        {
          //Create space for the old values used if we have deadband active
          if( m_db->m_items[idx].old_value != 0 ) {
            free(m_db->m_items[idx].old_value);
            m_db->m_items[idx].old_value = 0;
          }
          m_db->m_items[idx].old_value = malloc(m_db->m_items[idx].value_size);

          m_db->m_items[idx].sevid = buffP->sevid;
          sev_refid sevid(buffP->sevid);
          m_refid[sevid] = idx;
        }

        int numberOfAttributes = buffP->attrnum;
        //buffP points after the last attribute written
        if(numberOfAttributes == 0) {
          printf("Something is very strange at line:%d in file:%s\n", __LINE__, __FUNCTION__);
          break;
        }
        buffP = (sev_sHistItem *)&buffP->attr[numberOfAttributes];
      }
      break;
    }
    if ( !m_db->check_item( &m_sts, msg->Items[i].oid, msg->Items[i].oname, msg->Items[i].attr[0].aname,
			    storagetime, msg->Items[i].attr[0].type, msg->Items[i].attr[0].size, 
			    msg->Items[i].description, msg->Items[i].attr[0].unit, msg->Items[i].scantime, 
			    msg->Items[i].deadband, msg->Items[i].options, &idx)) {
      m_db->add_item( &m_sts, msg->Items[i].oid, msg->Items[i].oname, msg->Items[i].attr[0].aname,
		      storagetime, msg->Items[i].attr[0].type, msg->Items[i].attr[0].size, 
		      msg->Items[i].description, msg->Items[i].attr[0].unit, msg->Items[i].scantime, 
		      msg->Items[i].deadband, msg->Items[i].options, &idx);
      if ( EVEN(m_sts)) return m_sts;
    }
    //Create space for the old values used if we have deadband active
    if ( m_db->m_items[idx].old_value != 0 ) {
      free(m_db->m_items[idx].old_value);
      m_db->m_items[idx].old_value = 0;
    }
    m_db->m_items[idx].value_size = msg->Items[i].attr[0].size;
    m_db->m_items[idx].old_value = malloc(m_db->m_items[idx].value_size);

    //If node is coming up again we do not want deadband to be active due to init of old_value
    m_db->m_items[idx].deadband_active = 0;
    m_db->m_items[idx].first_storage = 1;

    m_db->m_items[idx].sevid = msg->Items[i].sevid;
    sev_refid sevid(msg->Items[i].sevid);
    m_refid[sevid] = idx;
  }
  printf( "----  Node up (%d) ----\n", nid);
  for ( iterator_refid it = m_refid.begin(); it != m_refid.end(); it++)
    printf( "Refid: %d,%d  Idx: %d Name %s\n", it->first.id.nid, it->first.id.rix, it->second, m_db->m_items[it->second].oname);
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

  starttime = net_NetTimeToTime( &rmsg->StartTime);
  endtime = net_NetTimeToTime( &rmsg->EndTime);
  sev_item item;
  char tablename[500];
  strcpy( tablename, m_db->oid_to_table( rmsg->Oid, rmsg->AName) );

  m_db->get_item(&m_sts, &item, tablename);
  if(ODD(m_sts)) {
    m_db->get_values( &m_sts, rmsg->Oid, item.options, item.deadband, 
            rmsg->AName, item.attr[0].type, item.attr[0].size, 
            item.scantime, &item.creatime,
            &starttime, &endtime, rmsg->NumPoints, &tbuf,  &vbuf, &rows);
  }
  if ( ODD(m_sts))
    msize = rows * ( sizeof(pwr_tTime) + item.attr[0].size) + sizeof(*msg) - sizeof(msg->Data);
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
    msg->VType = item.attr[0].type;
    msg->VSize = item.attr[0].size;
  }
  msg->Status = m_sts;

  if ( ODD(m_sts) && rows) {
    memcpy( &msg->Data, tbuf, sizeof(pwr_tTime) * rows);
    memcpy( (char *)&msg->Data + sizeof(pwr_tTime) * rows, vbuf, item.attr[0].size * rows);
  }
  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
  }    

  return 1;
}

int sev_server::send_objecthistdata( qcom_sQid tgt, sev_sMsgHistDataGetRequest *rmsg, unsigned int size)
{
  pwr_tTime *tbuf;
  void *vbuf;
  unsigned int rows;
  sev_sMsgHistObjectDataGet *msg;
  int msize;
  qcom_sPut	put;
  pwr_tStatus	sts, lsts;
  pwr_tTime	starttime, endtime;

  starttime = net_NetTimeToTime( &rmsg->StartTime);
  endtime = net_NetTimeToTime( &rmsg->EndTime);
  sev_item item;
  char tablename[500];
  sprintf( tablename, "obj_%s",  m_db->oid_to_table( rmsg->Oid, rmsg->AName) );

  m_db->get_objectitem(&m_sts, &item, tablename);
  if(ODD(m_sts)) {
    m_db->get_objectvalues(&m_sts, &item, item.value_size, &starttime, &endtime, rmsg->NumPoints, &tbuf,  &vbuf, &rows);
  }
  if ( ODD(m_sts)) {
    msize = rows * ( sizeof(pwr_tTime) + item.value_size);
    msize += item.attr.size() * sizeof(msg->Attr);
    msize += sizeof(*msg) - sizeof(msg->Data) - sizeof(msg->Attr);
  } 
  else
    msize = sizeof(*msg);

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevServer;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistObjectDataGet;
  put.msg_id = m_msg_id++;
  put.size = msize;
  msg  = (sev_sMsgHistObjectDataGet *) qcom_Alloc(&lsts, put.size);

  put.data = msg;

  msg->Type = sev_eMsgType_HistObjectDataGet;
  msg->Oid = rmsg->Oid;
  strncpy( msg->AName, rmsg->AName, sizeof(msg->AName));
  msg->Status = m_sts;
  msg->NumPoints = 0;
  msg->NumAttributes = 0;
  if ( ODD(m_sts)) {
    msg->NumPoints = rows;
    msg->NumAttributes = item.attr.size();
    msg->TotalDataSize = rows * ( sizeof(pwr_tTime) + item.value_size);
    for( size_t i = 0; i < item.attr.size(); i++ ) {
      strncpy( msg->Attr[i].aname, item.attr[i].aname, sizeof(msg->Attr[0].aname));
      msg->Attr[i].elem = 1; //Not used
      msg->Attr[i].size = item.attr[i].size;
      msg->Attr[i].type = item.attr[i].type;
    }
    if( rows ) {
      void *ptr = &msg->Attr[item.attr.size()];
      memcpy( ptr, tbuf, sizeof(pwr_tTime) * rows);
      memcpy( (char *)ptr + sizeof(pwr_tTime) * rows, vbuf, item.value_size * rows);
    }
  }

  if ( !qcom_Put( &sts, &tgt, &put)) {
    qcom_Free( &sts, put.data);
  }    

  return 1;
}

void sev_server::garbage_collector()
{
  int item_size = m_db->m_items.size();
  static int current = 0;
  float items_per_scan;
  int scan_per_items;
  static int scan_cnt = 0;
  int i;

  if ( item_size == 0)
    return;

  items_per_scan = ((float)sev_cGarbageInterval) * item_size / sev_cGarbageCycle;

  if ( items_per_scan >= 1) {
    for ( i = 0; i < (int)items_per_scan; i++) {
      garbage_item( current);
      current++;
      if ( current >= item_size)
        current = 0;
    }    
  }
  else {
    scan_per_items = (int)( 1.0 / items_per_scan);
    scan_cnt++;
    if ( scan_cnt >= scan_per_items) {
      scan_cnt = 0;

      garbage_item( current);

      current++;
      if ( current >= item_size)
        current = 0;
    }
  }
}

void sev_server::garbage_item( int idx)
{
  pwr_tTime currenttime, limit;

  //clock_gettime( CLOCK_REALTIME, &currenttime);
  time_GetTime( &currenttime);

  if ( m_db->m_items[idx].deleted)
    return;
  if ( m_db->m_items[idx].storagetime.tv_sec == 0)
    return;

  time_Asub( &limit, &currenttime, &m_db->m_items[idx].storagetime);

  if( m_db->m_items[idx].attrnum > 1 ) {
    m_db->delete_old_objectdata( &m_sts, m_db->m_items[idx].tablename, 
                                 m_db->m_items[idx].options, limit);
  }
  else {
    m_db->delete_old_data( &m_sts, m_db->m_items[idx].oid, m_db->m_items[idx].attr[0].aname, 
         m_db->m_items[idx].options, limit);
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
