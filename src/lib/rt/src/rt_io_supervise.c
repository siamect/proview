/* 
 * Proview   $Id: rt_io_supervise.c,v 1.3 2008-09-23 07:25:53 claes Exp $
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

/* rt_io_supervise.c -- Runtime environment - I/O supervise */

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_io_msg.h"
#include "rt_plc_timer.h"
#include "rt_plc_macro_sup.h"
#include "rt_io_supervise.h"



/* Local function definitions */

static void ASup_exec_io (
  io_tSupCtx	Ctx,
  sASupLstLink	*TimerLstP,
  pwr_sClass_ASup *o,
  pwr_tFloat32 In,
  pwr_tBoolean con
);
static void DSup_exec_io (
  io_tSupCtx	Ctx,
  sDSupLstLink	*TimerLstP,
  pwr_sClass_DSup *o,
  pwr_tBoolean In,
  pwr_tBoolean con
);

static pwr_tStatus io_ConnectToAnaSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
);

static pwr_tStatus io_ConnectToDigSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
);


/*------------------------------------------------------------------------------
* Detect an ASup object.
* NOTE:
*   This should be the same as the macro used by PLC,
*   in rplc_src:rt_plc_macro_sup.h  
*/
static void ASup_exec_io (
  io_tSupCtx	Ctx,
  sASupLstLink	*TimerLstP,
  pwr_sClass_ASup *o,
  pwr_tFloat32 In,
  pwr_tBoolean con
)
{

  sASupLstLink	*NextLstP, *NextP;	  /* Diff. */


  if ((o->High && In <= (o->CtrlLimit - o->Hysteres))
    || (!o->High && In >= (o->CtrlLimit + o->Hysteres))
  ) {
    if (o->Action) o->Action = FALSE;
    if (o->ReturnCheck) {
      time_GetTime(&o->ReturnTime);
      o->ReturnCheck = FALSE;
      o->ReturnSend = TRUE;
    }
    if (o->AlarmCheck && !o->DetectCheck) {
      o->TimerFlag = FALSE;
      o->DetectCheck = TRUE;
    }
  } else if (con && ((o->High && In > o->CtrlLimit)
    || (!o->High && In < o->CtrlLimit))
  ) {
    if (!o->Action) o->Action = TRUE;
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
	o->ActualValue = In;

	/* timer_in(o); */

	o->TimerCount = o->TimerTime / Ctx->TimerTime;
	if (!o->TimerFlag && o->TimerCount > 0) {
	  TimerLstP->TimerP = (plc_sTimer *)&o->TimerFlag;
	  if ( Ctx->TimerLstP == NULL ) {
	    Ctx->TimerLstP = TimerLstP;
	  }
	  else {
	    if ( Ctx->TimerLstP->NextTimerP == NULL ) {
	      Ctx->TimerLstP->NextTimerP = TimerLstP;
	    }             
	    else {
	      NextP = Ctx->TimerLstP->NextTimerP;
	      while ( NextP != NULL ) {
		NextLstP = NextP;
		NextP = NextP->NextTimerP;
	      } 
	      NextLstP->NextTimerP = TimerLstP;
	    }
	  }
          o->TimerFlag = TRUE;
        }

	time_GetTime(&o->DetectTime);
	o->DetectCheck = FALSE;
      }
      if (!o->TimerFlag) {
	o->DetectSend = TRUE;
	o->ReturnCheck = TRUE;
	o->Acked = FALSE;
	o->AlarmCheck = FALSE;
      }
    }
  }
}


