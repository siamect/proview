/* wb_login.c -- login

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined OS_VMS
#include <clidef.h>
#include <climsgdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <lib$routines.h> 
#endif

#include "wb_foe_macros.h"

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>

#include "wb_login_msg.h"
#include "wb_login.h"
#include "wb_utl.h"
#include "co_api.h"
#include "co_dcli.h"

pwr_dExport login_sPrv  login_prv;

void	login_delete();
void	login_message();

#define	BEEP	    putchar( '\7' );

/*  Local function prototypes.	*/

static void login_activate_ok (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_activate_cancel (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_create_label (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_create_adb (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_create_usernamevalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_create_passwordvalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_valchanged_passwordvalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);
static void login_valchanged_usernamevalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
);


/*************************************************************************
* Name:		void	login_activate_ok()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

static void login_activate_ok (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{
  int	sts;

  login_message( loginctx, "");
  sts = login_get_values( loginctx);
  if ( ODD(sts)) 
  {
    login_delete( loginctx);
    if (loginctx->login_bc_success != NULL)
      (loginctx->login_bc_success) ();
    return;
  }
  else
  {
    login_message( loginctx, "User not authorized");
    printf( "User not authorized\n");
    BEEP;
    strcpy( (char *) &loginctx->password, "");
  }

}

/*************************************************************************
* Name:		void	login_activate_cancel()
*
* Description:
*	Callback from the pushbutton.
**************************************************************************/

static void login_activate_cancel (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{

  login_delete( loginctx);
  if (loginctx->login_bc_cancel != NULL)
    (loginctx->login_bc_cancel) ();
}

static void login_create_label (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
) 
{

  loginctx->widgets.label = w;
}

static void login_create_adb (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{

  loginctx->widgets.adb = w;
}

static void login_create_usernamevalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{

  loginctx->widgets.usernamevalue = w;
}

static void login_create_passwordvalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{

  loginctx->widgets.passwordvalue = w;
}


/*************************************************************************
*
* Name:		void	login_valchanged_passwordvalue ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx *	loginctx		I	Context adress
* Widget	w		I	neted widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback when value changed.
*
**************************************************************************/

static void login_valchanged_passwordvalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{
  char	*value;
  int	sts;
  char	*s;

  if ( loginctx->widgets.passwordvalue == 0)
    return;

  value = XmTextGetString( loginctx->widgets.passwordvalue);
  if ( *value == 0)
    return;

  s = strrchr( value, 10);

  if (s == 0) 
  {
    strcat(loginctx->password, value);
    XtFree( value);
    XmTextSetString( loginctx->widgets.passwordvalue, "");
    return;
  }
  
  login_message( loginctx, "");
  sts = login_get_values( loginctx);
  if ( ODD(sts)) 
  {
    if (loginctx->login_bc_success != NULL)
      (loginctx->login_bc_success) ();
    XtFree( value);
    login_delete( loginctx);
    return;
  }
  else
  {
    login_message( loginctx, "User not authorized");
    BEEP;
    printf( "User not authorized\n");
    XmTextSetString( loginctx->widgets.passwordvalue, "");
    strcpy(loginctx->password, "");

    XtFree( value);
  }


}


/*************************************************************************
*
* Name:		void	login_valchanged_usernamevalue ()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx *	loginctx	I	Context adress
* Widget	w		I	widget
* XmAnyCallbackStruct * data	I	callbackdata
*
*
* Description:
*	Callback when value changed.
*
**************************************************************************/

static void login_valchanged_usernamevalue (
  Widget		w,
  login_ctx		loginctx,
  XmAnyCallbackStruct	*data
)
{
  char	*value;
  char	*s;

  if ( loginctx->widgets.usernamevalue == 0)
    return;

  value = XmTextGetString( loginctx->widgets.usernamevalue);
  if ( *value == 0)
    return;

  s = strrchr( value, 10);

  if (s == 0) 
  {
    XtFree( value);
    return;
  }
  
  login_message( loginctx, "");
  strcpy( s, s + 1);	
  XmTextSetString( loginctx->widgets.usernamevalue, value);
  XtFree( value);

  /* Focus password */
  XtSetKeyboardFocus( loginctx->widgets.toplevel,
    loginctx->widgets.passwordvalue);
}


/*************************************************************************
*
* Name:		int	login_new()
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

pwr_tStatus login_new (
  void		*parent_ctx,
  Widget	parent_wid,
  char		*name,
  char		*groupname,
  voidp		login_bc_success,
  voidp		login_bc_cancel
)
{
  Arg	args[20];
  int	sts;
  int 	i;
  login_ctx	loginctx;
  /* DRM database hierarchy related variables */
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		uid_filename[200] = {"pwr_exe:wb_login.uid"};
  char		*uid_filename_p = uid_filename;


  static MrmRegisterArg	reglist[] = {
  /* First the context variable */
  { "login_ctx", 0 },

  /* Callbacks for the controlled login widget */
  {"login_create_adb",(caddr_t)login_create_adb},
  {"login_create_label",(caddr_t)login_create_label},
  {"login_create_usernamevalue",(caddr_t)login_create_usernamevalue},
  {"login_create_passwordvalue",(caddr_t)login_create_passwordvalue},
  {"login_valchanged_usernamevalue",(caddr_t)login_valchanged_usernamevalue},
  {"login_valchanged_passwordvalue",(caddr_t)login_valchanged_passwordvalue},
  {"login_activate_ok",(caddr_t)login_activate_ok},
  {"login_activate_cancel",(caddr_t)login_activate_cancel},
  };

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

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
  loginctx = (login_ctx) XtCalloc( 1, sizeof(*loginctx));

  loginctx->cp.parent_ctx = parent_ctx;
  loginctx->cp.name = XtNewString( name );

  /* set initialization values in context */

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) loginctx);

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

  reglist[0].value = (caddr_t) loginctx;

  MrmRegisterNames(reglist, reglist_num);

  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  
  /* Save the id of the top in the context */ 
  loginctx->widgets.toplevel = XtCreatePopupShell (
    "login", topLevelShellWidgetClass, parent_wid, args, i);

  /* the positioning of a top level can only be define after the creation
   *   of the widget . So i do it now: 
   *  use the parameters received x and y 
   */

  i=0;
  XtSetArg(args[i],XmNx,100);i++;
  XtSetArg(args[i],XmNy,100);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  XtSetValues( loginctx->widgets.toplevel  ,args,i);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_DRMh, "login_window",
    loginctx->widgets.toplevel, name, args, 1, 
    &loginctx->widgets.login_window, &dclass);

  if (sts != MrmSUCCESS) printf("can't fetch utedit widget\n");

  XtManageChild(loginctx->widgets.login_window);

  /* SG 09.02.91 a top level should always be realized ! */
  XtPopup( loginctx->widgets.toplevel, XtGrabNone );

  MrmCloseHierarchy(s_DRMh);

  /* Fill in backcall routines */  
  loginctx->login_bc_success = login_bc_success;
  loginctx->login_bc_cancel = login_bc_cancel;

  /* Store group name in context */
  strcpy(loginctx->groupname, groupname);

  /* Focus username ... */

  return LOGIN__SUCCESS;
}


