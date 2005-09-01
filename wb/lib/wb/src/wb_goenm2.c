/** 
 * Proview   $Id: wb_goenm2.c,v 1.6 2005-09-01 14:57:58 claes Exp $
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

/* wb_goenm2.c 

   This module creates a nodetype from the objecttype specification
   and the instanceobject's mask. The same as method 0 but has
   two extra annotation above the objectinstance name.
									  
		!<-- g ->!						  
 	--------|========!==============|				  
  	! f	|       'objname'      	|  				  
 	--------|========!==============|   				  
 	     !e |	 !		|  				  
	     ---|'in1'   !              !				  
		|	 !		|				  
	     ---|'in2'	 !		|				  
		|	 !		|				  
             ---|'in3'	 !	   'sts'|---				  
		|	 !		|  !				  
	     ---|'in4'	 !		|  !				  
		|	 !		|  !				  
	     ---|'in5'	 !		|  ! c = height-f/2		  
	    d!	|!	 !		|  !				  
	     !	|!	 !		|  !				  
	    !---|!=======!==============|---				  
	    !	!!      'annot    '     !				  
	    !	!!      'annot    '     !				  
	    !	!!      'inst.name'     !				  
            !   !=======================!				  
	    ! a	!! h							  
	    !<->!!<-							  
   */									  

#include <stdio.h>
#include "wb_foe_macros.h"
#include <Xm/Xm.h>

#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"

/*_Local variables_______________________________________________________*/

	/* 	 name        value    name in drawing */
static	float	f_defwidth   = GOEN_F_MINWIDTH * GOEN_F_OBJNAME_STRWIDTH;
static	float	f_pinlength  = GOEN_F_PINLENGTH;
static	float	f_repeat     = GOEN_F_GRID;
static	float	f_nameoffin  = GOEN_F_OBJNAME_STRWIDTH / 2.;
static	float	f_nameoffout = GOEN_F_OBJNAME_STRWIDTH / 2.;
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;
static	float	f_circle     = GOEN_F_INVERTCIRCLE;
static	float	f_yoffs      = GOEN_F_GRID*3/2;
static	float	f_height;
static	float	f_width;
static	float	f_namepos;
static	float   f_namelength;
static	float   f_node_width;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m2()
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