/*------------------------------------------------------------------------------
* Detect an DSup object.
* NOTE:
*   This should be the same as the macro used by PLC,
*   in rplc_src:rt_plc_macro_sup.h  
*/
static void DSup_exec_io (
  io_tSupCtx	Ctx,
  sDSupLstLink	*TimerLstP,
  pwr_sClass_DSup *o,
  pwr_tBoolean In,
  pwr_tBoolean con
)
{

  sDSupLstLink	*NextLstP, *NextP;	  /* Diff. */


  if (In != o->CtrlPosition) {				
    if (o->Action) o->Action = FALSE;			
    if (o->ReturnCheck) {				
      time_GetTime(&o->ReturnTime);			
      o->ReturnCheck = FALSE;				
      o->ReturnSend = TRUE;				
    }							
    if (o->AlarmCheck && !o->DetectCheck) {		
      o->TimerCount = 0;				
      o->DetectCheck = TRUE;				
    }							
  } else if (con) {					
    if (!o->Action) o->Action = TRUE;			
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {	
      if (o->DetectCheck) {			
	o->ActualValue = In;				

	/* timer_in(o); */

	o->TimerCount = o->TimerTime / Ctx->TimerTime;
	if (!o->TimerFlag && o->TimerCount > 0) {
	  TimerLstP->TimerP = (plc_sTimer *)&o->TimerFlag;
	  if ( Ctx->TimerLstP == NULL ) {
	    Ctx->TimerLstP = (sASupLstLink *)TimerLstP;
	  }
	  else {
	    if ( Ctx->TimerLstP->NextTimerP == NULL ) {
	      Ctx->TimerLstP->NextTimerP = (sASupLstLink *)TimerLstP;
	    }             
	    else {
	      NextP = (sDSupLstLink *)Ctx->TimerLstP->NextTimerP;
	      while ( NextP != NULL ) {
		NextLstP = NextP;
		NextP = NextP->NextTimerP;
	      } 
	      NextLstP->NextTimerP = TimerLstP;
	    }
	  }
          o->TimerFlag = TRUE;
        }

	time_GetTime(&o->DetectTime);			
	o->DetectCheck = FALSE;				
      }							
      if (!o->TimerFlag) {				
	o->DetectSend = TRUE;				
	o->ReturnCheck = TRUE;				
	o->Acked = FALSE;				
	o->AlarmCheck = FALSE;				
      }							
    }							
  }
}



/*
* Name:    
*   io_ConnectToSupLst
*
*
* Function:
*   Initialize list of Sup object for bus connected in- or output 
*   signals.
* Description:
*   
*/
pwr_tStatus io_ConnectToSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
)
{
  pwr_tStatus sts;

  switch ( Class) {
    case pwr_cClass_Di:
    case pwr_cClass_Do:
    case pwr_cClass_Po:
      sts = io_ConnectToDigSupLst( Ctx, Class, ObjId, ObjP);
      break;
    case pwr_cClass_Ai:
    case pwr_cClass_Ao:
      sts = io_ConnectToAnaSupLst( Ctx, Class, ObjId, ObjP);
      break;
    default:
      return IO__SUCCESS;
  }
  return sts;
}


/*
* Name:    
*   io_ConnectToAnaSupLst
*
*
* Function:
*   Initialize list of ASup object for bus connected analog in- och output 
*   signals.
* Description:
*   
*/
static pwr_tStatus io_ConnectToAnaSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
)
{
    pwr_tStatus	  Sts;
    pwr_sClass_ASup *ASupP;
    pwr_sClass_Ai *AiP;
    pwr_sClass_Ao *AoP;
    pwr_tFloat32  *Float32P;
    pwr_tObjid	  SupId;
    pwr_tClassId  ObjClass;
    sASupLstLink  *ASupLstP;
    sASupLstLink  *NextASupLstP, *NextP;
    


    switch (Class) {
      case pwr_cClass_Ai:
        AiP = (pwr_sClass_Ai *) ObjP;
        Float32P = gdh_TranslateRtdbPointer( (unsigned long)AiP->ActualValue ); 
	break;

      case pwr_cClass_Ao:
        AoP = (pwr_sClass_Ao *) ObjP;
        Float32P = gdh_TranslateRtdbPointer( (unsigned long)AoP->ActualValue ); 
	break;

      default:
	return ( IO__SUCCESS );
	break;
    }


    /* Get child with class ASup */
    Sts = gdh_GetChild ( ObjId, &SupId);
    while ( ODD(Sts)) {
       Sts = gdh_GetObjectClass ( SupId, &ObjClass);  
       if ( EVEN(Sts)) return (Sts);

       if ( ObjClass == pwr_cClass_ASup ) {
         Sts = gdh_ObjidToPointer ( SupId, (pwr_tAddress *)&ASupP); 
 	 if (EVEN(Sts)) return ( Sts );

	 ASupLstP = (sASupLstLink *) calloc( 1, sizeof(sASupLstLink) );
         ASupLstP->SupP = ASupP;
         ASupLstP->ValueP = Float32P;
         
         if ( Ctx->ASupAnaLstP == NULL ) {
           Ctx->ASupAnaLstP = ASupLstP;
         }
         else {
           if ( Ctx->ASupAnaLstP->NextP == NULL ) {
             Ctx->ASupAnaLstP->NextP = ASupLstP;
           }             
           else {
	     NextP = Ctx->ASupAnaLstP->NextP;
	     while ( NextP != NULL ) {
	       NextASupLstP = NextP;
	       NextP = NextP->NextP;
	     } 
             NextASupLstP->NextP = ASupLstP;
           }
	 }
       }

       Sts = gdh_GetNextSibling ( SupId, &SupId);
    }		

    return IO__SUCCESS;
} /* END io_ConnectToAnaSupLst */


