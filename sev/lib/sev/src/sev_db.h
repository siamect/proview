/* 
 * Proview   $Id: sev_db.h,v 1.3 2008-09-18 14:37:43 claes Exp $
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


class sev_attr {
 public:
  pwr_tOName	aname;
  pwr_eType	type;
  unsigned int	size;
  unsigned int  elem;
  pwr_tString16 unit;
};

class sev_item {
 public:
  sev_item() : deadband_active(0), last_id(0), first_storage(1), deleted(0) 
    { memset( old_value, 0, sizeof(old_value));}
  unsigned int 	id;
  char		tablename[256];
  pwr_tOid	oid;
  pwr_tOName	oname;
  pwr_tTime	creatime;
  pwr_tTime	modtime;
  pwr_tDeltaTime storagetime;
  pwr_tRefId    sevid;
  pwr_tString80 description;
  pwr_tFloat32  scantime;
  pwr_tFloat32  deadband;
  pwr_tMask	options;
  int		deadband_active;
  unsigned int	last_id;
  char		old_value[8];
  int 		first_storage;
  unsigned int  attrnum;
  sev_attr	attr[1];
  int deleted;
};


class sev_db {
 public:
  vector<sev_item> m_items;

  sev_db() {}
  virtual ~sev_db() {}
  virtual int check_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			  pwr_tDeltaTime storatetime, pwr_eType type, unsigned int size, 
			  char *description, char *unit, pwr_tFloat32 scantime, 
			  pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx) 
    { return 0;}
  virtual int add_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
			char *description, char *unit, pwr_tFloat32 scantime, 
			pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx) 
    { return 0;}
  virtual int delete_item( pwr_tStatus *sts, pwr_tOid oid, char *aname) { return 0;}
  virtual int store_value( pwr_tStatus *sts, int item_idx, int attr_idx,
			   pwr_tTime time, void *buf, unsigned int size) { return 0;}
  virtual int get_values( pwr_tStatus *sts, pwr_tOid oid, pwr_tMask options, float deadband, 
			  char *aname, pwr_eType type, 
			  unsigned int size, pwr_tFloat32 scantime,  pwr_tTime *starttime, 
			  pwr_tTime *endtime, int maxsize, pwr_tTime **tbuf, void **vbuf, 
			  unsigned int *bsize) { return 0;}
  virtual int get_items( pwr_tStatus *sts) { return 0;}
  virtual int delete_old_data( pwr_tStatus *sts, pwr_tOid oid, char *aname, 
			       pwr_tMask options, pwr_tTime limit) { return 0;}
};
#endif
