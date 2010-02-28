/* 
 * Proview   $Id: rt_sevcli.c,v 1.4 2008-09-18 15:06:13 claes Exp $
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

#include <string.h>
#include "pwr.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_sev_msg.h"
#include "rt_sevcli.h"
#include "rt_sev_net.h"


int sevcli_init( pwr_tStatus *sts, sevcli_tCtx *ctx)
{
  // Create a queue to server
  qcom_sQattr attr;

  *ctx = calloc( 1, sizeof(sevcli_sCtx));

  (*ctx)->qid.qix = 0;
  (*ctx)->qid.nid = 0;
  attr.type = qcom_eQtype_private;
  attr.quota = 100;

  if (!qcom_CreateQ(sts, &(*ctx)->qid, &attr, "SevXtt")) {
    return 0;
  }
  *sts = SEV__SUCCESS;
  return 1;
}

int sevcli_close( pwr_tStatus *sts, sevcli_tCtx ctx)
{
  // Delete queue
  if (!qcom_DeleteQ( sts, &ctx->qid)) {
    return 0;
  }
  free( ctx);
  *sts = SEV__SUCCESS;
  return 1;
}

void sevcli_set_servernid( sevcli_tCtx ctx, pwr_tNid nid)
{
  ctx->server = nid;
}

int sevcli_set_servernode( pwr_tStatus *sts, sevcli_tCtx ctx, char *nodename)
{
  qcom_sNode		node;
  pwr_tNid		nid;

  if ( !nodename || strcmp( nodename, "") == 0) {
    ctx->server = ctx->qid.nid;
    *sts = SEV__SUCCESS;
    return 1;
  }

  // Try this node
  qcom_MyNode( sts, &node);
  if ( EVEN(*sts)) return 0;

  if ( cdh_NoCaseStrcmp( node.name, nodename) == 0) {
    ctx->server = node.nid;
    *sts = SEV__SUCCESS;
    return 1;
  }


  // Try other qcom nodes
  for (nid = qcom_cNNid; qcom_NextNode(sts, &node, nid); nid = node.nid) {
    if ( cdh_NoCaseStrcmp( nodename, node.name) == 0) {
      ctx->server = node.nid;
      *sts = SEV__SUCCESS;
      return 1;
    }
  }
  *sts = SEV__NOSUCHNODE;
  return 0;
}

int sevcli_get_itemlist( pwr_tStatus *sts, sevcli_tCtx ctx, sevcli_sHistItem **list, 
			 unsigned int *cnt)
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	lsts;
  int tmo = 1000;
  qcom_sGet get;
  sevcli_sHistItem *lp;
  int i;
  
  if ( ctx->server)
    tgt.nid = ctx->server;
  else
    tgt.nid = ctx->qid.nid;
  tgt.qix = sev_eProcSevServer;
    

  put.reply = ctx->qid;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItemsRequest;
  put.msg_id = ctx->msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc( sts, put.size);

  put.data = msg;

  msg->Type = sev_eMsgType_HistItemsRequest;

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
    return 0;
  }

  sev_sMsgHistItems *rmsg;

  memset( &get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistItems *) qcom_Get( sts, &ctx->qid, &get, tmo);
    if ( *sts == QCOM__TMO || !rmsg) {
      return 0;
    }

    if ( get.type.b == sev_cMsgClass && 
	 get.type.s == (qcom_eStype) sev_eMsgType_HistItems)
      break;

    qcom_Free( sts, rmsg);
  }
  
  *sts = rmsg->Status;
  if ( EVEN(*sts))
    return 0;

  //int item_cnt = (get.size - sizeof(sev_sMsgHistItems)) / sizeof(sev_sHistItem) + 1;
  int item_cnt = rmsg->NumItems;
  int attr_cnt = rmsg->NumAttributes;
  
  unsigned int data_size = (item_cnt * sizeof(sevcli_sHistItem)) + ((attr_cnt - item_cnt) * sizeof(sevcli_sHistAttr));
  lp = (sevcli_sHistItem *) malloc( data_size );
  sevcli_sHistItem *lp2 = lp;

  sev_sHistItem *itemPtr = ((sev_sMsgHistItems *)rmsg)->Items;
  for ( i = 0; i < item_cnt; i++) {
    lp->oid = itemPtr->oid;
    strncpy( lp->oname, itemPtr->oname, sizeof(lp->oname));
    lp->storagetime = net_NetTimeToDeltaTime( &itemPtr->storagetime);
    lp->deadband = itemPtr->deadband;
    lp->options = itemPtr->options;
    lp->creatime = net_NetTimeToTime( &itemPtr->creatime);
    lp->modtime = net_NetTimeToTime( &itemPtr->modtime);
    strncpy( lp->description, itemPtr->description, sizeof(lp->description));
    lp->scantime = itemPtr->scantime;

    lp->attrnum = itemPtr->attrnum;
    size_t j=0;
    for( j=0; j < lp->attrnum; j++ ) {
      lp->attr[j].type = itemPtr->attr[j].type;
      lp->attr[j].size = itemPtr->attr[j].size;
      strncpy( lp->attr[j].aname, itemPtr->attr[j].aname, sizeof(lp->attr[0].aname));
      strncpy( lp->attr[j].unit, itemPtr->attr[j].unit, sizeof(lp->attr[0].unit));
    }
    itemPtr = (sev_sHistItem *)&itemPtr->attr[j];
    lp = (sevcli_sHistItem *)&lp->attr[j];
  }

  qcom_Free( sts, rmsg);

  *cnt = item_cnt;
  *list = lp2;
  *sts = SEV__SUCCESS;
  return 1;
}
/*
int sevcli_get_itemlist( pwr_tStatus *sts, sevcli_tCtx ctx, sevcli_sHistItem **list, 
			 unsigned int *cnt)
{
  sev_sMsgAny 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  pwr_tStatus	lsts;
  int tmo = 1000;
  qcom_sGet get;
  sevcli_sHistItem *lp;
  int i;
  
  if ( ctx->server)
    tgt.nid = ctx->server;
  else
    tgt.nid = ctx->qid.nid;
  tgt.qix = sev_eProcSevServer;
    

  put.reply = ctx->qid;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItemsRequest;
  put.msg_id = ctx->msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc( sts, put.size);

  put.data = msg;

  msg->Type = sev_eMsgType_HistItemsRequest;

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
    return 0;
  }

  sev_sMsgHistItems *rmsg;

  memset( &get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistItems *) qcom_Get( sts, &ctx->qid, &get, tmo);
    if ( *sts == QCOM__TMO || !rmsg) {
      return 0;
    }

    if ( get.type.b == sev_cMsgClass && 
	 get.type.s == (qcom_eStype) sev_eMsgType_HistItems)
      break;

    qcom_Free( sts, rmsg);
  }
  
  *sts = rmsg->Status;
  if ( EVEN(*sts))
    return 0;

  int item_cnt = (get.size - sizeof(sev_sMsgHistItems)) / sizeof(sev_sHistItem) + 1;

  lp = (sevcli_sHistItem *) calloc( item_cnt, sizeof(sevcli_sHistItem));

  for ( i = 0; i < item_cnt; i++) {
    lp[i].oid = rmsg->Items[i].oid;
    strncpy( lp[i].oname, rmsg->Items[i].oname, sizeof(lp[0].oname));
    lp[i].storagetime = net_NetTimeToDeltaTime( &rmsg->Items[i].storagetime);
    lp[i].deadband = rmsg->Items[i].deadband;
    lp[i].options = rmsg->Items[i].options;
    lp[i].creatime = net_NetTimeToTime( &rmsg->Items[i].creatime);
    lp[i].modtime = net_NetTimeToTime( &rmsg->Items[i].modtime);
    strncpy( lp[i].description, rmsg->Items[i].description, sizeof(lp[0].description));
    lp[i].scantime = rmsg->Items[i].scantime;
    lp[i].attrnum = 1;
    lp[i].attr[0].type = rmsg->Items[i].attr[0].type;
    lp[i].attr[0].size = rmsg->Items[i].attr[0].size;
    strncpy( lp[i].attr[0].aname, rmsg->Items[i].attr[0].aname, sizeof(lp[0].attr[0].aname));
    strncpy( lp[i].attr[0].unit, rmsg->Items[i].attr[0].unit, sizeof(lp[0].attr[0].unit));
  }

  qcom_Free( sts, rmsg);

  *cnt = item_cnt;
  *list = lp;
  *sts = SEV__SUCCESS;
  return 1;
}
*/

