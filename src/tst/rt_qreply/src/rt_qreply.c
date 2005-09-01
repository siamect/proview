/* 
 * Proview   $Id: rt_qreply.c,v 1.2 2005-09-01 14:58:00 claes Exp $
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

#if defined(OS_ELN)
# include stdlib
# include string
# include time
#else
# include <stdlib.h>
# include <string.h>
# include <time.h>
#endif

#if defined(OS_VMS)
# include <lib$routines.h>
# include <in.h>
# include <inet.h>
#endif

#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qdb_msg.h"


main (int argc, char *argv[])
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qcom_sQid	fqid;
  qcom_sGet	get;
  qcom_sPut	put;
  qdb_sInit	init;
  qdb_sNode	*np;
  qcom_sQattr	attr;
  void		*p;

  if (!qcom_Init(&sts, NULL)) exit(sts);

  attr.type = qcom_eQtype_forward;
  attr.quota = 100;
  fqid.nid = pwr_cNNodeId;
  fqid.qix = 550715;

  if(!qcom_CreateQ(&sts, &fqid, &attr)) exit(sts);

  qid.nid = pwr_cNNodeId;
  qid.qix = qdb_cIloopBack;

  if(!qcom_CreateQ(&sts, &qid, NULL)) exit(sts);
  if(!qcom_Bind(&sts, &qid, &fqid)) exit(sts);

  while (1) {
    get.data = NULL;
    p = qcom_Get(&sts, &qid, &get, 6000000);  
    if (p == NULL) continue;
    printf("\nsender..: %d,%d\n", get.sender.nid, get.pid);
    printf("receiver: %d,%d\n", get.receiver.nid, get.receiver.qix);
    printf("reply...: %d,%d\n", get.reply.nid, get.reply.qix);
    printf("type....: %d,%d\n", get.type.b, get.type.s);
    printf("size....: %d\n", get.size);
    put.reply = qid;
    put.type = get.type;
    put.size = get.size;
    put.data = p;
    qcom_Reply(&sts, &get, &put);
    if (EVEN(sts))
      qcom_Free(&sts, p);
  }
}
