#ifndef wb_adrep_h
#define wb_adrep_h

#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "wb_name.h"

class wb_bdrep;
class wb_cdrep;
class wb_orepdbs;
class wb_vrep;
class wb_bdrep;
class wb_name;

class wb_adrep 
{
  int m_nRef;
  wb_orepdbs *m_orep; 
  pwr_tStatus m_sts;
   
  size_t m_size;
  pwr_eType m_type;
  int m_offset;
  int m_elements;
  int m_paramindex;
  int m_flags;
  pwr_tTid m_tid;
  pwr_tPgmName m_pgmname;
  pwr_tCid m_subClass;

  friend class wb_bdrep;
  friend class wb_cdrep;

public:
  wb_adrep(wb_orepdbs& );
  ~wb_adrep();

  void unref();
  wb_adrep *ref();

  wb_adrep *next( pwr_tStatus *sts);
  wb_adrep *prev( pwr_tStatus *sts);
  wb_bdrep *bdrep();
  wb_cdrep *cdrep();

  pwr_sAttrRef aref();
  size_t size() {return m_size;}
  int offset() {return m_offset;}
  pwr_eType type() const {return m_type;}
  pwr_tTid tid() {return m_tid;}
  int nElement() {return m_elements;}
  int index() {return m_paramindex;}
  pwr_tOid aoid();
  int aix();
  pwr_tCid cid();
  pwr_eBix bix();
  int flags() {return m_flags;}
  pwr_tOid boid();
  pwr_tCid subClass() { return m_subClass;}

  const char *name() const;
  wb_name longName();
  void *body( void *p = 0);

  wb_vrep *vrep() const;
};

#endif




