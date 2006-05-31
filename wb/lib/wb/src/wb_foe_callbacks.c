/* 
 * Proview   $Id: wb_foe_callbacks.c,v 1.14 2006-05-31 08:11:03 claes Exp $
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

/* wb_foe_callbacks.c
   This module creates function object editor and handles
   the graphical editor.
   It contains all the callbacks from the widgets of the function
   block editor excepts the one from the controled neted widget :
   see ds_foe_gre.c module.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wb_foe_macros.h"
#include "wb_gcg.h"
#include "wb_login.h"

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>

#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_gobj.h"
#include "wb_goen.h"
#include "wb_exo.h"
#include "wb_gre.h"
#include "wb_tra.h"
#include "wb_foe_callbacks.h"
#include "wb_foe_api.h"
#include "co_api.h"
#include "co_wow.h"
#include "pwr_baseclasses.h"
#include "flow_x.h"

#define	BEEP	    putchar( '\7' );

/* #define	BEEP	 XBell(XtDisplay( grectx->neted_window))*/

#if 0
#define	CLOCK_CURSOR	XDefineCursor(\
		XtDisplay( (foectx->grectx)->work_area),\
		XtWindow( (foectx->grectx)->work_area),\
		(foectx->grectx)->cursors[ GOEN_CURSOR_CLOCK ]); \
		XFlush(XtDisplay(foectx->grectx->work_area));

		

#define	NORMAL_CURSOR	XUndefineCursor(\
			XtDisplay( (foectx->grectx)->work_area),\
			XtWindow( (foectx->grectx)->work_area));

/* Use these in the gredit backcalls where foectx is not declared  */

#define	GRE_CLOCK_CURSOR	XDefineCursor(\
		XtDisplay( grectx->work_area),\
		XtWindow( grectx->work_area),\
		grectx->cursors[ GOEN_CURSOR_CLOCK ]);\
		XFlush(XtDisplay(grectx->work_area));

#define	GRE_NORMAL_CURSOR	XUndefineCursor(\
			XtDisplay( grectx->work_area),\
			XtWindow( grectx->work_area));
#endif
#define	CLOCK_CURSOR	

#define	NORMAL_CURSOR	

#define	GRE_CLOCK_CURSOR	

#define	GRE_NORMAL_CURSOR	

static void foe_exit_save(
	  foe_ctx	foectx
);

static void foe_exit_nosave(
	  foe_ctx	foectx
);


/*************************************************************************
*
* Name:		void	foe_activate_save()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	The routine first checks that the parent of the window is
*	saved, and then saves the window.
*
**************************************************************************/

static void foe_activate_save (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	vldh_t_wind	wind;
	vldh_t_node	parent_node;
	int		sts;
	int 		size;
	pwr_tOName 	name;
	vldh_t_plc	plc;
	pwr_tFileName 	fname;
	char 		classname[80];
	pwr_tObjid 	classdef;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File Save");
	  return;
	}

	/* Check that the parent node is saved */
	
	wind = foectx->grectx->wind;
	plc = wind->hw.plc;
	parent_node = wind->hw.parent_node_pointer;
	if ( parent_node != 0 )
	{
	  /* Parent is node, not a plc */
	  if ( foectx->cp.parent_ctx != 0 )
	  {
	     /* parent is alive */
	    if ((parent_node->hn.status & VLDH_CREATE) != 0)
	    {
	      /* Parentnode is not saved, then it's not allowed to save
	          the window */
              foe_msgbox ( foectx , 
		  "UNABLE TO SAVE \nSave the parent window first." 
                  ) ;
	      return;
	    }
	  }
	}
	CLOCK_CURSOR;
	if ( !foectx->classeditor) {
	  sts = gre_set_trace_attributes( foectx->grectx, 0);
	  sts = gre_save( foectx->grectx, 0);
	  sts = gre_undelete_reset( foectx->grectx);
	}
	else {
	  sts = ldh_ObjidToName( wind->hw.ldhses, plc->lp.oid,
				 ldh_eName_Hierarchy, name, sizeof( name), &size); 
	  if ( EVEN(sts)) return;

	  sts = ldh_GetParent( wind->hw.ldhses, plc->lp.oid, &classdef);
	  if ( EVEN(sts)) return;

	  sts = ldh_ObjidToName( wind->hw.ldhses, classdef,
				 ldh_eName_Object, classname, sizeof( classname), &size); 
	  if ( EVEN(sts)) return;
	  cdh_ToLower( classname, classname);
	  sprintf( fname, "$pwrp_load/pwr_%s.flw", classname);
	  sts = gre_set_trace_attributes( foectx->grectx, name);
	  sts = gre_save( foectx->grectx, fname);
	  sts = gre_undelete_reset( foectx->grectx);	  
	}
	foe_disable_ldh_cb(foectx);
	sts = vldh_wind_save( foectx->grectx->wind);
	foe_enable_ldh_cb(foectx);
	error_msg( sts);
	
	// gre_save(foectx->grectx, 0);

	NORMAL_CURSOR;

	if ( ODD(sts)) 
	  foe_message ( foectx, "Window saved");
	else if ( sts == VLDH__PLCNOTSAVED )
          foe_msgbox ( foectx , 
		"UNABLE TO SAVE \nSave the plcprogram in the hierarchy editor first." 
                ) ;
	else if (EVEN(sts)) 
	  foe_message( foectx,"ERROR   COULDN'T SAVE WINDOW");

}




/*************************************************************************
*
* Name:		void	foe_activate_exit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	The routine first checks that the parent node of the window
*	is saved, and then saves the window and quits.
*
**************************************************************************/

static void foe_activate_exit (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
}

/*************************************************************************
*
* Name:		void	foe_activate_winddelete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	It is not allowed to delete orderwindows due to problems with
*	the brotherhood in ldh.
*	Checks first that the window doesn't have any subwindows,
*	and then starts the popupmessagebox "Do you really want to ..."
*	which calls foe_delete if ok.
*
**************************************************************************/

static void foe_activate_winddelete (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, subwind_count;
	char			message[80];
	int			sts;
	vldh_t_wind		wind;
	pwr_tClassId		class;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File DeleteWindow");
	  return;
	}

	/*************************************************/
	/* BUGGFIX, quit on suborder windows not allowed */
	/*************************************************/
	wind = foectx->grectx->wind;
	sts = ldh_GetObjectClass( wind->hw.ldhses,
			wind->lw.poid,
			&class);
	error_msg( sts);
	if (( class == pwr_cClass_order) &&
		((wind->hw.status & VLDH_CREATE) != 0 ))
	{
	  foe_message( foectx, 
		" Operationen ej till}ten pga bugg, g|r save f|rst");
	  return;
	}	  
	/**************************************************/

	/* Check that there is no subwindows in this window */
	sts = vldh_get_nodes( foectx->grectx->wind, 
			&node_count, &nodelist);
	error_msg( sts);
	node_ptr = nodelist;
	subwind_count = 0;
	for ( i = 0; i < node_count; i++)
	{
	  if ( (*node_ptr)->ln.subwindow != 0 )	
	    /* Subwindow */
	    subwind_count += (((*node_ptr)->ln.subwindow & 1) != 0)
			   + (((*node_ptr)->ln.subwindow & 2) != 0);
	  node_ptr++;
	}
	if ( subwind_count > 0 )
	{
	  /* Created subwindows found, inform and break */
	  sprintf( message, 
	    " %d subwindow found in this window\nDelete them first",
	    subwind_count);
          foe_msgbox ( foectx , message ); 
	  return;
	}
	if ( node_count > 0) XtFree((char *)nodelist);

        /* You should take care of inconscious users.
          Ask them to confirm the action */

        foe_popupmsg ( foectx , 
		"Do you really want to delete this window ?" ,
                 foe_delete, NULL, NULL) ;
      
}



