#ifndef cnv_wbltoxtthelp_h
#define cnv_wbltoxtthelp_h

/* cnv_wbltoxtthelp.h -- Documentation

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

class CnvWblToXtthelp : public CnvWblTo {
 public:
  CnvWblToXtthelp( CnvCtx *cnv_ctx) : 
    ctx(cnv_ctx), xtthelp_index_open(0), xtthelp_in_topic(0) {}
  virtual ~CnvWblToXtthelp() {}

  CnvCtx  	*ctx;
  ofstream    	fp_xtthelp_index;
  int	      	xtthelp_index_open;
  int         	xtthelp_in_topic;
  ofstream 	fp_tmp;

  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int attribute_exec();
  int bit_exec();
  Cnv_eWblToType type() { return Cnv_eWblToType_Xtthelp;}
  int index_open() { return xtthelp_index_open;}
    
};

#if defined __cplusplus
}
#endif
#endif

