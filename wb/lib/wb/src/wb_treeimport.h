#ifndef wb_treeimport_h
#define wb_treeimport_h

#include <map>
#include "pwr.h"
#include "wb_ldh.h"

class wb_treeexport;
class wb_adrep;
class wb_vrep;
class wb_orep;

class wb_merep;

class wb_treeimport
{
  map<pwr_tOix, pwr_tOix> m_translation_table;
  pwr_tVid m_import_source_vid;

  typedef map<pwr_tOix, pwr_tOix>::iterator iterator_translation_table;

  bool importUpdateSubClass( wb_adrep *subattr, char *body, wb_vrep *vrep, 
			     bool *modified);
  bool importUpdateObject( wb_orep *o, wb_vrep *vrep);


public:
  virtual ~wb_treeimport() {}
  virtual bool importTree( bool keepref) = 0;
  virtual bool importTreeObject(wb_merep *merep, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody) = 0;
  virtual bool importPaste() = 0;
  virtual bool importPasteObject(pwr_tOid destination, ldh_eDest destcode, bool keepoid,
			  pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody) = 0;
  bool importTranslationTableInsert( pwr_tOix from, pwr_tOix to);
  void importTranslationTableClear();
  pwr_tOix importTranslate( pwr_tOix oix);
  void importSetSourceVid( pwr_tVid vid) { m_import_source_vid = vid;}
  bool importUpdateTree( wb_vrep *vrep);
};

#endif






