/* wb_wnav_command.cpp -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines for handling of command line in wnav. */


#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

extern "C" {
#include "pwr_class.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "co_ccm_msg.h"
#include "co_api.h"
#include "co_regex.h"
#include "wb_ldh.h"
#include "wb_ldhload.h"
#include "wb_login.h"
#include "wb_utl.h"
#include "wb_genstruct.h"
#include "wb_lfu.h"
#include "wb_dir.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>
#include <Xm/SelectioB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

extern "C" {
#include "wb_wccm.h"
#include "flow_x.h"
#include "wb_distr.h"
}
#include "wb_wnav.h"
#include "wb_wnav_item.h"
#include "co_dcli_msg.h"
#include "wb_wnav_msg.h"
#include "wb_foe_msg.h"
#include "ge.h"
#include "wb_wda.h"

#include "wb_env.h"
#include "wb_erep.h"
#include "wb_vrepwbl.h"

#define	WNAV_MENU_CREATE	0
#define	WNAV_MENU_ADD		1

static char wtt_version[] = "V3.3b";
static WNav *current_wnav[20];
static int wnav_cnt = 0;

static int wnav_wccm_get_ldhsession_cb( void *ctx, ldh_tWBContext *wbctx);
static int wnav_wccm_get_wbctx_cb( void *ctx, ldh_tSesContext *ldhses);
static char	*wnav_VolumeIdToStr( pwr_tVolumeId volumeid);
static char *wnav_dialog_convert_text( char *text);

static int	wnav_help_func(		void		*client_data,
					void		*client_flag);
static int	wnav_define_func(	void		*client_data,
					void		*client_flag);
static int	wnav_logout_func(	void		*client_data,
					void		*client_flag);
static int	wnav_login_func(	void		*client_data,
					void		*client_flag);
static int	wnav_two_func(		void		*client_data,
					void		*client_flag);
static int	wnav_one_func(		void		*client_data,
					void		*client_flag);
static int	wnav_edit_func(		void		*client_data,
					void		*client_flag);
static int	wnav_noedit_func(	void		*client_data,
					void		*client_flag);
static int	wnav_show_func(	void		*client_data,
				void		*client_flag);
static int	wnav_compile_func(	void		*client_data,
					void		*client_flag);
static int	wnav_print_func(	void		*client_data,
					void		*client_flag);
static int	wnav_redraw_func(	void		*client_data,
					void		*client_flag);
static int	wnav_list_func(		void		*client_data,
					void		*client_flag);
static int	wnav_configure_func(	void		*client_data,
					void		*client_flag);
static int	wnav_sort_func(		void		*client_data,
					void		*client_flag);
static int	wnav_copy_func(		void		*client_data,
					void		*client_flag);
static int	wnav_move_func(		void		*client_data,
					void		*client_flag);
static int	wnav_open_func(	void		*client_data,
				void		*client_flag);
static int	wnav_create_func( void		*client_data,
				void		*client_flag);
static int	wnav_delete_func( void		*client_data,
				void		*client_flag);
static int	wnav_connect_func( void		*client_data,
				void		*client_flag);
static int	wnav_disconnect_func( void	*client_data,
				void		*client_flag);
static int	wnav_wb_func( 	void		*client_data,
				void		*client_flag);
static int	wnav_exit_func(	void		*client_data,
				void		*client_flag);
static int	wnav_setup_func(	void		*client_data,
					void		*client_flag);
static int	wnav_set_func(		void		*client_data,
					void		*client_flag);
static int	wnav_search_func(	void		*client_data,
					void		*client_flag);
static int	wnav_save_func(		void		*client_data,
					void		*client_flag);
static int	wnav_revert_func(	void		*client_data,
					void		*client_flag);
static int	wnav_display_func(	void		*client_data,
					void		*client_flag);
static int	wnav_generate_func(     void		*client_data,
				        void		*client_flag);
static int	wnav_crossref_func(     void		*client_data,
				        void		*client_flag);

dcli_tCmdTable	wnav_command_table[] = {
		{
			"SHOW",
			&wnav_show_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/CLASS", 
			"/HIERARCHY", "/PARAMETER" , "/OBJID", 
			"/FILE", "/LOCAL", "/INITSTEP", 
			"/MAXOBJECTS", "/VOLUME", "/ALL", "/TYPE", 
			"/OPTION", "/TERMINAL", "/OUTPUT", 
			"/FULL", "/APPEND", "/EXACTORDER", 
			"/NODE", "/BUSNUMBER", ""}
		},
		{
			"COMPILE",
			&wnav_compile_func,
			{ "/MODIFIED", "/DEBUG", 
			"/HIERARCHY", "/PLCPGM" , "/WINDOW", 
			"/FROM_PLCPGM", "/ALLVOLUMES", "/VOLUMES", 
			""}
		},
		{
			"PRINT",
			&wnav_print_func,
			{ "/ALL", "/NODOCUMENT", "/NOOVERVIEW", 
			"/HIERARCHY", "/PLCPGM", "/FROM_PLCPGM",
			""}
		},
		{
			"REDRAW",
			&wnav_redraw_func,
			{ "/ALL",
			"/HIERARCHY", "/PLCPGM", "/FROM_PLCPGM",
			""}
		},
		{
			"LIST",
			&wnav_list_func,
			{ "dcli_arg1", "/OUTPUT",
			"/HIERARCHY", "/OBJECT", "/ALLVOLUMES", "/VOLUMES",
			"/DESCRIPTOR", "/SHORTNAME", "/NODE", "/PLCPGM",
			""}
		},
		{
			"CONFIGURE",
			&wnav_configure_func,
			{ "dcli_arg1", "/RACK", "/CARDNAME", "/CARDCLASS",
			"/CHANNELNAME", "/CHANIDENTITY", 
			"/CHANDESCRIPTION", "/TABLE",
			""}
		},
		{
			"SORT",
			&wnav_sort_func,
			{ "/SIGNALS", "/CLASS", "/PARENT",
			""}
		},
		{
			"COPY",
			&wnav_copy_func,
			{ "dcli_arg1", "/FIRST", "/LAST", "/AFTER", "/BEFORE",
			"/HIERARCHY", "/SOURCE", "/DESTINATION", "/NAME",
			"/NODE", "/BUSNUMBER", "/NOCONFIRM", "/VERBOSE", ""}
		},
		{
			"MOVE",
			&wnav_move_func,
			{ "dcli_arg1", "/FIRST", "/LAST", "/AFTER", "/BEFORE",
			"/SOURCE", "/DESTINATION", "/RENAME",
			""}
		},
		{
			"OPEN",
			&wnav_open_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", "/FILE", 
			"/SCROLLBAR", "/WIDTH", "/HEIGHT", "/MENU", 
			"/NAVIGATOR", "/CENTER", "/OBJECT", ""}
		},
		{
			"CREATE",
			&wnav_create_func,
			{ "dcli_arg1", "dcli_arg2", "/TEXT", "/MENU", 
                        "/DESTINATION",
			"/COMMAND", "/AFTER", "/BEFORE", "/FIRSTCHILD", 
			"/LASTCHILD", "/VOLUME", "/ALL", 
			"/CLASS", "/DEBUG", "/NODECONFIG",
			"/NAME", "/IDENTITY", "/FILES", "/OUT",
			""}
		},
		{
			"DELETE",
			&wnav_delete_func,
			{ "dcli_arg1", "/NAME", "/CLASS", "/HIERARCHY",
			"/NOCONFIRM", "/NOLOG", ""}
		},
		{
			"CONNECT",
			&wnav_connect_func,
			{ "/SOURCE", "/DESTINATION", "/RECONNECT",
			""}
		},
		{
			"DISCONNECT",
			&wnav_disconnect_func,
			{ "/SOURCE",
			""}
		},
		{
			"WB",
			&wnav_wb_func,
			{ "dcli_arg1", "/OUTPUT", "/HIERARCHY", "/INDENT",
			"/HEADER", "/VOLUME", "/LOADFILE", "/IGNORE",
			"/FULL", "/ANNOUNCE", "/NOINDEX", "/ROOT"
			""}
		},
		{
			"EXIT",
			&wnav_exit_func,
			{ "",}
		},
		{
			"QUIT",
			&wnav_exit_func,
			{ "",}
		},
		{
			"DEFINE",
			&wnav_define_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4", ""}
		},
		{
			"HELP",
			&wnav_help_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4",
			"/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", 
			"/INDEX", "/BASE", ""}
		},
		{
			"LOGOUT",
			&wnav_logout_func,
			{ ""}
		},
		{
			"LOGIN",
			&wnav_login_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{
			"TWO",
			&wnav_two_func,
			{ ""}
		},
		{
			"ONE",
			&wnav_one_func,
			{ ""}
		},
		{
			"EDIT",
			&wnav_edit_func,
			{ ""}
		},
		{
			"NOEDIT",
			&wnav_noedit_func,
			{ ""}
		},
		{
			"SET",
			&wnav_set_func,
			{ "dcli_arg1", "dcli_arg2", "/LOCAL", 
			"/WIDTH", "/HEIGHT", 
			"/TERMINAL", "/OUTPUT", "/NOCONFIRM", "/LOG",
			"/CLASS", "/HIERARCHY", "/NAME", "/ATTRIBUTE", 
			"/SIGNALOBJECTSEG", "/SIGCHANCONSEG", 
			"/SHOSIGCHANCON", "/SHODETECTTEXT", "/VOLUMENAME",
			"/DBID", "/VALUE", ""}
		},
		{
			"SETUP",
			&wnav_setup_func,
			{"",}
		},
		{
			"SEARCH",
			&wnav_search_func,
			{ "dcli_arg1", "/REGULAREXPRESSION", "/NEXT", ""}
		},
		{
			"SAVE",
			&wnav_save_func,
			{ ""}
		},
		{
			"REVERT",
			&wnav_revert_func,
			{ "/NOCONFIRM", "/CONFIRM", ""}
		},
		{
			"DISPLAY",
			&wnav_display_func,
			{ "dcli_arg1", "/CLASS", "/HIERARCHY", "/ATTRIBUTE", 
			  ""}
		},
		{
			"GENERATE",
			&wnav_generate_func,
			{ "dcli_arg1", ""}
		},
		{
			"CROSSREFERENCE",
			&wnav_crossref_func,
			{ "dcli_arg1", "/NAME", "/FILE", "/STRING", "/BRIEF",
			"/FUNCTION", "/CASE_SENSITIVE", ""}
		},
		{"",}};


static void wnav_store_wnav( WNav *wnav)
{
  if ( wnav_cnt >= 20)
    return;
  current_wnav[wnav_cnt] = wnav;  
  wnav_cnt++;
}

static void wnav_free_stored_wnav()
{
  wnav_cnt--;
}

static void wnav_get_stored_wnav( WNav **wnav)
{
  *wnav = current_wnav[wnav_cnt -1];
}

static int	wnav_help_func(		void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  int	sts;
  char	arg_str[80];
  char	file_str[80];
  char	bookmark_str[80];
  char	key[80];
  int	pop;
	

  if ( ODD( dcli_get_qualifier( "/INDEX", file_str)))
  {
    if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str)))
    {
      sts = wnav->help_index( navh_eHelpFile_Other, file_str);
      if ( EVEN(sts))
        wnav->message('E', "Unable to find file");
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/BASE", file_str)))
        sts = wnav->help_index( navh_eHelpFile_Base, NULL);
      else
        sts = wnav->help_index( navh_eHelpFile_Project, NULL);
    }
    return sts;
  }

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg_str)))
  {
    sts = wnav->help( "help command", "", navh_eHelpFile_Base, NULL);
    return sts;
  }
  pop =  ODD( dcli_get_qualifier( "/POPNAVIGATOR", file_str));
  if ( pop)
    wnav->brow_push_all();

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
  if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str)))
  {
    sts = wnav->help( key, bookmark_str, navh_eHelpFile_Other, file_str);
    if ( EVEN(sts))
      wnav->message('E', "No help on this subject");
  }
  else
  {
    sts = wnav->help( key, bookmark_str, navh_eHelpFile_Base, NULL);
    if ( EVEN(sts))
    {
      sts = wnav->help( key, bookmark_str, navh_eHelpFile_Project, NULL);
      if ( EVEN(sts))
        wnav->message('E', "No help on this subject");
    }
  }
//  if ( pop)
//    wnav->pop();

  return sts;
}

static int	wnav_define_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	arg3_str[80];
  char	arg4_str[80];
  char 	*arg3_ptr;
  char 	*arg4_ptr;
	
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
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
//if (ODD(sts))
//  wnav->message('I',"Symbol defined");
  return sts;
}

static int	wnav_two_func(		void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( wnav->set_twowindows_cb)
    wnav->set_twowindows_cb( wnav->parent_ctx, 1, 0, 0);
  return WNAV__SUCCESS;
}

static int	wnav_one_func(		void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( wnav->set_twowindows_cb)
    wnav->set_twowindows_cb( wnav->parent_ctx, 0, 0, 0);
  return WNAV__SUCCESS;
}

static int	wnav_display_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  char	arg1_str[80];
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }

  if ( strncmp( arg1_str, "W1", strlen( arg1_str)) == 0)
  {
    if ( wnav->set_twowindows_cb)
      wnav->set_twowindows_cb( wnav->parent_ctx, 0, 1, 0);
  }
  else if ( strncmp( arg1_str, "W2", strlen( arg1_str)) == 0)
  {
    if ( wnav->set_twowindows_cb)
      wnav->set_twowindows_cb( wnav->parent_ctx, 0, 0, 1);
  }
  else if ( strncmp( arg1_str, "ATTRIBUTE", strlen( arg1_str)) == 0)
  {
    char classstr[80];
    char hierstr[80];
    char attributestr[80];
    pwr_tClassId classid;
    int sts;
    pwr_tObjid hier_objid;

    if (( ODD( dcli_get_qualifier( "/CLASS" , classstr)))) {
      sts = ldh_ClassNameToId( wnav->ldhses, &classid, classstr);
      if ( EVEN(sts)) {
        wnav->message('E', "Class found");
        return sts;
      }
    }
    else {
      wnav->message('E', "Enter class");
      return WNAV__QUAL;
    }

    if (( ODD( dcli_get_qualifier( "/HIERARCHY" , hierstr)))) {
      sts = ldh_NameToObjid( wnav->ldhses, &hier_objid, hierstr);
      if ( EVEN(sts)) {
        wnav->message('E', "Hierarchy object not found");
        return sts;
      }
    }
    else
      hier_objid = pwr_cNObjid;

    if ( EVEN( dcli_get_qualifier( "/ATTRIBUTE" , attributestr))) {
      wnav->message('E', "Enter attribute");
      return WNAV__QUAL;
    }
    new Wda( wnav->parent_wid, wnav, wnav->ldhses,
	     hier_objid, classid, attributestr, wnav->editmode, 
	     wnav->gbl.advanced_user, 1);
    
  }
  else
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }

  return WNAV__SUCCESS;
}

static int	wnav_edit_func(		void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( wnav->set_configure_cb)
    wnav->set_configure_cb( wnav->parent_ctx, 1);
  return WNAV__SUCCESS;
}

