/* wb_goenm16.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates a nodetype from the objecttype specification
   and the instanceobject's mask.
									  
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
	    !	!!      'inst.name'     !				  
            !   !=======================!				  
	    ! a	!! h							  
	    !<->!!<-							  
   */									  

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
static	float	f_defwidth   = GOEN_F_MINWIDTH * GOEN_F_OBJNAME_STRWIDTH;
static	float	f_pinlength  = GOEN_F_PINLENGTH;
static	float	f_repeat     = GOEN_F_GRID;
static	float	f_nameoffin  = GOEN_F_OBJNAME_STRWIDTH / 2.;
static	float	f_nameoffout = GOEN_F_OBJNAME_STRWIDTH / 2.;
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;
static	float	f_circle     = GOEN_F_INVERTCIRCLE;
static	float	f_corner     = GOEN_F_GRID / 2;
static	float	f_yoffs      = GOEN_F_GRID*3/2;
static	float	f_height;
static	float	f_width;
static	float	f_namepos;
static	float   f_namelength;
static	float   f_node_width;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m16()
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


int goen_create_nodetype_m16( 
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
  int  i, sts;
  int	i_inpoints, i_inpoints_top, i_inpoints_bottom;
  int	i_outpoints, i_outpoints_top, i_outpoints_bottom;
  int	inputpoints, outputpoints, inputpoints_top, inputpoints_bottom,
		outputpoints_top, outputpoints_bottom;
  int	points_at_top, points_at_bottom, points_at_left, points_at_right;
  unsigned long    inpointmask;
  unsigned long    outpointmask;
  unsigned long    *inmask_pointer;
  unsigned long    *outmask_pointer;
  unsigned long    *invertmask_pointer;
  ldh_sParDef 	*bodydef;
  int 		rows;

  flow_tNodeClass nc_pid;
  char		name[80];
  int		size;
  int		conpoint_nr;
  static int	idx = 0;


  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get the runtime paramters for this class */
  sts = ldh_GetObjectBodyDef(ldhses, class, "RtBody", 1, 
			&bodydef, &rows);
  if ( EVEN(sts) ) return sts;

  inmask_pointer = mask++;
  outmask_pointer = mask++;
  invertmask_pointer = mask;

  /* Count number of inputpoints and outputpoints in mask  */
  inpointmask = 1;
  outpointmask = 1;
  inputpoints = 0;
  outputpoints = 0;
  inputpoints_top = 0;
  outputpoints_top = 0;
  inputpoints_bottom = 0;
  outputpoints_bottom = 0;
  for ( i = 0; i < rows; i++)  
  {
    if ( bodydef[i].ParClass == pwr_eClass_Input)
    {
      if ( bodydef[i].Par->Input.Graph.InputType == 1)
	inputpoints_top += ((*inmask_pointer & inpointmask) != 0);
      else if ( bodydef[i].Par->Input.Graph.InputType == 2)
        inputpoints_bottom += ((*inmask_pointer & inpointmask) != 0);
      else    
	inputpoints += ((*inmask_pointer & inpointmask) != 0);
      inpointmask <<= 1;
    }
    if ( bodydef[i].ParClass == pwr_eClass_Output)
    {
      if ( bodydef[i].Par->Input.Graph.InputType == 1)
        outputpoints_top += ((*outmask_pointer & outpointmask) != 0);
      else if ( bodydef[i].Par->Input.Graph.InputType == 2)
        outputpoints_bottom += ((*outmask_pointer & outpointmask) != 0);
      else    
        outputpoints += ((*outmask_pointer & outpointmask) != 0);
      outpointmask <<= 1;
    }
  }
	
  points_at_top = ((inputpoints_top + outputpoints_top) != 0);
  points_at_bottom = ((inputpoints_bottom + outputpoints_bottom) != 0);
  points_at_left = (inputpoints != 0);
  points_at_right = (outputpoints != 0);

  f_height  = 3 * f_repeat +
	       (max(inputpoints,outputpoints)-1)*f_repeat;

  f_width = max( f_strlength * (2 + node_width), f_defwidth);
  f_namepos = f_width/2.0 - strlen( graphbody->graphname)*
			      f_strlength/2.0;

  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc_pid);

  /* Draw the rectangle for gate  */

  if ( graphbody->graphindex == 1)
  {
    /* Draw the rectangle for gate */
    flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_LineErase, 2, flow_mDisplayLevel_1);
    if ( f_height > 2 * f_corner)
    {
      flow_AddLine( nc_pid, 0, f_corner - f_yoffs, 
		0, f_height - f_corner - f_yoffs,
		flow_eDrawType_Line, 2);
    }
    flow_AddArc( nc_pid, 0, f_height - 2*f_corner - f_yoffs,
		2*f_corner, f_height - f_yoffs,
		180, 90, flow_eDrawType_Line, 2);
    flow_AddLine( nc_pid, f_corner, f_height - f_yoffs, 
		f_width - f_corner, f_height - f_yoffs,
		flow_eDrawType_Line, 2);
    flow_AddArc( nc_pid, f_width - 2*f_corner, f_height - 2*f_corner - f_yoffs,
		f_width, f_height - f_yoffs,
		270, 90, flow_eDrawType_Line, 2);
    if ( f_height > 2 * f_corner)
    {
      flow_AddLine( nc_pid, f_width, f_corner - f_yoffs, 
		f_width, f_height - f_corner - f_yoffs,
		flow_eDrawType_Line, 2);
    }
    flow_AddArc( nc_pid, f_width - 2*f_corner, -f_yoffs,
		f_width, 2*f_corner - f_yoffs,
		0, 90, flow_eDrawType_Line, 2);
    flow_AddLine( nc_pid, f_corner, -f_yoffs, 
		f_width - f_corner, -f_yoffs,
		flow_eDrawType_Line, 2);
    flow_AddArc( nc_pid, 0, -f_yoffs,
		2*f_corner, 2*f_corner - f_yoffs,
		90, 90, flow_eDrawType_Line, 2);
  }
  else
  {
    flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
  }

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

  /* Draw the leadnames and lines					      */

  conpoint_nr = 0;
  inpointmask = 1;
  i_inpoints = 0;
  i_inpoints_top = 0;
  i_inpoints_bottom = 0;
  outpointmask = 1;
  i_outpoints = 0;
  i_outpoints_top = 0;
  i_outpoints_bottom = 0;
  for (i = 0; i < rows ; i++)
  {
    if ( bodydef[i].ParClass == pwr_eClass_Input)
    {
      if ( (*inmask_pointer & inpointmask) != 0)
      {
        if ( bodydef[i].Par->Input.Graph.InputType == 2)
	{
          flow_AddLine( nc_pid, 
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1),
		f_height - f_yoffs,
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1),
		f_height + f_pinlength - f_yoffs,
		flow_eDrawType_Line, 2);
          flow_AddConPoint( nc_pid,
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1), 
		f_height + f_pinlength - f_yoffs,
		conpoint_nr++,
		flow_eDirection_Up);
	  i_inpoints_bottom++;
        }
        else if ( bodydef[i].Par->Input.Graph.InputType != 1)
	{
          flow_AddText( nc_pid, bodydef[i].Par->Output.Graph.GraphName, 
		f_nameoffin, 
		f_repeat * (1.5 + i_inpoints) + f_strheight/2 - f_yoffs,
		flow_eDrawType_TextHelvetica, 2);

          if ( (*invertmask_pointer & inpointmask) == 0)
	  {
            flow_AddLine( nc_pid, 0, f_repeat*(1.5 + i_inpoints) - f_yoffs, 
		-f_pinlength, f_repeat*(1.5+i_inpoints) - f_yoffs,
		flow_eDrawType_Line, 2);
	  }
	  else
	  {
            flow_AddLine( nc_pid, 
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1), 
		-f_pinlength - f_yoffs,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1), 
		-f_yoffs,
		flow_eDrawType_Line, 2);
            flow_AddArc( nc_pid, -f_circle, 
		f_repeat*(1.5+i_inpoints) - f_circle / 2 - f_yoffs, 
		0, f_repeat*(1.5+i_inpoints) + f_circle / 2 - f_yoffs, 0, 360, 
		flow_eDrawType_Line, 2);
	  }
          flow_AddConPoint( nc_pid, -f_pinlength, 
		f_repeat * ( 1.5 + i_inpoints) - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Left);
	  i_inpoints++;
	}
        else	  
	{
          flow_AddLine( nc_pid, 
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1), 
		-f_pinlength - f_yoffs,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1), 
		-f_yoffs,
		flow_eDrawType_Line, 2);
          flow_AddConPoint( nc_pid,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1),
		-f_pinlength - f_yoffs,
		conpoint_nr++,
		flow_eDirection_Down);
          i_inpoints_top++;
        }
      }
      inpointmask <<= 1;
    }
    if ( bodydef[i].ParClass == pwr_eClass_Output)
    {
      if ( (*outmask_pointer & outpointmask) != 0)
      {
        if ( bodydef[i].Par->Output.Graph.InputType == 2)
	{
          flow_AddLine( nc_pid,
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1),
		f_height - f_yoffs,
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1),
		f_height + f_pinlength - f_yoffs,
		flow_eDrawType_Line, 2);
          flow_AddConPoint( nc_pid,
		f_repeat * ( i_inpoints_bottom + i_outpoints_bottom + 1),
		f_height + f_pinlength -f_yoffs,
		conpoint_nr++,
		flow_eDirection_Up);
	  i_inpoints_bottom++;
	}
        else if ( bodydef[i].Par->Output.Graph.InputType != 1)
	{
          f_namelength = strlen( bodydef[i].Par->Output.Graph.GraphName) *
		   f_strlength;
          flow_AddText( nc_pid, bodydef[i].Par->Output.Graph.GraphName, 
		f_width - f_nameoffout - f_namelength,
		f_repeat * (1.5 + i_outpoints) + f_strheight/2 - f_yoffs,
		flow_eDrawType_TextHelvetica, 2);
          flow_AddLine( nc_pid, f_width, f_repeat*(1.5 + i_outpoints) - f_yoffs, 
		f_width+f_pinlength, f_repeat*(1.5+i_outpoints) - f_yoffs,
		flow_eDrawType_Line, 2);
          flow_AddConPoint( nc_pid, f_width + f_pinlength, 
		f_repeat * ( i_outpoints + 1.5) - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Right);
	  i_outpoints++;
        }
	else	 
        {
          flow_AddLine( nc_pid,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1),
		-f_pinlength - f_yoffs,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1),
		-f_yoffs,
		flow_eDrawType_Line, 2);
          flow_AddConPoint( nc_pid,
		f_repeat * ( i_inpoints_top + i_outpoints_top + 1),
		-f_pinlength - f_yoffs,
		conpoint_nr++,
		flow_eDirection_Down);
	  i_inpoints_top++;
        }
      }
      outpointmask <<= 1;
    }
  }

  f_namelength = f_strlength*6;
  f_node_width = node_width;
  if ( node_width == 0 )
    f_node_width = 6;

  flow_AddAnnot( nc_pid, 
	f_strlength,
	f_height - (f_repeat - f_strheight)/2.0 - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);

  /* Add execute order display */
  flow_AddFilledRect( nc_pid, f_width - GOEN_DISPLAYNODEWIDTH, -f_yoffs, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
  flow_AddAnnot( nc_pid, 
	f_width - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_yoffs,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);

  free((char *) bodydef);
  *node_class = nc_pid;
  return GOEN__SUCCESS;
}                                                            



