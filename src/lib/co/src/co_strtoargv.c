/* co_strtoargv.c -- Build argv array from string.

   PROVIEW/R
   Copyright (C) 2000 by Mandator AB.

   .  */
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
