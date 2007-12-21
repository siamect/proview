/* 
 * Proview   $Id: wb_tdrep.h,v 1.10 2007-12-21 13:18:01 claes Exp $
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

#ifndef wb_tdrep_h
#define wb_tdrep_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_name.h"

class wb_adrep;
class wb_mrepdbs;
class wb_orep;
class wb_mvrep;

class wb_tdrep
{
  int m_nRef;
  wb_orep *m_orep;
  int m_sts;

  pwr_eType m_type;
  size_t m_size;
  int m_elements;
  pwr_tTid m_typeref;
  pwr_tObjName m_pgmname;
    
public:
  wb_tdrep();
  wb_tdrep(wb_mvrep *, pwr_tTid);
  wb_tdrep( wb_mvrep *mvrep, wb_name name);
  wb_tdrep( const wb_adrep& a);
  wb_tdrep(wb_mvrep *, const wb_adrep&);

  void unref();
  wb_tdrep *ref();

  size_t size() { return m_size;}
  pwr_tTid tid();
  pwr_tTid typeRef() {return m_typeref;}
  pwr_eType type() { return m_type;}
  int nElement() { return m_elements;}
  const char *pgmName();

  const char *name() const;
  wb_name longName();
  bool renameType( pwr_tStatus *sts, wb_name &name);

  pwr_tStatus sts() { return m_sts;}    

private:
  void init();
};

#endif
