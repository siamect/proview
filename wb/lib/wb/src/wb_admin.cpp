/** 
 * Proview   $Id: wb_admin.cpp,v 1.7 2005-09-01 14:57:57 claes Exp $
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

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

extern "C" {
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_cnf.h"
#include "flow_x.h"
#include "co_mrm_util.h"
#include "wb_login.h"
#include "co_wow.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_admin.h"
#include "wb_adminnav.h"

//
// Static variables

void Admin::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void Admin::set_clock_cursor_dia()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(adminprop_dia), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void Admin::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

void Admin::reset_cursor_dia()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(adminprop_dia));
}

void Admin::free_cursor()
{
  if (clock_cursor)
    XFreeCursor( flow_Display(toplevel), clock_cursor);
}

static char *pwrp_status_to_string( int value)
{
  static char str[80];

  switch ( value)
  {
    case 2: strcpy( str, "Database doesn't exist"); break;
    case 3: strcpy( str, "No such project"); break;
    case 4: strcpy( str, "Project root already exist"); break;
    case 5: strcpy( str, "Project is not yet configured"); break;
    case 6: strcpy( str, "Projectlist file not found"); break;
    case 7: strcpy( str, "Project already exist"); break;
    case 8: strcpy( str, "Base already exist"); break;
    case 9: strcpy( str, "No such base"); break;
    case 10: strcpy( str, "No write access to projectlist file"); break;
    case 11: strcpy( str, "Syntax error"); break;
    case 12: strcpy( str, "No write access to project root"); break;
    case 13: strcpy( str, "Unable to create project root"); break;
    case 14: strcpy( str, "Unable to delete project root"); break;
    case 15: strcpy( str, "Unable to delete database"); break;
    case 16: strcpy( str, "Base doesn't exist"); break;
    case 17: strcpy( str, "Error in move"); break;
    case 18: strcpy( str, "Current node is not mysql server"); break;
    case 19: strcpy( str, "Unable to find mysql datadir"); break;
    case 20: strcpy( str, "Unable to find database"); break;
    case 21: strcpy( str, "Unable to find template database"); break;
    case 22: strcpy( str, "Database already exist"); break;
    case 23: strcpy( str, "Error in copy"); break;
    case 24: strcpy( str, "Error when creating database"); break;
    case 25: strcpy( str, "No project is set"); break;
    case 26: strcpy( str, "Invalid project name"); break;
    case 27: strcpy( str, "Unable to copy mysql database"); break;
    case 28: strcpy( str, "Unable to rename mysql database"); break;
    case 29: strcpy( str, "Unable to find mysql"); break;
    default:
      sprintf( str, "Errorcode from pwrp: %d", value);
  }
  return str;
}

void Admin::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void Admin::adminprop_setup( char *title, int projname_edit, int base_edit, 
	int path_edit, int hier_edit, int descript_edit,
	char *projname_value, char *base_value, char *path_value,
	char *hier_value, char *descript_value)
{
  Arg 	args[4];
  XmString cstr;

  if ( projname_edit)
  {
    XtUnmanageChild( adminprop_projname_noe);
    XtManageChild( adminprop_projname);
    XmTextSetString( adminprop_projname, projname_value);
  }
  else
  {
    XtManageChild( adminprop_projname_noe);
    XtUnmanageChild( adminprop_projname);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(projname_value , "ISO8859-1"));
    XtSetValues( adminprop_projname_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( base_edit)
  {
    XtUnmanageChild( adminprop_base_noe);
    XtManageChild( adminprop_base);
    XmTextSetString( adminprop_base, base_value);
  }
  else
  {
    XtManageChild( adminprop_base_noe);
    XtUnmanageChild( adminprop_base);
    XtSetArg(args[0],XmNlabelString,
	cstr=XmStringCreateLtoR(base_value , "ISO8859-1"));
    XtSetValues( adminprop_base_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( path_edit)
  {
    XtUnmanageChild( adminprop_path_noe);
    XtManageChild( adminprop_path);
    XmTextSetString( adminprop_path, path_value);
  }
  else
  {
    XtManageChild( adminprop_path_noe);
    XtUnmanageChild( adminprop_path);
    XtSetArg(args[0],XmNlabelString,
	cstr=XmStringCreateLtoR(path_value , "ISO8859-1"));
    XtSetValues( adminprop_path_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( hier_edit)
  {
    XtUnmanageChild( adminprop_hier_noe);
    XtManageChild( adminprop_hier);
    XmTextSetString( adminprop_hier, hier_value);
  }
  else
  {
    XtManageChild( adminprop_hier_noe);
    XtUnmanageChild( adminprop_hier);
    XtSetArg(args[0],XmNlabelString,
	cstr=XmStringCreateLtoR(hier_value , "ISO8859-1"));
    XtSetValues( adminprop_hier_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( descript_edit)
  {
    XtUnmanageChild( adminprop_descript_noe);
    XtManageChild( adminprop_descript);
    XmTextSetString( adminprop_descript, descript_value);
  }
  else
  {
    XtManageChild( adminprop_descript_noe);
    XtUnmanageChild( adminprop_descript);
    XtSetArg(args[0],XmNlabelString,
	cstr=XmStringCreateLtoR(descript_value , "ISO8859-1"));
    XtSetValues( adminprop_descript_noe, args, 1);
    XmStringFree( cstr);
  }

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( XtParent(adminprop_dia), args, 1);

  XtSetArg(args[0],XmNwidth, 600);
  XtSetArg(args[1],XmNheight, 280);
  XtSetValues( adminprop_dia, args, 2);
}

void Admin::adminuser_setup( char *title, int username_edit, int password_edit, 
	int system_edit, int priv_edit,
	char *username_value, char *system_value, unsigned int priv_value)
{
  Arg 	args[4];
  XmString cstr;
  Arg	sensitive[1];

  if ( username_edit)
  {
    XtUnmanageChild( adminuser_username_noe);
    XtManageChild( adminuser_username);
    XmTextSetString( adminuser_username, username_value);
  }
  else
  {
    XtManageChild( adminuser_username_noe);
    XtUnmanageChild( adminuser_username);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(username_value , "ISO8859-1"));
    XtSetValues( adminuser_username_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( password_edit)
  {
    XtManageChild( adminuser_password);
    XmTextSetString( adminuser_password, "");
  }
  else
  {
    XtUnmanageChild( adminuser_password);
  }

  if ( system_edit)
  {
    XtUnmanageChild( adminuser_system_noe);
    XtManageChild( adminuser_system);
    XmTextSetString( adminuser_system, system_value);
  }
  else
  {
    XtManageChild( adminuser_system_noe);
    XtUnmanageChild( adminuser_system);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(system_value , "ISO8859-1"));
    XtSetValues( adminuser_system_noe, args, 1);
    XmStringFree( cstr);
  }

  XmToggleButtonSetState( adminuser_priv_rtread, (priv_value & pwr_mPrv_RtRead) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_rtwrite, (priv_value & pwr_mPrv_RtWrite) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_rtevents, (priv_value & pwr_mPrv_RtEvents) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_system, (priv_value & pwr_mPrv_System) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_maintenance, (priv_value & pwr_mPrv_Maintenance) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_process, (priv_value & pwr_mPrv_Process) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_instrument, (priv_value & pwr_mPrv_Instrument) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator1, (priv_value & pwr_mPrv_Operator1) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator2, (priv_value & pwr_mPrv_Operator2) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator3, (priv_value & pwr_mPrv_Operator3) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator4, (priv_value & pwr_mPrv_Operator4) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator5, (priv_value & pwr_mPrv_Operator5) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator6, (priv_value & pwr_mPrv_Operator6) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator7, (priv_value & pwr_mPrv_Operator7) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator8, (priv_value & pwr_mPrv_Operator8) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator9, (priv_value & pwr_mPrv_Operator9) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_operator10, (priv_value & pwr_mPrv_Operator10) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_devread, (priv_value & pwr_mPrv_DevRead) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_devplc, (priv_value & pwr_mPrv_DevPlc) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_devconfig, (priv_value & pwr_mPrv_DevConfig) != 0, 0);
  XmToggleButtonSetState( adminuser_priv_devclass, (priv_value & pwr_mPrv_DevClass) != 0, 0);

  if ( priv_edit)
    XtSetArg( sensitive[0],XmNsensitive, 1);
  else
    XtSetArg( sensitive[0],XmNsensitive, 0);

  XtSetValues( adminuser_priv_rtread, sensitive, 1);
  XtSetValues( adminuser_priv_rtwrite, sensitive, 1);
  XtSetValues( adminuser_priv_rtevents, sensitive, 1);
  XtSetValues( adminuser_priv_system, sensitive, 1);
  XtSetValues( adminuser_priv_maintenance, sensitive, 1);
  XtSetValues( adminuser_priv_process, sensitive, 1);
  XtSetValues( adminuser_priv_instrument, sensitive, 1);
  XtSetValues( adminuser_priv_operator1, sensitive, 1);
  XtSetValues( adminuser_priv_operator2, sensitive, 1);
  XtSetValues( adminuser_priv_operator3, sensitive, 1);
  XtSetValues( adminuser_priv_operator4, sensitive, 1);
  XtSetValues( adminuser_priv_operator5, sensitive, 1);
  XtSetValues( adminuser_priv_operator6, sensitive, 1);
  XtSetValues( adminuser_priv_operator7, sensitive, 1);
  XtSetValues( adminuser_priv_operator8, sensitive, 1);
  XtSetValues( adminuser_priv_operator9, sensitive, 1);
  XtSetValues( adminuser_priv_operator10, sensitive, 1);
  XtSetValues( adminuser_priv_devread, sensitive, 1);
  XtSetValues( adminuser_priv_devplc, sensitive, 1);
  XtSetValues( adminuser_priv_devconfig, sensitive, 1);
  XtSetValues( adminuser_priv_devclass, sensitive, 1);

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( XtParent(adminuser_dia), args, 1);

  XtSetArg(args[0],XmNwidth, 400);
  XtSetArg(args[1],XmNheight, 650);
  XtSetValues( adminuser_dia, args, 2);
}

void Admin::adminsys_setup( char *title, int systemname_edit, int attr_edit,
	char *systemname_value, unsigned int attr_value)
{
  Arg 	args[4];
  XmString cstr;
  Arg	sensitive[1];

  if ( systemname_edit)
  {
    XtUnmanageChild( adminsys_systemname_noe);
    XtManageChild( adminsys_systemname);
    XmTextSetString( adminsys_systemname, systemname_value);
  }
  else
  {
    XtManageChild( adminsys_systemname_noe);
    XtUnmanageChild( adminsys_systemname);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(systemname_value , "ISO8859-1"));
    XtSetValues( adminsys_systemname_noe, args, 1);
    XmStringFree( cstr);
  }


  XmToggleButtonSetState( adminsys_attr_userinherit, (attr_value & user_mSystemAttr_UserInherit) != 0, 0);

  if ( attr_edit)
    XtSetArg( sensitive[0],XmNsensitive, 1);
  else
    XtSetArg( sensitive[0],XmNsensitive, 0);

  XtSetValues( adminsys_attr_userinherit, sensitive, 1);

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( XtParent(adminsys_dia), args, 1);

  XtSetArg(args[0],XmNwidth, 400);
  XtSetArg(args[1],XmNheight, 350);
  XtSetValues( adminsys_dia, args, 2);
}

void Admin::adminvol_setup( char *title, int volumename_edit, int volumeid_edit,
	int projectname_edit, char *volumename_value, char *volumeid_value,
	char *projectname_value)
{
  XmString cstr;
  Arg 	args[4];

  if ( volumename_edit)
  {
    XtUnmanageChild( adminvol_volumename_noe);
    XtManageChild( adminvol_volumename);
    XmTextSetString( adminvol_volumename, volumename_value);
  }
  else
  {
    XtManageChild( adminvol_volumename_noe);
    XtUnmanageChild( adminvol_volumename);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(volumename_value , "ISO8859-1"));
    XtSetValues( adminvol_volumename_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( volumeid_edit)
  {
    XtUnmanageChild( adminvol_volumeid_noe);
    XtManageChild( adminvol_volumeid);
    XmTextSetString( adminvol_volumeid, volumeid_value);
  }
  else
  {
    XtManageChild( adminvol_volumeid_noe);
    XtUnmanageChild( adminvol_volumeid);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(volumeid_value , "ISO8859-1"));
    XtSetValues( adminvol_volumeid_noe, args, 1);
    XmStringFree( cstr);
  }

  if ( projectname_edit)
  {
    XtUnmanageChild( adminvol_projname_noe);
    XtManageChild( adminvol_projname);
    XmTextSetString( adminvol_projname, projectname_value);
  }
  else
  {
    XtManageChild( adminvol_projname_noe);
    XtUnmanageChild( adminvol_projname);
    XtSetArg(args[0],XmNlabelString, 
	cstr=XmStringCreateLtoR(projectname_value , "ISO8859-1"));
    XtSetValues( adminvol_projname_noe, args, 1);
    XmStringFree( cstr);
  }

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( XtParent(adminvol_dia), args, 1);

  XtSetArg(args[0],XmNwidth, 400);
  XtSetArg(args[1],XmNheight, 200);
  XtSetValues( adminvol_dia, args, 2);
}

void Admin::set_prompt( char *prompt)
{
  Arg 		args[3];
  XmString	cstr;

  cstr=XmStringCreateLtoR( prompt, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNwidth, 50);
  XtSetArg(args[2],XmNheight, 30);
  XtSetValues( cmd_prompt, args, 3);
  XmStringFree( cstr);
}

extern "C" void admin_delete_volume_ok( void *ctx, void *data)
{
  Admin *admin = (Admin *)ctx;
  Item *item = (Item *)data;
  int sts;

  ((AdminNav *)admin->adminnav)->volumes_delete( item->get_node());
  sts = ((AdminNav *)admin->adminnav)->volumes_save();
  if ( EVEN(sts)) {
    wow_DisplayError( admin->adminvol_dia, "Remove volume",
			"Unable to open file $pwra_db/pwr_volumelist.dat");
    ((AdminNav *)admin->adminnav)->refresh();  
    return;
  }
}

extern "C" void admin_delete_user_ok( void *ctx, void *data)
{
  Admin *admin = (Admin *)ctx;
  ItemUser *item = (ItemUser *)data;
  int sts;
  char system_name[80];

  SystemList *system = ((AdminNav *)admin->adminnav)->gu->get_system( item->user);
  if ( !system) return;

  ((AdminNav *)admin->adminnav)->gu->get_system_name( system, system_name);
  sts = ((AdminNav *)admin->adminnav)->gu->remove_user( system_name, item->user->get_name());
  if ( EVEN(sts)) {
    admin->message( 'E', ((AdminNav *)admin->adminnav)->gu->get_status( sts));
    return;
  }
  ((AdminNav *)admin->adminnav)->gu->save();
  ((AdminNav *)admin->adminnav)->refresh();  
}

extern "C" void admin_delete_system_ok( void *ctx, void *data)
{
  Admin *admin = (Admin *)ctx;
  ItemUsersGroup *item = (ItemUsersGroup *)data;
  int sts;
  char system_name[80];

  ((AdminNav *)admin->adminnav)->gu->get_system_name( item->group, system_name);
  sts = ((AdminNav *)admin->adminnav)->gu->remove_system( system_name);
  if ( EVEN(sts)) {
    admin->message( 'E', ((AdminNav *)admin->adminnav)->gu->get_status( sts));
    return;
  }
  ((AdminNav *)admin->adminnav)->gu->save();
  ((AdminNav *)admin->adminnav)->refresh();  
}

int Admin::open_project()
{
  Item *base_item;
  int sts;
  char cmd[200];

  sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
  if ( EVEN(sts))
  {
    message('E', "Select a project, database or volume");
    return 0;
  }

  switch ( base_item->type)
  {
    case adminnav_eItemType_Project:
    {
      ItemProject *item = (ItemProject *)base_item;

      sprintf( cmd, "wb_admin.sh opendb %s \"%s\" \"%s\" %s", item->project, 
		login_prv.username, login_prv.password, "dbdirectory");
      system( cmd);      
      break;
    }
    case adminnav_eItemType_ProjDb:
    {
      ItemProjDb *item = (ItemProjDb *)base_item;

      sprintf( cmd, "wb_admin.sh opendb %s \"%s\" \"%s\" %s", item->project, 
		login_prv.username, login_prv.password, item->db);
      system( cmd);      
      break;
    }
    case adminnav_eItemType_ProjVolume:
    {
      ItemProjVolume *item = (ItemProjVolume *)base_item;
      char db[80];

      if ( strcmp( item->db, "") == 0)
	strcpy( db, "_no_");
      else
	strcpy( db, item->db);

      sprintf( cmd, "wb_admin.sh opendb %s \"%s\" \"%s\" %s %s", item->project, 
		login_prv.username, login_prv.password, db,
		item->volume);
      system( cmd);      
      break;
    }
    default:
      message('E', "Select a project, database or volume");
  }
  return 1;
}

int Admin::create_project()
{
  char		base[80];
  char          project_root[200];
  char          project_name[] = "proj1";

  cnf_get_value( "defaultProjectRoot", project_root);
  if ( project_root[strlen(project_root)-1] != '/')
    strcat( project_root, "/");
  strcat( project_root, project_name);

  if ( ((AdminNav *)adminnav)->baselist)
    strcpy( base, ((AdminNav *)adminnav)->baselist->name);
  else
    strcpy( base, "");
 
  adminprop_setup( "Create project", 1, 1, 1, 1, 1,
        project_name, base, project_root, "", "");
  dia_function = admin_eDiaFunction_Create;

  XtManageChild( adminprop_dia);
  return 1;
}

extern "C" void admin_delete_project_ok( void *ctx, void *data)
{
  Admin *admin = (Admin *)ctx;
  char cmd[200];
  int sts;

  sprintf( cmd, "pwrp_env.sh delete project %s noconfirm",
		(char *) data);
  admin->set_clock_cursor();
  sts = system( cmd);      
  admin->reset_cursor();
  if ( (sts >>= 8) != 0)
  {
    wow_DisplayError( admin->adminprop_dia, "Delete error",
	pwrp_status_to_string( sts));
    return;
  }
  admin->message( 'I', "Project deleted");

  XtFree( (char *) data);

  ((AdminNav *)admin->adminnav)->refresh();  
}

int Admin::delete_project()
{
  char text[200];
  int sts;
  Item *base_item;

  sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
  if ( EVEN(sts))
  {
    message('E', "Select a project");
    return 0;
  }

  switch ( base_item->type)
  {
    case adminnav_eItemType_Project:
    {
      ItemProject *item = (ItemProject *)base_item;

      sprintf( text, "\
Do you wan't to delete all files\n\
and all databases in project '%s'", item->project);

      wow_DisplayQuestion( (void *)this, toplevel, "Delete Project", text,
      		admin_delete_project_ok, NULL, XtNewString( item->project));

      break;
    }
    default:
      message('E', "Select a project");
  }
  return 1;
}

int Admin::copy_project()
{
  int sts;
  Item *base_item;

  sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
  if ( EVEN(sts))
  {
    message('E', "Select a project");
    return 0;
  }

  switch ( base_item->type)
  {
    case adminnav_eItemType_Project:
    {
      ItemProject *item = (ItemProject *)base_item;
      char str_projname[80];
      char str_base[80];
      char str_root[80];
      char str_hier[80];
      char str_descr[80];

      strcpy( str_projname, item->project);
      strcat( str_projname, "2");

      strcpy( str_base, item->base);

      strcpy( str_root, item->root);
      strcat( str_root, "2");

      strcpy( str_hier, item->hierarchy);
      strcat( str_hier, "2");

      sprintf( str_descr, "%s (Copy of %s)", item->description, item->project);

      strcpy( copy_from_proj, item->project);
      dia_function = admin_eDiaFunction_Copy;

      adminprop_setup( "Copy project", 1, 1, 1, 1, 1,
        str_projname, str_base, str_root, str_hier, str_descr);

      XtManageChild( adminprop_dia);

      break;
    }
    default:
      message('E', "Select a project");
  }
  return 1;
}

int Admin::project_properties(int projname_edit, int base_edit,
	int path_edit, int hier_edit, int descr_edit)
{
  int sts;
  Item *base_item;

  sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
  if ( EVEN(sts))
  {
    message('E', "Select a project");
    return 0;
  }

  switch ( base_item->type)
  {
    case adminnav_eItemType_Project:
    {
      ItemProject *item = (ItemProject *)base_item;

      adminprop_setup( "Project properties", projname_edit, base_edit,
	path_edit, hier_edit, descr_edit,
        item->project, item->base, item->root, item->hierarchy,
	item->description);

      strcpy( modify_projname, item->project);
      strcpy( modify_base, item->base);
      strcpy( modify_root, item->root);
      strcpy( modify_hier, item->hierarchy);
      strcpy( modify_descript, item->description);
      projname_editable = projname_edit;
      base_editable = base_edit;
      root_editable = path_edit;
      hier_editable = hier_edit;
      descript_editable = descr_edit;

      dia_function = admin_eDiaFunction_Prop;

      XtManageChild( adminprop_dia);

      break;
    }
    default:
      message('E', "Select a project");
  }
  return 1;
}

int Admin::user_properties( admin_eDiaUserFunction function, int username_edit, int password_edit,
	int system_edit, int priv_edit)
{
  int sts;
  Item *base_item;

  switch ( function) {
  case admin_eDiaUserFunction_Prop:
    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( EVEN(sts)) {
      message('E', "Select a user");
      return 0;
    }

    switch ( base_item->type){
    case adminnav_eItemType_User: {
      ItemUser *item = (ItemUser *)base_item;
      char system_name[80];

      SystemList *system = ((AdminNav *)adminnav)->gu->get_system( item->user);
      ((AdminNav *)adminnav)->gu->get_system_name( system, system_name);
      adminuser_setup( "User properties", username_edit, password_edit,
	system_edit, priv_edit,
        item->user->get_name(), system_name, item->user->get_priv());

      diauser_function = admin_eDiaUserFunction_Prop;

      XtManageChild( adminuser_dia);

      break;
    }
    default:
      message('E', "Select a user");
    }
  case admin_eDiaUserFunction_New:
    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( EVEN(sts)) {
      message('E', "Select a systemgroup");
      return 0;
    }

    switch ( base_item->type){
    case adminnav_eItemType_UsersGroup: {
      ItemUsersGroup *item = (ItemUsersGroup *)base_item;
      char system_name[80];

      ((AdminNav *)adminnav)->gu->get_system_name( item->group, system_name);
      adminuser_setup( "New User", username_edit, password_edit,
	system_edit, priv_edit, "", system_name, 0);

      diauser_function = admin_eDiaUserFunction_New;

      XtManageChild( adminuser_dia);

      break;
    }
    default:
      message('E', "Select a systemgroup");
    }
    break;
  }

  return 1;
}

int Admin::system_properties( admin_eDiaSysFunction function, int systemname_edit, int attr_edit)
{
  int sts;
  Item *base_item;

  switch ( function) {
  case admin_eDiaSysFunction_Prop:
    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( EVEN(sts)) {
      message('E', "Select a systemgroup");
      return 0;
    }

    switch ( base_item->type){
    case adminnav_eItemType_UsersGroup: {
      ItemUsersGroup *item = (ItemUsersGroup *)base_item;
      char system_name[80];

      ((AdminNav *)adminnav)->gu->get_system_name( item->group, system_name);
      adminsys_setup( "System group properties", systemname_edit, attr_edit,
        system_name, item->group->get_attributes());

      diasys_function = admin_eDiaSysFunction_Prop;

      XtManageChild( adminsys_dia);

      break;
    }
    default:
      message('E', "Select a system group");
    }
  case admin_eDiaSysFunction_New: {
    char system_name[80];

    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( ODD(sts)) {
      switch ( base_item->type){
      case adminnav_eItemType_UsersGroup: {
	ItemUsersGroup *item = (ItemUsersGroup *)base_item;
	
	((AdminNav *)adminnav)->gu->get_system_name( item->group, system_name);
	strcat( system_name, ".");
	break;
      }
      default:
	message('E', "Select a system group");
	return 0;
      }
    }
    else
      strcpy( system_name, "");

    adminsys_setup( "New System Group", systemname_edit, attr_edit, system_name, 0);
    diasys_function = admin_eDiaSysFunction_New;
    
    XtManageChild( adminsys_dia);
    
    break;
    }
  }

  return 1;
}

int Admin::volume_properties( admin_eDiaVolFunction function, int volumename_edit, 
			      int volumeid_edit, int projectname_edit)
{
  int sts;
  Item *base_item;

  switch ( function) {
  case admin_eDiaVolFunction_Prop:
    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( EVEN(sts)) {
      message('E', "Select a volume");
      return 0;
    }

    switch ( base_item->type){
    case adminnav_eItemType_Volume: {
      ItemVolume *item = (ItemVolume *)base_item;
      char volumeid[80];

      strcpy( volumeid, cdh_VolumeIdToString( NULL, item->vid, 0, 0));
      adminvol_setup( "Volume properties", volumename_edit, volumeid_edit,
        projectname_edit, item->name, volumeid, item->project);

      adminvol_item = base_item;
      diavol_function = admin_eDiaVolFunction_Prop;

      XtManageChild( adminvol_dia);

      break;
    }
    default:
      message('E', "Select a volume");
    }
    break;
  case admin_eDiaVolFunction_New: {
    sts = ((AdminNav *)adminnav)->get_select( (void **) &base_item);
    if ( ODD(sts))
      adminvol_item = base_item;
    else
      adminvol_item = 0;

    adminvol_setup( "Volume properties", volumename_edit, volumeid_edit, 
		    projectname_edit, "", "", "");
    diavol_function = admin_eDiaVolFunction_New;
    
    XtManageChild( adminvol_dia);
    
    break;
    }
  }

  return 1;
}

static void admin_valchanged_cmd_input( Widget w, XEvent *event)
{
  Admin *admin;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &admin);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)admin->cmd_recall, sizeof(admin->cmd_recall[0]),
	sizeof( admin->cmd_recall)/sizeof(admin->cmd_recall[0]),
	&admin->cmd_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    sts = admin->command( text);
    XtUnmanageChild( w);
    admin->set_prompt( "");
    admin->input_open = 0;
    ((AdminNav *)admin->adminnav)->set_inputfocus(1);
  }
}

int Admin::open_input()
{
  Widget	text_w;
  char		*value = NULL;
  Arg 		args[5];
  int		input_size = 80;

  if ( input_open) 
  {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return 1;
  }

  text_w = cmd_input;
  XtManageChild( text_w);
  XtSetArg(args[0],XmNmaxLength, input_size-1);
  XtSetValues( text_w, args, 1);
  if ( value)
  {
    XmTextSetString( text_w, value);
    XmTextSetInsertionPosition( text_w, strlen(value));
  }
  else
    XmTextSetString( text_w, "");

  if ( value)
    XtFree( value);

  message( ' ', "");
  flow_SetInputFocus( text_w);
  set_prompt( "admin >");
  input_open = 1;

  return 1;
}

static void admin_message_cb( void *admin, char severity, char *message)
{
  ((Admin *)admin)->message( severity, message);
}


//
//  Callbackfunctions from menu entries
//
static void admin_activate_command( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->open_input();
}

static void admin_activate_openproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->open_project();
}

static void admin_activate_createproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not edit mode");
    return;
  }
  admin->create_project();
}

static void admin_activate_deleteproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not edit mode");
    return;
  }
  admin->delete_project();
}

static void admin_activate_copyproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not edit mode");
    return;
  }
  admin->copy_project();
}

static void admin_activate_renameproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not edit mode");
    return;
  }
  admin->project_properties(1,0,0,0,0);
}

static void admin_activate_moveproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not edit mode");
    return;
  }
  admin->project_properties(0,0,1,0,0);
}

static void admin_activate_projectprop( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->project_properties(0,0,0,0,0);
  else
    admin->project_properties(0,1,0,1,1);
}

static void admin_activate_volumeprop( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->volume_properties( admin_eDiaVolFunction_Prop, 0,0,0);
  else
    admin->volume_properties( admin_eDiaVolFunction_Prop, 1,1,1);
}

static void admin_activate_newvolume( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->message('E', "Not edit mode");
  else
    admin->volume_properties( admin_eDiaVolFunction_New, 1,1,1);
}

static void admin_activate_removevolume( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  Item *item;
  int sts;
  char text[200];

  if ( !admin->edit_mode) {
    admin->message('E', "Not edit mode");
    return;
  }

  sts = ((AdminNav *)admin->adminnav)->get_select( (void **) &item);
  if ( EVEN(sts) || !( item->type == adminnav_eItemType_Volume)) {
    admin->message('E', "Select a volume");
    return;
  }

  sprintf( text, "Do you wan't to remove volume %s\n", item->identity());
  wow_DisplayQuestion( (void *)admin, admin->toplevel, "Remove volume", text,
		       admin_delete_volume_ok, NULL, item);
}

static void admin_activate_userprop( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->user_properties( admin_eDiaUserFunction_Prop, 0,0,0,0);
  else
    admin->user_properties( admin_eDiaUserFunction_Prop, 0,1,0,1);
}

static void admin_activate_newuser( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->message('E', "Not edit mode");
  else
    admin->user_properties( admin_eDiaUserFunction_New, 1,1,0,1);
}

static void admin_activate_removeuser( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  ItemUser *item;
  char text[200];
  int sts;

  if ( !admin->edit_mode) {
    admin->message('E', "Not edit mode");
    return;
  }

  sts = ((AdminNav *)admin->adminnav)->get_select( (void **) &item);
  if ( EVEN(sts) || item->type != adminnav_eItemType_User) {
    admin->message('E', "Select a user");
    return;
  }

  sprintf( text, "Do you wan't to remove user %s\n", item->identity());
  wow_DisplayQuestion( (void *)admin, admin->toplevel, "Remove user", text,
		       admin_delete_user_ok, NULL, item);
}

static void admin_activate_systemprop( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->system_properties( admin_eDiaSysFunction_Prop, 0,0);
  else
    admin->system_properties( admin_eDiaSysFunction_Prop, 0,1);
}

static void admin_activate_newsystem( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
    admin->message('E', "Not edit mode");
  else
    admin->system_properties( admin_eDiaSysFunction_New, 1,1);
}

static void admin_activate_removesystem( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  ItemUsersGroup *item;
  char text[200];
  int sts;

  if ( !admin->edit_mode) {
    admin->message('E', "Not edit mode");
    return;
  }

  sts = ((AdminNav *)admin->adminnav)->get_select( (void **) &item);
  if ( EVEN(sts) || item->type != adminnav_eItemType_UsersGroup) {
    admin->message('E', "Select a system group");
    return;
  }

  sprintf( text, "Do you wan't to remove user %s\n", item->identity());
  wow_DisplayQuestion( (void *)admin, admin->toplevel, "Remove system grou", text,
		       admin_delete_system_ok, NULL, item);
}

static void admin_activate_exit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( admin->close_cb)
    (admin->close_cb)( admin);
  else
    delete admin;
}

static void admin_activate_refresh( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
    ((AdminNav *)admin->adminnav)->refresh();
}

static void admin_activate_print( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  int sts;
  char msg[80];
  char filename[80] = "/tmp/wb_adm.ps";
  char printscript[120] = "$pwr_exe/wb_gre_print.sh";
  char cmd[200];

  sts = ((AdminNav *)admin->adminnav)->print( filename);
  if ( EVEN(sts))
  {
    sprintf( msg, "Unable to open file %s", filename);
    admin->message('E', msg);
    return;
  }
  dcli_translate_filename( printscript, printscript);
  sprintf( cmd, "%s %s", printscript, filename);
  admin->set_clock_cursor();
  system( cmd);      
  admin->reset_cursor();

  sprintf( msg, "Postscript file created '%s'", filename);
  admin->message('I', msg);
}

static void admin_activate_edit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( admin->edit_mode) {
    admin->edit_mode = 0;
    admin->message('I', "Leaving edit mode");
    XmToggleButtonSetState( admin->menu_edit, 0, 0);
  }
  else {
    if ( !admin->loggedin) {
      admin->message('E', "Not authorized to enter edit");
      XmToggleButtonSetState( admin->menu_edit, 0, 0);
      return;
    }
    admin->edit_mode = 1;
    admin->message('I', "Entering edit mode");
    XmToggleButtonSetState( admin->menu_edit, 1, 0);
  }
}

static void admin_activate_viewprojects( Widget w, Admin *admin, XmToggleButtonCallbackStruct *data)
{
  if ( data->set) {
    ((AdminNav *)admin->adminnav)->view( adminnav_eMode_Projects);
    XtUnmanageChild( admin->volume_entry);
    XtUnmanageChild( admin->user_entry);
    XtManageChild( admin->proj_entry);
    admin->set_title( "PwR Project Administrator");
  }
}

static void admin_activate_viewvolumes( Widget w, Admin *admin, XmToggleButtonCallbackStruct *data)
{
  if ( data->set) {
    ((AdminNav *)admin->adminnav)->view( adminnav_eMode_Volumes);
    XtUnmanageChild( admin->proj_entry);
    XtUnmanageChild( admin->user_entry);
    XtManageChild( admin->volume_entry);
    admin->set_title( "PwR Volume Administrator");
  }
}

static void admin_activate_viewusers( Widget w, Admin *admin, XmToggleButtonCallbackStruct *data)
{
  if ( data->set) {
    ((AdminNav *)admin->adminnav)->view( adminnav_eMode_Users);
    XtUnmanageChild( admin->volume_entry);
    XtUnmanageChild( admin->proj_entry);
    XtManageChild( admin->user_entry);
    admin->set_title( "PwR User Administrator");
  }
}

static void admin_activate_zoomin( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  ((AdminNav *)admin->adminnav)->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  ((AdminNav *)admin->adminnav)->zoom( 1.18);
}

static void admin_activate_zoomout( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  ((AdminNav *)admin->adminnav)->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  ((AdminNav *)admin->adminnav)->zoom( 1.0/1.18);
}

static void admin_activate_zoomreset( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  ((AdminNav *)admin->adminnav)->unzoom();
}

static void admin_activate_help( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void admin_create_msg_label( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->msg_label = w;
}
static void admin_create_cmd_prompt( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->cmd_prompt = w;
}
static void admin_create_cmd_input( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, admin);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) admin_valchanged_cmd_input, 
	mrm_eUtility_Admin);

  admin->cmd_input = w;
}
static void admin_create_adminnav_form( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminnav_form = w;
}

static void admin_create_proj_entry( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->proj_entry = w;
}

static void admin_create_volume_entry( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->volume_entry = w;
}

static void admin_create_user_entry( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->user_entry = w;
}

static void admin_create_menu_project( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->menu_project = w;
}

static void admin_create_menu_edit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->menu_edit = w;
}

static void adminprop_create_projnamevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_projname = w;
}

static void adminprop_create_basevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_base = w;
}

static void adminprop_create_pathvalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_path = w;
}

static void adminprop_create_hiervalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_hier = w;
}

static void adminprop_create_descriptvalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_descript = w;
}

static void adminprop_create_projna_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_projname_noe = w;
}

static void adminprop_create_base_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_base_noe = w;
}

static void adminprop_create_path_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_path_noe = w;
}

static void adminprop_create_hier_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_hier_noe = w;
}

static void adminprop_create_descr_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminprop_descript_noe = w;
}

static void adminprop_activate_ok( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  char *pname;
  char *bname;
  char *proot;
  char *phier;
  char *descr;
  char broot[120];
  char cmd[300];
  int sts;

  switch ( admin->dia_function)
  {
    case admin_eDiaFunction_Create:
      printf( "Create project\n");

      pname = XmTextGetString( admin->adminprop_projname);
      bname = XmTextGetString( admin->adminprop_base);
      proot = XmTextGetString( admin->adminprop_path);
      phier = XmTextGetString( admin->adminprop_hier);
      descr = XmTextGetString( admin->adminprop_descript);

      sts = ((AdminNav *)admin->adminnav)->basename_to_baseroot( bname, broot);
      if ( EVEN(sts))
      {
        wow_DisplayError( admin->adminprop_dia, "Create error",
		"Invalid base version");
        return;
      }
      
      sprintf( cmd, "wb_admin.sh create project %s %s %s \"%s\" \"%s\"",
		pname, bname, proot, phier, descr);
      admin->set_clock_cursor();
      admin->set_clock_cursor_dia();
      sts = system( cmd);      
      admin->reset_cursor();
      admin->reset_cursor_dia();
      if ( (sts >>= 8) != 0)
      {
        wow_DisplayError( admin->adminprop_dia, "Create error",
	pwrp_status_to_string( sts));
        return;
      }
      XtUnmanageChild( admin->adminprop_dia);
      admin->message( 'I', "Project created");
      ((AdminNav *)admin->adminnav)->refresh();  

      break;
    case admin_eDiaFunction_Copy:
    {
      int sts;

      printf( "Copy project\n");

      pname = XmTextGetString( admin->adminprop_projname);
      bname = XmTextGetString( admin->adminprop_base);
      proot = XmTextGetString( admin->adminprop_path);
      phier = XmTextGetString( admin->adminprop_hier);
      descr = XmTextGetString( admin->adminprop_descript);

      sts = ((AdminNav *)admin->adminnav)->basename_to_baseroot( bname, broot);
      if ( EVEN(sts))
      {
        admin->message('E', "Invalid base version");
        return;
      }
      
      sprintf( cmd, "pwrp_env.sh copy project %s %s %s %s noconfirm",
		admin->copy_from_proj, pname, proot, phier);
      admin->set_clock_cursor();
      admin->set_clock_cursor_dia();
      sts = system( cmd);
      admin->reset_cursor();
      admin->reset_cursor_dia();
      if ( (sts >>= 8) != 0)
      {
        wow_DisplayError( admin->adminprop_dia, "Copy error",
		pwrp_status_to_string( sts));
        return;
      }

      sprintf( cmd, "pwrp_env.sh modify project %s  -d \"%s\"", pname, descr);
      admin->set_clock_cursor();
      admin->set_clock_cursor_dia();
      sts = system( cmd);
      admin->reset_cursor();
      admin->reset_cursor_dia();

      XtUnmanageChild( admin->adminprop_dia);
      admin->message( 'I', "Project created");
      ((AdminNav *)admin->adminnav)->refresh();  

      break;
    }
    case admin_eDiaFunction_Prop:
    {
      int sts;
      char broot[80];
      int mod_something = 0;

      printf( "Modify project\n");

      sprintf( cmd, "pwrp_env.sh modify project %s", admin->modify_projname);
      if ( admin->projname_editable)
      {
        pname = XmTextGetString( admin->adminprop_projname);
        if ( strcmp( pname, admin->modify_projname) != 0)
        {
          strcat( cmd, " -n ");
          strcat( cmd, pname);
          mod_something = 1;
        }
      }
      if ( admin->base_editable)
      {
        bname = XmTextGetString( admin->adminprop_base);
        if ( strcmp( bname, admin->modify_base) != 0)
        {
          sts = ((AdminNav *)admin->adminnav)->basename_to_baseroot( bname, broot);
          if ( EVEN(sts))
          {
            admin->message('E', "Invalid base version");
            return;
          }

          strcat( cmd, " -b ");
          strcat( cmd, bname);
          mod_something = 1;
        }
      }
      if ( admin->root_editable)
      {
        proot = XmTextGetString( admin->adminprop_path);
        if ( strcmp( proot, admin->modify_root) != 0)
        {
          strcat( cmd, " -r ");
          strcat( cmd, proot);
          mod_something = 1;
        }
      }
      if ( admin->hier_editable)
      {
        phier = XmTextGetString( admin->adminprop_hier);
        if ( strcmp( phier, admin->modify_hier) != 0)
        {
          strcat( cmd, " -h ");
          strcat( cmd, phier);
          mod_something = 1;
        }
      }
      if ( admin->descript_editable)
      {
        descr = XmTextGetString( admin->adminprop_descript);
        if ( strcmp( descr, admin->modify_descript) != 0)
        {
          sprintf( &cmd[strlen(cmd)], " -d \"%s\"", descr);
          mod_something = 1;
        }
      }

      if ( !mod_something)
        return; 
     
      admin->set_clock_cursor();
      admin->set_clock_cursor_dia();
      sts = system( cmd);
      admin->reset_cursor();
      admin->reset_cursor_dia();
      if ( (sts >>= 8) != 0)
      {
        wow_DisplayError( admin->adminprop_dia, "Modify error",
		pwrp_status_to_string( sts));
        return;
      }
      XtUnmanageChild( admin->adminprop_dia);
      admin->message( 'I', "Project modified");
      ((AdminNav *)admin->adminnav)->refresh();  

      break;
    }
    default:
      ;
  }
}

static void adminprop_activate_cancel( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( admin->adminprop_dia);
}

// User dialog callbacks

static void adminuser_create_usernamevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_username = w;
}

static void adminuser_create_passwordvalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_password = w;
}

static void adminuser_create_systemvalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_system = w;
}

static void adminuser_create_userna_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_username_noe = w;
}

static void adminuser_create_system_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_system_noe = w;
}

static void adminuser_create_rtread( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_rtread = w;
}
static void adminuser_create_rtwrite( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_rtwrite = w;
}
static void adminuser_create_rtevents( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_rtevents = w;
}
static void adminuser_create_system( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_system = w;
}
static void adminuser_create_maintenance( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_maintenance = w;
}
static void adminuser_create_process( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_process = w;
}
static void adminuser_create_instrument( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_instrument = w;
}
static void adminuser_create_operator1( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator1 = w;
}
static void adminuser_create_operator2( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator2 = w;
}
static void adminuser_create_operator3( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator3 = w;
}
static void adminuser_create_operator4( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator4 = w;
}
static void adminuser_create_operator5( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator5 = w;
}
static void adminuser_create_operator6( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator6 = w;
}
static void adminuser_create_operator7( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator7 = w;
}
static void adminuser_create_operator8( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator8 = w;
}
static void adminuser_create_operator9( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator9 = w;
}
static void adminuser_create_operator10( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_operator10 = w;
}
static void adminuser_create_devread( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_devread = w;
}
static void adminuser_create_devplc( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_devplc = w;
}
static void adminuser_create_devconfig( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_devconfig = w;
}
static void adminuser_create_devclass( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminuser_priv_devclass = w;
}

static void adminuser_activate_ok( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  char *username;
  char *password;
  unsigned int priv = 0;
  int sts;
  char old_password[80];
  unsigned int old_priv;

  if ( !admin->edit_mode) {
    XtUnmanageChild( admin->adminuser_dia);
    return;
  }

  if ( XmToggleButtonGetState( admin->adminuser_priv_rtread))
    priv |= pwr_mPrv_RtRead;
  if ( XmToggleButtonGetState( admin->adminuser_priv_rtwrite))
    priv |= pwr_mPrv_RtWrite;
  if ( XmToggleButtonGetState( admin->adminuser_priv_rtevents))
    priv |= pwr_mPrv_RtEvents;
  if ( XmToggleButtonGetState( admin->adminuser_priv_system))
    priv |= pwr_mPrv_System;
  if ( XmToggleButtonGetState( admin->adminuser_priv_maintenance))
    priv |= pwr_mPrv_Maintenance;
  if ( XmToggleButtonGetState( admin->adminuser_priv_process))
    priv |= pwr_mPrv_Process;
  if ( XmToggleButtonGetState( admin->adminuser_priv_instrument))
    priv |= pwr_mPrv_Instrument;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator1))
    priv |= pwr_mPrv_Operator1;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator2))
    priv |= pwr_mPrv_Operator2;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator3))
    priv |= pwr_mPrv_Operator3;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator4))
    priv |= pwr_mPrv_Operator4;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator5))
    priv |= pwr_mPrv_Operator5;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator6))
    priv |= pwr_mPrv_Operator6;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator7))
    priv |= pwr_mPrv_Operator7;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator8))
    priv |= pwr_mPrv_Operator8;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator9))
    priv |= pwr_mPrv_Operator9;
  if ( XmToggleButtonGetState( admin->adminuser_priv_operator10))
    priv |= pwr_mPrv_Operator10;
  if ( XmToggleButtonGetState( admin->adminuser_priv_devread))
    priv |= pwr_mPrv_DevRead;
  if ( XmToggleButtonGetState( admin->adminuser_priv_devplc))
    priv |= pwr_mPrv_DevPlc;
  if ( XmToggleButtonGetState( admin->adminuser_priv_devconfig))
    priv |= pwr_mPrv_DevConfig;
  if ( XmToggleButtonGetState( admin->adminuser_priv_devclass))
    priv |= pwr_mPrv_DevClass;
       
  switch ( admin->diauser_function) {
  case admin_eDiaUserFunction_Prop: {
    char username_label[80];
    char system_label[80];

    wow_GetLabel( admin->adminuser_username_noe, username_label);
    wow_GetLabel( admin->adminuser_system_noe, system_label);
    password = XmTextGetString( admin->adminuser_password);


    if ( strcmp( username_label, "") == 0) {
      wow_DisplayError( admin->adminuser_dia, "Modify user error",
			"Username is missing");
      return;
    }
    if ( strcmp( system_label, "") == 0) {
      wow_DisplayError( admin->adminuser_dia, "Modify user error",
			"Systemgroup is missing");
      return;
    }
    sts = ((AdminNav *)admin->adminnav)->gu->get_user_data( system_label, username_label, old_password, &old_priv);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminuser_dia, "Modify user error",
			((AdminNav *)admin->adminnav)->gu->get_status( sts));
      return;
    }

    if ( strcmp( password, "") == 0)
      password = old_password;
    
    sts = ((AdminNav *)admin->adminnav)->gu->modify_user( system_label, username_label, password, priv);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminuser_dia, "Modify user error",
			((AdminNav *)admin->adminnav)->gu->get_status( sts));
      return;
    }
    ((AdminNav *)admin->adminnav)->gu->save();
    break;
  }
  case admin_eDiaUserFunction_New: {
    char system_label[80];

    wow_GetLabel( admin->adminuser_system_noe, system_label);
    username = XmTextGetString( admin->adminuser_username);
    password = XmTextGetString( admin->adminuser_password);

    if ( strcmp( username, "") == 0) {
      wow_DisplayError( admin->adminuser_dia, "New user error",
			"Username is missing");
      return;
    }
    if ( strcmp( password, "") == 0) {
      wow_DisplayError( admin->adminuser_dia, "New user error",
			"Password is missing");
      return;
    }
    if ( strcmp( system_label, "") == 0) {
      wow_DisplayError( admin->adminuser_dia, "New user error",
			"Systemgroup is missing");
      return;
    }
    sts = ((AdminNav *)admin->adminnav)->gu->add_user( system_label, username, password, priv);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminuser_dia, "New user error",
			((AdminNav *)admin->adminnav)->gu->get_status( sts));
      return;
    }
    ((AdminNav *)admin->adminnav)->gu->save();
    ((AdminNav *)admin->adminnav)->refresh();  
    break;
  }
  }
  XtUnmanageChild( admin->adminuser_dia);
}

static void adminuser_activate_cancel( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( admin->adminuser_dia);
}

//
// Systemgroup dia callbacks
//
static void adminsys_create_systemnamevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminsys_systemname = w;
}

static void adminsys_create_systemna_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminsys_systemname_noe = w;
}

static void adminsys_create_userinherit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminsys_attr_userinherit = w;
}

static void adminsys_activate_ok( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  char *systemname;
  unsigned int attr = 0;
  int sts;

  if ( !admin->edit_mode) {
    XtUnmanageChild( admin->adminsys_dia);
    return;
  }

  if ( XmToggleButtonGetState( admin->adminsys_attr_userinherit))
    attr |= user_mSystemAttr_UserInherit;
       
  switch ( admin->diasys_function) {
  case admin_eDiaSysFunction_Prop: {
    char systemname_label[80];

    wow_GetLabel( admin->adminsys_systemname_noe, systemname_label);

    if ( strcmp( systemname_label, "") == 0) {
      wow_DisplayError( admin->adminsys_dia, "Modify systemgroup error",
			"Systemgroup name is missing");
      return;
    }

    sts = ((AdminNav *)admin->adminnav)->gu->modify_system( systemname_label, attr);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminsys_dia, "Modify systemgroup error",
			((AdminNav *)admin->adminnav)->gu->get_status( sts));
      return;
    }
    ((AdminNav *)admin->adminnav)->gu->save();
    break;
  }
  case admin_eDiaSysFunction_New: {
    systemname = XmTextGetString( admin->adminsys_systemname);

    if ( strcmp( systemname, "") == 0) {
      wow_DisplayError( admin->adminsys_dia, "New systemgroup error",
			"Systemgroup name is missing");
      return;
    }

    sts = ((AdminNav *)admin->adminnav)->gu->add_system( systemname, attr);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminsys_dia, "New systemgroup error",
			((AdminNav *)admin->adminnav)->gu->get_status( sts));
      return;
    }
    ((AdminNav *)admin->adminnav)->gu->save();
    ((AdminNav *)admin->adminnav)->refresh();  
    break;
  }
  }
  XtUnmanageChild( admin->adminsys_dia);
}

static void adminsys_activate_cancel( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( admin->adminsys_dia);
}

//
// Volume dia callbacks
//
static void adminvol_create_volumenamevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_volumename = w;
}

static void adminvol_create_volumena_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_volumename_noe = w;
}

static void adminvol_create_volumeidvalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_volumeid = w;
}

static void adminvol_create_volumeid_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_volumeid_noe = w;
}

static void adminvol_create_projnamevalue( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_projname = w;
}

static void adminvol_create_projna_noedit( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  admin->adminvol_projname_noe = w;
}

static void adminvol_activate_ok( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  char *volumename;
  char *volumeid;
  char *projectname;
  pwr_tVolumeId volid;
  int sts;

  if ( !admin->edit_mode) {
    XtUnmanageChild( admin->adminvol_dia);
    return;
  }

  switch ( admin->diavol_function) {
  case admin_eDiaVolFunction_Prop: {
    volumename = XmTextGetString( admin->adminvol_volumename);
    volumeid = XmTextGetString( admin->adminvol_volumeid);
    projectname = XmTextGetString( admin->adminvol_projname);

    if ( strcmp( volumename, "") == 0) {
      wow_DisplayError( admin->adminvol_dia, "Modify volume error",
			"Volume name is missing");
      return;
    }

    if ( strcmp( projectname, "") == 0) {
      wow_DisplayError( admin->adminvol_dia, "Modify volume error",
			"Project name is missing");
      return;
    }

    sts = cdh_StringToVolumeId( volumeid, &volid);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminvol_dia, "Modify volume error",
			"Syntax error in volume id");
      return;
    }

    ((ItemVolume *)admin->adminvol_item)->modify( volumename, volumeid, projectname);
    sts = ((AdminNav *)admin->adminnav)->volumes_save();
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminvol_dia, "Modify volume",
			"Unable to open file $pwra_db/pwr_volumelist.dat");
      return;
    }
    break;
  }
  case admin_eDiaSysFunction_New: {
    volumename = XmTextGetString( admin->adminvol_volumename);
    volumeid = XmTextGetString( admin->adminvol_volumeid);
    projectname = XmTextGetString( admin->adminvol_projname);

    if ( strcmp( volumename, "") == 0) {
      wow_DisplayError( admin->adminvol_dia, "New volume error",
			"Volume name is missing");
      return;
    }

    if ( strcmp( projectname, "") == 0) {
      wow_DisplayError( admin->adminvol_dia, "New volume error",
			"Project name is missing");
      return;
    }

    sts = cdh_StringToVolumeId( volumeid, &volid);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminvol_dia, "New volume error",
			"Syntax error in volume id");
      return;
    }

    if ( !admin->adminvol_item)
      sts = ((AdminNav *)admin->adminnav)->volumes_add( volumename, volumeid, projectname,
		      0, flow_eDest_IntoLast);
    else
      sts = ((AdminNav *)admin->adminnav)->volumes_add( volumename, volumeid, projectname,
		      admin->adminvol_item->get_node(), flow_eDest_After);
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminvol_dia, "New volume",
			"Volume id or volumename is not unic");
      return;
    }
    sts = ((AdminNav *)admin->adminnav)->volumes_save();
    if ( EVEN(sts)) {
      wow_DisplayError( admin->adminvol_dia, "New volume",
			"Unable to open file $pwra_db/pwr_volumelist.dat");
      return;
    }
    break;
  }
  }
  XtUnmanageChild( admin->adminvol_dia);
}

static void adminvol_activate_cancel( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( admin->adminvol_dia);
}

static void admin_enable_set_focus( Admin *admin)
{
  admin->set_focus_disabled--;
}

static void admin_disable_set_focus( Admin *admin, int time)
{
  admin->set_focus_disabled++;
  admin->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( admin->toplevel), time,
	(XtTimerCallbackProc)admin_enable_set_focus, admin);
}

static void admin_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Admin *admin;

  XtSetArg    (args[0], XmNuserData, &admin);
  XtGetValues (w, args, 1);

  if ( !admin)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( admin->set_focus_disabled)
    return;

  if ( flow_IsManaged( admin->cmd_input))
    flow_SetInputFocus( admin->cmd_input);
  else if ( admin->adminnav)
    ((AdminNav *)admin->adminnav)->set_inputfocus(1);

  admin_disable_set_focus( admin, 400);

}
void Admin::set_title( char *title)
{
  Arg args[1];

  XtSetArg(args[0],XmNtitle, title);
  XtSetValues( parent_wid, args, 1);
}

Admin::~Admin()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  free_cursor();
  delete (AdminNav *)adminnav;
  XtDestroyWidget( parent_wid);
}

Admin::Admin( 
	Widget 		wa_parent_wid,
	void 		*wa_parent_ctx) :
 	parent_wid(wa_parent_wid), parent_ctx(wa_parent_ctx),
	input_open(0), close_cb(0), set_focus_disabled(0),
	cmd_current_recall(0),
	ccm_func_registred(0), verify(0), scriptmode(0), edit_mode(0),
	clock_cursor(0), loggedin(0)
{
  char		uid_filename[120] = {"pwr_exe:wb_admin.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: admin_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"admin_inputfocus",      (XtActionProc) admin_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "admin_ctx", 0 },
	{"admin_create_proj_entry",(caddr_t)admin_create_proj_entry },
	{"admin_create_volume_entry",(caddr_t)admin_create_volume_entry },
	{"admin_create_user_entry",(caddr_t)admin_create_user_entry },
	{"admin_create_menu_edit",(caddr_t)admin_create_menu_edit },
	{"admin_create_menu_project",(caddr_t)admin_create_menu_project },
	{"admin_activate_exit",(caddr_t)admin_activate_exit },
	{"admin_activate_edit",(caddr_t)admin_activate_edit },
	{"admin_activate_refresh",(caddr_t)admin_activate_refresh },
	{"admin_activate_print",(caddr_t)admin_activate_print },
	{"admin_activate_command",(caddr_t)admin_activate_command },
	{"admin_activate_openproject",(caddr_t)admin_activate_openproject },
	{"admin_activate_createproject",(caddr_t)admin_activate_createproject },
	{"admin_activate_deleteproject",(caddr_t)admin_activate_deleteproject },
	{"admin_activate_copyproject",(caddr_t)admin_activate_copyproject },
	{"admin_activate_moveproject",(caddr_t)admin_activate_moveproject },
	{"admin_activate_renameproject",(caddr_t)admin_activate_renameproject },
	{"admin_activate_projectprop",(caddr_t)admin_activate_projectprop },
	{"admin_activate_volumeprop",(caddr_t)admin_activate_volumeprop },
	{"admin_activate_newvolume",(caddr_t)admin_activate_newvolume },
	{"admin_activate_removevolume",(caddr_t)admin_activate_removevolume },
	{"admin_activate_userprop",(caddr_t)admin_activate_userprop },
	{"admin_activate_newuser",(caddr_t)admin_activate_newuser },
	{"admin_activate_removeuser",(caddr_t)admin_activate_removeuser },
	{"admin_activate_systemprop",(caddr_t)admin_activate_systemprop },
	{"admin_activate_newsystem",(caddr_t)admin_activate_newsystem },
	{"admin_activate_removesystem",(caddr_t)admin_activate_removesystem },
	{"admin_activate_viewprojects",(caddr_t)admin_activate_viewprojects },
	{"admin_activate_viewvolumes",(caddr_t)admin_activate_viewvolumes },
	{"admin_activate_viewusers",(caddr_t)admin_activate_viewusers },
	{"admin_activate_zoomin",(caddr_t)admin_activate_zoomin },
	{"admin_activate_zoomout",(caddr_t)admin_activate_zoomout },
	{"admin_activate_zoomreset",(caddr_t)admin_activate_zoomreset },
	{"admin_activate_help",(caddr_t)admin_activate_help },
	{"admin_create_msg_label",(caddr_t)admin_create_msg_label },
	{"admin_create_cmd_prompt",(caddr_t)admin_create_cmd_prompt },
	{"admin_create_cmd_input",(caddr_t)admin_create_cmd_input },
	{"admin_create_adminnav_form",(caddr_t)admin_create_adminnav_form },
	{"adminprop_activate_ok",(caddr_t)adminprop_activate_ok },
	{"adminprop_activate_cancel",(caddr_t)adminprop_activate_cancel },
	{"adminprop_create_projnamevalue",(caddr_t)adminprop_create_projnamevalue },
	{"adminprop_create_basevalue",(caddr_t)adminprop_create_basevalue },
	{"adminprop_create_pathvalue",(caddr_t)adminprop_create_pathvalue },
	{"adminprop_create_hiervalue",(caddr_t)adminprop_create_hiervalue },
	{"adminprop_create_descriptvalue",(caddr_t)adminprop_create_descriptvalue },
	{"adminprop_create_projna_noedit",(caddr_t)adminprop_create_projna_noedit },
	{"adminprop_create_base_noedit",(caddr_t)adminprop_create_base_noedit },
	{"adminprop_create_path_noedit",(caddr_t)adminprop_create_path_noedit },
	{"adminprop_create_hier_noedit",(caddr_t)adminprop_create_hier_noedit },
	{"adminprop_create_descr_noedit",(caddr_t)adminprop_create_descr_noedit },
	{"adminuser_activate_ok",(caddr_t)adminuser_activate_ok },
	{"adminuser_activate_cancel",(caddr_t)adminuser_activate_cancel },
	{"adminuser_create_usernamevalue",(caddr_t)adminuser_create_usernamevalue },
	{"adminuser_create_passwordvalue",(caddr_t)adminuser_create_passwordvalue },
	{"adminuser_create_systemvalue",(caddr_t)adminuser_create_systemvalue },
	{"adminuser_create_userna_noedit",(caddr_t)adminuser_create_userna_noedit },
	{"adminuser_create_system_noedit",(caddr_t)adminuser_create_system_noedit },
	{"adminuser_create_rtread",(caddr_t)adminuser_create_rtread },
	{"adminuser_create_rtwrite",(caddr_t)adminuser_create_rtwrite },
	{"adminuser_create_rtevents",(caddr_t)adminuser_create_rtevents },
	{"adminuser_create_system",(caddr_t)adminuser_create_system },
	{"adminuser_create_maintenance",(caddr_t)adminuser_create_maintenance },
	{"adminuser_create_process",(caddr_t)adminuser_create_process },
	{"adminuser_create_instrument",(caddr_t)adminuser_create_instrument },
	{"adminuser_create_operator1",(caddr_t)adminuser_create_operator1 },
	{"adminuser_create_operator2",(caddr_t)adminuser_create_operator2 },
	{"adminuser_create_operator3",(caddr_t)adminuser_create_operator3 },
	{"adminuser_create_operator4",(caddr_t)adminuser_create_operator4 },
	{"adminuser_create_operator5",(caddr_t)adminuser_create_operator5 },
	{"adminuser_create_operator6",(caddr_t)adminuser_create_operator6 },
	{"adminuser_create_operator7",(caddr_t)adminuser_create_operator7 },
	{"adminuser_create_operator8",(caddr_t)adminuser_create_operator8 },
	{"adminuser_create_operator9",(caddr_t)adminuser_create_operator9 },
	{"adminuser_create_operator10",(caddr_t)adminuser_create_operator10 },
	{"adminuser_create_devread",(caddr_t)adminuser_create_devread },
	{"adminuser_create_devplc",(caddr_t)adminuser_create_devplc },
	{"adminuser_create_devconfig",(caddr_t)adminuser_create_devconfig },
	{"adminuser_create_devclass",(caddr_t)adminuser_create_devclass },
	{"adminsys_activate_ok",(caddr_t)adminsys_activate_ok },
	{"adminsys_activate_cancel",(caddr_t)adminsys_activate_cancel },
	{"adminsys_create_systemnamevalue",(caddr_t)adminsys_create_systemnamevalue },
	{"adminsys_create_systemna_noedit",(caddr_t)adminsys_create_systemna_noedit },
	{"adminsys_create_userinherit",(caddr_t)adminsys_create_userinherit },
	{"adminvol_activate_ok",(caddr_t)adminvol_activate_ok },
	{"adminvol_activate_cancel",(caddr_t)adminvol_activate_cancel },
	{"adminvol_create_volumenamevalue",(caddr_t)adminvol_create_volumenamevalue },
	{"adminvol_create_volumena_noedit",(caddr_t)adminvol_create_volumena_noedit },
	{"adminvol_create_volumeidvalue",(caddr_t)adminvol_create_volumeidvalue },
	{"adminvol_create_volumeid_noedit",(caddr_t)adminvol_create_volumeid_noedit },
	{"adminvol_create_projnamevalue",(caddr_t)adminvol_create_projnamevalue },
	{"adminvol_create_projna_noedit",(caddr_t)adminvol_create_projna_noedit }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  dcli_translate_filename( uid_filename, uid_filename);

  // Create object context
//  attrctx->close_cb = close_cb;
//  attrctx->redraw_cb = redraw_cb;

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Project Adiministrator");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);


  parent_wid = XtCreatePopupShell("projectAdministrator",
		topLevelShellWidgetClass, parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, "admin_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "adminprop_dia", parent_wid,
		&adminprop_dia, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "adminuser_dia", parent_wid,
		&adminuser_dia, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "adminsys_dia", parent_wid,
		&adminsys_dia, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "adminvol_dia", parent_wid,
		&adminvol_dia, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  MrmCloseHierarchy(s_DRMh);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
						actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( toplevel, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,420);i++;
  XtSetArg(args[i],XmNheight,700);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);
  XtUnmanageChild( cmd_input);

  adminnav = new AdminNav( (void *)this, adminnav_form, "Plant",
		&brow_widget, &sts);
  ((AdminNav *)adminnav)->message_cb = &admin_message_cb;

  set_title( "PwR Project Administrator");

  XtUnmanageChild( volume_entry);
  XtUnmanageChild( user_entry);
  XmToggleButtonSetState( menu_project, 1, 0);
  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)admin_activate_exit, this);

}









