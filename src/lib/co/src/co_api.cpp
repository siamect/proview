#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "pwr.h"
}

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "co_msgwindow.h"
#include "co_xhelp.h"

extern "C" {
#include "co_api.h"
#include "co_dcli.h"
}

#include "co_user.h"
#include "co_lng.h"

//
// c-api to co_user
//

int user_CheckUser( char *systemgroup, char *user, char *password, 
	unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user( systemgroup, user, password, priv);
  delete gu;
  return sts;
}

int user_CheckSystemGroup( char *systemgroup)
{
  GeUser *gu;
  int sts;
  unsigned int attributes;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_system_data( systemgroup, &attributes);
  delete gu;
  return sts;
}

int user_GetUserPriv( char *systemgroup, char *user, unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user_priv( systemgroup, user, priv);
  delete gu;
  return sts;
}

void user_PrivToString( unsigned int priv, char *str, int size)
{
  GeUser::priv_to_string( priv, str, size);
}

void user_RtPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::rt_priv_to_string( priv, str, size);
}

void user_DevPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::dev_priv_to_string( priv, str, size);
}

//
//  c api to co_lng
//

void lng_get_uid( char *in, char *out)
{
  Lng::get_uid( in, out);
}

char *lng_translate( char *str)
{
  return Lng::translate( str);
}

//
// c api to co_msgwindow
//
void msgw_message( int severity, char *text, msgw_ePop pop)
{
  MsgWindow::message( severity, text, pop);
}

void msgw_message_sts( pwr_tStatus sts, char *text1, char *text2)
{
  MsgWindow::message( wb_error(sts), text1, text2);
}

void msgw_message_object( pwr_tStatus sts, char *text1, char *text2, pwr_tOid oid)
{
  MsgWindow::message( wb_error(sts), text1, text2, oid);
}

void msgw_message_plcobject( pwr_tStatus sts, char *text1, char *text2, pwr_tOid oid)
{
  MsgWindow::message( wb_error(sts), text1, text2, oid, true);
}

void msgw_set_nodraw()
{
  MsgWindow::dset_nodraw();
}

void msgw_reset_nodraw()
{
  MsgWindow::dreset_nodraw();
}

//
// c api to co_xhelp
//

int xhelp_help( char *key, char *help_bookmark, navh_eHelpFile file_type,
		char *file_name, int strict)
{
  return CoXHelp::dhelp( key, help_bookmark, file_type, file_name, strict != 0);
}




