/* 
 * Proview   $Id: sev_db.h,v 1.1 2008-07-17 11:18:31 claes Exp $
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
 */

#ifndef sev_db_h
#define sev_db_h

#include <vector.h>

#include "pwr.h"
#include "pwr_class.h"


class sev_item {
 public:
  sev_item() {}
  unsigned int 	id;
  char		tablename[256];
  pwr_tOid	oid;
  pwr_tOName	aname;
  pwr_tOName	oname;
  pwr_tTime	cretime;
  pwr_tTime	uptime;
  pwr_tDeltaTime storagetime;
  pwr_eType	vtype;
  unsigned int	vsize;
  pwr_tRefId    sevid;
  pwr_tString80 description;
  pwr_tString16 unit;
  pwr_tFloat32  scantime;
};


class sev_db {
 public:
  vector<sev_item> m_items;

  sev_db() {}
  virtual ~sev_db() {}
  virtual int check_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			  pwr_tDeltaTime storatetime, pwr_eType type, unsigned int size, 
			  char *description, char *unit, pwr_tFloat32 scantime, unsigned int *idx) 
    { return 0;}
  virtual int add_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
			char *description, char *unit, pwr_tFloat32 scantime, unsigned int *idx) 
    { return 0;}
  virtual int store_value( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_eType type, 
			   pwr_tTime time, void *buf, unsigned int size) { return 0;}
  virtual int get_values( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_eType type, 
			  unsigned int size, pwr_tFloat32 scantime,  pwr_tTime *starttime, 
			  pwr_tTime *endtime, int maxsize, pwr_tTime **tbuf, void **vbuf, 
			  unsigned int *bsize) { return 0;}
  virtual int get_items( pwr_tStatus *sts) { return 0;}
  virtual int delete_old_data( pwr_tStatus *sts, pwr_tOid oid, char *aname, 
			       pwr_tTime limit) { return 0;}
};
#endif
