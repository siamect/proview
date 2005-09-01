/* 
 * Proview   $Id: rt_io_dioc.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_io_dioc.c 
   This file contains routines that handles communications
   with rtp_dioc rack interfaces.  */

#include stdio
#include $vaxelnc
#include $elnmsg
#include $kernelmsg

#include "pwr.h"

#include "co_cdh.h"

#include "rt_gdh_msg.h"
#include "rt_gdh.h"
#include "rt_io_msg.h"
#include "rt_io_base.h"
#include "rt_io_lat.h"
#include "rt_io_dioc.h"
#include "rt_io_reverseword.h"
#include "rt_io_supervise.h"
#include "rt_io_convert.h"


/* This structure is used to generate a temporary list that decribes the
   collections of cards and channels before creating the final list	*/

typedef struct s_dioctmps {
  pwr_tObjid		  Card;
  pwr_tObjid		  Channel;
  pwr_tClassId		  CardType;
  pwr_tClassId		  ChanType;
  DIOC_LISTTYP_ENUM	  IOType;
  struct s_dioctmps	  *Next;
} _DIOC_TEMPSTRUCT;
		  

/* this is a struct used to generate some tables */

typedef struct {
  pwr_tClassId		ObjClass;
  DIOC_LISTTYP_ENUM	IOType;
} _OBJTYPESTR;

typedef union {
  pwr_sClass_Line_LAT  *LATLinePtr;
} _DIOC_LINE_UNION;


/* This struct is used to describe a DIOC */

typedef struct s_DiocDevStr {
  pwr_sClass_RTP_DIOC	  *DiocObjPtr;
  struct s_DiocDevStr	  *Next;
  int			  NoOfScanLists;
  pwr_tFloat32		  Divider;
  DIOC_SCAN_LIST_DESCR	  *ScanLists;
} _DIOC_DEV_STRUCT;


/* This struct is used to describe a line with it's connected 
   Diocs */

typedef struct s_DiocLineStr {
  pwr_tClassId		LineType;
  _DIOC_LINE_UNION	ObjPtr;
  pwr_tObjid		LineObjId;
  PORT			DDAPort;
  PROCESS		LineProcess;
  int			NoOfDiocs;
  struct s_DiocLineStr  *NextLine;
  _DIOC_DEV_STRUCT	*DiocList;
} _DIOC_LINE_STRUCT;


/* This is the structure that is the root handle to it all */

typedef struct {
  int			NoOfLines;
  _DIOC_LINE_STRUCT	*FirstLine;
} _DIOC_ROOT_STRUCT ;


typedef union {
  pwr_tAddress		    Ptr;
  pwr_sClass_ChanAi	    *AiChanPtr;
  pwr_sClass_ChanAit	    *AitChanPtr;
  pwr_sClass_ChanAo	    *AoChanPtr;
  pwr_sClass_ChanDi	    *DiChanPtr;
  pwr_sClass_ChanDo	    *DoChanPtr;
} _DIOC_FIX_UNION;
		 

typedef struct s_DiocTempIOList {
  int			  RackAddr;
  int			  NoOfAi;
  int			  NoOfDi;
  int			  NoOfDoAo;
  _DIOC_TEMPSTRUCT	  *IOList;
  struct s_DiocTempIOList *Next;
} _DIOC_TEMP_RACK_LIST;

extern EVENT	    io_comm_terminate;

static const LARGE_INTEGER NoWait = { 0,0 };

static _DIOC_ROOT_STRUCT    DiocRootHandle = {0,NULL};

/* Here to hold pointers to the value base objects */

static pwr_sClass_AiArea	    *aiap = NULL;
static pwr_sClass_AoArea	    *aoap = NULL;
static pwr_sClass_DiArea	    *diap = NULL;
static pwr_sClass_DoArea	    *doap = NULL;
static pwr_sClass_CoArea	    *coap = NULL;
static pwr_sClass_CaArea	    *caap = NULL;

static io_tSupCtx	SupCtx;		/* Context for supervise object lists */

static pwr_tUInt16
io_get_rtp_ai_gain (
  pwr_tFloat32	Range
)
{
  typedef struct {
    pwr_tFloat32  UpperLim,LowerLim;
    pwr_tUInt16   GainMask;
  } GAINSTR;

  static GAINSTR GainTable[] = {
    {10.5,   9.5,    0},	/* 10V */
    { 5.5,   4.5,    1},	/* 5V  */
    { 2.8,   1.8,    2},	/* 2.5V*/
    { 1.5,   0.95,   3},	/* 1.25V*/
    { 0.8,   0.5,    4},	/* 0.64V*/
    { 0.4,   0.25,   5},	/* 0.32V*/
    { 0.2,   0.15,   6},	/* 0.16V*/
    { 0.1,   0.07,   7},	/* 0.08V (gain 7 and 8 the same !!)*/
    { 0.05,  0.032,  9},	/* 0.04V */
    { 0.03,  0.016, 10},	/* 0.02V */
    { 0.014, 0.005, 11}		/* 0.01V */
  };

  static int GainTblSize = sizeof(GainTable) / sizeof(GainTable[0]);
  int	 Index;
  pwr_tUInt16 RetVal;

  RetVal = 0;	/* Return zero if anything goes wrong */

  if ((Range < 11.0) && (Range >= 0.0))
    for (Index = 0; Index < GainTblSize; Index++)
      if ((Range >= GainTable[Index].LowerLim) && 
	  (Range <= GainTable[Index].UpperLim)) 
      {
	RetVal = GainTable[Index].GainMask;
	break;
      }

  return RetVal; 
}

/************************************************************************
*
* Name: InitValueBasePointers()		
*
* Type:	void	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description: This routine fixes the pointers to the Value Base objects
*	
*************************************************************************/

static void InitValueBasePointers()
{
    pwr_tUInt32	Sts;
    pwr_tObjid	ObjId;

	Sts = gdh_GetClassList(pwr_cClass_AiArea,
			       &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &aiap);  
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);		
		   if (EVEN(Sts))
			break;
		   }
		}
	    }

	Sts = gdh_GetClassList(pwr_cClass_AoArea, &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &aoap);  
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);		
		   if (EVEN(Sts))
			break;
		   }
		}
	    }

	Sts = gdh_GetClassList(pwr_cClass_DiArea, &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &diap);  
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);
		   if (EVEN(Sts))
			break;
		   }
		}
	    }

	
	Sts = gdh_GetClassList(pwr_cClass_DoArea, &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &doap);
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);
		   if (EVEN(Sts))
			break;
		   }
		}
	    }


	Sts = gdh_GetClassList(pwr_cClass_CoArea, &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &coap);
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);
		   if (EVEN(Sts))
			break;
		   }
		}
	    }

	Sts = gdh_GetClassList(pwr_cClass_CaArea, &ObjId);

	if (ODD(Sts))
            {
            for (;;)
	        {  
	        Sts = gdh_ObjidToPointer(ObjId, &caap);
	    
	        if (Sts != GDH__REMOTE)
	            break;
	        else
		   {
		   Sts = gdh_GetNextObject(ObjId, &ObjId);
		   if (EVEN(Sts))
			break;
		   }
		}
	    }



} /* END InitValueBasePointers */

/************************************************************************
*
* Name:	SignalConToPointer(ChanObjPtr,ChanObjClass)	
*
* Type:	pwr_tAddress	(Pointer to a signal object)
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* pwr_tAddress	ChanObjPtr	I	Pointer to a channel object
* pwr_tClassId	ChanObjClass	I	Type of the object whose pointer we
*					got
*
* Description:	This routine translates a signal connection to an address
*		NB ! This routine is written to compensate for any 
*		discrepancies in the difrent object definitions, some parts
*		of the io system relies on the fact that the parameters for
*		the diffrent signal objects are specified in the same way.
*		This means that although this routine is more robust, it must
*		be updated for every new signal/channel  object[type] pair.
*************************************************************************/

static pwr_tAddress SignalConToPointer(pwr_tAddress   ChanObjPtr,
				  pwr_tClassId     ChanObjClass)

{

    _DIOC_FIX_UNION	FixIt;
    pwr_tAddress	RetVal = NULL;
    pwr_tObjid		SigConId;
    pwr_tUInt32		Sts;
    pwr_tClassId 	Class;


    if (ChanObjPtr == NULL)
	return NULL;

    FixIt.Ptr = ChanObjPtr;

    switch (ChanObjClass) {
				
    case pwr_cClass_ChanAit:

	SigConId = FixIt.AitChanPtr->SigChanCon;
	break;

    case pwr_cClass_ChanAi:

	SigConId = FixIt.AiChanPtr->SigChanCon;
	break;

    case pwr_cClass_ChanAo:

	SigConId = FixIt.AoChanPtr->SigChanCon;
	break;

    case pwr_cClass_ChanDi:

	SigConId = FixIt.DiChanPtr->SigChanCon;
	break;

    case pwr_cClass_ChanDo:

	SigConId = FixIt.DoChanPtr->SigChanCon;
	break;

    default:
	return NULL;	/* If none of the above return NULL */    	

    }  /* End Case */

    Sts = gdh_GetObjectClass(SigConId, &Class);
    Sts = gdh_ObjidToPointer(SigConId, &RetVal);

    if (EVEN(Sts))	/* If error return NULL*/
	return NULL;	
    else {
	Sts = io_ConnectToSupLst( SupCtx, Class, SigConId, RetVal);
	return RetVal;
    }

} /* END SignalConToPointer */

