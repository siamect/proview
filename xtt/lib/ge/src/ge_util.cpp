#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
extern "C" {
#include "co_dcli.h"
#include "rt_load.h"
}


int ge_get_systemname(
  char		*systemname
)
{
  FILE 	*file;
  int	nr;
  char	fname[120];

  dcli_translate_filename( fname, load_cNameSysObject);
  file = fopen( fname, "r");
  if ( file == 0) {
    printf("** Warning, systemname not found\n");
    return 0;
  }

  nr = fscanf( file, "%s", systemname);
  fclose(file);
  if ( nr != 1) return 0;
  return 1;
}

