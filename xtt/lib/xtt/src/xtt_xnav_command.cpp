/* xtt_xnav_command.cpp -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines for handling of command line in xnav. */


#include "flow_std.h"


# include <vector>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

#include "co_nav_help.h"

extern "C" {
#include "rt_gdh.h"
#include "pwr_privilege.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "co_ccm_msg.h"
#include "co_api.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

extern "C" {
#include "flow_x.h"
#include "rt_trace.h"
}
#include "co_lng.h"
#include "co_error.h"
#include "xtt_xnav.h"
#include "xtt_ge.h"
#include "xtt_item.h"
#include "xtt_xnav_crr.h"
#include "co_dcli_msg.h"
#include "rt_xnav_msg.h"
#include "co_xhelp.h"

#include "glow_curvectx.h"
#include "glow_curvewidget.h"
#include "ge_curve.h"
#include "xtt_trend.h"
#include "xtt_fast.h"
#include "xtt_xcrr.h"
#include "xtt_menu.h"
#include "xtt_url.h"

#define IF_NOGDH_RETURN \
if ( !xnav->gbl.gdh_started)\
{\
  xnav->message('E',"Xtt is not connected to nethandler");\
  return XNAV__SUCCESS;\
}

#define	XNAV_MENU_CREATE	0
#define	XNAV_MENU_ADD		1

static char null_str[] = "";
static char xtt_version[] = "V3.3a";
static XNav *current_xnav;

static void xnav_ev_help_cb( void *xnav, char *key);
static void xnav_ev_display_in_xnav_cb( void *xnav, pwr_tObjid objid);
static void xnav_ev_update_info_cb( void *xnav);
static void xnav_ge_display_in_xnav_cb( void *xnav, pwr_tObjid objid);
static int xnav_ge_is_authorized_cb( void *xnav, unsigned int access);
static int xnav_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string);
static int xnav_ge_command_cb( ge_tCtx gectx, char *command);
static void xnav_ge_close_cb( ge_tCtx gectx);
//new code by Jonas Nylund 030131
static void xnav_hist_close_cb( void *ctx);
//end new code by Jonas Nylund 030131
static void xnav_op_command_cb( void *xnav, char *command);
static void xnav_op_close_cb( void *ctx);
static void xnav_op_help_cb( void *ctx, char *key);
static void xnav_op_map_cb( void *ctx);
static int xnav_op_get_alarm_info_cb( void *xnav, evlist_sAlarmInfo *info);
static void xnav_op_ack_last_cb( void *xnav, unsigned long type, unsigned long prio);
static void xnav_trend_close_cb( void *ctx, XttTrend *trend);

static int	xnav_help_func(		void		*client_data,
					void		*client_flag);
static int	xnav_define_func(	void		*client_data,
					void		*client_flag);
static int	xnav_logout_func(	void		*client_data,
					void		*client_flag);
static int	xnav_login_func(	void		*client_data,
					void		*client_flag);
static int	xnav_show_func(	void		*client_data,
				void		*client_flag);
static int	xnav_open_func(	void		*client_data,
				void		*client_flag);
static int	xnav_close_func(	void		*client_data,
				        void		*client_flag);
static int	xnav_create_func( void		*client_data,
				void		*client_flag);
static int	xnav_delete_func( void		*client_data,
				void		*client_flag);
static int	xnav_add_func(	void		*client_data,
				void		*client_flag);
static int	xnav_collect_func(	void		*client_data,
					void		*client_flag);
static int	xnav_store_func(	void		*client_data,
					void		*client_flag);
static int	xnav_exit_func(	void		*client_data,
				void		*client_flag);
static int	xnav_crossref_func(	void		*client_data,
					void		*client_flag);
static int	xnav_setup_func(	void		*client_data,
					void		*client_flag);
static int	xnav_search_func(	void		*client_data,
					void		*client_flag);
static int	xnav_eventlist_func(	void		*client_data,
					void		*client_flag);
static int	xnav_set_func(		void		*client_data,
					void		*client_flag);
static int	xnav_test_func(		void		*client_data,
					void		*client_flag);
static int	xnav_logging_func(	void		*client_data,
					void		*client_flag);

dcli_tCmdTable	xnav_command_table[] = {
		{
			"SHOW",
			&xnav_show_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/CLASS", 
				"/HIERARCHY", "/PARAMETER" , "/OBJID", 
				"/FILE", "/LOCAL", "/INITSTEP", 
				"/MAXOBJECTS", "/VOLUME", "/ALL", "/TYPE", 
				"/OPTION", "/ENTRY", "/NEW", ""}
		},
		{
			"OPEN",
			&xnav_open_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/FILE", 
			"/SCROLLBAR", "/WIDTH", "/HEIGHT", "/MENU", 
			"/NAVIGATOR", "/CENTER", "/OBJECT", "/NEW", 
			"/INSTANCE", "/COLLECT", "/FOCUS", "/INPUTEMPTY", 
                        "/ENTRY", "/TITLE", "/ACCESS", "/CLASSGRAPH", ""}
		},
		{
			"CLOSE",
			&xnav_close_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/OBJECT",
			"/INSTANCE", "/ALL", "/EXCEPT", ""}
		},
		{
			"CREATE",
			&xnav_create_func,
			{ "dcli_arg1", "/TEXT", "/MENU", "/DESTINATION",
			"/COMMAND", "/AFTER", "/BEFORE", "/FIRSTCHILD", 
			  "/LASTCHILD", "/CLASS", "/NAME", 
			""}
		},
		{
			"DELETE",
			&xnav_delete_func,
			{ "dcli_arg1", "/NAME", ""}
		},
		{
			"ADD",
			&xnav_add_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/CLASS", 
				"/HIERARCHY", "/PARAMETER", "/LOCAL",
				"/TEXT", "/OBJECT", "/COMMAND", ""}
		},
		{
			"STORE",
			&xnav_store_func,
			{ "dcli_arg1", "/COLLECT", "/FILE", "/SYMBOLS", ""}
		},
		{
			"EXIT",
			&xnav_exit_func,
			{ "",}
		},
		{
			"QUIT",
			&xnav_exit_func,
			{ "",}
		},
		{
			"DEFINE",
			&xnav_define_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4", ""}
		},
		{
			"HELP",
			&xnav_help_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4",
			"/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", 
			"/INDEX", "/BASE", "/RETURNCOMMAND", "/WIDTH",
			"/HEIGHT", "/VERSION", ""}
		},
		{
			"LOGOUT",
			&xnav_logout_func,
			{ ""}
		},
		{
			"LOGIN",
			&xnav_login_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{
			"COLLECT",
			&xnav_collect_func,
			{ "dcli_arg1", "/NAME", ""}
		},
		{
			"CROSSREFERENCE",
			&xnav_crossref_func,
			{ "dcli_arg1", "/NAME", "/FILE", "/STRING", "/BRIEF",
			"/FUNCTION", "/CASE_SENSITIVE", "/WINDOW", ""}
		},
		{
			"SET",
			&xnav_set_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/VALUE",
			"/BYPASS", ""}
		},
		{
			"SETUP",
			&xnav_setup_func,
			{"",}
		},
		{
			"SEARCH",
			&xnav_search_func,
			{ "dcli_arg1", "/REGULAREXPRESSION", "/NEXT", ""}
		},
		{
			"EVENTLIST",
			&xnav_eventlist_func,
			{"dcli_arg1", "/PRIORITY", "",}
		},
		{
			"TEST",
			&xnav_test_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{
			"LOGGING",
			&xnav_logging_func,
			{ "dcli_arg1", "dcli_arg2", "/FILE", "/TIME", "/ENTRY",
			"/TYPE", "/PARAMETER", "/CONDITION", "/INSERT", 
			"/BUFFER_SIZE", "/PRIORITY", "/STOP", "/NOSTOP", 
			"/CREATE", "/ALL", "/LINE_SIZE", "/SHORTNAME", 
			"/NOSHORTNAME", ""}
		},
		{"",}};


static void xnav_store_xnav( XNav *xnav)
{
  current_xnav = xnav;  
}

static void xnav_get_stored_xnav( XNav **xnav)
{
  *xnav = current_xnav;  
}

static int	xnav_help_func(		void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg_str[80];
  char	file_str[80];
  char	bookmark_str[80];
  char	key[80];
  char  return_str[80];
  int	pop;
  int   width, height;
  int   nr;	

  if ( ODD( dcli_get_qualifier( "/INDEX", file_str)))
  {
    if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str)))
    {
      sts = CoXHelp::dhelp_index( navh_eHelpFile_Other, file_str);
      if ( EVEN(sts))
        xnav->message('E', "Unable to find file");
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/BASE", file_str)))
        sts = CoXHelp::dhelp_index( navh_eHelpFile_Base, NULL);
      else
        sts = CoXHelp::dhelp_index( navh_eHelpFile_Project, NULL);
    }
    return 1;
  }

  if ( ODD( dcli_get_qualifier( "/VERSION", 0))) {
    sts = CoXHelp::dhelp( "version", "", navh_eHelpFile_Other, "$pwr_exe/xtt_version_help.dat", 0);
    if ( EVEN(sts))
      xnav->message('E', "No help on this subject");
    return sts;
  }

  int strict = 0;
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg_str)))
  {
    sts = CoXHelp::dhelp( "help command", "", navh_eHelpFile_Base, NULL, strict);
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "/BOOKMARK", bookmark_str)))
    strcpy( bookmark_str, "");

  strcpy( key, arg_str);
  if ( ODD( dcli_get_qualifier( "dcli_arg2", arg_str)))
  {
    strcat( key, " ");
    strcat( key, arg_str);
    if ( ODD( dcli_get_qualifier( "dcli_arg3", arg_str)))
    {
      strcat( key, " ");
      strcat( key, arg_str);
      if ( ODD( dcli_get_qualifier( "dcli_arg3", arg_str)))
      {
        strcat( key, " ");
        strcat( key, arg_str);
        if ( ODD( dcli_get_qualifier( "dcli_arg4", arg_str)))
        {
          strcat( key, " ");
          strcat( key, arg_str);
        }
      }
    }
  }
  if ( ! ODD( dcli_get_qualifier( "/RETURNCOMMAND", return_str)))
    strcpy( return_str, "");

  if ( ODD( dcli_get_qualifier( "/WIDTH", arg_str)))
  {
    // convert to integer
    nr = sscanf( arg_str, "%d", &width);
    if ( nr != 1)
    {
      xnav->message('E', "Width syntax error");
      return XNAV__HOLDCOMMAND;
    }
  }
  else
    width = 0;

  if ( ODD( dcli_get_qualifier( "/HEIGHT", arg_str)))
  {
    // convert to integer
    nr = sscanf( arg_str, "%d", &height);
    if ( nr != 1)
    {
      xnav->message('E', "Height syntax error");
      return XNAV__HOLDCOMMAND;
    }
  }
  else
    height = 0;


  pop =  ODD( dcli_get_qualifier( "/POPNAVIGATOR", file_str));

  if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str)))
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Other, file_str, strict);
    if ( EVEN(sts))
      xnav->message('E', "No help on this subject");
    else if ( strcmp( return_str, "") != 0)
      xnav->set_push_command( return_str);
  }
  else if ( ODD( dcli_get_qualifier( "/BASE", 0)))
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Base, 0, strict);
    if ( EVEN(sts))
      xnav->message('E', "No help on this subject");
    else if ( strcmp( return_str, "") != 0)
      xnav->set_push_command( return_str);
  }
  else
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Base, 0, strict);
    if ( EVEN(sts))
    {
      sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Project, 0, strict);
      if ( EVEN(sts))
        xnav->message('E', "No help on this subject");
    }
    //if ( ODD(sts) && strcmp( return_str, "") != 0)
    //  xnav->xhelp->set_push_command( return_str);
  }

  return 1;
}

static int	xnav_define_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	arg3_str[80];
  char	arg4_str[80];
  char 	*arg3_ptr;
  char 	*arg4_ptr;
	
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    xnav->message('E',"Syntax error");
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
  {
    xnav->message('E',"Syntax error");
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg3", arg3_str)))
    arg3_ptr = 0;
  else
    arg3_ptr = arg3_str;
  if ( EVEN( dcli_get_qualifier( "dcli_arg4", arg4_str)))
    arg4_ptr = 0;
  else
    arg4_ptr = arg4_str;
  sts = dcli_define_symbol( arg1_str, arg2_str, arg3_ptr, arg4_ptr);
  if (ODD(sts))
    xnav->message('I',"Symbol defined");
  return sts;
}

static int	xnav_login_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	systemgroup[80];
  unsigned int	priv;
  char	msg[80];
	
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    xnav->message('E',"Syntax error");
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
  {
    xnav->message('E',"Syntax error");
    return 1;
  }
  sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", &systemgroup, 
		sizeof(systemgroup));
  if ( EVEN(sts)) return sts;

  cdh_ToLower( arg1_str, arg1_str);
  cdh_ToLower( arg2_str, arg2_str);
  sts = user_CheckUser( systemgroup, arg1_str, arg2_str, &priv);
  if ( EVEN(sts))
    xnav->message('E',"Login failure");
  else
  {
    strcpy( xnav->user, arg1_str);
    xnav->priv = priv;
    sprintf( msg, "User %s logged in", arg1_str);
    xnav->message('I', msg);
  }
  return sts;
}

static int	xnav_logout_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  char	msg[80];
	
  if ( strcmp( xnav->base_user, "") == 0)
  {
    sprintf( msg, "User %s logged out", xnav->user);
    xnav->message('I', msg);
  }
  else
  {
    sprintf( msg, "Returned to user %s", xnav->base_user);
    xnav->message('I', msg);
  }
  strcpy( xnav->user, xnav->base_user);
  xnav->priv = xnav->base_priv;
  return 1;
}

static int	xnav_set_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "JOP_QUEID", strlen( arg1_str)) == 0)
  {
    // Command is "SET JOP_QUEID"
    char	arg2_str[80];
    int		nr;
    int		qid;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      xnav->message('E',"Syntax error");
      return 1;
    }

    // Convert to qid
    nr = sscanf( arg2_str, "%d", &qid);
    if ( nr != 1)
    {
      xnav->message('E', "Syntax error");
      return XNAV__HOLDCOMMAND; 	
    }
    xnav->op->set_jop_qid( qid);
  }
  else if ( strncmp( arg1_str, "ADVANCEDUSER", strlen( arg1_str)) == 0)
  {
    xnav->gbl.advanced_user = 1;
    xnav->message('I', "Advanced user");
  }
  else if ( strncmp( arg1_str, "NOADVANCEDUSER", strlen( arg1_str)) == 0)
  {
    xnav->gbl.advanced_user = 0;
  }
  else if ( strncmp( arg1_str, "PARAMETER", strlen( arg1_str)) == 0)
  {
    // Command is "SET PARAMETER"
    char	name_str[80];
    char	value_str[80];
    int	sts;
    int	bypass;

    bypass = ODD( dcli_get_qualifier( "/BYPASS", name_str));

    if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
    {
      xnav->message('E', "Enter name of parameter");
      return XNAV__HOLDCOMMAND;
    }
    if ( EVEN( dcli_get_qualifier( "/VALUE", value_str)))
    {
      xnav->message('E', "Enter value");
      return XNAV__HOLDCOMMAND;
    }
    sts = xnav->set_parameter( name_str, value_str, bypass);
    if ( EVEN(sts))
    {
      xnav->message('E',"Unable to set parameter");	
      return XNAV__HOLDCOMMAND;
    }
    else
      return sts;
  }
  else
    xnav->message('E',"Syntax error");
  return 1;
}

