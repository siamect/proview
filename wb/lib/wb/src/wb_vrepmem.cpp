

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "wb_vrepmem.h"
#include "wb_orepmem.h"
#include "wb_ldh_msg.h"
#include "wb_dbs.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_tdrep.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_dcli.h"
}

wb_vrepmem::wb_vrepmem( wb_erep *erep, pwr_tVid vid) :
    wb_vrep(vid), m_erep(erep), m_merep(erep->merep()), root_object(0), m_nextOix(0)
{
  pwr_tStatus sts;

  // Create the volume object // Fix class should be pwr_eClass_VolatileVolume...
  wb_cdrep *cdrep = m_merep->cdrep( &sts, pwr_eClass_RootVolume);
  wb_cdef cdef = wb_cdef( cdrep);
  wb_destination d = wb_destination( pwr_cNObjid, ldh_eDest_IntoFirst);
  wb_name n = wb_name("Tmp");
  
  wb_orep *o = createObject( &sts, cdef, d, n);
  delete o;
}

wb_vrepmem::~wb_vrepmem()
{
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

  if ( root_object && root_object->fch) {
    orep = new wb_orepmem( (wb_vrepmem *)this, root_object->fch);
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

  for ( int i = cnt - 1; i >= 0; i--) {
    strcat( str, vect[i]->name());
    if ( i == cnt - 1)
      strcat( str, ":");
    else if ( i != 0)
      strcat( str, "-");
  }
  free( vect);
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






