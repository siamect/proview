/* wb_goenm6.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates a nodetype for a document object.
   The mask specifies the format of the node. The document
   header is fetcht from a goe-editor stored graphic object.  */

#include <stdio.h>
#include <math.h>

#include "wb_foe_macros.h"

#include <Xm/Xm.h>

#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#if 0
#include "xnav_bitmap_pro8.h"
#include "xnav_bitmap_pro12.h"
#include "xnav_bitmap_pro16.h"
#endif

/*_Local variables_______________________________________________________*/

static float f_header_sep1 = 9 * GOEN_F_GRID;
static float f_header_sep2 = 17 * GOEN_F_GRID;
static float f_header_sep3 = 21 * GOEN_F_GRID;
static float f_header_width = 25 * GOEN_F_GRID;
static float f_repeat = GOEN_F_GRID;
static float f_yoffs = GOEN_F_GRID/2;
static float f_annot_offs_x = 1.5 * GOEN_F_GRID;
static float f_annot_offs_y = 0.2 * GOEN_F_GRID;
static float 	format[10][2] 	=	{
			{ 5.70, 8.40 },		/* A0  width height */
			{ 4.20, 6.20 },		/* A1  width height */
			{ 3.10, 4.60 },		/* A2  width height */
			{ 2.30, 3.40 },		/* A3  width height */
			{ 1.70, 2.50 },		/* A4  width height */
			{ 1.25, 1.80 },		/* A5  width height */
			{ 0.92, 1.35 },		/* A6  width height */
			{ 0.72, 1.06 },		/* A7  width height */
			{ 0.53, 0.78 },		/* A8  width height */
			};


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		goen_create_nodetype_m6()
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


int goen_create_nodetype_m6( 
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
  float	f_width;
  float	f_height;
  int		graph_index;
  int	size, sts;

  flow_tNodeClass	nc_pid;
  char		name[80];
  static int	idx = 0;
  int		*docsize_p, *docorientation_p;
  int		docsize, docorientation;

  sts = ldh_ClassIdToName(ldhses, class, name, sizeof(name), &size);
  if ( EVEN(sts) ) return sts;
  sprintf( &name[strlen(name)], "%d", idx++);

  /* Get graph index for this class */
  graph_index = graphbody->graphindex;	

  /* Get size and orientation attribute */
  sts = ldh_GetObjectPar(
			(node->hn.window_pointer)->hw.ldhsession,  
			node->ln.object_did, 
			"DevBody",
			"DocumentSize",
			(char **)&docsize_p, &size); 
  if ( EVEN(sts))
    docsize = 3;
  else
  {
    docsize = *docsize_p;
    free( (char *)docsize_p);
  }

  sts = ldh_GetObjectPar(
			(node->hn.window_pointer)->hw.ldhsession,  
			node->ln.object_did, 
			"DevBody",
			"DocumentOrientation",
			(char **)&docorientation_p, &size); 
  if ( EVEN(sts))
    docorientation = 0;
  else
  {
    docorientation = *docorientation_p;
    free( (char *)docorientation_p);
  }

  if ( docorientation == 0 )
  {
    /* vertical format */
    f_width = format[docsize][0];
    f_height = format[docsize][1];
  }
  else
  {
    /* horizontal format */
    f_width = format[docsize][1];
    f_height = format[docsize][0];
  }


  flow_CreateNodeClass(ctx, name, flow_eNodeGroup_Document, 
		&nc_pid);

  /* Draw the lines */
  flow_AddLine( nc_pid, 0, -f_yoffs, f_width, -f_yoffs, flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, f_width, -f_yoffs, f_width, f_height - f_yoffs, 
	flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, f_width, f_height - f_yoffs, 0, f_height - f_yoffs, 
	flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 0, f_height - f_yoffs, 0, -f_yoffs, flow_eDrawType_Line, 2);

  /* Draw the header */
  flow_AddRect( nc_pid, f_width - f_header_width, 
		f_height - 3 * f_repeat - f_yoffs, f_header_width, 3 * f_repeat, 
		flow_eDrawType_Line, 2, flow_mDisplayLevel_1);
  flow_AddLine( nc_pid, 
		f_width - f_header_width + f_header_sep1, 
		f_height - 2 * f_repeat - f_yoffs,
		f_width - f_header_width + f_header_sep1,
		f_height - f_yoffs, 
		flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 
		f_width - f_header_width + f_header_sep2, 
		f_height - 2* f_repeat - f_yoffs,
		f_width - f_header_width + f_header_sep2, 
		f_height - f_yoffs,
		flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 
		f_width - f_header_width + f_header_sep3, 
		f_height - f_repeat - f_yoffs,
		f_width - f_header_width + f_header_sep3, 
		f_height - f_yoffs,
		flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 
		f_width - f_header_width, 
		f_height - 2 * f_repeat - f_yoffs,
		f_width,
		f_height - 2 * f_repeat - f_yoffs,
		flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 
		f_width - f_header_width, 
		f_height - f_repeat - f_yoffs,
		f_width - f_header_width + f_header_sep1,
		f_height - f_repeat - f_yoffs,
		flow_eDrawType_Line, 2);
  flow_AddLine( nc_pid, 
		f_width - f_header_width + f_header_sep2, 
		f_height - f_repeat - f_yoffs,
		f_width,
		f_height - f_repeat - f_yoffs,
		flow_eDrawType_Line, 2);
  flow_AddAnnot( nc_pid, 
		f_width - f_header_width + f_annot_offs_x,
		f_height - 2 * f_repeat - f_annot_offs_y - f_yoffs,
		1, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine, 
		flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid, 
		f_width - f_header_width + f_annot_offs_x,
		f_height - f_repeat - f_annot_offs_y - f_yoffs,
		2, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine, 
		flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid, 
		f_width - f_header_width + f_annot_offs_x,
		f_height - f_annot_offs_y - f_yoffs,
		4, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine,
		flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid,
		f_width - f_header_width + f_header_sep2 + f_repeat/2,
		f_height - f_repeat - f_annot_offs_y - f_yoffs,
		0, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine, 
		flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid, 
		f_width - f_header_width + f_header_sep2 + f_repeat/2,
		f_height - f_annot_offs_y - f_yoffs,
		5, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine, 
		flow_mDisplayLevel_1);
  flow_AddAnnot( nc_pid, 
		f_width - f_header_width + f_header_sep3 + f_repeat/2,
		f_height - f_annot_offs_y - f_yoffs,
		3, flow_eDrawType_TextHelvetica, 3, flow_eAnnotType_OneLine, 
		flow_mDisplayLevel_1);
  if ( graph_index != 1)
    flow_AddText( nc_pid, "Proview",
		f_width - f_header_width + f_header_sep1 + f_repeat, 
		f_height - f_repeat /2 - f_yoffs,
		flow_eDrawType_TextHelveticaBold, 9);
  else
  {

    flow_AddText( nc_pid, "SSAB",
		f_width - f_header_width + f_header_sep1 + f_repeat, 
		f_height - f_repeat * 3/4 - f_yoffs,
		flow_eDrawType_TextHelveticaBold, 9);
    flow_AddText( nc_pid, "Oxelösund",
		f_width - f_header_width + f_header_sep1 + f_repeat * 1.1, 
		f_height - f_repeat /4 - f_yoffs,
		flow_eDrawType_TextHelveticaBold, 1);

#if 0
    int 	i;
    flow_tObject pix;    
    flow_sPixmapData pixmap_data;
    for ( i = 0; i < DRAW_PIXMAP_SIZE; i++)
    {
      if ( i < 2)
      {
        pixmap_data[i].width = xnav_bitmap_pro8_width;
        pixmap_data[i].height = xnav_bitmap_pro8_height;
        pixmap_data[i].bits = xnav_bitmap_pro8_bits;
      }
      else if ( i < 4)
      {
        pixmap_data[i].width = xnav_bitmap_pro12_width;
        pixmap_data[i].height = xnav_bitmap_pro12_height;
        pixmap_data[i].bits = xnav_bitmap_pro12_bits;
      }
      else
      {
        pixmap_data[i].width = xnav_bitmap_pro16_width;
        pixmap_data[i].height = xnav_bitmap_pro16_height;
        pixmap_data[i].bits = xnav_bitmap_pro16_bits;
      }
    }
    flow_CreatePixmap( ctx, &pixmap_data, 
		f_width - f_header_width + f_header_sep1 + f_repeat * 5.75, 
		f_height - f_repeat * 2 - f_yoffs,
		flow_eDrawType_Line, 4,
                &pix);
    flow_NodeClassAdd( nc_pid, pix);
#endif
  }

  *node_class = nc_pid;
  return GOEN__SUCCESS;
}



