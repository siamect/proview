
#include "wb_mvrep.h"

wb_cdrep *wb_mvrep::cdrep( pwr_tCid cid)
{
  return new wb_cdrep( this, cid);
}

wb_cdrep *wb_mvrep::cdrep( const wb_orep& o)
{
  return new wb_cdrep( this, o);
}

