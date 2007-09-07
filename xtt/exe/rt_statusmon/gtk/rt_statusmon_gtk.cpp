/* 
 * Proview   $Id: rt_statusmon_gtk.cpp,v 1.5 2007-09-07 06:26:50 claes Exp $
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
#include "co_msgwindow.h"
#include "co_statusmon_nodelist_gtk.h"

static void usage()
{
  printf("\n\
  Proview Status Monitor\n\n\
  rt_statusmon [-m 'n']\n\n\
  -m 'n'  Mode display UserStatus 'n', where 1 <= n <= 5.\n\
  $HOME/rt_statusmon.dat : file with reqested nodes\n");
}

static void statusmon_close( void *ctx)
{
  exit(0);
}

int main( int argc, char *argv[])
{
  int sts;
  int mode = nodelist_eMode_SystemStatus;

  if ( argc > 1) {
    for ( int i = 1; i < argc; i++) {
      if ( strcmp( argv[i], "-h") == 0) {
	usage();
	exit(0);
      }
      else if ( strcmp( argv[i], "-m") == 0) {
	if ( argc == i) {
	  usage();
	  exit(0);
	}
	if ( strcmp( argv[i+1], "1") == 0)
	  mode = nodelist_eMode_Status1;
	else if ( strcmp( argv[i+1], "2") == 0)
	  mode = nodelist_eMode_Status2;
	else if ( strcmp( argv[i+1], "3") == 0)
	  mode = nodelist_eMode_Status3;
	else if ( strcmp( argv[i+1], "4") == 0)
	  mode = nodelist_eMode_Status4;
	else if ( strcmp( argv[i+1], "5") == 0)
	  mode = nodelist_eMode_Status5;
	else {
	  usage();
	  exit(0);
	}
      }
    }
  }

  gtk_init( &argc, &argv);

  Nodelist *nl = new NodelistGtk( NULL, NULL, "Status Monitor", mode, msgw_ePop_No, &sts);
  nl->close_cb = statusmon_close;
  nl->set_scantime(3);
  
  gtk_main();
  return (0);
}