static int	xnav_show_func(	void		*client_data,
				void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "VERSION", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW VERSION"
    char	message_str[80];
	 
    strcpy( message_str, "Xtt version ");
    strcat( message_str, xtt_version);
    xnav->message('I', message_str);
    return 1;
  }
  else if ( strncmp( arg1_str, "SYMBOL", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW SYMBOL" */
    char	arg2_str[80];
    char	message_str[80];
    char  	value[80];
	 
    if ( ODD( dcli_get_qualifier( "/ALL", value)))
    {
      sts = xnav->show_symbols();
      return sts;
    }
    else
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
      {
        sts = xnav->show_symbols();
        return sts;
      }
      sts = dcli_get_symbol( arg2_str, value);
      if (EVEN(sts))
      {
        xnav->message('E', "Symbol not found");
        return 1;
      }
      sprintf( message_str, "Symbol %s = %s", arg2_str, value);
      xnav->message('I', message_str);
      return 1;
   }
  }
  else if ( strncmp( arg1_str, "PLCPGM", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW PLCPGM" */
    xnav->show_plcpgm();
    return 1;
  }
  else if ( strncmp( arg1_str, "PLCTHREADS", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW PLCTHREAD" */
    xnav->show_plcthreads();
    return 1;
  }
  else if ( strncmp( arg1_str, "NODES", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW NODES" */
    xnav->show_nethandler();
    return 1;
  }
  else if ( strncmp( arg1_str, "LINKS", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW LINKS" */
    xnav->show_nethandler();
    return 1;
  }
  else if ( strncmp( arg1_str, "SUBSRV", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW SUBSRV" */
    xnav->show_subsrv();
    return 1;
  }
  else if ( strncmp( arg1_str, "SUBCLI", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW SUBCLI" */
    xnav->show_subcli();
    return 1;
  }
  else if ( strncmp( arg1_str, "DEVICE", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW DEVICE" */
    xnav->show_device();
    return 1;
  }
  else if ( strncmp( arg1_str, "REMNODE", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW REMNODE" */
    xnav->show_remnode();
    return 1;
  }
  else if ( strncmp( arg1_str, "REMTRANS", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW REMTRANS" */
    xnav->show_remtrans( pwr_cNObjid);
    return 1;
  }
  else if ( strncmp( arg1_str, "DATABASE", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW DATABASE" */
    xnav->show_database();
    return 1;
  }
  else if ( strncmp( arg1_str, "LOGGING", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW LOGGING" */
    char entry_str[80];
    int	entry;
    int	nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      // entry 1 is default
      entry = 1;
    }
    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    xnav->show_logging( entry - 1);
  }
  else if ( strncmp( arg1_str, "FILE", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW FILE" */
    char	arg2_str[80];
    char	title[80];
    char	filename[80];
    char	option_str[80];
    int		hide_dir = 0;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      hide_dir = 1;
      dcli_get_defaultfilename( "*", filename, ".rtt_com");
      strcpy( title, "Stored pictures and scripts");
    }
    else
    {
      dcli_get_defaultfilename( arg2_str, filename, ".rtt_com");
      strcpy( title, "File list");
    }
    if ( ODD( dcli_get_qualifier( "/OPTION", option_str)))
    {
      if ( strstr( option_str, "HIDE_DIR"))
        hide_dir = 1;
    }
    sts = xnav->show_file( filename, title, hide_dir);
    if ( EVEN(sts))
    {
      xnav->message('E', "No files found");
      return XNAV__SUCCESS;
    }
    return sts;
  }
  else if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW GRAPH" */
    char	arg2_str[80];
    char	title[80];
    char	filename[80];
    char	option_str[80];
    int		hide_dir = 0;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      hide_dir = 1;
      dcli_get_defaultfilename( "pwrp_exe:*", filename, ".pwg");
      strcpy( title, "Graphs");
    }
    else
    {
      dcli_get_defaultfilename( arg2_str, filename, ".pwg");
      strcpy( title, "Graph list");
    }
    if ( ODD( dcli_get_qualifier( "/OPTION", option_str)))
    {
      if ( strstr( option_str, "HIDE_DIR"))
        hide_dir = 1;
    }
    sts = xnav->show_file( filename, title, hide_dir);
    if ( EVEN(sts))
    {
      xnav->message('E', "No files found");
      return XNAV__SUCCESS;
    }
    return sts;
  }
  else if ( strncmp( arg1_str, "LOGFILES", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW LOGFILES" */
    char	arg2_str[80];
    char	title[80];
    char	filename[80];
    char	option_str[80];
    int		hide_dir = 0;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      hide_dir = 1;
      dcli_get_defaultfilename( "*", filename, ".rtt_log");
      strcpy( title, "RttLogFiles");
    }
    else
    {
      dcli_get_defaultfilename( arg2_str, filename, ".rtt_log");
      strcpy( title, "RttLogFiles");
    }
    if ( ODD( dcli_get_qualifier( "/OPTION", option_str)))
    {
      if ( strstr( option_str, "HIDE_DIR"))
        hide_dir = 1;
    }
    sts = xnav->show_file( filename, title, hide_dir);
    if ( EVEN(sts))
    {
      xnav->message('E', "No files found");
      return XNAV__SUCCESS;
    }
    return sts;
  }
  else if ( strncmp( arg1_str, "TIME", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW TIME" */
    char	message_str[80];
   
    xnav->update_time();
    sprintf( message_str, "Time is %s", xnav->gbl.time);
    xnav->message('I', message_str);
    return XNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "DEFAULT", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW DEFAULT" */
    char	message_str[80];
	 
    sprintf( message_str, "Default directory: %s", xnav->gbl.default_directory);
    xnav->message('I', message_str);
    return XNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "MENU", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW MENU" */
    char	name_str[80];

    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] == '/')
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
      {
        xnav->message('E', "Syntax error, name not found");
        return XNAV__HOLDCOMMAND;
      }
    }
//    sts = rtt_show_menu( ctx, name_str);
    sts = 0;
    return sts;
  }
  else if ( strncmp( arg1_str, "HIERARCHY", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW HIERARCHY" */
    IF_NOGDH_RETURN;
    sts = dcli_cli( (dcli_tCmdTable *)&xnav_command_table, 
		"SHOW DATABASE", (void *) xnav, 0);
    return sts;
  }
  else if ( strncmp( arg1_str, "CHILDREN", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW CHILDREN"
    char        name_str[80];
    pwr_tObjid	objid;
    int         newlist;

    IF_NOGDH_RETURN;
    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
      sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
			name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Select an object or enter name");
        return XNAV__SUCCESS;
      }
    }
    newlist =  ODD( dcli_get_qualifier( "/NEW", NULL));

    try {
      if ( !newlist)
	xnav->display_object( objid, 1);
      else {
	sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str),
				cdh_mNName);
	xnav->brow_pop();
	ItemObject *item = new ItemObject( xnav->brow, objid, NULL,
						flow_eDest_IntoLast, 1);
	new ItemHeader( xnav->brow, "Title", name_str, NULL, flow_eDest_IntoLast);
	item->open_children( xnav->brow, 0, 0);
	delete item;
      }
    }
    catch ( co_error& e) {
      xnav->brow_push_all();
      brow_Redraw( xnav->brow->ctx, 0);
      xnav->message('E', (char *)e.what().c_str());
    }
    
    return XNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW OBJECT" */
    char	class_str[80];
    char	name_str[80];
    char	hierarchy_str[80];
    char	objdid_str[80];
    char	maxobjects_str[80];
    char	type_str[80];
    char	file_str[80];
    int	maxobjects;
    char	*class_ptr;
    char	*name_ptr;
    char	*hierarchy_ptr;
    int	global;
    char	str[80];
    int	nr;

    IF_NOGDH_RETURN;
    if ( ODD( dcli_get_qualifier( "/OBJID", objdid_str)))
    {
      pwr_tObjid		objid;
      pwr_tClassId		classid;

      /* Convert to objid */
      sts = cdh_StringToObjid( objdid_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Syntax error in objid");
        return XNAV__HOLDCOMMAND;
      }
      sts = gdh_GetObjectClass (objid, &classid);
      if ( EVEN(sts)) 
      {
        xnav->message('E', "Objid not found");
        return XNAV__HOLDCOMMAND;
      }
      /* Get the object name */
      sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str), 
			cdh_mName_volumeStrict);

      if ( EVEN(sts)) return sts;

      name_ptr = name_str;
//      sts = rtt_show_obj_hier_class_name( ctx, 0, 0,
//  		name_ptr, 0, 0);
      sts = 0;
      return sts;
    }
    else if ( ODD( dcli_get_qualifier( "/TYPE", type_str)))
    {
      pwr_tObjid		objid;
      char                      *msg;

      if ( EVEN( dcli_get_qualifier( "/FILE", file_str)))
      {
        xnav->message('E', "Enter filename");
        return XNAV__HOLDCOMMAND;
      }
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
      {
        sts = gdh_NameToObjid ( name_str, &objid);
        if ( EVEN(sts))
        {
          xnav->message('E', "Object not found");
          return XNAV__SUCCESS;
        }
        sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str),
  		cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;
      }
      else
      {
        /* Get the selected object */
        sts = xnav->get_current_object( &objid, 
  		name_str, sizeof( name_str), cdh_mName_volumeStrict);
        if ( EVEN(sts))
        {
          xnav->message('E', "Select an object or enter name");
          return XNAV__SUCCESS;
        }
      }
      sts = xnav->show_object_as_struct( objid, type_str, file_str);
      if ( EVEN(sts) && dcli_readstruct_get_message( &msg))
        xnav->message( 'E', msg);

      return sts;
    }

    /* Get maxobjects qualifier */
    if ( ODD( dcli_get_qualifier( "/MAXOBJECTS", maxobjects_str)))
    {
      /* Convert to objid */
      nr = sscanf( maxobjects_str, "%d", &maxobjects);
      if ( nr != 1)
      {
        xnav->message('E', "Syntax error in maxobjects");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      /* Default value */
      maxobjects = 0;

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
        name_ptr = NULL;
    }

    if ( ODD( dcli_get_qualifier( "/CLASS", class_str)))
      class_ptr = class_str;
    else
      class_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierarchy_str)))
      hierarchy_ptr = hierarchy_str;
    else
      hierarchy_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/LOCAL", str)))
      global = 0;
    else
      global = 1;

//    sts = rtt_show_obj_hier_class_name( ctx, hierarchy_ptr, class_ptr,
//			name_ptr, global, maxobjects);
    sts = 0;
    return sts;
  }

  else if ( strncmp( arg1_str, "OBJID", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW OBJID" */
    char		name_str[80];
    pwr_tObjid	objid;
    char		msg[160];

    IF_NOGDH_RETURN;
    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
      sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
			name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Select an object or enter name");
        return XNAV__SUCCESS;
      }
    }

    sprintf( msg, "Objid %s, Name %s", cdh_ObjidToString( NULL, objid, 0),
			name_str);
    xnav->message('I', msg);
    return XNAV__SUCCESS;
  }

  else if ( strncmp( arg1_str, "SIGNALS", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW SIGNALS" */
    char		file_str[80];
    char		name_str[80];
    char		*file_ptr;
    char		*name_ptr;
    pwr_tObjid	objid;
    pwr_tObjid	parentobjid;
    pwr_tClassId	classid;
    char		classname[80];

    IF_NOGDH_RETURN;
    if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
      file_ptr = file_str;
    else
      file_ptr = NULL;

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
      name_ptr = name_str;
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, name_str, 
			sizeof( name_str), cdh_mName_path | cdh_mName_object);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__HOLDCOMMAND;
      }
      name_ptr = name_str;

      sts = gdh_GetObjectClass ( objid, &classid);
      if ( EVEN(sts)) return sts;
      sts = gdh_ObjidToName ( cdh_ClassIdToObjid(classid), classname, 
  		sizeof(classname), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      /* Check that this is a or a plc */	
      if ( strcmp( classname, "pwrb:Class-PlcPgm") == 0)
      {
        /* Get all the windows in the plc */
        strcat( name_str, "-W*");
      }
      else if (!( (strcmp( classname, "pwrb:Class-WindowPlc") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowOrderact") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowCond") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowSubstep") == 0)))
      {
        /* Try with the parent */
        sts = gdh_GetParent( objid, &parentobjid);
        if ( EVEN(sts)) return sts;
        sts = gdh_GetObjectClass ( parentobjid, &classid);
        if ( EVEN(sts)) return sts;
        sts = gdh_ObjidToName ( cdh_ClassIdToObjid(classid), classname, 
			sizeof(classname), cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;

        if (!( (strcmp( classname, "pwrb:Class-WindowPlc") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowOrderact") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowCond") == 0) ||
	       (strcmp( classname, "pwrb:Class-WindowSubstep") == 0)))
        {
          xnav->message('E', "Selected object has to be in a plcpgm or a plcpgm");
          return XNAV__HOLDCOMMAND;
        }
        else
        {
          sts = gdh_ObjidToName ( parentobjid, name_str, sizeof(name_str), 
			cdh_mName_volumeStrict);
          if ( EVEN(sts)) return sts;
        }
      }
    }
//    sts = rtt_show_signals( ctx, file_ptr, name_ptr, 0);
    sts = 0;
    return sts;
  }

  else if ( strncmp( arg1_str, "PARAMETER", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW PARAMETER" */
    char	parameter_str[80];
    char	class_str[80];
    char	name_str[80];
    char	hierarchy_str[80];
    char	*class_ptr;
    char	*name_ptr;
    char	*hierarchy_ptr;
    char	*parameter_ptr;
    int	global;
    char		str[80];
    int	nr;
    char	maxobjects_str[80];
    int	maxobjects;

    IF_NOGDH_RETURN;
    /* Get maxobjects qualifier */
    if ( ODD( dcli_get_qualifier( "/MAXOBJECTS", maxobjects_str)))
    {
      /* Convert to objid */
      nr = sscanf( maxobjects_str, "%d", &maxobjects);
      if ( nr != 1)
      {
        xnav->message('E', "Syntax error in maxobjects");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      /* Default value */
      maxobjects = 0;

    if ( ODD( dcli_get_qualifier( "/PARAMETER", parameter_str)))
      parameter_ptr = parameter_str;
    else
      parameter_ptr = NULL;

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
        name_ptr = NULL;
    }
    if ( ODD( dcli_get_qualifier( "/CLASS", class_str)))
      class_ptr = class_str;
    else
      class_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierarchy_str)))
      hierarchy_ptr = hierarchy_str;
    else
      hierarchy_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/LOCAL", str)))
      global = 0;
    else
      global = 1;

    sts = xnav->show_par_hier_class_name( parameter_ptr,  
		hierarchy_ptr, class_ptr, name_ptr, XNAV_MENU_CREATE, global,
		maxobjects); 
    sts = 0;
    return sts; 
  } 
  else if ( strncmp( arg1_str, "CONVERSION", strlen( arg1_str)) == 0) 
  { 
#if 0
    int	on = 0;
    pwr_tObjid	objid; 
    char		name_str[80];

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
  		name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__HOLDCOMMAND;
      }
    }
//    sts = rtt_set_conversion( objid, on, 1);
    sts = 0;
    return sts;
#endif
  }
  else if ( strncmp( arg1_str, "INVERT", strlen( arg1_str)) == 0)
  {
#if 0
    int	on = 0;
    pwr_tObjid	objid;
    char		name_str[80];

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
			name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__HOLDCOMMAND;
      }
    }
//    sts = rtt_set_invert( objid, on, 1);
    sts = 0;
    return sts;
#endif
  }
  else if ( strncmp( arg1_str, "DOTEST", strlen( arg1_str)) == 0)
  {
#if 0
    int	on = 0;
    pwr_tObjid	objid;
    char		name_str[80];

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
			name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__HOLDCOMMAND;
      }
    }
//    sts = rtt_set_do_test( objid, on, 1);
    sts = 0;
    return sts;
#endif
  }
  else if ( strncmp( arg1_str, "TESTVALUE", strlen( arg1_str)) == 0)
  {
#if 0
    int	on = 0;
    pwr_tObjid	objid;
    char		name_str[80];

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if ( EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__SUCCESS;
      }
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, 
			name_str, sizeof( name_str), cdh_mName_volumeStrict);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__HOLDCOMMAND;
      }
    }
//    sts = rtt_set_do_testvalue( objid, on, 1);
    sts = 0;
    return sts;
