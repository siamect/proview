/* wb_foe_methods.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates function object editor and handles
   the graphical editor.
   It contains all the methods defined for the function block editor.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#endif
#include "wb_foe_macros.h"
#include "pwr.h"
#include "wb_foe_api.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#if defined OS_VMS
#include <Xm/DXmHelpB.h>
#endif
#include <Xm/SashP.h>
#include <Xm/ToggleB.h>
#include <Xm/Protocols.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_msg.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_login_msg.h"
#if defined OS_VMS
#include "wb_ate.h"
#endif
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_foe_backcalls.h"
#include "wb_foe_callbacks.h"
#include "wb_gsx.h"
#include "wb_tra.h"
#include "wb_utl.h"
#include "wb_login.h"
#include "co_mrm_util.h"

typedef struct 	{
	  void	*a_ctx;
	  foe_ctx		foectx;
	  vldh_t_node	node;
	} foe_t_attr;

#define	BEEP	    putchar( '\7' );

/* function declarations */

#if defined OS_VMS

static int foe_attr_update ( 
    void	*a_ctx
);
#endif

static int	foe_attrlist_get(
	void	*a_ctx,
	foe_ctx		*foectx,
	vldh_t_node	*node
);

static int	foe_attrlist_delete(
	void	*a_ctx
);

static int	foe_attrlist_insert(
	void	*a_ctx,
	foe_ctx		foectx,
	vldh_t_node	node
);

static int foe_parent_quit (
    foe_ctx	foectx
);

static int foe_child_quit (
	foe_ctx	foectx,
	vldh_t_node	node,
	unsigned long	windowindex
);

static int foe_child_delete ( 
	foe_ctx	foectx,
	vldh_t_node	node,
	vldh_t_wind	subwind
);

static void	foe_destroy(
	foe_ctx	foectx
);

static int foe_init_window ( 
	foe_ctx	foectx
);

#if defined OS_VMS
static void foe_attr_ok ( 
    void	*a_ctx
);

static void foe_attr_save (
	void	*a_ctx
);

static void foe_plcattr_quit (
	void	*a_ctx
);

static void foe_plcattr_ok ( 
	void	*a_ctx
);
#endif

static void foe_attr_quit (
    void	*a_ctx
);

static foe_ctx	foe_create_window(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
        int	x_top ,
        int	y_top ,
        int	width_adb ,
        int	height_adb , 
	ldh_tSesContext	ldhsession ,
	int 	function ,
	int	map_window,
	foe_eFuncAccess	function_access
);

static int foe_view_setup ( 
	foe_ctx foectx
);

static int  foe_edit_setup (
    foe_ctx foectx 
);

static void foe_function_setup (
	foe_ctx foectx
);

static int foe_edit_set_entries (
	foe_ctx foectx
);

static int foe_view_set_entries (
	foe_ctx foectx
);

static void foe_edit_exit_save(
	foe_ctx	foectx
);

static void foe_edit_exit_nosave(
	foe_ctx	foectx
);

static Pixmap icon_pixmap = 0;
static foe_t_attr	*attr_pointer = NULL;
static int		attr_count = 0;

void foe_enable_ldh_cb( foe_ctx foectx)
{
  foectx->ldh_cb_enabled = 1;
}

void foe_disable_ldh_cb( foe_ctx foectx)
{
  foectx->ldh_cb_enabled = 0;
}

static pwr_tStatus foe_ldh_this_session_cb (
  void *ctx,
  ldh_sEvent *event 
)
{
  foe_ctx foectx = (foe_ctx) ctx;
  vldh_t_node	node;
  int		sts;

  if ( !foectx->ldh_cb_enabled)
    return 1;

  while (event) 
  {
    switch (event->Event) 
    {
      case ldh_eEvent_AttributeModified:
      case ldh_eEvent_ObjectRenamed:
      case ldh_eEvent_BodyModified:
        sts = vldh_get_node_objdid( event->Object, 
		foectx->grectx->window_object, &node);
        if ( EVEN(sts)) return 1;

	sts = gre_node_update( foectx->grectx, node);
        break;
      default:
        break;
    }
    event = event->nep;
  }
  return 1;
}

static void foe_enable_set_focus( foe_ctx foectx)
{
  foectx->set_focus_disabled--;
}

static void foe_disable_set_focus( foe_ctx foectx, int time)
{
  foectx->set_focus_disabled++;
  foectx->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( foectx->cp.parent_wid), time,
	(XtTimerCallbackProc)foe_enable_set_focus, foectx);
}

static void foe_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  foe_ctx foectx;

  XtSetArg    (args[0], XmNuserData, &foectx);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( foectx->set_focus_disabled)
    return;

  if ( foectx->nav_palette_managed)
    nav_set_inputfocus( foectx->navctx, 1);
  else if ( foectx->node_palette_managed)
    pal_set_inputfocus( foectx->node_palctx, 1);
  else if ( foectx->con_palette_managed)
    pal_set_inputfocus( foectx->con_palctx, 1);

  foe_disable_set_focus( foectx, 400);
}


/*************************************************************************
*
* Name:		error_msg()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	If the message is a error or warning message it is printed.
*
**************************************************************************/
void error_msg(
	unsigned long 	sts
)
{
	char msg[256];

	if ( EVEN(sts))
	{
	  msg_GetMsg( sts, msg, sizeof(msg));
	  printf("%s\n", msg);
	}
}


/*************************************************************************
*
* Name:		info_msg()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	If the message is a error, warning or info message it is printed.
*
**************************************************************************/
void info_msg (
	unsigned long 	sts
)
{
	char msg[256];

	if ( ( EVEN(sts)) || ((sts & 1) && (sts & 2)) )
	{
	  msg_GetMsg( sts, msg, sizeof(msg));
	  printf("%s\n", msg);
	}
}



/*************************************************************************
*
* Name:		foe_attr_create()
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
*
* Description:
*	Start the attribute editor and store the attribute context pointer 
*	together with foe_ctx and the nodepointer to use when a backcall
*	comes.
*
**************************************************************************/

