#ifndef wb_gobj_h
#define wb_gobj_h

/* wb_gobj.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef wb_foe_h
#include "wb_foe.h"
#endif

int     gobj_get_object(
        foe_ctx         foectx,
        vldh_t_node     node,
        unsigned long   index
);

int	gobj_expand(	foe_ctx		foectx,
			vldh_t_node	node,
			int		compress);
#endif
