/* 
 * Proview   $Id: ini.c,v 1.2 2008-09-18 14:37:43 claes Exp $
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
#include <ctype.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_dbs.h"
#include "co_tree.h"
#include "co_time.h"
#include "co_errno.h"
#include "rt_qcom.h"
#include "ini.h"
#include "rt_ini_msg.h"
#include "rt_lst.h"
#include "rt_errh.h"
#include "co_syi.h"

#define cPrio_base		0
#define cPrio_qmon		(cPrio_base + 19)
#define cPrio_sev_server	(cPrio_base + 6)

static char		*strsav (char*);

static char *strsav( char *s)
{
  int	  len;
  char	  *t = NULL;

  if (s == NULL)
    return NULL;

  len = strlen(s);
  if (len > 0) {
    t = malloc(len + 1);
    strcpy(t, s);
  }

  return t;
}

ini_sContext *ini_CheckContext (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  ini_LoadDirectory(sts, cp);

  if (!cp->flags.b.aliasfile)
    sprintf(cp->aliasfile.name, dbs_cNameAlias, cp->dir);
  cdh_ToLower(cp->aliasfile.name, cp->aliasfile.name);
  cp->nodefile.errcount = NULL;
  cp->nodefile.logOpenFail = errh_LogInfo;

  if (!cp->flags.b.applfile)
    sprintf(cp->applfile.name, dbs_cNameAppl, cp->dir, cp->nodename, cp->busid);
  cdh_ToLower(cp->applfile.name, cp->applfile.name);
  cp->applfile.errcount = NULL;
  cp->applfile.logOpenFail = errh_LogInfo;

  if (!cp->flags.b.bootfile)
    sprintf(cp->bootfile.name, dbs_cNameBoot, cp->dir, cp->nodename, cp->busid);
  cdh_ToLower(cp->bootfile.name, cp->bootfile.name);
  cp->bootfile.errcount = &cp->fatals;
  cp->bootfile.logOpenFail = errh_LogFatal;

  if (!cp->flags.b.nodefile)
    sprintf(cp->nodefile.name, dbs_cNameNode, cp->dir, cp->nodename, cp->busid);
  cdh_ToLower(cp->nodefile.name, cp->nodefile.name);
  cp->nodefile.errcount = &cp->fatals;
  cp->nodefile.logOpenFail = errh_LogFatal;

  return cp;  
}

ini_sContext *ini_CreateContext (
  pwr_tStatus	*status
)
{
  ini_sContext *cp;

  pwr_dStatus(sts, status, INI__SUCCESS);

  cp = calloc(1, sizeof(*cp));
  if (cp == NULL)
    return NULL;

  cp->nid_t = tree_CreateTable(sts, sizeof(pwr_tNodeId),
    offsetof(qini_sNode, nid), sizeof(qini_sNode), 10,
    tree_Comp_nid);

  cp->proc_t = tree_CreateTable(sts, sizeof(((ini_sProc *)0)->id),
    offsetof(ini_sProc, id), sizeof(ini_sProc), 10,
    tree_Comp_strncmp);

  lst_Init(NULL, &cp->proc_lh, NULL);

  co_GetOwnFormat(&cp->format);

  return cp;  
}

FILE *ini_OpenFile (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sFile	*fp

)
{
  FILE *f;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = fopen(fp->name, "r");
  if (f == NULL) {
    if (fp->errcount)
      (*fp->errcount)++;
    if (fp->logOpenFail)
      fp->logOpenFail(&cp->log, "Could not open file %s", fp->name);
    return NULL;
  }

  if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "Opened file %s", fp->name);

  return f;
}

char *ini_LoadDirectory (
  pwr_tStatus	*status,
  ini_sContext *cp
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  if (!cp->flags.b.nodename)
    syi_NodeName(sts, cp->nodename, sizeof(cp->nodename));
  if (!cp->flags.b.hostname)
    syi_HostName(sts, cp->hostname, sizeof(cp->hostname));

  syi_NodeSpec(sts, cp->nodespec, sizeof(cp->nodespec));
  syi_HostSpec(sts, cp->hostspec, sizeof(cp->hostspec));
  syi_BootDisk(sts, cp->bootdisk, sizeof(cp->bootdisk));

  {
    char *s;

    if ((s = getenv(dbs_cNameBaseDirectory)) != NULL)
      sprintf(cp->bdir, "%s/", s);
    else
      errh_LogError(&cp->log, "Environment variable '%s' is not defined", dbs_cNameBaseDirectory);

    if ((s = getenv(dbs_cNameDirectory)) != NULL)
      sprintf(cp->dir, "%s/", s);
    else
      errh_LogError(&cp->log, "Environment variable '%s' is not defined", dbs_cNameDirectory);

    if (!cp->flags.b.busid) {
      s = getenv(pwr_dEnvBusId);
      if (s != NULL)

	cp->busid = atoi(s);
    }
  }

  return cp->dir;
}

void
ini_ReadBootFile (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  char			day[80];
  char			time[80];
  char			buffer[256];
  char			*s;
  int			i;
  int			n;
  int			nvol;

  /*  pwr_tProjVersion	ver;*/
  FILE *f;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = ini_OpenFile(sts, cp, &cp->bootfile);

  if (f == NULL)
    return;

  errh_LogInfo(&cp->log, "Reading Boot file %s", cp->bootfile.name);

  for (
    i = 0, nvol = 0, s = fgets(buffer, sizeof(buffer) - 1, f);
    s != NULL;
    s = fgets(buffer, sizeof(buffer) - 1, f)
  ) {
    if (*s == '!') {
      s++;
      continue;
    }

    switch (i) {
    case 0:	/* Creation Date.  */
      i++;
      time[0] = day[0] = '\0';
      n = sscanf(s, "%s %s", day, time);
      break;
    case 1:
      i++;
      cp->proj[0] = '\0';
      n = sscanf(s, "%s", cp->proj);
      errh_LogInfo(&cp->log, "Created at %s %s for project: %s", day, time, cp->proj);
      break;
    case 2:
      i++;
      cp->group[0] = '\0';
      n = sscanf(s, "%s", cp->group);
      break;
    }
  }

  fclose(f);
}

