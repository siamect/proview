/* 
 * Proview   $Id: rt_qini.c,v 1.3 2008-09-05 08:59:23 claes Exp $
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

/* rt_qini.c -- Queue Communication, initiation  */

#include "pwr.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qdb_msg.h"
#include "co_tree.h"
#include "co_cdh.h"
#include "rt_qini.h"
#include "rt_inet.h"


static qdb_sNode *
addNode (
  qini_sNode  *nep
)
{
  pwr_tStatus sts;
  qdb_sNode   *np;

  np = qdb_AddNode(&sts, nep->nid, 0);
  if (np == NULL) return NULL;

  strcpy(np->name, nep->name);
  np->sa.sin_family = AF_INET;  
  np->sa.sin_addr.s_addr = nep->naddr.s_addr;
  if (nep->flags.b.port)
    np->sa.sin_port = htons(nep->port);
  else
    np->sa.sin_port = htons(55000 + qdb->g->bus);  

  memcpy(&np->arp.arp_pa.sa_data, &np->sa.sin_addr.s_addr, sizeof(np->sa.sin_addr.s_addr));

  np->connection = nep->connection;

  return np;
}

static qdb_sQue *
addQueue (
  qdb_sAppl	*ap,
  qcom_tQix	qix,
  char		*name,
  qdb_eQue	type,
  pwr_tBitMask	flags
)
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qdb_sQue	*qp;

  qid.nid = qdb->g->nid;
  qid.qix = qix;
  qp = qdb_AddQue(&sts, qix);
  if (qp == NULL) errh_Bugcheck(sts, "qdb_AddQue(&sts, &qid)");
  qp->type = type;
  qp->flags.m = flags;
  strcpy(qp->name, name);                    

  if (qp->flags.b.broadcast) {
    qdb_AddBond(&sts, qp, qdb->exportque);
  }

  if (ap != NULL) {
    pool_QinsertPred(&sts, &qdb->pool, &qp->que_ll, &ap->que_lh);
    qp->aid = ap->aid;
  }

  return qp;
}

int
qini_ParseFile (
  FILE		*f,
  tree_sTable	*ntp,
  int		*warnings,
  int		*errors,
  int		*fatals
)
{
  pwr_tStatus	sts = 1;
  int		n;
  char		*s;
  char		buffer[256];
  int		error = 0;
  char		name[80];
  char		s_nid[80];
  char		s_naddr[80];
  char		s_port[80];
  char		s_connection[80];
  pwr_tNodeId	nid;
  struct in_addr	naddr;
  qini_sNode	*nep;
  struct arpreq	arpreq;

  while ((s = fgets(buffer, sizeof(buffer) - 1, f)) != NULL) {

    if (*s == '#' || *s == '!') {
      s++;
      continue;
    }

    n = sscanf(s, "%s %s %s %s %s", name, s_nid, s_naddr, s_port, s_connection);
    if (n < 3) {
      errh_Error("error in line, <wrong number of arguments>, skip to next line.\n>> %s", s);
      (*errors)++;
      continue;
    }

    sts = cdh_StringToVolumeId(s_nid, (pwr_tVolumeId *)&nid);
    if (EVEN(sts)) {
      errh_Error("error in line, <node identity>, skip to next line.\n>> %s", s);
      (*errors)++;
      continue;
    }

    naddr.s_addr = inet_network(s_naddr);
#if defined(OS_VMS) 
    if (naddr.s_addr == (in_addr_t)-1) {
#else
    if (naddr.s_addr == (unsigned long)-1) {
#endif
      errh_Error("error in line, <network address>, skip to next line.\n>> %s", s);
      (*errors)++;
      continue;
    }

    nep = tree_Find(&sts, ntp, &nid);
    if (nep != NULL) {
      errh_Warning("node is allready defined: %s, skip to next line", s);
      (*warnings)++;
      continue;
    } else {
      nep = tree_Insert(&sts, ntp, &nid);
    }

    strcpy(nep->name, name);
    nep->naddr.s_addr = htonl(naddr.s_addr);
    if (n > 3) nep->port = htons(atoi(s_port));
    if (n > 4) nep->connection = atoi(s_connection);
    memset(&arpreq, 0, sizeof(arpreq));
    memcpy(&arpreq.arp_pa.sa_data, &naddr, sizeof(naddr));
    inet_GetArpEntry(&sts, 0, &arpreq);
  }

  return error;
}

pwr_tBoolean
qini_BuildDb (
  pwr_tStatus		*sts,
  tree_sTable		*nodes,
  qini_sNode		*me,
#if 0 /* change when new class NetConfig is deined */
  pwr_sNetConfig	*cp,
#else
  void			*cp,
#endif
  qcom_tBus		bus
)
{
  qdb_sInit		init;
  qdb_sNode		*np;
  qini_sNode		*nep;
  void			*p;
  qdb_sAppl		*ap;

  memset(&init, 0, sizeof(init));

  init.nid	      = me->nid;
  init.bus	      = bus;
  init.nodes	      = nodes->nNode;
#if 0	/* change when new class NetConfig is deined */
  init.queues	      = cp->Queues;
  init.applications   = cp->Applications;
  init.sbufs	      = cp->SmallCount;
  init.mbufs	      = cp->MediumCount;
  init.lbufs	      = cp->LargeCount;
  init.size_sbuf      = cp->SmallSize;
  init.size_mbuf      = cp->MediumSize;
  init.size_lbuf      = cp->LargeSize;
#endif

  p = qdb_CreateDb(sts, &init);
  if (p == NULL) return NO;

  qdb_ScopeLock {

    for (nep = tree_Minimum(sts, nodes); nep != NULL; nep = tree_Successor(sts, nodes, nep)) {
      np = addNode(nep);
    }

    ap = qdb_AddAppl(NULL, YES);

    qdb->exportque = addQueue(NULL, qdb_cIexport, "export", qdb_eQue_private, qdb_mQue_system);
    addQueue(ap, qcom_cInetEvent, "netEvent", qdb_eQue_forward, qdb_mQue_system);
    addQueue(ap, qcom_cIapplEvent, "applEvent", qdb_eQue_forward, qdb_mQue_system);
    addQueue(ap, qcom_cImhAllHandlers, "allHandlers", qdb_eQue_forward, qdb_mQue_broadcast);
    addQueue(ap, qcom_cImhAllOutunits, "allOutunits", qdb_eQue_forward, qdb_mQue_broadcast);
    addQueue(ap, qcom_cIhdServer, "hdServer", qdb_eQue_forward, qdb_mQue_broadcast);
    addQueue(ap, qcom_cIhdClient, "hdClient", qdb_eQue_forward, qdb_mQue_broadcast);
    addQueue(NULL, qcom_cInacp, "nacp", qdb_eQue_private, qdb_mQue_system);
    addQueue(ap, qcom_cIini, "ini", qdb_eQue_forward, qdb_mQue_system | qdb_mQue_event);

  } qdb_ScopeUnlock;

  return (YES);
}