/*************************************************************************
*
* Name:		goen_get_point_info_m6()
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
int goen_get_point_info_m6( grectx, graphbody, point, mask, node_width, 
	info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long	 point;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_conpoint_type	 *info_pointer;
    vldh_t_node		 node;
{
	   info_pointer->x = 0;
	   info_pointer->y = 0;
	   info_pointer->type = CON_RIGHT;
	   return GOEN__NOPOINT;
}



/*************************************************************************
*
* Name:		goen_get_parameter_m6()
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
int	goen_get_parameter_m6( graphbody, class, ldhses,
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
	return GOEN__NOPOINT;
}


/*************************************************************************
*
* Name:		goen_get_location_point_m6()
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
int goen_get_location_point_m6( grectx, graphbody, mask, node_width, 
		info_pointer, node)
    gre_ctx		 grectx;
    pwr_sGraphPlcNode	*graphbody;
    unsigned long 	 *mask;
    unsigned long	 node_width;
    goen_point_type	 *info_pointer;
    vldh_t_node		 node;
{
	unsigned long	node_mask[2];
	float	f_width;
	float	f_height;


	node_mask[0] = *mask++;
	node_mask[1] = *mask;
	if ( (node_mask[1] & 1) == 0 )
	{
	  /* vertical format */
	  f_width = format[ node_mask[0] ][0];
	  f_height = format[ node_mask[0] ][1];
	}
	else
	{
	  /* horizontal format */
	  f_width = format[ node_mask[0] ][1];
	  f_height = format[ node_mask[0] ][0];
	}

	info_pointer->y = - f_height / 2 + 0.025 +  
			0.1 * floor( f_height / 2 / 0.1);
	info_pointer->x = - f_width / 2 + 0.1 * floor( f_width / 2 / 0.1);

	return GOEN__SUCCESS;
}
