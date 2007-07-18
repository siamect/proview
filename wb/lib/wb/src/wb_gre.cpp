/* 
 * Proview   $Id: wb_gre.cpp,v 1.5 2007-07-18 09:27:16 claes Exp $
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

/* wb_gre.cpp
   This module creates the flow window and handles
   the flow callbacks.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "flow_ctx.h"
#include "flow_api.h"
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
#include "wb_utl_api.h"
#include "wb_gre.h"
#include "wb_tra.h"


#define ANNOT_LEN_MAX 	1024
#define ANNOT_NR_MAX 	8


#define	CLOCK_CURSOR	

#define	NORMAL_CURSOR

#define	DEFERRED_UPDATE	

#define	UPDATE_SCREEN	
				
#define	BEEP	    putchar( '\7' );

#define GRE_MASKTST(cb, func) ((( cb->button == gre->func##_mask) ||\
	( cb->button == (gre->func##_mask | LockMask ))) &&\
	( cb->type == gre->func##_click))


#define GRE_SEARCHRECT_OFFSET 0.01

/*_Global variable______________________________________________________*/

/*_Global procedues_______________________________________________________*/

/*_Local procedues_______________________________________________________*/


//
//	Set or reset highlight on a node depending on the flag 
//	( set: 1, reset: 0).
//
int WGre::node_sethighlight( vldh_t_node node, unsigned long highlight_flag)
{
  flow_SetHighlight( node->hn.node_id, highlight_flag);
  return GRE__SUCCESS;
}

//
//	Get the highlight value of a node 
//	( set: 1, reset: 0).
//
int WGre::node_gethighlight( vldh_t_node node, unsigned long *highlight_flag)
{
  flow_GetHighlight( node->hn.node_id, (int *) &highlight_flag);
  return GRE__SUCCESS;
}

//
//	Get the annotations that should be written in the graphics 
//	for a node.
//
int WGre::get_annot_width( flow_tNodeClass nodeclass,
			   float *annot_width, char *annot_str, int annot_count,
			   int annot_size)
{
  int	       	i;
  double       	width, height;

  for ( i = 0; i < annot_count; i++) {
    flow_MeasureAnnotation( nodeclass, i, annot_str, 
			    &width, &height);
    *annot_width = width;
    annot_str += annot_size;
    annot_width++;
  }
  return GRE__SUCCESS;
}

