/* rt_plc_macro_mps.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

/*		 PREPROCESSOR RUTINER					    */

/*	MPSTRPTRIGG							*/
/*	funktion:	trigger mps transport object			*/
/*									*/
#define mpsTriggtrp_exec(obj,in)					\
  obj->InTriggFlag = in

/*	MPSTRPTRIGG							*/
/*	funktion:	trigger mps transport object			*/
/*									*/
#define mpsTrp_exec(obj)						\
  if ( obj->InTriggFlag && !obj->InTriggOld )				\
    obj->TriggFlag = TRUE;						\
  obj->InTriggOld = obj->InTriggFlag

