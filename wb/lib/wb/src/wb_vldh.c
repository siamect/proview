/* wb_vldh.c -- very local data handler

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "wb_vldh.h"
#include "wb_vldh_msg.h"
#include "wb_foe_macros.h"
#include "wb_trv.h"
#include "wb_utl.h"

/*_Local variables_______________________________________________________*/
static 	vldh_t_plc	plc_root = 0;
static	pwr_tClassId	class_id[90];
static 	char		class_name[90][80] = {
				{"Order"},
				{"SOrder"},
				{"LOrder"},
				{"COrder"},
				{"DOrder"},
				{"Trans"},
				{"Step"},
				{"InitStep"},
				{"SubStep"},
				{"ConAnalog"},
				{"ConDigital"},
				{"ConGrafcet"},
				{"SsBegin"},
				{"SsEnd"},
				{"Point"},
				{"Di"},
				{"Do"},
				{"Dv"},
				{"Ai"},
				{"Ao"},
				{"Av"},
				{"Co"},
				{"GetDp"},
				{"GetAp"},
				{"OrderAct"},
				{"ShowPlcAttr"},
				{"POrder"},
				{"GetDi"},
				{"GetDo"},
				{"GetDv"},
				{"GetAi"},
				{"GetAo"},
				{"GetAv"},
				{"And"},
				{"StoDo"},
				{"StoDv"},
				{"StoAo"},
				{"StoAv"},
				{"SetDo"},
				{"SetDv"},
				{"ResDo"},
				{"ResDv"},
				{"StoDp"},
				{"StoAp"},
				{"GetPi"},
				{"SetDp"},
				{"ResDp"},
				{"$Node"},
				{"PlcPgm"},
				{"$PlantHier"},
				{"WindowPlc"},
				{"WindowCond"},
				{"WindowOrderact"},
				{"Reset_SO"},
				{"AAnalyse"},
				{"DAnalyse"},
				{"ConTrace"},
				{"CStoAo"},
				{"CStoAv"},
				{"CStoAp"},
				{"$NodeHier"},
				{"Pos3p"},
				{"Inc3p"},
				{"WindowSubstep"},
				{"StoAtoIp"},
				{"CStoAtoIp"},
				{"GetIpToA"},
				{"ExternRef"},
				{"Text"},
				{"$LibHier"},
				{"ConData"},
				{"Po"},
				{"StoDi"},
				{"SetDi"},
				{"ResDi"},
				{"GetData"},
				{"PlcThread"},
				{"StepDiv"},
				{"StepConv"},
				{"TransDiv"},
				{"TransConv"},
				};

/*_Function declarations__________________________________________________*/

static int vldh_get_con_defname (
  vldh_t_wind	wind,
  char		*name
);

static int vldh_get_node_defname (
  vldh_t_wind	wind,
  unsigned long	object_typ,
  char		*objname,
  char		*name
);

static int vldh_get_object_defname (
  ldh_tSesContext ldhses,
  pwr_tObjid Objdid,
  unsigned long	object_type,
  char		*objname,
  char		*name
);


static int vldh_node_load (
  vldh_t_wind	wind,
  pwr_tObjid objdid
);

static void cnv_to_neted( vldh_t_node n);
static void cnv_from_neted( vldh_t_node n);






/*_Local procedues_______________________________________________________*/


/*************************************************************************
*
* Name:		vldh_check_document( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* unsigned long	objdid		I	objdid
*
* Description:
*	Check if an object is a document object.
*	The criterium f|r a document is that it has graphmethod 6.
*	If the object is a document, 1 is returned,  else 0.
*
**************************************************************************/

unsigned long vldh_check_document ( 
  ldh_tSesContext ldhses,
  pwr_tObjid objdid
)
{
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	pwr_tClassId		class;
	
	/* Get graphbody i class object */
	sts = ldh_GetObjectClass( ldhses, objdid, &class);
	if( EVEN(sts) ) return 0;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return 0;

	if( graphbody->graphmethod == 6)
	  return 1;

	return 0;
}

/*************************************************************************
*
* Name:		vldh_check_plcpgm( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* unsigned long	objdid		I	objdid
*
* Description:
*	Check if an object is a plcpgm.
*	The criterium f|r a plcpgm is that it has a buffer of
*	type PlcProgram.
*	If the object is a plcpgm, 1 is returned,  else 0.
*
**************************************************************************/

unsigned long vldh_check_plcpgm (
  ldh_tSesContext ldhses,
  pwr_tObjid objdid
)
{
	int			sts, size;
	char			*plcbuffer;
	pwr_eClass		class;
	
	/* This is probably a plcprogram, check that it really is */
	sts = ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcProgram", &class,	
			(char **)&plcbuffer, &size);
	if( EVEN(sts)) 
	  /* This is not a plcprogram object */
	  return 0;

	/* This is a plcpgm */
	free((char *) plcbuffer);
	return 1;
}

#if 0
/*************************************************************************
*
* Name:		vldh_check_window( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* unsigned long	objdid		I	objdid
*
* Description:
*	Check if an object is a window.
*	The criterium f|r a window is that it has a buffer of
*	type PlcWindow.
*	If the object is a window, 1 is returned,  else 0.
*
**************************************************************************/

static unsigned long vldh_check_window ( 
  ldh_tSesContext ldhses,
  pwr_tObjid objdid
)
{
	int			sts, size;
	char			*windbuffer;
	pwr_eClass		eclass;
	
	/* Check if the buffer exists */
	sts = ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcWindow", &eclass,	
			(char **)&windbuffer, &size);
	if( EVEN(sts)) 
	  /* This is not a window object */
	  return 0;

	/* This is a window */
	free((char *) windbuffer);
	return 1;
}
#endif

/*************************************************************************
*
* Name:		vldh_class( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* int 		index		I	index defined in vldh.h (VLDH_CLASS...)
*
* Description:
*	Get objdid for a predefined class.
*
**************************************************************************/

pwr_tClassId	vldh_class ( 
  ldh_tSesContext ldhses,
  int index
)
{
	int	sts;
	
	if ( class_id[index] == 0 )
	{
	  sts = ldh_ClassNameToId( ldhses, &class_id[index], 
			class_name[index]);
	  if ( EVEN(sts)) printf("Error in classname\n");
	}
	return class_id[index];

}

/*************************************************************************
*
* Name:		vldh_uclass( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* char*		classname	I	hierarchy name for the class
*
* Description:
*	Converts hierarchy class name to objdid.
*
**************************************************************************/

pwr_tClassId	vldh_uclass (
  ldh_tSesContext ldhses,
  char	*classname
)
{
	int	sts;
	pwr_tClassId class;
	
	sts = ldh_ClassNameToId( ldhses, &class,	classname);
	if ( EVEN(sts)) printf("Error in classname\n");

	return class;

}


/*************************************************************************
*
* Name:		vldh_eclass( )
*
* Type		unsigned long
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session
* char*		classname	I	hierarchy name for the class
*
* Description:
*	Converts hierarchy class name to objdid.
*	Returns 0 if the class is unknown.
*
**************************************************************************/

pwr_tClassId vldh_eclass (
  ldh_tSesContext ldhses,
  char	*classname
)
{
	int	sts;
	pwr_tClassId class;
	
	sts = ldh_ClassNameToId( ldhses, &class,		
			 classname);
	if ( EVEN(sts))
	  return 0;

	return class;

}

/*************************************************************************
*
* Name:		vldh_get_con_defname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* char *	name		O	default connection name.
*
* Description:
*	Get the default name for a connection object.
*	The defaulname is 'CONNECTIONx' where x is a value
*	from stored in the plcobject and increased for every
*	new created connection.
*
**************************************************************************/

static int vldh_get_con_defname (
  vldh_t_wind	wind,
  char		*name
)
{
	vldh_t_plc		plc;

	/* Get the plc object */
	plc = wind->hw.plcobject_pointer;

        sprintf(name,"cn%d", plc->lp.connamecount);
	(plc->lp.connamecount)++;
	vldh_plcmodified( plc);

	return VLDH__SUCCESS;
}
/*************************************************************************
*
* Name:		vldh_get_node_defname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long	object_type	I	class
* char *	objname		I	first part of default object name.
* char *	name		O	default object name.
*
* Description:
*	Get the default name for a object.
*	The defaulname is objname given as input followed by a value
*	stored in the plcobject for each class and increased for every
*	new created object of this class in this plcpgm.
*
**************************************************************************/

