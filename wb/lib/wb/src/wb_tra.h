#ifndef wb_tra_h
#define wb_tra_h

/* wb_tra.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef rt_gdh_h
#include "rt_gdh.h"
#endif

#ifndef wb_vldh_h
#include "wb_vldh.h"
#endif

#ifndef _XtIntrinsic_h 
#include <X11/Intrinsic.h>
#endif

#ifndef wb_foe_h
#include "wb_foe.h"
#endif

#ifndef wb_gre_h
#include "wb_gre.h"
#endif

int trace_get_attributes( 	gre_ctx		grectx, 
				vldh_t_node	node, 
				char		*object_str, 
				char		*attr_str,
				flow_eTraceType	*trace_type);
pwr_tStatus trace_get_attr_con( 	gre_ctx		grectx, 
					vldh_t_con	con,
					char		*debug_par,
					char		*object_str, 
					char		*attr_str,
					flow_eTraceType	*trace_type);

pwr_tStatus trace_simsetup( foe_ctx foectx);
pwr_tStatus trace_trasetup( foe_ctx foectx);
pwr_tStatus trace_start( foe_ctx foectx);
pwr_tStatus trace_stop( foe_ctx foectx);

int trace_create_analyse( 	gre_ctx grectx, 
				double x, 
				double y, 
				vldh_t_node source, 
				int source_conpoint);
int	trace_save( gre_ctx grectx);
int	trace_restore( gre_ctx grectx);
#endif
