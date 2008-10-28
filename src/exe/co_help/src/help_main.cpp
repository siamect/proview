/* 
 * Proview   $Id: help_main.cpp,v 1.1 2008-10-28 09:36:12 claes Exp $
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

/* rt_xtt.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_help_main.h"
#include "co_xhelp.h"
#include "co_lng.h"
#include "co_msg.h"

static void usage()
{
  cout << "co_help Proview Help Window" << endl << endl <<
    "Usage:" << endl << endl <<
    "  co_help [-t 'topic'] [-s 'sourcefile'] [-b 'bookmark']" << endl << endl <<
    "Arguments:" << endl << endl <<
    "  -t    Help topic, default 'index'" << endl <<
    "  -s    Source helpfile"  << endl <<
    "  -b    Bookmark" << endl <<
    "  -l    Language, e.g sv_se" << endl <<
    "  -c    Open Configuration help" << endl <<
    "  -d    Open Designer's Guide" << endl <<
    "  -g    Open Ge Reference Manual" << endl <<
    "  -o    Open Operator Help" << endl << endl;
}

void CoHelpMain::exec_help()
{
  pwr_tStatus sts;

  sts = CoXHelp::dhelp( topic, bookmark, type, helpfile, strict);
  if ( EVEN(sts)) {
    char msg[80];
    msg_GetText( sts, msg, sizeof(msg));
    printf( "%s\n", msg);    
    exit(0);
  }
}

CoHelpMain::CoHelpMain( int argc, char *argv[], int *return_sts) :
  strict(0), type( navh_eHelpFile_Project)
{
  strcpy( topic, "index");
  strcpy( helpfile, "");
  strcpy( bookmark, "");


  // Set language
  for ( int i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-l") == 0 && i + 1 < argc) {
      Lng::set( argv[i+1]);
      i++;
    }
    else if ( strcmp( argv[i], "-t") == 0 && i + 1 < argc) {
      strcpy( topic, argv[i+1]);
      i++;
    }
    else if ( strcmp( argv[i], "-s") == 0 && i + 1 < argc) {
      type = navh_eHelpFile_Other;
      strcpy( helpfile, argv[i+1]);
      i++;
    }
    else if ( strcmp( argv[i], "-b") == 0 && i + 1 < argc) {
      strcpy( bookmark, argv[i+1]);
      i++;
    }
    else if ( strcmp( argv[i], "-c") == 0) {
      type = navh_eHelpFile_Other;
      if ( strcmp(topic, "index") == 0)
	strcpy( topic, "overview");
      strcpy( helpfile, "$pwr_lang/wtt_help.dat");
    }
    else if ( strcmp( argv[i], "-d") == 0) {
      type = navh_eHelpFile_Other;
      strcpy( helpfile, "$pwr_lang/man_dg.dat");
    }
    else if ( strcmp( argv[i], "-p") == 0) {
      type = navh_eHelpFile_Project;
    }
    else if ( strcmp( argv[i], "-o") == 0) {
      type = navh_eHelpFile_Other;
      if ( strcmp(topic, "index") == 0)
	strcpy( topic, "overview");
      strcpy( helpfile, "$pwr_exe/xtt_help.dat");
    }
    else if ( strcmp( argv[i], "-g") == 0) {
      type = navh_eHelpFile_Other;
      strcpy( helpfile, "$pwr_lang/man_geref.dat");
    }
    else if ( strcmp( argv[i], "-h") == 0) {
      usage();
      exit(0);
    }
  }

}




