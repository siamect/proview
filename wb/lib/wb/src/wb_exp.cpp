/* 
 * Proview   $Id: wb_exp.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_exp.cpp -- export PLC programs
   Module for exporting the plcprogram.  */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_cdh.h"

#include "flow.h"
#include "flow_ctx.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_exo.h"
#include "wb_exp.h"
#include "wb_trv.h"
#include "wb_foe.h"
#include "wb_gcg.h"

#define MAX_FILENAME_LEN 32

typedef struct {
	pwr_tObjid	window_objdid;
	vldh_t_wind	wind;
	ldh_tSesContext ldhses;
	FILE		*file;
	int		step_gen;
	} exp_t_genctx, *exp_genctx;

gcg_t_ctx		gcgctx_dum;

/* Local function prototypes. */

static pwr_tStatus exp_vldh_load_window ( 	
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  pwr_tObjid		window, 
  vldh_t_wind		*loaded_wind,
  ldh_eAccess		access
);
static pwr_tStatus exp_node_gen (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus exp_ObjdidToName (
  vldh_t_node node,
  pwr_tObjid output_objdid, 
  char *output_name,
  int size
);
static pwr_tStatus gen_m2 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m3 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m4 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m5 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m6 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m7 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m8 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m9 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m10  (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m11 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_m12 (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_print_inputs (
  exp_genctx	genctx,
  vldh_t_node	node,
  unsigned long	printtype,
  unsigned long	*nocondef,
  unsigned long	*nocontype
);
static pwr_tStatus gen_print_interns (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_print_windows (
  exp_genctx	genctx,
  vldh_t_node	node
);
static pwr_tStatus gen_filename (
  ldh_tSesContext	ldhses,
  pwr_tObjid		objid,
  char			*filename
);

/*************************************************************************
*
* Name:		exp_vldh_load_window()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Load a window in vldh if not loaded.
*
**************************************************************************/

static pwr_tStatus exp_vldh_load_window ( 	
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  pwr_tObjid		window, 
  vldh_t_wind		*loaded_wind,
  ldh_eAccess		access
)
{
	pwr_tStatus	sts;
	int		i;
	vldh_t_wind	wind;
	vldh_t_wind	parentwind;
	vldh_t_plc	plc;
	vldh_t_node	node;
	pwr_tObjid	*parentlist;
	unsigned long	parent_count;

	/* Get the parentlist for this window */
	sts = trv_get_parentlist( ldhses, window, &parent_count, 
		&parentlist);
	if ( EVEN(sts)) return sts;

	/* Check if the plcpgm is loaded in vldh */
	sts = vldh_get_plc_objdid( *(parentlist + parent_count -1), &plc);
	if ( sts == VLDH__OBJNOTFOUND )
	{
	  /* Load the plcpgm */
	  sts = vldh_plc_load( *(parentlist + parent_count -1), 
		ldhwb, ldhses, &plc);
	}
	else
	  if ( EVEN(sts)) return sts;
	    
	for ( i = parent_count - 2; i >= 0; i -= 2)
	{
	  /* Check if this window is loaded */
	  sts = vldh_get_wind_objdid( *(parentlist + i), &wind); 
	  if ( sts == VLDH__OBJNOTFOUND )
	  {
	    /* Load the window */
	    if ( i == (int)( parent_count - 2))
	    {
	      /* This is the child to the plc */
	      sts = vldh_wind_load( plc, 0, *(parentlist + i), 0, &wind,
			access);
	      if ( EVEN(sts)) return sts;
	      plc->hp.wind = wind;
	    }
	    else
	    {
	      /* Get the parent vldhnode */
	      sts = vldh_get_node_objdid( *(parentlist + i + 1), parentwind, 
			&node);
	      if ( EVEN(sts)) return sts;
	       
	      sts = vldh_wind_load( plc, node, *(parentlist + i), 0, 
			&wind, access);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = vldh_wind_load_all( wind);
	    if ( EVEN(sts)) return sts;

	  }
	  else
	    if ( EVEN(sts)) return sts;

	  parentwind = wind;
	}
  	/* Load the last window */
	/* Check if this window is loaded */
	sts = vldh_get_wind_objdid( window, &wind); 
	if ( sts == VLDH__OBJNOTFOUND )
	{
	  if ( parent_count == 1)
	  {
	    /* This is the child to the plc */
	    sts = vldh_wind_load( plc, 0, window, 0, &wind, access);
	    if ( EVEN(sts)) return sts;
	    plc->hp.wind = wind;
	  }
	  else
	  {
	    /* Get the parent vldhnode */
	    sts = vldh_get_node_objdid( *parentlist, parentwind,  
			&node);
	    if ( EVEN(sts)) return sts;
	       
	    sts = vldh_wind_load( plc, node, window, 0, &wind, access);
	    if ( EVEN(sts)) return sts;
	  }
	  sts = vldh_wind_load_all( wind);
	  if ( EVEN(sts)) return sts;
	}
	*loaded_wind = wind;

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		exp_plcpgm()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Export a plcpgm.
*
**************************************************************************/

pwr_tStatus exp_plcpgm (
  ldh_tSesContext	ldhses,
  char			*plcpgm_str,
  char			*output
)
{
	pwr_tObjid	plcpgm;
	pwr_tStatus	sts;
	int		size;
	char		filename[80];
	FILE		*file;
	pwr_tObjid	window;
	char		*windbuffer;
	pwr_tClassId	cid;
	pwr_tObjid	parent;
	pwr_tString80	class_name;
	pwr_tString80	hiername;
	pwr_tString80	plcpgm_name;
	pwr_tString80	parent_name;
	pwr_tString80	cmd;
	pwr_tObjid	*resobjdid_ptr;
	pwr_tFloat32	*scantime_ptr;

	/* Get the objdid for the plcpgm */
	sts = ldh_NameToObjid( ldhses, &plcpgm, plcpgm_str);
	if ( EVEN(sts)) return sts;
	if ( EVEN(sts)) return FOE__PLCNOTFOUND;

	if ( !output)
	{
	  sts = gen_filename( ldhses, plcpgm, filename);
	  if ( EVEN(sts)) return sts;
	}
	else
	  strcpy( filename, output);

	file = fopen( filename, "w");
	if ( file == 0)
	  return FOE__NOFILE;

	sts = ldh_ObjidToName ( ldhses, plcpgm,
		ldh_eName_Hierarchy, hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;

	fprintf( file, "!\n!	Proview/R Export\n");
	fprintf( file, "!	PlcPgm %s\n!\n", hiername);

	sts = ldh_GetParent ( ldhses, plcpgm, &parent);
	if ( EVEN(sts)) return sts;
	sts = ldh_ObjidToName(
		ldhses, parent, ldh_eName_Hierarchy,
		parent_name, sizeof( parent_name), &size);
	if( EVEN(sts)) return sts;
	sts = ldh_ObjidToName(
		ldhses, plcpgm, ldh_eName_Object,
		plcpgm_name, sizeof( plcpgm_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( file, "object PlcPgm /name=%s /parent=%s",
		plcpgm_name,
		parent_name);

	/* Print the windows */
	sts = ldh_GetChild( ldhses, plcpgm, &window);
	while ( ODD(sts) )
	{
	  if ( ODD( ldh_GetObjectBuffer(ldhses, window, "DevBody",
		      "PlcWindow", (pwr_eClass *) &cid, &windbuffer, &size) ))
	  {
	    free( windbuffer);
	    sts = ldh_GetObjectClass ( ldhses, window, &cid);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_ObjidToName ( ldhses, cdh_ClassIdToObjid( cid), 
		ldh_eName_Object, class_name, sizeof(class_name), &size);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_ObjidToName ( ldhses, window, ldh_eName_Hierarchy, 
		hiername, sizeof(hiername), &size);
	    if ( EVEN(sts)) return sts;

	    fprintf( file, "/%s=%s", class_name, hiername);
	  }
	  sts = ldh_GetNextSibling( ldhses, window, &window);
	}

	/* Print reset object */
	sts = ldh_GetObjectPar( ldhses,
			plcpgm,
			"DevBody",
			"ResetObject",
			(char **) &resobjdid_ptr, &size);
	if ( EVEN(sts)) return sts;

	sts = ldh_ObjidToName ( ldhses, *resobjdid_ptr, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	if ( ODD(sts))
	  fprintf( file, " /ResetObject=%s.ActualValue", hiername);
	free( (char *) resobjdid_ptr);

	/* Print reset object */
	sts = ldh_GetObjectPar( ldhses,
			plcpgm,
			"DevBody",
			"ScanTime",
			(char **) &scantime_ptr, &size);
	if ( EVEN(sts)) return sts;
	fprintf( file, " /ScanTime=%g", *scantime_ptr);
	free( (char *) scantime_ptr);

	fclose( file);

	/* A system call to allow the user to do something */
	sprintf( cmd, "PWR_EXPORT PLCPGM %s", filename);
	system( cmd);
	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		exp_window()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Export a plc window.
*
**************************************************************************/

pwr_tStatus exp_window (
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  char			*window_str,
  char			*output
)
{
	pwr_tObjid		window; 
	vldh_t_wind		wind;
	pwr_tStatus		sts;
	int			size;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		node;
	int			j;
	char			filename[80];
	exp_genctx		genctx;
	pwr_tString80		hiername;
	pwr_tString80		cmd;
	pwr_tString80		class_name;
	pwr_tString80		parent_name;
	pwr_tString80		plcpgm_name;
	pwr_tClassId		cid;
	pwr_tObjid		parent;
	unsigned long		syntax_errcount;
	unsigned long		syntax_warncount;

	/* Get the objdid for the window */
	sts = ldh_NameToObjid( ldhses, &window, window_str);
	if ( EVEN(sts)) return FOE__PLCNOTFOUND;

	sts = exp_vldh_load_window( ldhwb, ldhses, window, &wind, 
		ldh_eAccess_ReadOnly);
	if ( EVEN(sts)) return sts;

	sts = gcg_plcwindow_compile( wind, 0, &syntax_errcount, 
		&syntax_warncount, 0, 0);
	if ( EVEN(sts)) return sts;

	/* Create a context for the window */
	genctx = (exp_genctx) calloc( 1, sizeof( exp_t_genctx));
	genctx->ldhses = wind->hw.ldhses;
	genctx->wind = wind;

	if ( !output)
	{
	  sts = gen_filename( genctx->ldhses, wind->lw.oid,
			filename);
	  if ( EVEN(sts)) return sts;
	}
	else
	  strcpy( filename, output);

	genctx->file = fopen( filename, "w");
	if ( genctx->file == 0)
	  return FOE__NOFILE;

	sts = ldh_ObjidToName ( genctx->ldhses, wind->lw.oid,
		ldh_eName_Hierarchy, hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, "!\n!	Proview/R Export\n");
	fprintf( genctx->file, "!	Window %s\n!\n", hiername);

	sts = ldh_GetParent ( ldhses, window, &parent);
	if ( EVEN(sts)) return sts;
	sts = ldh_ObjidToName(
		ldhses, parent, ldh_eName_Hierarchy,
		parent_name, sizeof( parent_name), &size);
	if( EVEN(sts)) return sts;
	sts = ldh_ObjidToName(
		ldhses, window, ldh_eName_Object,
		hiername, sizeof( hiername), &size);
	if( EVEN(sts)) return sts;
	sts = ldh_GetObjectClass ( ldhses, window, &cid);
	if ( EVEN(sts)) return sts;
	sts = ldh_ObjidToName ( ldhses, cdh_ClassIdToObjid( cid), 
		ldh_eName_Object, class_name, sizeof(class_name), &size);
	if ( EVEN(sts)) return sts;
	sts = ldh_ObjidToName ( ldhses, 
		((vldh_t_plc)(wind->hw.plc))->lp.oid,
		ldh_eName_Hierarchy, 
		plcpgm_name, sizeof(plcpgm_name), &size);
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s /parent=%s /plcpgm=%s\n",
		class_name,
		hiername,
		parent_name,
		plcpgm_name);

	/* Get all nodes this window */
	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	sts = exo_wind_exec(wind);
	if ( EVEN(sts)) return sts;

	gcg_executeorder_nodes( node_count, nodelist);

        for ( j = 0; j < (int)node_count; j++)
	{
	  node = *(nodelist + j);
	  sts = exp_node_gen( genctx, node);
	  if ( EVEN(sts)) return sts; 
	}
	if ( node_count > 0)
	  free((char *) nodelist);

	fclose( genctx->file);

	free((char *) genctx);

	/* A system call to allow the user to do something */
	sprintf( cmd, "PWR_EXPORT WINDOW %s", filename);
	system( cmd);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		exp_node_gen()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates code for an object.
*
**************************************************************************/

static pwr_tStatus exp_node_gen (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	pwr_tStatus		sts;
	int 			size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	int			compmethod;

	/* Get comp method for this node */
	sts = ldh_GetClassBody( genctx->ldhses, node->ln.cid, 
		"GraphPlcNode", &bodyclass, (char **) &graphbody, &size);
	if ( EVEN(sts)) return sts;

	compmethod = graphbody->compmethod;

	if ( 
	     node->ln.cid == pwr_cClass_and
	 ||  node->ln.cid == pwr_cClass_or
	 ||  node->ln.cid == pwr_cClass_or
	 ||  node->ln.cid == pwr_cClass_csub
	    ) 
	{
	  sts = gen_m2( genctx, node);
	}
	else if (
	    node->ln.cid == pwr_cClass_stodo
	 || node->ln.cid == pwr_cClass_setdo
	 || node->ln.cid == pwr_cClass_resdo
	 || node->ln.cid == pwr_cClass_stodv
	 || node->ln.cid == pwr_cClass_setdv
	 || node->ln.cid == pwr_cClass_resdv
	 || node->ln.cid == pwr_cClass_stoav
	 || node->ln.cid == pwr_cClass_cstoav
	 || node->ln.cid == pwr_cClass_stoao
	 || node->ln.cid == pwr_cClass_cstoao
	 || node->ln.cid == pwr_cClass_stoai
	 || node->ln.cid == pwr_cClass_cstoai
	    ) 
	{
	  sts = gen_m3( genctx, node);
	}
	else if (
	    node->ln.cid == pwr_cClass_stodp
	 || node->ln.cid == pwr_cClass_setdp
	 || node->ln.cid == pwr_cClass_resdp
	 || node->ln.cid == pwr_cClass_stoap
	 || node->ln.cid == pwr_cClass_cstoap
	 || node->ln.cid == pwr_cClass_StoIp
	 || node->ln.cid == pwr_cClass_CStoIp
	 || node->ln.cid == pwr_cClass_stopi
	    ) 
	{
	  sts = gen_m4( genctx, node);
	}
	else if ( 
	     node->ln.cid == pwr_cClass_timer
	 ||  node->ln.cid == pwr_cClass_wait
	 ||  node->ln.cid == pwr_cClass_pulse
	    ) 
	{
	  sts = gen_m5( genctx, node);
	}
	else if (
	    node->ln.cid == pwr_cClass_initstep
	 || node->ln.cid == pwr_cClass_substep
	 || node->ln.cid == pwr_cClass_step
	    ) 
	{
	  sts = gen_m6( genctx, node);
	}
	else if ( 
	     node->ln.cid == pwr_cClass_trans
	    ) 
	{
	  sts = gen_m7( genctx, node);
	}
	else if ( 
	     node->ln.cid == pwr_cClass_order
	    ) 
	{
	  sts = gen_m8( genctx, node);
	}
	else if ( 
	     node->ln.cid == pwr_cClass_reset_so
	    ) 
	{
	  sts = gen_m9( genctx, node);
	}
	else if (
	    node->ln.cid==pwr_cClass_aarithm
	 || node->ln.cid==pwr_cClass_darithm
	 || node->ln.cid==pwr_cClass_filter
	 || node->ln.cid==pwr_cClass_maxmin
	 || node->ln.cid==pwr_cClass_ramp
	 || node->ln.cid==pwr_cClass_inc3p
	 || node->ln.cid==pwr_cClass_pos3p
	 || node->ln.cid==pwr_cClass_drive
	 || node->ln.cid==pwr_cClass_mvalve
	 || node->ln.cid==pwr_cClass_Backup
	 || node->ln.cid==pwr_cClass_ASup
	 || node->ln.cid==pwr_cClass_DSup
	 || node->ln.cid==pwr_cClass_Backup
	    )
	{
	  sts = gen_m10( genctx, node);
	}
	else if (
	    node->ln.cid == pwr_cClass_ssend
	 || node->ln.cid == pwr_cClass_ssbegin
	    ) 
	{
	  sts = gen_m11( genctx, node);
	}
	else if (
	    node->ln.cid == pwr_cClass_setcond
	    ) 
	{
	  sts = gen_m12( genctx, node);
	}
	else if ( compmethod == 4)
	  sts = gen_m10( genctx, node);
	else if ( compmethod == 5)
	  sts = gen_m2( genctx, node);
	else
	{
	  sts = FOE__SUCCESS;
	}
	return sts;
}	

/*************************************************************************
*
* Name:		gen_m2()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates code for an object.
*
**************************************************************************/

static pwr_tStatus gen_m2 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	pwr_tStatus	sts;
	int 		size;
	char		class_name[80];
	
	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_CONPAR, NULL,
			NULL);
	if ( EVEN(sts)) return sts;

	sts = gen_print_windows( genctx, node);
	if ( EVEN(sts)) return sts;
	fprintf( genctx->file, "\n");

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m3()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates code for an object with an objid that should be
*	printed as a qualifier together with the input parameters.
*
**************************************************************************/

static pwr_tStatus gen_m3 (
exp_genctx	genctx,
vldh_t_node	node
)
{
	pwr_tStatus		sts;
	int 			size;
	char			class_name[80];
	char			object_name[80];
	ldh_sParDef 		*bodydef;
	int 			rows;
	int			i;
	int			found;
	pwr_tObjid		*parvalue;
	char			*nocondef_ptr;
	union			{
		int		bo;
		float		fl;
				} nocondef[2];
	unsigned long		nocontype[2];

	nocondef[1].bo = 1;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* If the object is not connected the value in the
	   parameter should be written in the macro call */
	sts = ldh_GetObjectPar( genctx->ldhses,
			node->ln.oid, 
			"RtBody",
			"In",
			&nocondef_ptr, &size); 
	if ( EVEN(sts)) return sts;

	memcpy( &nocondef[0], nocondef_ptr, size);
	free( nocondef_ptr);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_ALLPAR, 
		(unsigned long *) &nocondef, nocontype);
	if ( EVEN(sts)) return sts;

	/* Get the first attribute in devbody of type pwr_tObjid */
	sts = ldh_GetObjectBodyDef( genctx->ldhses,
			node->ln.cid, "DevBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	found = 0;
	for ( i = 0; i < rows; i++)
	{
	  if ( bodydef[i].Par->Param.Info.Type == pwr_eType_Objid)
	  {
	    /* Get the parameter value */
	    sts = ldh_GetObjectPar( genctx->ldhses,
			node->ln.oid, 
			"DevBody",
			(bodydef[i].ParName),
			(char **) &parvalue, &size); 
	    if ( EVEN(sts)) return sts;
	    found = 1;
	    break;
	  }
	}
	if ( !found )
	{
	  free((char *) parvalue);	
	  free((char *) bodydef);	
	  return FOE__SUCCESS;
	}
	
	sts = exp_ObjdidToName( node, *parvalue, object_name,
			sizeof( object_name));
	if( EVEN(sts))
	{
	  free((char *) parvalue);
	  free((char *) bodydef);
	  return FOE__SUCCESS;
	}
	free((char *) parvalue);

	fprintf( genctx->file, "/%s=%s\n", (bodydef[i].ParName), object_name);
	free((char *) bodydef);	

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m4()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates StoDp, CStoDp, StoAp, StoIp, CStoAp, CStoIp
*
**************************************************************************/

static pwr_tStatus gen_m4 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	pwr_tStatus		sts;
	int 			size;
	char			class_name[80];
	char			object_name[80];
	ldh_sParDef 		*bodydef;
	int 			rows;
	int			i;
	int			found;
	pwr_tObjid		*parvalue;
	char			*parameter;
	char			*nocondef_ptr;
	union			{
		int		bo;
		float		fl;
				} nocondef[2];
	unsigned long		nocontype[2];

	nocondef[1].bo = 1;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* If the object is not connected the value in the
	   parameter should be written in the macro call */
	sts = ldh_GetObjectPar( genctx->ldhses,
			node->ln.oid, 
			"RtBody",
			"In",
			&nocondef_ptr, &size); 
	if ( EVEN(sts)) return sts;

	memcpy( &nocondef[0], nocondef_ptr, size);
	free( nocondef_ptr);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_ALLPAR, 
		(unsigned long *) &nocondef, nocontype);
	if ( EVEN(sts)) return sts;

	/* Get the first attribute in devbody of type pwr_tObjid */
	sts = ldh_GetObjectBodyDef( genctx->ldhses,
			node->ln.cid, "DevBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	found = 0;
	for ( i = 0; i < rows; i++)
	{
	  if ( bodydef[i].Par->Param.Info.Type == pwr_eType_Objid)
	  {
	    /* Get the parameter value */
	    sts = ldh_GetObjectPar( genctx->ldhses,
			node->ln.oid, 
			"DevBody",
			(bodydef[i].ParName),
			(char **) &parvalue, &size); 
	    if ( EVEN(sts)) return sts;
	    found = 1;
	    break;
	  }
	}
	if ( !found )
	{
	  free((char *) parvalue);	
	  free((char *) bodydef);	
	  return FOE__SUCCESS;
	}
	
	sts = exp_ObjdidToName( node, *parvalue, object_name,
			sizeof( object_name));
	if( EVEN(sts))
	{
	  free((char *) parvalue);	
	  free((char *) bodydef);	
	  return FOE__SUCCESS;
	}
	free((char *) parvalue);

	sts = ldh_GetObjectPar( genctx->ldhses,
			node->ln.oid, 
			"DevBody",
			"Parameter",
			&parameter, &size); 
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, "/%s=%s.%s\n", (bodydef[i].ParName), 
		object_name, parameter);
	free((char *) parameter);
	free((char *) bodydef);	

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m5()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates code for a timer and wait object.
*
**************************************************************************/

static pwr_tStatus gen_m5 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	pwr_tStatus	sts;
	int 		size;
	char		class_name[80];
	float		*time_ptr;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_CONPAR, NULL,
			NULL);
	if ( EVEN(sts)) return sts;

	/* Get the Do signal */
	sts = ldh_GetObjectPar( genctx->ldhses, node->ln.oid, "RtBody", 
			"TimerTime", (char **) &time_ptr, &size);
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, "/TimerTime=%f", *time_ptr);
	fprintf( genctx->file, "\n");

	free((char *) time_ptr);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m6()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for step or initstep object.
*
**************************************************************************/

static pwr_tStatus gen_m6 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext		ldhses;
	pwr_tClassId		cid;
	pwr_tObjid		*resobjdid_ptr;
	pwr_tStatus		sts;
	int			size;
	vldh_t_plc		plc;
	char			hiername[120];
	char			class_name[80];
	unsigned long		point;
	unsigned long		point_count;
	vldh_t_conpoint		*pointlist;
	vldh_t_node		next_node;
	unsigned long		next_point;
	int			k;
	unsigned long		par_inverted;

	ldhses = (node->hn.wind)->hw.ldhses;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Get the reset object from the plcobject */
	plc = (node->hn.wind)->hw.plc;
	sts = ldh_GetObjectPar( ldhses,
			plc->lp.oid,
			"DevBody",
			"ResetObject",
			(char **) &resobjdid_ptr, &size);
	if ( EVEN(sts)) return sts;

	sts = ldh_ObjidToName ( ldhses, *resobjdid_ptr, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;
	free((char *) resobjdid_ptr);

	fprintf( genctx->file, " /ResetObject=%s.ActualValue", hiername);

	sts = gen_print_windows( genctx, node);
	if ( EVEN(sts)) return sts;
	fprintf( genctx->file, "\n");

	/* Check if there is any order connected to the outputpin */
	sts = gcg_get_point( node, 2, &point, &par_inverted);
	if ( ODD( sts))
	{
	  genctx->step_gen = TRUE;
	  gcg_get_conpoint_nodes( node, point, 
			&point_count, &pointlist, 
			GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);

	  for ( k = 1; k < (int)point_count; k++)
	  {
	    next_node = (pointlist + k)->node;
	    next_point = (pointlist + k)->conpoint;
	    /* Check class of connected nodes */
	    sts = ldh_GetObjectClass( ldhses, next_node->ln.oid, &cid);
	    if (EVEN(sts)) return sts;

	    if ((cid == pwr_cClass_order) && 
		 ( next_point == 0 ))
	    {
	      /* compile this nodes here */
	      sts = exp_node_gen( genctx, next_node);
	      if ( EVEN(sts)) return sts;
	    }
	  }
	  if ( point_count > 0) free((char *)  pointlist);
	  genctx->step_gen = FALSE;
	}

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m7()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for a trans object.
*
**************************************************************************/

static pwr_tStatus gen_m7 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext		ldhses;
	pwr_tClassId		cid;
	char			hiername[120];
	char			class_name[80];
	unsigned long		point;
	unsigned long		par_inverted;
	pwr_tStatus		sts;
	int			size;
	unsigned long		point_count;
	vldh_t_conpoint		*pointlist;
	vldh_t_node		next_node;
	unsigned long		next_point;
	int			k;
	pwr_sAttrRef		output_attrref;
	int			output_type;
	char			output_prefix;
	char			output_par[20];
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int			pincond_found;
	int			first;

	ldhses = (node->hn.wind)->hw.ldhses;  

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Print the step input list */
	fprintf( genctx->file, " /InStep=(");

	/* Check if there is any step connected to the inputpin */
	sts = gcg_get_point( node, 0, &point, &par_inverted);
	if ( EVEN( sts)) return sts;

	gcg_get_conpoint_nodes( node, point, &point_count, &pointlist,
		GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	if ( point_count > 1 )
	{
	  first = 1;
	  for ( k = 1; k < (int)point_count; k++)
	  {
	    next_node = (pointlist + k)->node;
	    next_point = (pointlist + k)->conpoint;
	    /* Check class of connected nodes */
	    sts = ldh_GetObjectClass( ldhses, next_node->ln.oid, &cid);
	    if (EVEN(sts)) return sts;

	    if ( 
		((cid == pwr_cClass_step) && 
			( next_point == 2 )) ||
		((cid == pwr_cClass_initstep) && 
			( next_point == 2 )) ||
		((cid == pwr_cClass_substep) && 
			( next_point == 2 )) ||
		((cid == pwr_cClass_ssbegin) && 
			( next_point == 1 )) )
	    {
	      /* Print in the input list */
	      sts = exp_ObjdidToName( node, next_node->ln.oid, hiername,
			sizeof( hiername));
	      if( EVEN(sts)) return sts;

	      if ( !first)
	        fprintf( genctx->file, ",");
	      fprintf( genctx->file, "%s", hiername);
	      first = 0;
	    }
	  }
	}
	if ( point_count > 0) free((char *) pointlist);

	/* Print the step output list */
	fprintf( genctx->file, ") /OutStep=(");

	/* Check if there is any step connected to the outputpin */
	sts = gcg_get_point( node, 1, &point, &par_inverted);
	if ( EVEN( sts)) return sts;

	gcg_get_conpoint_nodes( node, point, &point_count, &pointlist, 
		GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	if ( point_count > 1 )
	{
	  first = 1;
	  for ( k = 1; k < (int)point_count; k++)
	  {
	    next_node = (pointlist + k)->node;
	    next_point = (pointlist + k)->conpoint;
	    /* Check class of connected nodes */
	    sts = ldh_GetObjectClass( ldhses, next_node->ln.oid, &cid);
	    if (EVEN(sts)) return sts;

	    if ( 
		((cid == pwr_cClass_step) && 
			( next_point == 0 )) ||
		((cid == pwr_cClass_initstep) && 
			( next_point == 0 )) ||
		((cid == pwr_cClass_substep) && 
			( next_point == 0 )) ||
		((cid == pwr_cClass_ssend) && 
			( next_point == 0 )) )
	    {
	      /* Print in the output list */
	      sts = exp_ObjdidToName( node, next_node->ln.oid, hiername,
			sizeof( hiername));
	      if( EVEN(sts)) return sts;

	      if ( !first)
	        fprintf( genctx->file, ",");
	      fprintf( genctx->file, "%s", hiername);
	      first = 0;
	    }
	  }
	}
	if ( point_count > 0) free((char *) pointlist);
	fprintf( genctx->file, ")");

	/* Check if the condition pin is visible */
	sts = gcg_get_inputpoint( node, 2, &point, &par_inverted);
	if ( ODD( sts))
	{
	  /* Look for an output connected to this point */
	  sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	  if ( EVEN(sts)) return sts;

	  pincond_found = 0;
	  if ( output_count > 0 )
	  {
	    pincond_found = 1;

	    sts = gcg_get_outputstring( &gcgctx_dum, output_node, &output_bodydef, 
			&output_attrref, &output_type, &output_prefix, output_par);
	    if ( sts == GSX__NEXTPAR ) return sts;
	    if ( EVEN(sts)) return sts;
	    
	      /* Print the condition input */
	      sts = exp_ObjdidToName( node, output_attrref.Objid, hiername,
			sizeof( hiername));
	      if( EVEN(sts)) return sts;

	      fprintf( genctx->file, " /Cond=*%s.%s", hiername, output_par);
	  }
	}

	sts = gen_print_windows( genctx, node);
	if ( EVEN(sts)) return sts;
	fprintf( genctx->file, "\n");

	return FOE__SUCCESS;

}

/*************************************************************************
*
* Name:		gen_m8()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for a order object.
*
**************************************************************************/

static pwr_tStatus gen_m8 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext		ldhses;
	pwr_tClassId		cid;
	char			hiername[120];
	char			class_name[80];
	unsigned long		point;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		*bodydef;
	ldh_sParDef 		output_bodydef;
	pwr_tStatus		sts;
	int 			rows;
	int			i;
	int			output_found;
	int			size;
	pwr_sAttrRef		cond_output_attrref;
	int			cond_output_type;
	char			cond_output_par[20];
	unsigned long		cond_par_inverted;
	char			output_prefix;
	pwr_tObjid		step_objdid;
	pwr_tObjid		next_objdid;
	int			found;
	int			pincond_found;
	char			*parvalue;
	pwr_tFloat32		*timevalue;
	float			time;
	pwr_tClassId		suborderclass[6];
	unsigned long		subordercount;
	unsigned long		ldhsubordercount;
	vldh_t_plc		plc;
	pwr_tObjid		*resobjdid_ptr;

	if ( !genctx->step_gen)
	  return FOE__SUCCESS;
	ldhses = (node->hn.wind)->hw.ldhses;  

	/* Get the step connected to conpoint 0 */
	sts = gcg_get_output( node, 0, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	if ( EVEN(sts)) return sts;

	if ( output_count > 0 )
	{
	  output_found = 1;

	  /* Check that is it a step */
	  sts = ldh_GetObjectClass( ldhses, output_node->ln.oid, &cid);
	  if (EVEN(sts)) return sts;

	  if ( !((cid == pwr_cClass_step) ||
	         (cid == pwr_cClass_substep) ||
	         (cid == pwr_cClass_ssbegin) ||
	         (cid == pwr_cClass_ssend) ||
	         (cid == pwr_cClass_initstep)))
	  {
	    return GSX__NEXTNODE;
	  }
	}
	else
	{
	  return GSX__NEXTNODE;
	}
	
	step_objdid = output_node->ln.oid;

	/* Check if the condition pin is visible */
	sts = gcg_get_inputpoint( node, 1, &point, &cond_par_inverted);
	pincond_found = 0;
	if ( ODD( sts))
	{
	  /* Look for an output connected to this point */
	  sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	  if ( EVEN(sts)) return sts;

	  if ( output_count > 0 )
	  {
	    pincond_found = 1;

	    sts = gcg_get_outputstring( &gcgctx_dum, output_node, &output_bodydef, 
			&cond_output_attrref, &cond_output_type, &output_prefix, cond_output_par);
	    if ( sts == GSX__NEXTPAR ) return sts;
	    if ( EVEN(sts)) return sts;
	  }
	}

	/* Check the attributes in the parent order node */
	sts = ldh_GetObjectBodyDef((node->hn.wind)->hw.ldhses,
			node->ln.cid, "DevBody", 1, 
			&bodydef, &rows);

	if ( EVEN(sts) ) return sts;

	subordercount = 0;
	for ( i = 0; i < rows - 1; i += 2)
	{
	  /* Get the parameter value */
	  sts = ldh_GetObjectPar( (node->hn.wind)->hw.ldhses,
			node->ln.oid,
			"DevBody",
			(bodydef[i].ParName),
			&parvalue, &size);
	  if ( EVEN(sts)) return sts;

	  if ( *parvalue == 'S' )
	  {
	    suborderclass[subordercount] =
			pwr_cClass_sorder;
	    subordercount++;
	  }
	  else if ( *parvalue == 'L' )
	  {
	    suborderclass[subordercount] =
			pwr_cClass_lorder;
	    subordercount++;
	  }
	  else if ( *parvalue == 'C' )
	  {
	    suborderclass[subordercount] =
			pwr_cClass_corder;
	    subordercount++;
	  }
	  else if ( *parvalue == 'D' )
	  {
	    suborderclass[subordercount] =
			pwr_cClass_dorder;
	    subordercount++;
	  }
	  else if ( *parvalue == 'P' )
	  {
	    suborderclass[subordercount] =
			pwr_cClass_porder;
	    subordercount++;
	  }
	  free( parvalue);

	}
	free((char *) bodydef);
	
	/* Get the objdid for order children of this node */
	found = 0;
	ldhsubordercount = 0;
	sts = ldh_GetChild( ldhses, node->ln.oid, &next_objdid);
	while ( ODD(sts) )
	{
	  /* Find out if this is a suborder */
	  sts = ldh_GetObjectClass( ldhses, next_objdid, &cid);
	  if (EVEN(sts)) return sts;

	  if ( (cid == pwr_cClass_sorder) ||
	       (cid == pwr_cClass_dorder) ||
	       (cid == pwr_cClass_lorder) ||
	       (cid == pwr_cClass_corder) ||
	       (cid == pwr_cClass_porder) )
	  {

	    /* Print class and name */
	    sts = ldh_ObjidToName(
			genctx->ldhses, cdh_ClassIdToObjid( cid), 
			ldh_eName_Object, class_name, sizeof( class_name), &size);
	    if( EVEN(sts)) return sts;

	    /* Print class and name */
	    sts = ldh_ObjidToName(
			genctx->ldhses, next_objdid, ldh_eName_Object,
			hiername, sizeof( hiername), &size);
	    if( EVEN(sts)) return sts;

	    fprintf( genctx->file, "object %s /name=%s /order=%s",
			class_name,
			hiername,
			node->hn.name);

	    /* Print Step input */
	    sts = exp_ObjdidToName( node, step_objdid, hiername,
			sizeof( hiername));
	    if( EVEN(sts)) return sts;

	    fprintf( genctx->file, " /Step=*%s", hiername);

	    if ( cid != suborderclass[ldhsubordercount] )
	    {
	      return GSX__ORDERMISM;
	    }
	    if ( cid == pwr_cClass_sorder)
	    {
	      /* Get the reset object from the plcobject */
	      plc = (node->hn.wind)->hw.plc;
	      sts = ldh_GetObjectPar( ldhses,
			plc->lp.oid,
			"DevBody",
			"ResetObject",
			(char **) &resobjdid_ptr, &size);
	      if ( EVEN(sts)) return sts;

	      sts = ldh_ObjidToName ( ldhses, *resobjdid_ptr, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	      if ( EVEN(sts)) return sts;
	      free((char *) resobjdid_ptr);

	      fprintf( genctx->file, " /ResetObject=%s.ActualValue", hiername);
	    }
	    else if ( cid == pwr_cClass_lorder)
	    {
	      /* Print the time */
	      sts = ldh_GetObjectPar( ldhses, 
			next_objdid,
			"RtBody",
			"TimerTime",
			(char **) &timevalue, &size);
	      if ( EVEN(sts)) return sts;
	      time = *timevalue;
	      free((char *) timevalue);
	      fprintf( genctx->file, " /Time=%g", time);
	    }
	    else if ( cid == pwr_cClass_dorder)
	    {
	      /* Print the time */
	      sts = ldh_GetObjectPar( ldhses, 
			next_objdid,
			"RtBody",
			"TimerTime",
			(char **) &timevalue, &size);
	      if ( EVEN(sts)) return sts;
	      time = *timevalue;
	      free((char *) timevalue);
	      fprintf( genctx->file, " /Time=%g", time);
	    }
	    else if ( cid == pwr_cClass_porder)
	    {
	    }
	    else if ( cid == pwr_cClass_corder)
	    {
	      if ( pincond_found)
	      { 
	        sts = exp_ObjdidToName( node, cond_output_attrref.Objid, hiername,
			sizeof( hiername));
	        if( EVEN(sts)) return sts;

	        if ( cond_par_inverted)
	          fprintf( genctx->file, " /Cond=\"!%s.%s\"",
			hiername,
			cond_output_par);
	        else
	          fprintf( genctx->file, " /Cond=%s.%s",
			hiername,
			cond_output_par);
	      }
	    }
	    fprintf( genctx->file, "\n");
	    ldhsubordercount++;
	    /* Examine only the first */
/*	    break;
*/
	  }
	  sts = ldh_GetNextSibling( ldhses, next_objdid, &next_objdid);
	}

	if ( ldhsubordercount != subordercount )
	  return GSX__ORDERMISM;

/*	if ( subordercount == 0) */
	{
	  /* Print class and name */
	  sts = ldh_ObjidToName(
			genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
			ldh_eName_Object, class_name, sizeof( class_name), 
			&size);
	  if( EVEN(sts)) return sts;

	  fprintf( genctx->file, "object %s /name=%s",
			class_name,
			node->hn.name);

	  /* Print Step input */
	  sts = exp_ObjdidToName( node, step_objdid, hiername,
			sizeof( hiername));
	  if( EVEN(sts)) return sts;

	  fprintf( genctx->file, " /Step=*%s", hiername);
	}

	sts = gen_print_windows( genctx, node);
	if ( EVEN(sts)) return sts;
	fprintf( genctx->file, "\n");

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m9()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for a reset_so object.
*
**************************************************************************/

static pwr_tStatus gen_m9 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext	ldhses;
	pwr_tClassId	cid;
	char		hiername[120];
	char		class_name[80];
	pwr_tStatus	sts;
	int		size;
	pwr_tObjid	refobjdid;
	pwr_tObjid	*refobjdid_ptr;

	ldhses = (node->hn.wind)->hw.ldhses; 
	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_CONPAR, NULL,
			NULL);
	if ( EVEN(sts)) return sts;

	/* Get the objdid of the referenced io object stored in the
	  first parameter devbody */

	sts = ldh_GetObjectPar( ldhses,
			node->ln.oid, 
			"DevBody",
			"OrderObject",
			(char **) &refobjdid_ptr, &size); 
	if ( EVEN(sts)) return sts;

	refobjdid = *refobjdid_ptr;
	free((char *) refobjdid_ptr);

	/* Check that this is objdid of an existing object */
	sts = ldh_GetObjectClass(
			(node->hn.wind)->hw.ldhses, 
			refobjdid,
			&cid);
	if ( EVEN(sts)) 
	{
	  return GSX__NEXTNODE;
	}

	sts = exp_ObjdidToName( node, refobjdid, hiername,
			sizeof( hiername));
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, " /OrderObject=%s\n", hiername);

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m2()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* exp_genctx	genctx		I	expgen context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Generates code for an object. If the input is not connected
*	but a value i assigned the value will be printed.
*
**************************************************************************/

static pwr_tStatus gen_m10  (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	int 		size;
	pwr_tStatus	sts;
	char		class_name[80];
	ldh_tSesContext	ldhses;
	
	ldhses = (node->hn.wind)->hw.ldhses;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Print the connected inputs */
	sts = gen_print_inputs( genctx, node, GCG_PRINT_VALUE, NULL,
			NULL);
	if ( EVEN(sts)) return sts;

	sts = gen_print_interns( genctx, node);

	fprintf( genctx->file, "\n");

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_m11()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for ssbegin or ssend object.
*
**************************************************************************/

static pwr_tStatus gen_m11 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext		ldhses;
	pwr_tStatus		sts;
	int			size;
	vldh_t_plc		plc;
	pwr_tObjid		*resobjdid_ptr;
	char			hiername[120];
	char			class_name[80];
	pwr_tObjid		parent;

	ldhses = (node->hn.wind)->hw.ldhses;
	parent = (node->hn.wind)->lw.poid;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	/* Get the reset object from the plcobject */
	plc = (node->hn.wind)->hw.plc;
	sts = ldh_GetObjectPar( ldhses,
			plc->lp.oid,
			"DevBody",
			"ResetObject",
			(char **) &resobjdid_ptr, &size);
	if ( EVEN(sts)) return sts;

	sts = ldh_ObjidToName ( ldhses, *resobjdid_ptr, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;
	free((char *) resobjdid_ptr);

	fprintf( genctx->file, " /ResetObject=%s.ActualValue", hiername);

	sts = ldh_ObjidToName ( ldhses, parent, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, " /Parent=%s\n", hiername);

	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		gen_m12()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Method for a setcond object.
*
**************************************************************************/

static pwr_tStatus gen_m12 (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext		ldhses;
	pwr_tStatus		sts;
	int			size;
	char			hiername[120];
	char			class_name[80];
	pwr_tObjid		parent;

	ldhses = (node->hn.wind)->hw.ldhses;
	parent = (node->hn.wind)->lw.poid;

	/* Print class and name */
	sts = ldh_ObjidToName( 
		genctx->ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	if( EVEN(sts)) return sts;

	fprintf( genctx->file, "object %s /name=%s",
		class_name,
		node->hn.name);

	sts = gen_print_inputs( genctx, node, GCG_PRINT_CONPAR, NULL,
			NULL);
	if ( EVEN(sts)) return sts;

	sts = ldh_ObjidToName ( ldhses, parent, ldh_eName_Hierarchy,
		hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;

	fprintf( genctx->file, " /Parent=%s\n", hiername);

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_print_inputs()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Gets and prints all inputs.
*	This routine is used by some gcgmethods to get and print
*	all the connected objects and parameters in the printed function call.
*	Connected outputs for all the inputs are looked for and if 
*	they are found they are printed
*	If one input is not found printtype tells wether to jump to
*	the next on without printing anything, or to print 
*	- 0 if nocondef is NULL or
*	- the value specified by nocondef.
*	nocondef makes it possible to define an array that contains a default
*	value for each input parameter. nocondef is a pointer to this array and
*	nocontype is a pointer to an array with the types of the values in
*	nocondef.
*	Ex declaration of nocondef and nocontype:
* 	unsigned long		nocondef[2] = { 0, TRUE};
*	unsigned long		nocontype[2] = { GCG_BOOLEAN, GCG_BOOLEAN };
*
**************************************************************************/

static pwr_tStatus gen_print_inputs (
  exp_genctx	genctx,
  vldh_t_node	node,
  unsigned long	printtype,
  unsigned long	*nocondef,
  unsigned long	*nocontype
)
{
	ldh_sParDef 		*bodydef;
	pwr_tStatus		sts;
	int 			rows;
	int			size;
	ldh_tSesContext		ldhses;
	unsigned long		point;
	unsigned long		par_inverted;
	vldh_t_node		output_node;
	unsigned long		output_count;
	unsigned long		output_point;
	ldh_sParDef 		output_bodydef;
	int			i;
	int			output_found;
	pwr_sAttrRef		output_attrref;
	int			output_type;
	char			output_prefix;
	char			output_par[20];
	int			input_count, inputscon_count;
	char			output_name[80];
	char			*buffer;
	int			nocondef_val;

	ldhses = (node->hn.wind)->hw.ldhses;

	/* Get the runtime parameters for this class */
	sts = ldh_GetObjectBodyDef((node->hn.wind)->hw.ldhses, 
			node->ln.cid, "RtBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	i = 0;
	input_count = 0;
	inputscon_count = 0;
	while( (bodydef[i].ParClass == pwr_eClass_Input) &&
		(i < rows))
	{
	  /* Get the point for this parameter if there is one */
	  output_found = 0;
	  sts = gcg_get_inputpoint( node, i, &point, &par_inverted);
	  if ( ODD( sts))
	  {
	    /* Look for an output connected to this point */
	    input_count++;
	    sts = gcg_get_output( node, point, &output_count, &output_node,
			&output_point, &output_bodydef, 
			GOEN_CON_SIGNAL | GOEN_CON_OUTPUTTOINPUT);
	    if ( EVEN(sts)) return sts;

	    if ( output_count > 0)
	    {	
	      output_found = 1;
	      inputscon_count++;
	      if ( output_count > 1) 
	   	return GSX__CONOUTPUT;

	      sts = gcg_get_outputstring( &gcgctx_dum, output_node, &output_bodydef, 
			&output_attrref, &output_type, &output_prefix, output_par);
	      if ( sts == GSX__NEXTPAR ) 
	      {
	        i++;
	        continue;
	      }
	      if ( EVEN(sts)) return sts;

	      fprintf( genctx->file, "/%s=",
			bodydef[i].ParName);
	      if ( par_inverted ) 
	        fprintf( genctx->file, "\"!\"");

	      sts = exp_ObjdidToName( node, output_attrref.Objid, output_name,
			sizeof( output_name));
	      if( EVEN(sts)) return sts;

	      fprintf( genctx->file, "*%s.%s", 
			output_name,
			output_par);
	    }
	  }
	  if ( !output_found )
	  {
	    if ( printtype == GCG_PRINT_ALLPAR )
	    {
	      /* The point is not connected and not visible */
	      fprintf( genctx->file, "/%s=", bodydef[i].ParName);
	      if ( nocondef == NULL)
	      {
	        fprintf( genctx->file, "0");
	      }
	      else
	      { 
	        switch (bodydef[i].Par->Param.Info.Type)
	        {
	          case pwr_eType_Boolean:
	            nocondef_val = *(pwr_tBoolean *)(nocondef + i); 
	            fprintf( genctx->file, 
		 	"%d",
			nocondef_val);
	            break;
	          case pwr_eType_Int32:
	          case pwr_eType_UInt32:
	            fprintf( genctx->file, 
		 	"%d",
			*(int *)(nocondef + i));
	            break;
	          case pwr_eType_Float32:
	            fprintf( genctx->file, 
		 	"%f",
			*(float*) (nocondef + i));
	            break;
                  default:
                    ;
	        }
	      }
	    }
	    else if ( printtype == GCG_PRINT_VALUE )
	    {
	      /* Get the parameter value */
	      sts = ldh_GetObjectPar( ldhses,
			node->ln.oid,
			"RtBody",
			bodydef[i].ParName,
			&buffer, &size);
	      if ( EVEN(sts)) return sts;

	      switch (bodydef[i].Par->Param.Info.Type)
	      {
	        case pwr_eType_Float32:
	          if ( !(*(pwr_tFloat32 *)(buffer) < FLT_EPSILON && 
	               *(pwr_tFloat32 *)(buffer) > -FLT_EPSILON )) 
	   	  {
	            fprintf( genctx->file, "/%s=%g", bodydef[i].ParName,
				*(pwr_tFloat32 *)(buffer));
	 	  }
		  break;
	        case pwr_eType_Boolean:
	          if ( *(pwr_tBoolean *)(buffer))
	   	  {
	            fprintf( genctx->file, "/%s=%d", bodydef[i].ParName,
				*(pwr_tBoolean *)(buffer));
	 	  }
		  break;
                default:
                  ;
	      }
	      free( buffer);
	    }
	  }
	  i++;
	}
	if ( (input_count == 0) || (inputscon_count == 0))
	{
	  /* No inputs in the graphics or no inputs connected */
	  if ( printtype == GCG_PRINT_CONPAR )
	  {
/***
	    fprintf( genctx->file, "/%s=", bodydef[i].ParName);
	    if ( nocondef == NULL)
	    {
	      fprintf( genctx->file, "0");
  	    }
	    else
	    {
	      if ( *(nocontype + i) == GCG_BOOLEAN )
	      {
	        fprintf( genctx->file, 
		 	"%d",
			*(nocondef + i));
	      }
	      else
	      {
	        fprintf( genctx->file, 
		 	"%f",
			*(float*) (nocondef + i));
	      }
	    }
*******/
	  }	  
	}

	free((char *) bodydef);	
	return GSX__SUCCESS;
}



/*************************************************************************
*
* Name:		exp_ObjdidToName()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Get object name from objid. If the objekt is inside the
*		window just return the last segment of the name, else
*		return the whole hierarchy name.
*
**************************************************************************/
static pwr_tStatus exp_ObjdidToName (
  vldh_t_node node,
  pwr_tObjid output_objdid, 
  char *output_name,
  int size
)
{
	pwr_tStatus	sts;
	int 		outsize;
	vldh_t_wind	wind;
	char		wind_name[80];
	char		object_name[80];
	char		*s;

	wind = node->hn.wind;
	sts = ldh_ObjidToName(
		wind->hw.ldhses, output_objdid,
		ldh_eName_Hierarchy,
		object_name, size, &outsize);
	if ( EVEN(sts)) return sts;
	
	sts = ldh_ObjidToName(
		wind->hw.ldhses, wind->lw.oid,
		ldh_eName_Hierarchy,
		wind_name, sizeof( wind_name), &outsize);
	if ( EVEN(sts)) return sts;
	
	if ( strncmp( wind_name, object_name, strlen( wind_name)) == 0)
	{
	  s = object_name + strlen( wind_name)+ 1;
	  strcpy( output_name, s);
	}
	else
	  strcpy( output_name, object_name);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_print_interns()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Print all parameters of typ Intern or Param.
*
**************************************************************************/

static pwr_tStatus gen_print_interns (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext	ldhses;	
	pwr_tStatus	sts;
	int		size;
	int		i;
	int		j;
	int		k;
	ldh_sParDef 	*bodydef;
	int		rows;
	pwr_tObjid	templ;
	char		body[20];	
	char		parname[32];
	char		name[32];
	char		*object_par;
	char		*template_par;
	char		*object_element;
	char		*template_element;
	int		parsize;
	int		elements;
	char		timbuf[32];
	char		hiername[120];
	char		*hiername_ptr;

	ldhses = (node->hn.wind)->hw.ldhses;

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, cdh_ClassIdToObjid( node->ln.cid), 
		&templ);
	while ( ODD(sts) )
	{
	  sts = ldh_ObjidToName( 
		ldhses, templ, ldh_eName_Object,
		name, sizeof( name), &size);
	  if ( EVEN(sts)) return sts;
	  
	  if ( strcmp(name, "Template") == 0)
	    break;
	  sts = ldh_GetNextSibling( ldhses, templ, &templ);
	}
	if ( EVEN(sts)) return sts;
	
	for ( i = 0; i < 2; i++ )
	{
	  if ( i == 0)
	  {
	    strcpy( body, "RtBody");
	  }
	  else if ( i == 1)
	  {
	    strcpy( body, "DevBody");
	  }

    	  /* Get the runtime paramters for this class */
	  sts = ldh_GetObjectBodyDef(ldhses, node->ln.cid, body, 1, 
	  		&bodydef, &rows);
	  if ( EVEN(sts) ) continue;

	  for ( j = 0; j < rows; j++)
	  {
	    strcpy( parname, (bodydef[j].ParName));
	    /* Get the parameter value in the object */
	    sts = ldh_GetObjectPar( ldhses, node->ln.oid, body,   
			parname, &object_par, &parsize); 
	    if ( EVEN(sts)) return sts;

	    if ( !(bodydef[j].ParClass == pwr_eClass_Intern ||
	       bodydef[j].ParClass == pwr_eClass_Param))
	      continue;

	    if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_POINTER )
	      /* Don't write pointers */
	      continue;

	    if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_ARRAY )
	      elements = bodydef[j].Par->Output.Info.Elements;
	    else
	      elements = 1;

	    /* Get the parameter value in the template object */
	    sts = ldh_GetObjectPar( ldhses, templ, body,   
			parname, &template_par, &size); 
	    if ( EVEN(sts)) return sts;

	    object_element = object_par;
	    template_element = template_par;	    

	    /* Print only the first element */
	    for ( k = 0; k < 1; k++)
	    {
	      /* If the parameters in object and template are node equal, 
	         print them */
	      if ( memcmp( template_element, object_element, parsize/elements) != 0)
	      {
	        switch ( bodydef[j].Par->Output.Info.Type )
	        {
	          case pwr_eType_Boolean:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tBoolean *)(object_element));
	            break;
	          }
	          case pwr_eType_Float32:
	          {
	            fprintf( genctx->file, "/%s=%g", bodydef[j].ParName,
				*(pwr_tFloat32 *)(object_element));
	            break;
	          }
	          case pwr_eType_Float64:
	          {
	            fprintf( genctx->file, "/%s=%g", bodydef[j].ParName,
				*(pwr_tFloat64 *)(object_element));
	            break;
	          }
	          case pwr_eType_Char:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tChar *)(object_element));
	            break;
	          }
	          case pwr_eType_Int8:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tInt8 *)(object_element));
	            break;
	          }
	          case pwr_eType_Int16:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tInt16 *)(object_element));
	            break;
	          }
	          case pwr_eType_Int32:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tInt32 *)(object_element));
	            break;
	          }
	          case pwr_eType_UInt8:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tUInt8 *)(object_element));
	            break;
	          }
	          case pwr_eType_UInt16:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tUInt16 *)(object_element));
	            break;
	          }
	          case pwr_eType_UInt32:
	          {
	            fprintf( genctx->file, "/%s=%d", bodydef[j].ParName,
				*(pwr_tUInt32 *)(object_element));
	            break;
	          }
	          case pwr_eType_String:
	          {
	            fprintf( genctx->file, "/%s=\"%s\"", bodydef[j].ParName,
				object_element);
	            break;
	          }
	          case pwr_eType_Objid:
	          {
	            sts = exp_ObjdidToName( node, 
		    	*(pwr_tObjid *)(object_element), hiername, 
			sizeof( hiername));
	            if( ODD(sts))
	              fprintf( genctx->file, "/%s=%s", bodydef[j].ParName,
				hiername);
	            break;
	          }
	          case pwr_eType_AttrRef:
	          {
	            sts = ldh_AttrRefToName( ldhses,
		      (pwr_sAttrRef *) object_element, ldh_eName_Aref, &hiername_ptr, &size);
  	            if ( ODD(sts)) 
	              fprintf( genctx->file, "/%s=%s", bodydef[j].ParName,
				hiername_ptr);
	            break;
	          }
                  case pwr_eType_Time:
                  {
	            /* Convert time to ascii */	
                    time_AtoAscii((pwr_tTime *)object_element, 
                                        time_eFormat_DateAndTime, 
                                        timbuf, sizeof(timbuf));
                    timbuf[20] = '\0'; 
		    fprintf( genctx->file, "/%s=%s", bodydef[j].ParName,
				timbuf);
	            break;
                  }
                  default:
                    ;
	        }
	      }
   	      object_element += parsize / elements;
	      template_element += parsize / elements;
   	    }
	    free( object_par);
	    free( template_par);
	  }
	  free((char *) bodydef);	
	}


	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gen_print_windows()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Print all children of class window.
