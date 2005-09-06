/* 
 * Proview   $Id: wb_mvrep.cpp,v 1.6 2005-09-06 10:43:31 claes Exp $
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


#include "wb_mvrep.h"
#include "wb_tdrep.h"
#include "wb_cdrep.h"

wb_cdrep *wb_mvrep::cdrep( pwr_tCid cid)
{
  return new wb_cdrep( this, cid);
}

wb_cdrep *wb_mvrep::cdrep( const wb_orep& o)
{
  return new wb_cdrep( this, o);
}

wb_tdrep *wb_mvrep::tdrep( pwr_tTid tid)
{
  return new wb_tdrep( this, tid);
}

wb_tdrep *wb_mvrep::tdrep( const wb_adrep& a)
{
  return new wb_tdrep( this, a);
}

