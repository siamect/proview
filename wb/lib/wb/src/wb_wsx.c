/* wb_wsx.c -- wb syntax control

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Wb syntax control.
   This module control the syntax for objects.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#endif
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
/* Temporary, in next version (3.0 ??) the name is changed... */
#undef pwr_cClass_Point
#include "pwr_systemclasses.h"
#include "wb_foe_macros.h"
#include "co_cdh.h"
#include "co_msg.h"
#include "wb_ldh.h"
#include "wb_trv.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"

static int wsx_object_count ( 
  pwr_tObjid	Objid,
  int		*count,
  int		dum1,
  int		dum2,
  int		dum3,
  int		dum4
);

/*_define _______________________________________________________*/

/*_Local procedues_______________________________________________________*/

static int wsx_object_count ( 
  pwr_tObjid	Objid,
  int		*count,
  int		dum1,
  int		dum2,
  int		dum3,
  int		dum4
)
{
	(*count)++;
	return WSX__SUCCESS;
}

/*_Backcalls for the controlled gredit module______________________________*/


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		wsx_error_msg()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Prints a error or warning message for an object and increments the 
*	errorcount or warningcount.
*
**************************************************************************/

pwr_tStatus wsx_error_msg( 
    ldh_tSesContext	sesctx,
    pwr_tStatus 	sts,
    pwr_tObjid		objid,
    int			*errorcount,
    int			*warningcount
)
{
	static char msg[256];
	int	status, size;
	char	name[80];
	FILE 		*logfile;

	logfile = NULL;

	if (EVEN(sts))
	{
	  msg_GetMsg( sts, msg, sizeof(msg));

	  if (logfile != NULL)
	    fprintf(logfile, "%s\n", msg);
	  else
	    printf("%s\n", msg);
	  if ( cdh_ObjidIsNotNull( objid))
	  {
	    /* Get the full hierarchy name for the node */
	    status = ldh_ObjidToName( sesctx, objid,
		ldh_eName_Default, name, sizeof( name), &size);
	    if( EVEN(status)) return status;
	    if (logfile != NULL)
	      fprintf(logfile, "        in object  %s\n", name);
	    else
	      printf("        in object  %s\n", name);
	  }
	  if ( (sts & 2) && !(sts & 1))
	    (*errorcount)++;
	  else if ( !(sts & 2) && !(sts & 1))
	    (*warningcount)++;
	}
	return WSX__SUCCESS;
}

/*************************************************************************
*
* Name:		wsx_CheckCard()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext sesctx	I	ldh session context.
* pwr_tObjid	objid		I	card objid.
*
* Description:
*	Check the syntax of a card.
**************************************************************************/

pwr_tStatus wsx_CheckCard( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount,
	wsx_mCardOption options
)
{
	int		sts;
	int		size;
	char		*buf_ptr;
	pwr_tUInt16	chan_max;
	int		chan_number_array[256];
	int		chan_count;
	pwr_tUInt16 	number;
	pwr_tObjid	chan_objid;
	pwr_tClassId	class;

	/* Check DevName */
	if ( options & wsx_mCardOption_DevName)
	{
	  sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"DevName",
			(char **)&buf_ptr, &size);
	  if (EVEN(sts)) return sts;
	  if ( strcmp( buf_ptr, "") == 0)
	    wsx_error_msg( sesctx, WSX__CARDDEVNAME, objid, errorcount, warningcount);
	  XtFree((char *) buf_ptr);
	}
	
	/* Check ErrorSoftLimit */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"ErrorSoftLimit",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( *(int *) buf_ptr == 0)
	  wsx_error_msg( sesctx, WSX__CARDERRSOFTLIM, objid, errorcount, warningcount);
	XtFree((char *) buf_ptr);
	
	/* Check ErrorHardLimit */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"ErrorHardLimit",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( *(int *) buf_ptr == 0)
	  wsx_error_msg( sesctx, WSX__CARDERRHARDLIM, objid, errorcount, warningcount);
	XtFree((char *) buf_ptr);

	/* Get MaxNoOfChannels */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"MaxNoOfChannels",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	
	chan_max = *(pwr_tUInt16 *) buf_ptr;
	XtFree((char *) buf_ptr);

	if ( chan_max > 256)
	{
	  wsx_error_msg( sesctx, WSX__MAXCHAN, objid, errorcount, warningcount);
	  return WSX__SUCCESS;
	}
	/* Check that Number in channel-objects are unique within the card */
	memset( chan_number_array, 0, sizeof( chan_number_array));
	chan_count = 0;
	sts = ldh_GetChild( sesctx, objid, &chan_objid);
	while (ODD(sts))
	{
	  sts = ldh_GetObjectClass ( sesctx, chan_objid, &class);
	  if ( EVEN(sts)) return sts;
	  switch ( class)
	  {
	    case pwr_cClass_ChanDi:
	    case pwr_cClass_ChanDo:
	    case pwr_cClass_ChanAi:
	    case pwr_cClass_ChanAit:
	    case pwr_cClass_ChanAo:
	      chan_count++;
	      sts = ldh_GetObjectPar( sesctx,
			chan_objid, 
			"RtBody",
			"Number",
			(char **)&buf_ptr, &size);
	      if (EVEN(sts)) return sts;
	      number = * (pwr_tUInt16 *) buf_ptr;
	      XtFree((char *) buf_ptr);
	      /* Check than number is within limits */
	      if ( number >= chan_max)
	      {
	        wsx_error_msg( sesctx, WSX__NUMRANGE, chan_objid, errorcount, warningcount);
	        break;
	      }
	      if ( chan_number_array[ number])
	        /* Number is occupied */
	        wsx_error_msg( sesctx, WSX__NUMNOTUNIQUE, chan_objid, errorcount, warningcount);
	      else
	        chan_number_array[ number] = 1;
	      break;
	    default:
	      wsx_error_msg( sesctx, WSX__MISPLACED, objid, errorcount, warningcount);
	  }
	  sts = ldh_GetNextSibling( sesctx, chan_objid, &chan_objid);
	}
	if ( chan_count > chan_max)
	  wsx_error_msg( sesctx, WSX__CHANCOUNT, objid, errorcount, warningcount);
	
	return WSX__SUCCESS;
}

