/* wb_admin.cpp -- Display object attributes

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

extern "C" {
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
    case adminnav_eItemType_Db:
    {
      ItemDb *item = (ItemDb *)base_item;

      sprintf( cmd, "wb_admin.sh opendb %s \"%s\" \"%s\" %s", item->project, 
		login_prv.username, login_prv.password, item->db);
      system( cmd);      
      break;
    }
    case adminnav_eItemType_Volume:
    {
      ItemVolume *item = (ItemVolume *)base_item;

      sprintf( cmd, "wb_admin.sh opendb %s \"%s\" \"%s\" %s %s", item->project, 
		login_prv.username, login_prv.password, item->db,
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
    admin->message('E', "Not authorized for this operation");
    return;
  }
  admin->create_project();
}

static void admin_activate_deleteproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not authorized for this operation");
    return;
  }
  admin->delete_project();
}

static void admin_activate_copyproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not authorized for this operation");
    return;
  }
  admin->copy_project();
}

static void admin_activate_renameproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not authorized for this operation");
    return;
  }
  admin->project_properties(1,0,0,0,0);
}

static void admin_activate_moveproject( Widget w, Admin *admin, XmAnyCallbackStruct *data)
{
  if ( !admin->edit_mode)
  {
    admin->message('E', "Not authorized for this operation");
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
	clock_cursor(0)
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
	{"admin_activate_exit",(caddr_t)admin_activate_exit },
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
	{"adminprop_create_descr_noedit",(caddr_t)adminprop_create_descr_noedit }
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

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)admin_activate_exit, this);

}