static int	wnav_noedit_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( wnav->set_configure_cb)
    wnav->set_configure_cb( wnav->parent_ctx, 0);
  return WNAV__SUCCESS;
}

static int	wnav_login_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	systemname[80];
  char	systemgroup[80];
  unsigned int	priv;
  char	msg[80];
	
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }
  sts = utl_get_systemname( systemname, systemgroup);
  if ( EVEN(sts)) return sts;

  cdh_ToLower( arg1_str, arg1_str);
  cdh_ToLower( arg2_str, arg2_str);
  sts = user_CheckUser( systemgroup, arg1_str, arg2_str, &priv);
  if ( EVEN(sts))
    wnav->message('E',"Login failure");
  else
  {
    strcpy( wnav->user, arg1_str);
    wnav->priv = priv;
    strcpy( login_prv.username, arg1_str);
    login_prv.priv = (pwr_mPrv) priv;
    sprintf( msg, "User %s logged in", arg1_str);
    wnav->message('I', msg);
  }
  return sts;
}

static int	wnav_logout_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  char	msg[80];
	
  if ( strcmp( wnav->base_user, "") == 0)
  {
    sprintf( msg, "User %s logged out", wnav->user);
    wnav->message('I', msg);
  }
  else
  {
    sprintf( msg, "Returned to user %s", wnav->base_user);
    wnav->message('I', msg);
  }
  strcpy( wnav->user, wnav->base_user);
  wnav->priv = wnav->base_priv;
  strcpy( login_prv.username, wnav->base_user);
  login_prv.priv = (pwr_mPrv) wnav->priv;
  return WNAV__SUCCESS;
}