#endif
  }
  else if ( strncmp( arg1_str, "EVENTLIST", strlen( arg1_str)) == 0)
  {

    if ( !xnav->ev)
    {
      char alarm_title[40], event_title[40];

      strcpy( alarm_title, Lng::translate( "Alarm list"));
      strcpy( event_title, Lng::translate( "Event list"));
      xnav->ev = new Ev( xnav, xnav->parent_wid, event_title, alarm_title,
		xnav->gbl.UserObject, 0, 1, xnav->gbl.AlarmReturn,
		xnav->gbl.AlarmAck, xnav->gbl.AlarmBeep, &sts);
      if ( EVEN(sts))
      {
        delete xnav->ev;
        xnav->ev = NULL;
        xnav->message('E', "Unable to load eventlist");
        return XNAV__SUCCESS;
      }
      xnav->ev->start_trace_cb = xnav_start_trace_cb;
      xnav->ev->help_cb = xnav_ev_help_cb;
      xnav->ev->display_in_xnav_cb = xnav_ev_display_in_xnav_cb;
      xnav->ev->update_info_cb = xnav_ev_update_info_cb;
      xnav->ev->popup_menu_cb = xnav_popup_menu_cb;
    }
    else
      xnav->ev->map_eve();
  }
  /*new code by Jonas Nylund 030122*/
  else if ( strncmp( arg1_str, "HISTLIST", strlen( arg1_str)) == 0)
  {
    char hist_title[40];
    char name_str[80];
    pwr_tObjid objid = pwr_cNObjid;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if (EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }
    }

    strcpy( hist_title, Lng::translate( "Hist list"));
    Hist *hist;
    hist = new Hist( xnav, xnav->parent_wid, hist_title, objid, &sts);
    if ( EVEN(sts))
    {
      delete hist;
      hist = NULL;
      xnav->message('E', "Unable to load histlist");
      return XNAV__SUCCESS;
    }
    xnav->appl.insert( applist_eType_Hist, (void *)hist, pwr_cNObjid, "", "");
    hist->close_cb = xnav_hist_close_cb;
    hist->start_trace_cb = xnav_start_trace_cb;
    hist->help_cb = xnav_ev_help_cb;
    hist->display_in_xnav_cb = xnav_ev_display_in_xnav_cb;
    hist->update_info_cb = xnav_ev_update_info_cb;
    hist->popup_menu_cb = xnav_popup_menu_cb;
  }
  /*end new code by Jonas Nylund 030122*/
  else if ( strncmp( arg1_str, "ALARMLIST", strlen( arg1_str)) == 0)
  {

    if ( !xnav->ev)
    {
      char alarm_title[40], event_title[40];

      strcpy( alarm_title, Lng::translate( "Alarm list"));
      strcpy( event_title, Lng::translate( "Event list"));
      xnav->ev = new Ev( xnav, xnav->parent_wid, event_title, alarm_title,
		xnav->gbl.UserObject, 1, 0, xnav->gbl.AlarmReturn,
		xnav->gbl.AlarmAck, xnav->gbl.AlarmBeep, &sts);
      if ( EVEN(sts))
      {
        delete xnav->ev;
        xnav->ev = NULL;
        xnav->message('E', "Unable to load eventlist");
        return XNAV__SUCCESS;
      }
      xnav->ev->start_trace_cb = xnav_start_trace_cb;
      xnav->ev->help_cb = xnav_ev_help_cb;
      xnav->ev->display_in_xnav_cb = xnav_ev_display_in_xnav_cb;
      xnav->ev->update_info_cb = xnav_ev_update_info_cb;
      xnav->ev->popup_menu_cb = xnav_popup_menu_cb;
    }
    else
      xnav->ev->map_ala();
  }
  else if ( strncmp( arg1_str, "USER", strlen( arg1_str)) == 0)
  {
    char msg[120];
    char priv_str[80];

    if ( strcmp( xnav->user, "") == 0)
    {
      xnav->message('I', "Not logged in");
    }
    else
    {
      user_RtPrivToString( xnav->priv, priv_str, sizeof(priv_str));
      sprintf( msg, "User %s (%s)", xnav->user, priv_str);
      xnav->message('I', msg);
    }
  }
  else
  {
    /* This might be a system picture */
    sts = 0; // rttsys_start_system_picture( ctx, arg1_str);
    if ( sts == 0)
    {
      xnav->message('E', "Unknown qualifier");
      return XNAV__HOLDCOMMAND;
    }
    return sts;
  }
  return XNAV__SUCCESS;
}

static int	xnav_eventlist_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "LOAD", strlen( arg1_str)) == 0)
  {
    // Command is "EVENTLIST LOAD"
    if ( !xnav->ev)
    {
      char alarm_title[40], event_title[40];

      strcpy( alarm_title, Lng::translate( "Alarm list"));
      strcpy( event_title, Lng::translate( "Event list"));
      xnav->ev = new Ev( xnav, xnav->parent_wid, event_title, alarm_title,
		xnav->gbl.UserObject, 0, 0, xnav->gbl.AlarmReturn,
		xnav->gbl.AlarmAck, xnav->gbl.AlarmBeep, &sts);
      if ( EVEN(sts))
      {
        delete xnav->ev;
        xnav->ev = NULL;
        xnav->message('E', "Unable to load eventlist");
        return XNAV__SUCCESS;
      }
      xnav->ev->start_trace_cb = xnav_start_trace_cb;
      xnav->ev->help_cb = xnav_ev_help_cb;
      xnav->ev->display_in_xnav_cb = xnav_ev_display_in_xnav_cb;
      xnav->ev->update_info_cb = xnav_ev_update_info_cb;
      xnav->ev->popup_menu_cb = xnav_popup_menu_cb;
    }
    else
    {
      xnav->message('I', "Eventlist is already loaded");
    }
    return XNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "UNLOAD", strlen( arg1_str)) == 0)
  {
    // Command is "EVENTLIST UNLOAD"
    if ( xnav->ev)
    {
      delete ((XNav *)xnav)->ev;
      ((XNav *)xnav)->ev = NULL;
    }
    else
      xnav->message('I', "Eventlist is not loaded");
  }
  else if ( strncmp( arg1_str, "ACKNOWLEDGE", strlen( arg1_str)) == 0)
  {
    // Command is "EVENTLIST ACKNOWLEDGE"
    char prio_str[80];
    mh_sEventId *id;

    if ( xnav->ev)
    {
      if ( ODD( dcli_get_qualifier( "/PRIORITY", prio_str)))
      {
        cdh_ToUpper( prio_str, prio_str);
        if ( strcmp( prio_str, "A") == 0)
          xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_A);
        else if ( strcmp( prio_str, "B") == 0)
          xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_B);
        else if ( strcmp( prio_str, "C") == 0)
          xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_C);
        else if ( strcmp( prio_str, "D") == 0)
          xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_D);
        else if ( strncmp( prio_str, "I", 1) == 0)
          xnav->ev->ack_last_prio( evlist_eEventType_Info, 0);
        else if ( strcmp( prio_str, "NOA") == 0)
        {
          if ( ODD( xnav->ev->get_last_not_acked_prio( &id, 
			evlist_eEventType_Alarm, mh_eEventPrio_B)))
            xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_B);
          else if ( ODD( xnav->ev->get_last_not_acked_prio( &id, 
			evlist_eEventType_Alarm, mh_eEventPrio_C)))
            xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_C);
          else if ( ODD( xnav->ev->get_last_not_acked_prio( &id, 
			evlist_eEventType_Alarm, mh_eEventPrio_D)))
            xnav->ev->ack_last_prio( evlist_eEventType_Alarm, mh_eEventPrio_D);
          else if ( ODD( xnav->ev->get_last_not_acked_prio( &id,
			evlist_eEventType_Info, 0)))
            xnav->ev->ack_last_prio( evlist_eEventType_Info, 0);
        }
        else
          xnav->message('E', "Unknown priority");
     }
      else
        xnav->message('E', "Enter priority");
    }
    else
      xnav->message('I', "Eventlist is not loaded");
  }
  else
  {
    xnav->message('E', "Unknown qualifier");
    return XNAV__HOLDCOMMAND;
  }
  return XNAV__SUCCESS;
}

static int	xnav_add_func(	void		*client_data,
				void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "PARAMETER", strlen( arg1_str)) == 0)
  {
    /* Command is "ADD PARAMETER" */
    char	parameter_str[80];
    char	class_str[80];
    char	name_str[80];
    char	hierarchy_str[80];
    char	*class_ptr;
    char	*name_ptr;
    char	*hierarchy_ptr;
    char	*parameter_ptr;
    int	global;
    char	str[80];

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/PARAMETER", parameter_str)))
      parameter_ptr = parameter_str;
    else
      parameter_ptr = NULL;

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
        name_ptr = NULL;
    }
    if ( ODD( dcli_get_qualifier( "/CLASS", class_str)))
      class_ptr = class_str;
    else
      class_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierarchy_str)))
      hierarchy_ptr = hierarchy_str;
    else
      hierarchy_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/LOCAL", str)))
      global = 0;
    else
      global = 1;

    sts = xnav->show_par_hier_class_name( parameter_ptr, 
			hierarchy_ptr, class_ptr, name_ptr, XNAV_MENU_ADD,
			global, 0);
    return sts;
  }

  if ( strncmp( arg1_str, "DEBUG", strlen( arg1_str)) == 0)
  {
    /* Command is "ADD DEBUG" */
    char	class_str[80];
    char	name_str[80];
    char	hierarchy_str[80];
    char	*class_ptr;
    char	*name_ptr;
    char	*hierarchy_ptr;
    int	global;
    char	str[80];

    IF_NOGDH_RETURN;

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
        name_ptr = NULL;
    }
    if ( ODD( dcli_get_qualifier( "/CLASS", class_str)))
      class_ptr = class_str;
    else
      class_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierarchy_str)))
      hierarchy_ptr = hierarchy_str;
    else
      hierarchy_ptr = NULL;
    if ( ODD( dcli_get_qualifier( "/LOCAL", str)))
      global = 0;
    else
      global = 1;

