#include "wb_cdef.h"


wb_cdef::wb_cdef()
{
}

wb_cdef::wb_cdef(const wb_adef&)
{
}

wb_cdef::wb_cdef(const wb_orep&)
{
}

wb_cdef& wb_cdef::operator=(const wb_cdef&)
{
    return *this;
}


size_t wb_cdef::size()
{
    return m_cdrep->size();
}

pwr_tCid wb_cdef::cid()
{
    return m_cdrep->cid();
}

wb_name wb_cdef::name()
{
    return m_cdrep->name();
}

wb_name wb_cdef::name(ldh_eName type)
{
    return m_cdrep->name(type);
}

wb_bdef wb_cdef::bdef( char *bname) // Fix
{
  wb_bdef b;
  return b;
}






