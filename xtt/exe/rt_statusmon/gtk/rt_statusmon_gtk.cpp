/* 
 * Proview   $Id: rt_statusmon_gtk.cpp,v 1.2 2007-05-16 12:36:07 claes Exp $
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

/* rt_statusmon_gtk.cpp -- Status Monitor */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "co_statusmon_nodelist_gtk.h"

static void usage()
{
  printf("\nUsage: rt_statusmonitor\n\n $HOME/rt_statusmon.dat : file with reqested nodes");
}

static void statusmon_close( void *ctx)
{
  exit(0);
}

int main( int argc, char *argv[])
{
  int sts;

  if ( argc > 1) {
    for ( int i = 1; i < argc; i++) {
      if ( strcmp( argv[i], "-h") == 0) {
	usage();
	exit(0);
      }
    }
  }

  gtk_init( &argc, &argv);

  Nodelist *nl = new NodelistGtk( NULL, NULL, "Status Monitor", &sts);
  nl->close_cb = statusmon_close;
  nl->set_scantime(3);
  
  gtk_main();
  return (0);
}
