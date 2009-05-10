/* 
 * Proview   $Id: wb_goenm9.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_goenm9.c 

   This module creates a nodetype that if only the first input
   or output should be shown calls method 7, else method 0 is called.  */

#include <stdio.h>
#include <math.h>

#include "pwr.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "wb_ldh.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_goenm9.h"
#include "wb_goenm0.h"
#include "wb_goenm2.h"
#include "wb_goenm7.h"
#include "wb_goenm15.h"

/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m9()
*
* Type		
*
* Type		Parameter	IOGF	Description
*    pwr_sGraphPlcNode	*graphbody	Pointer to objecttype data
*    Widget	        widget			Neted widget
*    unsigned long 	*mask			Mask for drawing inputs/outputs
*    int		color			Highlight color
*    Cursor		cursor			Hot cursor
*    unsigned long      *node_type_id		Nodetypeid for created nodetype
*
* Description:
*	Create a nodetype
*
**************************************************************************/


int goen_create_nodetype_m9( 
    pwr_sGraphPlcNode	*graphbody,
    pwr_tClassId	cid,
    ldh_tSesContext	ldhses,
    flow_tCtx		ctx,
    unsigned int 	*mask,
    unsigned long	subwindowmark,
    unsigned long	node_width,
    flow_tNodeClass	*node_class,
    vldh_t_node		node)
{
	int sts;
	char		annot_str[3][80];
	int		annot_nr[3];
	int		annot_count;


	if (( *mask <= 1) && ( *(mask + 1) <= 1) )
	  sts = goen_create_nodetype_m7( graphbody, cid, ldhses, ctx, 
			mask, subwindowmark, node_width, node_class,
			node);
	else if ( *(mask + 1) <= 1)
	  sts = goen_create_nodetype_m15( graphbody, cid, ldhses, ctx, 
			mask, subwindowmark, node_width, node_class,
			node);
	else
	{
	  sts = WGre::get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
	  if ( EVEN(sts)) return sts;

	  if ( annot_count > 1)
	    sts = goen_create_nodetype_m2( graphbody, cid, ldhses, ctx, 
			mask, subwindowmark, node_width, node_class,
			node);
	  else
	    sts = goen_create_nodetype_m0( graphbody, cid, ldhses, ctx, 
			mask, subwindowmark, node_width, node_class,
			node);
	}
	return sts;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m9()
*
* Type		
*
* Type		Parameter	IOGF	Description
*    pwr_sGraphPlcNode	*graphbody	Pointer to objecttype data
*    unsigned long	point			Connection point nr
*    unsigned long 	*mask			Mask for drawing inputs/outputs
*    goen_conpoint_type	*info_pointer		Pointer to calculated data
*
* Description:
*	Calculates relativ koordinates for a connectionpoint and investigates
*	the connectionpoint type.
*
**************************************************************************/
int goen_get_point_info_m9( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
			    unsigned long point, unsigned int *mask, 
			    unsigned long node_width, goen_conpoint_type *info_pointer, 
			    vldh_t_node node)
{
	int sts;
	char		annot_str[3][80];
	int		annot_nr[3];
	int		annot_count;

	if (( *mask <= 1) && ( *(mask + 1) <= 1) )
	  sts = goen_get_point_info_m7( grectx, graphbody, point, mask, node_width, 
		info_pointer, node);
	else if ( *(mask + 1) <= 1)
	  sts = goen_get_point_info_m15( grectx, graphbody, point, mask, node_width, 
		info_pointer, node);
	else
	{
	  sts = WGre::get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
	  if ( EVEN(sts)) return sts;

	  if ( annot_count > 1)
	    sts = goen_get_point_info_m2( grectx, graphbody, point, mask, node_width, 
		info_pointer, node);
	  else
	    sts = goen_get_point_info_m0( grectx, graphbody, point, mask, node_width, 
		info_pointer, node);

	}
	return sts;
}



/*************************************************************************
*
* Name:		goen_get_parameter_m9()
*
* Type		
*
* Type		Parameter	IOGF	Description
*    pwr_sGraphPlcNode	*graphbody	Pointer to objecttype data
*    unsigned long	point			Connection point nr
*    unsigned long 	*mask			Mask for drawing inputs/outputs
*    unsigned long	*par_type		Input or output parameter	
*    godd_parameter_type **par_pointer		Pointer to parameter data
*
* Description:
*	Gets pointer to parameterdata for connectionpoint.
*
**************************************************************************/
int	goen_get_parameter_m9( pwr_sGraphPlcNode *graphbody, pwr_tClassId cid, 
			       ldh_tSesContext ldhses, unsigned long con_point, 
			       unsigned int *mask, unsigned long *par_type, 
			       unsigned long *par_inverted, unsigned long *par_index)
{
	int sts;

	if (( *mask <= 1) && ( *(mask + 1) <= 1) )
	  sts = goen_get_parameter_m7( graphbody, cid, ldhses,
			con_point, mask, par_type, par_inverted, par_index);
	else if ( *(mask + 1) <= 1)
	  sts = goen_get_parameter_m15( graphbody, cid, ldhses,
			con_point, mask, par_type, par_inverted, par_index);
	else
	  sts = goen_get_parameter_m0( graphbody, cid, ldhses,
			con_point, mask, par_type, par_inverted, par_index);
	return sts;
}


/*************************************************************************
*
* Name:		goen_get_location_point_m9()
*
* Type		
*
* Type		Parameter	IOGF	Description
*    pwr_sGraphPlcNode	*graphbody	Pointer to objecttype data
*    goen_point_type	*info_pointer		Locationpoint
*
* Description:
*	Calculates kooridates for locationpoint relativ geomtrical center.
*
**************************************************************************/
int goen_get_location_point_m9( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
				unsigned int *mask, unsigned long node_width, 
				goen_point_type *info_pointer, vldh_t_node node)
{
	int sts;
	char		annot_str[3][80];
	int		annot_nr[3];
	int		annot_count;
	
	if (( *mask <= 1) && ( *(mask + 1) <= 1) )
	  sts =  goen_get_location_point_m7( grectx, graphbody, mask, 
			node_width, info_pointer, node);
	else if ( *(mask + 1) <= 1)
	  sts =  goen_get_location_point_m15( grectx, graphbody, mask, 
			node_width, info_pointer, node);
	else
	{
	  sts = WGre::get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
	  if ( EVEN(sts)) return sts;

	  if ( annot_count > 1)
	    sts =  goen_get_location_point_m2( grectx, graphbody, mask, 
			node_width, info_pointer, node);
	  else
	    sts =  goen_get_location_point_m0( grectx, graphbody, mask, 
			node_width, info_pointer, node);
	}
	return sts;
}