/*
* Name:    
*   io_ConnectToDigSupLst
*
*
* Function:
*   Initialize list of DSup object for digitala in- och output signals.
* Description:
*   
*/
static pwr_tStatus io_ConnectToDigSupLst (
    io_tSupCtx		Ctx,
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
)
{
    pwr_tStatus	  Sts;
    pwr_sClass_DSup *DSupP;
    pwr_sClass_Di *DiP;
    pwr_sClass_Do *DoP;
    pwr_sClass_Po *PoP;
    pwr_tBoolean  *BooleanP;
    pwr_tObjid	  SupId;
    pwr_tClassId  ObjClass;
    sDSupLstLink  *DSupLstP;
    sDSupLstLink  *NextDSupLstP, *NextP;
    

    switch (Class) {
      case pwr_cClass_Di:
        DiP = (pwr_sClass_Di *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (unsigned long)DiP->ActualValue ); 
	break;

      case pwr_cClass_Do:
        DoP = (pwr_sClass_Do *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (unsigned long)DoP->ActualValue ); 
	break;

      case pwr_cClass_Po:
        PoP = (pwr_sClass_Po *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (unsigned long)PoP->ActualValue ); 
	break;

      default:
	return ( IO__SUCCESS );
	break;
    }


    /* Get child with class DSup */
    Sts = gdh_GetChild ( ObjId, &SupId);
    while ( ODD(Sts)) {
       Sts = gdh_GetObjectClass ( SupId, &ObjClass);  
       if ( EVEN(Sts)) return (Sts);

       if ( ObjClass == pwr_cClass_DSup ) {
         Sts = gdh_ObjidToPointer ( SupId, (pwr_tAddress *)&DSupP); 
 	 if (EVEN(Sts)) return ( Sts );

         DSupLstP = (sDSupLstLink *) calloc( 1, sizeof(sDSupLstLink) );
         DSupLstP->SupP = DSupP;
         DSupLstP->ValueP = BooleanP;
         DSupLstP->SupP->TimerDO = &(DSupLstP->SupP->TimerDODum);
         
         if ( Ctx->DSupDigLstP == NULL ) {
           Ctx->DSupDigLstP = DSupLstP;
         }
         else {
           if ( Ctx->DSupDigLstP->NextP == NULL ) {
             Ctx->DSupDigLstP->NextP = DSupLstP;
           }             
           else {
	     NextP = Ctx->DSupDigLstP->NextP;
	     while ( NextP != NULL ) {
	       NextDSupLstP = NextP;
	       NextP = NextP->NextP;
	     } 
             NextDSupLstP->NextP = DSupLstP;
           }
	 }
       }

       Sts = gdh_GetNextSibling ( SupId, &SupId);
    }		

    return IO__SUCCESS;
} /* END io_ConnectToDigSupLst */



/*
* Name:    
*   io_InitSupLst
*
*
* Function:
*   Initialize timer used for bus connected analog in- and outputs.
* Description:
*   
*/
pwr_tStatus io_InitSupLst (
  io_tSupCtx	*Ctx,
  pwr_tFloat32  CycleTime
)
{


    *Ctx = calloc( 1, sizeof(**Ctx));
    if ( CycleTime > 0.0 )
      (*Ctx)->TimerTime = CycleTime;
    else
      return IO__TIMEINVALID;

    return IO__SUCCESS;
} /* END io_InitTimer */


