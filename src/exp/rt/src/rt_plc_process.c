/* rt_plc_process.c -- <short description>

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   Runs the PLC programs of a node.  */

#if defined(OS_ELN)
# include $vaxelnc
# include $kernelmsg
# include $kerneldef
# include descrip
#elif defined(OS_VMS)
#elif defined(OS_LYNX)
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_qdb.h"
#include "rt_c_plcthread.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_proc.h"
#include "rt_thread.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_ini_msg.h"
#include "rt_plc_msg.h"
#include "rt_plc_rt.h"
#include "rt_plc.h"
#include "rt_plc_proc.h"
#include "rt_qcom.h"
#include "rt_que.h"
#include "rt_csup.h"
#include "rt_ini_event.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"

static plc_sProcess	*init_process ();
static pwr_tStatus	init_plc (plc_sProcess*);
static void		init_threads (plc_sProcess*);
static void		start_threads (plc_sProcess*);
static void		stop_threads (plc_sProcess*);
static void		clean_all (plc_sProcess*);
static void		link_io_base_areas (plc_sProcess*);
static void		link_io_copy_areas (plc_sThread*);
static void		dlink_area (plc_sDlink*, char*, int);
static void		create_threads (plc_sProcess*);
static void		create_thread (plc_sThread*, plc_sProctbl*, plc_sProcess*);
static void		init_grafcet ();
static void		save_values (plc_sProcess*);
static void		set_values (plc_sProcess*);
extern void		plc_thread();



/* When you use pthread_cond_timedwait is the shortest timeout always > 1 CLK_TCK
 * So if you want to run a PLC-program with the same frequency as the scheduler
 * you need to use a rt-timer. In this version there is no slip detection if
 * you use a rt-timer. You need to set USE_RT_TIMER to 1 in rt_plc_thread.c
 * and rt_plc_process.c
 */
#define USE_RT_TIMER 0

#if defined OS_LYNX && USE_RT_TIMER
static void signal_handler (int sig, siginfo_t *info, void *v)
{
  int 		i;
  plc_sThread	*tp;
  plc_sProcess 	*pp = (plc_sProcess *) info->si_value.sival_ptr;

  if (sig != SIGRTMIN+1)
    return;

  for (i = 0, tp = pp->thread; i < pp->thread_count ; i++, tp++) {
    if (--(tp->IntervalCount) == 0) {
	sem_post(&tp->ScanSem);
	tp->IntervalCount = tp->ScanMultiple;
    }
  }

}

static int create_timer(plc_sProcess* pp)
{
  struct itimerspec	tmr_interval;
  struct sigaction 	act;
  struct sigevent	notification;
  timer_t 		timer;

  act.sa_sigaction = signal_handler;
  act.sa_flags = SA_SIGINFO;
  sigemptyset (&act.sa_mask);

  sigaction (SIGRTMIN+1, &act, NULL);

  notification.sigev_notify = SIGEV_SIGNAL;
  notification.sigev_signo = SIGRTMIN+1;
  notification.sigev_value.sival_ptr = (void *) pp;
  timer_create (CLOCK_REALTIME, &notification, &timer);

  tmr_interval.it_interval.tv_sec = (time_t) 0; 
  tmr_interval.it_interval.tv_nsec = 1000000000 / CLK_TCK;
  tmr_interval.it_value = tmr_interval.it_interval;
  timer_settime(timer, 0, &tmr_interval, NULL);

  return 0;
}
#endif




int main (
  int	argc,
  char	*argv[]
)
{
  pwr_tStatus	sts;
  plc_sProcess	*pp;

  pp = init_process();

  qcom_WaitAnd(&sts, &pp->eventQ, &qcom_cQini, ini_mEvent_newPlcInit, qcom_cTmoEternal);

  init_plc(pp);
  create_threads(pp);
  init_threads(pp);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_newPlcInitDone);
  qcom_WaitAnd(&sts, &pp->eventQ, &qcom_cQini, ini_mEvent_newPlcStart, qcom_cTmoEternal);

//  proc_SetPriority(pp->PlcProcess->Prio);
  set_values(pp);
  start_threads(pp);
  time_Uptime(&sts, &pp->PlcProcess->StartTime, NULL);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_newPlcStartDone);

