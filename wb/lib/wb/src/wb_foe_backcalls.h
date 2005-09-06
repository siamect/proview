/* 
 * Proview   $Id: wb_foe_backcalls.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_foe_backcalls_h
#define wb_foe_backcalls_h

void foe_gre_setup_window( 
    gre_ctx	grectx
);

void foe_gre_node_created (
    gre_ctx	grectx,
    unsigned long	current_node_type,
    float	x,
    float	y
);

void 	foe_gre_node_moved (
    gre_ctx	grectx
);

void 	foe_gre_node_selected (
    gre_ctx	grectx
);

void 	foe_gre_con_selected (
    gre_ctx	grectx
);

void 	foe_gre_con_created (
	gre_ctx			grectx,
	double			x,
	double			y,
	vldh_t_node		source_obj,
	unsigned long		source_point,
	vldh_t_node		destination_obj,
	unsigned long		destination_point
);

void 	foe_gre_region_selected (
    gre_ctx	grectx
);

void 	foe_gre_enter_leave (
    gre_ctx	grectx
);

void foe_gre_delete (
    gre_ctx		grectx,
    void		*object,
    unsigned long	object_type
);

void foe_gre_cut (
	gre_ctx		grectx,
	void		*object,
	unsigned long	object_type
);

void foe_gre_copy (
    gre_ctx	    grectx,
    void	    *object,
    unsigned long   object_type
);

void foe_gre_paste (
    gre_ctx	grectx,
    float		x,
    float		y
);

void foe_gre_attribute (
    gre_ctx	grectx,
    vldh_t_node	object
);

void foe_gre_subwindow (
    gre_ctx		grectx,
    vldh_t_node 	object,
    unsigned long	subwindow_nr
);

void foe_gre_popupmenu (
    gre_ctx	grectx,
    int	x_pix,
    int	y_pix,
    int	popupmenu_type,
    vldh_t_node	node
);

void foe_gre_getobj (
    gre_ctx	grectx,
    vldh_t_node node,
    unsigned long	index
);

void foe_gre_undelete (
    gre_ctx	grectx
);

void foe_gre_unselect (
    gre_ctx	grectx
);

void foe_gre_help (
    gre_ctx	grectx,
    char	*help_title
);

void foe_gre_regionmoved (
    gre_ctx	grectx
);

void foe_gre_message (
    gre_ctx	grectx,
    char	*message
);













#endif
