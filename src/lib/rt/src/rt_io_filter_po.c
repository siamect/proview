/* 
 * Proview   $Id: rt_io_filter_po.c,v 1.2 2005-09-01 14:57:55 claes Exp $
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

/* rt_io_filter_po.c -- Runtime environment - I/O filter Po */

#include <stdio.h>
#include <stdlib.h>

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef pwr_baseclasses_h
#include "pwr_baseclasses.h"
#endif

#ifndef rt_gdh_h
#include "rt_gdh.h"
#endif

#ifndef rt_io_msg_h
#include "rt_io_msg.h"
#endif

#ifndef rt_io_filter_po_h
#include "rt_io_filter_po.h"
#endif


/* Defines */
#define _MaxNoOfPo	16 


/* Structures */
typedef struct s_FilterData sFilterData;


/*
* Name:    
*   _eState
* Description:
*   The different states used in filter
*/
enum e_State
    {
    _eState__		= 0,
    _eState_Neutral	= 1,
    _eState_Pulse	= 2,
    _eState_
    };

/* Enumerations */
typedef enum e_State _eState;

/*
* Name:    
*   sFilterData
* Description:
*   The structure of filter data used in filter
*/
struct s_FilterData
    {
    _eState	  Type;
    pwr_tFloat32  ScanTime;
    pwr_tUInt32	  TimerPulse;
    pwr_tBoolean  OldValue;
    pwr_tBoolean  *ActualValue;
    };



/*
* Name:    
*   io_FilterPo
*
*
* Function:
*   Filtration of Po.
* Description:
*   
*/
pwr_tStatus io_PoFilter
    (
    pwr_sClass_Po   *SignalObj[],
    pwr_tUInt16	    *Data,
    void	    *FilterData[]
    )
{
    pwr_tUInt16	Mask, ResetMask, OldData;
    pwr_tUInt16	InvMask = 65535;
    int Idx;
    sFilterData *Data_1;
	

    /* Save data local */
    OldData = *Data;

    /* Scan through the signals and do the filtration */

    for ( Mask = 1, Idx = 0; Idx < _MaxNoOfPo; Idx++, Mask <<= 1) {
       if ( SignalObj[Idx] == NULL || 
            FilterData[Idx] == NULL ) continue;
       Data_1 = ( sFilterData *)FilterData[Idx];
       ResetMask = 0;
       switch ( Data_1->Type ) {

       case _eState_Neutral:
	   if ( SignalObj[Idx]->PosPulse == TRUE ) {	/* Pos. pulse out */
	     if ( SignalObj[Idx]->PosFlank == TRUE ) {	/* Trigg pos. flank */
	       if ( (pwr_tBoolean) (( *Data & Mask ) != 0) == TRUE &&
		    Data_1->OldValue == FALSE )	{ /* Pos. flank detected */
		 if ( SignalObj[Idx]->ResetActualValue == TRUE )
		   *Data_1->ActualValue = FALSE;
                 if ( SignalObj[Idx]->PulseLength > 0.0 ) {
		   Data_1->TimerPulse  = SignalObj[Idx]->PulseLength/Data_1->ScanTime + 0.5;
		   Data_1->TimerPulse--;
		   Data_1->Type = _eState_Pulse;
		   break;
                 }
               }
             }
	     else {					/* Trigg neg. flank */	  
	       if ( (pwr_tBoolean) (( *Data & Mask ) != 0) == FALSE &&
		    Data_1->OldValue == TRUE ) { /* Neg. flank detected */
		 if ( SignalObj[Idx]->ResetActualValue == TRUE )
		   *Data_1->ActualValue = TRUE;
                 if ( SignalObj[Idx]->PulseLength > 0.0 ) {
		   Data_1->TimerPulse  = SignalObj[Idx]->PulseLength/Data_1->ScanTime + 0.5;
		   Data_1->TimerPulse--;
		   Data_1->Type = _eState_Pulse;
		   *Data |= Mask;	      /* Set bit */
		   break;
                 }
               }
             }
             ResetMask = Mask ^ InvMask;
             *Data &= ResetMask;      /* Reset bit */
           }
           else {					/* Neg. pulse out */
	     if ( SignalObj[Idx]->PosFlank == TRUE ) {	/* Trigg pos. flank */
	       if ( (pwr_tBoolean) (( *Data & Mask ) != 0) == TRUE &&
		    Data_1->OldValue == FALSE ) { /* Pos. flank detected */
		 if ( SignalObj[Idx]->ResetActualValue == TRUE )
		   *Data_1->ActualValue = FALSE;
                 if ( SignalObj[Idx]->PulseLength > 0.0 ) {
		   Data_1->TimerPulse  = SignalObj[Idx]->PulseLength/Data_1->ScanTime + 0.5;
		   Data_1->TimerPulse--;
		   Data_1->Type = _eState_Pulse;
		   *Data ^= Mask;	      /* Invert bit */
		   break;
		 }
               }
             }
	     else {					/* Trigg neg. flank */	  
	       if ( (pwr_tBoolean) (( *Data & Mask ) != 0) == FALSE &&
		    Data_1->OldValue == TRUE ) { /* Neg. flank detected */
		 if ( SignalObj[Idx]->ResetActualValue == TRUE )
		   *Data_1->ActualValue = TRUE;
                 if ( SignalObj[Idx]->PulseLength > 0.0 ) {
		   Data_1->TimerPulse  = SignalObj[Idx]->PulseLength/Data_1->ScanTime + 0.5;
		   Data_1->TimerPulse--;
		   Data_1->Type = _eState_Pulse;
		   break;
                 }
               }
             }
             *Data |= Mask;	      /* Set bit */
           }
           break;

       case _eState_Pulse:
	   if ( Data_1->TimerPulse > 0 ) {		  /* Timer on */
	     Data_1->TimerPulse--;
             if ( SignalObj[Idx]->PosPulse == TRUE )	  /* Pos. pulse out */
               *Data |= Mask;	      /* Set bit */
             else {
               ResetMask = Mask ^ InvMask;
               *Data &= ResetMask;      /* Reset bit */
             }
           }
           else {
	     Data_1->Type = _eState_Neutral;
             if ( SignalObj[Idx]->PosPulse == TRUE ) {    /* Pos. pulse out */
               ResetMask = Mask ^ InvMask;
               *Data &= ResetMask;      /* Reset bit */
             }
             else 
               *Data |= Mask;	      /* Set bit */
           }
#if 0	/** 950208/IL **/
	   if ( SignalObj[Idx]->ResetActualValue == TRUE ) {
	     if ( SignalObj[Idx]->PosPulse == TRUE )	  /* Pos. pulse out */
	       *Data_1->ActualValue = FALSE;
             else
	       *Data_1->ActualValue = TRUE;
           } 
#endif
           break;
   
       default:
	   break;

       }
       Data_1->OldValue = (pwr_tBoolean) ((OldData & Mask) != 0);
    }

    return IO__SUCCESS;
} /* END io_FilterPo */