/************************************************************************
*
* Name:	FindParentLine(DiocObjId)	
*
* Type:	static pwr_tObjid	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* pwr_tObjid		DiocObjId	I	Objid of the dioc object whose
*					parent Line we would like to find
*
* The function returns 0 if no valid parent found
*
* Description:	This function finds the appropriate line object that is the 
*		parent of the dioc object. As of now the only allowed 
*		line object is the LAT_line (920202)
*************************************************************************/

static pwr_tObjid FindParentLine(pwr_tObjid DiocObjId)


{
static pwr_tClassId ValidObjTypes[] = {pwr_cClass_Line_LAT};
static int	MaxValidObjTypes = (sizeof(ValidObjTypes)/
				    sizeof(ValidObjTypes[0]));


    pwr_tObjid   ParentId;
    pwr_tClassId  ParentType;
    pwr_tUInt32  Sts;
    int	    Index;
    pwr_tBoolean Found;

    Sts = gdh_GetParent(DiocObjId,
			&ParentId);

    if (EVEN(Sts))
	return pwr_cNObjid;

    Sts = gdh_GetObjectClass(ParentId,
			    &ParentType);

    if (EVEN(Sts))
	return pwr_cNObjid;

    
    for (Index = 0, Found = FALSE; Index < MaxValidObjTypes; Index++)
	if (ValidObjTypes[Index] == ParentType)
	    {
	    Found = TRUE;
	    break;
	    }

    if (Found)
	return ParentId;
    else
	return pwr_cNObjid;

} /* END FindParentLine */

/************************************************************************
*
* Name: InsertLine(ParentLine)		
*
* Type:	void	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* pwr_tObjid		ParentLine	I	Objid of the parent line
*
* Description:	
*************************************************************************/

static void InsertLine(pwr_tObjid ParentLine)

{
    _DIOC_LINE_STRUCT	*LinePtr;
    pwr_tUInt32		Sts;

    if (DiocRootHandle.FirstLine == NULL)  /* If no line exists */
	{
	LinePtr = calloc(1,sizeof(_DIOC_LINE_STRUCT));
	DiocRootHandle.FirstLine = LinePtr;
	}
    else
	{
	for (LinePtr = DiocRootHandle.FirstLine ;; LinePtr = LinePtr->NextLine) 
	    {
	    if (cdh_ObjidIsEqual(LinePtr->LineObjId, ParentLine))
		break;
	    else if (LinePtr->NextLine == NULL)
		{
		LinePtr->NextLine = calloc(1,sizeof(_DIOC_LINE_STRUCT));
		LinePtr = LinePtr->NextLine;
		break;
		}
	    }
	}

    if (LinePtr->LineType == pwr_cNClassId)
	{
	Sts = gdh_GetObjectClass(ParentLine,
			        &LinePtr->LineType);
	LinePtr->LineObjId = ParentLine;
	Sts = gdh_ObjidToPointer(ParentLine,
				 &LinePtr->ObjPtr.LATLinePtr);
	}
 
	       	
} /* END InsertLine */

/************************************************************************
*
* Name:	BuildLineList()
*
* Type:	pwr_tUInt32 
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description: This routine generatres the parts of the list that describes
*	       the LINE <-> Dioc relationship	
*************************************************************************/

pwr_tUInt32 BuildLineList()


{
    pwr_tObjid  DiocObj;
    pwr_tObjid  ParentLine;
    pwr_tUInt32 Sts;
    int         ObjCounter = 0;

    Sts = gdh_GetClassList(pwr_cClass_RTP_DIOC,
			  &DiocObj);

    for (; (ODD(Sts));)
	{
        ObjCounter++;
	ParentLine = FindParentLine(DiocObj);
	if (cdh_ObjidIsNotNull(ParentLine))	    /* If there exists an parent line */
	    InsertLine(ParentLine);
        else
	    errh_CErrLog(IO__NODIOCANCESTOR, NULL);
	Sts = gdh_GetNextObject(DiocObj,
				&DiocObj);
	}
 
    if (Sts == GDH__NO_TYPE)
    {
      if (ObjCounter == 0)
        errh_CErrLog(IO__NODIOCOBJECTS,errh_ErrArgMsg(Sts), NULL);
      return GDH__SUCCESS;
    }
    else
	return Sts;

} /* END BuildLineAndDiocList */

/************************************************************************
*
* Name:	FindChannelsInRack(RackObjId,InChannels,OutChannels);	
*
* Type:	pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* pwr_tObjid		RackObjId	I	The objid of the rack to count channel
*					object on.
* pwr_tInt32		*DIChannels	IO	Pointer to a value to which we ADD
*					the number of input channels configured
*					NB! We add the number, therefore it
*					is the calling routines responsibility
*					to supply an appropriate value (i.e. 0)
*
* pwr_tInt32		*AIChannels	IO	See DIChannels
*					
* pwr_tInt32		*OutChannels	IO	See DIChannels
*
* _DIOC_TEMPSTRUCT **TempStrPtr	IO	A pointer to a pointer that receives
*					the pointer to a  
*
* Description:	Count the number of configured IO channels in a rack and 
*		ADD the value to the supplied value.
*
*		This routine makes some assumptions 'bout the relationship
*		between Racks, Cards and Channels (for rtp of course:
*		
*		1. All cards are childen to the supplied rack, and the rack
*		   is the parent (not grand parent or worse [sorry!]) of all
*		   the cards.
*
*		2. This implies that cards cannot have another cards as their
*		   parents.
*
*		3. All channels must have a card as their parent, and no 
*		   other channels as descendants.
*
*		NB ! This could possibly be implemented better recursively
*		     but I haven't got the time to do it right now
*
*************************************************************************/

static pwr_tInt32 FindChannelsInRack(pwr_tObjid		    RackObjId,
				pwr_tInt32		    *DIChannels,
				pwr_tInt32		    *AIChannels,
				pwr_tInt32		    *OutChannels,
				_DIOC_TEMPSTRUCT    **TempStrPtr)
    

{


    static _OBJTYPESTR ValidCardTypes[] = 
	    {{pwr_cClass_Di_7437_37,_eDiocIOLtype_di},
	     {pwr_cClass_Do_7437_83,_eDiocIOLtype_do},
	     {pwr_cClass_Ai_7436,_eDiocIOLtype_ai},
	     {pwr_cClass_Ao_7455_20,_eDiocIOLtype_ao},
	     {pwr_cClass_Ao_7455_30,_eDiocIOLtype_ao},
	     {pwr_cClass_Pd_7435_26,_eDiocIOLtype_ao},
	     {pwr_cClass_Co_7435_33,_eDiocIOLtype_di},
	     {pwr_cClass_Co_7437_33,_eDiocIOLtype_di},
	     {0,_eDiocIOLtype_illegal}};

    static _OBJTYPESTR ValidChannelTypes[] = 
	     {{pwr_cClass_ChanDi,_eDiocIOLtype_di},
	      {pwr_cClass_ChanDo,_eDiocIOLtype_do},
	      {pwr_cClass_ChanAi,_eDiocIOLtype_ai},
	      {pwr_cClass_ChanAit,_eDiocIOLtype_ai},
	      {pwr_cClass_ChanAo,_eDiocIOLtype_ao},
	      {pwr_cClass_ChanCo,_eDiocIOLtype_di},
#if 0
	      {pwr_cClass_ChanCot,_eDiocIOLtype_di},
#endif
	      {0,_eDiocIOLtype_illegal}};



    pwr_tObjid	CardId;
    pwr_tObjid	ChannelId;
    pwr_tInt32	Sts;
    int		Index;
    int		Index2;
    pwr_tClassId CardType;
    pwr_tClassId ChanType;
    DIOC_LISTTYP_ENUM CardIOtyp;
    DIOC_LISTTYP_ENUM ChannelIOtyp;
    _DIOC_TEMPSTRUCT	*FirstPtr = NULL;
    _DIOC_TEMPSTRUCT	*NextPtr = NULL;

    Sts = gdh_GetChild(RackObjId,
		       &CardId);

    if (EVEN(Sts))
	{
	if (Sts == GDH__NO_CHILD)
	    return GDH__SUCCESS;
	else
	    return Sts;
	}

    /* Scan all cards and all channels */

    do  {
	Sts = gdh_GetObjectClass(CardId,	    /* Look the card type up */
				&CardType);

	for (Index = 0, CardIOtyp = _eDiocIOLtype_illegal; 
	     ValidCardTypes[Index].ObjClass != 0; 
	     Index++)
	     if (ValidCardTypes[Index].ObjClass  ==  CardType)
		{
		CardIOtyp = ValidCardTypes[Index].IOType;
		break;
		}

	/* Check if this card is valid, else do something */

	switch (CardIOtyp) {

	case  _eDiocIOLtype_ai:
	case  _eDiocIOLtype_ao:
	    
	    Sts = gdh_GetChild(CardId,
			       &ChannelId);  
	    if ((Sts != GDH__NO_CHILD) && (ODD(Sts)))
		{
		do  {
		    /* Check that we got something legal here i e a
		       valid channel */

		    Sts = gdh_GetObjectClass(ChannelId,	    /* Look the card type up */
					    &ChanType);
		    
		    for (Index2 = 0, ChannelIOtyp = _eDiocIOLtype_illegal;
			 ValidChannelTypes[Index2].ObjClass != 0;
			 Index2++)
			 if (ValidChannelTypes[Index2].ObjClass == ChanType)
			    {
			    ChannelIOtyp = ValidChannelTypes[Index2].IOType;
			    break;
			    }

		    /* Check if the found channel objects ok */
		    
		    if (ChannelIOtyp != _eDiocIOLtype_illegal)
			{
			/* Check so that both card and channel are of the 
			   same kind (input/output only) */

			if (ChannelIOtyp == CardIOtyp)
			    {
			    NextPtr = calloc(1,sizeof(_DIOC_TEMPSTRUCT));
			    NextPtr->Card = CardId;
			    NextPtr->Channel = ChannelId;
			    NextPtr->IOType = ChannelIOtyp;
			    NextPtr->CardType = CardType;
			    NextPtr->ChanType = ChanType;
			    if (FirstPtr == NULL)
			       {
			       FirstPtr = NextPtr;
			       *TempStrPtr = FirstPtr;
			       }
			    else
			       {
			       FirstPtr->Next = NextPtr;
			       FirstPtr = NextPtr;
			       }
		      	   if (ChannelIOtyp == _eDiocIOLtype_ai)
			      (*AIChannels)++;
			   else
			      (*OutChannels)++;
			   }
		       }

		    Sts = gdh_GetNextSibling(ChannelId,
					     &ChannelId);
		    }
		while (Sts != GDH__NO_SIBLING);
		}
		break;

	case  _eDiocIOLtype_di:
	case  _eDiocIOLtype_do:

		
	    NextPtr = calloc(1,sizeof(_DIOC_TEMPSTRUCT));
	    NextPtr->Card = CardId;
	    NextPtr->Channel = pwr_cNObjid;
	    NextPtr->IOType = CardIOtyp;
	    NextPtr->CardType = CardType;
	    if (FirstPtr == NULL)
	       {
	       FirstPtr = NextPtr;
	       *TempStrPtr = FirstPtr;
	       }
	    else
	       {
	       FirstPtr->Next = NextPtr;
	       FirstPtr = NextPtr;
	       }
		    
      	   if (CardIOtyp == _eDiocIOLtype_di)
	      (*DIChannels)++;
	   else
	      (*OutChannels)++;
	   break;

	} /* end case */
	Sts = gdh_GetNextSibling(CardId,
				 &CardId);
        }
    while (Sts != GDH__NO_SIBLING);
    return 1; 
} /* END FindChannelsInRack */

