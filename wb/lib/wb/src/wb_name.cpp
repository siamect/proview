#include "wb_name.h"

const char* wb_name::m_emptyString = "";



wb_name::wb_name( const char *name)
{
  if ( name == 0) {
    m_sts = LDH__NONAME;
    m_nrep = 0;
    return;
  }

  try {
    m_nrep = new wb_nrep( name);
    m_sts = LDH__SUCCESS;
    m_nrep->ref();
  }
  catch ( wb_error &e) {
    m_sts = e.sts();
    m_nrep = 0;
  }
}

wb_name::wb_name( wb_nrep *nrep) : wb_status(LDH__SUCCESS), m_nrep(nrep)
{
  if ( m_nrep)
    m_nrep->ref();
  else
    m_sts = LDH__NONAME;
}

wb_name::wb_name( const wb_name& n) : wb_status(n.sts()), m_nrep(n.m_nrep)
{
  if ( oddSts()) m_nrep->ref();
}

wb_name::~wb_name()
{
  if ( oddSts()) m_nrep->unref();
}

wb_name& wb_name::operator=(const wb_name& n)
{
  if ( n.m_nrep)
    n.m_nrep->ref();
  if ( m_nrep)
    m_nrep->unref();
  m_nrep = n.m_nrep;
  m_sts = n.sts();
  return *this;
}
























