#ifndef wb_env_h
#define wb_env_h

#include "pwr.h"
#include "wb_erep.h"
#include "wb_volume.h"
#include "wb_cdef.h"

class wb_volume;
class wb_erep;

class wb_env : public wb_status
{
protected:
  wb_erep *m_erep;
    
public:

  wb_env();
    
  wb_env(wb_erep*);
  wb_env(wb_env &e);
    
  wb_env& operator=(const wb_env& x);
    
  operator bool() const;
  operator wb_erep*() const;
  bool operator==(wb_env&);
    
  void load( char *db) { m_erep->load(&m_sts, db);}
  bool open();
  bool close();

  // Get first volume in list of db volumes.
  wb_volume volume();
  // Get db volume.
  wb_volume volume(pwr_tVid);

  wb_volume volume(const char *name);
  wb_volume externVolume(pwr_tVid);
  wb_volume bufferVolume();
    
  wb_volume snapshot(pwr_tVid);

  wb_volume createVolume(pwr_tVid vid, pwr_tCid cid, const char *name);

  int nextVolatileVid( char *name) { return m_erep->nextVolatileVid( &m_sts, name); }
    
};

#endif