/************************************************************************
*
* Name:	CreateRackScanLists(TempStr,InList,OutList)	
*
* Type:	void	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* _DIOC_TEMPSTRUCT *TempStr	I	The list that should contain the 
*					entries
*
* DIOC_SCAN_BUFFER  *DIList	I	Pointer to a scan buffer for digital 
*					input signals
*
* DIOC_SCAN_BUFFER  *AIList	I	Pointer to a scan buffer for analog
*					input signals
*
* DIOC_SCAN_BUFFER  *OutList	I	Pointer to a scan buffer for output
*					signals
*  
* Description: This is a routine that 
*************************************************************************/

void CreateRackScanLists(int			RackAddr,
			 _DIOC_TEMPSTRUCT	*TempStr,
			 DIOC_SCAN_BUFFER	*DIList,
			 DIOC_DESC_LIST_ENTRY	DIDescList[],
			 int			DIIndex,
			 DIOC_SCAN_BUFFER	*AIList,
			 DIOC_DESC_LIST_ENTRY	AIDescList[],
			 int			AIIndex,
			 DIOC_SCAN_BUFFER	*OutList,
			 DIOC_DESC_LIST_ENTRY	OutDescList[],
			 int			OutIndex)
{
    pwr_tUInt32  Sts;

    
    /* Loop through all entries in the list that TempStr points to 
       and build the lists */

    while (TempStr != NULL)
	{
	switch(TempStr->IOType) {

	case _eDiocIOLtype_di:

	    /* Get pointers to card object */

	    Sts = gdh_ObjidToPointer(TempStr->Card,
				     &DIDescList[DIIndex].Data.Di.CardPtr);

	    switch (TempStr->CardType) {

    
	    case pwr_cClass_Di_7437_37:

		DIDescList[DIIndex].EntryType = _eDiocLEtype_di;
		break;

	    case pwr_cClass_Co_7435_33:

		if (TempStr->ChanType == pwr_cClass_ChanCo)
		    DIDescList[DIIndex].EntryType = _eDiocLEtype_co1;
		else
		    DIDescList[DIIndex].EntryType = _eDiocLEtype_co1t;
		break;

	    case pwr_cClass_Co_7437_33:
	    
		if (TempStr->ChanType == pwr_cClass_ChanCo)
		    DIDescList[DIIndex].EntryType = _eDiocLEtype_co4;
		else
		    DIDescList[DIIndex].EntryType = _eDiocLEtype_co4t;
		break;

		} /* end case */

	    /* Build the scan list entry */

	    DIList->Entries[DIIndex].Crc16 = 0; /* Clear all bits */
	    DIList->Entries[DIIndex].IOEntry.CardAddr = 
		DIDescList[DIIndex].Data.Di.CardPtr->CardAddress;
	    DIList->Entries[DIIndex].IOEntry.CtrlAddr = RackAddr; 


	    DIIndex++;
	    break;

	case _eDiocIOLtype_ai:

	    /* Get pointers to card and channel object */

	    Sts = gdh_ObjidToPointer(TempStr->Card,
				     &AIDescList[AIIndex].Data.Ai.CardPtr);

	    Sts = gdh_ObjidToPointer(TempStr->Channel,
				     &AIDescList[AIIndex].Data.Ai.ChanPtr);
	    
	    
	    AIDescList[AIIndex].Data.Ai.SigPtr = 
		 SignalConToPointer(AIDescList[AIIndex].Data.Ai.ChanPtr,
				    TempStr->ChanType);

	    if (TempStr->ChanType == pwr_cClass_ChanAi)
		AIDescList[AIIndex].EntryType = _eDiocLEtype_ai;
	    else
		AIDescList[AIIndex].EntryType = _eDiocLEtype_ait;

	    /* Build the scan list entry */

	    AIList->Entries[AIIndex].Crc16 = 0; /* Clear all bits */
	    AIList->Entries[AIIndex].AIEntry.CardAddr = 
		AIDescList[AIIndex].Data.Ai.CardPtr->CardAddress;
	    if (AIDescList[AIIndex].Data.Ai.CardPtr->DiffInput)
		{
		AIList->Entries[AIIndex].AIEntry.Diff = 0; /* 0 = Diffrential input !!*/
		AIList->Entries[AIIndex].AIEntry.OddChan = 0;
		AIList->Entries[AIIndex].AIEntry.Channel = 
		  AIDescList[AIIndex].Data.Ai.ChanPtr->Number;
		}
	    else
		{
		AIList->Entries[AIIndex].AIEntry.Diff = 1; 
		AIList->Entries[AIIndex].AIEntry.OddChan = 
		  (AIDescList[AIIndex].Data.Ai.ChanPtr->Number & 1);
		AIList->Entries[AIIndex].AIEntry.Channel = 
		  (AIDescList[AIIndex].Data.Ai.ChanPtr->Number >> 1);
		} 

	    AIList->Entries[AIIndex].AIEntry.GainSel = 
		io_get_rtp_ai_gain(AIDescList[AIIndex].Data.Ai.CardPtr->VoltageRange);
	    AIIndex++;
	    break;

	case _eDiocIOLtype_do:
	case _eDiocIOLtype_ao:

	    /* Get pointers to card and channel object */

	    Sts = gdh_ObjidToPointer(TempStr->Card,
				     &OutDescList[OutIndex].Data.Ai.CardPtr);

	    OutList->Entries[OutIndex].Crc16 = 0; /* Clear all bits */

      	    if  (TempStr->IOType == _eDiocIOLtype_ao)
		{
	        Sts = gdh_ObjidToPointer(TempStr->Channel,
				         &OutDescList[OutIndex].Data.Ai.ChanPtr);
	    	    
	        OutDescList[OutIndex].Data.Ai.SigPtr = 
		         SignalConToPointer(OutDescList[OutIndex].Data.Ai.ChanPtr,
				            TempStr->ChanType);
                }

	    switch (TempStr->CardType) {

	    case pwr_cClass_Ao_7455_20:

		OutDescList[OutIndex].EntryType = _eDiocLEtype_ao1;
		OutList->Entries[OutIndex].IOEntry.CardAddr = 
		    OutDescList[OutIndex].Data.Ao1.CardPtr->CardAddress;
		break;

	    case pwr_cClass_Ao_7455_30:

		OutDescList[OutIndex].EntryType = _eDiocLEtype_ao4;
		OutList->Entries[OutIndex].IOEntry.CardAddr = 
		    OutDescList[OutIndex].Data.Ao4.CardPtr->CardAddress;
		break;

	    case pwr_cClass_Do_7437_83:

		OutDescList[OutIndex].EntryType = _eDiocLEtype_do;
		OutList->Entries[OutIndex].IOEntry.CardAddr = 
		    OutDescList[OutIndex].Data.Do.CardPtr->CardAddress;
		break;

	    case pwr_cClass_Pd_7435_26:

		OutDescList[OutIndex].EntryType = _eDiocLEtype_pd;
		OutList->Entries[OutIndex].IOEntry.CardAddr = 
		    OutDescList[OutIndex].Data.Pd.CardPtr->CardAddress;
		break;
	    }

	    /* Build the scan list entry */

	    OutList->Entries[OutIndex].IOEntry.CtrlAddr = RackAddr; 
	    OutIndex++;
	    break;
	    
	default:
	    break; 
	} /* end switch */

    TempStr = TempStr->Next;
    } /* end while */	

} /* END CreateRackScanList */