//    sts = rtt_debug_obj_hier_class_name( ctx,  
//			hierarchy_ptr, class_ptr, name_ptr, RTT_MENU_ADD,
//			global);
    sts = 0;
    return sts;
  }
  else if ( strncmp( arg1_str, "MENU", strlen( arg1_str)) == 0)
  {
    /* Command is "ADD MENU" */
    int		sts;
    char	object_str[80];
    char	text_str[80];
    char	command_str[80];
    int		command;
    int		object;
    pwr_tObjid	objid;

    if ( EVEN( dcli_get_qualifier( "/TEXT", text_str)))
    {
      xnav->message('E',"Enter text");	
      return XNAV__HOLDCOMMAND;
    }
    if ( ODD( dcli_get_qualifier( "/COMMAND", command_str)))
      command = 1;
    else
      command = 0;
    if ( ODD( dcli_get_qualifier( "/OBJECT", object_str)))
    {
      IF_NOGDH_RETURN;
      object = 1;
      sts = gdh_NameToObjid ( object_str, &objid);
      if (EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      object = 0;

    try {
      if ( object)
	new ItemObject( xnav->brow, objid, NULL,  flow_eDest_IntoLast, 0);
      else if ( command)
	new ItemCommand( xnav->brow, text_str, NULL,
			 flow_eDest_IntoLast, command_str, 0, xnav->brow->pixmap_map);
//    else
//      new ItemMenu( xnav->brow, text_str, NULL, 
//		flow_eDest_IntoLast, (xnav_sMenu *) menu->action,
//		0);
    }
    catch ( co_error& e) {
      xnav->brow_push_all();
      brow_Redraw( xnav->brow->ctx, 0);
      xnav->message('E', (char *)e.what().c_str());
    }
    return XNAV__SUCCESS;	
  }

  return XNAV__SUCCESS;	
}

static int	xnav_open_func(	void		*client_data,
				void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
    char	object_str[80];
    char	instance_str[120];
    char	*instance_p;
    int		classgraph;
    pwr_tStatus sts;

    if ( ODD( dcli_get_qualifier( "/INSTANCE", instance_str))) {
      instance_p = instance_str;

      if ( instance_str[0] == '&') {
	pwr_tOid oid;
	pwr_tStatus sts;

	// Objid attribute the contains the instance
        sts = gdh_GetObjectInfo( &instance_str[1], &oid, sizeof(oid));
	if ( ODD(sts))
	  sts = gdh_ObjidToName( oid, instance_str, sizeof(instance_str),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) {
	  xnav->message('E', "Instance object not found");
	  return XNAV__HOLDCOMMAND;
	}
      }
    }
    else
      instance_p = 0;

    if (  ODD( dcli_get_qualifier( "/CLASSGRAPH", 0))) {
      classgraph = 1;
      if ( !instance_p) {
	xnav->message('E', "Instance is missing");
	return XNAV__HOLDCOMMAND;
      }
    }
    else
      classgraph = 0;
    
    if ( ODD( dcli_get_qualifier( "/OBJECT", object_str)))
    {
      pwr_tObjid objid;
      pwr_tObjid node_objid;
      char	xttgraph_name[120];
      int 	sts;
      char focus_str[80];
      char *focus_p;
      int  inputempty;

      IF_NOGDH_RETURN;
      if ( strncmp( object_str, "*-", 2) == 0 || strncmp( object_str, "$node-", 6) == 0)
      {
        // Replace * by the node object
        sts = gdh_GetNodeObject( 0, &node_objid);
        if ( EVEN(sts)) return sts;
        sts = gdh_ObjidToName( node_objid, xttgraph_name, sizeof(xttgraph_name), 
			cdh_mNName);
        if ( EVEN(sts)) return sts;
	if ( object_str[0] == '*')
	  strcat( xttgraph_name, &object_str[1]);
	else
	  strcat( xttgraph_name, &object_str[5]);	  
      }
      else
        strcpy( xttgraph_name, object_str);
        
      sts = gdh_NameToObjid( xttgraph_name, &objid);
      if (EVEN(sts)) {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }

      inputempty = ODD( dcli_get_qualifier( "/INPUTEMPTY", NULL));

      if ( ODD( dcli_get_qualifier( "/FOCUS", focus_str)))
        focus_p = focus_str;
      else
        focus_p = 0;

      xnav->exec_xttgraph( objid, instance_p, focus_p, inputempty);
    }
    else
    {
      char file_str[80];
      char name_str[80];
      char focus_str[80];
      char tmp_str[80];
      char *focus_p;
      int  width, height, nr, scrollbar, menu, navigator;
      int  inputempty;
      char fname[80];
      int  use_default_access;
      unsigned int access;

      // Command is "OPEN GRAPH"
      if ( ODD( dcli_get_qualifier( "/COLLECT", NULL)))
      {
        scrollbar =  ODD( dcli_get_qualifier( "/SCROLLBAR", NULL));
        menu =  ODD( dcli_get_qualifier( "/MENU", NULL));
        navigator =  ODD( dcli_get_qualifier( "/NAVIGATOR", NULL));

        xnav->open_graph( "Collect", "_none_", scrollbar, menu, navigator,
	  0, 0, 0, 0, "collect", NULL, 0, 0, 0);
        return XNAV__SUCCESS;
      }
      if ( ODD( dcli_get_qualifier( "dcli_arg2", file_str)))
      {
        if ( file_str[0] == '/')
        {
          xnav->message('E', "Syntax error");
          return XNAV__HOLDCOMMAND;
        }
      }
      else if ( EVEN( dcli_get_qualifier( "/FILE", file_str)))
      {
	if ( classgraph) {
	  // Get file from class of instance object
	  pwr_tOid oid;
	  char cname[80];
	  pwr_tCid cid;
	  
	  if ( !instance_p) {
	    xnav->message('E',"Enter instance object");
	    return XNAV__HOLDCOMMAND;
	  }
	  sts = gdh_NameToObjid( instance_p, &oid);
	  if ( EVEN(sts)) {
	    xnav->message('E',"Instance object not found");
	    return XNAV__HOLDCOMMAND;
	  }
	  sts = gdh_GetObjectClass( oid, &cid);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_ObjidToName( cdh_ClassIdToObjid( cid), cname, sizeof(cname),
			   cdh_mName_object);
	  if ( EVEN(sts)) return sts;

	  cdh_ToLower( cname, cname);
	  if ( cdh_CidToVid(cid) < cdh_cUserClassVolMin) {
	    if ( cname[0] == '$')
	      sprintf( file_str, "pwr_c_%s", &cname[1]);
	    else
	      sprintf( file_str, "pwr_c_%s", cname);
	  }
	  else
	    strcpy( file_str, cname);
	}
	else {
	  xnav->message('E',"Enter file");
	  return XNAV__HOLDCOMMAND;
	}
      }
      if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
      {
	if ( instance_p) {
	  pwr_tOid oid;

	  // Use object name as title
	  sts = gdh_NameToObjid( instance_p, &oid);
	  if ( EVEN(sts)) {
	    xnav->message('E',"Instance object not found");
	    return XNAV__HOLDCOMMAND;
	  }
	  sts = gdh_ObjidToName( oid, name_str, sizeof(name_str),
			   cdh_mName_pathStrict);
	  if ( EVEN(sts)) return sts;
	}
	else
	  strcpy( name_str, file_str);
      }
      if ( ODD( dcli_get_qualifier( "/FOCUS", focus_str)))
        focus_p = focus_str;
      else
        focus_p = 0;

      scrollbar =  ODD( dcli_get_qualifier( "/SCROLLBAR", NULL));
      menu =  ODD( dcli_get_qualifier( "/MENU", NULL));
      navigator =  ODD( dcli_get_qualifier( "/NAVIGATOR", NULL));
      inputempty = ODD( dcli_get_qualifier( "/INPUTEMPTY", NULL));

      if ( ODD( dcli_get_qualifier( "/WIDTH", tmp_str)))
      {
        nr = sscanf( tmp_str, "%d", &width);
        if ( nr != 1)
        {
          xnav->message('E', "Syntax error in width");
          return XNAV__HOLDCOMMAND;
        }
      }
      else
        width = 0;

      if ( ODD( dcli_get_qualifier( "/HEIGHT", tmp_str)))
      {
        nr = sscanf( tmp_str, "%d", &height);
        if ( nr != 1)
        {
          xnav->message('E', "Syntax error in height");
          return XNAV__HOLDCOMMAND;
        }
      }
      else
        height = 0;

      if ( ODD( dcli_get_qualifier( "/ACCESS", tmp_str)))
      {
        nr = sscanf( tmp_str, "%u", &access);
        if ( nr != 1)
        {
          xnav->message('E', "Syntax error in access");
          return XNAV__HOLDCOMMAND;
        }
        use_default_access = 1;
      }
      else
        use_default_access = 0;

      // Get base class graphs on $pwr_exe
      cdh_ToUpper( fname, file_str);
      if ( instance_p && 
           (strncmp( fname, "PWR_C_", 6) == 0 || 
            strncmp( fname, "PWR_T_", 6) == 0)) {
	strcpy( fname, "$pwr_exe/");
        strcat( fname, file_str);
        strcpy( file_str, fname);
      }  

      xnav->open_graph( name_str, file_str, scrollbar, menu, navigator,
	width, height, 0, 0, instance_p, focus_p, inputempty,
	use_default_access, access);
      return XNAV__SUCCESS;	
    }
  }
  else if ( strncmp( arg1_str, "TRACE", strlen( arg1_str)) == 0)
  {
    char name_str[80];
    char center_str[80];
    char *center_p;
    pwr_sAttrRef attrref;
    int 	is_attr;
    pwr_tObjid	objid;
    int		sts;

    // Command is "OPEN TRACE"
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      sts = gdh_NameToObjid ( name_str, &objid);
      if (EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
      {
        sts = gdh_NameToObjid ( name_str, &objid);
        if (EVEN(sts))
        {
          xnav->message('E', "Object not found");
          return XNAV__HOLDCOMMAND;
        }
      }
      else
      {
        sts = xnav->get_select( &attrref, &is_attr);
        if ( EVEN(sts))
        {
          xnav->message('E', "Enter name or select object");
          return XNAV__SUCCESS;
        }
        objid = attrref.Objid;
      }
    }

    if ( ODD( dcli_get_qualifier( "/CENTER", center_str)))
      center_p  = center_str;
    else
      center_p = NULL;

    xnav->start_trace( objid, center_p);
    return XNAV__SUCCESS;	
  }
  else if ( strncmp( arg1_str, "OPERATORWINDOW", strlen( arg1_str)) == 0)
  {
    int sts;
    char opplace_str[80];

    if ( !xnav->op)
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", opplace_str)))
      {
        xnav->message('E', "Enter opplace");
        return XNAV__HOLDCOMMAND;
      }

      xnav->op = new Op( xnav, xnav->parent_wid, 
		opplace_str, &sts);
      if ( EVEN(sts))
      {
        delete xnav->op;
        xnav->op = NULL;
        xnav->message('E', "Unable to open operator window");
        return XNAV__SUCCESS;
      }
      xnav->op->command_cb = xnav_op_command_cb;
      xnav->op->close_cb = xnav_op_close_cb;
      xnav->op->help_cb = xnav_op_help_cb;
      xnav->op->map_cb = xnav_op_map_cb;
      xnav->op->get_alarm_info_cb = xnav_op_get_alarm_info_cb;
      xnav->op->ack_last_cb = xnav_op_ack_last_cb;
    }
    else
    {
      xnav->op->map();
    }
  }
  else if ( strncmp( arg1_str, "JGRAPH", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    char	command[200];

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      xnav->message('E',"Syntax error");
      return 1;
    }

    if ( xnav->op)
    {
      strcpy( command, "open jgraph ");
      strcat( command, arg2_str);
      xnav->op->jop_command( command);
    }
    else
      xnav->message('E',"Java process not started");
  }
  else if ( strncmp( arg1_str, "LOGGFILE", strlen( arg1_str)) == 0)
  {
    char file_str[80];
    char entry_str[80];
    int entry;
    int nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      if ( !strcmp( entry_str, "CURRENT"))
      {
        if ( xnav->current_logging_index == -1)
        {
          xnav->message('E', "No current logging entry");
          return XNAV__HOLDCOMMAND;
        }
        entry = xnav->current_logging_index + 1;
      }
      else
      {
        /* convert to integer */
        nr = sscanf( entry_str, "%d", &entry);
        if ( nr != 1)
        {
          xnav->message('E', "Entry syntax error");
          return XNAV__HOLDCOMMAND;
        }
      }
      strcpy( file_str, xnav->logg[entry-1].logg_filename);
    }
    else if ( EVEN( dcli_get_qualifier( "/FILE", file_str))) {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", file_str))) {
        xnav->message('E', "Enter filename");
        return XNAV__HOLDCOMMAND;
      }
    }
    xnav->open_rttlog( file_str, file_str);
  }
  else if ( strncmp( arg1_str, "TREND", strlen( arg1_str)) == 0)
  {

    char name_str[80];
    char *name_ptr;
    char title_str[80];
    pwr_tObjid objid_vect[11];
    Widget w;
    int sts;
    char name_array[10][120];
    int i, names;
    int plotgroup_found = 0;
    pwr_tObjid plotgroup = pwr_cNObjid;
    pwr_tClassId classid;
    pwr_tObjid node_objid;
    char trend_name[120];
    XttTrend *trend;

    // Command is "OPEN TREND"

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
      {
        /* Get the selected object */
        sts = xnav->get_current_object( &objid_vect[0], name_str, 
	  sizeof( name_str), cdh_mName_path | cdh_mName_object);
        if ( EVEN(sts))
        {
          xnav->message('E', "Enter name or select an object");
          return XNAV__SUCCESS;
        }
        name_ptr = name_str;
      }
    }

    // The name string can contain several trends separated by ','
    names = dcli_parse( name_str, ",", "",
	     (char *) name_array, sizeof( name_array)/sizeof( name_array[0]), 
	     sizeof( name_array[0]), 0);

    for ( i = 0; i < names; i++) {
      if ( strncmp( name_array[i], "*-", 2) == 0)
      {
        // Replace * by the node object
        sts = gdh_GetNodeObject( 0, &node_objid);
        if ( EVEN(sts)) return sts;
        sts = gdh_ObjidToName( node_objid, trend_name, sizeof(trend_name), 
			cdh_mNName);
        if ( EVEN(sts)) return sts;
        strcat( trend_name, &name_array[i][1]);
      }
      else
        strcpy( trend_name, name_array[i]);

      sts = gdh_NameToObjid( trend_name, &objid_vect[i]);
      if (EVEN(sts)) {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }
      sts = gdh_GetObjectClass( objid_vect[i], &classid);
      if (EVEN(sts)) return sts;

      switch ( classid) {
        case pwr_cClass_DsTrend:
          break;
        case pwr_cClass_PlotGroup:
          plotgroup = objid_vect[i];
          plotgroup_found = 1;
          if ( i != 0) {
            xnav->message('E', "Error in object class");
            return XNAV__HOLDCOMMAND;
          }
          break;
        default:
          xnav->message('E', "Error in object class");
          return XNAV__HOLDCOMMAND;
      }
      if ( plotgroup_found)
        break;
    }
    objid_vect[i] = pwr_cNObjid;    

    if ( EVEN( dcli_get_qualifier( "/TITLE", title_str))) {
      if ( plotgroup_found) {
        char attr[140];

        // Get title from plotgroup object
        strcpy( attr, trend_name);
        strcat( attr, ".Title");
        sts = gdh_GetObjectInfo( attr, &title_str, sizeof(title_str));
        if ( EVEN(sts)) return sts;
      }
      else
        strcpy( title_str, "Trend");
    }

    if ( plotgroup_found) {
      if ( xnav->appl.find( applist_eType_Trend, plotgroup, (void **) &trend)) {
        trend->pop();
      }
      else {
        trend = new XttTrend( xnav, xnav->parent_wid, title_str, &w, NULL, 
		  plotgroup, &sts);
        if ( EVEN(sts))
          xnav->message('E',"Error in trend configuration");
        else {
          trend->close_cb = xnav_trend_close_cb;
          xnav->appl.insert( applist_eType_Trend, (void *)trend, plotgroup, "",
		   NULL);
        }
      }
    }
    else
      new XttTrend( xnav, xnav->parent_wid, title_str, &w, objid_vect, 
		  pwr_cNObjid, &sts);
  }
  else if ( strncmp( arg1_str, "FAST", strlen( arg1_str)) == 0)
  {
    char name_str[80];
    char *name_ptr;
    char title_str[80];
    pwr_tObjid objid;
    Widget w;
    int sts;
    pwr_tClassId classid;

    // Command is "OPEN FAST"

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
      {
        /* Get the selected object */
        sts = xnav->get_current_object( &objid, name_str, 
	  sizeof( name_str), cdh_mName_path | cdh_mName_object);
        if ( EVEN(sts))
        {
          xnav->message('E', "Enter name or select an object");
          return XNAV__SUCCESS;
        }
        name_ptr = name_str;
      }
    }

    sts = gdh_NameToObjid( name_str, &objid);
    if (EVEN(sts)) {
      xnav->message('E', "Object not found");
      return XNAV__HOLDCOMMAND;
    }
    sts = gdh_GetObjectClass( objid, &classid);
    if (EVEN(sts)) return sts;

    switch ( classid) {
    case pwr_cClass_DsFastCurve:
      break;
    default:
      xnav->message('E', "Error in object class");
      return XNAV__HOLDCOMMAND;
    }

    if ( EVEN( dcli_get_qualifier( "/TITLE", title_str))) {
      strcpy( title_str, "Fast");
    }

    new XttFast( xnav, xnav->parent_wid, title_str, &w, objid, 
		 &sts);
  }
  else if ( strncmp( arg1_str, "URL", strlen( arg1_str)) == 0)
  {
    char	arg2_str[80];
    int         sts;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      xnav->message('E',"Syntax error");
      return 1;
    }

    sts = xnav_open_URL( arg2_str);
    if ( EVEN(sts))
      xnav->message(' ', XNav::get_message(sts));
  }
  else
    xnav->message('E',"Syntax error");

  return XNAV__SUCCESS;	
}


