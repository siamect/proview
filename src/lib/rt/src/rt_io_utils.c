/* rt_io_utils.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains the access routines to the 
   runtime IO.  */

#ifdef OS_ELN
# include ctype
# include $vaxelnc
# include stdio
# include string
#else
# include <stdio.h>
# include <string.h>
# include <ctype.h>
#endif
#if defined OS_VMS
# include <ssdef.h>
#endif


#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "rt_gdh_msg.h"
#include "rt_io_msg.h"
#include "rt_gdh.h"
#include "rt_io_utils.h"

static	pwr_tInt32 io_GetConInfo (pwr_tPathName FromName,
                             pwr_tPathName ToName,
                             pwr_tObjid   *ToId);

/************************************************************************
*
* Name: io_SetParTestValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassid	ObjType         I       Type of object (numeric).
*					pwr_cClass_Ao, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information.
*					pwr_cClass_Ao -> Pointer to a float 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine is used to change parameter TestValue in channel
*   		object and one bit in parameter TestValue1 or 2 in card object.
*************************************************************************/
pwr_tInt32 io_SetParTestValue (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)

{

    pwr_tInt32	  Sts;
    pwr_tBoolean  BVal;
    pwr_tFloat32  FVal;
    pwr_tUInt16   ChanNumber;
    pwr_tUInt16	  TestMask;
    pwr_tObjid    CardObjid;
    pwr_tObjid    NewCardObjid;
    pwr_tClassId  CardObjType;	
    pwr_tFullName FullName;
    pwr_tFullName CardName;
    pwr_tFullName TestOnName;
    pwr_tObjid	  Id;
    

    Sts = io_GetConInfo (ObjName, FullName, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy(TestOnName, FullName);
    strcat(TestOnName, ".TESTVALUE");

    switch(ObjType)
    {
   	case pwr_cClass_Ao:
	    FVal = *(pwr_tFloat32 *)ValueP;
            Sts = gdh_SetObjectInfo(TestOnName, &FVal, sizeof(pwr_tFloat32));
            if(EVEN(Sts)) return(Sts);
	    break;

   	case pwr_cClass_Do:

	    BVal = *(pwr_tBoolean *)ValueP;
            Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);

	    strcat (FullName, ".NUMBER");
            Sts = gdh_GetObjectInfo(FullName, &ChanNumber, 
				    sizeof(pwr_tUInt16));
            if(EVEN(Sts)) return(Sts);

            Sts = gdh_GetParent(Id, &CardObjid);
	    if(EVEN(Sts)) return(Sts);

	    while(Sts != GDH__NO_SIBLING)
            {
            	Sts = gdh_GetObjectClass(CardObjid, &CardObjType);
                if (EVEN(Sts)) return (Sts);

                if (CardObjType == pwr_cClass_Do_HVDO32 ||
                    CardObjType == pwr_cClass_Do_7437_83) /*Card object found*/
		{
			Sts = gdh_ObjidToName (CardObjid, CardName,
					       sizeof(pwr_tFullName), cdh_mNName);
	                if (EVEN(Sts)) return (Sts);
            		if(ChanNumber <= 15)		/* Use mask 1 */
                        {
				strcat (CardName, ".TESTVALUE1");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}
                        else				/* Use mask 2 */	
			{
				strcat (CardName, ".TESTVALUE2");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}

			if(BVal == 1)			/* Set bit in mask */
				TestMask |= (1 << ChanNumber);
                        else				/* Reset bit in mask */
				TestMask &= ~(1 << ChanNumber);

        		Sts = gdh_SetObjectInfo(CardName, &TestMask, 
				       		sizeof(pwr_tUInt16));
                        return(IO__SUCCESS);
                }
                Sts = gdh_GetNextSibling(CardObjid,&NewCardObjid);
                if (EVEN(Sts)) return (Sts);

                if(Sts != GDH__NO_SIBLING) CardObjid = NewCardObjid;
            } /* End while */
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }
    return(Sts);
} /* END io_SetParTestValue */


