/* wb_gre.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates the neted window and handles
   the neted callbacks.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if defined OS_VMS
#include <processes.h>
#include <descrip.h>
#include <lib$routines.h>
#include <clidef.h>
#include <ssdef.h>
#include <cdadef.h>
#endif

#include "wb_foe_macros.h"

#include <Xm/Xm.h>
#if defined OS_VMS
#include <Xm/DECspecific.h>
#endif
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/DialogS.h>

#if defined OS_VMS
#include <sys$library/DECw$Cursor.h>
#include <Xm/cursorfont.h>
#endif

#include "pwr_baseclasses.h"
#include "flow.h"
#include "flow_api.h"
#include "flow_widget.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_goec.h"
#include "wb_goen.h"
#include "wb_utl.h"
#include "wb_gre.h"
#include "wb_tra.h"


#define ANNOT_LEN_MAX 	1024
#define ANNOT_NR_MAX 	8

#if 0
#define	CLOCK_CURSOR	XDefineCursor( XtDisplay( grectx->work_area),\
		XtWindow( grectx->work_area),\
		grectx->cursors[ GOEN_CURSOR_CLOCK ]); \
		XFlush(XtDisplay(grectx->work_area));

#define	NORMAL_CURSOR	XUndefineCursor( XtDisplay( grectx->work_area),\
		XtWindow( grectx->work_area));


#define	DEFERRED_UPDATE	CLOCK_CURSOR \
		GobeSetDisplayUpdate(grectx->neted_window,\
		GobeKupdateDeferred);\


#define	UPDATE_SCREEN	GobeSetDisplayUpdate( grectx->neted_window,\
				GobeKupdateImmediate);\
			NORMAL_CURSOR
#endif

#define	CLOCK_CURSOR	

#define	NORMAL_CURSOR

#define	DEFERRED_UPDATE	

#define	UPDATE_SCREEN	
				
#define	BEEP	    putchar( '\7' );

#ifdef VAXC
#define GRE_MASKTST(cb, func) ((( cb->button == grectx->func/**/_mask) ||\
	( cb->button == (grectx->func/**/_mask | LockMask ))) &&\
	( cb->type == grectx->func/**/_click))
#else
#define GRE_MASKTST(cb, func) ((( cb->button == grectx->func##_mask) ||\
	( cb->button == (grectx->func##_mask | LockMask ))) &&\
	( cb->type == grectx->func##_click))
#endif


#define GRE_SEARCHRECT_OFFSET 0.01

/*_Global variable______________________________________________________*/

/*_Global procedues_______________________________________________________*/

/*_Local procedues_______________________________________________________*/

static int gre_init_flow( FlowCtx *ctx, void *client_data);
static int gre_get_nodeclass (
	gre_ctx	grectx,
	pwr_tClassId  class,
	ldh_tSesContext	    ldhses,
	unsigned long	node_type,
	unsigned int	*mask,
	unsigned long	subwindowmark,
	unsigned long	node_width,
	flow_tNodeClass	*node_type_id,
	vldh_t_node	node
);

static void	gre_delnode_insert(
	gre_ctx		grectx,
	vldh_t_node	node_id
);


static void gre_message( 
    gre_ctx		grectx,
    char		*message
);

static pwr_tStatus	gre_node_update_points( 
	vldh_t_node	node,
	unsigned long	old_input_mask, 
	unsigned long	old_output_mask,
	unsigned long	new_input_mask,
	unsigned long	new_output_mask,
	int		*point_array,
	int		*point_count
);

#if defined OS_VMS
static int	gre_spawn( char *command);
#endif










/*************************************************************************
*
* Name:		gre_node_sethighlight
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I 	neted node.
* unsigned long	highlight_flag	I	set (1) or reset (0) of highlight.
*
* Description:
*	Set or reset highlight on a node depending on the flag 
*	( set: 1, reset: 0).
*
**************************************************************************/

int gre_node_sethighlight ( 
	gre_ctx		grectx,
	vldh_t_node	node,
	unsigned long	highlight_flag
)
{
	flow_SetHighlight( node->hn.node_id, highlight_flag);
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_node_gethighlight
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I 	neted node.
* unsigned long	*highlight_flag	I	highlight value: set (1) or reset (0)
*
* Description:
*	Get the highlight value of a node 
*	( set: 1, reset: 0).
*
**************************************************************************/

int gre_node_gethighlight (	
	gre_ctx		grectx,
	vldh_t_node	node,
	unsigned long	*highlight_flag
)
{
	flow_GetHighlight( node->hn.node_id, (int *) &highlight_flag);
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_get_annot_width()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I 	neted node.
*
* Description:
*	Get the annotations that should be written in the graphics 
*	for a node.
*
**************************************************************************/

int gre_get_annot_width(
		flow_tNodeClass	nodeclass,
		float		*annot_width,
		char		*annot_str,
		int		annot_count,
		int		annot_size
		)
{
	int			i;
	double			width, height;

	for ( i = 0; i < annot_count; i++)
	{
          flow_MeasureAnnotation( nodeclass, i, annot_str, 
		&width, &height);
	  *annot_width = width;
	  annot_str += annot_size;
	  annot_width++;
	}
	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_get_annotations()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I 	neted node.
*
* Description:
*	Get the annotations that should be written in the graphics 
*	for a node.
*
**************************************************************************/

int gre_get_annotations(
		vldh_t_node	node,
		char		*annot_str,
	  	int		*annot_nr,
		int		*annot_count,
		int		annot_max,
		int		annot_size
		)
{
	int			i, j, sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	ldh_sParDef 		*bodydef;
	int 			rows;
	int			annotnr, type;
	char			*parvalue;
	pwr_tObjid		*parobjdid;
	pwr_sAttrRef		*parattrref;
	char			*parname;
	int			annotcond;
	int			annotsegments;
	char			condparname[36];
	pwr_tObjid		last_objid;
	pwr_tBoolean		*condparvalue;
	pwr_tBoolean		*segmentsparvalue;
	int			annot_segments;
	char			objid_str[120];
	char			*name;
	char			body[10];
	ldh_tSesContext		ldhses;
        pwr_tObjid		nullobjid = pwr_cNObjid;

	ldhses = (node->hn.window_pointer)->hw.ldhsession;
	*annot_count = 0;
	last_objid = pwr_cNObjid;

	/* Get graphbody for this node to fetch the parameter of the 
	   first annoation */
	sts = ldh_GetClassBody( ldhses,
			node->ln.classid, "GraphPlcNode",
			&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts)) return sts;

	node->ln.nodewidth = 0;
	if ( graphbody->segname_annotation >= 1 )
	{
	  strncpy( annot_str, node->hn.name, annot_size);
	  *annot_nr = graphbody->segname_annotation;
	  annot_str += annot_size;
	  annot_nr++;
	  (*annot_count)++;
	}

	if (node->ln.classid != vldh_class( ldhses, VLDH_CLASS_ORDER))
	{
	  for ( j = 0; j < 2; j++)
          {
	    /* Look for annotations in some parameter in devbody or rtbody */
            if ( j == 0)
	    {
	      if ( !graphbody->devbody_annotation)
 	        continue;
	      strcpy( body, "DevBody");
	    }
	    else
	    {
	      if ( !graphbody->rtbody_annotation)
 	        continue;
	      strcpy( body, "RtBody");
	    }

	    sts = ldh_GetObjectBodyDef( ldhses,
			node->ln.classid, body, 1, 
			&bodydef, &rows);
	    if ( EVEN(sts)) continue;

	    if ( EVEN(sts) ) return GRE__SUCCESS;

	    for ( i = 0; i < rows; i++)
	    {
	      annotnr = 0;
	      switch ( bodydef[i].ParClass )
	      {
	        case pwr_eClass_Input:
	        {
	          annotnr = bodydef[i].Par->Input.Graph.NiNaAnnot;
	          annotcond = bodydef[i].Par->Input.Graph.NiNaCond;
	          annotsegments = bodydef[i].Par->Input.Graph.NiNaSegments;
	          type = bodydef[i].Par->Input.Info.Type;
	          parname = bodydef[i].ParName;
	          break;
	        }
	        case pwr_eClass_Intern:
	        {
	          annotnr = bodydef[i].Par->Intern.Graph.NiNaAnnot;
	          annotcond = bodydef[i].Par->Intern.Graph.NiNaCond;
	          annotsegments = bodydef[i].Par->Intern.Graph.NiNaSegments;
	          type = bodydef[i].Par->Intern.Info.Type;
	          parname = bodydef[i].ParName;
	          break;
	        }
	        case pwr_eClass_Output:
	        {
	          annotnr = bodydef[i].Par->Output.Graph.NiNaAnnot;
	          annotcond = bodydef[i].Par->Output.Graph.NiNaCond;
	          annotsegments = bodydef[i].Par->Output.Graph.NiNaSegments;
	          type = bodydef[i].Par->Output.Info.Type;
	          parname = bodydef[i].ParName;
	          break;
	        }
                default:
                  ;
 	      }
              if( annotnr != 0 )
	      {
	        if ( annotcond)
	        {
	   	  /* Get the condition if the annotation should be shown from
		   the parameter Show'parname' in devbody */
		  strcpy( condparname, "Show");
		  strcat( condparname, parname);
	          sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			"DevBody",
			condparname,
			(char **)&condparvalue, &size);
		  if ( EVEN(sts))
		    continue;
	          if ( !*condparvalue)
	          {
		    /* No, this parameter should node be shown this time */
		    free((char *) condparvalue);
	            continue;
	          }
		  free((char *) condparvalue);
	        }

	        if ( !annotsegments)
		  annot_segments = 1;
	        else
	        {
	 	  /* Get the segments of the annotation that should be shown from
		     the parameter 'parname'Segments in devbody */
		  strcpy( condparname, parname);
		  strcat( condparname, "Segments");
	          sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			"DevBody",
			condparname,
			(char **)&segmentsparvalue, &size);
		  if ( EVEN(sts))
		    annot_segments = 1;
	          else
	          {
		    annot_segments = *segmentsparvalue;
		    if ( annot_segments < 1)
		      annot_segments = 1;
		    free((char *) segmentsparvalue);
	          }
	        }

	        if ( !strcmp( parname, "SigChanCon"))
	        {
	          /* Get attribute SigChanCon from previous objid attribute */
	          if ( cdh_ObjidIsNotNull( last_objid))
	          {
	            sts = ldh_GetObjectPar( ldhses,
			last_objid,
			"RtBody",
			bodydef[i].ParName,
			(char **)&parvalue, &size);
	            if ( EVEN(sts))
	              parvalue = (char *) &nullobjid;
	          }
	          else
	            parvalue = (char *) &nullobjid;
	        }
	        else
	        {
	          /* Get the parameter value */
	          sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			body,
			bodydef[i].ParName,
			(char **)&parvalue, &size);
	          if ( EVEN(sts)) return sts;
	        }
	        if ( type == pwr_eType_AttrRef)
	        {
	          /* Get the value of the referenced attribute */
	 	  parattrref = (pwr_sAttrRef *)parvalue;
		  if ( cdh_ObjidIsNull( parattrref->Objid))
	            strcpy( annot_str, "");
	          else
	          {
		    sts = ldh_StringGetAttribute( ldhses, parattrref,
				annot_size, annot_str);
		    if ( EVEN(sts)) 
	              strcpy( annot_str, "");
	          }
	        }
	        else
	        {
	          switch ( type )
	          {
	            case pwr_eType_String:
	            case pwr_eType_Text:
	            {
	              strncpy( annot_str, parvalue, annot_size);
	              break;
	            }
	            case pwr_eType_Char:
	            {
	  	      *annot_str = *parvalue;
		      *(annot_str + 1) = '\0';		
	              break;
	            }
	            case pwr_eType_Float32:
	            {
	  	      sprintf( annot_str, "%g", *(float *)parvalue);
	              break;
	            }
	            case pwr_eType_ObjDId:
	            {
 	              /* Get the object name from ldh */
	 	      parobjdid = (pwr_tObjid *)parvalue;
		      if ( cdh_ObjidIsNull(*parobjdid))
	                strcpy( objid_str, "");
		      else if ( (*parobjdid).oix == 0 )
	              {
	                /* Get the volume name */
	                sts = ldh_VolumeIdToName( ldh_SessionToWB(ldhses),
 			  (*parobjdid).vid, objid_str, sizeof( objid_str),
			  &size);
	                if ( EVEN(sts))
		          objid_str[0] = 0;
	              }
	              else
	              {
	                if ( annot_segments == 1)
	                {
	                  sts = ldh_ObjidToName( ldhses,
	         		*parobjdid, ldh_eName_Object,
		        	objid_str, sizeof(objid_str), &size);
	                  if ( EVEN(sts)) 
		            objid_str[0] = 0;
	                }
		        else
		        {
	                  sts = ldh_ObjidToName( ldhses,
	         		*parobjdid, ldh_eName_Hierarchy,
		        	objid_str, sizeof(objid_str), &size);
	                  if ( EVEN(sts)) 
		            objid_str[0] = 0;
		          else
	                    utl_cut_segments( objid_str, objid_str, annot_segments);
	                }
		      }
		      strncpy( annot_str, objid_str, annot_size);
		      last_objid = *parobjdid;
	              break;
	            }
	            case pwr_eType_AttrRef:
	            {
 	              /* Get the object name from ldh */
	 	      parattrref = (pwr_sAttrRef *)parvalue;
		      if ( cdh_ObjidIsNull(parattrref->Objid))
	                strcpy( objid_str, "");
	              else
	              {
	                if ( annot_segments == 1)
	                {
	                  sts = ldh_AttrRefToName( ldhses,
	         		parattrref, ldh_eName_Object,
		        	&name, &size);
	                  if ( EVEN(sts)) 
		            objid_str[0] = 0;
	                  else
	                    strcpy( objid_str, name);
	                }
		        else
		        {
	                  sts = ldh_AttrRefToName( ldhses,
	         		parattrref, ldh_eName_Hierarchy,
		        	&name, &size);
	                  if ( EVEN(sts)) 
		            objid_str[0] = 0;
		          else
	                    utl_cut_segments( objid_str, name, annot_segments);
	                }
		      }
		      strncpy( annot_str, objid_str, annot_size);
	              break;
	            }
	          }
	        }
	        *annot_nr = annotnr;
	        annot_str += annot_size;
	        annot_nr++;
	        (*annot_count)++;
	        if ( *annot_count >= annot_max)
	          break;
	        if ( parvalue != (char *) &nullobjid)
	          free((char *) parvalue);
	      }
	    }
	    free((char *) bodydef);	
	    if ( *annot_count >= annot_max)
	      break;
	  }
	}
	else
	{
	  char		attribute[80];
	  int 		attribute_count;
	  pwr_tFloat32	*attrtime_p;
	  char		*annot_str_next;
	  char		showattrtime;

	  /* Orderobjects are special as usual... */
	  /* Get the Order attributes */
	  attribute_count = 0;
	  strcpy( annot_str, "");
	  annot_str_next = annot_str + annot_size;
	  strcpy( annot_str_next, "");
	  /* Check if AttrTime's should be shown */
	  sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			"DevBody",
			"ShowAttrTime",
			(char **)&condparvalue, &size);
	  if ( EVEN(sts)) return sts;
	  showattrtime = *condparvalue;
	  free((char *) condparvalue);

	  for ( i = 0; i < 6; i++)
	  {
	    sprintf( attribute, "Attr%d", i + 1);
	    sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			"DevBody",
			attribute,
			(char **)&parvalue, &size);
	    if ( EVEN(sts)) return sts;
	    sprintf( annot_str + strlen(annot_str), "%c", *parvalue);
	    if ( showattrtime && 
		((*parvalue == 'D' || *parvalue == 'd') ||
	         (*parvalue == 'L' || *parvalue == 'l')))
	    {
	      sprintf( attribute, "AttrTime%d", i + 1);
	      
	      if ( attribute_count != 0)
	        strcat( annot_str_next, " ");
	      sprintf( annot_str_next + strlen(annot_str_next), "%c=", *parvalue);
	      sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did,
			"DevBody",
			attribute,
			(char **)&attrtime_p, &size);
	      if ( EVEN(sts)) return sts;

	      sprintf( annot_str_next + strlen(annot_str_next), "%g", *attrtime_p);
	      attribute_count++;
	      free((char *) attrtime_p);
	    }
	    free((char *) parvalue);
	  }
	  if ( attribute_count == 1)
	    /* Show only the time, not the attribute type */
	    strcpy( annot_str_next, annot_str_next + 2);
	  *annot_nr = 2;
	  annot_nr++;
	  (*annot_count)++;
	  *annot_nr = 3;
	  (*annot_count)++;
	}
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_message()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* char		*message	I	string of message.
*
* Description:
*	Send a message to the gre_message backcallroutine.
*	( foe_gre_message )
*
**************************************************************************/