static int	xnav_close_func(	void		*client_data,
				        void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
    char	object_str[80];
    char	instance_str[80];
    char	*instance_p;

    if ( ODD( dcli_get_qualifier( "/INSTANCE", instance_str)))
      instance_p = instance_str;
    else
      instance_p = 0;

    if ( ODD( dcli_get_qualifier( "/OBJECT", object_str)))
    {
      pwr_tObjid objid;
      pwr_tObjid node_objid;
      char	xttgraph_name[120];
      int 	sts;
      pwr_sClass_XttGraph *xttgraph_p;
      char	action[80];
      char	*s;

      IF_NOGDH_RETURN;
      if ( strncmp( object_str, "*-", 2) == 0)
      {
        // Replace * by the node object
        sts = gdh_GetNodeObject( 0, &node_objid);
        if ( EVEN(sts)) return sts;
        sts = gdh_ObjidToName( node_objid, xttgraph_name, sizeof(xttgraph_name), 
			cdh_mNName);
        if ( EVEN(sts)) return sts;
        strcat( xttgraph_name, &object_str[1]);
      }
      else
        strcpy( xttgraph_name, object_str);
        
      sts = gdh_NameToObjid( xttgraph_name, &objid);
      if (EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }

      sts = gdh_ObjidToPointer( objid, (void **) &xttgraph_p);
      if ( EVEN(sts)) return sts;

      cdh_ToLower( action, xttgraph_p->Action);
      if ( (s = strstr( action, ".pwg"))) {
        *s = 0;
        xnav->close_graph( action, 0);
      }
    }
    else
    {
      char file_str[80];
      char fname[80];

      // Command is "CLOSE GRAPH"
      if ( ODD( dcli_get_qualifier( "dcli_arg2", file_str)))
      {
        if ( file_str[0] == '/')
        {
          xnav->message('E', "Syntax error");
          return XNAV__HOLDCOMMAND;
        }
      }
      else if ( EVEN( dcli_get_qualifier( "/FILE", file_str)))
      {
        xnav->message('E',"Enter file");
        return XNAV__HOLDCOMMAND;
      }

      // Get base class graphs on $pwr_exe
      cdh_ToUpper( fname, file_str);
      if ( instance_p && 
           (strncmp( fname, "PWR_C_", 6) == 0 || 
            strncmp( fname, "PWR_T_", 6) == 0)) {
	strcpy( fname, "$pwr_exe/");
        strcat( fname, file_str);
        strcpy( file_str, fname);
      }  

      xnav->close_graph( file_str, instance_p);
      return XNAV__SUCCESS;	
    }
  }
  else if ( strncmp( arg1_str, "TREND", strlen( arg1_str)) == 0)
  {

    char name_str[80];
    char *name_ptr;
    pwr_tObjid objid;
    int sts;
    int plotgroup_found = 0;
    pwr_tClassId classid;
    pwr_tObjid node_objid;
    char trend_name[120];
    XttTrend *trend;

    // Command is "CLOSE TREND"

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND; 	
      } 
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Enter name");
        return XNAV__SUCCESS;
      }
    }

    if ( strncmp( name_str, "*-", 2) == 0)
    {
      // Replace * by the node object
      sts = gdh_GetNodeObject( 0, &node_objid);
      if ( EVEN(sts)) return sts;
      sts = gdh_ObjidToName( node_objid, trend_name, sizeof(trend_name), 
			cdh_mNName);
      if ( EVEN(sts)) return sts;
      strcat( trend_name, &name_str[1]);
    }
    else
      strcpy( trend_name, name_str);

    sts = gdh_NameToObjid( trend_name, &objid);
    if (EVEN(sts)) {
      xnav->message('E', "Object not found");
      return XNAV__HOLDCOMMAND;
    }
    sts = gdh_GetObjectClass( objid, &classid);
    if (EVEN(sts)) return sts;

    switch ( classid) {
      case pwr_cClass_PlotGroup:
        plotgroup_found = 1;
        break;
      default:
        xnav->message('E', "Error in object class");
        return XNAV__HOLDCOMMAND;
    }

    if ( plotgroup_found) {
      if ( xnav->appl.find( applist_eType_Trend, objid, (void **) &trend)) {
        xnav->appl.remove( (void *)trend);
        delete trend;
      }
    }
  }
  else if ( strncmp( arg1_str, "ALARMLIST", strlen( arg1_str)) == 0)
  {
    if ( xnav->ev && xnav->ev->is_mapped_ala())
      xnav->ev->unmap_ala();
  }
  else if ( strncmp( arg1_str, "EVENTLIST", strlen( arg1_str)) == 0)
  {
    if ( xnav->ev && xnav->ev->is_mapped_eve())
      xnav->ev->unmap_eve();
  }
  else if ( strncmp( arg1_str, "NAVIGATOR", strlen( arg1_str)) == 0)
  {
    if ( xnav->close_cb)
      (xnav->close_cb)( xnav->parent_ctx);
  }
  else if ( strncmp( arg1_str, "ALL", strlen( arg1_str)) == 0)
  {
    char except_str[80];
    char name_array[20][80];
    int names;
    ApplListElem *elem, *next_elem;
    applist_eType type;
    int keep;
    int i;
        

    // Close everything

    if ( ODD( dcli_get_qualifier( "/EXCEPT", except_str))) {
      // The except string can contain several items separated by ','
      names = dcli_parse( except_str, ",", "",
	     (char *) name_array, sizeof( name_array)/sizeof( name_array[0]), 
	     sizeof( name_array[0]), 0);
    }
    else
      names = 0;

    // Close graphs
    type = applist_eType_Graph;
    for ( elem = xnav->appl.root; elem;) {
      if ( elem->type == type) {
        keep = 0;
        for ( i = 0; i < names; i++) {
          if ( cdh_NoCaseStrcmp( name_array[i], elem->name) == 0) {
            keep = 1;
            break;
          }
	}
        if ( keep) {
          elem = elem->next;
          continue;
        }
        next_elem = elem->next;
        xnav->close_graph( elem->name, elem->instance);
        elem = next_elem;
        continue;
      }
      elem = elem->next;
    }

    // Close all trends
    type = applist_eType_Trend;
    for ( elem = xnav->appl.root; elem;) {
      if ( elem->type == type) {
        next_elem = elem->next;
        delete (XttTrend *)elem->ctx;
        xnav->appl.remove( elem->ctx);
        elem = next_elem;
        continue;
      }
      elem = elem->next;
    }

    //new code Jonas Nylund 030131
    // Close all hists
    type = applist_eType_Hist;
    for ( elem = xnav->appl.root; elem;) {
      if ( elem->type == type) {
        next_elem = elem->next;
        delete (Hist *)elem->ctx;
        xnav->appl.remove( elem->ctx);
        elem = next_elem;
        continue;
      }
      elem = elem->next;
    }


    // Close navigator
    keep = 0;
    for ( i = 0; i < names; i++) {
      if ( cdh_NoCaseStrcmp( name_array[i], "NAVIGATOR") == 0) {
        keep = 1;
        break;
      }
    }
    if ( !keep) {
      if ( xnav->close_cb)
        (xnav->close_cb)( xnav->parent_ctx);
    }

    // Close alarmlist
    keep = 0;
    for ( i = 0; i < names; i++) {
      if ( cdh_NoCaseStrcmp( name_array[i], "ALARMLIST") == 0) {
        keep = 1;
        break;
      }
    }
    if ( !keep) {
      if ( xnav->ev && xnav->ev->is_mapped_ala())
        xnav->ev->unmap_ala();
    }

    // Close eventlist
    keep = 0;
    for ( i = 0; i < names; i++) {
      if ( cdh_NoCaseStrcmp( name_array[i], "EVENTLIST") == 0) {
        keep = 1;
        break;
      }
    }
    if ( !keep) {
      if ( xnav->ev && xnav->ev->is_mapped_eve())
        xnav->ev->unmap_eve();
    }
  }
  else
    xnav->message('E',"Syntax error");

  return XNAV__SUCCESS;	
}

static void xnav_ge_help_cb( ge_tCtx gectx, char *key)
{
  XNav *xnav = (XNav *)gectx->parent_ctx;
  int sts;

  sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Project, NULL, 0);
  if ( EVEN(sts))
    sts = CoXHelp::dhelp( "graph window", "", navh_eHelpFile_Base, NULL, 0);
  if ( EVEN(sts))
    xnav->message( 'E', "Unable to find topic");
  else
    xnav->message( ' ', null_str);
}

static int xnav_ge_command_cb( ge_tCtx gectx, char *command)
{
  return ((XNav *)gectx->parent_ctx)->command( command);
}

static void xnav_ge_close_cb( ge_tCtx gectx)
{
  ((XNav *)gectx->parent_ctx)->appl.remove( (void *)gectx);
}

//new code Jonas Nylund 030131
static void xnav_hist_close_cb( void *ctx)
{
  ((XNav *)((Hist *)ctx)->parent_ctx)->appl.remove( (void *)ctx);
  delete (Hist *)ctx;
}
//end new code Jonas Nylund 030131

static void xnav_trend_close_cb( void *ctx, XttTrend *trend)
{
  XNav *xnav = (XNav *) ctx;

  xnav->appl.remove( (void *)trend);
  delete trend;
}

static int	xnav_create_func( void		*client_data,
				void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "ITEM", strlen( arg1_str)) == 0)
  {
    char text_str[80];
    char destination_str[80];
    char command_str[80];
    char tmp_str[80];
    int dest_code;
    int item_type;
    xnav_sMenu *menu_item;

    // Command is "CREATE ITEM"
    if ( ODD( dcli_get_qualifier( "dcli_arg2", text_str)))
    {
      if ( text_str[0] == '/')
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else if ( EVEN( dcli_get_qualifier( "/TEXT", text_str)))
    {
      xnav->message('E',"Enter text");
      return XNAV__HOLDCOMMAND;
    }
    if ( EVEN( dcli_get_qualifier( "/DESTINATION", destination_str)))
    {
      xnav->message('E',"Enter destination");
      return XNAV__HOLDCOMMAND;
    }
    if ( ODD( dcli_get_qualifier( "/COMMAND", command_str)))
      item_type = xnav_eItemType_Command;
    if ( ODD( dcli_get_qualifier( "/MENU", tmp_str)))
      item_type = xnav_eItemType_Menu;
    if ( ODD( dcli_get_qualifier( "/AFTER", tmp_str)))
      dest_code = xnav_eDestCode_After;
    else if ( ODD( dcli_get_qualifier( "/BEFORE", tmp_str)))
      dest_code = xnav_eDestCode_Before;
    else if ( ODD( dcli_get_qualifier( "/FIRSTCHILD", tmp_str)))
      dest_code = xnav_eDestCode_FirstChild;
    else if ( ODD( dcli_get_qualifier( "/LASTCHILD", tmp_str)))
      dest_code = xnav_eDestCode_LastChild;
    else
      dest_code = xnav_eDestCode_After;

    sts = xnav->menu_tree_insert( text_str, item_type, command_str,
		destination_str, dest_code, &menu_item);
    if ( EVEN(sts))
      xnav->message('E',"Destination not found");

    return XNAV__SUCCESS;	
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    // Command is "CREATE OBJECT2

    char	name_str[80];
    char	class_str[80];

    IF_NOGDH_RETURN;

    // Check authorization
    if ( !((xnav->priv & pwr_mPrv_RtWrite) ||
           (xnav->priv & pwr_mPrv_System)))
    {
      xnav->message('E', "Not authorized for this operation");
      return 0;
    }

    if ( EVEN( dcli_get_qualifier( "/CLASS", class_str)))
    {
      xnav->message('E',"Enter class");	
      return XNAV__HOLDCOMMAND;
    }

    if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
    {
      xnav->message('E',"Enter name");	
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->create_object( class_str, name_str);
    return sts;
  }
  else
    xnav->message('E',"Syntax error");

  return XNAV__SUCCESS;	
}

static int	xnav_delete_func( void		*client_data,
				void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;

  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "ITEM", strlen( arg1_str)) == 0)
  {
    char name_str[80];

    // Command is "DELETE ITEM"
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str)))
    {
      if ( name_str[0] == '/')
      {
        xnav->message('E', "Syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
    {
      xnav->message('E',"Enter name");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->menu_tree_delete( name_str);
    if ( EVEN(sts))
      xnav->message('E',"Item not found");

    return XNAV__SUCCESS;	
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    // Command is "DELETE OBJECT"

    char	name_str[80];

    IF_NOGDH_RETURN;

    // Check authorization
    if ( !((xnav->priv & pwr_mPrv_RtWrite) ||
           (xnav->priv & pwr_mPrv_System)))
    {
      xnav->message('E', "Not authorized for this operation");
      return 0;
    }

    if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
    {
      xnav->message('E',"Enter name");	
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->delete_object( name_str);
    return sts;
  }
  else
    xnav->message('E',"Syntax error");

  return XNAV__SUCCESS;	
}

static int	xnav_collect_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg1_str[80];
  char	name_str[80];
  pwr_sAttrRef attrref;
  int 	is_attr;

  IF_NOGDH_RETURN;

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = gdh_NameToAttrref( pwr_cNObjid, name_str, &attrref);
      if ( EVEN(sts))
      {
        xnav->message('E', "No such object");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      sts = xnav->get_select( &attrref, &is_attr);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select object");
        return XNAV__SUCCESS;
      }
    }
    sts = xnav->collect_insert( &attrref);
    return sts;
  }
  else if ( strncmp( arg1_str, "SHOW", strlen( arg1_str)) == 0)
  {
    sts = xnav->collect_show();
    return sts;
  }
  else if ( strncmp( arg1_str, "CLEAR", strlen( arg1_str)) == 0)
  {
    xnav->collect_clear();
    return XNAV__SUCCESS;
  }
  else
  {
    xnav->message('E', "Unknown qualifier");
    return XNAV__HOLDCOMMAND;
  }
}

static int	xnav_store_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	*file_ptr;
  char	file_str[80];
  char	filename[80];
  char	str[80];
  int	collect;

    IF_NOGDH_RETURN;

    if ( ODD( dcli_get_qualifier( "/SYMBOLS", str)))
    {
      if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
        file_ptr = file_str;
      else if ( ODD( dcli_get_qualifier( "dcli_arg1", file_str)))
      {
        if ( file_str[0] != '/')
          /* Assume that this is the filestring */
          file_ptr = file_str;
        else
          file_ptr = xnav->gbl.symbolfilename;
      }
      else
        file_ptr = xnav->gbl.symbolfilename;

      dcli_get_defaultfilename( file_ptr, filename, ".rtt_com");
      sts = dcli_store_symbols( filename);
      return sts;
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
        file_ptr = file_str;
      else if ( ODD( dcli_get_qualifier( "dcli_arg1", file_str)))
      {
        if ( file_str[0] != '/')
          /* Assume that this is the filestring */
          file_ptr = file_str;
        else
        {
          xnav->message('E',"Enter file");	
          return XNAV__HOLDCOMMAND; 	
        } 
      }
      else
      {
        xnav->message('E',"Enter file");	
        return XNAV__HOLDCOMMAND; 	
      } 

      /* Store a menue */
      if ( ODD( dcli_get_qualifier( "/COLLECT", str)))
        collect = 1;
      else
        collect = 0;

      sts = xnav->store( file_str, collect);
      return sts;
    }
}

static int	xnav_crossref_func(	void		*client_data,
					void		*client_flag)
{
  XNav 		*xnav = (XNav *)client_data;
  int		sts;
  char		file_str[80];
  char		name_str[80];
  char		string_str[80];
  char		func_str[80];
  char		*file_ptr;
  char		*name_ptr;
  pwr_tObjid	objid;
  pwr_tClassId	classid;
  int		brief;
  int           window;
  int		case_sens;
  char		title[120];

  IF_NOGDH_RETURN;

  if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
    file_ptr = file_str;
  else
    file_ptr = NULL;

  if ( ODD( dcli_get_qualifier( "/STRING", string_str)))
  {
    if ( ODD( dcli_get_qualifier( "/FUNCTION", func_str)))
    {
      xnav->message('E', "Syntax error");
      return XNAV__SUCCESS;
    }
    brief = ODD( dcli_get_qualifier( "/BRIEF", name_str));
    case_sens = ODD( dcli_get_qualifier( "/CASE_SENSITIVE", name_str));
    xnav->brow_pop();
    brow_SetNodraw( xnav->brow->ctx);
    sts = xnav_crr_code( xnav->brow, file_ptr, string_str, brief, 0, case_sens);
    brow_ResetNodraw( xnav->brow->ctx);
    brow_Redraw( xnav->brow->ctx, 0);
    if ( EVEN(sts)) {
      xnav->brow_push();
      xnav->message(' ', XNav::get_message(sts));
    }
  }
  else if ( ODD( dcli_get_qualifier( "/FUNCTION", func_str)))
  {
    brief = ODD( dcli_get_qualifier( "/BRIEF", name_str));
    case_sens = ODD( dcli_get_qualifier( "/CASE_SENSITIVE", name_str));
    xnav->brow_pop();
    brow_SetNodraw( xnav->brow->ctx);
    sts = xnav_crr_code( xnav->brow, file_ptr, func_str, brief, 1, case_sens);
    brow_ResetNodraw( xnav->brow->ctx);
    brow_Redraw( xnav->brow->ctx, 0);
    if ( EVEN(sts)) {
      xnav->brow_push();
      xnav->message(' ', XNav::get_message(sts));
    }
  }
  else
  {
    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg1", name_str)))
    {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else
      {
        xnav->message('E', "Syntax error");
        return XNAV__SUCCESS;
      } 
    }
    else if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      name_ptr = name_str;
      sts = gdh_NameToObjid ( name_str, &objid);
      if (EVEN(sts))
      {
        xnav->message('E', "Object not found");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      /* Get the selected object */
      sts = xnav->get_current_object( &objid, name_str, 
	sizeof( name_str), cdh_mName_path | cdh_mName_object);
      if ( EVEN(sts))
      {
        xnav->message('E', "Enter name or select an object");
        return XNAV__SUCCESS;
      }
      name_ptr = name_str;
    }

    sts = gdh_GetObjectClass ( objid, &classid);
    if ( EVEN(sts)) return sts;

    window = ODD( dcli_get_qualifier( "/WINDOW", NULL));
    if ( window) {
      XCrr *xcrr = new XCrr( xnav->form_widget, xnav, objid, 
		       xnav->gbl.advanced_user, &sts);
      if ( EVEN(sts))
        xnav->message(' ', XNav::get_message(sts));
      else {
        xcrr->popup_menu_cb = xnav_popup_menu_cb;
        xcrr->start_trace_cb = xnav_start_trace_cb;
      }
      return XNAV__SUCCESS;
    }

    xnav->brow_pop();
    brow_SetNodraw( xnav->brow->ctx);

    sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str), 
			cdh_mNName);
    sprintf( title, "Crossreference list %s\n", name_str);
    new ItemHeader( xnav->brow, "Title", title, NULL, flow_eDest_IntoLast);

    switch ( classid)
    {
      case pwr_cClass_Di:
      case pwr_cClass_Dv:
      case pwr_cClass_Do:
      case pwr_cClass_Po:
      case pwr_cClass_Av:
      case pwr_cClass_Ai:
      case pwr_cClass_Ao:
        sts = xnav_crr_signal( xnav->brow, file_ptr, name_ptr, NULL);
        break;
      default:
        /* Not a signal */
        sts = xnav_crr_object( xnav->brow, file_ptr, name_ptr, NULL);
    }
    if ( EVEN(sts))
      xnav->message(' ', XNav::get_message(sts));
    brow_ResetNodraw( xnav->brow->ctx);
    brow_Redraw( xnav->brow->ctx, 0);

  }
  return sts;	  
}

