/* 
 * Proview   $Id: wb_destination.cpp,v 1.6 2005-09-06 10:43:31 claes Exp $
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

#include "wb_destination.h"
#include "wb_object.h"
#include "pwr_systemclasses.h"
#include "pwr_baseclasses.h"

wb_destination::wb_destination(pwr_tOid oid, ldh_eDest code) :
  m_oid(oid), m_code(code)
{
}

wb_destination::wb_destination(wb_object &o, ldh_eDest code) :
  m_oid(o.oid()), m_code(code)
{    
}