/************************************************************************
*
* Name:	ConvertOutputData(DescList,InputData,NoOfEntries)		
*
* Type:	void
*
* TYPE			PARAMETER	IOGF	DESCRIPTION
* 
* DIOC_DESC_LIST_ENTRY	DescList[]	I	The output data description
* pwr_tInt16			Indata[]	I	The corresponding output data
* int			NoOfEntries	I	Number of entries in the output
*						data lists 
*
* Description:	Converts the input data to the appropriate values
*
*************************************************************************/

static void ConvertOutputData(DIOC_DESC_LIST_ENTRY   DescList[],
			      pwr_tInt16		     OutputData[],
			      int   		     NoOfEntries)

{
    pwr_tFloat32	*AoValP;
    pwr_tInt32	Sts;
    int		DoBaseIdx;
    int		Index;
    int		DoIndex;
    pwr_tUInt16	DoValue;
    pwr_tUInt16	DoTestVal;


    /* Scan through the entries and do the conversion */

    for (Index = 0; Index < NoOfEntries; Index++)
	{
	switch (DescList[Index].EntryType) {
	
	case _eDiocLEtype_illegal:

	    break;

	case _eDiocLEtype_ao1:
	case _eDiocLEtype_ao4:
	case _eDiocLEtype_pd:

	    if (DescList[Index].Data.Ao1.SigPtr == NULL) /* Check for unconnected*/
                break;				 /* signals */

	    if (!DescList[Index].Data.Ao1.ChanPtr->TestOn)
		AoValP = &aoap->Value[DescList[Index].Data.Ao1.
			  SigPtr->ValueIndex];
	    else
		AoValP = &DescList[Index].Data.Ao1.ChanPtr->FixedOutValue;

	    Sts = io_Convert(DescList[Index].Data.Ao1.ChanPtr,
			     DescList[Index].Data.Ao1.CardPtr,
			     &DescList[Index].Data.Ao1.SigPtr->RawValue,
			     &DescList[Index].Data.Ao1.SigPtr->SigValue,
			     AoValP,
			     pwr_cClass_ChanAo);
			     		
	    OutputData[Index] = DescList[Index].Data.Ao1.SigPtr->RawValue; 

	    /* Add channel address to the data, if not programmable delay
	       counter card */

	    if (DescList[Index].EntryType != _eDiocLEtype_pd)
		OutputData[Index]  
		    |= (DescList[Index].Data.Ao1.ChanPtr->Number << 14);

	    break;

	case _eDiocLEtype_do:
	    

	    DoBaseIdx = (DescList[Index].Data.Do.CardPtr->DevNumber - 1) * 16;

	    for (DoIndex = 0,DoValue = 0; DoIndex < 16; DoIndex++)
		if (doap->Value[DoBaseIdx+DoIndex] != 0)
		    DoValue |= (1 << DoIndex);

	    DoValue ^= DescList[Index].Data.Do.CardPtr->InvMask1;
	    
	    /* Test if we should output fixed testvalues for some/all
	       signals  */

	    if (DescList[Index].Data.Do.CardPtr->TestMask1);		
		DoValue = (DoValue & ~DescList[Index].Data.Do.CardPtr->TestMask1) | 
			  (DescList[Index].Data.Do.CardPtr->TestMask1 | 
			   DescList[Index].Data.Do.CardPtr->TestValue1);
	    
	    OutputData[Index] = io_ReverseWord(&DoValue); 
	    break;

       } /* End case */

    } /* End for */

}

/************************************************************************
*
* Name:	ConvertInputData(DescList,InputData,NoOfEntries)		
*
* Type:	void
*
* TYPE			PARAMETER	IOGF	DESCRIPTION
* 
* DIOC_DESC_LIST_ENTRY	DescList[]	I	The input data description
* pwr_tInt16			Indata[]	I	The corresponding input data
* int			NoOfEntries	I	Number of entries in the input
*						data lists 
*
* Description:	Converts the input data to the appropriate values
*
*************************************************************************/

static void ConvertInputData(DIOC_DESC_LIST_ENTRY   DescList[],
			    pwr_tInt16		    InputData[],
			    int			    NoOfEntries)

{
    int			Index;
    int			ConvIndex;
    int			ValueIndex;
    pwr_sClass_Ai_7436	  *AiCardPtr;
    pwr_sClass_Di_7437_37 *DiCardPtr;
    pwr_tClassId	AiType;
    pwr_tInt32		Sts;

    /* Scan through the entries and do the conversion */

    for (Index = 0; Index < NoOfEntries; Index++)
	{
	switch (DescList[Index].EntryType) {
	
	case _eDiocLEtype_illegal:

	    break;

	case _eDiocLEtype_ai:
	case _eDiocLEtype_ait:

	    if (DescList[Index].Data.Ai.SigPtr == NULL) /* Check for unconnected*/
                break;				     /* signals */

	    if (DescList[Index].Data.Ai.ChanPtr->ConversionOn)
		{
		DescList[Index].Data.Ai.SigPtr->RawValue = InputData[Index];
		if (DescList[Index].EntryType == _eDiocLEtype_ai)
		    AiType = pwr_cClass_ChanAi;
		else
		    AiType = pwr_cClass_ChanAit;
		Sts = io_Convert(DescList[Index].Data.Ai.ChanPtr,
				 DescList[Index].Data.Ai.CardPtr,
				 &DescList[Index].Data.Ai.SigPtr->RawValue,
				 &DescList[Index].Data.Ai.SigPtr->SigValue,
				 &aiap->Value[DescList[Index].Data.Ai
				 .SigPtr->ValueIndex],
				 AiType);
		}

	    break;
	case _eDiocLEtype_di:
	    
	    /* Explode some of the addressing expressions in hope of
	       speeding things up */

	    DiCardPtr = DescList[Index].Data.Di.CardPtr;
	    ValueIndex = (DiCardPtr->DevNumber-1) * 16;

	    /* First mirror the input data, since RTP numbers msb as 0 */
   
	    InputData[Index] = io_ReverseWord(&InputData[Index]);

	    /* Then invert the input data by applying an XOR with 
	       the inversion mask */

	    InputData[Index] ^= DiCardPtr->InvMask1;
	    	    	
	    /* This is a bit of a tricky expression */

	    for (ConvIndex = 0; 
		 ConvIndex < DiCardPtr->MaxNoOfChannels;
		 ConvIndex++)
		 if (DiCardPtr->ConvMask1 & (1 << ConvIndex))
		    diap->Value[ValueIndex + ConvIndex] =
			((InputData[Index] & (1 << ConvIndex)) != 0); 

	    break;

	case _eDiocLEtype_co1:
	case _eDiocLEtype_co1t:
	case _eDiocLEtype_co4:
	case _eDiocLEtype_co4t:


	    

	    break;
       } /* End case */

    } /* End for */

} /* END ConvertInputData */

/************************************************************************
*
* Name:	FixupDiocScanLists(DiocStrPtr,RackList);	
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	This routine creates the scan lists for one single dioc
*		unit. It tries to compact all lists as much as possible
*		since:
*
*		1. One single dioc handles up to 16 different scan lists
*
*		2. One dioc can handle up to eight controllers (racks)
*
*		3. A scan list can contain AI signals OR DI signals OR
*		   (AO AND DO signals)
*
*		4. DI and output signals in a scan list can span multiple
*		   controllers, AI's can't.
*
*		All this implies that in certain circumstances it's possi-
*		ble to run out of scan lists, if we don't pack the lists as
*		tight as possible.
*
*		NB !!! This compaction is not yet implemented due
*		to lack of time (what else !)
*
*************************************************************************/

static void FixupDiocScanLists(_DIOC_DEV_STRUCT	    *DiocStrPtr,    
			       _DIOC_TEMP_RACK_LIST *RackList)

