

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "wb_vrepmem.h"
#include "wb_orepmem.h"
#include "wb_ldh_msg.h"
#include "wb_dbs.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_attribute.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_tdrep.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_dcli.h"
}

wb_vrepmem::wb_vrepmem( wb_erep *erep, pwr_tVid vid) :
  wb_vrep(vid), m_erep(erep), m_merep(erep->merep()), root_object(0), m_nextOix(0),
  m_source_vid(0)
{
#if 0
  pwr_tStatus sts;

  // Create the volume object
  wb_cdrep *cdrep = m_merep->cdrep( &sts, pwr_eClass_VolatileVolume);
  wb_cdef cdef = wb_cdef( cdrep);
  wb_destination d = wb_destination( pwr_cNObjid, ldh_eDest_IntoFirst);
  wb_name n = wb_name("Tmp");
  
  wb_orep *o = createObject( &sts, cdef, d, n);
  delete o;
#endif
  m_vid = vid;
  m_cid = pwr_eClass_VolatileVolume;
}

wb_vrepmem::~wb_vrepmem()
{
}

int wb_vrepmem::nextOix()
{
  m_nextOix++;

  // This oix might be occupied
  while ( findObject( m_nextOix))
    m_nextOix++;

  return m_nextOix;
}

wb_orep *wb_vrepmem::object(pwr_tStatus *sts, pwr_tOid oid)
{
  if ( oid.vid != m_vid) {
    *sts = LDH__BADOBJID;
    return 0;
  }
  
  mem_object *n = findObject( oid.oix);
  if ( !n) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return new wb_orepmem( this, n);
}

wb_orep *wb_vrepmem::object(pwr_tStatus *sts, wb_name &name)
{
  mem_object *n = find( name.name());
  if ( !n) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return new wb_orepmem( this, n);
}

wb_vrep *wb_vrepmem::next()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

void wb_vrepmem::info()
{

  cout << "Volume : " << volume_name << " " << volume_class << " " << m_vid << endl;

}

bool
wb_vrepmem::createSnapshot(const char *fileName)
{
  try {
    wb_dbs dbs(this);
        
    if ( fileName)
      dbs.setFileName( fileName);

    dbs.importVolume(*this);

    return true;
  } catch (wb_error& e) {
    return false;
  }
}

bool
wb_vrepmem::exportVolume(wb_import &i)
{
  try {
    i.importVolume(*this);

    return true;
  } catch (wb_error& e) {
    return false;
  }
}

bool wb_vrepmem::exportHead(wb_import &i)
{
  if ( root_object)
    return root_object->exportHead(i);
  else
    return false;
}

bool wb_vrepmem::exportDbody(wb_import &i)
{
  if ( root_object)
    return root_object->exportDbody(i);
  else
    return false;
  
}

bool wb_vrepmem::exportRbody(wb_import &i)
{
  if ( root_object)
    return root_object->exportRbody(i);
  else
    return false;
}

bool wb_vrepmem::exportMeta(wb_import &i)
{
  return false;
}

mem_object *wb_vrepmem::findObject( pwr_tOix oix)
{
  iterator_oix_list it = m_oix_list.find( oix);
  if ( it == m_oix_list.end())
    return 0;
  return it->second;
}

int wb_vrepmem::nameToOid( const char *name, pwr_tOid *oid)
{
  if ( strncmp( name, "_O", 2) == 0) {
    cdh_StringToObjid( name, oid);
    return 1;
  }

  mem_object *n = find( name);
  if ( n) {
    *oid = n->m_oid;
    return 1;
  }

  // Search in other volume
  pwr_tStatus sts;

  wb_name na(name);
  
  wb_orep *orep = m_erep->object( &sts, na);
  if ( EVEN(sts))
    return 0;
  else {
    *oid = orep->oid();
    // Delete
    orep->ref();
    orep->unref();

    return 1;
  }
}

bool wb_vrepmem::registerObject( pwr_tOix oix, mem_object *node)
{
  pair<pwr_tOix, mem_object *>p(oix,node);
  pair<map<pwr_tOix,mem_object *>::iterator,bool>result = m_oix_list.insert(p);

  return result.second;
}

void wb_vrepmem::registerVolume( const char *name, pwr_tCid cid, pwr_tVid vid, mem_object *node)
{
  m_vid = vid;
  strcpy( volume_name, name);
  strcpy( m_name, name);
  m_cid = cid;
  root_object = node;
}


