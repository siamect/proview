/* 
 * Proview   $Id: rt_plc_macro_time.h,v 1.3 2006-05-09 05:39:00 claes Exp $
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

/*		 Preprocessor routines for time operations	    */

/*_*
  STOATV
  store absolute time value			
  @aref stoatv StoATv
*/
#define StoATv_exec(obj,in) \
  obj->ActualValue = in;

/*_*
  STODTV
  store delta time value			
  @aref stodtv StoDTv
*/
#define StoDTv_exec(obj,in) \
  obj->ActualValue = in;

/*_*
  CSTOATV								
  store conditionally into absolute time value		
  @aref cstoatv CStoATv
*/
#define CStoATv_exec(obj,in,cond) \
  if ( cond ) \
    obj->ActualValue = in;

/*_*
  CSTODTV								
  store conditionally into delta time value		
  @aref cstodtv CStoDTv
*/
#define CStoDTv_exec(obj,in,cond) \
  if ( cond ) \
    obj->ActualValue = in;

/*_*
  STOATP								
  Store into absolute time attribute			
  @aref stoatp StoATp
*/
#define StoATp_exec(ut,in) \
  ut = in;

/*_*
  STODTP								
  Store into delta time attribute			
  @aref stodtp StoDTp
*/
#define StoDTp_exec(ut,in) \
  ut = in;

/*_*
  CSTOATP
  Store conditionally into absolute time attribute	
  @aref cstoatp CStoATp
*/
#define CStoATp_exec(ut,in,cond) \
  if ( cond) \
    ut = in;

/*_*
  CSTODTP
  Store conditionally into delta time attribute	
  @aref cstodtp CStoDTp
*/
#define CStoDTp_exec(ut,in,cond) \
  if ( cond) \
    ut = in;

/*_*
  ATADD
  @aref atadd AtAdd
*/
#define AtAdd_exec(obj,t1,t2) \
  time_Aadd( &obj->ActVal, &t1, &t2);

/*_*
  DTADD
  @aref dtadd DtAdd
*/
#define DtAdd_exec(obj,t1,t2) \
  time_Dadd( &obj->ActVal, &t1, &t2);

/*_*
  ATSUB
  @aref atsub AtSub
*/
#define AtSub_exec(obj,t1,t2) \
  time_Adiff( &obj->ActVal, &t1, &t2);

/*_*
  ATDTSUB
  @aref atdtsub AtDtSub
*/
#define AtDtSub_exec(obj,t1,t2) \
  time_Asub( &obj->ActVal, &t1, &t2);

/*_*
  DTSUB
  @aref dtsub DtSub
*/
#define DtSub_exec(obj,t1,t2) \
  time_Dsub( &obj->ActVal, &t1, &t2);

/*_*
  DTTOA
  @aref dttoa DtToA
*/
#define DtToA_exec(obj,t) \
  time_DToFloat( &obj->ActVal, &t);

/*_*
  ATODT
  @aref atodt AToDt
*/
#define AToDt_exec(obj, val) \
  time_FloatToD( &obj->DTime, val);

/*_*
  CURRENTTIME
  @aref currentime CurrentTime
*/
#define CurrentTime_exec(obj) \
  obj->Time = tp->pp->Node->SystemTime;

/*_*
  ATGREATERTHAN
  @aref atgreaterthan AtGreaterThan
*/
#define AtGreaterThan_exec(obj,t1,t2) \
  obj->Status = (time_Acomp( &t1, &t2) == 1);

/*_*
  ATLESSTHAN
  @aref atlessthan AtLessThan
*/
#define AtLessThan_exec(obj,t1,t2) \
  obj->Status = (time_Acomp( &t1, &t2) == -1);

/*_*
  ATEQUAL
  @aref atequal AtEqual
*/
#define AtEqual_exec(obj,t1,t2) \
  obj->Status = (time_Acomp( &t1, &t2) == 0);

/*_*
  DTGREATERTHAN
  @aref dtgreaterthan DtGreaterThan
*/
#define DtGreaterThan_exec(obj,t1,t2) \
  obj->Status = (time_Dcomp( &t1, &t2) == 1);

/*_*
  DTLESSTHAN
  @aref dtlessthan DtLessThan
*/
#define DtLessThan_exec(obj,t1,t2) \
  obj->Status = (time_Dcomp( &t1, &t2) == -1);

/*_*
  DTEQUAL
  @aref dtequalthan DtEqual
*/
#define DtEqual_exec(obj,t1,t2) \
  obj->Status = (time_Dcomp( &t1, &t2) == 0);