{

    _DIOC_TEMP_RACK_LIST    *BackPointer;
    DIOC_SCAN_BUFFER	    *AIScanList;
    DIOC_SCAN_BUFFER	    *DIScanList;
    DIOC_SCAN_BUFFER	    *OutScanList;
    DIOC_DESC_LIST_ENTRY    *DIDesc;
    DIOC_DESC_LIST_ENTRY    *AIDesc;
    DIOC_DESC_LIST_ENTRY    *OutDesc;
    DIOC_SCAN_LIST_DESCR    *ScanListEntry = NULL;
    unsigned int	    ScanNo = 0;


    while (RackList != NULL)
	{
	if (RackList->NoOfAi != 0)
	    {
	    /* NB ! AI conv slot is always zero in proview/r systems */
	    AIScanList = calloc(1,sizeof(DIOC_6B_HEADER)+((RackList->NoOfAi + 1) * 2)); 
	    AIScanList->Header.FuncCode = DIOC_MT_ASG_SCAN_LIST;
	    AIScanList->Header.ScanType = DIOC_SC_UNIVERSAL_AI;
	    AIScanList->Header.WordCount = RackList->NoOfAi;
	    AIScanList->Header.SubFunc = ScanNo;
	    AIScanList->Header.DevAddr = RackList->RackAddr; /* 941215 */
	    AIScanList->Header.Crc16 = io_lat_crc16(AIScanList,sizeof(DIOC_6B_HEADER)-2);
	    AIDesc = calloc(RackList->NoOfAi,sizeof(DIOC_DESC_LIST_ENTRY));
	    ScanNo++;
	    }
	if (RackList->NoOfDi != 0)
	    {
	    DIScanList = calloc(1,sizeof(DIOC_6B_HEADER)+((RackList->NoOfDi + 1) * 2)); 
	    DIScanList->Header.FuncCode = DIOC_MT_ASG_SCAN_LIST;
	    DIScanList->Header.ScanType = DIOC_SC_MULTIPLE_DI;
	    DIScanList->Header.WordCount = RackList->NoOfDi;
	    DIScanList->Header.DevAddr = 0; /* 941215 */
	    DIScanList->Header.SubFunc = ScanNo;
	    DIScanList->Header.Crc16 = io_lat_crc16(DIScanList,sizeof(DIOC_6B_HEADER)-2);
	    DIDesc = calloc(RackList->NoOfDi,sizeof(DIOC_DESC_LIST_ENTRY));
	    ScanNo++;
	    }
	if (RackList->NoOfDoAo != 0)
	    {
	    OutScanList = calloc(1,sizeof(DIOC_6B_HEADER)+((RackList->NoOfDoAo + 1) * 2)); 
	    OutScanList->Header.FuncCode = DIOC_MT_ASG_SCAN_LIST;
	    OutScanList->Header.ScanType = DIOC_SC_MULTIPLE_DO;
	    OutScanList->Header.WordCount = RackList->NoOfDoAo;
	    OutScanList->Header.DevAddr = 0; /* 941215 */
	    OutScanList->Header.SubFunc = ScanNo;
	    OutScanList->Header.Crc16 = io_lat_crc16(OutScanList,sizeof(DIOC_6B_HEADER)-2);
	    OutDesc = calloc(RackList->NoOfDoAo,sizeof(DIOC_DESC_LIST_ENTRY));
	    ScanNo++;
	    }

	CreateRackScanLists(RackList->RackAddr,
			    RackList->IOList,
			    DIScanList,
			    DIDesc,
			    0,
			    AIScanList,
			    AIDesc,
			    0, 
			    OutScanList,
			    OutDesc,
			    0);
	
	if (RackList->NoOfAi != 0)
	    {
	    if (ScanListEntry == NULL)	    
		{
		if (DiocStrPtr->ScanLists == NULL)
		    {
		    DiocStrPtr->ScanLists = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    ScanListEntry = DiocStrPtr->ScanLists;
		    }
		else
		    {
		    for (ScanListEntry = DiocStrPtr->ScanLists;
			 ScanListEntry->Next != NULL;
			 ScanListEntry = ScanListEntry->Next);
		    ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    }
		 }
	     else
		{
		ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		ScanListEntry = ScanListEntry->Next;
		}
	    ScanListEntry->ListType = _eDiocIOLtype_ai;
	    ScanListEntry->NoOfEntries = RackList->NoOfAi;
	    ScanListEntry->ListBuffer = AIScanList;
	    ScanListEntry->DescList = AIDesc;
	    ScanListEntry->ScanListNo = AIScanList->Header.SubFunc;
	    ScanListEntry->ListBuffer->Entries[RackList->NoOfAi].Crc16 = 
		io_lat_crc16(ScanListEntry->ListBuffer->Entries,RackList->NoOfAi * 2);
	    }
	if (RackList->NoOfDi != 0)
	    {
	    if (ScanListEntry == NULL)	    
		{
		if (DiocStrPtr->ScanLists == NULL)
		    {
		    DiocStrPtr->ScanLists = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    ScanListEntry = DiocStrPtr->ScanLists;
		    }
		else
		    {
		    for (ScanListEntry = DiocStrPtr->ScanLists;
			 ScanListEntry->Next != NULL;
			 ScanListEntry = ScanListEntry->Next);
		    ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    }
		 }
	     else
		{
		ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		ScanListEntry = ScanListEntry->Next;
		}
	    
	    ScanListEntry->ListType = _eDiocIOLtype_di;
	    ScanListEntry->NoOfEntries = RackList->NoOfDi;
	    ScanListEntry->ListBuffer = DIScanList;
	    ScanListEntry->DescList = DIDesc;
	    ScanListEntry->ScanListNo = DIScanList->Header.SubFunc;
	    ScanListEntry->ListBuffer->Entries[RackList->NoOfDi].Crc16 = 
		io_lat_crc16(ScanListEntry->ListBuffer->Entries,RackList->NoOfDi * 2);
	    }
	if (RackList->NoOfDoAo != 0)
	    {
	    if (ScanListEntry == NULL)	    
		{
		if (DiocStrPtr->ScanLists == NULL)
		    {

		    DiocStrPtr->ScanLists = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    ScanListEntry = DiocStrPtr->ScanLists;
		    }
		else
		    {
		    for (ScanListEntry = DiocStrPtr->ScanLists;
			 ScanListEntry->Next != NULL;
			 ScanListEntry = ScanListEntry->Next);
		    ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		    }
		 }
	     else
		{
		ScanListEntry->Next = calloc(1,sizeof(DIOC_SCAN_LIST_DESCR));
		ScanListEntry = ScanListEntry->Next;
		}
	    
	    ScanListEntry->ListType = _eDiocIOLtype_do;
	    ScanListEntry->NoOfEntries = RackList->NoOfDoAo;
	    ScanListEntry->ListBuffer = OutScanList;
	    ScanListEntry->DescList = OutDesc;
	    ScanListEntry->ScanListNo = OutScanList->Header.SubFunc;
	    ScanListEntry->ListBuffer->Entries[RackList->NoOfDoAo].Crc16 = 
	      io_lat_crc16(ScanListEntry->ListBuffer->Entries,RackList->NoOfDoAo * 2);
	    }
			 
	BackPointer = RackList;
	RackList = RackList->Next;
	free(BackPointer);
	}

} /* END FixupDiocScanLists */


/************************************************************************
*
* Name:	BuildScanLists(DiocLine)	
*
* Type:	pwr_tUInt32	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* _DIOC_LINE_STRUCT DiocLine	I	Pointer to a line object
* 
* Description:	Builds scan lists for all diocs and all racks hanging on
*		the specified line
*************************************************************************/

static pwr_tUInt32 BuildScanLists(_DIOC_LINE_STRUCT *DiocLine)

{
    pwr_tObjid		RackId;
    pwr_tObjid		DiocId;
    pwr_tClassId	ObjClass;
    pwr_tUInt32		Sts;
    pwr_tUInt32		Sts2;
    pwr_sClass_Rack_RTP	*RackPtr;
    _DIOC_TEMP_RACK_LIST *TempRackList = NULL;
    _DIOC_TEMP_RACK_LIST *RackListPtr;
    _DIOC_DEV_STRUCT	*DiocStrPtr;	

    /* Get a grip on the dioc objects that are decendants of this line */

    Sts = gdh_GetChild(DiocLine->LineObjId,
		       &DiocId);

    while (ODD(Sts))
	{
	TempRackList = NULL;
	Sts = gdh_GetObjectClass(DiocId,
				&ObjClass);
	if (ObjClass == pwr_cClass_RTP_DIOC)
	    {
	    Sts2 = gdh_GetChild(DiocId,
				&RackId);
	    while ((Sts2 & 1) == 1)
		{
		Sts2 = gdh_GetObjectClass(RackId,
					 &ObjClass);
		if (ObjClass == pwr_cClass_Rack_RTP)
		    {
		    Sts2 = gdh_ObjidToPointer(RackId,
					      &RackPtr);
		    
		    if (TempRackList == NULL)
			{
			RackListPtr = calloc(1,sizeof(_DIOC_TEMP_RACK_LIST));
			TempRackList = RackListPtr;
			}
		    else
			{
			for (RackListPtr = TempRackList; 
			     RackListPtr->Next != NULL;
			     RackListPtr = RackListPtr->Next);
			RackListPtr->Next = calloc(1,sizeof(_DIOC_TEMP_RACK_LIST));
			RackListPtr = RackListPtr->Next;
			}

		    RackListPtr->RackAddr = RackPtr->RackAddress;
		    FindChannelsInRack(RackId,
				       &RackListPtr->NoOfDi,
				       &RackListPtr->NoOfAi,
				       &RackListPtr->NoOfDoAo,
				       &RackListPtr->IOList);
		    }
		
		Sts2 = gdh_GetNextSibling(RackId,
					  &RackId);     
		}
	    }
	/* Create the scan lists for a specified dioc device */

	if (DiocLine->DiocList == NULL)
	    {
	    DiocStrPtr = calloc(1,sizeof(_DIOC_DEV_STRUCT));
	    DiocLine->DiocList = DiocStrPtr;
	    }
	else
	    {
	    for (DiocStrPtr = DiocLine->DiocList; 
		 DiocStrPtr->Next != NULL;
		 DiocStrPtr = DiocStrPtr->Next);
	    DiocStrPtr->Next = calloc(1,sizeof(_DIOC_DEV_STRUCT));
	    DiocStrPtr = DiocStrPtr->Next;
	    }
	
	Sts = gdh_ObjidToPointer(DiocId,
				 &DiocStrPtr->DiocObjPtr);
    
	DiocStrPtr->Divider = 1.0; /* Scan first lap */

	FixupDiocScanLists(DiocStrPtr,
			   TempRackList);
			       	
	Sts = gdh_GetNextSibling(DiocId,   
				 &DiocId);
	}

    
}

