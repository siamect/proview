/** 
 * Proview   $Id: wb_vrepref.cpp,v 1.2 2005-09-01 14:57:59 claes Exp $
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

#include "wb_vrepref.h"
#include "wb_orepref.h"
#include "wb_ldh_msg.h"
#include "wb_cdrep.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_dbs.h"

wb_orep *wb_vrepref::object(pwr_tStatus *sts, pwr_tOid oid)
{
  wb_orepref *o = new wb_orepref( this, oid.oix);
  *sts = LDH__SUCCESS;
  return (wb_orep *) o;
}

wb_orep *wb_vrepref::object(pwr_tStatus *sts, wb_name &name) 
{
  char cn[200];
  char *s;
  strcpy( cn, name.name(cdh_mName_path | cdh_mName_object));
  if ( (s = strchr( cn, '-'))) 
    *s = ':';
  wb_name cname(cn);
  if ( !cname) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
    
  wb_cdrep *cdrep = m_merep->cdrep( sts, wb_name(cname));
  if ( EVEN(*sts)) return 0;
  
  wb_orepref *o = new wb_orepref( this, cdrep->cid());
  delete cdrep;
  return (wb_orep *)o;
}

void wb_vrepref::objectName(const wb_orep *o, char *str) 
{
  pwr_tStatus sts;
  char n[120];
  char *s;

  wb_cdrep *cdrep = m_merep->cdrep( &sts, ((wb_orepref *)o)->m_cid);
  if ( EVEN(sts)) { 
    strcpy( str,  ""); 
    return;
  }

  strcpy( str, m_name);
  strcat( str, ":");
  strcpy( n, cdrep->longName().c_str());
  delete cdrep;
  if ( (s = strchr( n, ':')))
    *s = '-';
  strcat( str, n);
}