/*
* Name:    
*   io_InitFilterPo
*
*
* Function:
*   Initialize filter for max 16 Po ( one Do-card ).
* Description:
*   
*/
pwr_tStatus io_InitPoFilter
    (
    pwr_sClass_Po   *SignalObj[],
    pwr_tBoolean    *Filter,
    void	    *FilterData[],
    pwr_tFloat32    ScanTime
    )
{
    sFilterData    *Data_1;
    int	    Idx;

    *Filter = FALSE;			  /* Supose no signals with filter */

    for ( Idx = 0; Idx < _MaxNoOfPo; Idx++) {
       if ( SignalObj[Idx] == NULL ) continue;

       if ( ScanTime > 0 ) {
	 Data_1 = (sFilterData *) malloc( sizeof(sFilterData) );
	 Data_1->ScanTime  = ScanTime;
	 Data_1->TimerPulse  = SignalObj[Idx]->PulseLength/Data_1->ScanTime + 0.5;
	 Data_1->ActualValue = gdh_TranslateRtdbPointer((pwr_tUInt32)SignalObj[Idx]->ActualValue);
	 Data_1->OldValue  = *Data_1->ActualValue;
	 Data_1->Type  = _eState_Neutral;
	 FilterData[Idx] = Data_1; 
	 *Filter = TRUE; 
       }
    }

    return IO__SUCCESS;
} /* END io_InitFilterPo */


/*
* Name:    
*   io_CloseFilterPo
*
*
* Function:
*   Close filter for max 16 Po.
* Description:
*   
*/
void io_ClosePoFilter
    (
    void	    *FilterData[]
    )
{
    int	i;

    for ( i = 0; i < _MaxNoOfPo; i++) {
      if ( FilterData[i] != NULL )
        free( FilterData[i]);
    }
}