static int	xnav_exit_func(	void		*client_data,
				void		*client_flag)
{
  XNav 		*xnav = (XNav *)client_data;

  if ( xnav->close_cb)
    (xnav->close_cb)( xnav->parent_ctx);
  else
    exit(1);
  return 1;
}

static int	xnav_setup_func(	void		*client_data,
					void		*client_flag)
{
  XNav 		*xnav = (XNav *)client_data;

  xnav->setup();
  return 1;
}

static int	xnav_search_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  int	sts;
  char	arg1_str[80];
  int	regexp;
	
  if ( ODD( dcli_get_qualifier( "/NEXT", NULL)))
  {
    sts = xnav->search_next();
    if ( EVEN(sts))
      xnav->message(' ', XNav::get_message(sts));
    return sts;
  }
  else if ( ODD( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    regexp =  ODD( dcli_get_qualifier( "/REGULAREXPRESSION", NULL));

    sts = xnav->search( arg1_str, regexp);
    if ( EVEN(sts))
      xnav->message(' ', XNav::get_message(sts));
    return sts;
  }
  else
  {
    xnav->message('E',"Syntax error");
    return XNAV__SYNTAX;
  }
}

static int	xnav_test_func(		void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  char	arg1_str[80];
  int	arg1_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "BELL", strlen( arg1_str)) == 0)
  {
    // Command is "TEST BELL"
    XBell( flow_Display(xnav->brow_widget), 100);
  }
  else if ( strncmp( arg1_str, "BEEP", strlen( arg1_str)) == 0)
  {
    // Command is "TEST BEEP"
    putchar( '\7' );
  }
  return 1;
}

