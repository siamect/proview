#ifndef wb_tdef_h
#define wb_tdef_h

#include "wb_tdrep.h"
#include "pwr.h"
#include "wb_orep.h"
#include "wb_name.h"
#include "wb_ldh.h"

class wb_tdrep;
class wb_orep;

class wb_tdef : public wb_status
{
public:
  wb_tdrep *m_tdrep;
    
  wb_tdef();
  wb_tdef( wb_tdrep *tdrep);
  wb_tdef( wb_adef&);
  wb_tdef(wb_mvrep *, pwr_tTid);
  wb_tdef(const wb_tdef&);
  wb_tdef& operator=(const wb_tdef&);
  ~wb_tdef();

  operator bool() const { return m_tdrep != 0;}
  operator wb_tdrep*() const;
  bool operator==(wb_tdef&);
    
  size_t size();
  pwr_tTid tid();
  int nElement();
  const char *name() const;
  wb_name longName();
    
private:
  void check() const;            
};

#endif



