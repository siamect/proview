/* wb_exo.c -- execute order

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module create an executer order for plc objects.  */

#include <stdio.h>
#include <string.h>
#ifdef OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#endif

#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_foe_macros.h"
#include <Xm/Xm.h>
#include "wb_vldh.h"
#include "wb_ldh.h"
#include "wb_goen.h"
#include "wb_gcg.h"
#include "wb_utl.h"
#include "co_msg.h"
#include "wb_exo.h"

/*_Local procedues_______________________________________________________*/


#define	EXO_NOT_YET_EXECUTED	60000


typedef struct {
	vldh_t_wind		wind;
	ldh_tSesContext		ldhses;
	unsigned long		execute_counter;
	int			errorcount;
	vldh_t_node		errornode;
	vldh_t_node		*objectarray;
	int			objectarraycount;
	} exo_t_ctx, *exo_ctx;

int	exo_exec_m0();
int	exo_exec_m1();
int	exo_exec_m2();
int	exo_exec_m3();
int	exo_exec_m4();
int	exo_exec_m5();
int	exo_exec_m6();
int	exo_exec_m7();
int	exo_exec_m8();
int	exo_exec_m9();

int	(* exo_exec_m[20]) () = {
	&exo_exec_m0,
	&exo_exec_m1,
	&exo_exec_m2,
	&exo_exec_m3,
	&exo_exec_m4,
	&exo_exec_m5,
	&exo_exec_m6,
	&exo_exec_m7,
	&exo_exec_m8,
	&exo_exec_m9,
	};


/*_Methods defined for this module_______________________________________*/

/*************************************************************************
*
* Name:		exo_ykoord_order_nodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* vldh_t_node	*nodelist	I	list of vldh nodes
* unsigned long	node_count	I	number of nodes in nodelist
*					on the nodes.
*
* Description:	Order the nodes in the nodelist in order of increasing
*	y koordinate.
*
**************************************************************************/

