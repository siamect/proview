#ifndef wb_bdef_h
#define wb_bdef_h

#include "pwr.h"
#include "wb_bdrep.h"
#include "wb_object.h"
#include "wb_name.h"

class wb_orep;
class wb_bdrep;
class wb_adef;

class wb_bdef : public wb_status
{
    int m_nRef;
    wb_bdrep *m_bdrep;

public:
    wb_bdef();
    wb_bdef(wb_bdrep* bdrep);
    wb_bdef(const wb_bdef&);
    wb_bdef& operator=(const wb_bdef&);

    wb_bdef(const wb_adef *a);
    wb_bdef(const wb_orep *o, pwr_tAix bix);

    ~wb_bdef() {}; // Fix

    operator bool() const { return (m_bdrep != 0);}
    operator wb_bdrep*() const;
    bool operator==(wb_bdef&);
    
    pwr_tOid boid();
    pwr_tOix bix();
    size_t size();
    int nAttribute();

    wb_name name(); // get attribute name
    wb_name name(ldh_eName type);

    pwr_sAttrRef aref();
    
    wb_adef adef(); // Get first attribute
    wb_adef adef(pwr_tAix aix);
    wb_adef adef(char *aname);
    wb_adef adef(wb_name aname);
            
};

#endif
