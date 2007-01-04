/* 
 * Proview   $Id: wb_goen.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_goen.c
   This module creates the gobe graphic context's used in
   gredit, the nodetypes and the connectiontypes.
   The module contains a number of methods to create the nodetypes
   and to fetch information about them.  */

#include <stdio.h>
#include <string.h>

#include "pwr.h"
#include "wb_ldh.h"
#include "flow_ctx.h"
#include "flow_api.h"

#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_goenm0.h"
#include "wb_goenm1.h"
#include "wb_goenm2.h"
#include "wb_goenm3.h"
#include "wb_goenm4.h"
#include "wb_goenm5.h"
#include "wb_goenm6.h"
#include "wb_goenm7.h"
#include "wb_goenm8.h"
#include "wb_goenm9.h"
#include "wb_goenm10.h"
#include "wb_goenm11.h"
#include "wb_goenm14.h"
#include "wb_goenm15.h"
#include "wb_goenm16.h"

typedef int (* goen_tCreateNodeTypeFunc)(pwr_sGraphPlcNode *, pwr_tCid, ldh_tSesContext,
					 flow_tCtx, unsigned long *, unsigned long,
					 unsigned long, flow_tNodeClass *, vldh_t_node);
typedef int (* goen_tGetParamFunc)(pwr_sGraphPlcNode *, pwr_tCid, ldh_tSesContext,
				   unsigned long, unsigned long *, unsigned long *,
				   unsigned long *, unsigned long *);
typedef int (* goen_tGetPointInfoFunc)(WGre *, pwr_sGraphPlcNode *, unsigned long,
				       unsigned long *, unsigned long, goen_conpoint_type *,
				       vldh_t_node);
typedef int (* goen_tGetLocationPointFunc)( WGre *, pwr_sGraphPlcNode *, unsigned long *,
					    unsigned long, goen_point_type *, vldh_t_node);

/*_Local variables_______________________________________________________*/

goen_tGetParamFunc goen_get_parameter_m[20] = {
	goen_get_parameter_m0,
	goen_get_parameter_m1,
	goen_get_parameter_m2,
	goen_get_parameter_m3,
	goen_get_parameter_m4,
	goen_get_parameter_m5,
	goen_get_parameter_m6,
	goen_get_parameter_m7,
	goen_get_parameter_m8,
	goen_get_parameter_m9,
	goen_get_parameter_m10,
	goen_get_parameter_m11,
	goen_get_parameter_m0,
	goen_get_parameter_m0,
	goen_get_parameter_m14,
	goen_get_parameter_m15,
	goen_get_parameter_m16,
	goen_get_parameter_m0,
	};
goen_tCreateNodeTypeFunc goen_create_nodetype_m[20] = {
	goen_create_nodetype_m0,
	goen_create_nodetype_m1,
	goen_create_nodetype_m2,
	goen_create_nodetype_m3,
	goen_create_nodetype_m4,
	goen_create_nodetype_m5,
	goen_create_nodetype_m6,
	goen_create_nodetype_m7,
	goen_create_nodetype_m8,
	goen_create_nodetype_m9,
	goen_create_nodetype_m10,
	goen_create_nodetype_m11,
	goen_create_nodetype_m0,
	goen_create_nodetype_m0,
	goen_create_nodetype_m14,
	goen_create_nodetype_m15,
	goen_create_nodetype_m16,
	goen_create_nodetype_m0,
	};
goen_tGetPointInfoFunc goen_get_point_info_m[20] = {
	goen_get_point_info_m0,
	goen_get_point_info_m1,
	goen_get_point_info_m2,
	goen_get_point_info_m3,
	goen_get_point_info_m4,
	goen_get_point_info_m5,
	goen_get_point_info_m6,
	goen_get_point_info_m7,
	goen_get_point_info_m8,
	goen_get_point_info_m9,
	goen_get_point_info_m10,
	goen_get_point_info_m11,
	goen_get_point_info_m0,
	goen_get_point_info_m0,
	goen_get_point_info_m14,
	goen_get_point_info_m15,
	goen_get_point_info_m16,
	goen_get_point_info_m0,
	};
goen_tGetLocationPointFunc goen_get_location_point_m[20] = {
	goen_get_location_point_m0,
	goen_get_location_point_m1,
	goen_get_location_point_m2,
	goen_get_location_point_m3,
	goen_get_location_point_m4,
	goen_get_location_point_m5,
	goen_get_location_point_m6,
	goen_get_location_point_m7,
	goen_get_location_point_m8,
	goen_get_location_point_m9,
	goen_get_location_point_m10,
	goen_get_location_point_m11,
	goen_get_location_point_m0,
	goen_get_location_point_m0,
	goen_get_location_point_m14,
	goen_get_location_point_m15,
	goen_get_location_point_m16,
	goen_get_location_point_m0,
	};


/*_Methods defined for this module_______________________________________*/


