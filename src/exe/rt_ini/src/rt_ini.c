#include <stdio.h>
#if defined OS_VMS
# include "co_getopt.h"
#elif defined OS_LYNX
  extern int getopt();
# include <sys/wait.h>
#elif defined OS_LINUX
  extern int getopt();
# include <sys/wait.h>
# include <fcntl.h>
#else
# include <unistd.h>
#endif
#include "co_ver.h"
#include "co_time.h"
#include "co_dcli.h"
#include "ini.h"
#include "ini_rc.h"
#include "rt_ini_alias.h"
#include "rt_ini_event.h"
#include "rt_bck_load.h"
#include "rt_errl.h"
#include "rt_errh.h"
#include "rt_mh_util.h"
#include "rt_qini.h"
#include "rt_qcom.h"
#include "rt_io_base.h"
#include "rt_ini_msg.h"
#include "rt_errh_msg.h"
#include "rt_pwr_msg.h"

static ini_sContext	*createContext (int argc, char **argv);
static int		checkErrors (ini_sContext*);
static pwr_tStatus	events (ini_sContext *cp);
static pwr_tStatus	interactive (int argc, char **argv, ini_sContext *cp);
static pwr_tStatus	stop (int argc, char **argv, ini_sContext *cp);
static void		load_backup ();
static void		logChanges (ini_sContext*);
static void		logCardinality (ini_sContext*);
static pwr_tStatus	restart (ini_sContext *cp);
static pwr_tStatus	terminate (ini_sContext *cp);
static pwr_tStatus	start (ini_sContext *cp);
static void		usage (char*);
static void             ini_errl_cb( void *userdata, char *str, char severity, 
				     pwr_tStatus sts, int anix, int message_type);


void set_valid_time()
{
  /* Set valid utc time */
#if defined(OS_ELN)
  pwr_tTime current_time;
  int sts;

  sys$gettim( &current_time);
  if ( current_time.tv_nsec < 10400000)
  {
    current_time.tv_nsec = 10400000;
    ker$set_time( &sts, &current_time);
  }
#endif
}


int main (int argc, char **argv)
{
  ini_sContext *cp;
  pwr_tStatus sts;

  set_valid_time();

  cp = createContext(argc, argv);

  ver_WriteVersionInfo("PROVIEW/R Process Environment");

  if (cp->flags.b.restart) {
    sts = interactive(argc, argv, cp);
  } else if (cp->flags.b.stop) {
    sts = stop(argc, argv, cp);
  } else {
    sts = start(cp);
    sts = events(cp);
    errh_LogInfo(&cp->log, "Ich sterbe!!");
  }

  exit(sts);
}

