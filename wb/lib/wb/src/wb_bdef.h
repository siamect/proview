#ifndef wb_bdef_h
#define wb_bdef_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_status.h"

class wb_orep;
class wb_bdrep;
class wb_adef;
class wb_name;

class wb_bdef : public wb_status
{
  int m_nRef;
  wb_bdrep *m_bdrep;

public:
  wb_bdef();
  wb_bdef(wb_bdrep* bdrep);
  wb_bdef(const wb_bdef&);
  wb_bdef& operator=(const wb_bdef&);

  wb_bdef(const wb_adef *a);
  wb_bdef(const wb_orep *o, pwr_eBix bix);

  ~wb_bdef();

  operator bool() const { return (m_bdrep != 0);}
  operator wb_bdrep*() const;
  bool operator==(wb_bdef&);
    
  pwr_tOid boid();
  pwr_eBix bix();
  size_t size();
  int nAttribute();

  const char* name() const; // get body name
  wb_name longName() const;

  pwr_sAttrRef aref();
    
  wb_adef adef(); // Get first attribute
  wb_adef adef(pwr_tAix aix);
  wb_adef adef(const char *aname);
  wb_adef adef(wb_name aname);
            
};

#endif
