#ifndef wb_mvrep_h
#define wb_mvrep_h

#include "wb_vrep.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"

class wb_mvrep : public wb_vrep {
 public:
  wb_cdrep *cdrep( const wb_orep& o) { return 0;} // Fix
  wb_bdrep *bdrep( const wb_orep& o) { return 0;} // Fix
  wb_adrep *adrep( const wb_orep& o) { return 0;} // Fix
};

#endif