/*
* Name:    
*   io_ScanAnaSupLst
*
*
* Function:
*   Scan bus connected analog in- and outputs ASup list.
* Description:
*   
*/
pwr_tStatus io_ScanSupLst (
  io_tSupCtx	Ctx
)
{
    sASupLstLink *ASupLstP;
    sDSupLstLink *DSupLstP;


    /* Scan through the ASup list and execute ASup */
    ASupLstP = Ctx->ASupAnaLstP;
    while ( ASupLstP != NULL ) {
      ASup_exec_io ( Ctx, ASupLstP, ASupLstP->SupP, *ASupLstP->ValueP, TRUE );
      ASupLstP = ASupLstP->NextP;
    } 

    /* Scan through DSup list and execute macro DSup_exec */
    DSupLstP = Ctx->DSupDigLstP;
    while ( DSupLstP != NULL ) {
      DSup_exec_io ( Ctx, DSupLstP, DSupLstP->SupP, *DSupLstP->ValueP, TRUE );
      DSupLstP = DSupLstP->NextP;
    } 

    io_ScanSupTimerLst( Ctx);

    return IO__SUCCESS;
} /* END io_ScanSupLst */


/*
* Name:    
*   io_ScanSupTimerLst
*
*
* Function:
*   Scan timerlist.
* Description:
*   
*/
pwr_tStatus io_ScanSupTimerLst (
  io_tSupCtx	Ctx
)
{
    sASupLstLink *TimerLstP, *PrevP = NULL;
    plc_sTimer	*TimerP;
    sASupLstLink *NextTimerP;

    /* Scan through the timer list */
    TimerLstP = Ctx->TimerLstP;
    while ( TimerLstP != NULL ) {
      TimerP = TimerLstP->TimerP;
      if ( (TimerP->TimerCount <= 1) || !TimerP->TimerFlag ) {
        TimerP->TimerCount = 0;
        TimerP->TimerFlag = FALSE;      
        if ( PrevP != NULL )
          PrevP->NextTimerP = TimerLstP->NextTimerP;
        else
          Ctx->TimerLstP = TimerLstP->NextTimerP;
        NextTimerP = TimerLstP->NextTimerP;
        TimerLstP->NextTimerP = NULL;
        TimerLstP = NextTimerP;        
      }
      else 
      {
        TimerP->TimerCount--;
        PrevP = TimerLstP;
        TimerLstP = TimerLstP->NextTimerP;
      }  
    } 
    return IO__SUCCESS;
}

/*
* Name:    
*   io_ClearSupLst
*
*
* Function:
*   Free Sup lists.
* Description:
*   
*/
pwr_tStatus io_ClearSupLst (
  io_tSupCtx	Ctx
)
{
    sASupLstLink *ASupLstP;
    sASupLstLink *ANextP;
    sDSupLstLink *DSupLstP;
    sDSupLstLink *DNextP;
    sASupLstLink *TimerLstP;
    plc_sTimer	*TimerP;

    /* Reset all timers... */
    TimerLstP = Ctx->TimerLstP;
    while ( TimerLstP != NULL ) {
      TimerP = TimerLstP->TimerP;
      TimerP->TimerCount = 0;
      TimerP->TimerFlag = FALSE;      

      TimerLstP = TimerLstP->NextTimerP;
    } 
    Ctx->TimerLstP = NULL;


    /* Free DSup list */
    DSupLstP = Ctx->DSupDigLstP;
    Ctx->DSupDigLstP = NULL;
    while ( DSupLstP != NULL ) {
      DNextP = DSupLstP->NextP;
      free( (char *) DSupLstP);
      DSupLstP = DNextP;
    } 

    /* Free ASup list */
    ASupLstP = Ctx->ASupAnaLstP;
    Ctx->ASupAnaLstP = NULL;
    while ( ASupLstP != NULL ) {
      ANextP = ASupLstP->NextP;
      free( (char *) ASupLstP);
      ASupLstP = ANextP;
    }

    free(Ctx);

    return IO__SUCCESS;
} /* END io_ClearSupLst */

#if 0

static sASupLstLink *TimerSerialLstP = NULL;

static sASupLstLink *ASupAnaSerialLstP = NULL;
static sDSupLstLink *DSupDigSerialLstP = NULL;

static	pwr_tFloat32  TimerTimeSerial = 1.0;

static void ASup_exec_io_serial (
  sASupLstLink	*TimerLstP,
  pwr_sClass_ASup *o,
  pwr_tFloat32 In,
  pwr_tBoolean con
);
static void DSup_exec_io_serial (
  sDSupLstLink	*TimerLstP,
  pwr_sClass_DSup *o,
  pwr_tBoolean In,
  pwr_tBoolean con
);

