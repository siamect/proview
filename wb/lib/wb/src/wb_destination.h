#ifndef wb_destination_h
#define wb_destination_h

#include "pwr.h"
#include "wb_ldh.h"

class wb_object;

class wb_destination
{
  pwr_tOid m_oid;
  ldh_eDest m_code;

public:
  wb_destination(wb_object &object, ldh_eDest code);
  wb_destination(pwr_tOid oid, ldh_eDest code);
    
  pwr_tOid oid() { return m_oid;}
  ldh_eDest code() { return m_code;}    
};

#endif



