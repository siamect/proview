#ifndef rt_plc_h
#define rt_plc_h

/* rt_plc.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   <Description>.  */

#if defined OS_LYNX
# include <pthread.h>
# include <semaphore.h>
#elif defined OS_LINUX
# include <pthread.h>
# include "rt_semaphore.h"
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_baseclasses_h
# include "pwr_baseclasses.h"
#endif

#ifndef rt_thread_h
# include "rt_thread.h"
#endif

#ifndef rt_lst_h
# include "rt_lst.h"
#endif

#ifndef rt_que_h
# include "rt_que.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef pwr_cClass_PlcThread
# include "pwrb_c_plcthread.h"
#endif

#ifndef pwr_cClass_Node
# include "pwrs_c_node.h"
#endif

#ifndef pwr_cClass_IOHandler
# include "pwrb_c_iohandler.h"
#endif

typedef struct plc_sProcess plc_sProcess;
typedef struct plc_sThread plc_sThread;
typedef struct plc_sDlink plc_sDlink;

struct plc_sDlink {
  void	*p;
  pwr_tObjid	oid;
  pwr_tSubid	sid;
  pwr_tUInt32	size;
};

#define IO_AREA(type)	\
 struct {		\
    type	*p;	\
    pwr_tObjid	oid;	\
    pwr_tSubid	sid;	\
    pwr_tUInt32	size;	\
 }

typedef struct {
  IO_AREA(pwr_sClass_AiArea) ai_a;
  IO_AREA(pwr_sClass_AoArea) ao_a;
  IO_AREA(pwr_sClass_AvArea) av_a;
  IO_AREA(pwr_sClass_CaArea) ca_a;
  IO_AREA(pwr_sClass_CoArea) co_a;
  IO_AREA(pwr_sClass_DiArea) di_a;
  IO_AREA(pwr_sClass_DoArea) do_a;
  IO_AREA(pwr_sClass_DvArea) dv_a;
  IO_AREA(pwr_sClass_IvArea) av_i;
  IO_AREA(pwr_sClass_IvArea) dv_i;
} plc_sArea;

#ifndef rt_io_base_h
# include "rt_io_base.h"
#endif

struct plc_sThread {
  thread_s		tid;
  char			name[40];
#if defined OS_LYNX || defined OS_LINUX
  sem_t			ScanSem;
  pwr_tUInt32		ScanMultiple;
  pwr_tUInt32		IntervalCount;
#endif

  pwr_tBoolean		exit;
  pwr_tBoolean		log;
  pwr_tBoolean		sliped;

  int			i_scan_time;
  float			f_scan_time;
  pwr_tDeltaTime	scan_time;
  float			ActualScanTime;
  pwr_tDeltaTime	sync_time;
  pwr_tDeltaTime	one_before_scan;
  pwr_tDeltaTime	before_scan;
  pwr_tDeltaTime	after_scan;
  pwr_tDeltaTime	delta_scan;
  pwr_tTime		one_before_scan_abs;
  pwr_tTime		before_scan_abs;
  pwr_tTime		after_scan_abs;

  pwr_sClass_PlcThread	*PlcThread;
  lst_sEntry		*csup_lh;
  pwr_sAttrRef		aref;
  pwr_tDlid		dlid;
  thread_sMutex		mutex;
  thread_sCond		cond;

  pwr_tUInt32		loops;

  que_sQue		q_in;
  que_sQue		q_out;
  lst_sEntry		event;

  plc_sProcess		*pp;

  int			prio;
  void			(*init)(int, plc_sThread*);
  void			(*exec)(int, plc_sThread*);

  plc_sArea		copy;
  io_tCtx		plc_io_ctx;
};

struct plc_sProcess {
  int			thread_count;
  plc_sThread		*thread;
  pwr_sNode		*Node;
  pwr_sClass_IOHandler	*IOHandler;
  pwr_sClass_PlcProcess	*PlcProcess;
  pwr_tFullName		nodeName;
  qcom_sQid		eventQ;
  thread_sMutex		io_copy_mutex;
  plc_sArea		base;
};

#if defined OS_LYNX || defined OS_LINUX
typedef struct 
{  
  pwr_tUInt32	AreaIndex;
  pwr_tUInt32	Prio;
  pwr_tUInt32	(*InitFunc)(int DirectLink, unsigned long AreaIndex, unsigned long Prio, double Time);
  pwr_tUInt32	(*ExecFunc)();
} THREAD_ARG;
#endif 


#endif
