#ifndef wb_adrep_h
#define wb_adrep_h

#include "pwr.h"
#include "wb_mvrepdbs.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"

class wb_orepdbs;

class wb_adrep 
{
    int m_nRef;
    wb_orepdbs *m_orep; 
    pwr_tStatus m_sts;
   
    friend class wb_bdrep;
    friend class wb_cdrep;

 public:
    wb_adrep(wb_orepdbs& );
    ~wb_adrep();

    void unref();
    wb_adrep *ref();

    wb_bdrep *bdrep();
    wb_cdrep *cdrep();

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




