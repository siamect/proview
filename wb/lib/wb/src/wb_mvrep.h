#ifndef wb_mvrep_h
#define wb_mvrep_h

#include "wb_vrep.h"

class wb_adrep;
class wb_cdrep;
class wb_orep;
class wb_tdrep;


class wb_mvrep : public wb_vrep {
public:
  wb_cdrep *cdrep( const wb_orep& o);
  wb_cdrep *cdrep( pwr_tCid cid);
  wb_tdrep *tdrep( const wb_adrep& a);
  wb_tdrep *tdrep( pwr_tTid tid);
};

#endif
