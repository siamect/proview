#ifndef wb_tdrep_h
#define wb_tdrep_h

#include "pwr.h"
#include "wb_mvrepdbs.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"

class wb_tdrep 
{
public:
    wb_mvrepdbs *m_vrep;
    dbs_sObject *m_ao;    
    
    wb_tdrep();
    wb_tdrep(wb_mvrepdbs *vrep, dbs_sObject *ao);    
    wb_tdrep(const wb_tdrep&);  // x = other_object
    wb_tdrep& operator=(const wb_tdrep&);

    ~wb_tdrep();

    wb_bdrep *bdef();
    wb_cdrep *cdef();

    pwr_sAttrRef aref();
    size_t size();   // get objects runtime body size
    int offset();
    int type();
    int nElement();
    int index();
    pwr_tOid aoid();
    int aix();
    pwr_tCid cid();
    int bix();
    int flags();
    pwr_tOid boid();

    wb_name name(); // get attribute name
    wb_name name(ldh_eName type);
    
};

#endif
