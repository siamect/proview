/* 
 * Proview   $Id: wb_upgrade.c,v 1.1 2006-01-12 13:48:59 claes Exp $
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

/* wb_drive_convert.c -- converts plc objects
   Program to convert plc objects from v2.0 to v2.1.

   The program should be executed after loading objects in the
   workbench of v2.1.

   Converts the grapic mask for objects with new attributes.

   In this version the following classes are changed:
      BaseClasses-Drive  */

#include <stdlib.h>
#include <iostream>
#include "pwr.h"
#include "pwr_class.h"
#include "wb_env.h"
#include "wb_volume.h"
#include "wb_session.h"
#include "wb_object.h"
#include "pwr_baseclasses.h"

static void usage()
{
  printf( "\nUsage: wb_upgrade -v 'volumename'\n\n");
}

static void process( wb_session& session, wb_object& o)
{
  pwr_sPlcNode plcnode;

  if ( o.cid() == pwr_cClass_csub) {
    printf ( "Object: %s\n", o.longName().c_str());

    wb_attribute a = session.attribute( o.oid(), "DevBody", "PlcNode");
    if ( !a) exit(0);

    a.value( &plcnode);

    printf( "subwindow:  %d, woid: %u,%u\n", plcnode.subwindow, 
	    plcnode.subwind_oid[0].vid, plcnode.subwind_oid[0].oix);

    wb_object c = o.first();
    if ( c && c.cid() == pwr_cClass_windowplc) {
      plcnode.subwind_oid[0].vid = c.oid().vid;
      plcnode.subwind_oid[0].oix = c.oid().oix;

      printf( "subwindow:  %d, woid: %u,%u\n", plcnode.subwindow, 
	      plcnode.subwind_oid[0].vid, plcnode.subwind_oid[0].oix);

      session.writeAttribute( a, &plcnode, sizeof(plcnode)); 
      if ( !a) cout << "** Write error" << endl;
    }
  }

  for ( wb_object c = o.first(); c.oddSts(); c = c.after()) {
    process( session, c);
  }
}

int main( int argc, char *argv[])
{
  pwr_tObjName volume;

  for ( int i = 1; i < argc; i++) {
    if ( argv[i][0] == '-') {
      switch ( argv[i][1]) {
      case 'h':
	usage();
	exit(0);
      case 'v':
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	strncpy( volume, argv[i+1], sizeof(volume));
	i++;
	break;
      default:
	printf("Unknown argument: %s\n", argv[i]);
	usage();
	exit(0);
      }
    }
  }

  wb_env env;
  env.load( volume);

  wb_volume vol;
  for ( vol = env.volume(); vol.oddSts(); vol.next()) {
    if ( vol.cid() == pwr_eClass_RootVolume ||
	 vol.cid() == pwr_eClass_SubVolume ||
	 vol.cid() == pwr_eClass_SharedVolume) {
      break;
    }
  }
  if ( vol.evenSts()) exit( vol.sts());

  wb_session session(vol);
  if ( ! session.access( ldh_eAccess_ReadWrite)) {
    cout << "** No write access" << endl;
    exit(0);
  }

  wb_object o;
  for ( o = session.object(); o.oddSts(); o = o.after()) {
    process( session, o);
  }
  session.commit();
}

