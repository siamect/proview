#ifndef WB_ATTRIBUTE_H
#define WB_ATTRIBUTE_H

#include "pwr.h"
#include "wb_orep.h"
#include "wb_name.h"
#include "wb_object.h"
#include "wb_ldh.h"
#include <string>

using namespace std;

/*
  class wb_attribute {
  void *value;

  ...


  wb_attribute::~wb_attribute() {
  if (value != 0)
  free value;
*/

class wb_adrep;
class wb_orep;

class wb_attribute : public wb_status
{
  wb_orep *m_orep;
  wb_adrep *m_adrep;    

  size_t m_size;
  size_t m_offset;
  int m_idx; // -1 if whole array, only valid for arrays 
  pwr_tTid m_tid;
  int m_elements;
  pwr_eType m_type;
  int m_flags;
  
  pwr_eBix m_bix; // Used when sub class
  void *m_body;
  
public:
  wb_attribute();
  wb_attribute(const wb_attribute&);
  wb_attribute(pwr_tStatus, wb_orep*);
  wb_attribute(pwr_tStatus, wb_orep*, wb_adrep*, int idx = -1);
  wb_attribute(pwr_tStatus, wb_orep*, const char *bname);
  wb_attribute(pwr_tStatus, wb_orep*, const char *bname, const char *aname);
  wb_attribute(const wb_attribute& pa, int idx, const char *aname);

  ~wb_attribute();
  wb_attribute& operator=(const wb_attribute&);
  operator bool() const { return oddSts();}
  operator wb_orep*() const { return m_orep;}
  
  bool operator==(const wb_attribute&) const;
  
  //wb_object& operator=(const wb_orep&);

  bool isClass() const {return (m_flags & PWR_MASK_CLASS || m_flags & PWR_MASK_BUFFER);}
  bool isArray() const {return (m_flags & PWR_MASK_ARRAY);}
    

  pwr_tOid aoid() const;  // get objects object id
  pwr_sAttrRef aref() const;
  pwr_sAttrRef *aref(pwr_sAttrRef *arp) const;
  size_t size() const;
  size_t offset() const;
  pwr_eType type() const;
  int nElement() const;
  int index() const;
  int flags() const;
  pwr_tAix aix() const;
  // Class of attribute object
  pwr_tCid cid() const;
  pwr_eBix bix() const;
  pwr_tOid boid() const;
  bool checkXref() const;
  pwr_sAttrXRef *xref() const;
  pwr_sObjXRef *oxref() const;
  pwr_tCid  subClass() const;
    
  void *value( void *p = 0);
  void *value(void *vp, size_t size, pwr_tStatus*);
    
  string toString() const;
  pwr_tStatus fromString(string) const;
  pwr_tStatus fromString(char *) const;
    
    
  wb_attribute after() const;
  wb_attribute before() const;

  wb_attribute first(int idx) const;
  wb_attribute child(int idx, const char *name) const;
    

  const char *name() const;
  wb_name longName() const;
  void name(const char *name);
  void name(wb_name *name);
    
    
  pwr_tStatus sts() const { return m_sts;}
  wb_adrep *adrep() { return m_adrep;}

private:
  void check() const;
  void init();
};

#endif