int sevcli_get_itemdata( pwr_tStatus *sts, sevcli_tCtx ctx, pwr_tOid oid, 
			 char *aname, pwr_tTime starttime, pwr_tTime endtime, int numpoints,
			 pwr_tTime **tbuf, void **vbuf, int *rows,
			 pwr_eType *vtype, unsigned int *vsize)
{
  sev_sMsgHistDataGetRequest 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  int tmo = 30000;
  qcom_sGet get;
  pwr_tStatus lsts;
  
  if ( ctx->server)
    tgt.nid = ctx->server;
  else
    tgt.nid = ctx->qid.nid;
  tgt.qix = sev_eProcSevServer;
    

  put.reply = ctx->qid;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistDataGetRequest;
  put.msg_id = ctx->msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgHistDataGetRequest *) qcom_Alloc( sts, put.size);
  

  put.data = msg;

  msg->Type = sev_eMsgType_HistDataGetRequest;
  msg->Oid = oid;
  strncpy( msg->AName, aname, sizeof(msg->AName));
  msg->StartTime = net_TimeToNetTime( &starttime);
  msg->EndTime = net_TimeToNetTime( &endtime);
  msg->NumPoints = numpoints;

  // Empty queue
  sev_sMsgHistDataGet *rmsg;

  memset( &get, 0, sizeof(get));
  for (;;) {
    rmsg = (sev_sMsgHistDataGet *) qcom_Get(sts, &ctx->qid, &get, 0);
    if ( !rmsg)
      break;
  }

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
    return 0;
  }

  memset( &get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistDataGet *) qcom_Get(sts, &ctx->qid, &get, tmo);
    if ( *sts == QCOM__TMO || !rmsg)
      return 0;

    if ( get.type.b == sev_cMsgClass && 
	 get.type.s == (qcom_eStype) sev_eMsgType_HistDataGet &&
	 cdh_ObjidIsEqual( oid, rmsg->Oid) &&
	 cdh_NoCaseStrcmp( aname, rmsg->AName) == 0)
      break;

    qcom_Free( sts, rmsg);
  }
  
  *sts = rmsg->Status;
  if ( EVEN(*sts)) {
    qcom_Free( sts, rmsg);
    return 0;
  }

  int item_cnt = rmsg->NumPoints;

  *tbuf = malloc( item_cnt * sizeof(pwr_tTime));
  *vbuf = malloc( item_cnt * rmsg->VSize);

  memcpy( *tbuf, &rmsg->Data, item_cnt * sizeof(pwr_tTime));
  memcpy( *vbuf, (char *)&rmsg->Data + item_cnt * sizeof(pwr_tTime), item_cnt * rmsg->VSize);
  *rows = item_cnt;
  *vtype = rmsg->VType;
  *vsize = rmsg->VSize;
  
  qcom_Free( sts, rmsg);

  *sts = SEV__SUCCESS;
  return 1;
}


