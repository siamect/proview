#include "wb_attribute.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"
#include "wb_bdef.h"
#include "wb_merep.h"
#include "wb_attrname.h"
#include "pwr.h"


wb_attribute::wb_attribute() : 
  wb_status(LDH__NOSUCHATTR), m_orep(0), m_adrep(0), 
  m_size(0), m_offset(0), m_idx(0), m_tid(0), m_elements(0),
  m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
{
}

wb_attribute::wb_attribute(const wb_attribute& x) : 
  wb_status(x.m_sts),m_orep(x.m_orep), m_adrep( x.m_adrep), m_size(x.m_size), 
  m_offset(x.m_offset) , m_idx(x.m_idx), m_tid(x.m_tid), m_elements(x.m_elements), m_type(x.m_type), 
  m_flags(x.m_flags), m_bix(x.m_bix)
{
  if ( m_orep)
    m_orep->ref();
  if ( m_adrep)
    m_adrep->ref();
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep * orep) : 
  wb_status(sts), m_orep(orep), m_adrep(0), m_size(0), m_offset(0), m_idx(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    m_orep->ref();

    // m_size == get rtbody size... Fix
  }
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep* orep, wb_adrep* adrep, int idx) : 
  wb_status(sts), m_orep(orep), m_adrep(adrep), m_size(0), m_offset(0), m_idx(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    m_orep->ref();

    if ( m_adrep) {
      m_adrep->ref();
      m_size = m_adrep->size();
      m_offset = m_adrep->offset();
      m_tid = m_adrep->tid();
      m_elements = m_adrep->nElement();
      m_type = m_adrep->type();
      m_flags = m_adrep->flags();

      if (m_flags & PWR_MASK_ARRAY) {
        if (idx >= m_elements)
          throw wb_error_str("wb_attribute() subscript out of range");
        m_idx = idx;
        if (idx != -1) { // element
          m_size = m_adrep->size() / m_adrep->nElement();
          m_offset = m_adrep->offset() + m_idx * m_size;
          m_elements = 1;
        }
      } else
        m_idx = 0;

    }
    else {
      // m_size == get rtbody size... Fix
      wb_cdef cdef(*orep);
      if (EVEN(cdef.sts())) {
        char msg[256];
        m_sts = cdef.sts();
        sprintf(msg, "Can't get cdef for orep: %s", orep->name());  
        throw wb_error_str(m_sts, msg);
      }
      wb_bdef bdef(((wb_cdrep *)cdef)->bdrep(&m_sts, pwr_eBix_rt));
      if (EVEN(bdef.sts())) {
        char msg[256];
        m_sts = bdef.sts();
        sprintf(msg, "Can't get bdef for orep: %s", orep->name());  
        throw wb_error_str(m_sts, msg);
      }

      m_size = bdef.size();
      
    }
  }
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep* orep, const char* bname) :
  wb_status(sts), m_orep(orep), m_adrep(0), m_size(0), m_offset(0), m_idx(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
{
  if ( orep == 0)
    m_sts = LDH__NOSUCHATTR;
  else {
    wb_cdrep *cd = m_orep->vrep()->merep()->cdrep( &m_sts, m_orep->cid());
    if ( oddSts()) {
      wb_bdrep *bd = cd->bdrep( &m_sts, bname);
      if ( oddSts()) {
        m_size = bd->size();
        delete bd;
      }
      delete cd;
    }
    if ( oddSts())
      m_orep->ref();
    else
      m_orep = 0;
  }
}

wb_attribute::wb_attribute(pwr_tStatus sts, wb_orep* orep, const char* bname, const char* aname) :
  wb_status(sts), m_orep(orep), m_adrep(0), m_size(0), m_offset(0), m_idx(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
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
                                                       m_orep->cid(), &m_size,
                                                       &m_offset, &m_tid, &m_elements, 
                                                       &m_type,
                                                       &m_flags, 0)) {
          m_adrep->unref();
          m_adrep = 0;
          m_sts = LDH__NOSUCHATTR;
        }
      } else
        m_adrep = 0;
      
      delete cd;
      if ( bd) 
        delete bd;
    }
    if ( oddSts())
      m_orep->ref();
    else
      m_orep = 0;
  }
}

