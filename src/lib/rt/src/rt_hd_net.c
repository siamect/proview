/* rt_hd_net.c -- HD net routines

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   <Description>.  */

#include "rt_hd_net.h"
#include "rt_hd_msg.h"
#include "rt_qcom.h"
#include "rt_errh.h"


pwr_tStatus                  
hd_NetSendMessage(           
  qcom_sQid *qid,            
  co_sPlatform *recPlatform,  
  int subtype,               
  hd_sHead *hp,              
  unsigned int size          
) {
  pwr_tStatus sts = HD__SUCCESS;
  pwr_tStatus sts2;
  qcom_sPut   msg;
  XDR xdrs;

  if ((msg.data = (char*)qcom_Alloc(&sts, size)) == NULL) {
    return HD__INSVIRMEM;
  }

  msg.type.b = hd_cMsgClass;
  msg.type.s = hp->type = subtype;
  msg.reply = hp->qid;
  msg.size = size;

  if (recPlatform == NULL || co_IsXdrNeeded(&hp->platform, recPlatform)) {
    hp->xdr = TRUE;
    xdrmem_create(&xdrs, msg.data, size, XDR_ENCODE);
    sts = hd_NetXdrMessage(&xdrs, subtype, hp);
    if (EVEN(sts)) {
      qcom_Free(&sts2, msg.data);
      return sts;
    }
  } else {
    hp->xdr = FALSE;
    memcpy(msg.data, hp, size);
  }


  qcom_Put(&sts, qid, &msg);
  if (EVEN(sts)) {
    qcom_Free(&sts2, msg.data);
  } 

  return sts;
}                           

pwr_tStatus                  
hd_NetXdrMessage (  
  XDR *xdrs,                  
  int subtype,               
  hd_sHead *hp              
) {

  if (!xdr_hd_sHead(xdrs, hp))  {
    return HD__XDRFAILED;
  }

  switch (hp->type) {    
  case hd_eMsg_hello:
    if (!xdr_hd_sHello(xdrs, (hd_sHello *)(hp + 1))) 
      return HD__XDRFAILED;
    break;
  case hd_eMsg_ack:
    if (!xdr_hd_sAck(xdrs, (hd_sAck *)(hp + 1))) 
      return HD__XDRFAILED;
    break;
  case hd_eMsg_adata:
    if (!xdr_hd_sAdata(xdrs, hp + 1)) 
      return HD__XDRFAILED;
    break;
  case hd_eMsg_sdata:
    if (!xdr_hd_sSdata(xdrs, hp + 1)) 
      return HD__XDRFAILED;
    break;
  default:
    errh_Info("Unexpected message type: %d, (%s)",
      hp->type, qcom_QidToString(NULL, &hp->qid, 1));
    return HD__XDRUNEXPECT;
    break;
  }

  return HD__SUCCESS;   
}                           
