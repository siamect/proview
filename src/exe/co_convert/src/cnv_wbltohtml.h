#ifndef cnv_wbltohtml_h
#define cnv_wbltohtml_h

/* cnv_wbltohtml.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_readwbl.h"
#include "cnv_wblto.h"


using namespace std;

class CnvReadWbl;
  
class CnvWblToHtml : public CnvWblTo {
 public:
  CnvWblToHtml( CnvCtx *cnv_ctx) : ctx(cnv_ctx),
    html_class_open(0), html_index_open(0), js_all_first(0) {}
  virtual ~CnvWblToHtml() {}

  CnvCtx	*ctx;
  CnvFile   	*html_clf;
  ofstream 	fp_html_index;
  ofstream 	fp_js_all;
  ofstream 	fp_tmp;
  ofstream	fp_html_group[MAX_GROUPS];
  ofstream	fp_js_group[MAX_GROUPS];
  char		html_first[80];
  char		html_tmp_name[80];
  int		html_class_open;
  int		html_index_open;
  bool		js_all_first;
  bool		js_group_first[80];
  
  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int body_close();
  int attribute_exec();
  int graphplcnode();
  int graphplccon();
  int template_exec();
  int typedef_exec();
  int typedef_close();
  int bit_exec();
  Cnv_eWblToType type() { return Cnv_eWblToType_Html;}
  int class_open() { return html_class_open;}
  int index_open() { return html_index_open;}
  
};

#if defined __cplusplus
}
#endif
#endif

