/* rt_plc_macro_div.h -- Runtime environment  -  PLC

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Contains macros for object that does not fit anywhere else.
   This code is used in the PLC-program environment.  */

#define Backup_init(object, data)\
    strcpy( &(object->DataName), data);

/*_*
  @aref windowplc WindowPlc
*/
#define windowplc_exec(object)\
    if ( object->ScanOff) return;

/*_*
  @aref windowcond WindowCond
*/
#define windowcond_exec(object)\
    if ( object->ScanOff) return;

/*_*
  @aref windoworderact WindowOrderact
*/
#define windoworderact_exec(object)\
    if ( object->ScanOff) return;

/*_*
  @aref windowsubstep WindowSubstep
*/
#define windowsubstep_exec(object)\
    if ( object->ScanOff) return;

/*_*
  @aref scantime ScanTime
*/
#define ScanTime_exec(object)\
    object->Time = *object->ScanTime;

/*_*
  @aref firstscan FirstScan
*/
#define FirstScan_exec(object)\
    object->Status = tp->first_scan;

/*_*
  @aref abs Abs
*/
#define Abs_exec(object, in)\
    object->ActVal = fabsf( in);

/*_*
  @aref iabs IAbs
*/
#define IAbs_exec(object, in)\
    object->ActVal = abs( in);

/*_*
  @aref sin Sin
*/
#define Sin_exec(object, in)\
    object->ActVal = object->FactorVal * sinf( object->FactorIn * in);

/*_*
  @aref sin Cos
*/
#define Cos_exec(object, in)\
    object->ActVal = object->FactorVal * cosf( object->FactorIn * in);

/*_*
  @aref sin Tan
*/
#define Tan_exec(object, in)\
    object->ActVal = object->FactorVal * tanf( object->FactorIn * in);

/*_*
  @aref asin ASin
*/
#define ASin_exec(object, in)\
    object->ActVal = object->FactorVal * asinf( object->FactorIn * in);

/*_*
  @aref sin ACos
*/
#define ACos_exec(object, in)\
    object->ActVal = object->FactorVal * acosf( object->FactorIn * in);

/*_*
  @aref sin ATan
*/
#define ATan_exec(object, in)\
    object->ActVal = object->FactorVal * atanf( object->FactorIn * in);

/*_*
  @aref sqrt Sqrt
*/
#define Sqrt_exec(object, in)\
    object->ActVal = object->FactorVal * sqrtf( object->FactorIn * in);

/*_*
  @aref even Even
*/
#define Even_exec(object, in)\
    object->Status = ((in & 1) == 0);

/*_*
  @aref odd Odd
*/
#define Odd_exec(object, in)\
    object->Status = ((in & 1) != 0);

/*_*
  @aref ln Ln
*/
#define Ln_exec(object, in)\
    object->ActVal = object->FactorVal * logf( object->FactorIn * in);

/*_*
  @aref log Log
*/
#define Log_exec(object, in)\
    object->ActVal = object->FactorVal * log10f( object->FactorIn * in);

/*_*
  @aref exp Exp
*/
#define Exp_exec(object, in)\
    object->ActVal = object->FactorVal * expf( object->FactorIn * in);

/*_*
  @aref bwand BwAnd
*/
#define BwAnd_exec(t, o)\
    o->Status = *o->In1P & *o->In2P;

/*_*
  @aref bwor BwOr
*/
#define BwOr_exec(t, o)\
    o->Status = *o->In1P | *o->In2P;








