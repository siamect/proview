/* rt_plc_macro_div.h -- Runtime environment  -  PLC

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Contains macros for object that does not fit anywhere else.
   This code is used in the PLC-program environment.  */

#define Backup_init(object, data)\
    strcpy( &(object->DataName), data);

/**
  @aref windowplc WindowPlc
*/
#define windowplc_exec(object)\
    if ( object->ScanOff) return;

/**
  @aref windowcond WindowCond
*/
#define windowcond_exec(object)\
    if ( object->ScanOff) return;

/**
  @aref windoworderact WindowOrderact
*/
#define windoworderact_exec(object)\
    if ( object->ScanOff) return;

/**
  @aref windowsubstep WindowSubstep
*/
#define windowsubstep_exec(object)\
    if ( object->ScanOff) return;

/**
  @aref scantime ScanTime
*/
#define ScanTime_exec(object)\
    object->Time = *object->ScanTime;

