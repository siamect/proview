#ifndef wb_import_h
#define wb_import_h

#include "pwr.h"
#include "co_dbs.h"

class wb_export;

class wb_import
{
public:

  virtual bool importVolume(wb_export &e) = 0;
    
  virtual bool importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                          const char *name, const char *normname,
                          pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                          size_t rbSize, size_t dbSize) = 0;

  virtual bool importRbody(pwr_tOid oid, size_t size, void *body) = 0;
    
  virtual bool importDbody(pwr_tOid oid, size_t size, void *body) = 0;
    
  virtual bool importMeta(dbs_sEnv *ep) = 0;
    
};

#endif