static int	wnav_set_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  char	arg1_str[80];
  int	arg1_sts;
  char	arg2_str[80];
  int	arg2_sts;
	
  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "ADVANCEDUSER", strlen( arg1_str)) == 0)
  {
    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET ADVANCEDUSER");
    else
      wnav->gbl.advanced_user = 1;
  }
  else if ( strncmp( arg1_str, "NOADVANCEDUSER", strlen( arg1_str)) == 0)
  {
    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOADVANCEDUSER");
    else
      wnav->gbl.advanced_user = 0;
  }
  else if ( strncmp( arg1_str, "ALLTOPLEVEL", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET ALLTOPLEVEL");
    else
    {
      wnav->gbl.all_toplevel = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOALLTOPLEVEL", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOALLTOPLEVEL");
    else
    {
      wnav->gbl.all_toplevel = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWCLASS", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWCLASS");
    else
    {
      wnav->gbl.show_class = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWCLASS", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWCLASS");
    else
    {
      wnav->gbl.show_class = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWALIAS", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWALIAS");
    else
    {
      wnav->gbl.show_alias = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWALIAS", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWALIAS");
    else
    {
      wnav->gbl.show_alias = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWDESCRIPTION", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWDESCRIPTION");
    else
    {
      wnav->gbl.show_descrip = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWDESCRIPTION", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWDESCRIPTION");
    else
    {
      wnav->gbl.show_descrip = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWOBJREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWOBJREF");
    else
    {
      wnav->gbl.show_objref = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWOBJREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWOBJREF");
    else
    {
      wnav->gbl.show_objref = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWOBJXREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWOBJXREF");
    else
    {
      wnav->gbl.show_objxref = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWOBJXREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWOBJXREF");
    else
    {
      wnav->gbl.show_objxref = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWATTRREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOATTRREF");
    else
    {
      wnav->gbl.show_attrref = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWATTRREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWATTRREF");
    else
    {
      wnav->gbl.show_attrref = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "SHOWATTRXREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET SHOWATTRXREF");
    else
    {
      wnav->gbl.show_attrxref = 1;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "NOSHOWATTRXREF", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOSHOWATTRXREF");
    else
    {
      wnav->gbl.show_attrxref = 0;
      wnav->ldh_refresh( pwr_cNObjid);
    }
  }
  else if ( strncmp( arg1_str, "VERIFY", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
    {
      wnav->gbl.verify = 1;
      return WNAV__SUCCESS;
    }

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET VERIFY");
    else
      wnav->gbl.verify = 1;
  }
  else if ( strncmp( arg1_str, "NOVERIFY", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
    {
      wnav->gbl.verify = 0;
      return WNAV__SUCCESS;
    }

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      // Global command to all windows
      (wnav->gbl_command_cb)( wnav->parent_ctx, "SET NOVERIFY");
    else
      wnav->gbl.verify = 0;
  }
  else if ( strncmp( arg1_str, "SYMBOLFILE", strlen( arg1_str)) == 0)
  {
    char filename_str[80];
    char cmd[200];

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", filename_str))) {
      wnav->message('E',"Syntax error");
      return WNAV__SYNTAX;
    }

    if ( wnav->window_type == wnav_eWindowType_No)
    {
      strcpy( wnav->gbl.symbolfilename, filename_str);
      return WNAV__SUCCESS;
    }

    if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str))) {
      // Global command to all windows
      sprintf( cmd, "set symbolfile \"%s\"", filename_str);
      (wnav->gbl_command_cb)( wnav->parent_ctx, cmd);
    }
    else
      strcpy( wnav->gbl.symbolfilename, filename_str);
  }
  else if ( strncmp( arg1_str, "INPUTFOCUS", strlen( arg1_str)) == 0)
  {
    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;
    
    arg2_sts = dcli_get_qualifier( "dcli_arg2", arg2_str);
    if ( strncmp( arg2_str, "W1", strlen( arg2_str)) == 0)
    {
      if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      {
        // Global command to all windows
        (wnav->gbl_command_cb)( wnav->parent_ctx, "SET INPUTFOCUS W1");
      }
      else
      {
        if ( wnav->window_type == wnav_eWindowType_W1)
          if ( wnav->set_focus_cb)
            (wnav->set_focus_cb)( wnav->parent_ctx, wnav);
      }
    }
    else if ( strncmp( arg2_str, "W2", strlen( arg2_str)) == 0)
    {
      if ( EVEN( dcli_get_qualifier( "/LOCAL", arg1_str)))
      {
        // Global command to all windows
        (wnav->gbl_command_cb)( wnav->parent_ctx, "SET INPUTFOCUS W2");
      }
      else
      {
        if ( wnav->window_type == wnav_eWindowType_W2)
          if ( wnav->set_focus_cb)
            (wnav->set_focus_cb)( wnav->parent_ctx, wnav);
      }
    }
  }
  else if ( strncmp( arg1_str, "WINDOW", strlen( arg1_str)) == 0)
  {
    short width = 0;
    short height = 0;
    int	nr;

    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    if ( ODD( dcli_get_qualifier( "/WIDTH", arg2_str)))
    {
      nr = sscanf( arg2_str, "%hd", &width);
      if ( nr != 1)
      {
        wnav->message('E',"Syntax error");
        return WNAV__SYNTAX;
      }
    }

    if ( ODD( dcli_get_qualifier( "/HEIGHT", arg2_str)))
    {
      nr = sscanf( arg2_str, "%hd", &height);
      if ( nr != 1)
      {
        wnav->message('E',"Syntax error");
        return WNAV__SYNTAX;
      }
    }

    if ( wnav->set_window_char_cb)
      (wnav->set_window_char_cb)( wnav->parent_ctx, width, height);
  }
  else if ( strncmp( arg1_str, "ATTRIBUTE", strlen( arg1_str)) == 0)
  {
    char		file[80]; 
    int			termflg;
    char		parameterstr[80];
    char		valuestr[80];
    char		classstr[80];
    char		hierstr[80];
    char		namestr[80];
    char		*classstr_p;
    char		*hierstr_p;
    char		*namestr_p;
    char		*valuestr_p;
    int			sts_class, sts_hier, sts_name;
    pwr_tStatus		sts;
    int			confirm;
    int			log;

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__SYNTAX;
    }

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    termflg = ODD( dcli_get_qualifier( "/TERMINAL", arg2_str));

    if (( EVEN( dcli_get_qualifier( "/OUTPUT", file))))
    {
      strcpy( file, "");
      termflg = 1;
    }

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", arg2_str));
    log = ODD( dcli_get_qualifier( "/LOG", arg2_str));
   
    if (( sts_class = ODD( dcli_get_qualifier( "/CLASS" , classstr))))
      classstr_p = classstr;
    else
      classstr_p = NULL;
    if (( sts_hier = ODD( dcli_get_qualifier( "/HIERARCHY" , hierstr))))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    if (( sts_name = ODD( dcli_get_qualifier( "/NAME" , namestr))))
      namestr_p = namestr;
    else
      namestr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/VALUE" , valuestr)))
      valuestr_p = valuestr;
    else
      valuestr_p = NULL;
    if ( EVEN( dcli_get_qualifier( "/ATTRIBUTE" , parameterstr)))
    {
      wnav->message('E', "Enter attribute");
      return WNAV__QUAL;
    }

    if ( sts_class || sts_hier || sts_name )
    {
      sts = utl_set_object_parameter
        ( wnav->ldhses, hierstr_p , classstr_p ,namestr_p, parameterstr, 
	valuestr_p, termflg , file, confirm, log);
      if ( EVEN(sts))
        wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
    else
    {
      wnav->message('E',"Qualifier required");
      return WNAV__QUAL;
    }
  }
  else if ( strncmp( arg1_str, "TEMPLATE", strlen( arg1_str)) == 0)
  {
    char		str[80] ; 
    int		signalobjectsegments;
    int		sigchanconsegments;
    int		shosigchancon;
    int		shodetecttext;
    pwr_tStatus	sts;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/SIGNALOBJECTSEG" , str)))
    {
      sts = sscanf(str, "%d", &signalobjectsegments);
      if ( sts != 1 || signalobjectsegments < 1)
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else
      signalobjectsegments = 0;
    
    if ( ODD( dcli_get_qualifier( "/SIGCHANCONSEG" , str)))
    {
      sts = sscanf(str, "%d", &sigchanconsegments);
      if ( sts != 1 || sigchanconsegments < 1)
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else
      sigchanconsegments = 0;
    
    if ( ODD( dcli_get_qualifier( "/SHOSIGCHANCON" , str)))
    {
      sts = sscanf(str, "%d", &shosigchancon);
      if ( sts != 1 || !(shosigchancon == 1 || shosigchancon == 0))
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
      shosigchancon += 1;
    }
    else
      shosigchancon = 0;
    
    if ( ODD( dcli_get_qualifier( "/SHODETECTTEXT" , str)))
    {
      sts = sscanf(str, "%d", &shodetecttext);
      if ( sts != 1 || !(shodetecttext == 1 || shodetecttext == 0))
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
      shodetecttext += 1;
    }
    else
      shodetecttext = 0;
    
    sts = utl_set_template( wnav->ldhses, signalobjectsegments, shosigchancon,
		sigchanconsegments, shodetecttext);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "VOLUME", strlen( arg1_str)) == 0)
  {
    pwr_tVolumeId	volid;
    char		volumename[80];
    int			sts;
    ldh_sSessInfo	info;
    int			pop;

    if ( EVEN( dcli_get_qualifier( "/VOLUMENAME", volumename)))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", volumename)))
      {
        wnav->message('E', "Qualifier required");
        return WNAV__QUAL;
      }
    }

    sts = wnav_wccm_get_wbctx_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = ldh_VolumeNameToId( wnav->wbctx, volumename, &volid);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }

    sts = (wnav->attach_volume_cb)( wnav->parent_ctx, volid, 1);
    if ( sts == WNAV__VOLATTACHED)
    {
      if ( wnav->window_type == wnav_eWindowType_No)
        pop = 0;
      else
      {
        if ( wnav->brow_cnt == 1)
          // Keep brow ctx
          pop = 0;
        else
          pop = 1;
      }

      // Detach volume first
      sts = ldh_GetSessionInfo( wnav->ldhses, &info);
      if ( EVEN(sts)) return sts;

      if ( !info.Empty )
      {
        wnav->message('E', "Session is not saved");
        return WNAV__NOTSAVED;
      }
      sts = (wnav->detach_volume_cb)( wnav->parent_ctx);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
      sts = (wnav->attach_volume_cb)( wnav->parent_ctx, volid, pop);
    }
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "DB", strlen( arg1_str)) == 0)
  {
    char		dbid[80] ; 
    pwr_tStatus		sts;

    if ( EVEN( dcli_get_qualifier( "/DBID", dbid)))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", dbid)))
      {
        wnav->message('E', "Qualifier required");
        return WNAV__QUAL;
      }
    }

    if ( wnav->wbctx != 0 )
    {
      wnav->message('E', "Db is already open");
      return WNAV__DBOPEN;
    }

    sts = utl_set_database_id( dbid);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }
  return WNAV__SUCCESS;
}

static int	wnav_show_func(	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  int	sts;
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "VERSION", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW VERSION"
    char	message_str[80];
	 
    strcpy( message_str, "Wtt version ");
    strcat( message_str, wtt_version);
    wnav->message('I', message_str);
    return WNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "SYMBOL", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW SYMBOL" */
    char	arg2_str[80];
    char	message_str[80];
    char  	value[80];
	 
    if ( ODD( dcli_get_qualifier( "/ALL", value)))
    {
      sts = wnav->show_symbols();
      return sts;
    }
    else
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
      {
        sts = wnav->show_symbols();
        return sts;
      }
      sts = dcli_get_symbol( arg2_str, value);
      if (EVEN(sts))
      {
        wnav->message('E', "Symbol not found");
        return sts;
      }
      sprintf( message_str, "Symbol %s = %s", arg2_str, value);
      wnav->message('I', message_str);
      return WNAV__SUCCESS;
   }
  }
  else if ( strncmp( arg1_str, "VOLUMES", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW VOLUMES" */
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( wnav->window_type == wnav_eWindowType_No)
    {
      int all = ODD( dcli_get_qualifier( "/ALL", NULL));

      sts = utl_show_volumes( wnav->ldhses, all);
      if ( EVEN(sts))
        wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
    else
      wnav->show_database();
    return 1;
  }
  else if ( strncmp( arg1_str, "DEFAULT", strlen( arg1_str)) == 0)
  {
    /* Command is "SHOW DEFAULT" */
    char	message_str[80];
	 
    sprintf( message_str, "Default directory: %s", wnav->gbl.default_directory);
    wnav->message('I', message_str);
    return WNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "USER", strlen( arg1_str)) == 0)
  {
    char msg[120];
    char priv_str[80];

    if ( strcmp( wnav->user, "") == 0)
    {
      wnav->message('I', "Not logged in");
    }
    else
    {
      user_DevPrivToString( login_prv.priv, priv_str, sizeof(priv_str));
      sprintf( msg, "User %s (%s)", login_prv.username, priv_str);
      wnav->message('I', msg);
    }
  }
  else if ( strncmp( arg1_str, "CHILDREN", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW CHILDREN" 
    char		name_str[80];
    pwr_tObjid		objid;
    int			size; 
    WItem		*item;

    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = ldh_NameToObjid( wnav->ldhses, &objid, name_str);
      if ( EVEN(sts))
      {
        wnav->message('E', "Object not found");
        return sts;
      }
      sts = ldh_ObjidToName( wnav->ldhses, objid, ldh_eName_Hierarchy,
		name_str, sizeof(name_str), &size);
      if ( EVEN(sts)) return sts;
    }
    else
    {
      // Get the selected object
      sts = wnav->get_current_object( &objid, 
			name_str, sizeof( name_str), ldh_eName_Hierarchy);
      if ( EVEN(sts))
      {
        wnav->message('E', "Select an object or enter name");
        return sts;
      }
    }

    wnav->brow_push_to_volume();
    brow_SetNodraw( wnav->brow->ctx);
    sts = wnav->display_object( objid);
    if ( EVEN(sts)) 
    {
      brow_ResetNodraw( wnav->brow->ctx);
      brow_Redraw( wnav->brow->ctx, 0);
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }

    sts = wnav->find( objid, (void **) &item);
    if ( !sts) 
    {
      brow_ResetNodraw( wnav->brow->ctx);
      brow_Redraw( wnav->brow->ctx, 0);
      wnav->message('E', "Object not found");
      return sts;
    }
    switch( item->type)
    {
      case wnav_eItemType_Object: 
	sts = ((WItemBaseObject *)item)->open_children( wnav, 0, 0);
        break;
      default:
        ;
    }
    brow_ResetNodraw( wnav->brow->ctx);
    brow_Redraw( wnav->brow->ctx, 0);
    brow_CenterObject( wnav->brow->ctx, item->node, 0.0);
    wnav->last_selected = item->node;
    return WNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "OBJECTS", strlen( arg1_str)) == 0)
  {
    char	objid_str[80];
    char	arg2_str[80];
    char	file[80]; 
    int		termflg;
    int		sts;

    // Command is "SHOW OBJECTS" 

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/OBJID", objid_str)))
    {
      termflg = ODD( dcli_get_qualifier( "/TERMINAL", arg2_str));

      if ( EVEN( dcli_get_qualifier( "/OUTPUT", file)))
      {
        strcpy( file, "");
        termflg = 1;
      }

      sts = utl_show_object_objdid( wnav->ldhses, objid_str, termflg, file);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    else
    {
      char		classstr[80] ;
      char		hierstr[80] ;
      char		namestr[80] ;
      char		parameterstr[80] ;
      char		volumestr[80] ;
      char		*classstr_p;
      char		*hierstr_p;
      char		*namestr_p;
      char		*parameterstr_p;
      char		*volumestr_p;
      int		sts_class, sts_hier, sts_name, sts_parameter;
      int		fullflg;
      int		allvolumes;
      int		append;
      int		exactorder;

      termflg = ODD( dcli_get_qualifier( "/TERMINAL", arg2_str));

      if (( EVEN( dcli_get_qualifier( "/OUTPUT", file))))
      {
        strcpy( file, "");
        termflg = 1;
      }

      fullflg = ODD( dcli_get_qualifier( "/FULL", arg2_str));
      allvolumes = ODD( dcli_get_qualifier( "/ALL", arg2_str));
      append = ODD( dcli_get_qualifier( "/APPEND", arg2_str));
      exactorder = ODD( dcli_get_qualifier( "/EXACTORDER", arg2_str));

      if (( sts_class = ODD( dcli_get_qualifier( "/CLASS", classstr))))
        classstr_p = classstr;
      else
        classstr_p = NULL;
      if (( sts_hier = ODD( dcli_get_qualifier( "/HIERARCHY", hierstr))))
        hierstr_p = hierstr;
      else
        hierstr_p = NULL;
      if (( sts_name = ODD( dcli_get_qualifier( "/NAME", namestr))))
        namestr_p = namestr;
      else
        namestr_p = NULL;
      if (( sts_parameter = ODD( dcli_get_qualifier( "/PARAMETER", parameterstr))))
        parameterstr_p = parameterstr;
      else
        parameterstr_p = NULL;
      if ( ODD( dcli_get_qualifier( "/VOLUME", volumestr)))
        volumestr_p = volumestr;
      else
        volumestr_p = NULL;

      if ( sts_parameter && !sts_class)
      {
        wnav->message('E', "Class qualifier required");
        return WNAV__QUAL;
      }

      if ( sts_class || sts_hier || sts_name )
      {
        sts = utl_show_obj_hier_class_name( wnav->ldhses, hierstr_p, 
		classstr_p, namestr_p, parameterstr_p, volumestr_p,
		termflg, file, fullflg, allvolumes, append, exactorder);
        if ( EVEN(sts))
	{
          wnav->message(' ', wnav_get_message(sts));
	  return sts;
	}
      }
      else 
      {
        wnav->message('E', "Qualifier required");
        return WNAV__QUAL;
      }
    }
  }
  else if ( strncmp( arg1_str, "OBJID", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW OBJID"
    char		name_str[80];
    pwr_tObjid		objid;
    char		msg[160];
    int			size; 

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
    {
      sts = ldh_NameToObjid( wnav->ldhses, &objid, name_str);
      if ( EVEN(sts))
      {
        wnav->message('E', "Object not found");
        return sts;
      }
      sts = ldh_ObjidToName( wnav->ldhses, objid, ldh_eName_Hierarchy,
		name_str, sizeof(name_str), &size);
      if ( EVEN(sts)) return sts;
    }
    else
    {
      // Get the selected object
      sts = wnav->get_current_object( &objid, 
			name_str, sizeof( name_str), ldh_eName_Hierarchy);
      if ( EVEN(sts))
      {
        wnav->message('E', "Select an object or enter name");
        return sts;
      }
    }

    sprintf( msg, "Objid %s, Name %s", cdh_ObjidToString( NULL, objid, 0),
			name_str);
    wnav->message('I', msg);
    return WNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "MODULES", strlen( arg1_str)) == 0)
  {
    // command is "show modules"
    char		namestr[80];
    char		hierstr[80];
    char		objidstr[80];
    char                *namestr_p;
    char                *hierstr_p;
    char                *objidstr_p;
    char                file[80];
    int                 termflg;

    termflg = ODD( dcli_get_qualifier( "/TERMINAL", NULL));

    if (( EVEN( dcli_get_qualifier( "/OUTPUT", file))))
    {
      strcpy( file, "");
      termflg = 1;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/NAME", namestr)))
      namestr_p = namestr;
    else
      namestr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/OBJID", objidstr)))
      objidstr_p = objidstr;
    else
      objidstr_p = NULL;

    // allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

    //if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
    //  volumestr_p = volumestr;
    //else
    //  volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_show_modules( wnav->ldhses, objidstr_p, namestr_p, 
		hierstr_p, termflg, file);
    if ( EVEN(sts)) {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else if ( strncmp( arg1_str, "SCRIPT", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW SCRIPT"
    char	arg2_str[80];
    char	title[80];
    char	filename[80];
    char	option_str[80];
    int		hide_dir = 0;

    if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
    {
      hide_dir = 1;
      dcli_get_defaultfilename( "*", filename, ".pwr_com");
      strcpy( title, "WNav scripts");
    }
    else
    {
      dcli_get_defaultfilename( arg2_str, filename, ".pwr_com");
      strcpy( title, "File list");
    }
    if ( ODD( dcli_get_qualifier( "/OPTION", option_str)))
    {
      if ( strstr( option_str, "HIDE_DIR"))
        hide_dir = 1;
      else if ( strstr( option_str, "PROJECT"))
      {
        dcli_translate_filename( filename, "pwrp_login:*.pwr_com");
        sts = wnav->show_file( filename, "Project scripts", 1);
        if ( EVEN(sts))
          wnav->message('E', "No files found");
        return sts;
      }
      else if ( strstr( option_str, "BASE"))
      {
        dcli_translate_filename( filename, "pwr_exe:*.pwr_com");
        sts = wnav->show_file( filename, "Base scripts", 1);
        if ( EVEN(sts))
          wnav->message('E', "No files found");
        return sts;
      }
    }
    sts = wnav->show_file( filename, title, hide_dir);
    if ( EVEN(sts))
    {
      wnav->message('E', "No files found");
      return sts;
    }
    return sts;
  }
  else if ( strncmp( arg1_str, "LOADFILES", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW LOADFILES"
    char	nodestr[80];
    char	busnumberstr[80];
    char        *nodestr_p;
    int         busnumber;
    int         nr;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    if ( ODD( dcli_get_qualifier( "/BUSNUMBER", busnumberstr))) {
      nr = sscanf( busnumberstr, "%d", &busnumber);
      if ( nr != 1) {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;	
      }
    }
    else
      busnumber = 0;

    sts = distr_show_load( nodestr_p, NULL, busnumber, 0);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "APPLICATIONS", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW APPLICATIONS"
    char	nodestr[80];
    char        *nodestr_p;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    sts = distr_show_applications( nodestr_p, 0, distr_eFile_Appl);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "SYSTEM", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW GRAPH"
    char	nodestr[80];
    char        *nodestr_p;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    sts = distr_show_applications( nodestr_p, 0, distr_eFile_System);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW GRAPH"
    char	nodestr[80];
    char        *nodestr_p;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    sts = distr_show_applications( nodestr_p, 0, distr_eFile_Graph);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else
  {
    wnav->message('E', "Unknown qualifier");
    return WNAV__QUAL;
  }
  return WNAV__SUCCESS;
}

static int	wnav_compile_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char		hierstr[120];
  char		*hierstr_p;
  char		plcpgmstr[120];
  char		*plcpgmstr_p;
  char		windowstr[120];
  char		*windowstr_p;
  char		fromstr[120];
  char		*fromstr_p;
  int		sts;
  int		modified;
  int		debug;
  int		allvolumes;
  char		volumestr[80] ;
  char		*volumestr_p;
  int		sts_plcpgm, sts_hier, sts_window, sts_from, sts_volumes;

  if ( !wnav->editmode)
  {
    wnav->message('E', "Not in edit mode");
    return WNAV__NOEDIT;
  }

  modified = ODD( dcli_get_qualifier( "/MODIFIED", NULL));
  debug = ODD( dcli_get_qualifier( "/DEBUG", NULL));

  if (( sts_hier = ODD( dcli_get_qualifier( "/HIERARCHY", hierstr))))
    hierstr_p = hierstr;
  else
    hierstr_p = NULL;
  if (( sts_plcpgm = ODD( dcli_get_qualifier( "/PLCPGM", plcpgmstr))))
    plcpgmstr_p = plcpgmstr;
  else
    plcpgmstr_p = NULL;

  if (( sts_window = ODD( dcli_get_qualifier( "/WINDOW" , windowstr))))
    windowstr_p = windowstr;
  else
    windowstr_p = NULL;
  if (( sts_from = ODD( dcli_get_qualifier( "/FROM_PLCPGM", fromstr))))
    fromstr_p = fromstr;
  else
    fromstr_p = NULL;
 
  allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

  if (( sts_volumes = ODD( dcli_get_qualifier( "/VOLUMES", volumestr))))
    volumestr_p = volumestr;
  else
    volumestr_p = NULL;

  if ( !( sts_plcpgm || sts_hier || sts_window || sts_from || sts_volumes ||
	allvolumes))
  {
    // Compile selected plcpgm or window
    pwr_sAttrRef 	*sel_list;
    int                 *sel_is_attr;
    int			sel_cnt;
    pwr_tClassId	classid;
    int			i;
    int			size;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    if ( EVEN(sts))
    {
      if ( modified) {
        // Compile all modified in this volume
        sts = utl_compile( wnav->ldhses, ldh_SessionToWB(wnav->ldhses), 
		plcpgmstr_p, windowstr_p, hierstr_p, fromstr_p, modified, 
		debug, allvolumes, volumestr_p);
        if ( EVEN(sts)) {
          wnav->message(' ', wnav_get_message(sts));
          return sts;
        }
        return WNAV__SUCCESS;
      }
      else {
        wnav->message( 'E', "Qualifier or selection required");
        return WNAV__QUAL;
      }
    }
    for ( i = 0; i < sel_cnt; i++)
    {
      ldh_GetObjectClass( wnav->ldhses, sel_list[i].Objid, &classid);
      if ( classid == pwr_cClass_plc)
      {
        sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, plcpgmstr, sizeof(plcpgmstr), &size);
        if ( EVEN(sts)) return sts;
        plcpgmstr_p = plcpgmstr;
        windowstr_p = NULL;
        hierstr_p = NULL;
      }
      else if ( classid == pwr_cClass_windowplc ||
                classid == pwr_cClass_windowcond ||
                classid == pwr_cClass_windoworderact ||
                classid == pwr_cClass_windowsubstep)
      {
        sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, windowstr, sizeof(windowstr), &size);
        if ( EVEN(sts)) return sts;
        windowstr_p = windowstr;
        hierstr_p = NULL;
        plcpgmstr_p = NULL;
      }
      else if ( classid == pwr_eClass_MountObject)
        continue;
      else
      {
        sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, hierstr, sizeof(hierstr), &size);
        if ( EVEN(sts)) return sts;
        hierstr_p = hierstr;
        windowstr_p = NULL;
        plcpgmstr_p = NULL;

      }
      sts = utl_compile( wnav->ldhses, ldh_SessionToWB(wnav->ldhses), 
		plcpgmstr_p, windowstr_p, hierstr_p, fromstr_p, modified, 
		debug, allvolumes, volumestr_p);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        free( sel_list);
        return sts;
      }
    }
    free( sel_list);
    free( sel_is_attr);
  }
  else
  {
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_compile( wnav->ldhses, ldh_SessionToWB(wnav->ldhses), 
		plcpgmstr_p, windowstr_p, hierstr_p, fromstr_p, modified, 
		debug, allvolumes, volumestr_p);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  return WNAV__SUCCESS;
}


static int	wnav_print_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char		plcpgm_str[80]; 
  char		hier_str[80]; 
  char		from_str[80];
  char		*fromstr_ptr;
  int		sts;
  int		document;
  int		overview;
  int		all;

  all = ODD( dcli_get_qualifier( "/ALL", NULL));
  document = EVEN( dcli_get_qualifier( "/NODOCUMENT", NULL));
  overview = EVEN( dcli_get_qualifier( "/NOOVERVIEW", NULL));

  sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
  if ( EVEN(sts)) return sts;

  if ( ODD( dcli_get_qualifier( "/PLCPGM", plcpgm_str)))
  {
    if ( wnav->parent_wid == 0)
      utl_create_mainwindow( 0, NULL, &wnav->parent_wid);

    sts = utl_print_plc( wnav->parent_wid, wnav->ldhses,
		ldh_SessionToWB(wnav->ldhses), plcpgm_str, document, overview);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else if ( ODD( dcli_get_qualifier( "/HIERARCHY", hier_str)))
  {
    if ( ODD( dcli_get_qualifier( "/FROM_PLCPGM", from_str)))
      fromstr_ptr = from_str;
    else
      fromstr_ptr = NULL;
   
    if ( wnav->parent_wid == 0)
      utl_create_mainwindow( 0, NULL, &wnav->parent_wid);

    sts = utl_print_plc_hier( wnav->parent_wid, wnav->ldhses, 
	ldh_SessionToWB(wnav->ldhses), hier_str, fromstr_ptr, document, 
	overview, all);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    // Print selected plcpgm objects
    pwr_sAttrRef 	*sel_list;
    int                 *sel_is_attr;
    int			sel_cnt;
    pwr_tClassId	classid;
    int			i;
    int			size;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    if ( EVEN(sts))
    {
      wnav->message( 'E', "Qualifier or selection required");
      return WNAV__QUAL;
    }

    for ( i = 0; i < sel_cnt; i++)
    {
      ldh_GetObjectClass( wnav->ldhses, sel_list[i].Objid, &classid);
      if ( classid == pwr_cClass_plc)
      {
        sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, plcpgm_str, sizeof(plcpgm_str), &size);
        if ( EVEN(sts)) return sts;

        if ( wnav->parent_wid == 0)
          utl_create_mainwindow( 0, NULL, &wnav->parent_wid);

        sts = utl_print_plc( wnav->parent_wid, wnav->ldhses,
		ldh_SessionToWB(wnav->ldhses), plcpgm_str, document, overview);
        if ( EVEN(sts))
        {
          wnav->message(' ', wnav_get_message(sts));
          free( sel_list);
          return sts;
        }
      }
    }
    free( sel_list);
    free( sel_is_attr);
  }

  return WNAV__SUCCESS;
}

static int	wnav_redraw_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char		plcpgm_str[80]; 
  char		hier_str[80]; 
  char		from_str[80];
  char		*fromstr_ptr;
  int		sts;

  if ( !wnav->editmode)
  {
    wnav->message('E', "Not in edit mode");
    return WNAV__NOEDIT;
  }

  if ( ODD( dcli_get_qualifier( "/ALL", NULL)))
  {
   
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/FROM_PLCPGM", from_str)))
      fromstr_ptr = from_str;
    else
      fromstr_ptr = NULL;
   
    if ( wnav->parent_wid == 0)
      utl_create_mainwindow( 0, NULL, &wnav->parent_wid);

    sts = utl_redraw_plc_hier( wnav->parent_wid, wnav->ldhses, 
	ldh_SessionToWB(wnav->ldhses), NULL, fromstr_ptr, 1);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else if ( ODD( dcli_get_qualifier( "/PLCPGM", plcpgm_str)))
  {
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( wnav->parent_wid == 0)
      utl_create_mainwindow( 0, NULL, &wnav->parent_wid);

    sts = utl_redraw_plc( wnav->parent_wid, wnav->ldhses,
		ldh_SessionToWB(wnav->ldhses), plcpgm_str);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else if ( ODD( dcli_get_qualifier( "/HIERARCHY", hier_str)))
  {
   
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( ODD( dcli_get_qualifier( "/FROM_PLCPGM", from_str)))
      fromstr_ptr = from_str;
    else
      fromstr_ptr = NULL;
   
    sts = utl_redraw_plc_hier( wnav->parent_wid, wnav->ldhses, 
	ldh_SessionToWB(wnav->ldhses), hier_str, fromstr_ptr, 0);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    // Redraw selected plcpgm objects
    pwr_sAttrRef 	*sel_list;
    int                 *sel_is_attr;
    int			sel_cnt;
    pwr_tClassId	classid;
    int			i;
    int			size;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    if ( EVEN(sts))
    {
      wnav->message( 'E', "Qualifier or selection required");
      return WNAV__QUAL;
    }

    for ( i = 0; i < sel_cnt; i++)
    {
      ldh_GetObjectClass( wnav->ldhses, sel_list[i].Objid, &classid);
      if ( classid == pwr_cClass_plc)
      {
        sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, plcpgm_str, sizeof(plcpgm_str), &size);
        if ( EVEN(sts)) return sts;

        sts = utl_redraw_plc( wnav->parent_wid, wnav->ldhses,
		ldh_SessionToWB(wnav->ldhses), plcpgm_str);
        if ( EVEN(sts))
        {
          wnav->message(' ', wnav_get_message(sts));
          free( sel_list);
          return sts;
        }
      }
    }
    free( sel_list);
    free( sel_is_attr);
  }

  return WNAV__SUCCESS;
}

static int	wnav_generate_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char		arg1_str[80]; 

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str))) {
    wnav->message( 'E', "Syntax error");
    return WNAV__SYNTAX;
  }

  if ( strncmp( arg1_str, "WEB", strlen( arg1_str)) == 0)
  {
    int sts;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = ge_generate_web( wnav->ldhses);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;    
  }

  return WNAV__SUCCESS;
}

