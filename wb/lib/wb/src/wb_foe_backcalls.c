/* wb_foe_backcalls.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates function object editor and handles
   the graphical editor.
   It contains all the backcalls from the controlled gre module.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wb_foe_macros.h"
#include "pwr.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>

#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_foe.h"
#include "wb_goen.h"
#include "wb_gobj.h"
#include "wb_gre.h"
#include "wb_gsx.h"
#include "wb_foe_backcalls.h"
#include "pwr_baseclasses.h"

#define	BEEP	    putchar( '\7' );

#if 0
#define	CLOCK_CURSOR	/* XDefineCursor(\
		XtDisplay( (foectx->grectx)->neted_window),\
		DwtGetWindow( (foectx->grectx)->neted_window),\
		(foectx->grectx)->cursors[ GOEN_CURSOR_CLOCK ]); */

#define	NORMAL_CURSOR	/* XUndefineCursor(\
			XtDisplay( (foectx->grectx)->neted_window),\
			DwtGetWindow( (foectx->grectx)->neted_window)); */

/* Use these in the gredit backcalls where foectx is not declared */

#define	GRE_CLOCK_CURSOR	/* XDefineCursor(\
		XtDisplay( grectx->neted_window),\
		DwtGetWindow( grectx->neted_window),\
		grectx->cursors[ GOEN_CURSOR_CLOCK ]); */

#define	GRE_NORMAL_CURSOR	/* XUndefineCursor(\
			XtDisplay( grectx->neted_window),\
			DwtGetWindow( grectx->neted_window)); */
#endif

#define	CLOCK_CURSOR	

#define	NORMAL_CURSOR	

#define	GRE_CLOCK_CURSOR	

#define	GRE_NORMAL_CURSOR	


#if 0
typedef struct {
	int	Level;
	int	Sensitive;
	char	Name[20];
	void 	(*Callback)();
	void 	*CallbackData;
} foe_sMenuItem;

static void foe_popup_unmap_cb( Widget w, foe_ctx foectx, 
				XmAnyCallbackStruct *data)
{
}

static Widget BuildMenu (
  Widget Parent,
  foe_sMenuItem *Items,
  int	x,
  int	y
)
{
  Widget Menu, W;
  int i;
  WidgetClass Class;
  Arg ArgList[5]; 
  foe_sMenuItem *Item;

  i = 0;
  XtSetArg(ArgList[i], XmNx, x); i++;
  XtSetArg(ArgList[i], XmNy, y); i++;
  Menu = XmCreatePopupMenu(Parent, "_popup", ArgList, i);

  Item = Items;
  for ( i = 0; Item->Level != 0 && Item->Level >= Items->Level; i++) 
  {
    Class = xmPushButtonGadgetClass;
    W = XtVaCreateManagedWidget( Item->Name, 
		    Class, Menu,
		    XmNsensitive, (Boolean)(Items->Sensitive == 1),
		    NULL);
/*
    XtAddCallback(W, XmNactivateCallback, Item->Callback, (XtPointer) 
	Item->CallbackData);
*/
    Item++;
  }
  return Menu;
}
int foe_create_popup( foe_ctx foectx)
{
  foe_sMenuItem *menu_items, *item;
  int x_pix, y_pix;

	menu_items = (foe_sMenuItem *) XtCalloc( 4, sizeof(foe_sMenuItem));
	item = menu_items;
	
	item->Level = 1;
	item->Sensitive = 1;
	strcpy( item->Name, "Copy");
/*
	item->Callback = foe_activate_copy;
	item->CallbackData = (void *) foectx;
*/
	item++;

	item->Level = 1;
	item->Sensitive = 1;
	strcpy( item->Name, "Paste");
/*
	item->Callback = foe_activate_copy;
	item->CallbackData = (void *) foectx;
*/
	item++;

	item->Level = 0;

        x_pix = 0;
        y_pix = 0;
	foectx->widgets.popupmenu = 
	  BuildMenu( foectx->grectx->flow_widget, menu_items, x_pix, y_pix);
        XtAddCallback( foectx->widgets.popupmenu, XmNunmapCallback, 
				(XtCallbackProc)foe_popup_unmap_cb, foectx);
        XtManageChild (foectx->widgets.popupmenu);
	return 1;
}
#endif

/*************************************************************************
*
* Name:		foe_gre_setup_window ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: Backcall from the controlled gre module
*
**************************************************************************/

