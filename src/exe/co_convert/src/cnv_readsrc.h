#ifndef cnv_readsrc_h
#define cnv_readsrc_h

/* cnv_readsrc.h -- Read source files

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_ctx.h"
#include "cnv_readwbl.h"

using namespace std;


class CnvReadSrc {
 public:
  CnvReadSrc( CnvCtx *cnv_ctx) :
    ctx(cnv_ctx), state(0), linetype(cread_eLine_No), doc_fresh(0), 
    html_class_open(0)
    {}

  CnvCtx 	*ctx;
  int	 	state;
  char		src_aref[80];
  char		src_aref_text[200];
  ofstream 	fp_src_html;
  cread_eLine	linetype;
  int		doc_fresh;
  int		html_class_open;

  int read_src( char *filename);
  int html_init( char *filename);
  int html_close();
  int html_line( char *line);
  int html_aref();
  static void filename_to_html( char *fname, char *filename);
  char *low( char *in);
};

#if defined __cplusplus
}
#endif
#endif