static int	wnav_list_func(	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;
  char	file[80];
  char	hierstr[80];
  char	objectstr[80];
  char	*hierstr_p;
  char	*objectstr_p;
  int	sts;
  int	print;
  char	volumestr[80];
  char	*volumestr_p;
  int	allvolumes;
  pwr_tObjid	objid;
  char		descriptor_str[80];

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "DESCRIPTOR", strlen( arg1_str)) == 0)
  {
    char		descstr[80];

    // command is "LIST DESCRIPTOR"

    if ( ODD( dcli_get_qualifier( "/OUTPUT", file)))
      print = 1;
    else
    {
      strcpy( file, "");
      print = 0;;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/OBJECT", objectstr)))
      objectstr_p = objectstr;
    else
      objectstr_p = NULL;

    allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;


    if ( ODD( dcli_get_qualifier( "/DESCRIPTOR" , descstr)))
    {
      // Add defaultvolume wb to descriptor
      if ( strchr( descstr, ':'))
        strcpy( descriptor_str, descstr);
      else
      {
        strcpy( descriptor_str, "wb:");
        strcat( descriptor_str, descstr);
      }

      sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
      if ( EVEN(sts)) return sts;

      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, objectstr_p, 
		file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
  }
  else if ( strncmp( arg1_str, "SIGNALS", strlen( arg1_str)) == 0)
  {
    int		shortname;

    // command is "LIST SIGNALS"

    if ( ODD( dcli_get_qualifier( "/OUTPUT", file)))
      print = 1;
    else
    {
      strcpy( file, "");
      print = 0;;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    objectstr_p = NULL;

    allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));
    shortname = ODD( dcli_get_qualifier( "/SHORTNAME", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    if ( !shortname)
    {
      // Search for local signal list descriptor
      strcpy(descriptor_str, "wb:Local-Lists-SignalList");
      sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
      if ( ODD(sts))
      {
        sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
        if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
        {
          wnav->message(' ', wnav_get_message(sts));
          return sts;
        }
      }
      if ( EVEN(sts))
      {
        // No local found, take global
        strcpy(descriptor_str, "wb:layout-Lists-SignalList");
        sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
        if ( EVEN(sts))
        {
          wnav->message(' ', wnav_get_message(sts));
          return sts;
        }
      }
    }  
    else
    {
      // Search for local signal list descriptor

      strcpy( descriptor_str, "wb:Local-Lists-SignalListShort");
      sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
      if ( ODD(sts))
      {
        sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, objectstr_p, 
		file, print, allvolumes, volumestr_p, 0);
        if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
        {
          wnav->message(' ', wnav_get_message(sts));
          return sts;
        }
      }
      if ( EVEN(sts))
      {
        // No local found, take global
        strcpy( descriptor_str, "wb:layout-Lists-SignalListShort");
        sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, objectstr_p, 
		file, print, allvolumes, volumestr_p, 0);
        if ( EVEN(sts))
        {
          wnav->message(' ', wnav_get_message(sts));
          return sts;
        }
      }  
    }
  }
  else if ( strncmp( arg1_str, "CHANNELS", strlen( arg1_str)) == 0)
  {

    // command is "LIST CHANNELS"

    if ( ODD( dcli_get_qualifier( "/OUTPUT", file)))
      print = 1;
    else
    {
      strcpy( file, "");
      print = 0;;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/NODE", objectstr)))
      objectstr_p = objectstr;
    else
      objectstr_p = NULL;

    allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    // Search for local signal list descriptor
    strcpy(descriptor_str, "wb:Local-Lists-ChannelList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-ChannelList");
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
  }
  else if ( strncmp( arg1_str, "HIERARCHY", strlen( arg1_str)) == 0)
  {

    // command is "LIST HIERARCHY"

    if ( ODD( dcli_get_qualifier( "/OUTPUT", file)))
      print = 1;
    else
    {
      strcpy( file, "");
      print = 0;;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    objectstr_p = NULL;

    allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    // Search for local signal list descriptor
    strcpy(descriptor_str, "wb:Local-Lists-HierList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-HierList");
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }

    // Write plcpgm list
    strcpy(descriptor_str, "wb:Local-Lists-PlcPgmList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-PlcPgmList");
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
  }
  else if ( strncmp( arg1_str, "PLCPGM", strlen( arg1_str)) == 0)
  {

    // command is "LIST PLCPGM"

    if ( ODD( dcli_get_qualifier( "/OUTPUT", file)))
      print = 1;
    else
    {
      strcpy( file, "");
      print = 0;;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY", hierstr)))
      hierstr_p = hierstr;
    else
      hierstr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/PLCPGM", objectstr)))
      objectstr_p = objectstr;
    else
      objectstr_p = NULL;

    allvolumes = ODD( dcli_get_qualifier( "/ALLVOLUMES", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUMES" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    // Search for local signal list descriptor
    strcpy(descriptor_str, "wb:Local-Lists-PlcModuleList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-PlcModuleList");
      sts = utl_list( wnav->ldhses, descriptor_str, hierstr_p, 
		objectstr_p, file, print, allvolumes, volumestr_p, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
  }
  else if ( strncmp( arg1_str, "RTTLISTS", strlen( arg1_str)) == 0)
  {
    ldh_sVolumeInfo 	info;

    // command is "LIST RTTLISTS"

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    // Get the filename
    sts = ldh_GetVolumeInfo( ldh_SessionToVol(wnav->ldhses), &info);
    if ( EVEN(sts)) return sts;

    sprintf( file, "pwrp_load:rtt_crr_%s.dat", wnav_VolumeIdToStr( info.Volume));

    strcpy( descriptor_str, "wb:Local-Lists-RttSignalList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 1);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-RttSignalList");
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 1);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    } 

    // Create the object crossreferece list

    // Get the filename
    sts = ldh_GetVolumeInfo( ldh_SessionToVol(wnav->ldhses), &info);
    if ( EVEN(sts)) return sts;

    sprintf( file, "pwrp_load:rtt_crro_%s.dat", wnav_VolumeIdToStr( info.Volume));

    strcpy(descriptor_str, "wb:Local-Lists-RttObjectList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-RttObjectList");
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }

    // Create the code crossreference list

    // Get the filename
    sts = ldh_GetVolumeInfo( ldh_SessionToVol(wnav->ldhses), &info);
    if ( EVEN(sts)) return sts;

    sprintf( file, "pwrp_load:rtt_crrc_%s.dat", wnav_VolumeIdToStr( info.Volume));

    strcpy(descriptor_str, "wb:Local-Lists-RttCodeList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }
    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-RttCodeList");
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }  

    // Create the plc list
    sprintf( file, "pwrp_load:rtt_plc_%s.dat", wnav_VolumeIdToStr( info.Volume));

    // Search for local descriptor
    strcpy(descriptor_str, "wb:Local-Lists-RttPlcList");
    sts = ldh_NameToObjid( wnav->ldhses, &objid, descriptor_str);
    if ( ODD(sts))
    {
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts) && ( sts != FOE__LISTOBJECT))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }

    if ( EVEN(sts))
    {
      // No local found, take global
      strcpy(descriptor_str, "wb:layout-Lists-RttPlcList");
      sts = utl_list( wnav->ldhses, descriptor_str, NULL, NULL, file, 0, 0, NULL, 0);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        return sts;
      }
    }  
  }
  else
  {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;
  }
  return WNAV__SUCCESS;
}

static int	wnav_configure_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "CARD", strlen( arg1_str)) == 0)
  {
    char		rack[80]; 
    char		cardname[80];
    char		cardclass[80];
    char		channelname[80];
    char		identitystr[80];
    char		descriptionstr[80];
    char		tablestr[80];
    char		*identitystr_p;
    char		*descriptionstr_p;
    char		*tablestr_p;
    int			sts;

    // command "CONFIGURE CARD" 

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    sts = 1;
    if ( EVEN( dcli_get_qualifier( "/RACK", rack)))
      sts = 0;
    if ( EVEN( dcli_get_qualifier( "/CARDNAME", cardname)))
      sts = 0;
    if ( EVEN( dcli_get_qualifier( "/CARDCLASS", cardclass)))
      sts = 0;
    if ( EVEN( dcli_get_qualifier( "/CHANNELNAME", channelname)))
      sts = 0;
    if ( EVEN(sts))
    {
      wnav->message('E', "Qualifier required");
      return WNAV__QUAL;
    }

    if ( ODD( dcli_get_qualifier( "/CHANIDENTITY", identitystr)))
      identitystr_p = identitystr;
    else
      identitystr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/CHANDESCRIPTION", descriptionstr)))
      descriptionstr_p = descriptionstr;
    else
      descriptionstr_p = NULL;
    if ( ODD( dcli_get_qualifier( "/TABLE", tablestr)))
      tablestr_p = tablestr;
    else
      tablestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_configure_card( wnav->ldhses, rack, cardname, cardclass, 
	channelname, identitystr_p, descriptionstr_p, tablestr_p);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;
  }

  return WNAV__SUCCESS;
}

static int	wnav_sort_func(	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char		parentstr[80];
  int		sts;
  int		sorttype;

  if ( !wnav->editmode)
  {
    wnav->message('E', "Not in edit mode");
    return WNAV__NOEDIT;
  }

  sorttype = 0;
  if ( ODD( dcli_get_qualifier( "/SIGNALS", NULL)))
    sorttype = 2;
  else  if ( ODD( dcli_get_qualifier( "/CLASS", NULL)))
    sorttype = 3;


  if ( ODD( dcli_get_qualifier( "/PARENT", NULL)))
  {
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_sortchildren( wnav->ldhses, parentstr, sorttype);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }  
  else
  {
    // Sort children of selected objects
    pwr_sAttrRef 	*sel_list;
    int                 *sel_is_attr;
    int			sel_cnt;
    int			i;
    int			size;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    if ( EVEN(sts))
    {
      wnav->message( 'E', "Qualifier or selection required");
      return WNAV__QUAL;
    }

    for ( i = 0; i < sel_cnt; i++)
    {
      sts = ldh_ObjidToName( wnav->ldhses, sel_list[i].Objid, 
		ldh_eName_Hierarchy, parentstr, sizeof(parentstr), &size);
      if ( EVEN(sts)) return sts;

      sts = utl_sortchildren( wnav->ldhses, parentstr, sorttype);
      if ( EVEN(sts))
      {
        wnav->message(' ', wnav_get_message(sts));
        free( sel_list);
        return sts;
      }
    }
    free( sel_list);
    free( sel_is_attr);
  }
  return WNAV__SUCCESS;
}

