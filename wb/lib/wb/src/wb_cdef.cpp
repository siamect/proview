/* 
 * Proview   $Id: wb_cdef.cpp,v 1.16 2007-09-19 15:12:16 claes Exp $
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

#include "wb_adef.h"
#include "wb_bdef.h"
#include "wb_cdef.h"
#include "wb_cdrep.h"
#include "wb_object.h"

wb_cdef::wb_cdef() : wb_status(LDH__NOCLASS), m_cdrep(0)
{
}

wb_cdef::wb_cdef( wb_cdrep *cdrep) : wb_status(LDH__SUCCESS), m_cdrep(cdrep) 
{
  if ( cdrep == 0)
    m_sts = LDH__NOCLASS;
  else
    cdrep->ref();
}

wb_cdef::wb_cdef( wb_adef& a) : m_cdrep(0)
{
  wb_adrep *adrep = a;
  try {
    m_cdrep = new wb_cdrep( adrep);
    m_cdrep->ref();
    m_sts = m_cdrep->sts();
  }
  catch ( wb_error& e) {
    m_sts = e.sts();
  }
}

wb_cdef::wb_cdef(const wb_orep& x) : m_cdrep(0)
{
  try {
    m_cdrep = new wb_cdrep( x);
    m_cdrep->ref();
    m_sts = m_cdrep->sts();
  }
  catch( wb_error& e) {
    m_sts = e.sts();
  }
}

wb_cdef::wb_cdef(wb_mvrep *mvrep, pwr_tCid cid) : m_cdrep(0)
{
  try {
    m_cdrep = new wb_cdrep( mvrep, cid);
    m_cdrep->ref();
    m_sts = m_cdrep->sts();
  }
  catch ( wb_error& e) {
    m_sts = e.sts();
  }
}

wb_cdef::~wb_cdef()
{
  if ( m_cdrep)
    m_cdrep->unref();
}

wb_cdef::wb_cdef(const wb_cdef& x) : wb_status(x.sts()), m_cdrep(x.m_cdrep)
{
  if ( m_cdrep)
    m_cdrep->ref();
}

wb_cdef& wb_cdef::operator=(const wb_cdef& x)
{
  if ( x.m_cdrep)
    x.m_cdrep->ref();
  if ( m_cdrep)
    m_cdrep->unref();
  m_cdrep = x.m_cdrep;
  m_sts = x.m_sts;
  return *this;
}

void wb_cdef::check() const
{
  if ( !m_cdrep) 
    throw wb_error(m_sts);
}

pwr_tCid wb_cdef::cid()
{
  check();
  return m_cdrep->cid();
}

pwr_mClassDef wb_cdef::flags()
{
  check();
  return m_cdrep->flags();
}

const char *wb_cdef::name() const
{
  check();
  return m_cdrep->name();
}

wb_name wb_cdef::longName()
{
  check();
  return m_cdrep->longName();
}

wb_bdef wb_cdef::bdef( const char *bname)
{
  check();
  pwr_tStatus sts;

  wb_bdrep *bdrep = m_cdrep->bdrep( &sts, bname);
  if ( ODD(sts))
    return wb_bdef( bdrep);
  else
    return wb_bdef();
}

wb_bdef wb_cdef::bdef( pwr_eBix bix)
{
  check();
  pwr_tStatus sts;

  wb_bdrep *bdrep = m_cdrep->bdrep( &sts, bix);
  if ( ODD(sts))
    return wb_bdef( bdrep);
  else
    return wb_bdef();
}

wb_object wb_cdef::classBody( const char *bname)
{
  check();
  pwr_tStatus sts;

  wb_orep *orep = m_cdrep->classBody( &sts, bname);
  if ( ODD(sts))
    return wb_object( orep);
  else
    return wb_object();
}

void wb_cdef::templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p, pwr_tOid oid)
{
  check();
  m_cdrep->templateBody( sts, bix, p, oid);
}

size_t wb_cdef::size( pwr_eBix bix)
{
  check();
  return m_cdrep->size( bix);
}

wb_cdef wb_cdef::super()
{
  pwr_tStatus sts;

  check();
  return wb_cdef( m_cdrep->super( &sts));
}