/************************************************************************
*
* Name:	io_dioc_init()		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description: This routine creates a datastructure that describes the dioc
*	       structure for a node as follows.
*
*	       1. It finds all RTP_DIOC objects for the node and orders them
*		  as descendants to the appropriate line objects
*
*************************************************************************/

pwr_tInt32 io_dioc_init()

{

    pwr_tUInt32 Sts;


    InitValueBasePointers();
    Sts =  BuildLineList();
    
    if (EVEN(Sts))
	{
	errh_CErrLog(IO__INIDIOCERR,errh_ErrArgMsg(Sts), NULL);
	/* here  error routine */
	}

    
} /* END iodioc_build_tree */

/************************************************************************
*
* Name: DownLoadScanLists(DDAPort,DiocPtr)		
*
* Type:	static pwr_tUInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* PORT		*DDAPort	I	DDA port to commmunicate through
*
* _DIOC_DEV_STR *DiocPtr	I	Pointer to the dioc we like to
*					try to download to 
* Description:	This routine tries to write all scan lists associated 
*		with a dioc to it.
*************************************************************************/

static pwr_tStatus
DownLoadScanLists(
    PORT		*DDAPort,
    _DIOC_DEV_STRUCT    *DiocPtr
)
{
    DIOC_SCAN_LIST_DESCR    *ScanListPtr;
    DIOC_4B_UNION	    Answer;
    DIOC_4B_UNION	    ResetMsg;
    pwr_tUInt32		    Sts;
    pwr_tUInt32		    Sts2;
    int			    NoOfRetries;
    pwr_tBoolean	    RecOk;
    int			    RecAnt;
    int			    who;


    Sts = io_tty_typecl(DDAPort,NULL,NULL);

    Answer.HdrStr.FuncCode = 0;
   
    ResetMsg.HdrStr.FuncCode = DIOC_MT_RESET;
    ResetMsg.HdrStr.DevAddr  = 0;
    ResetMsg.HdrStr.SubFunc  = 0xF; /* reset power fail flag  */
    ResetMsg.HdrStr.SiteAddr = DiocPtr->DiocObjPtr->Address;

    ResetMsg.HdrStr.Crc16 = io_lat_crc16(&ResetMsg,2);

    for (NoOfRetries = 0; 
	 NoOfRetries < DiocPtr->DiocObjPtr->NoOfReXmissions;
	 NoOfRetries ++)
	{
        Sts = io_tty_sdrc(DDAPort,
		      	  &ResetMsg,
		      	  sizeof(DIOC_4B_UNION),
		      	  &Answer,
		      	  sizeof(DIOC_4B_UNION),
		      	  NULL,
		      	  NULL,
		      	  NULL,
		      	  NULL,
		      	  DiocPtr->DiocObjPtr->TimeOutTime,
		      	  1,
		      	  NULL, 
		          &RecAnt);
	
	ker$wait_any(&Sts2, &who,&NoWait , io_comm_terminate);

	if (who == 1) /* Termination in progress */
	   {
	   DiocPtr->DiocObjPtr->CommOk = FALSE;
	   return ELN$_TIMEOUT;
	   }
	if (ODD(Sts))
	   break;	   
	}
    if (EVEN(Sts))
	return Sts;
		      
    if (Answer.HdrStr.FuncCode != DIOC_MT_ACK)
	{
	DiocPtr->DiocObjPtr->CommOk = FALSE;
	return ELN$_TIMEOUT;
	}

    Answer.HdrStr.FuncCode = 0;
    ResetMsg.HdrStr.SubFunc  = 0xB; /* Stop continous scans, delete scan lists
				       reset devices  */
    ResetMsg.HdrStr.Crc16 = io_lat_crc16(&ResetMsg,2);


    for (NoOfRetries = 0; 
	 NoOfRetries < DiocPtr->DiocObjPtr->NoOfReXmissions;
	 NoOfRetries ++)
	{
    	Sts = io_tty_sdrc(DDAPort,
		          &ResetMsg,
		          sizeof(DIOC_4B_UNION),
		      	  &Answer,
		      	  sizeof(DIOC_4B_UNION),
		      	  NULL,
		          NULL,
		          NULL,
		          NULL,
		          DiocPtr->DiocObjPtr->TimeOutTime,
		          1,
		      	  NULL, 
		      	  &RecAnt);

	ker$wait_any(&Sts2, &who,&NoWait , io_comm_terminate);

	if (who == 1) /* Termination in progress */
	   {
	   DiocPtr->DiocObjPtr->CommOk = FALSE;
	   return ELN$_TIMEOUT;
	   }
	if (ODD(Sts))
	   break;	   
	}
    if (EVEN(Sts))
	return Sts;


		      
    if (Answer.HdrStr.FuncCode != DIOC_MT_ACK)
	{
	DiocPtr->DiocObjPtr->CommOk = FALSE;
	return ELN$_TIMEOUT;
	}
    for (ScanListPtr = DiocPtr->ScanLists; ScanListPtr != NULL; ScanListPtr = ScanListPtr->Next)
	{
    	for (NoOfRetries = 0; 
	     NoOfRetries < DiocPtr->DiocObjPtr->NoOfReXmissions;
	     NoOfRetries ++)
	   {
	   Sts = io_tty_sdrc(DDAPort,
			     ScanListPtr->ListBuffer,
			     sizeof(DIOC_6B_HEADER) + ((ScanListPtr->NoOfEntries + 1) * 2),
			     &Answer,
			     sizeof(DIOC_4B_UNION),
			     NULL,
			     NULL,
			     NULL,
			     NULL,
			     DiocPtr->DiocObjPtr->TimeOutTime,
			     1,
			     NULL, 
			     &RecAnt);

	   ker$wait_any(&Sts2, &who,&NoWait , io_comm_terminate);

	   if (who == 1) /* Termination in progress */
	      {
	      DiocPtr->DiocObjPtr->CommOk = FALSE;
	      return ELN$_TIMEOUT;
	      }
	   if (ODD(Sts))
	      break;	   
	}
        if (EVEN(Sts))
	   return Sts;

	if ((RecAnt != sizeof(DIOC_4B_UNION)) || 
	    (Answer.HdrStr.FuncCode != DIOC_MT_ACK))
	    {
	    DiocPtr->DiocObjPtr->CommOk = FALSE;
	    return ELN$_TIMEOUT;
	    }
	}

    return ELN$_SUCCESS;
} /* END DownLoadScanLists  */

/************************************************************************
*
* Name:	ScanDioc(DDAPort,DiocListPtr,TxBuffer,RxBuffer)	
*
* Type:	static void	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* PORT		*DDAPort	I	Pointer to a DDA port structure
* _DIOC_DEV_STRUCT DiocListPtr	I	Pointer to the structure describing 
*					the dioc to scan
*
* DIOC_SCAN_BUFFER  *TxBuffert	I
* DIOC_SCAN_BUFFER  *RxBuffert	I	
*
* Description:	This routine scans all scan lists for a dioc 
*	
*************************************************************************/

