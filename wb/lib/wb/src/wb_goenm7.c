/* wb_goenm7.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates a nodetype from the objecttype specification
   and the instanceobject's mask.
	
	---------------------------------------------------------
	!		!					!
  ------!   'classname'	!  'instance name'			!-------	
	!		!					!
	---------------------------------------------------------

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
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;
static	float	f_circle     = GOEN_F_INVERTCIRCLE;
static	float	f_classnamewidth   = 0.10; 	/*	()	*/
static	float	f_yoffs      = GOEN_F_GRID/2;
static	float	f_height;
static	float	f_width;
static	float   f_namelength;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m7()
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


int goen_create_nodetype_m7( 
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
  int		inputpoints, outputpoints;
  unsigned long	pointmask;
  unsigned long	*inmask_pointer;
  unsigned long	*outmask_pointer;
  unsigned long	*invertmask_pointer;
  int 		inputs;
  int 		interns;
  int		outputs;
  int		sts;
  char		annot_str[3][80];
  int		annot_nr[3];
  int		annot_count;
  double	annot_width[3];
  double	annot_height;
  float		f_width_left;

  flow_tNodeClass nc_pid;
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
  else
    annot_width[0] = 0;
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
  inputs = graphbody->parameters[PAR_INPUT];
  interns = graphbody->parameters[PAR_INTERN];
  outputs = graphbody->parameters[PAR_OUTPUT];

  inmask_pointer = mask++;
  outmask_pointer = mask++;
  invertmask_pointer = mask;

  /* Check if input in mask (first bit) */
  pointmask = 1;
  inputpoints = 0;
  inputpoints = ((*inmask_pointer & 1 || *inmask_pointer & 2) != 0);
  // inputpoints += ((*inmask_pointer & pointmask) != 0);

  /* Check if output in mask (first bit) */
  pointmask = 1;
  outputpoints = 0;
  outputpoints += ((*outmask_pointer & pointmask) != 0);
	
  f_classnamewidth = f_strlength * 
		(strlen( graphbody->graphname) + 2);
  f_height  = f_repeat;
  if ( annot_count <= 1)	
    f_width = max( f_strlength * 2 + annot_width[0], f_defwidth) 
			+ f_classnamewidth;
  else
  {
    f_width = max( f_strlength * 4 + annot_width[0] + annot_width[1], 
			f_defwidth + f_strlength * 2) + f_classnamewidth;
    f_width_left = f_strlength * 2 + annot_width[1];
  }

  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Common, 
		&nc_pid);

  /* Draw the rectangle for gate		*/
  flow_AddRect( nc_pid, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);


	/* Draw the separator */

  if ( subwindowmark == 0 )
  {
    flow_AddLine( nc_pid, f_classnamewidth, -f_yoffs, f_classnamewidth, 
		f_height - f_yoffs, flow_eDrawType_Line, 2);
  }
  else
  {
    flow_AddLine( nc_pid, f_classnamewidth, -f_yoffs, f_classnamewidth, 
		f_height - f_yoffs, flow_eDrawType_LineGray, 4);
  }

  if ( annot_count >= 2)
  {
    flow_AddLine( nc_pid, f_width - f_width_left, -f_yoffs, 
		f_width - f_width_left, f_height - f_yoffs, flow_eDrawType_Line, 2);
  }

  flow_AddText( nc_pid, graphbody->graphname, f_strlength, 
		f_height/2 + f_strheight/2 - f_yoffs, 
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);

  /* Draw the leadnames and lines */
  conpoint_nr = 0;
  if ( inputpoints != 0 )
  {
    if ( !(((*inmask_pointer & 1 ) && (*invertmask_pointer & 1)) ||
           ((*inmask_pointer & 2 ) && (*invertmask_pointer & 2))))
    {
      flow_AddLine( nc_pid, -f_pinlength, f_height/2 - f_yoffs, 
		0, f_height/2 - f_yoffs, flow_eDrawType_Line, 2);
    }	  
    else
    {
      flow_AddLine( nc_pid, -f_pinlength, f_height/2 - f_yoffs, 
		-f_circle, f_height/2 - f_yoffs,
		flow_eDrawType_Line, 2);
      flow_AddArc( nc_pid, -f_circle, 
		f_height/2 - f_circle / 2 - f_yoffs, 
		0, f_height/2 + f_circle / 2 - f_yoffs, 0, 360, 
		flow_eDrawType_Line, 2);
    }
    flow_AddConPoint( nc_pid, -f_pinlength, 
		f_height/2 - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Left);
  }

  if (outputpoints != 0)
  {
    flow_AddLine( nc_pid, f_width, f_height/2 - f_yoffs, 
		f_width+f_pinlength, f_height/2 - f_yoffs,
		flow_eDrawType_Line, 2);
    flow_CreateConPoint( ctx, f_width + f_pinlength, 
		f_height/2 - f_yoffs, 
		conpoint_nr++,
		flow_eDirection_Right, &cp);
    flow_NodeClassAdd( nc_pid, cp);
    if (bodydef[inputs+interns].Par->Output.Info.Type == pwr_eType_Float32)
      flow_SetTraceAttr( cp, NULL,
		bodydef[inputs+interns].Par->Output.Info.PgmName, 
		flow_eTraceType_Float32);
    else if (bodydef[inputs+interns].Par->Output.Info.Type == pwr_eType_Boolean)
      flow_SetTraceAttr( cp, NULL, 
		bodydef[inputs+interns].Par->Output.Info.PgmName, 
		flow_eTraceType_Boolean);
  }

  f_namelength = f_strlength*6;

  flow_AddAnnot( nc_pid, 
	f_classnamewidth + f_strlength,
	f_height/2 + f_strheight/2 - f_yoffs,
	0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_1);

  if ( annot_count >= 2)
  {
    flow_AddAnnot( nc_pid, 
	f_width - f_width_left + f_strlength,
	f_height/2 + f_strheight/2 - f_yoffs,
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
  XtFree((char *) bodydef);
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m7()
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
int goen_get_point_info_m7( grectx, graphbody, point, mask, node_width,
			info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long	 point;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_conpoint_type	 *info_pointer;
    vldh_t_node		 node;
{
    int	inputpoints, outputpoints;
    unsigned long    pointmask;
    unsigned long    *inmask_pointer;
    unsigned long    *outmask_pointer;
    int 		inputs;
    int 		interns;
    int			outputs;
    double		ll_x,ll_y,ur_x,ur_y;


	/* Get number of parameters */
	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	outputs = graphbody->parameters[PAR_OUTPUT];

	inmask_pointer = mask++;
	outmask_pointer = mask;

	/* Check if input in mask (first bit) */
	pointmask = 1;
	inputpoints = 0;
	inputpoints = ((*inmask_pointer & 1 || *inmask_pointer & 2) != 0);
	// inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	pointmask = 1;
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);
	
        flow_MeasureNode( node->hn.node_id, &ll_x, &ll_y, &ur_x, &ur_y);
        f_width = ur_x - ll_x;
 	f_height = ur_y - ll_y;

	f_width -= 2* GOEN_F_LINEWIDTH;
	f_height -= 2* GOEN_F_LINEWIDTH;
	if ( inputpoints > 0)
	  f_width -= f_pinlength;
	if ( outputpoints > 0)
	  f_width -= f_pinlength;

	if (( point == 0) && (inputpoints != 0))
	{
	   info_pointer->x =  - f_width/2.0
		-f_pinlength *( 1 - ( outputpoints == 0 ) * 0.5);
	   info_pointer->y = 0.0;
	   info_pointer->type = CON_LEFT;
	}
	if ((point == 1) || ((point == 0) && (inputpoints == 0)))
	{
	  info_pointer->x =  f_width/2.0
		+ f_pinlength *( 1 - ( inputpoints == 0 ) * 0.5);
	  info_pointer->y = 0.0;
	  info_pointer->type = CON_RIGHT;
	}
	return GOEN__SUCCESS;    
}



/*************************************************************************
*
* Name:		goen_get_parameter_m7()
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
int	goen_get_parameter_m7( graphbody, class, ldhses,
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

        input_found = 0;
        output_found = 0;
	pointmask = 1;
	/* Check if input in mask (first bit) */
	inputpoints = ((*inmask_pointer & 1 || *inmask_pointer & 2) != 0);
        // inputpoints += ((*inmask_pointer & pointmask) != 0);

	/* Check if output in mask (first bit) */
	outputpoints = 0;
	outputpoints += ((*outmask_pointer & pointmask) != 0);
	
	if (( con_point == 0) && (inputpoints != 0))
	{
	    *par_type = PAR_INPUT;
            if ( *inmask_pointer & 1)
	      *par_index = 0;
            else
              *par_index = 1;
            if ( !(((*inmask_pointer & 1 ) && (*invertmask_pointer & 1)) ||
                   ((*inmask_pointer & 2 ) && (*invertmask_pointer & 2))))
	      *par_inverted = GOEN_NOT_INVERTED;
	    else
	      *par_inverted = GOEN_INVERTED;
	    input_found = 1;
	}
	if ((con_point == 1) || ((con_point == 0) && (inputpoints == 0)))
	{
	      *par_type = PAR_OUTPUT;
	      *par_index =  inputs + interns;
	      *par_inverted = GOEN_NOT_INVERTED;
              output_found = 1;
	}
	if ( input_found || output_found ) return GOEN__SUCCESS;
	else return GOEN__NOPOINT;
}


/*************************************************************************
*
* Name:		goen_get_location_point_m7()
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
int goen_get_location_point_m7( grectx, graphbody, mask, node_width, 
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





