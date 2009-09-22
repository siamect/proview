/* 
 * Proview   $Id: wb_ge_motif.cpp,v 1.3 2008-10-31 12:51:32 claes Exp $
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

/* wb_ge.c -- graphical editor */

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef void *ldh_tSesContext;

#include "ge_motif.h"
#include "co_lng.h"
#include "co_xhelp_motif.h"


/*  Fallback resources  */

static String	fbr[] = {
    NULL  
    };

static  Widget toplevel;
static  Widget mainwindow;

static void usage()
{
  printf("\nUsage: wb_ge [-l language] [graphname]\n");
}

#if 0
static void ge_close_cb( void *gectx)
{
  delete (Ge *)gectx;
  exit(0);
}
#endif

int main( int argc, char *argv[])
{
  char		uid_filename[120] = {"pwr_exe:wb.uid"};
  char		*uid_filename_p = uid_filename;
  unsigned long sts;
  char 		title[80];
  XtAppContext  app_ctx;
  MrmHierarchy 	s_DRMh;
  MrmType 	dclass;
  char		name[] = "Proview/R Ge";
  char		graph_name[80];
  char		file[120];
  int		i;
  unsigned int opt = ge_mOption_EnableComment;
  
#if !defined OS_VMS
  {
    char *s;

    if ( (s = getenv( "pwr_exe")) == NULL)
    {
      printf( "** pwr_exe is not defined\n");
      exit(0);
    }
    sprintf( uid_filename, "%s/wb.uid", s);
  }
#endif

  strcpy( title, "PROVIEW/R Ge ");

  MrmInitialize();

  toplevel = XtVaAppInitialize (
		      &app_ctx, 
		      "mainwindow",
		      NULL, 0, 
		      &argc, argv, 
		      fbr, 
		      XtNallowShellResize,  True,
		      XtNtitle, title,
		      XmNmappedWhenManaged, False,
		      NULL);
    

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "mainwindow", toplevel,
			name, NULL, 0, &mainwindow, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

  XtManageChild(mainwindow);

  // Create help window
  CoXHelpMotif *xhelp = new CoXHelpMotif( mainwindow, 0, xhelp_eUtility_Wtt, (int *)&sts);
  CoXHelp::set_default( xhelp);

  if ( argc > 1) {

    for ( i = 1; i < argc; i++) {
      if ( strcmp( argv[i], "-l") == 0) {
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	Lng::set( argv[i+1]);
	i++;
      }
      else
	strcpy( file, argv[i]);
    }

    printf( "Arg: %s\n", file);
    if ( file[0] == '@') {
      // Execute script
      Ge *gectx;
      pwr_tStatus sts;

      gectx = new GeMotif( NULL, mainwindow, 0, 1, 0, NULL);
      sts = gectx->command( file);
      if ( EVEN(sts))
	gectx->message( sts);
    }
    else {
      

      // Open graph
      strcpy( graph_name, file);
      new GeMotif( NULL, mainwindow, 0, 1, opt, graph_name);
    }
  }
  else
    new GeMotif( NULL, mainwindow, 0, 1, opt, NULL);

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_ctx);
  return (0);
}
