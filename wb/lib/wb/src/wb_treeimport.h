/* 
 * Proview   $Id: wb_treeimport.h,v 1.8 2005-09-06 10:43:32 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#ifndef wb_treeimport_h
#define wb_treeimport_h

#include <map>
#include "pwr.h"
#include "wb_ldh.h"

using namespace std;

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
				pwr_tOid boid, const char *name, pwr_mClassDef flags,
				size_t rbSize, size_t dbSize, void *rbody, void *dbody) = 0;
  virtual bool importPaste() = 0;
  virtual bool importPasteObject(pwr_tOid destination, ldh_eDest destcode, bool keepoid,
				 pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
				 pwr_tOid boid, const char *name, pwr_mClassDef flags,
				 size_t rbSize, size_t dbSize, void *rbody, void *dbody,
				 pwr_tOid *roid) = 0;
  virtual void importIgnoreErrors() = 0;
  bool importTranslationTableInsert( pwr_tOix from, pwr_tOix to);
  void importTranslationTableClear();
  pwr_tOix importTranslate( pwr_tOix oix);
  void importSetSourceVid( pwr_tVid vid) { m_import_source_vid = vid;}
  bool importUpdateTree( wb_vrep *vrep);
};

#endif






