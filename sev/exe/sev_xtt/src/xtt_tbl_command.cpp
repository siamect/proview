/* 
 * Proview   $Id: xtt_tbl_command.cpp,v 1.1 2008-09-05 08:38:58 claes Exp $
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
 */

/* xtt_tbl_command.cpp 
   This module contains routines for handling of command line in sevxtt. */


#include "flow_std.h"


# include <vector>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

#include "co_nav_help.h"
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_user.h"
#include "co_ccm_msg.h"
#include "co_api_user.h"
#include "co_syi.h"
#include "co_dbs.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "flow_msg.h"
#include "glow_curvectx.h"
#include "co_lng.h"
#include "co_error.h"
#include "cow_wow.h"
#include "xtt_tbl.h"
#include "xtt_tblnav.h"
#include "xtt_sevhist.h"
#include "co_dcli_msg.h"
#include "cow_xhelp.h"
#include "cow_login.h"


#define XTTTBL__HOLDCOMMAND 21;
#define XTTTBL__SUCCESS 23;


static pwr_tStatus command_sts = 1;
static XttTbl *current_xtttbl;

static int	xtttbl_exit_func(	void		*client_data,
					void		*client_flag);
static int	xtttbl_help_func(      	void		*client_data,
					void		*client_flag);
static int	xtttbl_logout_func(	void		*client_data,
					void		*client_flag);
static int	xtttbl_login_func(	void		*client_data,
					void		*client_flag);
static int	xtttbl_open_func(	void		*client_data,
					void		*client_flag);
static int	xtttbl_show_func(	void		*client_data,
					void		*client_flag);
static int	xtttbl_create_func(	void		*client_data,
					void		*client_flag);

dcli_tCmdTable	xtttbl_command_table[] = {
		{
			"OPEN",
			&xtttbl_open_func,
			{ "dcli_arg1", "dcli_arg2", "/NAME", ""}
		},
		{
			"SHOW",
			&xtttbl_show_func,
			{ "dcli_arg1", ""}
		},
		{
			"CREATE",
			&xtttbl_create_func,
			{ "dcli_arg1", "/NAME", "/COMMAND", ""}
		},
		{
			"EXIT",
			&xtttbl_exit_func,
			{ "",}
		},
		{
			"QUIT",
			&xtttbl_exit_func,
			{ "",}
		},
		{
			"HELP",
			&xtttbl_help_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "dcli_arg4",
			"/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", 
			"/INDEX", "/BASE", "/RETURNCOMMAND", "/WIDTH",
			"/HEIGHT", "/VERSION", ""}
		},
		{
			"LOGOUT",
			&xtttbl_logout_func,
			{ ""}
		},
		{
			"LOGIN",
			&xtttbl_login_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{"",}};


static void xtttbl_store_xtttbl( XttTbl *xtttbl)
{
  current_xtttbl = xtttbl;  
}

#if 0
static void xtttbl_get_stored_xtttbl( XttTbl **xtttbl)
{
  *xtttbl = current_xtttbl;  
}
#endif