int exo_ykoord_order_nodes( node_count, nodelist)
unsigned long		node_count;
vldh_t_node		*nodelist;
{
	vldh_t_node		*node_ptr1;
	vldh_t_node		*node_ptr2;
	vldh_t_node		dum;
	int			i,j;

	for ( i = node_count - 1; i > 0; i--)
	{
	  node_ptr1 = nodelist;
	  node_ptr2 = node_ptr1 + 1;
	  for ( j = 0; j < i; j++)
	  {
	    if ( (*node_ptr1)->ln.y > (*node_ptr2)->ln.y)
	    {
	      /* Change order */
	      dum = *node_ptr2;
	      *node_ptr2 = *node_ptr1;
	      *node_ptr1 = dum;
	    }
	    node_ptr1++;
	    node_ptr2++;
	  }
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		exo_ctx_new()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	*exoctx		O	created exo context
* vldh_t_wind	wind		I	vldh window.
*
* Description:
*	Create a exo context for a window.
*
**************************************************************************/

void	exo_ctx_new( exoctx, wind)
exo_ctx		*exoctx;
vldh_t_wind	wind;
{
	/* Create the context */
	*exoctx = (exo_ctx) XtCalloc( 1, sizeof( **exoctx ) );

	(*exoctx)->objectarray = (vldh_t_node *) XtCalloc( 1, sizeof( unsigned long));
	(*exoctx)->objectarraycount = 0;

	(*exoctx)->wind = wind;
	(*exoctx)->execute_counter = 1;
	(*exoctx)->ldhses = wind->hw.ldhses;
}
/*************************************************************************
*
* Name:		exo_ctx_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
*
* Description:
*	Delete a context for a window.
*	Free's all allocated memory in the exo context.
*
**************************************************************************/

void	exo_ctx_delete( exoctx)
exo_ctx		exoctx;
{
	/* Free the memory for the objectarraylist and the context */
	XtFree((char *) exoctx->objectarray);
	XtFree((char *) exoctx);
}

/*************************************************************************
*
* Name:		exo_objectarray_insert()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I 	vldh node.
*
* Description:
*	Insert the node in the objectarray list in exo context.
*	Check first that the node is not already inserted.
*
**************************************************************************/

int	exo_objectarray_insert( exoctx, node)
exo_ctx		exoctx;
vldh_t_node	node;
{
	int		i;
	vldh_t_node	*node_ptr;
	int		sts;
	
	/* Check if the objdid already is inserted */
	node_ptr = exoctx->objectarray;
	for ( i = 0; i < exoctx->objectarraycount; i++)
	{
	  if ( *node_ptr == node)
	  {
	    return GSX__ALREADY_INSERTED;
	  }
	  node_ptr++;
	}

	/* The objdid was not found, insert it */
	/* Increase size of the iolist */
	sts = utl_realloc( (char **) &exoctx->objectarray, 
		exoctx->objectarraycount * sizeof(vldh_t_node), 
		(exoctx->objectarraycount + 1) * sizeof(vldh_t_node));
	if (EVEN(sts)) return sts;

	node_ptr = exoctx->objectarray + exoctx->objectarraycount;
	*node_ptr = node;
	exoctx->objectarraycount++;

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		exo_error_msg()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long sts		I	message identifier
* vldh_t_node	node		I	vldh node
*
* Description:
*	Prints a error or warning message for a node.
*
**************************************************************************/

int exo_error_msg( sts, node)
unsigned long 	sts;
vldh_t_node	node;
{
	static char msg[256];
	int	status, size;
	char	hier_name[80];
	FILE 		*logfile;

	logfile = NULL;

	if (EVEN(sts))
	{
	  msg_GetMsg( sts, msg, sizeof(msg));

	  if (logfile != NULL)
	    fprintf(logfile, "%s\n", msg);
	  else
	    printf("%s\n", msg);
	  if ( node != 0)
	  {
	    /* Get the full hierarchy name for the node */
	    status = ldh_ObjidToName( 
		(node->hn.wind)->hw.ldhses, 
		node->ln.oid, ldh_eName_Hierarchy,
		hier_name, sizeof( hier_name), &size);
	    if( EVEN(status)) return status;
	    if (logfile != NULL)
	      fprintf(logfile, "        in object  %s\n", hier_name);
	    else
	      printf("        in object  %s\n", hier_name);
	  }
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		exo_node_exec()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Calculates an execute order for an object.
*	Calls the executeorder method for the object.
*	Executeordermethod is stored in the parameter executeordermethod 
*	in graphbody of the class object.
*
**************************************************************************/

int exo_node_exec( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	int 			sts, size;
	int			executeordermethod;

	/* Get executeorder method for this node */
	sts = ldh_GetClassBody( exoctx->ldhses, node->ln.cid, 
		"GraphPlcNode", &bodyclass, (char **) &graphbody, &size);
	if ( EVEN(sts)) return sts;

	executeordermethod = graphbody->executeordermethod;
	sts = (exo_exec_m[ executeordermethod ]) (exoctx, node); 

	return sts;
}	

/*************************************************************************
*
* Name:		exo_wind_exec()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Calculates an execute order for a window object.
*	Calls the executeorder method for the object.
*	Executeordermethod is stored in the parameter executeordermethod 
*	in graphbody of the class object.
*
**************************************************************************/

int exo_wind_exec ( 
	vldh_t_wind	wind
)
{
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcWindow 	*graphbody;
	int 			sts, size;
	int			executeordermethod;

	/* Get executeorder method for this node */
	sts = ldh_GetClassBody(wind->hw.ldhses, wind->lw.cid, 
		"GraphPlcWindow", &bodyclass, (char **) &graphbody, &size);
	if ( EVEN(sts)) return sts;

	executeordermethod = graphbody->executeordermethod;
	sts = (exo_exec_m[ executeordermethod ]) (0, wind); 

	return sts;
}	


/*************************************************************************
*
* Name:		exo_exec_m0()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for an object that should not execute.
*
**************************************************************************/

int exo_exec_m0( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		*bodydef;
	ldh_sParDef 		output_bodydef;
	int 			rows, sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Get the runtime parameters for this class */
	sts = ldh_GetObjectBodyDef( exoctx->ldhses, node->ln.cid, 
			"RtBody", 1, &bodydef, &rows);
	if ( ODD(sts) )
	{
	  i = 0;
	  while( (bodydef[i].ParClass == pwr_eClass_Input) &&
		(i < rows))
	  {
	    /* Get the point for this parameter if there is one */
	    sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	    if ( ODD( sts))
	    {
	      /* Look for an output connected to this point */
	      sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	      if ( EVEN(sts)) return sts;

	      if ( output_count > 0 )
	      {
	        sts = exo_node_exec( exoctx, output_node);
	        if (EVEN(sts)) return sts;
	      }
	    }
	    i++;
	  }
	  free((char *) bodydef);
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = 0;
	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m1()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	wind		I	vldh node.
*
* Description:
*	Execute order method for a plc window.
*
**************************************************************************/

int exo_exec_m1( dummy, wind)
exo_ctx dummy;
vldh_t_wind	wind;
{
	exo_ctx	exoctx;
	int			sts;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		node;
	int			j;

	exo_ctx_new( &exoctx, wind);


	/* Get all nodes this window */
	vldh_get_nodes( wind, &node_count, &nodelist);

	/* Order them in y koordinate */
	exo_ykoord_order_nodes( node_count, nodelist);

	/* Reset execute_order */
        for ( j = 0; j < node_count; j++)
	{
	  node = *(nodelist + j);
	  node->hn.executeorder = EXO_NOT_YET_EXECUTED;
	}	  

	/* Call the methods for each node */
        for ( j = 0; j < node_count; j++)
	{
	  node = *(nodelist + j);
	  exoctx->objectarraycount = 0;
	  sts = exo_node_exec( exoctx, node);
	  if ( sts == GSX__AMBIGOUS_EXECUTEORDER)
	  {
	    exo_error_msg( sts, exoctx->errornode);
	    goto classerror;
	  }
	  else if ( EVEN(sts)) goto classerror; 
	}	  
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	exo_ctx_delete( exoctx);

	return GSX__SUCCESS;

classerror:
	/* If error status from ldh */
/*	gcg_wind_msg( gcgctx, GSX__CLASSERR, 0);
*/
	/* Delete the context for the window */
	XtFree((char *) nodelist);
	exo_ctx_delete( exoctx);

	return sts;
}	


/*************************************************************************
*
* Name:		exo_exec_m2()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for an ordinary plc object.
*
**************************************************************************/

int exo_exec_m2( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		*bodydef;
	ldh_sParDef 		output_bodydef;
	int 			rows, sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Get the runtime parameters for this class */
	sts = ldh_GetObjectBodyDef( exoctx->ldhses, node->ln.cid, 
			"RtBody", 1, &bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	i = 0;
	while( (bodydef[i].ParClass == pwr_eClass_Input) &&
		(i < rows))
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0 )
	    {
	      sts = exo_node_exec( exoctx, output_node);
	      if (EVEN(sts)) return sts;
	    }
	  }
	  i++;
	}
	free((char *) bodydef);

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m3()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for a trans  object.
*
**************************************************************************/

int exo_exec_m3( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int 			sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check only the condition input */
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_inputpoint( node, 2, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0 )
	    {
	      sts = exo_node_exec( exoctx, output_node);
	      if (EVEN(sts)) return sts;
	    }
	  }
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0) 
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m4()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for a step  object.
*
**************************************************************************/

int exo_exec_m4( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	int 			sts;
	int			i, k;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;
	unsigned long		point_count;
	vldh_t_conpoint		*pointlist;
	vldh_t_node		next_node;
	pwr_tClassId		class;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check only the trans output on par nr 0 and 1 */
	for ( i = 0; i < 2; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_point( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    gcg_get_conpoint_nodes( node, point, &point_count, &pointlist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( point_count > 1 )
	    {
	      for ( k = 1; k < point_count; k++)
	      {
	        next_node = (pointlist + k)->node;
	        /* Check class of connected nodes */	
	        sts = ldh_GetObjectClass( exoctx->ldhses, 
			next_node->ln.oid, &class);
	        if (EVEN(sts)) return sts;
	        if (class == pwr_cClass_trans)
	        {
	          sts = exo_node_exec( exoctx, next_node);
	          if ( EVEN(sts)) return sts;
	        }
	      }
	    }
	  }
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m5()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for an order object.
*
**************************************************************************/

int exo_exec_m5( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int 			sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;
	unsigned long		step_execute_order;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check the inputs */
	for ( i = 0; i < 2; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0 )
	    {
	      sts = exo_node_exec( exoctx, output_node);
	      if (EVEN(sts)) return sts;
	    }
	    if ( i == 0)
	    {
	      if ( output_count > 0)
	        /* Store the executer order of the step */
	        step_execute_order = output_node->hn.executeorder;
	      else
	        /* Not connected to a step !! */
	        step_execute_order = 0;
	    }
	  }
	  i++;
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = step_execute_order;

	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m6()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for set-, res-, sto- do, di and dv, and reset_so.
*
**************************************************************************/

int exo_exec_m6( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int 			sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;
	unsigned long		order_execute_order;
	pwr_tClassId		class;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	for ( i = 0; i < 1; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0 )
	    {
	      sts = exo_node_exec( exoctx, output_node);
	      if (EVEN(sts)) return sts;
	      
	      sts = ldh_GetObjectClass( exoctx->ldhses, 
			output_node->ln.oid, &class);
	      if (EVEN(sts)) return sts;
	      /* Store the order execute order if it is an order */
	      if (class == pwr_cClass_order)
	        order_execute_order = output_node->hn.executeorder;
	      else
	        order_execute_order = 0;
	    }
	  }
	  i++;
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	if ( order_execute_order )
	  node->hn.executeorder = order_execute_order;
	else
	{
	  node->hn.executeorder = exoctx->execute_counter;
	  exoctx->execute_counter++;
	}

	return GSX__SUCCESS;
}	


/*************************************************************************
*
* Name:		exo_exec_m7()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for a ssbegin and ssend  object.
*
**************************************************************************/

int exo_exec_m7( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	int 			sts;
	int			i, k;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;
	unsigned long		point_count;
	vldh_t_conpoint		*pointlist;
	vldh_t_node		next_node;
	pwr_tClassId		class;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check only the trans output on par nr 0 */
	for ( i = 0; i < 1; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_point( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    gcg_get_conpoint_nodes( node, point, &point_count, &pointlist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( point_count > 1 )
	    {
	      for ( k = 1; k < point_count; k++)
	      {
	        next_node = (pointlist + k)->node;
	        /* Check class of connected nodes */	
	        sts = ldh_GetObjectClass( exoctx->ldhses, 
			next_node->ln.oid, &class);
	        if (EVEN(sts)) return sts;
	        if (class == pwr_cClass_trans)
	        {
	          sts = exo_node_exec( exoctx, next_node);
	          if ( EVEN(sts)) return sts;
	        }
	      }
	    }
	  }
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	

/*************************************************************************
*
* Name:		exo_exec_m8()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for a NMpsCell, NMpsStoreCell and NMpsOutCell
*	object.
*
**************************************************************************/

int exo_exec_m8( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	int 			sts;
	int			i, k;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;
	unsigned long		point_count;
	vldh_t_conpoint		*pointlist;
	vldh_t_node		next_node;
	pwr_tClassId		class;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check only the transportobjects output on par nr 0 and 1 */
	for ( i = 0; i < 2; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_point( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    gcg_get_conpoint_nodes( node, point, &point_count, &pointlist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( point_count > 1 )
	    {
	      for ( k = 1; k < point_count; k++)
	      {
	        next_node = (pointlist + k)->node;
	        /* Check class of connected nodes */	
	        sts = ldh_GetObjectClass( exoctx->ldhses, 
			next_node->ln.oid, &class);
	        if (EVEN(sts)) return sts;

	        sts = exo_node_exec( exoctx, next_node);
	        if ( EVEN(sts)) return sts;
	      }
	    }
	  }
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0)
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	

/*************************************************************************
*
* Name:		exo_exec_m9()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exo_ctx	exoctx		I	exo context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Execute order method for a NMpsTrp  object.
*
**************************************************************************/

int exo_exec_m9( exoctx, node)
exo_ctx	exoctx;
vldh_t_node	node;
{
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int 			sts;
	int			i;
	vldh_t_node 		*nodelist;
	vldh_t_node 		*node_ptr;
	unsigned long		node_count;

	if ( node->hn.executeorder != EXO_NOT_YET_EXECUTED)
	{
	  return GSX__SUCCESS;
	}

	sts = exo_objectarray_insert( exoctx, node);
	if ( sts == GSX__ALREADY_INSERTED)
	{
	  /* A loop is detected */
	  exoctx->errornode = node;
	  return GSX__AMBIGOUS_EXECUTEORDER;
	}
	/* Check the the nodes with an output connected to an input 
		connectionpoint of the current node */

	/* Check only the trigg condition inputs */
	for ( i = 2; i < 6; i++)
	{
	  /* Get the point for this parameter if there is one */
	  sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_EXECUTEORDER | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0 )
	    {
	      sts = exo_node_exec( exoctx, output_node);
	      if (EVEN(sts)) return sts;
	    }
	  }
	}

	/* Check the nodes connected with executer order connections */

	sts = vldh_get_nodes_node( node, &node_count, &nodelist,
		GOEN_CON_EXECUTEORDER | GOEN_CON_OBJTOOBJ,
		VLDH_NODE_SOURCE);
	if ( EVEN(sts)) return sts;

	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
	{
	  sts = exo_node_exec( exoctx, *node_ptr);
	  if (EVEN(sts)) return sts;
	  node_ptr++;
	}
	if ( node_count > 0) 
	  XtFree((char *) nodelist);

	node->hn.executeorder = exoctx->execute_counter;
	exoctx->execute_counter++;

	return GSX__SUCCESS;
}	

