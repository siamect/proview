#ifndef wb_destination_h
#define wb_destination_h

#include "pwr.h"
#include "wb_ldh.h"

class wb_object;

class wb_destination
{
public:

  pwr_tOid m_oid;
  ldh_eDest m_code;

  wb_destination(wb_object &object, ldh_eDest code);
  wb_destination(pwr_tOid oid, ldh_eDest code);
    

  bool canAdopt(pwr_tCid cid) {return true;} // Fix
  bool canAdopt(pwr_tOid oid) {return true;} // Fix
  pwr_tOid oid() { return m_oid;}
  ldh_eDest code() { return m_code;}
    
};

#endif



