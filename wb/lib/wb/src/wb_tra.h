/* 
 * Proview   $Id: wb_tra.h,v 1.6 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_tra_h
#define wb_tra_h

#ifndef rt_gdh_h
#include "rt_gdh.h"
#endif

#ifndef wb_vldh_h
#include "wb_vldh.h"
#endif

#ifndef wb_gre_h
#include "wb_gre.h"
#endif

class WFoe;

int trace_get_attributes( 	WGre		*grectx, 
				vldh_t_node	node, 
				char		*object_str, 
				char		*attr_str,
				flow_eTraceType	*trace_type,
				int		*inverted);
pwr_tStatus trace_get_attr_con( 	WGre		*grectx, 
					vldh_t_con	con,
					const char     	*debug_par,
					char		*object_str, 
					char		*attr_str,
					flow_eTraceType	*trace_type);

pwr_tStatus trace_simsetup( WFoe *foectx);
pwr_tStatus trace_trasetup( WFoe *foectx);
pwr_tStatus trace_start( WFoe *foectx);
pwr_tStatus trace_stop( WFoe *foectx);

int trace_create_analyse( 	WGre *grectx, 
				double x, 
				double y, 
				vldh_t_node source, 
				int source_conpoint);
int	trace_save( WGre *grectx);
int	trace_restore( WGre *grectx);
#endif
