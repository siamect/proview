/* 
 * Proview   $Id: co_procom.h,v 1.2 2007-03-14 06:42:32 claes Exp $
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

#ifndef co_procom_h
#define co_procom_h

#include <vector.h>

extern "C" {
#include "rt_errh.h"
}
#include "co_provider.h"

typedef enum {
  procom_obj_mFlags_Deleted 	= 1 << 0,
  procom_obj_mFlags_Created 	= 1 << 1,
  procom_obj_mFlags_Loaded 	= 1 << 2
} procom_obj_mFlags;

class procom_obj {
 public:
  int cid;
  pwr_tOix oix;
  pwr_tOix fthoix;
  pwr_tOix bwsoix;
  pwr_tOix fwsoix;
  pwr_tOix fchoix;
  pwr_tOix lchoix;
  char name[32];
  char lname[120];
  unsigned long flags;
  void *body;
  unsigned int body_size;
  void *userdata;
  unsigned int userdata_size;

  procom_obj() : fthoix(0), bwsoix(0), fwsoix(0), fchoix(0), lchoix(0), flags(0),
    body(0), body_size(0), userdata(0), userdata_size(0)
    { strcpy(lname,"");}

  ~procom_obj() 
    { 
      if ( body) 
	free(body);
      if ( userdata)
	free(userdata);
    }

  procom_obj( const procom_obj& x)
    { 
      memcpy( this, &x, sizeof(procom_obj));
      if ( x.body) {
	body = malloc( body_size);
	memcpy( body, x.body, body_size);
     }
      if ( x.userdata) {
	userdata = malloc( userdata_size);
	memcpy( userdata, x.userdata, userdata_size);
      }
    }    
  procom_obj& operator=(const procom_obj& x)
  {
    memcpy( this, &x, sizeof(procom_obj));
    if ( body) {
      body = calloc( 1, body_size);
      memcpy( body, x.body, body_size);
    }
    if ( userdata) {
      userdata = calloc( 1, userdata_size);
      memcpy( userdata, x.userdata, userdata_size);
    }
    return *this;
  }
};

class co_procom 
{

public:
  co_provider *m_provider;

  co_procom( co_provider *p) : m_provider(p) {}
  virtual ~co_procom() {}
  virtual void provideObjects( pwr_tStatus, vector<procom_obj>&) {}
  virtual void provideObject( pwr_tStatus sts, pwr_tOix oix, pwr_tOix fthoix, pwr_tOix bwsoix,
			      pwr_tOix fwsoix, pwr_tOix fchoix, pwr_tOix lchoix, pwr_tCid cid,
			      char *name, char *longname) {}
  virtual void provideBody( pwr_tStatus sts, pwr_tOix oix, int size, void *body) {}
  virtual void provideAttr( pwr_tStatus sts, pwr_tOix oix, int size, void *buff) {}
  virtual void provideStatus( pwr_tStatus sts) {}
  virtual void flushNodes() {};
};


#endif

