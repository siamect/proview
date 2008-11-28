/* 
 * Proview   $Id: wb_goenm5.cpp,v 1.2 2008-11-28 17:14:04 claes Exp $
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

/* wb_goenm5.c 

   This module creates a nodetype from the objecttype specification
   and the instanceobject's mask.

		!
		!
	---------------------------------------------------------
	!		!				!	!
  ------!   'ordertype'	!  'instance name'		!	!-------	
	!		!				!	!
	---------------------------------------------------------

   */

#include <stdio.h>

#include "pwr.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_goenm5.h"

/*_Local variables_______________________________________________________*/


	/* 	 name        value    name in drawing */
static	float	f_defwidth   = GOEN_F_MINWIDTH * GOEN_F_OBJNAME_STRWIDTH;
static	float	f_pinlength  = GOEN_F_PINLENGTH;
static	float	f_repeat     = GOEN_F_GRID;
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;
static	float	f_yoffs      = GOEN_F_GRID/2;
static	float	f_height;
static	float	f_width;
static	float   f_node_width;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m5()
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
*	Create a nodetype of ordertype
*
*		!
*		!
*	---------------------------------------------------------
*	!		!				!	!
* ------!   'orderspec'	!  'instance name'		!	!-------	
*	!		!				!	!
*	---------------------------------------------------------
*
*
**************************************************************************/


