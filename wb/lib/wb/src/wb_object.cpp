#include "wb_ldh_msg.h"
#include "wb_object.h"
#include "wb_location.h"
#include "wb_error.h"

bool wb_object::check( string str)
{
    if (m_orep == 0) {
        m_sts = LDH__NOSUCHOBJ;//LDH__NOOBJECT;
        throw wb_error_str(m_sts, str);
    }
    return true;
}

bool wb_object::check()
{
    if (m_orep == 0) {
        m_sts = LDH__NOSUCHOBJ;//LDH__NOOBJECT;
        throw wb_error(m_sts);
    }
    return true;
}

// Return the current position of this object
wb_location wb_object::location()
{
    return wb_location(*this);
}

wb_destination wb_object::destination(ldh_eDest dest)
{
    return wb_destination(*this, dest);
}


wb_object::wb_object(wb_orep *orep) : wb_status(LDH__SUCCESS), m_orep(orep)
{
    if (orep == 0)
      m_sts = LDH__NOSUCHOBJ;
    else
      m_orep->ref();
}

wb_object::wb_object(pwr_tStatus sts, wb_orep *orep) : wb_status(sts), m_orep(orep)
{
    if (m_orep != 0)
        m_orep = orep->ref();
}


wb_object::wb_object() : wb_status(LDH__NOSUCHOBJ), m_orep(0)
{
}

wb_object::wb_object(const wb_object& x) : wb_status(x.sts()), m_orep(x.m_orep)
{
  if ( m_orep)
    m_orep->ref();
}

wb_object::~wb_object()
{
  if ( m_orep)
    m_orep->unref();
}

wb_object& wb_object::operator=(const wb_object& x)
{
    // Note! first ref() then unref(), because if
    // m_orep == x.m_orep, orep could be deleted.
    if ( x.m_orep)
      x.m_orep->ref();
    if ( m_orep)
      m_orep->unref();
    m_orep = x.m_orep;
    m_sts = x.sts();
    
    return *this;
}

wb_object::operator bool() const
{
  return (m_orep != 0);
}

wb_object::operator wb_orep*() const
{
    return m_orep;
}

const char *wb_object::name()
{ 
  check("wb_object::name()");

  return m_orep->name();
}

wb_name wb_object::longName()
{ 
  check("wb_object::longName()");

  return m_orep->longName();
}

//
// Get next object of same class
//
wb_object wb_object::next()
{
    check("wb_object::next()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->next(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get previous object of same class
//
wb_object wb_object::previous()
{
    check("wb_object::previous()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->previous(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get the sibling after the current object
//
wb_object wb_object::after()
{
    check("wb_object::after()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->after(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get the sibling before the current object
//
wb_object wb_object::before()
{
    check("wb_object::before()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->before(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get the parent of the current object
//
wb_object wb_object::parent()
{
    check("wb_object::parent()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->parent(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get the first child of the current object.
//
wb_object wb_object::first()
{
    check("wb_object::first()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->first(&sts);
    wb_object o(sts, orep);
    return o;
}

//
// Get the child with the given name.
//
wb_object wb_object::child( const char* name)
{
    check("wb_object::child()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->child(&sts, name);
    wb_object o(sts, orep);
    return o;
}

//
// Get the last child of the current object.
//
wb_object wb_object::last()
{
    check("wb_object::last()");
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_orep *orep = m_orep->last(&sts);
    wb_object o(sts, orep);

    return o;
}

pwr_tOid wb_object::oid()
{
    check();
    
    return m_orep->oid();
}

pwr_tVid wb_object::vid()
{
    check();
    
    return m_orep->vid();
}

pwr_tCid wb_object::cid()
{
    check();
    
    return m_orep->cid();
}

pwr_tOix wb_object::oix()
{
    check();
    
    return m_orep->oix();
}

pwr_tOid wb_object::poid()
{
    check();
    
    return m_orep->poid();
}

pwr_tOid wb_object::foid()
{
    check();
    
    return m_orep->foid();
}

pwr_tOid wb_object::loid()
{
    check();
    
    return m_orep->loid();
}

pwr_tOid wb_object::boid()
{
    check();
    
    return m_orep->boid();
}

pwr_tOid wb_object::aoid()
{
    check();
    
    return m_orep->aoid();
}

pwr_tTime wb_object::ohTime()
{
    check();
    
    return m_orep->ohTime();
}

bool wb_object::operator==(wb_object& x)
{
    check();
    
    return (x.m_orep == m_orep);
}

wb_attribute wb_object::attribute()
{
    check();
    
    pwr_tStatus sts = LDH__SUCCESS;
    wb_attribute a(sts, m_orep);

    return a;
}

wb_attribute wb_object::attribute(const char *aname)
{
    check();

    pwr_tStatus sts = LDH__SUCCESS;
    wb_attribute a(sts, m_orep, aname);

    return a;
}

wb_attribute wb_object::attribute(const char *bname, const char *aname)
{
    check();

    pwr_tStatus sts = LDH__SUCCESS;
    wb_attribute a(sts, m_orep, bname, aname);

    return a;
}

size_t wb_object::rbSize()
{
  check();

  wb_cdef c = wb_cdef( *m_orep);
  wb_bdef b = c.bdef( pwr_eBix_rt);
  if ( !b)
    return 0;

  return b.size();
}

size_t wb_object::dbSize()
{
  wb_cdef c = wb_cdef( *m_orep);
  wb_bdef b = c.bdef( pwr_eBix_dev);
  if ( !b)
    return 0;

  return b.size();
}


wb_bdef wb_object::bdef(const char* bname)
{
  check();
  
  wb_cdef cdef(*m_orep);
  return cdef.bdef(bname);
}

wb_bdef wb_object::bdef(pwr_eBix bix)
{
  check();
  
  wb_cdef cdef(*m_orep);
  return cdef.bdef(bix);
}

/*      Object SigChanCon $ObjXRef 2

        Body SysBody
          Attr Identity = "Ai"
          Attr Source = "Ai"
          Attr Target = "ChanAi"
          Attr SourceAttribute = "SigChanCon"
          Attr TargetAttribute = "SigChanCon"
        EndBody
      EndObject
*/
pwr_tStatus wb_object::checkXref(const char *name)
{
    //pwr_sObjXRef pobjXDef;
    //wb_attribute a = attribute(name);
    
    //pwr_tOid *oidp = a.valp();
    
    //pwr_sObjXRef *oxrp = a.valp();

    return LDH__SUCCESS;
}

