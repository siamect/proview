#ifndef wb_tdef_h
#define wb_tdef_h

#include "wb_tdrep.h"
#include "pwr.h"
#include "wb_orep.h"
#include "wb_name.h"
#include "wb_ldh.h"

class wb_tdrep;
class wb_orep;

class wb_tdef : public wb_status
{
public:
    wb_tdrep *m_tdrep;
    
    wb_tdef() : wb_status(LDH__NOTYPE), m_tdrep(0) {}; // Fix
    wb_tdef(const wb_tdef& x) : wb_status(x.sts()), m_tdrep(x.m_tdrep) {}  // Fix // x = other_object
    wb_tdef(const wb_adrep*);  // 
    wb_tdef& operator=(const wb_tdef&);

    ~wb_tdef() {} // Fix

    operator bool() const { return (m_tdrep != 0);} // Fix 
    operator wb_tdrep*() const;
    bool operator==(wb_tdef&);
    
    size_t size();
    int offset();
    int type();
    int nElement();
    int index();
    pwr_tOid aoid();
    pwr_tAix aix();
    pwr_tCid cid();
    pwr_tOix bix();
    pwr_tOid boid();
    
    wb_tdef next();
    wb_tdef prev();

    wb_name name();
    wb_name name(ldh_eName type);
    
            
};

#endif



