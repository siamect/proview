/* 
 * Proview   $Id: wb_orepced.cpp,v 1.1 2007-11-23 14:25:09 claes Exp $
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

#include "wb_orepced.h"


wb_orepced::wb_orepced( wb_vrepced *v, wb_orep *n): m_vrep(v), m_orep(n)
{}

wb_orepced::~wb_orepced()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepced::oid() const
{
  return m_orep->oid();
}

pwr_tVid wb_orepced::vid() const
{
  return m_orep->vid();
}

pwr_tOix wb_orepced::oix() const
{
  return m_orep->oix();
}

pwr_tOix wb_orepced::cid() const
{
  return m_orep->cid();
}

pwr_tOid wb_orepced::poid() const
{
  return m_orep->poid();
}

pwr_tOid wb_orepced::foid() const
{
  return m_orep->foid();
}

pwr_tOid wb_orepced::loid() const
{
  return m_orep->loid();
}

pwr_tOid wb_orepced::boid() const
{
  return m_orep->boid();
}

pwr_tOid wb_orepced::aoid() const
{
  return m_orep->aoid();
}

const char * wb_orepced::name() const
{
  return m_orep->name();
}

wb_name wb_orepced::longName()
{
  return m_orep->longName();
}


pwr_tTime wb_orepced::ohTime() const
{
  return m_orep->ohTime();
}

pwr_tTime wb_orepced::rbTime() const
{
  return m_orep->rbTime();
}

pwr_tTime wb_orepced::dbTime() const
{
  return m_orep->dbTime();
}

pwr_mClassDef wb_orepced::flags() const
{
  return m_orep->flags();
}


bool wb_orepced::isOffspringOf(const wb_orep *o) const
{
  return m_orep->isOffspringOf( o);
}

wb_orep *wb_orepced::ancestor(pwr_tStatus *sts)
{
  return m_orep->ancestor( sts);
}

wb_orep *wb_orepced::parent(pwr_tStatus *sts)
{
  return m_orep->parent( sts);
}

wb_orep *wb_orepced::after(pwr_tStatus *sts)
{
  return m_orep->after( sts);
}


wb_orep *wb_orepced::before(pwr_tStatus *sts)
{
  return m_orep->before( sts);
}

wb_orep *wb_orepced::first(pwr_tStatus *sts)
{
  return m_orep->first( sts);
}

wb_orep *wb_orepced::child(pwr_tStatus *sts, wb_name &name)
{
  return m_orep->child( sts, name);
}

wb_orep *wb_orepced::last(pwr_tStatus *sts)
{
  return m_orep->last( sts);
}

wb_orep *wb_orepced::next(pwr_tStatus *sts)
{
  return m_orep->next( sts);
}

wb_orep *wb_orepced::previous(pwr_tStatus *sts)
{
  return m_orep->previous( sts);
}

wb_adrep *wb_orepced::attribute(pwr_tStatus* sts, const char *aname)
{
  return m_orep->attribute( sts, aname);
}

wb_adrep *wb_orepced::attribute(pwr_tStatus* sts)
{
  return m_orep->attribute( sts);
}

bool wb_orepced::docBlock( char **block, int *size) const
{
  return m_orep->docBlock( block, size);
}

bool wb_orepced::docBlock( char *block)
{
  return m_orep->docBlock( block);
}








