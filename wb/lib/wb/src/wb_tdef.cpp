#include "wb_tdef.h"
#include "wb_adef.h"


wb_tdef::wb_tdef() : wb_status(LDH__NOTYPE), m_tdrep(0)
{
}

wb_tdef::wb_tdef( wb_tdrep *tdrep) : wb_status(LDH__SUCCESS), m_tdrep(tdrep) 
{
  if ( tdrep == 0)
    m_sts = LDH__NOTYPE;
  else
    tdrep->ref();
}

wb_tdef::wb_tdef( wb_adef& a)
{
  const wb_adrep *adrep = a;
  try {
    m_tdrep = new wb_tdrep( *adrep);
    m_tdrep->ref();
    m_sts = m_tdrep->sts();
  }
  catch ( wb_error& e) {
    m_sts = e.sts();
  }
}

wb_tdef::wb_tdef(wb_mvrep *mvrep, pwr_tTid tid)
{
  try {
    m_tdrep = new wb_tdrep( mvrep, tid);
    m_tdrep->ref();
    m_sts = m_tdrep->sts();
  }
  catch ( wb_error& e) {
    m_sts = e.sts();
  }
}

wb_tdef::~wb_tdef()
{
  if ( m_tdrep)
    m_tdrep->unref();
}

wb_tdef::wb_tdef(const wb_tdef& x) : wb_status(x.sts()), m_tdrep(x.m_tdrep)
{
  if ( m_tdrep)
    m_tdrep->ref();
}

wb_tdef& wb_tdef::operator=(const wb_tdef& x)
{
  if ( x.m_tdrep)
    x.m_tdrep->ref();
  if ( m_tdrep)
    m_tdrep->unref();
  m_tdrep = x.m_tdrep;
  m_sts = x.m_sts;
  return *this;
}

void wb_tdef::check() const
{
  if ( !m_tdrep) throw wb_error(m_sts);
}

size_t wb_tdef::size()
{
  check();
  return m_tdrep->size();
}

pwr_tTid wb_tdef::tid()
{
  check();
  return m_tdrep->tid();
}

const char *wb_tdef::name() const
{
  check();
  return m_tdrep->name();
}

wb_name wb_tdef::longName()
{
  check();
  return m_tdrep->longName();
}