/*************************************************************************
*
* Name:		void	foe_activate_quit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	It is not allowed to quit order subwindows due to problems with
*	the brotherhood in ldh.
*	Checks that no node that is not saved contains a subwindow.
*	Then starts the popup messagebox "Do you really want to..."
* 	which calls foe_quit if ok.
* SG 27.05.91  add treatement for quit from trace .
**************************************************************************/

void foe_activate_quit (
    Widget	w,
    foe_ctx	foectx,
    XmAnyCallbackStruct	*data
)
{
	unsigned long		wind_count;
	vldh_t_wind		*windlist;
	vldh_t_wind		*wind_ptr;
	int			i, w_create_count, create_count;
	char			message[80];
	int			sts;
	vldh_t_wind		wind;
	int			vldh_mod;
	ldh_sSessInfo		info;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File Close");
	  return;
	}

	wind = foectx->grectx->wind;

	/* Check that there is no subwindows created during this session
	   that is not saved while the node also is not saved */
	sts = vldh_get_wind_subwindows( foectx->grectx->wind, 
			&wind_count, &windlist);
	error_msg( sts);
	wind_ptr = windlist;
	w_create_count = 0;
	create_count = 0;
	for ( i = 0; i < wind_count; i++)
	{
	  if ( (*wind_ptr)->hw.foectx != 0)
	  {
	    /* Subwindow created, check if parentnode also is created and
	       not saved */
	    create_count++;
	  }
	  wind_ptr++;
	}
	if ( wind_count > 0) XtFree((char *) windlist);

	if ( create_count > 0 && foectx->access != ldh_eAccess_SharedReadWrite)
	{
	  /* Created subwindows found, inform and break */
	  sprintf( message, 
	    "Exit subwindows first, %d subwindow found",
	    create_count);
          foe_msgbox ( foectx , message ); 
	  return;
	}

	/* If trace or view, exit without confirm */
        if ( ( foectx->function == TRACE ) || ( foectx->function == VIEW ) 
		|| ( foectx->function == SIMULATE ))
        {
	  /* If session is not empty revert it first */
/*
	  sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
	  error_msg( sts);

	  if ( !info.Empty )
	  {
	    sts = ldh_SetSession( wind->hw.ldhses,
			ldh_eAccess_ReadWrite);
	    error_msg( sts);
	    sts = ldh_RevertSession( wind->hw.ldhses);
	    error_msg( sts);
	  }
*/
	  trace_stop ( foectx );
	  foe_quit ( foectx ) ;
          return;

        }

	/* Check if changes are made */
	sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
	error_msg( sts);
	sts = vldh_get_wind_modification( wind, &vldh_mod);

	if ( !info.Empty || vldh_mod)
	{
          foe_popupmsg( foectx , "Do you want to save changes", 
		foe_exit_save, foe_exit_nosave, NULL); 
	}
	else
	  /* Dont bother to ask */
	  foe_quit( foectx);
}

/*************************************************************************
*
* Name:		void	foe_activate_print()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	This function finds all document objects in the window
*	and calls gre_print_docobj for each of them.
*
**************************************************************************/

static void foe_activate_savetrace (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int			sts;

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File SaveTrace");
	  return;
	}
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	sts = trace_save( foectx->grectx);
}
static void foe_activate_restoretrace (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int			sts;

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File RestoreTrace");
	  return;
	}
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	sts = trace_restore( foectx->grectx);
}


/*************************************************************************
*
* Name:		void	foe_activate_print()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	This function finds all document objects in the window
*	and calls gre_print_docobj for each of them.
*
**************************************************************************/

static void foe_activate_print (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
int			sts;

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File PrintDocuments");
	  return;
	}
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	sts = foe_print_document( foectx);
	if ( sts == FOE__NOOBJFOUND)
        {
	  foe_message( foectx, "No documents found");
        }

}


/*************************************************************************
*
* Name:		void	foe_activate_printoverv()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	This function finds all document objects in the window
*	prints an overview of them.
*
**************************************************************************/

static void foe_activate_printoverv (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
int		sts;

	vldh_t_wind	wind;

	wind = foectx->grectx->wind;

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File PrintOverview");
	  return;
	}
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	sts = foe_print_overview( foectx);
	if ( sts == FOE__NOOBJFOUND)
        {
	  foe_message( foectx, "Less than two documents, no overview printed");
        }

}


/*************************************************************************
*
* Name:		void	foe_activate_printselect()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	This function print a portion of the screen defined by the last
*	selected region.
*	Changed 980908 to print the selected document objects.
*
**************************************************************************/

static void foe_activate_printselect (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int		sts;

	sts = foe_print_selected_document( foectx);
	if ( sts == FOE__NOOBJFOUND)
        {
	  foe_message( foectx, "No documents found");
        }
}


/*************************************************************************
*
* Name:		void	foe_activate_syntax()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Calls foe_gcg to control syntax for the window.
*	Displays a message with number of errors or warnings.
*
**************************************************************************/

static void foe_activate_syntax (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int		sts;
	unsigned long	errcount = 0;
	unsigned long	warncount = 0;
	char		message[80];

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File Syntax");
	  return;
	}

	sts = gcg_plcwindow_compile( foectx->grectx->wind, 0, 
		&errcount, &warncount, 0, 0);
	if ( sts == GSX__AMBIGOUS_EXECUTEORDER )
	{
	  foe_message( foectx, "Execute order error");
	  BEEP;
	  return;
	}
	else if ( sts == GSX__PLCWIND_ERRORS)
	{
	  /* Errors found, inform the user*/
	  sprintf( message, 
	  "Syntax %ld errors  %ld warnings",
	    errcount, warncount);
	  foe_message( foectx, message);
	  BEEP;
	}
	else if ( EVEN(sts))
	{
	  foe_message( foectx, 
		"Error in class definition, syntax aborted");
	  BEEP;
	  error_msg( sts);
	  return;
	}
	else	
	{
	  if ( warncount > 0 )
	  {
	    /* Errors found, inform the user*/
	    sprintf( message, 
	    "Syntax %ld warnings", warncount);
	    foe_message( foectx, message);
	    BEEP;
	  }
	  else
	    foe_message( foectx, "Syntax successfull completion");
	}

	if ( foectx->show_executeorder)
	  foe_show_executeorder( foectx);
}


