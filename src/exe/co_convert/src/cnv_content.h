/* 
 * Proview   $Id: cnv_content.h,v 1.4 2008-10-31 12:51:30 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef cnv_content_h
#define cnv_content_h

/* cnv_content.h -- Table of content list */

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include "pwr.h"
#include "cnv_style.h"

class CnvContentElem {
 public:
  CnvContentElem() :
    page_number(0), header_level(0) 
    { strcpy( text, ""); strcpy( subject, "");}

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