static int	xtttbl_help_func(		void		*client_data,
						void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;
  int	sts;
  char	arg_str[80];
  char	file_str[80];
  char	bookmark_str[80];
  char	key[80];
  char  return_str[80];
  int	pop;
  int   width, height;
  int   nr;	

  if ( ODD( dcli_get_qualifier( "/INDEX", file_str, sizeof(file_str))))
  {
    if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str, sizeof(file_str))))
    {
      sts = CoXHelp::dhelp_index( navh_eHelpFile_Other, file_str);
      if ( EVEN(sts))
        xtttbl->message('E', "Unable to find file");
    }
    else
    {
      if ( ODD( dcli_get_qualifier( "/BASE", 0, 0)))
        sts = CoXHelp::dhelp_index( navh_eHelpFile_Base, NULL);
      else
        sts = CoXHelp::dhelp_index( navh_eHelpFile_Project, NULL);
    }
    return 1;
  }

  if ( ODD( dcli_get_qualifier( "/VERSION", 0, 0))) {
    sts = CoXHelp::dhelp( "version", "", navh_eHelpFile_Other, "$pwr_exe/xtt_version_help.dat", 0);
    if ( EVEN(sts))
      xtttbl->message('E', "No help on this subject");
    return sts;
  }

  int strict = 0;
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg_str, sizeof(arg_str))))
  {
    sts = CoXHelp::dhelp( "help command", "", navh_eHelpFile_Base, NULL, strict);
    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "/BOOKMARK", bookmark_str, sizeof(bookmark_str))))
    strcpy( bookmark_str, "");

  strcpy( key, arg_str);
  if ( ODD( dcli_get_qualifier( "dcli_arg2", arg_str, sizeof(arg_str))))
  {
    strcat( key, " ");
    strcat( key, arg_str);
    if ( ODD( dcli_get_qualifier( "dcli_arg3", arg_str, sizeof(arg_str))))
    {
      strcat( key, " ");
      strcat( key, arg_str);
      if ( ODD( dcli_get_qualifier( "dcli_arg3", arg_str, sizeof(arg_str))))
      {
        strcat( key, " ");
        strcat( key, arg_str);
        if ( ODD( dcli_get_qualifier( "dcli_arg4", arg_str, sizeof(arg_str))))
        {
          strcat( key, " ");
          strcat( key, arg_str);
        }
      }
    }
  }
  if ( ! ODD( dcli_get_qualifier( "/RETURNCOMMAND", return_str, sizeof(return_str))))
    strcpy( return_str, "");

  if ( ODD( dcli_get_qualifier( "/WIDTH", arg_str, sizeof(arg_str))))
  {
    // convert to integer
    nr = sscanf( arg_str, "%d", &width);
    if ( nr != 1)
    {
      xtttbl->message('E', "Width syntax error");
      return XTTTBL__HOLDCOMMAND;
    }
  }
  else
    width = 0;

  if ( ODD( dcli_get_qualifier( "/HEIGHT", arg_str, sizeof(arg_str))))
  {
    // convert to integer
    nr = sscanf( arg_str, "%d", &height);
    if ( nr != 1)
    {
      xtttbl->message('E', "Height syntax error");
      return XTTTBL__HOLDCOMMAND;
    }
  }
  else
    height = 0;


  pop =  ODD( dcli_get_qualifier( "/POPNAVIGATOR", 0, 0));

  if ( ODD( dcli_get_qualifier( "/HELPFILE", file_str, sizeof(file_str))))
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Other, file_str, strict);
    if ( EVEN(sts))
      xtttbl->message('E', "No help on this subject");
  }
  else if ( ODD( dcli_get_qualifier( "/BASE", 0, 0)))
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Base, 0, strict);
    if ( EVEN(sts))
      xtttbl->message('E', "No help on this subject");
  }
  else
  {
    sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Base, 0, strict);
    if ( EVEN(sts))
    {
      sts = CoXHelp::dhelp( key, bookmark_str, navh_eHelpFile_Project, 0, strict);
      if ( EVEN(sts))
        xtttbl->message('E', "No help on this subject");
    }
  }

  return 1;
}

static void xtttbl_login_success_bc( void *ctx)
{
  XttTbl *xtttbl = (XttTbl *)ctx;
  char	msg[80];

  CoLogin::get_login_info( 0, 0, xtttbl->user, (unsigned long *)&xtttbl->priv, 0);
  sprintf( msg, "User %s logged in", xtttbl->user);
  xtttbl->cologin = 0;
  xtttbl->message('I', msg);
}

static void xtttbl_login_cancel_bc(void *xtttbl)
{  
  ((XttTbl *)xtttbl)->cologin = 0;
}

static int	xtttbl_exit_func(	void		*client_data,
					void		*client_flag)
{
  XttTbl 		*xtttbl = (XttTbl *)client_data;

  if ( xtttbl->close_cb)
    (xtttbl->close_cb)( xtttbl->parent_ctx);
  else
    exit(1);
  return 1;
}

static int	xtttbl_login_func(	void		*client_data,
					void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;
  int	sts;
  char	arg1_str[80];
  char	arg2_str[80];
  char	systemgroup[80];
  unsigned int	priv;
  char	msg[80];
	
  // sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", &systemgroup, 
  //		sizeof(systemgroup));
  // if ( EVEN(sts)) return sts;
  sts = xtttbl->read_bootfile( 0, systemgroup);

  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str))))
  {
    xtttbl->cologin = xtttbl->login_new( "PwR Login", systemgroup, xtttbl_login_success_bc, 
				     xtttbl_login_cancel_bc, &sts);

    return 1;
  }
  if ( EVEN( dcli_get_qualifier( "dcli_arg2", arg2_str, sizeof(arg2_str))))
  {
    xtttbl->message('E',"Syntax error");
    return 1;
  }

  cdh_ToLower( arg1_str, arg1_str);
  cdh_ToLower( arg2_str, arg2_str);
  sts = user_CheckUser( systemgroup, arg1_str, UserList::pwcrypt(arg2_str), &priv);
  if ( EVEN(sts))
    xtttbl->message('E',"Login failure");
  else
  {
    strcpy( xtttbl->user, arg1_str);
    xtttbl->priv = priv;
    sprintf( msg, "User %s logged in", arg1_str);
    xtttbl->message('I', msg);
  }
  return sts;
}

