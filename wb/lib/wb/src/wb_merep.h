#ifndef wb_merep_h
#define wb_merep_h

#include "wb_mvrep.h"
#include <map>

using namespace std;

class wb_vrep;

class wb_merep {

    map<pwr_tVid, wb_mvrep*> m_mvrepdbs;

    wb_erep *m_erep;
    wb_vrep *m_vrep;

 public:
    wb_merep( wb_erep *erep, wb_vrep *vrep = 0) : m_erep(erep), m_vrep(vrep) {}
    wb_mvrep *volume(pwr_tStatus *sts);
    wb_mvrep *volume(pwr_tStatus *sts, pwr_tVid vid);
    void addDbs( pwr_tStatus *sts, wb_mvrep *mvrep);
    void removeDbs( pwr_tStatus *sts, wb_mvrep *mvrep);

    wb_cdrep *cdrep( pwr_tStatus *sts, const wb_orep& o);
    wb_cdrep *cdrep( pwr_tStatus *sts, pwr_tCid cid);
};

#endif