int foe_attr_create (
	foe_ctx	foectx,
	vldh_t_node	node
)
{
#if defined OS_VMS
	void	*a_ctx;
	vldh_t_wind	wind;
	vldh_t_plc	plc;
	int		ate_mode;
	int		sts;

	ate_mode = (  foectx->function == EDIT );

	if ( node == 0 )
	{	
	  /* This is a plc object calling */
	  wind = foectx->grectx->window_object;
	  plc = wind->hw.plcobject_pointer;

	  a_ctx = ate_edit( foectx, foectx->widgets.foe_window,
	      	"Attribute Editor", 0, plc->lp.objdid, 
	      	plc->hp.ldhsesctx,
	       	NULL, NULL, NULL, NULL, 
		0, TRUE);

	}
	else
	{
	  a_ctx = ate_edit( foectx, foectx->widgets.foe_window,
	      "Attribute Editor", 0, node->ln.object_did, 
	      (node->hn.window_pointer)->hw.ldhsession,
	       foe_attr_ok, foe_attr_save, foe_attr_quit, NULL, ate_mode,
	       TRUE);
	  /* Store in the array */
	  sts = foe_attrlist_insert( a_ctx, foectx, node);
	}
#else
	watt_tCtx	watt;
	vldh_t_wind	wind;
	vldh_t_plc	plc;
	int		ate_mode;
	int		sts;

	ate_mode = ( foectx->function == EDIT );

	if ( node == 0 )
	{	
	  /* This is a plc object calling */
	  wind = foectx->grectx->window_object;
	  plc = wind->hw.plcobject_pointer;

	  watt = watt_new( foectx->widgets.foe_window, foectx, 
	      	plc->hp.ldhsesctx, plc->lp.objdid, 0, 
		foectx->advanced_user, 0, NULL);

	}
	else
	{
          sts = foe_attrlist_get_by_node( node, &watt);
          if ( ODD(sts))
            watt_pop( watt);
          else {
	    watt = watt_new( foectx->widgets.foe_window, foectx, 
	      	(node->hn.window_pointer)->hw.ldhsession,
	      	node->ln.object_did, ate_mode, foectx->advanced_user, 1, 
		foe_attr_quit);

	    /* Store in the array */
	    sts = foe_attrlist_insert( watt, foectx, node);
          }
	}
#endif

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_attrlist_insert()
* Name:		foe_attrlist_get()
* Name:		foe_attrlist_delete()
* Name:		foe_attrlist_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Handles the list of attribute editors in foe.
*	A backcall from the attribute editor is identified by the ate_context.
*	To assosiate this with a foectx and a node the context is stored in
*	the attribute list when ate is created.
*
**************************************************************************/

static int	foe_attrlist_get(
	void	*a_ctx,
	foe_ctx		*foectx,
	vldh_t_node	*node
)
{
	int	i;

	for ( i = 0; i < attr_count; i++)
	{
	  if ( (attr_pointer + i)->a_ctx == a_ctx)
	  {
	    /* This is it */
	    *foectx = (attr_pointer + i)->foectx;
	    *node = (attr_pointer + i)->node;
	    return FOE__SUCCESS;
	  }
	}
	return FOE__OBJECT;
}

int	foe_attrlist_get_by_node(
	vldh_t_node	node,
	void	**a_ctx
)
{
	int	i;

	for ( i = 0; i < attr_count; i++)
	{
	  if ( (attr_pointer + i)->node == node)
	  {
	    /* This is it */
	    *a_ctx = (attr_pointer + i)->a_ctx;
	    return FOE__SUCCESS;
	  }
	}
	return FOE__OBJECT;
}

static int	foe_attrlist_insert(
	void	*a_ctx,
	foe_ctx		foectx,
	vldh_t_node	node
)
{
	int		sts;
	foe_ctx		dum_foectx;
	vldh_t_node	dum_node;

	/* Check that it doesn't exist */
	sts = foe_attrlist_get( a_ctx, &dum_foectx, &dum_node);
	if ( EVEN(sts))
	{
	  sts = utl_realloc( (char **)&attr_pointer, 
		attr_count * sizeof( foe_t_attr), 
	 	(attr_count + 1) * sizeof( foe_t_attr));
	  if (EVEN(sts)) return sts;

	  (attr_pointer + attr_count)->a_ctx = a_ctx;
	  (attr_pointer + attr_count)->foectx = foectx;
	  (attr_pointer + attr_count)->node = node;
	  attr_count++;	
	}
	return FOE__SUCCESS;
}

static int	foe_attrlist_delete(
	void	*a_ctx
)
{
	int		i;
	int		found;
	int		size;
	foe_t_attr	*item_ptr;
	foe_t_attr	*next_item_ptr;

	/* Find this atectx in the attrlist */
	found = 0;
	for ( i = 0; i < attr_count; i++)
	{
	  if ( (attr_pointer + i)->a_ctx == a_ctx)
	  {
	    /* This is it */
	    found = 1;
	    break;
	  }
	}
	if ( !found )
	  return FOE__OBJECT;

	item_ptr = attr_pointer + i;
	next_item_ptr = item_ptr + 1;
	size = (attr_count - i - 1) * sizeof( foe_t_attr);
	memcpy( item_ptr, next_item_ptr, size);
	attr_count--;

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_attr_update()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
*
* Description:
*	Gets a backcall from the attributeditor when something is changed.
*	The function fetches the foectx and nodepointer and calls a gre
*	functions to redraw the node.
*
**************************************************************************/

#if defined OS_VMS
static int foe_attr_update ( 
    void	*a_ctx
)
{
	vldh_t_node	node;
	foe_ctx		foectx;
	int		sts;
	
	/* Get the attribute context in the array */
	sts = foe_attrlist_get( a_ctx, &foectx, &node);
	if ( EVEN(sts)) return sts;
	
	/* Update the node */
	if ( foectx != 0)
	{
	  sts = gre_node_update( foectx->grectx, node);
	  if( EVEN(sts)) return sts;
	}

	return FOE__SUCCESS;
}
#endif

/*************************************************************************
*
* Name:		foe_attr_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
*
* Description:
*	Delete all attribute editors and remove them from the
*	attribute list.
*
**************************************************************************/

int foe_attr_delete (
	foe_ctx	foectx
)
{
	int i;
	
	/* Get the foectx in the attributectx array and reset it*/
	for ( i = 0; i < attr_count; i++ )
	{
	  if ( foectx == (attr_pointer + i)->foectx)
	  {
#if defined OS_VMS
            ate_delete( (attr_pointer + i)->a_ctx);
#else
            watt_delete( (attr_pointer + i)->a_ctx);
#endif	    
	    foe_attrlist_delete( (attr_pointer + i)->a_ctx);
	  }
	}
	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_parent_quit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
*
* Description:
*	Tell all the subwindows on the screen that I am quitting now
*	by setting the parentctx in foectx zero.
*
**************************************************************************/

static int foe_parent_quit (
    foe_ctx	foectx
)
{
	unsigned long		wind_count;
	vldh_t_wind		*windlist;
	vldh_t_wind		*wind_ptr;
	int			i;
	int			sts;
	foe_ctx			subw_foectx;

	/* Get all the subwindows and reset parentctx */
	sts = vldh_get_wind_subwindows( foectx->grectx->window_object, 
			&wind_count, &windlist);
	if( EVEN(sts)) return sts;
	wind_ptr = windlist;

	for ( i = 0; i < wind_count; i++)
	{
	  subw_foectx = (foe_ctx)(*wind_ptr)->hw.foectx;
	  subw_foectx->cp.parent_ctx = 0;
	  wind_ptr++;
	}
	if ( wind_count > 0) XtFree((char *) windlist);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_child_quit()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	windowindex	I	window index.
*
* Description:
*	Tell the parent of a subwindow that I am quitting by setting the 
*	subwindow pointer in the nodeobject to zero. 
*	Redraw the node if the window isn't saved.
*
**************************************************************************/

static int foe_child_quit (
	foe_ctx	foectx,
	vldh_t_node	node,
	unsigned long	windowindex
)
{
	int	sts;

	/* If the parent is not active there is nothing to do */
	if ( foectx == 0) return FOE__SUCCESS;

	/* Check if the window is created and not saved, 
	   if it is saved don't do anything */

	if ( ((node->hn.subwindowobject[ windowindex ])->hw.status &
		VLDH_CREATE) != 0 )
	{
	  /* The subwindow in the node will not be saved until the
	     subwindow itself is saved so just take away all 
	     tracks of the window and redraw the node, don't care
	     about ldh */

	  if ( node->ln.subwindow & ( windowindex + 1 ) )
	    node->ln.subwindow -= ( windowindex + 1 );
	  node->ln.subwind_objdid[ windowindex ] = pwr_cNObjid;

	  /* Redraw without subwindowmark */
	  sts = gre_subwindow_mark( foectx->grectx, node);
	  if(EVEN(sts)) return sts;
	}

	/* Indicate that the subwindow quits by setting the
	   subwindowpointer to zero */
	node->hn.subwindowobject[ windowindex ] = 0;


	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_child_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* vldh_t_wind	subwind		I	vldh window.
*
* Description:
*	Tell the parent that I am deleteing myself. If the parent is
*	alive take away all tracks of the subwindow. If the subwindow
*	is created and not saved thats all, byt if it is saved it should
*	also be taken away in ldh.
*
**************************************************************************/

static int foe_child_delete ( 
	foe_ctx	foectx,
	vldh_t_node	node,
	vldh_t_wind	subwind
)
{
	vldh_t_plc		plc;
	int			sts, size;
	pwr_sPlcNode		*nodebuffer;
	pwr_tClassId		class;
	ldh_tSesContext		ldhsession;
	unsigned long		windowindex;

	windowindex = subwind->lw.subwindowindex;
	
	if ( foectx != 0 )
	{
	  /* Parent is alive, take away in vldh and on the screen */
	  node->hn.subwindowobject[ windowindex ] = 0;
	  if ( (node->ln.subwindow & (1 << windowindex)) != 0 )
	    node->ln.subwindow -= ( 1 << windowindex );
	  node->ln.subwind_objdid[ windowindex ] = pwr_cNObjid;

	  /* Redraw without subwindowmark */
	  gre_subwindow_mark( foectx->grectx, node);
	}

	if ( ( subwind->hw.status & VLDH_CREATE) != 0 )
	{
	  /* The subwindow is not saved so we don't have to worry
	     about ldh */
	}
	else
	{
	  /* The subwindow should be taken away in ldh,
	     open a new session for this purpos */ 
	  plc = subwind->hw.plcobject_pointer;
	  sts = ldh_OpenSession(&ldhsession, 
		ldh_SessionToVol( plc->hp.ldhsesctx),
		ldh_eAccess_ReadWrite, ldh_eUtility_PlcEditor);
	  if( EVEN(sts)) return sts;
	  sts = ldh_GetObjectBuffer( ldhsession,
			subwind->lw.parent_node_did, "DevBody", "PlcNode", 
			(pwr_eClass *) &class,	
			(char **)&nodebuffer, &size);
	  if( EVEN(sts)) return sts;
	  if ( (nodebuffer->subwindow & (windowindex + 1)) != 0 )
	      nodebuffer->subwindow -= (windowindex + 1);
	  if ( (nodebuffer->subwindow & (1 << windowindex)) != 0 )
	    nodebuffer->subwindow -= ( 1 << windowindex );
	  nodebuffer->subwind_objdid[ windowindex ] = pwr_cNObjid;
	  sts = ldh_SetObjectBuffer(
			ldhsession,
			subwind->lw.parent_node_did,
			"DevBody",
			"PlcNode",
			(char *)nodebuffer);
	  if( EVEN(sts)) return sts;
	  XtFree((char *) nodebuffer);
	  sts = ldh_SaveSession(ldhsession);
	  sts = ldh_CloseSession(ldhsession);
	}
	return FOE__SUCCESS;

}

/*************************************************************************
*
* Name:		foe_destroy()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description:
*	Destroys a foe instance.
*	Destroys the widget and frees allocated memory for the 
*	context.
*
**************************************************************************/

static void	foe_destroy(
	foe_ctx	foectx
)
{
        if ( foectx->set_focus_disabled)
          XtRemoveTimeOut( foectx->focus_timerid);

	/* Delete controled modules */
	gre_del( foectx->grectx);
	pal_del( foectx->node_palctx);
	pal_del( foectx->con_palctx);
	nav_del( foectx->navctx);

	/* Destroy the widget */
/*
        XtAppAddWorkProc ( XtWidgetToApplicationContext(foectx->cp.parent_wid),
			(XtWorkProc)XtDestroyWidget, foectx->cp.parent_wid) ;
*/
	XtDestroyWidget( foectx->cp.parent_wid);

	/* Delete the context */
	XtFree((char *) foectx);
}

/*************************************************************************
*
* Name:		foe_quit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description:
*	Is called when quit is activated and confirmed.
*	Tells the parent and children that I am quitting
*	and destroys myself...
*
**************************************************************************/

void	foe_quit(
	foe_ctx	foectx
	)
{
	vldh_t_wind	wind;
	int		sts;

	/* Tell my parent that his child is quitting, if parent is a node */
	wind = foectx->grectx->window_object;
	if ( wind->hw.parent_node_pointer != 0 )
	  foe_child_quit ( foectx->cp.parent_ctx, wind->hw.parent_node_pointer, 
			wind->lw.subwindowindex);

	/* Get my children and tell them that their parent is quitting */
	sts = foe_parent_quit ( foectx);
	error_msg(sts);

	/* Avoid any pending backcall from a attribute editor */
	foe_attr_delete(foectx);

	/* Delete window in vldh */
	sts = vldh_wind_quit_all( foectx->grectx->window_object );
	error_msg(sts);

	/* Destroy the widget and controlled modules */
	foe_destroy( foectx);
}

/*************************************************************************
*
* Name:		foe_exit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description:
*	Is called when exit is activated and confirmed.
*	Tells the parent and children that I am exiting
*	and destroys myself...
*
**************************************************************************/

void	foe_exit(
	foe_ctx	foectx
	)
{
	int		windowindex;
	vldh_t_wind	wind;
	vldh_t_node	parent_node;
	int		sts;


	/* Tell my parent that his child is quitting, if parent is a node */
	wind = foectx->grectx->window_object;
	windowindex = wind->lw.subwindowindex;
	parent_node = wind->hw.parent_node_pointer;
	if ( parent_node != 0 )
	  if ( foectx->cp.parent_ctx != 0)
	    parent_node->hn.subwindowobject[ windowindex ] = 0;

	/* Get my children and tell them that their parent is exiting */
	sts = foe_parent_quit ( foectx);
	error_msg(sts);

	/* Avoid any pending backcall from a attribute editor */
	foe_attr_delete(foectx);

	/* Delete window in vldh */
	vldh_wind_quit( foectx->grectx->window_object );

	/* Destroy the widget and controlled modules */
	foe_destroy( foectx);
}

/*************************************************************************
*
* Name:		foe_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description:
*	Is called when window delete is activated and confirmed.
*	Tells the parent and children that I am to delete myself,
*	and do so.
*
**************************************************************************/

void	foe_delete(
	foe_ctx	foectx
	)
{
	vldh_t_wind	wind;
	int		sts;

	/* Tell my parent that his child is deleted, if parent is a node */
	wind = foectx->grectx->window_object;
	if ( wind->hw.parent_node_pointer != 0 )
	{
	  sts = foe_child_delete ( foectx->cp.parent_ctx, 
			wind->hw.parent_node_pointer, 
			wind);
	  error_msg(sts);
	}

	/* Avoid any pending backcall from a attribute editor */
	foe_attr_delete(foectx);

	if ( (wind->hw.status & VLDH_CREATE) != 0 ) 
	{
	  /* The window is not saved in ldh, just quit in vldh */
	  sts = vldh_wind_quit_all(wind);
	  error_msg(sts);
	}
	else
	{
	  /* Delete in ldh as well */	
	  sts = vldh_wind_delete_all( wind );
	  error_msg(sts);
	}

	/* Destroy the widget and controlled modules */
	foe_destroy( foectx);
}

/*************************************************************************
*
* Name:		foe_init_window()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
*
* Description:
*	Initiates a window when it is created.
*	Creates the default nodes for the windows specified in
*	graphbody of the classobject.
*
**************************************************************************/

static int foe_init_window ( 
	foe_ctx	foectx
)
{
	vldh_t_wind		wind;
	pwr_sGraphPlcWindow 	*graphbody;
	pwr_tClassId		bodyclass;
	int			i, sts, size;
	vldh_t_node		node;

 	/* Create the nodes in the window class object */
	/* Get the classinformation for this window */
	wind = foectx->grectx->window_object;

	sts = ldh_GetClassBody( wind->hw.ldhsession, wind->lw.classid, 
		"GraphPlcWindow", &bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return sts;

	/* Create the nodes specified there */
	for ( i = 0; i < graphbody->defaultobjects; i++)
	{
	  sts = gre_create_node(foectx->grectx, 
			graphbody->defobj_class[i],
	  		graphbody->defobj_x[i],
	  		graphbody->defobj_y[i], &node);
	}
	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		foe_subwindow_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	object		I	vldh node.
* unsigned long	subwindow_nr	I	window index.
*
* Description:
*	Creates a subwindow.
*	Calls the module foe_gsx to check sytax and return a suitable
*	window index.
*	
*
**************************************************************************/

int foe_subwindow_create (
	foe_ctx		foectx,
	vldh_t_node	object,
	unsigned long	subwindow_nr
)
{
	foe_ctx			newfoectx;
	int			sts, size;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
	int			subwindow_count;
	unsigned long		new_window;
	vldh_t_plc		plcobject;
	vldh_t_wind		subwind;
	foe_eFuncAccess		function_access;

	/* Check if this is a correct window to create */
	sts = gsx_check_subwindow( object, &subwindow_nr, &function_access);
 
	/* Check if the window exists */
	if ( object->hn.subwindowobject[ subwindow_nr - 1 ] != 0 )
	{
	  /* Yes, there is already a window */
	  foe_message( foectx, "Window already exists");
          /* GS 11.04.91 Little trick to put the window	
	  on top of the stack in any case ( ie to make is visible ) */
	
	  subwind = object->hn.subwindowobject[ subwindow_nr - 1 ];
	  newfoectx = (foe_ctx)subwind->hw.foectx;
	  XtUnmapWidget( newfoectx->cp.parent_wid );
	  XtMapWidget( newfoectx->cp.parent_wid );
	  return FOE__WINDEXIST;
	}

	/* Get graphbody for the class */
	sts = ldh_GetClassBody( (object->hn.window_pointer)->hw.ldhsession, 
		object->ln.classid, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if (EVEN(sts)) return sts;

	/* Get number of subwindows for this object */
	subwindow_count = graphbody->subwindows;

	if ( subwindow_count == 0 )
	{
	  /* Sorry, no subwindow in this class, message and return */
	  foe_message( foectx, "No subwindow in this object");
	  BEEP;
	  return FOE__SUCCESS;
	}

	if( subwindow_count == 1 )
	{
	  /* Get the subwindow type */
	  
	  /* Is this a new window ? */
	  if ( object->ln.subwindow == 0 )
	    new_window = TRUE;
	  else
	    new_window = FALSE;

	  /* Create subwindow */
	  plcobject = (object->hn.window_pointer)->hw.plcobject_pointer;
	  sts = foe_new_local( foectx, foectx->widgets.foe_window,
		object->hn.name, pwr_cNObjid, 0, plcobject->hp.ldhsesctx,
		object, 0, new_window,
		&newfoectx, foectx->map_window, foectx->access, function_access);
	  if ( sts == FOE__WINDNOTFOUND)
	  {
	    foe_message( foectx, "Subwindow does not exist");
	    BEEP;
	    return FOE__SUCCESS;
	  }
	  else if ( EVEN(sts)) return sts;

	  /* Draw subwindowmark in node */
	  if ( new_window )
	    gre_subwindow_mark( foectx->grectx, object);
	}
	else if ( subwindow_count == 2 )
	{
	  /* Is this a new window ? */
	  if ( (object->ln.subwindow & subwindow_nr) == 0 )
	    new_window = TRUE;
	  else
	    new_window = FALSE;

	  /* Create subwindow */
	  plcobject = (object->hn.window_pointer)->hw.plcobject_pointer;
	  sts = foe_new_local( foectx, foectx->widgets.foe_window,
		object->hn.name, pwr_cNObjid, 0, plcobject->hp.ldhsesctx, 
		object, (subwindow_nr - 1),
		new_window, &newfoectx, foectx->map_window, foectx->access,
		function_access);
	  if ( sts == FOE__WINDNOTFOUND)
	  {
	    foe_message( foectx, "Subwindow does not exist");
	    BEEP;
	    return FOE__SUCCESS;
	  }
	  else if ( EVEN(sts)) return sts;

	  /* Draw subwindowmark in node */
	  if ( new_window )
	    gre_subwindow_mark( foectx->grectx, object);
	}
	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_message()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* char 		*new_label	I	message to show. 
*
* Description:
*	Displays a message in the foe window.
*	Changes the label of a label widget which id is stored in the context.
* Author: SG
* Date : 25.01.91 
*    	To avoid unnecessary call to this routine the context variable :
*     	foectx.msg_label_id is set to null if the the message to be displayed 
*     	is "" . This case occurs when you want to erase a previous message.
*
**************************************************************************/

void foe_message ( 
	foe_ctx	foectx,
	char *new_label 
) 
{

	Arg		args[2];
	int 		l;
	XmString	cstr;

	gre_search_rectangle_delete( foectx->grectx);
	l= strlen (new_label);
        foectx->msg_label_id = ( l !=0 );
        if ( l == 0 ) new_label = " " ;
	XtSetArg(args[0], XmNlabelString, 
		cstr=XmStringCreateLtoR(new_label , "ISO8859-1"));
	XtSetValues( foectx->widgets.label ,args,1);
	XmStringFree( cstr);
}

/*************************************************************************
*
* Name:		foe_msgbox()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* char 		*new_label	I	message to be sent.
*
* Description:
* This routine is called by any one who wants to send a little message.
* in a ( modal) message box. No callback is expected. We just want to
* be sure that the user see the message.
* Author: SG
* Date : 24.02.91
*
**************************************************************************/

void foe_msgbox ( 
	foe_ctx	foectx,
	char *new_label
) 
{

	Arg		args[10];
	int 		i;
	XmString	cstr;

        i=0;
	XtSetArg(args[0], XmNmessageString,
		cstr=XmStringCreateLtoR(new_label , "ISO8859-1") ); i++;
	XtSetValues( foectx->widgets.msgbox ,args,i);
	XmStringFree( cstr);
        XtManageChild (foectx->widgets.msgbox);

}


/*************************************************************************
*
* Name:		void foe_popupmsg()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* char 		*msg		I	msg to be sent
* voidp         yes_procedure   I       procedure interessed by a yes.
*
* Description:
* This routine is called by any one who wants to send a little caution message.
* The calling routine send also the address of the function it want to
*  be executed when the yes button is pressed in the caution box.
* Author: SG
* Date : 13.02.91
*
**************************************************************************/

void foe_popupmsg ( 	
	foe_ctx	foectx,
	char *new_label,
	voidp yes_procedure,
	voidp no_procedure,
	voidp cancel_procedure
) 
{

	Arg		args[10];
	int 		i;
	XmString	cstr;

	cstr = XmStringCreateLtoR(new_label , "ISO8859-1");
        i=0;
	XtSetArg(args[0], XmNmessageString, cstr); i++;
	XtSetValues( foectx->widgets.caution ,args,i);
        XtManageChild (foectx->widgets.caution);
	XmStringFree( cstr);

        foectx->popupmsg_yes_p = yes_procedure ;        
        foectx->popupmsg_no_p = no_procedure ;        
        foectx->popupmsg_cancel_p = cancel_procedure ;        
}


/*_Callbacks_____________________________________________________________*/

/*************************************************************************
*
* Name:		foe_attr_ok()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* void	*a_ctx		I	attribute editor ctx
*
*
* Description:
*	Callback from the attribute editor when the ok button is pressed.
*
**************************************************************************/

#if defined OS_VMS
static void foe_attr_ok ( 
    void	*a_ctx
)
{
	int	sts;

	/* Redraw the node if anything was changed */
	sts = foe_attr_update( a_ctx);
	error_msg(sts);

	/* Delete from attribute list */
	sts = foe_attrlist_delete( a_ctx);
	error_msg(sts);

	/* Delete the attribute editor */
        ate_delete(a_ctx);
}
#endif

/*************************************************************************
*
* Name:		foe_attr_save()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* void	*a_ctx		I	attribute editor ctx
*
* Description:
*	Callback from the attribute editor when the exit button is pressed.
*	Redraws the node if anything has been changed.
*
**************************************************************************/

#if defined OS_VMS
static void foe_attr_save (
	void	*a_ctx
)
{
	int	sts;

	/* Redraw the node if anything was changed */
	sts = foe_attr_update( a_ctx);
	error_msg(sts);

}
#endif

/*************************************************************************
*
* Name:		foe_plcattr_quit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* void	*a_ctx		I	attribute editor ctx
*
* Description:
*	Backcall from the attribute editor when the quit button is pressed.
* 	and when the editor was called on an plc( not on a node ).
*	This backcall is not used in foe.
* Author: SG 19.03.91
**************************************************************************/

#if defined OS_VMS
static void foe_plcattr_quit (
	void	*a_ctx
)
{
	int	sts;

	/* Delete from attribute list */
	sts = foe_attrlist_delete( a_ctx);
	error_msg(sts);
}
#endif

/*************************************************************************
*
* Name:		foe_plcattr_ok()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* void	*a_ctx		I	attribute editor ctx
*
* Description:
*	Callback from the attribute editor when the OK or spara 
*	button are pressed and when the editor was called on a plc object (
* 	not on a node ).
* Author:  SG 19.03.91
**************************************************************************/

#if defined OS_VMS
static void foe_plcattr_ok ( 
	void	*a_ctx
)
{
}
#endif

/*************************************************************************
*
* Name:		foe_attr_quit()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* void	*a_ctx		I	attribute editor ctx
*
* Description:
*	Backcall from the attribute editor when the quit button is pressed.
*	This backcall is not used by foe.
*
**************************************************************************/

static void foe_attr_quit (
    void	*a_ctx
)
{
#if defined OS_VMS
	int	sts;

	/* Delete from attribute list */
	sts = foe_attrlist_delete( a_ctx);
	error_msg(sts);

	/* Delete the attribute editor */
        ate_delete(a_ctx);
#else
	int	sts;

	/* Delete from attribute list */
	sts = foe_attrlist_delete( a_ctx);
	error_msg(sts);

	/* Delete the attribute editor */
        watt_delete(a_ctx);
#endif
}

/*************************************************************************
*
* Name:		foe_new_local()
*
* Type		foe_ctx
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	parent context
* Widget	parent_wid	I	parent widget
* char 		*name		I	name of the created foe instance
* pwr_tObjid	plcprogram	I	objdid of plcpgm.
* ldh_tWBContext ldhwbctx	I	ldh workbench.
* ldh_tSesContext ldhsesctx	I	ldh session.
* vldh_t_node	nodeobject	I	vldh node of parent object.
* unsigned long	windowindex	I	windowindex of window.
* unsigned long	new_window	I	True if brandnew window, else
*					window is loaded from ldh.
* foe_ctx	*return_foectx	O	created foe context. 
*
* Description:
*	Create a new foe object
* SG : 09.02.91  Define a pixmap for the icon
* SG : 24.02.91  change the call to create_window
**************************************************************************/

int foe_new_local(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjid	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	vldh_t_node	nodeobject,
	unsigned long	windowindex,
	unsigned long	new_window,
	foe_ctx		*return_foectx, 
	int		map_window,
	ldh_eAccess	access,
	foe_eFuncAccess	function_access
	)
{
	foe_ctx		foectx , old_foectx ;
	pwr_tObjid	windowobjdid;
	vldh_t_plc	plcobject;
	vldh_t_wind 	windowobject;
	pwr_tClassId	wind_class;
	int		sts, size;
	pwr_tClassId	bodyclass;
	pwr_sGraphPlcProgram *plcgraphbody;
	pwr_sGraphPlcNode *nodegraphbody;
	pwr_sGraphPlcWindow *windgraphbody;
	char		*windbuffer;
	char		*plcbuffer;
	pwr_tFullName	subwind_name;
	int		window_found;
	pwr_eClass	windclass;
	pwr_tClassId	plcclass;
	int		plc_loaded;
	int		function;

	if ( cdh_ObjidIsNotNull( plcprogram))
	{
	  if ( !ldh_LocalObject( ldhsesctx, plcprogram))
	    function_access = foe_eFuncAccess_View;

	  /* This is probably a plcprogram, check that it really is */
	  sts = ldh_GetObjectBuffer( ldhsesctx,
			plcprogram, "DevBody", "PlcProgram", (pwr_eClass *)&plcclass,	
			&plcbuffer, &size);
	  if( EVEN(sts)) 
	    /* This is not a plcprogram object, return */
	    return FOE__NOPLC;

	  XtFree((char *) plcbuffer);

	  sts = ldh_GetObjectClass(ldhsesctx, plcprogram, &plcclass);
	  if ( EVEN(sts)) return sts;

	  /* Get the windowtype for this plcprogram */
	  sts = ldh_GetClassBody(ldhsesctx, plcclass, 
		"GraphPlcPgm", &bodyclass, (char **)&plcgraphbody, &size);
	  if ( EVEN(sts)) return sts;
	  wind_class = plcgraphbody->subwindow_class; 

	  /* Get graphbody for the window */
	  sts = ldh_GetClassBody(ldhsesctx, 
			wind_class, "GraphPlcWindow", &bodyclass, 
			(char **)&windgraphbody, &size);
	  if ( EVEN(sts)) return sts;

	  /* Check if this plcprogram already is loaded in vldh */
	  plc_loaded = 0;
	  sts = vldh_get_plc_objdid( plcprogram, &plcobject);
	  if ( ODD(sts) )
	  {
	    /* Loaded in vldh, do nothing */
	    foectx = (foe_ctx)(plcobject->hp.windowobject)->hw.foectx;
	    if ( foectx != 0)
	    {
              /* GS 11.04.91 Little trick to put the window	
	      on top of the stack ( ie to make is visible ) in any case */
	      XtUnmapWidget( foectx->cp.parent_wid );
	      XtMapWidget( foectx->cp.parent_wid );
	      return FOE__WINDEXIST;
	    }
	    else
	      plc_loaded = 1;
	  }
	  if ( (sts == VLDH__OBJNOTFOUND) || plc_loaded )
	  {
	    /* Check if windowobject in ldh exists */
	    /* Get the first child to the plc */
	    sts = ldh_GetChild( ldhsesctx, plcprogram, &windowobjdid);
	    window_found = 0;
	    while ( ODD(sts) )
	    {
	      /* Check if window */
	      sts = ldh_GetObjectBuffer( ldhsesctx,
			windowobjdid, "DevBody", "PlcWindow", &windclass,	
			&windbuffer, &size);
	      if ( ODD(sts)) 
	      {
	        XtFree((char *) windbuffer);
	        window_found = 1;	
		break;
	      }
	      sts = ldh_GetNextSibling( ldhsesctx, windowobjdid, &windowobjdid);
	    }

	    if ( window_found )
	    {
	      /* Window exists in ldh, load the plcprogram to vldh */
	      if ( !plc_loaded)
	      {
	        sts = vldh_plc_load( plcprogram, ldhwbctx, ldhsesctx,
				&plcobject);
	        if ( EVEN(sts)) return sts;
	      }
	      sts = vldh_get_wind_objdid( windowobjdid, &windowobject);
	      if ( sts == VLDH__OBJNOTFOUND )
	      {
	        sts = vldh_wind_load( plcobject, 0, windowobjdid, 0,
			 0, &windowobject, access);
	        if ( EVEN(sts)) return sts;

	        sts = vldh_wind_load_all( windowobject);
	        if ( EVEN(sts)) return sts;
	      }
	      /* Create the foe context */
	      /* SG 02.05.91 by default give the view function */
	      foectx = foe_create_window(parent_ctx, parent_wid, name,
		0, 50, 920, 750, windowobject->hw.ldhsession, VIEW, map_window,
		function_access);
	      if ( foectx == 0) return 0;

	      foectx->grectx->window_object = windowobject;
	      windowobject->hw.foectx = foectx;
	      windowobject->hw.widget_id = foectx->grectx->flow_widget;
	      gre_window_draw( foectx->grectx);

	      foectx->grectx->window_object = windowobject;
	      plcobject->hp.windowobject = windowobject;
	      plcobject->hp.hinactx = parent_ctx;
	      foectx->plcobject = plcobject;
	      windowobject->hw.foectx = foectx;	    	    
	    }
	    else
	    {
	      /* This is a brandnew plcprogram */

	      /* Check authorization to open a new plcpgm */
	      if ( !(login_prv.priv & pwr_mPrv_DevPlc ))
  	      {
		return LOGIN__USERNOTAU;
	      }
	      access = ldh_eAccess_ReadWrite;

	      sts = vldh_plc_create( plcprogram, ldhwbctx, ldhsesctx,
				&plcobject);
	      if ( EVEN(sts)) return sts;

	      /* Create window object in vldh */
	      sts = vldh_wind_create(plcobject, 0, 0, wind_class,
			0, 0,
			&windowobject, access);
	      if ( EVEN(sts))
	      {
		vldh_plc_delete(plcobject);
	        return sts;
	      }

	      /* Create the foe context */
	      foectx = foe_create_window(parent_ctx, parent_wid, name,
		windgraphbody->x, windgraphbody->y, 
		windgraphbody->width, windgraphbody->height, 
		windowobject->hw.ldhsession, EDIT, map_window, function_access);
	      if ( foectx == 0) return 0;

	      foectx->grectx->window_object = windowobject;
	      windowobject->hw.foectx = foectx;
	      windowobject->hw.widget_id = foectx->grectx->flow_widget;
	      windowobject->lw.x = windgraphbody->x;
	      windowobject->lw.y = windgraphbody->y;
	      windowobject->lw.width = windgraphbody->width;
	      windowobject->lw.height = windgraphbody->height;
	      plcobject->hp.windowobject = windowobject;
	      plcobject->hp.hinactx = parent_ctx;
	      foectx->plcobject = plcobject;
	      windowobject->hw.foectx = foectx;	    	    

	      sts = foe_init_window( foectx);
	      if ( EVEN(sts)) return sts;

	    }
	  }
	}
	else
	{
	  /* This is a subwindow to a node in gre */
	  plcobject = (nodeobject->hn.window_pointer)->hw.plcobject_pointer;
	  /* Get the windowtype for this plcprogram */
	  sts = ldh_GetClassBody(plcobject->hp.ldhsesctx, 
			nodeobject->ln.classid, 
			"GraphPlcNode", &bodyclass, (char **)&nodegraphbody, &size);
	  if ( EVEN(sts)) return sts;
	  wind_class = nodegraphbody->subwindow_class[windowindex];

	  /* Get graphname for the window and add to the nodename */
	  sts = ldh_GetClassBody(plcobject->hp.ldhsesctx, 
			wind_class, 
			"GraphPlcWindow", &bodyclass, (char **)&windgraphbody, &size);
	  if ( EVEN(sts)) return sts;
	  sprintf(subwind_name, "%s  %s", nodeobject->hn.name, 
			windgraphbody->graphname);

	  if ( new_window )
	  {
	    if ( access == ldh_eAccess_ReadOnly ||
		 function_access == foe_eFuncAccess_View ||
		 !ldh_LocalObject( plcobject->hp.ldhsesctx, 
			nodeobject->ln.object_did))
	      /* A subwindow can not be created */
	      return FOE__WINDNOTFOUND;

	    /* Create the foe module */
	    old_foectx = parent_ctx ;
	    function = old_foectx->function;
	    if ( function_access == foe_eFuncAccess_View)
	    {
	      if ( function == EDIT)
	         function = VIEW;
	      if ( function == SIMULATE)
	         function = TRACE;
	    }
	    /* Create window object in vldh */
	    sts = vldh_wind_create(
			(nodeobject->hn.window_pointer)->hw.plcobject_pointer,
			nodeobject, 0, wind_class,
			0, windowindex,
			&windowobject, access);
	    if ( EVEN(sts)) return sts;

	    foectx = foe_create_window(parent_ctx, parent_wid,
		subwind_name,
		windgraphbody->x, windgraphbody->y, 
		windgraphbody->width, windgraphbody->height, 
		windowobject->hw.ldhsession , function , map_window, 
		function_access );
	    if ( foectx == 0) return 0;

	    vldh_node_subwindow_created( nodeobject, windowobject, windowindex);
	    foectx->grectx->window_object = windowobject;
	    windowobject->hw.foectx = foectx;
	    windowobject->hw.widget_id = foectx->grectx->flow_widget;
	    foectx->plcobject = 0;
	    windowobject->hw.foectx = foectx;	    	    
	    windowobject->lw.x = windgraphbody->x;
	    windowobject->lw.y = windgraphbody->y;
	    windowobject->lw.width = windgraphbody->width;
	    windowobject->lw.height = windgraphbody->height;

	    sts = foe_init_window( foectx);
	    if ( EVEN(sts)) return sts;
	  }
	  else
	  {
	    /* Subwindow already exists, check if window exists */
	    if ( nodeobject->hn.subwindowobject[ windowindex ] == 0 )
	    {
	      /* No, create new foe window and load the window */	
	      old_foectx = parent_ctx ;

	      sts = vldh_get_wind_objdid( 
		nodeobject->ln.subwind_objdid[windowindex], &windowobject); 
	      if ( sts == VLDH__OBJNOTFOUND )
	      {
	        sts = vldh_wind_load( 
			(nodeobject->hn.window_pointer)->hw.plcobject_pointer, 
			nodeobject, 
			nodeobject->ln.subwind_objdid[windowindex], 0,
			0, &windowobject, access);
	        if ( EVEN(sts)) return sts;

	        sts = vldh_wind_load_all( windowobject);
	        if ( EVEN(sts)) return sts;
	      }

	      function = old_foectx->function;
	      if ( function_access == foe_eFuncAccess_View)
	      {
	        if ( function == EDIT)
	           function = VIEW;
	        if ( function == SIMULATE)
	           function = TRACE;
	      }
	      foectx = foe_create_window(parent_ctx, parent_wid,
			subwind_name,
			windgraphbody->x, windgraphbody->y, 
			windgraphbody->width, windgraphbody->height, 
			windowobject->hw.ldhsession , function, map_window,
			function_access);
	      if ( foectx == 0) return 0;

	      foectx->grectx->window_object = windowobject;
	      gre_window_draw( foectx->grectx);

	      foectx->plcobject = 0;
	      windowobject->hw.foectx = foectx;
	      windowobject->hw.widget_id = foectx->grectx->flow_widget;
	      nodeobject->hn.subwindowobject[ windowindex ] = windowobject;
	    }
	    else
	    {
	      /* foewindow exists, do nothing */
	      return FOE__WINDEXIST;
	    } 
	  }
	}
        ldh_AddThisSessionCallback( windowobject->hw.ldhsession,
		foectx, foe_ldh_this_session_cb);
	foe_enable_ldh_cb( foectx);

	*return_foectx = foectx;
	foectx->map_window = map_window;
	foectx->access = access;

	/* SG 22.05.91 is this the good position to start trace ??? */
        if ( foectx->function == TRACE) 
	  trace_start ( foectx ) ;
        else if ( foectx->function == SIMULATE) 
	{
	  trace_start ( foectx ) ;
	  gre_disable_button_events( foectx->grectx);
	  trace_simsetup( foectx);
	}


	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		foe_display_help()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* char 		*topic_title	I	str{ng med {mne i hj}lp.
* foe_ctx 	foectx		I	foe context.
*
* Description:
*	Display a help box widget.
*       Receive the char string which discrible the key acces to the topic
*	that will be first topic.
* SG : 14.04.91  
**************************************************************************/

void foe_display_help   ( 
	foe_ctx foectx,
	char *topic_title
)

{
  /* Obsolete */
}

/*************************************************************************
*
* Name:		foe_ctx	foe_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	parent context.
* Widget	parent_wid	I	parent widget.
* char 		*name		I	name of the foe instance.
*
* Description:
*	Create a new foe object
* SG : 09.02.91  Define a pixmap for the icon
**************************************************************************/

int foe_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjid	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	foe_ctx		*return_foectx,
	int		map_window,
	ldh_eAccess	access
	)
{
	foe_ctx		foectx;
	int		sts, size;
	pwr_tFullName	new_name;
	ldh_tSesContext	ldhses;

	/* Check that it is possible to open a session */
	sts = ldh_OpenSession(&ldhses, ldh_SessionToVol( ldhsesctx),
		access, ldh_eUtility_PlcEditor);
	if ( EVEN(sts)) return sts;

	sts = ldh_CloseSession(ldhses);

	/* 
	SG 24.03.91 : get in the variable name  
	the name of the plc object to write on the title bar of the widget 
	*/
	sts = ldh_ObjidToName( ldhsesctx, 
		plcprogram, 
		ldh_eName_Hierarchy,
		new_name,
		sizeof( new_name),
		&size); 
	error_msg(sts);
	if( EVEN(sts) ) return sts;

	sts = foe_new_local( parent_ctx, parent_wid, new_name, plcprogram,
		ldhwbctx, ldhsesctx, 0, 0, 0, &foectx, map_window, access,
		foe_eFuncAccess_Edit);
	if ( EVEN(sts)) return sts;

	*return_foectx = foectx;
	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		foe_ctx	foe_create_window()
*
* Type		foe_ctx
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char *	name		I	Name of the created object
* void		*parent_ctx	I	parent context
* Widget	parent_wid	I	parent widget
* char		*name		I	name of the foe instance.
* int		x_top		I	x koordinate of the foe window.
* int		y_top		I	y koordinate of the foe window.
* int		width_adb	I	width of the foe window.
* int		height_adb	I	height of the foe window.
* ldh_tSesContext	ldhsession	I	ldh session.
* int 		function	I	Editor mode (view, edit, trace...)
*
* Description:
*	Create a new foe window
* SG 09.02.91 : Create a new toplevel widget and give it an pixmap icon
* SG : 13.02.91 Fetch a caution box and save the id of the widget in the 
*               foe context for futher use
* SG : 24.02.91 Fetch a widget ( again !) . This time a message box
* SG : 24.03.91 Send the routine the desired position (x_top , y_top) 
* and the desired size ( width_adb , height_adb ) for the window to be created.
* THe two first parameters are set to the top level widget
* and the two last paramters are set to the adb widget (see in uil file
* the widget hierarchy )
* SG 14.04.91 A new widget to fetch , the help widget ! 
* SG 2.05.91 New parameter : function ( edit , trace , view , simulate ..)
* to indicate which neted events and which menu entries should be alllowed.
**************************************************************************/

static foe_ctx	foe_create_window(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
        int	x_top ,
        int	y_top ,
        int	width_adb ,
        int	height_adb , 
	ldh_tSesContext	ldhsession ,
	int 	function ,
	int	map_window,
	foe_eFuncAccess	function_access
	)
{
	Arg	args[20];
	int	sts;
	Widget *children;
        int i, num_children;

	foe_ctx	foectx;
	XmString   	TitleStr;
	Dimension icon_w, icon_h;

	/* DRM database hierarchy related variables */
	MrmHierarchy s_MrmH;
	MrmType dclass;
	char		uid_filename[200] = {"pwr_exe:wb_foe.uid"};
 	char		*uid_filename_p = uid_filename;
	static Atom	MwmDeleteWind = 0;
	static char translations[] = "\
<FocusIn>: foe_inputfocus()\n";
  	static XtTranslations compiled_translations = NULL;

  	static XtActionsRec actions[] =
  	{
  	  {"foe_inputfocus",      (XtActionProc) foe_action_inputfocus}
  	};
       
  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts))
  {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

/*
 * Now start the module creation
 */

/* Create object context */
 foectx = (foe_ctx)XtCalloc( 1, sizeof( t_foe_ctx));

/* SG 02.05.91 save the desired function ( edit , trace , view ... )
   in the context */
 foectx->ctx_type = wb_eUtility_PlcEditor;
 foectx->function = function ;
 foectx->cp.parent_ctx = parent_ctx;
 foectx->cp.name = XtNewString( name );
 foectx->popupmenu_mask = ~0;
 foectx->advanced_user = 1;

/* set initialization values in context */
 foectx->con_drawtype = GOEN_CONDRAW;

/* Save the context structure in the widget */
 XtSetArg (args[0], XmNuserData, (unsigned int) foectx);

/*
 * Create a new neted widget
 */
/*
 * Open the UID files (the output of the UIL compiler) in the hierarchy
 * Register the items DRM needs to bind for us.
 * Create a new neted-widget
 * Close the hierarchy
 * Compile the additional button translations and augment and add actions
 */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_MrmH);
  if (sts != MrmSUCCESS) 
  {
     printf("can't open hierarchy\n");
     return 0;
  }

  foe_register_callbacks (foectx) ;

  /* SG 09.02.91 : create the new hierarchy. By the way use a pixmap define
  with help of wb_foe_ptrn.h */

/* create a pixmap for the icon */
  if ( icon_pixmap == 0 )
     sts = MrmFetchBitmapLiteral(s_MrmH,"icon",
              XtScreen(parent_wid), XtDisplay(parent_wid),
              &icon_pixmap, &icon_w, &icon_h);  
 
  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  XtSetArg(args[i],XtNiconPixmap,icon_pixmap);  i++;
  
  /* SG 11.02.91 Save the id of the top in the context */ 
  foectx->cp.parent_wid = XtCreatePopupShell("plcEditor", 
		topLevelShellWidgetClass, parent_wid, args, i);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(foectx->cp.parent_wid), 
						actions, XtNumber(actions));

  /* the positioning of a top level can only be define after the creation
      of the widget . So i do it know: 
     SG 24.02.91 use the parameters received x and y 
  */

  i=0;
  XtSetArg(args[i],XmNx,x_top);i++;
  XtSetArg(args[i],XmNy,y_top);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  if ( ! map_window)
  {
    XtSetArg(args[i],XmNmappedWhenManaged,FALSE); i++;
  }

  XtSetValues( foectx->cp.parent_wid, args, i);

  XtSetArg (args[0], XmNuserData, (unsigned int) foectx);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_MrmH, "foe_window", foectx->cp.parent_wid ,
		name, args, 1,
		&foectx->widgets.foe_window, &dclass);
  if (sts != MrmSUCCESS) printf("can't fetch foedit widget\n");

  /* SG 24.02.91 Set the width and hight desired for the main window to his
	child the adb widget */
  i=0;
  XtSetArg(args[i],XmNwidth,width_adb);i++;
  XtSetArg(args[i],XmNheight,height_adb);i++;
  XtSetValues( foectx->widgets.adb  ,args,i);

  /* SG 24.02.91
     Get a litle message box widget that you will make appears when you want by
     calling foe_msgbox
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_msgbox", 
			foectx->widgets.foe_window,
			&foectx->widgets.msgbox,
			&dclass) != MrmSUCCESS)
	printf("can't fetch msgbox \n");

  TitleStr = XmStringCreateLtoR("PlcEdit Message" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( foectx->widgets.msgbox, args, 1);
  XmStringFree(TitleStr);


  /* SG 13.02.91 
     Get a litle caution widget that you will make appears when you want by
     calling foe_popupmsg 
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_popupmsg", 
			foectx->widgets.foe_window,
			&foectx->widgets.caution,
			&dclass) != MrmSUCCESS)
	printf("can't fetch popupmsg\n");

  TitleStr = XmStringCreateLtoR("PlcEdit Confirm" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( foectx->widgets.caution, args, 1);
  XmStringFree(TitleStr);


  /* create the pane widgets */
  /* create the palette module */
  /* SG 07.03.91 put two palletes in the sub pane widget */

#if 0
  foectx->node_palctx = hipa_new( foectx, foectx->widgets.subpane, "Objects",
		 ldhsession, "wb:layout-PlcEditor-w1", NULL, NULL);
  XtVaSetValues ( ((foectx->node_palctx)->w1)->svn_adb, 
      XmNallowResize, 1,
      XmNpaneMinimum, 100,
/*      XmNheight, height_adb - 130, */
      NULL);
#endif
  foectx->node_palctx = pal_new( foectx, foectx->widgets.subpane, "Objects",
		ldhsession, "PlcEditorPalette", 
		&foectx->widgets.node_palette, &sts);
  foectx->node_palette_managed = 1;

  foectx->navctx = nav_new( foectx, foectx->widgets.subpane, "Plant",
		ldhsession, "NavigatorW1",
		&foectx->widgets.nav_palette, &sts);
  XtUnmanageChild( foectx->widgets.nav_palette);

  foectx->con_palctx = pal_new( foectx, foectx->widgets.subpane, "Connections",
		ldhsession, "PlcEditorConPalette", 
		&foectx->widgets.con_palette, &sts);
  XtUnmanageChild( foectx->widgets.con_palette);


  /* Indicate that connections are not choosed from the palette.
   * The label is set sensitive in foe_activate_usercon 
   */
#if 0
  XtSetSensitive(foectx->conn_palctx->w1->svn_name_label, 0);
#endif

  /* Turn off traversal for sash widgets */
  XtVaGetValues(foectx->widgets.subpane, 
      XmNchildren, &children,
      XmNnumChildren, &num_children,
      NULL);
  for (i = 0; i < num_children; i++) {
      if (XmIsSash(children[i]))
	  XtVaSetValues(children[i], XmNtraversalOn, False, NULL);
  }

  /* create the gredit module */
  XtManageChild(foectx->widgets.foe_window);


  foectx->grectx = gre_new( foectx, foectx->widgets.pane, "Name");

  gre_new_navigator( foectx->grectx, foectx->widgets.subpane);

  if ( map_window)
    XtPopup(foectx->cp.parent_wid, XtGrabNone);
  else
    XtRealizeWidget( foectx->cp.parent_wid);

  gre_init( foectx->grectx);

  /* create the setup ie:
	enable netedevents,
	enable menu entries
  	setup backcalls from the gredit module */

  foe_function_setup ( foectx ) ;

/* SG 10.02.91 popup : Ask DRM to fetch the popup */

  if (MrmFetchWidget (s_MrmH, 
			"edit_popup", 
		      //  foectx->grectx->flow_widget,
		        foectx->widgets.pane,
			&foectx->widgets.popupmenu,
			&dclass) != MrmSUCCESS)
	printf("can't fetch popup\n");

  MrmCloseHierarchy(s_MrmH);

  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( foectx->widgets.foe_window, compiled_translations);

  XtUnmanageChild(foectx->widgets.textinput);

  if (  !(login_prv.priv & pwr_mPrv_DevPlc ) || 
	(function_access == foe_eFuncAccess_View))
  {
     /* Mode Edit and Simulate is not allowed */
    i=0;
    XtSetArg(args[i],XmNsensitive,0); i++;

    XtSetValues( foectx->widgets.edit_togg,args,i);
    XtSetValues( foectx->widgets.simulate_togg,args,i);
  }

  if (MwmDeleteWind == 0)
     MwmDeleteWind = XInternAtom(XtDisplay(foectx->cp.parent_wid), "WM_DELETE_WINDOW", FALSE);

  if (MwmDeleteWind != 0)
  {
    XtVaSetValues(foectx->cp.parent_wid, XmNdeleteResponse, XmDO_NOTHING, NULL);
    XmAddWMProtocolCallback(foectx->cp.parent_wid, MwmDeleteWind, 
			    (XtCallbackProc)foe_activate_quit, foectx);
  }


  /* SG 09.02.91 a top level should always be realized !
   * ML 16.03.94 changed Realize to Popup
   */

  return(foectx);
}


/*************************************************************************
*
* Name:		foe_view_setup()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx 	*foectx		I	foe context
*
* Description:
*	Setup for the view mode.
*	Defines the events and backcallfunktions for gre in view.
*
**************************************************************************/

static int foe_view_setup ( 
	foe_ctx foectx
)

{
  int sts ;

  /* the same entries are disables for trace and view */
  foe_view_set_entries ( foectx);

  gre_view_setup( foectx->grectx);

  sts = gre_setup_backcalls ( foectx->grectx , 
		foe_gre_setup_window,
		foe_gre_node_created,
		foe_gre_con_created,
		foe_gre_node_moved,
		foe_gre_delete,
		foe_gre_cut,
		foe_gre_copy,
		foe_gre_paste,
		foe_gre_attribute,
		foe_gre_subwindow,
		0,
		foe_gre_popupmenu,
		foe_gre_getobj,
		foe_gre_undelete,
		foe_gre_unselect,
		foe_gre_help,
		foe_gre_regionmoved,
		foe_gre_message);

  return sts;
}


/*************************************************************************
*
* Name:		foe_edit_setup()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx 	*foectx		I	foe context
*
* Description:
*	Setup for the edit mode.
*	Defines the events and backcallfunktions for gre in edit.
*
**************************************************************************/
static int  foe_edit_setup (
    foe_ctx foectx 
)
{
  int sts;

  /* enable the menu entries that were disable 
  for trace the same entries are disables for trace and view */
   
  foe_edit_set_entries ( foectx);

  gre_edit_setup( foectx->grectx);

  /* setup the backcalls from gre */
  sts = gre_setup_backcalls ( foectx->grectx , 
		foe_gre_setup_window,
		foe_gre_node_created,
		foe_gre_con_created,
		foe_gre_node_moved,
		foe_gre_delete,
		foe_gre_cut,
		foe_gre_copy,
		foe_gre_paste,
		foe_gre_attribute,
		foe_gre_subwindow,
		0,
		foe_gre_popupmenu,
		foe_gre_getobj,
		foe_gre_undelete,
		foe_gre_unselect,
		foe_gre_help,
		foe_gre_regionmoved,
		foe_gre_message);

  
  return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		int	foe_function_setup()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description: 
*	Enable events and menu entris depending on the function
*	in which the user works.
* Author : SG 15.05.91
*
**************************************************************************/

static void foe_function_setup (
	foe_ctx foectx
)
{

  /* SG 02.05.91 set the default toggel button corresponding to the mode on */

  if (  foectx->function == EDIT ) 
  {
    XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
    foe_edit_setup ( foectx ) ;
  }

  else if ( foectx->function == TRACE) 
  {
    XmToggleButtonSetState( foectx->widgets.trace_togg, 1, 0);
    /* SG 22.05.91 
    It is more that a setup: we want also to initialise the tracing.
    For this the differents contexts have to be defined.
    so trace_Start is not called here */
  }

  else if ( foectx->function == VIEW ) 
  {
    XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
    foe_view_setup ( foectx ) ; 
  }
  
  else if ( foectx->function == SIMULATE ) 
  {
    XmToggleButtonSetState( foectx->widgets.simulate_togg, 1, 0);
  }

}



/*************************************************************************
*
* Name:		int	foe_show_executeorder()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
*
* Description: 
*	Show the execute order in a display rectangle in each node.
*
**************************************************************************/

int	foe_show_executeorder ( 
	foe_ctx foectx
)
{
	vldh_t_node 	*node_list;
	vldh_t_node 	*node_ptr;
	unsigned long	node_count;
	int		i, sts;
	char		text[10];

	/* Get all nodes */
	sts = vldh_get_nodes( foectx->grectx->window_object, 
		&node_count, &node_list);
	if ( EVEN(sts)) return sts;
	node_ptr = node_list;

	for ( i = 0; i < node_count; i++)
	{
	  if ( (*node_ptr)->hn.executeorder > 0)
	  {
	    sprintf( text, "%ld", (*node_ptr)->hn.executeorder);
	    gre_set_display_value( foectx->grectx, *node_ptr, text);
	  }
	  node_ptr++;
	}
	gre_display( foectx->grectx);
	if ( node_count > 0) XtFree((char *)node_list);
	return FOE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	foe_get_textinput()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
* char		*message	I	message.
* voidp		function	I	function to call when input is
*					terminated.
*
* Description: 
*	Show the text input widget.
*
**************************************************************************/

int foe_get_textinput ( 
	foe_ctx		foectx,
	char		*message,
	int		(*function)()
)
{
	/* Print the text */
	foe_message( foectx, message);

	/* View the input widget */
	XtManageChild(foectx->widgets.textinput);
	XtSetKeyboardFocus( foectx->cp.parent_wid, foectx->widgets.textinput);

	/* Insert the function in foectx */
	foectx->textinput_func = function;

	return FOE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	foe_search_string()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
* char		*searchstr	I	search string.
*
* Description: 
*
**************************************************************************/

int	foe_search_string_next ( 
	foe_ctx foectx 
)
{
	char		searchstr[80];
	char		annotations[1300];
	int		sts, size;
	vldh_t_node 	*node_list;
	vldh_t_node 	*node_ptr;
	unsigned long	node_count;
	int		i, j, found;
	vldh_t_node	node;
	int		len1, len2;
	vldh_t_wind	wind;
	pwr_tObjName	class_name;
	pwr_tClassId	class;
	pwr_tObjid	class_objid;

	wind = foectx->grectx->window_object; 
	strcpy( searchstr, foectx->searchstring);

	if( searchstr[0] == 0)
	{
	  foe_message( foectx, "Start with SearchString");
	  BEEP;
	  return FOE__SUCCESS;
	}

	/* Get all nodes */
	sts = vldh_get_nodes( wind, &node_count, &node_list);
	if ( EVEN(sts)) return sts;

	found = 0;
	for ( j = foectx->searchindex; j < node_count; j++)
	{
	  node_ptr = node_list + j;
	  sts = gre_node_annot_message( foectx->grectx, *node_ptr, 
			annotations, sizeof(annotations)-200, 1024);
	  strcat( annotations, " ");
	  strcat( annotations, (*node_ptr)->hn.name);
	  strcat( annotations, " ");
	  sts = ldh_GetObjectClass( wind->hw.ldhsession,
		(*node_ptr)->ln.object_did,  &class);
	  if ( EVEN(sts)) return sts;
	  class_objid = cdh_ClassIdToObjid( class);
	  sts = ldh_ObjidToName( wind->hw.ldhsession, class_objid, 
		ldh_eName_Object, class_name, sizeof( class_name), &size);
	  if ( EVEN(sts)) return sts;
	  strcat( annotations, class_name);
	  strcat( annotations, " ");

	  utl_toupper( annotations, annotations);

	  len1 = strlen( annotations);
	  len2 = strlen( searchstr);

	  if ( len1 >= len2)
	  {
	    for ( i = 0; i <= (len1 - len2); i++)
	    {
	      if ( strncmp( &annotations[i], searchstr, strlen(searchstr)) == 0)
	      {
	        /* the string matches */
	        foectx->searchindex = j + 1;
	        node = *node_ptr;
	        found = 1;
	        break;
	      }
	    }
	  }
	  if ( found)
	    break;
	}
	if ( node_count > 0) XtFree((char *)node_list);

	if ( !found)
	{
	  gre_unselect( foectx->grectx);
	  foe_message( foectx, "String not found");
	  BEEP;
	  foectx->searchindex = 0;
	  return FOE__SUCCESS;
	}	

	/* Center and select the object */
	gre_unselect( foectx->grectx);
	foe_message( foectx, "");
	sts = gre_center_node( foectx->grectx, node);
#if 0
	if ( sts == GRE__CENTER)
	  foe_message( foectx, "Unable to center object");
#endif
	gre_search_rectangle_create( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		int	foe_search_string()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
* char		*searchstr	I	search string.
*
* Description: 
*
**************************************************************************/

int	foe_search_string ( 
	foe_ctx foectx,
	char	*searchstr
)
{

	/* Insert the search string in foectx */
	utl_toupper( foectx->searchstring, searchstr);

	/* Reset the search index */
	foectx->searchindex = 0;

	/* Search */
	foe_search_string_next( foectx);

	return FOE__SUCCESS;
}



/*************************************************************************
*
* Name:		int	foe_search_object()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* foe_ctx	*foectx		I	foe context.
* char		*searchstr	I	search string.
*
* Description: 
*
**************************************************************************/

int	foe_search_object ( 
	foe_ctx foectx,
	char	*searchstr
)
{
	int		sts, size;
	pwr_tFullName	hiername;
	vldh_t_wind	wind;
	pwr_tObjid	objdid;
	vldh_t_node	node;

	wind = foectx->grectx->window_object;
	sts = ldh_ObjidToName( wind->hw.ldhsession, wind->lw.objdid, 
		ldh_eName_Hierarchy,
		hiername, sizeof( hiername), &size); 
	if ( EVEN(sts)) return sts;

	strcat( hiername, "-");
	strcat( hiername, searchstr);
	sts = ldh_NameToObjid( wind->hw.ldhsession, &objdid, hiername);
	if ( EVEN(sts))
	{
	  foe_message( foectx, "Object does not exist");
	  BEEP;
	  return FOE__SUCCESS;
	}
	sts = vldh_get_node_objdid( objdid, wind, &node);
	if ( EVEN(sts))
	{
	  foe_message( foectx, "Object not found in this window");
	  BEEP;
	  return FOE__SUCCESS;
	}
	gre_unselect( foectx->grectx);
	foe_message( foectx, "");

	sts = gre_center_node( foectx->grectx, node);
#if 0
	if ( sts == GRE__CENTER)
	  foe_message( foectx, "Unable to center object");
#endif
	gre_search_rectangle_create( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_edit_set_entries ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
* int		flag		I	if true enable entries 
*					if false disable entries
*
* Description:
*  disable or enable the menu entries that are relevant for the trace function.
**************************************************************************/

static int foe_edit_set_entries (
	foe_ctx foectx
)
{
  
  Arg	args[20];
  int i;

  i=0;
  XtSetArg(args[i],XmNsensitive,1 ); i++;

  XtSetValues( foectx->widgets.save,args,i);
  XtSetValues( foectx->widgets.exit,args,i);
  XtSetValues( foectx->widgets.syntax,args,i);
  XtSetValues( foectx->widgets.compile,args,i);
  XtSetValues( foectx->widgets.savetrace,args,i);
  XtSetValues( foectx->widgets.restoretrace,args,i);
  XtSetValues( foectx->widgets.redraw,args,i);
  XtSetValues( foectx->widgets.plcattribute,args,i);
  XtSetValues( foectx->widgets.winddelete,args,i);
  XtSetValues( foectx->widgets.edit_entry,args,i);

  return FOE__SUCCESS ;

/*  XtSetValues( foectx->widgets.print,args,i); */

}


/*************************************************************************
*
* Name:		foe_view_set_entries ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*					if false disable entries
*
* Description:
*  disable or enable the menu entries that are relevant for the trace function.
**************************************************************************/

static int foe_view_set_entries (
	foe_ctx foectx
)
{
  
  Arg	args[20];
  int i;

  i=0;
  XtSetArg(args[i],XmNsensitive,0 ); i++;

  XtSetValues( foectx->widgets.save,args,i);
  XtSetValues( foectx->widgets.exit,args,i);
  XtSetValues( foectx->widgets.syntax,args,i);
  XtSetValues( foectx->widgets.compile,args,i);
  XtSetValues( foectx->widgets.redraw,args,i);
  XtSetValues( foectx->widgets.plcattribute,args,i);
  XtSetValues( foectx->widgets.winddelete,args,i);
  XtSetValues( foectx->widgets.edit_entry,args,i);

  return FOE__SUCCESS ;

/*  XtSetValues( foectx->widgets.print,args,i); */

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

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_attribute) &&
		     !(popupmenu_mask & foe_e_popupmenu_attribute) )
	  XtUnmanageChild(foectx->widgets.pop_attribute);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_attribute) &&
		     (popupmenu_mask & foe_e_popupmenu_attribute) )
	  XtManageChild(foectx->widgets.pop_attribute);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_subwindow) &&
		     !(popupmenu_mask & foe_e_popupmenu_subwindow) )
	  XtUnmanageChild(foectx->widgets.pop_subwindow);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_subwindow) &&
		     (popupmenu_mask & foe_e_popupmenu_subwindow) )
	  XtManageChild(foectx->widgets.pop_subwindow);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_connect) &&
		     !(popupmenu_mask & foe_e_popupmenu_connect) )
	  XtUnmanageChild(foectx->widgets.pop_getobj);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_connect) &&
		     (popupmenu_mask & foe_e_popupmenu_connect) )
	  XtManageChild(foectx->widgets.pop_getobj);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_delete) &&
		     !(popupmenu_mask & foe_e_popupmenu_delete) )
	  XtUnmanageChild(foectx->widgets.pop_delete);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_delete) &&
		     (popupmenu_mask & foe_e_popupmenu_delete) )
	  XtManageChild(foectx->widgets.pop_delete);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_paste) &&
		     !(popupmenu_mask & foe_e_popupmenu_paste) )
	  XtUnmanageChild(foectx->widgets.pop_paste);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_paste) &&
		     (popupmenu_mask & foe_e_popupmenu_paste) )
	  XtManageChild(foectx->widgets.pop_paste);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_copy) &&
		     !(popupmenu_mask & foe_e_popupmenu_copy) )
	  XtUnmanageChild(foectx->widgets.pop_copy);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_copy) &&
		     (popupmenu_mask & foe_e_popupmenu_copy) )
	  XtManageChild(foectx->widgets.pop_copy);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_cut) &&
		     !(popupmenu_mask & foe_e_popupmenu_cut) )
	  XtUnmanageChild(foectx->widgets.pop_cut);
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_cut) &&
		     (popupmenu_mask & foe_e_popupmenu_cut) )
	  XtManageChild(foectx->widgets.pop_cut);

	if ( (foectx->popupmenu_mask & foe_e_popupmenu_printselect ) &&
		     !(popupmenu_mask & foe_e_popupmenu_printselect ) )
	  XtUnmanageChild(foectx->widgets.pop_printselect );
	else if ( !(foectx->popupmenu_mask & foe_e_popupmenu_printselect ) &&
		     (popupmenu_mask & foe_e_popupmenu_printselect ) )
	  XtManageChild(foectx->widgets.pop_printselect );

	foectx->popupmenu_mask = popupmenu_mask;
	
	return FOE__SUCCESS ;
}