/*************************************************************************
*
* Name:		wsx_CheckCoCard()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext sesctx	I	ldh session context.
* pwr_tObjid	objid		I	card objid.
*
* Description:
*	Check the syntax of a co card.
**************************************************************************/

pwr_tStatus wsx_CheckCoCard( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount	
)
{
	int		sts;
	int		size;
	char		*buf_ptr;
	pwr_tUInt16	chan_max;
	int		chan_count;
	pwr_tObjid	chan_objid;
	pwr_tClassId	class;

	/* Check DevName */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"DevName",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( strcmp( buf_ptr, "") == 0)
	  wsx_error_msg( sesctx, WSX__CARDDEVNAME, objid, errorcount, warningcount);
	XtFree((char *) buf_ptr);
	
	/* Check ErrorSoftLimit */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"ErrorSoftLimit",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( *(int *) buf_ptr == 0)
	  wsx_error_msg( sesctx, WSX__CARDERRSOFTLIM, objid, errorcount, warningcount);
	XtFree((char *) buf_ptr);
	
	/* Check ErrorHardLimit */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"ErrorHardLimit",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( *(int *) buf_ptr == 0)
	  wsx_error_msg( sesctx, WSX__CARDERRHARDLIM, objid, errorcount, warningcount);
	XtFree((char *) buf_ptr);

	/* Get MaxNoOfChannels */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"MaxNoOfCounters",
			(char **)&buf_ptr, &size);
	if (EVEN(sts)) return sts;
	
	chan_max = *(pwr_tUInt16 *) buf_ptr;
	XtFree((char *) buf_ptr);

	chan_count = 0;
	sts = ldh_GetChild( sesctx, objid, &chan_objid);
	while (ODD(sts))
	{
	  sts = ldh_GetObjectClass ( sesctx, chan_objid, &class);
	  if ( EVEN(sts)) return sts;
	  switch ( class)
	  {
	    case pwr_cClass_ChanCo:
	      chan_count++;
	      break;
	    default:
	      wsx_error_msg( sesctx, WSX__MISPLACED, objid, errorcount, warningcount);
	  }
	  sts = ldh_GetNextSibling( sesctx, chan_objid, &chan_objid);
	}
	if ( chan_count > chan_max)
	  wsx_error_msg( sesctx, WSX__CHANCOUNT, objid, errorcount, warningcount);
	
	return WSX__SUCCESS;
}

/*************************************************************************
*
* Name:		wsx_CheckSigChanCon()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext sesctx	I	ldh session context.
* pwr_tObjid	objid		I	card objid.
*
* Description:
*	Check SigChanCon in a signal or a channel.
**************************************************************************/

