#ifndef wb_adef_h
#define wb_adef_h

#include "wb_adrep.h"
#include "pwr.h"
#include "wb_orep.h"
#include "wb_name.h"
#include "wb_ldh.h"

class wb_orep;
class wb_adrep;

class wb_adef : public wb_status
{
public:
    wb_adrep *m_adrep;
    
    wb_adef();
    wb_adef(const wb_adef&);  // x = other_object
    wb_adef(const wb_adrep*);  // 
    wb_adef(const wb_orep&);  // x = other orep
    wb_adef& operator=(const wb_adef&) { return *this;}; // Fix

    ~wb_adef();

    operator bool() const { return (m_adrep != 0);}
    operator wb_adrep*() const;
    bool operator==(wb_adef&);
    
    pwr_sAttrRef aref();
    size_t size();   // get objects runtime body size
    int offset();
    int type();
    int nElement();
    int index();
    pwr_tOid aoid();
    pwr_tAix aix();
    pwr_tCid cid();
    pwr_tOix bix();
    pwr_tOid boid();
    
    wb_adef next() { return *this;} // Fix
    wb_adef prev() { return *this;} // Fix

    wb_name name() { wb_name n; return n;} // Fix
    wb_name name(ldh_eName type) { wb_name n; return n;} // Fix
    
  private:
    void check();
};

#endif