wb_attribute::wb_attribute(const wb_attribute& pa, int idx, const char* aname) :
  wb_status(LDH__NOSUCHATTR), m_orep(0), m_adrep(0), m_size(0), m_offset(0), m_tid(0),
  m_elements(0), m_type(pwr_eType_), m_flags(0), m_bix(pwr_eBix__)
{
  pwr_tCid cid;
  wb_attrname n;
  
  if (!pa.isClass() || pa.m_orep == 0)
    return;

  if (idx < 0 || idx >= pa.m_elements)
    throw wb_error_str("Invalid subscript");
    

  if (!(pa.m_flags & PWR_MASK_BUFFER))
    throw wb_error_str("Only $Buffer is supported as sub class");

  cid = pa.subClass();
  m_orep = pa.m_orep;

  wb_cdrep *cd = m_orep->vrep()->merep()->cdrep(&m_sts, cid);
  if (oddSts()) {
    wb_bdrep* bd = cd->bdrep(&m_sts, pwr_eBix_sys);
    if (oddSts()) {
      if (aname != 0){
        n = aname;
        m_adrep = bd->adrep(&m_sts, n.attribute(0));
      } else {
        m_adrep = bd->adrep(&m_sts);
        if (oddSts())
          n = m_adrep->name();
      }

      if (oddSts()) {
        m_adrep->ref();

        if (!m_orep->vrep()->merep()->getAttrInfoRec( &n, m_adrep->bix(),
                                                      cid, &m_size,
                                                      &m_offset, &m_tid, &m_elements,
                                                      &m_type,
                                                      &m_flags, 0)) {
          m_adrep->unref();
          m_adrep = 0;
          m_sts = LDH__NOSUCHATTR;
        } else {
          m_offset = pa.m_offset + idx * pa.m_size/pa.m_elements;
        }
      } else
        m_adrep = 0;
    }
    if (bd) 
      delete bd;
    delete cd;
  }


  if (oddSts()) {    
    m_orep->ref();
    m_flags |= PWR_MASK_SUBCLASS;

    if (pa.m_flags & PWR_MASK_SUBCLASS)
      m_bix = pa.m_bix;
    else
      m_bix = pa.m_adrep->bix();
  }
  else {
    m_adrep = 0;
    m_orep = 0;
  }
}


wb_attribute::~wb_attribute()
{
  if ( m_orep) {
    m_orep->unref();
    m_orep = 0;
  }
  
  if ( m_adrep) {
    m_adrep->unref();
    m_adrep = 0;
  }
  
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
  m_idx = x.m_idx;
  m_tid = x.m_tid;
  m_elements = x.m_elements;
  m_type = x.m_type;
  m_flags = x.m_flags;
  m_bix = x.m_bix;
  
  return *this;
}

void wb_attribute::check() const
{
  if ( evenSts())
    throw wb_error( m_sts);
}

//
// Return object identifier of attribute.
//
pwr_sAttrRef wb_attribute::aref() const
{
  pwr_sAttrRef aref;
    
  return aref;
}

pwr_sAttrRef *wb_attribute::aref(pwr_sAttrRef *arp) const
{
  return arp;
}

size_t wb_attribute::size() const
{
  check();
  return m_size;
}

size_t wb_attribute::offset() const
{
  check();
  return m_offset;
}

pwr_eType wb_attribute::type() const
{
  check();
  return m_type;
}

int wb_attribute::nElement() const
{
  check();
  return m_elements;
}

int wb_attribute::index() const
{
  if ( m_adrep)
    return m_adrep->index();
  return 0;
}

int wb_attribute::flags() const
{
  check();
  return m_flags;
}

pwr_tAix wb_attribute::aix() const
{
  throw wb_error_str("wb_attribute::aix() NYI");
  return 0; // Fix
}

pwr_tCid wb_attribute::cid() const
{
  check();
  return m_orep->cid();
}

pwr_eBix wb_attribute::bix() const
{
  throw wb_error_str("wb_attribute::bix() NYI");

  return pwr_eBix__;  // Fix
}

pwr_tOid wb_attribute::boid() const
{
  throw wb_error_str("wb_attribute::boid() NYI");

  pwr_tOid oid;
  return oid;  // Fix
}

pwr_tCid wb_attribute::subClass() const
{
  if ( m_adrep)
    return m_adrep->subClass();
  return 0;
}

