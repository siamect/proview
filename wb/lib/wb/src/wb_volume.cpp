#include "wb_volume.h"
#include "wb_merep.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_vrepmem.h"

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
  else if (oid.vid < 65536)
    orep = m_vrep->merep()->object(&sts, oid);
  else
    // Other volume
    orep = m_vrep->erep()->object(&sts, oid);

  o = wb_object(sts, orep);
    
  return o;
}

wb_object wb_volume::object(pwr_tCid cid) const
{
  pwr_tStatus sts;
  wb_orep *orep;
  wb_object o;
    
  orep = m_vrep->object( &sts, cid);
  o = wb_object(sts, orep);
    
  return o;
}

wb_object wb_volume::object(const char *name) const
{
  pwr_tStatus sts;
  wb_orep *orep;
  wb_object o;
    
  wb_name n = wb_name( name);
  if ( n.evenSts()) 
    return wb_object( n.sts(), 0);

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
  if ( EVEN(sts)) return wb_adef();

  wb_cdef cdef = wb_cdef(cdrep);
  wb_bdef bdef = cdef.bdef( bname);
  if ( !bdef)
    return wb_adef();
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

wb_attribute wb_volume::attribute( wb_name aname)
{
  pwr_tStatus sts;

  if ( !aname)
    return wb_attribute();

  wb_object o = object( aname.name());
  if ( !o)
    return wb_attribute();

  if ( !aname.hasAttribute())
    return wb_attribute( LDH__SUCCESS, (wb_orep *)o);

  wb_cdef cd = cdef(o);
  if ( !cd)
    return wb_attribute();

  wb_adrep *adrep = ((wb_cdrep *)cd)->adrep( &sts, aname.attributesAllTrue());

  if ( ODD(sts)) {
    bool shadowed = false;
    if ( aname.hasSuper()) {
      // Check if shadowed
      wb_attrname an(aname.attributesAllTrue());
      wb_adrep *ad = ((wb_cdrep *)cd)->adrep( &sts, an.name(cdh_mName_attribute));
      if ( ODD(sts) && ad->offset() != adrep->offset()) {
	shadowed = true;
	delete ad;
      }
    }

    wb_attribute a;
    if ( aname.hasAttrIndex(aname.attributes() - 1))
      a = wb_attribute( sts, (wb_orep *)o, adrep, aname.attrIndex(aname.attributes() - 1));
    else
      a = wb_attribute( sts, (wb_orep *)o, adrep);
    a.setShadowed( shadowed);
    return a;
  }
#if 0
  // This only work on one level attributes... TODO
  wb_adrep *adrep = ((wb_cdrep *)cd)->adrep( &sts, aname.attributesAllTrue());
  if ( ODD(sts)) {
    if ( aname.hasAttrIndex())
      return wb_attribute( sts, (wb_orep *)o, adrep, aname.attrIndex());
    else
      return wb_attribute( sts, (wb_orep *)o, adrep);
  }
#endif
  return wb_attribute();
}

wb_attribute wb_volume::attribute( wb_object o, wb_attrname aname)
{
  pwr_tStatus sts;

  if ( !o)
    return wb_attribute();

  wb_cdef cd = cdef(o);
  if ( !cd)
    return wb_attribute();

  wb_adrep *adrep = ((wb_cdrep *)cd)->adrep( &sts, aname.attributesAllTrue());
  if ( ODD(sts))
    return wb_attribute( sts, (wb_orep *)o, adrep);
  return wb_attribute();
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
  bool subClass = false;
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
  orep->ref();

  cdrep = new wb_cdrep(*orep);
  if (EVEN(cdrep->sts())) { orep->unref(); return wb_attribute();}
      
  bdrep = cdrep->bdrep(&sts, pwr_eBix_rt);
  delete cdrep;
  if ( EVEN(sts)) { orep->unref(); return wb_attribute();}

  // Check if we shall reference the whole object
  if (arp->Size == 0 || (arp->Offset == 0 && arp->Size == bdrep->size())) {
    wb_attribute a(sts, orep);
    orep->unref();
    delete bdrep;
    return a;
  }
  
      
  // We need to find a matching attribute
  adrep = bdrep->adrep(&sts);
  while ( ODD(sts)) {
    bool newBody = false;
    while (ODD(sts)) {
      if (arp->Offset < (adrep->offset() + adrep->size())){
	if ( arp->Size == 0 || adrep->size() == arp->Size) {
	  // Attribute found
	  idx = (adrep->nElement() > 1) ? -1 : 0;
	  wb_attribute a( LDH__SUCCESS, orep, adrep, idx);
	  if ( arp->Flags.b.Shadowed)
	    a.setShadowed(true);
	  delete bdrep;
	  orep->unref();
	  return a;
	}
	else if ( adrep->flags() & PWR_MASK_ARRAY &&
		  adrep->size()/adrep->nElement() == arp->Size) {  
	  // Attribute element found
	  idx = (arp->Offset - adrep->offset()) / (adrep->size() / adrep->nElement());
	  wb_attribute a( LDH__SUCCESS, orep, adrep, idx);
	  delete bdrep;
	  orep->unref();
	  return a;
        }
	else if ( adrep->flags() & PWR_MASK_CLASS) {
	  // Continue to examine object attribute
	  cdrep = m_vrep->merep()->cdrep(&sts, adrep->subClass());
	  if ( EVEN(sts)) { orep->unref(); delete bdrep; return wb_attribute();}

	  if ( bdrep) 
	    delete bdrep;
	  bdrep = cdrep->bdrep(&sts, pwr_eBix_rt);
	  if ( EVEN(sts)) { orep->unref(); return wb_attribute();}

	  subClass = true;
	  delete cdrep;
	  old = adrep;
	  adrep = bdrep->adrep(&sts);
	  if ( EVEN(sts)) { orep->unref(); delete old; delete bdrep; return wb_attribute();}
	  if ( old->flags() & PWR_MASK_ARRAY) {
	    idx = (arp->Offset - old->offset()) / (old->size() / old->nElement());
	    adrep->add( old, idx);
	  }
	  else
	    adrep->add( old);
	  delete old;
	  newBody = true;
	  break;
	}
	else {
	  // Missmatch
	  delete bdrep;
	  delete adrep;
	  return wb_attribute();
	}
      }
      if ( newBody)
	break;
      old = adrep;
      adrep = adrep->next(&sts);
      delete old;
    }
  }

  delete bdrep;
  orep->unref();
  return wb_attribute();
}

bool wb_volume::exportTree( wb_volume &import, pwr_tOid oid)
{
  wb_vrepmem *vrepmem = (wb_vrepmem *)(wb_vrep *)import;
  return m_vrep->exportTree( *vrepmem, oid);
}

pwr_tStatus wb_volume::syntaxCheck( int *errorcount, int *warningcount)
{
  pwr_tStatus sts;

  wb_object o = object();
  if ( o) {
    sts = syntaxCheckObject( o, errorcount, warningcount);
    return sts;
  }
  return LDH__SUCCESS;
}

pwr_tStatus wb_volume::syntaxCheckObject( wb_object& o, int *errorcount, int *warningcount)
{
  pwr_tStatus sts;
  wb_object first, after;
  
  sts = triggSyntaxCheck( o, errorcount, warningcount);
  if ( EVEN(sts)) return sts;

  switch ( o.cid()) {
  case pwr_eClass_LibHier:
    break;
  default:
    first = o.first();
    if ( first) {
      sts = syntaxCheckObject( first, errorcount, warningcount);
      if ( EVEN(sts)) return sts;
    }
    after = o.after();
    if ( after) {
      sts = syntaxCheckObject( after, errorcount, warningcount);
      if (  EVEN(sts)) return sts;
    }
  }
  return LDH__SUCCESS;
}

pwr_tStatus wb_volume::triggSyntaxCheck( wb_object& o, int *errorcount, int *warningcount)
{
  pwr_tStatus sts;
  char methodName[] = "SyntaxCheck";
  wb_tMethod method;
  
  // wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, o.cid());
  // if ( EVEN(sts)) return sts;

  // cdrep->dbCallBack( &sts, ldh_eDbCallBack_SyntaxCheck, &methodName, 0);
  // delete cdrep;
  // if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodSyntaxCheck) (method))( (ldh_tSesContext)this, o.oid(), errorcount, warningcount);
  return sts;
}

pwr_tStatus wb_volume::triggAnteAdopt( wb_object& father, pwr_tCid cid)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  pwr_tOid foid;
  
  if ( !father)
    return LDH__SUCCESS;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteAdopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  if ( !father)
    foid = pwr_cNObjid;
  else
    foid = father.oid();

  sts = ((wb_tMethodAnteAdopt) (method))( (ldh_tSesContext)this, foid, cid);
  return sts;
}

