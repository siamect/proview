/* wb_goenm1.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates a nodetype from the objecttype specification
   and the instanceobject's mask.
									  
		!<-- g ->!						  
 	--------|========!==============|				  
  	! f	|       'objname'      	|  				  
 	     !e |	 !		|  				  
	     ---|        !              !				  
		|	 !		|				  
	     ---|  	 !		|				  
		|	 !		|				  
             ---|	 !	        |---				  
		|	 !		|  !				  
	     ---|	 !		|  !				  
		|	 !		|  !				  
	     ---|	 !		|  ! c = height-f/2		  
	    d!	|!	 !		|  !				  
	     !	|!	 !		|  !				  
	    !---|!=======!==============|---				  
	    !	!!      'inst.name'     !				  
            !   !=======================!				  
	    ! a	!! h							  
	    !<->!!<-  */

#include <stdio.h>

#include "wb_foe_macros.h"
#include <Xm/Xm.h>

#include "wb_ldh.h"
#include "wb_vldh.h"
#include "wb_foe_msg.h"
#include "wb_goen.h"
#include "wb_gre.h"

/*_Local variables_______________________________________________________*/

	/* 	 name        value    name in drawing */
static	float	f_defwidth   = (GOEN_F_MINWIDTH - 3)* GOEN_F_OBJNAME_STRWIDTH;
static	float	f_header     = 0.05; 	/*	( f )	*/
static	float	f_pinlength  = GOEN_F_PINLENGTH;
static	float	f_repeat     = GOEN_F_GRID;
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;
static	float	f_circle     = GOEN_F_INVERTCIRCLE;
static	float	f_yoffs     = GOEN_F_GRID/2;
static	float	f_height;
static	float	f_width;
static	float	f_namepos;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m1()
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