/*************************************************************************
*
* Name:		void	foe_activate_compile()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Calls foe_gcg to make syntax control and generate plc code
*	for the window. Displays a message of the result of the 
*	syntax control.
*
**************************************************************************/

static void foe_activate_compile (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int		sts;
	unsigned long	errcount = 0;
	unsigned long	warncount = 0;
	unsigned long	plc_errcount = 0;
	unsigned long	plc_warncount = 0;
	char		message[80];
	vldh_t_wind	wind;
	int		vldh_mod;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File Compile");
	  return;
	}

	wind = foectx->grectx->wind;

	sts = vldh_get_wind_modification( wind, &vldh_mod);

	if ( vldh_mod)
	{
	  foe_message( foectx, "Session is not saved");
	  BEEP;
	  return;
	}

	foe_disable_ldh_cb(foectx);
	sts = gcg_plcwindow_compile( wind, 1, &errcount, &warncount, 0, 0);
	foe_enable_ldh_cb(foectx);
	if ( sts == GSX__AMBIGOUS_EXECUTEORDER )
	{
	  foe_message( foectx, "Execute order error");
	  BEEP;
	  return;
	}
	else if ( sts == GSX__NOTSAVED)
	{
	  /* Session is not saved */
	  foe_message( foectx, "Session not saved, no objectfile created");
	  BEEP;
	  return;
	}
	else if ( sts == GSX__PLCWIND_ERRORS)
	{
	  /* Errors found, inform the user*/
	  sprintf( message, 
	  "Compile %ld errors  %ld warnings, no objectfile created",
	    errcount, warncount);
	  foe_message( foectx, message);
	  BEEP;
	  return;
	}
	else if ( EVEN(sts))
	{
	  foe_message( foectx, "Ldh error");
	  BEEP;
	  error_msg( sts);
	  return;
	}
	
	sts = gcg_plc_compile( wind->hw.plc, 1, 
		&plc_errcount, &plc_warncount, 0, 0);
	warncount += plc_warncount;
	errcount += plc_errcount;
	if ( sts == GSX__PLCPGM_ERRORS)
	{
	  /* Errors found, inform the user*/
	  sprintf( message, 
	  "Compile %ld errors  %ld warnings, no objectfile created",
	    errcount, warncount);
	  foe_message( foectx, message);
	  BEEP;
	  return;
	}
	else if ( EVEN(sts))
	{
	  foe_message( foectx, "Ldh error");
	  BEEP;
	  error_msg( sts);
	  return;
	}
	else if ( warncount > 0 )
	{
	  /* Errors found, inform the user*/
	  sprintf( message, 
	  "Compile %ld warnings", warncount);
	  foe_message( foectx, message);
	}

	if ( foectx->show_executeorder)
	  foe_show_executeorder( foectx);
}


/*************************************************************************
*
* Name:		void	foe_activate_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Deletes the selected nodes and connections.
*	Checks first that a selected node does not contain a subwindow.
*
**************************************************************************/
static void foe_delete_subwindow_ok_cb( void *ctx, void *data)
{
  foe_ctx foectx = (foe_ctx) ctx;
  vldh_t_node node = (vldh_t_node) data;
  pwr_tStatus sts, rsts;
  pwr_tOid child, nextchild;
  vldh_t_wind wind = foectx->grectx->wind;
  unsigned long		wind_count;
  vldh_t_wind		*windlist;
  vldh_t_wind		*wind_ptr;
  int			i;

  /* Check that subwindow is not open */
  sts = vldh_get_wind_subwindows( foectx->grectx->wind, 
				  &wind_count, &windlist);
  if( EVEN(sts)) { error_msg( sts); return;}

  for ( sts = ldh_GetChild( wind->hw.ldhses, node->ln.oid, &child);
	ODD(sts);
	sts = ldh_GetNextSibling( wind->hw.ldhses, child, &child)) {
    wind_ptr = windlist;
    for ( i = 0; i < wind_count; i++) {
      if ( cdh_ObjidIsEqual( (*wind_ptr)->lw.oid, child)) {
	wow_DisplayError( foectx->widgets.foe_window, "Window open",
			  "Subwindow is open\nClose subwindow first");
	XtFree((char *) windlist);
	return;
      }
      wind_ptr++;
    }
  }
  if ( wind_count > 0) XtFree((char *) windlist);

  for ( sts = ldh_GetChild( wind->hw.ldhses, node->ln.oid, &child);
	ODD(sts);
	child = nextchild) {
    sts = ldh_GetNextSibling( wind->hw.ldhses, child, &nextchild);

    rsts = ldh_DeleteObjectTree( wind->hw.ldhses, child);
    if ( EVEN(rsts)) { error_msg( rsts); return; }
  }
  node->ln.subwindow = 0;
  gre_delete( foectx->grectx);
}

static void foe_activate_delete (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, subwind_found;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit Delete");
	  return;
	}

	/* Delete the selected nodes and connections */
	/* Check that any node doesn't contain a subwindow */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1 && (*nodelist)->ln.subwindow != 0) {
	  /* Single node that has subwindow */
	  char msg[200];
	  sprintf( msg, 
         "Object \"%s\" has subwindow\n Do you wan't to delete the subwindow?",
		   (*nodelist)->hn.name);
	  wow_DisplayQuestion( foectx, foectx->widgets.foe_window, 
			       "Delete subwindow", msg,
			       foe_delete_subwindow_ok_cb, 0, *nodelist);
	  foectx->popupmenu_node = 0;
	  return;
	}

	node_ptr = nodelist;
	subwind_found = FALSE;
	for( i = 0; i < node_count; i++)
	{
	  if ( (*node_ptr)->ln.subwindow != 0 )
	  {
	    subwind_found = TRUE;
	    break;
	  }
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);
	if ( subwind_found )
	{
	  wow_DisplayError( foectx->widgets.foe_window, "Subwindow found",
		   "Subwindow found\nDelete objects with subwindows separately");
	  BEEP;
	}
	else
	{
	  /* Delete selected nodes */
	  gre_delete( foectx->grectx);
	  foectx->popupmenu_node = 0;
	}
}

/*************************************************************************
*
* Name:		void	foe_activate_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Deletes the selected nodes and connections.
*	Checks first that a selected node does not contain a subwindow.
*
**************************************************************************/

static void foe_activate_changetext (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	watt_tCtx               watt;
        int                     sts;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit ChangeText");
	  return;
	}

	/* Delete the selected nodes and connections */
	/* Check that any node doesn't contain a subwindow */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);
	if ( node_count != 1 )
	{
	  foe_message( foectx, "Select one text object");
	  BEEP;
	  return;
	}

        switch ( nodelist[0]->ln.cid) {
	  case pwr_cClass_Text:
          case pwr_cClass_BodyText:
          case pwr_cClass_Head:
          case pwr_cClass_Title:
	    foe_attr_create( foectx, nodelist[0]);
            sts = foe_attrlist_get_by_node( nodelist[0], &watt);
            if ( EVEN(sts)) return;
            sts = watt_open_changevalue( watt, "Text");
	    break;
	  default:
	    foe_message( foectx, "Select a text object");
	    BEEP;
        }
}