static int	wnav_copy_func(	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    char	sourcestr[80];
    char	destinationstr[80];
    char	namestr[80];
    int		hier;
    int		last;
    int		first;
    int		after;
    int		before;
    pwr_tStatus	sts;

    // command  is "COPY OBJECT"

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    first = ODD( dcli_get_qualifier( "/FIRST", NULL));
    last = ODD( dcli_get_qualifier( "/LAST", NULL));
    after = ODD( dcli_get_qualifier( "/AFTER", NULL));
    before = ODD( dcli_get_qualifier( "/BEFORE", NULL));
    hier = ODD( dcli_get_qualifier( "/HIERARCHY", NULL));

    sts = 1;
    if ( EVEN( dcli_get_qualifier( "/SOURCE", sourcestr)))
      sts = 0;
    if ( EVEN( dcli_get_qualifier( "/DESTINATION", destinationstr)))
      sts = 0;
    if ( EVEN( dcli_get_qualifier( "/NAME", namestr)))
      sts = 0;

    if ( EVEN(sts))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_copy_objects( wnav->ldhses, sourcestr, destinationstr, namestr, 
	hier, first, last, after, before);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else if ( strncmp( arg1_str, "LOADFILES", strlen( arg1_str)) == 0)
  {
    // Command is "COPY LOADFILES"
    char	nodestr[80];
    char	busnumberstr[80];
    char        *nodestr_p;
    int         busnumber;
    int         nr;
    int         confirm;
    int         sts;
    int         verbose;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    if ( ODD( dcli_get_qualifier( "/BUSNUMBER", busnumberstr))) {
      nr = sscanf( busnumberstr, "%d", &busnumber);
      if ( nr != 1) {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;	
      }
    }
    else
      busnumber = 0;

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    verbose = ODD( dcli_get_qualifier( "/VERBOSE", NULL));

    sts = distr_copy_load( nodestr_p, busnumber, 0, confirm, 0, verbose);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
    // Command is "COPY GRAPH"
    char	nodestr[80];
    char        *nodestr_p;
    int         confirm;
    int         sts;
    int         verbose;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    verbose = ODD( dcli_get_qualifier( "/VERBOSE", NULL));

    sts = distr_copy_applications( nodestr_p, confirm, 0, distr_eFile_Graph, verbose);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "APPLICATIONS", strlen( arg1_str)) == 0)
  {
    // Command is "COPY APPLICATIONS"
    char	nodestr[80];
    char        *nodestr_p;
    int         confirm;
    int         sts;
    int         verbose;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    verbose = ODD( dcli_get_qualifier( "/VERBOSE", NULL));

    sts = distr_copy_applications( nodestr_p, confirm, 0, distr_eFile_Appl, verbose);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "SYSTEM", strlen( arg1_str)) == 0)
  {
    // Command is "COPY SYSTEM"
    char	nodestr[80];
    char        *nodestr_p;
    int         confirm;
    int         sts;
    int         verbose;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    verbose = ODD( dcli_get_qualifier( "/VERBOSE", NULL));

    sts = distr_copy_applications( nodestr_p, confirm, 0, distr_eFile_System, verbose);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "ALL", strlen( arg1_str)) == 0)
  {
    // Command is "COPY ALL"
    char	nodestr[80];
    char        *nodestr_p;
    int         confirm;
    int         sts;
    int         verbose;

    if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else {
      if ( ODD( dcli_get_qualifier( "/NODE", nodestr)))
        nodestr_p = nodestr;
      else
        nodestr_p = NULL;
    }

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    verbose = ODD( dcli_get_qualifier( "/VERBOSE", NULL));

    sts = distr_copy_all( nodestr_p, confirm, 0, verbose);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else
  {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;
  }

  return WNAV__SUCCESS;
}

static int	wnav_move_func(	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    char	sourcestr[80];
    char	destinationstr[80];
    char	renamestr[80];
    char	*destinationstr_ptr;
    char	*rename_str_ptr;
    int		last;
    int		first;
    int		after;
    int		before;
    pwr_tStatus	sts;

    // command  is "MOVE OBJECT"

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    first = ODD( dcli_get_qualifier( "/FIRST", NULL));
    last = ODD( dcli_get_qualifier( "/LAST", NULL));
    after = ODD( dcli_get_qualifier( "/AFTER", NULL));
    before = ODD( dcli_get_qualifier( "/BEFORE", NULL));

    sts = 1;
    if ( EVEN( dcli_get_qualifier( "/SOURCE", sourcestr)))
      sts = 0;

    if ( ODD( dcli_get_qualifier( "/DESTINATION", destinationstr)))
      destinationstr_ptr = destinationstr;
    else
      destinationstr_ptr = NULL;    
    if ( ODD( dcli_get_qualifier( "/RENAME", renamestr)))
      rename_str_ptr = renamestr;
    else
      rename_str_ptr = NULL;    

    if ( EVEN(sts))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_move_object( wnav->ldhses, sourcestr, destinationstr_ptr, 
		rename_str_ptr, first, last, after, before);
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;
  }
  return WNAV__SUCCESS;
}

static int	wnav_open_func(	void		*client_data,
				void		*client_flag)
{
  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  return WNAV__SUCCESS;	
}

static int	wnav_create_func( void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

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
    wnav_sMenu *menu_item;

    if ( wnav->window_type == wnav_eWindowType_No)
      return WNAV__CMDMODE;

    // Command is "CREATE ITEM"
    if ( ODD( dcli_get_qualifier( "dcli_arg2", text_str)))
    {
      if ( text_str[0] == '/')
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else if ( EVEN( dcli_get_qualifier( "/TEXT", text_str)))
    {
      wnav->message('E',"Enter text");
      return WNAV__QUAL;
    }
    if ( EVEN( dcli_get_qualifier( "/DESTINATION", destination_str)))
    {
      wnav->message('E',"Enter destination");
      return WNAV__QUAL;
    }
    if ( ODD( dcli_get_qualifier( "/COMMAND", command_str)))
      item_type = wnav_eItemType_Command;
    if ( ODD( dcli_get_qualifier( "/MENU", tmp_str)))
      item_type = wnav_eItemType_Menu;
    if ( ODD( dcli_get_qualifier( "/AFTER", tmp_str)))
      dest_code = wnav_eDestCode_After;
    else if ( ODD( dcli_get_qualifier( "/BEFORE", tmp_str)))
      dest_code = wnav_eDestCode_Before;
    else if ( ODD( dcli_get_qualifier( "/FIRSTCHILD", tmp_str)))
      dest_code = wnav_eDestCode_FirstChild;
    else if ( ODD( dcli_get_qualifier( "/LASTCHILD", tmp_str)))
      dest_code = wnav_eDestCode_LastChild;
    else
      dest_code = wnav_eDestCode_After;

    sts = wnav->menu_tree_insert( text_str, item_type, command_str,
		destination_str, dest_code, &menu_item);
    if ( EVEN(sts))
      wnav->message('E',"Destination not found");

    return sts;
  }
  else if ( strncmp( arg1_str, "STRUCTFILES", strlen( arg1_str)) == 0)
  {
    char	filenamestr[160];
    pwr_tStatus	sts;
    char        cmd[200];

    // Command is "CREATE STRUCTFILES" 

    if ( ODD( dcli_get_qualifier( "/VOLUME", NULL)))
    {
      wnav->message('E',"Qualifier /VOLUME is obsolete");
      return WNAV__QUAL;
    }

    if ( ODD( dcli_get_qualifier( "/FILES", filenamestr)))
      cdh_ToLower( filenamestr, filenamestr);
    else
      // userclasses.wb_load is default filename
      strcpy( filenamestr, "$pwrp_db/userclasses.wb_load");

    // Create structfiles
    sprintf( cmd, "co_convert -sv -d $pwrp_inc/ %s", filenamestr);
    sts = system( cmd);
    if ( sts != 0) {
      wnav->message('E', "Conversion error, see terminal window for more information");
      return WNAV__CO_CONVERT;
    }

    // Create html-files
    sprintf( cmd, "co_convert -wv -d $pwrp_web/ %s", filenamestr);
    sts = system( cmd);
    if ( sts != 0) {
      wnav->message('E', "Conversion error, see terminal window for more information");
      return WNAV__CO_CONVERT;
    }

    // Create xtt help-files
    sprintf( cmd, "co_convert -xv -d $pwrp_inc/ %s", filenamestr);
    sts = system( cmd);
    if ( sts != 0) {
      wnav->message('E', "Conversion error, see terminal window for more information");
      return WNAV__CO_CONVERT;
    }



    return WNAV__SUCCESS;
  }
  else if ( strncmp( arg1_str, "LOADFILES", strlen( arg1_str)) == 0)
  {
    char	volumestr[160];
    char	*volumestr_p;
    pwr_tStatus	sts;
    int		allvolumes;
    int		classvolumes;

    // Command is "CREATE LOADFILES"

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    allvolumes = ODD( dcli_get_qualifier( "/ALL", NULL));
    classvolumes = ODD( dcli_get_qualifier( "/CLASS", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUME" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_create_loadfiles( wnav->ldhses, volumestr_p, allvolumes,
		classvolumes);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "BOOTFILES", strlen( arg1_str)) == 0)
  {
    int		debug;
    char	nodestr[160];
    char	*nodestr_p;
    pwr_tStatus	sts;
    int		allnodes;

    // Command is "CREATE BOOTFILES"

    allnodes = ODD( dcli_get_qualifier( "/ALL", NULL));
    debug = ODD( dcli_get_qualifier( "/DEBUG", NULL));

    if ( ODD( dcli_get_qualifier( "/NODECONFIG" , nodestr)))
      nodestr_p = nodestr;
    else if ( ODD( dcli_get_qualifier( "dcli_arg2", nodestr)))
      nodestr_p = nodestr;
    else
      nodestr_p = NULL;

    sts = lfu_create_bootfiles( nodestr_p, debug, allnodes);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    char	namestr[80];
    char	classtr[80];
    char	destinationstr[80];
    int		last;
    int		first;
    int		after;
    int		before;
    pwr_tStatus	sts;

    // Command is "CREATE OBJECT"
    first = ODD( dcli_get_qualifier( "/FIRST", NULL));
    last = ODD( dcli_get_qualifier( "/LAST", NULL));
    after = ODD( dcli_get_qualifier( "/AFTER", NULL));
    before = ODD( dcli_get_qualifier( "/BEFORE", NULL));

    sts = 1;
    if ( EVEN( dcli_get_qualifier( "/NAME", namestr)))
      sts = 0;

    if ( EVEN( dcli_get_qualifier( "/CLASS", classtr)))
      sts = 0;

    if ( EVEN( dcli_get_qualifier( "/DESTINATION", destinationstr)))
      sts = 0;

    if ( EVEN(sts))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_create_object( wnav->ldhses, namestr, classtr, destinationstr, 
		first, last, after, before);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;

  }
  else if ( strncmp( arg1_str, "VOLUME", strlen( arg1_str)) == 0)
  {
    char	namestr[80];
    char	classtr[80];
    char	identitystr[80];
    pwr_tStatus	sts;

    // Command is "CREATE VOLUME"

    sts = 1;
    if ( EVEN( dcli_get_qualifier( "/NAME", namestr)))
      sts = 0;

    if ( EVEN( dcli_get_qualifier( "/CLASS", classtr)))
      sts = 0;

    if ( EVEN( dcli_get_qualifier( "/IDENTITY", identitystr)))
      sts = 0;

    if ( EVEN(sts))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    sts = wnav_wccm_get_wbctx_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_create_volume ( wnav->wbctx, namestr, classtr, identitystr);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "RTTFILES", strlen( arg1_str)) == 0)
  {
    char cmd[80] = "LIST RTTLISTS";
    sts = wnav->command( cmd);
    if ( EVEN(sts)) return sts;
  }
  else if ( strncmp( arg1_str, "SNAPSHOT", strlen( arg1_str)) == 0)
  {
    char	filestr[80];
    char	outstr[80];
    pwr_tStatus	sts;

    // Command is "CREATE SNAPSHOT"

    if ( EVEN( dcli_get_qualifier( "/FILES", filestr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    if ( EVEN( dcli_get_qualifier( "/OUT", outstr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    sts = wnav_wccm_get_wbctx_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = WNAV__SUCCESS;
    try {
      wb_erep *erep = *(wb_env *)wnav->wbctx;
      wb_vrepwbl *wbl = new wb_vrepwbl(erep);
      wbl->load( filestr);
      printf( "-- Loadfile loaded, snapshot creation started...\n");
      wbl->createSnapshot( outstr);
      delete wbl;
    }
    catch ( wb_error &e) {
      sts = e.sts();
    }
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }

  return WNAV__SUCCESS;	
}

static int	wnav_delete_func( void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

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
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else if ( EVEN( dcli_get_qualifier( "/NAME", name_str)))
    {
      wnav->message('E',"Enter name");
      return WNAV__QUAL;
    }

    sts = wnav->menu_tree_delete( name_str);
    if ( EVEN(sts))
      wnav->message('E',"WItem not found");

    return sts;	
  }
  else if ( strncmp( arg1_str, "OBJECT", strlen( arg1_str)) == 0)
  {
    char	namestr[80];
    char	classtr[80];
    char	hierstr[80];
    char	*namestr_ptr;
    char	*classtr_ptr;
    char	*hierstr_ptr;
    int		confirm;
    int		log;
    pwr_tStatus	sts;

    // Command is "DELETE OBJECT"

   if ( ODD( dcli_get_qualifier( "/NAME" , namestr)))
      namestr_ptr = namestr;
    else
      namestr_ptr = NULL;    
   
    if ( ODD( dcli_get_qualifier( "/CLASS" , classtr)))
      classtr_ptr = classtr;
    else
      classtr_ptr = NULL;    

    if ( ODD( dcli_get_qualifier( "/HIERARCHY" , hierstr)))
      hierstr_ptr = hierstr;
    else
      hierstr_ptr = NULL;    

    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    log = EVEN( dcli_get_qualifier( "/NOLOG", NULL));

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_delete_objects( wnav->ldhses, hierstr_ptr, classtr_ptr, namestr_ptr, 
			confirm, log); 
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "TREE", strlen( arg1_str)) == 0)
  {
    char	namestr[80];
    char	*namestr_ptr;
    int		confirm;
    int		log;
    pwr_tStatus	sts;

    // Command is "DELETE TREE"

    if ( ODD( dcli_get_qualifier( "/NAME" , namestr)))
      namestr_ptr = namestr;
    else
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }
   
    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    log = EVEN( dcli_get_qualifier( "/NOLOG", NULL));

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_delete_tree( wnav->ldhses, namestr_ptr, confirm, log); 
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "VOLUME", strlen( arg1_str)) == 0)
  {
    char	namestr[80];
    char	*namestr_ptr;
    int		confirm;
    int		log;
    pwr_tStatus	sts;

    // Command is "DELETE VOLUME"

    if ( ODD( dcli_get_qualifier( "/NAME" , namestr)))
      namestr_ptr = namestr;
    else
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }
   
    confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));
    log = EVEN( dcli_get_qualifier( "/NOLOG", NULL));

    sts = wnav_wccm_get_wbctx_cb( wnav, &wnav->wbctx);
    if ( EVEN(sts)) return sts;

    sts = utl_delete_volume( wnav->wbctx, namestr_ptr, 
			confirm, log); 
    if ( EVEN(sts))
    {
      wnav->message(' ', wnav_get_message(sts));
      return sts;
    }
  }
  else
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }

  return WNAV__SUCCESS;	
}

