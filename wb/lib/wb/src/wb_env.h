/* 
 * Proview   $Id: wb_env.h,v 1.9 2005-11-22 12:22:29 claes Exp $
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

#ifndef wb_env_h
#define wb_env_h

#include "pwr.h"
#include "wb_erep.h"
#include "wb_volume.h"
#include "wb_cdef.h"

class wb_volume;
class wb_erep;

class wb_env : public wb_status
{
protected:
  wb_erep *m_erep;
    
public:

  wb_env();
    
  wb_env(wb_erep*);
  wb_env(wb_env &e);
    
  wb_env& operator=(const wb_env& x);
    
  operator bool() const;
  operator wb_erep*() const;
  bool operator==(wb_env&);
    
  void load( char *db) { m_erep->load(&m_sts, db);}
  bool open();
  bool close();

  // Get first volume in list of db volumes.
  wb_volume volume();
  // Get db volume.
  wb_volume volume(pwr_tVid);

  wb_volume volume(const char *name);
  wb_volume externVolume(pwr_tVid);
  wb_volume bufferVolume();
    
  wb_volume snapshot(pwr_tVid);

  wb_volume createVolume(pwr_tVid vid, pwr_tCid cid, const char *name, bool add=true);

  int nextVolatileVid( char *name) { return m_erep->nextVolatileVid( &m_sts, name); }
    
};

#endif




