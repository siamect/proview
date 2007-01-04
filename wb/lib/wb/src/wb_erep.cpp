/* 
 * Proview   $Id: wb_erep.cpp,v 1.48 2007-01-04 07:29:03 claes Exp $
 * Copyright (C) 2005 SSAB OxelÃ¶sund AB.
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

#include <iostream.h>
#include <fstream.h>

#include "pwr.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_vrepwbl.h"
#include "wb_vrepdbs.h"
#include "wb_vrepdb.h"
//#include "wb_vrepdbms.h"
#include "wb_vrepref.h"
#include "wb_vrepext.h"
#include "wb_cdrep.h"
#include "wb_orep.h"
#include "wb_tdrep.h"
#include "wb_adrep.h"
#include "wb_name.h"
#include "wb_dblock.h"
#include "wb_ldh_msg.h"
#include "co_msgwindow.h"

extern "C" {
#include "co_dcli.h"
#include "co_cdh.h"
#include "rt_load.h"
#include "wb_pwrs.h"
}

pwr_dImport pwr_BindClasses(System);
pwr_dImport pwr_BindClasses(Base);

wb_erep::wb_erep( unsigned int options) : m_nRef(0), m_dir_cnt(0), m_volatile_idx(0), m_buffer_max(10),
					  m_ref_merep_occupied(false), m_options( options)
{
  m_merep = new wb_merep(0);

  atexit( at_exit);
}

wb_erep::~wb_erep()
{
  delete m_merep;
}

void wb_erep::at_exit()
{
  wb_dblock::dbunlock_all();
}

void wb_erep::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_erep *wb_erep::ref()
{
  m_nRef++;
  return this;
}


// Get first volume
wb_vrep *wb_erep::volume( pwr_tStatus *sts)
{
  vrep_iterator it = m_vrepdb.begin();
  if ( it == m_vrepdb.end()) {
    it = m_vrepdbs.begin();
    if ( it == m_vrepdbs.end()) {
      *sts = LDH__NOSUCHVOL;
      return 0;
    }
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_vrep *wb_erep::volume(pwr_tStatus *sts, pwr_tVid vid)
{
  vrep_iterator it = m_vrepdb.find( vid);
  if ( it != m_vrepdb.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  it = m_vrepdbs.find(vid);
  if ( it != m_vrepdbs.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  it = m_vrepextern.find(vid);
  if ( it != m_vrepextern.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid) {
      *sts = LDH__SUCCESS;
      return *itb;
    }
  }

  *sts = LDH__NOSUCHVOL;
  return 0;
}


wb_vrep *wb_erep::volume(pwr_tStatus *sts, const char *name)
{
  vrep_iterator it;
  for ( it = m_vrepdb.begin(); it != m_vrepdb.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  for ( it = m_vrepdbs.begin(); it != m_vrepdbs.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  for ( it = m_vrepextern.begin(); it != m_vrepextern.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return 0;
}

wb_vrep *wb_erep::externVolume(pwr_tStatus *sts, pwr_tVid vid)
{
  vrep_iterator it = m_vrepextern.find( vid);
  if ( it == m_vrepextern.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_vrep *wb_erep::bufferVolume(pwr_tStatus *sts)
{
  buffer_iterator it = m_vrepbuffer.begin();
  if ( it == m_vrepbuffer.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return *it;
}

wb_vrep *wb_erep::bufferVolume(pwr_tStatus *sts, char *name)
{
  for ( buffer_iterator it = m_vrepbuffer.begin(); it != m_vrepbuffer.end(); it++) {
    if ( cdh_NoCaseStrcmp( name, (*it)->name()) == 0) {
      *sts = LDH__SUCCESS;
      return *it;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return 0;
}

wb_vrep *wb_erep::nextVolume(pwr_tStatus *sts, pwr_tVid vid)
{
  // Search in db
  vrep_iterator it = m_vrepdb.find( vid);
  if ( it != m_vrepdb.end()) {
    it++;
    if ( it != m_vrepdb.end()) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
    else {
      // Next volume in dbs
      it = m_vrepdbs.begin();
      if ( it != m_vrepdbs.end()) {
        *sts = LDH__SUCCESS;
        return it->second;
      }
      else {
        *sts = LDH__NOSUCHVOL;
        return 0;
      }
    }
  }

  // Search in dbs
  it = m_vrepdbs.find(vid);
  if ( it != m_vrepdbs.end()) {
    it++;
    if ( it != m_vrepdbs.end() && it->second->duplicateDb())
      it++;
    if ( it != m_vrepdbs.end()) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
    else {
      // Next volume in extern
      for ( it = m_vrepextern.begin();
	    it != m_vrepextern.end();
	    it++) {
	if ( it->second->cid() == pwr_eClass_ExternVolume) {
	  *sts = LDH__SUCCESS;
	  return it->second;
	}
      }
      *sts = LDH__NOSUCHVOL;
      return 0;
    }
  }

  // Search in buffer
  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid) {
      itb++;
      if ( itb != m_vrepbuffer.end()) {
        *sts = LDH__SUCCESS;
        return *itb;
      }
      else {
        *sts = LDH__NOSUCHVOL;
        return 0;
      }
    }
  }

  *sts = LDH__NOSUCHVOL;
  return 0;
}

void wb_erep::addDb( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdb.find( vrep->vid());
  if ( it == m_vrepdb.end()) {
    m_vrepdb[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__VOLIDALREXI;
}

void wb_erep::addDbs( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdbs.find( vrep->vid());
  if ( it == m_vrepdbs.end()) {
    m_vrepdbs[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }

  if ( vrep->cid() == pwr_eClass_ClassVolume) {
    m_merep->addDbs( sts, (wb_mvrep *)vrep);
  }
}

void wb_erep::addExtern( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepextern.find( vrep->vid());
  if ( it == m_vrepextern.end()) {
    m_vrepextern[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }
}

wb_vrep *wb_erep::findBuffer( pwr_tVid vid)
{
  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid)
      return *itb;
  }
  return 0;
}

void wb_erep::addBuffer( pwr_tStatus *sts, wb_vrep *vrep)
{
  wb_vrep *v = findBuffer( vrep->vid());
  if ( !v) {
    // Remove oldest buffer
    if ( (int) m_vrepbuffer.size() >= m_buffer_max) {
      m_vrepbuffer.erase( m_vrepbuffer.begin());
    }

    m_vrepbuffer.push_back( vrep);
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }
}

void wb_erep::removeDb(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdb.find( vrep->vid());
  if ( it == m_vrepdb.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepdb.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeDbs(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdbs.find( vrep->vid());
  if ( it == m_vrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepdbs.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeExtern(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepextern.find( vrep->vid());
  if ( it == m_vrepextern.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepextern.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeBuffer(pwr_tStatus *sts, wb_vrep *vrep)
{
  for ( buffer_iterator it = m_vrepbuffer.begin(); it != m_vrepbuffer.end(); it++) {
    if ( *it == vrep) {
      vrep->unref();
      m_vrepbuffer.erase( it);
      *sts = LDH__SUCCESS;
      return;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return;
}

void wb_erep::load( pwr_tStatus *sts, char *db)
{
  loadDirList( sts);
  if ( EVEN(*sts)) return;
  else if ( *sts == LDH__PROJCONFIG) {
    pwr_tStatus status;
    loadCommonMeta( &status);
    if ( EVEN(status)) {
      *sts = status;
      return;
    }
    loadMeta( sts, db);
    bindMethods();
    loadLocalWb( sts);
    wb_vrepref *vrep = new wb_vrepref( this, ldh_cPlcMainVolume);
    addExtern( sts, vrep);
    vrep = new wb_vrepref( this, ldh_cPlcFoVolume);
    addExtern( sts, vrep);
    vrep = new wb_vrepref( this, ldh_cIoConnectVolume);
    addExtern( sts, vrep);
    return;
  }
  loadCommonMeta( sts);
  if ( EVEN(*sts)) return;

  loadMeta( sts, db);
  bindMethods();
  loadLocalWb( sts);

  wb_vrepref *vrep = new wb_vrepref( this, ldh_cPlcMainVolume);
  addExtern( sts, vrep);
  vrep = new wb_vrepref( this, ldh_cPlcFoVolume);
  addExtern( sts, vrep);
  vrep = new wb_vrepref( this, ldh_cIoConnectVolume);
  addExtern( sts, vrep);
}

void wb_erep::loadDirList( pwr_tStatus *status)
{
  // Read directory file
  char line[200];
  char fname[200];

  dcli_translate_filename( m_dir_list[0], "$pwr_load");
  strcat( m_dir_list[0], "/");
  m_dir_cnt = 1;

  dcli_translate_filename( fname, load_cNameFilePath);
  ifstream fp( fname, ios::in);
  if ( !fp) {
    *status = LDH__PROJCONFIG;
    return;
  }

  while ( fp.getline( line, sizeof(line))) {
    if ( m_dir_cnt > (int) (sizeof(m_dir_list)/sizeof(m_dir_list[0]) - 1))
      break;
    dcli_translate_filename( m_dir_list[m_dir_cnt], line);
    if ( m_dir_list[m_dir_cnt][strlen(m_dir_list[m_dir_cnt])-1] != '/')
      strcat( m_dir_list[m_dir_cnt], "/");
    m_dir_cnt++;
  }
  fp.close();
}

void wb_erep::loadCommonMeta( pwr_tStatus *status)
{
  char fname[200];
  char fdir[200];
  char vname[200];
  char line[200];
  int i;
  pwr_tVid vid;
  pwr_tStatus sts;

  for ( i = 0; i < m_dir_cnt; i++) {

    // Load metavolumes
    strcpy( fdir, m_dir_list[i]);
    strcpy( fname, fdir);
    strcat( fname, load_cNameCmnVolumeList);

    ifstream fpm( fname, ios::in);
    if ( !fpm) {
      if ( i == 0) {
        *status = LDH__NOBASE;
        return;
      }
      else
        continue;
    }
    while ( fpm.getline( line, sizeof(line))) {
      char vol_array[2][80];
      int nr;

      if ( line[0] == '#')
        continue;

      nr = dcli_parse( line, " ", "", (char *)vol_array,
                       sizeof(vol_array)/sizeof(vol_array[0]),
                       sizeof(vol_array[0]), 0);
      if ( nr != 2)
        MsgWindow::message( 'E', "Syntax error in file:", fname);

      // Load this volume
      cdh_ToLower( vol_array[0], vol_array[0]);
      strcpy( vname, fdir);
      strcat( vname, vol_array[0]);
      strcat( vname, ".dbs");
      cdh_StringToVolumeId( vol_array[1], &vid);

      wb_vrepdbs *vrep = new wb_vrepdbs( this, vname);
      try {
        vrep->load();
        addDbs( &sts, vrep);
        char buff[256];
	if ( vrep->cid() == pwr_eClass_ClassVolume)
	  sprintf(buff, "Global class volume \"%s\" loaded from \"%s\"", vrep->dbsenv()->vp->name, vname);
	else
	  sprintf(buff, "Volume \"%s\" loaded from \"%s\"", vrep->dbsenv()->vp->name, vname);
        MsgWindow::message( 'O', buff);
      }
      catch ( wb_error& e) {
	if ( m_options & ldh_mWbOption_IgnoreDLoadError)
	  MsgWindow::message( 'I', "Unable to open class volume", vname);
	else
        MsgWindow::message( 'E', "Unable to open class volume", vname, e.what().c_str());
      }
    }
    fpm.close();
  }
  *status = LDH__SUCCESS;
}

void wb_erep::loadMeta( pwr_tStatus *status, char *db)
{
  // Load local metavolumes
  char found_file[200];
  char fname[200];
  char vname[200];
  char line[200];
  int i;
  pwr_tVid vid;
  pwr_tStatus sts;
  wb_vrepdbs *vrep;
  int vol_cnt = 0;

  strcpy( fname, load_cNameVolumeList);
  dcli_translate_filename( fname, fname);

  ifstream fpm( fname, ios::in);
  if ( !fpm) {
    *status = LDH__PROJCONFIG;

    // Load directory volume
    strcpy( vname, "$pwrp_db/directory.db");
    dcli_translate_filename( vname, vname);

    sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
    if ( ODD(sts)) {
      wb_vrepdb *vrepdb = new wb_vrepdb( this, vname);
      vrepdb->name("directory");
      addDb( &sts, vrepdb);
      MsgWindow::message( 'I', "Directory database opened", vname);
    }

    return;
  }

  while ( fpm.getline( line, sizeof(line))) {
    char vol_array[7][80];
    int nr;

    if ( line[0] == '#')
      continue;

    nr = dcli_parse( line, " ", "", (char *)vol_array,
                     sizeof(vol_array)/sizeof(vol_array[0]),
                     sizeof(vol_array[0]), 0);
    if ( nr > 6)
      cout << "Syntax error in file: " << fname << endl;

    cdh_StringToVolumeId( vol_array[1], &vid);

    if ( cdh_NoCaseStrcmp( vol_array[2], "ExternVolume") == 0) {
      if ( nr != 6)
	cout << "Syntax error in file: " << fname << endl;

      // Load extern volume for this volume
      cdh_ToLower( vol_array[0], vol_array[0]);

      try {
	wb_vrepext *vrepext = new wb_vrepext( this, vid, vol_array[0], vol_array[4]);
	addExtern( &sts, vrepext);
	MsgWindow::message( 'O', "Volume loaded", vname);
	vol_cnt++;
      }
      catch ( wb_error& e) {
	MsgWindow::message( 'E', "Unable to open volume", vname, e.what().c_str());
      }
    }
    else if ( (cdh_NoCaseStrcmp( vol_array[2], "ClassVolume") == 0) ||
         (strcmp( vol_array[3], "load") == 0)) {
      if ( nr != 4)
	cout << "Syntax error in file: " << fname << endl;

      // Load dbs for this volume
      cdh_ToLower( vol_array[0], vol_array[0]);

      if ( strcmp( vol_array[3], "cnf") == 0) {
        // Configured in this project, load from pwrp_load
        strcpy( vname, "$pwrp_load/");
        strcat( vname, vol_array[0]);
        strcat( vname, ".dbs");
        dcli_translate_filename( vname, vname);

        // Load...
        try {
          vrep = new wb_vrepdbs( this, vname);
          vrep->load();
          addDbs( &sts, vrep);
          MsgWindow::message( 'O', "Volume loaded from snapshot file", vname);
          vol_cnt++;
        }
        catch ( wb_error& e) {
	  if ( m_options & ldh_mWbOption_IgnoreDLoadError)
	    MsgWindow::message( 'I', "Unable to open volume snapshot file", vname);
	  else
          MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
        }
      }
      else {
        // Imported loadfile
        for ( i = 0; i < m_dir_cnt; i++) {
          strcpy( vname, m_dir_list[i]);
          strcat( vname, vol_array[0]);
          strcat( vname, ".dbs");
          sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
          dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
          if ( ODD(sts)) {
            // Load...
            try {
              vrep = new wb_vrepdbs( this, vname);
              vrep->load();
              // vrep->name( vol_array[0]);
              addDbs( &sts, vrep);
              MsgWindow::message( 'O', "Volume loaded from snapshot file", vname);
            }
            catch ( wb_error& e) {
	      if ( m_options & ldh_mWbOption_IgnoreDLoadError)
	        MsgWindow::message( 'I', "Unable to open volume snapshot file", vname);
	      else
              MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
            }
            break;
          }
        }
      }
    }
    else {
      // Load db for this volume
      char uname[80];

      if ( nr != 4)
	cout << "Syntax error in file: " << fname << endl;

      if ( db) {
        // If db is specified, load only specified db, load as dbs instead
        if ( cdh_NoCaseStrcmp( vol_array[0], db) != 0) {
          cdh_ToLower( vol_array[0], vol_array[0]);
          strcpy( vname, "$pwrp_load/");
          strcat( vname, vol_array[0]);
          strcat( vname, ".dbs");
          sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
          dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
          if ( ODD(sts)) {
            // Load...
            try {
              vrep = new wb_vrepdbs( this, found_file);
              vrep->load();
              // vrep->name( vol_array[0]);
              addDbs( &sts, vrep);
              MsgWindow::message( 'O', "Volume loaded from snapshot file", vname);
            }
            catch ( wb_error& e) {
	      if ( m_options & ldh_mWbOption_IgnoreDLoadError)
	        MsgWindow::message( 'I', "Unable to open volume snapshot file", vname);
	      else
              MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
            }
          }
          continue;
        }
      }
      strcpy( vname, "$pwrp_db/");
      strcat( vname, vol_array[0]);
      cdh_ToLower( vname, vname);
      strcat( vname, ".db");
      dcli_translate_filename( vname, vname);

      sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
      if ( wb_dblock::is_locked(vname, uname)) {

        MsgWindow::message( 'E', "Database is locked by user", uname, vname);
	if ( MsgWindow::has_window())
	  exit(0);

        // Try to load dbs-file instead
        cdh_ToLower( vol_array[0], vol_array[0]);
        strcpy( vname, "$pwrp_load/");
        strcat( vname, vol_array[0]);
        strcat( vname, ".dbs");
        dcli_translate_filename( vname, vname);

        try {
          vrep = new wb_vrepdbs( this, vname);
          vrep->load();
          addDbs( &sts, vrep);
          MsgWindow::message( 'I', "Volume loaded from snapshot file", vname);
          vol_cnt++;
        }
        catch ( wb_error& e) {
	  if ( m_options & ldh_mWbOption_IgnoreDLoadError)
	    MsgWindow::message( 'I', "Unable to open volume snapshot file", vname);
	  else
          MsgWindow::message( 'E', "Unable to open volume snapshot file", vname, e.what().c_str());
        }
      }
      else {
        if ( ODD(sts)) {
	  wb_vrepdb *vrepdb = new wb_vrepdb( this, vname);
          vrepdb->name(vol_array[0]);
          addDb( &sts, vrepdb);
          MsgWindow::message( 'I', "Database opened", vname);
          vol_cnt++;
        }
        else
          MsgWindow::message( 'E', "Database not found", vname);
      }
    }
  }
  fpm.close();

  // Identify dbs that also is loaded as db
  vrep_iterator itdbs, itdb;

  for ( itdbs = m_vrepdbs.begin(); itdbs != m_vrepdbs.end(); itdbs++) {
    for ( itdb = m_vrepdb.begin(); itdb != m_vrepdb.end(); itdb++) {
      if ( itdbs->first == itdb->first)
        itdbs->second->setDuplicateDb( true);
    }
  }

  // Load directory volume

  if ( !db || (db && cdh_NoCaseStrcmp( "directory", db) == 0)) {
    char uname[80];

    strcpy( vname, "$pwrp_db/directory.db");
    dcli_translate_filename( vname, vname);

    sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
    if ( wb_dblock::is_locked(vname, uname))
      MsgWindow::message( 'E', "Database is locked by user", uname, vname);
    else {
      if ( ODD(sts)) {
	wb_vrepdb *vrepdb = new wb_vrepdb( this, vname);
        vrepdb->name("directory");
        addDb( &sts, vrepdb);
        MsgWindow::message( 'I', "Database opened", vname);
      }
      if ( EVEN(sts)) {
        *status = LDH__PROJCONFIG;
        return;
      }
    }
  }
  if ( !vol_cnt)
    *status = LDH__PROJCONFIG;
  else
    *status = LDH__SUCCESS;

}

void wb_erep::loadLocalWb( pwr_tStatus *rsts)
{
  pwr_tStatus sts;
  char found_file[200];

  sts = dcli_search_file( load_cNameLocalWb, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( load_cNameLocalWb, found_file, DCLI_DIR_SEARCH_END);
  if ( ODD(sts)) {
    try {
      wb_vrepwbl *vrep = new wb_vrepwbl( this);
      sts = vrep->load( found_file);
      if ( ODD(sts))
        addExtern( &sts, vrep);
      else {
        *rsts = sts;
        MsgWindow::message('E', "Failiure loading local workbench volume");
        return;
      }
    }
    catch ( wb_error& e) {
      *rsts = e.sts();
      MsgWindow::message('E', "Failiure loading local workbench volume");
      return;
    }
  }
  *rsts = LDH__SUCCESS;
}

wb_cdrep *wb_erep::cdrep( pwr_tStatus *sts, const wb_orep& o)
{
  wb_vrep *vrep = volume(sts, o.vid());
  if ( EVEN(*sts)) return 0;

  return vrep->merep()->cdrep( sts, o);
}

wb_tdrep *wb_erep::tdrep( pwr_tStatus *sts, const wb_adrep& a)
{
  pwr_tVid vid = cdh_TidToVid(a.type());
  wb_vrep *vrep = volume(sts, vid);
  if ( EVEN(*sts)) return 0;

  return vrep->merep()->tdrep( sts, a);
}

wb_orep *wb_erep::object(pwr_tStatus *sts, pwr_tOid oid)
{
  wb_vrep *vrep = volume( sts, oid.vid);
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, oid);
}

wb_orep *wb_erep::object( pwr_tStatus *sts, const char *name)
{
  wb_name n(name);

  if ( n.evenSts()) {
    *sts = n.sts();
    return 0;
  }

  wb_vrep *vrep = volume( sts, n.volume());
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, n);
}

wb_orep *wb_erep::object(pwr_tStatus *sts, wb_name &name)
{
  if ( name.evenSts()) {
    *sts = name.sts();
    return 0;
  }

  wb_vrep *vrep = volume( sts, name.volume());
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, name);
}


void wb_erep::method( pwr_tStatus *sts, char *methodName, wb_tMethod *method)
{
  string key = string( methodName);
  methods_iterator it = m_methods.find( key);
  if ( it == m_methods.end()) {
    *sts = LDH__NOMETHOD;
    return;
  }
  *sts = LDH__SUCCESS;
  *method = it->second;
}

void wb_erep::bindMethods()
{
  int i, j;
  pwr_tStatus sts;
  char str[200];

  for (i = 0;; i++) {
    if (pwr_gSystem_ClassMethods[i].ClassName[0] == '\0')
      break;
    for (j = 0;; j++) {
      if ((*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
        break;
      strcpy( str, pwr_gSystem_ClassMethods[i].ClassName);
      strcat( str, "-");
      strcat( str, (*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName);
      string key = string( str);
      m_methods[ key] = (*pwr_gSystem_ClassMethods[i].Methods)[j].Method;
    }
  }

  for (i = 0;; i++) {
    if (pwr_gBase_ClassMethods[i].ClassName[0] == '\0') break;
    wb_name cname = wb_name( pwr_gBase_ClassMethods[i].ClassName);
    wb_cdrep *cdrep = m_merep->cdrep( &sts, cname);
    if ( EVEN(sts))
      continue;

    for (j = 0;; j++) {
      if ((*pwr_gBase_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
        break;
      strcpy( str, pwr_gBase_ClassMethods[i].ClassName);
      strcat( str, "-");
      strcat( str, (*pwr_gBase_ClassMethods[i].Methods)[j].MethodName);
      string key = string( str);
      m_methods[ key] = (*pwr_gBase_ClassMethods[i].Methods)[j].Method;
    }
    delete cdrep;
  }

#if DEBUG
  for (  methods_iterator it = m_methods.begin(); it != m_methods.end(); it++) {
    cout << " Method: " << it->first << endl;
  }
#endif
}

void wb_erep::printMethods()
{
  int i, j;

  printf("System Methods\n");
  for (i = 0;; i++) {
    if (pwr_gSystem_ClassMethods[i].ClassName[0] == '\0')
      break;
    printf( "%3d %-20s\n", i, pwr_gSystem_ClassMethods[i].ClassName);
    for (j = 0;; j++) {
      if ((*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      printf( "       %s\n", (*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName);
    }
  }

  printf( "Base Methods\n");
  for (i = 0;; i++) {
    if (pwr_gBase_ClassMethods[i].ClassName[0] == '\0') break;

    printf( "%3d %-20s\n", i, pwr_gBase_ClassMethods[i].ClassName);
    for (j = 0;; j++) {
      if ((*pwr_gBase_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      printf( "       %s\n", (*pwr_gBase_ClassMethods[i].Methods)[j].MethodName);
    }
  }
}

int wb_erep::nextVolatileVid( pwr_tStatus *sts, char *name)
{
  pwr_tVid vid = ldh_cVolatileVolMin + m_volatile_idx++;
  if ( vid > ldh_cVolatileVolMax) {
    // Recycle identities, and hope that the old volumes are history
    m_volatile_idx = 0;
    vid = ldh_cVolatileVolMin + m_volatile_idx++;
  }
  if ( name)
    // Suggest a name
    sprintf( name, "Buffer%03d", vid & 255);

  return vid;
}

wb_vrep *wb_erep::createVolume(pwr_tStatus *sts, pwr_tVid vid, pwr_tCid cid,
			      const char *name, bool add)
{
  char vname[200];

  sprintf( vname, "$pwrp_db/%s.db", cdh_Low(name));
  dcli_translate_filename( vname, vname);

  vrep_iterator it = m_vrepdb.find( vid);
  if ( it != m_vrepdb.end()) {
    *sts = LDH__VOLIDALREXI;
    return 0;
  }

  wb_vrepdb *vrepdb = new wb_vrepdb( this, vid, cid, name, vname);
  if ( add)
  addDb( sts, vrepdb);
  MsgWindow::message( 'I', "Database created", vname);

  return vrepdb;
}

void wb_erep::volumeNameToFilename( pwr_tStatus *sts, char *name, char *filename)
{
  char found_file[200];
  char vname[200];
  char line[200];
  char fname[200];
  char dir_list[11][200];
  int dir_cnt;
  int fsts;

  // Read directory file
  strcpy( dir_list[0], "$pwr_load/");
  strcpy( dir_list[1], "$pwrp_load/");
  dir_cnt = 2;

  dcli_translate_filename( fname, load_cNameFilePath);
  ifstream fp( fname, ios::in);
  if ( !fp) {
    *sts = LDH__PROJCONFIG;
    return;
  }

  while ( fp.getline( line, sizeof(line))) {
    if ( dir_cnt > (int) (sizeof(dir_list)/sizeof(dir_list[0]) - 1))
      break;
    strcpy( dir_list[dir_cnt], line);
    if ( dir_list[dir_cnt][strlen(dir_list[dir_cnt])-1] != '/')
      strcat( dir_list[dir_cnt], "/");
    dir_cnt++;
  }
  fp.close();

  for ( int i = 0; i < dir_cnt; i++) {
    strcpy( vname, dir_list[i]);
    cdh_ToLower( &vname[strlen(vname)], name);
    strcat( vname, ".dbs");
    fsts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
    if ( ODD(fsts)) {
      strcpy( filename, vname);
      *sts = LDH__SUCCESS;
      return;
    }
  }
  *sts = LDH__NOSUCHFILE;
}


void wb_erep::setRefMerep( wb_merep *merep)
{
  pwr_tStatus sts;
  m_ref_merep_occupied = true;

  wb_vrepref *vrepref = (wb_vrepref *) volume( &sts, ldh_cPlcMainVolume);
  if ( ODD(sts))
    vrepref->setMerep( merep);
  vrepref = (wb_vrepref *) volume( &sts, ldh_cPlcFoVolume);
  if ( ODD(sts))
    vrepref->setMerep( merep);
  vrepref = (wb_vrepref *) volume( &sts, ldh_cIoConnectVolume);
  if ( ODD(sts))
    vrepref->setMerep( merep);
}

void wb_erep::resetRefMerep()
{
  pwr_tStatus sts;
  m_ref_merep_occupied = false;

  wb_vrepref *vrepref = (wb_vrepref *) volume( &sts, ldh_cPlcMainVolume);
  if ( ODD(sts))
    vrepref->setMerep( m_merep);
  vrepref = (wb_vrepref *) volume( &sts, ldh_cPlcFoVolume);
  if ( ODD(sts))
    vrepref->setMerep( m_merep);
  vrepref = (wb_vrepref *) volume( &sts, ldh_cIoConnectVolume);
  if ( ODD(sts))
    vrepref->setMerep( m_merep);
}

