#include "wb_volume.h"
#include "wb_merep.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"

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

wb_object wb_volume::object(const char *name) const
{
  pwr_tStatus sts;
  wb_orep *orep;
  wb_object o;
    
  wb_name n = wb_name( name);
  if ( !n.hasVolume() || n.volumeIsEqual( m_vrep->name()))
    // This volume
    orep = m_vrep->object( &sts, n);
  else
    // Other volume
    orep = m_vrep->erep()->object(&sts, n);

  o = wb_object(sts, orep);
    
  return o;
}

wb_adef wb_volume::adef( pwr_tCid cid, const char *bname, const char *aname)
{
  pwr_tStatus sts;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, cid);
  if ( EVEN(sts)) return 0;

  wb_cdef cdef = wb_cdef(cdrep);
  wb_bdef bdef = cdef.bdef( bname);
  return bdef.adef( aname);
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

wb_attribute wb_volume::attribute(pwr_tOid oid, const char *bname, const char *aname) const
{
  pwr_tStatus sts;
  wb_orep *orep;
    
  if (oid.vid == m_vrep->vid())
    // This volume
    orep = m_vrep->object( &sts, oid);
  else
    // Other volume
    orep = m_vrep->erep()->object(&sts, oid);

    wb_attribute a(sts, orep, bname, aname);
    
  return a;
}

wb_attribute wb_volume::attribute(pwr_tOid oid, const char *bname) const
{
  pwr_tStatus sts;
  wb_orep *orep;
    
  if (oid.vid == m_vrep->vid())
    // This volume
    orep = m_vrep->object( &sts, oid);
  else
    // Other volume
    orep = m_vrep->erep()->object(&sts, oid);

    wb_attribute a(sts, orep, bname);
    
  return a;
}


wb_attribute wb_volume::attribute(const pwr_sAttrRef* arp) const
{
  pwr_tStatus sts;
  int idx;
  wb_orep* orep = 0;
  wb_cdrep* cdrep = 0;
  wb_bdrep* bdrep = 0;
  wb_adrep* adrep = 0;
  wb_adrep* old;
  
  if (arp->Objid.vid == m_vrep->vid())
    // This volume
    orep = m_vrep->object( &sts, arp->Objid);
  else
    // Other volume
    orep = m_vrep->erep()->object(&sts, arp->Objid);

  if (EVEN(sts))
    return wb_attribute();

  cdrep = new wb_cdrep(*orep);
  if (EVEN(cdrep->sts()))
    goto error;
      
  bdrep = cdrep->bdrep(&sts, pwr_eBix_rt);
  if (bdrep == 0)
    goto error;

  // Check if we shall reference the whole object
  if (arp->Size == 0 || (arp->Offset == 0 && arp->Size == bdrep->size())) {
    wb_attribute a(sts, orep);
    delete bdrep;
    delete cdrep;
    return a;
  }
  
      
  // We need to find a matching attribute
  adrep = bdrep->adrep(&sts);
  while (ODD(sts)) {
    if (arp->Offset < (adrep->offset() + adrep->size())){
      break;
    }
    old = adrep;
    adrep = adrep->next(&sts);
    delete old;
  }


  // We have found a matching attribute
  if (ODD(sts)) {  
    if (arp->Size == 0 || arp->Size > (adrep->size() / adrep->nElement())) {
      // Map whole atribute
      idx = (adrep->nElement() > 1) ? -1 : 0;
    } else if (adrep->nElement() > 1) {
      idx = (arp->Offset - adrep->offset()) / (adrep->size() / adrep->nElement());
    }

    delete cdrep;
    delete bdrep;

    wb_attribute a(LDH__SUCCESS, orep, adrep, idx);
    return a;
  }
  

 error:
  delete orep;
  delete cdrep;
  delete bdrep;
  return wb_attribute();
}

pwr_tStatus wb_volume::syntaxCheck( int *errorcount, int *warningcount)
{
  pwr_tStatus sts;

  wb_orep *orep = m_vrep->object(&sts);
  if ( ODD(sts)) {
    orep->ref();
    sts = syntaxCheckObject( orep, errorcount, warningcount);
    orep->unref();
    return sts;
  }
  return LDH__SUCCESS;
}