static pwr_tStatus
start (
  ini_sContext *cp
)
{
  pwr_tStatus sts;
  char console[80];

#if defined OS_LYNX || defined OS_LINUX
  int	fd;

  if ( strcmp( cp->console, "") == 0)
    strcpy( console, "/dev/console");
  else
    strcpy( console, cp->console);
  if ((fd = open(console, O_APPEND | O_WRONLY)) == -1)
    errl_Init(NULL, ini_errl_cb, cp);
  else {
    close(fd);
    errl_Init(console, ini_errl_cb, cp);
  }
#else
  errl_Init("CONSOLE:", ini_errl_cb, cp);
#endif

  errh_Init("pwr_ini", errh_eAnix_ini);

  if ( cp->flags.b.interactive)    
    errh_Interactive();

  mh_UtilCreateEvent();

  ini_CheckContext(&sts, cp);

  ini_ReadBootFile(&sts, cp);
  ini_ReadNodeFile(&sts, cp);
  ini_CheckNode(&sts, cp);

  cp->me = tree_Find(&sts, cp->nid_t, &cp->node.nid);
  if (cp->me == NULL) {
    errh_LogFatal(&cp->log, "Cannot find my own node in %s\n", cp->nodefile.name);
    exit(QCOM__WEIRD);
  }

  if (!checkErrors(cp))
    exit(0);

  if (cp->flags.b.verbose)
    logCardinality(cp);

  ini_CreateDb(&sts, cp);

  ini_LoadNode(&sts, cp);

  ini_BuildNode(&sts, cp);

  if (cp->np != NULL) {
    if (cp->np->ErrLogTerm[0] != '\0') {
      errh_LogInfo(&cp->log, "Setting log terminal to: %s", cp->np->ErrLogTerm);
      errl_SetTerm(cp->np->ErrLogTerm);
    }

/* Logfile is always $pwrp_log/pwr.log from V4.0.0 and handled by Linux log rotation */ 

    char fname[256];
    sprintf(fname, "$pwrp_log/pwr_%s.log", cp->nodename);
    dcli_translate_filename(fname, fname);
    errl_SetFile(fname);
    errh_LogInfo(&cp->log, "Setting log file to: %s", fname);

/*
    if (cp->np->ErrLogFile[0] != '\0') {
      struct tm *tp;
      char fname[256];
      time_t t;

      time(&t);
      tp = localtime(&t);
      strftime(fname, sizeof(fname), cp->np->ErrLogFile, tp );
      dcli_translate_filename( fname, fname);
      errl_SetFile(fname);
      errh_LogInfo(&cp->log, "Setting log file to: %s", cp->np->ErrLogFile);
    }
*/
  }

  ini_SetSystemStatus( cp, PWR__STARTUP);
  errh_SetStatus( PWR__STARTUP);

  sts = ini_RcReadAndSet(cp->dir, cp->nodename, cp->busid);
  if (EVEN(sts))
    errh_LogError(&cp->log, "ini_RcReadAndSet, %m", sts);

  sts = ini_SetAttribute(cp->aliasfile.name, cp->nodename, 0);
  if (EVEN(sts))
    errh_LogError(&cp->log, "ini_SetAttribute, %m", sts);

  qini_BuildDb(&sts, cp->nid_t, cp->me, NULL, cp->busid);

  load_backup();

  io_init_signals();

#if defined OS_ELN
  ker$initialization_done(NULL);
#endif

  ini_ProcTable(&sts, cp);
  ini_ProcIter(&sts, cp, proc_mProcess_system, ini_ProcLoad);
  ini_ProcIter(&sts, cp, proc_mProcess_system, ini_ProcStart);
  ini_ProcIter(&sts, cp, proc_mProcess_system, ini_ProcPrio);

  net_Connect(&sts, &gdbroot->my_aid, &gdbroot->my_qid, NULL, "pwr_ini");
  /*if (!qcom_Init(&sts, 0)) {*/
  if (EVEN(sts)) {
    errh_LogFatal(&cp->log, "net_Connect, %m", sts);
    exit(sts);
  }   

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_newPlcInit | ini_mEvent_newPlcStart);

  ini_ProcIter(&sts, cp, proc_mProcess_user, ini_ProcLoad);
  ini_ProcIter(&sts, cp, proc_mProcess_user, ini_ProcStart);
  ini_ProcIter(&sts, cp, proc_mProcess_user, ini_ProcPrio);

  qcom_CreateQ(&sts, &cp->eventQ, NULL, "iniEvent");
  if (EVEN(sts)) {
    errh_LogFatal(&cp->log, "qcom_CreateQ, %m", sts);
    exit(sts);
  }

  qcom_WaitAnd(&sts, &cp->eventQ, &qcom_cQini, ini_mEvent_newPlcStartDone, qcom_cTmoEternal);

  sts = ini_SetAttributeAfterPlc(cp->aliasfile.name, cp->nodename, 0);
  if (EVEN(sts) && sts != 0)
    errh_LogError(&cp->log, "ini_SetAttributeAfterPlc, %m", sts);
         
  ini_SetSystemStatus( cp, PWR__RUNNING);
  errh_SetStatus( PWR__SRUN);

  return sts;
}

