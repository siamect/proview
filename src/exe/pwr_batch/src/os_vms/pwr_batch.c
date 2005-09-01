/* 
 * Proview   $Id: pwr_batch.c,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#include <stdio.h>
#include <rms.h>
#include <string.h>
#include <stdlib.h>
#include <starlet.h>
#include <jbcmsgdef.h>
#include "pwr.h"
#include "co_time.h"
#include "co_utl_batch.h"

#define cProgName "cs_utl_batch_dispatcher"
#define cLogStart "-- PwrBatchStartLog"
#define cLogEnd "-- PwrBatchEndLog"

static int
ParseFilename (
  char *InFilename,
  char *OutFilename
);

static void
PrintLogFile (
  char *LogFile
);

main (
  int argc,
  char *argv[]
) {
  int sts;
  char *Par[9];
  char *Queue;
  char *File;
  int entry;
  int i = 0;
  int j = 0;
  pwr_tTime time;
  int number;
  char Logfile[256];
  char DeleCommand[256];
  int Entry;
  
  if (argc < 3) {
    printf("%s, to few arguments!", cProgName);
    exit(0);
  }

  Queue = argv[1];
  File = argv[2];

  for (j=0, i=3; argv[i] != NULL && j <= 8; i++, j++) {
    Par[j] = argv[i];
  }
  Par[j] = NULL;

  clock_gettime(CLOCK_REALTIME, &time);
  srand(time.tv_sec);
  number = rand();
  sprintf(Logfile, "pwrp_tmp:%s_%010.10d.log", Queue, number);
  sts = ParseFilename(Logfile, Logfile);
  sprintf(DeleCommand, "$ delete/nolog/noconf %s*", Logfile);

  sts = utl_BatchFindQueue(Queue);
  if (!(sts & 1)) exit(sts);
  sts = utl_BatchEnterJob(&Entry, Queue, File, Logfile, Par);
  if (!(sts & 1)) exit(sts);
  utl_BatchWaitJob(&Entry);
  PrintLogFile(Logfile);
  system(DeleCommand);
  exit(sts);
}


static void
PrintLogFile (
  char *LogFile
) {
  FILE *lf;
  char Line[1024];
  char *s;
  int log;
  int len;
  int lc;
  int startlen = strlen(cLogStart);
  int endlen = strlen(cLogEnd);

  lf = fopen(LogFile, "r");
  if (lf == NULL) {
    printf("!! Could not read logfile: %s", LogFile);
    perror(LogFile);
    return;
  }

  for (log = 0, lc = 0;;) {
    s = fgets(Line, sizeof(Line), lf);
    if (s == NULL)
      break;

    if (!log) {
      if (!strncmp(Line, cLogStart, startlen)) log = 1;
    } else if (!strncmp(Line, cLogEnd, endlen)) {
      break;
    } else {
      len = strlen(Line);
      Line[len - 1] = '\0';
      puts(Line);
      lc++;
    }
  }

  fclose(lf);
  if (lc == 0) {
    lf = fopen(LogFile, "r");
    if (lf == NULL) {
      printf("!! Could not read logfile: %s", LogFile);
      perror(LogFile);
      return;
    }

    for (;;) {
      s = fgets(Line, sizeof(Line), lf);
      if (s == NULL)
	break;

      len = strlen(Line);
      Line[len - 1] = '\0';
      puts(Line);
    }

    fclose(lf);
  }
  return;
}

static int
ParseFilename (
  char *InFilename,
  char *OutFilename
)
{
  int sts;
  struct NAM nam;
  struct FAB fab;
  char filename[NAM$C_MAXRSS];

  fab = cc$rms_fab;
  nam = cc$rms_nam;

  fab.fab$l_fna = InFilename;
  fab.fab$b_fns = strlen(InFilename);
  fab.fab$l_nam = &nam;
  nam.nam$l_esa = (char *) &filename;
  nam.nam$b_ess = sizeof(filename);
  nam.nam$v_noconceal = 1;

  sts = sys$parse(&fab, 0, 0);
  if (!(sts & 1)) return sts;

  memcpy(OutFilename, filename, nam.nam$b_esl);
  OutFilename[nam.nam$b_esl] = '\0';

  return sts;
}
