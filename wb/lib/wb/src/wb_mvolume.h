#include "pwr.h"
#include "wb_volume.h"
#include "wb_object.h"
#include "wb_attribute.h"

class wb_mvolume
{
    wb_mvrepdbs *m_mvrep;
    int m_vid;
    
public:
    
    wb_volume();
    wb_volume(pwr_tVid vid);

    wb_volume volume(pwr_tVid vid);
    wb_volume volume(char *name);
    
    wb_adef adef(wb_object o, wb_adef adef);
    wb_adef adef(wb_object o, wb_name aname);
    wb_adef adef(wb_name aname);
    wb_adef adef(pwr_tAref *arp);

    wb_bdef();
    
    wb_cdef();
    
};