pwr_tStatus wb_volume::triggAnteCreate( wb_object& father, pwr_tCid cid)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  pwr_tOid foid;
  

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, cid);
  if ( EVEN(sts))
    return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts))
    return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  if ( !father)
    foid = pwr_cNObjid;
  else
    foid = father.oid();

  sts = ((wb_tMethodAnteCreate) (method))( (ldh_tSesContext)this, foid, cid);
  return sts;
}

pwr_tStatus wb_volume::triggAnteMove( wb_object& o, wb_object& father, wb_object& old_father)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  pwr_tOid foid;
  pwr_tOid old_foid;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, o.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteMove, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  if ( father)
    foid = father.oid();
  else
    foid = pwr_cNObjid;

  if ( old_father)
    old_foid = old_father.oid();
  else
    old_foid = pwr_cNObjid;

  sts = ((wb_tMethodAnteMove) (method))( (ldh_tSesContext)this, o.oid(), foid,
					 old_foid);
  return sts;
}

pwr_tStatus wb_volume::triggAnteUnadopt( wb_object& father, wb_object& o)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  if ( !father)
    return LDH__SUCCESS;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_AnteAdopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodAnteUnadopt) (method))( (ldh_tSesContext)this, father.oid(), o.oid(),
					   o.cid());
  return sts;
}

