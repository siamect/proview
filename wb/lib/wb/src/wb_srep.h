#ifndef wb_srep_h
#define wb_srep_h

#include "pwr.h"
#include "wb_vrep.h"
#include "wb_ldh.h"
#include "wb_ldhi.h"


class wb_vrep;
class wb_object;

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
};
#endif






















