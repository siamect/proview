#include "wb_adrep.h"
#include "wb_bdrep.h"
#include "wb_vrep.h"
#include "wb_cdef.h"
#include "wb_cdrep.h"
#include "wb_orep.h"

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

wb_adrep::wb_adrep( wb_orep& o): m_nRef(0), m_orep(&o), m_sts(LDH__SUCCESS),
                                    m_subClass(pwr_eClass__)
{
  m_orep->ref();

  pwr_tStatus sts;
  switch ( m_orep->cid()) {
  case pwr_eClass_Param:
  {
    pwr_sParam attr;

    m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &attr);
    if ( EVEN(sts)) throw wb_error(sts);

    strcpy( m_pgmname, attr.Info.PgmName);
    m_size = attr.Info.Size;
    m_type = attr.Info.Type;
    m_offset = attr.Info.Offset;
    m_elements = attr.Info.Elements;
    m_paramindex = attr.Info.ParamIndex;
    m_flags = attr.Info.Flags;
    m_tid = attr.TypeRef;
    break;
  }
  case pwr_eClass_Intern:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  {
    pwr_sIntern attr;

    m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &attr);
    if ( EVEN(sts)) throw wb_error(sts);

    strcpy( m_pgmname, attr.Info.PgmName);
    m_size = attr.Info.Size;
    m_type = attr.Info.Type;
    m_offset = attr.Info.Offset;
    m_elements = attr.Info.Elements;
    m_paramindex = attr.Info.ParamIndex;
    m_flags = attr.Info.Flags;
    m_tid = attr.TypeRef;


    break;
  }
  case pwr_eClass_ObjXRef:
  {
    pwr_sObjXRef attr;

    m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &attr);
    if ( EVEN(sts)) throw wb_error(sts);

    strcpy( m_pgmname, attr.Info.PgmName);
    m_size = attr.Info.Size;
    m_type = attr.Info.Type;
    m_offset = attr.Info.Offset;
    m_elements = attr.Info.Elements;
    m_paramindex = attr.Info.ParamIndex;
    m_flags = attr.Info.Flags;
    m_tid = 0;

    break;
  }
  case pwr_eClass_AttrXRef:
  {
    pwr_sAttrXRef attr;

    m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &attr);
    if ( EVEN(sts)) throw wb_error(sts);

    strcpy( m_pgmname, attr.Info.PgmName);
    m_size = attr.Info.Size;
    m_type = attr.Info.Type;
    m_offset = attr.Info.Offset;
    m_elements = attr.Info.Elements;
    m_paramindex = attr.Info.ParamIndex;
    m_flags = attr.Info.Flags;
    m_tid = 0;

    break;
  }
  case pwr_eClass_Buffer:
  {
    pwr_sBuffer attr;

    m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, (void *) &attr);
    if ( EVEN(sts)) throw wb_error(sts);

    strcpy( m_pgmname, attr.Info.PgmName);
    m_size = attr.Info.Size;
    m_type = attr.Info.Type;
    m_offset = attr.Info.Offset;
    m_elements = attr.Info.Elements;
    m_paramindex = attr.Info.ParamIndex;
    m_flags = attr.Info.Flags;
    m_flags |= PWR_MASK_BUFFER;
    m_tid = 0;
    m_subClass = attr.Class;

    break;
  }
  default:
    throw wb_error(LDH__NYI);
  }
}

wb_adrep::~wb_adrep()
{
  m_orep->unref();
}

wb_adrep *wb_adrep::next( pwr_tStatus *sts)
{
  wb_orep *orep = m_orep->after( sts);
  if ( EVEN(*sts))
    return 0;

  wb_adrep *adrep = new wb_adrep( (wb_orep& ) *orep);
  return adrep;
}

wb_adrep *wb_adrep::prev( pwr_tStatus *sts)
{
  wb_orep *orep = m_orep->before( sts);
  if ( EVEN(*sts))
    return 0;

  wb_adrep *adrep = new wb_adrep( (wb_orep& ) *orep);
  return adrep;
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
  return m_orep->oid();
}

int wb_adrep::aix()
{
  return cdh_oixToAix( m_orep->oid().oix);
}

pwr_eBix wb_adrep::bix()
{
  return (pwr_eBix) cdh_oixToBix( m_orep->oid().oix);
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
  return m_orep->cid();
}

wb_vrep *wb_adrep::vrep() const
{
  if (EVEN(m_sts)) throw wb_error(m_sts);
  return m_orep->vrep();
}

const char *wb_adrep::name() const
{
  return m_orep->name();
}

wb_name wb_adrep::longName()
{
  return m_orep->longName();
}

void *wb_adrep::body( void *p)
{
  pwr_tStatus sts;
  int size;

  switch ( m_orep->cid()) {
  case pwr_eClass_Param:
    size = sizeof( pwr_sParam);
    break;
  case pwr_eClass_Intern:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
    size = sizeof( pwr_sIntern);
    break;
  case pwr_eClass_ObjXRef:
    size = sizeof( pwr_sObjXRef);
    break;
  case pwr_eClass_AttrXRef:
    size = sizeof( pwr_sAttrXRef);
    break;
  case pwr_eClass_Buffer:
    size = sizeof( pwr_sBuffer);
    break;
  default:
    throw wb_error(LDH__NYI);
  }

  return m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_sys, p);
}
