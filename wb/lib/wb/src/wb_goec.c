/* wb_goec.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates gre connections.  */

#include <stdio.h>
#include <string.h>
#include <math.h>


#include "wb_ldh.h"

#include <Xm/Xm.h>

#include "flow.h"
#include "flow_api.h"

#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_goec.h"
#include "flow_api.h"
#include "pwr_baseclasses.h"

#include "wb_foe_macros.h"

#define	DEFERRED_UPDATE	/* XDefineCursor( XtDisplay( grectx->neted_window),\
		DwtGetWindow( grectx->neted_window),\
		grectx->cursors[ GOEN_CURSOR_CLOCK ]); */\
	  	NetedSetDisplayUpdate(grectx->neted_window,\
		NetedKupdateDeferred); 

#define	UPDATE_SCREEN	NetedSetDisplayUpdate( grectx->neted_window,\
				NetedKupdateImmediate);\
			/* XUndefineCursor( XtDisplay( grectx->neted_window),\
				DwtGetWindow( grectx->neted_window));*/

#define	CLOCK_CURSOR	/* XDefineCursor(	XtDisplay( grectx->neted_window),\
			DwtGetWindow( grectx->neted_window),\
			grectx->cursors[ GOEN_CURSOR_CLOCK ]);*/

#define	NORMAL_CURSOR	/* XUndefineCursor( XtDisplay( grectx->neted_window),\
			DwtGetWindow( grectx->neted_window)); */

#define	BEEP	      /*XBell(XtDisplay( grectx->neted_window))*/;

/*_Global variable______________________________________________________*/


/*_Local procedues_______________________________________________________*/



/*************************************************************************
*
* Name:		gre_con_draw()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_object	I	vldh connection.
* int		create_flag	I	type of creation ( GRE_CON_CREATE, 
*					GRE_CON_REDRAW or GRE_CON_NONROUTE)
* vldh_t_node	node		I	node to reconfigure ref cons.
*
* Description:
*	This routine draws a connection in the neted window.
*
**************************************************************************/

int	goec_con_draw( 
    gre_ctx		grectx,
    vldh_t_con	con,
    int		create_flag,
    vldh_t_node	node
)
{
	int		sts;
	flow_tConClass	con_class;
	int		i, num;
	double		*x_arr, *y_arr;
	double		x[10], y[10];
	vldh_t_node	tmp_node_pointer;
	unsigned long	tmp_point;
	pwr_tObjid	tmp_node_did;

	if ( con->lc.drawtype == GOEN_CONSYSREF ||
	     con->lc.drawtype == GOEN_CONUSERREF)
	  /* This is a fix for backward compatibility */
	  sts = gre_get_conclass( grectx,
                        0,
                        (con->hc.window_pointer)->hw.ldhsession,
                        con->lc.object_type,
                        &con_class);
	else
	  sts = gre_get_conclass( grectx,
                        con->lc.classid,
                        (con->hc.window_pointer)->hw.ldhsession,
                        con->lc.object_type,
                        &con_class);

        /* For grafcet-connectins source and destination class has to be right*/
        if ( (con->lc.classid == pwr_cClass_TransDiv &&
              con->hc.dest_node_pointer->ln.classid == pwr_cClass_trans) ||
             (con->lc.classid == pwr_cClass_TransConv &&
              con->hc.source_node_pointer->ln.classid == pwr_cClass_trans) ||
             (con->lc.classid == pwr_cClass_StepDiv &&
              con->hc.source_node_pointer->ln.classid == pwr_cClass_trans) ||
             (con->lc.classid == pwr_cClass_StepConv &&
              con->hc.dest_node_pointer->ln.classid == pwr_cClass_trans))
        {
          /* Shift */
          tmp_node_pointer = con->hc.source_node_pointer;
          tmp_point = con->lc.source_point;
          tmp_node_did = con->lc.source_node_did;
          con->hc.source_node_pointer = con->hc.dest_node_pointer;
          con->lc.source_point = con->lc.dest_point;
          con->lc.source_node_did = con->lc.dest_node_did;
          con->hc.dest_node_pointer = tmp_node_pointer;
          con->lc.dest_point = tmp_point;
          con->lc.dest_node_did = tmp_node_did;
        }

	if ( create_flag != GRE_CON_NONROUTE)
        {
	  flow_CreateCon( grectx->flow_ctx, con->hc.name, con_class, 
	  	con->hc.source_node_pointer->hn.node_id,
		con->hc.dest_node_pointer->hn.node_id,
		con->lc.source_point, con->lc.dest_point,
		con, &con->hc.con_id, 0, NULL, NULL);
	  flow_GetConPosition( con->hc.con_id, &x_arr, &y_arr, &num);
	  for ( i = 0; i < num; i++)
	  {
	    con->lc.point[i].x = x_arr[i];
            con->lc.point[i].y = y_arr[i];
	  }
	  con->lc.point_count = num;
	}
	else
	{
	  // In V2.7 max point was 10, in flow it is 8...
	  if ( con->lc.point_count > 8)
	    con->lc.point_count = 8;

	  for ( i = 0; i < con->lc.point_count; i++)
	  {
	    x[i] = con->lc.point[i].x;
	    y[i] = con->lc.point[i].y;
	  }
	  flow_CreateCon( grectx->flow_ctx, con->hc.name, con_class, 
	  	con->hc.source_node_pointer->hn.node_id,
		con->hc.dest_node_pointer->hn.node_id,
		con->lc.source_point, con->lc.dest_point,
		con, &con->hc.con_id, con->lc.point_count,
		x, y);
	}
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		goec_con_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_object	I	vldh connections to be deleted.
*
* Description:
*	Deletes a neted connection.
*
**************************************************************************/

int	goec_con_delete( 
	gre_ctx		grectx,
	vldh_t_con	con
)
{

        flow_DeleteConnection( con->hc.con_id);
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		goec_con_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_object	I	vldh connections to be deleted.
*
* Description:
*	Deletes a neted connection without redrawint the routed
*	conntections.
*
**************************************************************************/

int	goec_con_delete_noredraw( 
	gre_ctx		grectx,
	vldh_t_con	con
)
{
        flow_DeleteConnection( con->hc.con_id);
	return GRE__SUCCESS;
}


/*************************************************************************
*
* Name:		goec_con_sethighlight()
*
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_object	I	vldh connection.
* unsigned long	highlight_flag	I	set (1) or reset (0) of highlight.
*
* Description:
*	Set or reset of permanent highlight on a neted connection.
*
**************************************************************************/

int	goec_con_sethighlight(
	gre_ctx		grectx,
	vldh_t_con	con_object,
	unsigned long	highlight_flag
)
{
	flow_SetHighlight( con_object->hc.con_id, highlight_flag);
	return GRE__SUCCESS;
}

/*************************************************************************
*
* Name:		goec_con_gethighlight()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	grectx		I	gre context.
* vldh_t_con	con_object	I	vldh connection.
* unsigned long	highlight_flag	O	if highlight 1, if not highlight 0
*
* Description:
*	Get permanent highlight of a neted connection. Return 1 if 
*	the connection is highlighted and 0 if it is not.
*
**************************************************************************/

int	goec_con_gethighlight(
    gre_ctx		grectx,
    vldh_t_con	con_object,
    unsigned long	*highlight_flag
)
{
	flow_GetHighlight( con_object->hc.con_id, (int *)highlight_flag);
	return GRE__SUCCESS;
}