//
//	Get the annotations that should be written in the graphics 
//	for a node.
//
int WGre::get_annotations( vldh_t_node node, char *annot_str,
			   int *annot_nr, int *annot_count, int annot_max,
			   int annot_size)
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
  pwr_sAttrRef		last_attrref;
  pwr_tBoolean		*condparvalue;
  pwr_tBoolean		*segmentsparvalue;
  int			annot_segments;
  pwr_tAName           	objid_str;
  char			*name;
  char			body[10];
  ldh_tSesContext		ldhses;
  pwr_tObjid		nullobjid = pwr_cNObjid;

  ldhses = (node->hn.wind)->hw.ldhses;
  *annot_count = 0;
  memset( &last_attrref, 0, sizeof(last_attrref));

  /* Get graphbody for this node to fetch the parameter of the 
     first annoation */
  sts = ldh_GetClassBody( ldhses,
			  node->ln.cid, "GraphPlcNode",
			  &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts)) return sts;

  node->ln.nodewidth = 0;
  if ( graphbody->segname_annotation >= 1 ) {
    strncpy( annot_str, node->hn.name, annot_size);
    *annot_nr = graphbody->segname_annotation;
    annot_str += annot_size;
    annot_nr++;
    (*annot_count)++;
  }

  if (node->ln.cid != pwr_cClass_order) {
    for ( j = 0; j < 2; j++) {
      /* Look for annotations in some parameter in devbody or rtbody */
      if ( j == 0) {
	if ( !graphbody->devbody_annotation)
	  continue;
	strcpy( body, "DevBody");
      }
      else {
	if ( !graphbody->rtbody_annotation)
	  continue;
	strcpy( body, "RtBody");
      }
      
      sts = ldh_GetObjectBodyDef( ldhses,
				  node->ln.cid, body, 1, 
				  &bodydef, &rows);
      if ( EVEN(sts)) continue;
      
      if ( EVEN(sts) ) return GRE__SUCCESS;
      
      for ( i = 0; i < rows; i++) {
	annotnr = 0;
	switch ( bodydef[i].ParClass ) {
	case pwr_eClass_Input: {
	  annotnr = bodydef[i].Par->Input.Graph.NiNaAnnot;
	  annotcond = bodydef[i].Par->Input.Graph.NiNaCond;
	  annotsegments = bodydef[i].Par->Input.Graph.NiNaSegments;
	  type = bodydef[i].Par->Input.Info.Type;
	  parname = bodydef[i].ParName;
	  break;
	}
	case pwr_eClass_Intern: {
	  annotnr = bodydef[i].Par->Intern.Graph.NiNaAnnot;
	  annotcond = bodydef[i].Par->Intern.Graph.NiNaCond;
	  annotsegments = bodydef[i].Par->Intern.Graph.NiNaSegments;
	  type = bodydef[i].Par->Intern.Info.Type;
	  parname = bodydef[i].ParName;
	  break;
	}
	case pwr_eClass_Output: {
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
	if( annotnr != 0 ) {
	  if ( annotcond) {
	    /* Get the condition if the annotation should be shown from
	       the parameter Show'parname' in devbody */
	    strcpy( condparname, "Show");
	    strcat( condparname, parname);
	    sts = ldh_GetObjectPar( ldhses,
				    node->ln.oid,
				    "DevBody",
				    condparname,
				    (char **)&condparvalue, &size);
	    if ( EVEN(sts))
	      continue;
	    if ( !*condparvalue) {
	      /* No, this parameter should node be shown this time */
	      free((char *) condparvalue);
	      continue;
	    }
	    free((char *) condparvalue);
	  }
	  
	  if ( !annotsegments)
	    annot_segments = 1;
	  else {
	    /* Get the segments of the annotation that should be shown from
	       the parameter 'parname'Segments in devbody */
	    strcpy( condparname, parname);
	    strcat( condparname, "Segments");
	    sts = ldh_GetObjectPar( ldhses,
				    node->ln.oid,
				    "DevBody",
				    condparname,
				    (char **)&segmentsparvalue, &size);
	    if ( EVEN(sts))
	      annot_segments = 1;
	    else {
	      annot_segments = *segmentsparvalue;
	      if ( annot_segments < 1)
		annot_segments = 1;
	      free((char *) segmentsparvalue);
	    }
	  }
	  
	  if ( strcmp( parname, "SigChanCon") == 0) {
	    /* Get attribute SigChanCon from previous attrref */
	    pwr_tObjid oid;
	    char *s;
	    
	    if ( cdh_ObjidIsNotNull( last_attrref.Objid)) {
	      sts = ldh_AttrRefToName( ldhses,
				       &last_attrref, ldh_eName_Hierarchy,
				       &name, &size);
	      if ( EVEN(sts)) continue;
	      
	      strcpy( objid_str, name);
	      strcat( objid_str, ".SigChanCon");
	      s = strchr( objid_str, '.');
	      *s = 0;
	      
	      sts = ldh_NameToObjid( ldhses, &oid, objid_str);
	      if ( EVEN(sts)) continue;
	      
	      sts = ldh_GetObjectPar( ldhses, oid,
				      "RtBody", s+1,
				      (char **)&parvalue, &size);
	      if ( EVEN(sts)) continue;
	    }
	    else
	      continue;
	  }
	  else {
	    /* Get the parameter value */
	    sts = ldh_GetObjectPar( ldhses,
				    node->ln.oid,
				    body,
				    bodydef[i].ParName,
				    (char **)&parvalue, &size);
	    if ( EVEN(sts)) return sts;
	  }
	  
	  
	  switch ( type ) {
	  case pwr_eType_String:
	  case pwr_eType_Text: {
	    strncpy( annot_str, parvalue, annot_size);
	    break;
	  }
	  case pwr_eType_Char: {
	    *annot_str = *parvalue;
	    *(annot_str + 1) = '\0';		
	    break;
	  }
	  case pwr_eType_Float32: {
	    sprintf( annot_str, "%g", *(float *)parvalue);
	    break;
	  }
	  case pwr_eType_ObjDId: {
	    /* Get the object name from ldh */
	    parobjdid = (pwr_tObjid *)parvalue;
	    if ( cdh_ObjidIsNull(*parobjdid))
	      strcpy( objid_str, "");
	    else if ( (*parobjdid).oix == 0 ) {
	      /* Get the volume name */
	      sts = ldh_VolumeIdToName( ldh_SessionToWB(ldhses),
					(*parobjdid).vid, objid_str, sizeof( objid_str),
					&size);
	      if ( EVEN(sts))
		objid_str[0] = 0;
	    }
	    else {
	      if ( annot_segments == 1) {
		sts = ldh_ObjidToName( ldhses,
				       *parobjdid, ldh_eName_Object,
				       objid_str, sizeof(objid_str), &size);
		if ( EVEN(sts)) 
		  objid_str[0] = 0;
	      }
	      else {
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
	    break;
	  }
	  case pwr_eType_AttrRef: {
	    /* Get the object name from ldh */
	    parattrref = (pwr_sAttrRef *)parvalue;
	    if ( cdh_ObjidIsNull(parattrref->Objid))
	      strcpy( objid_str, "");
	    else {
	      if ( parattrref->Objid.vid == ldh_cPlcMainVolume ||
		   parattrref->Objid.vid == ldh_cPlcFoVolume) {
		sts = ldh_AttrRefToName( ldhses, parattrref, 
					 ldh_eName_Ref,
					 &name, &size);
		if ( EVEN(sts)) 
		  objid_str[0] = 0;
		else
		  strcpy( objid_str, name);
	      }
	      else {
		sts = ldh_AttrRefToName( ldhses,
					 parattrref, ldh_eName_Hierarchy,
					 &name, &size);
		if ( EVEN(sts)) 
		  objid_str[0] = 0;
		else {
		  utl_cut_segments( objid_str, name, annot_segments);
		  last_attrref = *parattrref;
		}
	      }
	    }
	    strncpy( annot_str, objid_str, annot_size);
	    break;
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
  else {
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
			    node->ln.oid,
			    "DevBody",
			    "ShowAttrTime",
			    (char **)&condparvalue, &size);
    if ( EVEN(sts)) return sts;
    showattrtime = *condparvalue;
    free((char *) condparvalue);
    
    for ( i = 0; i < 6; i++) {
      sprintf( attribute, "Attr%d", i + 1);
      sts = ldh_GetObjectPar( ldhses,
			      node->ln.oid,
			      "DevBody",
			      attribute,
			      (char **)&parvalue, &size);
      if ( EVEN(sts)) return sts;
      sprintf( annot_str + strlen(annot_str), "%c", *parvalue);
      if ( showattrtime && 
	   ((*parvalue == 'D' || *parvalue == 'd') ||
	    (*parvalue == 'L' || *parvalue == 'l'))) {
	sprintf( attribute, "AttrTime%d", i + 1);
	
	if ( attribute_count != 0)
	  strcat( annot_str_next, " ");
	sprintf( annot_str_next + strlen(annot_str_next), "%c=", *parvalue);
	sts = ldh_GetObjectPar( ldhses,
				node->ln.oid,
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

//
//	Send a message to the gre_message backcallroutine.
//	( foe_gre_message )
//
void WGre::message( char *message)
{
  (gre_message)(this, message);
}

//
//  Inits the gre context.
//  SG 23.02.91 init of the timers which helps to detect double click and click
//  events .
//
void WGre::ctx_init()
{
  int	i,j;
	
  for ( i = 0; i < MAX_NUMBER_OF_OBJECTTYPES; i++ )	
    for ( j = 0; j < 4; j++ )	
      nodeclass_list[i][j] = 0;

  for ( i = 0; i < MAX_NUMBER_OF_CONTYPES; i++ )	
    conclass_list[i] = 0;
 
  sel_node_count = 0;
  sel_con_count = 0;
  grid_on = 0;
  conref_nodetypeid = 0;
  concreate_mask =0;
  nodemoved_mask =0;
  regionsel_mask =0;
  nodecreate_mask =0;
  nodesel_mask = 0;
  nodeseladd_mask = 0;
  delete_mask = 0;
  cut_mask = 0;
  copy_mask = 0;
  paste_mask = 0;
  attribute_mask = 0;
  subwindow_mask = 0;
  reserv_mask = 0;
  popupmenu_mask = 0;
  getobj_mask = 0;
  help_mask = 0;
}

//
// 	Draw one annotation in a node.
//
int WGre::modif_annot( vldh_t_node node, int annotation, char *str)
{
  flow_SetAnnotation( node->hn.node_id, annotation, str, strlen(str));
  return GRE__SUCCESS;
}

//
//	Draw the annotations in a node.
//	If segname_annotation (in graphbody) is not zero the segment name 
//	of the object is written in the annotation with nr: 
//	segname_annotation-1.
//	Every parameter in devbody that has a NiNaAnnot defined is also
//	written in the annotation with nr NiNaAnnot-1. The parameter can
//	be of type float, string, char or objdid.
//
int WGre::node_annotations_draw( vldh_t_node node, int paste)
{
  int		i, sts;
  char		annot_str[ANNOT_NR_MAX][ANNOT_LEN_MAX];
  int		annot_nr[ANNOT_NR_MAX];
  int		annot_count;
  
  /* Get the annotations of this node */
  sts = get_annotations( node,
			 (char *)annot_str, annot_nr, &annot_count,
			 sizeof(annot_str)/sizeof(annot_str[0]), sizeof( annot_str[0]));
  if ( EVEN(sts)) return sts;

  /* Draw the annotations */
  for ( i = 0; i < annot_count; i++) {
    if ( paste)
      flow_SetPasteNodeAnnotation( node->hn.node_id, annot_nr[i] - 1,
				   annot_str[i], strlen(annot_str[i]));
    else 
      flow_SetAnnotation( node->hn.node_id, annot_nr[i] - 1,
			  annot_str[i], strlen(annot_str[i]));
  }
  return GRE__SUCCESS;
}

//
//	This routine is used to get info of the annotations content of 
//	the annotaions a node.
//	Returns the three first annotaions ( the objectname is excluded)
//	and puts them in a string separated by a ','.
//
int WGre::node_annot_message( vldh_t_node node, char *message, int msg_size,
			      int annot_max_size)
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
  sts = ldh_GetClassBody( (node->hn.wind)->hw.ldhses, 
                          node->ln.cid, "GraphPlcNode",
                          &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts)) return sts;

  /* Draw the parameters in devbody that has an annotation nr */
  sts = ldh_GetObjectBodyDef((node->hn.wind)->hw.ldhses,
                             node->ln.cid, "DevBody", 1, 
                             &bodydef, &rows);

  if ( EVEN(sts) ) return GRE__SUCCESS;

  *message = '\0';
  annotcount = 0;
  for ( i = 0; i < rows; i++) {
    strcpy( annot_str, "");
    annotnr = 0;
    switch ( bodydef[i].ParClass ) {
    case pwr_eClass_Input: {
      annotnr = bodydef[i].Par->Input.Graph.NiNaAnnot;
      type = bodydef[i].Par->Input.Info.Type;
      parname = bodydef[i].ParName;
      break;
    }
    case pwr_eClass_Intern: {
      annotnr = bodydef[i].Par->Intern.Graph.NiNaAnnot;
      type = bodydef[i].Par->Intern.Info.Type;
      parname = bodydef[i].ParName;
      break;
    }
    case pwr_eClass_Output: {
      annotnr = bodydef[i].Par->Output.Graph.NiNaAnnot;
      type = bodydef[i].Par->Output.Info.Type;
      parname = bodydef[i].ParName;
      break;
    }
    default:
      ;
    }
    if( annotnr != 0) {
      /* Get the parameter value */
      sts = ldh_GetObjectPar( (node->hn.wind)->hw.ldhses,  
			      node->ln.oid, 
			      "DevBody",
			      bodydef[i].ParName,
			      (char **)&parvalue, &size); 
      if ( EVEN(sts)) return sts;
      
      switch ( type ) {
      case pwr_eType_Float32: {
	parfloat = (pwr_tFloat32 *)parvalue;
	sprintf( annot_str,"%f", *parfloat);
	break;
      }
      case pwr_eType_String:
      case pwr_eType_Text: {
	strncpy( annot_str, parvalue, 
		 co_min( (int)sizeof(annot_str), annot_max_size));
	annot_str[co_min( (int)sizeof(annot_str), annot_max_size)-1] = 0;
	break;
      }
      case pwr_eType_Char: {
	annot_str[0] = *parvalue;
	annot_str[1] = '\0';		
	break;
      }
      case pwr_eType_Objid: {
	/* Get the object name from ldh */
	parobjdid = (pwr_tObjid *)parvalue;
	if ( cdh_ObjidIsNull(*parobjdid))
	  annot_str[0] = '\0';
	else {
	  sts = ldh_ObjidToName( (node->hn.wind)->hw.ldhses, 
				 *parobjdid, ldh_eName_Object,
				 annot_str, sizeof( annot_str), &size);
	  if ( EVEN(sts)) annot_str[0] = '\0';
	}
	break;
      }
      case pwr_eType_AttrRef: {
	/* Get the object name from ldh */
	pwr_tAttrRef *arp = (pwr_tAttrRef *)parvalue;
	char *name;
	
	if ( cdh_ObjidIsNull(arp->Objid))
	  annot_str[0] = '\0';
	else {
	  sts = ldh_AttrRefToName( (node->hn.wind)->hw.ldhses,
				   arp, ldh_eName_ArefObject,
				   &name, &size);
	  if ( EVEN(sts)) annot_str[0] = '\0';
	  strcpy( annot_str, name);
	}
	break;
      }
      }
      if ( strcmp( annot_str, "") != 0) {
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

//
//	Zooms to a specified workingarea per screenmillimeter and
//	returns actual zoomfactor.
//
int WGre::zoom_absolute( float absolute_zoom, int expand, float *realized_zoom)
{
  flow_ZoomAbsolute( flow_ctx, absolute_zoom);
  *realized_zoom = absolute_zoom;

  return GRE__SUCCESS;
}

//
//	Resets permanent highlight on selected nodes and connections, and
//	resets the selectlists.
// 	Informs the user with a gre_message if there is nothing to unselect.
//
void WGre::unselect()
{
  flow_SelectClear( flow_ctx);
  sel_node_count = 0;
  sel_con_count = 0;
}


//
//	Delete the search rectangle.
//
void WGre::search_rectangle_delete()
{
  if ( searchrect_node_id != 0 ) {
    flow_SetInverse( searchrect_node_id, 0);
    searchrect_node_id = 0;
  }
}

//
//	Create rectangle in the neted window with the lower left
//	koordinates x,y and the specified width and height.
//	Returns neted node id for the created rectangle.
//
void WGre::search_rectangle_create( vldh_t_node node)
{
  flow_SetInverse( node->hn.node_id, 1);
  searchrect_node_id = node->hn.node_id;
}

#if 0
//
//	Load colors, font and cursor for the neted widget.
//	Inits the goe graph_table.
//
void WGre::create_cursors()
{
  /* Create colors and fonts for this widget */
  goen_create_cursors( flow_widget, &cursors);
}
#endif

//
//	Undeletes all nodes and connections in delete list.
// 	Informs the user by gre_message if there is nothing to undelete
//
int WGre::undelete()
{
  int	i;
  int	sts;
  flow_tNodeClass	node_class;
  flow_tNode	node_id;

  /* SG 20.03.91 if nothing has been done say it to the user */
  if ( del_con_count == 0 && del_node_count == 0 ) {
    message( "Nothing to undelete" );
    BEEP;
    return GRE__SUCCESS;
  }

  /* Restore nodes in delete_list */
  for ( i = 0; i < del_node_count; i++ ) {
    node_class = 0;
    sts = get_nodeclass( del_node_list[i]->ln.cid, 
			 del_node_list[i]->hn.wind->hw.ldhses, 
			 del_node_list[i]->ln.object_type, 
			 del_node_list[i]->ln.mask, 
			 del_node_list[i]->ln.subwindow,
			 del_node_list[i]->ln.nodewidth,
			 &node_class, del_node_list[i]);
    if( EVEN(sts) ) return sts;
    flow_CreateNode( flow_ctx, del_node_list[i]->hn.name, 
		     node_class, del_node_list[i]->ln.x,
		     del_node_list[i]->ln.y, 
		     del_node_list[i], 
		     &node_id);
    del_node_list[i]->hn.node_id = node_id;
    sts = node_annotations_draw( del_node_list[i], 0); 
    if( EVEN(sts) ) return sts;
    vldh_node_undelete( del_node_list[i]);
  }
  /* Restore connections in delete_list */
  for ( i = 0; i < del_con_count; i++ ) {
    vldh_con_undelete( del_con_list[i]);
    sts = goec_con_draw( this, del_con_list[i], GRE_CON_CREATE, 0);
  }
	
  /* Update header /CJ 050415 */ 
  init_docobjects();

  del_node_count = 0;
  del_con_count = 0;
  return GRE__SUCCESS;
}
//
//	Resets the undelete list.
//
int WGre::undelete_reset()
{
  del_node_count = 0;
  del_con_count = 0;
  return GRE__SUCCESS;
}

//
//	Insert node in delete node list.
//  	The delete node list contains all deleted nodes in the last
//	delete or cut operation.
//
void WGre::delnode_insert( vldh_t_node node_id)
{
  int	i;
  int	found;

  /* Check not aready inserted */
  found = 0;
  for ( i = 0; i < del_node_count; i++) {
    if ( del_node_list[i] == node_id ) {
      found = 1;
      break;
    }
  }
  if ( !found && (del_node_count < MAX_DEL_NODE)) {
    /* Insert node in selected node list */
    del_node_list[del_node_count] = node_id;
    del_node_count++;
  }
}

//
//	Insert connection in delete connection list.
//  	The delete connection list contains all deleted connections in the last
//	delete or cut operation.
//
void WGre::delcon_insert( vldh_t_con con_id)
{
  int	i;
  int	found;

  /* Check not aready inserted */
  found = 0;
  for ( i = 0; i < del_con_count; i++) {
    if ( del_con_list[i] == con_id ) {
      found = 1;
      break;
    }
  }
  if ( !found && (del_con_count < MAX_DEL_CON)) {
    /* Insert con in selected con list */
    del_con_list[del_con_count] = con_id;
    del_con_count++;
  }
}

//
//	Deletes selected nodes and connections and connections connected
//	to selectetd nodes. Inserts all deleted objects in deletelists.
// 	Informs the user by gre_message if there is nothing to delete.
//
void WGre::delete_selected()
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

  popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
	
  /* SG 20.03.91 if nothing has been done say it to the user */
  flow_GetSelectedNodes( flow_ctx, &fnode_list, &fnode_count);
  flow_GetSelectedCons( flow_ctx, &fcon_list, &fcon_count);
  if ( fnode_count == 0 && fcon_count == 0 ) {
    message( "Nothing to delete");
    BEEP;
    return;
  }

  /* Delete nodes */
  /* Get all connections connected to every node and delete it */
  for ( i = 0; i < fnode_count; i++ ) {
    flow_GetUserData( fnode_list[i], (void **)&node);

    /* delete connected connections */
    sts = vldh_get_cons_node( node, &con_count, &con_list);
    con_ptr = con_list;
    for ( j = 0; j < (int)con_count; j++) {
      vldh_con_delete(*con_ptr);
      goec_con_delete( this, *con_ptr);
      delcon_insert( *con_ptr);
      con_ptr++;
    }
    if ( con_count > 0) free((char *) con_list);
    /* delete the node */
    vldh_node_delete( node);
    flow_DeleteNode( fnode_list[i]);
    
    delnode_insert( node);
  }
  if ( fcon_count > 0)
    free( fcon_list);
  if ( fnode_count > 0)
    free( fnode_list);

  /* delete the remaining selected connections */
  flow_GetSelectedCons( flow_ctx, &fcon_list, &fcon_count);
  for ( i = 0; i < fcon_count; i++ ) {
    flow_GetUserData( fcon_list[i], (void **)&con);
    if ( ( con->hc.status & VLDH_DELETE) == 0 ) {
      /* delete the con */
      vldh_con_delete( con);
      goec_con_delete( this, con);
      delcon_insert( con);
    }
  }
  if ( fcon_count > 0)
    free( fcon_list);

  sel_con_count = 0;
  sel_node_count = 0;
}

//
//	Deletes a single node. All connections connected to the node
//	are also deleted. The deleted objects are
//	inserted in deletelist.
//
int WGre::delete_node( vldh_t_node node)
{
  vldh_t_con  	*con_list;
  vldh_t_con  	*con_ptr;
  unsigned long	con_count;
  int		j;
  int		sts;

  del_con_count = 0;
  del_node_count = 0;

  /* Get all connections connected to the node and delete it */
  sts = vldh_get_cons_node( node, &con_count, &con_list);
  if ( EVEN(sts)) return sts;
  con_ptr = con_list;
  for ( j = 0; j < (int)con_count; j++) {
    vldh_con_delete(*con_ptr);
    goec_con_delete( this, *con_ptr);
    delcon_insert( *con_ptr);
    con_ptr++;
  }
  if ( con_count > 0) free((char *) con_list);

  /* delete the node */
  vldh_node_delete( node);
  flow_DeleteNode( node->hn.node_id);
  delnode_insert( node);
  
  return GRE__SUCCESS;
}

//
//	Deletes a single connection. 
//	The deleted connections is inserted in deletelist.
//
void WGre::delete_con( vldh_t_con con)
{
  del_con_count = 0;
  del_node_count = 0;

  vldh_con_delete( con);
  goec_con_delete( this, con);
  delcon_insert( con);
}

//
//	Inserts the paste signelnode in the current window.
//
int WGre::paste_node_exec( float x, float y)
{
  return GRE__SUCCESS;
}


//
//	This routine deletes all selected objects and makes it possible
//	to recover them by undelete or paste.
//	Deletes selected nodes and connections and connections connected
//	to selectetd nodes. Inserts all deleted objects in deletelist and
//	in the vldh_paste list.
//
int WGre::cut()
{
  int		i;
  vldh_t_con  	con;
  vldh_t_node	node;
  flow_tNode	*fnode_list;
  flow_tCon	*fcon_list;
  int		fnode_count;
  int		fcon_count;

  /* Copy selected objects to pastelist */
  flow_GetSelectedNodes( flow_ctx, &fnode_list, &fnode_count);
  flow_GetSelectedCons( flow_ctx, &fcon_list, &fcon_count);
  if ( fnode_count == 0 && fcon_count == 0 ) {
    message( "Nothing to cut" );
    BEEP;
    return GRE__SUCCESS;
  }

  /* Insert in vldh_pastelist */
  vldh_paste_init();

  /* Insert the nodes */
  for ( i = 0; i < fnode_count; i++ ) {
    flow_GetUserData( fnode_list[i], (void **)&node);
    vldh_paste_node_insert( wind, node);
  }
  /* Insert the cons */
  for ( i = 0; i < fcon_count; i++ ) {
    flow_GetUserData( fcon_list[i], (void **)&con);
    vldh_paste_con_insert( wind, con);
  }
  if ( fcon_count > 0)
    free( fcon_list);
  if ( fnode_count > 0)
    free( fnode_list);

  /* Delete the selected objects */
  delete_selected();
  
  return GRE__SUCCESS;
}

//
//	Deletes one single node and all connections connected
//	to the node. Inserts all the deleted objects in deletelist and
//	the node in vldh_singlenode buffer.
int WGre::cut_node( vldh_t_node node)
{
  /* Insert in vldh_pastelist */
  vldh_paste_init();

  vldh_paste_node_insert( wind, node);
	
  delete_node( node);
  return GRE__SUCCESS;
}

//
//	Copies one single node to the paste singelnode buffer.
//
int WGre::copy_node( vldh_t_node node)
{
  /* Insert in vldh_pastelist */
  vldh_paste_init();

  /* Insert the nodes */
  vldh_paste_node_insert( wind, node);
  return GRE__SUCCESS;
}

//
//	Copys all selected objects to the vldh_paste list.
//
int WGre::copy()
{
  int		i;
  vldh_t_con  	con;
  vldh_t_node	node;
  flow_tNode	*fnode_list;
  flow_tCon	*fcon_list;
  int		fnode_count;
  int		fcon_count;

  flow_GetSelectedNodes( flow_ctx, &fnode_list, &fnode_count);
  flow_GetSelectedCons( flow_ctx, &fcon_list, &fcon_count);
  if ( fnode_count == 0 && fcon_count == 0 ) {
    message( "Nothing to copy" );
    BEEP;
    return GRE__SUCCESS;
  }

  /* Insert in vldh_pastelist */
  vldh_paste_init();

  /* Insert the nodes */
  for ( i = 0; i < fnode_count; i++ ) {
    flow_GetUserData( fnode_list[i], (void **)&node);
    vldh_paste_node_insert( wind, node);
  }
  /* Insert the cons */
  for ( i = 0; i < fcon_count; i++ ) {
    flow_GetUserData( fcon_list[i], (void **)&con);
    vldh_paste_con_insert( wind, con);
  }
  unselect();

  if ( fcon_count > 0)
    free( fcon_list);
  if ( fnode_count > 0)
    free( fnode_list);

  return GRE__SUCCESS;
}

//
//	This routine draws all nodes and connections in a vldh window
//	into a neted window.
//
int WGre::window_draw()
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
  sts = vldh_get_nodes( wind, &node_count, &node_list);
  if ( EVEN(sts)) return sts;
  node_ptr = node_list;

  /* Get all the connections */
  sts = vldh_get_cons( wind, &con_count, &con_list);
  if ( EVEN(sts)) return sts;
  con_ptr = con_list;

  flow_SetNodraw( flow_ctx);

  /* Create the nodes */
  for ( i = 0; i < (int)node_count; i++) {
    node_class = 0;
    sts = get_nodeclass( (*node_ptr)->ln.cid,
			 ((*node_ptr)->hn.wind)->hw.ldhses, 
			 (*node_ptr)->ln.object_type,
			 (*node_ptr)->ln.mask, (*node_ptr)->ln.subwindow,
			 (*node_ptr)->ln.nodewidth,
			 &node_class, *node_ptr);
    if (EVEN(sts)) return sts;

    flow_CreateNode( flow_ctx, (*node_ptr)->hn.name, 
		     node_class, (*node_ptr)->ln.x, (*node_ptr)->ln.y,
		     *node_ptr, &node_id);
    (*node_ptr)->hn.node_id = node_id;

    sts = node_annotations_draw( *node_ptr, 0); 
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
  for ( i = 0; i < (int)con_count; i++) {
    sts = goec_con_draw( this, *con_ptr, GRE_CON_NONROUTE, 0);
    con_ptr++;
  }
  if ( con_count > 0) free((char *) con_list);
  if ( node_count > 0) free((char *) node_list);

  flow_ResetNodraw( flow_ctx);
  flow_Redraw( flow_ctx);

  return GRE__SUCCESS;
}

//
//	Returns an array of all selected nodes.
//	The array should be freed by the user with a free call.
//
int WGre::get_selnodes( unsigned long *node_count, vldh_t_node **nodelist)
{
  int	i;
  vldh_t_node	node;
  flow_tNode	*fnode_list;
  int		fnode_count;

  flow_GetSelectedNodes( flow_ctx, &fnode_list, &fnode_count);

  *node_count = 0;
  if ( fnode_count == 0)
    return GRE__SUCCESS;

  /* create array */
  *nodelist = (vldh_t_node *)calloc( fnode_count, 
				     sizeof(node));

  /* Insert the nodes */
  for ( i = 0; i < fnode_count; i++ ) {
    flow_GetUserData( fnode_list[i], (void **)&node);
    *(*nodelist + *node_count) = node;
    (*node_count)++;
  }
  return GRE__SUCCESS;
}

//
//	Returns a neted nodetypeid for the specified objecttype.
//	If the the mask for the nodetype is of the defaultmask for
//	the objecttype, the nodetypeid is fetcht from the nodetypelist
//	in the grecontext. If it doesn't exist the nodetype is created
//	and put into the nodetypelist.
//	If the mask is not the defautlmask a new nodetype is created every
//	time and not stored.
//
int WGre::get_nodeclass( pwr_tClassId cid, ldh_tSesContext ldhses, unsigned long node_type,
			 unsigned int *mask, unsigned long subwindowmark,
			 unsigned long node_width, flow_tNodeClass *node_class,
			 vldh_t_node node)
{
  unsigned char	new_nodetype;
  int			sts, size;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;
	
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

  if ( !new_nodetype ) {
    /* Get the defaultmask */
    /* Get graphbody for the class */
    sts = ldh_GetClassBody( ldhses, cid, "GraphPlcNode",
			    &bodyclass, (char **)&graphbody, &size);
    if( EVEN(sts)) return sts;

    /* Check if the mask is changed */
    if( (graphbody->default_mask[0] != *mask) ||
	(graphbody->default_mask[1] != *(mask + 1)))
      /* New nodetype */
      new_nodetype = TRUE;
    
  }

  if ( !new_nodetype ) {
    /* Use the standard graphics and save nodetypeid in context */
    if ( nodeclass_list[node_type][subwindowmark] == 0 ) {
      /* Create a new nodetype_id */
      sts = goen_create_nodetype( flow_ctx, cid, ldhses, 
				  mask, subwindowmark, node_width,
				  &nodeclass_list[node_type][subwindowmark],
				  node);
      if ( EVEN(sts) ) return sts;
    }
    *node_class = nodeclass_list[node_type][subwindowmark];
  }
  else {
    /* create a special nodetype for this node */
    sts = goen_create_nodetype( flow_ctx, cid, ldhses, mask, 
				subwindowmark, node_width,
				node_class, node);
    if ( EVEN(sts) ) return sts;
  }
  return GRE__SUCCESS;
}
//
//	Get neted connectiontype id for a connection.
//	If it is the first time the con_type is created and the id is
//	stored in contypeidlist in gre context. If not the first time it
//	is fetched from there.
//
int WGre::get_conclass( pwr_tClassId cid, ldh_tSesContext ldhses, unsigned long con_type,
			flow_tConClass *con_class)
{
  int sts;

  if ( cid == 0)
    con_type = MAX_NUMBER_OF_CONTYPES - 1;
  if ( conclass_list[con_type] == 0 ) {
    /* If the connectiontype is not yet created, create it */
    sts = goen_create_contype( flow_ctx, cid, ldhses,
			       &conclass_list[con_type]);
    if ( EVEN(sts) ) return sts;
  }
  *con_class = conclass_list[ con_type ];
  return GRE__SUCCESS;
}


//_Backcalls for the controlled flow widget______________________________

//
//	Callback from flow.
//
int WGre::flow_cb( FlowCtx *ctx, flow_tEvent event)
{
  unsigned long dummy = 0;
  WGre		*gre;
  vldh_t_node	source, dest;
  void		*vobject;
  double	ll_x, ll_y, ur_x, ur_y, width, height;
  int		subwindow_nr;
  int		index;

  flow_GetCtxUserData( ctx, (void **)&gre);

  gre->search_rectangle_delete();

  if ( event->any.type == flow_eEventType_CreateCon) {
    flow_GetUserData( event->con_create.source_object, (void **) &source);
    if ( event->con_create.dest_object)
      flow_GetUserData( event->con_create.dest_object, (void **) &dest);
    else
      dest = 0;
    (gre->gre_con_created) (gre, 
		event->con_create.x, event->con_create.y, 
		source, event->con_create.source_conpoint,
		dest, event->con_create.dest_conpoint);	
  }

  switch ( event->event) {
  case flow_eEvent_Init:
    break;
  case flow_eEvent_PasteSequenceStart:
    flow_SetSelectHighlight( ctx);
    break;
  case flow_eEvent_MB2DoubleClick:
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_delete) (gre, vobject, VLDH_NODE);
      break;
    case flow_eObjectType_Con:
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_delete) (gre, vobject, VLDH_CON);
      break;
    default:
      (gre->gre_delete) (gre, 0, 0);
    }
    break;
  case flow_eEvent_MB1Click:
    /* Select */
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
    case flow_eObjectType_Con:
      if ( flow_FindSelectedObject( ctx, event->object.object)) {
	flow_SelectClear( ctx);
      }
      else {
	flow_SelectClear( ctx);
	flow_SetHighlight( event->object.object, 1);
	flow_SelectInsert( ctx, event->object.object);
      }
      break;
    default:
      flow_SelectClear( ctx);
    }
    break;
  case flow_eEvent_ObjectMoved: {
    vldh_t_node	vnode;
    vldh_t_con	vcon;
    double		pos_x, pos_y;
    double		*x_arr, *y_arr;
    int		i, num;

    /* Object moved */
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      flow_GetUserData( event->object.object, (void **) &vnode);
      flow_GetNodePosition( event->object.object, &pos_x, &pos_y);
      vnode->ln.x = pos_x;
      vnode->ln.y = pos_y;
      vldh_nodemodified( vnode);

      (gre->gre_node_moved) (gre);
      break;
    case flow_eObjectType_Con:
      flow_GetUserData( event->object.object, (void **) &vcon);
      flow_GetConPosition( event->object.object, &x_arr, &y_arr, &num);
      for ( i = 0; i < num; i++) {
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
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
    case flow_eObjectType_Con:
      if ( flow_FindSelectedObject( ctx, event->object.object)) {
	flow_SetHighlight( event->object.object, 0);
	flow_SelectRemove( ctx, event->object.object);
      }
      else {
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
    /* Create node */
    switch ( event->object.object_type) {
    case flow_eObjectType_NoObject:
      (gre->gre_node_created) ( gre, dummy, event->object.x,
				   event->object.y);
      break;
    default:
      break;
    }
    break;
  case flow_eEvent_MB1DoubleClick:
    /* Open attribute editor */
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      gre->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_attribute) (gre, (vldh_t_node) vobject);
      break;
    default:
      gre->message( "No hit in object");
      BEEP;
    }
    break;
  case flow_eEvent_MB1DoubleClickShift:
    /* Open subwindow */
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      gre->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
      flow_MeasureNode( event->object.object, &ll_x,&ll_y,&ur_x,&ur_y);
      width = ur_x - ll_x;
      if ( event->object.x > ( ll_x + width / 2 ))	
	subwindow_nr = 1;
      else				
	subwindow_nr = 2;
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_subwindow) ( gre, (vldh_t_node)vobject, subwindow_nr);
      break;
    default:
      gre->message( "No hit in object");
      BEEP;
    }
    break;
  case flow_eEvent_MB1DoubleClickCtrl:
    /* Insert reference object */
    switch ( event->object.object_type) {
    case flow_eObjectType_Node:
      gre->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
      flow_MeasureNode( event->object.object, &ll_x,&ll_y,&ur_x,&ur_y);
      height = ur_y - ll_y;
      if ( event->object.y > ll_y + height / 2)	
	index = 1;
      else
	index = 2;
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_getobj) ( gre, (vldh_t_node)vobject, index);
      break;
    default:
      gre->message( "No hit in object");
      BEEP;
    }
    break;
  case flow_eEvent_MB3Down:
    flow_SetClickSensitivity( gre->flow_ctx, flow_mSensitivity_MB3Press);
    break;
  case flow_eEvent_MB3Press: {
    /* Popup menu */
    int			x_pix, y_pix;
    vldh_t_node		current_node;
    flow_tObject	*select_list;
    int			select_cnt;

    flow_PositionToPixel( gre->flow_ctx, event->object.x,
			  event->object.y, &x_pix, &y_pix); 
	  
    gre->popup_menu_x = x_pix;
    gre->popup_menu_y = y_pix;
    gre->get_popup_position( &x_pix, &y_pix);

    current_node = 0;

    flow_GetSelectList( ctx, &select_list, &select_cnt);
    if ( !select_cnt) {
      if ( event->object.object_type == flow_eObjectType_Node)
	/* Get the current object */
	flow_GetUserData( event->object.object, (void **)&current_node);
    }	
    if ( current_node || select_cnt == 1)
      gre->popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
    else
      gre->popupmenu_mode = GRE_POPUPMENUMODE_AREA;
    (gre->gre_popupmenu) (gre, x_pix, y_pix, 
			     gre->popupmenu_mode, current_node);
    break;
  }
  case flow_eEvent_MB1ClickCtrl: {
    char			help_title[32];
    vldh_t_node		node;
    vldh_t_con		con;
    int			size, sts;
      
    if ( event->object.object_type == flow_eObjectType_Node) {
      flow_GetUserData( event->object.object, (void **)&node);
      sts = ldh_ObjidToName( (node->hn.wind)->hw.ldhses,
			     cdh_ClassIdToObjid( node->ln.cid), ldh_eName_Object,
			     help_title, sizeof( help_title), &size);
      if ( EVEN(sts)) return 1;
      (gre->gre_help) ( gre, help_title);
    }
    if ( event->object.object_type == flow_eObjectType_Con) {
      flow_GetUserData( event->object.object, (void **)&con);
      sts = ldh_ObjidToName( (gre->wind)->hw.ldhses, 
			     cdh_ClassIdToObjid( con->lc.cid), ldh_eName_Object,
			     help_title, sizeof( help_title), &size);
      if ( EVEN(sts)) return 1;
      (gre->gre_help) ( gre, help_title);
    }
    else {
      gre->message( "No hit in object");
      BEEP;
    }	
    break;
  }
  case flow_eEvent_MB1DoubleClickShiftCtrl: {
    /* Copy */
    if ( event->object.object_type == flow_eObjectType_Node) {
      /* Copy only this object */
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_copy) (gre, vobject, VLDH_NODE);
    }
    else {
      (gre->gre_copy) (gre, 0, 0);
    }
    break;
  }
  case flow_eEvent_MB2DoubleClickShiftCtrl: {
    /* Cut */
    if ( event->object.object_type == flow_eObjectType_Node) {
      /* Copy only this object */
      flow_GetUserData( event->object.object, &vobject);
      (gre->gre_cut) (gre, vobject, VLDH_NODE);
    }
    else {
      (gre->gre_cut) (gre, 0, 0);
    }
    break;
  }
  case flow_eEvent_MB2ClickShiftCtrl: {
    /* Paste */
    (gre->gre_paste) (gre, event->object.x, event->object.y);
    break;
  }
  case flow_eEvent_SelectClear:
    flow_ResetSelectHighlight( ctx);
    break;
  case flow_eEvent_ScrollDown:
    flow_Scroll( ctx, 0, -0.05);
    break;
  case flow_eEvent_ScrollUp:
    flow_Scroll( ctx, 0, 0.05);
    break;
  default:
    ;
  }
  return 1;
}

//_Methods defined for this module_______________________________________

//
//	Create a new gre instance.
//	This routine creates the neted widget.
//

WGre::WGre( void *wg_parent_ctx,
	    char *name) :
  parent_ctx( wg_parent_ctx), grid_size(0.05), gridobject(0), grid_on(0), wind(0), 
  conref_nodetypeid(0), display_nodetypeid(0),
  sel_node_count(0), del_node_count(0), searchrect_node_id(0), popupmenu_mode(0),
  trace_started(0), trace_analyse_nc(0), trace_con_cc(0),
  trace_changenode(0)
{
}

int WGre::init()
{
#if 0
  FlowWidget	flow_widget;
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_widget = (FlowWidget) this->flow_widget;
  flow_ctx = (flow_tCtx)flow_widget->flow.flow_ctx;

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
  flow_SetAttributes( flow_ctx, &flow_attr, mask);
  flow_SetCtxUserData( flow_ctx, this);
#endif
  return GRE__SUCCESS;
}

int WGre::init_flow( FlowCtx *ctx, void *client_data)
{
  WGre *gre = (WGre *)client_data;
  flow_sAttributes flow_attr;
  unsigned long	mask;

  gre->flow_ctx = ctx;

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
  flow_SetAttributes( gre->flow_ctx, &flow_attr, mask);
  flow_SetCtxUserData( gre->flow_ctx, gre);
  return 1;
}

int WGre::edit_setup()
{
  flow_tCtx ctx = flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_MoveNode, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Press, flow_eEventType_CreateCon, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_RegionSelect, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1PressShift, flow_eEventType_RegionAddSelect, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClick, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2Click, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2DoubleClickShiftCtrl, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB2ClickShiftCtrl, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_Init, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_PasteSequenceStart, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ObjectMoved, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	flow_cb);
  return 1;
}

int WGre::view_setup()
{
  flow_tCtx ctx = flow_ctx;

  flow_DisableEventAll( ctx);
  flow_EnableEvent( ctx, flow_eEvent_MB1Press, flow_eEventType_RegionSelect, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1PressShift, flow_eEventType_RegionAddSelect, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1ClickShift, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1DoubleClickShift, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_Init, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	flow_cb);
  flow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	flow_cb);
  return 1;
}

/*******************************************************************
*	Store the adresses of the desired backcalls functions in the 
*	gre context.
* 	Description of backcall routines:
*
* static void zzz_gre_setup_window( gre)
*	
*	Routine called when the neted window is created.
*	Makes is possible to enable the desired events by the routine
*	gre_setup_window()
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
*
* static void zzz_gre_node_created( gre, node_type, x, y)
*	
*	Routine called when a node creation callback is recieved from neted.
*	Makes is possible to create of the desired class by gre_create_node.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* unsigned long	node_type	I	Not used. 
* float		x		I	x koordinate ( on grid).
* float		y		I	y koordinate ( on grid).
*
* static void 	zzz_gre_con_created (gre, source_obj, source_point,
*				destination_obj, destination_point)
*	
*	Routine called when a connection creation callback is recieved from 
*	neted.
*	Makes is possible to create a connections of the desired class by 
*	gre_create_con.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* vldh_t_node	source_obj	I	source node.
* unsigned long	source_point	I	connection point on source node.
* vldh_t_node	destination_obj	I	destination node.
* unsigned long	destination_point I	connection point on destination node.
*
* static void 	zzz_gre_node_moved (gre)
*	
* 	Routine called when a node is moved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
*
* static void 	zzz_gre_node_moved (gre)
*	
* 	Routine called when a node is moved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
*
* static void 	zzz_gre_delete (gre, object, object_type)
*	
* 	Routine called when a delete callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.	
* unsigned long	object		I	vldh node or connection,
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, VLDH_CON
*					or zero if no objects is hit by the 
*					click.
*
* static void 	zzz_gre_cut (gre, object, object_type)
*	
* 	Routine called when a cut callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.	
* unsigned long	object		I	vldh node or connection,
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, VLDH_CON
*					or zero if no objects is hit by the 
*					click.
*
* static void 	zzz_gre_copy (gre, object, object_type)
*	
* 	Routine called when a copy callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.	
* unsigned long	object		I	vldh node or
*					or zero if no objects is hit by the 
*					click.
* unsigned long	object_type	I	type of object. VLDH_NODE, or zero 
*					if no objects is hit by the 
*					click.
*
* static void foe_gre_paste (gre, x, y)
*
* 	Routine called when a paste callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* float		x		I	x koordinate ( on grid)
* float		y		I	y koordinate ( on grid)
*
* static void zzz_gre_attribute (gre, object)
*	
* 	Routine called when a attribute callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* vldh_t_node	object		I	vldh node.
*
* static void zzz_gre_subwindow (gre, object, subwindow_nr)
*	
* 	Routine called when a subwindow callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* vldh_t_node 	object		I	vldh node.
* unsigned long	subwindow_nr	I	1 if the click is in the left part
*					of node,
*					2 in the right part.
*
* static void zzz_reserv(gre, x, y)
*
* 	Routine called when a reserv callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* float		x		I	x koordinate ( on grid)
* float		y		I	y koordinate ( on grid)
*
* static void foe_gre_popupmenu (gre, x_pix, y_pix)
*	
* 	Routine called when popupmenu callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* int		x_pix		I	click x koordinate in pixel
* int		y_pix		I	click y koordinate in pixel
*
* static void zzz_gre_getobj (gre , node, index)
*	
* 	Routine called when a getobj callback is recieved.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* vldh_t_node 	node 		I	vldh node.
* unsigned long	index		I	0 if the click is in the upper part
*					of the node.
*					1 if it is in the lower part.
*
* static void zzz_gre_undelete (gre )
*
* 	Not yet implemented.
*
* static void zzz_gre_unselect (gre )
*
* 	Not yet implemented.
*
* static void foe_gre_help (gre, help_title)
*
* 	Routine called when a help callback is recieved from neted.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* char		*help_title	I	name of the objects class.
*
* static void zzz_gre_regionmoved (gre) 
*
* 	Not yet implemented.
*
* static void zzz_gre_message (gre, message)
*
* 	Routine called when gre feels he has to send a message to the user.
*
* Type		Parameter	IOGF	Description
* gre_ctx	gre		I	gre context.
* char		*message	I	text message.
*
**************************************************************************/

int WGre::setup_backcalls (
	void (*setup_window_bc)(WGre *),
	void (*node_created_bc)(WGre *, unsigned long, float, float),
	void (*con_created_bc)(WGre *, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long),
	void (*node_moved_bc)(WGre *),
	void (*delete_bc)(WGre *, void *, unsigned long),
	void (*cut_bc)(WGre *, void *, unsigned long),
	void (*copy_bc)(WGre *, void *, unsigned long),
	void (*paste_bc)(WGre *, float, float),
	void (*attribute_bc)(WGre *, vldh_t_node),
	void (*subwindow_bc)(WGre *, vldh_t_node, unsigned long),
	void (*reserv_bc)(),
	void (*popupmenu_bc)(WGre *, int, int, int, vldh_t_node),
	void (*getobj_bc)(WGre *, vldh_t_node, unsigned long),
	void (*undelete_bc)(WGre *),
	void (*unselect_bc)(WGre *),
	void (*help_bc)(WGre *, char *),
	void (*regionmoved_bc)(WGre *),
	void (*message_bc)(WGre *, char *))
{

  /* Fill in callback addresses */
  gre_setup_window = setup_window_bc;
  gre_node_created = node_created_bc;
  gre_con_created = con_created_bc;
  gre_node_moved = node_moved_bc;
  gre_delete = delete_bc;
  gre_cut = cut_bc;
  gre_copy = copy_bc;
  gre_paste = paste_bc;
  gre_attribute = attribute_bc;
  gre_subwindow = subwindow_bc;
  gre_reserv = reserv_bc;
  gre_popupmenu = popupmenu_bc;
  gre_getobj = getobj_bc;
  gre_undelete = undelete_bc;
  gre_unselect = unselect_bc;
  gre_help = help_bc;
  gre_regionmoved = regionmoved_bc;
  gre_message = message_bc;
  
  return GRE__SUCCESS;
}

//
//	Deletes a gre instance.
//
WGre::~WGre()
{
}

//
//	Disables all events in the netedwidget.
//	The id of these events is given by the gre
// 	Author: SG 29.04.91
//
void WGre::disable_button_events()
{
}

//
//	Creates a node of specified class.
//	A node is created in vldh and drawn in the window.
//
int WGre::create_node( pwr_tClassId cid, float x, float y, vldh_t_node *node)
{
  flow_tNodeClass	node_class = 0;
  vldh_t_node 	node_object;
  double		ll_x, ll_y, ur_x, ur_y;
  unsigned long	subwindowmark = 0;
  int		sts;

  sts = vldh_node_create( wind, cid, &node_object);
  if( EVEN(sts) ) return sts;

  sts = get_nodeclass( cid,
		       (node_object->hn.wind)->hw.ldhses, 
		       node_object->ln.object_type, 
		       node_object->ln.mask,
		       subwindowmark, 
		       node_object->ln.nodewidth,
		       &node_class, node_object);
  if( EVEN(sts) ) return sts;

  DEFERRED_UPDATE;

  flow_CreateNode( flow_ctx, node_object->hn.name, 
		   node_class, x, y,
		   node_object, &node_object->hn.node_id);
  flow_MeasureNode( node_object->hn.node_id,
		    &ll_x, &ll_y, &ur_x, &ur_y);
  node_object->ln.width = ur_x - ll_x;
  node_object->ln.height = ur_y - ll_y;
  flow_GetNodePosition( node_object->hn.node_id, &ll_x, &ll_y);
  node_object->ln.x = ll_x;
  node_object->ln.y = ll_y;

  sts = node_annotations_draw( node_object, 0); 
  if( EVEN(sts) ) return sts;

  UPDATE_SCREEN;

  *node = node_object;

  /* Update header /CJ 050415 */ 
  if ( cid == pwr_cClass_Document) {
    init_docobjects();
  }
	
  return GRE__SUCCESS;
}

//
//	Creates a connections of the specified class.
//	Create a connection in vldh and draw a connection in the window
//	between the source and destination nodes. The user can specify
//	if the connections should be drawn as a referens connection.
//
int WGre::create_con( pwr_tClassId  cid,
		      vldh_t_node source_obj, unsigned long source_point,
		      vldh_t_node destination_obj, unsigned long destination_point,
		      unsigned long drawtype)
{
  vldh_t_con 	con_object;
  int		sts;

  sts= vldh_con_create( wind, cid, drawtype, 
			source_obj,source_point,
			destination_obj,destination_point,&con_object);
  if ( EVEN(sts)) return sts;
	
  sts = goec_con_draw( this, con_object, GRE_CON_CREATE, 0);
  if( EVEN(sts)) return sts;

  return GRE__SUCCESS;
}

//
//	Prints an area described by a documentation object in a ddif file.
//	Converts the ddif file to postscript and sends it to suitable que.
//
int WGre::print_docobj( vldh_t_node doc_obj)
{
  pwr_tFileName filename;
  double ll_x, ll_y, ur_x, ur_y;
  char	cmd[250];
  
  flow_MeasureNode( doc_obj->hn.node_id, &ll_x, &ll_y, &ur_x, &ur_y);
  sprintf( filename, "pwrp_tmp:pssdoc%s.ps", 
	   vldh_IdToStr(0, doc_obj->ln.oid));
  dcli_translate_filename( filename, filename);
  flow_PrintRegion( flow_ctx, ll_x, ll_y, ur_x, ur_y, filename);

  dcli_translate_filename( cmd, "$pwr_exe/wb_gre_print.sh");
  strcat( cmd, " ");
  strcat( cmd, filename);
  system( cmd);
  return GRE__SUCCESS;
}

//
//	Prints an area described by the coordinates.
//
int WGre::measure_object( vldh_t_node node, float *ll_x, float *ll_y,
			  float *width, float *height)
{
	*ll_x = node->ln.x;
	*ll_y = node->ln.y;
	*width = node->ln.width;
	*height = node->ln.height;

	return GRE__SUCCESS;
}
//
//	Prints an area described by the coordinates.
//
int WGre::print_rectangle( float ll_x, float ll_y, float ur_x, float ur_y,
			   char *file_id)
{
  pwr_tFileName filename;
  char		cmd[250] ;

  sprintf( filename, "pwrp_tmp:pssdoc%s.ps", file_id);
  dcli_translate_filename( filename, filename);
  flow_PrintRegion( flow_ctx, ll_x, ll_y, ur_x, ur_y, filename);

  dcli_translate_filename( cmd, "$pwr_exe/wb_gre_print.sh");
  strcat( cmd, " ");
  strcat( cmd, filename);
  system( cmd);

  return GRE__SUCCESS;
}

//
//	This routine is called when a paste callback is recieved.
//	Initiates paste by drawing a paste rectangle in the window.
//	When a node moved callback from this rectangle occurs the 
//	objects will be pasted, se routine gre_neted_node_moved.
//	
void WGre::paste( float cursor_x, float cursor_y, int paste_type)
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

  flow_PasteClear( flow_ctx);

  /* Copy objects from vldh paste */
  sts = vldh_paste_copy( wind, 0, 0, 
			 &node_count, &node_list, &con_count, &con_list);
  if ( EVEN(sts)) return;

  if ( node_count == 0 && con_count == 0) {
    /* Paste buffer is empty */
    message( "Nothing to paste");
    return;
  }

  /* Create the objects in flow paste list */
  node_ptr = node_list;
  for ( i = 0; i < (int)node_count; i++) {
    node_class = 0;
    sts = get_nodeclass( (*node_ptr)->ln.cid,
			 ((*node_ptr)->hn.wind)->hw.ldhses, 
			 (*node_ptr)->ln.object_type,
			 (*node_ptr)->ln.mask, (*node_ptr)->ln.subwindow,
			 (*node_ptr)->ln.nodewidth,
			 &node_class, *node_ptr);
    if ( EVEN(sts)) return;

    flow_CreatePasteNode( flow_ctx, (*node_ptr)->hn.name, 
			  node_class,
			  (*node_ptr)->ln.x,
			  (*node_ptr)->ln.y,
			  *node_ptr, &node_id);
    (*node_ptr)->hn.node_id = node_id;

    sts = node_annotations_draw( *node_ptr, 1); 
    if( EVEN(sts)) return;
    node_ptr++;
  }

  /* Create the neted connections */
  con_ptr = con_list;
  for ( i = 0; i < (int)con_count; i++) {
    if ( (*con_ptr)->lc.drawtype == GOEN_CONSYSREF ||
	 (*con_ptr)->lc.drawtype == GOEN_CONUSERREF)
      /* This is a fix for backward compatibility */
      sts = get_conclass( 0,
			  ((*con_ptr)->hc.wind)->hw.ldhses,
			  (*con_ptr)->lc.object_type,
			  &con_class);
    else
      sts = get_conclass( (*con_ptr)->lc.cid,
			  ((*con_ptr)->hc.wind)->hw.ldhses,
			  (*con_ptr)->lc.object_type,
			  &con_class);
    
    for ( j = 0; j < (int)(*con_ptr)->lc.point_count; j++) {
      x[j] = (*con_ptr)->lc.point[j].x;
      y[j] = (*con_ptr)->lc.point[j].y;
    }
    flow_CreatePasteCon( flow_ctx, (*con_ptr)->hc.name, con_class, 
			 (*con_ptr)->hc.source_node->hn.node_id,
			 (*con_ptr)->hc.dest_node->hn.node_id,
			 (*con_ptr)->lc.source_point, (*con_ptr)->lc.dest_point,
			 *con_ptr, &(*con_ptr)->hc.con_id, (*con_ptr)->lc.point_count,
			 x, y);
    con_ptr++;
  }
  
  flow_Paste( flow_ctx);
  flow_SetSelectHighlight( flow_ctx);

  /* Update header /CJ 050415 */ 
  init_docobjects();

}

//
//	Redraw a node when a subwindow i created or deleted with the
//	new subwindow mark.
//
int WGre::subwindow_mark( vldh_t_node object)
{
  flow_tNodeClass	node_class = 0;
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
  sts = get_nodeclass( object->ln.cid, 
		       (object->hn.wind)->hw.ldhses, 
		       object->ln.object_type, 
		       object->ln.mask,
		       object->ln.subwindow,
		       object->ln.nodewidth,
		       &node_class, object);
  if (EVEN(sts)) return sts;

  flow_CreateNode( flow_ctx, object->hn.name, node_class,
		   object->ln.x, object->ln.y,
		   (void **) object, &node_id);
  object->hn.node_id = node_id;
  sts = node_annotations_draw( object, 0); 
  if (EVEN(sts)) return sts;

  /* Set permanent highlight if selected */
  if ( highlight_flag ) {
    flow_SetHighlight( object->hn.node_id, highlight_flag);
    flow_SelectInsert( flow_ctx, object->hn.node_id);
  }

  /* redraw the connections */
  del_con_count = 0;
  del_node_count = 0;
  sts = vldh_get_cons_node( object, &con_count, &con_list);
  if ( EVEN(sts)) return sts;

  con_ptr = con_list;
  for ( j = 0; j < (int)con_count; j++) {
    sts = goec_con_draw( this, *con_ptr, GRE_CON_NONROUTE, 0);
    con_ptr++;
  }
  if ( con_count > 0) free((char *) con_list);
  
  return GRE__SUCCESS;
}

//
//	Identifies the new point nr for the connections after a 
//	modifikation of the input or outputmask.
//	Returns an array (point_array) where
//	new_point_nr = point_array[old_point_nr]
//
pwr_tStatus WGre::node_update_points( vldh_t_node node,
				      unsigned long old_input_mask, 
				      unsigned long old_output_mask,
				      unsigned long new_input_mask,
				      unsigned long new_output_mask,
				      int *point_array, int *point_count)
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

  sts = ldh_GetClassBody( (node->hn.wind)->hw.ldhses,
			  node->ln.cid, "GraphPlcNode",
			  &bodyclass, (char **)&graphbody, &size);
  if( EVEN(sts)) return sts;

  inputs = graphbody->parameters[PAR_INPUT];
  outputs = graphbody->parameters[PAR_OUTPUT];

  pointmask = 1;
  *point_count = 0;
  new_point_count = 0;
  point_array_ptr = point_array;
  /* Input points */
  for ( i = 0; i < inputs; i++) {
    if (old_input_mask & pointmask) {
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
  for ( i = 0; i < outputs; i++) {
    if (old_output_mask & pointmask) {
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

//
//	Updates a node width all modifications made in the ldh object.
//	- the width of the node is change dependent on the lengt of the 
//	  first annotation.
//	- the graphmask is changed, inputs or outputs is added or taken
//	  away.
//	- the inversemask i changed, inputs are inverted or reinverted.
//	- data affekting the annotaions is changed.
//
int WGre::node_update( vldh_t_node object)
{
  flow_tNodeClass	node_class = 0;
  flow_tNode	node_id;
  vldh_t_con 	*con_list;
  vldh_t_con 	*con_ptr;
  unsigned long	con_count;
  int		i,j;
  int		sts;
  int		highlight_flag;
  int		size;
  pwr_sPlcNode	*nodebuffer;
  pwr_eClass	cid;
  int		nodewidth_changed;
  int		graphmask_changed;
  int		invertmask_changed;
  double		ll_x, ll_y, ur_x, ur_y;
  int		point_conv[ VLDH_MAX_CONPOINTS];
  int		point_conv_count;
  int		point;
  int		deleted_connections;
  char		msg[80];

  if ( ! object->hn.node_id)
    /* Backcall from ldh, object is not yet created */
    return GRE__SUCCESS;


  /* Do some class specific updates */
  vldh_node_update_spec( object);

  /* Check if node is highlighted */
  flow_GetHighlight( object->hn.node_id, &highlight_flag);

  /* Get the object name from ldh */
  sts = ldh_ObjidToName( (object->hn.wind)->hw.ldhses, 
			 object->ln.oid, ldh_eName_Object,
			 object->hn.name, sizeof( object->hn.name), &size);
  if( EVEN(sts)) return sts;

  /* Check if there is new nodewidth */
  nodewidth_changed = TRUE;	  

  /* Get the ldh graphbuffer of the node */
  sts = ldh_GetObjectBuffer( (object->hn.wind)->hw.ldhses,
			     object->ln.oid, "DevBody", "PlcNode", &cid,	
			     (char **)&nodebuffer, &size);
  if( EVEN(sts)) return sts;

  /* Check if the mask is changed */
  if( (nodebuffer->mask[0] != object->ln.mask[0]) ||
      (nodebuffer->mask[1] != object->ln.mask[1])) {
    graphmask_changed = TRUE;
    sts = node_update_points( object,
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

  if ( nodewidth_changed || graphmask_changed || invertmask_changed ) {
    /* The node has to be redrawn */
    /* For the moment delete all connections and redraw the node */
    sts = vldh_get_cons_node( object, &con_count, &con_list);
    if ( EVEN(sts)) return sts;
    con_ptr = con_list;
    for ( i = 0; i < (int)con_count; i++) {
      goec_con_delete_noredraw( this, *con_ptr);
      (*con_ptr)->lc.point_count = 0;
      (*con_ptr)->hc.redrawn = 0;
      con_ptr++;
    }
    
    DEFERRED_UPDATE;

    /* Delete current node */
    flow_DeleteNode( object->hn.node_id);

    /* Create a new node */
    sts = get_nodeclass( object->ln.cid, 
			 (object->hn.wind)->hw.ldhses, 
			 object->ln.object_type, 
			 object->ln.mask,
			 object->ln.subwindow,
			 object->ln.nodewidth,
			 &node_class, object);
    if (EVEN(sts)) return sts;

    /* Create the new node on the old place */	
    flow_CreateNode( flow_ctx, object->hn.name,
		     node_class,
		     object->ln.x,
		     object->ln.y,
		     (void **) object,
		     &node_id);
    object->hn.node_id = node_id;

    /* If the name would be changed, copy it to vldh and redraw the
       annotaions */
    sts = node_annotations_draw( object, 0);
    if (EVEN(sts)) return sts;

    flow_MeasureNode( node_id, &ll_x,&ll_y,&ur_x,&ur_y);
    object->ln.width = ur_x - ll_x;
    object->ln.height = ur_y - ll_y;


    if ( !graphmask_changed ) {
      /* redraw the connections */
      del_con_count = 0;
      del_node_count = 0;
      con_ptr = con_list;
      for ( j = 0; j < (int)con_count; j++) {
	sts = goec_con_draw( this, *con_ptr, GRE_CON_CREATE, 0);
	con_ptr++;
      }
    }
    else {
      /* redraw the connections */
      del_con_count = 0;
      del_node_count = 0;
      con_ptr = con_list;
      deleted_connections = 0;
      for ( j = 0; j < (int)con_count; j++) {
	/* Convert connection point */
	if ( (*con_ptr)->hc.source_node == object) {
	  point = point_conv[(*con_ptr)->lc.source_point];
	  if ( point != (int)(*con_ptr)->lc.source_point && point != -1) {
	    vldh_node_con_delete( object, (*con_ptr)->lc.source_point, 
				  *con_ptr);
	    (*con_ptr)->lc.source_point = point;
	    vldh_conmodified( *con_ptr);
	    vldh_node_con_insert( object, (*con_ptr)->lc.source_point, 
				  *con_ptr, VLDH_NODE_SOURCE);
	  }
	}
	else {
	  point = point_conv[(*con_ptr)->lc.dest_point];
	  if ( point != (int)(*con_ptr)->lc.dest_point && point != -1) {
	    vldh_node_con_delete( object, (*con_ptr)->lc.dest_point, 
				  *con_ptr);
	    (*con_ptr)->lc.dest_point = point;
	    vldh_conmodified( *con_ptr);
	    vldh_node_con_insert( object, (*con_ptr)->lc.dest_point, 
				  *con_ptr, VLDH_NODE_DESTINATION);
	  }
	}
	if ( point == -1) {
	  /* Delete the connection */
	  vldh_con_delete(*con_ptr);
	  deleted_connections++;
	}
	con_ptr++;
      }
      sts = vldh_get_cons_node( object, &con_count, &con_list);
      if ( EVEN(sts)) return sts;
      con_ptr = con_list;
      for ( i = 0; i < (int)con_count; i++) {
	sts = goec_con_draw( this, *con_ptr, GRE_CON_CREATE, 0);
	con_ptr++;
      }
    }
    if ( con_count > 0) free((char *) con_list);
    
    UPDATE_SCREEN;
  }
  free((char *) nodebuffer);

  /* Set permanent highlight if selected */
  if ( highlight_flag ) {
    flow_SetHighlight( object->hn.node_id, highlight_flag);
    flow_SelectInsert( flow_ctx, object->hn.node_id);
  }
  
  vldh_nodemodified( object);

  if ( graphmask_changed && deleted_connections ) {
    sprintf( msg, "Warning, %d connections removed", 
	     deleted_connections);
    message( msg);
    BEEP;
  }
	
  /* Update the document headers /CJ 050415 */
  init_docobjects();

  return GRE__SUCCESS;
}


//
//	Zoom the display window.
//
void WGre::zoom( float zoom)
{
  flow_Zoom( flow_ctx, zoom);
}

//
//	Zoom the display window to initial zoom degree.
//
void WGre::unzoom()
{
  flow_UnZoom( flow_ctx);
}


//
//	Enable view of display rectangle.
//
int WGre::display()
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_attr.display_level = flow_mDisplayLevel_1 | flow_mDisplayLevel_2;
  mask = flow_eAttr_display_level;
  flow_SetAttributes( flow_ctx, &flow_attr, mask);
  flow_Redraw( flow_ctx);
  return GRE__SUCCESS;
}

//
//	Creates a display rectangle on a node and displays the string.
//	Max length of string is 5 characters.
//	If the rectangle exists the string is changed.
//
int WGre::set_display_value( vldh_t_node node, char *string)
{
  /* Set display annotation */
  flow_SetAnnotation( node->hn.node_id, GOEN_DISPLAYNODE_ANNOT, 
		      string, strlen(string));
  return GRE__SUCCESS;
}

//
//	Take away the display of execute order.
//
int WGre::undisplay()
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  flow_attr.display_level = flow_mDisplayLevel_1;
  mask = flow_eAttr_display_level;
  flow_SetAttributes( flow_ctx, &flow_attr, mask);
  flow_Redraw( flow_ctx);
  return GRE__SUCCESS;
}

//
//	Select a node.
//
int WGre::node_select( vldh_t_node node)
{
  flow_SelectInsert( flow_ctx, node->hn.node_id);
  flow_SetHighlight( node->hn.node_id, 1); 
  return GRE__SUCCESS;
}


//
//	Unselect a node.
//
int WGre::node_unselect( vldh_t_node node)
{
  flow_SelectRemove( flow_ctx, node->hn.node_id);
  flow_SetHighlight( node->hn.node_id, 0); 
  return GRE__SUCCESS;
}

//
//	Center a node in display window.
//
int WGre::center_node( vldh_t_node node)
{
  flow_CenterObject( flow_ctx, node->hn.node_id);
  return GRE__SUCCESS;
}


//
//	Prints time, plcprogram, window, system and page in the
//	document objects in a window.
//
int WGre::init_docobjects()
{
  int	sts, size;
  vldh_t_plc	plc;
  char	timstr[32];
  char	annot_str[16];
  char	systemname[80];
  char	systemgroup[80];
  char	plcname[60];
  pwr_tOName objname;
  pwr_tOName windname;
  char	*windname_ptr;
  pwr_tOName short_windname;
  int	plclen;
  int			doc_count;
  unsigned long		node_count;
  vldh_t_node		doc_obj;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i;
  char			*parvalue;
  pwr_tTime		mod_time;
  pwr_tTime		*mod_time_ptr;

  plc = wind->hw.plc;

  /* Get the system name */
  sts = utl_get_systemname( systemname, systemgroup);

  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;

  /* Loop all nodes once first in order to count documents. /CJ 050415 */
  doc_count = 0;
  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, (*node_ptr)->ln.oid)) doc_count++;
    node_ptr++;
  }

  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, 
			      (*node_ptr)->ln.oid)) {
      doc_obj = *node_ptr;

      /* System name in annotation nr 0 */
      flow_SetAnnotation( doc_obj->hn.node_id, 0, systemname, 
			  strlen(systemname));

      /* Plcname in annot 1 */
      sts = ldh_ObjidToName( wind->hw.ldhses, 
			     plc->lp.oid, ldh_eName_Hierarchy,
			     objname, sizeof( objname), &size);
      if ( EVEN(sts)) annot_str[0] = '\0';
	    
      cdh_StrncpyCutOff( plcname, objname, sizeof(plcname), 1);
      flow_SetAnnotation( doc_obj->hn.node_id, 1, plcname, 
			  strlen(plcname));

      /* Windowname in annot 2 */

      sts = ldh_ObjidToName( wind->hw.ldhses, 
			     wind->lw.oid, ldh_eName_Hierarchy,
			     windname, sizeof( windname), &size);
      if ( EVEN(sts)) annot_str[0] = '\0';

      /* Take away the plcpart of windowname before printing it */
      plclen = strlen( objname);
      windname_ptr = windname;
      strcpy( short_windname, windname_ptr + plclen + 1);

      flow_SetAnnotation( doc_obj->hn.node_id, 2, short_windname, 
			  strlen(short_windname));

      /* Page in annotations nr 3 */
      sts = ldh_GetObjectPar( wind->hw.ldhses,  
			      doc_obj->ln.oid, 
			      "DevBody", "Page",
			      (char **)&parvalue, &size); 
      if ( ODD(sts)) {
	flow_SetAnnotation( doc_obj->hn.node_id, 3, parvalue, 
			    strlen(parvalue));
	free((char *) parvalue);
      }

      /* PLC Window's Modified Time in annot 4 */
      mod_time_ptr = NULL;
      sts = ldh_GetObjectPar( wind->hw.ldhses,  
			      wind->lw.oid, 
			      "DevBody", "Modified",
			      (char **)&mod_time_ptr, &size); 
      if (ODD(sts)) {
	memcpy(&mod_time, mod_time_ptr, sizeof(mod_time));
	free((char *) mod_time_ptr);
	mod_time_ptr = &mod_time;
      }

      time_AtoAscii(mod_time_ptr, time_eFormat_DateAndTime, timstr, sizeof(timstr));             

      timstr[strlen(timstr) - 6] = '\0'; 
      flow_SetAnnotation( doc_obj->hn.node_id, 4, timstr, strlen(timstr));

      /* Signature in annotations nr 5 */
      sts = ldh_GetObjectPar( wind->hw.ldhses,  
			      doc_obj->ln.oid, 
			      "DevBody", "Signature",
			      (char **)&parvalue, &size); 
      if ( ODD(sts)) {
	flow_SetAnnotation( doc_obj->hn.node_id, 5, parvalue, 
			    strlen(parvalue));
	free((char *) parvalue);
      }
      
    }
    node_ptr++;

  }

  if ( node_count > 0) free((char *) nodelist);

  return GRE__SUCCESS;
}

//
//	Redraws all nodes and connections in a window.
//
int WGre::redraw()
{
  flow_Reconfigure( flow_ctx );

#if 0
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

    wind = gre->wind;
    sts = vldh_get_nodes( wind, &node_count, &nodelist);
    if ( EVEN(sts)) return sts;

    node_ptr = nodelist;
    for ( i = 0; i < node_count; i++) {
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
    if ( node_count > 0) free((char *) nodelist);
  }
#endif
  return GRE__SUCCESS;
}

int WGre::set_trace_attributes( char *host)
{
  flow_eTraceType 	trace_type;
  flow_tTraceObj      	object_str;
  flow_tTraceAttr      	attr_str = "";
  int			inverted;
  int			sts, i, j, size;
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  ldh_sParDef 		*bodydef;
  int 			rows;
  pwr_sAttrRef		*objarp;
  char			*np;
  char			*s;

  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;

  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    inverted = 0;
    sts = trace_get_attributes( this, *node_ptr, object_str, attr_str,
		&trace_type, &inverted);
    if ( ODD(sts) && sts != TRA__DISCARD ) {
      if ( host && strncmp( object_str, host, strlen(host)) == 0) {
	char tmp[120];
	strcpy( tmp, "$host");
	strcat( tmp, &object_str[strlen(host)]);
	strcpy( object_str, tmp);
      }
      flow_SetTraceAttr( (*node_ptr)->hn.node_id, object_str,
		attr_str, trace_type, inverted);
    }
    else {
      switch( (*node_ptr)->ln.cid ) {
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
      case pwr_cClass_GetAp:
      case pwr_cClass_GetIp:
	sts = ldh_GetObjectBodyDef( wind->hw.ldhses, 
			(*node_ptr)->ln.cid, "DevBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;
	strcpy( object_str, "");
	for ( j = 0; j < rows; j++ ) {
	  if ( bodydef[j].Par->Output.Info.Type == pwr_eType_AttrRef) {
	    /* Get the objid stored in the parameter */
	    sts = ldh_GetObjectPar( wind->hw.ldhses,
			(*node_ptr)->ln.oid,  "DevBody",
			bodydef[j].ParName, (char **)&objarp, &size);
	    if ( EVEN(sts)) return sts;

	    sts = ldh_AttrRefToName( wind->hw.ldhses, objarp,
				       cdh_mNName, &np, &size);
	    if ( EVEN(sts))
	      strcpy( object_str, "");
	    else
	      strncpy( object_str, np, sizeof(object_str));
	    free((char *) objarp);
	    
	    break;
	  }
	}
	break;
      default:
	/* Store object name */
	sts = ldh_ObjidToName( wind->hw.ldhses, (*node_ptr)->ln.oid,
		ldh_eName_Hierarchy, object_str, sizeof(object_str), &size);
	if ( EVEN(sts)) return sts;
      }

      switch ((*node_ptr)->ln.cid ) {
      case pwr_cClass_GetAp:
      case pwr_cClass_GetIp:
	s = strrchr( object_str, '.');
	if ( s) {
	  strcpy( attr_str, s + 1);
	  *s = 0;
	}
	break;
      default: ;
      }

      if ( host && strncmp( object_str, host, strlen(host)) == 0) {
	char tmp[120];
	strcpy( tmp, "$host");
	strcat( tmp, &object_str[strlen(host)]);
	strcpy( object_str, tmp);
      }
      flow_SetTraceAttr( (*node_ptr)->hn.node_id, object_str,
      		attr_str, flow_eTraceType_User, inverted);
    }
    node_ptr++;
  }
  if ( node_count > 0) free((char *) nodelist);
  return GRE__SUCCESS;
}

//
// Save to flow file
//
int WGre::save( char *filename)
{
  char fname[200];
  int sts;

  /* Update document headers before saving flow file  /CJ 050415 */
  init_docobjects();

  if ( !filename) {
    sprintf( fname, "pwrp_load:pwr_%s.flw",
		vldh_IdToStr(0, wind->lw.oid));
    dcli_translate_filename( fname, fname);
  }
  else
    dcli_translate_filename( fname, filename);
  sts = flow_Save( flow_ctx, fname);  
  return sts;
}


//
// Set gridsize
//
void WGre::set_grid_size( double size)
{
  flow_sAttributes flow_attr;
  unsigned long	mask;

  grid_size = size;

  mask = 0;
  flow_attr.grid_size_x = size;
  mask |= flow_eAttr_grid_size_x;
  flow_attr.grid_size_y = size;
  mask |= flow_eAttr_grid_size_y;
  flow_SetAttributes( flow_ctx, &flow_attr, mask);
}

void WGre::pixel_to_position( int pix_x, int pix_y, double *x, double *y)
{ 
  flow_PixelToPosition( flow_ctx, pix_x, pix_y, x, y);
}

void WGre::select_nextobject( flow_eDirection dir)
{
  flow_tNode 	sel, next;
  flow_tNode	*fnode_list;
  int		fnode_count;
  int		sts;

  flow_GetSelectedNodes( flow_ctx, &fnode_list, &fnode_count);
  if ( fnode_count == 0)
    sel = 0;
  else
    sel = fnode_list[0];

  if ( !sel || !flow_IsVisible( flow_ctx, sel, flow_eVisible_Partial)) {
    sts = flow_GetNextObject( flow_ctx, 0, dir, &next);
    if ( EVEN(sts)) {
      message( "Unable to find a visible object");
      return;
    }

    flow_SelectClear( flow_ctx);
    flow_SetHighlight( next, 1);
    flow_SelectInsert( flow_ctx, next);
  }
  else {
    sts = flow_GetNextObject( flow_ctx, sel, dir, &next);
    if ( EVEN(sts)) {
      message( "Unable to find next object");
      return;
    }

    flow_SelectClear( flow_ctx);
    flow_SetHighlight( next, 1);
    flow_SelectInsert( flow_ctx, next);

    if ( !flow_IsVisible( flow_ctx, next, flow_eVisible_Full))
      flow_CenterObject( flow_ctx, next);
  }
}
