#include "wb_adef.h"

wb_adef::wb_adef() : wb_status(LDH__NOSUCHATTR), m_adrep(0)
{
}

wb_adef::wb_adef( wb_adrep *adrep) : wb_status(LDH__SUCCESS), m_adrep(adrep)
{
  if ( adrep == 0)
    m_sts = LDH__NOSUCHATTR;
  else
    m_adrep->ref();
}

wb_adef::wb_adef(const wb_adef& x) : wb_status(x.m_sts), m_adrep(x.m_adrep)
{
  if ( m_adrep)
    m_adrep->ref();
}

wb_adef& wb_adef::operator=(const wb_adef& x)
{
  if ( x.m_adrep)
    x.m_adrep->ref();
  if ( m_adrep)
    m_adrep->unref();

  m_adrep = x.m_adrep;
  m_sts = x.m_sts;
  return *this;
}

wb_adef::~wb_adef()
{
  if ( m_adrep)
    m_adrep->unref();
}

void wb_adef::check()
{
  if ( evenSts()) throw wb_error(m_sts);
}

wb_adef wb_adef::next()
{
  pwr_tStatus sts;

  check();
  return wb_adef( m_adrep->next( &sts));
}

wb_adef wb_adef::prev()
{
  pwr_tStatus sts;

  check();
  return wb_adef( m_adrep->prev( &sts));
}


// get objects runtime body size
size_t wb_adef::size() 
{ 
  check();
  return m_adrep->size();
}

int wb_adef::offset() 
{ 
  check();
  return m_adrep->offset();
}

int wb_adef::type() 
{
  check();
  return m_adrep->type();
}

int wb_adef::nElement()
{
  check();
  return m_adrep->nElement();
}

int wb_adef::index() 
{
  check();
  return m_adrep->index();
}

pwr_tOid wb_adef::aoid() 
{
  check();
  return m_adrep->aoid();
}

pwr_tAix wb_adef::aix() 
{
  check();
  return m_adrep->aix();
}

pwr_tCid wb_adef::cid() 
{
  check();
  return m_adrep->cid();
}

pwr_tOix wb_adef::bix() 
{
  check();
  return m_adrep->bix();
}

pwr_tOid wb_adef::boid() 
{
  check();
  return m_adrep->boid();
}

char *wb_adef::name()
{
  check();
  return m_adrep->name();
}

wb_name wb_adef::longName()
{
  check();
  return m_adrep->longName();
}

void wb_adef::body( void **p)
{
  check();
  m_adrep->body( p);
}