void foe_gre_setup_window( 
    gre_ctx	grectx
)
{
	/* not in use at this time */
}

/*************************************************************************
*
* Name:		foe_gre_node_created()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: Backcall from the controlled gre module
*	when a node will be created.
*	Fetches the current class from the palette and 
*	creates a node of this class.
*
**************************************************************************/


void foe_gre_node_created (
    gre_ctx	grectx,
    unsigned long	current_node_type,
    float	x,
    float	y
)
{
	pwr_tClassId		class;
	foe_ctx			foectx;
	int			sts;
	vldh_t_node		node;
	watt_tCtx               watt;

	foectx = grectx->cp.parent_ctx;

	/* Get selected nodes from palette */
        sts =  pal_get_select( foectx->node_palctx, &class);
/*	sts = hipa_sel_get_type( foectx->node_palctx, &class);*/

	if ( EVEN(sts)) 
	{
	    /* SG 20.03.91 Inform the user that he has to choose a node*/
	    foe_message( foectx, "Choose an object from the palette");
	    BEEP;
	    return;
	}

	foe_disable_ldh_cb(foectx);
	sts = gre_create_node( grectx, class, x, y, &node);
	foe_enable_ldh_cb(foectx);
	error_msg( sts);
        if ( EVEN(sts)) return;
        
        switch ( class) {
	  case pwr_cClass_Text:
          case pwr_cClass_BodyText:
          case pwr_cClass_Head:
          case pwr_cClass_Title:
	    foe_attr_create(grectx->cp.parent_ctx, node);
            sts = foe_attrlist_get_by_node( node, &watt);
            if ( EVEN(sts)) return;
            sts = watt_open_changevalue( watt, "Text");
	    break;
	  default:
	    ;
        }
}

/*************************************************************************
*
* Name:		foe_gre_node_moved()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: Backcall from the controlled gre module when 
*	a node is moved. Does nothing.
*
**************************************************************************/

