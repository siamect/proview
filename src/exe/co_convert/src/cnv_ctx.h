#ifndef co_convert_h
#define co_convert_h

/* co_convert.h -- Setup

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "pwr.h"
#include "cnv_setup.h"

class CnvWblTo;
class CnvReadWbl;
class CnvReadXtthelp;

class CnvCtx {
 public:
  CnvCtx() : generate_html(0), generate_xtthelp(0), generate_src(0),
    generate_struct(0), generate_ps(0), common_structfile_only(0), hpp(0), 
    verbose(0), first_class(1), setup(0), wblto(0), rw(0), rx(0)
    {
      strcpy( dir, "");
      strcpy( setup_filename, "");
    }
  char		dir[120];
  int		generate_html;
  int		generate_xtthelp;
  int		generate_src;
  int		generate_struct;
  int		generate_ps;
  int		common_structfile_only;
  int		hpp;
  int	       	verbose;
  char	 	setup_filename[120];
  int		first_class;
  CnvSetup	*setup;
  CnvWblTo	*wblto;
  CnvReadWbl	*rw;
  CnvReadXtthelp *rx;

  static int read_line( char *line, int maxsize, FILE *file);
  static int remove_spaces( char *in, char *out);
  static char *low( char *in);
};

#if defined __cplusplus
}
#endif
#endif





