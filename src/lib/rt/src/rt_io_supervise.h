#ifndef rt_io_supervise_h
#define rt_io_supervise_h

/* rt_io_supervise.h -- Runtime environment - I/O supervise

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef PWR_BASECLASSES_H
#include "pwr_baseclasses.h"
#endif

typedef struct s_ASupLstLink sASupLstLink;
typedef struct s_DSupLstLink sDSupLstLink;

typedef struct {
  sASupLstLink	*TimerLstP;
  sASupLstLink	*ASupAnaLstP;
  sDSupLstLink	*DSupDigLstP;
  pwr_tFloat32	TimerTime;
} *io_tSupCtx;

#ifndef rt_plc_timer_h
#include "rt_plc_timer.h"
#endif

struct s_ASupLstLink {
  sASupLstLink		*NextP;
  sASupLstLink		*NextTimerP;
  pwr_sClass_ASup	*SupP;
  pwr_tFloat32		*ValueP;
  plc_sTimer		*TimerP;
};

struct s_DSupLstLink {
  sDSupLstLink		*NextP;
  sDSupLstLink		*NextTimerP;
  pwr_sClass_DSup	*SupP;
  pwr_tBoolean		*ValueP;
  plc_sTimer		*TimerP;
};

pwr_tStatus io_ConnectToSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
);

/* Initialize sup lists */

pwr_tStatus io_InitSupLst (
  io_tSupCtx	*Ctx,
  pwr_tFloat32  CycleTime
  );

/* Scan bus connected analog in- and outputs ASup list.  */

pwr_tStatus io_ScanSupLst (
    io_tSupCtx		Ctx
);

/* Scan bus connected analog in- and outputs ASup timer list.  */

pwr_tStatus io_ScanSupTimerLst (
  io_tSupCtx	Ctx
);

/* Clear digital and analog lists */

pwr_tStatus io_ClearSupLst (
    io_tSupCtx		Ctx
);

#endif