static pwr_tStatus
interactive (
  int		argc,
  char		**argv,
  ini_sContext	*cp
)
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qcom_sPut	put;
  qcom_sGet	get;
  char		*bp, *sp;
  int		i;
  int		len;
  int		totlen;

  errh_Interactive();

  if (!qcom_Init(&sts, 0, "pwr_ini_restart")) {
    errh_LogFatal(&cp->log, "qcom_Init, %m", sts);
    exit(sts);
  } 

  qcom_CreateQ(&sts, &cp->myQ, NULL, "pwr_ini_restart");
  if (EVEN(sts)) {
    errh_LogFatal(&cp->log, "qcom_CreateQ, %m", sts);
    exit(sts);
  }

  for (i = 0, totlen = 0; i < argc; i++) {
    len = strlen(argv[i]);
    totlen += 1 + len;
    errh_LogInfo(&cp->log, "argv[%d]: %d \"%s\"", i, len, argv[i]);
  }
  bp = malloc(totlen);
  for (i = 0, sp = bp; i < argc; i++) {
    len = strlen(argv[i]);
    memcpy(sp, argv[i], len + 1);
    sp += len + 1;
  }
  
  qid.qix = 550715;
  qid.nid = 0;
    put.type.b = 10;
    put.type.s = 1;
    put.reply = cp->myQ;
    put.data = bp;
    put.size = totlen;
  qcom_Put(&sts, &qid, &put);

  while (1) {
    char *s;

    get.data = NULL;
    s = qcom_Get(&sts, &cp->myQ, &get, 100000);
    if (sts == QCOM__TMO && sts == QCOM__QEMPTY) {
      break;
    } else if (s != NULL) {
      printf("%s\n", s);
      qcom_Free(NULL, s);
    }
    if (get.type.s == 2)
      break;
  }

  return sts;
}

static pwr_tStatus
stop (
  int		argc,
  char		**argv,
  ini_sContext	*cp
)
{
  pwr_tStatus	sts;
  qcom_sQid	qid;
  qcom_sPut	put;
  char          data[] = "Shutdown you fool!";
/*  qcom_sGet	get;
  char		*bp, *sp;
  int		i;
  int		len;
  int		totlen; */

//  errh_Interactive();

  if (!qcom_Init(&sts, 0, "pwr_ini_stop")) {
//    errh_LogFatal(&cp->log, "qcom_Init, %m", sts);
    exit(sts);
  } 

//  qcom_CreateQ(&sts, &cp->myQ, NULL, "pwr_ini_restart");
//  if (EVEN(sts)) {
//    errh_LogFatal(&cp->log, "qcom_CreateQ, %m", sts);
//    exit(sts);
//  }

//  for (i = 0, totlen = 0; i < argc; i++) {
//    len = strlen(argv[i]);
//    totlen += 1 + len;
//    errh_LogInfo(&cp->log, "argv[%d]: %d \"%s\"", i, len, argv[i]);
//  }
//  bp = malloc(totlen);
//  for (i = 0, sp = bp; i < argc; i++) {
//    len = strlen(argv[i]);
//    memcpy(sp, argv[i], len + 1);
//    sp += len + 1;
// }
  
  qid.qix = 550715;
  qid.nid = 0;
    put.type.b = 11;
    put.type.s = 1;
    put.reply.qix = 0;
    put.reply.nid = 0;
    put.data = data;
    put.size = sizeof(data) + 1;
  qcom_Put(&sts, &qid, &put);

//  while (1) {
//    char *s;

//    get.data = NULL;
//    s = qcom_Get(&sts, &cp->myQ, &get, 100000);
//    if (sts == QCOM__TMO && sts == QCOM__QEMPTY) {
//      break;
//    } else if (s != NULL) {
//      printf("%s\n", s);
//      qcom_Free(NULL, s);
//    }
//    if (get.type.s == 2)
//      break;
//  }

  return 0;
}