static int	xtttbl_logout_func(	void		*client_data,
					void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;
  char	msg[80];
	
  if ( strcmp( xtttbl->base_user, "") == 0)
  {
    sprintf( msg, "User %s logged out", xtttbl->user);
    xtttbl->message('I', msg);
  }
  else
  {
    sprintf( msg, "Returned to user %s", xtttbl->base_user);
    xtttbl->message('I', msg);
  }
  strcpy( xtttbl->user, xtttbl->base_user);
  xtttbl->priv = xtttbl->base_priv;
  return 1;
}

static int	xtttbl_open_func(	void		*client_data,
				void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "GRAPH", strlen( arg1_str)) == 0)
  {
  }
  else if ( cdh_NoCaseStrncmp( arg1_str, "HISTORY", strlen( arg1_str)) == 0)
  {

    pwr_tAName name_str;
    char *name_ptr;
    pwr_tAName name_array[10];
    int i, names;
    pwr_tOid oidv[11];
    pwr_tOName anamev[11];
    pwr_tOName onamev[11];
    bool sevhistobjectv[11];
    int sts;
    XttSevHist *hist;

    // Command is "OPEN HISTORY"

    /* Get the name qualifier */
    if ( ODD( dcli_get_qualifier( "dcli_arg2", name_str, sizeof(name_str)))) {
      if ( name_str[0] != '/')
        /* Assume that this is the namestring */
        name_ptr = name_str;
      else {
        xtttbl->message('E', "Syntax error");
        return XTTTBL__HOLDCOMMAND; 	
      } 
    }
    else {
      if ( ODD( dcli_get_qualifier( "/NAME", name_str, sizeof(name_str))))
        name_ptr = name_str;
      else {
        /* Get the selected object */
	ItemBase *item;

	if ( !xtttbl->tblnav->get_select( &item)) {
          xtttbl->message('E', "Enter name or select an object");
          return XTTTBL__SUCCESS;
        }
        xtttbl->activate_opensevhist();
      }
    }

    // The name string can contain several hists separated by ','
    names = dcli_parse( name_str, ",", "",
	     (char *) name_array, sizeof( name_array)/sizeof( name_array[0]), 
	     sizeof( name_array[0]), 0);

    for ( i = 0; i < names; i++) {
      TblNav_sevhistobject *hi;

      if ( i == 10)
	break;

      sts = xtttbl->tblnav->get_item( name_array[i], &hi);
      if ( EVEN(sts)) {
	xtttbl->message('E', "Name object not found");
	return XTTTBL__SUCCESS;
      }

      oidv[i] = hi->oid;
      if ( hi->attrnum == 1) {
	strcpy( anamev[i], hi->objectattrlist[0].aname);
	sevhistobjectv[i] = false;
	strcpy( onamev[i], hi->oname);
      }
      else {
	strcpy( anamev[i], "");
	sevhistobjectv[i] = true;
	strcpy( onamev[i], hi->oname);
      }
    }
    oidv[i] = pwr_cNOid;

    hist = xtttbl->sevhist_new( oidv, anamev, onamev, sevhistobjectv, &sts);
    if ( ODD(sts)) {
      hist->help_cb = XttTbl::sevhist_help_cb;
      hist->get_select_cb = XttTbl::sevhist_get_select_cb;
    }
  }
  else
    xtttbl->message('E',"Syntax error");

  return XTTTBL__SUCCESS;	
}


static int	xtttbl_create_func(	void		*client_data,
					void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "ITEM", strlen( arg1_str)) == 0)
  {

    pwr_tOName name_str;
    pwr_tCmd command_str;

    // Command is "CREATE ITEM"

    if ( EVEN( dcli_get_qualifier( "/NAME", name_str, sizeof(name_str)))) {
      xtttbl->message('E', "Enter name");
      return XTTTBL__SUCCESS;
    }

    if ( EVEN( dcli_get_qualifier( "/COMMAND", command_str, sizeof(command_str)))) {
      xtttbl->message('E', "Enter command");
      return XTTTBL__SUCCESS;
    }

    xtttbl->tblnav->add_item_command( name_str, command_str);
  }
  else
    xtttbl->message('E',"Syntax error");

  return XTTTBL__SUCCESS;	
}


