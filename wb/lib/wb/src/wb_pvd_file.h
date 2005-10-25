/* 
 * Proview   $Id: wb_pvd_file.h,v 1.2 2005-10-25 12:04:25 claes Exp $
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

#ifndef wb_pvd_file_h
#define wb_pvd_file_h

#include "wb_provider.h"

typedef enum {
  pitem_mFlags_Deleted 	= 1 << 0,
  pitem_mFlags_Created 	= 1 << 1
} pitem_mFlags;

class pitem {
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

  pitem() : fthoix(0), bwsoix(0), fwsoix(0), fchoix(0), lchoix(0), flags(0),
    body(0), body_size(0), userdata(0), userdata_size(0)
    { strcpy(lname,"");}

  ~pitem() 
    { 
      if ( body) 
	free(body);
      if ( userdata)
	free(userdata);
    }

  pitem( const pitem& x)
    { 
      memcpy( this, &x, sizeof(pitem));
      if ( x.body) {
	body = malloc( body_size);
	memcpy( body, x.body, body_size);
      }
      if ( x.userdata) {
	userdata = malloc( userdata_size);
	memcpy( userdata, x.userdata, userdata_size);
      }
    }    
  pitem& operator=(const pitem& x)
  {
    memcpy( this, &x, sizeof(pitem));
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


class wb_pvd_file : public wb_provider {
public:
  wb_pvd_file() : root(0), next_oix(1) {}
  virtual void object( wb_procom *pcom);
  virtual void objectOid( wb_procom *pcom, pwr_tOix oix);
  virtual void objectName( wb_procom *pcom, char *name);
  virtual void objectBody( wb_procom *pcom, pwr_tOix oix);
  virtual void createObject( wb_procom *pcom, pwr_tOix destoix, int desttype,
		     pwr_tCid cid, char *name);
  virtual void moveObject( wb_procom *pcom, pwr_tOix oix, pwr_tOix destoix, int desttype);
  virtual void copyObject( wb_procom *pcom, pwr_tOix oix, pwr_tOix destoix, int desttype, 
		   char *name);
  virtual void deleteObject( wb_procom *pcom, pwr_tOix oix);
  virtual void deleteFamily( wb_procom *pcom, pwr_tOix oix);
  virtual void renameObject( wb_procom *pcom, pwr_tOix oix, char *name);
  virtual void writeAttribute( wb_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer);
  virtual void commit( wb_procom *pcom);
  virtual void abort( wb_procom *pcom);

  virtual char *longname( pwr_tOix oix);
  virtual void delete_tree( pwr_tOix oix);

  virtual void save( pwr_tStatus *sts) {}
  virtual void load( pwr_tStatus *sts) {}

  virtual bool find( pwr_tOix fthoix, char *name, pwr_tOix *oix);
  
  vector<pitem> m_list;
  pwr_tOix root;
  pwr_tOix next_oix;
};

#endif