/*------------------------------------------------------------------------------
* Detect an ASup object.
* NOTE:
*   This should be the same as the macro used by PLC,
*   in rplc_src:rt_plc_macro_sup.h  
*/
static void ASup_exec_io_serial (
  sASupLstLink	*TimerLstP,
  pwr_sClass_ASup *o,
  pwr_tFloat32 In,
  pwr_tBoolean con
)
{

  sASupLstLink *NextLstP, *NextP;	  /* Diff. */


  if ((o->High && In <= (o->CtrlLimit - o->Hysteres))
    || (!o->High && In >= o->CtrlLimit + o->Hysteres)
  ) {
    if (o->Action) o->Action = FALSE;
    if (o->ReturnCheck) {
      time_GetTime(&o->ReturnTime);
      o->ReturnCheck = FALSE;
      o->ReturnSend = TRUE;
    }
    if (o->AlarmCheck && !o->DetectCheck) {
      o->TimerFlag = FALSE;
      o->DetectCheck = TRUE;
    }
  } else if (con && ((o->High && In > o->CtrlLimit)
    || (!o->High && In < o->CtrlLimit))
  ) {
    if (!o->Action) o->Action = TRUE;
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
	o->ActualValue = In;

	/* timer_in(o); */
	o->TimerCount = o->TimerTime / TimerTimeSerial;
	if (!o->TimerFlag && o->TimerCount > 0) {
	  TimerLstP->TimerP = (plc_sTimer *)&o->TimerFlag;
	  if ( TimerSerialLstP == NULL ) {
	    TimerSerialLstP = TimerLstP;
	  }
	  else {
	    if ( TimerSerialLstP->NextTimerP == NULL ) {
	      TimerSerialLstP->NextTimerP = TimerLstP;
	    }             
	    else {
	      NextP = TimerSerialLstP->NextTimerP;
	      while ( NextP != NULL ) {
		NextLstP = NextP;
		NextP = NextP->NextTimerP;
	      } 
	      NextLstP->NextTimerP = TimerLstP;
	    }
	  }
          o->TimerFlag = TRUE;
        }

	time_GetTime(&o->DetectTime);
	o->DetectCheck = FALSE;
      }
      if (!o->TimerFlag) {
	o->DetectSend = TRUE;
	o->ReturnCheck = TRUE;
	o->Acked = FALSE;
	o->AlarmCheck = FALSE;
      }
    }
  }
}


/*------------------------------------------------------------------------------
* Detect an DSup object.
* NOTE:
*   This should be the same as the macro used by PLC,
*   in rplc_src:rt_plc_macro_sup.h  
*/
static void DSup_exec_io_serial (
  sDSupLstLink	*TimerLstP,
  pwr_sClass_DSup *o,
  pwr_tBoolean In,
  pwr_tBoolean con
)
{
  sDSupLstLink	*NextLstP, *NextP;	  /* Diff. */

  if (In != o->CtrlPosition) {
    if (o->Action) o->Action = FALSE;
    if (o->ReturnCheck) {
      time_GetTime(&o->ReturnTime);
      o->ReturnCheck = FALSE;
      o->ReturnSend = TRUE;
    }
    if (o->AlarmCheck && !o->DetectCheck) {
      o->TimerFlag = FALSE;
      o->DetectCheck = TRUE;
    }
  } else if (con) {
    if (!o->Action) o->Action = TRUE;
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
	o->ActualValue = In;

	/* timer_in(o); */

	o->TimerCount = o->TimerTime / TimerTimeSerial;
	if (!o->TimerFlag && o->TimerCount > 0) {
	  TimerLstP->TimerP = (plc_sTimer *)&o->TimerFlag;
	  if ( TimerSerialLstP == NULL ) {
	    TimerSerialLstP = (sASupLstLink *)TimerLstP;
	  }
	  else {
	    if ( TimerSerialLstP->NextTimerP == NULL ) {
	      TimerSerialLstP->NextTimerP = (sASupLstLink *)TimerLstP;
	    }             
	    else {
	      NextP = (sDSupLstLink *)TimerSerialLstP->NextTimerP;
	      while ( NextP != NULL ) {
		NextLstP = NextP;
		NextP = NextP->NextTimerP;
	      } 
	      NextLstP->NextTimerP = TimerLstP;
	    }
	  }
          o->TimerFlag = TRUE;
        }

	time_GetTime(&o->DetectTime);
	o->DetectCheck = FALSE;
      }
      if (!o->TimerFlag) {
	o->DetectSend = TRUE;
	o->ReturnCheck = TRUE;
	o->Acked = FALSE;
	o->AlarmCheck = FALSE;
      }
    }
  }
}