static void ScanDioc(PORT		 *DDAPort,	
		     _DIOC_DEV_STRUCT	 *DiocPtr,
		     DIOC_SCAN_BUFFER	 *TxBuffer,
		     DIOC_SCAN_BUFFER	 *RxBuffer)
{
    DIOC_SCAN_LIST_DESCR    *ScanListPtr;
    DIOC_4B_UNION	    XqtInpScanMsg;    
    int			    RecAnt;
    pwr_tUInt16		    Crc16;
    pwr_tUInt16		    InpCrc16;
    int			    Sts,Sts2;
    int		 	    who;
    int			    NoOfRetries;


    XqtInpScanMsg.HdrStr.SiteAddr= DiocPtr->DiocObjPtr->Address;
    XqtInpScanMsg.HdrStr.FuncCode = DIOC_MT_XQT_IN_SCAN;

    for (ScanListPtr = DiocPtr->ScanLists; ScanListPtr != NULL; ScanListPtr = ScanListPtr->Next)
	{
	switch(ScanListPtr->ListType) {
	
	case _eDiocIOLtype_ai:
	case _eDiocIOLtype_di:

	    XqtInpScanMsg.HdrStr.DevAddr = ScanListPtr->ListBuffer->Header.DevAddr;
	    XqtInpScanMsg.HdrStr.SubFunc = ScanListPtr->ScanListNo;
	    XqtInpScanMsg.HdrStr.Crc16 = io_lat_crc16(&XqtInpScanMsg,2);
	
    	    for (NoOfRetries = 0; 
	         NoOfRetries < DiocPtr->DiocObjPtr->NoOfReXmissions;
	         NoOfRetries ++)
	         {
	         Sts = io_tty_sdrc(DDAPort,
			           &XqtInpScanMsg,
			           sizeof(DIOC_4B_UNION),
			      	   RxBuffer,
			      	   sizeof(DIOC_6B_HEADER) + ((ScanListPtr->NoOfEntries+ 1) * 2),
			      	   NULL,
			      	   NULL, 
			      	   NULL,
			      	   NULL,
			      	   DiocPtr->DiocObjPtr->TimeOutTime,
			      	   DiocPtr->DiocObjPtr->NoOfReXmissions,
			      	   NULL, 
			      	   &RecAnt);
	    
		  if (EVEN(Sts))
		     {
		     DiocPtr->DiocObjPtr->CommOk = FALSE;
		     return;
		     }
	   	  ker$wait_any(&Sts2, &who,&NoWait , io_comm_terminate);

	          if (who == 1) /* Termination in progress */
		     {
                     Sts = ELN$_TIMEOUT;
		     break;
		     }
		  if ((ODD(Sts)) && (Sts != ELN$_TIMEOUT))
		     break;   
                  }
	    if (Sts == ELN$_TIMEOUT)	/* If no answer, tell the world and quit */
		{
		DiocPtr->DiocObjPtr->CommOk = FALSE;
		return;
		}

	    if (RxBuffer->Header.FuncCode == DIOC_MT_RMT_BLK_RESPONSE)
		{
		Crc16 = io_lat_crc16(RxBuffer,4);
		InpCrc16 = RxBuffer->Header.Crc16;
		if (InpCrc16 != Crc16)
		    {
		    /* Here if header checksum error */
		    DiocPtr->DiocObjPtr->CommOk = FALSE;
		    return;
		    }
		Crc16 = io_lat_crc16(RxBuffer->Entries,
				     (RxBuffer->Header.WordCount) * 2);
		InpCrc16 =  
		    RxBuffer->Entries[RxBuffer->Header.WordCount].Crc16;
		if (Crc16 != InpCrc16)
		    {
		    /* Here if data checksum error */
		    DiocPtr->DiocObjPtr->CommOk = FALSE;
		    return;
		    }
		ConvertInputData(ScanListPtr->DescList,
				 RxBuffer->Entries,
				 RxBuffer->Header.WordCount);
		}
	    else /* if (RxBuffer->Header.FuncCode == DIOC_MT_NACK) */
		{
		DiocPtr->DiocObjPtr->CommOk = FALSE;
		return;
		/* Here if we get a nack message from the node */
		}
	    break;

	case _eDiocIOLtype_do:
	case _eDiocIOLtype_ao:

	    /* Copy data from assign scan list header */
	    memcpy(&TxBuffer->Header,&ScanListPtr->ListBuffer->Header,2);
	    TxBuffer->Header.FuncCode = DIOC_MT_XQT_OUT_SCAN;
	    
	    /* Enter the number of words, not forgetting the data crc, and
	       calculate the header checksum */

	    TxBuffer->Header.WordCount = ScanListPtr->NoOfEntries;
	    TxBuffer->Header.Crc16 = io_lat_crc16(&TxBuffer->Header,4);
	    
	    /* Convert the output data to a form suitable for the dioc */

	    ConvertOutputData(ScanListPtr->DescList,
			      TxBuffer->Entries,
			      ScanListPtr->NoOfEntries);

	    /* Calculate data checksum */

	    TxBuffer->Entries[ScanListPtr->NoOfEntries].Crc16 = 
		io_lat_crc16(TxBuffer->Entries,ScanListPtr->NoOfEntries * 2);
	    
	    /* Send output scan to dioc */

    	    for (NoOfRetries = 0; 
	         NoOfRetries < DiocPtr->DiocObjPtr->NoOfReXmissions;
	         NoOfRetries ++)
	         {
	    	 Sts = io_tty_sdrc(DDAPort,
			      	   TxBuffer,
			      	   sizeof(DIOC_6B_HEADER) + ((ScanListPtr->NoOfEntries+ 1) * 2),
			      	   RxBuffer,
			      	   sizeof(DIOC_4B_HEADER),
			      	   NULL,
			      	   NULL,
			      	   NULL,
			      	   NULL,
			      	   DiocPtr->DiocObjPtr->TimeOutTime,
			      	   DiocPtr->DiocObjPtr->NoOfReXmissions,
			      	   NULL, 
			      	   &RecAnt);

		  if (EVEN(Sts))
		     {
		     DiocPtr->DiocObjPtr->CommOk = FALSE;
		     return;
		     }
	   	  ker$wait_any(&Sts2, &who,&NoWait , io_comm_terminate);

	          if (who == 1) /* Termination in progress */
		     {
		     DiocPtr->DiocObjPtr->CommOk = FALSE;
		     return;
		     }
		  if ((ODD(Sts)) && (Sts != ELN$_TIMEOUT))
		     break;   
                  }

	    if (RxBuffer->Header.FuncCode != DIOC_MT_ACK)
		{
		/* Take action */
		return;
		}
	    break;
	    } /* END switch */
	} /* END for */
    if (DiocPtr->ScanLists != NULL)
       DiocPtr->DiocObjPtr->CommOk = TRUE; /* Only true if scanlists really  exist */
} /* END ScanDioc */

/************************************************************************
*
* Name: Termination_routine(LineStr,TxBuffer,RxBuffer)		
*
* Type:		
*
* TYPE			PARAMETER	IOGF	DESCRIPTION
* 
* _DIOC_LINE_STRUCT	*LineStr	I	Pointer to line desc
* DIOC_SCAN_BUFFER	*TxBuffer	I	Pointer to recieve buffer
* DIOC_SCAN_BUFFER	*RxBuffer	I	Pointer to transitter buffer
*
* Description:	Cleans up wheh the process has been told to terminate
*
*************************************************************************/

static void Termination_routine(_DIOC_LINE_STRUCT   *LineStr,
				 DIOC_SCAN_BUFFER   *TxBuffer,
				 DIOC_SCAN_BUFFER   *RxBuffer)

{
    
    _DIOC_DEV_STRUCT	    *DiocPtr;
    _DIOC_DEV_STRUCT	    *BackPtr;
    DIOC_SCAN_LIST_DESCR    *ListPtr;
    DIOC_SCAN_LIST_DESCR    *BackListPtr;
    pwr_sClass_Line_LAT	    *LatStrPtr;
    pwr_tInt32		    Sts;

    /*  start deallocating the xmission buffers */

    free(TxBuffer);
    free(RxBuffer);

    /* Loop through all of the dioc nodes on the line and deallocate
       the scan lists and dioc descriptions */

    for (DiocPtr = LineStr->DiocList; DiocPtr != NULL;)
	{
	for (ListPtr = DiocPtr->ScanLists ; ListPtr != NULL; )
	    {
	    free(ListPtr->DescList);
	    free(ListPtr->ListBuffer);
	    free(ListPtr->DataBuffer);
	    BackListPtr = ListPtr;
	    ListPtr = ListPtr->Next;
	    free(BackListPtr);
	    }
	BackPtr = DiocPtr;
	DiocPtr = DiocPtr->Next;
	free(BackPtr);
	}

    /* Now clobber the LAT links and ports , if any */

    if (LineStr->LineType == pwr_cClass_Line_LAT)
	{
	Sts = gdh_ObjidToPointer(LineStr->LineObjId,
				 &LatStrPtr);
	if (ODD(Sts))
	  Sts = io_lat_disc_and_delete_port(&LineStr->DDAPort,
					     LatStrPtr->LatDevName);
						
	}

    /* Free sup lists and reset timers */
    io_ClearSupLst( SupCtx);


} /* END Termination routine */

/************************************************************************
*
* Name: DiocComProcess(MyLine)		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	This process handles all dioc's connected to a specified line
*		Watch out for some things :
*
*		1. This process has a hardwired base time of one second
*		   That is further divided by what is specified in the 
*		   dioc object.
*
*		2. The wait is done on an absolute time, that may lead to
*		   problems if someone resets system time [this is curable]
*
*************************************************************************/

static void DiocComProcess(_DIOC_LINE_STRUCT	*MyLine)

