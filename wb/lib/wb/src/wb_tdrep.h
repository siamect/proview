#ifndef wb_tdrep_h
#define wb_tdrep_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_name.h"

class wb_adrep;
class wb_mrepdbs;
class wb_orepdbs;
class wb_mvrep;

class wb_tdrep
{
    int m_nRef;
    wb_orepdbs *m_orep;
    int m_sts;

public:
    wb_tdrep();
    wb_tdrep(wb_mvrep *, pwr_tTid);
    wb_tdrep( wb_mvrep *mvrep, wb_name name);
    wb_tdrep( const wb_adrep& a);
    wb_tdrep(wb_mvrep *, const wb_adrep&);

    void unref();
    wb_tdrep *ref();

    size_t size() { return 0;} // Fix   // get objects runtime body size
    pwr_tTid tid();
    pwr_eType type() { return (pwr_eType)0;} // Fix
    int nElement() { return 0;} // Fix

    wb_name name() { wb_name n; return n;} // Fix // get type name
    wb_name name(ldh_eName type) { wb_name n; return n;} // Fix

    pwr_tStatus sts() { return m_sts;}    
};

#endif
