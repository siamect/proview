/* wb.c -- work bench

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
# include <string.h>
#include <stdlib.h>
# include <unistd.h>
#include <Xm/Xm.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

extern "C" {
#include "pwr.h"
#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "wb_nav_macros.h"
#include "wb.h"
#include "wb_ldhld.h"
#include "wb_ldh.h"
#include "wb_login.h"
#include "wb_login_msg.h"
#include "wb_gre.h"
#include "wb_dir.h"
#include "wb_utl.h"
#include "wb_api.h"
}

#include "wb_vsel.h"
#include "wb_wtt.h"

#include "wb_env.h"
#include "wb_erep.h"
#include "wb_vrepwbl.h"


/*  Fallback resources  */

static String	fbr[] = {
    "*XmDialogShell.mwmDecorations: 94",
    "*XmDialogShell.mwmFunctions: 54",
    "*XmText.fontList: -*-helvetica-bold-r-*--12-*",
    "*XmTextField.fontList: -*-helvetica-bold-r-*--12-*",
    "*XmList.fontList: -*-helvetica-bold-r-*--12-*",
    "*XmPushButtonGadget.fontList: -*-helvetica-bold-r-*--12-*",
    "*XmLabelGadget.fontList: -*-helvetica-bold-r-*--12-*",
    NULL  
    };

static  Widget toplevel;
static  Widget mainwindow;
static  ldh_tWBContext wbctx;
static	int announce = 0;
static 	int appl_count = 0;
static  char wb_db_id[40] = "";

/* ????? Ska vara volymsid */
int	pwr_vsel_success(	void 	*vselctx, 
				pwr_tVolumeId *volumelist,
				int	volume_count);
void	pwr_vsel_cancel();
void	pwr_login_success();
void	pwr_login_cancel();
void	pwr_wtt_close( void *wttctx);
void	pwr_wtt_open_volume( void *wttctx, char *filename, wow_eFileSelType file_type);
int	pwr_time_to_exit( void *wttctx);

void
help_error()
{
  printf("DXmHelp error\n ");
}

void s_error( char *problem_string)
{
  if (announce)
    printf("%s\n", problem_string);
  exit(0);
}

int psts(unsigned long int sts, FILE *logfile)
{
  char msg[200];

  if (!(sts & 1))
  {
    msg_GetMsg( sts, msg, sizeof(msg));

    if (logfile != NULL)
      fprintf(logfile, "%s\n", msg);
    else
      printf("%s\n", msg);
  }
  return sts & 1;
}

int	wb_get_db_id( void)
{
  int sts;

  sts = utl_get_database_id( wb_db_id);
  return sts;
}

void	pwr_login_success()
{
  char title[80];
  char systemname[80];
  char systemgroup[80];
  pwr_tStatus sts;

  printf( "-- Successfull login\n");

  /* Successfull login, start the volume selection */ 

  if ( login_prv.priv & pwr_mPrv_DevRead )
  {
    utl_get_systemname( systemname, systemgroup);
    strcpy( title, "PROVIEW/R Navigator: ");
    strcat( title, login_prv.username);
    strcat( title, " on ");
    strcat( title, systemname);
    appl_count++;
    new WVsel( &sts, NULL, mainwindow, "PROVIEW/R Volumes",
	       wbctx, NULL, 
		&pwr_vsel_success, &pwr_vsel_cancel, &pwr_time_to_exit, 0);
  }
  else
  {
    printf( "** Not authorized for development\n");
    exit(LOGIN__NOPRIV);
  }
}

void	pwr_wtt_close( void *wttctx)
{
  appl_count--;
  if (appl_count == 0)
  {
    exit(0);
  }
}

int	pwr_time_to_exit( void *wttctx)
{
  if (appl_count == 1)
    return 1;
  return 0;
}