/************************************************************************
*
* Name: io_GetParTestValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Ao, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					pwr_cClass_Ao -> Pointer to a float 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine fetches the parameter TestValue in channel object.
*************************************************************************/
pwr_tInt32 io_GetParTestValue (
                pwr_tPathName	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)
{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFloat32    FVal;
    pwr_tFullName   FullName;
    pwr_tFullName   Name;
    pwr_tObjid 	    Id;    


    Sts = io_GetConInfo (ObjName, Name, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (FullName, Name); 
    strcat (FullName, ".TESTVALUE");
    
    switch(ObjType)
    {
   	case pwr_cClass_Ao:
            Sts = gdh_GetObjectInfo(FullName, &FVal, sizeof(pwr_tFloat32));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tFloat32 *)ValueP = FVal;
            Sts = IO__SUCCESS;
	    break;

   	case pwr_cClass_Do:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }
    return(Sts);

} /* END io_GetParTestValue */


/************************************************************************
*
* Name: io_SetParTestOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Ao, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information. 
*					pwr_cClass_Ao -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine is used to change parameter TestOn in channel
*   		object and one bit in parameter TestMask1 or 2 in card object.
*************************************************************************/
pwr_tInt32 io_SetParTestOn (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)

{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tUInt16     ChanNumber;
    pwr_tUInt16	    TestMask;
    pwr_tObjid      CardObjid;
    pwr_tObjid      NewCardObjid;
    pwr_tClassId    CardObjType;	
    pwr_tFullName   TestOnName;
    pwr_tFullName   FullName;
    pwr_tFullName   CardName;
    pwr_tObjid	    Id;


    Sts = io_GetConInfo (ObjName, FullName, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (TestOnName, FullName); 
    strcat (TestOnName, ".TESTON");
    
    switch(ObjType)
    {
   	case pwr_cClass_Ao:
	    BVal = *(pwr_tBoolean *)ValueP;
            Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
            Sts = IO__SUCCESS;
	    break;
   	case pwr_cClass_Do:

	    /** Store value in card object **/
    
	    BVal = *(pwr_tBoolean *)ValueP;
	    Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
	    if(EVEN(Sts)) return(Sts);


	    strcat (FullName, ".NUMBER");
	    Sts = gdh_GetObjectInfo(FullName, &ChanNumber, 
				    sizeof(pwr_tUInt16));
	    if(EVEN(Sts)) return(Sts);

	    Sts = gdh_GetParent(Id, &CardObjid);
	    if(EVEN(Sts)) return(Sts);

	    while(Sts != GDH__NO_SIBLING)
            {
            	Sts = gdh_GetObjectClass(CardObjid, &CardObjType);
                if (EVEN(Sts)) return (Sts);

                if (CardObjType == pwr_cClass_Do_HVDO32 ||
                    CardObjType == pwr_cClass_Do_7437_83) /*Card object found*/
		{
			Sts = gdh_ObjidToName (CardObjid, CardName,
					       sizeof(pwr_tFullName), cdh_mNName);
	                if (EVEN(Sts)) return (Sts);
            		if(ChanNumber <= 15)		/* Use mask 1 */
                        {
				strcat (CardName, ".TESTMASK1");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}
                        else				/* Use mask 2 */	
			{
				strcat (CardName, ".TESTMASK2");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}

			if(BVal == 1)			/* Set bit in mask */
				TestMask |= (1 << ChanNumber);
                        else				/* Reset bit in mask */
				TestMask &= ~(1 << ChanNumber);

  			Sts = gdh_SetObjectInfo(CardName, &TestMask, 
						sizeof(pwr_tUInt16));
                        return(IO__SUCCESS);
                }
                Sts = gdh_GetNextSibling(CardObjid,&NewCardObjid);
                if(EVEN(Sts))
                	if(Sts != GDH__NO_SIBLING) return (Sts);

                if(Sts != GDH__NO_SIBLING) CardObjid = NewCardObjid;
            } /* End while */
	    break;
   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }
    return(Sts);
} /* END io_SetParTestOn */


/************************************************************************
*
* Name: io_GetParTestOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Ao, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					pwr_cClass_Ao -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine fetches the parameter TestOn in channel object.
*************************************************************************/
pwr_tInt32 io_GetParTestOn (
                pwr_tPathName		ObjName,
                pwr_tClassId		ObjType,                
                pwr_tAddress		ValueP)

{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFullName   FullName;
    pwr_tFullName   Name;
    pwr_tObjid      Id;    

    Sts = io_GetConInfo (ObjName, Name, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (FullName, Name); 
    strcat (FullName, ".TESTON");
    
    switch(ObjType)
    {
   	case pwr_cClass_Ao:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	case pwr_cClass_Do:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }

    return(Sts);
} /* END io_GetParTestOn */


/************************************************************************
*
* Name: io_SetParInvertOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					- pwr_cClass_Di, 
*					- pwr_cClass_Do
*
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information. 
*					pwr_cClass_Di -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine is used to change parameter InvertOn in channel
*   		object and one bit in parameter InvMask1 or 2 in card object.
*************************************************************************/
pwr_tInt32 io_SetParInvertOn (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)

{
    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tUInt16     ChanNumber;
    pwr_tUInt16	    TestMask;
    pwr_tObjid      CardObjid;
    pwr_tObjid      NewCardObjid;
    pwr_tClassId    CardObjType;	
    pwr_tFullName   TestOnName;
    pwr_tFullName   FullName;
    pwr_tFullName   CardName;
    pwr_tObjid	    Id;
    pwr_tBoolean    Found;


    Sts = io_GetConInfo (ObjName, FullName, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (TestOnName, FullName); 
    strcat (TestOnName, ".INVERTON");

    /** Store value in channel object **/
    
    BVal = *(pwr_tBoolean *)ValueP;
    Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
    if(EVEN(Sts)) return(Sts);


    strcat (FullName, ".NUMBER");
    Sts = gdh_GetObjectInfo(FullName, &ChanNumber, 
				    sizeof(pwr_tUInt16));
    if(EVEN(Sts)) return(Sts);

    /** Store value in card object **/

    Sts = gdh_GetParent(Id, &CardObjid);
    if(EVEN(Sts)) return(Sts);

    while(Sts != GDH__NO_SIBLING)
    {
      Found = FALSE;      

      Sts = gdh_GetObjectClass(CardObjid, &CardObjType);
      if (EVEN(Sts)) return (Sts);
      
      if(ObjType == pwr_cClass_Di)			/* DI object */
      {
        if (CardObjType == pwr_cClass_Di_DIX2 ||
            CardObjType == pwr_cClass_Di_7437_37)	/*DI card object found*/
          Found = TRUE;
      }
      else						/* DO object */ 
      {
        if (CardObjType == pwr_cClass_Do_HVDO32 ||
            CardObjType == pwr_cClass_Do_7437_83)	/*DO card object found*/
          Found = TRUE;
      }

      if (Found)
      {
        Sts = gdh_ObjidToName (CardObjid, CardName,
			       sizeof(pwr_tFullName), cdh_mNName);
	if (EVEN(Sts)) return (Sts);
        if(ChanNumber <= 15)				/* Use mask 1 */
        {
	  strcat (CardName, ".INVMASK1");
 	  Sts = gdh_GetObjectInfo(CardName, &TestMask, 
	  				sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
	}
        else						/* Use mask 2 */	
	{
	  strcat (CardName, ".INVMASK2");
 	  Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
	  if(EVEN(Sts)) return(Sts);
	}

	if(BVal == 1)			/* Set bit in mask */
		TestMask |= (1 << ChanNumber);
	else				/* Reset bit in mask */
		TestMask &= ~(1 << ChanNumber);

        Sts = gdh_SetObjectInfo(CardName, &TestMask, 
				sizeof(pwr_tUInt16));
        return(IO__SUCCESS);
      }

      Sts = gdh_GetNextSibling(CardObjid,&NewCardObjid);
      if(EVEN(Sts))
        if(Sts != GDH__NO_SIBLING) return (Sts);

      if(Sts != GDH__NO_SIBLING) CardObjid = NewCardObjid;

    } /* End while */

    return(Sts);
} /* END io_SetParInvertOn */


/************************************************************************
*
* Name: io_GetParInvertOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Di, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					pwr_cClass_Di -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine fetches the parameter InvertOn from channel object.
*************************************************************************/
pwr_tInt32 io_GetParInvertOn (
                pwr_tPathName		ObjName,
                pwr_tClassId		ObjType,                
                pwr_tAddress		ValueP)

{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFullName   FullName;
    pwr_tFullName   Name;
    pwr_tObjid      Id;    

    Sts = io_GetConInfo (ObjName, Name, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (FullName, Name); 
    strcat (FullName, ".INVERTON");
    
    switch(ObjType)
    {
   	case pwr_cClass_Di:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	case pwr_cClass_Do:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }

    return(Sts);
} /* END io_GetParInvertOn */


/************************************************************************
*
* Name: io_SetParConversionOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tObjid	ObjType         I       Type of object (numeric).
*					- pwr_cClass_Di, 
*					- pwr_cClass_Do
*
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information. 
*					pwr_cClass_Di -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Ai -> Pointer to a boolean 
*							 value.
*
* Description:  This routine is used to change parameter ConversionOn in 
*		channel object and one bit in parameter ConvMask1 or 2 in 
*		card object.
*************************************************************************/
pwr_tInt32 io_SetParConversionOn (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)

{
    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tUInt16     ChanNumber;
    pwr_tUInt16	    TestMask;
    pwr_tObjid      CardObjid;
    pwr_tObjid      NewCardObjid;
    pwr_tClassId    CardObjType;	
    pwr_tFullName   TestOnName;
    pwr_tFullName   FullName;
    pwr_tFullName   CardName;
    pwr_tObjid	    Id;
    pwr_tBoolean    Found;


    Sts = io_GetConInfo (ObjName, FullName, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (TestOnName, FullName); 
    strcat (TestOnName, ".CONVERSIONON");
    
    /** Store value in channel object **/
    BVal = *(pwr_tBoolean *)ValueP;
    Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
    if(EVEN(Sts)) return(Sts);


    strcat (FullName, ".NUMBER");
    Sts = gdh_GetObjectInfo(FullName, &ChanNumber, sizeof(pwr_tUInt16));
    if(EVEN(Sts)) return(Sts);

    /** Store value in card object **/
    Sts = gdh_GetParent(Id, &CardObjid);
    if(EVEN(Sts)) return(Sts);

    while(Sts != GDH__NO_SIBLING)
    {
      Found = FALSE;      

      Sts = gdh_GetObjectClass(CardObjid, &CardObjType);
      if (EVEN(Sts)) return (Sts);
      
      if(ObjType == pwr_cClass_Di)			/* DI object */
      {
        if (CardObjType == pwr_cClass_Di_DIX2 ||
            CardObjType == pwr_cClass_Di_7437_37)	/*DI card object found*/
          Found = TRUE;
      }

      if (Found)
      {
        Sts = gdh_ObjidToName (CardObjid, CardName, sizeof(pwr_tFullName), cdh_mNName);
	if (EVEN(Sts)) return (Sts);
        if(ChanNumber <= 15)				/* Use mask 1 */
        {
	  strcat (CardName, ".CONVMASK1");
 	  Sts = gdh_GetObjectInfo(CardName, &TestMask, sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
	}
        else						/* Use mask 2 */	
	{
	  strcat (CardName, ".CONVMASK2");
 	  Sts = gdh_GetObjectInfo(CardName, &TestMask, sizeof(pwr_tUInt16));
	  if(EVEN(Sts)) return(Sts);
	}

	if(BVal == 1)			/* Set bit in mask */
	  TestMask |= (1 << ChanNumber);
	else				/* Reset bit in mask */
	  TestMask &= ~(1 << ChanNumber);

        Sts = gdh_SetObjectInfo(CardName, &TestMask, sizeof(pwr_tUInt16));

        return(IO__SUCCESS);
      }

      Sts = gdh_GetNextSibling(CardObjid,&NewCardObjid);
      if(EVEN(Sts))
        if(Sts != GDH__NO_SIBLING) return (Sts);

      if(Sts != GDH__NO_SIBLING) CardObjid = NewCardObjid;

    } /* End while */

    return(Sts);
} /* END io_SetParConversionOn */


/************************************************************************
*
* Name: io_GetParConversionOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Di, 
*					pwr_cClass_Ai 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					pwr_cClass_Di -> Pointer to a boolean 
*							 value.
*					pwr_cClass_Ai -> Pointer to a boolean 
*							 value.
*
* Description:  This routine fetches the parameter ConversiOn from 
*		channel object.
*************************************************************************/
pwr_tInt32 io_GetParConversionOn (
                pwr_tPathName		ObjName,
                pwr_tClassId		ObjType,                
                pwr_tAddress		ValueP)

{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFullName   FullName;
    pwr_tFullName   Name;
    pwr_tObjid      Id;    

    Sts = io_GetConInfo (ObjName, Name, &Id);
    if(EVEN(Sts)) return(Sts);
    
    strcpy (FullName, Name); 
    strcat (FullName, ".CONVERSIONON");
    
    switch(ObjType)
    {
   	case pwr_cClass_Di:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	case pwr_cClass_Ai:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }

    return(Sts);
} /* END io_GetParConversionOn */


/************************************************************************
*
* Name: io_SetParFixedOutValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					- pwr_cClass_Ao, 
*					- pwr_cClass_Do
*
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information. 
*					pwr_cClass_Ao -> Pointer to a float 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine is used to change parameter FixedOutValue in 
*		channel object and one bit in parameter FixedOutValue1 or 2 
*		in card object.
*************************************************************************/
pwr_tInt32 io_SetParFixedOutValue (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)

{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFloat32    FVal;
    pwr_tUInt16     ChanNumber;
    pwr_tUInt16	    TestMask;
    pwr_tObjid      CardObjid;
    pwr_tObjid      NewCardObjid;
    pwr_tClassId    CardObjType;	
    pwr_tFullName   FullName;
    pwr_tFullName   CardName;
    pwr_tFullName   TestOnName;
    pwr_tObjid	    Id;
    

    Sts = io_GetConInfo (ObjName, FullName, &Id);
    if(EVEN(Sts)) return(Sts);


    strcpy (TestOnName, FullName); 
    strcat (TestOnName, ".FIXEDOUTVALUE");

    switch(ObjType)
    {
   	case pwr_cClass_Ao:
	    FVal = *(pwr_tFloat32 *)ValueP;
            Sts = gdh_SetObjectInfo(TestOnName, &FVal, sizeof(pwr_tFloat32));
            if(EVEN(Sts)) return(Sts);
	    break;

   	case pwr_cClass_Do:

	    /** Store value in channel object **/
	    BVal = *(pwr_tBoolean *)ValueP;
	    Sts = gdh_SetObjectInfo(TestOnName, &BVal, sizeof(pwr_tBoolean));
	    if(EVEN(Sts)) return(Sts);


	    strcat (FullName, ".NUMBER");
	    Sts = gdh_GetObjectInfo(FullName, &ChanNumber, 
				    sizeof(pwr_tUInt16));
	    if(EVEN(Sts)) return(Sts);

	    /** Store value in card object **/
	    Sts = gdh_GetParent(Id, &CardObjid);
	    if(EVEN(Sts)) return(Sts);

	    while(Sts != GDH__NO_SIBLING)
            {
            	Sts = gdh_GetObjectClass(CardObjid, &CardObjType);
                if (EVEN(Sts)) return (Sts);

                if (CardObjType == pwr_cClass_Do_HVDO32 ||
                    CardObjType == pwr_cClass_Do_7437_83) /*Card object found*/
		{
			Sts = gdh_ObjidToName (CardObjid, CardName,
					       sizeof(pwr_tFullName), cdh_mNName);
	                if (EVEN(Sts)) return (Sts);
            		if(ChanNumber <= 15)		/* Use mask 1 */
                        {
				strcat (CardName, ".FIXEDOUTVALUE1");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}
                        else				/* Use mask 2 */	
			{
				strcat (CardName, ".FIXEDOUTVALUE2");
 				Sts = gdh_GetObjectInfo(CardName, &TestMask, 
				                        sizeof(pwr_tUInt16));
				if(EVEN(Sts)) return(Sts);
			}
			if(BVal == 1)			/* Set bit in mask */
				TestMask |= (1 << ChanNumber);
                        else				/* Reset bit in mask */
				TestMask &= ~(1 << ChanNumber);

            		Sts = gdh_SetObjectInfo(CardName, &TestMask, 
						sizeof(pwr_tUInt16));
                        return(IO__SUCCESS);
                }
                Sts = gdh_GetNextSibling(CardObjid,&NewCardObjid);
                if (EVEN(Sts)) return (Sts);

                if(Sts != GDH__NO_SIBLING) CardObjid = NewCardObjid;
            } /* End while */
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }
    return(Sts);
} /* END io_SetParFixedOutValue */


/************************************************************************
*
* Name: io_GetParFixedOutValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
*				        ObjName must be the name of signal
*					object.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					pwr_cClass_Ao, 
*					pwr_cClass_Do 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					pwr_cClass_Ao -> Pointer to a float 
*							 value.
*					pwr_cClass_Do -> Pointer to a boolean 
*							 value.
*
* Description:  This routine fetches the parameter TestValue in channel object.
*************************************************************************/
pwr_tInt32 io_GetParFixedOutValue (
                pwr_tPathName	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP)
{

    pwr_tInt32	    Sts;
    pwr_tBoolean    BVal;
    pwr_tFloat32    FVal;
    pwr_tFullName   FullName;
    pwr_tFullName   Name;
    pwr_tObjid      Id;    

    Sts = io_GetConInfo (ObjName, Name, &Id);
    if(EVEN(Sts)) return(Sts);

    strcpy (FullName, Name); 
    strcat (FullName, ".FIXEDOUTVALUE");
    
    switch(ObjType)
    {
   	case pwr_cClass_Ao:
            Sts = gdh_GetObjectInfo(FullName, &FVal, sizeof(pwr_tFloat32));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tFloat32 *)ValueP = FVal;
            Sts = IO__SUCCESS;
	    break;

   	case pwr_cClass_Do:
            Sts = gdh_GetObjectInfo(FullName, &BVal, sizeof(pwr_tBoolean));
            if(EVEN(Sts)) return(Sts);
	    *(pwr_tBoolean *)ValueP = BVal;
            Sts = IO__SUCCESS;
	    break;

   	default:
            Sts = IO__BADOBJTYPE;
            break;	
    }
    return(Sts);

} /* END io_GetParFixedOutValue */


/************************************************************************
*
* Name: io_GetConInfo (FromName, ToName, ToId)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	FromName        I	NULL terminated object name string.
*				        FromName must be the name of signal
*					object or channel object.
* pwr_tPathName	ToName           O	NULL terminated object name string.
*				        Name of signal object or channel object.
* pwr_tObjid  	*ToId            O	Object iD of signal object or 
*				        channel object.
*
* Description:  This routine fetches the name of signal object or channel 
*               object and it's object id.
*************************************************************************/
pwr_tInt32 io_GetConInfo (
                pwr_tPathName		FromName,
                pwr_tPathName		ToName,
                pwr_tObjid           *ToId)
{

    pwr_tInt32	    Sts;
    pwr_tFullName        FullName;

    strcpy (FullName, FromName); 
    strcat (FullName, ".SIGCHANCON");

    Sts = gdh_GetObjectInfo(FullName, ToId, sizeof(pwr_tObjid));
    if(EVEN(Sts)) return(Sts);

    Sts = gdh_ObjidToName (*ToId, ToName, sizeof(pwr_tFullName), cdh_mNName);
    if(EVEN(Sts)) return(Sts);

    return(Sts);

} /* END io_GetConInfo */
