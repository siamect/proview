/* 
 * Proview   $Id: wb_orepwbl.cpp,v 1.13 2005-09-06 10:43:31 claes Exp $
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

#include "wb_orepwbl.h"


wb_orepwbl::wb_orepwbl( wb_vrepwbl *v, wb_wblnode *n): m_vrep(v), m_wblnode(n)
{}

wb_orepwbl::~wb_orepwbl()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepwbl::oid() const
{
  return m_wblnode->o->m_oid;
}

pwr_tVid wb_orepwbl::vid() const
{
  return m_wblnode->o->m_oid.vid;
}

pwr_tOix wb_orepwbl::oix() const
{
  return m_wblnode->o->m_oid.oix;
}

pwr_tOix wb_orepwbl::cid() const
{
  return m_wblnode->o->m_cid;
}

pwr_tOid wb_orepwbl::poid() const
{
  if ( m_wblnode->o->fth)
    return m_wblnode->o->fth->o->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::foid() const
{
  if ( m_wblnode->o->fch)
    return m_wblnode->o->fch->o->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::loid() const
{
  ref_wblnode n = m_wblnode->get_o_lch();
  if ( n)
    return n->o->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::boid() const
{
  if ( m_wblnode->o->bws)
    return m_wblnode->o->bws->o->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::aoid() const
{
  if ( m_wblnode->o->fws)
    return m_wblnode->o->fws->o->m_oid;
  return pwr_cNOid;
}

const char * wb_orepwbl::name() const
{
  return m_wblnode->name();
}

wb_name wb_orepwbl::longName()
{
  char str[200];

  m_vrep->objectName( this, str);
  return wb_name( str);
}


pwr_tTime wb_orepwbl::ohTime() const
{
  return m_wblnode->o->m_ohtime;
}


pwr_tTime wb_orepwbl::rbTime() const
{
  return m_wblnode->o->m_rbtime;
}


pwr_tTime wb_orepwbl::dbTime() const
{
  return m_wblnode->o->m_dbtime;
}


pwr_mClassDef wb_orepwbl::flags() const
{
  return m_wblnode->o->m_flags;
}

bool wb_orepwbl::isOffspringOf(const wb_orep *o) const
{
  return true;
}

wb_orep *wb_orepwbl::ancestor(pwr_tStatus *sts)
{
  return m_vrep->ancestor(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::parent(pwr_tStatus *sts)
{
  return m_vrep->parent(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::after(pwr_tStatus *sts)
{
  return m_vrep->after(sts, (wb_orep *)this);
}


wb_orep *wb_orepwbl::before(pwr_tStatus *sts)
{
  return m_vrep->before(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::first(pwr_tStatus *sts)
{
  return m_vrep->first(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::child(pwr_tStatus *sts, wb_name &name)
{
  return m_vrep->child(sts, (wb_orep *)this, name);
}

wb_orep *wb_orepwbl::last(pwr_tStatus *sts)
{
  return m_vrep->last(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::next(pwr_tStatus *sts)
{
  return m_vrep->next(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::previous(pwr_tStatus *sts)
{
  return m_vrep->previous(sts, (wb_orep *)this);
}

wb_adrep *wb_orepwbl::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepwbl::attribute(pwr_tStatus*)
{
  return 0;
}

bool wb_orepwbl::docBlock( char **block, int *size) const
{
  return m_wblnode->docBlock( block, size);
}

