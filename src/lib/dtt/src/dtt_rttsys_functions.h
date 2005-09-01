/* 
 * Proview   $Id: dtt_rttsys_functions.h,v 1.2 2005-09-01 14:57:52 claes Exp $
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

int	rttsys_start_grafcet_monitor(
			menu_ctx	ctx,
			pwr_tObjid	plc_objid);
int rttsys_start_system_picture(
			menu_ctx	ctx,
			char		*picture_name);
int rttsys_get_current_logg_entry( int *entry);

int	rttsys_get_login_picture( rtt_t_backgr **picture);

/*
int RTTSYS_SHOW_SUBSRV( menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_SHOW_SUBCLI( menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_SHOW_NODES( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_OBJECT_PID( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_OBJECT_AV( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_GRAFCET( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_GRAFCET_PLC( 	menu_ctx	ctx,
				int		event,
				char		*parameter_ptr,
				char		*objectname,
				char		**picture);
int RTTSYS_PLCPGM( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_ERROR( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_DEVICE( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_WATCHDOG(	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_PID( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_LOGGING( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_SHOW_SYS( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_VMSPROC( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_OBJECT_CELL( menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);
int RTTSYS_NMPSCELL( 	menu_ctx	ctx,
			int		event,
			char		*parameter_ptr,
			char		*objectname,
			char		**picture);

*/