/*************************************************************************
*
* Name:		void	foe_activate_cut()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Deletes the selected nodes and connections and places them
*	in the vldh paste list.
*	Checks first that a selected node does not contain a subwindow.
*
**************************************************************************/

static void foe_activate_cut (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int sts;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, subwind_found;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit CutRegion");
	  return;
	}

	/* Cut */
	/* Check that any node doesn't contain a subwindow */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);
	node_ptr = nodelist;
	subwind_found = FALSE;
	for( i = 0; i < node_count; i++)
	{
	  if ( (*node_ptr)->ln.subwindow != 0 )
	  {
	    subwind_found = TRUE;
	    break;
	  }
	  node_ptr++;
	}
	if ( node_count > 0) XtFree((char *) nodelist);
	if ( subwind_found )
	{
	  foe_message( foectx, 
	     "Subwindow found, use copy or delete the subwindow first");
	  BEEP;
	}
	else
	{
	  /* Cut selected nodes */
	  sts = gre_cut(foectx->grectx);
	  if (sts == GRE__NOREGSEL)
	  {
	    foe_message( foectx, "Select region before cutting");
	    BEEP;
	  }
	  else if ( EVEN(sts))
            error_msg( sts);
	  foectx->popupmenu_node = 0;
	}
	

}

/*************************************************************************
*
* Name:		void	foe_activate_copy()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Copies selected nodes to vldh paste list.
*
**************************************************************************/

static void foe_activate_copy (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int	sts;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit CopyRegion");
	  return;
	}

	/* Copy */
	sts = gre_copy(foectx->grectx);
	if (sts == GRE__NOREGSEL)
	{
	  foe_message( foectx, "Select region before copying");
	  BEEP;
	}
	else if ( EVEN(sts))
          error_msg( sts);
	else
	  foe_message( foectx, "Region copied");
}

/*************************************************************************
*
* Name:		void	foe_activate_paste()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Calls the gre paste funktion.
*
**************************************************************************/

static void foe_activate_paste (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit PasteRegion");
	  return;
	}
	foe_disable_ldh_cb(foectx);
	gre_paste(foectx->grectx, 0.0, 0.0, GRE_PASTE_CENTER);
	foe_enable_ldh_cb(foectx);
}

/*************************************************************************
*
* Name:		void	foe_activate_attribute()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Creates a attribute editor for the selected object
*	unselects it.
* SG 25.09.91 new message to the user ( see call to foe_message )
*
**************************************************************************/

static void foe_activate_attribute (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		object;
        int                     sts;
	watt_tCtx               watt;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Functions OpenObject");
	  return;
	}

	/* Get selected nodes */

	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1 ) 
	{
	  /* Create attribute editor */
          if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	  object = *nodelist;
          sts = foe_attrlist_get_by_node( object, &watt);
          if ( ODD(sts))
            watt_pop( watt);
          else {
	    CLOCK_CURSOR;
	    foe_attr_create(foectx, object);
	    NORMAL_CURSOR;
          }
	  gre_unselect( foectx->grectx);
	}

        if ( node_count > 0) XtFree((char *) nodelist);

	if ( node_count == 0) 
	{
	  foe_message( foectx, "Select an object to edit attributes");
	  BEEP;
	}
	else if ( node_count > 1 ) 
	{
  	  foe_message( foectx, 
		"Unable to open object, more than one object selected");
	  BEEP;
	}
        return;
}

/*************************************************************************
*
* Name:		void	foe_activate_subwindow()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Creates a subwindow with window index 1 to the selected node by
* 	the function foe_subwindow_create.
* SG 25.09.91 new message to the user ( see call to foe_message )
*
**************************************************************************/

static void foe_activate_subwindow (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		object;
	int			sts;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Functions Subwindow");
	  return;
	}

	/* Get selected nodes */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1) 
	{
          if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	  /* Create subwindow, take the first windowindex */
	  object = *nodelist;
	  CLOCK_CURSOR;
	  sts = foe_subwindow_create( foectx, object, 1);
          error_msg( sts);
	  NORMAL_CURSOR;
	  gre_unselect( foectx->grectx);
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( node_count == 0) 
	{
	  foe_message( foectx, "Select an object to create a subwindow");
	  BEEP;
	}
        else if ( node_count > 1 ) 
	{
	  foe_message( foectx, 
		"Can't create a subwindow, more than one object selected");
	  BEEP;
	}
}


/*************************************************************************
*
* Name:		void	foe_activate_undelete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Calls the gre undelete function which restores all
*	objects deleted at the last delete or cut operation.
*
**************************************************************************/

static void foe_activate_undelete (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit UndoDelete");
	  return;
	}

	gre_undelete( foectx->grectx);
}

/*************************************************************************
*
* Name:		void	foe_activate_unselect()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Unselects all selected nodes.
*
**************************************************************************/

static void foe_activate_unselect (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit UndoSelect");
	  return;
	}

	gre_unselect( foectx->grectx);
}


/*************************************************************************
*
* Name:		void	foe_activate_gridsize()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Sets gridsize.
*
**************************************************************************/

static void foe_activate_gridsize1 (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  gre_set_grid_size( foectx->grectx, 0.01);
}


static void foe_activate_gridsize2 (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  gre_set_grid_size( foectx->grectx, 0.02);
}


static void foe_activate_gridsize3 (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  gre_set_grid_size( foectx->grectx, 0.05);
}

static void foe_activate_gridsize4 (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  gre_set_grid_size( foectx->grectx, 0.10);
}


static void foe_activate_gridsize5 (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  gre_set_grid_size( foectx->grectx, 0.20);
}


/*************************************************************************
*
* Name:		void	foe_activate_showgrid()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Draws or deletes the grid.
*
**************************************************************************/

static void foe_activate_showgrid (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{
#if 0
	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ShowGrid");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	if(data->set == 1)
	{
	  gre_drawgrid( foectx->grectx);
	}
	else  /* data->set == 0 */
	{
	  gre_undrawgrid( foectx->grectx);
	}
#endif
}


/*************************************************************************
*
* Name:		void	foe_activate_redraw()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*
**************************************************************************/

static void foe_activate_redraw (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int			sts;


	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View Redraw");
	  return;
	}

	sts = gre_redraw( foectx->grectx);
	error_msg( sts);
}


/*************************************************************************
*
* Name:		void	foe_activate_zoomin()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Zoom in.
*
**************************************************************************/

static void foe_activate_zoomin (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	gre_zoom( foectx->grectx, 1.43);
}


/*************************************************************************
*
* Name:		void	foe_activate_zoomout()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Zoom out
*
**************************************************************************/

static void foe_activate_zoomout (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	gre_zoom( foectx->grectx, 0.7);
}


/*************************************************************************
*
* Name:		void	foe_activate_unzoom()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Calls the gre unzoom funktion.
*
**************************************************************************/

static void foe_activate_unzoom (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	gre_unzoom( foectx->grectx);
}

