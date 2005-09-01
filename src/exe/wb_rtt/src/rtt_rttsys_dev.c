/* 
 * Proview   $Id: rtt_rttsys_dev.c,v 1.2 2005-09-01 14:57:49 claes Exp $
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
*
* Filename:		rtt_rttsys_dev.c
*			Date	Pgm.	Read.	Remark
* Modified		970502	CS		Initial creation
*
* Description:
*	This file declares the external functions in rttsys.
*	ds_rtt will link agaist this file to avoid the the moment 22
* 	situation : ds_rtt can't be linked befor ra_rttsys i compiled
*	and ra_rttsys can't be compiled before ds_rtt exist.
*
**************************************************************************/

/*_Include files_________________________________________________________*/

#include <stdio.h>

#include "rt_rtt.h"
#include "rt_rtt_edit.h"
#include "rt_rtt_menu.h"
#include "rt_rtt_msg.h"
#include "dtt_rttsys_functions.h"

RTT_RTTSYSDB_START
RTT_RTTSYSDB_CONTINUE
RTT_RTTSYSDB_END

int	rttsys_start_grafcet_monitor(
			menu_ctx	ctx,
			pwr_tObjid	plc_objid) 
{return RTT__SUCCESS;}

int rttsys_start_system_picture(
			menu_ctx	ctx,
			char		*picture_name)
{return RTT__SUCCESS;}

int rttsys_get_current_logg_entry( int *entry)
{return RTT__SUCCESS;}

int	rttsys_get_login_picture( rtt_t_backgr **picture)
{return RTT__SUCCESS;}

int RTTSYS_SHOW_NODES
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_NODE
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_SHOW_SUBCLI
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_SHOW_SUBSRV
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_GRAFCET
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_GRAFCET_PLC
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_PLCPGM
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_ERROR
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_DEVICE
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_CHANDI
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_CHANDO
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_CHANAI
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_CHANAO
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_CHANCO
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_WATCHDOG
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_PID
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_LOGGING
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_SHOW_SYS
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_VMSPROC
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_SHOW_SYS_ELN
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_ELNPROC
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_NMPSCELL
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_REMNODE
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_REMTRANS
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_OBJECT_PID
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_OBJECT_AV
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
int RTTSYS_OBJECT_NMPSCELL
		( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture)
	{return RTT__SUCCESS;}