/*
* Name:    
*   io_ConnectToAnaSupLstSerial
*
*
* Function:
*   Initialize list of ASup object for serial connected analog in- och output 
*   signals.
* Description:
*   
*/
pwr_tStatus io_ConnectToAnaSupLstSerial (
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
)
{
    pwr_tStatus	  Sts;
    pwr_sClass_ASup *ASupP;
    pwr_sClass_Ai *AiP;
    pwr_sClass_Ao *AoP;
    pwr_tFloat32  *Float32P;
    pwr_tObjid	  SupId;
    pwr_tClassId  ObjClass;
    sASupLstLink  *ASupLstP;
    sASupLstLink  *NextASupLstP, *NextP;

    switch (Class) {
      case pwr_cClass_Ai:
        AiP = (pwr_sClass_Ai *) ObjP;
        Float32P = gdh_TranslateRtdbPointer( (pwr_tUInt32)AiP->ActualValue ); 
	break;

      case pwr_cClass_Ao:
        AoP = (pwr_sClass_Ao *) ObjP;
        Float32P = gdh_TranslateRtdbPointer( (pwr_tUInt32)AoP->ActualValue ); 
	break;

      default:
	return ( IO__SUCCESS );
	break;
    }


    /* Get child with class ASup */
    Sts = gdh_GetChild ( ObjId, &SupId);
    while ( ODD(Sts)) {
       Sts = gdh_GetObjectClass ( SupId, &ObjClass);  
       if ( EVEN(Sts)) return (Sts);

       if ( ObjClass == pwr_cClass_ASup ) {
         Sts = gdh_ObjidToPointer ( SupId, (pwr_tAddress *)&ASupP); 
 	 if (EVEN(Sts)) return ( Sts );

         ASupLstP = (sASupLstLink *) calloc( 1, sizeof(sASupLstLink) );
         ASupLstP->SupP = ASupP;
         ASupLstP->ValueP = Float32P;

         if ( ASupAnaSerialLstP == NULL ) {
           ASupAnaSerialLstP = ASupLstP;
         }
         else {
           if ( ASupAnaSerialLstP->NextP == NULL ) {
             ASupAnaSerialLstP->NextP = ASupLstP;
           }             
           else {
	     NextP = ASupAnaSerialLstP->NextP;
	     while ( NextP != NULL ) {
	       NextASupLstP = NextP;
	       NextP = NextP->NextP;
	     } 
             NextASupLstP->NextP = ASupLstP;
           }
	 }
       }

       Sts = gdh_GetNextSibling ( SupId, &SupId);
    }		

    return IO__SUCCESS;
} /* END io_ConnectToAnaSupLstSerial */


