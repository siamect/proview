/* 
 * Proview   $Id: wb_gobj.h,v 1.4 2007-01-04 07:29:03 claes Exp $
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

#ifndef wb_gobj_h
#define wb_gobj_h

#ifndef wb_foe_h
#include "wb_foe.h"
#endif

int     gobj_get_object(
        WFoe            *foe,
        vldh_t_node     node,
        unsigned long   index
);

int	gobj_expand(	WFoe		*foe,
			vldh_t_node	node,
			int		compress);
#endif
