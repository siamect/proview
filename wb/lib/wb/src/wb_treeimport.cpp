#include "pwr.h"
#include "pwr_class.h"
#include "wb_treeimport.h"
#include "wb_vrep.h"
#include "wb_orep.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"
#include "wb_merep.h"

bool wb_treeimport::importTranslationTableInsert( pwr_tOix from, pwr_tOix to)
{
  pair<pwr_tOix, pwr_tOix>p(from,to);
  pair<map<pwr_tOix,pwr_tOix>::iterator,bool>result = m_translation_table.insert(p);

  return result.second;
}

void wb_treeimport::importTranslationTableClear()
{
  while( ! m_translation_table.empty())
    m_translation_table.erase( m_translation_table.begin());
}

pwr_tOix wb_treeimport::importTranslate( pwr_tOix oix)
{
  iterator_translation_table it = m_translation_table.find( oix);
  if ( it == m_translation_table.end())
    return 0;
  return it->second;
}

bool wb_treeimport::importUpdateTree( wb_vrep *vrep)
{
  iterator_translation_table it;
  pwr_tStatus sts;

  // Update all imported objects, get objects from the translationtable
  for ( it = m_translation_table.begin(); it != m_translation_table.end(); it++) {
    pwr_tOid oid;
    oid.oix = it->second;
    oid.vid = vrep->vid();
    wb_orep *o = vrep->object( &sts, oid);
    if ( EVEN(sts)) throw wb_error( LDH__NOSUCHOBJ);

    o->ref();
    importUpdateObject( o, vrep);
    o->unref();
  }
  return true;
}

bool wb_treeimport::importUpdateSubClass( wb_adrep *subattr, char *body, wb_vrep *vrep, 
					  bool *modified)
{
  pwr_tStatus sts;
  pwr_tOix oix;
  pwr_tCid cid = subattr->subClass();
  wb_cdrep *cdrep = vrep->merep()->cdrep( &sts, cid);
  wb_bdrep *bdrep = cdrep->bdrep( &sts, pwr_eBix_rt);
  if ( EVEN(sts)) return false;

  int subattr_elements = subattr->isArray() ? subattr->nElement() : 1;

  for ( int i = 0; i < subattr_elements; i++) {
    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      if ( adrep->isClass()) {
	importUpdateSubClass( adrep, body + i * subattr->size() / subattr_elements + adrep->offset(),
			vrep, modified);
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( oidp->vid == m_import_source_vid && (oix = importTranslate( oidp->oix))) {
	      oidp->vid = vrep->vid();
	      oidp->oix = oix;
	      *modified = true;
	    }
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + i * subattr->size() / subattr_elements + 
					adrep->offset());
	  for ( int j = 0; j < elements; j++) {
	    if ( arp->Objid.vid == m_import_source_vid && (oix = importTranslate( arp->Objid.oix))) {
	      arp->Objid.vid = vrep->vid();
	      arp->Objid.oix = oix;
	      *modified = true;
	    }
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

bool wb_treeimport::importUpdateObject( wb_orep *o, wb_vrep *vrep)
{
  pwr_tOix oix;
  pwr_tStatus sts;
  wb_cdrep *cdrep = vrep->merep()->cdrep( &sts, o->cid());
  pwr_mClassDef flags = cdrep->flags();

  for ( int i = 0; i < 2; i++) {
    pwr_eBix bix = i ? pwr_eBix_rt : pwr_eBix_dev;

    wb_bdrep *bdrep = cdrep->bdrep( &sts, bix);
    if ( EVEN(sts)) continue;

    
    char *body = (char *)malloc( bdrep->size());
    vrep->readBody( &sts, o, bix, body);
    bool modified = false;

    wb_adrep *adrep = bdrep->adrep( &sts);
    while ( ODD(sts)) {
      int elements = adrep->isArray() ? adrep->nElement() : 1;
      if ( adrep->isClass()) {
	importUpdateSubClass( adrep, body + adrep->offset(), vrep, &modified);
      }
      else {
	switch ( adrep->type()) {
	case pwr_eType_Objid: {
	  pwr_tOid *oidp = (pwr_tOid *)(body + adrep->offset());
	  for ( i = 0; i < elements; i++) {
	    if ( oidp->vid == m_import_source_vid && (oix = importTranslate( oidp->oix))) {
	      oidp->vid = vrep->vid();
	      oidp->oix = oix;
	      modified = true;
	    }
	    oidp++;
	  }
	  break;
	}
	case pwr_eType_AttrRef: {
	  pwr_sAttrRef *arp = (pwr_sAttrRef *)(body + adrep->offset());
	  for ( i = 0; i < elements; i++) {
	    if ( arp->Objid.vid == m_import_source_vid && (oix = importTranslate( arp->Objid.oix))) {
	      arp->Objid.vid = vrep->vid();
	      arp->Objid.oix = oix;
	      modified = true;
	    }
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
    if ( modified)
      vrep->writeBody( &sts, o, bix, body);
    free( body);
    delete bdrep;
  }

  delete cdrep;
  return true;
}