/*************************************************************************
*
* Name:		void	foe_activate_palette_con()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Display or hide the connection palette.
*
**************************************************************************/

static void foe_activate_palette_con (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{

	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	if ( data->event && data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ConnectionPalette");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	if ( data->set)
	  XtManageChild( foectx->widgets.con_palette);
	else
	  XtUnmanageChild( foectx->widgets.con_palette);
#if 0
	XtSetSensitive(foectx->conn_palctx->w1->svn_name_label, data->set);
#endif
	foectx->con_palette_managed = data->set;
        pal_set_inputfocus( foectx->con_palctx, data->set);
}


/*************************************************************************
*
* Name:		void	foe_activate_palette_object()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Display or hide the connection palette.
*
**************************************************************************/

static void foe_activate_palette_object(w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{

	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	if ( data->event && data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ConnectionPalette");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	if ( data->set)
	  XtManageChild( foectx->widgets.node_palette);
	else
	  XtUnmanageChild( foectx->widgets.node_palette);
#if 0
	XtSetSensitive(foectx->conn_palctx->w1->svn_name_label, data->set);
#endif
	foectx->node_palette_managed = data->set;
        pal_set_inputfocus( foectx->node_palctx, data->set);
}


/*************************************************************************
*
* Name:		void	foe_activate_palette_plant()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Display or hide the connection palette.
*
**************************************************************************/

static void foe_activate_palette_plant(w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{

	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 
	if ( data->event && data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ConnectionPalette");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	if ( data->set)
	  XtManageChild( foectx->widgets.nav_palette);
	else
	  XtUnmanageChild( foectx->widgets.nav_palette);
#if 0
	XtSetSensitive(foectx->conn_palctx->w1->svn_name_label, data->set);
#endif
	foectx->nav_palette_managed = data->set;
        nav_set_inputfocus( foectx->navctx, data->set);
}


/*************************************************************************
*
* Name:		void	foe_activate_refcon()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* 	Description:
* 	Callback from the menu.
*	Sets foectx->con_drawtype to GOEN_CONUSERREF or GOEN_CONDRAW.
*	If GOEN_CONUSERREF created connections will be referencetype.
*	
* 	SG 24.03.91
**************************************************************************/

static void foe_activate_refcon (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{

	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ConnectionReference");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	foectx->con_drawtype = data->set ? GOEN_CONUSERREF : GOEN_CONDRAW ;
}


/*************************************************************************
*
* Name:		void	foe_activate_showexeord()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* 	Description:
* 	Callback from the menu.
*	Show execute order.
*
**************************************************************************/

static void foe_activate_showexeord (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct     *data;
{
	int		sts;

	if ( data->set > 1) return;
        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu View ShowExecuteOrder");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

	foectx->show_executeorder = data->set;

	if ( foectx->show_executeorder)
	{
	  sts = exo_wind_exec( foectx->grectx->wind);
	  if ( sts == GSX__AMBIGOUS_EXECUTEORDER )
	  {
	    foe_message( foectx, "Execute order error");
	    BEEP;
	  }
	  else if ( EVEN(sts))
            error_msg( sts);
	  else
	    foe_show_executeorder( foectx);
	}
	else
	  gre_undisplay( foectx->grectx);
}



/*************************************************************************
*
* Name:		void	foe_activate_plcattribute()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
* 	Calls the attribute editor for the plcprogram.
*
**************************************************************************/

static void foe_activate_plcattribute (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu File PlcAttribute");
	  return;
	}

	foe_attr_create ( foectx, 0);
}


/*************************************************************************
*
* Name:		void	foe_activate_search ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Not yet implemented.
*	For the moment it starts GOE.
*
**************************************************************************/

static void foe_activate_search  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Search");
	  return;
	}

/*
    foe_message( foectx, "You have just called the Graphic Object Editor");
    goe_new( 0, foectx->widgets.foe_window, "Graphic Object Editor");
*/
}


/*************************************************************************
*
* Name:		void	foe_activate_searchobject ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Not yet implemented.
*	For the moment it starts GOE.
*
**************************************************************************/

static void foe_activate_searchobject  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Search Object");
	  return;
	}

	foe_get_textinput ( foectx, "Enter object : ", &foe_search_object);
}


/*************************************************************************
*
* Name:		void	foe_activate_searchstring ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Not yet implemented.
*
**************************************************************************/

static void foe_activate_searchstring  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Search String");
	  return;
	}

	foe_get_textinput ( foectx, "Enter string: ", &foe_search_string);
}


/*************************************************************************
*
* Name:		void	foe_activate_searchnext ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Not yet implemented.
*	For the moment it starts GOE.
*
**************************************************************************/

static void foe_activate_searchnext  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	int sts;

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Search Next");
	  return;
	}

	sts = foe_search_string_next( foectx);
	error_msg( sts);
}


/*************************************************************************
*
* Name:		void	foe_activate_getobj()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the edit entry of the foe_menu widget
* THe user hopes to get the id of an object from the hardware hierarchy
* to connect it to the getsomething object in his graphcet  diagram.
* THe getsomthing object should should be selected first.
* SG 11.04.91 
**************************************************************************/

static void foe_activate_getobj (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

	unsigned long		node_count;
	vldh_t_node		*nodelist;
	int			sts;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit Connect");
	  return;
	}

	/* Get selected nodes */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1) 
	{
          if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	  /* get the id of the desired object */
          sts = gobj_get_object ( foectx , *nodelist , 1) ;
          error_msg( sts);
	  gre_unselect( foectx->grectx);
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( node_count == 0) 
	{
	  foe_message( foectx, "Select an object first ");
	  BEEP;
	}
	else if ( node_count > 1 ) 
	{
  	  foe_message( foectx, 
		"Unable to connect, more than one object selected");
	  BEEP;
	}

}

/*************************************************************************
*
* Name:		void	foe_activate_expand()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the edit entry of the foe_menu widget
**************************************************************************/

static void foe_activate_expand (
	Widget	w,
	foe_ctx	foectx,
	XmAnyCallbackStruct	*data)
{

	unsigned long		node_count;
	vldh_t_node		*nodelist;
	int			sts;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit Expand");
	  return;
	}

	/* Get selected nodes */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1) 
	{
          if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	  /* get the id of the desired object */
          sts = gobj_expand( foectx , *nodelist , 0);
          error_msg( sts);
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( node_count == 0) 
	{
	  foe_message( foectx, "Select an object first ");
	  BEEP;
	}
	else if ( node_count > 1 ) 
	{
  	  foe_message( foectx, 
		"Unable to expand, more than one object selected");
	  BEEP;
	}
}

/*************************************************************************
*
* Name:		void	foe_activate_compress()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the edit entry of the foe_menu widget
**************************************************************************/

