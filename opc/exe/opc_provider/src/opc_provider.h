/* 
 * Proview   $Id: opc_provider.h,v 1.10 2007-11-22 15:10:23 claes Exp $
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

#ifndef opc_provider_h
#define opc_provider_h

#include <map.h>
#include "co_provider.h"
#include "co_procom.h"
#include "pwr_opcclasses.h"
#include "opc_soap_H.h"

class opcprv_obj {
 public:
  procom_obj po;
  char item_name[512];
  int type;
  int size;

  opcprv_obj() { strcpy( item_name, "");}
  opcprv_obj( const opcprv_obj& x) : po(x.po), type(x.type), size(x.size)
    {
      strcpy( item_name, x.item_name);
    }
  opcprv_obj& operator=(const opcprv_obj& x)
  {
    po = x.po;
    type = x.type;
    size = x.size;
    strcpy( item_name, x.item_name);
    return *this;
  }
};

class opcprv_sub {
 public:
  pwr_tOix oix;
  std::string handle;
};

class opc_provider : public co_provider {
public:
  opc_provider( pvd_eEnv env = pvd_eEnv_Wb) : co_provider(env), root(0), next_oix(1),
    server_state(0) {
    memset( &server_state, 0, sizeof(server_state));
  }
  ~opc_provider() {
    for ( int i = 0; i < (int) m_list.size(); i++)
      delete m_list[i];
  }
  virtual void object( co_procom *pcom);
  virtual void objectOid( co_procom *pcom, pwr_tOix oix);
  virtual void objectName( co_procom *pcom, char *name, pwr_tOix poix);
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
  virtual void subDisassociateBuffer( co_procom *pcom, pwr_tSubid sid); 
  virtual void cyclic( co_procom *pcom);
  virtual void commit( co_procom *pcom);
  virtual void abort( co_procom *pcom);

  virtual char *longname( pwr_tOix oix);
  virtual void delete_tree( pwr_tOix oix);

  virtual void save( pwr_tStatus *sts) {}

  virtual void load( pwr_tStatus *rsts) {}

  virtual bool find( pwr_tOix fthoix, char *name, pwr_tOix *oix);
  
  void insert_object( pwr_tOix fth, pwr_tOix bws, s0__BrowseElement *element,
		      int first, int last, int load_children, std::string *path);

  void get_server_state();
  char *name_to_objectname( char *name);
  void errlog( std::string* item, std::vector<s0__OPCError *>& errvect);
  void init();
  void fault();

  vector<opcprv_obj *> m_list;
  map<pwr_tUInt32, opcprv_sub> m_sublist;
  pwr_tOix root;
  pwr_tOix next_oix;
  struct soap soap;
  pwr_sClass_Opc_ServerState *server_state;
};

#endif
