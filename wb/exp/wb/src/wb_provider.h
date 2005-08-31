#ifndef wb_provider_h
#define wb_provider_h


#include "wb_vext.h"

class wb_procom;

class wb_provider {
public:
  virtual void object( wb_procom *pcom) {}
  virtual void objectOid( wb_procom *pcom, pwr_tOix oix) {}
  virtual void objectName( wb_procom *pcom, char *name) {}
  virtual void objectBody( wb_procom *pcom, pwr_tOix oix) {}
};


class wb_procom 
{
  char m_key[200];
  int m_connected;
  int m_msgsndid;
  int m_msgrcvid;
  wb_provider *m_provider;

public:
  wb_procom( char *key, wb_provider *p) : m_connected(0), m_provider(p)
  {
    strcpy( m_key, key);
  }
  void put( vext_sAMsg *msg, int size, pwr_tStatus *sts);
  void receive( vext_sQMsg *msg, int size, pwr_tStatus *sts);
  void mainloop();
  void provideObject( pwr_tStatus sts, pwr_tOix oix, pwr_tOix fthoix, pwr_tOix bwsoix,
		      pwr_tOix fwsoix, pwr_tOix fchoix, pwr_tOix lchoix, pwr_tCid cid,
		      char *name, char *longname);
  void provideBody( pwr_tStatus sts, pwr_tOix oix, int size, void *body);
};


#endif

