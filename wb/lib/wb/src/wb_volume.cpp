#include "wb_volume.h"
#include "wb_merep.h"

wb_volume::wb_volume() : wb_status(LDH__NOSUCHVOL), m_vrep(0)
{
}

wb_volume::wb_volume(wb_vrep *vrep) : wb_status(LDH__SUCCESS), m_vrep(vrep)
{
  if ( !m_vrep)
    m_sts = LDH__NOSUCHVOL;
  else {
    m_vrep->ref();
  }
}

wb_volume::~wb_volume()
{
  if ( m_vrep)
    m_vrep->unref();
}

wb_volume::wb_volume( const wb_volume &v) : wb_status(v.sts()), m_vrep(v.m_vrep)
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
    wb_object o;
    
    if (oid.vid == m_vrep->vid())
      // This volume
      orep = m_vrep->object( &sts, oid);
    else
      // Other volume
      orep = m_vrep->erep()->object(&sts, oid);

    o = wb_object(sts, orep);
    
    return o;
}

wb_object wb_volume::object(char *name) const
{
    pwr_tStatus sts;
    wb_orep *orep;
    wb_object o;
    
    wb_name n = wb_name( name);
    if ( !n.hasVolume() || n.volumeIsEqual( m_vrep->name()))
      // This volume
      orep = m_vrep->object( &sts, name);
    else
      // Other volume
      orep = m_vrep->erep()->object(&sts, name);

    o = wb_object(sts, orep);
    
    return o;
}

wb_adef wb_volume::adef( pwr_tCid cid, char *bname, char *aname)
{
  pwr_tStatus sts;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, cid);
  if ( EVEN(sts)) return 0;

  wb_cdef cdef = wb_cdef(cdrep);
  wb_bdef bdef = cdef.bdef( bname);
  return bdef.adef( aname);
}

wb_bdef wb_volume::bdef(wb_cdef cdef, char *bname)
{
  return cdef.bdef( bname);
}

wb_bdef wb_volume::bdef(wb_object o, char *bname)
{
  wb_orep *orep = o;
  wb_cdef cdef = wb_cdef(*orep);

  return cdef.bdef( bname);
}

wb_cdef wb_volume::cdef(wb_object o)
{
  pwr_tStatus sts;
  wb_orep *orep = o;
  wb_cdrep *cdrep;
  if ( orep->vrep() == m_vrep)
    // Object in this volume
    cdrep = m_vrep->merep()->cdrep( &sts, *orep);
  else
    // Object in other volume, get class info from this volume's meta environment
    cdrep = m_vrep->erep()->cdrep( &sts, *orep);
  return wb_cdef( cdrep);
}

wb_cdef wb_volume::cdef(pwr_tCid cid)
{
  pwr_tStatus sts;

  // Look in this volume's meta environment only
  return wb_cdef(m_vrep->merep()->cdrep( &sts, cid));
}

wb_cdef wb_volume::cdef(pwr_tOid coid)
{
  return cdef( cdh_ClassObjidToId( coid));
}

wb_cdef wb_volume::cdef(wb_name n)
{
  pwr_tStatus sts;

  // Look in this volume's meta environment only
  return wb_cdef(m_vrep->merep()->cdrep( &sts, n));
}











