#ifndef cnv_setup_h
#define cnv_setup_h

/* cnv_setup.h -- Setup

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"

#define MAX_GROUPS 100

class CnvSetup {
 public:
  int 		group_cnt;
  char 	        groups[MAX_GROUPS][40];
  char 		groups_startpage[MAX_GROUPS][80];

  int setup( char *filename);
};

#if defined __cplusplus
}
#endif
#endif




