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


