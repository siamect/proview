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

public:
    wb_attribute();
    wb_attribute(const wb_attribute&);
    wb_attribute(pwr_tStatus, wb_orep* const );
    wb_attribute(pwr_tStatus, wb_orep* const, wb_adrep* const);
    wb_attribute(pwr_tStatus, wb_orep* const, char* aname, char* bname = 0);

    ~wb_attribute();
    wb_attribute& operator=(const wb_attribute&);
    operator bool() const { return evenSts();}
    operator wb_orep*() const;
    bool operator==(wb_attribute&);
    
    //wb_object& operator=(const wb_orep&);

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
    pwr_tAix bix();
    pwr_tOid boid();
    bool checkXref();
    pwr_sAttrXRef *xref();
    pwr_sObjXRef *oxref();
    
    void *value( void *p = 0);
    void *value(void *vp, size_t size, pwr_tStatus*);
    
    string toString();
    pwr_tStatus fromString(string);
    pwr_tStatus fromString(char *);
    
    
    wb_attribute next();
    wb_attribute prev();
    

    wb_name name(); // get attribute name
    wb_name name(ldh_eName type);
    void name(const char *name);
    void name(wb_name *name);
    
    
    pwr_tStatus sts() { return m_sts;}
            
   private:
    void check();
    void init();
};

#endif
