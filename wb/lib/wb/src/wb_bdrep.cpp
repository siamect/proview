
#include "wb_bdrep.h"
#include "wb_adrep.h"
#include "wb_orep.h"

void wb_bdrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_bdrep *wb_bdrep::ref()
{
  m_nRef++;
  return this;
}

wb_bdrep::wb_bdrep(wb_orep& o) : m_nRef(0), m_orep(&o), m_sts(LDH__SUCCESS)
{
  m_orep->ref();
}

wb_bdrep::wb_bdrep( wb_adrep *adrep) : m_nRef(0)
{
  pwr_tStatus sts;
  m_orep = adrep->m_orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  m_sts = LDH__SUCCESS;
}

wb_bdrep::~wb_bdrep()
{
  m_orep->unref();
}

wb_adrep *wb_bdrep::adrep( pwr_tStatus *sts)
{
  wb_orep *orep = m_orep->vrep()->first( sts, m_orep);
  if ( EVEN(*sts)) return 0;

  return new wb_adrep(*orep);
}

wb_adrep *wb_bdrep::adrep( pwr_tStatus *sts, const char *aname)
{
  wb_name n(aname);
  wb_orep *orep = m_orep->vrep()->child( sts, m_orep, n);
  if ( EVEN(*sts))
    return 0;
  wb_adrep *adrep = new wb_adrep( *orep);
  return adrep;
}

pwr_eBix wb_bdrep::bix()
{
  return  cdh_oixToBix( m_orep->oid().oix);
}

size_t wb_bdrep::size()
{
  pwr_tStatus sts;
  pwr_sObjBodyDef body;

  m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &body);
  if ( EVEN(sts)) throw wb_error(sts);

  return body.Size;
}

int wb_bdrep::nAttribute()
{
  pwr_tStatus sts;
  int attr_count = 0;
  wb_orep *old;

  wb_orep *orep = m_orep->vrep()->first( &sts, m_orep);
  while ( ODD(sts)) {
    switch ( orep->cid()) {
    case pwr_eClass_Param:
    case pwr_eClass_Intern:
    case pwr_eClass_Input:
    case pwr_eClass_Output:
    case pwr_eClass_ObjXRef:
    case pwr_eClass_AttrXRef:
    case pwr_eClass_Buffer:
      attr_count++;
      break;
    default:
      ;
    }
    old = orep;
    orep = orep->after( &sts);
    old->ref();
    old->unref();
  }
  return attr_count;
}

pwr_tOid wb_bdrep::boid() 
{ 
  return m_orep->oid();
}

pwr_tCid wb_bdrep::bcid() 
{ 
  return m_orep->cid();
}


const char* wb_bdrep::name() const
{
  return m_orep->name();
}

wb_name wb_bdrep::longName() const
{
  return m_orep->longName();
}
