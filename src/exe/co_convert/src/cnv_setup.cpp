#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "cnv_ctx.h"
#include "cnv_setup.h"

int CnvSetup::setup( char *filename)
{
  int sts;
  char line[400];
  char	line_part[4][80];
  int nr;
  char line_cnt = 0;
  FILE *fp;

  fp = fopen( filename, "r");
  if ( !fp)
    return 0;

  while( 1) {
    sts = CnvCtx::read_line( line, sizeof(line), fp);
    if ( !sts)
      break;
    else {
      line_cnt++;
      CnvCtx::remove_spaces( line, line);
      if ( strcmp( line, "") == 0)
        continue;

      if ( line[0] == '!' || line[0] == '#')
        continue;

      nr = dcli_parse( line, " 	=", "", (char *)line_part,
                	sizeof( line_part) / sizeof( line_part[0]), 
			sizeof( line_part[0]), 0);

      if ( strcmp( CnvCtx::low( line_part[0]), "group") == 0){
	if ( nr < 2) {
	  printf("** Setup syntax error in file %s, line %d\n", filename, line_cnt);
	  continue;
	}
	if ( group_cnt >= (int)(sizeof(groups)/sizeof(groups[0]))) {
	  printf("** Max number of groups exceeded in file %s, line %d\n", filename, line_cnt);
	  continue;
	}
        strcpy( groups[group_cnt], line_part[1]);
	if ( nr >= 3)
	  strcpy( groups_startpage[group_cnt], line_part[2]);
	else
	  strcpy( groups_startpage[group_cnt], "");
	group_cnt++;
      }
    }
  }
  fclose(fp);

  return 1;
}