/*************************************************************************
*
* Name:		foe_print_overview()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  	Prints an overview.
**************************************************************************/

int foe_print_overview (
	foe_ctx foectx
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, sts;
	float		ll_x_min;
	float		ll_y_min;
	float		ur_x_max;
	float		ur_y_max;
	float		ll_x;
	float		ll_y;
	float		width;
	float		height;
	int		doc_count;
	vldh_t_wind	wind;
	char		file_id[80];

	ll_x_min = 10000.;
	ll_y_min = 10000.;
	ur_x_max = -10000.;
	ur_y_max = -10000.;

	wind = foectx->grectx->window_object;

	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	/* Init the document objects */
	gre_init_docobjects( foectx->grectx);

	doc_count = 0;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
  	{
	  if ( vldh_check_document( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did))
	  {
	    /* Calculate coordinates for an overview */
	    gre_measure_object( *node_ptr, &ll_x, &ll_y, &width, &height);
	    ll_x_min = min( ll_x_min, ll_x);
	    ll_y_min = min( ll_y_min, ll_y);
	    ur_x_max = max( ur_x_max, ll_x + width);
	    ur_y_max = max( ur_y_max, ll_y + height);

	    doc_count++;	  
	  }
	  node_ptr++;
	}

	if ( doc_count > 1)
	{
	  /* Print the overview */
	  strcpy( file_id, vldh_IdToStr(0, wind->lw.objdid)); 
	  gre_print_rectangle( foectx->grectx , ll_x_min, ll_y_min, ur_x_max, 
		ur_y_max, file_id);
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( doc_count <= 1)
	  return FOE__NOOBJFOUND;
	
	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		foe_print_document()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  	Prints the documents in a window.
**************************************************************************/

int foe_print_document( 
	foe_ctx foectx
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, sts;
	vldh_t_wind	wind;
	int		doc_count;

	wind = foectx->grectx->window_object;

	sts = vldh_get_nodes( wind, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	gre_init_docobjects( foectx->grectx);

	doc_count = 0;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
  	{
	  if ( vldh_check_document( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did))
	  {
	    gre_print_docobj( foectx->grectx , *node_ptr);
	    doc_count++;	  
	  }
	  node_ptr++;
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( doc_count == 0)
	  return FOE__NOOBJFOUND;

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_print_selected_document()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  	Prints the documents in a window.
**************************************************************************/

int foe_print_selected_document( 
	foe_ctx foectx
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, sts;
	vldh_t_wind	wind;
	int		doc_count;

	wind = foectx->grectx->window_object;

	sts = gre_get_selnodes( foectx->grectx, &node_count, &nodelist);
	if ( EVEN(sts)) return sts;

	gre_unselect( foectx->grectx);
	gre_init_docobjects( foectx->grectx);

	doc_count = 0;
	node_ptr = nodelist;
	for ( i = 0; i < node_count; i++)
  	{
	  if ( vldh_check_document( wind->hw.ldhsession, 
			(*node_ptr)->ln.object_did))
	  {
	    gre_print_docobj( foectx->grectx , *node_ptr);
	    doc_count++;	  
	  }
	  node_ptr++;
	}

	if ( node_count > 0) XtFree((char *) nodelist);

	if ( doc_count == 0)
	  return FOE__NOOBJFOUND;

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		foe_change_mode()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  	Changed mode.
**************************************************************************/

int foe_change_mode(
	foe_ctx foectx,
	int	new_mode
)
{
 	vldh_t_wind	wind;
	int		sts, vldh_mod;
	ldh_sSessInfo	info;

	wind = foectx->grectx->window_object;

	switch( new_mode)
	{
	  case VIEW:
	    switch( foectx->function)
	    {
	      case VIEW:
	        /* Mode unchanged, set togglebutton */
	        XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	        break;
	      case EDIT:
	        /* Save changes... */
		wind = foectx->grectx->window_object;

		/* Check if changes are made */
		sts = ldh_GetSessionInfo( wind->hw.ldhsession, &info);
	        if ( EVEN(sts)) return sts;
		sts = vldh_get_wind_modification( wind, &vldh_mod);

		if ( !info.Empty || vldh_mod)
		{
		  /* Assume mode change is not successfull */
	          foectx->wanted_function = VIEW;
	          XmToggleButtonSetState( foectx->widgets.view_togg, 0, 0);
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	          foe_popupmsg( foectx , "Do you want to save changes",
			foe_edit_exit_save, foe_edit_exit_nosave,
			NULL);
		}
		else
	   	{
		  /* Change funktion */
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	  	  foectx->function = VIEW;
	  	  gre_disable_button_events( foectx->grectx);
	  	  foe_view_setup( foectx);
	          sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	          if ( EVEN(sts)) return sts;
	          foectx->access = ldh_eAccess_ReadOnly;
		}
	        break;
	      case TRACE:
	        /* Stop trace */
	        XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	        XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
		trace_stop ( foectx);
		foectx->function = VIEW ;
	  	gre_disable_button_events ( foectx->grectx ) ;
	  	foe_view_setup ( foectx );
	        sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	        if ( EVEN(sts)) return sts;
	        foectx->access = ldh_eAccess_ReadOnly;
	        break;
	      case SIMULATE:
	        /* Stop trace and simulate */
	        XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	        XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
		trace_stop ( foectx);
		foectx->function = VIEW ;
	  	gre_disable_button_events ( foectx->grectx ) ;
	  	foe_view_setup ( foectx ) ;
	        sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	        if ( EVEN(sts)) return sts;
	        foectx->access = ldh_eAccess_ReadOnly;
	        break;
	    }
	    break;
	  case EDIT:
	    if ( !(login_prv.priv & pwr_mPrv_DevPlc ))
	    {
	      foe_message( foectx, "No privilege to enter Edit");
	      BEEP;
	      XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	      return FOE__SUCCESS;
	    }

	    switch( foectx->function)
	    {
	      case VIEW:
	        sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadWrite);
	        if( EVEN(sts))
	        {
	          foe_message( foectx, "No access to edit, close other session");
	          BEEP;
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	          ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	          foectx->access = ldh_eAccess_ReadOnly;
	          return FOE__SUCCESS;
	        }
                ldh_AddThisSessionCallback( wind->hw.ldhsession,
		    foectx, foe_ldh_this_session_cb);
	        foe_enable_ldh_cb( foectx);
	        foectx->access = ldh_eAccess_ReadWrite;
	        foectx->function = EDIT ;
	        gre_disable_button_events ( foectx->grectx ) ;
	        foe_edit_setup( foectx);
	        XmToggleButtonSetState( foectx->widgets.view_togg, 0, 0);
	        break;
	      case EDIT:
	        /* Mode unchanged */
	        XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	        break;
	      case TRACE:
	        /* Stop trace */
		trace_stop( foectx);
		foectx->function = EDIT;
	  	gre_disable_button_events( foectx->grectx);
	  	foe_edit_setup( foectx);
	        XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	        break;
	      case SIMULATE:
	        /* Stop trace and simulate */
	        trace_stop( foectx);
		foectx->function = EDIT;
	  	gre_disable_button_events ( foectx->grectx ) ;
	  	foe_edit_setup( foectx);
	        XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	        break;
	    }
	    break;
	  case TRACE:

	    switch( foectx->function)
	    {
	      case VIEW:
		/* Change funktion */
	        sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadWrite);
	        if( EVEN(sts))
	        {
	          foe_message( foectx, "No access to trace, close other session");
	          BEEP;
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	          XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	          return FOE__SUCCESS;
	        }
                ldh_AddThisSessionCallback( wind->hw.ldhsession,
		    foectx, foe_ldh_this_session_cb);
	        foe_enable_ldh_cb( foectx);
	        foectx->access = ldh_eAccess_ReadWrite;
	  	gre_disable_button_events( foectx->grectx);
		sts = trace_start( foectx);
	        if ( EVEN(sts))
	        {
	          /* Return to view */
	          foe_view_setup( foectx);
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	          XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	          ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	          foectx->access = ldh_eAccess_ReadOnly;
	          return sts;
	        }
	        XmToggleButtonSetState( foectx->widgets.view_togg, 0, 0);
	  	foectx->function = TRACE;
	        break;
	      case EDIT:
	        /* Save changes... */
		wind = foectx->grectx->window_object;

		/* Check if changes are made */
		sts = ldh_GetSessionInfo( wind->hw.ldhsession, &info);
	        if ( EVEN(sts)) return sts;
		sts = vldh_get_wind_modification( wind, &vldh_mod);

		if ( !info.Empty || vldh_mod)
		{
		  /* Assume mode change is not successfull */
	          foectx->wanted_function = TRACE;
	          XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	          foe_popupmsg( foectx , "Do you want to save changes", 
			foe_edit_exit_save, foe_edit_exit_nosave, 
			NULL); 
		}
		else
	   	{
		  /* Change function */
		  sts = trace_start( foectx);
	          if ( EVEN(sts))
	          {
	            /* Return to view */
	            foe_edit_setup( foectx);
	            XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	            XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
	            return sts;
	          }
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	  	  foectx->function = TRACE;
	  	  gre_disable_button_events( foectx->grectx);
		}
	        break;
	      case TRACE:
	        /* Mode unchanged, set togglebutton */
	        XmToggleButtonSetState( foectx->widgets.trace_togg, 1, 0);
	        break;
	      case SIMULATE:
	        XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
		gre_disable_button_events( foectx->grectx);
		trace_trasetup( foectx);
	  	foectx->function = TRACE;
	        break;
	    }
	    break;
	  case SIMULATE:

	    if ( !(login_prv.priv & pwr_mPrv_DevPlc ))
	    {
	      foe_message( foectx, "No privilege to enter Simulate");
	      BEEP;
	      XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	      return FOE__SUCCESS;
	    }
	    switch( foectx->function)
	    {
	      case VIEW:
		/* Change funktion */
	        sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadWrite);
	        if( EVEN(sts))
	        {
	          foe_message( foectx, "No access to simulate, close other session");
	          BEEP;
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	          XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	          return FOE__SUCCESS;
	        }
                ldh_AddThisSessionCallback( wind->hw.ldhsession,
		    foectx, foe_ldh_this_session_cb);
	        foe_enable_ldh_cb( foectx);
	        foectx->access = ldh_eAccess_ReadWrite;
	  	gre_disable_button_events( foectx->grectx);
		sts = trace_start( foectx);
	        if ( EVEN(sts))
	        {
	          /* Return to view */
	          foe_view_setup( foectx);
	          XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	          XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	          return sts;
	        }
	        XmToggleButtonSetState( foectx->widgets.view_togg, 0, 0);
	  	foectx->function = SIMULATE;
	 	gre_disable_button_events( foectx->grectx);
		trace_simsetup( foectx);
	        foe_message( foectx, "Mode is Simulate");
	        break;
	      case EDIT:
	        /* Save changes... */
		wind = foectx->grectx->window_object;

		/* Check if changes are made */
		sts = ldh_GetSessionInfo( wind->hw.ldhsession, &info);
	        if ( EVEN(sts)) return sts;
		sts = vldh_get_wind_modification( wind, &vldh_mod);

		if ( !info.Empty || vldh_mod)
		{
		  /* Assume mode change is not successfull */
	          foectx->wanted_function = SIMULATE;
	          XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	          foe_popupmsg( foectx , "Do you want to save changes", 
			foe_edit_exit_save, foe_edit_exit_nosave, 
			NULL); 
		}
		else
	   	{
		  /* Change function */
	  	  gre_disable_button_events( foectx->grectx);
		  sts = trace_start( foectx);
	          if ( EVEN(sts))
	          {
	            /* Return to view */
	            foe_view_setup( foectx);
	            XmToggleButtonSetState( foectx->widgets.edit_togg, 1, 0);
	            XmToggleButtonSetState( foectx->widgets.simulate_togg, 0, 0);
	            return sts;
	          }
	          XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	  	  foectx->function = SIMULATE;
	  	  gre_disable_button_events( foectx->grectx);
		  trace_simsetup( foectx);
	          foe_message( foectx, "Mode is Simulate");
		}
	        break;
	      case TRACE:
	        XmToggleButtonSetState( foectx->widgets.trace_togg, 0, 0);
		foectx->function = SIMULATE;
		gre_disable_button_events( foectx->grectx);
		trace_simsetup( foectx);
	        foe_message( foectx, "Mode is Simulate");
	        break;
	      case SIMULATE:
	        /* Mode unchanged, set togglebutton */
	        XmToggleButtonSetState( foectx->widgets.simulate_togg, 1, 0);
	        break;

	    }
	    break;
	}
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

static void foe_edit_exit_save(
	foe_ctx	foectx
)
{
	vldh_t_wind	wind;
	vldh_t_node	parent_node;
	int	sts;


	/* Check that the parent node is saved */
	wind = foectx->grectx->window_object;
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
	foe_disable_ldh_cb(foectx);
	sts = vldh_wind_save( foectx->grectx->window_object);
	error_msg( sts);
	if ( sts == VLDH__PLCNOTSAVED )
	{
          foe_msgbox ( foectx , 
		"Save the plcprogram in the hierarchy editor first") ;
	  foe_enable_ldh_cb(foectx);
	  return;
	}
	if (EVEN(sts))
	{ 
	  foe_message( foectx,"ERROR   COULDN'T SAVE WINDOW");
	  foe_enable_ldh_cb(foectx);
	  return;
	}

	/* Change the funktion */
	switch( foectx->wanted_function)
	{
	  case SIMULATE:
	    gre_disable_button_events( foectx->grectx);
	    sts = trace_start( foectx);
	    if ( EVEN(sts))
	    {
	      /* Return to edit */
	      foe_edit_setup( foectx);
	      return;
	    }
	    gre_disable_button_events( foectx->grectx);
	    trace_simsetup( foectx);
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.simulate_togg, 1, 0);
	    foectx->function = SIMULATE;
	    break;
	  case TRACE:
	    gre_disable_button_events( foectx->grectx);
	    sts = trace_start( foectx);
	    if ( EVEN(sts))
	    {
	      /* Return to edit */
	      foe_edit_setup( foectx);
	      return;
	    }
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.trace_togg, 1, 0);
	    foectx->function = TRACE;
	    break;
	  case VIEW:
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	    foectx->function = VIEW;
	    gre_disable_button_events( foectx->grectx);
	    foe_view_setup( foectx);
	    sts = ldh_SetSession( wind->hw.ldhsession, 
			ldh_eAccess_ReadOnly);
	    error_msg(sts);
	    foectx->access = ldh_eAccess_ReadOnly;
	    break;
	}
}

/*************************************************************************
*
* Name:		void	foe_exit_nosave()
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
*
* Description:
*	Backcall when no save is pressed in the questionbox at exit.
**************************************************************************/

static void foe_edit_exit_nosave(
	foe_ctx	foectx
)
{
	int			sts;
	vldh_t_wind		wind;
	pwr_tClassId		class;

	wind = foectx->grectx->window_object;

	foe_message( foectx, "Mode is still Edit, revert by exiting the PlcEditor");
	BEEP;
	return;

	/*************************************************/
	/* BUGGFIX, quit on suborder windows not allowed */
	/*************************************************/
	ldh_GetObjectClass( wind->hw.ldhsession,
			wind->lw.parent_node_did,
			&class);
	if ( class == vldh_class( wind->hw.ldhsession, VLDH_CLASS_ORDER))
	{
	  foe_message( foectx, 
		"Operation is not allowed, do save !!");
	  return;
	}	  

	/* Change the funktion */
	switch( foectx->wanted_function)
	{
	  case SIMULATE:
	    gre_disable_button_events( foectx->grectx);
	    sts = trace_start( foectx);
	    if ( EVEN(sts))
	    {
	      /* Return to edit */
	      foe_edit_setup( foectx);
	      return;
	    }
	    gre_disable_button_events( foectx->grectx);
	    trace_simsetup( foectx);
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.simulate_togg, 1, 0);
	    foectx->function = SIMULATE;
	    break;
	  case TRACE:
	    gre_disable_button_events( foectx->grectx);
	    sts = trace_start( foectx);
	    if ( EVEN(sts))
	    {
	      /* Return to edit */
	      foe_edit_setup( foectx);
	      return;
	    }
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.trace_togg, 1, 0);
	    foectx->function = TRACE;
	    break;
	  case VIEW:
	    XmToggleButtonSetState( foectx->widgets.edit_togg, 0, 0);
	    XmToggleButtonSetState( foectx->widgets.view_togg, 1, 0);
	    foectx->function = VIEW;
	    gre_disable_button_events( foectx->grectx);
	    foe_view_setup( foectx);
	    sts = ldh_SetSession( wind->hw.ldhsession,
			ldh_eAccess_ReadOnly);
	    error_msg(sts);
	    foectx->access = ldh_eAccess_ReadOnly;
	    break;
	}
}

