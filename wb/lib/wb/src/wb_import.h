/* 
 * Proview   $Id: wb_import.h,v 1.9 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_import_h
#define wb_import_h

#include "pwr.h"
#include "co_dbs.h"

class wb_export;

class wb_import
{
public:

  virtual bool importVolume(wb_export &e) = 0;
    
  virtual bool importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                          const char *name, const char *normname, pwr_mClassDef flags,
                          pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                          size_t rbSize, size_t dbSize) = 0;

  virtual bool importRbody(pwr_tOid oid, size_t size, void *body) = 0;
    
  virtual bool importDbody(pwr_tOid oid, size_t size, void *body) = 0;
    
  virtual bool importDocBlock(pwr_tOid oid, size_t size, char *block) = 0;
    
  virtual bool importMeta(dbs_sMenv *mep) = 0;
    
};

#endif