static void foe_activate_compress (
	Widget	w,
	foe_ctx	foectx,
	XmAnyCallbackStruct	*data)
{

	unsigned long		node_count;
	vldh_t_node		*nodelist;
	int			sts;


        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Edit Expand");
	  return;
	}

	/* Get selected nodes */
	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( node_count == 1) 
	{
          if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	  /* get the id of the desired object */
          sts = gobj_expand( foectx , *nodelist , 1);
          error_msg( sts);
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( node_count == 0) 
	{
	  foe_message( foectx, "Select an object first ");
	  BEEP;
	}
	else if ( node_count > 1 ) 
	{
  	  foe_message( foectx, 
		"Unable to expand, more than one object selected");
	  BEEP;
	}
}


/*************************************************************************
*
* Name:		void	foe_activate_help()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Display help window for topic plc editor
*
**************************************************************************/

static void foe_activate_help(w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
  xhelp_help( "plc editor", 0, navh_eHelpFile_Base, 0, 1);
}


/*************************************************************************
*
* Name:		void	foe_activate_helpclass()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Display class help for selected object
*
**************************************************************************/

static void foe_activate_helpclass(w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_wind		wind;
	char			cname[32];
	int			size;
	int			sts;

	wind = foectx->grectx->wind;

	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);
	if ( node_count != 1 )
	{
	  foe_message( foectx, "Select one object");
	  BEEP;
	}

	sts = ldh_ClassIdToName( wind->hw.ldhses, nodelist[0]->ln.cid, cname,
				 sizeof(cname), &size);
	if ( EVEN(sts)) return;

	xhelp_help( cname, 0, navh_eHelpFile_Base, 0, 1);
}


/*************************************************************************
*
* Name:		void	foe_activate_trace_togg () 
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callbacks from the toggle buttons under the function entry.
*	This callbacks are called when the value of a toggle button is changed.
*	The function pulldown menu is a radio box which ensure that 
*	one and only one toggle button is set at any time:
*	If the callback is generated by the radio entry 
*	( and not directly by the user ) the event is 0.
*
*	These callbacks are use to determine in which mode the user works: 
* SG 01.05.91
**************************************************************************/

static void foe_activate_trace_togg (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct	*data;
{
	int		sts;

	if ( data->set > 1) return;
	if ( data->event == 0)
	  return;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Mode Trace");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

        if ( data->set == 1 ) 
	{
	  sts = foe_change_mode( foectx, TRACE);
	}
}


/*************************************************************************
*
* Name:		void	foe_activate_simulate_togg () 
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callbacks from the toggle buttons under the customize entry.
**************************************************************************/
static void foe_activate_simulate_togg (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct	*data;
{
	int		sts;

	if ( data->set > 1) return;
	if ( data->event == 0)
	  return;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Mode Simulate");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

        if ( data->set == 1 ) 
	  sts = foe_change_mode( foectx, SIMULATE);
	else
	  sts = foe_change_mode( foectx, TRACE);

}


/*************************************************************************
*
* Name:		void	foe_activate_view_togg () 
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callbacks from the toggle buttons under the function entry.
*	This callbacks are called when the value of a toggle button is changed.
*	The function pulldown menu is a radio box which ensure that 
*	one and only one toggle button is set at any time:
*	If the callback is generated by the radio entry the event is 0.
*	and no treatement is made in the callback routine
*
*	These callbacks are use to determine in which mode the user works: 
* SG 01.05.91
**************************************************************************/
static void foe_activate_view_togg (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct	*data;
{
	int		sts;

	if ( data->set > 1) return;
	if ( data->event == 0)
	  return;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Mode View");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

        if ( data->set == 1 ) 
	{
	  sts = foe_change_mode( foectx, VIEW);
	}
}


/*************************************************************************
*
* Name:		void	foe_activate_edit_togg () 
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callbacks from the toggle buttons under the function entry.
*	This callbacks are called when the value of a toggle button is changed.
*	The function pulldown menu is a radio box which ensure that 
*	one and only one toggle button is set at any time:
*	If the callback is generated by the radio entry the event is 0.
*	and no treatement is made in the callback routine
*
*	These callbacks are use to determine in which mode the user works: 
* SG 01.05.91
**************************************************************************/

static void foe_activate_edit_togg (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmToggleButtonCallbackStruct	*data;
{
	int		sts;

	if ( data->set > 1) return;
	if ( data->event == 0)
	  return;

        if ( foectx->msg_label_id != 0 ) foe_message( foectx, ""); 

	if ( data->event->type == ButtonRelease 
	     && data->event->xbutton.state & Mod5Mask 
	) {
	  foe_display_help( foectx, "Help Menu Mode Edit");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

        if ( data->set == 1 )
	  sts = foe_change_mode( foectx, EDIT);
	else 
	  sts = foe_change_mode( foectx, VIEW);
	
}



/*************************************************************************
*
* Name:		int	foe_create_*()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
*
* Description:
*	store the id of the created widget in the context
* Author : SG
* Date : 25.01.91
*
**************************************************************************/

static void foe_create_label 
	( Widget w , foe_ctx foectx, XmAnyCallbackStruct *data) 
		{ foectx->widgets.label= w ; }

static void foe_create_textinput 
	( Widget w , foe_ctx foectx, XmAnyCallbackStruct *data) 
		{ foectx->widgets.textinput = w ; }

static void foe_create_widget_cb( Widget w, foe_ctx foectx, XmAnyCallbackStruct	*data)
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);

  switch ( key) {
  case 1:
    foectx->widgets.file_entry = w;
    break;
  case 2:
    foectx->widgets.save = w;
    break;
  case 3:
    foectx->widgets.print = w;
    break;
  case 6:
    foectx->widgets.syntax = w;
    break;
  case 7:
    foectx->widgets.compile = w;
    break;
  case 8:
    foectx->widgets.plcattribute = w;
    break;
  case 9:
    foectx->widgets.winddelete = w;
    break;
  case 10:
    foectx->widgets.savetrace = w;
    break;
  case 11:
    foectx->widgets.restoretrace = w;
    break;
  case 12:
    foectx->widgets.quit = w;
    break;
  case 13:
    foectx->widgets.exit = w;
    break;
  case 14:
    foectx->widgets.edit_entry = w;
    break;
  case 15:
    foectx->widgets.cut = w;
    break;
  case 16:
    foectx->widgets.copy = w;
    break;
  case 17:
    foectx->widgets.paste = w;
    break;
  case 18:
    foectx->widgets.undelete = w;
    break;
  case 19:
    foectx->widgets.unselect = w;
    break;
  case 20:
    foectx->widgets.connect = w;
    break;
  case 21:
    foectx->widgets.del = w;
    break;
  case 22:
    foectx->widgets.changetext = w;
    break;
  case 23:
    foectx->widgets.expand = w;
    break;
  case 24:
    foectx->widgets.compress = w;
    break;
  case 25:
    foectx->widgets.search_entry = w;
    break;
  case 26:
    foectx->widgets.customize_entry = w;
    break;
  case 28:
    foectx->widgets.redraw = w;
    break;
  case 30:
    foectx->widgets.view_togg = w;
    break;
  case 31:
    foectx->widgets.edit_togg = w;
    break;
  case 32:
    foectx->widgets.trace_togg = w;
    break;
  case 33:
    foectx->widgets.simulate_togg = w;
    break;
  }
}

static void foe_create_adb ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
/* save the identity of the created widget */

foectx->widgets.adb= w ;
}

static void foe_create_edit_form ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
/* save the identity of the created widget */

foectx->widgets.pane = w ;
/* XtUninstallTranslations( w);*/
}

static void foe_create_subpane ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
/* save the identity of the created widget */

foectx->widgets.subpane = w ;
XtUninstallTranslations( w);
}

