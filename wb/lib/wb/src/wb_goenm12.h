/* 
 * Proview   $Id: wb_goenm12.h,v 1.1 2008-01-17 14:22:31 claes Exp $
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

#ifndef wb_goenm12_h
#define wb_goenm12_h

int goen_create_nodetype_m12( pwr_sGraphPlcNode	*graphbody,
			     pwr_tClassId	cid,
			     ldh_tSesContext	ldhses,
			     flow_tCtx		ctx,
			     unsigned long 	*mask,
			     unsigned long	subwindowmark,
			     unsigned long	node_width,
			     flow_tNodeClass	*node_class,
			     vldh_t_node	node);

int goen_get_point_info_m12( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
			    unsigned long point, unsigned long *mask, 
			    unsigned long node_width, goen_conpoint_type *info_pointer, 
			    vldh_t_node node);

int	goen_get_parameter_m12( pwr_sGraphPlcNode *graphbody, pwr_tClassId cid, 
			       ldh_tSesContext ldhses, unsigned long con_point, 
			       unsigned long *mask, unsigned long *par_type, 
			       unsigned long *par_inverted, unsigned long *par_index);

int goen_get_location_point_m12( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
				unsigned long *mask, unsigned long node_width, 
				goen_point_type *info_pointer, vldh_t_node node);
#endif