int sevcli_get_objectitemdata( pwr_tStatus *sts, sevcli_tCtx ctx, pwr_tOid oid, char *aname,
			 pwr_tTime starttime, pwr_tTime endtime, int numpoints,
			 pwr_tTime **tbuf, void **vbuf, int *rows,
			 sevcli_sHistAttr **histattr, int *numattributes)
{
  sev_sMsgHistDataGetRequest 	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  int tmo = 30000;
  qcom_sGet get;
  pwr_tStatus lsts;
  
  if ( ctx->server)
    tgt.nid = ctx->server;
  else
    tgt.nid = ctx->qid.nid;
  tgt.qix = sev_eProcSevServer;
    

  put.reply = ctx->qid;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistObjectDataGetRequest;
  put.msg_id = ctx->msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgHistDataGetRequest *) qcom_Alloc( sts, put.size);
  

  put.data = msg;

  msg->Type = sev_eMsgType_HistObjectDataGetRequest;
  msg->Oid = oid;
  strncpy( msg->AName, aname, sizeof(msg->AName));
  msg->StartTime = net_TimeToNetTime( &starttime);
  msg->EndTime = net_TimeToNetTime( &endtime);
  msg->NumPoints = numpoints;

  // Empty queue
  sev_sMsgHistObjectDataGet *rmsg;

  memset(&get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistObjectDataGet *) qcom_Get(sts, &ctx->qid, &get, 0);
    if ( !rmsg)
      break;
  }

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
    *sts = 0;
    return 0;
  }

  memset( &get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistObjectDataGet *) qcom_Get(sts, &ctx->qid, &get, tmo);
    if ( *sts == QCOM__TMO || !rmsg) {
      *sts = 0;
      return 0;
    }

    if ( get.type.b == sev_cMsgClass && 
         get.type.s == (qcom_eStype) sev_eMsgType_HistObjectDataGet &&
         cdh_ObjidIsEqual( oid, rmsg->Oid) &&
         cdh_NoCaseStrcmp( aname, rmsg->AName) == 0)
      break;

    qcom_Free( sts, rmsg);
  }
  
  *sts = rmsg->Status;
  if ( EVEN(*sts)) {
    qcom_Free( sts, rmsg);
    return 0;
  }
  if( rmsg->NumPoints == 0 ) {
    qcom_Free( sts, rmsg);
    *sts = 0;
    return 0;
  }

  *numattributes = rmsg->NumAttributes;
  int item_cnt = rmsg->NumPoints;
  unsigned int timebufsize = item_cnt * sizeof(pwr_tTime);
  unsigned int databufsize = rmsg->TotalDataSize - timebufsize;
  *tbuf = malloc( timebufsize);
  *vbuf = malloc( databufsize );
  sevcli_sHistAttr *attrptr;
  attrptr = calloc( rmsg->NumAttributes, sizeof(sevcli_sHistAttr) );

  int attrCount = rmsg->NumAttributes;
  void *ptr = &rmsg->Attr[attrCount];
  memcpy( *tbuf, ptr, item_cnt * sizeof(pwr_tTime));
  memcpy( *vbuf, (char *)ptr + item_cnt * sizeof(pwr_tTime), databufsize );
  *rows = item_cnt;

  int i = 0;
  for( i = 0; i < rmsg->NumAttributes; i++ ) {
    strncpy(attrptr[i].aname, rmsg->Attr[i].aname, sizeof(attrptr[0].aname));
    attrptr[i].type = rmsg->Attr[i].type;
    attrptr[i].size = rmsg->Attr[i].size;
  }
  *histattr = attrptr;
  
  qcom_Free( sts, rmsg);

  *sts = SEV__SUCCESS;
  return 1;
}