static int	xtttbl_show_func(	void		*client_data,
					void		*client_flag)
{
  XttTbl *xtttbl = (XttTbl *)client_data;

  char	arg1_str[80];
  int	arg1_sts;

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "USER", strlen( arg1_str)) == 0) {
    char msg[120];
    char priv_str[80];

    if ( strcmp( xtttbl->user, "") == 0) {
      user_RtPrivToString( xtttbl->priv, priv_str, sizeof(priv_str));
      sprintf( msg, "Not logged in (%s)", priv_str);
      xtttbl->message('I', msg);
    }
    else {
      user_RtPrivToString( xtttbl->priv, priv_str, sizeof(priv_str));
      sprintf( msg, "User %s (%s)", xtttbl->user, priv_str);
      xtttbl->message('I', msg);
    }
  }
  else
    xtttbl->message('E',"Syntax error");

  return 1;
}

int XttTbl::command( char* input_str)
{
  char     	command[1000];
  int		sts, sym_sts;
  char		symbol_value[DCLI_SYM_VALUE_SIZE];


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

  sts = dcli_cli( (dcli_tCmdTable *)&xtttbl_command_table, command, (void *) this, 0);
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
      sts = dcli_cli( (dcli_tCmdTable *)&xtttbl_command_table, symbol_value, (void *) this, 0);
    }
    else if ( sym_sts == DCLI__SYMBOL_AMBIG)
      sts = sym_sts;
  }
  if (sts == DCLI__COM_AMBIG) message('E',"Ambiguous command");
  else if (sts == DCLI__COM_NODEF) message('E',"Undefined command");
  else if (sts == DCLI__QUAL_AMBIG) message('E',"Ambiguous qualifier");
  else if (sts == DCLI__QUAL_NODEF) message('E',"Undefined qualifier");
  else if (sts == DCLI__SYMBOL_AMBIG) message('E', "Ambiguous symbol abbrevation");

  command_sts = sts;
  return DCLI__SUCCESS;
}

static int xtttbl_ccm_errormessage_func( char *msg, int severity, void *client_data)
{
  XttTbl *xtttbl = (XttTbl *) client_data; 

  if ( EVEN(severity))
    xtttbl->message( 'I', msg);
  else
    xtttbl->message( 'E', msg);
  return 1;
}

static int xtttbl_ccm_deffilename_func( char *outfile, char *infile, void *client_data)
{
  pwr_tFileName fname;

  dcli_translate_filename( fname, infile);
  dcli_get_defaultfilename( fname, outfile, ".rtt_com");
  return 1;
}

int	xtttbl_externcmd_func( char *cmd, void *client_data)
{
  XttTbl *xtttbl = (XttTbl *) client_data; 

  return xtttbl->command( cmd);
}

int XttTbl::readcmdfile( 	char		*incommand)
{
  char		input_str[160];
  int		sts;
  int		appl_sts;
  
  if ( !ccm_func_registred) {
      ccm_func_registred = 1;
  }

  strcpy( input_str, incommand);
  dcli_remove_blank( input_str, input_str);
  xtttbl_store_xtttbl( this);

  /* Read and execute the command file */
  sts = ccm_file_exec( input_str, xtttbl_externcmd_func,
		       xtttbl_ccm_deffilename_func, xtttbl_ccm_errormessage_func, 
		       &appl_sts, verify, 0, NULL, 0, 0, NULL, (void *)this);
  if ( EVEN(sts)) return sts;
  
  return 1;
}

int XttTbl::read_bootfile( char *systemname, char *systemgroup)
{
  pwr_tFileName fname;
  char nodename[80];
  int busid;
  FILE *fp;
  char buffer[256];
  pwr_tStatus sts;
  char *s;

  syi_NodeName(&sts, nodename, sizeof(nodename));
  s = getenv(pwr_dEnvBusId);
  if ( !s)
    return 0;
  
  busid = atoi(s);

  sprintf(fname, dbs_cNameBoot, "$pwrp_load/", nodename, busid);
  dcli_translate_filename( fname, fname);

  fp = fopen( fname, "r");
  if ( !fp)
    return 0;

  s = fgets( buffer, sizeof(buffer) - 1, fp);
  if ( !s) return 0;

  s = fgets( buffer, sizeof(buffer) - 1, fp);
  if ( !s) return 0;
  
  if ( systemname) {
    if ( buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = 0;
    strcpy( systemname, buffer);
  }
  s = fgets( buffer, sizeof(buffer) - 1, fp);
  if ( !s) return 0;

  if ( systemgroup) {
    if ( buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = 0;
    strcpy( systemgroup, buffer);
  }

  fclose(fp);
  return 1;
}
