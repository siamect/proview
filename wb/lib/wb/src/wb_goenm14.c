/* wb_goenm14.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

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
	    !<->!!<-  */
									  
#include <stdio.h>
#include <math.h>
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
* Name:		goen_create_nodetype_m14()
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


int goen_create_nodetype_m14( 
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
  int  		i, sts, size;
  int		i_inpoints;
  int		i_outpoints;
  int		inputpoints, outputpoints;
  unsigned long	inpointmask;
  unsigned long	outpointmask;
  unsigned long	*inmask_pointer;
  unsigned long	*outmask_pointer;
  unsigned long	*invertmask_pointer;
  ldh_sParDef 	*bodydef;
  int 		rows;
  char		*parvalue;
  double	code_width;
  double	code_height;
  flow_tNodeClass nc_pid;
  char		name[80];
  int		conpoint_nr;
  int		annot_rows;
  static int	idx = 0;
  flow_tObject	cp;

  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get the text in the parameter Code */
  sts = ldh_GetObjectPar(
			(node->hn.window_pointer)->hw.ldhsession,  
			node->ln.object_did, 
			"DevBody",
			"Code",
			&parvalue, &size); 
  if ( EVEN(sts)) return sts;

  flow_MeasureAnnotText( ctx, parvalue,
	     	flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, flow_eAnnotType_MultiLine,
		&code_width, &code_height, &annot_rows);
  XtFree( parvalue);	

  /* Get the runtime paramteers for this class */
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
  for ( i = 0; i < rows; i++)  
  {
    if ( bodydef[i].ParClass == pwr_eClass_Input)
    {
      inputpoints += ((*inmask_pointer & inpointmask) != 0);
      inpointmask <<= 1;
    }
    if ( bodydef[i].ParClass == pwr_eClass_Output)
    {
      outputpoints += ((*outmask_pointer & outpointmask) != 0);
      outpointmask <<= 1;
    }
  }

  f_height  = (max(inputpoints,outputpoints) + 2)*f_repeat
			+ (floor( code_height / GOEN_F_GRID) + 1) * GOEN_F_GRID;
  f_width = max( f_strlength * (2 + node_width), f_defwidth);
  f_width = max( f_width, code_width + f_strlength *2);
  f_namepos = f_width/2.0 - strlen( graphbody->graphname)*
			      f_strlength/2.0;

  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc_pid);

  /* Draw the rectangle for gate */
  flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);

  /* Draw the separator line for header and footer			      */
  flow_AddLine( nc_pid, 0, f_height - f_repeat - f_yoffs, f_width, 
		f_height - f_repeat - f_yoffs, flow_eDrawType_Line, 2);

  flow_AddLine( nc_pid, 0, 
	(max(inputpoints,outputpoints) + 1)*f_repeat - f_yoffs, 
	f_width, 
	(max(inputpoints,outputpoints) + 1)*f_repeat - f_yoffs, 
	flow_eDrawType_Line, 2);

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
  inpointmask = 1;
  i_inpoints = 0;
  outpointmask = 1;
  i_outpoints = 0;
  for (i = 0; i < rows ; i++)
  {
    if ( bodydef[i].ParClass == pwr_eClass_Input)
    {
      if ( (*inmask_pointer & inpointmask) != 0)
      {
        flow_AddText( nc_pid, bodydef[i].Par->Input.Graph.GraphName,
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
          flow_AddLine( nc_pid, -f_pinlength, f_repeat*(1.5 + i_inpoints) - f_yoffs, 
		-f_circle, f_repeat*(1.5+i_inpoints) - f_yoffs,
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
      inpointmask <<= 1;
    }
    if ( bodydef[i].ParClass == pwr_eClass_Output)
    {
      if ( (*outmask_pointer & outpointmask) != 0)
      {
        f_namelength = strlen( bodydef[i].Par->Output.Graph.GraphName) *
		   f_strlength;

        if ( bodydef[i].ParClass == pwr_eClass_Output) 
        {
          flow_AddText( nc_pid, bodydef[i].Par->Output.Graph.GraphName, 
		f_width-f_nameoffout-f_namelength, 
		f_repeat * (1.5 + i_outpoints) + f_strheight/2 - f_yoffs,
		flow_eDrawType_TextHelvetica, 2);

        }
        flow_AddLine( nc_pid, f_width, f_repeat*(1.5 + i_outpoints) - f_yoffs, 
		f_width+f_pinlength, f_repeat*(1.5+i_outpoints) - f_yoffs,
		flow_eDrawType_Line, 2);
        flow_CreateConPoint( ctx, f_width + f_pinlength, 
		f_repeat * ( i_outpoints + 1.5) - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Right, &cp);
        flow_NodeClassAdd( nc_pid, cp);
        if (bodydef[i].Par->Output.Info.Type == pwr_eType_Float32)
          flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Float32);
        else if (bodydef[i].Par->Output.Info.Type == pwr_eType_Boolean)
          flow_SetTraceAttr( cp, NULL, bodydef[i].ParName, 
		flow_eTraceType_Boolean);
	i_outpoints++;
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
	f_height - (f_repeat - f_strheight)/2 - f_yoffs,
	2, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid, 
	f_strlength,
	f_height - (f_repeat - f_strheight)/2 - f_repeat - code_height +
	code_height/annot_rows - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_MultiLine, flow_mDisplayLevel_1);

  /* Add execute order display */
  flow_AddFilledRect( nc_pid, f_width - GOEN_DISPLAYNODEWIDTH, -f_yoffs, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
  flow_AddAnnot( nc_pid, 
	f_width - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_yoffs,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);

  XtFree( (char *)bodydef);
  *node_class = nc_pid;
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m14()
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
int goen_get_point_info_m14( grectx, graphbody, point, mask, node_width,
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
    unsigned long    inpointmask;
    unsigned long    outpointmask;
    int			sts;
    double		ll_x,ll_y,ur_x,ur_y;
    int			rows;
    ldh_sParDef 	*bodydef;

    	/* Get the runtime paramters for this class */
	sts = ldh_GetObjectBodyDef( (node->hn.window_pointer)->hw.ldhsession, 
			node->ln.classid, "RtBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	/* Count number of inputpoints and outputpoints in mask  */
	inpointmask = 1;
	outpointmask = 1;
	inputpoints = 0;
	outputpoints = 0;
	for ( i = 0; i < rows; i++)  
	{
          if ( bodydef[i].ParClass == pwr_eClass_Input)
	  {
	    inputpoints += ((*mask & inpointmask) != 0);
	    inpointmask <<= 1;
	  }
          if ( bodydef[i].ParClass == pwr_eClass_Output)
	  {
	    outputpoints += ((*(mask+1) & outpointmask) != 0);
	    outpointmask <<= 1;
	  }
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
	XtFree( (char *)bodydef);
	return GOEN__SUCCESS;
}


/*************************************************************************
*
* Name:		goen_get_parameter_m14()
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
int	goen_get_parameter_m14( graphbody, class, ldhses,
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
          if ( bodydef[i].ParClass == pwr_eClass_Input)
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
	}
	if ( !input_found )
	{
	  /* continue with outputs */
	  mask++;
	  pointmask = 1;
	  for ( i = 0; i < rows; i++)  
	  {
            if ( bodydef[i].ParClass == pwr_eClass_Output)
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
	}
	XtFree( (char *)bodydef);
	if ( input_found || output_found ) return GOEN__SUCCESS;
	else return GOEN__NOPOINT;
}



/*************************************************************************
*
* Name:		goen_get_location_point_m14()
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
int goen_get_location_point_m14( grectx, graphbody, mask, node_width, 
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

