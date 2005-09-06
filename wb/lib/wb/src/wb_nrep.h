/* 
 * Proview   $Id: wb_nrep.h,v 1.7 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_nrep_h
#define wb_nrep_h

#include <string.h>
#include "pwr.h"
#include "wb_error.h"
extern "C" {
#include "co_cdh.h"
}

typedef enum {
  wname_eType_Vid,
  wname_eType_Oid,
  wname_eType_Aid,
  wname_eType_Name
} wname_eType;

class wb_namesegments {
public:
  int offs;
  int len;
};

class wb_nameattributes {
public:
  int offs;
  int len;
  int index;
  bool isSuper;
};

class wb_nrep
{
protected:
  int m_nRef;

private:
  static char normname_tab[];
  static char idname_tab[];
  static char pathname_tab[];
  static char objname_tab[];
  static char unatname_tab[];

  wname_eType name_type;
  int num_seg;
  int num_attr;
  int vol_len;
  int vol_offs;
  int b_size;
  int b_offset;
  wb_namesegments *seg;
  wb_nameattributes *attr;
  char oname[200];
  char norm_name[200];
  pwr_tStatus status;
  bool m_hasSuper;
  bool m_shadowed;

  void parse() throw (wb_error);
  char *objectName( const char *n, char *res = 0);
  char *segmentName( const char *n, int idx = 0, char *res = 0);
  char *attributeName( const char *n, int idx = 0, char *res = 0);
  char *pathName( const char *n, char *res = 0);
  char *volumeName( const char *n, char *res = 0);
  char *nameName( const char *n, int ntype, char *res);

public:

  void unref();
  wb_nrep *ref();

  wb_nrep();
  wb_nrep( const char *n) throw (wb_error);
  wb_nrep( const wb_nrep& n);
  ~wb_nrep();
  
  bool hasVolume() const { return (vol_len > 0);}
  bool hasPath() const { return (num_seg > 1);}
  bool hasObject() const { return hasSegment();}
  bool hasSegment( int idx = 0) const { return ( idx >= 0 && idx < num_seg);}
  bool hasAttribute( int idx = 0) const { return ( idx >= 0 && idx < num_attr);}
  bool hasAttrIndex( int idx = 0) const
  { 
    if ( hasAttribute(idx))
      return ( attr[idx].index != -1);
    return false;
  }
  int attrIndex( int idx = 0) const
  { 
    if ( hasAttrIndex(idx)) 
      return attr[idx].index;
    return 0;
  }
  int attributes() { return num_attr;}
  int segments() { return num_seg;}
  char *segment( int idx = 0, char *res = 0);
  char *path( char *res = 0);
  char *volume( char *res = 0);
  char *object( char *res = 0);
  char *attribute( int idx = 0, char *res = 0);
  char *normObject( char *res = 0);
  char *normPath( char *res = 0);
  char *normVolume( char *res = 0);
  char *normSegment( int idx = 0, char *res = 0);
  char *normAttribute( int idx = 0, char *res = 0);
  char *name() { return oname;}
  char *normName() { return norm_name;}
  char *name( int ntype, char *res = 0);
  char *normName( int ntype, char *res = 0);
  bool volumeIsEqual( const char *n);
  bool objectIsEqual( const char *n);
  bool segmentIsEqual( const char *n, int idx = 0);
  bool attributeIsEqual( const char *n, int idx = 0);
  int bodyOffset() const { return b_offset;}
  int bodySize() const { return b_size;}
  char *attributesAll( int idx = 0, char *res = 0, bool true_db = false);
  char *segmentsAll( int idx = 0, char *res = 0);
  bool hasSuper() { return m_hasSuper;}
  void setShadowed( bool shadowed) { m_shadowed = shadowed;}
  static char *unatName( const char *name);
  static bool checkObjectName( const char *name);
};
  
#endif




