/** 
 * Proview   $Id: wb_mvrep.h,v 1.6 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_mvrep_h
#define wb_mvrep_h

#include "wb_vrep.h"

class wb_adrep;
class wb_cdrep;
class wb_orep;
class wb_tdrep;


class wb_mvrep : public wb_vrep {
public:
  wb_cdrep *cdrep( const wb_orep& o);
  wb_cdrep *cdrep( pwr_tCid cid);
  wb_tdrep *tdrep( const wb_adrep& a);
  wb_tdrep *tdrep( pwr_tTid tid);
};

#endif
