#ifndef cnv_xtthelpto_h
#define cnv_xtthelpto_h

/* cnv_xtthelpto.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "co_nav_help.h"
#include "cnv_readxtthelp.h"

using namespace std;

class CnvReadXtthelp;

typedef enum {
  Cnv_eXtthelpToType_Html,
  Cnv_eXtthelpToType_Xml,
  Cnv_eXtthelpToType_Ps
} Cnv_eXtthelpToType;
  
class CnvXtthelpTo {
 public:
  // CnvXtthelpTo() {}

  virtual Cnv_eXtthelpToType type() { return Cnv_eXtthelpToType_Html;}
  virtual void *insert( navh_eItemType item_type, char *text1,
			char *text2, char *text3, char *link, 
			char *link_bookmark, char *file_name,
			navh_eHelpFile file_type, int help_index, 
			char *bookmark) 
    { return 0;}
  virtual bool confpass() { return false;}
  virtual void set_confpass( bool conf) {};
};

#if defined __cplusplus
}
#endif
#endif