{
    pwr_tInt32		    who;
    int			    Sts;
    pwr_sClass_Line_LAT	    *LineStrPtr;
    DIOC_SCAN_BUFFER	    *TxBuffer;
    DIOC_SCAN_BUFFER	    *RxBuffer;
    int			    MaxTxSize = 0;
    int			    MaxRxSize = 0;
    DIOC_SCAN_LIST_DESCR    *ScanListPtr = NULL;
    _DIOC_DEV_STRUCT	    *DiocListPtr = NULL;
    LARGE_INTEGER	    CurrentTime;
    LARGE_INTEGER	    NextTime;
    LARGE_INTEGER	    l_diff;
    LARGE_INTEGER	    DeltaTime = {10000000,0}; /* This should represent 1 s */

    pwr_tObjid		    IOObjId;
    pwr_tString80	    Name;
    pwr_tString132	    FullName;
    pwr_tFloat32	    CycleTime = 1.0;	      /* This should represent 1 s */
    pwr_tInt32		    CycleTimeInt;	  
    

    /* Start with setting up the lat line (if it is a LAT line)*/

    if (MyLine->LineType == pwr_cClass_Line_LAT)
    {
	ker$create_port(&Sts,
			&MyLine->DDAPort,
			NULL); 
	Sts = gdh_ObjidToPointer(MyLine->LineObjId,
				 &LineStrPtr);

	if (EVEN(Sts))
        {
            errh_CErrLog(IO__DIOCERRGETPTR,errh_ErrArgMsg(Sts), NULL);
            ker$exit(NULL,Sts);
        }
    
	Sts = io_lat_create_and_map_port(&MyLine->DDAPort,
					 LineStrPtr->LatDevName,
					 LineStrPtr->ServerName,
					 LineStrPtr->ServerPortName);
	if (EVEN(Sts))
        {
            errh_CErrLog(IO__DIOCLATCREERR,errh_ErrArgMsg(Sts), NULL);
            ker$exit(NULL,Sts);
        }

	Sts = io_lat_set_characteristics(&MyLine->DDAPort,
					 FALSE,	    /* No escape recognition */
					 FALSE,	    /* No echo */
					 TRUE,	    /* Pass all characters */
					 TRUE,	    /* Eight bit characters */
					 FALSE,	    /* Not a video term */
					 FALSE);    /* No XON/XOFF */
	if (EVEN(Sts))
        {
	    errh_CErrLog(IO__DIOCCHARSETERR,errh_ErrArgMsg(Sts), NULL);
	    ker$exit(NULL,Sts);
        }
    }

    /* Initialization of sup lists */
    Sts = io_InitSupLst(&SupCtx, CycleTime);

    Sts = BuildScanLists(MyLine);     

    /* Find out how large transmitt and recieve buffers we need */

    for (DiocListPtr = MyLine->DiocList; DiocListPtr != NULL;
	 DiocListPtr = DiocListPtr->Next)
	{
        DiocListPtr->DiocObjPtr->CommOk = FALSE; /* assume error */
	for (ScanListPtr = DiocListPtr->ScanLists; 
	     ScanListPtr != NULL;     
	     ScanListPtr = ScanListPtr->Next)
	     {
	     if ((ScanListPtr->ListType == _eDiocIOLtype_ai) || 
		 (ScanListPtr->ListType == _eDiocIOLtype_di))
		 MaxRxSize = MAX(MaxRxSize,(ScanListPtr->NoOfEntries * 2));
	     else if ((ScanListPtr->ListType == _eDiocIOLtype_do) ||
		      (ScanListPtr->ListType == _eDiocIOLtype_ao))
		 MaxTxSize = MAX(MaxTxSize,(ScanListPtr->NoOfEntries * 2));
	     }

	}
    MaxRxSize += sizeof(DIOC_6B_HEADER) + 2;
    MaxTxSize += sizeof(DIOC_6B_HEADER) + 2;

    /* Now create those buffers */

    RxBuffer = calloc(1,MaxRxSize); 
    TxBuffer = calloc(1,MaxTxSize);
    
    /* DownLoad scan lists to DIOC s */

    for (DiocListPtr = MyLine->DiocList; DiocListPtr != NULL; 
	 DiocListPtr = DiocListPtr->Next)
	 {
	 Sts = DownLoadScanLists(&MyLine->DDAPort,
				 DiocListPtr);
         ScanDioc(&MyLine->DDAPort,	
	           DiocListPtr,
		   TxBuffer,
		   RxBuffer);
         }

    /* Get cycle time from IOHandler object */
    Sts = gdh_GetClassList(pwr_cClass_IOHandler, &IOObjId);
    if (EVEN(Sts))
      errh_CErrLog(IO__CYCTIMESERIAL,errh_ErrArgMsg(Sts), NULL);
    else
    {
      Sts = gdh_ObjidToName(IOObjId, Name, sizeof(Name), cdh_mNName);
      if (EVEN(Sts))
        errh_CErrLog(IO__CYCTIMESERIAL,errh_ErrArgMsg(Sts), NULL);
      else
      {
        strcpy (FullName, Name);
        strcat (FullName, ".CycleTimeSerial");
        Sts = gdh_GetObjectInfo ( FullName,
			      &CycleTime,
			      sizeof(CycleTime));
        if (EVEN(Sts))
	  errh_CErrLog(IO__CYCTIMESERIAL,errh_ErrArgMsg(Sts), NULL);
      }
    }

    /* Calculate delta time */
    CycleTimeInt = (pwr_tInt32) (CycleTime * 1000.0);   /* Cycle time (ms) */
    plc_ConvMSToLI ( CycleTimeInt, &DeltaTime ); /* Delta time in VAX time (100 ns) */

    ker$get_uptime(&Sts,&CurrentTime);
    for (;;)
	{
	NextTime = eln$add_large_integers(&DeltaTime,&CurrentTime);
        l_diff = eln$sub_large_integers( &CurrentTime, &NextTime);
	ker$wait_any(&Sts, &who, &l_diff, io_comm_terminate);
	if (who == 0)
	    {
	    for (DiocListPtr = MyLine->DiocList; DiocListPtr != NULL; 
		 DiocListPtr = DiocListPtr->Next)
		 if (--DiocListPtr->Divider == 0.0)
		     {
		     if (DiocListPtr->DiocObjPtr->CommOk) /* check if time out */				 
			 ScanDioc(&MyLine->DDAPort,	
				  DiocListPtr,
				  TxBuffer,
				  RxBuffer);
		     else
			 { 
			 Sts = DownLoadScanLists(&MyLine->DDAPort,
						 DiocListPtr);
			 ScanDioc(&MyLine->DDAPort,	
				  DiocListPtr,
				  TxBuffer,
				  RxBuffer);
			 ker$get_uptime(&Sts,&CurrentTime); /* to stop from biting one's tail */
			 }
		     DiocListPtr->Divider = DiocListPtr->DiocObjPtr->ScanInterval;
		     }
	      Sts = io_ScanSupLst( SupCtx);
	      }
	  else
	      {
	      Termination_routine(MyLine,TxBuffer,RxBuffer);
	      ker$exit(NULL,1);
	      }
	}

} /* END DiocComProcess */

/************************************************************************
*
* Name:	io_dioc_start()		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description: This routine starts the processes for all dioc lines
*
*************************************************************************/

pwr_tInt32 io_dioc_start()


{
    _DIOC_LINE_STRUCT	*LineStrPtr;
    int			Sts;

    LineStrPtr = DiocRootHandle.FirstLine;

    while (LineStrPtr != NULL)
	{
	ker$create_process(&Sts,    
			   &LineStrPtr->LineProcess,
			   DiocComProcess,
			   NULL,
			   LineStrPtr);
        if (ODD(Sts))
           errh_CErrLog(IO__STARTDIOCPROC, NULL);
        else
           errh_CErrLog(IO__DIOCPROCERR,errh_ErrArgMsg(Sts), NULL);

        LineStrPtr = LineStrPtr->NextLine;
	}

    return 1;
}

/************************************************************************
*
* Name:	io_dioc_terminated()		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description: This routine should do some checking on terminated processes
*
*************************************************************************/

pwr_tInt32 io_dioc_terminated()

{
    _DIOC_LINE_STRUCT	*LineStrPtr,*BackPtr;
    int			Sts;

    LineStrPtr = DiocRootHandle.FirstLine;

    while (LineStrPtr != NULL)
    {
	ker$wait_any(&Sts, NULL, NULL, LineStrPtr->LineProcess );
	BackPtr = LineStrPtr;
        LineStrPtr = LineStrPtr->NextLine;
	free(BackPtr);
    }

    return 1;
}

/************************************************************************
*
* Name:	io_dioc_ancestor_is_dioc(Object)	
*
* Type:	pwr_tBoolean	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* pwr_tObjid		Object		I	The object whoose ancestor we
*					want to find
*
* Description:	Find out if an object has a node as ancestor or a dioc
*			    object
*************************************************************************/

pwr_tBoolean io_dioc_ancestor_is_dioc(pwr_tObjid Object)

{
    pwr_tBoolean	IsDioc = FALSE;
    pwr_tUInt32		Sts;
    pwr_tClassId	ObjClass;
    
    for (;;)
	{
	Sts = gdh_GetParent(Object,
			    &Object);
	if (EVEN(Sts))
	    return FALSE;
	Sts = gdh_GetObjectClass(Object,
				 &ObjClass);
	if (EVEN(Sts))
	    return FALSE;

	if (ObjClass == pwr_cClass_RTP_DIOC)
	    return TRUE;
	else if (ObjClass == pwr_eClass_Node)
	    return FALSE;
	}	

} /* END io_dioc_ancestor */
