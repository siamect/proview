/* 
 * Proview   $Id: wb_ldlist.cpp,v 1.1 2006-05-11 07:11:22 claes Exp $
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

/* wb_ldlist.c -- 
   Lists load data files.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "pwr.h"
#include "co_time.h"
#include "wb_erep.h"
#include "wb_vrepdbs.h"

static void usage( char *txt)
{
  cout << txt << endl;
}

int main (
  int			argc,
  char			*argv[]
)
{
  int			bflag = 0;
  int			oflag = 0;
  char			c;
  pwr_tTime		time;
  char			timestr[40];
  
  while (--argc > 0 && (*++argv)[0] == '-') {
    while ((c = *++argv[0])) {
      switch (c) {
      case 'o':
	++oflag;
	break;
      case 'b':
	++bflag;
	break;
      default:
	usage("Unknown flag");
      }
    }
  }

  if ((*argv)[0] == '?') usage(0);
  if (argc < 1) usage("No load file given!\n");

  wb_erep *erep = new wb_erep();
  wb_vrepdbs *vrep = new wb_vrepdbs( erep, *argv);

  vrep->load();
  vrep->time( &time);
  time_AtoAscii( &time, time_eFormat_DateAndTime, timestr, sizeof(timestr));

  cout << "Name : " << vrep->name() << endl <<  
    "Time : " << timestr << " (" << time.tv_sec << "," << time.tv_nsec <<  ")" << endl;
  // lf = fopen(*argv, "rb");
}