static int	xnav_logging_func(	void		*client_data,
					void		*client_flag)
{
  XNav *xnav = (XNav *)client_data;
  char	arg1_str[80];
  int	arg1_sts;
  int	sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "SET", strlen( arg1_str)) == 0 ||
       strncmp( arg1_str, "CREATE", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING SET" */

    char	entry_str[80];
    char	line_size_str[80];
    char	parameter_str[80];
    char	time_str[80];
    char	file_str[80];
    char	condition_str[80];
    char	buffer_size_str[80];
    char	priority_str[80];
    char	str[80];
    int	        stop;
    int	        priority;
    int 	entry;
    int 	line_size;
    char	*parameter_ptr;
    int 	logg_time;
    char	*file_ptr;
    char	*condition_ptr;
    int 	buffer_size;
    int 	nr;
    int 	logg_type;
    int 	insert;
    int 	create;
    int 	shortname;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      if ( !strcmp( entry_str, "CURRENT"))
      {
        if ( xnav->current_logging_index == -1)
        {
          xnav->message('E', "No current logging entry");
          return XNAV__HOLDCOMMAND;
        }
        entry = xnav->current_logging_index + 1;
      }
      else
      {
        /* convert to integer */
        nr = sscanf( entry_str, "%d", &entry);
        if ( nr != 1)
        {
          xnav->message('E', "Entry syntax error");
          return XNAV__HOLDCOMMAND;
        }
      }
    }
    else
    {
      xnav->message('E',"Enter entry");  
      return XNAV__HOLDCOMMAND;
    }

    if ( ODD( dcli_get_qualifier( "/TIME", time_str)))
    {
      /* convert to integer */
      nr = sscanf( time_str, "%d", &logg_time);
      if ( nr != 1)
      {
        xnav->message('E', "Time syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      logg_time = 0;

    if ( ODD( dcli_get_qualifier( "/BUFFER_SIZE", buffer_size_str)))
    {
      /* convert to integer */
      nr = sscanf( buffer_size_str, "%d", &buffer_size);
      if ( nr != 1)
      {
        xnav->message('E', "Buffer size syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      buffer_size = 0;

    if ( ODD( dcli_get_qualifier( "/LINE_SIZE", line_size_str)))
    {
      /* convert to integer */
      nr = sscanf( line_size_str, "%d", &line_size);
      if ( nr != 1)
      {
        xnav->message('E', "Line size syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      line_size = 0;

    if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
      file_ptr = file_str;
    else
      file_ptr = NULL;

    if ( ODD( dcli_get_qualifier( "/PARAMETER", parameter_str)))
      parameter_ptr = parameter_str;
    else
      parameter_ptr = NULL;

    if ( ODD( dcli_get_qualifier( "/CONDITION", condition_str)))
      condition_ptr = condition_str;
    else
      condition_ptr = NULL;

    if ( ODD( dcli_get_qualifier( "/TYPE", str)))
    {
      if ( strncmp( str, "EVENT", strlen( str)) == 0)
        logg_type = xtt_LoggType_Mod;
      else if ( strncmp( str, "CONTINOUS", strlen( str)) == 0)
        logg_type = xtt_LoggType_Cont;
    }
    else
      logg_type = 0;

    if ( ODD( dcli_get_qualifier( "/PRIORITY", priority_str)))
    {
      /* convert to integer */
      nr = sscanf( priority_str, "%d", &priority);
      if ( nr != 1)
      {
        xnav->message('E', "Priority syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
      priority = -1;

    if ( ODD( dcli_get_qualifier( "/STOP", str)))
      stop = 1;
    else if ( ODD( dcli_get_qualifier( "/NOSTOP", str)))
      stop = 0;
    else
      stop = -1;

    if ( ODD( dcli_get_qualifier( "/INSERT", str)))
      insert = 1;
    else
      insert = 0;

    if ( ODD( dcli_get_qualifier( "/SHORTNAME", str)))
      shortname = 1;
    else if ( ODD( dcli_get_qualifier( "/NOSHORTNAME", str)))
      shortname = 0;
    else 
      shortname = -1;

    if ( ODD( dcli_get_qualifier( "/CREATE", str)))
      create = 1;
    else
      create = 0;

    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->logg[entry-1].logging_set( logg_time, file_ptr, 
        parameter_ptr, condition_ptr, logg_type, insert, buffer_size,
  	stop, priority, create, line_size, shortname);
    return sts;
  }

  else if ( strncmp( arg1_str, "DELETE", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING DELETE" */

    char	entry_str[80];
    char	parameter_str[80];
    char	*parameter_ptr;
    int	entry;
    int	nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      xnav->message('E',"Enter entry");  
      return XNAV__HOLDCOMMAND;
    }

    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    if ( ODD( dcli_get_qualifier( "/PARAMETER", parameter_str)))
      parameter_ptr = parameter_str;
    else
      return XNAV__SUCCESS;

    sts = xnav->logg[entry-1].remove( parameter_ptr);
    return sts;
  }

  else if ( strncmp( arg1_str, "START", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING START" */

    char  entry_str[80];
    int  entry;
    int  nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      xnav->message('E',"Enter entry");  
      return XNAV__HOLDCOMMAND;
    }

    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->logg[entry-1].start();
    return sts;
  }

  else if ( strncmp( arg1_str, "STOP", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING STOP" */

    char	entry_str[80];
    int	entry;
    int	nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      xnav->message('E',"Enter entry");  
      return XNAV__HOLDCOMMAND;
    }

    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->logg[entry-1].stop();
    return sts;
  }
  else if ( strncmp( arg1_str, "SHOW", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING SHOW" */

    char  entry_str[80];
    int  entry;
    int  nr;

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      entry = 0;
    }

    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->logg[entry-1].show();
    return sts;
  }
  else if ( strncmp( arg1_str, "STORE", strlen( arg1_str)) == 0)
  {
    /* Command is "LOGGING STORE" */

    char	entry_str[80];
    char	file_str[80];
    char        *file_ptr;
    int         entry;
    int   	nr;

    if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
      file_ptr = file_str;
    else if ( ODD( dcli_get_qualifier( "dcli_arg2", file_str)))
    {
      if ( file_str[0] != '/')
        /* Assume that this is the filestring */
        file_ptr = file_str;
      else
      {
        xnav->message('E',"Enter file");  
        return XNAV__HOLDCOMMAND;
      } 
    }
    else
    {
      xnav->message('E',"Enter file");  
      return XNAV__HOLDCOMMAND;
    } 

    if ( ODD( dcli_get_qualifier( "/ENTRY", entry_str)))
    {
      /* convert to integer */
      nr = sscanf( entry_str, "%d", &entry);
      if ( nr != 1)
      {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
    }
    else
    {
      if ( xnav->current_logging_index == -1) {
        xnav->message('E', "Entry syntax error");
        return XNAV__HOLDCOMMAND;
      }
      entry = xnav->current_logging_index + 1;
    }
    if ( entry > XNAV_LOGG_MAX || entry < 1) {
      xnav->message( 'E', "Entry out of range");
      return XNAV__HOLDCOMMAND;
    }

    sts = xnav->logg[entry-1].store( file_str);
    return sts;
  }
  else
  {
    xnav->message('E', "Unknown qualifier");
    return XNAV__HOLDCOMMAND;
  }

  // return XNAV__SUCCESS;  
}


int XNav::show_database()
{
  int		sts;
  pwr_tObjid 	root;
  pwr_tClassId	classid;
  Item 		*item;
  int		toplevel_cnt;


  // Get the toplevel objects
  sts = gdh_GetRootList( &root);
  if ( sts == GDH__NOSUCHOBJ)
     return  1;
  if ( EVEN(sts)) return sts;

  //  Loop through all root objects and see if they are valid at toplevel
  toplevel_cnt = 0;
  while ( ODD(sts))
  {
    sts = gdh_GetObjectClass( root, &classid);
    if (ODD(sts))
    {
      if ( !toplevel_cnt)
      {
        brow_pop();
        brow_SetNodraw( brow->ctx);
      }
      sts = create_object_item( root, NULL, 
			flow_eDest_IntoLast, (void **)&item, 0);
      if ( EVEN(sts)) return sts;
      toplevel_cnt++;
    }
    sts = gdh_GetNextSibling( root, &root);
  }

  if ( !toplevel_cnt)
  {
    message( 'I', "No objects found in database");
    return 1;
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return 1;
}

int XNav::command( char* input_str)
{
  char		command[200];
  int		sts, sym_sts;
  char		symbol_value[80];


  if ( input_str[0] == '@')
  {
    sts = dcli_replace_symbol( input_str, command, sizeof(command));
    if (EVEN(sts)) return sts;

    /* Read command file */
    sts = readcmdfile( &command[1]);
    if ( sts == DCLI__NOFILE)
    {
      message('E',"Unable to open file");
      return DCLI__SUCCESS;
    }
    else if ( EVEN(sts)) return sts;
    return DCLI__SUCCESS;
  }
  dcli_toupper( input_str, input_str);
  sts = dcli_replace_symbol( input_str, command, sizeof(command));
  if ( EVEN(sts)) return sts;

  sts = dcli_cli( (dcli_tCmdTable *)&xnav_command_table, command, (void *) this, 0);
  if ( sts == DCLI__COM_NODEF)
  {
    /* Try to find a matching symbol */
    sym_sts = dcli_get_symbol_cmd( command, symbol_value);
    if ( ODD(sym_sts))
    {
      if ( symbol_value[0] == '@')
      {
        /* Read command file */
        sts = readcmdfile( &symbol_value[1]);
        if ( sts == DCLI__NOFILE)
        {
          message('E',"Unable to open file");
          return DCLI__SUCCESS;
        }
        else if ( EVEN(sts)) return sts;
        return DCLI__SUCCESS;
      }
      sts = dcli_cli( (dcli_tCmdTable *)&xnav_command_table, symbol_value, (void *) this, 0);
    }
    else if ( sym_sts == DCLI__SYMBOL_AMBIG)
      sts = sym_sts;
  }
  if (sts == DCLI__COM_AMBIG) message('E',"Ambiguous command");
  else if (sts == DCLI__COM_NODEF) message('E',"Undefined command");
  else if (sts == DCLI__QUAL_AMBIG) message('E',"Ambiguous qualifier");
  else if (sts == DCLI__QUAL_NODEF) message('E',"Undefined qualifier");
  else if (sts == DCLI__SYMBOL_AMBIG) message('E', "Ambiguous symbol abbrevation");

  return DCLI__SUCCESS;
}

static int xnav_getcurrenttext_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  XNav *xnav;
  char text[80];
  brow_tNode	*node_list;
  int		node_count;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  xnav_get_stored_xnav( &xnav);
  brow_GetSelectedNodes( xnav->brow->ctx, &node_list, &node_count);
  if ( !node_count)
    strcpy( return_string, "");
  else
  {
    // Get annotation number 0
    brow_GetAnnotation( node_list[0], 0, text, sizeof(text));
    free( node_list);
    strcpy( return_string, text);
  }
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getcurrentobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  XNav			*xnav;
  char			name[80];
  int			sts;
  pwr_sAttrRef		attrref;
  int			is_attr;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  xnav_get_stored_xnav( &xnav);
  sts = xnav->get_select( &attrref, &is_attr);
  if ( ODD(sts))
    sts = gdh_ObjidToName( attrref.Objid, name, sizeof(name), cdh_mNName);
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getchild_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_NameToObjid( arg_list->value_string, &parent_objid);
  if ( ODD(sts))
  {
    sts = gdh_GetChild( parent_objid, &child_objid);
    if (ODD(sts))
      sts = gdh_ObjidToName( child_objid, name, sizeof(name), cdh_mNName);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getparent_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_NameToObjid( arg_list->value_string, &child_objid);
  if ( ODD(sts))
  {
    sts = gdh_GetParent( child_objid, &parent_objid);
    if (ODD(sts))
      sts = gdh_ObjidToName( parent_objid, name, sizeof(name), cdh_mNName);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getnextsibling_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;
  pwr_tObjid	next_objid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_NameToObjid( arg_list->value_string, &objid);
  if ( ODD(sts))
  {
    sts = gdh_GetNextSibling( objid, &next_objid);
    if (ODD(sts))
      sts = gdh_ObjidToName( next_objid, name, sizeof(name), cdh_mNName);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getclasslist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tClassId	classid;
  pwr_tObjid	objid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_ClassNameToId ( arg_list->value_string, &classid);
  if ( ODD(sts))
  {
    sts = gdh_GetClassList ( classid, &objid);
    if (ODD(sts))
      sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getrootlist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = gdh_GetRootList( &objid);
  if (ODD(sts))
    sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getnodeobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = gdh_GetNodeObject( 0, &objid);
  if (ODD(sts))
    sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_getnextobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;
  pwr_tObjid	next_objid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_NameToObjid( arg_list->value_string, &objid);
  if ( ODD(sts))
  {
    sts = gdh_GetNextObject( objid, &next_objid);
    if (ODD(sts))
      sts = gdh_ObjidToName( next_objid, name, sizeof(name), cdh_mNName);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}


static int xnav_getobjectclass_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;
  pwr_tClassId	classid;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = gdh_NameToObjid( arg_list->value_string, &objid);
  if ( ODD(sts))
    sts = gdh_GetObjectClass( objid, &classid);
  if ( ODD(sts))
    sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  name, sizeof(name), cdh_mName_object);
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int xnav_messageerror_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  XNav *xnav;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  xnav_get_stored_xnav( &xnav);
  xnav->message('E', arg_list->value_string);

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int xnav_messageinfo_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  XNav *xnav;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  xnav_get_stored_xnav( &xnav);
  xnav->message('I', arg_list->value_string);

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int xnav_cutobjectname_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg 	*arg_p2; 

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  xnav_cut_segments( return_string, arg_list->value_string, 
	arg_p2->value_int);

  *return_decl = CCM_DECL_STRING;
  
  return 1;
}


static int xnav_getattribute_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg	*arg_p2;
  int	sts;
  int	value_decl;
  int	value_int;
  float	value_float;
  char	value_string[80];

  if ( !(arg_count == 2 || arg_count == 1))
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_count == 2 && arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  sts = xnav_attribute_func( arg_list->value_string, &value_decl,
	&value_float, &value_int, value_string);
  if ( EVEN(sts))
  {
    if ( arg_count == 2)
    {
      arg_p2->value_int = 0;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
    *return_decl = CCM_DECL_UNKNOWN;
  }
  else
  {
    if ( value_decl == CCM_DECL_INT)
    {
      *return_int = value_int;
      *return_decl = CCM_DECL_INT;
    }
    else if ( value_decl == CCM_DECL_FLOAT)
    {
      *return_float = value_float;
      *return_decl = CCM_DECL_FLOAT;
    }
    else if ( value_decl == CCM_DECL_STRING)
    {
      strcpy( return_string, value_string);
      *return_decl = CCM_DECL_STRING;
    }
    if ( arg_count == 2)
    {
      arg_p2->value_int = 1;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
  }

  return 1;
}

static int xnav_ccm_deffilename_func( char *outfile, char *infile, void *client_data)
{
  char fname[200];

  dcli_get_defaultfilename( infile, fname, ".rtt_com");
  dcli_translate_filename( outfile, fname);
  return 1;
}

static int xnav_ccm_errormessage_func( char *msg, int severity, void *client_data)
{
  XNav *xnav = (XNav *) client_data; 

  if ( EVEN(severity))
    xnav->message( 'I', msg);
  else
    xnav->message( 'E', msg);
  return 1;
}

int	xnav_externcmd_func( char *cmd, void *client_data)
{
  XNav *xnav = (XNav *) client_data; 

  return xnav->command( cmd);
}

/*************************************************************************
*
* Name:		readcmdfile()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

int XNav::readcmdfile( 	char		*incommand)
{
	char		input_str[160];
	int		sts;
	int		appl_sts;

	if ( !ccm_func_registred)
	{
	  sts = ccm_register_function( "GetCurrentText", xnav_getcurrenttext_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetCurrentObject", xnav_getcurrentobject_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetChild", xnav_getchild_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetParent", xnav_getparent_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNextSibling", xnav_getnextsibling_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetClassList", xnav_getclasslist_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetRootList", xnav_getrootlist_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNodeObject", xnav_getnodeobject_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNextObject", xnav_getnextobject_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetObjectClass", xnav_getobjectclass_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "MessageError", xnav_messageerror_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "MessageInfo", xnav_messageinfo_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "CutObjectName", xnav_cutobjectname_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetAttribute", xnav_getattribute_func);
	  if ( EVEN(sts)) return sts;
	  ccm_func_registred = 1;
        }

	strcpy( input_str, incommand);
	dcli_remove_blank( input_str, input_str);
        xnav_store_xnav( this);

	/* Read and execute the command file */
	sts = ccm_file_exec( input_str, xnav_externcmd_func,
		xnav_ccm_deffilename_func, xnav_ccm_errormessage_func, 
		&appl_sts, verify, 0, NULL, 0, 0, NULL, (void *)this);
        if ( EVEN(sts)) return sts;

	return 1;
}

int xnav_cut_segments (
  char	*outname,
  char	*name,
  int	segments
)
{
	char	*s[20];
	int	i, j, last_i;

	for( i = 0; i < segments; i++)
	{
	  s[i] = strrchr( name, '-');
	  if ( s[i] == 0)
	  {
	    last_i = i;
	    break;
	  }
	  *s[i] = '+';
	  last_i = i;
	}
	for ( j = 0; j <= last_i; j++)
	{
	  if ( s[j] != 0)
	    *s[j] = '-';
	}
	if ( s[last_i] == 0)
	  strcpy( outname, name);
	else
	  strcpy( outname, s[last_i] + 1);

	return 1;
}

/*************************************************************************
*
* Name:		xnav_attribute_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Backcall function for ccm.
*
**************************************************************************/
static int xnav_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string
)
{
	int		sts;
	char		hier_name[80];
	char		object_par[80];
	char		*object_element;
	int		element;
	char		elementstr[20];
	char		*s;
	char		*t;
	int		len;
	int		decl;
	int		int_val;
	float		float_val;
	char		string_val[80];
	pwr_sAttrRef	attrref;
	pwr_tTypeId	attrtype;
	unsigned int	attrsize, attroffs, attrelem;

	sts = gdh_NameToAttrref( pwr_cNObjid, name, &attrref);
	if ( EVEN(sts)) return sts;

	/* Get type of attribute */
	sts = gdh_GetAttributeCharacteristics ( name,
		&attrtype, &attrsize, &attroffs, &attrelem);
	if ( EVEN(sts)) return sts;

	/* Get the pointer to attribute */
	sts = gdh_GetObjectInfo ( name, &object_par, sizeof(object_par));
	if ( EVEN(sts)) return sts;

	/* Check index in parameter */
	s = strchr( name, '[');
	if ( s == 0)
	  element = 0;
	else
	{
	  t = strchr( name, ']');
	  if ( t == 0)
	    return DCLI__ELEMSYNTAX;
	  else
	  {
	    len = t - s - 1;
	    strncpy( elementstr, s + 1, len);
	    elementstr[ len ] = 0;
	    sscanf( elementstr, "%d", &element);
	    *s = '\0';
	    if ( (element < 0) || (element > 100) )
	      return DCLI__ELEMSYNTAX;
	  }
	}
	
	object_element = object_par + element * attrsize / attrelem;

        switch ( attrtype )
        {
          case pwr_eType_Boolean:
          {
	    int_val = *(pwr_tBoolean *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Float32:
          {
	    float_val = *(pwr_tFloat32 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Float64:
          {
	    float_val = *(pwr_tFloat64 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Char:
          {
	    int_val = *(pwr_tChar *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int8:
          {
	    int_val = *(pwr_tInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int16:
          {
	    int_val = *(pwr_tInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int32:
          {
	    int_val = *(pwr_tInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt8:
          {
	    int_val = *(pwr_tUInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt16:
          {
	    int_val = *(pwr_tUInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt32:
	  case pwr_eType_ClassId:
	  case pwr_eType_TypeId:
	  case pwr_eType_VolumeId:
	  case pwr_eType_ObjectIx:
          {
	    int_val = *(pwr_tUInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_String:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Text:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Objid:
          {
            /* Get the object name from ldh */
            sts = gdh_ObjidToName( *(pwr_tObjid *)object_element, 
		hier_name, sizeof( hier_name), cdh_mName_volumeStrict);
            if ( EVEN(sts))
	      strcpy( string_val, "Undefined Object");
	    else
	      strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
	  case pwr_eType_AttrRef:
          {
            /* Get the object name from ldh */
            sts = gdh_AttrrefToName( (pwr_sAttrRef *)object_element,
		hier_name, sizeof(hier_name), cdh_mName_volumeStrict);
 	    if ( EVEN(sts))
	      strcpy( string_val, "Undefined attribute");
	    else
	      strncpy( string_val, hier_name, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Time:
          {
	    /* Convert time to ascii */	
	    sts = time_AtoAscii((pwr_tTime *)object_element, 
                      time_eFormat_DateAndTime, 
                      string_val, sizeof(string_val));
	    string_val[20] = 0;
	    decl = CCM_DECL_STRING;
	    break;
          }
        }

	*return_decl = decl;
	if ( decl == CCM_DECL_INT)
	  *return_int = int_val;
	else if ( decl == CCM_DECL_FLOAT)
	  *return_float = float_val;
	else if ( decl == CCM_DECL_STRING)
	  strcpy( return_string, string_val);

	return DCLI__SUCCESS;
}

int XNav::get_current_object(
			pwr_tObjid	*objid,
			char		*objectname,
			int		size,
			pwr_tBitMask	nametype)
{
  int			sts;
  pwr_sAttrRef		attrref;
  int			is_attr;

  sts = get_select( &attrref, &is_attr);
  if ( ODD(sts))
  {
    *objid = attrref.Objid;
    sts = gdh_ObjidToName ( attrref.Objid, objectname, size, nametype);
  }
  return sts;
}
/*************************************************************************
*
* Name:		show_file()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Displays all files with matching a wilcard specification.
*
**************************************************************************/

int	XNav::show_file(
			char		*filename,
			char		*intitle,
			int		hide_dir)
{
  int		sts;
  char		title[80] = "FILE LIST";
  char		found_file[120];
  char		dev[80], dir[80], file[80], type[80];
  int		version;
  char		file_spec[80];
  char		text[80];
  item_eFileType file_type;

  if ( intitle)
    strcpy( title, intitle);

  strcpy( file_spec, filename);

  sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  if ( EVEN(sts)) return sts;

  dcli_parse_filename( found_file, dev, dir, file, type, &version);
  if ( hide_dir)
  {
    strcpy( text, file);
    cdh_ToLower( text, text);
    text[0] = toupper( text[0]);
  }
  else
    strcpy( text, found_file);
  cdh_ToUpper( type, type);
  if ( strcmp( type, ".RTT_COM") == 0)
    file_type = item_eFileType_Script;
  else if ( strcmp( type, ".PWG") == 0)
    file_type = item_eFileType_Graph;
  else if ( strcmp( type, ".RTT_LOG") == 0)
    file_type = item_eFileType_RttLog;
  else
    file_type = item_eFileType_Unknown;

  brow_pop();
  brow_SetNodraw( brow->ctx);
  new ItemHeader( brow, "Title", title, NULL, flow_eDest_IntoLast);
  new ItemFile( brow, "", text, found_file, file_type, NULL, flow_eDest_IntoLast);

  while ( ODD(sts))
  {
    sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
    if ( ODD(sts))
    {
      dcli_parse_filename( found_file, dev, dir, file, type, &version);
      if ( hide_dir)
      {
        strcpy( text, file);
        cdh_ToLower( text, text);
        text[0] = toupper( text[0]);
      }
      else
        strcpy( text, found_file);
      cdh_ToUpper( type, type);
      if ( strcmp( type, ".RTT_COM") == 0)
        file_type = item_eFileType_Script;
      else if ( strcmp( type, ".PWG") == 0)
        file_type = item_eFileType_Graph;
      else if ( strcmp( type, ".RTT_LOG") == 0)
        file_type = item_eFileType_RttLog;
      else
        file_type = item_eFileType_Unknown;
      new ItemFile( brow, "", text, found_file, file_type, NULL, flow_eDest_IntoLast);
    }
  }
  dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  return XNAV__SUCCESS;
}

void XNav::update_time()
{
	char	timstr[64];

	time_AtoAscii( NULL, time_eFormat_DateAndTime, timstr, sizeof(timstr));
	timstr[strlen(timstr) - 3] = '\0';
	strcpy( gbl.time, timstr);
}

/*************************************************************************
*
* Name:		rtt_show_par_hier_class_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*parametername	I	name of parameter.
* char		*hiername	I	name of hierarchy object.
* char		*classname	I	name of class.
* char		*name		I	name description.
* int		add		I	if added to existing menulist or
*					createing a new menu.
*
* Description:
*	This function is called when a 'show parameter' command is recieved.
*	All object under the hierarchy object, with the specified class
*	that fits in the name description is inserted in a menulist
*	and displayed on the screen.
*
**************************************************************************/

int	XNav::show_par_hier_class_name(
			char		*parametername,
			char		*hiername,
			char		*classname,
			char		*name,
			int		add,
			int		global,
			int		max_objects)
{
	char		parametername_str[80];
	char		name_str[80];
	int		sts;
	pwr_tClassId	classid;
	pwr_tObjid	hierobjid;
	pwr_tObjid	objid;
	char		*s;
	int		single_object = 0;
	char		*t;
	char		elementstr[10];
	int		len;
	int		element;
	char		name_array[2][80];
	int		names;
	ItemObject	*item;

	if ( max_objects == 0)
	  max_objects = 300;

	if ( (parametername == NULL) && (name != NULL))
	{
	  /* Parse the parameter name to get object name and
	   parameter name */
	   names = dcli_parse( name, ".", "",
		(char *) name_array, sizeof( name_array)/sizeof( name_array[0]), 
		sizeof( name_array[0]), 0);
	  if ( names != 2 )
	  {
	    message('E',"Name syntax error");
	    return XNAV__SUCCESS;
	  }
	  strncpy( name_str, name_array[0], sizeof(name_str));
	  strncpy( parametername_str, name_array[1], sizeof(parametername_str));
	  parametername = parametername_str;
	  name = name_str;
	}
	else if (parametername == NULL)
	{
	  message('E', "Enter parameter");
	  return XNAV__HOLDCOMMAND;
	}

	if ( name != NULL)
	{
	  /* Check if name does not include a wildcard */
	  s = strchr( name, '*');
	  if ( s == 0)
	  {
	    /* Get objid for the object */
	    sts = find_name( name, &objid);
	    if ( EVEN(sts))
	    {
     	      message('E',"Object does not exist");
	      return XNAV__HOLDCOMMAND;
	    }
	    single_object = 1;
	  }
	  else
	    /* Convert name to upper case */
	    cdh_ToUpper( name, name);
	}

	/* Check if class */
	if ( classname != NULL )
	{
	  /* Get classid for the class */
	  sts = gdh_ClassNameToId ( classname, &classid);
	  if ( EVEN(sts))
	  {
	    /* Class not found */
	    message('E',"Unknown class");
	    return XNAV__HOLDCOMMAND;
	  }
	}	
	else
	  classid = 0;

	/* Check if hierarchy */
	if ( hiername != NULL )
	{
	  if ( *hiername == '\0')
	  {
	    /* No value is given, take the title as default */
//	    sts = rtt_find_hierarchy( parent_ctx, &hierobjid);
	    sts = 0;
	    if (EVEN(sts)) 
	    {
	      message('E', "No hierarchy found");
	      return XNAV__HOLDCOMMAND;
	    }
	  }
	  else
	  {
	    /* Get objid for the hierarchy object */
	    sts = gdh_NameToObjid ( hiername, &hierobjid);
	    if (EVEN(sts))
	    {
	      message('E',"Hierarchy object not found");
	      return XNAV__HOLDCOMMAND;
	    }
	  }
	}
	else
	  hierobjid = pwr_cNObjid;

	/* Check index in parameter */
	s = strchr( parametername, '[');
	if ( s == 0)
	  element = -1;
	else
	{
	  t = strchr( parametername, ']');
	  if ( t == 0)
	  {
	    message('E',"Syntax error in parameter name");
	    return XNAV__HOLDCOMMAND;
	  }
	  else
	  {
	    len = t - s - 1;
	    strncpy( elementstr, s + 1, len);
	    elementstr[ len] = 0;
	    sscanf( elementstr, "%d", &element);
	    *s = '\0';
	    if ( (element < 0) || (element > 1000) )
	    {
	      message('E',"Syntax error in parameter name");
	      return XNAV__HOLDCOMMAND;
	    }
	  }
	}


	if ( !add == XNAV_MENU_ADD)
	{
          brow_pop();
	}

	try {
	  if ( single_object) {
	    item = new ItemObject( brow, objid, NULL,  flow_eDest_IntoLast, 1);
	    item->open_attribute( brow, 0, 0, parametername, element);
	    delete item;
//	    sts = rtt_show_parameter_add( objid, &menulist, 
//		parametername, &index, &element, 0);
          }
	  else
          {
//	    sts = rtt_get_objects_hier_class_name( parent_ctx, hierobjid, 
//		classid, name, max_objects, global,
//		&rtt_show_parameter_add, (void *) &menulist, 
//		(void *) parametername, (void *) &index, 
//		(void *) &element, 0);
//	    if ( sts == XNAV__MAXCOUNT)
//	      message('E',"To many object, all objects could not be shown");
//	    else if ( EVEN (sts)) return sts;

//	    if ( index)
//	      sts = rtt_menu_upd_bubblesort( menulist);
// 	    else
//	    {
//	      message('E', "No objects found");
//	      return XNAV__HOLDCOMMAND;
//	    }
	  }
	}
	catch ( co_error& e) {
	  brow_push_all();
	  brow_Redraw( brow->ctx, 0);
	  message('E', (char *)e.what().c_str());
	}

	return XNAV__SUCCESS;
}

int	XNav::find_name(
			char		*name,
			pwr_tObjid	*objid)
{
  int	sts;

  sts = gdh_NameToObjid ( name, objid);
  return sts;
}

int XNav::store(	char		*filename,
			int		collect)
{
  char	filename_str[120];
  char	msg[120];
  FILE	*outfile;
  int	first;
  brow_tNode	*node_list;
  int		node_count;
  Item		*item;
  int		i;

  /* Add default extention '.rtt_com' if there is no extention given */
  dcli_get_defaultfilename( filename, filename_str, ".rtt_com");

  /* Open the file */
  outfile = fopen( filename_str, "w");
  if ( outfile == 0)
  {
    message('E', "Unable to open file");
    return XNAV__HOLDCOMMAND;
  }

  /* Loop through the menu item */
  if ( collect)
    brow_GetObjectList( collect_brow->ctx, &node_list, &node_count);
  else
    brow_GetObjectList( brow->ctx, &node_list, &node_count);

  first = 1;
  for ( i = 0; i < node_count; i++)
  {
    brow_GetUserData( node_list[i], (void **)&item);
    if ( first)
    {
      if ( collect)
      {
        fprintf( outfile, "collect clear\n");
        fprintf( outfile, "collect/name=%s\n",
			item->name);
      }
      else
        fprintf( outfile, "show parameter/name=%s\n", 
				item->name);
      first = 0;
    }
    else
    {
      if ( collect)
        fprintf( outfile, "collect/name=%s\n", 
			item->name);
      else
        fprintf( outfile, "add parameter/name=%s\n", 
			item->name);
    }
  }
	
  if ( collect)
    fprintf( outfile, "collect show\n");

  dcli_fgetname( outfile, filename_str, filename_str);
  fclose( outfile);	

  sprintf( msg, "Picture stored %s", filename_str);
  message('I', msg);
  return XNAV__SUCCESS;
}

int XNav::show_symbols()
{
  int sts;
  int i;
  char	key[80];
  char	value[80];
  char	text[160];

  i = 0;
  while( 1)
  {
    sts = dcli_get_symbol_by_index( i, key, value);
    if (EVEN(sts)) break;

    sprintf( text, "%s = \"%s\"", key, value);
    if ( i == 0)
    {
      brow_pop();
      brow_SetNodraw( brow->ctx);
      new ItemHeader( brow, "Title", "Symbols", NULL, flow_eDest_IntoLast);
    }
    new ItemCommand( brow, text, NULL,
		flow_eDest_IntoLast, key, 0, brow->pixmap_symbol);
    i++;
  }
  if ( i == 0)
    message('I', "Symboltable is empty");
  else
  {
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }
  return XNAV__SUCCESS;
}


void xnav_popup_menu_cb( void *xnav, pwr_sAttrRef attrref,
			 unsigned long item_type,
			 unsigned long utility, char *arg,  Widget *popup)
{
  *popup = xnav_create_popup_menu( (XNav *)xnav, attrref, 
				   (xmenu_eItemType) item_type,
				   (xmenu_mUtility) utility,
				   ((XNav *)xnav)->priv, arg);
}

int xnav_call_method_cb( void *xnav, char *method, char *filter,
			 pwr_sAttrRef attrref,
			 unsigned long item_type,
			 unsigned long utility, char *arg)
{
  return xnav_call_method( (XNav *)xnav, method, filter, attrref, 
				   (xmenu_eItemType) item_type,
				   (xmenu_mUtility) utility,
				   ((XNav *)xnav)->priv, arg);
}

void xnav_start_trace_cb( void *xnav, pwr_tObjid objid, char *name)
{
  ((XNav *)xnav)->start_trace( objid, name);
}

static void xnav_ev_help_cb( void *ctx, char *key)
{
  XNav *xnav = (XNav *) ctx;
  int	sts;
  pwr_tObjid objid;
  char objid_str[40];

  sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Project, NULL, 0);
  if ( EVEN(sts)) {
    // Try to convert to objid and search for objid as topic
    sts = gdh_NameToObjid ( key, &objid);
    if ( ODD(sts)) {
      cdh_ObjidToString( objid_str, objid, 1);
      sts = CoXHelp::dhelp( objid_str, "", navh_eHelpFile_Project, NULL, 0);
    }
  }
  if ( EVEN(sts))
    sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Base, NULL, 0);

  if ( EVEN(sts))
    xnav->message( 'E', "Unable to find topic");
  else
    xnav->message( ' ', null_str);
}

static void xnav_ev_display_in_xnav_cb( void *xnav, pwr_tObjid objid)
{
  ((XNav *)xnav)->display_object( objid, 0);
  ((XNav *)xnav)->pop();
}

static void xnav_ev_update_info_cb( void *xnav)
{
  if (((XNav *)xnav)->op)
    ((XNav *)xnav)->op->update_alarm_info();
}

static void xnav_ge_display_in_xnav_cb( void *xnav, pwr_tObjid objid)
{
  ((XNav *)xnav)->display_object( objid, 0);
  ((XNav *)xnav)->pop();
}

static int xnav_ge_is_authorized_cb( void *xnav, unsigned int access)
{
  return (((XNav *)xnav)->priv & access) != 0;
}

static int xnav_ge_get_current_objects_cb( void *vxnav, pwr_sAttrRef **alist,
					   int **is_areflist)
{
  int sts;
  XNav *xnav = (XNav *) vxnav;

    //if ( xnav->brow->ctx == xnav->collect_brow->ctx)
  sts = xnav->get_all_objects( alist, is_areflist);
    //else
    //sts = xnav->get_select_all( alist, is_areflist);
  return sts;
}

void XNav::open_graph( char *name, char *filename, int scrollbar, int menu, 
	int navigator, int width, int height, int x, int y,
	char *object_name, char *focus_name, int input_focus_empty,
	int use_default_access, unsigned int access)
{
  ge_tCtx gectx;

  if ( appl.find( applist_eType_Graph, filename, object_name, 
		  (void **) &gectx)) {
    ge_pop( gectx);
    if ( focus_name)
      ge_set_object_focus( gectx, focus_name, input_focus_empty);
  }
  else
  {
    gectx = ge_new( parent_wid, (void *)this, name, filename, 
	scrollbar, menu, navigator, width, height, x, y, gbl.scantime,
	object_name, use_default_access, access,
	&xnav_ge_close_cb, &xnav_ge_help_cb, &xnav_ge_command_cb,
	&xnav_ge_get_current_objects_cb, &xnav_ge_is_authorized_cb);
    gectx->display_in_xnav_cb = xnav_ge_display_in_xnav_cb;
    gectx->popup_menu_cb = xnav_popup_menu_cb;
    gectx->call_method_cb = xnav_call_method_cb;

    appl.insert( applist_eType_Graph, (void *)gectx, pwr_cNObjid, filename,
		   object_name);
    if ( focus_name)
      ge_set_object_focus( gectx, focus_name, input_focus_empty);

  }
}

void XNav::close_graph( char *filename, char *object_name)
{
  ge_tCtx gectx;

  if ( appl.find( applist_eType_Graph, filename, object_name,
		  (void **) &gectx)) {
    ge_delete( gectx);
  }
}

int XNav::exec_xttgraph( pwr_tObjid xttgraph, char *instance,
        char *focus, int inputempty)
{
  pwr_sClass_XttGraph xttgraph_o;
  char	action[80];
  char	*s;
  int	sts;
  char  instance_str[120];
  char  name[120];

  sts = gdh_ObjidToName( xttgraph, name, 
		sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetObjectInfo( name, (void *) &xttgraph_o, sizeof(xttgraph_o));
  if ( EVEN(sts)) return sts;

  cdh_ToLower( action, xttgraph_o.Action);
  if ( !instance) {
    if ( cdh_ObjidIsNotNull( xttgraph_o.Object)) {
      sts = gdh_ObjidToName( xttgraph_o.Object, 
		instance_str, sizeof( instance_str), cdh_mName_volumeStrict);
      if ( ODD(sts))
        instance = instance_str;
    }
  }

  if ( (s = strstr( action, ".pwg")))
  {
    // Open graph
    *s = 0;
    open_graph( xttgraph_o.Title, action, xttgraph_o.Scrollbar, 
	xttgraph_o.Menu, xttgraph_o.Navigator, xttgraph_o.Width,
	xttgraph_o.Height, xttgraph_o.X, xttgraph_o.Y, instance, 
	focus, inputempty, 0, 0);
  }
  else if ( (strstr( action, ".class")))
  {
    // Open jgraph
    char cmd[80];

    strcpy( action, xttgraph_o.Action);
    strrchr( action, '.');
    *s = 0;

    if ( op)
    {
      strcpy( cmd, "open jgraph ");
      strcat( cmd, s);
        op->jop_command( cmd);
    }
    else
      message('E',"Java process not started");
  }
  else
  {
    // Execute command
    return command( xttgraph_o.Action);
  }
  return XNAV__SUCCESS;
}

static int xnav_op_get_alarm_info_cb( void *xnav, evlist_sAlarmInfo *info)
{
  if ( ((XNav *)xnav)->ev)
    return ((XNav *)xnav)->ev->get_alarm_info( info);
  else
    return 0;
}

static void xnav_op_ack_last_cb( void *xnav, unsigned long type,
	unsigned long prio)
{
  if ( ((XNav *)xnav)->ev)
   ((XNav *)xnav)->ev->ack_last_prio( type, prio);
}

static void xnav_op_command_cb( void *xnav, char *command)
{
  ((XNav *)xnav)->command( command);
}

static void xnav_op_map_cb( void *ctx)
{
  XNav *xnav = (XNav *)ctx;
  if ( xnav->map_cb)
    (xnav->map_cb)( xnav->parent_ctx);
}

static void xnav_op_close_cb( void *ctx)
{
  XNav *xnav = (XNav *)ctx;
  delete xnav->op;
  xnav->op = 0;
  if ( xnav->close_cb)
    (xnav->close_cb)( xnav->parent_ctx);
}

static void xnav_op_help_cb( void *ctx, char *key)
{
  XNav *xnav = (XNav *) ctx;
  int	sts;

  sts = CoXHelp::dhelp( key, "", navh_eHelpFile_Project, NULL, 0);
  if ( EVEN(sts))
    xnav->message( 'E', "Unable to find topic");
  else
    xnav->message( ' ', null_str);
}

int XNav::set_parameter( char *name_str, char *value_str, int bypass)
{
  pwr_sAttrRef	attrref;
  int		sts;
  pwr_tTypeId	attr_type;
  unsigned int	attr_size, attr_offset, attr_dim;
  char		buffer[80];

  if ( !bypass)
  {
    // Check authorization
    if ( !((priv & pwr_mPrv_RtWrite) ||
           (priv & pwr_mPrv_System)))
    {
      message('E', "Not authorized for this operation");
      return 0;
    }
  }

  sts = gdh_NameToAttrref( pwr_cNObjid, name_str, &attrref);
  if (EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharAttrref( &attrref, &attr_type, &attr_size,
	&attr_offset, &attr_dim);
  if ( EVEN(sts)) return sts;

  sts = xnav_attr_string_to_value( attr_type, value_str, 
		buffer, sizeof(buffer), attr_size);
  if ( EVEN(sts)) return sts;

  sts = gdh_SetObjectInfo( name_str, buffer, attr_size);
  if ( EVEN(sts)) return sts;

  return 1;
}

void XNav::open_rttlog( char *name, char *filename)
{
  new GeCurve( this, parent_wid, name, filename, NULL, 0);
}


int XNav::search( char *search_str, int regexp)
{
  int sts;
  pwr_tObjid found_objid;
  char str[200];

  if ( regexp)
  {
    if ( search_compiled)
      regfree( &search_creg);

    sts = regcomp( &search_creg, search_str, REG_NOSUB | REG_ICASE);
    if (sts != 0)
    {
      search_compiled = 0;
      search_type = xnav_eSearchType_No;
      return XNAV__SEARCHCOMPILE;
    }
    search_compiled = 1;
    search_type = xnav_eSearchType_RegularExpr;
  }
  else
    search_type = xnav_eSearchType_Name;

  cdh_ToUpper( str, search_str);
  sts = search_root( str, &found_objid, 0);
  if ( EVEN(sts)) return sts;

  // Store for search next
  search_last = found_objid;
  strcpy( search_string, str);

  sts = display_object( found_objid, 0);
  return sts;
}

int XNav::search_next()
{
  int sts;
  pwr_tObjid found_objid;

  if ( search_type == xnav_eSearchType_No)
    return XNAV__NOSEARCH;

  search_last_found = 0;
  sts = search_root( search_string, &found_objid, 1);
  if ( EVEN(sts)) return sts;

  // Store for search next
  search_last = found_objid;

  sts = display_object( found_objid, 0);
  return sts;
}

int XNav::search_root( char *search_str, pwr_tObjid *found_objid, int next)
{
  pwr_tObjid	root;
  int		sts, search_sts;

  search_sts = 0;

  sts = gdh_GetRootList( &root);
  while ( ODD(sts))
  {
    search_sts = search_object( root, search_str, found_objid, next);
    if ( ODD(search_sts))
      break;
    sts = gdh_GetNextSibling( root, &root);
  }
  return search_sts;
}

int XNav::search_object( pwr_tObjid objid, char *search_str, 
	pwr_tObjid *found_objid, int next)
{
  char	name[80];
  int	sts, search_sts;
  pwr_tObjid child;
  pwr_tClassId classid;

  if ( next && !search_last_found)
  {
    if ( cdh_ObjidIsEqual( objid, search_last))
      search_last_found = 1;
  }
  else
  {
    sts = gdh_ObjidToName( objid, name, sizeof( name), cdh_mName_object);
    if (EVEN(sts)) return sts;

    if ( search_type == xnav_eSearchType_Name)
    {
      cdh_ToUpper( name, name);
      if ( strcmp( name, search_str) == 0)
      {
        *found_objid = objid;
        return XNAV__SUCCESS;
      }
    }
    else
    {
      sts = regexec( &search_creg, name, 0, NULL, 0);
      if ( sts == REG_OK) 
      {
        *found_objid = objid;
        return XNAV__SUCCESS;
      }
    }
  }

  // If mountobject, don't search children
  sts = gdh_GetObjectClass( objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_eClass_MountObject)
    return XNAV__NOMATCH;

  search_sts = XNAV__NOMATCH;
  sts = gdh_GetChild( objid, &child);
  while( ODD(sts))
  {
    search_sts = search_object( child, search_str, found_objid, next);
    if (ODD(search_sts))
      break;
    sts = gdh_GetNextSibling( child, &child);
  }
  return search_sts;  
}


int XNav::create_object(
			char		*classname,
			char		*name)
{
  int		sts;
  pwr_tObjid	objid;
  pwr_tClassId	classid;
	
  /* Check if class */
  if ( classname != NULL )
  {
    /* Get classid for the class */
    sts = gdh_ClassNameToId( classname, &classid);
    if ( EVEN(sts))
    {
      /* Class not found */
      message('E',"Unknown class");
      return XNAV__HOLDCOMMAND;
    }
  }

  sts = gdh_CreateObject( name, classid, 0, &objid, pwr_cNObjid, 0, 
			  pwr_cNObjid);
  if ( EVEN(sts)) 	
  {
    message('E',"Error in object name");
    return XNAV__HOLDCOMMAND;
  }

  message('I',"Object created");
  return XNAV__SUCCESS;
}


int XNav::delete_object(
			char		*name)
{
  int		sts;
  pwr_tObjid	objid;

	
  /* Get objid for the object */
  sts = gdh_NameToObjid ( name, &objid);
  if ( EVEN(sts))
  {
    message('E',"Object does not exist");
    return XNAV__HOLDCOMMAND;
  }

  sts = gdh_DeleteObject( objid);
  if ( EVEN(sts))
  {
    message('E',"Unable to delete object");
    return XNAV__HOLDCOMMAND;
  }

  message('I',"Object deleted");
  return XNAV__SUCCESS;
}



