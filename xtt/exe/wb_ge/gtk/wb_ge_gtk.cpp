/* 
 * Proview   $Id: wb_ge_gtk.cpp,v 1.1 2007-01-04 08:41:00 claes Exp $
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

/* wb_ge_gtk.cpp -- graphical editor */

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"

#include <gtk/gtk.h>

typedef void *ldh_tSesContext;

#include "ge_gtk.h"
#include "co_lng.h"
#include "co_xhelp_gtk.h"


/*  Fallback resources  */

static  GtkWidget *toplevel;
static  GtkWidget *mainwindow;

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
  int i;
  pwr_tFileName file;
  char graph_name[80];
  int sts;

  gtk_init( &argc, &argv);

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW,
					 "default-height", 100,
					 "default-width", 100,
					 "title", "Pwr wb_ge",
					 NULL);


  // Create help window
  CoXHelpGtk *xhelp = new CoXHelpGtk( mainwindow, 0, xhelp_eUtility_Wtt, (int *)&sts);
  CoXHelpGtk::set_default( xhelp);

  printf( "Argc: %d\n", argc);
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

      gectx = new GeGtk( NULL, toplevel, 0, 1, NULL);
      sts = gectx->command( file);
      if ( EVEN(sts))
	gectx->message( sts);
    }
    else {
      

      // Open graph
      strcpy( graph_name, file);
      new GeGtk( NULL, mainwindow, 0, 1, graph_name);
    }
  }
  else
    new GeGtk( NULL, mainwindow, 0, 1, NULL);

  gtk_widget_show_all( toplevel);
  g_object_set( toplevel, "visible", FALSE, NULL);

  gtk_main();
  return (0);
}
