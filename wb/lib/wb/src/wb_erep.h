#ifndef wb_erep_h
#define wb_erep_h

#include "wb_vrep.h"
#include <map>

using namespace std;

class wb_merep;
class wb_vrep;
class wb_cdrep;
class wb_orep;
class wb_tdrep;
class wb_adrep;

class wb_erep
{
    unsigned int m_nRef;
    wb_merep *m_merep;
    map<pwr_tVid, wb_vrep*> m_vrepdb;
    map<pwr_tVid, wb_vrep*> m_vrepdbs;
    map<pwr_tVid, wb_vrep*> m_vrepextern;

    char m_dir_list[10][200];
    int m_dir_cnt;

    typedef map<pwr_tVid, wb_vrep*>::iterator vrep_iterator;

public:
    wb_erep();
    ~wb_erep();
    void unref();
    wb_erep *ref();

    //map<string
    wb_merep *merep() { return m_merep;}
    wb_vrep *volume(pwr_tStatus *sts);
    wb_vrep *volume(pwr_tStatus *sts, pwr_tVid vid);
    wb_vrep *volume(pwr_tStatus *sts, char *name);
    wb_vrep *nextVolume(pwr_tStatus *sts, pwr_tVid vid);
    wb_vrep *externVolume(pwr_tStatus *sts, pwr_tVid vid);
    void addDb( pwr_tStatus *sts, wb_vrep *vrep);
    void addDbs( pwr_tStatus *sts, wb_vrep *vrep);
    void addExtern( pwr_tStatus *sts, wb_vrep *vrep);
    void removeDb( pwr_tStatus *sts, wb_vrep *vrep);
    void removeDbs( pwr_tStatus *sts, wb_vrep *vrep);
    void removeExtern( pwr_tStatus *sts, wb_vrep *vrep);
    void load( pwr_tStatus *sts);

    wb_cdrep *cdrep( pwr_tStatus *sts, const wb_orep& o);
    wb_tdrep *tdrep( pwr_tStatus *sts, const wb_adrep& a);

private:
    void loadDirList( pwr_tStatus *status);
    void loadCommonMeta( pwr_tStatus *status);
    void loadMeta( pwr_tStatus *status);
};

#endif






