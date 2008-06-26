/* 
 * Proview   $Id: rt_plc_macro_div.h,v 1.7 2008-06-26 13:05:01 claes Exp $
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

/* rt_plc_macro_div.h -- Runtime environment  -  PLC
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

/*_*
  @aref bwand BwInv
*/
#define BwInv_exec(t, o)\
    o->Out = ~(*o->InP);

/*_*
  @aref getdatap GetDatap
*/
#define GetDatap_exec(object, in)\
    memcpy( &object->Out, &in, 12);

/*_*
  @aref getdatainput GetDatainput
*/
#define GetDataInput_exec(object, in)\
    if ( in) \
      memcpy( &object->OutDataP, in, 12);








