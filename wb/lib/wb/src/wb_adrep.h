#ifndef wb_adrep_h
#define wb_adrep_h

#include "pwr.h"
#include "wb_mvrepdbs.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"

class wb_adrep 
{
public:
    wb_mvrepdbs *m_vrep;
    dbs_sObject *m_ao;    
    
    wb_adrep();
    wb_adrep(wb_mvrepdbs *vrep, dbs_sObject *ao);    
    wb_adrep(const wb_adrep&);  // x = other_object
    wb_adrep& operator=(const wb_adrep&);

    ~wb_adrep();

    wb_bdrep *bdef();
    wb_cdrep *cdef();

    pwr_sAttrRef aref();
    size_t size(); // get objects runtime body size
    int offset() {return 0;}; // Fix
    int type() {return 0;} // Fix
    int nElement() {return 0;} // Fix
    int index() {return 0;} // Fix
    pwr_tOid aoid();
    int aix() {return 0;} // Fix
    pwr_tCid cid();
    int bix() {return 0;} // Fix
    int flags() {return 0;} // Fix
    pwr_tOid boid();

    wb_name name() { wb_name n; return n;} // Fix // get attribute name
    wb_name name(ldh_eName type) { wb_name n; return n; } // Fix 
    
};

#endif




