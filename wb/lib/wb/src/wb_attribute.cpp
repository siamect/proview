#include "wb_attribute.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_attrname.h"
#include "pwr.h"


wb_attribute::wb_attribute() : wb_status(LDH__NOSUCHATTR), m_orep(0), m_adrep(0), 
			       m_size(0), m_offset(0), m_tid(0), m_elements(0), 
			       m_type(pwr_eType_), m_flags(0)
{
}

wb_attribute::wb_attribute(const wb_attribute& x) : 
  wb_status(x.m_sts),m_orep(x.m_orep), m_adrep( x.m_adrep), m_size(x.m_size), 
  m_offset(x.m_offset), m_tid(x.m_tid), m_elements(x.m_elements), m_type(x.m_type), 
  m_flags(x.m_flags)
{
  if ( m_orep)
    m_orep->ref();
  if ( m_adrep)
    m_adrep->ref();
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep * const orep) : 
  wb_status(sts), m_orep(orep), m_adrep(0), m_size(0), m_offset(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    m_orep->ref();

    // m_size == get rtbody size... Fix
  }
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep * const orep, wb_adrep * const adrep) : 
  wb_status(sts), m_orep(orep), m_adrep(adrep), m_size(0), m_offset(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    m_orep->ref();

    if ( m_adrep) {
      m_adrep->ref();
      m_size = m_adrep->size();
      m_offset = m_adrep->offset();
      m_type = m_adrep->type();
      m_tid = m_adrep->tid();
      m_flags = m_adrep->flags();
    }
    else {
      // m_size == get rtbody size... Fix
    }
  }
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep * const orep, char *aname, char *bname) :
  wb_status(sts), m_orep(orep)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    wb_attrname n = wb_attrname(aname);  
    wb_bdrep *bd = 0;
    wb_cdrep *cd = m_orep->vrep()->merep()->cdrep( &m_sts, m_orep->cid());
    if ( oddSts()) {
      if ( bname) {
        bd = cd->bdrep( &m_sts, bname);
	if ( oddSts())
	  m_adrep = bd->adrep( &m_sts, n.attribute(0));
      }
      else {
        m_adrep = cd->adrep( &m_sts, n.attribute(0));
      }
      if ( oddSts()) {
        m_adrep->ref();

        if ( !m_orep->vrep()->merep()->getAttrInfoRec( &n, m_adrep->bix(),
						       m_orep->cid(), (int *) &m_size,
						       &m_offset, &m_tid, &m_elements, 
						       &m_type,
						       &m_flags, 0)) {
          m_adrep->unref();
          m_sts = LDH__NOSUCHATTR;
        }
      }
      delete cd;
      if ( bd) delete bd;
    }
    if ( oddSts())
      m_orep->ref();
  }
}

wb_attribute::~wb_attribute()
{
  if ( m_orep)
    m_orep->unref();
  if ( m_adrep)
    m_adrep->unref();
}

wb_attribute& wb_attribute::operator=(const wb_attribute& x)
{
  if ( x.m_orep)
    x.m_orep->ref();
  if ( m_orep)
    m_orep->unref();
  if ( x.m_adrep)
    x.m_adrep->ref();
  if ( m_adrep)
    m_adrep->unref();
  m_orep = x.m_orep;
  m_adrep = x.m_adrep;
  m_sts = x.m_sts;
  m_size = x.m_size;
  m_offset = x.m_offset;
  m_tid = x.m_tid;
  m_type = x.m_type;
  m_flags = x.m_flags;
  return *this;
}

void wb_attribute::check()
{
  if ( evenSts())
    throw wb_error( m_sts);
}

//
// Return object identifier of attribute.
//
pwr_sAttrRef wb_attribute::aref()
{
    pwr_sAttrRef aref;
    
    return aref;
}

pwr_sAttrRef *wb_attribute::aref(pwr_sAttrRef *arp)
{
    return arp;
}

size_t wb_attribute::size()
{
  check();
  return m_size;
}

int wb_attribute::offset()
{
  check();
  return m_offset;
}

pwr_eType wb_attribute::type()
{
  check();
  return m_type;
}

int wb_attribute::nElement()
{
  check();
  return m_elements;
}

int wb_attribute::index()
{
  if ( m_adrep)
    return m_adrep->index();
  return 0;
}

int wb_attribute::flags()
{
  check();
  return m_flags;
}

pwr_tAix wb_attribute::aix()
{
  return 0; // Fix
}

pwr_tCid wb_attribute::cid()
{
  check();
  return m_orep->cid();
}

pwr_tAix wb_attribute::bix()
{
  return 0;  // Fix
}

pwr_tOid wb_attribute::boid()
{
  pwr_tOid oid;
  return oid;  // Fix
}

bool wb_attribute::checkXref()
{
  return true; // Fix
}

pwr_sAttrXRef *wb_attribute::xref()
{
  return (pwr_sAttrXRef*)0; // Fix
}

pwr_sObjXRef *wb_attribute::oxref()
{
  return (pwr_sObjXRef*)0; // Fix
}

void *wb_attribute::value( void *p)
{
  pwr_tStatus sts;
  check();

  if ( m_adrep == 0) {
    return m_orep->vrep()->readBody( &sts, m_orep, cdh_eBix_rt, p);
  }
  return m_orep->vrep()->readAttribute( &sts, m_orep, (cdh_eBix) m_adrep->bix(), m_offset,
				 m_size, p);
}

void *wb_attribute::value(void *vp, size_t size, pwr_tStatus *sts)
{
    return 0;
}

    
string wb_attribute::toString()
{
    string a;
    return a;
}

pwr_tStatus wb_attribute::fromString(string)
{
    
    pwr_tStatus sts;
    return sts;
}

pwr_tStatus wb_attribute::fromString(char *)
{
    pwr_tStatus sts;
    return sts;
}
   
wb_attribute wb_attribute::next()
{
    wb_attribute a;
    return a;
}

wb_attribute wb_attribute::prev()
{
    wb_attribute a;
    return a;
}   

wb_name wb_attribute::name()
{
    wb_name n;
    return n;
}

wb_name wb_attribute::name(ldh_eName type)
{
    wb_name n;
    return n;
}

void wb_attribute::name(const char *name)
{
}

void wb_attribute::name(wb_name *name)
{
}