/*************************************************************************
*
* Name:		goen_get_point_info_m16()
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
int goen_get_point_info_m16( grectx, graphbody, point_nr, mask, node_width,
			info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long	 point_nr;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_conpoint_type	 *info_pointer;
    vldh_t_node		 node;
{
    int	point;
    int  i, sts, imin, imax;
    int	inputpoints, outputpoints, inputpoints_top, inputpoints_bottom,
		outputpoints_top, outputpoints_bottom;
    int	points_top, points_bottom;
    int	points_at_top, points_at_bottom, points_at_left, points_at_right;
    unsigned long    inpointmask;
    unsigned long    outpointmask;
    int			rows;
    ldh_sParDef 	*bodydef;

        point = point_nr;

    	/* Get the runtime paramters for this class */
	sts = ldh_GetObjectBodyDef( (node->hn.wind)->hw.ldhses, 
			node->ln.cid, "RtBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	/* Count number of inputpoints and outputpoints in mask  */
	inpointmask = 1;
	outpointmask = 1;
	inputpoints = 0;
	outputpoints = 0;
	inputpoints_top = 0;
	outputpoints_top = 0;
	inputpoints_bottom = 0;
	outputpoints_bottom = 0;
	for ( i = 0; i < rows; i++)  
	{
          if ( bodydef[i].ParClass == pwr_eClass_Input)
	  {
            if ( bodydef[i].Par->Input.Graph.InputType == 1)
	      inputpoints_top += ((*mask & inpointmask) != 0);
            else if ( bodydef[i].Par->Input.Graph.InputType == 2)
	      inputpoints_bottom += ((*mask & inpointmask) != 0);
	    else    
	      inputpoints += ((*mask & inpointmask) != 0);
	    inpointmask <<= 1;
	  }
          if ( bodydef[i].ParClass == pwr_eClass_Output)
	  {
            if ( bodydef[i].Par->Input.Graph.InputType == 1)
	      outputpoints_top += ((*(mask+1) & outpointmask) != 0);
            else if ( bodydef[i].Par->Input.Graph.InputType == 2)
	      outputpoints_bottom += ((*(mask+1) & outpointmask) != 0);
	    else    
	      outputpoints += ((*(mask+1) & outpointmask) != 0);
	    outpointmask <<= 1;
	  }
	}
	points_at_top = ((inputpoints_top + outputpoints_top) != 0);
	points_at_bottom = ((inputpoints_bottom + outputpoints_bottom) != 0);
	points_at_left = (inputpoints != 0);
	points_at_right = (outputpoints != 0);

	f_height  = (max(inputpoints,outputpoints) + 2)*f_repeat;
	f_width = max( f_strlength * (2 + node_width), f_defwidth);

	points_top = 0;
	points_bottom = 0;
	imin = 0;
	imax = inputpoints_top;
	if ((point >= imin) && (point < imax))
	{
	  info_pointer->x =  - f_width/2.0
		+ f_pinlength  * (points_at_left - points_at_right) / 2.0 
		+ f_repeat * ( point - imin + 1);
      	  info_pointer->y = f_height / 2.0
		+ f_pinlength * ( 1 + points_at_bottom) / 2.0;
	  info_pointer->type = CON_UP;
    	}

	imin = imax;
	imax += inputpoints_bottom;
	if ((point >= imin) && (point < imax))
	{
      	  info_pointer->x =  - f_width/2.0
		+ f_pinlength  * (points_at_left - points_at_right) / 2.0 
		+ f_repeat * ( point - imin + 1);
      	  info_pointer->y = - f_height / 2.0
		- f_pinlength * ( 1 + points_at_top) / 2.0;
	  info_pointer->type = CON_DOWN;
	}
    
	imin = imax;
	imax += inputpoints;
	if ((point >= imin) && (point < imax))
	{
	  /* Connectionpoint is an left input */
	  info_pointer->x = - f_width/2.0
		 - f_pinlength  * ( 1 + points_at_right) / 2.0;
	  info_pointer->y = f_height / 2.0  
		- f_repeat * ( point - imin + 1.5)
		 + f_pinlength  * ( points_at_top - points_at_bottom)/ 2.0;
	  info_pointer->type = CON_LEFT;
	}
	imin = imax;
	imax += outputpoints_top;
	if ((point >= imin) && (point < imax))
	{
      	  info_pointer->x =  - f_width/2.0
		 + f_pinlength  * ( points_at_left - points_at_right) / 2.0 
		 + f_repeat * ( point - imin + 1);
	  info_pointer->y = f_height / 2.0
		 + f_pinlength * ( 1 + points_at_bottom) / 2.0;
	  info_pointer->type = CON_UP;
	}
	imin = imax;
	imax += outputpoints_bottom;
	if ((point >= imin) && (point < imax))
	{
	  info_pointer->x =  - f_width/2.0
		 + f_pinlength  * (points_at_left - points_at_right) / 2.0
		 + f_repeat * ( point - imin + 1);
	  info_pointer->y = - f_height / 2.0
		 -  f_pinlength * ( 1 + points_at_top) / 2.0;
	  info_pointer->type = CON_DOWN;
	  points_bottom++;
	}
	imin = imax;
	imax += outputpoints;
	if ((point >= imin) && (point < imax))
	{
      	  info_pointer->x =  f_width/2.0
		+ f_pinlength * (1 + points_at_left) / 2.0;
      	  info_pointer->y = f_height / 2.0  
		- f_repeat * ( point - imin + 1.5)
		- f_pinlength  * ( points_at_top - points_at_bottom)/ 2.0;
	  info_pointer->type = CON_RIGHT;
	}

	free( (char *)bodydef);
	return GOEN__SUCCESS;
}


