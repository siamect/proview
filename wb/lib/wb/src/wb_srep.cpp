#include "wb_srep.h"
#include "wb_vrep.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"

wb_srep::wb_srep(wb_vrep *vrep) : m_access(ldh_eAccess_ReadOnly), m_utility(ldh_eUtility__),
				  m_editorContext(0), m_sendThisSession(0), m_sendOtherSession(0),
				  m_nUpdate(0), m_refcount(0)
{
    m_vrep = vrep->ref();
}

wb_srep::wb_srep(wb_srep *srep) // Fix ????? access utility ...
{
    m_vrep = ((wb_vrep*)srep)->ref();
}

wb_srep::~wb_srep()
{
    pwr_tStatus sts = LDH__SUCCESS;

    if (m_nUpdate != 0) {
        m_vrep->abort(&sts);
    }
    
    if (m_vrep != 0)
        m_vrep->unref();
}

wb_srep::operator wb_vrep*() const
{
    return m_vrep;
}


void wb_srep::unref()
{
    if (--m_refcount == 0)
        delete this;
}

wb_srep *wb_srep::ref()
{
    m_refcount++;
    return this;
}

ldh_eAccess wb_srep::access(pwr_tStatus *sts) const
{
  *sts = LDH__SUCCESS; // Fix
  return m_access;
}

bool wb_srep::access(pwr_tStatus *sts, ldh_eAccess access) // Fix
{
  if ( ldh_eAccess__ < access && access < ldh_eAccess_) { 
    m_access = access;
    *sts = LDH__SUCCESS;
    return true;
  }
  *sts = LDH__NYI;
  return false;
}

ldh_eUtility wb_srep::utility(pwr_tStatus *sts) const // Fix
{
  *sts = LDH__SUCCESS;
  return m_utility;
}

bool wb_srep::utility(pwr_tStatus *sts, ldh_eUtility utility) // Fix
{
  if ( ldh_eUtility__ < utility && utility < ldh_eUtility_) {
    m_utility = utility;
    *sts = LDH__SUCCESS;
    return true;
  }
  *sts = LDH__NYI;
  return false;
}

bool wb_srep::isReadonly(pwr_tStatus *sts) const
{
    return (m_access == ldh_eAccess_ReadOnly);
}

bool wb_srep::isEmpty(pwr_tStatus *sts) const
{
    return (m_nUpdate == 0);
}

bool wb_srep::commit(pwr_tStatus *sts)
{
    bool ok;
    
    ok = m_vrep->commit(sts);
    if (ok) {
        m_nUpdate = 0;
    }
    return ok;
}

bool wb_srep::abort(pwr_tStatus *sts) // Fix was inline...
{
    bool ok;
    
    ok = m_vrep->abort(sts);
    if (ok) {
        m_nUpdate = 0;
    }
    return ok;
}