static pwr_tStatus
restart (
  ini_sContext	*cp
)
{
  pwr_tStatus	sts;

  ini_CheckContext(&sts, cp);

  ini_ReadBootFile(&sts, cp);
  ini_CheckNode(&sts, cp);

  checkErrors(cp);

  if (cp->flags.b.verbose)
    logCardinality(cp);

  qcom_SignalAnd(&sts, &qcom_cQini, 0);
  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_swapInit);

  ini_ReloadNode(&sts, cp);

  if (cp->flags.b.verbose)
    logChanges(cp);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_rebuildInit);
  ini_RebuildNode(&sts, cp);
  ini_DecodeBodies(&sts, cp, 0);
  ini_DecodeBodies(&sts, cp, 1);

  if (cp->flags.b.verbose)
    errh_LogInfo(&cp->log, "Update bodies of io objects");
  ini_UpdateBodies(&sts, cp, 1);
  io_init_signals();

  ini_ProcTable(&sts, cp);

  ini_ProcLoad(&sts, cp, cp->plc);
  ini_ProcStart(&sts, cp, cp->plc);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_newPlcInit);
  qcom_WaitAnd(&sts, &cp->eventQ, &qcom_cQini, ini_mEvent_newPlcInitDone, qcom_cTmoEternal);
  errh_LogInfo(&cp->log, "Entering time critical period, stopping old PLC");
  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_oldPlcStop);
  qcom_WaitAnd(&sts, &cp->eventQ, &qcom_cQini, ini_mEvent_oldPlcStopDone, qcom_cTmoEternal);
  qcom_SignalAnd(&sts, &qcom_cQini, ~ini_mEvent_oldPlcStop);

  ini_UpdateBodies(&sts, cp, 0);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_newPlcStart);
  qcom_WaitAnd(&sts, &cp->eventQ, &qcom_cQini, ini_mEvent_newPlcStartDone, qcom_cTmoEternal);
  errh_LogInfo(&cp->log, "Time critical period over, new PLC is running");
  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_swapDone);
  
  if (cp->PlcProcess != NULL) {
    char time[24];

    clock_gettime(CLOCK_REALTIME, &cp->PlcProcess->LastChgTime);
    time_Dsub(&cp->PlcProcess->StallTime, &cp->PlcProcess->StartTime, &cp->PlcProcess->StopTime);
    time_DtoAscii(&cp->PlcProcess->StallTime, 1, time, sizeof(time));
    cp->log.put.type.s = 2;
    errh_LogInfo(&cp->log, "IO stall time: %s", time);
    cp->np->RestartStallTime = cp->PlcProcess->StallTime;
  }

  ini_FreeBodies(&sts, cp, 0);
  ini_FreeBodies(&sts, cp, 1);

  return sts;
}

static pwr_tStatus
terminate (
  ini_sContext	*cp
)
{
  pwr_tStatus	sts;

  qcom_SignalAnd(&sts, &qcom_cQini, 0);
  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_terminate);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_oldPlcStop);
  qcom_WaitAnd(&sts, &cp->eventQ, &qcom_cQini, ini_mEvent_oldPlcStopDone, qcom_cTmoEternal);

  qcom_Exit(NULL);

  /* Kill programs which until now doesn't handle qcom */
/*  
  for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
    if (strncmp("pwr_webmonmh", pp->proc.name, 12) == 0) {
      kill(pp->proc.pid, SIGKILL);
    }
    else if (strncmp("pwr_webmon", pp->proc.name, 10) == 0) {
      kill(pp->proc.pid, SIGKILL);
    }
  }
*/  
  /* Now sleep for a while */
  
  sleep(3);
  
  /* Unlink shared memory and semaphores */
  
  gdb_UnlinkDb();
  qdb_UnlinkDb();

  /* Destroy message handler semaphore */
  
  mh_UtilDestroyEvent();

  #if defined(OS_LINUX)   
    /* Unlink errlog mwessage queue */
    errl_Unlink();
  #endif

  exit(1);
}

