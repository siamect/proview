/* 
 * Proview   $Id: wb_session.h,v 1.9 2005-09-06 10:43:32 claes Exp $
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

/* wb_session.h -- work bench database session */

#ifndef wb_session_h
#define wb_session_h


#include "pwr.h"
//#include "wb_vrep.h"
#include "wb_object.h"
#include "wb_attribute.h"
#include "wb_ldh.h"
//#include "wb_ldhi.h"
//#include "wb_adef.h"
//#include "wb_bdef.h"
#include "wb_cdef.h"
#include "wb_name.h"
#include "wb_destination.h"
#include "wb_oset.h"
#include "wb_srep.h"
#include "wb_volume.h"

class wb_object;
class wb_bdef;
class wb_cdef;
class wb_destination;
class wb_volume;


class wb_session : public wb_volume
{
protected:
  wb_srep    *m_srep;
    
public:

    
  wb_session();
  wb_session(wb_volume &v);
  wb_session(wb_session &s);
  ~wb_session();
  wb_session& operator=(const wb_session& x);
    
  // Calls redirected to srep.
    
  ldh_eAccess access() { return m_srep->access( &m_sts);}
  bool access(ldh_eAccess access) { return m_srep->access( &m_sts, access);}
  ldh_eUtility utility() { return m_srep->utility( &m_sts);}
  bool utility(ldh_eUtility utility) { return m_srep->utility( &m_sts, utility);}

  //

  bool isReadonly();
  bool isEmpty();
  bool commit();
  bool abort();

  // Calls that need write privileges

  wb_object createObject(wb_cdef cdef, wb_destination d, wb_name name);
  wb_object copyObject(wb_object o, wb_destination d, wb_name name);

  bool moveObject(wb_object o, wb_destination d);
  bool renameObject(wb_object o, wb_name name);
  bool deleteObject(wb_object o);
  bool deleteFamily(wb_object o);

  bool writeAttribute(wb_attribute &a, void *p, size_t size);
  bool writeAttribute(wb_attribute &a, void *p);
  bool writeBody() {return false;} // Fix

  bool copyOset( pwr_sAttrRef *arp, bool keepref, bool ignore_errors);
  bool cutOset( pwr_sAttrRef *arp, bool keepref);
  bool pasteOset( pwr_tOid doid, ldh_eDest dest, 
		  bool keepoid, char *buffer);


  void getAllMenuItems( ldh_sMenuCall	*ip, ldh_sMenuItem **Item, wb_cdrep *cdrep,
			wb_orep *o, void *o_body, pwr_tUInt32 Level,
			int *nItems, int AddSeparator);

  pwr_tStatus getMenu( ldh_sMenuCall *ip);
  pwr_tStatus callMenuMethod( ldh_sMenuCall *mcp, int Index);

  void editorContext( void *ctx) { m_srep->editorContext( ctx);}
  void sendThisSession( ldh_tSessionCb thisSessionCb) { m_srep->sendThisSession( thisSessionCb);}
  void sendOtherSession( ldh_tSessionCb otherSessionCb) { m_srep->sendOtherSession( otherSessionCb);}

  bool validateDestination( wb_destination d, pwr_tCid cid);
  bool castAttribute( pwr_sAttrRef *arp, pwr_tCid cid);
};


inline bool wb_session::isEmpty()
{
  return m_srep->isEmpty(&m_sts);
}

inline bool wb_session::isReadonly()
{
  return m_srep->isReadonly(&m_sts);
}

inline bool wb_session::commit()
{
  return m_srep->commit(&m_sts);
}

inline bool wb_session::abort()
{
  return m_srep->abort(&m_sts);
}



#endif









