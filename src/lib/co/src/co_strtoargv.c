/** 
 * Proview   $Id: co_strtoargv.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/* co_strtoargv.c -- Build argv array from string. */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "co_strtoargv.h"

char **
co_StrToArgv (
  char* file,
  char* args
)
{
  int i;
  int argc;
  char **argv;
  char *arg;
  int last_was_white;
  int in_quote = 0;

  if (args == NULL) {
    argv = (char**) malloc(2 * sizeof(void*));
    argv[1] = NULL;
    argv[0] = file;

    return argv;
  }

  /* Get an upper bound on argc. */
  for (argc = 1, arg = args; *arg != '\0'; arg++)
    if (isspace(*arg)) argc++;

  argc += 2;
  argv = (char**) malloc(argc * sizeof(void*) + strlen(args) + 1);
  arg = strcpy((char*)&argv[argc], args);

  for (i = 1; *arg != '\0'; arg++) {
    if (in_quote) {
      if (*arg == '"') {
	*arg = '\0';
	arg++;
	in_quote = 0;
	last_was_white = 1;
      }
    } else if (*arg == '"') {
      in_quote = 1;
      *arg = '\0';
      argv[i++] = arg + 1;
    } else if (isspace(*arg)) {
      *arg = '\0';
      last_was_white = 1;
    } else {
      if (last_was_white) {
	argv[i++] = arg;
	last_was_white = 0;
      }
    }
  }

  argv[i] = NULL;
  argv[0] = file;

  return argv;
}