pwr_tStatus wsx_CheckSigChanCon( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount	
)
{
	int	sts;
	int	size;
	pwr_tObjid	*con_ptr;
	pwr_tObjid	*back_ptr;
	int		class_error;
	pwr_tClassId	class;
	pwr_tClassId	con_class;

	sts = ldh_GetObjectClass ( sesctx, objid, &class);
	if ( EVEN(sts)) return sts;
	
	/* Check SigChanCon */
	sts = ldh_GetObjectPar( sesctx,
			objid, 
			"RtBody",
			"SigChanCon",
			(char **)&con_ptr, &size);
	if (EVEN(sts)) return sts;
	if ( cdh_ObjidIsNull(*con_ptr))
	{
	  switch ( class)
	  {
	    case pwr_cClass_ChanDi:
	    case pwr_cClass_ChanDo:
	    case pwr_cClass_ChanAi:
	    case pwr_cClass_ChanAit:
	    case pwr_cClass_ChanAo:
	    case pwr_cClass_ChanCo:
	      /* Null objid is OK för channels */
	      return WSX__SUCCESS;
	    default:
	      wsx_error_msg( sesctx, WSX__SIGCHANCON, objid, errorcount, warningcount);
	      XtFree((char *) con_ptr);
	      return WSX__SUCCESS;
	  }
	}
	
	/* Check object class of connected object */
	sts = ldh_GetObjectClass ( sesctx, *con_ptr, &con_class);
	if ( EVEN(sts))
	{
	  wsx_error_msg( sesctx, WSX__SIGCHANCON, objid, errorcount, warningcount);
	  XtFree((char *) con_ptr);
	  return WSX__SUCCESS;
	}
	class_error = 0;
	switch ( class)
	{
	  case pwr_cClass_ChanDi:
	    if ( con_class != pwr_cClass_Di)
	      class_error = 1;
	    break;
	  case pwr_cClass_ChanDo:
	    if ( !(con_class == pwr_cClass_Do ||
	           con_class == pwr_cClass_Po))
	      class_error = 1;
	    break;
	  case pwr_cClass_ChanAi:
	  case pwr_cClass_ChanAit:
	    if ( con_class != pwr_cClass_Ai)
	      class_error = 1;
	    break;
	  case pwr_cClass_ChanAo:
	    if ( con_class != pwr_cClass_Ao)
	      class_error = 1;
	    break;
	  case pwr_cClass_ChanCo:
	    if ( con_class != pwr_cClass_Co)
	      class_error = 1;
	    break;
	  case pwr_cClass_Di:
	    if ( con_class != pwr_cClass_ChanDi)
	      class_error = 1;
	    break;
	  case pwr_cClass_Do:
	    if ( con_class != pwr_cClass_ChanDo)
	      class_error = 1;
	    break;
	  case pwr_cClass_Po:
	    if ( con_class != pwr_cClass_ChanDo)
	      class_error = 1;
	    break;
	  case pwr_cClass_Ai:
	    if ( !(con_class == pwr_cClass_ChanAi ||
		   con_class == pwr_cClass_ChanAit))
	      class_error = 1;
	    break;
	  case pwr_cClass_Ao:
	    if ( con_class != pwr_cClass_ChanAo)
	      class_error = 1;
	    break;
	  case pwr_cClass_Co:
	    if ( con_class != pwr_cClass_ChanCo)
	      class_error = 1;
	    break;
	}

	if ( class_error)
	{
	  wsx_error_msg( sesctx, WSX__SIGCHANCONCLASS, objid, errorcount, warningcount);
	  XtFree((char *) con_ptr);
	  return WSX__SUCCESS;
	}

	/* Check that the connections is mutual */
	sts = ldh_GetObjectPar( sesctx,
			*con_ptr, 
			"RtBody",
			"SigChanCon",
			(char **)&back_ptr, &size);
	if (EVEN(sts)) return sts;

	if ( cdh_ObjidIsNotEqual( *back_ptr, objid))
	{
	  wsx_error_msg( sesctx, WSX__SIGCHANCON, objid, errorcount, warningcount);
	  XtFree((char *) con_ptr);
	  XtFree((char *) back_ptr);
	  return WSX__SUCCESS;
	}

	XtFree((char *) con_ptr);
	XtFree((char *) back_ptr);
	return WSX__SUCCESS;
}
/*************************************************************************
*
* Name:		wsx_CheckVolume()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext sesctx	I	ldh session context.
* pwr_tObjid	objid		I	card objid.
*
* Description:
*	Check the syntax of a volume.
**************************************************************************/