pwr_tStatus wb_volume::triggPostAdopt( wb_object& father, wb_object& o)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  if ( !father)
    return LDH__SUCCESS;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostAdopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodPostAdopt) (method))( (ldh_tSesContext)this, father.oid(), o.oid(),
					   o.cid());
  return sts;
}

pwr_tStatus wb_volume::triggPostCreate( wb_object& o)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, o.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostCreate, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  wb_object father = o.parent();
  if ( father) {
    sts = ((wb_tMethodPostCreate) (method))( (ldh_tSesContext)this, o.oid(), father.oid(),
					   father.cid());
  }
  else
    sts = ((wb_tMethodPostCreate) (method))( (ldh_tSesContext)this, o.oid(), pwr_cNObjid,
					   pwr_cNClassId);
  return sts;
}

pwr_tStatus wb_volume::triggPostMove( wb_object& o)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, o.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostMove, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  wb_object father = o.parent();
  if ( father) {
    sts = ((wb_tMethodPostMove) (method))( (ldh_tSesContext)this, o.oid(), father.oid(),
					   father.cid());
  }
  else
    sts = ((wb_tMethodPostMove) (method))( (ldh_tSesContext)this, o.oid(), pwr_cNObjid,
					   pwr_cNClassId);
  return sts;
}

pwr_tStatus wb_volume::triggPostUnadopt( wb_object& father, wb_object& o)
{
  pwr_tStatus sts;
  char *methodName;
  wb_tMethod method;
  
  if ( !father)
    return LDH__SUCCESS;

  wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, father.cid());
  if ( EVEN(sts)) return sts;

  cdrep->dbCallBack( &sts, ldh_eDbCallBack_PostUnadopt, &methodName, 0);
  delete cdrep;
  if ( EVEN(sts)) return LDH__SUCCESS;

  m_vrep->erep()->method( &sts, methodName, &method);
  if ( EVEN(sts)) return LDH__SUCCESS;

  sts = ((wb_tMethodPostUnadopt) (method))( (ldh_tSesContext)this, father.oid(), o.oid(),
					   o.cid());
  return sts;
}

