#include "wb_volume.h"
#include "wb_merep.h"

wb_volume::wb_volume() : wb_status(LDH__NOSUCHVOL), m_vrep(0), m_vid(0)
{
}

wb_volume::wb_volume(wb_vrep *vrep) : wb_status(LDH__SUCCESS), m_vrep(vrep)
{
  if ( !m_vrep)
    m_sts = LDH__NOSUCHVOL;
  else {
    m_vid = m_vrep->vid();
    m_vrep->ref();
  }
}

wb_volume::~wb_volume()
{
  if ( m_vrep)
    m_vrep->unref();
}

wb_volume::wb_volume( const wb_volume &v) : wb_status(v.sts()), m_vrep(v.m_vrep), m_vid(v.vid())
{
  if ( m_vrep)
    m_vrep->ref();
}

wb_volume& wb_volume::operator=(const wb_volume& x)
{
    // Note! first ref() then unref(), because if
    // m_vrep == x.m_vrep, vrep could be deleted.
    if ( x.m_vrep)
      x.m_vrep->ref();
    if ( m_vrep)
      m_vrep->unref();
    m_vrep = x.m_vrep;
    m_sts = x.sts();
    m_vid = x.vid();
    
    return *this;
}

wb_volume::operator bool() const
{
  return (m_vrep != 0);
}

bool wb_volume::operator==(const wb_volume& v) const
{
  if ( m_vrep != 0 && v.m_vrep != 0 && m_vrep == v.m_vrep)
    return true;

  return false;
}

bool wb_volume::operator!=(const wb_volume& v) const
{
  if ( m_vrep != 0 && v.m_vrep != 0 && m_vrep == v.m_vrep)
    return false;

  return true;
}

wb_env wb_volume::env()
{
  wb_env e;

  if ( !m_vrep)
    return e;

  e = wb_env(m_vrep->erep());
  return e;
}

bool wb_volume::isLocal(wb_object &o) const
{
    //return m_vrep == o.m_orep->m_vrep;
    return m_vrep->isLocal(o.m_orep);
}


wb_volume wb_volume::next() const
{
  wb_volume v;
  if ( !m_vrep)
    return v;

  v = wb_volume(m_vrep->next());
  return v;
}

wb_object wb_volume::object() const
{
    pwr_tStatus sts;
    wb_orep *orep;
    
    orep = m_vrep->object(&sts);
    wb_object o(sts, orep);
    
    return o;
}

wb_object wb_volume::object(pwr_tOid oid) const
{
    pwr_tStatus sts;
    wb_orep *orep;
    wb_vrep *vrep;
    wb_object o;
    
    if (oid.vid == m_vrep->vid())
      vrep = m_vrep;
    else
      vrep = m_vrep->erep()->volume(&sts, oid.vid);

    if ( !vrep)
      return o;
    
    orep = vrep->object(&sts, oid);
    o = wb_object(sts, orep);
    
    return o;
}

wb_object wb_volume::object(char *name) const
{
    pwr_tStatus sts;
    wb_orep *orep;
    wb_vrep *vrep;
    wb_object o;
    
    wb_name n = wb_name( name);
    if ( !n.hasVolume() || n.volumeIsEqual( m_vrep->name()))
      vrep = m_vrep;
    else
      vrep = m_vrep->erep()->volume(&sts, n.volume());

    if ( !vrep)
      return o;
    
	 // orep = vrep->object(&sts, name); // Fix
    o = wb_object(sts, orep);
    
    return o;
}

wb_cdef wb_volume::cdef(wb_object o)
{
  pwr_tStatus sts;
  return wb_cdef(m_vrep->erep()->merep()->cdrep( &sts, o.cid()));
}















