#ifndef cnv_content_h
#define cnv_content_h

/* cnv_content.h -- Table of content list

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

using namespace std;

#include <iostream.h>
#include <vector.h>
#include <string>
#include "pwr.h"
#include "cnv_style.h"

class CnvContentElem {
 public:
  int page_number;
  int header_level;
  char header_number[40];
  char subject[80];
  char text[80];
};

class CnvContent {
 public:
  CnvContent() {}
  vector<CnvContentElem> tab;
  void add( CnvContentElem& elem);
  int find_link( char *subject, char *text, int *page);
};

#endif