static void gre_message( 
    gre_ctx		grectx,
    char		*message
)
{
	(grectx->gre_message) (grectx, message);
}


/*************************************************************************
*
* Name:		gre_ctx_init()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Inits the gre context.
*
*  SG 23.02.91 init of the timers which helps to detect double click and click
*  events .
*
**************************************************************************/

void	gre_ctx_init( grectx)
gre_ctx		grectx;
{
	int	i,j;
	
	for ( i = 0; i < MAX_NUMBER_OF_OBJECTTYPES; i++ )	
	  for ( j = 0; j < 4; j++ )	
	    grectx->nodeclass_list[i][j] = 0;

	for ( i = 0; i < MAX_NUMBER_OF_CONTYPES; i++ )	
	  grectx->conclass_list[i] = 0;
 
	grectx->sel_node_count = 0;
	grectx->sel_con_count = 0;
	grectx->grid_on = 0;
	grectx->conref_nodetypeid = 0;
	grectx->concreate_mask =0;
	grectx->nodemoved_mask =0;
	grectx->regionsel_mask =0;
	grectx->nodecreate_mask =0;
	grectx->nodesel_mask = 0;
	grectx->nodeseladd_mask = 0;
	grectx->delete_mask = 0;
	grectx->cut_mask = 0;
	grectx->copy_mask = 0;
	grectx->paste_mask = 0;
	grectx->attribute_mask = 0;
	grectx->subwindow_mask = 0;
	grectx->reserv_mask = 0;
	grectx->popupmenu_mask = 0;
	grectx->getobj_mask = 0;
	grectx->help_mask = 0;
}


/*************************************************************************
*
* Name:		gre_modif_annot 
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
* int		annotation	I	neted annotation nr.
* char		*str		I	annotation string.
*
* Description:
* 	Draw one annotation in a node.
*
**************************************************************************/