mem_object *wb_vrepmem::find( const char *name)
{
  wb_name oname = wb_name(name);

  if ( oname.evenSts() || (oname.hasVolume() && !oname.volumeIsEqual(volume_name)))
    return 0;

  if ( root_object)
    return root_object->find( &oname, 0);
  return 0;
}

void wb_vrepmem::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_vrep *wb_vrepmem::ref()
{
  m_nRef++;
  return this;
}

wb_orep *wb_vrepmem::object(pwr_tStatus *sts)
{
  wb_orepmem *orep = 0;

  if ( root_object) {
    orep = new wb_orepmem( (wb_vrepmem *)this, root_object);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NOSUCHOBJ;

  return orep;
}

wb_orep *wb_vrepmem::ancestor(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;

  mem_object *n = ((wb_orepmem *)o)->memobject();
  while ( n->fth)
    ;

  orep = new wb_orepmem( (wb_vrepmem *)this, n);

  *sts = LDH__SUCCESS;
  return orep;
}

wb_orep *wb_vrepmem::parent(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;
        
  if ( ((wb_orepmem *)o)->memobject()->fth) {
    orep = new wb_orepmem( (wb_vrepmem *)this, ((wb_orepmem *)o)->memobject()->fth);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_PARENT;
    
  return orep;
}

wb_orep *wb_vrepmem::after(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;
    
  if ( ((wb_orepmem *)o)->memobject()->fws) {
    orep = new wb_orepmem( (wb_vrepmem *)this, ((wb_orepmem *)o)->memobject()->fws);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_SIBLING;
  return orep;
}

wb_orep *wb_vrepmem::before(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;
    
  if ( ((wb_orepmem *)o)->memobject()->bws) {
    orep = new wb_orepmem( (wb_vrepmem *)this, ((wb_orepmem *)o)->memobject()->bws);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_SIBLING;
    
  return orep;
}

wb_orep *wb_vrepmem::first(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;
    
  if ( ((wb_orepmem *)o)->memobject()->fch) {
    orep = new wb_orepmem( (wb_vrepmem *)this, ((wb_orepmem *)o)->memobject()->fch);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_CHILD;
    
  return orep;
}

wb_orep *wb_vrepmem::child(pwr_tStatus *sts, const wb_orep *o, wb_name &name)
{
  return 0;
}

wb_orep *wb_vrepmem::last(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepmem *orep = 0;

  mem_object *n = ((wb_orepmem *)o)->memobject()->get_lch();
    
  if ( n) {
    orep = new wb_orepmem( (wb_vrepmem *)this, n);
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__NO_CHILD;
    
  return orep;
}

wb_orep *wb_vrepmem::next(pwr_tStatus *sts, const wb_orep *o)
{
  return 0;
}

wb_orep *wb_vrepmem::previous(pwr_tStatus *sts, const wb_orep *o)
{
  return 0;
}

void wb_vrepmem::objectName(const wb_orep *o, char *str)
{
  *str = 0;
        
  // Count ancestors
  int cnt = 0;
  mem_object *n = ((wb_orepmem *)o)->memobject();
  while ( n) {
    cnt++;
    n = n->fth;
  }

  mem_object **vect = (mem_object **) calloc( cnt, sizeof(vect));

  n = ((wb_orepmem *)o)->memobject();
  for ( int i = 0; i < cnt; i++) {
    vect[i] = n;
    n = n->fth;
  }

  strcat( str, name());
  strcat( str, ":");
  for ( int i = cnt - 1; i >= 0; i--) {
    strcat( str, vect[i]->name());
    if ( i != 0)
      strcat( str, "-");
  }
  free( vect);
}

bool wb_vrepmem::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  *sts = LDH__SUCCESS;
    
  mem_object *n = ((wb_orepmem *) o)->memobject();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return false;
    }
    memcpy( (char *)n->rbody + offset, p, MIN(n->rbody_size - offset, size));
    return true;
  case pwr_eBix_dev:
    if ( n->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return false;
    }
    memcpy( (char *)n->dbody + offset, p, MIN(n->dbody_size - offset, size));
    return true;
  default:
    *sts = LDH__NOSUCHBODY;
    return false;
  }
}

void *wb_vrepmem::readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  *sts = LDH__SUCCESS;
    
  mem_object *n = ((wb_orepmem *) o)->memobject();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, (char *)n->rbody + offset, MIN(n->rbody_size - offset, size));
      return p;
    }
    return (void *)((char *)n->rbody + offset);
  case pwr_eBix_dev:
    if ( n->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, (char *)n->dbody + offset, MIN(n->dbody_size - offset, size));
      return p;
    }
    return (void *)((char *)n->dbody + offset);
  default:
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
}