/*************************************************************************
*
* Name:		void	foe_get_hinactx()
*
* Type		Parameter	IOGF	Description
* foe_ctx *	foectx		I	Context adress
* hina_ctx *	hinactx		I	Context adress
*
* Description:
*	Returns the hina ctx.
**************************************************************************/

void foe_get_hinactx(
	foe_ctx	foectx,
	void **hinactx
)
{
	vldh_t_wind	wind;
	vldh_t_plc	plc;

	  wind = foectx->grectx->window_object;
	  plc = wind->hw.plcobject_pointer;
	  *hinactx = plc->hp.hinactx;
}



/*************************************************************************
*
* Name:		foe_redraw_and_save()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context
*
* Description:
*  	Redraw the window.
**************************************************************************/

int foe_redraw_and_save ( foe_ctx foectx
)
{
	int sts;

	gre_edit_setup( foectx->grectx);

	sts = gre_redraw( foectx->grectx);
	if ( EVEN(sts)) return sts;

	foe_disable_ldh_cb(foectx);
	sts = vldh_wind_save( foectx->grectx->window_object);
	foe_enable_ldh_cb(foectx);
	if ( EVEN(sts)) return sts;

	sts = gre_set_trace_attributes( foectx->grectx);
	sts = gre_save( foectx->grectx);
	if ( EVEN(sts)) return sts;

	return FOE__SUCCESS;
}


/* API routines */

/************************************************************************
*
* Name: foe_Open 
*		 
* Description:	Activates an editor context and creates a new instance if 
*		needed 
*
*************************************************************************/

foe_tContext foe_Open(
  void		    *ParentEditorContext,
  void		    *ParentWindowContext,
  ldh_tSesContext   SessCtx,
  pwr_tObjid	    PlcPgmObject
) {

  foe_tContext FoeCtx = NULL;


  foe_new (ParentEditorContext, (Widget)ParentWindowContext,  
	   "PlcProgram", PlcPgmObject, ldh_SessionToWB(SessCtx), SessCtx, 
	   (foe_ctx *)&FoeCtx, 1, ldh_eAccess_ReadOnly);

  return FoeCtx;

} /* foe_Open */

/************************************************************************
*
* Name: foe_Close 
*		 
* Description:	Closes a foe editor and frees allocated resources
*************************************************************************/
void foe_Close (
  foe_tContext FoeContext
) {

  foe_delete ((foe_ctx) FoeContext);

}/* foe_Close */
