#ifndef cnv_xtthelptopdf_h
#define cnv_xtthelptopdf_h

/* cnv_xtthelptodf.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

using namespace std;

#include <iostream.h>
#include <vector.h>
#include <string>
#include "pwr.h"
#include "cnv_xtthelpto.h"
#include "cnv_style.h"
#include "cnv_content.h"
#include "cnv_topdf.h"

class CnvCtx;

typedef enum {
  pdf_mStatus_xtthelp 	= 1 << 0,
  pdf_mStatus_paragraph 	= 1 << 1,
  pdf_mStatus_table 	= 1 << 2,
  pdf_mStatus_topic 	= 1 << 3
} pdf_mStatus;

typedef enum {
  pdf_mOption_printDisable	= 1 << 0
} pdf_mOption;

class CnvXtthelpToPdf : public CnvXtthelpTo {
 public:
  CnvXtthelpToPdf( CnvCtx *cnv_ctx) :
    ctx(cnv_ctx), base_ci(0), first_topic(1), user_style(0), status(0),
    conf_pass(false), option(0)
    { 
      strcpy( current_subject, "");
    }
  virtual ~CnvXtthelpToPdf();

  Cnv_eXtthelpToType type() { return Cnv_eXtthelpToType_Ps;}
  void *insert( navh_eItemType item_type, char *text1,
		char *text2, char *text3, char *link, 
		char *link_bookmark, char *file_name,
		navh_eHelpFile file_type, int help_index, 
		char *bookmark);
  void subject_to_fname( char *fname, char *subject, int path);
  bool confpass() { return true;}
  void set_confpass( bool conf) { 
    topdf.set_confpass( conf);
    conf_pass = conf;
    if ( !conf) {
      // Reset
      base_ci = 0;
      user_style = 0;
      first_topic = 1;
      status = 0;    
    }
  }

  CnvCtx *ctx;
  CnvToPdf topdf;
  int base_ci;
  int first_topic;
  int first_chaptertopic;
  int user_style;
  unsigned int status;
  bool conf_pass;
  char current_subject[80];
  unsigned int option;
};

#endif