bool wb_vrepmem::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__SUCCESS;
    
  mem_object *n = ((wb_orepmem *) o)->memobject();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return false;
    }

    memcpy( n->rbody, p, n->rbody_size);
    return true;
  case pwr_eBix_dev:
    if ( n->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return false;
    }
    memcpy( n->dbody, p, n->dbody_size);
    return true;
  default:
    *sts = LDH__NOSUCHBODY;
    return false;
  }
}

void *wb_vrepmem::readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__SUCCESS;
    
  mem_object *n = ((wb_orepmem *) o)->memobject();

  switch ( bix) {
  case pwr_eBix_rt:
    if ( n->rbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, n->rbody, n->rbody_size);
      return p;
    }
    return n->rbody;
  case pwr_eBix_dev:
    if ( n->dbody_size == 0) {
      *sts = LDH__NOSUCHBODY;
      return 0;
    }
    if ( p) {
      memcpy( p, n->dbody, n->dbody_size);
      return p;
    }
    return n->dbody;
  default:
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
}


mem_object *mem_object::find( wb_name *oname, int level)
{
  if ( oname->segmentIsEqual( m_name, level)) {
    if ( !oname->hasSegment(level+1))
      return this;
    else if ( fch)
      return fch->find( oname, level+1);
    else
      return 0;
  }
  else if ( fws)
    return fws->find( oname, level);
  else
    return 0;
  return 0;
}

wb_orep *wb_vrepmem::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  mem_object *dest;
  ldh_eDest code = d.code();
  char name_str[32];

  m_nextOix++;
  if ( name.evenSts())
    sprintf( name_str, "O%d", m_nextOix);
  else
    strcpy( name_str, name.object());

  if ( cdh_ObjidIsNull( d.oid())) {
    dest = root_object;
    if ( code == ldh_eDest_After)
      code = ldh_eDest_IntoLast;
    if ( code == ldh_eDest_Before)
      code = ldh_eDest_IntoFirst;
  }
  else {
    dest = findObject( d.oid().oix);
    if ( !dest) {
      *sts = LDH__BADDEST;
      return 0;
    }
  }

  mem_object *memo = new mem_object();
  strcpy( memo->m_name, name_str);
  memo->m_oid.oix = m_nextOix;
  memo->m_oid.vid = m_vid;
  memo->m_cid = cdef.cid();
  memo->rbody_size = cdef.size( pwr_eBix_rt);
  if ( memo->rbody_size) {
    memo->rbody = malloc( memo->rbody_size);
    cdef.templateBody( sts, pwr_eBix_rt, memo->rbody);
    if ( EVEN(*sts)) return 0;
  }
  memo->dbody_size = cdef.size( pwr_eBix_dev);
  if ( memo->dbody_size) {
    memo->dbody = malloc( memo->dbody_size);
    cdef.templateBody( sts, pwr_eBix_dev, memo->dbody);
    if ( EVEN(*sts)) return 0;
  }

  if ( !root_object) {
    // This is the root object
    root_object = memo;
    wb_orepmem *o = new wb_orepmem( this, memo);
    return o;
  }

  switch ( code) {
  case ldh_eDest_IntoFirst:
    memo->fws = dest->fch;
    dest->fch = memo;
    memo->fth = dest;
    break;
  case ldh_eDest_IntoLast:
    {
    mem_object *lch = dest->get_lch();
    if ( lch)
      lch->fws = memo;
    else
      dest->fch = memo;
    memo->bws = lch;
    memo->fth = dest;
    break;
    }
  case ldh_eDest_After:
    memo->fws = dest->fws;
    memo->bws = dest;
    dest->fws = memo;
    memo->fth = dest->fth;
    break;
  case ldh_eDest_Before:
    memo->bws = dest->bws;
    memo->fws = dest;
    dest->bws = memo;
    memo->fth = dest->fth;
    if ( dest->bws)
      dest->bws->fws = memo;
    else
      dest->fth->fch = memo;
    break;
  default:
    *sts = LDH__NODEST;
    delete memo;
    return 0;
  }
  registerObject( memo->m_oid.oix, memo);

  wb_orepmem *o = new wb_orepmem( this, memo);
  return o;
}