/*
* Name:    
*   io_ConnectToDigSupLstSerial
*
*
* Function:
*   Initialize list of DSup object for digitala in- och output signals.
* Description:
*   
*/
pwr_tStatus io_ConnectToDigSupLstSerial (
    pwr_tClassId 	Class,
    pwr_tObjid		ObjId,
    pwr_tAddress	ObjP	/* Pointer to the object */
)
{
    pwr_tStatus	  Sts;
    pwr_sClass_DSup *DSupP;
    pwr_sClass_Di *DiP;
    pwr_sClass_Do *DoP;
    pwr_sClass_Po *PoP;
    pwr_tBoolean  *BooleanP;
    pwr_tObjid	  SupId;
    pwr_tClassId  ObjClass;
    sDSupLstLink  *DSupLstP;
    sDSupLstLink  *NextDSupLstP, *NextP;
    

    switch (Class) {
      case pwr_cClass_Di:
        DiP = (pwr_sClass_Di *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (pwr_tUInt32)DiP->ActualValue ); 
	break;

      case pwr_cClass_Do:
        DoP = (pwr_sClass_Do *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (pwr_tUInt32)DoP->ActualValue ); 
	break;

      case pwr_cClass_Po:
        PoP = (pwr_sClass_Po *) ObjP;
        BooleanP = gdh_TranslateRtdbPointer( (pwr_tUInt32)PoP->ActualValue ); 
	break;

      default:
	return ( IO__SUCCESS );
	break;
    }


    /* Get child with class DSup */
    Sts = gdh_GetChild ( ObjId, &SupId);

    while ( ODD(Sts)) {
       Sts = gdh_GetObjectClass ( SupId, &ObjClass);  
       if ( EVEN(Sts)) return (Sts);

       if ( ObjClass == pwr_cClass_DSup ) {
         Sts = gdh_ObjidToPointer ( SupId, (pwr_tAddress *)&DSupP); 
 	 if (EVEN(Sts)) return ( Sts );

         DSupLstP = (sDSupLstLink *) calloc( 1, sizeof(sDSupLstLink) );
         DSupLstP->SupP = DSupP;
         DSupLstP->ValueP = BooleanP;
         
         if ( DSupDigSerialLstP == NULL ) {
           DSupDigSerialLstP = DSupLstP;
         }
         else {
           if ( DSupDigSerialLstP->NextP == NULL ) {
             DSupDigSerialLstP->NextP = DSupLstP;
           }             
           else {
	     NextP = DSupDigSerialLstP->NextP;
	     while ( NextP != NULL ) {
	       NextDSupLstP = NextP;
	       NextP = NextP->NextP;
	     } 
             NextDSupLstP->NextP = DSupLstP;
           }
	 }
       }

       Sts = gdh_GetNextSibling ( SupId, &SupId);
    }		

    return IO__SUCCESS;
} /* END io_ConnectToDigSupLstSerial */


/*
* Name:    
*   io_InitTimerSerial
*
*
* Function:
*   Initialize timer used for serial connected analog in- and outputs.
* Description:
*   
*/
pwr_tStatus io_InitTimerSerial (
      pwr_tFloat32  CycleTime
)
{
    if ( CycleTime > 0.0 )
      TimerTimeSerial = CycleTime;

    return IO__SUCCESS;
} /* END io_InitTimerSerial */


/*
* Name:    
*   io_ScanSupLstSerial
*
*
* Function:
*   Scan serial connected analog and digital in- and outputs supervise list.
* Description:
*   
*/
pwr_tStatus io_ScanSupLstSerial ()
{
    sASupLstLink *ASupLstP;
    sDSupLstLink *DSupLstP;


    /* Scan through the ASup list and execute ASup */
    ASupLstP = ASupAnaSerialLstP;
    while ( ASupLstP != NULL ) {
      ASup_exec_io_serial ( ASupLstP, ASupLstP->SupP, *ASupLstP->ValueP, TRUE );
      ASupLstP = ASupLstP->NextP;
    } 

    /* Scan through the DSup list and execute DSup */
    DSupLstP = DSupDigSerialLstP;
    while ( DSupLstP != NULL ) {
      DSup_exec_io_serial ( DSupLstP, DSupLstP->SupP, *DSupLstP->ValueP, TRUE );
      DSupLstP = DSupLstP->NextP;
    } 

    return IO__SUCCESS;
} /* END io_ScanSupLstSerial */


/*
* Name:    
*   io_ScanTimerLstSerial
*
*
* Function:
*   Scan serial connected analog and digital in- and outputs timer list.
* Description:
*   
*/
pwr_tStatus io_ScanTimerLstSerial ()
{
    sASupLstLink  *TimerLstP, *PrevP = NULL;
    plc_sTimer	*TimerP;
    sASupLstLink *NextTimerP;
 

    /* Scan through the timer list */
    TimerLstP = TimerSerialLstP;
    while ( TimerLstP != NULL ) {
       TimerP = TimerLstP->TimerP;
      if ( (TimerP->TimerCount <= 1) || !TimerP->TimerFlag ) {
        TimerP->TimerCount = 0;
        TimerP->TimerFlag = FALSE;      
        if ( PrevP != NULL )
          PrevP->NextTimerP = TimerLstP->NextTimerP;
        else
          TimerSerialLstP = TimerLstP->NextTimerP;
        NextTimerP = TimerLstP->NextTimerP;
        TimerLstP->NextTimerP = NULL;
        TimerLstP = NextTimerP;        
      }
      else
      {
        TimerP->TimerCount--;
        PrevP = TimerLstP;
        TimerLstP = TimerLstP->NextTimerP;
      }  
    } 

    return IO__SUCCESS;
} /* END io_ScanTimerLstSerial */
#endif
