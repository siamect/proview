#ifndef cnv_wbltoh_h
#define cnv_wbltoh_h

/* cnv_wbltoh.h -- Documentation

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
  
class CnvWblToH : public CnvWblTo {
 public:
  CnvWblToH( CnvCtx *cnv_ctx) : ctx(cnv_ctx), struct_class_open(0),
    struct_filler_cnt(0), attr_count(0) {}
  virtual ~CnvWblToH() {}

  CnvCtx      *ctx;
  int         struct_class_open;
  ofstream 	fp_struct;
  CnvFile     *cstruc;
  int		struct_cclass_written;
  int		struct_cclass_endwritten;
  int		struct_filler_cnt;
  char	struct_volid[80];
  unsigned int	struct_vid_0;
  unsigned int	struct_vid_1;
  int		attr_count;
  
  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int body_close();
  int attribute_exec();
  int typedef_exec();
  int typedef_close();
  int bit_exec();
  int graphplcnode() { return 1;}
  int graphplccon() { return 1;}
  int template_exec() { return 1;}
  Cnv_eWblToType type() { return Cnv_eWblToType_H;}
  int class_open() { return struct_class_open;}
  int index_open() { return 0;}
 
  int volname_to_id();
  void cix_to_classid( unsigned int cix, pwr_tClassId *cid);
  int cixstr_to_classid( char *cix_str, pwr_tClassId *cid);
  static void get_filename( CnvReadWbl *rw, char *struct_file, int hpp);
  int check_typename( char *type_volume, char *type_name);
  
};

#if defined __cplusplus
}
#endif
#endif

