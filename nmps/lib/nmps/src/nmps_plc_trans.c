/* 
 * Proview   $Id: nmps_plc_trans.c,v 1.1 2006-01-12 05:57:43 claes Exp $
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


#if defined(OS_VMS) || defined(OS_LYNX)
#include <stdio.h>
#include <float.h>
#include <string.h>
#endif

#ifdef OS_ELN
#include stdio
#include float
#include string
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "rt_gdh.h"
#include "rt_plc.h"
#include "nmps_cnv.h"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define	NMPS_REQUESTFUNC_ACCEPTBOTH	16

void datacnv_init( pwr_sClass_datacnv  *object)
{
	int	sts;

	sts = cvn_ConvInit( object->ToConvdefType,
			    object->ToConvdef,
	 	   	    object->ToConvdefFile,
			    object->FromConvdefType,
			    object->FromConvdef,
	 	   	    object->FromConvdefFile,
			    &object->ConvTableCount,
			    0,
			    (char **) object->ConvTable);
}

void datacnv_exec( 
  plc_sThread		*tp,
  pwr_sClass_datacnv  *object)
{
	if ( *object->ConditionP)
	  cnv_ConvertData(
			(cnv_t_conv_table *) object->ConvTable,
			object->ConvTableCount,
			(char *)(* object->DataFromP),
			(char *)(* object->DataToP));
}

void DataRequest_exec( 
  plc_sThread		*tp,
  pwr_sClass_DataRequest  *object)
{
	object->DataReceived = 0;
	object->Error = 0;

	if ( *object->TriggP && !object->TriggOld)
	  object->TriggDetected = 1;
	if ( object->OpTrigg)
	{
	  object->TriggDetected = 1;
	  object->OpTrigg = 0;
	}
	if ( object->Function & NMPS_REQUESTFUNC_ACCEPTBOTH)
	{
	  if ( *object->AcceptP && !object->AcceptOld)
	    object->SysAcceptDetected = 1;
	  if ( object->OpAccept)
	  {
	    object->OpAccept = 0;
	    object->OpAcceptDetected = 1;
	  }
	  if ( object->OpAcceptDetected && object->SysAcceptDetected)
	  {
	    object->AcceptDetected = 1;
	    object->OpAcceptDetected = 0;
	    object->SysAcceptDetected = 0;
	  }
	}
	else
	{
	  if ( *object->AcceptP && !object->AcceptOld)
	    object->AcceptDetected = 1;
	  if ( object->OpAccept)
	  {
	    object->OpAccept = 0;
	    object->AcceptDetected = 1;
	  }
	}
	if ( *object->ResetP && !object->ResetOld)
	{
	  object->ResetDetected = 1;
	  object->OpAcceptDetected = 0;
	  object->SysAcceptDetected = 0;
	}
	if ( object->OpReset)
	{
	  object->ResetDetected = 1;
	  object->OpReset = 0;
	  object->OpAcceptDetected = 0;
	  object->SysAcceptDetected = 0;
	}

	if ( object->DataRcvDetected)
	{
	  object->DataReceived = 1;
	  object->DataRcvDetected = 0;
	}
	if ( object->ErrorDetected)
	{
	  object->Error = 1;
	  object->ErrorDetected = 0;
	}
	object->TriggOld = *object->TriggP;
	object->AcceptOld = *object->AcceptP;
	object->ResetOld = *object->ResetP;
}


void DataRcv_exec( 
  plc_sThread		*tp,
  pwr_sClass_DataRcv  *object)
{
	object->DataReceived = 0;
	object->Error = 0;

	if ( object->Function & NMPS_REQUESTFUNC_ACCEPTBOTH)
	{
	  if ( *object->AcceptP && !object->AcceptOld)
	    object->SysAcceptDetected = 1;
	  if ( object->OpAccept)
	  {
	    object->OpAccept = 0;
	    object->OpAcceptDetected = 1;
	  }
	  if ( object->OpAcceptDetected && object->SysAcceptDetected)
	  {
	    object->AcceptDetected = 1;
	    object->OpAcceptDetected = 0;
	    object->SysAcceptDetected = 0;
	  }
	}
	else
	{
	  if ( *object->AcceptP && !object->AcceptOld)
	    object->AcceptDetected = 1;
	  if ( object->OpAccept)
	  {
	    object->OpAccept = 0;
	    object->AcceptDetected = 1;
	  }
	}
	if ( *object->ResetP && !object->ResetOld)
	{
	  object->ResetDetected = 1;
	  object->OpAcceptDetected = 0;
	  object->SysAcceptDetected = 0;
	}
	if ( object->OpReset)
	{
	  object->ResetDetected = 1;
	  object->OpReset = 0;
	  object->OpAcceptDetected = 0;
	  object->SysAcceptDetected = 0;
	}

	if ( object->DataRcvDetected)
	{
	  object->DataReceived = 1;
	  object->DataRcvDetected = 0;
	}
	if ( object->ErrorDetected)
	{
	  object->Error = 1;
	  object->ErrorDetected = 0;
	}
	object->AcceptOld = *object->AcceptP;
	object->ResetOld = *object->ResetP;
}


void DataSend_exec( 
  plc_sThread		*tp,
  pwr_sClass_DataSend  *object)
{
	object->Error = 0;
	object->DataSent = 0;

	if ( *object->TriggP && !object->TriggOld)
	  object->TriggDetected = 1;

	if ( object->ErrorDetected)
	{
	  object->Error = 1;
	  object->ErrorDetected = 0;
	}
	if ( object->DataSentDetected)
	{
	  object->DataSent = 1;
	  object->DataSentDetected = 0;
	}
	object->TriggOld = *object->TriggP;
}

