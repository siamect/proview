#ifndef wb_srep_h
#define wb_srep_h

#include "pwr.h"
#include "wb_vrep.h"
#include "wb_ldh.h"
#include "wb_ldhi.h"

class wb_vrep;

class wb_srep
{
protected:

  ldh_eAccess m_access;
  ldh_eUtility m_utility;
  //ldh_sEvent *fep;
  //ldh_sEvent *lep;
  void *m_editorContext;
  ldhi_tSesEventCB m_sendThisSession;
  ldhi_tSesEventCB m_sendOtherSession;

  unsigned int m_nUpdate;
  unsigned int m_refcount;
    
  wb_vrep *m_vrep;
    
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

};


#endif






















