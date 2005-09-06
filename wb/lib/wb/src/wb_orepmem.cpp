/** 
 * Proview   $Id: wb_orepmem.cpp,v 1.5 2005-09-06 08:02:04 claes Exp $
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

#include "wb_orepmem.h"


wb_orepmem::wb_orepmem( wb_vrepmem *v, mem_object *n): m_vrep(v), m_memobject(n)
{}

wb_orepmem::~wb_orepmem()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepmem::oid() const
{
  return m_memobject->m_oid;
}

pwr_tVid wb_orepmem::vid() const
{
  return m_memobject->m_oid.vid;
}

pwr_tOix wb_orepmem::oix() const
{
  return m_memobject->m_oid.oix;
}

pwr_tOix wb_orepmem::cid() const
{
  return m_memobject->m_cid;
}

pwr_tOid wb_orepmem::poid() const
{
  if ( m_memobject->fth)
    return m_memobject->fth->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::foid() const
{
  if ( m_memobject->fch)
    return m_memobject->fch->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::loid() const
{
  mem_object *n = m_memobject->get_lch();
  if ( n)
    return n->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::boid() const
{
  if ( m_memobject->bws)
    return m_memobject->bws->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::aoid() const
{
  if ( m_memobject->fws)
    return m_memobject->fws->m_oid;
  return pwr_cNOid;
}

const char * wb_orepmem::name() const
{
  return m_memobject->name();
}

wb_name wb_orepmem::longName()
{
  char str[200];

  m_vrep->objectName( this, str);
  return wb_name( str);
}


pwr_tTime wb_orepmem::ohTime() const
{
  return m_memobject->m_ohtime;
}

pwr_tTime wb_orepmem::rbTime() const
{
  return m_memobject->m_rbtime;
}

pwr_tTime wb_orepmem::dbTime() const
{
  return m_memobject->m_dbtime;
}

pwr_mClassDef wb_orepmem::flags() const
{
  return m_memobject->m_flags;
}


bool wb_orepmem::isOffspringOf(const wb_orep *o) const
{
  return true;
}

wb_orep *wb_orepmem::ancestor(pwr_tStatus *sts)
{
  return m_vrep->ancestor(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::parent(pwr_tStatus *sts)
{
  return m_vrep->parent(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::after(pwr_tStatus *sts)
{
  return m_vrep->after(sts, (wb_orep *)this);
}


wb_orep *wb_orepmem::before(pwr_tStatus *sts)
{
  return m_vrep->before(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::first(pwr_tStatus *sts)
{
  return m_vrep->first(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::child(pwr_tStatus *sts, wb_name &name)
{
  return m_vrep->child(sts, (wb_orep *)this, name);
}

wb_orep *wb_orepmem::last(pwr_tStatus *sts)
{
  return m_vrep->last(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::next(pwr_tStatus *sts)
{
  return m_vrep->next(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::previous(pwr_tStatus *sts)
{
  return m_vrep->previous(sts, (wb_orep *)this);
}

wb_adrep *wb_orepmem::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepmem::attribute(pwr_tStatus*)
{
  return 0;
}

bool wb_orepmem::docBlock( char **block, int *size) const
{
  return m_memobject->docBlock( block, size);
}

bool wb_orepmem::docBlock( char *block)
{
  return m_memobject->docBlock( block);
}








