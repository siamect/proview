#ifndef wb_tdrep_h
#define wb_tdrep_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_name.h"

class wb_adrep;
class wb_mrepdbs;
class wb_orepdbs;
class wb_mvrep;

class wb_tdrep
{
    int m_nRef;
    wb_orepdbs *m_orep;
    int m_sts;

    pwr_eType m_type;
    size_t m_size;
    int m_elements;
    pwr_tTid m_typeref;
    
public:
    wb_tdrep();
    wb_tdrep(wb_mvrep *, pwr_tTid);
    wb_tdrep( wb_mvrep *mvrep, wb_name name);
    wb_tdrep( const wb_adrep& a);
    wb_tdrep(wb_mvrep *, const wb_adrep&);

    void unref();
    wb_tdrep *ref();

    size_t size() { return m_size;}
    pwr_tTid tid();
    pwr_tTid typeRef() {return m_typeref;}
    pwr_eType type() { return m_type;}
    int nElement() { return m_elements;}

    const char *name() const;
    wb_name longName();

    pwr_tStatus sts() { return m_sts;}    

 private:
    void init();
};

#endif
