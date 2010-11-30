/* 
 * Proview   $Id: rt_plc.h,v 1.9 2008-09-23 07:24:00 claes Exp $
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

#ifndef rt_plc_h
#define rt_plc_h

#if defined OS_LYNX
# include <pthread.h>
# include <semaphore.h>
#elif defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD
# include <pthread.h>
# include "rt_semaphore.h"
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_systemclasses_h
# include "pwr_systemclasses.h"
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



static const unsigned int pwr_cInputOffset = pwr_AlignLW(sizeof(void*)) + pwr_AlignLW(sizeof(pwr_tBoolean));

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
  IO_AREA(pwr_sClass_IiArea) ii_a;
  IO_AREA(pwr_sClass_IoArea) io_a;
  IO_AREA(pwr_sClass_IvArea) iv_a;
  IO_AREA(pwr_sClass_InitArea) av_i;
  IO_AREA(pwr_sClass_InitArea) dv_i;
  IO_AREA(pwr_sClass_InitArea) iv_i;
  IO_AREA(pwr_sClass_InitArea) ai_i;
  IO_AREA(pwr_sClass_InitArea) di_i;
  IO_AREA(pwr_sClass_InitArea) ii_i;
  IO_AREA(pwr_sClass_InitArea) ao_i;
  IO_AREA(pwr_sClass_InitArea) do_i;
  IO_AREA(pwr_sClass_InitArea) io_i;
} plc_sArea;

#ifndef rt_io_base_h
# include "rt_io_base.h"
#endif

struct plc_sThread {
  thread_s		tid;
  pwr_tOName   		name;
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
  pwr_tTime	        sync_time;
  pwr_tTime	        one_before_scan;
  pwr_tTime	        before_scan;
  pwr_tTime	        after_scan;
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
  pwr_tBoolean		first_scan;
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
