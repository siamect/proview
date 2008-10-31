/* 
 * Proview   $Id: cnv_xtthelpto.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#ifndef cnv_xtthelpto_h
#define cnv_xtthelpto_h

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
  virtual void *insert( navh_eItemType item_type, const char *text1,
			const char *text2, const char *text3, const char *link, 
			const char *link_bookmark, const char *file_name,
			navh_eHelpFile file_type, int help_index, 
			const char *bookmark) 
    { return 0;}
  virtual bool confpass() { return false;}
  virtual void set_confpass( bool conf) {};

  virtual ~CnvXtthelpTo() {}
};

#if defined __cplusplus
}
#endif
#endif