/*************************************************************************
 *
 * Name:		goen_create_contype()
 *
 * Type		int
 *
 * Type		Parameter	IOGF	Description
 * Widget	widget		I	neted widget.
 * unsigned long	conclass	I	connection class.
 * unsigned long	ldhses		I	ldh session.
 * goe_t_colors	*colors		I	array of loaded colors.
 * goe_t_fonts	*fonts		I	array of loaded fonts.
 * goen_t_cursors *cursors	I	array of loaded cursors.
 * unsigned long  *con_type_id	O	created neted connections type id.
 *
 * Description:
 *	Create a neted connection type.
 *	The attributes of the connection is described in grapbody in the 
 *	class object.
 *
 *	Parameters:
 *
 *    unsigned long 	con_type	index for where the connection type
 *					id is stored in the window object.
 *    int		arrows		no, one or two arrows.
 *    float		linewidth	widht of the connection line.
 *    float		dashes		length of dashes.
 *    int 		fillpattern	fillpattern for line.
 *    int		color		color.
 *    int		curvature	drawing method for the connection.
 *    float		corners		radius of corners.
 *
 *	Values:
 *
 *    arrows:
 *	GOEN_ARROW_NONE	0		No arrow
 *	GOEN_ARROW_END	1		One arrow at end of connection
 *	GOEN_ARROW_BOTH	2		Two arrows
 *    fillpattern:
 *	GOEN_UNFILL	0		No fillpattern	
 *	GOEN_FILL1	1		Some fillpattern...
 *    color:
 *	GOE_K_BLACK	0		black
 *	GOE_K_BLUE      1		blue
 *	GOE_K_RED	2		red
 *	GOE_K_GREEN     3		greem
 *    curvature:
 *	GOEN_ROUTECON	0		Routed connection
 *	GOEN_JAGGED	1		Streight line connection
 *	GOEN_RECTILINEAR 2		Rectilinear connectionstyle
 *	GOEN_STRANSDIV 	3		Simultaneus transition divergence
 *	GOEN_STRANSCONV 4		Simultaneus transition convergence
 *	GOEN_STEPDIV 	5		Step divergence
 *	GOEN_STEPCONV 	6		Step convergence
 *
 **************************************************************************/
int goen_create_contype(
			flow_tCtx	        ctx,
			pwr_tClassId	conclass,
			ldh_tSesContext	ldhses,
			flow_tConClass      *con_class
			)
{
  pwr_tClassId			bodyclass;
  pwr_sGraphPlcConnection 	*graphbody;
  int				sts, size;
  char				name[80];
  flow_eDrawType			line_type;
  flow_eConType			con_type;
  int				line_width;
  double				arrow_width = 0;
  double				arrow_length = 0;
  double				round_corner_amount;
  flow_eCorner			corner_type;

  if ( conclass == 0) {
    /* Fix for syref connections */
    flow_CreateConClass(ctx, "RefCon", 
			flow_eConType_Reference, flow_eCorner_Right,
			flow_eDrawType_Line, 2, 0, 0, 0, flow_eConGroup_Common,
			con_class);
    return 1;
  }
  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, conclass, "GraphPlcCon", 
			 &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts) ) return sts;

  sts = ldh_ClassIdToName( ldhses, conclass, name, sizeof(name), &size);
  if( EVEN(sts) ) return sts;

  if (graphbody->arrows != 0) {
    con_type = flow_eConType_StraightOneArrow;
    arrow_width = 0.03;
    arrow_length = 0.045;
  }
  else if ( graphbody->curvature == GOEN_ROUTECON ||
	    graphbody->curvature == GOEN_RECTILINEAR)
    con_type = flow_eConType_Routed;
  else if ( graphbody->curvature == GOEN_STRANSDIV)
    con_type = flow_eConType_TransDiv;
  else if ( graphbody->curvature == GOEN_STRANSCONV)
    con_type = flow_eConType_TransConv;
  else if ( graphbody->curvature == GOEN_STEPDIV)
    con_type = flow_eConType_StepDiv;
  else if ( graphbody->curvature == GOEN_STEPCONV)
    con_type = flow_eConType_StepConv;
  else if ( graphbody->curvature == GOEN_JAGGED)
    con_type = flow_eConType_Straight;
  else
    con_type = flow_eConType_Straight;

  line_width = (int)(graphbody->linewidth * 400 + 0.5);
  if ( graphbody->dashes > 0)
    line_type = flow_eDrawType_LineDashed;
  else if ( graphbody->fillpattern > 0)
    line_type = flow_eDrawType_LineGray;
  else
    line_type = flow_eDrawType_Line;
	
  if ( graphbody->corners > 0.0) {
    corner_type = flow_eCorner_Rounded;
    round_corner_amount = 0.025;
  }
  else {
    corner_type = flow_eCorner_Right;
    round_corner_amount = 0;
  }

  flow_CreateConClass(ctx, name,
		      con_type, corner_type, 
		      line_type, line_width, arrow_width, arrow_length, 
		      round_corner_amount, flow_eConGroup_Common,
		      con_class);
  return GOEN__SUCCESS;
}