static int	wnav_connect_func( void		*client_data,
				void		*client_flag)
{
  WNav 		*wnav = (WNav *)client_data;

  char		source_str[80];
  char		dest_str[80];
  pwr_tStatus	sts;
  int		reconnect;

  // Command is "CONNECT"

  sts = 1;
  if ( EVEN( dcli_get_qualifier( "/SOURCE" , source_str)))
    sts = 0;

  if ( EVEN( dcli_get_qualifier( "/DESTINATION" , dest_str)))
    sts = 0;
  if ( EVEN(sts))
  {
    wnav->message('E', "Qualifer required");
    return WNAV__QUAL;
  }

  reconnect = ODD( dcli_get_qualifier( "/RECONNECT" , NULL));

  sts = utl_connect( wnav->ldhses, source_str, dest_str, reconnect);
  if ( EVEN(sts))
    wnav->message(' ', wnav_get_message(sts));
  return sts;

}

static int	wnav_disconnect_func( void	*client_data,
				void		*client_flag)
{
  WNav 		*wnav = (WNav *)client_data;

  char		source_str[80];
  pwr_tStatus	sts;

  // Command is "DISCONNECT"

  if ( EVEN( dcli_get_qualifier( "/SOURCE" , source_str)))
  {
    wnav->message('E', "Qualifer required");
    return WNAV__QUAL;
  }

  sts = utl_disconnect( wnav->ldhses, source_str);
  if ( EVEN(sts))
    wnav->message(' ', wnav_get_message(sts));
  return sts;
}

static int	wnav_wb_func( 	void		*client_data,
				void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "DUMP", strlen( arg1_str)) == 0)
  {
    char	hierarchystr[80]; 
    char	outputstr[80]; 
    int		hierarchy_flag;
    int		root_flag;
    char	indentstr[80];
    int		indent;
    int		indent_flag;
    int		header;
    pwr_tStatus	sts;
    char	volumestr[160];
    char	*volumestr_p;

    if ( EVEN( dcli_get_qualifier( "/OUTPUT" , outputstr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    if ( ODD( dcli_get_qualifier( "/HIERARCHY" , hierarchystr)))
      hierarchy_flag = 1;
    else
      hierarchy_flag = 0;

    root_flag = 0;

    if ( ODD( dcli_get_qualifier( "/INDENT" , indentstr)))
    { 
      indent_flag = 1;
      sts = sscanf(indentstr, "%d", &indent);
      if ( sts != 1)
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else
      indent_flag = 0;

    header = ODD( dcli_get_qualifier( "/HEADER", NULL));

    if ( ODD( dcli_get_qualifier( "/VOLUME" , volumestr)))
      volumestr_p = volumestr;
    else
      volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);

    sts = utl_wb_dump( wnav->ldhses, indent_flag, header, hierarchy_flag,
	root_flag, indent, hierarchystr, outputstr, volumestr_p);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "EXPORT", strlen( arg1_str)) == 0)
  {
    char	outputstr[80]; 
    int		hierarchy_flag;
    char	hierarchystr[80]; 
    int		root_flag;
    char	indentstr[80];
    int		indent;
    int		indent_flag;
    int		header;
    pwr_tStatus	sts;
    char	*volumestr_p;

    if ( EVEN( dcli_get_qualifier( "/OUTPUT" , outputstr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    hierarchy_flag = 0;

    if ( ODD( dcli_get_qualifier( "/HIERARCHY" , hierarchystr)))
    {
      root_flag = 1;
      hierarchy_flag = 1;
    }    
    else
      root_flag = 0;

    if ( ODD( dcli_get_qualifier( "/INDENT" , indentstr)))
    { 
      indent_flag = 1;
      sts = sscanf(indentstr, "%d", &indent);
      if ( sts != 1) 
      {
        wnav->message('E', "Syntax error");
        return WNAV__SYNTAX;
      }
    }
    else
      indent_flag = 0;

    header = ODD( dcli_get_qualifier( "/HEADER", NULL));

    volumestr_p = NULL;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);

    sts = utl_wb_dump( wnav->ldhses, indent_flag, header, hierarchy_flag,
	root_flag, indent, hierarchystr, outputstr, volumestr_p);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( strncmp( arg1_str, "LOAD", strlen( arg1_str)) == 0)
  {
    char	loadfilestr[80]; 
    char	outputstr[80]; 
    char	*rootstr_ptr; 
    char	*outputstr_ptr; 
    int		ignore_flag;
    int		full_flag;
    int		index_flag;
    int		announce_flag;
    pwr_tStatus	sts, status;
    ldh_sSessInfo	info;

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    if ( EVEN( dcli_get_qualifier( "/LOADFILE" , loadfilestr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    if ( ODD( dcli_get_qualifier( "/OUTPUT" , outputstr)))
      outputstr_ptr = outputstr;
    else
      outputstr_ptr = NULL;

    rootstr_ptr = NULL;

    ignore_flag =  ODD( dcli_get_qualifier( "/IGNORE" , NULL));
    full_flag = ODD( dcli_get_qualifier( "/FULL" , NULL));
    announce_flag = ODD( dcli_get_qualifier( "/ANNOUNCE" , NULL));
    index_flag = EVEN( dcli_get_qualifier( "/NOINDEX" , NULL));

    // The ldh session has to be saved
    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);
    sts = ldh_GetSessionInfo( wnav->ldhses, &info);
    if ( EVEN(sts)) return sts;

    if ( !info.Empty )
    {
      wnav->message('E', "Session is not saved");
      return WNAV__NOTSAVED;
    }

    sts = ldh_SetSession( wnav->ldhses, ldh_eAccess_ReadOnly);
    if ( EVEN(sts)) return sts;

    sts = ldh_wbload( ldh_SessionToWB( wnav->ldhses), wnav->ldhses, 
	ignore_flag, full_flag, rootstr_ptr, loadfilestr,
	outputstr_ptr, announce_flag, !index_flag);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
  
    status = ldh_SetSession( wnav->ldhses, ldh_eAccess_ReadWrite);
    if ( EVEN(status)) return status;

    if ( wnav->window_type != wnav_eWindowType_No)
      wnav->ldh_refresh( pwr_cNObjid);
    if ( EVEN(sts)) return sts;
  }
  else if ( strncmp( arg1_str, "IMPORT", strlen( arg1_str)) == 0)
  {
    char	loadfilestr[80]; 
    char	outputstr[80]; 
    char	rootstr[80]; 
    char	*rootstr_ptr; 
    char	*outputstr_ptr; 
    int		ignore_flag;
    int		full_flag;
    int		announce_flag;
    pwr_tStatus	sts, status;
    ldh_sSessInfo	info;
    int		index_flag;

    if ( !wnav->editmode)
    {
      wnav->message('E', "Not in edit mode");
      return WNAV__NOEDIT;
    }

    if ( EVEN( dcli_get_qualifier( "/LOADFILE" , loadfilestr)))
    {
      wnav->message('E', "Qualifer required");
      return WNAV__QUAL;
    }

    if ( ODD( dcli_get_qualifier( "/OUTPUT" , outputstr)))
      outputstr_ptr = outputstr;
    else
      outputstr_ptr = NULL;

    if ( ODD( dcli_get_qualifier( "/ROOT" , rootstr)))
      rootstr_ptr = rootstr;
    else
      rootstr_ptr = NULL;

    ignore_flag =  ODD( dcli_get_qualifier( "/IGNORE" , NULL));
    full_flag = ODD( dcli_get_qualifier( "/FULL" , NULL));
    announce_flag = ODD( dcli_get_qualifier( "/ANNOUNCE" , NULL));
    index_flag = 1;

    sts = wnav_wccm_get_ldhsession_cb( wnav, &wnav->wbctx);

    // The ldh session has to be saved
    sts = ldh_GetSessionInfo( wnav->ldhses, &info);
    if ( EVEN(sts)) return sts;

    if ( !info.Empty )
    {
      wnav->message('E', "Session is not saved");
      return WNAV__NOTSAVED;
    }

    sts = ldh_SetSession( wnav->ldhses, ldh_eAccess_ReadOnly);
    if ( EVEN(sts)) return sts;

    sts = ldh_wbload( ldh_SessionToWB(wnav->ldhses), wnav->ldhses, ignore_flag,
	full_flag, rootstr_ptr, loadfilestr, outputstr_ptr, announce_flag, 
	!index_flag);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
  
    status = ldh_SetSession( wnav->ldhses, ldh_eAccess_ReadWrite);
    if ( EVEN(status)) return status;

    if ( wnav->window_type != wnav_eWindowType_No)
      wnav->ldh_refresh( pwr_cNObjid);
    if ( EVEN(sts)) return sts;
  }
  else
  {
    wnav->message('E', "Syntax error");
    return WNAV__SYNTAX;
  }
  return 1;
}

static int	wnav_exit_func(	void		*client_data,
				void		*client_flag)
{
  WNav 		*wnav = (WNav *)client_data;

  if ( wnav->close_cb)
    (wnav->close_cb)( wnav->parent_ctx);
  else
#if defined OS_VMS
    exit(1);
#else
    exit(0);
#endif
  return 1;
}

static int	wnav_setup_func(	void		*client_data,
					void		*client_flag)
{
  WNav 		*wnav = (WNav *)client_data;

  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  wnav->setup();
  return 1;
}

static int	wnav_search_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  int	sts;
  char	arg1_str[80];
  int	regexp;
	
  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  if ( ODD( dcli_get_qualifier( "/NEXT", NULL)))
  {
    sts = wnav->search_next();
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else if ( ODD( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    regexp =  ODD( dcli_get_qualifier( "/REGULAREXPRESSION", NULL));

    sts = wnav->search( arg1_str, regexp);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
    return sts;
  }
  else
  {
    wnav->message('E',"Syntax error");
    return WNAV__SYNTAX;
  }
}

static int	wnav_save_func(		void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;

  (wnav->save_cb)( wnav->parent_ctx);
  return 1;
}

static int	wnav_revert_func(	void		*client_data,
					void		*client_flag)
{
  WNav *wnav = (WNav *)client_data;
  int	confirm;

  confirm = EVEN( dcli_get_qualifier( "/NOCONFIRM", NULL));

  (wnav->revert_cb)( wnav->parent_ctx, confirm);
  return 1;
}

static int	wnav_crossref_func(	void		*client_data,
					void		*client_flag)
{
  WNav 		*wnav = (WNav *)client_data;
  int		sts;
  char		file_str[80];
  char		string_str[80];
  char		func_str[80];
  char		*file_ptr;
  char          name_str[80];

  if ( ODD( dcli_get_qualifier( "/FILE", file_str)))
    file_ptr = file_str;
  else
    file_ptr = NULL;

  if ( ODD( dcli_get_qualifier( "/STRING", string_str)))
  {
    int		brief;
    int		case_sens;

    if ( ODD( dcli_get_qualifier( "/FUNCTION", func_str)))
    {
      wnav->message('E', "Syntax error");
      return WNAV__SUCCESS;
    }
    brief = ODD( dcli_get_qualifier( "/BRIEF", name_str));
    case_sens = ODD( dcli_get_qualifier( "/CASE_SENSITIVE", name_str));
    sts = wnav->crr_code( file_ptr, string_str, brief, 0, case_sens);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
  }
  else if ( ODD( dcli_get_qualifier( "/FUNCTION", func_str)))
  {
    int		brief;
    int		case_sens;

    brief = ODD( dcli_get_qualifier( "/BRIEF", name_str));
    case_sens = ODD( dcli_get_qualifier( "/CASE_SENSITIVE", name_str));

    sts = wnav->crr_code( file_ptr, func_str, brief, 1, case_sens);
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
  }
  if ( ODD( dcli_get_qualifier( "/NAME", name_str)))
  {
    pwr_tObjid objid;
    pwr_tClassId classid;
    char name[120];
    int size;

    sts = ldh_NameToObjid( wnav->ldhses, &objid, name_str);
    if ( EVEN(sts)) {
      wnav->message(' ', wnav_get_message(sts));
      return WNAV__SUCCESS;
    }
    sts = ldh_ObjidToName( wnav->ldhses, objid, ldh_eName_Hierarchy, 
	name, sizeof(name), &size);
    if ( EVEN(sts)) return sts;

    printf( "Crossreferences for %s\n\n", name);

    sts = ldh_GetObjectClass( wnav->ldhses, objid, &classid);
    if ( EVEN(sts)) return sts;

    switch ( classid) {
      case pwr_cClass_Di:
      case pwr_cClass_Dv:
      case pwr_cClass_Do:
      case pwr_cClass_Po:
      case pwr_cClass_Av:
      case pwr_cClass_Ai:
      case pwr_cClass_Ao:
        sts = wnav->crr_signal( NULL, name_str, NULL);
        break;
      default:
        sts = wnav->crr_object( NULL, name_str, NULL);
    }
    if ( EVEN(sts))
      wnav->message(' ', wnav_get_message(sts));
  }
  else
  {
    /* Get the selected object */
    brow_tNode	*node_list;
    int		node_count;
    WItem		*item;

    brow_GetSelectedNodes( wnav->brow->ctx, &node_list, &node_count);
    if ( !node_count)
    {
      wnav->message('E', "Enter name or select an object");
      return WNAV__SUCCESS;
    }
    brow_GetUserData( *node_list, (void **)&item);

    switch( item->type)
    {
      case wnav_eItemType_Object: 
  	sts = ((WItemBaseObject *)item)->open_crossref( wnav, 0, 0);
        break;
      default:
        ;
    }
  }
  return sts;	  
}

int WNav::show_database()
{
  int		sts;
  pwr_tVolumeId	volume;
  int		volume_cnt;

  if ( !wbctx)
  {
    sts = (get_wbctx_cb)( parent_ctx, &wbctx);
    if ( EVEN(sts)) return sts;
  }

  // Get the volumelist
  sts = ldh_GetVolumeList( wbctx, &volume);
  volume_cnt = 0;
  while ( ODD(sts) )
  {
    if ( !volume_cnt)
    {
      brow_pop( wnav_eBrowType_Other);
      brow_SetNodraw( brow->ctx);
    }
    new WItemVolume( this, volume, NULL, flow_eDest_IntoLast);
    volume_cnt++;
    sts = ldh_GetNextVolume( wbctx, volume, &volume);
  }

  if ( !volume_cnt)
  {
    message( 'I', "No volumes found in database");
    return 1;
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  return 1;
}

int WNav::show_volume( int pop)
{
  if ( window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  // Create the root list
  if ( pop)
    brow_pop(wnav_eBrowType_Volume);
  brow_SetNodraw( brow->ctx);
  if ( !pop)
    brow_DeleteAll( brow->ctx);

  get_rootlist();

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

  return 1;
}

int WNav::get_rootlist()
{
  pwr_tObjid	root;
  int		sts;
  int		class_cnt;
  pwr_tClassId	valid_class[100];
  int		i;
  pwr_tClassId	classid;
  pal_sMenu     *menu_p;

  if ( gbl.all_toplevel || strcmp( layout, "") == 0)
  {
    // Display all toplevel objects
    sts = ldh_GetRootList( ldhses, &root);
    while ( ODD(sts))
    {
      new WItemObject( this, root, NULL, flow_eDest_IntoLast, 0);
      sts = ldh_GetNextSibling( ldhses, root, &root);
    }
  }
  else
  {
    // Get valid toplevel classes from config file
    if ( !menu) {
      menu = Pal::config_tree_build( ldhses, pal_cPaletteFile,
	  pal_eNameType_TopObjects, layout, NULL); 
      if ( !menu) {
        printf( 
          "** WNav: topobjects entry \"%s\" not found in configuration file\n",
	  layout);
        return 0;
      }
      Pal::config_tree_build( ldhses, pal_cLocalPaletteFile,
	  pal_eNameType_TopObjects, layout, menu);
    }

    class_cnt = 0;
    for ( menu_p = menu->child_list; menu_p; menu_p = menu_p->next) 
    {
      sts = ldh_ClassNameToId( ldhses, &valid_class[class_cnt],
			menu_p->title);
      if ( ODD(sts))
        class_cnt++;
      else
        printf( "** Unknown window root class: %s\n", menu_p->title);
    }
    if ( !class_cnt)
      return sts;

    // Create the root list
    sts = ldh_GetRootList( ldhses, &root);
    while ( ODD(sts))
    {
      sts = ldh_GetObjectClass( ldhses, root, &classid);
      if ( EVEN(sts)) return sts;

      for ( i = 0; i < class_cnt; i++)
      {
        if ( classid == valid_class[i])
        {
          new WItemObject( this, root, NULL, flow_eDest_IntoLast, 0);
          break;
        }
      }
      sts = ldh_GetNextSibling( ldhses, root, &root);
    }
  }
  return 1;
}

int WNav::command( char* input_str)
{
  char		command[160];
  int		sts, sym_sts;
  char		symbol_value[80];
  char		filename[120];

  dcli_toupper( input_str, input_str);
  sts = dcli_replace_symbol( input_str, command, sizeof(command));
  if ( EVEN(sts)) return sts;

  if ( input_str[0] == '@')
  {
    /* Read command file */

    cdh_ToLower( filename, &command[1]);
    sts = readcmdfile( filename);
    if ( sts == DCLI__NOFILE)
    {
      message('E',"Unable to open file");
      return DCLI__SUCCESS;
    }
    else if ( EVEN(sts)) return sts;
    return DCLI__SUCCESS;
  }

  sts = dcli_cli( (dcli_tCmdTable *)&wnav_command_table, command, (void *) this, 0);
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
      sts = dcli_cli( (dcli_tCmdTable *)&wnav_command_table, symbol_value, (void *) this, 0);
    }
    else if ( sym_sts == DCLI__SYMBOL_AMBIG)
      sts = sym_sts;
  }
  if (sts == DCLI__COM_AMBIG) message('E',"Ambiguous command");
  else if (sts == DCLI__COM_NODEF) message('E',"Undefined command");
  else if (sts == DCLI__QUAL_AMBIG) message('E',"Ambiguous qualifier");
  else if (sts == DCLI__QUAL_NODEF) message('E',"Undefined qualifier");
  else if (sts == DCLI__SYMBOL_AMBIG) message('E', "Ambiguous symbol abbrevation");

  return sts;
}

static int wnav_getcurrenttext_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  char text[80];
  brow_tNode	*node_list;
  int		node_count;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);

  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  brow_GetSelectedNodes( wnav->brow->ctx, &node_list, &node_count);
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

static int wnav_getcurrentobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav			*wnav;
  char			name[80];
  int			sts;
  pwr_sAttrRef		*sel_list;
    int                 *sel_is_attr;
  int			sel_cnt;
  int			size;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);

  if ( wnav->window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( ODD(sts))
  {
    sts = ldh_ObjidToName( wnav->ldhses, sel_list[0].Objid, ldh_eName_Hierarchy, 
	name, sizeof(name), &size);
    free( sel_list);
    free( sel_is_attr);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wnav_getcurrentvolume_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav			*wnav;
  char			name[80];
  int			sts;
  int			size;
  ldh_sVolumeInfo	info;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  *return_decl = CCM_DECL_STRING;

  wnav_get_stored_wnav( &wnav);

  if ( !wnav->ldhses)
  {
    strcpy( return_string, "");
    return 1;
  }

  sts = ldh_GetVolumeInfo( ldh_SessionToVol(wnav->ldhses), &info);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    return 1;
  }

  sts = ldh_VolumeIdToName( ldh_SessionToWB(wnav->ldhses),
	info.Volume, name, sizeof(name), &size);
  if ( EVEN(sts))
    strcpy( return_string, "");
  else
    strcpy( return_string, name);
  return 1;
}

static int wnav_messageerror_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);
  wnav->message('E', arg_list->value_string);

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_messageinfo_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);
  wnav->message('I', arg_list->value_string);

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_isw1_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;


  if ( arg_count != 0)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);

  *return_int = ( wnav->window_type == wnav_eWindowType_W1);
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_isw2_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;


  if ( arg_count != 0)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);

  *return_int = ( wnav->window_type == wnav_eWindowType_W2);
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_editmode_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;


  if ( arg_count != 0)
    return CCM__ARGMISM;

  wnav_get_stored_wnav( &wnav);

  *return_int = wnav->editmode;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_setdialogsize_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  ccm_s_arg *arg_p2, *arg_p3, *arg_p4; 

  if ( arg_count != 4)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  arg_p3 = arg_p2->next;
  arg_p4 = arg_p3->next;
  if ( arg_list->value_decl != CCM_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != CCM_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p3->value_decl != CCM_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p4->value_decl != CCM_DECL_INT)
    return CCM__VARTYPE;

  wnav_get_stored_wnav( &wnav);
  wnav->dialog_width = arg_list->value_int;
  wnav->dialog_height = arg_p2->value_int;
  wnav->dialog_x = arg_p3->value_int;
  wnav->dialog_y = arg_p4->value_int;

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_messagedialog_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  ccm_s_arg *arg_p2; 

  if ( arg_count != 2)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;

  wnav_get_stored_wnav( &wnav);

  if ( wnav->window_type == wnav_eWindowType_No)
    wnav->message('I', arg_p2->value_string);
  else
    wnav->message_dialog( arg_list->value_string, arg_p2->value_string);

  *return_int = 1;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_confirmdialog_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  ccm_s_arg *arg_p2, *arg_p3; 
  int sts;
  int cancel;
  char str[80];

  if ( !(arg_count == 2 || arg_count == 3))
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_count == 3)
  {
    // Add cancel button and return if cancel is pressed
    arg_p3 = arg_p2->next;
    if ( arg_p3->value_decl != CCM_DECL_INT)
      return CCM__VARTYPE;

    wnav_get_stored_wnav( &wnav);
    if ( wnav->window_type == wnav_eWindowType_No)
    {
      printf( arg_p2->value_string);
      printf(" (y/n/q) : ");
      scanf( "%s", str);
      if ( strcmp( str, "Y") == 0 || strcmp( str, "y") == 0 ||
           strcmp( str, "") == 0)
      {
        sts = 1;
        cancel = 0;
      }
      else if ( strcmp( str, "Q") == 0 || strcmp( str, "q") == 0)
      {
        sts = 0;
        cancel = 1;
      }
      else
      {
        sts = 0;
        cancel = 0;
      }
    }
    else
      sts = wnav->confirm_dialog( arg_list->value_string, arg_p2->value_string,
		1, &cancel);
    arg_p3->value_int = cancel;
    arg_p3->value_returned = 1;
  }
  else
  {
    wnav_get_stored_wnav( &wnav);
    if ( wnav->window_type == wnav_eWindowType_No)
    {
      printf( arg_p2->value_string);
      printf(" (y/n) : ");
      scanf( "%s", str);
      if ( strcmp( str, "Y") == 0 || strcmp( str, "y") == 0 ||
           strcmp( str, "") == 0)
      {
        sts = 1;
        cancel = 0;
      }
      else
      {
        sts = 0;
        cancel = 0;
      }
    }
    else
      sts = wnav->confirm_dialog( arg_list->value_string, arg_p2->value_string,
		0, &cancel);
  }

  *return_int = sts;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_continuedialog_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  ccm_s_arg *arg_p2; 
  int sts;
  char str[80];

  if ( arg_count != 2)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;

  wnav_get_stored_wnav( &wnav);
  if ( wnav->window_type == wnav_eWindowType_No)
  {
    printf( arg_p2->value_string);
    printf("\nDo you want to continue ? (y/n) : ");
    scanf( "%s", str);
    if ( strcmp( str, "Y") == 0 || strcmp( str, "y") == 0 ||
         strcmp( str, "") == 0)
      sts = 1;
    else
      sts = 0;
  }
  else
    sts = wnav->continue_dialog( arg_list->value_string, arg_p2->value_string);

  *return_int = sts;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wnav_promptdialog_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  WNav *wnav;
  ccm_s_arg *arg_p2, *arg_p3; 
  int sts;
  char *value;
  char str[80];

  if ( arg_count != 3)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  arg_p3 = arg_p2->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p3->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;

  wnav_get_stored_wnav( &wnav);
  if ( wnav->window_type == wnav_eWindowType_No)
  {
    printf( arg_p2->value_string);
    printf( " : ");
    scanf( "%s", str);
    value = str;
    sts = 1;
  }
  else
    sts = wnav->prompt_dialog( arg_list->value_string, arg_p2->value_string,
		&value);

  strncpy( arg_p3->value_string, value, sizeof( arg_p3->value_string));
  arg_p3->value_string[sizeof( arg_p3->value_string)-1] = 0;
  arg_p3->value_returned = 1;

  *return_int = sts;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}