int gre_modif_annot ( 
	gre_ctx		grectx,
	vldh_t_node	node,
	int		annotation,
	char		*str
)
{

  flow_SetAnnotation( node->hn.node_id, annotation, str, strlen(str));
  return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_node_annotations_draw()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Draw the annotations in a node.
*	If segname_annotation (in graphbody) is not zero the segment name 
*	of the object is written in the annotation with nr: 
*	segname_annotation-1.
*	Every parameter in devbody that has a NiNaAnnot defined is also
*	written in the annotation with nr NiNaAnnot-1. The parameter can
*	be of type float, string, char or objdid.
*
**************************************************************************/

int	gre_node_annotations_draw (
	gre_ctx		grectx,
	vldh_t_node	node,
	int		paste
)
{
	int		i, sts;
	char		annot_str[ANNOT_NR_MAX][ANNOT_LEN_MAX];
	int		annot_nr[ANNOT_NR_MAX];
	int		annot_count;

	/* Get the annotations of this node */
	sts = gre_get_annotations( node,
		(char *)annot_str, annot_nr, &annot_count,
		XtNumber(annot_str), sizeof( annot_str[0]));
	if ( EVEN(sts)) return sts;

	/* Draw the annotations */
	for ( i = 0; i < annot_count; i++)
	{
	  if ( paste)
            flow_SetPasteNodeAnnotation( node->hn.node_id, annot_nr[i] - 1,
		annot_str[i], strlen(annot_str[i]));
          else 
            flow_SetAnnotation( node->hn.node_id, annot_nr[i] - 1,
		annot_str[i], strlen(annot_str[i]));
	}
	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_node_annot_message()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
* char		*message	O	the three first annotations
*					separated by a ','.
*
* Description:
*	This routine is used to get info of the annotations content of 
*	the annotaions a node.
*	Returns the three first annotaions ( the objectname is excluded)
*	and puts them in a string separated by a ','.
*
**************************************************************************/

int	gre_node_annot_message( 
	gre_ctx		grectx,
	vldh_t_node	node,
	char		*message,
	int		msg_size,
	int		annot_max_size
)
{
	int			i, sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	ldh_sParDef 	*bodydef;
	int 		rows;
	int	annotnr, type;
	char	annot_str[ANNOT_LEN_MAX + 80];
	char	*parvalue;
	pwr_tObjid	*parobjdid;
	char	*parname;
	float	*parfloat;
	int	annotcount;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody( (node->hn.window_pointer)->hw.ldhsession, 
                          node->ln.classid, "GraphPlcNode",
                          &bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts)) return sts;

	/* Draw the parameters in devbody that has an annotation nr */
	sts = ldh_GetObjectBodyDef((node->hn.window_pointer)->hw.ldhsession,
                             node->ln.classid, "DevBody", 1, 
                             &bodydef, &rows);

	if ( EVEN(sts) ) return GRE__SUCCESS;

	*message = '\0';
	annotcount = 0;
	for ( i = 0; i < rows; i++)
    {
      strcpy( annot_str, "");
      annotnr = 0;
      switch ( bodydef[i].ParClass )
        {
        case pwr_eClass_Input:
        {
          annotnr = bodydef[i].Par->Input.Graph.NiNaAnnot;
          type = bodydef[i].Par->Input.Info.Type;
          parname = bodydef[i].ParName;
          break;
        }
        case pwr_eClass_Intern:
        {
          annotnr = bodydef[i].Par->Intern.Graph.NiNaAnnot;
          type = bodydef[i].Par->Intern.Info.Type;
          parname = bodydef[i].ParName;
          break;
        }
        case pwr_eClass_Output:
        {
          annotnr = bodydef[i].Par->Output.Graph.NiNaAnnot;
          type = bodydef[i].Par->Output.Info.Type;
          parname = bodydef[i].ParName;
          break;
        }
        default:
          ;
        }
      if( annotnr != 0 )
        {
          /* Get the parameter value */
          sts = ldh_GetObjectPar( (node->hn.window_pointer)->hw.ldhsession,  
                                  node->ln.object_did, 
                                  "DevBody",
                                  bodydef[i].ParName,
                                  (char **)&parvalue, &size); 
          if ( EVEN(sts)) return sts;

          switch ( type )
            {
            case pwr_eType_Float32:
            {
              parfloat = (pwr_tFloat32 *)parvalue;
              sprintf( annot_str,"%f", *parfloat);
              break;
            }
            case pwr_eType_String:
            case pwr_eType_Text:
            {
              strncpy( annot_str, parvalue, 
                       MIN( sizeof(annot_str), annot_max_size));
              annot_str[MIN( sizeof(annot_str), annot_max_size)-1] = 0;
              break;
            }
            case pwr_eType_Char:
            {
              annot_str[0] = *parvalue;
              annot_str[1] = '\0';		
              break;
            }
            case pwr_eType_ObjDId:
            {
              /* Get the object name from ldh */
              parobjdid = (pwr_tObjid *)parvalue;
              if ( cdh_ObjidIsNull(*parobjdid))
                annot_str[0] = '\0';
              else
                {
                  sts = ldh_ObjidToName( (node->hn.window_pointer)->hw.ldhsession, 
                                         *parobjdid, ldh_eName_Object,
                                         annot_str, sizeof( annot_str), &size);
                  if ( EVEN(sts)) annot_str[0] = '\0';
                }
              break;
            }
            }
          if ( strcmp( annot_str, "") != 0)
            {
              if (annotcount != 0)
                strncat( message, ", ", msg_size-strlen(message));
              strncat( message, annot_str, msg_size-strlen(message));
              annotcount++;
            }
          free((char *) parvalue);	
          if ( annotcount >= 3 ) break;
        }
    }
	free((char *) bodydef);

	message[msg_size-1] = 0;
	return GRE__SUCCESS;
}

#if 0
/*************************************************************************
*
* Name:		gre_grid()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		x		I	x koordinate.
* float		y		I	y koordinate.
* float		*grid_x		O	x grid koordinate.
* float		*grid_y		O	y grid koordinate.
*
* Description:
*	Returns the koordinates of the closest gridpoint.
*
**************************************************************************/

static void	gre_grid( 
	gre_ctx		grectx,
	float		x,
	float		y,
	float		*grid_x,
	float		*grid_y
)
{
      *grid_x = (grectx->grid_size * floor( x / grectx->grid_size + 0.5));
      *grid_y = (grectx->grid_size * floor( y / grectx->grid_size + 0.5));
}
#endif


/*************************************************************************
*
* Name:		gre_zoom_absolute()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		absolute_zoom	I	absolute zoom.
* int		expand		I	expand scroll region if needed.
* float		*realized_zoom	O	realized zoom factor.
*
* Description:
*	Zooms to a specified workingarea per screenmillimeter and
*	returns actual zoomfactor.
*
**************************************************************************/

int	gre_zoom_absolute(
	gre_ctx		grectx,
	float		absolute_zoom,
	int		expand,
	float		*realized_zoom
)
{
	flow_ZoomAbsolute( grectx->flow_ctx, absolute_zoom);
	*realized_zoom = absolute_zoom;

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_unselect()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Resets permanent highlight on selected nodes and connections, and
*	resets the selectlists.
* 	Informs the user with a gre_message if there is nothing to unselect.
*
**************************************************************************/

void	gre_unselect(
    gre_ctx		grectx
)
{
	flow_SelectClear( grectx->flow_ctx);
	grectx->sel_node_count = 0;
	grectx->sel_con_count = 0;
}



/*************************************************************************
*
* Name:		gre_search_rectangle_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Delete the search rectangle.
*
**************************************************************************/

void	gre_search_rectangle_delete( gre_ctx	grectx)
{
    if ( grectx->searchrect_node_id != 0 )
    {
      flow_SetInverse( grectx->searchrect_node_id, 0);
      grectx->searchrect_node_id = 0;
    }
}

/*************************************************************************
*
* Name:		gre_search_rectangle_create()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		x		I	x koordinate for lower left.
* float		y		I	y koordinate for lower left.
* float		width		I	widht of the rectangle.
* float		height		I	height of the rectangle.
*
* Description:
*	Create rectangle in the neted window with the lower left
*	koordinates x,y and the specified width and height.
*	Returns neted node id for the created rectangle.
*
**************************************************************************/

void	gre_search_rectangle_create(
	gre_ctx		grectx,
	vldh_t_node	node
)
{
	flow_SetInverse( node->hn.node_id, 1);
	grectx->searchrect_node_id = node->hn.node_id;
}


/*************************************************************************
*
* Name:		gre_create_colors()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Load colors, font and cursor for the neted widget.
*	Inits the goe graph_table.
*
**************************************************************************/

static void	gre_create_cursors(
	gre_ctx		grectx
)
{
    /* Create colors and fonts for this widget */
    goen_create_cursors( grectx->flow_widget, &(grectx->cursors));
}



/*************************************************************************
*
* Name:		gre_undelete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Undeletes all nodes and connections in delete list.
* 	Informs the user by gre_message if there is nothing to undelete
*
**************************************************************************/

int	gre_undelete( 
	gre_ctx		grectx
)
{
	int	i;
	int	sts;
	flow_tNodeClass	node_class;
	flow_tNode	node_id;

        /* SG 20.03.91 if nothing has been done say it to the user */
	if ( grectx->del_con_count == 0 && grectx->del_node_count == 0 )
	{
	  gre_message ( grectx , "Nothing to undelete" );
	  BEEP;
	  return GRE__SUCCESS;
	}


	/* Restore nodes in delete_list */
	for ( i = 0; i < grectx->del_node_count; i++ )
	{
	  sts = gre_get_nodeclass( grectx, 
		(grectx->del_node_list[i])->ln.classid, 
		((grectx->del_node_list[i])->hn.window_pointer)->hw.ldhsession, 
		(grectx->del_node_list[i])->ln.object_type, 
		(grectx->del_node_list[i])->ln.mask, 
		(grectx->del_node_list[i])->ln.subwindow,
		(grectx->del_node_list[i])->ln.nodewidth,
		&node_class, grectx->del_node_list[i]);
	  if( EVEN(sts) ) return sts;
          flow_CreateNode( grectx->flow_ctx, grectx->del_node_list[i]->hn.name, 
		node_class, grectx->del_node_list[i]->ln.x,
		grectx->del_node_list[i]->ln.y, 
		grectx->del_node_list[i], 
		&node_id);
	  (grectx->del_node_list[i])->hn.node_id = node_id;
	  sts = gre_node_annotations_draw( grectx, grectx->del_node_list[i], 0); 
	  if( EVEN(sts) ) return sts;
	  vldh_node_undelete(grectx->del_node_list[i]);
	}
	/* Restore connections in delete_list */
	for ( i = 0; i < grectx->del_con_count; i++ )
	{
	  vldh_con_undelete(grectx->del_con_list[i]);
	  sts = goec_con_draw( grectx, grectx->del_con_list[i], GRE_CON_CREATE,
		0);
	}

	grectx->del_node_count = 0;
	grectx->del_con_count = 0;
	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_undelete_reset()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Resets the undelete list.
*
**************************************************************************/

int	gre_undelete_reset( 
	gre_ctx	    grectx
)
{
	grectx->del_node_count = 0;
	grectx->del_con_count = 0;
	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_delnode_insert()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node_id		I	vldh node.
*
* Description:
*	Insert node in delete node list.
*  	The delete node list contains all deleted nodes in the last
*	delete or cut operation.
*
**************************************************************************/

static void	gre_delnode_insert(
	gre_ctx		grectx,
	vldh_t_node	node_id
)
{
	int	i;
	int	found;

	/* Check not aready inserted */
	found = 0;
	for ( i = 0; i < grectx->del_node_count; i++)
	{
	  if ( grectx->del_node_list[i] == node_id )
	  {
	    found = 1;
	    break;
	  }
	}
	if ( !found && (grectx->del_node_count < MAX_DEL_NODE)) 
	/* Insert node in selected node list */
	{
	  grectx->del_node_list[grectx->del_node_count] = node_id;
	  grectx->del_node_count++;
	}
}


/*************************************************************************
*
* Name:		gre_delcon_insert()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_id		I	vldh connection.
*
* Description:
*	Insert connection in delete connection list.
*  	The delete connection list contains all deleted connections in the last
*	delete or cut operation.
*
**************************************************************************/
static void	gre_delcon_insert( 
	gre_ctx		grectx,
	vldh_t_con	con_id
)
{
	int	i;
	int	found;

	/* Check not aready inserted */
	found = 0;
	for ( i = 0; i < grectx->del_con_count; i++)
	{
	  if ( grectx->del_con_list[i] == con_id )
	  {
	    found = 1;
	    break;
	  }
	}
	if ( !found && (grectx->del_con_count < MAX_DEL_CON)) 
	/* Insert con in selected con list */
	{
	  grectx->del_con_list[grectx->del_con_count] = con_id;
	  grectx->del_con_count++;
	}
}


/*************************************************************************
*
* Name:		gre_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Deletes seleced nodes and connections and connections connected
*	to selectetd nodes. Inserts all deleted objects in deletelists.
* 	Informs the user by gre_message if there is nothing to delete.
*
**************************************************************************/

void	gre_delete( 
	gre_ctx		grectx
)
{
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_con  	con;
	vldh_t_node	node;
	unsigned long	con_count;
	flow_tNode	*fnode_list;
	flow_tCon	*fcon_list;
	int		fnode_count;
	int		fcon_count;
	int		i,j;
	int		sts;

	grectx->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
	
        /* SG 20.03.91 if nothing has been done say it to the user */
        flow_GetSelectedNodes( grectx->flow_ctx, &fnode_list, &fnode_count);
        flow_GetSelectedCons( grectx->flow_ctx, &fcon_list, &fcon_count);
	if ( fnode_count == 0 && fcon_count == 0 )
        {
	  gre_message ( grectx , "Nothing to delete" );
	  BEEP;
	  return;
	}

	/* Delete nodes */
	/* Get all connections connected to every node and delete it */
	for ( i = 0; i < fnode_count; i++ )
	{
	  flow_GetUserData( fnode_list[i], (void **)&node);

	  /* delete connected connections */
	  sts = vldh_get_cons_node( node, &con_count, &con_list);
	  con_ptr = con_list;
	  for ( j = 0; j < con_count; j++)
	  {
	    vldh_con_delete(*con_ptr);
	    goec_con_delete( grectx, *con_ptr);
	    gre_delcon_insert( grectx, *con_ptr);
	    con_ptr++;
	  }
	  if ( con_count > 0) XtFree((char *) con_list);
	  /* delete the node */
	  vldh_node_delete( node);
	  flow_DeleteNode( fnode_list[i]);

	  gre_delnode_insert( grectx, node);
	}
	if ( fcon_count > 0)
	  free( fcon_list);
	if ( fnode_count > 0)
	  free( fnode_list);

	/* delete the remaining selected connections */
        flow_GetSelectedCons( grectx->flow_ctx, &fcon_list, &fcon_count);
	for ( i = 0; i < fcon_count; i++ )
	{
	  flow_GetUserData( fcon_list[i], (void **)&con);
	  if ( ( con->hc.status & VLDH_DELETE) == 0 )
	  {
	    /* delete the con */
	    vldh_con_delete( con);
	    goec_con_delete( grectx, con);
	    gre_delcon_insert( grectx, con);
	  }
	}
	if ( fcon_count > 0)
	  free( fcon_list);

	grectx->sel_con_count = 0;
	grectx->sel_node_count = 0;

}


/*************************************************************************
*
* Name:		gre_delete_node()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Deletes a single node. All connections connected to the node
*	are also deleted. The deleted objects are
*	inserted in deletelist.
*
**************************************************************************/

int	gre_delete_node(
	gre_ctx		grectx,	
	vldh_t_node	node
)
{
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	unsigned long	con_count;
	int		j;
	int		sts;

	grectx->del_con_count = 0;
	grectx->del_node_count = 0;

	/* Get all connections connected to the node and delete it */
	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++)
	{
	  vldh_con_delete(*con_ptr);
	  goec_con_delete( grectx, *con_ptr);
	  gre_delcon_insert( grectx, *con_ptr);
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) con_list);

	/* delete the node */
	vldh_node_delete( node);
	flow_DeleteNode( node->hn.node_id);
	gre_delnode_insert( grectx, node);

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_delete_con()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con		I	vldh connection.
*
* Description:
*	Deletes a single connection. 
*	The deleted connections is inserted in deletelist.
*
**************************************************************************/

void	gre_delete_con(
	gre_ctx		grectx,
	vldh_t_con	con
)
{

	grectx->del_con_count = 0;
	grectx->del_node_count = 0;

	vldh_con_delete( con);
	goec_con_delete( grectx, con);
	gre_delcon_insert( grectx, con);
}


/*************************************************************************
*
* Name:		gre_paste_node_exec()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		x		I	x koordinate.
* float		y		I	y koordinate.
*
* Description:
*	Inserts the paste signelnode in the current window.
*
**************************************************************************/

int  gre_paste_node_exec(
	gre_ctx		grectx,
	float		x,
	float		y
)
{
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		gre_cut()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	This routine deletes all selected objects and makes it possible
*	to recover them by undelete or paste.
*	Deletes seleced nodes and connections and connections connected
*	to selectetd nodes. Inserts all deleted objects in deletelist and
*	in the vldh_paste list.
*
**************************************************************************/

int	gre_cut(
	gre_ctx		grectx
)
{
	int		i;
	vldh_t_con  	con;
	vldh_t_node	node;
	flow_tNode	*fnode_list;
	flow_tCon	*fcon_list;
	int		fnode_count;
	int		fcon_count;

	/* Copy selected objects to pastelist */
        flow_GetSelectedNodes( grectx->flow_ctx, &fnode_list, &fnode_count);
        flow_GetSelectedCons( grectx->flow_ctx, &fcon_list, &fcon_count);
	if ( fnode_count == 0 && fcon_count == 0 )
        {
	  gre_message ( grectx , "Nothing to cut" );
	  BEEP;
	  return GRE__SUCCESS;
	}

	/* Insert in vldh_pastelist */
	vldh_paste_init();

	/* Insert the nodes */
	for ( i = 0; i < fnode_count; i++ )
	{
	  flow_GetUserData( fnode_list[i], (void **)&node);
	  vldh_paste_node_insert( grectx->window_object, node);
	}
	/* Insert the cons */
	for ( i = 0; i < fcon_count; i++ )
	{
	  flow_GetUserData( fcon_list[i], (void **)&con);
	  vldh_paste_con_insert( grectx->window_object, con);
	}
	if ( fcon_count > 0)
	  free( fcon_list);
	if ( fnode_count > 0)
	  free( fnode_list);

	/* Delete the selected objects */
	gre_delete( grectx);

	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_cut_node()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Deletes one single node and all connections connected
*	to the node. Inserts all the deleted objects in deletelist and
*	the node in vldh_singlenode buffer.
*
**************************************************************************/

int  gre_cut_node(
	gre_ctx		grectx,
	vldh_t_node	node
)
{

	/* Insert in vldh_pastelist */
	vldh_paste_init();

	vldh_paste_node_insert( grectx->window_object, node);
	
	gre_delete_node( grectx, node);
	return GRE__SUCCESS;

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_copy_node()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Copies one single node to the paste singelnode buffer.
*
**************************************************************************/

int gre_copy_node(
	gre_ctx		grectx,
	vldh_t_node	node
)
{

	/* Insert in vldh_pastelist */
	vldh_paste_init();

	/* Insert the nodes */
	vldh_paste_node_insert( grectx->window_object, node);
	return GRE__SUCCESS;
#if 0
	vldh_paste_setmode( VLDH_PASTEMODE_OBJECT);

	sts = vldh_paste_singlenode_insert( grectx->window_object, node);
	return sts;
#endif
}


/*************************************************************************
*
* Name:		gre_copy()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Copys all selected objects to the vldh_paste list.
*
**************************************************************************/

int	gre_copy(
    gre_ctx		grectx
)
{
	int		i;
	vldh_t_con  	con;
	vldh_t_node	node;
	flow_tNode	*fnode_list;
	flow_tCon	*fcon_list;
	int		fnode_count;
	int		fcon_count;

        flow_GetSelectedNodes( grectx->flow_ctx, &fnode_list, &fnode_count);
        flow_GetSelectedCons( grectx->flow_ctx, &fcon_list, &fcon_count);
	if ( fnode_count == 0 && fcon_count == 0 )
        {
	  gre_message ( grectx , "Nothing to copy" );
	  BEEP;
	  return GRE__SUCCESS;
	}

	/* Insert in vldh_pastelist */
	vldh_paste_init();

	/* Insert the nodes */
	for ( i = 0; i < fnode_count; i++ )
	{
	  flow_GetUserData( fnode_list[i], (void **)&node);
	  vldh_paste_node_insert( grectx->window_object, node);
	}
	/* Insert the cons */
	for ( i = 0; i < fcon_count; i++ )
	{
	  flow_GetUserData( fcon_list[i], (void **)&con);
	  vldh_paste_con_insert( grectx->window_object, con);
	}
	gre_unselect( grectx);

	if ( fcon_count > 0)
	  free( fcon_list);
	if ( fnode_count > 0)
	  free( fnode_list);

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_window_draw()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	This routine draws all nodes and connections in a vldh window
*	into a neted window.
*
**************************************************************************/

int	gre_window_draw(
	gre_ctx		grectx
)
{
	vldh_t_node 	*node_list;
	vldh_t_node 	*node_ptr;
	unsigned long	node_count;
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	    con_count;
	int		i;
	flow_tNode	node_id;
	flow_tNodeClass	node_class;
	int		sts;
	double		ll_x, ll_y, ur_x, ur_y;

	/* Get all the nodes */
	sts = vldh_get_nodes( grectx->window_object, &node_count, &node_list);
	if ( EVEN(sts)) return sts;
	node_ptr = node_list;

	/* Get all the connections */
	sts = vldh_get_cons( grectx->window_object, &con_count, &con_list);
	if ( EVEN(sts)) return sts;
	con_ptr = con_list;

        flow_SetNodraw( grectx->flow_ctx);

	/* Create the nodes */
	for ( i = 0; i < node_count; i++)
	{
	  sts = gre_get_nodeclass( grectx, 
		(*node_ptr)->ln.classid,
		((*node_ptr)->hn.window_pointer)->hw.ldhsession, 
		(*node_ptr)->ln.object_type,
		(*node_ptr)->ln.mask, (*node_ptr)->ln.subwindow,
		(*node_ptr)->ln.nodewidth,
		&node_class, *node_ptr);
	  if (EVEN(sts)) return sts;

	  flow_CreateNode( grectx->flow_ctx, (*node_ptr)->hn.name, 
		node_class, (*node_ptr)->ln.x, (*node_ptr)->ln.y,
		*node_ptr, &node_id);
	  (*node_ptr)->hn.node_id = node_id;

	  sts = gre_node_annotations_draw( grectx, *node_ptr, 0); 
	  if (EVEN(sts)) return sts;

	  /* Update widht */
	  flow_MeasureNode( (*node_ptr)->hn.node_id,
		&ll_x, &ll_y, &ur_x, &ur_y);
	  (*node_ptr)->ln.width = ur_x - ll_x;
	  (*node_ptr)->ln.height = ur_y - ll_y;
	  flow_GetNodePosition( (*node_ptr)->hn.node_id, &ll_x, &ll_y);
	  (*node_ptr)->ln.x = ll_x;
	  (*node_ptr)->ln.y = ll_y;

	  node_ptr++;
	}

	/* Create the neted connections */
	for ( i = 0; i < con_count; i++)
	{
	  sts = goec_con_draw( grectx, *con_ptr, GRE_CON_NONROUTE, 0);
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) con_list);
	if ( node_count > 0) XtFree((char *) node_list);

        flow_ResetNodraw( grectx->flow_ctx);
        flow_Redraw( grectx->flow_ctx);


	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		gre_get_selnodes()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* unsigned long	*node_count	O	number of nodes in nodelist.
* vldh_t_node	**intern_nodelist O	list of selected nodes.
*
* Description:
*	Returns an array of all selected nodes.
*	The array should be freed by the user with a Xtfree call.
*
**************************************************************************/

int  gre_get_selnodes( 
	gre_ctx		grectx,
	unsigned long		*node_count,
	vldh_t_node		**nodelist
)
{
	int	i;
	vldh_t_node	node;
	flow_tNode	*fnode_list;
	int		fnode_count;

        flow_GetSelectedNodes( grectx->flow_ctx, &fnode_list, &fnode_count);

	*node_count = 0;
	if ( fnode_count == 0)
	  return GRE__SUCCESS;

	/* create array */
	*nodelist = (vldh_t_node *)XtCalloc( fnode_count, 
		sizeof(node));

	/* Insert the nodes */
	for ( i = 0; i < fnode_count; i++ )
	{
	  flow_GetUserData( fnode_list[i], (void **)&node);
	  *(*nodelist + *node_count) = node;
	  (*node_count)++;
	}
	return GRE__SUCCESS;
}


#if 0
/*************************************************************************
*
* Name:		gre_get_selcons()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* unsigned long	*con_count	O	number of nodes in conlist.
* vldh_t_con	**intern_conlist O	list of selected connections.
*
* Description:
*	Returns an array of all selected connections.
*	The array should be freed by the user with a Xtfree call.
*
**************************************************************************/

static int  gre_get_selcons(
	gre_ctx		grectx,
	unsigned long	*con_count,
	vldh_t_con	**conlist
)
{
	int	i;
	vldh_t_con	con;
	flow_tCon	*fcon_list;
	int		fcon_count;

        flow_GetSelectedCons( grectx->flow_ctx, &fcon_list, &fcon_count);

	*con_count = 0;
	if ( fcon_count == 0)
	  return GRE__SUCCESS;

	/* create array */
	*conlist = (vldh_t_con *)XtCalloc( fcon_count, 
		sizeof(con));

	/* Insert the cons */
	for ( i = 0; i < fcon_count; i++ )
	{
	  flow_GetUserData( fcon_list[i], (void **)&con);
	  *(*conlist + *con_count) = con;
	  (*con_count)++;
	}
	return GRE__SUCCESS;
}
#endif


/*************************************************************************
*
* Name:		void	gre_get_nodeclass()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* pwr_tClassId  class		I 	class of the object.
* ldh_tSesContext	    ldhses		I	ldh session.
* unsigned long	node_type	I	object type in graph body.
* unsigned long	*mask		I	mask of the node.
* unsigned long	subwindowmark	I	if subwindow mark should be drawn.
* unsigned long	node_width	I	width of node.
* unsigned long	*node_class	O	neted node type id.
*
* Description:
*	Returns a neted nodetypeid for the specified objecttype.
*	If the the mask for the nodetype is of the defaultmask for
*	the objecttype, the nodetypeid is fetcht from the nodetypelist
*	in the grecontext. If it doesn't exist the nodetype is created
*	and put into the nodetypelist.
*	If the mask is not the defautlmask a new nodetype is created every
*	time and not stored.
*
**************************************************************************/

static int gre_get_nodeclass (
	gre_ctx	grectx,
	pwr_tClassId  class,
	ldh_tSesContext	    ldhses,
	unsigned long	node_type,
	unsigned int	*mask,
	unsigned long	subwindowmark,
	unsigned long	node_width,
	flow_tNodeClass	*node_class,
	vldh_t_node	node
)
{
	unsigned char	new_nodetype;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	
/*	new_nodetype = FALSE; */
	new_nodetype = TRUE;

	if ( node->ln.nodewidth + 2 >= GOEN_F_MINWIDTH )
	  /* We have to make a new nodetype */
	  new_nodetype = TRUE;

	if ( *(mask + 2) != 0 )
	  /* Inverted inputs, new nodetype */
	  new_nodetype = TRUE;	

	  if ( (node->ln.object_type == 11) || (node->ln.object_type >= 250))
	    /* Always new node type for text objects */
	    new_nodetype = TRUE;

	if ( !new_nodetype )
	{
	  /* Get the defaultmask */
	  /* Get graphbody for the class */
	  sts = ldh_GetClassBody( ldhses, class, "GraphPlcNode",
			&bodyclass, (char **)&graphbody, &size);
	  if( EVEN(sts)) return sts;

	  /* Check if the mask is changed */
	  if( (graphbody->default_mask[0] != *mask) ||
	    (graphbody->default_mask[1] != *(mask + 1)))
	    /* New nodetype */
	    new_nodetype = TRUE;

	}

	if ( !new_nodetype )
	{
	  /* Use the standard graphics and save nodetypeid in context */
	  if ( grectx->nodeclass_list[node_type][subwindowmark] == 0 )
	  /* Create a new nodetype_id */
	  {
            sts = goen_create_nodetype( grectx->flow_ctx, class, ldhses, 
			 mask, subwindowmark, node_width,
			 &grectx->nodeclass_list[node_type][subwindowmark],
			 node);
	    if ( EVEN(sts) ) return sts;
	  }
	  *node_class = grectx->nodeclass_list[node_type][subwindowmark];
	}
	else
	{
	  /* create a special nodetype for this node */
          sts = goen_create_nodetype( grectx->flow_ctx, class, ldhses, mask, 
			 subwindowmark, node_width,
			 node_class, node);
	  if ( EVEN(sts) ) return sts;
	}
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		void	gre_get_conclass()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	Context adress
* pwr_tClassId  class		I	class of connection object.
* ldh_tSesContext	    ldhses		I	ldh session.
* unsigned long con_type	I	connection type
* unsigned long con_class	O	neted connection type id
*
* Description:
*	Get neted connectiontype id for a connection.
*	If it is the first time the con_type is created and the id is
*	stored in contypeidlist in gre context. If not the first time it
*	is fetched from there.
*
**************************************************************************/

int gre_get_conclass (
	gre_ctx	grectx,
	pwr_tClassId  class,
	ldh_tSesContext	    ldhses,
	unsigned long con_type,
	flow_tConClass *con_class
)
{
	int sts;

	if ( class == 0)
	  con_type = MAX_NUMBER_OF_CONTYPES - 1;
	if ( grectx->conclass_list[con_type] == 0 )
	{
	  /* If the connectiontype is not yet created, create it */
	  sts = goen_create_contype( grectx->flow_ctx, class, ldhses,
			&grectx->conclass_list[con_type]);
	  if ( EVEN(sts) ) return sts;
	}
	*con_class = grectx->conclass_list[ con_type ];
	return GRE__SUCCESS;
}


/*_Backcalls for the controlled flow widget______________________________*/


/*************************************************************************
*
* Name:		int	gre_flow_cb()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Callback from flow.
**************************************************************************/
int gre_flow_cb( FlowCtx *ctx, flow_tEvent event)
{
  unsigned long dummy = 0;
  gre_ctx	grectx;
  vldh_t_node	source, dest;
  void		*vobject;
  double	ll_x, ll_y, ur_x, ur_y, width, height;
  int		subwindow_nr;
  int		index;

  flow_GetCtxUserData( ctx, (void **)&grectx);

  gre_search_rectangle_delete( grectx);

  if ( event->any.type == flow_eEventType_CreateCon)
  {
    flow_GetUserData( event->con_create.source_object, (void **) &source);
    if ( event->con_create.dest_object)
      flow_GetUserData( event->con_create.dest_object, (void **) &dest);
    else
      dest = 0;
    (grectx->gre_con_created) (grectx, 
		event->con_create.x, event->con_create.y, 
		source, event->con_create.source_conpoint,
		dest, event->con_create.dest_conpoint);	
  }
  switch ( event->event)
  {
    case flow_eEvent_Init:
      break;
    case flow_eEvent_PasteSequenceStart:
      flow_SetSelectHighlight( ctx);
      break;
    case flow_eEvent_MB2DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          flow_GetUserData( event->object.object, &vobject);
	  (grectx->gre_delete) (grectx, vobject, VLDH_NODE);
          break;
        case flow_eObjectType_Con:
          flow_GetUserData( event->object.object, &vobject);
	  (grectx->gre_delete) (grectx, vobject, VLDH_CON);
          break;
        default:
	  (grectx->gre_delete) (grectx, 0, 0);
      }
      break;
    case flow_eEvent_MB1Click:
      /* Select */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
        case flow_eObjectType_Con:
          if ( flow_FindSelectedObject( ctx, event->object.object))
          {
            flow_SelectClear( ctx);
          }
          else
          {
            flow_SelectClear( ctx);
            flow_SetHighlight( event->object.object, 1);
            flow_SelectInsert( ctx, event->object.object);
          }
          break;
        default:
          flow_SelectClear( ctx);
      }
      break;
    case flow_eEvent_ObjectMoved:
    {
      vldh_t_node	vnode;
      vldh_t_con	vcon;
      double		pos_x, pos_y;
      double		*x_arr, *y_arr;
      int		i, num;

      /* Object moved */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          flow_GetUserData( event->object.object, (void **) &vnode);
	  flow_GetNodePosition( event->object.object, &pos_x, &pos_y);
	  vnode->ln.x = pos_x;
	  vnode->ln.y = pos_y;
	  vldh_nodemodified( vnode);

	  (grectx->gre_node_moved) (grectx);
          break;
        case flow_eObjectType_Con:
          flow_GetUserData( event->object.object, (void **) &vcon);
	  flow_GetConPosition( event->object.object, &x_arr, &y_arr, &num);
	  for ( i = 0; i < num; i++)
	  {
	    vcon->lc.point[i].x = x_arr[i];
	    vcon->lc.point[i].y = y_arr[i];
	  }
	  vcon->lc.point_count = num;
	  vldh_conmodified( vcon);
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_MB1ClickShift:
      /* Select */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
        case flow_eObjectType_Con:
          if ( flow_FindSelectedObject( ctx, event->object.object))
          {
            flow_SetHighlight( event->object.object, 0);
            flow_SelectRemove( ctx, event->object.object);
          }
          else
          {
            flow_SetHighlight( event->object.object, 1);
            flow_SelectInsert( ctx, event->object.object);
          }
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_MB1Press:
      /* Select region */
      flow_SetSelectHighlight( ctx);
      break;
    case flow_eEvent_MB1PressShift:
      /* Select region */
      flow_SetSelectHighlight( ctx);
      break;
    case flow_eEvent_MB2Click:
      /* Crdate node */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_NoObject:
	  (grectx->gre_node_created) ( grectx, dummy, event->object.x,
			event->object.y);
          break;
        default:
          break;
      }
      break;
    case flow_eEvent_MB1DoubleClick:
      /* Open attribute editor */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          grectx->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
          flow_GetUserData( event->object.object, &vobject);
	  (grectx->gre_attribute) (grectx, (vldh_t_node) vobject);
          break;
        default:
	  gre_message( grectx, "No hit in object");
	  BEEP;
      }
      break;
    case flow_eEvent_MB1DoubleClickShift:
      /* Open subwindow */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  grectx->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
	  flow_MeasureNode( event->object.object, &ll_x,&ll_y,&ur_x,&ur_y);
          width = ur_x - ll_x;
	  if ( event->object.x > ( ll_x + width / 2 ))	
	    subwindow_nr = 1;
	  else				
	    subwindow_nr = 2;
          flow_GetUserData( event->object.object, &vobject);
	  (grectx->gre_subwindow) ( grectx, (vldh_t_node)vobject, subwindow_nr);
          break;
        default:
	  gre_message (grectx,"No hit in object");
	  BEEP;
      }
      break;
    case flow_eEvent_MB1DoubleClickCtrl:
      /* Insert reference object */
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
	  grectx->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
	  flow_MeasureNode( event->object.object, &ll_x,&ll_y,&ur_x,&ur_y);
          height = ur_y - ll_y;
	  if ( event->object.y > ll_y + height / 2)	
	    index = 1;
	  else
	    index = 2;
          flow_GetUserData( event->object.object, &vobject);
	  (grectx->gre_getobj) ( grectx, (vldh_t_node)vobject, index);
          break;
        default:
	  gre_message (grectx,"No hit in object");
	  BEEP;
      }
      break;
    case flow_eEvent_MB3Down:
      flow_SetClickSensitivity( grectx->flow_ctx, flow_mSensitivity_MB3Press);
      break;
    case flow_eEvent_MB3Press:
    {
      /* Popup menu */
      short		x_work_area, y_work_area;
      int		x_pix, y_pix;
      vldh_t_node	current_node;
      flow_tObject	*select_list;
      int		select_cnt;
      Arg		arg[4];
      Widget		w;

      flow_PositionToPixel( grectx->flow_ctx, event->object.x,
			event->object.y, &x_pix, &y_pix); 
	  
      w = XtParent( grectx->cp.parent_wid);
      XtSetArg( arg[0], XmNx, &x_work_area);
      XtSetArg( arg[1], XmNy, &y_work_area);
      XtGetValues( w, arg, 2);
      x_pix += x_work_area;
      y_pix += y_work_area;

      w = XtParent(w);
      XtSetArg( arg[0], XmNx, &x_work_area);
      XtSetArg( arg[1], XmNy, &y_work_area);
      XtGetValues( w, arg, 2);
      x_pix += x_work_area;
      y_pix += y_work_area;

      /* Add coordinates of the work_area widget */
      XtSetArg( arg[0], XmNx, &x_work_area);
      XtSetArg( arg[1], XmNy, &y_work_area);
      XtGetValues( grectx->flow_widget, arg, 2);

      /* Add coordinates of the work_area widget */
      XtSetArg( arg[0], XmNx, &x_work_area);
      XtSetArg( arg[1], XmNy, &y_work_area);
      XtGetValues( grectx->cp.parent_wid, arg, 2);

      x_pix += x_work_area;
      y_pix += y_work_area;

      current_node = 0;

      /* If there is no selected object, select current object */
      if ( grectx->popupmenu_mode == GRE_POPUPMENUMODE_OBJECT)
      {
        flow_GetSelectList( ctx, &select_list, &select_cnt);
        if ( !select_cnt)
        {
          if ( event->object.object_type == flow_eObjectType_Node)
            /* Get the current object */
            flow_GetUserData( event->object.object, (void **)&current_node);
        }	
      }

      (grectx->gre_popupmenu) (grectx, x_pix, y_pix, 
			grectx->popupmenu_mode, current_node);
      break;
    }
    case flow_eEvent_MB1ClickCtrl:
    {
      char			help_title[32];
      vldh_t_node		node;
      vldh_t_con		con;
      int			size, sts;

      if ( event->object.object_type == flow_eObjectType_Node)
      {
        flow_GetUserData( event->object.object, (void **)&node);
	sts = ldh_ObjidToName(
		(node->hn.window_pointer)->hw.ldhsession,
	         cdh_ClassIdToObjid( node->ln.classid), ldh_eName_Object,
		 help_title, sizeof( help_title), &size);
	error_msg(sts);
	if ( EVEN(sts)) return 1;
	(grectx->gre_help) ( grectx, help_title);
      }
      if ( event->object.object_type == flow_eObjectType_Con)
      {
        flow_GetUserData( event->object.object, (void **)&con);
	sts = ldh_ObjidToName( 
			(grectx->window_object)->hw.ldhsession, 
	         	cdh_ClassIdToObjid( con->lc.classid), ldh_eName_Object,
		        help_title, sizeof( help_title), &size);
	error_msg(sts);
	if ( EVEN(sts)) return 1;
	(grectx->gre_help) ( grectx, help_title);
      }
      else
      {
	gre_message( grectx, "No hit in object");
        BEEP;
      }	
      break;
    }
    case flow_eEvent_MB1DoubleClickShiftCtrl:
    {
      /* Copy */
      if ( event->object.object_type == flow_eObjectType_Node)
      {
        /* Copy only this object */
        flow_GetUserData( event->object.object, &vobject);
	(grectx->gre_copy) (grectx, vobject, VLDH_NODE);
      }
      else
      {
	(grectx->gre_copy) (grectx, 0, 0);
      }
      break;
    }
    case flow_eEvent_MB2DoubleClickShiftCtrl:
    {
      /* Cut */
      if ( event->object.object_type == flow_eObjectType_Node)
      {
        /* Copy only this object */
        flow_GetUserData( event->object.object, &vobject);
	(grectx->gre_cut) (grectx, vobject, VLDH_NODE);
      }
      else
      {
	(grectx->gre_cut) (grectx, 0, 0);
      }
      break;
    }
    case flow_eEvent_MB2ClickShiftCtrl:
    {
      /* Paste */
      (grectx->gre_paste) (grectx, event->object.x, event->object.y);
      break;
    }
    case flow_eEvent_SelectClear:
      flow_ResetSelectHighlight( ctx);
      break;
    default:
      ;
  }
  return 1;
}


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		gre_ctx	gre_new()
*
* Type		gre_ctx
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char 		*name		I	Name of the created object
*
* Description:
*	Create a new gre instance.
*	This routine creates the neted widget.
*
**************************************************************************/

gre_ctx	gre_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name
	)
{
  Arg		args[20];
  int		i;
  gre_ctx	grectx;
	
  static MrmRegisterArg	reglist[] = {
    { "gre_ctx", 0 }
	};

  /* Create object context */
  grectx = (gre_ctx)XtCalloc( 1, sizeof( *grectx ) );
  reglist[0].value = (caddr_t) grectx;

  /* Fill in common part */
  grectx->cp.parent_wid = parent_wid;
  grectx->cp.parent_ctx = parent_ctx;
  grectx->cp.name = XtNewString( name );

  gre_ctx_init( grectx);
 


  i = 0;
  XtSetArg( args[i], XmNwidth, 600);i++;
  XtSetArg( args[i], XmNheight, 600);i++;
  XtSetArg( args[i], XmNtopAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM);i++;
/*
  XtSetArg( args[i], XmNresizePolicy, XmRESIZE_ANY);i++;
*/
  grectx->form_widget = ScrolledFlowCreate( grectx->cp.parent_wid, "Flow window", 
	args, i, gre_init_flow, grectx, &grectx->flow_widget);


  XtManageChild( grectx->form_widget);
  /* Get work_area widget */
  grectx->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;

  return(grectx);
}

int gre_new_navigator( gre_ctx grectx, Widget parent)
{
  Arg 		args[20];
  int		i;

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,800);i++;
  grectx->nav_widget = FlowCreateNav( parent, "navigator",
        args, i, grectx->flow_widget);
  XtManageChild( grectx->nav_widget);

  return 1;
}

int gre_new_navigator_popup( gre_ctx grectx)
{
  Arg 		args[20];
  int		i;

  /* Create navigator popup */

  i = 0;
  XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
  XtSetArg(args[i],XmNallowResize, TRUE); i++;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNx,500);i++;
  XtSetArg(args[i],XmNy,500);i++;

  grectx->nav_shell = XmCreateDialogShell( grectx->flow_widget, "Navigator",
        args, i);
  XtManageChild( grectx->nav_shell);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  grectx->nav_widget = FlowCreateNav( grectx->nav_shell, "navigator",
        args, i, grectx->flow_widget);
  XtManageChild( grectx->nav_widget);
  XtRealizeWidget(grectx->nav_shell);

  return 1;
}

