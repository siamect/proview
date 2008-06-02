/* 
 * Proview   $Id: wb_adrep.h,v 1.20 2008-06-02 14:57:38 claes Exp $
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

#ifndef wb_adrep_h
#define wb_adrep_h

#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "wb_name.h"

class wb_bdrep;
class wb_cdrep;
class wb_orep;
class wb_vrep;
class wb_bdrep;
class wb_name;

class wb_adrep 
{
  int m_nRef;
  wb_orep *m_orep; 
  pwr_tStatus m_sts;
   
  size_t m_size;
  pwr_eType m_type;
  int m_offset;
  int m_suboffset;
  int m_elements;
  int m_paramindex;
  int m_flags;
  pwr_tTid m_tid;
  pwr_tPgmName m_pgmname;
  pwr_tCid m_subClass;
  pwr_tOName m_subName;
  bool m_isSubattr;

  friend class wb_bdrep;
  friend class wb_cdrep;

public:
  wb_adrep(wb_orep& );
  ~wb_adrep();

  void unref();
  wb_adrep *ref();

  wb_adrep *next( pwr_tStatus *sts);
  wb_adrep *prev( pwr_tStatus *sts);
  wb_bdrep *bdrep();
  wb_cdrep *cdrep();

  pwr_sAttrRef aref();
  size_t size() {return m_size;}
  int offset() {return m_offset;}
  pwr_eType type() const {return m_type;}
  pwr_tTid tid() {return m_tid;}
  int nElement() {return m_elements;}
  int index() {return m_paramindex;}
  pwr_tOid aoid();
  int aix();
  pwr_tCid cid();
  pwr_eBix bix();
  int flags() {return m_flags;}
  pwr_tOid boid();
  pwr_tCid subClass() { return m_subClass;}
  bool isClass() const {return (m_flags & PWR_MASK_CLASS || m_flags & PWR_MASK_BUFFER);}
  bool isSuperClass() const {return (m_flags & PWR_MASK_CLASS && m_flags & PWR_MASK_SUPERCLASS);}
  bool isArray() const {return (m_flags & PWR_MASK_ARRAY);}
  void add( wb_adrep *ad, int idx = 0);
  bool isSubattr() { return m_isSubattr;}

  const char *name() const;
  const char *subName() const;
  wb_name longName();
  void *body( void *p = 0);
  bool renameAttribute( pwr_tStatus *sts, wb_name &name);

  wb_vrep *vrep() const;
};

#endif