//
//	Create a nodetype of the specified objecttype, mask and graphical
//	method.
//	The routine calls the graphical method for creating a nodetype
//	of this class.
//
int goen_create_nodetype( 
			 flow_tCtx	         ctx,
			 pwr_tClassId	 cid,
			 ldh_tSesContext	 ldhses,
			 unsigned int 	 *mask,
			 unsigned long	 subwindowmark,
			 unsigned long	 node_width,
			 flow_tNode           *node_class,
			 vldh_t_node		 node
			 )
{
  int			sts, size, graphmethod;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;

  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, cid, "GraphPlcNode", 
			 &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts) ) return sts;

  graphmethod = graphbody->graphmethod;
  if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

  sts = (goen_create_nodetype_m[graphmethod]) ( graphbody, cid, ldhses, ctx, 
						(unsigned long *)mask, subwindowmark, 
						node_width, node_class, node);

  return sts;
}

//
//  Returns geometrical info of a connectionpoint and info of the 
//  corresponding parameter.
//
int	goen_get_parinfo( 
			 WGre			*grectx,
			 pwr_tClassId		cid,
			 ldh_tSesContext		ldhses,
			 unsigned int		*mask,
			 unsigned long		node_width,
			 unsigned long		con_point,
			 goen_conpoint_type		*graph_pointer,
			 unsigned long		*par_type,
			 unsigned long		*par_inverted,
			 unsigned long		*par_index,
			 vldh_t_node			 node
			 )
{
  int			sts, size, graphmethod;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;

  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, cid, "GraphPlcNode", 
			 &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts) ) return sts;

  graphmethod = graphbody->graphmethod;
  if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

  sts = (goen_get_point_info_m[graphmethod]) 
    ( grectx, graphbody, con_point, (unsigned long *)mask, node_width, 
      graph_pointer, node);
  if ( EVEN(sts) ) return ( sts);

  sts = (goen_get_parameter_m[graphmethod]) 
    ( graphbody, cid, ldhses, con_point, (unsigned long *)mask, 
      par_type, par_inverted,	par_index);
  return sts;	
}
//
//	Gets returns geometrical info of a connectionpoint.
//
int	goen_get_pointinfo( 
			   WGre		 	*grectx,
			   pwr_tClassId		cid,
			   ldh_tSesContext		ldhses,
			   unsigned int		*mask,
			   unsigned long		node_width,
			   unsigned long		con_point,
			   goen_conpoint_type	*graph_pointer,
			   vldh_t_node		 node
			   )
{
  int			sts, size, graphmethod;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;

  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, cid, "GraphPlcNode", 
			 &bodyclass, (char **)&graphbody, &size);
  if ( EVEN(sts)) return sts;
  graphmethod = graphbody->graphmethod;
  if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

  sts = (goen_get_point_info_m[graphmethod]) 
    ( grectx, graphbody, con_point, (unsigned long *)mask, node_width, 
      graph_pointer, node);
  return sts;
}

//
//  Gets info of a corresponding parameter to a connectionpoint.
//
int	goen_get_parameter( 
			   pwr_tClassId		cid,
			   ldh_tSesContext		ldhses,
			   unsigned int		*mask,
			   unsigned long		con_point,
			   unsigned long		*par_type,
			   unsigned long		*par_inverted,
			   unsigned long		*par_index
			   )
{
  int			sts, size, graphmethod;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;

  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, cid, "GraphPlcNode", 
			 &bodyclass, (char **)&graphbody, &size);
  if ( EVEN(sts)) return sts;

  graphmethod = graphbody->graphmethod;
  if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

  sts = ( goen_get_parameter_m[graphmethod])
    ( graphbody, cid, ldhses, con_point, (unsigned long *)mask, 
      par_type, par_inverted, par_index);
  return sts;
}

//
//	Returns location point of a node.
//	The location point is the point in the node that should be positioned
//	on a gridpoint in the neted window.
//	The returned koordinates are relative the lower left corner of the node.
//
int	goen_get_location_point( 
				WGre		 	*grectx,
				pwr_tClassId		cid,
				ldh_tSesContext		ldhses,
				unsigned int		*mask,
				unsigned long		node_width,
				goen_point_type		*location_point,
				vldh_t_node			node
				)
{
  int			sts, size, graphmethod;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;

  /* Get graphbody for the class */
  sts = ldh_GetClassBody(ldhses, cid, "GraphPlcNode", 
			 &bodyclass, (char **)&graphbody, &size);
  if ( EVEN(sts)) return sts;
  graphmethod = graphbody->graphmethod;
  if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

  sts = (goen_get_location_point_m[graphbody->graphmethod]) 
    ( grectx, graphbody, (unsigned long *)mask, node_width, location_point, node);
  return sts;
}