int gre_init( gre_ctx grectx)
{
  FlowWidget	flow_widget;
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_widget = (FlowWidget) grectx->flow_widget;
  grectx->flow_ctx = (flow_tCtx)flow_widget->flow.flow_ctx;

  mask = 0;
  flow_attr.base_zoom_factor = 450;
  mask |= flow_eAttr_base_zoom_factor;
  flow_attr.grid_size_x = 0.05;
  mask |= flow_eAttr_grid_size_x;
  flow_attr.grid_size_y = 0.05;
  mask |= flow_eAttr_grid_size_y;
  flow_attr.grid_on = 1;
  mask |= flow_eAttr_grid_on;
  flow_attr.user_highlight = 0;
  mask |= flow_eAttr_user_highlight;
  flow_attr.draw_delta = 0.015;
  mask |= flow_eAttr_draw_delta;
  flow_attr.grafcet_con_delta = 0.05;
  mask |= flow_eAttr_grafcet_con_delta;
  flow_attr.refcon_width = 0.065;
  mask |= flow_eAttr_refcon_width;
  flow_attr.refcon_height = 0.035;
  mask |= flow_eAttr_refcon_height;
  flow_attr.refcon_linewidth = 2;
  mask |= flow_eAttr_refcon_textsize;
  flow_attr.refcon_textsize = GOEN_F_TEXTSIZE;
  mask |= flow_eAttr_refcon_linewidth;
  flow_attr.application_paste = 1;
  mask |= flow_eAttr_application_paste;
  flow_SetAttributes( grectx->flow_ctx, &flow_attr, mask);
  flow_SetCtxUserData( grectx->flow_ctx, grectx);

  /* Create cursors and colors */
  gre_create_cursors( grectx);
  return GRE__SUCCESS;
}

