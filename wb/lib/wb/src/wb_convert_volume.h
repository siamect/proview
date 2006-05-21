/* 
 * Proview   $Id: wb_convert_volume.h,v 1.1 2006-05-21 22:36:22 lw Exp $
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

#ifndef wb_convert_volume_h
#define wb_convert_volume_h

#include "pwr.h"

class wb_convert_volume
{
public:
  pwr_tCid m_cidChecked;
  int m_classCount;
  int m_instanceCount;
  int m_totalInstanceCount;
  bool m_update;  
  
  wb_convert_volume() : m_cidChecked(pwr_cNCid), m_classCount(0), m_instanceCount(0), m_update(false) {};
  wb_convert_volume(bool update) : m_cidChecked(pwr_cNCid), m_classCount(0), m_instanceCount(0), m_update(update) {};
  virtual ~wb_convert_volume() {}

  void setUpdate(bool update) { m_update = update;}
  virtual void updateObject(pwr_tOid oid, pwr_tCid cid) = 0;  
  virtual void checkObject(pwr_tOid oid, pwr_tCid cid) = 0;  
};

#endif