bool wb_attribute::checkXref() const
{
  throw wb_error_str("wb_attribute::checkXref() NYI");
  return true; // Fix
}

pwr_sAttrXRef *wb_attribute::xref() const
{
  throw wb_error_str("wb_attribute::xref() NYI");
  return (pwr_sAttrXRef*)0; // Fix
}

pwr_sObjXRef *wb_attribute::oxref() const
{
  throw wb_error_str("wb_attribute::oxref() NYI");
  return (pwr_sObjXRef*)0; // Fix
}

void *wb_attribute::value( void *p) const
{
  pwr_eBix bix;
  pwr_tStatus sts;
  check();

  if ( m_adrep == 0) {
    return m_orep->vrep()->readBody( &sts, m_orep, pwr_eBix_rt, p);
  }
  
  if (m_flags & PWR_MASK_SUBCLASS)
    bix = m_bix;
  else
    bix = m_adrep->bix();

  return m_orep->vrep()->readAttribute( &sts, m_orep, bix, m_offset, m_size, p);
}

void *wb_attribute::value(void *vp, size_t size, pwr_tStatus *sts) const
{
  return 0;
}

    
string wb_attribute::toString() const
{
  throw wb_error_str("wb_attribute::toString() NYI");
  string a;
  return a;
}

pwr_tStatus wb_attribute::fromString(string) const
{
  throw wb_error_str("wb_attribute::fromString() NYI");    
  pwr_tStatus sts;
  return sts;
}

pwr_tStatus wb_attribute::fromString(char *) const
{
  throw wb_error_str("wb_attribute::fromString() NYI");    
  pwr_tStatus sts;
  return sts;
}
   
wb_attribute wb_attribute::after() const
{
  pwr_tStatus sts;

  check();
  if (m_adrep == 0)
    return wb_attribute();

  wb_adrep* adrep = m_adrep->next(&sts);
  if (evenSts())
    return wb_attribute();
  
  wb_attribute a(LDH__SUCCESS, m_orep, adrep);
  
  // Fix for sub classes
  if (m_flags & PWR_MASK_SUBCLASS) {
    a.m_flags |= PWR_MASK_SUBCLASS;
    a.m_bix = m_bix;
    if ((m_flags & PWR_MASK_ARRAY) && m_idx != -1) // array element
      a.m_offset = (m_offset - m_size * m_idx) + m_adrep->size();
    else
      a.m_offset = m_offset + m_size;
  }

  return a;
}

wb_attribute wb_attribute::before() const
{
  pwr_tStatus sts;

  check();
  if (m_adrep == 0)
    return wb_attribute();

  wb_adrep* adrep = m_adrep->prev(&sts);
  if (evenSts())
    return wb_attribute();
  
  wb_attribute a(LDH__SUCCESS, m_orep, adrep);
  
  // Fix for sub classes
  if (m_flags & PWR_MASK_SUBCLASS) {
    a.m_flags |= PWR_MASK_SUBCLASS;
    a.m_bix = m_bix;
    if ((m_flags & PWR_MASK_ARRAY) && m_idx != -1) // array element
      a.m_offset = (m_offset - m_size * m_idx) - adrep->size();
    else
      a.m_offset = m_offset - adrep->size();
  }

  return a;
}   


wb_attribute wb_attribute::first(int idx) const
{
  if (!isClass())
    return wb_attribute();    

  return wb_attribute(*this, idx, NULL);
    
}


wb_attribute wb_attribute::child(int idx, const char* name) const
{
  if (!isClass())
    return wb_attribute();    

  return wb_attribute(*this, idx, name);
    
}


const char *wb_attribute::name() const
{
  check();

  return m_orep->name();
}

// Fix, no no subclass !!!
wb_name wb_attribute::longName()  const
{
  check();

  if ( !m_adrep)
    return m_orep->longName();

    char str[512];
    int len;
    len = sprintf( str, "%s.%s", m_orep->longName().c_str(), m_adrep->name());
    if (m_flags & PWR_MASK_ARRAY && m_idx != -1)
      sprintf(&str[len], "[%d]", m_idx);

    wb_name n( str);
    return n;
}

void wb_attribute::name(const char *name)
{
}

void wb_attribute::name(wb_name *name)
{
}