static int gre_init_flow( FlowCtx *ctx, void *client_data)
{
  return 1;
}

int gre_edit_setup( gre_ctx grectx)
{
  flow_tCtx ctx = grectx->flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_MoveNode, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Press, flow_eEventType_CreateCon, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_RegionSelect, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1PressShift, flow_eEventType_RegionAddSelect, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClick, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Click, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2ClickShiftCtrl, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_Init, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_PasteSequenceStart, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ObjectMoved, flow_eEventType_CallBack, 
	gre_flow_cb);
  return 1;
}

int gre_view_setup( gre_ctx grectx)
{
  flow_tCtx ctx = grectx->flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_RegionSelect, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1PressShift, flow_eEventType_RegionAddSelect, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_Init, flow_eEventType_CallBack, 
	gre_flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	gre_flow_cb);
  return 1;
}

/*************************************************************************
*
* Name:		gre_setup_backcalls ()
*
* Type		gre_ctx
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* voidp		gre_setup_window I	backcall routine.
* voidp		gre_node_created I	backcall routine.
* voidp		gre_con_created	I	backcall routine.
* voidp		gre_node_moved	I	backcall routine.
* voidp		gre_delete	I	backcall routine.
* voidp		gre_cut		I	backcall routine.
* voidp		gre_copy	I	backcall routine.
* voidp		gre_paste	I	backcall routine.
* voidp		gre_attribute	I	backcall routine.
* voidp		gre_subwindow	I	backcall routine.
* voidp		reserv		I	backcall routine.
* voidp		gre_popupmenu	I	backcall routine.
* voidp		gre_getobj	I	backcall routine.
* voidp		gre_undelete	I	backcall routine.
* voidp		gre_unselect	I	backcall routine.
* voidp		gre_help	I	backcall routine.
* voidp		gre_regionmoved	I	backcall routine.
* voidp		gre_message	I	backcall routine.
*
* Description:
*	Store the adresses of the desired backcalls functions in the 
*	gre context.
* 	Description of backcall routines:
*
* static void zzz_gre_setup_window( grectx)
*	
*	Routine called when the neted window is created.
*	Makes is possible to enable the desired events by the routine
*	gre_setup_window()
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* static void zzz_gre_node_created( grectx, node_type, x, y)
*	
*	Routine called when a node creation callback is recieved from neted.
*	Makes is possible to create of the desired class by gre_create_node.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* unsigned long	node_type	I	Not used. 
* float		x		I	x koordinate ( on grid).
* float		y		I	y koordinate ( on grid).
*
* static void 	zzz_gre_con_created (grectx, source_obj, source_point,
*				destination_obj, destination_point)
*	
*	Routine called when a connection creation callback is recieved from 
*	neted.
*	Makes is possible to create a connections of the desired class by 
*	gre_create_con.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	source_obj	I	source node.
* unsigned long	source_point	I	connection point on source node.
* vldh_t_node	destination_obj	I	destination node.
* unsigned long	destination_point I	connection point on destination node.
*
* static void 	zzz_gre_node_moved (grectx)
*	
* 	Routine called when a node is moved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* static void 	zzz_gre_node_moved (grectx)
*	
* 	Routine called when a node is moved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* static void 	zzz_gre_delete (grectx, object, object_type)
*	
* 	Routine called when a delete callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.	
* unsigned long	object		I	vldh node or connection,
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, VLDH_CON
*					or zero if no objects is hit by the 
*					click.
*
* static void 	zzz_gre_cut (grectx, object, object_type)
*	
* 	Routine called when a cut callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.	
* unsigned long	object		I	vldh node or connection,
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, VLDH_CON
*					or zero if no objects is hit by the 
*					click.
*
* static void 	zzz_gre_copy (grectx, object, object_type)
*	
* 	Routine called when a copy callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.	
* unsigned long	object		I	vldh node or
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, or zero 
*					if no objects is hit by the 
*					click.
*
* static void foe_gre_paste (grectx, x, y)
*
* 	Routine called when a paste callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		x		I	x koordinate ( on grid)
* float		y		I	y koordinate ( on grid)
*
* static void zzz_gre_attribute (grectx, object)
*	
* 	Routine called when a attribute callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	object		I	vldh node.
*
* static void zzz_gre_subwindow (grectx, object, subwindow_nr)
*	
* 	Routine called when a subwindow callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node 	object		I	vldh node.
* unsigned long	subwindow_nr	I	1 if the click is in the left part
*					of node,
*					2 in the right part.
*
* static void zzz_reserv(grectx, x, y)
*
* 	Routine called when a reserv callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		x		I	x koordinate ( on grid)
* float		y		I	y koordinate ( on grid)
*
* static void foe_gre_popupmenu (grectx, x_pix, y_pix)
*	
* 	Routine called when popupmenu callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* int		x_pix		I	click x koordinate in pixel
* int		y_pix		I	click y koordinate in pixel
*
* static void zzz_gre_getobj (grectx , node, index)
*	
* 	Routine called when a getobj callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node 	node 		I	vldh node.
* unsigned long	index		I	0 if the click is in the upper part
*					of the node.
*					1 if it is in the lower part.
*
* static void zzz_gre_undelete (grectx )
*
* 	Not yet implemented.
*
* static void zzz_gre_unselect (grectx )
*
* 	Not yet implemented.
*
* static void foe_gre_help (grectx, help_title)
*
* 	Routine called when a help callback is recieved from neted.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* char		*help_title	I	name of the objects class.
*
* static void zzz_gre_regionmoved (grectx) 
*
* 	Not yet implemented.
*
* static void zzz_gre_message (grectx, message)
*
* 	Routine called when gre feels he has to send a message to the user.
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* char		*message	I	text message.
*
**************************************************************************/

int gre_setup_backcalls (
	gre_ctx	grectx,
	void (*gre_setup_window)(gre_ctx),
	void (*gre_node_created)(gre_ctx, unsigned long, float, float),
	void (*gre_con_created)(gre_ctx, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long),
	void (*gre_node_moved)(gre_ctx),
	void (*gre_delete)(gre_ctx, void *, unsigned long),
	void (*gre_cut)(gre_ctx, void *, unsigned long),
	void (*gre_copy)(gre_ctx, void *, unsigned long),
	void (*gre_paste)(gre_ctx, float, float),
	void (*gre_attribute)(gre_ctx, vldh_t_node),
	void (*gre_subwindow)(gre_ctx, vldh_t_node, unsigned long),
	voidp (*gre_reserv),
	void (*gre_popupmenu)(gre_ctx, int, int, int, vldh_t_node),
	void (*gre_getobj)(gre_ctx, vldh_t_node, unsigned long),
	void (*gre_undelete)(gre_ctx),
	void (*gre_unselect)(gre_ctx),
	void (*gre_help)(gre_ctx, char *),
	void (*gre_regionmoved)(gre_ctx),
	void (*gre_message)(gre_ctx, char *)
	)
{

/* Fill in callback addresses */
 grectx->gre_setup_window = gre_setup_window;
 grectx->gre_node_created = gre_node_created;
 grectx->gre_con_created = gre_con_created;
 grectx->gre_node_moved = gre_node_moved;
 grectx->gre_delete = gre_delete;
 grectx->gre_cut = gre_cut;
 grectx->gre_copy = gre_copy;
 grectx->gre_paste = gre_paste;
 grectx->gre_attribute = gre_attribute;
 grectx->gre_subwindow = gre_subwindow;
 grectx->gre_reserv = gre_reserv;
 grectx->gre_popupmenu = gre_popupmenu;
 grectx->gre_getobj = gre_getobj;
 grectx->gre_undelete = gre_undelete;
 grectx->gre_unselect = gre_unselect;
 grectx->gre_help = gre_help;
 grectx->gre_regionmoved = gre_regionmoved;
 grectx->gre_message = gre_message;

 return GRE__SUCCESS ;
}


