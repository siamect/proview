/* ini_rc.c -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

     */

#if defined(OS_ELN)
# include $vaxelnc
# include stdio
# include errno
# include stdlib
#else
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
#endif

#include "pwr.h"
#include "rt_gdb.h"
#include "rt_load.h"

#include "ini_rc.h"


typedef struct {
  char			*name;
  int			*var;
  int			val;
} ini_sRc;


static pwr_tStatus
setRcValue(
  ini_sRc 		*def_rc, 
  char			*name, 
  int			interval
) {
  ini_sRc		*rc;

  for (rc = def_rc; rc && rc->name; rc++) {
    if (strcmp(rc->name, name) == 0) {
      *rc->var = interval;
      return 1;
    }
  }

  printf("There is no variable named %s\n", name);
  return 0; 
}
  



pwr_tStatus
ini_RcReadAndSet(
  const char		*dir,
  const char		*nodename,
  int			busid
) {

#ifdef OS_VMS
#pragma message disable NEEDCONSTEXT
#endif

  ini_sRc 		def_rc[] = {
    {"cacheTrim",  	(int *)&gdbroot->db->cache_trim_int, 	1000},
    {"sancAdd",     	(int *)&gdbroot->db->sanc_add_int, 	1000},
    {"sancExpired",	(int *)&gdbroot->db->sanc_exp_int,     60000},
    {"sansCheck", 	(int *)&gdbroot->db->sans_chk_int, 	1500},
    {"subcCheck",	(int *)&gdbroot->db->subc_chk_int, 	3000},
    {NULL,NULL,0}
  };

#ifdef OS_VMS
#pragma message enable NEEDCONSTEXT
#endif

  FILE 			*fp;
  ini_sRc 		*rc;
  char 			*s;
  char 			buf[256];
  char			name[128];
  char			filename[128];
  int			interval;

  
  for (rc = def_rc; rc && rc->name; rc++)
    *rc->var = rc->val;

  sprintf(filename, load_cNameRc, dir, nodename, busid);

  cdh_ToLower(filename, filename);
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("** Could not read '%s'\n%s", filename, strerror(errno));
  } else {    
    printf("-- Reading rc file %s\n", filename);
    for (
      s = fgets(buf, sizeof(buf) - 1, fp);
      s != NULL;
      s = fgets(buf, sizeof(buf) - 1, fp)
    ) {
      printf("-- %s", buf);
      if (buf[0] == '#') continue;
      name[0] = '\0';
      sscanf(buf, "%s %d\n", name, &interval);
      if (name[0] == '\0') continue;
      setRcValue(def_rc, name, interval);
    }
    fclose(fp);
  }

  return 1;
}