pwr_tStatus wsx_CheckVolume( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount	
)
{
	pwr_tStatus sts;
	pwr_tUInt32 *opsys_ptr;
	pwr_tUInt32 opsys;
	pwr_tUInt32 opsys_sum;
	int size;
	int i;
	pwr_tClassId class;
	pwr_tClassId	class_vect[2];
	int node_object_count;  
	
	sts = ldh_GetObjectClass ( sesctx, objid, &class);
	if ( EVEN(sts)) return sts;

	/* Check number of $Node objects in the volume */
	class_vect[0] = pwr_cClass_Node;
	class_vect[1] = 0;
	node_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&node_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;

#if 0
	/* Dynamic size of valuebase objects !!! */
	int di_object_count;  
	int do_object_count;  
	int dv_object_count;  
	int ai_object_count;  
	int ao_object_count;  
	int av_object_count;  
	int co_object_count;  

	pwr_sClass_di_value_base *di_valuebase;
	pwr_sClass_do_value_base *do_valuebase;
	pwr_sClass_dv_value_base *dv_valuebase;
	pwr_sClass_ai_value_base *ai_valuebase;
	pwr_sClass_ao_value_base *ao_valuebase;
	pwr_sClass_av_value_base *av_valuebase;
	pwr_sClass_co_value_base *co_valuebase;


	/* Check number of signals objects in the volume */
	class_vect[0] = pwr_cClass_Di;
	class_vect[1] = 0;
	di_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&di_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( di_object_count > 
	    sizeof( di_valuebase->DIValue) / sizeof( di_valuebase->DIValue[0]))
  	  wsx_error_msg( sesctx, WSX__DICOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Do;
	class_vect[1] = 0;
	do_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&do_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( do_object_count > 
	    sizeof( do_valuebase->DOValue) / sizeof( do_valuebase->DOValue[0]))
  	  wsx_error_msg( sesctx, WSX__DOCOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Dv;
	class_vect[1] = 0;
	dv_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&dv_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( dv_object_count > 
	    sizeof( dv_valuebase->DVValue) / sizeof( dv_valuebase->DVValue[0]))
  	  wsx_error_msg( sesctx, WSX__DVCOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Ai;
	class_vect[1] = 0;
	ai_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&ai_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( ai_object_count > 
	    sizeof( ai_valuebase->AIValue) / sizeof( ai_valuebase->AIValue[0]))
  	  wsx_error_msg( sesctx, WSX__AICOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Ao;
	class_vect[1] = 0;
	ao_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&ao_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( ao_object_count > 
	    sizeof( ao_valuebase->AOValue) / sizeof( ao_valuebase->AOValue[0]))
  	  wsx_error_msg( sesctx, WSX__AOCOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Av;
	class_vect[1] = 0;
	av_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&av_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( av_object_count > 
	    sizeof( av_valuebase->AVValue) / sizeof( av_valuebase->AVValue[0]))
  	  wsx_error_msg( sesctx, WSX__AVCOUNT, objid, errorcount, warningcount);

	class_vect[0] = pwr_cClass_Co;
	class_vect[1] = 0;
	co_object_count = 0;
	sts = trv_get_objects_hier_class_name( sesctx, pwr_cNObjid,
		class_vect, NULL,
		&wsx_object_count,
		&co_object_count, 0, 0, 0, 0);
	if (EVEN(sts)) return sts;
	if ( co_object_count > 
	    sizeof( co_valuebase->COValue) / sizeof( co_valuebase->COValue[0]))
  	  wsx_error_msg( sesctx, WSX__COCOUNT, objid, errorcount, warningcount);
#endif
	switch (class)
 	{
	  case pwr_cClass_RootVolume:
	  case pwr_cClass_SubVolume:

	    if ( class == pwr_cClass_RootVolume)
	    {
	      if ( node_object_count != 1)
  	        wsx_error_msg( sesctx, WSX__NODECOUNT, objid, errorcount, warningcount);
	    }
	    else
	    {
	      if ( node_object_count != 0)
  	        wsx_error_msg( sesctx, WSX__NODECOUNT, objid, errorcount, warningcount);
	    }

	    /* Check OperatingSystem */
	    sts = ldh_GetObjectPar( sesctx,
  			objid,
			"SysBody",
			"OperatingSystem",
			(char **)&opsys_ptr, &size);
	    if ( EVEN(sts)) return sts;

	    opsys_sum = 0;
	    for ( i = 0;; i++)
	    {
    	      opsys = 1 << i;
	      opsys_sum |= opsys;
  	      if ( opsys >= pwr_mOpSys_)
  	        break;
	    }

  	    if ( *opsys_ptr & ~opsys_sum)
  	      wsx_error_msg( sesctx, WSX__OSINVALID, objid, errorcount, warningcount);

  	    XtFree( (char *) opsys_ptr);
	    break;

	  default:
	    if ( node_object_count != 0)
  	      wsx_error_msg( sesctx, WSX__NODECOUNT, objid, errorcount, warningcount);
	}
	return WSX__SUCCESS;
}