pwr_tBoolean ini_ReadNodeFile (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  FILE *f;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = ini_OpenFile(sts, cp, &cp->nodefile);

  if (f == NULL)
    return NO;

  errh_LogInfo(&cp->log, "Reading Node file %s", cp->nodefile.name);

  qini_ParseFile(f, cp->nid_t, &cp->warnings, &cp->errors, &cp->fatals);
  fclose(f);

  return YES;
}

ini_sProc *ini_ProcInsert (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  char		*id,
  char		*name,
  int		load,
  int		run,
  char		*file,
  int		prio,
  int		debug,
  char		*arg
)
{
  ini_sProc	*pp;
  char		buf[255];
  char          *s;
  int           ret;
  struct stat   f_stat;

  pwr_dStatus(sts, status, INI__SUCCESS);

  pp = tree_Insert(sts, cp->proc_t, id);
  if (pp == NULL) return NULL;

  if (name != NULL && name[0] != '\0' && strcmp(name, "\"\"")) {
    if (pp->proc.name != NULL) free(pp->proc.name);
    sprintf(buf, name, cp->busid);
    pp->proc.name = strsav(buf);
  }
  if (load != -1) pp->proc.flags.b.load = load != 0;
  if (run != -1) pp->flags.b.run = run != 0;
  if (file != NULL && file[0] != '\0' && strcmp(file, "\"\"")) {
    if (pp->proc.file != NULL) free(pp->proc.file);
      pp->proc.file = strsav(file);
#if defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
      s = getenv("pwr_exe");
      sprintf(buf, "%s/%s", s, file);
      ret = stat(buf, &f_stat);
      if (ret == -1)
      {
        s = getenv("pwrp_exe");
	sprintf(buf, "%s/%s", s, file);
	ret = stat(buf, &f_stat);
	if (ret == -1)
	{
	  pp->flags.b.run  = 0;
	  pp->proc.flags.b.load = 0;
	}
      }
#endif
  }
  if (arg != NULL && arg[0] != '\0' && strcmp(arg, "\"\"")) {
    if (pp->proc.arg != NULL) free(pp->proc.arg);
    pp->proc.arg = strsav(arg);
  }
  if (prio != -1) pp->proc.p_prio = prio;
  if (debug != -1) pp->proc.flags.b.debug = debug != 0;
  if (!lst_IsLinked(NULL, &pp->proc_ll)) {
    lst_InsertPred(NULL, &cp->proc_lh, &pp->proc_ll, pp);
  }

  return pp;
}

void  ini_ProcStart (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sProc	*pp
)  
{

  pwr_dStatus(sts, status, INI__SUCCESS);

  if (pp->flags.b.running) 
    return;
  
  if (pp->flags.b.run) {
    errh_LogInfo(&cp->log, "Starting %s, file: %s, prio: %d", pp->id, pp->proc.file, pp->proc.p_prio);
  } else {
    errh_LogInfo(&cp->log, "%s, file: %s, prio: %d, will not be started.", pp->id, pp->proc.file, pp->proc.p_prio);
    return;
  }

  *sts = proc_Start(&pp->proc);
  if (EVEN(*sts))
    errh_LogError(&cp->log, "Error starting %s, %m", pp->id, *sts);
}

void ini_ProcPrio (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sProc	*pp
)  
{

  char    set[100];
  
  pwr_dStatus(sts, status, INI__SUCCESS);

  if (pp->flags.b.running) 
    return;
  
  if (pp->flags.b.run) {
#if defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
    if (!(pp->flags.b.plc)) {
      sprintf(set, "rt_prio -rp %d %d", pp->proc.p_prio, pp->proc.pid);
      system(set);
    }
#endif 
  }
}