ldh_sRefInfo *wb_volume::refinfo( wb_object o, ldh_sRefInfo *rp)
{
  memset( rp, 0, sizeof(*rp));

  wb_cdef cd = cdef( o);
  wb_bdef bd = cd.bdef( pwr_eBix_rt);
  if (!bd) return rp;

  wb_attribute body = wb_attribute( bd.sts(), (wb_orep *)o, bd.name());
  char *bp = (char *)body.value(0);
  pwr_tOid oid;
  pwr_sAttrRef attrref;

  wb_adef ad = bd.adef();
  while ( ad) {
    switch ( ad.cid()) {
    case pwr_eClass_Input:
    case pwr_eClass_Output:
    case pwr_eClass_Intern:
    case pwr_eClass_Param:
      switch ( ad.type()) {
      case pwr_eType_Objid:
	for ( int i = 0; i < ad.nElement(); i++) {
	  rp->ObjRef.Total++;
	  oid = *(pwr_tOid *)(bp + ad.offset() + i * ad.size() / ad.nElement());
	  if ( cdh_ObjidIsNotNull( oid)) {
	    rp->ObjRef.Used++;
	    wb_object otst = object(oid);
	    if ( !otst)
	      rp->ObjRef.Errors++;
	  }
	}
	break;
      case pwr_eType_AttrRef:
	for ( int i = 0; i < ad.nElement(); i++) {
	  rp->ObjRef.Total++;
	  attrref = *(pwr_sAttrRef *)(bp + ad.offset() + i * ad.size() / ad.nElement());
	  if ( cdh_ObjidIsNotNull( attrref.Objid)) {
	    rp->ObjRef.Used++;
	    wb_object otst = object(attrref.Objid);
	    if ( !otst)
	      rp->ObjRef.Errors++;
	  }
	}
	break;
      default: ;
      }
      break;
    case pwr_eClass_AttrXRef:
      // TODO
      break;
    case pwr_eClass_ObjXRef:
      // TODO
      break;
    default: ;
    }
    ad = ad.next();
  }
  return rp;
}

void wb_volume::aref( pwr_tCid cid, pwr_sAttrRef *arp)
{
  tree_sTable *catt_tt = m_vrep->merep()->buildCatt( &m_sts);

  // Look in classlist
  wb_orep *o = m_vrep->object( &m_sts, cid);
  if ( oddSts()) {
    o->ref();
    *arp = cdh_ObjidToAref( o->oid());
    o->unref();
    return;
  }
  else {
  // Find attribute object
    pwr_tCid hostCid = 0;
    merep_sClassAttrKey key;
    merep_sClassAttr *item;

    key.subCid = cid;
    key.hostCid = 0;
    key.idx = 0;
    for ( item = (merep_sClassAttr*) tree_FindSuccessor( &m_sts, catt_tt, &key);
	  item && item->key.subCid == cid;
	  item = (merep_sClassAttr*) tree_FindSuccessor( &m_sts, catt_tt, &item->key)) {
      if ( hostCid && item->key.hostCid == hostCid)
	// Same class with other index
	continue;

      hostCid = item->key.hostCid;

      wb_orep *o = m_vrep->object( &m_sts, item->key.hostCid);
      if ( oddSts()) {
	o->ref();

	wb_cdrep *cd = m_vrep->merep()->cdrep( &m_sts, cid);
	if ( evenSts()) return;

	int bd_size;
	wb_bdrep *bd = cd->bdrep( &m_sts, pwr_eBix_rt);
	if ( oddSts()) {
	  bd_size = bd->size();
	  delete bd;
	}
	else
	  bd_size = 0;
	delete cd;

	*arp = pwr_cNAttrRef;
	arp->Objid = o->oid();
	arp->Flags.b.ObjectAttr = 1;
	arp->Offset = item->offset[0];
	arp->Size = bd_size;
	arp->Body = cid;
	o->unref();
	return;
      }      
    }
  }
}

