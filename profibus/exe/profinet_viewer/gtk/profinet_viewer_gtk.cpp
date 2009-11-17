/* 
 * Proview   $Id: wb_ge_gtk.cpp,v 1.2 2007-09-07 06:26:21 claes Exp $
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

/* profinet_viewer.cpp -- Profinet viewer */

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"

#include <gtk/gtk.h>

#include "co_lng.h"
#include "co_error.h"
#include "cow_xhelp_gtk.h"
#include "pn_viewer_gtk.h"


typedef struct {
  GtkWidget *toplevel;
  PnViewer  *viewer;
} tViewer;

static void usage()
{
  printf("\nUsage: profinet_viewer [-l language]\n");
}

static void viewer_close( void *c)
{
  delete ((tViewer *)c)->viewer;
  exit(0);
}

int main( int argc, char *argv[])
{
  int i;
  int sts;
  tViewer *ctx;

  ctx = (tViewer *)calloc( 1, sizeof(tViewer));
  
  gtk_init( &argc, &argv);

  ctx->toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW,
					      "default-height", 100,
					      "default-width", 100,
					      "title", "Profinet Viewer",
					      NULL);


  // Create help window
  CoXHelpGtk *xhelp = new CoXHelpGtk( ctx->toplevel, 0, xhelp_eUtility_Wtt, (int *)&sts);
  CoXHelpGtk::set_default( xhelp);

  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-l") == 0) {
      if ( i+1 >= argc) {
	usage();
	exit(0);
      }
      Lng::set( argv[i+1]);
      i++;
    }
  }

  // Open window
  try {
   ctx->viewer = new PnViewerGtk( ctx, ctx->toplevel, "Profinet Viewer", &sts);
   ctx->viewer->close_cb = viewer_close;
  }
  catch ( co_error &e) {
    printf( "** Exception: %s\n", e.what().c_str());
    exit(0);
  }
  

  gtk_widget_show_all( ctx->toplevel);

  g_object_set( ctx->toplevel, "visible", FALSE, NULL);

  try {
    ctx->viewer->update_devices();
  }
  catch( co_error &e) {
    printf( "** Exception: %s\n", e.what().c_str());
  }

  gtk_main();
  return (0);
}
