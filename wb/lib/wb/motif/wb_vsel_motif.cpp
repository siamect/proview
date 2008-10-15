/* 
 * Proview   $Id: wb_vsel_motif.cpp,v 1.3 2008-10-15 06:04:55 claes Exp $
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

/* wb_vsel_motif.cpp -- select volume */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <Xm/List.h>
#include <X11/Intrinsic.h>
#include <Xm/MessageB.h>

#include "rt_load.h"

#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "pwr.h"
#include "pwr_systemclasses.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb.h"
#include "wb_ldh.h"
#include "wb_foe.h"
#include "wb_utl_api.h"
#include "wb_lfu.h"
#include "co_login.h"
#include "co_login_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh_msg.h"
#include "co_msgwindow.h"
#include "co_wow_motif.h"
#include "wb_vsel_motif.h"

#define	BEEP	    putchar( '\7' );

/*  Local function prototypes.	*/

static void vsel_exit_qb_yes( void *ctx, void *cbdata);

static void vsel_exit_qb_yes( void *ctx, void *cbdata)
{
  WVsel *vsel = (WVsel *) ctx;

  if (vsel->vsel_bc_cancel != NULL)
    (vsel->vsel_bc_cancel) ();
  delete vsel;
}


/*************************************************************************
* Name:		void	vsel_activate_ok()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselMotif::activate_ok( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{
  int		sts;
  int 		*pos_list, pos_cnt;
  int 		i;
  pwr_tVolumeId	*volume_ptr;

  volume_ptr = (pwr_tVolumeId *) calloc( vsel->volume_count, 
					 sizeof( pwr_tVolumeId));
  if (XmListGetSelectedPos( vsel->widgets.volumelist, 
			    &pos_list, &pos_cnt))  {
    for (i = 0; i < pos_cnt; i++) {
      *(volume_ptr + i) = vsel->volumes[ pos_list[i] - 1];
    }
  }

  if (vsel->vsel_bc_success != NULL)
    sts = (vsel->vsel_bc_success) ( vsel, volume_ptr, pos_cnt);
  free( (char *) volume_ptr);

  if ( ODD(sts)) {
    if (vsel->vsel_bc_cancel != NULL)
      (vsel->vsel_bc_cancel) ();
    delete vsel;
  }
  else if ( sts == LDH__VOLALRATT) {
    vsel->wow->DisplayError( "Error",
			     "         Volume is already open        ");
  }
}

/*************************************************************************
* Name:		void	vsel_activate_cancel()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselMotif::activate_cancel( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{

  if (vsel->vsel_bc_cancel != NULL)
    (vsel->vsel_bc_cancel) ();
  delete vsel;
}


/*************************************************************************
* Name:		void	vsel_activate_close()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselMotif::activate_close( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{
  if ( (vsel->vsel_bc_time_to_exit) ( vsel->parent_ctx)) {
    vsel->wow->DisplayQuestion( vsel,
			 "Pwr exit", "Do you really want to exit ?",
			 vsel_exit_qb_yes, NULL, NULL);
  }
  else {
    if (vsel->vsel_bc_cancel != NULL)
      (vsel->vsel_bc_cancel) ();
    delete vsel;
  }
}

/*************************************************************************
* Name:		void	vsel_activate_showall()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVselMotif::activate_showall( Widget w, WVselMotif *vsel, XmToggleButtonCallbackStruct *data)
{
  pwr_tStatus sts;

  if ( data->set > 1) return;
  vsel->all = data->set;

  sts = vsel->load_volumelist();
  WFoe::error_msg(sts);
}


void WVselMotif::create_label( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{

  vsel->widgets.label = w;
}

void WVselMotif::create_adb( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{

  vsel->widgets.adb = w;
}

void WVselMotif::destroy_adb( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{
}

void WVselMotif::action_volumelist( Widget w, WVselMotif *vsel, XmListCallbackStruct *data)
{
  int		sts;
  pwr_tVolumeId	*volume_ptr;

  if ( data->event->type == KeyPress)
    // The ok callback will be called later
    return;

  volume_ptr = (pwr_tVolumeId *) calloc( 1, sizeof( pwr_tVolumeId));

  *volume_ptr = vsel->volumes[ data->item_position - 1];

  if (vsel->vsel_bc_success != NULL)
    sts = (vsel->vsel_bc_success) ( vsel, volume_ptr, 1);
  free( (char *) volume_ptr);

  if ( ODD(sts)) {
    if (vsel->vsel_bc_cancel != NULL)
      (vsel->vsel_bc_cancel) ();
    delete vsel;
  }
  else if ( sts == LDH__VOLALRATT) {
    vsel->wow->DisplayError( "Error",
		       "         Volume is already open        ");
  }
}

void WVselMotif::create_volumelist( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data)
{
  pwr_tStatus	sts;

  vsel->widgets.volumelist = w;

  sts = vsel->load_volumelist();
  WFoe::error_msg(sts);

}


/*************************************************************************
*
* Name:		int	vsel_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char *	name		I	Name of the created object
*
* Description:
*	Create a new login window
**************************************************************************/