static void foe_create_palette_object ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	XmToggleButtonSetState( w, 1, 0);
}



/*************************************************************************
*
* Name:		void	foe_popdown_pop ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the popup menu when poping down.
*
**************************************************************************/

static void	foe_reset_current_node( foectx)
foe_ctx	foectx;
{

	if ( foectx->popupmenu_node != 0)
	{
	  gre_node_unselect( foectx->grectx, foectx->popupmenu_node);
	}
}

static void foe_popdown_pop  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	XtIntervalId		TimerID;

	if ( foectx->popupmenu_node != 0)
	{
	  /* Wait some time and take away select if nobody
	     else has done that */
	  TimerID = XtAppAddTimeOut( 
			XtWidgetToApplicationContext(foectx->cp.parent_wid),
			300, (XtTimerCallbackProc)foe_reset_current_node, foectx);
	}
	NORMAL_CURSOR;
	
}

/*************************************************************************
*
* Name:		void	foe_valchanged_textinput ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback from the menu.
*	Not yet implemented.
*
**************************************************************************/

static void foe_valchanged_textinput  (w, foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	char	*value;
	int	len, sts;

	if ( foectx->widgets.textinput == 0)
	  return;

	value = XmTextGetString( foectx->widgets.textinput);
	if ( *value == 0)
	  return;

	len = strlen( value);
	if ( *(value + len - 1) == 10)
	{
	  /* Return */
	  *(value + len - 1) = 0;
	  strcpy( foectx->searchstring, value);

	  foe_message( foectx, "");
	  XmTextSetString( foectx->widgets.textinput, "");
	  XtUnmanageChild(foectx->widgets.textinput);

	  /* Call the specified function */
	  sts = (foectx->textinput_func) (foectx, value);
	  error_msg( sts);
	}
	XtFree((char *) value);

}


/*************************************************************************
*
* Name:		int	foe_yes_popupmsg()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
*
* Description:
*       callback from the caution widget 
*	call any function interessed by the yes aknowledge from the caution
*       box. The address of this function has been previoulsly carefully saved
*       if the context. ( foectx->Yes_procedure )
* Author : SG
* Date : 13.02.91
*
**************************************************************************/

static void foe_yes_popupmsg ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

	XtUnmanageChild( foectx->widgets.caution);

	if ( foectx->popupmsg_yes_p != NULL)
	  (foectx->popupmsg_yes_p) (foectx ) ;
}


/*************************************************************************
*
* Name:		int	foe_yes_popupmsg()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
*
* Description:
*       callback from the caution widget 
*	call any function interessed by the no aknowledge from the caution
*       box. The address of this function has been previoulsly carefully saved
*       if the context. ( foectx->Yes_procedure )
* Author : SG
* Date : 13.02.91
*
**************************************************************************/

static void foe_no_popupmsg ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{

	XtUnmanageChild( foectx->widgets.caution);

	if ( foectx->popupmsg_no_p != NULL)
	  (foectx->popupmsg_no_p) (foectx ) ;
}


/*************************************************************************
*
* Name:		int	foe_cancel_popupmsg()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
*
* Description:
*       Callback form the caution widget 
*
**************************************************************************/

static void foe_cancel_popupmsg ( w , foectx, data)
Widget	w;
foe_ctx	foectx;
XmAnyCallbackStruct	*data;
{
	XtUnmanageChild( foectx->widgets.caution);

	if ( foectx->popupmsg_cancel_p != NULL)
	  (foectx->popupmsg_cancel_p) (foectx ) ;
}

/*************************************************************************
*
* Name:		foe_register_callbacks
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
*
* Description:
*	This routine registers the callbackfunktions for the 
*	objects in the foe menu.
*
**************************************************************************/

int foe_register_callbacks (
  foe_ctx foectx
)

/* it is supposed that the hierachy is open */
{

        static MrmRegisterArg	reglist[] = {
  	/* First the context variable */
 	{ "foe_ctx", 0 },

	/* Callbacks for the controlled foe widget */
	{"foe_activate_save",(XtPointer)foe_activate_save},
	{"foe_activate_exit",(XtPointer)foe_activate_exit},
	{"foe_activate_print",(XtPointer)foe_activate_print},
	{"foe_activate_printselect",(XtPointer)foe_activate_printselect},
	{"foe_activate_printoverv",(XtPointer)foe_activate_printoverv},
	{"foe_activate_compile",(XtPointer)foe_activate_compile},
	{"foe_activate_syntax",(XtPointer)foe_activate_syntax},
	{"foe_activate_winddelete",(XtPointer)foe_activate_winddelete},
	{"foe_activate_savetrace",(XtPointer)foe_activate_savetrace},
	{"foe_activate_restoretrace",(XtPointer)foe_activate_restoretrace},
	{"foe_activate_quit",(XtPointer)foe_activate_quit},
	{"foe_activate_delete",(XtPointer)foe_activate_delete},
	{"foe_activate_changetext",(XtPointer)foe_activate_changetext},
	{"foe_activate_cut",(XtPointer)foe_activate_cut},
	{"foe_activate_copy",(XtPointer)foe_activate_copy},
	{"foe_activate_paste",(XtPointer)foe_activate_paste},
	{"foe_activate_attribute",(XtPointer)foe_activate_attribute},
	{"foe_activate_subwindow",(XtPointer)foe_activate_subwindow},
	{"foe_activate_undelete",(XtPointer)foe_activate_undelete},
	{"foe_activate_unselect",(XtPointer)foe_activate_unselect},
	{"foe_activate_gridsize1",(XtPointer)foe_activate_gridsize1},
	{"foe_activate_gridsize2",(XtPointer)foe_activate_gridsize2},
	{"foe_activate_gridsize3",(XtPointer)foe_activate_gridsize3},
	{"foe_activate_gridsize4",(XtPointer)foe_activate_gridsize4},
	{"foe_activate_gridsize5",(XtPointer)foe_activate_gridsize5},
	{"foe_activate_showgrid",(XtPointer)foe_activate_showgrid},
	{"foe_activate_zoomin",(XtPointer)foe_activate_zoomin},
	{"foe_activate_zoomout",(XtPointer)foe_activate_zoomout},
	{"foe_activate_unzoom",(XtPointer)foe_activate_unzoom},
	{"foe_activate_redraw",(XtPointer)foe_activate_redraw},
	{"foe_activate_palette_plant",(XtPointer)foe_activate_palette_plant},
	{"foe_activate_palette_object",(XtPointer)foe_activate_palette_object},
	{"foe_activate_palette_con",(XtPointer)foe_activate_palette_con},
	{"foe_activate_refcon",(XtPointer)foe_activate_refcon},
	{"foe_activate_showexeord",(XtPointer)foe_activate_showexeord},
	{"foe_activate_plcattribute",(XtPointer)foe_activate_plcattribute},
	{"foe_activate_search",(XtPointer)foe_activate_search},
	{"foe_activate_searchobject",(XtPointer)foe_activate_searchobject},
	{"foe_activate_searchstring",(XtPointer)foe_activate_searchstring},
	{"foe_activate_searchnext",(XtPointer)foe_activate_searchnext},
	{"foe_activate_getobj",(XtPointer)foe_activate_getobj}, /* sg 11.04.91 */
	{"foe_activate_expand",(XtPointer)foe_activate_expand},
	{"foe_activate_compress",(XtPointer)foe_activate_compress},
	{"foe_activate_helpclass",(XtPointer)foe_activate_helpclass},
	{"foe_activate_help",(XtPointer)foe_activate_help},
	{"foe_activate_trace_togg",(XtPointer)foe_activate_trace_togg},  
	{"foe_activate_simulate_togg",(XtPointer)foe_activate_simulate_togg},  
	{"foe_activate_view_togg",(XtPointer)foe_activate_view_togg},  
	{"foe_activate_edit_togg",(XtPointer)foe_activate_edit_togg},  
	{"foe_create_adb",(XtPointer)foe_create_adb},  /* SG 24.02.91 */
	{"foe_create_widget_cb",(XtPointer)foe_create_widget_cb},  
	{"foe_create_label",(XtPointer)foe_create_label},  
	{"foe_create_textinput",(XtPointer)foe_create_textinput},  
	{"foe_create_edit_form",(XtPointer)foe_create_edit_form},  
	{"foe_create_subpane",(XtPointer)foe_create_subpane},  
	{"foe_create_palette_object",(XtPointer)foe_create_palette_object},  
	{"foe_valchanged_textinput",(XtPointer)foe_valchanged_textinput},  
	{"foe_yes_popupmsg",(XtPointer)foe_yes_popupmsg},
	{"foe_no_popupmsg",(XtPointer)foe_no_popupmsg},
	{"foe_cancel_popupmsg",(XtPointer)foe_cancel_popupmsg},  
	{"foe_popdown_pop",(XtPointer)foe_popdown_pop}
	};

  static int	reglist_num = XtNumber(reglist);

  reglist[0].value = (XtPointer) foectx;

  MrmRegisterNames(reglist, reglist_num);

  return FOE__SUCCESS;
}