/*************************************************************************
*
 * Name:		gre_del()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	Parent context adress
*
* Description:
*	Deletes a gre instance.
*	Deletes the widget and frees allocated memory.
*
**************************************************************************/

void	gre_del(
	gre_ctx	grectx
	)
{

	/* Destroy the widgets */
	XtDestroyWidget( XtParent( grectx->form_widget));

	/* free the context */
	XtFree((char *) grectx);

}

/*************************************************************************
*
* Name:		void	gre_setup_window()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx *	grectx		I	Parent context adress
* unsigned long zoom_mask	I	Buttonmask for zoom event
* unsigned long scroll_mask	I	Buttonmask for scroll event
* unsigned long nodecreate_mask	I	Buttonmask for nodecreate event
* unsigned long concreate_mask	I	Buttonmask for concreate event
* unsigned long nodemoved_mask	I	Buttonmask for nodemoved event
* unsigned long nodesel_mask	I	Buttonmask for nodesel event
* unsigned long nodeseladd_mask	I	Buttonmask for nodeseladd event
* unsigned long enterleave_flag	I	Flag for enterleave event
*
* unsigned long	zoom_mask	I	Buttonmask for zoom event in navigation
*					window
* unsigned long	scroll_mask		Buttonmask for scroll event in
*					navigation window.
* unsigned long	concreate_mask	I	Buttonmask for create connection.
* unsigned long	nodemoved_mask	I	Buttonmask for move node.
* unsigned long	regionsel_mask	I	Buttonmask for select region.
* unsigned long	nodecreate_mask	I	Buttonmask for create node.
* unsigned long	nodecreate_click I	Click for create node.
* unsigned long	nodesel_mask	I	Buttonmask for select node.
* unsigned long	nodesel_click	I	Click for select node.
* unsigned long	nodeseladd_mask	I	Buttonmask for select connection. 
* unsigned long	nodeseladd_click	I	Click for select connection.
* unsigned long	delete_mask	I	Buttonmask for delete.
* unsigned long	delete_click	I	Click for delete.
* unsigned long	cut_mask	I	Buttonmask for cut.
* unsigned long	cut_click	I	Click for cut.
* unsigned long	copy_mask	I	Buttonmask for copy.
* unsigned long	copy_click	I	Click for copy.
* unsigned long	paste_mask	I	Buttonmask for paste.
* unsigned long	paste_click	I	Click for paste.
* unsigned long	attribute_mask	I	Buttonmask for attribute.
* unsigned long	attribute_click	I	Click for attribute.
* unsigned long	subwindow_mask	I	Buttonmask for subwindow.
* unsigned long	subwindow_click	I	Click for subwindow.
* unsigned long	reserv_mask	I	Buttonmask for paste area.
* unsigned long	reserv_click	I	Click for paste area.
* unsigned long	popupmenu_mask	I	Buttonmask for popupmenu.
* unsigned long	popupmenu_click	I	Click for popupmenu.
* unsigned long	getobj_mask	I	Buttonmask for connect.
* unsigned long	getobj_click	I	Click for connect.
* unsigned long	help_mask	I	Buttonmask for help.
* unsigned long	help_click	I	Click for help
* unsigned char	enterleave_flag	I	Enterleave event or not ( 1/0).
*
* Description:
*	This routine defines the mouse buttons and type of click
*	for operations in the neted window.
*	The masks can be Button1Mask, Button2Mask, Button3Mask or combinations
*	with one of these tre and ShiftMask and/or ControlMask.
*	The clicks can be GRE_CLICK for singleclick, GRE_DOUBLECLICK for 
*	doubleclick and GRE_PRESS for press.
*	The buttons and clicktypes are stored in the gre context and used
*	later to identify the events and disable them.
*	For all different buttonmasks the events is also enabled for the 
*	LockMask.
*
*  SG 23.02.91 : Because of the algorithm for interpreting the receive
*  neted events in gre_neted_click it is necessary to enable the click event
*  even if you are only interessed in double click event.
***************************************************************************/

void	gre_setup_window(
	gre_ctx		grectx,
	unsigned long	zoom_mask,
	unsigned long	scroll_mask,
	unsigned long	concreate_mask,
	unsigned long	nodemoved_mask,
	unsigned long	regionsel_mask,
	unsigned long	nodecreate_mask,
	unsigned long	nodecreate_click,
	unsigned long	nodesel_mask,
	unsigned long	nodesel_click,
	unsigned long	nodeseladd_mask,
	unsigned long	nodeseladd_click,
	unsigned long	delete_mask,
	unsigned long	delete_click,
	unsigned long	cut_mask,
	unsigned long	cut_click,
	unsigned long	copy_mask,
	unsigned long	copy_click,
	unsigned long	paste_mask,
	unsigned long	paste_click,
	unsigned long	attribute_mask,
	unsigned long	attribute_click,
	unsigned long	subwindow_mask,
	unsigned long	subwindow_click,
	unsigned long	reserv_mask,
	unsigned long	reserv_click,
	unsigned long	popupmenu_mask,
	unsigned long	popupmenu_click,
	unsigned long	getobj_mask,
	unsigned long	getobj_click,
	unsigned long	help_mask,
	unsigned long	help_click,
	unsigned char	enterleave_flag
)
{
}

/*************************************************************************
*
* Name:		void	gre_disable_button_events ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx 	grectx		I	gre context.
*
* Description:
*	Disables all events in the netedwidget.
*	The id of these events is given by the grectx
* Author: SG 29.04.91
**************************************************************************/


void gre_disable_button_events (
    gre_ctx grectx 
)
{
}

/*************************************************************************
*
* Name:		void	gre_create_node()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx 	grectx		I	gre context.
* pwr_tClassId  class		I	class for the node to be created.
* float		x		I	x-koordinate for the node.
* float		y		I	y-koordinate for the node.
*
* Description:
*	Creates a node of specified class.
*	A node is created in vldh and drawn in the window.
*
**************************************************************************/

