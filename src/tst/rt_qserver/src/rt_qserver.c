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
    p = qcom_Get(&sts, &qid, &get, 60000);  
    if (p == NULL) continue;
//    printf("\nsender..: %d,%d\n", get.sender.nid, get.pid);
//    printf("receiver: %d,%d\n", get.receiver.nid, get.receiver.qix);
//    printf("reply...: %d,%d\n", get.reply.nid, get.reply.qix);
//    printf("type....: %d,%d\n", get.type.b, get.type.s);
//    printf("size....: %d\n", get.size);
    put.reply = qid;
    put.type = get.type;
    put.size = get.size;
    put.data = p;
    qcom_Put(&sts, &get.reply, &put);
    if (EVEN(sts))
      qcom_Free(&sts, p);
  }
}