static int wnav_ccm_deffilename_func( char *outfile, char *infile, void *client_data)
{

  dcli_get_defaultfilename( infile, outfile, ".pwr_com");
  dcli_translate_filename( outfile, outfile);
  return 1;
}

static int wnav_ccm_errormessage_func( char *msg, int severity, void *client_data)
{
  WNav *wnav = (WNav *) client_data; 

  if ( EVEN(severity))
    wnav->message( 'I', msg);
  else
    wnav->message( 'E', msg);
  return 1;
}

int	wnav_externcmd_func( char *cmd, void *client_data)
{
  WNav *wnav = (WNav *) client_data; 
  int sts;

  wnav->scriptmode++;
  sts = wnav->command( cmd);
  wnav->scriptmode--;
  return sts;
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

int WNav::readcmdfile( 	char		*incommand)
{
  char		input_str[160];
  int		sts;
  int		appl_sts;

  if ( !ccm_func_registred)
  {
    sts = ccm_register_function( "GetCurrentText", wnav_getcurrenttext_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetCurrentObject", wnav_getcurrentobject_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetCurrentVolume", wnav_getcurrentvolume_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "MessageError", wnav_messageerror_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "MessageInfo", wnav_messageinfo_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "IsW1", wnav_isw1_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "IsW2", wnav_isw2_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "EditMode", wnav_editmode_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "MessageDialog", wnav_messagedialog_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "ConfirmDialog", wnav_confirmdialog_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "ContinueDialog", wnav_continuedialog_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "PromptDialog", wnav_promptdialog_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "SetDialogSize", wnav_setdialogsize_func);
    if ( EVEN(sts)) return sts;

    ccm_func_registred = 1;

    // Register wb standard functions
    wccm_register( wnav_wccm_get_wbctx_cb, wnav_wccm_get_ldhsession_cb);
  }

  strcpy( input_str, incommand);
  dcli_remove_blank( input_str, input_str);
  wnav_store_wnav( this);
  wccm_store_ldhses( this, ldhses);

  // Read and execute the command file
  sts = ccm_file_exec( input_str, wnav_externcmd_func,
		wnav_ccm_deffilename_func, wnav_ccm_errormessage_func, 
		&appl_sts, gbl.verify, 0, NULL, 0, 0, NULL, (void *)this);
  if ( EVEN(sts)) return sts;

  wnav_free_stored_wnav();
  return 1;
}

int wnav_cut_segments (
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

int WNav::get_current_object(
			pwr_tObjid	*objid,
			char		*objectname,
			int		size,
			ldh_eName	nametype)
{
  int			sts;
  pwr_sAttrRef		*sel_list;
  int                   *sel_is_attr;
  int			sel_cnt;
  int			rsize;

  if ( window_type == wnav_eWindowType_No)
    return WNAV__CMDMODE;

  sts = get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( ODD(sts))
  {
    if ( cdh_ObjidIsNull( sel_list[0].Objid))
      return WNAV__NOOBJECT;
    *objid = sel_list[0].Objid;
    sts = ldh_ObjidToName( ldhses, sel_list[0].Objid, nametype,
	objectname, size, &rsize);
    free( sel_list);
    free( sel_is_attr);
  }
  return sts;
}

int	WNav::find_name(
			char		*name,
			pwr_tObjid	*objid)
{
  int	sts;

  sts = ldh_NameToObjid( ldhses, objid, name);
  return sts;
}

int WNav::show_symbols()
{
  int sts;
  int i;
  char	key[80];
  char	value[80];
  char	text[160];

  if ( window_type == wnav_eWindowType_No)
  {
    i = 0;
    while( 1)
    {
      sts = dcli_get_symbol_by_index( i, key, value);
      if (EVEN(sts)) break;

      sprintf( text, "%s = \"%s\"", key, value);
      if ( i == 0)
        printf( "Symbols\n\n");
      printf( "%s\n", text);
      i++;
    }
    if ( i == 0)
      message('I', "Symboltable is empty");
  }
  else
  {
    i = 0;
    while( 1)
    {
      sts = dcli_get_symbol_by_index( i, key, value);
      if (EVEN(sts)) break;

      sprintf( text, "%s = \"%s\"", key, value);
      if ( i == 0)
      {
        brow_pop( wnav_eBrowType_Other);
        brow_SetNodraw( brow->ctx);
        new WItemHeader( this, "Title", "Symbols", NULL, flow_eDest_IntoLast);
      }
      new WItemCommand( this, text, NULL,
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
  }
  return WNAV__SUCCESS;
}

int WNav::search( char *search_str, int regexp)
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
      search_type = wnav_eSearchType_No;
      return WNAV__SEARCHCOMPILE;
    }
    search_compiled = 1;
    search_type = wnav_eSearchType_RegularExpr;
  }
  else
    search_type = wnav_eSearchType_Name;

  cdh_ToUpper( str, search_str);
  sts = search_root( str, &found_objid, 0);
  if ( EVEN(sts)) return sts;

  // Store for search next
  search_last = found_objid;
  strcpy( search_string, str);

  sts = display_object( found_objid);
  return sts;
}

int WNav::search_next()
{
  int sts;
  pwr_tObjid found_objid;

  if ( search_type == wnav_eSearchType_No)
    return WNAV__NOSEARCH;

  search_last_found = 0;
  sts = search_root( search_string, &found_objid, 1);
  if ( EVEN(sts)) return sts;

  // Store for search next
  search_last = found_objid;

  sts = display_object( found_objid);
  return sts;
}

int WNav::search_root( char *search_str, pwr_tObjid *found_objid, int next)
{
  pwr_tObjid	root;
  int		sts, search_sts;
  int		class_cnt;
  pwr_tClassId	valid_class[100];
  int		i;
  pwr_tClassId	classid;
  pal_sMenu     *menu_p;

  search_sts = 0;
  if ( gbl.all_toplevel || strcmp( layout, "") == 0)
  {
    // Display all toplevel objects
    sts = ldh_GetRootList( ldhses, &root);
    while ( ODD(sts))
    {
      search_sts = search_object( root, search_str, found_objid, next);
      if ( ODD(search_sts))
        break;
      sts = ldh_GetNextSibling( ldhses, root, &root);
    }
  }
  else
  {
    if ( !menu)
      return 0;
    class_cnt = 0;
    for ( menu_p = menu->child_list; menu_p; menu_p = menu_p->next) 
    {
      sts = ldh_ClassNameToId( ldhses, &valid_class[class_cnt],
			menu_p->title);
      if ( ODD(sts))
        class_cnt++;
    }
    if ( !class_cnt)
      return sts;

    // Create the root list
    sts = ldh_GetRootList( ldhses, &root);
    while ( ODD(sts))
    {
      sts = ldh_GetObjectClass( ldhses, root, &classid);
      if ( EVEN(sts)) return sts;

      for ( i = 0; i < class_cnt; i++)
      {
        if ( classid == valid_class[i])
        {
          search_sts = search_object( root, search_str, found_objid, next);
          break;
        }
      }
      if ( ODD(search_sts))
        break;
      sts = ldh_GetNextSibling( ldhses, root, &root);
    }
  }
  return search_sts;
}

int WNav::search_object( pwr_tObjid objid, char *search_str, 
	pwr_tObjid *found_objid, int next)
{
  char	name[80];
  int	sts, search_sts;
  pwr_tObjid child;
  int 	size;
  pwr_tClassId classid;

  if ( next && !search_last_found)
  {
    if ( cdh_ObjidIsEqual( objid, search_last))
      search_last_found = 1;
  }
  else
  {
    sts = ldh_ObjidToName( ldhses, objid, ldh_eName_Object,
			name, sizeof(name), &size);
    if (EVEN(sts)) return sts;

    if ( search_type == wnav_eSearchType_Name)
    {
      cdh_ToUpper( name, name);
      if ( strcmp( name, search_str) == 0)
      {
        *found_objid = objid;
        return WNAV__SUCCESS;
      }
    }
    else
    {
      sts = regexec( &search_creg, name, 0, NULL, 0);
      if ( sts == REG_OK) 
      {
        *found_objid = objid;
        return WNAV__SUCCESS;
      }
    }
  }

  // If mountobject, don't search children
  sts = ldh_GetObjectClass( ldhses, objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_eClass_MountObject)
    return WNAV__NOMATCH;

  search_sts = WNAV__NOMATCH;
  sts = ldh_GetChild( ldhses, objid, &child);
  while( ODD(sts))
  {
    search_sts = search_object( child, search_str, found_objid, next);
    if (ODD(search_sts))
      break;
    sts = ldh_GetNextSibling( ldhses, child, &child);
  }
  return search_sts;  
}

static int wnav_wccm_get_ldhsession_cb( void *ctx, ldh_tSesContext *ldhses)
{
  WNav *wnav = (WNav *)ctx;
  int  sts;

  if ( wnav->ldhses)
    *ldhses = wnav->ldhses;
  else
  {
    // Attach first volume
    sts = (wnav->attach_volume_cb)( wnav->parent_ctx, 0, 1);
    if ( EVEN(sts))
      return sts;
    *ldhses = wnav->ldhses;
  }
  return 1;
}

static int wnav_wccm_get_wbctx_cb( void *ctx, ldh_tWBContext *wbctx)
{
  WNav *wnav = (WNav *)ctx;
  int	sts;

  if ( wnav->ldhses)
    *wbctx = ldh_SessionToWB( wnav->ldhses);
  else
  {
    sts = (wnav->get_wbctx_cb)( wnav->parent_ctx, wbctx);
    if ( EVEN(sts)) return sts;
  }
  return 1;
}

char	*wnav_VolumeIdToStr( pwr_tVolumeId volumeid)
{
	static char	str[80];
	unsigned char	volid[4];

	memcpy( &volid, &volumeid, sizeof(volid));
	sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u",
		volid[3], volid[2], volid[1], volid[0]);
	return str;
}

