/* wb_vsel -- select volume

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wb_foe_macros.h"

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <Xm/List.h>
#include <X11/Intrinsic.h>
#include <Xm/MessageB.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

extern "C" {
#include "pwr.h"
#include "pwr_systemclasses.h"
#include "rt_load.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb.h"
#include "wb_ldh.h"
#include "wb_vsel.h"
#include "wb_foe.h"
#include "wb_utl.h"
#include "wb_lfu.h"
#include "co_wow.h"
#include "wb_login.h"
#include "wb_login_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh_msg.h"
}


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

void WVsel::vsel_activate_ok (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
{
	int		sts;
	int 		*pos_list, pos_cnt;
	int 		i;
	pwr_tVolumeId	*volume_ptr;

	volume_ptr = (pwr_tVolumeId *) calloc( vsel->volume_count, 
		sizeof( pwr_tVolumeId));
	if (XmListGetSelectedPos( vsel->widgets.volumelist, 
		&pos_list, &pos_cnt)) 
	{
	  for (i = 0; i < pos_cnt; i++)
	  {
	    *(volume_ptr + i) = vsel->volumes[ pos_list[i] - 1];
	  }
	}

	if (vsel->vsel_bc_success != NULL)
	  sts = (vsel->vsel_bc_success) ( vsel, volume_ptr, pos_cnt);
        free( (char *) volume_ptr);

	if ( ODD(sts)) 
	{
	  if (vsel->vsel_bc_cancel != NULL)
	    (vsel->vsel_bc_cancel) ();
	  delete vsel;
	}
	else if ( sts == LDH__VOLALRATT)
	{
	  wow_DisplayError( vsel->widgets.vsel_window, "Error",
		"         Volume is already open        ");
	}
}

/*************************************************************************
* Name:		void	vsel_activate_cancel()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

void WVsel::vsel_activate_cancel (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
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

void WVsel::vsel_activate_close (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
{
  if ( (vsel->vsel_bc_time_to_exit) ( vsel->parent_ctx))
  {
    wow_DisplayQuestion( vsel, vsel->widgets.vsel_window,
		  "Pwr exit", "Do you really want to exit ?",
		  vsel_exit_qb_yes, NULL, NULL);
  }
  else
  {
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

void WVsel::vsel_activate_showall (
  Widget		w,
  WVsel		        *udata,
  XmToggleButtonCallbackStruct *data
)
{
	pwr_tStatus sts;
	WVsel *vsel = (WVsel *)udata;

	if ( data->set > 1) return;
	vsel->all = data->set;

	sts = vsel->load_volumelist();
	error_msg(sts);
}


void WVsel::vsel_create_label (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
) 
{

  vsel->widgets.label = w;
}

void WVsel::vsel_create_adb (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
{

  vsel->widgets.adb = w;
}

void WVsel::vsel_destroy_adb (
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
{
}

void WVsel::vsel_action_volumelist(
  Widget                w,
  WVsel *vsel,
  XmListCallbackStruct *data)
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

	if ( ODD(sts)) 
	{
	  if (vsel->vsel_bc_cancel != NULL)
	    (vsel->vsel_bc_cancel) ();
	  delete vsel;
	}
	else if ( sts == LDH__VOLALRATT)
	{
	  wow_DisplayError( vsel->widgets.vsel_window, "Error",
		"         Volume is already open        ");
	}
}

void WVsel::vsel_create_volumelist( 
  Widget		w,
  WVsel		*vsel,
  XmAnyCallbackStruct	*data
)
{
	pwr_tStatus	sts;

	vsel->widgets.volumelist = w;

	sts = vsel->load_volumelist();
	error_msg(sts);

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

WVsel::WVsel (
  pwr_tStatus   *status,
  void		*wv_parent_ctx,
  Widget	wv_parent_wid,
  char		*wv_name,
  ldh_tWBContext wv_wbctx,
  char		*volumename,
  int		(*bc_success)( void *, pwr_tVolumeId *, int),
  void		(*bc_cancel)(),
  int		(*bc_time_to_exit)( void *),
  int		show_volumes
  ) : parent_ctx(wv_parent_ctx), parent_wid(wv_parent_wid), wbctx(wv_wbctx),
      vsel_bc_success(bc_success), vsel_bc_cancel(bc_cancel),
      vsel_bc_time_to_exit( bc_time_to_exit), volume_count(0), all(0),
      write_priv(0)
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
  {"vsel_create_adb",(caddr_t)vsel_create_adb},
  {"vsel_destroy_adb",(caddr_t)vsel_destroy_adb},
  {"vsel_create_label",(caddr_t)vsel_create_label},
  {"vsel_activate_ok",(caddr_t)vsel_activate_ok},
  {"vsel_activate_cancel",(caddr_t)vsel_activate_cancel},
  {"vsel_activate_close",(caddr_t)vsel_activate_close},
  {"vsel_activate_showall",(caddr_t)vsel_activate_showall},
  {"vsel_create_volumelist",(caddr_t)vsel_create_volumelist},
  {"vsel_action_volumelist",(caddr_t)vsel_action_volumelist}
  };

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts))
  {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

  strcpy( name, wv_name);
  if ( login_prv.priv & pwr_mPrv_DevConfig )
    write_priv = 1;

  strcpy( volname, "");
  if ( volumename != NULL && *volumename != 0 && !show_volumes)
    /* Start the navigater for this volume */
    strcpy( volname, volumename);
  else if ( !show_volumes)
  {
    /* If there is only one volume in the db, select this volume */
    volume_count = 0;

    sts = ldh_GetVolumeList( wbctx, &volume);
    while ( ODD(sts) )
    {
      sts = ldh_GetVolumeClass( wbctx, volume, &classid);
      if (EVEN(sts)) { *status = sts; return; }
  
      if ( classid == pwr_eClass_ClassVolume ||
 	   classid == pwr_eClass_WorkBenchVolume )
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

  if ( strcmp( volname, ""))
    /* Check syntax, if new volumes is found, show the window */
    sts = check_volumelist( 1, &display_window);

  if ( strcmp( volname, "") && !display_window)
  {
    sts = ldh_VolumeNameToId( wbctx, volname, &volid);
    error_msg(sts);
    if ( ODD(sts))
    {
      volume_ptr = (pwr_tVolumeId *) XtCalloc( 1, sizeof( pwr_tVolumeId));
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

  sts = check_volumelist( 0, &display_window);

  // Set input focus to the scrolled list widget
  XmProcessTraversal( widgets.volumelist, XmTRAVERSE_CURRENT);

  *status = LOGIN__SUCCESS;
}



WVsel::~WVsel()
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

void WVsel::message(
  char 		*new_label 
)
{
  Arg	args[2];

  XtSetArg(args[0], XmNlabelString, 
	  XmStringCreateLtoR(new_label , "ISO8859-1"));
  XtSetValues( widgets.label ,args, 1);
}


/*************************************************************************
*
* Name:		load_list()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Load the volumelist.
*
**************************************************************************/

pwr_tStatus WVsel::load_volumelist()
{
	pwr_tVolumeId	volume;
	pwr_tClassId	classid;
	char		name[80];
	int		size;
	XmString 	cstr;  
	pwr_tStatus	sts;
	int		i;
	bool            local_wbvolume = false;

	XmListDeleteAllItems( widgets.volumelist);
	volume_count = 0;

	sts = ldh_GetVolumeList( wbctx, &volume);
	while ( ODD(sts) )
	{
	  sts = ldh_GetVolumeClass( wbctx, volume, &classid);
	  if (EVEN(sts)) return sts;

	  if ( !all)
	  {
	    if ( classid == pwr_eClass_ClassVolume ||
	         classid == pwr_eClass_WorkBenchVolume )
	    {
	      sts = ldh_GetNextVolume( wbctx, volume, &volume);
	      continue;
	    }
	  }
	  sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
	    &size);
	  if (EVEN(sts)) return sts;

	  strcat( name, " ");
	  for ( i = strlen(name); i < 20; i++)
	    strcat( name, " ");

	  switch (classid)
	  {
	    case pwr_eClass_RootVolume:
	      strcat( name, "RootVolume");
	      break;
	    case pwr_eClass_SubVolume:
	      strcat( name, "SubVolume");
	      break;
	    case pwr_eClass_SharedVolume:
	      strcat( name, "SharedVolume");
	      break;
	    case pwr_eClass_ClassVolume:
	      strcat( name, "ClassVolume");
	      break;
	    case pwr_eClass_WorkBenchVolume:
	      strcat( name, "WorkBenchVolume");
	      break;
	    case pwr_eClass_DirectoryVolume:
	      strcat( name, "DirectoryVolume");
	      break;
	  }
	  cstr = XmStringCreateSimple( name);
	  XmListAddItemUnselected( widgets.volumelist, cstr, 0);
	  XmStringFree(cstr);	  

	  /* Store the volume */
	  volumes[ volume_count] = volume;
	  volume_count++;
	  if ( volume_count >= VSEL_MAX_VOLUMES)
	    break;
	  if ( local_wbvolume)
	    break;

	  sts = ldh_GetNextVolume( wbctx, volume, &volume);
	  if ( EVEN(sts) && all) {
	    // Look for local workbench volume
	    volume = ldh_cWBVolLocal;
	    sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
				      &size);
	    local_wbvolume = true;
	  }
	}
	return 1;
}


/*************************************************************************
*
* Name:		check_volumelist()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Load the volumelist.
*
**************************************************************************/

pwr_tStatus WVsel::check_volumelist(
  int		quiet,
  int		*display_window
)
{
	pwr_tVolumeId	volume;
	pwr_tClassId	classid;
	char		name[80];
	int		size;
	pwr_tStatus	sts;
	int		i;
	lfu_t_volumelist *volumelist;
	lfu_t_volumelist *volumelist_ptr;
	int		volumecount;	
	int		class_error;
	char		volname_conf[80];
	char		volname_db[80];
	int		volume_found;
	int		error_count;
	int		errlen=0;
	char		errstr[800];

	error_count = 0;
	*display_window = 0;

	if ( !(login_prv.priv & pwr_mPrv_DevConfig ))
	  /* No privileges for edit, don't check the volumes */
	  return 1;

	/* Load the configured volume list */
	sts = lfu_volumelist_load( load_cNameVolumeList,  &volumelist, 
		&volumecount);
	if (sts == FOE__NOFILE)
	{
	  if ( !quiet)
	  {
	    printf( "** Error, project is not configured\n");
            BEEP;
 	    *display_window = 1;
	  }
	  return 1;
	}
	else if (EVEN(sts))
          return 1;

	sts = ldh_GetVolumeList( wbctx, &volume);
	while ( ODD(sts) )
	{
	  sts = ldh_GetVolumeClass( wbctx, volume, &classid);
	  if (EVEN(sts)) return sts;

	  sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
	    &size);
	  if (EVEN(sts)) return sts;

	  if ( classid == pwr_eClass_RootVolume ||
	       classid == pwr_eClass_SubVolume ||
	       classid == pwr_eClass_SharedVolume ||
	       (classid == pwr_eClass_ClassVolume && volume > 511))
	  {
	    /* This volume should be configured */
	    volume_found = 0;
            volumelist_ptr = volumelist;
	    for ( i = 0; i < volumecount; i++)
	    {
	      if ( volumelist_ptr->volume_id == volume)
	      {
	        /* Mark that volume is found */
	        volume_found = 1;
	        strcpy( volumelist_ptr->p3, "Found");

	        /* Check volume name */
	        utl_toupper( volname_conf, volumelist_ptr->volume_name);
	        utl_toupper( volname_db, name);
	        if ( strcmp( volname_db, volname_conf))
	        {
	          /* Volume name differs */
	          if ( !quiet && errlen < (int)(sizeof(errstr)-100))
	          {
	            errlen += sprintf( &errstr[errlen], 
		      "** Error, Volume %s is configured with another name '%s'\n", 
		      name, volumelist_ptr->volume_name);
                    BEEP;
	          }
	          error_count++;
	        }
	        class_error = 0;
	        switch( classid)
	        {
		  case pwr_eClass_RootVolume :
	            if ( strcmp( volumelist_ptr->p2, "RootVolume"))
		      class_error = 1;
	            break;
		  case pwr_eClass_SubVolume :
	            if ( strcmp( volumelist_ptr->p2, "SubVolume"))
		      class_error = 1;
	            break;
		  case pwr_eClass_ClassVolume :
	            if ( strcmp( volumelist_ptr->p2, "ClassVolume"))
		      class_error = 1;
	            break;
		  case pwr_eClass_SharedVolume :
	            if ( strcmp( volumelist_ptr->p2, "SharedVolume"))
		      class_error = 1;
	            break;
	        }
	        if ( class_error)
	        {
	          if ( !quiet && errlen < (int)(sizeof(errstr)-100))
	          {
	            errlen += sprintf( &errstr[errlen], 
	                "** Error, Volume %s is configured with another class '%s'\n", 
		        name, volumelist_ptr->p2);
                    BEEP;
	          }
	          error_count++;
	        }
	        break;
	      }
	      volumelist_ptr++;
	    }
	    if ( !volume_found)
	    {
	      if ( !quiet && errlen < (int)(sizeof(errstr)-100))
 	      {
	        errlen += sprintf( &errstr[errlen], 
	        	"** Error, Volume %s is not configured in the ProjectVolume\n", 
		        name);
                BEEP;
	      }	
	      error_count++;
	    }
	  }
	  sts = ldh_GetNextVolume( wbctx, volume, &volume);
	}

	if ( error_count)
	{
	  *display_window = 1;
	  if ( !quiet)
	  {
	    errlen += sprintf( &errstr[errlen],
	    	"\n   %d syntax error%s found\n", 
		error_count, (error_count == 1) ? "" : "s");
	    wow_DisplayError( widgets.vsel_window, "Syntax control",
		errstr);
	  }
	}
	return 1;
}




