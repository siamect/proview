/* 
 * Proview   $Id: wb_procom.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_procom_h
#define wb_procom_h

#include "co_procom.h"

#include "wb_vext.h"

class wb_procom : public co_procom
{
  char m_key[200];
  int m_connected;
  int m_msgsndid;
  int m_msgrcvid;
  procom_eType type;
  vext_sAMsg m_msg;
  procom_eType m_type;

public:
  wb_procom( const char *key, co_provider *p, procom_eType type) : 
    co_procom( p), m_connected(0), m_type( type)
  {
    strcpy( m_key, key);
  }
  void put( vext_sAMsg *msg, int size, pwr_tStatus *sts);
  void receive( vext_sQMsg *msg, int size, pwr_tStatus *sts);
  void dispatch( vext_sQMsg *qmsg);
  void mainloop();
  void provideObject( pwr_tStatus sts, pwr_tOix oix, pwr_tOix fthoix, pwr_tOix bwsoix,
		      pwr_tOix fwsoix, pwr_tOix fchoix, pwr_tOix lchoix, pwr_tCid cid,
		      const char *name, const char *longname);
  void provideBody( pwr_tStatus sts, pwr_tOix oix, int size, void *body);
  void provideStatus( pwr_tStatus sts);

  int lmsgsnd( int msgid, const void *msg_ptr, size_t msg_sz, int msgflg);
  int lmsgrcv( int msgid, const void *msg_ptr, size_t msg_sz, int msgtype, int msgflg);
};


#endif

