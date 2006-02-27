/* 
 * Proview   $Id: cnv_changelog.cpp,v 1.1 2006-02-27 06:17:41 claes Exp $
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

#include <functional>
#include <algorithm>
#include <iostream.h>
#include <fstream.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_time.h"
}
#include "cnv_changelog.h"


CnvChangeLog::CnvChangeLog( CnvCtx *cnv_ctx) :
  ctx(cnv_ctx)
{
  read( "src");
  read( "xtt");
  read( "wb");
  read( "nmps");
  read( "remote");
  read( "profibus");
  read( "java");
  read( "tlog");
  read( "bcomp");
  read( "othermanu");
  read( "abb");
  read( "siemens");
  read( "ssabox");
  read( "telemecanique");
  read( "inor");
  read( "klocknermoeller");
  print();
}

int CnvChangeLog::read( char *module)
{
  int sts;
  char orig_line[400];
  char line[400];
  char timstr1[40];
  char timstr2[40];
  FILE *fp;
  int i;
  pwr_tFileName fname;
  char *s;

  sprintf( fname, "$pwre_croot/%s/changelog.txt", module);
  dcli_translate_filename( fname, fname);
  fp = fopen( fname, "r");
  if ( !fp)
    return 0;

  while( 1) {
    sts = CnvCtx::read_line( orig_line, sizeof(orig_line), fp);
    if ( !sts)
      break;
    else {
      if ( isdigit(orig_line[0])) {
	LogEntry p;

	// New log entry
	i = 0;
	for ( s = orig_line; *s && *s != 32 && *s != 9; s++) {
	  if ( i >= (int)sizeof(timstr1))
	    break;
	  timstr1[i++] = *s;
	}
	timstr1[i] = 0;
	  
	for ( ; *s && (*s == 32 || *s == 9); s++) ;

	i = 0;
	for ( ; *s && *s != 32 && *s != 9; s++) {
	  if ( i >= (int)sizeof(p.signature))
	    break;
	  p.signature[i++] = *s;
	}
	p.signature[i] = 0;

	for ( ; *s && (*s == 32 || *s == 9); s++) ;

	i = 0;
	for ( ; *s && *s != 32 && *s != 9; s++) {
	  if ( i >= (int)sizeof(p.component))
	    break;
	  p.component[i++] = *s;
	}
	p.component[i] = 0;

	for ( ; *s && (*s == 32 || *s == 9); s++) ;

	strncpy( p.text, s, sizeof(p.text));
	strncpy( p.module, module, sizeof(p.module));

	sprintf( timstr2, "20%c%c-%c%c-%c%c 00:00", timstr1[0], timstr1[1],
		 timstr1[2], timstr1[3], timstr1[4], timstr1[5]);
	time_FormAsciiToA( timstr2, MINUTE, 0, &p.time);

	entries.push_back( p);
      }
      else {
	// Continuation of log entry
	CnvCtx::remove_spaces( orig_line, line);
	if ( strcmp( line, "") == 0)
	  continue;

	if ( entries.size() == 0)
	  continue;
	strcat( entries[entries.size()-1].text, " ");
	strcat( entries[entries.size()-1].text, line);
      }
    }
  }
  fclose( fp);

  return 1;
}

void CnvChangeLog::sort_time()
{
  int n = entries.size();

  for ( int gap = n/2; 0 < gap; gap /= 2) {
    for ( int i = gap; i < n; i++) {
      for ( int j = i - gap; 0 <= j; j -= gap) {
	if ( entries[j+gap].time.tv_sec < entries[j].time.tv_sec) {
	  LogEntry temp = entries[j];
	  entries[j] = entries[j+gap];
	  entries[j+gap] = temp;
	}
      }
    }
  }
}

void CnvChangeLog::sort_module()
{
  int n = entries.size();

  for ( int gap = n/2; 0 < gap; gap /= 2) {
    for ( int i = gap; i < n; i++) {
      for ( int j = i - gap; 0 <= j; j -= gap) {
	if ( strcmp( entries[j+gap].module, entries[j].module) > 0) {
	  LogEntry temp = entries[j];
	  entries[j] = entries[j+gap];
	  entries[j+gap] = temp;
	}
      }
    }
  }
}

void CnvChangeLog::sort_component()
{
  int n = entries.size();

  for ( int gap = n/2; 0 < gap; gap /= 2) {
    for ( int i = gap; i < n; i++) {
      for ( int j = i - gap; 0 <= j; j -= gap) {
	if ( strcmp( entries[j+gap].component, entries[j].component) > 0) {
	  LogEntry temp = entries[j];
	  entries[j] = entries[j+gap];
	  entries[j+gap] = temp;
	}
      }
    }
  }
}

void CnvChangeLog::sort_signature()
{
  int n = entries.size();

  for ( int gap = n/2; 0 < gap; gap /= 2) {
    for ( int i = gap; i < n; i++) {
      for ( int j = i - gap; 0 <= j; j -= gap) {
	if ( strcmp( entries[j+gap].signature, entries[j].signature) > 0) {
	  LogEntry temp = entries[j];
	  entries[j] = entries[j+gap];
	  entries[j+gap] = temp;
	}
      }
    }
  }
}

void CnvChangeLog::print()
{
  char timstr1[40];

  sort_time();
  for ( int i = 0; i < (int) entries.size(); i++) {
    time_AtoAscii( &entries[i].time, time_eFormat_DateAndTime, timstr1, sizeof(timstr1));
    timstr1[11] = 0;
    
    printf( "%s %4s %-8s %-8s %s\n", timstr1, entries[i].signature, entries[i].module, 
	    entries[i].component, entries[i].text);
  }
}