int sevcli_delete_item( pwr_tStatus *sts, sevcli_tCtx ctx, pwr_tOid oid, char *aname)
{
  sev_sMsgHistItemDelete	*msg;
  qcom_sQid   	tgt;
  qcom_sPut	put;
  int tmo = 10000;
  qcom_sGet get;
  pwr_tStatus lsts;
  
  if ( ctx->server)
    tgt.nid = ctx->server;
  else
    tgt.nid = ctx->qid.nid;
  tgt.qix = sev_eProcSevServer;
    

  put.reply = ctx->qid;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItemDelete;
  put.msg_id = ctx->msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgHistItemDelete *) qcom_Alloc( sts, put.size);
  

  put.data = msg;

  msg->Type = sev_eMsgType_HistItemDelete;
  msg->Oid = oid;
  strncpy( msg->AName, aname, sizeof(msg->AName));

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
    return 0;
  }

  sev_sMsgHistItemStatus *rmsg;

  memset( &get, 0, sizeof(get));

  for (;;) {
    rmsg = (sev_sMsgHistItemStatus *) qcom_Get(sts, &ctx->qid, &get, tmo);
    if ( *sts == QCOM__TMO || !rmsg)
      return 0;

    if ( get.type.b == sev_cMsgClass && 
	 get.type.s == (qcom_eStype) sev_eMsgType_HistItemStatus &&
	 cdh_ObjidIsEqual( oid, rmsg->Oid) &&
	 cdh_NoCaseStrcmp( aname, rmsg->AName) == 0)
      break;

    qcom_Free( sts, rmsg);
  }
  
  *sts = rmsg->Status;
  if ( EVEN(*sts))
    return 0;
  qcom_Free( sts, rmsg);
  return 1;
}
