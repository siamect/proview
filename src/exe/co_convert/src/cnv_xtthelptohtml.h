#ifndef cnv_xtthelptohtml_h
#define cnv_xtthelptohtml_h

/* cnv_xtthelptohtml.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_xtthelpto.h"

using namespace std;

class CnvCtx;

class CnvXtthelpToHtml : public CnvXtthelpTo {
 public:
  CnvXtthelpToHtml( CnvCtx *cnv_ctx) :
    ctx(cnv_ctx) {}
  virtual ~CnvXtthelpToHtml() {}

  Cnv_eXtthelpToType type() { return Cnv_eXtthelpToType_Html;}
  void *insert( navh_eItemType item_type, char *text1,
		char *text2, char *text3, char *link, 
		char *link_bookmark, char *file_name,
		navh_eHelpFile file_type, int help_index, 
		char *bookmark);
  void subject_to_fname( char *fname, char *subject, int path);

  CnvCtx *ctx;
  ofstream fp;
};

#if defined __cplusplus
}
#endif
#endif

