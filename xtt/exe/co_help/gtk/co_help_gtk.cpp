/* 
 * Proview   $Id: co_help_gtk.cpp,v 1.1 2008-10-28 09:36:12 claes Exp $
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

/* co_help_gtk.cpp -- Help window */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <gtk/gtk.h>

#include "co_dcli.h"

#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_help_gtk.h"
#include "cow_xhelp_gtk.h"

void CoHelpGtk::close_cb( void *ctx, void *xhelp)
{
  delete (CoXHelpGtk *)xhelp;
  exit(0);
}

void CoHelpGtk::cohelp_mainloop()
{
  gtk_main();
}

int main(  int argc, char *argv[])
{
  int sts;

  gtk_init( &argc, &argv);

  setlocale( LC_NUMERIC, "POSIX");
  setlocale( LC_TIME, "en_US");

  new CoHelpGtk( argc, argv, &sts);
  exit(sts);
}

CoHelpGtk::~CoHelpGtk()
{
}

CoHelpGtk::CoHelpGtk( int argc, char *argv[], int *return_sts) :
  CoHelpMain( argc, argv, return_sts)
{
  pwr_tStatus   sts;
#if 0
  const int	window_width = 400;
  const int    	window_height = 700;
  char		title[120] = "Xtt ";

  // Gtk
  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", title,
					 NULL);
#endif
  // Create help window
  CoXHelp *xhelp = new CoXHelpGtk( 0 /*toplevel*/, this, xhelp_eUtility_Xtt, &sts);
  // xhelp->open_URL_cb = open_URL_cb;
  CoXHelp::set_default( xhelp);
  xhelp->close_cb = close_cb;

  exec_help();

  cohelp_mainloop();  

}