*
**************************************************************************/

static pwr_tStatus gen_print_windows (
  exp_genctx	genctx,
  vldh_t_node	node
)
{
	ldh_tSesContext	ldhses;
	pwr_tObjid	window;
	pwr_tStatus	sts;
	int		size;
	char		*windbuffer;
	pwr_tClassId	cid;
	pwr_tString80	class_name;
	pwr_tString80	hiername;

	ldhses = (node->hn.wind)->hw.ldhses;

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, node->ln.oid, &window);
	while ( ODD(sts) )
	{
	  if ( ODD( ldh_GetObjectBuffer( ldhses,
			window, "DevBody", "PlcWindow", (pwr_eClass *) &cid,
			&windbuffer, &size) ))
	  {
	    free( windbuffer);
	    sts = ldh_GetObjectClass ( ldhses, window, &cid);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_ObjidToName ( ldhses, cdh_ClassIdToObjid( cid), 
		ldh_eName_Object, class_name, sizeof(class_name), &size);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_ObjidToName ( ldhses, window, ldh_eName_Hierarchy, 
		hiername, sizeof(hiername), &size);
	    if ( EVEN(sts)) return sts;

	    fprintf( genctx->file, "/%s=%s", class_name, hiername);
	  }
	  sts = ldh_GetNextSibling( ldhses, window, &window);
	}

	return FOE__SUCCESS;
}
/*************************************************************************
*
* Name:		gen_filename()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create a file name from objid.
*
**************************************************************************/

static pwr_tStatus gen_filename (
  ldh_tSesContext	ldhses,
  pwr_tObjid		objid,
  char			*filename
)
{
	pwr_tStatus	sts;
	int		size;
	pwr_tString80	hiername;
	char		*s;

	sts = ldh_ObjidToName ( ldhses, objid,
		ldh_eName_Hierarchy, hiername, sizeof(hiername), &size);
	if ( EVEN(sts)) return sts;

	for ( s = hiername; *s; s++)
	  if ( *s == '-') *s = '_';

	if ( strlen( hiername) >= MAX_FILENAME_LEN)
	  strcpy( hiername, &hiername[ strlen(hiername)-MAX_FILENAME_LEN+1]);

	strcpy( filename, "pwrp_lis:");
	strcat( filename, hiername);
	strcat( filename, ".exp");
	return FOE__SUCCESS;
}