static int vldh_get_node_defname (
  vldh_t_wind	wind,
  unsigned long	object_type,
  char		*objname,
  char		*name
)
{
	vldh_t_plc		plc;

	if ( object_type >= PWR_OBJTYPES_MAX ) return VLDH__BADOBJTYPE; 
	/* Get the plc object */
	plc = wind->hw.plcobject_pointer;

        sprintf(name,"%s%d",
	      objname,plc->lp.defnamecount[object_type]);
	(plc->lp.defnamecount[object_type])++;
	vldh_plcmodified( plc);
	return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_get_node_defname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long	object_type	I	class
* char *	objname		I	first part of default object name.
* char *	name		O	default object name.
*
* Description:
*	Get the default name for a object.
*	The defaulname is objname given as input followed by a value
*	stored in the plcobject for each class and increased for every
*	new created object of this class in this plcpgm.
*
**************************************************************************/

static int vldh_get_object_defname (
  ldh_tSesContext ldhses,
  pwr_tObjid Objdid,
  unsigned long	object_type,
  char		*objname,
  char		*name
)
{
	int	sts, size;
	pwr_sPlcProgram	*plcbuffer;
	pwr_eClass	 class;
	pwr_tObjid	*parentlist;
	unsigned long	parent_count;
	pwr_tObjid	plcobjdid;
	vldh_t_plc	plc;

	if ( object_type >= PWR_OBJTYPES_MAX ) return VLDH__BADOBJTYPE; 

	/* Get the parentlist for this object */
	sts = trv_get_parentlist( ldhses, Objdid, &parent_count, 
		&parentlist);
	if ( EVEN(sts)) return sts;

	/* Check if the plcpgm is loaded in vldh */
	plcobjdid = *(parentlist + parent_count -1);
	sts = vldh_get_plc_objdid( plcobjdid, &plc);
	if ( ODD(sts))
	{
          sprintf(name,"%s%d",
	      objname,plc->lp.defnamecount[object_type]);
	  (plc->lp.defnamecount[object_type])++;
	  vldh_plcmodified( plc);
	}
	else
	{
	  /* Get the object i ldh */
	  sts = ldh_GetObjectBuffer( ldhses,
		plcobjdid,
		"DevBody", "PlcProgram", &class,
		(char **)&plcbuffer, &size);
	  if( EVEN(sts)) return sts;

          sprintf(name,"%s%d",
	      objname, plcbuffer->defnamecount[object_type]);
	  (plcbuffer->defnamecount[object_type])++;

	  sts = ldh_SetObjectBuffer( ldhses, 
		plcobjdid,
		"DevBody",
		"PlcProgram",
		(char *)plcbuffer);
	  if ( EVEN(sts)) return sts;
	
	  free((char *) plcbuffer);
	}

	if ( parent_count > 0)
	  XtFree((char *) parentlist);
	return VLDH__SUCCESS;
}



/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		vldh_node_con_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	con_point	I	connection point in the node where
*					the connection is to be inserted.
* vldh_t_con	con		I	connection to be inserted.
* int		type		I 	source or destination part of the 
*					connection, not used.
*
* Description:
*	Insert a connection object in the connectionlist for the specified
*	nodeobject and connectionpoint.
*
**************************************************************************/

int vldh_node_con_insert (
  vldh_t_node	node,
  unsigned long	con_point,
  vldh_t_con	con,
  int		type
)
{
	int	sts;
	
	if ( con_point >= VLDH_MAX_CONPOINTS ) return VLDH__BADCONPOINT;

	if ( node->hn.con_count[con_point] == 0 )
	{
	  /* First con on this connectionpoint, calloc */
	  node->hn.con_list[con_point] =  (void *)XtCalloc(1, sizeof(vldh_t_con));
	}
	else
	{
	  /* Increase size of conlist */
	  sts = utl_realloc( (char **)&node->hn.con_list[con_point], 
		node->hn.con_count[con_point] * sizeof(vldh_t_con), 
		(node->hn.con_count[con_point] + 1) * sizeof(vldh_t_con));
	  if (EVEN(sts)) return sts;
	}
	/* Insert new connection object in conlist */
	*((vldh_t_con *) (node->hn.con_list[con_point]) + 
		node->hn.con_count[con_point])  = con;
	node->hn.con_count[con_point] ++;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_con_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	con_point	I	connection point in the node where
*					the connection is to be deleted.
* vldh_t_con	con		I	connection to be deleted.
*
* Description:
*	Eliminate a connection object in the connectionlist for the specified
*	nodeobject and connectionpoint.
*
**************************************************************************/

int vldh_node_con_delete (
  vldh_t_node	node,
  unsigned long	con_point,
  vldh_t_con	con
)
{
	int	i;

	/* Find the conobject in conlist and insert 0 */
	for ( i = 0; i < node->hn.con_count[con_point]; i++)
	{
	  if ( *((vldh_t_con *) (node->hn.con_list[con_point]) + i) == con )
	  {
	    /* Connection found */
	    *((vldh_t_con *) (node->hn.con_list[con_point]) + i) = 0;
	    vldh_nodemodified( node);
	    return VLDH__SUCCESS;
	  }
	}
	return VLDH__OBJNOTFOUND;
}



/*************************************************************************
*
* Name:		vldh_node_subwind_created()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node.
* vldh_t_wind	subwind		I	vldh window for subwindow.
* unsigned long	windowindex	I	windowindex for subwindow.
*
* Description:
*	Insert information in the parent node when a subwindow is created.
*
**************************************************************************/

int vldh_node_subwindow_created (
  vldh_t_node	node,
  vldh_t_wind	subwind,
  unsigned long	windowindex
)
{
	/* Store the objdid and wind for the subwindow */
	node->ln.subwind_objdid[ windowindex ] = subwind->lw.objdid;
	node->hn.subwindowobject[ windowindex ] = subwind;
	vldh_nodemodified( node);
	node->ln.subwindow |= ( windowindex + 1);

	return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_node_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* unsigned long	class		I	class of the new object.
* vldh_t_node * node		O	created vldh node.
*
* Description:
*	Create a node object.
*	The node is created in vldh and an object of the specified 
*	class is created in ldh. The name of the object is fetched from
*	objname in graphbody with an index value added.
*
**************************************************************************/

int vldh_node_create (
  vldh_t_wind	wind,
  pwr_tClassId 	class,
  vldh_t_node	*node
)
{
	char			segment_name[32];
	pwr_tObjid objdid;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;


	  /* Get graphbody for the class */
	  sts = ldh_GetClassBody(wind->hw.ldhsession, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	  if ( EVEN(sts)) return sts;

	  /* Get default segment name */
	  for (;;)
	  {
	    sts = vldh_get_node_defname( wind, graphbody->object_type, 
		graphbody->objname, segment_name);
	    if ( EVEN(sts) ) return sts;
	  
	    /* Create the object i ldh */
	    sts = ldh_CreateObject( wind->hw.ldhsession, &objdid, segment_name, 
		class, wind->lw.objdid, ldh_eDest_IntoLast);
	    if ( sts == LDH__NAMALREXI)
	      /* Try again with incremented name index */
	      continue;
	    else if ( EVEN(sts)) 
	      return sts;
	    break;
	  }
	  /* Create node in vldh */
 	  *node = (void *)XtCalloc(1, sizeof(**node));

	  (*node)->ln.mask[0] = graphbody->default_mask[0];
	  (*node)->ln.mask[1] = graphbody->default_mask[1];
	  (*node)->ln.mask[2] = 0;
	  (*node)->ln.object_type = graphbody->object_type;
	  (*node)->ln.compdirection = graphbody->compindex;

	  (*node)->ln.window_did = wind->lw.objdid;
	  (*node)->ln.classid = class;
	  (*node)->ln.object_did = objdid;
	  (*node)->hn.vldhtype = VLDH_NODE;
	  (*node)->hn.status = VLDH_CREATE;
	  (*node)->hn.window_pointer = wind;
	  strcpy(((*node)->hn.name), segment_name);
	  (*node)->hn.object_pointer = wind->hw.node_list_pointer;
	  wind->hw.node_list_pointer = *node;

	  /* Set the buffer in ldh so the mask can be accessed by ate */
	  sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		(*node)->ln.object_did,
		"DevBody",
		"PlcNode",
		(char *)&((*node)->ln));
	  if( EVEN(sts)) return sts;

	  /* Do some class specific actions */
	  vldh_node_create_spec( *node);

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_load()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* unsigned long	objdid		I	objdid for the ldh object.
*
* Description:
*	Create a nodeobject in vldh from an ldhobject.
*
**************************************************************************/

static int vldh_node_load (
  vldh_t_wind	wind,
  pwr_tObjid objdid
)
{
	int	i, size, sts;
	vldh_t_node	node;
	pwr_sPlcNode	*nodebuffer;
	pwr_eClass	eclass;
	pwr_tClassId	class;


	  /* Create node in vldh */
 	  node = (void *)XtCalloc(1, sizeof(*node));

	  /* Get the object i ldh */
	  sts = ldh_GetObjectBuffer( wind->hw.ldhsession,
		objdid,	"DevBody", "PlcNode", &eclass,	
		(char **)&nodebuffer, &size);
	  if( EVEN(sts)) return sts;

	  sts = ldh_GetObjectClass( wind->hw.ldhsession,
		objdid,	&class);
	  if( EVEN(sts) ) return sts;

	  memcpy( &(node->ln), nodebuffer, sizeof(*nodebuffer));
	  free((char *) nodebuffer);

	  /* Get the object name from ldh */
	  sts = ldh_ObjidToName( wind->hw.ldhsession, objdid, ldh_eName_Object,
		node->hn.name, sizeof( node->hn.name), &size);
	  if( EVEN(sts)) return sts;

	  node->hn.status = VLDH_LOAD;
	  node->hn.vldhtype = VLDH_NODE;
	  node->hn.window_pointer = wind;
	  node->hn.subwindowobject[0] = 0;
	  node->hn.subwindowobject[1] = 0;
	  node->ln.window_did = wind->lw.objdid;
	  node->ln.classid = class;
	  node->ln.object_did = objdid;

	  for ( i = 0; i < VLDH_MAX_CONPOINTS; i++ ) 
	    node->hn.con_count[i] = 0;
	  node->hn.object_pointer = wind->hw.node_list_pointer;
	  wind->hw.node_list_pointer = node;

	  cnv_from_neted( node);

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Delete a node in vldh.
*
**************************************************************************/

int vldh_node_delete (
  vldh_t_node	node
)
{
	node->hn.status |= VLDH_DELETE;
	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_undelete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Undelete a node that is deleted in vldh but not yet in ldh.
*
**************************************************************************/

int vldh_node_undelete (
  vldh_t_node	node
)
{
	if ( node->hn.status & VLDH_DELETE ) 
	  node->hn.status -= VLDH_DELETE;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_con_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* unsigned long	class		I	connection class.
* unsigned long	drawtype	I	drawtype, reference or line.
* vldh_t_node	source_node	I	node connected to the source part
*					of the connection.
* unsigned long	source_point	I	connection point on the source node.
* vldh_t_node	dest_node	I	node connected to the destination part
*					of the connection.
* unsigned long	dest_point	I	connection point on the destination 
*					node.
* vldh_t_con *	con		O	create vldh connection.
*
* Description:
*	Create a connection in vldh and an object in ldh.
*
**************************************************************************/

int vldh_con_create (
  vldh_t_wind	wind,
  pwr_tClassId	class,
  unsigned long	drawtype,
  vldh_t_node	source_node,
  unsigned long	source_point,
  vldh_t_node	dest_node,
  unsigned long	dest_point,
  vldh_t_con	*con
)
{
	char			segment_name[32];
	pwr_tObjid 		objdid;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcConnection	*graphbody;


	  /* Get graphbody for the class */
	  sts = ldh_GetClassBody(wind->hw.ldhsession, class, "GraphPlcCon", 
		&bodyclass, (char **)&graphbody, &size);
	  if( EVEN(sts)) return sts;

	  for (;;)
	  {
	    vldh_get_con_defname( wind, segment_name);
	  
	    /* Create the object i ldh */
	    sts = ldh_CreateObject( wind->hw.ldhsession, &objdid, segment_name, 
		class, wind->lw.objdid, ldh_eDest_IntoFirst);
	    if ( sts == LDH__NAMALREXI)
	      /* Try again with incremented name index */
	      continue;
	    else if ( EVEN(sts)) 
	      return sts;
	    break;
	  }

	  /* Create con in vldh */
 	  *con = (void *)XtCalloc(1, sizeof(**con));

	  (*con)->lc.objdid = objdid;
	  (*con)->lc.object_type = graphbody->con_type;
	  (*con)->lc.curvature = graphbody->curvature;
	  (*con)->lc.classid = class;
	  (*con)->lc.drawtype = drawtype;
	  (*con)->lc.attributes = graphbody->attributes;
	  (*con)->lc.refnr = 0;
	  (*con)->hc.source_node_pointer = source_node;
	  (*con)->lc.source_point = source_point;
	  (*con)->lc.source_node_did = source_node->ln.object_did;
	  (*con)->hc.dest_node_pointer = dest_node;
	  (*con)->lc.dest_point = dest_point;
	  (*con)->lc.dest_node_did = dest_node->ln.object_did;
	  (*con)->lc.point_count = 0;
	  (*con)->hc.vldhtype = VLDH_CON;
	  (*con)->hc.status = VLDH_CREATE;
	  (*con)->hc.window_pointer = wind;
	  (*con)->lc.window_did = wind->lw.objdid;

	  (*con)->hc.object_pointer = 
				wind->hw.con_list_pointer;
	  wind->hw.con_list_pointer = *con;
	  vldh_node_con_insert(source_node,source_point,
				*con,VLDH_NODE_SOURCE);
	  vldh_node_con_insert(dest_node,dest_point,
				*con,VLDH_NODE_DESTINATION);

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_con_load()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* unsigned long	objdid		I	objdid of connection object.
*
* Description:
*	Load a connection object in vldh.
*	Create a connection in vldh and load info from 
*	the ldhobject into the connection.
*
**************************************************************************/

int vldh_con_load (
  vldh_t_wind	wind,
  pwr_tObjid objdid
)
{
	int		size, sts;
	vldh_t_con	con;
	pwr_sPlcConnection	*conbuffer;
	pwr_tClassId	class;
	pwr_eClass	eclass;

	  /* Create con in vldh */
 	  con = (void *)XtCalloc(1, sizeof(*con));

	  /* Get the object i ldh */
	  sts = ldh_GetObjectBuffer( wind->hw.ldhsession,
		objdid,	"DevBody", "PlcConnection", &eclass,
		(char **)&conbuffer, &size);
	  if( EVEN(sts)) return sts;

	  sts = ldh_GetObjectClass( wind->hw.ldhsession,
		objdid,	&class);
	  if( EVEN(sts) ) return sts;

	  memcpy( &(con->lc), conbuffer, sizeof(*conbuffer));
	  free((char *) conbuffer);

	  con->hc.vldhtype = VLDH_CON;
	  con->hc.status = VLDH_LOAD;
	  con->hc.window_pointer = wind;
	  con->hc.source_node_pointer = 0;
	  con->hc.dest_node_pointer = 0;
	  con->lc.objdid = objdid;
	  con->lc.classid = class;
	  con->hc.window_pointer = wind;

	  con->hc.object_pointer = wind->hw.con_list_pointer;
	  wind->hw.con_list_pointer = con;

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_con_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_con	con		I	vldh connection
*
* Description:
*	Delete a connection in vldh.
*
**************************************************************************/

int vldh_con_delete (
  vldh_t_con	con
)
{
	con->hc.status |= VLDH_DELETE;
	vldh_node_con_delete(con->hc.source_node_pointer,
		con->lc.source_point, con);
	vldh_node_con_delete(con->hc.dest_node_pointer,
		con->lc.dest_point, con);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_con_undelete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_con	con		I	vldh connection
*
* Description:
*	Undelete a connectionobject that is preiously deleted in
*	vldh but not yet in ldh.
*	Resets the delete bit in the statusword and inserts the connection
*	in the connected nodes conlists.
*
**************************************************************************/

int vldh_con_undelete (
  vldh_t_con	con
)
{
	if ( con->hc.status & VLDH_DELETE ) 
	  con->hc.status -= VLDH_DELETE;
	vldh_node_con_insert(con->hc.source_node_pointer,
		con->lc.source_point, con, VLDH_NODE_SOURCE);
	vldh_node_con_insert(con->hc.dest_node_pointer,
		con->lc.dest_point, con, VLDH_NODE_DESTINATION);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_con_nodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_con	con		I	vldh connection.
* vldh_t_node * source_node	O	source node
* vldh_t_node *	dest_node	O	destination node.
*
* Description:
*	Returns the source and destination nodes for a connection.
*
**************************************************************************/

int vldh_get_con_nodes (
  vldh_t_con	con,
  vldh_t_node	*source_node,
  vldh_t_node	*dest_node
)
{
	*source_node = con->hc.source_node_pointer;
	*dest_node = con->hc.dest_node_pointer;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_con_getrefnr()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_con	con		I	vldh connection.
*
* Description:
*	Get a number for the annotation in a reference connection.
*	The number is fetched from the window object which contains
*	a counter that increments for every new reference connection.
*
**************************************************************************/

int vldh_con_getrefnr (
  vldh_t_con	con
)
{
	vldh_t_wind	wind;

	wind = con->hc.window_pointer;
 	con->lc.refnr = wind->lw.refconcount;
	(wind->lw.refconcount)++;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_quit()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Routine that removes a window and all nodes and connections
*	in the window in vldh.
*	Frees all memory allocated in a window and sets zero in the
*	subwindowpointer in the parent object. The application should
*	make sure that no subwindows to this window exists in vldh.
*
**************************************************************************/

int vldh_wind_quit (
  vldh_t_wind	wind
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	unsigned long		con_count;
	vldh_t_con		*conlist;
	vldh_t_con		*con_ptr;
	int			i,j;
	vldh_t_plc		plc;
	vldh_t_plc		*plc_pointer;
	int			sts;

	/* Free all the  nodes */
	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  for ( j = 0; j < VLDH_MAX_CONPOINTS; j++ ) 
	  {
	    if ( (*node_ptr)->hn.con_count[j] > 0 )
	    {
	      XtFree((char *)(*node_ptr)->hn.con_list[j]);
	    }
	  }
	  XtFree((char *) *node_ptr);
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Free all the connections */
	sts = vldh_get_cons( wind, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  XtFree((char *) *con_ptr);
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);
	if( wind->hw.parent_node_pointer == 0 )
	{
	  /* Parent is a plc, remove the plc */
	  plc = wind->hw.plcobject_pointer;

	  /* Close the session for the plc */
	  sts = ldh_CloseSession( plc->hp.ldhsesctx);

	  /* Delete from the plc list */
	  plc_pointer = &plc_root;
	  while ( *plc_pointer != 0 )
	  {
	    if ( *plc_pointer == plc )
	    {
	      /* This is it */
	      *plc_pointer = (*plc_pointer)->hp.plcobj_pointer;
	      break;
	    }
	    /* Next object */
	    plc_pointer = &((*plc_pointer)->hp.plcobj_pointer);
	  }
	  XtFree((char *) plc);
	}
	/* Close the session for the window */
	sts = ldh_CloseSession( wind->hw.ldhsession);

	/* Free the window */
	XtFree((char *) wind);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_quit_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Removes a window from vldh 
*	and reverts the ldhsession of the window.
*
**************************************************************************/

int vldh_wind_quit_all (
  vldh_t_wind	wind
)
{
	int	sts;

	sts = ldh_RevertSession( wind->hw.ldhsession);
	if( EVEN(sts)) return sts;
	
	vldh_wind_quit(wind);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_save()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Saves all modifications done in a window in ldh.
*	The plc of the window is always saved in the navigator session.
*	Dependent of the statusword in the vldhobjects the window,
*	nodes and connections are saved, deleted or ignored.
*	If the window is created the subwindowmark in the parent node
*	is saved.
*
**************************************************************************/

int vldh_wind_save (
  vldh_t_wind	wind
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	unsigned long		con_count;
	vldh_t_con		*conlist;
	vldh_t_con		*con_ptr;
	vldh_t_plc		plc;
	int			i, j, sts, size;
	pwr_sPlcNode		*nodebuffer;
	pwr_eClass		eclass;
	vldh_t_wind		subwindow;
	pwr_tObjid		ln_subwind_objdid[VLDH_MAX_SUBWINDOWS];
	unsigned long		ln_subwindow;
	pwr_tTime		time;

	/* Save the plc */
	plc = wind->hw.plcobject_pointer;

	/* Save the plc in its own session */
	/* Plc session may well be readonly, set readwrite */
	sts = ldh_SetSession( plc->hp.ldhsesctx, 
			ldh_eAccess_ReadWrite);
	if ( EVEN(sts)) return sts;

	sts = ldh_SetObjectBuffer(
		plc->hp.ldhsesctx,
		plc->lp.objdid,
		"DevBody",
		"PlcProgram",
		(char *)&(plc->lp));
	if( sts == LDH__NOSUCHOBJ ) return VLDH__PLCNOTSAVED;
	if( EVEN(sts)) return sts;

	/* Save the plc session */
	sts = ldh_SaveSession( plc->hp.ldhsesctx);
	if( EVEN(sts)) return sts;

	sts = ldh_SetSession( plc->hp.ldhsesctx,
			ldh_eAccess_ReadOnly);
	if ( EVEN(sts)) return sts;

	/* If window is created, save the subwindowmark in the parent node */
	if ( wind->hw.parent_node_pointer != 0)
	{
	  /* Check if the window is created and not saved */
	  if ( (wind->hw.status & VLDH_CREATE) != 0 )
	  {
	    /* The parentnode is saved but without subwindowdata because
	       the subwindow was not saved, add subwindowdata to the
	       object in ldh */

	    sts = ldh_GetObjectBuffer( wind->hw.ldhsession,
			wind->lw.parent_node_did, "DevBody",
			"PlcNode", &eclass,	
			(char **)&nodebuffer, &size);
	    if( EVEN(sts)) return sts;
	    nodebuffer->subwind_objdid[ wind->lw.subwindowindex ] = 
			wind->lw.objdid;
	    nodebuffer->subwindow |= (wind->lw.subwindowindex + 1);
	      


	    sts = ldh_SetObjectBuffer(
			wind->hw.ldhsession,
			wind->lw.parent_node_did,
			"DevBody",
			"PlcNode",
			(char *)nodebuffer);
	    if( EVEN(sts)) return sts;
	    free((char *)nodebuffer);
	  }
	}

	/* Save the wind */
	if ( (wind->hw.status & VLDH_DELETE) != 0 )
	{
	  /* Window deleted during this session */
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
		wind->lw.objdid);
	  if( EVEN(sts)) return sts;
	  wind->hw.status = VLDH_DELETE | VLDH_LDHDELETE;
	}
	else
	{
	  /* Wind modified or created during this session */
	  sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		wind->lw.objdid,
		"DevBody",
		"PlcWindow",
		(char *)&(wind->lw));
	  if( EVEN(sts)) return sts;
	  wind->hw.status = VLDH_LOAD;

	  /* Store the modification time */
	  clock_gettime(CLOCK_REALTIME, &time);
	  sts = ldh_SetObjectPar( wind->hw.ldhsession, wind->lw.objdid,
		"DevBody", "Modified", (char *)&time, sizeof( time)); 
	}

	/* Save the nodes that is created or modified */
	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  if ( (((*node_ptr)->hn.status & VLDH_MODIFY) != 0 ) || 
	 	    (((*node_ptr)->hn.status & VLDH_CREATE) != 0 ))
	  {
	    /* Node modified or created during this session */
	    /* Look for any created and not saved subwindows,
	       these should not be saved until the subwindow is saved
	       in case of quitting the subwindow */
	    for ( j = 0; j < VLDH_MAX_SUBWINDOWS; j++)
	      ln_subwind_objdid[j] = (*node_ptr)->ln.subwind_objdid[j]; 
	    ln_subwindow = (*node_ptr)->ln.subwindow;
	    for ( j = 0; j < VLDH_MAX_SUBWINDOWS; j++)
	    {
	      if ( (*node_ptr)->hn.subwindowobject[j] != 0 )
	      {
	        subwindow = (*node_ptr)->hn.subwindowobject[j];
	        if ( (subwindow->hw.status & VLDH_CREATE) != 0)
	        {
	          /* Created not saved subwindow found */
	          (*node_ptr)->ln.subwind_objdid[j] = pwr_cNObjid;
	          if ( ((*node_ptr)->ln.subwindow & (1 << j)) != 0 )
	            (*node_ptr)->ln.subwindow -= (1 << j);
		}
	      }	      
	    }

	    cnv_to_neted( *node_ptr);

	    sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		(*node_ptr)->ln.object_did,
		"DevBody",
		"PlcNode",
		(char *)&((*node_ptr)->ln));
	    if( EVEN(sts)) return sts;

	    cnv_from_neted( *node_ptr);

	    /* Load the current subwindowstatus in vldh again */
	    for ( j = 0; j < VLDH_MAX_SUBWINDOWS; j++)
	      (*node_ptr)->ln.subwind_objdid[j] = ln_subwind_objdid[j]; 
	    (*node_ptr)->ln.subwindow = ln_subwindow;
	    (*node_ptr)->hn.status = VLDH_LOAD;
	  }
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Delete the nodes that has been delete during this session */
	sts = vldh_get_nodes_del( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  /* Node deleted during this session */
	  sts = vldh_node_ldhdel_spec( *node_ptr);
	  if( EVEN(sts)) return sts;
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did);
	  if( EVEN(sts)) return sts;
	  (*node_ptr)->hn.status = VLDH_DELETE | VLDH_LDHDELETE;
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Save the cons that is created or modified */
	vldh_get_cons( wind, &con_count, &conlist);
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  if ( (((*con_ptr)->hc.status & VLDH_MODIFY) != 0) ||
	       (((*con_ptr)->hc.status & VLDH_CREATE) != 0 ))
	  {
	    /* Con modified or created during this session */
	    sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		(*con_ptr)->lc.objdid,
		"DevBody",
		"PlcConnection",
		(char *)&((*con_ptr)->lc));
	    if( EVEN(sts)) return sts;
	    (*con_ptr)->hc.status = VLDH_LOAD;

	  }
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);

	/* Delete the cons that has been delete during this session */
	sts = vldh_get_cons_del( wind, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  /* Con deleted during this session */
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*con_ptr)->lc.objdid);
	  if( EVEN(sts)) return sts;
	  (*con_ptr)->hc.status = VLDH_DELETE | VLDH_LDHDELETE;
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);
	/* Save the ldh session */
	sts = ldh_SaveSession( wind->hw.ldhsession);
	if( EVEN(sts)) return sts;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_delete_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Deletes the window, all nodes and connections in vldh and
*	in ldh and saves the ldh session.
*
**************************************************************************/

int vldh_wind_delete_all (
  vldh_t_wind	wind
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	unsigned long		con_count;
	vldh_t_con		*conlist;
	vldh_t_con		*con_ptr;
	int			i, sts;
	ldh_tSesContext ldhsession;

	/* Store the ldh session ctx */
	ldhsession = wind->hw.ldhsession;

	/* Delete all the nodes */
	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = vldh_node_ldhdel_spec( *node_ptr);
	  if( EVEN(sts)) return sts;
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did);
	  if( EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Delete the 'deleted' nodes */
	sts = vldh_get_nodes_del( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = vldh_node_ldhdel_spec( *node_ptr);
	  if( EVEN(sts)) return sts;
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did);
	  if( EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Delete the cons */
	sts = vldh_get_cons( wind, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*con_ptr)->lc.objdid);
	  if( EVEN(sts)) return sts;
	  con_ptr++;
	}
	if( con_count > 0) XtFree((char *) conlist);

	/* Delete the 'deleted' cons */
	sts = vldh_get_cons_del( wind, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  sts = ldh_DeleteObject( wind->hw.ldhsession, 
			(*con_ptr)->lc.objdid);
	  if( EVEN(sts)) return sts;
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);

	/* Delete the wind */
	sts = ldh_DeleteObject( wind->hw.ldhsession, 
		wind->lw.objdid);
	if( EVEN(sts)) return sts;

	/* Save the ldh session */
	sts = ldh_SaveSession( ldhsession);
	if( EVEN(sts)) return sts;

	/* Free the memory in vldh */
	vldh_wind_quit(wind);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_load_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	wind		I	vldh window.
*
* Description:
*	Loads a nodes and connections in a window from ldh to vldh.
*
**************************************************************************/

int vldh_wind_load_all (
  vldh_t_wind wind
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node;
	unsigned long		con_count;
	vldh_t_con		*conlist;
	vldh_t_con		*con_ptr;
	int			i, j;
	pwr_tObjid		next_objdid;
	pwr_eClass		eclass;
	int			sts, size;
	char			*nodebuffer;
	unsigned char		source_found;
	unsigned char		dest_found;

	/* Get the first child to the window */
	sts = ldh_GetChild( wind->hw.ldhsession, wind->lw.objdid, &next_objdid);

	while ( ODD(sts) )
	{

	  /* Check if node or connection */
	  sts = ldh_GetObjectBuffer( wind->hw.ldhsession,
		next_objdid, "DevBody", "PlcNode", &eclass,	
		(char **)&nodebuffer, &size);

	  if ( ODD(sts) )
	  {
	    /* This is a node */
	    free((char *) nodebuffer);
	    sts = vldh_node_load( wind, next_objdid);
	    if( EVEN(sts) ) return sts;

	  }
	  else
	  {
	    /* This is a connection */
	    sts = vldh_con_load( wind, next_objdid);
	    if( EVEN(sts) ) return sts;
	  }
	  sts = ldh_GetNextSibling( wind->hw.ldhsession, 
			next_objdid, &next_objdid);
	}
	/* Get source and destination nodes for the connections */
	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	sts = vldh_get_cons( wind, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  source_found = 0;
	  dest_found = 0;
	  node = nodelist;
	  for ( j = 0; j < node_count; j++)
	  {
	    if ( cdh_ObjidIsEqual( (*con_ptr)->lc.source_node_did,
			(*node)->ln.object_did))
	    {
	      /* Sourcenode found */
	      (*con_ptr)->hc.source_node_pointer = *node;
	      vldh_node_con_insert( *node,
		(*con_ptr)->lc.source_point, *con_ptr, VLDH_NODE_SOURCE);
	      source_found = 1;
	      if ( dest_found ) break; 
	    }
	    if ( cdh_ObjidIsEqual( (*con_ptr)->lc.dest_node_did,
			(*node)->ln.object_did))
	    {
	      /* Destinationnode found */
	      (*con_ptr)->hc.dest_node_pointer = *node;
	      vldh_node_con_insert( *node,
		(*con_ptr)->lc.dest_point, *con_ptr, VLDH_NODE_DESTINATION);
	      dest_found = 1;
	      if ( source_found ) break; 
	    }
	    node++;
	  }
#if 0
	  /* LOOK FOR A BUGG!!! */
	  if ( !(source_found && dest_found ))
	  {
	    /* Source or destination is missing, delete the connection */
	    (*con_ptr)->hc.status |= VLDH_DELETE;
	    if ( source_found)
	      vldh_node_con_delete((*con_ptr)->hc.source_node_pointer,
		(*con_ptr)->lc.source_point, *con_ptr);
	    if ( dest_found )
	      vldh_node_con_delete( (*con_ptr)->hc.dest_node_pointer,
		(*con_ptr)->lc.dest_point, *con_ptr);
	    printf("LOAD ERROR CON con: %d source: %d pnt: %d dest: %d pnt: %d \n",
	  	(*con_ptr)->lc.objdid,
		(*con_ptr)->lc.source_node_did,
		(*con_ptr)->hc.source_node_pointer,
		(*con_ptr)->lc.dest_node_did,
		(*con_ptr)->hc.dest_node_pointer);
	  }
	  /* END LOOK FOR A BUGG!!! */
#endif
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);
	if ( node_count > 0) XtFree((char *) nodelist);
	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_plc	plc		I	the plc of the window
* vldh_t_node	parentnode	I	the node that has this window as a
*					subwindow, = 0 if parent is a plc.
* void		*foectx		I	foe context
* unsigned long	class		I	window class
* unsigned long	widget_id	I	neted widget id
* unsigned long	subwindowindex	I	windowindex
* vldh_t_wind	* wind		O	created vldh window
*
* Description:
*	Create a window in vldh and a windowobject of specified 
*	class in ldh.
*
**************************************************************************/

int vldh_wind_create ( 
  vldh_t_plc	plc,
  vldh_t_node	parentnode,
  void	*foectx,
  pwr_tClassId	class,
  Widget	widget_id,
  unsigned long	subwindowindex,
  vldh_t_wind	*wind,
  ldh_eAccess	access
)
{
	char			*segment_name;
	pwr_tObjid 		objdid;
	ldh_tSesContext 	ldhsession;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcWindow 	*graphbody;
	pwr_tObjid		parent_objdid;

	  /* Open a session for this window */
	  sts = ldh_OpenSession( &ldhsession, 
		ldh_SessionToVol( plc->hp.ldhsesctx), 
		access, ldh_eUtility_PlcEditor);
	  if( EVEN(sts) ) return sts;

	  /* Get graphbody for the class */
	  sts = ldh_GetClassBody(ldhsession, class, "GraphPlcWindow", 
		&bodyclass, (char **)&graphbody, &size);
	  if( EVEN(sts) ) return sts;

	  /* Get default segment name */
	  segment_name = graphbody->objname;
	  
	  /* Get objdid for the parent */
	  if ( parentnode == 0 )
	    parent_objdid = plc->lp.objdid;
	  else
	    parent_objdid = parentnode->ln.object_did;

	  /* Create the object i ldh */
	  sts = ldh_CreateObject( ldhsession, &objdid, segment_name, class,
		parent_objdid,	ldh_eDest_IntoFirst);
	  if( EVEN(sts) ) return sts;

	  /* Create window in vldh */
 	  *wind = (void *)XtCalloc(1, sizeof(**wind));

	  /* Get the full hierarky name */
/*TEST*/  strcpy((*wind)->hw.full_name, "full-window-name");
	  strcpy((*wind)->hw.object_name, segment_name);
	
	  (*wind)->hw.ldhsession = ldhsession;
	  (*wind)->lw.object_type = graphbody->window_type;
	  (*wind)->lw.classid = class;
	  (*wind)->lw.objdid = objdid;
	  (*wind)->lw.refconcount = 1;
	  (*wind)->hw.plcobject_pointer = plc;
	  (*wind)->hw.foectx = foectx;
	  (*wind)->hw.parent_node_pointer = parentnode;
	  if ( parentnode != 0 )
	    (*wind)->lw.parent_node_did = parentnode->ln.object_did;
	  else
	    (*wind)->lw.parent_node_did = pwr_cNObjid;
	  (*wind)->hw.widget_id = widget_id;
	  (*wind)->lw.subwindowindex = subwindowindex;
	  (*wind)->hw.status = VLDH_CREATE;

	  (*wind)->hw.node_list_pointer = 0;
	  (*wind)->hw.con_list_pointer = 0;

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_load()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_plc	plc		I	the plc of the window
* vldh_t_node	parentnode	I	the node that has this window as a
*					subwindow, = 0 if parent is a plc.
* unsigned long	objdid		I 	objdid of the windowobject.
* void	      *foectx		I	foe context
* Widget	widget_id	I	neted widget id
* vldh_t_wind	* wind		O	created vldh window
*
* Description:
*	Load a window object from ldh to vldh.
*
**************************************************************************/

int vldh_wind_load (
  vldh_t_plc	plc,
  vldh_t_node	parentnode,
  pwr_tObjid 	objdid,
  void		*foectx,
  Widget	widget_id,
  vldh_t_wind	*wind,
  ldh_eAccess	access
)
{
	ldh_tSesContext ldhsession;
	int		sts, size;
	pwr_sPlcWindow	*windbuffer;
	pwr_tClassId	class;
	pwr_eClass	eclass;

	  /* Create node in vldh */
 	  *wind = (void *)XtCalloc(1, sizeof(**wind));

	  /* Open a session for plcobjects only */
	  sts = ldh_OpenSession(&ldhsession, 
		ldh_SessionToVol( plc->hp.ldhsesctx),
		access, ldh_eUtility_PlcEditor);
	  if( EVEN(sts) ) return sts;

	  /* Get the object i ldh */
	  sts = ldh_GetObjectBuffer( ldhsession,
		objdid,	"DevBody", "PlcWindow", &eclass,	
		(char **)&windbuffer, &size);
	  if( EVEN(sts)) return sts;

	  memcpy( &(*wind)->lw, windbuffer, sizeof(*windbuffer));
	  free((char *) windbuffer);

	  /* Get the object name from ldh */
	  sts = ldh_ObjidToName( ldhsession, objdid, ldh_eName_Object,
		(*wind)->hw.object_name, sizeof((*wind)->hw.object_name), &size);
	  if( EVEN(sts)) return sts;

	  sts = ldh_GetObjectClass( ldhsession, objdid, &class);
	  if( EVEN(sts) ) return sts;

	  /* Get the full hierarchy name */
/*TEST*/  strcpy((*wind)->hw.full_name, "full-window-name");
	
	  (*wind)->hw.ldhsession = ldhsession;
	  (*wind)->hw.plcobject_pointer = plc;
	  (*wind)->hw.foectx = foectx;
	  (*wind)->hw.parent_node_pointer = parentnode;
	  (*wind)->hw.widget_id = widget_id;
	  (*wind)->hw.status = VLDH_LOAD;
	  (*wind)->lw.classid = class;
	  (*wind)->lw.objdid = objdid;

	  (*wind)->hw.node_list_pointer = 0;
	  (*wind)->hw.con_list_pointer = 0;

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_wind_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Delete a window in vldh.
*
**************************************************************************/

int vldh_wind_delete (
  vldh_t_wind	wind
)
{
	wind->hw.status |= VLDH_DELETE;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_plc_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	objdid		I	objdid of the plcpgm object.
* ldh_tWBContext ldhwbctx	I	ldh workbench.
* ldh_tSesContext ldhses; ldhsesctx	I	ldh session.
* vldh_t_plc	*plc		O	created plc.
*
* Description:
*	Create a plcobject in vldh. 
*
**************************************************************************/

int vldh_plc_create (
  pwr_tObjid	  objdid,
  ldh_tWBContext  ldhwbctx,
  ldh_tSesContext ldhsesctx,
  vldh_t_plc	*plc
)
{
	int	i, sts, size;
	pwr_tClassId bodyclass;
	pwr_sGraphPlcProgram *graphbody;
	ldh_tSesContext ldhsession;

 	  *plc = (void *)XtCalloc(1, sizeof(**plc));

	  /* Open a session for the plc */
	  sts = ldh_OpenSession( &ldhsession, ldh_SessionToVol( ldhsesctx), 
		ldh_eAccess_ReadOnly, ldh_eUtility_PlcEditor);

	  if( EVEN(sts) ) return sts;
	  /* Get the name and class of this objdid */
	  sts = ldh_GetObjectClass( ldhsesctx, objdid, &(*plc)->lp.classid);
	  if( EVEN(sts) ) return sts;
	  sts = ldh_ObjidToName( ldhsesctx, objdid, ldh_eName_Hierarchy,
		(*plc)->hp.object_name, sizeof( (*plc)->hp.object_name),
		&size); 
	  if( EVEN(sts) ) return sts;
	  sts = ldh_GetClassBody(ldhsesctx, (*plc)->lp.classid, "GraphPlcPgm", 
		&bodyclass, (char **)&graphbody, &size);
	  if( EVEN(sts) ) return sts;

	  (*plc)->lp.object_type = graphbody->plc_type;
	  (*plc)->lp.objdid = objdid;
	  (*plc)->lp.connamecount = 0;
	  (*plc)->hp.ldhwbctx = ldhwbctx;
	  (*plc)->hp.ldhsesctx = ldhsession;
	  (*plc)->hp.status = VLDH_CREATE;
	  for ( i = 0; i < MAX_OBJECTTYPES; i++) (*plc)->lp.defnamecount[i] = 0;

	  /* Insert in plcobject list */
	  (*plc)->hp.plcobj_pointer = plc_root;
	  plc_root = *plc;

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_plc_load()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	objdid		I	objdid of the plcpgm object.
* ldh_tWBContext ldhwbctx	I	ldh workbench.
* ldh_tSesContext ldhses; ldhsesctx	I	ldh session.
* vldh_t_plc	*plc		O	create plc.
*
* Description:
*	Load a plc in vldh.
*
**************************************************************************/

int vldh_plc_load (
  pwr_tObjid	  objdid,
  ldh_tWBContext  ldhwbctx,
  ldh_tSesContext ldhsesctx,
  vldh_t_plc	*plc
)
{
	int	sts, size;
	pwr_sPlcProgram	*plcbuffer;
	pwr_eClass	eclass;
	ldh_tSesContext ldhsession;

	  /* Open a session for the plc */
	  sts = ldh_OpenSession( &ldhsession, ldh_SessionToVol( ldhsesctx), 
		ldh_eAccess_ReadOnly, ldh_eUtility_PlcEditor);

 	  *plc = (void *)XtCalloc(1, sizeof(**plc));

	  /* Get the object i ldh */
	  sts = ldh_GetObjectBuffer( ldhsesctx,
		objdid,	"DevBody", "PlcProgram", &eclass,
		(char **)&plcbuffer, &size);
	  if( EVEN(sts)) return sts;

	  memcpy( &(*plc)->lp, plcbuffer, sizeof(*plcbuffer));
	  free((char *) plcbuffer);

	  /* Get the object name from ldh */
	  sts = ldh_ObjidToName( ldhsesctx, objdid, ldh_eName_Hierarchy,
		(*plc)->hp.object_name, sizeof( (*plc)->hp.object_name),
		&size);
	  if( EVEN(sts) ) return sts;

	  (*plc)->hp.ldhwbctx = ldhwbctx;
	  (*plc)->hp.ldhsesctx = ldhsession;
	  (*plc)->hp.status = VLDH_LOAD;

	  /* Insert in plcobject list */
	  (*plc)->hp.plcobj_pointer = plc_root;
	  plc_root = *plc;

	  return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_plc_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_plc	plc		I	vldh plc.
*
* Description:
*	Delete a plc in vldh.
*
**************************************************************************/

int vldh_plc_delete (
  vldh_t_plc	plc
)
{
	plc->hp.status |= VLDH_DELETE;

	return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_get_subwindows_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * wind_count	O	number of windowsin windlist
* void **	intern_windlist	O	list of subwindows 
*
* Description:
*	Returns subwindows to a window at one level, that is 
*	subwindows that are children of nodes in the windows.
*	Only windows loaded in vldh is returned.
*	When the caller is finished with the windowlist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_wind_subwindows ( 
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
)
{
	vldh_t_wind	*windlist_pointer;
	vldh_t_wind	wind_pointer;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, j;
	int			sts;

	*wind_count = 0;

	wind_pointer = wind;
	sts = vldh_get_nodes( wind, &node_count,
			&nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  for ( j = 0; j < VLDH_MAX_SUBWINDOWS; j++)
	  {
	    if ( (*node_ptr)->hn.subwindowobject[j] != 0 )
	    {
	      wind_pointer = (*node_ptr)->hn.subwindowobject[j];
	      if ( (wind_pointer->hw.status & VLDH_DELETE) == 0 )
	      {
	        wind_pointer = (*node_ptr)->hn.subwindowobject[j];
	        sts = utl_realloc( (char **)intern_windlist, 
			*wind_count * sizeof( *intern_windlist), 
			(*wind_count + 1) * sizeof(*intern_windlist));
	        if (EVEN(sts)) return sts;
	        windlist_pointer = *intern_windlist;
	        *(windlist_pointer + *wind_count) = wind_pointer;
	        (*wind_count)++;
	      }
	    }
	  }
	  node_ptr++;
	}
 	if ( node_count) XtFree((char *) nodelist);

	if( *wind_count == 0) return VLDH__NOWINDS;
	else return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_get_subwindows_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * wind_count	O	number of windowsin windlist
* void **	intern_windlist	O	list of subwindows 
*
* Description:
*	Recursive routine used by vldh_get_wind_windows.
*	Searches trough vldh for subwindows and accumulates them
*	in vector.
*
**************************************************************************/

int vldh_get_subwindows_all ( 
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
)
{
	vldh_t_wind	*windlist_pointer;
	vldh_t_wind	wind_pointer;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, j;
	int			sts;

	sts = vldh_get_nodes( wind, &node_count,
			&nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  for ( j = 0; j < VLDH_MAX_SUBWINDOWS; j++)
	  {
	    if ( (*node_ptr)->hn.subwindowobject[j] != 0 )
	    {
	      wind_pointer = (*node_ptr)->hn.subwindowobject[j];
	      if ( (wind_pointer->hw.status & VLDH_DELETE) == 0 )
	      {
	        wind_pointer = (*node_ptr)->hn.subwindowobject[j];
	        sts = utl_realloc( (char **)intern_windlist, 
			*wind_count * sizeof( *intern_windlist), 
			(*wind_count + 1) * sizeof(*intern_windlist));
	        if (EVEN(sts)) return sts;
	        windlist_pointer = *intern_windlist;
	        *(windlist_pointer + *wind_count) = wind_pointer;
	        (*wind_count)++;
	        sts = vldh_get_subwindows_all( wind_pointer, wind_count, intern_windlist);	
	        if ( EVEN(sts)) return sts;
	      }
	    }
	  }
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	if( *wind_count == 0) return VLDH__NOWINDS;
	else return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_get_wind_windows()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * wind_count	O	number of windowsin windlist
* void **	intern_windlist	O	list of subwindows 
*
* Description:
*	Returns all subwindows at all levels to a window.
*	The parent window is returnd first in the windowlist.
*	Only subwindows loaded in vldh is returned.
*	When the caller is finished with the windowlist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_wind_windows (
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
)
{
	vldh_t_wind	*windlist_pointer;
	vldh_t_wind	wind_pointer;
	int		sts;

	*wind_count = 0;

	wind_pointer = wind;
	if ( wind_pointer != 0 )
	{
	  if ( (wind_pointer->hw.status & VLDH_DELETE) == 0)
	  {
	    sts = utl_realloc( (char **)intern_windlist, 
			*wind_count * sizeof( *intern_windlist), 
			(*wind_count + 1) * sizeof(*intern_windlist));
	    windlist_pointer = *intern_windlist;
	    *(windlist_pointer + *wind_count) = wind_pointer;
	    (*wind_count)++;
	    sts = vldh_get_subwindows_all( wind_pointer, wind_count, intern_windlist);	
	    if ( EVEN(sts)) return sts;
	  }
	}
	if( *wind_count == 0) return VLDH__NOWINDS;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_plc_windows()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * wind_count	O	number of windowsin windlist
* void **	intern_windlist	O	list of subwindows 
*
* Description:
*	Returns all windows loaded in vldh in a plc.
*	When the caller is finished with the windowlist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_plc_windows (
  vldh_t_plc		plc,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
)
{
	int	sts;
	sts = vldh_get_wind_windows( plc->hp.windowobject, wind_count, 
		intern_windlist);
	return sts;
}


/*************************************************************************
*
* Name:		vldh_get_nodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * node_count	O	number of nodes in the nodelist
* void **	intern_nodelist	O	list of nodes in the window 
*
* Description:
*	Returns all nodes in a window.
*	When the caller is finished with the nodelist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_nodes (
  vldh_t_wind		wind,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist
)
{
	vldh_t_node	*nodelist_pointer;
	vldh_t_node	node_pointer;
	int		sts;
	int		count;

	/* Count the nodes */
	count = 0;
	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted */
	  if ( (node_pointer->hn.status & VLDH_DELETE) == 0 )
	    count++;

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	
	if ( count > 0)
	  /* Allocate memory */
	  *intern_nodelist = (void *)XtCalloc( count, sizeof( *intern_nodelist));

	*node_count = 0;
	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted */
	  if ( (node_pointer->hn.status & VLDH_DELETE) == 0 )
	  {
	    sts = utl_realloc( (char **)intern_nodelist, 
			count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	    if (EVEN(sts)) return sts;
	    nodelist_pointer = *intern_nodelist;
	    *(nodelist_pointer + *node_count) = node_pointer;
	    (*node_count)++;
	  }

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	if( *node_count == 0) return VLDH__NONODES;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_nodes_del()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * node_count	O	number of nodes in the nodelist
* void **	intern_nodelist	O	list of nodes in the window 
*
* Description:
*	Returns all nodes in a window that are deleted in vldh 
*	but not yet in ldh.
*	When the caller is finished with the nodelist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_nodes_del (
  vldh_t_wind		wind,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist
)
{
	vldh_t_node	*nodelist_pointer;
	vldh_t_node	node_pointer;
	int		sts;
	int		count;

	/* Count the nodes */
	count = 0;
	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted */
	  if ( 	((node_pointer->hn.status & VLDH_DELETE) != 0) &&
		((node_pointer->hn.status & VLDH_LDHDELETE) == 0) )
	    count++;

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	if ( count > 0)
	  /* Allocate memory */
	  *intern_nodelist = (void *)XtCalloc( count, sizeof( *intern_nodelist));

	*node_count = 0;

	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array deleted */
	  if ( 	((node_pointer->hn.status & VLDH_DELETE) != 0) &&
		((node_pointer->hn.status & VLDH_LDHDELETE) == 0) )
	  {
	    sts = utl_realloc( (char **)intern_nodelist, 
			count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	    if (EVEN(sts)) return sts;
	    nodelist_pointer = *intern_nodelist;
	    *(nodelist_pointer + *node_count) = node_pointer;
	    (*node_count)++;
	  }

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	if( *node_count == 0) return VLDH__NONODES;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_nodes_class()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long	object_type	I	object type
* unsigned long * node_count	O	number of nodes in the nodelist
* void **	intern_nodelist	O	list of nodes in the window 
*
* Description:
*	Returns all nodes of a specific object type in a window.
*	When the caller is finished with the nodelist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_nodes_class (
  vldh_t_wind		wind,
  unsigned long		object_type,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist
)
{
	vldh_t_node	*nodelist_pointer;
	vldh_t_node	node_pointer;
	int		sts;
	int		count;

	/* Count the nodes */
	count = 0;
	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted */
	  if ( ( node_pointer->ln.object_type == object_type ) &&
		( node_pointer->hn.status & VLDH_DELETE ) == 0)
	    count++;

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}

	if ( count > 0)
	  /* Allocate memory */
	  *intern_nodelist = (void *)XtCalloc( count, sizeof( *intern_nodelist));

	*node_count = 0;
	nodelist_pointer = *intern_nodelist;

	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted and if specified class */
	  if ( ( node_pointer->ln.object_type == object_type ) &&
		( node_pointer->hn.status & VLDH_DELETE ) == 0)
	  {
	    sts = utl_realloc( (char **)intern_nodelist, 
			count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	    if (EVEN(sts)) return sts;
	    nodelist_pointer = *intern_nodelist;
	    *(nodelist_pointer + *node_count) = node_pointer;
	    (*node_count)++;
	  }

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	if( *node_count == 0) return VLDH__NONODES;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_cons()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * con_count	O	number of cons in the conlist
* void **	intern_conlist	O	list of cons in the window 
*
* Description:
*	Returns all connections in a window.
*	When the caller is finished with the connectionslist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_cons (
  vldh_t_wind		wind,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
)
{
	vldh_t_con	*conlist_pointer;
	vldh_t_con	con_pointer;
	int		sts;
	int		count;

	/* Start to count the cons */
	count = 0;
	con_pointer = wind->hw.con_list_pointer;
	while ( con_pointer != 0 )
	{
	  /* Put con in array if not deleted */
	  if ( (con_pointer->hc.status & VLDH_DELETE) == 0 )
	    count++;

	  /* Next con */
	  con_pointer = con_pointer->hc.object_pointer;
	}

	if ( count > 0)
	  /* Allocate memory */
	  *intern_conlist = (void *)XtCalloc( count, sizeof( *intern_conlist));

	*con_count = 0;

	con_pointer = wind->hw.con_list_pointer;
	while ( con_pointer != 0 )
	{
	  /* Put con in array if not deleted */
	  if ( (con_pointer->hc.status & VLDH_DELETE) == 0 )
	  {
	    sts = utl_realloc( (char **)intern_conlist, 
			count * sizeof( *intern_conlist), 
			(*con_count + 1) * sizeof(*intern_conlist));
	    if (EVEN(sts)) return sts;
	    conlist_pointer = *intern_conlist;
	    *(conlist_pointer + *con_count) = con_pointer;
	    (*con_count)++;
	  }

	  /* Next con */
	  con_pointer = con_pointer->hc.object_pointer;
	}
	if( *con_count == 0) return VLDH__NOCONS;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_cons_del()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * con_count	O	number of cons in the conlist
* void **	intern_conlist	O	list of cons in the window 
*
* Description:
*	Returns all connections in a window that is deleted in vldh
*	but not yet in ldh.
*	When the caller is finished with the connectionslist
*	the space should be freed by a Xtfree call.
*
**************************************************************************/

int vldh_get_cons_del ( 
  vldh_t_wind		wind,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
)
{
	vldh_t_con	*conlist_pointer;
	vldh_t_con	con_pointer;
	int		sts;
	int		count;

	/* Start to count the cons */
	count = 0;
	con_pointer = wind->hw.con_list_pointer;
	while ( con_pointer != 0 )
	{
	  /* Put con in array if not deleted */
	  if ( ((con_pointer->hc.status & VLDH_DELETE) != 0 ) &&
		((con_pointer->hc.status & VLDH_LDHDELETE) == 0) )
	    count++;

	  /* Next con */
	  con_pointer = con_pointer->hc.object_pointer;
	}

	if ( count > 0)
	  /* Allocate memory */
	  *intern_conlist = (void *)XtCalloc( count, sizeof( *intern_conlist));


	*con_count = 0;

	con_pointer = wind->hw.con_list_pointer;
	while ( con_pointer != 0 )
	{
	  /* Put con in array if not deleted */
	  if ( ((con_pointer->hc.status & VLDH_DELETE) != 0 ) &&
		((con_pointer->hc.status & VLDH_LDHDELETE) == 0) )
	  {
	    sts = utl_realloc( (char **)intern_conlist, 
			count * sizeof( *intern_conlist), 
			(*con_count + 1) * sizeof(*intern_conlist));
	    if (EVEN(sts)) return sts;
	    conlist_pointer = *intern_conlist;
	    *(conlist_pointer + *con_count) = con_pointer;
	    (*con_count)++;
	  }

	  /* Next con */
	  con_pointer = con_pointer->hc.object_pointer;
	}
	if( *con_count == 0) return VLDH__NOCONS;
	else return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_get_conpoints_next()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	point		I	connection point in the node
* unsigned long	* point_count	O	number of nodes in pointlist
* vldh_t_conpoint ** pointlist	O	list of nodes and connectonspoints
*					connected to the point.
*
* Description:
*	Recursiv function uset by vldh_get_conpoint_nodes
*	to find all nodes connected to a connectionpoint.
*
**************************************************************************/

int vldh_get_conpoints_next (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint		**pointlist,
  unsigned long		attributemask
)
{
	int	i, j, found, sts;
	vldh_t_node	next_node;
	unsigned long	next_point;
	vldh_t_con	next_con;

	/* Get all connected connectionobjects */
	for ( i = 0; i < node->hn.con_count[point]; i++ )
	{
	  next_con = *((node->hn.con_list[point]) + i);
	  if ( next_con != 0 )
	  {
	    if ( (next_con->hc.status & VLDH_DELETE) == 0 )
	    {
	      if ( (next_con->lc.attributes & attributemask) == attributemask) 
	      {
	        /* Get the node and point in the other end */
	        next_node = next_con->hc.source_node_pointer;
	        next_point = next_con->lc.source_point;
	        if ( (next_node == node) && (next_point == point) )
	        {
	          /* We are looking for the destination node */
	          next_node = next_con->hc.dest_node_pointer;
	          next_point = next_con->lc.dest_point;
	        }
	        /* Check that node and point not already registered in pointlist */
	        found = 0;
	        for ( j = 0; j < *point_count; j++ )
	        {
	          if ( ( ((*pointlist) + j)->node == next_node ) &&
			 ( ((*pointlist) + j)->conpoint == next_point ) )
	          {
	            found = 1;
	            break;
	          }
	        }
	        if ( found == 0 )
	        {	
	          /* Put node in pointlist if not a pointnode */
	          sts = utl_realloc( (char **)pointlist, 
			*point_count * sizeof( vldh_t_conpoint), 
			(*point_count + 1) * sizeof(vldh_t_conpoint));
	          if (EVEN(sts)) return sts;

	          ((*pointlist) + *point_count)->node = next_node;
	          ((*pointlist) + *point_count)->conpoint = next_point;
	          (*point_count)++;

	          /* Get connected nodes to this point */
	          sts = vldh_get_conpoints_next( next_node, next_point,
	   		 point_count, pointlist, attributemask);
	          if ( EVEN(sts)) return sts;
	        }
	      }
	    }
	  }
	}   	

	return VLDH__SUCCESS;
}	


/*************************************************************************
*
* Name:		vldh_get_conpoint_nodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	point		I	connection point in the node
* unsigned long	* point_count	O	number of nodes in pointlist
* vldh_t_conpoint ** pointlist	O	list of nodes and connectonspoints
*					connected to the point.
*
* Description:
*	Returns all nodeobjects connected to a connectionpoint and the
*	connectionpoints on the found nodes. The input node and
*	connectionpoint is place first in the list.
*	The list should be freed by the user with a Xtfree call.
*
**************************************************************************/

int vldh_get_conpoint_nodes ( 
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint	**pointlist,
  unsigned long		attributemask
)
{
	int	sts;

	*point_count = 0;
	sts = utl_realloc( (char **)pointlist, 
			*point_count * sizeof( vldh_t_conpoint), 
			(*point_count + 1) * sizeof(vldh_t_conpoint));
	if (EVEN(sts)) return sts;
	(*pointlist)->node = node;
	(*pointlist)->conpoint = point;
	(*point_count)++;
	sts = vldh_get_conpoints_next( node, point, point_count, pointlist,
		attributemask);
	if ( EVEN(sts)) return sts;
	return VLDH__SUCCESS;
}	


/*************************************************************************
*
* Name:		vldh_get_close_conpoint_nodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	point		I	connection point in the node
* unsigned long	* point_count	O	number of nodes in pointlist
* vldh_t_conpoint ** pointlist	O	list of nodes and connectonspoints
*					connected to the point.
*
* Description:
*	Returns all the closests nodeobjects connected to a connectionpoint 
*	and the connectionpoints on the found nodes. The input node and
*	connectionpoint is place first in the list.
*	The list should be freed by the user with a Xtfree call.
*
**************************************************************************/

int vldh_get_conpoint_nodes_close (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint	**pointlist,
  unsigned long		attributemask
)
{
	int	i, j, found, sts;
	vldh_t_node	next_node;
	unsigned long	next_point;
	vldh_t_con	next_con;

	*point_count = 0;
	sts = utl_realloc((char **) pointlist, 
			*point_count * sizeof( vldh_t_conpoint), 
			(*point_count + 1) * sizeof(vldh_t_conpoint));
	if (EVEN(sts)) return sts;
	(*pointlist)->node = node;
	(*pointlist)->conpoint = point;
	(*point_count)++;

	/* Get all connected connectionobjects */
	for ( i = 0; i < node->hn.con_count[point]; i++ )
	{
	  next_con = *((node->hn.con_list[point]) + i);
	  if ( next_con != 0 )
	  {
	    if ( (next_con->hc.status & VLDH_DELETE) == 0 )
	    {
	      if ( (next_con->lc.attributes & attributemask) == attributemask) 
	      {
	        /* Get the node and point in the other end */
	        next_node = next_con->hc.source_node_pointer;
	        next_point = next_con->lc.source_point;
	        if ( (next_node == node) && (next_point == point) )
	        {
	          /* We are looking for the destination node */
	          next_node = next_con->hc.dest_node_pointer;
	          next_point = next_con->lc.dest_point;
	        }
	        /* Check that node and point not already registered in pointlist */
	        found = 0;
	        for ( j = 0; j < *point_count; j++ )
	        {
	          if ( ( ((*pointlist) + j)->node == next_node ) &&
			 ( ((*pointlist) + j)->conpoint == next_point ) )
	          {
	            found = 1;
	            break;
	          }
	        }
	      }
	    }
	    if ( found == 0 )
	    {	
	      /* Put node in pointlist if not a pointnode */
	      sts = utl_realloc((char **) pointlist, 
			*point_count * sizeof( vldh_t_conpoint), 
			(*point_count + 1) * sizeof(vldh_t_conpoint));
	      if (EVEN(sts)) return sts;

	      ((*pointlist) + *point_count)->node = next_node;
	      ((*pointlist) + *point_count)->conpoint = next_point;
	      (*point_count)++;
	    }
	  }
	}   	

	return VLDH__SUCCESS;
}	


/*************************************************************************
*
* Name:		vldh_get_conpoint_cons()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	point		I	connection point in the node
* unsigned long	* con_count	O	number of connections in conlist
* vldh_t_conpoint ** conlist	O	list of connections 
*					connected to the point.
*
* Description:
*	Returns a list of all connections connected to a connectionpoint. 
*
**************************************************************************/

int vldh_get_conpoint_cons (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
)
{
	vldh_t_con	*conlist_pointer;
	int		i;
	vldh_t_con	con;
	int		sts;


	*con_count = 0;

	/* Get all connected connectionobjects */
	for ( i = 0; i < node->hn.con_count[point]; i++ )
	{
	  con = *((node->hn.con_list[point]) + i);
	  if ( con != 0 )
	  {
	    if ( (con->hc.status & VLDH_DELETE) == 0 )
	    {
	      sts = utl_realloc((char **) intern_conlist, 
			*con_count * sizeof( *intern_conlist), 
			(*con_count + 1) * sizeof(*intern_conlist));
	      if (EVEN(sts)) return sts;
	      conlist_pointer = *intern_conlist;
	      *(conlist_pointer + *con_count) = con;
	      (*con_count)++;
	    }
	  }
	}   	

	if( *con_count == 0) return VLDH__NOCONS;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_nodes_direction()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window.
* unsigned long	* node_count_horizontal O number of nodes in horizontal nodelist
* void **	nodelist_horizontal O	list of nodes with horizontal 
*					compdirection.
* unsigned long	* node_count_vertical O number of nodes in vertical nodelist
* void **	nodelist_vertical O	list of nodes with vertical 
*					compdirection.
*
* Description:
*	Returns two lists containing the nodes with horizontal compdirection
*	and the nodes with vertical compdirection in the window.
*	Both lists should be freed by the user with a Xtfree call.
*
**************************************************************************/

int vldh_get_nodes_direction (
  vldh_t_wind		wind,
  unsigned long		*node_count_horizontal,
  vldh_t_node		**nodelist_horizontal,
  unsigned long		*node_count_vertical,
  vldh_t_node		**nodelist_vertical
)
{
	vldh_t_node		*node_pointer;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*listpointer_horizontal;
	vldh_t_node		*listpointer_vertical;
	vldh_t_node		*pointer;
	int			i, l, k, found;
	int			sts;

	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	*nodelist_horizontal = (void *)XtCalloc( node_count, sizeof(vldh_t_node));
	*nodelist_vertical = (void *)XtCalloc(node_count, sizeof(vldh_t_node));
	*node_count_horizontal = 0;
	*node_count_vertical = 0;
	node_pointer = nodelist;
	listpointer_horizontal = *nodelist_horizontal;
	listpointer_vertical = *nodelist_vertical;

	for ( i = 0; i < node_count; i++)
	{
	  if ( (*node_pointer)->ln.compdirection == VLDH_HORIZONTAL )
	  {
	    /* Put in list for horizontal nodes */ 
	    found = 0;
	    pointer = listpointer_horizontal;
	    for ( k=0; k < *node_count_horizontal; k++) 
	    {
	      if ( (*node_pointer)->ln.x + (*node_pointer)->ln.width <
		    (*pointer)->ln.x + (*pointer)->ln.width ) 
	      {
		found = 1;
                for ( l = *node_count_horizontal; l >k; l-- ) 
		{
		  *(listpointer_horizontal + l) = *(listpointer_horizontal +l-1);
		}
		*(listpointer_horizontal + k) = *node_pointer;
		break;
	      }
	      pointer++;
	    }
	    if (!found) 
	    {
	    *(listpointer_horizontal + *node_count_horizontal) = 
				*node_pointer;
	    }
	    (*node_count_horizontal)++;
	  }
	  else
	  {
	    /* Put in list for vertical nodes */ 
	    found = 0;
	    pointer = listpointer_vertical;
	    for ( k=0; k < *node_count_vertical; k++) 
	    {
	      if ( (*node_pointer)->ln.y + (*node_pointer)->ln.height >
		    (*pointer)->ln.y + (*pointer)->ln.height ) 
	      {
		found = 1;
                for ( l = *node_count_vertical; l >k; l-- ) 
		{
		  *(listpointer_vertical + l) = *(listpointer_vertical +l-1);
		}
		*(listpointer_vertical + k) = *node_pointer;
		break;
	      }
	      pointer++;
	    }
	    if (!found) 
	    {
	    *(listpointer_vertical + *node_count_vertical) = 
				*node_pointer;
	    }
	    (*node_count_vertical)++;
	  }
	  /* Next node */
	  node_pointer++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_cons_node()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	* con_count	O	number of cons in conlist
* vldh_t_con * 	intern_conlist	O	connections connected to a node.
*
* Description:
*	Return a list of all connections connected to a node.
*	The list should be freed by the caller with a Xtfree call.
*
**************************************************************************/

int vldh_get_cons_node (
  vldh_t_node		node,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
)
{
	int		con_point;
	vldh_t_con	*conlist_pointer;
	vldh_t_con	con, *con_p;
	int		i, j;
	int		sts;
        int             found;

	*con_count = 0;
	for ( con_point = 0; con_point < VLDH_MAX_CONPOINTS; con_point++ )
	{
	  /* Find the conobject in conlist and insert 0 */
	  for ( i = 0; i < node->hn.con_count[con_point]; i++)
	  {
	    con = *((node->hn.con_list[con_point]) + i);
	    if ( con != 0 )
	    {
	      if ( (con->hc.status & VLDH_DELETE) == 0 )
	      {
                /* Check that it's not already inserted */
                con_p = *intern_conlist;
                found = 0;
                for ( j = 0; j < *con_count; j++)
                {
                  if ( *con_p == con)
                  {
                    found = 1;
                    break;
                  }
                  con_p++;
                }
                if ( found)
                  continue;

	        sts = utl_realloc((char **) intern_conlist, 
			*con_count * sizeof( *intern_conlist), 
			(*con_count + 1) * sizeof(*intern_conlist));
	        if (EVEN(sts)) return sts;
	        conlist_pointer = *intern_conlist;
	        *(conlist_pointer + *con_count) = con;
	        (*con_count)++;
	      }
	    }
	  }
	}
	if( *con_count == 0) return VLDH__NOCONS;
	else return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_nodes_node()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node
* unsigned long	* con_count	O	number of cons in conlist
* vldh_t_con * 	intern_conlist	O	connections connected to a node.
*
* Description:
*	Return a list of all nodes connected to a node with a connection
*	with the specified attributes.
*	The list should be freed by the caller with a Xtfree call.
*
**************************************************************************/

int vldh_get_nodes_node (
  vldh_t_node		node,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist,
  unsigned long		conmask,
  unsigned long		nodemask
)
{
	vldh_t_node	*nodelist_pointer;
	int		i;
	vldh_t_con 	*con_list;
	vldh_t_con 	*con_ptr;
	unsigned long	con_count;
	int		sts;

	*node_count = 0;

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;
	con_ptr = con_list;

	for ( i = 0; i < con_count; i++)
	{
	  if ( ((*con_ptr)->lc.attributes & conmask) == conmask) 
	  {
	    /* Connections with the correct attributes */
	    if ( nodemask & VLDH_NODE_DESTINATION)	
	    {
	      /* We want the nodes that are destination */
	      if ( (*con_ptr)->hc.source_node_pointer == node)
	      {
	        /* Insert the destination node in the nodelist */
	        sts = utl_realloc((char **) intern_nodelist, 
			*node_count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	        if (EVEN(sts)) return sts;
	        nodelist_pointer = *intern_nodelist;
	        *(nodelist_pointer + *node_count) =
			(*con_ptr)->hc.dest_node_pointer;
	        (*node_count)++;
	      }
	    }
	    if ( nodemask & VLDH_NODE_SOURCE)	
	    {
	      /* We want the nodes that are source */
	      if ( (*con_ptr)->hc.dest_node_pointer == node)
	      {
	        /* Insert the source node in the nodelist */
	        sts = utl_realloc((char **) intern_nodelist, 
			*node_count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	        if (EVEN(sts)) return sts;
	        nodelist_pointer = *intern_nodelist;
	        *(nodelist_pointer + *node_count) =
			(*con_ptr)->hc.source_node_pointer;
	        (*node_count)++;
	      }
	    }
	  }
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) con_list);

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_plc_objdid()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	objdid		I	objdid of a Plcpgm.
* vldh_t_plc * 	plc		O	vldh plc of the Plcpgm.
*
* Description:
*	Get a plc with a specified objdid.
*
**************************************************************************/

int vldh_get_plc_objdid (
  pwr_tObjid objdid,
  vldh_t_plc	*plc
)
{
	  vldh_t_plc	obj_pointer;

	  obj_pointer = plc_root;
	  while ( obj_pointer != 0 )
	  {
	    if ( cdh_ObjidIsEqual( objdid, obj_pointer->lp.objdid) && 
		!(obj_pointer->hp.status & VLDH_DELETE))
	    {
	      /* This is it */
	      *plc = obj_pointer;
	      return VLDH__SUCCESS;
	    }
	    /* Next object */
	    obj_pointer = obj_pointer->hp.plcobj_pointer;
	  }
	  /* If this is reached, objdid not found */
	  return VLDH__OBJNOTFOUND;
}

/*************************************************************************
*
* Name:		vldh_get_plcs()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	* plc_count	O	number of plc's in plclist.
* void **	plclist		O	list of all plc's in vldh.
*
* Description:
*	Returns a list of all plc's in vldh.
*	The list should be freed with a Xtfree call.
*
**************************************************************************/

int vldh_get_plcs (
  unsigned long		*plc_count,
  vldh_t_plc		**plclist
)
{
	  vldh_t_plc	obj_pointer;
	  vldh_t_plc	*plclist_pointer;
	  int		sts;

	  *plc_count = 0;

	  obj_pointer = plc_root;
	  while ( obj_pointer != 0 )
	  {
	    sts = utl_realloc((char **) plclist, 
			*plc_count * sizeof( vldh_t_plc), 
			(*plc_count + 1) * sizeof( vldh_t_plc));
	    if (EVEN(sts)) return sts;
	    plclist_pointer = *plclist;
	    *(plclist_pointer + *plc_count) = obj_pointer;
	    (*plc_count)++;

	    /* Next object */
	    obj_pointer = obj_pointer->hp.plcobj_pointer;
	  }

	  return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_get_node_objdid()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	objdid		I	objdid of the node
* vldh_t_wind	wind		I	vldh window of the node
* vldh_t_node * node		O	found vldh node.
*
* Description:
*	Get a node with a specified objdid.
*
**************************************************************************/

int vldh_get_node_objdid (
  pwr_tObjid		objdid,
  vldh_t_wind		wind,
  vldh_t_node		*node
)
{
	vldh_t_node	*nodelist;
	unsigned long	node_count;
	vldh_t_node	*nodelist_ptr;
	int		sts, i, node_found;

	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	node_found = 0;
	nodelist_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  if ( cdh_ObjidIsEqual( (*nodelist_ptr)->ln.object_did, objdid ))
	  {
	    *node = *nodelist_ptr;
	    node_found = 1;
	    break;
	  }
	  nodelist_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	if ( !node_found )
	  return VLDH__OBJNOTFOUND;
 
	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_wind_objdid()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	objdid		I	objdid of the window.
* vldh_t_wind * wind		O	vldh window.
*
* Description:
*	Get a window with a specific objdid.
*
**************************************************************************/

int vldh_get_wind_objdid (
  pwr_tObjid	objdid,
  vldh_t_wind	*wind
)
{
	vldh_t_plc	*plclist;
	unsigned long	plc_count;
	vldh_t_plc	*plclist_ptr;
	vldh_t_wind	*windlist;
	unsigned long	wind_count;
	vldh_t_wind	*windlist_ptr;
	int		sts, i, j, window_found;
	  
	/* Get all the plc */
	sts = vldh_get_plcs( &plc_count, &plclist);
	if ( EVEN(sts)) return sts;

	window_found = 0;
	plclist_ptr = plclist;
	for ( i = 0; i < plc_count; i++)
	{
	  /* Get all windows in this plc */
	  sts = vldh_get_plc_windows( *plclist_ptr, &wind_count, &windlist);
	  if ( EVEN(sts)) return sts;

	  windlist_ptr = windlist;
	  for ( j = 0; j < wind_count; j++)
	  {
	    if ( cdh_ObjidIsEqual( (*windlist_ptr)->lw.objdid, objdid))
	    {
	      window_found = 1;
	      *wind = *windlist_ptr;
	      break;
	    }
	    windlist_ptr++;
	  }
	  if ( wind_count > 0) XtFree((char *) windlist);
	  if ( window_found)
	    break;
	  plclist_ptr++;
	}
	if ( plc_count > 0) XtFree((char *) plclist);

	if ( !window_found )
	  return VLDH__OBJNOTFOUND;
 
	return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_paste...
*
* Description:
*	Handles the paste buffer. The node_id and con_id is not
*	used for displaying neted nodes but used temporary to 	
*	solve connections between nodes.
*
**************************************************************************/

/*_Local variables_______________________________________________________*/

vldh_t_nodeobject	paste_singlenode;
int			paste_singelnode_init = 0;
vldh_t_windobject	paste_window;
float			paste_x;
float			paste_y;
float			paste_width;
float			paste_height;
void			*paste_ctx;
static	int		paste_mode = 0;



/*************************************************************************
*
* Name:		vldh_paste_setmode()
* Name:		vldh_paste_getmode()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set or set the last paste operation.
*
**************************************************************************/

int vldh_paste_setmode ( 
  int mode
)
{
	paste_mode = mode;
	return VLDH__SUCCESS;
}
int vldh_paste_getmode (
)
{
	return paste_mode;
}

/*************************************************************************
*
* Name:		vldh_paste_selrectinfo()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void *	ctx		I	foectx of the cut operation
* float		x		I	x-koordinate of selection rectangel.
* float		y		I	y-koordinate of selection rectangel.
* float		width		I	width of selection rectangel.
* float		height		I	height of selection rectangel.
*
* Description:
*	Inserts the koordinates for the selection rectangle at a
*	cut operation.
*
**************************************************************************/

int vldh_paste_setrectinfo (
  void	*ctx,
  float	x,
  float	y,
  float	width,
  float	height
)
{

	paste_x = x;
	paste_y = y;
	paste_width = width;
	paste_height = height;
	paste_ctx = ctx;

	return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_paste_getrectinfo()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void **	ctx		I	foectx of the cut operation
* float	*	x		I	x-koordinate of selection rectangel.
* float	*	y		I	y-koordinate of selection rectangel.
* float	*	width		I	width of selection rectangel.
* float	*	height		I	height of selection rectangel.
*
*
* Description:
*	Returns the selectrectangle koordinates stored at the 
*	last cut-operation.
*
**************************************************************************/

int vldh_paste_getrectinfo (
  void	**ctx,
  float	*x,
  float	*y,
  float	*width,
  float	*height
)
{

	*x = paste_x;
	*y = paste_y;
	*width = paste_width;
	*height = paste_height;
	*ctx = paste_ctx;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_paste_singlenode_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	send_window	I	vldh window of the inserted node.
* vldh_t_node	send_node	I	node to insert in the paste buffer.
*
* Description:
*	Inserts a node in the singlenode paste buffer.
*
**************************************************************************/

int vldh_paste_singlenode_insert (
  vldh_t_wind	send_window,
  vldh_t_node	send_node
)
{
	int	i;

	/* Copy sendnode to created pastebuffer node */
	memcpy( &paste_singlenode, send_node, sizeof(paste_singlenode));
	paste_singlenode.hn.window_pointer = &paste_window;
	paste_singlenode.hn.node_id = send_node;
	paste_singlenode.ln.window_did = pwr_cNObjid;
	paste_singlenode.hn.status = VLDH_CREATE;

	for ( i = 0; i < VLDH_MAX_CONPOINTS; i++ ) 
			paste_singlenode.hn.con_count[i] = 0;

	paste_singelnode_init = 1;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_paste_node_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	send_window	I	vldh window of the inserted node.
* vldh_t_node	send_node	I	node to insert in the paste buffer.
*
* Description:
*	Inserts a nodeobject in the paste nodelist.
*
**************************************************************************/

int vldh_paste_node_insert (
  vldh_t_wind	send_window,
  vldh_t_node	send_node
)
{
	vldh_t_node	node;
	int	i;

	/* Create a pastebuffer node */
	node = (void *)XtCalloc(1, sizeof(*node));

	/* Copy sendnode to created pastebuffer node */
	memcpy( node, send_node, sizeof(*node));
	node->hn.window_pointer = &paste_window;
	node->hn.node_id = send_node;
	node->ln.window_did = pwr_cNObjid;
	node->hn.status = VLDH_CREATE;

	for ( i = 0; i < VLDH_MAX_CONPOINTS; i++ ) 
			node->hn.con_count[i] = 0;

	/* Insert in pastewindows nodelist */
	node->hn.object_pointer = paste_window.hw.node_list_pointer;
	paste_window.hw.node_list_pointer = node;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_paste_con_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	send_window	I	vldh window of the inserted connection.
* vldh_t_con	send_con	I	con to insert in the paste buffer.
*
* Description:
*	Inserts a connectionobject in the paste connectionlist.
*
**************************************************************************/

int vldh_paste_con_insert (
  vldh_t_wind	send_window,
  vldh_t_con	send_con
)
{
vldh_t_con	con;
vldh_t_node	node_pointer;
vldh_t_node	source_node_pointer;
vldh_t_node	dest_node_pointer;
int		dest_found;
int		source_found;

	/* Find the new source and destination node pointers */
	/* They must be inserted in the pastebuffer, if not return */
	source_found = 0;
	dest_found = 0;
	node_pointer = paste_window.hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  if ( node_pointer->hn.node_id == send_con->hc.source_node_pointer )
	  {
	    source_node_pointer = node_pointer;
	    source_found = 1;
	  }
	  if ( node_pointer->hn.node_id == send_con->hc.dest_node_pointer )
	  {
	    dest_node_pointer = node_pointer;
	    dest_found = 1;
	  }
	  node_pointer = node_pointer->hn.object_pointer;
	}

	if ( source_found && dest_found )
	{
	  /* Create a pastebuffer node */
	  con = (void *)XtCalloc(1, sizeof(*con));
	  memcpy( con, send_con, sizeof(*con));

	  con->hc.con_id = 0;
	  con->hc.source_node_pointer = source_node_pointer;
	  con->lc.source_node_did = pwr_cNObjid;
	  con->hc.dest_node_pointer = dest_node_pointer;
	  con->lc.dest_node_did = pwr_cNObjid;
	  con->hc.status = VLDH_CREATE;
	  con->hc.window_pointer = &paste_window;
	  con->lc.window_did = pwr_cNObjid;

	  /* Insert in pastewindows connection list */
	  con->hc.object_pointer = paste_window.hw.con_list_pointer;
	  paste_window.hw.con_list_pointer = con;
	}
	return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_paste_init()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Intitiates paste. 
*
**************************************************************************/

int vldh_paste_init( )
{
	/* free all memory allocated for the nodes */
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	unsigned long		con_count;
	vldh_t_con		*conlist;
	vldh_t_con		*con_ptr;
	int			i,j;
	int			sts;

	/* Free all the  nodes */
	sts = vldh_get_nodes( &paste_window, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  for ( j = 0; j < VLDH_MAX_CONPOINTS; j++ ) 
	  {
	    if ( (*node_ptr)->hn.con_count[j] > 0 )
	    {
	      XtFree((char *)(*node_ptr)->hn.con_list[j]);
	    }
	  }
	  XtFree((char *) *node_ptr);
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);

	/* Free all the connections */
	sts = vldh_get_cons( &paste_window, &con_count, &conlist);
	if ( EVEN(sts)) return sts;
	con_ptr = conlist;
	for ( i = 0; i < con_count; i++)
	{
	  XtFree((char *) *con_ptr);
	  con_ptr++;
	}
	if ( con_count > 0) XtFree((char *) conlist);

	paste_window.hw.node_list_pointer = 0;
	paste_window.hw.con_list_pointer = 0;

	return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_paste_signlenode_copy()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	window		I	vldh window where the node should
*					be copied.
* vldh_t_node	node		O	the created copy of the node.
*
* Description:
*	Copies the paste singlenode into the specified
*	window and calculates the new koordinates for the node.
*
**************************************************************************/

int vldh_paste_singlenode_copy (
  vldh_t_wind	wind,
  vldh_t_node	*node
)
{
	char			segment_name[32];
	pwr_tObjid 		objdid;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	if ( paste_singelnode_init == 0 ) 
	  return VLDH__OBJNOTFOUND;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(wind->hw.ldhsession, paste_singlenode.ln.classid,
		"GraphPlcNode", &bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return sts;

	for (;;)
	{
	  /* Get default segment name */
	  sts = vldh_get_node_defname( wind, graphbody->object_type, 
		graphbody->objname, segment_name);
	  if ( EVEN(sts) ) return sts;
	  
	  /* Create the object i ldh */
	  sts = ldh_CopyObject( wind->hw.ldhsession, &objdid, segment_name, 
		paste_singlenode.ln.object_did, wind->lw.objdid, 
		ldh_eDest_IntoLast);
	  if ( sts == LDH__NAMALREXI)
	    /* Try again with incremented name index */
	    continue;
	  else if ( EVEN(sts))
	  {
	    /* Create a new node without source */
	    sts = ldh_CreateObject( wind->hw.ldhsession, &objdid, segment_name, 
		paste_singlenode.ln.classid, wind->lw.objdid, 
		ldh_eDest_IntoLast);
	    if ( EVEN(sts)) return sts;
	  }
	  break;
	}
	/* Create a new node in the referenced window */
	*node = (void *)XtCalloc(1, sizeof(**node));

	/* Copy pastebuffer node to the created node */
	memcpy( *node, &paste_singlenode, sizeof(**node));
	(*node)->hn.window_pointer = wind;
	/* Temporary set node_id to paste_node to solve connections */
	(*node)->hn.status = VLDH_CREATE;
	(*node)->ln.object_did = objdid;
	(*node)->ln.window_did = wind->lw.objdid;
	(*node)->hn.window_pointer = wind;
	strcpy(((*node)->hn.name), segment_name);
	(*node)->ln.subwindow = 0;
	(*node)->ln.subwind_objdid[0] = pwr_cNObjid;
	(*node)->ln.subwind_objdid[1] = pwr_cNObjid;

	/* Insert in windowobjects nodelist */
	(*node)->hn.object_pointer = wind->hw.node_list_pointer;
	wind->hw.node_list_pointer = *node;
	
	/* Do some class specific stuff */
	vldh_node_create_spec( *node);

	/* Set the buffer in ldh so the mask can be accessed by ate */
	sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		(*node)->ln.object_did,
		"DevBody",
		"PlcNode",
		(char *)&((*node)->ln));
	if( EVEN(sts)) return sts;

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_paste_copy()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	window to paste in.
* float		x		I	x-koordinate of paste operation.
* float		y		I	y-koordinate of paste operation.
* unsigned long	*node_count	O	number of nodes in the nodelist.
* char **	intern_nodelist O	list of  created nodes.
* unsigned long	* con_count	O	number of cons in the conlist.
* char **	intern_conlist	O	list of create connections.
*
* Description:
*	Copies the paste nodelist and connectionlist into the specified
*	windowobject and calculates the new koordinates for the node.
*
**************************************************************************/

int vldh_paste_copy (
  vldh_t_wind	wind,
  float		x,
  float		y,
  unsigned long	*node_count,
  vldh_t_node	**intern_nodelist,
  unsigned long	*con_count,
  vldh_t_con	**intern_conlist
)
{
	vldh_t_con	con;
	vldh_t_node	node;
	vldh_t_con	paste_con;
	vldh_t_node	paste_node;
	vldh_t_node	*nodelist_pointer;
	vldh_t_con	*conlist_pointer;
	int	i;
	char			segment_name[32];
	pwr_tObjid 		objdid;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	*node_count = 0;
	*con_count = 0;

	paste_node = paste_window.hw.node_list_pointer;
	while ( paste_node != 0 )
	{

	  /* Get graphbody for the class and fetch the defaultname */
	  sts = ldh_GetClassBody(wind->hw.ldhsession, paste_node->ln.classid,
		"GraphPlcNode", &bodyclass, (char **)&graphbody, &size);
	  if ( EVEN(sts)) return sts;

	  for (;;)
	  {
	    /* Get default segment name */
	    sts = vldh_get_node_defname( wind, graphbody->object_type, 
		graphbody->objname, segment_name);
	    if ( EVEN(sts) ) return sts;
	  
	    /* Create the object i ldh */
	    sts = ldh_CopyObject( wind->hw.ldhsession, &objdid, segment_name, 
		paste_node->ln.object_did, wind->lw.objdid, 
		ldh_eDest_IntoLast);
	    if ( sts == LDH__NAMALREXI)
	      /* Try again with incremented name index */
	    continue;
	    else if ( EVEN(sts))
	    {
	      /* Create a new object without source */ 
	      sts = ldh_CreateObject( wind->hw.ldhsession, &objdid, segment_name, 
		paste_node->ln.classid, wind->lw.objdid, 
		ldh_eDest_IntoLast);
	      if ( EVEN(sts))
	      return sts;
	    }
	    break;
	  }

	  /* Create a new node in the referenced window */
	  node = (void *)XtCalloc(1, sizeof(*node));

	  /* Copy pastebuffer node to the created node */
	  memcpy( node, paste_node, sizeof(*node));
	  node->hn.window_pointer = wind;
	  /* Temporary set node_id to paste_node to solve connections */
	  node->hn.node_id = paste_node;
	  node->ln.x = paste_node->ln.x - paste_x + x;
	  node->ln.y = paste_node->ln.y - paste_y + y;
	  node->hn.status = VLDH_CREATE;
	  node->ln.object_did = objdid;
	  node->ln.window_did = wind->lw.objdid;
	  node->ln.subwindow = 0;
	  node->ln.subwind_objdid[0] = pwr_cNObjid;
	  node->ln.subwind_objdid[1] = pwr_cNObjid;
	  node->hn.window_pointer = wind;
	  node->hn.subwindowobject[0] = 0;
	  node->hn.subwindowobject[1] = 0;
	  strcpy( (node->hn.name), segment_name);

	  /* Insert in windowobjects nodelist */
	  node->hn.object_pointer = wind->hw.node_list_pointer;
	  wind->hw.node_list_pointer = node;

	  /* Do some class specific stuff */
	  vldh_node_create_spec( node);

	  /* Set the buffer in ldh so the mask can be accessed by ate */
	  sts = ldh_SetObjectBuffer(
		wind->hw.ldhsession,
		node->ln.object_did,
		"DevBody",
		"PlcNode",
		(char *)&(node->ln));
	  if( EVEN(sts)) return sts;

	  /* Insert in intern nodelist */
	  sts = utl_realloc((char **) intern_nodelist, 
			*node_count * sizeof( *intern_nodelist), 
			(*node_count + 1) * sizeof(*intern_nodelist));
	  if (EVEN(sts)) return sts;
	  nodelist_pointer = *intern_nodelist;
	  *(nodelist_pointer + *node_count) = node;
	  (*node_count)++;

	  /* Next node */
	  paste_node = paste_node->hn.object_pointer;
	}
	if ( *node_count == 0) return VLDH__NONODES;


	paste_con = paste_window.hw.con_list_pointer;
	while ( paste_con != 0 )
	{

	  for (;;)
	  {
	    /* Get default segment name */
	    vldh_get_con_defname( wind, segment_name);
	  
	    /* Create the object i ldh */
	    sts = ldh_CreateObject( wind->hw.ldhsession, &objdid, segment_name, 
		paste_con->lc.classid, wind->lw.objdid, ldh_eDest_IntoFirst);
	    if ( sts == LDH__NAMALREXI)
	      /* Try again with incremented name index */
	      continue;
	    else if( EVEN(sts)) return sts;
	    break;
	  }

	  /* Create a new connection in the referenced window */
	  con = (void *)XtCalloc(1, sizeof(*con));

	  /* Copy pastebuffer node to the created connection */
	  memcpy( con, paste_con, sizeof(*con));
	  con->hc.window_pointer = wind;
	  con->hc.con_id = 0;
	  con->hc.status = VLDH_CREATE;
	  con->lc.objdid = objdid;
	  con->lc.refnr = 0;
	  con->hc.window_pointer = wind;
	  con->lc.window_did = wind->lw.objdid;
	  /* Calculate new reroutingpoints here */

	  /* Find the new source and destination node pointers */
	  /* They must be inserted in the internal nodelist */
	  for ( i = 0; i < *node_count; i++)
	  {
	    node = *(nodelist_pointer + i);
	    if ( node->hn.node_id == paste_con->hc.source_node_pointer )
	    {
	      con->hc.source_node_pointer = node;
	      con->lc.source_node_did = node->ln.object_did;
	    }
	    if ( node->hn.node_id == paste_con->hc.dest_node_pointer )
	    {
	      con->hc.dest_node_pointer = node;
	      con->lc.dest_node_did = node->ln.object_did;
	    }
	  }
	  /* Insert in the node connection pointer */
	  vldh_node_con_insert(con->hc.source_node_pointer,
		con->lc.source_point, con, VLDH_NODE_SOURCE);
	  vldh_node_con_insert(con->hc.dest_node_pointer,
		con->lc.dest_point, con, VLDH_NODE_DESTINATION);

	  /* Insert in windowobjects conlist */
	  con->hc.object_pointer = wind->hw.con_list_pointer;
	  wind->hw.con_list_pointer = con;

	  /* Insert in intern conlist */
	  sts = utl_realloc((char **) intern_conlist, 
			*con_count * sizeof( *intern_conlist), 
			(*con_count + 1) * sizeof(*intern_conlist));
	  if (EVEN(sts)) return sts;
	  conlist_pointer = *intern_conlist;
	  *(conlist_pointer + *con_count) = con;
	  (*con_count)++;

	  /* Next connection */
	  paste_con = paste_con->hc.object_pointer;
 	}

	return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_node_update_spec()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node that is updated.
*
* Description:
*	Apply class specific elements when the node is updated.
*
**************************************************************************/

int vldh_node_update_spec (
  vldh_t_node	node
)
{

	int		sts, size;
	vldh_t_wind	wind;
	ldh_tSesContext ldhses;
	vldh_t_plc	plc;
	float		*time_ptr;
	unsigned long	*objdid_ptr;

	wind = node->hn.window_pointer;
	plc = wind->hw.plcobject_pointer;
	ldhses = wind->hw.ldhsession;
	
	/* Updates that are not vldh specific */
	sts = vldh_object_update_spec( ldhses, node->ln.object_did);
	if ( EVEN(sts)) return sts;

	/**********************************************************
	*	ORDERACT						
	*	Put window objdid in the ldhobject.
	**********************************************************/
	else if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_ORDERACT))
	{
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"OrderObject",
		(char *)&wind->lw.parent_node_did, 
		sizeof(wind->lw.parent_node_did)); 
	  if ( EVEN(sts)) return sts;
	}
	/**********************************************************
	*	CurrentData and CurrentIndex
	*	Put cell objdid in the ldhobject.
	**********************************************************/
	else if ( (node->ln.classid == vldh_eclass( ldhses, "CurrentData")) ||
		(node->ln.classid == vldh_eclass( ldhses, "CurrentIndex")))
	{
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"CellObject",
		(char *)&wind->lw.parent_node_did, 
		sizeof(wind->lw.parent_node_did)); 
	  if ( EVEN(sts)) return sts;
	}
	/**********************************************************
	*	SHOWPLCATTR					
	*	Put plc attributes in the ldhobject.
	**********************************************************/
	else if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_SHOWPLCATTR))
	{
	  pwr_tObjid volume_objid;
	  pwr_tObjid *thread_objid;

	  volume_objid.oix = 0;
	  volume_objid.vid = node->ln.object_did.vid;

	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"Node",
		(char *)&volume_objid, sizeof( volume_objid)); 
	  if ( EVEN(sts)) return sts;

	  sts = ldh_GetObjectPar( ldhses,
			plc->lp.objdid, 
			"RtBody",
			"ThreadObject",
			(char **)&thread_objid, &size); 
	  if ( EVEN(sts)) return sts;
	  if ( cdh_ObjidIsNotNull( *thread_objid)) {
	    sts = ldh_GetObjectPar( ldhses,
				    *thread_objid, 
				    "RtBody",
				    "ScanTime",
				    (char **)&time_ptr, &size); 
	  }
	  free((char *) thread_objid);
	  if ( ODD(sts)) {
	    sts = ldh_SetObjectPar( ldhses,
				    node->ln.object_did, 
				    "DevBody",
				    "ScanTime",
				    (char *)time_ptr, size); 
	    if ( EVEN(sts)) return sts;
	    free((char *) time_ptr);
	  }

	  sts = ldh_GetObjectPar( ldhses,
			plc->lp.objdid, 
			"DevBody",
			"ResetObject",
			(char **)&objdid_ptr, &size); 
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"ResetObject",
		(char *)objdid_ptr, size); 
	  if ( EVEN(sts)) return sts;
	  free((char *) objdid_ptr);
	}

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_update_spec()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	vldh node that is updated.
*
* Description:
*	Apply class specific elements when the node is updated.
*
**************************************************************************/

int vldh_object_update_spec (
  ldh_tSesContext ldhses,
  pwr_tObjid Objdid
)
{
	char		segment_name[32];
	int		i, sts, size;
	ldh_sParDef 	*bodydef;
	int 		rows;
	char		*parvalue;
	pwr_tClassId	class;
	char		newsuborderchar[6];
	pwr_tClassId	newsuborderclass[6];
	pwr_tClassId	oldsuborderclass[6];
	pwr_tObjid	oldsuborderobjdid[6];
	pwr_tObjid	newsuborderobjdid[6];
	unsigned long	newsubordercount;
	unsigned long	oldsubordercount;
	int		newsuborderindex[6];
	int		count_ok, classdiff_ok, diffcount;
	pwr_tObjid	next_objdid;
	char		graphname[2];
	float		*time_ptr;
	pwr_tClassId	suborderclass;
	pwr_tObjid	suborderobjdid;

	/**********************************************************
	*	ORDER						
	*	Create objects for the orderattributes.
	**********************************************************/

	sts = ldh_GetObjectClass( ldhses, Objdid, &class);
	if ( EVEN(sts)) return sts;

	if ( class == vldh_class( ldhses, VLDH_CLASS_ORDER))
	{
	  /* Check the present suborder objects and compare them with
	    the attributes in the order object */

	  oldsubordercount = 0;
	  sts = ldh_GetChild( ldhses, Objdid, &next_objdid);
	  while ( ODD(sts) )
	  {
	    /* Find out if this is a suborder and wich class */
	    sts = ldh_GetObjectClass( ldhses, next_objdid, &suborderclass);

	    if ( (suborderclass == vldh_class( ldhses, VLDH_CLASS_SORDER)) || 
		(suborderclass == vldh_class( ldhses, VLDH_CLASS_LORDER)) || 
		(suborderclass == vldh_class( ldhses, VLDH_CLASS_DORDER)) || 
		(suborderclass == vldh_class( ldhses, VLDH_CLASS_PORDER)) || 
		(suborderclass == vldh_class( ldhses, VLDH_CLASS_CORDER)))
	    {
	      oldsuborderclass[oldsubordercount] = suborderclass;
	      oldsuborderobjdid[oldsubordercount] = next_objdid;
	      oldsubordercount++;
	    }

	    sts = ldh_GetNextSibling( ldhses, next_objdid, &next_objdid);
	  }

	  /* Check the attributes in the parent order node */
	  sts = ldh_GetObjectBodyDef( ldhses, 
			class, "DevBody", 1, 
			&bodydef, &rows);

	  if ( EVEN(sts) ) return sts;

	  newsubordercount = 0;
	  for ( i = 0; i < rows - 2; i += 2)
	  {
	    /* Get the parameter value */
	    sts = ldh_GetObjectPar( ldhses, 
			Objdid, 
			"DevBody",
			bodydef[i].ParName,
			&parvalue, &size); 
	    if ( EVEN(sts)) return sts;

	    if ( *parvalue == 'S' || *parvalue == 's')
	    {
	      newsuborderclass[newsubordercount] =
			vldh_class( ldhses, VLDH_CLASS_SORDER);
	      newsuborderchar[newsubordercount] = *parvalue;
	      newsuborderindex[newsubordercount] = i;
	      newsubordercount++;
	    }
	    else if ( *parvalue == 'L' || *parvalue == 'l')
	    {
	      newsuborderclass[newsubordercount] =
			vldh_class( ldhses, VLDH_CLASS_LORDER);
	      newsuborderchar[newsubordercount] = *parvalue;
	      newsuborderindex[newsubordercount] = i;
	      newsubordercount++;
	    }
	    else if ( *parvalue == 'C' || *parvalue == 'c')
	    {
	      newsuborderclass[newsubordercount] =
			vldh_class( ldhses, VLDH_CLASS_CORDER);
	      newsuborderchar[newsubordercount] = *parvalue;
	      newsuborderindex[newsubordercount] = i;
	      newsubordercount++;
	    }
	    else if ( *parvalue == 'D' || *parvalue == 'd')
	    {
	      newsuborderclass[newsubordercount] =
			vldh_class( ldhses, VLDH_CLASS_DORDER);
	      newsuborderchar[newsubordercount] = *parvalue;
	      newsuborderindex[newsubordercount] = i;
	      newsubordercount++;
	    }
	    else if ( *parvalue == 'P' || *parvalue == 'p')
	    {
	      newsuborderclass[newsubordercount] =
			vldh_class( ldhses, VLDH_CLASS_PORDER);
	      newsuborderchar[newsubordercount] = *parvalue;
	      newsuborderindex[newsubordercount] = i;
	      newsubordercount++;
	    }
	    free((char *) parvalue);
	  }
	
	  /* Compare old  and new suborders */
	  if ( newsubordercount == oldsubordercount ) 
	    count_ok = 1;
	  else
	    count_ok = 0;

	  classdiff_ok = 1;
	  diffcount = 0;
	  for ( i = 0; i < oldsubordercount; i++ )
	  {
	    if ( newsuborderclass[i] != oldsuborderclass[i] )
	    {
	      classdiff_ok = 0;
	      diffcount = i;
	      break;
	    }
	    else
	      newsuborderobjdid[i] = oldsuborderobjdid[i];
	  }

	  if ( !(classdiff_ok && count_ok ))
	  {
	    /* Delete suborderobjects after the first difference */
	    for ( i = diffcount; i < oldsubordercount; i++)
	    {
	      sts = ldh_DeleteObject( ldhses, oldsuborderobjdid[i]);
	      if( EVEN(sts)) return sts;
	    }
	    /* Create new suborderobjects starting with the first found
	      difference */
	    for ( i = diffcount; i < newsubordercount; i++ )
	    {
	      /* Get default segment name */
	      graphname[0] = newsuborderchar[i];
	      graphname[1] = '\0';
	      sts = vldh_get_object_defname( ldhses, Objdid, OT_SUBORDER,  
		graphname, segment_name);
	      if ( EVEN(sts) ) return sts;
	  
	      /* Create the object i ldh */
	      sts = ldh_CreateObject( ldhses, &suborderobjdid, segment_name, 
		 newsuborderclass[i], Objdid, ldh_eDest_IntoLast);
	      if ( EVEN(sts)) return sts;
	      newsuborderobjdid[i] = suborderobjdid;
	    }	
	  }

	  /* Move the time from parent to the children */
	  for ( i = 0; i < newsubordercount; i++ )
	  {
	    if ((newsuborderclass[i] == vldh_class( ldhses, VLDH_CLASS_DORDER))
		||
	        (newsuborderclass[i] == vldh_class( ldhses, VLDH_CLASS_LORDER)))
	    {
	      /* Move the time from the parent to the child */
	      sts = ldh_GetObjectPar( ldhses,
			Objdid, 
			"DevBody",
			bodydef[ newsuborderindex[i] + 1].ParName,
			(char **)&time_ptr, &size); 
	      if ( EVEN(sts)) return sts;
	      sts = ldh_SetObjectPar( ldhses,
			newsuborderobjdid[i], 
			"RtBody",
			"TimerTime",
			(char *)time_ptr, size); 
	      if ( EVEN(sts)) return sts;
	      free((char *) time_ptr);
	    }
	  }
	}

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_create_spec()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	node that is created.
*
* Description:
*	Apply class specific elements in the creation of a node.
*
**************************************************************************/

int vldh_node_create_spec (
  vldh_t_node	node
)
{

	pwr_tObjid 	objdid;
	char		segment_name[32];
	int		i, sts, size;
	ldh_sParDef 	*bodydef;
	int 		rows;
	char		*parvalue;
	int		noclass;
	pwr_tClassId	class;
	char		graphname[2];
	ldh_tSesContext ldhses;
	vldh_t_plc	plc;
	vldh_t_wind	wind;
	float		*time_ptr;
	unsigned long	*objdid_ptr;

	wind = node->hn.window_pointer;
	plc = wind->hw.plcobject_pointer;
	ldhses = wind->hw.ldhsession;

	/**********************************************************
	*	ORDER						
	*	Create objects for the orderattribues.
	**********************************************************/

	if ( node->ln.classid == 
		vldh_class( ldhses, VLDH_CLASS_ORDER))
	{
	  /* Order : Create one object for every attibute with the
	    orderobject as parent */
	  
	  sts = ldh_GetObjectBodyDef((node->hn.window_pointer)->hw.ldhsession,
			node->ln.classid, "DevBody", 1, 
			&bodydef, &rows);

	  if ( EVEN(sts) ) return sts;

	  for ( i = 0; i < rows - 2; i += 2)
	  {
	    /* Get the parameter value */
	    sts = ldh_GetObjectPar( (node->hn.window_pointer)->hw.ldhsession,  
			node->ln.object_did, 
			"DevBody",
			bodydef[i].ParName,
			(char **)&parvalue, &size); 
	    if ( EVEN(sts)) return sts;

	    noclass = 0;
	    if ( *parvalue == 'S' || *parvalue == 's')
	      class = vldh_class( ldhses, VLDH_CLASS_SORDER);
	    else if ( *parvalue == 'L' || *parvalue == 'l')
	      class = vldh_class( ldhses, VLDH_CLASS_LORDER);
	    else if ( *parvalue == 'C' || *parvalue == 'c')
	      class = vldh_class( ldhses, VLDH_CLASS_CORDER);
	    else if ( *parvalue == 'D' || *parvalue == 'd')
	      class = vldh_class( ldhses, VLDH_CLASS_DORDER);
	    else if ( *parvalue == 'P' || *parvalue == 'p')
	      class = vldh_class( ldhses, VLDH_CLASS_PORDER);
	    else
	      noclass = 1;

	    if ( !noclass )
	    {
	      /* Get a suitable name for the object and create it */

	      /* Get default segment name */
	      graphname[0] = *parvalue;
	      graphname[1] = '\0';		
	      sts = vldh_get_node_defname( wind, OT_SUBORDER,  
		graphname, segment_name);
	      if ( EVEN(sts) ) return sts;
	  
	      /* Create the object i ldh */
	      sts = ldh_CreateObject( ldhses, &objdid, segment_name, 
		 class, node->ln.object_did, ldh_eDest_IntoLast);
	      if ( EVEN(sts)) return sts;

	      if ((class == vldh_class( ldhses, VLDH_CLASS_DORDER)) ||
	          (class == vldh_class( ldhses, VLDH_CLASS_LORDER)))
	      {
	        /* Move the time from the parent to the child */
	        sts = ldh_GetObjectPar( ldhses,
			node->ln.object_did, 
			"DevBody",
			bodydef[i + 1].ParName,
			(char **)&time_ptr, &size); 
	        if ( EVEN(sts)) return sts;
	        sts = ldh_SetObjectPar( ldhses,
			objdid, 
			"RtBody",
			"TimerTime",
			(char *)time_ptr, size); 
	        if ( EVEN(sts)) return sts;
	        free((char *) time_ptr);
	      }
	    }	
	    free((char *) parvalue);
	  }
	}

	/**********************************************************
	*	ORDERACT						
	*	Put window objdid in the ldhobject.
	**********************************************************/
	else if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_ORDERACT))
	{
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"OrderObject",
		(char *)&wind->lw.parent_node_did, 
		sizeof(wind->lw.parent_node_did)); 
	  if ( EVEN(sts)) return sts;
	}
	/**********************************************************
	*	CurrentData and CurrentIndex
	*	Put cell objdid in the ldhobject.
	**********************************************************/
	else if ( (node->ln.classid == vldh_eclass( ldhses, "CurrentData")) ||
		(node->ln.classid == vldh_eclass( ldhses, "CurrentIndex")))
	{
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"CellObject",
		(char *)&wind->lw.parent_node_did, 
		sizeof(wind->lw.parent_node_did)); 
	  if ( EVEN(sts)) return sts;
	}

	/**********************************************************
	*	SHOWPLCATTR					
	*	Put plc attributes in the ldhobject.
	**********************************************************/
	else if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_SHOWPLCATTR))
	{
	  pwr_tObjid volume_objid;
	  pwr_tObjid *thread_objid;

	  volume_objid.oix = 0;
	  volume_objid.vid = node->ln.object_did.vid;

	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"Node",
		(char *)&volume_objid, sizeof( volume_objid)); 
	  if ( EVEN(sts)) return sts;

	  sts = ldh_GetObjectPar( ldhses,
			plc->lp.objdid, 
			"RtBody",
			"ThreadObject",
			(char **)&thread_objid, &size); 
	  if ( EVEN(sts)) return sts;
	  if ( cdh_ObjidIsNotNull( *thread_objid)) {
	    sts = ldh_GetObjectPar( ldhses,
				    *thread_objid, 
				    "RtBody",
				    "ScanTime",
				    (char **)&time_ptr, &size); 
	  }
	  free((char *) thread_objid);
	  if ( ODD(sts)) {
	    sts = ldh_SetObjectPar( ldhses,
				    node->ln.object_did, 
				    "DevBody",
				    "ScanTime",
				    (char *)time_ptr, size); 
	    if ( EVEN(sts)) return sts;
	    free((char *) time_ptr);
	  }

	  sts = ldh_GetObjectPar( ldhses,
			plc->lp.objdid, 
			"DevBody",
			"ResetObject",
			(char **)&objdid_ptr, &size); 
	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"ResetObject",
		(char *)objdid_ptr, size); 
	  if ( EVEN(sts)) return sts;
	  free((char *) objdid_ptr);
	}


	/**********************************************************
	*	RESET_SO
	*	Reset sorder object to avoid unwanted sorder at
	*	paste operation.
	**********************************************************/
	else if ( node->ln.classid == vldh_class( ldhses, VLDH_CLASS_RESET_SO))
	{
	  pwr_tOid order_objdid = pwr_cNOid;

	  sts = ldh_SetObjectPar( ldhses,
		node->ln.object_did, 
		"DevBody",
		"OrderObject",
		(char *)&order_objdid, 
		sizeof(order_objdid)); 
	  if ( EVEN(sts)) return sts;
	}

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_node_ldhdel_spec()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	node		I	node that is deleted.
*
* Description:
*	Apply class specific elements when the node deleted in ldh.
*
**************************************************************************/

int vldh_node_ldhdel_spec (
  vldh_t_node	node
)
{

	pwr_tObjid	suborderobjdid[6];
	unsigned long	subordercount;
	pwr_tObjid	next_objdid;
	vldh_t_wind	wind;
	int		i, sts;
	pwr_tClassId	class;
	ldh_tSesContext ldhses;
	vldh_t_plc	plc;

	wind = node->hn.window_pointer;
	plc = wind->hw.plcobject_pointer;
	ldhses = wind->hw.ldhsession;
 
	/**********************************************************
	*	ORDER						
	*	Delete the orderattributes.
	**********************************************************/

	wind = node->hn.window_pointer;
	if ( node->ln.classid == 
		vldh_class( ldhses, VLDH_CLASS_ORDER))
	{
	  /* Check the present suborder objects and compare them with
	    the attributes in the order object */

	  subordercount = 0;
	  sts = ldh_GetChild( ldhses, node->ln.object_did, &next_objdid);
	  while ( ODD(sts) )
	  {
	    /* Find out if this is a suborder and */
	    sts = ldh_GetObjectClass( ldhses, 
		next_objdid, &class);

	    if ( (class == vldh_class( ldhses, VLDH_CLASS_SORDER)) || 
		(class == vldh_class( ldhses, VLDH_CLASS_LORDER)) || 
		(class == vldh_class( ldhses, VLDH_CLASS_DORDER)) || 
		(class == vldh_class( ldhses, VLDH_CLASS_PORDER)) || 
		(class == vldh_class( ldhses, VLDH_CLASS_CORDER)))
	    {
	      suborderobjdid[subordercount] = next_objdid;
	      subordercount++;
	    }

	    sts = ldh_GetNextSibling( ldhses, 
			next_objdid, &next_objdid);
	  }

	  for ( i = 0; i < subordercount; i++)
	  {
	    /* delete the object */
	    sts = ldh_DeleteObject( ldhses, 
		suborderobjdid[i]);
	    if( EVEN(sts)) return sts;
	  }

	}

	return VLDH__SUCCESS;
}


/*************************************************************************
*
* Name:		vldh_get_node_coordinates()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* unsigned long * node_count	O	number of nodes in the nodelist
* void **	intern_nodelist	O	list of nodes in the window 
*
* Description:
*	Returns a node that covers the specified coordinates.
*	Excludes the document nodes.
*	If no covering node is found VLDH__NONODES i returned
*
**************************************************************************/

int vldh_get_node_coordinates ( 
  vldh_t_wind		wind,
  float			x,
  float			y,
  vldh_t_node		*node
)
{
	vldh_t_node	node_pointer;
	int		found;

	found = 0;
	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  /* Put node in array if not deleted */
	  if ( (node_pointer->hn.status & VLDH_DELETE) == 0 )
	  {
	    if ( !vldh_check_document( wind->hw.ldhsession,
					node_pointer->ln.object_did))
	    {
	      /* This is not a document, check coordinates */
	      if ( 	( x >= node_pointer->ln.x ) &&
			( x <= node_pointer->ln.x + node_pointer->ln.width) &&
			( y >= node_pointer->ln.y ) &&
			( y <= node_pointer->ln.y + node_pointer->ln.height))
	      {	   
	        *node = node_pointer;
	        found = 1;
	        break;
	      }
	    }
	  }

	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}
	if( !found) return VLDH__NONODES;
	else return VLDH__SUCCESS;
}

/*************************************************************************
*
* Name:		vldh_get_modification()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_wind	wind		I	vldh window
* int		*modified	O	1 if modifications found, else 0.
*
* Description:
*	Examines if any object is created, deleted or modified since last
*	save.
*
**************************************************************************/

int vldh_get_wind_modification (
  vldh_t_wind		wind,
  int			*modified
)
{
	vldh_t_node	node_pointer;
	vldh_t_con	con_pointer;


	if ( wind->hw.status & 
		( VLDH_DELETE | VLDH_MODIFY | VLDH_CREATE ))
	{
	  *modified = 1;
	  return VLDH__SUCCESS;
	}

	node_pointer = wind->hw.node_list_pointer;
	while ( node_pointer != 0 )
	{
	  if ( node_pointer->hn.status & 
		( VLDH_DELETE | VLDH_MODIFY | VLDH_CREATE ))
	  {
	    if ( !( node_pointer->hn.status & VLDH_LDHDELETE))
	    {
	      *modified = 1;
	      return VLDH__SUCCESS;
	    }
	  }
	  /* Next node */
	  node_pointer = node_pointer->hn.object_pointer;
	}

	con_pointer = wind->hw.con_list_pointer;
	while ( con_pointer != 0 )
	{
	  if ( con_pointer->hc.status & 
		( VLDH_DELETE | VLDH_MODIFY | VLDH_CREATE ))
	  {
	    if ( !( con_pointer->hc.status & VLDH_LDHDELETE))
	    {
	      *modified = 1;
	      return VLDH__SUCCESS;
	    }
	  }
	  /* Next con */
	  con_pointer = con_pointer->hc.object_pointer;
	}

	*modified = 0;
	return VLDH__SUCCESS;
}



/*************************************************************************
*
* Name:		vldh_IdToStr()
*
* Type		* char
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Converts an objid to a string.
*	The returned string is static and must be used befor next call
*	of the function.
*
**************************************************************************/

char	*vldh_IdToStr(
  int		idx,
  pwr_tObjid 	objid
)
{
	static char	str0[80];
	static char	str1[80];
	static char	str2[80];
	static char	str3[80];
	static char	str4[80];
	static char	str5[80];
	unsigned char	volid[4];
	char		*str;

	memcpy( &volid, &objid.vid, sizeof(volid));
	switch (idx)
	{
	  case 0: str = str0; break;
	  case 1: str = str1; break;
	  case 2: str = str2; break;
	  case 3: str = str3; break;
	  case 4: str = str4; break;
	  case 5: str = str5; break;
	  default:
	    printf( "** Error, vldh_IdToStr, Invalid index!!\n");
	    str = str0;
	}
	sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u_%8.8x",
		volid[3], volid[2], volid[1], volid[0], objid.oix);
	return str;
}


/*************************************************************************
*
* Name:		vldh_StrToId()
*
* Type		* char
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Converts a string to objid. String can be of normal objid 
*	of plc module format (with underscore and hexadecimal objid).
*
**************************************************************************/
pwr_tStatus vldh_StrToId( 
  char 		*instr, 
  pwr_tObjid 	*objid
)
{
	char	outstr[40];
	char	*s;
	char	*t;
	int	i;
	int	plc_m;

	s = instr;
	t = outstr;
	while( *s && !isdigit(*s))
          s++;

	if ( strchr( s, '_'))
	  plc_m = 1;
	else
	  plc_m = 0;

	i = 0;
	while( *s)
	{
	  if ( *s == '_')
	  {
	    i++;
	    if ( i == 4)
	    {
	      *t = ':';
	      if ( plc_m)
	        break;
	    }
	    else
	      *t = '.';
	  }
	  else
	    *t = *s;
          s++;
          t++;
	}

	if ( plc_m)
	{
	  s++;
	  t++;
	  if ( sscanf( s, "%x", &i) == 1)
	    sprintf( t, "%d", i);
	  else
	    sprintf( t, "%s", s);
	}
	else
  	  *t = 0;

	return cdh_StringToObjid( outstr, objid);
}

/*************************************************************************
*
* Name:		vldh_VolumeIdToStr()
*
* Type		* char
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Converts an VolumeId to a string.
*	The returned string is static and must be used befor next call
*	of the function.
*
**************************************************************************/

char	*vldh_VolumeIdToStr( pwr_tVolumeId volumeid)
{
	static char	str[80];
	unsigned char	volid[4];

	memcpy( &volid, &volumeid, sizeof(volid));
	sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u",
		volid[3], volid[2], volid[1], volid[0]);
	return str;
}

static void cnv_from_neted( vldh_t_node n)
{
  const float pin = 0.05;
  const float grid = 0.05;

  switch( n->ln.classid)
  {
    case pwr_cClass_order:
    case pwr_cClass_OrderAct:
    case pwr_cClass_reset_so:
    case pwr_cClass_setcond:
    case pwr_cClass_cstoao:
    case pwr_cClass_cstoap:
    case pwr_cClass_cstoav:
    case pwr_cClass_CStoAtoIp:
    case pwr_cClass_GetAi:
    case pwr_cClass_GetAo:
    case pwr_cClass_GetAp:
    case pwr_cClass_GetAv:
    case pwr_cClass_GetData:
    case pwr_cClass_GetDi:
    case pwr_cClass_GetDo:
    case pwr_cClass_GetDp:
    case pwr_cClass_GetDv:
    case pwr_cClass_GetIpToA:
    case pwr_cClass_GetPi:
    case pwr_cClass_resdo:
    case pwr_cClass_resdp:
    case pwr_cClass_resdv:
    case pwr_cClass_setdo:
    case pwr_cClass_setdp:
    case pwr_cClass_setdv:
    case pwr_cClass_stoao:
    case pwr_cClass_stoap:
    case pwr_cClass_stoav:
    case pwr_cClass_stodo:
    case pwr_cClass_stodp:
    case pwr_cClass_stodv:
    case pwr_cClass_StoAtoIp:
    case pwr_cClass_ASup:
    case pwr_cClass_and:
    case pwr_cClass_DSup:
    case pwr_cClass_edge:
    case pwr_cClass_inv:
    case pwr_cClass_or:
    case pwr_cClass_pulse:
    case pwr_cClass_timer:
    case pwr_cClass_wait:
    case pwr_cClass_xor:
    case pwr_cClass_sum:
    case pwr_cClass_Backup:
      n->ln.y = 1 - n->ln.y - n->ln.height + 0.5 * grid;
      break;
    case pwr_cClass_initstep:
    case pwr_cClass_step:
    case pwr_cClass_substep:
    case pwr_cClass_trans:
    case pwr_cClass_ssbegin:
    case pwr_cClass_ssend:
      n->ln.y = 1 - n->ln.y - n->ln.height/2;     
      break;
    case pwr_cClass_BodyText:
    case pwr_cClass_Document:
    case pwr_cClass_DocUser1:
    case pwr_cClass_DocUser2:
    case pwr_cClass_Frame:
    case pwr_cClass_Head:
    case pwr_cClass_ShowPlcAttr:
    case pwr_cClass_Text:
    case pwr_cClass_Title:
      n->ln.y = 1 - n->ln.y  - n->ln.height + 0.5 * grid;
      break;
    case pwr_cClass_Point:
      n->ln.y = 1 - n->ln.y;
      break;
    default:
      n->ln.y = 1 - n->ln.y  - n->ln.height + 1.5 * grid;
  }

  switch( n->ln.classid)
  {
    case pwr_cClass_trans:
      n->ln.x = n->ln.x + n->ln.width/2;     
      break;
    case pwr_cClass_initstep:
    case pwr_cClass_step:
    case pwr_cClass_substep:
    case pwr_cClass_ssbegin:
    case pwr_cClass_ssend:
      n->ln.x = n->ln.x + n->ln.width/2 - pin/2;     
      break;
    case pwr_cClass_GetAi:
    case pwr_cClass_GetAo:
    case pwr_cClass_GetAp:
    case pwr_cClass_GetAv:
    case pwr_cClass_GetData:
    case pwr_cClass_GetDi:
    case pwr_cClass_GetDo:
    case pwr_cClass_GetDp:
    case pwr_cClass_GetDv:
    case pwr_cClass_GetIpToA:
    case pwr_cClass_GetPi:
    case pwr_cClass_BodyText:
    case pwr_cClass_Document:
    case pwr_cClass_DocUser1:
    case pwr_cClass_DocUser2:
    case pwr_cClass_Frame:
    case pwr_cClass_Head:
    case pwr_cClass_ShowPlcAttr:
    case pwr_cClass_Text:
    case pwr_cClass_Title:
    case pwr_cClass_Point:
      break;
    default:
      n->ln.x += pin;
      break;
  }
}

static void cnv_to_neted( vldh_t_node n)
{
  const float pin = 0.05;
  const float grid = 0.05;

  switch( n->ln.classid)
  {
    case pwr_cClass_order:
    case pwr_cClass_OrderAct:
    case pwr_cClass_reset_so:
    case pwr_cClass_setcond:
    case pwr_cClass_cstoao:
    case pwr_cClass_cstoap:
    case pwr_cClass_cstoav:
    case pwr_cClass_CStoAtoIp:
    case pwr_cClass_GetAi:
    case pwr_cClass_GetAo:
    case pwr_cClass_GetAp:
    case pwr_cClass_GetAv:
    case pwr_cClass_GetData:
    case pwr_cClass_GetDi:
    case pwr_cClass_GetDo:
    case pwr_cClass_GetDp:
    case pwr_cClass_GetDv:
    case pwr_cClass_GetIpToA:
    case pwr_cClass_GetPi:
    case pwr_cClass_resdo:
    case pwr_cClass_resdp:
    case pwr_cClass_resdv:
    case pwr_cClass_setdo:
    case pwr_cClass_setdp:
    case pwr_cClass_setdv:
    case pwr_cClass_stoao:
    case pwr_cClass_stoap:
    case pwr_cClass_stoav:
    case pwr_cClass_stodo:
    case pwr_cClass_stodp:
    case pwr_cClass_stodv:
    case pwr_cClass_StoAtoIp:
    case pwr_cClass_ASup:
    case pwr_cClass_and:
    case pwr_cClass_DSup:
    case pwr_cClass_edge:
    case pwr_cClass_inv:
    case pwr_cClass_or:
    case pwr_cClass_pulse:
    case pwr_cClass_timer:
    case pwr_cClass_wait:
    case pwr_cClass_xor:
    case pwr_cClass_sum:
    case pwr_cClass_Backup:
      n->ln.y = 1 - n->ln.y - n->ln.height + 0.5 * grid;
      break;
    case pwr_cClass_initstep:
    case pwr_cClass_step:
    case pwr_cClass_substep:
    case pwr_cClass_trans:
    case pwr_cClass_ssbegin:
    case pwr_cClass_ssend:
      n->ln.y = 1 - n->ln.y - n->ln.height/2;     
      break;
    case pwr_cClass_BodyText:
    case pwr_cClass_Document:
    case pwr_cClass_DocUser1:
    case pwr_cClass_DocUser2:
    case pwr_cClass_Frame:
    case pwr_cClass_Head:
    case pwr_cClass_ShowPlcAttr:
    case pwr_cClass_Text:
    case pwr_cClass_Title:
      n->ln.y = 1 - n->ln.y  - n->ln.height + 0.5 * grid;
      break;
    case pwr_cClass_Point:
      n->ln.y = 1 - n->ln.y;
      break;
    default:
      n->ln.y = 1 - n->ln.y  - n->ln.height + 1.5 * grid;
  }
  switch( n->ln.classid)
  {
    case pwr_cClass_trans:
      n->ln.x = n->ln.x - n->ln.width/2;     
      break;
    case pwr_cClass_initstep:
    case pwr_cClass_step:
    case pwr_cClass_substep:
    case pwr_cClass_ssbegin:
    case pwr_cClass_ssend:
      n->ln.x = n->ln.x - n->ln.width/2 + pin/2;     
      break;
    case pwr_cClass_GetAi:
    case pwr_cClass_GetAo:
    case pwr_cClass_GetAp:
    case pwr_cClass_GetAv:
    case pwr_cClass_GetData:
    case pwr_cClass_GetDi:
    case pwr_cClass_GetDo:
    case pwr_cClass_GetDp:
    case pwr_cClass_GetDv:
    case pwr_cClass_GetIpToA:
    case pwr_cClass_GetPi:
    case pwr_cClass_BodyText:
    case pwr_cClass_Document:
    case pwr_cClass_DocUser1:
    case pwr_cClass_DocUser2:
    case pwr_cClass_Frame:
    case pwr_cClass_Head:
    case pwr_cClass_ShowPlcAttr:
    case pwr_cClass_Text:
    case pwr_cClass_Title:
    case pwr_cClass_Point:
      break;
    default:
      n->ln.x -= pin;
      break;
  }
}
