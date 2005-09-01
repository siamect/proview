/** 
 * Proview   $Id: wb_env.cpp,v 1.7 2005-09-01 14:57:58 claes Exp $
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

#include "wb_env.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"

wb_env::wb_env() : wb_status(LDH__SUCCESS)
{
  m_erep = new wb_erep();
  m_erep->ref();
}

wb_env::wb_env(wb_erep *e) : wb_status(LDH__SUCCESS), m_erep(e)
{
  m_erep->ref();
}

wb_env::wb_env(wb_env &e) : wb_status( e.sts()), m_erep(e.m_erep)
{
  if ( m_erep)
    m_erep->ref();
}

wb_env& wb_env::operator=(const wb_env& x)
{
  // Note! first ref() then unref(), because if
  // m_erep == x.m_erep, erep could be deleted.
  if ( x.m_erep)
    x.m_erep->ref();
  if ( m_erep)
    m_erep->unref();
  m_erep = x.m_erep;
  m_sts = x.sts();
    
  return *this;
}

wb_env::operator bool() const
{
  if (!m_erep)
    return false;

  return true;
}

wb_env::operator wb_erep*() const
{
  return m_erep;
}

bool wb_env::open()
{
  return true;
}

bool wb_env::close()
{
  return true;
}

wb_volume wb_env::volume()
{
  wb_volume v( m_erep->volume( &m_sts));
  return v;
}

wb_volume wb_env::volume(pwr_tVid vid)
{
  wb_vrep *vrep = m_erep->volume( &m_sts, vid);
  wb_volume v( vrep);
  return v;
}

wb_volume wb_env::externVolume(pwr_tVid vid)
{
  wb_vrep *vrep = m_erep->externVolume( &m_sts, vid);
  wb_volume v( vrep);
  return v;
}

wb_volume wb_env::bufferVolume()
{
  wb_vrep *vrep = m_erep->bufferVolume( &m_sts);
  wb_volume v( vrep);
  return v;
}

wb_volume wb_env::volume(const char *name)
{
  wb_volume v( m_erep->volume( &m_sts, name));
  return v;
}

wb_volume wb_env::createVolume(pwr_tVid vid, pwr_tCid cid, const char *name)
{
  wb_volume v( m_erep->createVolume( &m_sts, vid, cid, name));
  return v;
}

wb_volume wb_env::snapshot(pwr_tVid) // Fix
{
  wb_volume v;
  return v;
}