int goen_create_nodetype_m1( 
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
  int  i;
  int	ipoints;
  int	inputpoints, outputpoints;
  unsigned long    pointmask;
  unsigned long    *inmask_pointer;
  unsigned long    *outmask_pointer;
  unsigned long    *invertmask_pointer;
  int 		i_innr;
  int 		i_internnr;
  int			i_outnr;
  int			sts;
  char		annot_str[3][80];
  int		annot_nr[3];
  int		annot_count;
  double	annot_width[3];
  double	annot_height;
  float		f_width_left;
  float		classname_width;
  flow_tNodeClass	nc_pid;
  char		name[80];
  int		size;
  int		conpoint_nr;
  int		rows;
  static int	idx = 0;
  ldh_sParDef 	*bodydef;
  flow_tObject	cp;

  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get number of annotations and the width of the annotations */
  sts = gre_get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
  if ( EVEN(sts)) return sts;

  if ( annot_count > 0)
  {
    flow_MeasureAnnotText( ctx, annot_str[0],
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[0], &annot_height, &rows);
  }
  if ( annot_count > 1)
  {
    flow_MeasureAnnotText( ctx, annot_str[1],
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, flow_eAnnotType_OneLine,
		&annot_width[1], &annot_height, &rows);
  }

  /* Get the runtime paramteers for this class */
  sts = ldh_GetObjectBodyDef(ldhses, class, "RtBody", 1, 
			&bodydef, &rows);
  if ( EVEN(sts) ) return sts;

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
	
  f_height  = f_header/2 + f_header/2 +
	       MAX((MAX(inputpoints,outputpoints)-1), 1) * f_repeat;

  classname_width = strlen( graphbody->graphname) * f_strlength;
  if ( annot_count <= 1)
  {
    f_width = MAX( f_strlength * 2 + MAX( classname_width, annot_width[0]),
			f_defwidth + f_strlength * 2);
    f_width_left = 0;
  }
  else
  {
    f_width = MAX( f_strlength * 4 + MAX( classname_width, annot_width[0])
			 + annot_width[1], f_defwidth + f_strlength * 2);
    f_width_left = f_strlength * 2 + annot_width[1];
  }

  f_namepos = f_width/2.0 - classname_width/2 - f_width_left/2;

  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc_pid);

  /* Draw the rectangle for gate		*/
  flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, flow_eDrawType_Line, 2, flow_mDisplayLevel_1);

  /* Draw the separator line for header and footer			      */

  if ( subwindowmark != 0 )
  {
    flow_AddLine( nc_pid, 0, f_header - f_yoffs, f_width, 
		f_header - f_yoffs, flow_eDrawType_LineGray, 4);
  }

  if ( annot_count >= 2)
  {
    flow_AddLine( nc_pid, f_width - f_width_left, -f_yoffs, f_width - f_width_left,
		f_height - f_yoffs, flow_eDrawType_Line, 1);
  }


  /* Draw the class name */
  flow_AddText( nc_pid, graphbody->graphname, f_namepos,
		f_header * .75 - f_yoffs,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);

  /* Draw the leadnames and lines					      */

  conpoint_nr = 0;
  pointmask = 1;
  ipoints = 0;
  for (i = 0; i <i_innr ;i++) 
  {
    if ( (*inmask_pointer & pointmask) != 0)
    {
      if ( (*invertmask_pointer & pointmask) == 0)
      {
        flow_AddLine( nc_pid, 0, f_header/2 + f_repeat * ipoints - f_yoffs, 
		-f_pinlength, f_header/2 + f_repeat * ipoints - f_yoffs,
		flow_eDrawType_Line, 2);
      }
      else
      {
        flow_AddLine( nc_pid, -f_pinlength, f_header/2 + f_repeat * ipoints - f_yoffs, 
		-f_circle, f_header/2 + f_repeat * ipoints - f_yoffs,
		flow_eDrawType_Line, 2);
        flow_AddArc( nc_pid, -f_circle, 
		f_header/2 + f_repeat * ipoints - f_circle / 2 - f_yoffs, 
		0, f_header/2 + f_repeat * ipoints + f_circle / 2 - f_yoffs, 
		0, 360, flow_eDrawType_Line, 2);
      }
      flow_AddConPoint( nc_pid, -f_pinlength, 
		f_header/2 + f_repeat * ipoints - f_yoffs, 
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
      flow_AddLine( nc_pid, f_width, f_header/2 + f_repeat * ipoints - f_yoffs, 
		f_width+f_pinlength, f_header/2 + f_repeat * ipoints - f_yoffs,
		flow_eDrawType_Line, 2);
      flow_CreateConPoint( ctx, f_width + f_pinlength, 
		f_header/2 + f_repeat * ipoints - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Right, &cp);
      flow_NodeClassAdd( nc_pid, cp);
      if (bodydef[i].Par->Output.Info.Type == pwr_eType_Float32)
        flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Float32);
      else if (bodydef[i].Par->Output.Info.Type == pwr_eType_Boolean)
        flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Boolean);
      ipoints++;
    }
    pointmask <<= 1;
  }

  flow_AddAnnot( nc_pid, 
	(f_width - f_width_left - annot_width[0])/2,
	f_height - f_header * 0.25 - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);

  if ( annot_count >= 2)
  {
    flow_AddAnnot( nc_pid, 
	f_width - f_width_left + f_strlength,
	f_height - f_header * 0.75 - f_yoffs,
	1, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
  }

  /* Add execute order display */
  flow_AddFilledRect( nc_pid, f_width - GOEN_DISPLAYNODEWIDTH, -f_yoffs, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
  flow_AddAnnot( nc_pid, 
	f_width - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_yoffs,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);

  *node_class = nc_pid;

  free( (char *)bodydef);
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m1()
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
int goen_get_point_info_m1( grectx, graphbody, point, mask, node_width,
			info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long	 point;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_conpoint_type	 *info_pointer;
    vldh_t_node		 node;
{
    int  i;
    int	inputpoints, outputpoints;
    unsigned long    pointmask;
    unsigned long    *mask_pointer;
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
	
	flow_MeasureNode( node->hn.node_id,
			  &ll_x,&ll_y,&ur_x,&ur_y);

	f_width = ur_x - ll_x - 2* GOEN_F_LINEWIDTH;
	f_height = ur_y - ll_y - 2* GOEN_F_LINEWIDTH;
	if ( inputpoints > 0)
	  f_width -= f_pinlength;
	if ( outputpoints > 0)
	  f_width -= f_pinlength;

	if ( point < inputpoints )
	{
	   /* Connectionpoint is an input */
	   info_pointer->x =  - f_width/2.0
		-f_pinlength *( 1 - ( outputpoints == 0 ) * 0.5);
	   info_pointer->y =  f_height/2.0 - f_header/2 - point * f_repeat;
	   info_pointer->type = CON_LEFT;
	}
	else
	{
	   /* Connectionpoint is an output */
	   info_pointer->x =  f_width/2.0 + f_pinlength
		*( 1 - ( inputpoints == 0 ) * 0.5);
	   info_pointer->y =  f_height/2.0 - f_header/2 -
				 (point - inputpoints) * f_repeat;
	   info_pointer->type = CON_RIGHT;
	}
	return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_parameter_m1()
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
int	goen_get_parameter_m1( graphbody, class, ldhses,
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
        output_found = 0;
        input_found = 0;
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
* Name:		goen_get_location_point_m1()
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
int goen_get_location_point_m1( grectx, graphbody, mask, node_width, 
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
