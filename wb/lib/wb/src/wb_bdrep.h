#ifndef wb_bdrep_h
#define wb_bdrep_h

#include "pwr.h"
#include "wb_orep.h"
#include "wb_adef.h"
#include "wb_object.h"
#include "wb_name.h"

class wb_adef;

class wb_bdrep
{
public:

    pwr_tStatus m_sts;
    
    wb_bdrep();
    wb_bdrep(const wb_adef&);
    wb_bdrep(const wb_orep&);
    wb_bdrep& operator=(const wb_bdrep&);

    pwr_sAttrRef aref() { pwr_sAttrRef a; return a;} // Fix
    size_t size() { return 0;} // Fix   // get objects runtime body size
    int nAttribute() { return 0;} // Fix
    int bix() { return 0;} // Fix
    pwr_tOid boid() { pwr_tOid o; return o;} // Fix
    

    wb_name name() { wb_name n; return n;} // Fix // get attribute name
    wb_name name(ldh_eName type) { wb_name n; return n;} // Fix
    
    pwr_tStatus sts() { return m_sts;}
    
            
};

#endif