static int
ask_yes_no (
  ini_sContext	*cp,
  char *text
)
{

  printf("%s ? (y|n) [n]: ", text);
  printf("n\n");

  return 0;
}

static int
checkErrors (
  ini_sContext	*cp
)
{

  if (cp->warnings == 0 && cp->errors == 0 && cp->fatals == 0)
    return 1;

  if (cp->fatals > 0) {
    errh_LogFatal(&cp->log, "Found %d warning(s), %d error(s) and %d fatal error(s)", cp->warnings, cp->errors, cp->fatals);
    if (cp->flags.b.ignoreFatal) {
      errh_LogInfo(&cp->log, "Ignoring fatal errors, errors and warnings, continued...");
      return 1;
    } else {
      return ask_yes_no(cp, "Do you want to continue");
    }
  }
  if (cp->errors > 0) {
    errh_LogError(&cp->log, "Found %d warning(s), %d error(s) and %d fatal error(s)", cp->warnings, cp->errors, cp->fatals);
    if (cp->flags.b.ignoreError) {
      errh_LogInfo(&cp->log, "Ignoring errors and warnings, continued...");
      return 1;
    } else {
      return ask_yes_no(cp, "Do you want to continue");
    }
  }
  if (cp->warnings > 0) {
    errh_LogWarning(&cp->log, "Found %d warning(s), %d error(s) and %d fatal error(s)", cp->warnings, cp->errors, cp->fatals);
    if (cp->flags.b.ignoreWarning) {
      errh_LogInfo(&cp->log, "Ignoring warnings, continued...");
      return 1;
    } else {
      return ask_yes_no(cp, "Do you want to continue");
    }
  }
  return 1;
}

