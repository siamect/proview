#ifndef wb_cdrep_h
#define wb_cdrep_h

#include "pwr.h"
#include "pwr_class.h"
#include "wb_name.h"
#include "wb_orep.h"

class wb_adef;
class wb_orep;
class wb_mvrep;
class wb_bdrep;
class wb_orepdbs;
class wb_adrep;

class wb_cdrep 
{
    int m_nRef;
    wb_orepdbs *m_orep;
    pwr_tStatus m_sts;

 public:
    wb_cdrep();
    wb_cdrep(wb_adrep *);  // x = other_object
    wb_cdrep(const wb_orep&);  // x = other orep
    wb_cdrep(wb_mvrep *, const wb_orep&);
    wb_cdrep(wb_mvrep *, pwr_tCid);
    wb_cdrep(wb_mvrep *, wb_name);

    void unref();
    wb_cdrep *ref();

    //wb_object& operator=(const wb_orep&);

    size_t size() { return 0;}   // Fix get objects runtime body size
    pwr_tCid cid();
    

    const char *name() const;
    wb_name longName();
    void name(const char *name);
    void name(wb_name *name);
    
    wb_bdrep *bdrep( pwr_tStatus *sts, int bix);
    wb_bdrep *bdrep( pwr_tStatus *sts, const char *bname);
    wb_adrep *adrep( pwr_tStatus *sts, const char *aname);    
    wb_orep *classBody( pwr_tStatus *sts, const char *bname);

    void templateBody( pwr_tStatus *sts, cdh_eBix bix, void *p);

    pwr_tStatus sts() { return m_sts;}
};

#endif
