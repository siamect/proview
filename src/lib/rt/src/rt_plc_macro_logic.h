/* rt_plc_macro_logic.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

/*		 PREPROCESSOR ROUTINES FOR LOGIC CONTROL	    */

/**
  And

  and-gate
  @aref and And
*/
#define and_exec(obj,expr)						\
  obj->Status = ( expr );

/**
  OR
  or-gate

  @aref or Or
*/
#define or_exec(obj,expr)						\
  obj->Status = ( expr );

/**
  XOR
  exclusive or-block

  @aref xor XOr
*/
#define xor_exec(obj,in1,in2)						\
  obj->Status = !(in1 == in2 );

/**
  EDGE
  detects logic rise.

  @aref edge Edge
*/
#define edge_exec(obj,in)						\
  obj->Status = (in && !obj->StatusOld);				\
  obj->StatusOld = in;



/**
  AARITHM
  @aref aarithm AArithm
*/
#define aarithm_exec(obj,expr)						\
  obj->ActVal = expr;

/**
  DARITHM
  @aref darithm DArithm
*/
#define darithm_exec(obj,expr)						\
  obj->Status = expr;

/**
  CARITHM
  @aref carithm CArithm
*/
#define carithm_exec(obj,expr)\
{\
  expr\
}

/**
  SR_S
  @aref sr_s SR_S
*/
#define sr_s_exec(obj,set,reset)						\
  if (set)								\
    obj->Status = true;							\
  else if (reset) obj->Status = false;

/**
  SR_R
  @aref sr_r SR_R
*/
#define sr_r_exec(obj,set,reset)						\
  if (reset)								\
    obj->Status = false;						\
  else if (set) obj->Status = true;

/**
  INV
  @aref inv Inv
*/
#define inv_exec(obj,in)						\
  obj->Status = !in;

/**
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

/**
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

/**
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

/**
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

/**
  STRCAT
  @aref strcat STRCAT
*/
#define Strcat_exec(obj,str1,str2)				       	\
  strcpy(obj->ActVal, str1);   						\
  strncat(obj->ActVal, str2, sizeof(obj->ActVal));