static ini_sContext *
createContext (int argc, char **argv)
{
  int c;
  extern char *optarg;
  extern int optind;
#if 1
  extern int optind;
#endif
  ini_sContext *cp;
  pwr_tStatus sts;
#if defined(OS_LYNX) || defined(OS_LINUX)
  char *options = "a:b:c:d:efg:hin:p:q:rsu:vwA:H:V";
#else
  char *options = "a:b:d:efhin:p:q:rvwA:H:V";
#endif

#if 0
  extern int opterr;
  opterr = 0;
#endif
  optind = 0;

  if (!(cp = ini_CreateContext(&sts))) {
    fprintf(stderr, "%s: could not allocate context\n", argv[0]);
    exit(1);
  }

  while ((c = getopt(argc, argv, options)) != -1) {
    switch (c) {
    case 'a':
      cp->flags.b.applfile = 1;
      strcpy(cp->applfile.name, optarg);
      break;
    case 'b':
      cp->flags.b.bootfile = 1;
      strcpy(cp->bootfile.name, optarg);
      break;
    case 'c':
      strcpy(cp->console, optarg);
      break;
    case 'd':
      strcpy(cp->dir, optarg);
      break;
    case 'e':
      cp->flags.b.ignoreError = 1;
      break;
    case 'f':
      cp->flags.b.ignoreFatal = 1;
      break;
    case 'h':
      cp->flags.b.hostname = 1;
      strcpy(cp->hostname, optarg);
      break;
    case 'i':
      cp->flags.b.interactive = 1;
      break;
    case 'n':
      cp->flags.b.nodename = 1;
      strcpy(cp->nodename, optarg);
      break;
    case 'p':
      cp->flags.b.plcfile = 1;
      strcpy(cp->plcfile.name, optarg);
      break;
    case 'q':
      cp->flags.b.busid = 1;
      cp->busid =  atoi(optarg);
      break;
    case 'r':
      cp->flags.b.restart = 1;
      cp->flags.b.interactive = 1;
      break;
    case 's':
      cp->flags.b.stop = 1;
      break;
    case 'v':
      cp->flags.b.verbose = 1;
      break;
    case 'w':
      cp->flags.b.ignoreWarning = 1;
      break;
    case 'A':
      cp->flags.b.aliasfile = 1;
      strcpy(cp->aliasfile.name, optarg);
      break;
    case '?':
      usage(argv[0]);
      break;
    }
  }

  return cp;
}
static void
usage (
  char *name
)
{
#if defined(OS_LYNX) || defined(OS_LINUX)
  fprintf(stderr, "usage: %s -a arg -b arg -d arg -efg arg -hip arg -q arg -ru arg -s arg -vwA arg -H arg\n", name);
#else
  fprintf(stderr, "usage: %s -a arg -b arg -d arg -efhip arg -q arg -rvwA arg -H arg\n", name);
#endif
  fprintf(stderr, "  -?    : give help\n");
  fprintf(stderr, "  -a arg: use 'arg' as application file\n");
  fprintf(stderr, "  -b arg: use 'arg' as boot file\n");
  fprintf(stderr, "  -d arg: use files from directory 'arg'\n");
  fprintf(stderr, "  -e    : ignore errors\n");
  fprintf(stderr, "  -f    : ignore fatal errors\n");
#if defined(OS_LYNX) || defined(OS_LINUX)
  fprintf(stderr, "  -g arg: setgid to 'arg' before starting\n");
#endif
  fprintf(stderr, "  -h    : give help\n");
  fprintf(stderr, "  -i    : interactive, log to stdout\n");
  fprintf(stderr, "  -p arg: use 'arg' as PLC\n");
  fprintf(stderr, "  -q arg: use 'arg' as qcom bus id\n");
  fprintf(stderr, "  -r    : restart with new versions of loadfiles and PLC\n");
#if defined(OS_LYNX) || defined(OS_LINUX)
  fprintf(stderr, "  -s    : stop of Proview/R\n");
  fprintf(stderr, "  -u arg: setuid to 'arg' before starting\n");
#endif
  fprintf(stderr, "  -v    : verbose\n");
  fprintf(stderr, "  -w    : ignore warnings\n");
  fprintf(stderr, "  -A arg: use 'arg' as alias file\n");
  fprintf(stderr, "  -H arg: use 'arg' as hostname\n");
  fprintf(stderr, "  -N arg: use 'arg' as nodename\n");
  exit(1);
}

static void
logChanges (
  ini_sContext *cp
)
{
  lst_sEntry *vl;
  lst_sEntry *ol;
  ivol_sVolume *vp;
  ivol_sObject *iop;
  int cre, upd, upd_io;

  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
    cre = upd = upd_io = 0;
    for (iop = lst_Succ(NULL, &vp->cre_lh, &ol); iop != NULL; iop = lst_Succ(NULL, ol, &ol)) {
      errh_LogInfo(&cp->log, "cre: %s\n", iop->oh.name);
      cre++;
    }
    for (iop = lst_Succ(NULL, &vp->upd_io_lh, &ol); iop != NULL; iop = lst_Succ(NULL, ol, &ol)) {
      printf("upd: %s, (", iop->op->g.f.name.orig);
      if (iop->flags.b.father) printf("father, ");
      if (iop->flags.b.name) printf("name, ");
      if (iop->flags.b.server) printf("server, ");
      if (iop->flags.b.classid) printf("class, ");
      if (iop->flags.b.size) printf("size, ");
      if (iop->flags.b.flags) printf("flags, ");
      if (iop->flags.b.body) printf("body, ");
      printf(")\n");
      upd_io++;
    }
    for (iop = lst_Succ(NULL, &vp->upd_lh, &ol); iop != NULL; iop = lst_Succ(NULL, ol, &ol)) {
      printf("upd: %s, (", iop->op->g.f.name.orig);
      if (iop->flags.b.father) printf("father, ");
      if (iop->flags.b.name) printf("name, ");
      if (iop->flags.b.server) printf("server, ");
      if (iop->flags.b.classid) printf("class, ");
      if (iop->flags.b.size) printf("size, ");
      if (iop->flags.b.flags) printf("flags, ");
      if (iop->flags.b.body) printf("body, ");
      printf(")\n");
      upd++;
    }
    errh_LogInfo(&cp->log, "Volume: %s, id: %d, cre: %d, upd_io: %d, upd: %d\n", vp->volume.name, vp->vid, cre, upd_io, upd);
  }
}

