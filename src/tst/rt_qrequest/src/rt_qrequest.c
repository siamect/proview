/* 
 * Proview   $Id: rt_qrequest.c,v 1.2 2005-09-01 14:58:00 claes Exp $
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

#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"


main (int argc, char *argv[])
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qcom_sGet	get;
  qcom_sPut	put;
  qcom_sQid	loop;
  int		data[500];
  int		i;
  int		j;
  int		max = 1000;
  char		*sp;
  int tmo;

  qid = qcom_cNQid;

  if (argc < 2) exit(12);
  if (argc > 2) max = atoi(argv[2]);

  for (j = 0; j < 500; j++)
    data[j] = j;

  loop.nid = atoi(argv[1]);
  loop.qix = qcom_cIloopBack;

  if (!qcom_Init(&sts, NULL)) exit(sts);
  if (!qcom_CreateQ(&sts, &qid, NULL)) exit(sts);

  for (i = 1; i < max; i++) {
    for (j = 0; j < 500; j++)
      ++data[j];
    sp = qcom_Alloc(&sts, sizeof(data));
    if (sp == NULL) exit(sts);
    memcpy(sp, &data, sizeof(data));
    put.type.b = 10;
    put.type.s = i;
    put.reply = qid;
    put.size = sizeof(data);
    put.data = sp;

    get.data = NULL;

    
    tmo = 10 + (i%3)*3000;
    sp = qcom_Request(&sts, &loop, &put, &qid, &get, tmo);

    if (sp == NULL) {
      printf("\n%d tmo: %d empty buffer\n", i, tmo);
      continue;
    }
    for (j = 0; j < 500; j++) {
      if (data[j] != *(int *)sp) break;
      sp += sizeof(int);
    }
    printf("\nsender..: %d,%X\n", get.sender.nid, get.pid);
    printf("receiver: %d,%d\n", get.receiver.nid, get.receiver.qix);
    printf("reply...: %d,%d\n", get.reply.nid, get.reply.qix);
    printf("type....: %d,%d\n", get.type.b, get.type.s);
    printf("size....: %d\n", get.size);
    printf("memcmp..: %d\n", memcmp(get.data, &data, sizeof(data)));
    printf("index...: %d, (%d)\n", j, data[j]);
    qcom_Free(&sts, get.data);
  }

  qcom_Exit(&sts);
}
