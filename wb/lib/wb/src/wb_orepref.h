#ifndef wb_orepref_h
#define wb_orepref_h

#include "wb_orep.h"
#include "wb_vrepref.h"
#include "pwr.h"

class wb_orepref : public wb_orep
{
  wb_vrepref *m_vrep;
  unsigned int m_refCount;
  pwr_tCid m_cid;

  friend class wb_vrepref;
public:

  wb_orepref( wb_vrepref *vrep, int oix) : m_vrep(vrep), m_cid(oix) {}
  ~wb_orepref() {}
    
  virtual pwr_tOid oid() const { pwr_tOid oid = {m_cid, m_vrep->vid()}; return oid;}
  virtual pwr_tVid vid() const { return m_vrep->vid();}
  virtual pwr_tOix oix() const { return m_cid;}

  virtual pwr_tCid cid() const { return m_cid;}
  virtual pwr_tOid poid() const { return pwr_cNObjid;}
  virtual pwr_tOid foid() const { return pwr_cNObjid;}
  virtual pwr_tOid loid() const { return pwr_cNObjid;}
  virtual pwr_tOid boid() const { return pwr_cNObjid;}
  virtual pwr_tOid aoid() const { return pwr_cNObjid;}
    
  virtual const char * name() const { return m_vrep->name();}
  virtual wb_name longName() {
    char str[200];

    m_vrep->objectName( this, str);
    return wb_name( str);
  }
  virtual pwr_tTime ohTime() const { pwr_tTime t = {0,0}; return t;}
  virtual pwr_mClassDef flags() const { pwr_mClassDef m; m.m = 0; return m;}
    
  virtual bool isOffspringOf(const wb_orep *o) const {return false;}
  wb_orep *ancestor(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *parent(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *after(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *before(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *first(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *child(pwr_tStatus *sts, wb_name &name) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *last(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *next(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep  *previous(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  virtual wb_adrep *attribute(pwr_tStatus *sts, const char *name) {*sts = LDH__NOSUCHOBJ; return 0;}
  virtual wb_adrep *attribute(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_erep *erep() const { return m_vrep->erep();}
  wb_vrep *vrep() const { return m_vrep;}

  virtual bool docBlock( char **block, int *size) const {return false;}
  virtual bool docBlock( char *block) {return false;}
};

#endif