int	gre_create_node( 
	gre_ctx	grectx,
	pwr_tClassId  class,
	float		x,
	float		y,
	vldh_t_node	*node
)
{
	flow_tNodeClass	node_class;
	vldh_t_node 	node_object;
	double		ll_x, ll_y, ur_x, ur_y;
	unsigned long	subwindowmark = 0;
	int		sts;

	sts = vldh_node_create( grectx->window_object, class, &node_object);
	if( EVEN(sts) ) return sts;

	sts = gre_get_nodeclass( grectx, 
		class,
		(node_object->hn.window_pointer)->hw.ldhsession, 
		node_object->ln.object_type, 
		node_object->ln.mask,
		subwindowmark, 
		node_object->ln.nodewidth,
		&node_class, node_object);
	if( EVEN(sts) ) return sts;

	DEFERRED_UPDATE

	flow_CreateNode( grectx->flow_ctx, node_object->hn.name, 
		node_class, x, y,
		node_object, &node_object->hn.node_id);
	flow_MeasureNode( node_object->hn.node_id,
		&ll_x, &ll_y, &ur_x, &ur_y);
	node_object->ln.width = ur_x - ll_x;
	node_object->ln.height = ur_y - ll_y;
	flow_GetNodePosition( node_object->hn.node_id, &ll_x, &ll_y);
 	node_object->ln.x = ll_x;
	node_object->ln.y = ll_y;

	sts = gre_node_annotations_draw( grectx, node_object, 0); 
	if( EVEN(sts) ) return sts;

	UPDATE_SCREEN

	*node = node_object;
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_create_con()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx 	grectx		I	gre context.
* pwr_tClassId  class		I	class of connection.	
* vldh_t_node 	source_obj	I	source node.	
* unsigned long	source_point;	I	connection point on source node.	
* unsigned long	destination_obj	I	destination node.
* unsigned long	destination_point I	connection point on dest node.
* unsigned long	drawtype	I	connections drawn as a reference
*					node or not (GOEN_CONUSERREF or
*					GOEN_CONDRAW).
*
* Description:
*	Creates a connections of the specified class.
*	Create a connection in vldh and draw a connection in the window
*	between the source and destination nodes. The user can specify
*	if the connections should be drawn as a referens connection.
*
**************************************************************************/

int	gre_create_con(
	gre_ctx	grectx,
	pwr_tClassId  class,
	vldh_t_node 	source_obj,
	unsigned long	source_point,
	vldh_t_node 	destination_obj,
	unsigned long	destination_point,
	unsigned long	drawtype
)
{
	vldh_t_con 	con_object;
	int		sts;

	sts= vldh_con_create(grectx->window_object,class, drawtype, 
		source_obj,source_point,
		destination_obj,destination_point,&con_object);
	if ( EVEN(sts)) return sts;

	sts = goec_con_draw( grectx, con_object, GRE_CON_CREATE, 0);
	if( EVEN(sts)) return sts;

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_print_docobj()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node doc_obj		I	vldh node of class document.
*
* Description:
*	Prints an area described by a documentation object in a ddif file.
*	Converts the ddif file to postscript and sends it to suitable que.
* 
* SG 17.04.91 : 
* 1: PB with convert that crashes somtimes. Fixed(?) by changing
* the dcl command that execute the convert.
*2: improve the speed for the execution of the subprocess.
**************************************************************************/

int	gre_print_docobj( 
	gre_ctx	grectx,
	vldh_t_node doc_obj
)
{
	char filename[120];
	double ll_x, ll_y, ur_x, ur_y;
	static char	cmd[250] ;
#if defined OS_VMS
	static $DESCRIPTOR(cmd_desc,cmd);
        unsigned long cli_flag = CLI$M_NOWAIT  ;
#endif

	flow_MeasureNode( doc_obj->hn.node_id, &ll_x, &ll_y, &ur_x, &ur_y);
	sprintf( filename, "pwrp_tmp:pssdoc%s.ps", 
		vldh_IdToStr(0, doc_obj->ln.object_did));
	dcli_translate_filename( filename, filename);
	flow_PrintRegion( grectx->flow_ctx, ll_x, ll_y, ur_x, ur_y, filename);

#if defined OS_VMS
        sprintf( cmd, "@pwr_exe:wb_gre_print %s", filename );
	gre_spawn( cmd);
#else
	dcli_translate_filename( cmd, "$pwr_exe/wb_gre_print.sh");
	strcat( cmd, " ");
	strcat( cmd, filename);
        system( cmd);
#endif
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_measure_object()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		ll_x		I	x coordinate lower left
* float		ll_y		I	y coordinate lower left
* float		ur_x		I	x coordinate upper right
* float		ur_y		I	y coordinate upper right
*
* Description:
*	Prints an area described by the coordinates.
**************************************************************************/

int	gre_measure_object(
	vldh_t_node	node,
	float		*ll_x,
	float		*ll_y,
	float		*width,
	float		*height
)
{
	*ll_x = node->ln.x;
	*ll_y = node->ln.y;
	*width = node->ln.width;
	*height = node->ln.height;

	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		void	gre_print_rectangel()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		ll_x		I	x coordinate lower left
* float		ll_y		I	y coordinate lower left
* float		ur_x		I	x coordinate upper right
* float		ur_y		I	y coordinate upper right
*
* Description:
*	Prints an area described by the coordinates.
**************************************************************************/

int gre_print_rectangle(
	gre_ctx	grectx,
	float	ll_x,
	float	ll_y,
	float	ur_x,
	float	ur_y,
	char	*file_id
)
{
	char filename[120];
	char	cmd[250] ;

	sprintf( filename, "pwrp_tmp:pssdoc%s.ps", file_id);
	dcli_translate_filename( filename, filename);
	flow_PrintRegion( grectx->flow_ctx, ll_x, ll_y, ur_x, ur_y, filename);

#if defined OS_VMS
        sprintf( cmd, "@pwr_exe:wb_gre_print %s", filename );
	gre_spawn( cmd);
#else
	dcli_translate_filename( cmd, "$pwr_exe/wb_gre_print.sh");
	strcat( cmd, " ");
	strcat( cmd, filename);
        system( cmd);
#endif
	return GRE__SUCCESS;

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_paste()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		cursor_x	I	x-koordinate of paste event.
* float		cursor_y	I	y-koordinate of paste event.
* int		paste_type	I	GRE_PASTE_CENTER draws an paste
*					rectangle in the center of the display
*					window ( paste callback from menu).
*					GRE_PASTE_CURSOR draws the rectangle
*					at the given koordinates ( paste
*					callback from button event).
*
* Description:
*	This routine is called when a paste callback is recieved.
*	Initiates paste by drawing a paste rectangle in the window.
*	When a node moved callback from this rectangle occurs the 
*	objects will be pasted, se routine gre_neted_node_moved.
*	
* SG 20.03.91  Return from this routine if there is nothing to paste
**************************************************************************/

void gre_paste (
    gre_ctx	grectx,
    float	cursor_x,
    float	cursor_y,
    int		paste_type
)
{
	vldh_t_node 	*node_list;
	vldh_t_node 	*node_ptr;
	unsigned long	node_count;
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	con_count;
	int		i, j;
	flow_tNode	node_id;
	flow_tNodeClass	node_class;
	flow_tConClass	con_class;
	int		sts;
	double		x[10], y[10];

	flow_PasteClear( grectx->flow_ctx);

	/* Copy objects from vldh paste */
	sts = vldh_paste_copy( grectx->window_object, 0, 0, 
		&node_count, &node_list, &con_count, &con_list);
	if ( EVEN(sts)) return;

        if ( node_count == 0 && con_count == 0) {
          /* Paste buffer is empty */
          gre_message ( grectx, "Nothing to paste");
          return;
        }

	/* Create the objects in flow paste list */
	node_ptr = node_list;
	for ( i = 0; i < node_count; i++)
	{
	  sts = gre_get_nodeclass( grectx, 
		(*node_ptr)->ln.classid,
		((*node_ptr)->hn.window_pointer)->hw.ldhsession, 
		(*node_ptr)->ln.object_type,
		(*node_ptr)->ln.mask, (*node_ptr)->ln.subwindow,
		(*node_ptr)->ln.nodewidth,
		&node_class, *node_ptr);
	  if ( EVEN(sts)) return;

          flow_CreatePasteNode( grectx->flow_ctx, (*node_ptr)->hn.name, 
		node_class,
		(*node_ptr)->ln.x,
		(*node_ptr)->ln.y,
		*node_ptr, &node_id);
	  (*node_ptr)->hn.node_id = node_id;

	  sts = gre_node_annotations_draw( grectx, *node_ptr, 1); 
	  if( EVEN(sts)) return;
	  node_ptr++;
	}

	/* Create the neted connections */
	con_ptr = con_list;
	for ( i = 0; i < con_count; i++)
	{
	  if ( (*con_ptr)->lc.drawtype == GOEN_CONSYSREF ||
	       (*con_ptr)->lc.drawtype == GOEN_CONUSERREF)
	    /* This is a fix for backward compatibility */
	    sts = gre_get_conclass( grectx,
                        0,
                        ((*con_ptr)->hc.window_pointer)->hw.ldhsession,
                        (*con_ptr)->lc.object_type,
                        &con_class);
	  else
	    sts = gre_get_conclass( grectx,
                        (*con_ptr)->lc.classid,
                        ((*con_ptr)->hc.window_pointer)->hw.ldhsession,
                        (*con_ptr)->lc.object_type,
                        &con_class);

	  for ( j = 0; j < (*con_ptr)->lc.point_count; j++)
	  {
	    x[j] = (*con_ptr)->lc.point[j].x;
	    y[j] = (*con_ptr)->lc.point[j].y;
	  }
	  flow_CreatePasteCon( grectx->flow_ctx, (*con_ptr)->hc.name, con_class, 
	  	(*con_ptr)->hc.source_node_pointer->hn.node_id,
		(*con_ptr)->hc.dest_node_pointer->hn.node_id,
		(*con_ptr)->lc.source_point, (*con_ptr)->lc.dest_point,
		*con_ptr, &(*con_ptr)->hc.con_id, (*con_ptr)->lc.point_count,
		x, y);
	  con_ptr++;
	}

	flow_Paste( grectx->flow_ctx);
        flow_SetSelectHighlight( grectx->flow_ctx);

}


/*************************************************************************
*
* Name:		void	gre_moveregion()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	This routine is called when a move retion callback is recieved.
*	Initiates move region by drawing a move region rectangle in the window.
*	The geometry of the move region rectangle is determined by the last
*	region selection.
*	When a node moved callback from this rectangle occurs the 
*	objects will be moved, se routine gre_neted_node_moved.
*	
*
**************************************************************************/

int	gre_moveregion( 
    gre_ctx	grectx
)
{
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_subwindow_mark()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	object		I	vldh node.
*
* Description:
*	Redraw a node when a subwindow i created or deleted with the
*	new subwindow mark.
*
**************************************************************************/

int gre_subwindow_mark (
	gre_ctx	grectx,
	vldh_t_node object
)
{
	flow_tNodeClass	node_class;
	flow_tNode	node_id;
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	    con_count;
	int		j;
	int		sts;
	int	highlight_flag;

	/* Check if node is highligted */
	flow_GetHighlight( object->hn.node_id, &highlight_flag);

	/* Delete current node and connections */
	flow_DeleteNode( object->hn.node_id);

	/* Create a new node with subwindow mark */
	sts = gre_get_nodeclass( grectx, 
		object->ln.classid, 
		(object->hn.window_pointer)->hw.ldhsession, 
		object->ln.object_type, 
		object->ln.mask,
		object->ln.subwindow,
		object->ln.nodewidth,
		&node_class, object);
	if (EVEN(sts)) return sts;

	flow_CreateNode( grectx->flow_ctx, object->hn.name, node_class,
			object->ln.x, object->ln.y,
			(void **) object, &node_id);
	object->hn.node_id = node_id;
	sts = gre_node_annotations_draw( grectx, object, 0); 
	if (EVEN(sts)) return sts;

	/* Set permanent highlight if selected */
	if ( highlight_flag )
	{
	  flow_SetHighlight( object->hn.node_id, highlight_flag);
          flow_SelectInsert( grectx->flow_ctx, object->hn.node_id);
	}

	/* redraw the connections */
	grectx->del_con_count = 0;
	grectx->del_node_count = 0;
	sts = vldh_get_cons_node( object, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	con_ptr = con_list;
	for ( j = 0; j < con_count; j++)
	{
	  sts = goec_con_draw( grectx, *con_ptr, GRE_CON_NONROUTE, 0);
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) con_list);

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_node_update_points()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	object		I	vldh node.
*
* Description:
*	Identifies the new point nr for the connections after a 
*	modifikation of the input or outputmask.
*	Returns an array (point_array) where
*	new_point_nr = point_array[old_point_nr]
***********************************************************************/

static pwr_tStatus	gre_node_update_points( 
	vldh_t_node	node,
	unsigned long	old_input_mask, 
	unsigned long	old_output_mask,
	unsigned long	new_input_mask,
	unsigned long	new_output_mask,
	int		*point_array,
	int		*point_count
)
{
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	int			inputs;
	int			outputs;
	unsigned long		pointmask;
	int			new_point_count;
	int			*point_array_ptr;
	int			i;

	sts = ldh_GetClassBody( (node->hn.window_pointer)->hw.ldhsession,
			node->ln.classid, "GraphPlcNode",
			&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts)) return sts;

	inputs = graphbody->parameters[PAR_INPUT];
	outputs = graphbody->parameters[PAR_OUTPUT];

	pointmask = 1;
	*point_count = 0;
	new_point_count = 0;
	point_array_ptr = point_array;
	/* Input points */
	for ( i = 0; i < inputs; i++)
	{
	  if (old_input_mask & pointmask)
	  {
	    if ( new_input_mask & pointmask)
	      /* Point should be converted */
	      *point_array_ptr = new_point_count;
	    else
	      /* Point is made invisible */
	      *point_array_ptr = -1;
	    point_array_ptr++;
	    (*point_count)++;
	  }
	  if ( new_input_mask & pointmask)
	    new_point_count++;
	  pointmask <<= 1;
	}
	/* Output points */
	pointmask = 1;
	for ( i = 0; i < outputs; i++)
	{
	  if (old_output_mask & pointmask)
	  {
	    if ( new_output_mask & pointmask)
	      /* Point should be converted */
	      *point_array_ptr = new_point_count;
	    else
	      /* Point is made invisible */
	      *point_array_ptr = -1;
	    point_array_ptr++;
	    (*point_count)++;
	  }
	  if ( new_output_mask & pointmask)
	    new_point_count++;
	  pointmask <<= 1;
	}
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_node_update()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	object		I	vldh node.
*
* Description:
*	Updates a node width all modifications made in the ldh object.
*	- the width of the node is change dependent on the lengt of the 
*	  first annotation.
*	- the graphmask is changed, inputs or outputs is added or taken
*	  away.
*	- the inversemask i changed, inputs are inverted or reinverted.
*	- data affekting the annotaions is changed.
*
**************************************************************************/

int gre_node_update (
	gre_ctx	grectx,
	vldh_t_node object
)
{
	flow_tNodeClass	node_class;
	flow_tNode	node_id;
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	con_count;
	int		i,j;
	int		sts;
	int		highlight_flag;
	int		size;
	pwr_sPlcNode	*nodebuffer;
	pwr_eClass	class;
	int		nodewidth_changed;
	int		graphmask_changed;
	int		invertmask_changed;
	double		ll_x, ll_y, ur_x, ur_y;
	int		point_conv[ VLDH_MAX_CONPOINTS];
	int		point_conv_count;
	int		point;
	int		deleted_connections;
	char		message[80];

        if ( ! object->hn.node_id)
          /* Backcall from ldh, object is not yet created */
	  return GRE__SUCCESS;


	/* Do some class specific updates */
	vldh_node_update_spec( object);

	/* Check if node is highlighted */
	flow_GetHighlight( object->hn.node_id, &highlight_flag);

	/* Get the object name from ldh */
	sts = ldh_ObjidToName( (object->hn.window_pointer)->hw.ldhsession, 
		object->ln.object_did, ldh_eName_Object,
		object->hn.name, sizeof( object->hn.name), &size);
	if( EVEN(sts)) return sts;

	/* Check if there is new nodewidth */
	nodewidth_changed = TRUE;	  

	/* Get the ldh graphbuffer of the node */
	sts = ldh_GetObjectBuffer( (object->hn.window_pointer)->hw.ldhsession,
		object->ln.object_did, "DevBody", "PlcNode", &class,	
		(char **)&nodebuffer, &size);
	if( EVEN(sts)) return sts;

	/* Check if the mask is changed */
	if( (nodebuffer->mask[0] != object->ln.mask[0]) ||
	    (nodebuffer->mask[1] != object->ln.mask[1]))
	{
	  graphmask_changed = TRUE;
	  sts = gre_node_update_points( object,
		object->ln.mask[0], object->ln.mask[1],
		nodebuffer->mask[0], nodebuffer->mask[1],
		point_conv, &point_conv_count);
	  if ( EVEN(sts)) return sts;
	}
	else
	  graphmask_changed = FALSE;
 
	if ( (object->ln.object_type == 11) || (object->ln.object_type >= 250))
	  /* Always new node type for text objects */
	  nodewidth_changed = TRUE;

	/* Check if invertmask is changed */
	if ( nodebuffer->mask[2] != object->ln.mask[2] )
	  invertmask_changed = TRUE;
	else
	  invertmask_changed = FALSE;

	object->ln.mask[0] = nodebuffer->mask[0];
	object->ln.mask[1] = nodebuffer->mask[1];
	object->ln.mask[2] = nodebuffer->mask[2];

	if ( nodewidth_changed || graphmask_changed || invertmask_changed )
	{
	  /* The node has to be redrawn */
	  /* For the moment delete all connections and redraw the node */
	  sts = vldh_get_cons_node( object, &con_count, &con_list);
	  if ( EVEN(sts)) return sts;
	  con_ptr = con_list;
	  for ( i = 0; i < con_count; i++)
  	  {
	    goec_con_delete_noredraw( grectx, *con_ptr);
	    (*con_ptr)->lc.point_count = 0;
	    (*con_ptr)->hc.redrawn = 0;
	    con_ptr++;
	  }
 
	  DEFERRED_UPDATE

	  /* Delete current node */
	  flow_DeleteNode( object->hn.node_id);

	  /* Create a new node */
	  sts = gre_get_nodeclass( grectx, 
		object->ln.classid, 
		(object->hn.window_pointer)->hw.ldhsession, 
		object->ln.object_type, 
		object->ln.mask,
		object->ln.subwindow,
		object->ln.nodewidth,
		&node_class, object);
	  if (EVEN(sts)) return sts;

	  /* Create the new node on the old place */	
	  flow_CreateNode( grectx->flow_ctx, object->hn.name,
			node_class,
			object->ln.x,
			object->ln.y,
			(void **) object,
			&node_id);
	  object->hn.node_id = node_id;

	  /* If the name would be changed, copy it to vldh and redraw the
	   annotaions */
	  sts = gre_node_annotations_draw( grectx, object, 0);
	  if (EVEN(sts)) return sts;

 	  flow_MeasureNode( node_id, &ll_x,&ll_y,&ur_x,&ur_y);
	  object->ln.width = ur_x - ll_y;
	  object->ln.height = ur_y - ll_x;


	  if ( !graphmask_changed )
	  {
	    /* redraw the connections */
	    grectx->del_con_count = 0;
	    grectx->del_node_count = 0;
	    con_ptr = con_list;
	    for ( j = 0; j < con_count; j++)
	    {
	      sts = goec_con_draw( grectx, *con_ptr, GRE_CON_CREATE, 0);
	      con_ptr++;
	    }
/*
	    sts = gre_redraw_cons( grectx, object);
	    if ( EVEN(sts)) return sts;
*/
	  }
	  else
	  {
	    /* redraw the connections */
	    grectx->del_con_count = 0;
	    grectx->del_node_count = 0;
	    con_ptr = con_list;
	    deleted_connections = 0;
	    for ( j = 0; j < con_count; j++)
	    {
	      /* Convert connection point */
	      if ( (*con_ptr)->hc.source_node_pointer == object)
	      {
		point = point_conv[(*con_ptr)->lc.source_point];
	        if ( point != (*con_ptr)->lc.source_point && point != -1)
	        {
	          vldh_node_con_delete( object, (*con_ptr)->lc.source_point, 
			*con_ptr);
		  (*con_ptr)->lc.source_point = point;
	          vldh_conmodified( *con_ptr);
		  vldh_node_con_insert( object, (*con_ptr)->lc.source_point, 
			*con_ptr, VLDH_NODE_SOURCE);
		}
	      }
	      else
	      {
		point = point_conv[(*con_ptr)->lc.dest_point];
	        if ( point != (*con_ptr)->lc.dest_point && point != -1)
	        {
	          vldh_node_con_delete( object, (*con_ptr)->lc.dest_point, 
			*con_ptr);
		  (*con_ptr)->lc.dest_point = point;
	          vldh_conmodified( *con_ptr);
		  vldh_node_con_insert( object, (*con_ptr)->lc.dest_point, 
			*con_ptr, VLDH_NODE_DESTINATION);
		}
	      }
	      if ( point == -1)
	      {
	        /* Delete the connection */
	        vldh_con_delete(*con_ptr);
	        deleted_connections++;
	      }
/*
	      else
	        sts = goec_con_draw( grectx, *con_ptr, GRE_CON_CREATE, 0);
*/
	      con_ptr++;
	    }
	    sts = vldh_get_cons_node( object, &con_count, &con_list);
	    if ( EVEN(sts)) return sts;
	    con_ptr = con_list;
	    for ( i = 0; i < con_count; i++)
  	    {
	      sts = goec_con_draw( grectx, *con_ptr, GRE_CON_CREATE, 0);
	      con_ptr++;
	    }
/*
	    sts = gre_redraw_cons( grectx, object);
	    if ( EVEN(sts)) return sts;
*/
	  }
/*	  else if ( (object->ln.object_type == 11) || (object->ln.object_type >= 250))
	  {
	    grectx->del_con_count = 0;
	    grectx->del_node_count = 0;
	    con_ptr = con_list;
	    for ( j = 0; j < con_count; j++)
	    {
	      sts = goec_con_draw( grectx, *con_ptr, GRE_CON_CREATE, 0);
	      con_ptr++;
	    }
	  }
*/
	  if ( con_count > 0) XtFree((char *) con_list);

	  UPDATE_SCREEN
	}
	free((char *) nodebuffer);

	/* Set permanent highlight if selected */
	if ( highlight_flag )
	{
	  flow_SetHighlight( object->hn.node_id, highlight_flag);
          flow_SelectInsert( grectx->flow_ctx, object->hn.node_id);
	}

	vldh_nodemodified( object);

	if ( graphmask_changed && deleted_connections )
	{
	  sprintf( message, "Warning, %d connections removed", 
			deleted_connections);
          gre_message ( grectx, message);
	  BEEP;
	}
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		void	gre_zoom()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* float		zoom		I	zoom factor.
*
* Description:
*	Zoom the display window.
*
**************************************************************************/

void gre_zoom (
	gre_ctx	grectx,
	float	zoom
)
{
	flow_Zoom( grectx->flow_ctx, zoom);
}


/*************************************************************************
*
* Name:		void	gre_unzoom()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Zoom the display window to initial zoom degree.
*
**************************************************************************/

void gre_unzoom (
	gre_ctx	grectx
)
{
	flow_UnZoom ( grectx->flow_ctx);
}



/*************************************************************************
*
* Name:		gre_display()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
* char		*string		I 	string to display.
*
* Description:
*	Enable view of display rectangle.
*
**************************************************************************/

int	gre_display(
	gre_ctx		grectx
)
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_attr.display_level = flow_mDisplayLevel_1 | flow_mDisplayLevel_2;
  mask = flow_eAttr_display_level;
  flow_SetAttributes( grectx->flow_ctx, &flow_attr, mask);
  flow_Redraw( grectx->flow_ctx);
  return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		gre_set_display_value()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
* char		*string		I 	string to display.
*
* Description:
*	Creates a display rectangle on a node and displays the string.
*	Max length of string is 5 characters.
*	If the rectangle exists the string is changed.
*
**************************************************************************/

int	gre_set_display_value(
	gre_ctx		grectx,
	vldh_t_node	node,
	char		*string
)
{
	/* Set display annotation */
        flow_SetAnnotation( node->hn.node_id, GOEN_DISPLAYNODE_ANNOT, 
		string, strlen(string));
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	gre_undisplay()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context.
*
* Description: 
*	Take away the display of execute order.
*
**************************************************************************/

int gre_undisplay (
	gre_ctx grectx
)
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_attr.display_level = flow_mDisplayLevel_1;
  mask = flow_eAttr_display_level;
  flow_SetAttributes( grectx->flow_ctx, &flow_attr, mask);
  flow_Redraw( grectx->flow_ctx);
  return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	gre_redraw_cons()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context.
*
* Description: 
*	Redraw the connections connected to a node.
*
**************************************************************************/

#if 0
static int gre_redraw_cons(
	gre_ctx 	grectx,
	vldh_t_node 	node
)
{
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	con_count;
	int		sts, j;

	  /* Reroute or redraw connections */
	  sts = vldh_get_cons_node( node, &con_count, &con_list);
	  if ( EVEN(sts)) return sts;
	  con_ptr = con_list;
	  for ( j = 0; j < con_count; j++)
	  {
	    (*con_ptr)->lc.point_count = 0;	/* tillagg RN */
	    (*con_ptr)->hc.redrawn = 0;
	    con_ptr++;
	  }

	  con_ptr = con_list;
	  for ( j = 0; j < con_count; j++)
	  {
	    sts = goec_con_draw( grectx, *con_ptr, 
		GRE_CON_REDRAW | GRE_CON_NODEREDRAW, node);
	    con_ptr++;
	  }
	  if ( con_count > 0) XtFree((char *) con_list);

	  return GRE__SUCCESS;
}
#endif


/*************************************************************************
*
* Name:		int	gre_node_select()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description: 
*	Select a node.
*
**************************************************************************/

int	gre_node_select(
	gre_ctx		grectx,
	vldh_t_node	node
)
{
	flow_SelectInsert( grectx->flow_ctx, node->hn.node_id);
	flow_SetHighlight( node->hn.node_id, 1); 
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	gre_node_unselect()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description: 
*	Unselect a node.
*
**************************************************************************/

int	gre_node_unselect(
	gre_ctx		grectx,
	vldh_t_node	node
)
{
	flow_SelectRemove( grectx->flow_ctx, node->hn.node_id);
	flow_SetHighlight( node->hn.node_id, 0); 
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		int	gre_center_node()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context.
* vldh_t_node	node		I	vldh node.
*
* Description: 
*	Center a node in display window.
*
**************************************************************************/

int	gre_center_node( 
	gre_ctx		grectx,
	vldh_t_node	node
)
{
	flow_CenterObject( grectx->flow_ctx, node->hn.node_id);
	return GRE__SUCCESS;
}



/*************************************************************************
*
* Name:		void	gre_init_docobjects()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Prints time, plcprogram, window, system and page in the
*	document objects in a window.
**************************************************************************/

int	gre_init_docobjects(
    gre_ctx	grectx
)
{
	int	sts, size;
	vldh_t_plc	plc;
	char	timstr[32];
	char	annot_str[16];
	char	systemname[80];
	char	systemgroup[80];
	char	plcname[80];
	char	windname[80];
	char	*windname_ptr;
	char	short_windname[80];
	int	plclen;
	vldh_t_wind		wind;
	int			doc_count;
	unsigned long		node_count;
	vldh_t_node		doc_obj;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i;
	char			*parvalue;

	wind = grectx->window_object;
	plc = wind->hw.plcobject_pointer;

	/* Get the system name */
	sts = utl_get_systemname( systemname, systemgroup);

	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	doc_count = 0;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
  	{
	  if ( vldh_check_document( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did))
	  {
	    doc_obj = *node_ptr;
	    doc_count++;	  

	    /* System name in annotation nr 0 */
            flow_SetAnnotation( doc_obj->hn.node_id, 0, systemname, 
		strlen(systemname));

	    /* Plcname in annot 1 */
	    sts = ldh_ObjidToName( wind->hw.ldhsession, 
	         	plc->lp.objdid, ldh_eName_Hierarchy,
		        plcname, sizeof( plcname), &size);
	    if ( EVEN(sts)) annot_str[0] = '\0';

            flow_SetAnnotation( doc_obj->hn.node_id, 1, plcname, 
		strlen(plcname));

	    /* Windowname in annot 2 */

	    sts = ldh_ObjidToName( wind->hw.ldhsession, 
	         	wind->lw.objdid, ldh_eName_Hierarchy,
		        windname, sizeof( windname), &size);
	    if ( EVEN(sts)) annot_str[0] = '\0';

	    /* Take away the plcpart och windowname before printing it */
	    plclen = strlen( plcname);
	    windname_ptr = windname;
	    strcpy( short_windname, windname_ptr + plclen + 1);

            flow_SetAnnotation( doc_obj->hn.node_id, 2, short_windname, 
		strlen(short_windname));

	    /* Page in annotations nr 3 */
	    sts = ldh_GetObjectPar( wind->hw.ldhsession,  
			doc_obj->ln.object_did, 
			"DevBody", "Page",
			(char **)&parvalue, &size); 
	    if ( ODD(sts))
	    {
              flow_SetAnnotation( doc_obj->hn.node_id, 3, parvalue, 
		strlen(parvalue));
	      free((char *) parvalue);
	    }

	    /* Time in annot 4 */
	    time_AtoAscii(NULL, time_eFormat_DateAndTime, 
	    					timstr, sizeof(timstr));             
            timstr[strlen(timstr) - 6] = '\0'; 
            flow_SetAnnotation( doc_obj->hn.node_id, 4, timstr, 
		strlen(timstr));

	    /* Signature in annotations nr 5 */
	    sts = ldh_GetObjectPar( wind->hw.ldhsession,  
			doc_obj->ln.object_did, 
			"DevBody", "Signature",
			(char **)&parvalue, &size); 
	    if ( ODD(sts))
	    {
              flow_SetAnnotation( doc_obj->hn.node_id, 5, parvalue, 
		strlen(parvalue));
	      free((char *) parvalue);
	    }

	  }
	  node_ptr++;

	}

	if ( node_count > 0) XtFree((char *) nodelist);

	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		void	gre_redraw()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
*
* Description:
*	Redraws all nodes and connections in a window.
*
**************************************************************************/

int	gre_redraw(
	gre_ctx	grectx
)
{
	flow_Reconfigure( grectx->flow_ctx );

	/* This is a fix to update width and height which has to be done
	   after upgrade from V2.7. This fix can be removed in later versions */

	{
	  int			i;
	  double		ur_x, ur_y, ll_x, ll_y;
	  vldh_t_node		*nodelist;
	  vldh_t_node		*node_ptr;
	  int			sts;
	  vldh_t_wind		wind;
	  unsigned long		node_count;

	  wind = grectx->window_object;
	  sts = vldh_get_nodes( wind, &node_count, &nodelist);
	  if ( EVEN(sts)) return sts;

	  node_ptr = nodelist;
	  for ( i = 0; i < node_count; i++)
	  {
	    flow_MeasureNode( (*node_ptr)->hn.node_id,
		&ll_x, &ll_y, &ur_x, &ur_y);
	    (*node_ptr)->ln.width = ur_x - ll_x;
	    (*node_ptr)->ln.height = ur_y - ll_y;
	    flow_GetNodePosition( (*node_ptr)->hn.node_id, &ll_x, &ll_y);
	    (*node_ptr)->ln.x = ll_x;
	    (*node_ptr)->ln.y = ll_y;
	    vldh_nodemodified( *node_ptr);
	    node_ptr++;
	  }
	  if ( node_count > 0) XtFree((char *) nodelist);
        }

	return GRE__SUCCESS;
}

#if defined OS_VMS
static int	gre_spawn( char *command)
{
	static char	cmd[150] ;
	static $DESCRIPTOR(cmd_desc,cmd);
        unsigned long cli_flag = CLI$M_NOWAIT  ;
	int	sts;

	strcpy( cmd, command);

        sts = lib$spawn (&cmd_desc, 0, 0, &cli_flag);
	if ( sts != SS$_NORMAL)
          printf ("Error returned from lib$spawn");
	return sts;
}
#endif

int	gre_set_trace_attributes( gre_ctx grectx)
{
  flow_eTraceType 	trace_type;
  char			object_str[120];
  char			attr_str[80];
  int			sts, i, j, size;
  vldh_t_wind		wind;
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  ldh_sParDef 		*bodydef;
  int 			rows;
  pwr_tObjid		*objid;

  wind = grectx->window_object;
  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;

  node_ptr = nodelist;
  for ( i = 0; i < node_count; i++)
  {
    sts = trace_get_attributes( grectx, *node_ptr, object_str, attr_str,
		&trace_type);
    if ( ODD(sts) && sts != TRA__DISCARD )
      flow_SetTraceAttr( (*node_ptr)->hn.node_id, object_str,
		attr_str, trace_type);
    else
    {
      switch( (*node_ptr)->ln.classid )
      {
        case pwr_cClass_GetAi:
        case pwr_cClass_GetAo:
        case pwr_cClass_GetAv:
        case pwr_cClass_stoai:
        case pwr_cClass_stoao:
        case pwr_cClass_stoav:
        case pwr_cClass_cstoai:
        case pwr_cClass_cstoao:
        case pwr_cClass_cstoav:
        case pwr_cClass_GetIi:
        case pwr_cClass_GetIo:
        case pwr_cClass_GetIv:
        case pwr_cClass_stoii:
        case pwr_cClass_stoio:
        case pwr_cClass_stoiv:
        case pwr_cClass_cstoii:
        case pwr_cClass_cstoio:
        case pwr_cClass_cstoiv:
          sts = ldh_GetObjectBodyDef( wind->hw.ldhsession, 
			(*node_ptr)->ln.classid, "DevBody", 1, 
			&bodydef, &rows);
	  if ( EVEN(sts) ) return sts;
          strcpy( object_str, "");
	  for ( j = 0; j < rows; j++ )
	  {
	    if ( bodydef[j].Par->Output.Info.Type == pwr_eType_Objid)
            {
              /* Get the objid stored in the parameter */
	      sts = ldh_GetObjectPar( wind->hw.ldhsession,
			(*node_ptr)->ln.object_did,  "DevBody",
			bodydef[j].ParName, (char **)&objid, &size);
              if ( EVEN(sts)) return sts;

              sts = ldh_ObjidToName( wind->hw.ldhsession, *objid,
	        ldh_eName_Hierarchy, object_str, sizeof(object_str), &size);
              if ( EVEN(sts))
                strcpy( object_str, "");
	      free((char *) objid);
              
              break;
            }
          }
          break;
        default:
          /* Store object name */
          sts = ldh_ObjidToName( wind->hw.ldhsession, (*node_ptr)->ln.object_did,
	         ldh_eName_Hierarchy, object_str, sizeof(object_str), &size);
          if ( EVEN(sts)) return sts;
      }
      flow_SetTraceAttr( (*node_ptr)->hn.node_id, object_str,
      		"", flow_eTraceType_User);
    }
    node_ptr++;
  }
  if ( node_count > 0) XtFree((char *) nodelist);
  return GRE__SUCCESS;
}

int	gre_save( gre_ctx grectx)
{
  char filename[80];
  int sts;

  sprintf( filename, "pwrp_load:pwr_%s.flw",
		vldh_IdToStr(0, grectx->window_object->lw.objdid));
  dcli_translate_filename( filename, filename);
  sts = flow_Save( grectx->flow_ctx, filename);  
  return sts;
}

void gre_set_grid_size( gre_ctx grectx, double size)
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  grectx->grid_size = size;

  mask = 0;
  flow_attr.grid_size_x = size;
  mask |= flow_eAttr_grid_size_x;
  flow_attr.grid_size_y = size;
  mask |= flow_eAttr_grid_size_y;
  flow_SetAttributes( grectx->flow_ctx, &flow_attr, mask);
}
