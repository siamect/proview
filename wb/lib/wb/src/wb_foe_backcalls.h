#ifndef wb_foe_backcalls_h
#define wb_foe_backcalls_h

/* wb_foe_backcalls.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
