#ifndef	rt_io_utils_h
#define	rt_io_utils_h

/* rt_io_utils.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
                pwr_tPathName	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP);

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
                pwr_tAddress		ValueP);


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
* pwr_tClassId	ObjType         I       Type of object (numeric).
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
                pwr_tAddress	ValueP);

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
                pwr_tAddress	ValueP);

/************************************************************************
*
* Name: io_SetParTestValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
* pwr_tClassid	ObjType         I       Type of object (numeric).
*					OT_AO, OT_DO (see pinc$:ot_def.h). 
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information.
*					OT_AO -> Pointer to a float value.
*					OT_DO -> Pointer to a boolean value.
*
* Description:  This routine is used to change parameter TestValue in channel
*   		object and parameters TestValue1,2 in card object.
*************************************************************************/
extern pwr_tInt32 io_SetParTestValue (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP);


/************************************************************************
*
* Name: io_GetParTestValue(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					OT_AO, OT_DO (see pinc$:ot_def.h). 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					OT_AO -> Pointer to a float value.
*					OT_DO -> Pointer to a boolean value.
*
* Description:  This routine fetches the parameter TestValue in channel object.
*************************************************************************/
extern pwr_tInt32 io_GetParTestValue (
                pwr_tPathName	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP);


/************************************************************************
*
* Name: io_SetParTestOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					OT_AO, OT_DO (see pinc$:ot_def.h). 
* pwr_tAddress	ValueP          I       Pointer to a buffer filled with 
*				        information. 
*					OT_AO -> Pointer to a boolean value.
*					OT_DO -> Pointer to a boolean value.
*
* Description:  This routine is used to change parameter TestOn in channel
*   		object and parameters TestMask1,2 in card object.
*************************************************************************/
extern pwr_tInt32 io_SetParTestOn (
                pwr_tPathName  	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP);


/************************************************************************
*
* Name: io_GetParTestOn(ObjName, ObjType, ValueP)
*
* Type: pwr_tInt32   return value is a standard format condition code.
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* pwr_tPathName	ObjName         I	NULL terminated object name string.
* pwr_tClassId	ObjType         I       Type of object (numeric).
*					OT_AO, OT_DO (see pinc$:ot_def.h). 
* pwr_tAddress	ValueP           O      Pointer to a buffer.
*					The buffer is filled with the  
*				        requested information.
*					OT_AO -> Pointer to a boolean value.
*					OT_DO -> Pointer to a boolean value.
*
* Description:  This routine fetches the parameter TestOn in channel object.
*************************************************************************/
extern pwr_tInt32 io_GetParTestOn (
                pwr_tPathName	ObjName,
                pwr_tClassId	ObjType,                
                pwr_tAddress	ValueP);

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
                pwr_tAddress	ValueP);

#endif /* if defined */