/*************************************************************************
*
* Name:		goen_get_parameter_m16()
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
int	goen_get_parameter_m16( graphbody, class, ldhses,
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

	int		sts;
	unsigned long	conpointcount;
	unsigned long	pointmask;
	int		i, input_found, output_found;
	unsigned long	*invertmask_pointer;
	int		rows;
	ldh_sParDef 	*bodydef;

    	/* Get the runtime paramters for this class */
	sts = ldh_GetObjectBodyDef( ldhses,
			class, "RtBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	/* Identify the conpoints corresponding input or output */
	invertmask_pointer = mask + 2;
	*par_type = 0;
	conpointcount = 0;
	pointmask = 1;
        input_found = 0;
        output_found = 0;
	for ( i = 0; i < rows; i++)
	{
          if ( (bodydef[i].ParClass == pwr_eClass_Input) && 
		(bodydef[i].Par->Input.Graph.InputType != 0))
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
	      free( (char *)bodydef);
	      return GOEN__SUCCESS;
	    }
	    pointmask <<= 1;
	  }
	}

	for ( i = 0; i < rows; i++)
	{
          if ( (bodydef[i].ParClass == pwr_eClass_Input) && 
		(bodydef[i].Par->Input.Graph.InputType == 0))
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
	      free( (char *)bodydef);
	      return GOEN__SUCCESS;
	    }
	    pointmask <<= 1;
	  }
	}
	/* continue with outputs */
	mask++;
	pointmask = 1;
	for ( i = 0; i < rows; i++)  
	{
          if ( (bodydef[i].ParClass == pwr_eClass_Output) &&
		(bodydef[i].Par->Input.Graph.InputType != 0))
	  {
	    conpointcount += ((*mask & pointmask) != 0);
	    if ( conpointcount == (con_point + 1) )
	    {
	      /* this is the output */
	      *par_type = PAR_OUTPUT;
	      *par_index = i;
	      *par_inverted = GOEN_NOT_INVERTED;
	      output_found = 1;
	      free( (char *)bodydef);
	      return GOEN__SUCCESS;
	    }
	    pointmask <<= 1;
	  }
	}

	for ( i = 0; i < rows; i++)  
	{
          if ( (bodydef[i].ParClass == pwr_eClass_Output) &&
		(bodydef[i].Par->Input.Graph.InputType == 0))
	  {
	    conpointcount += ((*mask & pointmask) != 0);
	    if ( conpointcount == (con_point + 1) )
	    {
	      /* this is the output */
	      *par_type = PAR_OUTPUT;
	      *par_index = i;
	      *par_inverted = GOEN_NOT_INVERTED;
	      output_found = 1;
	      free( (char *)bodydef);
	      return GOEN__SUCCESS;
	    }
	    pointmask <<= 1;
	  }
	}
	free( (char *)bodydef);
	return GOEN__NOPOINT;
}



/*************************************************************************
*
* Name:		goen_get_location_point_m16()
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
int goen_get_location_point_m16( grectx, graphbody, mask, node_width, 
		info_pointer, node)
    gre_ctx		grectx;
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