/*************************************************************************
*
* Name:		loginctx()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx	*loginctx		I	login context.
*
* Description:
*	Destroys a login instance.
*	Destroys the widget and frees allocated memory for the 
*	context.
*
**************************************************************************/

void login_delete (
  login_ctx	loginctx
)
{

  /* Destroy the widget */
  XtDestroyWidget(loginctx->widgets.toplevel) ;

  /* Delete the context */
  XtFree((char *) loginctx);
}


/*************************************************************************
*
* Name:		login_get_values()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx	*loginctx		I	login context.
*
* Description:
*	Get values in username and password.
*
**************************************************************************/

pwr_tStatus login_get_values (
  login_ctx	loginctx
)
{
  char	password[40];
  char	username[40];
  char	*value;
  pwr_tStatus	sts;

  /* Get UserName */
  value = XmTextGetString( loginctx->widgets.usernamevalue);
  strcpy(username, value);
  XtFree( value);

  /* Get Password */
  strcpy(password, loginctx->password);

  sts = login_user_check(loginctx->groupname, username, password);
  if ( EVEN(sts))
    return sts;

  return LOGIN__SUCCESS;
}


/*************************************************************************
*
* Name:		login_user_check()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx	*loginctx		I	login context.
*
* Description:
*	Check username and password and insert login infomation.
*
**************************************************************************/

pwr_tStatus login_user_check  (
  char			*groupname,
  char			*username,
  char			*password
)
{
  pwr_tStatus		sts;
  unsigned int		priv;
  unsigned long		attr = 0;
  
  sts = user_CheckUser( groupname, username, password, 
	&priv);
  if ( EVEN(sts)) return sts;

  login_insert_login_info( groupname, password,
      username, priv, attr);
  return LOGIN__SUCCESS;
}


/*************************************************************************
*
* Name:		login_insert_login_info()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* login_ctx	*loginctx		I	login context.
*
* Description:
*	Inserts login info in global priv struct.
*
**************************************************************************/

pwr_tStatus login_insert_login_info (
  char		*groupname,
  char		*password,
  char		*username,
  unsigned long	priv,
  unsigned long	attr
)
{

  strcpy(login_prv.username, username);
  strcpy(login_prv.password, password);
  strcpy(login_prv.group, groupname);
  login_prv.priv = priv;
  login_prv.attribute = attr;

  return LOGIN__SUCCESS;
}


/*************************************************************************
*
* Name:		login_message()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* login_ctx	loginctx		I	login context.
* char 		*new_label	I	message to show. 
*
* Description:
*	Displays a message in the login window.
*
**************************************************************************/

void login_message ( 
  login_ctx	loginctx,
  char 		*new_label 
)
{
  Arg	args[2];

  XtSetArg(args[0], XmNlabelString, 
	  XmStringCreateLtoR(new_label , "ISO8859-1"));
  XtSetValues( loginctx->widgets.label ,args, 1);
}


/*************************************************************************
*
* Name:		login_db_show_useritem()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Writes a userlist.
*
**************************************************************************/

pwr_tStatus login_show_priv ( 
	void
)
{

  printf("\n");
  printf("Authorized privileges for    User:        %s\n", login_prv.username);
  printf("                             SystemGroup: %s\n", login_prv.group);
  if ( login_prv.priv & pwr_mPrv_RtRead)
    printf("	RtRead\n");
  if ( login_prv.priv & pwr_mPrv_RtWrite)
    printf("	RtWrite\n");
  if ( login_prv.priv & pwr_mPrv_DevRead)
    printf("	DevRead\n");
  if ( login_prv.priv & pwr_mPrv_DevPlc)
    printf("	DevPlc\n");
  if ( login_prv.priv & pwr_mPrv_DevConfig)
    printf("	DevConfig\n");
  if ( login_prv.priv & pwr_mPrv_DevClass)
    printf("	DevClass\n");

  return LOGIN__SUCCESS;
}

