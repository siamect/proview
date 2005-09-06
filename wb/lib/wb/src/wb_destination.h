/* 
 * Proview   $Id: wb_destination.h,v 1.7 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_destination_h
#define wb_destination_h

#include "pwr.h"
#include "wb_ldh.h"

class wb_object;

class wb_destination
{
  pwr_tOid m_oid;
  ldh_eDest m_code;

public:
  wb_destination(wb_object &object, ldh_eDest code);
  wb_destination(pwr_tOid oid, ldh_eDest code);
    
  pwr_tOid oid() { return m_oid;}
  ldh_eDest code() { return m_code;}    
};

#endif



