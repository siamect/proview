#ifndef wb_mvrepdbs_h
#define wb_mvrepdbs_h

#include "pwr.h"
#include "wb_adrep.h"
#include "wb_orep.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"

class wb_adrep;
class wb_bdrep;

class wb_mvrepdbs
{
  pwr_tVid m_vid;
public:


  wb_mvrepdbs();
  wb_mvrepdbs(pwr_tVid vid);

  wb_adrep adrep(wb_orep *o, char *aname);
  wb_adrep adrep(wb_cdrep *c, char *aname);
  wb_bdrep bdrep(wb_bdrep *b, char *aname);
  wb_adrep adrep(pwr_sAttrRef *arp);

};

#endif
