#ifndef wb_volume_h
#define wb_volume_h

#include "pwr.h"
#include "wb_vrep.h"
#include "wb_object.h"
#include "wb_attribute.h"
#include "wb_env.h"
#include "wb_adef.h"
#include "wb_bdef.h"
#include "wb_cdef.h"
#include "wb_tdef.h"
#include "wb_name.h"
#include "wb_attrname.h"

class wb_env;
class wb_vrep;
class wb_attribute;
class wb_adef;
class wb_object;

class wb_volume : public wb_status
{
protected:
  wb_vrep *m_vrep;

public:
    
  wb_volume();
  wb_volume(wb_vrep *vrep);
  wb_volume(const wb_volume&);
    
  ~wb_volume();

  wb_volume& operator=(const wb_volume& v);

  operator bool() const;
  operator wb_vrep*() const { return m_vrep;}
  bool operator==(const wb_volume& v) const;
  bool operator!=(const wb_volume& v) const;

  wb_env env();
    
  pwr_tVid vid() const { return m_vrep->vid();}
  pwr_tCid cid() const { return m_vrep->cid();}
  const char *name() const { return m_vrep->name();}
  ldh_eVolRep type() const { return m_vrep->type();}

  wb_volume next() const;

  wb_object object() const; // Get root list
  wb_object object(pwr_tOid oid) const;
  wb_object object(pwr_tCid cid) const;
  wb_object object(const char *name) const;
    
  wb_attribute attribute(pwr_tOid oid, const char *bname, const char *aname) const;
  wb_attribute attribute(pwr_tOid oid, const char *bname) const;
  wb_attribute attribute(wb_object o, wb_adef adef) { wb_attribute a; return a;}; // Fix
  wb_attribute attribute(wb_object o, wb_attrname aname);
  wb_attribute attribute(wb_name aname);
  wb_attribute attribute(const pwr_sAttrRef *arp) const;
  wb_attribute attribute() { wb_attribute a; return a;}; // Fix

  wb_adef adef(pwr_sAttrRef *arp) { wb_adef a; return a;}; // Fix
  wb_adef adef(pwr_tCid cid, const char *bname, const char *aname);
    
  wb_cdef cdef(wb_object o);
  wb_cdef cdef(pwr_tCid cid);
  wb_cdef cdef(pwr_tOid coid);
  wb_cdef cdef(wb_name n);

  //wb_tdef tdef(wb_object o);
  wb_tdef tdef(pwr_tTid tid) { wb_tdef t; return t;} // Fix
  //wb_tdef tdef(pwr_tOid coid);
  //wb_tdef tdef(wb_name n);

  void readAttribute();
  void readBody();
    
  bool isLocal(wb_object &o) const;
    
  bool createSnapshot(const char *fileName) { return m_vrep->createSnapshot(fileName);}
  bool exportTree( wb_volume &import, pwr_tOid oid);
    
  pwr_tStatus syntaxCheck( int *errorcount, int *warningcount);
  pwr_tStatus syntaxCheckObject( wb_object& o, int *errorcount, int *warningcount);
  pwr_tStatus triggSyntaxCheck( wb_object& o, int *errorcount, int *warningcount);
  pwr_tStatus triggAnteAdopt( wb_object& o, pwr_tCid cid);
  pwr_tStatus triggAnteCreate( wb_object& father, pwr_tCid cid);
  pwr_tStatus triggAnteMove( wb_object& o, wb_object& father);
  pwr_tStatus triggAnteUnadopt( wb_object& father, wb_object& o);
  pwr_tStatus triggPostAdopt( wb_object& father, wb_object& o);
  pwr_tStatus triggPostCreate( wb_object& o);
  pwr_tStatus triggPostMove( wb_object& o);
  pwr_tStatus triggPostUnadopt( wb_object& father, wb_object& o);
};

#endif










