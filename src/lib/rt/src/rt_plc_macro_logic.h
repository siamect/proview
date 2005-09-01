/* 
 * Proview   $Id: rt_plc_macro_logic.h,v 1.4 2005-09-01 14:57:56 claes Exp $
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

/*		 PREPROCESSOR ROUTINES FOR LOGIC CONTROL	    */

/*_*
  And

  and-gate
  @aref and And
*/
#define and_exec(obj,expr)						\
  obj->Status = ( expr );

/*_*
  OR
  or-gate

  @aref or Or
*/
#define or_exec(obj,expr)						\
  obj->Status = ( expr );

/*_*
  XOR
  exclusive or-block

  @aref xor XOr
*/
#define xor_exec(obj,in1,in2)						\
  obj->Status = !(in1 == in2 );

/*_*
  EDGE
  detects logic rise.

  @aref edge Edge
*/
#define edge_exec(obj,in)						\
  obj->Status = (in && !obj->StatusOld);				\
  obj->StatusOld = in;



/*_*
  AARITHM
  @aref aarithm AArithm
*/
#define aarithm_exec(obj,expr)						\
  obj->ActVal = expr;

/*_*
  DARITHM
  @aref darithm DArithm
*/
#define darithm_exec(obj,expr)						\
  obj->Status = expr;

/*_*
  CARITHM
  @aref carithm CArithm
*/
#define carithm_exec(obj,expr)\
{\
  expr\
}

/*_*
  SR_S
  @aref sr_s SR_S
*/
#define sr_s_exec(obj,set,reset)						\
  if (set)								\
    obj->Status = true;							\
  else if (reset) obj->Status = false;

/*_*
  SR_R
  @aref sr_r SR_R
*/
#define sr_r_exec(obj,set,reset)						\
  if (reset)								\
    obj->Status = false;						\
  else if (set) obj->Status = true;

/*_*
  INV
  @aref inv Inv
*/
#define inv_exec(obj,in)						\
  obj->Status = !in;

/*_*
  PULSE
  @aref pulse Pulse
*/
#define pulse_exec(obj,in)						\
  if ( in && !obj->StatusOld )						\
  {									\
    timer_in( tp, obj );						\
    obj->Status = true;							\
  }									\
  else									\
    obj->Status = obj->TimerFlag;					\
  obj->StatusOld = in;

/*_*
  WAIT
  @aref wait Wait
*/
#define wait_exec(obj,in)						\
  if ( in && !obj->StatusOld )						\
  {									\
    timer_in( tp, obj);							\
  }									\
  obj->StatusOld = in;							\
  obj->Status = in && !obj->TimerFlag;

/*_*
  TIMER
  @aref timer Timer
*/
#define timer_exec(obj,in)						\
  if ( !in && obj->StatusOld )						\
  {									\
    timer_in( tp, obj);							\
  }									\
  obj->StatusOld = in;							\
  obj->Status = in || obj->TimerFlag;

/*_*
  WAITH
  @aref waith Waith
*/
#define waith_exec(obj,in,hold)						\
  if ( in )								\
  {									\
    if ( !obj->StatusOld )						\
    {									\
      timer_in( tp, obj);						\
      obj->CountOld = obj->TimerCount;					\
    }									\
    if ( hold ) 							\
    {									\
      obj->TimerCount = obj->CountOld;					\
      obj->StatusOld = true;						\
    }									\
    else								\
    {									\
      obj->CountOld = obj->TimerCount;					\
      obj->StatusOld = true;						\
      obj->Status = !obj->TimerFlag;					\
    }									\
  }									\
  else 									\
  {									\
    obj->StatusOld = false;						\
    obj->Status = false;						\
  }

/*_*
  STRCAT
  @aref strcat Strcat
*/
#define Strcat_exec(obj,str1,str2)				       	\
  strcpy(obj->ActVal, str1);   						\
  strncat(obj->ActVal, str2, sizeof(obj->ActVal));

