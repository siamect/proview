/* 
 * Proview   $Id: ra_rtt_rttsys.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/************************************************************************
* Description:
*	Application functions in rtt.
**************************************************************************/

/*_Include files_________________________________________________________*/
#include "pwr_inc:rt_rtt_menu.h"
#include "pwr_inc:rt_rtt_msg.h"

/*_Local rtt database____________________________________________________*/
RTT_DB_START
#include "pwrp_rtt:dtt_appl_RTTSYS_m.rdb1"
RTT_DB_CONTINUE
#include "pwrp_rtt:dtt_appl_RTTSYS_m.rdb2"
RTT_DB_END

/*************************************************************************
*
* Name:		RTTSYS_SHOW_NODES
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	context of the picture.
* int		event		I 	type of event.
* char		*parameter_ptr	I	pointer to the parameter which value
*					has been changed.
*
* Description:
*	Application function of a picture.
*
**************************************************************************/

int RTTSYS_SHOW_NODES ( ctx, event, parameter_ptr)
menu_ctx	ctx;
int		event;
char		*parameter_ptr;
{ 

  switch ( event)
  {
    /**********************************************************
    *	Initialization of the picture
    ***********************************************************/
    case RTT_APPL_INIT:
      break;
    /**********************************************************
    *	Udate of the picture
    ***********************************************************/
    case RTT_APPL_UPDATE:
      break;
    /**********************************************************
    *	Exit of the picture
    ***********************************************************/
    case RTT_APPL_EXIT:
      break;
    /**********************************************************
    *	The value of a parameter is changed.
    ***********************************************************/
    case RTT_APPL_VALUECHANGED:
      break;
    /**********************************************************
    *	Next page key activated.
    ***********************************************************/
    case RTT_APPL_NEXTPAGE:
      break;
    /**********************************************************
    *	Pevious page key activated.
    ***********************************************************/
    case RTT_APPL_PREVPAGE:
      break;
  }
  return RTT__SUCCESS;
}
