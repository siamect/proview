#include <iostream.h>
#include <fstream.h>

#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_vrepwbl.h"  // Should be wb_vrepdbs.h ...
#include "wb_ldh_msg.h"

extern "C" {
#include "co_dcli.h"
#include "rt_load.h"
}

wb_erep::wb_erep() : m_dir_cnt(0)
{
  m_merep = new wb_merep(0);
}

wb_erep::~wb_erep()
{
  delete m_merep;
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
  if ( it == m_vrepdb.end()) {
    it = m_vrepdbs.find(vid);
    if ( it == m_vrepdbs.end()) {
      it = m_vrepextern.find(vid);
      if ( it == m_vrepextern.end()) {
        *sts = LDH__NOSUCHVOL;
        return 0;
      }
    }
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_vrep *wb_erep::volume(pwr_tStatus *sts, char *name) // Fix
{
  vrep_iterator it;
  for ( it = m_vrepdb.begin(); it != m_vrepdb.end(); it++) {
    // Check name
  }
  for ( it = m_vrepdbs.begin(); it != m_vrepdbs.end(); it++) {
    // Check name
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

wb_vrep *wb_erep::nextVolume(pwr_tStatus *sts, pwr_tVid vid)
{
  vrep_iterator it = m_vrepdb.find( vid);
  if ( it != m_vrepdb.end()) {
    it++;
    if ( it != m_vrepdb.end()) {
      it = m_vrepdbs.begin();
      if ( it == m_vrepdbs.end()) {
	*sts = LDH__NOSUCHVOL;
	return 0;
      }
    }
  }
  else {
    it = m_vrepdbs.find(vid);
    it++;
    if ( it == m_vrepdbs.end()) {
      *sts = LDH__NOSUCHVOL;
      return 0;
    }
  }
  *sts = LDH__SUCCESS;
  return it->second;
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

  if ( vrep->cid() == pwr_eClass_ClassVolume)
    m_merep->addDbs( sts, (wb_mvrep *)vrep);
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

void wb_erep::load( pwr_tStatus *sts)
{
  loadDirList( sts);
  if ( EVEN(*sts)) return;
  else if ( *sts == LDH__PROJCONFIG) {
    pwr_tStatus status;
    loadCommonMeta( &status);
    if ( EVEN(status))
      *sts = status;
    return;
  }
  loadCommonMeta( sts);
  if ( EVEN(*sts)) return;

  loadMeta( sts);
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

    cout << "File : " << fname << endl;
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

      nr = dcli_parse( line, " ", "", (char *)vol_array,
		       sizeof(vol_array)/sizeof(vol_array[0]),
		       sizeof(vol_array[0]), 0);
      if ( nr != 2)
	cout << "Syntax error in file: " << fname << endl;

      // Load this volume
      cdh_ToLower( vol_array[0], vol_array[0]);
      strcpy( vname, fdir);
      strcat( vname, vol_array[0]);
      strcat( vname, ".dbs");
      cdh_StringToVolumeId( vol_array[1], &vid);
      cout << "Loading volume: " << vname << " " << vid << endl;

      wb_vrepwbl *vrep = new wb_vrepwbl( this, vid);
      vrep->name( vol_array[0]);
      addDbs( &sts, vrep);
    }
    fpm.close();
  }
  *status = LDH__SUCCESS;
}

void wb_erep::loadMeta( pwr_tStatus *status)
{
  // Load local metavolumes
  char found_file[200];
  char fname[200];
  char vname[200];
  char line[200];
  int i;
  pwr_tVid vid;
  pwr_tStatus sts;
  wb_vrepwbl *vrep;
  int vol_cnt = 0;

  strcpy( fname, load_cNameVolumeList);
  dcli_translate_filename( fname, fname);

  cout << "File : " << fname << endl;

  ifstream fpm( fname, ios::in);
  if ( !fpm) {
    *status = LDH__PROJCONFIG;
    return;
  }

  while ( fpm.getline( line, sizeof(line))) {
    char vol_array[5][80];
    int nr;

    cout << line << endl;
    nr = dcli_parse( line, " ", "", (char *)vol_array,
		     sizeof(vol_array)/sizeof(vol_array[0]),
		     sizeof(vol_array[0]), 0);
    if ( nr != 4)
      cout << "Syntax error in file: " << fname << endl;

    cdh_StringToVolumeId( vol_array[1], &vid);

    if ( (cdh_NoCaseStrcmp( vol_array[2], "ClassVolume") == 0) ||
         (strcmp( vol_array[3], "load") == 0)) {
      // Load dbs for this volume
      cdh_ToLower( vol_array[0], vol_array[0]);

      if ( strcmp( vol_array[3], "cnf") == 0) {
	// Configured in this project, load from pwrp_load
	strcpy( vname, "$pwrp_load/");
        strcat( vname, vol_array[0]);
	strcat( vname, ".dbs");
        dcli_translate_filename( vname, vname);
        vol_cnt++;
      }
      else {
	// Imported loadfile
	for ( i = 0; i < m_dir_cnt; i++) {
	  strcpy( vname, m_dir_list[i]);
	  strcat( vname, vol_array[0]);
	  strcat( vname, ".dbs");
	  cout << "Try: " << vname << endl;
	  sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
	  dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
	  if ( ODD(sts)) {
	    cout << "Found: " << found_file << endl;
	    // Load...
            vrep = new wb_vrepwbl( this, vid);
            vrep->name( vol_array[0]);
            addDbs( &sts, vrep);
	    break;
	  }
	}
      }
    }
    else {
      // Load db for this volume
      strcpy( vname, "$pwrp_db/");
      strcat( vname, vol_array[0]);
      strcat( vname, ".dbs");
      dcli_translate_filename( vname, vname);

      vrep = new wb_vrepwbl( this, vid);
      vrep->name(vol_array[0]);
      addDbs( &sts, vrep);
      vol_cnt++;
    }
  }
  fpm.close();

  if ( !vol_cnt)
    *status = LDH__PROJCONFIG;
  else
    *status = LDH__SUCCESS;
}