bool wb_vrepmem::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
  mem_object *memo = ((wb_orepmem *)orep)->memobject();

  if ( memo->fch) {
    *sts = LDH__HAS_CHILD;
    return false;
  }
     
  if ( !memo->bws && memo->fth)
    memo->fth->fch = memo->fws;
  if ( memo->bws)
    memo->bws->fws = memo->fws;
  if ( memo->fws)
    memo->fws->bws = memo->bws;

  delete memo;

  return true;
}

bool wb_vrepmem::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  mem_object *memo = ((wb_orepmem *)orep)->memobject();

  if ( !memo->bws && memo->fth)
    memo->fth->fch = memo->fws;
  if ( memo->bws)
    memo->bws->fws = memo->fws;
  if ( memo->fws)
    memo->fws->bws = memo->bws;

  memo->deleteChildren();

  delete memo;

  return true;
}

//
// Update oid and attrref attributes, reset extern references
//
bool wb_vrepmem::importTree()
{
  if ( !root_object)
    return true;

  pwr_tStatus sts;

  wb_orep *o = object( &sts);
  if ( EVEN(sts)) return false;

  while ( ODD(sts)) {
    o->ref();
    updateObject( o);

    wb_orep *prev = o;
    o = o->after( &sts);
    prev->unref();
  }
  return true;
}

bool wb_vrepmem::updateSubClass( wb_adrep *subattr, char *body)
{
  pwr_tStatus sts;
  pwr_tCid cid = subattr->subClass();
  wb_cdrep *cdrep = m_merep->cdrep( &sts, cid);
  wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
  if ( EVEN(sts)) return false;

  int subattr_elements = subattr->isArray() ? subattr->nElement() : 1;

  for ( int i = 0; i < subattr_elements; i++) {
    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      if ( adrep->isClass()) {
	updateSubClass( adrep, body + i * subattr->size() / subattr_elements + adrep->offset());
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( oidp->vid == m_source_vid && findObject( oidp->oix))
	      oidp->vid = m_vid;
	    else 
	      *oidp = pwr_cNOid;
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( arp->Objid.vid == m_source_vid && findObject( arp->Objid.oix))
	      arp->Objid.vid = m_vid;
	    else
	      arp->Objid = pwr_cNOid;
	    arp++;
	  }
	  break;
	}
	default:
	  ;
	}
      }
      wb_adrep *prev = adrep;
      adrep = adrep->next( &sts);
      delete prev;
    }
  }
  delete bdrep;
  delete cdrep;

  return true;
}

bool wb_vrepmem::updateObject( wb_orep *o)
{
  pwr_tStatus sts;
  wb_cdrep *cdrep = m_merep->cdrep( &sts, o->cid());
  pwr_mClassDef flags = cdrep->flags();

  for ( int i = 0; i < 2; i++) {
    pwr_eBix bix = i ? pwr_eBix_rt : pwr_eBix_dev;

    wb_bdrep *bdrep = cdrep->bdrep( &sts, bix);
    if ( EVEN(sts)) continue;

    char *body = (char *) readBody( &sts, o, bix, 0);

    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      if ( adrep->isClass()) {
	updateSubClass( adrep, body + adrep->offset());
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + adrep->offset());
	  for ( i = 0; i < elements; i++) {
	    if ( oidp->vid == m_source_vid && findObject( oidp->oix))
	      oidp->vid = m_vid;
	    else
	      *oidp = pwr_cNOid;
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + adrep->offset());
	  for ( i = 0; i < elements; i++) {
	    if ( arp->Objid.vid == m_source_vid && findObject( arp->Objid.oix))
	      arp->Objid.vid = m_vid;
	    else
	      arp->Objid = pwr_cNOid;
	    arp++;
	  }
	  break;
	}
	default:
	  ;
	}
      }
      wb_adrep *prev = adrep;
      adrep = adrep->next( &sts);
      delete prev;
    }
    delete bdrep;
  }

  delete cdrep;

  wb_orep *child = o->first( &sts);
  while ( ODD(sts)) {
    child->ref();
    updateObject( child);

    wb_orep *prev = child;
    child = child->after( &sts);
    prev->unref();
  }
  return true;
}

