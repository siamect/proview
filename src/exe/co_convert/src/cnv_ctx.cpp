
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

char *CnvCtx::low( char *in)
{
  static char str[400];

  cdh_ToLower( str, in);
  return str;
}

int CnvCtx::remove_spaces(
			char	*in,
			char	*out)
{
  char    *s;

  for ( s = in; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++);

  strcpy( out, s);
        
  s = out;
  if ( strlen(s) != 0) {
    for ( s += strlen(s) - 1; 
          !((s == out) || ((*s != ' ') && (*s != 9))); s--) ;
    s++;
    *s = 0;
  }

  return 1;
}

int CnvCtx::read_line(
			char	*line,
			int	maxsize,
			FILE	*file)
{ 
  char	*s;

  if (fgets( line, maxsize, file) == NULL)
    return 0;
  line[maxsize-1] = 0;
  s = strchr( line, 10);
  if ( s != 0)
    *s = 0;
  s = strchr( line, 13);
  if ( s != 0)
    *s = 0;

  return 1;
}