static void
logCardinality (
  ini_sContext *cp
)
{
  lst_sEntry *vl;
  ivol_sVolume *vp;

  errh_LogInfo(&cp->log, "Cardinality: %d, bodysize: %d\n", cp->node.cardinality, cp->node.bodySize);  
  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl))
    errh_LogInfo(&cp->log, "Volume: %s, id: %d, cardinality: %d, bodysize: %d\n", vp->volume.name, vp->vid,
      vp->volume.cardinality, vp->volume.rbodySize);
}

static pwr_tStatus
events (
  ini_sContext	*cp
)
{
  lst_sEntry	*pl;
  ini_sProc	*pp;
  pid_t		pid;
  pid_t		last_pid = 1;
  pwr_tStatus	sts = INI__SUCCESS;
  qcom_sGet	get;
#if defined(OS_LYNX) || defined(OS_LINUX)
  int		tmo_ms = 1000;
#else
  int		tmo_ms = qcom_cTmoEternal;
#endif

  cp->myQ.qix = 550715;
  cp->myQ.nid = 0;

  qcom_CreateQ(&sts, &cp->myQ, NULL, "events");
  if (EVEN(sts)) {
    errh_LogFatal(&cp->log, "qcom_CreateQ, %m", sts);
    exit(sts);
  }

  for (;;) {
    int status;

    get.data = NULL;
    qcom_Get(&sts, &cp->myQ, &get, tmo_ms);
    
    /* Request for termination ?? */
    if (sts != QCOM__TMO && sts != QCOM__QEMPTY && get.type.b == 11) {
      sts = terminate(cp);
      return sts;
    }
    
    /* Request for restart */
    if (sts != QCOM__TMO && sts != QCOM__QEMPTY && get.data != NULL) {
      int len, i, argc, totlen;
      char **argv, *s;
      ini_sContext *ncp;

      for (argc = 0, s = get.data, totlen = 0; totlen < get.size; argc++) {
	len = strlen(s);
	s += len + 1;
	totlen += len + 1;
      }
      argv = (char **) calloc(sizeof(void*), argc);
      for (i = 0, s = get.data; i < argc; i++) {
	len = strlen(s);
	argv[i] = s;
	s += len + 1;
      }
      ncp = createContext(argc, argv);
      if (ncp != NULL) {
	ncp->log.put.type.b = 10;
	ncp->log.put.type.s = 1;
	ncp->log.send = 1;
	ncp->log.put.reply = cp->myQ;
	ncp->log.logQ = get.reply;
	ncp->eventQ = cp->eventQ;
	ncp->yourQ = get.reply;
	restart(ncp);
	free(ncp);
      }
      free(argv);
      qcom_Free(NULL, get.data);
    }

#if defined(OS_LYNX) || defined(OS_LINUX)
    if (lst_Succ(NULL, &cp->proc_lh, &pl) == NULL) break;
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    if (pid == 0) continue;
    if (pid == last_pid) break;
    
    for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
      if (pp->proc.pid == pid) {
	errh_LogInfo(&cp->log, "Process %s exited with status %d", pp->proc.name, status);
	break;
      }
    }
#endif

  }
  return INI__SUCCESS;
}

