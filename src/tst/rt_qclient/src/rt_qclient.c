#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"


#define DATA_SIZE 10000

main (int argc, char *argv[])
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qcom_sGet	get;
  qcom_sPut	put;
  qcom_sQid	loop;
  int		data[DATA_SIZE];
  int		i;
  int		j;
  int		max = 1000;
  char		*sp;

  qid = qcom_cNQid;

  if (argc < 2) exit(12);
  if (argc > 2) max = atoi(argv[2]);

  for (j = 0; j < DATA_SIZE; j++)
    data[j] = j;

  loop.nid = atoi(argv[1]);
  loop.qix = qcom_cIloopBack;

  if (!qcom_Init(&sts, NULL)) exit(sts);
  if (!qcom_CreateQ(&sts, &qid, NULL)) exit(sts);

  for (i = 1; i < max; i++) {
    for (j = 0; j < DATA_SIZE; j++)
      ++data[j];
    sp = qcom_Alloc(&sts, sizeof(data));
    if (sp == NULL) exit(sts);
    memcpy(sp, &data, sizeof(data));
    put.type.b = 10;
    put.type.s = i;
    put.reply = qid;
    put.size = sizeof(data);
    put.data = sp;
    qcom_Put(&sts, &loop, &put);
    if (EVEN(sts)) exit(sts);
    get.data = NULL;
    sp = qcom_Get(&sts, &qid, &get, qcom_cTmoEternal);  
    if (sp == NULL) continue;
    for (j = 0; j < DATA_SIZE; j++) {
      if (data[j] != *(int *)sp) {
	printf("index...: %d, (%d)\n", j, data[j]);
	break;
      }
      sp += sizeof(int);
    }
//    printf("\nsender..: %d,%X\n", get.sender.nid, get.pid);
//    printf("receiver: %d,%d\n", get.receiver.nid, get.receiver.qix);
//    printf("reply...: %d,%d\n", get.reply.nid, get.reply.qix);
//    printf("type....: %d,%d\n", get.type.b, get.type.s);
//    printf("size....: %d\n", get.size);
//    printf("memcmp..: %d\n", memcmp(get.data, &data, sizeof(data)));
//    printf("index...: %d, (%d)\n", j, data[j]);
    qcom_Free(&sts, get.data);
  }

  qcom_Exit(&sts);
}
