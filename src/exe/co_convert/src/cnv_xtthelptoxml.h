/* 
 * Proview   $Id: cnv_xtthelptoxml.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_xtthelptoxml_h
#define cnv_xtthelptoxml_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_xtthelpto.h"

using namespace std;

class CnvCtx;

typedef enum {
  xml_mStatus_xtthelp 	= 1 << 0,
  xml_mStatus_paragraph = 1 << 1,
  xml_mStatus_table 	= 1 << 2,
  xml_mStatus_topic 	= 1 << 3
} xml_mStatus;

class CnvXtthelpToXml : public CnvXtthelpTo {
 public:
  CnvXtthelpToXml( CnvCtx *cnv_ctx) :
    ctx(cnv_ctx), first_topic(1), status(0) {}
  virtual ~CnvXtthelpToXml();

  Cnv_eXtthelpToType type() { return Cnv_eXtthelpToType_Xml;}
  void *insert( navh_eItemType item_type, char *text1,
		char *text2, char *text3, char *link, 
		char *link_bookmark, char *file_name,
		navh_eHelpFile file_type, int help_index, 
		char *bookmark);
  void subject_to_fname( char *fname, char *subject, int path);
  void cnv_text( char *to, char *from);

  CnvCtx *ctx;
  ofstream fp;
  int first_topic;
  unsigned int status;
};

#if defined __cplusplus
}
#endif
#endif

