#ifndef rt_proc_h
#define rt_proc_h

/* rt_proc.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#if defined OS_LYNX || defined OS_LINUX
# include <sys/types.h>
#else
# include <types.h>
#endif
#include "pwr.h"
#include "co_errno.h"
#include "rt_errh.h"

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( debug	, 1),
    pwr_Bits( k_mode	, 1),		/* Kernel mode.  */
    pwr_Bits( system	, 1),
    pwr_Bits( base	, 1),
    pwr_Bits( user	, 1),
    pwr_Bits( load	, 1),
    pwr_Bits( fill_0	, 2),,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define proc_mProcess__			0
#define proc_mProcess_debug		pwr_Bit(0)
#define proc_mProcess_k_mode		pwr_Bit(1)
#define proc_mProcess_system		pwr_Bit(2)
#define proc_mProcess_base		pwr_Bit(3)
#define proc_mProcess_user		pwr_Bit(4)
#define proc_mProcess_load		pwr_Bit(5)

#define proc_mProcess_all		(proc_mProcess_system|proc_mProcess_base|proc_mProcess_user)

#define proc_mProcess_			(~proc_mProcess__)
} proc_mProcess;

typedef struct {
  proc_mProcess flags;
  int		p_prio;			/* Process priority.  */
  int		t_prio;			/* Thread priority.  */
  size_t	k_size;			/* Kernel stack size.  */
  size_t	u_size;			/* User stack size.  */
  char		*file;
  char		*name;
  char		*arg;
  pid_t		pid;
} proc_sProcess;
    
pwr_tStatus	proc_Load (proc_sProcess*);
pwr_tStatus	proc_Start (proc_sProcess*);
pwr_tStatus	proc_SetPriority (int);
pwr_tStatus	proc_UnloadProgram (proc_sProcess*);

#endif
