/* 
 * Proview   $Id: rt_mozilla.cpp,v 1.2 2005-09-01 14:57:48 claes Exp $
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

//
// rt_mozilla
// Mozilla cannot be started by a program that has the suid bit set, if
// the current user is not equal to the original user.
// This program resets to original uid before starting mozilla.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(  int argc, char *argv[])
{
  char cmd[200];
  int sts;
  bool remote = false;

  strcpy( cmd, "mozilla ");
  for ( int i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-remote") == 0) {
      remote = true;
      strcat( cmd, argv[i]);
      strcat( cmd, " \"");
    }
    else {
      strcat( cmd, argv[i]);
      if ( i != argc - 1)
	strcat( cmd, " ");
    }
  }
  if ( remote)
    strcat( cmd, "\"");

  uid_t uid = getuid();

  // printf( "rt_mozilla '%s' uid: %d\n", cmd, uid);

  setuid( uid);
  sts = system( cmd);

  if ( sts)
    exit(1);
  else
    exit(0);
}


