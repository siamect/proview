/* 
 * Proview   $Id: wb_srep.h,v 1.9 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_srep_h
#define wb_srep_h

#include "pwr.h"
#include "wb_vrep.h"
#include "wb_ldh.h"


class wb_vrep;
class wb_object;
class wb_recix;

class wb_srep
{
protected:

  ldh_eAccess m_access;
  ldh_eUtility m_utility;

  unsigned int m_nUpdate;
  unsigned int m_refcount;
    
  wb_vrep *m_vrep;

  void       *m_editorContext;
  ldh_tSessionCb m_thisSessionCb;
  ldh_tSessionCb m_otherSessionCb;
  ldh_sEvent *m_events;
  wb_recix   *m_recix;
    
public:

  wb_srep(wb_vrep*);
  wb_srep(wb_srep*);
  ~wb_srep();

  void unref();
  wb_srep *ref();

  operator wb_vrep*() const;

  ldh_eAccess access(pwr_tStatus *sts) const;
  bool access(pwr_tStatus *sts, ldh_eAccess access);
  ldh_eUtility utility(pwr_tStatus *sts) const;
  bool utility(pwr_tStatus *sts, ldh_eUtility utility);

  //

  bool isReadonly(pwr_tStatus *sts) const;
  bool isEmpty(pwr_tStatus *sts) const;
  bool commit(pwr_tStatus *sts);
  bool abort(pwr_tStatus* sts);

  void editorContext( void *ctx) { m_editorContext = ctx;}
  void sendThisSession( ldh_tSessionCb thisSessionCb) { m_thisSessionCb = thisSessionCb;}
  void sendOtherSession( ldh_tSessionCb otherSessionCb) { m_otherSessionCb = otherSessionCb;}
  ldh_sEvent *newEvent();
  void deleteEvents();
  void eventNewFamily( ldh_sEvent *ep, wb_object o);
  void eventOldFamily( ldh_sEvent *ep, wb_object o);
  void eventSend( ldh_sEvent *ep);
  pwr_tStatus sendThisSession( void *editorContext, ldh_sEvent *event);
  pwr_tStatus sendOtherSession( void *editorContext, ldh_sEvent *event);
  void eventSendAllSessions( ldh_eEvent event);
  void eventSendSession( ldh_eEvent event);
  ldh_sEvent *eventStart( pwr_tOid Object, ldh_eEvent event);
  void update() { m_nUpdate++;}
  void recix_add( wb_object o);
  void recix_clear();
  void recix_set_destination( const char *d);
  wb_recix *recix() { return m_recix;}
};
#endif






















