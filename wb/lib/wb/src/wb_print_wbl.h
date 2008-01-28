/* 
 * Proview   $Id: wb_print_wbl.h,v 1.11 2008-01-28 06:33:58 claes Exp $
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
 **/

/**
 * @file wb_print_wbl.h
 *
 * Prints a volume on wbl format
 *
 */

#ifndef wb_print_wbl_h
#define wb_print_wbl_h

#include <iostream.h>

#include "pwr_class.h"

class wb_adef;
class wb_attribute;
class wb_cdef;
class wb_object;
class wb_volume;


class wb_print_wbl 
{
protected:
  int  m_errCnt;
  bool m_idxFlag;
  bool m_noFoCodeFlag;
  bool m_timeFlag;
  int  m_level;
  int  m_levelInd;
  char m_indBuf[256];
  bool m_keepName;
  ostream& m_os;
    

  ostream& indent(int levelIncr = 0);    

  void printAttribute(wb_volume& v, 
                      wb_attribute& attr, 
                      wb_attribute& tattr, ///< template attribute
                      wb_adef& adef);

  void printBody(wb_volume& v, 
                 wb_object& o,
                 wb_object& templ,
                 wb_cdef& cdef, 
                 pwr_eBix bix);

  void printBuffer(wb_volume& v,
                  wb_attribute& attr,
                  wb_attribute& tattr, ///< template attribute
                  wb_adef& adef);
    
  void printClass(wb_volume& v,
                  wb_attribute& attr,
                  wb_attribute& tattr, ///< template attribute
                  wb_adef& adef);
    
  void printParameter(wb_volume& v, 
                      wb_attribute& attr, 
                      wb_attribute& tattr, ///< template attribute
                      wb_adef& adef);

  void printText(wb_volume& v, 
                 wb_adef& adef,
                 const char* text,
                 int varSize);

  bool printValue(wb_volume& v, 
                  wb_adef& adef,
                  void *val,
                  int varSize,
                  char **svalp);
  bool isFoCodeObject( wb_volume& v, 
		       wb_object& o);
    

public:
  wb_print_wbl(ostream& os, int levelIndentation = 2);
  ~wb_print_wbl();

  int getErrCnt() const { return m_errCnt;}
  void resetErrCnt() {m_errCnt = 0; }
  void keepName() { m_keepName = true;}
  void noIndex() { m_idxFlag = false; m_timeFlag = false;}
  void noFoCode() { m_noFoCodeFlag = true;}

  void printHierarchy(wb_volume& v, wb_object& o); //< Prints a hierarchy
  void printObject(wb_volume& v, wb_object& o, bool recursive = true); //< Prints an object
  void printVolume(wb_volume& v, bool recursive = true); //< Prints the volume

};

#endif