/*_Backcall from questionbox___________________________________________*/
/*************************************************************************
*
* Name:		void	foe_exit_save()
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
* Description:
*	Backcall when save is pressed in the questionbox at exit.
**************************************************************************/

static void foe_exit_save(
	  foe_ctx	foectx
)
{
	vldh_t_wind	wind;
	vldh_t_node	parent_node;
	int	sts;

	/* Check that the parent node is saved */
	wind = foectx->grectx->wind;
	parent_node = wind->hw.parent_node_pointer;
	if ( parent_node != 0 )
	{
	  /* Parent is node, not a plc */
	  if ( foectx->cp.parent_ctx != 0 )
	  {
	     /* parent is alive */
	    if ((parent_node->hn.status & VLDH_CREATE) != 0)
	    {
	      /* Parentnode is not saved, then its not allowed to save
	          the window */
              foe_msgbox ( foectx, "Save the parent window first") ;
	      return;
	    }
	  }
	}
	CLOCK_CURSOR;
	foe_disable_ldh_cb(foectx);
	sts = vldh_wind_save( foectx->grectx->wind);
	error_msg( sts);
	
	gre_save(foectx->grectx, 0);

	NORMAL_CURSOR;
	if ( sts == VLDH__PLCNOTSAVED )
	{
          foe_msgbox ( foectx , 
		"Save the plcprogram in the hierarchy editor first") ;
	  return;
	}
	if (EVEN(sts))
	{ 
	  foe_message( foectx,"ERROR   COULDN'T SAVE WINDOW");
	  return;
	}
	foe_exit( foectx);
}


/*************************************************************************
*
* Name:		void	foe_exit_nosave()
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
* Description:
*	Backcall when save is pressed in the questionbox at exit.
**************************************************************************/

static void foe_exit_nosave(
	  foe_ctx	foectx
)
{
	vldh_t_wind		wind;
	pwr_tClassId		class;

	wind = foectx->grectx->wind;

	/*************************************************/
	/* BUGGFIX, quit on suborder windows not allowed */
	/*************************************************/
	ldh_GetObjectClass( wind->hw.ldhses,
			wind->lw.poid,
			&class);
	if ( class == pwr_cClass_order)
	{
	  foe_message( foectx, 
		"Operationen not allowed, do save !!");
	  return;
	}	  
	/**************************************************/
	foe_quit( foectx);
}

static void foe_popup_unmap_cb(Widget w, foe_ctx foectx)
{
  XtDestroyWidget(w);
}


/*************************************************************************
*
* Name:		foe_modify_popup()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  Modifies the popupmenu.
**************************************************************************/

int foe_modify_popup (
	foe_ctx foectx,
	unsigned long	popupmenu_mask
)
{
  static char buttontext[][40] = { "ObjectEditor", "SubWindow", "Connect", "Delete",
			    "Paste", "Copy", "Cut", "PrintSelect", "HelpClass"};
  static XtCallbackProc callback[] = { foe_activate_attribute,
				       foe_activate_subwindow,
				       foe_activate_getobj,
				       foe_activate_delete,
				       foe_activate_paste,
				       foe_activate_copy,
				       foe_activate_cut,
				       foe_activate_printselect,
				       foe_activate_helpclass};

  Widget Menu, w;
  int i;
  Arg ArgList[5]; 
  XmFontList fontlist;
  XFontStruct *font;
  XmFontListEntry fontentry;

  foectx->popupmenu_mask = popupmenu_mask;

  // Set default fontlist
  font = XLoadQueryFont( flow_Display(foectx->widgets.pane),
			 "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
  fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
  fontlist = XmFontListAppendEntry( NULL, fontentry);
  XtFree( (char *)fontentry);

  i = 0;
  XtSetArg(ArgList[i], XmNbuttonFontList, fontlist); i++;
  XtSetArg(ArgList[i], XmNlabelFontList, fontlist); i++;
  Menu = XmCreatePopupMenu( foectx->widgets.pane, "_popup", ArgList, i);  
  XtAddCallback( Menu, XmNunmapCallback, 
		 (XtCallbackProc)foe_popup_unmap_cb, foectx);

  XmFontListFree( fontlist);

  for ( i = 0; i < 9; i++) {
    if ( foectx->popupmenu_mask & (1 << i)) {
      w = XtVaCreateManagedWidget( buttontext[i], xmPushButtonGadgetClass, 
			       Menu, NULL);

      XtAddCallback( w, XmNactivateCallback, (XtCallbackProc) callback[i], 
		    (XtPointer) foectx);
    }
  }
  foectx->widgets.popupmenu = Menu;
  return FOE__SUCCESS ;
}