WVselMotif::WVselMotif (
  pwr_tStatus   *status,
  void		*wv_parent_ctx,
  Widget	wv_parent_wid,
  char		*wv_name,
  ldh_tWBContext wv_wbctx,
  char		*volumename,
  int		(*bc_success)( void *, pwr_tVolumeId *, int),
  void		(*bc_cancel)(),
  int		(*bc_time_to_exit)( void *),
  int		show_volumes,
  wb_eType      wv_wb_type) : 
  WVsel(status,wv_parent_ctx,wv_name,wv_wbctx,volumename,bc_success,bc_cancel,
	bc_time_to_exit,show_volumes,wv_wb_type), parent_wid(wv_parent_wid)
{
  Arg		args[20];
  int		sts;
  int 		i;
  pwr_tVolumeId volid, *volume_ptr;
  pwr_tVolumeId	volume;
  pwr_tClassId	classid;
  char		volname[80];
  int		size;
  int		volume_count;
  int		display_window;

  /* DRM database hierarchy related variables */
  MrmHierarchy 	s_DRMh;
  MrmType 	dclass;
  char		uid_filename[200] = {"pwr_exe:wb_vsel.uid"};
  char		*uid_filename_p = uid_filename;


  static MrmRegisterArg	reglist[] = {
  /* First the context variable */
  { "vsel_ctx", 0 },
  { "vsel_write_priv", 0 },

  /* Callbacks for the controlled login widget */
  {"vsel_create_adb",(caddr_t)create_adb},
  {"vsel_destroy_adb",(caddr_t)destroy_adb},
  {"vsel_create_label",(caddr_t)create_label},
  {"vsel_activate_ok",(caddr_t)activate_ok},
  {"vsel_activate_cancel",(caddr_t)activate_cancel},
  {"vsel_activate_close",(caddr_t)activate_close},
  {"vsel_activate_showall",(caddr_t)activate_showall},
  {"vsel_create_volumelist",(caddr_t)create_volumelist},
  {"vsel_action_volumelist",(caddr_t)action_volumelist}
  };

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts)) {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

  strcpy( volname, "");
  if ( volumename != NULL && *volumename != 0 && !show_volumes)
    /* Start the navigater for this volume */
    strcpy( volname, volumename);
  else if ( !show_volumes && wb_type != wb_eType_Buffer)
  {
    /* If there is only one volume in the db, select this volume */
    volume_count = 0;

    sts = ldh_GetVolumeList( wbctx, &volume);
    while ( ODD(sts) )
    {
      sts = ldh_GetVolumeClass( wbctx, volume, &classid);
      if (EVEN(sts)) { *status = sts; return; }
  
      if ( cdh_isClassVolumeClass( classid)
 	   classid == pwr_eClass_WorkBenchVolume ||
	   volume == ldh_cRtVolume)
      {
        sts = ldh_GetNextVolume( wbctx, volume, &volume);
        continue;
      }
      sts = ldh_VolumeIdToName( wbctx, volume, volname, sizeof(volname),
	    &size);
      if (EVEN(sts)) { *status = sts; return; }
      volume_count++;

      sts = ldh_GetNextVolume( wbctx, volume, &volume);
    }
    if ( volume_count != 1)
      strcpy( volname, "");
  }

  
  //  If volume name is supplied, find this volume and open the navigator.

  if ( strcmp( volname, "") && wb_type != wb_eType_Buffer)
    /* Check syntax, if new volumes is found, show the window */
    sts = check_volumelist( 1, &display_window);

  if ( strcmp( volname, "") && !display_window)
  {
    sts = ldh_VolumeNameToId( wbctx, volname, &volid);
    WFoe::error_msg(sts);
    if ( ODD(sts))
    {
      volume_ptr = (pwr_tVolumeId *) calloc( 1, sizeof( pwr_tVolumeId));
      *volume_ptr = volid;
      (vsel_bc_success) ( (void *)this, volume_ptr, 1);
      if (vsel_bc_cancel != NULL)
        (vsel_bc_cancel) ();

      *status = LOGIN__SUCCESS;
      return;
    }
    else
      printf( "-- Unable to open volume '%s', volume doesn't exist\n",
	volname);
  }
 

  
  // Now start the module creation

  // set initialization values in context */

  // Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /*
   * Create a new widget
   * Open the UID files (the output of the UIL compiler) in the hierarchy
   * Register the items DRM needs to bind for us.
   * Create a new widget
   * Close the hierarchy
   * Compile the additional button translations and augment and add actions
   */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open hierarchy\n");

  reglist[0].value = (caddr_t) this;
  reglist[1].value = (caddr_t) write_priv;

  MrmRegisterNames(reglist, reglist_num);

  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  
  /* Save the id of the top in the context */ 
  widgets.toplevel = XtCreatePopupShell (
    "selectvolume", topLevelShellWidgetClass, parent_wid, args, i);

  /* the positioning of a top level can only be define after the creation
   *   of the widget . So i do it now: 
   *  use the parameters received x and y 
   */

  i=0;
  XtSetArg(args[i],XmNx,100);i++;
  XtSetArg(args[i],XmNy,100);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  XtSetValues( widgets.toplevel  ,args,i);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_DRMh, "vsel_window",
    widgets.toplevel, name, args, 1, 
    &widgets.vsel_window, &dclass);

  if (sts != MrmSUCCESS) printf("can't fetch utedit widget\n");

  XtManageChild( widgets.vsel_window);

  /* SG 09.02.91 a top level should always be realized ! */
  XtPopup( widgets.toplevel, XtGrabNone );


  MrmCloseHierarchy(s_DRMh);

  wow = new CoWowMotif( widgets.vsel_window);

  if ( wb_type != wb_eType_Buffer)
    sts = check_volumelist( 0, &display_window);

  // Set input focus to the scrolled list widget
  XmProcessTraversal( widgets.volumelist, XmTRAVERSE_CURRENT);

  *status = LOGIN__SUCCESS;
}



WVselMotif::~WVselMotif()
{

  /* Destroy the widget */
  XtDestroyWidget( widgets.toplevel);

}


/*************************************************************************
*
* Name:		message()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char 		*new_label	I	message to show. 
*
* Description:
*	Displays a message in the vsel window.
*
**************************************************************************/

void WVselMotif::message( char *new_label)
{
  Arg	args[2];

  XtSetArg(args[0], XmNlabelString, 
	  XmStringCreateLtoR(new_label , "ISO8859-1"));
  XtSetValues( widgets.label ,args, 1);
}

void WVselMotif::list_clear()
{
  XmListDeleteAllItems( widgets.volumelist);
}

void WVselMotif::list_add_item( char *str)
{
  XmString 	cstr;  

  cstr = XmStringCreateSimple( str);
  XmListAddItemUnselected( widgets.volumelist, cstr, 0);
  XmStringFree(cstr);	  
}