void wnav_message_dialog_ok( Widget w, XtPointer wnav, XtPointer data)
{
  ((WNav *)wnav)->dialog_ok = 1;
}

void wnav_message_dialog_cancel( Widget w, XtPointer wnav, XtPointer data)
{
  ((WNav *)wnav)->dialog_cancel = 1;
}

void wnav_message_dialog_help( Widget w, XtPointer wnav, XtPointer data)
{
  ((WNav *)wnav)->dialog_help = 1;
}

void wnav_message_dialog_read( Widget w, XtPointer client_data, XtPointer data)
{
  WNav *wnav = (WNav *)client_data;
  char *value;

  XmSelectionBoxCallbackStruct *cbs = (XmSelectionBoxCallbackStruct *)data;
  wnav->dialog_ok = 1;
  XmStringGetLtoR( cbs->value, XmSTRING_DEFAULT_CHARSET, &value);
  strncpy( wnav->dialog_value, value, sizeof(wnav->dialog_value));
  XtFree( value);
}

void WNav::message_dialog( char *title, char *text)
{
  Widget 	dialog;
  XmString	text_str;
  XmString	title_str;
  Arg		args[9];
  int		i;
  XEvent 	Event;

  text_str = XmStringCreateLocalized( wnav_dialog_convert_text(text));
  title_str = XmStringCreateLocalized( title);
  i = 0;
  XtSetArg( args[i], XmNmessageString, text_str); i++;
  XtSetArg( args[i], XmNdialogTitle, title_str); i++;
  XtSetArg( args[i], XmNdialogType, XmDIALOG_MESSAGE); i++;
  if ( dialog_width && dialog_height)
  {
    XtSetArg( args[i], XmNwidth, dialog_width); i++;
    XtSetArg( args[i], XmNheight, dialog_height); i++;
    XtSetArg( args[i], XmNx, dialog_x); i++;
    XtSetArg( args[i], XmNy, dialog_y); i++;
  }

  dialog = XmCreateInformationDialog( parent_wid, "Info", args, i);
  XmStringFree( text_str);
  XmStringFree( title_str);

  XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_CANCEL_BUTTON));

  XtAddCallback( dialog, XmNokCallback, wnav_message_dialog_ok, this);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_ok, this);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

  dialog_ok = 0;

  for (;;)
  {
    XtAppNextEvent( XtWidgetToApplicationContext( dialog), &Event);
    XtDispatchEvent( &Event);
    if ( dialog_ok)
      return;
  }
}

int WNav::confirm_dialog( char *title, char *text, int display_cancel,
		int *cancel)
{
  Widget 	dialog;
  XmString	text_str;
  XmString	title_str;
  XmString	no_str;
  XmString	yes_str;
  XmString	cancel_str;
  Arg		args[14];
  int		i;
  XEvent 	Event;

  text_str = XmStringCreateLocalized( wnav_dialog_convert_text(text));
  title_str = XmStringCreateLocalized( title);
  no_str = XmStringCreateLocalized( "No");
  yes_str = XmStringCreateLocalized( "Yes");
  cancel_str = XmStringCreateLocalized( "Cancel");
  i = 0;
  XtSetArg( args[i], XmNmessageString, text_str); i++;
  XtSetArg( args[i], XmNdialogTitle, title_str); i++;
  XtSetArg( args[i], XmNcancelLabelString, no_str); i++;
  XtSetArg( args[i], XmNokLabelString, yes_str); i++;
  XtSetArg( args[i], XmNhelpLabelString, cancel_str); i++;
  XtSetArg( args[i], XmNdialogType, XmDIALOG_WARNING); i++;
  if ( dialog_width && dialog_height)
  {
    XtSetArg( args[i], XmNwidth, dialog_width); i++;
    XtSetArg( args[i], XmNheight, dialog_height); i++;
    XtSetArg( args[i], XmNx, dialog_x); i++;
    XtSetArg( args[i], XmNy, dialog_y); i++;
  }

  dialog = XmCreateInformationDialog( parent_wid, "Info", args, i);
  XmStringFree( text_str);
  XmStringFree( title_str);
  XmStringFree( no_str);
  XmStringFree( yes_str);
  XmStringFree( cancel_str);

  if ( !display_cancel)
    XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON));

  XtAddCallback( dialog, XmNokCallback, wnav_message_dialog_ok, this);
  XtAddCallback( dialog, XmNcancelCallback, wnav_message_dialog_cancel, this);
  XtAddCallback( dialog, XmNhelpCallback, wnav_message_dialog_help, this);

  // Connect the window manager close-button to exit
  if ( !display_cancel)
    flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_cancel, this);
  else
    flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_help, this);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

  dialog_ok = 0;
  dialog_cancel = 0;
  dialog_help = 0;

  for (;;)
  {
    XtAppNextEvent( XtWidgetToApplicationContext( dialog), &Event);
    XtDispatchEvent( &Event);
    if ( dialog_ok)
    {
      if ( display_cancel)
        *cancel = 0;
      return 1;
    }
    if ( dialog_cancel)
    {
      if ( display_cancel)
        *cancel = 0;
      return 0;
    }
    if ( dialog_help)
    {
      *cancel = 1;
      XtDestroyWidget( dialog);
      return 0;
    }
  }
}

int WNav::continue_dialog( char *title, char *text)
{
  Widget 	dialog;
  XmString	text_str;
  XmString	title_str;
  XmString	continue_str;
  XmString	quit_str;
  Arg		args[10];
  int		i;
  XEvent 	Event;

  text_str = XmStringCreateLocalized( wnav_dialog_convert_text(text));
  title_str = XmStringCreateLocalized( title);
  continue_str = XmStringCreateLocalized( "Continue");
  quit_str = XmStringCreateLocalized( "Quit");
  i = 0;
  XtSetArg( args[i], XmNmessageString, text_str); i++;
  XtSetArg( args[i], XmNdialogTitle, title_str); i++;
  XtSetArg( args[i], XmNcancelLabelString, quit_str); i++;
  XtSetArg( args[i], XmNokLabelString, continue_str); i++;
  XtSetArg( args[i], XmNdialogType, XmDIALOG_WARNING); i++;
  if ( dialog_width && dialog_height)
  {
    XtSetArg( args[i], XmNwidth, dialog_width); i++;
    XtSetArg( args[i], XmNheight, dialog_height); i++;
    XtSetArg( args[i], XmNx, dialog_x); i++;
    XtSetArg( args[i], XmNy, dialog_y); i++;
  }

  dialog = XmCreateInformationDialog( parent_wid, "Info", args, i);
  XmStringFree( text_str);
  XmStringFree( title_str);
  XmStringFree( quit_str);
  XmStringFree( continue_str);

  XtUnmanageChild( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON));

  XtAddCallback( dialog, XmNokCallback, wnav_message_dialog_ok, this);
  XtAddCallback( dialog, XmNcancelCallback, wnav_message_dialog_cancel, this);

  flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_cancel, this);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

  dialog_ok = 0;
  dialog_cancel = 0;
  dialog_help = 0;

  for (;;)
  {
    XtAppNextEvent( XtWidgetToApplicationContext( dialog), &Event);
    XtDispatchEvent( &Event);
    if ( dialog_ok)
      return 1;
    if ( dialog_cancel)
      return 0;
  }
}

int WNav::prompt_dialog( char *title, char *text, char **value)
{
  Widget 	dialog;
  XmString	text_str;
  XmString	title_str;
  Arg		args[10];
  int		i;
  XEvent 	Event;

  text_str = XmStringCreateLocalized( wnav_dialog_convert_text(text));
  title_str = XmStringCreateLocalized( title);
  i = 0;
  XtSetArg( args[i], XmNselectionLabelString, text_str); i++;
  XtSetArg( args[i], XmNdialogTitle, title_str); i++;
  if ( dialog_width && dialog_height)
  {
    XtSetArg( args[i], XmNwidth, dialog_width); i++;
    XtSetArg( args[i], XmNheight, dialog_height); i++;
    XtSetArg( args[i], XmNx, dialog_x); i++;
    XtSetArg( args[i], XmNy, dialog_y); i++;
  }
//  XtSetArg( args[i], XmNautoUnmanage, False); i++;

  dialog = XmCreatePromptDialog( parent_wid, "Info", args, i);
  XmStringFree( text_str);
  XmStringFree( title_str);

  XtUnmanageChild( XmSelectionBoxGetChild( dialog, XmDIALOG_HELP_BUTTON));

  XtAddCallback( dialog, XmNokCallback, wnav_message_dialog_read, this);
  XtAddCallback( dialog, XmNcancelCallback, wnav_message_dialog_cancel, this);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( XtParent(dialog),
	(XtCallbackProc)wnav_message_dialog_cancel, this);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

  dialog_ok = 0;
  dialog_cancel = 0;
  dialog_help = 0;

  for (;;)
  {
    XtAppNextEvent( XtWidgetToApplicationContext( dialog), &Event);
    XtDispatchEvent( &Event);
    if ( dialog_ok)
    {
      *value = dialog_value;
      XtDestroyWidget( dialog);
      return 1;
    }
    if ( dialog_cancel)
    {
      strcpy( dialog_value, "");
      *value = dialog_value;
      XtDestroyWidget( dialog);
      return 0;
    }
  }
}

//
// Displays all files with matching a wilcard specification.
//

int	WNav::show_file(
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
  if ( strcmp( type, ".PWR_COM") == 0)
    file_type = item_eFileType_Script;
  else if ( strcmp( type, ".PWG") == 0)
    file_type = item_eFileType_Graph;
  else
    file_type = item_eFileType_Unknown;

  if ( window_type != wnav_eWindowType_No)
  {
    brow_pop( wnav_eBrowType_Other);
    brow_SetNodraw( brow->ctx);
    new WItemHeader( this, "Title", title, NULL, flow_eDest_IntoLast);
    new WItemFile( this, "", text, found_file, file_type, NULL, flow_eDest_IntoLast);
  }
  else
    printf( "%s\n\n",  title);

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
      if ( strcmp( type, ".PWR_COM") == 0)
        file_type = item_eFileType_Script;
      else if ( strcmp( type, ".PWG") == 0)
        file_type = item_eFileType_Graph;
      else
        file_type = item_eFileType_Unknown;
      if ( window_type == wnav_eWindowType_No)
        printf( "%s\n", found_file);
      else
        new WItemFile( this, "", text, found_file, file_type, NULL, flow_eDest_IntoLast);
    }
  }
  dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
  if ( window_type != wnav_eWindowType_No)
  {
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }
  return WNAV__SUCCESS;
}

static char *wnav_dialog_convert_text( char *text)
{
  char *s, *t;
  static char new_text[200];

  for ( s = text, t = new_text; *s; s++, t++)
  {
    if ( *s == '\\' && *(s+1) == 'n')
    {
      *t = 10;
      s++;
    }
    else
     *t = *s;
    if ( t > &new_text[sizeof(new_text)-1])
      break;
  }
  *t = *s;
  return new_text;
}