void 	foe_gre_node_moved (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		foe_gre_node_selected()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: Backcall from the controlled gre module
*	when a node is selected. Does nothing.
*
**************************************************************************/

void 	foe_gre_node_selected (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		foe_gre_con_selected()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
*
* Description: Backcall from the controlled gre module
*	when a connection is selected. Does nothing.
*
**************************************************************************/

void 	foe_gre_con_selected (
    gre_ctx	grectx
){
}

/*************************************************************************
*
* Name:		foe_gre_con_created()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: Backcall from the controlled gre module when
*	a connection should be created.
*	Checks syntax and gets a suitible connectionclass by foe_gsx.
*	If foectx->con_palette_managed is set the class is fetched 
*	from the connection palette. Creates a connection.
*
**************************************************************************/

void 	foe_gre_con_created (
	gre_ctx			grectx,
	double			x,
	double			y,
	vldh_t_node		source_obj,
	unsigned long		source_point,
	vldh_t_node		destination_obj,
	unsigned long		destination_point
)
{
	pwr_tClassId	con_class;
	pwr_tClassId 	user_class;
	int		sts;	
	foe_ctx		foectx;
	vldh_t_node	dest;
	unsigned long	destpoint;

	foectx = grectx->cp.parent_ctx;

	if ( grectx->trace_started)
	{
	  if ( !destination_obj)
	  {
	    sts = trace_create_analyse( grectx, x, y, source_obj, source_point);
	    if ( EVEN(sts))
	    {
	      foe_message( foectx, "Unable to find trace attribute");
	      BEEP;
	      return;
	    }
	  }
          return;
	}

        /* 
	SG 10.03.91 : If the user as asked to choose the connection from 
        the palette ( by setting the toggle buttton "user connection"
	under the menu entry "customize" )  you get the type that he has
	selected otherwise use the one that the routine gsx_check_connection
        gives to you by default 
	*/
	if ( foectx->con_palette_managed == 0 )
	  user_class = 0;
	else
	{
/*	  sts = hipa_sel_get_type( foectx->conn_palctx, &user_class);*/
	  sts = pal_get_select( foectx->con_palctx, &user_class);
	  if ( EVEN(sts)) 
	  {
	    /* SG 20.03.91 Inform the user that he has to choose a conn */
	    foe_message( foectx, "Choose a connection from the palette");
	    BEEP;
	    return;
	  }
	}

	if ( !destination_obj)
	{
	  foe_disable_ldh_cb(foectx);
	  sts = gsx_auto_create( foectx, x, y, source_obj, source_point,
		&dest, &destpoint);
	  foe_enable_ldh_cb(foectx);
	  if ( EVEN(sts)) return;
	}
	else
	{
	  dest = destination_obj;
	  destpoint = destination_point;
	}
	/* Check connection syntax */
	sts = gsx_check_connection( foectx, source_obj, source_point, 
		dest, destpoint, &con_class, user_class);	
	if ( sts == GSX__CONTYPE)
	{
	  foe_message( foectx, "Connected attributes are not of the same type");
	  BEEP;
	  return;
	}
	error_msg( sts);
	if ( EVEN(sts)) return;


	if ( user_class)
	  con_class = user_class;

	sts = gre_create_con( grectx, con_class, source_obj, source_point,
		dest, destpoint, foectx->con_drawtype);
	error_msg( sts);
	if ( EVEN(sts)) return;
}

/*************************************************************************
*
* Name:		foe_gre_region_selected()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: 
*	Backcall from the controlled gre module when
*	a region is selected. Does nothing.
*
**************************************************************************/

void 	foe_gre_region_selected (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		foe_gre_enter_leave()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
* Description: 
*	Backcall from the controlled gre module when
*	the cursor enters or leaves the neted widget.
*	Does nothing.
*
**************************************************************************/

void 	foe_gre_enter_leave (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		foe_gre_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* unsigned long object		I	vldh  node or connection.
* unsigned long	object_type	I	type of object (VLDH_NODE or VLDH_CON)
*
* Description: 
*	Backcall from the controlled gre module.
*	Delete of selected objects or one single object.
*	If the parameter object is zero all selected objects is deleted.
*	If the parameter object is not zero the object is deleted.
*	Checks first that no object that is to be deleted
*	has a subwindow.
*	
**************************************************************************/

void foe_gre_delete (
    gre_ctx		grectx,
    void		*object,
    unsigned long	object_type
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, subwind_found;
	

	if ( object == 0 )
	{
	  /* Check that any node doesn't contain a subwindow */
	  gre_get_selnodes( grectx, &node_count, &nodelist);
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
	  if ( node_count > 0) XtFree((char *)nodelist);
	  if ( subwind_found )
 	  {
	    foe_message( grectx->cp.parent_ctx, 
	     "Subwindow found, delete the subwindow first");
	    BEEP;
	  }
	  else
	    /* Delete selected nodes */
	    gre_delete( grectx);
	}
	else
 	{
	  if ( object_type == VLDH_NODE )
 	  {
	    /* Check that the node doesn't have any subwindow!!!! */
	    if (((vldh_t_node) object)->ln.subwindow != 0 )
   	    {
	      foe_message( grectx->cp.parent_ctx, 
		  "Subwindow found, delete the subwindow first");
	      BEEP;
	    }
	    else
	      gre_delete_node( grectx, (vldh_t_node)object);
	  }
	  else if ( object_type == VLDH_CON )
	  {
	     /* Delete the connection */
	    gre_delete_con( grectx, (vldh_t_con)object);
	  }
	}
}
/*************************************************************************
*
* Name:		foe_gre_cut()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* unsigned long object		I	vldh  node or connection.
* unsigned long	object_type	I	type of object (VLDH_NODE or VLDH_CON)
*
* Description: 
*	Backcall from the controlled gre module.
*	Cut of selected objects or one single object.
*	If the parameter object is zero all selected objects are cut.
*	If the parameter object is not zero the object is cut.
*	Checks first that no object that is to be deleted
*	has a subwindow.
*	If the object is connection it is just deleted.
*
**************************************************************************/

void foe_gre_cut (
	gre_ctx		grectx,
	void		*object,
	unsigned long	object_type
)
{
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		*node_ptr;
	int			i, subwind_found;
	int 			sts;
	foe_ctx			foectx;

	if ( object == 0 )
	{
	  /* Check that any node doesn't contain a subwindow */
	  gre_get_selnodes( grectx, &node_count, &nodelist);
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
	    foe_message( grectx->cp.parent_ctx, 
	     "Subwindow found, delete the subwindow first");
	    BEEP;
	  }
	  else
	  {
	    /* Cut selected nodes */
	    sts = gre_cut( grectx);

	    if ( sts == GRE__NOREGSEL)
	    {
	      foectx = grectx->cp.parent_ctx;
	      foe_message( foectx, "Select region before cutting");
	      BEEP;
	    }
	    else if ( EVEN(sts)) error_msg( sts);
	  }
	}
	else
	{
	  if ( object_type == VLDH_NODE )
	  {
	    /* Check that the node doesn't have any subwindow!!!! */
	    if (((vldh_t_node) object)->ln.subwindow != 0 )
	    {
	      foe_message( grectx->cp.parent_ctx, 
		  "Subwindow found, delete the subwindow first");
	    }
	    else
	    gre_cut_node( grectx, (vldh_t_node)object);
	  }
	  else if ( object_type == VLDH_CON )
	  {
	     /* Just delete the connection */
	    gre_delete_con( grectx, (vldh_t_con)object);
	  }
	}
}
/*************************************************************************
*
* Name:		foe_gre_copy()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* unsigned long object		I	vldh  node or connection.
* unsigned long	object_type	I	type of object (VLDH_NODE or VLDH_CON)
*
* Description: 
*	Backcall from the controlled gre module.
*	Copy of selected objects or one single object.
*	If the parameter object is zero all selected objects are copied.
*	If the parameter object is not zero the object is copied.
*
**************************************************************************/

void foe_gre_copy (
    gre_ctx	    grectx,
    void	    *object,
    unsigned long   object_type
)
{
	int	sts;

	if ( object == 0 )
	{
	  /* Copy selected region */
	  sts = gre_copy( grectx);
	  if ( ODD(sts))
	    foe_message( grectx->cp.parent_ctx, "Region copied");
	}
	else
	{
	  if ( object_type == VLDH_NODE )
	  {
	    sts = gre_copy_node( grectx, (vldh_t_node)object);
	    if ( ODD(sts))
	      foe_message( grectx->cp.parent_ctx, "Object copied");
	  }
	}
}
/*************************************************************************
*
* Name:		foe_gre_paste()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* float		x		I	x koordinate for mouse click.
* float		y		I	y koordinate for mouse click.
*
* Description: 
*	Backcall from the controlled gre module
*	Paste objects in vldh paste singlenode.
*	Paste a nodes that is cut or copied as a single node.
*
**************************************************************************/

void foe_gre_paste (
    gre_ctx	grectx,
    float		x,
    float		y
)
{
	foe_ctx	foectx;

	foectx = grectx->cp.parent_ctx;

	foe_disable_ldh_cb(foectx);
	gre_paste( grectx, x, y, GRE_PASTE_CURSOR);
	foe_enable_ldh_cb(foectx);
}
/*************************************************************************
*
* Name:		foe_gre_attribute()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* vldh_t_node	object		I	object to start ate for.
*
*
* Description: 
*	Backcall from the controlled gre module
*	Starts the attribute editor for the object.
*
**************************************************************************/

void foe_gre_attribute (
    gre_ctx	grectx,
    vldh_t_node	object
)
{

	foe_attr_create(grectx->cp.parent_ctx, object);
}
/*************************************************************************
*
* Name:		foe_gre()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* vldh_t_node 	object		I	vldh node.
* unsigned long	subwindow_nr	I	window index for subwindow.
*
*
* Description: 
*	Backcall from the controlled gre module
*	Create a subwindow with the specified windowindex or the object.
*
**************************************************************************/

void foe_gre_subwindow (
    gre_ctx		grectx,
    vldh_t_node 	object,
    unsigned long	subwindow_nr
)
{
	int	sts;

	sts = foe_subwindow_create( grectx->cp.parent_ctx, object, 
		subwindow_nr);
	error_msg(sts);

}

/*************************************************************************
*
* Name:		foe_gre_popupmenu()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* int		x_pix		I	x koord in pixels	
* int		y_pix		I	y koord in pixels
*
*
* Description: 
*	Backcall from the controlled gre module
*	Not yet implemented.
*
**************************************************************************/

void foe_gre_popupmenu (
    gre_ctx	grectx,
    int	x_pix,
    int	y_pix,
    int	popupmenu_type,
    vldh_t_node	node
)
{
	foe_ctx	foectx;
	XEvent event;

	foectx = grectx->cp.parent_ctx;

	event.xbutton.x_root = x_pix + 5;
	event.xbutton.y_root = y_pix;
	event.xbutton.x = 0;
	event.xbutton.y = 0;
	foectx = grectx->cp.parent_ctx;

	foectx->popupmenu_node = 0;
	if ( foectx->function == EDIT)
	{
	  if ( popupmenu_type == GRE_POPUPMENUMODE_OBJECT)
	  {
	    if ( node != 0)
	    {
	      gre_node_select( grectx, node);
	      foectx->popupmenu_node = node;
	    }

	    foe_modify_popup ( foectx,
		foe_e_popupmenu_attribute	|
		foe_e_popupmenu_subwindow	|
		foe_e_popupmenu_connect		|
		foe_e_popupmenu_delete);

	    XmMenuPosition(foectx->widgets.popupmenu, &event.xbutton );
            XtManageChild (foectx->widgets.popupmenu);
	  }
	  else
	  {
	    foe_modify_popup ( foectx,
		foe_e_popupmenu_delete		|
		foe_e_popupmenu_copy		|
		foe_e_popupmenu_cut		|
		foe_e_popupmenu_printselect	);
	    XmMenuPosition(foectx->widgets.popupmenu, &event.xbutton );
	    XtManageChild (foectx->widgets.popupmenu);
	  }
	}
	else
	{
	  if ( node != 0)
	  {
	    gre_node_select( grectx, node);
	    foectx->popupmenu_node = node;
	  }
	  foe_modify_popup ( foectx,
		foe_e_popupmenu_attribute |
		foe_e_popupmenu_subwindow);
	  XmMenuPosition(foectx->widgets.popupmenu, &event.xbutton );
	  XtManageChild (foectx->widgets.popupmenu);
	}
}

/*************************************************************************
*
* Name:		int	foe_gre_getobj (grectx , node )
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* vldh_t_node	node		I	node that should be connected.
*
* Description: 
*	Backcall from the controlled gre module.
*	Calls the module foe_gobj to make connect operation for 
*	node.
*
**************************************************************************/

void foe_gre_getobj (
    gre_ctx	grectx,
    vldh_t_node node,
    unsigned long	index
)
{
  foe_ctx	foectx;
  int sts ;

  foectx = grectx->cp.parent_ctx;
  sts = gobj_get_object ( foectx , node, index );
  error_msg(sts);
}


/*************************************************************************
*
* Name:		int	foe_gre_undelete ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
*
* Description: 
*	Backcall from the controlled gre module.
*	Does nothing.
*
**************************************************************************/

void foe_gre_undelete (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		int	foe_gre_unselect ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
*
* Description: Backcall from the controlled gre module
*	Does nothing.
*
**************************************************************************/

void foe_gre_unselect (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		int	foe_gre_help()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* char		*help_title	I	help subject.
*
* Description: 
*	Backcall from the controlled gre module
*	Starts the helpwidget with the passed subject ( class of an object).
*
**************************************************************************/

void foe_gre_help (
    gre_ctx	grectx,
    char	*help_title
)
{

        foe_display_help ( grectx->cp.parent_ctx , help_title ) ;
#ifdef DEBUG
	printf(" foe_gre_help %s\n",help_title);
#endif
}

/*************************************************************************
*
* Name:		foe_gre_regionmoved()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
*
*
* Description: 
*	Backcall from the controlled gre module
*	Does nothing.
*
**************************************************************************/

void foe_gre_regionmoved (
    gre_ctx	grectx
)
{
}

/*************************************************************************
*
* Name:		foe_gre_message()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* gre_ctx	*grectx		I	gre context
* char		*message	I	message sent from gre.
*
*
* Description: 
*	Backcall from the controlled gre module
*	Recieves messages from gre and displays them with
*	foe_message.
*
**************************************************************************/

void foe_gre_message (
    gre_ctx	grectx,
    char	*message
)
{
	/* 
	SG 19.03.91
	send the message received from gre modules to the foe routine
	that will write them on the label widget . The backcalls routines
	foe_gre_xxx should never erase a message because there is chance that
	the message to be erased has just been set by a gre routine
	*/

	foe_ctx foectx;
	int l;

        foectx = grectx->cp.parent_ctx ;
	l= strlen(message) ;
	if ( foectx->msg_label_id != 0 && l ==0 ) 
            /* erase a message only if there is one to be erased */
	    foe_message( grectx->cp.parent_ctx,"");
        else if (l !=0 )
	    /* otherwise just send the message to be set */
	       foe_message( grectx->cp.parent_ctx, message);
	else
	  gre_search_rectangle_delete( foectx->grectx);
}


