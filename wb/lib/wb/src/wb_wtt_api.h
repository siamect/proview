/* 
 * Proview   $Id: wb_wtt_api.h,v 1.4 2005-09-06 10:43:32 claes Exp $
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
 **/

int wnav_foe_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjDId	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	void		**return_foectx,
	int		map_window,
	ldh_eAccess	access,
	pwr_tOid	oid);