#if 0
  /* Force the backup to take care initialized backup objects. */

  bck_ForceBackup(NULL);
#endif

  errh_SetStatus( PWR__SRUN);

  qcom_WaitAnd(&sts, &pp->eventQ, &qcom_cQini, ini_mEvent_oldPlcStop, qcom_cTmoEternal);

  errh_SetStatus( PWR__SRVTERM);

  time_Uptime(&sts, &pp->PlcProcess->StopTime, NULL);
  stop_threads(pp);
  save_values(pp);

  qcom_SignalOr(&sts, &qcom_cQini, ini_mEvent_oldPlcStopDone);

#if defined OS_ELN
  sts = proc_SetPriority(31);
#endif

  clean_all(pp);

  exit(0);
}

static plc_sProcess *
init_process ()
{
  plc_sProcess	*pp;
  pwr_tStatus	sts = PLC__SUCCESS;

#if 0
  thread_SetPrio(NULL, 15);
#endif

  errh_Init("pwr_plc", errh_eAnix_plc);
  errh_SetStatus( PWR__SRVSTARTUP);

  pp = (plc_sProcess *) calloc(1, sizeof(*pp));
  if (pp == NULL) {
    errh_Fatal("Out of virtual memory");
    exit(0);
  }

  sts = gdh_Init("pwr_plc");
  if (EVEN(sts)) {
    errh_Fatal("gdh_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

#if defined OS_VMS
  qdb->thread_lock.isThreaded = 1;
  qdb->thread_lock.cond_signal = thread_CondSignal;
  qdb->thread_lock.cond_wait = thread_CondWait;
#endif

  qcom_CreateQ(&sts, &pp->eventQ, NULL, "plcEvent");
  if (EVEN(sts)) {
    errh_Fatal("qcom_CreateQ(eventQ), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  sts = thread_MutexInit(&pp->io_copy_mutex);
  if (EVEN(sts)) {
    errh_Fatal("thread_MutexInit(io_copy_mutex), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  return pp;
}

static pwr_tStatus 
init_plc (
  plc_sProcess	*pp
)
{
  pwr_tStatus	sts = PLC__SUCCESS;
  pwr_tObjid   	oid;
  pwr_tObjid   	pp_oid;
  pwr_tObjid   	io_oid;
  pwr_tObjid	thread_oid;
  int		sec;
  int		msec;
  int		i;
  pwr_tCid	cid;

  sts = gdh_GetNodeObject(0, &oid);
  if (EVEN(sts)) {
    errh_Fatal("gdh_GetNodeObject, %m", sts);
    exit(sts);
  }

  sts = gdh_ObjidToPointer(oid, (void *)&pp->Node);
  if (EVEN(sts)) return sts;

  sts = gdh_GetClassList(pwr_cClass_PlcProcess, &pp_oid);
  if (EVEN(sts)) {
    errh_Error("Found no PlcProcess-object\n%m", sts);
    return sts;
  }

  sts = gdh_ObjidToPointer(pp_oid, (void *)&pp->PlcProcess);
  if (EVEN(sts)) return sts;

  i = 0;
  sts = gdh_GetChild( pp_oid, &thread_oid);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( thread_oid, &cid);
    if ( EVEN(sts)) return sts;
    
    if ( cid == pwr_cClass_PlcThread)
      pp->PlcProcess->PlcThreadObjects[i++] = thread_oid;

    sts = gdh_GetNextSibling( thread_oid, &thread_oid);
  }
  for ( ; i > sizeof(pp->PlcProcess->PlcThreadObjects)/sizeof(pp->PlcProcess->PlcThreadObjects[0]); i++)
    pp->PlcProcess->PlcThreadObjects[i] = pwr_cNObjid;

  aproc_RegisterObject( pp_oid);

  sts = gdh_GetClassList(pwr_cClass_IOHandler, &io_oid);
  if (EVEN(sts)) {
    errh_Error("Found no IOHandler-object\n%m", sts);
    return sts;
  }

  sts = gdh_ObjidToPointer(io_oid, (void *)&pp->IOHandler);
  if (EVEN(sts)) return sts;

  /* Set subscription defaults for PLC job */

  sts = gdh_SetSubscriptionDefaults(
    (pwr_tInt32)(pp->PlcProcess->SubscriptionInterval * 1000.), 10000);

  sec = pp->PlcProcess->SubscriptionInterval;
  msec = (int)((pp->PlcProcess->SubscriptionInterval - sec) * 1000.);
  errh_Info("Setting subscription defaults to %d.%03d seconds", sec, msec);

  sts = gdh_ObjidToName(oid, pp->nodeName, sizeof(pp->nodeName), cdh_mNName);
  if (EVEN(sts)) return sts;

  init_grafcet(pp);
  link_io_base_areas(pp);

  return sts;
}

static void
init_threads (
  plc_sProcess	*pp
)
{
  int i;
  plc_sThread *tp;
  pwr_tStatus sts;
  int phase;  

  for (i = 0, tp = pp->thread; i < pp->thread_count ; i++, tp++) {
    /* Tell thread it is time for phase 2.  */
    que_Put(&sts, &tp->q_in, &tp->event, (void *)2);
    phase = (int)que_Get(&sts, &tp->q_out, NULL, NULL);
    pwr_Assert(phase == 2);
  }
}

static void
start_threads (
  plc_sProcess	*pp
)
{
  int i;
  plc_sThread *tp;
  pwr_tStatus sts;
  int phase;

#if defined OS_LYNX && USE_RT_TIMER
  create_timer(pp);
#endif

  for (i = 0, tp = pp->thread; i < pp->thread_count ; i++, tp++) {
    /* Tell thread it is time for phase 3, run.  */
    que_Put(&sts, &tp->q_in, &tp->event, (void *)3);
    phase = (int)que_Get(&sts, &tp->q_out, NULL, NULL);
    pwr_Assert(phase == 3);
  }
}

/* Wait for ALL of the threads to finish. If a thread'
   CycleTime <> 0, then we assume that there in fact is an active
   thread at that index, and therefore we wait until the
   process terminates. When ALL threads have terminated,
   then we lower the job priority for this job. This is done
   to avoid competing with a new PLC job for the CPU resources
   while we clean up before exiting.  */

static void
stop_threads (
  plc_sProcess	*pp
)
{
  pwr_tInt32	i;
  pwr_tStatus	sts;
  plc_sThread	*tp;

  for (i = 0, tp = pp->thread; i < pp->thread_count ; i++, tp++) {
    tp->exit = TRUE;
    /* Tell thread it is time for phase 4, stop.  */

#if defined OS_LYNX && USE_RT_TIMER
    sem_post(&tp->ScanSem);   
#else
    que_Put(&sts, &tp->q_in, &tp->event, (void *)4);
#endif


#if 0
  pwr_tStatus	t_sts;
    sts = thread_Join(&tp->tid, &t_sts);
    if (EVEN(sts))
      errh_Error("thread_Join, thread %n, %m", tp->name, sts);
#endif
  }
}

static void
clean_all (
  plc_sProcess	*pp
)
{
  pwr_tStatus	sts;

  sts = gdh_UnrefObjectInfoAll();
  if (EVEN(sts))
    errh_Error("gdh_UnrefObjectInfoAll, %m", sts);

}

/* Link to I/O base areas.
   Create area if it doesn't exist.
   All threads write to base area.  */

static void
link_io_base_areas (
  plc_sProcess *pp
)
{

  dlink_area((plc_sDlink *)&pp->base.ai_a, "pwrNode-active-io-ai", pp->IOHandler->AiCount * sizeof(pwr_tFloat32));
  dlink_area((plc_sDlink *)&pp->base.ao_a, "pwrNode-active-io-ao", pp->IOHandler->AoCount * sizeof(pwr_tFloat32));
  dlink_area((plc_sDlink *)&pp->base.av_a, "pwrNode-active-io-av", pp->IOHandler->AvCount * sizeof(pwr_tFloat32));
  dlink_area((plc_sDlink *)&pp->base.ca_a, "pwrNode-active-io-ca", pp->IOHandler->CoCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.co_a, "pwrNode-active-io-co", pp->IOHandler->CoCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.di_a, "pwrNode-active-io-di", pp->IOHandler->DiCount * sizeof(pwr_tBoolean));
  dlink_area((plc_sDlink *)&pp->base.do_a, "pwrNode-active-io-do", pp->IOHandler->DoCount * sizeof(pwr_tBoolean));
  dlink_area((plc_sDlink *)&pp->base.dv_a, "pwrNode-active-io-dv", pp->IOHandler->DvCount * sizeof(pwr_tBoolean));
  dlink_area((plc_sDlink *)&pp->base.ii_a, "pwrNode-active-io-ii", pp->IOHandler->IiCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.io_a, "pwrNode-active-io-io", pp->IOHandler->IoCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.iv_a, "pwrNode-active-io-iv", pp->IOHandler->IvCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.av_i, "pwrNode-active-io-av_init", pp->IOHandler->AvCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.dv_i, "pwrNode-active-io-dv_init", pp->IOHandler->DvCount * sizeof(pwr_tInt32));
  dlink_area((plc_sDlink *)&pp->base.iv_i, "pwrNode-active-io-iv_init", pp->IOHandler->DvCount * sizeof(pwr_tInt32));
}

/* Link to I/O copy areas.
   Create area if it doesn't exist.
   All threads read from owned copy area.  */

static void
link_io_copy_areas (
  plc_sThread *tp
)
{
  plc_sProcess *pp = tp->pp;


  tp->copy.ai_a = pp->base.ai_a;
  tp->copy.ai_a.p = calloc(1, tp->copy.ai_a.size);

  tp->copy.ao_a = pp->base.ao_a;
  tp->copy.ao_a.p = calloc(1, tp->copy.ao_a.size);

  tp->copy.av_a = pp->base.av_a;
  tp->copy.av_a.p = calloc(1, tp->copy.av_a.size);

  tp->copy.ca_a = pp->base.ca_a;
  tp->copy.ca_a.p = calloc(1, tp->copy.ca_a.size);

  tp->copy.co_a = pp->base.co_a;
  tp->copy.co_a.p = calloc(1, tp->copy.co_a.size);

  tp->copy.di_a = pp->base.di_a;
  tp->copy.di_a.p = calloc(1, tp->copy.di_a.size);

  tp->copy.do_a = pp->base.do_a;
  tp->copy.do_a.p = calloc(1, tp->copy.do_a.size);

  tp->copy.dv_a = pp->base.dv_a;
  tp->copy.dv_a.p = calloc(1, tp->copy.dv_a.size);

  tp->copy.ii_a = pp->base.ii_a;
  tp->copy.ii_a.p = calloc(1, tp->copy.ii_a.size);

  tp->copy.io_a = pp->base.io_a;
  tp->copy.io_a.p = calloc(1, tp->copy.io_a.size);

  tp->copy.iv_a = pp->base.iv_a;
  tp->copy.iv_a.p = calloc(1, tp->copy.iv_a.size);

}

static void
dlink_area (
  plc_sDlink *dp,
  char *name,
  int size
)
{
  pwr_tStatus	sts;

  if ((dp->size = size) == 0) return; /* We have no objects of this type.  */

  sts = gdh_RefObjectInfo(name, &dp->p, &dp->sid, dp->size);
  if (EVEN(sts)) {
    errh_Error("Direct link object %s, %m", name, sts);
    exit(sts);
  }
}

static void
create_threads (
  plc_sProcess	*pp
)
{
  plc_sThread	*tp;
  plc_sProctbl **ptp;
  extern plc_sProctbl *plc_proctbllist[];

  /* Count our threads.  */
  for (ptp = plc_proctbllist; ptp != NULL && *ptp != NULL; ptp++)
    pp->thread_count++;

  tp = pp->thread = calloc(pp->thread_count, sizeof(*tp));

  for (ptp = plc_proctbllist; ptp != NULL && *ptp != NULL; ptp++, tp++)
    create_thread(tp, *ptp, pp);
}

static void
create_thread (
  plc_sThread	*tp,
  plc_sProctbl	*ptp,
  plc_sProcess	*pp
)
{
  pwr_tStatus	sts;
  int phase;

  tp->aref.Objid = ptp->thread;
  tp->init = ptp->init;
  tp->exec = ptp->exec;

  tp->PlcThread = pwrb_PlcThread_Init(&sts, tp);
  tp->csup_lh = csup_Init(&sts, ptp->thread, tp->f_scan_time);

  tp->i_scan_time = tp->f_scan_time * 1000.0 + 0.5;

  time_MsToD(&tp->scan_time, tp->i_scan_time);
  tp->pp = pp;
 
  plc_inittimer(tp);
  tp->exit = FALSE;
  link_io_copy_areas(tp);

  que_Create(&sts, &tp->q_in);
  que_Create(&sts, &tp->q_out);

  sts = gdh_ObjidToName(ptp->thread, tp->name, sizeof(tp->name), cdh_mNName);
  if (EVEN(sts)) {
    errh_Error("Get name of thread object %s, %m", cdh_ObjidToString(NULL, ptp->thread, 1), sts);
    return;
  }

  sts = gdh_ObjidToPointer(ptp->thread, (void *)&tp->PlcThread);
  if (EVEN(sts)) {
    errh_Error("Direct link to thread object \"%s\", %m", tp->name, sts);
    return;
  }

#if defined OS_LYNX && USE_RT_TIMER
  sem_init(&tp->ScanSem, 0, 0);
  tp->ScanMultiple = tp->i_scan_time / (CLK_TCK/1000);
  tp->IntervalCount = tp->ScanMultiple; 
#endif


  sts = thread_Create(&tp->tid, tp->name, (void *(*)())&plc_thread, tp);
  if (EVEN(sts)) {
    errh_Error("Creating thread \"%s\", %m", tp->name, sts);
    return;
  }

  /* Wait for thread to initialize.  */
  phase = (int)que_Get(&sts, &tp->q_out, NULL, NULL);
  pwr_Assert(phase == 1);
}

/* Initializes all init steps on own node. */

static void
init_grafcet ()
{
  pwr_tStatus	sts;
  pwr_tObjid	oid;
  pwr_sClass_initstep *o;

  sts = gdh_GetClassList(pwr_cClass_initstep, &oid);
  while (ODD(sts)) {
    sts = gdh_ObjidToPointer(oid, (void *)&o);
    if (EVEN(sts)) {
      errh_Error("Initialize all GRAFCET init steps, %m", sts);
      return;
    }
    if (!o->StatusInit) {
      /* This InitStep is not initialized. */
      o->Status[0] = 1;
      o->StatusInit = 1;
    }
    sts = gdh_GetNextObject(oid, &oid);
  }
}

static void
save_values (
  plc_sProcess	*pp
)
{
  int i;

  for (i = 0; i < pp->IOHandler->AvCount; i++) {
    pwr_tFloat32 *p = gdh_TranslateRtdbPointer(pp->base.av_i.p->Value[i]);
    if (p != NULL)
      *p = pp->base.av_a.p->Value[i];
  }
  for (i = 0; i < pp->IOHandler->IvCount; i++) {
    pwr_tInt32 *p = gdh_TranslateRtdbPointer(pp->base.iv_i.p->Value[i]);
    if (p != NULL)
      *p = pp->base.iv_a.p->Value[i];
  }
  for (i = 0; i < pp->IOHandler->DvCount; i++) {
    pwr_tBoolean *p = gdh_TranslateRtdbPointer(pp->base.dv_i.p->Value[i]);
    if (p != NULL)
      *p = pp->base.dv_a.p->Value[i];
  }
}

static void
set_values (
  plc_sProcess	*pp
)
{
  int i;

  for (i = 0; i < pp->IOHandler->AvCount; i++) {
    pwr_tFloat32 *p = gdh_TranslateRtdbPointer(pp->base.av_i.p->Value[i]);
    if (p != NULL)
      pp->base.av_a.p->Value[i] = *p;
  }
  for (i = 0; i < pp->IOHandler->IvCount; i++) {
    pwr_tInt32 *p = gdh_TranslateRtdbPointer(pp->base.iv_i.p->Value[i]);
    if (p != NULL)
      pp->base.iv_a.p->Value[i] = *p;
  }
  for (i = 0; i < pp->IOHandler->DvCount; i++) {
    pwr_tBoolean *p = gdh_TranslateRtdbPointer(pp->base.dv_i.p->Value[i]);
    if (p != NULL)
      pp->base.dv_a.p->Value[i] = *p;
  }
}