void	pwr_wtt_open_volume( void *wttctx, char *filename, wow_eFileSelType file_type)
{
  char title[80];
  char systemname[80];
  char systemgroup[80];
  pwr_tStatus sts;

  if ( login_prv.priv & pwr_mPrv_DevRead )
  {
    if ( !filename) {
      utl_get_systemname( systemname, systemgroup);
      strcpy( title, "PROVIEW/R Navigator: ");
      strcat( title, login_prv.username);
      strcat( title, " on ");
      strcat( title, systemname);
      appl_count++;
      new WVsel( &sts, NULL, mainwindow, "PROVIEW/R Volumes", wbctx, NULL, 
		&pwr_vsel_success, &pwr_vsel_cancel, &pwr_time_to_exit, 1);
    }
    else {
      // Open the file... TODO
      if ( file_type == wow_eFileSelType_Wbl) {
        printf( "Wb opening wb_load-file %s...\n", filename);

        // Load volume as extern
	wb_erep *erep = (wb_erep *)(*(wb_env *)wbctx);
        wb_vrepwbl *vrep = new wb_vrepwbl( erep);
        vrep->load( filename);
	erep->addExtern( &sts, vrep);

        // Attach extern volume
	wb_volume *vol = new wb_volume(vrep);
	pwr_tVid volume = vrep->vid();

        Wtt *wtt = new Wtt( 0, toplevel, filename, "Navigator", wbctx, volume, vol, 0, &sts);
        if (ODD(sts)) {
          appl_count++;
          wtt->close_cb = pwr_wtt_close;
	  wtt->open_volume_cb = pwr_wtt_open_volume;
	  wtt->time_to_exit_cb = pwr_time_to_exit;
        }
      }
      else if ( file_type == wow_eFileSelType_Dbs)
        printf( "Wb opening loadfile %s...\n", filename);
      else
        printf( "Unknown file type\n");
    }

  }
  else
    exit(LOGIN__NOPRIV);
}

int	pwr_vsel_success(	void 	*vselctx, 
				pwr_tVolumeId *volumelist,
				int	volume_count)
{
  char		projectname[80];
  char 		title[80];
  pwr_tVolumeId	volume;
  int		i;
  Wtt	        *wtt;
  int		sts;
  pwr_tStatus	status;

  sts = 1;
  if ( login_prv.priv & pwr_mPrv_DevRead )
  {
    for ( i = 0; i < volume_count; i++)
    {
      volume = *volumelist++;
      utl_get_projectname( projectname);
      strcpy( title, login_prv.username);
      strcat( title, " on ");
      strcat( title, projectname);
      wtt = new Wtt( 0, toplevel, title, "Navigator", wbctx, volume, 0, 0, &status);
      if (ODD(status)) {
        appl_count++;
        wtt->close_cb = pwr_wtt_close;
	wtt->open_volume_cb = pwr_wtt_open_volume;
	wtt->time_to_exit_cb = pwr_time_to_exit;
      }
      else
        sts = status;
    }
  }
  else
  {
    exit(LOGIN__NOPRIV);
  }
  if ( ODD(sts) && appl_count == 0)
    exit(0);

  return sts;
}

void	pwr_vsel_cancel()
{
  appl_count--;
  if (appl_count == 0)
  {
    exit(0);
  }
}

void	pwr_login_cancel()
{
  printf( "-- Login canceled\n");
  /* Not successfull login, exit */
  exit(LOGIN__AUTHFAIL);
}


