/** 
 * Proview   $Id: wb_mvolume.h,v 1.3 2005-09-01 14:57:58 claes Exp $
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

#include "pwr.h"
#include "wb_volume.h"
#include "wb_object.h"
#include "wb_attribute.h"

class wb_mvolume
{
  wb_mvrepdbs *m_mvrep;
  int m_vid;
    
public:
    
  wb_volume();
  wb_volume(pwr_tVid vid);

  wb_volume volume(pwr_tVid vid);
  wb_volume volume(char *name);
    
  wb_adef adef(wb_object o, wb_adef adef);
  wb_adef adef(wb_object o, wb_name aname);
  wb_adef adef(wb_name aname);
  wb_adef adef(pwr_tAref *arp);

  wb_bdef();
    
  wb_cdef();
    
};
