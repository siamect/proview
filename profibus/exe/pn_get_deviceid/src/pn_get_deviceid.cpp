/** 
 * Proview   $Id$
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
 **/

//
// Search for DeviceID and VendorID in gsdml files and extract ProductFamly and TextInfo.
// The content should be written in $pwr_exe/profinet_deviceid.dat which is used by the
// Proview Viewer to show info for a device.
//


#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwr.h"
#include "co_dcli.h"

using namespace std;

void parse_file( char *filename);

int main( int argc, char *argv[])
{
  pwr_tFileName file_spec, found_file;
  
  pwr_tStatus sts;
  

  if ( argc < 2 || strcmp( argv[1], "-h") == 0) {
    printf( "\nUsage: pn_get_deviceid \"file-pattern\" > profinet_deviceid.dat\n\n");
    exit(0);
  }

  dcli_translate_filename( file_spec, argv[1]);

  sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {

    printf("# Processing file: %s\n", found_file);
    parse_file( found_file);

    sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
  }  
  dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_END);

}

void parse_file( char *filename)
{
  ifstream fp;
  pwr_tFileName fname;
  char line[1024];
  unsigned int deviceid;
  unsigned int vendorid;
  char infotextid[500] = "";
  char infotext[500] = "";
  char family[500] = "";

  dcli_translate_filename( fname, filename);

  fp.open( fname);
  if ( !fp) {
    printf( "# Unable to open file\n");
    exit(0);
  }

  int in_deviceid = 0;
  int in_devicefunction = 0;
  int in_text = 0;
  int deviceid_found = 0;
  int family_found = 0;
  int vendorid_found = 0;
  int infotextid_found = 0;
  int infotext_found = 0;
  while( fp.getline( line, sizeof(line))) {
    char *s;

    if ( !in_deviceid) {
      if ( (s = strstr( line, "<DeviceIdentity")))
	in_deviceid = 1;
    }

    if ( in_deviceid) {
      if ( !deviceid_found) {
	if ( (s = strstr( line, "DeviceID"))) {
	  for ( s += 9; *s; s++) {
	    if ( *s == '\"') {
	      sscanf( s+3, "%x", &deviceid);
	      deviceid_found = 1;
	      break;
	    }
	  }	
	}
      }
      if ( !vendorid_found) {
	if ( (s = strstr( line, "VendorID"))) {
	  for ( s += 9; *s; s++) {
	    if ( *s == '\"') {
	      sscanf( s+3, "%x", &vendorid);
	      vendorid_found = 1;
	      break;
	    }
	  }	
	}
      }
      if ( !infotextid_found) {
	if ( (s = strstr( line, "<InfoText"))) {
	  for ( s += 16; *s; s++) {
	    if ( *s == '\"') {
	      strncpy( infotextid, s+1, sizeof(infotextid));
	      if ( (s = strchr( infotextid, '\"')))
		*s = 0;
	      infotextid_found = 1;
	      break;
	    }
	  }	
	}
      }
      if ( strstr( line, "</DeviceIdentity>")) {
	in_deviceid = 0;

	printf( "%u %u\n", vendorid, deviceid);
	if ( !deviceid_found)
	  printf( "# DeviceID not found\n");
	if ( !vendorid_found)
	  printf( "# VendorID not found\n");
	if ( !infotextid_found)
	  printf( "# TextId not found\n");
	if ( !(deviceid_found && vendorid_found && infotextid_found))
	  return;
      }
    }

    if ( !in_devicefunction) {
      if ( (s = strstr( line, "<DeviceFunction")))
	in_devicefunction = 1;
    }

    if ( in_devicefunction) {
      if ( !family_found) {
	if ( (s = strstr( line, "ProductFamily"))) {
	  for ( s += 14; *s; s++) {
	    if ( *s == '\"') {
	      strncpy( family, s+1, sizeof(family));
	      if ( (s = strchr( family, '\"')))
		*s = 0;
	      family_found = 1;
	      in_devicefunction = 0;
	      break;
	    }
	  }	
	}
      }
    }
    if ( infotextid_found && !infotext_found) {
      if ( strstr( line, "<Text") && strstr( line, infotextid))
	in_text = 1;
    
      if ( in_text) {
	if ( (s = strstr( line, "Value"))) {
	  for ( s +=6; *s; s++) {
	    if ( *s == '\"') {
	      strncpy( infotext, s+1, sizeof(infotext));
	      if ( (s = strchr( infotext, '\"')))
		*s = 0;
	      infotext_found = 1;
	      in_text = 0;
	      break;
	    }
	  }		  
	}
      }
    }
  }
  printf( "%s\n%s\n", family, infotext);

  fp.close();
}
