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
    int m_offset;
    pwr_tTid m_tid;
    int m_elements;
    pwr_eType m_type;
    int m_flags;
    
    pwr_eBix m_bix; // Used when sub class
  
public:
    wb_attribute();
    wb_attribute(const wb_attribute&);
    wb_attribute(pwr_tStatus, wb_orep*);
    wb_attribute(pwr_tStatus, wb_orep*, wb_adrep*);
    wb_attribute(pwr_tStatus, wb_orep*, const char* bname);
    wb_attribute(pwr_tStatus, wb_orep*, const char* bname, const char* aname);
    wb_attribute(wb_attribute& pa, int idx, const char* aname);

    ~wb_attribute();
    wb_attribute& operator=(const wb_attribute&);
    operator bool() const { return oddSts();}
    operator wb_orep*() const;
    bool operator==(wb_attribute&);
    
    //wb_object& operator=(const wb_orep&);

    bool isClass() const {return (m_flags & PWR_MASK_CLASS || m_flags & PWR_MASK_BUFFER);}
    bool isArray() const {return (m_flags & PWR_MASK_ARRAY);}
    

    pwr_tOid aoid();  // get objects object id
    pwr_sAttrRef aref();
    pwr_sAttrRef *aref(pwr_sAttrRef *arp);
    size_t size();
    int offset();
    pwr_eType type();
    int nElement();
    int index();
    int flags();
    pwr_tAix aix();
    // Class of attribute object
    pwr_tCid cid();
    pwr_eBix bix();
    pwr_tOid boid();
    bool checkXref();
    pwr_sAttrXRef *xref();
    pwr_sObjXRef *oxref();
    pwr_tCid  subClass();
    
    void *value( void *p = 0);
    void *value(void *vp, size_t size, pwr_tStatus*);
    
    string toString();
    pwr_tStatus fromString(string);
    pwr_tStatus fromString(char *);
    
    
    wb_attribute after();
    wb_attribute before();

    wb_attribute first(int idx);
    wb_attribute child(int idx, const char* name);
    

    const char *name() const;
    wb_name longName();
    void name(const char *name);
    void name(wb_name *name);
    
    
    pwr_tStatus sts() { return m_sts;}
            
   private:
    void check() const;
    void init();
};

#endif