void wb_volume::nextAref( pwr_tCid cid, pwr_sAttrRef *arp,
			  pwr_sAttrRef *oarp)
{
  tree_sTable *catt_tt = m_vrep->merep()->catt_tt();
  int bd_size;

  if ( !catt_tt) throw wb_error(LDH__CATT);

  wb_orep *op = m_vrep->object( &m_sts, arp->Objid);
  if ( evenSts()) return;

  // Get body size
  wb_cdrep *cd = m_vrep->merep()->cdrep( &m_sts, cid);
  if ( evenSts()) return;

  wb_bdrep *bd = cd->bdrep( &m_sts, pwr_eBix_rt);
  if ( oddSts()) {
    bd_size = bd->size();
    delete bd;
  }
  else
    bd_size = 0;
  delete cd;

  op->ref();
  if ( op->cid() == cid) {
    // Find next object in class list
    wb_orep *ol = m_vrep->next( &m_sts, op);
    if ( oddSts()) {
      *oarp = pwr_cNAttrRef;
      oarp->Objid = ol->oid();
      oarp->Flags.b.Object = 1;
      oarp->Size = bd_size;
      oarp->Body = cid;
      ol->unref();
      op->unref();
      return;
    } 
    else {
      // Find first attribute object
      merep_sClassAttrKey key;
      merep_sClassAttr *item;
      pwr_tCid hostCid = 0;

      key.subCid = cid;
      key.hostCid = 0;
      key.idx = 0;
      for ( item = (merep_sClassAttr*) tree_FindSuccessor( &m_sts, catt_tt, &key);
	    item && item->key.subCid == cid;
	    item = (merep_sClassAttr*) tree_FindSuccessor( &m_sts, catt_tt, &item->key)) {
	if ( cd && item->key.hostCid == hostCid)
	  // Same class with other index
	  continue;
	  
	hostCid = item->key.hostCid;

	  
	wb_orep *ol = m_vrep->object( &m_sts, item->key.hostCid);
	if ( oddSts()) {
	  ol->ref();
	  *oarp = pwr_cNAttrRef;
	  oarp->Objid = ol->oid();
	  oarp->Flags.b.ObjectAttr = 1;
	  oarp->Size = bd_size;
	  oarp->Offset = item->offset[0];
	  oarp->Body = cid;
	  ol->unref();
	  op->unref();
	  return;
	}
      }
      op->unref();
      m_sts = LDH__NONEXT;
      return;
    }
  }

  // Find next attribute object in current object
  merep_sClassAttrKey key;
  merep_sClassAttr *item;
  int first_offset = 0;

  key.subCid = cid;
  key.hostCid = op->cid();
  key.idx = 0;
  for ( item = (merep_sClassAttr*)tree_Find( &m_sts, catt_tt, &key);
	item && item->key.subCid == cid && item->key.hostCid == op->cid();
	item = (merep_sClassAttr*)tree_FindSuccessor( &m_sts, catt_tt, &item->key)) {
    // Find next offset
    for ( int i = 0; i < item->numOffset; i++) {
      if ( i == 0 && item->key.idx == 0)
	first_offset = item->offset[0];
      if ( item->offset[i] > arp->Offset) {
	*oarp = pwr_cNAttrRef;
	oarp->Objid = op->oid();
	oarp->Flags.b.ObjectAttr = 1;
	oarp->Offset = item->offset[i];
	oarp->Size = bd_size;
	oarp->Body = cid;
	op->unref();
	return;
      }
    }
  }

  // Find first attribute in next object
  wb_orep *ol = m_vrep->next( &m_sts, op);
  if ( oddSts()) {
    ol->ref();
    *oarp = pwr_cNAttrRef;
    oarp->Objid = ol->oid();
    oarp->Flags.b.ObjectAttr = 1;
    oarp->Offset = first_offset;
    oarp->Size = bd_size;
    oarp->Body = cid;
    ol->unref();
    op->unref();
    return;
  }

  // Find first offset in first object of next class
  key.subCid = cid;
  key.hostCid = op->cid();
  key.idx = 0;
  for ( item = (merep_sClassAttr*) tree_Find( &m_sts, catt_tt, &key);
	item && item->key.subCid == cid;
	item = (merep_sClassAttr*) tree_FindSuccessor( &m_sts, catt_tt, &item->key)) {
    if ( item->key.hostCid == key.hostCid)
      continue;

    wb_orep *ol = m_vrep->object( &m_sts, item->key.hostCid);
    if ( oddSts()) {
      ol->ref();
      *oarp = pwr_cNAttrRef;
      oarp->Objid = ol->oid();
      oarp->Flags.b.ObjectAttr = 1;
      oarp->Offset = item->offset[0];
      oarp->Size = bd_size;
      oarp->Body = cid;
      ol->unref();
      op->unref();
      return;
    }
  }
  m_sts = LDH__NONEXT;
  op->unref();
}

