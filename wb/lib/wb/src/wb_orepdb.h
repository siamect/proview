#ifndef wb_orepdb_h
#define wb_orepdb_h

#include "pwr.h"
#include "wb_orep.h"
#include "pwr_class.h"
#include "wb_db.h"

class wb_orepdb : wb_orep
{
  friend class wb_vrepdb;
  db_sObject *m_o;
  pwr_tOid m_oid;
    
public:
    
  wb_orepdb();
  wb_orepdb(pwr_tOid oid);
  wb_orepdb(db_sObject *o);
  ~wb_orepdb();

  virtual pwr_tOid oid() const;
  virtual pwr_tVid vid() const;
  virtual pwr_tOix oix() const;

  virtual pwr_tCid cid() const;
  virtual pwr_tOid poid() const;
  virtual pwr_tOid foid() const;
  virtual pwr_tOid loid() const;
  virtual pwr_tOid boid() const;
  virtual pwr_tOid aoid() const;
    
  virtual const char * name() const;
  virtual wb_name longName();
  virtual pwr_tTime ohTime() const;
    
  virtual bool isOffspringOf(const wb_orep *o) const;
    
  // Navigational operations

  virtual wb_orep *ancestor(pwr_tStatus *sts) const;             //< get object at top of hierarchy
  virtual wb_orep *parent(pwr_tStatus *sts) const;
  virtual wb_orep *after(pwr_tStatus *sts) const;                //< get next sibling
  virtual wb_orep *before(pwr_tStatus *sts) const;               //< get previous sibling
  virtual wb_orep *first(pwr_tStatus *sts) const;                //< get first child
  virtual wb_orep *child(pwr_tStatus *sts, const char *name) const;    //< get named child
  virtual wb_orep *last(pwr_tStatus *sts) const;                 //< get last child
  virtual wb_orep *next(pwr_tStatus *sts) const;                 //< get next in list of objects of same class in one volume
  virtual wb_orep *previous(pwr_tStatus *sts) const;             //< get previous in list of objects of same class in one volume

  virtual wb_position position();
  virtual wb_position position(ldh_eDest dest);

  virtual wb_adrep *attribute(pwr_tStatus*, const char *aname);
  virtual wb_adrep *attribute(pwr_tStatus*);

  wb_erep *erep() const { return m_vrep->erep();}
  wb_vrep *vrep() const { return m_vrep;}
};

#endif
