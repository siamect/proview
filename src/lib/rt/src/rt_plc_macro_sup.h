/* rt_plc_macro_sup.h -- Runtime environment  -  PLC

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Contains macros for analog and digital supervision of parameters.
   This code is used in the PLC-program environment.  */

#ifdef OS_VMS
#include <starlet.h>
#endif

/*
 Name:
   Sup_init(object, In, con)				

 Description:
   Initialize a supervisor object
   This macro is not used.

*/
#define Sup_init(o)					\
    ;
#if 0
  o->AlarmCheck = TRUE;					\
  o->DetectCheck = TRUE;				\
  o->ReturnCheck = FALSE;				\
  o->Acked = TRUE;
#endif
/*
* Name:
*   ASup_init(object, In, con)				
*
* Description:
*   Initialize analog supervisor object.
*   This macro is not used.
*/
#define ASup_init(o, In, con)				\
  ;
#if 0
  o->CheckAlarm = TRUE;					\
  o->CheckDetect = TRUE;				\
  o->CheckReturn = FALSE;				\
  o->Acked = TRUE;
#endif

/*_*
  Name:
    ASup_exec(object, In, con)				

  Description:
    Supervise analog parameter

  @aref asup ASup
*/
#define	ASup_exec(o, In, con)\
  if (o->High && In <= o->CtrlLimit - o->Hysteres	\
    || !o->High && In >= o->CtrlLimit + o->Hysteres	\
  ) {							\
    if (o->Action) o->Action = FALSE;			\
    if (o->ReturnCheck) {				\
      clock_gettime(CLOCK_REALTIME, &o->ReturnTime);	\
      o->ReturnCheck = FALSE;				\
      o->ReturnSend = TRUE;				\
    }							\
    if (o->AlarmCheck && !o->DetectCheck) {		\
      o->TimerCount = 0;				\
      o->DetectCheck = TRUE;				\
    }							\
  } else if (con && (o->High && In > o->CtrlLimit	\
    || !o->High && In < o->CtrlLimit)			\
  ) {							\
    if (!o->Action) o->Action = TRUE;			\
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {	\
      if (o->DetectCheck) {				\
	o->ActualValue = In;				\
	timer_in(tp, o);				\
	clock_gettime(CLOCK_REALTIME, &o->DetectTime);	\
	o->DetectCheck = FALSE;				\
      }							\
      if (!o->TimerFlag) {				\
	o->DetectSend = TRUE;				\
	o->ReturnCheck = TRUE;				\
	o->Acked = FALSE;				\
	o->AlarmCheck = FALSE;				\
      }							\
    }							\
  }							\
  if (o->Blocked) {					\
    o->TimerCount = 0;					\
    o->DetectCheck = TRUE;				\
  }

/*
* Name:
*   DSup_init(object, In, con)				
*
* Description:
*   Initialize digital supervisor object
*   This macro is not used.
*/
#define DSup_init(o, In, con)				\
  ;
#if 0
  o->CheckAlarm = TRUE;					\
  o->CheckDetect = TRUE;				\
  o->CheckReturn = FALSE;				\
  o->Acked = TRUE;
#endif
/*_*
  Name:
    DSup_exec(object, In, con)				

  Description:
    Supervise digital parameter

  @aref dsup DSup
*/
#define	DSup_exec(o, In, con)				\
  if (In != o->CtrlPosition) {				\
    if (o->Action) o->Action = FALSE;			\
    if (o->ReturnCheck) {				\
      clock_gettime(CLOCK_REALTIME, &o->ReturnTime);	\
      o->ReturnCheck = FALSE;				\
      o->ReturnSend = TRUE;				\
    }							\
    if (o->AlarmCheck && !o->DetectCheck) {		\
      o->TimerCount = 0;				\
      o->DetectCheck = TRUE;				\
    }							\
  } else if (con) {					\
    if (!o->Action) o->Action = TRUE;			\
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {	\
      if (o->DetectCheck) {				\
	o->ActualValue = In;				\
	timer_in(tp, o);				\
	clock_gettime(CLOCK_REALTIME, &o->DetectTime);	\
	o->DetectCheck = FALSE;				\
      }							\
      if (!o->TimerFlag) {				\
	o->DetectSend = TRUE;				\
	o->ReturnCheck = TRUE;				\
	o->Acked = FALSE;				\
	o->AlarmCheck = FALSE;				\
      }							\
    }							\
  }							\
  if (o->Blocked) {					\
    o->TimerCount = 0;					\
    o->DetectCheck = TRUE;				\
  }    
