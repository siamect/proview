/* 
 * Proview   $Id: wb_gsx.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_gsx_h
#define wb_gsx_h

int	gsx_check_connection( 
	foe_ctx		foectx,
	vldh_t_node	sourceobject,
	unsigned long	sourcepoint,
	vldh_t_node	destobject,
	unsigned long	destpoint,
	pwr_tClassId	*conclass,
	pwr_tClassId	user_conclass
);

int gsx_check_subwindow( 
	vldh_t_node	object,
	unsigned long	*subwindow_nr,
	foe_eFuncAccess	*function_access
);

int gsx_auto_create( 
	foe_ctx		foectx,
	double 		x,
	double		y,
	vldh_t_node	source,
	unsigned long	sourcepoint,
	vldh_t_node	*dest,
	unsigned long	*destpoint
);

#endif
