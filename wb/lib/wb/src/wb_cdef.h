#ifndef wb_cdef_h
#define wb_cdef_h

#include "pwr.h"
#include "pwr_class.h"
#include "wb_name.h"

class wb_bdef;
class wb_adef;
class wb_cdrep;
class wb_mvrep;
class wb_object;
class wb_orep;

class wb_cdef : public wb_status
{


public:
  wb_cdrep *m_cdrep;

  wb_cdef();
  wb_cdef( wb_cdrep *cdrep);
  wb_cdef( wb_adef&);  // x = other_object
  wb_cdef(const wb_orep&);  // x = other orep
  wb_cdef(wb_mvrep *, pwr_tCid);
  wb_cdef(const wb_cdef&);
  ~wb_cdef();

  wb_cdef& operator=(const wb_cdef&);
  operator bool() const { return (m_cdrep != 0);}
  operator wb_cdrep*() const {return m_cdrep;}
  
  bool operator==(wb_cdef&);
    
  size_t size( pwr_eBix bix);
  pwr_tCid cid();
  pwr_tOid oid() { pwr_tOid oid = pwr_cNOid; return oid;} // Fix

  const char *name() const;
  wb_name longName();
  pwr_mClassDef flags();

  wb_bdef bdef(pwr_eBix bix);
  wb_bdef bdef(const char *bname);
  wb_bdef bdef(wb_name bname);

  void templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p, pwr_tOid oid);
  wb_object classBody( const char *bname);
  wb_cdef super();

private:
  void check() const;
    
};

#endif