bool wb_vrepmem::importTreeObject(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody)
{
  mem_object *memo = new mem_object();
  strcpy( memo->m_name, name);
  memo->m_oid.oix = oid.oix;
  memo->m_oid.vid = m_vid;
  memo->m_cid = cid;
  memo->rbody_size = rbSize;
  if ( memo->rbody_size) {
    memo->rbody = malloc( memo->rbody_size);
    memcpy( memo->rbody, rbody, memo->rbody_size);
  }
  memo->dbody_size = dbSize;
  if ( memo->dbody_size) {
    memo->dbody = malloc( memo->dbody_size);
    memcpy( memo->dbody, dbody, memo->dbody_size);
  }

  if ( cdh_ObjidIsNull( poid) && cdh_ObjidIsNull( boid)) {
    // This is a top object
    if ( !root_object) {
      root_object = memo;
      m_source_vid = oid.vid;
    }
    else {
      // Insert as last sibling to rootobject
      mem_object *next = root_object;
      while( next->fws)
	next = next->fws;

      next->fws = memo;
      memo->bws = next;
    }
  }
  else if ( cdh_ObjidIsNotNull( boid)) {
    // Insert as next sibling to boid
    mem_object *bws = findObject( boid.oix);
    if ( !bws) {
      delete memo;
      return false;
    }
    memo->bws = bws;
    memo->fws = bws->fws;
    memo->fth = bws->fth;
    if ( bws->fws)
      bws->fws->bws = memo;
    bws->fws = memo;
  }
  else {
    // Insert as first child to poid
    mem_object *fth = findObject( poid.oix);
    if ( !fth) {
      delete memo;
      return false;
    }
    memo->fth = fth;
    memo->fws = fth->fch;
    if ( fth->fch)
      fth->fch->bws = memo;
    fth->fch = memo;
  }
  registerObject( memo->m_oid.oix, memo);

  return true;
}

bool wb_vrepmem::importPasteObject(pwr_tOid destination, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody)
{
  mem_object *memo = new mem_object();
  strcpy( memo->m_name, name);

  memo->m_oid.oix = nextOix();

  if ( cdh_ObjidIsNotNull( boid)) {
    boid.oix = importTranslate( boid.oix);
    if ( !boid.oix) return false;
  }
  if ( cdh_ObjidIsNotNull( poid)) {
    poid.oix = importTranslate( poid.oix);
    if ( !poid.oix) return false;
  }

  memo->m_oid.vid = m_vid;
  memo->m_cid = cid;
  memo->rbody_size = rbSize;
  if ( memo->rbody_size) {
    memo->rbody = malloc( memo->rbody_size);
    memcpy( memo->rbody, rbody, memo->rbody_size);
  }
  memo->dbody_size = dbSize;
  if ( memo->dbody_size) {
    memo->dbody = malloc( memo->dbody_size);
    memcpy( memo->dbody, dbody, memo->dbody_size);
  }

  if ( cdh_ObjidIsNull( poid) && cdh_ObjidIsNull( boid)) {
    // This is the top object
    importSetSourceVid( oid.vid);

    if ( !root_object) {
      if ( cdh_ObjidIsNull( destination))
        root_object = memo;
      else
	return false;
    }
    else {
      // Insert as next sibling to destination object
      mem_object *dest = findObject( destination.oix);
      if ( !dest) 
	return false;

      memo->bws = dest;
      memo->fws = dest->fws;
      memo->fth = dest->fth;
      if ( dest->fws)
	dest->fws->bws = memo;
      dest->fws = memo;
    }
  }
  else if ( cdh_ObjidIsNotNull( boid)) {
    // Insert as next sibling to boid
    mem_object *bws = findObject( boid.oix);
    if ( !bws) {
      delete memo;
      return false;
    }
    memo->bws = bws;
    memo->fws = bws->fws;
    memo->fth = bws->fth;
    if ( bws->fws)
      bws->fws->bws = memo;
    bws->fws = memo;
  }
  else {
    // Insert as first child to poid
    mem_object *fth = findObject( poid.oix);
    if ( !fth) {
      delete memo;
      return false;
    }
    memo->fth = fth;
    memo->fws = fth->fch;
    if ( fth->fch)
      fth->fch->bws = memo;
    fth->fch = memo;
  }
  registerObject( memo->m_oid.oix, memo);
  importTranslationTableInsert( oid.oix, memo->m_oid.oix);

  return true;
}

bool wb_vrepmem::importPaste()
{
  importUpdateTree( this);
  importTranslationTableClear();
  return true;
}

bool wb_vrepmem::exportTree(wb_treeimport &i, pwr_tOid oid)
{
  mem_object *memo = findObject( oid.oix);
  if ( !memo)
    return false;

  memo->exportTree( i, true);
  return true;
}

bool wb_vrepmem::exportPaste(wb_treeimport &i, pwr_tOid destination)
{
  if ( root_object) {
    root_object->exportPaste( i, destination, true);
    i.importPaste();
  }
  return true;
}





