/** 
 * Proview   $Id: wb_adef.cpp,v 1.12 2005-09-01 14:57:57 claes Exp $
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

#include "wb_ldh_msg.h"
#include "wb_adef.h"
#include "wb_adrep.h"
#include "wb_name.h"

wb_adef::wb_adef() : wb_status(LDH__NOSUCHATTR), m_adrep(0)
{
}

wb_adef::wb_adef( wb_adrep *adrep) : wb_status(LDH__SUCCESS), m_adrep(adrep)
{
  if ( adrep == 0)
    m_sts = LDH__NOSUCHATTR;
  else
    m_adrep->ref();
}

wb_adef::wb_adef(const wb_adef& x) : wb_status(x.m_sts), m_adrep(x.m_adrep)
{
  if ( m_adrep)
    m_adrep->ref();
}

wb_adef& wb_adef::operator=(const wb_adef& x)
{
  if ( x.m_adrep)
    x.m_adrep->ref();
  if ( m_adrep)
    m_adrep->unref();

  m_adrep = x.m_adrep;
  m_sts = x.m_sts;
  return *this;
}

wb_adef::~wb_adef()
{
  if ( m_adrep)
    m_adrep->unref();
}

void wb_adef::check()
{
  if ( evenSts()) throw wb_error(m_sts);
}

wb_adef wb_adef::next()
{
  pwr_tStatus sts;

  check();
  return wb_adef( m_adrep->next( &sts));
}

wb_adef wb_adef::prev()
{
  pwr_tStatus sts;

  check();
  return wb_adef( m_adrep->prev( &sts));
}


// get objects runtime body size
size_t wb_adef::size() 
{ 
  check();
  return m_adrep->size();
}

int wb_adef::offset() 
{ 
  check();
  return m_adrep->offset();
}

pwr_eType wb_adef::type() 
{
  check();
  return m_adrep->type();
}

int wb_adef::nElement()
{
  check();
  return m_adrep->nElement();
}

int wb_adef::index() 
{
  check();
  return m_adrep->index();
}

int wb_adef::flags() 
{
  check();
  return m_adrep->flags();
}

pwr_tOid wb_adef::aoid() 
{
  check();
  return m_adrep->aoid();
}

pwr_tAix wb_adef::aix() 
{
  check();
  return m_adrep->aix();
}

pwr_tCid wb_adef::cid() 
{
  check();
  return m_adrep->cid();
}

pwr_eBix wb_adef::bix() 
{
  check();
  return m_adrep->bix();
}

pwr_tOid wb_adef::boid() 
{
  check();
  return m_adrep->boid();
}

const char *wb_adef::name()
{
  check();
  return m_adrep->name();
}

const char *wb_adef::subName()
{
  check();
  return m_adrep->subName();
}

wb_name wb_adef::longName()
{
  check();
  return m_adrep->longName();
}

void *wb_adef::body( void *p)
{
  check();
  return m_adrep->body( p);
}

bool wb_adef::isClass()
{
  check();
  return m_adrep->isClass();
}

bool wb_adef::isSuperClass()
{
  check();
  return m_adrep->isSuperClass();
}

pwr_tCid wb_adef::subClass()
{
  check();
  return m_adrep->subClass();
}

