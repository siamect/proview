#include "wb_adrep.h"

wb_adrep::wb_adrep(const wb_adrep& x)
{
}

wb_adrep::~wb_adrep()
{
    //m_adrep->unref();
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
