#ifndef wb_cdrep_h
#define wb_cdrep_h

#include "pwr.h"
#include "pwr_class.h"
#include "co_dbs.h"
#include "wb_adef.h"
#include "wb_name.h"
#include "wb_orep.h"

class wb_adef;
class wb_orep;

class wb_cdrep 
{
public:

    dbs_sObject *m_coh;
    pwr_tStatus m_sts;

    wb_cdrep() {} // Fix
    wb_cdrep(const wb_adef&);  // x = other_object
    wb_cdrep(const wb_orep&) {}  // Fix x = other orep
    wb_cdrep& operator=(const wb_cdrep&);

    //wb_object& operator=(const wb_orep&);

    size_t size() { return 0;}   // Fix get objects runtime body size
    pwr_tCid cid() { return 0;} // Fix
    

    wb_name name() { wb_name n; return n;} // Fix get class name
    wb_name name(ldh_eName type) { wb_name n; return n;} // Fix 
    void name(const char *name);
    void name(wb_name *name);
    
    
    pwr_tStatus sts() { return m_sts;}
};

#endif
