/** 
 * Proview   $Id: wb_mvrepdbs.h,v 1.3 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_mvrepdbs_h
#define wb_mvrepdbs_h

#include "pwr.h"
#include "wb_adrep.h"
#include "wb_orep.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"

class wb_adrep;
class wb_bdrep;

class wb_mvrepdbs
{
  pwr_tVid m_vid;
public:


  wb_mvrepdbs();
  wb_mvrepdbs(pwr_tVid vid);

  wb_adrep adrep(wb_orep *o, char *aname);
  wb_adrep adrep(wb_cdrep *c, char *aname);
  wb_bdrep bdrep(wb_bdrep *b, char *aname);
  wb_adrep adrep(pwr_sAttrRef *arp);

};

#endif