pwr_tStatus wb_volume::syntaxCheckObject( wb_orep *orep, int *errorcount, int *warningcount)
{
  pwr_tStatus sts;
  wb_orep *first, *after;
  
  sts = triggSyntaxCheck( orep, errorcount, warningcount);
  if ( EVEN(sts)) return sts;

  switch ( orep->cid()) {
  case pwr_eClass_LibHier:
    break;
  default:
    first = orep->first( &sts);
    if ( ODD(sts)) {
      first->ref();
      sts = syntaxCheckObject( first, errorcount, warningcount);
      first->unref();
      if ( EVEN(sts)) return sts;
    }
    after = orep->after( &sts);
    if ( ODD(sts)) {
      after->ref();
      sts = syntaxCheckObject( after, errorcount, warningcount);
      after->unref();
      if (  EVEN(sts)) return sts;
    }
  }
  return LDH__SUCCESS;
}

pwr_tStatus wb_volume::triggSyntaxCheck( wb_orep *orep, int *errorcount, int *warningcount)
{
  pwr_tStatus sts;
  char methodName[] = "SyntaxCheck";
  wb_tMethod method;
  
  // wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, orep->cid());
  // if ( EVEN(sts)) return sts;

  // cdrep->dbCallBack( &sts, ldh_eDbCallBack_SyntaxCheck, &methodName, 0);
  // delete cdrep;
  // if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodSyntaxCheck) (method))( (ldh_tSesContext)this, orep->oid(), errorcount, warningcount);
  return sts;
}

pwr_tStatus wb_volume::triggAnteAdopt( wb_orep *father, pwr_tCid cid)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteAdopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodAnteAdopt) (method))( (ldh_tSesContext)this, father->oid(), cid);
  return sts;
}

pwr_tStatus wb_volume::triggAnteCreate( wb_orep *father, pwr_tCid cid)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, cid);
  if ( EVEN(sts))
    return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts))
    return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodAnteCreate) (method))( (ldh_tSesContext)this, father->oid(), cid);
  return sts;
}

pwr_tStatus wb_volume::triggAnteMove( wb_orep *orep, wb_orep *father)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, orep->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  if ( father)
    sts = ((wb_tMethodAnteMove) (method))( (ldh_tSesContext)this, orep->oid(), father->oid(),
					   father->cid());
  else
    sts = ((wb_tMethodAnteMove) (method))( (ldh_tSesContext)this, orep->oid(), pwr_cNObjid,
					   pwr_cNClassId);
  return sts;
}

pwr_tStatus wb_volume::triggAnteUnadopt( wb_orep *father, wb_orep *orep)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodAnteUnadopt) (method))( (ldh_tSesContext)this, father->oid(), orep->oid(),
					   orep->cid());
  return sts;
}

pwr_tStatus wb_volume::triggPostAdopt( wb_orep *father, wb_orep *orep)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostAdopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodPostAdopt) (method))( (ldh_tSesContext)this, father->oid(), orep->oid(),
					   orep->cid());
  return sts;
}

pwr_tStatus wb_volume::triggPostCreate( wb_orep *orep)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, orep->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  wb_orep *father = orep->parent( &sts);
  if ( ODD(sts)) {
    father->ref();
    sts = ((wb_tMethodPostCreate) (method))( (ldh_tSesContext)this, orep->oid(), father->oid(),
					   father->cid());
    father->unref();
  }
  else
    sts = ((wb_tMethodPostCreate) (method))( (ldh_tSesContext)this, orep->oid(), pwr_cNObjid,
					   pwr_cNClassId);
  return sts;
}

pwr_tStatus wb_volume::triggPostMove( wb_orep *orep)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, orep->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostMove, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  wb_orep *father = orep->parent( &sts);
  if ( ODD(sts)) {
    father->ref();
    sts = ((wb_tMethodPostMove) (method))( (ldh_tSesContext)this, orep->oid(), father->oid(),
					   father->cid());
    father->unref();
  }
  else
    sts = ((wb_tMethodPostMove) (method))( (ldh_tSesContext)this, orep->oid(), pwr_cNObjid,
					   pwr_cNClassId);
  return sts;
}

pwr_tStatus wb_volume::triggPostUnadopt( wb_orep *father, wb_orep *orep)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father->cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostUnadopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodPostUnadopt) (method))( (ldh_tSesContext)this, father->oid(), orep->oid(),
					   orep->cid());
  return sts;
}






