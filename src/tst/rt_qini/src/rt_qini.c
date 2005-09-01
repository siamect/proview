/* 
 * Proview   $Id: rt_qini.c,v 1.2 2005-09-01 14:58:00 claes Exp $
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

#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qini.h"
#include "rt_qdb_msg.h"
#include "co_tree.h"
#include "co_cdh.h"

main () {
  pwr_tStatus	sts;
  tree_sTable	*nodes;
  qini_sNode	*nep;
  qini_sNode	*me = NULL;
  char		*s;
  FILE		*f;
  char		my_name[80];
  int		len = sizeof(my_name);
  int		errors;

  gethostname(my_name, len);
  if ((s = strchr(my_name, '.')) != NULL)
    *s = '\0';

  printf("-- Starting QCOM on node: %s\n", my_name);

  f = fopen("sys$login:ld_hosts_001.dat", "r");
  if (f == NULL) {
    perror("sys$login:ld_hosts_001.dat");
    exit(1);
  }

  nodes = tree_CreateTable(sizeof(pwr_tNodeId), offsetof(qini_sNode, nid),
    sizeof(qini_sNode), 10, tree_eComp_nid, NULL);

  errors = qini_ParseFile(f, nodes);
  if (errors != 0) {
    printf("** %d errors where found, qcom will not be started!\n", errors);
    exit(1);
  }
  
  for (
    nep = tree_Minimum(nodes);
    nep != NULL;
    nep = tree_Successor(nodes, nep)
  ) { 
    printf("%s %d %d %d %d\n", nep->name, nep->nid, nep->naddr, nep->port,
      nep->connect);
    if (strcmp(my_name, nep->name) == 0) {
      printf("-- My nid is: %d\n", nep->nid);
      me = nep;
    }
  }

  if (me == NULL) {
    printf("** Could not find myself!\n");
    exit(1);
  }

  qini_BuildDb(&sts, nodes, me, NULL, 1);

  qdb->g->log.m = 7;

  sleep(20000000);
}