void ini_ProcIter (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  int		mask,
  void		(*func)(pwr_tStatus*, ini_sContext*, ini_sProc*)
)
{
  lst_sEntry	*pl;
  ini_sProc	*pp;

  pwr_dStatus(sts, status, INI__SUCCESS);

  for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
    if (pp->proc.flags.m & mask)
      func(sts, cp, pp);
  }
}

void ini_ProcTable (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  FILE		*f;
  ini_sProc	*pp;
  char		*s;
  char		buffer[256];

  pwr_dStatus(sts, status, INI__SUCCESS);

  pp = ini_ProcInsert(sts, cp, "pwr_qmon", "pwr_qmon_%d", 0, 1, "rt_qmon", cPrio_qmon, 0, "-n");
  pp->flags.b.qmon = 1;
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_sev_server", "pwr_sev_server_%d", 0, 1, "sev_server", cPrio_sev_server, 0, "-n");
  pp->proc.flags.b.system = 1;

  
  f = ini_OpenFile(sts, cp, &cp->applfile);
  if (f != NULL) {    
    if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "Reading Application file %s\n", cp->applfile.name);
    for (;;) {
      char *nl;

      s = fgets(buffer, sizeof(buffer) - 1, f);
      if (s == NULL) break;
      nl = strchr(s, '\n');
      if (nl != NULL) *nl = '\0';

      if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "   %s", buffer);
      if (buffer[0] == '#') continue;
	
      do {
	int i_load = - 1;
	int i_run = - 1;
	int i_debug = -1;
	int i_prio = -1;
	char *id = NULL;
	char *name = NULL;
	char *load = NULL;
	char *run = NULL;
	char *file = NULL;
	char *prio = NULL;
	char *debug = NULL;
	char *arg = NULL;

	id	= strtok(s, ",");	if (id == NULL) break;
	name	= strtok(NULL, ",");	if (name == NULL) break;
	load	= strtok(NULL, ",");	if (load == NULL) break;
	run	= strtok(NULL, ",");	if (run == NULL) break;
	file	= strtok(NULL, ",");	if (file == NULL) break;
	prio	= strtok(NULL, ",");	if (prio == NULL) break;
	debug	= strtok(NULL, ",");	if (debug == NULL) break;
	arg	= strtok(NULL, ",");	if (arg == NULL) break;

	while (isspace(*id)) id++;
	while (isspace(*name)) name++;
	while (isspace(*load)) load++;
	while (isspace(*run)) run++;
	while (isspace(*file)) file++;
	while (isspace(*prio)) prio++;
	while (isspace(*debug)) debug++;
	while (isspace(*arg)) arg++;

	if (id[0] == '\0') break;
	if (strstr(load, "no"))
	  i_load = 0;
	else if (strstr(load, "load"))
	  i_load = 1;

	if (strstr(run, "no"))
	  i_run = 0;
	else if (strstr(run, "run"))
	  i_run = 1;

	if (strstr(debug, "no"))
	  i_debug = 0;
	else if (strstr(debug, "debug"))
	  i_debug = 1;

	if (strcspn(prio, "0123456789") > 0)
	  i_prio = -1;
	else
	  i_prio = atoi(prio);

	pp = ini_ProcInsert(sts, cp, id, name, i_load, i_run, file, i_prio, i_debug, arg);
	if (!pp->proc.flags.b.system && !pp->proc.flags.b.base)
	  pp->proc.flags.b.user = 1;
      } while (0);

    }
    fclose(f);
  }
}

void
ini_ProcWait (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  lst_sEntry	*pl;
  ini_sProc	*pp;
  pid_t		pid;
  pid_t		last_pid = 1;
  qcom_sGet get;
  int tmo = 1000;

  pwr_dStatus(sts, status, INI__SUCCESS);

#if defined OS_LYNX || defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD
  for (;;) {
    int status;

    get.data = NULL;
    qcom_Get(sts, &cp->eventQ, &get, tmo);
    if (*sts == QCOM__TMO || *sts == QCOM__QEMPTY) {
      errh_Info("Timeout");
    } else if (get.data == NULL) {
      errh_Info("Nulldata");
    } else {
      errh_Info("%d %s", get.size, get.data);
      qcom_Free(NULL, get.data);
    }
    if (lst_Succ(NULL, &cp->proc_lh, &pl) == NULL) break;
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    if (pid == 0) continue;
    if (pid == last_pid) break;
    
    for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
      if (pp->proc.pid == pid) {
	errh_Info("Process %s exited with status %d", pp->proc.name, status);
	break;
      }
    }
  }
#else
  sleep(100000);
#endif
  errh_Info("Ich sterbe!!");
}







