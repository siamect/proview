#ifndef wb_gsx_h
#define wb_gsx_h

/* wb_gsx.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