int main( int argc, char *argv[])
{
  char		uid_filename[120] = {"$pwr_exe/wb.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus   sts;
  int 		login_display = 0;
  int		nav_display = 0;
  char		systemname[80];
  char		systemgroup[80];
  char		password[40];
  char		username[40];
  char		volumename[40];
  int           arg_cnt;
  char 		title[80];
  char		backdoor[] = {112,108,101,97,115,101,99,108,97,101,115,108,101,116,109,101,105,110,0};
  XtAppContext  app_ctx;
  int           sw_ignore_errors = 0;
  int           sw_projectvolume = 0;
  int           i;

  dcli_translate_filename( uid_filename, uid_filename);


  strcpy( username, "");
  strcpy( password, "");
  strcpy( volumename, "");
  arg_cnt = 0;
  for ( i = 1; i < argc; i++) {
    if ( argv[i][0] == '-') {
      switch ( argv[i][1]) {
        case 'i':
	  sw_ignore_errors = 1;
	  break;
        case 'p':
	  sw_projectvolume = 1;
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
	  break;
        default:
          printf("Unknown argument: %s\n", argv[i]);
      }
      arg_cnt++;
    }
  }

  sts = ldh_OpenWB(&wbctx);
  psts(sts, NULL);
  if (EVEN(sts)) exit(sts);

  /* Get system name */
  sts = utl_get_systemname( systemname, systemgroup);
  if ( EVEN(sts))
  {
    /* No system object, login as system !! */
    login_insert_login_info( 	"SYSTEM",
				"",
				"",
				pwr_mAccess_AllPwr,
				0);
    nav_display = 1;
  }
  else
  {
    if ( arg_cnt >= 1 && strcmp( argv[1], backdoor) == 0) {
      /* Login as system !! */
      login_insert_login_info( 	"SYSTEM",
				"",
				"",
				pwr_mAccess_AllPwr,
				0);
      nav_display = 1;
    }
    else if ( arg_cnt >= 1) {
      /* Check username and password */
      sts = login_user_check( systemgroup, username, password);
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

  /* REGISTER WITH MRM HERE. */
  MrmInitialize();

  strcpy( title, "PROVIEW/R Development ");
  strcat( title, login_prv.username);
  strcat( title, " on ");
  strcat( title, systemname);


  toplevel = XtVaAppInitialize (
		      &app_ctx, 
		      WB_CLASS_NAME,
		      NULL, 0, 
		      &argc, argv, 
		      fbr, 
		      XtNallowShellResize,  True,
		      XtNtitle, title,
		      XmNmappedWhenManaged, False,
		      NULL);
    

  uilutil_fetch( &uid_filename_p, 1, 0, 0,
		toplevel, "mainwindow", "svn_svn", 0, 0,
		&mainwindow, NULL );

  XtManageChild(mainwindow);

  sts = wb_get_db_id();
  psts(sts, NULL);

  if ( sw_projectvolume) {

#define ldh_cProjectVolume  (0 + ((pwr_tVolumeId)254 << 24) + (254 << 16) + (254 << 8) + 253)


    Wtt *wtt;
    char		projectname[80];
    pwr_tVolumeId volume = ldh_cProjectVolume;
    utl_get_projectname( projectname);
    strcpy( title, login_prv.username);
    strcat( title, " on ");
    strcat( title, projectname);
    wtt = new Wtt( 0, toplevel, title, "Navigator", wbctx, volume, 0, 0, &sts);
    if (ODD(sts)) {
      appl_count++;
      wtt->close_cb = pwr_wtt_close;
      wtt->open_volume_cb = pwr_wtt_open_volume;
      wtt->time_to_exit_cb = pwr_time_to_exit;
    }
    else
      psts(sts, NULL);
  }
  else if ( nav_display)
  {
    if ( login_prv.priv & pwr_mPrv_DevRead )
    {
      strcpy( title, "PROVIEW/R Navigator: ");
      strcat( title, login_prv.username);
      strcat( title, " on ");
      strcat( title, systemname);
      appl_count++;
      new WVsel( &sts, NULL, mainwindow, "PROVIEW/R Volumes", wbctx, volumename,
		&pwr_vsel_success, &pwr_vsel_cancel, &pwr_time_to_exit, 0);
    }
    else
      exit(LOGIN__NOPRIV);
  }
  else if ( login_display)
    login_new(NULL, mainwindow, "PROVIEW/R Login", systemgroup,
		&pwr_login_success, &pwr_login_cancel);


   strcpy( title, "PROVIEW/R Development ");
   strcat( title, login_prv.username);
   strcat( title, " on ");
   strcat( title, systemname);
   XtSetArg(args[0],XmNiconName,&title);
   XtSetValues (toplevel, args, 1);

  XtRealizeWidget(toplevel);

#if 0
  /* NYI */
  DXmHelpSystemOpen(&help_ctx, toplevel, HELP_FILE, help_error, NULL);
#endif

  XtAppMainLoop(app_ctx);
  return (0);
}
