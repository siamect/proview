/* 
 * Proview   $Id: wb_admin_command.cpp,v 1.4 2005-09-06 10:43:30 claes Exp $
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

/* wb_admin_command.cpp 
   This module contains routines for handling of command line in admin. */


#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

extern "C" {
#include "pwr_class.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_dcli.h"
#include "co_ccm_msg.h"
#include "co_api.h"
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
#include "flow_x.h"
#include "co_ccm.h"
}
#include "wb_admin.h"
#include "co_dcli_msg.h"

#define ADMIN__SUCCESS 1
#define ADMIN__QUAL 2

static char admin_version[] = "V3.4.0";
static Admin *current_admin;

static int	admin_show_func(void		*client_data,
				void		*client_flag);
static int	admin_exit_func( void		*client_data,
				void		*client_flag);
static int	admin_quit_func( void		*client_data,
				void		*client_flag);
static int	admin_login_func( void		*client_data,
				void		*client_flag);
static int	admin_logout_func( void		*client_data,
				void		*client_flag);

dcli_tCmdTable	admin_command_table[] = {
		{
			"SHOW",
			&admin_show_func,
			{ "dcli_arg1", "dcli_arg2",
			""}
		},
		{
			"EXIT",
			&admin_exit_func,
			{""}
		},
		{
			"QUIT",
			&admin_quit_func,
			{""}
		},
		{
			"LOGOUT",
			&admin_logout_func,
			{ ""}
		},
		{
			"LOGIN",
			&admin_login_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{"",}};

static void admin_store_admin( Admin *admin)
{
  current_admin = admin;  
}

// Not used yet...
#if 0
static void admin_get_stored_admin( Admin **admin)
{
  *admin = current_admin;
}
#endif

static int	admin_login_func(	void		*client_data,
					void		*client_flag)
{
  Admin *admin = (Admin *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	systemgroup[80] = "administrator";
  unsigned int	priv;
  char	msg[80];
	
  sts = user_CheckSystemGroup( systemgroup);
  if ( EVEN(sts))
  {
    // Username and password are not required
    admin->loggedin = 1;
    admin->message('I', "Administrator logged in");
    return 1;
  }

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str)))
  {
    admin->message('E',"Username and password required");
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str)))
  {
    admin->message('E',"Username and password required");
    return 1;
  }

  cdh_ToLower( arg1_str, arg1_str);
  cdh_ToLower( arg2_str, arg2_str);
  sts = user_CheckUser( systemgroup, arg1_str, arg2_str, &priv);
  if ( EVEN(sts))
    admin->message('E',"Login failure");
  else
  {
    admin->loggedin = 1;
    sprintf( msg, "User %s logged in", arg1_str);
    admin->message('I', msg);
  }
  return sts;
}

static int	admin_logout_func(	void		*client_data,
					void		*client_flag)
{
  Admin *admin = (Admin *)client_data;
	
  if ( admin->loggedin)
  {
    admin->message('I', "Administrator logged out");
    admin->loggedin = 0;
  }
  else
    admin->message('I', "Not logged in");
  return 1;
}

static int	admin_show_func(void		*client_data,
				void		*client_flag)
{
  Admin *admin = (Admin *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str);

  if ( strncmp( arg1_str, "VERSION", strlen( arg1_str)) == 0)
  {
    // Command is "SHOW VERSION"
    char	message_str[80];
	 
    strcpy( message_str, "Admin version ");
    strcat( message_str, admin_version);
    admin->message('I', message_str);
  }
  else
  {
    admin->message('E', "Unknown qualifier");
    return ADMIN__QUAL;
  }
  return ADMIN__SUCCESS;
}

static int	admin_quit_func(void		*client_data,
				void		*client_flag)
{
  exit(0);
  return ADMIN__SUCCESS;
}

static int	admin_exit_func(void		*client_data,
				void		*client_flag)
{
  exit(0);
  return ADMIN__SUCCESS;
}

static int admin_ccm_deffilename_func( char *outfile, char *infile, void *client_data)
{

  dcli_get_defaultfilename( infile, outfile, ".adm_com");
  return 1;
}

static int admin_ccm_errormessage_func( char *msg, int severity, void *client_data)
{
  Admin *admin = (Admin *) client_data; 

  if ( EVEN(severity))
    admin->message( 'I', msg);
  else
    admin->message( 'E', msg);
  return 1;
}

int	admin_externcmd_func( char *cmd, void *client_data)
{
  Admin *admin = (Admin *) client_data; 
  int sts;

  admin->scriptmode++;
  sts = admin->command( cmd);
  admin->scriptmode--;
  return sts;
}

int Admin::command( char* input_str)
{
  char		command[160];
  int		sts, sym_sts;
  char		symbol_value[80];

  dcli_toupper( input_str, input_str);
  sts = dcli_replace_symbol( input_str, command, sizeof(command));
  if ( EVEN(sts)) return sts;

  if ( input_str[0] == '@')
  {
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

  sts = dcli_cli( (dcli_tCmdTable *)&admin_command_table, command, (void *) this, 0);
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
      sts = dcli_cli( (dcli_tCmdTable *)&admin_command_table, symbol_value, (void *) this, 0);
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

int Admin::readcmdfile( 	char		*incommand)
{
  char		input_str[160];
  int		sts;
  int		appl_sts;

  if ( !ccm_func_registred)
  {
    ccm_func_registred = 1;
  }

  strcpy( input_str, incommand);
  dcli_remove_blank( input_str, input_str);
  admin_store_admin( this);

  // Read and execute the command file
  sts = ccm_file_exec( input_str, admin_externcmd_func,
		admin_ccm_deffilename_func, admin_ccm_errormessage_func, 
		&appl_sts, verify, 0, NULL, 0, 0, NULL, (void *)this);
  if ( EVEN(sts)) return sts;

  return 1;
}