int goen_create_nodetype_m5( 
    pwr_sGraphPlcNode	*graphbody,
    pwr_tClassId	cid,
    ldh_tSesContext	ldhses,
    flow_tCtx		ctx,
    unsigned long 	*mask,
    unsigned long	subwindowmark,
    unsigned long	node_width,
    flow_tNodeClass	*node_class,
    vldh_t_node		node)
{
	int		inputpoints, outputpoints;
	unsigned long    pointmask;
	unsigned long    *inmask_pointer;
	unsigned long    *outmask_pointer;
	int 		inputs;
	int 		interns;
	int		ouputs;
	int		sts;
	char		annot_str[3][80];
	int		annot_nr[3];
	int		annot_count;
	double		annot_width[3];
	double		annot_height;
	float		f_ordertypewidth   = 0.04;
	float		f_backcheckwidth   = 0.01;
  	flow_tNodeClass	nc;
  	char		name[80];
  	int		size;
  	int		conpoint_num;
	int		rows;
	static int	idx = 0;


  	sts = ldh_ClassIdToName(ldhses, cid, name, sizeof(name), &size);
  	if ( EVEN(sts) ) return sts;
	sprintf( &name[strlen(name)], "%d", idx++);


	/* Get number of annotations and the width of the annotations */
	sts = WGre::get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
	if ( EVEN(sts)) return sts;

	if ( annot_count > 0)
  	{
    	  flow_MeasureAnnotText( ctx, annot_str[0],
	     	flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[0], &annot_height, &rows);
  	}
	if ( annot_count > 1)
  	{
    	  flow_MeasureAnnotText( ctx, annot_str[1],
	     	flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[1], &annot_height, &rows);
  	}
	if ( annot_count > 2)
  	{
    	  flow_MeasureAnnotText( ctx, annot_str[2],
	     	flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[2], &annot_height, &rows);
  	}
	
	/* Get how many parameters there are */
	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	ouputs = graphbody->parameters[PAR_OUTPUT];

	inmask_pointer = mask++;
	outmask_pointer = mask;

	/* Check if condition input in mask (second bit) */
	pointmask = 2;
	inputpoints = 1;
	inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	pointmask = 1;
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);

	f_height  = f_repeat;
	f_ordertypewidth =
	        co_max( f_ordertypewidth, annot_width[1] + f_strlength);
	f_backcheckwidth =
		co_max( f_backcheckwidth, annot_width[2] + 2 * f_strlength);
	f_width = f_ordertypewidth +
		  co_max( f_defwidth, annot_width[0] + 2 * f_strlength) +
		  f_backcheckwidth;

	flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc);

	/* Draw the rectangle for gate	*/
	flow_AddRect( nc, 0, -f_yoffs, f_width, f_height, flow_eDrawType_Line, 2, flow_mDisplayLevel_1);

	if ( (subwindowmark & 2) == 0 )
	{
          flow_AddLine(nc, f_ordertypewidth, -f_yoffs, 
		f_ordertypewidth, f_height - f_yoffs, flow_eDrawType_Line, 1);
	}
	else
	{
          flow_AddLine(nc, f_ordertypewidth, -f_yoffs, 
		f_ordertypewidth, f_height - f_yoffs, flow_eDrawType_LineGray, 5);
	}
	if ( (subwindowmark & 1) == 0 )
	{
          flow_AddLine( nc, f_width - f_backcheckwidth,  -f_yoffs, 
		f_width - f_backcheckwidth, f_height - f_yoffs, flow_eDrawType_Line, 1);
	}
	else
	{
          flow_AddLine( nc, f_width - f_backcheckwidth,  -f_yoffs, 
		f_width - f_backcheckwidth, f_height - f_yoffs, flow_eDrawType_LineGray, 5);
	}



	conpoint_num = 0;
        flow_AddLine( nc, -f_pinlength, f_height / 2 - f_yoffs, 
		0, f_height / 2 - f_yoffs, flow_eDrawType_Line, 2);
        flow_AddConPoint(nc, -f_pinlength, f_height /2 - f_yoffs, 
		conpoint_num++, flow_eDirection_Left);

	if ( inputpoints == 2 )
	{
	  /* Draw condition pin */
          flow_AddLine( nc, f_repeat, - f_pinlength - f_yoffs, 
		f_repeat,  -f_yoffs, flow_eDrawType_Line, 2);
          flow_AddConPoint(nc, f_repeat, - f_pinlength - f_yoffs, 
		conpoint_num++, flow_eDirection_Down);
	}	

	if (outputpoints != 0)
	{
          flow_AddLine( nc, f_width, f_height / 2 - f_yoffs, 
		f_width + f_pinlength, f_height /2 - f_yoffs, flow_eDrawType_Line, 2);
          flow_AddConPoint( nc, f_width + f_pinlength, f_height /2 - f_yoffs, 
		conpoint_num++, flow_eDirection_Right);
	}

	f_node_width = node_width;
        flow_AddAnnot( nc, 
		f_ordertypewidth + f_strlength,
		f_height - f_strheight / 2 - f_yoffs,
		0, flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
        flow_AddAnnot( nc, 
		f_strlength / 2,
		f_height - f_strheight / 2 - f_yoffs,
		1, flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
        flow_AddAnnot( nc, 
		f_width - f_backcheckwidth + f_strlength,
		f_height - f_strheight / 2 - f_yoffs,
		2, flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);

  	/* Add execute order display */
  	flow_AddFilledRect( nc, f_width - GOEN_DISPLAYNODEWIDTH, -f_yoffs, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
  	flow_AddAnnot( nc, 
	  f_width - GOEN_DISPLAYNODEWIDTH + f_strlength,
	  (GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_yoffs,
	  GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelvetica, GOEN_F_TEXTSIZE, 
	  flow_eAnnotType_OneLine, flow_mDisplayLevel_2);

	*node_class = nc;
	return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m5()
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
int goen_get_point_info_m5( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
			    unsigned long point, unsigned long *mask, 
			    unsigned long node_width, goen_conpoint_type *info_pointer, 
			    vldh_t_node node)
{
    int	inputpoints, outputpoints;
    unsigned long    pointmask;
	unsigned long    *inmask_pointer;
	unsigned long    *outmask_pointer;
	int 		inputs;
	int 		interns;
	int		outputs;
	double		ll_x,ll_y,ur_x,ur_y;

	/* Get number of parameters */
	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	outputs = graphbody->parameters[PAR_OUTPUT];

	inmask_pointer = mask++;
	outmask_pointer = mask;

	/* Check if condition input in mask (second bit) */
	pointmask = 2;
	inputpoints = 1;
	inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	pointmask = 1;
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);
	
	flow_MeasureNode( 
			  node->hn.node_id,
			  &ll_x,&ll_y,&ur_x,&ur_y);

	f_width = ur_x - ll_x - 2* GOEN_F_LINEWIDTH;
	f_height = ur_y - ll_y - 2* GOEN_F_LINEWIDTH;
	if ( inputpoints == 2)
	  f_height -= f_pinlength;
	f_width -= f_pinlength;
	if ( outputpoints > 0)
	  f_width -= f_pinlength;


	if ( point == 0)
	{
	   info_pointer->x =  - f_width/2.0
		-f_pinlength *( 1 - ( outputpoints == 0 ) * 0.5);
	   info_pointer->y = -f_pinlength *( ( inputpoints == 2 ) * 0.5);
	   info_pointer->type = CON_LEFT;
	}
	if ( (point == 1) && (inputpoints == 2 )) 
	{
	  info_pointer->x =  - f_width/2.0 + f_repeat
		+ f_pinlength * ( outputpoints == 0 ) * 0.5;
	  info_pointer->y = f_height / 2 + 
		f_pinlength *( 1 -  ( inputpoints == 2 ) * 0.5);
	  info_pointer->type = CON_UP;
	}
	if ( ((point == 2) && (inputpoints == 2)) ||
		((point == 1) && (inputpoints == 1)))
	{
	  info_pointer->x =  f_width/2.0 + f_pinlength;
	  info_pointer->y = -f_pinlength *( ( inputpoints == 2 ) * 0.5);
	  info_pointer->type = CON_RIGHT;
	}
	return GOEN__SUCCESS;    
}



/*************************************************************************
*
* Name:		goen_get_parameter_m5()
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
int	goen_get_parameter_m5( pwr_sGraphPlcNode *graphbody, pwr_tClassId cid, 
			       ldh_tSesContext ldhses, unsigned long con_point, 
			       unsigned long *mask, unsigned long *par_type, 
			       unsigned long *par_inverted, unsigned long *par_index)
{

	unsigned long	inputs,interns,outputs;
	unsigned long    pointmask;
	unsigned long    *inmask_pointer;
	unsigned long    *outmask_pointer;
	int		input_found, output_found;
	int		inputpoints, outputpoints;
	unsigned long	*invertmask_pointer;

	/* Get number of parameters */
	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	outputs = graphbody->parameters[PAR_OUTPUT];

	inmask_pointer = mask++;
	outmask_pointer = mask++;
	invertmask_pointer = mask;

	/* Check if condition input in mask (second bit) */
	pointmask = 2;
	inputpoints = 1;
	inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	pointmask = 1;
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);
	
        input_found = 0;
        output_found = 0;
	if ( con_point == 0)
	{
	    *par_type = PAR_INPUT;
	    *par_index = 0;
	    *par_inverted = GOEN_NOT_INVERTED;
	    input_found = 1;
	}
	if ( (con_point == 1) && (inputpoints == 2 )) 
	{
	    *par_type = PAR_INPUT;
	    *par_index = 1;
	    *par_inverted = GOEN_NOT_INVERTED;
	    input_found = 1;
	}
	if ( ((con_point == 2) && (inputpoints == 2)) ||
		((con_point == 1) && (inputpoints == 1)))
	{
	      *par_type = PAR_OUTPUT;
	      *par_index = inputs + interns;
	      *par_inverted = GOEN_NOT_INVERTED;
	      output_found = 1;
	}
	if ( input_found || output_found ) return GOEN__SUCCESS;
	else return GOEN__NOPOINT;
}


/*************************************************************************
*
* Name:		goen_get_location_point_m5()
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
int goen_get_location_point_m5( WGre *grectx, pwr_sGraphPlcNode *graphbody, 
				unsigned long *mask, unsigned long node_width, 
				goen_point_type *info_pointer, vldh_t_node node)
{
#if 0
    int  i;
    int	ipoints;
    int	inputpoints, outputpoints;
	unsigned long    pointmask;
	unsigned long    *inmask_pointer;
	unsigned long    *outmask_pointer;
	int 		inputs;
	int 		interns;
	int			ouputs;
	float		ll_x,ll_y,mid_x,mid_y;
	int		sts;

	/* Get number of parameters */
	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	ouputs = graphbody->parameters[PAR_OUTPUT];

	inmask_pointer = mask++;
	outmask_pointer = mask;

	/* Check if condition input in mask (second bit) */
	pointmask = 2;
	inputpoints = 1;
	inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	pointmask = 1;
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);
	
	sts = NetedMeasureNode( grectx->neted_window,
			  node->hn.node_id,
			  &ll_x,&ll_y,&mid_x,&mid_y,&f_width,&f_height);
	tst_neted( sts, "NetedMeasureNode", 204 );

	f_width -= 2* GOEN_F_LINEWIDTH;
	f_height -= 2* GOEN_F_LINEWIDTH;
	if ( inputpoints == 2)
	  f_height -= f_pinlength;
	f_width -= f_pinlength;
	if ( outputpoints > 0)
	  f_width -= f_pinlength;

	info_pointer->x = f_width/2 + f_pinlength / 2 * 
		(( inputpoints == 0 ) - ( outputpoints == 0 ));
	info_pointer->y = f_pinlength/2 * ( inputpoints == 2 );


#endif
	return GOEN__SUCCESS;
}
