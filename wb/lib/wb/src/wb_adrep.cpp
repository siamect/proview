#include "wb_adrep.h"
#include "wb_vrep.h"
#include "wb_cdef.h"
#include "wb_orepdbs.h"

void wb_adrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_adrep *wb_adrep::ref()
{
  m_nRef++;
  return this;
}

wb_adrep::wb_adrep( wb_orepdbs& o): m_nRef(0), m_orep(&o), m_sts(LDH__SUCCESS)
{
  m_orep->ref();
}

wb_adrep::~wb_adrep()
{
  m_orep->unref();
}

wb_cdrep *wb_adrep::cdrep()
{
  return new wb_cdrep( this);
}

wb_bdrep *wb_adrep::bdrep()
{
  return new wb_bdrep( this);
}

pwr_tOid wb_adrep::aoid()
{
    pwr_tOid oid;
    
    //return m_a->oid;
    return oid;
}

size_t wb_adrep::size()
{
    return 0;
}

pwr_sAttrRef wb_adrep::aref()
{
    pwr_sAttrRef aref;

    //aref.Objid = ;
    //aref.Body = bid();
    //aref.Offset = ;
    //aref.Size = ;
    //aref.Flags = ;

    return aref;
}

//
// Return object identity of body that owns this attribute
//
pwr_tOid wb_adrep::boid()
{
    pwr_tOid oid;
    

    //dbs_sBdef *b = (dbs_sBdef *)dbs_Address(sts, m_v->m_env, m_a->bdef);
    
    return oid;
}

//
// Return identity of class that owns this attribute
//
pwr_tCid wb_adrep::cid()
{
    pwr_tCid cid;
    
    //dbs_sCdef *c = (dbs_sCdef*)dbs_Address(sts, m_v->m_env, m_a->cdef);
    
    return cid;
}

wb_vrep *wb_adrep::vrep() const
{
  if (EVEN(m_sts)) throw wb_error(m_sts);
   return m_orep->m_vrep;
}
