#ifndef cnv_wbltops_h
#define cnv_wbltops_h

/* cnv_wbltops.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#include "pwr.h"
#include "cnv_readwbl.h"
#include "cnv_wblto.h"
#include "cnv_tops.h"

using namespace std;

class CnvReadWbl;

class CnvWblToPs : public CnvWblTo {
 public:
  CnvWblToPs( CnvCtx *cnv_ctx) : 
    ctx(cnv_ctx), conf_pass(false) {}
  virtual ~CnvWblToPs();

  CnvCtx  	*ctx;
  CnvToPs	tops;
  bool		conf_pass;

  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int attribute_exec();
  int typedef_exec();
  int bit_exec();
  Cnv_eWblToType type() { return Cnv_eWblToType_Ps;}
  int index_open() { return 0;}
  void set_confpass( bool conf) {
    tops.set_confpass( conf);
    conf_pass = conf;
  }
  bool confpass() { return true;}
};

#endif

