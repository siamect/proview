/* 
 * Proview   $Id: co_pvd_file.h,v 1.1 2006-09-14 14:16:07 claes Exp $
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

#ifndef co_pvd_file_h
#define co_pvd_file_h

#include "co_provider.h"
#include "co_procom.h"


class co_pvd_file : public co_provider {
public:
  co_pvd_file( pvd_eEnv env = pvd_eEnv_Wb) : co_provider(env), root(0), next_oix(1) {}
  virtual void object( co_procom *pcom);
  virtual void objectOid( co_procom *pcom, pwr_tOix oix);
  virtual void objectName( co_procom *pcom, char *name);
  virtual void objectBody( co_procom *pcom, pwr_tOix oix);
  virtual void createObject( co_procom *pcom, pwr_tOix destoix, int desttype,
		     pwr_tCid cid, char *name);
  virtual void moveObject( co_procom *pcom, pwr_tOix oix, pwr_tOix destoix, int desttype);
  virtual void copyObject( co_procom *pcom, pwr_tOix oix, pwr_tOix destoix, int desttype, 
		   char *name);
  virtual void deleteObject( co_procom *pcom, pwr_tOix oix);
  virtual void deleteFamily( co_procom *pcom, pwr_tOix oix);
  virtual void renameObject( co_procom *pcom, pwr_tOix oix, char *name);
  virtual void writeAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer);
  virtual void readAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
			      unsigned int size);
  virtual void subAssociateBuffer( co_procom *pcom, void **buff, int oix, int offset, 
				   int size, pwr_tSubid sid);
  virtual void commit( co_procom *pcom);
  virtual void abort( co_procom *pcom);

  virtual char *longname( pwr_tOix oix);
  virtual void delete_tree( pwr_tOix oix);

  virtual void save( pwr_tStatus *sts) {}

  virtual void load( pwr_tStatus *rsts) {}

  virtual bool find( pwr_tOix fthoix, char *name, pwr_tOix *oix);
  
  vector<procom_obj> m_list;
  pwr_tOix root;
  pwr_tOix next_oix;
};

#endif