void wb_volume::aref( pwr_tCid cid, wb_object o, pwr_sAttrRef *arp)
{
  tree_sTable *catt_tt = m_vrep->merep()->buildCatt( &m_sts);

  merep_sClassAttrKey key;
  merep_sClassAttr *item;

  key.subCid = cid;
  key.hostCid = o.cid();
  key.idx = 0;
  item = (merep_sClassAttr *) tree_Find( &m_sts, catt_tt, &key);
  if ( item == NULL) {
    m_sts = LDH__CLASSLIST;
    return;
  }

  wb_cdrep *cd = m_vrep->merep()->cdrep( &m_sts, cid);
  if ( evenSts()) return;

  int bd_size;
  wb_bdrep *bd = cd->bdrep( &m_sts, pwr_eBix_rt);
  if ( oddSts()) {
    bd_size = bd->size();
    delete bd;
  }
  else
    bd_size = 0;
  delete cd;
  
  *arp = pwr_cNAttrRef;
  arp->Objid = o.oid();
  arp->Flags.b.ObjectAttr = 1;
  arp->Offset = item->offset[0];
  arp->Size = bd_size;
  arp->Body = cid;
}

void wb_volume::nextObjectAref( pwr_tCid cid, pwr_sAttrRef *arp,
			  pwr_sAttrRef *oarp)
{
  tree_sTable *catt_tt = m_vrep->merep()->catt_tt();
  int bd_size;

  wb_orep *op = m_vrep->object( &m_sts, arp->Objid);
  if ( evenSts()) return;

  // Get body size
  wb_cdrep *cd = m_vrep->merep()->cdrep( &m_sts, cid);
  if ( evenSts()) return;

  wb_bdrep *bd = cd->bdrep( &m_sts, pwr_eBix_rt);
  if ( oddSts()) {
    bd_size = bd->size();
    delete bd;
  }
  else
    bd_size = 0;
  delete cd;

  op->ref();

  // Find next attribute object in current object
  merep_sClassAttrKey key;
  merep_sClassAttr *item;

  key.subCid = cid;
  key.hostCid = op->cid();
  key.idx = 0;
  for ( item = (merep_sClassAttr*)tree_Find( &m_sts, catt_tt, &key);
	item && item->key.subCid == cid && item->key.hostCid == op->cid();
	item = (merep_sClassAttr*)tree_FindSuccessor( &m_sts, catt_tt, &item->key)) {
    // Find next offset
    for ( int i = 0; i < item->numOffset; i++) {
      if ( item->offset[i] > arp->Offset) {
	*oarp = pwr_cNAttrRef;
	oarp->Objid = op->oid();
	oarp->Flags.b.ObjectAttr = 1;
	oarp->Offset = item->offset[i];
	oarp->Size = bd_size;
	oarp->Body = cid;
	op->unref();
	return;
      }
    }
  }
  m_sts = LDH__NONEXT;
  op->unref();
}

bool wb_volume::isAncestor( wb_object& ancestor, wb_object& o)
{
  if ( ancestor.oid().vid != o.oid().vid)
    return false;

  wb_object p = o.parent();
  while ( p) {
    if ( p.oid().oix == ancestor.oid().oix)
      return true;
    p = p.parent();
  }
  return false;
}







