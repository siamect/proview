#ifndef cnv_wblto_h
#define cnv_wblto_h

/* cnv_wblto.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_readwbl.h"

using namespace std;

class CnvReadWbl;

typedef enum {
  Cnv_eWblToType_Html,
  Cnv_eWblToType_H,
  Cnv_eWblToType_Xtthelp,
  Cnv_eWblToType_Ps
} Cnv_eWblToType;
  
class CnvWblTo {
 public:
  // CnvWblTo() {}
  virtual ~CnvWblTo() {}

  virtual int init( char *);
  virtual int close() { return 1;}
  virtual int class_exec() { return 1;}
  virtual int class_close() { return 1;}
  virtual int body_exec() { return 1;}
  virtual int body_close() { return 1;}
  virtual int attribute_exec() { return 1;}
  virtual int typedef_exec() { return 1;}
  virtual int typedef_close() { return 1;}
  virtual int bit_exec() { return 1;}
  virtual int graphplcnode() { return 1;}
  virtual int graphplccon() { return 1;}
  virtual int template_exec() { return 1;}
  virtual Cnv_eWblToType type() { return Cnv_eWblToType_H;} 
  virtual int class_open() { return 0;}
  virtual int index_open() { return 0;}
  virtual bool confpass() { return false;}
  virtual void set_confpass( bool conf) {};
};

#if defined __cplusplus
}
#endif
#endif

