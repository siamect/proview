#ifndef wb_bdrep_h
#define wb_bdrep_h

#include "pwr.h"
#include "wb_orep.h"
#include "wb_object.h"
#include "wb_name.h"

class wb_adef;
class wb_adrep;
class wb_orepdbs;

class wb_bdrep
{
    int m_nRef;
    wb_orepdbs *m_orep;
    pwr_tStatus m_sts;
    
public:
    wb_bdrep();
    wb_bdrep(wb_orepdbs& o); // own orep
    wb_bdrep( wb_adrep *adrep);
    ~wb_bdrep();

    void unref();
    wb_bdrep *ref();

    pwr_sAttrRef aref() { pwr_sAttrRef a; return a;} // Fix
    size_t size();
    int nAttribute() { return 0;} // Fix
    cdh_eBix bix();
    pwr_tOid boid() { pwr_tOid o; return o;} // Fix
    

    wb_name name() { wb_name n; return n;} // Fix // get attribute name
    wb_name name(ldh_eName type) { wb_name n; return n;} // Fix
    
    wb_adrep *adrep( pwr_tStatus *sts); // Get first attribute
    wb_adrep *adrep( pwr_tStatus *sts, char *aname);

    pwr_tStatus sts() { return m_sts;}
    
            
};

#endif
