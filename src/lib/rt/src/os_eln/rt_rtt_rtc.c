/* 
 * Proview   $Id: rt_rtt_rtc.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_rtt_rtc.c 
   This module contains routines for communication with real time clock
   processes from rtt. */

/*_Include files_________________________________________________________*/

#if defined(OS_ELN)
# include $vaxelnc
# include stdio
# include stdlib
# include string
# include ctype
# include chfdef
# include descrip
# include $elnmsg
# include $kernelmsg

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh"
#include "rt_rtt.h"
#include "rt_rtt_global.h"
#include "rt_rtt_functions.h"
#include "rt_rtt_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))


int rtt_rtc( int	function)
{
	int	sts;
	unsigned int  job_result;
	int		     *exit_sts;
	int		     size;
	PORT	job,job_exit;
	MESSAGE msg_id;
	static $DESCRIPTOR( jobdsc, "rs_rtctime");
	static $DESCRIPTOR( show_dsc, "show");
	static $DESCRIPTOR( get_dsc, "get");
	static $DESCRIPTOR( set_dsc, "set");
	$DESCRIPTOR (arg1, "");
	$DESCRIPTOR (arg2, "");
	$DESCRIPTOR (arg3, "");
	VARYING_STRING(255)	arg1_string;
	VARYING_STRING(255)	arg2_string;
	VARYING_STRING(255)	arg3_string;

	eln$program_argument (&arg1_string, 1);	/* Get stdin */
	arg1.dsc$w_length  = arg1_string.count;
	arg1.dsc$a_pointer = &arg1_string.data;

	eln$program_argument (&arg2_string, 2);	/* Get stdout */
	arg2.dsc$w_length  = arg2_string.count;
	arg2.dsc$a_pointer = &arg2_string.data;

	eln$program_argument (&arg3_string, 3);	/* Get stderr */
	arg3.dsc$w_length  = arg3_string.count;
	arg3.dsc$a_pointer = &arg3_string.data;

	rtt_cursor_abs( 0, RTT_ROW_MESSAGE - 1);
	r_print_buffer();

	ker$create_port(&sts,&job_exit,1);
	if ( EVEN(sts))
	{
	  rtt_message( 'E', "Error creating port");
	  return RTT__NOPICTURE;
	}
	if ( function == RTT_RTC_SHOW)
	  ker$create_job(	&sts,	 	/* Status */
				&job,	 	/* Jobbport */
				&jobdsc, 	/* Programnamn */
				&job_exit,	/* Exitport */
				 &arg1, &arg2, &arg3,
				 &show_dsc); 
	else if ( function == RTT_RTC_GET)
	  ker$create_job(	&sts,	 	/* Status */
				&job,	 	/* Jobbport */
				&jobdsc, 	/* Programnamn */
				&job_exit,	/* Exitport */
				 &arg1, &arg2, &arg3,
				 &get_dsc); 
	else if ( function == RTT_RTC_SET)
	  ker$create_job(	&sts,	 	/* Status */
				&job,	 	/* Jobbport */
				&jobdsc, 	/* Programnamn */
				&job_exit,	/* Exitport */
				 &arg1, &arg2, &arg3,
				 &set_dsc);
	if (sts == KER$_NO_SUCH_PROGRAM)
	{
	  rtt_message( 'E', "Rtc program is not loaded");
	  return RTT__NOPICTURE;
	}
	else if ( EVEN(sts))
	{
	  rtt_message( 'E', "Error creating job");
	  return RTT__NOPICTURE;
	}
	ker$wait_any(&sts,&job_result,NULL,&job_exit);
	ker$receive(&sts, &msg_id, &exit_sts, &size, &job_exit, NULL, NULL);

	ker$delete(&sts, &job_exit);

	return RTT__NOPICTURE;
}

#endif
