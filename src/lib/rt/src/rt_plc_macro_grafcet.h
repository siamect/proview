/* rt_plc_macro_grafcet.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#define false 0
#define true 1
#define _z_ ,

static pwr_tBoolean *pointer;
static pwr_tBoolean **ppointer;
static pwr_tBoolean test;
static pwr_tBoolean order_old;

/*_*
   InitStep

   Sequence-step that is set on reset-order for sequence.
   You may chain a simple order direct to the step.

   @aref initstep InitStep
*/
#define initstep_exec(obj,reset,chain)					\
  if ( reset) obj->Status[0] = false;					\
  else if ( obj->ResetOld ) obj->Status[0] = true;			\
  obj->ResetOld = reset;						\
    chain								\
  obj->Status[1] = obj->Status[0]

/*_*
   Step

   Sequence-step that is reset on reset-order for sequence.
   You may chain a simple order direct to the step.

    @aref step Step
*/

#define step_exec(obj,reset,chain)					\
  if (reset) obj->Status[0] = false;					\
    chain								\
  obj->Status[1] = obj->Status[0]

/*_* 
   SubStep

   Sequence-step that includes a sub-sequence.
   You may chain a simple order direct to the step.

   @aref substep SubStep
*/

#define substep_exec(obj,reset,subseq,chain)				\
  if (reset) obj->Status[0] = false;					\
    chain								\
    subseq

/*_*
   SSBegin

   First sequence-step in subsequence.
   You may chain a simple order direct to the step.

   @aref ssbegin SsBegin
*/

#define ssbegin_exec(obj,substep,reset,chain)				\
  if (substep->Status[0] && !obj->StatusOld) obj->Status[0] = true;	\
  if (reset) obj->Status[0] = false;					\
  obj->StatusOld = substep->Status[0];					\
    chain								\
  obj->Status[1] = obj->Status[0]

/*_*
   SSEnd

   Last sequence-step in subsequence.
   You may chain a simple order direct to the step.

   @aref ssend SsEnd
*/

#define ssend_exec(obj,substep,reset,chain)				\
  if (reset || !substep->Status[0]) obj->Status[0] = false;		\
    chain								\
  substep->Status[1]  = obj->Status[1] = obj->Status[0]


/*_*
   Trans

   Transition of active status from one or more step to one or
   more next step.

   @aref trans Trans
*/

#define trans_exec(obj,insteplist,outsteplist,cond)			\
{									\
  pwr_tBoolean *inpntr[] = insteplist;		 			\
  pwr_tBoolean *outpntr[] = outsteplist;				\
  /* Test condition */							\
  cond									\
  if ( obj->Man ) obj->Cond = obj->OpCond;				\
  if ( obj->Cond )                                                      \
  {                                                                     \
    /* Test if step(s) above are active */				\
    test = true;							\
    ppointer = inpntr;							\
    while ( test && *ppointer != NULL )					\
    {									\
      pointer = *ppointer++;						\
      if ( !*pointer++ ) test = false;					\
      else if ( !*pointer ) test = false;				\
    }									\
    /* Transfer active status to step(s) below */			\
    if ( test )								\
    {									\
      ppointer = inpntr;						\
      while ( *ppointer != NULL )					\
        **ppointer++ = false;						\
      ppointer = outpntr;						\
      while ( *ppointer != NULL )					\
        **ppointer++ = true;						\
      obj->OpCond = false;						\
    }							   		\
  }									\
}

/*_*
   Order 
   @aref order Order
*/

#define order_exec(obj,stepobj,chain)					\
  order_old = obj->Status[0];						\
  obj->Status[0] = stepobj->Status[0];					\
  if (obj->Status[0] || order_old || stepobj->Status[0] || stepobj->Status[1]) \
  {									\
    chain								\
  }

/*_*
  DOrder
  @aref dorder DOrder
*/

#define dorder_exec(obj,stepobj)					\
  if (stepobj->Status[0])						\
  {									\
    if ( !obj->Old )							\
    {									\
      timer_in( tp, obj );						\
    }									\
    obj->Status[0] = (obj->TimerFlag) ? false : true;			\
  }									\
  else									\
  {									\
    if ( obj->Status[0])						\
    {									\
      obj->TimerCount = 0;						\
      obj->Status[0] = false;						\
    }									\
  }									\
  obj->Status[1] = stepobj->Status[0] || stepobj->Status[1];		\
  obj->Old = stepobj->Status[0];

/*_*
   LOrder
   @aref lorder LOrder
*/

#define lorder_exec(obj,stepobj)					\
  if ( stepobj->Status[0] )						\
  {									\
    if ( !obj->StatusOld )						\
    {									\
      timer_in( tp, obj );						\
      obj->Status[0] = true;						\
    }									\
    else								\
      obj->Status[0] = obj->TimerFlag;					\
  }									\
  else									\
  {									\
    if ( obj->Status[0])						\
    {									\
      obj->TimerCount = 0;						\
      obj->Status[0] = false;						\
    }									\
  }									\
  obj->Status[1] = stepobj->Status[0] || stepobj->Status[1];		\
  obj->StatusOld = stepobj->Status[0]

/*_*
   POrder
   @aref porder POrder
*/

#define porder_exec(obj,stepobj)					\
  obj->Status[0] = ( stepobj->Status[0] && !obj->StatusOld );		\
  obj->Status[1] = stepobj->Status[0] || stepobj->Status[1];		\
  obj->StatusOld = stepobj->Status[0]

/*_*
   COrder
   @aref corder COrder
*/

#define corder_exec(obj,stepobj,condition)				\
  condition								\
  obj->Status[0] = stepobj->Status[0] && obj->Cond;			\
  obj->Status[1] = stepobj->Status[0] || stepobj->Status[1]

/*_*
   SOrder
   @aref sorder SOrder
*/

#define sorder_exec(obj,stepobj,reset)					\
  if ( obj->Reset || reset) obj->Status[0] = false;			\
  if ( stepobj->Status[0] && !obj->Old ) obj->Status[0] = true;		\
  obj->Old = stepobj->Status[0];					\
  obj->Status[1] = obj->Status[0] || stepobj->Status[1];		\
  obj->Reset = false

/*_* 
  Reset_So
  @aref reset_so Reset_SO
*/

#define reset_so_exec(sorderobj,in)					\
  if ( in ) sorderobj->Reset = true

/*_*
   SetCond 

   Set condition
   @aref setcond SetCond
*/

#define setcond_exec(obj,in)						\
  obj->Cond = in

/*_*
   CSub	

   Conditional sub-window
   @aref csub CSub
*/

#define csub_exec(in,chain)						\
  if ( in )								\
  {									\
    chain								\
  }
