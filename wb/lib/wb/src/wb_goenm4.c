/* wb_goenm4.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>

#include "wb_foe_macros.h"

#include <Xm/Xm.h>

#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"

typedef enum {
	goen_eGraphIndex_Step 		= 2,
	goen_eGraphIndex_InitStep 	= 3,
	goen_eGraphIndex_SubStep 	= 4,
	goen_eGraphIndex_Trans 		= 5,
	goen_eGraphIndex_Point 		= 6,
	goen_eGraphIndex_OrderAct 	= 10,
	goen_eGraphIndex_SetCond	= 11,
	goen_eGraphIndex_SSBegin 	= 12,
	goen_eGraphIndex_SSEnd 		= 13,
	goen_eGraphIndex_ShowPlcAttr 	= 15,
	goen_eGraphIndex_MpsIn 		= 25,
	goen_eGraphIndex_MpsQueue	= 26,
	goen_eGraphIndex_MpsStack	= 27,
	goen_eGraphIndex_MpsWare	= 28,
	goen_eGraphIndex_MpsTube	= 29,
	goen_eGraphIndex_MpsOut		= 30,
	goen_eGraphIndex_MpsTrp		= 31,
	goen_eGraphIndex_GetAgeneric	= 32,
	goen_eGraphIndex_GetDgeneric	= 33,
	goen_eGraphIndex_StoAgeneric	= 34,
	goen_eGraphIndex_StoDgeneric	= 35,
	goen_eGraphIndex_GetSgeneric	= 36,
	goen_eGraphIndex_StoSgeneric	= 37,
	goen_eGraphIndex_GetIgeneric	= 38,
	goen_eGraphIndex_StoIgeneric	= 39
	} goen_eGraphIndex;

static	float	f_pinlength  = GOEN_F_PINLENGTH;
static	float	f_strlength  = GOEN_F_OBJNAME_STRWIDTH;
static	float	f_strheight  = GOEN_F_OBJNAME_STRHEIGHT;


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m4()
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


int goen_create_nodetype_m4( 
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
  int		graph_index;
  int		annot_count;
  int		sts;
  char		annot_str[3][80];
  int		annot_nr[3];
  float		f_width, f_height;
  char		name[80];
  int		size;
  flow_tNodeClass	nc;
  static int	idx = 0;
	
  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get graph index for this class */
  graph_index = graphbody->graphindex;	

  /* Get number of annotations and the width of the annotations */
  sts = gre_get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		sizeof( annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
  if ( EVEN(sts)) return sts;

  flow_CreateNodeClass( ctx, name, flow_eNodeGroup_Common, &nc);

  switch ( graph_index)
  {
    case goen_eGraphIndex_Step:
    {
      f_width = GOEN_F_GRID * 2.5;
      f_height = GOEN_F_GRID * 2.5;

      flow_AddRect( nc, -f_width/2, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, 0, -f_height/2 - f_pinlength, 
		0, -f_height/2, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, -f_height/2 - f_pinlength, 
		0, flow_eDirection_Down);
      flow_AddLine( nc, f_width/2, 0, 
		f_width/2 + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, f_width/2 + f_pinlength, 0, 
		1, flow_eDirection_Right);
      flow_AddLine( nc, 0, f_height/2, 
		0, f_height/2 + f_pinlength, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, f_height/2 + f_pinlength, 
		2, flow_eDirection_Up);
      flow_AddAnnot( nc, -f_width/2 + f_strlength, 0,
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);

      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_InitStep:
    {
      float f_delta = GOEN_F_GRID / 4;

      f_width = GOEN_F_GRID * 2.5;
      f_height = GOEN_F_GRID * 2.5;
      flow_AddRect( nc, -f_width/2, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddRect( nc, -f_width/2 + f_delta, -f_height/2 + f_delta, 
		f_width - 2 * f_delta, f_height - 2 * f_delta, 
		flow_eDrawType_Line, 1, flow_mDisplayLevel_1);
      flow_AddLine( nc, 0, -f_height/2 - f_pinlength, 
		0, -f_height/2, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, -f_height/2 - f_pinlength, 
		0, flow_eDirection_Down);
      flow_AddLine( nc, f_width/2, 0, 
		f_width/2 + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, f_width/2 + f_pinlength, 0, 
		1, flow_eDirection_Right);
      flow_AddLine( nc, 0, f_height/2, 
		0, f_height/2 + f_pinlength, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, f_height/2 + f_pinlength, 
		2, flow_eDirection_Up);
      flow_AddAnnot( nc, -f_width/2 + f_strlength, 0, 
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_SubStep:
    {
      float f_delta = GOEN_F_GRID / 4;

      f_width = GOEN_F_GRID * 2.5;
      f_height = GOEN_F_GRID * 2.5;
      if ( subwindowmark & 1)
        flow_AddLine( nc, -f_width/2, -f_height/2 + f_delta, 
		f_width/2, -f_height/2 + f_delta, flow_eDrawType_LineGray, 6);
      else
        flow_AddLine( nc, -f_width/2, -f_height/2 + f_delta, 
		f_width/2, -f_height/2 + f_delta, flow_eDrawType_Line, 6);
      flow_AddRect( nc, -f_width/2, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);

      flow_AddLine( nc, 0, -f_height/2 - f_pinlength, 
		0, -f_height/2, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, -f_height/2 - f_pinlength, 
		0, flow_eDirection_Down);
      flow_AddLine( nc, f_width/2, 0, 
		f_width/2 + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, f_width/2 + f_pinlength, 0, 
		1, flow_eDirection_Right);
      flow_AddLine( nc, 0, f_height/2, 
		0, f_height/2 + f_pinlength, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, f_height/2 + f_pinlength, 
		2, flow_eDirection_Up);
      flow_AddAnnot( nc, -f_width/2 + f_strlength, 0, 
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_SSEnd:
    {
      f_width = GOEN_F_GRID * 2.5;
      f_height = GOEN_F_GRID * 2.5;
      flow_AddRect( nc, -f_width/2, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, 0, -f_height/2 - f_pinlength, 
		0, -f_height/2, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, -f_height/2 - f_pinlength, 
		0, flow_eDirection_Down);
      flow_AddLine( nc, f_width/2, 0, 
		f_width/2 + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, f_width/2 + f_pinlength, 0, 
		1, flow_eDirection_Right);
      flow_AddAnnot( nc, -f_width/2 + f_strlength, 0,
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_SSBegin:
    {
      f_width = GOEN_F_GRID * 2.5;
      f_height = GOEN_F_GRID * 2.5;
      flow_AddRect( nc, -f_width/2, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, f_width/2, 0, 
		f_width/2 + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, f_width/2 + f_pinlength, 0, 
		0, flow_eDirection_Right);
      flow_AddLine( nc, 0, f_height/2, 
		0, f_height/2 + f_pinlength, flow_eDrawType_Line, 2);
      flow_AddConPoint(nc, 0, f_height/2 + f_pinlength, 
		1, flow_eDirection_Up);
      flow_AddAnnot( nc, -f_width/2 + f_strlength, 0,
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_Trans:
    {
      float f_delta = GOEN_F_GRID / 5;

      f_width = GOEN_F_GRID * 1.6;
      if ( subwindowmark & 1)
        flow_AddLine( nc, -f_width/2, f_delta, 
		f_width/2, f_delta, flow_eDrawType_LineGray, 6);

      flow_AddLine( nc, -f_width/2, 0, f_width/2, 0, flow_eDrawType_Line, 4);
      flow_AddLine( nc, 0, - f_pinlength, 0, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, 0, - f_pinlength, 
		0, flow_eDirection_Down);
      flow_AddLine( nc, 0, 0, 0, f_pinlength, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, 0, f_pinlength, 
		2, flow_eDirection_Up);
      flow_AddConPoint( nc, -f_width/2, 0, 
		1, flow_eDirection_Left);
      flow_AddAnnot( nc, f_strlength, - f_strheight/2, 
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      /* Add execute order display */
      flow_AddFilledRect( nc, f_width/2 - GOEN_DISPLAYNODEWIDTH, 0, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width/2 - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_Point:
    {
      float f_radius = GOEN_F_GRID / 5;

      flow_AddArc( nc, -f_radius/2, -f_radius/2, f_radius/2, f_radius/2,
		0, 360, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, 0, 0,
		0, flow_eDirection_Center);
      break;
    }
    case goen_eGraphIndex_OrderAct:
    {
      f_width = GOEN_F_GRID * 4;
      f_height = GOEN_F_GRID; 

      flow_AddRect( nc, 0, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, f_width, 0, f_width + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, f_width + f_pinlength, 0,
		0, flow_eDirection_Right);
      flow_AddText( nc, "OrderAct", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      break;
    }
    case goen_eGraphIndex_SetCond:
    {
      f_width = GOEN_F_GRID * 4;
      f_height = GOEN_F_GRID; 

      flow_AddRect( nc, 0, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, -f_pinlength, 0, 0, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, -f_pinlength, 0,
		0, flow_eDirection_Left);
      flow_AddText( nc, "SetCond", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);

      /* Add execute order display */
      flow_AddFilledRect( nc, f_width - GOEN_DISPLAYNODEWIDTH, -f_height/2, 
		GOEN_DISPLAYNODEWIDTH, GOEN_DISPLAYNODEHEIGHT, 
		flow_eDrawType_LineGray, flow_mDisplayLevel_2);
      flow_AddAnnot( nc, 
	f_width - GOEN_DISPLAYNODEWIDTH + f_strlength,
	(GOEN_DISPLAYNODEHEIGHT + f_strheight)/2.0 - f_height/2,
	 GOEN_DISPLAYNODE_ANNOT, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
	flow_eAnnotType_OneLine, flow_mDisplayLevel_2);
      break;
    }
    case goen_eGraphIndex_ShowPlcAttr:
    {
      float f_yoffs = GOEN_F_GRID / 2;
      float f_repeat = GOEN_F_GRID;
      f_width = GOEN_F_GRID * 9;
      f_height = GOEN_F_GRID * 3; 

      flow_AddRect( nc, 0, -f_yoffs, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, 0, f_repeat - f_yoffs, f_width, f_repeat - f_yoffs, 
		flow_eDrawType_Line, 2);
      flow_AddLine( nc, 0, f_repeat * 2 - f_yoffs, f_width, f_repeat * 2 - f_yoffs, 
		flow_eDrawType_Line, 2);
      flow_AddAnnot( nc, f_repeat*2.7, f_repeat - f_strheight*0.25 - f_yoffs, 
		0, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      flow_AddAnnot( nc, f_repeat*2.7, f_repeat*2 - f_strheight*0.25 - f_yoffs, 
		1, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      flow_AddAnnot( nc, f_repeat*2.7, f_repeat*3 - f_strheight*0.25 - f_yoffs, 
		2, flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE, 
		flow_eAnnotType_OneLine, flow_mDisplayLevel_1);
      flow_AddText( nc, "Volume", f_strlength/3, f_repeat - f_strheight*0.3 - f_yoffs, 
		flow_eDrawType_TextHelvetica, 1);
      flow_AddText( nc, "ScanTime", f_strlength/3, f_repeat*2 - f_strheight*0.3 - f_yoffs, 
		flow_eDrawType_TextHelvetica, 1);
      flow_AddText( nc, "Reset", f_strlength/3, f_repeat*3 - f_strheight*0.3 - f_yoffs, 
		flow_eDrawType_TextHelvetica, 1);
      break;
    }
    case goen_eGraphIndex_GetAgeneric:
    case goen_eGraphIndex_GetDgeneric:
    case goen_eGraphIndex_GetSgeneric:
    case goen_eGraphIndex_GetIgeneric:
    {
      f_width = GOEN_F_GRID * 4;
      f_height = GOEN_F_GRID; 

      flow_AddRect( nc, 0, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, f_width, 0, f_width + f_pinlength, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, f_width + f_pinlength, 0,
		0, flow_eDirection_Right);
      if ( graph_index == goen_eGraphIndex_GetAgeneric)
        flow_AddText( nc, "GetA", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else if ( graph_index == goen_eGraphIndex_GetSgeneric)
        flow_AddText( nc, "GetS", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else if ( graph_index == goen_eGraphIndex_GetIgeneric)
        flow_AddText( nc, "GetI", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else
        flow_AddText( nc, "GetD", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      break;
    }
    case goen_eGraphIndex_StoAgeneric:
    case goen_eGraphIndex_StoDgeneric:
    case goen_eGraphIndex_StoSgeneric:
    case goen_eGraphIndex_StoIgeneric:
    {
      f_width = GOEN_F_GRID * 4;
      f_height = GOEN_F_GRID; 

      flow_AddRect( nc, 0, -f_height/2, f_width, f_height, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
      flow_AddLine( nc, -f_pinlength, 0, 0, 0, flow_eDrawType_Line, 2);
      flow_AddConPoint( nc, -f_pinlength, 0,
		0, flow_eDirection_Left);
      if ( graph_index == goen_eGraphIndex_StoAgeneric)
        flow_AddText( nc, "StoA", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else if ( graph_index == goen_eGraphIndex_StoSgeneric)
        flow_AddText( nc, "StoS", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else if ( graph_index == goen_eGraphIndex_StoIgeneric)
        flow_AddText( nc, "StoI", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      else
        flow_AddText( nc, "StoD", f_strlength, 0.5 * f_strheight,
		flow_eDrawType_TextHelveticaBold, GOEN_F_TEXTSIZE);
      break;
    }
  }
  *node_class = nc;
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info()
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
int goen_get_point_info_m4( grectx, graphbody, point, mask, node_width, 
			info_pointer, node)
gre_ctx		 grectx;
pwr_sGraphPlcNode	*graphbody;
unsigned long	 point;
unsigned long 	 *mask;
unsigned long 	 node_width;
goen_conpoint_type	 *info_pointer;
vldh_t_node		 node;
{
#if 0
	int	graph_index;
	float 	center_x;
	float	center_y;
	double	root_ll_x;
	double	root_ll_y;
	double	root_width;
	double	root_height;
	goe_tab_con goe_point_info;
     	int	size;
	float	ll_x,ll_y,mid_x,mid_y;
	int	sts;
	float	f_width, f_height;
	float	delta = 0.004079;
	ldh_tSesContext	 ldhses;

	ldhses = (node->hn.window_pointer)->hw.ldhsession;
 
	/* Get graph index for this class */
	graph_index = graphbody->graphindex;

	sts = NetedMeasureNode( grectx->neted_window,
			  node->hn.node_id,
			  &ll_x,&ll_y,&mid_x,&mid_y,&f_width,&f_height);
	tst_neted( sts, "NetedMeasureNode", 204 );

	goe_measure_node( graph_index, scale, &root_ll_x, &root_ll_y,
				&root_width, &root_height);

	if ( goe_get_connection_point( graph_index, point, scale, 
		&goe_point_info))
	{
	  /* The point exist */
	  info_pointer->x = goe_point_info.loc.x - root_ll_x + delta 
			- f_width / 2;
	  info_pointer->y = f_height / 2 - ( delta + root_height 
			- ( goe_point_info.loc.y - root_ll_y));
	  info_pointer->type = goe_point_info.direction;

	  if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_TRANS))
	    info_pointer->x = goe_point_info.loc.x - root_ll_x + delta * 3.0/2
			- f_width / 2;

	  return GOEN__SUCCESS;
	}
	else
	  /* The point doesn't exist */
	  return GOEN__NOPOINT;
#endif
	info_pointer->x = 0;
	info_pointer->y = 0;
	info_pointer->type = CON_RIGHT;
	return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_parameter_m4()
*
* Type		
*
* Type		Parameter	IOGF	Description
*    pwr_sGraphPlcNode	*graphbody	Pointer to objecttype data
*    unsigned long	point			Connection point nr
*    unsigned long 	*mask			Mask for drawing inputs/outputs
*    unsigned long	*par_type		Input or output parameter	
*
* Description:
*	Gets pointer to parameterdata for connectionpoint.
*
**************************************************************************/
int	goen_get_parameter_m4( graphbody, class, ldhses,
			con_point, mask, par_type, par_inverted, par_index)
pwr_sGraphPlcNode	*graphbody;
pwr_tClassId	    class;
ldh_tSesContext	    ldhses;
unsigned long		con_point;
unsigned long		*mask;
unsigned long		*par_type;
unsigned long		*par_inverted;
unsigned long		*par_index;
{
	ldh_sParDef 	*bodydef;
	int 		rows;
	unsigned long	inputs,interns,outputs;
	int		i, input_found, output_found;
	int		sts;

    	/* Get the runtime paramters for this class */
	sts = ldh_GetObjectBodyDef(ldhses, class, "RtBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) )
	{
      	  /* This object contains only a devbody */
	  sts = ldh_GetObjectBodyDef(ldhses, class, "DevBody", 1, 
			&bodydef, &rows);
	  if ( EVEN(sts) ) return GOEN__NOPOINT;
	}

	inputs = graphbody->parameters[PAR_INPUT];
	interns = graphbody->parameters[PAR_INTERN];
	outputs = graphbody->parameters[PAR_OUTPUT];

        input_found = 0;
        output_found = 0;
	for ( i = 0; i < inputs; i++)
	{
	  if (bodydef[i].Par->Input.Graph.ConPointNr == con_point )
	  {
	    *par_type = PAR_INPUT;
	    *par_index = i;
	    *par_inverted = GOEN_NOT_INVERTED;
	    input_found = 1;
	    break;
	  }
	}
	if ( input_found == 0 )
	{
	  for ( i = inputs + interns; i < inputs + interns + outputs; i++)
	  {
	    if (bodydef[i].Par->Output.Graph.ConPointNr == con_point )
            {
	      *par_type = PAR_OUTPUT;
	      *par_index = i;
	      *par_inverted = GOEN_NOT_INVERTED;
	      output_found = 1;
	      break;
	    }
	  }
	}

	free((char *) bodydef);
	if ( input_found || output_found ) return GOEN__SUCCESS;
	else return GOEN__NOPOINT;
}



/*************************************************************************
*
* Name:		goen_get_location_point_m4()
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
int goen_get_location_point_m4( grectx, graphbody, mask, node_width, 
		info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_point_type	 *info_pointer;
    vldh_t_node		 node;
{
#if 0
	float 	center_x;
	float	center_y;
	double	root_ll_x;
	double	root_ll_y;
	double	root_width;
	double	root_height;
	goe_tab_con goe_point_info;
	int	graph_index;
	float	x_min, y_max;
	int	x_found, y_found;
	int	i;
	float	ll_x,ll_y,mid_x,mid_y;
	int	sts;
	float	f_width, f_height;
	float	delta = 0.004079;
	ldh_tSesContext	 ldhses;

	ldhses = (node->hn.window_pointer)->hw.ldhsession;

	/* Get graph index for this class */
	graph_index = graphbody->graphindex;	
	sts = NetedMeasureNode( grectx->neted_window,
			  node->hn.node_id,
			  &ll_x,&ll_y,&mid_x,&mid_y,&f_width,&f_height);
	tst_neted( sts, "NetedMeasureNode", 204 );

	goe_measure_node( graph_index, scale, &root_ll_x, &root_ll_y,
				&root_width, &root_height);

	y_found = 0;
	x_found = 0;
	y_max = -10000.;
	x_min = 10000.;
	i = 0;
	while ( goe_get_connection_point( graph_index, i, scale, &goe_point_info)
						 != 0 )
	{
	  if ( (goe_point_info.direction == CON_CENTER) ||
		(goe_point_info.direction == CON_LEFT) ||
		(goe_point_info.direction == CON_RIGHT) )
	  {
	    y_max = max( y_max, goe_point_info.loc.y );
	    y_found = 1;
	  }
	  if ( (goe_point_info.direction == CON_CENTER) ||
		(goe_point_info.direction == CON_UP) ||
		(goe_point_info.direction == CON_DOWN) )
	  {
	    x_min = min( x_min, goe_point_info.loc.x );
	    x_found = 1;
	  }
	  i++;
	}
	if ( !y_found )
	  y_max = root_height / 2 + root_ll_y;

	if ( !x_found )
	  x_min = root_width / 2 + root_ll_x;

	info_pointer->x = f_width / 2 - ( x_min - root_ll_x + delta);
	info_pointer->y = f_height / 2  - ( delta + root_height 
			- ( y_max - root_ll_y));

	if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_TRANS))
	  info_pointer->x = f_width / 2 - ( x_min - root_ll_x + delta * 3.0/2);

#endif	
	return GOEN__SUCCESS;
}
