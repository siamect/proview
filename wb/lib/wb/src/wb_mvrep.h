#ifndef wb_mvrep_h
#define wb_mvrep_h

#include "wb_vrep.h"
#include "wb_cdrep.h"
#include "wb_tdrep.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"

class wb_mvrep : public wb_vrep {
 public:
  wb_cdrep *cdrep( const wb_orep& o);
  wb_cdrep *cdrep( pwr_tCid cid);
  wb_tdrep *tdrep( const wb_adrep& a);
  wb_tdrep *tdrep( pwr_tTid tid);
};

#endif