static void
load_backup ()
{
  pwr_tObjid			oid;
  pwr_sClass_AvArea		*avp;
  pwr_sClass_DvArea		*dvp;
  pwr_sClass_IvArea		*ivp;
  pwr_sClass_IvArea		*iavp;
  pwr_sClass_IvArea		*idvp;
  pwr_sClass_IvArea		*iivp;
  pwr_tStatus			sts;
  int				i;
  pwr_sClass_IOHandler		*iop;
 
  sts = io_get_iohandler_object(&iop, NULL);  
  if (EVEN(sts)) {
    errh_Error("io_get_iohandler_object, %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-av", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-av, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &avp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &avp), %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-av_init", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-av_init, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &iavp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &iavp), %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-dv", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-dv, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &dvp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &dvp), %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-dv_init", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-dv_init, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &idvp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &idvp), %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-iv", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-iv, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &ivp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &ivp), %m", sts);
    return;
  }

  sts = gdh_NameToObjid("pwrNode-active-io-iv_init", &oid);
  if (EVEN(sts)) {
    errh_Error("gdh_NameToObjid(pwrNode-active-io-iv_init, &oid), %m", sts);
    return;
  }

  sts = gdh_ObjidToPointer(oid, (void *) &iivp);
  if (EVEN(sts)) {
    errh_Error("gdh_ObjidToPointer(oid, (void *) &iivp), %m", sts);
    return;
  }

  for (i = 0; i < iop->AvCount; i++) {
    pwr_tFloat32 *ifp = gdh_TranslateRtdbPointer(iavp->Value[i]);
    avp->Value[i]= *ifp;
  }

  for (i = 0; i < iop->DvCount; i++) {
    pwr_tBoolean *ibp = gdh_TranslateRtdbPointer(idvp->Value[i]);
    dvp->Value[i] = *ibp;
  }

  for (i = 0; i < iop->IvCount; i++) {
    pwr_tInt32 *iip = gdh_TranslateRtdbPointer(iivp->Value[i]);
    ivp->Value[i] = *iip;
  }

  sts = bck_LoadBackup();
  if (EVEN(sts)) {
    errh_Error("bck_LoadBackup, %m", sts);
    return;
  }

  for (i = 0; i < iop->AvCount; i++) {
    pwr_tFloat32 *ifp = gdh_TranslateRtdbPointer(iavp->Value[i]);
    *ifp = avp->Value[i];
  }

  for (i = 0; i < iop->DvCount; i++) {
    pwr_tBoolean *ibp = gdh_TranslateRtdbPointer(idvp->Value[i]);
    *ibp = dvp->Value[i];
  }

  for (i = 0; i < iop->IvCount; i++) {
    pwr_tInt32 *iip = gdh_TranslateRtdbPointer(iivp->Value[i]);
    *iip = ivp->Value[i];
  }
}

static void ini_errl_cb( void *userdata, char *str, char severity, pwr_tStatus sts, int anix,
			 int message_type)
{
  ini_sContext *cp = (ini_sContext *) userdata;

  if ( anix == 0 || !cp->np) return;
  if ( anix >= sizeof(cp->np->ProcStatus)/sizeof(cp->np->ProcStatus[0])) {
    printf ( "Init: undefined anix %d\n", anix);
    return;
  }

  switch ( message_type) {
  case errh_eMsgType_Log:
    if ( sts == 0) {
      switch ( severity) {
      case 'S':
      case 'I':
	sts = PWR__SRVINFO;
	break;
      case 'E':
	sts = PWR__SRVERROR;
	break;
      case 'W':
	sts = PWR__SRVWARNING;
	break;
      case 'F':
	sts = PWR__SRVFATAL;
	break;
      }
    }
    cp->np->ProcMsgSeverity[anix-1] = sts;
    strncpy( cp->np->ProcMessage[anix-1], &str[49], sizeof(cp->np->ProcMessage[0]));
    cp->np->ProcMessage[anix-1][sizeof(cp->np->ProcMessage[0])-1] = 0;
    break;
  case errh_eMsgType_Status:
    cp->np->ProcStatus[anix-1] = sts;
    break;
  }
}






