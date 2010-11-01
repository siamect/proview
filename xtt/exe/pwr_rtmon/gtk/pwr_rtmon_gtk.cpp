/* 
 * Proview   $Id: pwr_rtmon_gtk.cpp,v 1.2 2008-10-31 12:51:30 claes Exp $
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

/* pwr_rtmonitor_gtk.cpp -- Runtime Monitor */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "cow_rtmon_gtk.h"
#include "cow_xhelp_gtk.h"

static void usage()
{
  printf("\nUsage: pwr_rtmonitor\n");
}

static void rtmon_help_cb( void *ctx, const char *topic)
{
  CoXHelp::dhelp( "index", "", navh_eHelpFile_Other, "$pwr_lang/rtmon_help.dat", 0);
}

static void rtmon_close_cb( void *ctx)
{
  exit(0);
}

int main( int argc, char *argv[])
{
  int sts;
  char display[80] = "";

  if ( argc > 1) {
    for ( int i = 1; i < argc; i++) {
      if ( strcmp( argv[i], "-h") == 0) {
	usage();
	exit(0);
      }
      else if ( strcmp( argv[i], "--display") == 0 && i + 1 < argc) {
	strncpy( display, argv[i+1], sizeof(display));
      }
    }
  }

  gtk_init( &argc, &argv);

  setlocale( LC_NUMERIC, "POSIX");
  setlocale( LC_TIME, "en_US");

  // Create help window
  CoXHelp *xhelp = new CoXHelpGtk( 0, 0, xhelp_eUtility_Xtt, &sts);
  CoXHelp::set_default( xhelp);

  RtMon *rtmon = new RtMonGtk( NULL, NULL, "Runtime Monitor", display, &sts);
  rtmon->help_cb = rtmon_help_cb;
  rtmon->close_cb = rtmon_close_cb;
  
  gtk_main();
  return (0);
}