int goen_create_nodetype_m2( 
    pwr_sGraphPlcNode	*graphbody,
    pwr_tClassId	class,
    ldh_tSesContext	ldhses,
    flow_tCtx		ctx,
    unsigned long 	*mask,
    unsigned long	subwindowmark,
    unsigned long	node_width,
    flow_tNodeClass	*node_class,
    vldh_t_node		node)
{
  int  		i, sts;
  int		ipoints;
  int		inputpoints, outputpoints;
  unsigned long	pointmask;
  unsigned long	*inmask_pointer;
  unsigned long	*outmask_pointer;
  unsigned long	*invertmask_pointer;
  int 		i_innr;
  int 		i_internnr;
  int		i_outnr;
  ldh_sParDef 	*bodydef;
  int 		rows;
  char		annot_str[3][200];
  int		annot_nr[3];
  int		annot_count;
  double	annot_width[3];
  double	annot_height;
  float		classname_width;
  flow_tNodeClass nc_pid;
  char		name[80];
  int		size;
  int		conpoint_nr;
  int		annot_rows;
  static int	idx = 0;
  flow_tObject	cp;


  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get the runtime paramteers for this class */
  sts = ldh_GetObjectBodyDef(ldhses, class, "RtBody", 1, 
			&bodydef, &rows);
  if ( EVEN(sts) ) return sts;

  /* Get number of annotations and the width of the annotations */
  sts = gre_get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
  if ( EVEN(sts)) return sts;

  annot_width[0] = 0;
  annot_width[1] = 0;
  annot_width[2] = 0;
  if ( annot_count > 0)
  {
    flow_MeasureAnnotText( ctx, annot_str[0],
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine,
		&annot_width[0], &annot_height, &annot_rows);
  }
  if ( annot_count > 1)
  {
    flow_MeasureAnnotText( ctx, annot_str[1],
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[1], &annot_height, &annot_rows);
  }
  if ( annot_count > 2)
  {
    flow_MeasureAnnotText( ctx, annot_str[2],
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[2], &annot_height, &annot_rows);
  }

  /* Get how many parameters there are */
  i_innr = graphbody->parameters[PAR_INPUT];
  i_internnr = graphbody->parameters[PAR_INTERN];
  i_outnr = graphbody->parameters[PAR_OUTPUT];

  inmask_pointer = mask++;
  outmask_pointer = mask++;
  invertmask_pointer = mask;
  /* Count number of inputpoints in mask  */
  pointmask = 1;
  inputpoints = 0;
  for ( i = 0; i < i_innr; i++)  
  {
    inputpoints += ((*inmask_pointer & pointmask) != 0);
    pointmask <<= 1;
  }
  /* Count number of outputpoints in mask  */
  pointmask = 1;
  outputpoints = 0;
  for ( i = 0; i < i_outnr; i++)  
  {
    outputpoints += ((*outmask_pointer & pointmask) != 0);
    pointmask <<= 1;
  }
	

  f_height  = (max(inputpoints,outputpoints) + 4)*f_repeat;
  classname_width = strlen( graphbody->graphname) * f_strlength;

  f_width = max( f_strlength * 4 + max( classname_width, annot_width[0]),
			 f_defwidth + f_strlength * 2);
  if ( annot_count >= 2)
    f_width = max( f_width, f_strlength * 4 + annot_width[1]);

  if ( annot_count >= 3)
    f_width = max( f_width, f_strlength * 4 + annot_width[2]);

  f_namepos = f_width/2.0 - classname_width/2;

  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc_pid);

  /* Draw the rectangle for gate		*/
  flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);

  /* Draw the separator line for header and footer			      */

  if ( subwindowmark == 0 )
  {
    flow_AddLine( nc_pid, 0, f_repeat - f_yoffs, f_width, 
		f_repeat - f_yoffs, flow_eDrawType_Line, 2);
  }
  else
  {
    flow_AddLine( nc_pid, 0, f_repeat - f_yoffs, f_width, 
		f_repeat - f_yoffs, flow_eDrawType_LineGray, 4);
  }


  /* Draw the objname */
  flow_AddText( nc_pid, graphbody->graphname, f_namepos, 
		f_repeat/2 + f_strheight/2 - f_yoffs, 
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);

  /* Draw the leadnames and lines */

  conpoint_nr = 0;
  pointmask = 1;
  ipoints = 0;
  for (i = 0; i <i_innr ;i++) 
  {
    if ( (*inmask_pointer & pointmask) != 0)
    {
      if ( bodydef[i].ParClass == pwr_eClass_Input) 
      {
        flow_AddText( nc_pid, bodydef[i].Par->Input.Graph.GraphName,
		f_nameoffin, 
		f_repeat * (1.5 + ipoints) + f_strheight/2 - f_yoffs, 
		flow_eDrawType_TextHelvetica, 2);
      }
      if ( (*invertmask_pointer & pointmask) == 0)
      {
        flow_AddLine( nc_pid, 0, f_repeat*(1.5 + ipoints) - f_yoffs, 
		-f_pinlength, f_repeat*(1.5+ipoints) - f_yoffs,
		flow_eDrawType_Line, 2);
      }
      else
      {
        flow_AddLine( nc_pid, -f_pinlength, f_repeat*(1.5 + ipoints) - f_yoffs, 
		-f_circle, f_repeat*(1.5+ipoints) - f_yoffs,
		flow_eDrawType_Line, 2);
        flow_AddArc( nc_pid, -f_circle, 
		f_repeat*(1.5+ipoints) - f_circle / 2 - f_yoffs, 
		0, f_repeat*(1.5+ipoints) + f_circle / 2 - f_yoffs, 0, 360, 
		flow_eDrawType_Line, 2);
      }
      flow_AddConPoint( nc_pid, -f_pinlength, 
		f_repeat * ( 1.5 + ipoints) - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Left);
      ipoints++;
    }
    pointmask <<= 1;
  }
  pointmask = 1;
  ipoints = 0;
  for (i = i_innr + i_internnr; i < i_innr + i_internnr + i_outnr ; i++)
  {
    if ( (*outmask_pointer & pointmask) != 0)
    {

      f_namelength = strlen( bodydef[i].Par->Output.Graph.GraphName) *
		   f_strlength;

      if ( bodydef[i].ParClass == pwr_eClass_Output) 
      {
        flow_AddText( nc_pid, bodydef[i].Par->Output.Graph.GraphName, 
		f_width-f_nameoffout-f_namelength, 
		f_repeat * (1.5 + ipoints) + f_strheight/2 - f_yoffs,
		flow_eDrawType_TextHelvetica, 2);

      }
      flow_AddLine( nc_pid, f_width, f_repeat*(1.5 + ipoints) - f_yoffs, 
		f_width+f_pinlength, f_repeat*(1.5+ipoints) - f_yoffs,
		flow_eDrawType_Line, 2);
      flow_CreateConPoint( ctx, f_width + f_pinlength, 
		f_repeat * ( ipoints + 1.5) - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Right, &cp);
      flow_NodeClassAdd( nc_pid, cp);
      if (bodydef[i].Par->Output.Info.Type == pwr_eType_Float32)
        flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Float32);
      else if (bodydef[i].Par->Output.Info.Type == pwr_eType_Int32)
        flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Int32);
      else if (bodydef[i].Par->Output.Info.Type == pwr_eType_Boolean)
        flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Boolean);
      ipoints++;
    }
    pointmask <<= 1;
  }

  f_namelength = f_strlength*6;
  f_node_width = node_width;
  if ( node_width == 0 )
    f_node_width = 6;
  if ( graphbody->graphmethod != 9)
  {
    flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[1]/2,
	f_height - 2*f_repeat - (f_repeat - f_strheight)/2.0 - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
    flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[2]/2,
	f_height - f_repeat - (f_repeat - f_strheight)/2.0 - f_yoffs,
	1, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
    flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[0]/2,
	f_height - (f_repeat - f_strheight)/2.0 - f_yoffs,
	2, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
  }
  else
  {
    /* Make it compatible with the annotations i method 9 */
    flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[0]/2,
	f_height - 2*f_repeat - (f_repeat - f_strheight)/2.0 - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
    if ( annot_count >= 1)
      flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[1]/2,
	f_height - f_repeat - (f_repeat - f_strheight)/2.0 - f_yoffs,
	1, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
    if ( annot_count >= 2)
      flow_AddAnnot( nc_pid, 
	f_width/2 - annot_width[2]/2,
	f_height - (f_repeat - f_strheight)/2.0 - f_yoffs,
	2, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
  }

  free( (char *)bodydef);
  *node_class = nc_pid;
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m2()
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
int goen_get_point_info_m2( grectx, graphbody, point, mask, node_width,
			info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long	 point;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_conpoint_type	 *info_pointer;
    vldh_t_node		 node;
{
    int			i;
    int			inputpoints, outputpoints;
    unsigned long    	pointmask;
    unsigned long    	*mask_pointer;
    int 		i_innr;
    int 		i_internnr;
    int			i_outnr;
    double		ll_x,ll_y,ur_x,ur_y;


	/* Get number of parameters */
	i_innr = graphbody->parameters[PAR_INPUT];
	i_internnr = graphbody->parameters[PAR_INTERN];
	i_outnr = graphbody->parameters[PAR_OUTPUT];

	/* Count number of inputpoints in mask  */
	mask_pointer = mask;
	pointmask = 1;
	inputpoints = 0;
	for ( i = 0; i < i_innr; i++)  
	{
	  inputpoints += ((*mask_pointer & pointmask) != 0);
	  pointmask <<= 1;
	}
	/* Count number of outputpoints in mask  */
	pointmask = 1;
	outputpoints = 0;
	mask_pointer++;
	for ( i = 0; i < i_outnr; i++)  
	{
	  outputpoints += ((*mask_pointer & pointmask) != 0);
	  pointmask <<= 1;
	}
	
        flow_MeasureNode( node->hn.node_id, &ll_x, &ll_y, &ur_x, &ur_y);
        f_width = ur_x - ll_x;
 	f_height = ur_y - ll_y;

	f_width -= 2* GOEN_F_LINEWIDTH;
	f_height -= 2* GOEN_F_LINEWIDTH;
	if ( inputpoints > 0)
	  f_width -= f_pinlength;
	if ( outputpoints > 0)
	  f_width -= f_pinlength;

	if ( point < inputpoints )
	{
	   /* Connectionpoint is an input */
	   info_pointer->x =  - f_width/2.0
		-f_pinlength *( 1 - ( outputpoints == 0 ) * 0.5);
	   info_pointer->y =  f_height/2.0 - (point + 1.5) * f_repeat;
	   info_pointer->type = CON_LEFT;
	}
	else
	{
	   /* Connectionpoint is an output */
	   info_pointer->x =  f_width/2.0 + f_pinlength
		*( 1 - ( inputpoints == 0 ) * 0.5);
	   info_pointer->y =  f_height/2.0 - 
		(point + 1.5 - inputpoints) * f_repeat;
	   info_pointer->type = CON_RIGHT;
	}
	return GOEN__SUCCESS;
}


/*************************************************************************
*
* Name:		goen_get_parameter_m2()
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
int	goen_get_parameter_m2( graphbody, class, ldhses,
			con_point, mask, par_type, par_inverted, par_index)
pwr_sGraphPlcNode	*graphbody;
pwr_tClassId		class;
ldh_tSesContext		ldhses;
unsigned long		con_point;
unsigned long		*mask;
unsigned long		*par_type;
unsigned long		*par_inverted;
unsigned long		*par_index;
{

	unsigned long	inputs,interns,outputs,conpointcount;
	unsigned long	pointmask;
	int		i, input_found, output_found;
	unsigned long	*invertmask_pointer;

	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	outputs = graphbody->parameters[PAR_OUTPUT];

	/* Identify the conpoints corresponding input or output */
	invertmask_pointer = mask + 2;
	*par_type = 0;
	conpointcount = 0;
	pointmask = 1;
        input_found = 0;
        output_found = 0;
	for ( i = 0; i < inputs; i++)  
	{
	  conpointcount += ((*mask & pointmask) != 0);
	  if ( conpointcount == (con_point + 1) )
	  {
	    /* this is the input */
	    *par_type = PAR_INPUT;
	    *par_index = i;
	    if ( (*invertmask_pointer & pointmask) == 0)
	      *par_inverted = GOEN_NOT_INVERTED;
	    else
	      *par_inverted = GOEN_INVERTED;
	    input_found = 1;
	    break;
	  }
	  pointmask <<= 1;
	}
	if ( !input_found )
	{
	  /* continue with outputs */
	  mask++;
	  pointmask = 1;
	  for ( i = inputs + interns; i < inputs + interns + outputs; i++)  
	  {
	    conpointcount += ((*mask & pointmask) != 0);
	    if ( conpointcount == (con_point + 1) )
	    {
	      /* this is the output */
	      *par_type = PAR_OUTPUT;
	      *par_index = i;
	      *par_inverted = GOEN_NOT_INVERTED;
	      output_found = 1;
	      break;
	    }
	    pointmask <<= 1;
	  }
	}
	if ( input_found || output_found ) return GOEN__SUCCESS;
	else return GOEN__NOPOINT;
}



/*************************************************************************
*
* Name:		goen_get_location_point_m2()
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
int goen_get_location_point_m2( grectx, graphbody, mask, node_width, 
		info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_point_type	 *info_pointer;
    vldh_t_node		 node;
{
   info_pointer->y = 0;
   info_pointer->x = 0;
   return GOEN__SUCCESS;
}

