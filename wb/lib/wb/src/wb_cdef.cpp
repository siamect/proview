#include "wb_cdef.h"
#include "wb_adef.h"


wb_cdef::wb_cdef() : wb_status(LDH__NOCLASS), m_cdrep(0)
{
}

wb_cdef::wb_cdef( wb_cdrep *cdrep) : wb_status(cdrep->sts()), m_cdrep(cdrep) 
{
  cdrep->ref();
}

wb_cdef::wb_cdef( wb_adef& a)
{
  wb_adrep *adrep = a;
  m_cdrep = new wb_cdrep( adrep);
  m_cdrep->ref();
  m_sts = m_cdrep->sts();
}

wb_cdef::wb_cdef(const wb_orep& x)
{
  m_cdrep = new wb_cdrep( x);
  m_cdrep->ref();
  m_sts = m_cdrep->sts();
}

wb_cdef::wb_cdef(wb_mvrep *mvrep, pwr_tCid cid)
{
  m_cdrep = new wb_cdrep( mvrep, cid);
  m_cdrep->ref();
  m_sts = m_cdrep->sts();
}

wb_cdef::~wb_cdef()
{
  if ( m_cdrep)
    m_cdrep->unref();
}

wb_cdef::wb_cdef(const wb_cdef& x) : wb_status(x.sts()), m_cdrep(x.m_cdrep)
{
  if ( m_cdrep)
    m_cdrep->ref();
}

wb_cdef& wb_cdef::operator=(const wb_cdef& x)
{
  if ( x.m_cdrep)
    x.m_cdrep->ref();
  if ( m_cdrep)
    m_cdrep->unref();
  m_cdrep = x.m_cdrep;
  m_sts = x.m_sts;
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
  pwr_tStatus sts;

  wb_bdrep *bdrep = m_cdrep->bdrep( &sts, bname);
  if ( ODD(sts))
    return wb_bdef( bdrep);
  else {
    delete bdrep;
    return wb_bdef();
  }
}






