/* 
 * Proview   $Id: wb_merep.h,v 1.20 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_merep_h
#define wb_merep_h

#include "wb_mvrep.h"
#include "wb_attrname.h"
#include "wb_vrepdbs.h"
#include "co_tree.h"
#include <map>

using namespace std;

class wb_vrep;
class wb_tdrep;
class wb_adrep;

typedef struct {
  pwr_tCid		subCid;        	/**< Class Id for class attribute */
  pwr_tCid              hostCid;       	/**< Class Id for owner class */
  pwr_tUInt32  		idx;		/**< Index of offset data */
} merep_sClassAttrKey;

#define merep_cCattOffsetSize 20

typedef struct {
  tree_sNode		n;
  merep_sClassAttrKey	key;
  int			numOffset;
  pwr_tUInt32		offset[merep_cCattOffsetSize];
} merep_sClassAttr;  

class wb_merep {

  map<pwr_tVid, wb_mvrep*> m_mvrepdbs;

  wb_erep *m_erep;
  wb_vrep *m_vrep;
  tree_sTable *m_catt_tt;

  typedef map<pwr_tVid, wb_mvrep*>::iterator mvrep_iterator;

public:
  wb_merep( wb_erep *erep, wb_vrep *vrep = 0) : 
    m_erep(erep), m_vrep(vrep), m_catt_tt(0) {}
  ~wb_merep();
  wb_merep( const wb_merep& x, wb_vrep *vrep);
  wb_mvrep *volume(pwr_tStatus *sts);
  wb_mvrep *volume(pwr_tStatus *sts, pwr_tVid vid);
  wb_mvrep *volume(pwr_tStatus *sts, const char *name);

  void copyFiles(const char *dirname);
  
  wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
  void addDbs( pwr_tStatus *sts, wb_mvrep *mvrep);
  void removeDbs( pwr_tStatus *sts, wb_mvrep *mvrep);

  wb_cdrep *cdrep( pwr_tStatus *sts, const wb_orep& o);
  wb_cdrep *cdrep( pwr_tStatus *sts, pwr_tCid cid);
  wb_cdrep *cdrep( pwr_tStatus *sts, wb_name name);
  wb_tdrep *tdrep( pwr_tStatus *sts, const wb_adrep& a);
  wb_tdrep *tdrep( pwr_tStatus *sts, pwr_tTid tid);
  wb_tdrep *tdrep( pwr_tStatus *sts, wb_name name);

  int getAttrInfoRec( wb_attrname *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
                      size_t *offset, pwr_tTid *tid, int *elements, 
                      pwr_eType *type, int *flags, int level);

  void classDependency( pwr_tStatus *sts, pwr_tCid cid, 
			pwr_tCid **lst, pwr_sAttrRef **arlst, int *cnt);
  void classVersion( pwr_tStatus *sts, pwr_tCid cid, pwr_tTime *time);
  tree_sTable *buildCatt( pwr_tStatus *sts);
  void insertCattObject( pwr_tStatus *sts, pwr_tCid cid, 
			 wb_adrep *adp, int offset);
  tree_sTable *catt_tt() { return m_catt_tt;}
  void subClass( pwr_tCid supercid, pwr_tCid subcid, pwr_tCid *nextsubcid, pwr_tStatus *sts);
};

#endif

